#if !defined(AFX_WINDGUSTNBC95DLG_H__7ED14227_C515_4BAB_99CE_BF38A5B36A7C__INCLUDED_)
#define AFX_WINDGUSTNBC95DLG_H__7ED14227_C515_4BAB_99CE_BF38A5B36A7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WindGustNbc95Dlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CWindGustNbc95Dlg dialog
class CWindGustNbc95Dlg : public CDialogMove
{
// Construction
public:
	CWindGustNbc95Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWindGustNbc95Dlg)
	enum { IDD = IDD_ETC_WIND_GUST_NBC95_DLG };	
	CTextUnit	m_wndBreadthYUnit;
	CTextUnit	m_wndBreadthXUnit;
	CTextUnit	m_wndBldgHeightUnit;
	CEditUnit	m_wndTopography;
	CEditUnit	m_wndWindSpeed;
	CEditUnit	m_wndBreadthY;
	CEditUnit	m_wndBreadthX;
	CEditUnit	m_wndGustY;
	CEditUnit	m_wndGustX;
	CEditUnit	m_wndFrequencyY;
	CEditUnit	m_wndFrequencyX;
	CEditUnit	m_wndExposure;
	CEditUnit	m_wndDamping;	
	CEditUnit	m_wndBldgHeight;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindGustNbc95Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	int    m_nExposure;   
	double m_dWindSpeed;
	double m_dBldgHeight;
	double m_dBreadthX;
	double m_dBreadthY;
	double m_dFrequencyX;
	double m_dFrequencyY;
	double m_dDamping;
	double m_dGustFactorX;
	double m_dGustFactorY;
	BOOL   m_bTopography;
	double m_dHillHeight;
	double m_dHillLength;
	double m_dHillDistance;
	int    m_nHillShape;
	int    m_nBldgLocation;
	double CvtUnit(int nForce, int nLength, int nUnitType, double dblData);

protected:
	double CalcExposureFactor(BOOL bTopography, double dHeight, int nExposure, int nHillShape, 
				 int nBldgLocation, double dHillHeight, double dHillLength, double dHillDistance, double& dCeH);
	double CalcGustFactor(double dCeH, int nExposure, double dWindSpeed, double dBldgHeight,
												double dBldgWidth, double dno, double beta, double& Gfactor);
	// Generated message map functions
	//{{AFX_MSG(CWindGustNbc95Dlg)
	afx_msg void OnCmdNbc95CalculateBtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdWindgustOk();
	afx_msg void OnCmdWindgustCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDGUSTNBC95DLG_H__7ED14227_C515_4BAB_99CE_BF38A5B36A7C__INCLUDED_)
