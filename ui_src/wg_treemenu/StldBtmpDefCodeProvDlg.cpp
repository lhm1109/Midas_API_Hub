//////////
// CStldBtmpDefCodeProvDlg.cpp : implementation file
// 
#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldBtmpDefCodeProvDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\IUsageCounter.h"

#include "StldBtmpDefCodeProvUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCStldBtmpDefCodeProvDlgdialog
CStldBtmpDefCodeProvDlg::CStldBtmpDefCodeProvDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CStldBtmpDefCodeProvDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();

	m_aControlsOrg.RemoveAll();
	m_aControlsAASHTO.RemoveAll();
	m_aControlsEuro.RemoveAll();
	m_aControlsEuro_Proc.RemoveAll();
	m_aControlsEuro_T1.RemoveAll();
	m_aControlsEuro_T2.RemoveAll();
	m_aControlsEuro_T3.RemoveAll();
	m_aControlsEuro_T4.RemoveAll();
	m_aControlsEuro_H0.RemoveAll();
	m_aControlsEuro_H1.RemoveAll();
	m_aControlsEuro_H2.RemoveAll();
	m_aControlsEuro_H3.RemoveAll();
	m_aControlsEuro_H4.RemoveAll();
	m_aControlsAS.RemoveAll();
	m_aControlsAS_T1.RemoveAll();
	m_aControlsAS_T2.RemoveAll();
	m_aControlsAS_Small_T.RemoveAll();
	m_aControlsAS_H1.RemoveAll();
	m_aControlsAS_H2.RemoveAll();
}

CStldBtmpDefCodeProvDlg::~CStldBtmpDefCodeProvDlg()
{
}

void CStldBtmpDefCodeProvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldBtmpDefCodeProvDlg)	
	DDX_Control(pDX, IDC_TMAX_EDT, m_edtSnipTMax);
	DDX_Control(pDX, IDC_DEF_CODE_CBX, m_cbxDefCode);
	DDX_Control(pDX, IDC_SECT_TYPE_CBX, m_cbxSectType);
	DDX_Control(pDX, IDC_JTG_T1_EDT, m_edtJTGT1);
	//DDX_Control(pDX, IDC_JTG_T1_UNT, m_untJTGT1);
	DDX_Control(pDX, IDC_JTG_T2_EDT, m_edtJTGT2);
	//DDX_Control(pDX, IDC_JTG_T2_UNT, m_untJTGT2);
	DDX_Control(pDX, IDC_JTG_SMALL_T_EDT, m_edtJTGSmallT);
	DDX_Control(pDX, IDC_JTG_DUMB_T1_EDT, m_edtJTGTDumb1);
	DDX_Control(pDX, IDC_JTG_DUMB_T2_EDT, m_edtJTGTDumb2);
	// AASHTO
	DDX_Control(pDX, IDC_AASHTO_T1_EDT, m_edtAASHTOT1);
	DDX_Control(pDX, IDC_AASHTO_T2_EDT, m_edtAASHTOT2);
	DDX_Control(pDX, IDC_AASHTO_T3_EDT, m_edtAASHTOT3);
	DDX_Control(pDX, IDC_AASHTO_A_EDT,  m_edtAASHTOTA);
	DDX_Control(pDX, IDC_AASHTO_SMALL_T_EDT, m_edtAASHTOTSmallT);
	//Euro
	DDX_Control(pDX, IDC_EURO_T1_EDT,       m_edtEuroT1);
	DDX_Control(pDX, IDC_EURO_T2_EDT,       m_edtEuroT2);
	DDX_Control(pDX, IDC_EURO_T3_EDT,       m_edtEuroT3);
	DDX_Control(pDX, IDC_EURO_T4_EDT,       m_edtEuroT4);

	DDX_Control(pDX, IDC_EURO_SMALL_H_EDT,  m_edtEuroH0);
	DDX_Control(pDX, IDC_EURO_SMALL_H1_EDT, m_edtEuroH1);
	DDX_Control(pDX, IDC_EURO_SMALL_H2_EDT, m_edtEuroH2);
	DDX_Control(pDX, IDC_EURO_SMALL_H3_EDT, m_edtEuroH3);
	DDX_Control(pDX, IDC_EURO_SMALL_H4_EDT, m_edtEuroH4);
	// AS
	DDX_Control(pDX, IDC_AS_SMALL_T_EDT,  m_edtAsSmallT);
	DDX_Control(pDX, IDC_AS_T1_EDT,       m_edtAsT1);
	DDX_Control(pDX, IDC_AS_T2_EDT,       m_edtAsT2);
	DDX_Control(pDX, IDC_AS_SMALL_H1_EDT, m_edtAsH1);
	DDX_Control(pDX, IDC_AS_SMALL_H2_EDT, m_edtAsH2);
	DDX_Control(pDX, IDC_SW_BITMAP,       m_mvPicture);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStldBtmpDefCodeProvDlg, CDialogMove)
	//{{AFX_MSG_MAP(CStldBtmpDefCodeProvDlg)
	ON_CBN_SELCHANGE(IDC_DEF_CODE_CBX, OnSelchangeTypeCbx)
	ON_CBN_SELCHANGE(IDC_SECT_TYPE_CBX, OnSelchangeModelTypCbx)

	ON_BN_CLICKED(IDC_TEMP_GRAD_TYPE1_RDO,     OnClickGradRdo)
	ON_BN_CLICKED(IDC_TEMP_GRAD_TYPE2_RDO,     OnClickGradRdo)
	ON_BN_CLICKED(IDC_JTG_DUMB_TYPE_STUB_RDO,  OnClickDumbRdo)
	ON_BN_CLICKED(IDC_JTG_DUMB_TYPE_DUMB_RDO,  OnClickDumbRdo)
	ON_BN_CLICKED(IDC_AASHTO_SMALL_T_AUTO_RDO, OnClickAASHTO_SmalT_Rdo)
	ON_BN_CLICKED(IDC_AASHTO_SMALL_T_USER_RDO, OnClickAASHTO_SmalT_Rdo)
	ON_BN_CLICKED(IDC_AASHTO_A_AUTO_RDO,       OnClickAASHTO_A_Rdo)
	ON_BN_CLICKED(IDC_AASHTO_A_USER_RDO,       OnClickAASHTO_A_Rdo)

	ON_BN_CLICKED(IDC_EURO_PROCEDURE_NORM_RDO, OnClickEuro_Procedure_Rdo)
	ON_BN_CLICKED(IDC_EURO_PROCEDURE_SIMP_RDO, OnClickEuro_Procedure_Rdo)
	ON_BN_CLICKED(IDC_EURO_SMALL_H_AUTO_RDO,   OnClickEuro_H0_Rdo)
	ON_BN_CLICKED(IDC_EURO_SMALL_H_USER_RDO,   OnClickEuro_H0_Rdo)
	ON_BN_CLICKED(IDC_EURO_SMALL_H1_AUTO_RDO,  OnClickEuro_H1_Rdo)
	ON_BN_CLICKED(IDC_EURO_SMALL_H1_USER_RDO,  OnClickEuro_H1_Rdo)
	ON_BN_CLICKED(IDC_EURO_SMALL_H2_AUTO_RDO,  OnClickEuro_H2_Rdo)
	ON_BN_CLICKED(IDC_EURO_SMALL_H2_USER_RDO,  OnClickEuro_H2_Rdo)
	ON_BN_CLICKED(IDC_EURO_SMALL_H3_AUTO_RDO,  OnClickEuro_H3_Rdo)
	ON_BN_CLICKED(IDC_EURO_SMALL_H3_USER_RDO,  OnClickEuro_H3_Rdo)
	ON_BN_CLICKED(IDC_EURO_SMALL_H4_AUTO_RDO,  OnClickEuro_H4_Rdo)
	ON_BN_CLICKED(IDC_EURO_SMALL_H4_USER_RDO,  OnClickEuro_H4_Rdo)

	ON_BN_CLICKED(IDC_AS_SMALL_T_AUTO_RDO,   OnClickAS_SmalT_Rdo)
	ON_BN_CLICKED(IDC_AS_SMALL_T_USER_RDO,   OnClickAS_SmalT_Rdo)
