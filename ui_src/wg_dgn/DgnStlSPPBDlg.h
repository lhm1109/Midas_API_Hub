//sppb
#if !defined(__DGNSTLSPPBDLG_H__)
#define      __DGNSTLSPPBDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlSLRSDlg.h : header file
//
#include "wg_dgn.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

// CDgnStlSPPBDlg 대화 상자입니다.

class CDBDoc;

class CDgnStlSPPBDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CDgnStlSPPBDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDgnStlSPPBDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DGN_STL_SPPB_DLG };
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

public:
	int m_nAddDel;
	BOOL m_bCheck;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	CDBDoc* m_pDoc;

	// Generated message map functions
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};
#endif