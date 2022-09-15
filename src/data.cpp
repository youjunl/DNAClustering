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

vector<string> readCopyText(string file)
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

        // Remove delimiter and get data
        if(buffer[0] != '=')
        {
            ans.push_back((string)buffer);
        }
    }
    cout << "Imported " << ans.size() << " copies" << endl;
    return ans;
}