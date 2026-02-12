#if !defined(__CMTENDONCHANGEPROFDLG_H__)
#define __CMTENDONCHANGEPROFDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

class CDBDoc;
class CCMTendonChangeProfDlg : public CDialogMove, public CDBUpdateConnector
{

public:
	CCMTendonChangeProfDlg(CWnd* pParent = NULL);   // standard constructor

	void SetSelectedProfiles(CArray<UINT, UINT> &aSelectedK);

	enum { IDD = IDD_CMD_DEF_TENDON_CHANGE_PROF_DLG };

	MComboBox	m_cobxProperty;
	MComboBox	m_cobxGroup;
	CListBox	m_listSelProfiles;
	CListBox	m_listAllProfiles;
	int		m_nParamType;
	int		m_nNumTd;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

protected:
	void InitAllList();
	void InitSelList();
	void InitCtrl();
	void InitPropCombo();
	void InitGroupCombo();
	void DoSelectItem();
	void DoUnselectItem();
	BOOL DoChangeProperty();
	void AlignControlsDynamic();
	BOOL RenameTendonProfiles();
	BOOL GenBysuffixButton();

protected:
	CDBDoc* m_pDoc;
	CArray<UINT, UINT> m_aSelectedProfiles;
	CArray<UINT, UINT> m_aCtrlParamTypeRadio;
	CArray<UINT, UINT> m_aCtrlList;
	CArray<UINT, UINT> m_aCtrlTdName;
	CArray<UINT, UINT> m_aCtrlTdProperty;
	CArray<UINT, UINT> m_aCtrlTdPropertyGrp;
	CArray<UINT, UINT> m_aCtrlTypicalTd;
	CArray<UINT, UINT> m_aCtrlTdGroup;
	CArray<UINT, UINT> m_aCtrlTransfer;
	CArray<UINT, UINT> m_aCtrlTDebonded;

	BOOL bName;
	BOOL bProperty;
	BOOL bPropertyGrp;
	BOOL bTypicalTD;
	BOOL bGroup;
	BOOL bTransfer;
	BOOL bDebonded;
	CString	m_strName;
	CString	m_strSuffix;
	// Implementation
protected:


	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnApply();
	afx_msg void OnCmdSelButton();
	afx_msg void OnCmdUnselButton();
	afx_msg void OnCmdPropertyButton();
	afx_msg void OnCmdGroupButton();
	afx_msg void OnDblclkCmdAllList();
	afx_msg void OnDblclkCmdSelectedList();
	afx_msg void OnSelChangeCombo();
	afx_msg void OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
public:
	CFormulaEditSpin   m_editNum;
	CEditUnit	m_editDebondBegin; //
	CEditUnit	m_editDebondEnd; //
	CTextUnit   m_uniDebondEnd;
	CEditUnit	m_editTransBegin; //
	CEditUnit	m_editTransEnd; //
	CTextUnit   m_uniTransEnd;

};

#endif // !defined(__CMTENDONCHANGEPROFDLG_H__)
