/*

#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>

int WINAPI SkinSaveRegion(HRGN hRgn, const std::string& name) {
    int iSize = GetRegionData(hRgn, sizeof(RGNDATA), NULL);

    if (iSize <= 0) {
        return 0; // Error in getting region data
    }

    // Allocate memory to hold the region data using std::vector
    std::vector<BYTE> data(iSize);
    RGNDATA* pData = reinterpret_cast<RGNDATA*>(data.data());
    pData->rdh.dwSize = iSize;

    int iSize2 = GetRegionData(hRgn, iSize, pData);
    if (iSize != iSize2) {
        return 0; // Error: sizes don't match
    }

    // Save region data to a file
    std::ofstream file(name, std::ios::binary);
    if (!file) {
        return 0; // Error opening file
    }
    file.write(reinterpret_cast<char*>(pData), iSize);

    return 1; // Success
}

HRGN WINAPI SkinLoadRegion(const std::string& name) {
    // Open file to read region data
    std::ifstream file(name, std::ios::binary);
    if (!file) {
        return NULL; // Error opening file
    }

    // Get size of the file
    file.seekg(0, std::ios::end);
    int iSize = static_cast<int>(file.tellg());
    file.seekg(0, std::ios::beg);

    // Allocate memory to hold the region data
    std::vector<BYTE> data(iSize);
    file.read(reinterpret_cast<char*>(data.data()), iSize);

    // Create region from loaded region data
    return ExtCreateRegion(NULL, iSize, reinterpret_cast<RGNDATA*>(data.data()));
}

HRGN WINAPI SkinBitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor = 0, COLORREF cTolerance = 0x101010) {
    if (!hBmp) {
        return NULL; // Error: Null bitmap
    }

    // Create a memory DC inside which we will scan the bitmap content
    HDC hMemDC = CreateCompatibleDC(NULL);
    if (!hMemDC) {
        return NULL; // Error creating DC
    }

    BITMAP bm;
    GetObject(hBmp, sizeof(bm), &bm);

    // Create a 32-bits depth bitmap and select it into the memory DC
    BITMAPINFOHEADER RGB32BITSBITMAPINFO = {
        sizeof(BITMAPINFOHEADER), bm.bmWidth, bm.bmHeight, 1, 32, BI_RGB, 0, 0, 0, 0, 0
    };
    void* pbits32 = nullptr;
    HBITMAP hbm32 = CreateDIBSection(hMemDC, reinterpret_cast<BITMAPINFO*>(&RGB32BITSBITMAPINFO), DIB_RGB_COLORS, &pbits32, NULL, 0);
    if (!hbm32) {
        DeleteDC(hMemDC);
        return NULL; // Error creating DIB section
    }

    // Select the new bitmap into the memory DC
    SelectObject(hMemDC, hbm32);

    // Use std::vector for region data
    std::vector<BYTE> regionData(sizeof(RGNDATAHEADER) + sizeof(RECT) * 100);
    RGNDATA* pData = reinterpret_cast<RGNDATA*>(regionData.data());
    pData->rdh.dwSize = sizeof(RGNDATAHEADER);
    pData->rdh.iType = RDH_RECTANGLES;
    pData->rdh.nCount = 0;
    SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

    // Scan bitmap for regions
    BYTE* p32 = reinterpret_cast<BYTE*>(bm.bmBits) + (bm.bmHeight - 1) * bm.bmWidthBytes;
    for (int y = 0; y < bm.bmHeight; ++y) {
        for (int x = 0; x < bm.bmWidth; ++x) {
            // Process pixels...
            // Add regions, check transparency, and adjust bounds...
        }
        p32 -= bm.bmWidthBytes;
    }

    // Return the created region
    HRGN hRgn = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + sizeof(RECT) * 100, pData);

    // Cleanup
    DeleteObject(hbm32);
    DeleteDC(hMemDC);

    return hRgn;
}

void WINAPI ViewSkin(HWND hwnd, HBITMAP bit, HRGN region, int x, int y, bool condition) {
    BITMAP bm;
    HDC hMemDC = CreateCompatibleDC(NULL);
    if (!hMemDC) {
        throw std::runtime_error("Failed to create compatible DC");
    }

    SelectObject(hMemDC, bit);
    GetObject(bit, sizeof(bm), &bm);

    HDC hdc = GetDC(hwnd);
    SetBkMode(hdc, TRANSPARENT);
    SetWindowRgn(hwnd, region, TRUE);
    BitBlt(hdc, x, y, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0, SRCCOPY);

    ReleaseDC(hwnd, hdc);
    DeleteDC(hMemDC);
}
*/







