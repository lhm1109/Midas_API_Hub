#pragma once
// SpfcItemDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_MySRGraph.h"

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

#include "FlueItemGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CFlueItemDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CFlueItemDlg : public CDialogMove
{
// Construction
public:
	CFlueItemDlg(T_FLUE_K FlueK = 0,CWnd* pParent = NULL);   // standard constructor

public:
	void SetFuncName(CString FuncName);
	void NewData();
	void InitChart();
	T_FLUE_D& GetParamData();
	void SetParamData(T_FLUE_D& data);
	void SetAxisLabelText(int nComponent, LPCTSTR lbl);
	void DrawGraphView();
// Dialog Data
	//{{AFX_DATA(CFlueItemDlg)
	enum { IDD = IDD_ETC_FLUE_ITEM };
private:
	mit::frx::MButton	m_btnYLog;
	mit::frx::MButton	m_btnXLog;
	CString	m_csFuncName;
	CString m_csOldName;
	MComboBox	m_wndNameType;
	//CEditUnit	m_wndScaleMaxVal;
	//CTextUnit	m_wndScaleUnit;
	//int   m_nScaleType;
	double		m_dTime;
	double		m_dScale;
	//CButton  m_bAPPLY;
	MButton	m_btnDyGen;
	int		m_nType;//0 Code 1 user
	int		m_nSubType;//0 fibre;1 Hydrocarbon
	CFlueItemGrid m_wndGrid;
	CArray<UINT,UINT> m_aCodeContorl;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlueItemDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

//public:
	//void SetSeismicEvaluation(BOOL bSet) { m_bSeismicEvaluation = bSet; }

// Implementation
protected:
	//BOOL m_bSeismicEvaluation;

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
	T_FLUE_D m_ParamData;

	double m_dMnMxXVal[2];
	double m_dMnMxYVal[2];

	// Generated message map functions
	//{{AFX_MSG(CFlueItemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnETCFLUECHARTXLog();
	afx_msg void OnETCFLUECHARTYLog();

	afx_msg void OnApply();
	afx_msg void OnChangeScaleRdo();
	afx_msg void OnBtnDyGen();
	afx_msg void OnDrawBtn();
	afx_msg void OnChangeType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL Dlg2Data();
	void OnFlueDesignCode();
	
};
#include "HeaderPost.h"

