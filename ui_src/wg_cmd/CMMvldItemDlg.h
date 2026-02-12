#if !defined(AFX_CMMVLDITEMDLG_H__8A601085_2359_11D4_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMMVLDITEMDLG_H__8A601085_2359_11D4_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldItemDlg.h : header file
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
// CCMMvldItemDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvldItemDlg : public CDialogMove
{
// Construction
public:
	CCMMvldItemDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_MVLD_D &ParamData) 
	{ m_csOldName = ParamData.LoadCaseName; m_Data = ParamData; m_bModify = TRUE; }
	BOOL OnApplySubDlg(BOOL bModify, T_MVLD_BASE& data, int nPos);

//----------------------------------------------------------------------
// Implementation
protected:
	void Data2Dlg();
	void Data2DlgOptm();
	BOOL Dlg2Data();
	BOOL Dlg2DataOptm();
	BOOL ApplyOrOK();
	void SetHeaderTitle();
	void SetHeaderTitle_AssignVehl();
	void MakeItemEx();
	BOOL InsertItem(int nIndex, T_MVLD_BASE &Data);
	BOOL DeleteItem(int nIndex, T_MVLD_BASE &Data);
	BOOL ModifyItem(int nIndex, T_MVLD_BASE &Data);
	CString DataToStr(int i, T_MVLD_BASE &Data);
	CString GetLaneName(UINT nLaneKey);
	void MyCtrlShowHideByRect(UINT nFrmID, BOOL bShow, BOOL bIncRectWnd);
	void ShowHideCtrl();
	void AlignControl();
	void ResizeDialog();
	void InitCtrl();  
	void InitCombo();
	void InitUnit();
	void InitUnitOptm();
	void MakeItemEx_AssignVehl();
	BOOL InsertItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data);
	BOOL DeleteItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data);
	BOOL ModifyItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data);
	CString DataToStr_AssignVehl(int i, T_MVLD_OPTIMIZE &Data);
	void ShowHideKSRailCtrl();
	void InitCtrlPos();
	void EnableCtrl();

protected:
	CDBDoc*   m_pDoc;
	T_MVLD_D  m_Data;
	CString   m_csOldName;
	BOOL      m_bModify;
	BOOL      m_bSurface;  
	BOOL m_bInitDlg; 
public:
// Dialog Data
	//{{AFX_DATA(CCMMvldItemDlg)
	enum { IDD = IDD_CMD_ML_MVLD_ITEM };
	CEditUnit	m_edtScaleFactor;
	CTextUnit	m_untEccen;
	CEditUnit	m_edtEccen;
	MComboBox	m_cmbRefLane;
	MComboBox	m_cmbVehicle;
	MButton	m_chkLCPV;
	MEdit	m_wndDesc;
	CListCtrl	m_List;
	MEdit	m_wndSF6;
	MEdit	m_wndSF5;
	MEdit	m_wndSF4;
	MEdit	m_wndSF3;
	MEdit	m_wndSF2;
	MEdit	m_wndSF1;
	MEdit	m_wndLoadCase;
	int       m_nLaneFactorType;
	CEditUnit m_edt2LaneFactor1;
	CEditUnit m_edt2LaneFactor2;
	CEditUnit m_edt3LaneFactor1;
	CEditUnit m_edt3LaneFactor2;
	CEditUnit m_edt3LaneFactor3;
	CEditUnit m_edt3LaneFactor4;

	// Optm
	MButton	m_chkAutoOptim;
	CEditUnit m_edtMinVehlDist;
	CTextUnit m_untMinVehlDist;
	MComboBox m_cmbOptimLane;
	CEditUnit m_edtMinNumVehl;
	CEditUnit m_edtMaxNumVehl;
	int   m_nCombOption2;
	MComboBox m_cmbSelVehl;
	CListCtrl	m_AssignVehlList;
	CEditUnit m_edtScaleFactorOptim;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<UINT, UINT> m_aCombOption;
	CArray<UINT, UINT> m_aCtrlLCPV;
	CArray<UINT, UINT> m_aCtrlBtn;
	CArray<UINT, UINT> m_aKSRail;
	CArray<UINT, UINT> m_aMPF;
	CArray<UINT, UINT> m_aOptimCtrl;
	CArray<UINT, UINT> m_aEtc;
	CArray<UINT, UINT> m_aFoldGroup;

	// Generated message map functions
	//{{AFX_MSG(CCMMvldItemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCmdApply();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdLcpvChk();
	afx_msg void OnCmdMvldLaneFactorTypeRdo();
	afx_msg void OnCmdOptmChk();
	afx_msg void OnCmdBtnAdd_AssignVehl();
	afx_msg void OnCmdBtnModify_AssignVehl();
	afx_msg void OnCmdBtnDelete_AssignVehl();

	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	


protected:
	CFoldGroupBox_Cross m_chkLoadFactor;






};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVLDITEMDLG_H__8A601085_2359_11D4_92DE_0000C0B0E6B3__INCLUDED_)
