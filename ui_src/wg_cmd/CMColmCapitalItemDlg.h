#if !defined(AFX_CMColmCapitalItemDlg_H__5FD4EE01_64A1_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CMColmCapitalItemDlg_H__5FD4EE01_64A1_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMColmCapitalItemDlg.h : header file
//
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MillustViewer.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CCMColmCapitalItemDlg dialog
using namespace mit::frx;

class CCMColmCapitalItemDlg : public CDialogMove
{
// Construction
public:
	CCMColmCapitalItemDlg(CWnd* pParent = NULL);   // standard constructor

public:
	CDBDoc* m_pDoc;
	BOOL		m_bModify;

protected:
	T_CCPT_K m_Key;
	T_CCPT_D m_Data;

	BOOL Dlg2Data();
	BOOL Data2Dlg();

public:
	void	SetData(T_CCPT_K Key, T_CCPT_D &Data);
	BOOL	OnApplyPublic();

// Dialog Data
	//{{AFX_DATA(CCMColmCapitalItemDlg)
	enum { IDD = IDD_CMD_COLM_CAPITAL_ITEM_DLG };
	MEdit     m_edtName;
	MEdit     m_edtDesc;
	CEditUnit m_edtB1;
	CTextUnit m_untB1;
	CEditUnit m_edtB2;
	CTextUnit m_untB2;
	CEditUnit m_edtH1;
	CTextUnit m_untH1;
	CEditUnit m_edtH2;
	CTextUnit m_untH2;
	CEditUnit m_edtAng;
	CTextUnit m_untAng;

	MButton	  m_chkAutoRigid;
	int m_nLocation;

	mit::frx::MillustViewer m_wndDropPanel;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMColmCapitalItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitUnit();
	BOOL AddToDB();

	// Generated message map functions
	//{{AFX_MSG(CCMColmCapitalItemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMColmCapitalItemDlg_H__5FD4EE01_64A1_11D5_880F_00010263A1F4__INCLUDED_)
