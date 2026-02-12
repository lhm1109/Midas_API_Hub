#if !defined(AFX_STLDULCBDLG_H__84D2D3A4_C8C8_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_STLDULCBDLG_H__84D2D3A4_C8C8_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldUlcbDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxLdgr.h"
/////////////////////////////////////////////////////////////////////////////
// CStldUlcbDlg dialog

class CStldUlcbDlg : public CMenuBarChildDlg
{
	// Construction
public:
	CStldUlcbDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnTmExecute(); }
	// Dialog Data
	enum { IDD = IDD_TM_STLD_ULCB };

	CTextUnit	m_untLength;
	CEditUnit	m_edtLength;
	int		m_nOption;
	CCobxLdgr m_cmbLdgr;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	CDBDoc* m_pDoc;
	CArray<UINT, UINT> m_aDeleteCtrl;

	// Generated message map functions
	//{{AFX_MSG(CStldUlcbDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnChangeOption();
	afx_msg void OnTmDefineLdgrBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_STLDULCBDLG_H__84D2D3A4_C8C8_11D3_92DE_0000C0B0E6B3__INCLUDED_)
