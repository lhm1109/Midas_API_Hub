#if !defined(AFX_CMHYDRATIONANALYDLG_MEC_H__E6082F01_0CA9_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CMHYDRATIONANALYDLG_MEC_H__E6082F01_0CA9_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMHydrationAnalyDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\wg_base\SpinBtnExCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CCMHydrationAnalyDlg_MEC dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__  CCMHydrationAnalyDlg_MEC : public CDialogMove
{
	// Construction
public:
	CCMHydrationAnalyDlg_MEC(CWnd* pParent = NULL);   // standard constructor

	void InitUnit();
	void InitCombo();
	void EnableDisableControls();

	void Data2Dlg();
	BOOL Dlg2Data();

	// Dialog Data
		//{{AFX_DATA(CCMHydrationAnalyDlg_MEC)
	enum { IDD = IDD_ETC_HYDRATION_MEC };

	int m_nFinalStage;
	mit::frx::MComboBox m_cmbHstg;
	CEditUnit m_edtFactor;
	CEditUnit m_edtTemper;
	CTextUnit m_untTemper;
	int	m_nEvaluation;
	BOOL m_bCreepShrink;
	int	m_nCreepShrinkType;
	int	m_nCreepCalcMethod;
	CEditUnit m_edtphi1;
	CEditUnit m_edtDay1;
	CEditUnit m_edtphi2;
	CEditUnit m_edtDay2;
	BOOL m_bUseEquivalent;
	BOOL m_bSelfWeight;
	CEditUnit m_edtSelfWeightFact;
	CEditUnit m_edtMaxIterPerIncre;
	BOOL m_bDisp;
	CEditUnit m_edtDisp;
	BOOL m_bLoad;
	CEditUnit m_edtLoad;
	BOOL m_bWork;
	CEditUnit m_edtWork;

	// 아래는 사용 안함 
	int		m_num;
	CFormulaEditSpin	m_num_spin;
	CEditUnit m_tol;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMHydrationAnalyDlg_MEC)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CDBDoc* m_pDoc;
	T_HHCT_D m_Data;

	CArray<UINT, UINT> m_aCreep;
	CArray<UINT, UINT> m_aEffModul;

	// Generated message map functions
	//{{AFX_MSG(CCMHydrationAnalyDlg_MEC)
	virtual BOOL OnInitDialog();
	afx_msg void OnHhctFinalStageRdo();
	afx_msg void OnHhctCreepShrinkChk();
	afx_msg void OnHhctCreepCalcMethodChk();
	afx_msg void OnHhctSelfWeightChk();
	afx_msg void OnHhctDispChk();
	afx_msg void OnHhctLoadChk();
	afx_msg void OnHhctWorkChk();
	afx_msg void OnHhctRemove();
	afx_msg void OnHhctOk();
	afx_msg void OnHhctCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMHYDRATIONANALYDLG_MEC_H__E6082F01_0CA9_11D5_880F_00010263A1F4__INCLUDED_) 