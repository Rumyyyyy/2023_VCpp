#include <windows.h>
#include <cmath>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DrawBonobono(HDC hdc, int centerX, int centerY, int size);

HBRUSH hbrBackground;
HWND hwndMain; // 메인 윈도우 핸들

int bonobonoX, bonobonoY;
int shapeSize = 500; // 초기 크기를 설정
bool spaceKeyPressed = false;
bool drawBonobono = false; // 보노보노 그리기 여부

int main() {
    // 윈도우 클래스를 등록합니다.
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"MyWindowClass";

    // 배경색을 설정합니다.
    hbrBackground = CreateSolidBrush(RGB(255, 240, 200));
    wc.hbrBackground = hbrBackground;

    if (!RegisterClass(&wc)) {
        return 1;
    }

    // 메인 윈도우를 생성합니다.
    hwndMain = CreateWindow(L"MyWindowClass", L"202218016 김아름", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 100, 100, 1000, 800, NULL, NULL, GetModuleHandle(NULL), NULL);

    if (!hwndMain) {
        return 2;
    }

    // 버튼을 생성합니다.
    CreateWindow(L"BUTTON", L"Bonobono", WS_CHILD | WS_VISIBLE, 50, 50, 160, 64, hwndMain, (HMENU)1, GetModuleHandle(NULL), NULL);
    CreateWindow(L"BUTTON", L"Button2", WS_CHILD | WS_VISIBLE, 220, 50, 160, 64, hwndMain, (HMENU)2, GetModuleHandle(NULL), NULL);
    CreateWindow(L"BUTTON", L"Button3", WS_CHILD | WS_VISIBLE, 390, 50, 160, 64, hwndMain, (HMENU)3, GetModuleHandle(NULL), NULL);
    CreateWindow(L"BUTTON", L"Button4", WS_CHILD | WS_VISIBLE, 560, 50, 160, 64, hwndMain, (HMENU)4, GetModuleHandle(NULL), NULL);
    CreateWindow(L"BUTTON", L"Button5", WS_CHILD | WS_VISIBLE, 730, 50, 160, 64, hwndMain, (HMENU)5, GetModuleHandle(NULL), NULL);

    // 윈도우를 표시
    ShowWindow(hwndMain, SW_SHOW);

    // 메인 메시지 루프
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    PAINTSTRUCT ps;
    RECT clientRect;

    switch (uMsg) {
    case WM_CREATE:
        return 0;
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        if (drawBonobono) {
            GetClientRect(hwnd, &clientRect);
            int centerX = (clientRect.right + clientRect.left) / 2;
            int centerY = (clientRect.bottom + clientRect.top) / 2;
            DrawBonobono(hdc, centerX, centerY, shapeSize);
        }
        EndPaint(hwnd, &ps);
        break;

    case WM_KEYDOWN:
        if (wParam == VK_SPACE) {
            if (drawBonobono) {
                spaceKeyPressed = true; // 스페이스바 눌림
                // Force a redraw to update the eyes to lines or circles
                InvalidateRect(hwnd, NULL, FALSE);
            }
        }
        break;

    case WM_KEYUP:
        if (wParam == VK_SPACE) {
            if (drawBonobono) {
                spaceKeyPressed = false; // 스페이스바 뗌
                // Force a redraw to update the eyes back to circles
                InvalidateRect(hwnd, NULL, FALSE);
            }
        }
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 1: // 버튼 1을 클릭했을 때
            // 버튼 1의 포커스를 해제합니다.
            SetFocus(NULL);

            // 스페이스바 이벤트를 시뮬레이트 (WM_KEYDOWN)
            SendMessage(hwnd, WM_KEYDOWN, VK_SPACE, 0);

            // 보노보노 윈도우에 포커스를 설정합니다.
            SetFocus(hwnd);

            spaceKeyPressed = false; // 스페이스 바 효과 비활성화
            drawBonobono = true; // drawBonobono를 true로 설정
            bonobonoX = bonobonoY = 0; // 보노보노 위치 초기화
            // Force a redraw to display Bonobono
            InvalidateRect(hwnd, NULL, FALSE);
            break;
        }
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void DrawBonobono(HDC hdc, int centerX, int centerY, int size) {
    POINT points[10];

    HBRUSH hBonoBrush = CreateSolidBrush(RGB(127, 200, 255)); // 보노보노 피부색
    HBRUSH hBonoEyes = CreateSolidBrush(RGB(0, 0, 0));
    HBRUSH hInner = CreateSolidBrush(RGB(255, 255, 255));
    HBRUSH hMouse = CreateSolidBrush(RGB(255, 150, 255));
    HPEN hBonoPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0)); // 테두리

    SelectObject(hdc, hBonoBrush);
    SelectObject(hdc, hBonoPen);

    int halfSize = size / 2;
    int left = centerX - halfSize;
    int top = centerY - halfSize;
    int right = centerX + halfSize;
    int bottom = centerY + halfSize;

    Ellipse(hdc, left, top, right, bottom);

    // 입 그리기
    int innerEllipseWidth = size / 7; // 내부 타원 너비
    int innerEllipseHeight = size / 5; // 내부 타원 높이

    // 입 위치
    int innerEllipseLeft = centerX - innerEllipseWidth / 2;
    int innerEllipseTop = centerY + size / 10; // 이 부분을 수정하여 입 위치 변경
    int innerEllipseRight = innerEllipseLeft + innerEllipseWidth;
    int innerEllipseBottom = innerEllipseTop + innerEllipseHeight;

    SelectObject(hdc, hMouse);
    Ellipse(hdc, innerEllipseLeft, innerEllipseTop, innerEllipseRight, innerEllipseBottom);

    // 코 뒤에 원 그리기
    SelectObject(hdc, hInner);
    int circleSize = size / 6; // 크기 조정

    int circle1Left = centerX - circleSize; // 왼쪽 좌표 설정
    int circle1Top = centerY + size / 100; // 위쪽 좌표 설정
    int circle1Right = circle1Left + circleSize; // 오른쪽 좌표 설정
    int circle1Bottom = circle1Top + circleSize; // 아래쪽 좌표 설정

    int circle2Left = centerX + circleSize / 100; // 왼쪽 좌표 설정
    int circle2Top = centerY + size / 100; // 위쪽 좌표 설정
    int circle2Right = circle2Left + circleSize; // 오른쪽 좌표 설정
    int circle2Bottom = circle2Top + circleSize; // 아래쪽 좌표 설정

    Ellipse(hdc, circle1Left, circle1Top, circle1Right, circle1Bottom);
    Ellipse(hdc, circle2Left, circle2Top, circle2Right, circle2Bottom);

    // 코 그리기
    int noseSize = size / 7; // 크기 조정
    int noseLeft = centerX - noseSize / 2; // 왼쪽 좌표 설정
    int noseTop = centerY - noseSize / 3; // 위쪽 좌표 설정
    int noseRight = noseLeft + noseSize; // 오른쪽 좌표 설정
    int noseBottom = noseTop + noseSize; // 아래쪽 좌표 설정

    SelectObject(hdc, hBonoEyes); // 코 색 == 눈 색과 같음
    Ellipse(hdc, noseLeft, noseTop, noseRight, noseBottom);

    // 수염 1 (오른쪽 위)
    int startX1 = centerX + size / 10; // 시작 X 좌표
    int startY1 = centerY + size / 15; // 시작 Y 좌표
    int endX1 = centerX + size / 3; // 끝 X 좌표
    int endY1 = centerY + size / 25; // 끝 Y 좌표

    // 수염 2 (오른쪽 아래)
    int startX2 = centerX + size / 10; // 시작 X 좌표
    int startY2 = centerY + size / 10; // 시작 Y 좌표
    int endX2 = centerX + size / 3; // 끝 X 좌표
    int endY2 = centerY + size / 5; // 끝 Y 좌표

    // 수염 3 (왼쪽 위)
    int startX3 = centerX - size / 10; // 시작 X 좌표
    int startY3 = centerY + size / 15; // 시작 Y 좌표
    int endX3 = centerX - size / 3; // 끝 X 좌표
    int endY3 = centerY + size / 25; // 끝 Y 좌표

    // 수염 4 (왼쪽 아래)
    int startX4 = centerX - size / 10; // 시작 X 좌표
    int startY4 = centerY + size / 10; // 시작 Y 좌표
    int endX4 = centerX - size / 3; // 끝 X 좌표
    int endY4 = centerY + size / 5; // 끝 Y 좌표

    // 수염 그리기
    MoveToEx(hdc, startX1, startY1, NULL);
    LineTo(hdc, endX1, endY1);

    MoveToEx(hdc, startX2, startY2, NULL);
    LineTo(hdc, endX2, endY2);

    MoveToEx(hdc, startX3, startY3, NULL);
    LineTo(hdc, endX3, endY3);

    MoveToEx(hdc, startX4, startY4, NULL);
    LineTo(hdc, endX4, endY4);

    // 눈 그리기
    int ellipseWidth = size / 30; // 타원 너비
    int ellipseHeight = size / 15; // 타원 높이

    // 왼쪽 눈
    int ellipse1Left = centerX - size / 2.5; // 왼쪽 좌표 설정
    int ellipse1Top = centerY - size / 8; // 위쪽 좌표 설정
    int ellipse1Right = ellipse1Left + ellipseWidth; // 오른쪽 좌표 설정
    int ellipse1Bottom = ellipse1Top + ellipseHeight; // 아래쪽 좌표 설정

    // 오른쪽 눈
    int ellipse2Left = centerX + size / 3; // 왼쪽 좌표 설정
    int ellipse2Top = centerY - size / 8; // 위쪽 좌표 설정
    int ellipse2Right = ellipse2Left + ellipseWidth; // 오른쪽 좌표 설정
    int ellipse2Bottom = ellipse2Top + ellipseHeight; // 아래쪽 좌표 설정

    // 눈의 내부 하얀색 타원
    int innerEllipse1Left = ellipse1Left + ellipseWidth / 2.5; // 내부 타원의 왼쪽 좌표 설정
    int innerEllipse1Top = ellipse1Top + ellipseHeight / 4; // 내부 타원의 위쪽 좌표 설정
    int innerEllipse1Right = ellipse1Right - ellipseWidth / 3; // 내부 타원의 오른쪽 좌표 설정
    int innerEllipse1Bottom = ellipse1Bottom - ellipseHeight / 2; // 내부 타원의 아래쪽 좌표 설정

    int innerEllipse2Left = ellipse2Left + ellipseWidth / 2.5; // 내부 타원의 왼쪽 좌표 설정
    int innerEllipse2Top = ellipse2Top + ellipseHeight / 4; // 내부 타원의 위쪽 좌표 설정
    int innerEllipse2Right = ellipse2Right - ellipseWidth / 3; // 내부 타원의 오른쪽 좌표 설정
    int innerEllipse2Bottom = ellipse2Bottom - ellipseHeight / 2; // 내부 타원의 아래쪽 좌표 설정

    if (spaceKeyPressed) {
        // 왼쪽 눈 모양
        int leftEyeCenterX = (ellipse1Left + ellipse1Right) / 2;
        int leftEyeCenterY = (ellipse1Top + ellipse1Bottom) / 2;
        int leftEyeLineLength = (ellipse1Right - ellipse1Left) * 2;

        MoveToEx(hdc, leftEyeCenterX, ellipse1Top, NULL);
        LineTo(hdc, leftEyeCenterX + leftEyeLineLength, leftEyeCenterY);
        MoveToEx(hdc, leftEyeCenterX + leftEyeLineLength, leftEyeCenterY, NULL);
        LineTo(hdc, leftEyeCenterX, ellipse1Bottom);

        // 오른쪽 눈 모양
        int rightEyeCenterX = (ellipse2Left + ellipse2Right) / 2;
        int rightEyeCenterY = (ellipse2Top + ellipse2Bottom) / 2;
        int rightEyeLineLength = (ellipse2Right - ellipse2Left) * 2;

        MoveToEx(hdc, rightEyeCenterX, ellipse2Top, NULL);
        LineTo(hdc, rightEyeCenterX - rightEyeLineLength, rightEyeCenterY);
        MoveToEx(hdc, rightEyeCenterX - rightEyeLineLength, rightEyeCenterY, NULL);
        LineTo(hdc, rightEyeCenterX, ellipse2Bottom);
    }
    else {
        // 스페이스 바가 떼어진 경우 눈을 원 모양으로 그립니다
        SelectObject(hdc, hBonoEyes);
        Ellipse(hdc, ellipse1Left, ellipse1Top, ellipse1Right, ellipse1Bottom);
        Ellipse(hdc, ellipse2Left, ellipse2Top, ellipse2Right, ellipse2Bottom);

        // 눈의 내부 하얀색 타원도 그립니다
        SelectObject(hdc, hInner);
        Ellipse(hdc, innerEllipse1Left, innerEllipse1Top, innerEllipse1Right, innerEllipse1Bottom);
        Ellipse(hdc, innerEllipse2Left, innerEllipse2Top, innerEllipse2Right, innerEllipse2Bottom);
    }

    DeleteObject(hBonoEyes); // 눈 색
    DeleteObject(hBonoBrush); // 브러시 삭제
    DeleteObject(hBonoPen); // 펜 삭제 
}