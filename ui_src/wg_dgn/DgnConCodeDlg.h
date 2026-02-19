#if !defined(__DGNCONCODEDLG_H__)
#define __DGNCONCODEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConCodeDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

#include "HeaderPre.h"

const int iDgn_ConID0 = 3;
const int iDgn_ConID1 = 4;
const int iDgn_ConID2 = 4;
const int iDgn_ConID3 = 7;
const int iDgn_ConID4 = 3;
const int iDgn_ConID6 = 5;
const int iDgn_ConID7 = 22;
const int iDgn_ConID8 = 4;
const int iDgn_ConID9 = 19;     //Eurocode2:04
const int iDgn_ConID10 = 12;     //KCI-USD07
const int iDgn_ConID10_part = 4;//KCI-USD07
const int iDgn_ConID11 = 6;     //AASHTO-LRFD07(Interim 2009)
const int iDgn_ConID12 = 3;     // IS 456 Ductile COde Additions
const int iDgn_ConID13 = 2;		//IS3370 Civil

using namespace mit::frx;
class CDgnDataCtrl;
class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeDlg dialog
class __MY_EXT_CLASS__ CDgnConCodeDlg : public CDialogMove
{
// Construction
public:
	CDgnConCodeDlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~CDgnConCodeDlg();

// Dialog Data
	//{{AFX_DATA(CDgnConCodeDlg)
	enum { IDD = IDD_DGN_CON_CODE_DLG };
	MComboBox	m_DesignCode;
	BOOL	  m_bSpecialEQ;
	int		  m_iColmMethod;
	int		  m_iSafeLevel;
	int		  m_iEqGrade;
	int		  m_iPmCurve;
	int     m_iEqCategory;
	int     m_iSlabType;
	double  m_dMrft;
	double  m_dTrft;
	int     m_iAlphaType1;
	double  m_dAlpha1;
	double  m_dAlpha2;
	double  m_dConShearStrengthRatio;
	int     m_iFrameType;
	MComboBox	    m_cmbGroup;
	MComboBox	    m_cmbGroup_KCIUSD07;
	int		  m_iDuctility;
	double  m_dStorColmWeakBeamFactor;
	double  m_dGamma_rdForBeam;
	double  m_dGamma_rdForColm;
	double  m_dGamma_rdForWall;
	double  m_dGamma_rdForJoint;
	BOOL	  m_bTorsionDesign;
	BOOL	  m_bBCJointDgn;
	BOOL	  m_bNTCFlag;// PMS:DNKIM-20121030 NTC2012 추가
	BOOL    m_bUseVcWall;
	BOOL    m_bUseVcColm;
	BOOL    m_bUseVcBeam;

	MComboBox	m_ComboNationAnnex;
	double    m_dUf;
	MComboBox m_DesignAnnex;

	MButton	  m_btnTorsion;

	MButton   m_chkApplyNTC;
	CEditUnit m_edtStrutAngle;
	CEditUnit m_edtEffectivePhi;
	MButton   m_chkApplyBCJDesign;

	MButton    m_ChkUseVcWall;
	MButton    m_ChkUseVcColm;
	MButton    m_ChkUseVcBeam;
	//Pinakin added for IS456 MAt design
	CEditUnit m_dedtFOS; 
	CEditUnit m_dedtSubRex;
	CTextUnit	m_ctrlUnit;
	int m_iIs13920Sel;
	MButton m_chkIS3370;
	int m_iIS3370Sel;
	BOOL m_bIS3370AnnexA;//If TRUE Annex A ; else Annex B
	MComboBox m_cmbIS3370AnnexB2mm;
	CEditUnit m_dedtESS; 
	CEditUnit m_dedtETT;
	CEditUnit m_dedtT1;
//   CButton m_chkBehaviorFactor;
//   CEditUnit m_edtBehaviorFactor;

	CEditUnit m_edtSlendLimitA;
	CEditUnit m_edtSlendLimitB;
	CEditUnit m_edtSlendLimitC;
	MButton   m_chkAutoCalcA;
	MButton   m_chkAutoCalcC;

