#pragma once

#ifndef skin
#define skin

#include <windows.h>
#include <string>

void   ViewSkin(HWND hwnd, HBITMAP bit, HRGN region, int x, int y, bool condition);
HRGN   SkinBitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor = 0, COLORREF cTolerance = 0x101010);
int   SkinSaveRegion(HRGN hRgn, char* name);
HRGN   SkinLoadRegion(char* name);

//int   SkinSaveRegion(HRGN hRgn, const std::string& name);
//HRGN   SkinLoadRegion(const std::string& name);
//HRGN   SkinBitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor = 0, COLORREF cTolerance = 0x101010);
//void   ViewSkin(HWND hwnd, HBITMAP bit, HRGN region, int x, int y, bool condition);

#endif // YOUR_HEADER_FILE_H
