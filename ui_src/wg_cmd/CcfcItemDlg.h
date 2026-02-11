#if !defined(AFX_CCFCITEMDLG_H__613CB5E2_3A24_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CCFCITEMDLG_H__613CB5E2_3A24_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CcfcItemDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\wg_base_MySRGraph.h"
#include "..\MIT_frx\MButton.h"
#include "CcfcGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CCcfcItemDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCcfcItemDlg : public CDialogMove
{
// Construction
public:
	CCcfcItemDlg(CWnd* pParent = NULL);   // standard constructor
	void	AlignControls();

public:
	CCcfcGrid		m_wndGrid;
	BOOL				m_bGridInit;
	CMySRGraph			m_Graph;
	SREGraphView m_GraphView;

public:
	T_CCFC_D&	GetParamData();
	void	SetParamData(T_CCFC_D& data);
	int		SelChangeCodeCobx(CString str);
	void	InitGrid();
	void	InitChart();
	void	OnDataChanged();
	void	SetValue(int nIndex, int nGroup, double x, double y);
	void	Data2Dlg();
	void	Dlg2Data();

	void	DrawChart();
	void	GraphConstant();
	void	GraphUser();
	void	SetAxisLabelText(int nComponent, LPCTSTR lbl);

// Dialog Data
	//{{AFX_DATA(CCcfcItemDlg)
	enum { IDD = IDD_ETC_CCFC_ITEM };
	MButton	m_btnYLog;
	MButton	m_btnXLog;
	CFormulaEditSpin	m_wndScaleFactor;
	CTextUnit	m_CoefficientUnit;
	CEditUnit	m_Coefficient;
	CString		m_csFuncName;
	int		m_type;
	MButton	m_btnDyGen;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCcfcItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_CCFC_D	m_ParamData;
	BOOL			m_bModify;
	CString		m_csOldName;
	CArray<UINT, UINT> m_aUserCtrl;
	CArray<UINT, UINT> m_aConstCtrl;

	// Generated message map functions
	//{{AFX_MSG(CCcfcItemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCcfcRedrawGraph();
	afx_msg void OnSelChangeType();
	afx_msg void OnCCFCCHARTXLog();
	afx_msg void OnCCFCCHARTYLog();
	afx_msg void OnBtnDyGen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCFCITEMDLG_H__613CB5E2_3A24_11D5_880F_00010263A1F4__INCLUDED_)
