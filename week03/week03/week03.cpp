#include <iostream>
#include <conio.h> // _kbhit와 _getch 함수를 사용하기 위한 헤더 파일

int main() {
    std::cout << "대전액션 커맨드" << std::endl;
    std::string inputBuffer;

    while (true) { // 무한 루프, 프로그램이 종료될 때까지 계속 반복
        if (_kbhit()) { // _kbhit 함수는 키 입력이 있는지 확인하는 함수
            char ch = _getch(); // _getch 함수는 키 입력을 받는 함수
            inputBuffer += ch; // 입력된 키를 버퍼에 추가

            if (ch == 27) { // esc 키의 아스키 코드 => 27
                return 0; // 프로그램 전체 종료
            }
        }

        if (inputBuffer.find("26a") != std::string::npos) { // 26a가 포함되어 있는지 확인
            std::cout << "아도겐! =o" << std::endl;
            break; // 프로그램 종료
        }
        else if (inputBuffer.find("24a") != std::string::npos) { // 24a가 포함되어 있는지 확인
            std::cout << "뿅! =o" << std::endl;
            break; // 프로그램 종료
        }
        else if (inputBuffer.find("28a") != std::string::npos) { // 28a가 포함되어 있는지 확인
            std::cout << "짠! =o" << std::endl;
            break; // 프로그램 종료
        }
    }

    return 0; // 프로그램 종료
}