#if !defined(AFX_CMHCRPDLG_H__2B112166_2744_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CMHCRPDLG_H__2B112166_2744_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
	// CMHcrpDlg.h : header file
	//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxTdmf.h"

#include "CMTdmtJapan.h"
#include "CMTdmtChJTG04.h"
#include "CMTdmtAASHTO.h"
#include "CMTdmtEuropean.h"
#include "CMTdmtIndia.h"
#include "CMTdmtIndia2011.h"
#include "CMTdmtIndia2011_2020.h"
#include "CMTdmtAUSTRALIA.h"
#include "CMTdmtRus.h"
#include "CMTdmtChJTG16.h"
#include "CMTdmtNEWZEALAND.h"
#include "CMTdmtKDS2016.h"
#include "CMTdmtJSCE07.h"
#include "CMTdmtJSCE12.h"
#include "CMTdmt_ABNT_NBR_6118.h"

#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\McomboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\wg_base\SpinBtnExCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCMHcrpDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMHcrpDlg : public CDialogMove
{
protected:
	BOOL m_bIsTimeDependentMatMode; // Time dependent material type 입력 모드 인가..
	int  m_nOptionForTimeDep;       // (0) Add (1) Modify

	// Construction
public:
	void SetTimeDepMatMode(BOOL bIsTimeDepMatMode, int nOPMode);
	BOOL SetCurTDMTData(T_TDMT_D * pTdmt);

	CCMHcrpDlg(CWnd* pParent = NULL);   // standard constructor

public:
	////////////////////////////////////////////////////
	// 차후 Type dependent material data 로 변경...
	// 관계설정은 DB Data 결정 되면 수행한다.

	T_TDMT_D m_OriData;
	T_TDMT_D m_CurData;
	T_TDMT_D m_data;
	CString  m_strOldMatName;

	// Dialog Data
	//{{AFX_DATA(CCMHcrpDlg)
	enum { IDD = IDD_ETC_HCRP };
	/*CSpinButtonCtrl	m_jsceRhSpin;*/
	CFormulaEditSpin	m_jsceRh;
	/*CSpinButtonCtrl	m_jsceAcSpin;*/
	CFormulaEditSpin	m_jsceAc;
	CEditUnit	m_jsceWc;
	CEditUnit	m_jsceCc;
	CEditUnit	m_jsceVs;
	CTextUnit m_jsceCcUnit;
	CTextUnit m_jsceWcUnit;
	CTextUnit m_jsceVsUnit;
	/*CSpinButtonCtrl	m_chinaAgespin;*/
	CFormulaEditSpin	m_chinaAge;
	/*CSpinButtonCtrl	m_chinaRelaspin;*/
	CFormulaEditSpin	m_chinaRela;
	CTextUnit	m_chinaRelaunit;
	CTextUnit	m_chinaNotaunit;
	CEditUnit	m_chinaNota;
	CTextUnit	m_chinaCompunit;
	CEditUnit	m_chinaComp;
	CTextUnit	m_chinaAgeunit;
	
	/*CSpinButtonCtrl	m_jpanrelaspin;*/
	CFormulaEditSpin	m_jpanrela;
	CTextUnit	m_jpannotaunit;
	CEditUnit	m_jpannota;
	CTextUnit	m_jpancompunit;
	CEditUnit	m_jpancomp;
	CTextUnit	m_jpanageunit;
	/*CSpinButtonCtrl	m_jpanagespin;*/
	CFormulaEditSpin	m_jpanage;
	CTextUnit	m_combmatFacUT;
	/*CSpinButtonCtrl	m_pcacreepstrnSP;*/
	CFormulaEditSpin	m_pcacrpstrn;
	/*CSpinButtonCtrl	m_pcasrkgstrnSP;*/
	CFormulaEditSpin	m_pcasrkgstrn;
	CTextUnit	m_combVSUT;
	CTextUnit	m_combmodulUT;
	CTextUnit	m_combfc28UT;
	CTextUnit	m_pcavsratioUT;
	CTextUnit	m_pcamodulUT;
	CTextUnit	m_pcafc28UT;
	CEditUnit	m_combVS;
	CEditUnit	m_combsmatFac;
	CEditUnit	m_combmodul;
	CEditUnit	m_combfc28;
	CEditUnit	m_combmatFac;
	CEditUnit	m_pcavsratio;
	CEditUnit	m_pcamodul;
	CEditUnit	m_pcafc28;
	CEditUnit	m_acisrkgstrn;
	CEditUnit	m_acicrpcoef;
	/*CSpinButtonCtrl	m_combratioSP;*/
	CFormulaEditSpin	m_combratio;
	/*CSpinButtonCtrl	m_combhumidSP;*/
	CFormulaEditSpin	m_combhumid;
	/*CSpinButtonCtrl	m_pcareinratioSP;*/
	CFormulaEditSpin	m_pcareinratio;
	/*CSpinButtonCtrl	m_pcahumidSP;*/
	CFormulaEditSpin	m_pcahumid;
	CButton	m_frmCEB;
	mit::frx::MEdit	m_wndLoadAge;
	CCobxTdmf	m_CobxCreep;
	CCobxTdmf	m_CobxSstr;
	CListCtrl	m_wndUdefFList;
	CEditUnit	m_memPhi2;
	CEditUnit	m_memPhi1;
	CTextUnit	m_notaunit;
	CEditUnit	m_nota;
	CTextUnit	m_cebcompunit;
	CEditUnit	m_cebcomp;
	CTextUnit	m_cebageunit;
	CEditUnit m_cebrela2;

	// MNET:2406-JHLEE-20070822
	mit::frx::MComboBox	m_cmbCebRela;
	CTextUnit m_stcRHEnv;

	mit::frx::MComboBox	m_cmbAggre;
	/////////////////////////////

	CTextUnit	m_voluunit;
	CEditUnit	m_volu;
	CTextUnit	m_slumunit;
	CEditUnit	m_slum;
	CTextUnit	m_acicompunit;
	CEditUnit	m_acicomp;
	CTextUnit	m_cemeunit;
	CEditUnit	m_ceme;
	CTextUnit	m_aciageunit;
	/*CSpinButtonCtrl	m_aciagespin;*/
	CFormulaEditSpin m_aciage;
	/*CSpinButtonCtrl	m_cebrelaspin;*/
	CFormulaEditSpin	m_cebrela;
	/*CSpinButtonCtrl	m_cebagespin;*/
	CFormulaEditSpin m_cebage;
	/*CSpinButtonCtrl	m_acirelaspin;*/
	CFormulaEditSpin	m_acirela;
	/*CSpinButtonCtrl	m_airspin;*/
	CFormulaEditSpin	m_air;
	/*CSpinButtonCtrl	m_finespin;*/
	CFormulaEditSpin	m_fine;
	/*CSpinButtonCtrl	m_memN1spin;*/
	CFormulaEditSpin m_memN1;
	/*CSpinButtonCtrl	m_memN2spin;*/
	CFormulaEditSpin m_memN2;
	mit::frx::MComboBox	m_CobxCode;
	int		m_type;
	
	int		m_curemethod;
	int		m_cementType;
	
	BOOL	m_bSSTR;
	BOOL	m_bTSTR;
	CString	m_strTDefMatName;
	int		m_combLFac;
	int		m_combvsratio;
	int		m_combprog;
	int		m_combsvsratio;
	int		m_ACIorUser;
	BOOL	m_bPCAEffect;
	int		m_jpancemen;
	int		m_jpanhumid;
	int		m_chinaRelradio;
	CEditUnit m_concA;
	CEditUnit m_concB;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMHcrpDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

	// Implementation
protected:
	CDBDoc* m_pDoc;

	// MNET:2406-JHLEE-20070822
	CArray<UINT, UINT> m_aCebgroup;
	CArray<UINT, UINT> m_aCebshrinkgroup;

	CArray<UINT, UINT> m_acigroup;
	CArray<UINT, UINT> m_acishrinkgroup;
	CArray<UINT, UINT> m_cebgroup;
	CArray<UINT, UINT> m_cebshrinkgroup;
	CArray<UINT, UINT> m_memgroup;
	CArray<UINT, UINT> m_udefgroup;
	CArray<UINT, UINT> m_udefcrp;
	CArray<UINT, UINT> m_udefshr;
	CArray<UINT, UINT> m_combroup;
	CArray<UINT, UINT> m_pcagroup;
	CArray<UINT, UINT> m_aciacigroup;
	CArray<UINT, UINT> m_aciUsergroup;
	CArray<UINT, UINT> m_buttongroup;
	CArray<UINT, UINT> m_japangroup;
	CArray<UINT, UINT> m_chinagroup;
	CArray<UINT, UINT> m_jscegroup;  //SKN
	CArray<UINT, UINT> m_ChJTG04Group;
	CCMTdmtJapan* m_pJapanDlg;
	CCMTdmtChJTG04* m_pChJTG04Dlg;
	CCMTdmtAASHTO* m_pAASHTODlg;
	CCMTdmtEuropean* m_pEuropeanDlg;
	CCMTdmtIndia* m_pIndiaDlg;
	CCMTdmtAUSTRALIA* m_pASDlg;
	CCMTdmtIndia2011* m_pIndia2011Dlg;
	CCMTdmtIndia2011_2020* m_pIndia2011_2020Dlg;
	CCMTdmtRUS* m_pRusDlg;
	CCMTdmtChJTG16* m_pChJTG16Dlg;
	CCMTdmtNEWZEALAND* m_pNZDlg;
	CCMTdmtKDS2016* m_pKDS2016Dlg;
	CCMTdmtJSCE07* m_pJSCE07Dlg;
	CCMTdmtJSCE12* m_pJSCE12Dlg;
	CCMTdmt_ABNT_NBR_6118* m_pABNT_NBR_6118Dlg;

	void SetComboWidth(CComboBox& ComboBox);
	void AlignControls();
	void ModeCtrlMan();
	void ManUDefCtrls();
	void SetUnitType();
	void SetData2Dlg();
	void SetDlg2Data();
	void EnableJapanRelaCtrl(BOOL bEnable);
	void EnableChinaRelaCtrl(BOOL bEnable);

	// Generated message map functions
	//{{AFX_MSG(CCMHcrpDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeHcrpCodeCombo();
	afx_msg void OnHcrpSelChangeType();
	afx_msg void OnCmdHcrpRemoveBtn();
	afx_msg void OnSSTR();
	afx_msg void OnCreepFunctionButton();
	afx_msg void OnSSTRFuncButton();
	afx_msg void OnCmdUdefAddCfuncButton();
	afx_msg void OnCmdUdefDelCfuncButton();
	afx_msg void OnCmdUdefModifyCfuncButton();
	afx_msg void OnDeltaposACI_RELA(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposACI_FINE(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposACI_AIR(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposACI_AGE(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposCEB_AGE(NMHDR* pNMHDR, LRESULT* pResult);

	// MNET:2406-JHLEE-20070823
	afx_msg void OnDeltaposCEB1978_AGE(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnDeltaposCEB_RELA(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposEMOD_N1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposEMOD_N2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdEtcHcrpShow();
	afx_msg void OnCmdAciAciRd();
	afx_msg void OnCmdAciUserRd();
	afx_msg void OnDeltaposCmdCombEffctRatioSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposCmdCombReltHumidSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposCmdPcaRltvHumidSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposCmdPcaRnfcRatioSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposCmdPcaSrkgStrnSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposCmdPcaCreepStrnSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdCombPcaEffectChk();
	afx_msg void OnDeltaposCmdHcrpJapanRelaSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposCmdHcrpJapanAgeSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdHcrpJapanRelRd1();
	afx_msg void OnCmdHcrpJapanRelRd2();
	afx_msg void OnDeltaposCmdHcrpChinaRelaSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposCmdHcrpChinaAgeSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdHcrpChinaRelRd();
	afx_msg void OnCmdApply();
	afx_msg void OnDeltaposCmdJsceRhSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposCmdJsceAcSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
  //{{AFX_INSERT_LOCATION}}
  // Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMHCRPDLG_H__2B112166_2744_11D5_880F_00010263A1F4__INCLUDED_)





