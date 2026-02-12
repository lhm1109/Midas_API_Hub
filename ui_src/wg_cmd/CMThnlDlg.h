#if !defined(AFX_CMTHNLDLG_H__93AC9A81_E959_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMTHNLDLG_H__93AC9A81_E959_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThnlDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxThfc.h"
#include "..\wg_db\wg_db_SelectLC.h"

//#include "CMDlgBarBase.h"
/////////////////////////////////////////////////////////////////////////////
// CCMThnlDlg form view

#include "CMChildBarBase.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CCMThnlDlg :  public CCMChildBarBase  , public CDBUpdateConnector
{
public:
	CCMThnlDlg();           // protected constructor used by dynamic creation
	//DECLARE_DYNCREATE(CCMThnlDlg)

// Form Data
public:
	//{{AFX_DATA(CCMThnlDlg)
	enum { IDD = IDD_DBAR_THNL_DLG };
	CBCGPStatic	m_wndLoadType;
	CFormulaEdit m_wndScaleFactor;
	CTextUnit	m_wndArTimeUnit;
	CEditUnit	m_wndArTime;
	CCobxThfc	m_wndFuncName;
	CSelectLC	m_wndLoadCase;
	//}}AFX_DATA

// Attributes
public:
	CDBDoc* m_pDoc;

// Operations
public:
	virtual BOOL ExternalInit(UINT key);
	void Execute();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThnlDlg)
	public:
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
// Implementation
protected:
	CArray<UINT, UINT> m_aCtrlDelete;
	CArray<UINT, UINT> m_aCtrlOptionRadio;
	CArray<UINT, UINT> m_aCtrlDirRadio;

protected:
	virtual ~CCMThnlDlg();
	BOOL Dlg2Data(T_THNL_D &data);
	BOOL Data2Dlg(T_THNL_D &data);
	void UpdateLoadType();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CCMThnlDlg)
	afx_msg void OnCmdLoadcaseBtn();
	afx_msg void OnChangeOption();
	afx_msg void OnCmdFuncBtn();
	afx_msg void OnCmdClose();
	afx_msg void OnCmdApply();
	afx_msg void OnSelchangeCmdFuncName();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTHNLDLG_H__93AC9A81_E959_11D3_92DE_0000C0B0E6B3__INCLUDED_)
