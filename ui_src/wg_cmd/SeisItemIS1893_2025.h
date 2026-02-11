#if !defined(__SEISITEMIS1893_2025_H__)
#define __SEISITEMIS1893_2025_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisItemIS2002.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "../wg_base/FoldGroupBox.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CSeisItemIS1893_2025 dialog

class CSeisItemIS1893_2025 : public CChildDialog
{
// Construction
public:
	CSeisItemIS1893_2025(CWnd* pParent = NULL);   // standard constructor

	void SetData2Dlg(T_SEIS_IS1893_2025& data, BOOL bModify);
	BOOL SetDlg2Data(T_SEIS_IS1893_2025& data);

// Dialog Data
	//{{AFX_DATA(CSeisItemIS1893_2025)
	enum { IDD = IDD_ETC_SEIS_ITEM_IS1893_2025 };
	MComboBox   m_cmbSpecType;
	MComboBox	m_cmbEqZone;
	MComboBox	m_cmbRetPeriod;
	CEditUnit	m_editZoneFactor;
	MComboBox	m_cmbSiteClass;
	MComboBox	m_cmbImportanceFact;
	CEditUnit	m_editDamping;

	CEditUnit	m_editPeriodX;
	CEditUnit	m_editPeriodY;
	MComboBox	m_cobxRX;
	MComboBox	m_cobxRY;
	CFoldGroupBox_Cross m_grpFold;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisItemIS1893_2025)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void Data2Dlg();
	BOOL Dlg2Data();

	void InitDefaultData();
	void InitSpectrumTypeCombo();
	void InitEqZoneCombo();
	void InitRetPeriodCombo();
	void InitSiteClassCombo();
	void InitZoneCombo();
	void InitSoilCombo();

private:
	CStringArray m_aRFactorName;
	CArray<double, double> m_aRFactorData;

protected:
	BOOL m_bInit;
	BOOL m_bModify;
	CSeisItemDlgNew* m_pParent;
	T_SEIS_IS1893_2025  m_Data;

	T_STOR_K m_nBaseStoryK;
	double	m_dHXIS2025;
	double	m_dHYIS2025;
	double	m_dBXIS2025;
	double	m_dBYIS2025;	
	int     m_nMtdXIS2025;
	int     m_nMtdYIS2025;
	double	m_dAfpX;
	double	m_dAfpY;
	double	m_dAmwX;
	double	m_dAmwY;
	double	m_dAwaX;
	double	m_dAwaY;
	double	m_dAcoX;
	double	m_dAcoY;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSeisItemIS1893_2025)
	virtual BOOL OnInitDialog();
	void SetFoldGroupCtrls();
	afx_msg void OnCmdIs25TCalcBtn();
	afx_msg void OnEqZoneChange();
	afx_msg void OnRetPeriodChange();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	double m_adZoneFacts[EN_IS25_EQ_ZONE_COUNT][EN_IS25_RET_COUNT] = {
	{ 0.0375, 0.0500, 0.0600, 0.0750, 0.1000, 0.1125, 0.1500, 0.2000, 0.2700 }, // Zone II
	{ 0.0625, 0.0850, 0.1000, 0.1250, 0.1670, 0.1875, 0.2500, 0.3330, 0.4500 }, // Zone III
	{ 0.1400, 0.1750, 0.2100, 0.2330, 0.2800, 0.2917, 0.3500, 0.4400, 0.5250 }, // Zone IV
	{ 0.2000, 0.2500, 0.3000, 0.3330, 0.4000, 0.4167, 0.5000, 0.6250, 0.7500 }, // Zone V
	{ 0.3000, 0.3750, 0.4500, 0.5000, 0.6000, 0.6250, 0.7500, 0.9400, 1.1250 }  // Zone VI
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SEISITEMIS2002_H__)
