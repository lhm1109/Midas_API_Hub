#if !defined(__DGNSTLSMSPDLG_H__)
#define      __DGNSTLSMSPDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlSLRSDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

// CDgnStlSMSPDlg 대화 상자입니다.

class CDBDoc;

class CDgnStlSMSPDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CDgnStlSMSPDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDgnStlSMSPDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DGN_STL_SMSP_DLG };
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

public:
	int m_nAddDel;
	int m_nMethod;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	CDBDoc* m_pDoc;

	// Generated message map functions
	virtual void Execute();
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};
#endif