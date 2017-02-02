#include "ply.h"
#include "math3d++.h"

#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <cassert>

using namespace std;
using namespace math3d;

// Definition of various types

struct Face {
	int i[3];				//!< the vertex indices
};

struct AABB {
	vec3 min;
	vec3 max;
};

struct KdTreeNode {
	unsigned splitaxis;
	unsigned splitpos;
	KdTreeNode *pchild;
	KdTreeNode *nchild;
	vector<Face*> facerefs;
};

struct PartitionInfo {
  bool dosplit;
	unsigned splitpos;
	vector<Face*> pfaces;
	vector<Face*> nfaces;
	vector<Face*> cfaces;
};

struct SplitPositionInfo {
  bool min_is_valid; //!< if true min can be used to cut off empty space
  bool max_is_valid; //!< if true max can be used to cut off empty space
  unsigned min; //!< minimum split position to cut off empty space
  unsigned max; //!< maximum split position to cut off empty space
  unsigned med; //!< median split position (always valid)
};

const unsigned PF_MAX_SPLITPOS = (1<<30)-1;

// The objects variables

static vector<vec3> vlist;
static vector<Face> flist;

static AABB object_aabb;
static KdTreeNode root;

// configuration data passed via the command line
struct {
	char *input_filename;
	char *output_filename;
	int max_treedepth;
	unsigned n0;
	float q;
	float scale;
} config_data;

// Variables for statistics

static unsigned stat_max_treedepth = 0;
static unsigned stat_num_median_split = 0;
static unsigned stat_num_center_split = 0;
static unsigned stat_num_cutoff_split = 0;

// Functions

void swap32(float *f)
{
	unsigned x = *reinterpret_cast<unsigned*>(f);
	x = ((x<<24)|((x<<8)&0x00ff0000)|((x>>8)&0x0000ff00)|(x>>24));
	*f = *reinterpret_cast<float*>(&x);
}

void swap32(int *i)
{
	unsigned x = *reinterpret_cast<unsigned*>(i);
	x = ((x<<24)|((x<<8)&0x00ff0000)|((x>>8)&0x0000ff00)|(x>>24));
	*i = *reinterpret_cast<int*>(&x);
}

void read_file(FILE *f)
{
	int i,j;
	int elem_count;
	char *elem_name;
	PlyFile *in_ply;
	
	struct PlyVertex {
		float x,y,z;
	};

	struct PlyFace {
		unsigned char nverts;    /* number of vertex indices in list */
		int *verts;              /* vertex index list */
	};

	PlyProperty vert_props[] = { /* list of property information for a vertex */
		{"x", Float32, Float32, offsetof(PlyVertex,x), 0, 0, 0, 0},
		{"y", Float32, Float32, offsetof(PlyVertex,y), 0, 0, 0, 0},
		{"z", Float32, Float32, offsetof(PlyVertex,z), 0, 0, 0, 0},
	};

	PlyProperty face_props[] = { /* list of property information for a face */
		{"vertex_indices", Int32, Int32, offsetof(PlyFace,verts),
		 1, Uint8, Uint8, offsetof(PlyFace,nverts)},
	};
	
	/*** Read in the original PLY object ***/

	in_ply = read_ply(f);
	
	for ( i = 0; i < in_ply->num_elem_types; ++i ) {
		
		/* prepare to read the i'th list of elements */
		elem_name = setup_element_read_ply(in_ply, i, &elem_count);
		
		if ( equal_strings("vertex", elem_name) ) {
			
			/* reserve enough space to holt all vertices */
			vlist.reserve(elem_count);

			/* set up for getting vertex elements */
			/* (we want x,y,z) */

			setup_property_ply(in_ply, &vert_props[0]);
			setup_property_ply(in_ply, &vert_props[1]);
			setup_property_ply(in_ply, &vert_props[2]);
			
			/* grab the vertex elements and store them in our list */

			for ( j = 0; j < elem_count; j++ ) {
				PlyVertex v;
				get_element_ply(in_ply, (void*)&v);
				
				/* convert and add the vertex to our vertex vector */
				vlist.push_back(vec3(v.x, v.y, v.z));
			}
		}
		else if ( equal_strings("face", elem_name) ) {

			/* reserve enough space to hold all triangles */
			flist.reserve(elem_count);
			
			/* set up for getting face elements */
			/* (all we need are vertex indices) */

			setup_property_ply(in_ply, &face_props[0]);

			/* grab all the face elements and place them in our list */

			for ( j = 0; j < elem_count; j++ ) {
				PlyFace f;
				get_element_ply(in_ply, (void*)&f);
				
				/* sanity check */
				if ( f.nverts != 3 ) {
					fprintf(stderr, "Problem: Face does not have 3 vertices.");
					exit(-1);
				}
				
				/* convert and add the face to the face vector */
				Face face;
				face.i[0] = f.verts[0];
				face.i[1] = f.verts[1];
				face.i[2] = f.verts[2];
				
				flist.push_back(face);
			}
		}
	}
	
	/* We are ony a little endian machine and the file is big endian. */
	/* Go through all vertices and faces and convert accordingly. */
	if ( in_ply->file_type == PLY_BINARY_BE ) {
		for ( unsigned i = 0; i < vlist.size(); ++i ) {
			vec3& v = vlist[i];
			swap32(&v[0]);
			swap32(&v[1]);
			swap32(&v[2]);
		}
		
		for ( unsigned i = 0; i < flist.size(); ++i ) {
			Face& f = flist[i];
			swap32(&f.i[0]);
			swap32(&f.i[1]);
			swap32(&f.i[2]);
		}
	}

	free_ply (in_ply);
}

