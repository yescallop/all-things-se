#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

int main() {
    int t, n;
    scanf("%d", &t);

    std::vector<uint64_t> stack;

    for (int i = 0; i < t; i++) {
        stack.clear();
        scanf("%d", &n);

        for (int j = 0; j < n; j++) {
            char op[6];
            scanf("%5s", op);

            if (!strcmp(op, "push")) {
                uint64_t x;
                scanf("%lu", &x);
                stack.push_back(x);
            } else if (!strcmp(op, "pop")) {
                if (stack.empty())
                    printf("Empty\n");
                else
                    stack.pop_back();
            } else if (!strcmp(op, "query")) {
                if (stack.empty())
                    printf("Anguei!\n");
                else
                    printf("%lu\n", stack.back());
            } else if (!strcmp(op, "size")) {
                printf("%lu\n", stack.size());
            }
        }
    }
    return 0;
}