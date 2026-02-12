#if !defined(__CM_WIND_PRESSURE_PROFILEKBC2016_DLG_H__)
#define __CM_WIND_PRESSURE_PROFILEKBC2016_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindPressureProfileKBC2016Dlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_LateralLoad.h"
#include "..\wg_base\wg_base_MySRGraph.h"

#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

#include "CMWindPressureProfileGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureProfileKBC2016Dlg dialog

#include "HeaderPre.h"      /////////

class __MY_EXT_CLASS__ CCMWindPressureProfileKBC2016Dlg : public CDialogMove
{
// Construction
public:
	CCMWindPressureProfileKBC2016Dlg(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_CMD_DEF_WIND_PROFILE_KBC2016_DLG };
	
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

	afx_msg void OnChangeDir();
	afx_msg void OnWINDPFViewCalcSheet();
	afx_msg void OnWindPFBrowse();
	afx_msg void OnBtnDyGen();

	DECLARE_MESSAGE_MAP()

protected:
	int		m_nDir;

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

#endif // !defined(AFX_WINDPROFILEKBC2016DLG_H__73923241_1CE0_11D4_AA8B_0080AD78AAC8__INCLUDED_)
