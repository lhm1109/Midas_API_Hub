#if !defined(__SEISITEMJAPAN2000_H__)
#define __SEISITEMJAPAN2000_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisItemJapan2000.h : header file
//
#include "..\MIT_frx\MComboBox.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "../wg_base/FoldGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisItemJapan2000 dialog

class CSeisItemDlgNew;
class CSeisItemJapan2000 : public CChildDialog
{
public:
	CSeisItemJapan2000(CWnd* pParent = NULL); // standard constructor

	void SetData2Dlg(T_SEIS_JIS& data, BOOL bModify);
	BOOL SetDlg2Data(T_SEIS_JIS& data);

	enum { IDD = IDD_ETC_SEIS_ITEM_JP2000 };

	CEditUnit m_editZoneF;
	CEditUnit m_editCo;
	mit::frx::MComboBox m_cobxTc;
	CEditUnit m_editPeriodX;
	CEditUnit m_editPeriodY;
	CFoldGroupBox_Cross m_grpFold;

public:
	virtual BOOL DestroyWindow();

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

protected:
	void Data2Dlg();
	BOOL Dlg2Data();

	void InitDefaultData();
	void InitSoilPeriodCombo();
	void SetFoldGroupCtrls();

protected:
	BOOL m_bInit;
	BOOL m_bInitAi;
	BOOL m_bModify;
	T_SEIS_JIS m_Data;
	CArray<UINT, UINT> m_aCtrlRadio;
	CSeisItemDlgNew* m_pParent;

	double m_dHXJPN2000;
	double m_dHYJPN2000;
	double m_dAXJPN2000;
	double m_dAYJPN2000;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdBtnCalcPeriod2();
	afx_msg void OnCmdAiUserBtn();
	afx_msg void OnCmdAiRadio();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
#endif // !defined(__SEISITEMJAPAN2000_H__)
