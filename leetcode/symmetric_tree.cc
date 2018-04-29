// https://leetcode.com/problems/symmetric-tree/
// 二叉树自身对称

// Solution 3, iterative
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
    bool isSymmetric(TreeNode *root) {
      // corner case
      if (root==NULL) return true;
      // intialize queue_left, queue_right
      queue<TreeNode *> queue_left, queue_right;
      queue_left.push(root->left);
      queue_right.push(root->right);
      TreeNode *p_left, *p_right;
      // while
      while (!queue_left.empty() && !queue_right.empty()) {
        // get left and right
        p_left = queue_left.front();
        p_right = queue_right.front();
        queue_left.pop();
        queue_right.pop();
        // judge NULL
        if (p_left==NULL && p_right==NULL) continue;
        if (p_left==NULL || p_right==NULL) return false;
        // add left and right node into the queues
        if (p_left->val!=p_right->val) return false;
        queue_left.push(p_left->left);
        queue_left.push(p_left->right);
        queue_right.push(p_right->right);
        queue_right.push(p_right->left);
        // continue
      }
      // return
      return queue_left.empty() && queue_right.empty();
    }
};

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
    bool isSymmetric(TreeNode *root) {
      // corner case
      if (root==NULL) return true;
      if (root->left==NULL && root->right==NULL) return true;
      if (root->left==NULL || root->right==NULL) return false;
      // initialize st_node queue_node, queue_val
      stack<TreeNode *> st_node;
      queue<int> queue_node, queue_val;
      TreeNode *p=NULL;
      // while first half
      p = root->left;
      while (p!=NULL || !st_node.empty()) {
        if (p!=NULL) {
          st_node.push(p);
          queue_node.push(p->val);
          p = p->left;
        }
        else {
          p = st_node.top();
          queue_val.push(p->val);
          st_node.pop();
          p = p->right;
        }
      }
      // while second half
      p = root->right;
      while (p!=NULL || !st_node.empty()) {
        if (p!=NULL) {
          st_node.push(p);
          if (queue_node.empty()) return false;
          else if (queue_node.front()!=p->val) return false;
          else queue_node.pop();
          p = p->right;
        }
        else {
          p = st_node.top();
          if (queue_val.empty()) return false;
          else if (queue_val.front()!=p->val) return false;
          else queue_val.pop();
          st_node.pop();
          p = p->left;
        }
      }
      // return
      return queue_node.empty() && queue_val.empty();
    }
};

// Solution 1, recursive
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
    bool isSymmetric(TreeNode *root) {
      // root corner case
      if (root == NULL) return true;
      // left and right symmetric
      return helper(root->left, root->right);
    }

    bool helper(TreeNode *p, TreeNode *q) {
      // end case
      if (p==NULL && q==NULL) return true;
      if (p==NULL || q==NULL) return false;
      // judge current node
      if (p->val != q->val) return false;
      // recursion
      return helper(p->left, q->right) && helper(p->right, q->left);
    }
};
