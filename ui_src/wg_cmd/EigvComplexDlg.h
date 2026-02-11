#if !defined(AFX_EIGVCOMPLEXDLG_H__INCLUDED_)
#define AFX_EIGVCOMPLEXDLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThisItemDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CEigvComplexDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CEigvComplexDlg : public CDialogMove
{
// Construction
public:
	CEigvComplexDlg(CWnd* pParent = NULL);   // standard constructor
	~CEigvComplexDlg();

//-----------------------------------------------------------------------
// Interfaces
//-----------------------------------------------------------------------
public:
	void SetDlgData(T_EIGV_COMP &data);
	BOOL GetDlgData(T_EIGV_COMP &data);
//-----------------------------------------------------------------------
// Implementations
//-----------------------------------------------------------------------
protected:
	BOOL Dlg2Data();
	BOOL Data2Dlg();  
	void AutoCalcCoef();
	void CtrlManager();
protected:
	CDBDoc*   m_pDoc;
	T_EIGV_COMP m_Data;

	// CalcDampingDlg의 변수를   
	int		m_nFreqPeriodRdoDlg;
	double	m_dFrequencyDlg;
	double	m_dPeriodDlg;
	BOOL	m_bCancel;
	int		m_nCoefDefineRdo;
	int		m_nFreqPeriodRdo;

	CArray<UINT, UINT> m_aCtrlAttribute; 
// Dialog Data
	//{{AFX_DATA(CEigvComplexDlg)
	enum { IDD = IDD_ETC_EIGV_COMPLEX };

	CButton   m_wndShowBtn;
	CButton	  m_wndMassProp;
	CButton	  m_wndStiffProp;
	CEditUnit	m_wndDirectRm;
	CEditUnit	m_wndDirectRk;
	CEditUnit	m_wndCalcRm;
	CEditUnit	m_wndCalcRk;
	CEditUnit	m_wndF1;
	CEditUnit	m_wndF2;
	CEditUnit	m_wndT1;
	CEditUnit	m_wndT2;
	CEditUnit	m_wndX1;
	CEditUnit	m_wndX2;

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEigvComplexDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEigvComplexDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK(); 
	virtual void OnCancel();
	afx_msg void OnChangeCoefDefine();
	afx_msg void OnChangeFreqPeriod();
	afx_msg void OnShowDampingBtn();
	afx_msg void OnMassStiffChk();
	afx_msg void OnCallAutoCalcCoef();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //