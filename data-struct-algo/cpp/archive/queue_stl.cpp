#include <cstdio>
#include <queue>

int main() {
    int n;
    scanf("%d", &n);

    std::queue<int> queue;

    for (int i = 0; i < n; i++) {
        int op, x;
        scanf("%d", &op);
        switch (op) {
        case 1:
            scanf("%d", &x);
            queue.push(x);
            break;
        case 2:
            if (queue.empty())
                printf("ERR_CANNOT_POP\n");
            else
                queue.pop();
            break;
        case 3:
            if (queue.empty())
                printf("ERR_CANNOT_QUERY\n");
            else
                printf("%d\n", queue.front());
            break;
        case 4:
            printf("%d\n", queue.size());
            break;
        }
    }
    return 0;
}