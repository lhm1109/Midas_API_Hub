/////////////////////////////////////////////////////////////////////////////////////////////////
//(2009.07.10) Add by Unsang :: SPEC변경에 따라 해당 입력창은 기존 방식으로 적용한다.
/////////////////////////////////////////////////////////////////////////////////////////////////

// DgnConBarRatioDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConBarRatioDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_main\wg_mainres2.h"	// For ID_DGN_CON_DCON_RMAX
#include "..\wg_base\DlgUtil.h"
#include "NationalAnnexTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConBarRatioDlg dialog
using namespace dgn::def;

CDgnConBarRatioDlg::CDgnConBarRatioDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnConBarRatioDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConBarRatioDlg)
	m_Rhoc = 0.0;
	m_Rhor = 0.0;
	m_Rhow = 0.0;
	m_strCode = _T("");
	m_strCode2 = _T("");

	m_bBeam = TRUE;
	m_bColm = TRUE;
	m_bBrce = TRUE;
	m_bWall = TRUE;
	m_bSlab = TRUE;
	m_bMat = TRUE;

	m_nNationalAnnex = -1;
	m_strNationalAnnex = _T("");
	//}}AFX_DATA_INIT
}


void CDgnConBarRatioDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConBarRatioDlg)
	DDX_Text(pDX, IDC_DGN_RCCTRL_RHOC, m_Rhoc);
	DDX_Text(pDX, IDC_DGN_RCCTRL_RHOR, m_Rhor);
	DDX_Text(pDX, IDC_DGN_RCCTRL_RHOW, m_Rhow);
	DDX_Text(pDX, IDC_DGN_STATIC_RCCODE, m_strCode);
	DDX_Text(pDX, IDC_DGN_STATIC_RCCODE2, m_strCode2);

	DDX_Check(pDX, IDC_DGN_CON_RBAR_LIMIT_FRAME3_BEAM_CHK, m_bBeam);
	DDX_Check(pDX, IDC_DGN_CON_RBAR_LIMIT_FRAME3_COLM_CHK, m_bColm);
	DDX_Check(pDX, IDC_DGN_CON_RBAR_LIMIT_FRAME3_BRCE_CHK, m_bBrce);
	DDX_Check(pDX, IDC_DGN_CON_RBAR_LIMIT_FRAME3_WALL_CHK, m_bWall);
	DDX_Check(pDX, IDC_DGN_CON_RBAR_LIMIT_FRAME3_SLAB_CHK, m_bSlab);
	DDX_Check(pDX, IDC_DGN_CON_RBAR_LIMIT_FRAME3_MAT_CHK, m_bMat);
	DDX_Check(pDX, IDC_DGN_CON_RBAR_LIMIT_FRAME3_SHELL_CHK, m_bShell);

	DDX_Control(pDX, IDC_DGN_CON_RBAR_HOR_EDIT, m_edtShellHorz);
	DDX_Control(pDX, IDC_DGN_CON_RBAR_VERT_EDIT, m_edtShellVert);

	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnConBarRatioDlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnConBarRatioDlg)
	ON_BN_CLICKED(IDC_DGN_CON_RBAR_LIMIT_FRAME3_SHELL_CHK, OnChkShell)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConBarRatioDlg message handlers

BOOL CDgnConBarRatioDlg::OnInitDialog()
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	// Limit Maximum Rebar Ratio : Except EC2-2:05
	m_iRBARRATIOID1[0] = IDC_DGN_CON_RBAR_RATIO_FRAME;
	m_iRBARRATIOID1[1] = IDC_DGN_RCCTRL_RHOW_STC;	
	m_iRBARRATIOID1[2] = IDC_DGN_RCCTRL_RHOW;
	m_iRBARRATIOID1[3] = IDC_DGN_RCCTRL_RHOC_STR;	
	m_iRBARRATIOID1[4] = IDC_DGN_RCCTRL_RHOC;
	m_iRBARRATIOID1[5] = IDC_DGN_RCCTRL_RHOR_STR;	
	m_iRBARRATIOID1[6] = IDC_DGN_RCCTRL_RHOR;

	m_iRBARRATIOID2[0] = IDC_DGN_CON_RBAR_LIMIT_FRAME1;
	m_iRBARRATIOID2[1] = IDC_DGN_CON_RBAR_LIMIT_STR3;	
	m_iRBARRATIOID2[2] = IDC_DGN_CON_RBAR_LIMIT_EDIT1;

	// Limit Maximum/Minimum Rebar Ratio : Only EC2-2:05
	m_iRBARRATIOID3[0] = IDC_DGN_CON_RBAR_LIMIT_FRAME2;
	m_iRBARRATIOID3[1] = IDC_DGN_CON_RBAR_LIMIT_STR5;	
	m_iRBARRATIOID3[2] = IDC_DGN_CON_RBAR_LIMIT_EDIT2;

	// Limiting Rebar Ratio : Only Eurocode2:04, Italy.
	m_iRBARRATIOID4[0] = IDC_DGN_CON_RBAR_LIMIT_FRAME3;
	m_iRBARRATIOID4[1] = IDC_DGN_CON_RBAR_LIMIT_FRAME3_BEAM_CHK;
	m_iRBARRATIOID4[2] = IDC_DGN_CON_RBAR_LIMIT_FRAME3_COLM_CHK;
	m_iRBARRATIOID4[3] = IDC_DGN_CON_RBAR_LIMIT_FRAME3_BRCE_CHK;
	m_iRBARRATIOID4[4] = IDC_DGN_CON_RBAR_LIMIT_FRAME3_WALL_CHK;
	m_iRBARRATIOID4[5] = IDC_DGN_CON_RBAR_LIMIT_FRAME3_SLAB_CHK;
	m_iRBARRATIOID4[6] = IDC_DGN_CON_RBAR_LIMIT_FRAME3_MAT_CHK;
	m_iRBARRATIOID4[7] = IDC_DGN_CON_RBAR_LIMIT_FRAME3_SHELL_CHK;
	m_iRBARRATIOID4[8] = IDC_DGN_CON_RBAR_HOR_STR;	
	m_iRBARRATIOID4[9] = IDC_DGN_CON_RBAR_HOR_EDIT;
	m_iRBARRATIOID4[10] = IDC_DGN_CON_RBAR_VERT_STR;	
	m_iRBARRATIOID4[11] = IDC_DGN_CON_RBAR_VERT_EDIT;
	m_iRBARRATIOID4[12] = IDC_DGN_SHELL_GRP;

	// SET NATIONAL ANNEX
	T_DCON_D rData;
	rData.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetDcon(rData))
	{
		T_PREFERENCE rPref;
		rPref.Initialize();
		m_pDoc->m_pInitCtrl->GetPreference(rPref);

		rData.DesignCode = rPref.DgnCode.ConcCode;
		rData.nNationalAnnex = rPref.DgnCode.nConcNationalAnnex;
	}

	m_nNationalAnnex = rData.nNationalAnnex;
	m_strNationalAnnex = CNationalAnnexTool::ConvertNationalAnnex(m_nNationalAnnex);

	if (m_pDoc->m_pAttrCtrl->ExistDcon())  Initial_ExistData();
	else                                  Initial_DefaultData();

	Update_InitDataByCode(m_strCode);

	EnableCtrlDynamic();

	ResizeDialog();

	return TRUE;
}

