#if !defined(AFX_CMDCSG2SLOAD_H__CB10F7F1_03B4_4764_8E28_7BA09985D2C1__INCLUDED_)
#define AFX_CMDCSG2SLOAD_H__CB10F7F1_03B4_4764_8E28_7BA09985D2C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMDcsG2SLoad.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
/////////////////////////////////////////////////////////////////////////////
// CCMDcsG2SLoad dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMDcsG2SLoad : public CDialogMove
{
// Construction
public:
	CCMDcsG2SLoad(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMDcsG2SLoad)
	enum { IDD = IDD_DCS_GEN2SDS_LOAD };
	CTextUnit	m_unitLevelReaction;
	CEdit	m_edtLevelReaction;
	CEdit	m_edtFileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMDcsG2SLoad)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	void InitCtrlData();

	// Generated message map functions
	//{{AFX_MSG(CCMDcsG2SLoad)
	afx_msg void OnCmdG2sloadFileBtn();
	afx_msg void OnCmdG2sloadOk();
	afx_msg void OnCmdG2sloadCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMDCSG2SLOAD_H__CB10F7F1_03B4_4764_8E28_7BA09985D2C1__INCLUDED_)
