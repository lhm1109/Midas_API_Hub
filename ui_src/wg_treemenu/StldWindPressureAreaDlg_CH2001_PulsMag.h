#if !defined(__TM_STLD_WIND_AREA_CH2001_PULSMAGDLG_H__)
#define __TM_STLD_WIND_AREA_CH2001_PULSMAGDLG_H__

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

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

class CStldWindPressureAreaDlg_CH2001_PulsMag : public CDialogMove
{
// Construction
public:
	CStldWindPressureAreaDlg_CH2001_PulsMag(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_TM_STLD_WINDP_DLG_AREA_CH2001_PULS_MAGF };

// Attribute
public:
	double GetPeriod() { return m_dPeriod; }
	double GetDampingRatio() { return m_dDampingRatio; }
	double GetPulsMagnifyFactor() { return m_dMagnifyFactor; }
	void   SetWdpr(T_WDPR_D* pWdpr) { m_pWdpr = pWdpr; SetWvepKey(pWdpr->KeyWvep); }
protected:
	T_WDPR_D* m_pWdpr;
	T_WVEP_K  m_WvepK;

	MComboBox	m_cobxMaterial;
	CEditUnit m_edtDampingRatio;

	CEditUnit m_edtFundamentalPeriod;
	CEditUnit m_edtPulsMagFactor;

	T_WVEP_D  m_WvepD;
	double    m_dMagnifyFactor;
	double    m_dPeriod;


	double    m_dDampingRatio;
	int       m_nPeriodType;



protected:
	void SetWvepKey(T_WVEP_K key);
	void InitMaterialCombo();
	void InitControl();
	void Data2Dlg();
	void Dlg2Data();


	virtual void OnOK();
	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnBtnFundamentalPeriod();
	afx_msg void OnCalcMagFactor();
	afx_msg void OnSelChangeMatl();

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDGUSTKBC2009DLG_H__D88A7361_8496_11D4_A678_00010263A1CE__INCLUDED_)
