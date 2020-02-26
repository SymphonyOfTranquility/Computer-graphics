#include <iostream>
#include "RegionTree.h"

int main()
{
    region_tree::RegionTree tree;
    tree.input_graph("../input.txt");
    tree.pre_processing();
    tree.output_points();
    tree.output_differ();
    tree.output_tree();


    region_tree::TPoint a[3] = {{3, 4},{1,0}, {8, 12}},
                        b[3] = {{9, 10},{9, 6}, {13, 14}};
    for (int i = 0;i < 3;++i)
    {
        int ans = tree.find_number_in_area(a[i], b[i]);
        std::cout << ans << std::endl;
    }

    return 0;
}
