#if !defined(AFX_SPFCITEMDLG_H__25B94AA2_B849_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_SPFCITEMDLG_H__25B94AA2_B849_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpfcItemDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_MySRGraph.h"
#include "SpfcItemGrid.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CSpfcItemDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CSpfcItemDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CSpfcItemDlg(CWnd* pParent = NULL);   // standard constructor

public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	void SetFuncName(CString FuncName);
	void NewData();
	double UnitCvt(double dReadData);
	int lex(FILE *fp, char *yytext);
	BOOL ReadSPDData(FILE *fp);
	BOOL ReadSGSData(FILE *fp);
	void InitChart();
	T_SPFC_D& GetParamData();
	void SetParamData(T_SPFC_D& data);
	void OnDataChanged();               // MNET:2138-GSJI-20060525 ¼öÁ¤µÊ  
	void SetAxisLabelText(int nComponent, LPCTSTR lbl);
	void SetShowType(BOOL bShowReadOnly);
	void SetDescription(CString Descript);

// Dialog Data
	//{{AFX_DATA(CSpfcItemDlg)
	enum { IDD = IDD_ETC_SPFC_ITEM };
	CFormulaEdit m_wndScaleFactor;
	CEditUnit	m_wndGravity;
	CTextUnit	m_wndGravityUnit;
	MButton	m_btnYLog;
	MButton	m_btnXLog;
	CString	m_csFuncName;
	CString	m_csDescription;
	int		m_nDataType;
	CEditUnit	m_wndScaleMaxVal;
	CTextUnit	m_wndScaleUnit;
	int   m_nScaleType;
	double   m_dDamping;
	MButton  m_bAPPLY;
	MButton	m_btnDyGen;
	//}}AFX_DATA
	CSpfcItemGrid m_wndGrid;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpfcItemDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetSeismicEvaluation(BOOL bSet) { m_bSeismicEvaluation = bSet; }

// Implementation
protected:
	BOOL m_bSeismicEvaluation;

	BOOL m_bSetParam;  
	BOOL m_bTempSetParam ;   
	BOOL m_bModify4SpecCode;      // for CETC_DESIGN_SPECT  // MNET:2138-GSJI-20060601
	BOOL m_bGridInit;
	BOOL m_bShowReadOnly;
	CString m_csOldName;
	SREGraphView m_GraphView;
	CMySRGraph m_Graph;
	void InitParam();
	void InitGrid();
	void SetValue(int nIndex, int nGroup, double x,double y);
	void EnableGravity(BOOL bEnable);
	void ChangeDataType(int nType);
	void MakeControlToReadOnly();
	INT_PTR DoModalSpectDlgAndGetStr(CString& strFuncName, CString& strDesc);

	BOOL m_LogFlagX;
	BOOL m_LogFlagY;
	CArray <double,double> m_arPeriod,m_arAccel;

	T_SPFC_D m_ParamData;

	double m_dMnMxXVal[2];
	double m_dMnMxYVal[2];
	bool m_bInternal;
	T_SPFC_K m_nSpfcKey;
	// Generated message map functions
	//{{AFX_MSG(CSpfcItemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSpfcImportFile();
	afx_msg void OnSpfcSeismicGen();
	afx_msg void OnChangeDataType();
	afx_msg void OnSpfcDesignSpectrum();
	afx_msg void OnETCSPFCCHARTXLog();
	afx_msg void OnETCSPFCCHARTYLog();
	afx_msg void OnApply();
	afx_msg void OnChangeScaleRdo();
	afx_msg void OnBtnDyGen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPFCITEMDLG_H__25B94AA2_B849_11D3_92DE_0000C0B0E6B3__INCLUDED_)
