#if !defined(__CMGROUPDAMPINGSTRAINDLG_H__)
#define __CMGROUPDAMPINGSTRAINDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMGroupDampingStrainDlg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CCMGroupDampingStrainDlg dialog

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMGroupDampingStrainDlg : public CDialogMove
{
// Construction
public:
	CCMGroupDampingStrainDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMGroupDampingStrainDlg)
	enum { IDD = IDD_CMD_PROP_GRDP_STRN };
	CEditUnit	m_edtDampingDef;
	mit::frx::MComboBox m_cmbGroupName ;
	CEditUnit	m_edtDamping;
	CListCtrl	m_wndList;
	mit::frx::MButton   m_chkCalculate;
	//}}AFX_DATA
	CArray<UINT,UINT> m_aGroupTypeRdo;
	CArray<UINT,UINT> m_aMaterialDirectRdo;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMGroupDampingStrainDlg)
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

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMGroupDampingStrainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnModify();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelChangeNameCmb();
	afx_msg void OnCmdGroupTypeRadio();
	afx_msg void OnChangeMaterialDirectRdo();
	afx_msg void OnItemchangedCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdSelectOptionBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMGROUPDAMPINGSTRAINDLG_H__)
