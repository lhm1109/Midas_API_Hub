#if !defined(__CMMVLDITEMPOLANDDLG_H__)
#define __CMMVLDITEMPOLANDDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldItemPolandDlg.h : header file
//

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemPolandDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvldItemPolandDlg : public CDialogMove
{
// Construction
public:
	CCMMvldItemPolandDlg(CWnd* pParent = NULL);   // standard constructor

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
	void AlignControl();
	void ShowHideControls();
	void EnableDisableControls();
	void ChangeText();
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

protected:
	CDBDoc*   m_pDoc;
	T_MVLD_D  m_Data;
	CString   m_csOldName;
	BOOL      m_bModify;
	BOOL      m_bSurface;
	
	CArray<UINT, UINT> m_aLoadModel;
	CArray<UINT, UINT> m_aVehicle;
	CArray<UINT, UINT> m_aAssignLane;
	CArray<UINT, UINT> m_aSubCase;
	CArray<UINT, UINT> m_aPermit;
	CArray<UINT, UINT> m_aMinDist;
	CArray<UINT, UINT> m_aOptim1;
	CArray<UINT, UINT> m_aOptim2;
	CArray<UINT, UINT> m_aOkCancel;	

	CArray<UINT, UINT> m_aLaneList;
	CArray<BOOL, BOOL> m_aSelFlag;

public:
// Dialog Data
	//{{AFX_DATA(CCMMvldItemPolandDlg)
	enum { IDD = IDD_CMD_ML_MVLD_ITEM_POLAND };

	MEdit     m_edtLoadCase;
	MEdit     m_edtDesc;
	BOOL      m_bPermit;
	BOOL      m_bAutoOptim;
	int       m_nLoadModel;
	MComboBox m_cmbVehicle;
	CListBox  m_lstUnSel;
	CListBox  m_lstSel;
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
	MComboBox m_cmbOptimLane2;
	CEditUnit m_edtNumLoadLanes;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldItemPolandDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

	// Generated message map functions
	//{{AFX_MSG(CCMMvldItemPolandDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdApply();
	afx_msg void OnCmdPermitChk();
	afx_msg void OnCmdAutoOptimChk();
	afx_msg void OnCmdLoadModelRdo();
	afx_msg void OnCmdLaneAddBtn();
	afx_msg void OnCmdLaneDelBtn();
	afx_msg void OnCmdSubCaseAddBtn();
	afx_msg void OnCmdSubCaseModBtn();
	afx_msg void OnCmdSubCaseDelBtn();
	afx_msg void OnCmdAssignVehlAddBtn();
	afx_msg void OnCmdAssignVehlModBtn();
	afx_msg void OnCmdAssignVehlDelBtn();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnDblclkCmdUnSelList();
	afx_msg void OnDblclkCmdSelList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVLDITEMPOLANDDLG_H__)
