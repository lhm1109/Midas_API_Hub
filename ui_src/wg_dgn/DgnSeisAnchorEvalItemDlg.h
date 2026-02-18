#if !defined(__DGNSEISANCHOREVALITEMDLG_H__)
#define __DGNSEISANCHOREVALITEMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchorEvalItemDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "DgnMPhiCurveWnd.h"
#include "DgnSeisAnchorEvalEditBtn.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItemDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnSeisAnchorEvalItemDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CDgnSeisAnchorEvalItemDlg(CWnd* pParent = NULL);   // standard constructor
	
// 	//----------------------------------------------------------------------
// 	// Interface
// public:
// 	void SetParamData(T_ANEV_K Key, T_ANEV_D &Data) 
// 	{ m_Key = Key; m_Data = Data; m_bModify = TRUE; }
// 	void MakeEmfAndClose(CString strPath);
// 
// 	//----------------------------------------------------------------------
// 	// Overrides
// 	//----------------------------------------------------------------------
// public:
// 	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
// 	virtual void UpdateBuffer();

public:
// Dialog Data
	//{{AFX_DATA(CDgnSeisAnchorEvalItemDlg)
	enum { IDD = IDD_DGN_SEIS_ANCHOR_EVAL_ITEM_DLG };
// 
// 	CEdit     m_edtName;
// 	CComboBox m_cmbGroup;
// 	CEditUnit m_edtfck;
// 	CTextUnit m_untfck;
// 	CEditUnit m_edtHcop;
// 	CTextUnit m_untHcop;
// 	CComboBox m_cmbProp;
// 	CEditUnit m_edtNum;
// 	CEditUnit m_edtB;
// 	CTextUnit m_untB;
// 	CEditUnit m_edtH;
// 	CTextUnit m_untH;
// 	CEditUnit m_edtTheta1;
// 	CTextUnit m_untTheta1;
// 	CEditUnit m_edtTheta2;
// 	CTextUnit m_untTheta2;
// 	CEditUnit m_edtThetaq1;
// 	CTextUnit m_untThetaq1;
// 	CEditUnit m_edtThetaq2;
// 	CTextUnit m_untThetaq2;
// 	CEditUnit m_edtD1;
// 	CTextUnit m_untD1;
// 	CEditUnit m_edtD1q;
// 	CTextUnit m_untD1q;
// 	CEditUnit m_edtN1;  // Read Only
// 	CString   m_strSax; //
// 	CTextUnit m_untSax; //
// 	CEditUnit m_edtD2;
// 	CTextUnit m_untD2;
// 	CEditUnit m_edtD2q;
// 	CTextUnit m_untD2q;
// 	CEditUnit m_edtN2;  // Read Only
// 	CString   m_strSay; //
// 	CTextUnit m_untSay; //
// 	BOOL      m_bRebar;
// 	BOOL      m_bSupport;
// 	int       m_nViewType;
// 	CDgnSeisAnchorEvalEditBtn m_View;
// 	//}}AFX_DATA
// 
// // Overrides
// 	// ClassWizard generated virtual function overrides
// 	//{{AFX_VIRTUAL(CDgnSeisAnchorEvalItemDlg)
// 	protected:
// 	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
// 	//}}AFX_VIRTUAL
// 
// // Implementation
// protected:
// 	void InitUnit();
// 	void InitCombo();
// 	void InitView();	
// 
// 	void AlignControl();
// 	void ControlsShowHide();
// 	void EnableDisableControls();
// 
// 	void SetViewData();
// 	void Data2Dlg();	
// 	BOOL Dlg2Data();
// 
// 	void UpdateName();
// 
// 	void Calcfck();
// 	void CalcHcop();
// 	void CalcNum();
// 	void CalcB();
// 	void CalcH();
// 	void CalcN1();
// 	void CalcN2();
// 	
// 	CDBDoc* m_pDoc;
// 	
// 	T_ANEV_K m_Key;
// 	T_ANEV_D m_Data;
// 
// 	BOOL m_bModify;
// 	BOOL m_bSetEmf;
// 	CString m_strPath;

// 	// Generated message map functions
// 	//{{AFX_MSG(CDgnSeisAnchorEvalItemDlg)
// 	virtual BOOL OnInitDialog();
// 	afx_msg void OnSeisAnchorEvalGroupCmb();
// 	afx_msg void OnSeisAnchorEvalGroupBtn();
// 	afx_msg void OnSeisAnchorEvalPropCmb();
// 	afx_msg void OnSeisAnchorEvalPropBtn();
// 	afx_msg void OnSeisAnchorEvalPropHcopEdt();
// 	afx_msg void OnSeisAnchorEvalPropTheta1Edt();
// 	afx_msg void OnSeisAnchorEvalPropTheta2Edt();
// 	afx_msg void OnSeisAnchorEvalPropTheta1QEdt();
// 	afx_msg void OnSeisAnchorEvalPropTheta2QEdt();
// 	afx_msg void OnSeisAnchorEvalPropD1Edt();
// 	afx_msg void OnSeisAnchorEvalPropD1_qEdt();
// 	afx_msg void OnSeisAnchorEvalPropSaxEdt();
// 	afx_msg void OnSeisAnchorEvalPropD2Edt();
// 	afx_msg void OnSeisAnchorEvalPropD2_qEdt();
// 	afx_msg void OnSeisAnchorEvalPropSayEdt();
// 	afx_msg void OnSeisAnchorEvalRebarChk();
// 	afx_msg void OnSeisAnchorEvalRebarBtn();
// 	afx_msg void OnSeisAnchorEvalSupportChk();
// 	afx_msg void OnSeisAnchorEvalSupportBtn();
// 	afx_msg void OnSeisAnchorEvalFailureBtn();
// 	afx_msg void OnSeisAnchorEvalDrawRdo();
// 	afx_msg void OnSeisAnchorEvalGuideRdo();
// 	afx_msg void OnSeisAnchorEvalOKBtn();
// 	afx_msg void OnSeisAnchorEvalCanCelBtn();
// 	afx_msg void OnSeisAnchorEvalApplyBtn();
// 	//}}AFX_MSG
// 	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNSEISANCHOREVALITEMDLG_H__)
