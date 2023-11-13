#include <windows.h>
#include <cmath>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

RECT View = { (1000 - 800) / 2, 100, (970 - 850) / 2 + 800, 100 + 480 };

void DrawView(HDC hdc, RECT viewRect);
void DrawBox(HDC hdc, RECT boxRect);

void DrawBonobono(HDC hdc, int centerX, int centerY, int size);
void DrawRyan(HDC hdc, int centerX, int centerY, int size);
void DrawCube(HDC hdc, int centerX, int centerY, int size);
bool isDrawing = false;  // �巡�� ������ ���θ� ��Ÿ���� ����
int dragStartX, dragStartY;  // �巡�� ���� ��ǥ
bool drawRyanPending = false; // ���̾��� �׸��� ���� �÷���

HBRUSH hbrBackground;
HWND hwndMain; // ���� ������ �ڵ�

int bonobonoX, bonobonoY;
int shapeSize = 400; // �ʱ� ũ�⸦ ����
bool spaceKeyPressed = false;
bool drawBonobono = false; // ���뺸�� �׸��� ����
bool drawRyan = false;
bool drawCube = false;
bool isLeftMouseButtonDown = false;

int main() {
    // ������ Ŭ������ ����մϴ�.
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"MyWindowClass";

    // ������ �����մϴ�.
    hbrBackground = CreateSolidBrush(RGB(255, 240, 200));
    wc.hbrBackground = hbrBackground;

    if (!RegisterClass(&wc)) {
        return 1;
    }

    // ���� �����츦 �����մϴ�.
    hwndMain = CreateWindow(L"MyWindowClass", L"202218016 ��Ƹ�", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 100, 100, 970, 700, NULL, NULL, GetModuleHandle(NULL), NULL);

    if (!hwndMain) {
        return 2;
    }

    // ��ư�� �����մϴ�.
    CreateWindow(L"BUTTON", L"Bonobono", WS_CHILD | WS_VISIBLE, 50, 30, 160, 64, hwndMain, (HMENU)1, GetModuleHandle(NULL), NULL);
    CreateWindow(L"BUTTON", L"Ryan", WS_CHILD | WS_VISIBLE, 220, 30, 160, 64, hwndMain, (HMENU)2, GetModuleHandle(NULL), NULL);
    CreateWindow(L"BUTTON", L"Cube", WS_CHILD | WS_VISIBLE, 390, 30, 160, 64, hwndMain, (HMENU)3, GetModuleHandle(NULL), NULL);
    CreateWindow(L"BUTTON", L"Button4", WS_CHILD | WS_VISIBLE, 560, 30, 160, 64, hwndMain, (HMENU)4, GetModuleHandle(NULL), NULL);
    CreateWindow(L"BUTTON", L"Button5", WS_CHILD | WS_VISIBLE, 730, 30, 160, 64, hwndMain, (HMENU)5, GetModuleHandle(NULL), NULL);

    // �����츦 ǥ��
    ShowWindow(hwndMain, SW_SHOW);

    // ���� �޽��� ����
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
            SetCursor(LoadCursor(NULL, IDC_CROSS));  // View ���� �������� ���ڰ� ����� Ŀ���� ����
            return TRUE;  // �̺�Ʈ ó�� �Ϸ�
        }
        else {
            SetCursor(LoadCursor(NULL, IDC_ARROW));  // �� ���� ���������� �⺻ ���콺 Ŀ���� ����
            return TRUE;  // �̺�Ʈ ó�� �Ϸ�
        }

        // ������ ��쿡�� �⺻ ó���� �����մϴ�.
        return DefWindowProc(hwnd, uMsg, wParam, lParam);

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &clientRect);
        DrawView(hdc, View);  // DrawView �Լ� ȣ��
        if (drawBonobono) {
            int centerX = (clientRect.right + clientRect.left) / 2;
            int centerY = (clientRect.bottom + clientRect.top) / 2;
            DrawBonobono(hdc, centerX, centerY, shapeSize);
        }
        else if (drawRyan) {  // drawRyan�� true�� �� DrawRyan �Լ� ȣ��
            int centerX = (clientRect.right + clientRect.left) / 2;
            int centerY = (clientRect.bottom + clientRect.top) / 2;
            DrawRyan(hdc, centerX, centerY, shapeSize);
        }
        EndPaint(hwnd, &ps);
        break;

    case WM_KEYDOWN:
        if (wParam == VK_SPACE) {
            if (drawBonobono) {
                spaceKeyPressed = true; // �����̽��� ����
                // Force a redraw to update the eyes to lines or circles
                InvalidateRect(hwnd, NULL, FALSE);
            }
        }
        break;

    case WM_KEYUP:
        if (wParam == VK_SPACE) {
            if (drawBonobono) {
                spaceKeyPressed = false; // �����̽��� ��
                // Force a redraw to update the eyes back to circles
                InvalidateRect(hwnd, NULL, FALSE);
            }
        }
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 1: // ��ư 1�� Ŭ������ ��
            // ��ư 1�� ��Ŀ���� �����մϴ�.
            SetFocus(NULL);
            // �����̽��� �̺�Ʈ�� �ùķ���Ʈ (WM_KEYDOWN)
            SendMessage(hwnd, WM_KEYDOWN, VK_SPACE, 0);
            // ���뺸�� �����쿡 ��Ŀ���� �����մϴ�.
            SetFocus(hwnd);
            spaceKeyPressed = false; // �����̽� �� ȿ�� ��Ȱ��ȭ
            drawBonobono = true; // drawBonobono�� true�� ����
            drawRyan = false;
            drawCube = false;
            bonobonoX = bonobonoY = 0; // ���뺸�� ��ġ �ʱ�ȭ
            // Force a redraw to display Bonobono
            InvalidateRect(hwnd, NULL, FALSE);
            break;

        case 2: // ��ư 2�� Ŭ������ ��
            // ��ư 2�� ��Ŀ�� ����
            SetFocus(NULL);
            SetFocus(hwnd);
            drawBonobono = false; // ���� ���뺸�� �����
            drawRyanPending = true; // drawRyanPending�� true�� ����
            drawCube = false;
            InvalidateRect(hwnd, NULL, FALSE);
            break;

        case 3: // ��ư 3�� Ŭ������ �� (Cube)
            // ��ư 3�� ��Ŀ�� ����
            SetFocus(NULL);
            SetFocus(hwnd);
            drawBonobono = false; // ���� ���뺸�� �����
            drawRyan = false; // ���� Ryan �����
            drawCube = true; // drawCube�� true�� ����
            InvalidateRect(hwnd, NULL, FALSE);
            break;
            // ������ �ڵ�...
        }
        break;

    case WM_LBUTTONDOWN:
        if (drawRyanPending) {
            isDrawing = true;
            isLeftMouseButtonDown = true;  // ���콺 ���� ��ư�� �����ִ� ��� �÷��׸� �����մϴ�
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

            // �巡�� ���� ���� ���� ��ġ���� ���̾��� �׸��ϴ�.
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

            // �巡�װ� ���� ��ġ���� ���������� ���̾��� �׸��ϴ�.
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

    HBRUSH hBonoBrush = CreateSolidBrush(RGB(127, 200, 255)); // ���뺸�� �Ǻλ�
    HBRUSH hBonoEyes = CreateSolidBrush(RGB(0, 0, 0));
    HBRUSH hInner = CreateSolidBrush(RGB(255, 255, 255));
    HBRUSH hMouse = CreateSolidBrush(RGB(255, 150, 255));
    HPEN hBonoPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0)); // �׵θ�

    SelectObject(hdc, hBonoBrush);
    SelectObject(hdc, hBonoPen);

    int halfSize = size / 2;
    int left = centerX - halfSize;
    int top = centerY - halfSize;
    int right = centerX + halfSize;
    int bottom = centerY + halfSize;

    Ellipse(hdc, left, top, right, bottom);

    // �� �׸���
    int innerEllipseWidth = size / 7; // ���� Ÿ�� �ʺ�
    int innerEllipseHeight = size / 5; // ���� Ÿ�� ����

    // �� ��ġ
    int innerEllipseLeft = centerX - innerEllipseWidth / 2;
    int innerEllipseTop = centerY + size / 10; // �� �κ��� �����Ͽ� �� ��ġ ����
    int innerEllipseRight = innerEllipseLeft + innerEllipseWidth;
    int innerEllipseBottom = innerEllipseTop + innerEllipseHeight;

    SelectObject(hdc, hMouse);
    Ellipse(hdc, innerEllipseLeft, innerEllipseTop, innerEllipseRight, innerEllipseBottom);

    // �� �ڿ� �� �׸���
    SelectObject(hdc, hInner);
    int circleSize = size / 6; // ũ�� ����

    int circle1Left = centerX - circleSize; // ���� ��ǥ ����
    int circle1Top = centerY + size / 100; // ���� ��ǥ ����
    int circle1Right = circle1Left + circleSize; // ������ ��ǥ ����
    int circle1Bottom = circle1Top + circleSize; // �Ʒ��� ��ǥ ����

    int circle2Left = centerX + circleSize / 100; // ���� ��ǥ ����
    int circle2Top = centerY + size / 100; // ���� ��ǥ ����
    int circle2Right = circle2Left + circleSize; // ������ ��ǥ ����
    int circle2Bottom = circle2Top + circleSize; // �Ʒ��� ��ǥ ����

    Ellipse(hdc, circle1Left, circle1Top, circle1Right, circle1Bottom);
    Ellipse(hdc, circle2Left, circle2Top, circle2Right, circle2Bottom);

    // �� �׸���
    int noseSize = size / 7; // ũ�� ����
    int noseLeft = centerX - noseSize / 2; // ���� ��ǥ ����
    int noseTop = centerY - noseSize / 3; // ���� ��ǥ ����
    int noseRight = noseLeft + noseSize; // ������ ��ǥ ����
    int noseBottom = noseTop + noseSize; // �Ʒ��� ��ǥ ����

    SelectObject(hdc, hBonoEyes); // �� �� == �� ���� ����
    Ellipse(hdc, noseLeft, noseTop, noseRight, noseBottom);

    // ���� 1 (������ ��)
    int startX1 = centerX + size / 10; // ���� X ��ǥ
    int startY1 = centerY + size / 15; // ���� Y ��ǥ
    int endX1 = centerX + size / 3; // �� X ��ǥ
    int endY1 = centerY + size / 25; // �� Y ��ǥ

    // ���� 2 (������ �Ʒ�)
    int startX2 = centerX + size / 10; // ���� X ��ǥ
    int startY2 = centerY + size / 10; // ���� Y ��ǥ
    int endX2 = centerX + size / 3; // �� X ��ǥ
    int endY2 = centerY + size / 5; // �� Y ��ǥ

    // ���� 3 (���� ��)
    int startX3 = centerX - size / 10; // ���� X ��ǥ
    int startY3 = centerY + size / 15; // ���� Y ��ǥ
    int endX3 = centerX - size / 3; // �� X ��ǥ
    int endY3 = centerY + size / 25; // �� Y ��ǥ

    // ���� 4 (���� �Ʒ�)
    int startX4 = centerX - size / 10; // ���� X ��ǥ
    int startY4 = centerY + size / 10; // ���� Y ��ǥ
    int endX4 = centerX - size / 3; // �� X ��ǥ
    int endY4 = centerY + size / 5; // �� Y ��ǥ

    // ���� �׸���
    MoveToEx(hdc, startX1, startY1, NULL);
    LineTo(hdc, endX1, endY1);

    MoveToEx(hdc, startX2, startY2, NULL);
    LineTo(hdc, endX2, endY2);

    MoveToEx(hdc, startX3, startY3, NULL);
    LineTo(hdc, endX3, endY3);

    MoveToEx(hdc, startX4, startY4, NULL);
    LineTo(hdc, endX4, endY4);

    // �� �׸���
    int ellipseWidth = size / 30; // Ÿ�� �ʺ�
    int ellipseHeight = size / 15; // Ÿ�� ����

    // ���� ��
    int ellipse1Left = centerX - size / 2.5; // ���� ��ǥ ����
    int ellipse1Top = centerY - size / 8; // ���� ��ǥ ����
    int ellipse1Right = ellipse1Left + ellipseWidth; // ������ ��ǥ ����
    int ellipse1Bottom = ellipse1Top + ellipseHeight; // �Ʒ��� ��ǥ ����

    // ������ ��
    int ellipse2Left = centerX + size / 3; // ���� ��ǥ ����
    int ellipse2Top = centerY - size / 8; // ���� ��ǥ ����
    int ellipse2Right = ellipse2Left + ellipseWidth; // ������ ��ǥ ����
    int ellipse2Bottom = ellipse2Top + ellipseHeight; // �Ʒ��� ��ǥ ����

    // ���� ���� �Ͼ�� Ÿ��
    int innerEllipse1Left = ellipse1Left + ellipseWidth / 2.5; // ���� Ÿ���� ���� ��ǥ ����
    int innerEllipse1Top = ellipse1Top + ellipseHeight / 4; // ���� Ÿ���� ���� ��ǥ ����
    int innerEllipse1Right = ellipse1Right - ellipseWidth / 3; // ���� Ÿ���� ������ ��ǥ ����
    int innerEllipse1Bottom = ellipse1Bottom - ellipseHeight / 2; // ���� Ÿ���� �Ʒ��� ��ǥ ����

    int innerEllipse2Left = ellipse2Left + ellipseWidth / 2.5; // ���� Ÿ���� ���� ��ǥ ����
    int innerEllipse2Top = ellipse2Top + ellipseHeight / 4; // ���� Ÿ���� ���� ��ǥ ����
    int innerEllipse2Right = ellipse2Right - ellipseWidth / 3; // ���� Ÿ���� ������ ��ǥ ����
    int innerEllipse2Bottom = ellipse2Bottom - ellipseHeight / 2; // ���� Ÿ���� �Ʒ��� ��ǥ ����

    if (spaceKeyPressed) {
        // ���� �� ���
        int leftEyeCenterX = (ellipse1Left + ellipse1Right) / 2;
        int leftEyeCenterY = (ellipse1Top + ellipse1Bottom) / 2;
        int leftEyeLineLength = (ellipse1Right - ellipse1Left) * 2;

        MoveToEx(hdc, leftEyeCenterX, ellipse1Top, NULL);
        LineTo(hdc, leftEyeCenterX + leftEyeLineLength, leftEyeCenterY);
        MoveToEx(hdc, leftEyeCenterX + leftEyeLineLength, leftEyeCenterY, NULL);
        LineTo(hdc, leftEyeCenterX, ellipse1Bottom);

        // ������ �� ���
        int rightEyeCenterX = (ellipse2Left + ellipse2Right) / 2;
        int rightEyeCenterY = (ellipse2Top + ellipse2Bottom) / 2;
        int rightEyeLineLength = (ellipse2Right - ellipse2Left) * 2;

        MoveToEx(hdc, rightEyeCenterX, ellipse2Top, NULL);
        LineTo(hdc, rightEyeCenterX - rightEyeLineLength, rightEyeCenterY);
        MoveToEx(hdc, rightEyeCenterX - rightEyeLineLength, rightEyeCenterY, NULL);
        LineTo(hdc, rightEyeCenterX, ellipse2Bottom);
    }
    else {
        // �����̽� �ٰ� ������ ��� ���� �� ������� �׸��ϴ�
        SelectObject(hdc, hBonoEyes);
        Ellipse(hdc, ellipse1Left, ellipse1Top, ellipse1Right, ellipse1Bottom);
        Ellipse(hdc, ellipse2Left, ellipse2Top, ellipse2Right, ellipse2Bottom);

        // ���� ���� �Ͼ�� Ÿ���� �׸��ϴ�
        SelectObject(hdc, hInner);
        Ellipse(hdc, innerEllipse1Left, innerEllipse1Top, innerEllipse1Right, innerEllipse1Bottom);
        Ellipse(hdc, innerEllipse2Left, innerEllipse2Top, innerEllipse2Right, innerEllipse2Bottom);
    }

    DeleteObject(hBonoEyes); // �� ��
    DeleteObject(hBonoBrush); // �귯�� ����
    DeleteObject(hBonoPen); // �� ���� 
}

