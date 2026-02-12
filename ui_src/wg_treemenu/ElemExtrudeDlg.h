// Old Version : ElemExtrudeDlg2 »ç¿ë
#if !defined(AFX_ELEMEXTRUDEDLG_H__BAFE50C8_DECC_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_ELEMEXTRUDEDLG_H__BAFE50C8_DECC_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElemExtrudeDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_CobxMatl.h"
#include "..\wg_db\wg_db_CobxSect.h"
#include "..\wg_db\wg_db_CobxThik.h"
#include "..\wg_db\wg_db_EditMatl.h"
#include "..\wg_db\wg_db_EditSect.h"
#include "..\wg_db\wg_db_EditThik.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\mit_frx\MillustViewer.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MEdit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CElemExtrudeDlg dialog

class  _ElemExtrude
{
public:
	// For Common
	int m_nExtrudeType;
	BOOL m_bMoveSrc;
	BOOL m_bRemoveSrc;
	int m_nElemType;
	T_MATL_K m_nMatlNo;
	T_SECT_K m_nSectNo;
	T_THIK_K m_nThikNo;
	int m_nGenerationType;
	// Reverse I-J
	BOOL m_bReverseIJ;
	BOOL m_BetaAngle;


	// For Translate
	int  m_nOption; // equal/unequal
	double m_EQ_UX, m_EQ_UY, m_EQ_UZ;
	double m_EQ_WX, m_EQ_WY, m_EQ_WZ;
	int  m_NTimes;
	int  m_nUEQ_Axis;
	CArray<double,double> m_UEQ_DistList;
	double m_UEQ_UX,m_UEQ_UY,m_UEQ_UZ;
	double m_UEQ_WX,m_UEQ_WY,m_UEQ_WZ;

	// For Rotate
	int    m_nRotNTimes;
	double m_AngleOfRotDEG;
	CArray<double,double> m_arAngle;
	double m_DistR,m_DistZ;
	int    m_nAxis; // 0 : X , 1 : Y , 2 : Z , 3 : 2Point

	double m_FirstP_UX,m_FirstP_UY, m_FirstP_UZ;
	double m_FirstP_WX,m_FirstP_WY, m_FirstP_WZ;
	double m_SecondP_UX,m_SecondP_UY, m_SecondP_UZ;
	double m_SecondP_WX,m_SecondP_WY, m_SecondP_WZ;

	double m_AxisV_Ux, m_AxisV_Uy, m_AxisV_Uz;
	double m_AxisV_Wx, m_AxisV_Wy, m_AxisV_Wz;

	// For Project Added by water tree.
	int nProjType;
	int nDirType;
	double dProjux, dProjuy, dProjuz;
	double dProjpt1[3], dProjpt2[3], dProjpt3[3], dProjRad[3];
	int KeyElemTarget;

	// For Thickness Extrude Added by water tree.
	double dThickness;
	int nDirection;

	// data for element type
	int  m_nSubType;
	double m_dSubData;
	int m_nWallID;
};

class CElemExtrudeDlg : public CMenuBarChildDlg
{
// Construction
public:
	CElemExtrudeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();

	BOOL GetParameter(_ElemExtrude& EE);
	BOOL ExtrudeElem(_ElemExtrude& EE);


// Dialog Data
	//{{AFX_DATA(CElemExtrudeDlg)
	enum { IDD = IDD_TM_ELEM_EXTRUDE };
	CMouseEdit	m_wndThicknessEdit;
	CTextUnit	m_wndThicknessUnit;
	MComboBox	m_wndProjectType;
	CTextUnit	m_wndCTDistUnit;
	CTextUnit	m_wndTTForceUnit;
	CTextUnit	m_wndTTDistUnit;
	CFormulaEdit m_wndWallID;
	CEditUnit	m_wndTTForceEdit;
	CMouseEdit m_wndCTDistEdit;
	CMouseEdit m_wndTTDistEdit;
	MButton	m_wndRemoveCheck;
	CFormulaEditSpin m_wndRotIterNum;
	CTextUnit	m_wndRotAngleUnit;
	CFormulaEditSpin m_wndRotAngle;
	CTextUnit	m_wndDistRUnit;
	CMouseEdit	m_wndDistR;
	CTextUnit	m_wndDistAUnit;
	CMouseEdit	m_wndDistA;
	CTextUnit	m_wnd2ndUnit;
	CTextUnit	m_wnd1stUnit;
	CMouseEdit	m_wnd2ndPoint;
	CMouseEdit	m_wnd1stPoint;
	CFormulaEditSpin m_wndIterNum;
	CTextUnit	m_wndDistUnit3;
	CTextUnit	m_wndDistUnit2;
	CTextUnit	m_wndDistUnit1;
	CMouseEdit	m_wndEQDist;
	CEdit	m_wndUEQDist;
	CMouseEdit	m_wndDirVector;
	CCobxThik	m_wndThikName;
	CEditThik	m_wndThikNo;
	CCobxSect	m_wndSectName;
	CEditSect	m_wndSectNo;
	CCobxMatl	m_wndMatlName;
	CEditMatl	m_wndMatlNo;
	MComboBox	m_wndElemType;
	MComboBox	m_wndExtrudeType;
	BOOL	m_bRemoveSrc;
	int		m_nGenerationType;
	int		m_nTranOption;
	int		m_nTranChooseAxis;
	int		m_nRotAxis;
	CTextUnit	  m_wndDirPt1Unit;
	CMouseEdit	m_wndDirPt1;
	CMouseEdit	m_wndElemNo;
	CTextUnit	  m_wndElipPt1Unit;
	CMouseEdit	m_wndElipPt1;
	CTextUnit	  m_wndElipRadUnit;
	CMouseEdit	m_wndElipRad;
	CTextUnit	  m_wndElipOrgUnit;
	CMouseEdit	m_wndElipOrg;
	CTextUnit	  m_wndSphrRadUnit;
	CMouseEdit  m_wndSphrRad;
	CTextUnit	  m_wndSphrOrgUnit;
	CMouseEdit	m_wndSphrOrg;
	CTextUnit	  m_wndConeRad2Unit;
	CMouseEdit  m_wndConeRad2;
	CTextUnit	  m_wndConeRad1Unit;
	CMouseEdit	m_wndConeRad1;
	CTextUnit	  m_wndConePt2Unit;
	CMouseEdit	m_wndConePt2;
	CTextUnit	  m_wndConePt1Unit;
	CMouseEdit	m_wndConePt1;
	CTextUnit	  m_wndCylRadUnit;
	CMouseEdit	m_wndCylRad;
	CTextUnit	  m_wndCylPt2Unit;
	CMouseEdit	m_wndCylPt2;
	CTextUnit	  m_wndCylPt1Unit;
	CMouseEdit	m_wndCylPt1;
	CTextUnit	  m_wndPlanePt3Unit;
	CMouseEdit	m_wndPlanePt3;
	CTextUnit	  m_wndPlanePt2Unit;
	CMouseEdit	m_wndPlanePt2;
	CTextUnit	  m_wndPlanePt1Unit;
	CMouseEdit	m_wndPlanePt1;
	CTextUnit	  m_wndLinePt2Unit;
	CMouseEdit	m_wndLinePt2;
	CTextUnit	  m_wndLinePt1Unit;
	CMouseEdit	m_wndLinePt1;
	int		m_nDirection;
	int		m_wndThicknessDir;
	BOOL	m_chkReverseIJ;
	BOOL	m_bMoveSrc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElemExtrudeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	CArray<UINT,UINT> m_aTranslateCtrl;
	CArray<UINT,UINT> m_aRotateCtrl;
	CArray<UINT,UINT> m_aMatlCtrl;
	CArray<UINT,UINT> m_aSectCtrl;
	CArray<UINT,UINT> m_aThikCtrl;

