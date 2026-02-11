#if !defined(AFX_CMWINDITEMEURO2005PARAM_H__B2F29E7D_04B1_48EF_84AA_4068064B5887__INCLUDED_)
#define AFX_CMWINDITEMEURO2005PARAM_H__B2F29E7D_04B1_48EF_84AA_4068064B5887__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemEuro2005Param.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMWindItemEuro2005Param dialog

class CCMWindItemEuro2005Param : public CDialogMove
{
// Construction
public:
	CCMWindItemEuro2005Param(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMWindItemEuro2005Param)
	enum { IDD = IDD_ETC_WIND_ITEM_EURO2005_PARAM };
	MButton	  m_chkOrography;
	MButton	  m_chkNeighbor;
	MButton	  m_chkDisplace;
	MComboBox	m_cobxOroType;
	MComboBox	m_cobxBldgLoc;
	CEditUnit	m_editOroH;
	CEditUnit	m_editOroLu;
	CEditUnit	m_editOroLd;
	CEditUnit	m_editOroX;
	CTextUnit	m_unitOroH;
	CTextUnit	m_unitOroLu;
	CTextUnit	m_unitOroLd;
	CTextUnit	m_unitOroX;	
	CEditUnit	m_editNeiH;
	CEditUnit	m_editNeiHa;
	CEditUnit	m_editNeiX;
	CEditUnit	m_editNeiDl;	
	CEditUnit	m_editNeiHh;
	CTextUnit	m_unitNeiH;
	CTextUnit	m_unitNeiX;
	CTextUnit	m_unitNeiHa;
	CTextUnit	m_unitNeiDl;  
	CTextUnit	m_unitNeiHh;	
	CEdit   	m_editDisTC;
	CEditUnit	m_editDisHa;
	CEditUnit	m_editDisX;
	CTextUnit m_unitDisHa;
	CTextUnit m_unitDisX;  
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemEuro2005Param)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitControls();
	void Data2Dlg();
	BOOL Dlg2Data();
	void InitOroTypeCobx();
	void InitBldgLocCobx();
	void SetTerrainCategory();
	void CtrlManager();

public:
	BOOL   m_bOrography;
	BOOL   m_bNeighbor; 
	BOOL   m_bDisplace; 
	int    m_nOroType;
	int    m_nBldgLoc;  
	int    m_nTerrain;
	double m_dOroH;     
	double m_dOroLu;    
	double m_dOroLd;    
	double m_dOroX;     
	double m_dNeiH;     
	double m_dNeiHa;    
	double m_dNeiX;     
	double m_dNeiDl;    
	double m_dNeiDs;    
	double m_dNeiHh;    
	double m_dDisHa;    
	double m_dDisX;     

	// Generated message map functions
	//{{AFX_MSG(CCMWindItemEuro2005Param)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCmdOrographyCheck();
	afx_msg void OnCmdNeighborCheck();
	afx_msg void OnCmdDisplaceCheck();
	afx_msg void OnSelchangeCmdBldgLocCmb();
	afx_msg void OnSelchangeCmdOroTypeCmb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMWINDITEMEURO2005PARAM_H__B2F29E7D_04B1_48EF_84AA_4068064B5887__INCLUDED_)