void CDgnConBarRatioDlg::Initial_DefaultData()
{
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);
	m_strCode = rPref.DgnCode.ConcCode;
	m_nNationalAnnex = rPref.DgnCode.nConcNationalAnnex;

	if (m_strCode != CONCODE_EC2_2_05 && m_strCode != CONCODE_EC2_04)
	{
		//m_Rhor  = 0.03;
		m_Rhor = m_strCode == CONCODE_IRC112_2011 ? 0.025 : 0.03;
		m_Rhor = m_strCode == CONCODE_IRC112_2020 ? 0.025 : 0.03;

		if (m_strCode == CONCODE_EC2_04 || m_strCode == CONCODE_NSR_10 ||
			m_strCode == CONCODE_AS5100_5_17)
			m_Rhoc = 0.04;
		else 
			m_Rhoc = 0.03;

		m_Rhow = 0.04;
		if (m_strCode == CONCODE_AASHTO_LRFD07 || m_strCode == CONCODE_AASHTO_LRFD12 ||
			m_strCode == CONCODE_AASHTO_LRFD16 || m_strCode == CONCODE_AASHTO_LRFD17 ||
			m_strCode == CONCODE_AASHTO_LRFD20 || m_strCode == CONCODE_AASHTO_LRFD24 ||
			m_strCode == CONCODE_AREMA_2023)
		{
			m_Rhor = 0.08;
			m_Rhoc = 0.08;
		}

		if ( m_strCode == CONCODE_BS5400 )
		{
            m_Rhor = 0.06;
            m_Rhoc = 0.06;
		}

		m_bShell = TRUE;
		m_edtShellHorz.SetEditUnit(0.001);
		m_edtShellVert.SetEditUnit(0.002);
	}
	else
	{
		double dShellDir1 = 0.0, dShellDir2 = 0.0;

		T_DCON_D  rDconD;
		rDconD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetDcon(rDconD))
		{
			T_PREFERENCE rPref;
			rPref.Initialize();
			m_pDoc->m_pInitCtrl->GetPreference(rPref);

			rDconD.DesignCode = rPref.DgnCode.ConcCode;
			rDconD.nNationalAnnex = rPref.DgnCode.nConcNationalAnnex;
		}
		m_strNationalAnnex = CNationalAnnexTool::ConvertNationalAnnex(rDconD.nNationalAnnex);

		if (m_strCode == CONCODE_EC2_04 || m_strCode == CONCODE_EC2_2_05)
			m_strCode2.Format(_T("%s, %s"), m_strCode, m_strNationalAnnex);

		double dMax = 0.0;
		double dMin = 0.0;
		int iStatus = GetDataStatus(m_strCode);
		SetDatabyNationalAnnex(iStatus, m_nNationalAnnex, dMax, dMin, dShellDir1, dShellDir2);

		//m_Rhor  = 0.03;
		m_Rhor = m_strCode == CONCODE_IRC112_2011 ? 0.025 : 0.03;
		m_Rhor = m_strCode == CONCODE_IRC112_2020 ? 0.025 : 0.03;

		if (m_strCode == CONCODE_EC2_04 || m_strCode == CONCODE_NSR_10 || m_strCode == CONCODE_AS5100_5_17)
			m_Rhoc = 0.04;
		else 
			m_Rhoc = dMax;

		m_Rhow = 0.04;
		m_bBeam = rDconD.bMinRebarBeam;
		m_bColm = rDconD.bMinRebarColumn;
		m_bBrce = rDconD.bMinRebarBrace;
		m_bWall = rDconD.bMinRebarWall;
		m_bSlab = rDconD.bMinRebarSlab;
		m_bMat = rDconD.bMinRebarMat;

		m_bShell = TRUE;
		m_edtShellHorz.SetEditUnit(dShellDir2);
		m_edtShellVert.SetEditUnit(dShellDir1);
	}

	UpdateData(FALSE);

	InitDlgCtrls();
}

