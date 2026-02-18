// DgnLateralCapaCheckDlg.h: interface for the CDgnLateralCapaCheckDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNLATERALCAPACHECKDLG_H__3824B475_10EB_4A1D_9A5F_338EBCF6FEDA__INCLUDED_)
#define AFX_DGNLATERALCAPACHECKDLG_H__3824B475_10EB_4A1D_9A5F_338EBCF6FEDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "DgnDlgBase.h"

#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_SelectCtrl.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_DBDoc.h"

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

class CDgnLateralCapaCheckDlg : public CDgnDlgBase, public CDBUpdateConnector 
{
public:
	CDgnLateralCapaCheckDlg(CWnd* pParent = NULL);
	virtual void Execute() {}

// Dialog Data
	//{{AFX_DATA(CDgnLateralCapaCheckDlg)
	enum { IDD = IDD_DGN_LATERAL_CAPA_CHK_DLG };
	MComboBox	m_TypeComb;
	MEdit   m_NameEdit;
	CString m_SelectElemString;
	CSelectEdit m_editSelectElem;
	MButton m_MyChkBox;
	MButton m_MzChkBox;
	CEditUnit m_WuEdit;
	CTextUnit m_WuUnit;
	CEditUnit m_WpEdit;
	CTextUnit m_WpUnit;
	CEditUnit m_TyEdit;
	CEditUnit m_TzEdit;
	CTextUnit m_TUnit;
	CEditUnit m_HEdit;
	CTextUnit m_HUnit;
	CEditUnit m_M0yEdit;
	CEditUnit m_M0zEdit;
	CTextUnit m_M0Unit;
	CEditUnit m_P0Edit;
	CTextUnit m_P0Unit;

	CListCtrl   m_RlccList;;
	//}}AFX_DATA

public:
	void InitCtrl();
	void Initial_Data();
	void SetHeaderTitle();		
	
	void UpdateRlccList();

	BOOL Dlg2Data();
	BOOL Data2Dlg();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnLateralCapaCheckDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Implementation
protected:
	CDBDoc*  m_pDoc;
	int      m_nCurSelectedKey;
	T_RLCC_D m_Data;
	BOOL     m_bDataUpDate;
	BOOL     m_nInitialForce;
	
	// Generated message map functions
	//{{AFX_MSG(CDgnLateralCapaCheckDlg)
	virtual BOOL OnInitDialog();	
	afx_msg void OnMyMzChk();
	afx_msg void OnAddBtn();
	afx_msg void OnModifyBtn();
	afx_msg void OnDelBtn();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnItemchangedRlccList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickRlccList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString GetStrElemList(CArray<T_ELEM_K, T_ELEM_K>& KeyList, BOOL bExistChk=FALSE);
	CString GetTypeName(int nType);
	int     GetTypeID(CString strName);
};


#endif // !defined(AFX_DGNLATERALCAPACHECKDLG_H__3824B475_10EB_4A1D_9A5F_338EBCF6FEDA__INCLUDED_)
