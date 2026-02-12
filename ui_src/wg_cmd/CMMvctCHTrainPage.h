#if !defined(__CMMVCTCHTRAINPAGE_H__)
#define __CMMVCTCHTRAINPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvctCHTrainPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvctCHTrainPage dialog
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

class CCMMvctCHTrainPage : public CInternationalDlg
{
// Construction
public:
	CCMMvctCHTrainPage(T_BRDG_TRAIN* pData, int nCodeType, CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CCMMvctCHTrainPage)
	enum { IDD = IDD_CMD_ML_MVCT_CHINA_TRAIN };
	
	int       m_nMethod;
	CEditUnit m_edtSimple_U;
	CEditUnit m_edtSimple_L;
	CEditUnit m_edtCompo_U;
	CEditUnit m_edtCompo_L;
	CEditUnit m_edtConc_U;
	CEditUnit m_edtConc_L;
	CEditUnit m_edtConc_h;       // h (Surcharge Thickness)
	BOOL      m_bConcLoadedLength;
	MComboBox m_cmbConcGrup;
	CEditUnit m_edtArch_U;
	CEditUnit m_edtArch_L;
	CEditUnit m_edtArchLambda;
	CEditUnit m_edtArch_f;
	BOOL      m_bArchLoadedLength;
	MComboBox m_cmbArchGrup;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvctCHTrainPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData2Dlg();
	BOOL SetDlg2Data();
	void EnableDisableControls(BOOL bImpactFactor, int nSpanType);

// Implementation
protected:
	void InitUnit();
	void InitCombo();
	void AlignControl();
	void ControlsShowHide();
	void ChangeText();

	CDBDoc* m_pDoc;

	T_BRDG_TRAIN* m_pData;

	int  m_nCodeType;
	int  m_nSpanType;
	BOOL m_bImpactFactor;

	CArray<UINT, UINT> m_aSimple;
	CArray<UINT, UINT> m_aCompo;
	CArray<UINT, UINT> m_aConc;
	CArray<UINT, UINT> m_aArch;
	
	// Generated message map functions
	//{{AFX_MSG(CCMMvctCHTrainPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdMvchCHTrainMethodRdo();
	afx_msg void OnCmdMvchCHTrainConcLChk();
	afx_msg void OnCmdMvchCHTrainArchLChk();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVCTCHTRAINPAGE_H__)
