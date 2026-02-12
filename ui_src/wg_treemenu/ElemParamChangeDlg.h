#if !defined(__ELEMPARAMCHANGEDLG_H__)
#define      __ELEMPARAMCHANGEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElemParamChangeDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_CobxMatl.h"
#include "..\wg_db\wg_db_CobxSect.h"
#include "..\wg_db\wg_db_CobxThik.h"
#include "..\wg_db\wg_db_EditMatl.h"
#include "..\wg_db\wg_db_EditSect.h"
#include "..\wg_db\wg_db_EditThik.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_EditWid.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CElemParamChangeDlg dialog
class _ElemParam
{
public:
	int m_nType;
	int m_nMode;
	UINT m_nKey;
	CString m_csName;

	int m_nElemType;
	int m_nIncNum;
	double m_dblBetaAngle;
	double m_dblBetaInc;
	int m_nBAngle;  // Truss, Tension Only, Compression Only, Beam, Plate, Plane Stress
	int m_nRefAngle; // Plane Strain, Axisymmetric, Solid
	double m_dBetaWx, m_dBetaWy, m_dBetaWz;

	// data for element type
	int m_nFromType, m_nToType, m_nSubType;
	double m_dSubData;
	double m_dSubData2; // MNET:2774 20070509 mylee - dLimit
	BOOL   m_bUseLimit;

	int m_nWallID;
	int m_nCableOpt;

	// data for reverse local
	int m_nElType;
	BOOL m_bReverse;

	// data for Align Element Local
	unsigned int m_nKeyStandardElem;
	int m_nSortOrder;

	T_SECT_SECTION_D* m_pSectData;   // ADD BY ZHOUMIN  20151016
	int m_nMode2;
	int m_nAxis;
	int m_nCoordDir;
	double m_dPointx, m_dPointy, m_dPointz;
	T_NUCS_K m_NuckK;
};

class CElemParamChangeRefVecDlg;

class CElemParamChangeDlg : public CMenuBarChildDlg
{
	// Construction
public:
	CElemParamChangeDlg(CWnd* pParent = NULL);   // standard constructor
	~CElemParamChangeDlg();
	virtual void Execute();

	BOOL GetParameter(_ElemParam& EP);
	BOOL ChangeParam(_ElemParam& EP);
	void CtrlShowHide(CArray<UINT, UINT>& aCtrl, BOOL bShow);
	void AlignControl();
	void AssignCtrlMan();

	CArray<UINT, UINT> m_MatlCtrl;
	CArray<UINT, UINT> m_SectCtrl;
	CArray<UINT, UINT> m_ThikCtrl;
	CArray<UINT, UINT> m_BetaCtrl;
	CArray<UINT, UINT> m_ModeCtrl;
	CArray<UINT, UINT> m_AssignCtrl;
	CArray<UINT, UINT> m_ChangeCtrl;
	CArray<UINT, UINT> m_AssignCtrl2;
	CArray<UINT, UINT> m_ChangeCtrl2;
	CArray<UINT, UINT> m_CoordDirCtrl;
	CArray<UINT, UINT> m_CopyCtrl;
	CArray<UINT, UINT> m_RefAngleAxis;
	CArray<UINT, UINT> m_aRefAngle;
	CArray<UINT, UINT> m_aRefAxis;

	CArray<UINT, UINT> m_ElemTypeCtrl;
	CArray<UINT, UINT> m_FromToCtrl;
	CArray<UINT, UINT> m_TTCtrl;
	CArray<UINT, UINT> m_TTDistCtrl;
	//CArray<UINT,UINT> m_TTForceCtrl;
	CArray<UINT, UINT> m_TTAlCompCtrl;
	CArray<UINT, UINT> m_TTCableCtrl1;
	CArray<UINT, UINT> m_TTCableCtrl2;

	CArray<UINT, UINT> m_CTCtrl;
	CArray<UINT, UINT> m_CTDistCtrl;
	CArray<UINT, UINT> m_CTAlTensCtrl;
	CArray<UINT, UINT> m_WallCtrl;
	CArray<UINT, UINT> m_RevCtrl;

	CArray<UINT, UINT> m_TTRadioCtrl;
	CArray<UINT, UINT> m_CTRadioCtrl;
	CArray<UINT, UINT> m_WallRadioCtrl;
	CArray<UINT, UINT> m_RevRadioCtrl;

	CArray<UINT, UINT> m_ModeRadioCtrl;

