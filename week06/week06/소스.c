#include <windows.h>

// 전역 변수로 사각형의 위치와 크기를 저장
int blueRectX;
int blueRectY;
int blueRectWidth = 50; // 파란색 사각형의 너비
int blueRectHeight = 50; // 파란색 사각형의 높이

int textX; // 텍스트의 X 좌표
int textY; // 텍스트의 Y 좌표
BOOL showText = TRUE; // 텍스트를 표시할지 여부를 나타내는 변수

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // 화면을 흰색으로 클리어
        HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255)); // 흰색
        FillRect(hdc, &ps.rcPaint, whiteBrush);
        DeleteObject(whiteBrush);

        // 핑크 사각형 그리기
        RECT pinkRect = { 50, 50, 150, 150 };
        HBRUSH pinkBrush = CreateSolidBrush(RGB(255, 192, 203)); // 핑크색
        FillRect(hdc, &pinkRect, pinkBrush);
        DeleteObject(pinkBrush);

        // 파란색 사각형 그리기
        RECT blueRect = { blueRectX, blueRectY, blueRectX + blueRectWidth, blueRectY + blueRectHeight };
        HBRUSH blueBrush = CreateSolidBrush(RGB(0, 0, 255)); // 파란색
        FillRect(hdc, &blueRect, blueBrush);
        DeleteObject(blueBrush);

        if (showText) {
            // 텍스트 출력을 화면 중앙에 배치
            RECT textRect;
            GetClientRect(hwnd, &textRect); // 윈도우 클라이언트 영역 크기를 가져옴
            int textWidth = 100; // 텍스트 너비
            int textHeight = 20; // 텍스트 높이
            textX = (textRect.right - textRect.left - textWidth) / 2;
            textY = (textRect.bottom - textRect.top - textHeight) / 2;
            TextOut(hdc, textX, textY, L"HIT !", 5);
        }

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_KEYDOWN: {
        int stepSize = 10;
        switch (wParam) {
        case VK_LEFT:
            blueRectX -= stepSize;
            break;
        case VK_RIGHT:
            blueRectX += stepSize;
            break;
        case VK_UP:
            blueRectY -= stepSize;
            break;
        case VK_DOWN:
            blueRectY += stepSize;
            break;
        }

        // 충돌 검사
        RECT blueRect = { blueRectX, blueRectY, blueRectX + blueRectWidth, blueRectY + blueRectHeight };
        RECT pinkRect = { 50, 50, 150, 150 };
        if (IntersectRect(&blueRect, &blueRect, &pinkRect)) {
            showText = FALSE; // 충돌 시 텍스트 숨김
        }
        else {
            showText = TRUE; // 충돌이 없을 때 텍스트 표시
        }

        // 윈도우를 다시 그리도록 요청
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

int main() {
    // 윈도우 클래스 구조체 초기화
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL); // 현재 모듈의 핸들을 사용
    wc.lpszClassName = L"SampleWindowClass";

    // 윈도우 클래스 등록
    RegisterClass(&wc);

    // 윈도우 생성
    HWND hwnd = CreateWindow(L"SampleWindowClass", L"202218016 김아름", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 100, 100, 800, 600, NULL, NULL, GetModuleHandle(NULL), NULL);

    // 파란색 사각형의 초기 위치를 오른쪽 하단으로 설정
    blueRectX = 700 - blueRectWidth;
    blueRectY = 500 - blueRectHeight;

    // 윈도우 표시
    ShowWindow(hwnd, SW_SHOWDEFAULT); // SW_SHOWDEFAULT 사용

    // 메시지 루프
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
