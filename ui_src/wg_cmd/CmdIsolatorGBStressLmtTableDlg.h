
#include "..\wg_db\wg_db_DBDoc.h"
#include "CmdIsolatorGBStressLmtGridWnd.h"
/////////////////////////////////////////////////////////////////////////////
// CCmdIsolatorGBStressLmtTableDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCmdIsolatorGBStressLmtTableDlg : public CDialogMove
{
// Construction
public:
	CCmdIsolatorGBStressLmtTableDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCmdIsolatorGBStressLmtTableDlg();

	CCmdIsolatorGBStressLmtGridWnd	m_wndGrid;
	CEdit m_Classify;
	int m_nType;
//----------------------------------------------------------------------
// Overrides
//----------------------------------------------------------------------
public:
	//void OnLButtonDblClkOnGrid(T_ASGB_D &data);
	BOOL SetParamData(T_ISGB_D *pParamData);
	BOOL GetParamData(T_ISGB_D *pParamData);
//----------------------------------------------------------------------
// Implementation

protected:
	CDBDoc* m_pDoc;
	T_ISGB_D *m_pData;
// Dialog Data
	//{{AFX_DATA(CCmdIsolatorGBStressLmtTableDlg)
	enum {IDD = IDD_CMD_LOAD_ISOLATOR_GB_LMT_DLG};
 	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdIsolatorGBStressLmtTableDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCmdIsolatorGBStressLmtTableDlg)
 	afx_msg void OnCmdBtnOK();
	afx_msg void OnCmdBtnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
