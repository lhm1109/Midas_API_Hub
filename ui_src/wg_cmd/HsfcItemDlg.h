#if !defined(AFX_HSFCITEMDLG_H__4B6310A1_241E_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_HSFCITEMDLG_H__4B6310A1_241E_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HsfcItemDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\wg_base_MySRGraph.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

#include "HsfcGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CHsfcItemDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CHsfcItemDlg : public CDialogMove
{
// Construction
public:
	CHsfcItemDlg(CWnd* pParent = NULL);   // standard constructor
	void AlignControls();

public:
	CHsfcGrid		m_wndGrid;
	BOOL				m_bGridInit;
	CMySRGraph			m_Graph;
	SREGraphView m_GraphView;

public:
	T_HSFC_D&	GetParamData();
	void	SetParamData(T_HSFC_D& data);
	void	InitGrid();
	void	InitChart();
	void	OnDataChanged();
	void	SetValue(int nIndex, int nGroup, double x, double y);

	void	DrawChart();
	void	GraphConstant();
	void	GraphCode();
	void	GraphUser();
	void	SetAxisLabelText(int nComponent, LPCTSTR lbl);
	BOOL  Calcu();

// Dialog Data
	//{{AFX_DATA(CHsfcItemDlg)
	enum { IDD = IDD_ETC_HSFC_ITEM };
	CEditUnit	m_editCementContent;
	MButton	m_btnUseConc;
	CComboBox	m_ctrlCobTemperature;
	CComboBox	m_ctrlCobCement;
	MButton	m_btnYLog;
	MButton	m_btnXLog;
	CTextUnit	m_temperunit;
	CFormulaEditSpin	m_wndScaleFactor;
	CEditUnit	m_reactive;
	CTextUnit	m_maxtemperUnit;
	CEditUnit	m_maxtemper;
	CEditUnit	m_temper;
	int		m_type;
	CString	m_csFuncName;
	int		m_wndDataType;
	MButton	m_btnDyGen;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHsfcItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_HSFC_D	m_ParamData;
	BOOL			m_bModify;
	CString		m_csOldName;
	CArray<UINT, UINT> m_aConstCtrl;
	CArray<UINT, UINT> m_aUserCtrl;
	CArray<UINT, UINT> m_aCodeCtrl;
	CArray<UINT, UINT> m_aCtrlDataType;
	CDBDoc* m_pDoc;

	void InitComboBox();

	// Generated message map functions
	//{{AFX_MSG(CHsfcItemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHsfcSelChangeType();
	virtual void OnOK();
	afx_msg void OnHsfcSinRedrawGraph();
	afx_msg void OnETCHSFCCHARTXLog();
	afx_msg void OnETCHSFCCHARTYLog();
	afx_msg void OnCheckUseConc();
	afx_msg void OnSelchangeCobCementType();
	afx_msg void OnSelchangeCobTemperatureType();
	afx_msg void OnChangeEditCementContent();
	afx_msg void OnUpdateEditCementContent();
	afx_msg void OnSetfocusCobCementType();
	afx_msg void OnHsfcDataTypeRdo();
	afx_msg void OnBtnDyGen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HSFCITEMDLG_H__4B6310A1_241E_11D5_880F_00010263A1F4__INCLUDED_)
