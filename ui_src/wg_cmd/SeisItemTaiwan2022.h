#pragma once

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\mit_frx\MComboBox.h"
#include "..\wg_base\FoldGroupBox.h"

using namespace mit::frx;
class CSeisItemDlgNew;
class CSeisItemTaiwan2022 : public CChildDialog
{
public:
	CSeisItemTaiwan2022(CWnd* pParent = NULL);
	enum { IDD = IDD_ETC_SEIS_ITEM_TAIWAN2022 };

private:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	virtual void DoDataExchange(CDataExchange* pDX);

public:
	void SetData2Dlg(T_SEIS_TAIWAN22& data, BOOL bModify);
	BOOL SetDlg2Data(T_SEIS_TAIWAN22& data);

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
	afx_msg void OnBtnPeriod();
	afx_msg void OnRdoPeriod();
	afx_msg void OnChangePeriod();
	afx_msg void OnBtnSeisSub();
	afx_msg void OnSelChangeZone();
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
	CFoldGroupBox_Cross m_grpFold;

protected:
	CSeisItemDlgNew* m_pParent;
	T_SEIS_TAIWAN22 m_Data;

private:
	BOOL m_bInit;
	BOOL m_bModify;

	double m_dAcXTaiwan02;
	double m_dAcYTaiwan02;
	double m_dHnXTaiwan02;
	double m_dHnYTaiwan02;
	int	   m_nMtdXTaiwan02;
	int	   m_nMtdYTaiwan02;

private:
	CArray<UINT, UINT> m_aCtrlStruct;
	CArray<UINT, UINT> m_aCtrlPeriod;

	CStringArray m_aIFactorName;
	CStringArray m_aAFactorName;
	CStringArray m_aRFactorName;
	CArray<double, double> m_aIFactorData;
	CArray<double, double> m_aAFactorData;
	CArray<double, double> m_aRFactorData;
};
