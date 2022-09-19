

#include"model.h"
#include"data.h"

int main()
{
    //string center_path = "../test_data/Centers.txt";
    //string copies_path = "../test_data/Clusters.txt";
    string copies_path = "../test_data/test_file.txt";
    //Read Data
    //vector<string> centers = readCenterText(center_path);
    auto data = readCopyText(copies_path);
    auto copies = data.first;
    auto indexes = data.second;
    cout << "Number of Clusters: " << *(indexes.end() - 1) + 1 << endl;
    //Clustering DNA strands
    int edit_th = 6;
    int ngram = 3;
    int anchorLen = 3;
    int hashLen = 6;
    int low_th = 10;
    int high_th = 30;
    auto ans = compute_comm(copies, edit_th, ngram, anchorLen, hashLen, low_th, high_th);
    for(auto & cluster : ans)
    {
        cout << "###############################################" << endl;
        for(auto & it : cluster)
        {
            cout << it << endl;
        }
    }
    return 0;
}