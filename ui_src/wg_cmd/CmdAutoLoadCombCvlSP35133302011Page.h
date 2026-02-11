#if !defined(__CMDAUTOLOADCOMBCVLSP35133302011PAGE_H__)
#define __CMDAUTOLOADCOMBCVLSP35133302011PAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdAutoLoadCombCvlSP35133302011Page.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlSP35133302011Page dialog

class CCmdAutoLoadCombCvlDlg;
class CCmdAutoLoadCombCvlSP35133302011Page : public CChildDialog
{
// Construction
public:
	CCmdAutoLoadCombCvlSP35133302011Page(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCmdAutoLoadCombCvlSP35133302011Page)
	enum { IDD = IDD_CMD_LCOMBDEFAULT_SP35_13330_2011_PAGE };

	MComboBox m_cmbBridgeType;
	int       m_nSP3511D;
	CEditUnit m_edtSP3511D_Max;
	CEditUnit m_edtSP3511D_Min;
	int       m_nSP3511LIP;
	CEditUnit m_edtSP3511LIP_Max;
	CEditUnit m_edtSP3511LIP_Min;
	int       m_nSP3511PL;
	CEditUnit m_edtSP3511PL_Max;
	CEditUnit m_edtSP3511PL_Min;
	int       m_nSP3511BL;
	CEditUnit m_edtSP3511BL_Max;
	CEditUnit m_edtSP3511BL_Min;
	int       m_nSP3511PS;
	CEditUnit m_edtSP3511PS_Max;
	CEditUnit m_edtSP3511PS_Min;
	int       m_nSP3511EP;
	CEditUnit m_edtSP3511EP_Max;
	CEditUnit m_edtSP3511EP_Min;
	int       m_nSP3511WP;
	CEditUnit m_edtSP3511WP_Max;
	CEditUnit m_edtSP3511WP_Min;
	int       m_nSP3511CRSH;
	CEditUnit m_edtSP3511CRSH_Max;
	CEditUnit m_edtSP3511CRSH_Min;
	int       m_nSP3511STL;
	CEditUnit m_edtSP3511STL_Max;
	CEditUnit m_edtSP3511STL_Min;

	CFoldGroupBox_Cross	m_chkLoadFactor;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdAutoLoadCombCvlSP35133302011Page)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void InitCombo();
	void SetDefault();
	void GetSP3511LoadFactor();
	void OnCmdSP3511EnableDisable();

	int m_nConstLoad;

protected:
	CDBDoc* m_pDoc;
	CCmdAutoLoadCombCvlDlg* m_pParent;

	// Generated message map functions
	//{{AFX_MSG(CCmdAutoLoadCombCvlSP35133302011Page)
	virtual BOOL OnInitDialog();

	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	int m_nBridgeType;

	int m_nSP3511DNum;
	int m_nSP3511LIPNum;
	int m_nSP3511PLNum;
	int m_nSP3511BLNum;
	int m_nSP3511PSNum;
	int m_nSP3511EPNum;
	int m_nSP3511WPNum;
	int m_nSP3511CRSHNum;
	int m_nSP3511STLNum;

	double m_dSP3511DMax;
	double m_dSP3511DMin;
	double m_dSP3511LIPMax;
	double m_dSP3511LIPMin;
	double m_dSP3511PLMax;
	double m_dSP3511PLMin;
	double m_dSP3511BLMax;
	double m_dSP3511BLMin;
	double m_dSP3511PSMax;
	double m_dSP3511PSMin;
	double m_dSP3511EPMax;
	double m_dSP3511EPMin;
	double m_dSP3511WPMax;
	double m_dSP3511WPMin;
	double m_dSP3511CRSHMax;
	double m_dSP3511CRSHMin;
	double m_dSP3511STLMax;
	double m_dSP3511STLMin;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMDAUTOLOADCOMBCVLSP35133302011PAGE_H__)
