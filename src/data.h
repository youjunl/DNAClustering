#ifndef DATA_H
#define DATA_H 

#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<cassert>
#include<cstring>


#define MAX_IN_BUFFER 1024 // Buffer size for reading sequence data

using namespace std;

class Sequence
{
    public:
    Sequence(string pdata, int pcluster)
    {
        data = pdata;
        cluster = pcluster;
    }
        string data;
        int cluster;
        int est_cluster;
};

/*
Function for reading centers data
*/
vector<string> readCenterText(const char * file);

/*
Function for reading noisy copies data
*/
vector<Sequence> readCopyText(const char * file);

#endif