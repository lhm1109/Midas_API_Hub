#if !defined(AFX_ETFCITEMDLG_H__BAC17D62_2356_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_ETFCITEMDLG_H__BAC17D62_2356_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EtfcItemDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\wg_base_MySRGraph.h"
#include "..\MIT_frx\MButton.h"
#include "EtfcGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CEtfcItemDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CEtfcItemDlg : public CDialogMove
{
// Construction
public:
	CEtfcItemDlg(CWnd* pParent = NULL);   // standard constructor
	void AlignControls();

public:
	CEtfcGrid		m_wndGrid;
	BOOL				m_bGridInit;
	CMySRGraph			m_Graph;
	SREGraphView m_GraphView;

public:
	T_ETFC_D&	GetParamData();
	void	SetParamData(T_ETFC_D& data);
	void	InitGrid();
	void	InitChart();
	void	OnDataChanged();
	void	SetValue(int nIndex, int nGroup, double x, double y);

	void	DrawChart();
	void	GraphConstant();
	void	GraphCode();
	void	GraphUser();
	void	SetAxisLabelText(int nComponent, LPCTSTR lbl);

// Dialog Data
	//{{AFX_DATA(CEtfcItemDlg)
	enum { IDD = IDD_ETC_ETFC_ITEM };
	MButton	m_btnYLog;
	MButton	m_btnXLog;
	CTextUnit	m_meantemperUnit;
	CEditUnit	m_meantemper;
	CTextUnit	m_delayUnit;
	CEditUnit	m_delay;
	CTextUnit	m_maxtemperUnit;
	CEditUnit	m_maxtemper;
	CTextUnit	m_temperUnit;
	CEditUnit	m_temper;
	CFormulaEditSpin m_wndScaleFactor;
	int		m_type;
	CString	m_csFuncName;
	MButton	m_btnDyGen;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEtfcItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_ETFC_D	m_ParamData;
	BOOL			m_bModify;
	CString		m_csOldName;
	CArray<UINT, UINT> m_aConstCtrl;
	CArray<UINT, UINT> m_aSineCtrl;
	CArray<UINT, UINT> m_aUserCtrl;

	// Generated message map functions
	//{{AFX_MSG(CEtfcItemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnEtfcSelChangeType();
	virtual void OnOK();
	afx_msg void OnEtfcSinRedrawGraph();
	afx_msg void OnETFCCHARTXLog();
	afx_msg void OnETFCCHARTYLog();
	afx_msg void OnBtnDyGen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETFCITEMDLG_H__BAC17D62_2356_11D5_880F_00010263A1F4__INCLUDED_)
