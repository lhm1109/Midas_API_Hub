#if !defined(__DGNCRCDGNPARAMETERDLG_H__)
#define __DGNCRCDGNPARAMETERDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnCRCDgnParameterDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "DgnPscCommon.h"

#include "..\MIT_frx\MComboBox.h"

#include "HeaderPre.h"

const int iDgn_PscID0 = 27;
const int iDgn_PscID1 = 43;
const int iDgn_PscID2 = 28;
const int iDgn_PscID3 = 15;
const int iDgn_PscID4 = 23;
const int iDgn_PscID5 = 13;  // Modify by GAY. MNET:2444. ('06.09.08). In case of JTG D62-04, delete Group Box.
const int iDgn_PscID6 = 12;  // by maxiao
const int iDgn_PscID7 = 29;
const int iDgn_PscID8 = 22;
const int iDgn_PscID9 = 15;
const int iDgn_PscID10=  1;
const int iDgn_PscID11=  1;
const int iDgn_PscID12=  1;
const int iDgn_PscID13=  1;
const int iDgn_PscID14=  1;
const int iDgn_PscID15=  1;
const int iDgn_PscID16=  1;
const int iDgn_PscID17=  1;
const int iDgn_PscID18=  1;
const int iDgn_PscID19=  1;
const int iDgn_PscID20=  1;

const int iIDPRINT0 = 5, iPOS0=2;
const int iIDPRINT1 = 5, iPOS1=8;
const int iIDPRINT2 = 8, iPOS2=14;
const int iIDPRINT3 = 5, iPOS3=23;
const int iIDPRINT4 = 4, iPOS4=29;
const int iIDPRINT5 = 2, iPOS5=34;
const int iIDPRINT6 = 4, iPOS8=37; // CRC

const int iIDPRINTSERV = 5, iPOS6=2;
const int iIDPRINTULTI = 5, iPOS7=8;

// Modify by GAY. MNET:2444. ('06.09.08). Change ID Count. (5+5)->(6+4)
const int iIDPRINTSERV_JTG = 6, iPOS9=1;
const int iIDPRINTULTI_JTG = 4, iPOS10=7;

// Add by sshan. MNET:2728. TB 10002.3-2005 ('20070411)
const int iIDPRINTBEAM_TB = 14, iPOS11=2;
const int iIDPRINTCOLU_TB = 10, iPOS12=17;

const int iOUTPUTPARAM_SERVICE_AASHTO_COUNT = 6, iOUTPUTPARAM_SERVICE_AASHTO_POS = 2;
const int iOUTPUTPARAM_FACTORED_AASHTO_COUNT = 4, iOUTPUTPARAM_FACTORED_AASHTO_POS = 9;

/////////////////////////////////////////////////////////////////////////////
// CDgnConMatDlg dialog
class CMyChildDialog;
class __MY_EXT_CLASS__ CDgnCRCDgnParameterDlg : public CDialogMove
{
// Construction
public:
	CDgnCRCDgnParameterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDgnCRCDgnParameterDlg();   // standard destructor

// Dialog Data
	//{{AFX_DATA(CDgnConMatDlg)
#if defined(_US) || defined(_RUS)
	enum { IDD = IDD_DGN_CRC_DESIGN_PARAMETER_US };
#else
	enum { IDD = IDD_DGN_CRC_DESIGN_PARAMETER };
#endif
	mit::frx::MComboBox	m_DesignCode;
	mit::frx::MComboBox m_DesignAnnex;
	mit::frx::MComboBox m_DesignAmendment;

	int m_iDimType;
	int m_iBrgType;
	BOOL m_bCmpWeb;
	int m_iCrackCode;
	int m_iTCrackWidth;
	int m_iBCrackWidth;
	int m_iPsReduFactor;
	int m_iBrgDir;
	int m_iDimTypeKsce;
	int m_iBrgTypeKsce;
	int m_iTCrackWidthKsce;
	int m_iBCrackWidthKsce;

	int m_ifpsMethod;  //KSCE-USD
	int m_iTndnKind;
	
	int m_iDimTypeJTG;
	int m_iBrgTypeJTG;
	int m_iBrgGradeJTG;
	int m_iCastTypeJTG;
	int m_iSBarCrackWidthJTG;
	int m_iWSCrackWidthJTG;

	// Added by NM. (01.04.08)
	int m_iTndnTypeAASHTO;
	int m_iBrgTypeAASHTO;
	int m_iConstructionTypeAASHTO;
	int m_iCorrosiveTypeAASHTO;
	int m_iExposureTypeAASHTO;
	int m_iFlexureCalcTypeAASHTO;	

	BOOL m_bDesignPrint[iIDPRINT0];
	BOOL m_bStructPrint[iIDPRINT1];
	BOOL m_bMomentPrint[iIDPRINT2];
	BOOL m_bShearPrint[iIDPRINT3];
	BOOL m_bTorsionPrint[iIDPRINT4];
	BOOL m_bEtcPrint[iIDPRINT5];
	BOOL m_bBckPrint[iIDPRINT6];
	
