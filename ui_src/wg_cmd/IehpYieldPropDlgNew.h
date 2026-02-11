#if !defined(AFX_IEHPYIELDPROPDLGNEW_H__151F169E_46CE_4706_8C1E_91D3BE31E4F8__INCLUDED_)
#define AFX_IEHPYIELDPROPDLGNEW_H__151F169E_46CE_4706_8C1E_91D3BE31E4F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IehpYieldPropDlgNew.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "../MIT_frx/MTabCtrl.h"
#include "../MIT_frx/MComboBox.h"
#include "../MIT_frx/MillustViewer.h"
#include "../MIT_frx/MButton.h"

using namespace mit::frx;

#include "IehpYieldPropGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CIehpYieldPropDlgNew dialog
//class CIehpYieldPropGrid;
//class CIehpYieldPropGraph;

class CIehpYieldPropDlgNew : public CDialogMove
{
// Construction
public:
	CIehpYieldPropDlgNew(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIehpYieldPropDlgNew();

	void SetIehpPropData(T_IEHP_D iehpD, T_IEHP_YLDSUR data, T_IEHP_YLDSUR Subdata, int nSectType, T_IEHG_K AssignedIehgKey, 
											 BOOL bAssignProperties=FALSE, BOOL bUseMz=TRUE, BOOL bUnable1stCtrl=FALSE);
	void SetIJDataExist(BOOL bExistIJData, int nCurType);
	void Set3rdControl(BOOL bUse3rdCtrl);
// Dialog Data
	//{{AFX_DATA(CIehpYieldPropDlgNew)
	enum { IDD = IDD_ETC_IEHP_YLD_PROP };
	enum TABSTYLE  {LOCATION_NONE = 0,I_LOCATION=1,J_LOCATION=2, I_J_LOCATION=3, CENTER_LOCATION=4};
	enum MyEnum{D_TAB_DEFALUT = 0, D_TAB_SUB = 1};

	mit::frx::MTabCtrl m_Tab;	
	MComboBox	m_cmbPlot;
	CEditUnit	m_edtPC0;
	int		    m_nPMInputType;
	int		    m_nYLDInputType;
	CString	  m_strAlpha;
	CString	m_str1Gamma;
	CString	m_str2Gamma;
	CString	m_str3Gamma;
	int		m_nPmcv;
	int		m_nValueTypeSymmetry;
	//}}AFX_DATA
	CEditUnit m_edtMC0[2];
	CEditUnit m_edtPC[2];
	CEditUnit m_edtPCB[2];
	CEditUnit m_edtMC[2];
	CEditUnit m_edtPY[2];
	CEditUnit m_edtPYB[2];
	CEditUnit m_edtMY[2];
	CEditUnit m_edtPU[2];
	CEditUnit m_edtPUB[2];
	CEditUnit m_edtMU[2];
	CString   m_strBetay1st[2];
	CString   m_strBetay2nd[2];
	CString   m_strBetay3rd[2];
	CString   m_strBetaz1st[2];
	CString   m_strBetaz2nd[2];
	CString   m_strBetaz3rd[2];

	BOOL m_bAssignProperties;
	BOOL m_bExistIJData;
	BOOL m_bUnable1stCtrl;
	BOOL m_bUse3rdCtrl;

	MButton m_chkShowValue;
	BOOL   m_bShowValueSet;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIehpYieldPropDlgNew)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	MillustViewer m_wndLegendPicture;
	MillustViewer m_wndFormulaPicture;

public:
	T_ELEM_K m_ElemK;
	T_IEHP_D IehpData;
	T_IEHP_YLDSUR m_MainData;
	T_IEHP_YLDSUR m_SubData;
// Implementation
protected:
	
	CString  m_strName;
	// 0: RC/SRC(encased) , 1: Steel/SRC(filled)
	int m_nMessage; // 0: Steel, 1: Concrete, 2: SRC(filled) 3: SRC(encased) 4: User
	BOOL     m_bConc;
	CArray<UINT, UINT> m_arUser;
	CArray<UINT, UINT> m_arPMInput;
	CArray<UINT, UINT> m_arYLDInput;
	CArray<UINT, UINT> m_aAssignHinge;
	//CArray<UINT, UINT> m_ar1stCtrl;
	int m_nSectType; // 0: 적용할수 없는 단면, 1: 일반, 2: value type
	BOOL        m_bUseMz;    // FALSE : Z-Axis 가 필요없는 경우임.

	double dPC[2];
	double dPCB[2];
	double dMC[2];
	double dPY[2];
	double dPYB[2];
	double dMY[2];
	double dPU[2];
	double dPUB[2];
	double dMU[2];

	double dP1st[2][11];     // y-axis P/PC  z-axis P/PC  (1st P-M Interaction Curve)
	double dM1st[2][11];     // y-axis M/MCy z-axis M/MCy (1st P-M Interaction Curve)
	double dP2nd[2][11];     // y-axis P/PC  z-axis P/PC  (2nd P-M Interaction Curve)
	double dM2nd[2][11];     // y-axis M/MCy z-axis M/MCy (2nd P-M Interaction Curve)
	double dP3rd[2][11];     // y-axis P/PC  z-axis P/PC  (3rd P-M Interaction Curve)
	double dM3rd[2][11];     // y-axis M/MCy z-axis M/MCy (3rd P-M Interaction Curve)

	// Generated message map functions
	//{{AFX_MSG(CIehpYieldPropDlgNew)
	afx_msg void OnPMInputType();
	afx_msg void OnYLDInputType();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangePlotComb();
	virtual void OnOK();
	afx_msg void OnChangeEditBox();
	afx_msg void OnKillFocusEditBox();
	afx_msg void OnPmcvGrid();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void WndMessageCtrl();
	void SetInitData();
	void Data2Dlg();
	void PMData2Dlg(T_IEHP_YLDSUR& Data);
	void YLDData2Dlg(T_IEHP_YLDSUR& Data);
	void Dlg2Data(BOOL bUpdateRatios);
	BOOL CheckDataCtrl(int nTabID, BOOL bMsg=TRUE);
	BOOL CheckErrorNameDataVar(LPCTSTR lpszData, LPCTSTR lpszVar, BOOL bMsg);

	void InitCtrl();
	void InitCtrl_Unable1stCtrl();
	BOOL InitTabCtrl();
	BOOL ChangeDlgCtrls();
	void ChangeTabCtrl();
	void Init3rdYieldCtrl();
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnValueTypeSymmetry();
	afx_msg void OnShowValueChk();

protected:
	// Graph operations
	CIehpYieldPropGraph m_Draw;
	void InitGraph();
	
public:
	void UpdateGraph(int nSel);

	void Data2TabDlg(int nTabID, T_IEHP_YLDSUR& Data);
	BOOL TabDlg2Data(int nTabID, T_IEHP_YLDSUR& Data, BOOL bUpdateRatios);
private:
	CIehpYieldPropGrid* m_wndGrid;
	
	void BackupDisplayData();
	void LoadDisplayBackupData();
	CArray<double, double> m_aBackupArr;

};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IEHPYIELDPROPDLGNEW_H__151F169E_46CE_4706_8C1E_91D3BE31E4F8__INCLUDED_)
