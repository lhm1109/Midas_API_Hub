#if !defined(AFX_IEHPYIELDPROPDLG_H__11173D29_1CDE_456A_846B_A57F74170A6E__INCLUDED_)
#define AFX_IEHPYIELDPROPDLG_H__11173D29_1CDE_456A_846B_A57F74170A6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IehpYieldPropDlg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\wg_base\wg_base_MySRGraph.h"
////////////////////////////////////////////////////////////////////////////
// CIehpYieldPropDlg dialog
class CIehpYieldPropGrid;

class CIehpYieldPropDlg : public CDialogMove
{
// Construction
public:
	CIehpYieldPropDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIehpYieldPropDlg();

	T_IEHP_YLDSUR m_Data;
	void SetIehpPropData(T_IEHP_YLDSUR data, CString str) { m_Data = data; m_strName = str; }
////////////////////////
	void UpdateChartY();
	void UpdateChartZ();
////////////////////////

// Dialog Data
	//{{AFX_DATA(CIehpYieldPropDlg)
	enum { IDD = IDD_ETC_IEHP_YLD_PROP };
	CEdit	m_edtAlpha;
	CTextUnit	m_untAxial;
	CEditUnit	m_edtAxial;
	int		m_nLinear;
	int		m_nYldUser;
	BOOL	m_bConsiderPM;
	CString	m_strBetaYplus;
	CString	m_strBetaYmnus;
	CString	m_strBetaZmnus;
	CString	m_strBetaZplus;
	CString	m_strGamma;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIehpYieldPropDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_strName;

	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL CheckErrorNameDataVar(LPCTSTR lpszData, LPCTSTR lpszName, LPCTSTR lpszVar);
	BOOL CheckDataCtrl();
	CArray<UINT, UINT> m_arYldUser;
	double m_dR2[11];

	///////////////////////////
	SREGraphView m_GraphViewY, m_GraphViewZ;
	CMySRGraph m_GraphY, m_GraphZ;
	void InitChartY();
	void InitChartZ();
	void SetValueY(int nIndex, int nGroup, double x,double y);
	void SetValueZ(int nIndex, int nGroup, double x,double y);
	///////////////////////////

	// Generated message map functions
	//{{AFX_MSG(CIehpYieldPropDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnIehpLinearRdo();
	virtual void OnOK();
	afx_msg void OnIehpYldCalc();
	afx_msg void OnChangeUserDirSpec();
	afx_msg void OnChangeUserDirSpecY();
	afx_msg void OnChangeUserDirSpecZ();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CIehpYieldPropGrid* m_wndGrid;
public:
	afx_msg void OnBnClickedYieldInputTypeUserRdo();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IEHPYIELDPROPDLG_H__11173D29_1CDE_456A_846B_A57F74170A6E__INCLUDED_)
