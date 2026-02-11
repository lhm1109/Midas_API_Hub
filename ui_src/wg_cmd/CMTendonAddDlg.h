#if !defined(__CMTENDONADDDLG_H__)
#define      __CMTENDONADDDLG_H__

// Old Version : 5.1.0부터 CCMTendonProfileDlg.cpp로 변경

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMTendonAddDlg.h : header file
//
#include "MyListCtrl.h"
#include "CMDlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxTdnt.h"
#include "..\wg_db\wg_db_SelectCtrl.h"

#include "..\wg_base\wg_base_MouseEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CCMTendonAddDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMTendonAddDlg : public CCMDlgBase ,public CDBUpdateConnector
{
protected:
	CArray<T_TDNA_BASE_501,T_TDNA_BASE_501&> m_LocalProfileData;
	CString  m_OriTdnaName;
	T_TDNA_K m_TdnaKey ;
	T_TDNA_D_501 m_TdnaData;
	// Construction
	CDialog *m_pMyParentDlg;
public:
	CCMTendonAddDlg(CWnd* pParent = NULL);   // standard constructor
	void MakeListHeader();
	void SetTDNA(T_TDNA_K TdnaKey);
	void SetParentDlg(CDialog *pParentDlg);
	void AbsRelCtrlMan();
	BOOL ApplyDlg();
	void SetData2Dlg();
	void UpdateProfileList();
	BOOL ConvREL2ABS(T_TDNA_BASE_501& TdnaB,BOOL bBeforeAdd = FALSE);
	void ListSelOperation();
	void DlgSizeMan();
	LRESULT OnListCtrlKeyUp(WPARAM wParam, LPARAM lParam);

	void SetDirVectorMode();

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	void OnUnitChange();
	void DoStageChange();
// Dialog Data
	//{{AFX_DATA(CCMTendonAddDlg)
	enum { IDD = IDD_CMD_DEF_TENDON_ADDMODIFY_DLG };
	CTextUnit	m_RELDistUnit;
	CTextUnit	m_wndDistUnit1;
	CTextUnit	m_wndDistUnit2;
	CTextUnit	m_wndDistUnit3;
	CTextUnit	m_wndDistUnit4;
	CTextUnit	m_wndDistUnit5;
	CTextUnit	m_wndDistUnit6;
	CEdit	m_wndDZ;
	CEdit	m_wndDY;
	CMouseEdit	m_wndEZ;
	CMouseEdit	m_wndEY;
	CMouseEdit	m_wndRefElem;
	CMouseEdit	m_wndDistFromINode;
	CMouseEdit	m_wndEndLength;
	CMouseEdit	m_wndBeginLength;
	CMouseEdit	m_wndABSDVector;
	CMouseEdit	m_wndABSCoord;
	CSelectEdit	m_wndAssignElem;
	CMyListCtrl	m_wndTendonDataList;
	CCobxTdnt	m_wndTdntCobx;
	CString	m_strTendonName;
	BOOL	m_bDY;
	BOOL	m_bDZ;
	int		m_nPropileOpt;
	int		m_nDistFromIOpt;
	BOOL	m_bDirVector;
	BOOL	m_bXVector;
	BOOL	m_bYVector;
	BOOL	m_bZVector;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMTendonAddDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMTendonAddDlg)
	afx_msg void OnCmdApply();
	afx_msg void OnCmdCancel();
	afx_msg void OnCmdOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdProfileOptionRadio();
	afx_msg void OnCmdTendonPropButton();
	afx_msg void OnCmdAddButton();
	afx_msg void OnCmdDirvectCheck();
	afx_msg void OnCmdDeleteButton();
	afx_msg void OnCmdModifyButton();
	afx_msg void OnDistOption();
	afx_msg void OnCmdAzAyCheck();
	afx_msg void OnClickCmdTendonDataList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnddragCmdTendonDataList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdPrioUpButton();
	afx_msg void OnCmdPrioDownButton();
	afx_msg void OnOdstatechangedCmdTendonDataList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposCmdRelAlphaYSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposCmdRelAlphaZSpin(NMHDR* pNMHDR, LRESULT* pResult);
	
	//}}AFX_MSG
	afx_msg LRESULT OnMouseEditEnter(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTENDONADDDLG_H__715BCD15_8651_41E9_A827_226860602596__INCLUDED_)
