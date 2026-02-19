#if !defined(AFX_DGNRCRATINGPRINTDLG_H__)
#define AFX_DGNRCRATINGPRINTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRationPrintDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MButton.h"

class CDBDoc;

/////////////////////////////////////////////////////////////////////////////
// CDgnRCRatingPrintDlg dialog

class CDgnRCRatingPrintDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
	// Construction
public:
	CDgnRCRatingPrintDlg(CWnd* pParent = NULL);   // standard constructor

public:
	void Initial_SelectItem();
	void Initial_Data();
	// Add by ZINU.('01.1.3).
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	// Dialog Data
	//{{AFX_DATA(CDgnRationPrintDlg)
	enum { IDD = IDD_DGN_RATING_PRINT_OPTION_DLG };
	int		m_nOption;
	mit::frx::MButton	m_ratioI;
	mit::frx::MButton	m_ratioJ;
	mit::frx::MButton	m_ratioIJ;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnRationPrintDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void Execute();
// 	afx_msg void OnDgnClose();
// 	afx_msg void OnDgnExecute();
	afx_msg void OnDgnAr();
	afx_msg void OnDgnDel();
	afx_msg void SetEnableByCtrl();
	//}}AFX_VIRTUAL

	// Implementation
protected:
	CDBDoc* m_pDoc;

	BOOL SetEnableByDgnCode();


	// Generated message map functions
	//{{AFX_MSG(CDgnRationPrintDlg)
	// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNRCRATINGPRINTDLG_H__)
