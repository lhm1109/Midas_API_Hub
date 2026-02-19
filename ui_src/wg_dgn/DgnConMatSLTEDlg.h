#if !defined(AFX_DGNCONMATSLTEDLG_H__63E0A181_C5B7_4A4F_85AC_B24812C67C20__INCLUDED_)
#define AFX_DGNCONMATSLTEDLG_H__63E0A181_C5B7_4A4F_85AC_B24812C67C20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConMatSLTEDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_TextUnit.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnConMatSLTEDlg dialog

class CDgnConMatSLTEDlg : public CDialogMove
{
// Construction
public:
	CDgnConMatSLTEDlg(CWnd* pParent = NULL);   // standard constructor

public:
	int m_iMatdKey;

// Dialog Data
	//{{AFX_DATA(CDgnConMatSLTEDlg)
	enum { IDD = IDD_DGN_CON_MAT_SLEL_DLG };
	double	m_dSTn;
	double	m_dLTn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConMatSLTEDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	double m_dUnitElast;
	
	void Init_Unit();
	void Init_EcData();
	void Set_Unit();
	double Covert_Unit(int iLengthUnit, int iForceUnit);

	void Set_InitByCode(T_MATD_D rData);
	double Get_Fc(CString strMatlDB, double dfc);
	double Get_Ec(double dfc);
	double Get_Es(double dFy);
	// Generated message map functions
	//{{AFX_MSG(CDgnConMatSLTEDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONMATSLTEDLG_H__63E0A181_C5B7_4A4F_85AC_B24812C67C20__INCLUDED_)





















