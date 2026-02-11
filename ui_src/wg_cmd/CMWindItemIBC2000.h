#if !defined(__CMWINDITEMIBC2000_H__)
#define __CMWINDITEMIBC2000_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemIBC2000.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MButton.h"
/////////////////////////////////////////////////////////////////////////////
// CCMWindItemIBC2000 dialog
class CCMWindItemDlg;

class CCMWindItemIBC2000 : public CChildDialog
{
	// Construction
public:
	CCMWindItemIBC2000(CWnd* pParent = NULL);   // standard constructor
	
	void SetData2Dlg(T_WIND_IBC2000& data, BOOL bModify);
	BOOL SetDlg2Data(T_WIND_IBC2000& data);
	
	// Dialog Data
	//{{AFX_DATA(CCMWindItemIBC2000)
	enum { IDD = IDD_ETC_WIND_ITEM_IBC2000 };
	CBCGPStatic  	m_unitSpeed2;
	CEditUnit	m_editSpeed2;
	MComboBox	m_cobxIFactor2;
	MComboBox	m_cobxCategory2;
	MButton	  m_chkTopographic;
	CBCGPStatic  	m_unitSpeed;
	CEditUnit	m_editSpeed;
	CTextUnit	m_unitRoofH;
	CEditUnit	m_editRoofH;
	MButton	  m_chkLoadEval;
	MComboBox	m_cobxIFactor;
	CTextUnit	m_unitHillLeng;
	CEditUnit	m_editHillLeng;
	MComboBox	m_cobxHillShape;
	CTextUnit	m_unitHHeight;
	CEditUnit	m_editHHeight;
	CEditUnit	m_editGustGy;
	CEditUnit	m_editGustGx;
	CEditUnit	m_editForceCoef;
	CEditUnit	m_editDFactor;
	CTextUnit	m_unitCBDistance;
	CEditUnit	m_editCBDistance;
	MComboBox	m_cobxCategory;
	MComboBox	m_cobxBLocation;
		CFoldGroupBox_Cross m_grpWindParam;
	//}}AFX_DATA
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemIBC2000)
public:
	virtual BOOL DestroyWindow();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
protected:
	void InitDefaultData();
	void InitControls();
	void InitIFactorCombo();
	void InitCategoryCombo();
	void InitHillShapeCombo();
	void InitBLocationCombo();
	void AlignControls();
	void Data2Dlg();
	BOOL Dlg2Data();
	
private:
	CCMWindItemDlg* m_pParent;
	T_WNAT_D m_WnatD;

public:
	void SetParentMgr(CCMWindItemCodeMgr* pCMWIndItemCodeMagr);
	void GetWnatData(T_WNAT_D& WnatD) { WnatD.arWnatStor.Copy(m_WnatD.arWnatStor); }
	
protected:
	// IBC 2000
	double m_dBXIBC2000;
	double m_dBYIBC2000;
	double m_dXNaturalIBC2000; 
	double m_dYNaturalIBC2000; 
	double m_dDampingIBC2000; 
	
	BOOL m_bInit;
	BOOL m_bModify;
	T_WIND_IBC2000  m_Data;
	
	CArray<UINT, UINT> m_aCtrlPrecedure;
	CArray<UINT, UINT> m_aCtrlStructType;
	CArray<UINT, UINT> m_aCtrlSimple;
	CArray<UINT, UINT> m_aCtrlAnalytic;
	
	CArray<UINT, UINT> m_aCtrlForceCoef_user;
	CArray<UINT, UINT> m_aCtrlForceCoef_auto;
	CArray<UINT, UINT> m_aCtrlForceCoef;
	CArray<UINT, UINT> m_aRdoForceCoefCalcType;
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CCMWindItemIBC2000)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdSimpleAnalRadio();
	afx_msg void OnSelchangeCmdAnalCategoryCombo();
	afx_msg void OnCmdAnalRigidFlexRd();
	afx_msg void OnCmdAnalGustButton();
	afx_msg void OnCmdAnalLoadevalChk();
	afx_msg void OnCmdAnalTopoCheck();
	afx_msg void OnSelchangeCmdAnalHillshapeCombo();
	afx_msg void OnCmdChangeRdo_ForceCoefCalcType();
	afx_msg void OnBtnAutoCalc();
		afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMWINDITEMIBC2000_H__)
