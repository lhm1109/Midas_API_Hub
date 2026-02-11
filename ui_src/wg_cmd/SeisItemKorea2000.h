#if !defined(__SEISITEMKOREA2000_H__)
#define __SEISITEMKOREA2000_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisItemKorea2000.h : header file
//
#include "..\MIT_frx\MComboBox.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\FoldGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisItemKorea2000 dialog
class CSeisItemDlgNew;

class CSeisItemKorea2000 : public CChildDialog
{
// Construction
public:
	CSeisItemKorea2000(CWnd* pParent = NULL);   // standard constructor

	void SetData2Dlg(T_SEIS_KS2000& data, BOOL bModify);
	BOOL SetDlg2Data(T_SEIS_KS2000& data);

// Dialog Data
	enum { IDD = IDD_ETC_SEIS_ITEM_KR2000 };
	mit::frx::MComboBox	m_cobxSFactor;
	mit::frx::MComboBox	m_cobxZFactor;
	mit::frx::MComboBox	m_cobxIFactor;
	CEditUnit	m_editAX;
	CEditUnit	m_editAY;
	CEditUnit	m_editCX;
	CEditUnit	m_editCY;
	CEditUnit	m_editRX;
	CEditUnit	m_editRY;
	CFoldGroupBox_Cross m_grpFold;

public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	void Data2Dlg();
	BOOL Dlg2Data();

	void InitDefaultData();
	void InitSFactorCombo();
	void InitZFactorCombo();
	void InitIFactorCombo();
	void SetFoldGroupCtrls();

protected:
	BOOL m_bInit;
	BOOL m_bModify;
	CSeisItemDlgNew* m_pParent;
	T_SEIS_KS2000  m_Data;

 	double	m_dHXKOR;	
	double	m_dHYKOR;	
	double	m_dAcXKOR2000;	
	double	m_dAcYKOR2000;
	int		  m_nMtdXKOR2000;
	int			m_nMtdYKOR2000;

	CStringArray m_aSoilName;
	CArray<double, double> m_aSoilData;
	CStringArray m_aAreaName;
	CArray<double, double> m_aAreaData;
	CStringArray m_aIFactorName;
	CArray<double, double> m_aIFactorData;

// Implementation
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdBtnCalcPeriod();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
#endif // !defined(__SEISITEMKOREA2000_H__)
