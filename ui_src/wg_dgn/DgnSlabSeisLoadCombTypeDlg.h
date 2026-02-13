#if !defined(__DgnSlabSeisLoadCombTypeDlg_H__)
#define __DgnSlabSeisLoadCombTypeDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSlabSeisLoadCombTypeDlg.h : header file
//
#include "..\MIT_frx\MEdit.h"

#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "DgnDlgBase.h"
#include "HeaderPre.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnSlabSeisLoadCombTypeDlg dialog

class __MY_EXT_CLASS__ CDgnSlabSeisLoadCombTypeDlg : public CDgnDlgBase, public CDBUpdateConnector
{
	// Construction
public:
	CDgnSlabSeisLoadCombTypeDlg(CWnd* pParent = NULL);
	virtual ~CDgnSlabSeisLoadCombTypeDlg();

	// Dialog Data
	//{{AFX_DATA(CDgnSlabSeisLoadCombTypeDlg)
	enum { IDD = IDD_DGN_SLAB_SEIS_LOAD_COMB_DLG };
	CListCtrl m_List;
	int m_nMembType;
	mit::frx::MEdit m_edtName;
	CSelectEdit m_edtElem;
	int m_nSeisType; // 0:Special, 1:Vertical
	//}}AFX_DATA

protected:
	BOOL m_bInit;
	CDBDoc* m_pDoc;

protected:
	void InitCtrls();
	void Data2Dlg();
	BOOL Dlg2Data();

	void SetListCtrlHeader();
	BOOL GetSelectedItem(CString& strName);
	int GetSelectedItems(CStringArray& aStrName);
	void MakeItemEx();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSlabSeisLoadCombTypeDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

private:
	int GetSlabElemList(T_ELEM_K_LIST& raElemK);
	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnSlabSeisLoadCombTypeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual BOOL DestroyWindow();	
	afx_msg void OnClkListItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChangedList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddReplace();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnSlabSeisLoadCombTypeDlg_H__)
