#if !defined(AFX_BNDRCLDRDLG_H__)
#define AFX_BNDRCLDRDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrCldrDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\MIT_frx\MillustViewer.h"

/////////////////////////////////////////////////////////////////////////////
// CBndrCldrDlg dialog

class CBndrCldrDlg : public CMenuBarChildDlg
{
// Construction
public:
	CBndrCldrDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBndrCldrDlg();
	virtual void Execute();
	virtual void PostNcDestroy() { m_wndPicture.CloseDialog(); CMenuBarChildDlg::PostNcDestroy(); }
	virtual BOOL ExternalInit(UINT key);
											 //(T_BNGR_K *)

public:
	void ChangeLocalDir();

// Dialog Data
	//{{AFX_DATA(CBndrCldrDlg)
	enum { IDD = IDD_TM_BNDR_CLDR };
	
	int m_nOption  ;
	int m_nLocalDir;
	//}}AFX_DATA

	CArray<UINT, UINT> m_aCtrlOption;
	CArray<UINT, UINT> m_aCtrlLocalDir;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrCldrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ChangeBitmap();

	//CBitmap* m_pBitmap;
	mit::frx::MillustViewer m_wndPicture;
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CBndrCldrDlg)
	virtual BOOL OnInitDialog    ();
	afx_msg void OnTmExecute     ();
	afx_msg void OnTmClose       ();
	afx_msg void OnChangeOption  ();
	afx_msg void OnChangeLocalDir();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BNDRCLDRDLG_H__)
