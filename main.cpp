#include<iostream>
#include"KDTree.hpp"
using namespace std;

void display(Node *p, int space) 
{
		if (p == nullptr) return ;
        space += 5;
        display(p->right, space);
        for (int i = 5; i < space; i++) std::cout << " ";
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
        data.push_back(point);
    }
    KDTree tree(data);
    
    display(tree.root, 0);
    
    return 0;
}