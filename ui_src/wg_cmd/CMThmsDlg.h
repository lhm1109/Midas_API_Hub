#if !defined(AFX_CMTHMSDLG_H__87C2DC98_E89C_424F_ABF2_BBBBBE673CF5__INCLUDED_)
#define AFX_CMTHMSDLG_H__87C2DC98_E89C_424F_ABF2_BBBBBE673CF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThmsDlg.h : header file
//
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxThfc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_SelectCtrl.h"

#include "CMChildBarBase.h"

using namespace mit::frx;

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
/////////////////////////////////////////////////////////////////////////////
// CCMThmsDlg dialog

class CCMThmsDlg : public CCMChildBarBase, public CDBUpdateConnector
{
// Construction
public:
	CCMThmsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMThmsDlg)
	enum { IDD = IDD_DBAR_THMS_DLG };
	CFormulaEditSpin	      m_wndHGA;
	CSelectLC	  m_wndLoadCase;
	int		m_nOption;
	//}}AFX_DATA
	CTextUnit	m_wndArTimeUnit[3];
	CEditUnit	m_wndArTime[3];
	CFormulaEdit m_wndScaleFactor[3];
	CCobxThfc	m_wndFuncName[3];

	CDBDoc* m_pDoc;
	CArray<UINT, UINT> m_aCtrlDir[3];

public:
	virtual BOOL ExternalInit(UINT key);
	virtual void Execute();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThmsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	//virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//virtual void UpdateBuffer();

// Implementation
protected:

	protected:
	//CArray<UINT, UINT> m_aCtrlRadio;
	//CArray<UINT, UINT> m_aCtrlAngle;
	CArray<UINT, UINT> m_aCtrl;

protected:
	virtual ~CCMThmsDlg();

	BOOL Dlg2Data(T_THMS_D &data);
	void Data2Dlg(T_THMS_D &data);
	BOOL GetLoadCaseKey(T_THIS_K& key);
	void FuncDirCtrl(BOOL bAdd);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CCMThmsDlg)
// 	afx_msg void OnCmdClose();
// 	afx_msg void OnCmdApply();
	afx_msg void OnCmdLoadcaseBtn();
	afx_msg void OnCmdFuncBtn();
	afx_msg void OnSelchangeCmdFuncName();
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposCmdAngleOfHgaSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdOptionAdd();
	afx_msg void OnCmdOptionDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTHMSDLG_H__87C2DC98_E89C_424F_ABF2_BBBBBE673CF5__INCLUDED_)
