#if !defined(__CMMVLDITEMBRAZILDLG_H__)
#define __CMMVLDITEMBRAZILDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldItemBrazilDlg.h : header file
//

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemBrazilDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvldItemBrazilDlg : public CDialogMove
{
	// Construction
public:
	CCMMvldItemBrazilDlg(CWnd* pParent = NULL);   // standard constructor

	//----------------------------------------------------------------------
	// Interface
public:
	void SetParamData(T_MVLD_D& ParamData)
	{
		m_csOldName = ParamData.LoadCaseName; m_Data = ParamData; m_bModify = TRUE;
	}
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
	BOOL InsertItem(int nIndex, T_MVLD_BASE& Data);
	BOOL DeleteItem(int nIndex, T_MVLD_BASE& Data);
	BOOL ModifyItem(int nIndex, T_MVLD_BASE& Data);
	CString DataToStr(int i, T_MVLD_BASE& Data);
	CString GetLaneName(UINT nLaneKey);
	void MyCtrlShowHideByRect(UINT nFrmID, BOOL bShow, BOOL bIncRectWnd);
	void ShowHideCtrl();
	void AlignControl();
	void InitCombo();
	void InitUnit();
	void MakeItemEx_AssignVehl();
	BOOL InsertItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE& Data);
	BOOL DeleteItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE& Data);
	BOOL ModifyItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE& Data);
	CString DataToStr_AssignVehl(int i, T_MVLD_OPTIMIZE& Data);
	void Data2Dlg_LaneListByVehicleIndex();
	void InitSelList();
protected:
	CDBDoc* m_pDoc;
	T_MVLD_D  m_Data;
	CString   m_csOldName;
	BOOL      m_bModify;
	BOOL      m_bSurface;
	BOOL m_bInitDlg;

	CArray<UINT, UINT> m_aLaneList;
	CArray<BOOL, BOOL> m_aSelFlag;
	CArray<BOOL, BOOL> m_aFootSelFlag;
	CArray<BOOL, BOOL> m_aOptFootSelFlag;
public:
	// Dialog Data
		//{{AFX_DATA(CCMMvldItemBrazilDlg)
	enum { IDD = IDD_CMD_ML_MVLD_ITEM_BRAZIL };

	MEdit	m_wndLoadCase;
	MEdit	m_wndDesc;

	MButton	m_chkAutoOptim;

	MEdit	m_wndSF1;
	MEdit	m_wndSF2;
	MEdit	m_wndSF3;
	MEdit	m_wndSF4;

	MComboBox m_cmbSelRoadVehl;
	MComboBox m_cmbSelFootVehl;

	CListBox m_wndUnselList;
	CListBox m_wndSelList;
	CListBox m_wndFootSelList;

	CListCtrl	m_List;

	CEditUnit m_edtMinVehlDist;
	CTextUnit m_untMinVehlDist;

	MComboBox m_cmbOptimRailLane;

	CEditUnit m_edtMinNumVehl;
	CEditUnit m_edtMaxNumVehl;

	MComboBox m_cmbSelOptRailVehl;
	CEditUnit m_edtScaleFactorOptim;
	CListCtrl	m_AssignOptRailVehlList;

	MComboBox m_cmbOptimRoadLane;

	CEditUnit m_edtOptimRoadNumVehl;
	CListBox m_wndOptUnselList;
	CListBox m_wndOptFootSelList;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldItemBrazilDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CArray<UINT, UINT> m_aCtrlLaneFactor;
	CArray<UINT, UINT> m_aCtrlRoadVehicle;
	CArray<UINT, UINT> m_aCtrlRoadSubCase;
	CArray<UINT, UINT> m_aCtrlRailSubCase;
	CArray<UINT, UINT> m_aCtrl;
	CArray<UINT, UINT> m_aCtrlOptMinDist;
	CArray<UINT, UINT> m_aCtrlOptRail;
	CArray<UINT, UINT> m_aCtrlOptRoad;

	CArray<UINT, UINT> m_aLoadModelRdo;
	CArray<UINT, UINT> m_aCtrlRailComb;
	CArray<UINT, UINT> m_aCtrlOptRailComb;
	// Generated message map functions
	//{{AFX_MSG(CCMMvldItemBrazilDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdApply();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelLoadModelRdo();
	afx_msg void OnCmdOptmChk();
	afx_msg void OnCmdBtnAdd_AssignVehl();
	afx_msg void OnCmdBtnModify_AssignVehl();
	afx_msg void OnCmdBtnDelete_AssignVehl();

	afx_msg void OnCmdSubBtnAdd();
	afx_msg void OnCmdSubBtnDelete();
	afx_msg void OnCmdFootBtnAdd();
	afx_msg void OnCmdFootBtnDelete();
	afx_msg void OnCmdOptFootBtnAdd();
	afx_msg void OnCmdOptFootBtnDelete();

	afx_msg void OnCmdAssignVehlAddBtn();
	afx_msg void OnCmdAssignVehlModBtn();
	afx_msg void OnCmdAssignVehlDelBtn();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVLDITEMDLG_H__8A601085_2359_11D4_92DE_0000C0B0E6B3__INCLUDED_)
