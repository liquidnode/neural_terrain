#include <array>
#include <iostream>
#include <string>

#include "fann.h"


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
}


void testNeuralNet(const char* const input)
{
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
}