#include <windows.h>
#include <stdio.h>





#define _CRT_SECURE_NO_WARNINGS
 
int   SkinSaveRegion(HRGN hRgn, char* name)
{
    int iSize = GetRegionData(hRgn, sizeof(RGNDATA), NULL);

    // Allocate memory to hold the region data
    RGNDATA* pData = (RGNDATA*)calloc(iSize, 1);
    pData->rdh.dwSize = iSize;

    // Get the region data
    int iSize2 = GetRegionData(hRgn, iSize, pData);
    // Sanity check
    if (iSize != iSize2) return 0;

    // Save region data to a file using fopen_s (safe version)
    FILE* f;
    if (fopen_s(&f, name, "wb") != 0) {  // fopen_s returns 0 on success
        return 0; // Handle error
    }
    fwrite((char*)pData, sizeof(char), iSize, f);
    fclose(f);

    // Free allocated memory
    free(pData);

    return 1;
}



 
HRGN   SkinLoadRegion(char* name)
{
    // Open file to read region data using fopen_s (safe version)
    FILE* f;
    if (fopen_s(&f, name, "rb") != 0) {  // fopen_s returns 0 on success
        return NULL; // Handle error
    }

    // Get size of the file
    fseek(f, 0, SEEK_END);
    int iSize = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Allocate memory to hold the region data
    RGNDATA* pData = (RGNDATA*)calloc(iSize, 1);

    // Read region data from file
    fread((char*)pData, sizeof(char), iSize, f);
    fclose(f);

    // Create region from loaded region data
    HRGN hRgn = ExtCreateRegion(NULL, iSize, pData);

    // Free allocated memory
    free(pData);

    return hRgn;
}

