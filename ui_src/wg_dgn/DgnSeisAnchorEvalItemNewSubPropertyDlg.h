#if !defined(__DgnSeisAnchorEvalItemNewSubPropertyDlg_H__)
#define __DgnSeisAnchorEvalItemNewSubPropertyDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchorEvalItemNewSubPropertyDlg.h : header file
//

//#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_ChildDialog.h"

#include "DgnSeisAnchorEvalItemNewDlg.h"
#include "DgnSeisAnchorPropertyGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItemNewSubPropertyDlg dialog
#include "HeaderPre.h"

class CDgnSeisAnchorEvalItemNewDlg;

class __MY_EXT_CLASS__ CDgnSeisAnchorEvalItemNewSubPropertyDlg : public CChildDialog
{
// Construction
public:
	CDgnSeisAnchorEvalItemNewSubPropertyDlg(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDgnSeisAnchorEvalItemNewSubPropertyDlg)
	enum { IDD = IDD_DGN_SEIS_ANCHOR_EVAL_ITEM_SUB_PROPERTY_DLG };
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisAnchorEvalItemNewSubPropertyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetPropertyData(T_ANEV_D* pData);
	void Data2Dlg();
	BOOL Dlg2Data();
	void SetViewData();

protected:

	CDgnSeisAnchorEvalItemNewDlg* m_pParent;

	CDgnSeisAnchorPropertyGrid* m_pGrid;

public:
	T_ANEV_D* m_pData;
	//T_ANEV_REG_SUPP_TYPE_D m_Data;

protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnSeisAnchorEvalItemNewSubPropertyDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnSeisAnchorEvalItemNewSubPropertyDlg_H__)
