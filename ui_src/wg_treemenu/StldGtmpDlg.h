#if !defined(AFX_STLDGTMPDLG_H__C92116E4_CA5B_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_STLDGTMPDLG_H__C92116E4_CA5B_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldGtmpDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxLdgr.h"

#include "..\mit_frx\MillustViewer.h"
/////////////////////////////////////////////////////////////////////////////
// CStldGtmpDlg dialog

class CStldGtmpDlg : public CMenuBarChildDlg
{
// Construction
public:
	CStldGtmpDlg(CWnd* pParent = NULL);   // standard constructor
	~CStldGtmpDlg();
	virtual void Execute() { OnTmExecute(); }
// Dialog Data
	//{{AFX_DATA(CStldGtmpDlg)
	enum { IDD = IDD_TM_STLD_GTMP };
	CTextUnit	m_wndTzUnit;
	CTextUnit	m_wndTyUnit;
	CTextUnit	m_wndHzUnit;
	CTextUnit	m_wndHyUnit;
	CEditUnit	m_wndTzEdit;
	CEditUnit	m_wndTyEdit;
	CEditUnit	m_wndHzEdit;
	CEditUnit	m_wndHyEdit;
	CSelectLC	m_LoadCase;
	int		m_nOption;
	int		m_nElemType;
	BOOL	m_bUseSectionHz;
	BOOL	m_bUseSectionHy;
	CCobxLdgr m_wndLdgrCobx;

	mit::frx::MillustViewer m_wndTypesPicture;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldGtmpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ChangeBitmap(int nBitmap);

protected:
	CDBDoc* m_pDoc;
	CBitmap*  m_pBitmap;
	CArray<UINT, UINT> m_aDeleteCtrl;
	CArray<UINT, UINT> m_aUseSecHzCtrl;
	CArray<UINT, UINT> m_aUseSecHyCtrl;
	CArray<UINT, UINT> m_aBeamCtrl;

	// Generated message map functions
	//{{AFX_MSG(CStldGtmpDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnClickLoadCaseBtn();
	afx_msg void OnChangeOption();
	afx_msg void OnChangeElemType();
	afx_msg void OnToggleUseSection();
	afx_msg void OnTmDefineLdgrBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDGTMPDLG_H__C92116E4_CA5B_11D3_92DE_0000C0B0E6B3__INCLUDED_)
