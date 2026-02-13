#if !defined(AFX_DGNCONCOLMBARDLG_H__A450E347_EE82_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNCONCOLMBARDLG_H__A450E347_EE82_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConColmBarDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "DgnDrawWnd.h"


class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnConColmBarDlg dialog

class CDgnConColmBarDlg : public CDialogMove
{
// Construction
public:
	CDgnConColmBarDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnConColmBarDlg)
	enum { IDD = IDD_DGN_CON_COLM_BAR_DLG };
	CEditUnit	m_SpaceCtrl;
	CComboBox	m_SpaceYNumCtrl;
	CComboBox	m_SpaceZNumCtrl;
	CComboBox	m_BarSize;
	CSpinButtonCtrl	m_NrowSpin;
	CSpinButtonCtrl	m_NqrbSpin;
	CTextUnit	m_D0Unit;
	CString	m_strNqrb;
	CString	m_strNrow;
	//}}AFX_DATA

public:
	CDgnDrawWnd m_SectWnd;
	T_RCHK_K m_RchkKey;
	int m_NqrbSpinNo,m_NrowSpinNo,m_nNqrb,m_nNrow;
	double m_Space,m_nD0,m_Hc,m_Bc;
	CString m_ColShape,m_strBarSize;
	int m_ElemType;
	int m_iSpaceYNum;
	int m_iSpaceZNum;

public:
	void Initial_Data();
	void Show_RectColumnShape();
	void Show_CircColumnShape();
	double Get_SectionDimension();

	void Initial_brExistData(T_REBR_D Data);
	void Initial_cExistData(T_REBC_D Data);
	void Initial_WithoutData();

	double Get_ChangeUnitFactor();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConColmBarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnConColmBarDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposDgnRcNqrbspin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposDgnRcNrowspin(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnChangeDgnRcCd0();
	afx_msg void OnChangeDgnRcNqrb();
	afx_msg void OnChangeDgnRcNrow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONCOLMBARDLG_H__A450E347_EE82_11D3_888F_0000C0F30D4D__INCLUDED_)
