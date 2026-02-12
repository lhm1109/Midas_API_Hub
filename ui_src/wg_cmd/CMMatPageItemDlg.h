#if !defined(AFX_CMMATPAGEITEMDLG_H__EBA9F0A2_1CCE_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMMATPAGEITEMDLG_H__EBA9F0A2_1CCE_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMatPageItemDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\CobxEpmt.h"

#include "..\mit_frx\MEdit.h"
#include "..\mit_frx\MButton.h"	
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MillustViewer.h"

using namespace mit::frx;
using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMMatPageItemDlg dialog

/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////

class __MY_EXT_CLASS__ CCMMatPageItemDlg : public CDialogMove
{
// Construction
public:
	CCMMatPageItemDlg(CDBDoc* pDoc, CWnd* pParent = NULL);   // standard constructor
	~CCMMatPageItemDlg() override = default;

// Attributes
public:
	int m_FixID;
	bool m_bWorkTab;

// Operations
public:
	void SetModifyData(T_MATL_K key, T_MATL_D& data);
	void SetMaterialID(UINT key);

// Dialog Data
	//{{AFX_DATA(CCMMatPageItemDlg)
	enum { IDD = IDD_CMD_MP_ITEM_DLG };
	MButton	m_wndOrtMDenChk;
	MButton	m_wndIsoConMDenChk;
	MButton	m_wndIsoStlMDenChk;
	CTextUnit m_wndUnitSpHeat;
	CTextUnit m_wndUnitHeatCo;
	CEditUnit m_wndSpHeat;
	CEditUnit m_wndHeatCo;
	CBCGPGroup	m_wndElasticityFrame;
	CBCGPGroup	m_wndIsoStlFrame; // CFoldGroupBox_Cross	m_wndIsoStlFrame;   
	CFoldGroupBox_Cross	m_wndIsoConFrame;
	CBCGPGroup	m_wndStlCodeFrame;
	CBCGPGroup m_wndOrtFrame;
	MButton	m_wndBtnApply;
	CTextUnit	m_wndIsoStlElaUnit;
	CTextUnit	m_wndIsoStlPoiUnit;
	CTextUnit	m_wndIsoStlThrUnit;
	CTextUnit	m_wndIsoStlDenUnit;
	CTextUnit	m_wndIsoStlMDenUnit;
	CTextUnit	m_wndIsoConElaUnit;
	CTextUnit	m_wndIsoConPoiUnit;
	CTextUnit	m_wndIsoConThrUnit;
	CTextUnit	m_wndIsoConDenUnit;	
	CTextUnit	m_wndIsoConMDenUnit;	
	CTextUnit	m_wndOrtElaUnit;
	CTextUnit	m_wndOrtPoiUnit;
	CTextUnit	m_wndOrtThrUnit;
	CTextUnit	m_wndOrtShrUnit;
	CTextUnit	m_wndOrtDenUnit;	
	CTextUnit	m_wndOrtMDenUnit;	
	CEditUnit	m_wndIsoStlEla;
	CEditUnit	m_wndIsoStlPoi;
	CEditUnit	m_wndIsoStlThr;
	CEditUnit	m_wndIsoStlDen;
	CEditUnit	m_wndIsoStlMDen;
	CEditUnit	m_wndIsoConEla;
	CEditUnit	m_wndIsoConPoi;
	CEditUnit	m_wndIsoConThr;
	CEditUnit	m_wndIsoConDen;
	CEditUnit	m_wndIsoConMDen;
	CEditUnit	m_wndOrtElaLx;
	CEditUnit	m_wndOrtElaLy;
	CEditUnit	m_wndOrtElaLz;
	CEditUnit	m_wndOrtThrLx;
	CEditUnit	m_wndOrtThrLy;
	CEditUnit	m_wndOrtThrLz;
	CEditUnit	m_wndOrtPoiLxy;
	CEditUnit	m_wndOrtPoiLxz;
	CEditUnit	m_wndOrtPoiLyz;
	CEditUnit	m_wndOrtShrLxy;
	CEditUnit	m_wndOrtShrLxz;
	CEditUnit	m_wndOrtShrLyz;
	CEditUnit	m_wndOrtDen;  
	CEditUnit	m_wndOrtMDen;  
	CCobxAutoFill	m_wndConcrName;
	MComboBox	m_wndConcrCode;
	MComboBox m_wndConcrSubCode;
	CCobxAutoFill	m_wndSteelName;
	MButton	m_wndOnlyStrandChk;
	CCobxAutoFill	m_wndProductName;
	MComboBox	m_wndSteelCode;
	MComboBox	m_wndType;
	MEdit	m_wndName;
	CFormulaEdit m_wndID;
	CCobxEpmt m_wndPlasticCmb;
	int   m_wndMatlTypeRdo;
	CEditUnit m_edtDampingRatio;
	MButton	m_wndIsoStlUserElaChk;
	MButton	m_wndIsoConUserElaChk;

	MComboBox m_cmbInelasConc;
	MComboBox m_cmbInelasCoverConc;
	MComboBox m_cmbInelasStl;
	
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMatPageItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void AlignControl();
	void SetUnitType();
	void SetTypeCombo();
	void SetInelasticCombo();
	void ShowDataToDlg();
	int  GetTypeIndex(CString csType);
	CString GetTypeCode(int nTypeIndex);
	void ChangeDlgCtrls();
	void SetFrameName(int nTypeIndex);
	void SetSteelCode(int nTypeIndex);
	void SetConcCode(int nTypeIndex);
	
