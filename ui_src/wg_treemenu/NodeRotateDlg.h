#if !defined(AFX_NODEROTATEDLG_H__70E9CCC7_3443_11D3_8899_0000C0A2329D__INCLUDED_)
#define AFX_NODEROTATEDLG_H__70E9CCC7_3443_11D3_8899_0000C0A2329D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NodeRotateDlg.h : header file
//
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CNodeRotateDlg dialog
class _NodeRotate
{
public:
	BOOL   m_bIsCopy;
	int    m_NTimes;

	CArray<double, double> m_arAngle;

	double m_AngleOfRotDEG;
	double m_DistR,m_DistZ;
	int    m_nAxis; // 0 : X , 1 : Y , 2 : Z , 3 : 2Point

	double m_FirstP_UX,m_FirstP_UY, m_FirstP_UZ;
	double m_FirstP_WX,m_FirstP_WY, m_FirstP_WZ;
	double m_SecondP_UX,m_SecondP_UY, m_SecondP_UZ;
	double m_SecondP_WX,m_SecondP_WY, m_SecondP_WZ;

	double m_AxisV_Ux, m_AxisV_Uy, m_AxisV_Uz;
	double m_AxisV_Wx, m_AxisV_Wy, m_AxisV_Wz;
	
	BOOL m_bMergeDupNode;
	BOOL m_bAttrCopy;
	BOOL m_bIntersect;
};

class CNodeRotateDlg : public CMenuBarChildDlg
{
// Construction
public:
	BOOL GetParameter(_NodeRotate& NR);
	BOOL GetDistanceDrDz(double& Dr, double& Dz);
	BOOL GetRotAngle(double & RotAng);
	BOOL GetRotAngle(_NodeRotate &NR);
	BOOL GetNTimes(int & NTimes);
	BOOL GetSecondPoint(double & Px, double &Py , double & Pz);
	BOOL GetFirstPoint(double &Px, double &Py, double &Pz);
	virtual void Execute();
	CNodeRotateDlg(CWnd* pParent = NULL);   // standard constructor

	void MoveNode(_NodeRotate& NR);
	void CopyNode(_NodeRotate& NR);
	
	CArray<UINT, UINT> m_MoveDisable;
	CArray<UINT, UINT> m_2PEnable;
	CArray<UINT, UINT> m_OneAxisDisable;
	CArray<UINT, UINT> m_OneAxisEnable;
// Dialog Data
	//{{AFX_DATA(CNodeRotateDlg)
	enum { IDD = IDD_TM_NODE_ROTATE };
	CTextUnit	m_wndUEAngleUnit;
	MEdit		m_edtUEAngle;
	CTextUnit	m_wndDistAxialUnit;
	CMouseEdit	m_wndDistAxialEdit;
	CTextUnit	m_wndAngleStatic;
	CTextUnit	m_wndDistStatic3;
	CTextUnit	m_wndDistStatic2;
	CTextUnit	m_wndDistStatic1;
	CMouseEdit	m_wndDist;
	CFormulaEditSpin	m_wndRotAngle;
	CMouseEdit	m_wnd2ndPoint;
	CMouseEdit	m_wnd1stPoint;
	CFormulaEditSpin	m_wndIterNum;
	int		m_nCopyOrMove;
	int		m_nRotAxis;
	BOOL	m_bCopyNodeAttr;
	BOOL	m_bMergeDupNodeCheck;
	BOOL	m_bIntersect;
	int		m_nAngleType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNodeRotateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	void AlignControl();
	CArray<UINT, UINT> m_arEqualCtrl;
	CArray<UINT, UINT> m_arUnEqualCtrl;

protected:

	// Generated message map functions
	//{{AFX_MSG(CNodeRotateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposTmNtimesSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmAxisRadio();
	afx_msg void OnTmCopyMoveRadio();
	afx_msg void OnTmCopyattrOpt();
	afx_msg void OnTmDuplicateTol();
	afx_msg void OnDeltaposTmRotAngleSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmIntsectTol();
	afx_msg void OnAngleType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NODEROTATEDLG_H__70E9CCC7_3443_11D3_8899_0000C0A2329D__INCLUDED_)
