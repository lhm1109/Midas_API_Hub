 #if !defined(AFX_CMDAUTOLOADCOMBDLG_H__9E765C21_0CC3_11D4_888F_0000C0F30D4D__INCLUDED_)
#define AFX_CMDAUTOLOADCOMBDLG_H__9E765C21_0CC3_11D4_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\mit_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "LoadCombCtrl.h"
using namespace mit::frx;
class CDBDoc;
class CCmdAutoLoadCombDlg : public CDialogMove
{
	// Construction
public:
	CCmdAutoLoadCombDlg(CWnd* pParent = NULL);   // standard constructor

	enum EN_CODE_SELECT
	{
		EN_STEEL      = 0,
		EN_CONCRETE   = 1,
		EN_SRC        = 2,
		EN_FOOTING    = 3,
		EN_ALUMINUM   = 4,
		EB_CFSTL      = 5,
	};

	enum EN_SEIS_ORTHO_TYPE
	{
		EN_SEIS_ORTHO_NONE   = 0,
		EN_SEIS_ORTHO_100_30 = 1, // support only 100:30 rule.
		EN_SEIS_ORTHO_SRSS   = 2, // support only SRSS.
		EN_SEIS_ORTHO_BOTH   = 3  // support both 100:30 and SRSS.
	};

	enum EN_SEIS_LCOMADD_TYPE
	{
		EN_SEIS_LCOMADD_NONE = 0,
		EN_SEIS_LCOMADD_SPEC = 1, // support only special seismic load.
		EN_SEIS_LCOMADD_VERT = 2, // support only vertical seismic forces.
		EN_SEIS_LCOMADD_BOTH = 3
	};

	
public:
	void SetLcomType(int nLcomType) { m_nLcomType = nLcomType; }

	// Dialog Data
	//{{AFX_DATA(CCmdAutoLoadCombDlg)
	enum { IDD = IDD_CMD_LCOMBDEFAULT_DLG };
	MComboBox	m_SpecLcase;
	CListCtrl	m_CtrlList;
	MComboBox	m_Code;
	MComboBox	m_cmbNationalAnnex;
	MComboBox	m_cmbLoadCode;
	CFoldGroupBox_Cross	m_wndScaleUpFrame;
	CFoldGroupBox_Cross	m_wndManipulationFrame;
	int		    m_nOption;
	int       m_nDesignType;
	double    m_Factor;	
	CEditUnit m_wndLossFactorTransfer;
	CEditUnit m_wndLossFactorService;
	MButton   m_wndLossFactorChk;
	MButton   m_wndCSAnalysisChk;
	BOOL      m_bAddEnvelope;
	int       m_nConstLoad;
	//add by cylee 05.04.22
	MButton   m_wndOthoChk;
	int       m_nOthoType;
	//add by cylee 05.06.15
	MButton   m_wndSpecialChk;
	MButton   m_wndVertForceChk;
	double    m_dLLPsi0;
	double    m_dLLPsi1;
	double    m_dLLPsi2;
	double    m_dWLPsi0;
	double    m_dWLPsi1;
	double    m_dWLPsi2;
	double    m_dSLPsi0;
	double    m_dSLPsi1;
	double    m_dSLPsi2;
	double    m_dTLPsi0;
	double    m_dTLPsi1;
	double    m_dTLPsi2;
	double    m_dGammaG;
	double    m_dGammaQ;
	// Coded by Seungjun ('20071204) MNet:No.3095.
	double    m_dAlpha_H;
	MComboBox	m_CobxStrength;
	MComboBox	m_CobxServiceability;
	MButton   m_wndHorEarthChk;
	MButton   m_wndUnderLoadChk;
	BOOL	  m_bUseUnderLoadSeisFactor;
	CEditUnit m_edtGammaL;
	CEditUnit m_edtReduFactor;

	MButton   m_wndLLReduFactor;	// IDC_CMD_LL_REDUN_LOAD_FACTOR_CHK
	MButton   m_wndReduFactor;		// IDC_CMD_REDUN_LOAD_FACTOR_CHK

	// Add by GAY. PMS:4267. ('11.12.12). 중국신기준 추가 : 수직지진하중을 주하중으로 고려.
	MButton   m_wndVertSeisMainChk;  // IDC_CMD_CONSIDER_VERT_SEIS_MAIN_CHK

