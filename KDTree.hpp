#pragma once
#include <vector>
#include <math.h>
using namespace std;

using point_t = vector<double>;
using pointVec = vector<point_t>;

struct Node
{
    point_t data;
    size_t level;
    Node* left;
    Node* right;
    Node (point_t data_, size_t level_)
    {
        data = data_;
        level = level_;
    }
};

double average(point_t a)
{
    return a[0] + a[1] + a[2];
}

void quickSort(pointVec &A, int l, int r)
{
    int i = l, j = r;
    double p = average(A[(l+r)/2]);
    while (i < j)
    {
        while (average(A[i]) < p) i++;
        while (average(A[j]) > p) j--;
        if (i <= j)
        {
            swap(A[i], A[j]);
            i++;
            j--;
        }
    }
    if (i < r) quickSort(A,i,r);
    if (l < j) quickSort(A,l,j);
}

struct KDTree
{
    Node *root = nullptr;
    Node* make_tree(pointVec A, size_t l, size_t r, size_t level)
    {
        if (l == r) return nullptr;
        size_t mid = (l + r) / 2;
        Node *newNode = new Node(A[mid], level);
        newNode->left = make_tree(A,l,mid,level+1);
        newNode->right = make_tree(A,mid+1,r,level+1);
        return newNode;
    }
    KDTree(pointVec data)
    {
        quickSort(data, 0, data.size()-1); 
        root = make_tree(data, 0, data.size()-1, 0);
    }
};