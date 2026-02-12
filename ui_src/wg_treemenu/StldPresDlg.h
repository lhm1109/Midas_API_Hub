#if !defined(AFX_STLDPRESDLG_H__66BE732A_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_STLDPRESDLG_H__66BE732A_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldPresDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxLdgr.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CStldPresDlg dialog

class CStldPresDlg : public CMenuBarChildDlg
{
// Construction
public:
	CStldPresDlg(CWnd* pParent = NULL);   // standard constructor
	~CStldPresDlg();
	virtual void Execute() { OnTmExecute(); }
	void CheckMode(BOOL bCheck);
// Dialog Data
	//{{AFX_DATA(CStldPresDlg)
	enum { IDD = IDD_TM_STLD_PRES };
	CTextUnit	m_wndForcePUnit;
	CTextUnit	m_wndDZUnit;
	CTextUnit	m_wndDYUnit;
	CTextUnit	m_wndDXUnit;
	MComboBox	m_wndLoadType;
	MComboBox	m_cboDirection;
	MButton	m_wndFrame;
	MComboBox	m_wndDirCobx;
	CBCGPStatic	m_wndDirTitle;
	MComboBox	m_wndElemType;
	CSelectLC	m_LoadCase;
	int		m_nOption;
	int		m_nPlateOption;
	int		m_nSolidOption;
	int		m_nSelectionMethod;
	int		m_nLoadType;
	CCobxLdgr m_wndLdgrCobx;
	//}}AFX_DATA
	CBCGPStatic   m_Title[4];
	CEditUnit m_Value[4];
	CTextUnit m_Unit[4];
	CEditUnit m_ConcentVal[4];


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldPresDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	CArray<UINT, UINT> m_aCtrlDisable;
	CArray<UINT, UINT> m_aCtrlValue[4];
	CArray<UINT, UINT> m_aCtrlUniform;

	CArray<UINT,UINT> m_arCtrlProject;
	CArray<UINT,UINT> m_arCtrlDirection;

	CArray<UINT,UINT> m_arCtrlPlateOpt;
	CArray<UINT,UINT> m_arCtrlSolidOpt;

	CArray<UINT,UINT> m_arConLType;
	CArray<UINT,UINT> m_arNoConLType;

	CBitmap* m_pBitmap;

protected:
	int m_nDistPrjCtrl;
	void DistPrjCtrl();
	void EnableValueEdit(BOOL bEnable);
	void EnableValue(BOOL bEnable);
	void SetDirection(int nElemType);
	void SetStateOfCtrl(int nElemType, int nDir, BOOL bUniform);
	void ChangeBitmap(int nElemType);
	void GetElemByType(int nOption, int nElemType, CArray<UINT, UINT> &aSelKey,CArray<int,int>&arFaceOrEdge);
	int  GetForceNum(int nElemType, int nDir);
	void SetUnitType(int nElemType);

	void ModifyProjectCtrl(int nIndex);
	void ChangePrjCtrlPosition(BOOL bUpDown);  //bUpDown=1 위로, bUpDown=0 아래로.
	BOOL m_bUpDown;

	int  GetPlateSolidOptionMan();
	void SelectionMethodMan();
	void DlgSizeMan();
	void LoadTypeCtrlMan();
	int  GetElemTypeExternal();


	// Generated message map functions
	//{{AFX_MSG(CStldPresDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnChangeOption();
	afx_msg void OnClickLoadCaseBtn();
	afx_msg void OnChangeElemType();
	afx_msg void OnChangeDirection();
	afx_msg void OnChangeUniform();
	afx_msg void OnSelchangeTmDirCombo2();
	afx_msg void OnTmSolidOptionRadio();
	afx_msg void OnTmPlateOptionRadio();
	afx_msg void OnTmSelectionMethod();
	afx_msg void OnSelchangeTmLoadtypeCombo();
	afx_msg void OnTmDefineLdgrBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDPRESDLG_H__66BE732A_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
