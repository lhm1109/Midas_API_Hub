#if !defined(__CMGRIDMAINGIRDERITEMDLG_H__)
#define __CMGRIDMAINGIRDERITEMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMGridMainGirderItemDlg.h : header file
//
#include "CMDlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_cobxgrup.h"
#include "..\wg_base\wg_base_MouseEdit.h"

#include "CMGXGridWnd.h"
/////////////////////////////////////////////////////////////////////////////
// CCMGXMainGirderGridWnd grid
class CCMGXMainGirderGridWnd : public CCMGXGridWnd2
{
public:
	CCMGXMainGirderGridWnd();
	virtual ~CCMGXMainGirderGridWnd() {}

protected:
	BOOL m_bStartModify;
	CString m_strOld;

public:
	BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol);
	BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
};

/////////////////////////////////////////////////////////////////////////////
// CCMGridMainGirderItemDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMGridMainGirderItemDlg : public CCMDlgBase
{
// Construction
public:
	CCMGridMainGirderItemDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	void UpdateUnit();

	void SetModify(BOOL bModify);
	void SetInitModify(T_MGDR_K key);
	BOOL GridCellChanged(ROWCOL nRow, ROWCOL nCol);

	BOOL ExternalSetting(T_MGDR_K MgdrKey);

// Dialog Data
	//{{AFX_DATA(CCMGridMainGirderItemDlg)
	enum { IDD = IDD_CMD_GRID_MNGD_ITEM };
	CTextUnit	m_2PointUnit2;
	CTextUnit	m_2PointUnit1;
	MEdit	m_wndNodeSelectEdit;
	CMouseEdit	m_wnd2PointsEdit1;
	CMouseEdit	m_wnd2PointsEdit2;
	CMouseEdit	m_wndPickingEdit;
	MEdit	m_wndLaneName;
	CListCtrl	m_List;
	int		m_nSelectionMethod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMGridMainGirderItemDlg)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CCMGXMainGirderGridWnd m_wndGrid;
	T_MGDR_D m_Data;
	BOOL m_bModify;

	CArray<UINT, UINT> m_aCtrlSelect;
	CString m_csOldLaneName;
	void InitGrid();
	void SetGridTitle();
	void InitItemDataVar();
	void SelectMethodMan();
	void Data2Dlg();
	BOOL Dlg2Data();
	void InsertBaseItem(int nPos, int nType);
	void SelectNode();
	BOOL Grid_IsExistKey(int nKey);
	void Grid_InsertItem(int nPos, T_MGDR_BASE &item);
	BOOL OnExecute();
	void InitDlg();
	void ButtonCtrl();
	CString CreateGirderName();

	// Generated message map functions
	//{{AFX_MSG(CCMGridMainGirderItemDlg)
	afx_msg void OnCmdOpAdd();
	afx_msg void OnCmdOpInsert();
	afx_msg void OnCmdOpDelete();
	afx_msg void OnCmdChangeSelectBy();
	afx_msg void OnCmdBtnOk();
	afx_msg void OnCmdBtnCancel();
	afx_msg void OnCmdBtnApply();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMGRIDMAINGIRDERITEMDLG_H__)
