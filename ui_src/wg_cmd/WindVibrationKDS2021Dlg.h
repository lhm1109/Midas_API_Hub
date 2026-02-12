#if !defined(AFX_WINDVIBRATIONKDS2021DLG_H__INCLUDED_)
#define AFX_WINDVIBRATIONKDS2021DLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WindVibrationKDS2021Dlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "HeaderPre.h"      /////////

/////////////////////////////////////////////////////////////////////////////
// CWindVibrationKDS2021Dlg dialog

class __MY_EXT_CLASS__ CWindVibrationKDS2021Dlg : public CDialogMove
{
// Construction
public:
	CWindVibrationKDS2021Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWindVibrationKDS2021Dlg)
	enum { IDD = IDD_ETC_WIND_VIBRATION_KDS2021_DLG	};
	CEditUnit	m_edtBreadthX;
	CEditUnit	m_edtBreadthY;
	CEditUnit	m_edtFreqX;
	CEditUnit	m_edtFreqY;
	CEditUnit	m_edtFreqT;
	CEditUnit	m_edtMassT;
	CEditUnit	m_edtMassX;
	CEditUnit	m_edtMassY;
	CEditUnit	m_edtMoment;
	CEditUnit	m_edtZf;
	CEditUnit	m_edtVibration;
	CTextUnit	m_wndBreadthXUnit;
	CTextUnit	m_wndBreadthYUnit;
	CTextUnit	m_wndMassTUnit;
	CTextUnit	m_wndMassXUnit;
	CTextUnit	m_wndMassYUnit;
	CTextUnit	m_wndMoment;
	//}}AFX_DATA

// Attribute
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindVibrationKDS2021Dlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_nBuildingType;
	int m_nAcrossCheck;
	int m_nTorsionalCheck;
	int m_nResponseCheck;
	double m_dBreadthX;
	double m_dBreadthY;
	double m_dFreqX;
	double m_dFreqY;
	double m_dFreqT;
	double m_dMassT;
	double m_dMassX;
	double m_dMassY;
	double m_dMoment;
	double m_dZf;
	double m_dVibration;

protected:
	void InitialData();

	// Generated message map functions
	//{{AFX_MSG(CWindVibrationKDS2021Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdZfBtn();
	afx_msg void OnCmdImportBtn();
	afx_msg void OnCmdOk();
	afx_msg void OnCmdCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#include "HeaderPost.h"      /////////

#endif // !defined(AFX_WINDVIBRATIONKDS2021DLG_H__INCLUDED_)
