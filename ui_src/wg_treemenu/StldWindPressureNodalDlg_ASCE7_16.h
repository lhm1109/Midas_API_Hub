#if !defined(__TM_STLD_WIND_NODAL_ASCE7_16_H__)
#define __TM_STLD_WIND_NODAL_ASCE7_16_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldWindPressureNodalDlg_ASCE7_16.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\wg_cmd\CMWindPressureBaseChildDlg.h"
#include "wg_treemenures.h"

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureNodalDlg_ASCE7_16 dialog
class CCMWindPressureDlgArea;

class CStldWindPressureNodalDlg_ASCE7_16: public CCMWindPressureBaseChildDlg, public CDBUpdateConnector
{
	// Construction
public:
	CStldWindPressureNodalDlg_ASCE7_16(CWnd* pParent = NULL);   // standard constructor
	
	virtual BOOL DestroyWindow();
	virtual BOOL Dlg2Data( void* pData, BOOL bWarning = FALSE );
	virtual void Data2Dlg( void* pData, BOOL bWarning = FALSE );
	virtual void SetMemberData( void* pData) { m_pData = (T_WDPR_D*)pData; }
	
	enum { IDD = IDD_TM_STLD_WINDP_DLG_NODAL_ASCE7_16 };
	
private:
	T_WDPR_D* m_pData;

	CEditUnit	m_editGroundFactor;
	CEditUnit	m_editDirectFactor;
	CEditUnit	m_editGustFactor_Ex;

	BOOL      m_bInAutoCalc; //dlg2data ¹«½Ã¿ë...

protected:	
	void InitDefaultData();

	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

	afx_msg void OnCmdGustFactor();
	afx_msg void OnCmdBuildingTypeRadio();

	DECLARE_MESSAGE_MAP()
};


#endif // __TM_STLD_WIND_NODAL_ASCE7_16_H__
