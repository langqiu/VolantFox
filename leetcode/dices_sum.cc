#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

class Solution {
public:
  vector<pair<int, double>> dicesSum(int n) {
    float** d = new float*[n+1];
    for (int i=0; i<=n; i++) {
      d[i] = new float[6*i+1];
      memset(d[i], 0, sizeof(float)*(6*i+1));
    }
    //
    for (int i=1; i<=n; i++) {
      for (int j=i; j<=6*i; j++) {
        if (i==1) {
          d[i][j] = 1;
          continue;
        }
        for (int k=1; k<=6; k++) {
          if (j-k>=i-1 && j-k<=6*(i-1)) d[i][j] += d[i-1][j-k];
        }
      }
    }
    //
    float sum = pow(6, n);
    vector<pair<int, double>> ans;
    for (int j=n; j<=6*n; j++) {
      double p = floor((d[n][j]/sum)*100+0.5)/100;
      ans.push_back(make_pair(j, p));
    }
    return ans;
  }
};

int main() {
  Solution s;
  vector<pair<int, double>> ans;
  int n;
  while (cin >> n) {
    ans = s.dicesSum(n);
    for (auto vec : ans) {
      cout << "[" << vec.first << "," << vec.second << "]" << endl;
    }
  }
  return 0;
}
