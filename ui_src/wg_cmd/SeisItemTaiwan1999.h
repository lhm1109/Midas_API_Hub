#if !defined(__SEISITEMTAIWAN1999_H__)
#define __SEISITEMTAIWAN1999_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisItemTaiwan1999.h : header file
//
#include "..\MIT_frx\MComboBox.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\FoldGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisItemTaiwan1999 dialog
class CSeisItemDlgNew;

class CSeisItemTaiwan1999 : public CChildDialog
{
public:
	CSeisItemTaiwan1999(CWnd* pParent = NULL); // standard constructor

	void SetData2Dlg(T_SEIS_TAIWAN86& data, BOOL bModify);
	BOOL SetDlg2Data(T_SEIS_TAIWAN86& data);

	enum { IDD = IDD_ETC_SEIS_ITEM_TAIWAN1999 };

	mit::frx::MComboBox m_cobxZone;
	mit::frx::MComboBox m_cobxSoilType;
	mit::frx::MComboBox m_cobxIFactor;
	mit::frx::MComboBox m_cobxAlpha;
	CEditUnit m_editAnalX;
	CEditUnit m_editAnalY;
	CEditUnit m_editApprX;
	CEditUnit m_editApprY;
	CEditUnit m_editFinalX;
	CEditUnit m_editFinalY;
	mit::frx::MComboBox m_cobxRX;
	mit::frx::MComboBox m_cobxRY;
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
	void InitSoilCombo();
	void InitIFactorCombo();
	void InitAlphaCombo();
	void InitRxRyCombo();
	void SetFoldGroupCtrls();

protected:
	BOOL m_bInit;
	BOOL m_bModify;
	CSeisItemDlgNew* m_pParent;
	T_SEIS_TAIWAN86 m_Data;

	double m_dAcXTaiwan86;
	double m_dAcYTaiwan86;
	double m_dHnXTaiwan86;
	double m_dHnYTaiwan86;
	int m_nMtdXTaiwan86;
	int m_nMtdYTaiwan86;

	CArray<UINT, UINT> m_aCtrlRadio;
	CStringArray m_aIFactorName;
	CArray<double, double> m_aIFactorData;
	CStringArray m_aAFactorName;
	CArray<double, double> m_aAFactorData;
	CStringArray m_aRFactorName;
	CArray<double, double> m_aRFactorData;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdTaiwan86PeriodBtn();
	afx_msg void OnCmdTaiwan86PeriodRdo();
	afx_msg void OnChangeCmdPeriodEdit();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
#endif // !defined(__SEISITEMTAIWAN1999_H__)
