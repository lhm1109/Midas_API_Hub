#if !defined(__CMTHATDLG_H__)
#define __CMTHATDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThatDlg.h : header file
//
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "CMChildBarBase.h"
/////////////////////////////////////////////////////////////////////////////
// CCMThatDlg dialog

class CCMThatDlg : public CCMChildBarBase, public CDBUpdateConnector
{
// Construction
public:
	CCMThatDlg(CWnd* pParent = NULL);   // standard constructor

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	virtual void Execute(){}
	virtual BOOL ExternalInit(UINT key);

// Dialog Data
	//{{AFX_DATA(CCMThatDlg)
	enum { IDD = IDD_DBAR_THAT_DLG };
	CListCtrl	m_List;
	CEditUnit	m_edtInc;
	CEditUnit	m_edtEnd;
	CEditUnit	m_edtStart;
	CTextUnit	m_StartUnit;
	CTextUnit	m_IncUnit;
	CTextUnit	m_EndUnit;
	CSelectLC	m_cmbThlc;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void MakeListHeader();
	void MakeItemEx();
	void SetItem(int nIndex, T_THAT_K key, T_THAT_D& rData);
	CString DataToStr(int nIndex, T_THAT_K key, T_THAT_D& rData);
	void Data2Dlg(T_THAT_K& rKey, T_THAT_D& rData);
	void Dlg2Data(T_THAT_K& rKey, T_THAT_D& rData);
	BOOL GetSelectedData(int &nIndex);

	// Generated message map functions
	//{{AFX_MSG(CCMThatDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedThatList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdClose();
	afx_msg void OnCmdLoadcaseBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMTHATDLG_H__)
