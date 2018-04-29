// https://leetcode.com/problems/validate-binary-search-tree/
// 二叉树合法

// Solution 3, inorder traversal
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
    bool isValidBST(TreeNode *root) {
      stack<TreeNode *> st;
      TreeNode *p=root, *last=NULL;
      while (p!=NULL || !st.empty()) {
        if (p != NULL) {
          st.push(p);
          p = p->left;
        }
        else {
          p = st.top();
          if (last!=NULL && last->val>=p->val) return false;
          st.pop();
          last = p;
          p = p->right;
        }
      }
      return true;
    }
};

// Solution 2, beautiful top-down
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
    bool isValidBST(TreeNode *root) {
      long max_val=(long)INT_MAX+1;
      long min_val=(long)INT_MIN-1;
      return helper(root, min_val, max_val);
    }

    bool helper(TreeNode *root, long min_val, long max_val) {
      if (root == NULL) return true; // search till the NULL node, waste of time
      if (root->val <= min_val || root->val >= max_val) return false;
      return helper(root->left, min_val, root->val) && helper(root->right, root->val, max_val);
    }
};

// Solution 1, ugly down-top
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
    bool isValidBST(TreeNode *root) {
      if (root == NULL) return true;
      int min_val, max_val;
      return helper(root, min_val, max_val);
    }

    bool helper(TreeNode *root, int& min_val, int& max_val) {
      if (root->left == NULL && root->right == NULL) {
        min_val = root->val;
        max_val = root->val;
        return true;
      }
      int left_min_val, left_max_val, right_min_val, right_max_val;
      if (root->left != NULL) {
        if (!helper(root->left, left_min_val, left_max_val)) return false;
      }
      if (root->right != NULL) {
        if (!helper(root->right, right_min_val, right_max_val)) return false;
      }
      if ((root->left != NULL && left_max_val >= root->val) ||
          (root->right != NULL && right_min_val <= root->val)) return false;
      if (root->left == NULL) min_val = root->val;
      else min_val = left_min_val;
      if (root->right == NULL) max_val = root->val;
      else max_val = right_max_val;
      return true;
    }
};
