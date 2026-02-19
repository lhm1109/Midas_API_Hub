#if !defined(AFX_DGNCONSTRUTANGLEDLG_H__)
#define AFX_DGNCONSTRUTANGLEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConStrutAngleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDgnConTRFTDlg dialog
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;

class CDgnConStrutAngleDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnConStrutAngleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Initial_SelectItem();
	void Initial_Data();
	void Update_InitDataByCode();

// Dialog Data
	//{{AFX_DATA(CDgnConTRFTDlg)
	enum { IDD = IDD_DGN_CON_STRUT_ANGLE_DLG };
	int		m_nOption;
	double	m_dStrutAngle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConTRFTDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnStrutAngleAr();
	afx_msg void OnDgnStrutAngleDel();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnConTRFTDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONSTRUTANGLEDLG_H__)
