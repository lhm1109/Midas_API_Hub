#if !defined(__CMWINDITEMKBC2009_H__)
#define __CMWINDITEMKBC2009_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemKBC2009.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"
using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMWindItemKBC2009 dialog
class CCMWindItemDlg;

class CCMWindItemKBC2009 : public CChildDialog
{
	// Construction
public:
	CCMWindItemKBC2009(CWnd* pParent = NULL);   // standard constructor
	
	void SetData2Dlg(T_WIND_KBC2009& data, BOOL bModify);
	BOOL SetDlg2Data(T_WIND_KBC2009& data);
	
	// Dialog Data
	//{{AFX_DATA(CCMWindItemKBC2009)
	enum { IDD = IDD_ETC_WIND_ITEM_KBC2009 };
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
	//}}AFX_DATA
	
protected:
	void InitDefaultData();
	void InitCategoryCombo();
	void InitImportFactorCombo();
	void Data2Dlg();
	BOOL Dlg2Data();

private:
	CCMWindItemDlg* m_pParent;
	T_WNAT_D m_WnatD;

public:
	void GetWnatData(T_WNAT_D& WnatD) { WnatD.arWnatStor.Copy(m_WnatD.arWnatStor); }

protected:
	double m_dBXKBC2009 ;		
	double m_dBYKBC2009 ;		
	double m_dFreqXKBC2009;	
	double m_dFreqYKBC2009;	
	double m_dDampingKBC2009;
	
	BOOL m_bInit;
	BOOL m_bModify;
	T_WIND_KBC2009  m_Data;
	
	CArray<UINT, UINT> m_aCtrlType;
	CArray<UINT, UINT> m_aCtrlForceCoef_user;
	CArray<UINT, UINT> m_aCtrlForceCoef_auto;
	CArray<UINT, UINT> m_aCtrlForceCoef;
	CArray<UINT, UINT> m_aRdoForceCoefCalcType;
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemKBC2009)
public:
	virtual BOOL DestroyWindow();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CCMWindItemKBC2009)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdGustFactorBtn();
	afx_msg void OnCmdLoadEvalCheck();
	afx_msg void OnCmdTopoEffectCheck();
	afx_msg void OnCmdChangeRdo_ForceCoefCalcType();
	afx_msg void OnBtnAutoCalc();
		afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMWINDITEMKBC2009_H__)
