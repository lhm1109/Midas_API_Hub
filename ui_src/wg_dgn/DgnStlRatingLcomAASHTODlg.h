#if !defined(__DGNSTLRATINGLCOMAASHTODLG_H__)
#define __DGNSTLRATINGLCOMAASHTODLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlRatingLcomAASHTODlg.h : header file
//

#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "DgnStlRatingLcomAASHTOMainGrid.h"
#include "DgnStlRatingLcomAASHTOSubGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnStlRatingLcomAASHTODlg dialog
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnStlRatingLcomAASHTODlg : public CDialogMove
{
// Construction
public:
	CDgnStlRatingLcomAASHTODlg(CWnd* pParent = NULL);   // standard constructor
	~CDgnStlRatingLcomAASHTODlg();
// Dialog Data
	//{{AFX_DATA(CDgnStlRatingLcomAASHTODlg)
	enum { IDD = IDD_DGN_STL_RATING_LCOM_AASHTO_DLG };
	
	int		    m_nLimitState;
	MComboBox	m_cmbPrimary;
	MComboBox	m_cmbAdjacent;
	MComboBox	m_cmbFatigue;
	CEditUnit m_edtPrimaryFactor;
	CEditUnit m_edtAdjacentFactor;
	int       m_nEvaluation;
	int       m_nEvaluation19;
	int       m_nEvaluation19Fat;
	int       m_nEvalumethod;
	CString	  m_strCaseName;
	CString	  m_strDescription;
	CListCtrl	m_List;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlRatingLcomAASHTODlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	CDBDoc* m_pDoc;
	
	T_RLCS_K m_Key;
	T_RLCS_D m_Data;
	T_RCDS_D m_RcdsD;

	int Rlcskey;
	
	CArray<T_MVLD_K,T_MVLD_K> m_arMoveLoadKey;
	
	CDgnStlRatingLcomAASHTOMainGrid* m_pMainGrid;
	CDgnStlRatingLcomAASHTOSubGrid*  m_pSubGrid;
	CArray<UINT, UINT> m_aEVLcontrols, m_aEVLcontrols19;
	CArray<UINT, UINT> m_aLiveControls, m_aLiveControls19;

	void InitCtrl();
	void InitUnit();
	void InitRlcsKey();
	void InitCurrentData();
	void InitMoveLoadCombo();	
	void SetListCtrlHeader();

	BOOL InsertListItem(T_RLCS_K Key);
	BOOL ModifyListItem(T_RLCS_K Key);
	BOOL DeleteListItem(T_RLCS_K Key);
	int  FindInsertionPos(T_RLCS_K nID);
	CString DataToStr(int i, T_RLCS_K Key, T_RLCS_D &Data);
	void ChangeItem(int nIndex);
	void GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem);
	void ShowhideGrid();
	void AlignControls();

	void Data2Dlg();
	void Dlg2Data();
	
	int  GetMoveLoadKeyIndex(T_MVLD_K MvKey);
	BOOL RatingCaseNameCheck(CString strName);

public:
	void EnableDisableSubGrid(int nRow);
	
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnStlRatingLcomAASHTODlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnStlRatingLcomLimitStateRdo();
	afx_msg void OnDgnStlRatingLcomLiveFactorBtn();
	afx_msg void OnDgnStlRatingLcomAddBtn();
	afx_msg void OnDgnStlRatingLcomModBtn();
	afx_msg void OnDgnStlRatingLcomDelBtn();
	afx_msg void OnDgnStlRatingLcomCopyBtn();
	afx_msg void OnDgnStlRatingLcomClose();

	afx_msg void OnOnItemchangedDgnStlRatingLcomList(NMHDR* pNMHDR, LRESULT* pResult);	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNSTLRATINGLCOMAASHTODLG_H__)
