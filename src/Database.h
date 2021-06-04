#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

using std::fstream;
using std::map;
using std::shared_lock;
using std::string;
using std::to_string;
using std::unique_lock;
using std::vector;

class Database {
public:
  Database(void) : m() {
    file = fstream("database", fstream::in | fstream::out);

    string abc;
    char info[1033];

    // file.seekg (0, file.end);
    // int length = file.tellg();

    // file.seekg(0);
    int counter = 0;
    while (file.read(info, 1033)) {

      vector<string> v;
      std::stringstream ss(info);
      while (ss.good()) {
        string substr;
        getline(ss, substr, ',');
        v.push_back(substr);
      }
      long long key = stoll(v[0]);
      positions[key] = 1033 * counter++;
    }
    file.close();
    file = fstream("database", fstream::in | fstream::out);
  }

  ~Database(void) {}

  bool put(long long key, string value) {

    unique_lock lock(m);

    file.seekg(0, file.end);
    int length = file.tellg();

    if (positions.find(key) != positions.end()) {
      file.seekg(positions[key]);
      file.write((to_string(key) + ',' + value + '\n').c_str(), 1033);

      return true;
    }

    file.write((to_string(key) + ',' + value + '\n').c_str(), 1033);
    positions[key] = length;

    return true;
  }

  string get(long long key) {

    shared_lock lock(m);

    if (positions.find(key) != positions.end()) {

      file.seekg(positions[key]);
      string result;
      getline(file, result);

      return result;
    }
    return "erro";
  }

private:
  fstream file;
  map<long long, long long> positions;
  mutable std::shared_mutex m;
};
