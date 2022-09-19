#include "model.h"

unordered_map<int, string> dna_alphabet = {{0, "A"}, {1, "T"}, {2, "G"}, {3, "C"}};

/*
Utility function to find minimum of three numbers
*/
int min(int x, int y, int z) { return min(min(x, y), z); }

int editDistance(string str1, string str2, int m, int n)
{
    if (m == 0 || n == 0)
    {
        return m == 0 ? n : m;
    }

    // If no error
    if (str1[m - 1] == str2[n - 1])
    {
        return editDistance(str1, str2, m - 1, n - 1);
    }

    // If has error
    return 1 + min(editDistance(str1, str2, m, n - 1),
                        editDistance(str1, str2, m - 1, n),
                        editDistance(str1, str2, m - 1, n - 1));
}

string compute_hash(string str, string anchor, int len)
{
    if(anchor.size() >= str.size())return str;

    int win = anchor.size() + len;
    size_t pos = str.find_first_of(anchor, 0);
    // Return the first found substring else return last len characters
    return pos == string::npos ? str.substr(str.size() - len) : str.substr(pos, len);
}

int bsd(string str1, string str2, const int q)
{
    // Indicator vector
    unordered_map<string, int> diff;
    int n = str1.size(), m = str2.size();
    for (int i = 0; i < n - q; i++)
    {
        diff[str1.substr(i, q)] = 1;
    }
    for (int i = 0; i < m - q; i++)
    {
        string tmp = str2.substr(i, q);
        diff[str2] = diff.count(tmp) ? 0 : 1;
    }

    // Calculate final distance
    int ans = 0;
    for (auto &it : diff)
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
        if (i * nblock >= n)
        {
            str1_block.push_back("");
        }
        else
        {
            str1_block.push_back(str1.substr(i * nblock));
        }
        if (i * nblock >= m)
        {
            str2_block.push_back("");
        }
        else
        {
            str2_block.push_back(str2.substr(i * nblock));
        }
    }
    int ans = 0;
    for (int i = 0; i < nblock; i++)
    {
        ans += bsd(str1_block[i], str2_block[i], q);
    }
    return ans;
}


vector<vector<string>> compute_comm(vector<string> S, int r, int q, int w, int l, int theta_low, int theta_high)
{
    int len = w + l;
    int num_core = 4;
    vector<vector<string>> C(S.size());
    // Initialization
    for(int i = 0; i < S.size(); i++)
    {
        C[i].push_back(S[i]);
    }

    for(int comm_step = 0; comm_step < 20; comm_step++)
    {
        cout << "Communication Step : " << comm_step << endl;
        // Random anchor    
        string anchor = random_anchor(w);

        // Partition map
        vector<vector<vector<string>>> Partition(num_core);
        // Get representatives and compute hash value
        for(int i = 0; i < C.size(); i++)
        {
            string sample = random_sample(C[i]);
            string hash_val = compute_hash(sample, anchor, len);
            //bitset<20> bit_val(hash_val);
            //srand(bit_val.to_ulong());
            srand((unsigned)time(NULL));
            Partition[rand() % num_core].push_back(C[i]);
        }
        // Reset S
        C.resize(0);
        for(int i = 0; i < Partition.size(); i++)
        {
            auto new_C = compute_local(Partition[i], r, q, w, l, theta_low, theta_high);
            C.insert(C.begin(), new_C.begin(), new_C.end());
        }
    }
    return C;
}

vector<vector<string>> compute_local(vector<vector<string>> C, int r, int q, int w, int l, int theta_low, int theta_high)
{
    int len = w + l;
    // Local Steps
    for(int i = 0; i < 20; i++)
    {
        // Random anchor    
        string anchor = random_anchor(w);
        vector<string> bucket;

        // Group list
        vector<int> group;
        // Get representatives and compute hash value
        for(int j = 0; j < C.size(); j++)
        {
            group.push_back(j);
            string sample = random_sample(C[j]);
            bucket.push_back(compute_hash(sample, anchor, len));
        }
        // Put in bucket and merge clusters
        for(int j = 0; j < bucket.size(); j++)
        {
            for(int k = j + 1; k < bucket.size(); k++)
            {
                string str1 = bucket[k], str2 = bucket[k];
                if(str1.compare(str2))
                {
                    int distance = blocking_bsd(str1, str2, 22, q);
                    if(distance <= theta_low || (distance <= theta_high && editDistance(str1, str2, str1.size(), str2.size())))
                    {
                        // Merge
                        group[k] = group[j];
                    }
                }
            }
        }
        // Update the cluster
        unordered_map<int, vector<string>> group_map;
        for(int j = 0; j < group.size(); j++)
        {
            group_map[group[j]].insert(group_map[group[i]].end(), C[j].begin(), C[j].end());
        }
        vector<vector<string>> new_C;
        for(auto & it : group_map)
        {
            new_C.push_back(it.second);
        }
        C = new_C;
    }
    return C;
}

string random_anchor(int w)
{
    srand((unsigned)time(NULL));
    string anchor = "";
    for(int i = 0; i < w; i++)
    {
        anchor.append(dna_alphabet[rand() % 4]);
    }
    return anchor;
}

string random_sample(vector<string> cur)
{
    return cur[rand() % cur.size()];
}