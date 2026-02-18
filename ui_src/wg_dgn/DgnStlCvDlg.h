#if !defined(AFX_DGNSTLCVDLG_H__147352C4_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNSTLCVDLG_H__147352C4_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlCvDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnStlCvDlg dialog

class CDgnStlCvDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnStlCvDlg(CWnd* pParent = NULL);   // standard constructor

public:
	void Initial_SelectItem();
	void Initial_Data();
	BOOL ErrorCheck();
	// Add by ZINU.('01.1.3).
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode();

public:


// Dialog Data
	//{{AFX_DATA(CDgnStlCvDlg)
	enum { IDD = IDD_DGN_STL_CV_DLG };
	int		m_nOption;
	double	m_Cv;
	BOOL	m_bAutoCv;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlCvDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnStlCvDlg)
	virtual void Execute();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnCvAr();
	afx_msg void OnDgnCvDel();
	afx_msg void OnDgnCvProgram();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNSTLCVDLG_H__147352C4_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