// 	ON_BN_CLICKED(IDC_AS_SMALL_H1_AUTO_RDO,  OnClickAS_H1_Rdo)
// 	ON_BN_CLICKED(IDC_AS_SMALL_H1_USER_RDO,  OnClickAS_H1_Rdo)
// 	ON_BN_CLICKED(IDC_AS_SMALL_H2_AUTO_RDO,  OnClickAS_H2_Rdo)
// 	ON_BN_CLICKED(IDC_AS_SMALL_H2_USER_RDO,  OnClickAS_H2_Rdo)

	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CStldBtmpDefCodeProvDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	SetControl();
	
	 // 기본값을 세팅하기 위한 코드별 Data2Dlg를 부름
	Data2DlgSnip();
	Data2DlgJTG();
	Data2Dlg();
	ChangeBitmap();

	ShowHideCtrl();
	AlignCtrl();
	ResizeDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CStldBtmpDefCodeProvDlg::SetControl()
{
	//m_dAlwTendonStress.SetUnitType(D_UNITSYS_BASE_STRESS);

	m_edtSnipTMax.SetUnitType(D_UNITSYS_NONE);
	
	m_aRdoSnipTempGrad.RemoveAll();
	m_aRdoSnipTempGrad.Add(IDC_TEMP_GRAD_TYPE1_RDO);
	m_aRdoSnipTempGrad.Add(IDC_TEMP_GRAD_TYPE2_RDO);

	// CH Dumb
	m_aRdoDumbType.RemoveAll();
	m_aRdoDumbType.Add(IDC_JTG_DUMB_TYPE_STUB_RDO);
	m_aRdoDumbType.Add(IDC_JTG_DUMB_TYPE_DUMB_RDO);

	// AASHTO
	m_aRdoAASHTOSmallT.RemoveAll();
	m_aRdoAASHTOSmallT.Add(IDC_AASHTO_SMALL_T_AUTO_RDO);
	m_aRdoAASHTOSmallT.Add(IDC_AASHTO_SMALL_T_USER_RDO);

	m_aRdoAASHTO_A.RemoveAll();
	m_aRdoAASHTO_A.Add(IDC_AASHTO_A_AUTO_RDO);
	m_aRdoAASHTO_A.Add(IDC_AASHTO_A_USER_RDO);

	// EURO
	m_aRdoEuroPro.RemoveAll();
	m_aRdoEuroPro.Add(IDC_EURO_PROCEDURE_NORM_RDO);
	m_aRdoEuroPro.Add(IDC_EURO_PROCEDURE_SIMP_RDO);

	m_aRdoEuroH0.RemoveAll();
	m_aRdoEuroH0.Add(IDC_EURO_SMALL_H_AUTO_RDO);
	m_aRdoEuroH0.Add(IDC_EURO_SMALL_H_USER_RDO);

	m_aRdoEuroH1.RemoveAll();
	m_aRdoEuroH1.Add(IDC_EURO_SMALL_H1_AUTO_RDO);
	m_aRdoEuroH1.Add(IDC_EURO_SMALL_H1_USER_RDO);

	m_aRdoEuroH2.RemoveAll();
	m_aRdoEuroH2.Add(IDC_EURO_SMALL_H2_AUTO_RDO);
	m_aRdoEuroH2.Add(IDC_EURO_SMALL_H2_USER_RDO);

	m_aRdoEuroH3.RemoveAll();
	m_aRdoEuroH3.Add(IDC_EURO_SMALL_H3_AUTO_RDO);
	m_aRdoEuroH3.Add(IDC_EURO_SMALL_H3_USER_RDO);

	m_aRdoEuroH4.RemoveAll();
	m_aRdoEuroH4.Add(IDC_EURO_SMALL_H4_AUTO_RDO);
	m_aRdoEuroH4.Add(IDC_EURO_SMALL_H4_USER_RDO);

	// AS
	m_aRdoAsSmallT.RemoveAll();
	m_aRdoAsSmallT.Add(IDC_AS_SMALL_T_AUTO_RDO);
	m_aRdoAsSmallT.Add(IDC_AS_SMALL_T_USER_RDO);
	m_aRdoAsH1.RemoveAll();
	m_aRdoAsH1.Add(IDC_AS_SMALL_H1_AUTO_RDO);
	m_aRdoAsH1.Add(IDC_AS_SMALL_H1_USER_RDO);
	m_aRdoAsH2.RemoveAll();
	m_aRdoAsH2.Add(IDC_AS_SMALL_H2_AUTO_RDO);
	m_aRdoAsH2.Add(IDC_AS_SMALL_H2_USER_RDO);

	m_cbxDefCode.ResetContent();
	CDlgUtil::CobxAddItem(m_cbxDefCode, _LS(IDS_SNIP_2050384), D_BTMP_CODE_SNIP_2050384);
	CDlgUtil::CobxAddItem(m_cbxDefCode, _LS(IDS_SP_35_13330_2011), D_BTMP_CODE_SP_35_13330_2011);
	CDlgUtil::CobxAddItem(m_cbxDefCode, _LS(IDS_JTG_D60_2015), D_BTMP_CODE_JTG_D60_2015);
    CDlgUtil::CobxAddItem(m_cbxDefCode, _LS(IDS_IRC6_2017),D_BTMP_CODE_IRC6_2017);
	CDlgUtil::CobxAddItem(m_cbxDefCode, _LS(IDS_AASHTO_2020),D_BTMP_CODE_AASHTO_LRFD_2020);
	CDlgUtil::CobxAddItem(m_cbxDefCode, _LS(IDS_EUROCODE),D_BTMP_CODE_EUROCODE);
	CDlgUtil::CobxAddItem(m_cbxDefCode, _LS(IDS_AS_5100),D_BTMP_CODE_AS);
	m_cbxDefCode.SetCurSel(0);

	// Control
	m_aControlsOrg.RemoveAll();
	m_aControlsOrg.Add(IDC_TMAX_STC);
	m_aControlsOrg.Add(IDC_TMAX_EDT);
	m_aControlsOrg.Add(IDC_TMAX_UNT);
	m_aControlsOrg.Add(IDC_JTG_T1_STC     );
	m_aControlsOrg.Add(IDC_JTG_T1_EDT     );
	m_aControlsOrg.Add(IDC_JTG_T1_UNT     );
	m_aControlsOrg.Add(IDC_JTG_T2_STC     );
	m_aControlsOrg.Add(IDC_JTG_T2_EDT     );
	m_aControlsOrg.Add(IDC_JTG_T2_UNT     );
	m_aControlsOrg.Add(IDC_JTG_SMALL_T_STC);
	m_aControlsOrg.Add(IDC_JTG_SMALL_T_EDT);
	m_aControlsOrg.Add(IDC_JTG_SMALL_T_UNT);

	m_aControlsAASHTO.RemoveAll();
	m_aControlsAASHTO.Add(IDC_AASHTO_T1_STC);
	m_aControlsAASHTO.Add(IDC_AASHTO_T1_EDT);
	m_aControlsAASHTO.Add(IDC_AASHTO_T1_UNT);
	m_aControlsAASHTO.Add(IDC_AASHTO_T2_STC);
	m_aControlsAASHTO.Add(IDC_AASHTO_T2_EDT);
	m_aControlsAASHTO.Add(IDC_AASHTO_T2_UNT);
	m_aControlsAASHTO.Add(IDC_AASHTO_T3_STC);
	m_aControlsAASHTO.Add(IDC_AASHTO_T3_EDT);
	m_aControlsAASHTO.Add(IDC_AASHTO_T3_UNT);
	m_aControlsAASHTO.Add(IDC_AASHTO_SMALL_T_STC);
	m_aControlsAASHTO.Add(IDC_AASHTO_SMALL_T_EDT);
	m_aControlsAASHTO.Add(IDC_AASHTO_SMALL_T_UNT);
	m_aControlsAASHTO.Add(IDC_AASHTO_SMALL_T_AUTO_RDO);
	m_aControlsAASHTO.Add(IDC_AASHTO_SMALL_T_USER_RDO);
	m_aControlsAASHTO.Add(IDC_AASHTO_A_STC);
	m_aControlsAASHTO.Add(IDC_AASHTO_A_EDT);
	m_aControlsAASHTO.Add(IDC_AASHTO_A_UNT);
	m_aControlsAASHTO.Add(IDC_AASHTO_A_AUTO_RDO);
	m_aControlsAASHTO.Add(IDC_AASHTO_A_USER_RDO);

	m_aControlsEuro_Proc.RemoveAll();
	m_aControlsEuro_Proc.Add(IDC_EURO_PROCEDURE_GRP);
	m_aControlsEuro_Proc.Add(IDC_EURO_PROCEDURE_NORM_RDO);
	m_aControlsEuro_Proc.Add(IDC_EURO_PROCEDURE_SIMP_RDO);
	m_aControlsEuro_T1.RemoveAll();
	m_aControlsEuro_T1.Add(IDC_EURO_T1_STC);
	m_aControlsEuro_T1.Add(IDC_EURO_T1_EDT);
	m_aControlsEuro_T1.Add(IDC_EURO_T1_UNT);
	m_aControlsEuro_T2.RemoveAll();
	m_aControlsEuro_T2.Add(IDC_EURO_T2_STC);
	m_aControlsEuro_T2.Add(IDC_EURO_T2_EDT);
	m_aControlsEuro_T2.Add(IDC_EURO_T2_UNT);
	m_aControlsEuro_T3.RemoveAll();
	m_aControlsEuro_T3.Add(IDC_EURO_T3_STC);
	m_aControlsEuro_T3.Add(IDC_EURO_T3_EDT);
	m_aControlsEuro_T3.Add(IDC_EURO_T3_UNT);
	m_aControlsEuro_T4.RemoveAll();
	m_aControlsEuro_T4.Add(IDC_EURO_T4_STC);
	m_aControlsEuro_T4.Add(IDC_EURO_T4_EDT);
	m_aControlsEuro_T4.Add(IDC_EURO_T4_UNT);
	m_aControlsEuro_H0.RemoveAll();
	m_aControlsEuro_H0.Add(IDC_EURO_SMALL_H_STC);
	m_aControlsEuro_H0.Add(IDC_EURO_SMALL_H_EDT);
	m_aControlsEuro_H0.Add(IDC_EURO_SMALL_H_UNT);
	m_aControlsEuro_H0.Add(IDC_EURO_SMALL_H_AUTO_RDO);
	m_aControlsEuro_H0.Add(IDC_EURO_SMALL_H_USER_RDO);
	m_aControlsEuro_H1.RemoveAll();
	m_aControlsEuro_H1.Add(IDC_EURO_SMALL_H1_STC);
	m_aControlsEuro_H1.Add(IDC_EURO_SMALL_H1_EDT);
	m_aControlsEuro_H1.Add(IDC_EURO_SMALL_H1_UNT);
	m_aControlsEuro_H1.Add(IDC_EURO_SMALL_H1_AUTO_RDO);
	m_aControlsEuro_H1.Add(IDC_EURO_SMALL_H1_USER_RDO);
	m_aControlsEuro_H2.RemoveAll();
	m_aControlsEuro_H2.Add(IDC_EURO_SMALL_H2_STC);
	m_aControlsEuro_H2.Add(IDC_EURO_SMALL_H2_EDT);
	m_aControlsEuro_H2.Add(IDC_EURO_SMALL_H2_UNT);
	m_aControlsEuro_H2.Add(IDC_EURO_SMALL_H2_AUTO_RDO);
	m_aControlsEuro_H2.Add(IDC_EURO_SMALL_H2_USER_RDO);
	m_aControlsEuro_H3.RemoveAll();
	m_aControlsEuro_H3.Add(IDC_EURO_SMALL_H3_STC);
	m_aControlsEuro_H3.Add(IDC_EURO_SMALL_H3_EDT);
	m_aControlsEuro_H3.Add(IDC_EURO_SMALL_H3_UNT);
	m_aControlsEuro_H3.Add(IDC_EURO_SMALL_H3_AUTO_RDO);
	m_aControlsEuro_H3.Add(IDC_EURO_SMALL_H3_USER_RDO);
	m_aControlsEuro_H4.RemoveAll();
	m_aControlsEuro_H4.Add(IDC_EURO_SMALL_H4_STC);
	m_aControlsEuro_H4.Add(IDC_EURO_SMALL_H4_EDT);
	m_aControlsEuro_H4.Add(IDC_EURO_SMALL_H4_UNT);
	m_aControlsEuro_H4.Add(IDC_EURO_SMALL_H4_AUTO_RDO);
	m_aControlsEuro_H4.Add(IDC_EURO_SMALL_H4_USER_RDO);

	m_aControlsEuro.RemoveAll();
	m_aControlsEuro.Append(m_aControlsEuro_Proc);
	m_aControlsEuro.Append(m_aControlsEuro_T1);
	m_aControlsEuro.Append(m_aControlsEuro_T2);
	m_aControlsEuro.Append(m_aControlsEuro_T3);
	m_aControlsEuro.Append(m_aControlsEuro_T4);
	m_aControlsEuro.Append(m_aControlsEuro_H0);
	m_aControlsEuro.Append(m_aControlsEuro_H1);
	m_aControlsEuro.Append(m_aControlsEuro_H2);
	m_aControlsEuro.Append(m_aControlsEuro_H3);
	m_aControlsEuro.Append(m_aControlsEuro_H4);
	
	m_aControlsAS_T1.RemoveAll();
	m_aControlsAS_T1.Add(IDC_AS_T1_STC);
	m_aControlsAS_T1.Add(IDC_AS_T1_EDT);
	m_aControlsAS_T1.Add(IDC_AS_T1_UNT);
	m_aControlsAS_T2.RemoveAll();
	m_aControlsAS_T2.Add(IDC_AS_T2_STC);
	m_aControlsAS_T2.Add(IDC_AS_T2_EDT);
	m_aControlsAS_T2.Add(IDC_AS_T2_UNT);
	m_aControlsAS_Small_T.RemoveAll();
	m_aControlsAS_Small_T.Add(IDC_AS_SMALL_T_STC);
	m_aControlsAS_Small_T.Add(IDC_AS_SMALL_T_AUTO_RDO);
	m_aControlsAS_Small_T.Add(IDC_AS_SMALL_T_USER_RDO);
	m_aControlsAS_Small_T.Add(IDC_AS_SMALL_T_EDT);
	m_aControlsAS_Small_T.Add(IDC_AS_SMALL_T_UNT);
	m_aControlsAS_H1.RemoveAll();
	m_aControlsAS_H1.Add(IDC_AS_SMALL_H1_STC);
// 	m_aControlsAS_H1.Add(IDC_AS_SMALL_H1_AUTO_RDO);
// 	m_aControlsAS_H1.Add(IDC_AS_SMALL_H1_USER_RDO);
	m_aControlsAS_H1.Add(IDC_AS_SMALL_H1_EDT);
	m_aControlsAS_H1.Add(IDC_AS_SMALL_H1_UNT);
	m_aControlsAS_H2.RemoveAll();
	m_aControlsAS_H2.Add(IDC_AS_SMALL_H2_STC);
// 	m_aControlsAS_H2.Add(IDC_AS_SMALL_H2_AUTO_RDO);
// 	m_aControlsAS_H2.Add(IDC_AS_SMALL_H2_USER_RDO);
	m_aControlsAS_H2.Add(IDC_AS_SMALL_H2_EDT);
	m_aControlsAS_H2.Add(IDC_AS_SMALL_H2_UNT);

	m_aControlsAS.RemoveAll();
	m_aControlsAS.Append(m_aControlsAS_T1);
	m_aControlsAS.Append(m_aControlsAS_T2);
	m_aControlsAS.Append(m_aControlsAS_Small_T);
	m_aControlsAS.Append(m_aControlsAS_H1);
	m_aControlsAS.Append(m_aControlsAS_H2);

	//m_mvPicture.SetIgnoreLargeView();
}

void CStldBtmpDefCodeProvDlg::ResetSectTypeContents() 
{
	int nDgnCode = CDlgUtil::CobxGetCurSelItemData(m_cbxDefCode, m_cbxDefCode.GetCurSel());

	if(nDgnCode == D_BTMP_CODE_SNIP_2050384 || nDgnCode == D_BTMP_CODE_SP_35_13330_2011)
	{
		m_cbxSectType.ResetContent();
		CDlgUtil::CobxAddItem(m_cbxSectType, _LS(IDS_STL_COMP), 0);
		m_cbxSectType.SetCurSel(0);
	}
	else if(nDgnCode == D_BTMP_CODE_JTG_D60_2015)
	{
		m_cbxSectType.ResetContent();
		CDlgUtil::CobxAddItem(m_cbxSectType, _LS(IDS_STL_PSC), 0);
		CDlgUtil::CobxAddItem(m_cbxSectType, _LS(IDS_STL_STEEL_COMP), 1);
#if defined(_CH)
		CDlgUtil::CobxAddItem(m_cbxSectType, _T("SRC(Dumbbell)"), 2);
#endif
		m_cbxSectType.SetCurSel(0);
	}
    else if(nDgnCode == D_BTMP_CODE_IRC6_2017)
    {
        m_cbxSectType.ResetContent();
        CDlgUtil::CobxAddItem(m_cbxSectType, _LS(IDS_STL_PSC), 0);
        CDlgUtil::CobxAddItem(m_cbxSectType, _LS(IDS_STL_STEEL_COMP), 1);
        m_cbxSectType.SetCurSel(0);
    }
	else if(nDgnCode == D_BTMP_CODE_AASHTO_LRFD_2020 || nDgnCode == D_BTMP_CODE_EUROCODE)
	{
		m_cbxSectType.ResetContent();
		CDlgUtil::CobxAddItem(m_cbxSectType, _LS(IDS_STL_PSC), 0);
		CDlgUtil::CobxAddItem(m_cbxSectType, _LS(IDS_STL_STEEL_COMP), 1);
		m_cbxSectType.SetCurSel(0);
	}
	else if(nDgnCode == D_BTMP_CODE_AS)
	{
		m_cbxSectType.ResetContent();
		CDlgUtil::CobxAddItem(m_cbxSectType, _LS(IDS_STL_PSC)         , 0);
		CDlgUtil::CobxAddItem(m_cbxSectType, _LS(IDS_STL_STEEL_COMP)  , 1);
		m_cbxSectType.SetCurSel(0);
	}
	else
		ASSERT(0);

}

void CStldBtmpDefCodeProvDlg::SetInitData()
{
	m_Data.Snip2050284.Initialize();
	m_Data.Jtg2015_2018.Initialize();
	m_Data.IRC6_2017.Initialize();
	m_Data.AASHTO_2020.Initialize();
	m_Data.EuroCode.Initialize();
	m_Data.AsCode.Initialize();
}

void CStldBtmpDefCodeProvDlg::OnOk() 
{
	if(!Dlg2Data())
	{
		return;
	}
	
	if     (m_Data.nDgnCode == D_BTMP_CODE_AASHTO_LRFD_2020)  IUsageCounter::Use(_T("BTMP04"), FALSE);
	else if(m_Data.nDgnCode == D_BTMP_CODE_EUROCODE        )  IUsageCounter::Use(_T("BTMP05"), FALSE);
	else if(m_Data.nDgnCode == D_BTMP_CODE_AS              )  IUsageCounter::Use(_T("BTMP06"), FALSE);

	CDialogMove::OnOK();
}

void CStldBtmpDefCodeProvDlg::OnSelchangeModelTypCbx()
{
	ShowHideCtrl();
	AlignCtrl();
	ResizeDialog();
	SetData2Dlg();
	ChangeBitmap();
}

