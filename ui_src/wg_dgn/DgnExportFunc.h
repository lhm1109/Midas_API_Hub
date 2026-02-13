#if !defined(__DGNEXPORTFUNC_H__)
#define __DGNEXPORTFUNC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace mit {
	namespace frx {
		/*!@#$*/class MBaseTabWnd;
	}
}

// Tree menu related exported functions
//!@#$extern __declspec(dllexport) void DGN_CreateBarTreeDlg(CWnd* pParent, SEC3DTabWnd* pTabWnd, int nWhich);
extern __declspec(dllexport) void DGN_CreateBarTreeDlg(CWnd* pParent, mit::frx::MBaseTabWnd* pTabWnd, int nWhich);
//!@#$extern __declspec(dllexport) void DGN_ShowChildDlgByID(SEC3DTabWnd* pTabWnd, int nCurMode);
extern __declspec(dllexport) void DGN_ShowChildDlgByID(mit::frx::MBaseTabWnd* pTabWnd, int nCurMode, CString& curString);

#endif // !defined(__DGNEXPORTFUNC_H__)