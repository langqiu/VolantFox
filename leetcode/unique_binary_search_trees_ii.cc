// https://leetcode.com/problems/unique-binary-search-trees-ii/
// 二叉树形态遍历

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
  vector<TreeNode *> generateTrees(int n) {
    vector<TreeNode *> result;
    if (n!=0) {
      helper(1, n, result);
    }
    return result;
  }

  void helper(int start, int end, vector<TreeNode *>& result) {
    if (start > end) {
      result.push_back(NULL);
      return;
    }
    for (int i=start; i<=end; i++) {
      vector<TreeNode *> vec_left, vec_right;
      helper(start, i-1, vec_left);
      helper(i+1, end, vec_right);
      //
      for (int j=0; j<vec_left.size(); j++) {
        for (int k=0; k<vec_right.size(); k++) {
          TreeNode *root = new TreeNode(i);
          root->left = vec_left[j];
          root->right = vec_right[k];
          result.push_back(root);
        }
      }
    }
    return;
  }
};