void CStldBtmpDefCodeProvDlg::OnSelchangeTypeCbx() //Code
{
	ResetSectTypeContents();
	SetInitData();
	ShowHideCtrl();
	AlignCtrl();
	ResizeDialog();
	SetData2Dlg(TRUE);
	ChangeBitmap();
}

void CStldBtmpDefCodeProvDlg::OnClickDumbRdo()
{
	int nDgnCode = CDlgUtil::CobxGetCurSelItemData(m_cbxDefCode, m_cbxDefCode.GetCurSel());
	int nSectType = m_cbxSectType.GetCurSel();

	if (nDgnCode != D_BTMP_CODE_JTG_D60_2015) return;
	if (nSectType != 2) return;

	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoDumbType, m_Data.Jtg2015_2018.nDumbType);
	if (m_Data.Jtg2015_2018.nDumbType == 0) // Single-tube
	{
		m_Data.Jtg2015_2018.dT1_Dumb = 8.0;
		m_Data.Jtg2015_2018.dT2_Dumb = 6.0;
	}
	else if (m_Data.Jtg2015_2018.nDumbType == 1) //Dumbbell
	{
		m_Data.Jtg2015_2018.dT1_Dumb = 5.0;
		m_Data.Jtg2015_2018.dT2_Dumb = 0.0;
	}
	m_edtJTGTDumb1.SetEditUnit(m_Data.Jtg2015_2018.dT1_Dumb);
	m_edtJTGTDumb2.SetEditUnit(m_Data.Jtg2015_2018.dT2_Dumb);

	ShowHideCtrl();
	AlignCtrl();
	SetData2Dlg();
	ChangeBitmap();

	GetDlgItem(IDC_JTG_DUMB_T2_STC)->EnableWindow(m_Data.Jtg2015_2018.nDumbType == 0);
	GetDlgItem(IDC_JTG_DUMB_T2_EDT)->EnableWindow(m_Data.Jtg2015_2018.nDumbType == 0);
	GetDlgItem(IDC_JTG_DUMB_T2_UNT)->EnableWindow(m_Data.Jtg2015_2018.nDumbType == 0);
}

void CStldBtmpDefCodeProvDlg::OnClickGradRdo()
{
	int nDgnCode = CDlgUtil::CobxGetCurSelItemData(m_cbxDefCode, m_cbxDefCode.GetCurSel());

	switch (nDgnCode)
	{
	case D_BTMP_CODE_SNIP_2050384:
	case D_BTMP_CODE_SP_35_13330_2011:
	case D_BTMP_CODE_JTG_D60_2015:
	case D_BTMP_CODE_IRC6_2017:
		return;
	default:
		break;
	}

	if(nDgnCode == D_BTMP_CODE_AASHTO_LRFD_2020)
	{
		CDlgUtil::CtrlRadioGetCheck(this, m_aRdoSnipTempGrad, m_Data.AASHTO_2020.nTempGrad);
		ShowHideCtrl();
		AlignCtrl();
		//ResizeDialog();
		SetData2Dlg();
		ChangeBitmap();
	}
	else if(nDgnCode == D_BTMP_CODE_EUROCODE)
	{
		CDlgUtil::CtrlRadioGetCheck(this, m_aRdoSnipTempGrad, m_Data.EuroCode.nTempGrad);
		ShowHideCtrl();
		AlignCtrl();
		//ResizeDialog();
		SetData2Dlg();
		ChangeBitmap();
	}
	else if(nDgnCode == D_BTMP_CODE_AS)
	{
		CDlgUtil::CtrlRadioGetCheck(this, m_aRdoSnipTempGrad, m_Data.AsCode.nTempGrad);
		ShowHideCtrl();
		AlignCtrl();
		//ResizeDialog();
		SetData2Dlg();
		ChangeBitmap();
	}

}

void CStldBtmpDefCodeProvDlg::OnClickAASHTO_SmalT_Rdo()
{
	int nDgnCode = CDlgUtil::CobxGetCurSelItemData(m_cbxDefCode, m_cbxDefCode.GetCurSel());
	if(nDgnCode != D_BTMP_CODE_AASHTO_LRFD_2020) return;

	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoAASHTOSmallT, m_Data.AASHTO_2020.nSmallT);

	GetDlgItem(IDC_AASHTO_SMALL_T_EDT)->EnableWindow(m_Data.AASHTO_2020.nSmallT==1);
	GetDlgItem(IDC_AASHTO_SMALL_T_UNT)->EnableWindow(m_Data.AASHTO_2020.nSmallT==1);
}

void CStldBtmpDefCodeProvDlg::OnClickAASHTO_A_Rdo()
{
	int nDgnCode = CDlgUtil::CobxGetCurSelItemData(m_cbxDefCode, m_cbxDefCode.GetCurSel());
	if(nDgnCode != D_BTMP_CODE_AASHTO_LRFD_2020) return;

	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoAASHTO_A, m_Data.AASHTO_2020.nA);

	GetDlgItem(IDC_AASHTO_A_EDT)->EnableWindow(m_Data.AASHTO_2020.nA==1);
	GetDlgItem(IDC_AASHTO_A_UNT)->EnableWindow(m_Data.AASHTO_2020.nA==1);
}

void CStldBtmpDefCodeProvDlg::OnClickEuro_Procedure_Rdo()
{
	int nDgnCode = CDlgUtil::CobxGetCurSelItemData(m_cbxDefCode, m_cbxDefCode.GetCurSel());
	if(nDgnCode != D_BTMP_CODE_EUROCODE) return;

	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoEuroPro, m_Data.EuroCode.nProcedure);

	// 삽도, 컨트롤 변경
	ShowHideCtrl();
	AlignCtrl();
	//ResizeDialog();
	SetData2Dlg();
	ChangeBitmap();
}
void CStldBtmpDefCodeProvDlg::OnClickEuro_H0_Rdo()
{
	int nDgnCode = CDlgUtil::CobxGetCurSelItemData(m_cbxDefCode, m_cbxDefCode.GetCurSel());
	if(nDgnCode != D_BTMP_CODE_EUROCODE) return;

	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoEuroH0, m_Data.EuroCode.nH0);
	GetDlgItem(IDC_EURO_SMALL_H_EDT)->EnableWindow(m_Data.EuroCode.nH0==1);
	GetDlgItem(IDC_EURO_SMALL_H_UNT)->EnableWindow(m_Data.EuroCode.nH0==1);
}
void CStldBtmpDefCodeProvDlg::OnClickEuro_H1_Rdo()
{
	int nDgnCode = CDlgUtil::CobxGetCurSelItemData(m_cbxDefCode, m_cbxDefCode.GetCurSel());
	if(nDgnCode != D_BTMP_CODE_EUROCODE) return;

	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoEuroH1, m_Data.EuroCode.nH1);
	GetDlgItem(IDC_EURO_SMALL_H1_EDT)->EnableWindow(m_Data.EuroCode.nH1==1);
	GetDlgItem(IDC_EURO_SMALL_H1_UNT)->EnableWindow(m_Data.EuroCode.nH1==1);
}
void CStldBtmpDefCodeProvDlg::OnClickEuro_H2_Rdo()
{
	int nDgnCode = CDlgUtil::CobxGetCurSelItemData(m_cbxDefCode, m_cbxDefCode.GetCurSel());
	if(nDgnCode != D_BTMP_CODE_EUROCODE) return;

	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoEuroH2, m_Data.EuroCode.nH2);
	GetDlgItem(IDC_EURO_SMALL_H2_EDT)->EnableWindow(m_Data.EuroCode.nH2==1);
	GetDlgItem(IDC_EURO_SMALL_H2_UNT)->EnableWindow(m_Data.EuroCode.nH2==1);
}
void CStldBtmpDefCodeProvDlg::OnClickEuro_H3_Rdo()
{
	int nDgnCode = CDlgUtil::CobxGetCurSelItemData(m_cbxDefCode, m_cbxDefCode.GetCurSel());
	if(nDgnCode != D_BTMP_CODE_EUROCODE) return;

	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoEuroH3, m_Data.EuroCode.nH3);
	GetDlgItem(IDC_EURO_SMALL_H3_EDT)->EnableWindow(m_Data.EuroCode.nH3==1);
	GetDlgItem(IDC_EURO_SMALL_H3_UNT)->EnableWindow(m_Data.EuroCode.nH3==1);
}
void CStldBtmpDefCodeProvDlg::OnClickEuro_H4_Rdo()
{
	int nDgnCode = CDlgUtil::CobxGetCurSelItemData(m_cbxDefCode, m_cbxDefCode.GetCurSel());
	if(nDgnCode != D_BTMP_CODE_EUROCODE) return;

	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoEuroH4, m_Data.EuroCode.nH4);
	GetDlgItem(IDC_EURO_SMALL_H4_EDT)->EnableWindow(m_Data.EuroCode.nH4==1);
	GetDlgItem(IDC_EURO_SMALL_H4_UNT)->EnableWindow(m_Data.EuroCode.nH4==1);
}

void CStldBtmpDefCodeProvDlg::OnClickAS_SmalT_Rdo()
{
	int nDgnCode = CDlgUtil::CobxGetCurSelItemData(m_cbxDefCode, m_cbxDefCode.GetCurSel());
	if(nDgnCode != D_BTMP_CODE_AS) return;

	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoAsSmallT, m_Data.AsCode.nSmallT);

	GetDlgItem(IDC_AS_SMALL_T_EDT)->EnableWindow(m_Data.AsCode.nSmallT==1);
	GetDlgItem(IDC_AS_SMALL_T_UNT)->EnableWindow(m_Data.AsCode.nSmallT==1);
}

// void CStldBtmpDefCodeProvDlg::OnClickAS_H1_Rdo()
// {
// 	int nDgnCode = CDlgUtil::CobxGetCurSelItemData(m_cbxDefCode, m_cbxDefCode.GetCurSel());
// 	if(nDgnCode != D_BTMP_CODE_AS) return;
// 
// 	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoAsH1, m_Data.AsCode.nH1);
// 	GetDlgItem(IDC_AS_SMALL_H1_EDT)->EnableWindow(m_Data.AsCode.nH1==1);
// 	GetDlgItem(IDC_AS_SMALL_H1_UNT)->EnableWindow(m_Data.AsCode.nH1==1);
// }
// 
// void CStldBtmpDefCodeProvDlg::OnClickAS_H2_Rdo()
// {
// 	int nDgnCode = CDlgUtil::CobxGetCurSelItemData(m_cbxDefCode, m_cbxDefCode.GetCurSel());
// 	if(nDgnCode != D_BTMP_CODE_AS) return;
// 
// 	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoAsH2, m_Data.AsCode.nH2);
// 	GetDlgItem(IDC_AS_SMALL_H2_EDT)->EnableWindow(m_Data.AsCode.nH2==1);
// 	GetDlgItem(IDC_AS_SMALL_H2_UNT)->EnableWindow(m_Data.AsCode.nH2==1);
// }


BOOL CStldBtmpDefCodeProvDlg::CheckData()
{
	m_Data.nDgnCode = CDlgUtil::CobxGetCurSelItemData(m_cbxDefCode, m_cbxDefCode.GetCurSel());

	// 기존 기준에 대해서는 체크 안함.
	switch (m_Data.nDgnCode)
	{
	case D_BTMP_CODE_SNIP_2050384:
	case D_BTMP_CODE_SP_35_13330_2011:
	case D_BTMP_CODE_JTG_D60_2015:
	case D_BTMP_CODE_IRC6_2017:
		return TRUE;
	case D_BTMP_CODE_AASHTO_LRFD_2020:
		return CheckDataAASHTO();
	case D_BTMP_CODE_EUROCODE:
		return CheckDataEuro();
	case D_BTMP_CODE_AS:
		return CheckDataAs();
	default:

		break;
	}

	return TRUE;
}

BOOL CStldBtmpDefCodeProvDlg::Dlg2Data() 
{
	if(!CheckData()) return FALSE;

	m_Data.nDgnCode = CDlgUtil::CobxGetCurSelItemData(m_cbxDefCode, m_cbxDefCode.GetCurSel());

	switch (m_Data.nDgnCode)
	{
	case D_BTMP_CODE_SNIP_2050384:
	case D_BTMP_CODE_SP_35_13330_2011:
		return Dlg2DataSnip();
	case D_BTMP_CODE_JTG_D60_2015:
		return Dlg2DataJTG();
    case D_BTMP_CODE_IRC6_2017:
        return Dlg2DataIRC6();
	case D_BTMP_CODE_AASHTO_LRFD_2020:
		return Dlg2DataAASHTO();
	case D_BTMP_CODE_EUROCODE:
		return Dlg2DataEuro();
	case D_BTMP_CODE_AS:
		return Dlg2DataAS();
	default:
		break;
	}
	
	return TRUE;
}

BOOL CStldBtmpDefCodeProvDlg::Dlg2DataSnip() 
{
	m_Data.Snip2050284.nSectType = m_cbxSectType.GetCurSel();
	m_Data.Snip2050284.dTMax = m_edtSnipTMax.GetEditValue();  
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoSnipTempGrad, m_Data.Snip2050284.nTempGrad);

	return TRUE;
}

BOOL CStldBtmpDefCodeProvDlg::Dlg2DataJTG() 
{
	m_Data.Jtg2015_2018.nSectType = m_cbxSectType.GetCurSel();
	m_Data.Jtg2015_2018.dT1 = m_edtJTGT1.GetEditValue();
	m_Data.Jtg2015_2018.dT2 = m_edtJTGT2.GetEditValue();
	m_Data.Jtg2015_2018.dSmallT = m_edtJTGSmallT.GetEditValue();
	m_Data.Jtg2015_2018.dT1_Dumb = m_edtJTGTDumb1.GetEditValue();
	m_Data.Jtg2015_2018.dT2_Dumb = m_edtJTGTDumb2.GetEditValue();

	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoDumbType, m_Data.Jtg2015_2018.nDumbType);
	return TRUE;
}


