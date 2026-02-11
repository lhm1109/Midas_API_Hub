// SeisCvlEvalParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCvlEvalParamDlg.h"

#include "SeisCvlForceDirDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\ETC_DESIGN_SPECT_TS.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlEvalParamDlg dialog


CSeisCvlEvalParamDlg::CSeisCvlEvalParamDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSeisCvlEvalParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisCvlEvalParamDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nOption = 0;
	m_nBHMethod = 0;

	m_pDoc = CDBDoc::GetDocPoint();
}


void CSeisCvlEvalParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisCvlEvalParamDlg)
	DDX_Radio  (pDX, IDC_TM_SEIS_CVL_MISC_ADD_RDO,              m_nOption);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_EVAL_GRP_CMB,         m_cmbEvalGrup);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_EVAL_GRP_COLM_CMB,    m_cmbEvalColm);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_L_BEHAV_CMB,          m_cmbBehavL);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_L_HE1_EDT,            m_edtHe1L);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_L_HE1_UNT,            m_untHe1L);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_L_HE2_EDT,            m_edtHe2L);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_L_HE2_UNT,            m_untHe2L);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_T_BEHAV_CMB,          m_cmbBehavT);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_T_HE1_EDT,            m_edtHe1T);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_T_HE1_UNT,            m_untHe1T);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_T_HE2_EDT,            m_edtHe2T);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_T_HE2_UNT,            m_untHe2T);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_AE_RAT_EDT,           m_edtAeRat);
	DDX_Radio  (pDX, IDC_TM_SEIS_CVL_MISC_BH_METHOD_RDO1,       m_nBHMethod);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_SPLICE_CMB,           m_cmbSplice);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_LP_EDT,               m_edtLP);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_TS_EDT,               m_edtTs);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MISC_TS_UNT,               m_untTs);
	//}}AFX_DATA_MAP
}

void CSeisCvlEvalParamDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:       
		UpdateBuffer();		
		break;
	case D_UPDATE_BUFFER_BEFORE: 
		break;
	case D_UPDATE_BUFFER_AFTER:	 
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:	
		break;
	case D_UPDATE_SEL_ADD: 
	case D_UPDATE_SEL_DEL:
		break;
	default: ASSERT(TRUE);
	}
}

void CSeisCvlEvalParamDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount==0) return; 
	ASSERT(nCount==1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;

		switch(nCmd)
		{
		case(UR_EVGP_ADD):
		case(UR_EVGP_DEL):
		case(UR_EVGP_MFD):
		case(UR_EVGP_MFS):
			InitCombo_EvgpGrup();
			InitCombo_EvgpGrup();
			EnableDisableControls();
			break;
		case(UR_SECT_ADD):
		case(UR_SECT_DEL):
		case(UR_SECT_MFD):
		case(UR_SECT_MFS):
		case(UR_ELEM_ADD):
		case(UR_ELEM_DEL):
		case(UR_ELEM_MFD):
		case(UR_ELEM_MFS):
			EnableDisableControls();
			break;
		default:
			break;
		}
	}
}

void CSeisCvlEvalParamDlg::InitUnit()
{
	m_edtHe1L.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untHe1L.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtHe2L.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untHe2L.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtHe1T.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untHe1T.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtHe2T.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untHe2T.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtAeRat.SetUnitType(D_UNITSYS_NONE);
	m_edtLP.SetUnitType(D_UNITSYS_NONE);
	m_edtTs.SetUnitType(D_UNITSYS_BASE_TIME);
	m_untTs.SetUnitType(D_UNITSYS_BASE_TIME);
}

