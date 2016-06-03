#ifndef IMPORTER_TRAINING_H_
#define IMPORTER_TRAINING_H_

//partly from http://stackoverflow.com/questions/5751749/how-can-i-read-bmp-pixel-values-into-an-array

#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

#include "data_container.h"
#include "floatfann.h"


//do you need this?

/*typedef int LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;

typedef struct tagBITMAPFILEHEADER {
	WORD  bfType;
	DWORD bfSize;
	WORD  bfReserved1;
	WORD  bfReserved2;
	DWORD bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	DWORD biSize;
	LONG  biWidth;
	LONG  biHeight;
	WORD  biPlanes;
	WORD  biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG  biXPelsPerMeter;
	LONG  biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;*/

class Importer
{
private: 
	static fann_type getPixel(int x, int y, char* buffer, int width, int offset);
public:
	static DataContainer getDataFromBMP(const char* file, int& width, int& height);
	static void testWriteData(const char* file, int w, int h, fann_type* data);
};

#endif