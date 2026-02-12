#if !defined(AFX_DAMPERGBITEM_VFD_H__INCLUDED_)
#define AFX_DAMPERGBITEM_VFD_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NLLinkPrtGapDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CDamperGBItem_VFD dialog

class CDamperGBItem_VFD : public CChildDialog
{
// Construction
public:
	CDamperGBItem_VFD(CWnd* pParent = NULL);   // standard constructor
	~CDamperGBItem_VFD();

// Dialog Data
	//{{AFX_DATA(CDamperGBItem_VFD)
	enum {IDD = IDD_ETC_DAMPER_GB_VFD};
	CEditUnit	m_wndDamperFEdit;
	CEditUnit	m_wndIndexEdit;
	CEditUnit	m_wndSpringStiffEdit;
	CEditUnit	m_wndDamperStiffEdit;
	CEditUnit	m_wndDesignFEdit;
	CEditUnit	m_wndDispEdit;

	CTextUnit	m_wndDamperFUnit;
	CTextUnit	m_wndSpringStiffUnit;
	CTextUnit	m_wndDesignFUnit;
	CTextUnit	m_wndDispUnit;
	int m_nDof;
	int m_nDampType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDamperGBItem_VFD)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnCmdDamperTypeRdo();
	//}}AFX_VIRTUAL

// Implementation
public:
	T_DMGB_D m_Data;

	void SetData2Dlg(T_DMGB_D& data);
	BOOL SetDlg2Data(T_DMGB_D& data);
	void EnableDisableItemCtrls(int nInputMethod);
protected:
	// Generated message map functions
	//{{AFX_MSG(CDamperGBItem_VFD)
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

#endif // !defined(AFX_DAMPERGBITEM_VFD_H__E93289AF_D809_4145_94DE_FA909D7220BC__INCLUDED_)
