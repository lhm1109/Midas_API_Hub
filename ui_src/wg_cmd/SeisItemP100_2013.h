#if !defined(AFX_SEISITEMP100_2013_H__1266C336_52D6_4A44_BCC7_1D2FD1FA40BB__INCLUDED_)
#define AFX_SEISITEMP100_2013_H__1266C336_52D6_4A44_BCC7_1D2FD1FA40BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisItemP100_2013.h : header file
//
#include "..\MIT_frx\MComboBox.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\FoldGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisItemP100_2013 dialog

class CSeisItemDlgNew;

class CSeisItemP100_2013 : public CChildDialog
{
public:
	CSeisItemP100_2013(CWnd* pParent = NULL); // standard constructor

	void SetData2Dlg(T_SEIS_P100_2013& data, BOOL bModify);
	BOOL SetDlg2Data(T_SEIS_P100_2013& data);

	enum { IDD = IDD_ETC_SEIS_ITEM_P100_2013 };

	CEditUnit m_wndTd;
	CEditUnit m_wndTc;
	CEditUnit m_wndTb;
	CEditUnit m_wndQ;
	CEditUnit m_wndAg;
	mit::frx::MComboBox m_cmbImportance;
	int m_nParaTypeRdo;
	CEditUnit m_editPeriodY;
	CEditUnit m_editPeriodX;
	CFoldGroupBox_Cross m_grpFold;

public:
	virtual BOOL DestroyWindow();

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

protected:
	void InitCtrl();
	void Data2Dlg();
	BOOL Dlg2Data();
	void InitDefaultData();
	void InitIFactorCombo();
	void SetFoldGroupCtrls();

protected:
	BOOL m_bInit;
	BOOL m_bModify;
	CSeisItemDlgNew* m_pParent;
	T_SEIS_P100_2013 m_Data;

	int m_nGroundType;

	double m_dHX;
	double m_dHY;
	double m_dAcX;
	double m_dAcY;
	int m_nStoryNumX;
	int m_nStoryNumY;
	int m_nMtdX;
	int m_nMtdY;

protected:
	CArray<UINT, UINT> m_aCtrlParaType, m_aCtrlRadioSpecType;
	void CtrlManager();
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdEuroBtnCalcPeriod();
	afx_msg void OnSelchangeP100_2013SpecTypeCmb();
	afx_msg void OnP100_2013ParatypeRdo();
	afx_msg void OnSelchangeP100_2013GroundCmb();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SEISITEMP100_2013_H__1266C336_52D6_4A44_BCC7_1D2FD1FA40BB__INCLUDED_)
