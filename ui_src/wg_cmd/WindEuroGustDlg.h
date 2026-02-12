#if !defined(AFX_WINDEUROGUSTDLG_H__161493AD_292C_4D23_91EA_75B071E32000__INCLUDED_)
#define AFX_WINDEUROGUSTDLG_H__161493AD_292C_4D23_91EA_75B071E32000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WindEuroGustDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CWindEuroGustDlg dialog
class CWindEuroGustDlg : public CDialogMove
{
// Construction
public:
	CWindEuroGustDlg(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CWindEuroGustDlg)
	enum { IDD = IDD_ETC_WIND_EURO_GUST_DLG };
	CTextUnit	m_wndEuroGustDepthUnit;  
	CTextUnit	m_wndEuroGustBreadthUnit; 
	CTextUnit	m_wndEuroGustHeightUnit; 
	CEditUnit	m_wndEuroGustFactor;
	CEditUnit	m_wndEuroGustDamping;
	CEditUnit	m_wndEuroGustFrequency;
	CEditUnit	m_wndEuroGustDepth;
	CEditUnit	m_wndEuroGustBreadth;
	CEditUnit	m_wndEuroGustHeight;
	CEditUnit	m_wndEuroGustVref;
	CEditUnit	m_wndEuroGustRoughCategory;
	CEditUnit m_wndEuroGustTopography;
	//}}AFX_DATA
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindEuroGustDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
//	T_WIND_D  m_Data;
	BOOL   m_bTopography;
	int		 m_nRoughCategory;	
	double m_dVref;
	double m_dCt;  
	double m_dCtRange;
	double m_dGustFactor;
	double m_dH;
	double m_dBB;
	double m_dBD;
	double m_dFreq;
	double m_dDamping;
	double CvtUnit(int nForce, int nLength, int nUnitType, double dblData);

protected:
	double GetTopographyFactor(double Ze);
	// Generated message map functions
	//{{AFX_MSG(CWindEuroGustDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdEuroGustOk();
	afx_msg void OnCmdEuroGustCancel();
	afx_msg void OnCmdEuroGustCal();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDEUROGUSTDLG_H__161493AD_292C_4D23_91EA_75B071E32000__INCLUDED_)
