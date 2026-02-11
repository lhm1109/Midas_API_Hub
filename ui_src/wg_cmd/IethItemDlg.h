#if !defined(AFX_IETHITEMDLG_H__0FC7F0F3_1669_41A6_88D0_0205821CA0C6__INCLUDED_)
#define AFX_IETHITEMDLG_H__0FC7F0F3_1669_41A6_88D0_0205821CA0C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IethItemDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CIethItemDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CIethItemDlg : public CDialogMove
{
// Construction
public:
	CIethItemDlg(CWnd* pParent = NULL);   // standard constructor

//-----------------------------------------------------------------------
// Interfaces
//-----------------------------------------------------------------------
public:
	void SetParamData(T_IETH_D &data);

//-----------------------------------------------------------------------
// Implementations
//-----------------------------------------------------------------------
protected:
	BOOL ValidData(T_IETH_D &data);
	BOOL Dlg2Data(T_IETH_D &data);
	BOOL Data2Dlg();  

	void SetListCtrlHeaderDamping();
	BOOL GetSelectedDataDamping(int &nIndex);
	void MakeItemExDamping();
	CString DataToStrDamping(int i, T_IETH_BASE &data);
	void SetItemDamping(int nIndex, T_IETH_BASE &data);
	BOOL Dlg2IethBase(T_IETH_BASE &data);
	BOOL IethBase2Dlg(T_IETH_BASE &data);
	void AlignControl();
	void CtrlManager();
	int  GetFinalStateIndex(T_IETH_K KeyIeth);
	BOOL ApplyOrOK();
	void SetFinalStateCmb();
	void AutoCalcCoef();

protected:
	CDBDoc*   m_pDoc;
	T_IETH_D  m_Data;
	CString   m_csOldName;
	UINT      m_nOldID;
	BOOL      m_bModify;
	BOOL      m_bInitCmb;
	CArray<T_IETH_BASE, T_IETH_BASE&> m_aDampData;
	CArray<UINT, UINT> m_aCtrlDirect, m_aCtrlMass, m_aCtrlAttribute;  // AlignControl()로 같은 자리에 있기 때문에 CtrlShowHideByRect를 쓰지 못함
	CArray<UINT, UINT> m_aCtrlMassStiff;

	// CalcDampingDlg의 변수를   
	int    m_nFreqPeriodRdoDlg;
	double m_dFrequencyDlg;
	double m_dPeriodDlg;  

// Dialog Data
	//{{AFX_DATA(CIethItemDlg)
	enum { IDD = IDD_ETC_IETH_ITEM };
	CEdit	    m_wndMaxIterNum;	
	CEdit	    m_wndLoadcaseName;
	CEdit   	m_wndDesc;
	CListCtrl	m_DampingList;
	CButton   m_wndInitCondition;
	CButton   m_wndFinalStepLoad;
	CButton   m_wndShowBtn;
	CButton   m_wndCalcBtn;
	CButton	  m_wndMassProp;
	CButton	  m_wndStiffProp;
	CButton   m_wndBtnApply;
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
	CEditUnit	m_wndMinSubStepSize;
	CTextUnit m_wndMinSubStepSizeUnit;
	CEditUnit	m_wndDamping;
	CEditUnit	m_wndMode;
	CEditUnit	m_wndDampAllMode;
	CEditUnit	m_wndOutputStep;
	CTextUnit	m_wndTimeIncUnit;
	CEditUnit	m_wndTimeInc;
	CTextUnit	m_wndEndTimeUnit;
	CEditUnit	m_wndEndTime;
	CEditUnit m_wndConvTol;
	CComboBox m_wndFinalStateCmb;	
	int       m_nAnalTypeRdo;	
	int       m_nAnalMethodRdo;
	int		    m_nTimeHistoryTypeRdo;
	int       m_nDampingTypeRdo;
	int       m_nCoefDefineRdo;
	int		    m_nFreqPeriodRdo;
	int       m_nIntegrationParaRdo;	
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIethItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIethItemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposDouble(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposInteger(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDampingAdd();
	afx_msg void OnDampingModify();
	afx_msg void OnDampingDelete();
	afx_msg void OnChangedCurIethBase(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnCmdApply();
	afx_msg void OnDeltaposCmdMaxIterNumSpin(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnInitConditionChk();
	afx_msg void OnChangeAnalType();
	afx_msg void OnChangeAnalMethod();
	afx_msg void OnChangeTimeHistoryType();
	afx_msg void OnChangeDampingType();
	afx_msg void OnChangeCoefDefine();
	afx_msg void OnChangeFreqPeriod();
	afx_msg void OnChangeIntegrationPara();  
	afx_msg void OnShowDampingBtn();
	afx_msg void OnMassStiffChk();
	afx_msg void OnCallAutoCalcCoef();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IETHITEMDLG_H__0FC7F0F3_1669_41A6_88D0_0205821CA0C6__INCLUDED_)
