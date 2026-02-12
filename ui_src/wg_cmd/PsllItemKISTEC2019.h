#if !defined(__PSLLITEM_KISTEC2019_H__)
#define __PSLLITEM_KISTEC2019_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\mit_frx\MComboBox.h"
using namespace mit::frx;

class CPsllItemKISTEC2019 : public CChildDialog
{
// Construction
public:
	CPsllItemKISTEC2019(CWnd* pParent = NULL);   // standard constructor

	 void SetData2Dlg(T_PSLL_KISTEC2019& data, BOOL bModify, int nMethod);
	 BOOL SetDlg2Data(T_PSLL_KISTEC2019& data);

// Dialog Data
	//{{AFX_DATA(CPsllItemKISTEC2019)
	enum { IDD = IDD_ETC_PSLL_ITEM_KISTEC2019 };
	MComboBox	m_cmbSeisZone;
	MComboBox	m_cmbSeisZoneFactor;
	MComboBox	m_cmbSiteClass;
	
	CEditUnit	m_editCu;
	CEditUnit	m_editFa;
	CEditUnit	m_editFv;
	CEditUnit	m_editSds;
	CEditUnit	m_editSd1;

	CEditUnit	m_editApprX;
	CEditUnit	m_editApprY;

	CEditUnit	m_editParamC;
	CEditUnit	m_editParamJ;
	CEditUnit	m_editParamX;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPsllItemKISTEC2019)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void Data2Dlg();
	BOOL Dlg2Data();

	void InitDefaultData();
	void InitDefaultPeriodData();
	void InitZoneCombo();
	void InitZoneFactorCombo();
	void InitSiteCombo();

	void SetComboValue();
	void SetFaFvSdsSd1();
	void SetKey(T_PSLL_K key);
	
protected:
	BOOL m_bModify;
	int m_nMethod;
	T_PSLL_KISTEC2019  m_Data;
	T_PSLL_K m_Key;

	double	m_dAcX;	
	double	m_dAcY;
	double	m_dHnX;	
	double	m_dHnY;
	int     m_nNX;
	int     m_nNY;
	int		m_nMtdX;
	int		m_nMtdY;
	double  m_dC;
	double  m_dJ;
	double  m_dX;
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPsllItemKISTEC2019)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmdZoneCmb();
	afx_msg void OnSelchangeCmdZoneFactorCmb();
	afx_msg void OnSelchangeCmdSiteCmb();

	afx_msg void OnCmdPeriodBtn();
	afx_msg void OnChangeCmdZoneFactor();
	afx_msg void OnChangeCmdFaEdit();
	afx_msg void OnChangeCmdFvEdit();
	afx_msg void OnChangeCmdSd1Edit();
	afx_msg void OnNotConsCuChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__PSLLITEM_KISTEC2019_H__)
