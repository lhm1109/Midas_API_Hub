#if !defined(__CMSPLCSUBCALCECCNDLG_H__)
#define __CMSPLCSUBCALCECCNDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSplcSubDlg.h : header file
//

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

enum { EN_STATIC, EN_RS };

enum EN_ECCN_DGN_CODE
{
	EN_FIXED_ECCENTRICITY = 0,
	EN_DGN_CODE_NTCS_2023,
	EN_DGN_CODE_NONE,
};

class CDBDoc;
class CCMSplcSubCalcEccnDlg : public CDialogMove
{
public:
	CCMSplcSubCalcEccnDlg(CWnd* pParent = NULL);
	virtual ~CCMSplcSubCalcEccnDlg();

	enum { IDD = IDD_DBAR_SPLC_SUB_CALC_ECCN_DLG };
	CEdit	m_edtPercent;
	CEdit	m_edtMultiple;
	CEdit	m_edtRatio;
	mit::frx::MButton m_chkConsiderEccnbyAnal;
	mit::frx::MComboBox m_cmbDesignCode;

	CRect m_InitDlgRect;
	CRect m_InitGroupRect;
	CRect m_InitOkRect;
	CRect m_InitCancelRect;

	double m_dPercent4PlanDimEccn;
	double m_dMultiple4TopStoryEccn;
	double m_dRatio4AnalEccn;
	bool   m_bConsiderEccnbyAnal;
	UINT   m_nDlgType;
	UINT   m_nDgnCode;

	CArray<UINT, UINT> m_arNotFixedEccn;
	CArray<UINT, UINT> m_arConsiderEccn4Anal;

public:
	void GetInputData(OUT double& dPercent4PlanDimEccn, OUT double& dMultiple4TopStoryEccn, OUT bool& bConsiderEccnbyAnal, OUT double& dRatio4AnalEccn);
	void SetDlgType(UINT nType);
	UINT GetDgnCodeSellected();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CDBDoc* m_pDoc;

protected:
	void Data2Dlg();
	BOOL Dlg2Data();

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();

	void CheckCtrlEnable();
	void InitCombo();
	void SetBitmap();
	void GetDgnCodeEnum(UINT& enVal);
	void ResetDilaog();
	void AdjustDlg4DgnCode();

	afx_msg void OnChkConsiderEccnbyAnal();

	CStatic	m_wndBmp;
	CBitmap m_Bitmap;

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(__CMSPLCSUBCALCECCNDLG_H__)
