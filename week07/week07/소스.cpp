#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DrawRectangle(HDC hdc);
void EraseRectangle(HDC hdc);

bool isRectangleVisible = false; // �簢���� ���� ȭ�鿡 ǥ�õǴ��� ����

int main() {
    // ������ Ŭ������ ����մϴ�.
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"MyWindowClass";

    if (!RegisterClass(&wc)) {
        return 1;
    }

    // �����츦 �����մϴ�.
    HWND hwnd = CreateWindow(L"MyWindowClass", L"My Window", WS_OVERLAPPEDWINDOW, 100, 100, 400, 300, NULL, NULL, GetModuleHandle(NULL), NULL);

    if (!hwnd) {
        return 2;
    }

    // ��ư 1�� �����մϴ�.
    CreateWindow(L"BUTTON", L"Button 1", WS_CHILD | WS_VISIBLE, 50, 50, 100, 30, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);

    // ��ư 2�� �����մϴ�.
    CreateWindow(L"BUTTON", L"Button 2", WS_CHILD | WS_VISIBLE, 200, 50, 100, 30, hwnd, (HMENU)2, GetModuleHandle(NULL), NULL);

    // �����츦 ȭ�鿡 ǥ���մϴ�.
    ShowWindow(hwnd, SW_SHOWNORMAL);

    // ���� �޽��� ����
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
            // ��ư 1�� Ŭ������ �� �簢�� �׸���
            HDC hdc = GetDC(hwnd);
            if (!isRectangleVisible) {
                DrawRectangle(hdc);
                isRectangleVisible = true;
            }
            ReleaseDC(hwnd, hdc);
        }
        else if (LOWORD(wParam) == 2) {
            // ��ư 2�� Ŭ������ �� �簢�� �����
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
    // ������ �귯�� ����
    HBRUSH hRedBrush = CreateSolidBrush(RGB(255, 0, 0)); // ������

    // �簢�� �׸���
    RECT rect = { 150, 100, 250, 200 };
    FillRect(hdc, &rect, hRedBrush); // ������ �귯�� ���

    // �귯�� ����
    DeleteObject(hRedBrush);
}

void EraseRectangle(HDC hdc) {
    // �簢�� ������ ����� (�������� �����)
    RECT rect = { 150, 100, 250, 200 };
    HBRUSH hWhiteBrush = (HBRUSH)(COLOR_WINDOW + 1);
    FillRect(hdc, &rect, hWhiteBrush);
}
