#if !defined(__ETC_GDS_CH_JTG_T_2231_01_2020_H__)
#define __ETC_GDS_CH_JTG_T_2231_01_2020_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_CH_JTG_T_2231_01_2020.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

#include "MyChildDialog.h"


class CSpfcCHJTG2020Util;

class CETC_GDS_CH_JTG_T_2231_01_2020 : public CMyChildDialog
{

//Construction
public:
	CETC_GDS_CH_JTG_T_2231_01_2020(T_SPFC_D* pParamData, CWnd* pParent = NULL);
	void End();
	void EndNotClose() {};
	BOOL ISVALID();
	BOOL IsValid_d(CEdit* pEdt);

	BOOL m_bVertResSpec;

	CEditUnit m_edtDampingRatio;
	CEditUnit m_edtTg;
	CEditUnit m_edtCi;
	CEditUnit m_edtCs;
	CEditUnit m_edtCd;
	CEditUnit m_edtA;
	CEditUnit m_edtSMax;
	CEditUnit m_edtMaxPeriod;

	MButton m_chkLargeBridge;
	MButton m_chkVertResSpec;

	MillustViewer m_ImgViewer;

	// constructor 에서 T_SPFC_D m_ParamData의 주소를 넘겨받음.
	BOOL Dlg2Data();
	void Data2Dlg();
	void Initialize();
	void ShowDlgData();
	void Dlg2UtilData() override;
	void Button2UtilData();
	void UtilData2Button();

	enum { IDD = IDD_ETC_GDS_CH_JTG_T_2231_01_2020 };
	
	// Implementation
protected:
	CArray<UINT, UINT> m_aBridgeType;
	CArray<UINT, UINT> m_aZoningMap;
	CArray<UINT, UINT> m_aSiteTpye;
	CArray<UINT, UINT> m_aSeismicIntensity;
	CArray<UINT, UINT> m_aSeismicCriterion;

protected:
	T_SPFC_D   *m_pParamData;
	T_SPFC_CODE m_CodeD;
	std::shared_ptr<CSpfcCHJTG2020Util> m_pSpfcUtil;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnCalcOutputData();
	afx_msg void OnChaneInputData();
	afx_msg void OnChaneOutputData();
	afx_msg void OnClickLargeBridge();
	afx_msg void OnClickVertResSpec();
	DECLARE_MESSAGE_MAP()
};
#endif