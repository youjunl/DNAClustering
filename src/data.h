#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<cassert>
#include<cstring>

#define MAX_IN_BUFFER 1024 // Buffer size for reading sequence data

using namespace std;

/*
Function for reading centers data
*/
vector<string> readCenterText(string file);

/*
Function for reading noisy copies data
*/
vector<string> readCopyText(string file);

