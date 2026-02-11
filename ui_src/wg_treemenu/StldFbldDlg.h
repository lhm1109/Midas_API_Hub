#if !defined(AFX_STLDFBLDDLG_H__F68DAEE2_C119_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_STLDFBLDDLG_H__F68DAEE2_C119_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldFbldDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxFbld.h"
#include "..\wg_db\wg_db_CobxLdgr.h"

#include "..\mit_frx\MillustViewer.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MEdit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CStldFbldDlg dialog

class CStldFbldDlg : public CMenuBarChildDlg
{
// Construction
public:
	CStldFbldDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStldFbldDlg();
	virtual void Execute();
// Dialog Data
	//{{AFX_DATA(CStldFbldDlg)
	enum { IDD = IDD_TM_STLD_FBLD };
	CFormulaEditSpin	m_wndSubNo;
	MComboBox	m_wndSubA2;
	MComboBox	m_wndAngleA1;
	MEdit	    m_wndDist;
	CEditUnit	m_wndUSW;
	CMouseEdit m_wndNodeList;
	CCobxFbld	m_wndLoadType;
	CTextUnit	m_wndDistUnit;
	CTextUnit	m_wndUSWUnit;
	CTextUnit	m_wndAngleA1Unit;
	MComboBox	m_wndLoadDir;
	MComboBox	m_wndDistType;
	CString	m_csAngleA1;
	CString	m_csSubA2;
	int		m_nProj;
	CString	m_csDescription;
	BOOL	m_bCopyFloorLoad;
	int		m_nAxis;
	BOOL	m_bConvertToBeamLoad;
	CCobxLdgr	m_wndLdgrCobx;
	MButton m_chkExclude;
	MButton m_chkAllow;

	MillustViewer m_wndFbldTypePicture;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldFbldDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	CArray<UINT, UINT> m_ctrlLoadAngleA1;
	CArray<UINT, UINT> m_ctrlProjection;
	CArray<UINT, UINT> m_ctrlCopyFloorLoad;
	CArray<UINT, UINT> m_ctrlLoadPolygon;
	CBitmap* m_pBitmap;

	void AdjustComboListBox();
	void ChangeBitmap(int nBitmap);
	BOOL GetParameter(T_FBLA_D &data, BOOL &bChangeBeamLoad, BOOL &bCopyLoad, 
										double &ux, double &uy, double &uz, CArray<double, double> &aDist);

	BOOL Do();

	// Generated message map functions
	//{{AFX_MSG(CStldFbldDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnSelchangeTmFblDistType();
	afx_msg void OnSelchangeTmFblLoadDir();
	afx_msg void OnTmFblCopyFbld();
	afx_msg void OnTmFblLoadtypeBtn();
	afx_msg void OnSetfocusTmFblNodeList();
	afx_msg void OnDeltaposTmFblSubNoSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmDefineLdgrBtn();
	//}}AFX_MSG
	afx_msg LRESULT OnMouseEditEnter(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseEditEscape(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDFBLDDLG_H__F68DAEE2_C119_11D3_92DE_0000C0B0E6B3__INCLUDED_)
