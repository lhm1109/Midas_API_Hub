#if !defined(AFX_CMTHLoadCombListDlg_H__50132E62_1E8E_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMTHLoadCombListDlg_H__50132E62_1E8E_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "CMDlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMTHLoadCombListDlg : public CCMDlgBase
{
public:
	CCMTHLoadCombListDlg(CWnd* pParent = NULL); 

public:
	CDBDoc*		m_pDoc;

public:
	void SetHeaderTitle(BOOL bInit = FALSE);

// Dialog Data
	//{{AFX_DATA(CCMTHLoadCombListDlg)
	enum { IDD = IDD_CMD_TH_LOAD_COMB_LIST_DLG };
	CListCtrl	m_List;
	//}}AFX_DATA

// Overrides
public:
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMTHLoadCombListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void MakeItemEx();

	BOOL InsertItem(T_THLC_K Key, T_THLC_D &Data);
	BOOL DeleteItem(T_THLC_K Key, T_THLC_D &Data);
	BOOL ModifyItem(T_THLC_K KeyOld, T_THLC_K Key, T_THLC_D &Data);

	CString DataToStr(int i, T_THLC_K Key, T_THLC_D &Data);

	// Generated message map functions
	//{{AFX_MSG(CCMTHLoadCombListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnModify();
	afx_msg void OnBtnDelete();
	afx_msg void OnBtnClose();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

#endif // !defined(AFX_CMTHLoadCombListDlg_H__50132E62_1E8E_11D3_92DE_0000C0B0E6B3__INCLUDED_)
