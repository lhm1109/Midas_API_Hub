#if !defined(AFX_CMElsPlsConcDmgGraphDlg_H__A428192D_0E6E_4F7F_965F_B9EACB662686__INCLUDED_)
#define AFX_CMElsPlsConcDmgGraphDlg_H__A428192D_0E6E_4F7F_965F_B9EACB662686__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMElsPlsConcDmgGraphDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsConcDmgGraphDlg dialog
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_MySRGraph.h"

#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

#include "CMElsPlsConcDmgGraphGrid.h"

#include "..\MIT_frx\MButton.h"	
using namespace mit::frx;

class CCMElsPlsConcDmgGraphDlg : public CDialogMove
{
// Construction
public:
	CCMElsPlsConcDmgGraphDlg(int nType, T_EPMT_K EpmtK, T_EPMT_CONCDMG& Data, CWnd* pParent = NULL);   // standard constructor
	void InitChart();
	void SetAxisLabelText(int nComponent, LPCTSTR lbl);
	CCMElsPlsConcDmgGraphGrid m_Grid;
	T_TDMT_D m_tdmt_d;
	void SetTdmt(T_TDMT_D& tdmt_d){m_tdmt_d = tdmt_d;}
	void RedrawGraph();

	SREGraphView m_GraphView;
	CMySRGraph m_Graph;
	T_EPMT_CONCDMG m_Data;  

// Dialog Data
	//{{AFX_DATA(CCMElsPlsConcDmgGraphDlg)
	enum { IDD = IDD_CMD_PROP_ELS_PLS_CONCDMG_GRAPH_DLG };
	int		m_nDrawingType;
	MButton	m_btnYLog;
	MButton	m_btnXLog;
	MButton	m_btnDyGen;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMElsPlsConcDmgGraphDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
		
	void	NewData();
	void	InitGrid();
	void	SetValue(int nIndex, int nGroup, double x,double y);
	double UnitCvt(double dReadData);
	BOOL Dlg2Data();  
	void Data2Dlg();
	BOOL Grid2Data();
	void Data2Grid();

	int m_nType; // 0:Compressive Behavior 1:Tensile Behavior 
	BOOL m_bGridInit;
	BOOL m_LogFlagX;
	BOOL m_LogFlagY;
	T_EPMT_K m_EpmtK;
	CArray<int, int> m_arTime;
	CArray<double, double> m_arValue;
	
	
	// Generated message map functions
	//{{AFX_MSG(CCMElsPlsConcDmgGraphDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnXLog();
	afx_msg void OnYLog();
	afx_msg void	OnRedraw();
	afx_msg void OnDrawingData();
	afx_msg void OnBtnDyGen();
	afx_msg void OnOK();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	



};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMElsPlsConcDmgGraphDlg_H__A428192D_0E6E_4F7F_965F_B9EACB662686__INCLUDED_)
