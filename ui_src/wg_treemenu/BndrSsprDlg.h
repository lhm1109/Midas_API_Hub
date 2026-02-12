#if !defined(AFX_BNDRSSPRDLG_H__56828181_8D3A_11D4_A678_00010263A1CE__INCLUDED_)
#define AFX_BNDRSSPRDLG_H__56828181_8D3A_11D4_A678_00010263A1CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrSsprDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxBngr.h"
#include "..\MIT_frx\MillustViewer.h"
#include "BndrSsprCalcDlg.h"
#include "..\MIT_frx\MButton.h"
/////////////////////////////////////////////////////////////////////////////
// CBndrSsprDlg dialog

struct Sspring {
	int nElType;
	int nFace;
	int nConvertType;
	double dSx;
	double dSy;
	double dSz;
	double dPHU;

	BOOL bDamping;
	double dCx;
	double dCy;
	double dCz;

	int nLinkType;
	double dLength;
	double dWidth;
	int nDirection;
	double dUx;
	double dUy;
	double dUz;
	int nSpringType; // MNET:2743 20070424 mylee
};

class CBndrSsprDlg : public CMenuBarChildDlg
{
// Construction
public:
	CBndrSsprDlg(CWnd* pParent = NULL);   // standard constructor
	~CBndrSsprDlg();
	virtual void Execute();
// Dialog Data
	//{{AFX_DATA(CBndrSsprDlg)
	enum { IDD = IDD_TM_BNDR_SSPR };
	CTextUnit	m_wndWidthUnit;
	CEditUnit	m_wndWidth;
	mit::frx::MComboBox	m_CobxElemType;
	mit::frx::MComboBox	m_cboElemType;
	CTextUnit	m_unitSpringX;
	CTextUnit	m_unitSpringY;
	CTextUnit	m_unitSpringZ;
	CTextUnit	m_unitPHU;
	CEditUnit	m_edtSpringX;
	CEditUnit	m_edtSpringY;
	CEditUnit	m_edtSpringZ;
	CEditUnit	m_edtPHU;

	mit::frx::MButton	m_chkDamping;
	CTextUnit	m_unitDampingX;
	CTextUnit	m_unitDampingY;
	CTextUnit	m_unitDampingZ;
	CEditUnit	m_edtDampingX;
	CEditUnit	m_edtDampingY;
	CEditUnit	m_edtDampingZ;


// 	MButton	m_chkTension;
// 	MButton	m_chkCompression;
	CMouseEdit m_edtLinkLength;
	CTextUnit	m_unitLinkLength;
	CTextUnit	m_unitLinkModulus;
	CEditUnit	m_edtLinkModulus;
	CEditUnit	m_edtLinkLimitStr;
	CTextUnit	m_untLinkLimitStr;
	mit::frx::MComboBox	m_cmbSpringType2;
	mit::frx::MComboBox	m_cboDirection;
	CCobxBngr	m_wndGroupCombo;
	int		m_nConvertSpringType;
	int		m_nElementType;
	int   m_nSpringType;
	int   m_nSpringType2;

	int m_nConvertType;
	int m_nElemSelType;

	int m_nElemSelSelection;

	mit::frx::MComboBox	m_cmbSpringType; // MNET:2743 20070424 mylee
	mit::frx::MComboBox	m_cmbDirectionNon;
	CTextUnit	m_untNonLinearModulus;
	CEditUnit	m_edtNonLinearModulus;

	mit::frx::MComboBox m_cmbElemSelType;

	mit::frx::MComboBox m_cmbLoaclAxis;
	mit::frx::MComboBox m_cmbPlanarFace;
	mit::frx::MComboBox m_cmbPlanarEdge;
	mit::frx::MComboBox m_cmbSolidFace;

	mit::frx::MComboBox m_cmbFrameDirection;
	mit::frx::MComboBox m_cmbPlanarFaceDirection;
	mit::frx::MComboBox m_cmbPlanarEdgeDirection;
	mit::frx::MComboBox m_cmbSolidFaceDirection;

	CTextUnit	m_unitWidth;
	CEditUnit	m_edtWidth;

	mit::frx::MComboBox m_cmbSpringProp;
	
	CTextUnit	m_unitSpringPropModulus;
	CEditUnit	m_edtSpringPropModulus;

	mit::frx::MillustViewer m_wndPicture;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrSsprDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void CheckMode(BOOL bCheck);

protected:
	CDBDoc* m_pDoc;

	T_SSPS_D m_Data;
	T_SOSD_DATA m_SosdD;
	
	CArray<UINT,UINT> m_arCtrlLink;
	CArray<UINT,UINT> m_arCtrlSpring;

	CArray<UINT,UINT> m_arCtrlSpring_Type;
	CArray<UINT,UINT> m_arCtrlSpring_Linear;
	CArray<UINT,UINT> m_arCtrlSpring_Non;

	CArray<UINT,UINT> m_aElementType;
	CArray<UINT,UINT> m_aConvertType;
	CArray<UINT,UINT> m_aElemSelSpringProp;

	CArray<UINT,UINT> m_aFrameType;
	CArray<UINT,UINT> m_aPlanarFace;
	CArray<UINT,UINT> m_aPlanarEdge;
	CArray<UINT,UINT> m_aSolidFace;

	CArray<UINT,UINT> m_aElemSelSelection;
	CArray<UINT,UINT> m_arCtrlSpring_Damping;

	BOOL m_bUseSsprCalcDlg;

	void AlignControl();
	void InitUnitType();
	void InitCmbData();
	void ChangeBitmap();
	void ControlsShowHide();
	void EnableDisableControls();
	BOOL GetParameter(Sspring& Sspr);

	BOOL Dlg2Data();
	BOOL Data2Dlg();

	BOOL GetElemByType(CArray<UINT, UINT> &aSelKey,CArray<int,int>&arFaceOrEdge);

	// Generated message map functions
	//{{AFX_MSG(CBndrSsprDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmBndrSsprConvertType();
	afx_msg void OnTmBndrSsprDampingChk();
// 	afx_msg void OnTmBndrSsprTensChk();
// 	afx_msg void OnTmBndrSsprCompChk();
	afx_msg void OnTmBndrSsprTypePlanar();
	afx_msg void OnTmBndrSsprTypeSolid();
	afx_msg void OnSelchangeTmBndrSsprTypeSolidCbo();
	afx_msg void OnTmDefineGroupButton();
	afx_msg void OnSelchangeTmElemTypeCombo();
	afx_msg void OnSelchangeSpringTypeCombo();
	afx_msg void OnSelchangeSpringTypeCombo2();

	afx_msg void OnSelectChangeConvertType();
	afx_msg void OnSelectChangeElementType();
	afx_msg void OnSelectElemSelNodeorElem();
	afx_msg void OnClickCalcBtn_Mod();
	afx_msg void OnClickCalcBtn_Cz ();
	afx_msg void OnClickCalcBtn_Val();
	afx_msg void OnClickCalcBtn_Spr();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BNDRSSPRDLG_H__56828181_8D3A_11D4_A678_00010263A1CE__INCLUDED_)
