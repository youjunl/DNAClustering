#include "model.h"

unordered_map<int, string> dna_alphabet = {{0, "A"}, {1, "T"}, {2, "G"}, {3, "C"}};

/*
Utility function to find minimum of three numbers
*/
int min(int x, int y, int z) { return min(min(x, y), z); }

bool cmp(Sequence a,Sequence b) {return a.data>b.data;}

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

int bsd(string str1, string str2, const int q)
{
    // Indicator vector
    unordered_map<string, int> diff((int)pow(4, q));
    int n = str1.size(), m = str2.size();
    for (int i = 0; i < n - q + 1; i++)
    {
        diff[str1.substr(i, q)] = 1;
    }
    for (int i = 0; i < m - q + 1; i++)
    {
        string tmp = str2.substr(i, q);
        diff[tmp] = diff.count(tmp) > 0 ? 0 : 1;
    }

    // Calculate final distance
    int ans = 0;
    for (auto it : diff)
    {
        ans += it.second;
    }
    return ans;
}

int blocking_bsd(string str1, string str2, const int blockLen, const int q)
{
    // Segment the string into blocks
    vector<string> str1_block, str2_block;
    int n = str1.size(), m = str2.size();
    int nblock = max(n, m) / blockLen + 1;
    for (int i = 0; i < nblock; i++)
    {
        i *nblock >= n ? str1_block.push_back("") : str1_block.push_back(str1.substr(i * nblock));
        i *nblock >= m ? str2_block.push_back("") : str2_block.push_back(str2.substr(i * nblock));
    }
    int ans = 0;
    for (int i = 0; i < nblock; i++)
    {
        ans += bsd(str1_block[i], str2_block[i], q);
    }
    return ans;
}

vector<vector<Sequence>> compute_comm(vector<Sequence> &S, Config params)
{
    int len = params.w + params.l;
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
            //srand((unsigned)time(NULL));
            string sample = (random_sample(C[i])).data;
            string hash_val = compute_hash(sample, anchor, len);
            // bitset<20> bit_val(hash_val);
            // srand(bit_val.to_ulong());
            //srand((unsigned)time(NULL));
            Partition[rand() % params.core_num].push_back(C[i]);
        }
        // Reset S
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
    int len = params.w + params.l, count = C.size();
    // Local Steps
    for (int i = 0; i < params.local_steps; i++)
    {
        // Random anchor
        string anchor = random_anchor(params.w);
        vector<Sequence> bucket;
        vector<string> hashval(C.size());
        vector<int> indexes(C.size());
        // Get representatives and compute hash value
        for (int j = 0; j < C.size(); j++)
        {
            if (C[j].size() < 1)
            {
                continue;
            }
            auto sample = random_sample(C[j]);
            bucket.push_back(sample);
            indexes[j] = sample.est_cluster;
        }
        UnionSet IndexesMap(indexes);
        // Put in bucket and merge clusters and sort
        sort(bucket.begin(), bucket.end(), cmp);

        // Compute hash for samples in buckets
        for (int j = 0; j < bucket.size(); j++)
        {
            hashval[j] = compute_hash(bucket[j].data, anchor, len);
        }
        int bucket_len = bucket.size();
        for (int j = 0; j < bucket_len; j++)
        {
            // Only compare adjacent elements
            for (int k = j + 1; k < min(j + 20, bucket_len); k++)
            {
                string str1 = bucket[j].data, str2 = bucket[k].data;
                if (hashval[j].compare(hashval[k]) == 0)
                {
                    int distance = blocking_bsd(str1, str2, 22, params.q);
                    if (distance > params.theta_high)
                    {
                        continue; // Skip
                    }
                    else if (distance <= params.theta_low || (distance <= params.theta_high && editDistance(str1, str2, str1.size(), str2.size()) <= params.r))
                    { 
                        // Merge
                        int ind1 = bucket[j].est_cluster;
                        int ind2 = bucket[k].est_cluster;
                        // Synchronize the index1 and index2
                        IndexesMap.connect(ind1, ind2);
                    }
                }
            }
        }
        // Update the clustering result
        unordered_map<int, int> mapping; // <cluster index, vector index>
        count = IndexesMap.count;
        int vi = 0;
        for(int i = 0; i < C.size(); i++)
        {
            indexes[i] = IndexesMap.find(indexes[i]);
            if(mapping.find(indexes[i]) == mapping.end())
            {
                mapping[indexes[i]] = ++vi;
            }
        }
        
        vector<vector<Sequence>> new_C(count);
        for(int i = 0; i < C.size(); i++)
        {
            if(C[i].size() == 0)
            {
                continue;
            }
            int new_index = indexes[i];
            int new_vi = mapping[new_index];
            for(int j = 0; j < C[i].size(); j++)
            {
                C[i][j].est_cluster = new_index;
                new_C[i].push_back(C[i][j]);
            }
            //new_C[i].insert(new_C.end(), C[i].begin(), C[i].end());
        }
        C = new_C;
    }
}

// Generate a random anchor
string random_anchor(int w)
{
    srand((unsigned)time(NULL));
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
