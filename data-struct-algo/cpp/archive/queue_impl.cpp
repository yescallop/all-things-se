#include <cstdio>
#include <cstring>
#include <memory>
#include <utility>

/// A queue implemented with a growable ring buffer.
template <class T> class Queue {
    // A pointer to the allocated buffer.
    T *ptr = nullptr;
    // The capacity of the buffer.
    size_t cap = 0;
    // The index of the first element in the queue.
    size_t head = 0;
    // The length of the queue.
    size_t len = 0;

    /// Returns a pair of lengths indicating how many consecutive elements
    /// there are in the buffer starting from indexes `head` and `0`.
    ///
    /// It is guaranteed that the two lengths sum to `len`.
    std::pair<size_t, size_t> lens() const {
        size_t head_len = len, tail_len = 0;
        if (head + len > cap) {
            head_len = cap - head;
            tail_len = len - head_len;
        }
        return {head_len, tail_len};
    }

    /// Grows the buffer.
    void grow() {
        if (cap == 0) {
            ptr = malloc(sizeof(T));
            cap = 1;
            return;
        }

        size_t new_cap = cap * 2;
        T *new_ptr = malloc(new_cap * sizeof(T));
        auto [head_len, tail_len] = lens();
        memcpy(new_ptr, ptr + head, head_len * sizeof(T));
        memcpy(new_ptr + head_len, ptr, tail_len * sizeof(T));
        free(ptr);

        cap = new_cap;
        ptr = new_ptr;
        head = 0;
    }

  public:
    /// Destructs the queue.
    ~Queue() {
        auto [head_len, tail_len] = lens();
        for (size_t i = 0; i < head_len; i++)
            ptr[head + i].~T();
        for (size_t i = 0; i < tail_len; i++)
            ptr[i].~T();
        free(ptr);
    }

    /// Returns the number of elements in the queue.
    size_t size() const { return len; }

    /// Checks whether the queue is empty.
    bool empty() const { return len == 0; }

    /// Returns a reference to the first element in the queue.
    ///
    /// Calling this method on an empty queue is undefined behavior.
    T &front() const { return ptr[head]; }

    /// Appends an element to the back of the queue.
    void push(const T &value) {
        if (len == cap)
            grow();

        size_t i = head + len;
        if (i >= cap)
            i -= cap;
        ptr[i] = value;
        len++;
    }

    /// Removes the first element in the queue.
    ///
    /// Calling this method on an empty queue is undefined behavior.
    void pop() {
        ptr[head].~T();
        head++;
        if (head == cap)
            head = 0;
        len--;
    }
};

int main() {
    int n;
    scanf("%d", &n);

    Queue<int> queue;

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