#if !defined(__DGNCPGMATHYBRIDDLG_H__)
#define __DGNCPGMATHYBRIDDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnCpgMatDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

#include "HeaderPre.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnCpgMatHybridDlg dialog

class __MY_EXT_CLASS__ CDgnCpgMatHybridDlg : public CDialogMove
{
// Construction
public:
	CDgnCpgMatHybridDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnCpgMatHybridDlg)
	enum { IDD = IDD_DGN_CPG_MAT_HYBRID_DLG };
	// Flange(Top)
	mit::frx::MComboBox	m_sMatName_top;
	CString	m_sMatName2_top;
	CTextUnit	m_FuUnit_top;
	CTextUnit	m_Fy1Unit_top;
	CTextUnit	m_Fy2Unit_top;
	CTextUnit	m_Fy3Unit_top;
	CTextUnit	m_Fy4Unit_top;
	CTextUnit	m_Fy5Unit_top; 
	CTextUnit	m_Fy6Unit_top; 
	CTextUnit	m_EsUnit_top;
	double	m_Fu_top;
	double	m_Es_top;
	double	m_Fy1_top;
	double	m_Fy2_top;
	double	m_Fy3_top;
	double	m_Fy4_top;
	double	m_Fy5_top;  
	double	m_Fy6_top;  
	//Flange(Bot)
	MComboBox	m_sMatName_bot;
	CString	m_sMatName2_bot;
	CTextUnit	m_FuUnit_bot;
	CTextUnit	m_Fy1Unit_bot;
	CTextUnit	m_Fy2Unit_bot;
	CTextUnit	m_Fy3Unit_bot;
	CTextUnit	m_Fy4Unit_bot;
	CTextUnit	m_Fy5Unit_bot; 
	CTextUnit	m_Fy6Unit_bot; 
	CTextUnit	m_EsUnit_bot;
	double	m_Fu_bot;
	double	m_Es_bot;
	double	m_Fy1_bot;
	double	m_Fy2_bot;
	double	m_Fy3_bot;
	double	m_Fy4_bot;
	double	m_Fy5_bot;  
	double	m_Fy6_bot;  
	//Web
	MComboBox	m_sMatName_web;
	CString	m_sMatName2_web;
	CTextUnit	m_FuUnit_web;
	CTextUnit	m_Fy1Unit_web;
	CTextUnit	m_Fy2Unit_web;
	CTextUnit	m_Fy3Unit_web;
	CTextUnit	m_Fy4Unit_web;
	CTextUnit	m_Fy5Unit_web; 
	CTextUnit	m_Fy6Unit_web; 
	CTextUnit	m_EsUnit_web;
	double	m_Fu_web;
	double	m_Es_web;
	double	m_Fy1_web;
	double	m_Fy2_web;
	double	m_Fy3_web;
	double	m_Fy4_web;
	double	m_Fy5_web;  
	double	m_Fy6_web;  
	//}}AFX_DATA

public:
	void Data2Dlg();
	BOOL Dlg2Data();
	void Initial_MaterialCombo();
	//Flange Top
	void Initial_SteelMatNameTop(CString sCodeName, CString sCodeMatlName);
	void SetSteelStrengthTop(CString strCode, CString strName);
	void ShowSteelMatNameTop(CString &strSCode);
	//Flange Bot
	void Initial_SteelMatNameBot(CString sCodeName, CString sCodeMatlName);
	void SetSteelStrengthBot(CString strCode, CString strName);
	void ShowSteelMatNameBot(CString &strSCode);
	//Web
	void Initial_SteelMatNameWeb(CString sCodeName, CString sCodeMatlName);
	void SetSteelStrengthWeb(CString strCode, CString strName);
	void ShowSteelMatNameWeb(CString &strSCode);

	void Initial_Unit();
	void SetData(const int &nMatlKey, T_MATD_ALL *pData, const CString &strCode, const CString &strMatl);
	int GetChkKind(CString strMatlCode);
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnCpgMatHybridDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

public:
	T_MATD_ALL *m_pData;
	CString m_strCode;
	CString m_strMatlName;
	int     m_nMatlKey;

	// Generated message map functions
	//{{AFX_MSG(CDgnCpgMatHybridDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeTopGrade();
	afx_msg void OnSelchangeBotGrade();
	afx_msg void OnSelchangeWebGrade();
	afx_msg void OnDgnSrcmatModify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"


#endif
