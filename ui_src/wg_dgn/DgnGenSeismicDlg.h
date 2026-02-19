#if !defined(AFX_DGNGENSEISMICDLG_H__INCLUDED_)
#define AFX_DGNGENSEISMICDLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenSeismicDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_base\wg_base_MouseEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnGenSeismicDlg dialog

class CDBDoc;

class CDgnGenSeismicDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnGenSeismicDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnGenSeismicDlg)
	enum { IDD = IDD_DGN_GEN_SEISMIC_DLG };
	int		m_nOption;
	int   m_nOptSeis;
	//}}AFX_DATA

public:
	void Initial_SelectItem();
	void Initial_Data();
	void Update_InitDataByCode();
	//BOOL ErrorCheck();
	// Add by ZINU.('02.3.21).
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenSeismicDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	// Generated message map functions
	//{{AFX_MSG(CDgnGenSeismicDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnSeismicAr();
	afx_msg void OnDgnSeismicDel();
	afx_msg void OnDgnSeismicLoads();
	afx_msg void OnDgnSeismicForces();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNGENSEISMICDLG_H__INCLUDED_)
