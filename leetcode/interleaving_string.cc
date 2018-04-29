// https://leetcode.com/problems/interleaving-string/
// 字符串

//Solution 2, dynamic programming
class Solution {
  public:
    bool isInterleave(string s1, string s2, string s3) {
      // corner case
      if (s1.size() + s2.size() != s3.size()) return false;
      int m=s1.size(), n=s2.size();
      // initialize dynamic array d[][]
      bool* *d=new bool*[m+1];
      for (int i=0; i<=m; i++) {
        d[i] = new bool[n+1];
        memset(d[i], 0, sizeof(bool)*(n+1));
      }
      d[0][0] = true;
      // go
      for (int i=0; i<=m; i++) {
        for (int j=0; j<=n; j++) {
          if (i==0 && j==0) continue;
          if (i==0) {
            d[i][j] = d[i][j-1] && (s2[j-1]==s3[i+j-1]);
          }
          else if (j==0) {
            d[i][j] = d[i-1][j] && (s1[i-1]==s3[i+j-1]);
          }
          else {
            d[i][j] = (d[i][j-1] && (s2[j-1]==s3[i+j-1])) || (d[i-1][j] && (s1[i-1]==s3[i+j-1]));
          }
        }
      }
      return d[m][n];
    }
};

// Solution 1, time limit excceeded
class Solution {
  public:
    bool isInterleave(string s1, string s2, string s3) {
      if (s1.size() + s2.size() != s3.size()) return false;
      return helper(s1, s2, s3, 0, 0, 0);
    }

    bool helper(string& s1, string& s2,  string& s3, int left1, int left2, int left3) {
      // s3 reach the end
      if (left3 == s3.size()) return true;
      // s1 reach the end
      if (left1 == s1.size()) {
        if (s2.substr(left2) == s3.substr(left3)) return true;
        else return false;
      }
      // s2 reach the end
      if (left2 == s2.size()) {
        if (s1.substr(left1) == s3.substr(left3)) return true;
        else return false;
      }
      // both s1 and s2 not reach the end
      if (s1[left1] == s3[left3] && s2[left2] == s3[left3]) {
        return helper(s1, s2, s3, left1+1, left2, left3+1) || helper(s1, s2, s3, left1, left2+1, left3+1);
      }
      else if (s1[left1] == s3[left3]) {
        return helper(s1, s2, s3, left1+1, left2, left3+1);
      }
      else if (s2[left2] == s3[left3]) {
        return helper(s1, s2, s3, left1, left2+1, left3+1);
      }
      else return false;
    }
};
