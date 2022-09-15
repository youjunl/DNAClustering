#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<cassert>
#include<cstring>
#include<unordered_map>

using namespace std;

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
string hash(string target, string anchor, int len);

/*
Extract q-grams from inputs and calculate binary signature distance
*/
int bsd(string str1, string str2, const int q);

/*
Utility function to find minimum of three numbers
*/
int min(int x, int y, int z) { return min(min(x, y), z); }