void CSeisCvlEvalParamDlg::InitCombo()
{
	InitCombo_EvgpGrup();
	InitCombo_EvgpColm();

	m_cmbBehavL.ResetContent();
	m_cmbBehavL.AddString(_LS(IDS_WG_TREEMENU_EVAL_PARAM_BEHAV_RAHMEN));
	m_cmbBehavL.AddString(_LS(IDS_WG_TREEMENU_EVAL_PARAM_BEHAV_CANTI));

	m_cmbBehavT.ResetContent();
	m_cmbBehavT.AddString(_LS(IDS_WG_TREEMENU_EVAL_PARAM_BEHAV_RAHMEN));
	m_cmbBehavT.AddString(_LS(IDS_WG_TREEMENU_EVAL_PARAM_BEHAV_CANTI));	

	m_cmbSplice.ResetContent();
	m_cmbSplice.AddString(_LS(IDS_WG_TREEMENU_EVAL_PARAM_SPLICE_1));
	m_cmbSplice.AddString(_LS(IDS_WG_TREEMENU_EVAL_PARAM_SPLICE_2));
	m_cmbSplice.AddString(_LS(IDS_WG_TREEMENU_EVAL_PARAM_SPLICE_3));

	CDlgUtil::CobxAdjustListBoxWidth(m_cmbBehavL);
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbBehavT);
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbSplice);

	m_cmbBehavL.SetCurSel(0);
	m_cmbBehavT.SetCurSel(0);
	m_cmbSplice.SetCurSel(0);
}

void CSeisCvlEvalParamDlg::InitCombo_EvgpGrup()
{
	m_cmbEvalGrup.ResetContent();

	CArray<T_EVGP_K,T_EVGP_K> aEvgpK;
	m_pDoc->m_pAttrCtrl2->GetEvgpKeyList(aEvgpK);

	T_EVGP_D EvgpD; EvgpD.Initialize();

	for(int i=0; i<aEvgpK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl2->GetEvgp(aEvgpK[i], EvgpD)) continue;
		if(EvgpD.nType == 1) continue;
		m_cmbEvalGrup.SetItemData(m_cmbEvalGrup.AddString(EvgpD.strName), aEvgpK[i]);
	}

	CDlgUtil::CobxAdjustListBoxWidth(m_cmbEvalGrup);

	m_cmbEvalGrup.SetCurSel(0);

	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
}

void CSeisCvlEvalParamDlg::InitCombo_EvgpColm()
{
	m_cmbEvalColm.ResetContent();

	T_EVGP_K EvgpK;
	EvgpK = CDlgUtil::CobxGetCurSelItemData(m_cmbEvalGrup, m_cmbEvalGrup.GetCurSel());

	T_EVGP_D EvgpD; EvgpD.Initialize();
	if(!m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD)) return;

	for(int i=0; i<EvgpD.aEvgpColm.GetSize(); i++)
	{		
		m_cmbEvalColm.SetItemData(m_cmbEvalColm.AddString(EvgpD.aEvgpColm[i].strName), i);
	}

	CDlgUtil::CobxAdjustListBoxWidth(m_cmbEvalColm);

	m_cmbEvalColm.SetCurSel(0);

	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
}

BOOL CSeisCvlEvalParamDlg::IsEvgpSectSR()
{
	T_EVGP_K EvgpK;
	EvgpK = CDlgUtil::CobxGetCurSelItemData(m_cmbEvalGrup, m_cmbEvalGrup.GetCurSel());

	T_EVGP_D EvgpD; EvgpD.Initialize();
	if(!m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD)) return FALSE;

	int nEvgpColm = CDlgUtil::CobxGetCurSelItemData(m_cmbEvalColm, m_cmbEvalColm.GetCurSel());
	
	T_ELEM_D ElemD;
	T_SECT_D SectD;
	for (int i = 0; i < EvgpD.aEvgpColm[nEvgpColm].aColumn.GetSize(); i++)
	{ 
		ElemD.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetElem(EvgpD.aEvgpColm[nEvgpColm].aColumn[i], ElemD)) return FALSE;

		SectD.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetSect(ElemD.elpro, SectD)) return FALSE;

		if(SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SR)
			return TRUE;
	}

	return FALSE;
}

void CSeisCvlEvalParamDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	int nBehavL = m_cmbBehavL.GetCurSel();
	int nBehavT = m_cmbBehavT.GetCurSel();
	BOOL bEvgpSectSR = IsEvgpSectSR();
	int nSplice = m_cmbSplice.GetCurSel();

	T_BSED_D BsedD;
	if (!m_pDoc->m_pAttrCtrl2->GetDgnBsed(BsedD)) BsedD.Initialize();

	GetDlgItem(IDC_TM_SEIS_CVL_MISC_EFF_LENGTH_GRP)->EnableWindow(m_nOption==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_LONGI_GRP)->EnableWindow(m_nOption==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_L_BEHAV_STC)->EnableWindow(m_nOption==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_L_BEHAV_CMB)->EnableWindow(m_nOption==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_L_BEHAV_BTN)->EnableWindow(m_nOption==0 && m_pDoc->IsPostMode());
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_L_HE1_STC)->EnableWindow(m_nOption==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_L_HE1_EDT)->EnableWindow(m_nOption==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_L_HE1_UNT)->EnableWindow(m_nOption==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_L_HE2_STC)->EnableWindow(m_nOption==0 && nBehavL==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_L_HE2_EDT)->EnableWindow(m_nOption==0 && nBehavL==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_L_HE2_UNT)->EnableWindow(m_nOption==0 && nBehavL==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_TRANS_GRP)->EnableWindow(m_nOption==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_T_BEHAV_STC)->EnableWindow(m_nOption==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_T_BEHAV_CMB)->EnableWindow(m_nOption==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_T_BEHAV_BTN)->EnableWindow(m_nOption==0 && m_pDoc->IsPostMode());
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_T_HE1_STC)->EnableWindow(m_nOption==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_T_HE1_EDT)->EnableWindow(m_nOption==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_T_HE1_UNT)->EnableWindow(m_nOption==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_T_HE2_STC)->EnableWindow(m_nOption==0 && nBehavT==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_T_HE2_EDT)->EnableWindow(m_nOption==0 && nBehavT==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_T_HE2_UNT)->EnableWindow(m_nOption==0 && nBehavT==0);

	GetDlgItem(IDC_TM_SEIS_CVL_MISC_EFF_AE_STC)->EnableWindow(m_nOption==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_AE_RAT_EDT)->EnableWindow(m_nOption==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_EFF_AG_STC)->EnableWindow(m_nOption==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_SPLICE_STC)->EnableWindow(m_nOption==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_SPLICE_CMB)->EnableWindow(m_nOption==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_BH_METHOD_RDO1)->EnableWindow(m_nOption==0 && !bEvgpSectSR);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_BH_METHOD_RDO2)->EnableWindow(m_nOption==0 && !bEvgpSectSR);

	if(BsedD.nDesignCode == KISTEC_2019 || BsedD.nDesignCode == KALIS_2023)
	{
		GetDlgItem(IDC_TM_SEIS_CVL_MISC_LP_EDT)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_SEIS_CVL_MISC_LP_STC)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_TM_SEIS_CVL_MISC_LP_STC)->EnableWindow(m_nOption == 0 && !bEvgpSectSR && nSplice == 2);
		GetDlgItem(IDC_TM_SEIS_CVL_MISC_LP_EDT)->EnableWindow(m_nOption == 0 && !bEvgpSectSR && nSplice == 2);
	}

	GetDlgItem(IDC_TM_SEIS_CVL_MISC_TS_STC)->EnableWindow(m_nOption==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_TS_EDT)->EnableWindow(m_nOption==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_TS_UNT)->EnableWindow(m_nOption==0);
	GetDlgItem(IDC_TM_SEIS_CVL_MISC_TS_BTN)->EnableWindow(m_nOption==0);
}

