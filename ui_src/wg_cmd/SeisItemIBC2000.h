#if !defined(__SEISITEMIBC2000_H__)
#define __SEISITEMIBC2000_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SiesItemIBC2000.h : header file
//
#include "..\MIT_frx\MComboBox.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\FoldGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisItemIBC2000 dialog
class CSeisItemDlgNew;

class CSeisItemIBC2000 : public CChildDialog
{
	// Construction
public:
	CSeisItemIBC2000(CWnd* pParent = NULL); // standard constructor

	void SetData2Dlg(T_SEIS_IBC2000& data, BOOL bModify);
	BOOL SetDlg2Data(T_SEIS_IBC2000& data);

	enum { IDD = IDD_ETC_SEIS_ITEM_IBC2000 };

	mit::frx::MComboBox m_cobxCategory;
	mit::frx::MComboBox m_cobxSs;
	mit::frx::MComboBox m_cobxSite;
	mit::frx::MComboBox m_cobxS1;
	mit::frx::MComboBox m_cobxIFactor;
	CEditUnit m_editAX;
	CEditUnit m_editAY;
	CEditUnit m_editCX;
	CEditUnit m_editCY;
	CEditUnit m_editRX;
	CEditUnit m_editRY;
	CFoldGroupBox_Cross m_grpFold;

public:
	virtual BOOL DestroyWindow();

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

protected:
	void Data2Dlg();
	BOOL Dlg2Data();

	void InitDefaultData();
	void InitCategoryCombo();
	void InitSiteClassCombo();
	void InitShtPeriodCombo();
	void InitSecPeriodCombo();
	void InitIFactorCombo();
	void SetFoldGroupCtrls();

protected:
	BOOL m_bInit;
	BOOL m_bModify;
	CSeisItemDlgNew* m_pParent;
	T_SEIS_IBC2000 m_Data;

	double m_dHXIBC2000;
	double m_dHYIBC2000;
	int m_nNXIBC2000;
	int m_nNYIBC2000;
	int m_nMtdXIBC2000;
	int m_nMtdYIBC2000;

	CStringArray m_aSsName;
	CArray<double, double> m_aSsData;
	CStringArray m_aS1Name;
	CArray<double, double> m_aS1Data;
	CStringArray m_aIFactorName;
	CArray<double, double> m_aIFactorData;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdIbcBtnCalcPeriod();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(__SEISITEMIBC2000_H__)