int face_side(const Face& face, const vec4& plane)
{
	int a = 0;
	
	const vec3& v1 = vlist[face.i[0]];
	const vec3& v2 = vlist[face.i[1]];
	const vec3& v3 = vlist[face.i[2]];
	
	// determine on which side of the plane each vertex lies
	if ( dot(vec4(v1, 1.0f), plane) > 0 ) ++a;
	if ( dot(vec4(v2, 1.0f), plane) > 0 ) ++a;
	if ( dot(vec4(v3, 1.0f), plane) > 0 ) ++a;
	
	if ( a == 0 ) return -1;
	else if ( a == 3 ) return 1;
	else return 0;
}

void compute_aabb(const vector<Face>& faces, AABB *aabb)
{
	assert(aabb && "This cannot be NULL!");
	
	// set the bounding box to an existing vertex
	aabb->min = aabb->max = vlist[faces[0].i[0]];
	
	// loop through all the face's vertices
	for ( unsigned i = 0; i < faces.size(); ++i ) {
		for ( int j = 0; j < 3; ++j ) {
			const vec3& v = vlist[faces[i].i[j]];
			
			aabb->min[0] = min(aabb->min[0], v[0]);
			aabb->min[1] = min(aabb->min[1], v[1]);
			aabb->min[2] = min(aabb->min[2], v[2]);
			
			aabb->max[0] = max(aabb->max[0], v[0]);
			aabb->max[1] = max(aabb->max[1], v[1]);
			aabb->max[2] = max(aabb->max[2], v[2]);
		}
	}
}

float splitpos_u2f(const AABB& aabb, unsigned axis, unsigned p)
{
	float d = (aabb.max[axis] - aabb.min[axis]);
	d *= (float)p / PF_MAX_SPLITPOS;
	d += aabb.min[axis];
	return d;
}

unsigned splitpos_f2u(const AABB& aabb, unsigned axis, float p)
{	
	// if the following two variables are not doubles numerical errors can
  // result in missing triangles in the raytracer's rendering!
	const double l = aabb.max[axis] - aabb.min[axis];
  const double f = PF_MAX_SPLITPOS / l;
  
  return static_cast<unsigned>(floor((p - aabb.min[axis]) * f));
}

void split_aabb(
	const AABB& aabb,
	unsigned axis,
	unsigned splitpos,
	AABB *naabb,
	AABB *paabb
)
{
	float d = splitpos_u2f(aabb, axis, splitpos);
	
	(*naabb) = (*paabb) = aabb;
	naabb->max[axis] = d;
	paabb->min[axis] = d;
}

