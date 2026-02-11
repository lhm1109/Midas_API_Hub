#if !defined(AFX_CMMLSPAASHTOLRFDDLG_H__49F116E2_2582_11D4_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMMLSPAASHTOLRFDDLG_H__49F116E2_2582_11D4_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\FoldGroupBox.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_cobxgrup.h"

#include "CMChildBarBase.h"

#ifndef __AFXEXT_H__
#endif

class CCMMlspAASHTODlg : public CCMChildBarBase, public CDBUpdateConnector
{
public:
	CCMMlspAASHTODlg();           // protected constructor used by dynamic creation

//----------------------------------------------------------------
// Overridables
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

//----------------------------------------------------------------
// Implementations
protected:
	void SetListCtrlHeader();
	void MakeItemEx();
	CString DataToStr(int i, int nNo, T_MLSP_K key);
	void SetItem(int nIndex, T_MLSP_K Key);
	void SetGirderItem(int nIndex, T_MLSP_K Key);
	void InsertItem(T_MLSP_K Key);
	void DeleteItem(T_MLSP_K Key);
	void OnDelKeyPressedInList();
	void OnDelKeyPressedInListAuto();
	void GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem);
	void CtrlMan();

public:
	CDBDoc* m_pDoc;

protected:  
	CArray<UINT, UINT> m_aCtrlGridGrp;
	CArray<UINT, UINT> m_aCtrlUserInputID;
	CArray<UINT, UINT> m_aCtrlElemType;

// Form Data
public:
	enum { IDD = IDD_DBAR_MLSP_ASSHTOLRFD_DLG };
	CListCtrl	m_List;
	int		m_nEType;
	int		m_nPosition;

	CCobxGrup m_cbxGirderGrp;   // MNET:3021-JHLEE-20070928
	CListCtrl m_lstAuto;
	CFoldGroupBox_Cross m_frmAutoInput;
	CFoldGroupBox_Cross m_frmUserInput;
	BOOL m_bFoldState;
	BOOL m_bFirstInit;
	BOOL m_bFoldInitialize;
	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMlspAASHTODlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCMMlspAASHTODlg();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	afx_msg void OnCmdClose();
	afx_msg void OnCmdAutoAdd();
	afx_msg void OnCmdAutoDel();
	afx_msg void OnCmdUserAdd();
	afx_msg void OnCmdUserDel();
	// afx_msg void OnCmdOptionAddDel();
	afx_msg void OnCmdEtype();
	afx_msg void OnCmdGrpTableBtn();
	afx_msg void OnCmdGridGrpCmb();
	afx_msg void OnClicklist(NMHDR* pNMHDR, LRESULT* pResult);
 	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_CMMLSPAASHTOLRFDDLG_H__49F116E2_2582_11D4_92DE_0000C0B0E6B3__INCLUDED_)
