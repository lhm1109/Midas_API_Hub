#if !defined(__ETCPZEFDLG_H__)
#define __ETCPZEFDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EtcPzefDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CEtcPzefDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CEtcPzefDlg : public CDialogMove
{
// Construction
public:
	CEtcPzefDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEtcPzefDlg)
	enum { IDD = IDD_ETC_PZEF_DLG };
	CEditUnit	m_wndOffsetFactor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEtcPzefDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void Data2DlgVar();
	void DlgVar2Data();

protected:
	CDBDoc* m_pDoc;
	T_PZEF_D m_Data;
	CArray<UINT, UINT> m_aCtrlCalc;
	CArray<UINT, UINT> m_aCtrlOffType;
	CArray<UINT, UINT> m_aCtrlCalcSub;

	// Generated message map functions
	//{{AFX_MSG(CEtcPzefDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeAutoCalc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__ETCPZEFDLG_H__)
