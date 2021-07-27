/* Práctica Final Informática Gráfica 2006
   Simulador Sistema Solar
   
   Mario Arias Gallego <mariari@lab.fi.uva.es>
   Álvaro Zubizarreta López <alvazub@lab.fi.uva.es>
*/

#define TEXTURAS_C

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "supergl.h"

#include <jpeglib.h>
#include <jerror.h>

#define PNG_DEBUG 3
#include <png.h>


#include "texturas.h"


/* Almacenamiento para las texturas */
GLuint textura[MAX_TEXTURAS];

// Fichero de la captura
char captura_name[100];


// Leer bitmaps y convertir en texturas
void CargarTexturas() {	
	int i;

	// Inicializar todas inactivas
	for(i=0;i<MAX_TEXTURAS;i++) {
		textura[i] = 0;
	}

	printf("---- Cargando texturas ----\n");
	CargarTextura(0, "texturas/mercurio.jpg");
	CargarTextura(1, "texturas/venus.jpg");
	CargarTextura(2, "texturas/tierra2.jpg");
	CargarTextura(3, "texturas/marte.jpg");
	CargarTextura(4, "texturas/jupiter.jpg");
	CargarTextura(5, "texturas/saturno.jpg");
	CargarTextura(6, "texturas/neptuno.jpg");
	CargarTextura(7, "texturas/urano.png");
	CargarTextura(8, "texturas/pluton.jpg");

	CargarTextura(10,"texturas/MercuryNormal.jpg");
	CargarTextura(11,"texturas/VenusNormal.jpg");
	CargarTextura(12,"texturas/EarthNormal.jpg");
	CargarTextura(13,"texturas/MarteNormal.jpg");

	CargarTextura(20, "texturas/sol.jpg");
	CargarTextura(21,"texturas/nubes.png");
	CargarTextura(22, "texturas/anillo.png");
};




void CargarTextura(int pos, char *fichero)
{
	// Load Texture
	Imagen imagen1;
	char *ext;

	ext = getExt(fichero);

	if( strcmp(ext, ".jpg")==0) {
		if (!cargarJPG(fichero, &imagen1)) {
			exit(1);
		}        
	} else if( strcmp(ext, ".png")==0) {
		if (!cargarPNG(fichero, &imagen1)) {
			exit(1);
		}        
	} else if( strcmp(ext, ".bmp")==0) {
		if (!cargarBMP(fichero, &imagen1)) {
			exit(1);
		}        
	} else {
		printf("Los ficheros con extension %s no están soportados como textura\n",ext);
		exit(1);
	}

	// Crear Textura
	glGenTextures(1, &textura[pos]);
	glBindTexture(GL_TEXTURE_2D, textura[pos]);

	// Escalar linealmente cuando la textura no sea del tamaño adecuado
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	// Crear MipMaps
	if(imagen1.has_alpha) {
		gluBuild2DMipmaps (GL_TEXTURE_2D, 4, imagen1.tamX, imagen1.tamY, GL_RGBA,GL_UNSIGNED_BYTE, imagen1.datos);
		//glTexImage2D(GL_TEXTURE_2D, 0, 4, imagen1.tamX, imagen1.tamY, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagen1.datos);
	} else {
		gluBuild2DMipmaps (GL_TEXTURE_2D, 3, imagen1.tamX, imagen1.tamY, GL_RGB,GL_UNSIGNED_BYTE, imagen1.datos);
		//glTexImage2D(GL_TEXTURE_2D, 0, 3, imagen1.tamX, imagen1.tamY, 0, GL_RGB, GL_UNSIGNED_BYTE, imagen1.datos);
	}

	// Liberar memoria no utilizada
	free(imagen1.datos);

    	printf("Cargada textura %s: %lu,%lu\n", fichero, imagen1.tamX, imagen1.tamY);
}


char *getExt(char *str)
{
	char *punt;

	for(punt=str; *punt; punt++);
	for(; (*punt !='.') && (punt>=str); punt--);

	if(punt<str) 
		return NULL;
	else
		return(punt);
}


