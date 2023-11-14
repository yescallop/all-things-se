// 洛谷 P1996 约瑟夫问题

#include <cstdio>
#include <optional>

/// A circular singly-linked list.
template <class T> class CircularList {

    /// A node in a circular singly-linked list.
    struct Node {
        T value;
        Node *next = nullptr;

        Node() {}
        Node(const T &value, Node *next) : value(value), next(next) {}
    };

    Node *dummy;

  public:
    /// Constructs an empty list.
    CircularList() {
        dummy = new Node();
        dummy->next = dummy;
    }

    /// Destructs the list.
    ~CircularList() {
        Node *cur = dummy->next;
        // Pointer arithmetic is undefined for invalid
        // pointers so we need to take care. Specifically,
        // we must not compare pointers that have been deleted.
        while (cur != dummy) {
            Node *next = cur->next;
            delete cur;
            cur = next;
        }
        delete dummy;
    }

    /// An iterator to an element of a circular singly-linked list.
    ///
    /// When incremented, the iterator goes round and round the list.
    class Iterator {
        Node *ptr;
        Node *dummy;

      public:
        Iterator(Node *ptr, Node *dummy) : ptr(ptr), dummy(dummy) {}

        /// Points the iterator to the next element.
        Iterator &operator++() {
            ptr = ptr->next;
            if (ptr == dummy)
                // Skip the dummy node.
                ptr = ptr->next;
            return *this;
        }

        /// Removes and returns the next element.
        /// Returns `std::nullopt` if there is no element left.
        std::optional<T> remove_next() {
            Node *next = ptr->next;
            if (next == dummy) {
                if (ptr == dummy)
                    // No element left.
                    return std::nullopt;
                // Point the iterator to `next` if `next` is dummy.
                ptr = next;
                next = ptr->next;
            }

            // Removes the next node.
            T value = next->value;
            next = next->next;
            delete ptr->next;
            ptr->next = next;

            return value;
        }
    };

    /// Returns an iterator to the element before beginning.
    Iterator before_begin() const { return Iterator(dummy, dummy); }

    /// Inserts an element to the beginning.
    void push_front(const T &value) {
        dummy->next = new Node(value, dummy->next);
    }
};

int main() {
    int n, m;
    if (scanf("%d%d", &n, &m) < 2) {
        fprintf(stderr, "error: invalid input\n");
        return 1;
    }
    if (n < 1 || m < 1) {
        fprintf(stderr, "error: input out of range\n");
        return 1;
    }

    CircularList<int> list = {};
    // Initialize the list.
    for (int i = n; i > 0; i--) {
        list.push_front(i);
    }

    auto it = list.before_begin();
    while (true) {
        // Count off until the (m-1)th person.
        for (int i = 1; i < m; i++) {
            ++it;
        }
        // Remove the next person from the list.
        // If there is no person left, break the loop.
        auto person = it.remove_next();
        if (!person)
            break;
        // Print the number assigned to the removed person.
        printf("%d ", *person);
    }
    printf("\n");
    return 0;
}
