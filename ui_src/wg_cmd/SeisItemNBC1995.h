#if !defined(__SEISITEMNBC1995_H__)
#define __SEISITEMNBC1995_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisItemNBC1995.h : header file
//
#include "..\MIT_frx\MComboBox.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\FoldGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisItemNBC1995 dialog

class CSeisItemDlgNew;

class CSeisItemNBC1995 : public CChildDialog
{
public:
	CSeisItemNBC1995(CWnd* pParent = NULL); // standard constructor

	void SetData2Dlg(T_SEIS_NBC1995& data, BOOL bModify);
	BOOL SetDlg2Data(T_SEIS_NBC1995& data);

	enum { IDD = IDD_ETC_SEIS_ITEM_NBC1995 };

	mit::frx::MComboBox m_cobxVRatio;
	mit::frx::MComboBox m_cobxAZone;
	mit::frx::MComboBox m_cobxVZone;
	mit::frx::MComboBox m_cobxIFactor;
	mit::frx::MComboBox m_cobxFFactor;
	CEditUnit m_editAX;
	CEditUnit m_editAY;
	CEditUnit m_editCX;
	CEditUnit m_editCY;
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
	void InitVRatioCombo();
	void InitAZoneCombo();
	void InitVZoneCombo();
	void InitIFactorCombo();
	void InitFFactorCombo();
	void InitRxRyCombo();
	void SetFoldGroupCtrls();

protected:
	BOOL m_bInit;
	BOOL m_bModify;
	CSeisItemDlgNew* m_pParent;
	T_SEIS_NBC1995 m_Data;

	double m_dHXNBC1995;
	double m_dHYNBC1995;
	double m_dBXNBC1995;
	double m_dBYNBC1995;
	int m_nNXNBC1995;
	int m_nNYNBC1995;
	int m_nMtdXNBC1995;
	int m_nMtdYNBC1995;

	CStringArray m_aVRatioName;
	CArray<double, double> m_aVRatioData;
	CStringArray m_aIFactorName;
	CArray<double, double> m_aIFactorData;
	CStringArray m_aFFactorName;
	CArray<double, double> m_aFFactorData;
	CStringArray m_aRFactorName;
	CArray<double, double> m_aRFactorData;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdNbc95TCalcBtn();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(__SEISITEMNBC1995_H__)
