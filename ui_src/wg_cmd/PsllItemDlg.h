#if !defined(__PSLLITEMDLG_H__)
#define __PSLLITEMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MEdit.h"
using namespace mit::frx;
class CPsllItemCodeMgr;
class CWindSeisAddLoadGrid;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CPsllItemDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CPsllItemDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPsllItemDlg();

//----------------------------------------------------------------------
// Overrides
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_PSLL_K Key, T_PSLL_D &ParamData); 
	void AddAdditionalLoad(T_STOR_K StartStorKey, T_STOR_K EndStorKey, double dblX, double dblY, double dblRZ);

// Dialog Data
	//{{AFX_DATA(CPsllItemDlg)
	enum { IDD = IDD_ETC_PSLL_ITEM };
	CSelectLC	m_wndLoadCase;
	MComboBox	m_wndCodeName;
	MEdit	    m_wndDescription;
	CEditUnit	m_wndScaleGY;
	CEditUnit	m_wndScaleGX;
	CBCGPStatic	  m_wndHolder;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPsllItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void CreateChildDlgs();
	void DeleteChildDlgs();
	void ShowCurChildDlg(int nDlgIndex);

	void InitCodeNameCobx();
	void Data2Dlg();
	BOOL Dlg2Data();

	void MakeItemEx();
/*  CString DataToStr(int i, T_PSLL_ADDITION& data);*/

	void AdjustComboListBox();

protected:
	BOOL m_bModify;
	CDBDoc* m_pDoc;
	T_PSLL_K m_Key, m_OldKey;
	T_PSLL_D m_Data;

	CPsllItemCodeMgr* m_pCodeMgr;
	CWindSeisAddLoadGrid* m_pGridWnd;

	CArray<UINT, UINT> m_aMoveCtrlID;
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPsllItemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdApply();
	virtual void OnCancel();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnCalcSheet();
	afx_msg void OnCmdLoadcaseBtn();
	afx_msg void OnCmdMethodBtn();
	afx_msg void OnSelchangeCmdCodeName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__PSLLITEMDLG_H__)