float surface_area(const AABB& aabb)
{
  const float dx = aabb.max[0] - aabb.min[0];
  const float dy = aabb.max[1] - aabb.min[1];
  const float dz = aabb.max[2] - aabb.min[2];
  
  return (2 * dx * dy) + (2 * dy * dz) + (2 * dx * dz);
}

SplitPositionInfo compute_splitpositions(
  const vector<Face*> faces,
  const AABB& aabb,
  unsigned axis
)
{
  SplitPositionInfo result;

  // compute some metrics about the triangle's positions in space to be able to
  // choose a good partitioning plane.
	float min, max, median;

  // the copying of the coordinates is necessary for the median search.
  // this can be quite expensive, when the 3d model is really big.
  vector<float> coords;

  min = max = vlist[faces[0]->i[0]][axis];
  
  // search min and max and copy coordinates to the coords vector
  coords.reserve(faces.size() * 3);
  for ( unsigned i = 0; i < faces.size(); ++i ) {
    for ( int j = 0; j < 3; ++j ) {
      const float v = vlist[faces[i]->i[j]][axis];
      min = std::min(min, v);
      max = std::max(max, v);
      coords.push_back(v);
    }
  }
  
  // determine the median
  nth_element(coords.begin(), coords.begin() + coords.size()/2, coords.end());
  median = coords[coords.size()/2];
  
  // the min and max positions are only valid if they lie inside the AABB
  result.min_is_valid = min > aabb.min[axis];
  result.max_is_valid = max < aabb.max[axis];
  
  // the split positions must be expressed as an unsigned integer
  
  result.min = splitpos_f2u(aabb, axis, min);
	result.max = splitpos_f2u(aabb, axis, max) + 1;
	result.med = splitpos_f2u(aabb, axis, median);
	
  if ( result.min > PF_MAX_SPLITPOS ) result.min = 0;
  if ( result.max > PF_MAX_SPLITPOS ) result.max = PF_MAX_SPLITPOS;
  
  return result;
}

float evaluate_split_cost(
	const AABB& aabb,  //!< Enclosing ABBB
	unsigned splitpos, //!< position of the split
	unsigned axis,  //!< axis of the split
	unsigned c_np,	//!< number of triangles in the negative half space
	unsigned c_nn,	//!< number of triangles in the positive half space
	unsigned c_nsp  //!< number of triangles that cross the spliting plane
)
{
	struct Helper {
		static float f(float n)
		{
      const float N0 = config_data.n0;
      const float Q = config_data.q;
      
			if ( n < N0 ) return n;
			else return std::pow(n - N0, 1 + log2(Q)) + N0;
		}
		
		static float log2(float x)
		{
			return log(x)/log(2.0f);
		}
	};
	
	AABB naabb, paabb;
	
	split_aabb(aabb, axis, splitpos, &naabb, &paabb);
	
	const float np  = Helper::f(c_np);
	const float nn  = Helper::f(c_nn);
	const float nsp = Helper::f(c_nsp);
	
	const float saa = surface_area(aabb);
	const float san = surface_area(naabb);
	const float sap = surface_area(paabb);
	
	return (san * (nn + nsp) + sap * (np + nsp)) / saa;
}

void do_partitioning(
	const vector<Face*> faces,
	const AABB& aabb,
	unsigned axis,
	unsigned splitpos,
	PartitionInfo *result
)
{
	result->dosplit = true;
	result->splitpos = splitpos;
	
	const float d = splitpos_u2f(aabb, axis, splitpos);
	
	// static array for the three different planes (makes it faster)
	static vec4 planes[] = {
		vec4(1, 0, 0, 0),
		vec4(0, 1, 0, 0),
		vec4(0, 0, 1, 0)
	};
	
	// the only parameter that changes for the planes is
	// the distance from the origin
	
	planes[axis][3] = -d;
	
	// categorize all the faces in the face list to lie either in
	// the positive or negative half space or cross the plane
	
	for ( unsigned i = 0; i < faces.size(); ++i ) {
		Face *f = faces[i];
		
		int fs = face_side(*f, planes[axis]);
		
		switch ( fs ) {
		case -1:
			result->nfaces.push_back(f);
			break;
		case 0:
			result->cfaces.push_back(f);
			break;
		case 1:
			result->pfaces.push_back(f);
			break;
		}
	}
}

