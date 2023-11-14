#include "stack.h" // y_stack, Node Ŭ������ ����

y_stack::y_stack() : top(nullptr) {} // y_stack ������

// ���ÿ� �����ִ� ������ ����, top�� nullptr�� �� ������ �ݺ��ϸ� ����
y_stack::~y_stack() {
    while (top != nullptr) {
        Node* temp = top;
        top = top->next;
        delete temp;
    }
}

// ���� ������ ���ο� ��� ����, ���� �� ���� �߰�
void y_stack::push(int num) {
    Node* newNode = new Node(num);
    newNode->next = top;
    top = newNode;
}

// ���ÿ��� �� ���� �� ���� �� ��ȯ, ������ ��������� -1 ��ȯ��
int y_stack::pop() {
    if (top == nullptr) {
        // ������ ������� ��
        return -1;
    }

    int poppedValue = top->data;
    Node* temp = top;
    top = top->next;
    delete temp;

    return poppedValue;
}