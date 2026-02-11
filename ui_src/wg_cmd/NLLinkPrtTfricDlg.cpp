// NLLinkPrtTfricDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "NLLinkPrtTfricDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtTfricDlg dialog


CNLLinkPrtTfricDlg::CNLLinkPrtTfricDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CNLLinkPrtTfricDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNLLinkPrtTfricDlg)
	//}}AFX_DATA_INIT
}


void CNLLinkPrtTfricDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNLLinkPrtTfricDlg)
	DDX_Check  (pDX, IDC_CMD_NLLK_SYM_OUT_CHK, m_bSymOut);
	DDX_Control(pDX, IDC_CMD_NLLK_STIF_OT_EDT, m_edtStif[0]);
	DDX_Control(pDX, IDC_CMD_NLLK_STIF_OB_EDT, m_edtStif[1]);
	DDX_Control(pDX, IDC_CMD_NLLK_STIF_IT_EDT, m_edtStif[2]);
	DDX_Control(pDX, IDC_CMD_NLLK_STIF_IB_EDT, m_edtStif[3]);
	DDX_Control(pDX, IDC_CMD_NLLK_FRCS_OT_EDT, m_edtFrcs[0]);
	DDX_Control(pDX, IDC_CMD_NLLK_FRCS_OB_EDT, m_edtFrcs[1]);
	DDX_Control(pDX, IDC_CMD_NLLK_FRCS_IT_EDT, m_edtFrcs[2]);
	DDX_Control(pDX, IDC_CMD_NLLK_FRCS_IB_EDT, m_edtFrcs[3]);
	DDX_Control(pDX, IDC_CMD_NLLK_FRCF_OT_EDT, m_edtFrcf[0]);
	DDX_Control(pDX, IDC_CMD_NLLK_FRCF_OB_EDT, m_edtFrcf[1]);
	DDX_Control(pDX, IDC_CMD_NLLK_FRCF_IT_EDT, m_edtFrcf[2]);
	DDX_Control(pDX, IDC_CMD_NLLK_FRCF_IB_EDT, m_edtFrcf[3]);
	DDX_Control(pDX, IDC_CMD_NLLK_RATE_OT_EDT, m_edtRate[0]);
	DDX_Control(pDX, IDC_CMD_NLLK_RATE_OB_EDT, m_edtRate[1]);
	DDX_Control(pDX, IDC_CMD_NLLK_RATE_IT_EDT, m_edtRate[2]);
	DDX_Control(pDX, IDC_CMD_NLLK_RATE_IB_EDT, m_edtRate[3]);
	DDX_Control(pDX, IDC_CMD_NLLK_RADI_OT_EDT, m_edtRadi[0]);
	DDX_Control(pDX, IDC_CMD_NLLK_RADI_OB_EDT, m_edtRadi[1]);
	DDX_Control(pDX, IDC_CMD_NLLK_RADI_IT_EDT, m_edtRadi[2]);
	DDX_Control(pDX, IDC_CMD_NLLK_RADI_IB_EDT, m_edtRadi[3]);
	DDX_Control(pDX, IDC_CMD_NLLK_STPD_OT_EDT, m_edtStpd[0]);
	DDX_Control(pDX, IDC_CMD_NLLK_STPD_OB_EDT, m_edtStpd[1]);
	DDX_Control(pDX, IDC_CMD_NLLK_STPD_IT_EDT, m_edtStpd[2]);
	DDX_Control(pDX, IDC_CMD_NLLK_STPD_IB_EDT, m_edtStpd[3]);
	DDX_Control(pDX, IDC_CMD_NLLK_H_IN_EDT,    m_edtHin);
	DDX_Control(pDX, IDC_CMD_NLLK_H_OUT_EDT,   m_edtHout);
	DDX_Control(pDX, IDC_CMD_NLLK_STIF_UNT,    m_untStif);
	DDX_Control(pDX, IDC_CMD_NLLK_RATE_UNT,    m_untRate);
	DDX_Control(pDX, IDC_CMD_NLLK_RADI_UNT,    m_untRadi);
	DDX_Control(pDX, IDC_CMD_NLLK_STPD_UNT,    m_untStpd);
	DDX_Control(pDX, IDC_CMD_NLLK_H_IN_UNT,    m_untHin);
	DDX_Control(pDX, IDC_CMD_NLLK_H_OUT_UNT, m_untHout);
	DDX_Control(pDX, IDC_CMD_NLLK_TFRIC_BMP1, m_wndPicture);
	DDX_Control(pDX, IDC_CMD_NLLK_TFRIC_BMP2, m_wndPicture2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNLLinkPrtTfricDlg, CDialogMove)
	//{{AFX_MSG_MAP(CNLLinkPrtTfricDlg)
	ON_BN_CLICKED(IDC_CMD_NLLK_SYM_OUT_CHK, OnSymmetricOutChk)
	ON_EN_CHANGE (IDC_CMD_NLLK_STIF_OT_EDT, OnOTValueChangeEdt)
	ON_EN_CHANGE (IDC_CMD_NLLK_FRCS_OT_EDT, OnOTValueChangeEdt)
	ON_EN_CHANGE (IDC_CMD_NLLK_FRCF_OT_EDT, OnOTValueChangeEdt)
	ON_EN_CHANGE (IDC_CMD_NLLK_RATE_OT_EDT, OnOTValueChangeEdt)
	ON_EN_CHANGE (IDC_CMD_NLLK_RADI_OT_EDT, OnOTValueChangeEdt)
	ON_EN_CHANGE (IDC_CMD_NLLK_STPD_OT_EDT, OnOTValueChangeEdt)
	ON_EN_CHANGE (IDC_CMD_NLLK_STIF_IT_EDT, OnITValueChangeEdt)
	ON_EN_CHANGE (IDC_CMD_NLLK_FRCS_IT_EDT, OnITValueChangeEdt)
	ON_EN_CHANGE (IDC_CMD_NLLK_FRCF_IT_EDT, OnITValueChangeEdt)
	ON_EN_CHANGE (IDC_CMD_NLLK_RATE_IT_EDT, OnITValueChangeEdt)
	ON_EN_CHANGE (IDC_CMD_NLLK_RADI_IT_EDT, OnITValueChangeEdt)
	ON_EN_CHANGE (IDC_CMD_NLLK_STPD_IT_EDT, OnITValueChangeEdt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtTfricDlg message handlers

BOOL CNLLinkPrtTfricDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	//CIVIL - 3253 : TFP UI 개선 반영#6 - 2
	//	    - 910 출시에는 사용못하도록 적용.추후 되돌릴 예정
	CDlgUtil::CtrlEnableDisableByRect(this, IDC_CMD_NLLK_H_GRP, FALSE, TRUE);

	// TODO: Add extra initialization here
	
	switch(m_nDofType)
	{    
		case 1: ShowData(1); break;
		case 2: ShowData(2); break;
	}
	
	m_untStif.SetUnitType(D_UNITSYS_BASE_SPRING);
	m_untRate.SetUnitType(D_UNITSYS_BASE_SEC_L);   //1unit
	m_untRadi.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untStpd.SetUnitType(D_UNITSYS_BASE_LENGTH);

	OnSymmetricOutChk();

	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_nllp-tfric1.svg"));
	m_wndPicture2.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_nllp-tfric2.svg"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNLLinkPrtTfricDlg::ShowData(int nFlag)
{
	m_bSymOut = m_Data.NDP_Prop[nFlag].TFPSI.bSymOut;

	for(int i=0; i<4; i++)
	{
		m_edtStif[i].SetEditUnit(m_Data.NDP_Prop[nFlag].TFPSI.dStiffness[i]    );
		m_edtFrcs[i].SetEditUnit(m_Data.NDP_Prop[nFlag].TFPSI.dFricCoeffSlow[i]);
		m_edtFrcf[i].SetEditUnit(m_Data.NDP_Prop[nFlag].TFPSI.dFricCoeffFast[i]);
		m_edtRate[i].SetEditUnit(m_Data.NDP_Prop[nFlag].TFPSI.dRateParam[i]    );
		m_edtRadi[i].SetEditUnit(m_Data.NDP_Prop[nFlag].TFPSI.dRadius[i]       );
		m_edtStpd[i].SetEditUnit(m_Data.NDP_Prop[nFlag].TFPSI.dStopDist[i]     );
	}
	
	m_edtHin.SetEditUnit(m_Data.NDP_Prop[nFlag].TFPSI.dHeightIn);
	m_edtHout.SetEditUnit(m_Data.NDP_Prop[nFlag].TFPSI.dHeightOut);

	UpdateData(FALSE);
}

void CNLLinkPrtTfricDlg::OnSymmetricOutChk() 
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_NLLK_OB_STC     )->EnableWindow(!m_bSymOut);
	GetDlgItem(IDC_CMD_NLLK_STIF_OB_EDT)->EnableWindow(!m_bSymOut);
	GetDlgItem(IDC_CMD_NLLK_FRCS_OB_EDT)->EnableWindow(!m_bSymOut);
	GetDlgItem(IDC_CMD_NLLK_FRCF_OB_EDT)->EnableWindow(!m_bSymOut);
	GetDlgItem(IDC_CMD_NLLK_RATE_OB_EDT)->EnableWindow(!m_bSymOut);
	GetDlgItem(IDC_CMD_NLLK_RADI_OB_EDT)->EnableWindow(!m_bSymOut);
	GetDlgItem(IDC_CMD_NLLK_STPD_OB_EDT)->EnableWindow(!m_bSymOut);
}

