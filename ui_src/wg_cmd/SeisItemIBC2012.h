#if !defined(AFX_SEISITEMIBC2012_H__A87C3B44_CF70_4AA2_AF5E_3C30BBF05A2A__INCLUDED_)
#define AFX_SEISITEMIBC2012_H__A87C3B44_CF70_4AA2_AF5E_3C30BBF05A2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisItemIBC2012.h : header file
//
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\FoldGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisItemIBC2012 dialog
class CSeisItemDlgNew;

class CSeisItemIBC2012 : public CChildDialog
{
public:
	CSeisItemIBC2012(CWnd* pParent = NULL, int nCode = 0); // standard constructor

	void SetData2Dlg(T_SEIS_IBC2012& data, BOOL bModify);
	BOOL SetDlg2Data(T_SEIS_IBC2012& data);

	void SetCode(int nCode);

	enum { IDD = IDD_ETC_SEIS_ITEM_IBC2012 };

	mit::frx::MComboBox m_cmbSiteClass;
	mit::frx::MComboBox m_cmbSs;
	mit::frx::MComboBox m_cmbS1;
	CEditUnit m_editFa;
	CEditUnit m_editFv;
	CEditUnit m_editSds;
	CEditUnit m_editSd1;
	CEditUnit m_editCu;
	CEditUnit m_editTL;
	mit::frx::MComboBox m_cmbCategory;
	mit::frx::MComboBox m_cmbImportance;
	CEditUnit m_editSDC;
	CEditUnit m_editSDC_S;
	CEditUnit m_editSDC_1;
	CEditUnit m_editAnalX;
	CEditUnit m_editAnalY;
	CEditUnit m_editApprX;
	CEditUnit m_editApprY;
	CEditUnit m_editFinalX;
	CEditUnit m_editFinalY;
	mit::frx::MComboBox m_cmbRx;
	mit::frx::MComboBox m_cmbRy;
	mit::frx::MButton m_btnCityPlan;
	CFoldGroupBox_Cross m_grpFold;

public:
	virtual BOOL DestroyWindow();

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

protected:
	void Data2Dlg();
	BOOL Dlg2Data();

	void InitDefaultData();
	void InitSsCombo();
	void InitS1Combo();
	void InitSiteCombo();
	void InitImportanceFactorCombo();
	void InitCategoryCombo();
	void InitRxRyCombo();
	void SetFoldGroupCtrls();

	void SetFaFvSdsSd1();
	void SetSeisDesignCategory();
	void SetImportanceFactor();
	void CtrlManager();

protected:
	int m_nCode;
	BOOL m_bInit;
	BOOL m_bModify;
	CSeisItemDlgNew* m_pParent;
	T_SEIS_IBC2012 m_Data;

	double m_dCwX;
	double m_dCwY;
	double m_dHnX;
	double m_dHnY;
	int m_nNX;
	int m_nNY;
	int m_nMtdX;
	int m_nMtdY;

	CArray<UINT, UINT> m_aCtrlRadioAcc, m_aCtrlRadioPeriod;
	CArray<UINT, UINT> m_aCtrlPerCode, m_aCtrlUserDefined;
	CArray<UINT, UINT> m_aCtrlPeriodAnal;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdPeriodBtn();
	afx_msg void OnCmdPeriodRdo();
	afx_msg void OnSelchangeCmdSiteCmb();
	afx_msg void OnSelchangeCmdSsCmb();
	afx_msg void OnSelchangeCmdS1Cmb();
	afx_msg void OnSelchangeCmdCategoryCmb();
	afx_msg void OnChangeCmdPeriodEdit();
	afx_msg void OnChangeCmdSdsEdit();
	afx_msg void OnChangeCmdSd1Edit();
	afx_msg void OnChangeCmdFaEdit();
	afx_msg void OnChangeCmdFvEdit();
	afx_msg void OnChangeCmdSs();
	afx_msg void OnChangeCmdS1();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SEISITEMIBC2012_H__A87C3B44_CF70_4AA2_AF5E_3C30BBF05A2A__INCLUDED_)
