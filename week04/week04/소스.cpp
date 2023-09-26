// 강의 예제 : C코드
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#include <windows.h>

// 윈도우의 이벤트를 처리하는 콜백(Callback) 함수. => return 값을 LRESULT로 보내기로 정해져있음
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) // CALLBACK 함수 : 윈도우의 우저가 어떠한 행동을하면 알려주는 함수 => 다시 알려줌 
// hwnd : 창에 대한 핸들, unit : 메세지 -> 메세지 하나당 시스템 메세지 하나, wparam : 추가 메세지 정보, lparam: 추가 메세지 정보
{
	// 윈도우 디폴드 설정에 따라 처리함
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
#ifdef UNICODE
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) // 한글을 저장하기 위한 WinMain = > wWinMain : char 두 개 -> ANSI
// OS와 윈도우를 연결하기 위한 권한의 대한 영역 hinstance -> 윈도우 화면, 소리, 네트워크 권한 등 
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) // 영어만 사용하는 경우 WinMain으로도 가능 : char 한 개 -> ASCII
#endif
{
	// 윈도우 클래스 선언. -> struct : window.h 함수에 정의 : 윈도우 창 특성을 정의해둔 클래스 WNDCLASS
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));	// 모두 0으로 초기화. -> C는 새로 만들었을 때 쓰레기 값이 들어있기에 0으로 초기화 해야함

	// 윈도우 클래스 값 설정
	wc.hInstance = hInstance;
	wc.lpszClassName = TEXT("Computer Software");
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;

	// 윈도우 클래스 등록.
	if (RegisterClass(&wc) == false) // 레지스터 클래스로 원하는 윈도우 창?을 생성할 수 있도록 함 
	{
		exit(-1);	//예외
	}

	// Window viewport 영역 조정
	RECT rect = { 0, 0, 1600, 900 }; // 사각형 영역을 잡음 
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false); // AdjustWindowRect : 윈도우의 영역을 잡아옴
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// 윈도우 생성
	HWND hwnd = CreateWindow(
		wc.lpszClassName,
		TEXT("컴소 Application"), // 윈도우 타이틀
		WS_OVERLAPPEDWINDOW, // 중복해서 쓸 수 있는 윈도우의 권한
		0, 0,
		width, height, // 0, 0에 그림
		NULL, NULL,
		hInstance, // 자신의 인스턴스 번호를 윈도우에 보내줌 return은 윈도우 핸들러 값
		NULL
	);

	// 오류 검사.
	if (hwnd == NULL) // 아무 것도 주지 않은 경우 
	{
		exit(-1);
	}

	// 창 보이기.
	ShowWindow(hwnd, SW_SHOW); // 창 띄우고
	UpdateWindow(hwnd); // 업데이트해야 보임. 한 쌍으로 쓴다고 보면 됨.



	// 메시지 처리 루프.
// 엔진 루프.
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		// 메시지 처리.
		if (GetMessage(&msg, hwnd, 0, 0)) // 메세지가 없으면 대기
			//if (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) // PM_REMOVE의 자리는 이 메세지를 쓰고 어떡할거냐.의 의미인데 지운다는 것임.
		{
			// 메시지 해석해줘.
			TranslateMessage(&msg);
			// 메시지를 처리해야할 곳에 전달해줘.
			DispatchMessage(&msg);
		}
		else
		{

		}
	}

}