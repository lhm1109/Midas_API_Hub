#if !defined(__DGNSEISSUPPORTLENGTHLISTDLG_H__)
#define __DGNSEISSUPPORTLENGTHLISTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisSupportLengthListDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisSupportLengthListDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnSeisSupportLengthListDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CDgnSeisSupportLengthListDlg(CWnd* pParent = NULL);   // standard constructor

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
	void MakeItemEx();

	int  FindInsertionPos(int nID);	
	BOOL InsertItem(T_SUPL_K Key, T_SUPL_D &Data);
	BOOL DeleteItem(T_SUPL_K Key, T_SUPL_D &Data);
	BOOL ModifyItem(T_SUPL_K KeyOld, T_SUPL_K Key, T_SUPL_D &Data);
	CString DataToStr(int i, T_SUPL_K Key, T_SUPL_D &Data);

protected:
	CDBDoc* m_pDoc;

public:
// Dialog Data
	//{{AFX_DATA(CDgnSeisSupportLengthListDlg)
	enum { IDD = IDD_DGN_SEIS_SUPPORT_LENGTH_LIST_DLG };

	CListCtrl	m_List;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisSupportLengthListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnSeisSupportLengthListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSeisSupportLengthAddBtn();
	afx_msg void OnSeisSupportLengthModBtn();
	afx_msg void OnSeisSupportLengthDelBtn();
	afx_msg void OnSeisSupportLengthCopyBtn();
	afx_msg void OnSeisSupportLengthCloseBtn();
	afx_msg void OnSeisSupportLengthDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNSEISSUPPORTLENGTHLISTDLG_H__)
