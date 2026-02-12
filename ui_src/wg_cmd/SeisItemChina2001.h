#if !defined(__SEISITEMCHINA2001_H__)
#define __SEISITEMCHINA2001_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisItemChina2001.h : header file
//
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "../wg_base/FoldGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisItemChina2001 dialog
class CSeisItemDlgNew;

class CSeisItemChina2001 : public CChildDialog
{
public:
	CSeisItemChina2001(CWnd* pParent = NULL); // standard constructor

	void SetData2Dlg(T_SEIS_CH2002& data, BOOL bModify);
	BOOL SetDlg2Data(T_SEIS_CH2002& data);

	enum { IDD = IDD_ETC_SEIS_ITEM_CH2001 };

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
	T_SEIS_CH2002 m_Data;

	double m_dHXCH2001;
	double m_dHYCH2001;
	double m_dBXCH2001;
	double m_dBYCH2001;
	int m_nNXCH2001;
	int m_nNYCH2001;
	int m_nMtdXCH2001;
	int m_nMtdYCH2001;

	CArray<UINT, UINT> m_aCtrlRadio;
	CSeisItemDlgNew* m_pParent;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdChinaBtnCalcPeriod();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(__SEISITEMCHINA2001_H__)
