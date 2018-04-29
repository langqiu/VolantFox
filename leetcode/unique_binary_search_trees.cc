// https://leetcode.com/problems/unique-binary-search-trees/
// 二叉树形态个数

class Solution {
  public:
    int numTrees(int n) {
      if (n==0) return 1;
      int* d=new int[n+1];
      memset(d, 0, sizeof(int)*(n+1));
      d[0] = 1;
      d[1] = 1;
      for (int i=2; i<=n; i++) {
        for (int j=1; j<=i; j++) {
          d[i] += d[j-1]*d[i-j];
        }
      }
      return d[n];
    }
};
