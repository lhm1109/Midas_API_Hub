#if !defined(__ELEMROTATEDLG_H__)
#define      __ELEMROTATEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElemRotateDlg.h : header file
//
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MEdit.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CElemRotateDlg dialog
class _ElemRotate
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
	
	BOOL m_bCopyNodeAttr;
	BOOL m_bCopyElemAttr;

	int  m_nMatlInc;
	int  m_nSectInc;
	int  m_nThikInc;
	BOOL m_bMatlIncRep;
	BOOL m_bSectIncRep;
	BOOL m_bThikIncRep;

	BOOL m_bIntstNode;
	BOOL m_bIntstElem;

	BOOL m_bBetaAngle;
};

class CElemRotateDlg : public CMenuBarChildDlg
{
// Construction
public:
	BOOL GetParameter(_ElemRotate& ER);
	BOOL GetDistanceDrDz(double& Dr, double& Dz);
	BOOL GetRotAngle(double & RotAng);
	BOOL GetRotAngle(_ElemRotate &ER);
	BOOL GetNTimes(int & NTimes);
	BOOL GetSecondPoint(double & Px, double &Py , double & Pz);
	BOOL GetFirstPoint(double &Px, double &Py, double &Pz);
	virtual void Execute();
	CElemRotateDlg(CWnd* pParent = NULL);   // standard constructor

	void MoveElem(_ElemRotate& ER);
	void CopyElem(_ElemRotate& ER);
	
	CArray<UINT, UINT> m_MoveDisable;
	CArray<UINT, UINT> m_2PEnable;
	CArray<UINT, UINT> m_OneAxisDisable;
	CArray<UINT, UINT> m_OneAxisEnable;
	CArray<UINT, UINT> m_aThik;

// Dialog Data
	//{{AFX_DATA(CElemRotateDlg)
	enum { IDD = IDD_TM_ELEM_ROTATE };
	CTextUnit	m_wndUEAngleUnit;
	MEdit	m_edtUEAngle;
	MButton	m_wndThikIncRep;
	MButton	m_wndSectIncRep;
	MButton	m_wndMatlIncRep;
	CMouseEdit	m_wndDistAxialEdit;
	CTextUnit	m_wndDistAxialUnit;
	CFormulaEditSpin	m_wndThikInc;
	CFormulaEditSpin	m_wndSectInc;
	CFormulaEditSpin	m_wndMatlInc;
	CTextUnit	m_wndAngleStatic;
	CTextUnit	m_wndDistStatic3;
	CTextUnit	m_wndDistStatic2;
	CTextUnit	m_wndDistStatic1;
	CMouseEdit	m_wndDist;
	CFormulaEditSpin m_wndRotAngle;
	CMouseEdit	m_wnd2ndPoint;
	CMouseEdit	m_wnd1stPoint;
	CFormulaEditSpin	m_wndIterNum;
	int		m_nCopyOrMove;
	int		m_nRotAxis;
	BOOL	m_bCopyNodeAttr;
	BOOL  m_bCopyElemAttr;
	BOOL	m_bIntstNode;
	BOOL	m_bIntstElem;
	BOOL	m_bBetaAngle;
	int		m_nAngleType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElemRotateDlg)
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
	//{{AFX_MSG(CElemRotateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposTmNtimesSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmAxisRadio();
	afx_msg void OnTmCopyMoveRadio();
	afx_msg void OnTmCopynodeOpt();
	afx_msg void OnTmCopyattrOpt();
	afx_msg void OnTmDuplicateTol();
	afx_msg void OnDeltaposTmRotAngleSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposTmIncSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmIntstTol();
	afx_msg void OnChangeAngleType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEMROTATEDLG_H__E3A50C07_6049_11D3_8899_0000C0A2329D__INCLUDED_)
