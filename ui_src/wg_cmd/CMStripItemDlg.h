#if !defined(AFX_CMStripItemDlg_H__5FD4EE01_64A1_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CMStripItemDlg_H__5FD4EE01_64A1_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMStripItemDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_DlgChild.h"

/////////////////////////////////////////////////////////////////////////////
// CCMStripItemDlg dialog

class CCMStripItemDlg : public CDialogMove
{
	// Construction
public:
	CCMStripItemDlg(CWnd* pParent = NULL);   // standard constructor

public:
	CDBDoc*  m_pDoc;
	BOOL		 m_bModify;
	T_PRCS_K m_Key;
	T_PRCS_D m_Data;

public:
	void SetModifyData(T_PRCS_K Key);

	// Dialog Data
	//{{AFX_DATA(CCMStripItemDlg)
	enum { IDD = IDD_CMD_STRIP_ITEM_DLG };
	CEdit     m_edtName;
	CEdit     m_edtDesc;
	CEditUnit m_edtW1;
	CTextUnit m_untW1;
	CEditUnit m_edtW2;
	CTextUnit m_untW2;
	CEditUnit m_edtW3;
	CTextUnit m_untW3;
	CEditUnit m_edtW4;
	CTextUnit m_untW4;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMStripItemDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	void InitUnit();
	BOOL Apply();
	void Data2Dlg();

	// Generated message map functions
	//{{AFX_MSG(CCMStripItemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnApply      ();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMStripItemDlg_H__5FD4EE01_64A1_11D5_880F_00010263A1F4__INCLUDED_)
