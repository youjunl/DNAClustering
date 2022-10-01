#include "model.h"

unordered_map<int, string> dna_alphabet = {{0, "A"}, {1, "T"}, {2, "G"}, {3, "C"}};
unordered_map<char, int> ind_alphabet = { {'A', 0}, {'T', 1}, {'G', 2}, {'C', 3}};
/*
Utility function to find minimum of three numbers
*/
int min(int x, int y, int z) { return min(min(x, y), z); }

bool cmp(Sequence a,Sequence b) {return a.data>b.data;}

vector<bool> compute_indicator(string str, int numBlock, int blockLen, const int q)
{
    const int blockIVLen = pow(4, q); // 4^3 = 64
    // Indicator vector
    vector<bool> iv((str.size() / blockLen + 1) * blockIVLen, false);
    for (int i = 0; i < str.size() - q + 1; i++)
    {
        int index = 0, carry = 1;
        for (int j = q - 1; j >= 0; j--)
        {
            index += carry * ind_alphabet[str[i + j]];
            carry *= 4;
        }
        iv[index + i/blockLen*blockIVLen] = true;
    }
    return iv;
}

int editDistance(string str1, string str2, int m, int n)
{
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    for (int i = 1; i <= m; i++) {
        dp[i][0] = i;
    }
    for (int j = 1; j <= n; j++) {
        dp[0][j] = j;
    }
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = min(dp[i - 1][j - 1], min(dp[i][j - 1], dp[i - 1][j])) + 1;
            }
        }
    }
    return dp[m][n];
}

string compute_hash(string str, string anchor, int len)
{
    if (anchor.size() >= str.size())
    {
        return str;
    }
    size_t pos = str.find_first_of(anchor, 0);
    // Return the first found substring else return last len characters
    return pos == string::npos ? str.substr(str.size() - len) : str.substr(pos, len);
}

int compute_bsd(vector<bool>& iv1, vector<bool>& iv2)
{
    // Calculate hamming distance between the indicator vectors
    int ans = 0;
    const int maxLen = max(iv1.size(), iv2.size());
    const int minLen = min(iv1.size(), iv2.size());
    for (int i = 0; i < minLen; i++)
    {
        ans += iv1[i] ^ iv2[i]; // XOR
    }
    if (iv1.size() >= iv2.size())
    {
        for (int i = minLen; i < maxLen; i++)
        {
            ans += (int)iv1[i];
        }
    }
    else
    {
        for (int i = minLen; i < maxLen; i++)
        {
            ans += (int)iv2[i];
        }
    }
    return ans;
}

vector<vector<Sequence>> compute_comm(vector<Sequence> &S, Config params)
{
    const int len = params.w + params.l;
    vector<vector<Sequence>> C(S.size());
    // Initialization
    for (int i = 0; i < S.size(); i++)
    {
        S[i].est_cluster = i;
        C[i].push_back(S[i]);
    }

    for (int comm_step = 0; comm_step < params.comm_steps; comm_step++)
    {
        cout << "Communication Step: " << comm_step << endl;
        // Random anchor
        string anchor = random_anchor(params.w);
        // Partition map
        vector<vector<vector<Sequence>>> Partition(params.core_num);
        // Get representatives and compute hash value
        for (int i = 0; i < C.size(); i++)
        {
            // auto represent = random_sample(C[i]);
            // string sample = represent.data;
            // string hash_val = compute_hash(sample, anchor, len);
            // unsigned int hash_num = (int)hash_val[0] << 12 + (int)hash_val[1] << 4 + (int)hash_val[2] >> 4;
            // srand(hash_num);
            //cout << rand() % params.core_num << ' ' << represent.cluster << endl;
            Partition[rand() % params.core_num].push_back(C[i]);
        }
        // Reset
        C.resize(0);
        // Multi-thread par for
#pragma omp parallel for num_threads(8)
        // Start all the core and wait until finish
        for (int i = 0; i < Partition.size(); i++)
        {
            compute_local(Partition[i], params);
        }
        // Collect results
        for (int i = 0; i < Partition.size(); i++)
        {
            C.insert(C.begin(), Partition[i].begin(), Partition[i].end());
        }
        // Output Accuracy
        if(params.compute_acc)
        {
            auto acc = accuracy(C, params);
            cout << "Accuracy: " << acc << endl;            
        }
    }

    // Add estimated indexes for result
    int cnt = 0;
    for (auto &cluster : C)
    {
        for (auto &it : cluster)
        {
            it.est_cluster = cnt;
        }
        ++cnt;
    }
    return C;
}

