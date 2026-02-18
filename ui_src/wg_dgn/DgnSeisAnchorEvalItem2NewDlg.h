#if !defined(__DgnSeisAnchorEvalItem2NewDlg_H__)
#define __DgnSeisAnchorEvalItem2NewDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchorEvalItem2NewDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "DgnMPhiCurveWnd.h"
#include "DgnSeisAnchorEvalEditBtn.h"
#include "DgnSeisAnchorEvalGuide2Dlg.h"

#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MEdit.h"

class CDgnSeisAnchorEvalItem2NewSubArrangeDlg;
class CDgnSeisAnchorEvalItem2NewSubPropertyDlg;
class CDgnSeisAnchorEvalItem2NewSubSupportDlg;
class CDgnSeisAnchorEvalItem2NewSubTensionDlg;
/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItem2NewDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnSeisAnchorEvalItem2NewDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CDgnSeisAnchorEvalItem2NewDlg(CWnd* pParent = NULL, BOOL bAdd=FALSE);   // standard constructor
	~CDgnSeisAnchorEvalItem2NewDlg();
	
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

	void OnCloseGuideDlg();
public:
// Dialog Data
	//{{AFX_DATA(CDgnSeisAnchorEvalItem2NewDlg)
	enum { IDD = IDD_DGN_SEIS_ANCHOR_EVAL_ITEM2_NEW_DLG };
	CDlgTabCtrl	 m_tabEval;

	mit::frx::MEdit     m_edtName;
	mit::frx::MEdit     m_edtDesc;
	mit::frx::MComboBox m_cmbGroup;
	CEditUnit m_edtfck;
	CTextUnit m_untfck;
	CEditUnit m_edtHcop;
	CTextUnit m_untHcop;
	mit::frx::MComboBox m_cmbProp;

	BOOL m_bAdd;
	BOOL      m_bRebar;
	BOOL      m_bShearFactor;
	BOOL      m_bPryout;
	BOOL      m_bPryoutRow;
	BOOL      m_bTension;
	//int       m_nViewType;
	BOOL      m_bDrawOKEval;
	CDgnSeisAnchorEvalEditBtn m_View;

	CDgnSeisAnchorEvalItem2NewSubArrangeDlg*  m_pArrange;
	CDgnSeisAnchorEvalItem2NewSubPropertyDlg* m_pProperty;
	CDgnSeisAnchorEvalItem2NewSubSupportDlg*  m_pSupport;
	CDgnSeisAnchorEvalItem2NewSubTensionDlg*  m_pTension;

	CDgnSeisAnchorEvalGuide2Dlg* m_pGuideDlg;

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisAnchorEvalItem2NewDlg)
protected:
virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//}}AFX_VIRTUAL

// Implementation
protected:
	void InitTabControl(BOOL bTension=FALSE);
	void InitUnit();
	void InitCombo();
	void InitView();	

	void EnableDisableControls();

	void Data2Dlg(BOOL bChangEval=FALSE);
	BOOL Dlg2Data();
	BOOL DataCheck();
	void UpdateName();
	BOOL ApplyData();

	void Calcfck();
	void CalcHcop();
	
	CDBDoc* m_pDoc;
	
	T_ANEV_K m_Key;
	T_ANEV_D m_Data;
	T_ANEV_D m_DataPre;

	BOOL m_bModify;
	BOOL m_bSetEmf;
	CString m_strPath;

	int  m_nType;
	int  m_nAbutPos;
	BOOL m_IsGuideView;

	// Generated message map functions
	//{{AFX_MSG(CDgnSeisAnchorEvalItem2NewDlg)
	virtual BOOL OnInitDialog();

	afx_msg void OnSeisAnchorEvalGroupCmb();
	afx_msg void OnSeisAnchorEvalGroupBtn();
	afx_msg void OnSeisAnchorEvalPropCmb();
	afx_msg void OnSeisAnchorEvalPropBtn();
	afx_msg void OnSeisAnchorEvalGuideBtn();
	afx_msg void OnSeisAnchorEvalPropHcopEdt();
	
	afx_msg void OnSeisAnchorEvalTensionChk();
	afx_msg void OnSeisAnchorEvalRebarChk();	
	afx_msg void OnSeisAnchorEvalRebarBtn();
	afx_msg void OnSeisAnchorEvalShearFactorChk();
	afx_msg void OnSeisAnchorEvalShearFactorBtn();
	afx_msg void OnSeisAnchorEvalFailureBtn();
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

#endif // !defined(__DgnSeisAnchorEvalItem2NewDlg_H__)
