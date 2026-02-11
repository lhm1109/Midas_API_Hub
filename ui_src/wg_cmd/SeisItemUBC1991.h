#if !defined(__SEISITEMUBC1991_H__)
#define __SEISITEMUBC1991_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisItemUBC1991.h : header file
//
#include "..\MIT_frx\MComboBox.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\FoldGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisItemUBC1991 dialog

class CSeisItemDlgNew;

class CSeisItemUBC1991 : public CChildDialog
{
public:
	CSeisItemUBC1991(CWnd* pParent = NULL); // standard constructor

	void SetData2Dlg(T_SEIS_UBC1991& data, BOOL bModify);
	BOOL SetDlg2Data(T_SEIS_UBC1991& data);

	enum { IDD = IDD_ETC_SEIS_ITEM_UBC1991 };

	mit::frx::MComboBox m_cobxSFactor;
	mit::frx::MComboBox m_cobxZFactor;
	mit::frx::MComboBox m_cobxIFactor;
	CEditUnit m_editAX;
	CEditUnit m_editAY;
	CEditUnit m_editCX;
	CEditUnit m_editCY;
	CEditUnit m_editRwX;
	CEditUnit m_editRwY;
	CFoldGroupBox_Cross m_grpFold;

public:
	virtual BOOL DestroyWindow();

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

protected:
	void Data2Dlg();
	BOOL Dlg2Data();

	void InitSFactorCombo();
	void InitZFactorCombo();
	void InitIFactorCombo();
	void SetFoldGroupCtrls();

protected:
	BOOL m_bModify;
	CSeisItemDlgNew* m_pParent;

	T_SEIS_UBC1991 m_Data;

	CStringArray m_aSoilName;
	CArray<double, double> m_aSoilData;
	CStringArray m_aZoneName;
	CArray<double, double> m_aZoneData;
	CStringArray m_aIFactorName;
	CArray<double, double> m_aIFactorData;

protected:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(__SEISITEMUBC1991_H__)
