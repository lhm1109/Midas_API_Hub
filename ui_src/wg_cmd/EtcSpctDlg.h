#if !defined(__ETCSPCTDLG_H__)
#define __ETCSPCTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EtcSpctDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CEtcSpctDlg dialog

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CEtcSpctDlg : public CDialogMove
{
// Construction
public:
	CEtcSpctDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEtcSpctDlg)
	enum { IDD = IDD_ETC_SPCT_DLG };	
	CButton	m_chkResultSign;
	CEditUnit	m_wndDampEdit;
	int		m_nSignType;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEtcSpctDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc *m_pDoc;
	CArray<UINT, UINT> m_aCtrlRadio;
	CArray<UINT, UINT> m_aCtrlDamping;
	CArray<UINT, UINT> m_arCtrlSignType;  
		
	// Generated message map functions
	//{{AFX_MSG(CEtcSpctDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickRadio();
	virtual void OnOK();
	afx_msg void OnCmdSpecTypeResultsign();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__ETCSPCTDLG_H__)
