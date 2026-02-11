#if !defined(__CMMVLDITEMSOUTHAFRICADLG_H__)
#define __CMMVLDITEMSOUTHAFRICADLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldItemSouthAfricaDlg.h : header file
//

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemSouthAfricaDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvldItemSouthAfricaDlg : public CDialogMove
{
// Construction
public:
	CCMMvldItemSouthAfricaDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_MVLD_D &ParamData) 
	{ m_csOldName = ParamData.LoadCaseName; m_Data = ParamData; m_bModify = TRUE; }
	BOOL OnApplySubDlg(BOOL bModify, T_MVLD_BASE& data, int nPos);

//----------------------------------------------------------------------
// Implementation
protected:
	void InitUnit();
	void InitCombo();
	void ChangeVehiCombo();
	void AlignControl();
	void ShowHideControls();
	void EnableDisableControls();
	void ChangeText();
	void ChangeLaneList();
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

	BOOL IsNAVehicle(T_MVHL_D& Data);
	BOOL IsNBVehicle(T_MVHL_D& Data);
	BOOL IsNCVehicle(T_MVHL_D& Data);

protected:
	CDBDoc*   m_pDoc;
	T_MVLD_D  m_Data;
	CString   m_csOldName;
	BOOL      m_bModify;
	BOOL      m_bSurface;

	CArray<UINT, UINT> m_aVehicle;
	CArray<UINT, UINT> m_aUser;
	CArray<UINT, UINT> m_aNotUser;
	CArray<UINT, UINT> m_aSelLane;
	CArray<UINT, UINT> m_aNANCLane;
	CArray<UINT, UINT> m_aMinDist;
	CArray<UINT, UINT> m_aOptimUser;
	CArray<UINT, UINT> m_aNotOptimUser;
	CArray<UINT, UINT> m_aOkCancel;	

	CArray<UINT, UINT> m_aLaneList;
	CArray<BOOL, BOOL> m_aSelFlag;
	CArray<BOOL, BOOL> m_aNCSelFlag;
	CArray<BOOL, BOOL> m_aNASelFlag;	

public:
// Dialog Data
	//{{AFX_DATA(CCMMvldItemSouthAfricaDlg)
	enum { IDD = IDD_CMD_ML_MVLD_ITEM_SOUTH_AFRICA };

	MEdit     m_edtLoadCase;
	MEdit     m_edtDesc;
	BOOL      m_bAutoOptim;
	int       m_nLoadModel;
	MComboBox m_cmbVehicle1;
	MComboBox m_cmbVehicle2;
	CListBox  m_lstUnSel;
	CListBox  m_lstSel;
	CListBox  m_lstNCSel;
	CListBox  m_lstNASel;
	int       m_nCombOption;
	CListCtrl m_lstSubCase;
	CEditUnit m_edtMinVehlDist;
	CTextUnit m_untMinVehlDist;
	MComboBox m_cmbOptimLane;
	CEditUnit m_edtMinNumVehl;
	CEditUnit m_edtMaxNumVehl;
	int       m_nCombOption2;
	MComboBox m_cmbSelVehl;
	CEditUnit m_edtScaleFactorOptim;
	CListCtrl	m_lstAssignVehl;
	MComboBox m_cmbOptimLane2;
	CEditUnit m_edtNumLoadLanes;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldItemSouthAfricaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

	// Generated message map functions
	//{{AFX_MSG(CCMMvldItemSouthAfricaDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdApply();
	afx_msg void OnCmdAutoOptimChk();
	afx_msg void OnCmdLoadModelRdo();
	afx_msg void OnCmdLaneAddBtn();
	afx_msg void OnCmdLaneDelBtn();
	afx_msg void OnCmdNCLaneAddBtn();
	afx_msg void OnCmdNCLaneDelBtn();
	afx_msg void OnCmdNALaneAddBtn();
	afx_msg void OnCmdNALaneDelBtn();
	afx_msg void OnCmdSubCaseAddBtn();
	afx_msg void OnCmdSubCaseModBtn();
	afx_msg void OnCmdSubCaseDelBtn();
	afx_msg void OnCmdAssignVehlAddBtn();
	afx_msg void OnCmdAssignVehlModBtn();
	afx_msg void OnCmdAssignVehlDelBtn();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnDblclkCmdUnSelList();
	afx_msg void OnDblclkCmdSelList();
	afx_msg void OnDblclkCmdNCSelList();
	afx_msg void OnDblclkCmdNASelList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVLDITEMSOUTHAFRICADLG_H__)
