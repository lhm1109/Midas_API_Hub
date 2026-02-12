#if !defined(__TM_STLD_WIND_AREA_CH2012_CROSSTORSION_H__)
#define __TM_STLD_WIND_AREA_CH2012_CROSSTORSION_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WindGustKBC2009Dlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureArea_KBC2009_GustF dialog

class CStldWindPressureAreaDlg_CH2012_CrossTorsion : public CDialogMove
{
// Construction
public:
	CStldWindPressureAreaDlg_CH2012_CrossTorsion(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_TM_STLD_WINDP_DLG_AREA_CH2012_TOR_VIB };

// Attribute
public:
	void SetData(const T_WDPR_CH2012& rWdprCH2012);
	void GetData(T_WDPR_CH2012& rWdprCH2012);

protected:
	CEditUnit m_edtCrossPeriod;
	CEditUnit m_edtTorsionPeriod;

	BOOL   m_bCross;
	int    m_nType;
	double m_dCrossPeriod;
	BOOL   m_bTorsion;
	double m_dTorsionPeriod;

protected:
	void Data2Dlg();
	void Dlg2Data();

	virtual void OnOK();
	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnChkCross();
	afx_msg void OnChkTorsion();
	afx_msg void OnRdoRect();
	afx_msg void OnRdoReound();

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDGUSTKBC2009DLG_H__D88A7361_8496_11D4_A678_00010263A1CE__INCLUDED_)
