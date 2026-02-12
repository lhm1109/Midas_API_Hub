#if !defined(AFX_NODEMIRRORDLG_H__FBFF4A42_5FA9_11D3_8899_0000C0A2329D__INCLUDED_)
#define AFX_NODEMIRRORDLG_H__FBFF4A42_5FA9_11D3_8899_0000C0A2329D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NodeMirrorDlg.h : header file
//

#include "..\wg_db\wg_db_TextUnit.h"
/////////////////////////////////////////////////////////////////////////////
// CNodeMirrorDlg dialog
class _NodeMirror
{
public:
	BOOL m_bIsCopy;
	int       m_nWhatPlane; // 0 : YZ  1: XY 2: ZX 3:3Point
	
	double m_XCoordW,  m_XCoordU;
	double m_ZCoordW,  m_ZCoordU; 
	double m_YCoordW,  m_YCoordU;
	
	double m_Point1XW ,m_Point1YW ,m_Point1ZW;
	double m_Point1XU ,m_Point1YU ,m_Point1ZU;

	double m_Point2XW ,m_Point2YW ,m_Point2ZW;
	double m_Point2XU ,m_Point2YU ,m_Point2ZU;

	double m_Point3XW ,m_Point3YW ,m_Point3ZW;
	double m_Point3XU ,m_Point3YU ,m_Point3ZU;

	BOOL m_bMergeDupNode;
	BOOL m_bAttrCopy;
	BOOL m_bIntersect;
};

class CNodeMirrorDlg : public CMenuBarChildDlg
{
public:
	CArray<UINT,UINT> m_MoveDisable  ;
	CArray<UINT,UINT> m_PlaneDisable ;
	CArray<UINT,UINT> m_PlaneYZEnable;
	CArray<UINT,UINT> m_PlaneXYEnable;
	CArray<UINT,UINT> m_PlaneXZEnable;
	CArray<UINT,UINT> m_Plane3PEnable;
	
	// Construction
public:
	CNodeMirrorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	BOOL GetPoint(CMouseEdit& m_wndPoint, double &Px, double &Py, double &Pz);
	BOOL GetParameter(_NodeMirror &NM);
	void MoveNode(_NodeMirror& NM);
	void CopyNode(_NodeMirror& NM);

// Dialog Data
	//{{AFX_DATA(CNodeMirrorDlg)
	enum { IDD = IDD_TM_NODE_MIRROR };
	CMouseEdit	m_wndZCoord;
	CMouseEdit	m_wndYCoord;
	CMouseEdit	m_wndXCoord;
	CMouseEdit	m_wnd3rdPoint;
	CMouseEdit	m_wnd2ndPoint;
	CMouseEdit	m_wnd1stPoint;
	CTextUnit   m_wndXCoordUnit;
	CTextUnit   m_wndYCoordUnit;
	CTextUnit   m_wndZCoordUnit;
	CTextUnit   m_wndP1CoordUnit;
	CTextUnit   m_wndP2CoordUnit;
	CTextUnit   m_wndP3CoordUnit;
	int		m_nCopyOrMove;
	int		m_nWhatPlane;
	BOOL	m_bIsMergeDupNode;
	BOOL	m_bIsCopyAttr;
	BOOL	m_bIntersect;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNodeMirrorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNodeMirrorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmCopyMoveRadio();
	afx_msg void OnTmPlaneRadio();
	afx_msg void OnTmCopynodedata();
	afx_msg void OnTmTolerance();
	afx_msg void OnTmIntsectTol();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NODEMIRRORDLG_H__FBFF4A42_5FA9_11D3_8899_0000C0A2329D__INCLUDED_)
