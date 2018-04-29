// https://leetcode.com/problems/binary-tree-level-order-traversal/
// 二叉树广度优先遍历

// Solution 2
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
    vector<vector<int>> levelOrder(TreeNode *root) {
      // corner case and initialize
      vector<vector<int>> result;
      queue<TreeNode *> queue_node;
      if (root!=NULL) queue_node.push(root);
      // while
      while (!queue_node.empty()) {
        // count the nodes in current level
        int num=queue_node.size();
        // for loop
        result.push_back(vector<int>{});
        for (int i=1; i<=num; i++) {
          // get the node
          TreeNode *p=queue_node.front();
          queue_node.pop();
          // output the node
          result.back().push_back(p->val);
          // add the left and right node in the next level
          if (p->left!=NULL) queue_node.push(p->left);
          if (p->right!=NULL) queue_node.push(p->right);
        }
      }
      // return
      return result;
    }
};

// Solution 1
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
    vector<vector<int>> levelOrder(TreeNode *root) {
      // corner case and initialize
      vector<vector<int>> result;
      queue<pair<TreeNode *, int>> queue_node;
      pair<TreeNode *, int> p;
      if (root!=NULL) queue_node.push(make_pair(root, 0));
      // while
      while (!queue_node.empty()) {
        // get front node and push the value into result
        p = queue_node.front();
        queue_node.pop();
        int level=p.second;
        TreeNode *node=p.first;
        if (level==result.size()) result.push_back(vector<int>{node->val});
        else result[level].push_back(node->val);
        // push left and right node into the queue
        if (node->left!=NULL) queue_node.push(make_pair(node->left, level+1));
        if (node->right!=NULL) queue_node.push(make_pair(node->right, level+1));
      }
      // return
      return result;
    }
};
