#if !defined(__CMLOADPLCBDLG_H__)
#define      __CMLOADPLCBDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadPLCBDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DialogMove.h"

/////////////////////////////////////////////////////////////////////////////
// CCMLoadPLCBDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMLoadPLCBDlg : public CDialogMove
{
// Construction
public:
	CCMLoadPLCBDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMLoadPLCBDlg)
	enum { IDD = IDD_CMD_LOAD_PLCB_DLG };
	CListBox	m_lstSelLCase;
	CListBox	m_lstLCase;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadPLCBDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	
	T_PLCB_K m_PlcbKey;
	T_PLCB_D m_PlcbData;
	T_STLD_D m_StldData;
	T_STLD_K m_StldKey;

	void InitStldData();
	void CtrlButtonOK();

	// Generated message map functions
	//{{AFX_MSG(CCMLoadPLCBDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkCmdPlcbLoadList();
	afx_msg void OnDblclkCmdPlcbSelloadList();
	afx_msg void OnCmdPlcbSelectBtn();
	afx_msg void OnCmdPlcbRemoveBtn();
	afx_msg void OnCmdPlcbRemBtn();
	afx_msg void OnCmdPlcbOk();
	afx_msg void OnCmdPlcbCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMLOADPLCBDLG_H__D50D1701_7380_11D4_9AAF_0000C0B9C58C__INCLUDED_)
