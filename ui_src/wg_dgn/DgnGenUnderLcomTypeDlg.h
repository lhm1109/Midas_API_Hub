#if !defined(AFX_DgnGenUnderLcomTypeDlg_H__INCLUDED_)
#define AFX_DgnGenUnderLcomTypeDlg_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenUnderLcomTypeDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_base\wg_base_MouseEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnGenUnderLcomTypeDlg dialog

class CDBDoc;

class CDgnGenUnderLcomTypeDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
	// Construction
public:
	CDgnGenUnderLcomTypeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

	// Dialog Data
		//{{AFX_DATA(CDgnGenUnderLcomTypeDlg)
	enum { IDD = IDD_DGN_GEN_UNDER_LCOM_DLG };
	int		m_nOption;
	int   m_nOptUnder;
	//}}AFX_DATA

public:
	void Update_InitDataByCode();
	void EnableCtrl(BOOL bEnable);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CDgnGenUnderLcomTypeDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	// Generated message map functions
	//{{AFX_MSG(CDgnGenUnderLcomTypeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnLcomAr();
	afx_msg void OnDgnLcomDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DgnGenUnderLcomTypeDlg_H__INCLUDED_)
