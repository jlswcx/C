
#include "c_jpeglib.h"

#include <jpeglib.h>
#pragma comment(lib, "jpeg.lib")


void c_jpeg_bmp2jpg_from_buffer(long _width, long _height, unsigned short _bitcount, char* _bmpimagedata, const char* _to_filename, int _quality)
{
	FILE * outfile;				/* target file */
	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
	int row_stride;				/* physical row width in image buffer */

	int pix_depth = _bitcount/8;

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err=jpeg_std_error(&jerr);        //libjpeg各种配置
    jpeg_create_compress(&cinfo);

	if ((outfile = fopen(_to_filename, "wb")) == NULL) {
		fprintf(stderr, "can't open %s\n", _to_filename);
		exit(1);
	}

    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = _width;
    cinfo.image_height = _height;
    cinfo.input_components = pix_depth;
	if(pix_depth == 1)
		cinfo.in_color_space = JCS_GRAYSCALE;
	else
		cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, _quality, TRUE);   // 中间的值为压缩质量，越大质量越好
    jpeg_start_compress(&cinfo, TRUE);

	row_stride = _width * pix_depth;			// JSAMPLEs per row in image_buffer

//	buffer=(*cinfo.mem->alloc_sarray)
//			((j_common_ptr)&cinfo,JPOOL_IMAGE,_width * pix_depth,1);

	while (cinfo.next_scanline < cinfo.image_height) {
		/* jpeg_write_scanlines expects an array of pointers to scanlines.
		 * Here the array is only one element long, but you could pass
		 * more than one scanline at a time if that's more convenient.
		*/
		row_pointer[0] = &_bmpimagedata[cinfo.next_scanline * row_stride];
		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

    jpeg_finish_compress(&cinfo);
	fclose(outfile);
    jpeg_destroy_compress(&cinfo);
}


void c_jpeg_bmp2jpg_from_file(const char* _from_filename, const char* _to_filename)
{

}