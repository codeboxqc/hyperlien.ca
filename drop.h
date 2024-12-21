#ifndef DROP_H
#define DROP_H

#include <windows.h>
#include <shobjidl.h>
#include <string>

class DropTarget : public IDropTarget {
private:
    LONG m_refCount;
    HWND m_hWnd;
    std::wstring m_lastDroppedUrl;

public:
    DropTarget(HWND hWnd);  // Constructor declaration

    // IUnknown methods
    HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override;
    ULONG __stdcall AddRef() override;
    ULONG __stdcall Release() override;

    // IDropTarget methods
    HRESULT __stdcall DragEnter(IDataObject* pDataObject, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override;
    HRESULT __stdcall DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override;
    HRESULT __stdcall DragLeave() override;
    HRESULT __stdcall Drop(IDataObject* pDataObject, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override;

    // Custom methods
    void ProcessDroppedData(const wchar_t* url);
    const std::wstring& GetLastDroppedUrl() const;

    // Method to clear the buffer
    void ClearBuffer();
};

// Register and Unregister functions
HRESULT RegisterDropTarget(HWND hWnd, DropTarget** ppDropTarget);
void UnregisterDropTarget(HWND hWnd, DropTarget* pDropTarget);

#endif // DROP_H
