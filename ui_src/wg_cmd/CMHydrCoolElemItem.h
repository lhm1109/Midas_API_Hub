#if !defined(AFX_CMHYDRCOOLELEMITEM_H__ACDCCD0E_16DE_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CMHYDRCOOLELEMITEM_H__ACDCCD0E_16DE_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMHydrCoolElemItem.h : header file
//

#include "CMDlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MEdit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCMHydrCoolElemItem dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMHydrCoolElemItem : public CCMDlgBase
{
// Construction
public:
	CCMHydrCoolElemItem(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Overrides
public:
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	void UpdateUnit();
//----------------------------------------------------------------------
// public Operation
public:
	int CalcTime(T_HSTG_K HstgK, int nTime) const ;
	void CalcStageTime(int nTotalTime, T_HSTG_K& rHstgK, int& rnTime) const;
//----------------------------------------------------------------------
// Interface
public:

//----------------------------------------------------------------------
// Implementation
protected:
	void AlignControl();
	void ResizeDialog();
	// Main List
	void SetHeaderTitle1();
	void MakeItemEx();
	BOOL InsertItem(T_HPCE_K Key, T_HPCE_D &Data);
	BOOL DeleteItem(T_HPCE_K Key, T_HPCE_D &Data);
	int  FindInsertionPos(int nID);
	CString DataToStr(int i, T_HPCE_K Key, T_HPCE_D &Data);
	// Cool List
	void InitItemDataVar();
	void InitGrid();
	void SetGridTitle();
	void InsertBaseItem(int nPos, int nType);
	BOOL Grid_IsExistKey(int nKey);
	void Grid_InsertItem(int nPos, T_HPCE_BASE &item);
	void SelectNode();
	BOOL Dlg2Data();
	void Data2Dlg();

protected:
	BOOL  m_bModify;
	BOOL  m_bListMode;
	CArray<UINT, UINT> m_aCtrlList;
	CArray<UINT, UINT> m_aCtrlData;
	CArray<UINT, UINT> m_aCtrlSelect;
	T_HPCE_D m_Data;

	CGXGridWnd m_wndGrid;
	CString m_csOldCElemName;

public:
// Dialog Data
	//{{AFX_DATA(CCMHydrCoolElemItem)
	enum { IDD = IDD_CMD_HYDR_COOL_ITEM };
	mit::frx::MComboBox	m_cobxEndStage;
	mit::frx::MComboBox	m_cobxStartStage;
	CTextUnit m_dConvectUnit;
	CTextUnit	m_dDiaUnit;
	CTextUnit	m_2PointEditUnit2;
	CTextUnit	m_2PointEditUnit1;
	CTextUnit	m_TimeUnit;
	CTextUnit	m_TimeUnit2;
	CTextUnit	m_dFRateUnit;
	CTextUnit	m_dInletUnit;
	CTextUnit	m_dDensityUnit;
	CTextUnit	m_dSHeatUnit;
	CEditUnit	m_dConvect;
	CEditUnit	m_dDia;
	CEditUnit	m_dFRate;
	CEditUnit	m_dInlet;
	CEditUnit	m_dDensity;
	CEditUnit	m_dSHeat;
	CFormulaEditSpin	m_ITEnd;
	CFormulaEditSpin	m_ITStart;
	//CSpinButtonCtrl	m_ITEndSpin;
	//CSpinButtonCtrl	m_ITStartSpin;
	CMouseEdit	m_wndPickingEdit;
	mit::frx::MEdit	m_wndNodeSelectEdit;
	CMouseEdit	m_wnd2PointsEdit2;
	CMouseEdit	m_wnd2PointsEdit1;
	mit::frx::MEdit	m_wndCoolName;
	CListCtrl	m_List;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMHydrCoolElemItem)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetStageCobx(BOOL bOnlyEnd=FALSE, int nStartIx=0);
	int m_nCurStartSel;

	// Generated message map functions
	//{{AFX_MSG(CCMHydrCoolElemItem)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdBtnClose();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnOk();
	afx_msg void OnCmdBtnCancel();
	afx_msg void OnCmdBtnApply();
	afx_msg void OnCmdChangeSelectBy();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdCoolAdd();
	afx_msg void OnCmdCoolDel();
	afx_msg void OnCmdCoolInsert();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnSelchangeCobxStartStage();
	//}}AFX_MSG
	afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMHYDRCOOLELEMITEM_H__ACDCCD0E_16DE_11D5_880F_00010263A1F4__INCLUDED_)