BOOL CStldBtmpDefCodeProvDlg::Dlg2DataIRC6() 
{
    m_Data.IRC6_2017.nSectType = m_cbxSectType.GetCurSel();    
    CDlgUtil::CtrlRadioGetCheck(this, m_aRdoSnipTempGrad, m_Data.IRC6_2017.nTempGrad);
    if (m_Data.IRC6_2017.nSectType==0)//PSC Section
    {
        if (m_Data.IRC6_2017.nTempGrad==1)
        {
            //For Reverse Gradient
            m_Data.IRC6_2017.dT1 = -10.6;
            m_Data.IRC6_2017.dT2 = -0.7;
            m_Data.IRC6_2017.dT3 = -0.8;
            m_Data.IRC6_2017.dT4 = -6.6;

        }
    }
    else // StlComposite Section
    {
        if (m_Data.IRC6_2017.nTempGrad==0)//Positive
        {
            //For Positive Gradient
            m_Data.IRC6_2017.dT1 = 0;
            m_Data.IRC6_2017.dT2 = 4.;
            m_Data.IRC6_2017.dT3 = 0.0;
            m_Data.IRC6_2017.dT4 = 0.0;

        }
        else
        {
            //For Reverse Gradient
            m_Data.IRC6_2017.dT1 = -4.4;
            m_Data.IRC6_2017.dT2 = 0.;
            m_Data.IRC6_2017.dT3 = 0.;
            m_Data.IRC6_2017.dT4 = -8.;
        }
    }
   
    return TRUE;
}

BOOL CStldBtmpDefCodeProvDlg::Dlg2DataAASHTO() 
{
	m_Data.AASHTO_2020.nSectType = m_cbxSectType.GetCurSel();    
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoSnipTempGrad, m_Data.AASHTO_2020.nTempGrad);
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoAASHTOSmallT, m_Data.AASHTO_2020.nSmallT);
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoAASHTO_A    , m_Data.AASHTO_2020.nA);

	m_Data.AASHTO_2020.dT1     = m_edtAASHTOT1.GetEditValue();
	m_Data.AASHTO_2020.dT2     = m_edtAASHTOT2.GetEditValue();
	m_Data.AASHTO_2020.dT3     = m_edtAASHTOT3.GetEditValue();
	m_Data.AASHTO_2020.dA      = m_edtAASHTOTA.GetEditValue();
	m_Data.AASHTO_2020.dSmallT = m_edtAASHTOTSmallT.GetEditValue();

	return TRUE;
}

BOOL CStldBtmpDefCodeProvDlg::Dlg2DataEuro() 
{
	m_Data.EuroCode.nSectType = m_cbxSectType.GetCurSel();    

	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoSnipTempGrad, m_Data.EuroCode.nTempGrad);
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoEuroPro, m_Data.EuroCode.nProcedure);
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoEuroH0 , m_Data.EuroCode.nH0);
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoEuroH1 , m_Data.EuroCode.nH1);
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoEuroH2 , m_Data.EuroCode.nH2);
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoEuroH3 , m_Data.EuroCode.nH3);
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoEuroH4 , m_Data.EuroCode.nH4);

	m_Data.EuroCode.dT1 = m_edtEuroT1.GetEditValue();
	m_Data.EuroCode.dT2 = m_edtEuroT2.GetEditValue();
	m_Data.EuroCode.dT3 = m_edtEuroT3.GetEditValue();
	m_Data.EuroCode.dT4 = m_edtEuroT4.GetEditValue();
	m_Data.EuroCode.dH0 = m_edtEuroH0.GetEditValue();
	m_Data.EuroCode.dH1 = m_edtEuroH1.GetEditValue();
	m_Data.EuroCode.dH2 = m_edtEuroH2.GetEditValue();
	m_Data.EuroCode.dH3 = m_edtEuroH3.GetEditValue();
	m_Data.EuroCode.dH4 = m_edtEuroH4.GetEditValue();

	return TRUE;
}

BOOL CStldBtmpDefCodeProvDlg::Dlg2DataAS() 
{
	m_Data.AsCode.nSectType = m_cbxSectType.GetCurSel();    

	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoSnipTempGrad, m_Data.AsCode.nTempGrad);
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoAsSmallT    , m_Data.AsCode.nSmallT);
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoAsH1        , m_Data.AsCode.nH1);
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoAsH2        , m_Data.AsCode.nH2);

	m_Data.AsCode.dSmallT = m_edtAsSmallT.GetEditValue();
	m_Data.AsCode.dT1 = m_edtAsT1.GetEditValue();
	m_Data.AsCode.dT2 = m_edtAsT2.GetEditValue();
	m_Data.AsCode.dH1 = m_edtAsH1.GetEditValue();
	m_Data.AsCode.dH2 = m_edtAsH2.GetEditValue();

	return TRUE;
}


BOOL CStldBtmpDefCodeProvDlg::CheckDataAASHTO() 
{
	int nSectType = m_cbxSectType.GetCurSel();    
	int nTempGrad = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoSnipTempGrad, nTempGrad);

	double dT1     = m_edtAASHTOT1.GetEditValue();
	double dT2     = m_edtAASHTOT2.GetEditValue();
	double dT3     = m_edtAASHTOT3.GetEditValue();
	double dA      = m_edtAASHTOTA.GetEditValue();
	double dSmallT = m_edtAASHTOTSmallT.GetEditValue();

	if(nSectType==0) //PSC
	{
		if(nTempGrad==0) // Positive
		{
			if(dT1 <= 0.0 || dT2 <= 0.0 || dT3 <= 0.0){ AfxMessageBox(_T("Temperature must be larger than zero.")); return FALSE;}
		}
		else if(nTempGrad==1) // Negative
		{
			if(dT1 >= 0.0 || dT2 >= 0.0 || dT3 >= 0.0){ AfxMessageBox(_T("Temperature must be less than zero.")); return FALSE;}
		}
	}
	else if(nSectType==1) //Comp
	{
		if(nTempGrad==0) // Positive
		{
			if(dT1 <= 0.0 || dT2 <= 0.0)              { AfxMessageBox(_T("Temperature must be larger than zero.")); return FALSE;}

			if(dSmallT <= 0.0)                        { AfxMessageBox(_T("t must be larger than zero.")); return FALSE;}
		}
		else if(nTempGrad==1) // Negative
		{
			if(dT1 >= 0.0 || dT2 >= 0.0)             { AfxMessageBox(_T("Temperature must be less than zero.")); return FALSE;}

			if(dSmallT <= 0.0)                       { AfxMessageBox(_T("t must be larger than zero.")); return FALSE;}
		} 
	}

	if(fabs(dT1) < fabs(dT2))  { AfxMessageBox(_T("ABS(T1) must be larger than ABS(T2).")); return FALSE;}

	if(dA <= 0.0)  { AfxMessageBox(_T("A must be larger than zero.")); return FALSE;}

	return TRUE;
}


BOOL CStldBtmpDefCodeProvDlg::CheckDataEuro() 
{
	int nSectType = m_cbxSectType.GetCurSel();    
	int nTempGrad = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoSnipTempGrad, nTempGrad);
	int nProcedure = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoEuroPro, nProcedure);

	double dT1 = m_edtEuroT1.GetEditValue();
	double dT2 = m_edtEuroT2.GetEditValue();
	double dT3 = m_edtEuroT3.GetEditValue();
	double dT4 = m_edtEuroT4.GetEditValue();

	double dH0 = m_edtEuroH0.GetEditValue();
	double dH1 = m_edtEuroH1.GetEditValue();
	double dH2 = m_edtEuroH2.GetEditValue();
	double dH3 = m_edtEuroH3.GetEditValue();
	double dH4 = m_edtEuroH4.GetEditValue();

	if(nSectType==0) //PSC
	{
		if(nTempGrad==0) // Heat
		{
			if(dT1 <= 0.0 || dT2 <= 0.0 || dT3 <= 0.0){ AfxMessageBox(_T("Temperature must be larger than zero.")); return FALSE;}
			if(dH1 <= 0.0 || dH2 <= 0.0 || dH3 <= 0.0){ AfxMessageBox(_T("Height information must be larger than zero.")); return FALSE;}

			if(fabs(dT1) < fabs(dT2))  { AfxMessageBox(_T("ABS(T1) must be larger than ABS(T2).")); return FALSE;}
		}
		else if(nTempGrad==1) // Cool
		{
			if(dT1 >= 0.0 || dT2 >= 0.0 || dT3 >= 0.0 || dT4 >= 0.0){ AfxMessageBox(_T("Temperature must be less than zero.")); return FALSE;}
			if(dH1 <= 0.0 || dH2 <= 0.0 || dH3 <= 0.0 || dH4 <= 0.0){ AfxMessageBox(_T("Height information must be larger than zero.")); return FALSE;}

			if(fabs(dT1) < fabs(dT2))  { AfxMessageBox(_T("ABS(T1) must be larger than ABS(T2).")); return FALSE;}
			if(fabs(dT4) < fabs(dT3))  { AfxMessageBox(_T("ABS(T4) must be larger than ABS(T3).")); return FALSE;}
		}
	}
	else if(nSectType==1) //Comp
	{		
		if(nProcedure==0) // Normal
		{
			if(dH0 <= 0.0 || dH1 <= 0.0 || dH2 <= 0.0){ AfxMessageBox(_T("Height information must be larger than zero.")); return FALSE;}

			if(nTempGrad==0) // Heat
			{
				if(dT1 <= 0.0 || dT2 <= 0.0){ AfxMessageBox(_T("Temperature must be larger than zero.")); return FALSE;}
			}
			else if(nTempGrad==1) // Cool
			{
				if(dT1 >= 0.0 || dT2 >= 0.0){ AfxMessageBox(_T("Temperature must be less than zero.")); return FALSE;}
			}
		}
		else if(nProcedure==1)
		{
			if(dH0 <= 0.0){ AfxMessageBox(_T("Height information must be larger than zero.")); return FALSE;}

			if(nTempGrad==0) // Heat
			{
				if(dT1 <= 0.0){ AfxMessageBox(_T("Temperature must be larger than zero.")); return FALSE;}
			}
			else if(nTempGrad==1) // Cool
			{
				if(dT1 >= 0.0){ AfxMessageBox(_T("Temperature must be less than zero.")); return FALSE;}
			}
		}
	}

	return TRUE;
}

BOOL CStldBtmpDefCodeProvDlg::CheckDataAs()
{
	int nSectType = m_cbxSectType.GetCurSel();    
	int nTempGrad = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoSnipTempGrad, nTempGrad);

	double dT1 = m_edtAsT1.GetEditValue();
	double dT2 = m_edtAsT2.GetEditValue();

	double dH1 = m_edtAsH1.GetEditValue();
	double dH2 = m_edtAsH2.GetEditValue();


	if(nSectType == 0) //Psc.
	{
		if(dH1 <= 0.0 || dH2 < 0.0){ AfxMessageBox(_T("Height information must be larger than zero.")); return FALSE;}

		if(m_Data.AsCode.nTempGrad == 0)
		{
			if(dT1 <= 0.0 || dT2 <= 0.0){ AfxMessageBox(_T("Temperature must be larger than zero.")); return FALSE;}
		}
		else if(m_Data.AsCode.nTempGrad == 1)
		{
			//if(dT1 >= 0.0){ AfxMessageBox(_T("Temperature must be less than zero.")); return FALSE;}
			if (dT1 <= 0.0) { AfxMessageBox(_T("Temperature must be larger than zero.")); return FALSE; }
		}
	}
	else if(nSectType == 1) //Psc Box, Super-T
	{
		if(dH1 <= 0.0 || dH2 < 0.0){ AfxMessageBox(_T("Height information must be larger than zero.")); return FALSE;}

		if(m_Data.AsCode.nTempGrad == 0)  
		{
			if(dT1 <= 0.0){ AfxMessageBox(_T("Temperature must be larger than zero.")); return FALSE;}
		}
		else if(m_Data.AsCode.nTempGrad == 1)  
		{
			//if(dT1 >= 0.0){ AfxMessageBox(_T("Temperature must be less than zero.")); return FALSE;}
			if (dT1 <= 0.0) { AfxMessageBox(_T("Temperature must be larger than zero.")); return FALSE; }
		}
	}
	else if(nSectType == 2) //Stl Comp Box
	{
		if(dH1 <= 0.0) 	{ AfxMessageBox(_T("Height information must be larger than zero.")); return FALSE;}

		if(m_Data.AsCode.nTempGrad == 0)
		{
			if(dT1 <= 0.0){ AfxMessageBox(_T("Temperature must be larger than zero.")); return FALSE;}
		}
		else if(m_Data.AsCode.nTempGrad == 1)
		{
			//if(dT1 >= 0.0){ AfxMessageBox(_T("Temperature must be less than zero.")); return FALSE;}
			if (dT1 <= 0.0) { AfxMessageBox(_T("Temperature must be larger than zero.")); return FALSE; }
		}
	}
	else if(nSectType == 3) //Stl Comp Box
	{
		if(dH1 <= 0.0) { AfxMessageBox(_T("Height information must be larger than zero.")); return FALSE;}

		if(m_Data.AsCode.nTempGrad == 0)
		{
			if(dT1 <= 0.0) { AfxMessageBox(_T("Temperature must be larger than zero.")); return FALSE;}
		}
		else if(m_Data.AsCode.nTempGrad == 1)
		{
			//if(dT1 >= 0.0) { AfxMessageBox(_T("Temperature must be less than zero.")); return FALSE;}	
			if (dT1 <= 0.0) { AfxMessageBox(_T("Temperature must be larger than zero.")); return FALSE; }
		}
	}

	return TRUE;
}

