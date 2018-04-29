#include <iostream>
#include <map>
using namespace std;

class TrieNode {
public:
  TrieNode() : c(' '), country(""), is_leaf(false) {}
  TrieNode(char ch) : c(ch), country(""), is_leaf(false) {}
  TrieNode* subNode(char ch) {
    if (children.find(ch) == children.end()) return NULL;
    return children[ch];
  }
  ~TrieNode() {
    for (auto kv : children) {
      if (kv.second != NULL) {
        delete kv.second;
        kv.second = NULL;
      }
    }
  }
  char c;
  string country;
  bool is_leaf;
  map<char, TrieNode*> children;
};

class Trie {
public:
  Trie() {
    root = new TrieNode();
  }
  void insert(string num, string c) {
    TrieNode* p = root;
    int i = 0;
    while (i<num.size()) {
      if (p->subNode(num[i])!=NULL) {
        p = p->subNode(num[i]);
      } else {
        TrieNode* temp = new TrieNode(num[i]);
        p->children[num[i]] = temp;
        p = temp;
      }
      i++;
    }
    p->is_leaf = true;
    p->country = c;
  }
  string search(string num) {
    TrieNode* p = root;
    int i = 0;
    while (i<num.size()) {
      p = p->subNode(num[i]);
      if (p==NULL) return "";
      i++;
    }
    return p->country;
  }
private:
  TrieNode* root;
};


class Solution {
public:
  string findTelCountry(string num) {
    return root->search(num);
  }
  void init(map<string, string>& input) {
    root = new Trie();
    for (auto kv : input) {
      root->insert(kv.first, kv.second);
    }
  }
private:
  Trie* root;
};

int main() {
  map<string, string> input = {{"+1","North America"},{"+1950","Northern Cal"},{"+44","UK"},{"+4420","London"},{"+447","UK Mobile"},{"+44750","Vodafoned"}};
  Solution s;
  s.init(input);
  string number;
  while (cin>>number) {
    cout << s.findTelCountry(number) << endl;
  }
  return 0;
}
