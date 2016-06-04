#include "importer.h"

fann_type Importer::getPixel(int x, int y, char* buffer, int width, int offset)
{
	int stride = ((24 * width + 31) / 32) * 4;
	return ((float)*reinterpret_cast<unsigned char*>(&buffer[x * 3 + y * stride + offset]) / 255.0f) * 2.0f - 1.0f;
}

DataContainer Importer::getDataFromBMP(const char* file_, int& width, int& height)
{
	char* buffer;
	PBITMAPFILEHEADER file_header;
	PBITMAPINFOHEADER info_header;

	std::ifstream file(file_, std::ios_base::binary);

	if (file) {
		file.seekg(0, std::ios::end);
		streampos length = file.tellg();
		file.seekg(0, std::ios::beg);

		int l = length.seekpos();
		buffer = new char[l];

		if (!file.read(buffer, length))
		{
			std::cout << "error: only " << file.gcount() << " could be read" << endl;
			if (file.eof())
				cout << "eof" << endl;
			if (file.bad())
				cout << "bad" << endl;
			if (file.fail())
				cout << "fail" << endl;
		}

		file_header = (PBITMAPFILEHEADER)(buffer);
		info_header = (PBITMAPINFOHEADER)(buffer + sizeof(BITMAPFILEHEADER));

		width = info_header->biWidth;
		height = info_header->biHeight;

		fann_type* ret = new fann_type[width * height];
		for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
			ret[x + y * width] = Importer::getPixel(x, y, buffer, width, file_header->bfOffBits);

		return DataContainer(ret, width, height);
	}
	else
	{
		std::cerr << "File not found" << std::endl;
		exit(0);
		return DataContainer();
	}
}

void Importer::testWriteData(const char* file, int w, int h, fann_type* data)
{
	ofstream file_;
	file_.open(file);
	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			file_ << data[x + y * w] << " ";
		}
		file_ << endl;
	}
	file_.close();
}