PartitionInfo partition_faces(
	const vector<Face*> faces,
	const AABB& aabb,
	unsigned axis
)
{
  SplitPositionInfo spinfo = compute_splitpositions(faces, aabb, axis);
  
	PartitionInfo pmedian, pcenter;
	
	do_partitioning(faces, aabb, axis, spinfo.med, &pmedian);
	do_partitioning(faces, aabb, axis, (PF_MAX_SPLITPOS>>1), &pcenter);
	
	// evaluate the cost of each split and choose the best one
	
	const float cnone = faces.size();
	
	const float cmed = evaluate_split_cost(aabb, pmedian.splitpos, axis, pmedian.pfaces.size(),
		pmedian.nfaces.size(), pmedian.cfaces.size());
	const float cmid = evaluate_split_cost(aabb, pcenter.splitpos, axis, pcenter.pfaces.size(),
		pcenter.nfaces.size(), pcenter.cfaces.size());		
		
	float cmin = numeric_limits<float>::infinity();
	float cmax = numeric_limits<float>::infinity();		
		
	if ( spinfo.min_is_valid )
		cmin = evaluate_split_cost(aabb, spinfo.min, axis, faces.size(), 0, 0);
	if ( spinfo.max_is_valid )
		cmax = evaluate_split_cost(aabb, spinfo.max, axis, 0, faces.size(), 0);
	
	// choose the best split
	
	const float splitcosts[5] = {cmin, cmax, cmed, cmid, cnone};
	
	float min_v = splitcosts[0];
	int   min_i = 0;
	
	for ( int i = 0; i < 5; ++i ) {
		if ( splitcosts[i] < min_v ) {
			min_v = splitcosts[i];
			min_i = i;
		}
	}
		
	PartitionInfo result;
	result.dosplit = true;
	
	if ( min_i == 0 ) {
		// minimum is cmin
    result.splitpos = spinfo.min;
    result.nfaces.clear();
    result.cfaces.clear();
    result.pfaces = faces;
		
		++stat_num_cutoff_split;
	}
	else if ( min_i == 1 ) {
		// minimum is cmax
    result.splitpos = spinfo.max;
    result.pfaces.clear();
    result.cfaces.clear();
    result.nfaces = faces;
		
		++stat_num_cutoff_split;
	}
	else if ( min_i == 2 ) {
		// minimum is cmed
    result = pmedian;
		
		++stat_num_median_split;
	}
	else if ( min_i == 3 ) {
		// minimum is cmid
		result = pcenter;
		
		++stat_num_center_split;
	}
	else if ( min_i == 4 ) {
		// minimum is cnone
    result.dosplit = false;
	}
	else {
		assert(!"This can not happen at all!");
	}
	
	return result;
}

struct KdTreeBuildParams {
	unsigned mintriangles;
	unsigned maxdepth;
};

