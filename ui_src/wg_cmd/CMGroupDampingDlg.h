#if !defined(__CMGROUPDAMPINGDLG_H__)
#define __CMGROUPDAMPINGDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMGroupDampingDlg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CCMGroupDampingDlg dialog

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMGroupDampingDlg : public CDialogMove
{
// Construction
public:
	CCMGroupDampingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMGroupDampingDlg)
	enum { IDD = IDD_CMD_PROP_GROUP_DAMPING };
	CEditUnit	m_editDamping;
	CEditUnit	m_editDampingAlpha;
	CEditUnit	m_editDampingBeta;
	CEditUnit	m_editRatio;
	CEditUnit	m_editAlpha;
	CEditUnit	m_editBeta;
	CComboBox	m_cobxName;
	CListCtrl	m_wndList;
	CButton   m_CalculateChk;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMGroupDampingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void SetGroupNameCombo();
	void SetHeaderTitle();

	CString DataToStr(int i, int nIndex, T_GRDP_BASE &Data);
	BOOL InsertItem(int nIndex, T_GRDP_BASE &Data);
	void MakeItemEx();

	CString GetGroupName(int nGrupType, UINT GrupK);
	CString GetGroupType(int nGrupType);
	int IsUsedGrupData(int nType, UINT GrupK);

	void BaseData2Dlg(T_GRDP_BASE& rBaseData);
	BOOL Dlg2BaseData(T_GRDP_BASE& rBaseData);

	void Data2Dlg();
	BOOL Dlg2Data();

protected:
	T_GRDP_D m_Data;

	CDBDoc* m_pDoc;
	CArray<UINT, UINT> m_aCtrlTypeRadio;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMGroupDampingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnModify();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCmdGroupTypeRadio();
	afx_msg void OnItemchangedCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMGROUPDAMPINGDLG_H__)
