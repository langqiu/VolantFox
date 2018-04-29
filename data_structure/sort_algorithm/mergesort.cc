#include <iostream>
#include <vector>
using namespace std;

void mergesort(vector<int> &nums) {
  int n=nums.size();
  if (n == 1) return;
  int l=0, r=n-1, m=(l+r)/2;
  vector<int> l_nums, r_nums;
  for (int i=0; i<=m; i++) {
    l_nums.push_back(nums[i]);
  }
  mergesort(l_nums);
  for (int i=m+1; i<=r; i++) {
    r_nums.push_back(nums[i]);
  }
  mergesort(r_nums);
  // merge
  int i=0, j=0;
  while (i<=m && j<=r-m-1) {
    if (l_nums[i] < r_nums[j]) {
      nums[i+j] = l_nums[i];
      i++;
    }
    else {
      nums[i+j] = r_nums[j];
      j++;
    }
  }
  while (i <= m) {
    nums[i+j] = l_nums[i];
    i++;
  }
  while (j <= r-m-1) {
    nums[i+j] = r_nums[j];
    j++;
  }
}

void print_vector(vector<int> &nums) {
  for (int i=0; i<nums.size(); i++) {
    if (i != 0) cout << ",";
    cout << nums[i];
  }
  cout << endl;
}

int main(int argc, char *argv[])
{
  vector<int> nums={4, 1, 3, 2, 16, 9, 10, 14, 8, 7};
  mergesort(nums);
  print_vector(nums);
  return 0;
}
