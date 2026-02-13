#if !defined(AFX_DGNRATINGOPTIONDLG_H__)
#define AFX_DGNRATINGOPTIONDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRationOptionDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MButton.h"

class CDBDoc;

/////////////////////////////////////////////////////////////////////////////
// CDgnRationOptionDlg dialog

class CDgnRationOptionDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnRationOptionDlg(CWnd* pParent = NULL);   // standard constructor

public:
	void Initial_SelectItem();
	void Initial_Data();
	// Add by ZINU.('01.1.3).
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	
// Dialog Data
	//{{AFX_DATA(CDgnRationOptionDlg)
	enum { IDD = IDD_DGN_RATING_OPTION_DLG };
	int		m_nOption;
	mit::frx::MButton	m_ratioI;
	mit::frx::MButton	m_ratioJ;
	mit::frx::MButton	m_ratioIJ;
	CEditUnit	m_editDroopI;
	CEditUnit	m_editDroopJ;
	CEditUnit	m_editStrainI;
	CEditUnit	m_editStrainJ;
	CEditUnit	m_editImpact_I;
	CEditUnit	m_editImpact_J;
	CEditUnit	m_editAddReponceF_I;
	CEditUnit	m_editAddReponceF_J;
	CEditUnit	m_editTraffit;
	CEditUnit	m_editLoad;
	CTextUnit	m_unitDroopI;
	CTextUnit	m_unitDroopJ;
	CEditUnit	m_editCompen;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnRationOptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
// 	afx_msg void OnDgnClose();
// 	afx_msg void OnDgnExecute();
	virtual BOOL OnInitDialog();
	virtual void Execute();
	afx_msg void OnDgnAr();
	afx_msg void OnDgnDel();
	afx_msg void SetEnableByCtrl();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Controls ID
	CArray<UINT, UINT> m_aGroupBox;
	CArray<UINT, UINT> m_aRadioBtn;

	CArray<UINT, UINT> m_aCtrl_I;
	CArray<UINT, UINT> m_aCtrl_J;

	CArray<UINT, UINT> m_aStrain_I;
	CArray<UINT, UINT> m_aStrain_J;
	CArray<UINT, UINT> m_aDispl_I;
	CArray<UINT, UINT> m_aDispl_J;
	CArray<UINT, UINT> m_aImpact_I;
	CArray<UINT, UINT> m_aImpact_J;
	CArray<UINT, UINT> m_aAddReponce_I;
	CArray<UINT, UINT> m_aAddReponce_J;
	CArray<UINT, UINT> m_aKSCE_USD05;
	CArray<UINT, UINT> m_aEtc;

	CDBDoc* m_pDoc;

	CRect m_rcGrpDefineBig;
	CRect m_rcGrpDefineSmall;
	CRect m_rcGrpFactorBig;
	CRect m_rcGrpFactorSmall;
			
	void CtrlMoveToSamePos(int nTargetCtrlID, int nMoveingCtrlID);
	void SetControl();
	void SetControlText();
	void HideAndMoveControlByDB();
	BOOL SetControlEnableByDB();
	
	// Generated message map functions
	//{{AFX_MSG(CDgnRationOptionDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNRATINGOPTIONDLG_H__)
