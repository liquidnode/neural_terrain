#ifndef DATA_CONTAINER_H_
#define DATA_CONTAINER_H_

#include "floatfann.h"

class DataContainer
{
private:
	fann_type* data;
public:
	int sizex;
	int sizey;
	DataContainer(){}
	DataContainer(fann_type* _data, int _sizex, int _sizey);
	fann_type* getRectangle(int x, int y, int dx, int dy);
	fann_train_data getTrainingsData(int w, int h);
};

#endif