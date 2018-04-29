// 0～n的整数，二进制表示中1的个数
// 数字

class Solution {
  public:
    vector<int> countBits(int num) {
      vector<int> result;
      result.push_back(0);
      for (int i=1; i<=num; i++){
        int last_num=result.back(), curr_num=last_num+1, n=i-1;
        while (n%2!=0) {
          curr_num--;
          n = n/2;
        }
        result.push_back(curr_num);
      }
      return result;
    }
};
