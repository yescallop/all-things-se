#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

struct string_hash {
    using hash_type = hash<string_view>;
    using is_transparent = void;

    size_t operator()(string_view str) const { return hash_type{}(str); }
    size_t operator()(const string &str) const { return hash_type{}(str); }
    size_t operator()(const string *str) const { return hash_type{}(*str); }
};

struct string_equal_to {
    using is_transparent = void;

    bool operator()(const string *lhs, const string *rhs) const {
        return *lhs == *rhs;
    }
    bool operator()(const string &lhs, const string *rhs) const {
        return lhs == *rhs;
    }
    bool operator()(const string *lhs, const string &rhs) const {
        return *lhs == rhs;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    unordered_map<string, string, string_hash, equal_to<>> fwd_map;
    unordered_map<const string *, const string *, string_hash, string_equal_to>
        bwd_map;

    string line;
    while (getline(cin, line)) {
        if (line == "@END@")
            break;
        size_t i = line.find(']');
        fwd_map[move(line.substr(1, i - 1))] = move(line.substr(i + 2));
    }

    for (auto &[name, desc] : fwd_map) {
        bwd_map[&desc] = &name;
    }

    int n;
    cin >> n;
    getline(cin, line);
    for (int i = 0; i < n; i++) {
        getline(cin, line);
        if (line.starts_with('[')) {
            string_view name(line.data() + 1, line.size() - 2);
            auto it = fwd_map.find(name);
            if (it != fwd_map.end())
                cout << it->second << '\n';
            else
                cout << "what?\n";
        } else {
            auto it = bwd_map.find(line);
            if (it != bwd_map.end())
                cout << *it->second << '\n';
            else
                cout << "what?\n";
        }
    }
    return 0;
}
