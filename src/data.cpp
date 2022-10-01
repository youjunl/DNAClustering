/*
For reading the nanopore dataset
*/
#include"data.h"

vector<string> readCenterText(const char * file)
{
    FILE* infile;
    errno_t err = fopen_s(&infile, file, "r");
    size_t linesz = MAX_IN_BUFFER;

    // Input buffer
    char buffer[MAX_IN_BUFFER];
    memset(buffer, 0, MAX_IN_BUFFER);
    vector<string> ans;

    while(!feof(infile))
    {
        // Empty buffer
        memset(buffer, 0, MAX_IN_BUFFER);
        fgets(buffer, MAX_IN_BUFFER, infile);
        string converted_buffer = (string)buffer;
        converted_buffer[converted_buffer.size() - 1] = 0;
        ans.push_back(converted_buffer);
    }
    cout << "Imported " << ans.size() << " centers" << endl;
    return ans;
}

vector<Sequence> readCopyText(const char * file)
{
    FILE* infile;
    errno_t err = fopen_s(&infile, file, "r");
    size_t linesz = MAX_IN_BUFFER;

    // Input buffer
    char buffer[MAX_IN_BUFFER];
    memset(buffer, 0, MAX_IN_BUFFER);
    vector<Sequence> ans;
    int cnt = 0;
    while(!feof(infile))
    {
        // Empty buffer
        memset(buffer, 0, MAX_IN_BUFFER);
        fgets(buffer, MAX_IN_BUFFER, infile);

        // Remove delimiter and get data
        if(buffer[0] != '=')
        {
            string converted_buffer = (string)buffer;
            converted_buffer[converted_buffer.size() - 1] = 0;
            Sequence data(converted_buffer, cnt);
            ans.push_back(data);
        }
        else
        {
            ++cnt;
        }
    }
    cout << "Imported " << ans.size() << " copies" << endl;
    cout << "Number of Clusters: " << cnt + 1 << endl;
    return ans;
}