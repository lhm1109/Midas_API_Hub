#if !defined(__ColmCapAssignDlg_H__)
#define      __ColmCapAssignDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColmCapAssignDlg.h : header file
//

// 유효보 생성 대화상자
// ElemCreateDlg와 기본동작은 같다.
// 4개의 폭 너비를 입력받아 Tapered section을 생성하여 사용

/////////////////////////////////////////////////////////////////////////////
// CColmCapAssignDlg dialog
#include "ElemCreateDlg.h"
#include "..\wg_base\wg_base_ControlEx.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
class CColmCapAssignDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CColmCapAssignDlg(CWnd* pParent = NULL);   // standard constructor
	~CColmCapAssignDlg();

public:
	virtual void Execute();
	  
	// Dialog Data
	//{{AFX_DATA(CColmCapAssignDlg)
	enum { IDD = IDD_TM_COLM_CAP_ASSIGN };
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColmCapAssignDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

protected:
	CDBDoc* m_pDoc;
	int m_nAddDel; // 0:Add/Replace, 1:Delete

	MComboBox m_cmbCcpt;
// Implementation
protected:
	void InitColmCapListCmb();
	// Generated message map functions
	//{{AFX_MSG(CColmCapAssignDlg)
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnTmCcpaButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ColmCapAssignDlg_H__E3A50C03_6049_11D3_8899_0000C0A2329D__INCLUDED_)
