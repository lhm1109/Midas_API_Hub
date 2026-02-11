#if !defined(AFX_THGC_GLOBALCTRL_WALL_OPTION_DLG_H)
#define AFX_THGC_GLOBALCTRL_WALL_OPTION_DLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\\wg_base\DlgChild.h"  

class CMThgcWallOptionDlg : public CDlgChild
{
	// Construction
public:
	CMThgcWallOptionDlg(CWnd* pParent = NULL);   // standard constructor
	~CMThgcWallOptionDlg();

	// Dialog Data
	enum { IDD = IDD_ETC_THGC_WALL_DLG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPO_GlobalCtrl_WallNodeConnectivityDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		// Implementation
public:
	void   SetData(BOOL bchkWallStiffnessConvert);
	void   GetData(BOOL& bchkWallStiffnessConvert) const;
	CRect  m_rectPosition;

protected:
	void MoveDlg();
	void Data2Dlg();
	void Dlg2Data();
	BOOL m_chkWallStiffnessConvert; // GEN 7660 Wall Stiffness Scale Factor Convert
};

#endif // !defined(AFX_THGC_GLOBALCTRL_WALL_OPTION_DLG_H)
