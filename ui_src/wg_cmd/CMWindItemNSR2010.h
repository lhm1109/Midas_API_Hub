#if !defined(__CMWINDITEMNSR2010_H__)
#define __CMWINDITEMNSR2010_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemIBC2012.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"
using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMWindItemNSR2010 dialog
class CCMWindItemDlg;

class CCMWindItemNSR2010 : public CChildDialog
{
	// Construction
public:
	CCMWindItemNSR2010(CWnd* pParent = NULL);   // standard constructor

	void SetData2Dlg(T_WIND_NSR2010& data, BOOL bModify);
	BOOL SetDlg2Data(T_WIND_NSR2010& data);

	// Dialog Data
	//{{AFX_DATA(CCMWindItemNSR2010)
	enum { IDD = IDD_ETC_WIND_ITEM_NSR2010 };
	MComboBox	m_cobxCategory2;
	CBCGPStatic  	m_unitSpeed;
	CEditUnit	m_editSpeed;
	CTextUnit	m_unitRoofH;
	CEditUnit	m_editRoofH;
	MButton	  m_chkLoadEval;
	MComboBox	m_cobxIFactor;
	CEditUnit	m_editGustGy;
	CEditUnit	m_editGustGx;
	CEditUnit	m_editForceCoef;
	CEditUnit	m_editDFactorX;
	CEditUnit	m_editDFactorY;
	MComboBox	m_cobxCategory;
	CFoldGroupBox_Cross m_grpWindParam;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemNSR2010)
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
	void Data2Dlg();
	BOOL Dlg2Data();

private:
	int m_nType; // 16=IBC2009, 17=IBC2012
	CCMWindItemDlg* m_pParent;
	T_WNAT_D m_WnatD;

public:
	void SetParentMgr(CCMWindItemCodeMgr* pCMWIndItemCodeMagr);
	void GetWnatData(T_WNAT_D& WnatD) { WnatD.arWnatStor.Copy(m_WnatD.arWnatStor); }

protected:
	// IBC 2012
	double m_dBXIBC2012;
	double m_dBYIBC2012;
	double m_dXNaturalIBC2012; 
	double m_dYNaturalIBC2012; 
	double m_dDampingIBC2012; 

	BOOL m_bInit;
	BOOL m_bModify;
	T_WIND_NSR2010  m_Data;

	CArray<UINT, UINT> m_aCtrlStructType;

	CArray<UINT, UINT> m_aCtrlForceCoef_user;
	CArray<UINT, UINT> m_aCtrlForceCoef_auto;
	CArray<UINT, UINT> m_aCtrlForceCoef;
	CArray<UINT, UINT> m_aRdoForceCoefCalcType;

	CArray<UINT, UINT> m_aCtrlExRadio;
	CArray<UINT, UINT> m_aCtrlEyRadio;

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMWindItemNSR2010)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdAnalRigidFlexRd();
	afx_msg void OnCmdAnalGustButton();
	afx_msg void OnCmdAnalLoadevalChk();
	afx_msg void OnCmdChangeRdo_ForceCoefCalcType();
	afx_msg void OnBtnAutoCalc();
	afx_msg void OnBtnTopography();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMWINDITEMNSR2010_H__)
