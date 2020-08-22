#pragma once
#include <cstdio>
#include <stdint.h>

enum ImageType {
	PNG, JPG, BMP, TGA
};

struct Image
{
	uint8_t* data = NULL;
	size_t size = 0;
	int w;
	int h;
	int channels;
	
	bool created = false;

	Image(const char* filename);
	Image(int w, int h, int channels);
	Image(const Image& img);
	~Image();

	bool read(const char* filename);
	bool write(const char* filename);

	bool isCreated();

	ImageType getFileType(const char* filename);
};

