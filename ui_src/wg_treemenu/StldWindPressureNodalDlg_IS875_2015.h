#if !defined(__TM_STLD_WIND_NODAL_IS875_2015_H__)
#define __TM_STLD_WIND_NODAL_IS875_2015_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldWindPressureNodalDlg_IS875_2015.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\wg_cmd\CMWindPressureBaseChildDlg.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg_IS875_2015 dialog
class CCMWindPressureDlgArea;

class CStldWindPressureNodalDlg_IS875_2015 : public CCMWindPressureBaseChildDlg, public CDBUpdateConnector
{
public:
	CStldWindPressureNodalDlg_IS875_2015(CWnd* pParent = NULL);
	virtual BOOL DestroyWindow();
	virtual BOOL Dlg2Data(void* pData, BOOL bWarning = FALSE);
	virtual void Data2Dlg(void* pData, BOOL bWarning = FALSE);
	virtual void SetMemberData(void* pData) { m_pData = (T_WDPR_D*)pData; }

	enum { IDD = IDD_TM_STLD_WINDP_DLG_NODAL_IS875_2015 };
private:
	T_WDPR_D* m_pData;
	int m_nCodeType;

	CEditUnit	m_editDirectFactor;
	CEditUnit	m_editCombFactor;

	// Pressure/Force coefficients
	CEditUnit	m_editCpe;
	CEditUnit	m_editCpi;
	CEditUnit	m_editForceCoef;

	// Radio buttons for coefficient mode
	MButton     m_radioPressCoef;
	MButton     m_radioForceCoef;
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	afx_msg void OnCoefModeRadioClicked();

	void ShowCoefFields(int mode);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(__TM_STLD_WIND_NODAL_IS875_2015_H__)