#include <iostream>
#include "ApproximateConvex.h"

int main()
{
    convexhull_space::ApproximateConvex convex;
    convex.input_points("../input.txt");
    for (int i = 1;i <= 15; ++i)
    {
        convex.get_approximate(i);
        convex.output();
        std::cout << '\n';
    }

    return 0;
}
