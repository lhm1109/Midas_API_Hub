#if !defined(__WINDLISTDLG_H__)
#define __WINDLISTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WindListDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CWindListDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CWindListDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CWindListDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Overrides
//----------------------------------------------------------------------
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

//----------------------------------------------------------------------
// Implementation
protected:
	void SetHeaderTitle();
	virtual void MakeItemEx(BOOL bUnitChanged=FALSE);

	int  FindInsertionPos(T_WIND_K Key);
	BOOL InsertItem(T_WIND_K Key, T_WIND_D &Data);
	BOOL DeleteItem(T_WIND_K Key, T_WIND_D &Data);
	BOOL ModifyItem(T_WIND_K KeyOld, T_WIND_K Key, T_WIND_D &Data);
	CString DataToStr(int i, T_WIND_K Key, T_WIND_D &Data);

	CString ConvWindLoadShortTypeEng2Rus(const CString& strName) const;
	CString ConvWindLoadShortTypeRus2Eng(const CString& strName) const;

protected:
	CDBDoc* m_pDoc;

// Dialog Data
	//{{AFX_DATA(CWindListDlg)
	enum { IDD = IDD_ETC_WIND_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWindListDlg)
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnClose();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__WINDLISTDLG_H__)
