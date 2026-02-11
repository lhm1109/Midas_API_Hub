#if !defined(AFX_CMEBeamPageDlg_H__50132E62_1E8E_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMEBeamPageDlg_H__50132E62_1E8E_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMEBeamPageDlg.h : header file
//
#include "CMDlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCMEBeamPageDlg dialog

/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////

class __MY_EXT_CLASS__ CCMEBeamPageDlg : public CCMDlgBase
{
	// Construction
public:
	CCMEBeamPageDlg(CWnd* pParent = NULL);   // standard constructor

	// Attributes
public:
	CDBDoc*		m_pDoc;

	// Operations
public:
	void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);

	// Dialog Data
	//{{AFX_DATA(CCMEBeamPageDlg)
	enum { IDD = IDD_CMD_EBEAM_PAGE_DLG };
	CListCtrl	m_List;
	//}}AFX_DATA

	// Overrides
public:
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMEBeamPageDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

	// Implementation
protected:
	BOOL InsertItem(T_PREB_K Key, T_PREB_D &Data);
	BOOL DeleteItem(T_PREB_K Key, T_PREB_D &Data);
	BOOL ModifyItem(T_PREB_K KeyOld, T_PREB_K Key, T_PREB_D &Data);
	CString DataToStr(int i, T_PREB_K Key, T_PREB_D &Data);

	virtual void MakeItemEx(BOOL bUnitChanged = FALSE);

	// Generated message map functions
	//{{AFX_MSG(CCMEBeamPageDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnModify();
	afx_msg void OnBtnDelete();
	afx_msg void OnBtnClose();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////
#include "HeaderPost.h"      ////////
/////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMEBeamPageDlg_H__50132E62_1E8E_11D3_92DE_0000C0B0E6B3__INCLUDED_)
