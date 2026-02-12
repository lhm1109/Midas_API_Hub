#if !defined(__CMSPECDLG_H__)
#define __CMSPECDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSpecDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxSpfc.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MCheckListBox.h"

#include "CMDlgBarBase.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMSpecDlg dialog

class CCMSpecGrid;

#include "HeaderPre.h"

struct T_FUNC_NAME
{
	T_SPFC_K Key;
	CString  strName;    
};

class __MY_EXT_CLASS__ CCMSpecDlg : public CCMDlgBarBase, public CDBUpdateConnector
{
// Construction
protected:
	CCMSpecDlg();       // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCMSpecDlg)

// Form Data
public:
	//{{AFX_DATA(CCMSpecDlg)
	enum { IDD = IDD_DBAR_SPLC_DLG };
	MCheckListBox	m_wndSpecFuncList;
	MButton	m_wndDampMethodBtn;
	CFormulaEdit m_edtModFact;
	CFormulaEdit m_wndScaleFactor;
	MEdit	m_wndLCname;
	MEdit	m_wndDesc;
	CFormulaEditSpin	m_wndAngleEdit;
	CEditUnit	m_wndCQCRatioEdit;
	CListCtrl	m_List;
	CTextUnit	m_wndAngleUnit;
	MComboBox	m_wndDirection;
	BOOL	m_bEccent;
	MButton	m_wndCorrDampChk ;
	MButton m_wndApplyDampingChk;
	MEdit m_wndDampingMethod;
	int m_nInterpolateType;
	int m_nAutoType;
	int m_nAngleType;
	//BOOL m_bAutoSearchAngle;
	MButton m_chkAuto;
	MButton m_chkND;
	CFormulaEdit m_wndNDEdit;
	//}}AFX_DATA
	CArray<UINT, UINT> m_arAngle;

// Attributes
public:
	CDBDoc* m_pDoc;

// Operations
public:
	virtual BOOL ExternalInit(UINT key);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSpecDlg)

protected:
	virtual void OnInitialUpdate();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	void UpdateUnit();
	virtual bool ShowApplyBtn() { return false; }

// Implementation
protected:
	~CCMSpecDlg();
	void EnableDampRatio(BOOL bEnable);
	void SetListCtrlHeader();
	BOOL GetSelectedSpecCase(int &nIndex);
	void MakeItemEx();
	CString DataToStr(int i, T_SPLC_D &data);
	void SetItem(int nIndex, T_SPLC_K key, T_SPLC_D &data);
	BOOL FindItem(T_SPLC_K key, int &nIndex);
	BOOL ValidSpecCase(T_SPLC_D &data);
	BOOL Dlg2SpecCase(T_SPLC_D &data);
	BOOL SpecCase2Dlg(T_SPLC_D &data);
	void SetDefault(T_SPLC_D &data);

	// Sub Dialog에서 받아오는 Data
	T_SPLC_D m_SubData;
	CString m_aDampMethod[3];
	
	void AlignControls();
	void EnableEccentricity(BOOL bEnable);
	void EnableAutoSearch(BOOL bEnable);
	void EnableAngleCQC();
	void EnableNonDissipative();

	void SetInitSpecFunc();
	void SetSpecFunc(CArray<UINT, UINT>& aSpfcKey);
	void GetSpecFunc(CArray<UINT, UINT>& aSpfcKey);
	void UpdateSpecFunc();
	void IsExistEigvComplex();

	CArray<T_FUNC_NAME, T_FUNC_NAME&> m_aFuncName; 

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CCMSpecDlg)
	virtual void OnTmClose() { OnDlgClose(); }
	afx_msg void OnDlgClose();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDelete();
	afx_msg void OnBtnModify();
	afx_msg void OnBtnCopy();
	afx_msg void OnChangeDirection();
	afx_msg void OnDeltaposCmdSpecAngleSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEigvDlg();
	afx_msg void OnSpfcDlg();
	afx_msg void OnChangedCurSpecCase(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdSpecSpctDlg();
	afx_msg void OnCmdSpecEccenChk();
	afx_msg void OnCmdSpecEccenMore();
	afx_msg void OnCmdSpecDampMethodBtn();
	afx_msg void OnCmdApplyDampingChk();
	afx_msg void OnClickCmdSpecFuncList();
	afx_msg void OnClickCmdSpecList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdSpecModeShapes();
	afx_msg void OnCmdAutoChk();
	afx_msg void OnCmdNonDissipativeChk();
	afx_msg void OnCmdAngleType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSPECDLG_H__)
