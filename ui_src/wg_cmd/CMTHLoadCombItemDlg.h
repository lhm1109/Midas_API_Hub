#if !defined(AFX_CMTHLoadCombItemDlg_H__5FD4EE01_64A1_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CMTHLoadCombItemDlg_H__5FD4EE01_64A1_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMTHLoadCombItemDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MEdit.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMTHLoadCombItemDlg dialog

class CCMTHLoadCombItemDlg : public CDialogMove
{
// Construction
public:
	CCMTHLoadCombItemDlg(CWnd* pParent = NULL);   // standard constructor

public:
	CDBDoc* m_pDoc;
	BOOL		m_bModify;

protected:
	T_THLC_K m_Key;
	T_THLC_D m_Data;

	BOOL Dlg2Data();
	BOOL Data2Dlg();

public:
	void	SetData(T_THLC_K Key, T_THLC_D &Data);

// Dialog Data
	//{{AFX_DATA(CCMTHLoadCombItemDlg)
	enum { IDD = IDD_CMD_TH_LOAD_COMB_ITEM_DLG };
	MEdit     m_edtName;
	MEdit     m_edtDesc;
	CListBox	m_lstDefined  ;
	CListBox	m_lstSelected ;
	int m_nType;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMTHLoadCombItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL AddToDB();

	// Generated message map functions
	//{{AFX_MSG(CCMTHLoadCombItemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnApply();
	afx_msg void OnSelectBtn();
	afx_msg void OnUnselectBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTHLoadCombItemDlg_H__5FD4EE01_64A1_11D5_880F_00010263A1F4__INCLUDED_)
