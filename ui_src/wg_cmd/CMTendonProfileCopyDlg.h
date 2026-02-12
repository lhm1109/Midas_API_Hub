#if !defined(AFX_CMTENDONPROFILECOPYDLG_H__8920FD46_6225_4DA4_89DA_4274F813056C__INCLUDED_)
#define AFX_CMTENDONPROFILECOPYDLG_H__8920FD46_6225_4DA4_89DA_4274F813056C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMTendonProfileCopyDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_CobxTdnt.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\wg_base\wg_base_MySRGraph.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "CMDlgBase.h"
#include "TdnaElemGrid.h"
#include "..\MIT_frx\MButton.h"	
/////////////////////////////////////////////////////////////////////////////
// CCMTendonProfileCopyDlg dialog
using namespace mit::frx;

class  CCMTendonProfileCopyDlg : public CCMDlgBase, public CDBUpdateConnector
{
// Construction
public:
	CCMTendonProfileCopyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMTendonProfileCopyDlg)
	enum { IDD = IDD_CMD_DEF_TENDON_PROFILE_COPY_DLG };
	CFormulaEditSpin	m_ElemEdit;
// 	CSpinButtonCtrl	m_IncreSpin;
	CMouseEdit	m_VectorEdit;
	CTextUnit	m_VectorUnit;
	CMouseEdit	m_wndInsertion;
	CSelectEdit	m_wndAssignElem;
	MButton	m_OptionRDO;
	MButton m_wndAutoChk;
	MButton m_wndCurrentChk;
	int		m_nOption;
	int		m_nMode;
	//}}AFX_DATA
 
	CArray<UINT,UINT> m_arIncre,m_arEqual,m_arNew,m_arOption,m_arCopyMove;
	CArray<T_TDNA_ELEM_D,T_TDNA_ELEM_D&> m_aElems;
	T_TDNA_ELEM_D m_Data;
	CWnd* m_pParentDlg;
	
public:
	//void OnChangeCurrentFbld(T_FBLD_K nKey);
	//void UpdateBuffer();
	//void UnitChanged();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMTendonProfileCopyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
	//virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

// Implementation

protected:
	CDBDoc* m_pDoc;
	CTdnaElemGrid m_wndGrid;

	// Generated message map functions
	//{{AFX_MSG(CCMTendonProfileCopyDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDlgClose();
	afx_msg void OnCmdElemAdd();
	afx_msg void OnCmdElemDelete();
	afx_msg void OnCmdElemModify();
	afx_msg void OnChangeLoadCase();
	afx_msg void OnCmdTendonCopyRadioElem();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	BOOL Dlg2Data();
	BOOL Data2Dlg();
	void InitGrid();
	void UnselectAll();
	BOOL DoApply();
	void EnableControls();
public:
	void OnChangeCurrentElem(int key);
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTENDONPROFILECOPYDLG_H__8920FD46_6225_4DA4_89DA_4274F813056C__INCLUDED_)
