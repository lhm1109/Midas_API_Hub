// MySEC3DTabWnd.h: interface for the MySEC3DTabWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSEC3DTABWND_H__0A34CB4A_5FAD_11D4_889A_0000C0A2329D__INCLUDED_)
#define AFX_MYSEC3DTABWND_H__0A34CB4A_5FAD_11D4_889A_0000C0A2329D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\MIT_frx\MBaseTabWnd.h"

class MySEC3DTabWnd : public mit::frx::MBaseTabWnd
{
public:
	
	DECLARE_DYNAMIC( MySEC3DTabWnd )
	MySEC3DTabWnd();
	virtual ~MySEC3DTabWnd();
	virtual void ActivateTab(int nIndex);
	virtual void ActivateTab(CWnd* pWnd, int nIndex);
	//!@#$BOOL CreateTabCtrl(DWORD dwStyle, UINT nID);

	virtual BOOL SetActiveTab (int iTab);
	virtual void FireChangeActiveTab (int nNewTab);

	// Generated message map functions
	//{{AFX_MSG(MySEC3DTabWnd)
	afx_msg LRESULT OnTabSelect(WPARAM wParam, LPARAM );
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnTreeCtrlNotify(WPARAM wParam, LPARAM lParam);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()  
};



class MySEC3DTabControl : public SEC3DTabControl
{
public:
	MySEC3DTabControl(): SEC3DTabControl(){};
	void SizeTabs(int nTotal, int nTotIcon, int nDimParam);
};

#endif // !defined(AFX_MYSEC3DTABWND_H__0A34CB4A_5FAD_11D4_889A_0000C0A2329D__INCLUDED_)