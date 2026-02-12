#if !defined(AFX_NODEDIVIDEDLG_H__FBFF4A43_5FA9_11D3_8899_0000C0A2329D__INCLUDED_)
#define AFX_NODEDIVIDEDLG_H__FBFF4A43_5FA9_11D3_8899_0000C0A2329D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NodeDivideDlg.h : header file
//

#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MEdit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CNodeDivideDlg dialog
class  _NodeDivide
{
public:
		int  m_nDivision;
		int  m_nOption;
		CArray<double,double> m_List;
		int m_nNode1;
		int m_nNode2;
		BOOL m_bMergeDupNode;
		BOOL m_bIntersect;
};

class CNodeDivideDlg : public CMenuBarChildDlg
{
public:
	CArray<UINT,UINT> m_EqCtrl;
	CArray<UINT,UINT> m_UeqCtrl;
	CArray<UINT,UINT> m_ParamCtrl;

// Construction
public:
	CNodeDivideDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	void DivideNode(_NodeDivide& ND);
	BOOL GetParameter(_NodeDivide& ND);

	BOOL GetNumOfDiv(int &nDiv);
	BOOL GetUeqDistance(CArray<double, double>& aDistList);
	BOOL GetParamRatio(CArray<double, double>& aRatioList);
	BOOL GetNode4Div(int &nNode1, int &nNode2);

	LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);

// Dialog Data
	//{{AFX_DATA(CNodeDivideDlg)
	enum { IDD = IDD_TM_NODE_DIVIDE };
	CFormulaEditSpin	m_wndNumOfDiv;
	CTextUnit	m_wndUeqDistUnit;
	MEdit	m_wndUeqDist;
	MEdit	m_wndRatio;
	CMouseEdit	m_wndNode4Divide;
	BOOL	m_bMergeDupNode;
	int		m_nOption;
	BOOL	m_bIntersect;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNodeDivideDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNodeDivideDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmEqualDist();
	afx_msg void OnTmUnequalDist();
	afx_msg void OnTmParamDist();
	afx_msg void OnDeltaposTmNumofdivSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmTolerance();
	afx_msg void OnTmIntsectTol();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NODEDIVIDEDLG_H__FBFF4A43_5FA9_11D3_8899_0000C0A2329D__INCLUDED_)
