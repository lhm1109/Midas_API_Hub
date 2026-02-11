#if !defined(__ELEMCREATEWALLOPENDLG_H__)
#define      __ELEMCREATEWALLOPENDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElemCreateWallOpenDlg.h : header file
//

#include "..\wg_db\wg_db_TextUnit.h"
#include "../MIT_frx/MillustViewer.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CElemCreateWallOpenDlg dialog
class  _ElemWallOpen
{
public:
	// Direction
	int  m_nDir;    // 0: i->j,  1: j->i,  2: Middle

	// Distance
	double m_dx;
	double m_dy;

	// Opening Size
	double m_dw;
	double m_dh;

	// divide option
	BOOL m_bMergeDupNode;
	BOOL m_bSubDivideFrame;
};

class CElemCreateWallOpenDlg : public CMenuBarChildDlg
{
// Construction
public:
	CElemCreateWallOpenDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CElemCreateWallOpenDlg();
	virtual void Execute();
	void CreateWallOpenElem(_ElemWallOpen& ED);
	BOOL GetParameter(_ElemWallOpen& ED);

// Dialog Data
	//{{AFX_DATA(CElemCreateWallOpenDlg)
	enum { IDD = IDD_TM_ELEM_WALL_OPEN };
	//}}AFX_DATA

	MEdit	m_edtDx;
	MEdit	m_edtDy;
	MEdit	m_edtDw;
	MEdit	m_edtDh;
	CTextUnit m_untDx;
	CTextUnit m_untDy;
	CTextUnit m_untDw;
	CTextUnit m_untDh;
	BOOL	m_bMergeDupNode;
	BOOL	m_bSubDivideFrame;
	MillustViewer m_wndPicture;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElemCreateWallOpenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CElemCreateWallOpenDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmTolerance();
	afx_msg void OnTmWallOpenDir();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

private:
	CArray<UINT,UINT> m_aDirCtrlsRdo;
	int   m_nDirType;
private:
	void ChangeBitmap(int nBitmap);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEMCREATEWALLOPENDLG_H__E3A50C04_6049_11D3_8899_0000C0A2329D__INCLUDED_)