void CStldBtmpDefCodeProvDlg::Data2Dlg() 
{
	CDlgUtil::CobxSetCurSelItemData(m_cbxDefCode, m_Data.nDgnCode);

	ResetSectTypeContents();

	switch (m_Data.nDgnCode)
	{
	case D_BTMP_CODE_SNIP_2050384:
	case D_BTMP_CODE_SP_35_13330_2011:
		return Data2DlgSnip();
	case D_BTMP_CODE_JTG_D60_2015:
		return Data2DlgJTG();
    case D_BTMP_CODE_IRC6_2017:
        return Data2DlgIRC();
	case D_BTMP_CODE_AASHTO_LRFD_2020:
		return Data2DlgAASHTO();
	case D_BTMP_CODE_EUROCODE:
		return Data2DlgAEuro();
	case D_BTMP_CODE_AS:
		return Data2DlgAS();

	default:

		break;
	}
}

void CStldBtmpDefCodeProvDlg::Data2DlgSnip()
{
	m_cbxSectType.SetCurSel(m_Data.Snip2050284.nSectType);

	m_edtSnipTMax.SetEditUnit(m_Data.Snip2050284.dTMax);  
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSnipTempGrad, m_Data.Snip2050284.nTempGrad);
}

void CStldBtmpDefCodeProvDlg::Data2DlgJTG()
{
	m_cbxSectType.SetCurSel(m_Data.Jtg2015_2018.nSectType);

	m_edtJTGT1.SetEditUnit(m_Data.Jtg2015_2018.dT1);
	m_edtJTGT2.SetEditUnit(m_Data.Jtg2015_2018.dT2);
	m_edtJTGSmallT.SetEditUnit(m_Data.Jtg2015_2018.dSmallT);

	m_edtJTGTDumb1.SetEditUnit(m_Data.Jtg2015_2018.dT1_Dumb);
	m_edtJTGTDumb2.SetEditUnit(m_Data.Jtg2015_2018.dT2_Dumb);
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoDumbType, m_Data.Jtg2015_2018.nDumbType);
}

void CStldBtmpDefCodeProvDlg::Data2DlgIRC()
{
    m_cbxSectType.SetCurSel(m_Data.IRC6_2017.nSectType);
    CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSnipTempGrad, m_Data.IRC6_2017.nTempGrad);
}

void CStldBtmpDefCodeProvDlg::Data2DlgAASHTO()
{
	m_cbxSectType.SetCurSel(m_Data.AASHTO_2020.nSectType);
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSnipTempGrad, m_Data.AASHTO_2020.nTempGrad);
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoAASHTOSmallT, m_Data.AASHTO_2020.nSmallT);
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoAASHTO_A    , m_Data.AASHTO_2020.nA);

	m_edtAASHTOT1.SetEditUnit(m_Data.AASHTO_2020.dT1);
	m_edtAASHTOT2.SetEditUnit(m_Data.AASHTO_2020.dT2);
	m_edtAASHTOT3.SetEditUnit(m_Data.AASHTO_2020.dT3);
	m_edtAASHTOTA.SetEditUnit(m_Data.AASHTO_2020.dA );
	m_edtAASHTOTSmallT.SetEditUnit(m_Data.AASHTO_2020.dSmallT);

}

void CStldBtmpDefCodeProvDlg::Data2DlgAEuro()
{
	m_cbxSectType.SetCurSel(m_Data.EuroCode.nSectType);
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSnipTempGrad, m_Data.EuroCode.nTempGrad);
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoEuroPro, m_Data.EuroCode.nProcedure);
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoEuroH0 , m_Data.EuroCode.nH0);
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoEuroH1 , m_Data.EuroCode.nH1);
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoEuroH2 , m_Data.EuroCode.nH2);
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoEuroH3 , m_Data.EuroCode.nH3);
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoEuroH4 , m_Data.EuroCode.nH4);

	m_edtEuroT1.SetEditUnit(m_Data.EuroCode.dT1);
	m_edtEuroT2.SetEditUnit(m_Data.EuroCode.dT2);
	m_edtEuroT3.SetEditUnit(m_Data.EuroCode.dT3);
	m_edtEuroT4.SetEditUnit(m_Data.EuroCode.dT4);
	m_edtEuroH0.SetEditUnit(m_Data.EuroCode.dH0);
	m_edtEuroH1.SetEditUnit(m_Data.EuroCode.dH1);
	m_edtEuroH2.SetEditUnit(m_Data.EuroCode.dH2);
	m_edtEuroH3.SetEditUnit(m_Data.EuroCode.dH3);
	m_edtEuroH4.SetEditUnit(m_Data.EuroCode.dH4);

}

void CStldBtmpDefCodeProvDlg::Data2DlgAS()
{
	m_cbxSectType.SetCurSel(m_Data.AsCode.nSectType);

	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSnipTempGrad, m_Data.AsCode.nTempGrad);
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoAsSmallT    , m_Data.AsCode.nSmallT);
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoAsH1      , m_Data.AsCode.nH1);
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoAsH2      , m_Data.AsCode.nH2);

	m_edtAsSmallT.SetEditUnit(m_Data.AsCode.dSmallT);
	m_edtAsT1.SetEditUnit(m_Data.AsCode.dT1);
	m_edtAsT2.SetEditUnit(m_Data.AsCode.dT2);
	m_edtAsH1.SetEditUnit(m_Data.AsCode.dH1);
	m_edtAsH2.SetEditUnit(m_Data.AsCode.dH2);
}

void CStldBtmpDefCodeProvDlg::SetData2Dlg(BOOL bCodeChange/*=FALSE*/) 
{
	int nDgnCode = CDlgUtil::CobxGetCurSelItemData(m_cbxDefCode, m_cbxDefCode.GetCurSel());

	switch (nDgnCode)
	{
	case D_BTMP_CODE_SNIP_2050384:
	case D_BTMP_CODE_SP_35_13330_2011:
	case D_BTMP_CODE_JTG_D60_2015:
	case D_BTMP_CODE_IRC6_2017:
		return ;
	case D_BTMP_CODE_AASHTO_LRFD_2020:
		return SetData2DlgAASHTO(bCodeChange);
	case D_BTMP_CODE_EUROCODE:
		return SetData2DlgAEuro(bCodeChange);
	case D_BTMP_CODE_AS:
		return SetData2DlgAS(bCodeChange);
	default:
		break;
	}

}
void CStldBtmpDefCodeProvDlg::SetData2DlgAASHTO(BOOL bCodeChange/*=FALSE*/)
{
	m_Data.AASHTO_2020.dT1     = (m_Data.AASHTO_2020.nTempGrad==0)? 54.0 : -1*0.3*54.0;
	m_Data.AASHTO_2020.dT2     = (m_Data.AASHTO_2020.nTempGrad==0)? 14.0 : -1*0.3*14.0;
	m_Data.AASHTO_2020.dT3     = (m_Data.AASHTO_2020.nTempGrad==0)?  5.0 : -1*0.3* 5.0;
	m_Data.AASHTO_2020.dSmallT = 8.5;
	m_Data.AASHTO_2020.dA      = 12.0;

	m_edtAASHTOT1     .SetEditUnit(m_Data.AASHTO_2020.dT1);
	m_edtAASHTOT2     .SetEditUnit(m_Data.AASHTO_2020.dT2);
	m_edtAASHTOT3     .SetEditUnit(m_Data.AASHTO_2020.dT3);
	m_edtAASHTOTA     .SetEditUnit(m_Data.AASHTO_2020.dA );
	m_edtAASHTOTSmallT.SetEditUnit(m_Data.AASHTO_2020.dSmallT);

	if(bCodeChange)
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSnipTempGrad, m_Data.AASHTO_2020.nTempGrad);
		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoAASHTOSmallT, m_Data.AASHTO_2020.nSmallT);
		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoAASHTO_A    , m_Data.AASHTO_2020.nA);

 		OnClickAASHTO_SmalT_Rdo();
 		OnClickAASHTO_A_Rdo();
	}
}

void CStldBtmpDefCodeProvDlg::SetData2DlgAEuro(BOOL bCodeChange/*=FALSE*/)
{
	double dT1, dT2, dT3, dT4;
	double dH0, dH1, dH2, dH3, dH4;
	dT1 = dT2 = dT3 = dT4 = 0.0;
	dH0 = dH1 = dH2 = dH3 = dH4 = 0.0;

	int nSectType = m_cbxSectType.GetCurSel();

	if(nSectType == 0) //Psc.
	{
		if     (m_Data.EuroCode.nTempGrad == 0)  {dH0 =  0.8; dH1 = 0.15; dH2 = 0.25;  dH3 = 0.24;}
		else if(m_Data.EuroCode.nTempGrad == 1)  {dH0 =  1.5; dH1 = 0.25; dH2 = 0.20;  dH3 = 0.20; dH4 = 0.25;}

		if     (m_Data.EuroCode.nTempGrad == 0)  {dT1 = 13.0; dT2 =  3.0; dT3 =  2.5;}
		else if(m_Data.EuroCode.nTempGrad == 1)  {dT1 = -8.4; dT2 = -0.5; dT3 = -1.0;  dT4 = -6.5;}		
	}
	else if(nSectType == 1) //Comp.
	{
		if(m_Data.EuroCode.nProcedure == 0) // Normal
		{
			if     (m_Data.EuroCode.nTempGrad == 0)  {dH0 =  0.2; dH1 = 0.12; dH2 = 0.4;}  // Heating
			else if(m_Data.EuroCode.nTempGrad == 1)  {dH0 =  0.2; dH1 = 0.12; dH2 = 0.4;}  // Cooling

			if     (m_Data.EuroCode.nTempGrad == 0)  {dT1 = 13.0; dT2 =  4.0;}
			else if(m_Data.EuroCode.nTempGrad == 1)  {dT1 = -3.5; dT2 = -8.0;}		
		}
		else if(m_Data.EuroCode.nProcedure == 1) // Simple
		{
			if     (m_Data.EuroCode.nTempGrad == 0)  {dH0 = 0.2;}
			else if(m_Data.EuroCode.nTempGrad == 1)  {dH0 = 0.2;}

			if     (m_Data.EuroCode.nTempGrad == 0)  {dT1 = 10.0;}
			else if(m_Data.EuroCode.nTempGrad == 1)  {dT1 =-10.0;}		
		}
	}

	m_Data.EuroCode.dT1 = dT1;
	m_Data.EuroCode.dT2 = dT2;
	m_Data.EuroCode.dT3 = dT3;
	m_Data.EuroCode.dT4 = dT4;
	m_Data.EuroCode.dH0 = dH0;
	m_Data.EuroCode.dH1 = dH1;
	m_Data.EuroCode.dH2 = dH2;
	m_Data.EuroCode.dH3 = dH3;
	m_Data.EuroCode.dH4 = dH4;

	m_edtEuroT1.SetEditUnit(m_Data.EuroCode.dT1);
	m_edtEuroT2.SetEditUnit(m_Data.EuroCode.dT2);
	m_edtEuroT3.SetEditUnit(m_Data.EuroCode.dT3);
	m_edtEuroT4.SetEditUnit(m_Data.EuroCode.dT4);
	m_edtEuroH0.SetEditUnit(m_Data.EuroCode.dH0);
	m_edtEuroH1.SetEditUnit(m_Data.EuroCode.dH1);
	m_edtEuroH2.SetEditUnit(m_Data.EuroCode.dH2);
	m_edtEuroH3.SetEditUnit(m_Data.EuroCode.dH3);
	m_edtEuroH4.SetEditUnit(m_Data.EuroCode.dH4);

	if(bCodeChange)
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSnipTempGrad, m_Data.EuroCode.nTempGrad);
		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoEuroPro, m_Data.EuroCode.nProcedure);
		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoEuroH0 , m_Data.EuroCode.nH0);
		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoEuroH1 , m_Data.EuroCode.nH1);
		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoEuroH2 , m_Data.EuroCode.nH2);
		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoEuroH3 , m_Data.EuroCode.nH3);
		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoEuroH4 , m_Data.EuroCode.nH4);
	}
}

void CStldBtmpDefCodeProvDlg::SetData2DlgAS(BOOL bCodeChange/*=FALSE*/)
{

	double dT1, dT2;
	double dH1, dH2;
	dT1 = dT2 = 0.0;
	dH1 = dH2 = 0.0;

	int nSectType = m_cbxSectType.GetCurSel();

	if(nSectType == 0) //Psc.
	{
		if     (m_Data.AsCode.nTempGrad == 0)  {dH1 = 300.0; dH2 = 200.0;}
		else if(m_Data.AsCode.nTempGrad == 1)  {dH1 = 300.0; dH2 = 200.0;}

		if     (m_Data.AsCode.nTempGrad == 0)  {dT1 = 20.0;  dT2 =  5.0;}
		else if(m_Data.AsCode.nTempGrad == 1)  {dT1 = 20.0;}		
	}
	else if(nSectType == 1) //Psc Box, Super-T
	{
		if     (m_Data.AsCode.nTempGrad == 0)  {dH1 = 300.0; dH2 = 200.0;}
		else if(m_Data.AsCode.nTempGrad == 1)  {dH1 = 300.0; dH2 = 200.0;}

		if     (m_Data.AsCode.nTempGrad == 0)  {dT1 = 20.0;}
		else if(m_Data.AsCode.nTempGrad == 1)  {dT1 = 20.0;}		
	}
	else if(nSectType == 2) //Stl Comp Box
	{
		if     (m_Data.AsCode.nTempGrad == 0)  {dH1 = 300.0;}
		else if(m_Data.AsCode.nTempGrad == 1)  {dH1 = 300.0;}

		if     (m_Data.AsCode.nTempGrad == 0)  {dT1 = 20.0;}
		else if(m_Data.AsCode.nTempGrad == 1)  {dT1 = 20.0;}		
	}
	else if(nSectType == 3) //Stl Comp Box
	{
		if     (m_Data.AsCode.nTempGrad == 0)  {dH1 = 300.0;}
		else if(m_Data.AsCode.nTempGrad == 1)  {dH1 = 300.0;}

		if     (m_Data.AsCode.nTempGrad == 0)  {dT1 = 20.0;}
		else if(m_Data.AsCode.nTempGrad == 1)  {dT1 = 20.0;}		
	}

	m_Data.AsCode.dSmallT = 300.0;
	m_Data.AsCode.dT1 = dT1;
	m_Data.AsCode.dT2 = dT2;
	m_Data.AsCode.dH1 = dH1;
	m_Data.AsCode.dH2 = dH2;

	m_edtAsSmallT.SetEditUnit(m_Data.AsCode.dSmallT);
	m_edtAsT1.SetEditUnit(m_Data.AsCode.dT1);
	m_edtAsT2.SetEditUnit(m_Data.AsCode.dT2);
	m_edtAsH1.SetEditUnit(m_Data.AsCode.dH1);
	m_edtAsH2.SetEditUnit(m_Data.AsCode.dH2);

	if(bCodeChange)
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSnipTempGrad, m_Data.AsCode.nTempGrad);
		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoAsSmallT    , m_Data.AsCode.nSmallT);
		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoAsH1        , m_Data.AsCode.nH1);
		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoAsH2        , m_Data.AsCode.nH2);
	}
}


