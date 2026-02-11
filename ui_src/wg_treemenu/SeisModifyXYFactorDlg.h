#if !defined(AFX_SEISMODIFYXYFACTORDLG_H__66BE7323_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_SEISMODIFYXYFACTORDLG_H__66BE7323_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisModifyXYFactorDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisModifyXYFactorDlg dialog

class CSeisModifyXYFactorDlg : public CMenuBarChildDlg
{
// Construction
public:
	CSeisModifyXYFactorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSeisModifyXYFactorDlg)
	enum { IDD = IDD_TM_SEIS_MODIFY_XYFACT };
	int		m_nOption;
	BOOL	m_bDoNotExceed;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisModifyXYFactorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	CEditUnit	m_Value1;
	CEditUnit	m_Value2;
	CEditUnit	m_Value3;
	
// Implementation
protected:
	CDBDoc* m_pDoc;
	T_SEME_D m_SemeD;
	void EnableValueEdit(BOOL bEnable);
	void InitCtrl();
	void SetChiGamma();

	// Generated message map functions
	//{{AFX_MSG(CSeisModifyXYFactorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnChangeOption();
	afx_msg void OnDoNotExceedChk();
	afx_msg void OnChangeEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEISMODIFYXYFACTORDLG_H__66BE7323_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
