#if !defined(AFX_CMMVHCITEMDLG_H__BA9D23AD_21D8_11D4_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMMVHCITEMDLG_H__BA9D23AD_21D8_11D4_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhcItemDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

#include "..\MIT_frx\MEdit.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMMvhcItemDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvhcItemDlg : public CDialogMove
{
// Construction
public:
	CCMMvhcItemDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_MVHC_D &ParamData) 
	{ m_csOldName = ParamData.VehicleClassName; m_Data = ParamData; m_bModify = TRUE; }

//----------------------------------------------------------------------
// Implementation
protected:
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL ApplyOrOK();

protected:
	CDBDoc*   m_pDoc;
	T_MVHC_D  m_Data;
	CString   m_csOldName;
	BOOL      m_bModify;
	CArray<T_MVHL_K, T_MVHL_K> m_aMvhlList;
	CArray<BOOL, BOOL> m_aSelFlag;

public:
// Dialog Data
	//{{AFX_DATA(CCMMvhcItemDlg)
	enum { IDD = IDD_CMD_ML_MVHC_ITEM };
	MEdit	m_wndName;
	CListBox	m_wndUnselList;
	CListBox	m_wndSelList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhcItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMMvhcItemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnDelete();
	virtual void OnOK();
	afx_msg void OnCmdApply();
	afx_msg void OnDblclkCmdMvhcSelList();
	afx_msg void OnDblclkCmdMvhcUnselList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVHCITEMDLG_H__BA9D23AD_21D8_11D4_92DE_0000C0B0E6B3__INCLUDED_)
