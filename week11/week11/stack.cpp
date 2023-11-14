#include "stack.h" // y_stack, Node 클래스의 선언

y_stack::y_stack() : top(nullptr) {} // y_stack 생성자

// 스택에 남아있는 노드들을 제거, top이 nullptr이 될 때까지 반복하며 제거
y_stack::~y_stack() {
    while (top != nullptr) {
        Node* temp = top;
        top = top->next;
        delete temp;
    }
}

// 값을 가지는 새로운 노드 생성, 스택 맨 위에 추가
void y_stack::push(int num) {
    Node* newNode = new Node(num);
    newNode->next = top;
    top = newNode;
}

// 스택에서 맨 위의 값 제거 후 반환, 스택이 비어있으면 -1 반환함
int y_stack::pop() {
    if (top == nullptr) {
        // 스택이 비어있을 때
        return -1;
    }

    int poppedValue = top->data;
    Node* temp = top;
    top = top->next;
    delete temp;

    return poppedValue;
}