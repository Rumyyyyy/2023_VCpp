#include <windows.h>
#include <cmath>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DrawStar(HDC hdc, int centerX, int centerY, int size);
void DrawRectangle(HDC hdc, int centerX, int centerY, int size);
void DrawCircle(HDC hdc, int centerX, int centerY, int size);

bool isDrawing = false;
bool isButton1Clicked = false;
bool isButton2Clicked = false;
bool isButton3Clicked = false;
int startX, startY, endX, endY;
int shapeSize = 0; // 모양의 초기 크기
int shapeType = 0; // 0: 별, 1: 사각형, 2: 원

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
    HWND hwnd = CreateWindow(L"MyWindowClass", L"202218016 김아름", WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, NULL, GetModuleHandle(NULL), NULL);

    if (!hwnd) {
        return 2;
    }

    // 버튼 1을 생성합니다.
    CreateWindow(L"BUTTON", L"Star", WS_CHILD | WS_VISIBLE, 50, 50, 100, 30, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);

    // 버튼 2을 생성합니다.
    CreateWindow(L"BUTTON", L"Sqare", WS_CHILD | WS_VISIBLE, 200, 50, 100, 30, hwnd, (HMENU)2, GetModuleHandle(NULL), NULL);

    // 버튼 3을 생성합니다.
    CreateWindow(L"BUTTON", L"Circle", WS_CHILD | WS_VISIBLE, 350, 50, 100, 30, hwnd, (HMENU)3, GetModuleHandle(NULL), NULL);

    // clear 버튼 생성
    CreateWindow(L"BUTTON", L"Clear", WS_CHILD | WS_VISIBLE, 500, 50, 100, 30, hwnd, (HMENU)4, GetModuleHandle(NULL), NULL);

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
    HDC hdc;
    PAINTSTRUCT ps;
    switch (uMsg) {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        if (isDrawing) {
            if (isButton1Clicked) {
                shapeType = 0; // 별 그리기
            }
            else if (isButton2Clicked) {
                shapeType = 1; // 사각형 그리기
            }
            else if (isButton3Clicked) {
                shapeType = 2; // 원 그리기
            }

            if (shapeType == 0) {
                DrawStar(hdc, startX, startY, shapeSize);
            }
            else if (shapeType == 1) {
                DrawRectangle(hdc, startX, startY, shapeSize);
            }
            else if (shapeType == 2) {
                DrawCircle(hdc, startX, startY, shapeSize);
            }
        }
        EndPaint(hwnd, &ps);
        break;

    case WM_LBUTTONDOWN:
        if (isButton1Clicked || isButton2Clicked || isButton3Clicked) {
            isDrawing = true;
            startX = LOWORD(lParam);
            startY = HIWORD(lParam);
            endX = startX;
            endY = startY;
        }
        break;

    case WM_MOUSEMOVE:
        if (isDrawing) {
            endX = LOWORD(lParam);
            endY = HIWORD(lParam);
            shapeSize = static_cast<int>(sqrt(pow(endX - startX, 2) + pow(endY - startY, 2)));
            InvalidateRect(hwnd, NULL, FALSE); // 화면 갱신
        }
        break;

    case WM_LBUTTONUP:
        if (isDrawing) {
            isDrawing = false;
            InvalidateRect(hwnd, NULL, FALSE); // 화면 갱신
        }
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {
            // Button 1 clicked (Star)
            isButton1Clicked = true;
            isButton2Clicked = false;
            isButton3Clicked = false;
            isDrawing = false; // Disable drawing
        }
        else if (LOWORD(wParam) == 2) {
            // Button 2 clicked (Square)
            isButton1Clicked = false;
            isButton2Clicked = true;
            isButton3Clicked = false;
            isDrawing = false; // Disable drawing
        }
        else if (LOWORD(wParam) == 3) {
            // Button 3 clicked (Circle)
            isButton1Clicked = false;
            isButton2Clicked = false;
            isButton3Clicked = true;
            isDrawing = false; // Disable drawing
        }
        else if (LOWORD(wParam) == 4) {
            // Clear button clicked
            isButton1Clicked = false;
            isButton2Clicked = false;
            isButton3Clicked = false;
            isDrawing = false;

            // Clear the window by filling it with a white brush
            HDC hdc = GetDC(hwnd);
            HBRUSH hWhiteBrush = CreateSolidBrush(RGB(255, 255, 255));
            RECT rect;
            GetClientRect(hwnd, &rect);
            FillRect(hdc, &rect, hWhiteBrush);
            DeleteObject(hWhiteBrush);
            ReleaseDC(hwnd, hdc);
        }
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void DrawStar(HDC hdc, int centerX, int centerY, int size) {
    POINT points[10];

    HBRUSH hYellowBrush = CreateSolidBrush(RGB(255, 255, 0)); // 노란색 브러시 생성
    HPEN hYellowPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 0)); // 노란색 펜 생성

    SelectObject(hdc, hYellowBrush);
    SelectObject(hdc, hYellowPen);

    for (int i = 0; i < 10; i++) {
        double angle = 2 * 3.14159265359 / 10 * i;
        if (i % 2 == 0) {
            points[i].x = centerX + static_cast<int>(size * cos(angle));
            points[i].y = centerY + static_cast<int>(size * sin(angle));
        }
        else {
            points[i].x = centerX + static_cast<int>(size * 0.35 * cos(angle));
            points[i].y = centerY + static_cast<int>(size * 0.35 * sin(angle));
        }
    }

    Polygon(hdc, points, 10);

    DeleteObject(hYellowBrush); // 브러시 삭제
    DeleteObject(hYellowPen); // 펜 삭제
}

void DrawRectangle(HDC hdc, int centerX, int centerY, int size) {
    // 클라이언트 영역의 중앙을 계산
    int halfSize = size / 2;
    int left = centerX - halfSize;
    int top = centerY - halfSize;
    int right = centerX + halfSize;
    int bottom = centerY + halfSize;

    // 사각형 그리기
    RECT rect = { left, top, right, bottom };
    HBRUSH hYellowBrush = CreateSolidBrush(RGB(255, 0, 0)); // 빨간색 브러시 생성
    HPEN hYellowPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); // 빨간색 펜 생성

    SelectObject(hdc, hYellowBrush);
    SelectObject(hdc, hYellowPen);

    Rectangle(hdc, left, top, right, bottom);

    DeleteObject(hYellowBrush); // 브러시 삭제
    DeleteObject(hYellowPen); // 펜 삭제
}

void DrawCircle(HDC hdc, int centerX, int centerY, int size) {
    int halfSize = size / 2;
    int left = centerX - halfSize;
    int top = centerY - halfSize;
    int right = centerX + halfSize;
    int bottom = centerY + halfSize;

    HBRUSH hYellowBrush = CreateSolidBrush(RGB(0, 0, 255)); // 파란색 브러시 생성
    HPEN hYellowPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255)); // 파란색 펜 생성

    SelectObject(hdc, hYellowBrush);
    SelectObject(hdc, hYellowPen);

    Ellipse(hdc, left, top, right, bottom);

    DeleteObject(hYellowBrush); // 브러시 삭제
    DeleteObject(hYellowPen); // 펜 삭제
}