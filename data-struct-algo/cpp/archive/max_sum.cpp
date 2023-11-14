#include <climits>
#include <cstdio>
#include <utility>
#include <vector>

int main() {
    int t, n, x;
    scanf("%d", &t);

    std::vector<int> prefix_sum;
    // (index, max)
    std::vector<std::pair<int, int>> max_right;

    for (int case_i = 1; case_i <= t; case_i++) {
        scanf("%d", &n);
        prefix_sum.resize(n);
        max_right.resize(n);
        int sum = 0;

        for (int i = 0; i < n; i++) {
            scanf("%d", &x);
            sum += x;
            prefix_sum[i] = sum;
        }

        int max_i = n, max = INT_MIN;
        for (int i = n - 1; i >= 0; i--) {
            x = prefix_sum[i];
            if (x > max) {
                max_i = i;
                max = x;
            }
            max_right[i] = {max_i, max};
        }

        int min_i = 0, min_left = 0;
        int max_sum = INT_MIN, max_sum_l = 0, max_sum_r = 0;
        for (int i = 0; i < n; i++) {
            sum = max_right[i].second - min_left;
            if (sum > max_sum) {
                max_sum = sum;
                max_sum_l = min_i;
                max_sum_r = max_right[i].first;
            }
            x = prefix_sum[i];
            if (x < min_left) {
                min_i = i + 1;
                min_left = x;
            }
        }
        printf("Case %d:\n%d %d %d\n\n", case_i, max_sum, max_sum_l + 1,
               max_sum_r + 1);
    }
    return 0;
}