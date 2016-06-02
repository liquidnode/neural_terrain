#include "importer.h"

fann_type Importer::getPixel(int x, int y, char* buffer, int width, int offset)
{
	int stride = ((24 * width + 31) / 32) * 4;
	return (float)*reinterpret_cast<unsigned char*>(&buffer[x * 3 + y * stride + offset]) / 255.0f;
}

fann_train_data Importer::getDataFromBMP(const char* file_, int w, int h, int& width, int& height)
{
	fann_train_data ret;
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

		ret.num_data = (width / w) * (height / h);
		ret.num_input = w * h;
		ret.num_output = ret.num_input;
	
		ret.input = new fann_type*[ret.num_data];
		ret.output = new fann_type*[ret.num_data];

		for (int gy = 0; gy < (height / h); ++gy)
		for (int gx = 0; gx < (width / w); ++gx)
		{
			int i = gx + gy * (width / w);
			ret.input[i] = new fann_type[ret.num_input];

			for (int y = 0; y < h; ++y)
			for (int x = 0; x < w; ++x)
			{
				int g = x + gx * w + (y + gy * h) * (width / w) * w;
				ret.input[i][x + y * w] = Importer::getPixel(x + gx * w, y + gy * h, buffer, width, file_header->bfOffBits);
			}

			ret.output[i] = ret.input[i]; //reproduce output
		}
	}
	else
	{
		std::cerr << "File not found" << std::endl;
		exit(0);
	}
	return ret;
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