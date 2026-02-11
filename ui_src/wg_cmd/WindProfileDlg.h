#if !defined(AFX_WINDPROFILEDLG_H__73923241_1CE0_11D4_AA8B_0080AD78AAC8__INCLUDED_)
#define AFX_WINDPROFILEDLG_H__73923241_1CE0_11D4_AA8B_0080AD78AAC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WindProfileDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_LateralLoad.h"
#include "..\wg_base\wg_base_MySRGraph.h"

#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CWindProfileDlg dialog

class CWindProfileDlg : public CDialogMove
{
// Construction
public:
	CWindProfileDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWindProfileDlg)
	enum { IDD = IDD_CMD_WINDPF };
	int		m_nComponent;
	int		m_nProfileType;
	CString	m_strFileName;
	MButton	m_btnDyGen;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindProfileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWindProfileDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeCompont();
	afx_msg void OnChangeProfileType();
	afx_msg void OnWINDPFViewCalcSheet();
	afx_msg void OnWindPFBrowse();
	afx_msg void OnBtnDyGen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()



///////////////////////////////////

public:
	void InitWindProfile(T_WIND_K nKey, T_WIND_D& rDataWind, T_WNAT_D* pWnatD=NULL);

protected:
	BOOL PreTranslateMessage(MSG* pMsg) ;

	CLateralLoad m_LateralLoad;
	void InitChart();
	void SetAxisLabelText(int nComponent, LPCTSTR lbl);
	CGXGridWnd m_WindPfGrid;
	SREGraphView m_GraphView;
	CMySRGraph m_Graph;
	void InitGrid(int nCode);
	void SetValue(int nIndex, int nGroup, double x,double y);
	CString ConvertValToFmtStr(double Val,int nLimit);

	BOOL m_LogFlagX;
	BOOL m_LogFlagY;
	T_WIND_D* m_pDataWind;
	T_WIND_K m_Key;
	T_WNAT_D* m_pWnatD;

private:
	void UpdateChart();
	void UpdateGrid();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDPROFILEDLG_H__73923241_1CE0_11D4_AA8B_0080AD78AAC8__INCLUDED_)
