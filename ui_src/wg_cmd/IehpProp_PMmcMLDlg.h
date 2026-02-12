#if !defined(AFX_IehpProp_PMmcMLDlg_H__INCLUDED_)
#define AFX_IehpProp_PMmcMLDlg_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "IehpYieldPropGrid.h"
#include "IehpProp_MLinGrid.h"
#include "..\mit_frx\MComboBox.h"
using namespace mit::frx;

class CIehpProp_PMmcMLDlg : public CDialogMove
{
	// Construction
public:
	CIehpProp_PMmcMLDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIehpProp_PMmcMLDlg();   

	// Dialog Data
	//{{AFX_DATA(CIehpProp_PMmcMLDlg)
	enum { IDD = IDD_ETC_IEHP_PROP_PMMC_ML };

	//CButton m_chkMultiCurve;
	MComboBox m_cmbAxialForce;
	CTextUnit	m_untAxialForce;

	CEditUnit	m_edtBeta;
	CEditUnit	m_edtStiffPlus;
	CTextUnit	m_untStiffPlus;
	CEditUnit	m_edtStiffMnus;
	CTextUnit	m_untStiffMnus;
	CTextUnit	m_untAxial;

	//}}AFX_DATA
public:
	T_IEHP_D m_Data;
	T_IEHP_PMMULT m_PMMultiData;
	T_IEHP_MULT   m_MultiData;  // CIehpProp_MLinGrid를 활용하기 위한 변수

	void SetIehpPropData(T_IEHP_D iehpD, T_IEHP_PMMULT data, int nFg1, int nFg2);
	void OnDataChanged();
	bool SyncDialogData();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIehpProp_PMmcMLDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	int m_nDof;
	double m_dZero;

	CArray<UINT, UINT> m_aOK; 
	CArray<UINT, UINT> m_aTypeRdo;
	CArray<UINT, UINT> m_aAxialCtrl;
	CArray<UINT, UINT> m_aBetaCtrl;
	CArray<UINT, UINT> m_aHardeningRdo;

	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL CheckErrorNameDataVar(LPCTSTR lpszData, LPCTSTR lpszVar, BOOL bMsg);
	BOOL CheckData(BOOL bMsg=TRUE);

	//void AlignCtrl();
	void SetUnitAndTitle();  
	void CtrlEnableDisable();

	// Generated message map functions
	//{{AFX_MSG(CIehpProp_PMmcMLDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual BOOL DestroyWindow();
	afx_msg void OnSelectTypeRdo();
	afx_msg void OnSelectHardeningRdo();
	//afx_msg void OnMultiCurveChk();
	afx_msg void OnAxialForceBtn();
	afx_msg void OnDeformationIndex();  
	afx_msg void OnSelChangeAxialCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	int	m_nSymmetry; // 0=Sym, 1=Asym
	int m_nDeformDefineType;
	int m_nComboBoxIndex;
	void ResetAxialCombo();

	// Chart
	CIehpYieldPropGraph m_Draw;
	void InitGraph();
	void UpdateGraph();
	void UpdateInitStiff();

	// Grid
	//CIehpProp_PMMLinGrid m_wndGrid;
	CIehpProp_MLinGrid m_wndGrid;
	void InitGrid();
	void SetGridData();
	void GetGridData();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IehpProp_PMmcMLDlg_H__INCLUDED_)
