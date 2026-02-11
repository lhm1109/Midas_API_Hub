#if !defined(__CM_WIND_VELO_KBC2016_H__)
#define __CM_WIND_VELO_KBC2016_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindPressureVelo_KBC2016.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureVelo_KBC2016 dialog
class CCMWindPressureVelocityAddMod;

class CCMWindPressureVelo_KBC2016 : public CChildDialog
{
	// Construction
public:
	CCMWindPressureVelo_KBC2016(CWnd* pParent = NULL);   // standard constructor
	
	void SetData2Dlg(T_WVEP_KBC2016& data);
	BOOL SetDlg2Data(T_WVEP_KBC2016& data);
	
	// Dialog Data
	//{{AFX_DATA(CCMWindPressureVelo_KBC2016)
	enum { IDD = IDD_CMD_DEF_WIND_VELOCITY_ITEM_KBC2016 };
	MComboBox	m_cobxCategory;
// 	CTextUnit m_unitSpeed;        // by hp, 단위변환 안되야함..
	CEditUnit	m_editSpeed;
	MComboBox m_cobxImportFactor;
	CTextUnit	m_unitRoofHeight;
	CEditUnit	m_editRoofHeight;
	MButton	m_chkTopoEffect;
	CEditUnit	m_editKZT;
	CTextUnit	m_unitVKZT;
	CEditUnit	m_editVKZT;
	
// 	CTextUnit m_unitSpeed2;
	CEditUnit	m_editSpeed2;
	CTextUnit	m_unitRoofHeight2;
	CEditUnit	m_editRoofHeight2;
	MComboBox	m_cobxCeFactor;
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
	T_WVEP_KBC2016  m_Data;
	
	CArray<UINT, UINT> m_aCtrlMethod;
	CArray<UINT, UINT> m_aCtrlSimple;
	CArray<UINT, UINT> m_aCtrlGeneral;
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindPressureVelo_KBC2016)
public:
	virtual BOOL DestroyWindow();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CCMWindPressureVelo_KBC2016)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdMethodRadio();
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

#endif // !defined(__CMWINDITEMKBC2015_H__)
