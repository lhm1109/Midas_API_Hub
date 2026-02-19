#if !defined(__CPGTRANSTIFENDSUPPORT_IRC_DLG_H__)
#define __CPGTRANSTIFENDSUPPORT_IRC_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPGTranStifEndSupport_IRC_Dlg.h : header file
//
#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\mit_frx\MCheckListBox.h"

/////////////////////////////////////////////////////////////////////////////
// CCPGTranStifEndSupport_IRC_Dlg dialog

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCPGTranStifEndSupport_IRC_Dlg : public CDialogMove
{
// Construction
public:
	CCPGTranStifEndSupport_IRC_Dlg(CWnd* pParent = NULL);   // standard constructor
	~CCPGTranStifEndSupport_IRC_Dlg();

// Dialog Data
	//{{AFX_DATA(CCPGTranStifEndSupport_IRC_Dlg)
	enum { IDD = IDD_DGN_CPG_STIF_SUPPORT_IRC_DLG };
	CListCtrl	m_list;
	mit::frx::MCheckListBox	m_checkListSelectSupport;
	int	m_iEndpost;
	CEditUnit	m_dHt;
	CEditUnit	m_dT;
	CEditUnit	m_dE;
	CTextUnit	m_dHtUnit;
	CTextUnit	m_dTUnit;
	CTextUnit	m_dEUnit;
	//}}AFX_DATA

	CSelectEdit m_editSelectNode;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPGTranStifEndSupport_IRC_Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCPGTranStifEndSupport_IRC_Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnAddBtn();
	afx_msg void OnDgnModifyBtn();
	afx_msg void OnDgnDeleteBtn();
	afx_msg void OnItemchangedDgnList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedDgnCheckList();
	afx_msg void OnDgnClose();
	afx_msg void OnPostRadio();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:

	CDBDoc* m_pDoc;
	CArray<T_NODE_K, T_NODE_K> m_arNodeKeyList;
	CArray<T_NODE_K, T_NODE_K> m_arSelectNodeKey;
	CMap<T_CGTE_IRC_K,T_CGTE_IRC_K,int,int> m_mapListKey;

	T_CGTE_IRC_D m_MainData;

	void InitSelectSupportList();
	void SetListCtrlHeader();
	void GetDBCgte();
	int  FindInsertionPos(T_CGTE_IRC_K nID);
	BOOL InsertListItem(T_CGTE_IRC_K Key);
	BOOL DeleteListItem(T_CGTE_IRC_K Key);
	BOOL ModifyListItem(T_CGTE_IRC_K Key);
	CString  DataToStr(int i, T_CGTE_IRC_K Key, T_CGTE_IRC_D &Data);

	void LoadDlgData();
	
	BOOL SaveDlgSelectSupport();
	BOOL SaveDlgFactorPosition();	

	void ChangeItem(int nIndex);

	void GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem);

};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CPGTranStifEndSupport_IRC_Dlg_H__)
