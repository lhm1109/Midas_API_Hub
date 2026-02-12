#if !defined(AFX_STLDFMLDDLG_H__E009E119_C07E_4647_8691_AE18AA9236E6__INCLUDED_)
#define AFX_STLDFMLDDLG_H__E009E119_C07E_4647_8691_AE18AA9236E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldFmldDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxLdgr.h"

#include "..\mit_frx\MillustViewer.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CStldFmldDlg dialog

class CStldFmldDlg : public CMenuBarChildDlg
{
// Construction
public:
	CStldFmldDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStldFmldDlg();
	virtual void Execute() { OnTmExecute(); }
// Dialog Data
	//{{AFX_DATA(CStldFmldDlg)
	enum { IDD = IDD_TM_STLD_FMLD };
	mit::frx::MComboBox	m_wndDirection;
	CTextUnit	m_wndDensityUnit;
	CTextUnit	m_wndThickUnit;
	CEditUnit	m_wndDensity;
	CEditUnit	m_wndThick;
	CCobxLdgr	m_wndLoadGroup;
	CSelectLC m_wndLoadCase;
	int		m_nOption;
	int		m_nCoverType;	
	CString	m_wndScaleFactor;
	//}}AFX_DATA
	BOOL	m_bRangeF[4];
	BOOL	m_bRangeH[4];
	
	mit::frx::MillustViewer m_wndRangePicture;
	mit::frx::MillustViewer m_wndTypePicture;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldFmldDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	CBitmap*  m_pBitmap;
	void ChangeBitmap(int nBitmap);

protected:
	CArray<UINT, UINT> m_aOptGrpCtrl;

	// Generated message map functions
	//{{AFX_MSG(CStldFmldDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnChangeOption();
	afx_msg void OnLoadCaseBtn();
	afx_msg void OnLoadGroupBtn();
	afx_msg void OnCoverType();
 	afx_msg void OnCmdRangeFullChk();
	afx_msg void OnCmdRangeHalfChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDFMLDDLG_H__E009E119_C07E_4647_8691_AE18AA9236E6__INCLUDED_)