void CDgnConBarRatioDlg::Initial_ExistData() // Data2Dlg
{
	T_DCON_D rData;
	rData.Initialize();
	m_pDoc->m_pAttrCtrl->GetDcon(rData);

	double dMax = 0.0;
	double dMin = 0.0;

	double dShellDir1 = 0.0, dShellDir2 = 0.0;
	if (rData.DesignCode == _T(""))
	{
		T_PREFERENCE rPref;
		rPref.Initialize();
		m_pDoc->m_pInitCtrl->GetPreference(rPref);
		m_strCode = rPref.DgnCode.ConcCode;
		m_nNationalAnnex = rPref.DgnCode.nConcNationalAnnex;
		m_strNationalAnnex = CNationalAnnexTool::ConvertNationalAnnex(m_nNationalAnnex);

		if (m_strCode == CONCODE_EC2_04 || m_strCode == CONCODE_EC2_2_05)
			m_strCode2.Format(_T("%s, %s"), m_strCode, m_strNationalAnnex);

		m_Rhor = 0.03;
		m_Rhoc = 0.03;
		m_Rhow = 0.04;

		if (m_strCode != CONCODE_EC2_2_05)
		{
			if (m_strCode == CONCODE_AASHTO_LRFD07 ||
				m_strCode == CONCODE_AASHTO_LRFD12 ||
				m_strCode == CONCODE_AASHTO_LRFD16 ||
				m_strCode == CONCODE_AASHTO_LRFD17 ||
                m_strCode == CONCODE_AASHTO_LRFD20 ||
				m_strCode == CONCODE_AASHTO_LRFD24)
			{
				m_Rhor = 0.08;
				m_Rhoc = 0.08;
			}
			else if (m_strCode == CONCODE_IRC112_2011)
			{
				m_Rhor = 0.025;
				m_Rhoc = 0.04;

			}
			else if (m_strCode == CONCODE_IRS)
			{
				m_Rhor = 0.04;
				m_Rhoc = 0.06;

			}
			else if (m_strCode == CONCODE_IRC112_2020)
			{
				m_Rhor = 0.025;
				m_Rhoc = 0.04;

			}
			else if (m_strCode == CONCODE_AREMA_2023)
			{
				m_Rhoc = 0.08;
				m_Rhor = 0.03;
			}
			else if (m_strCode == CONCODE_IS456_2000)
			{
				m_Rhoc = 0.04;
			}
			else if (m_strCode == CONCODE_EC2_04 || m_strCode == CONCODE_NSR_10 || m_strCode == CONCODE_AS5100_5_17)
			{
				m_Rhoc = 0.04;
			}
			else
			{
				m_Rhoc = 0.03;
			}
		}

		else
		{
			int iStatus = GetDataStatus(m_strCode);
			SetDatabyNationalAnnex(iStatus, m_nNationalAnnex, dMax, dMin, dShellDir1, dShellDir2);
			m_Rhoc = dMax;
		}
	}
	else
	{
		m_strCode = rData.DesignCode;
		if (m_strCode == CONCODE_EC2_04 || m_strCode == CONCODE_EC2_2_05)
			m_strCode2.Format(_T("%s, %s"), m_strCode, m_strNationalAnnex);

		m_Rhor = (rData.dRhor == 0.0 ? 0.03 : rData.dRhor);
		m_Rhoc = (rData.dRhoc == 0.0 ? 0.03 : rData.dRhoc);
		m_Rhow = (rData.dRhow == 0.0 ? 0.04 : rData.dRhow);

		if (m_strCode != CONCODE_EC2_2_05)
		{
			if (m_strCode == CONCODE_AASHTO_LRFD07 || m_strCode == CONCODE_AASHTO_LRFD12 ||
				m_strCode == CONCODE_AASHTO_LRFD16 || m_strCode == CONCODE_AASHTO_LRFD17 ||
				m_strCode == CONCODE_AASHTO_LRFD20 || m_strCode == CONCODE_AASHTO_LRFD24 )
			{
				m_Rhor = (rData.dRhor == 0.0) ? 0.08 : rData.dRhor;
				m_Rhoc = (rData.dRhoc == 0.0) ? 0.08 : rData.dRhoc;
			}
			else if (m_strCode == CONCODE_IRC112_2011)
			{
				m_Rhoc = (rData.dRhoc == 0.0) ? 0.04 : rData.dRhoc;
				m_Rhor = (rData.dRhor == 0.0) ? 0.025 : rData.dRhor;
			}
			else if (m_strCode == CONCODE_AREMA_2023)
			{
				m_Rhoc = (rData.dRhoc == 0.0) ? 0.08 : rData.dRhoc;
				m_Rhor = (rData.dRhor == 0.0) ? 0.03 : rData.dRhor;
			}
			else if (m_strCode == CONCODE_IRC112_2020)
			{
				m_Rhoc = (rData.dRhoc == 0.0) ? 0.04 : rData.dRhoc;
				m_Rhor = (rData.dRhor == 0.0) ? 0.025 : rData.dRhor;
			}
			else if (m_strCode == CONCODE_IRS)
			{
				m_Rhor = 0.04;
				m_Rhoc = 0.06;
			}
			else if (m_strCode == CONCODE_EC2_04 || m_strCode == CONCODE_NSR_10 ||
					 m_strCode == CONCODE_AS5100_5_17)
			{
				m_Rhoc = (rData.dRhoc == 0.0 ? 0.04 : rData.dRhoc);
			}
			else if (m_strCode == CONCODE_TWN_USD112)
			{
				m_Rhoc = (rData.dRhoc == 0.0 ? 0.06 : rData.dRhoc);
			}
			else if ( m_strCode == CONCODE_BS5400_90 )
			{
                m_Rhoc = (rData.dRhoc == 0.0) ? 0.06 : rData.dRhoc;
                m_Rhor = (rData.dRhor == 0.0) ? 0.06 : rData.dRhor;
			}
			else if (m_strCode == CONCODE_TMH07_89)
			{
				m_Rhoc = (rData.dRhoc == 0.0) ? 0.06 : rData.dRhoc;
				m_Rhor = (rData.dRhor == 0.0) ? 0.06 : rData.dRhor;
			}
			else
			{
				m_Rhoc = (rData.dRhoc == 0.0 ? 0.03 : rData.dRhoc);
			}
		}
		else
		{
			int iStatus = GetDataStatus(m_strCode);
			SetDatabyNationalAnnex(iStatus, m_nNationalAnnex, dMax, dMin, dShellDir1, dShellDir2);
			m_Rhoc = (rData.dRhoc == 0.0 ? dMax : rData.dRhoc);
		}

	}

	m_bBeam = rData.bMinRebarBeam;
	m_bColm = rData.bMinRebarColumn;
	m_bBrce = rData.bMinRebarBrace;
	m_bWall = rData.bMinRebarWall;
	m_bSlab = rData.bMinRebarSlab;
	m_bMat = rData.bMinRebarMat;

	m_bShell = rData.bMinRebarShell;
	m_edtShellHorz.SetEditUnit(rData.dRhominHorzShell == 0.0 ? dShellDir2 : rData.dRhominHorzShell);
	m_edtShellVert.SetEditUnit(rData.dRhominVertShell == 0.0 ? dShellDir1 : rData.dRhominVertShell);

	UpdateData(FALSE);

	InitDlgCtrls();
}

