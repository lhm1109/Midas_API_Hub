#if !defined(AFX_CMSTAGEITEMLOADDLG_H__AAB2555E_4C20_497F_9E6A_85848B23156E__INCLUDED_)
#define AFX_CMSTAGEITEMLOADDLG_H__AAB2555E_4C20_497F_9E6A_85848B23156E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMStageItemLoadDlg.h : header file
//
#include "CMStageItemBase.h"
#include "../MIT_frx/MComboBox.h"
/////////////////////////////////////////////////////////////////////////////
// CCMStageItemLoadDlg dialog
struct T_STAG_D;
class CCMStageItemLoadDlg : public CCMStageItemBase 
{
// Construction
	T_STAG_D *m_pCurStageD;
	double m_Age;
public:
	void SetCurStageData(T_STAG_D* pStageD);
	void UpdateGroupList();
	void SetListCtrlHeader();
	CCMStageItemLoadDlg(CWnd* pParent = NULL);   // standard constructor
	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	void _AddFunc(int nWhat); // (0) Act Add (1) Deact Add
	void UpdateGroupListByCurState();  // Active List Deactive List 설정후에 호출할것...
	void ResetParam();
	void SetDayCombo();
	BOOL GetActAge  (double& dAge,CString& StrAge);
	BOOL GetDeactAge(double& dAge, CString& StrAge);

// Dialog Data
	//{{AFX_DATA(CCMStageItemLoadDlg)
	enum { IDD = IDD_CMD_DEF_STAGE_ITEM_LOAD };
	mit::frx::MComboBox	m_wndDeactDayCombo;
	mit::frx::MComboBox	m_wndActDayCombo;
	CListBox	m_wndGroupList;
	CListCtrl	m_DeactiveGroupList;
	CListCtrl	m_ActiveGroupList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMStageItemLoadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
// Implementation
protected:
	void GetLoadGroupHavingFloorOrPlaneLoad(CMapEx<UINT, UINT, int, int>& rmLdgrK);

	// Generated message map functions
	//{{AFX_MSG(CCMStageItemLoadDlg)
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

#endif // !defined(AFX_CMSTAGEITEMLOADDLG_H__AAB2555E_4C20_497F_9E6A_85848B23156E__INCLUDED_)
