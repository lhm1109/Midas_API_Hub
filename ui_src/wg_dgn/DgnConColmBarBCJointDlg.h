#if !defined(AFX_DGNCONCOLMBARBCJOINTDLG_H__)
#define AFX_DGNCONCOLMBARBCJOINTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConColmBarBCJointDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "DgnDrawWnd.h"


class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnConColmBarBCJointDlg dialog

class CDgnConColmBarBCJointDlg : public CDialogMove
{
// Construction
public:
	CDgnConColmBarBCJointDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnConColmBarBCJointDlg)
	enum { IDD = IDD_DGN_CON_COLM_BAR_BC_JOINT_DLG };
	CEditUnit	m_SpaceCtrl;
	CComboBox	m_SpaceYNumCtrl;
	CComboBox	m_SpaceZNumCtrl;
	CComboBox	m_BarSize;
	CSpinButtonCtrl	m_NrowSpin;
	CSpinButtonCtrl	m_NqrbSpin;
	CTextUnit	m_D0Unit;
	CString	m_strNqrb;
	CString	m_strNrow;
	CString	m_strBcjr;
	CSpinButtonCtrl	m_BcjrSpin;
	//}}AFX_DATA

public:
	CDgnDrawWnd m_SectWnd;
	T_RCHK_K m_RchkKey;
	int m_NqrbSpinNo,m_NrowSpinNo,m_BcjrSpinNo,m_nNqrb,m_nNrow,m_nBcjr;
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

	//void Initial_brExistData(T_REBR_D Data);
	void Initial_cExistData(T_REBC_D Data);
	void Initial_WithoutData();

	double Get_ChangeUnitFactor();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConColmBarBCJointDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnConColmBarBCJointDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposDgnRcNqrbspin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposDgnRcNrowspin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposDgnRcBcjrspin(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnChangeDgnRcCd0();
	afx_msg void OnChangeDgnRcNqrb();
	afx_msg void OnChangeDgnRcNrow();
	afx_msg void OnChangeDgnRcBcjr();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONCOLMBARBCJOINTDLG_H__)
