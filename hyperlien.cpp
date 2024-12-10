#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include <stdio.h>
#include <shellapi.h>
#include <string>
#include <iostream>
#include <fstream>
#include <tchar.h>
#include <sstream>
#include <thread> // For std::thread
#include <commctrl.h> 
 
#include <gdiplus.h>
#pragma comment (lib, "gdiplus.lib")

#pragma comment(lib, "comctl32.lib")

//#include <mmsystem.h>
//#pragma comment(lib, "winmm.lib")

 


 
#include <shlobj.h> // For CSIDL_APPDATA, CSIDL_COMMON_DOCUMENTS, et

#include "resource.h"
#include "drop.h"

#define IDC_CURSOR1 108

#define IDB_PNG1 103
#define IDB_PNG2 104
#define IDB_PNG3 105
#define IDB_PNG4 106
#define IDB_PNG5 107
#define IDB_PNG6 109
#define IDB_PNG7 110
#define IDB_PNG8 111
#define IDB_PNG9 113

#define IDB_PNG10 114
#define IDB_PNG11 115
#define IDB_PNG12 116
#define IDB_PNG13 117
#define IDB_PNG14 118
#define IDB_PNG15 119
#define  IDB_PNG16 124

#define ID_EDITBOX 101 
#define ID_LISTBOX 102  // Define an ID for the list box
#define ID_LISTBOX2 114

 
#define IDD_ABOUTBOX 123
HINSTANCE hInst;


 const wchar_t animationChars[] = L"/-\\| \0"; // Characters for animation
//const wchar_t   animationChars[] = L"◐◓◑◒";
//const wchar_t animationChars[] = L"▉▊▋▌▍▎▏▎▍▌▋▊▉"; // Sliding block for a progress effect
 //static const wchar_t frames[] = L"🕛🕐🕑🕒🕓🕔🕕🕖🕗🕘🕙🕚"; // Clock face hands

int animationIndex = 0; // Current animation frame
 
//void RunCommandAndCaptureOutputAsync(const std::wstring& command);
void RunCommandAndCaptureOutputAsync(const std::wstring& command, HWND hwndListBox);
 

wchar_t* URL = nullptr;

extern "C" {
    WINUSERAPI BOOL WINAPI SetLayeredWindowAttributes(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
}
void Transparent(HWND hWnd, int);
#define WS_EX_LAYERED           0x00080000
#define LWA_COLORKEY            0x00000001
#define LWA_ALPHA               0x00000002

#define ONE 1
#define TIMER_ID 2
#define FOCUS 3
#define MASK 4
#define clipb 5

HCURSOR hCustomCursor = nullptr;
 
std::wstring exeDirectory  ;
//std::wstring downloadsFolder  ;

HBITMAP splash = nullptr;
HBITMAP splashmask = nullptr;
HBITMAP hBitmap = nullptr; // Handle for the loaded bitmap
int mx = 0, my = 0;        // Mouse coordinates
bool dragging = false;     // Flag to track dragging state
POINT dragStart;           // Start point for dragging


HWND hConsoleOutput;



// Global variable for DropTarget
DropTarget* g_pDropTarget = nullptr;

//test
//HBITMAP  hBitmap1 = nullptr;
//HBITMAP  hBitmap2 = nullptr;

HWND hListBox;  // Global variable for the list box
HWND hEditBox;
HWND console;

std::wstring lastURL;  // Global variable to store the last detected URL

int running = 0;

HWND hwnd;

std::wstring GetExeDirectory();
//std::wstring GetDownloadsFolder();

HFONT hFont1 = nullptr;
HFONT hFont2 = nullptr;

using namespace Gdiplus;

ULONG_PTR gdiplusToken;
 Image* pImage = nullptr;
 Image* pImage2 = nullptr;
 Image* pImage3 = nullptr;
 Image* pImage4 = nullptr;
 Image* pImage5 = nullptr;
 Image* pImage6 = nullptr;
 Image* pImage7 = nullptr;
 Image* pImage8 = nullptr;
 Image* pImage9 = nullptr;

 Image* smily[6] ;
 
 void DrawInvertedImage(Graphics& graphics, Image* pImage, float x, float y,int opt);
void LoadPNGImage(HWND hwnd, UINT resourceID, Image** targetImage);
 

int ix1 = 0, iy1 = 0;
int add = 0;
int brain = 0;
bool stopReading = false;
// Set up PROCESS_INFORMATION
PROCESS_INFORMATION pi = {};

void InitializeGDIPlus() {
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
}
void ShutdownGDIPlus() {
    GdiplusShutdown(gdiplusToken);
