#include<iostream>
#include"KDTree.hpp"
using namespace std;

void display(Node *p, int space) 
{
		if (p == nullptr) return ;
        space += 5;
        display(p->right, space);
        for (int i = 5; i < space; i++) cout << " ";
        printf("(%d,%d,%d)\n", (int)p->data[0], (int)p->data[1], (int)p->data[2]);
        display(p->left, space);
}

int main()
{
    
    point_t point = {0,0,0};
    pointVec data;
    for (int i = 9; i >= 0; i--)
    {
        point[0] = i;
        point[1] = i + 1;
        point[2] = i;
        data.push_back(point);
    }
    KDTree tree(data);

    point_t p = {6,6,6};
    point_t res = tree.nearest(p);
    
    cout <<  res[0] << res[1] << res[2] << "\n\n";


    display(tree.root, 0);

    std::cout << "in tat ca neighborhood " ;


    auto res2 = tree.neighborhood_points(p, 10);

    for (point_t a : res2) {
        for (double b : a) {
            std::cout << b << " ";
        }
        std::cout << '\n';
    }
    return 0;
}