#if !defined(AFX_HMTPITEMDLG_H__771F3F42_228C_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_HMTPITEMDLG_H__771F3F42_228C_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HmtpItemDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\wg_base_MySRGraph.h"
#include "HmtpGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CHmtpItemDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CHmtpItemDlg : public CDialogMove
{
// Construction
public:
	CHmtpItemDlg(CWnd* pParent = NULL);   // standard constructor
	void	AlignControls();

public:
	CHmtpGrid		m_wndGrid;
	BOOL				m_bGridInit;
	CMySRGraph			m_Graph;
	SREGraphView m_GraphView;

public:
	T_HMTP_D&	GetParamData();
	void	SetParamData(T_HMTP_D& data);
	int		SelChangeCodeCobx(CString str);
	void	InitGrid();
	void	InitChart();
	void	OnDataChanged();
	void	SetValue(int nIndex, int nGroup, double x, double y);
	void	ShowHideCode(BOOL flag);
	void	Data2Dlg();
	void	Dlg2Data();

	void	DrawChart();
	void	GraphConstant();
	void	GraphCode();
	void	GraphUser();
	double	OhzagiY(int nCement, double m);
	void	SetAxisLabelText(int nComponent, LPCTSTR lbl);

// Dialog Data
	//{{AFX_DATA(CHmtpItemDlg)
	enum { IDD = IDD_ETC_HMTP_ITEM };
	CButton	m_btnYLog;
	CButton	m_btnXLog;
	CTextUnit	m_dOHZCompStrengthUnit;
	CEditUnit	m_dOHZCompStrength;
	CComboBox	m_OHZCementType;
	CTextUnit	m_dKSDCompStrengthUnit;
	CEditUnit	m_dKSDCompStrength;
	CEditUnit	m_dKSDFactorB;
	CEditUnit	m_dKSDFactorA;
	CTextUnit	m_dCEBCompStrengthUnit;
	CEditUnit	m_dCEBCompStrength;
	CComboBox	m_CEBCementType;
	CTextUnit	m_dACICompStrengthUnit;
	CEditUnit	m_dACICompStrength;
	CEditUnit	m_dACIFactorB;
	CEditUnit	m_dACIFactorA;
	CTextUnit	m_SpecHeatUnit;
	CEditUnit	m_SpecHeatEdit;
	CTextUnit	m_HeatCondUnit;
	CEditUnit	m_HeatCondEdit;
	CFormulaEdit	m_wndScaleFactor;
	CComboBox	m_CobxCode;
	int		m_type;
	CString	m_csTypeName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHmtpItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_HMTP_D	m_ParamData;
	BOOL			m_bModify;
	CString		m_csOldName;
	CArray<UINT, UINT> m_aUserCtrl;
	CArray<UINT, UINT> m_aCodeCtrl;
	CArray<UINT, UINT> m_aKSDCodeCtrl;
	CArray<UINT, UINT> m_aACICodeCtrl;
	CArray<UINT, UINT> m_aCEBCodeCtrl;
	CArray<UINT, UINT> m_aOHZCodeCtrl;

	// Generated message map functions
	//{{AFX_MSG(CHmtpItemDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnHmtpSelChangeType();
	afx_msg void OnSelchangeHmtpCodeCombo();
	afx_msg void OnHmtpSinRedrawGraph();
	afx_msg void OnETCHMTPCHARTXLog();
	afx_msg void OnETCHMTPCHARTYLog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMTPITEMDLG_H__771F3F42_228C_11D5_880F_00010263A1F4__INCLUDED_)
