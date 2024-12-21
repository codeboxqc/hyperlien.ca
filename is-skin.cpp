#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"
#define STBI_GAMMA_CORRECT
#include <windows.h>
#include <iostream>

#include <dwmapi.h> // For DwmExtendFrameIntoClientArea
#pragma comment(lib, "dwmapi.lib")

void ConvertRGBAToBGRA(unsigned char* data, int width, int height);


struct ImageData {
    unsigned char* data;
    int width;
    int height;
    int channels;
};

// Functions for loading and freeing image data
ImageData LoadPNG(const char* filename) {
    ImageData image = { nullptr, 0, 0, 0 };

     //image.data = stbi_load(filename, &image.width, &image.height, &image.channels, 3); // rgb

    // image.data = stbi_load(filename, &image.width, &image.height, &image.channels, 0); // No forced conversion

    image.data = stbi_load(filename, &image.width, &image.height, &image.channels, 4); // Force RGBA
    if (!image.data) {
        std::cerr << "Failed to load image: " << filename << std::endl;
    }

    // Ensure the image data is converted from RGBA to BGRA
    ConvertRGBAToBGRA(image.data, image.width, image.height);

    image.channels = 4;

    return image;
}

void FreeImage(ImageData& image) {
    if (image.data) {
        stbi_image_free(image.data);
        image.data = nullptr;
    }
}

// Utility to create HBITMAP from pixel data
HBITMAP CreateBitmapFromData(HDC hdc, unsigned char* data, int width, int height, int channels) {
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height; // Negative to indicate top-down DIB
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = channels * 8; // Assume 4 channels (32 bits per pixel)
    bmi.bmiHeader.biCompression = BI_RGB;

    void* pBits = nullptr;
    HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pBits, nullptr, 0);

    if (hBitmap && pBits) {
        memcpy(pBits, data, width * height * channels);
    }

    return hBitmap;
}

void ConvertRGBAToBGRA(unsigned char* data, int width, int height) {
    for (int i = 0; i < width * height; ++i) {
        unsigned char* pixel = data + i * 4;
        std::swap(pixel[0], pixel[2]); // Swap R (0) and B (2)
    }
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        

        HDC hdc = BeginPaint(hwnd, &ps);

        // Load image and render (hardcoded file for demonstration)
        static ImageData img = LoadPNG("skn.png");
        static HBITMAP hBitmap = CreateBitmapFromData(hdc, img.data, img.width, img.height, 4);

        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

      
        int x = (GetSystemMetrics(SM_CXSCREEN) - img.width) / 2;
        int y = (GetSystemMetrics(SM_CYSCREEN) - img.height) / 2;

        BitBlt(hdc,x,y, img.width, img.height, memDC, 0, 0, SRCCOPY);

        SelectObject(memDC, oldBitmap);
        DeleteDC(memDC);
        EndPaint(hwnd, &ps);
        Sleep(666);
        ShowWindow(hwnd, SW_HIDE);
        //ExitProcess(0);
        //PostQuitMessage(0);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int begin() {
    const wchar_t CLASS_NAME[] = L"TransparentWindowClass";

    // Register Window Class
    WNDCLASSEX wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = GetModuleHandle(nullptr);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.lpszClassName = CLASS_NAME;

    if (!RegisterClassEx(&wcex)) {
        std::cerr << "Failed to register window class!" << std::endl;
        return -1;
    }

    // Create the window
    HWND hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST, // Transparent and topmost
        CLASS_NAME, L"Transparent Window", WS_POPUP,       // Borderless
        0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        nullptr, nullptr, GetModuleHandle(nullptr), nullptr);

    if (!hwnd) {
        std::cerr << "Failed to create window!" << std::endl;
        return -1;
    }

    // Make window transparent
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

    // Show the window and set it as fullscreen
    SetWindowPos(hwnd, HWND_TOPMOST, -1, -1, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

   // Sleep(2000);

    return 0;
}
 