	BOOL      m_bSpecialWall;
	int       m_nSpecialWallMtd;  //0: DES, 1: DMO, 2: DMI
	int       m_nBndrElemMethod;
	MComboBox m_CmbCd;
	MComboBox m_CmbIe;
	double    m_dCd;
	double    m_dIe;

	int       m_nExposureFactor;
	int       m_nPouringMethod;

	BOOL      m_bSubBeam;
	BOOL      m_bCantilever;
	BOOL      m_bUnderBmColm;

	//(Tel. 4022) MNET:XXXX-BSCHOI-20121015 #F1
	MComboBox m_cmbNTCType;

	int       m_nExposure; // 0:Dry, 1:etc
	MComboBox m_cmbBridgeType;
	MComboBox m_CmbPL;
	MComboBox m_CmbSL;
	MComboBox m_CmbSType;
    CEditUnit m_EdtDg;
    CTextUnit m_UntDg;
    CEditUnit m_EdtKc;

	BOOL      m_bmb7mb8;
	int       m_nmb7mb8Type;
	CEditUnit m_edtmb7;
	CEditUnit m_edtmb8;
	BOOL      m_bmb9;
	int       m_nmb9Type;
	CEditUnit m_edtmb9;

	MButton    m_ChkLimitShearStrgConc;
	BOOL      m_bChkPM;
	

	//}}AFX_DATA
	int m_iID0[iDgn_ConID0];
	int m_iID1[iDgn_ConID1];
	int m_iID2[iDgn_ConID2];
	int m_iID3[iDgn_ConID3];
	int m_iID4[iDgn_ConID4];
	int m_iID6[iDgn_ConID6];
	int m_iID7[iDgn_ConID7];
	int m_iID8[iDgn_ConID8];	
	int m_iID9[iDgn_ConID9];
	int m_iID10[iDgn_ConID10];
	int m_iID10_part[iDgn_ConID10_part];
	int m_iID11[iDgn_ConID11];
	int m_iID12[iDgn_ConID12];
	int m_iID13[iDgn_ConID13];

	CArray<UINT,UINT> m_aStrutAngleCtrls;
	CArray<UINT,UINT> m_aEffectivePhiCtrls;
	CArray<UINT,UINT> m_aSlendernessCtrls;
	CArray<UINT,UINT> m_aCtrlSCWBMethod;
	CArray<UINT,UINT> m_aRdoSCWBMethod;
	CArray<UINT,UINT> m_aBCJointGroup;
	CArray<UINT,UINT> m_aSlendernessCtrlsNTC2012;
	CArray<UINT,UINT> m_aUseVc;
	CArray<UINT,UINT> m_aNTCCheckCombo;
	CArray<UINT,UINT> m_aSpecialProvision;
	CArray<UINT,UINT> m_aExposure;
	CArray<UINT,UINT> m_aCondiFactor;
	CArray<UINT,UINT> m_aCtrlWallBoundMtd;
	CArray<UINT,UINT> m_aRdoIS1392Sel;
	CArray<UINT,UINT> m_aCtrlIS3370;
	CArray<UINT,UINT> m_aCtrlIS3370_GrpA;
	CArray<UINT,UINT> m_aCtrlIS3370_GrpB;
	CArray<UINT,UINT> m_aCtrlIS3370_RadAB;
	CArray<UINT,UINT> m_aRdoBeamDgn;
	CArray<UINT,UINT> m_aRdoColumnDgn;
	CArray<UINT,UINT> m_aAnnexCtrls;

	T_KEY m_GroupKey; 

	CString m_strDCH;
	CString m_strDCM;
protected:
	CDgnDataCtrl* m_pDgnDataCtrl;

public:
	void Initial_ExistData();
	void Initial_Data();
	void EnableDisableControls();
	CRect MoveCtrl(int iKind, int* pID, CRect Rect0);
	void ChangeDlgItemInfo();
	int GetDataStatus(CString strCode);
	void 	SelchangeDgnCtrldataDesigncode(BOOL bUpdateDef);

	void EnableWindowByBndrElemMethod();
	void EnableWindowBySpecialWall();

