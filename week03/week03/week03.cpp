#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <conio.h>

int main() {
    std::vector<char> inputBuffer;

    while (true) {
        if (_kbhit()) {
            char ch = _getch();
            inputBuffer.push_back(ch);

            if (ch == 27) { // 27은 'esc' 키의 ASCII 코드입니다.
                std::cout << "프로그램 종료!" << std::endl;
                break; // 루프 종료
            }
        }

        if (inputBuffer.size() >= 3) {
            // 입력이 쌓였고, 패턴을 확인
            if (inputBuffer[0] == '2' && inputBuffer[2] == '6' && inputBuffer[3] == 'a') {
                std::cout << "아도겐! =o" << std::endl;
                inputBuffer.clear(); // 입력 버퍼 비우기
            }
            else {
                std::cout << "잘못된 입력" << std::endl;
                inputBuffer.clear(); // 잘못된 입력을 처리하고 버퍼 비우기
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(300)); // 0.3초(300ms) 대기
    }

    return 0;
}