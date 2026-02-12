#if !defined(AFX_CMMVLDITEMFRNEWDLG_H__INCLUDED_)
#define AFX_CMMVLDITEMFRNEWDLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldIteFRNewDlg.h : header file
//

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"

#include "..\MIT_frx\MEdit.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemFRDlg dialog
#include "HeaderPre.h"

#define COLCOUNT 4

class __MY_EXT_CLASS__ CCMMvldItemFRDlg : public CDialogMove
{
// Construction
public:
	CCMMvldItemFRDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_MVLDfr_D &ParamData) 
	{ m_csOldName = ParamData.LoadCaseName; m_Data = ParamData; m_bModify = TRUE;}
	BOOL OnApplySubDlg(BOOL bModify, T_MVLDfr_BASE& data, int nPos);

//----------------------------------------------------------------------
// Implementation
public:
	void SetHeaderTitle();
	void Data2Dlg();
	BOOL Dlg2Data();

	void MakeItemEx();
	BOOL InsertItem(int nIndex, T_MVLDfr_BASE &Data);
	BOOL DeleteItem(int nIndex, T_MVLDfr_BASE &Data);
	BOOL ModifyItem(int nIndex, T_MVLDfr_BASE &Data);

	CString DataToStr(int i, T_MVLDfr_BASE &Data);
	CString GetLaneName(UINT nLaneKey);
	BOOL ApplyOrOK();

	CDBDoc*   m_pDoc;

	T_MVLDfr_D  m_Data;
	CString     m_csOldName;
	BOOL        m_bModify;
	BOOL        m_bSurface;

	int m_nPreLoadModel;
public:
// Dialog Data
	//{{AFX_DATA(CCMMvldItemFRDlg)
	enum { IDD = IDD_CMD_ML_MVLD_ITEM_FRANCE };
	
	MEdit	    m_wndLoadCase;
	MEdit	    m_wndDesc;
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldItemFRDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<UINT, UINT> m_aLoadModelType;

	// Generated message map functions
	//{{AFX_MSG(CCMMvldItemFRDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdApply();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdLoadModelRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVLDITEMFRNEWDLG_H__INCLUDED_)
