#include "drop.h"
#include <windows.h>
#include <shobjidl.h>
#include <shellapi.h>
#include <string>




// Constructor definition
DropTarget::DropTarget(HWND hWnd) : m_refCount(1), m_hWnd(hWnd) {}

// IUnknown methods
HRESULT DropTarget::QueryInterface(REFIID riid, void** ppvObject) {
    if (riid == IID_IUnknown || riid == IID_IDropTarget) {
        *ppvObject = static_cast<IDropTarget*>(this);
        AddRef();
        return S_OK;
    }
    *ppvObject = nullptr;
    return E_NOINTERFACE;
}

ULONG DropTarget::AddRef() {
    return InterlockedIncrement(&m_refCount);
}

ULONG DropTarget::Release() {
    ULONG count = InterlockedDecrement(&m_refCount);
    if (count == 0) {
        delete this;
    }
    return count;
}

// IDropTarget methods
HRESULT DropTarget::DragEnter(IDataObject* pDataObject, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) {
    *pdwEffect = DROPEFFECT_COPY;
    return S_OK;
}

HRESULT DropTarget::DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) {
    *pdwEffect = DROPEFFECT_COPY;
    return S_OK;
}

HRESULT DropTarget::DragLeave() {
    return S_OK;
}

HRESULT DropTarget::Drop(IDataObject* pDataObject, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) {
    FORMATETC format = { CF_UNICODETEXT, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
    STGMEDIUM medium;
    wchar_t* url = nullptr;

    if (SUCCEEDED(pDataObject->GetData(&format, &medium))) {
        url = (wchar_t*)GlobalLock(medium.hGlobal);
        if (url) {
            // Store the dropped URL in the member variable
            m_lastDroppedUrl = url;
            ProcessDroppedData(url);  // Optionally, process the URL

            // Unlock memory
            GlobalUnlock(medium.hGlobal);
             
        }
        ReleaseStgMedium(&medium);
    }

     

    *pdwEffect = DROPEFFECT_COPY;

    // After processing, clear the buffer
   // ClearBuffer();

    return S_OK;
}

// Method to clear the buffer after processing a drop
void DropTarget::ClearBuffer() {
    // Clear the last dropped URL to reset the state


    
    // Clear the last dropped URL to reset the state
    m_lastDroppedUrl.clear();  // Clears the content of the string (empty string)
    m_lastDroppedUrl = L"";    // Optionally, explicitly reset it to an empty string (redundant but clear)
   

    // Any additional cleanup logic if required can be added here
}

// Process the dropped data (URL)
void DropTarget::ProcessDroppedData(const wchar_t* url) {
    // Custom logic to process the dropped URL
    //wprintf(L"Dropped URL: %s\n", url);
}

// Getter for the last dropped URL
const std::wstring& DropTarget::GetLastDroppedUrl() const {
    return m_lastDroppedUrl;
}

// Register and Unregister functions
HRESULT RegisterDropTarget(HWND hWnd, DropTarget** ppDropTarget) {
    *ppDropTarget = new DropTarget(hWnd);

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr)) {
        hr = RegisterDragDrop(hWnd, *ppDropTarget);
    }

    return hr;
}

void UnregisterDropTarget(HWND hWnd, DropTarget* pDropTarget) {
    RevokeDragDrop(hWnd);
    pDropTarget->Release();
    CoUninitialize();
}
