#if !defined(__SEISITEMUBC1997_H__)
#define __SEISITEMUBC1997_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisItemUBC1997.h : header file
//
#include "..\MIT_frx\MComboBox.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisItemUBC1997 dialog
class CSeisItemDlgNew;

class CSeisItemUBC1997 : public CChildDialog
{
public:
	CSeisItemUBC1997(CWnd* pParent = NULL); // standard constructor

	void SetData2Dlg(T_SEIS_UBC1997& data, BOOL bModify);
	BOOL SetDlg2Data(T_SEIS_UBC1997& data);
	enum { IDD = IDD_ETC_SEIS_ITEM_UBC1997 };

	mit::frx::MComboBox m_cobxSoilType;
	mit::frx::MComboBox m_cobxZFactor;
	mit::frx::MComboBox m_cobxSType;
	CBCGPStatic m_unitDistance;
	CEditUnit m_editDistance;
	mit::frx::MComboBox m_cobxIFactor;
	CFoldGroupBox_Cross m_grpFold;

	//6.5.0 추가
	CEditUnit m_editPXAnal;
	CEditUnit m_editPYAnal;

	CEditUnit m_editPX;
	CEditUnit m_editPY;
	CEditUnit m_editRX;
	CEditUnit m_editRY;

public:
	virtual BOOL DestroyWindow();

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

protected:
	void Data2Dlg();
	BOOL Dlg2Data();

	void InitDefaultData(); // 6.5.0 추가...by hp
	void InitSoilTypeCombo();
	void InitZFactorCombo();
	void InitSrcTypeCombo();
	void InitIFactorCombo();
	void GetZoneFactorValue(int nCodeType, int nCode, double& dblZF);
	void SetFoldGroupCtrls();

protected:
	BOOL m_bInit; // 6.5.0 추가...by hp
	BOOL m_bModify;
	CSeisItemDlgNew* m_pParent;
	T_SEIS_UBC1997 m_Data;

	double m_dHXUBC1997;
	double m_dHYUBC1997;
	int m_nMtdXUBC1997;
	int m_nMtdYUBC1997;

	CStringArray m_aZoneName;
	CArray<double, double> m_aZoneData;
	CStringArray m_aIFactorName;
	CArray<double, double> m_aIFactorData;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmdZfCombo();
	afx_msg void OnCmdUbcBtnCalcPeriod(); // 6.5.0 추가...by hp
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(__SEISITEMUBC1997_H__)
