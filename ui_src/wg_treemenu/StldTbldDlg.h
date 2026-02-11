#if !defined(AFX_STLDTBLDDLG_H__93E7A5A1_C805_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_STLDTBLDDLG_H__93E7A5A1_C805_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldTbldDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxLdgr.h"

#include "..\mit_frx\MillustViewer.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CStldTbldDlg dialog

class CStldTbldDlg : public CMenuBarChildDlg
{
// Construction
public:
	CStldTbldDlg(CWnd* pParent = NULL);   // standard constructor
	~CStldTbldDlg();
	virtual void Execute() { OnTmExecute(); }
// Dialog Data
	//{{AFX_DATA(CStldTbldDlg)
	enum { IDD = IDD_TM_STLD_TBLD };
	CTextUnit	m_wndSWUnit;
	CEditUnit	m_wndSWEdit;
	CEditUnit	m_wndSpanEdit;
	CEditUnit	m_wndForceEdit;
	CTextUnit	m_wndSpanUnit;
	CTextUnit	m_wndForceUnit;
	mit::frx::MComboBox	m_wndScaleFactor;
	mit::frx::MComboBox	m_wndDirection;
	CSelectLC	m_LoadCase;
	int		m_nOption;
	int		m_nProj;
	CString	m_csScale;
	CCobxLdgr m_wndLdgrCobx;
	//}}AFX_DATA
	MButton m_wndLoadType[6];
	mit::frx::MillustViewer m_wndCurLoadTypePicture;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldTbldDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void AlignControls();
	void AlignProjectionControls(const bool& bInit);
	void ChangeBitmap(int nBitmap);

protected:
	CDBDoc* m_pDoc;
	CArray<UINT, UINT> m_aLoadTypeCtrl;
	CArray<UINT, UINT> m_aLoadValueCtrl;
	CArray<UINT, UINT> m_aEtcCtrl;
	CBitmap*  m_pBitmap;
	int       m_nBitmap;
	BOOL      m_bInputMode;


	// Generated message map functions
	//{{AFX_MSG(CStldTbldDlg)
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickLoadTypeBtn();
	afx_msg void OnTmCancel();
	afx_msg void OnChangeOption();
	afx_msg void OnClickLoadCaseBtn();
	afx_msg void OnTmDefineLdgrBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDTBLDDLG_H__93E7A5A1_C805_11D3_92DE_0000C0B0E6B3__INCLUDED_)
