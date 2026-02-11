#if !defined(AFX_CMRPOSITEMDLG_H__31202A53_4932_4ACA_9AB1_F8481E3FC71F__INCLUDED_)
#define AFX_CMRPOSITEMDLG_H__31202A53_4932_4ACA_9AB1_F8481E3FC71F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMRposItemDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCMRposItemDlg dialog

class CCMRposItemDlg : public CDialogMove
{
// Construction
public:
	CCMRposItemDlg(CWnd* pParent = NULL);   // standard constructor

public:
	void SetParamData(T_RPOS_D& data);

// Dialog Data
	//{{AFX_DATA(CCMRposItemDlg)
	enum { IDD = IDD_CMD_RPOS_ITEM };
	CEdit	m_editDesc;
	CEdit	m_editName;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMRposItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitStagData();
	void InitGrid();
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL DoApply();

protected:
	CDBDoc* m_pDoc;

	BOOL m_bSetParam;
	T_RPOS_D m_ParamData;
	CString m_csOldName;

	CArray<T_STAG_K, T_STAG_K> m_aStagKey;
	CMap<T_STAG_K, T_STAG_K, int, int> m_mapStag2Index;

	CGXGridWnd m_wndGrid;

	// Generated message map functions
	//{{AFX_MSG(CCMRposItemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdBtnApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMRPOSITEMDLG_H__31202A53_4932_4ACA_9AB1_F8481E3FC71F__INCLUDED_)
