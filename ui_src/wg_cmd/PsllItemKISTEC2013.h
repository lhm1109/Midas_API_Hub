#if !defined(AFX_PSLLITEMKISTEC2013_H__A87C3B44_CF70_4AA2_AF5E_3C30BBF05A2A__INCLUDED_)
#define AFX_PSLLITEMKISTEC2013_H__A87C3B44_CF70_4AA2_AF5E_3C30BBF05A2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\mit_frx\MComboBox.h"
using namespace mit::frx;

class CPsllItemKISTEC2013 : public CChildDialog
{
// Construction
public:
	CPsllItemKISTEC2013(CWnd* pParent = NULL);   // standard constructor

	 void SetData2Dlg(T_PSLL_KISTEC2013& data, BOOL bModify, int nMethod);
	 BOOL SetDlg2Data(T_PSLL_KISTEC2013& data);

// Dialog Data
	//{{AFX_DATA(CPsllItemKISTEC2013)
	enum { IDD = IDD_ETC_PSLL_ITEM_KISTEC2013 };
	MComboBox	m_cmbSeisZone;
	MComboBox m_cmbSeisZoneFactor;
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
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPsllItemKISTEC2013)
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
	//void InitImportanceFactorCombo();
	//void InitUseGroupCombo();

	void SetComboValue();
	void SetFaFvSdsSd1();

	void SetKey(T_PSLL_K key);

	
protected:
	BOOL m_bModify;
	int m_nMethod;
	T_PSLL_KISTEC2013  m_Data;
	T_PSLL_K m_Key;

	double	m_dAcX;	
	double	m_dAcY;
	double	m_dHnX;	
	double	m_dHnY;
	int     m_nNX;
	int     m_nNY;
	int		  m_nMtdX;
	int			m_nMtdY;
	double  m_dC;
	double  m_dJ;
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPsllItemKISTEC2013)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmdZoneCmb();
	afx_msg void OnSelchangeCmdZoneFactorCmb();
	afx_msg void OnSelchangeCmdSiteCmb();

	afx_msg void OnCmdPeriodBtn();
	afx_msg void OnChangeCmdZoneFactor();
	afx_msg void OnChangeCmdFaEdit();
	afx_msg void OnChangeCmdFvEdit();
	afx_msg void OnChangeCmdSdsEdit();
	afx_msg void OnChangeCmdSd1Edit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PSLLITEMKISTEC2013_H__A87C3B44_CF70_4AA2_AF5E_3C30BBF05A2A__INCLUDED_)
