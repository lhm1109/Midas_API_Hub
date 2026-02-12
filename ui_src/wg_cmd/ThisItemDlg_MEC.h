#if !defined(AFX_THISITEMDLG_H_MEC__66CFF362_FF05_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_THISITEMDLG_H_MEC__66CFF362_FF05_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThisItemDlg_MEC.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
//#include "..\wg_db\wg_db_SelectLC.h"

#include "CMDlgBase.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"
#include "..\wg_base\SpinBtnExCtrl.h"

#include "..\wg_base\MouseEdit.h"
#include "..\wg_base\FoldGroupBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CThisItemDlg_MEC dialog
#include "HeaderPre.h"

class CDampGrid;
class __MY_EXT_CLASS__ CThisItemDlg_MEC : public CCMDlgBase
{
// Construction
public:
	CThisItemDlg_MEC(CWnd* pParent = nullptr);   // standard constructor
	~CThisItemDlg_MEC();

	enum { IDD = IDD_ETC_THIS_ITEM_MEC };

//-----------------------------------------------------------------------
// Interfaces
//-----------------------------------------------------------------------
public:
	BOOL ExternalSetting(const T_THIS_K key);
	void SetParamData(T_THIS_D &data);

//-----------------------------------------------------------------------
// Implementations
//-----------------------------------------------------------------------
protected:
	BOOL ValidData();
	BOOL Dlg2Data();
	BOOL Data2Dlg();  

	void SetGridListCtrlHeader();
	void MakeGridItemEx();
	void SetGridItem(int nIndex, T_GILC_K data);

	void AlignControl();
	void CtrlManager();
	void SetInitLoadFoldControl(const bool isShowJP, const int initMethod);
	int  GetFinalStateIndex(int nSubLoadCaseType, UINT KeyFinalState);
	BOOL ApplyOrOK();
	void SetFinalStateCmb();
	void AutoCalcCoef();
	BOOL CheckValidNllp();
	void SetInitialLoadText();
	void MakeItemInitialLoadCmb(int nAnalType, int nAnalMethod);
	void SetGridDeadLoadCmb();

	void SetCheckGeomNonlinearType(int nGeomNonlinearType);
	void GetCheckGeomNonlinearType(int& nGeomNonlinearType);

protected:
	CDBDoc*   m_pDoc;
	T_THIS_D  m_Data;
	CString   m_csOldName;
	UINT      m_nOldID;
	BOOL      m_bModify;
	BOOL      m_bInitCmb;
	BOOL      m_bUseValidNllp; // 이 모델에서 (Force Type or Seismic Control Device) 가 설정된 General Link를 사용중인 경우 TRUE
	CDampGrid* m_pGrid;
	CArray<T_THIS_BASE, T_THIS_BASE&> m_aDampData;
	CArray<UINT, UINT> m_aCtrlDirect, m_aCtrlMass, m_aCtrlAttribute;  // AlignControl()로 같은 자리에 있기 때문에 CtrlShowHideByRect를 쓰지 못함
	CArray<UINT, UINT> m_aCtrlMassStiff;
	CArray<UINT, UINT> m_aCtrlUpdate;
	CArray<UINT, UINT> m_aCtrlUpdateRadio;
	CArray<UINT, UINT> m_aCtrlDamping, m_aCtrlStatic;
	CArray<UINT, UINT> m_aCtrlOption, m_aCtrlOption2;
	CArray<UINT, UINT> m_aCtrlIteration;
	CArray<UINT, UINT> m_aCtrlInitLoad_Initial,m_aCtrlInitLoad_Sequence;
	CArray<UINT, UINT> m_aCtrlNEWMARK;
	CArray<UINT, UINT> m_aCtrl_GeomNonlinearType;
	CArray<UINT, UINT> m_aGridModelList, m_aCtrlGeoNonlinear, m_aCtrlTimeParam, m_aCtrlNonAnalParam;
	BOOL m_bJapanMode;

