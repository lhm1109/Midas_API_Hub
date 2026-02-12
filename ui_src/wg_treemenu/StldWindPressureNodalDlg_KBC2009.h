#if !defined(__TM_STLD_WIND_NODAL_KBC2009_H__)
#define __TM_STLD_WIND_NODAL_KBC2009_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindPressureArea_KBC2009.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\wg_cmd\CMWindPressureBaseChildDlg.h"
#include "wg_treemenures.h"

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureNodalDlg_KBC2009 dialog
class CCMWindPressureDlgArea;

class CStldWindPressureNodalDlg_KBC2009: public CCMWindPressureBaseChildDlg
{
	// Construction
public:
	CStldWindPressureNodalDlg_KBC2009(CWnd* pParent = NULL);   // standard constructor
	
	virtual BOOL DestroyWindow();
	virtual BOOL Dlg2Data( void* pData, BOOL bWarning = FALSE );
	virtual void Data2Dlg( void* pData, BOOL bWarning = FALSE );
	virtual void SetMemberData( void* pData) { m_pData = (T_WDPR_D*)pData; }
	
	enum { IDD = IDD_TM_STLD_WINDP_DLG_NODAL_KBC2009 };  
	
private:
	T_WDPR_D* m_pData;

	CEditUnit	m_editGustFactor_Ex;
	BOOL      m_bInAutoCalc; //dlg2data ¹«½Ã¿ë...

protected:	
	void InitDefaultData();

	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnCmdGustFactor();

	DECLARE_MESSAGE_MAP()
};


#endif
