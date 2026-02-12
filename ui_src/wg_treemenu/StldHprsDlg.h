#if !defined(AFX_STLDHPRSDLG_H__8D099521_C992_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_STLDHPRSDLG_H__8D099521_C992_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldHprsDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxLdgr.h"

#include "..\mit_frx\MillustViewer.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CStldHprsDlg dialog

class CStldHprsDlg : public CMenuBarChildDlg
{
// Construction
public:
	CStldHprsDlg(CWnd* pParent = NULL);   // standard constructor
	~CStldHprsDlg();
	virtual void Execute() { OnTmExecute(); }
	void CheckMode(BOOL bCheck);
// Dialog Data
	//{{AFX_DATA(CStldHprsDlg)
	enum { IDD = IDD_TM_STLD_HPRS };
	mit::frx::MComboBox	m_cboDirection;
	CTextUnit	m_wndRefLevelUnit;
	CTextUnit	m_wndGradIntenUnit;
	CTextUnit	m_wndConstIntenUnit;
	CTextUnit	m_wndCurvedIntenUnit;
	CEditUnit	m_wndGradInten;
	CEditUnit	m_wndConstInten;
	CMouseEdit m_wndRefLevel;
	mit::frx::MComboBox	m_wndGradDir;
	CBCGPStatic	m_wndDirTitle;
	mit::frx::MComboBox	m_wndDirCobx;
	mit::frx::MComboBox	m_wndElemType;
	mit::frx::MComboBox m_wndLoadType;
	CSelectLC	m_LoadCase;
	int		m_nSelectionMethod;
	int		m_nOption;
	int   m_nCurveType;
	CCobxLdgr m_wndLdgrCobx;
	double m_dFactorA;
	double m_dFactorB;
	double m_dFactorC;

	mit::frx::MillustViewer m_wndEdgeFacePicture;
	mit::frx::MillustViewer m_wndLoadTypePicture;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldHprsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nDistPrjCtrl;
	void DistPrjCtrl();
	void SetDirection(int nElemType);
	void ChangeBitmap(int nElemType);
	void GetElemByType(int nOption, int nElemType, CArray<UINT, UINT> &aSelKey);

	void ModifyProjectCtrl(int nIndex);
	void ChangePrjCtrlPosition(BOOL bUpDown);  //bUpDown=1 위로, bUpDown=0 아래로.
	BOOL m_bUpDown;

	int  GetPlateSolidOptionMan();
	int  GetElemTypeExternal();
	void SelectionMethodMan();

	void ChangeLoadTypeMan();

	void GetElemByType(int nOption, int nElemType, CArray<UINT, UINT> &aSelKey,CArray<int,int>&arFaceOrEdge);
protected:
	CDBDoc* m_pDoc;
	CBitmap* m_pBitmap;
	CBitmap* m_pLoadTypeBitmap;
	CArray<UINT, UINT> m_aCtrlOption;
	CArray<UINT, UINT> m_aCtrlDisable;

	CArray<UINT,UINT> m_arCtrlProject;
	CArray<UINT,UINT> m_arCtrlDirection;

	CArray<UINT, UINT> m_arLinearType;
	CArray<UINT, UINT> m_arCurvedType;

	// Generated message map functions
	//{{AFX_MSG(CStldHprsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnClickLoadCaseBtn();
	afx_msg void OnChangeElemType();
	afx_msg void OnChangeOption();
	afx_msg void OnChangeGradDir();
	afx_msg void OnSelchangeTmLoadedEdge();
	afx_msg void OnSelchangeTmDirCombo2();
	afx_msg void OnTmSelectionMethod();
	afx_msg void OnTmDefineLdgrBtn();
	afx_msg void OnSelchangeTmLoadType();
	afx_msg void OnTmCurveMtdRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL GetReferenceLevel(double& rdRefLevel);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDHPRSDLG_H__8D099521_C992_11D3_92DE_0000C0B0E6B3__INCLUDED_)
