#if !defined(__DgnSeisAnchorEvalItem2NewSubSupportDlg_H__)
#define __DgnSeisAnchorEvalItem2NewSubSupportDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchorEvalItem2NewSubSupportDlg.h : header file
//

//#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_ChildDialog.h"

#include "DgnSeisAnchorEvalItem2NewDlg.h"
#include "DgnSeisAnchor2SupportGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItem2NewSubSupportDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnSeisAnchorEvalItem2NewSubSupportDlg : public CChildDialog
{
// Construction
public:
	CDgnSeisAnchorEvalItem2NewSubSupportDlg(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDgnSeisAnchorEvalItem2NewSubSupportDlg)
	enum { IDD = IDD_DGN_SEIS_ANCHOR_EVAL_ITEM2_SUB_SUPPORT_DLG };
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisAnchorEvalItem2NewSubSupportDlg)
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
	CDgnSeisAnchorEvalItem2NewDlg* m_pParent;

	CDgnSeisAnchor2SupportGrid* m_pGrid;

public:
	T_ANEV_D* m_pData;
	//T_ANEV_REG_SUPP_TYPE_D m_Data;

protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnSeisAnchorEvalItem2NewSubSupportDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnSeisAnchorEvalItem2NewSubSupportDlg_H__)