void build_kd_tree(
	KdTreeNode *node,
	const KdTreeBuildParams& params,
	const AABB& aabb,
	unsigned currentdepth,
	unsigned splitaxis
)
{
	stat_max_treedepth = std::max(stat_max_treedepth, currentdepth);
	
	// test for termination criteria
	if ( node->facerefs.size() <= params.mintriangles ) return;
	if ( currentdepth >= params.maxdepth ) return;
	
	PartitionInfo pinfo = partition_faces(node->facerefs, aabb, splitaxis);
	
	// if the subdivision would yield undesired result bail out
	// todo: implement some criteria
	if ( !pinfo.dosplit )
    return;
	
	// transform the current node from a leaf node to an internal node
	node->splitaxis = splitaxis;
	node->splitpos = pinfo.splitpos;
	
	// make sure the now unneeded vector is cleared
	vector<Face*>().swap(node->facerefs);
	
	// build the child nodes
	KdTreeNode *n;

	n = new KdTreeNode;
	n->splitaxis = 3;
	n->pchild = 0;
	n->nchild = 0;
	n->facerefs.swap(pinfo.pfaces);
	
	// the faces that cross the partition plane are also put in this node
	n->facerefs.insert(n->facerefs.end(),pinfo.cfaces.begin(),pinfo.cfaces.end());

	node->pchild = n;
	
	n = new KdTreeNode;
	n->splitaxis = 3;
	n->pchild = 0;
	n->nchild = 0;
	n->facerefs.swap(pinfo.nfaces);
	
	// the faces that cross the partition plane are also put in this node
	n->facerefs.insert(n->facerefs.end(),pinfo.cfaces.begin(),pinfo.cfaces.end());
	
	node->nchild = n;
	
	// make sure the now unneeded vector is actually cleared to save memory
	vector<Face*>().swap(pinfo.cfaces);
	
	// continue building the kd tree by recursing into the child nodes.
	// we have to compute the AABBs for each node because partition_faces()
	// needs one.
	
	AABB naabb, paabb;
	split_aabb(aabb, splitaxis, pinfo.splitpos, &naabb, &paabb);
	
	build_kd_tree(node->nchild,params,naabb,currentdepth + 1,(splitaxis + 1) % 3);
	build_kd_tree(node->pchild,params,paabb,currentdepth + 1,(splitaxis + 1) % 3);
}

void write_node(const KdTreeNode *node, FILE *f)
{
	// structure used to hold the information which will be written to file
	struct {
		unsigned splitaxis : 2;
		unsigned sp_or_tc  : 30;
	} file_node;

	assert(sizeof(file_node) == 4);
	
	file_node.splitaxis = node->splitaxis;
	file_node.sp_or_tc =
		node->splitaxis != 3 ? node->splitpos : node->facerefs.size();
		
	fwrite(&file_node, sizeof(file_node), 1, f);
	
	if ( node->splitaxis == 3 ) {
		for ( unsigned i = 0; i < node->facerefs.size(); ++i )
			fwrite(node->facerefs[i], sizeof(Face), 1, f);
	}
	else {
		assert(node->nchild && node->pchild && "Both must be non NULL");
		write_node(node->nchild, f);
		write_node(node->pchild, f);
	}
}

void write_file(FILE *f)
{
	// write the bounding box
	fwrite(&object_aabb, sizeof(object_aabb), 1, f);
	
	// number of vertices
	unsigned vlist_size = vlist.size();
	fwrite(&vlist_size, sizeof(vlist_size), 1, f);
	
	// the vertices themself
	fwrite(&vlist[0], sizeof(vec3), vlist_size, f);
	
	write_node(&root, f);
}

void scale_vertices(float factor)
{
	for ( unsigned i = 0; i < vlist.size(); ++i )
		vlist[i] *= factor;
}

void usage()
{
  printf(
    "plyconvert filename_in [options]\n"
    "Options:\n"
    "  -o  <file>    Place the output into <file> (default is 'output.mesh').\n"
    "  -d  <depth>   Build the tree with a maximum depth of <depth>\n"
    "                (default value is determined automatically).\n"
    "  -n0 <int>     Only try subdividing if there are more than n0 triangles\n"
    "                in a node.\n"
    "  -q  <float>   Used for a non linear cost estimate when building the kd-tree.\n"
    "                A value of 1.0 makes the cost esimate linear. Good values\n"
    "                are 0.90, 0.95 or 0.98. The default is 0.98.\n"
    "  -s <float>    Prescale the input mesh by the given factor.\n"
  );
}

