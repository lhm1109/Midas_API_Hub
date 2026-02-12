#if !defined(AFX_MTDMFITEMSHOWDLG_H__A428192D_0E6E_4F7F_965F_B9EACB662686__INCLUDED_)
#define AFX_MTDMFITEMSHOWDLG_H__A428192D_0E6E_4F7F_965F_B9EACB662686__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MTdmfItemShowDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMTdmfItemShowDlg dialog
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_MySRGraph.h"

#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

#include "CMTdmfItemGrid.h"

class CMTdmfItemShowDlg : public CDialogMove
{
// Construction
public:
	CMTdmfItemShowDlg(CWnd* pParent = NULL);   // standard constructor
	void InitChart();
	void SetAxisLabelText(int nComponent, LPCTSTR lbl);
	CCMTdmfItemGrid m_wndGrid;
	T_TDMT_D m_tdmt_d;
	void SetTdmt(T_TDMT_D& tdmt_d){m_tdmt_d = tdmt_d;}

	SREGraphView m_GraphView;
	CMySRGraph m_Graph;


// Dialog Data
	//{{AFX_DATA(CMTdmfItemShowDlg)
	enum { IDD = IDD_CMD_TDMT_ITEM_SHOW };
	int		m_nDataType;
	int   m_nOptionForIndia;
	BOOL  m_bIndia;
	BOOL  m_bAustralia;
	BOOL  m_bRussia;
	BOOL  m_bNewzealand;
	double	m_dLoadingAge;
	MButton	m_btnYLog;
	MButton	m_btnXLog;
	double	m_dEndLoadingAge;
	UINT	m_nSteps;
	MButton	m_btnDyGen;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMTdmfItemShowDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
		
	BOOL	GetTDMData();
	void	NewData();
	void	InitGrid();
	void	SetValue(int nIndex, int nGroup, double x,double y);
	double UnitCvt(double dReadData);
	void AlignCtrlForIndia();
	
	BOOL m_bGridInit;
	BOOL m_LogFlagX;
	BOOL m_LogFlagY;
	CArray<int, int> m_arTime;
	CArray<double, double> m_arValue;
	T_TDMF_D m_ParamData;  
	
	// Generated message map functions
	//{{AFX_MSG(CMTdmfItemShowDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnETCSPFCCHARTXLog();
	afx_msg void OnETCSPFCCHARTYLog();
	afx_msg void	OnRedraw();
	afx_msg void OnCmdTdmfDataType();
	afx_msg void OnCmdTdmfOptionForIndia();
	afx_msg void OnBtnDyGen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MTDMFITEMSHOWDLG_H__A428192D_0E6E_4F7F_965F_B9EACB662686__INCLUDED_)
