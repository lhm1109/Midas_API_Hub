// 
// DgnCRCAnnexDgnParameter_TMH7_Dlg.cpp : implementation file
// 
#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCRCAnnexDgnParameter_TMH7_Dlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnCRCAnnexDgnParameter_TMH7_Dlgdialog
CDgnCRCAnnexDgnParameter_TMH7_Dlg::CDgnCRCAnnexDgnParameter_TMH7_Dlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnCRCAnnexDgnParameter_TMH7_Dlg::IDD, pParent)
{
	m_bIncrevtu = FALSE;

	m_pDoc = CDBDoc::GetDocPoint();
}

CDgnCRCAnnexDgnParameter_TMH7_Dlg::~CDgnCRCAnnexDgnParameter_TMH7_Dlg()
{

}

void CDgnCRCAnnexDgnParameter_TMH7_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCRCAnnexDgnParameter_TMH7_Dlg)	
	DDX_Control(pDX, IDC_DGN_STR_REBAR_TNDN_EDT, m_edtCsTndn);
	DDX_Control(pDX, IDC_DGN_STR_CONC_EDT,       m_edtCsConc);
	DDX_Control(pDX, IDC_DGN_TRI_COMP_STRS_EDT,  m_edtSfTriCmp);
	DDX_Control(pDX, IDC_DGN_UNI_COMP_STRS_EDT,  m_edtSfUniCmp);
	DDX_Control(pDX, IDC_DGN_PRE_TENS_EDT,       m_edtSfPreTens);
	DDX_Control(pDX, IDC_DGN_POST_TENS_EDT,      m_edtSfPostTens);
	DDX_Control(pDX, IDC_DGN_POST_TENS_EDT2,      m_edtSfPostTens2);
	DDX_Check  (pDX, IDC_DGN_INCRE_VTU_CHK,      m_bIncrevtu);
	DDX_Control(pDX, IDC_DGN_PRE_TENS_MEMB_EDT,  m_edtPreTensMemb);
	DDX_Control(pDX, IDC_DGN_PRE_TENS_MEMB_UNT,  m_untPreTensMemb);
	DDX_Control(pDX, IDC_DGN_POST_TENS_MEMB_EDT, m_edtPostTensMemb);
	DDX_Control(pDX, IDC_DGN_POST_TENS_MEMB_UNT, m_untPostTensMemb);
	//}}AFX_DATA_MAP
}

void CDgnCRCAnnexDgnParameter_TMH7_Dlg::InitUnit()
{
	m_edtCsTndn.SetUnitType(D_UNITSYS_NONE);
	m_edtCsConc.SetUnitType(D_UNITSYS_NONE);
	m_edtSfTriCmp.SetUnitType(D_UNITSYS_NONE);
	m_edtSfUniCmp.SetUnitType(D_UNITSYS_NONE);
	m_edtSfPreTens.SetUnitType(D_UNITSYS_NONE);
	m_edtSfPostTens.SetUnitType(D_UNITSYS_NONE);
	m_edtSfPostTens2.SetUnitType(D_UNITSYS_NONE);
	m_edtPreTensMemb.SetUnitType(CUnitCtrl::m_PSCD_UNIT.dTsPreTens);
	m_untPreTensMemb.SetUnitType(CUnitCtrl::m_PSCD_UNIT.dTsPreTens);
	m_edtPostTensMemb.SetUnitType(CUnitCtrl::m_PSCD_UNIT.dTsPostTens);
	m_untPostTensMemb.SetUnitType(CUnitCtrl::m_PSCD_UNIT.dTsPostTens);
}

