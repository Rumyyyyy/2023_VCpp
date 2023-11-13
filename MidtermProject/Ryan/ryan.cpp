#include <windows.h>
#include <cmath>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

RECT View = { (1000 - 800) / 2, 100, (970 - 850) / 2 + 800, 100 + 480 };

void DrawView(HDC hdc, RECT viewRect);
void DrawBox(HDC hdc, RECT boxRect);

void DrawBonobono(HDC hdc, int centerX, int centerY, int size);
void DrawRyan(HDC hdc, int centerX, int centerY, int size);
void DrawCube(HDC hdc, int centerX, int centerY, int size);
bool isDrawing = false;  // 드래그 중인지 여부를 나타내는 변수
int dragStartX, dragStartY;  // 드래그 시작 좌표
bool drawRyanPending = false; // 라이언을 그리기 위한 플래그

HBRUSH hbrBackground;
HWND hwndMain; // 메인 윈도우 핸들

int bonobonoX, bonobonoY;
int shapeSize = 400; // 초기 크기를 설정
bool spaceKeyPressed = false;
bool drawBonobono = false; // 보노보노 그리기 여부
bool drawRyan = false;
bool drawCube = false;
bool isLeftMouseButtonDown = false;

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
    hwndMain = CreateWindow(L"MyWindowClass", L"202218016 김아름", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 100, 100, 970, 700, NULL, NULL, GetModuleHandle(NULL), NULL);

    if (!hwndMain) {
        return 2;
    }

    // 버튼을 생성합니다.
    CreateWindow(L"BUTTON", L"Bonobono", WS_CHILD | WS_VISIBLE, 50, 30, 160, 64, hwndMain, (HMENU)1, GetModuleHandle(NULL), NULL);
    CreateWindow(L"BUTTON", L"Ryan", WS_CHILD | WS_VISIBLE, 220, 30, 160, 64, hwndMain, (HMENU)2, GetModuleHandle(NULL), NULL);
    CreateWindow(L"BUTTON", L"Cube", WS_CHILD | WS_VISIBLE, 390, 30, 160, 64, hwndMain, (HMENU)3, GetModuleHandle(NULL), NULL);
    CreateWindow(L"BUTTON", L"Button4", WS_CHILD | WS_VISIBLE, 560, 30, 160, 64, hwndMain, (HMENU)4, GetModuleHandle(NULL), NULL);
    CreateWindow(L"BUTTON", L"Button5", WS_CHILD | WS_VISIBLE, 730, 30, 160, 64, hwndMain, (HMENU)5, GetModuleHandle(NULL), NULL);

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

    case WM_SETCURSOR:
        POINT cursorPos;
        GetCursorPos(&cursorPos);
        ScreenToClient(hwnd, &cursorPos);

        if (PtInRect(&View, cursorPos)) {
            SetCursor(LoadCursor(NULL, IDC_CROSS));  // View 영역 내에서는 십자가 모양의 커서로 변경
            return TRUE;  // 이벤트 처리 완료
        }
        else {
            SetCursor(LoadCursor(NULL, IDC_ARROW));  // 그 외의 영역에서는 기본 마우스 커서로 변경
            return TRUE;  // 이벤트 처리 완료
        }

        // 나머지 경우에는 기본 처리를 수행합니다.
        return DefWindowProc(hwnd, uMsg, wParam, lParam);

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &clientRect);
        DrawView(hdc, View);  // DrawView 함수 호출
        if (drawBonobono) {
            int centerX = (clientRect.right + clientRect.left) / 2;
            int centerY = (clientRect.bottom + clientRect.top) / 2;
            DrawBonobono(hdc, centerX, centerY, shapeSize);
        }
        else if (drawRyan) {  // drawRyan이 true일 때 DrawRyan 함수 호출
            int centerX = (clientRect.right + clientRect.left) / 2;
            int centerY = (clientRect.bottom + clientRect.top) / 2;
            DrawRyan(hdc, centerX, centerY, shapeSize);
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
            drawRyan = false;
            drawCube = false;
            bonobonoX = bonobonoY = 0; // 보노보노 위치 초기화
            // Force a redraw to display Bonobono
            InvalidateRect(hwnd, NULL, FALSE);
            break;

        case 2: // 버튼 2을 클릭했을 때
            // 버튼 2의 포커스 해제
            SetFocus(NULL);
            SetFocus(hwnd);
            drawBonobono = false; // 기존 보노보노 지우기
            drawRyanPending = true; // drawRyanPending를 true로 설정
            drawCube = false;
            InvalidateRect(hwnd, NULL, FALSE);
            break;

        case 3: // 버튼 3을 클릭했을 때 (Cube)
            // 버튼 3의 포커스 해제
            SetFocus(NULL);
            SetFocus(hwnd);
            drawBonobono = false; // 기존 보노보노 지우기
            drawRyan = false; // 기존 Ryan 지우기
            drawCube = true; // drawCube를 true로 설정
            InvalidateRect(hwnd, NULL, FALSE);
            break;
            // 나머지 코드...
        }
        break;

    case WM_LBUTTONDOWN:
        if (drawRyanPending) {
            isDrawing = true;
            isLeftMouseButtonDown = true;  // 마우스 왼쪽 버튼이 눌려있는 경우 플래그를 설정합니다
            dragStartX = LOWORD(lParam);
            dragStartY = HIWORD(lParam);
        }
        break;

    case WM_MOUSEMOVE:
        if (isDrawing && drawRyanPending) {
            hdc = GetDC(hwnd);
            GetClientRect(hwnd, &clientRect);
            int currentX = LOWORD(lParam);
            int currentY = HIWORD(lParam);

            // 드래그 중인 동안 현재 위치까지 라이언을 그립니다.
            DrawView(hdc, View);
            DrawRyan(hdc, (dragStartX + currentX) / 2, (dragStartY + currentY) / 2, abs(currentX - dragStartX));
            ReleaseDC(hwnd, hdc);
        }
        break;

    case WM_LBUTTONUP:
        if (isDrawing && drawRyanPending) {
            isDrawing = false;
            isLeftMouseButtonDown = false;
            hdc = GetDC(hwnd);
            GetClientRect(hwnd, &clientRect);
            int currentX = LOWORD(lParam);
            int currentY = HIWORD(lParam);

            // 드래그가 끝난 위치에서 최종적으로 라이언을 그립니다.
            DrawView(hdc, View);
            DrawRyan(hdc, (dragStartX + currentX) / 2, (dragStartY + currentY) / 2, abs(currentX - dragStartX));
            ReleaseDC(hwnd, hdc);
        }
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void DrawView(HDC hdc, RECT viewRect) {
    HBRUSH hBackgroundBrush = CreateSolidBrush(RGB(255, 240, 200));
    SelectObject(hdc, hBackgroundBrush);
    Rectangle(hdc, viewRect.left, viewRect.top, viewRect.right, viewRect.bottom);
    DeleteObject(hBackgroundBrush);
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

void DrawRyan(HDC hdc, int centerX, int centerY, int size) {
    RECT View;
    GetClipBox(hdc, &View);

    int halfSize = size / 2;
    int left = centerX - halfSize;
    int top = centerY - halfSize;
    int right = centerX + halfSize;
    int bottom = centerY + halfSize;

    // 뷰 영역을 벗어나면 그리지 않음
    if (left > View.right || right < View.left || top > View.bottom || bottom < View.top) {
        return;
    }

    int innerSize = size / 20; // 내부 타원의 크기
    int innerHalfSize = innerSize / 2;
    int earSize = size / 3;
    int earHalfSize = earSize / 2;

    int LearX = left + (earSize / 2);
    int LearY = centerY - 50 - (earSize / 2);

    int RearX = right - 10 - (earSize / 2);
    int RearY = LearY;

    // 뷰 영역을 벗어나면 귀를 그리지 않음
    if (LearX - earHalfSize > View.left && LearY - earHalfSize > View.top &&
        RearX + earHalfSize < View.right && RearY + earHalfSize < View.bottom) {
        HBRUSH RearBrush = CreateSolidBrush(RGB(219, 154, 37));
        HPEN RearPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
        SelectObject(hdc, RearBrush);
        SelectObject(hdc, RearPen);
        Ellipse(hdc, RearX - earHalfSize, RearY - earHalfSize, RearX + earHalfSize, RearY + earHalfSize);
        DeleteObject(RearBrush);
        DeleteObject(RearPen);
    }

    // 뷰 영역을 벗어나면 귀를 그리지 않음
    if (LearX - earHalfSize > View.left && LearY - earHalfSize > View.top &&
        RearX + earHalfSize < View.right && RearY + earHalfSize < View.bottom) {
        HBRUSH LearBrush = CreateSolidBrush(RGB(219, 154, 37));
        HPEN LearPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
        SelectObject(hdc, LearBrush);
        SelectObject(hdc, LearPen);
        Ellipse(hdc, LearX - earHalfSize, LearY - earHalfSize, LearX + earHalfSize, LearY + earHalfSize);
        DeleteObject(LearBrush);
        DeleteObject(LearPen);
    }

    HBRUSH hYellowBrush = CreateSolidBrush(RGB(219, 154, 37));
    HPEN hYellowPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));

    SelectObject(hdc, hYellowBrush);
    SelectObject(hdc, hYellowPen);

    Ellipse(hdc, left, top, right, bottom);

    int lEyeCenterX = centerX - innerHalfSize * 10;
    int lEyeCenterY = centerY - innerSize; // 수평 중앙에 위치

    HBRUSH hLEyeBrush = CreateSolidBrush(RGB(0, 0, 0)); // 검정색 브러시 생성
    SelectObject(hdc, hLEyeBrush); // 내부 타원을 검정색 브러시로 설정
    Ellipse(hdc, lEyeCenterX - innerHalfSize, lEyeCenterY - innerHalfSize, lEyeCenterX + innerHalfSize, lEyeCenterY + innerHalfSize);

    // hREyeBrush 위치를 오른쪽으로 이동
    int rEyeCenterX = centerX + innerHalfSize * 10;
    int rEyeCenterY = centerY - innerSize; // 수평 중앙에 위치

    HBRUSH hREyeBrush = CreateSolidBrush(RGB(0, 0, 0)); // 검정색 브러시 생성
    SelectObject(hdc, hREyeBrush); // 내부 타원을 검정색 브러시로 설정
    Ellipse(hdc, rEyeCenterX - innerHalfSize, rEyeCenterY - innerHalfSize, rEyeCenterX + innerHalfSize, rEyeCenterY + innerHalfSize);

    int lineLength = size / 10;
    int distanceFromEyes = innerSize + 30;

    MoveToEx(hdc, lEyeCenterX - lineLength, lEyeCenterY - distanceFromEyes, NULL);
    LineTo(hdc, lEyeCenterX + lineLength, lEyeCenterY - distanceFromEyes);

    MoveToEx(hdc, rEyeCenterX - lineLength, rEyeCenterY - distanceFromEyes, NULL);
    LineTo(hdc, rEyeCenterX + lineLength, rEyeCenterY - distanceFromEyes);

    int whiteSize = size / 8; // 흰색 원 크기 설정
    int whiteHalfSize = whiteSize / 2;

    // 두 개의 흰색 원을 이어붙이기
    int whiteX1 = centerX - whiteHalfSize; // 첫 번째 흰색 원
    int whiteY1 = centerY + innerSize * 3; // 아래쪽으로 이동
    int whiteX2 = centerX + whiteHalfSize; // 두 번째 흰색 원
    int whiteY2 = centerY + innerSize * 3; // 아래쪽으로 이동

    HBRUSH hWhiteBrush = CreateSolidBrush(RGB(255, 255, 255)); // 흰색 브러시 생성
    HPEN hYellowPen2 = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    SelectObject(hdc, hWhiteBrush); // 흰색 브러시로 설정
    SelectObject(hdc, hYellowPen2);
    Ellipse(hdc, whiteX1 - whiteHalfSize, whiteY1 - whiteHalfSize, whiteX1 + whiteHalfSize, whiteY1 + whiteHalfSize);
    Ellipse(hdc, whiteX2 - whiteHalfSize, whiteY2 - whiteHalfSize, whiteX2 + whiteHalfSize, whiteY2 + whiteHalfSize);

    DeleteObject(hLEyeBrush); // 사용한 브러시 삭제
    DeleteObject(hREyeBrush); // 사용한 브러시 삭제
    DeleteObject(hWhiteBrush);
    DeleteObject(hYellowBrush);
    DeleteObject(hYellowPen);
    DeleteObject(hYellowPen2);
}