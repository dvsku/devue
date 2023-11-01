#include "utilities/dv_util_dialog.hpp"

using namespace devue;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

void dv_util_dialog::open_file_dialog(std::string& selected_path, const std::vector<core::dv_file_filter>& filters) {
    std::vector<COMDLG_FILTERSPEC> vff;
    for (const core::dv_file_filter& filter : filters)
        vff.push_back({ filter.name.c_str(), filter.name.c_str() });

    IFileDialog* pfd = NULL;
    IFileDialogEvents* pfde = NULL;
    LPWSTR pszFilePath = NULL;

    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));

    if (!SUCCEEDED(hr))
        goto release_pfd;

    hr = event_handler::CreateInstance(IID_PPV_ARGS(&pfde));

    if (!SUCCEEDED(hr))
        goto release_pfde;

    DWORD dwCookie;
    pfd->Advise(pfde, &dwCookie);

    DWORD dwFlags;
    pfd->GetOptions(&dwFlags);
    pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM | FOS_FILEMUSTEXIST | FOS_PATHMUSTEXIST);
    pfd->SetFileTypes((UINT)vff.size(), vff.data());
    pfd->SetFileTypeIndex(1);

    hr = pfd->Show(NULL);

    if (!SUCCEEDED(hr))
        goto unadvise_pfd;

    IShellItem* psiResult;
    hr = pfd->GetResult(&psiResult);

    if (!SUCCEEDED(hr))
        goto unadvise_pfd;

    hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

    if (!SUCCEEDED(hr))
        goto release_psi;

    selected_path.resize(wcslen(pszFilePath));
    wcstombs(selected_path.data(), pszFilePath, 4096);

    if (SUCCEEDED(hr)) {
        LPWSTR pszFilePath = NULL;
        hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

        if (SUCCEEDED(hr)) {
            selected_path.resize(wcslen(pszFilePath));
            wcstombs(selected_path.data(), pszFilePath, 4096);
        }
        psiResult->Release();
    }

release_psi:
    psiResult->Release();
unadvise_pfd:
    pfd->Unadvise(dwCookie);
release_pfde:
    pfde->Release();
release_pfd:
    pfd->Release();
}

///////////////////////////////////////////////////////////////////////////////
// EVENT HANDLER

dv_util_dialog::event_handler::event_handler() : m_refs(1) {}

dv_util_dialog::event_handler::~event_handler() {}

HRESULT dv_util_dialog::event_handler::CreateInstance(REFIID riid, void** ppv) {
    *ppv = NULL;
    event_handler* ev_handler = new (std::nothrow) event_handler();

    HRESULT hr = ev_handler ? S_OK : E_OUTOFMEMORY;
    if (SUCCEEDED(hr)) {
        hr = ev_handler->QueryInterface(riid, ppv);
        ev_handler->Release();
    }

    return hr;
}

IFACEMETHODIMP dv_util_dialog::event_handler::QueryInterface(REFIID riid, void** ppv) {
    static const QITAB qit[] = {
        QITABENT(event_handler, IFileDialogEvents),
        QITABENT(event_handler, IFileDialogControlEvents),
        { 0 },
        #pragma warning(suppress:4838)
    };

    return QISearch(this, qit, riid, ppv);
}

IFACEMETHODIMP_(ULONG) dv_util_dialog::event_handler::AddRef() {
    return InterlockedIncrement(&m_refs);
}

IFACEMETHODIMP_(ULONG) dv_util_dialog::event_handler::Release() {
    long ref = InterlockedDecrement(&m_refs);

    if (!ref)
        delete this;

    return ref;
}

IFACEMETHODIMP dv_util_dialog::event_handler::OnTypeChange(IFileDialog* pfd) {
    IFileSaveDialog* pfsd;
    HRESULT hr = pfd->QueryInterface(&pfsd);
    return hr;
}

IFACEMETHODIMP dv_util_dialog::event_handler::OnItemSelected(IFileDialogCustomize* pfdc, DWORD dwIDCtl, DWORD dwIDItem) {
    IFileDialog* pfd = NULL;

    HRESULT hr = pfdc->QueryInterface(&pfd);
    if (SUCCEEDED(hr)) {
        pfd->Release();
    }

    return hr;
}