//(2009.07.10) Add by Unsang :: SPEC변경에 따라 해당 입력창은 기존 방식으로 적용한다.
/*
void CDgnConBarRatioDlg::OnDgnRbarRatioUpdateBtn()
{
	Set_DefaultByCode(m_strCode);
	UpdateData(FALSE);
}
*/

void CDgnConBarRatioDlg::SetDatabyNationalAnnex(int iStatus, int nNationalAnnex, double& dMax, double& dMin, double& dShellDir1, double& dShellDir2)
{
	//     if ( iStatus != 2 )  return; // Only EC2-2:05
	//     if ( nNationalAnnex < 0 || nNationalAnnex > 2 )  return; // 0:Recommended 1:British 2:Italy

	switch (nNationalAnnex)
	{
	case Recommended:
	{
		dMax = 0.040;  // Maximum Rebar Ratio by Column
		dMin = 0.002;  // Minimum Rebar Ratio by Column
		dShellDir1 = 0.002;
		dShellDir2 = 0.001;
	}
	break;
	case UnitedKingdom:
	{
		dMax = 0.040;  // Maximum Rebar Ratio by Column
		dMin = 0.002;  // Minimum Rebar Ratio by Column
		dShellDir1 = 0.002;
		dShellDir2 = 0.001;
	}
	break;
	case Italy:
		{
			dMax = 0.040;  // Maximum Rebar Ratio by Column
			dMin = 0.003;  // Minimum Rebar Ratio by Column
			dShellDir1 = 0.004;
			dShellDir2 = 0.001;
		}
		break;
	case Austria:
	case Belgium:
	case Cyprus:
	case CzechRepublic:
	case Denmark:
	case Netherlands:
	case Finland:
	case France:
	case Germany:
	case Greece:
	case Ireland:
	case Luxembourg:
	case Norway:
	case Poland:
	case Romania:
	case Singapore:
	case Slovakia:
	case Slovenia:
	case Spain:
	case Sweden:
	case Sweden19:
	case Malaysia:
	{
		dMax = 0.040;  // Maximum Rebar Ratio by Column
		dMin = 0.003;  // Minimum Rebar Ratio by Column
		dShellDir1 = 0.002;
		dShellDir2 = 0.001;
	}
	break;
	default:
		ASSERT(0);
		break;
	}
}

