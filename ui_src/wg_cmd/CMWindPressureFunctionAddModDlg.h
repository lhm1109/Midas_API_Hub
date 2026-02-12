#if !defined(__CM_WIND_PRESSURE_FUNCTION_ADDMOD_DLG_H__)
#define __CM_WIND_PRESSURE_FUNCTION_ADDMOD_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindPressureProfileDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_LateralLoad.h"
#include "..\wg_base\wg_base_MySRGraph.h"
#include "..\MIT_frx\MComboBox.h"

#include "CMWindPressureFunctionGrid.h"

#include "..\Grid\GXWND.h"

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureFunctionAddModDlg dialog

#include "HeaderPre.h"      /////////

using namespace mit::frx;
class CCMWindPressureFunctionHelper;
class __MY_EXT_CLASS__ CCMWindPressureFunctionAddModDlg : public CDialogMove
{
// Construction
public:
	CCMWindPressureFunctionAddModDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMWindPressureFunctionAddModDlg();

	enum { IDD = IDD_CMD_DEF_WIND_FUNCTION_ADDMOD };

	void SetWdfcKey( T_WDFC_K Key );
	void DestoryHelperDlg();
	void ApplyEquation( const CString& strApply );
	void AppendEquation( const CString& strAdd );
	BOOL IsRectCoordSys();

protected:
	void InitMinMax();
	void InitGrid();
	void UpdateGrid();
	BOOL GetStringByCoordFixedAxis(CString& csD1, CString& csD2, CString& csD3, BOOL bWithUnit, CString* pPres); //순서고려하지 않는다.
	BOOL GetStringByCoordFixedAxis(CString& csD1, CString& csD2, CString& csD3); //이건 순서 고려.
	BOOL GetValueByCoordFixedAxis(double& dFrom, double& dTo, double& dInc);

	void InitComboCoord();
	void InitComboFixAxis();
	void SetStaticCtrlNameValue();
	void Calculate(BOOL bWarning = FALSE, BOOL bInit = FALSE);
	BOOL CheckValid();

	BOOL Grid2Data();
	BOOL Dlg2Data(BOOL bWarning = FALSE, BOOL bCalc = FALSE);
	BOOL Data2Dlg();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnCmdMethodChanged();
	afx_msg void OnCmdCalculate();
	afx_msg void OnCmdCoordChanged();
	afx_msg void OnCmdFixAxisChanged();
	afx_msg void OnCmdFuncHelper();

	DECLARE_MESSAGE_MAP()

protected:
	T_WDFC_K  m_Key;
	T_WDFC_D  m_Data;
	CArray<UINT, UINT> m_aMethodRdo;
	int m_nMethod;
	MComboBox m_cbxCoord;
	MComboBox m_cbxFixAxis;
	CCMWindPressureFunctionHelper* m_pHelpDlg;

	double    m_dMinX;
	double    m_dMinY;
	double    m_dMinZ;
	double    m_dMaxX;
	double    m_dMaxY;
	double    m_dMaxZ;

	double    m_d1From;
	double    m_d1To;
	double    m_d1Inc;
	double    m_d2Fix;
	double    m_d3Fix;
	CString   m_cs1;
	CString   m_cs2;
	CString   m_cs3;

	CArray<double, double> m_caD1;
	CArray<double, double> m_caD2;
	CArray<double, double> m_caD3;
	CArray<double, double> m_caPresure;

	CCMWindPressureFunctionGrid m_GridWnd;

	//CCMWindPressureProfileGrid m_wndGrid;

};

#include "HeaderPost.h"      /////////

#endif // !defined(AFX_WINDPROFILEDLG_H__73923241_1CE0_11D4_AA8B_0080AD78AAC8__INCLUDED_)
