#include"model.h"
#include"data.h"

int main(int argc,char *argv[])
{
    string center_path = "E:/DNAClustering/Data/generatedCenters.txt";
    string copies_path = "E:/DNAClustering/Data/generatedCopies.txt";
    //string center_path = "E:/DNAClustering/Data/mini_test_centers.txt";
    //string copies_path = "E:/DNAClustering/Data/mini_test_cluster.txt";
    //Read Data
    auto centers = readCenterText(center_path.c_str());
    auto copies = readCopyText(copies_path.c_str());

    Config params;
    params.r = 25; // Threshold for edit distance
    params.q = 3; // Substring's length
    params.w = 4; // Anchor's length
    params.l = 12; // 
    params.n = 110; // Real segmentation length
    params.theta_low = 40;
    params.theta_high = 60;
    params.core_num = 8;
    params.local_steps = 30;
    params.comm_steps = 26;
    params.gamma = 0.9;
    params.compute_acc = true;
    params.num_cluster = centers.size();
    cout << "Clustering..." << endl;
    auto ans = compute_comm(copies, params);
    
    return 0;
}