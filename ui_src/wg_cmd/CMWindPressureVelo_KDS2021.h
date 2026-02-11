#if !defined(__CM_WIND_VELO_KDS2021_H__)
#define __CM_WIND_VELO_KDS2021_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindPressureVelo_KDS2021.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureVelo_KDS2021 dialog
class CCMWindPressureVelocityAddMod;

class CCMWindPressureVelo_KDS2021 : public CChildDialog
{
	// Construction
public:
	CCMWindPressureVelo_KDS2021(CWnd* pParent = NULL);   // standard constructor
	
	void SetData2Dlg(T_WVEP_KDS2021& data);
	BOOL SetDlg2Data(T_WVEP_KDS2021& data);
	
	// Dialog Data
	//{{AFX_DATA(CCMWindPressureVelo_KDS2021)
	enum { IDD = IDD_CMD_DEF_WIND_VELOCITY_ITEM_KDS2021 };
	MComboBox	m_cobxCategory;
// 	CTextUnit m_unitSpeed;        // by hp, 단위변환 안되야함..
	CEditUnit	m_editSpeed;
	MComboBox	m_cobxImportFactor;
	CTextUnit	m_unitRoofHeight;
	CEditUnit	m_editRoofHeight;
	MButton		m_chkTopoEffect;
	CEditUnit	m_editKZT;
	CTextUnit	m_unitVKZT;
	CEditUnit	m_editVKZT;
	
// 	CTextUnit m_unitSpeed2;
	CEditUnit	m_editSpeed2;
	CTextUnit	m_unitRoofHeight2;
	CEditUnit	m_editRoofHeight2;
	MComboBox	m_cobxCeFactor;
	MButton		m_chkVortex;

	CTextUnit	m_unitRoofHeight3;
	CTextUnit	m_unitDiaDM;
	CTextUnit	m_unitDiaDB;
// 	CTextUnit	m_unitNatural;
	CTextUnit	m_unitMass;
	CEditUnit	m_editRoofHeight3;
	CEditUnit	m_editDiaDM;
	CEditUnit	m_editDiaDB;
	CEditUnit	m_editNatural;
	CEditUnit	m_editMass;
	CEditUnit	m_editDamping;
	//}}AFX_DATA
	
protected:
	void InitDefaultData();
	void InitCategoryCombo();
	void InitImportFactorCombo();
	void InitCeFactorCombo();
	void AlignControls();
	void Data2Dlg();
	BOOL Dlg2Data();

private:
	CCMWindPressureVelocityAddMod* m_pParent;

protected:
	int m_nClassifyStructure;
	int m_nClassifyPlane;
	double m_dClassifyBX;
	double m_dClassifyBY;
	double m_dAreadKBC2015;
	
	BOOL m_bInit;
	T_WVEP_KDS2021  m_Data;
	
	CArray<UINT, UINT> m_aCtrlMethod;
	CArray<UINT, UINT> m_aCtrlSimple;
	CArray<UINT, UINT> m_aCtrlGeneral;
	CArray<UINT, UINT> m_aCtrlVortex;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindPressureVelo_KDS2021)
public:
	virtual BOOL DestroyWindow();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CCMWindPressureVelo_KDS2021)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdMethodRadio();
	afx_msg void OnCmdVortexSheddingBtn();
	afx_msg void OnCmdGeneralBtn();
	afx_msg void OnCmdTopoEffectCheck();
	afx_msg void OnChangeSpeedEdit();
	afx_msg void OnChangeSimpleSpeedEdit();
	afx_msg void OnChangeRoofHeightEdit();
	afx_msg void OnChangeSimpleRoofHeightEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CM_WIND_VELO_KDS2021_H__)
