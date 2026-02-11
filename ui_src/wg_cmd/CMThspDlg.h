#if !defined(AFX_CMTHSPDLG_H__E317A07C_1A96_4280_B5CB_E2E8FA42D2C7__INCLUDED_)
#define AFX_CMTHSPDLG_H__E317A07C_1A96_4280_B5CB_E2E8FA42D2C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThspDlg.h : header file
//
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_NotifyListCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxThfc.h"
#include "..\wg_db\wg_db_SelectLC.h"

#include "CMChildBarBase.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
/////////////////////////////////////////////////////////////////////////////
// CCMThspDlg dialog

class CCMThspDlg : public CCMChildBarBase , public CDBUpdateConnector
{
// Construction
public:
	CCMThspDlg(CWnd* pParent = NULL);   // standard constructor
	void MakeListHeader();
	void MakeThslList();
	void GetParameter(void* pThslD, void * pThslK);
	void SetParameter(void* pThslD, void * pThslK);
	BOOL GetSelectedData(int &nIndex);

	virtual BOOL ExternalInit(UINT key);
	void Execute(){};
// Dialog Data
	//{{AFX_DATA(CCMThspDlg)
	enum { IDD = IDD_DBAR_THSP_DLG };
	MEdit	m_wndScaleFactor;
	MEdit	m_wndArrivalTime;
	CNotifyListCtrl	m_wndFuncList;
	CCobxThfc	m_FuncNameCombo;
	CSelectLC	m_wndSTLCCombo;
	CSelectLC	m_wndTHLCCombo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThspDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMThspDlg)
	afx_msg void OnCmdLoadcaseBtn();
	afx_msg void OnCmdFuncNameBtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdSTLoadcaseBtn();
	afx_msg void OnSelchangeCmdFuncNameCombo();
	afx_msg void OnCmdAddButton();
	afx_msg void OnCmdModifyButton();
	afx_msg void OnCmdDeleteButton();
	afx_msg void OnItemchangedCmdFuncList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickCmdFuncList(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnCmdClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTHSPDLG_H__E317A07C_1A96_4280_B5CB_E2E8FA42D2C7__INCLUDED_)
