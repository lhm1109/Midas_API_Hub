#if !defined(AFX_CMPROPRENUMDLG_H__6EE59597_7398_4F18_9B85_410B5F72AF78__INCLUDED_)
#define AFX_CMPROPRENUMDLG_H__6EE59597_7398_4F18_9B85_410B5F72AF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMPropRenumDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMPropRenumDlg dialog

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMPropRenumDlg : public CDialogMove
{
// Construction
public:
	CCMPropRenumDlg(CWnd* pParent = NULL);   // standard constructor
	int m_nPropNum;// 0:Material, 1:Section, 2:Thickness

// Dialog Data
	//{{AFX_DATA(CCMPropRenumDlg)
	enum { IDD = IDD_CMD_PROP_RENUM_DLG };
	CListBox	m_List;
	//CSpinButtonCtrl	m_Spin;
	CFormulaEditSpin	m_edtIncrement;
	BOOL	m_bElemRenum;
	int		m_nIncrement;
	UINT	m_nStart;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMPropRenumDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CDBDoc* m_pDoc;
	void InitList();

	// Generated message map functions
	//{{AFX_MSG(CCMPropRenumDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdBtnClose();
	afx_msg void OnCmdBtnRenum();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMPROPRENUMDLG_H__6EE59597_7398_4F18_9B85_410B5F72AF78__INCLUDED_)
