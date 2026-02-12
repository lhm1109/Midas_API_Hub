#if !defined(__CMWINDITEMKBC2015_H__)
#define __CMWINDITEMKBC2015_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemKBC2015.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MButton.h"

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemKBC2015 dialog
class CCMWindItemDlg;

class CCMWindItemKBC2015 : public CChildDialog
{
	// Construction
public:
	CCMWindItemKBC2015(CWnd* pParent = NULL);   // standard constructor
	
	void SetData2Dlg(T_WIND_KBC2015& data, BOOL bModify, T_KEY KeyWAorWT);
	BOOL SetDlg2Data(T_WIND_KBC2015& data);
	
	// Dialog Data
	//{{AFX_DATA(CCMWindItemKBC2015)
	enum { IDD = IDD_ETC_WIND_ITEM_KBC2015 };
	CBCGPStatic  	m_unitSpeed2;
	CEditUnit	m_editSpeed2;
	CTextUnit	m_unitRoofHeight2;
	CEditUnit	m_editRoofHeight2;
	CTextUnit	m_unitMinX;
	CEditUnit	m_editMinX;
	CTextUnit	m_unitMinY;
	CEditUnit	m_editMinY;
	MComboBox	m_cobxCeFactor;
	MButton	m_chkTopoEffect;
	MButton	m_chkLoadEval;
	CTextUnit	m_unitRoofHeight;
	CEditUnit	m_editRoofHeight;
	CBCGPStatic 	m_unitSpeed;        // by hp, 단위변환 안되야함..
	CEditUnit	m_editSpeed;
	CEditUnit	m_editKZT;
	CEditUnit	m_editGustFactorX;
	CEditUnit	m_editGustFactorY;  
	CEditUnit	m_editForceCoef;
	MComboBox	m_cobxCategory;
	MComboBox m_cobxImportFactor;
	CFoldGroupBox_Cross m_grpWindParam;
	MComboBox	m_cmbStructureType;
	MButton	m_chkAcrossWind;
	MButton	m_chkTorsionalWind;
	MButton	m_chkWindResponse;
	//}}AFX_DATA
	
protected:
	void InitDefaultData();
	void InitCategoryCombo();
	void InitImportFactorCombo();
	void InitStructureTypeCombo();
	void InitCeFactorCombo();
	void AlignControls();
	void Data2Dlg();
	BOOL Dlg2Data();
	void GetMassXYValue(double& dMassX, double& dMassY, double& dMassXY);

private:
	CCMWindItemDlg* m_pParent;
	T_WNAT_D m_WnatD;

public:
	void GetWnatData(T_WNAT_D& WnatD) { WnatD.arWnatStor.Copy(m_WnatD.arWnatStor); }

protected:
	int m_nClassifyStructure;
	int m_nClassifyPlane;
	double m_dClassifyBX;
	double m_dClassifyBY;

	double m_dBLxKBC2015;		
	double m_dBLyKBC2015;	
	double m_dMinBXKBC2015;
	double m_dMinBYKBC2015;
	double m_dAreadKBC2015;	
	double m_dFreqXKBC2015;	
	double m_dFreqYKBC2015;	
	double m_dDampingKBC2015;

	double m_dBX;
	double m_dBY;
	double m_dFreqX;
	double m_dFreqY;
	double m_dFreqT;
	double m_dMassX;
	double m_dMassY;
	double m_dMoment;
	double m_dZf;
	
	BOOL m_bInit;
	BOOL m_bModify;
	T_KEY m_KeyWAorWT;
	T_WIND_KBC2015  m_Data;
	
	CArray<UINT, UINT> m_aCtrlMethod;
	CArray<UINT, UINT> m_aCtrlSimple;
	CArray<UINT, UINT> m_aCtrlGeneral;
	CArray<UINT, UINT> m_aCtrlType;
	CArray<UINT, UINT> m_aCtrlForceCoef_user;
	CArray<UINT, UINT> m_aCtrlForceCoef_auto;
	CArray<UINT, UINT> m_aCtrlForceCoef;
	CArray<UINT, UINT> m_aCtrlRiseType;
	CArray<UINT, UINT> m_aCtrlRise;
	CArray<UINT, UINT> m_aRdoForceCoefCalcType;
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemKBC2015)
public:
	virtual BOOL DestroyWindow();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CCMWindItemKBC2015)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdMethodRadio();
	afx_msg void OnCmdGeneralBtn();
	afx_msg void OnCmdVibrationBtn();
	afx_msg void OnCmdGustFactorBtn();
	afx_msg void OnCmdLoadEvalCheck();
	afx_msg void OnCmdTopoEffectCheck();
	afx_msg void OnCmdChangeRdo_ForceCoefCalcType();
	afx_msg void OnCmdRiseTypeRadio();
	afx_msg void OnCmdWindTypeCheck();
	afx_msg void OnBtnAutoCalc();
	afx_msg void OnChangeSpeedEdit();
	afx_msg void OnChangeSimpleSpeedEdit();
	afx_msg void OnChangeRoofHeightEdit();
	afx_msg void OnChangeSimpleRoofHeightEdit();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMWINDITEMKBC2015_H__)
