#if !defined(AFX_CMHYDRATIONANALYDLG_H__E6082F01_0CA9_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CMHYDRATIONANALYDLG_H__E6082F01_0CA9_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMHydrationAnalyDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\wg_db\TextUnit.h"   // to use CTextUnit 
#include "..\wg_db\UnitCtrl.h"   // to use CTextUnit 

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

#include "..\wg_base\SpinBtnExCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCMHydrationAnalyDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__  CCMHydrationAnalyDlg : public CDialogMove
{
// Construction
public:
	CCMHydrationAnalyDlg(CWnd* pParent = NULL);   // standard constructor
	int   AddData2List(double sum);
	void  SetListCtrlHeader();
	BOOL  GetSelectedItem(int &nIndex);
	void  MakeItemEx();
	void  SetItem(int nIndex);
	void  AddList2Data();
	BOOL  Dlg2Item();
	BOOL  Item2Dlg();
	void ResizeDialog();
public:
	T_HHCT_D m_data;
	CDBDoc* m_pDoc;

// Dialog Data
	//{{AFX_DATA(CCMHydrationAnalyDlg)
	enum { IDD = IDD_ETC_HYDRATION };
	CEditUnit	m_day2;
	CEditUnit	m_phi2;
	CEditUnit	m_Day1;
	CEditUnit	m_phi1;
	mit::frx::MButton	m_General;
	mit::frx::MComboBox	m_cobxHstg;
	CEditUnit	m_tol;
	mit::frx::MButton	m_CreepShrinkChk;
	CFormulaEditSpin	m_num_spin;
	CEditUnit	m_temper;
	CTextUnit	m_wndHhctInitThrUnit;         ///< temperature unit text 
	CEditUnit	m_factor;
	CEditUnit	m_selfWeightFactor;
	int		m_evaluation;
	int		m_type; //3°³ 
	BOOL	m_bSelfWeight;
	int		m_nCreepType;//2°³ 
	BOOL  m_bUseEquivalent;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMHydrationAnalyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetHstgCombo();

	//CArray<double,double> m_TimeList;
	CArray<UINT, UINT>		m_aCreep;
	CArray<UINT, UINT>		m_aShrinkage;
	CArray<UINT, UINT>    m_aFinalStage;

	CArray<UINT,UINT>     m_Convergence,m_Eeff;

	// Generated message map functions
	//{{AFX_MSG(CCMHydrationAnalyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHhctOk();
	afx_msg void OnHhctCancel();
	afx_msg void OnHhctRemove();
	afx_msg void OnHhctCsChk();
	afx_msg void OnSelChangeType();
	afx_msg void OnChangeFinalOption();
	afx_msg void OnSelfWeightChk();
	afx_msg void OnHhctType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMHYDRATIONANALYDLG_H__E6082F01_0CA9_11D5_880F_00010263A1F4__INCLUDED_)
