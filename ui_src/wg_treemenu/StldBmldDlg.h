#if !defined(AFX_STLDBMLDDLG_H__66BE7329_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_STLDBMLDDLG_H__66BE7329_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldBmldDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\SelectLC.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"
#include "..\wg_db\CobxLdgr.h"

#include "..\mit_frx\MillustViewer.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CStldBmldDlg dialog

class CStldBmldDlg : public CMenuBarChildDlg
{
// Construction
public:
	CStldBmldDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStldBmldDlg();
	virtual void Execute() { OnTmExecute(); }
// Dialog Data
	//{{AFX_DATA(CStldBmldDlg)
	enum { IDD = IDD_TM_STLD_BMLD };

	CSelectLC	m_LoadCase;
	CCobxLdgr	m_wndLdgrCobx;
	int		    m_nOption;
	MComboBox	m_cboLoadType;
	MButton	m_chkEccen;
	int       m_nEccenType;
	MComboBox	m_cmbEccenDir;
	CTextUnit	m_txtEccenDist;
	CEditUnit	m_edtEccenDistI;
	CEditUnit	m_edtEccenDistJ;
	MButton	  m_chkEccenDistJ;
	MButton	m_chkAdditionalH;
	CTextUnit	m_stcAdditionalDist;
	BOOL      m_bAdditionalDistJ;
	CEditUnit m_edtAdditionalDistI;
	CEditUnit m_edtAdditionalDistJ;
	int		    m_nDirection;
	MComboBox	m_wndDirection;
	int		    m_nProj;
	int		    m_nRelative;
	CFormulaEdit	m_wndD1;
	CFormulaEdit	m_wndD2;
	CFormulaEdit	m_wndD3;
	CFormulaEdit	m_wndD4;
	//}}AFX_DATA
	double    m_ValueD[4];
	CEditUnit m_ValueP[4];
	CTextUnit m_Unit;

	MillustViewer m_wndBmldTypePicture;

	virtual BOOL ExternalInit_by_SelectLabel(UINT key);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldBmldDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitLoadTypeCombo();
	void InitEccenDirectionCombo();
	void InitDirectionCombo();
	void AlignControl();
	void SetUnitType();
	void ChangeBitmap(int nBitmap);
	void ChangeValueTitle(int nLoadType);
	void EnableEccentricity(BOOL bEnable);
	void EnableAdditionalH(BOOL bEnable);
	void EnableProjection(BOOL bEnable);
	void EnableValueByUniform(int nUniform);
	void EnableValueEdit(BOOL bEnable);
	
	CDBDoc* m_pDoc;
	CBitmap*  m_pBitmap;

protected:
	CArray<UINT, UINT> m_aLengOptCtrl;
	CArray<UINT, UINT> m_aEccen;
	CArray<UINT, UINT> m_aEtc;
	CArray<UINT, UINT> m_aAdditionalH;
	BOOL m_bInitialized;
	BOOL m_bFirstInit;

	// Generated message map functions
	//{{AFX_MSG(CStldBmldDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnClickLoadCaseBtn();
	afx_msg void OnTmDefineLdgrBtn();
	afx_msg void OnChangeOption();
	afx_msg void OnChangeLoadType();
	afx_msg void OnTmEccenDist2Chk();
	afx_msg void OnTmAdditionalDistJChk();
	afx_msg void OnChangeDirection();
	afx_msg void OnClickEccenBtn();
	afx_msg void OnClickAdditionalDIstBtn();
	//}}AFX_MSG
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDBMLDDLG_H__66BE7329_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
