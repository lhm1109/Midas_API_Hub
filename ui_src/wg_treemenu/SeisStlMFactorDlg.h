#if !defined(AFX_SEISSTLMFACTORDLG_H__66BE7323_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_SEISSTLMFACTORDLG_H__66BE7323_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000 
// SeisStlMFactorDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisStlMFactorDlg dialog

class CSeisStlMFactorDlg : public CMenuBarChildDlg
{
// Construction
public:
	CSeisStlMFactorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();

// Dialog Data
	//{{AFX_DATA(CSeisStlMFactorDlg)
	enum { IDD = IDD_TM_SEIS_STL_MFACT };
	int		m_nOption;
	int		m_nMembType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisStlMFactorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	CBCGPStatic		m_Title1;
	CBCGPStatic		m_Title2;
	CBCGPStatic		m_Title3;
	CBCGPStatic   m_Text2[3];
	CBCGPStatic   m_Text3[3];
	CEditUnit	m_Value1[3];
	CEditUnit	m_Value2[3];
	CEditUnit	m_Value3[3];

// Implementation
protected:
	CDBDoc* m_pDoc;
	void EnableValueEdit(BOOL bEnable);
	void ChangeType(int nMembType);
	void InitCtrl();

	// Generated message map functions
	//{{AFX_MSG(CSeisStlMFactorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnChangeOption();
	afx_msg void OnChangeType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEISSTLMFACTORDLG_H__66BE7323_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
