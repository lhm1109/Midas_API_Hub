// DgnUserMPhiDlg.h: interface for the CDgnUserMPhiDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNUSERMPHIDLG_H__2BFB3891_F8C3_465D_AD62_7269BB4456F2__INCLUDED_)
#define AFX_DGNUSERMPHIDLG_H__2BFB3891_F8C3_465D_AD62_7269BB4456F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_db\DB_ST_DN.h"
#include "..\MIT_frx\MEdit.h"
#include "DgnUserMPhiGrid.h"

using namespace mit::frx;

class CDgnUserMPhiDlg : public CDialogMove
{
public:
	CDgnUserMPhiDlg(CWnd* pParent = NULL);   // standard constructor
	~CDgnUserMPhiDlg();   // standard deconstruction

// Dialog Data
	//{{AFX_DATA(CDgnMPhiCurveDlg)
	enum { IDD = IDD_DGN_USER_MPHI_DLG };
	
	MEdit     m_Size_Y_Edit;
	MEdit     m_Size_Z_Edit;
	CEditUnit	m_YieldCur_Y_Edit;
	CEditUnit	m_YieldCur_Z_Edit;
	CEditUnit	m_UltiCur_Y_Edit;
	CEditUnit	m_UltiCur_Z_Edit;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnMPhiCurveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:	

	CDgnUserMPhiGrid* m_pY_Grid; 
	CDgnUserMPhiGrid* m_pZ_Grid; 

public:	
	CArray<T_RSDL_MPHI_D,T_RSDL_MPHI_D> m_arPierUserMPhi[2]; // Pier - »ç¿ëÀÚ ÀÔ·Â M-Phi °î¼± Á¤º¸
	double m_dPierYieldCurvature[2];   // Pier - »ç¿ëÀÚ ÀÔakr·Â Ç×º¹°î·ü
	double m_dPierUltimateCurvature[2];// Pier - »ç¿ëÀÚ ÀÔ·Â ±ØÇÑ°î·ü

public:	
	void InitUnit();
	void InitCtrl();

	void Data2Dlg();
	BOOL Dlg2Data();

public:
	// Generated message map functions
	//{{AFX_MSG(CDgnUserMPhiDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOK();
	afx_msg void OnKillFocusYSize();
	afx_msg void OnKillFocusZSize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_DGNUSERMPHIDLG_H__2BFB3891_F8C3_465D_AD62_7269BB4456F2__INCLUDED_)
