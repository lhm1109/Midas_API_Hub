#if !defined(__CMLTOMDLG_H__)
#define __CMLTOMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLtomDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\MIT_frx\MButton.h"	

/////////////////////////////////////////////////////////////////////////////
// CCMLtomDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMLtomDlg : public CDialogMove, public CDBUpdateConnector
{
//-------------------------------------------------
// Construction
//-------------------------------------------------
public:
	CCMLtomDlg(CWnd* pParent = NULL);   // standard constructor

//-------------------------------------------------
// Interfaces
//-------------------------------------------------
public:
	BOOL m_bWorkTab;
	int  m_WTLoadCaseNum;
	void SetCurrentLC();
	void SetParamData(int iItem);

//-------------------------------------------------
// Implementations
//-------------------------------------------------
protected:
	void SetListCtrlHeader();
	BOOL GetSelectedItem(int &nIndex);
	void MakeItemEx();
	CString DataToStr(int i, T_LTOM_BASE &data);
	void SetItem(int nIndex, T_LTOM_BASE &data);
	BOOL ValidItem(T_LTOM_BASE& data);
	BOOL Dlg2Item(T_LTOM_BASE &data);
	BOOL Item2Dlg(T_LTOM_BASE &data);
	void Data2Dlg();

protected:
	CDBDoc* m_pDoc;
	T_LTOM_D m_Data;
	CArray<UINT, UINT> m_aCtrlDir;


// Dialog Data
	//{{AFX_DATA(CCMLtomDlg)
	enum { IDD = IDD_ETC_LTOM };
	CTextUnit	m_wndGravityUnit;
	CSelectLC	m_LoadCase;
	mit::frx::MButton	m_wndRemoveBtn;
	CListCtrl	m_List;
	CEditUnit	m_wndScaleFactor;
	CEditUnit	m_wndGravity;
	mit::frx::MButton	m_wndPres;
	mit::frx::MButton	m_wndFbld;
	mit::frx::MButton	m_wndBmld;
	mit::frx::MButton	m_wndCnld;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLtomDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMLtomDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdLoadcaseBtn();
	afx_msg void OnCmdRemoveData();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	virtual void OnOK();
	afx_msg void OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMLTOMDLG_H__)
