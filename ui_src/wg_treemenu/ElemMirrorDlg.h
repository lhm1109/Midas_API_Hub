#if !defined(__ELEMMIRRORDLG_H__)
#define      __ELEMMIRRORDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElemMirrorDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_TextUnit.h"
/////////////////////////////////////////////////////////////////////////////
// CElemMirrorDlg dialog
class _ElemMirror
{
public:
	BOOL   m_bIsCopy;
	int    m_nWhatPlane; // 0 : YZ  1: XY 2: ZX 3:3Point
	
	double m_XCoordW,  m_XCoordU;
	double m_ZCoordW,  m_ZCoordU; 
	double m_YCoordW,  m_YCoordU;
	
	double m_Point1XW ,m_Point1YW ,m_Point1ZW;
	double m_Point1XU ,m_Point1YU ,m_Point1ZU;

	double m_Point2XW ,m_Point2YW ,m_Point2ZW;
	double m_Point2XU ,m_Point2YU ,m_Point2ZU;

	double m_Point3XW ,m_Point3YW ,m_Point3ZW;
	double m_Point3XU ,m_Point3YU ,m_Point3ZU;

	BOOL m_bAttrCopy;
	BOOL m_bNodeAttrCopy;

	int  m_nMatlInc;
	int  m_nSectInc;
	int  m_nThikInc;

	BOOL m_bIntstNode;
	BOOL m_bIntstElem;

	BOOL m_bBetaAngle;
	BOOL m_bReverseElem;  // Reverse Element Local 
												// Added by bugboy 2000.8.27
};


class CElemMirrorDlg : public CMenuBarChildDlg
{
public:
	CArray<UINT,UINT> m_MoveDisable  ;
	CArray<UINT,UINT> m_PlaneDisable ;
	CArray<UINT,UINT> m_PlaneYZEnable;
	CArray<UINT,UINT> m_PlaneXYEnable;
	CArray<UINT,UINT> m_PlaneXZEnable;
	CArray<UINT,UINT> m_Plane3PEnable;
	CArray<UINT, UINT> m_aThik;
	
// Construction
public:
	CElemMirrorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	BOOL GetPoint(CMouseEdit& m_wndPoint, double &Px, double &Py, double &Pz);
	BOOL GetParameter(_ElemMirror &EM);
	void MoveElem(_ElemMirror& EM);
	void CopyElem(_ElemMirror& EM);

// Dialog Data
	//{{AFX_DATA(CElemMirrorDlg)
	enum { IDD = IDD_TM_ELEM_MIRROR };
	CFormulaEditSpin	m_wndThikInc;
	CFormulaEditSpin	m_wndSectInc;
	CFormulaEditSpin	m_wndMatlInc;
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
	BOOL	m_bIsCopyElemAttr;
	BOOL  m_bIsCopyNodeAttr;
	BOOL	m_bIntstNode;
	BOOL	m_bIntstElem;
	BOOL	m_bBetaAngle;
	BOOL	m_bReverseElem;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElemMirrorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CElemMirrorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmCopyMoveRadio();
	afx_msg void OnTmPlaneRadio();
	afx_msg void OnTmCopynodedata();
	afx_msg void OnTmCopyelemdata();
	afx_msg void OnTmTolerance();
	afx_msg void OnDeltaposTmIncSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmIntstTol();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEMMIRRORDLG_H__E3A50C06_6049_11D3_8899_0000C0A2329D__INCLUDED_)
