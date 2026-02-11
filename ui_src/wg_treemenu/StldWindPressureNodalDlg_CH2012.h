#if !defined(__TM_STLD_WIND_NODAL_CH2012_H__)
#define __TM_STLD_WIND_NODAL_CH2012_H__

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
// CStldWindPressureNodalDlg_CH2012 dialog
class CCMWindPressureDlgArea;

class CStldWindPressureNodalDlg_CH2012 : public CCMWindPressureBaseChildDlg
{
	// Construction
public:
	CStldWindPressureNodalDlg_CH2012(CWnd* pParent = NULL);   // standard constructor
	
	virtual BOOL DestroyWindow();
	virtual BOOL Dlg2Data( void* pData, BOOL bWarning = FALSE );
	virtual void Data2Dlg( void* pData, BOOL bWarning = FALSE );
	virtual void SetMemberData( void* pData) { m_pData = (T_WDPR_D*)pData; }
	
	enum { IDD = IDD_TM_STLD_WINDP_DLG_NODAL_CH2012 };
	
private:  
	T_WDPR_D* m_pData;

	int  m_nType;
	BOOL bConsiderPartition;
	BOOL bConsiderWindshield;
	BOOL bConsiderAlongWind;
	BOOL bConsiderCrossWind;

	CEditUnit m_edtDampingRatio;
	CEditUnit m_edtBasePeriod;
	CEditUnit m_edtBasePeriod2;
	CEditUnit m_edtWindwardCf;
	CEditUnit m_edtLeewardCf;
	CEditUnit m_edtPartitionPresCf;
	CEditUnit m_edtWindshieldCf;
//   CEditUnit m_edtCftWinPressure;
//   CEditUnit m_edtCftDampingRatio;
	
	int m_nPeriodType; // Period 의 값 임시 저장.
	double m_dPeriod;  // Period 의 값 임시 저장.
	double m_dPeriod2;  // Period 의 값 임시 저장.

protected:	
	void InitControl();
	int  GetType();

	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnChkConsiderPartition();
	afx_msg void OnChkConsiderWindShield();
	afx_msg void OnChkConsiderCrossWind();
	afx_msg void OnBtnPeriod();
	afx_msg void OnBtnPeriod2();
	afx_msg void OnBtnCrossWind();
	afx_msg void OnCmdWindAlongCheck();
	//afx_msg void OnChkConsiderComfort(); //Comfort 하지 않는다. 140326 kys0522

	DECLARE_MESSAGE_MAP()
};


#endif
