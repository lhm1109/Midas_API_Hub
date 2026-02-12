#pragma once
// SpfcItemDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_MySRGraph.h"

#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

#include "FlueItemGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CSteelTimeTempItemDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CSteelTimeTempItemDlg : public CDialogMove
{
// Construction
public:
	CSteelTimeTempItemDlg(T_STTF_K sttfK = 0, CWnd* pParent = NULL);   // standard constructor

public:
	void SetFuncName(CString FuncName);
	void NewData();
	void InitChart();
	T_STTF_D& GetParamData();
	
	void SetAxisLabelText(int nComponent, LPCTSTR lbl);
	void DrawGraphView();
// Dialog Data
	//{{AFX_DATA(CSteelTimeTempItemDlg)
	enum { IDD = IDD_ETC_STTF_ITEM };
private:
	MButton	m_btnYLog;
	MButton	m_btnXLog;
	CString	m_csFuncName;
	CString m_csOldName;
	double		m_dScale;
	MButton	m_btnDyGen;
	CFlueItemGrid m_wndGrid;
	CArray<UINT,UINT> m_aCodeContorl;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSteelTimeTempItemDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:
	void SetParamData(T_STTF_D& data);
	BOOL m_bSetParam;  
	//BOOL m_bTempSetParam ;   
	BOOL m_bGridInit;
	SREGraphView m_GraphView;
	CMySRGraph m_Graph;
	void InitGrid();
	void SetValue(int nIndex, int nGroup, double x,double y);
	//INT_PTR DoModalSpectDlgAndGetStr(CString& strFuncName, CString& strDesc);

	BOOL m_LogFlagX;
	BOOL m_LogFlagY;
	CArray <double,double> m_arPeriod,m_arAccel;
	CArray<T_FLUE_BASE, T_FLUE_BASE&> m_arUserData;
	CArray<T_FLUE_BASE, T_FLUE_BASE&> m_arCodeData;
	T_STTF_D m_ParamData;

	double m_dMnMxXVal[2];
	double m_dMnMxYVal[2];

	// Generated message map functions
	//{{AFX_MSG(CSteelTimeTempItemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnETCFLUECHARTXLog();
	afx_msg void OnETCFLUECHARTYLog();

	afx_msg void OnApply();
	afx_msg void OnChangeScaleRdo();
	afx_msg void OnBtnDyGen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL Dlg2Data();
	void OnFlueDesignCode();
	
};
#include "HeaderPost.h"

