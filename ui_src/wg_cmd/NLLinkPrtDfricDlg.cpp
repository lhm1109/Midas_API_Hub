// NLLinkPrtDfricDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "NLLinkPrtDfricDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtDfricDlg dialog


CNLLinkPrtDfricDlg::CNLLinkPrtDfricDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CNLLinkPrtDfricDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNLLinkPrtDfricDlg)
	//}}AFX_DATA_INIT
}


void CNLLinkPrtDfricDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNLLinkPrtDfricDlg)
	DDX_Control(pDX, IDC_CMD_NLLK_AREA_EDT   , m_edtArea);
	DDX_Control(pDX, IDC_CMD_NLLK_REFE_EDT   , m_edtRefe);
	DDX_Control(pDX, IDC_CMD_NLLK_STIF_OT_EDT, m_edtStif[0]);
	DDX_Control(pDX, IDC_CMD_NLLK_STIF_OB_EDT, m_edtStif[1]);
	DDX_Control(pDX, IDC_CMD_NLLK_UREF_OT_EDT, m_edtUref[0]);
	DDX_Control(pDX, IDC_CMD_NLLK_UREF_OB_EDT, m_edtUref[1]);
	DDX_Control(pDX, IDC_CMD_NLLK_RATE_OT_EDT, m_edtRate[0]);
	DDX_Control(pDX, IDC_CMD_NLLK_RATE_OB_EDT, m_edtRate[1]);
	DDX_Control(pDX, IDC_CMD_NLLK_RADI_OT_EDT, m_edtRadi[0]);
	DDX_Control(pDX, IDC_CMD_NLLK_RADI_OB_EDT, m_edtRadi[1]);
	DDX_Control(pDX, IDC_CMD_NLLK_STPD_OT_EDT, m_edtStpd[0]);
	DDX_Control(pDX, IDC_CMD_NLLK_STPD_OB_EDT, m_edtStpd[1]);
	DDX_Control(pDX, IDC_CMD_NLLK_COUV_OT_EDT, m_edtCouv[0]);
	DDX_Control(pDX, IDC_CMD_NLLK_COUV_OB_EDT, m_edtCouv[1]);
	DDX_Control(pDX, IDC_CMD_NLLK_COUP_OT_EDT, m_edtCoup[0]);
	DDX_Control(pDX, IDC_CMD_NLLK_COUP_OB_EDT, m_edtCoup[1]);
	DDX_Control(pDX, IDC_CMD_NLLK_COAP_OT_EDT, m_edtCoap[0]);
	DDX_Control(pDX, IDC_CMD_NLLK_COAP_OB_EDT, m_edtCoap[1]);
	DDX_Control(pDX, IDC_CMD_NLLK_COHP_OT_EDT, m_edtCohp[0]);
	DDX_Control(pDX, IDC_CMD_NLLK_COHP_OB_EDT, m_edtCohp[1]);
	DDX_Control(pDX, IDC_CMD_NLLK_AREA_UNT,    m_untArea);
	DDX_Control(pDX, IDC_CMD_NLLK_REFE_UNT,    m_untRefe);
	DDX_Control(pDX, IDC_CMD_NLLK_STIF_UNT,    m_untStif);
	DDX_Control(pDX, IDC_CMD_NLLK_RATE_UNT,    m_untRate);
	DDX_Control(pDX, IDC_CMD_NLLK_RADI_UNT,    m_untRadi);
	DDX_Control(pDX, IDC_CMD_NLLK_STPD_UNT, m_untStpd);
	DDX_Control(pDX, IDC_CMD_NLLK_DFRIC_BMP1, m_wndPicture);
	DDX_Control(pDX, IDC_CMD_NLLK_DFRIC_BMP2, m_wndPicture2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNLLinkPrtDfricDlg, CDialogMove)
	//{{AFX_MSG_MAP(CNLLinkPrtDfricDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtDfricDlg message handlers

BOOL CNLLinkPrtDfricDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	switch(m_nDofType)
	{    
		case 1: ShowData(1); break;
		case 2: ShowData(2); break;
	}
		
	m_untArea.SetUnitType(D_UNITSYS_BASE_AREA);
	m_untRefe.SetUnitType(D_UNITSYS_BASE_PRESSURE);
	m_untStif.SetUnitType(D_UNITSYS_BASE_SPRING);
	m_untRate.SetUnitType(D_UNITSYS_BASE_SEC_L);   //1unit
	m_untRadi.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untStpd.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_nllp-dfric1.svg"));
	m_wndPicture2.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_nllp-dfric2.svg"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNLLinkPrtDfricDlg::ShowData(int nFlag)
{
	m_edtArea.SetEditUnit(m_Data.NDP_Prop[nFlag].DFPSI.dAreaPressure);
	m_edtRefe.SetEditUnit(m_Data.NDP_Prop[nFlag].DFPSI.dRefAxialPressure);
	for(int i=0; i<2; i++)
	{
		m_edtStif[i].SetEditUnit(m_Data.NDP_Prop[nFlag].DFPSI.dStiffness[i]    );
		m_edtUref[i].SetEditUnit(m_Data.NDP_Prop[nFlag].DFPSI.dCoeff_uref[i]   );
		m_edtRate[i].SetEditUnit(m_Data.NDP_Prop[nFlag].DFPSI.dRateParam[i]    );
		m_edtRadi[i].SetEditUnit(m_Data.NDP_Prop[nFlag].DFPSI.dRadius[i]       );
		m_edtStpd[i].SetEditUnit(m_Data.NDP_Prop[nFlag].DFPSI.dStopDist[i]     );
		m_edtCouv[i].SetEditUnit(m_Data.NDP_Prop[nFlag].DFPSI.dRatioCoeff_uv[i]);
		m_edtCoup[i].SetEditUnit(m_Data.NDP_Prop[nFlag].DFPSI.dRatioCoeff_up[i]);
		m_edtCoap[i].SetEditUnit(m_Data.NDP_Prop[nFlag].DFPSI.dParamCoeff_ap[i]);
		m_edtCohp[i].SetEditUnit(m_Data.NDP_Prop[nFlag].DFPSI.dParamCoeff_hp[i]);
	}
}

void CNLLinkPrtDfricDlg::OnOK() 
{
	// TODO: Add extra validation here

	if(m_edtRefe.GetEditValue() > 0)
	{
		if(m_edtArea.GetEditValue() <= 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Stiffness)); 	return;
		}
	}
	for(int i=0; i<2; i++)
	{
		if(m_edtStif[i].GetEditValue() <= 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Stiffness)); 	return;
		}

		if(m_edtRate[i].GetEditValue() < 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_RateParameter)); 	return;
		}

		if(m_edtRadi[i].GetEditValue() < 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Radius)); 	return;
		}

		if(m_edtStpd[i].GetEditValue() <= 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Stop_Distance)); 	return;
		}

		if(m_edtCouv[i].GetEditValue() > 1.0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_COEFFI_SMALL_VEL_UV)); 	return;
		}

		if(m_edtCoup[i].GetEditValue() > 1.0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_COEFFI_HIGH_PRE_UP)); 	return;
		}
	}

	m_Data.NDP_Prop[m_nDofType].DFPSI.dAreaPressure     = m_edtArea.GetEditValue();
	m_Data.NDP_Prop[m_nDofType].DFPSI.dRefAxialPressure = m_edtRefe.GetEditValue();
	for(int i=0; i<2; i++)
	{
		m_Data.NDP_Prop[m_nDofType].DFPSI.dStiffness[i]     = m_edtStif[i].GetEditValue();
		m_Data.NDP_Prop[m_nDofType].DFPSI.dCoeff_uref[i]    = m_edtUref[i].GetEditValue();
		m_Data.NDP_Prop[m_nDofType].DFPSI.dRateParam[i]     = m_edtRate[i].GetEditValue();
		m_Data.NDP_Prop[m_nDofType].DFPSI.dRadius[i]        = m_edtRadi[i].GetEditValue();
		m_Data.NDP_Prop[m_nDofType].DFPSI.dStopDist[i]      = m_edtStpd[i].GetEditValue();
		m_Data.NDP_Prop[m_nDofType].DFPSI.dRatioCoeff_uv[i] = m_edtCouv[i].GetEditValue();
		m_Data.NDP_Prop[m_nDofType].DFPSI.dRatioCoeff_up[i] = m_edtCoup[i].GetEditValue();
		m_Data.NDP_Prop[m_nDofType].DFPSI.dParamCoeff_ap[i] = m_edtCoap[i].GetEditValue();
		m_Data.NDP_Prop[m_nDofType].DFPSI.dParamCoeff_hp[i] = m_edtCohp[i].GetEditValue();
	}

	CDialogMove::OnOK();
}

void CNLLinkPrtDfricDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}