	BOOL m_bServPrint[iIDPRINTSERV];
	BOOL m_bUltiPrint[iIDPRINTULTI];

	BOOL m_bServPrintJTG[iIDPRINTSERV_JTG];
	BOOL m_bUltiPrintJTG[iIDPRINTULTI_JTG];

	// Add by sshan. MNET:2728. TB 10002.3-2005 ('20070411)
	int m_iDimTypeTB;
	int m_iMemTypeTB;
	int m_iCastTypeTB;
	BOOL m_bBeamPrintTB[iIDPRINTBEAM_TB];
	BOOL m_bColuPrintTB[iIDPRINTCOLU_TB];

	BOOL m_bRefRebar; //KSCE

	BOOL m_bPrintParam_Service_AASHTO[iOUTPUTPARAM_SERVICE_AASHTO_COUNT];
	BOOL m_bPrintParam_Factored_AASHTO[iOUTPUTPARAM_FACTORED_AASHTO_COUNT];

	CEditUnit	m_dExposureFactor;

	//
	CBCGPStatic	m_wndPlaceHolder;
	//}}AFX_DATA
	
	int m_iID0[iDgn_PscID0];
	int m_iID1[iDgn_PscID1];
	int m_iID2[iDgn_PscID2];
	int m_iID3[iDgn_PscID3];
	int m_iID4[iDgn_PscID4];
	int m_iID5[iDgn_PscID5];
	
	// Add by sshan. MNET:2728. TB 10002.3-2005 ('20070411)
	int m_iID6[iDgn_PscID6];
	int m_iID7[iDgn_PscID7];
	int m_iID8[iDgn_PscID8];
	int m_iID9[iDgn_PscID9];
	int m_iID10[iDgn_PscID10];
	int m_iID11[iDgn_PscID11];
	int m_iID12[iDgn_PscID12];
	int m_iID13[iDgn_PscID13];
	int m_iID14[iDgn_PscID14];
	int m_iID15[iDgn_PscID15];
	int m_iID16[iDgn_PscID16];
	int m_iID17[iDgn_PscID17];
	int m_iID18[iDgn_PscID18];
	int m_iID19[iDgn_PscID19];
	int m_iID20[iDgn_PscID20];

public:

	void Initial_AnnexAASHTO_Ctrl();
	void Initial_Data();
	void Initial_ExistData();
	void Inittal_Amendment(int iDgnCode, int nNationalAnnex);
	void ShowHIdeAnnexCtrl();
	void ChangeDesignCode();
	void EnableCtrls();
	CRect MoveCtrl(int iKind, int* pID, CRect Rect0);
    void MoveDlgOkCn(CRect RectOrg);
	
//  void UpdateCurDlg(int iDgnCode);
	// Code별 함수 분리..
	void UpDateCurDlg_EC2(int iDgnCode);
	void UpDateCurDlg_BS();
	void UpDateCurDlg_TMH7();
	void UpDateCurDlg_CSA();
	void UpDateCurDlg_CSA19();
	void UpDateCurDlg_AASHTO12();
	void UpDateCurDlg_LSD15();
	void UpdateCurDlg_SNiP();
	void UpdateCurDlg_SP();
	void UpDateCurDlg_AS5100();
	void UpDateCurDlg_IRS();
	void UpDateCurDlg_ABNT();

	int m_nCurrentDlg;
	CArray<int,int> m_aKey;
	CArray<int,int> m_aDgnCode;
	CMap <int,int,CMyChildDialog*,CMyChildDialog*> m_mapChildDlg;
	//
	//void SetDefaultDatabyNationalAnnex(T_PSCD_D& PscdD);

private:
	double GetExposureFactorAASHTO();
	void SetExposureFactorAASHTO(int iExposureType);

protected:
	int m_iDgnCode;
	int m_nAnnex;

	T_PSCD_D  m_Data;
	T_PSCD_D* pParamData;
	
	BOOL Dlg2Data();
	BOOL Data2Dlg();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConMatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnConMatDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnSelchangeDgnCtrldataDesigncode();
	afx_msg void OnSelchangeDgnCtrldataDesignAnnex();
	afx_msg void OnSelchangeDgnCtrldataDesignAmendment();
	afx_msg void OnClickSelectAll();
	afx_msg void OnClickUnSelectAll();
	afx_msg void OnClickSelectAllKsce();
	afx_msg void OnClickUnSelectAllKsce();
	afx_msg void OnClickSelectAllJtg();
	afx_msg void OnClickUnSelectAllJtg();
	afx_msg void OnClickSelectAllTB();
	afx_msg void OnClickUnSelectAllTB();
	afx_msg void OnClickOption();
	afx_msg void OnClickSelectAllAASHTO();
	afx_msg void OnClickUnSelectAllAASHTO();
	afx_msg void OnClickSpecialLoad();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCRCDGNPARAMETERDLG_H__4B768343_DD95_11D3_888F_0000C0F30D4D__INCLUDED_)
