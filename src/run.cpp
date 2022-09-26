#include"model.h"
#include"data.h"

int main()
{
    string center_path = "../test_data/Centers.txt";
    //string copies_path = "../test_data/Clusters.txt";
    string copies_path = "../test_data/test_file.txt";
    //Read Data
    //vector<string> centers = readCenterText(center_path.c_str());
    auto copies = readCopyText(copies_path.c_str());

    Config params;
    params.r = 25;
    params.q = 3;
    params.w = 4;
    params.l = 12;
    params.theta_low = 40;
    params.theta_high = 60;
    params.core_num = 8;
    params.local_steps = 30;
    params.comm_steps = 130;
    cout << "Clustering..." << endl;
    auto ans = compute_comm(copies, params);
    int clustered_num = 0;
    for(auto & cluster : ans)
    {
        cout << cluster.size() << "###############################################" << endl;
        for(auto & it : cluster)
        {
            cout << it.data << ' ' << it.est_cluster << ' ' << it.cluster << endl;
        }
        clustered_num += cluster.size();
    }
    cout << clustered_num << endl;
    
    return 0;
}

/*
TO DO
1. Accuracy
2. Parallel for
*/