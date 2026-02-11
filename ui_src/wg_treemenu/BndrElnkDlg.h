#if !defined(AFX_BNDRELNKDLG_H__19067F01_6A49_11D4_92DF_0000C0B0E6B3__INCLUDED_)
#define AFX_BNDRELNKDLG_H__19067F01_6A49_11D4_92DF_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrElnkDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxBngr.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MillustViewer.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "wg_treemenures.h"

#include "BndrElnkGrid.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CBndrElnkDlg dialog
class CBndrElnkDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CBndrElnkDlg(CWnd* pParent = NULL);   // standard constructor
	~CBndrElnkDlg();
	virtual void PostNcDestroy() { m_wndPicture.CloseDialog(); CMenuBarChildDlg::PostNcDestroy(); }
	virtual void Execute();

// Dialog Data
	//{{AFX_DATA(CBndrElnkDlg)
	enum { IDD = IDD_TM_BNDR_ELNK };
	CCobxBngr	 m_wndGroupCombo;     // Boundary Group Name
	MComboBox  m_wndELNKTypeCmb;    // Elastic Link Data Type;
	MButton    m_chkShearSpr;       // Shear Spring Location
	CEditUnit  m_editDistRatioSDy;  // Distance Ratio From End I - SDy
	CEditUnit  m_editDistRatioSDz;  // Distance Ratio From End I - SDz
	MComboBox	 m_wndBetaAngle;      // Beta Angle Combo 
	CMouseEdit m_wnd2Nodes;         // 2 Nodes
	MButton	   m_wndCopyLoad;       // Copy Elastic Link
	MEdit	     m_wndDistEdit;       // Copy Elastic Link - Distance 
	CTextUnit	 m_wndDistUnit;       // Copy Elastic Link - Distance Unit
	CFormulaEditSpin      m_editNtimes;        // Number of Times
	CFormulaEditSpin      m_editNodeInc;       // Node Increment
	//int		     m_wndLinkType;

	MComboBox  m_wndDirectionCmb;   // Direction
	CEditUnit  m_wndDistRatioEndI;  // Distance Ratio From End I

	MComboBox  m_wndMlfccmbx;

	//}}AFX_DATA
	CEditUnit m_wndEdit[6]; // SDx, SDy, SDz, SRx, SRy, SRz
	CTextUnit m_wndUnit[6]; // SDx, SDy, SDz, SRx, SRy, SRz
	BOOL      m_bFixed[6];

	//T_ELNK_D::nSubOption ==7 Limited-displacement-sliding-bearing
	CEditUnit m_wndDispEdit[6]; // SDx, SDy, SDz, SRx, SRy, SRz
	CTextUnit m_wndDispUnit[6]; // SDx, SDy, SDz, SRx, SRy, SRz

	CArray<UINT, UINT> m_aCtrlDisable;
	CArray<UINT, UINT> m_aCtrlOption;        // Add, Delete
	CArray<UINT, UINT> m_aCtrlCopyLoad;      // Copy Elastic Link
	CArray<UINT, UINT> m_aCtrlAxis;          // Copy Elastic Link - Axis
	CArray<UINT, UINT> m_aCtrlSpring;        // SDx, SDy, SDz, SRx, SRy, SRz
	CArray<UINT, UINT> m_aCtrlCopyDist;      // Copy Elastic Link - Distance
	CArray<UINT, UINT> m_aCtrlCopyNodeInc;   // Copy Elastic Link - Node Inc.
	CArray<UINT, UINT> m_aCtrlCopyType;      // Node Inc., Distance
	CArray<UINT, UINT> m_aCtrlDistRatio;     // Shear Spring Location
	CArray<UINT, UINT> m_aCtrlMultiLinear;   // Multi-Linear Type
	CArray<UINT, UINT> m_aCtrlDistRatioEndI; // Distance Ratio From End I

	CArray<UINT, UINT> m_aCtrlShearSpringLoc;// Shear Spring Location
	CArray<UINT, UINT> m_aCtrlBetaAngle;     // Beta Angle
	CArray<UINT, UINT> m_aCtrlSelNodes;      // Sel Nodes
	CArray<UINT, UINT> m_aCtrlCopyElstLink;  // Copy Elastic Link
	CArray<UINT, UINT> m_aCtrlApplyClose;  // Apply Close

	CArray<UINT, UINT> m_aCtrlNoLimited;	// No- Limited-displacement-sliding-bearing
	CArray<UINT, UINT> m_aCtrlLiDisp;		// Limited-displacement-sliding-bearing

// Overrides
	virtual	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
protected:
	BOOL m_bSelectByLabelMode = FALSE;
	unsigned int m_CurElnkKeyBySelectLabel = 0;
public:
	virtual BOOL ExternalInit_by_SelectLabel(UINT key);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrElnkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support  
	//}}AFX_VIRTUAL
	LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnNodeListEnter(WPARAM wParam, LPARAM lParam);
// Implementation
protected:
	void AlignControl();
	void AlignControlDynamic();
	void InitComboBoxData();
	void InitComboBoxDataDirection();
	void ChangeBitMap();
	void RestoreMlfcCbx();
	void RestoreMlfcData();
	void RestoreRlfcData();
	void SortMultiLinearValue(double dDistance[10], double dForce[10], int nCount);

	void AddLdsb();//Limited - displacement - sliding - bearing(MGEN_CH)
	BOOL AddLdsb_Elnk(T_ELNK_K Key, T_ELNK_D& rData);

	CDBDoc* m_pDoc;

	MillustViewer m_wndPicture;

	// Generated message map functions
	//{{AFX_MSG(CBndrElnkDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmDefineGroupButton();
	afx_msg void OnMlfcButton();
	afx_msg void OnChangeOption();
	afx_msg void OnTmElasticLinkDataTypeChange();
	afx_msg void OnTmElnkFixedChk();
	afx_msg void OnTmShearCheck();
	afx_msg void OnTmCopyLoad();
	afx_msg void OnTmCopyType();  
	afx_msg void OnTmDirectionChange();
	afx_msg void OnViewStartLinkNum();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BNDRELNKDLG_H__19067F01_6A49_11D4_92DF_0000C0B0E6B3__INCLUDED_)





















