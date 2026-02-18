#if !defined(__RATINGCASEDLG_H__)
#define __RATINGCASEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RatingCaseDlg.h : header file
//

#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

#include "RatingCaseMainGrid.h"
#include "RatingCaseSubGrid.h"
#include "RatingCaseSubGrid.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CRatingCaseDlg dialog

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CRatingCaseDlg : public CDialogMove
{
// Construction
public:
	CRatingCaseDlg(CWnd* pParent = NULL);   // standard constructor
	~CRatingCaseDlg();
// Dialog Data
	//{{AFX_DATA(CRatingCaseDlg)
	enum { IDD = IDD_DGN_PSC_RATING_CASE };
	CListCtrl	m_List;
	MComboBox	m_AdjacentCombo;
	MComboBox	m_PrimaryCombo;
	double	m_PrimaryFactor;
	double	m_AdjacentFactor;
	int		m_iLimitState;
	int   m_iEvaluation; // Design Load / Legal Load
	CString	m_strName;
	CString	m_strDescription;
	MButton m_Ctrl_LimitState;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRatingCaseDlg)

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRatingCaseDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnDgnDefineRatingCaseAddBtn();
	afx_msg void OnDgnDefineRatingCaseDeleteBtn();
	afx_msg void OnOnItemchangedDgnRatingCaseList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDgnDefineRatingCaseModifyBtn();
	afx_msg void OnDgnDefineRatingCaseClose();
	afx_msg void OnDgnDefineRatingCaseServiceRadio_Clicked();
	afx_msg void OnDgnDefineRatingCaseLiveFactorsBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	//CArray<T_RTLD_D_TEMP, T_RTLD_D_TEMP&> m_arData;
	
	T_BLRC_K m_BlrcKey;
	T_BLRC_D m_CurrentData;

	CRatingCaseMainGrid* m_pMainGrid;
	CRatingCaseSubGrid*  m_pSubGrid;

	//RatingLiveFactorDlg* m_LiveFactorDlg;

	CDBDoc* m_pDoc;

	CArray<T_MVLD_K,T_MVLD_K> m_arMoveLoadKey;

	void TestDataInitialize();
	void InitCtrl();	
	void InitMoveLoadCombo();	
	void SetListCtrlHeader();
	void InitBlrcKey();
	void InitCurrentData();

	void LoadDlgMoveLoad();
	void LoadDlgLimitState();
	void LoadDlgEvaluation();
	void LoadDlgNameDescription();
	BOOL InsertListItem(T_BLRC_K Key);
	BOOL DeleteListItem(T_BLRC_K Key);
	BOOL ModifyListItem(T_BLRC_K Key);
	int  FindInsertionPos(T_BLRC_K nID);
	CString  DataToStr(int i, T_BLRC_K Key, T_BLRC_D &Data);

	void SaveDlgMoveLoad();
	void SaveDlgLimitState();
	void SaveDlgEvaluation();
	void SaveDlgNameDescription();

	void ChangeItem(int nIndex);
	void GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem);

	int GetMoveLoadKeyIndex(T_MVLD_K MvKey);

	int Blrckey;

	BOOL RatingCaseNameCheck(CString strName);
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__RATINGCASEDLG_H__)
