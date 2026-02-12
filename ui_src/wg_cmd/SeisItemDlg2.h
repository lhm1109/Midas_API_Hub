#if !defined(AFX_SEISITEMDLG2_H__8D4C2B21_412D_11D4_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_SEISITEMDLG2_H__8D4C2B21_412D_11D4_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\mit_frx\MComboBox.h"
using namespace mit::frx;
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CSeisItemDlg2 : public CDialogMove, public CDBUpdateConnector
{
	// Construction
public:
	CSeisItemDlg2(CWnd* pParent = NULL); // standard constructor

	//----------------------------------------------------------------------
	// Overrides
	//----------------------------------------------------------------------
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	//----------------------------------------------------------------------
	// Interface
public:
	void SetParamData(T_SEIS_K Key, T_SEIS_D& ParamData)
	{
		m_Key = m_OldKey = Key, m_Data = ParamData;
		m_bModify = TRUE;
	}

	void AddAdditionalLoad(T_STOR_K StartStorKey, T_STOR_K EndStorKey, double dblX, double dblY);
	BOOL IsSaveFoldedInfo() { return m_bSaveFoldedInfo; }
	//----------------------------------------------------------------------
	// Implementation
protected:
	void CtrlShowHide();
	void Data2Dlg();
	BOOL Dlg2Data();
	void AlignControl();

	void SetHeaderTitle();
	virtual void MakeItemEx();
	BOOL InsertItem(int nPos, T_SEIS_ADDITION& data);
	BOOL DeleteItem(int nPos);
	BOOL ModifyItem(int nPos, T_SEIS_ADDITION& data);
	CString DataToStr(int i, T_SEIS_ADDITION& data);

	int GetSoilFactorCode(int nCodeType, double dblSF);
	int GetZoneFactorCode(int nCodeType, double dblZF);
	void GetSoilFactorValue(int nCodeType, int nCode, double& dblSF);
	void GetZoneFactorValue(int nCodeType, int nCode, double& dblZF);
	void SetZoneFactorCombo(int nCodeType);
	void SetImpoFactorCombo(int nCodeType);
	void SetSoilFactorCombo(int nCodeType);
	double GetDampingFactorIS2002(double dDamping);

	BOOL ApplyOrOK();

protected:
	CDBDoc* m_pDoc;
	T_SEIS_K m_Key, m_OldKey;
	T_SEIS_D m_Data;
	BOOL m_bModify;

	// common
	double m_dHn;
	double m_dBx;
	double m_dBy;

	// KOR1992, KOR2000
	double m_dHXKOR;
	double m_dHYKOR;

	// KOR2000
	double m_dAcXKOR2000;
	double m_dAcYKOR2000;
	int m_nMtdXKOR2000;
	int m_nMtdYKOR2000;

	// KOR1992
	double m_dBXKOR1992;
	double m_dBYKOR1992;
	int m_nMtdXKOR1992;
	int m_nMtdYKOR1992;

	// IBC2000
	double m_dHXIBC2000;
	double m_dHYIBC2000;
	int m_nNXIBC2000;
	int m_nNYIBC2000;
	int m_nMtdXIBC2000;
	int m_nMtdYIBC2000;

	// EURO1996
	double m_dHXEURO1996;
	double m_dHYEURO1996;
	double m_dAcXEURO1996;
	double m_dAcYEURO1996;
	double m_nDXEURO1996;
	double m_nDYEURO1996;
	int m_nMtdXEURO1996;
	int m_nMtdYEURO1996;

	// CH2001
	double m_dHXCH2001;
	double m_dHYCH2001;
	double m_dBXCH2001;
	double m_dBYCH2001;
	int m_nNXCH2001;
	int m_nNYCH2001;
	int m_nMtdXCH2001;
	int m_nMtdYCH2001;

	// JPN2000
	double m_dHXJPN2000;
	double m_dHYJPN2000;
	double m_dAXJPN2000;
	double m_dAYJPN2000;

	// NBC1995
	double m_dHXNBC1995;
	double m_dHYNBC1995;
	double m_dBXNBC1995;
	double m_dBYNBC1995;
	int m_nNXNBC1995;
	int m_nNYNBC1995;
	int m_nMtdXNBC1995;
	int m_nMtdYNBC1995;

	// IS2002
	double m_dHXIS2002;
	double m_dHYIS2002;
	double m_dBXIS2002;
	double m_dBYIS2002;
	int m_nMtdXIS2002;
	int m_nMtdYIS2002;

	// TAIWAN86
	double m_dAcXTaiwan86;
	double m_dAcYTaiwan86;
	double m_dHnXTaiwan86;
	double m_dHnYTaiwan86;
	int m_nMtdXTaiwan86;
	int m_nMtdYTaiwan86;

	void InitCalcKSPeriod();

	//	void CalcForKS2000Ac();

	CArray<UINT, UINT> m_aCtrlEtc;
	CArray<UINT, UINT> m_aCtrlUbc97;
	CArray<UINT, UINT> m_aCtrlEccenX;
	CArray<UINT, UINT> m_aCtrlEccenY;
	CArray<UINT, UINT> m_aCtrlAtcEdit;
	CArray<UINT, UINT> m_aCtrlSfZf;
	CArray<UINT, UINT> m_aCtrlZone4;
	CArray<UINT, UINT> m_aCtrlJapan;
	CArray<UINT, UINT> m_aCtrlIbc;
	CArray<UINT, UINT> m_aCtrlEuro1996; // added by KYM
	CArray<UINT, UINT> m_aCtrlChina2002; // added by KYM
	CArray<UINT, UINT> m_aCtrlNbc1995;
	CArray<UINT, UINT> m_aCtrlIs2002;
	CArray<UINT, UINT> m_aCtrlTaiwan86;

	CStringArray m_aKS, m_aKS2000, m_aUBC91, m_aATC;

	// Dialog Data
	//{{AFX_DATA(CSeisItemDlg2)
	enum { IDD = IDD_ETC_SEIS_ITEM2 };

	CEditUnit m_wndTaiwan86TYFinal;
	CEditUnit m_wndTaiwan86TYAppr;
	CEditUnit m_wndTaiwan86TYAnal;
	CEditUnit m_wndTaiwan86TXFinal;
	CEditUnit m_wndTaiwan86TXAppr;
	CEditUnit m_wndTaiwan86TXAnal;
	MComboBox m_wndTaiwan86ZoneCmb;
	MComboBox m_wndTaiwan86SoilCmb;
	MComboBox m_wndTaiwan86RYCmb;
	MComboBox m_wndTaiwan86RXCmb;
	MComboBox m_wndTaiwan86ImportanceCmb;
	MComboBox m_wndTaiwan86AlphaCmb;
	MComboBox m_wndIs02ZoneCmb;
	MComboBox m_wndIs02SoilCmb;
	MComboBox m_wndIs02ImportanceCmb;
	CEditUnit m_wndIs02Damping;
	CEditUnit m_wndIs02DampingFactor;
	CEditUnit m_wndIs02Tx;
	CEditUnit m_wndIs02Ty;
	MComboBox m_wndIs02RxCmb;
	MComboBox m_wndIs02RyCmb;
	MComboBox m_wndNbc95ZoneRatio;
	MComboBox m_wndNbc95AcceZone;
	MComboBox m_wndNbc95VeloZone;
	MComboBox m_wndNbc95Importance;
	MComboBox m_wndNbc95Foundation;
	MComboBox m_wndNbc95Rx;
	MComboBox m_wndNbc95Ry;
	CEditUnit m_wndNbc95TxAnal;
	CEditUnit m_wndNbc95TyAnal;
	CEditUnit m_wndNbc95TxCode;
	CEditUnit m_wndNbc95TyCode;
	MComboBox m_wndChSDC;
	MComboBox m_wndChSFI;
	MComboBox m_wndChSC;
	MComboBox m_wndChDeltaN;
	CEditUnit m_wndChDamping;
	CEditUnit m_wndChPeriodY;
	CEditUnit m_wndChPeriodX;
	CEditUnit m_wndEuroPeriodY;
	CEditUnit m_wndEuroPeriodX;
	MComboBox m_wndEuroSc;
	MComboBox m_wndEuroQ0;
	MComboBox m_wndEuroKr;
	MComboBox m_wndEuroKd;
	CEditUnit m_wndEuroKw;
	CEditUnit m_wndEuroAlpha;
	CEditUnit m_wndIBCPeriodCoefY;
	CEditUnit m_wndIBCPeriodCalcY;
	CEditUnit m_wndIBCPeriodCodeY;
	CEditUnit m_wndIBCPeriodCoefX;
	CEditUnit m_wndIBCPeriodCalcX;
	CEditUnit m_wndIBCPeriodCodeX;
	MComboBox m_wndIBCCombo5;
	MComboBox m_wndIBCCombo4;
	MComboBox m_wndIBCCombo3;
	MComboBox m_wndIBCCombo2;
	MComboBox m_wndIBCCombo1;
	CEditUnit m_wndZone;
	CEditUnit m_wndShearFactor;
	MComboBox m_wndSPCombo;
	CEditUnit m_wndRmfYEdit3;
	CEditUnit m_wndRmfXEdit3;
	CEditUnit m_wndPeriodCY_JIS;
	CEditUnit m_wndPeriodCX_JIS;
	CEditUnit m_wndPeriodAY_JIS;
	CEditUnit m_wndPeriodAX_JIS;
	CButton m_btnCalcPeriod;
	CButton m_wndBtnApply;
	CListCtrl m_List;
	CEdit m_wndDescription;
	CEditUnit m_wndScaleGY;
	CEditUnit m_wndScaleGX;
	CSelectLC m_wndLoadCase;
	CEditUnit m_wndRmfYEdit2;
	CEditUnit m_wndRmfXEdit2;
	CEditUnit m_wndPeriodY2;
	CEditUnit m_wndPeriodX2;
	CEditUnit m_wndCdEdit2;
	MComboBox m_wndSstCombo2;
	MComboBox m_wndIfCombo2;
	MComboBox m_wndSptCombo2;
	MComboBox m_wndZfCombo2;
	CEditUnit m_wndPeriodCY;
	CEditUnit m_wndPeriodCX;
	CEditUnit m_wndPeriodAY;
	CEditUnit m_wndPeriodAX;
	CEditUnit m_wndRmfYEdit;
	CEditUnit m_wndRmfXEdit;
	CBCGPStatic m_wndRmfTitle;
	CBCGPStatic m_wndRmfTitle2;
	MComboBox m_wndSfCombo;
	CEditUnit m_wndSfEdit;
	CBCGPStatic m_wndSfTitle;
	MComboBox m_wndIfCombo;
	CBCGPStatic m_wndIfTitle;
	MComboBox m_wndZfCombo;
	CEditUnit m_wndZfEdit;
	CBCGPStatic m_wndZfTitle;
	MComboBox m_wndCodeName;
	BOOL m_wndChAlpha1;
	int m_wndChER;
	int m_nTaiwan86Period;
	//}}AFX_DATA

	BOOL m_bSaveFoldedInfo;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisItemDlg2)
protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSeisItemDlg2)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeCodeName();
	virtual void OnOK();
	afx_msg void OnCmdLoadcaseBtn();
	afx_msg void OnCmdApply();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnProfile();
	afx_msg void OnCmdBtnCalcPeriod();
	afx_msg void OnSelchangeCmdZfCombo2();
	afx_msg void OnCmdBtnCalcPeriod2();
	afx_msg void OnCmdIbcBtnCalcPeriod();
	afx_msg void OnCmdEuroBtnCalcPeriod(); // added by KYM 
	afx_msg void OnCmdChinaBtnCalcPeriod(); // added by KYM 
	afx_msg void OnCmdChinaErRdo();
	afx_msg void OnCmdChinaAlpha1Chk();
	afx_msg void OnCmdNbc95TCalcBtn();
	afx_msg void OnCmdIs02CalcPeriodBtn();
	afx_msg void OnChangeCmdIs02DampingEdit();
	afx_msg void OnCmdTaiwan86CalcPeriodBtn();
	afx_msg void OnCmdTaiwan86PeriodRdo();
	afx_msg void OnChangeTaiwan86PeriodEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEISITEMDLG2_H__8D4C2B21_412D_11D4_92DE_0000C0B0E6B3__INCLUDED_)