void CStldBtmpDefCodeProvDlg::ShowHideCtrl()
{
	int nDgnCode = CDlgUtil::CobxGetCurSelItemData(m_cbxDefCode, m_cbxDefCode.GetCurSel());
	int nSectType = m_cbxSectType.GetCurSel();

	BOOL bDgnCodeNew =(nDgnCode == D_BTMP_CODE_AASHTO_LRFD_2020 || nDgnCode == D_BTMP_CODE_EUROCODE || nDgnCode == D_BTMP_CODE_AS )? TRUE : FALSE;

	GetDlgItem(IDC_TEMP_GRAD_GRP      )->ShowWindow(nDgnCode == D_BTMP_CODE_SNIP_2050384 || nDgnCode == D_BTMP_CODE_SP_35_13330_2011 || nDgnCode == D_BTMP_CODE_IRC6_2017 || bDgnCodeNew);
	GetDlgItem(IDC_TEMP_GRAD_TYPE1_RDO)->ShowWindow(nDgnCode == D_BTMP_CODE_SNIP_2050384 || nDgnCode == D_BTMP_CODE_SP_35_13330_2011 || nDgnCode == D_BTMP_CODE_IRC6_2017 || bDgnCodeNew);
	GetDlgItem(IDC_TEMP_GRAD_TYPE2_RDO)->ShowWindow(nDgnCode == D_BTMP_CODE_SNIP_2050384 || nDgnCode == D_BTMP_CODE_SP_35_13330_2011 || nDgnCode == D_BTMP_CODE_IRC6_2017 || bDgnCodeNew);
	if (nDgnCode==D_BTMP_CODE_IRC6_2017)
	{
		GetDlgItem(IDC_TEMP_GRAD_GRP)->SetWindowText(_LS(IDS_TM_STLD_BTMP_DEF_CODE_PROV_TEMP_DIFF));
		GetDlgItem(IDC_TEMP_GRAD_TYPE1_RDO)->SetWindowText(_LS(IDS_TM_STLD_BTMP_DEF_CODE_PROV_TEMP_POSITIVE));
		GetDlgItem(IDC_TEMP_GRAD_TYPE2_RDO)->SetWindowText(_LS(IDS_TM_STLD_BTMP_DEF_CODE_PROV_TEMP_REVERSE));
	}
	else if (nDgnCode==D_BTMP_CODE_AASHTO_LRFD_2020)
	{
		GetDlgItem(IDC_TEMP_GRAD_GRP)->SetWindowText(_LS(IDS_TM_STLD_BTMP_DEF_CODE_PROV_TEMP_GRAD));
		GetDlgItem(IDC_TEMP_GRAD_TYPE1_RDO)->SetWindowText(_LS(IDS_TM_STLD_BTMP_DEF_CODE_PROV_TEMP_POSITIVE));
		GetDlgItem(IDC_TEMP_GRAD_TYPE2_RDO)->SetWindowText(_LS(IDS_TM_STLD_BTMP_DEF_CODE_PROV_TEMP_NEGATIVE));
	}
	else if (nDgnCode==D_BTMP_CODE_EUROCODE)
	{
		GetDlgItem(IDC_TEMP_GRAD_GRP)->SetWindowText(_LS(IDS_TM_STLD_BTMP_DEF_CODE_PROV_TEMP_GRAD));
		GetDlgItem(IDC_TEMP_GRAD_TYPE1_RDO)->SetWindowText(_LS(IDS_TM_STLD_BTMP_DEF_CODE_PROV_TEMP_HEATING));
		GetDlgItem(IDC_TEMP_GRAD_TYPE2_RDO)->SetWindowText(_LS(IDS_TM_STLD_BTMP_DEF_CODE_PROV_TEMP_COOLING));
	}
	else if (nDgnCode==D_BTMP_CODE_AS)
	{
		GetDlgItem(IDC_TEMP_GRAD_GRP)->SetWindowText(_LS(IDS_TM_STLD_BTMP_DEF_CODE_PROV_TEMP_GRAD));
		GetDlgItem(IDC_TEMP_GRAD_TYPE1_RDO)->SetWindowText(_LS(IDS_TM_STLD_BTMP_DEF_CODE_PROV_TEMP_POSITIVE));
		GetDlgItem(IDC_TEMP_GRAD_TYPE2_RDO)->SetWindowText(_LS(IDS_TM_STLD_BTMP_DEF_CODE_PROV_TEMP_NEGATIVE));
	}
	else
	{
		GetDlgItem(IDC_TEMP_GRAD_GRP)->SetWindowText(_LS(IDS_TM_STLD_BTMP_DEF_CODE_PROV_TEMP_GRAD));
		GetDlgItem(IDC_TEMP_GRAD_TYPE1_RDO)->SetWindowText(_LS(IDS_TM_STLD_BTMP_DEF_CODE_PROV_TEMP_TYPE1));
		GetDlgItem(IDC_TEMP_GRAD_TYPE2_RDO)->SetWindowText(_LS(IDS_TM_STLD_BTMP_DEF_CODE_PROV_TEMP_TYPE2));
	}


	CDlgUtil::CtrlShowHide(this, m_aControlsOrg   , nDgnCode == D_BTMP_CODE_SNIP_2050384 || nDgnCode == D_BTMP_CODE_SP_35_13330_2011 || nDgnCode == D_BTMP_CODE_JTG_D60_2015 || nDgnCode == D_BTMP_CODE_IRC6_2017);
	CDlgUtil::CtrlShowHide(this, m_aControlsAASHTO, nDgnCode == D_BTMP_CODE_AASHTO_LRFD_2020);
	CDlgUtil::CtrlShowHide(this, m_aControlsEuro  , nDgnCode == D_BTMP_CODE_EUROCODE);
	CDlgUtil::CtrlShowHide(this, m_aControlsAS    , nDgnCode == D_BTMP_CODE_AS);

	BOOL bDumbbell = (nDgnCode == D_BTMP_CODE_JTG_D60_2015 && nSectType == 2) ? TRUE : FALSE;

	GetDlgItem(IDC_JTG_T1_STC)->ShowWindow(nDgnCode == D_BTMP_CODE_JTG_D60_2015 && nSectType != 2);
	GetDlgItem(IDC_JTG_T1_EDT)->ShowWindow(nDgnCode == D_BTMP_CODE_JTG_D60_2015 && nSectType != 2);
	GetDlgItem(IDC_JTG_T1_UNT)->ShowWindow(nDgnCode == D_BTMP_CODE_JTG_D60_2015 && nSectType != 2);
	GetDlgItem(IDC_JTG_T2_STC)->ShowWindow(nDgnCode == D_BTMP_CODE_JTG_D60_2015 && nSectType != 2);
	GetDlgItem(IDC_JTG_T2_EDT)->ShowWindow(nDgnCode == D_BTMP_CODE_JTG_D60_2015 && nSectType != 2);
	GetDlgItem(IDC_JTG_T2_UNT)->ShowWindow(nDgnCode == D_BTMP_CODE_JTG_D60_2015 && nSectType != 2);

	GetDlgItem(IDC_JTG_DUMB_TYPE_GRP)->ShowWindow(bDumbbell);
	GetDlgItem(IDC_JTG_DUMB_TYPE_STUB_RDO)->ShowWindow(bDumbbell);
	GetDlgItem(IDC_JTG_DUMB_TYPE_DUMB_RDO)->ShowWindow(bDumbbell);
	GetDlgItem(IDC_JTG_DUMB_T1_STC)->ShowWindow(bDumbbell);
	GetDlgItem(IDC_JTG_DUMB_T1_EDT)->ShowWindow(bDumbbell);
	GetDlgItem(IDC_JTG_DUMB_T1_UNT)->ShowWindow(bDumbbell);
	GetDlgItem(IDC_JTG_DUMB_T2_STC)->ShowWindow(bDumbbell);
	GetDlgItem(IDC_JTG_DUMB_T2_EDT)->ShowWindow(bDumbbell);
	GetDlgItem(IDC_JTG_DUMB_T2_UNT)->ShowWindow(bDumbbell);

	if(nDgnCode == D_BTMP_CODE_SNIP_2050384 || nDgnCode == D_BTMP_CODE_SP_35_13330_2011 || nDgnCode == D_BTMP_CODE_JTG_D60_2015 || nDgnCode == D_BTMP_CODE_IRC6_2017)
	{
		GetDlgItem(IDC_TMAX_STC)->ShowWindow(nDgnCode == D_BTMP_CODE_SNIP_2050384 || nDgnCode == D_BTMP_CODE_SP_35_13330_2011);
		GetDlgItem(IDC_TMAX_EDT)->ShowWindow(nDgnCode == D_BTMP_CODE_SNIP_2050384 || nDgnCode == D_BTMP_CODE_SP_35_13330_2011);
		GetDlgItem(IDC_TMAX_UNT)->ShowWindow(nDgnCode == D_BTMP_CODE_SNIP_2050384 || nDgnCode == D_BTMP_CODE_SP_35_13330_2011);

		GetDlgItem(IDC_JTG_SMALL_T_STC)->ShowWindow(nDgnCode == D_BTMP_CODE_JTG_D60_2015 && nSectType == 1);
		GetDlgItem(IDC_JTG_SMALL_T_EDT)->ShowWindow(nDgnCode == D_BTMP_CODE_JTG_D60_2015 && nSectType == 1);
		GetDlgItem(IDC_JTG_SMALL_T_UNT)->ShowWindow(nDgnCode == D_BTMP_CODE_JTG_D60_2015 && nSectType == 1);
	}
	else if(nDgnCode == D_BTMP_CODE_AASHTO_LRFD_2020)
	{
    	GetDlgItem(IDC_AASHTO_T3_STC     )->ShowWindow(nSectType == 0);
		GetDlgItem(IDC_AASHTO_T3_EDT     )->ShowWindow(nSectType == 0);
		GetDlgItem(IDC_AASHTO_T3_UNT     )->ShowWindow(nSectType == 0);

		GetDlgItem(IDC_AASHTO_SMALL_T_STC     )->ShowWindow(nSectType == 1); // Stl Comp.		
		GetDlgItem(IDC_AASHTO_SMALL_T_EDT     )->ShowWindow(nSectType == 1);
		GetDlgItem(IDC_AASHTO_SMALL_T_UNT     )->ShowWindow(nSectType == 1);
		GetDlgItem(IDC_AASHTO_SMALL_T_AUTO_RDO)->ShowWindow(nSectType == 1);
		GetDlgItem(IDC_AASHTO_SMALL_T_USER_RDO)->ShowWindow(nSectType == 1);

		OnClickAASHTO_SmalT_Rdo();
		OnClickAASHTO_A_Rdo();
	}
	else if(nDgnCode == D_BTMP_CODE_EUROCODE)
	{
		//int nSectType;  // 0 : PSC, 1 : Steel Composite
		int nTempGrad=0;  // 0 : Heat 1 : Cool
		int nProcedure=0; // 0 : Normal, 1 : Simplified

		CDlgUtil::CtrlRadioGetCheck(this, m_aRdoSnipTempGrad, nTempGrad);
		CDlgUtil::CtrlRadioGetCheck(this, m_aRdoEuroPro     , nProcedure);

		if(nSectType == 0) // PSC
		{
			CDlgUtil::CtrlShowHide(this, m_aControlsEuro_Proc  , FALSE);
			CDlgUtil::CtrlShowHide(this, m_aControlsEuro_T1    , TRUE); 
			CDlgUtil::CtrlShowHide(this, m_aControlsEuro_T2    , TRUE);
			CDlgUtil::CtrlShowHide(this, m_aControlsEuro_T3    , TRUE);
			CDlgUtil::CtrlShowHide(this, m_aControlsEuro_T4    , nTempGrad == 1);
			CDlgUtil::CtrlShowHide(this, m_aControlsEuro_H0    , FALSE);
			CDlgUtil::CtrlShowHide(this, m_aControlsEuro_H1    , TRUE);
			CDlgUtil::CtrlShowHide(this, m_aControlsEuro_H2    , TRUE);
			CDlgUtil::CtrlShowHide(this, m_aControlsEuro_H3    , TRUE);
			CDlgUtil::CtrlShowHide(this, m_aControlsEuro_H4    , nTempGrad == 1);

			OnClickEuro_H0_Rdo();	
			OnClickEuro_H1_Rdo();	
			OnClickEuro_H2_Rdo();	
			OnClickEuro_H3_Rdo();	
			OnClickEuro_H4_Rdo();	
		}
		else if(nSectType == 1) // Comp
		{
			CDlgUtil::CtrlShowHide(this, m_aControlsEuro_Proc  , TRUE);
			CDlgUtil::CtrlShowHide(this, m_aControlsEuro_T1    , TRUE);
			CDlgUtil::CtrlShowHide(this, m_aControlsEuro_T2    , nProcedure== 0);
			CDlgUtil::CtrlShowHide(this, m_aControlsEuro_T3    , FALSE);
			CDlgUtil::CtrlShowHide(this, m_aControlsEuro_T4    , FALSE);
			CDlgUtil::CtrlShowHide(this, m_aControlsEuro_H0    , TRUE);
			CDlgUtil::CtrlShowHide(this, m_aControlsEuro_H1    , nProcedure== 0);
			CDlgUtil::CtrlShowHide(this, m_aControlsEuro_H2    , nProcedure== 0);
			CDlgUtil::CtrlShowHide(this, m_aControlsEuro_H3    , FALSE);
			CDlgUtil::CtrlShowHide(this, m_aControlsEuro_H4    , FALSE);

			OnClickEuro_H0_Rdo();	
			OnClickEuro_H1_Rdo();	
			OnClickEuro_H2_Rdo();	
		}
		else 
			ASSERT(0);
	}
	else if(nDgnCode == D_BTMP_CODE_AS)
	{
		//int nSectType;  // 0 : PSC, 1 : Steel Composite
		int nTempGrad=0;  // 0 : Heat 1 : Cool

		CDlgUtil::CtrlRadioGetCheck(this, m_aRdoSnipTempGrad, nTempGrad);

		if(nSectType == 0) // PSC
		{
			CDlgUtil::CtrlShowHide(this, m_aControlsAS_T1      , TRUE); 
			CDlgUtil::CtrlShowHide(this, m_aControlsAS_T2      , nTempGrad==0);
			CDlgUtil::CtrlShowHide(this, m_aControlsAS_Small_T , FALSE);
			CDlgUtil::CtrlShowHide(this, m_aControlsAS_H1      , nTempGrad==1);
			CDlgUtil::CtrlShowHide(this, m_aControlsAS_H2      , nTempGrad==0);

		    //OnClickAS_SmalT_Rdo();
			//OnClickAS_H1_Rdo();	
			//OnClickAS_H2_Rdo();	
		}
		else if (nSectType == 1) // Steel Comp
		{
			CDlgUtil::CtrlShowHide(this, m_aControlsAS_T1, TRUE);
			CDlgUtil::CtrlShowHide(this, m_aControlsAS_T2, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aControlsAS_Small_T, TRUE);
			CDlgUtil::CtrlShowHide(this, m_aControlsAS_H1, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aControlsAS_H2, FALSE);

			OnClickAS_SmalT_Rdo();
			//OnClickAS_H1_Rdo();	
			//OnClickAS_H2_Rdo();	
		}
		else 
			ASSERT(0);
	}						
}

