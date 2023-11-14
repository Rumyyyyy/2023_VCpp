#pragma once

class Node {
public:
    int data;
    Node* next; // node* Ÿ���� next

    Node(int value) : data(value), next(nullptr) {} // data, next �ʱ�ȭ
    // Node : �����Ϳ� ���� ��带 ����Ű�� ������
};

class y_stack { // ����
public:
    y_stack();   // ������
    ~y_stack();  // �Ҹ���

    void push(int num);
    int pop();

private:
    Node* top;  // ������ �� ���� ����Ű�� ������
};