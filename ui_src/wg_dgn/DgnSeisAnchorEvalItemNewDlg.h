#if !defined(__DgnSeisAnchorEvalItemNewDlg_H__)
#define __DgnSeisAnchorEvalItemNewDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchorEvalItemNewDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "DgnMPhiCurveWnd.h"
#include "DgnSeisAnchorEvalEditBtn.h"
#include "DgnSeisAnchorEvalGuideDlg.h"

class CDgnSeisAnchorEvalItemNewSubArrangeDlg;
class CDgnSeisAnchorEvalItemNewSubPropertyDlg;
class CDgnSeisAnchorEvalItemNewSubSupportDlg;
class CDgnSeisAnchorEvalItemNewSubTensionDlg;
/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItemNewDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnSeisAnchorEvalItemNewDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CDgnSeisAnchorEvalItemNewDlg(CWnd* pParent = NULL, BOOL bAdd=FALSE);   // standard constructor
	
	//----------------------------------------------------------------------
	// Interface
public:
	void SetParamData(T_ANEV_K Key, T_ANEV_D &Data);
	void MakeEmfAndClose(CString strPath);

	//----------------------------------------------------------------------
	// Overrides
	//----------------------------------------------------------------------
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	void SetViewData();
	void CalcN1();
	void CalcN2();

	void OnCloseGuideDlg();
public:
// Dialog Data
	//{{AFX_DATA(CDgnSeisAnchorEvalItemNewDlg)
	enum { IDD = IDD_DGN_SEIS_ANCHOR_EVAL_ITEM_NEW_DLG };
	CDlgTabCtrl	 m_tabEval;

	CEdit     m_edtName;
	CEdit     m_edtDesc;
	CComboBox m_cmbGroup;
	CEditUnit m_edtfck;
	CTextUnit m_untfck;
	CEditUnit m_edtHcop;
	CTextUnit m_untHcop;
	CComboBox m_cmbProp;
	CEditUnit m_edtNum;
	CEditUnit m_edtN1;  // Read Only
	CEditUnit m_edtN2;  // Read Only

	BOOL m_bAdd;
	BOOL      m_bRebar;
	//BOOL      m_bSupport;
	BOOL      m_bTension;
	BOOL      m_bShowPreview;	
	int       m_nViewType;
	BOOL      m_bDrawOKEval;
	CDgnSeisAnchorEvalEditBtn m_View;

	CDgnSeisAnchorEvalItemNewSubArrangeDlg*  m_pArrange;
	CDgnSeisAnchorEvalItemNewSubPropertyDlg* m_pProperty;
	CDgnSeisAnchorEvalItemNewSubSupportDlg*  m_pSupport;
	CDgnSeisAnchorEvalItemNewSubTensionDlg*  m_pTension;

	CDgnSeisAnchorEvalGuideDlg m_GuideDlg;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisAnchorEvalItemNewDlg)
protected:
virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//}}AFX_VIRTUAL

// Implementation
protected:
	void InitTabControl(BOOL bTension=FALSE);
	void InitUnit();
	void InitCombo();
	void InitView();	

	void AlignControl();
	void ControlsShowHide();
	void EnableDisableControls();
	void InitGuideDlg();

	void Data2Dlg(BOOL bChangEval=FALSE);
	BOOL Dlg2Data();
	BOOL DataCheck();
	void UpdateName();

	void Calcfck();
	void CalcHcop();
	void CalcNum();
	
	CDBDoc* m_pDoc;
	
	T_ANEV_K m_Key;
	T_ANEV_D m_Data;
	T_ANEV_D m_DataPre;

	BOOL m_bModify;
	BOOL m_bSetEmf;
	CString m_strPath;

	// Generated message map functions
	//{{AFX_MSG(CDgnSeisAnchorEvalItemNewDlg)
	virtual BOOL OnInitDialog();

	afx_msg void OnSeisAnchorEvalGroupCmb();
	afx_msg void OnSeisAnchorEvalGroupBtn();
	afx_msg void OnSeisAnchorEvalPropCmb();
	afx_msg void OnSeisAnchorEvalPropBtn();
	afx_msg void OnSeisAnchorEvalPropHcopEdt();

	afx_msg void OnSeisAnchorEvalTensionChk();
	afx_msg void OnSeisAnchorEvalRebarChk();	
	afx_msg void OnSeisAnchorEvalShowPreview();
	afx_msg void OnSeisAnchorEvalRebarBtn();
	afx_msg void OnSeisAnchorEvalFailureBtn();
	afx_msg void OnSeisAnchorEvalDrawRdo();
	afx_msg void OnSeisAnchorEvalGuideRdo();
	afx_msg void OnSeisAnchorEvalOKBtn();
	afx_msg void OnSeisAnchorEvalCanCelBtn();
	afx_msg void OnSeisAnchorEvalApplyBtn();

	afx_msg void OnEvaluationChangTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEvaluationChangedTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnSeisAnchorEvalItemNewDlg_H__)
