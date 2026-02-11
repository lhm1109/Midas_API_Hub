#if !defined(__SEISITEMEURO1996_H__)
#define __SEISITEMEURO1996_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisItemEURO1996.h : header file
//
#include "../MIT_frx/MComboBox.h"

#include "../wg_db/wg_db_DBDoc.h"
#include "../wg_db/wg_db_EditUnit.h"
#include "..\wg_base\FoldGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisItemEURO1996 dialog
class CSeisItemDlgNew;

class CSeisItemEURO1996 : public CChildDialog
{
public:
	CSeisItemEURO1996(CWnd* pParent = nullptr); // standard constructor

	void SetData2Dlg(T_SEIS_EURO1996& data, BOOL bModify);
	BOOL SetDlg2Data(T_SEIS_EURO1996& data);

	enum { IDD = IDD_ETC_SEIS_ITEM_EURO1996 };

	frx::MComboBox m_cobxSc;
	frx::MComboBox m_cobxQo;
	frx::MComboBox m_cobxKd;
	frx::MComboBox m_cobxKr;
	CEditUnit m_editKw;
	CEditUnit m_editAlpha;
	CEditUnit m_editPeriodY;
	CEditUnit m_editPeriodX;
	CFoldGroupBox_Cross m_grpFold;

public:
	virtual BOOL DestroyWindow();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void Data2Dlg();
	BOOL Dlg2Data();

	void InitDefaultData();
	void InitScCombo();
	void InitQoCombo();
	void InitKdCombo();
	void InitKrCombo();
	void SetFoldGroupCtrls();

	BOOL m_bInit;
	BOOL m_bModify;
	CSeisItemDlgNew* m_pParent;
	T_SEIS_EURO1996 m_Data;

	double m_dHXEURO1996;
	double m_dHYEURO1996;
	double m_dAcXEURO1996;
	double m_dAcYEURO1996;
	double m_nDXEURO1996;
	double m_nDYEURO1996;
	int m_nMtdXEURO1996;
	int m_nMtdYEURO1996;

	BOOL OnInitDialog() override;
	afx_msg void OnCmdEuroBtnCalcPeriod();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(__SEISITEMEURO1996_H__)
