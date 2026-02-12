#if !defined(__WG_CMD_CMSDVEDLG_H__)
#define __WG_CMD_CMSDVEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThrdDisp.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CCMSdveDlg dialog

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

#include "HeaderPre.h"

class CIGenSDCDBManager;
class __MY_EXT_CLASS__ CCMSdveDlg : public CDialogMove
{
// Construction
public:
	CCMSdveDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMSdveDlg();

	T_SDVE_K m_key;
	T_SDVE_D m_data;
	BOOL     m_bModify;
	void     SetParamData(T_SDVE_K key, T_SDVE_D data) { m_key=key; m_data=data; m_bModify=TRUE; }
			
// Dialog Data
	//{{AFX_DATA(CCMSdveDlg)
	enum { IDD = IDD_ETC_SDVP_VISCOELA_DLG };

	CString	m_Name;
	CString	m_Desc;
	int		m_nInputMethod;
	mit::frx::MComboBox	m_cmbCompany;
	mit::frx::MComboBox	m_cmbProductName;
	mit::frx::MComboBox m_cmbTypeNumber;
	mit::frx::MComboBox m_cmbMatlType;
	mit::frx::MComboBox m_cmbDir;
	//CComboBox m_cmbCTC     ;
	
	CEditUnit	m_edtSA;
	CTextUnit	m_untSA;
	CEditUnit	m_edtTh;
	CTextUnit	m_untTh;
	CEditUnit	m_edtMp;
	CTextUnit	m_untMp;
	CEditUnit	m_edtLD;
	CTextUnit	m_untLD;
	BOOL      m_bMS;
	CEditUnit	m_edtMS;
	CTextUnit	m_untMS;
	CEditUnit	m_edtES;
	CTextUnit	m_untES;
	CEditUnit	m_edtED;
	CTextUnit	m_untED;
	BOOL      m_bKF;
	CEditUnit	m_edtKF;
	CTextUnit	m_untKF;
	CEditUnit	m_edtFq;
	CEditUnit	m_edtSF;
	CEditUnit	m_edtDF;
	CEditUnit	m_edtRT;

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSdveDlg)
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

	mit::frx::MillustViewer m_ImgViewer;
	CIGenSDCDBManager* m_pIGenSDCDB;
	BOOL m_bOnInit;
	CArray<UINT,UINT> m_aCtrlID_DBValue_ReadOnly; // DB에서 가져온 데이터인 경우 읽기전용되는 컨트롤
	CArray<UINT,UINT> m_aCtrlID_DBValue_Disabled; // DB에서 가져온 데이터인 경우 읽기전용되는 컨트롤

	// Generated message map functions
	//{{AFX_MSG(CCMSdveDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdOK();
	afx_msg void OnCmdApply();  
	afx_msg void OnMatlTypeCmb();
	afx_msg void OnClickChkBtn();
	afx_msg void Update3MPropEdt();
	afx_msg void OnCmdInputMethodRdo();
	afx_msg void OnSelChangeCompany    ();
	afx_msg void OnSelChangeProductName();
	afx_msg void OnSelChangeTypeNumber ();
	afx_msg void OnCmdMountStiffChk();
	afx_msg void OnCmdKineticFricChk();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__WG_CMD_CMSDVEDLG_H__)
