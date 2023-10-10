/*
 사각형 가운데에서 우클릭을 하면 그 위치를 확인하여 사각형 이내에 있는 곳이면 옮기는 걸로 인식 해야함
 원래 있는 사각형의 좌표와 새로 옮길 좌표를 계산하여 옮겨야함
 (A, B) (a, b) 를 이용해 그 사이에 들어가는 (x, y) 좌표가 우클릭되면 미리 그려둔 사각형을 옮길 수 있도록
*/
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#include <windows.h>

RECT drawRect = { 0 };
int isMouseLButtonPressed = 0;
HBRUSH pinkBrush; // 핑크색 브러시 핸들

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        pinkBrush = CreateSolidBrush(RGB(255, 0, 255)); // 핑크색 브러시 생성
        break;

    case WM_DESTROY:
        DeleteObject(pinkBrush); // 브러시 삭제
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

        // 현재 사각형 영역을 검은색으로 칠해 이전 사각형을 지우기
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        // 핑크색 브러시를 사용하여 사각형 안을 채움
        FillRect(hdc, &drawRect, pinkBrush);

        // 사각형의 테두리를 검은색으로 그림
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
    // 윈도우 생성 코드
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

    // 창 크기를 고정으로 설정
    int windowWidth = 800;
    int windowHeight = 600;

    hwnd = CreateWindow(
        wc.lpszClassName,
        TEXT("202218016 김아름"),
        WS_OVERLAPPED, // 윈도우 창 고정
        0, 0,
        windowWidth, windowHeight, // 창의 크기
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