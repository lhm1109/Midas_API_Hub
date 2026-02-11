#if !defined(__CMMVLDITEMBSDLG_H__)
#define __CMMVLDITEMBSDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldItemIDDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemBSDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvldItemBSDlg : public CDialogMove
{
// Construction
public:
	CCMMvldItemBSDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_MVLDbs_D &ParamData) 
	{ m_csOldName = ParamData.LoadCaseName; m_Data = ParamData; m_bModify = TRUE; }
	BOOL OnApplySubDlg(BOOL bModify, T_MVLDbs_BASE& data, int nPos);
	BOOL IsAutoLiveLoadComb();
	BOOL IsNetworkRail();

//----------------------------------------------------------------------
// Implementation
protected:
	void InitUnit();
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL ApplyOrOK();
	void SetHeaderTitle();
	void MakeItemEx();
	BOOL InsertItem(int nIndex, T_MVLDbs_BASE &Data);
	BOOL DeleteItem(int nIndex, T_MVLDbs_BASE &Data);
	BOOL ModifyItem(int nIndex, T_MVLDbs_BASE &Data);
	CString DataToStr(int i, T_MVLDbs_BASE &Data);
	CString GetLaneName(UINT nLaneKey);
	CString GetMvhlName(UINT nMvhlKey);
	void EnableDisableCtrl();
	void InitRcAr();  
	void SetCtrlShowHide();
	void AlignControl();
	void AlignControlDynamic();
	void SetAutoLiveState(int nLoadModel);
	void InitCombo();
	void Data2Dlg_LaneListByVehicleIndex();
	void LoadModel_LaneListByVehicleIndex();

	
	BOOL IsDuplicatedItem(T_LLAN_EURO_NEW_KEY& key);
	BOOL IsStraddlingLane(T_LLAN_K LaneK);

	void SetHeaderTitle_AssignVehl();
	void MakeItemEx_AssignVehl();
	BOOL InsertItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data);
	BOOL DeleteItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data);
	BOOL ModifyItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data);
	CString DataToStr_AssignVehl(int i, T_MVLD_OPTIMIZE &Data);
	void CmdSubBtnDeleteSpecial();
	void CmdSubBtnDeleteLM1();
protected:
	CDBDoc*     m_pDoc;
	T_MVLDbs_D  m_Data;
	CString     m_csOldName;
	BOOL        m_bModify;
	BOOL        m_bSurface;

	CArray<UINT, UINT> m_aCtrlRadio, m_aCtrlCombRdo, m_aCtrlLoadModelRdo/*m_aCtrlLM1Hide,*/, m_aCtrlLM1SpecialHide, m_aCtrlLM1SpecialHideStradd;  
	CArray<UINT, UINT> m_aCtrlLoadCase, m_aCtrlSpecial, m_aCtrlStandard, m_aOKCancelApply;  
	CArray<UINT, UINT> /*m_aCtrlLoadLM1Case,*/ m_aCtrlAreaLane, m_aCtrlSpecialSub;
	CArray<UINT, UINT> m_aMinDist;
	CArray<UINT, UINT> m_aOptimCtrl;
	CArray<UINT, UINT> m_aOptimAssignLane, m_aOptimAssignLaneSub;
	CArray<UINT, UINT> m_m_aCtrNetworkRailHide;

	CArray<UINT, UINT> m_m_aCtrTrackFactor;

	CArray<UINT, UINT> m_aLaneList;
	CArray<BOOL, BOOL> m_aSelFlag;
	CArray<BOOL, BOOL> m_aAreaFlag;
	CArray<T_LLAN_EURO_NEW_KEY, T_LLAN_EURO_NEW_KEY> m_aSelLlan;
	
public:
// Dialog Data
	//{{AFX_DATA(CCMMvldItemBSDlg)
	enum { IDD = IDD_CMD_ML_MVLD_ITEM_BS };
	MEdit	m_wndDesc;
	CListCtrl	m_List;
	MEdit	m_wndLoadCase;
	MButton m_chkAutoLoadComb;
	MComboBox m_cbxStandardLoadVehi;
	MComboBox m_cbxSpecialLoadVehi;
	int		m_nCombinationRa2;
	int   m_nLoadingEffect;

	CListBox	m_wndUnselList;
	CListBox	m_wndSelList;
	CListBox	m_lstStrad;
	CListBox	m_lstStradLM1;
	CListBox	m_lstRemain;
	CListBox    m_lstOptimUnsel;
	CListBox    m_lstOptimRemain;

	BOOL    	m_bAutoOptim;
	BOOL    	m_bOptimRemainArea;
	CEditUnit m_edtMinVehlDist;
	CTextUnit m_untMinVehlDist;
	MComboBox m_cmbOptimLane;
	CEditUnit m_edtNumLoadLanes;
	int       m_nCombOption2;
	MComboBox m_cmbSelVehl;
	CListCtrl	m_AssignVehlList;
	CEditUnit m_edtScaleFactorOptim;
	MComboBox m_cmbOptimLane2;
	CEditUnit m_edtNumLoadLanes2;

	CEditUnit m_edtTrackFactor1;
	CEditUnit m_edtTrackFactor2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldItemBSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//CArray<UINT, UINT> m_aCombOption;

	// Generated message map functions
	//{{AFX_MSG(CCMMvldItemBSDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdApply();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickAutoLiveChk();
	afx_msg void OnSelLoadModelRdo();
	afx_msg void OnDblclkCmdSubUnselList();
	afx_msg void OnDblclkCmdSubSelList();
	afx_msg void OnDblclkCmdSubStradList();
	afx_msg void OnDblclkCmdSubStradListLM1();
	afx_msg void OnDblclkCmdSubRemainArea();
	afx_msg void OnDblclkCmdSubOptimRemainList();
	afx_msg void OnCmdSubBtnAdd();
	afx_msg void OnCmdSubBtnDelete();
	afx_msg void OnCmdSubStradAddBtn();
	afx_msg void OnCmdSubStradDelBtn();
	afx_msg void OnCmdSubRemainAreaAddBtn();
	afx_msg void OnCmdSubRemainAreaDelBtn();
	afx_msg void OnCmdSubBtnAdd2();
	afx_msg void OnCmdSubOptimRemainAddBtn();
	afx_msg void OnCmdSubOptimRemainDelBtn();
	afx_msg void OnCmdOptmChk();
	afx_msg void OnCmdBtnAdd_AssignVehl();
	afx_msg void OnCmdBtnModify_AssignVehl();
	afx_msg void OnCmdBtnDelete_AssignVehl();
	//afx_msg void OnSelChangeVClass();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	


};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVLDITEMBSDLG_H__)
