#if !defined(__CMGRIDMAINGIRDERLISTDLG_H__)
#define __CMGRIDMAINGIRDERLISTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMGridMainGirderListDlg.h : header file
//

#include "CMDlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_cobxgrup.h"
#include "..\wg_base\wg_base_MouseEdit.h"

#include "CMDlgBase.h"

#include "CMGXGridList.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCMGXMgdrList
class CCMGXMgdrList : public CCMGXGridList
{
public:
	CCMGXMgdrList() {}
	virtual ~CCMGXMgdrList() {}

public:
	virtual void MakeItemList();
	virtual void ModifySerialID(CArray<CString, CString&> &aName, CArray<UINT, UINT> &aSerial);
	virtual BOOL OnLButtonDblClkRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);

	void ConvertDataToStr(T_MGDR_D &Data, CStringArray &aValue);
	CString DataToStr(T_MGDR_D &Data);
};

/////////////////////////////////////////////////////////////////////////////
// CCMGridMainGirderListDlg dialog
#include "HeaderPre.h"
//class CCMGridMainGirderItemDlg;
class __MY_EXT_CLASS__ CCMGridMainGirderListDlg : public CCMDlgBase
{
// Construction
public:
	CCMGridMainGirderListDlg(CWnd* pParent = NULL);   // standard constructor
public:
	void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	void OnLButtonDblClkOnGrid(UINT key);
	//CCMGridMainGirderItemDlg* m_pWndChild;

// Dialog Data
	//{{AFX_DATA(CCMGridMainGirderListDlg)
	enum { IDD = IDD_CMD_GRID_MNGD_LIST };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMGridMainGirderListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CCMGXMgdrList m_wndGrid;
	
	// Generated message map functions
	//{{AFX_MSG(CCMGridMainGirderListDlg)
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnClose();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMGRIDMAINGIRDERLISTDLG_H__)