	CArray<UINT, UINT> m_ThikRadioCtrl;
	CArray<UINT, UINT> m_PlaneSolidCtrl;

	CArray<UINT, UINT> m_arWallCtrl;
	CArray<UINT, UINT> m_arAlignElemCtrl;
	CArray<UINT, UINT> m_arEccentCtrl;       // ADD BY ZHOUMIN  
	CArray<UINT, UINT> m_arLastCtrl;

	CArray<UINT, UINT> m_arPlateTypeCtrl; // Plate Type Control

	CArray<UINT, UINT> m_arPlstrsTypeCtrl;

	T_SECT_SECTION_D m_SectData;   // ADD BY ZHOUMIN  20151016
	BOOL  m_bEccentChangeFlag;
	int m_nElemType;
	int m_nCoordDir;

	// Dialog Data
		//{{AFX_DATA(CElemParamChangeDlg)
	enum { IDD = IDD_TM_ELEM_PARAMETER };
	CEditUnit m_wndTTLengthEdit;
	CTextUnit m_wndTTLengthUnit;
	CEditUnit	m_wndTTLnLEdit;
	MComboBox	m_wndTTLnLCobx;
	MComboBox	m_cboWall;
	CEditWid	m_editWall;
	CMouseEdit	m_wndRefPoint;
	CTextUnit	m_wndRefPointDistUnit;
	MEdit m_edtWallInc;
	MComboBox	m_cboOrder3;
	MComboBox	m_cboOrder2;
	MComboBox	m_cboOrder1;            // For Align Local
	CMouseEdit	m_edtAlignLocal;
	CFormulaEdit m_wndWallID;
	CEditUnit	 m_wndTTForceEdit;
	CMouseEdit m_wndCTDistEdit;
	CMouseEdit m_wndTTDistEdit;
	CMouseEdit m_wndDirVectorEdit;
	CTextUnit	m_wndCTDistUnit;
	CTextUnit	m_wndTTForceUnit;
	CTextUnit	m_wndTTDistUnit;
	CTextUnit m_wndVectorUnit;

	CEditUnit	m_wndTTAlCompEdit;
	CEditUnit	m_wndCTAlTensEdit;
	CEditUnit	m_edtCompLimit; // MNET:2774 20070509 mylee
	CEditUnit	m_edtTensLimit;
	CTextUnit	m_wndCTAlTensUnit;
	CTextUnit	m_wndTTAlCompUnit;
	CTextUnit	m_untCompLimit; // MNET:2774 20070509 mylee
	CTextUnit	m_untTensLimit;

	MComboBox	m_wndElTypeTo;
	MComboBox	m_wndElTypeFrom;
	MButton	m_wndRevCheck;
	MComboBox	m_wndBetaAngle;
	MButton	m_wndThikBtn;
	MButton	m_wndSectBtn;
	MButton	m_wndMatlBtn;
	CCobxThik	m_wndThikName;
	CEditThik	m_wndThikNo;
	CCobxSect	m_wndSectName;
	CEditSect	m_wndSectNo;
	CCobxMatl	m_wndMatlName;
	CEditMatl	m_wndMatlNo;
	int		m_nType;
	int		m_nMode;
	CString	m_csNoInc;
	CString	m_csBetaAngle;
	CString	m_csBetaInc;
	int		m_nBetaAngleAssignType;
	int		m_nDefOrCopy;
	int		m_nCopyType;
	MComboBox m_cmbElemType;
	int   m_nMode2;
	int   m_nAxis;
	MComboBox m_cmbCoordDir;
	CMouseEdit m_edtPoint;
	CTextUnit	m_untPoint;
	int		   m_nRefAngle;
	MComboBox  m_edtRefAngle;
	CTextUnit  m_untRefAngle;
	MComboBox  m_cmbRefAxis;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElemParamChangeDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitComboNucs();
	void ChangeStateOfElemTypeSubCtrls(int nType);
	void InitializeElemTypeSubCtrls();
	void SetFromComboBox();
	void SetToComboBox(int nFromElTypeID);
	void SetOrder();                        // For Align Local
	int GetWallMode();
//	void AlignControlLocalAxis(int nElemType);
	void ChangeText();
	void ShowHideCtrl_MEC(BOOL bShowFirstModeGrpCtrl);

