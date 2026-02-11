#if !defined(__CMDAUTOLOADCOMBCVLKOREALRFD11DLG_H__)
#define __CMDAUTOLOADCOMBCVLKOREALRFD11DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdAutoLoadCombCvlKOREALRFD11Dlg.h : header file
//
#include "..\wg_base\FoldGroupBox.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlKOREALRFD11Dlg dialog

class CCmdAutoLoadCombCvlDlg;
class CCmdAutoLoadCombCvlKOREALRFD11Dlg : public CChildDialog
{
// Construction
public:
	CCmdAutoLoadCombCvlKOREALRFD11Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCmdAutoLoadCombCvlKOREALRFD11Dlg)
	enum { IDD = IDD_CMD_LCOMBDEFAULT_KOREA_LRFD11_DLG };

	CEditUnit m_edtLrfd11Modifier;
	int       m_nLrfd11DC;
	int       m_nLrfd11DD;
	int       m_nLrfd11DW;
	int       m_nLrfd11EHType;
	int       m_nLrfd11EH1;
	int       m_nLrfd11EH2;
	int       m_nLrfd11EVType;
	int       m_nLrfd11EV2;
	int       m_nLrfd11EV3;
	int       m_nLrfd11EV4;
	int       m_nLrfd11EV5;
	int       m_nLrfd11EV6;
	int       m_nLrfd11ES;
	CEditUnit	m_edtLrfd11Settlement;
	BOOL   	  m_bLrfd11CulvertChk;
	int   		m_nLrfd11Deformation;
	mit::frx::MComboBox m_cmbPSCRSHType;
	mit::frx::MComboBox	m_cmbLLCase;
	mit::frx::MComboBox	m_cmbDLCase;
	CListCtrl	m_lstDL;
	BOOL   	  m_bLrfd11GravityLoadChk;
	CEditUnit m_edtLrfd11EffLoadFactor;

	CFoldGroupBox_Cross	m_chkLoadFactor;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdAutoLoadCombCvlKOREALRFD11Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void InitPSCRSHCombo();
	void InitLLCombo();
	void InitDLCombo();
	void InitDLListBox();
	BOOL CheckDLCaseName(CString strDLName);
	void GetKoreaLRFD11LoadFactor();
	void OnCmdKoreaLRFD11EnableDisable();
	
	int m_nConstLoad;

protected:
	CDBDoc* m_pDoc;
	CString m_strCodeName;

	CCmdAutoLoadCombCvlDlg* m_pParent;

	// Generated message map functions
	//{{AFX_MSG(CCmdAutoLoadCombCvlKOREALRFD11Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdKoreaLRFD11EHTypeRdo();
	afx_msg void OnCmdKoreaLRFD11EVTypeRdo();
	afx_msg void OnCmdKoreaLRFD11PSCRSHBtn();
	afx_msg void OnCmdKoreaLRFD11DLAddBtn();
	afx_msg void OnCmdKoreaLRFD11DLDelBtn();
	afx_msg void OnCmdKoreaLRFD11GRACHK();

	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	int m_nKoreaLr11DcNum;
	int m_nKoreaLr11DdNum;
	int m_nKoreaLr11DwNum;
	int m_nKoreaLr11EhNum;
	int m_nKoreaLr11ElNum;
	int m_nKoreaLr11EvNum;
	int m_nKoreaLr11EsNum;
	int m_nKoreaLr11PsNum;
	int m_nKoreaLr11CrNum;
	int m_nKoreaLr11ShNum;
	int m_nKoreaLr11PSCRSHType;
	double m_dKoreaLr11Dc1;
	double m_dKoreaLr11Dc2;
	double m_dKoreaLr11Dd1;
	double m_dKoreaLr11Dd2;
	double m_dKoreaLr11Dw1;
	double m_dKoreaLr11Dw2;
	double m_dKoreaLr11Eh1;
	double m_dKoreaLr11Eh2;
	double m_dKoreaLr11El1;
	double m_dKoreaLr11El2;
	double m_dKoreaLr11Ev1;
	double m_dKoreaLr11Ev2;
	double m_dKoreaLr11Es1;
	double m_dKoreaLr11Es2;
	double m_dKoreaLr11Ps1;
	double m_dKoreaLr11Ps2;
	double m_dKoreaLr11Cr1;
	double m_dKoreaLr11Cr2;
	double m_dKoreaLr11Sh1;
	double m_dKoreaLr11Sh2;

	CArray<UINT, UINT> m_aDLData;

	//CMap<UINT, UINT, UINT, UINT> m_DLData;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMDAUTOLOADCOMBCVLKOREALRFD11DLG_H__)
