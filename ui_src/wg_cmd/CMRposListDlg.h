#if !defined(AFX_CMRPOSLISTDLG_H__EBEF3912_F70F_42B3_ACDB_CAC92926ED34__INCLUDED_)
#define AFX_CMRPOSLISTDLG_H__EBEF3912_F70F_42B3_ACDB_CAC92926ED34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMRposListDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCMRposListDlg dialog

class CCMRposListDlg :  public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CCMRposListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMRposListDlg)
	enum { IDD = IDD_CMD_RPOS_LIST };
	CListCtrl	m_listData;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMRposListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	virtual void UnitChanged();

// Implementation
protected:
	void SetHeaderTitle(BOOL bInit = FALSE);
	virtual void MakeItemEx(BOOL bUnitChanged = FALSE);

	BOOL InsertItem(T_RPOS_K Key, T_RPOS_D &Data);
	BOOL DeleteItem(T_RPOS_K Key, T_RPOS_D &Data);
	BOOL ModifyItem(T_RPOS_K KeyOld, T_RPOS_K Key, T_RPOS_D &Data);
	CString DataToStr(int i, T_RPOS_K Key, T_RPOS_D &Data);

protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CCMRposListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnClose();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMRPOSLISTDLG_H__EBEF3912_F70F_42B3_ACDB_CAC92926ED34__INCLUDED_)
