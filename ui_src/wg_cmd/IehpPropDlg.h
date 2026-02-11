#if !defined(AFX_IEHPPROPDLG_H__03FB54E0_BE93_48BC_B797_6242ED2A829F__INCLUDED_)
#define AFX_IEHPPROPDLG_H__03FB54E0_BE93_48BC_B797_6242ED2A829F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IehpPropDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "../MIT_frx/MTabCtrl.h"
//#include "..\wg_base\wg_base_MySRGraph.h"
#include "IehpYieldPropGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CIehpPropDlg dialog

class CIehpPropDlg : public CDialogMove
{
// Construction
public:
	CIehpPropDlg(int nTabStyle, CWnd* pParent = NULL);   // standard constructor
		
// Dialog Data
	//{{AFX_DATA(CIehpPropDlg)
	enum { IDD = IDD_ETC_IEHP_PROP };
	enum TABSTYLE  {LOCATION_NONE = 0,I_LOCATION=1,J_LOCATION=2, I_J_LOCATION=3, CENTER_LOCATION=4};
	enum MyEnum{D_TAB_DEFALUT = 0, D_TAB_SUB = 1};


	mit::frx::MTabCtrl	m_Tab;
	int m_nTabStyle;

	BOOL InitTabCtrl();
	void ChangeTabCtrl(int m_nTabStyle);

	CEditUnit m_edtDisp1Plus;
	CEditUnit m_edtDisp1Mnus;
	CEditUnit m_edtDisp2Plus;
	CEditUnit m_edtDisp2Mnus;
	CEditUnit m_edtDisp3Plus;
	CEditUnit m_edtDisp3Mnus;
	CEditUnit m_edtDisp4Plus;
	CEditUnit m_edtDisp4Mnus;
	CEditUnit m_edtFraPlus;
	CEditUnit m_edtFraMnus;
	CEditUnit m_edtUltPlus;
	CEditUnit m_edtUltMnus;
	CEditUnit m_edt3Plus;
	CEditUnit m_edt3Mnus;
	CTextUnit m_untUltmt;
	CEditUnit	m_edtCrkPlus;
	CEditUnit	m_edtCrkMnus;
	CTextUnit	m_untStiffUser;
	CEditUnit	m_edtStiffUser;
	CTextUnit	m_untCrack;
	CTextUnit	m_untYield;
	CTextUnit	m_untFract;
	CTextUnit	m_untDisp1;
	CTextUnit	m_untDisp2;
	CTextUnit	m_untDisp3;
	CTextUnit	m_untDisp4;
	CEditUnit	m_edtYldPlus;
	CEditUnit	m_edtYldMnus;
	CString	m_str1Plus;
	CString	m_str1Mnus;
	CString	m_str2Mnus;
	CString	m_str2Plus;
	CString m_str3Plus;
	CString m_str3Mnus;
	CEditUnit	m_edtExponent;
	CEditUnit	m_edtLoop;
	CEditUnit	m_edtPinch;
	CTextUnit	m_untExponent;
	CTextUnit	m_untLoop;
	int		m_nValueTypeSymmetry;
	int		m_nUserAuto;
	int		m_nSymmetry;
	int		m_nStiffType;
	int		m_nDeformDefineType;
	int		m_nAlphaDelta;
	CEditUnit	m_edtSlipT;
	CEditUnit	m_edtSlipC;
	CTextUnit	m_untSlip;
	//}}AFX_DATA
	double m_dDeformCapacity[2][5];
public:
	T_IEHP_D m_Data;
	T_ELEM_K m_ElemK;
	//void SetIehpPropData(T_IEHP_D& data, int nFg1, int nFg2);
	void SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K  IehgKey, T_IEHP_KINEMA data,T_IEHP_KINEMA subdata, int nFg1, int nFg2, int nSectType=0, BOOL bAssignProperties = FALSE);
	void SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K  IehgKey, T_IEHP_ORIGIN data,T_IEHP_ORIGIN subdata, int nFg1, int nFg2, int nSectType=0, BOOL bAssignProperties = FALSE);
	void SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K  IehgKey, T_IEHP_PEAK   data,T_IEHP_PEAK		subdata, int nFg1, int nFg2, int nSectType=0, BOOL bAssignProperties = FALSE);
	void SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K  IehgKey, T_IEHP_CLOUGH data,T_IEHP_CLOUGH subdata, int nFg1, int nFg2, int nSectType=0, BOOL bAssignProperties = FALSE);
	void SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K  IehgKey, T_IEHP_DEGRAD data,T_IEHP_DEGRAD subdata, int nFg1, int nFg2, int nSectType=0, BOOL bAssignProperties = FALSE);
	void SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K  IehgKey, T_IEHP_TAKEDA data,T_IEHP_TAKEDA subdata, int nFg1, int nFg2, int nSectType=0, BOOL bAssignProperties = FALSE);
	void SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K  IehgKey, T_IEHP_NORBIL data,T_IEHP_NORBIL subdata, int nFg1, int nFg2, int nSectType=0, BOOL bAssignProperties = FALSE);
	void SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K  IehgKey, T_IEHP_ELABIL data,T_IEHP_ELABIL subdata, int nFg1, int nFg2, int nSectType=0, BOOL bAssignProperties = FALSE);
	void SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K  IehgKey, T_IEHP_ELATRI data,T_IEHP_ELATRI subdata, int nFg1, int nFg2, int nSectType=0, BOOL bAssignProperties = FALSE);  
	void SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K  IehgKey, T_IEHP_ELATET data,T_IEHP_ELATET subdata, int nFg1, int nFg2, int nSectType=0, BOOL bAssignProperties = FALSE);
	void SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K  IehgKey, T_IEHP_TAKTET data,T_IEHP_TAKTET subdata, int nFg1, int nFg2, int nSectType=0, BOOL bAssignProperties = FALSE);
	void SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K  IehgKey, T_IEHP_SLIP   data,T_IEHP_SLIP		subdata, int nFg1, int nFg2, int nSectType=0, BOOL bAssignProperties = FALSE);
	void SetIehpPropData(T_IEHP_D iehpD, T_IEHG_K  IehgKey, T_IEHP_SRCTET data,T_IEHP_SRCTET subdata, int nFg1, int nFg2, int nSectType=0, BOOL bAssignProperties = FALSE);

	void SetControlStateData(BOOL bExistIJData, int nOptType);
	BOOL CompareProp(T_IEHP_PROP MainProp, T_IEHP_PROP SubProp);
	BOOL m_bExistIJData;

	void BackupDisplayData();
	void LoadDisplayBackupData();
	CArray<double, double> m_aBackupArr;

	bool SyncDialogData();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIehpPropDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc*   m_pDoc;
	int m_nModelType;
	int m_nDof;
	int m_nIntrType;  // Interaction Type(IehpItemDlg)
	int m_nHingeType; // Hinge Type(IehpItemDlg)
	int m_nDefinition; 
	double m_dZero;
	int m_nSectType;  // 0: 적용할수 없는 단면, 1: 일반, 2: value type
	BOOL m_bAssignProperties;
	BOOL m_bDisableAutoCalc;    // Auto 계산을 할 수 없는 경우
	CArray<UINT, UINT> m_aIntractionNone;
	CArray<UINT, UINT> m_aInitStiff;
	CArray<UINT, UINT> m_aOK;
	CArray<UINT, UINT> m_aClough;
	CArray<UINT, UINT> m_aCtrlMinus;
	CArray<UINT, UINT> m_aAssignHinge;
	CArray<UINT, UINT> m_aCtrlYield;
	CArray<UINT, UINT> m_aCtrlUltmt;
	CArray<UINT, UINT> m_aCtrlFail;
	CArray<UINT, UINT> m_aCtrlAlpha;
	CArray<UINT, UINT> m_aCtrlDelta;
	CArray<UINT, UINT> m_aCtrlP1, m_aCtrlP2, m_aCtrlP3, m_aCtrlP4;
	CArray<UINT, UINT> m_aCtrlD1, m_aCtrlD2, m_aCtrlD3, m_aCtrlD4, m_aCtrlD0;
	CArray<UINT, UINT> m_aCtrlA1, m_aCtrlA2, m_aCtrlA3, m_aCtrlA0;

	void Data2Dlg();
	BOOL Dlg2Data();

	void Data2TabDlg(const int nTabID, T_IEHP_PROP* PROP);				
	BOOL TabDlg2Data(const int nTabID, T_IEHP_PROP* PROP);				

	BOOL CheckErrorNameDataVar(LPCTSTR lpszData, LPCTSTR lpszName, LPCTSTR lpszVar, BOOL bMsg);
	BOOL CheckData(BOOL bMsg = TRUE);
	BOOL CheckAllData(BOOL bMsg=TRUE);
	void SetTabData4CheckData(const int nTabID);

	void AlignCtrl();
	void SetUnitAndTitle();  
	void CtrlManager();

	// Generated message map functions
	//{{AFX_MSG(CIehpPropDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeUltiEdit();
	afx_msg void OnChangeYieldEdit();
	afx_msg void OnChangeCrackEdit();
	afx_msg void OnChangeStif1stEdit();
	afx_msg void OnChangeStif2ndEdit();
	afx_msg void OnChangeStif3rdEdit();
	afx_msg void OnChangeDisp1Edit();
	afx_msg void OnChangeDisp2Edit();
	afx_msg void OnChangeDisp3Edit();
	afx_msg void OnChangeDisp4Edit();
	afx_msg void OnChangeFailEdit();
	afx_msg void OnChangeDrawData();
	afx_msg void OnChangeSlipInitGap();
	
	afx_msg void OnSymmetryTypeRdo();
	afx_msg void OnUserAutoRdo();
	afx_msg void OnStiffTypeRdo();
	afx_msg void OnAlphaDeltaRdo();
	afx_msg void OnDeformationIndex();

	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnValueTypeSymmetry();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	/////////////////////////////////////////////////////
	// Chart

	CIehpYieldPropGraph m_Draw;
	void InitGraph();
	void UpdateGraph();
	void AdjustEndPoint(double& xa0, double& ya0, double xa1, double ya1, double xb0, double yb0, double& xb1, double& yb1); 

	void SaveDeformCapacity(int nD, T_IEHP_PROP* PROP);
	void SetInitialStiffness4HingeType(const int nD, T_IEHP_PROP* PROP);
	void GetInitialStiffness4HingeType(const int nD, T_IEHP_PROP* PROP);
	//SREGraphView m_GraphView;
	//CMySRGraph   m_Graph;
	//void InitChart();
	//void SetValue(int nIndex, int nGroup, double x,double y);
	//void UpdateChart();


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IEHPPROPDLG_H__03FB54E0_BE93_48BC_B797_6242ED2A829F__INCLUDED_)
