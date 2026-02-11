#if !defined(AFX_SEISMIC_LOAD_PROPERTY_H__INCLUDED_)
#define AFX_SEISMIC_LOAD_PROPERTY_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeismicLoadProperty.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\mit_frx\MComboBox.h"
#include "MyChildDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CSeismicLoadProperty dialog
#include "HeaderPre.h"      /////////


class __MY_EXT_CLASS__ CSeismicLoadProperty : public CDialogMove
{
// Construction
public:
	CSeismicLoadProperty(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSeismicLoadProperty();


// Dialog Data
	//{{AFX_DATA(CSeismicLoadProperty)
	enum { IDD = IDD_ETC_EP_PARAM_OF_SEISMIC_LOAD };
	mit::frx::MEdit	m_editName;
	mit::frx::MComboBox   m_cobxCodeName;
	BOOL m_bCivil;
	//}}AFX_DATA
public:
	void SetParamData(T_POSL_K Key, T_POSL_D &ParamData);

	T_POSL_K m_Key, m_OldKey;
	T_POSL_D m_Data;
	BOOL m_bModify;
	int m_nCurrentDlg;
	int m_nCurrentDlgSeq;
	CMap <int, int, CMyChildDialog*, CMyChildDialog*> m_mapChildDlg;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeismicLoadProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void Data2Dlg();
	BOOL Dlg2Data();

	void InitCobxCodeName();
	
	void OnSelchangePoslSpectrumCmb();
	void UpdateCurDlg();
	void AlignDialog();

protected:
	T_POSL_D* m_pParamData;
	CArray<UINT, UINT> m_aCtrl_HideOnCivil;
	CArray<UINT, UINT> m_aCtrl_ShowOnCivil;
	CArray<int, int>         m_aCodeSeq;
	CArray<int, int>         m_aLockSeq;
	CArray<CString, LPCTSTR> m_aCodeName;
	CArray<UINT, UINT> m_aCtrlMethod;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSeismicLoadProperty)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	
	afx_msg void OnCmdMethodChange();

	afx_msg void OnCmdMethodBtn();
	afx_msg void OnCmdApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#include "HeaderPost.h"      /////////

#endif // !defined(AFX_SEISMIC_LOAD_PROPERTY_H__INCLUDED_)
