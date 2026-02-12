#if !defined(AFX_WINDGUSTFIBC2000DLG_H__04C9E1E2_3D38_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_WINDGUSTFIBC2000DLG_H__04C9E1E2_3D38_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WindGustFIBC2000Dlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CWindGustFIBC2000Dlg dialog
class CWindGustFIBC2000Dlg : public CDialogMove
{
// Construction
public:
	CWindGustFIBC2000Dlg(CWnd* pParent = NULL);   // standard constructor
	enum CALCTYPE: int
	{
		IBC = 0,
		NSR = 1,
		NSCP = 2
	};
	void SetDlgType(const CALCTYPE nType){ m_nCalcType = nType; }

	double CvtUnit(int nForce, int nLength, int nUnitType, double dblData);

// Dialog Data
	//{{AFX_DATA(CWindGustFIBC2000Dlg)
	enum { IDD = IDD_ETC_WIND_GUSTRIGID_DLG };
	CTextUnit	m_YBreadthUnit;
	CTextUnit	m_XBreadthUnit;
	CTextUnit	m_heightUnit;
	CEditUnit	m_YFactor;
	CEditUnit	m_XFactor;
	CEditUnit	m_Exposure;
	CEditUnit	m_XBreadth;
	CEditUnit	m_YBreadth;
	CEditUnit	m_height;
	//}}AFX_DATA

	double	m_dheight;
	int			m_nExposure;
	double	m_dXFactor;
	double	m_dYFactor;
	double  m_dBX;
	double  m_dBY;
	CALCTYPE    m_nCalcType;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindGustFIBC2000Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWindGustFIBC2000Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdWindgustCancel();
	afx_msg void OnCmdWindgustOk();
	afx_msg void OnCmdWindgustRigidCal();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////










/////////////////////////////////////////////////////////////////////////////
// CWindGustF2IBC2000Dlg dialog
class CWindGustF2IBC2000Dlg : public CDialogMove
{
// Construction
public:
	CWindGustF2IBC2000Dlg(CWnd* pParent = NULL);   // standard constructor

	enum CALCTYPE: int
	{
		IBC = 0,
		NSR = 1,
		NSCP = 2
	};
	void SetDlgType(const CALCTYPE nType){ m_nCalcType = nType; }

	double CvtUnit(int nForce, int nLength, int nUnitType, double dblData);

// Dialog Data
	//{{AFX_DATA(CWindGustF2IBC2000Dlg)
	enum { IDD = IDD_ETC_WIND_GUSTFLEXI_DLG };
	CTextUnit	m_YBreadthUnit;
	CTextUnit	m_XBreadthUnit;
	CTextUnit	m_heightUnit;
	CEditUnit	m_YFactor;
	CEditUnit	m_XFactor;
	CEditUnit	m_Damping;
	CEditUnit	m_YNatural;
	CEditUnit	m_XNatural;
	CEditUnit	m_YBreadth;
	CEditUnit	m_XBreadth;
	CEditUnit	m_height;
	CEditUnit	m_Exposure;
	CEditUnit	m_BasicWind;
	//}}AFX_DATA

	double	m_dheight;
	int			m_nExposure;
	double	m_dBasicWind;
	double	m_dXFactor;
	double	m_dYFactor;
	double  m_dBX;
	double  m_dBY;
	double  m_dXNatural;
	double  m_dYNatural;
	double  m_dDamping;
	CALCTYPE m_nCalcType;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindGustF2IBC2000Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWindGustF2IBC2000Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdWindgustOk();
	afx_msg void OnCmdWindgustCancel();
	afx_msg void OnCmdWindgustFlexiCal();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDGUSTFIBC2000DLG_H__04C9E1E2_3D38_11D5_880F_00010263A1F4__INCLUDED_)