    void GetDataFromDlg(T_DCON_D& rData);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConCodeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	CArray<BOOL, BOOL> m_aCodeEnable;
	int m_nOldCodeIndex;
	int m_nOldAnnexIdx;
	
	//CMap<T_GRUP_K,T_GRUP_K,T_GRUP_D,T_GRUP_D&>  m_arAllGroup;
	
//   CArray<UINT, UINT> m_aAllGroupK;
//   CArray<UINT, UINT> m_aExcludeGroupK;
//   CArray<UINT, UINT> m_aNotSFSRGroupK;

	T_DCON_D m_DconEC8Data;

	BOOL m_bExistData;  // PMS:xxxx-Seungjun-20100422 저장된 데이터가 있는 경우 초기값 세팅하지 않음.

	void SetComboCdAndIe();
	void SetComboCdAngle_ACI318();
	void SetComboAnnexAASHTO();
	void SetComboAnnexAS();
	void SDRControl();

    void SetNTC2008EnableControl();
    void SetEC2Ctrl(int nAnnex);
    void ShowEC2Ctrl(int nAnnex);
    void MoveEC2Ctrl(int nAnnex);
	void ShowBSCtrl();
    void MoveBSCtrl();

    void ShowAnnexCtrl(int nAnnex);
    void InitAnnexCtrl(int nAnnex);
    bool IsExistWmaxCtrl(int nAnnex) const;
    bool IsExistDgCtrl(int nAnnex) const;

	void SetBndrElemClimitDesc(const CString& strCodeName);

	BOOL IsValidDoubleNumber(CString value, int &nErrCode);
	BOOL ConvertFormulaToValue(CString& value);
	BOOL IsNumeric(CString str, int& ecode);

	// Generated message map functions
	//{{AFX_MSG(CDgnConCodeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDgnRcctrlSpecial();
	afx_msg void OnSelchangeDgnCtrldataDesigncode();
	afx_msg void OnSelchangeNTCType();
	afx_msg void OnChangeAlphaType();
	afx_msg void OnChangeFrameType();
	afx_msg void OnUpdateByCode();
	afx_msg void OnSetGrupCombo();
	afx_msg void OnDuctilityBtn();
	afx_msg void OnBtnGrup();
	afx_msg void OnBCJoint();
	//afx_msg void OnBtnNewGrup(); // MNET:4076-SHJUNG-20100609 - 이 기능 안 쓴다. 다른 옵션으로 대체.
	afx_msg void OnChangeTorsion();
	afx_msg void OnBtnEC8Data();
	afx_msg void OnSelchangeNationalAnnexCombo();
	afx_msg void OnChkApplyNTC();
	afx_msg void OnSelchangeDgnCtrldataDesignAnnexAashto();
//  afx_msg void OnChkBehaviorFactor();
	afx_msg void OnChkAutoCalcA();
	afx_msg void OnChkAutoCalcC();
	afx_msg void OnChkSpecialWall();
	afx_msg void OnChangeBndrElemMethodType();
	afx_msg void OnSelchangeCdCmb();
	afx_msg void OnSelchangeIeCmb();
	afx_msg void OnEditchangeCdCmb();
	afx_msg void OnEditchangeIeCmb();
	afx_msg void OnChangeExposureFactor();
	afx_msg void OnChangePouringMethod();
	afx_msg void OnDgnCtrlDataAdvBtn();
	afx_msg void OnChangeIS13920_DuctileCode();
	afx_msg void OnChangeIS3370_CrackCheck();

	afx_msg void OnChkUseVc();// PMS:DNKIM-20121030

	afx_msg void OnConditionFactormb7mb8Chk();
	afx_msg void OnConditionFactormb7mb8TypeRdo();
	afx_msg void OnConditionFactormb9Chk();
	afx_msg void OnConditionFactormb9TypeRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedDgnIs204N25();
	afx_msg void OnBnClickedDgnMb9Chk2();
	afx_msg void OnConditionIS3370TypeRdo();
	afx_msg void OnConditionIS3370AnnexBRdo();
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNCONCODEDLG_H__)