	// For Translate
	CArray<UINT,UINT> m_aEqualCtrl;
	CArray<UINT,UINT> m_aUnEqualCtrl;
	CArray<UINT,UINT> m_aVectorCtrl;
	CArray<UINT,UINT> m_aThicknessCtrl;

	// For Rotate
	CArray<UINT,UINT> m_aRot2ndPtCtrl;

	// Sub Controls of Element
	CArray<UINT,UINT> m_TTCtrl;
	CArray<UINT,UINT> m_TTDistCtrl;
	CArray<UINT,UINT> m_TTForceCtrl;
	CArray<UINT,UINT> m_CTCtrl;
	CArray<UINT,UINT> m_CTDistCtrl;
	CArray<UINT,UINT> m_WallCtrl;

	CArray<UINT,UINT> m_TTRadioCtrl;
	CArray<UINT,UINT> m_CTRadioCtrl;
	CArray<UINT,UINT> m_WallRadioCtrl;

	CArray<UINT,UINT> m_arPlateTypeCtrl;

	// For Project
	CArray<UINT,UINT> m_aLineCtrl;
	CArray<UINT,UINT> m_aPlaneCtrl;
	CArray<UINT,UINT> m_aCylinderCtrl;
	CArray<UINT,UINT> m_aConeCtrl;
	CArray<UINT,UINT> m_aSphereCtrl;
	CArray<UINT,UINT> m_aEllipsoidCtrl;
	CArray<UINT,UINT> m_aElementCtrl;
	CArray<UINT,UINT> m_aDirPt2Ctrl;
	CArray<UINT,UINT> m_aFrameCtrl;

	void AlignControl();
	void ChangeStateOfElemTypeSubCtrls();
	void InitializeElemTypeSubCtrls();
	void InitializeProjectSubCtrls();
	void ShowHideProjectCtrls(BOOL flag);
	void ShowHideByGenType(int nGenerationType);
	void ShowHideElemAttr(int nElemType);
	void FillElemTypeByExtrudeType(int nExtrudeType);
	BOOL GetParamForTranslate(_ElemExtrude& EE);
	BOOL GetParamForRotate(_ElemExtrude& EE);
	BOOL GetParamForProject(_ElemExtrude& EE);
	BOOL GetParamElemType(_ElemExtrude &EE);
	BOOL Get1st2ndPoint(CMouseEdit& pt, double &Px, double &Py, double &Pz);
	BOOL GetDistanceDrDz(double &Dr, double &Dz);
	BOOL Get3Point(CMouseEdit& pt, double &Px, double &Py, double &Pz);

	// Generated message map functions
	//{{AFX_MSG(CElemExtrudeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeGT();
	afx_msg void OnSelchangeTmEtCombo();
	afx_msg void OnClickPropBtn();
	afx_msg void OnChangeElemType();
	afx_msg void OnChangeTranOption();
	afx_msg void OnChangeTranAxis();
	afx_msg void OnChangeRotAxis();
	afx_msg void OnTmTolerance();
	afx_msg void OnDeltaposTmRotAngleSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposTmIterNumSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeTT();
	afx_msg void OnChangeCT();
	afx_msg void OnSelchangeTmPrjtypeCbo();
	afx_msg void OnChangeDirection();
	afx_msg void OnTmEtRemoveCheck();
	afx_msg void OnTmEtMoveCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEMEXTRUDEDLG_H__BAFE50C8_DECC_11D3_92DE_0000C0B0E6B3__INCLUDED_)
