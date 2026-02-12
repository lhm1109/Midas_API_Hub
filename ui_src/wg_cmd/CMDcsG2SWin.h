#if !defined(AFX_CMDCSG2SWIN_H__6C340321_0268_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CMDCSG2SWIN_H__6C340321_0268_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMDcsG2SWin.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
/////////////////////////////////////////////////////////////////////////////
// CCMDcsG2SWin dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMDcsG2SWin : public CDialogMove
{
// Construction
public:
	CCMDcsG2SWin(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMDcsG2SWin)
	enum { IDD = IDD_DCS_GEN2SDS_WIN };
	CEdit	m_edtFileName;
	CComboBox	m_cboStory;
	CEdit	m_edtStiffnessFactor;
	int		m_object;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMDcsG2SWin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	
	void InitCtrlData();
	void InitStorData();

	// Generated message map functions
	//{{AFX_MSG(CCMDcsG2SWin)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdG2swinFileBtn();
	afx_msg void OnCmdG2swinOk();
	afx_msg void OnCmdG2swinCancel();
	afx_msg void OnSelectObj();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMDCSG2SWIN_H__6C340321_0268_11D5_880F_00010263A1F4__INCLUDED_)
