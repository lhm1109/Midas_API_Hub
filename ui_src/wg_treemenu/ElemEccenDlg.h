#if !defined(AFX_ELEMECCENDLG_H__7BCD8C19_8D07_44C9_BB9A_09E718B6D5BD__INCLUDED_)
#define AFX_ELEMECCENDLG_H__7BCD8C19_8D07_44C9_BB9A_09E718B6D5BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElemEccenDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CElemEccenDlg dialog
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\TextUnit.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\CobxBngr.h"

#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MEdit.h"

using namespace mit::frx;

class CElemEccenDlg : public CMenuBarChildDlg
{
// Construction
public:
	CElemEccenDlg(CWnd* pParent = NULL);   // standard constructor

	virtual void Execute();

// Dialog Data
	//{{AFX_DATA(CElemEccenDlg)
	enum { IDD = IDD_TM_ELEM_ECCENTRIC };
	CTextUnit	m_unitGDZj;
	CTextUnit	m_unitGDZi;
	CTextUnit	m_unitGDYj;
	CTextUnit	m_unitGDYi;
	CTextUnit	m_unitGDXj;
	CTextUnit	m_unitGDXi;
	CEditUnit	m_editGDZj;
	CEditUnit	m_editGDZi;
	CEditUnit	m_editGDYj;
	CEditUnit	m_editGDYi;
	CEditUnit	m_editGDXj;
	CEditUnit	m_editGDXi;
	MButton	m_wndSectIncRep;
	MButton	m_wndMatlIncRep;
	MButton m_chkCopyNodeAttr;
	MButton m_chkCopyElemAttr;
	CFormulaEditSpin m_wndSectInc;
	CFormulaEditSpin m_wndMatlInc;
	CCobxBngr m_cobxBngr;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElemEccenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<UINT,UINT> m_DisableOnMove;
	CArray<UINT,UINT> m_aCtrlCopyMove;

	// Generated message map functions
	//{{AFX_MSG(CElemEccenDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmCopyRadio();
	afx_msg void OnTmMoveRadio();
	afx_msg void OnTmCopynodedata();
	afx_msg void OnTmCopyelemdata();
	afx_msg void OnTmBtnBngr();
	afx_msg void OnDeltaposTmIncSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEMECCENDLG_H__7BCD8C19_8D07_44C9_BB9A_09E718B6D5BD__INCLUDED_)
