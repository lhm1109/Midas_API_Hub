#if !defined(__DGNSTLRATINGGROUPSETTINGDLG_H__)
#define __DGNSTLRATINGGROUPSETTINGDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlRatingGroupSettingDlg.h : header file
//
#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\mit_frx\MCheckListBox.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnStlRatingGroupSettingDlg dialog

#include "HeaderPre.h"

class CDgnDataCtrl;
class __MY_EXT_CLASS__ CDgnStlRatingGroupSettingDlg : public CDialogMove
{
// Construction
public:
	CDgnStlRatingGroupSettingDlg(CWnd* pParent = NULL);   // standard constructor
	~CDgnStlRatingGroupSettingDlg();

// Dialog Data
	//{{AFX_DATA(CDgnStlRatingGroupSettingDlg)
	enum { IDD = IDD_DGN_STL_RATING_GROUP_DLG };
	CListCtrl	m_listGroup;
	mit::frx::MCheckListBox	m_checkListSelectGroup;
	double	m_dFactor;
	BOOL	m_bIEnd;
	BOOL	m_bJEnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlRatingGroupSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnStlRatingGroupSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnRatingGroupAddBtn();
	afx_msg void OnDgnRatingGroupModifyBtn();
	afx_msg void OnDgnRatingGroupDeleteBtn();
	afx_msg void OnItemchangedDgnRatingGroupList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDgnRatingGroupClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:

	CDBDoc* m_pDoc;
	CArray<T_GRUP_K, T_GRUP_K> m_arGrupKey;
	CArray<T_GRUP_K, T_GRUP_K> m_arSelectGrupKey;
	CMap<T_GRUP_K,T_GRUP_K,int,int> m_mapListKey;

	T_RSGS_D m_MainData;

	CDgnDataCtrl* m_pDataCtrl;

	void InitSelectGroupList();
	void SetListCtrlHeader();
	void GetDBRsgs();
	int  FindInsertionPos(T_RSGS_K nID);
	BOOL InsertListItem(T_RSGS_K Key);
	BOOL DeleteListItem(T_RSGS_K Key);
	BOOL ModifyListItem(T_RSGS_K Key);
	CString  DataToStr(int i, T_RSGS_K Key, T_RSGS_D &Data);

	void LoadDlgFactorPosition();
	
	BOOL SaveDlgSelectGroup();
	BOOL SaveDlgFactorPosition();

	void ChangeItem(int nIndex);

	void GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem);

};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNSTLRATINGGROUPSETTINGDLG_H__)