void CSeisCvlEvalParamDlg::GetLp()
{
	double dMinLp = 0.0;

	T_EVGP_K EvgpK;
	EvgpK = m_cmbEvalGrup.GetItemData(m_cmbEvalGrup.GetCurSel());

	CArray<T_MPCC_K, T_MPCC_K> aMpccK;
	m_pDoc->m_pAttrCtrl2->GetMpccKeyList(aMpccK);

	T_FIMP_D FimpD; FimpD.Initialize();
	T_MPCC_D MpccD; MpccD.Initialize();

	for(int i=0; i<aMpccK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl2->GetMpcc(aMpccK[i], MpccD)) continue;
		if(MpccD.EvgpK != EvgpK) continue;

		// Longitudinal
		if(!m_pDoc->m_pAttrCtrl->GetFimp(MpccD.MatlConcK1, FimpD)) continue;
		if(FimpD.nMaterialType != 0 || FimpD.nHysModelType != 6 || FimpD.PROP.CON_MANDER.nConcType != 0) continue;
		
		if(!m_pDoc->m_pAttrCtrl->GetFimp(MpccD.MatlConcK2, FimpD)) continue;
		if(FimpD.nMaterialType != 0 || FimpD.nHysModelType != 6 || FimpD.PROP.CON_MANDER.nConcType != 1) continue;

		if(FimpD.PROP.CON_MANDER.nSectRebarType == 0 || FimpD.PROP.CON_MANDER.nSectRebarType == 2)
		{
			if(FimpD.PROP.CON_MANDER.dRectRatiopx > 0.0) 
			{
				if(dMinLp > 0.0) dMinLp = __min(dMinLp, FimpD.PROP.CON_MANDER.dRectRatiopx);
				else             dMinLp = FimpD.PROP.CON_MANDER.dRectRatiopx;
			}
				
			if(FimpD.PROP.CON_MANDER.dRectRatiopy > 0.0) 
			{
				if(dMinLp > 0.0) dMinLp = __min(dMinLp, FimpD.PROP.CON_MANDER.dRectRatiopy);
				else             dMinLp = FimpD.PROP.CON_MANDER.dRectRatiopy;
			}	
		}
		else
		{
			if(FimpD.PROP.CON_MANDER.dCircularRatiops > 0.0) 
			{
				if(dMinLp > 0.0) dMinLp = __min(dMinLp, FimpD.PROP.CON_MANDER.dCircularRatiops);
				else             dMinLp = FimpD.PROP.CON_MANDER.dCircularRatiops;
			}	
		}

		// Transverse
		if(!m_pDoc->m_pAttrCtrl->GetFimp(MpccD.MatlConcK1Trans, FimpD)) continue;
		if(FimpD.nMaterialType != 0 || FimpD.nHysModelType != 6 || FimpD.PROP.CON_MANDER.nConcType != 0) continue;

		if(!m_pDoc->m_pAttrCtrl->GetFimp(MpccD.MatlConcK2Trans, FimpD)) continue;
		if(FimpD.nMaterialType != 0 || FimpD.nHysModelType != 6 || FimpD.PROP.CON_MANDER.nConcType != 1) continue;

		if(FimpD.PROP.CON_MANDER.nSectRebarType == 0 || FimpD.PROP.CON_MANDER.nSectRebarType == 2)
		{
			if(FimpD.PROP.CON_MANDER.dRectRatiopx > 0.0) 
			{
				if(dMinLp > 0.0) dMinLp = __min(dMinLp, FimpD.PROP.CON_MANDER.dRectRatiopx);
				else             dMinLp = FimpD.PROP.CON_MANDER.dRectRatiopx;
			}

			if(FimpD.PROP.CON_MANDER.dRectRatiopy > 0.0) 
			{
				if(dMinLp > 0.0) dMinLp = __min(dMinLp, FimpD.PROP.CON_MANDER.dRectRatiopy);
				else             dMinLp = FimpD.PROP.CON_MANDER.dRectRatiopy;
			}	
		}
		else
		{
			if(FimpD.PROP.CON_MANDER.dCircularRatiops > 0.0) 
			{
				if(dMinLp > 0.0) dMinLp = __min(dMinLp, FimpD.PROP.CON_MANDER.dCircularRatiops);
				else             dMinLp = FimpD.PROP.CON_MANDER.dCircularRatiops;
			}	
		}
	}

	m_edtLP.SetEditUnit(dMinLp, 6);
}


BEGIN_MESSAGE_MAP(CSeisCvlEvalParamDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CSeisCvlEvalParamDlg)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_MISC_ADD_RDO,              OnTmEvalParamOptionRdo)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_MISC_DEL_RDO,              OnTmEvalParamOptionRdo)
	ON_CBN_SELCHANGE(IDC_TM_SEIS_CVL_MISC_EVAL_GRP_CMB,      OnTmEvalParamEvalGrpCmb)
	ON_CBN_SELCHANGE(IDC_TM_SEIS_CVL_MISC_EVAL_GRP_COLM_CMB, OnTmEvalParamEvalColmCmb)
	ON_CBN_SELCHANGE(IDC_TM_SEIS_CVL_MISC_L_BEHAV_CMB,       OnTmEvalParamBehavLCmb)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_MISC_L_BEHAV_BTN,          OnTmEvalParamLongiDirBtn)
	ON_CBN_SELCHANGE(IDC_TM_SEIS_CVL_MISC_T_BEHAV_CMB,       OnTmEvalParamBehavTCmb)
	ON_CBN_SELCHANGE(IDC_TM_SEIS_CVL_MISC_SPLICE_CMB,        OnTmEvalParamSpliceCmb)  
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_MISC_T_BEHAV_BTN,          OnTmEvalParamTransDirBtn)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_MISC_TS_BTN,               OnTmEvalParamTsBtn)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_MISC_APPLY_BTN,            OnTmEvalParamApplyBtn)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_MISC_CLOSE_BTN,            OnTmEvalParamCloseBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlEvalParamDlg message handlers

BOOL CSeisCvlEvalParamDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	InitUnit();
	InitCombo();
	EnableDisableControls();

	GetLp();

	// Default Setting
	m_edtAeRat.SetEditUnit(0.8);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisCvlEvalParamDlg::OnTmEvalParamEvalGrpCmb()
{
	UpdateData(TRUE);

	T_EVGP_K EvgpK;
	T_EVGP_D EvgpD; EvgpD.Initialize();
	EvgpK = m_cmbEvalGrup.GetItemData(m_cmbEvalGrup.GetCurSel());
	if(!m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD)) return;

	InitCombo_EvgpColm();

	CArray<T_ELEM_K, T_ELEM_K> aElemK;
	aElemK.RemoveAll();

	if(EvgpD.aEvgpColm.GetSize() > 0)
	{
		for(int i=0; i<EvgpD.aEvgpColm[0].aColumn.GetSize(); i++) aElemK.Add(EvgpD.aEvgpColm[0].aColumn[i]);

		if(EvgpD.bPierCap)
		{
			for(int i=0; i<EvgpD.aPierCap.GetSize(); i++) aElemK.Add(EvgpD.aPierCap[i]);
		}

		m_pDoc->m_pViewCtrl->SelectElem(NULL, aElemK, FALSE, TRUE);

		EnableDisableControls();

		GetLp();
	}	
}

void CSeisCvlEvalParamDlg::OnTmEvalParamEvalColmCmb()
{
	UpdateData(TRUE);

	T_EVGP_K EvgpK;
	T_EVGP_D EvgpD; EvgpD.Initialize();
	EvgpK = m_cmbEvalGrup.GetItemData(m_cmbEvalGrup.GetCurSel());
	if(!m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD)) return;

	CArray<T_ELEM_K, T_ELEM_K> aElemK;
	aElemK.RemoveAll();

	int nEvgpColm = CDlgUtil::CobxGetCurSelItemData(m_cmbEvalColm, m_cmbEvalColm.GetCurSel());
		
	for(int i=0; i<EvgpD.aEvgpColm[nEvgpColm].aColumn.GetSize(); i++) aElemK.Add(EvgpD.aEvgpColm[nEvgpColm].aColumn[i]);

	if(EvgpD.bPierCap)
	{
		for(int i=0; i<EvgpD.aPierCap.GetSize(); i++) aElemK.Add(EvgpD.aPierCap[i]);
	}

	m_pDoc->m_pViewCtrl->SelectElem(NULL, aElemK, FALSE, TRUE);

	EnableDisableControls();

	GetLp();
}

void CSeisCvlEvalParamDlg::OnTmEvalParamOptionRdo()
{
	EnableDisableControls();
}

void CSeisCvlEvalParamDlg::OnTmEvalParamBehavLCmb()
{
	EnableDisableControls();
}

void CSeisCvlEvalParamDlg::OnTmEvalParamLongiDirBtn()
{
	T_EVGP_K EvgpK;
	EvgpK = m_cmbEvalGrup.GetItemData(m_cmbEvalGrup.GetCurSel());
	int nEvgpColm = CDlgUtil::CobxGetCurSelItemData(m_cmbEvalColm, m_cmbEvalColm.GetCurSel());

	CSeisCvlForceDirDlg dlg;
	dlg.m_EvgpK = EvgpK;
	dlg.m_nEvgpColm = nEvgpColm;
	dlg.m_nDir = 0;
	if(dlg.DoModal() == IDOK)
	{
		m_cmbBehavL.SetCurSel(dlg.m_nBehav);
		m_edtHe1L.SetEditUnit(dlg.m_dHe1);
		m_edtHe2L.SetEditUnit(dlg.m_dHe2);
	}

	EnableDisableControls();
}

