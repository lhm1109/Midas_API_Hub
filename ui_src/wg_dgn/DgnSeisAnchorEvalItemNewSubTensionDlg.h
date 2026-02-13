#if !defined(__DgnSeisAnchorEvalItemNewSubTensionDlg_H__)
#define __DgnSeisAnchorEvalItemNewSubTensionDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchorEvalItemNewSubTensionDlg.h : header file
//

//#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_ChildDialog.h"

#include "DgnSeisAnchorTensionGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItemNewSubTensionDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnSeisAnchorEvalItemNewSubTensionDlg : public CChildDialog
{
// Construction
public:
	CDgnSeisAnchorEvalItemNewSubTensionDlg(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDgnSeisAnchorEvalItemNewSubTensionDlg)
	enum { IDD = IDD_DGN_SEIS_ANCHOR_EVAL_ITEM_SUB_TENSION_DLG };
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisAnchorEvalItemNewSubTensionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetTensionData(T_ANEV_D* pData);
	void Data2Dlg();
	BOOL Dlg2Data();

protected:

	CDgnSeisAnchorTensionGrid* m_pGrid;

public:
	T_ANEV_D* m_pData;

protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnSeisAnchorEvalItemNewSubTensionDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnSeisAnchorEvalItemNewSubTensionDlg_H__)
