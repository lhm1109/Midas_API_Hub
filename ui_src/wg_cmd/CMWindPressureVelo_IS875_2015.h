#if !defined(__CM_WIND_VELO_IS875_2015_H__)
#define __CM_WIND_VELO_IS875_2015_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////   // anshika
// CCMWindPressureVelo_IS875_2015 dialog
class CCMWindPressureVelocityAddMod;

class CCMWindPressureVelo_IS875_2015 : public CChildDialog
{
	// Construction
public:
	CCMWindPressureVelo_IS875_2015(CWnd* pParent = NULL);   // standard constructor

	void SetData2Dlg(T_WVEP_IS875_2015& data);
	BOOL SetDlg2Data(T_WVEP_IS875_2015& data);


	// Dialog Data
	enum { IDD = IDD_CMD_DEF_WIND_VELOCITY_ITEM_IS875_2015 };
	CEditUnit	m_editSpeed;
	CTextUnit	m_unitVRange;
	MComboBox	m_cobxCategory;
	MComboBox	m_cobxBuildingClass;
	MComboBox	m_cobxStructClass;
	CEditUnit	m_editRiskCoef;
	MButton	  m_chkTopographic;
	CStatic 	m_unitSpeed;
	CEditUnit	m_editTopoFactor;
	CEditUnit	m_editVRange;
	MButton	  m_chkCycloRegEffects;
	MComboBox	m_cobxImportanceClass;
	CEditUnit	m_editk4Factor;

protected:
	void InitDefaultData();
	void InitCategoryCombo();
	void InitBuildingClassCombo();
	void InitStructureClassCombo();
	void InitCycloRegCombo();
	void CalculateRiskCoef();
	double GetK4Factor(int impfactor);
	void Data2Dlg();
	BOOL Dlg2Data();

private:
	CCMWindPressureVelocityAddMod* m_pParent;

protected:
	// Add your IS 875-specific members here
	CArray<UINT, UINT> m_aCtrlMethod;
	CArray<UINT, UINT> m_aCtrlAutoUser;
	BOOL m_bInit;
	//BOOL m_bModify;
	T_WVEP_IS875_2015  m_Data;
public:
	virtual BOOL DestroyWindow();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	virtual BOOL OnInitDialog();
	//afx_msg void OnCmdAutoCalcBtn();
	afx_msg void OnCmdTopoEffectCheck();
	afx_msg void OnCmdAutoUserRadio();
	afx_msg void OnSelchangeCmdSclassCombo();
	afx_msg void OnChangeCmdSpeedEdit();
	afx_msg void OnCmdCyclonRadio();
	afx_msg void OnSelchangeImportanceCombo();
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(__CM_WIND_VELO_IS875_2015_H__)