	void SetCode(CComboBox* pCombo, CString csType, BOOL bSRC=FALSE);
	void SetProductCombo(CComboBox* pCombo, CString csType, CString csCodeName, CString csMatlName, CString csProd);
	void SetSubCode(CComboBox* pCombo, CString csType, CString csCode);
	void SetNameCombo(CComboBox* pCombo, CString &csType, CString &csCode);
	void ChangeBitmap(int nBitmap);
	void SetInelasticMatlCombo();

	void GetSteelData(CString csCode, CString csName, int nThermalUnit, T_MATL_ANALYSIS &AnalData);
	void GetConcData(CString csCode, CString csSubCode, CString csName, int nThermalUnit, T_MATL_ANALYSIS &AnalData);
	void GetAlumiData(CString csCode, CString csName, CString csProd, int nThermalUnit, T_MATL_ANALYSIS &AnalData);
	void GetTimberData(CString csCode, CString csName, int nThermalUnit, T_MATL_ANALYSIS& AnalData);
	void SetAnalData(int nTypeIndex, T_MATL_ANALYSIS &AnalData, CString &csCodeName);
	void SetTherData(const CString& csCodeName);
	void GetInelasticMatlCombo(const int nTypeIndex, T_KEY* pImfmK);

	void InitIsoAnalData();
	void InitOrtAnalData();
	BOOL GetAnalData(int nTypeIndex, T_MATL_ANALYSIS &AnalData);

	BOOL Dlg2Data();
	BOOL CheckCode();
	BOOL IsExistInCodeList(CArray<CString, CString&> &aList, CString &csCode);
	BOOL ConvertFahrCelsins(int nThermalUnit, double &dThermalCoef);
	void ChangeThermalUnit(int nThermalUnit);
	BOOL GetThermalData(int nTypeIndex, double& dThermal);

	void GetDefaultCode();
	void EnableDisableMDensity(BOOL bEnable);
	void EnableDisableUserEla();
	void EnableShowOnlyStrand();

	// MatlDB Export
	void ExportMatlDB();

protected:
	CDBDoc*   m_pDoc;
	BOOL      m_bModify;
	T_MATL_K  m_Key, m_OldKey;
	T_MATL_D  m_Data;

	CArray<UINT, UINT> m_aCtrlIso, m_aCtrlOrt;
	CArray<UINT, UINT> m_aCtrlStlCode, m_aCtrlConCode, m_aCtrlThermal;
	CArray<UINT, UINT> m_aCtrlIsoStlAnal, m_aCtrlIsoConAnal, m_aCtrlOrtAnal;  
	CArray<UINT, UINT> m_aCtrlMatlTypeRdoIso, m_aCtrlMatlTypeRdoOrt;
	CArray<UINT, UINT> m_aCtrlThermalSteel, m_aCtrlThermalConc;
	CArray<UINT, UINT> m_aCtrlInelasticMatl;
	//CBitmap* m_pBitmap;
	MillustViewer m_ImgMatl;
	CString  m_csDefSteelCode;
	CString  m_csDefConcrCode;
	CString  m_csDefConcrSubCode;
	CString  m_csDefAluminumCode;
	CString  m_csDefTimberCode;
	// m_csDefSteelCode, m_csDefConcrCode가 None일때 SRC에서 사용될 Default Code
	CString  m_csDefSteelCodeSRC; 
	CString  m_csDefConcrCodeSRC;
	CString  m_csDefConcrSubCodeSRC;

	// For AutoChangeName
	void SetAutoChangeNameFlag(CString csType, CString csName, CString csProd="");
	BOOL    m_bAutoChangeName;

	// Generated message map functions
	//{{AFX_MSG(CCMMatPageItemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeType();
	afx_msg void OnChangeProductCode();
	afx_msg void OnChangeSteelCode();
	afx_msg void OnChangeConcrCode();
	afx_msg void OnChangeConcrSubCode();
	afx_msg void OnChangeSteelName();
	afx_msg void OnChangeConcrName();
	virtual void OnOK();
	afx_msg void OnApply();
	afx_msg void OnChangeName();
	afx_msg void OnChangeID();
	afx_msg void OnMatlTypeChangeRdo();
	afx_msg void OnCmdMatlIsoConThrRd();
	afx_msg void OnCmdMatlIsoStlThrRd();
	afx_msg void OnCmdMatlIsoConMdenChk();
	afx_msg void OnCmdMatlIsoStlMdenChk();
	afx_msg void OnCmdMatlOrtMdenChk();
	afx_msg void OnCmdMatlIsoStlUserElaChk();
	afx_msg void OnCmdMatlIsoConUserElaChk();
	afx_msg void OnCmdOnlyStrandChk();
	afx_msg LRESULT OnBnClickedCmdMatlFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	afx_msg void OnInelasticBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////
#include "HeaderPost.h"      ////////
/////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMATPAGEITEMDLG_H__EBA9F0A2_1CCE_11D3_92DE_0000C0B0E6B3__INCLUDED_)