void process_command_line(int argc, char* argv[]) {
	// set all values to default
  config_data.input_filename = NULL;
	config_data.output_filename = "output.mesh";
	config_data.max_treedepth = -1; // -1 = automatic
	config_data.n0 = 4;
	config_data.q = 0.98f;	
	config_data.scale = 1.0f;
	
	// at least the input filename must be given
	if ( argc < 2 ) {
		usage();
		exit(-1);
	}
	
	config_data.input_filename = argv[1];
	
	argc -= 2;
	argv += 2;
	
	// helper macro to save typing
	#define ADVANCE_ARG_AND_CHECK() \
	{\
		--argc; ++argv;\
		if ( !argc ) {\
			usage();\
			exit(-1);\
		}\
	}
	
	while ( argc > 0 ) {
		if ( strcmp(argv[0], "-o") == 0) {
			ADVANCE_ARG_AND_CHECK();
			config_data.output_filename = argv[0];
		}
		else if ( strcmp(argv[0], "-d") == 0 ) {
			ADVANCE_ARG_AND_CHECK();
			int value = atoi(argv[0]);
			if ( value < 0 ) value = 0;
			config_data.max_treedepth = value;
		}
    else if ( strcmp(argv[0], "-n0") == 0 ) {
			ADVANCE_ARG_AND_CHECK();
			int value = atoi(argv[0]);
			if ( value < 1 ) value = 1;
			config_data.n0 = value;
		}
    else if ( strcmp(argv[0], "-q") == 0 ) {
			ADVANCE_ARG_AND_CHECK();
			float value = atof(argv[0]);
			if ( value < 0.0f ) value = 0.0f;
			if ( value > 1.0f ) value = 1.0f;
			config_data.q = value;
		}
    else if ( strcmp(argv[0], "-s") == 0 ) {
			ADVANCE_ARG_AND_CHECK();
			float value = atof(argv[0]);
			config_data.scale = value;
		}
		else {
			usage();
			exit(-1);
		}
		
		--argc; ++argv;
	}
	#undef ADVANCE_ARG_AND_CHECK
}

int main(int argc, char* argv[])
{
  process_command_line(argc, argv);
  
	FILE *f = fopen(config_data.input_filename, "rb");
	if ( !f ) {
    printf("Cannot open file\n");
    exit(-1);
	}
	printf("reading file\n");
	read_file(f);
	fclose(f);

	printf("  nvertices : %u\n", vlist.size());
	printf("  nfaces    : %u\n", flist.size());
	
	if ( config_data.scale != 1.0f ) {
    printf("\nscaling vertices\n");
    printf("  scale factor : %.2f\n", config_data.scale);	
    scale_vertices(config_data.scale);
	}
	
	printf("\ncomputing bounding box\n");
	compute_aabb(flist, &object_aabb);
	const AABB& o = object_aabb;
	printf("  min : %9.3f %9.3f %9.3f\n", o.min[0], o.min[1], o.min[2]);
	printf("  max : %9.3f %9.3f %9.3f\n", o.max[0], o.max[1], o.max[2]);
	
	// construct the kd tree root node
	root.splitaxis = 3;
	root.pchild = 0;
	root.nchild = 0;
	root.facerefs.resize(flist.size());
	for ( unsigned i = 0; i < root.facerefs.size(); ++i )
		root.facerefs[i] = &flist[i];
	
	// build the kd tree
	KdTreeBuildParams params;
	params.mintriangles = config_data.n0;
	params.maxdepth = config_data.max_treedepth == -1 ?
    (unsigned)ceil(log((float)flist.size())/log(2.0f)) :
    config_data.max_treedepth;
	
	printf("\nconstructing kd-tree\n");
	printf("  maxdepth : %u\n", params.maxdepth);
	printf("  n0       : %u\n", config_data.n0);
	printf("  q        : %.2f\n", config_data.q);
	build_kd_tree(&root, params, object_aabb, 0, 0);
	
	printf("\nstatistics\n");
  printf("  max treedepth : %u\n", stat_max_treedepth);
	printf("  median splits : %u\n", stat_num_median_split);
	printf("  center splits : %u\n", stat_num_center_split);
	printf("  cutoff splits : %u\n", stat_num_cutoff_split);
	
	printf("\nsaving to file\n");
	// write the result to the output file
	f = fopen(config_data.output_filename, "wb");
	if ( !f ) {
    printf("Cannot open output file\n");
    exit(-1);
  }
	write_file(f);
	fclose(f);
	printf("done\n");
}
