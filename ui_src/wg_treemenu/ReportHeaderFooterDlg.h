#if !defined(__REPORTHEADERFOOTERDLG_H__)
#define __REPORTHEADERFOOTERDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportHeaderFooterDlg.h : header file
//

#include "wg_treemenuRes2.h"
#include "..\wg_cmd\BtnSTNew.h"

/////////////////////////////////////////////////////////////////////////////
// CReportHeaderFooterDlg dialog
class CDBDoc;
class CReportHeaderFooterDlg : public CDialogMove
{
// Construction
public:
	CReportHeaderFooterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CReportHeaderFooterDlg();

// Dialog Data
	//{{AFX_DATA(CReportHeaderFooterDlg)
	enum { IDD = IDD_TM_REPORT_HEADER_FOOTER_DLG };
	CListBox	m_listProj;
	CListBox	m_listHead;
	CListBox	m_listFoot;
	CButtonSTNew	m_btnHeadUp;
	CButtonSTNew	m_btnFootUp;
	CButtonSTNew	m_btnHeadDown;
	CButtonSTNew	m_btnFootDown;
	BOOL          m_bApply;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportHeaderFooterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetIcon();
	void InitHeadFootArray();
	void InitProjList();
	void InitHeadList();
	void InitFootList();
	BOOL IsSelectedHead(UINT nIndex);
	BOOL IsSelectedFoot(UINT nIndex);
	void DoSelectHeadItem();
	void DoSelectFootItem();
	void DoUnselectHeadItem();
	void DoUnselectFootItem();
	void DoAllSelectItem();
	void DoNoneSelectItem();
	void DoUpHeadItem();
	void DoDownHeadItem();
	void DoUpFootItem();
	void DoDownFootItem();
	BOOL Dlg2Data();

protected:
	CDBDoc* m_pDoc;
	CArray<UINT,UINT> m_aSelectedHead;
	CArray<UINT,UINT> m_aSelectedFoot;
	CStringArray      m_aAllProjStr; // index 맞추기 위해 1부터 시작.

// Implementation
protected:
	virtual void PostNcDestroy();

	// Generated message map functions
	//{{AFX_MSG(CReportHeaderFooterDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnHeadSelButton();
	afx_msg void OnHeadUnselButton();
	afx_msg void OnFootSelButton();
	afx_msg void OnFootUnselButton();
	afx_msg void OnHeadUpButton();
	afx_msg void OnHeadDownButton();
	afx_msg void OnFootUpButton();
	afx_msg void OnFootDownButton();
	afx_msg void OnProjectButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__REPORTHEADERFOOTERDLG_H__)

