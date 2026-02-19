#if !defined(AFX_CDgnStlSideswayDlg_H__147352C7_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_CDgnStlSideswayDlg_H__147352C7_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenKFacDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnStlSideswayDlg dialog

class CDBDoc;

class CDgnStlSideswayDlg : public CMenuBarChildDlg
{
// Construction
public:
	CDgnStlSideswayDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnStlPhibFacDlg)
	enum { IDD = IDD_DGN_GEN_SDSW_DLG };
	int		m_nOption;
	int	    m_nSideswayy;
	int	    m_nSideswayz;
	//}}AFX_DATA

public:
	void Initial_SelectItem();
	void Initial_Data();
	BOOL ErrorCheck();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode();

	void SectPropertyTest();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlPhibFacDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	// Generated message map functions
	//{{AFX_MSG(CDgnStlPhibFacDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnAr();
	afx_msg void OnDgnDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDgnStlPhibFacDlg_H__147352C7_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
