
#pragma once

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\DlgReplaceMgr.h"

#include "BndrSospUserDlg.h"
#include "BndrSospIntegral.h"

class CResponseDispLoadDlg : public CMenuBarChildDlg , public CDBUpdateConnector
{
		// Construction
public:
		CResponseDispLoadDlg(CWnd* pParent = NULL);   // standard constructor
		~CResponseDispLoadDlg();
		virtual void Execute();
		// Dialog Data
		//{{AFX_DATA(CBndrSospDlg)
		enum { IDD = IDD_TM_RESPONS_DISP_LOAD };
		CDlgReplaceMgr	m_ChildDlgMgr;
		//}}AFX_DATA
public:


		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CBndrSospDlg)
protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		//}}AFX_VIRTUAL

		// Implementation
protected:
		BOOL	CreateChildDlgs(void);		
		int     GetCurMode();
		BOOL	ShowChildDlgs(int which);	
		virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

protected:
		CDBDoc* m_pDoc;
		virtual BOOL OnInitDialog();
		//}}AFX_MSG
		afx_msg void OnChangeMode();
		DECLARE_MESSAGE_MAP()
public:
		afx_msg void OnBnClickedStaticResponsLoadMode();
};

