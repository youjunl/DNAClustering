

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
    Config params;
    params.r = 5;
    params.q = 3;
    params.w = 4;
    params.l = 2;
    params.theta_low = 10;
    params.theta_high = 20;
    params.core_num = 4;
    params.local_steps = 10;
    params.comm_steps = 30;
    auto ans = compute_comm(copies, params);
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