	// Add by GAY. PMS:4450. ('12.03.20). Add ACI318-11.
	// Define load level of wind load and seismic load (Strength-level, Service-level).
	// Define lateral soil pressure load factor.
	int       m_nWindLevel;
	int       m_nSeisLevel;
	MComboBox m_cmbLSPfact;    // Lateral soil pressure load fator.
	MButton   m_wndLSPfactChk; // IDC_CMD_LATERAL_SOIL_FACTOR_CHK

	MButton   m_chkImpLoad;

	//Coded by tss(201909)GB 51249-2017 
	MComboBox	m_CobxTempCond;	//Temperature condition
	double m_dPhif;
	double m_dPhiq;

	CArray<_LCOM_IMPF, _LCOM_IMPF&> m_aImperfectionLoad;

	int       m_nStructType;
	//}}AFX_DATA

public:
	CMap<int,int,double,double> m_ScaleData;  
	CMapEx<UINT, UINT, T_LPSI_D, T_LPSI_D&> m_mLcomPsiFactor;
	dgn::def::enNationalAnnex m_enNationalAnnex;
	bool m_bOver1000m; // for national annex

public:
	void Initial_Data();
	void Initial_LCaseComboBox();
	void Show_EditComboBox(BOOL bCheck);
	void Initial_ListCtrlBox();  
	void Write_ListCtrlBox(int Index,double Factor,CString strLoadCase);
	void OverWrite_ListCtrlBox(int Index,double Factor,CString strLoadCase);

	BOOL ErrorCheck();
	BOOL ErrorCheckPS();
	BOOL CheckSpectrumName(CString str,int& Index);

	void Set_LoadCode();
	void Set_InitNationalAnnex();
	void Set_InitCode();
	void Set_InitHorEarth();
	void Set_InitTemperatureCaseList();
protected:
	dgn::def::enNationalAnnex GetCurrentNationalAnnex();
	BOOL GetUsedStldKeyList(CArray<T_STLD_K, T_STLD_K>& aKeyList);
	void Initial_SoilFactor_ComboBox();
	void InitLcomPsiFactor();
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdAutoLoadCombDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	void AlignControl(CString strCodeName);

	void GetImperfectionCtrlState( CString strCurCode, UINT& uiTargetImpf, BOOL &bShowImpf);
	void GetNonDissipativeCtrlState( CString strCurCode, UINT& uiTargetND, BOOL &bShowND);
	void CtrlManager();
	void Set_InitEC();
	void EnableCtrlNationalAnnex();
	void SetInitAlignLoadCode();

	int GetSeisOrthoType(const CString& strCodeName);
	int GetSeisAddLcomType(const CString& strCodeName);

	int GetWindFactCtrls(CArray<UINT, UINT>& raCtrl);
	int GetSeisFactCtrls(CArray<UINT, UINT>& raCtrl);
	int GetSoilPresFactCtrls(CArray<UINT, UINT>& raCtrl);

	BOOL IsCodeWindFact(const CString& strCodeName);	
	BOOL IsCodeSeisFact(const CString& strCodeName);	
	BOOL IsCodeSoilPresFact(const CString& strCodeName);
	BOOL IsCodeStlTempFact(const CString& strCodeName);	//Add by tss(2019/09)
	BOOL IsConcludeTemperatureCase();//Add by tss(2019/09)
	BOOL IsCodeUnderLoad(CString strCodeName = _T("")); //Underground Load 인지
	void SetDefaultImpfData();
	BOOL GetLoadCombTypeByLoadCaseType(int nLoadCaseType, int& nAnalType);
	void ShowCBCtrl(CString strCurCode);
	void InitUnderLoadCtrl();
	void EnableUnderCtrl();
	void UseLoadCaseZCombo(CString& strCodeName);