void CStldBtmpDefCodeProvDlg::AlignCtrl()
{
	CRect rRef, rMove;
	int nDistX, nDistY;
	CArray<UINT, UINT> aCtrl;

	int nDgnCode = CDlgUtil::CobxGetCurSelItemData(m_cbxDefCode, m_cbxDefCode.GetCurSel());
	int nSectType = m_cbxSectType.GetCurSel();

	if (nDgnCode == D_BTMP_CODE_JTG_D60_2015 && nSectType==2)
	{
		aCtrl.RemoveAll();
		aCtrl.Add(IDC_JTG_DUMB_TYPE_GRP     );
		aCtrl.Add(IDC_JTG_DUMB_TYPE_STUB_RDO);
		aCtrl.Add(IDC_JTG_DUMB_TYPE_DUMB_RDO);
		aCtrl.Add(IDC_JTG_DUMB_T1_STC       );
		aCtrl.Add(IDC_JTG_DUMB_T1_EDT       );
		aCtrl.Add(IDC_JTG_DUMB_T1_UNT       );
		aCtrl.Add(IDC_JTG_DUMB_T2_STC       );
		aCtrl.Add(IDC_JTG_DUMB_T2_EDT       );
		aCtrl.Add(IDC_JTG_DUMB_T2_UNT       );

		GetDlgItem(IDC_TEMP_GRAD_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_JTG_DUMB_TYPE_GRP)->GetWindowRect(rMove);
		nDistX = rRef.left - rMove.left;
		nDistY = rRef.top - rMove.top;
		CDlgUtil::CtrlMoveDistXY(this, aCtrl, nDistX, nDistY);
	}
	else if(nDgnCode == D_BTMP_CODE_SNIP_2050384 || nDgnCode == D_BTMP_CODE_SP_35_13330_2011 || nDgnCode == D_BTMP_CODE_IRC6_2017)
	{
		aCtrl.RemoveAll();
		aCtrl.Add(IDC_JTG_T1_STC);
		aCtrl.Add(IDC_JTG_T1_EDT);
		aCtrl.Add(IDC_JTG_T1_UNT);
		aCtrl.Add(IDC_JTG_T2_STC);
		aCtrl.Add(IDC_JTG_T2_EDT);
		aCtrl.Add(IDC_JTG_T2_UNT);
		aCtrl.Add(IDC_JTG_SMALL_T_STC);
		aCtrl.Add(IDC_JTG_SMALL_T_EDT);
		aCtrl.Add(IDC_JTG_SMALL_T_UNT);

		GetDlgItem(IDC_TEMP_GRAD_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_JTG_T1_STC)->GetWindowRect(rMove);
		nDistY = rRef.top-rMove.top+globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistY(this, aCtrl, nDistY);
	}
	else if(nDgnCode == D_BTMP_CODE_AASHTO_LRFD_2020)
	{
		GetDlgItem(IDC_TMAX_EDT     )->GetWindowRect(rRef);
		GetDlgItem(IDC_AASHTO_T1_EDT)->GetWindowRect(rMove);
		nDistY = rRef.top - rMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aControlsAASHTO, nDistY);

		GetDlgItem(IDC_AASHTO_T1_EDT)->GetWindowRect(rRef);
		GetDlgItem(IDC_AASHTO_T3_EDT)->GetWindowRect(rMove);
		int nGap = rRef.top - rMove.top;

		if(nSectType==0) // psc
		{
			// t  => 항상 숨김
			aCtrl.RemoveAll();
			aCtrl.Add(IDC_AASHTO_SMALL_T_STC);
			aCtrl.Add(IDC_AASHTO_SMALL_T_EDT);
			aCtrl.Add(IDC_AASHTO_SMALL_T_UNT);
			aCtrl.Add(IDC_AASHTO_SMALL_T_AUTO_RDO);
			aCtrl.Add(IDC_AASHTO_SMALL_T_USER_RDO);

			GetDlgItem(IDC_AASHTO_T3_EDT)->GetWindowRect(rRef);
			GetDlgItem(IDC_AASHTO_SMALL_T_EDT)->GetWindowRect(rMove);
			nDistY = rRef.top - rMove.top - nGap;
			CDlgUtil::CtrlMoveDistY(this, aCtrl, nDistY);

			// A
			aCtrl.RemoveAll();
			aCtrl.Add(IDC_AASHTO_A_STC);
			aCtrl.Add(IDC_AASHTO_A_EDT);
			aCtrl.Add(IDC_AASHTO_A_UNT);
			aCtrl.Add(IDC_AASHTO_A_AUTO_RDO);
			aCtrl.Add(IDC_AASHTO_A_USER_RDO);

			GetDlgItem(IDC_AASHTO_SMALL_T_EDT)->GetWindowRect(rRef);
			GetDlgItem(IDC_AASHTO_A_EDT )->GetWindowRect(rMove);
			nDistY = rRef.top - rMove.top - 0;
			CDlgUtil::CtrlMoveDistY(this, aCtrl, nDistY);
		}
		else // Stl Comp.
		{
			// t
			aCtrl.RemoveAll();
			aCtrl.Add(IDC_AASHTO_SMALL_T_STC);
			aCtrl.Add(IDC_AASHTO_SMALL_T_EDT);
			aCtrl.Add(IDC_AASHTO_SMALL_T_UNT);
			aCtrl.Add(IDC_AASHTO_SMALL_T_AUTO_RDO);
			aCtrl.Add(IDC_AASHTO_SMALL_T_USER_RDO);

			GetDlgItem(IDC_AASHTO_T2_EDT     )->GetWindowRect(rRef);
			GetDlgItem(IDC_AASHTO_SMALL_T_EDT)->GetWindowRect(rMove);
			nDistY = rRef.top - rMove.top - nGap;
			CDlgUtil::CtrlMoveDistY(this, aCtrl, nDistY);

			// A
			aCtrl.RemoveAll();
			aCtrl.Add(IDC_AASHTO_A_STC);
			aCtrl.Add(IDC_AASHTO_A_EDT);
			aCtrl.Add(IDC_AASHTO_A_UNT);
			aCtrl.Add(IDC_AASHTO_A_AUTO_RDO);
			aCtrl.Add(IDC_AASHTO_A_USER_RDO);

			GetDlgItem(IDC_AASHTO_SMALL_T_EDT)->GetWindowRect(rRef);
			GetDlgItem(IDC_AASHTO_A_EDT      )->GetWindowRect(rMove);
			nDistY = rRef.top - rMove.top - nGap;
			CDlgUtil::CtrlMoveDistY(this, aCtrl, nDistY);

		}
	}
	else if(nDgnCode == D_BTMP_CODE_EUROCODE)
	{
		GetDlgItem(IDC_TEMP_GRAD_GRP     )->GetWindowRect(rRef);
		GetDlgItem(IDC_EURO_PROCEDURE_GRP)->GetWindowRect(rMove);
		nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(5);
		CDlgUtil::CtrlMoveDistY(this, m_aControlsEuro, nDistY);

		GetDlgItem(IDC_EURO_T1_EDT)->GetWindowRect(rRef);
		GetDlgItem(IDC_EURO_T3_EDT)->GetWindowRect(rMove);
		int nGap = rRef.top - rMove.top;

		//int nSectType;  // 0 : PSC, 1 : Steel Composite
		int nTempGrad =m_Data.EuroCode.nTempGrad;  // 0 : Heat 1 : Cool
		int nProcedure=m_Data.EuroCode.nProcedure; // 0 : Normal, 1 : Simplified

		if(nSectType==1)  // Comp.
		{
			if(nTempGrad==0 || nTempGrad==1)
			{
				if(nProcedure==0)
				{
					GetDlgItem(IDC_EURO_PROCEDURE_GRP)->GetWindowRect(rRef);
					GetDlgItem(IDC_EURO_T1_EDT       )->GetWindowRect(rMove);
					nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(5);
					CDlgUtil::CtrlMoveDistY(this, m_aControlsEuro_T1, nDistY);
					CDlgUtil::CtrlMoveDistY(this, m_aControlsEuro_T2, nDistY);
					CDlgUtil::CtrlMoveDistY(this, m_aControlsEuro_T3, nDistY);
					CDlgUtil::CtrlMoveDistY(this, m_aControlsEuro_T4, nDistY);

					GetDlgItem(IDC_EURO_T1_EDT     )->GetWindowRect(rRef);
					GetDlgItem(IDC_EURO_SMALL_H_EDT)->GetWindowRect(rMove);
					nDistY = rRef.top - rMove.top - nGap;
					CDlgUtil::CtrlMoveDistY(this, m_aControlsEuro_H0, nDistY);

					GetDlgItem(IDC_EURO_SMALL_H_EDT )->GetWindowRect(rRef);
					GetDlgItem(IDC_EURO_SMALL_H1_EDT)->GetWindowRect(rMove);
					nDistY = rRef.top - rMove.top - nGap;
					CDlgUtil::CtrlMoveDistY(this, m_aControlsEuro_H1, nDistY);

					GetDlgItem(IDC_EURO_SMALL_H1_EDT )->GetWindowRect(rRef);
					GetDlgItem(IDC_EURO_SMALL_H2_EDT)->GetWindowRect(rMove);
					nDistY = rRef.top - rMove.top - nGap;
					CDlgUtil::CtrlMoveDistY(this, m_aControlsEuro_H2, nDistY);
				}
				else
				{
					GetDlgItem(IDC_EURO_PROCEDURE_GRP)->GetWindowRect(rRef);
					GetDlgItem(IDC_EURO_T1_EDT       )->GetWindowRect(rMove);
					nDistY = rRef.bottom - rMove.top + globalUtils.ScaleByDPI(5);
					CDlgUtil::CtrlMoveDistY(this, m_aControlsEuro_T1, nDistY);
					CDlgUtil::CtrlMoveDistY(this, m_aControlsEuro_T2, nDistY);
					CDlgUtil::CtrlMoveDistY(this, m_aControlsEuro_T3, nDistY);
					CDlgUtil::CtrlMoveDistY(this, m_aControlsEuro_T4, nDistY);

					GetDlgItem(IDC_EURO_T1_EDT     )->GetWindowRect(rRef);
					GetDlgItem(IDC_EURO_SMALL_H_EDT)->GetWindowRect(rMove);
					nDistY = rRef.top - rMove.top - nGap;
					CDlgUtil::CtrlMoveDistY(this, m_aControlsEuro_H0, nDistY);
				}
			}
		}
		else if(nSectType==0) //PSC
		{
			GetDlgItem(IDC_EURO_PROCEDURE_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_EURO_T1_EDT       )->GetWindowRect(rMove);
			nDistY = rRef.top - rMove.top;
			CDlgUtil::CtrlMoveDistY(this, m_aControlsEuro_T1, nDistY);
			CDlgUtil::CtrlMoveDistY(this, m_aControlsEuro_T2, nDistY);
			CDlgUtil::CtrlMoveDistY(this, m_aControlsEuro_T3, nDistY);
			CDlgUtil::CtrlMoveDistY(this, m_aControlsEuro_T4, nDistY);

			GetDlgItem(IDC_EURO_T3_EDT      )->GetWindowRect(rRef);
			GetDlgItem(IDC_EURO_SMALL_H1_EDT)->GetWindowRect(rMove);
			nDistY = rRef.top - rMove.top - nGap;
			CDlgUtil::CtrlMoveDistY(this, m_aControlsEuro_H1, nDistY);

// 			GetDlgItem(IDC_EURO_SMALL_H_EDT )->GetWindowRect(rRef);
// 			GetDlgItem(IDC_EURO_SMALL_H1_EDT)->GetWindowRect(rMove);
// 			nDistY = rRef.top - rMove.top - nGap;
// 			CDlgUtil::CtrlMoveDistY(this, m_aControlsEuro_H1, nDistY);

			GetDlgItem(IDC_EURO_SMALL_H1_EDT )->GetWindowRect(rRef);
			GetDlgItem(IDC_EURO_SMALL_H2_EDT)->GetWindowRect(rMove);
			nDistY = rRef.top - rMove.top - nGap;
			CDlgUtil::CtrlMoveDistY(this, m_aControlsEuro_H2, nDistY);

			GetDlgItem(IDC_EURO_SMALL_H2_EDT )->GetWindowRect(rRef);
			GetDlgItem(IDC_EURO_SMALL_H3_EDT)->GetWindowRect(rMove);
			nDistY = rRef.top - rMove.top - nGap;
			CDlgUtil::CtrlMoveDistY(this, m_aControlsEuro_H3, nDistY);

			GetDlgItem(IDC_EURO_SMALL_H3_EDT )->GetWindowRect(rRef);
			GetDlgItem(IDC_EURO_SMALL_H4_EDT)->GetWindowRect(rMove);
			nDistY = rRef.top - rMove.top - nGap;
			CDlgUtil::CtrlMoveDistY(this, m_aControlsEuro_H4, nDistY);
		}
		else
			ASSERT(0);

	}
	else if(nDgnCode == D_BTMP_CODE_AS)
	{
		//int nSectType;  // 0 : PSC, 1 : Steel Composite
		int nTempGrad =m_Data.AsCode.nTempGrad;  // 0 : Heat 1 : Cool

		GetDlgItem(IDC_TMAX_EDT )->GetWindowRect(rRef);
		GetDlgItem(IDC_AS_T1_EDT)->GetWindowRect(rMove);
		nDistY = rRef.top - rMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aControlsAS, nDistY);

		GetDlgItem(IDC_AS_SMALL_H1_EDT)->GetWindowRect(rRef);
		GetDlgItem(IDC_AS_SMALL_H2_EDT)->GetWindowRect(rMove);
		int nGap = rRef.top - rMove.top;

		if(nSectType==0) // PSC
		{
			GetDlgItem(IDC_AS_T1_EDT)->GetWindowRect(rRef);
			GetDlgItem(IDC_AS_SMALL_H1_EDT)->GetWindowRect(rMove);
			nDistY = rRef.top - rMove.top - nGap;
			CDlgUtil::CtrlMoveDistY(this, m_aControlsAS_H1, nDistY);
			CDlgUtil::CtrlMoveDistY(this, m_aControlsAS_H2, nDistY);

			if(nTempGrad == 0)
			{
				GetDlgItem(IDC_AS_SMALL_H1_EDT)->GetWindowRect(rRef);
				GetDlgItem(IDC_AS_SMALL_H2_EDT)->GetWindowRect(rMove);
				nDistY = rRef.top - rMove.top;
				CDlgUtil::CtrlMoveDistY(this, m_aControlsAS_H1, nDistY);
				CDlgUtil::CtrlMoveDistY(this, m_aControlsAS_H2, nDistY);
			}
// 			else 
// 			{
// 				GetDlgItem(IDC_AS_SMALL_H1_EDT)->GetWindowRect(rRef);
// 				GetDlgItem(IDC_AS_SMALL_H2_EDT)->GetWindowRect(rMove);
// 				nDistY = rRef.top - rMove.top - nGap;
// 				CDlgUtil::CtrlMoveDistY(this, m_aControlsAS_H1, nDistY);
// 				CDlgUtil::CtrlMoveDistY(this, m_aControlsAS_H2, nDistY);
// 			}
		}
		else if (nSectType == 1) // Stl Comp.
		{
			GetDlgItem(IDC_AS_T1_EDT)->GetWindowRect(rRef);
			GetDlgItem(IDC_AS_SMALL_T_EDT)->GetWindowRect(rMove);
			nDistY = rRef.top - rMove.top - nGap;
			CDlgUtil::CtrlMoveDistY(this, m_aControlsAS_Small_T, nDistY);
		}

	}

	// 컨트롤의 잔상이 남는 문제로 인해 아래 함수 적용!
	CRect pRect;
	GetClientRect( &pRect );
	InvalidateRect( &pRect, TRUE );

}

