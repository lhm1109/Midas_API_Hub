#if !defined(__CMMVLDITEMRUSDLG_H__)
#define __CMMVLDITEMRUSDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldItemRusDlg.h : header file
//

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemRusDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvldItemRusDlg : public CDialogMove
{
// Construction
public:
	CCMMvldItemRusDlg(CWnd* pParent = NULL);   // standard constructor

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

	CArray<UINT, UINT> m_aOptimCtrl;
	CArray<UINT, UINT> m_aNotOptimCtrl;
	CArray<UINT, UINT> m_aOkCancelCtrl;

public:
// Dialog Data
	//{{AFX_DATA(CCMMvldItemRusDlg)
	enum { IDD = IDD_CMD_ML_MVLD_ITEM_RUS };
	MEdit	m_wndLoadCase;
	MEdit	m_wndDesc;
	CListCtrl	m_List;
	int   m_nLcomType;
	int   m_nCombOption;
	BOOL  m_bAutoOptim;
	CEditUnit m_edtMinVehlDist;
	CTextUnit m_untMinVehlDist;
	MComboBox m_cmbOptimLane;
	CEditUnit m_edtMinNumVehl;
	CEditUnit m_edtMaxNumVehl;
	int   m_nCombOption2;
	MComboBox m_cmbSelVehl;
	CEditUnit m_edtScaleFactorOptim;
	CListCtrl	m_AssignVehlList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldItemRusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

	// Generated message map functions
	//{{AFX_MSG(CCMMvldItemRusDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdApply();
	afx_msg void OnCmdAutoOptimChk();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnAdd_AssignVehl();
	afx_msg void OnCmdBtnModify_AssignVehl();
	afx_msg void OnCmdBtnDelete_AssignVehl();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVLDITEMRUSDLG_H__)
