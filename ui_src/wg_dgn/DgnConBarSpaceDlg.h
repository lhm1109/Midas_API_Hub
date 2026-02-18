#if !defined(AFX_DGNCONBARSPACEDLG_H__49F823C1_EA92_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNCONBARSPACEDLG_H__49F823C1_EA92_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConBarSpaceDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnConBarSpaceDlg dialog

class CDgnConBarSpaceDlg : public CDialogMove
{
// Construction
public:
	CDgnConBarSpaceDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnConBarSpaceDlg)
	enum { IDD = IDD_DGN_CON_BAR_SPACE_DLG };
	CListBox	m_aRebarSelList;
	CListBox	m_aRebarAllList;
	int		m_RebarSpaceRadio;
	int		m_iSpaceEdit;
	//}}AFX_DATA

public:
	CString m_TotalVSpace;
	CStringArray m_arSelSpaceMM, m_arSelSpaceIN;

	CString Get_SelectRebarSpace();
	BOOL Cutting_RebarSpaceString(CString str, CStringArray& BarSpace);
	BOOL Show_RebarKind(int iRebarUnit);
	BOOL Sort_SelRebarSpace();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConBarSpaceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnConBarSpaceDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnKsBarspaceRadio();
	afx_msg void OnDgnAddDbBtn();
	afx_msg void OnDgnAddUserBtn();
	afx_msg void OnDgnDelBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONBARSPACEDLG_H__49F823C1_EA92_11D3_888F_0000C0F30D4D__INCLUDED_)
