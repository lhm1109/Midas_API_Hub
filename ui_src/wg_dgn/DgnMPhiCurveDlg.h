#if !defined(AFX_DGNMPHICURVEDLG_H__DE99E2AA_1F5C_4560_B233_AA15FF23E8D4__INCLUDED_)
#define AFX_DGNMPHICURVEDLG_H__DE99E2AA_1F5C_4560_B233_AA15FF23E8D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnMPhiCurveDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "DgnMomentCurvatureDesign.h"
#include "DgnMPhiCurveWnd.h"
#include "..\MIT_frx\McomboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnMPhiCurveDlg dialog
#include "HeaderPre.h"
class CDgnMPhiCurveGrid;
class __MY_EXT_CLASS__ CDgnMPhiCurveDlg : public CDialogMove
{
	// Construction
public:
	CDgnMPhiCurveDlg(CWnd* pParent = NULL);   // standard constructor
	~CDgnMPhiCurveDlg();   // standard deconstruction

	// Dialog Data
	//{{AFX_DATA(CDgnMPhiCurveDlg)
	enum { IDD = IDD_DGN_MPHI_CURVE_DLG };

	CEditUnit m_edtName;

	mit::frx::MComboBox  m_cmbSection;
	mit::frx::MComboBox  m_cmbPosition;
	mit::frx::MComboBox  m_cmbConcrete1;
	mit::frx::MComboBox  m_cmbConcrete2;
	mit::frx::MComboBox  m_cmbSteel;

	CEditUnit m_edtAxialForce;
	CTextUnit m_untAxialForce;
	CEditUnit m_edtAxisAngle;
	CTextUnit m_untAxisAngle;
	CEditUnit m_edtNumPoint;

	BOOL      m_bIdealized;
	BOOL      m_bUserCurve;
	CEditUnit m_edtUserCurve;
	CTextUnit m_untUserCurve;
	int       m_nIdealOpt;

	mit::frx::MComboBox  m_cmbTypeCurve;

	CTextUnit m_untMoment;
	CTextUnit m_untCurvature;

	CEditUnit m_edtCrackMomentPlus;
	CEditUnit m_edtCrackMomentMinus;
	CEditUnit m_edtCrackCurvaturePlus;
	CEditUnit m_EdtCrackCurvatureMinus;

	CEditUnit m_edtYieldInitMomentPlus;
	CEditUnit m_edtYieldInitMomentMinus;
	CEditUnit m_edtYieldInitCurvaturePlus;
	CEditUnit m_edtYieldInitCurvatureMinus;

	CEditUnit m_edtYieldMomentPlus;
	CEditUnit m_edtYieldMomentMinus;
	CEditUnit m_edtYieldCurvaturePlus;
	CEditUnit m_edtYieldCurvatureMinus;

	CEditUnit m_edtYieldIdealMomentPlus;
	CEditUnit m_edtYieldIdealMomentMinus;
	CEditUnit m_edtYieldIdealCurvaturePlus;
	CEditUnit m_edtYieldIdealCurvatureMinus;

	CEditUnit m_edtUltimateMomentPlus;
	CEditUnit m_edtUltimateMomentMinus;
	CEditUnit m_edtUltimateCurvaturePlus;
	CEditUnit m_edtUltimateCurvatureMinus;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnMPhiCurveDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
public:
	void InitCombo();
	void InitUnit();
	void ControlsShowHide();
	void ControlsEnableDisable();

	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL CheckDialogData();
	void ShowResultData();

	void GetGridData(T_MPCC_K MpccKey);

	void DrawSection();
	void DrawGraph();

	CDBDoc*  m_pDoc;

	CDgnMPhiCurveGrid* m_pGrid;
	CDgnMPhiCurveWnd   m_SectWnd;
	CDgnMPhiCurveWnd   m_ChartWnd;


	T_MPCC_K m_InitMpccK;
	T_MPCC_D m_Data;
	DGN_CALC_MPHI_RESULT m_ResultD;
	CDgnMomentCurvatureDesign m_DgnMPhiDesign;// 계산, 삽도생성 및 계산된 MPCC들에 대한 결과정보들을 보관

	BOOL m_bConcTypeMander;

	// Generated message map functions
	//{{AFX_MSG(CDgnMPhiCurveDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnMPhiCurveSelAllBtn();
	afx_msg void OnDgnMPhiCurveUnSelAllBtn();
	afx_msg void OnDgnMPhiCurveDelBtn();
	afx_msg void OnDgnMPhiCurveCalcSelBtn();
	afx_msg void OnDgnMPhiCurveOutputBtn();
	afx_msg void OnDgnMPhiCurveSectionCmb();
	afx_msg void OnDgnMPhiCurvePositionCmb();
	afx_msg void OnDgnMPhiCurveConcrete1Cmb();
	afx_msg void OnDgnMPhiCurveIdealizedChk();
	afx_msg void OnDgnMPhiCurveUserCurveChk();
	afx_msg void OnDgnMPhiCurveOptGuideBtn();
	afx_msg void OnDgnMPhiCurveAddBtn();
	afx_msg void OnDgnMPhiCurveModifyBtn();
	afx_msg void OnDgnMPhiCurveCalcBtn();
	afx_msg void OnDgnMPhiCurveTypeCurveCmb();
	afx_msg void OnDgnMPhiCurveECUCBtn(); // Estimate Condition of Ultimate Curvature
	afx_msg void OnChangeAxisAngle();
	afx_msg void OnDgnMPhiCurveSectionBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNMPHICURVEDLG_H__DE99E2AA_1F5C_4560_B233_AA15FF23E8D4__INCLUDED_)

