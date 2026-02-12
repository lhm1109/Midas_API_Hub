#if !defined(AFX_NLLINKPRTITEMDLG_H__0B2FF36B_20AE_4875_9C45_A0E31BC43CEB__INCLUDED_)
#define AFX_NLLINKPRTITEMDLG_H__0B2FF36B_20AE_4875_9C45_A0E31BC43CEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NLLinkPrtItemDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtItemDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CNLLinkPrtItemDescDlg : public CDialogMove
{
public:
	CNLLinkPrtItemDescDlg::CNLLinkPrtItemDescDlg() : CDialogMove(CNLLinkPrtItemDescDlg::IDD) {}
	// Dialog Data
	//{{AFX_DATA(CNLLinkPrtItemDescDlg)
	enum { IDD = IDD_ETC_NLLINK_PRT_DESC_DLG };
	//}}AFX_DATA
	virtual BOOL OnInitDialog();
};

class __MY_EXT_CLASS__ CNLLinkPrtItemDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CNLLinkPrtItemDlg(CWnd* pParent = NULL);   // standard constructor
	void SetParamData(T_NLLP_K Key, T_NLLP_D &ParamData) 
	{ m_Key = m_OldKey = Key, m_Data = ParamData; m_bModify = TRUE; }

protected:
// Dialog Data
	//{{AFX_DATA(CNLLinkPrtItemDlg)
	enum { IDD = IDD_ETC_NLLINK_PRT_ITEM };
	mit::frx::MButton	m_wndMassChk;
	CTextUnit	m_wndMassUnit;
	CEditUnit	m_wndMassEdit;
	CEditUnit	m_wndMassRatioEdit_i;
	CEditUnit	m_wndMassRatioEdit_j;
	mit::frx::MButton	  m_chkShearSpr;
	mit::frx::MButton	  m_wndBtnApply;
	CEditUnit m_wndDzSprLoc;
	CEditUnit m_wndDySprLoc;
	CTextUnit	m_wndWeightUnit;
	CEditUnit	m_wndWeightEdit;
	CEditUnit	m_wndWeightRatioEdit_i;
	CEditUnit	m_wndWeightRatioEdit_j;
	mit::frx::MEdit	    m_wndDescription;
	mit::frx::MComboBox	m_wndPropType;
	mit::frx::MComboBox	m_wndSeisDvType;
	mit::frx::MComboBox	m_wndSeisDvProp;
	mit::frx::MEdit	    m_wndName;	
	int       m_nAppType;
	//}}AFX_DATA
	mit::frx::MButton	m_wndNBtn[6];
	mit::frx::MButton	m_wndNChk[6];
	mit::frx::MButton	m_wndLChk[6];
	CTextUnit	m_wndStifUnit[6];
	CEditUnit	m_wndStifEdit[6];
	CTextUnit	m_wndStifUnit2[6];
	CEditUnit	m_wndStifEdit2[6];
	BOOL	m_bLChk[6];
	BOOL	m_bNChk[6];
	mit::frx::MButton m_wndCoupledBtn;
	mit::frx::MEdit	    m_wndRemarkEdt[6];	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNLLinkPrtItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	//}}AFX_VIRTUAL

