#if !defined(AFX_TDMEITEMDLG_H__771F3F42_228C_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_TDMEITEMDLG_H__771F3F42_228C_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TdmeItemDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\wg_base_MySRGraph.h"
#include "TdmeGrid.h"

#include "..\MIT_frx\McomboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CTdmeItemDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CTdmeItemDlg : public CDialogMove, public CDBUpdateConnector
{
	// Construction
public:
	CTdmeItemDlg(CWnd* pParent = NULL);   // standard constructor
	void	AlignControls();
	void	AlignControlsDensity(int nCode);

public:
	CTdmeGrid		m_wndGrid;
	BOOL				m_bGridInit;
	CMySRGraph			m_Graph;
	SREGraphView m_GraphView;
public:
	T_TDME_D&	GetParamData();
	void	SetParamData(T_TDME_D& data);
	int		SelChangeCodeCobx(CString str);
	void	InitGrid();
	void	InitChart();
	//void    InitMatlCombo();
	void	OnDataChanged();
	void	SetValue(int nIndex, int nGroup, double x, double y);
	void	ShowHideCode(BOOL flag);
	void	Data2Dlg();
	void	Dlg2Data();

	void	DrawChart();
	void	GraphConstant();
	void	GraphCode();
	void	GraphUser();

	void	SetAxisLabelText(int nComponent, LPCTSTR lbl);

	double OhzagiY(int nCement, double m);
	double GetFactorForCode(int iCurrUnitFor, int iCurrUnitLen);

	void  SetComboCode();
	void  SetCementTypeSCmb();
	// Dialog Data
	//{{AFX_DATA(CTdmeItemDlg)
	enum { IDD = IDD_ETC_TDME_ITEM };
	CTextUnit	m_dCHCompStrengthUnit;
	CEditUnit	m_dCHCompStrength;
	mit::frx::MButton 	m_btnYLog;
	mit::frx::MButton 	m_btnXLog;
	CTextUnit	m_dOHZCompStrengthUnit;
	CEditUnit	m_dOHZCompStrength;
	mit::frx::MComboBox 	m_OHZCementType;
	CTextUnit	m_dKSDCompStrengthUnit;
	CEditUnit	m_dKSDCompStrength;
	CEditUnit	m_dKSDFactorB;
	CEditUnit	m_dKSDFactorA;
	CTextUnit	m_dCEBCompStrengthUnit;
	CEditUnit	m_dCEBCompStrength;
	mit::frx::MComboBox 	m_CEBCementType;
	mit::frx::MComboBox 	m_CEBAffreType;
	CTextUnit	m_dACICompStrengthUnit;
	CEditUnit	m_dACICompStrength;
	CEditUnit	m_dACIFactorB;
	CEditUnit	m_dACIFactorA;
	CTextUnit	m_SpecHeatUnit;
	CEditUnit	m_SpecHeatEdit;
	CTextUnit	m_HeatCondUnit;
	CEditUnit	m_HeatCondEdit;
	CFormulaEdit	m_wndScaleFactor;
	mit::frx::MComboBox 	m_CobxCode;
	int		m_type;
	CString	m_csTypeName;
	CEditUnit	m_dJAPANCompStrength;
	CEditUnit	m_dJAPANFactorA;
	CEditUnit	m_dJAPANFactorB;
	CEditUnit	m_dJAPANFactorD;
	CTextUnit	m_dJAPANCompStrengthUnit;
	mit::frx::MButton 	m_btnConcreteData;
	mit::frx::MComboBox 	m_JAPANCementType;
	int m_nJpnCementType;
	CEditUnit	m_dJPN2CompStrength;
	CTextUnit	m_dJPN2CompStrengthUnit;
	CEditUnit	m_dJpnTensileStrength;
	int m_nJpnGraphType;
	CEditUnit m_dINDIACompStrength;
	CTextUnit m_dINDIACompStrengthUnit;
	CTextUnit	m_dEUROPEANCompStrengthUnit;
	CEditUnit	m_dEUROPEANCompStrength;
	mit::frx::MComboBox 	m_EUROPEANCementType;
	CTextUnit m_dCEBFIP1978CompStrengthUnit;
	CEditUnit m_dCEBFIP1978CompStrength;
	CTextUnit	m_dIND2011CompStrengthUnit; // MNET:XXXX-JWKWON-20130909 : 인도 시간의존 재질 추가..
	CEditUnit	m_dIND2011CompStrength;
	mit::frx::MComboBox 	m_dIND2011AffreType;
	mit::frx::MComboBox 	m_IND2011CementType;
	CEditUnit m_edtRusConcB;
	CTextUnit m_untRusConcB;
	mit::frx::MComboBox  m_cmbRusCementType;
	int       m_nRusCuringMeth;
	int       m_nRusConcType;
	CEditUnit m_edtRusMaxSize;
	CTextUnit m_untRusMaxSize;
	CEditUnit m_edtRusPz;
	MButton	m_btnDyGen;
	//mit::frx::MComboBox  m_cmbMatl;
	CEditUnit m_edtDensity; // 단위계 고정

	CEditUnit m_dGilbetCompStrength;
	CTextUnit m_dGilbetCompStrengthUnit;
	mit::frx::MComboBox  m_GilbertCementType;

	mit::frx::MillustViewer m_wndPicture;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTdmeItemDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	//}}AFX_VIRTUAL

	// Implementation
protected:
	T_TDME_D	m_ParamData;
	BOOL			m_bModify;
	CString		m_csOldName;
	T_UNIT_INDEX m_CurrUnit;
	CArray<UINT, UINT> m_aButtonCtrl;
	CArray<UINT, UINT> m_aUserCtrl;
	CArray<UINT, UINT> m_aCodeCtrl;
	CArray<UINT, UINT> m_aKSDCodeCtrl;
	CArray<UINT, UINT> m_aACICodeCtrl;
	CArray<UINT, UINT> m_aCEBCodeCtrl;
	CArray<UINT, UINT> m_aOHZCodeCtrl;
	CArray<UINT, UINT> m_aCHCodeCtrl;
	CArray<UINT, UINT> m_aJAPANCodeCtrl, m_arGraphtype;
	CArray<UINT, UINT> m_aJPN2CodeCtrl;
	CArray<UINT, UINT> m_aJPN2CodeRadioCtrl;
	CArray<UINT, UINT> m_aINDIACodeCtrl;
	CArray<UINT, UINT> m_aEUROPEANCodeCtrl;
	CArray<UINT, UINT> m_aCEBFIP1978CodeCtrl;
	CArray<UINT, UINT> m_aIND2011CodeCtrl;
	CArray<UINT, UINT> m_aRussiaCodeCtrl;
	CArray<UINT, UINT> m_aKDS2016Ctrl;
	CArray<UINT, UINT> m_aGilbertCtrl;

	// Generated message map functions
	//{{AFX_MSG(CTdmeItemDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnTdmeSelChangeType();
	afx_msg void OnSelchangeTdmeCodeCombo();
	afx_msg void OnTdmeSinRedrawGraph();
	afx_msg void OnETCTDMECHARTXLog();
	afx_msg void OnETCTDMECHARTYLog();
	afx_msg void OnPaint();
	afx_msg void OnTdmeJPNConcreteChk();
	afx_msg void OnSelchangeJapanCementCombo();
	afx_msg void OnBtnDyGen();
	//afx_msg void OnTdmeMatlCmb();
	afx_msg void OnTdmeMatlBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDMEITEMDLG_H__771F3F42_228C_11D5_880F_00010263A1F4__INCLUDED_)

