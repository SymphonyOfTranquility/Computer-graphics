#include <iostream>
#include "Voroniy.h"

int main()
{
    vor::Voroniy voroniy;
    voroniy.input_points("../input.txt");
    voroniy.fortune_algorithm();
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
