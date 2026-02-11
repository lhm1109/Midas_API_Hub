#if !defined(__TM_STLD_WIND_NODAL_KDS2021_H__)
#define __TM_STLD_WIND_NODAL_KDS2021_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindPressureArea_KDS2021.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\wg_cmd\CMWindPressureBaseChildDlg.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "wg_treemenures.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureNodalDlg_KDS2021 dialog
class CCMWindPressureDlgArea;

class CStldWindPressureNodalDlg_KDS2021: public CCMWindPressureBaseChildDlg, public CDBUpdateConnector
{
	// Construction
public:
	CStldWindPressureNodalDlg_KDS2021(CWnd* pParent = NULL);   // standard constructor
	
	virtual BOOL DestroyWindow();
	virtual BOOL Dlg2Data( void* pData, BOOL bWarning = FALSE );
	virtual void Data2Dlg( void* pData, BOOL bWarning = FALSE );
	virtual void SetMemberData( void* pData) { m_pData = (T_WDPR_D*)pData; }
	
	enum { IDD = IDD_TM_STLD_WINDP_DLG_NODAL_KDS2021 };  
	
private:
	T_WDPR_D* m_pData;

	MButton m_chkVortex;
	CEditUnit	m_editDirectFactor;
	CEditUnit	m_editGustFactor_Ex;
	MButton	m_rdoAlongWind;
	MButton	m_rdoAcrossWind;
	MButton	m_rdoTorsionalWind;

	CMouseEdit	m_wndCoordXYZ;
	CTextUnit   m_wndCoordUnit;

	BOOL      m_bInAutoCalc; //dlg2data ¹«½Ã¿ë...

	CArray<UINT, UINT> m_aCtrlMethod;
	CArray<UINT, UINT> m_aCtrlBuildingType;
	CArray<UINT, UINT> m_aCtrlTorsional_Sub;

protected:	
	void InitDefaultData();

	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

	afx_msg void OnCmdGustFactor();
	afx_msg void OnCmdVibrationBtn();
	afx_msg void OnCmdBuildingTypeRadio();
	afx_msg void OnCmdWindTypeCheck();

	DECLARE_MESSAGE_MAP()
};


#endif // __TM_STLD_WIND_NODAL_KDS2021_H__
