#include <iostream>
#include <vector>
#include <set>
using namespace std;

class Board {
public:
  Board(vector<vector<int>>& input) : hash_ok(123456780), board(input) {
    detectHole();
  }
  void move(int command) {
    switch (command) {
      case 1 : swap(board[i][j], board[i-1][j]); i--; break;
      case 2 : swap(board[i][j], board[i][j-1]); j--; break;
      case 3 : swap(board[i][j], board[i][j+1]); j++; break;
      case 4 : swap(board[i][j], board[i+1][j]); i++; break;
      default : break;
    }
  }
  bool succeed() {
    int hash_val = hash_func();
    hash_record.insert(hash_val);
    return hash_val == hash_ok;
  }
  bool validCommand(int command) {
    if (i==0 && command==1) return false;
    if (i==2 && command==4) return false;
    if (j==0 && command==2) return false;
    if (j==2 && command==3) return false;
    // 1
    move(command);
    bool res = hash_record.find(hash_func())!=hash_record.end();
    move(5-command);
    if (res) return false;
    return true;
  }
  void detectHole() {
    for (int i=0; i<3; i++)
      for (int j=0; j<3; j++) {
        if (board[i][j]==0) {
          this->i = i;
          this->j = j;
        }
      }
  }
private:
  int i, j;
  vector<vector<int>> board;
  int hash_ok;
  set<int> hash_record;
  int hash_func() {
    int hash_val = 0;
    for (int i=0; i<3; i++) {
      for (int j=0; j<3; j++) {
        hash_val = hash_val*10 + board[i][j];
      }
    }
    return hash_val;
  }
};

class Solution {
private:
  Board *b;
public:
  Solution() {}
  ~Solution() {
    delete b;
  }
  void init(vector<vector<int>>& input) {
    b = new Board(input);
  }
  bool process() {
    if (b->succeed()) return true;
    for (int i=1; i<=4; i++) {
      // 2
      if (!b->validCommand(i)) continue;
      b->move(i);
      if (process()) return true;
      b->move(5-i);
    }
    return false;
  }
};

vector<string> split(string& source, string separator) {
  vector<string> ans;
  int l = 0;
  int r = source.find(separator, l);
  while (r != string::npos) {
    string str = source.substr(l, r-l);
    ans.push_back(str);
    l = r + separator.size();
    r = source.find(separator, l);
  }
  ans.push_back(source.substr(l));
  return ans;
}

vector<vector<int>> trans(vector<string>& input) {
  vector<vector<int>> ans;
  vector<int> vec_tmp;
  for (int i=0; i<input.size(); i++) {
    vec_tmp.push_back(atoi(input[i].c_str()));
    if (i % 3 == 2) {
      ans.push_back(vec_tmp);
      vec_tmp.clear();
    }
  }
  return ans;
}

int main() {
  string line = "";
  while (cin >> line) {
    vector<string> vec_line = split(line, ",");
    vector<vector<int>> input = trans(vec_line);
    Solution s;
    s.init(input);
    cout << s.process() << endl;
  }
  return 0;
}
