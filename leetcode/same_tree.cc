// https://leetcode.com/problems/same-tree/
// 二叉树相等

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
    bool isSameTree(TreeNode *p, TreeNode *q) {
      // judge the root node
      if (p==NULL && q==NULL) return true;
      if (p==NULL || q==NULL) return false;
      if (p->val != q->val) return false;
      // judge the left and right node
      return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
    }
};
