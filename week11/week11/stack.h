#pragma once

class Node {
public:
    int data;
    Node* next; // node* 타입의 next

    Node(int value) : data(value), next(nullptr) {} // data, next 초기화
    // Node : 데이터와 다음 노드를 가리키는 포인터
};

class y_stack { // 스택
public:
    y_stack();   // 생성자
    ~y_stack();  // 소멸자

    void push(int num);
    int pop();

private:
    Node* top;  // 스택의 맨 위를 가리키는 포인터
};