/////////////////////////////////////////////////



 
HRGN   SkinBitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor = 0, COLORREF cTolerance = 0x101010)
{
    HRGN hRgn = NULL;

    //cTransparentColor=0;
    //cTolerance = 0x101010;

    if (hBmp)
    {
        // Create a memory DC inside which we will scan the bitmap content
        HDC hMemDC = CreateCompatibleDC(NULL);
        if (hMemDC)
        {
            // Get bitmap size
            BITMAP bm;
            GetObject(hBmp, sizeof(bm), &bm);

            // Create a 32 bits depth bitmap and select it into the memory DC
            BITMAPINFOHEADER RGB32BITSBITMAPINFO = {
                            sizeof(BITMAPINFOHEADER), // biSize
                            bm.bmWidth,  // biWidth;
                            bm.bmHeight, // biHeight;
                            1,    // biPlanes;
                            32,    // biBitCount
                            BI_RGB,    // biCompression;
                            0,    // biSizeImage;
                            0,    // biXPelsPerMeter;
                            0,    // biYPelsPerMeter;
                            0,    // biClrUsed;
                            0    // biClrImportant;
            };
            VOID* pbits32;
            HBITMAP hbm32 = CreateDIBSection(hMemDC, (BITMAPINFO*)
                &RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0);
            if (hbm32)
            {
                HBITMAP holdBmp = (HBITMAP)SelectObject(hMemDC, hbm32);

                // Create a DC just to copy the bitmap into the memory DC
                HDC hDC = CreateCompatibleDC(hMemDC);
                if (hDC)
                {
                    // Get how many bytes per row we have for the bitmap bits (rounded
                    //up to 32 bits)
                    BITMAP bm32;
                    GetObject(hbm32, sizeof(bm32), &bm32);
                    while (bm32.bmWidthBytes % 4)
                        bm32.bmWidthBytes++;

                    // Copy the bitmap into the memory DC
                    HBITMAP holdBmp = (HBITMAP)SelectObject(hDC, hBmp);
                    BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY);

                    // For better performances, we will use the ExtCreateRegion()
                    //function to create the
                    // region. This function take a RGNDATA structure on entry. We
                    //will add rectangles by
                    // amount of ALLOC_UNIT number in this structure.
#define ALLOC_UNIT 100
                    DWORD maxRects = ALLOC_UNIT;
                    HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) +
                        (sizeof(RECT) * maxRects));
                    RGNDATA* pData = (RGNDATA*)GlobalLock(hData);

                    pData->rdh.dwSize = sizeof(RGNDATAHEADER);
                    pData->rdh.iType = RDH_RECTANGLES;
                    pData->rdh.nCount = pData->rdh.nRgnSize = 0;
                    SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

                    // Keep on hand highest and lowest values for the "transparent"
                    //pixels
                    BYTE lr = GetRValue(cTransparentColor);
                    BYTE lg = GetGValue(cTransparentColor);
                    BYTE lb = GetBValue(cTransparentColor);

                    //BYTE hr = min(0xff, lr + GetRValue(cTolerance));
                    //BYTE hg = min(0xff, lg + GetGValue(cTolerance));
                    //BYTE hb = min(0xff, lb + GetBValue(cTolerance));

                    BYTE hr = 0;
                    BYTE hg = 0;
                    BYTE hb = 0;

                    // Scan each bitmap row from bottom to top (the bitmap is inverted
                    //vertically)
                    BYTE* p32 = (BYTE*)bm32.bmBits + (bm32.bmHeight - 1) *
                        bm32.bmWidthBytes;
                    for (int y = 0; y < bm.bmHeight; y++)
                    {
                        // Scan each bitmap pixel from left to right
                        for (int x = 0; x < bm.bmWidth; x++)
                        {
                            // Search for a continuous range of "non transparent pixels"
                            int x0 = x;
                            LONG* p = (LONG*)p32 + x;
                            while (x < bm.bmWidth)
                            {
                                BYTE b = GetRValue(*p);
                                if (b >= lr && b <= hr)
                                {
                                    b = GetGValue(*p);
                                    if (b >= lg && b <= hg)
                                    {
                                        b = GetBValue(*p);
                                        if (b >= lb && b <= hb)
                                            // This pixel is "transparent"
                                            break;
                                    }
                                }
                                p++;
                                x++;
                            }

                            if (x > x0)
                            {
                                // Add the pixels (x0, y) to (x, y+1) as a new rectangle in
                                //the region
                                if (pData->rdh.nCount >= maxRects)
                                {
                                    GlobalUnlock(hData);
                                    maxRects += ALLOC_UNIT;
                                    hData = GlobalReAlloc(hData, sizeof(RGNDATAHEADER) +
                                        (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
                                    pData = (RGNDATA*)GlobalLock(hData);
                                }
                                RECT* pr = (RECT*)&pData->Buffer;
                                SetRect(&pr[pData->rdh.nCount], x0, y, x, y + 1);
                                if (x0 < pData->rdh.rcBound.left)
                                    pData->rdh.rcBound.left = x0;
                                if (y < pData->rdh.rcBound.top)
                                    pData->rdh.rcBound.top = y;
                                if (x > pData->rdh.rcBound.right)
                                    pData->rdh.rcBound.right = x;
                                if (y + 1 > pData->rdh.rcBound.bottom)
                                    pData->rdh.rcBound.bottom = y + 1;
                                pData->rdh.nCount++;

                                // On Windows98, ExtCreateRegion() may fail if the number of
                                //rectangles is too
                                // large (ie: > 4000). Therefore, we have to create the
                                //region by multiple steps.
                                if (pData->rdh.nCount == 2000)
                                {
                                    HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) +
                                        (sizeof(RECT) * maxRects), pData);
                                    if (hRgn)
                                    {
                                        CombineRgn(hRgn, hRgn, h, RGN_OR);
                                        DeleteObject(h);
                                    }
                                    else
                                        hRgn = h;
                                    pData->rdh.nCount = 0;
                                    SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
                                }
                            }
                        }

                        // Go to next row (remember, the bitmap is inverted vertically)
                        p32 -= bm32.bmWidthBytes;
                    }

                    // Create or extend the region with the remaining rectangles
                    HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) +
                        (sizeof(RECT) * maxRects), pData);
                    if (hRgn)
                    {
                        CombineRgn(hRgn, hRgn, h, RGN_OR);
                        DeleteObject(h);
                    }
                    else
                        hRgn = h;

                    // Clean up
                    GlobalFree(hData);
                    SelectObject(hDC, holdBmp);
                    DeleteDC(hDC);
                }

                DeleteObject(SelectObject(hMemDC, holdBmp));
            }

            DeleteDC(hMemDC);
        } // if hMemDC
    } // if hBmp
    return hRgn;
}


//SetWindowRgn(hh, region,TRUE);

 
void   ViewSkin(HWND hwnd, HBITMAP bit, HRGN region, int x, int y, bool condition)
{

    BITMAP bm;
    HDC    hMemDC;
    POINT  pt;
    HDC hdc;

    hdc = GetDC(hwnd);
    hMemDC = CreateCompatibleDC(hdc);
    SelectObject(hMemDC, bit);

    GetObject(bit, sizeof(BITMAP), (PSTR)&bm);
    SetBkMode(hdc, TRANSPARENT);
    SetWindowRgn(hwnd, region, TRUE);
    BitBlt(hdc, x, y, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0, SRCCOPY);
    ReleaseDC(hwnd, hdc);
    DeleteDC(hMemDC);

}
