#pragma once

#include "importers/dv_file_type.hpp"

#define STRICT_TYPED_ITEMIDS
#include <shlobj.h>
#include <shlwapi.h>
#include <string>
#include <vector>

namespace devue {
    class dv_util_dialog {
    public:
    	static void open_file_dialog(std::string& selected_path, const std::vector<core::dv_file_filter>& filters);

    private:
    	class event_handler : public IFileDialogEvents, public IFileDialogControlEvents {
    	public:
    		event_handler();

    		static HRESULT CreateInstance(REFIID riid, void** ppv);

    		IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv);

    		IFACEMETHODIMP_(ULONG) AddRef();
    		IFACEMETHODIMP_(ULONG) Release();

    		IFACEMETHODIMP OnFileOk(IFileDialog*) { return S_OK; };
    		IFACEMETHODIMP OnFolderChange(IFileDialog*) { return S_OK; };
    		IFACEMETHODIMP OnFolderChanging(IFileDialog*, IShellItem*) { return S_OK; };
    		IFACEMETHODIMP OnHelp(IFileDialog*) { return S_OK; };
    		IFACEMETHODIMP OnSelectionChange(IFileDialog*) { return S_OK; };
    		IFACEMETHODIMP OnShareViolation(IFileDialog*, IShellItem*, FDE_SHAREVIOLATION_RESPONSE*) { return S_OK; };
    		IFACEMETHODIMP OnOverwrite(IFileDialog*, IShellItem*, FDE_OVERWRITE_RESPONSE*) { return S_OK; };

    		IFACEMETHODIMP OnTypeChange(IFileDialog* pfd);
    		IFACEMETHODIMP OnItemSelected(IFileDialogCustomize* pfdc, DWORD dwIDCtl, DWORD dwIDItem);

    		IFACEMETHODIMP OnButtonClicked(IFileDialogCustomize*, DWORD) { return S_OK; };
    		IFACEMETHODIMP OnCheckButtonToggled(IFileDialogCustomize*, DWORD, BOOL) { return S_OK; };
    		IFACEMETHODIMP OnControlActivating(IFileDialogCustomize*, DWORD) { return S_OK; };

    	private:
    		~event_handler();
    		long m_refs;
    	};
    };
}