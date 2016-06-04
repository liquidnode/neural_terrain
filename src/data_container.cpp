#include "data_container.h"

DataContainer::DataContainer(fann_type* _data, int _sizex, int _sizey)
{
	data = _data;
	sizex = _sizex;
	sizey = _sizey;
}

fann_type* DataContainer::getRectangle(int x, int y, int dx, int dy)
{
	fann_type* ret = new fann_type[dx * dy];
	if (x + dx > sizex || x < 0 || y + dy > sizey || y < 0)
		return 0;
	for (int gy = 0; gy < dy; ++gy)
	for (int gx = 0; gx < dx; ++gx)
		ret[gx + gy * dx] = data[gx + x + (gy + y) * sizex];
	return ret;
}

fann_train_data DataContainer::getTrainingsData(int w, int h)
{
	fann_train_data ret;
	ret.num_data = (sizex / w) * (sizey / h);
	ret.num_input = w * h;
	ret.num_output = ret.num_input;

	ret.input = new fann_type*[ret.num_data];
	ret.output = new fann_type*[ret.num_data];

	for (int gy = 0; gy < (sizey / h); ++gy)
	for (int gx = 0; gx < (sizex / w); ++gx)
	{
		int i = gx + gy * (sizex / w);

		ret.input[i] = getRectangle(gx * w, gy * h, w, h);

		ret.output[i] = ret.input[i]; //reproduce output
	}
	return ret;
}