void CNLLinkPrtTfricDlg::OnOTValueChangeEdt()
{
	if(m_bSymOut)
	{
		// Outer Bottom = Outer Top
		double dStif = m_edtStif[0].GetEditValue();
		double dFrcs = m_edtFrcs[0].GetEditValue();
		double dFrcf = m_edtFrcf[0].GetEditValue();
		double dRate = m_edtRate[0].GetEditValue();
		double dRadi = m_edtRadi[0].GetEditValue();
		double dStpd = m_edtStpd[0].GetEditValue();

		m_edtStif[1].SetEditUnit(dStif);
		m_edtFrcs[1].SetEditUnit(dFrcs);
		m_edtFrcf[1].SetEditUnit(dFrcf);
		m_edtRate[1].SetEditUnit(dRate);
		m_edtRadi[1].SetEditUnit(dRadi);
		m_edtStpd[1].SetEditUnit(dStpd);
	}
}

void CNLLinkPrtTfricDlg::OnITValueChangeEdt()
{
	// Inner Bottom = Inner Top
	double dStif = m_edtStif[2].GetEditValue();
	double dFrcs = m_edtFrcs[2].GetEditValue();
	double dFrcf = m_edtFrcf[2].GetEditValue();
	double dRate = m_edtRate[2].GetEditValue();
	double dRadi = m_edtRadi[2].GetEditValue();
	double dStpd = m_edtStpd[2].GetEditValue();

	m_edtStif[3].SetEditUnit(dStif);
	m_edtFrcs[3].SetEditUnit(dFrcs);
	m_edtFrcf[3].SetEditUnit(dFrcf);
	m_edtRate[3].SetEditUnit(dRate);
	m_edtRadi[3].SetEditUnit(dRadi);
	m_edtStpd[3].SetEditUnit(dStpd);
}

