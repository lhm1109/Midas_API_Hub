#if !defined(__DgnSeisAnchorEvalItem2NewSubTensionDlg_H__)
#define __DgnSeisAnchorEvalItem2NewSubTensionDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchorEvalItem2NewSubTensionDlg.h : header file
//

//#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_ChildDialog.h"

#include "DgnSeisAnchorEvalItem2NewDlg.h"
#include "DgnSeisAnchor2TensionGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItem2NewSubTensionDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnSeisAnchorEvalItem2NewSubTensionDlg : public CChildDialog
{
// Construction
public:
	CDgnSeisAnchorEvalItem2NewSubTensionDlg(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDgnSeisAnchorEvalItem2NewSubTensionDlg)
	enum { IDD = IDD_DGN_SEIS_ANCHOR_EVAL_ITEM2_SUB_TENSION_DLG };
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisAnchorEvalItem2NewSubTensionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetTensionData(T_ANEV_D* pData);
	void Data2Dlg();
	BOOL Dlg2Data();

protected:
	CDgnSeisAnchorEvalItem2NewDlg* m_pParent;

	CDgnSeisAnchor2TensionGrid* m_pGrid;

public:
	T_ANEV_D* m_pData;

protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnSeisAnchorEvalItem2NewSubTensionDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnSeisAnchorEvalItem2NewSubTensionDlg_H__)
