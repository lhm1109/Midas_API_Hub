#if !defined(AFX_NODECREATEDLG_H__70E9CCC6_3443_11D3_8899_0000C0A2329D__INCLUDED_)
#define AFX_NODECREATEDLG_H__70E9CCC6_3443_11D3_8899_0000C0A2329D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NodeCreateDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNodeCreateDlg dialog
#include "ColorEdit.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

class  _NodeCreate
{
public:
	 double  Nx,Ny,Nz;  
	 bool    IsNodePosError;
	 int     NCopyNum;
	 bool    IsNCopyError;
	 double  Dx,Dy,Dz;
	 bool    IsDistError;
	 void    InitErrorFlag()
	 {
		 IsNodePosError = FALSE;
		 IsNCopyError   = FALSE;
		 IsDistError    = FALSE;
		 Nz = Ny = Nx = 0.0; 
		 NCopyNum     = 0;
		 Dz = Dy = Dx = 0.0;
	 }
};

class CNodeCreateDlg : public CMenuBarChildDlg
{
protected:
	BOOL m_bIsParamError;
// Construction
public:
	BOOL AddNode(_NodeCreate& CS);
	void InitOption();
	BOOL GetParameter(_NodeCreate& CS);
	void Execute();
	CNodeCreateDlg(CWnd* pParent = NULL);   // standard constructor

	virtual void OnTabSelect(WPARAM wParam,LPARAM lParam);
// Dialog Data
	//{{AFX_DATA(CNodeCreateDlg)
	enum { IDD = IDD_TM_NODE_CREATE };
	CTextUnit	m_DistUnit0;
	CTextUnit	m_DistUnit2;
	CMouseEdit	m_wndDistXYZ;
	CMouseEdit	m_wndCoordXYZ;
	BOOL	m_bIsCreateSamePos;
	CString	m_strCoordXYZ;
	CString	m_strDisXYZ;
	CFormulaEditSpin	m_spinNCopy;
	BOOL	m_bIsIntersectFrameElem;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNodeCreateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNodeCreateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmTolerance();
	//afx_msg void OnDeltaposTmIterSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmIntsectTol();
	//}}AFX_MSG
	afx_msg LRESULT OnMouseEditEnter(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	int m_nNCopy;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NODECREATEDLG_H__70E9CCC6_3443_11D3_8899_0000C0A2329D__INCLUDED_)
