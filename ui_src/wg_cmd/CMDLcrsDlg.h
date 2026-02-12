#if !defined(AFX_CMDLCRSDLG_H__94066C5B_AD13_4025_BC24_63955A781A44__INCLUDED_)
#define AFX_CMDLCRSDLG_H__94066C5B_AD13_4025_BC24_63955A781A44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMDLcrsDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\mit_frx\MCheckListBox.h"
/////////////////////////////////////////////////////////////////////////////
// CCMDLcrsDlg dialog

class CCMDLcrsDlg : public CDialogMove
{
// Construction
public:
	CCMDLcrsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMDLcrsDlg)
	enum { IDD = IDD_CMD_RESULT_LCRS };
	mit::frx::MCheckListBox	m_cklistLCRS;
	CButton m_btnRemove;
	CButton m_btnIdOk;
	//}}AFX_DATA

protected:
	void GetTotalLoadCase(T_SELECTEDLOAD_LIST &aLoadList);
	void MakeLoadList();
	BOOL Dlg2Data();
	void Data2Dlg();
//CString GetLoadCaseName(CString sLoadName,UINT nType);
	//void DataExchange();
	//void InitLoadData();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMDLcrsDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	T_LCRS_D m_LoadAttr;
	T_SELECTEDLOAD_LIST m_aLoadList;

	CArray<UINT,UINT> m_arLType;

	// Generated message map functions
	//{{AFX_MSG(CCMDLcrsDlg)
		virtual BOOL OnInitDialog();
		afx_msg void OnOk();
		afx_msg void OnRemove();  
	  virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMDLCRSDLG_H__94066C5B_AD13_4025_BC24_63955A781A44__INCLUDED_)
