#if !defined(AFX_DGNMESHSLABWALLLCOMDLG_H__D20A9A5A_1E5B_4429_B53D_D850D1A97853__INCLUDED_)
#define AFX_DGNMESHSLABWALLLCOMDLG_H__D20A9A5A_1E5B_4429_B53D_D850D1A97853__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnMeshSlabWallLcomDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\MIT_frx\MCheckListBox.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnMeshSlabWallLcomDlg dialog
#include "HeaderPre.h"
class CDBDoc;
class __MY_EXT_CLASS__ CDgnMeshSlabWallLcomDlg : public CDialogMove
{
// Construction
public:
	CDgnMeshSlabWallLcomDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnMeshSlabWallLcomDlg)
	enum { IDD = IDD_DGN_MESH_SLAB_WALL_LCOM };
	mit::frx::MCheckListBox	m_lstSlabStrength;
	mit::frx::MCheckListBox   m_lstSlabELSStrength;
	mit::frx::MCheckListBox	m_lstSlabService ;
	mit::frx::MCheckListBox	m_lstWallStrength;
	mit::frx::MCheckListBox	m_lstSlabDeflection;
	mit::frx::MCheckListBox	m_lstShellStrength;
	mit::frx::MCheckListBox	m_lstSlabCrackService;
	//}}AFX_DATA

	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnMeshSlabWallLcomDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;	

	BOOL Dlg2Data();
	void Data2Dlg();
	void SetDescription(UINT nLcomK);
	void SetDlgTitle();

	// Generated message map functions
	//{{AFX_MSG(CDgnMeshSlabWallLcomDlg)
	virtual BOOL OnInitDialog();  
	virtual void OnOK();
	afx_msg void OnSelchangeSlabStrnLst();
	afx_msg void OnSelchangeSlabELSStrnLst();
	afx_msg void OnSelchangeSlabServLst();
	afx_msg void OnSelchangeSlabCrackServLst();
	afx_msg void OnSelchangeWallStrnLst();
	afx_msg void OnSelchangeSlabDeflLst();
	afx_msg void OnSelchangeShellStrnLst();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CArray<UINT, UINT> aCtrlID_HideInKR;
	void HideWallCtrls();
	void ShowHideShellCtrls(BOOL bShowWallCtrl);  
	void ShowULSAndELSCtrl();
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNMESHSLABWALLLCOMDLG_H__D20A9A5A_1E5B_4429_B53D_D850D1A97853__INCLUDED_)
