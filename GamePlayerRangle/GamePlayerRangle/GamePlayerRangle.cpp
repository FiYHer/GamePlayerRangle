// GamePlayerRangle.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "GamePlayerRangle.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
HWND g_hWnd = NULL;                             // 窗口句柄

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
UINT CALLBACK       Thread(LPVOID);

UINT CALLBACK Thread(LPVOID lpBuffer)
{
	tagPOINT stPos = { 0 };
	TCHAR szBuffer[100] = { 0 };
	TCHAR szText[100] = { 0 };
	double nX, nY;
	HDC hDc = NULL;
	HDC hDcMem = NULL;
	HBITMAP hBitmap = NULL;
	HBRUSH hBrush = NULL;
	RECT stRect = { 0,0,500,500 };
	double PI =  3.14159265;
	double Rangle = 0;
	while (true)
	{

		//获取当前鼠标在窗口里面的位置
		GetCursorPos(&stPos);
		ScreenToClient(g_hWnd, &stPos);

		//相对原点的坐标【原地x=250 y=250】
		nY = stPos.y - 250;
		nX = stPos.x - 250;

		//不能为0，否则会出现除零错误
		nY = (nY == 0) ? 1 : nY;
		nX = (nX == 0) ? 1 : nX;

		//获取角度
		Rangle = atan(nY / nX) * 180.0 / PI;
		wsprintf(szBuffer, TEXT("鼠标位置X=%d Y=%d"),stPos.x, stPos.y);
		wsprintf(szText, TEXT("相对坐标X=%d Y=%d 角度%d"), (INT)nX, (INT)nY, (INT)Rangle);

		hDc = GetDC(g_hWnd);
		hDcMem = CreateCompatibleDC(hDc);
		hBitmap = CreateCompatibleBitmap(hDcMem, 500, 500);
		SelectObject(hDcMem, hBitmap);
		hBrush = CreateSolidBrush(RGB(200, 200, 200));
		FillRect(hDcMem, &stRect, hBrush);

		//绘制坐标系
		MoveToEx(hDcMem, 250, 0, NULL);
		LineTo(hDcMem, 250, 500);
		MoveToEx(hDcMem, 0, 250, NULL);
		LineTo(hDcMem, 500, 250);

		//绘制当前鼠标位置到原点的线条
		MoveToEx(hDcMem, 250, 250, NULL);
		LineTo(hDcMem, stPos.x, stPos.y);

		SetBkMode(hDcMem, 0);
		TextOut(hDcMem, 0, 20, szBuffer, wcslen(szBuffer));
		TextOut(hDcMem, 0, 40, szText, wcslen(szText));

		BitBlt(hDc, 0, 0, 500, 500, hDcMem, 0, 0, SRCCOPY);
		ReleaseDC(g_hWnd, hDc);
		DeleteObject(hDcMem);
		DeleteObject(hBitmap);
		DeleteObject(hBrush);
	}
	return 0;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GAMEPLAYERRANGLE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMEPLAYERRANGLE));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAMEPLAYERRANGLE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GAMEPLAYERRANGLE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      200, 200, 500+6, 500+28, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		_beginthreadex(0, 0, Thread, 0, 0, 0);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
