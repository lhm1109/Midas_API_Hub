#if !defined(__CMLOADLINELISTDLG_H__)
#define __CMLOADLINELISTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadLineListDlg.h : header file
//

#include "CMDlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_cobxgrup.h"
#include "..\wg_base\wg_base_MouseEdit.h"

#include "CMGXLdlnGridList.h"
/////////////////////////////////////////////////////////////////////////////
// CCMLoadLineListDlg dialog
#include "HeaderPre.h"
//class CCMLoadLineItemDlg;
class __MY_EXT_CLASS__ CCMLoadLineListDlg : public CCMDlgBase
{
// Construction
public:
	CCMLoadLineListDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Overrides
public:
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	void UpdateUnit();
	void OnLButtonDblClkOnGrid(UINT key);
	//CCMLoadLineItemDlg* m_pWndChild;

// Dialog Data
	//{{AFX_DATA(CCMLoadLineListDlg)
	enum { IDD = IDD_CMD_GRID_LOADLINE_LIST };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadLineListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CCMGXLdlnGridList m_wndGrid;

	void InitGrid();

	// Generated message map functions
	//{{AFX_MSG(CCMLoadLineListDlg)
	afx_msg void OnCmdAdd();
	afx_msg void OnCmdModify();
	afx_msg void OnCmdDelete();
	afx_msg void OnCmdCopy();
	afx_msg void OnCmdImport();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMLOADLINELISTDLG_H__)