void CDgnConBarRatioDlg::Update_InitDataByCode(CString strCodeName)
{
	/*
	CString strTitle=_T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleCon(ID_DGN_CON_DCON_RMAX, strTitle, bShow, m_pDoc->IsPostMode());
	//if(strCodeName!=_T("Eurocode2-2:05")) m_pDoc->GetDesignTitleCon(ID_DGN_CON_DCON_RMAX, strTitle, bShow, m_pDoc->IsPostMode());
	//else                              m_pDoc->GetDesignTitleCon(ID_DGN_CON_DCON_RMAX, strTitle, bShow, m_pDoc->IsPostMode());
	SetWindowText(strTitle);

	if(bShow)
	{
	*/
	GetDlgItem(IDC_DGN_STATIC)->EnableWindow(TRUE);
	GetDlgItem(IDC_DGN_STATIC_RCCODE)->EnableWindow(TRUE);
	GetDlgItem(IDC_DGN_STATIC_RCCODE2)->EnableWindow(TRUE);
	GetDlgItem(IDC_DGN_RCCODE_BTN)->EnableWindow(TRUE);
	// Except EC2-2:05
	GetDlgItem(IDC_DGN_CON_RBAR_RATIO_FRAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_DGN_RCCTRL_RHOW_STC)->EnableWindow(TRUE);	
	GetDlgItem(IDC_DGN_RCCTRL_RHOW)->EnableWindow(TRUE);
	GetDlgItem(IDC_DGN_RCCTRL_RHOC_STR)->EnableWindow(TRUE);	
	GetDlgItem(IDC_DGN_RCCTRL_RHOC)->EnableWindow(TRUE);
	GetDlgItem(IDC_DGN_RCCTRL_RHOR_STR)->EnableWindow(TRUE);
	GetDlgItem(IDC_DGN_RCCTRL_RHOR)->EnableWindow(TRUE);
	//(2009.07.10) Add by Unsang :: SPEC변경에 따라 해당 입력창은 기존 방식으로 적용한다.
	/*
	// Only EC2-2:05
	GetDlgItem(IDC_DGN_CON_RBAR_LIMIT_FRAME1)->EnableWindow(TRUE);
	GetDlgItem(IDC_DGN_CON_RBAR_LIMIT_STR3)->EnableWindow(TRUE);	
	GetDlgItem(IDC_DGN_CON_RBAR_LIMIT_EDIT1)->EnableWindow(TRUE);
	GetDlgItem(IDC_DGN_CON_RBAR_LIMIT_FRAME2)->EnableWindow(TRUE);
	GetDlgItem(IDC_DGN_CON_RBAR_LIMIT_STR5)->EnableWindow(TRUE);	
	GetDlgItem(IDC_DGN_CON_RBAR_LIMIT_EDIT2)->EnableWindow(TRUE);
	*/

#if defined(_CIVIL)
	GetDlgItem(IDC_DGN_RCCTRL_RHOR)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_RCCTRL_RHOW)->EnableWindow(FALSE);
	if (m_strCode == _T("IS456:2000"))
		CDlgUtil::CtrlEnableDisable(this, IDC_DGN_RCCTRL_RHOR, TRUE);
	if (m_strCode == _T("IRS"))
		GetDlgItem(IDC_DGN_RCCTRL_RHOR)->EnableWindow(TRUE);
#endif

	if (m_strCode == _T("Eurocode2:04") || m_strCode == _T("Eurocode2-2:05"))
	{
		GetDlgItem(IDC_DGN_STATIC_RCCODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_RCCODE2)->ShowWindow(SW_SHOW);
		//GetDlgItem(IDC_DGN_RCCODE_BTN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RCCODE_BTN)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_DGN_STATIC_RCCODE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_RCCODE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RCCODE_BTN)->ShowWindow(SW_HIDE);
	}
	/*
	}
	else
	{
		GetDlgItem(IDC_DGN_STATIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STATIC_RCCODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STATIC_RCCODE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_RCCODE_BTN)->EnableWindow(FALSE);
		// Except EC2-2:05
		GetDlgItem(IDC_DGN_CON_RBAR_RATIO_FRAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_RCCTRL_RHOW_STC)->EnableWindow(FALSE);		
		GetDlgItem(IDC_DGN_RCCTRL_RHOW)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_RCCTRL_RHOC_STR)->EnableWindow(FALSE);		
		GetDlgItem(IDC_DGN_RCCTRL_RHOC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_RCCTRL_RHOR_STR)->EnableWindow(FALSE);		
		GetDlgItem(IDC_DGN_RCCTRL_RHOR)->EnableWindow(FALSE);
		//(2009.07.10) Add by Unsang :: SPEC변경에 따라 해당 입력창은 기존 방식으로 적용한다.

		// Only EC2-2:05
		//GetDlgItem(IDC_DGN_CON_RBAR_LIMIT_FRAME1)->EnableWindow(FALSE);
		//GetDlgItem(IDC_DGN_CON_RBAR_LIMIT_STR3)->EnableWindow(FALSE);		
		//GetDlgItem(IDC_DGN_CON_RBAR_LIMIT_EDIT1)->EnableWindow(FALSE);
		//GetDlgItem(IDC_DGN_CON_RBAR_LIMIT_FRAME2)->EnableWindow(FALSE);
		//GetDlgItem(IDC_DGN_CON_RBAR_LIMIT_STR5)->EnableWindow(FALSE);		
		//GetDlgItem(IDC_DGN_CON_RBAR_LIMIT_EDIT2)->EnableWindow(FALSE);
	}
	*/
#if defined(_CIVIL)
	GetDlgItem(IDC_DGN_RCCTRL_RHOW_STC)->ShowWindow(SW_HIDE);	
	GetDlgItem(IDC_DGN_RCCTRL_RHOW)->ShowWindow(SW_HIDE);
#else
	GetDlgItem(IDC_DGN_RCCTRL_RHOW_STC)->ShowWindow(SW_SHOW);	
	GetDlgItem(IDC_DGN_RCCTRL_RHOW)->ShowWindow(SW_SHOW);
#endif
}

int CDgnConBarRatioDlg::GetDataStatus(CString strCode)
{
	BOOL bUS_RUS = FALSE;

#if defined(_MGEN)
	if (CProduct::IsMovingType(D_PRODUCT_MOVING_US) || CProduct::IsMovingType(D_PRODUCT_MOVING_RUS))
		bUS_RUS = TRUE;
#endif

	BOOL bCivil = FALSE;
#if defined(_CIVIL)
	bCivil = TRUE;
#endif

	int iStatus = 0;
	if (bUS_RUS && strCode == _T("Eurocode2:04") /*&& m_strNationalAnnex == _LS(IDS_DGN_ITALY)*/) iStatus = 4; // Only Eurocode2:04, Italy
	else if (bCivil && strCode == _T("Eurocode2-2:05") /*&& m_strNationalAnnex == _LS(IDS_DGN_ITALY)*/)  iStatus = 6;
	else if (strCode == _T("Eurocode2-2:05"))  iStatus = 2;  // Only EC2-2:05!!
	else if (strCode == _T("IRC:112-2011") || strCode == _T("IRS") || strCode == _T("IRC:112-2020"))		iStatus = 5;
	else if (strCode == CONCODE_SP_63_13330_2018) iStatus = 4;
	else                                iStatus = 1;

	return iStatus;
}

