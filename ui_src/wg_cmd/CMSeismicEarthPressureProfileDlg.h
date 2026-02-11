#if !defined(__CM_SEISMIC_EARTH_PRESSURE_PROFILE_DLG_H__)
#define __CM_SEISMIC_EARTH_PRESSURE_PROFILE_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSeismicEarthPressureProfileDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_LateralLoad.h"
#include "..\wg_base\wg_base_MySRGraph.h"
#include "CMSeismicEarthPressureProfileGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSeismicEarthPressureProfileDlg dialog

#include "HeaderPre.h"      /////////

class __MY_EXT_CLASS__ CCMSeismicEarthPressureProfileDlg : public CDialogMove
{
	// Construction
public:
	CCMSeismicEarthPressureProfileDlg(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_CMD_DEF_EARTHP_SEISMIC_PROFILE_DLG };

	void InitProfile(T_EPSE_D& rData, BOOL bInitData=TRUE);
	void OnDataChanged();
	void UpdateProfileData();
protected:
	void UpdateChart();
	void InitChart();
	void InitGrid();
	//BOOL PreTranslateMessage(MSG* pMsg);
	void SetValue(int nIndex, int nGroup, double x, double y);
	void SetAxisLabelText(int nComponent, LPCTSTR lbl);
	void ChangeLabel(int nSelType);
	void AddTextPannelComponent();
// 	CString ConvertValToFmtStr(double Val, int nLimit);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnPFViewCalcSheet();
	afx_msg void OnChangeProfile();
	afx_msg void OnPFBrowse();

	DECLARE_MESSAGE_MAP()

protected:
	BOOL    m_bGridInit;
	CString m_strFileName;

	CLateralLoad m_LateralLoad;
	T_EPSE_D*    m_pEpseD;
	int m_nMethodType;
	double m_dFootingLevel;
	SREGraphView    m_GraphView;
	SRGraphDisplay* m_pDisplay;
	CMySRGraph      m_Graph;
	CCMSeismicEarthPressureProfileGrid m_wndGrid;
	CArray<UINT, UINT> m_CtrlProfileOption;
	CArray<UINT, UINT> m_CtrlProfileEqui;
};

#include "HeaderPost.h"      /////////

#endif // !defined(__CM_SEISMIC_EARTH_PRESSURE_PROFILE_DLG_H__)
