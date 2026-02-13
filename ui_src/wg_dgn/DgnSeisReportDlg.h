#if !defined(AFX_DGNSEISREPORTDLG_H__)
#define AFX_DGNSEISREPORTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDgnSeisReportDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\MIT_frx\MButton.h"

#include "HeaderPre.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnSaveReportoExcelWithEMFDlg dialog

class __MY_EXT_CLASS__ CDgnSeisReportDlg : public CDialogMove
{
    // Construction
public:
    CDgnSeisReportDlg(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDgnSeisReportDlg)
    enum { IDD = IDD_DGN_SEIS_REPORT_DLG };
    BOOL	m_bItems[8];
    mit::frx::MButton m_BtnItems[8];
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDgnSeisReportDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL	

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDgnSeisReportDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnPrintReportBtn();
    afx_msg void OnSelectAllBtn();
    afx_msg void OnUnselectAllBtn();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    BOOL m_bExistRes[2];
};


#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNSEISREPORTDLG_H__)