void CDgnConBarRatioDlg::InitDlgCtrls()
{
	// TODO: Add your control notification handler code here
	// Change by ZINU.('02.7.8). For Dynamic Size.
	UpdateData(TRUE);
	GetDlgItem(IDC_DGN_REFPOS)->ShowWindow(SW_HIDE);

	int i = 0;
	for (i = 0; i < iDgn_RbarRatioID1; i++)	GetDlgItem(m_iRBARRATIOID1[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_RbarRatioID2; i++)	GetDlgItem(m_iRBARRATIOID2[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_RbarRatioID3; i++)	GetDlgItem(m_iRBARRATIOID3[i])->ShowWindow(SW_HIDE);
	for (i = 0; i < iDgn_RbarRatioID4; i++)	GetDlgItem(m_iRBARRATIOID4[i])->ShowWindow(SW_HIDE);

	CRect RectDlg, RectOk, RectCn, RectOrg;
	GetWindowRect(&RectDlg);
	GetDlgItem(IDOK)->GetWindowRect(&RectOk);
	GetDlgItem(IDCANCEL)->GetWindowRect(&RectCn);

	int iStatus = GetDataStatus(m_strCode);

	CRect RectRef;
	GetDlgItem(IDC_DGN_REFPOS)->GetWindowRect(&RectRef);
	switch (iStatus)
	{
	case 1:
		RectOrg = MoveCtrl(m_iRBARRATIOID1, iDgn_RbarRatioID1, RectRef);  //  이 함수에서 ctrl을 Show합니다.. ㅡ_ㅡ;;;;;
		GetDlgItem(IDC_DGN_CON_RBAR_RATIO_FRAME)->SetWindowText(_LS(IDS_DGN_REBAR_RATIO));
		break;

	case 2:
		RectOrg = MoveCtrl(m_iRBARRATIOID1, iDgn_RbarRatioID1, RectRef);  //  이 함수에서 ctrl을 Show합니다.. ㅡ_ㅡ;;;;;
		RectRef.top = RectOrg.bottom + (RectOrg.left - RectDlg.left);
		RectRef.bottom = RectRef.top + (RectOrg.bottom - RectOrg.top);
		RectRef.left = RectOrg.left;
		RectRef.right = RectOrg.right;
		RectOrg = MoveCtrl(m_iRBARRATIOID4, iDgn_RbarRatioID4, RectRef);  //  이 함수에서 ctrl을 Show합니다.. ㅡ_ㅡ;;;;;
		GetDlgItem(IDC_DGN_CON_RBAR_RATIO_FRAME)->SetWindowText(_LS(IDS_DGN_MAX_REBAR_RATIO));

		for (i = 2; i < iDgn_RbarRatioID4; i++)	GetDlgItem(m_iRBARRATIOID4[i])->ShowWindow(SW_HIDE); //EnableWindow(FALSE);

		break;
	case 4:
		RectOrg = MoveCtrl(m_iRBARRATIOID1, iDgn_RbarRatioID1, RectRef);  //  이 함수에서 ctrl을 Show합니다.. ㅡ_ㅡ;;;;;
		RectRef.top = RectOrg.bottom + (RectOrg.left - RectDlg.left);
		RectRef.bottom = RectRef.top + (RectOrg.bottom - RectOrg.top);
		RectRef.left = RectOrg.left;
		RectRef.right = RectOrg.right;
		RectOrg = MoveCtrl(m_iRBARRATIOID4, iDgn_RbarRatioID4, RectRef);  //  이 함수에서 ctrl을 Show합니다.. ㅡ_ㅡ;;;;;
		GetDlgItem(IDC_DGN_CON_RBAR_RATIO_FRAME)->SetWindowText(_LS(IDS_DGN_MAX_REBAR_RATIO));
		break;
	case 5://IRC112
		RectOrg = MoveCtrl(m_iRBARRATIOID1, iDgn_RbarRatioID1, RectRef);  //  이 함수에서 ctrl을 Show합니다.. ㅡ_ㅡ;;;;;
		RectRef.top = RectOrg.bottom + (RectOrg.left - RectDlg.left);
		RectRef.bottom = RectRef.top + (RectOrg.bottom - RectOrg.top);
		RectRef.left = RectOrg.left;
		RectRef.right = RectOrg.right;
		RectOrg = MoveCtrl(m_iRBARRATIOID4, iDgn_RbarRatioID4, RectRef);  //  이 함수에서 ctrl을 Show합니다.. ㅡ_ㅡ;;;;;
		GetDlgItem(IDC_DGN_CON_RBAR_RATIO_FRAME)->SetWindowText(_LS(IDS_DGN_MAX_REBAR_RATIO));
		GetDlgItem(IDC_DGN_RCCTRL_RHOR_STR)->SetWindowText(_T("Beam Design(Rhor)"));
		//SetDlgItemText(IDC_DGN_RCCTRL_RHOR_STR,_LS(IDS_DGN_BEAM_DGN_RHOR_IRC));
		for (i = 2; i < iDgn_RbarRatioID4; i++)	GetDlgItem(m_iRBARRATIOID4[i])->ShowWindow(SW_HIDE); //EnableWindow(FALSE);
		break;
	case 6:
		RectOrg = MoveCtrl(m_iRBARRATIOID1, iDgn_RbarRatioID1, RectRef);  //  이 함수에서 ctrl을 Show합니다.. ㅡ_ㅡ;;;;;
		RectRef.top = RectOrg.bottom + (RectOrg.left - RectDlg.left);
		RectRef.bottom = RectRef.top + (RectOrg.bottom - RectOrg.top);
		RectRef.left = RectOrg.left;
		RectRef.right = RectOrg.right;
		RectOrg = MoveCtrl(m_iRBARRATIOID4, iDgn_RbarRatioID4, RectRef);  //  이 함수에서 ctrl을 Show합니다.. ㅡ_ㅡ;;;;;
		GetDlgItem(IDC_DGN_CON_RBAR_RATIO_FRAME)->SetWindowText(_LS(IDS_DGN_MAX_REBAR_RATIO));

		// MoveCtrl() 에서 Show 했던 것 중 일부를 다시 Hide함..
		GetDlgItem(IDC_DGN_CON_RBAR_LIMIT_FRAME3_BEAM_CHK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CON_RBAR_LIMIT_FRAME3_COLM_CHK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CON_RBAR_LIMIT_FRAME3_BRCE_CHK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CON_RBAR_LIMIT_FRAME3_WALL_CHK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CON_RBAR_LIMIT_FRAME3_SLAB_CHK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CON_RBAR_LIMIT_FRAME3_MAT_CHK)->ShowWindow(SW_HIDE);

		// Ctrl 위치는 ResizeDialog() 에서 다시 맞춥니다.

		break;
	default:
		ASSERT(0);
		break;
		//(2009.07.10) Add by Unsang :: SPEC변경에 따라 해당 입력창은 기존 방식으로 적용한다.
		/*
		case 2:
			{
				RectOrg = MoveCtrl(2,m_iRBARRATIOID2,RectRef);
				RectRef.top    = RectOrg.bottom + (RectOrg.left-RectDlg.left);
				RectRef.bottom = RectRef.top + (RectOrg.bottom-RectOrg.top);
				RectRef.left   = RectOrg.left;
				RectRef.right  = RectOrg.right;
				RectOrg = MoveCtrl(3,m_iRBARRATIOID3,RectRef);
			}
			break;
		*/
	}

	if (iStatus == 4 || iStatus == 6 )
	{
		// 대화창 이름 : Limiting Rebar Ratio
		this->SetWindowText(_LS(IDS_DGN_LIMIT_REBAR_RATIO));
	}
	else
	{
		// 대화창 이름 : Limiting Maximum Rebar Ratio
		this->SetWindowText(_LS(IDS_DGN_LIMIT_MAX_REBAR_RATIO));
	}

	CRect rDlg;
	rDlg.left = RectDlg.left;
	rDlg.top = RectDlg.top;
	rDlg.right = RectDlg.right;
	rDlg.bottom = RectOrg.bottom +(int)(globalUtils.ScaleByDPI(2.5) * RectOk.Height());

	CRect rOk;
	rOk.left = RectOk.left;
	rOk.right = RectOk.right;
	rOk.top = rDlg.bottom - RectOk.Height() - globalUtils.ScaleByDPI(8);
	rOk.bottom = rOk.top + RectOk.Height();

	CRect rCn;
	rCn.left = RectCn.left;
	rCn.right = RectCn.right;
	rCn.top = rOk.top;
	rCn.bottom = rOk.bottom;

	ScreenToClient(rDlg);
	ScreenToClient(rOk);
	ScreenToClient(rCn);
	SetWindowPos(NULL, rDlg.left, rDlg.top, rDlg.Width(), rDlg.Height(), SWP_NOMOVE | SWP_NOZORDER);
	GetDlgItem(IDOK)->SetWindowPos(NULL, rOk.left, rOk.top, rOk.Width(), rOk.Height(), SWP_NOSIZE);
	GetDlgItem(IDCANCEL)->SetWindowPos(NULL, rCn.left, rCn.top, rCn.Width(), rCn.Height(), SWP_NOSIZE);

}

CRect CDgnConBarRatioDlg::MoveCtrl(int* pID, int iArSize, CRect Rect0)
{
	CRect RectOrg;

	CRect* pRectCng = new CRect[iArSize];

	//CRect Rect0, Rect1;
	//GetDlgItem(m_iFPHIID[0])->GetWindowRect(&Rect0);
	CRect Rect1;
	for (int i = 0; i < iArSize; i++)
	{
		GetDlgItem(pID[i])->ShowWindow(SW_SHOW);
		GetDlgItem(pID[i])->GetWindowRect(&(pRectCng[i]));
		if (i == 0)	Rect1 = pRectCng[i];
		pRectCng[i].top -= Rect1.top - Rect0.top;
		pRectCng[i].bottom -= Rect1.top - Rect0.top;
	}
	RectOrg = pRectCng[0];
	for (int i = 0; i < iArSize; i++)
	{
		ScreenToClient(pRectCng[i]);
		GetDlgItem(pID[i])->SetWindowPos(NULL, pRectCng[i].left, pRectCng[i].top, pRectCng[i].Width(), pRectCng[i].Height(), SWP_NOSIZE | SWP_NOZORDER);
	}
	delete[] pRectCng;

	return RectOrg;
}

void CDgnConBarRatioDlg::OnCancel()
{
	CDialogMove::OnCancel();
}

void CDgnConBarRatioDlg::OnOK()
{
	UpdateData(TRUE);

	T_DCON_D rData;
	rData.Initialize();
	if (m_pDoc->m_pAttrCtrl->ExistDcon())	m_pDoc->m_pAttrCtrl->GetDcon(rData);
	else
	{
		rData.DesignCode = m_strCode;
		rData.nNationalAnnex = m_nNationalAnnex;
	}

	rData.dRhor = m_Rhor;
	rData.dRhoc = m_Rhoc;
	rData.dRhow = m_Rhow;

	rData.bMinRebarBeam = m_bBeam;
	rData.bMinRebarColumn = m_bColm;
	rData.bMinRebarBrace = m_bBrce;
	rData.bMinRebarWall = m_bWall;
	rData.bMinRebarSlab = m_bSlab;
	rData.bMinRebarMat = m_bMat;

	rData.bMinRebarShell = m_bShell;
	rData.dRhominHorzShell = m_edtShellHorz.GetEditValue();
	rData.dRhominVertShell = m_edtShellVert.GetEditValue();

	// Save RC Control Data at DB.
	BOOL bCheck = m_pDoc->m_pDataCtrl->AddDcon(rData);
	if (bCheck)  CDialogMove::OnOK();
}

// Error 처리를 하지 않음. 반영되지 않음.
BOOL CDgnConBarRatioDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;
	BOOL bCheck_min = TRUE;

	T_DCON_D rData;
	rData.Initialize();
	if (m_pDoc->m_pAttrCtrl->ExistDcon())	m_pDoc->m_pAttrCtrl->GetDcon(rData);
	else																	rData.DesignCode = m_strCode;

	if (m_Rhor < 0.02 || m_Rhor > 0.08) bCheck = FALSE;
	if (m_Rhoc < 0.02 || m_Rhoc > 0.08) bCheck = FALSE;
	if (m_Rhow < 0.02 || m_Rhow > 0.08) bCheck = FALSE;

	return bCheck;
}

void CDgnConBarRatioDlg::ResizeDialog()
{
#if defined(_CIVIL)
	CRect rRef;
	CRect rToMove;
	int nDistY = 0;
	CArray<UINT, UINT> aControls;

	int iStatus = GetDataStatus(m_strCode);

	aControls.RemoveAll();
	aControls.Add(IDC_DGN_RCCTRL_RHOC_STR);	
	aControls.Add(IDC_DGN_RCCTRL_RHOC);
	aControls.Add(IDC_DGN_RCCTRL_RHOR_STR);	
	aControls.Add(IDC_DGN_RCCTRL_RHOR);

	// Column Design
	GetDlgItem(IDC_DGN_RCCTRL_RHOW_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_RCCTRL_RHOC_STR)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// Group Box 
	GetDlgItem(IDC_DGN_RCCTRL_RHOR_STR)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_CON_RBAR_RATIO_FRAME)->GetWindowRect(rToMove);
	rToMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
	ScreenToClient(rToMove);
	GetDlgItem(IDC_DGN_CON_RBAR_RATIO_FRAME)->MoveWindow(rToMove);

	if (iStatus == 6)
	{
		aControls.RemoveAll();
		aControls.Add(IDC_DGN_CON_RBAR_LIMIT_FRAME3);
		GetDlgItem(IDC_DGN_CON_RBAR_RATIO_FRAME)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_CON_RBAR_LIMIT_FRAME3)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(6);
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

		aControls.RemoveAll();
		aControls.Add(IDC_DGN_CON_RBAR_LIMIT_FRAME3_SHELL_CHK);
		aControls.Add(IDC_DGN_CON_RBAR_HOR_STR);		
		aControls.Add(IDC_DGN_CON_RBAR_HOR_EDIT);
		aControls.Add(IDC_DGN_CON_RBAR_VERT_STR);		
		aControls.Add(IDC_DGN_CON_RBAR_VERT_EDIT);
		aControls.Add(IDC_DGN_SHELL_GRP);
		GetDlgItem(IDC_DGN_CON_RBAR_LIMIT_FRAME3)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_CON_RBAR_LIMIT_FRAME3_SHELL_CHK)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top + globalUtils.ScaleByDPI(20);
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

		GetDlgItem(IDC_DGN_SHELL_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_CON_RBAR_LIMIT_FRAME3)->GetWindowRect(rToMove);
		rToMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
		ScreenToClient(rToMove);
		GetDlgItem(IDC_DGN_CON_RBAR_LIMIT_FRAME3)->MoveWindow(rToMove);
	}

	// OK, Cancel
	aControls.RemoveAll();
	aControls.Add(IDOK);
	aControls.Add(IDCANCEL);
	if (iStatus == 6)
	{
		GetDlgItem(IDC_DGN_CON_RBAR_LIMIT_FRAME3)->GetWindowRect(rRef);
	}
	else
	{
		GetDlgItem(IDC_DGN_CON_RBAR_RATIO_FRAME)->GetWindowRect(rRef);
	}
	GetDlgItem(IDOK)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16); /// 10 = OFFSET DIST, 18 = BUTTON HEIGHT.
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDOK);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(r);

#endif
}

void CDgnConBarRatioDlg::EnableCtrlDynamic()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_CON_RBAR_HOR_STR)->EnableWindow(m_bShell);	
	GetDlgItem(IDC_DGN_CON_RBAR_HOR_EDIT)->EnableWindow(m_bShell);
	GetDlgItem(IDC_DGN_CON_RBAR_VERT_STR)->EnableWindow(m_bShell);	
	GetDlgItem(IDC_DGN_CON_RBAR_VERT_EDIT)->EnableWindow(m_bShell);
}

void CDgnConBarRatioDlg::OnChkShell()
{
	EnableCtrlDynamic();
}
