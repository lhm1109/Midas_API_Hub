#if !defined(__ELEMTRANSLATEDLG_H__)
#define      __ELEMTRANSLATEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElemTranslateDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CElemTranslateDlg dialog
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\mit_frx\MillustViewer.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MEdit.h"

#define ELEMTRANS_XAXIS 0
#define ELEMTRANS_YAXIS 1
#define ELEMTRANS_ZAXIS 2
#define ELEMTRANS_ARBIT 3

using namespace mit::frx;

class  _ElemTranslate
{
public:
	BOOL m_bIsCopy;
	int  m_NTimes;
	int  m_nOption; // node increment/equal/unequal
	int  m_nNodeInc;
	double m_EQ_UX, m_EQ_UY, m_EQ_UZ;
	double m_EQ_WX, m_EQ_WY, m_EQ_WZ;
	int  m_nUEQ_Axis;
	CArray<double,double> m_UEQ_DistList;
	double m_UEQ_UX,m_UEQ_UY,m_UEQ_UZ;
	double m_UEQ_WX,m_UEQ_WY,m_UEQ_WZ;
	BOOL m_bAttrNodeCopy;
	BOOL m_bAttrElemCopy;

	int  m_nMatlInc;
	int  m_nSectInc;
	int  m_nThikInc;
	BOOL m_bMatlIncRep;
	BOOL m_bSectIncRep;
	BOOL m_bThikIncRep;

	BOOL m_bIntstNode;
	BOOL m_bIntstElem;
	BOOL m_bDelFreeNode;
};


class CElemTranslateDlg : public CMenuBarChildDlg
{
// Construction
public:
	CElemTranslateDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL GetParameter(_ElemTranslate& ET);
	
//	void CtrlEnableDisable(CArray<UINT,UINT> &CtrlArray,BOOL bIsEnable);
	virtual void Execute();
	void MoveElem(_ElemTranslate& ET);
	void CopyElem(_ElemTranslate& ET);
	//CNodeTransDlg(CWnd* pParent = NULL);   // standard constructor
	CArray<UINT,UINT> m_DisableOnMove;
	CArray<UINT,UINT> m_EnableOnMove;
	CArray<UINT,UINT> m_NodeInc; 
	CArray<UINT,UINT> m_EqualDistance;
	CArray<UINT,UINT> m_UEqualDistance;
	CArray<UINT,UINT> m_aIterNum;
	CArray<UINT,UINT> m_aIterNum2;
	CArray<UINT,UINT> m_aThik;
// Dialog Data
	//{{AFX_DATA(CElemTranslateDlg)
	enum { IDD = IDD_TM_ELEM_TRANSLATE };
	MButton	m_wndThikIncRep;
	MButton	m_wndSectIncRep;
	MButton	m_wndMatlIncRep;
	CFormulaEditSpin m_wndThikInc;
	CFormulaEditSpin m_wndSectInc;
	CFormulaEditSpin m_wndMatlInc;
	CFormulaEditSpin m_wndIterNum2;
	CMouseEdit	m_wndDirVector;
	CMouseEdit	m_wndEQDist;
	mit::frx::MEdit	m_wndUEQDist;
	CFormulaEditSpin m_wndIterNum;
	CFormulaEditSpin m_wndNodeInc;
	CTextUnit	m_DistUnit3;
	CTextUnit	m_DistUnit2;
	CTextUnit	m_DistUnit1;
	int		m_nCopyOrMove;
	int		m_nOption;
	int		m_nChooseAxis;
	BOOL  m_bAttrNodeCopy;
	BOOL	m_bAttrElemCopy;
	BOOL	m_bIntstNode;
	BOOL	m_bIntstElem;
	BOOL	m_bDelFreeNode;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElemTranslateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CElemTranslateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmCopyRadio();
	afx_msg void OnTmMoveRadio();
	afx_msg void OnTmAxisRadios();
	afx_msg void OnDeltaposTmIterNumSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmNodeIncRadio();
	afx_msg void OnTmEqualDistRadio();
	afx_msg void OnTmUnequalDistRadio();
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

#endif // !defined(AFX_ELEMTRANSLATEDLG_H__E3A50C08_6049_11D3_8899_0000C0A2329D__INCLUDED_)
