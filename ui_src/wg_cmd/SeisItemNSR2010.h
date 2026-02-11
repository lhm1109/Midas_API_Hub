#if !defined(AFX_SEISITEMNSR2010_H_)
#define AFX_SEISITEMNSR2010_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisItemIBC2012.h : header file
//
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "../wg_base/FoldGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisItemNSR2010 dialog
class CSeisItemDlgNew;

class CSeisItemNSR2010 : public CChildDialog
{
public:
	CSeisItemNSR2010(CWnd* pParent = NULL); // standard constructor

	void SetData2Dlg(T_SEIS_NSR2010& data, BOOL bModify);
	BOOL SetDlg2Data(T_SEIS_NSR2010& data);

	enum { IDD = IDD_ETC_SEIS_ITEM_NSR2010 };

	mit::frx::MComboBox m_cmbSiteClass;
	mit::frx::MComboBox m_cmbAA;
	mit::frx::MComboBox m_cmbAV;
	CEditUnit m_editFA;
	CEditUnit m_editFV;
	CEditUnit m_editCu;
	mit::frx::MComboBox m_cmbImportance;
	CEditUnit m_editAnalX;
	CEditUnit m_editAnalY;
	CEditUnit m_editApprX;
	CEditUnit m_editApprY;
	CEditUnit m_editFinalX;
	CEditUnit m_editFinalY;
	mit::frx::MComboBox m_cmbRx;
	mit::frx::MComboBox m_cmbRy;
	mit::frx::MButton m_btnCityPlan;
	CEditUnit m_editPhiX;
	CEditUnit m_editPhiY;
	CFoldGroupBox_Cross m_grpFold;

public:
	virtual BOOL DestroyWindow();

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

protected:
	void Data2Dlg();
	BOOL Dlg2Data();

	void InitDefaultData();
	void InitAaCombo();
	void InitAvCombo();
	void InitSiteCombo();
	void InitImportanceFactorCombo();
	void InitRxRyCombo();

	void SetFa();
	void SetFv();
	void SetCu();
	void CtrlManager();
	void SetFoldGroupCtrls();

protected:
	BOOL m_bInit;
	BOOL m_bModify;
	T_SEIS_NSR2010 m_Data;
	CSeisItemDlgNew* m_pParent;

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
	afx_msg void OnSelchangeCmdAaCmb();
	afx_msg void OnSelchangeCmdAvCmb();
	afx_msg void OnChangeCmdPeriodEdit();
	afx_msg void OnChangeCmdFvEdit();
	afx_msg void OnChangeCmdAa();
	afx_msg void OnChangeCmdAv();
	afx_msg void OnCmdPhiBtn();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_SEISITEMNSR2010_H_)
