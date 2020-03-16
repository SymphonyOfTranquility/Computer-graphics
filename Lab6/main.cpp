#include <iostream>
#include "GiftWrapping.h"

int main()
{
    gift_wrapping_space::GiftWrapping convex_hull;
    convex_hull.input_graph("../input.txt");
    convex_hull.jarvis_convex_hull();
    convex_hull.output_convex_hull();
    return 0;
}
