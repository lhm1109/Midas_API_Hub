#if !defined(AFX_CMMVHLITEMUSRBSHBPAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_)
#define AFX_CMMVHLITEMUSRBSHBPAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemUsrBSHBPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrBSHBPage dialog
#include "..\wg_db\TextUnit.h"
#include "..\wg_db\EditUnit.h"

struct T_MVHL_D;
class CCMMvhlItemUsrBSHBPage : public CInternationalDlg
{
// Construction
public:
	CCMMvhlItemUsrBSHBPage(T_MVHL_D* pData, CWnd* pParent = NULL);   // standard constructor

public:
	T_MVHL_D* m_pData;

// Dialog Data
	//{{AFX_DATA(CCMMvhlItemUsrBSHBPage)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMU_BS_HB_PAGE };
	CEditUnit m_edtNumUnit;
	CEditUnit m_edtBSPb;
	CEditUnit m_edtBSD1;
	CEditUnit m_edtBSD2;
	CEditUnit m_edtBSD;
	CEditUnit m_edtBSD3;
	CEditUnit m_edtBSD4;
	CEditUnit m_edtBSD5;
	CEditUnit m_edtBSD6;
	CTextUnit	m_untBSPb;
	CTextUnit	m_untBSD1;
	CTextUnit	m_untBSD2;
	CTextUnit	m_untBSD;
	CTextUnit	m_untBSD3;
	CTextUnit	m_untBSD4;
	CTextUnit	m_untBSD5;
	CTextUnit m_untBSD6;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemUsrBSHBPage)
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
	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemUsrBSHBPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeLoadDistEdit();
		//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVHLITEMUSRBSHBPAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_)
