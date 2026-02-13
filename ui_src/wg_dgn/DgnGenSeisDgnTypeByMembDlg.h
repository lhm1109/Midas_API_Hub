#if !defined(AFX_DgnGenSeisDgnTypeByMembDlg_H__INCLUDED_)
#define AFX_DgnGenSeisDgnTypeByMembDlg_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenSeisDgnTypeByMembDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_base\wg_base_MouseEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnGenSeisDgnTypeByMembDlg dialog

class CDBDoc;

class CDgnGenSeisDgnTypeByMembDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnGenSeisDgnTypeByMembDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnGenSeisDgnTypeByMembDlg)
	enum { IDD = IDD_DGN_GEN_SEISDGN_TYPE_BYMEMB_DLG };
	int		m_nOption;
	int   m_nOptSeis;
	//}}AFX_DATA

public:
	void Initial_SelectItem();
	void Initial_Data();
	void Update_InitDataByCode();
	//BOOL ErrorCheck();
	// Add by ZINU.('02.3.21).
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenSeisDgnTypeByMembDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	void EnableAssignMemberCtrl(BOOL bEnable);
	// Generated message map functions
	//{{AFX_MSG(CDgnGenSeisDgnTypeByMembDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnSeismicAr();
	afx_msg void OnDgnSeismicDel();
	afx_msg void OnDgnSeisDgn();
	afx_msg void OnDgnNonSeisDgn();
	afx_msg void OnDgnNonSeisForce();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DgnGenSeisDgnTypeByMembDlg_H__INCLUDED_)
