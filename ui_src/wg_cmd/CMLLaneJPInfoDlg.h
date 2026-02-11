#if !defined(AFX_CMMVCTJPINFODLG_H__159F1004_0F97_4134_A81F_0A0EAEC59347__INCLUDED_)
#define AFX_CMMVCTJPINFODLG_H__159F1004_0F97_4134_A81F_0A0EAEC59347__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvctJPInfoDlg.h : header file
//

#include "CMDlgBase.h"
//#include "..\wg_db\wg_db_DlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMLLaneJPInfoDlg dialog
struct T_LLANjp_BASE_OLD;
class  CCMLLaneJPDlg ;
class CCMLLaneJPInfoDlg : public CCMDlgBase
{
// Construction
	CCMLLaneJPDlg *m_pParentDlg;
	struct T_LLANjp_BASE_OLD*m_pLLAND;
	
public:
	void Data2Dlg(int * npOP , T_LLANjp_BASE_OLD* pLLAND, CCMLLaneJPDlg* pParentDlg);
	BOOL Dlg2Data(T_LLANjp_BASE_OLD* pLLAND);

	CCMLLaneJPInfoDlg** m_ppMySelf;
	int * m_pnOP; // (0) Add  (1) Modify
	CCMLLaneJPInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMLLaneJPInfoDlg();
	void MakeListHeader();
	void CtrlMan();
	void SelectCtrlMan();

	BOOL AddOp(int nAddOrInsert);
	BOOL ModifyOp();
	BOOL DelOp();
	BOOL InsertOp();
	BOOL IsValidElem(CArray<unsigned int, unsigned int> & aElemKey);
	// nAddOrInsert (0) Add (1) Insert
	BOOL AddElemToList(CArray<unsigned int, unsigned int> & aElemKey,int nAddOrInsert);
	void SelectElem();
	BOOL IsRelevantCode(int nCode);
	void ShowHideByCode();
	void ButtonCtrl();

	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

// Dialog Data
	//{{AFX_DATA(CCMLLaneJPInfoDlg)
	enum { IDD = IDD_CMD_ML_LLAN_INFO_JAPAN };
	
	MButton	m_wndModifyBtn;
	CEditUnit	m_wndLength;
	CEditUnit	m_wndP1Edit;
	CEditUnit	m_wndP1Edit2;
	CEditUnit	m_wndP2Edit;
	CEditUnit	m_wndCrowdLoad;
	CEditUnit	m_wndTLoadEdit;
	CEditUnit	m_wndImpactCoeff;
	CEditUnit	m_wndExtraCoeff;
	CTextUnit	m_wndP1Unit;
	CTextUnit	m_wndP1Unit2;
	CTextUnit	m_wndP2Unit;
	CTextUnit	m_CrowdUnit;
	CTextUnit	m_wndTLoadUnit;
	CTextUnit	m_wndLengthUnit;
	CMouseEdit	m_wndSelBy1;
	CMouseEdit	m_wndSelBy2;
	CTextUnit	m_wndSelByUnit1;
	CTextUnit	m_wndSelByUnit2;
	int		m_nSelectBy;
	BOOL	m_bP1Load;
	BOOL	m_bP2Load;
	BOOL	m_bCrowdLoad;
	BOOL	m_bTLoadCheck;
	BOOL	m_bExtraCoeff;
	BOOL	m_bImpactCoeff;
	CListCtrl	m_wndElemList;
	CString	m_strName;
	BOOL	m_bP1Load2;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLLaneJPInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMLLaneJPInfoDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnCmdCrowdloadCheck();
	afx_msg void OnCmdExtracoeffCheck();
	afx_msg void OnCmdImpactcoeffCheck();
	afx_msg void OnCmdP1loadCheck();
	afx_msg void OnCmdP2loadCheck();
	afx_msg void OnCmdTloadCheck();
	afx_msg void OnSelectByRadio();
	afx_msg void OnCmdAddButton();
	afx_msg void OnCmdDeleteButton();
	afx_msg void OnCmdModifyButton();
	afx_msg void OnCmdP1loadCheck2();
	afx_msg void OnCmdApply();
	//}}AFX_MSG
	afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEnterKeyPressed (WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVCTJPINFODLG_H__159F1004_0F97_4134_A81F_0A0EAEC59347__INCLUDED_)