BOOL CNLLinkPrtTfricDlg::DataCheck()
{

	for (int i = 0; i < 4; i++)
	{
		if (m_edtStif[i].GetEditValue() <= 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Stiffness)); 	return FALSE;
		}

		if (m_edtFrcs[i].GetEditValue() <= 0 || m_edtFrcf[i].GetEditValue() <= 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_FrictionCoefficient)); 	return FALSE;
		}

		if (m_edtRate[i].GetEditValue() < 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_RateParameter)); 	return FALSE;
		}

		if (m_edtRadi[i].GetEditValue() < 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Radius)); 	return FALSE;
		}

		if (m_edtStpd[i].GetEditValue() <= 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Stop_Distance)); 	return FALSE;
		}
	}

	if (m_edtHin.GetEditValue() < 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_H_IN)); 	return FALSE;
	}
	if (m_edtHout.GetEditValue() < 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_H_OUT)); 	return FALSE;
	}
	if(m_edtHin.GetEditValue()>0 || m_edtHout.GetEditValue()>0)
	{
		if (m_edtHin.GetEditValue() >= m_edtHout.GetEditValue())
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_H_IN_CHEK)); 	return FALSE;
		}
	}

	//0: Out Top, 1:Out Bot, 2:In Top, 3:In Bot
	// OutTop
	double dR_Top = m_edtRadi[0].GetEditValue(); // OutTop
	double dR_Bot = m_edtRadi[1].GetEditValue(); // OutBot
	double dR_In  = m_edtRadi[2].GetEditValue(); // InTop = InBot

	double dH_In  = m_edtHin.GetEditValue();
	double dH_Out = m_edtHout.GetEditValue();

	double dr = dR_Bot / (dR_Bot + dR_Top);

	double dL_In  = dR_In + dH_In / 2.0;
	double dL_Bot = dR_Bot - dr * dH_Out;
	double dL_Top = dR_Top - (1 - dr)*dH_Out;
	
	double duf_Top = m_edtFrcf[0].GetEditValue(); // OutTop
	double duf_Bot = m_edtFrcf[1].GetEditValue(); // OutBot
	double duf_In  = m_edtFrcf[2].GetEditValue(); // InTop = InBot
	double dus_Top = m_edtFrcs[0].GetEditValue(); // OutTop
	double dus_Bot = m_edtFrcs[1].GetEditValue(); // OutBot
	double dus_In  = m_edtFrcs[2].GetEditValue(); // InTop = InBot

	double dd_Top = m_edtStpd[0].GetEditValue(); // OutTop
	double dd_Bot = m_edtStpd[1].GetEditValue(); // OutBot
	double dd_In  = m_edtStpd[2].GetEditValue(); // InTop = InBot

	//////////////////////////////////////////////////////////////////////////
	// Test Friction Coefficient 

	// _T("Error : Frictional coefficient(uf, Outer Top) must be greater than or equal to Frictional coefficient(uf, Outer Bottom).")
	if (duf_Top < duf_Bot) 
	{ AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Frictional_coefficient_Check01)); 	return FALSE; }
	// _T("Error : Frictional coefficient(us, Outer Top) must be greater than or equal to Frictional coefficient(us, Outer Bottom).")
	if (dus_Top < dus_Bot) 
	{ AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Frictional_coefficient_Check02)); 	return FALSE; }
	// _T("Error : Frictional coefficient(uf, Outer Bottom) must be greater than or equal to Frictional coefficient(uf, Inner Top).")
	if (duf_Bot < duf_In) 
	{ AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Frictional_coefficient_Check03)); 	return FALSE; }
	// _T("Error : Frictional coefficient(us, Outer Bottom) must be greater than or equal to Frictional coefficient(us, Inner Top).")
	if (dus_Bot < dus_In) 
	{ AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Frictional_coefficient_Check04)); 	return FALSE; }

	//////////////////////////////////////////////////////////////////////////
	// Test Shear Force
	double dShear01 = duf_Bot + dd_Bot / dL_Bot;
	double dShear02 = duf_Top + dd_Top / dL_Top;
	if (dShear01 > dShear02) 
		return AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Frictional_coefficient_Check05));
	//////////////////////////////////////////////////////////////////////////

	return TRUE;
}

