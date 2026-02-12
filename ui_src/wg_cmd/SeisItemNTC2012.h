#if !defined(AFX_SEISITEMNTC2012_H__1266C336_52D6_4A44_BCC7_1D2FD1FA40BB__INCLUDED_)
#define AFX_SEISITEMNTC2012_H__1266C336_52D6_4A44_BCC7_1D2FD1FA40BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisItemNTC2012.h : header file
//
#include "..\MIT_frx\MComboBox.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\FoldGroupBox.h"


/////////////////////////////////////////////////////////////////////////////
// CSeisItemNTC2012 dialog
class CSeisItemDlgNew;

class CSeisItemNTC2012 : public CChildDialog
{
public:
	CSeisItemNTC2012(BOOL bNTC2018Mode = FALSE, CWnd* pParent = NULL); // standard constructor

	void SetData2Dlg(T_SEIS_NTC2008& data, BOOL bModify);
	BOOL SetDlg2Data(T_SEIS_NTC2008& data);

	enum { IDD = IDD_ETC_SEIS_ITEM_NTC2008 }; // PMS:DNKIM-20121021 : 2012 기준에 대해 2008과 동일한 Dlg 사용. 해당 리소스 걍 사용함.
	CEditUnit m_wndTd;
	CEditUnit m_wndTc;
	CEditUnit m_wndTb;
	CEditUnit m_wndS;
	mit::frx::MComboBox m_GroundTypeCmb;
	CEditUnit m_wndAg;
	CEditUnit m_wndQ;
	CEditUnit m_wndAmpF;
	CEditUnit m_wndTcStar;
	int m_nParaTypeRdo;
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
	void InitGroundTypeCombo();
	void SetFoldGroupCtrls();

protected:
	BOOL m_bNTC2018Mode;
	BOOL m_bInit;
	BOOL m_bModify;
	CSeisItemDlgNew* m_pParent;
	T_SEIS_NTC2008 m_Data;

	int m_nGroundType;

	double m_dHX;
	double m_dHY;
	double m_dAcX;
	double m_dAcY;
	double m_nDX;
	double m_nDY;
	int m_nMtdX;
	int m_nMtdY;

protected:
	CArray<UINT, UINT> m_aCtrlParaType, m_aCtrlRadioSpecType;
	void CtrlManager();
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdNTCBtnCalcPeriod();
	afx_msg void OnSelchangeNTC2012SpecTypeCmb();
	afx_msg void OnNTC2012ParatypeRdo();
	afx_msg void OnSelchangeNTC2012GroundCmb();
	afx_msg void OnKillfocusEdit();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SEISITEMNTC2012_H__1266C336_52D6_4A44_BCC7_1D2FD1FA40BB__INCLUDED_)