void compute_local(vector<vector<Sequence>> &C, Config params)
{
    const int len = params.w + params.l;
    const int blockLen = 22;
    const int numBlock = params.n / blockLen;
    const int IVLen = pow(4, params.q) * numBlock;

    // Local Steps
    for (int i = 0; i < params.local_steps; i++)
    {
        // Random anchor
        string anchor = random_anchor(params.w);
        vector<Sequence> bucket;
        vector<string> hashval(C.size());
        vector<vector<bool>> indicators(C.size());
        unordered_map<int, int> merge_map;
        // Get representatives and compute hash value
        for (int j = 0; j < C.size(); j++)
        {
            if (C[j].size() < 1)
            {
                continue;
            }
            auto sample = random_sample(C[j]);
            bucket.push_back(sample);
            merge_map[sample.est_cluster] = j; // <cluster index, vector index>
        }
        // Put in bucket and merge clusters and sort the reads based on hashval;
        sort(bucket.begin(), bucket.end(), cmp);

        // Compute hash for samples in buckets
        for (int j = 0; j < bucket.size(); j++)
        {
            hashval[j] = compute_hash(bucket[j].data, anchor, len);
            indicators[j] = compute_indicator(bucket[j].data, numBlock, blockLen, params.q);
        }
        int bucket_len = bucket.size();

        for (int j = 0; j < bucket_len; j++)
        {
            // Only compare adjacent elements
            for (int k = j + 1; k < min(j + 20, bucket_len); k++)
            {
                string str1 = bucket[j].data, str2 = bucket[k].data;

                // 优化 提前计算导向向量，在这里进行比较
                int distance = compute_bsd(indicators[j], indicators[k]);
                if (distance > params.theta_high)
                {
                    continue; // Skip
                }
                else if (distance <= params.theta_low || (distance <= params.theta_high && editDistance(str1, str2, str1.size(), str2.size()) <= params.r))
                {      
                    // Merge j and k
                    int ind1 = bucket[j].est_cluster;
                    int ind2 = bucket[k].est_cluster;
                    // Merge all elements from cluster ind2 to cluster ind1
                    int c_size = C[merge_map[ind2]].size();
                    for(int h = 0; h < c_size; h++)
                    {
                        auto tmp = C[merge_map[ind2]][h];
                        tmp.est_cluster = ind1;
                        //it.est_cluster = ind1;
                        C[merge_map[ind1]].push_back(tmp);
                    }
                    // Clear origin cluster after merging
                    C[merge_map[ind2]].clear();
                    // Redirect parent
                }
            }
        }
    }
    // remove empty clusters
    remove_empty(C);
}

// Generate a random anchor
string random_anchor(int w)
{
    string anchor = "";
    for (int i = 0; i < w; i++)
    {
        anchor.append(dna_alphabet[rand() % 4]);
    }
    return anchor;
}

// Sample a random data from inputs
Sequence random_sample(vector<Sequence> cur)
{
    return cur.size() > 1 ? cur[rand() % cur.size()] : cur[0];
}

// Compute accuracy
double accuracy(vector<vector<Sequence>> C, Config params)
{
    int n = C.size(), cnt = 0;
    unordered_map<int, int> mapping; // <est index, index>
    unordered_map<int, int> groundTruth; // <index, count>
    for(auto & cluster : C)
    {
        for(auto & it : cluster)
        {
            ++groundTruth[it.cluster];
        }
    }
    for(auto & cluster : C)
    {
        int index = cluster[0].cluster;
        int est_index = cluster[0].est_cluster;
        int num = cluster.size();
        if(num == 0 || num < (double)groundTruth[index]*params.gamma)
        {
            continue;
        }
        for(auto & it : cluster)
        {
            // Exist false positive
            if(it.cluster != index)
            {
                break;
            }
            --num;
        }
        // Exist false positive
        if(num != 0)
        {
            continue;
        }
        ++cnt;
    }
    return (double)cnt/(double)params.num_cluster;
}


// Remove empty clusters
void remove_empty(vector<vector<Sequence>> & C)
{
    // remove empty clusters
    vector<vector<Sequence>> new_C;
    for (auto & cluster : C)
    {
        if (cluster.size() != 0)
        {
            new_C.push_back(cluster);
        }
    }
    C = new_C; 
}