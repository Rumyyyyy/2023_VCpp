#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DrawRectangle(HDC hdc);
void EraseRectangle(HDC hdc);

bool isRectangleVisible = false; // 사각형이 현재 화면에 표시되는지 여부

int main() {
    // 윈도우 클래스를 등록합니다.
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"MyWindowClass";

    if (!RegisterClass(&wc)) {
        return 1;
    }

    // 윈도우를 생성합니다.
    HWND hwnd = CreateWindow(L"MyWindowClass", L"My Window", WS_OVERLAPPEDWINDOW, 100, 100, 400, 300, NULL, NULL, GetModuleHandle(NULL), NULL);

    if (!hwnd) {
        return 2;
    }

    // 버튼 1을 생성합니다.
    CreateWindow(L"BUTTON", L"Button 1", WS_CHILD | WS_VISIBLE, 50, 50, 100, 30, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);

    // 버튼 2를 생성합니다.
    CreateWindow(L"BUTTON", L"Button 2", WS_CHILD | WS_VISIBLE, 200, 50, 100, 30, hwnd, (HMENU)2, GetModuleHandle(NULL), NULL);

    // 윈도우를 화면에 표시합니다.
    ShowWindow(hwnd, SW_SHOWNORMAL);

    // 메인 메시지 루프
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {
            // 버튼 1을 클릭했을 때 사각형 그리기
            HDC hdc = GetDC(hwnd);
            if (!isRectangleVisible) {
                DrawRectangle(hdc);
                isRectangleVisible = true;
            }
            ReleaseDC(hwnd, hdc);
        }
        else if (LOWORD(wParam) == 2) {
            // 버튼 2를 클릭했을 때 사각형 지우기
            if (isRectangleVisible) {
                HDC hdc = GetDC(hwnd);
                EraseRectangle(hdc);
                ReleaseDC(hwnd, hdc);
                isRectangleVisible = false;
            }
        }
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void DrawRectangle(HDC hdc) {
    // 빨간색 브러시 생성
    HBRUSH hRedBrush = CreateSolidBrush(RGB(255, 0, 0)); // 빨간색

    // 사각형 그리기
    RECT rect = { 150, 100, 250, 200 };
    FillRect(hdc, &rect, hRedBrush); // 빨간색 브러시 사용

    // 브러시 삭제
    DeleteObject(hRedBrush);
}

void EraseRectangle(HDC hdc) {
    // 사각형 영역을 지우기 (배경색으로 지우기)
    RECT rect = { 150, 100, 250, 200 };
    HBRUSH hWhiteBrush = (HBRUSH)(COLOR_WINDOW + 1);
    FillRect(hdc, &rect, hWhiteBrush);
}
