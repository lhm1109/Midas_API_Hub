#if !defined(__DgnSeisAnchorEvalItem2NewSubArrangeDlg_H__)
#define __DgnSeisAnchorEvalItem2NewSubArrangeDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchorEvalItem2NewSubArrangeDlg.h : header file
//

//#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_ChildDialog.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "DgnSeisAnchorEvalItem2NewDlg.h"
#include "DgnSeisAnchor2ArrangeGrid.h"
#include "DgnSeisAnchorEvalGuide2Dlg.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItem2NewSubArrangeDlg dialog
#include "HeaderPre.h"

class CDgnSeisAnchorEvalItem2NewDlg;

class __MY_EXT_CLASS__ CDgnSeisAnchorEvalItem2NewSubArrangeDlg : public CChildDialog
{
// Construction
public:
	CDgnSeisAnchorEvalItem2NewSubArrangeDlg(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDgnSeisAnchorEvalItem2NewSubArrangeDlg)
	enum { IDD = IDD_DGN_SEIS_ANCHOR_EVAL_ITEM2_SUB_ARRANGE_DLG };
	
	//}}AFX_DATA

	CEditUnit m_edtB;
	CTextUnit m_untB;
	CEditUnit m_edtH;
	CTextUnit m_untH;
	CEditUnit m_edtRelAngle;
	CTextUnit m_untRelAngle;
	CEditUnit m_edtTheta1;
	CTextUnit m_untTheta1;
	CEditUnit m_edtTheta2;
	CTextUnit m_untTheta2;
	CEditUnit m_edtThetaCoping;
	CTextUnit m_untThetaCoping;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisAnchorEvalItem2NewSubArrangeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetArrangeData(T_ANEV_D* pData);
	void Data2Dlg();
	BOOL Dlg2Data(BOOL bIgnoreUpdateData=FALSE);
	void SetViewData();
	void ModifyLayerData();
	void OnCloseGuideDlg();

protected:

	CDgnSeisAnchor2ArrangeGrid* m_pGrid;

protected:
	void InitUnit();
	void EnableCtrl();
	void CalRelAngle();

	T_ANEV_D  m_AnevD;
	T_ANEV_D* m_pData;
	CDBDoc* m_pDoc;

	BOOL      m_bRelAngle;
	BOOL      m_IsGuideView;

	CDgnSeisAnchorEvalItem2NewDlg* m_pParent;

	CDgnSeisAnchorEvalGuide2Dlg* m_pGuideDlg;

public:
	//T_ANEV_REG_SUPP_TYPE_D m_Data;

protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnSeisAnchorEvalItem2NewSubArrangeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSeisAnchorEvalPropRelAngle();
	afx_msg void OnSeisAnchorEvalPropTheta1Edt();
	afx_msg void OnSeisAnchorEvalPropTheta2Edt();
	afx_msg void OnSeisAnchorEvalPropBEdt();
	afx_msg void OnSeisAnchorEvalPropHEdt();
	afx_msg void OnSeisAnchorEvalPropThetaCopingEdt();
	afx_msg void OnSeisAnchorEvalRelAngleChk();
	afx_msg void OnSeisAnchorEvalGuideBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnSeisAnchorEvalItem2NewSubArrangeDlg_H__)
