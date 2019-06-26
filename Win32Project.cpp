// Win32Project.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Win32Project.h"
#include <sstream>
#include <vector>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32PROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT));

    MSG msg;

    // Main message loop:
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      700, 200, 290, 200, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

BOOL CALLBACK EnumWinProc(HWND, LPARAM);
DWORD WINAPI ShowHideWindow(LPVOID);
std::vector<HWND> listHwnd;
std::wstringstream ss, s1;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool enab_dis = true;
	static HWND h;
	static HANDLE thread;
	static DWORD thrId;
    switch (message)
    {
	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
		case VK_SHIFT:
		{
			if (enab_dis)
			{
				h = FindWindow(TEXT("CalcFrame"), TEXT("�����������"));
				if (h)
				{
					listHwnd.push_back(h);
					EnumChildWindows(h, EnumWinProc, NULL);
					thread = CreateThread(NULL, 0, &ShowHideWindow, &listHwnd, 0, &thrId);
					enab_dis = !enab_dis;
				}
				else
				{
					MessageBox(hWnd, TEXT("�������� �����������, ����������"), TEXT(""), 0);
				}
			}
			else
			{
				PostThreadMessage(thrId, WM_QUIT, 0, 0);
				WaitForSingleObject(thread, INFINITE);
				CloseHandle(thread);
				enab_dis = !enab_dis;
			}
			InvalidateRect(hWnd, NULL, true);
			break;
		}
		}
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
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
			if (enab_dis)
				TextOut(hdc, 0, 20, L"��������� ����������� � ������� Shift", lstrlen(L"��������� ����������� � ������� Shift"));
			else
				TextOut(hdc, 0, 20, L"��� ���������� ������� Shift ��� ���", lstrlen(L"��� ���������� ������� Shift ��� ���"));


            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		if (h)
			PostMessage(h, WM_QUIT, NULL, NULL);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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

BOOL CALLBACK EnumWinProc(HWND hWnd, LPARAM lParam)
{
	TCHAR lpszClsName[20];
	TCHAR checkName[20];
	lstrcpy(checkName, TEXT("Button"));
	GetClassName(hWnd, lpszClsName, 20);
	if (!lstrcmp(lpszClsName, checkName))
	{
		listHwnd.push_back(hWnd);
	}
	return TRUE;
}

DWORD WINAPI ShowHideWindow(LPVOID data)
{
	std::vector<HWND> *listHwnd = (std::vector<HWND>*)data;
	MSG msg;
	HWND htmp;
	int i;
	ZeroMemory(&msg, sizeof(MSG));
	srand(time(NULL));
	while (true)
	{
		if (PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE))
			break;
		while (!(i = rand() % listHwnd->size()));
		htmp = listHwnd->at(i);
		ShowWindow(htmp, SW_HIDE);
		InvalidateRect(listHwnd->at(0), NULL, true);
		Sleep(1000);
		ShowWindow(htmp, SW_SHOW);
		Sleep(1000);
		InvalidateRect(listHwnd->at(0), NULL, true);
	}
	return 0;
}