void CStldBtmpDefCodeProvDlg::ResizeDialog()
{
	UINT nLastCtrlID = IDCANCEL;
	CWnd* pWndLast = GetDlgItem(nLastCtrlID);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(8);

	MoveWindow(r);
}

void CStldBtmpDefCodeProvDlg::ChangeBitmap()
{	
	CString strSVG;
	int nSectType = m_cbxSectType.GetCurSel();
	int nDgnCode = CDlgUtil::CobxGetCurSelItemData(m_cbxDefCode, m_cbxDefCode.GetCurSel());

	switch (nDgnCode)
	{
	case D_BTMP_CODE_SNIP_2050384:
	case D_BTMP_CODE_SP_35_13330_2011:
		strSVG = _T("SVG\\Illustration\\Dialog\\DefCodeProvSnip.svg");//IDB_TM_BTMP_DEF_CODE_PROV_SNIP;
		break;
	case D_BTMP_CODE_JTG_D60_2015:
#ifdef _CH
		if(nSectType == 0)
		{
			strSVG = _T("SVG\\Illustration\\Dialog\\DefCodeProvJTGD60_2015_PSC.svg");//IDB_TM_BTMP_DEF_CODE_PROV_JTGD60_2015_PSC;
		}
		else if(nSectType == 1)
		{
			strSVG = _T("SVG\\Illustration\\Dialog\\DefCodeProvJTGD60_2015_COMP.svg");//IDB_TM_BTMP_DEF_CODE_PROV_JTGD60_2015_COMP;
		}
		else if (nSectType == 2)
		{
			if (m_Data.Jtg2015_2018.nDumbType == 0)
				strSVG = _T("SVG\\Illustration\\Dialog\\DefCodeProvJTGD60_2015_STUB.svg");
			else if (m_Data.Jtg2015_2018.nDumbType == 1)
				strSVG = _T("SVG\\Illustration\\Dialog\\DefCodeProvJTGD60_2015_DUMB.svg");
			else
				ASSERT(0);
		}
		else
		{
			ASSERT(0);
		}
#else
		if(nSectType == 0)
		{
			strSVG = _T("SVG\\Illustration\\Dialog\\DefCodeProvJTGD60_2015_PSC_US.svg");//IDB_TM_BTMP_DEF_CODE_PROV_JTGD60_2015_PSC_US;
		}
		else if(nSectType == 1)
		{
			strSVG = _T("SVG\\Illustration\\Dialog\\DefCodeProvJTGD60_2015_COMP_US.svg");//IDB_TM_BTMP_DEF_CODE_PROV_JTGD60_2015_COMP_US;
		}
		else if (nSectType == 2)
		{
			if (m_Data.Jtg2015_2018.nDumbType == 0)
				strSVG = _T("SVG\\Illustration\\Dialog\\DefCodeProvJTGD60_2015_STUB.svg");
			else if (m_Data.Jtg2015_2018.nDumbType == 1)
				strSVG = _T("SVG\\Illustration\\Dialog\\DefCodeProvJTGD60_2015_DUMB.svg");
			else
				ASSERT(0);
		}
		else
		{
			ASSERT(0);
		}
#endif // _CH
		break;
    case D_BTMP_CODE_IRC6_2017:
        if(nSectType == 0)
        {
			strSVG = _T("SVG\\Illustration\\Dialog\\bitmap14.svg");//IDB_TM_BTMP_DEF_CODE_PROV_IRC6_2017_PSC;
        }
        else if(nSectType == 1)
        {
			strSVG = _T("SVG\\Illustration\\Dialog\\bitmap15.svg");//IDB_TM_BTMP_DEF_CODE_PROV_IRC6_2017_STLCOMP;
        }
        else
        {
            ASSERT(0);
        }
        break;
	case D_BTMP_CODE_AASHTO_LRFD_2020:
		{
			int nTempGrad=0;
			CDlgUtil::CtrlRadioGetCheck(this, m_aRdoSnipTempGrad, nTempGrad);
			if(nSectType == 0) // PSC
			{
				if     (nTempGrad==0) strSVG = _T("SVG\\Illustration\\Dialog\\DefCodeProvAASHTO_PSC_Positive.svg");//IDB_TM_BTMP_DEF_CODE_PROV_AASHTO_PSC_POSITIVE;  // Positive
				else if(nTempGrad==1) strSVG = _T("SVG\\Illustration\\Dialog\\DefCodeProvAASHTO_PSC_Negative.svg");//IDB_TM_BTMP_DEF_CODE_PROV_AASHTO_PSC_NEGATIVE;  // Negative
				else ASSERT(0);
			}
			else if(nSectType == 1) // Comp.
			{
				if     (nTempGrad==0) strSVG = _T("SVG\\Illustration\\Dialog\\DefCodeProvAASHTO_Com_Positive.svg");//IDB_TM_BTMP_DEF_CODE_PROV_AASHTO_COM_POSITIVE;  // Positive
				else if(nTempGrad==1) strSVG = _T("SVG\\Illustration\\Dialog\\DefCodeProvAASHTO_Com_Negative.svg");//IDB_TM_BTMP_DEF_CODE_PROV_AASHTO_COM_NEGATIVE;  // Negative
				else ASSERT(0);
			}
			else ASSERT(0);

			break;
		}
	case D_BTMP_CODE_EUROCODE:
		{
			int nTempGrad=0;
			CDlgUtil::CtrlRadioGetCheck(this, m_aRdoSnipTempGrad, nTempGrad);
			int nProcedure=0;
			CDlgUtil::CtrlRadioGetCheck(this, m_aRdoEuroPro, nProcedure);

			if(nSectType == 0) // PSC
			{
				if     (nTempGrad==0) strSVG = _T("SVG\\Illustration\\Dialog\\DefCodeProvEuro_PSC_Heat.svg");//IDB_TM_BTMP_DEF_CODE_PROV_EURO_PSC_HEAT;  // Heat
				else if(nTempGrad==1) strSVG = _T("SVG\\Illustration\\Dialog\\DefCodeProvEuro_PSC_Cool.svg");//IDB_TM_BTMP_DEF_CODE_PROV_EURO_PSC_COOL;  // Cool
				else ASSERT(0);
			}
			else if(nSectType == 1) // Comp.
			{
				if(nTempGrad==0)  // Heat
				{
					if     (nProcedure==0) strSVG = _T("SVG\\Illustration\\Dialog\\DefCodeProvEuro_Com_Nomal_Heat.svg");//IDB_TM_BTMP_DEF_CODE_PROV_EURO_COM_NOMAL_HEAT;  // Normal
					else if(nProcedure==1) strSVG = _T("SVG\\Illustration\\Dialog\\DefCodeProvEuro_Com_Simple_Heat.svg");//IDB_TM_BTMP_DEF_CODE_PROV_EURO_COM_SIMPLE_HEAT; // Simple
					else ASSERT(0);
				}
				else if(nTempGrad==1)  // Cool
				{
					if     (nProcedure==0) strSVG = _T("SVG\\Illustration\\Dialog\\DefCodeProvEuro_Com_Nomal_Cool.svg");//IDB_TM_BTMP_DEF_CODE_PROV_EURO_COM_NOMAL_COOL;  // Normal
					else if(nProcedure==1) strSVG = _T("SVG\\Illustration\\Dialog\\DefCodeProvEuro_Com_Simple_Cool.svg");//IDB_TM_BTMP_DEF_CODE_PROV_EURO_COM_SIMPLE_COOL; // Simple
					else ASSERT(0);
				}
				else ASSERT(0);
			}
			else ASSERT(0);

			break;
		}
	case D_BTMP_CODE_AS:
		{
			int nTempGrad = 0;
			CDlgUtil::CtrlRadioGetCheck(this, m_aRdoSnipTempGrad, nTempGrad);

			if (nSectType == 0)  // PSC
			{
				//nResID = IDB_TM_BTMP_DEF_CODE_PROV_AS_PSC_BASIC;

				if      (nTempGrad == 0)  strSVG = _T("SVG\\Illustration\\Dialog\\DefCodeProvAS_PSC_Basic_Positive.svg");//IDB_TM_BTMP_DEF_CODE_PROV_AS_PSC_BASIC_POSITIVE; // Positive
				else if (nTempGrad == 1)  strSVG = _T("SVG\\Illustration\\Dialog\\DefCodeProvAS_PSC_Basic_Negative.svg");//IDB_TM_BTMP_DEF_CODE_PROV_AS_PSC_BASIC_NEGATIVE; // Negative
			}
			else if (nSectType == 1) // Stl Comp.
			{
				//nResID = IDB_TM_BTMP_DEF_CODE_PROV_AS_COM_I;

				if      (nTempGrad == 0)  strSVG = _T("SVG\\Illustration\\Dialog\\DefCodeProvAS_Com_Basic_Positive.svg");//IDB_TM_BTMP_DEF_CODE_PROV_AS_COM_BASIC_POSITIVE; // Positive
				else if (nTempGrad == 1)  strSVG = _T("SVG\\Illustration\\Dialog\\DefCodeProvAS_Com_Basic_Negative.svg");//IDB_TM_BTMP_DEF_CODE_PROV_AS_COM_BASIC_NEGATIVE; // Negative
			}
			else
			{
				ASSERT(0);
			}
			break;
		}
	default:
		break;
	}   

	m_mvPicture.SetImage(strSVG);
}