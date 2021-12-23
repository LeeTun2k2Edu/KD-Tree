#pragma once
#include <algorithm>
#include <vector>
#include <math.h>
using namespace std;

using point_t = vector<double>;
using pointVec = vector<point_t>;
using pointIndex = typename std::pair< std::vector< double >, size_t >;
using pointIndexArr = typename std::vector< pointIndex >;

#define DIM 3

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


inline double dist2(const point_t a, const point_t b) {
    double distc = 0;
    for (size_t i = 0; i < DIM; i++) {
        double di = a.at(i) - b.at(i);
        distc += di * di;
    }
    return distc;
}
inline double dist(const point_t a, const point_t b) {
    return sqrt(dist2(a, b));
}
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
    Node* nearestt(Node* branch, point_t pt, size_t level, Node* best, double best_dist)
    {
        
        if ( (branch->left == nullptr && branch->right == nullptr)) {
            return nullptr;
        }

        double d, dx, dx2;
        point_t branch_pt = branch->data;
        d = dist2(branch_pt, pt);
        dx = branch_pt.at(level) - pt.at(level);
        dx2 = dx * dx;

        Node* best_l = best;
        double best_dist_l = best_dist;

        if (d < best_dist) 
        {
            best_dist_l = d;
            best_l = branch;
        }

        size_t next_lv = (level + 1) % DIM;
        Node* section;
        Node* other;

        if (dx > 0) 
        {
            section = branch->left;
            other = branch->right;
        }
        else 
        {
            section = branch->right;
            other = branch->left;
        }

        Node* further;
        if (section != nullptr)
        {
            further = nearestt(section, pt, next_lv, best_l, best_dist_l);
            if (further != nullptr)
            {
                double dl = dist2(further->data, pt);
                if (dl < best_dist_l)
                {
                    best_dist_l = dl;
                    best_l = further;
                }
            }

        }
        if (dx2 < best_dist_l && other != nullptr) 
        {
            further = nearestt(other, pt, next_lv, best_l, best_dist_l);
            if (further != nullptr)
            {
                double dl = dist2(further->data, pt);
                if (dl < best_dist_l) 
                {
                    best_dist_l = dl;
                    best_l = further;
                }
            }
            
        }

        
        return best_l;
    }
    point_t nearest(point_t pt) 
    {
        size_t level = 0;
        double branch_dist = dist2(root->data, pt);
        Node* result = nearestt(root, pt, level, root,  branch_dist);
        return result->data;
    }

    pointIndexArr neighborhood_(Node* branch, point_t pt, double rad, size_t level)
    {
        double d, dx, dx2;
        if ( (branch->left == nullptr && branch->right == nullptr)) {
            return pointIndexArr();
        }

        
        double r2 = rad * rad;

        point_t branch_pt = branch->data;
        d = dist2(branch_pt, pt);
        dx = branch_pt.at(level) - pt.at(level);
        dx2 = dx * dx;


        pointIndexArr nbh, nbh_s, nbh_o;
        if (d <= r2) 
        {
            nbh.push_back(pointIndex(branch_pt, level));
        }

        Node* section;
        Node* other;
        if (dx > 0) {
            section = branch->left;
            other = branch->right;
        }
        else {
            section = branch->right;
            other = branch->left;
        }
        
        if (section != nullptr)
        {
            nbh_s = neighborhood_(section, pt, rad, (level + 1) % DIM);
            nbh.insert(nbh.end(), nbh_s.begin(), nbh_s.end());

        }
        if (dx2 < r2 && other != nullptr) 
        {
           nbh_o = neighborhood_(other, pt, rad, (level + 1) % DIM);
            nbh.insert(nbh.end(), nbh_o.begin(), nbh_o.end());
            
        }
        return nbh;
        
    }
    pointIndexArr neighborhood(const point_t & pt, const double& rad) {
        size_t level = 0;
        return neighborhood_(root, pt, rad, level);
    }
    pointVec neighborhood_points(const point_t& pt, const double& rad) 
    {
        size_t level = 0;
        pointIndexArr nbh = neighborhood_(root, pt, rad, level);
        pointVec nbhp;
        nbhp.resize(nbh.size());
        std::transform(nbh.begin(), nbh.end(), nbhp.begin(), [](pointIndex x) { return x.first; });
        return nbhp;
    }
    

};