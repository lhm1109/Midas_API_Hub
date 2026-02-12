#if !defined(AFX_CCMTendonSetLongTermStressLossDlg_H__8D52BAAE_F6B4_4741_8BF2_06CC99341743__INCLUDED_)
#define AFX_CCMTendonSetLongTermStressLossDlg_H__8D52BAAE_F6B4_4741_8BF2_06CC99341743__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"

class CDBDoc;
class CCMTendonSetLongTermStressLossDlg : public CChildDialogMove
{
public:
	CCMTendonSetLongTermStressLossDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_CMD_DEF_TENDON_SET_LONG_TERM_STRESS_LOSS_DLG };

//	int m_nLongTermLossType;
	int m_nStrandType;
//	double m_dEstForce;
	double m_dConcAge;
	double m_dConcElasticity;
	double m_dRelativeHumidity;
	double m_dVSRatio;
	double m_dAvgPreComp;
	double m_dUltCreepCoef;
	double m_dTensFactor;
	double m_dShrinkageConst;
	double m_dRelaxConst;
	double m_dRelaxReduct;
	BOOL m_bAuto;

//	CTextUnit m_unitEstForce;
	CTextUnit m_unitConcElasticity;
	CTextUnit m_unitVS;
	CTextUnit m_unitAvgPreComp;
	CTextUnit m_unitRelaxConst;

public:
	void SetData(T_TDNT_LTSL& Data);
	void GetData(T_TDNT_LTSL& Data);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	void SetControl();
	void SetUnitType();

	void Data2Dlg();
	bool Dlg2Data();

protected:
	CDBDoc* m_pDoc;

	virtual BOOL OnInitDialog();
	virtual void OnOK();

	afx_msg void OnStrandType();
	afx_msg void OnApply();

	DECLARE_MESSAGE_MAP()

private:
	T_TDNT_LTSL m_LtslData;
};

#endif // !defined(AFX_CCMTendonSetLongTermStressLossDlg_H__8D52BAAE_F6B4_4741_8BF2_06CC99341743__INCLUDED_)