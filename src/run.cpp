

#include"model.h"
#include"data.h"

int main()
{
    string center_path = "../test_data/Centers.txt";
    string copies_path = "../test_data/Clusters.txt";
    //Read Data
    vector<string> centers = readCenterText(center_path);
    auto data = readCopyText(copies_path);
    auto copies = data.first;
    auto indexes = data.second;
    cout << "Number of Clusters: " << *(indexes.end() - 1) << endl;
    //Clustering DNA strands
    auto ans = compute_comm(copies, 25, 3, 4, 12, 40, 60);
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