#if !defined(AFX_WINDADDLOADKBC2015DLG_H__4C538681_1A82_11D4_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_WINDADDLOADKBC2015DLG_H__4C538681_1A82_11D4_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WindAddLoadKBC2015Dlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MComboBox.h"
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CWindAddLoadKBC2015Dlg dialog

class CWindAddLoadKBC2015Dlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CWindAddLoadKBC2015Dlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Overrides
//----------------------------------------------------------------------
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

//----------------------------------------------------------------------
// Interface
public:
	void SetMode(void* pWind, BOOL bAcross, BOOL bTorsinal);
//----------------------------------------------------------------------
// Implementation
protected:
	void FillStoryCombo(CComboBox& wndCombo, int nStart);
	BOOL ApplyOrOK();

protected:
	CDBDoc*   m_pDoc;
	void*     m_pWind;
	CArray<UINT, UINT> m_aRZ;
	BOOL m_bAcross;
	BOOL m_bTorsinal;

// Dialog Data
	//{{AFX_DATA(CWindAddLoadKBC2015Dlg)
	enum { IDD = IDD_ETC_WIND_SITEM_KBC2015 };
	CBCGPStatic	m_wndTitle1;
	CBCGPStatic	m_wndTitle2;
	CBCGPStatic	m_wndTitle3;
	CBCGPStatic	m_wndTitle4;
	CBCGPStatic	m_wndTitle5;
	CBCGPStatic	m_wndTitle6;
	CTextUnit	m_wndXUnit;
	CTextUnit	m_wndYUnit;
	CTextUnit	m_wndXUnit2;
	CTextUnit	m_wndYUnit2;
	CTextUnit	m_wndRZUnit;
	CTextUnit	m_wndRZUnit2;
	CEditUnit	m_wndXEdit;
	CEditUnit	m_wndYEdit;
	CEditUnit	m_wndXEdit2;
	CEditUnit	m_wndYEdit2;
	CEditUnit	m_wndRZEdit;
	CEditUnit	m_wndRZEdit2;
	MComboBox	m_wndStoryEnd;
	MComboBox	m_wndStoryStart;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindAddLoadKBC2015Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWindAddLoadKBC2015Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdStoryBtn();
	afx_msg void OnSelchangeCmdStoryStart();
	virtual void OnOK();
	afx_msg void OnCmdApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDADDLOADKBC2015DLG_H__4C538681_1A82_11D4_92DE_0000C0B0E6B3__INCLUDED_)
