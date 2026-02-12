#if !defined(__WG_CMD_CMSDVIDLG_H__)
#define __WG_CMD_CMSDVIDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThrdDisp.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CCMSdviDlg dialog

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MillustViewer.h"

#include "HeaderPre.h"
class CCMSdviGrid;
class CIGenSDCDBManager;
class __MY_EXT_CLASS__ CCMSdviDlg : public CDialogMove
{
// Construction
public:
	CCMSdviDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMSdviDlg();

	T_SDVI_K m_key;
	T_SDVI_D m_data;
	BOOL     m_bModify;
	void     SetParamData(T_SDVI_K key, T_SDVI_D data) { m_key=key; m_data=data; m_bModify=TRUE; }
			
// Dialog Data
	//{{AFX_DATA(CCMSdviDlg)
	enum { IDD = IDD_ETC_SDVP_VISDAMP_DLG };
	
	mit::frx::MComboBox	m_cmbDeviceType;
	mit::frx::MComboBox m_cmbCompany;
	mit::frx::MComboBox	m_cmbProductName;
	mit::frx::MComboBox m_cmbTypeNumber;
	CString	m_Name;
	CString	m_Desc;
	int		m_nInputMethod;
	int		m_nDamperType;
	int		m_nDashpotType;
	int		m_nInputType;
	int   m_nDOF;
	mit::frx::MEdit	m_edtCe;
	mit::frx::MEdit	m_edtP1;
	mit::frx::MEdit	m_edtV1;
	mit::frx::MEdit	m_edtC1;
	mit::frx::MEdit	m_edtA1;
	mit::frx::MEdit	m_edtKb;
	int		m_nInputType_Exfn;
	mit::frx::MEdit	m_edtPy_Exfn;
	mit::frx::MEdit	m_edtVy_Exfn;
	mit::frx::MEdit	m_edtDe_Exfn;
	mit::frx::MEdit	m_edtDc_Exfn;
	mit::frx::MEdit	m_edtCe_Exfn;
	BOOL 	m_bCe_Exfn  ;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSdviDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitCtrl();
	void DataInit();
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL OnExecute();
	void EnableDisableCtrls();
	void UpdateProperties();
	void SetDamperBmpImage() ;
	void SetDashpotBmpImage();
	void UpdateUnitText();
	void SetAutoCalcV1();
	void SetAutoCalcC1a1();
	void UpdateDeviceType();
	void UpdateCompany();
	void UpdateProductName();
	void UpdateTypeNumber();
	void SetAutoCalcExfnVal();
	int  GetDeviceTypeInt(CString strDeviceType);

    void InitCtrlTexts();

	CCMSdviGrid* m_pGrid;
	CIGenSDCDBManager* m_pIGenSDCDB;
	mit::frx::MillustViewer m_ImgDamper;
	mit::frx::MillustViewer m_ImgDashpot;
	mit::frx::MillustViewer m_ImgExfnP1;
	mit::frx::MillustViewer m_ImgExfnP2;
	BOOL m_bOnInit;
	CArray<UINT,UINT> m_aCtrlID_DBValue_ReadOnly; // DB에서 가져온 데이터인 경우 읽기전용되는 컨트롤
	CArray<UINT,UINT> m_aCtrlID_DBValue_Disabled; // DB에서 가져온 데이터인 경우 비활성화되는 컨트롤
	
	CArray<UINT,UINT> m_aCtrlID_ShowElastic; // Dashpot type : Linear Elastic Type, Elastic Bilinear Type 일 때 보임
	CArray<UINT,UINT> m_aCtrlID_ShowExpFunc; // Dashpot type : Exponential Function Type 일 때 보임
	
	// Generated message map functions
	//{{AFX_MSG(CCMSdviDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdCancel();
	afx_msg void OnCmdOK();
	afx_msg void OnCmdApply();
	
	afx_msg void OnCmdInputMethodRdo();
	afx_msg void OnCmdDamperTypeRdo();
	afx_msg void OnCmdDashopotTypeRdo();
	afx_msg void OnCmdInputTypeRdo();
	afx_msg void OnCmdInputTypeRdo_Exfn();
	afx_msg void OnCmdDOFRdo();
	afx_msg void OnCmdCeEdtChanged();
	afx_msg void OnSelChangeDeviceType ();
	afx_msg void OnSelChangeCompany    ();
	afx_msg void OnSelChangeProductName();
	afx_msg void OnSelChangeTypeNumber ();
	afx_msg void OnCmdExfnDEEdtChanged();
		//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__WG_CMD_CMSDVIDLG_H__)
