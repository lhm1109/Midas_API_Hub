#if !defined(AFX_CMMVHLITEMSTDBSBS5400PAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_)
#define AFX_CMMVHLITEMSTDBSBS5400PAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemStdBSBS5400Page.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdBSBS5400Page dialog
#include "..\MIT_frx\MComboBox.h"

#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"

using namespace mit::frx;

class CCMMvhlItemStdBSDlg;
class CCMMvhlItemStdBSBS5400Page : public CInternationalDlg
{
// Construction
public:
	CCMMvhlItemStdBSBS5400Page(T_MVHL_D* pData, CWnd* pParent = NULL);   // standard constructor

public:
	T_MVHL_D* m_pData;

// Dialog Data
	//{{AFX_DATA(CCMMvhlItemStdBSBS5400Page)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMS_BS_BS5400 };

	CEditUnit m_edtW1;
	CEditUnit m_edtW2;
	CEditUnit m_edtW2_exp;
	CEditUnit m_edtW3;
	CEditUnit m_edtL1;
	CEditUnit m_edtL2;
	CEditUnit m_edtPa;
	CEditUnit m_edtPb;
	CEditUnit m_edtNumUnit;
	CEditUnit m_edtD1;
	CEditUnit m_edtD2;
	CEditUnit m_edtD3;
	CEditUnit m_edtD4;
	CEditUnit m_edtD5;
	CEditUnit m_edtD6;
	CEditUnit m_edtd;

	T_MVHL_PATCH_LOAD m_PatchD;

	CTextUnit m_untW1;
	CTextUnit m_untW2;
	CTextUnit m_untW3;
	CTextUnit m_untL1;
	CTextUnit m_untL2;
	CTextUnit m_untPa;
	CTextUnit m_untPb;
	CTextUnit m_untD1;
	CTextUnit m_untD2;
	CTextUnit m_untD3;
	CTextUnit m_untD4;
	CTextUnit m_untD5;
	CTextUnit m_untD6;
	CTextUnit m_untd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemStdBSBS5400Page)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData2Dlg();
	BOOL SetDlg2Data();

protected:
	void InitUnit();

// Implementation
protected:
	CCMMvhlItemStdBSDlg* m_pParent;

	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemStdBSBS5400Page)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVHLITEMSTDBSBS5400PAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_)
