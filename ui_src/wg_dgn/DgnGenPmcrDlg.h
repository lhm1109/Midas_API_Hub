#if !defined(AFX_DGNGENPMCRDLG_H__147352C8_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNGENPMCRDLG_H__147352C8_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenPMcrDlg.h : header file
//
#include "..\MIT_frx\MillustViewer.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnGenPMcrDlg dialog

class CDgnGenPmcrDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnGenPmcrDlg(CWnd* pParent = NULL);   // standard constructor

public:


public:
	void Initial_Data();
	void InitCtrl();
	void Initial_SelectItem();
	// Add by ZINU.('01.1.3).
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode();

// Dialog Data
	//{{AFX_DATA(CDgnGenPMcrDlg)
	enum { IDD = IDD_DGN_GEN_PMCR_DLG };
	int		m_nOption;
	
	CEditUnit m_edtUserZg;
	CTextUnit m_untUserZg;
	mit::frx::MillustViewer m_wndSteelImg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenPMcrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	CArray<UINT, UINT> m_aPMCRRdo;
	CArray<UINT, UINT> m_aUserZg;

	// Generated message map functions
	//{{AFX_MSG(CDgnGenPMcrDlg)
	virtual void Execute();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnPmcrnifyAr();
	afx_msg void OnDgnPmcrnifyDel();
	afx_msg void OnDgnPmcrTypeRdo();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNGENPMCRDLG_H__147352C8_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
