// ILI_sim.cpp : Defines the entry point for the application.
//

#include <stdint.h>
#include "framework.h"
#include "ILI_sim.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND ghWnd;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// fake Arduino in Display.cpp
void setup();
void loop();

COLORREF arr_screen[320 * 240];
// ILI uses an "active rectangle so that subsequent data writes go across
// the X and down the Y so hold the bounds here.
int sim_x0, sim_x1, sim_y0, sim_y1;
int x_ptr, y_ptr;

void simSetAddr(int x0, int y0, int x1, int y1) {
    sim_x0 = x0;
    sim_y0 = y0;
    sim_x1 = x1;
    sim_y1 = y1;
    x_ptr = x0;
    y_ptr = y0;
}

void writeSim(uint16_t data) {
	uint8_t r, g, b;
	if ((x_ptr > sim_x1) && (y_ptr > sim_y1)) {
		// stop if beyond x1,y1
		return;
	}
	r = (data & 0xF800) >> 8; // 1111 1000 0000 0000 : 5
	g = (data & 0x07E0) >> 3;  // 0000 0111 1110 0000 : 6
	b = (data & 0x001F) << 3;  // 0000 0000 0001 1111 : 5
	arr_screen[(y_ptr * 320) + x_ptr] = (r << 16) | (g << 8) | b;
    x_ptr++;
    if (x_ptr > sim_x1) {
        x_ptr = sim_x0;
        y_ptr++;
    }
}

void simUpdate() {
	InvalidateRect(ghWnd, NULL, true);
}

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
    LoadStringW(hInstance, IDC_ILISIM, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ILISIM));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ILISIM));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ILISIM);
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
      CW_USEDEFAULT, 0, 400, 360, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   int R, G, B;
   for (int n = 0; n < (320 * 240); n++) {
       R = 0;
	   G = 0; //rand() & 0xFF;
	   B = 0xFF; // rand() & 0xFF;
       arr_screen[n] = (R << 16) | (G << 8) | B;
   }

   // take global copy of wiondow handle so we can invalidate it to force WM_PAINT
   ghWnd = hWnd;
   setup();
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
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
            HDC src;
            HDC hdc = BeginPaint(hWnd, &ps);
            HBITMAP map;
            map = CreateBitmap(320, 240, 1, 8 * 4, (void *)arr_screen);
            src = CreateCompatibleDC(hdc);
            SelectObject(src, map);
            BitBlt(hdc, // Destination
                25,  // x and
                25,  // y - upper-left corner of place, where we'd like to copy
                320, // width of the region
                240, // height
                src, // source
                0,   // x and
                0,   // y of upper left corner  of part of the source, from where we'd like to copy
                SRCCOPY); // Defined DWORD to juct copy pixels. Watch more on msdn;
            DeleteDC(src);
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
