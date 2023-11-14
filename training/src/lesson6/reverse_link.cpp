#include <iostream>
#include <vector>

struct Node {
    char data;
    Node *next;
};

Node *create_list() {
    std::string line;
    std::getline(std::cin, line);

    Node *head = nullptr;
    for (int i = line.size() - 1; i >= 0; i--) {
        Node *node = new Node();
        node->data = line[i];
        node->next = head;
        head = node;
    }
    return head;
}

void display(Node *node) {
    while (node != nullptr) {
        std::cout << node->data;
        node = node->next;
    }
    std::cout << '\n';
}

Node *reverse(Node *node) {
    Node *tail = nullptr;
    while (node != nullptr) {
        Node *next = node->next;
        node->next = tail;
        tail = node;
        node = next;
    } 
    return tail;
}

int main() {
    Node *head = create_list();
    display(head);
    head = reverse(head);
    display(head);
    return 0;
}
