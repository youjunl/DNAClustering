#include"model.h"

int editDistance(string str1, string str2, int m, int n)
{
    if(m == 0 || n == 0)
    {
        return m == 0 ? n : m; 
    }

    // If no error
    if(str1[m-1] == str2[n-1])
    {
        return editDistance(str1, str2, m - 1, n - 1);
    }

    // If has error
    return 1 + min( editDistance(str1, str2, m, n - 1), 
                    editDistance(str1, str2, m - 1, n),
                    editDistance(str1, str2, m - 1, n - 1)
                    );
}

string hash(string str, string anchor, int len)
{
    int win = anchor.size() + len;
    size_t pos = str.find_first_of(anchor, 0);
    // Return the first found substring else return last len characters
    return pos != string::npos ? str.substr(pos, len) : str.substr(str.size() - len);
}

int bsd(string str1, string str2, const int q)
{
    // Indicator vector
    int* d;
    memset(d, 0, (int)pow(4, q));

    int ans = 0;
    // TO DO...
    return ans;
}