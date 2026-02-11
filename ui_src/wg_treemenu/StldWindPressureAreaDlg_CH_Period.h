#if !defined(__TM_STLD_WIND_AREA_CH_PERIOD_DLG_H__)
#define __TM_STLD_WIND_AREA_CH_PERIOD_DLG_H__

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

class CStldWindPressureAreaDlg_CH_Period : public CDialogMove
{
// Construction
public:
	CStldWindPressureAreaDlg_CH_Period(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_TM_STLD_WINDP_DLG_AREA_CH_PERIOD };

// Attribute
public:
	void SetDlgType(int nType) { m_nType = nType; }
	void SetInit(double dB, double dH, int dN);
	int  GetDlgType() { return m_nType; }
	void GetDlgPeriod( double& dPeriod ) { dPeriod = m_dPeriod; }
	

protected:
	int  m_nType;
	CEditUnit m_edtH;
	CEditUnit m_edtB;
	CEditUnit m_edtN;
	double m_dPeriod;
	double m_dB;
	double m_dH;
	int    m_dN;

protected:
	void InitControl();
	void Data2Dlg();
	void Dlg2Data();
	
	int  GetType();
	BOOL CalcPeriod(double dH, double dB, int nN, int nMethod, double& dPeriod);

	virtual void OnOK();
	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	
	afx_msg void OnCmdRdoDirChanged();
	afx_msg void OnCmdRdoTypeChanged();
	
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDGUSTKBC2009DLG_H__D88A7361_8496_11D4_A678_00010263A1CE__INCLUDED_)
