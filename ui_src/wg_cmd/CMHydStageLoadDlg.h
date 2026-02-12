#if !defined(AFX_CMHYDSTAGELOADDLG_H__157A2C69_40FB_4574_84DC_995B9118BD23__INCLUDED_)
#define AFX_CMHYDSTAGELOADDLG_H__157A2C69_40FB_4574_84DC_995B9118BD23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMHydStageLoadDlg.h : header file
//
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

#include "CMHydStageItemBase.h"
/////////////////////////////////////////////////////////////////////////////
// CCMHydStageLoadDlg dialog
struct T_HSTG_D;
class CCMHydStageLoadDlg : public CCMHydStageItemBase
{
// Construction
public:
	CCMHydStageLoadDlg(CWnd* pParent = NULL);   // standard constructor
	
	void SetCurStageData(T_HSTG_D* pStageD);
	void UpdateGroupList();
	void SetListCtrlHeader();
	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	void _AddFunc(int nWhat); // (0) Act Add (1) Deact Add
	void UpdateGroupListByCurState();
	void ResetParam();
	void SetDayCombo();
	BOOL GetActAge  (double& dAge,CString& StrAge);
	BOOL GetDeactAge(double& dAge, CString& StrAge);

	double m_Age;

// Dialog Data
	//{{AFX_DATA(CCMHydStageLoadDlg)
	enum { IDD = IDD_CMD_DEF_HYD_STAGE_ITEM_LOAD };
	MComboBox	m_wndDeactDayCombo;
	MComboBox	m_wndActDayCombo;
	CListBox	m_wndGroupList;
	CListCtrl	m_DeactiveGroupList;
	CListCtrl	m_ActiveGroupList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMHydStageLoadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMHydStageLoadDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdDefgroupButton();
	afx_msg void OnActAddButton();
	afx_msg void OnActDeleteButton();
	afx_msg void OnActModifyButton();
	afx_msg void OnDeactAdd();
	afx_msg void OnDeactDeleteButton();
	afx_msg void OnDeactModifyButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMHYDSTAGELOADDLG_H__157A2C69_40FB_4574_84DC_995B9118BD23__INCLUDED_)
