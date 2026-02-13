#if !defined(__DgnSeisAnchorEvalItemNewSubSupportDlg_H__)
#define __DgnSeisAnchorEvalItemNewSubSupportDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchorEvalItemNewSubSupportDlg.h : header file
//

//#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_ChildDialog.h"
#include "DgnSeisAnchorEvalItemNewDlg.h"

#include "DgnSeisAnchorSupportGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItemNewSubSupportDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnSeisAnchorEvalItemNewSubSupportDlg : public CChildDialog
{
// Construction
public:
	CDgnSeisAnchorEvalItemNewSubSupportDlg(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDgnSeisAnchorEvalItemNewSubSupportDlg)
	enum { IDD = IDD_DGN_SEIS_ANCHOR_EVAL_ITEM_SUB_SUPPORT_DLG };
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisAnchorEvalItemNewSubSupportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetSupportData(T_ANEV_D* pData);
	void Data2Dlg();
	BOOL Dlg2Data();
	void SetViewData();

protected:
	CDgnSeisAnchorEvalItemNewDlg* m_pParent;

	CDgnSeisAnchorSupportGrid* m_pGrid;

public:
	T_ANEV_D* m_pData;
	//T_ANEV_REG_SUPP_TYPE_D m_Data;

protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnSeisAnchorEvalItemNewSubSupportDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnSeisAnchorEvalItemNewSubSupportDlg_H__)
