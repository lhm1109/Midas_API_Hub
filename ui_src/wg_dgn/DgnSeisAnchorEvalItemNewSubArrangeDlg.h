#if !defined(__DgnSeisAnchorEvalItemNewSubArrangeDlg_H__)
#define __DgnSeisAnchorEvalItemNewSubArrangeDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchorEvalItemNewSubArrangeDlg.h : header file
//

//#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_ChildDialog.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "DgnSeisAnchorEvalItemNewDlg.h"
#include "DgnSeisAnchorSupportGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItemNewSubArrangeDlg dialog
#include "HeaderPre.h"

class CDgnSeisAnchorEvalItemNewDlg;

class __MY_EXT_CLASS__ CDgnSeisAnchorEvalItemNewSubArrangeDlg : public CChildDialog
{
// Construction
public:
	CDgnSeisAnchorEvalItemNewSubArrangeDlg(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDgnSeisAnchorEvalItemNewSubArrangeDlg)
	enum { IDD = IDD_DGN_SEIS_ANCHOR_EVAL_ITEM_SUB_ARRANGE_DLG };
	
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
	CEditUnit m_edtThetaq1;
	CTextUnit m_untThetaq1;
	CEditUnit m_edtThetaq2;
	CTextUnit m_untThetaq2;
	CEditUnit m_edtD1;
	CTextUnit m_untD1;
	CEditUnit m_edtD1q;
	CTextUnit m_untD1q;
	CString   m_strSax; //
	CTextUnit m_untSax; //
	CEditUnit m_edtD2;
	CTextUnit m_untD2;
	CEditUnit m_edtD2q;
	CTextUnit m_untD2q;
	CString   m_strSay; //
	CTextUnit m_untSay; //


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisAnchorEvalItemNewSubArrangeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetArrangeData(T_ANEV_D* pData);
	void Data2Dlg();
	BOOL Dlg2Data(BOOL bIgnoreUpdateData=FALSE);

protected:
	void InitUnit();
	void EnableCtrl();
	void CalRelAngle();

	void CalcB();
	void CalcH();

	T_ANEV_D* m_pData;
	CDBDoc* m_pDoc;

	BOOL      m_bRelAngle;

	CDgnSeisAnchorEvalItemNewDlg* m_pParent;

public:
	//T_ANEV_REG_SUPP_TYPE_D m_Data;

protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnSeisAnchorEvalItemNewSubArrangeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSeisAnchorEvalPropRelAngle();
	afx_msg void OnSeisAnchorEvalPropTheta1Edt();
	afx_msg void OnSeisAnchorEvalPropTheta2Edt();
	afx_msg void OnSeisAnchorEvalPropTheta1QEdt();
	afx_msg void OnSeisAnchorEvalPropTheta2QEdt();
	afx_msg void OnSeisAnchorEvalPropD1Edt();
	afx_msg void OnSeisAnchorEvalPropD1_qEdt();
	afx_msg void OnSeisAnchorEvalPropSaxEdt();
	afx_msg void OnSeisAnchorEvalPropD2Edt();
	afx_msg void OnSeisAnchorEvalPropD2_qEdt();
	afx_msg void OnSeisAnchorEvalPropSayEdt();

	afx_msg void OnSeisAnchorEvalRelAngleChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnSeisAnchorEvalItemNewSubArrangeDlg_H__)