/* Cargador de ficheros BMP de 24bit */
int cargarBMP(char *filename, Imagen *imagen) {
	FILE *file;
	unsigned long taman;
	unsigned long i;
	unsigned short int planos;
	unsigned short int bpp;
	char temp;

	// make sure the file is there.
	if ((file = fopen(filename, "rb"))==NULL)
	{
		printf("No se ha podido habrir el fichero de textura: %s\n",filename);
		return 0;
	}

	// seek through the bmp header, up to the width/height:
	fseek(file, 18, SEEK_CUR);

	// read the width
	if ((i = fread(&imagen->tamX, 4, 1, file)) != 1) {
		printf("Error leyendo anchura de: %s.\n", filename);
		return 0;
	}

	// read the height 
	if ((i = fread(&imagen->tamY, 4, 1, file)) != 1) {
	printf("Error reading height from %s.\n", filename);
	return 0;
	}

	imagen->has_alpha = 0;
	printf("Dimensiones de la textura %s: (%lu,%lu)\n", filename, imagen->tamX, imagen->tamY);

	// Tamaño que ocupa la imagen
	taman = imagen->tamX * imagen->tamY * 3;

	// Leer planos
	if ((fread(&planos, 2, 1, file)) != 1) {
		printf("Error leyendo planos de %s.\n", filename);
		return 0;
	}
	if (planos != 1) {
		printf("Planes from %s is not 1: %u\n", filename, planos);
		return 0;
	}

	// Leer bpp
	if ((i = fread(&bpp, 2, 1, file)) != 1) {
		printf("Error leyendo bpp de %s.\n", filename);
		return 0;
	}
	if (bpp != 24) {
		printf("Solo se admiten imágenes con BPP=24: %s\n", filename);
		return 0;
	}

	// Saltar resto de cabecera bmp
	fseek(file, 24, SEEK_CUR);

	// Reservar memoria para la imagen
	imagen->datos = (char *) malloc(taman);
	if (imagen->datos == NULL) {
		printf("Error allocating memory for color-corrected imagen datos");
		return 0;	
	}

	// Leer la imagen del fichero
	if ((i = fread(imagen->datos, taman, 1, file)) != 1) {
		printf("Error reading imagen datos from %s.\n", filename);
		return 0;
	}

	// Transformar BGR en RGB
	for (i=0;i<taman;i+=3) { 
		temp = imagen->datos[i];
		imagen->datos[i] = imagen->datos[i+2];
		imagen->datos[i+2] = temp;
	}

	return 1;
}


int cargarJPG(char *fichero, Imagen *imagen)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE *file;
	char *linea;
	unsigned long taman;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	// Abrir fichero
	if ((file=fopen(fichero,"rb"))==NULL)
	{
		printf("Error: Imposible abrir el fichero para lectura: %s\n", fichero);
		return 0;
	}
	
	// Leer cabecera
	jpeg_stdio_src(&cinfo, file);
	jpeg_read_header(&cinfo, TRUE);

	// Comprobar formato 
	if (cinfo.jpeg_color_space==JCS_GRAYSCALE)
	{
		fprintf(stdout,"Error: Las texturas deben estar en formato RGB");
		return 0;
	}

    	// Calcular tamaño
	imagen->tamX = cinfo.image_width;
	imagen->tamY = cinfo.image_height;
	imagen->has_alpha = 0;
	taman = imagen->tamX * imagen->tamY * 3;


	// Reservar memoria para la imagen 
	imagen->datos = (char *) malloc(taman);
	if (imagen->datos == NULL) {
		printf("Error allocating memory for color-corrected imagen datos");
		return 0;	
	}

	// Leer imagen del fichero
	jpeg_start_decompress(&cinfo);
	linea=imagen->datos;
	while (cinfo.output_scanline<cinfo.output_height)
	{
		linea=imagen->datos+3*imagen->tamX*cinfo.output_scanline;
		jpeg_read_scanlines(&cinfo,(JSAMPARRAY) &linea,1);
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	return 1;
}
    


