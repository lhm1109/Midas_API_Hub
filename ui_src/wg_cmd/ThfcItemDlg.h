#pragma once

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\wg_base_MySRGraph.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx
;
#include "ThfcItemGrid.h"

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CThfcItemDlg : public CDialogMove
{
public:
	CThfcItemDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_ETC_THFC_ITEM };

public:
	void SetFunctionName(CString FuncName);

	void NewData();
	double UnitCvt(double dReadData, int nUnitType = D_UNITSYS_BASE_LENGTH);
	int lex(FILE *fp, char *yytext);
	BOOL ReadTHDData(FILE *fp);
	BOOL ReadSGSData(FILE *fp);
	BOOL ReadTGSData(FILE *fp);
	BOOL ReadAT2Data(FILE* fp);
	void InitChart();
	T_THFC_D& GetParamData();
	void SetParamData(T_THFC_D& data);
	void OnDataChanged();
	void SetAxisLabelText(int nComponent, LPCTSTR lbl);
	BOOL ReadEQData(FILE *fp);
	void AutoBtnDyGen();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	void AlignControls();
	void SetSinuCtrlUnitType(int nDataType);
	BOOL CalcParamDataFromSinu(CArray<T_THFC_BASE, T_THFC_BASE&> &aThisFuncData);

	void InitGrid();
	void SetValue(int nIndex, int nGroup, double x, double y);
	void EnableGravity(BOOL bEnable);
	void ChangeDataType();
	void ChangeDataType(int nType);
	void SetDescription(CString Descript);
	void SetFFT();

	void SetEnableDygenButton();

protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnThfcImportFile();
	afx_msg void OnThfcSeismicGen();
	afx_msg void OnChangeDataType();
	afx_msg void OnThfcEQRecord();
	afx_msg void OnETCTHFCCHARTXLog();
	afx_msg void OnETCTHFCCHARTYLog();
	afx_msg void OnThfcSinRedrawGraph();
	afx_msg void OnThfcGrCheck();
	afx_msg void OnThfcDesignHeeldrop();
	afx_msg void OnApply();
	afx_msg void OnChangeScaleRdo();
	afx_msg void OnEtcThfcChartFft();
	afx_msg void OnResponseSpectrum();
	afx_msg void OnBtnDyGen();
	DECLARE_MESSAGE_MAP()

public:
	BOOL m_bSinusoidal;
	T_THFC_D m_ParamData;

private:
	MButton m_btnFFT;
	CEditUnit	m_wndGravity;
	CFormulaEdit	m_wndScaleFactor;
	MButton m_wndDrawCheck;
	CFormulaEdit	m_wndDTime;
	CFormulaEdit	m_wndTimeInc;
	CTextUnit	m_wndSinPAUnit;
	CEditUnit	m_wndSinPA;
	CEditUnit	m_wndSinD;
	CTextUnit	m_wndSinFUnit;
	CEditUnit	m_wndSinF;
	CTextUnit	m_wndSinCUnit;
	CEditUnit	m_wndSinC;
	CTextUnit	m_wndSinAUnit;
	CEditUnit	m_wndSinA;
	CTextUnit	m_wndGravityUnit;
	MButton m_btnYLog;
	MButton m_btnXLog;
	CString	m_csFuncName;
	CString	m_csDescription;
	int		m_nDataType;
	CEditUnit	m_wndScaleMaxVal;
	CTextUnit	m_wndScaleUnit;
	int   m_nScaleType;
	MButton	m_btnDyGen;
	CThfcItemGrid m_wndGrid;

private:
	double m_dSampleTime;
	CString m_strEarthquake;
	float m_fTimeScale;
	float m_fScaleFactor;
	BOOL m_bIsTimeDomain;
	BOOL m_LogFlagX;
	BOOL m_LogFlagY;
	CArray <double, double> m_arTime, m_arAccel;

	BOOL m_bSetParam;
	BOOL m_bGridInit;
	CString m_csOldName;
	SREGraphView m_GraphView;
	CMySRGraph m_Graph;
	CArray<UINT, UINT> m_aSinuCtrl;
	CArray<UINT, UINT> m_aTimeCtrl;
	CArray<UINT, UINT> m_aParamCtrl;
	CArray<UINT, UINT> m_aTypeCtrl;
};
#include "HeaderPost.h"