	BOOL ChangeParam_BetaAngle(CArray<UINT, UINT>& aKeyList, _ElemParam& EP);
	BOOL ChangeParam_BetaAngle_MEC(CArray<UINT, UINT>& aKeyList, _ElemParam& EP);
	BOOL ChangeParam_ElemType(CArray<UINT, UINT>& aKeyList, _ElemParam& EP);
	BOOL ChangeParam_ElemRev(CArray<UINT, UINT>& aKeyList, _ElemParam& EP);
	BOOL ChangeParam_ElemAlignLocal(CArray<UINT, UINT>& aKeyList, _ElemParam& EP);
	BOOL ChangeParam_Eccent(CArray<UINT, UINT>& aKeyList, _ElemParam& EP);  // ADD BY ZHOUMIN 
	BOOL GetParamElemType(_ElemParam& EP);
	BOOL GetParamReverseLocal(_ElemParam& EP);
	BOOL GetParamAlignLocal(_ElemParam& EP);
	BOOL GetParameter_MEC(_ElemParam& EP);
	BOOL GetParamEccent(_ElemParam& EP);       // ADD BY ZHOUMIN 
	BOOL GetKNodeDatas_MEC();
	BOOL GetKNodeDatas(int& nElemType, int& nBAngle, int& nRefAngle, double& BAngle, double& RPWx, double& RPWy, double& RPWz,
		int& nAxis, int& nCoordDir, double& dPointx, double& dPointy, double& dPointz, T_NUCS_K& NucsK);
	BOOL GetKNodeDatas_MEC(int& nElemType, int& nBAngle, int& nRefAngle, double& BAngle, double& RPWx, double& RPWy, double& RPWz,
		int& nAxis, int& nCoordDir, double& dPointx, double& dPointy, double& dPointz, T_NUCS_K& NucsK);
	void ChangeElemTypeCombo_MEC();
	CString sOrder;

	CElemParamChangeRefVecDlg* m_pRefVecDlg;

	// 새로 추가 
	void ChangeTensCableCombo();
	void InitTensCableCombo();
	int  GetTensCableKind();

public:
	void SetBetaVectorEdit(CString str);

	// Generated message map functions
	//{{AFX_MSG(CElemParamChangeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmPtMatl();
	afx_msg void OnTmPtSect();
	afx_msg void OnTmPtThik();
	afx_msg void OnTmModeAssign();
	afx_msg void OnTmModeChange();
	afx_msg void OnTmMatlBtn();
	afx_msg void OnTmSectBtn();
	afx_msg void OnTmThikBtn();
	afx_msg void OnTmPtBeta();
	afx_msg void OnTmPtElemType();
	afx_msg void OnTmPtReverse();
	afx_msg void OnChangeFromType();
	afx_msg void OnChangeToType();
	afx_msg void OnChangeTT();
	afx_msg void OnChangeCT();
	afx_msg void OnTmRevFrame();
	afx_msg void OnTmRevNoFrame();
	afx_msg void OnTmPtWall();
	afx_msg void OnTmPtAlignelem();
	afx_msg void OnSelchangeTmOrder1st();
	afx_msg void OnSelchangeTmOrder2nd();

	afx_msg void OnTmBetaTypeRadio_BetaAngle();
	afx_msg void OnTmBetaTypeRadio_RefPoint();
	afx_msg void OnTmBetaTypeRadio_Vector();
	afx_msg void OnTmBetaTypeRadio_Coord();
	afx_msg void OnTmBetaTypeRadio();
	afx_msg void OnTmWIdAutoincCheck();
	afx_msg void OnTmDefCopyRadio();
	afx_msg void OnSelchangeTmWallAssignCombo();
	afx_msg void OnSelchangeTmTtLnlCombo();
	afx_msg void OnTmClickTensLimit(); // MNET:XXXX 20070531 mylee
	afx_msg void OnTmClickCompLimit();
	afx_msg void OnTmClickRefVectorBtn();
	afx_msg void OnChangeEccent();  //  ZHOUMIN 20151016
	afx_msg void OnTmEccentBtn();   //  ZHOUMIN 20151016
	afx_msg void OnTmElemTypeCombo();
	afx_msg void OnTmModeAssign2();
	afx_msg void OnTmModeChange2();
	afx_msg void OnTmLocalAxisRdo();
	afx_msg void OnTmRefAngleAxisRdo();
	afx_msg void OnTmRefDirCmb();
	afx_msg void OnTmRefButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEMPARAMCHANGEDLG_H__527919E7_99A8_11D3_8899_0000C0A2329D__INCLUDED_)
