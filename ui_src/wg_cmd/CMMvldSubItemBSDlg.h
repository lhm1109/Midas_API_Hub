#if !defined(__CMMVLDSUBITEMBSDLG_H__)
#define __CMMVLDSUBITEMBSDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldSubItemBSDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

#include "../MIT_frx/MEdit.h"
#include "../MIT_frx/MComboBox.h"

union T_LLAN_BS_KEY
{
	unsigned int keymap;
	struct
	{
		unsigned int key1 : 16;
		unsigned int key2 : 16;
	} key;
};

using namespace mit::frx;

class CCMMvldItemBSDlg;
/////////////////////////////////////////////////////////////////////////////
// CCMMvldSubItemBSDlg dialog

class CCMMvldSubItemBSDlg : public CDialogMove
{
// Construction
public:
	CCMMvldSubItemBSDlg(CWnd* pParent = NULL);   // standard constructor
	CCMMvldItemBSDlg* m_pMvldItemDlg;

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_MVLDbs_BASE &ParamData, int nPos) 
	{ m_Data = ParamData; m_nPos = nPos; m_bModify = TRUE; }

public:
	T_MVLDbs_BASE m_Data;
	int           m_nPos;

//----------------------------------------------------------------------
// Implementation
protected:
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL ApplyOrOK();
	void FillVClassList();
	void AlignControl();
	void SetCtrlShowHide();
	CString GetLaneName(UINT nLaneKey);
	BOOL IsDuplicatedItem(T_LLAN_BS_KEY& key);
	void DeleteItemFromSelList2(UINT LlanK);

protected:
	CDBDoc*     m_pDoc;
	BOOL        m_bModify;
	BOOL        m_bSurface;

	CArray<UINT, UINT> m_aLaneList;
	CArray<BOOL, BOOL> m_aSelFlag;
	CArray<T_LLAN_BS_KEY, T_LLAN_BS_KEY> m_aSelLlan2;

public:
// Dialog Data
	//{{AFX_DATA(CCMMvldSubItemBSDlg)
	enum { IDD = IDD_CMD_ML_MVLD_SITEM_BS };
	CListBox	m_wndUnselList;
	CListBox	m_wndSelList;
	CListBox	m_wndSelList2;
	MComboBox	m_wndVClass;
	CFormulaEditSpin   m_wndLaneNum;
	MEdit	    m_wndScaleFactor;

	CArray<UINT, UINT> aCtrlRailHide;
	CArray<UINT, UINT> m_aOKCancelApply;
	CArray<UINT, UINT> m_aUnSelLane;
	CArray<UINT, UINT> m_aSelLane;
	CArray<UINT, UINT> m_aSelLaneAddDel;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldSubItemBSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMMvldSubItemBSDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnDblclkCmdSelList();
	afx_msg void OnDblclkCmdUnselList();
	afx_msg void OnCmdBtnVclass();
	afx_msg void OnCmdApply();
	afx_msg void OnCmdBtnAdd2();
	afx_msg void OnCmdBtnDelete2();
	afx_msg void OnDblclkCmdSelList2();
	afx_msg void OnSelChangeVClass();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVLDSUBITEMBSDLG_H__)
