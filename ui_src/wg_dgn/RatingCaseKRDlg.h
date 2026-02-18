#if !defined(__RATINGCASEKRDLG_H__)
#define __RATINGCASEKRDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RatingCaseKRDlg.h : header file
//

#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "RatingCaseKRGrid.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CRatingCaseKRDlg dialog

#include "HeaderPre.h"


class __MY_EXT_CLASS__ CRatingCaseKRDlg : public CDialogMove
{
// Construction
public:
	CRatingCaseKRDlg(CWnd* pParent = NULL);   // standard constructor
	~CRatingCaseKRDlg();
// Dialog Data
	//{{AFX_DATA(CRatingCaseKRDlg)
	enum { IDD = IDD_DGN_PSC_RATING_CASE_KR_DLG };
	MButton	m_ratioState1Service;
	MButton	m_ratioState2Strength;	
	CListCtrl	m_List;
	MComboBox	m_comboLoadCase;	
	CEditUnit	m_editLoadCase;	
	CEditUnit	m_editImpactFactor;	
	MEdit	m_editName;	
	MEdit	m_editDescription;		
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRatingCaseKRDlg)

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRatingCaseKRDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnDgnDefineRatingCaseAddBtn();
	afx_msg void OnDgnDefineRatingCaseDeleteBtn();
	afx_msg void OnOnItemchangedDgnRatingCaseList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDgnDefineRatingCaseModifyBtn();
	afx_msg void OnDgnDefineRatingCaseClose();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	
	
	T_RKLC_K m_Key;
	T_RKLC_D m_Data;
	
	CRatingCaseKRGrid*  m_pGrid;

	//RatingLiveFactorDlg* m_LiveFactorDlg;

	CDBDoc* m_pDoc;

	CArray<int,int> m_arLoadKey;
	CArray<int,int> m_arAnalType;

	void TestDataInitialize();
	void InitLoadCombo();	
	void SetListCtrlHeader();
	void InitKey();
	void InitData();

	void LoadDlgMoveLoad();
	void LoadDlgLimitState();
	void LoadDlgNameDescription();
	BOOL InsertListItem(T_RKLC_K Key);
	BOOL DeleteListItem(T_RKLC_K Key);
	BOOL ModifyListItem(T_RKLC_K Key);
	int  FindInsertionPos(T_RKLC_K nID);
	CString  DataToStr(int i, T_RKLC_K Key, T_RKLC_D &Data);

	void SaveDlgMoveLoad();
	void SaveDlgLimitState();
	void SaveDlgNameDescription();

	void ChangeItem(int nIndex);
	void GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem);

	int GetLoadCaseKeyIndex(int Key, int AnalType);

	int m_nNewkey;

	BOOL RatingCaseNameCheck(CString strName);
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__RATINGCASEKRDLG_H__)
