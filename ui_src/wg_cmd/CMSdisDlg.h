#if !defined(__WG_CMD_CMSDISDLG_H__)
#define __WG_CMD_CMSDISDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThrdDisp.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CCMSdisDlg dialog

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

#include "HeaderPre.h"

class CIGenSDCDBManager;
class __MY_EXT_CLASS__ CCMSdisDlg : public CDialogMove
{
	// Construction
public:
	CCMSdisDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMSdisDlg();

	T_SDIS_K m_key;
	T_SDIS_D m_data;
	BOOL     m_bModify;
	void     SetParamData(T_SDIS_K key, T_SDIS_D data) { m_key=key; m_data=data; m_bModify=TRUE; }

	// Dialog Data
	//{{AFX_DATA(CCMSdisDlg)
	enum { IDD = IDD_ETC_SDVP_ISOLATOR_DLG };

	CString	m_Name;
	CString	m_Desc;
	int   m_nDevType; // 0:LRB, 1:NRB, 2:HDRI, 3:SB
	int		m_nInputMethod;
	int		m_nHysProp;
	mit::frx::MComboBox m_cmbCompany;
	mit::frx::MComboBox	m_cmbProductName;
	mit::frx::MComboBox m_cmbTypeNumber;
	mit::frx::MComboBox m_cmbNSS;
	mit::frx::MComboBox m_cmbQd;

	CEditUnit	m_edtAr   ;
	CTextUnit	m_untAr   ;
	CEditUnit	m_edtTr   ;
	CTextUnit	m_untTr   ;
	CEditUnit	m_edtKe   ;
	CTextUnit	m_untKe   ;
	CEditUnit	m_edtTauk ;
	CEditUnit	m_edtTauq ;
	CEditUnit	m_edtTheta;
	CEditUnit	m_edtAs   ;
	CTextUnit	m_untAs   ;  
	CEditUnit	m_edtPi   ;
	CTextUnit	m_untPi   ;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSdisDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	void InitCtrl();
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL OnExecute();
	void SetBmpImage();
	void UpdateCtrls();
	void UpdateCompany();
	void UpdateProductName();
	void UpdateTypeNumber();
	int  GetDBTypeFromDevTypeRdo(int nDevType);
	int  GetDeviceTypeFromDevTypeRdo(int nDevType);

	mit::frx::MillustViewer m_ImgViewer;
	CIGenSDCDBManager* m_pIGenSDCDB;
	BOOL m_bOnInit;
	CArray<UINT,UINT> m_aCtrls_LRBNRBProp;
	CArray<UINT,UINT> m_aCtrls_SLDProp;
	CArray<UINT,UINT> m_aCtrlID_DBValue_ReadOnly; // DB에서 가져온 데이터인 경우 읽기전용되는 컨트롤

	// Generated message map functions
	//{{AFX_MSG(CCMSdisDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdOK();
	afx_msg void OnCmdApply();  
	afx_msg void OnNSSCmb();
	afx_msg void OnDevTypeRdo();
	afx_msg void OnHysModelRdo();
	afx_msg void OnHDPBtn();
	afx_msg void OnVDPBtn();
	afx_msg void OnCmdInputMethodRdo();
	afx_msg void OnSelChangeCompany    ();
	afx_msg void OnSelChangeProductName();
	afx_msg void OnSelChangeTypeNumber ();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__WG_CMD_CMSDISDLG_H__)
