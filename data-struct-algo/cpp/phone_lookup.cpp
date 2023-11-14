#include <iostream>
#include <unordered_map>

using namespace std;

int main() {
    int n;
    cin >> n;

    string line;
    getline(cin, line);
    unordered_map<string, string> map;

    for (int i = 0; i < n; i++) {
        getline(cin, line);
        size_t pos = line.find(',');
        if (pos == string::npos)
            continue;
        map[line.substr(0, pos)] = line.substr(pos + 1);
    }

    int m;
    cin >> m;
    getline(cin, line);

    for (int i = 0; i < m; i++) {
        getline(cin, line);
        cout << line << ':';
        auto it = map.find(line);
        if (it == map.end())
            cout << "No";
        else
            cout << it->second;
        cout << '\n';
    }
}