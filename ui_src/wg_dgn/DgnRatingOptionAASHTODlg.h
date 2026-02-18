#if !defined(AFX_DGNRATINGOPTIONAASHTODLG_H__)
#define AFX_DGNRATINGOPTIONAASHTODLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRatingOptionAASHTODlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MButton.h"

class CDBDoc;

/////////////////////////////////////////////////////////////////////////////
// CDgnRatingOptionAASHTODlg dialog

class CDgnRatingOptionAASHTODlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
	// Construction
public:
	CDgnRatingOptionAASHTODlg(CWnd* pParent = NULL);   // standard constructor

public:
	void Initial_SelectItem();
	void Initial_Data();
	// Add by ZINU.('01.1.3).
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	// Dialog Data
	//{{AFX_DATA(CDgnRatingOptionAASHTODlg)
	enum { IDD = IDD_DGN_RATING_OPTION_AASHTO_DLG };
	int		m_nElemType;
	int		m_nOption;
	int   m_nPosition;
	int   m_nCalcType;
	mit::frx::MButton	m_ratioI;
	mit::frx::MButton	m_ratioJ;
	mit::frx::MButton	m_ratioIJ;
	mit::frx::MButton m_bAuto;
	mit::frx::MButton m_bUser;
	CEditUnit	m_editDroopI;
	CEditUnit	m_editDroopJ;
	CEditUnit	m_editStrainI;
	CEditUnit	m_editStrainJ;
	CTextUnit	m_unitDroopI;
	CTextUnit	m_unitDroopJ;
	CTextUnit	m_unitStrainI;
	CTextUnit	m_unitStrainJ;
	CEditUnit m_editDynamic;
	CEditUnit m_editKFactor;

	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnRatingOptionAASHTODlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
// 	afx_msg void OnDgnClose();
// 	afx_msg void OnDgnExecute();
	virtual BOOL OnInitDialog();
	virtual void Execute();
	afx_msg void OnDgnAr();
	afx_msg void OnDgnDel();
	afx_msg void SetEnableByCtrl();
	afx_msg void OnDgnPSCElemTypeRdo();
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Controls ID
	CArray<UINT, UINT> m_aGroupBox;
	CArray<UINT, UINT> m_aRadioBtn;

	CArray<UINT, UINT> m_aAutoUser;

	CArray<UINT, UINT> m_aCtrl_I;
	CArray<UINT, UINT> m_aCtrl_J;

	CArray<UINT, UINT> m_aDispl_I;
	CArray<UINT, UINT> m_aDispl_J;
	CArray<UINT, UINT> m_aStrain_I;
	CArray<UINT, UINT> m_aStrain_J;

	CArray<UINT, UINT> m_aDynamic;
	CArray<UINT, UINT> m_aK;
	CArray<UINT, UINT> m_aEtc;

	CArray<UINT, UINT> m_aElemTypeCtrl;
	CArray<UINT, UINT> m_aMoveCtrl;

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
	//{{AFX_MSG(CDgnRatingOptionAASHTODlg)
	// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNRATINGOPTIONAASHTODLG_H__)
