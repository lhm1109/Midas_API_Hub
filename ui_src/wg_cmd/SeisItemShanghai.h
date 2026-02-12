#if !defined(AFX_SEISITEMSHANGHAI_H__707193A3_82A9_49EE_8512_A17A620E1C46__INCLUDED_)
#define AFX_SEISITEMSHANGHAI_H__707193A3_82A9_49EE_8512_A17A620E1C46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisItemShanghai.h : header file
//
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "../wg_base/FoldGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisItemShanghai dialog

class CSeisItemDlgNew;

class CSeisItemShanghai : public CChildDialog
{
	// Construction
public:
	CSeisItemShanghai(CWnd* pParent = NULL); // standard constructor

	void SetData2Dlg(T_SEIS_CHSH2003& data, BOOL bModify);
	BOOL SetDlg2Data(T_SEIS_CHSH2003& data);

	enum { IDD = IDD_ETC_SEIS_ITEM_SHANGHAI };

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
	//}}AFX_VIRTUAL

protected:
	void Data2Dlg();
	BOOL Dlg2Data();

	void InitDefaultData();
	void InitIntensityCombo();
	void InitSiteClassCombo();
	void InitStrtTypeCombo();
	void SetFoldGroupCtrls();

protected:
	BOOL m_bInit;
	BOOL m_bModify;
	T_SEIS_CHSH2003 m_Data;
	CSeisItemDlgNew* m_pParent;

	double m_dHXCH2001;
	double m_dHYCH2001;
	double m_dBXCH2001;
	double m_dBYCH2001;
	int m_nNXCH2001;
	int m_nNYCH2001;
	int m_nMtdXCH2001;
	int m_nMtdYCH2001;

	CArray<UINT, UINT> m_aCtrlRadio;

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSeisItemShanghai)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdChinaBtnCalcPeriod();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEISITEMSHANGHAI_H__707193A3_82A9_49EE_8512_A17A620E1C46__INCLUDED_)
