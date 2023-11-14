#include <cstdio>

struct Node {
    int value;
    Node *next;
};

Node *create_list(int start, int end) {
    Node *head = nullptr;
    for (int i = end; i >= start; i--) {
        Node *node = new Node();
        node->value = i;
        node->next = head;
        head = node;
    }
    return head;
}

Node *concat_list(Node *head_a, Node *head_b) {
    if (head_a == nullptr)
        return head_b;

    Node *node = head_a;

    while (node->next != nullptr)
        node = node->next;
    node->next = head_b;
    return head_a;
}

void display(Node *node) {
    while (node != nullptr) {
        printf("%d ", node->value);
        node = node->next;
    }
    printf("\n");
}

int main() {
    Node *head_a = create_list(1, 5);
    display(head_a);

    Node *head_b = create_list(6, 10);
    display(head_b);

    Node *head_c = concat_list(head_a, head_b);
    display(head_c);

    return 0;
}