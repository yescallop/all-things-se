#include <algorithm>
#include <cstdio>
#include <vector>

struct Interval {
    int start;
    int end;
    int efficiency;
};

bool cmp(const Interval &a, const Interval &b) { return a.end < b.end; }

bool pred(const Interval &a, int start) { return a.end <= start; }

int main() {
    int n, m, r;
    scanf("%d%d%d", &n, &m, &r);

    std::vector<Interval> intervals(m);
    for (int i = 0; i < m; i++) {
        scanf("%d%d%d", &intervals[i].start, &intervals[i].end,
              &intervals[i].efficiency);
        intervals[i].end += r;
    }
    std::sort(intervals.begin(), intervals.end(), cmp);

    std::vector<int> last_available(m);
    for (int i = 0; i < m; i++) {
        std::vector<Interval>::iterator begin = intervals.begin();
        std::vector<Interval>::iterator it =
            std::lower_bound(begin, begin + i, intervals[i].start, pred);
        last_available[i] = it - begin;
    }
    // std::fill(last_available.begin(), last_available.end(), 0);
    // for (int i = 0; i < m; i++) {
    //     for (int j = i - 1; j >= 0; j--) {
    //         if (intervals[j].end <= intervals[i].start) {
    //             last_available[i] = j + 1;
    //             break;
    //         }
    //     }
    // }

    std::vector<int> sol(m + 1);
    for (int i = 0; i < m; i++) {
        sol[i + 1] =
            std::max(sol[i], sol[last_available[i]] + intervals[i].efficiency);
    }
    printf("%d\n", sol[m]);
    return 0;
}