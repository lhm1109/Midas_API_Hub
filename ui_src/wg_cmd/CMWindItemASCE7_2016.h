#if !defined(__CMWINDITEMASCE7_2016_H__)
#define __CMWINDITEMASCE7_2016_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMWindItemASCE7_2016.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"
using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMWindItemASCE7_2016 dialog
class CCMWindItemDlg;

class CCMWindItemASCE7_2016 : public CChildDialog
{
	// Construction
public:
	CCMWindItemASCE7_2016(CWnd* pParent = NULL, int nCode = 0);   // standard constructor
	
	void SetData2Dlg(T_WIND_ASCE7_16& data, BOOL bModify);
	BOOL SetDlg2Data(T_WIND_ASCE7_16& data);
	
// 	void SetIBCType(int nType);
	
	// Dialog Data
	//{{AFX_DATA(CCMWindItemASCE7_2016)
	enum { IDD = IDD_ETC_WIND_ITEM_ASCE7_16 };
	MComboBox	m_cobxCategory2;
	CStatic  	m_unitSpeed;
	CEditUnit	m_editSpeed;
	CTextUnit	m_unitRoofH;
	CEditUnit	m_editRoofH;
	MButton	  m_chkLoadEval;
	CEditUnit	m_editGustGy;
	CEditUnit	m_editGustGx;
	CEditUnit	m_editForceCoef;
	CEditUnit	m_editGFactorX;
	CEditUnit	m_editGFactorY;
	CEditUnit	m_editDFactorX;
	CEditUnit	m_editDFactorY;
	MComboBox	m_cobxCategory;
	CFoldGroupBox_Cross m_grpWindParam;
	//}}AFX_DATA
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindItemASCE7_2016)
public:
	virtual BOOL DestroyWindow();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
protected:
	void InitDefaultData();
	void InitControls();
// 	void InitIFactorCombo();
	void InitCategoryCombo();
// 	void AlignControls();
	void Data2Dlg();
	BOOL Dlg2Data();
// 	void SetCtrlByIBCType();
	
private:
	int m_nCode;
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
	T_WIND_ASCE7_16  m_Data;
	
	CArray<UINT, UINT> m_aCtrlStructType;
	CArray<UINT, UINT> m_aCtrlSimple;
	CArray<UINT, UINT> m_aCtrlAnalytic;
	
	CArray<UINT, UINT> m_aCtrlForceCoef;
	
	CArray<UINT, UINT> m_aCtrlExRadio;
	CArray<UINT, UINT> m_aCtrlEyRadio;
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CCMWindItemASCE7_2016)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdAnalRigidFlexRd();
	afx_msg void OnCmdAnalGustButton();
	afx_msg void OnCmdAnalLoadevalChk();
	afx_msg void OnBtnTopography();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMWINDITEMASCE7_2016_H__)
