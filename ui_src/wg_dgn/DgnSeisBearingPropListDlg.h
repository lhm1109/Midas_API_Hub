#if !defined(__DGNSEISBEARINGPROPLISTDLG_H__)
#define __DGNSEISBEARINGPROPLISTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisBearingPropListDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisBearingPropListDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnSeisBearingPropListDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CDgnSeisBearingPropListDlg(CWnd* pParent = NULL);   // standard constructor

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
	BOOL InsertItem(T_BRPR_K Key, T_BRPR_D &Data);
	BOOL DeleteItem(T_BRPR_K Key, T_BRPR_D &Data);
	BOOL ModifyItem(T_BRPR_K KeyOld, T_BRPR_K Key, T_BRPR_D &Data);
	CString DataToStr(int i, T_BRPR_K Key, T_BRPR_D &Data);
	BOOL CheckData(T_BRPR_K Key);

protected:
	CDBDoc* m_pDoc;

public:
// Dialog Data
	//{{AFX_DATA(CDgnSeisBearingPropListDlg)
	enum { IDD = IDD_DGN_SEIS_BEARING_PROP_LIST_DLG };
	CListCtrl	m_List;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisBearingPropListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnSeisBearingPropListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSeisBearingPropAddBtn();
	afx_msg void OnSeisBearingPropModBtn();
	afx_msg void OnSeisBearingPropDelBtn();
	afx_msg void OnSeisBearingPropCopyBtn();
	afx_msg void OnSeisBearingPropCloseBtn();
	afx_msg void OnSeisBearingPropDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNSEISBEARINGPROPLISTDLG_H__)
