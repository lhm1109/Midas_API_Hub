#if !defined(__CMELSPLSMATLADDDLG_H__)
#define __CMELSPLSMATLADDDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMElsPlsMatlAddDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\mit_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"	
#include "..\mit_frx\MComboBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsMatlAddDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMElsPlsMatlAddDlg : public CDialogMove
{
// Construction
public:
	CCMElsPlsMatlAddDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMElsPlsMatlAddDlg)
	enum { IDD = IDD_CMD_PROP_ELS_PLS_ADD };
	MEdit	    m_edtStrCoefMhr;
	MEdit    	m_edtStrCoefTr;
	CTextUnit	m_untCohe;
	CEditUnit	m_edtCohe;
	CTextUnit	m_untCoefTr;
	CTextUnit	m_untCoefMhr;
	CEditUnit	m_edtCoefTr;
	CEditUnit	m_edtCeofMhr;
	MEdit	    m_edtFric;
	CBCGPStatic	  m_untFric;
	CTextUnit	m_untYield;
	CEditUnit	m_edtYield;
	MComboBox	m_cmbModel;
	BOOL    	m_chkHardTr;
	CButton     m_chkFiberModel;
	int		    m_nHardTr;
	BOOL    	m_chkHardMhr;
	CString	  m_strName;
	int		m_nHardMhr;
	//}}AFX_DATA
	MComboBox	m_cmbMasonryMatlCoord;
	MComboBox	m_cmbMasonryMatlAngle;

	CEditUnit	m_edtDiliationAng;
	CEditUnit m_edtEccentricity;
	CEditUnit m_edtfbo_fco;
	CEditUnit m_edtK;
	CEditUnit m_edtViscosityParam;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMElsPlsMatlAddDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetElsPlsData(T_EPMT_K Key, T_EPMT_D& data)
	{
		m_Key = m_OldKey = Key;
		m_bModify	= m_bMicroMod = TRUE;
		m_Data = data;
		m_csOldName = data.strMatlName;
	}

// Implementation
protected:
	CDBDoc*  m_pDoc;
	double   m_dKValue[4];
	BOOL     m_bModify;
	BOOL     m_bMicroMod;
	T_EPMT_K m_Key, m_OldKey;
	T_EPMT_D m_Data;
	CString  m_csOldName;

	CArray<UINT, UINT> m_aTresca;
	CArray<UINT, UINT> m_aVmisesFibr;
	CArray<UINT, UINT> m_aMohrCl;
	CArray<UINT, UINT> m_aMicroP;
	CArray<UINT, UINT> m_aHardTresca; 
	CArray<UINT, UINT> m_aHardMohrCl; 
	CArray<UINT, UINT> m_aMason; // MNET:2603 20061212 mylee
	CArray<UINT, UINT> m_aMasonAngle; // MNET:XXXX-HSSHIM-20110828
	CArray<UINT, UINT> m_aConcDmg;
	CArray<UINT, UINT> m_aOKCancel;

	void InitCtrls();
	void AlignControl();
	void Data2Dlg();
	void SetDataCtrl(int nType);
	void Dlg2Data(BOOL& bSuc);
	// Generated message map functions
	//{{AFX_MSG(CCMElsPlsMatlAddDlg)
	afx_msg void OnApplyBtn();
	afx_msg void OnSelchangeModelCombo();
	afx_msg void OnElsplsModBtn();
	afx_msg void OnElsplsMasonBtn1(); // MNET:2603 20061212 mylee
	afx_msg void OnElsplsMasonBtn2();
	afx_msg void OnElsplsMasonBtn3();
	afx_msg void OnElsplsMasonBtn4();
	virtual BOOL OnInitDialog();
	afx_msg void OnElsplsHardChk();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnHardeningTypeTresca();
	afx_msg void OnHardeningTypeMohr();
	afx_msg void OnSelchangeMatlCoordCombo();	
	afx_msg void OnMatlCoord();
	afx_msg void OnCompBehBtn();
	afx_msg void OnTensileBehBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMELSPLSMATLADDDLG_H__)
