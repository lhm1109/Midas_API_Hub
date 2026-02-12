#if !defined(AFX_CMMVLDITEMEUROBSNEWDLG_H__FCDD579C_3CD4_4417_9AA8_27DF2BB8DA39__INCLUDED_)
#define AFX_CMMVLDITEMEUROBSNEWDLG_H__FCDD579C_3CD4_4417_9AA8_27DF2BB8DA39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldItemEuroBSNewDlg.h : header file
//

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemEuroBSNewDlg dialog
#include "HeaderPre.h"

#define COLCOUNT 8

class __MY_EXT_CLASS__ CCMMvldItemEuroBSNewDlg : public CDialogMove
{
// Construction
public:
	CCMMvldItemEuroBSNewDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_MVLD_D &ParamData) 
	{ m_csOldName = ParamData.LoadCaseName; m_Data = ParamData; m_bModify = TRUE;}
	BOOL OnApplySubDlg(BOOL bModify, T_MVLD_BASE& data, int nPos);

//----------------------------------------------------------------------
// Implementation
public:
	void InitCombo();
	void AlignControls();
	void ShowHideControls();
	void ControlsEnableDisable();
	void ResizeWindow();
	void ChangeCtrlText();
	void MaintainVehicleIndex();
	void Data2Dlg();
	void Data2Dlg_LaneListByVehicleIndex();
	BOOL Dlg2Data();
	void SetHeaderTitle();
	void MakeItemEx();
	BOOL InsertItem(int nIndex, T_MVLD_BASE &Data);
	BOOL DeleteItem(int nIndex, T_MVLD_BASE &Data);
	BOOL ModifyItem(int nIndex, T_MVLD_BASE &Data);
	BOOL IsDuplicatedItem(T_LLAN_EURO_NEW_KEY& key);
	CString DataToStr(int i, T_MVLD_BASE &Data);
	CString GetLaneName(UINT nLaneKey);
	int  GetIndex(T_MVHL_D &MvhlD, int &nLoadType, int &nSelVehicle);
	BOOL ApplyOrOK();

	void SetHeaderTitle_AssignVehl();
	void MakeItemEx_AssignVehl();
	BOOL InsertItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data);
	BOOL DeleteItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data);
	BOOL ModifyItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data);
	CString DataToStr_AssignVehl(int i, T_MVLD_OPTIMIZE &Data);
	void AdjustComboListBox(MComboBox& Cbx);


	CDBDoc*   m_pDoc;

	T_MVLD_D  m_Data;
	CString   m_csOldName;
	BOOL      m_bModify;
	BOOL      m_bSurface;
	CArray<UINT, UINT> m_aLaneList;
	CArray<BOOL, BOOL> m_aSelFlag;
	CArray<BOOL, BOOL> m_aAreaFlag;
	CArray<BOOL, BOOL> m_aFootFlag;
	CArray<T_LLAN_EURO_NEW_KEY, T_LLAN_EURO_NEW_KEY> m_aSelLlan;
	int m_nLMSelected1;
	int m_nLMSelected3;
	int m_nLM3SpecialSel;
	int m_nPreLoadModel;


public:
// Dialog Data
	//{{AFX_DATA(CCMMvldItemEuroBSNewDlg)
	enum { IDD = IDD_CMD_ML_MVLD_ITEM_EURO_BS };
	
	MEdit	    m_wndLoadCase;
	MEdit	    m_wndDesc;
	MButton	  m_chkLeading;
	MComboBox	m_cmbLM1;
	MComboBox	m_cmbLM3;
	CListCtrl	m_List;
	CListBox	m_wndUnselList;
	CListBox	m_wndSelList;
	CListBox	m_wndAreaList; 
	MComboBox m_cmbLM3Special;
	CListBox	m_lstStrad;
	CListBox	m_lstRemain;
	CListBox  m_lstFootway;
	MComboBox m_cmbFootway;
	
	BOOL    	m_bAutoOptim;
	CEditUnit m_edtMinVehlDist;
	CTextUnit m_untMinVehlDist;
	MComboBox m_cmbOptimLane;
	CEditUnit m_edtMinNumVehl;
	CEditUnit m_edtMaxNumVehl;
	int       m_nCombOption2;
	MComboBox m_cmbSelVehl;
	CListCtrl	m_AssignVehlList;
	CEditUnit m_edtScaleFactorOptim;
	MComboBox m_cmbOptimLane2;
	CEditUnit m_edtNumLoadLanes;
	MComboBox m_cmbOptimLane3;
	CListBox	m_lstOptimUnsel;
	CListBox	m_lstOptimRemain;
	CListBox	m_lstOptimRemain2;
	CListBox	m_lstOptimFootway;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldItemEuroBSNewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<UINT, UINT> m_aLoadModelType;
	CArray<UINT, UINT> m_aSubLoadCase;
	CArray<UINT, UINT> m_aCombOption;
	CArray<UINT, UINT> m_aLM3Special;
	CArray<UINT, UINT> m_aStraddLanes;
	CArray<UINT, UINT> m_aRemainLanes;
	CArray<UINT, UINT> m_aFootway;
	CArray<UINT, UINT> m_aFootwayLanes;
	CArray<UINT, UINT> m_aTrainLoad;
	CArray<UINT, UINT> m_aEtc; // OK. Cancel, Apply

	CArray<UINT, UINT> m_aMinDist;
	CArray<UINT, UINT> m_aOptimCtrl;
	CArray<UINT, UINT> m_aOptimAssignLane;
	CArray<UINT, UINT> m_aOptimStraddLane;
	CArray<UINT, UINT> m_aOptimRemain;
	CArray<UINT, UINT> m_aOptimRemain2;
	CArray<UINT, UINT> m_aOptimFootway;

	// Generated message map functions
	//{{AFX_MSG(CCMMvldItemEuroBSNewDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdApply();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdSubBtnAdd();
	afx_msg void OnCmdSubBtnDelete();
	afx_msg void OnCmdSubBtnAdd2();
	afx_msg void OnCmdSubBtnDelete2();
	afx_msg void OnCmdSubStradAddBtn();
	afx_msg void OnCmdSubStradDelBtn();
	afx_msg void OnCmdSubReaminAddBtn();
	afx_msg void OnCmdSubReaminDelBtn();
	afx_msg void OnCmdSubFootwayAddBtn();
	afx_msg void OnCmdSubFootwayDelBtn();
	afx_msg void OnCmdSubOptimRemainAddBtn();
	afx_msg void OnCmdSubOptimRemainDelBtn();
	afx_msg void OnCmdSubOptimRemain2AddBtn();
	afx_msg void OnCmdSubOptimRemain2DelBtn();
	afx_msg void OnCmdSubOptimFootwayAddBtn();
	afx_msg void OnCmdSubOptimFootwayDelBtn();
	afx_msg void OnDblclkCmdSubSelList();
	afx_msg void OnDblclkCmdSubUnselList();
	afx_msg void OnDblclkCmdSubAreaList();
	afx_msg void OnDblclkCmdSubStradList();
	afx_msg void OnDblclkCmdSubRemainList();
	afx_msg void OnDblclkCmdSubFootwayList();
	afx_msg void OnDblclkCmdSubOptimRemainList();
	afx_msg void OnDblclkCmdSubOptimRemain2List();
	afx_msg void OnDblclkCmdSubOptimFootwayList();
	afx_msg void OnCmdLoadModelRdo();
	afx_msg void OnCmdOptmChk();
	afx_msg void OnCmdRailLoadDataBtn();
	afx_msg void OnCmdBtnAdd_AssignVehl();
	afx_msg void OnCmdBtnModify_AssignVehl();
	afx_msg void OnCmdBtnDelete_AssignVehl();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVLDITEMEUROBSNEWDLG_H__FCDD579C_3CD4_4417_9AA8_27DF2BB8DA39__INCLUDED_)
