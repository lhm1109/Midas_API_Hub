#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMDTendonAreaDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgChild.h"
#include "CMDlgBase.h"
#include "..\MIT_frx\McomboBox.h"
/////////////////////////////////////////////////////////////////////////////
// CCMDTendonAreaDlg dialog

class CDBDoc;
class CCMDRefwdKDS2016Dlg : public CDialogMove, public CDBUpdateConnector
{
	// Construction
public:
	CCMDRefwdKDS2016Dlg(CWnd* pParent = NULL);   // standard constructor
	// Dialog Data
		//{{AFX_DATA(CCMDTendonAreaDlg)
	enum { IDD = IDD_CMD_TDMT_KDS2016_WD};
	mit::frx::MComboBox	m_wndMatComb;
	double m_dDensity;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMDTendonAreaDlg)
protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CDBDoc* m_pDoc;
	//T_MATL_K  m_MatlKey;
	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMDTendonAreaDlg)
	afx_msg void OnCmdTdmtKSD2016MatlCmb();
	afx_msg void OnCmdTdmtKSD2016MatlBtn();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	void InitCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


