#pragma once

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\mit_frx\MComboBox.h"
#include "..\wg_base\FoldGroupBox.h"

using namespace mit::frx;

class CSeisItemDlgNew;
class CSeisItemTaiwan2011 : public CChildDialog
{
public:
	CSeisItemTaiwan2011(CWnd* pParent = NULL);

	enum { IDD = IDD_ETC_SEIS_ITEM_TAIWAN2011 };

private:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	virtual void DoDataExchange(CDataExchange* pDX);

public:
	void SetData2Dlg(T_SEIS_TAIWAN02& data, BOOL bModify);
	BOOL SetDlg2Data(T_SEIS_TAIWAN02& data);

private:
	void InitDefaultData();
	void InitZoneCombo();
	void InitIFactorCombo();
	void InitAlphaCombo();
	void InitRxRyCombo();

	void Data2Dlg();
	BOOL Dlg2Data();
	void SetFinalPeriod();
	void SetFoldGroupCtrls();

private:
	afx_msg void OnCmdTaiwan02PeriodBtn();
	afx_msg void OnCmdTaiwan02PeriodRdo();
	afx_msg void OnChangeCmdPeriodEdit();
	afx_msg void OnCmdSeisTaiwan02SubBtn();
	afx_msg void OnSelchangeZone();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	MComboBox m_cbxZone;
	MComboBox m_cbxAlpha;
	MComboBox m_cbxIFactor;
	MComboBox m_cbxRY;
	MComboBox m_cbxRX;
	CEditUnit m_editApprY;
	CEditUnit m_editApprX;
	CEditUnit m_editAnalX;
	CEditUnit m_editAnalY;
	CEditUnit m_editFinalX;
	CEditUnit m_editFinalY;
	CSeisItemDlgNew* m_pParent;
	CFoldGroupBox_Cross m_grpFold;

protected:
	T_SEIS_TAIWAN02 m_Data;

private:
	BOOL m_bInit;
	BOOL m_bModify;

	double m_dAcXTaiwan02;
	double m_dAcYTaiwan02;
	double m_dHnXTaiwan02;
	double m_dHnYTaiwan02;
	int m_nMtdXTaiwan02;
	int m_nMtdYTaiwan02;

private:
	CArray<UINT, UINT> m_aCtrlRadio;
	CStringArray m_aIFactorName;
	CArray<double, double> m_aIFactorData;
	CStringArray m_aAFactorName;
	CArray<double, double> m_aAFactorData;
	CStringArray m_aRFactorName;
	CArray<double, double> m_aRFactorData;
};
