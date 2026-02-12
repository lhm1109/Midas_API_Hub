#if !defined(__TM_STLD_WIND_AREA_ASCE7_16_H__)
#define __TM_STLD_WIND_AREA_ASCE7_16_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\mit_frx\MComboBox.h"
#include "..\wg_cmd\CMWindPressureBaseChildDlg.h"
#include "wg_treemenures.h"

using namespace mit::frx;
class CCMWindPressureDlgArea;

class CStldWindPressureAreaDlg_ASCE7_16: public CCMWindPressureBaseChildDlg, public CDBUpdateConnector
{
	// Construction
public:
	CStldWindPressureAreaDlg_ASCE7_16(int nCodeType, CWnd* pParent = NULL);   // standard constructor
	
	virtual BOOL DestroyWindow();
	virtual BOOL Dlg2Data( void* pData, BOOL bWarning = FALSE );
	virtual void Data2Dlg( void* pData, BOOL bWarning = FALSE );
	virtual void SetMemberData( void* pData) { m_pData = (T_WDPR_D*)pData; }
	
	enum { IDD = IDD_TM_STLD_WINDP_DLG_AREA_ASCE7_16 };  
	
private:
	T_WDPR_D* m_pData;
	int m_nCodeType;
	CEditUnit	m_editGroundFactor;
	CEditUnit	m_editDirectFactor;

	CEditUnit	m_editGustFactor_Ex;
	CEditUnit	m_editGustFactor_In;
	CEditUnit	m_editCoefFactor_Ex;
	CEditUnit	m_editCoefFactor_In;

	MComboBox	m_cmbStructureType;

// 	CMouseEdit	m_wndCoordXYZ;
// 	CTextUnit   m_wndCoordUnit;

	BOOL      m_bInAutoCalc; //dlg2data ¹«½Ã¿ë...

protected:	
	void InitDefaultData();
	void InitStructureTypeCombo();

	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

	afx_msg void OnCmdGustFactor();
	afx_msg void OnCmdCoefFactor();
	afx_msg void OnChkAutoCalcForceCoef();

	DECLARE_MESSAGE_MAP()
};


#endif // !defined(__TM_STLD_WIND_AREA_ASCE7_16_H__)
