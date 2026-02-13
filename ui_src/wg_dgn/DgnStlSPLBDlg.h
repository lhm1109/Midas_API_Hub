//splb
#if !defined(__DGNSTLSPLBDLG_H__)
#define      __DGNSTLSPLBDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlSLRSDlg.h : header file
//
#include "wg_dgn.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

// CDgnStlSPLBDlg 대화 상자입니다.

class CDBDoc;

class CDgnStlSPLBDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CDgnStlSPLBDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDgnStlSPLBDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DGN_STL_SPLB_DLG };
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

public:
	int m_nAddDel;
	int m_nCompression_Girder;
	int m_nFixing_Lateral_Bucking;
	int m_nLoadType_Lateral_Bucking;

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