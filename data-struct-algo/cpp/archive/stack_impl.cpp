#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

/// A stack implemented with a growable buffer.
template <class T> class Stack {
    // A pointer to the allocated buffer.
    T *ptr = nullptr;
    // The capacity of the buffer.
    size_t cap = 0;
    // The length of the stack.
    size_t len = 0;

    /// Grows the buffer.
    void grow() {
        size_t new_cap = cap == 0 ? 1 : cap * 2;
        ptr = realloc(ptr, new_cap);
        cap = new_cap;
    }

  public:
    /// Destructs the stack.
    ~Stack() {
        for (size_t i = 0; i < len; i++)
            ptr[i].~T();
        free(ptr);
    }

    /// Returns the number of elements in the stack.
    size_t size() const { return len; }

    /// Checks whether the stack is empty.
    bool empty() const { return len == 0; }

    /// Returns a reference to the last element in the stack.
    ///
    /// Calling this method on an empty stack is undefined behavior.
    T &back() const { return ptr[len - 1]; }

    /// Appends an element to the back of the stack.
    void push(const T &value) {
        if (len == cap)
            grow();
        ptr[len++] = value;
    }

    /// Removes the last element in the stack.
    ///
    /// Calling this method on an empty stack is undefined behavior.
    void pop() { ptr[--len].~T(); }

    /// Clears the stack.
    void clear() {
        for (size_t i = 0; i < len; i++)
            ptr[i].~T();
        len = 0;
    }
};

int main() {
    int t, n;
    scanf("%d", &t);

    Stack<uint64_t> stack;

    for (int i = 0; i < t; i++) {
        stack.clear();
        scanf("%d", &n);

        for (int j = 0; j < n; j++) {
            char op[6];
            scanf("%5s", op);

            if (!strcmp(op, "push")) {
                uint64_t x;
                scanf("%lu", &x);
                stack.push(x);
            } else if (!strcmp(op, "pop")) {
                if (stack.empty())
                    printf("Empty\n");
                else
                    stack.pop();
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