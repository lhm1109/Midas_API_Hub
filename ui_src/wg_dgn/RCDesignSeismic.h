// RCDesignSeismic.h: interface for the CRCDesignSeismic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCDESIGNSEISMIC_H__CB0C3EEF_41C2_43FF_A5EF_F172602EDB36__INCLUDED_)
#define AFX_RCDESIGNSEISMIC_H__CB0C3EEF_41C2_43FF_A5EF_F172602EDB36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_SelectCtrl.h"

class CDBDoc;

class CRCDesignSeismic : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CRCDesignSeismic(CWnd* pParent = NULL);
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CRCDesignSeismic)
	enum { IDD = IDD_DGN_RC_DESIGN_SEISMIC };
	CTextUnit m_PierHnUnit;
	CTextUnit m_PierAllowDispUnit;
	CTextUnit	m_FdnAGroundUnit;
	CTextUnit	m_CoppingSpanUnit;
	CTextUnit	m_RubSupThickUnit;
	CTextUnit	m_ActSupHDisUnit;
	CTextUnit	m_FixSupHLoadUnit;
	CEditUnit m_PierHnEdit;
	CEditUnit m_PierAllowDispEdit;
	CEditUnit	m_FdnAGroundEdit;	
	CEditUnit	m_CoppingSpanEdit;
	CEditUnit	m_RubSupThickEdit;
	CEditUnit	m_RubSupFricEdit;
	CEditUnit	m_ActSupHDisEdit;
	CEditUnit	m_FixSupHLoadEdit;

	CTextUnit m_SinglePierHnUnit;
	CEditUnit m_SinglePierHnEdit;
	CTextUnit m_PairPierHnUnit;
	CEditUnit m_PairPierHnEdit;
	CTextUnit m_PairPierAllowDispUnit;
	CEditUnit m_PairPierAllowDispEdit;

	CTextUnit m_PairPierBOTMAXUnit;
	CEditUnit m_PairPierBOTMAXEdit;
	CTextUnit m_PairPierBOTMINUnit;
	CEditUnit m_PairPierBOTMINEdit;
	CTextUnit m_PairPierTOPMAXUnit;
	CEditUnit m_PairPierTOPMAXEdit;
	CTextUnit m_PairPierTOPMINUnit;
	CEditUnit m_PairPierTOPMINEdit;



	CSelectEdit m_PierElemSelEdit;
	CSelectEdit m_CoppingElemSelEdit;
	CSelectEdit m_CoppingNodeSelEdit;

	CComboBox m_cmbMpcc;
	
	CListCtrl m_ElemList;
	CListCtrl m_ELinkList;
	CListCtrl m_GLinkList;

	int		m_nOption;	
	int		m_nMemberType;
	int		m_nCurMemberType;
	int   m_nPierHingeType;
	BOOL  m_bDoubleColumnPier;
	int   m_nElemOutType;
	int   m_nLinkOutType;

	CString m_strSelectElem;
	CString m_strSelectNode;

	CArray<T_RSDL_MPHI_D,T_RSDL_MPHI_D> m_arPierUserMPhi[2]; // Pier - »ç¿ëÀÚ ÀÔ·Â M-Phi °î¼± Á¤º¸
	double m_dPierYieldCurvature[2];   // Pier - »ç¿ëÀÚ ÀÔakr·Â Ç×º¹°î·ü
	double m_dPierUltimateCurvature[2];// Pier - »ç¿ëÀÚ ÀÔ·Â ±ØÇÑ°î·ü
	
	//}}AFX_DATA
	CArray<UINT, UINT> m_aPierID;
	CArray<UINT, UINT> m_aFoundationID;	
	CArray<UINT, UINT> m_aCoppingBeamID;	
	CArray<UINT, UINT> m_aRubSupportID;	
	CArray<UINT, UINT> m_aActSupportID;
	CArray<UINT, UINT> m_aFixSupportID;	
	CArray<UINT, UINT> m_aElemPrintID;
	CArray<UINT, UINT> m_aLinkPrintID;
	CArray<UINT, UINT> m_aElemListID;	
	CArray<UINT, UINT> m_aELinkListID;	
	CArray<UINT, UINT> m_aGLinkListID;
	CArray<UINT, UINT> m_aCtrlButtonID;

	CArray<UINT, UINT> m_aSinglePierListID;
	CArray<UINT, UINT> m_aSinglePierID;
	CArray<UINT, UINT> m_aPairPierListID;

	CArray<UINT, UINT> m_aMphiListID;
	CArray<UINT, UINT> m_aElemInputListID;

	CArray<UINT, UINT> m_aHingeListID;


	int m_iDgnCode;

public:
	void InitCtrl();
	void InitCtrl_CJJ();
	void Initial_SelectItem();
	void Initial_MpccCombo();
	void Initial_Data();
	void SetHeaderTitle();
	
	void ChangeMemberType();
	void ChangeMemberType_CJJ();
	void UpdateElemList(int nMemberType);
	void UpdateELinkList();
	void UpdateGLinkList();

	void Data2Dlg_Rsdm(T_RSDM_D& rData);
	void Data2Dlg_Rsdl(T_RSDL_D& rData);
public:
	int  GetSelectRsdlKey(int nMemberType, CArray<T_ELEM_K, T_ELEM_K>& arElemKey, CArray<T_RSDL_K, T_RSDL_K>& arRsdlKey);
	CString GetNumbericOptimizer(CArray<UINT, UINT>& KeyList);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRCDesignSeismic)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Implementation
protected:
	CDBDoc* m_pDoc;
	BOOL    m_bUpdateItemChanged;
	// Generated message map functions
	//{{AFX_MSG(CRCDesignSeismic)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeMemberType();	
	afx_msg void OnMPhiBtn();
	afx_msg void OnDoubleColumnPierChk();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnCRCRsdmAr();
	afx_msg void OnDgnCRCRsdmDel();

	afx_msg void OnItemchangedElemList(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnItemchangedELinkList(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnItemchangedGLinkList(NMHDR* pNMHDR, LRESULT* pResult);	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		void OnDgnExecute4CJJ();
};


#endif // !defined(AFX_RCDESIGNSEISMIC_H__CB0C3EEF_41C2_43FF_A5EF_F172602EDB36__INCLUDED_)
