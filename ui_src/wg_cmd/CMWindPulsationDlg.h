#if !defined(AFX_CMWINDPULSATIONDLG_H__)
#define AFX_CMWINDPULSATIONDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSpecModeDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "CMWindPulsationModeGrid.h"
//#include "CMSpecModeGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CCMWindPulsationDlg dialog

#include "HeaderPre.h"
enum EN_WIND_DIRECTION
{
	EN_WIND_DIRECTION_X = 0,
	EN_WIND_DIRECTION_Y = 1
};

class __MY_EXT_CLASS__ CCMWindPulsationDlg : public CDialogMove
{
// Construction
public:
	CCMWindPulsationDlg(CDBDoc* pDoc, T_KEY KeyWind, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMWindPulsationDlg)
	enum { IDD = IDD_ETC_WIND_PULSATION_DLG	};
	CComboBox m_cmbConsType;
	CComboBox m_cmbTerrType;
	
	// Logarithmic Decrement of Oscillations (IDC_CMD_WIND_PUL_OSCI_DECR_EDT)
	CEditUnit m_edtOsciDecrement;

	// Normative value of wind pressure (IDC_CMD_WIND_PUL_WIND_PRES_EDT)
	CEditUnit m_edtWindPressure;

	// Dimension - According to Plan (X-Direction, Y-Direction)
	CEditUnit m_edtDimPlanX;  // IDC_CMD_WIND_DIMS_PLAN_X_EDT
	CEditUnit m_edtDimPlanY;  // IDC_CMD_WIND_DIMS_PLAN_Y_EDT

	// Dimension - Along Wind Front (X-Direction, Y-Direction)
	CEditUnit m_edtDimWindX;  // IDC_CMD_WIND_DIMS_WIND_X_EDT
	CEditUnit m_edtDimWindY;  // IDC_CMD_WIND_DIMS_WIND_Y_EDT

	// Unit labels for dimensions (static text controls)
	CTextUnit m_untDimPlanX;  // IDC_CMD_WIND_DIMS_PLAN_X_UNT
	CTextUnit m_untDimPlanY;  // IDC_CMD_WIND_DIMS_PLAN_Y_UNT
	CTextUnit m_untDimWindX;  // IDC_CMD_WIND_DIMS_WIND_X_UNT
	CTextUnit m_untDimWindY;  // IDC_CMD_WIND_DIMS_WIND_Y_UNT

	// Direction Factor (X-Direction, Y-Direction)
	CEditUnit m_edtDirectionFactorX;  // IDC_CMD_WIND_PUL_DIRE_FACT_X_EDT
	CEditUnit m_edtDirectionFactorY;  // IDC_CMD_WIND_PUL_DIRE_FACT_Y_EDT

	// Mode selection checkbox (IDC_CMD_WIND_SELECT_MODE_CHK)
	CButton m_chkAutoSelectMode;

	// Sum of Modal Mass Percentage (X-Direction, Y-Direction)
	CEditUnit m_edtSumModalMassX;  // IDC_CMD_PUL_DIRE_SUM_X_EDT
	CEditUnit m_edtSumModalMassY;  // IDC_CMD_PUL_DIRE_SUM_Y_EDT
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindPulsationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc *m_pDoc;
	T_KEY m_KeyWind;
	CArray<UINT, UINT> m_aCtrlRadio;
	CArray<UINT, UINT> m_arCtrlSignType;  
	CArray<UINT, UINT> m_arCtrlHide;
	CArray<UINT, UINT> m_arCtrlMove;
	CCMWindPulsationModeGrid m_wndGridX;
	CCMWindPulsationModeGrid m_wndGridY;
	T_WNPS_D* m_pData;
	CArray<double, double> m_arModePercentX;
	CArray<double, double> m_arModePercentY;
//  CCMWindPulsationModeGrid m_wndGrid;
protected:
	void Dlg2Data();
	BOOL Data2Dlg();
public:
	void UpdatePercent(int nDirection, double dPercent);
	void SetDataSource(T_WNPS_D* pData, CArray<double, double>& arModePercentX, CArray<double, double>& arModePercentY);
	void InitControls();
	// Generated message map functions
	//{{AFX_MSG(CCMWindPulsationDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdSpecSelectModeChk();
	afx_msg void OnCmdSpecAllBtn();
	afx_msg void OnCmdSpecNoneBtn();
	afx_msg void OnCmdSpecAll_Y_Btn();
	afx_msg void OnCmdSpecNone_Y_Btn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMWINDPULSATIONDLG_H__)
