#if !defined(AFX_SEISITEMEURO2004_H__1266C336_52D6_4A44_BCC7_1D2FD1FA40BB__INCLUDED_)
#define AFX_SEISITEMEURO2004_H__1266C336_52D6_4A44_BCC7_1D2FD1FA40BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\mit_frx\MComboBox.h"
#include "../wg_base/FoldGroupBox.h"

using namespace mit::frx;
class CSeisItemDlgNew;

class CSeisItemEURO2004 : public CChildDialog
{
public:
	CSeisItemEURO2004(CWnd* pParent = NULL); // standard constructor

	void SetData2Dlg(T_SEIS_EURO2004& data, BOOL bModify, int nNationalAnnex);
	BOOL SetDlg2Data(T_SEIS_EURO2004& data);
	void SetNationalAnnex(int nNationalAnnex);

	enum { IDD = IDD_ETC_SEIS_ITEM_EURO2004 };

	CEditUnit m_wndTd;
	CEditUnit m_wndTc;
	CEditUnit m_wndTb;
	CEditUnit m_wndS;
	MComboBox m_GroundTypeCmb;
	MComboBox m_RegionMalCmb;
	CEditUnit m_wndBeta;
	CEditUnit m_wndQ;
	CEditUnit m_wndAg;
	MComboBox m_cmbImportance;
	int m_nParaTypeRdo;
	CEditUnit m_editPeriodY;
	CEditUnit m_editPeriodX;
	CFoldGroupBox_Cross m_grpFold;
	CSeisItemDlgNew* m_pParent;

public:
	virtual BOOL DestroyWindow();

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

protected:
	void Data2Dlg();
	BOOL Dlg2Data();
	void InitDefaultData();
	void InitRegionCombo();
	void InitGroundTypeCombo();
	void InitIFactorCombo(BOOL bInit = FALSE);
	void AlignControlByNA();
	void SetFoldGroupCtrls();

protected:
	BOOL m_bInit;
	BOOL m_bModify;
	int m_nNationalAnnex;
	T_SEIS_EURO2004 m_Data;

	int m_nGroundType;
	int m_nRegion;

	double m_dHX;
	double m_dHY;
	double m_dAcX;
	double m_dAcY;
	double m_nDX;
	double m_nDY;
	int m_nMtdX;
	int m_nMtdY;
	CArray<UINT, UINT> m_aCtrlRegionType, m_aCtrlMove;

protected:
	CArray<UINT, UINT> m_aCtrlParaType, m_aCtrlRadioSpecType;
	void CtrlManager();
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdEuroBtnCalcPeriod();
	//afx_msg void OnSelchangeEuro2004SpecTypeCmb();
	afx_msg void OnEuro2004ParatypeRdo();
	afx_msg void OnSelchangeEuro2004GroundCmb();
	afx_msg void OnSelchangeEuro2004RegionCmb();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SEISITEMEURO2004_H__1266C336_52D6_4A44_BCC7_1D2FD1FA40BB__INCLUDED_)
