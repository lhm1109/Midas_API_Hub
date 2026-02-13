#if !defined(__DGN_CON_CODE_NEW_CHILD_9_DLG_H__)
#define __DGN_CON_CODE_NEW_CHILD_9_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConCodeNewChild_9_Dlg.h.h : header file
//
#include "DconChildDialog.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"
using namespace mit::frx;

#include "HeaderPre.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild9Dlg dialog
class __MY_EXT_CLASS__ CDgnConCodeNewChild9Dlg : public CDconChildDialog
{
// Construction
public:
	CDgnConCodeNewChild9Dlg(CDgnConCodeNewDlg* pParent = NULL, T_DCON_D* pData=NULL, BOOL bExistData=FALSE);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnConCodeNewChild9Dlg)
	enum { IDD = IDD_DGN_CON_CODE_CHILD9_DLG };
	
	BOOL	  m_bNTCFlag;// PMS:DNKIM-20121030 NTC2012 추가
	double  m_dMrft;
	double  m_dUf;
	double  m_dGamma_rdForBeam;
	double  m_dGamma_rdForColm;
	double  m_dGamma_rdForWall;
	double  m_dGamma_rdForJoint;
	double  m_dStorColmWeakBeamFactor;

	BOOL    m_bStrongColmWeakBeam;
	BOOL    m_bTorsionDesign;
	BOOL    m_bBCJointDgn;
	BOOL    m_bShearWallAlphaMax;
	BOOL    m_bVedPrimaryMemb;

	CFoldGroupBox_Cross	m_grpSeisParam;

	int		m_nPMCurveMethod;
	int		m_nCheckPos;

	MButton   m_ChkUseVcWall;
	MButton   m_ChkUseVcColm;
	MButton   m_ChkUseVcBeam;

	MButton   m_chkApplyNTC;
	MButton   m_chkApplyBCJDesign;
	MButton   m_chkAutoCalcA;
	MButton   m_chkAutoCalcC;
	MButton   m_ChkLimitShearStrgConc;
	MButton   m_chkConfinedJoint;
	MButton   m_chkNotConfinedJoint;
	MButton   m_chkShearWallAlphaMax;
	MButton   m_chkVedPrimaryMemb;

	CEditUnit m_edtStrutAngle;
	CEditUnit m_edtEffectivePhi;
	CEditUnit m_edtSlendLimitA;
	CEditUnit m_edtSlendLimitB;
	CEditUnit m_edtSlendLimitC;

	MComboBox m_cmbNDGroup;
	MComboBox m_cmbGroup;
	MComboBox m_cmbNTCType;
	MComboBox	m_ComboNationAnnex;

	int		m_nNDDesignMethod;
	double  m_dApproximateAlpha;
	int		m_nFsMethod;

	T_KEY m_NDGroupKey;
	T_KEY m_GroupKey; 
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConCodeNewChild9Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	virtual void ChangeDesignCode();
	virtual void UpdateChildData();
	virtual void ResizeChildDlg();
	virtual BOOL Dlg2Data();
protected:
	CArray<UINT, UINT> m_aSlendernessCtrls;
	CArray<UINT, UINT> m_aSlendernessCtrlsNTC2012;
	CArray<UINT, UINT> m_aBCJointGroup;
	CArray<UINT, UINT> m_aSelectPosGroup;
//   CArray<UINT, UINT> m_aStrongColmWeakBeam;
	CArray<UINT, UINT> m_aStrongColm;
	CArray<UINT, UINT> m_aSelectDuctility;
	CArray<UINT, UINT> m_aDuctilityRadio;
	CArray<UINT, UINT> m_aDesignMethodND;
	CArray<UINT, UINT> m_aNonDissipative;
	CArray<UINT, UINT> m_aSecondarySeis;
	CArray<UINT, UINT> m_aShearForce;
	CArray<UINT, UINT> m_aEnvelopMethod;
	CArray<UINT, UINT> m_aFrictionCoeff;
	CArray<UINT, UINT> m_aTorsionDesign;
	CArray<UINT, UINT> m_aMomentRedistr;
	CArray<UINT, UINT> m_aConsiderShear;
	CArray<UINT, UINT> m_aLimitShearStr;
	CArray<UINT, UINT> m_aPMCurveMethod;
	
	CString m_strDCH;
	CString m_strDCM;

	BOOL m_bSingaporeAndMore;
	BOOL m_bGermanyOrSpain;
	BOOL m_bLimitShearMoved;  // Limit Shear 위치 이동 여부
	BOOL m_bApplyEurocodeEdited;
	T_DCON_D m_DconEC8Data;
	BOOL m_bShowPMCurveMethod;
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnConCodeNewChild9Dlg)
	void InitCtrl();
	void SetComboBox();
	void SetNTC2008EnableControl();
	void SetShowLimitShear();
	void ShowSlendernessLimit();
	void SelchangeNTCType(BOOL bResize);
	void SetGrupComboBox(MComboBox& cobx);
	void ShowPMCurveMethod();
	void EnableDisableAlphaMax();
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnRcctrlSpecial();
	afx_msg void OnChkApplyNTC();
	afx_msg void OnBtnEC8Data();
	afx_msg void OnChkAutoCalcA();
	afx_msg void OnChkAutoCalcC();
	afx_msg void OnDuctilityBtn();
	afx_msg void OnBCJoint();
	afx_msg void OnNDDesignMethod();
	afx_msg void OnBCConfinedJoint();
	afx_msg void OnBCNotConfinedJoint();
	afx_msg void OnBtnWallEnvelopMethod();
	afx_msg void OnBtnGrup();
	afx_msg void OnSetNDGrupCombo();
	afx_msg void OnSetGrupCombo();
	afx_msg void OnSelchangeNTCType();
	afx_msg void OnSelchangeNationalAnnexCombo();
	afx_msg void OnSelchangeNDGrupCombo();
	afx_msg void OnSelchangeGrupCombo();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGN_CON_CODE_NEW_CHILD_9_DLG_H__)