	void GetPsi4WindLoad(double& dPsi0, double& dPsi1, double& dPsi2);
	void GetPsi4SnowLoad(double& dPsi0, double& dPsi1, double& dPsi2);

protected:
	CDBDoc* m_pDoc;
	CString m_strCodeName;
	CString m_strOldCodeName;
	CArray<UINT, UINT> m_aCtrlPS;
	CArray<UINT, UINT> m_aCtrlOkCancel;  
	int     m_nDistY;
	CArray<UINT, UINT> m_aCtrlScaleUp;
	CArray<UINT, UINT> m_aCtrlWindComb;  // Add by JWKWON. PMS:5002. ('16.04.01). for KBC2015
	CArray<UINT, UINT> m_aCtrlConstLoad;
	CArray<UINT, UINT> m_aCtrlEC2Phi;  // Coded by Seungjun MNet:No.2393 ('20060919)
	CArray<UINT, UINT> m_aCtrlSEISLCB; // Coded by Seungjun MNet:No.2393 ('20060919)
	CArray<UINT, UINT> m_aCtrlOrthoLCB;
	CArray<UINT, UINT> m_aCtrlGamma;   // Coded by Seungjun ('20070726).
	CArray<UINT, UINT> m_aCtrlPress;   // Coded by Seungjun ('20071204) MNet:No.3095.
	CArray<UINT, UINT> m_aCtrlGamma_L; // GB/T50010-10    // PMS:XXXX-HTLEE-20111014 
	CArray<UINT, UINT> m_aCtrlACI318_11; // Add by GAY. PMS:4450. ('12.03.20). for ACI318-11.
	CArray<UINT, UINT> m_aCtrlWindFact;
	CArray<UINT, UINT> m_aCtrlSeisFact;
	CArray<UINT, UINT> m_aCtrlSoilPresFact;
	CArray<UINT, UINT> m_aCtrlHorEarth;
	CArray<UINT, UINT> m_aCtrlImperfectLoad;
	CArray<UINT, UINT> m_aCtrlNonDissipative;
	CArray<UINT, UINT> m_aCtrlTemperature;
	CArray<UINT, UINT> m_aCtrlUnderLoad;
	CArray<UINT, UINT> m_aCtrlNSCP2015;
	CArray<UINT, UINT> m_aCtrlNTCCDE2017;
	CArray<UINT, UINT> m_aCtrlSP_20_13330_2016;

	CArray<UINT, UINT> m_aMoveCtrlID;
	CArray<BOOL, BOOL> m_aCodeEnable;
	int m_nOldCodeIndex;

	BOOL    m_bExistCS;
	BOOL    m_bExistEV;
	// Generated message map functions
	//{{AFX_MSG(CCmdAutoLoadCombDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdLoadcombAdd();
	afx_msg void OnCmdLoadcombReplace();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCmdScaleupAdd();
	afx_msg void OnCmdScaleupDel();
	afx_msg void OnCmdScaleupModify();
	afx_msg void OnClickCmdScaleupList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedCmdScaleupList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdLossFactorChk();
	afx_msg void OnCmdDesignTypeRdo();
	afx_msg void OnCmdLossFactorBtn();
	//add by cylee 05.04.22
	afx_msg void OnCmdSetOtholcBtn();
	afx_msg void OnCmdOthoChk();
	afx_msg void OnCmdOthoType();
	//add by cylee 05.05.13
	afx_msg void OnCodeSelchange();
	//add by cylee 05.06.15
	afx_msg void OnCmdSpecialVertChk();
	afx_msg void OnCmdSeisFactorBtn();
	afx_msg void OnCmdUnderLoadBtn();
	afx_msg void OnCmdAutoAlphaHBtn();

	afx_msg void OnCmdHorEarthChk();
	afx_msg void OnCmdLLRedunBtn();
	afx_msg void OnCmdLLRedunChk();
	afx_msg void OnCmdRedunChk();
	// Add by GAY. PMS:4450. ('12.03.20). for ACI318-11.
	afx_msg void OnCmdWindLevel();
	afx_msg void OnCmdSeisLevel();
	afx_msg void OnCmdSoilFactorChk();

	afx_msg void OnCmdServFactorBtn();
	afx_msg void OnNASelchange();
	afx_msg void OnCmdWindDirectionBtn();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);

	afx_msg void OnCmdImpLoadChk();  
	afx_msg void OnCmdImpLoadBtn();

	afx_msg void OnCmdLiveLoadTypeBtn();

	afx_msg void OnCmdTemperatureCaseSelChange();
	afx_msg void OnCmdUnderLoadChk();

	afx_msg void OnCmdSP20ReliabilityBtn();
	afx_msg void OnCmdSP20LinkChk();
	afx_msg void OnCmdSP20LinkBtn();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	

private:
	int m_nLcomType;
	BOOL m_bUseLoadCaseZCombo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMDAUTOLOADCOMBDLG_H__9E765C21_0CC3_11D4_888F_0000C0F30D4D__INCLUDED_)
