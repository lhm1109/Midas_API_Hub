#if !defined(AFX_WINDPROFILEKBC2015DLG_H__73923241_1CE0_11D4_AA8B_0080AD78AAC8__INCLUDED_)
#define AFX_WINDPROFILEKBC2015DLG_H__73923241_1CE0_11D4_AA8B_0080AD78AAC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WindProfileKBC2015Dlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_LateralLoad.h"
#include "..\wg_base\wg_base_MySRGraph.h"

#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CWindProfileKBC2015Dlg dialog

class CWindProfileKBC2015Dlg : public CDialogMove
{
// Construction
public:
	CWindProfileKBC2015Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWindProfileKBC2015Dlg)
	enum { IDD = IDD_CMD_WINDPF_KBC2015 };
	int		m_nDir;
	int		m_nComponent;
	int		m_nProfileType;
	CString	m_strFileName;
	MButton	m_btnDyGen;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindProfileKBC2015Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWindProfileKBC2015Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeDir();
	afx_msg void OnChangeCompont();
	afx_msg void OnChangeProfileType();
	afx_msg void OnWINDPFViewCalcSheet();
	afx_msg void OnWindPFBrowse();
	afx_msg void OnBtnDyGen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


///////////////////////////////////

public:
	void InitWindProfile(T_WIND_K nKey, T_WIND_D& rDataWind, T_KEY KeyWAorWT, T_WNAT_D* pWnatD=NULL);

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
	T_KEY m_KeyWAorWT;
	T_WNAT_D* m_pWnatD;
		
	CArray<UINT, UINT> m_aCtrlProfileSub;
	CArray<UINT, UINT> m_aCtrlProfileSub2;

private:
	void UpdateChart();
	void UpdateGrid();

    BOOL IsEnableAcross(const T_WIND_D* pWindD);
    BOOL IsEnableTorsional(const T_WIND_D* pWindD);
    T_KEY GetKeyWA(const T_WIND_D* pWindD);

    BOOL IsHideColPresssure(int nDir, const T_WIND_D* pWindD);    
    BOOL IsHideColMaxDispAccl(int nDir, const T_WIND_D* pWindD);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDPROFILEKBC2015DLG_H__73923241_1CE0_11D4_AA8B_0080AD78AAC8__INCLUDED_)
