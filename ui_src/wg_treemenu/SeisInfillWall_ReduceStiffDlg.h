#if !defined(AFX_SEIS_INFILL_WALL_REDUCE_STIFF_H__INCLUDED_)
#define AFX_SEIS_INFILL_WALL_REDUCE_STIFF_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisInfillWall_ReduceStiffDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisInfillWall_ReduceStiffDlg dialog

class CSeisInfillWall_ReduceStiffDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CSeisInfillWall_ReduceStiffDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Dialog Data
	//{{AFX_DATA(CSeisInfillWall_ReduceStiffDlg)
	enum { IDD = IDD_TM_SEIS_INFILL_WALL_REDUCE_STIFF };
	int		m_nOption;
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisInfillWall_ReduceStiffDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	NM_LISTVIEW m_TempListView;
	BOOL        m_bSentMessage;

	CDBDoc* m_pDoc;
	void InitCtrl();
	void InitSelectedItem();
	void SetListCtrlHeader();
	void MakeItemEx();
	void DisplaySelElemList();  
	void GetSelElemList(CArray<T_ELEM_K, T_ELEM_K>& aSelElem);
	void GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem);
	void OnDelKeyPressedInList();    
	void SetListFocus(CArray<int, int>& aSelItem);
	void InsertItem(T_RISS_K Key);
	void DeleteItem(T_RISS_K Key);

	// Generated message map functions
	//{{AFX_MSG(CSeisInfillWall_ReduceStiffDlg)
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEIS_INFILL_WALL_REDUCE_STIFF_H__INCLUDED_)