void CSeisCvlEvalParamDlg::OnTmEvalParamBehavTCmb()
{
	EnableDisableControls();
}

void CSeisCvlEvalParamDlg::OnTmEvalParamSpliceCmb()
{
	EnableDisableControls();
}

void CSeisCvlEvalParamDlg::OnTmEvalParamTransDirBtn()
{
	T_EVGP_K EvgpK;
	EvgpK = m_cmbEvalGrup.GetItemData(m_cmbEvalGrup.GetCurSel());
	int nEvgpColm = CDlgUtil::CobxGetCurSelItemData(m_cmbEvalColm, m_cmbEvalColm.GetCurSel());

	CSeisCvlForceDirDlg dlg;
	dlg.m_EvgpK = EvgpK;
	dlg.m_nEvgpColm = nEvgpColm;
	dlg.m_nDir = 1;
	if(dlg.DoModal() == IDOK)
	{
		m_cmbBehavT.SetCurSel(dlg.m_nBehav);
		m_edtHe1T.SetEditUnit(dlg.m_dHe1);
		m_edtHe2T.SetEditUnit(dlg.m_dHe2);
	}

	EnableDisableControls();
}

void CSeisCvlEvalParamDlg::OnTmEvalParamTsBtn()
{
	CETC_DESIGN_SPECT_TS dlg;
	if(dlg.DoModal() == IDOK)
	{
		m_edtTs.SetEditUnit(dlg.m_dTs);
	}
}

void CSeisCvlEvalParamDlg::OnTmEvalParamApplyBtn()
{
	UpdateData(TRUE);

	T_EVGP_K EvgpK;
	EvgpK = m_cmbEvalGrup.GetItemData(m_cmbEvalGrup.GetCurSel());
	int nEvgpColm = CDlgUtil::CobxGetCurSelItemData(m_cmbEvalColm, m_cmbEvalColm.GetCurSel());

	if(m_nOption == 0)
	{
		T_EVPM_K EvpmK; 
		T_EVPM_D EvpmD; EvpmD.Initialize();

		EvpmD.EvgpK = EvgpK;
		EvpmD.nEvgpColm = nEvgpColm;
		
		EvpmD.nBehavL = m_cmbBehavL.GetCurSel();
		EvpmD.dHe1L = m_edtHe1L.GetEditValue();
		EvpmD.dHe2L = m_edtHe2L.GetEditValue();

		EvpmD.nBehavT = m_cmbBehavT.GetCurSel();
		EvpmD.dHe1T = m_edtHe1T.GetEditValue();
		EvpmD.dHe2T = m_edtHe2T.GetEditValue();

		EvpmD.dAeRat = m_edtAeRat.GetEditValue();
		EvpmD.nBH = m_nBHMethod;
		EvpmD.nSplice = m_cmbSplice.GetCurSel();
		EvpmD.dLp = m_edtLP.GetEditValue();
		EvpmD.dTs = m_edtTs.GetEditValue();

		EvpmK = m_pDoc->m_pAttrCtrl2->GetEvpmKey(EvgpK, nEvgpColm);
		if(m_pDoc->m_pAttrCtrl2->ExistEvpm(EvpmK))
		{
			if(!m_pDoc->m_pDataCtrl->ModifyEvpm(EvpmK, EvpmK, EvpmD)) return;
		}
		else
		{
			EvpmK = m_pDoc->m_pAttrCtrl2->GetStartNumEvpm();
			if(!m_pDoc->m_pDataCtrl->AddEvpm(EvpmK, EvpmD)) return;	
		}
	}
	else
	{
		T_EVPM_K EvpmK; EvpmK = m_pDoc->m_pAttrCtrl2->GetEvpmKey(EvgpK, nEvgpColm);
		if(!m_pDoc->m_pDataCtrl->DelEvpm(EvpmK)) return;
	}

	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
}

void CSeisCvlEvalParamDlg::OnTmEvalParamCloseBtn()
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}