/*
#if !defined(AFX_CMMVLDITEMEUROBSDLG_H__FCDD579C_3CD4_4417_9AA8_27DF2BB8DA39__INCLUDED_)
#define AFX_CMMVLDITEMEUROBSDLG_H__FCDD579C_3CD4_4417_9AA8_27DF2BB8DA39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldItemEuroBSDlg.h : header file
//

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"

union T_LLAN_EURO_KEY
{
	unsigned int keymap;
	struct
	{
		unsigned int key1 : 16;
		unsigned int key2 : 16;
	} key;
};

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemEuroBSDlg dialog
#include "HeaderPre.h"

#define D_TYPE_COUNT 4 // code
#define D_TABLE_COUNT  3
#define D_MAX_COUNT  18
#define D_MAX_POINT_LOAD  D_MVHL_NUMLOAD

#define COLCOUNT 8

class __MY_EXT_CLASS__ CCMMvldItemEuroBSDlg : public CDialogMove
{
// Construction
public:
	CCMMvldItemEuroBSDlg(CWnd* pParent = NULL);   // standard constructor

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
	void ShowHideCtrl();
	void ResizeWindow();
	void ChangeCtrlText();
	void MaintainVehicleIndex();
	void Data2Dlg();
	BOOL Dlg2Data();
	void SetHeaderTitle();
	void MakeItemEx();
	BOOL InsertItem(int nIndex, T_MVLD_BASE &Data);
	BOOL DeleteItem(int nIndex, T_MVLD_BASE &Data);
	BOOL ModifyItem(int nIndex, T_MVLD_BASE &Data);
	BOOL IsDuplicatedItem(T_LLAN_EURO_KEY& key);
	CString DataToStr(int i, T_MVLD_BASE &Data);
	CString GetLaneName(UINT nLaneKey);
	int  GetIndex(CString &csName, int *sitem=NULL);
	BOOL ApplyOrOK();


	CDBDoc*   m_pDoc;

	T_MVLD_D  m_Data;
	CString   m_csOldName;
	BOOL      m_bModify;
	BOOL      m_bSurface;
	CArray<UINT, UINT> m_aLaneList;
	CArray<BOOL, BOOL> m_aSelFlag;
	CArray<BOOL, BOOL> m_aAreaFlag;
	CArray<T_LLAN_EURO_KEY, T_LLAN_EURO_KEY> m_aSelLlan;
	static CString m_aTypeList[D_TYPE_COUNT][D_MAX_COUNT];
	int m_nLMSelected1;
	int m_nLMSelected3;
	int m_nLM3SpecialSel;
	int m_nPreLoadModel;


public:
// Dialog Data
	//{{AFX_DATA(CCMMvldItemEuroBSDlg)
	enum { IDD = IDD_CMD_ML_MVLD_ITEM_EURO_BS };
	
	CEdit	    m_wndLoadCase;
	CEdit	    m_wndDesc;
	CButton	  m_chkLeading;
	CComboBox	m_cmbLM1;
	CComboBox	m_cmbLM3;
	CListCtrl	m_List;
	CListBox	m_wndUnselList;
	CListBox	m_wndSelList;
	CListBox	m_wndAreaList; 
	CComboBox m_cmbLM3Special;
	CListBox	m_lstStrad;
	CListBox	m_lstRemain;
	CEditUnit m_edtPsi1Fact1;
	CEditUnit m_edtPsi1Fact2;
	CEditUnit m_edtPsi1Fact3;
	BOOL      m_bPsi1Fact;
	CEditUnit m_edtMultiFact1;
	CEditUnit m_edtMultiFact2;
	CEditUnit m_edtMultiFact3;
	int       m_nTrainLoading;
	CListCtrl	m_TrainList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldItemEuroBSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<UINT, UINT> m_aLoadModelType;
	CArray<UINT, UINT> m_aSubLoadCase;
	CArray<UINT, UINT> m_aCombOption;
	CArray<UINT, UINT> m_aLM3Special;
	CArray<UINT, UINT> m_aLM3Lanes;
	CArray<UINT, UINT> m_aTrainLoad;
	CArray<UINT, UINT> m_aEtc; // OK. Cancel, Apply

	// Generated message map functions
	//{{AFX_MSG(CCMMvldItemEuroBSDlg)
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
	afx_msg void OnDblclkCmdSubSelList();
	afx_msg void OnDblclkCmdSubUnselList();
	afx_msg void OnDblclkCmdSubAreaList();
	afx_msg void OnDblclkCmdSubStradList();
	afx_msg void OnDblclkCmdSubRemainList();
	afx_msg void OnCmdLoadModelRdo();
	afx_msg void OnCmdIgnorePsi1FactorChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVLDITEMEUROBSDLG_H__FCDD579C_3CD4_4417_9AA8_27DF2BB8DA39__INCLUDED_)
*/