void DrawRyan(HDC hdc, int centerX, int centerY, int size) {
    RECT View;
    GetClipBox(hdc, &View);

    int halfSize = size / 2;
    int left = centerX - halfSize;
    int top = centerY - halfSize;
    int right = centerX + halfSize;
    int bottom = centerY + halfSize;

    // �� ������ ����� �׸��� ����
    if (left > View.right || right < View.left || top > View.bottom || bottom < View.top) {
        return;
    }

    int innerSize = size / 20; // ���� Ÿ���� ũ��
    int innerHalfSize = innerSize / 2;
    int earSize = size / 3;
    int earHalfSize = earSize / 2;

    int LearX = left + (earSize / 2);
    int LearY = centerY - 50 - (earSize / 2);

    int RearX = right - 10 - (earSize / 2);
    int RearY = LearY;

    // �� ������ ����� �͸� �׸��� ����
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

    // �� ������ ����� �͸� �׸��� ����
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
    int lEyeCenterY = centerY - innerSize; // ���� �߾ӿ� ��ġ

    HBRUSH hLEyeBrush = CreateSolidBrush(RGB(0, 0, 0)); // ������ �귯�� ����
    SelectObject(hdc, hLEyeBrush); // ���� Ÿ���� ������ �귯�÷� ����
    Ellipse(hdc, lEyeCenterX - innerHalfSize, lEyeCenterY - innerHalfSize, lEyeCenterX + innerHalfSize, lEyeCenterY + innerHalfSize);

    // hREyeBrush ��ġ�� ���������� �̵�
    int rEyeCenterX = centerX + innerHalfSize * 10;
    int rEyeCenterY = centerY - innerSize; // ���� �߾ӿ� ��ġ

    HBRUSH hREyeBrush = CreateSolidBrush(RGB(0, 0, 0)); // ������ �귯�� ����
    SelectObject(hdc, hREyeBrush); // ���� Ÿ���� ������ �귯�÷� ����
    Ellipse(hdc, rEyeCenterX - innerHalfSize, rEyeCenterY - innerHalfSize, rEyeCenterX + innerHalfSize, rEyeCenterY + innerHalfSize);

    int lineLength = size / 10;
    int distanceFromEyes = innerSize + 30;

    MoveToEx(hdc, lEyeCenterX - lineLength, lEyeCenterY - distanceFromEyes, NULL);
    LineTo(hdc, lEyeCenterX + lineLength, lEyeCenterY - distanceFromEyes);

    MoveToEx(hdc, rEyeCenterX - lineLength, rEyeCenterY - distanceFromEyes, NULL);
    LineTo(hdc, rEyeCenterX + lineLength, rEyeCenterY - distanceFromEyes);

    int whiteSize = size / 8; // ��� �� ũ�� ����
    int whiteHalfSize = whiteSize / 2;

    // �� ���� ��� ���� �̾���̱�
    int whiteX1 = centerX - whiteHalfSize; // ù ��° ��� ��
    int whiteY1 = centerY + innerSize * 3; // �Ʒ������� �̵�
    int whiteX2 = centerX + whiteHalfSize; // �� ��° ��� ��
    int whiteY2 = centerY + innerSize * 3; // �Ʒ������� �̵�

    HBRUSH hWhiteBrush = CreateSolidBrush(RGB(255, 255, 255)); // ��� �귯�� ����
    HPEN hYellowPen2 = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    SelectObject(hdc, hWhiteBrush); // ��� �귯�÷� ����
    SelectObject(hdc, hYellowPen2);
    Ellipse(hdc, whiteX1 - whiteHalfSize, whiteY1 - whiteHalfSize, whiteX1 + whiteHalfSize, whiteY1 + whiteHalfSize);
    Ellipse(hdc, whiteX2 - whiteHalfSize, whiteY2 - whiteHalfSize, whiteX2 + whiteHalfSize, whiteY2 + whiteHalfSize);

    DeleteObject(hLEyeBrush); // ����� �귯�� ����
    DeleteObject(hREyeBrush); // ����� �귯�� ����
    DeleteObject(hWhiteBrush);
    DeleteObject(hYellowBrush);
    DeleteObject(hYellowPen);
    DeleteObject(hYellowPen2);
}