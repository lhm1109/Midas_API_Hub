#if !defined(__SEISITEMIS2002_H__)
#define __SEISITEMIS2002_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisItemIS2002.h : header file
//
#include "..\MIT_frx\MComboBox.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "../wg_base/FoldGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisItemIS2002 dialog

class CSeisItemDlgNew;
class CSeisItemIS2002 : public CChildDialog
{
public:
	CSeisItemIS2002(CWnd* pParent = NULL); // standard constructor

	void SetData2Dlg(T_SEIS_IS2002& data, BOOL bModify);
	BOOL SetDlg2Data(T_SEIS_IS2002& data);

	enum { IDD = IDD_ETC_SEIS_ITEM_IS2002 };

	mit::frx::MComboBox m_cobxZone;
	mit::frx::MComboBox m_cobxSoilType;
	mit::frx::MComboBox m_cobxIFactor;
	CEditUnit m_editDamping;
	CEditUnit m_editDFactor;
	CEditUnit m_editPeriodX;
	CEditUnit m_editPeriodY;
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
	void InitRxRyCombo();
	double GetDampingFactor(double dDamping);
	void SetFoldGroupCtrls();

protected:
	BOOL m_bInit;
	BOOL m_bModify;
	CSeisItemDlgNew* m_pParent;
	T_SEIS_IS2002 m_Data;

	double m_dHXIS2002;
	double m_dHYIS2002;
	double m_dBXIS2002;
	double m_dBYIS2002;
	int m_nMtdXIS2002;
	int m_nMtdYIS2002;

	CStringArray m_aIFactorName;
	CArray<double, double> m_aIFactorData;
	CStringArray m_aRFactorName;
	CArray<double, double> m_aRFactorData;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdIs02TCalcBtn();
	afx_msg void OnChangeCmdIs02DampingEdit();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(__SEISITEMIS2002_H__)
