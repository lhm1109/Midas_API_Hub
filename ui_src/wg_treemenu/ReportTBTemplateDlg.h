// ReportTBTemplateDlg.h: interface for the CReportTBTemplateDlg class.
//////////////////////////////////////////////////////////////////////

#if !defined(__REPORTTBTEMPLATE_H__)
#define __REPORTTBTEMPLATE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "wg_treemenuRes2.h"
#include "..\wg_base\wg_base_DialogMove.h"

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CReportTBTemplateDlg : public CDialogMove
{
// Construction
public:
	CReportTBTemplateDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CReportTBTemplateDlg();
	
// Dialog Data
	//{{AFX_DATA(CReportTBTemplateDlg)
	enum { IDD = IDD_TM_REPORT_TBTEMPLATE_DLG };
	CString   m_strName;
	//}}AFX_DATA
	CString   m_strFilePath;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportTBTemplateDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CReportTBTemplateDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void  Initialize();
	BOOL  Data2Dlg();
	void  Dlg2Data();
	void  SetXmlFilePath(CString strFilePath);
	void  SetData(UINT UtplK);

protected:
	BOOL    IsSymbol(LPCTSTR lpStr);
	CString DeleteSymbol(LPCTSTR lpStr);

protected:
	BOOL  m_bExteriorSet; // 외부에서 값을 세팅했을 경우 TRUE (workstree에서 불렀을때)
	CString m_strNameOld; // Workstree에서 Name을 변경하기 전의 이름
};
#include "HeaderPost.h"

#endif // !defined(__REPORTTBTEMPLATE_H__)
