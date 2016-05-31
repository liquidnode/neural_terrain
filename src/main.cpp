#include "fixedfann.h"

int main(int argc, char* argv[])
{
	fann* ann = fann_create_standard(
		1, 0.7, 3, 26, 13, 3
		);
    return 0;
}
