#include <iostream>
#include <vector>
using namespace std;

void insertion_sort(vector<int> &nums) {
  for (int i=1; i<nums.size(); i++) {
    int target=nums[i];
    int j=i-1;
    while (j>=0 && nums[j]>target) {
      nums[j+1] = nums[j];
      j--;
    }
    nums[j+1] = target;
  }
}

// selection-sort
void selection_sort(vector<int> &nums) {
  for (int i=0; i<nums.size()-1; i++) {
    int target=i;
    for (int j=i+1; j<nums.size(); j++) {
      if (nums[j] < nums[target]) {
        target = j;
      }
    }
    if (target != i) swap(nums[i], nums[target]);
  }
}

// exchange-sort
void bubble_sort(vector<int> &nums) {
  for (int i=nums.size()-1; i>0; i--) {
    for (int j=0; j<i; j++) {
      if (nums[j] > nums[j+1]) {
        swap(nums[j], nums[j+1]);
      }
    }
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
  //bubble_sort(nums);
  //selection_sort(nums);
  insertion_sort(nums);
  print_vector(nums);
  return 0;
}
