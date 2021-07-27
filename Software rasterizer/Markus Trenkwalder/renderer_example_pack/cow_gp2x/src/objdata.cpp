/*
Copyright (c) 2007, 2008 Markus Trenkwalder

All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, 
this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation 
and/or other materials provided with the distribution.

* Neither the name of the library's copyright owner nor the names of its 
contributors may be used to endorse or promote products derived from this 
software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "objdata.h"

#include <fstream>
#include <string>
#include <sstream>

#include <iostream>
#include <map>

using namespace std;

typedef vmath::vec3<float> vec3f;
typedef vmath::vec2<float> vec2f;

// string is supposed to be in the form <vertex_index>/<texture_index>/<normal_index>, 
void extract_indices_from_string(string str, unsigned &vi, unsigned &ni, unsigned &ti)
{
	string v;
	string t;
	string n;

	string *tmp[3] = {&v, &t, &n};
	
	int tidx = 0;
	string *target = tmp[0];
	
	for (size_t i = 0; i < str.size(); ++i) {
		if (str[i] == '/') target = tmp[++tidx];
		else *target += str[i];
	}

	istringstream iss1(v);
	vi = 0;
	iss1 >> vi;

	istringstream iss2(t);
	ti = 0;
	iss2 >> ti;

	istringstream iss3(n);
	ni = 0;
	iss3 >> ni;
}

ObjData ObjData::load_from_file(const char *filename)
{
	ObjData result;
	result.vertices.push_back(vec3f(0.0f));
	result.normals.push_back(vec3f(0.0f));
	result.texcoords.push_back(vec2f(0.0));

	ifstream in(filename, ios::in);
	while (in) {
		string line;
		getline(in, line);
		istringstream iss;
		iss.str(line);
		string cmd;
		iss >> cmd;
		
		if (cmd[0] == '#') continue;
		else if (cmd == "v") {
			vec3f v;
			iss >> v.x >> v.y >> v.z;
			result.vertices.push_back(v);
		} else if (cmd == "vn") {
			vec3f v;
			iss >> v.x >> v.y >> v.z;
			result.normals.push_back(v);
		} else if (cmd == "vt") {
			vec2f t;
			iss >> t.x >> t.y;
			result.texcoords.push_back(t);
		} else if (cmd == "f") {
			result.faces.push_back(ObjData::Face());
			ObjData::Face &face = result.faces.back();
			
			while (iss) {
				string word;
				iss >> word;

				if (word == "") continue;
				
				ObjData::VertexRef vr;
				extract_indices_from_string(word, vr.vertex_index, vr.normal_index, vr.texcoord_index);

				face.push_back(vr);
			}
		}
	}

	return result;
}

namespace internal {
	struct VertexRefCompare {
		bool operator () (const ObjData::VertexRef &v1, const ObjData::VertexRef &v2) const
		{
			if (v1.vertex_index < v2.vertex_index) return true;
			else if (v1.vertex_index > v2.vertex_index) return false;
			else {
				if (v1.normal_index < v2.normal_index) return true;
				else if (v1.normal_index > v2.normal_index) return false;
				else {
					if (v1.texcoord_index < v2.texcoord_index) return true;
					else return false;
				}
			}
		}
	};
}

using namespace internal;

void ObjData::to_vertex_array(std::vector<VertexArrayData> &vdata, std::vector<unsigned> &idata)
{
	vdata.clear();
	idata.clear();

	struct Helper {
		vector<VertexArrayData> &vdata_;
		vector<unsigned> &idata_;
		const ObjData &obj_;

		typedef map<VertexRef, unsigned, internal::VertexRefCompare> vertex_index_map_t;
		vertex_index_map_t vertex_index_map;

		Helper(const ObjData& obj, std::vector<VertexArrayData> &vdata, std::vector<unsigned> &idata):
			vdata_(vdata), idata_(idata), obj_(obj) {}

		void process() 
		{
			for (size_t i = 0; i < obj_.faces.size(); ++i) {
				unsigned i1 = add_vertex(obj_.faces[i][0]);

				// make a triangle fan if there are more than 3 vertices
				for (size_t j = 2; j < obj_.faces[i].size(); ++j) {
					unsigned i2 = add_vertex(obj_.faces[i][j-1]);
					unsigned i3 = add_vertex(obj_.faces[i][j]);

					add_face(i1, i2, i3);
				}
			}
		}

		unsigned add_vertex(const VertexRef &v) 
		{
			// check if this vertex already exists
			vertex_index_map_t::const_iterator it = vertex_index_map.find(v);
			if (it != vertex_index_map.end()) 
				return it->second;

			// does not exist, so insert a new one
			unsigned i = static_cast<unsigned>(vertex_index_map.size());
			vertex_index_map.insert(make_pair(v, i));

			VertexArrayData vd;
			vd.vertex = obj_.vertices[v.vertex_index];
			vd.normal = obj_.normals[v.normal_index];
			vd.texcoord = obj_.texcoords[v.texcoord_index];
			vdata_.push_back(vd);

			return i;
		}

		void add_face(unsigned i1, unsigned i2, unsigned i3) 
		{
			idata_.push_back(i1);
			idata_.push_back(i2);
			idata_.push_back(i3);
		}

	} helper(*this, vdata, idata);

	helper.process();
}
