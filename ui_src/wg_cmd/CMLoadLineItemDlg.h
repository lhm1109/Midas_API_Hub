#if !defined(__CMLOADLINEITEMDLG_H__)
#define __CMLOADLINEITEMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadLineItemDlg.h : header file
//

#include "CMDlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_cobxgrup.h"
#include "..\wg_base\wg_base_MouseEdit.h"

//#include "CMGXGridWnd.h"
#include "CMGirdLdlnGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CCMLoadLineItemDlg dialog
#include "HeaderPre.h"
class CCMLoadLineItemDlg : public CCMDlgBase
{
// Construction
public:
	CCMLoadLineItemDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL ExternalSetting(T_LDLN_K LdlnKey);

public:
	void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	void UpdateUnit();
	void SetItemData();
	void InitDlg();
	void SetItemData(CString& strName);
	void SetModify(BOOL bModify);
	void SetInitModify(T_LDLN_K key);
// Dialog Data
	//{{AFX_DATA(CCMLoadLineItemDlg)
	enum { IDD = IDD_CMD_GRID_LOADLINE_ITEM };
	MEdit	m_wndName;
	CMouseEdit	m_wndPointEdit;
	CTextUnit	m_wndPointUnit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadLineItemDlg)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CCMGridLdlnGrid m_wndGrid;
	//CCMGXGridWnd m_wndGrid;
	T_LDLN_D m_Data;
	BOOL m_bModify;
	BOOL m_bFromTree;
	CString m_csOldName;
	void InitGrid();
	void SetGridTitle();
	void InsertBaseItem(int nPos);
	BOOL IsExistPoint(double dX, double dY, double dZ);
	void InsertGridItem(int nPos, T_LDLN_BASE& item);
	BOOL Execute();
	void InitItemDataVar();
	void Data2Dlg();
	BOOL Dlg2Data();
	CString CreateGirderName();

	// Generated message map functions
	//{{AFX_MSG(CCMLoadLineItemDlg)
	afx_msg void OnCmdOpAdd();
	afx_msg void OnCmdOpInsert();
	afx_msg void OnCmdOpDelete();
	afx_msg void OnCmdBtnOk();
	afx_msg void OnCmdBtnCancel();
	afx_msg void OnCmdBtnApply();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMLOADLINEITEMDLG_H__F642C98D_558F_4E65_8F39_A79FAC72306E__INCLUDED_)
