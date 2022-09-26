#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <unordered_map>
#include <time.h>
#include <bitset>
#include <math.h>
#include <mpi.h>
#include <omp.h>
#include <algorithm>
#include <set>

#include "data.h"

using namespace std;

class Config
{
public:
    int r = 6;
    int q = 3;
    int w = 3;
    int l = 6;
    int theta_low = 10;
    int theta_high = 30;
    int core_num = 4;
    int local_steps = 20;
    int comm_steps = 20;
};

/*
Calculate edit distance between two strings (minimum number of operation
for coverting str1 with length m to str2 with length n).
*/
int editDistance(string str1, string str2, int m, int n);

/*
Calculate accuracy between the ground truth and estimated result
*/
double accuracy(unordered_map<int, vector<int>> c, unordered_map<int, vector<int>> est_c);

/*
Hashing for sequence using anchor
*/
string compute_hash(string target, string anchor, int len);

/*
Extract q-grams from inputs and calculate binary signature distance
*/
int blocking_bsd(string str1, string str2, const int blockLen, const int q);

int bsd(string str1, string str2, const int q);

/*
Clustering in core
*/
vector<vector<Sequence>> compute_comm(vector<Sequence> &S, Config params);

void compute_local(vector<vector<Sequence>> &C, Config params);

string random_anchor(int w);

Sequence random_sample(vector<Sequence> cur);

// Union-Find Set for mering operation
class UnionSet
{
private:
    vector<int> parent;

public:
    int count;
    UnionSet(vector<int> &init)
    {
        
        count = init.size();
        parent = init;
    }

    void connect(int p, int q)
    {
        int rootP = find(p);
        int rootQ = find(q);

        if (rootP == rootQ)
            return;

        parent[rootQ] = rootP;

        count--;
    }

    bool connected(int p, int q)
    {
        int rootP = find(p);
        int rootQ = find(q);
        return rootP == rootQ;
    }

    int find(int x)
    {
        if (parent[x] != x)
        {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
};

#endif