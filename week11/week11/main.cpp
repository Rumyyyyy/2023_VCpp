#include "stack.h"
#include <iostream>

int main() {
    y_stack myStack;

    int inputValue;
    int count;

    // 사용자로부터 입력받은 개수만큼 스택에 값을 넣기
    std::cout << "스택에 몇 개의 수를 넣을지 적으세요: ";
    std::cin >> count;

    for (int i = 0; i < count; i++) {
        std::cout << i + 1 << "번째로 스택에 넣을 값을 입력하세요: ";
        std::cin >> inputValue;
        myStack.push(inputValue);
    }

    // 스택이 빌 때까지 pop하여 출력
    while (true) {
        int poppedValue = myStack.pop();

        if (poppedValue != -1) {
            std::cout << "POP: " << poppedValue << std::endl;
        }
        else {
            std::cout << "\n스택이 비어있습니다." << std::endl;
            if (myStack.pop() == -1) {
                break; // 스택이 빈 경우 루프에서 나가기
            }
        }
    }

    return 0;
}