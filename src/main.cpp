#include <array>
#include <iostream>
#include <string>

#include "floatfann.h"
#include "importer.h"

//#define TEST

void trainNeuralNet(const char* input, const char* output);
void testNeuralNet(const char* input);


int main(int argc, char* argv[])
{
    if(argc < 2)
    {
		std::cerr << "Usage: " << argv[0] << " <mode>" << std::endl;

        return 1;
    }

    const std::string mode = argv[1];
    if(mode == "train")
    {
        if(argc < 4)
        {
            std::cerr << "Usage: " << argv[0] << " train <train_data_in> <neural_net_out>" << std::endl;

            return 2;
        }

        trainNeuralNet(argv[2], argv[3]);
		testNeuralNet(argv[3]);
    }
    else if(mode == "test")
    {
        if(argc < 3)
        {
            std::cerr << "Usage: " << argv[0] << " test <neural_net_in>" << std::endl;

            return 3;
        }

        testNeuralNet(argv[2]);
    }
    else
    {
        std::cerr << "Unknown mode." << std::endl;

        return 4;
    }

    return 0;
}


void trainNeuralNet(const char* const input, const char* const output)
{
#ifdef TEST
    const unsigned int numLayers = 3;
    const unsigned int numInputs = 2;
    const unsigned int numNeuronsHidden = 3;
    const unsigned int numOutputs = 1;
    const float desiredError = 0.001f;
    const unsigned int maxEpochs = 500000;
    const unsigned int epochsBetweenReports = 1000;

    fann* ann = fann_create_standard(numLayers, numInputs,  numNeuronsHidden, numOutputs);
    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);
    fann_train_on_file(ann, input, maxEpochs, epochsBetweenReports, desiredError);
    fann_save(ann, output);
    fann_destroy(ann);
#else
	int w = 5;
	int h = 5;
	int width;
	int height;
	DataContainer container = Importer::getDataFromBMP(input/*"../data/test_data_3500_1000.bmp"*/, width, height);
	fann_train_data data = container.getTrainingsData(w, h);

	const unsigned int numLayers = 4;
	const unsigned int numInputs = w * h;
	const unsigned int numNeuronsHidden1 = 15;
	const unsigned int numNeuronsHidden2 = 10;
	const unsigned int numOutputs = w * h;
	const float desiredError = 0.0f;
	const unsigned int maxEpochs = 10000;
	const unsigned int epochsBetweenReports = 100;
	
	fann* ann = fann_create_standard//(numLayers, numInputs, numNeuronsHidden1, numNeuronsHidden2, numNeuronsHidden1, numOutputs);
		(numLayers, numInputs, numNeuronsHidden1, numNeuronsHidden2, numOutputs);
	fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
	fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);
	fann_train_on_data(ann, &data, maxEpochs, epochsBetweenReports, desiredError);
	fann_save(ann, output);

	fann_destroy(ann);
	for (int i = 0; i < data.num_data; ++i)
		delete[] data.input[i];
	delete[] data.input;
#endif
}

//train ../data/test_data_3500_1000x2.bmp NN_layer1_0.txt
//test NN_layer1_0.txt

void testNeuralNet(const char* const input)
{
#ifdef TEST
    using InputData = std::array<fann_type, 2u>;
    std::array<InputData, 4u> testData =
    { {
        { -1.0f, -1.0f },
        { -1.0f, 1.0f },
        { 1.0f, -1.0f },
        { 1.0f, 1.0f },
    } };

    fann* ann = fann_create_from_file(input);

    for(InputData& data : testData)
    {
        fann_type* result = fann_run(ann, data.data());
        std::cout << data[0] << " XOR " << data[1] << " -> " << *result << std::endl;
    }

    fann_destroy(ann);
#else
	fann* ann = fann_create_from_file(input);
	int w = sqrt(ann->num_input);
	int h = sqrt(ann->num_input); 
	int width;
	int height;
	DataContainer container = Importer::getDataFromBMP("../data/test_data_3500_1000x2.bmp", width, height);
	fann_train_data testdata = container.getTrainingsData(w, h);

	fann_type* imageout = new fann_type[testdata.num_data * testdata.num_output];
	int gw = (width / w);
	for (int i = 0; i < testdata.num_data; ++i)
	{
		fann_type* result = fann_run(ann, testdata.input[i]);
		int gx = i % gw;
		int gy = i / gw;
		for (int y = 0; y < h; ++y)
		for (int x = 0; x < w; ++x)
		{
			imageout[x + gx * w + (y + gy * h) * gw * w] = result[x + y * w];
		}
	}

	Importer::testWriteData("test2.txt", gw * w, (height / h) * h, imageout);

	delete[] imageout;
#endif
}
