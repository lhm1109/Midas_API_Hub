#if !defined(AFX_STLDLBLDDLG_H__84D2D3A1_C8C8_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_STLDLBLDDLG_H__84D2D3A1_C8C8_11D3_92DE_0000C0B0E6B3__INCLUDED_

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\SelectLC.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"
#include "..\wg_db\CobxLdgr.h"

#include "..\mit_frx\MillustViewer.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldLbldDlg.h : header file
//

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CStldLbldDlg dialog

class CStldLbldDlg : public CMenuBarChildDlg
{

// Construction
public:
	CStldLbldDlg(CWnd* pParent = NULL);   // standard constructor
	~CStldLbldDlg();
	virtual void Execute() { OnTmExecute(); }
	
// Dialog Data
	//{{AFX_DATA(CStldLbldDlg)
	enum { IDD = IDD_TM_STLD_LBLD };

	CSelectLC	m_LoadCase;
	CCobxLdgr m_wndLdgrCobx;
	int		    m_nOption;
	MComboBox	m_cboLoadType;
	MComboBox	m_CobxSelType;
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
	CMouseEdit	  m_wndNodeList;
	BOOL	    m_bCopyLoad;
	int		    m_nAxis;
	CTextUnit	m_wndDistUnit;
	int		    m_nCurveMethod;
	CEditUnit m_wndA;
	CEditUnit m_wndB;
	CEditUnit m_wndC;
	CString	  m_csDistance;
	//}}AFX_DATA
	double    m_ValueD[4];
	CEditUnit m_ValueP[4];
	CTextUnit m_Unit;
	CTextUnit m_Unit2;

	MillustViewer m_wndLbldTypePicture;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldLbldDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	CBitmap*  m_pBitmap;
	int  m_nSelType;

	void InitLoadTypeCombo();
	void InitEccenDirectionCombo();
	void DirectionCtrlMan();
	void AlignControl();
	void SetUnitType(int nLoadType);
	void ChangeBitmap(int nBitmap);
	void ChangeValueTitle(int nLoadType);
	void CurveValueCtrlMan();
	void EnableElementSelection(BOOL bEnable);
	void EnableEccentricity(BOOL bEnable);
	void EnableAdditionalH(BOOL bEnable);
	void EnableCurvedType(BOOL bEnable);
	void EnableValueByUniform(int nUniform);
	void EnableProjection(BOOL bEnable);
	void EnableValueEdit(BOOL bEnable);

protected:
	CArray<UINT, UINT> m_aCopyLoadCtrl;
	CArray<UINT, UINT> m_aLengOptCtrl;
	CArray<UINT, UINT> m_aNodeListCtrl;
	CArray<UINT, UINT> m_aCtrlLine;
	CArray<UINT, UINT> m_aCtrlCurve;
	BOOL m_bInitialized;
	BOOL m_bFirstInit;

	// Generated message map functions
	//{{AFX_MSG(CStldLbldDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnClickLoadCaseBtn();
	afx_msg void OnTmDefineLdgrBtn();
	afx_msg void OnChangeOption();
	afx_msg void OnChangeLoadType();
	afx_msg void OnSelchangeTmSelCombo();
	afx_msg void OnTmEccenDist2Chk();
	afx_msg void OnTmAdditionalDistJChk();
	afx_msg void OnChangeDirection();
	afx_msg void OnTmCopyLoad();
	afx_msg void OnTmCurveMtdRdo();
	afx_msg void OnClickEccenBtn();
	afx_msg void OnClickAdditionalDIstBtn();
	//}}AFX_MSG
	afx_msg LRESULT OnMouseEditEnter(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDLBLDDLG_H__84D2D3A1_C8C8_11D3_92DE_0000C0B0E6B3__INCLUDED_)
