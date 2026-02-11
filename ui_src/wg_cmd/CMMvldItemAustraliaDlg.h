#if !defined(__CMMVLDITEMAUSTRALIADLG_H__)
#define __CMMVLDITEMAUSTRALIADLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldItemAustraliaDlg.h : header file
//

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"

#include "..\wg_base\FoldGroupBox.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemAustraliaDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvldItemAustraliaDlg : public CDialogMove
{
// Construction
public:
	CCMMvldItemAustraliaDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMMvldItemAustraliaDlg();

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_MVLD_D &ParamData) 
	{ m_csOldName = ParamData.LoadCaseName; m_Data = ParamData; m_bModify = TRUE; }
	BOOL OnApplySubDlg(BOOL bModify, T_MVLD_BASE& data, int nPos);

//----------------------------------------------------------------------
// Implementation
protected:
	void InitCtrl();
	void InitUnit();
	void InitCombo();
	void InitComboheavy();
	void AlignControl();
	void AlignControl_Sub();

	void ShowHideControls();
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL ApplyOrOK();
	void SetHeaderTitle();
	void MakeItemEx();
	BOOL InsertItem(int nIndex, T_MVLD_BASE &Data);
	BOOL DeleteItem(int nIndex, T_MVLD_BASE &Data);
	BOOL ModifyItem(int nIndex, T_MVLD_BASE &Data);
	CString DataToStr(int i, T_MVLD_BASE &Data);
	CString GetLaneName(UINT nLaneKey);

	void SetHeaderTitle_AssignVehl();
	void MakeItemEx_AssignVehl();
	BOOL InsertItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data);
	BOOL DeleteItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data);
	BOOL ModifyItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data);
	CString DataToStr_AssignVehl(int i, T_MVLD_OPTIMIZE &Data);

	void Data2Dlg_LaneListByVehicleIndex();
	BOOL IsDuplicatedItem(T_LLAN_EURO_NEW_KEY& key);
	BOOL IsDuplicatedItemBD(T_LLAN_EURO_NEW_KEY& key);

	void CmdSubBDoubleAddBtn();
	void CmdSubBDoubleDelBtn();

protected:
	CDBDoc*   m_pDoc;
	T_MVLD_D  m_Data;
	CString   m_csOldName;
	BOOL      m_bModify;
	BOOL      m_bSurface;
	int       m_nCodeType;
	
	CArray<UINT, UINT> m_aFatigue;
	CArray<UINT, UINT> m_aModelType;
	CArray<UINT, UINT> m_aDgnComb;


	CArray<UINT, UINT> m_aLaneFactor;
	CArray<UINT, UINT> m_aLaneFactorAdd;
	CArray<UINT, UINT> m_aLaneFactorFold;
	CArray<UINT, UINT> m_aSubCase;
	CArray<UINT, UINT> m_aPermit;
	CArray<UINT, UINT> m_aOptim;
	CArray<UINT, UINT> m_aOptim01;
	CArray<UINT, UINT> m_aOptim02;

	CArray<UINT, UINT> m_aLaneFactor2;
	CArray<UINT, UINT> m_aLCase2;
	CArray<UINT, UINT> m_aAssinLane;
	CArray<UINT, UINT> m_aAssinLane01;
	CArray<UINT, UINT> m_aAssinLane02;
	CArray<UINT, UINT> m_aAssinLane03;

	CArray<UINT, UINT> m_aBDoubleCmb;
	CArray<UINT, UINT> m_aBDoubleList;

	CArray<UINT, UINT> m_aGroup2Cmb;
	CArray<UINT, UINT> m_aGroup2List;

	CArray<UINT, UINT> m_aOkCancel;	

	// Rdo
	CArray<UINT, UINT> m_aLoadModelRdo;
	CArray<UINT, UINT> m_aDgnCombRdo;


	CArray<UINT, UINT> m_aLaneList;
	CArray<BOOL, BOOL> m_aSelFlag;
	CArray<BOOL, BOOL> m_aBDFlag;
	CArray<T_LLAN_EURO_NEW_KEY, T_LLAN_EURO_NEW_KEY> m_aSelLlan; // heavy or BDouble

	CArray<UINT, UINT> m_aMultiLaneFactorList;
	CArray<UINT, UINT> m_aMultiLaneFactorFold;
