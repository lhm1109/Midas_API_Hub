#if !defined(__CMDAUTOLOADCOMBCVLCSAPAGE_H__)
#define __CMDAUTOLOADCOMBCVLCSAPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdAutoLoadCombCvlCSAPage.h : header file
//
#include "..\wg_base\FoldGroupBox.h"
#include "..\wg_db\wg_db_EditUnit.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlCSAPage dialog

class CCmdAutoLoadCombCvlDlg;
class CCmdAutoLoadCombCvlCSAPage : public CChildDialog
{
// Construction
public:
	CCmdAutoLoadCombCvlCSAPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCmdAutoLoadCombCvlCSAPage)
	enum { IDD = IDD_CMD_LCOMBDEFAULT_CSA_PAGE };

	int m_nCSAD;
	int m_nCSADW;
	int m_nCSAEP1;
	int m_nCSAEP2;
	int m_nCSAEP3;
	int m_nCSAEP4;
	int m_nCSAWP;
	int m_nCSAPS;

	CFoldGroupBox_Cross	m_chkLoadFactor;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdAutoLoadCombCvlCSAPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void GetCSALoadFactor();
	void OnCmdCSAEnableDisable();
	void ChangeText(int nDesignType);
    int m_nConstLoad;

protected:
	CDBDoc* m_pDoc;
	CCmdAutoLoadCombCvlDlg* m_pParent;

	int m_nDesignType; // 0:Steel, 1:Concrete, 2:SRC, 3:Footing

	// Generated message map functions
	//{{AFX_MSG(CCmdAutoLoadCombCvlCSAPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdCSAEPBtn();

	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	int m_nCSADNum;
	int m_nCSADWNum;
	int m_nCSAEP1Num;
	int m_nCSAEP2Num;
	int m_nCSAEP3Num;
	int m_nCSAEP4Num;
	int m_nCSAWPNum;
	int m_nCSAPSNum;
	double m_dCSADMax;
	double m_dCSADMin;
	double m_dCSADWMax;
	double m_dCSADWMin;
	double m_dCSAEP1Max;
	double m_dCSAEP1Min;
	double m_dCSAEP2Max;
	double m_dCSAEP2Min;
	double m_dCSAEP3Max;
	double m_dCSAEP3Min;
	double m_dCSAEP4Max;
	double m_dCSAEP4Min;
	double m_dCSAWPMax;
	double m_dCSAWPMin;
	double m_dCSAPSMax;
	double m_dCSAPSMin;

	CArray<T_STLD_K, T_STLD_K> m_aStldEp1;
	CArray<T_STLD_K, T_STLD_K> m_aStldEp2;
	CArray<T_STLD_K, T_STLD_K> m_aStldEp3;
	CArray<T_STLD_K, T_STLD_K> m_aStldEp4;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMDAUTOLOADCOMBCVLCSAPAGE_H__)
