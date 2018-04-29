// https://leetcode.com/problems/recover-binary-search-tree/
// 二叉树复原

// another solution, Morris Traversal

// Solution 1, stack iterative
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
    void recoverTree(TreeNode *root) {
      // find the two swapped node
      TreeNode *first=NULL, *second=NULL, *last=NULL;
      stack<TreeNode *> st;
      TreeNode *p=root;
      while (p!=NULL || !st.empty()) {
        if (p != NULL) {
          st.push(p);
          p = p->left;
        }
        else {
          p = st.top();
          if (first==NULL && last!=NULL && last->val>p->val) first = last;
          if (second==NULL && first!=NULL && p->val>first->val) second = last;
          st.pop();
          last = p;
          p = p->right;
        }
      }
      if (second == NULL) second = last;
      swap(first->val, second->val);
    }
};

// Solution 2, recursion
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
    void recoverTree(TreeNode *root) {
      TreeNode *first=NULL, *second=NULL, *last=NULL;
      helper(root, first, second, last);
      if (second == NULL) second = last;
      swap(first->val, second->val);
      return;
    }

    void helper(TreeNode *root, TreeNode *&first, TreeNode *&second, TreeNode *&last) {
      if (root == NULL) return;
      helper(root->left, first, second, last);
      if (last!=NULL && first==NULL && last->val>root->val) first = last;
      if (second==NULL && first!=NULL && root->val>first->val) second = last;
      last = root;
      helper(root->right, first, second, last);
      return;
    }
};
