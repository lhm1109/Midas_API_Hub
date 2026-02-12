#if !defined(AFX_CMWINDITEMEURO2005_H__B2111A3B_1634_4BEC_BE40_C3B205CEB262__INCLUDED_)
#define AFX_CMWINDITEMEURO2005_H__B2111A3B_1634_4BEC_BE40_C3B205CEB262__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemEuro2005.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMWindItemEuro2005 dialog
class CCMWindItemDlg;
class CCMWindItemEuro2005 : public CChildDialog
{
// Construction
public:
	CCMWindItemEuro2005(CWnd* pParent = NULL);   // standard constructor

	void SetData2Dlg(T_WIND_EURO2005& data, BOOL bModify, BOOL bSingapore);
	BOOL SetDlg2Data(T_WIND_EURO2005& data);
	void SetNationalAnnex(BOOL bSingapore);

// Dialog Data
	//{{AFX_DATA(CCMWindItemEuro2005)
	enum { IDD = IDD_ETC_WIND_ITEM_EURO2005 };
	MComboBox	m_cobxTerrain;
	CEditUnit m_editCfr;
	CEditUnit m_editVb;
	CEditUnit m_editCdir;
	CEditUnit m_editCseason;
	CEditUnit m_editKl;
	CEditUnit m_editH;
	CEditUnit m_editCpe10;
	CEditUnit m_editCpe01;
	CEditUnit m_editCpeLee;
	CEditUnit m_editLackFactor;
	CEditUnit m_editCsCd;
	CTextUnit	m_unitH;
	MButton	  m_chkForcCoef;
	CEditUnit m_editForceCoef;
	CFoldGroupBox_Cross m_grpWindParam;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemEuro2005)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void InitControls();
	void InitDefaultData();
	void InitCategoryCombo();
	void Data2Dlg();
	BOOL Dlg2Data(T_WIND_EURO2005* pData);
	void CtrlManager();
	
protected:
	T_WIND_EURO2005  m_Data;
	T_WIND_EURO2005  m_DataBack;
	CCMWindItemDlg *m_pParent;
	BOOL    m_bSingapore;
	BOOL    m_bModify;
	BOOL    m_bInit;
	double  m_dH;
	CArray<UINT, UINT> m_aCtrlCpeMethod;
	CArray<UINT, UINT> m_aCtrlLackMethod;
	CArray<UINT, UINT> m_aCtrlCpeInput;
	CArray<UINT, UINT> m_aCtrlCpeLack;



// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMWindItemEuro2005)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdCscdBtn();
	afx_msg void OnCmdParaBtn();
	afx_msg void OnCmdCpeMethodRadio();
	afx_msg void OnCmdLackMethodRadio();
	afx_msg void OnCmdForcCoefCheck();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMWINDITEMEURO2005_H__B2111A3B_1634_4BEC_BE40_C3B205CEB262__INCLUDED_)

