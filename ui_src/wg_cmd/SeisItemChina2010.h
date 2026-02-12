#if !defined(__SEISITEMCHINA2010_H__)
#define __SEISITEMCHINA2010_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisItemChina2010.h : header file
//
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "../wg_base/FoldGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisItemChina2010 dialog
class CSeisItemDlgNew;
class CSeisItemChina2010 : public CChildDialog
{
	// Construction
public:
	CSeisItemChina2010(CWnd* pParent = NULL); // standard constructor

	void SetData2Dlg(T_SEIS_CH2010& data, BOOL bModify);
	BOOL SetDlg2Data(T_SEIS_CH2010& data);

	// Dialog Data
	enum { IDD = IDD_ETC_SEIS_ITEM_CH2010 };

	mit::frx::MComboBox m_cobxCategory;
	mit::frx::MComboBox m_cobxIntensity;
	mit::frx::MComboBox m_cobxSiteClass;
	mit::frx::MComboBox m_cobxStType;
	CEditUnit m_editDampRatio;
	mit::frx::MButton m_btnAlpha;
	CEditUnit m_editPeriodY;
	CEditUnit m_editPeriodX;
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
	void InitIntensityCombo();
	void InitSiteClassCombo();
	void InitStrtTypeCombo();
	void SetFoldGroupCtrls();

protected:
	BOOL m_bInit;
	BOOL m_bModify;
	T_SEIS_CH2010 m_Data;
	CSeisItemDlgNew* m_pParent;

	double m_dHXCH2010;
	double m_dHYCH2010;
	double m_dBXCH2010;
	double m_dBYCH2010;
	int m_nNXCH2010;
	int m_nNYCH2010;
	int m_nMtdXCH2010;
	int m_nMtdYCH2010;

	CArray<UINT, UINT> m_aCtrlRadio;

	// Implementation
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdChinaBtnCalcPeriod();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(__SEISITEMCHINA2010_H__)