// Implementation
protected:
 	double	m_dDySprLoc;
	double	m_dDzSprLoc;
	double  m_dMass;
	double  m_dWeight;
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL ApplyOrOK();
	void OnCmdViscDlg(int nDofType);
	void OnCmdGapDlg(int nDofType);
	void OnCmdHookDlg(int nDofType);
	void OnCmdHystDlg(int nDofType);
	void OnCmdLeadDlg(int nDofType);
	void OnCmdFricDlg(int nDofType);
	void OnCmdTfricDlg(int nDofType);
	void OnCmdDfricDlg(int nDofType);
	void CtrlLNProperty();
	void ChangeCombo(int nSel);
	void MakeCombo(int nSel);
	void OnIehpButtonCtrl();
	void AlignCtrls();
	void EnableDisableSeisDvCtrls();
	void UpdateRemarkText();

    void InitCtrlTexts();

	// Generated message map functions
	//{{AFX_MSG(CNLLinkPrtItemDlg)
	afx_msg void OnCmdNonLinDxBtn();
	afx_msg void OnCmdNonLinDyBtn();
	afx_msg void OnCmdNonLinDzBtn();
	afx_msg void OnCmdNonLinRxBtn();
	afx_msg void OnCmdNonLinRyBtn();
	afx_msg void OnCmdNonLinRzBtn();
	afx_msg void OnCmdNllkLChk();
	afx_msg void OnCmdNllkNDxChk();
	afx_msg void OnCmdNllkNDyChk();
	afx_msg void OnCmdNllkNDzChk();
	afx_msg void OnCmdNllkNRxChk();
	afx_msg void OnCmdNllkNRyChk();
	afx_msg void OnCmdNllkNRzChk();
	afx_msg void OnChangePropType();
	afx_msg void OnCmdApply();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdNllkShearChk();
	afx_msg void OnCmdNllkMassChk();
	afx_msg void OnCmdNookTypeRdo();
	afx_msg void OnCmdIehpBtn();
	afx_msg void OnChangeMassRatioIEdit();
	afx_msg void OnChangeWeightRatioIEdit();
	afx_msg void OnCmdCoupledBtn();
	afx_msg void OnCmdNllkDyEdt();
	afx_msg void OnCmdNllkDyEdt2();
	afx_msg void OnSelChangeSeisTypeCmb();
	afx_msg void OnSelChangeSeisPropCmb();
	afx_msg void OnClickSeisPropBtn();
	afx_msg void OnCmdDescBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CDBDoc*		m_pDoc;
	CDlgUtil* m_pDlgUtil;
	CArray<UINT, UINT> m_DofGroup[6];
	 
	T_NLLP_K	    m_Key, m_OldKey;
	T_NLLP_D	    m_Data;
	T_NLLP_VD		  m_VD[6];				// Visco-elastic damper의 nonlinear property에 쓸 변수를 위한 멤버
	T_NLLP_GAP	  m_GAP[6];			  // Gap의 nonlinear property에 쓸 변수를 위한 멤버
	T_NLLP_HOOK	  m_HOOK[6];			// Hook의 nonlinear property에 쓸 변수를 위한 멤버
	T_NLLP_HS		  m_HS[6];				// Hysteretic System의 nonlinear property에 쓸 변수를 위한 멤버
	T_NLLP_LRBI 	m_LRBI[6];			// Lead Rubber Bearing Isolator의 nonlinear property에 쓸 변수를 위한 멤버
	T_NLLP_FPSI 	m_FPSI[6];			// Friction Pendulum System Isolator의 nonlinear property에 쓸 변수를 위한 멤버
	T_NLLP_TFPSI	m_TFPSI[6];			// Triple Friction Pendulum System Isolator의 nonlinear property에 쓸 변수를 위한 멤버
	T_NLLP_DFPSI	m_DFPSI[6];			// Double Friction Pendulum System Isolator의 nonlinear property에 쓸 변수를 위한 멤버
	
	double m_dStif[21];
	double m_dDamp[21];

	int   m_nType;
	BOOL	m_bModify;
	BOOL  m_bOnInit;

	// Element2 선택 시 영향받는 컨트롤
	CArray<UINT,UINT> m_aCtrl_EnableOnSeis;
	CArray<UINT,UINT> m_aCtrl_DisableOnSeis;
	CArray<UINT,UINT> m_aCtrl_ShowOnSeis;
	CArray<UINT,UINT> m_aCtrl_HideOnSeis;
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NLLINKPRTITEMDLG_H__0B2FF36B_20AE_4875_9C45_A0E31BC43CEB__INCLUDED_)
