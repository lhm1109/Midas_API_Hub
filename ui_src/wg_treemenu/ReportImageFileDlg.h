// ReportImageFileDlg.h: interface for the CReportImageFileDlg class.
//////////////////////////////////////////////////////////////////////

#if !defined(__REPORTIMAGEFILEDLG_H__)
#define __REPORTIMAGEFILEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "wg_treemenuRes2.h"
#include "..\wg_base\wg_base_DialogMove.h"

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CReportImageFileDlg : public CDialogMove
{
// Construction
public:
	CReportImageFileDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CReportImageFileDlg();
	
// Dialog Data
	//{{AFX_DATA(CReportImageFileDlg)
	enum { IDD = IDD_TM_REPORT_IMAGEFILE_DLG };
	CString   m_strFilePath;
	CListBox	m_List;
	//}}AFX_DATA

protected:
	CString m_strImageFilePath;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportImageFileDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CReportImageFileDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void  Initialize();
	BOOL  Data2Dlg();
	void  Dlg2Data();
	void  MakeFileItemList();
	
protected:
	BOOL    IsSymbol(LPCTSTR lpStr);
	CString DeleteSymbol(LPCTSTR lpStr);
};
#include "HeaderPost.h"

#endif // !defined(__REPORTIMAGEFILEDLG_H__)
