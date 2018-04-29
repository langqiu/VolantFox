// https://leetcode.com/problems/binary-tree-inorder-traversal/
// 二叉树中序遍历

// Solution 2, iterative
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
    vector<int> inorderTraversal(TreeNode *root) {
      vector<int> result;
      stack<TreeNode *> st;
      TreeNode *p=root;
      while (p!=NULL || !st.empty()) {
        if (p != NULL) {
          st.push(p);
          p = p->left;
        }
        else {
          p = st.top();
          st.pop();
          result.push_back(p->val);
          p = p->right;
        }
      }
      return result;
    }
};

// Solution 1, recursion
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
    vector<int> inorderTraversal(TreeNode *root) {
      vector<int> result;
      if (root != NULL) helper(root, result);
      return result;
    }

    void helper(TreeNode *root, vector<int>& result) {
      if (root->left == NULL && root->right == NULL) {
        result.push_back(root->val);
        return;
      }
      if (root->left != NULL) helper(root->left, result);
      result.push_back(root->val);
      if (root->right != NULL) helper(root->right, result);
      return;
    }
};


// Solution 3, recursion, waste of time
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
    vector<int> inorderTraversal(TreeNode *root) {
      vector<int> result;
      return helper(root, result);
      return result;
    }

    void helper(TreeNode *root, vector<int>& result) {
      if (root == NULL) return;
      helper(root->left, result);
      result.push_back(root->val);
      helper(root->right, result);
      return;
    }
};
