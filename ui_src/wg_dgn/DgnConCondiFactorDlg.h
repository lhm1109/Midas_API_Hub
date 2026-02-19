#if !defined(__DGNCONCONDIFACTORDLG_H__)
#define __DGNCONCONDIFACTORDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConCondiFactorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDgnConCondiFactorDlg dialog
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

class CDBDoc;

class CDgnConCondiFactorDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnConCondiFactorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

	void InitUnit();
	void EnableDisableControl();

// Dialog Data
	//{{AFX_DATA(CDgnConCondiFactorDlg)
	enum { IDD = IDD_DGN_CON_CONDI_FACTOR_DLG };

	int		    m_nOption;
	BOOL      m_bmb6;
	int       m_nPosmb6; // 현재 안쓰기로 함
	int       m_nmb6Type;
	CEditUnit m_edtSigby;
	CTextUnit m_untSigby;
	CEditUnit m_edtmb6;
	CEditUnit m_edtmb6_s;
	BOOL      m_bmb10;
	int       m_nPosmb10;
	int       m_nmb10Type;
	CEditUnit m_edtThick;
	CTextUnit m_untThick;
	CEditUnit m_edtRbj;
	CTextUnit m_untRbj;
	CEditUnit m_edtmb10;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConCondiFactorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnCondiFactorAddDelRdo();
	afx_msg void OnDgnCondiFactormb6Chk();
	afx_msg void OnDgnCondiFactormb6Rdo();
	afx_msg void OnDgnCondiFactormb10Chk();
	afx_msg void OnDgnCondiFactormb10Rdo();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnConCondiFactorDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNCONCONDIFACTORDLG_H__)
