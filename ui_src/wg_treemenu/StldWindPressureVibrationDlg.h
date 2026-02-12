#if !defined(AFX_WINDPRESSUREVIBRATIONDLG_H__D88A7361_8496_11D4_A678_00010263A1CE__INCLUDED_)
#define AFX_WINDPRESSUREVIBRATIONDLG_H__D88A7361_8496_11D4_A678_00010263A1CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldWindPressureVibrationDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "wg_treemenures.h"

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureVibrationDlg dialog

class CStldWindPressureVibrationDlg : public CDialogMove
{
// Construction
public:
	CStldWindPressureVibrationDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStldWindPressureVibrationDlg)
	enum { IDD = IDD_TM_STLD_WINDP_VIBRATION_DLG };
	CEditUnit	m_edtBreadthX;
	CEditUnit	m_edtBreadthY;
	CEditUnit	m_edtFreqA;
	CEditUnit	m_edtFreqT;
	CEditUnit	m_edtZf;
	CTextUnit	m_wndBreadthXUnit;
	CTextUnit	m_wndBreadthYUnit;
	//}}AFX_DATA

// Attribute
public:
	BOOL SetWindData(T_WDPR_KBC2016* pData, int nDirection, double dAngle);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldWindPressureVibrationDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_nBuildingType;
	int m_nAcrossCheck;
	int m_nTorsionalCheck;
	double m_dBreadthX;
	double m_dBreadthY;
	double m_dFreqA;
	double m_dFreqT;
	double m_dZf;

protected:
	void InitialData();
	BOOL CheckValue();
	BOOL CheckErrorDataVar(LPCTSTR lpszData, LPCTSTR lpszVar);

	// Generated message map functions
	//{{AFX_MSG(CStldWindPressureVibrationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdZfBtn();
	afx_msg void OnCmdOk();
	afx_msg void OnCmdCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDPRESSUREVIBRATIONDLG_H__D88A7361_8496_11D4_A678_00010263A1CE__INCLUDED_)
