#if !defined(__CMWINDITEMKDS2021_H__)
#define __CMWINDITEMKDS2021_H__

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
// CCMWindItemKDS2021 dialog
class CCMWindItemDlg;

class CCMWindItemKDS2021 : public CChildDialog
{
	// Construction
public:
	CCMWindItemKDS2021(CWnd* pParent = NULL);   // standard constructor
	
	void SetData2Dlg(T_WIND_KDS2021& data, BOOL bModify, T_KEY KeyWAorWT);
	BOOL SetDlg2Data(T_WIND_KDS2021& data);
	
	// Dialog Data
	//{{AFX_DATA(CCMWindItemKDS2021)
	enum { IDD = IDD_ETC_WIND_ITEM_KDS2021 };
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
	MButton	m_chkVortex;
	MButton	m_chkLShape;
	CEditUnit	m_editDirectFactorX;
	CEditUnit	m_editDirectFactorY;

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
	void InitStructureTypeCombo();
	void InitCeFactorCombo();
	void AlignControls();
	void Data2Dlg();
	BOOL Dlg2Data();
	void GetMassXYValue(double dVibBeta, double& dMassX, double& dMassY, double& dMassXY, double& dTotal,
						double& dMass, double& dGustTotal, double& dHmax);
	void UpdateGridData(int nMethod, int nCheck);
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

	double m_dBLxKDS2021;		
	double m_dBLyKDS2021;	
	double m_dMinBXKDS2021;
	double m_dMinBYKDS2021;
	double m_dAreadKDS2021;	
	double m_dFreqXKDS2021;	
	double m_dFreqYKDS2021;	
	double m_dDampingKDS2021;

	double m_dBX;
	double m_dBY;
	double m_dFreqX;
	double m_dFreqY;
	double m_dFreqT;
	double m_dMassX;
	double m_dMassY;
	double m_dMoment;
	double m_dZf;
	double m_dTotalM;
	double m_dVibration;
	
	double m_dGustM;
	double m_dGustMx;
	double m_dGustMy;
	double m_dGustVibration;

	double m_dAmFactorAlong[2];
	double m_dAmFactorAcross[2];
	double m_dAmFactorTorsional[2];

	BOOL m_bInit;
	BOOL m_bModify;
	T_KEY m_KeyWAorWT;
	T_WIND_KDS2021  m_Data;
	
	CArray<UINT, UINT> m_aCtrlMethod;
	CArray<UINT, UINT> m_aCtrlVortex;
	CArray<UINT, UINT> m_aCtrlSimple;
	CArray<UINT, UINT> m_aCtrlGeneral;
	CArray<UINT, UINT> m_aCtrlType;
	CArray<UINT, UINT> m_aCtrlForceCoef_user;
	CArray<UINT, UINT> m_aCtrlForceCoef_auto;
	CArray<UINT, UINT> m_aCtrlForceCoef;
	CArray<UINT, UINT> m_aCtrlRiseType;
	CArray<UINT, UINT> m_aCtrlRise;
	CArray<UINT, UINT> m_aRdoForceCoefCalcType;
	
	double m_dStoryHMax;
	CArray<double, double&> m_StoryHeight;
	CArray<double, double&> m_StoryMass;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemKDS2021)
public:
	virtual BOOL DestroyWindow();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CCMWindItemKDS2021)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdMethodRadio();
	afx_msg void OnCmdVortexSheddingBtn();
	afx_msg void OnCmdLShapeBtn();
	afx_msg void OnCmdGeneralBtn();
	afx_msg void OnCmdVibrationBtn();
	afx_msg void OnCmdGustFactorBtn();
	afx_msg void OnCmdLoadEvalCheck();
	afx_msg void OnCmdTopoEffectCheck();
	afx_msg void OnCmdChangeRdo_ForceCoefCalcType();
	afx_msg void OnCmdRiseTypeRadio();
	afx_msg void OnCmdWindTypeCheck();
	afx_msg void OnCmdLShapeCheck();
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

#endif // !defined(__CMWINDITEMKDS2021_H__)
