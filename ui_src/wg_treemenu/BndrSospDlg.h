#if !defined(AFX_BNDRSOSPDLG_H__06DFDCAC_7F25_44BC_B84A_FB4B1CE86ADD__INCLUDED_)
#define AFX_BNDRSOSPDLG_H__06DFDCAC_7F25_44BC_B84A_FB4B1CE86ADD__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrSospDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxBngr.h"



#include "BndrSospUserDlg.h"
#include "BndrSospIntegral.h"

/////////////////////////////////////////////////////////////////////////////
// CBndrSospDlg dialog





class CBndrSospDlg : public CMenuBarChildDlg , public CDBUpdateConnector
{
// Construction
public:
	CBndrSospDlg(CWnd* pParent = NULL);   // standard constructor
	~CBndrSospDlg();
	virtual void Execute() { OnTmExecute(); };
// Dialog Data
	//{{AFX_DATA(CBndrSospDlg)
	enum { IDD = IDD_TM_BNDR_SOSP };
	CCobxBngr	m_wndGroupCombo;
	CBCGPStatic	m_wndPlaceHolder;
	//}}AFX_DATA
public:
	 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrSospDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL	CreateChildDlgs(void);		
	BOOL	DestroyChildDlgs(void);		
	BOOL	ShowChildDlgs(int which);	
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	
protected:
	CDBDoc* m_pDoc;
	CBndrSospUserDlg* m_pUserDlg;
	CBndrSospIntegral* m_pIntegralDlg;
	SpringUser UserData;
	SpringIntegral IntegralData;
	int	m_nType;
	int m_nBottom[2]; 


		
	void AlignControl();
	void InitSubCtrl();
	BOOL GetParameter();
	void ExecuteUser();
	void ExecuteIntegral();

	// Generated message map functions
	//{{AFX_MSG(CBndrSospDlg)
	
	virtual BOOL OnInitDialog();
	afx_msg void OnTmBndrSospType();
	
	
	afx_msg void OnTmClose();
	afx_msg void OnTmExecute();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BNDRSOSPDLG_H__06DFDCAC_7F25_44BC_B84A_FB4B1CE86ADD__INCLUDED_)