	// CalcDampingDlg의 변수를   
	int    m_nFreqPeriodRdoDlg;
	double m_dFrequencyDlg;
	double m_dPeriodDlg;  
	int    m_nDampingMethod;
// Dialog Data
	//{{AFX_DATA(CThisItemDlg_MEC)
	MButton	  m_wndSubsequentChk;
	MEdit	    m_wndLoadcaseName;
	MEdit   	m_wndDesc;
	MButton   m_wndNonlinearLink;
	MButton   m_wndInelasticHinge;
	MButton   m_wndFinalStepLoad;
	MButton   m_wndFinalStepAcc;
	MButton   m_wndShowBtn;
	MButton   m_wndCalcBtn;
	MButton	  m_wndMassProp;
	MButton	  m_wndStiffProp;
	MButton   m_wndBtnApply;
	CEditUnit	m_wndDirectRm;
	CEditUnit	m_wndDirectRk;
	CEditUnit	m_wndCalcRm;
	CEditUnit	m_wndCalcRk;
	CEditUnit	m_wndF1;
	CEditUnit	m_wndF2;
	CEditUnit	m_wndT1;
	CEditUnit	m_wndT2;
	CEditUnit	m_wndX1;
	CEditUnit	m_wndX2;
	CEditUnit	m_wndGamma;
	CEditUnit	m_wndBeta;
	CEditUnit	m_wndDampAllMode;
	CFormulaEditSpin	m_wndOutputStep;
	CTextUnit	m_wndTimeIncUnit;
	CFormulaEditSpin	m_wndTimeInc;
	CTextUnit	m_wndEndTimeUnit;
	CFormulaEditSpin	m_wndEndTime;
	CEditUnit m_wndOptEdit1;
	CMouseEdit m_wndOptEdit2;
	CEditUnit m_wndOptEdit3;
//  CSelectLC m_wndFinalStateCmb;	
	MComboBox m_wndFinalStateCmb;	
	MComboBox m_wndOptDirCmb;
	MComboBox m_wndDampingMethodCmb;
	int       m_nAnalTypeRdo;	
	int       m_nAnalMethodRdo;
	int		    m_nTimeHistoryTypeRdo;  
	int		    m_nGeomNonlinearTypeRdo;
	int       m_nDampingTypeRdo;
	int       m_nCoefDefineRdo;
	int		    m_nFreqPeriodRdo;
  //int       m_nIntegrationParaRdo;
	int       m_nIntegrationParam;
	int       m_nStaticIncrementRdo;
	int       m_nStaticCtrlOptRdo;
	CTextUnit	m_wndTransUnit;
	CTextUnit	m_wndIncreDispUnit;
	MButton   m_wndOptputOpt;
	MButton   m_wndCumulateDVACheck;
	CEditUnit	m_wndIncreStep;
	CEditUnit m_wndScaleFactor;
	MButton   m_wndIteration;
	int		    m_nSubsequentType;
	MButton   m_chkUseInitial;
	MComboBox m_cmbInitLoadMethod;
	int		  m_nTimeIntegrationParam;
	MComboBox m_cmbDampUpdate;
	MComboBox m_cmbIntegrationParam;
	MComboBox m_cmbGridLoadCase;
	CListCtrl	m_GridList;
	CFoldGroupBox_Cross	m_wndInitLoadGroupBox;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThisItemDlg_MEC)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CThisItemDlg_MEC)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeIteration();
	afx_msg void OnDeltaposDouble(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposInteger(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangedCurThisBase(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCmdApply();
	afx_msg void OnChangeAnalType();
	afx_msg void OnChangeAnalMethod();
	afx_msg void OnChangeTimeHistoryType();
	afx_msg void OnChangeGeomNonlinearType();
	afx_msg void OnChangeSequenceLoad();  
	afx_msg void OnChangeCoefDefine();
	afx_msg void OnChangeFreqPeriod();
	afx_msg void OnChangeIntegrationPara();  
	afx_msg void OnShowDampingBtn();
	afx_msg void OnMassStiffChk();
	afx_msg void OnCallAutoCalcCoef();
	afx_msg void OnStaticIncMethod();
	afx_msg void OnStaticCtrlOpt();
	afx_msg void OnSelchangeCmdThisDampMethodCmb();
	afx_msg void OnSelchangeCmdFinalStateCmb();
	afx_msg void OnCmdSubsequentTypeRdo();
	afx_msg void OnCmdSubsequentChk();
	afx_msg void OnCmdIterationCtrlBtn();
	afx_msg void OnCmdDescBtn();
	afx_msg void OnChkUseInitial();
	afx_msg void OnCmdInitLoadMethod();
	afx_msg void OnChkKeepFinalStep();
	afx_msg void OnNewmarkOpt();
	afx_msg void OnSelchangeCmdNemark();
	afx_msg void OnCmdBtnLdGridAdd();
	afx_msg void OnCmdBtnLdGridDel();
	afx_msg LRESULT OnBnClickedInitLoadFoldButton(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THISITEMDLG_H__66CFF362_FF05_11D3_92DE_0000C0B0E6B3__INCLUDED_)
