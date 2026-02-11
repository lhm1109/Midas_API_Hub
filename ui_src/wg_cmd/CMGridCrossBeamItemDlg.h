#if !defined(__CMGRIDCROSSBEAMITEMDLG_H__)
#define __CMGRIDCROSSBEAMITEMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMGridCrossBeamItemDlg.h : header file
//
#include "CMDlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_cobxgrup.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\wg_base\wg_base_MouseEdit.h"

#include "..\MIT_frx\MButton.h"
using namespace mit::frx;

#include "CMGXGridWnd.h"
/////////////////////////////////////////////////////////////////////////////
// CCMGXMainGirderGridWnd grid
class CCMGXCrossBeamGridWnd : public CCMGXGridWnd2
{
public:
	CCMGXCrossBeamGridWnd();
	virtual ~CCMGXCrossBeamGridWnd() {}

protected:
	BOOL m_bStartModify;
	CString m_strOld;

public:
	BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol);
	BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
};


/////////////////////////////////////////////////////////////////////////////
// CCMGridCrossBeamItemDlg dialog
#include "HeaderPre.h"
class CCMGridCrossBeamItemDlg : public CCMDlgBase
{
// Construction
public:
	CCMGridCrossBeamItemDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	void UpdateUnit();

	void SetModify(BOOL bModify);
	void SetInitModify(T_CBEM_K key);
	BOOL GridCellChanged(ROWCOL nRow, ROWCOL nCol);

	BOOL ExternalSetting(T_CBEM_K CbemKey);

// Dialog Data
	//{{AFX_DATA(CCMGridCrossBeamItemDlg)
	enum { IDD = IDD_CMD_GRID_CBEM_ITEM };
	CFormulaEditSpin	m_edtCopyTimes;
	CFormulaEditSpin	m_edtCopyNodeinc;
	//CSpinButtonCtrl	m_spnCopyNodeinc;
	//CSpinButtonCtrl	m_spnCopyTimes;
	MButton	m_chkCopy;
	MEdit	m_edtDist;
	CTextUnit	m_unitCopy;
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
	//{{AFX_VIRTUAL(CCMGridCrossBeamItemDlg)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CCMGXCrossBeamGridWnd m_wndGrid;
	T_CBEM_D m_Data;
	BOOL m_bModify;

	CArray<UINT, UINT> m_aCtrlSelect;
//----- add by jkpark 2005.9.9 start -----//
	CArray<UINT, UINT> m_aCtrlCopy;
	CArray<UINT, UINT> m_aCtrlCopyDist;
	CArray<UINT, UINT> m_aCtrlCopyNodeInc;
	CArray<UINT, UINT> m_aCtrlCopyType;
	CArray<UINT, UINT> m_aCtrlAxis;
//----- add by jkpark 2005.9.9 end -----//

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
	void Grid_InsertItem(int nPos, T_CBEM_BASE &item);
	BOOL OnExecute();
	void InitDlg();
	void ButtonCtrl();
	CString CreateGirderName();

//----- add by jkpark 2005.9.9 start -----//
	void AlignControl();
	void SetCopyRdo(int nCopyType);
//----- add by jkpark 2005.9.9 end -----//

	// Generated message map functions
	//{{AFX_MSG(CCMGridCrossBeamItemDlg)
	afx_msg void OnCmdOpAdd();
	afx_msg void OnCmdOpInsert();
	afx_msg void OnCmdOpDelete();
	afx_msg void OnCmdChangeSelectBy();
	afx_msg void OnCmdBtnOk();
	afx_msg void OnCmdBtnCancel();
	afx_msg void OnCmdBtnApply();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdCbemCopyChk();
	afx_msg void OnCmdCbemCopyRdo();
	//}}AFX_MSG
	afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMGRIDCROSSBEAMITEMDLG_H__)
