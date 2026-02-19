#if !defined(AFX_DGNCONBEAMBARDLG_H__27BE1765_EAF6_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNCONBEAMBARDLG_H__27BE1765_EAF6_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConBeamBarDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\SpinBtnExCtrl.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\mit_frx\MComboBox.h"

#include "DgnDrawWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamBarDlg dialog
using namespace mit::frx;
class CDBDoc;
class CDgnConBeamBarDlg : public CDialogMove
{
// Construction
public:
	CDgnConBeamBarDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnConBeamBarDlg)
	enum { IDD = IDD_DGN_CON_BEAM_BAR_DLG };
	CTextUnit	m_StirrupUnit;
	CEditUnit	m_StirrupCtrl;	
	MComboBox	m_StirrupNumCtrl;
	MComboBox	m_NtSize;
	MComboBox	m_NbSize;
	int		m_BOneArray;
	int		m_TOneArray;	
	CFormulaEditSpin m_edtNt;
	CFormulaEditSpin m_edtNb;
	CFormulaEditSpin m_edtNt2;
	CFormulaEditSpin m_edtNb2;
	//}}AFX_DATA

public:

	CDgnDrawWnd m_SectWnd;
	T_RCHK_K    m_RchkKey;
	int m_iMode; // 0: RC-Beam ,  1: SRC-Beam
	int m_Nb,m_Nb2,m_Nt,m_Nt2;	
	double m_DtNo,m_DbNo,m_Hc,m_Bc,m_Hf,m_Bf,m_Space;
	CString m_BeamShape,m_sNb,m_sNb2,m_sNt,m_sNt2,m_strPos;
	double m_H, m_B1, m_B2, m_Tw, m_Tf, m_Tf2;

	int m_iStirrupNum;

public:
	void Get_RebarSizeNumByString(CString strTemp, int& iNum, CString& RebarNa);
	void Write_GridItem(CString Type,CString strNt1,CString strNt2,CString strNb1,CString strNb2);
	void Initial_Data();
	void Initial_UnitData();
	void Initial_SectionData();
	void Draw_Beam();

	double Get_ChangeUnitFactor();
	double Get_SectionDimension();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConBeamBarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	bool m_bInit;

	// Generated message map functions
	//{{AFX_MSG(CDgnConBeamBarDlg)
	afx_msg void OnDgnBeamtOnearray();
	afx_msg void OnDgnBeamtTwoarray();
	afx_msg void OnDgnBeambOnearray();
	afx_msg void OnDgnBeambTwoarray();
	virtual BOOL OnInitDialog();	
	virtual void OnOK();
	afx_msg void OnChangeDgnRcNt();
	afx_msg void OnChangeDgnRcNb();
	afx_msg void OnChangeDgnRcNt2();
	afx_msg void OnChangeDgnRcNb2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONBEAMBARDLG_H__27BE1765_EAF6_11D3_888F_0000C0F30D4D__INCLUDED_)
