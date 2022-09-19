/*
For reading the nanopore dataset
*/
#include"data.h"

vector<string> readCenterText(string file)
{
    // Input buffer
    char buffer[MAX_IN_BUFFER];

    ifstream infile;
    infile.open(file.data());

    // Output error message if failed
    assert(infile.is_open());

    vector<string> ans;
    while(infile.good() && !infile.eof())
    {
        // Empty buffer
        memset(buffer, 0, MAX_IN_BUFFER);
        infile.getline(buffer, MAX_IN_BUFFER);
        ans.push_back((string)buffer);
    }
    cout << "Imported " << ans.size() << " centers" << endl;
    return ans;
}

pair<vector<string>, vector<int>> readCopyText(string file)
{
    // Input buffer
    char buffer[MAX_IN_BUFFER];

    ifstream infile;
    infile.open(file.data());

    // Output error message if failed
    assert(infile.is_open());

    vector<string> data;
    vector<int> indexes;
    int cnt = 0;
    while(infile.good() && !infile.eof())
    {
        // Empty buffer
        memset(buffer, 0, MAX_IN_BUFFER);
        infile.getline(buffer, MAX_IN_BUFFER);

        // Remove delimiter and get data
        if(buffer[0] != '=')
        {
            data.push_back((string)buffer);
            indexes.push_back(cnt);
        }
        else
        {
            ++cnt;
        }
    }
    cout << "Imported " << data.size() << " copies" << endl;
    pair<vector<string>, vector<int>> ans = {data, indexes};
    return ans;
}