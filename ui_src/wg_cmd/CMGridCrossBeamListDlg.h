#if !defined(__CMGRIDCROSSBEAMLISTDLG_H__)
#define __CMGRIDCROSSBEAMLISTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMGridCrossBeamListDlg.h : header file
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
class CCMGXCbemList : public CCMGXGridList
{
public:
	CCMGXCbemList() {}
	virtual ~CCMGXCbemList() {}

public:
	virtual void MakeItemList();
	virtual void ModifySerialID(CArray<CString, CString&> &aName, CArray<UINT, UINT> &aSerial);
	virtual BOOL OnLButtonDblClkRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);

	void ConvertDataToStr(T_CBEM_D &Data, CStringArray &aValue);
	CString DataToStr(T_CBEM_D &Data);
};

/////////////////////////////////////////////////////////////////////////////
// CCMGridCrossBeamListDlg dialog
#include "HeaderPre.h"
//class CCMGridCrossBeamItemDlg;
class CCMGridCrossBeamListDlg : public CCMDlgBase
{
// Construction
public:
	CCMGridCrossBeamListDlg(CWnd* pParent = NULL);   // standard constructor
public:
	void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	void OnLButtonDblClkOnGrid(UINT key);
	//CCMGridCrossBeamItemDlg* m_pWndChild;

// Dialog Data
	//{{AFX_DATA(CCMGridCrossBeamListDlg)
	enum { IDD = IDD_CMD_GRID_CBEM_LIST };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMGridCrossBeamListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CCMGXCbemList m_wndGrid;

	// Generated message map functions
	//{{AFX_MSG(CCMGridCrossBeamListDlg)
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

#endif // !defined(__CMGRIDCROSSBEAMLISTDLG_H__)
