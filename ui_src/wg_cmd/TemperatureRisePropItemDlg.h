#if !defined(AFX_TEMPERATURERISEPROPITEMDLG_H__771F3F42_228C_11D5_880F_00010263A1F4__INCLUDED_)
#define  AFX_TEMPERATURERISEPROPITEMDLG_H__771F3F42_228C_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MhtpItemDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\wg_base_MySRGraph.h"

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

#include "MhtpGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CTemperatureRisePropItemDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CTemperatureRisePropItemDlg : public CDialogMove, public CDBUpdateConnector
{
	// Construction
public:
	CTemperatureRisePropItemDlg(CWnd* pParent = NULL);   // standard constructor

public:
	CMhtpGrid			m_wndGrid;
	BOOL				m_bGridInit;
	CMySRGraph			m_Graph;
	SREGraphView m_GraphView;
public:
	T_MHTP_D&	GetParamData();
	void	SetParamData(T_MHTP_D& data);
	void	InitGrid();
	void	InitChart();	
	void	SetValue(int nIndex, int nGroup, double x, double y);
	void	Data2Dlg();
	void	Dlg2Data();

	void	DrawChart();
	void	GraphCode();
	void	GraphUser();
	void	OnDataChanged();

	void	SetAxisLabelText(int nComponent, LPCTSTR lbl);
	// Dialog Data
	//{{AFX_DATA(CTemperatureRisePropItemDlg)
	enum { IDD = IDD_TEMPERATURE_RISE_PROP_ITEM };
	MButton		m_btnYLog;
	MButton		m_btnXLog;
	MComboBox	m_CobxCode;
	int			m_nType;
	CString		m_csTypeName;
	int			m_nStlType;
	double		m_dScaleFactor;
	MButton		m_btnDyGen;
	int			m_nGraphType;

	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTemperatureRisePropItemDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	//}}AFX_VIRTUAL

	// Implementation
protected:
	CDBDoc* m_pDoc;
	T_MHTP_D	m_ParamData;
	BOOL		m_bModify;
	CString		m_csOldName;
	T_UNIT_INDEX m_CurrUnit;
	CArray<UINT, UINT> m_aCodeCtrl;
	CArray<T_MHTP_BASE, T_MHTP_BASE> m_acode;
	CRect rect;

	// Generated message map functions
	//{{AFX_MSG(CTemperatureRisePropItemDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnMhtpSelChangeType();
	afx_msg void OnSelchangeMhtpCodeCombo();
	afx_msg void OnMhtpSinRedrawGraph();
	afx_msg void OnMhtpChartXLog();
	afx_msg void OnMhtpChartYLog();
	afx_msg void OnMhtpSteelTypeChk();
	afx_msg void OnBtnDyGen();
	//afx_msg void OnRdoRelaxTime() ;
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEMPERATURERISEPROPITEMDLG_H__771F3F42_228C_11D5_880F_00010263A1F4__INCLUDED_)
