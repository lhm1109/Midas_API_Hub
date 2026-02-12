#if !defined(__CMGROUPDAMPINGELEMENTDLG_H__)
#define __CMGROUPDAMPINGELEMENTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMGroupDampingElementDlg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"
class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CCMGroupDampingDlg dialog

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMGroupDampingElementDlg : public CDialogMove
{
// Construction
public:
	CCMGroupDampingElementDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMGroupDampingElementDlg)
	enum { IDD = IDD_CMD_PROP_GRDP_ELEM };
	mit::frx::MButton	  m_chkMassPropDef  ;
	mit::frx::MButton	  m_chkStiffPropDef ;
	CEditUnit	m_edtDirectMassDef;
	CEditUnit	m_edtDirectStifDef;
	CEditUnit	m_edtCalcMassDef  ;
	CEditUnit	m_edtCalcStifDef  ;
	CEditUnit	m_edtFreqDef1     ;
	CEditUnit	m_edtFreqDef2     ;
	CEditUnit	m_edtPeriodDef1   ;
	CEditUnit	m_edtPeriodDef2   ;
	CEditUnit	m_edtDampingDef1  ;
	CEditUnit	m_edtDampingDef2  ;

	mit::frx::MComboBox m_cmbGroupName ;
	mit::frx::MButton	  m_chkMassProp  ;
	mit::frx::MButton	  m_chkStiffProp ;
	CEditUnit	m_edtDirectMass;
	CEditUnit	m_edtDirectStif;
	CEditUnit	m_edtCalcMass  ;
	CEditUnit	m_edtCalcStif  ;
	CEditUnit	m_edtFreq1     ;
	CEditUnit	m_edtFreq2     ;
	CEditUnit	m_edtPeriod1   ;
	CEditUnit	m_edtPeriod2   ;
	CEditUnit	m_edtDamping1  ;
	CEditUnit	m_edtDamping2  ;

	CListCtrl	m_wndList;

	//}}AFX_DATA
	CArray<UINT,UINT> m_aDirectCalcDefRdo;
	CArray<UINT,UINT> m_aFreqPeriodDefRdo;

	CArray<UINT,UINT> m_aGroupTypeRdo;
	CArray<UINT,UINT> m_aDirectCalcRdo;
	CArray<UINT,UINT> m_aFreqPeriodRdo;
	CArray<UINT,UINT> m_aMaterialDirectRdo;

	CArray<UINT,UINT> m_aCtrlFreqDamp;
	CArray<UINT,UINT> m_aCtrlDefCoefCalc;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMGroupDampingElementDlg)
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

	void AutoCalcDefaultCoef();
	void AutoCalcCoef();
	void EnableDisableDefaultCtrls();
	void EnableDisableCtrls();
	void ChangeControlsText();

protected:
	T_GRDP_D m_Data;

	CDBDoc* m_pDoc;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMGroupDampingElementDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeDefaultDirectCalcRdo();
	afx_msg void OnChangeDefaultFreqPeriodRdo();
	afx_msg void OnChkDefaultMassStiffBtn();
	afx_msg void OnChangeDefaultAutoCalcCoef();
	afx_msg void OnChangeDirectCalcRdo();
	afx_msg void OnChangeFreqPeriodRdo();
	afx_msg void OnChkMassStiffBtn();
	afx_msg void OnChangeAutoCalcCoef();
	afx_msg void OnSetDefaultDataBtn();
	afx_msg void OnChangeMaterialDirectRdo();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnModify();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelChangeNameCmb();
	afx_msg void OnCmdGroupTypeRadio();
	afx_msg void OnItemchangedCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdSelectOptionBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMGROUPDAMPINGELEMENTDLG_H__)
