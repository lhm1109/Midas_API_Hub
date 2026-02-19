#if !defined(__DGNCONBARDLG_H__)
#define __DGNCONBARDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConBarDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\mit_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"	
#include "..\mit_frx\MComboBox.h"
#include "..\wg_base\FoldGroupBox.h"

#include "HeaderPre.h"


class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnConBarDlg dialog

class __MY_EXT_CLASS__ CDgnConBarDlg : public CDialogMove
{
// Construction
public:
	CDgnConBarDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnConBarDlg)
	enum { IDD = IDD_DGN_CON_BAR_DLG1 };	
	CTextUnit	m_BEHSpaceUnit;
	CTextUnit	m_BEVSpaceUnit;
	CTextUnit	m_DwUnit;
	CTextUnit	m_DeUnit;
	CTextUnit	m_cDoUnit;
	CTextUnit	m_brDoUnit;
	CTextUnit	m_DtUnit;
	CTextUnit	m_DbUnit;
	mit::frx::MComboBox	m_WEndBar;
	mit::frx::MComboBox	m_WHBar;
	mit::frx::MComboBox	m_BEWHBar;
	mit::frx::MComboBox	m_CStirrup;
	mit::frx::MComboBox	m_BrStirrup;
	mit::frx::MComboBox	m_BStirrup;
	mit::frx::MComboBox	m_BSide;
	mit::frx::MComboBox	m_BStirrupNum;
	mit::frx::MComboBox	m_CStirrupYNum;
	mit::frx::MComboBox	m_CStirrupZNum;
	mit::frx::MComboBox	m_BrStirrupYNum;
	mit::frx::MComboBox	m_BrStirrupZNum;
	CString	m_BMainBar;
	CString	m_CMainBar;
	CString	m_BrMainBar;
	CString	m_WVBar;
	double	m_Db;
	double	m_Dt;
	double	m_brDo;
	double	m_cDo;
	double	m_BEHSpace;
	double	m_BEVSpace;
	double	m_De;
	double	m_Dw;
	// add by cylee 05.06.24
	BOOL    m_bAIJBeamRecommend;
	int     m_iSpliceSubType;
	int     m_bsplice_radio;
	BOOL    m_bAIJColRecommend;
	int     m_csplice_radio;
	BOOL    m_bAIJBraRecommend;
	int     m_rsplice_radio;
	// Add by sshan. MNET:2619.('20070126)
	mit::frx::MEdit		m_DoublyRainEdit;
	BOOL    m_bDoublyRain;
	double	m_dDoublyRain;
	// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
	BOOL    m_bConsiderSpacingLimitBeam;
	BOOL    m_bConsiderSpacingLimitColumn;
	BOOL    m_bConsiderSpacingLimitBrace;
	// Add by sshan (090222) NO:3909
	//BOOL    m_bVerEndWall;
	//CComboBox    m_ComboVerEndWall;
	//CComboBox    m_ComboVerEndFyWall;

	mit::frx::MButton m_chkMatlByRbar;
	
	CFoldGroupBox_Cross_New	m_wndBeamDgnFrame;
	CFoldGroupBox_Cross_New	m_wndColmDgnFrame;
	CFoldGroupBox_Cross_New	m_wndBrtrDgnFrame;
	CFoldGroupBox_Cross_New	m_wndWallDgnFrame;
	//}}AFX_DATA
public:
	int m_SpinNo;
	T_DCRB_D m_Dcrb;

public:
	CString Adding_ReBarSize(CString* strSize);
	void Initial_UnitData();
	void Initial_ExistData();
	void Initial_EditStaus(CDBDoc* pDoc);
	void Initial_DefaultData();
	void Cutting_ReBarSize(CString strSize, CArray<CString,CString>& RebarSize);
	
	// Add by ZINU.('01.7.7). Not used End Rebar if WSD.
	void Enable_EndBarEdit();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConBarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	CString m_strCode;
	BOOL Is_KCIUSD07_KS_KS01RC(BOOL bOnlyDgnCode=FALSE);
	void ControlMan();
	void SetBEControlByRCSpecialWall();
	void AlignCtrl(const CString& strCode);

	// Generated message map functions
	//{{AFX_MSG(CDgnConBarDlg)
	afx_msg void OnDgnRcSelbarsize1();
	afx_msg void OnDgnRcSelbarsize2();
	afx_msg void OnDgnRcSelbarsize3();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnRcSelbarsize4();
	afx_msg void OnDgnAddWalldata();
	afx_msg void OnDgnAIJWidCheck();
	afx_msg void OnDgnRcBeam();
	afx_msg void OnDgnRcColumn();
	afx_msg void OnDgnRcBrace();
	afx_msg void OnDgnRcWall();
	afx_msg void OnDgnBeamDoublyReinCheck();
	// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
	afx_msg void OnDgnConsiderSpacingLimitCheck();
	// Add by sshan (090222) NO:3909
	afx_msg void OnDgnRcVerEndWall();
	afx_msg void OnMatlRbarDiamCheck();
	afx_msg void OnMatlRbarDiamBtn();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONBARDLG_H__5D7AB3A9_D4E8_11D3_888F_0000C0F30D4D__INCLUDED_)
