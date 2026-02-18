#if !defined(AFX_DGNGENLRDFCASEDLG_H__05FB06A3_E381_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNGENLRDFCASEDLG_H__05FB06A3_E381_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenLrdfCaseDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "DgnStruct.h"

#include "..\mit_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"	
#include "..\mit_frx\MComboBox.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnGenLrdfCaseDlg dialog

class CDgnGenLrdfCaseDlg : public CDialogMove
{
// Construction
public:
	CDgnGenLrdfCaseDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnGenLrdfCaseDlg)
	enum { IDD = IDD_DGN_GEN_LRDF_CASE_DLG };
	CListCtrl	m_SelListCtrl;
	CListCtrl	m_ListCtrl;
	//}}AFX_DATA

public:
	CString m_LoadCaseNames;
	CArray<_DGN_REDU,_DGN_REDU&> m_LiveData;
	CArray<_DGN_REDU,_DGN_REDU&> m_SelLiveData;

public:
	_DGN_REDU Search_LiveLoadData(CString name);

	BOOL IsEQLoadCase(CString strno);

	void Initial_SelLoadCase(CString strLoadNo);
	void Initial_LoadCase();
	void Initial_ListCtrlBox();
	void Initial_SelListCtrlBox();
	void Write_ListCtrlBox(int Index,CString strNo,CString strName,CString strType,CString strDes);
	void Write_SelListCtrlBox(int Index,CString strNo,CString strName,CString strType,CString strDes);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenLrdfCaseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnGenLrdfCaseDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkDgnReducliveList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDgnReducliveSel();
	afx_msg void OnDgnReducselDel();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNGENLRDFCASEDLG_H__05FB06A3_E381_11D3_888F_0000C0F30D4D__INCLUDED_)
