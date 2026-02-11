#if !defined(AFX_DAMPERGBITEM_HDRD_H__E93289AF_D809_4145_94DE_FA909D7220BC__INCLUDED_)
#define AFX_DAMPERGBITEM_HDRD_H__E93289AF_D809_4145_94DE_FA909D7220BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NLLinkPrtGapDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CDamperGBItem_HDRD dialog

class CDamperGBItem_HDRD : public CChildDialog
{
// Construction
public:
	CDamperGBItem_HDRD(CWnd* pParent = NULL);   // standard constructor
	~CDamperGBItem_HDRD();

// Dialog Data
	//{{AFX_DATA(CDamperGBItem_HDRD)
	enum {IDD = IDD_ETC_DAMPER_GB_HDRD};
	CEditUnit	m_wndInitStiffEdit;
	CEditUnit	m_wndFyEdit;
	CEditUnit	m_wndPostStiffEdit;
	CEditUnit	m_wndPostStiffRtoEdit;
	CEditUnit	m_wndEquiStiffEdit;
	CEditUnit	m_wndEquiStiffRtoEdit;
	
	CTextUnit	m_wndInitStiffUnit;
	CTextUnit	m_wndFyUnit;
	CTextUnit	m_wndPostStiffUnit;
	CTextUnit	m_wndEquiStiffUnit;

	int m_nDof;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDamperGBItem_HDRD)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	T_DMGB_D m_Data;

	void SetData2Dlg(T_DMGB_D& data);
	BOOL SetDlg2Data(T_DMGB_D& data);
	void EnableDisableItemCtrls(int nInputMethod);

	afx_msg void OnUpdatePostStiffnessValue();
protected:
	// Generated message map functions
	//{{AFX_MSG(CDamperGBItem_HDRD)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL CheckData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DAMPERGBITEM_HDRD_H__E93289AF_D809_4145_94DE_FA909D7220BC__INCLUDED_)
