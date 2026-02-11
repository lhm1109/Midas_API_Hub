#if !defined(AFX_STLDPRSTDLG_H__84D2D3A3_C8C8_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_STLDPRSTDLG_H__84D2D3A3_C8C8_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldPrstDlg.h : header file
//

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
// CStldPrstDlg dialog

class CStldPrstDlg : public CMenuBarChildDlg
{
// Construction
public:
	CStldPrstDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnTmExecute(); }
// Dialog Data
	//{{AFX_DATA(CStldPrstDlg)
	enum { IDD = IDD_TM_STLD_PRST };
	CMouseEdit m_wndLineEdit;
	CTextUnit	m_wndDMUnit;
	CTextUnit	m_wndDJUnit;
	CTextUnit	m_wndDIUnit;
	CTextUnit	m_wndTensionUnit;
	CEditUnit	m_wndDM;
	CEditUnit	m_wndDJ;
	CEditUnit	m_wndDI;
	CEditUnit	m_wndTension;
	CSelectLC	m_LoadCase;
	int		m_nOption;
	CCobxLdgr m_wndLdgrCobx;
	mit::frx::MComboBox m_cmbDirect;
	mit::frx::MillustViewer m_wndPicture;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldPrstDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	CArray<UINT, UINT> m_aDeleteCtrl;
	//CArray<UINT, UINT> m_aLoadTypeCtrl;
	CArray<UINT, UINT> m_aSelTypeCtrl;

	// Generated message map functions
	//{{AFX_MSG(CStldPrstDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnClickLoadCaseBtn();
	afx_msg void OnChangeOption();
	afx_msg void OnChangeSelType();
	afx_msg void OnTmDefineLdgrBtn();
	//}}AFX_MSG
	afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDPRSTDLG_H__84D2D3A3_C8C8_11D3_92DE_0000C0B0E6B3__INCLUDED_)
