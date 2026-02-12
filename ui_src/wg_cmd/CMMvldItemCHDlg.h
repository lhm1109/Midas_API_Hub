#if !defined(__CMMVLDITEMCHDLG_H__)
#define __CMMVLDITEMCHDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldItemCHDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"
#include "CMGXGridWnd.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemCHDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvldItemCHDlg : public CDialogMove
{
// Construction
public:
	CCMMvldItemCHDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_MVLDch_D &ParamData) 
	{ m_csOldName = ParamData.LoadCaseName; m_Data = ParamData; m_bModify = TRUE; }
	BOOL OnApplySubDlg(BOOL bModify, T_MVLDch_BASE& data, int nPos);

//----------------------------------------------------------------------
// Implementation
protected:
	void InitCombo();
	void InitGrid();
	void AlignControl();
	void ShowHideControls();
	void SetGridTitle();
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL ApplyOrOK();
	void SetHeaderTitle();
	void MakeItemEx();
	BOOL InsertItem(int nIndex, T_MVLDch_BASE &Data);
	BOOL DeleteItem(int nIndex, T_MVLDch_BASE &Data);
	BOOL ModifyItem(int nIndex, T_MVLDch_BASE &Data);
	CString DataToStr(int i, T_MVLDch_BASE &Data);
	CString GetLaneName(UINT nLaneKey);

	void SetHeaderTitle_AssignVehl();
	void MakeItemEx_AssignVehl();
	BOOL InsertItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data);
	BOOL DeleteItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data);
	BOOL ModifyItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data);
	CString DataToStr_AssignVehl(int i, T_MVLD_OPTIMIZE &Data);

protected:
	CDBDoc*   m_pDoc;
	T_MVLDch_D  m_Data;
	CCMGXGridWnd  m_wndGrid; 
	CString   m_csOldName;
	BOOL      m_bModify;
	BOOL      m_bSurface;

	CArray<UINT, UINT> m_aOptimCtrl;
	CArray<UINT, UINT> m_aNotOptimCtrl;
	CArray<UINT, UINT> m_aOkCancelCtrl;

public:
// Dialog Data
	//{{AFX_DATA(CCMMvldItemCHDlg)
	enum { IDD = IDD_CMD_ML_MVLD_ITEM_CHINA };
	MEdit	m_wndDesc;
	MComboBox m_cmbBridgeType;
	CListCtrl	m_List;
	MEdit	m_wndLoadCase;
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
	//{{AFX_VIRTUAL(CCMMvldItemCHDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<UINT, UINT> m_aCombOption;

	// Generated message map functions
	//{{AFX_MSG(CCMMvldItemCHDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeBridgeTypeCmb();
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

#endif // !defined(AFX_CMMVLDITEMDLG_H__8A601085_2359_11D4_92DE_0000C0B0E6B3__INCLUDED_)
