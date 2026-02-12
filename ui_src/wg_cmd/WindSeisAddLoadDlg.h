#if !defined(AFX_WINDSEISADDLOADDLG_H__4C538681_1A82_11D4_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_WINDSEISADDLOADDLG_H__4C538681_1A82_11D4_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WindSeisAddLoadDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MComboBox.h"
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CWindSeisAddLoadDlg dialog

class CWindSeisAddLoadDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CWindSeisAddLoadDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Overrides
//----------------------------------------------------------------------
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

//----------------------------------------------------------------------
// Interface
public:
	void SetMode(void* pWind, void* pSeis, void* pPsll);
//----------------------------------------------------------------------
// Implementation
protected:
	void FillStoryCombo(CComboBox& wndCombo, int nStart);
	BOOL ApplyOrOK();

protected:
	CDBDoc*   m_pDoc;
	void*     m_pWind;
	void*     m_pSeis;
	void*     m_pPsll;
	CArray<UINT, UINT> m_aRZ;

// Dialog Data
	//{{AFX_DATA(CWindSeisAddLoadDlg)
	enum { IDD = IDD_ETC_WIND_SITEM };
	CBCGPStatic	m_wndTitle2;
	CBCGPStatic	m_wndTitle1;
	CBCGPStatic	m_wndTitle3;
	CTextUnit	m_wndYUnit;
	CTextUnit	m_wndXUnit;
	CTextUnit	m_wndRZUnit;
	CEditUnit	m_wndYEdit;
	CEditUnit	m_wndXEdit;
	CEditUnit	m_wndRZEdit;
	MComboBox	m_wndStoryEnd;
	MComboBox	m_wndStoryStart;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindSeisAddLoadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWindSeisAddLoadDlg)
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

#endif // !defined(AFX_WINDSEISADDLOADDLG_H__4C538681_1A82_11D4_92DE_0000C0B0E6B3__INCLUDED_)
