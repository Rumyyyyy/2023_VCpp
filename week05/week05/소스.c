/*
 �簢�� ������� ��Ŭ���� �ϸ� �� ��ġ�� Ȯ���Ͽ� �簢�� �̳��� �ִ� ���̸� �ű�� �ɷ� �ν� �ؾ���
 ���� �ִ� �簢���� ��ǥ�� ���� �ű� ��ǥ�� ����Ͽ� �Űܾ���
 (A, B) (a, b) �� �̿��� �� ���̿� ���� (x, y) ��ǥ�� ��Ŭ���Ǹ� �̸� �׷��� �簢���� �ű� �� �ֵ���
*/
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#include <windows.h>

RECT drawRect = { 0 };
int isMouseLButtonPressed = 0;
HBRUSH pinkBrush; // ��ũ�� �귯�� �ڵ�

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        pinkBrush = CreateSolidBrush(RGB(255, 0, 255)); // ��ũ�� �귯�� ����
        break;

    case WM_DESTROY:
        DeleteObject(pinkBrush); // �귯�� ����
        PostQuitMessage(0);
        break;

    case WM_LBUTTONDOWN:
    {
        drawRect.left = LOWORD(lParam);
        drawRect.top = HIWORD(lParam);
        drawRect.right = LOWORD(lParam);
        drawRect.bottom = HIWORD(lParam);
        isMouseLButtonPressed = 1;
    }
    break;

    case WM_MOUSEMOVE:
    {
        if (isMouseLButtonPressed)
        {
            drawRect.right = LOWORD(lParam);
            drawRect.bottom = HIWORD(lParam);
            InvalidateRect(hwnd, NULL, TRUE);
        }
    }
    break;

    case WM_LBUTTONUP:
    {
        isMouseLButtonPressed = 0;
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // ���� �簢�� ������ ���������� ĥ�� ���� �簢���� �����
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        // ��ũ�� �귯�ø� ����Ͽ� �簢�� ���� ä��
        FillRect(hdc, &drawRect, pinkBrush);

        // �簢���� �׵θ��� ���������� �׸�
        SelectObject(hdc, GetStockObject(BLACK_PEN));
        SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Rectangle(hdc, drawRect.left, drawRect.top, drawRect.right, drawRect.bottom);

        EndPaint(hwnd, &ps);
    }
    break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

#ifdef UNICODE
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
#else
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR pCmdLine, _In_ int nCmdShow)
#endif
{
    // ������ ���� �ڵ�
    HWND hwnd;

    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("Computer Software");
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_CROSS);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;

    if (RegisterClass(&wc) == 0)
    {
        MessageBox(NULL, L"RegisterClass failed!", L"Error", MB_ICONERROR);
        exit(-1);
    }

    // â ũ�⸦ �������� ����
    int windowWidth = 800;
    int windowHeight = 600;

    hwnd = CreateWindow(
        wc.lpszClassName,
        TEXT("202218016 ��Ƹ�"),
        WS_OVERLAPPED, // ������ â ����
        0, 0,
        windowWidth, windowHeight, // â�� ũ��
        NULL, NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL)
    {
        MessageBox(NULL, L"CreateWindow failed!", L"Error", MB_ICONERROR);
        exit(-1);
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while (msg.message != WM_QUIT)
    {
        if (GetMessage(&msg, hwnd, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}