public:
// Dialog Data
	//{{AFX_DATA(CCMMvldItemAustraliaDlg)
	enum { IDD = IDD_CMD_ML_MVLD_ITEM_AUSTRALIA };

	MEdit     m_edtLoadCase;
	MEdit     m_edtDesc;
	BOOL      m_bPermit;
	BOOL      m_bAutoOptim;
	BOOL      m_bFatigue;
	CEditUnit m_edtLaneFact1;
	CEditUnit m_edtLaneFact2;
	CEditUnit m_edtLaneFact3;
	CEditUnit m_edtLaneFact4;
	CEditUnit m_edtLaneFact5;
	int       m_nCombOption;
	CListCtrl m_lstSubCase;
	MComboBox m_cmbPermit;
	MComboBox m_cmbRefLane;
	CEditUnit m_edtEccen;
	CTextUnit m_untEccen;
	CEditUnit m_edtScaleFactor;
	CEditUnit m_edtMinVehlDist;
	CTextUnit m_untMinVehlDist;
	MComboBox m_cmbOptimLane;
	CEditUnit m_edtMinNumVehl;
	CEditUnit m_edtMaxNumVehl;
	int       m_nCombOption2;
	MComboBox m_cmbSelVehl;
	CEditUnit m_edtScaleFactorOptim;
	CListCtrl	m_lstAssignVehl;

	CEditUnit m_edtLaneFact_MS1600;
	CEditUnit m_edtMinNumLane;
	CEditUnit m_edtMaxNumLane;
	MComboBox m_cmbOptimLane2;

	MComboBox m_cbxHeavyLoadVehi;
	MComboBox m_cbxMS1600LoadVehi;
	MComboBox m_cbxDoubleVehi;

	CListBox	m_wndUnselList;
	CListBox	m_wndSelList;
	CListBox	m_lstHeavy;
	CListBox	m_lstDouble;

	CEditUnit m_edtMultiLaneFact1;
	CEditUnit m_edtMultiLaneFact2;
	CEditUnit m_edtMultiLaneFact3;
	CEditUnit m_edtMultiLaneFact4;
	CEditUnit m_edtMultiLaneFact5;
	CEditUnit m_edtMultiLaneFact6;

	CFoldGroupBox_Cross m_chkLoadModel;
	CFoldGroupBox_Cross m_chkMultiLaneFactor;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldItemAustraliaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

	// Generated message map functions
	//{{AFX_MSG(CCMMvldItemAustraliaDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCmdApply();
	afx_msg void OnCmdPermitChk();
	afx_msg void OnCmdAutoOptimChk();
	afx_msg void OnSelLoadModelRdo();
	afx_msg void OnCmdSubCaseAddBtn();
	afx_msg void OnCmdSubCaseModBtn();
	afx_msg void OnCmdSubCaseDelBtn();
	afx_msg void OnCmdAssignVehlAddBtn();
	afx_msg void OnCmdAssignVehlModBtn();
	afx_msg void OnCmdAssignVehlDelBtn();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnSelChangeVClass();

    afx_msg void OnDblclkCmdSubUnselList();
	afx_msg void OnDblclkCmdSubHeavyList();
	afx_msg void OnCmdSubBtnAdd();
	afx_msg void OnCmdSubBtnDelete();
	afx_msg void OnCmdSubHeavyAddBtn();
	afx_msg void OnCmdSubHeavyDelBtn();

	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVLDITEMAUSTRALIADLG_H__)
