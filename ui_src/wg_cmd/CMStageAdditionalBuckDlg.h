#if !defined(__CMStageAdditionalBuckDlg_H__)
#define __CMStageAdditionalBuckDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMBuckDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxSpfc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;

#include "CMDlgBarBase.h"
/////////////////////////////////////////////////////////////////////////////
// CCMStageAdditionalBuckDlg form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMStageAdditionalBuckDlg : public CDialogMove, public CDBUpdateConnector
{
public:
	CCMStageAdditionalBuckDlg(CWnd* pParent = NULL); // protected constructor used by dynamic creation
	virtual ~CCMStageAdditionalBuckDlg();
	//DECLARE_DYNCREATE(CCMStageAdditionalBuckDlg)

// Form Data
public:
	//{{AFX_DATA(CCMStageAdditionalBuckDlg)
	enum { IDD = IDD_CMD_DEF_STAGE_ADDITIONAL_BUCK_DLG };

	BOOL		m_bInitAdd;
	BOOL        m_bBucklingChk;

	CSelectLC	m_LoadCase;		// CS type 을 포함해서 처리할 수 있나?
	CSelectLC	m_LoadStageCase;

	MComboBox	m_cmbLoadCase;

	//CSelectLC	m_CSCase;
	CListCtrl	m_List;
	MEdit	m_wndScaleFactor;
	MEdit	m_wndConvTol;
	CFormulaEditSpin 	m_wndIterEdit;
	CFormulaEditSpin 	m_wndBuckNumEdit;
	int   m_nLoadCategory;
	int   m_nLoadType;
	BOOL  m_bPositive;
	MButton  m_bAxialForce;
	MButton  m_bSturm;
	CEditUnit m_wndSearchFrom;
	CEditUnit m_wndSearchTo;
	//}}AFX_DATA

// Attributes
public:
	CDBDoc* m_pDoc;
	T_CSBK_D m_Data;

	T_CSBK_K m_CSBuckKey;

	// Operations
public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CCMStageAdditionalBuckDlg)
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	// Implementation
protected:
	CArray<UINT, UINT> m_aLoadFactor;
	CArray<UINT, UINT> m_aBuckComb;
	CArray<UINT, UINT> m_aCategoryComb;

	CArray<T_STLD_K, T_STLD_K> m_aLoadCaseKeys;
	CArray<int, int> m_aLoadType;

protected:
	void SetListCtrlHeader();
	BOOL GetSelectedItem(int& nIndex);
	void MakeItemEx();
	CString DataToStr(int i, T_CSBK_BASE& data);
	void SetItem(int nIndex, T_CSBK_BASE& data);
	BOOL ValidItem(T_CSBK_BASE& data);
	BOOL Dlg2Item(T_CSBK_BASE& data);
	BOOL Item2Dlg(T_CSBK_BASE& data);
	void EnableDisableCtrl();

	void Data2Dlg();
	void Dlg2Data();

	void InitializeLoadCaseCombo();
	void GetStageTypeKey(CArray<T_STLD_K, T_STLD_K>& aKeys);
	void GetStageTypeKeySumDL(CArray<T_STLD_K, T_STLD_K>& aKeys);
	void GetStageTypeKeyErection(CArray<T_STLD_K, T_STLD_K>& aKeys);
	BOOL GetSelectedLoadCaseKey(T_STLD_K& outKey, int& nCSType);
	BOOL SelectLoadCaseByKey(T_STLD_K key, int nCSType);

	// Generated message map functions
	//{{AFX_MSG(CCMStageAdditionalBuckDlg)
	afx_msg void OnCmdApply();
	afx_msg void OnCmdClose();
	//afx_msg void OnClickLoadCaseBtn();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdRemoveData();
	afx_msg void OnCmdLoadFactor();
	//afx_msg void OnCmdCategoryChanged();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMBUCKDLG_H__)