void CNLLinkPrtTfricDlg::OnOK() 
{
	//CIVIL-3253 : TFP UI 개선 내용 중 데이터 체크는 이전버전대로 되돌림.
    //             추후 다시 오픈 예정
	//if (!DataCheck()) return;

	// TODO: Add extra validation here
	m_Data.NDP_Prop[m_nDofType].TFPSI.bSymOut = m_bSymOut;

	for(int i=0; i<4; i++)
	{
		m_Data.NDP_Prop[m_nDofType].TFPSI.dStiffness[i]     = m_edtStif[i].GetEditValue();
		m_Data.NDP_Prop[m_nDofType].TFPSI.dFricCoeffSlow[i] = m_edtFrcs[i].GetEditValue();
		m_Data.NDP_Prop[m_nDofType].TFPSI.dFricCoeffFast[i] = m_edtFrcf[i].GetEditValue();
		m_Data.NDP_Prop[m_nDofType].TFPSI.dRateParam[i]     = m_edtRate[i].GetEditValue();
		m_Data.NDP_Prop[m_nDofType].TFPSI.dRadius[i]        = m_edtRadi[i].GetEditValue();
		m_Data.NDP_Prop[m_nDofType].TFPSI.dStopDist[i]      = m_edtStpd[i].GetEditValue();
	}

	m_Data.NDP_Prop[m_nDofType].TFPSI.dHeightIn = m_edtHin.GetEditValue();
	m_Data.NDP_Prop[m_nDofType].TFPSI.dHeightOut = m_edtHout.GetEditValue();

	CDialogMove::OnOK();
}

void CNLLinkPrtTfricDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}
