#if !defined(__WG_CMD_CMSDHYDLG_H__)
#define __WG_CMD_CMSDHYDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThrdDisp.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CCMSdhyDlg dialog

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\mit_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

#include "HeaderPre.h"

class CIGenSDCDBManager;
class __MY_EXT_CLASS__ CCMSdhyDlg : public CDialogMove
{
	// Construction
public:
	CCMSdhyDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMSdhyDlg();

	T_SDHY_K m_key;
	T_SDHY_D m_data;
	BOOL     m_bModify;
	void     SetParamData(T_SDHY_K key, T_SDHY_D data) { m_key=key; m_data=data; m_bModify=TRUE; }

	// Dialog Data
	//{{AFX_DATA(CCMSdhyDlg)
	enum { IDD = IDD_ETC_SDVP_HYSDAMP_DLG };

	CString	m_Name;
	CString	m_Desc;
	int		m_nInputMethod;
	mit::frx::MComboBox	m_cmbCompany;
	mit::frx::MComboBox	m_cmbProductName;
	mit::frx::MComboBox m_cmbTypeNumber;
	mit::frx::MComboBox m_cmbHysModel;
	mit::frx::MComboBox m_cmbNSS;

	CEditUnit	m_edtK0;
	CTextUnit	m_untK0;
	CEditUnit	m_edtP1;
	CTextUnit	m_untP1;
	CEditUnit	m_edtP2;
	CTextUnit	m_untP2;
	CEditUnit	m_edtKb;
	CTextUnit	m_untKb;

	CEditUnit	m_edtA1    ;
	CEditUnit	m_edtA2    ;
	CEditUnit	m_edtPhi   ;
	CEditUnit	m_edtLambda;
	CEditUnit	m_edtBeta  ;
	CEditUnit	m_edtTheta ;

	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSdhyDlg)
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

	mit::frx::MillustViewer m_ImgViewer_Curve;
	mit::frx::MillustViewer m_ImgViewer_MSS;
	CIGenSDCDBManager* m_pIGenSDCDB;
	BOOL m_bOnInit;
	CArray<UINT,UINT> m_aCtrlID_DBValue_ReadOnly; // DB에서 가져온 데이터인 경우 읽기전용되는 컨트롤
	CArray<UINT,UINT> m_aCtrlID_DBValue_Disabled; // DB에서 가져온 데이터인 경우 비활성화되는 컨트롤

	// Generated message map functions
	//{{AFX_MSG(CCMSdhyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdOK();
	afx_msg void OnCmdApply();  
	afx_msg void OnHysModelCmb();
	afx_msg void OnNSSCmb();
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

#endif // !defined(__WG_CMD_CMSDHYDLG_H__)