void CDgnCRCAnnexDgnParameter_TMH7_Dlg::Data2Dlg()
{
	m_edtCsTndn.SetEditUnit(m_pParamData->dCsTndn);
	m_edtCsConc.SetEditUnit(m_pParamData->dCsConcrete);

	m_edtSfTriCmp.SetEditUnit(1.25);
	m_edtSfUniCmp.SetEditUnit(1.67);
	m_edtSfPreTens.SetEditUnit(1.25);
	m_edtSfPostTens.SetEditUnit(1.55);
	m_edtSfPostTens2.SetEditUnit(m_pParamData->dmb9);

	m_bIncrevtu = m_pParamData->bIncrevtu;
	m_edtPreTensMemb.SetEditUnit(m_pParamData->dTsPreTens);
	m_edtPostTensMemb.SetEditUnit(m_pParamData->dTsPostTens);

	UpdateData(FALSE);
}

BOOL CDgnCRCAnnexDgnParameter_TMH7_Dlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_pParamData->dCsTndn = m_edtCsTndn.GetEditValue();
	m_pParamData->dCsConcrete = m_edtCsConc.GetEditValue();

    m_pParamData->dSfTriCmp = 1.25;
    m_pParamData->dSfUniformCmp = 1.67;
    m_pParamData->dSfPreTens = 1.25;
    m_pParamData->dSfPostTens = 1.55;
    m_pParamData->dAlphacc4Shear = 0.7;

	m_pParamData->bIncrevtu = m_bIncrevtu;
	m_pParamData->dTsPreTens = m_edtPreTensMemb.GetEditValue();
	m_pParamData->dTsPostTens = m_edtPostTensMemb.GetEditValue();
	m_pParamData->dmb9 = m_edtSfPostTens2.GetEditValue();
	return TRUE;
}


BEGIN_MESSAGE_MAP(CDgnCRCAnnexDgnParameter_TMH7_Dlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnCRCAnnexDgnParameter_TMH7_Dlg)
	ON_BN_CLICKED(IDC_DGN_UPDATE_BY_CODE_BTN, OnResetDgnParam_byAnnex)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDgnCRCAnnexDgnParameter_TMH7_Dlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	InitUnit();

	if (!m_pDoc->m_pAttrCtrl->ExistPscd())
	{
		m_pParamData->dTsPreTens = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, 1.0);
		m_pParamData->dTsPostTens = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, 1.0);
	}

	Data2Dlg();

    EnableDisableControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnCRCAnnexDgnParameter_TMH7_Dlg::OnOK()
{
	
	if(!Dlg2Data())
	{
		return;
	}

	CDialogMove::OnOK();
}

void CDgnCRCAnnexDgnParameter_TMH7_Dlg::OnCancel()
{
	CDialogMove::OnCancel();
}

void CDgnCRCAnnexDgnParameter_TMH7_Dlg::OnResetDgnParam_byAnnex()
{
	m_pParamData->dCsTndn = 1.15;
	m_pParamData->dCsConcrete = 1.5;
	m_pParamData->dSfTriCmp = 1.25;
	m_pParamData->dSfUniformCmp = 1.67;
	m_pParamData->dSfPreTens = 1.25;
	m_pParamData->dSfPostTens = 1.55;
	m_pParamData->dAlphacc4Shear = 0.7;
	m_pParamData->bIncrevtu = FALSE;
	m_pParamData->dTsPreTens = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, 1.0);
	m_pParamData->dTsPostTens = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, 1.0);
	m_pParamData->dmb9 = 0.7;
	Data2Dlg();
}

void CDgnCRCAnnexDgnParameter_TMH7_Dlg::EnableDisableControls()
{
    UpdateData(TRUE);

    GetDlgItem(IDC_DGN_TRI_COMP_STRS_EDT)->EnableWindow(FALSE);
    GetDlgItem(IDC_DGN_UNI_COMP_STRS_EDT)->EnableWindow(FALSE);
    GetDlgItem(IDC_DGN_PRE_TENS_EDT)->EnableWindow(FALSE);
    GetDlgItem(IDC_DGN_POST_TENS_EDT)->EnableWindow(FALSE);
   // GetDlgItem(IDC_DGN_POST_TENS_EDT2)->EnableWindow(FALSE); Big Fixing R1
}
