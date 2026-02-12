#if !defined(AFX_SEISITEMKBC2009_H__A87C3B44_CF70_4AA2_AF5E_3C30BBF05A2A__INCLUDED_)
#define AFX_SEISITEMKBC2009_H__A87C3B44_CF70_4AA2_AF5E_3C30BBF05A2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisItemKBC2009.h : header file
//
#include "..\MIT_frx\MComboBox.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\FoldGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisItemKBC2009 dialog

class CSeisItemDlgNew;

class CSeisItemKBC2009 : public CChildDialog
{
	// Construction
public:
	CSeisItemKBC2009(CWnd* pParent = NULL); // standard constructor

	void SetData2Dlg(T_SEIS_KBC2009& data, BOOL bModify);
	BOOL SetDlg2Data(T_SEIS_KBC2009& data);

	enum { IDD = IDD_ETC_SEIS_ITEM_KBC2009 };

	mit::frx::MComboBox m_cmbSeisZone;
	mit::frx::MComboBox m_cmbSeisZoneFactor;
	mit::frx::MComboBox m_cmbSiteClass;
	mit::frx::MComboBox m_cmbUseGroup;
	mit::frx::MComboBox m_cmbImportance;
	mit::frx::MComboBox m_cmbRy;
	mit::frx::MComboBox m_cmbRx;

	CEditUnit m_editCu;
	CEditUnit m_editFa;
	CEditUnit m_editFv;
	CEditUnit m_editSds;
	CEditUnit m_editSd1;
	CEditUnit m_editSDC_S;
	CEditUnit m_editSDC_1;
	CEditUnit m_editSDC;
	CEditUnit m_editAnalX;
	CEditUnit m_editAnalY;
	CEditUnit m_editApprX;
	CEditUnit m_editApprY;
	CEditUnit m_editFinalX;
	CEditUnit m_editFinalY;
	CFoldGroupBox_Cross m_grpFold;

public:
	virtual BOOL DestroyWindow();

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

protected:
	void Data2Dlg();
	BOOL Dlg2Data();

	void InitDefaultData();
	void InitZoneCombo();
	void InitZoneFactorCombo();
	void InitSiteCombo();
	void InitImportanceFactorCombo();
	void InitUseGroupCombo();
	void InitRxRyCombo();

	void SetComboValue();
	void SetFaFvSdsSd1();
	void SetSeisDesignCategory();
	void SetImportanceFactor();
	void CtrlManager();
	void SetFoldGroupCtrls();

protected:
	BOOL m_bInit;
	BOOL m_bModify;
	T_SEIS_KBC2009 m_Data;
	CSeisItemDlgNew* m_pParent;

	double m_dAcX;
	double m_dAcY;
	double m_dHnX;
	double m_dHnY;
	int m_nNX;
	int m_nNY;
	int m_nMtdX;
	int m_nMtdY;

	CArray<UINT, UINT> m_aCtrlRadioPeriod;
	CArray<UINT, UINT> m_aCtrlPeriodAnal;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmdZoneCmb();
	afx_msg void OnSelchangeCmdZoneFactorCmb();
	afx_msg void OnSelchangeCmdSiteCmb();
	afx_msg void OnSelchangeCmdUseGroupCmb();
	afx_msg void OnCmdPeriodBtn();
	afx_msg void OnCmdPeriodRdo();
	afx_msg void OnChangeCmdZoneFactor();
	afx_msg void OnChangeCmdFaEdit();
	afx_msg void OnChangeCmdFvEdit();
	afx_msg void OnChangeCmdSdsEdit();
	afx_msg void OnChangeCmdSd1Edit();
	afx_msg void OnChangeCmdPeriodEdit();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_SEISITEMKBC2009_H__A87C3B44_CF70_4AA2_AF5E_3C30BBF05A2A__INCLUDED_)
