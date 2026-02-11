#if !defined(__BNDRINTEGRALPILE_H__)
#define __BNDRINTEGRALPILE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrIntegralPileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBndrIntegralPileDlg dialog

#include "..\wg_base\wg_base_InternationalDlg.h"
#include "..\wg_base\DlgChild.h"
#include "..\wg_base\MouseEdit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

struct SpringPile {
	int nSType;
	int nKType;
	double dGroundLevel;
	double dUnitSoilWeight;
	double dK0; 
	double dCU;
	double dInternalAngle; 
	double dPileDiameter; 
	double dTriAxial;
	double dK1;
	double dKH;
};

class CBndrIntegralPileDlg : public CDlgChild
{
// Construction
public:
	CBndrIntegralPileDlg(SpringPile* pData, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBndrIntegralPileDlg)
	enum { IDD = IDD_TM_BNDR_INTEBR_PILE };
	
	MComboBox	m_cmbType;
	MComboBox	m_cmbK1; 

	CMouseEdit m_edtGroundLevel;
	CEditUnit m_edtUnitSoilWeight;
	CEditUnit m_edtEarthCoeff;
	CEditUnit m_edtKH;
	CEditUnit m_edtCU;
	CEditUnit m_edtInternalAngle;
	CEditUnit m_edtPileDiameter;
	CEditUnit m_edtK1;
	CEditUnit m_edtTriAxial;

	CTextUnit m_untGroundLevel;
	CTextUnit m_untUnitSoilWeight;
	CTextUnit m_untKH;
	CTextUnit m_untCU;
	CTextUnit m_untInternalAngle;
	CTextUnit m_untPileDiameter;
	CTextUnit m_untK1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrIntegralPileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void InitSubCtrl();
	BOOL GetParameter();
	BOOL GetPosition(CString& strPos,double& Px ,double& Py ,double& Pz);
protected:
	SpringPile* m_pData;
	CArray<UINT, UINT> m_arSand;
	CArray<UINT, UINT> m_arClay;

	// Generated message map functions
	//{{AFX_MSG(CBndrIntegralPileDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTypeCombo();
	afx_msg void OnSelchangeK1Combo();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__BNDRINTEGRALPILE_H__)
