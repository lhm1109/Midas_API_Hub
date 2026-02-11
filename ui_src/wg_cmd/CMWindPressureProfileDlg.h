#if !defined(__CM_WIND_PRESSURE_PROFILE_DLG_H__)
#define __CM_WIND_PRESSURE_PROFILE_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindPressureProfileDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_LateralLoad.h"
#include "..\wg_base\wg_base_MySRGraph.h"

#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

#include "CMWindPressureProfileGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureProfileDlg dialog

#include "HeaderPre.h"      /////////

class __MY_EXT_CLASS__ CCMWindPressureProfileDlg : public CDialogMove
{
// Construction
public:
	CCMWindPressureProfileDlg(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_CMD_DEF_WIND_PROFILE_DLG };
	
	void InitWindProfile( T_WDPR_D& rDataWind, double* pWindDir = NULL );
	void OnDataChanged();
	void UpdateProfileData();
protected:
	void UpdateChart();
	void InitChart();
	void InitGrid();
	//BOOL PreTranslateMessage(MSG* pMsg);
	void SetValue(int nIndex, int nGroup, double x,double y);
	void SetAxisLabelText(int nComponent, LPCTSTR lbl);
	CString ConvertValToFmtStr(double Val, int nLimit);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnWINDPFViewCalcSheet();
	afx_msg void OnWindPFBrowse();
	afx_msg void OnBtnDyGen();

	DECLARE_MESSAGE_MAP()

protected:
	BOOL    m_bGridInit;
	CString m_strFileName;

	CLateralLoad m_LateralLoad;
	T_WDPR_D*    m_pWdprD;
	
	MButton	    m_btnDyGen;
	SREGraphView    m_GraphView;
	SRGraphDisplay* m_pDisplay;
	CMySRGraph      m_Graph;
	CCMWindPressureProfileGrid m_wndGrid;

};

#include "HeaderPost.h"      /////////

#endif // !defined(AFX_WINDPROFILEDLG_H__73923241_1CE0_11D4_AA8B_0080AD78AAC8__INCLUDED_)
