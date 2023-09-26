// ���� ���� : C�ڵ�
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#include <windows.h>

// �������� �̺�Ʈ�� ó���ϴ� �ݹ�(Callback) �Լ�. => return ���� LRESULT�� ������� ����������
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) // CALLBACK �Լ� : �������� ������ ��� �ൿ���ϸ� �˷��ִ� �Լ� => �ٽ� �˷��� 
// hwnd : â�� ���� �ڵ�, unit : �޼��� -> �޼��� �ϳ��� �ý��� �޼��� �ϳ�, wparam : �߰� �޼��� ����, lparam: �߰� �޼��� ����
{
	// ������ ������ ������ ���� ó����
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
#ifdef UNICODE
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) // �ѱ��� �����ϱ� ���� WinMain = > wWinMain : char �� �� -> ANSI
// OS�� �����츦 �����ϱ� ���� ������ ���� ���� hinstance -> ������ ȭ��, �Ҹ�, ��Ʈ��ũ ���� �� 
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) // ��� ����ϴ� ��� WinMain���ε� ���� : char �� �� -> ASCII
#endif
{
	// ������ Ŭ���� ����. -> struct : window.h �Լ��� ���� : ������ â Ư���� �����ص� Ŭ���� WNDCLASS
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));	// ��� 0���� �ʱ�ȭ. -> C�� ���� ������� �� ������ ���� ����ֱ⿡ 0���� �ʱ�ȭ �ؾ���

	// ������ Ŭ���� �� ����
	wc.hInstance = hInstance;
	wc.lpszClassName = TEXT("Computer Software");
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;

	// ������ Ŭ���� ���.
	if (RegisterClass(&wc) == false) // �������� Ŭ������ ���ϴ� ������ â?�� ������ �� �ֵ��� �� 
	{
		exit(-1);	//����
	}

	// Window viewport ���� ����
	RECT rect = { 0, 0, 1600, 900 }; // �簢�� ������ ���� 
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false); // AdjustWindowRect : �������� ������ ��ƿ�
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// ������ ����
	HWND hwnd = CreateWindow(
		wc.lpszClassName,
		TEXT("�ļ� Application"), // ������ Ÿ��Ʋ
		WS_OVERLAPPEDWINDOW, // �ߺ��ؼ� �� �� �ִ� �������� ����
		0, 0,
		width, height, // 0, 0�� �׸�
		NULL, NULL,
		hInstance, // �ڽ��� �ν��Ͻ� ��ȣ�� �����쿡 ������ return�� ������ �ڵ鷯 ��
		NULL
	);

	// ���� �˻�.
	if (hwnd == NULL) // �ƹ� �͵� ���� ���� ��� 
	{
		exit(-1);
	}

	// â ���̱�.
	ShowWindow(hwnd, SW_SHOW); // â ����
	UpdateWindow(hwnd); // ������Ʈ�ؾ� ����. �� ������ ���ٰ� ���� ��.



	// �޽��� ó�� ����.
// ���� ����.
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		// �޽��� ó��.
		if (GetMessage(&msg, hwnd, 0, 0)) // �޼����� ������ ���
			//if (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) // PM_REMOVE�� �ڸ��� �� �޼����� ���� ��Ұų�.�� �ǹ��ε� ����ٴ� ����.
		{
			// �޽��� �ؼ�����.
			TranslateMessage(&msg);
			// �޽����� ó���ؾ��� ���� ��������.
			DispatchMessage(&msg);
		}
		else
		{

		}
	}

}