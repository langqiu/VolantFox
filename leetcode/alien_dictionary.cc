#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>
using namespace std;

class Solution {
public:
  string alienDict(vector<string>& input) {
    map<char, int> indegree;
    for (int i=0; i<input.size(); i++)
      for (int j=0; j<input[i].size(); j++)
        indegree[input[i][j]] = 0;
    //
    map<char, set<char>> graph;
    for (int i=0; i<input.size()-1; i++) {
      string word1 = input[i], word2 = input[i+1];
      int j = 0;
      while (j<word1.size() && j<word2.size()) {
        if (word1[j]==word2[j]) {
          j++;
          continue;
        }
        if (graph.find(word1[j])==graph.end()) {
          graph[word1[j]] = set<char>{word2[j]};
          indegree[word2[j]]++;
        } else {
          if (graph[word1[j]].find(word2[j])==graph[word1[j]].end()) {
            graph[word1[j]].insert(word2[j]);
            indegree[word2[j]]++;
          }
        }
        break;
      }
    }
    //
    for (auto kv : graph) {
      cout << kv.first << ":";
      for (auto s : kv.second) {
        cout << s << ",";
      }
      cout << endl;
    }
    string ans;
    queue<char> q;
    for (auto kv : indegree) {
      if (kv.second == 0) {
        q.push(kv.first);
        ans += kv.first;
      }
    }
    while (!q.empty()) {
      char ch = q.front();
      cout << ch << endl;
      if (graph.find(ch) != graph.end()) {
        for (auto k : graph[ch]) {
          indegree[k]--;
          if (indegree[k]==0) {
            q.push(k);
            ans += k;
          }
        }
      }
      q.pop();
    }
    if (ans.size()!=indegree.size()) ans = "";
    return ans;
  }
};

int main() {
  Solution s;
  vector<string> input = {"fft", "fcp", "aac", "act", "acd", "atp", "tbk", "tdf"};
  for (auto v : input) {
    cout << v << endl;
  }
  cout << s.alienDict(input) << endl;
  return 0;
}
