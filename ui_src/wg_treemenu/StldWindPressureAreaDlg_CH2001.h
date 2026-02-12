#if !defined(__TM_STLD_WIND_AREA_CH2001_H__)
#define __TM_STLD_WIND_AREA_CH2001_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindPressureVelo_KBC2009.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\wg_cmd\CMWindPressureBaseChildDlg.h"
#include "wg_treemenures.h"

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg_CH2001 dialog
class CCMWindPressureDlgArea;

class CStldWindPressureAreaDlg_CH2001 : public CCMWindPressureBaseChildDlg //CChildDialog
{
	// Construction
public:
	CStldWindPressureAreaDlg_CH2001(CWnd* pParent = NULL);   // standard constructor
	
	virtual BOOL DestroyWindow();
	virtual BOOL Dlg2Data( void* pData, BOOL bWarning = FALSE );
	virtual void Data2Dlg( void* pData, BOOL bWarning = FALSE );
	virtual void SetMemberData( void* pData) { m_pData = (T_WDPR_D*)pData; }
	
	enum { IDD = IDD_TM_STLD_WINDP_DLG_AREA_CH2001 };
	
private:
	T_WDPR_D* m_pData;

	CEditUnit m_edtMagnifyFactor;
	CEditUnit m_edtCoeff;
//   CEditUnit m_edtWindPressure;  //added by yskim 140326 일단 중국기준 comfort 하지 않도록.
//   CEditUnit m_edtDampingRatio;
	
	BOOL   m_bInitChild;
	double m_dChildPeriod;
	double m_dChildDampingRatio;

protected:	
	int GetType();
	void InitDefaultData();
	void InitControl();

	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnBtnMagFactorAutoCalc();
	afx_msg void OnRdoChanged();
	//afx_msg void OnBtnCheckVibration();

	DECLARE_MESSAGE_MAP()
};


#endif
