#if !defined(__CM_STATIC_EARTH_PRESSURE_PROFILE_DLG_H__)
#define __CM_STATIC_EARTH_PRESSURE_PROFILE_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMStaticEarthPressureProfileDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_LateralLoad.h"
#include "..\wg_base\wg_base_MySRGraph.h"
#include "CMStaticEarthPressureProfileGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CCMStaticEarthPressureProfileDlg dialog

#include "HeaderPre.h"      /////////

class __MY_EXT_CLASS__ CCMStaticEarthPressureProfileDlg : public CDialogMove
{
	// Construction
public:
	CCMStaticEarthPressureProfileDlg(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_CMD_DEF_EARTHP_STATIC_PROFILE_DLG };

	void InitProfile(T_EPST_D& rData, BOOL bInitData, double* pDir = NULL);
	void UpdateProfileData();
protected:
	void UpdateChart();
	void InitChart();
	void InitGrid();
	//BOOL PreTranslateMessage(MSG* pMsg);
	void SetValue(int nIndex, int nGroup, double x, double y);
	void SetAxisLabelText(int nComponent, LPCTSTR lbl);
	CString ConvertValToFmtStr(double Val, int nLimit);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnPFViewCalcSheet();
	afx_msg void OnPFBrowse();

	DECLARE_MESSAGE_MAP()

protected:
	BOOL    m_bGridInit;
	CString m_strFileName;

	CLateralLoad m_LateralLoad;
	T_EPST_D*    m_pEpstD;

	SREGraphView    m_GraphView;
	SRGraphDisplay* m_pDisplay;
	CMySRGraph      m_Graph;
	CCMStaticEarthPressureProfileGrid m_wndGrid;

};

#include "HeaderPost.h"      /////////

#endif // !defined(AFX_STATIC_EARTHPROFILEDLG_H__73923241_1CE0_11D4_AA8B_0080AD78AAC8__INCLUDED_)
