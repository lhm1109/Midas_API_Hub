#if !defined(AFX_DGNGENMEMBCHGDLG_H__F2494C2A_BA21_441F_B511_4334D2C76511__INCLUDED_)
#define AFX_DGNGENMEMBCHGDLG_H__F2494C2A_BA21_441F_B511_4334D2C76511__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenMembChgDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\MIT_frx\MButton.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnGenMembChgDlg dialog

class CDgnGenMembChgDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnGenMembChgDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
// Dialog Data
	//{{AFX_DATA(CDgnGenMembChgDlg)
	enum { IDD = IDD_DGN_GEN_MEMB_CHG_DLG };
	int		m_nSelectType; 
	mit::frx::MButton m_BtnSelect;
 	CListCtrl	m_List;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenMembChgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc; 
 	virtual ~CDgnGenMembChgDlg();
	void ReverseMemberLocalDirection();
 	void InitSelectedItem();
	// Generated message map functions
	//{{AFX_MSG(CDgnGenMembDlg)
	afx_msg void OnDgnMemberSelect();
	virtual BOOL OnInitDialog();
	virtual void Execute();
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	// Generated message map functions
	//{{AFX_MSG(CDgnGenMembChgDlg)
	afx_msg void OnSetfocusList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
protected:
	CArray<T_ELEM_K, T_ELEM_K> m_aSelFrmKey;   // 선택된 element 중에서 frame type의 element list  
	CMap<T_MEMB_K, T_MEMB_K, T_MEMB_K, T_MEMB_K> m_mapSelMembKey;  // 방향을 바꿀 멤버 리스트
	NM_LISTVIEW m_TempListView;
	BOOL        m_bSentMessage;

protected:
	void SetListCtrlHeader();
	void MakeItemEx();
	void GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem);
	void DisplaySelElemList();
	void HighlightListSelElem();
	void GetAllElemList(CArray<T_ELEM_K, T_ELEM_K>& aSelElem);
	void GetSelElemList(CArray<T_ELEM_K, T_ELEM_K>& aSelElem);
	void InsertItem(T_MEMB_K Key);
	void DeleteItem(T_MEMB_K Key);
	void ModifyItem(T_MEMB_K Key);
	void ModifyItem(CArray<T_MEMB_K, T_MEMB_K>& aKeyMemb);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNGENMEMBCHGDLG_H__F2494C2A_BA21_441F_B511_4334D2C76511__INCLUDED_)
