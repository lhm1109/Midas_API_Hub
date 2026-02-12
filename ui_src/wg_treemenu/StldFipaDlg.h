#pragma once

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditTemp.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CStldFipaDlg dialog

class CStldFipaDlg : public CMenuBarChildDlg
{
// Construction
public:
	CStldFipaDlg(CWnd* pParent = NULL);   // standard constructor

	virtual void Execute() { OnTmExecute(); }
// Dialog Data
	//{{AFX_DATA(CStldFipaDlg)
	enum { IDD = IDD_TM_STLD_FIRE_FIPA };

	double	m_wndThicknessEdit;
	double	m_wndResisEdit;
	double	m_wndConDucEdit;
	double	m_wndDensityEdit;

	CEditTemp	m_wndT0Edit;
	CSelectLC	m_LoadCase;
	int		m_nOption;
	MComboBox m_wndTypeCobx;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldFipaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	CArray<UINT, UINT> m_aCtrlEdit[4], m_aCtrlText[4],m_aCtrlAll;
// Implementation
protected:
	CDBDoc* m_pDoc;
	T_FIPA_D m_data;
	double		m_dfireResTime;//fire resistance time
	//double		m_dThickness;
	double		m_dRadiance;
	//double		m_dResistance;//The equivalent thermal resistance
	//double		m_dConductivity;//Equivalent heat conductivity
	//double		m_dDensity;
	double		m_dSHeat;//specific heat
	// Generated message map functions
	//{{AFX_MSG(CStldFipaDlg)
	virtual BOOL OnInitDialog();
	BOOL Dlg2Data();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	//afx_msg void OnClickLoadCaseBtn();
	//afx_msg void OnChangeOption();
	//afx_msg void OnTmProjConfBtn();
	//afx_msg void OnTmDefineLdgrBtn();
	afx_msg void OnSelFipaType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};


