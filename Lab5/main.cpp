#include <iostream>
#include "QuickHull.h"
int main()
{
    quickhull_space::QuickHull quick_hull;
    quick_hull.input_graph("../input.txt");
    quick_hull.create_convex_hull();
    quick_hull.output_convex_hull();
    
    return 0;
}
