#include <windows.h>
#include <cmath>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DrawBonobono(HDC hdc, int centerX, int centerY, int size);

HBRUSH hbrBackground;
HWND hwndMain; // ���� ������ �ڵ�

int bonobonoX, bonobonoY;
int shapeSize = 500; // �ʱ� ũ�⸦ ����
bool spaceKeyPressed = false;
bool drawBonobono = false; // ���뺸�� �׸��� ����

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
    hwndMain = CreateWindow(L"MyWindowClass", L"202218016 ��Ƹ�", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 100, 100, 1000, 800, NULL, NULL, GetModuleHandle(NULL), NULL);

    if (!hwndMain) {
        return 2;
    }

    // ��ư�� �����մϴ�.
    CreateWindow(L"BUTTON", L"Bonobono", WS_CHILD | WS_VISIBLE, 50, 50, 160, 64, hwndMain, (HMENU)1, GetModuleHandle(NULL), NULL);
    CreateWindow(L"BUTTON", L"Button2", WS_CHILD | WS_VISIBLE, 220, 50, 160, 64, hwndMain, (HMENU)2, GetModuleHandle(NULL), NULL);
    CreateWindow(L"BUTTON", L"Button3", WS_CHILD | WS_VISIBLE, 390, 50, 160, 64, hwndMain, (HMENU)3, GetModuleHandle(NULL), NULL);
    CreateWindow(L"BUTTON", L"Button4", WS_CHILD | WS_VISIBLE, 560, 50, 160, 64, hwndMain, (HMENU)4, GetModuleHandle(NULL), NULL);
    CreateWindow(L"BUTTON", L"Button5", WS_CHILD | WS_VISIBLE, 730, 50, 160, 64, hwndMain, (HMENU)5, GetModuleHandle(NULL), NULL);

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
            bonobonoX = bonobonoY = 0; // ���뺸�� ��ġ �ʱ�ȭ
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