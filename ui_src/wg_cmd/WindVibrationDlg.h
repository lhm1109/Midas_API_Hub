#if !defined(AFX_WINDVIBRATIONDLG_H__D88A7361_8496_11D4_A678_00010263A1CE__INCLUDED_)
#define AFX_WINDVIBRATIONDLG_H__D88A7361_8496_11D4_A678_00010263A1CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WindVibrationDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

#include "HeaderPre.h"      /////////

/////////////////////////////////////////////////////////////////////////////
// CWindVibrationZfDlg dialog
class __MY_EXT_CLASS__ CWindVibrationZfDlg : public CDialogMove
{
	// Construction
public:
	CWindVibrationZfDlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CWindVibrationDlg)
	enum { IDD = IDD_ETC_WIND_VIBRATION_ZF_DLG };

	MillustViewer m_wndPicture;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	// Generated message map functions
	//{{AFX_MSG(CWindVibrationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CWindVibrationDlg dialog

class __MY_EXT_CLASS__ CWindVibrationDlg : public CDialogMove
{
// Construction
public:
	CWindVibrationDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWindVibrationDlg)
	enum { IDD = IDD_ETC_WIND_VIBRATION_DLG };
	CEditUnit	m_edtBreadthX;
	CEditUnit	m_edtBreadthY;
	CEditUnit	m_edtFreqX;
	CEditUnit	m_edtFreqY;
	CEditUnit	m_edtFreqT;
	CEditUnit	m_edtMassX;
	CEditUnit	m_edtMassY;
	CEditUnit	m_edtMoment;
	CEditUnit	m_edtZf;
	CTextUnit	m_wndBreadthXUnit;
	CTextUnit	m_wndBreadthYUnit;
	CTextUnit	m_wndMassXUnit;
	CTextUnit	m_wndMassYUnit;
	CTextUnit	m_wndMoment;
	//}}AFX_DATA

// Attribute
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindVibrationDlg)
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
	double m_dMassX;
	double m_dMassY;
	double m_dMoment;
	double m_dZf;

protected:
	void InitialData();

	// Generated message map functions
	//{{AFX_MSG(CWindVibrationDlg)
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

#endif // !defined(AFX_WINDVIBRATIONDLG_H__D88A7361_8496_11D4_A678_00010263A1CE__INCLUDED_)
