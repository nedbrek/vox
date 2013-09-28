#include <png.h>
#include <cassert>
#include <iostream>

unsigned char* loadPngTexture(const char *filename, int *width, int *height,
  bool *hasAlpha)
{
	assert(filename && width && height && hasAlpha);

	FILE *fp = fopen(filename, "rb");
	if (!fp)
	{
		std::cerr << "PNG loader: Cannot open file " << std::string(filename)
		  << std::endl;
		return NULL;
	}

	// read 8 bytes
	png_byte header[8];
	if (fread(header, 1, 8, fp) != 8)
	{
		std::cerr << "PNG loader: File " << std::string(filename)
		  << " is too short." << std::endl;
		return NULL;
	}

	// check signature
	if (png_sig_cmp(header, 0, 8) != 0)
	{
		std::cerr << "PNG loader: File " << std::string(filename)
		  << " is not PNG format." << std::endl;
		return NULL;
	}

	// create main control structure
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
	  NULL, NULL);
	if (!png_ptr)
		return NULL;

	// create main info structure
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return NULL;
	}

	// create current info structure
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return NULL;
	}

	// set recovery point
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		// PNG errors will redirect here
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return NULL;
	}

	// initialize i/o
	png_init_io(png_ptr, fp);

	// supply the original 8 signature bytes
	png_set_sig_bytes(png_ptr, 8);

	// read the header
	png_read_info(png_ptr, info_ptr);

	// extract relevant info
	int bit_depth, color_type;
	png_uint_32 temp_width, temp_height;
	png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth,
	  &color_type, NULL, NULL, NULL);

	if (bit_depth != 8 || (color_type != PNG_COLOR_TYPE_RGB &&
		                    color_type != PNG_COLOR_TYPE_RGBA))
	{
		std::cerr << "PNG loader: Bad format." << std::endl;
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		return NULL;
	}

	*width = temp_width;
	*height = temp_height;
	*hasAlpha = color_type == PNG_COLOR_TYPE_RGBA;

	// get row size
	png_read_update_info(png_ptr, info_ptr);
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	png_byte *image_data =
	  new png_byte[rowbytes * temp_height * sizeof(png_byte)+15];

	// load by row
	png_byte **row_pointers = new png_byte*[temp_height * sizeof(png_byte*)];
	for (unsigned i = 0; i < temp_height; ++i)
	{
		row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
	}

	// do the read
	png_read_image(png_ptr, row_pointers);

	// clean up
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	fclose(fp);
	delete[] row_pointers;
	return reinterpret_cast<unsigned char*>(image_data);
}