int cargarPNG(char* fichero, Imagen *imagen)
{
	unsigned int x, y, count;
	png_byte color_type;
	png_byte bit_depth;

	png_structp png_ptr;
	png_byte* ptr;
	png_infop info_ptr;
	int number_of_passes;
	png_bytep * row_pointers;
	unsigned char header[8];	

	// Abrir fichero 
	FILE *fp = fopen(fichero, "rb");
	if (!fp) {
		printf("[read_png_file] File %s could not be opened for reading", fichero);
		return 0;
	}

	fread(header, 1, 8, fp);
	if (png_sig_cmp(header, 0, 8)) {
		printf("[read_png_file] File %s is not recognized as a PNG file", fichero);
		return 0;
	}


	/* initialize stuff */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	
	if (!png_ptr) {
		printf("[read_png_file] png_create_read_struct failed");
		return 0;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		printf("[read_png_file] png_create_info_struct failed");
		return 0;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		printf("[read_png_file] Error during init_io");
		return 0;
	}

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	imagen->tamX = info_ptr->width;
	imagen->tamY = info_ptr->height;
	color_type = info_ptr->color_type;
	bit_depth = info_ptr->bit_depth;

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
	      png_set_expand(png_ptr);

	if (info_ptr->color_type == PNG_COLOR_TYPE_RGBA){
		imagen->has_alpha = 1;
	} else {
		imagen->has_alpha = 0;
	}

	number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);


	/* read file */
	if (setjmp(png_jmpbuf(png_ptr))) {
		printf("[read_png_file] Error during read_image");
		return 0;
	}

	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * imagen->tamY);
	for (y=0; y< imagen->tamY; y++)
		row_pointers[y] = (png_byte*) malloc(info_ptr->rowbytes);

	png_read_image( png_ptr , row_pointers);

	/* Copy to data buffer */
	if(imagen->has_alpha) {
    		imagen->datos = (char *) malloc( 4 * imagen->tamX * imagen->tamY );
	} else {
    		imagen->datos = (char *) malloc( 3 * imagen->tamX * imagen->tamY );
	}

	count=0;
	for (y=0; y<imagen->tamY; y++) {
		png_byte* row = row_pointers[y];
		for (x=0; x<imagen->tamX; x++) {
			if(imagen->has_alpha) {
				ptr = &(row[x*4]);

				/* Save value */
				imagen->datos[count++] = ptr[0];
				imagen->datos[count++] = ptr[1];
				imagen->datos[count++] = ptr[2];
				imagen->datos[count++] = ptr[3];
			} else {
				ptr = &(row[x*3]);

				/* Save value */
				imagen->datos[count++] = ptr[0];
				imagen->datos[count++] = ptr[1];
				imagen->datos[count++] = ptr[2];
			}

		}
	}

        fclose(fp);

	// Liberar memoria
	for (y=0; y< imagen->tamY; y++)
		free(row_pointers[y]);
	free(row_pointers); 

	return(1);
}

// Tomar captura de pantalla
char *capturar() {
	int i;
	FILE *file;

	for(i=0; i<MAXCAP; i++) {
	#if defined(__APPLE__) || defined(MACOSX)
		sprintf(captura_name,"%s/Desktop/%s%03d.jpg", getenv("HOME"),CAPNAME,i);
	#else	
		sprintf(captura_name,"%s%03d.jpg", CAPNAME,i);
	#endif
		if ( (file = fopen(captura_name, "r")) )
		{
			fclose(file);
		} else {
			capturaJPG(captura_name, 100);
			return(captura_name);
		}
	}
	return NULL;
}

// Capturar y guardar en JPG
void capturaJPG(char *filename, int calidad)
{
	FILE *fichero;
	int tamx,tamy;
	unsigned char *pixelData;
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
	int row_stride;		/* physical row width in image buffer */

	tamx = glutGet (GLUT_WINDOW_WIDTH);
	tamy = glutGet (GLUT_WINDOW_HEIGHT);

  	// Reservar memoria
	pixelData  = (unsigned char *) malloc( 3*tamx*tamy );

	// Leer Buffer de color de opengl.
	glReadPixels(0, 0, tamx, tamy, GL_RGB,GL_UNSIGNED_BYTE, pixelData); 

	// Comprimir en JPG

	/* Step 1: allocate and initialize JPEG compression object */
	cinfo.err = jpeg_std_error(&jerr);

	/* Now we can initialize the JPEG compression object. */
	jpeg_create_compress(&cinfo);

	/* Step 2: specify data destination (eg, a file) */
	if ((fichero = fopen(filename, "wb")) == NULL) {
		fprintf(stderr, "Error abriendo fichero %s para guardar la captura.\n", filename);
		return;
	}
	jpeg_stdio_dest(&cinfo, fichero);

	/* Step 3: set parameters for compression */
	cinfo.image_width = tamx; 	/* image width and height, in pixels */
	cinfo.image_height = tamy;
	cinfo.input_components = 3;		/* # of color components per pixel */
	cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, calidad, TRUE);  /* limit to baseline-JPEG values */

	/* Step 4: Start compressor */
	jpeg_start_compress(&cinfo, TRUE);

	row_stride = cinfo.image_width * 3;	/* JSAMPLEs per row in image_buffer */

	while (cinfo.next_scanline < cinfo.image_height) {
		row_pointer[0] = & pixelData[ (tamy-1)*row_stride-cinfo.next_scanline * row_stride];
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	/* Step 6: Finish compression */
	jpeg_finish_compress(&cinfo);

	/* After finish_compress, we can close the output file. */
	fclose(fichero);

	jpeg_destroy_compress(&cinfo);

	// Liberar recursos
	free( pixelData );

	printf("Guardada captura %s de tamaño: %dx%d\n", filename,tamx, tamy);
}
