#if !defined(AFX_NODETRANSDLG_H__70E9CCC8_3443_11D3_8899_0000C0A2329D__INCLUDED_)
#define AFX_NODETRANSDLG_H__70E9CCC8_3443_11D3_8899_0000C0A2329D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NodeTransDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNodeTransDlg dialog
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;

#define NODETRANS_XAXIS 0
#define NODETRANS_YAXIS 1
#define NODETRANS_ZAXIS 2
#define NODETRANS_ARBIT 3

class  _NodeTranslate
{
public:
		BOOL m_bIsCopy;
		int  m_NTimes;
		BOOL m_bIsEqual;
		double m_EQ_UX, m_EQ_UY, m_EQ_UZ;
		double m_EQ_WX, m_EQ_WY, m_EQ_WZ;
		int  m_nUEQ_Axis;
		CArray<double,double> m_UEQ_DistList;
		double m_UEQ_UX,m_UEQ_UY,m_UEQ_UZ;
		double m_UEQ_WX,m_UEQ_WY,m_UEQ_WZ;
		BOOL m_bDupNode;
		BOOL m_bAttrCopy;
		BOOL m_bIntersect;
};

class CNodeTransDlg : public CMenuBarChildDlg
{
// Construction
public:

	/*
	BOOL GetNumberOfTimes  (CString& NTStr    , int & NTimes);
	BOOL GetUEDistance     (CString& UEStr    ,CArray<double,double>&UEDistants);
	BOOL GetDirectionVector(CString& strVector,double& Ux,double& Uy,double& Uz);
	BOOL GetEQDistance     (CString& EQStr    ,double& Dx,double& Dy,double& Dz);
	*/

	BOOL GetParameter(_NodeTranslate& NT);
	
//	void CtrlEnableDisable(CArray<UINT,UINT> &CtrlArray,BOOL bIsEnable);
	virtual void Execute();
	void MoveNode(_NodeTranslate& NT);
	void CopyNode(_NodeTranslate& NT);
	CNodeTransDlg(CWnd* pParent = NULL);   // standard constructor
	CArray<UINT,UINT> m_DisableOnMove;
	CArray<UINT,UINT> m_EqualDistance;
	CArray<UINT,UINT> m_UEqualDistance;
	CArray<UINT,UINT> m_aIterNum;

// Dialog Data
	//{{AFX_DATA(CNodeTransDlg)
	enum { IDD = IDD_TM_NODE_TRANSLATE };
	CMouseEdit	m_wndDirVector;
	CMouseEdit	m_wndEQDist;
	MEdit		m_wndUEQDist;
	CFormulaEditSpin	m_wndIterNum;
	CTextUnit	m_DistUnit3;
	CTextUnit	m_DistUnit2;
	CTextUnit	m_DistUnit1;
	int		m_nCopyOrMove;
	int		m_nEqualOrUnEqual;
	int		m_nChooseAxis;
	BOOL	m_bDupNode;
	BOOL	m_bAttrCopy;
	BOOL	m_bIntersect;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNodeTransDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNodeTransDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmCopyRadio();
	afx_msg void OnTmMoveRadio();
	afx_msg void OnTmAxisRadios();
	afx_msg void OnDeltaposTmIterNumSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmEqualDistRadio();
	afx_msg void OnTmUnequalDistRadio();
	afx_msg void OnTmCopynodedata();
	afx_msg void OnTmTolerance();
	afx_msg void OnTmIntsectTol();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NODETRANSDLG_H__70E9CCC8_3443_11D3_8899_0000C0A2329D__INCLUDED_)
