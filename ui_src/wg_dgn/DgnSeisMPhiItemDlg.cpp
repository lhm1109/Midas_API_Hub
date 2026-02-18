// DgnSeisMPhiItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisMPhiItemDlg.h"

#include "DgnMPhiCurveConditionDlg.h"
#include "DgnSeisMPhiAutoAxialForceDlg.h"
#include "DgnSeisMPhiCurveDlg.h"
#include "DgnSeisMPhiIdealGuideDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "BrdgSeisEvalKALIS2023.h"
#include "BrdgSeisEvalKISTEC2019.h"
#include "BrdgSeisEvalKISTEC2015.h"
#include "BrdgSeisEvalKEC2012.h"

#include "../wg_main/wg_main.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisMPhiItemDlg dialog

CDgnSeisMPhiItemDlg::CDgnSeisMPhiItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSeisMPhiItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisMPhiItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bEvalGrup = TRUE;
	m_nAxialForce = 0;
	m_bIdealized = FALSE;
	m_bUserCurve = FALSE;
	m_nIdealOpt = 0;

	m_pDoc = CDBDoc::GetDocPoint();

	m_Data.Initialize();
	m_SyefD.Initialize();
	m_bModify = FALSE;
}


void CDgnSeisMPhiItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisMPhiItemDlg)
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_NAME_EDT,               m_edtName);
	DDX_Check  (pDX, IDC_DGN_SEIS_MPHI_EVAL_CHK,               m_bEvalGrup);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_EVAL_CMB,               m_cmbEvalGrup);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_EVAL_COLM_CMB,          m_cmbEvalColm);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_EVAL_POS_CMB,           m_cmbEvalPos);	
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_SECT_CMB,               m_cmbSect);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_SECT_POS_CMB,           m_cmbSectPos);	
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_INELAS_CONC_CMB1,       m_cmbConc1);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_INELAS_CONC_CMB2,       m_cmbConc2);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_INELAS_CONC_TRANS_CMB1, m_cmbConc1Trans);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_INELAS_CONC_TRANS_CMB2, m_cmbConc2Trans);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_INELAS_STL_CMB,         m_cmbSteel);
	DDX_Radio  (pDX, IDC_DGN_SEIS_MPHI_AXIAL_FORCE_AUTO_RDO,   m_nAxialForce);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_L_CASE_COMB_CMB,        m_cmbLcom);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_AXIAL_FORCE_EDT,        m_edtAxialForce);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_AXIAL_FORCE_UNT,        m_untAxialForce);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_ANGLE_EDT,              m_edtAxisAngle);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_ANGLE_UNT,              m_untAxisAngle);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_POINT_EDT,              m_edtPoint);
	DDX_Check  (pDX, IDC_DGN_SEIS_MPHI_IDEAL_CHK,              m_bIdealized);
	DDX_Check  (pDX, IDC_DGN_SEIS_MPHI_USER_CHK,               m_bUserCurve);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_USER_EDT,               m_edtUserCurve);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_USER_UNT,               m_untUserCurve);
	DDX_Radio  (pDX, IDC_DGN_SEIS_MPHI_IDEAL_OPT1_RDO,         m_nIdealOpt);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_SECT_VIEW,              m_SectWnd);	
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_SECT_GRP,               m_chkSect);
	//}}AFX_DATA_MAP
}

void CDgnSeisMPhiItemDlg::InitUnit()
{
	m_edtAxialForce.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_untAxialForce.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_edtAxisAngle .SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_untAxisAngle .SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_edtPoint     .SetUnitType(D_UNITSYS_NONE);
	m_edtUserCurve .SetUnitType(D_UNITSYS_BASE_PERLENGTH);
	m_untUserCurve .SetUnitType(D_UNITSYS_BASE_PERLENGTH);	
}

void CDgnSeisMPhiItemDlg::InitCombo()
{
	// Evaluation Group
	m_cmbEvalGrup.ResetContent();
	
	T_EVGP_D EvgpD; EvgpD.Initialize();
	CArray<T_EVGP_K, T_EVGP_K> aEvgpK;
	m_pDoc->m_pAttrCtrl2->GetEvgpKeyList(aEvgpK);

	for(int i=0; i<aEvgpK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl2->GetEvgp(aEvgpK[i], EvgpD)) continue;
		if(EvgpD.nType == 1) continue;
		m_cmbEvalGrup.SetItemData(m_cmbEvalGrup.AddString(EvgpD.strName), aEvgpK[i]);
	}

	m_cmbEvalGrup.SetCurSel(0);

	// Column
	InitEvgpColmCombo();

	// Position
	m_cmbEvalPos.ResetContent();
	m_cmbEvalPos.AddString(_LS(IDS_DGN_SEIS_MPCC_TOP));
	m_cmbEvalPos.AddString(_LS(IDS_DGN_SEIS_MPCC_BOT));
	m_cmbEvalPos.SetCurSel(0);

	// Section
	m_cmbSect.ResetContent();

	T_SECT_D SectD; SectD.Initialize();
	CArray<T_SECT_K, T_SECT_K> aSectK;
	m_pDoc->m_pAttrCtrl->GetSectKeyList(aSectK);

	for(int i=0; i<aSectK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetSect(aSectK[i], SectD)) continue;
		m_cmbSect.SetItemData(m_cmbSect.AddString(SectD.SName), aSectK[i]);
	}

	m_cmbSect.SetCurSel(0);

	// Position
	m_cmbSectPos.ResetContent();
	m_cmbSectPos.AddString(_LS(IDS_DGN_SEIS_MPCC_I));
	m_cmbSectPos.AddString(_LS(IDS_DGN_SEIS_MPCC_M));
	m_cmbSectPos.AddString(_LS(IDS_DGN_SEIS_MPCC_J));
	m_cmbSectPos.SetCurSel(0);

	SetInMatlCobx();


	// Load Case/Combination
	m_cmbLcom.ResetContent();
	m_cmbLcom.SetLoadType(D_SELECTLOAD_ALLWITHENVELOPE);	
	m_cmbLcom.SetCurSel(0);

	CDlgUtil::CobxAdjustListBoxWidth(m_cmbLcom);
}

void CDgnSeisMPhiItemDlg::InitEvgpColmCombo()
{
	m_cmbEvalColm.ResetContent();

	T_EVGP_K EvgpK;
	EvgpK = m_cmbEvalGrup.GetItemData(m_cmbEvalGrup.GetCurSel());

	if(m_pDoc->m_pAttrCtrl2->ExistEvgp(EvgpK))
	{
		T_EVGP_D EvgpD; EvgpD.Initialize();
		if(!m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD))
		{
			return;
		}

		if(EvgpD.aEvgpColm.GetSize() > 0)
		{
			for(int i=0; i<EvgpD.aEvgpColm.GetSize(); i++)
			{
				m_cmbEvalColm.SetItemData(m_cmbEvalColm.AddString(EvgpD.aEvgpColm[i].strName), i);
			}

			m_cmbEvalColm.SetCurSel(0);
		}
	}
}

void CDgnSeisMPhiItemDlg::SetInMatlCobx()
{
	// Inelastic Material Property
	T_FIMP_K FimpKConc1, FimpKConc2, FimpKConc1Trans, FimpKConc2Trans, FimpKSteel;
	FimpKConc1      = CDlgUtil::CobxGetCurSelItemData(m_cmbConc1,      m_cmbConc1.GetCurSel());
	FimpKConc2      = CDlgUtil::CobxGetCurSelItemData(m_cmbConc2,      m_cmbConc2.GetCurSel());
	FimpKConc1Trans = CDlgUtil::CobxGetCurSelItemData(m_cmbConc1Trans, m_cmbConc1Trans.GetCurSel());
	FimpKConc2Trans = CDlgUtil::CobxGetCurSelItemData(m_cmbConc2Trans, m_cmbConc2Trans.GetCurSel());
	FimpKSteel      = CDlgUtil::CobxGetCurSelItemData(m_cmbSteel,      m_cmbSteel.GetCurSel());

	m_cmbConc1.ResetContent();
	m_cmbConc2.ResetContent();
	m_cmbConc1Trans.ResetContent();
	m_cmbConc2Trans.ResetContent();
	m_cmbSteel.ResetContent();

	T_FIMP_D FimpD; FimpD.Initialize();
	CArray<T_FIMP_K, T_FIMP_K> aFimpK;
	m_pDoc->m_pAttrCtrl->GetFimpKeyList(aFimpK);

	for(int i=0; i<aFimpK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetFimp(aFimpK[i], FimpD)) continue;

		if(FimpD.nMaterialType == 0) // Concrete
		{
			if(FimpD.nHysModelType == 6 && FimpD.PROP.CON_MANDER.nConcType == 1) // Mander && Mander - confined
			{
				m_cmbConc2.SetItemData(m_cmbConc2.AddString(FimpD.strName), aFimpK[i]);
				m_cmbConc2Trans.SetItemData(m_cmbConc2Trans.AddString(FimpD.strName), aFimpK[i]);
			}
			else
			{
				m_cmbConc1.SetItemData(m_cmbConc1.AddString(FimpD.strName), aFimpK[i]);
				m_cmbConc1Trans.SetItemData(m_cmbConc1Trans.AddString(FimpD.strName), aFimpK[i]);
			}
		}
		else // Steel
		{
			m_cmbSteel.SetItemData(m_cmbSteel.AddString(FimpD.strName), aFimpK[i]);
		}
	}
	
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbConc1);
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbConc2);
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbConc1Trans);
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbConc2Trans);
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbSteel);

	if(CDlgUtil::CobxSetCurSelItemData(m_cmbConc1,      FimpKConc1     ) == CB_ERR && m_cmbConc1.GetCount() > 0     ) m_cmbConc1.SetCurSel(0);
	if(CDlgUtil::CobxSetCurSelItemData(m_cmbConc2,      FimpKConc2     ) == CB_ERR && m_cmbConc2.GetCount() > 0     ) m_cmbConc2.SetCurSel(0);
	if(CDlgUtil::CobxSetCurSelItemData(m_cmbConc1Trans, FimpKConc1Trans) == CB_ERR && m_cmbConc1Trans.GetCount() > 0) m_cmbConc1Trans.SetCurSel(0);
	if(CDlgUtil::CobxSetCurSelItemData(m_cmbConc2Trans, FimpKConc2Trans) == CB_ERR && m_cmbConc2Trans.GetCount() > 0) m_cmbConc2Trans.SetCurSel(0);
	if(CDlgUtil::CobxSetCurSelItemData(m_cmbSteel,      FimpKSteel     ) == CB_ERR && m_cmbSteel.GetCount() > 0     ) m_cmbSteel.SetCurSel(0);
}

void CDgnSeisMPhiItemDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	T_FIMP_K FimpK;
	T_FIMP_K FimpKTrans;
	T_FIMP_D FimpD;
	T_FIMP_D FimpDTrans;
	FimpK = m_cmbConc1.GetItemData(m_cmbConc1.GetCurSel());
	FimpKTrans = m_cmbConc1Trans.GetItemData(m_cmbConc1Trans.GetCurSel());
	if(!m_pDoc->m_pAttrCtrl->GetFimp(FimpK, FimpD)) FimpD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetFimp(FimpKTrans, FimpDTrans)) FimpDTrans.Initialize();

	GetDlgItem(IDC_DGN_SEIS_MPHI_EVAL_STC)->EnableWindow(m_bEvalGrup);
	GetDlgItem(IDC_DGN_SEIS_MPHI_EVAL_CMB)->EnableWindow(m_bEvalGrup);
	GetDlgItem(IDC_DGN_SEIS_MPHI_EVAL_COLM_STC)->EnableWindow(m_bEvalGrup);
	GetDlgItem(IDC_DGN_SEIS_MPHI_EVAL_COLM_CMB)->EnableWindow(m_bEvalGrup);
	GetDlgItem(IDC_DGN_SEIS_MPHI_EVAL_POS_STC)->EnableWindow(m_bEvalGrup);
	GetDlgItem(IDC_DGN_SEIS_MPHI_EVAL_POS_CMB)->EnableWindow(m_bEvalGrup);
	GetDlgItem(IDC_DGN_SEIS_MPHI_SECT_STC)->EnableWindow(!m_bEvalGrup);
	GetDlgItem(IDC_DGN_SEIS_MPHI_SECT_CMB)->EnableWindow(!m_bEvalGrup);
	GetDlgItem(IDC_DGN_SEIS_MPHI_SECT_POS_STC)->EnableWindow(!m_bEvalGrup);
	GetDlgItem(IDC_DGN_SEIS_MPHI_SECT_POS_CMB)->EnableWindow(!m_bEvalGrup);
	
	GetDlgItem(IDC_DGN_SEIS_MPHI_INELAS_CONC_CMB2)->EnableWindow(FimpD.nHysModelType == 6);
	GetDlgItem(IDC_DGN_SEIS_MPHI_INELAS_CONC_TRANS_CMB2)->EnableWindow(FimpDTrans.nHysModelType == 6);

	BOOL bExistEvgp = m_pDoc->m_pAttrCtrl2->ExistEvgp(m_cmbEvalGrup.GetItemData(m_cmbEvalGrup.GetCurSel()));
	GetDlgItem(IDC_DGN_SEIS_MPHI_AXIAL_FORCE_AUTO_RDO)->EnableWindow(m_pDoc->IsPostMode() && m_bEvalGrup && bExistEvgp);
	GetDlgItem(IDC_DGN_SEIS_MPHI_L_CASE_COMB_STC)->EnableWindow(m_pDoc->IsPostMode() && m_bEvalGrup && bExistEvgp && m_nAxialForce == 0);
	GetDlgItem(IDC_DGN_SEIS_MPHI_L_CASE_COMB_CMB)->EnableWindow(m_pDoc->IsPostMode() && m_bEvalGrup && bExistEvgp && m_nAxialForce == 0);
	GetDlgItem(IDC_DGN_SEIS_MPHI_L_CASE_COMB_BTN)->EnableWindow(m_pDoc->IsPostMode() && m_bEvalGrup && bExistEvgp && m_nAxialForce == 0);
	
	GetDlgItem(IDC_DGN_SEIS_MPHI_ANGLE_STC)->EnableWindow(!m_bEvalGrup);
	GetDlgItem(IDC_DGN_SEIS_MPHI_ANGLE_EDT)->EnableWindow(!m_bEvalGrup);
	GetDlgItem(IDC_DGN_SEIS_MPHI_ANGLE_UNT)->EnableWindow(!m_bEvalGrup);
	GetDlgItem(IDC_DGN_SEIS_MPHI_POINT_STC)->EnableWindow(!m_bEvalGrup);
	GetDlgItem(IDC_DGN_SEIS_MPHI_POINT_EDT)->EnableWindow(!m_bEvalGrup);
	
	GetDlgItem(IDC_DGN_SEIS_MPHI_USER_CHK)->EnableWindow(m_bIdealized);
	GetDlgItem(IDC_DGN_SEIS_MPHI_USER_EDT)->EnableWindow(m_bIdealized && m_bUserCurve);
	GetDlgItem(IDC_DGN_SEIS_MPHI_USER_UNT)->EnableWindow(m_bIdealized && m_bUserCurve);
	GetDlgItem(IDC_DGN_SEIS_MPHI_IDEAL_OPT_GRP)->EnableWindow(m_bIdealized);
	GetDlgItem(IDC_DGN_SEIS_MPHI_IDEAL_OPT1_RDO)->EnableWindow(m_bIdealized);
	GetDlgItem(IDC_DGN_SEIS_MPHI_IDEAL_OPT2_RDO)->EnableWindow(m_bIdealized);
	GetDlgItem(IDC_DGN_SEIS_MPHI_IDEAL_OPT3_RDO)->EnableWindow(m_bIdealized);
	GetDlgItem(IDC_DGN_SEIS_MPHI_IDEAL_OPT_GUIDE_BTN)->EnableWindow(m_bIdealized);	
}

void CDgnSeisMPhiItemDlg::Data2Dlg()
{
 	m_edtName.SetWindowText(m_Data.strName);
	m_bEvalGrup = m_Data.bEvgp;

	CArray<T_EVGP_K, T_EVGP_K> aEvgpK;
	m_pDoc->m_pAttrCtrl2->GetEvgpKeyList(aEvgpK);

    T_EVGP_D EvgpD; EvgpD.Initialize();

    int k = 0;

	for(int i=0; i<aEvgpK.GetSize(); i++)
	{
        if ( !m_pDoc->m_pAttrCtrl2->GetEvgp(aEvgpK[i], EvgpD) ) continue;
        if ( EvgpD.nType == 1 )
        {
            k += 1;
            continue;
        }

        if ( aEvgpK[i] == m_Data.EvgpK )
        {
            m_cmbEvalGrup.SetCurSel(i - k);
            break;
        }
    }

	m_cmbEvalColm.SetCurSel(m_Data.nEvgpColm);
	m_cmbEvalPos.SetCurSel(m_Data.nEvgpPos);
	
	CArray<T_SECT_K, T_SECT_K> aSectK;
	m_pDoc->m_pAttrCtrl->GetSectKeyList(aSectK);

	for(int i=0; i<aSectK.GetSize(); i++)
	{
		if(aSectK[i] == m_Data.SectK) 
		{ 
			m_cmbSect.SetCurSel(i); 
			break; 
		}
	}
	
	m_cmbSectPos.SetCurSel(m_Data.nPosition);

	int nSelect = 0;
	for(int i=0; i<m_cmbConc1.GetCount(); i++)
	{
		if(m_Data.MatlConcK1 != m_cmbConc1.GetItemData(i)) continue;
		nSelect = i;
		break;
	}
	m_cmbConc1.SetCurSel(nSelect);

	nSelect = 0;
	for(int i=0; i<m_cmbConc2.GetCount(); i++)
	{
		if(m_Data.MatlConcK2 != m_cmbConc2.GetItemData(i)) continue;
		nSelect = i;
		break;
	}
	m_cmbConc2.SetCurSel(nSelect);

	nSelect = 0;
	for(int i=0; i<m_cmbConc1Trans.GetCount(); i++)
	{
		if(m_Data.MatlConcK1Trans != m_cmbConc1Trans.GetItemData(i)) continue;
		nSelect = i;
		break;
	}
	m_cmbConc1Trans.SetCurSel(nSelect);

	nSelect = 0;
	for(int i=0; i<m_cmbConc2Trans.GetCount(); i++)
	{
		if(m_Data.MatlConcK2Trans != m_cmbConc2Trans.GetItemData(i)) continue;
		nSelect = i;
		break;
	}
	m_cmbConc2Trans.SetCurSel(nSelect);

	nSelect = 0;
	for(int i=0; i<m_cmbSteel.GetCount(); i++)
	{
		if(m_Data.MatlSteelK != m_cmbSteel.GetItemData(i)) continue;
		nSelect = i;
		break;
	}
	m_cmbSteel.SetCurSel(nSelect);

	m_nAxialForce = m_Data.nAxialForce;
	m_cmbLcom.ChangeSelect(m_Data.MpccLcom.AnalType,m_Data.MpccLcom.LoadCaseKey,m_Data.MpccLcom.MnMxType);

	m_edtAxialForce.SetEditUnit(m_Data.dAxialForce, 3);
	m_edtAxisAngle.SetEditUnit(m_Data.dAxisAngle);
	m_edtPoint.SetEditUnit(m_Data.nNumPoint);

	m_bIdealized = m_Data.bIdealize;
	m_bUserCurve = m_Data.bCurvature;
	m_edtUserCurve.SetEditUnit(m_Data.dCurvature);
	m_nIdealOpt = m_Data.nIdealOpt;

	UpdateData(FALSE);
}

BOOL CDgnSeisMPhiItemDlg::Dlg2Data()
{
	UpdateData(TRUE);
	
	m_edtName.GetWindowText(m_Data.strName);
	m_Data.bEvgp = m_bEvalGrup;
	m_Data.EvgpK = m_cmbEvalGrup.GetItemData(m_cmbEvalGrup.GetCurSel());
	m_Data.nEvgpColm = m_cmbEvalColm.GetCurSel();
	m_Data.nEvgpPos = m_cmbEvalPos.GetCurSel();
	m_Data.SectK = m_cmbSect.GetItemData(m_cmbSect.GetCurSel());
	m_Data.nPosition = m_cmbSectPos.GetCurSel();
	m_Data.MatlConcK1 = m_cmbConc1.GetItemData(m_cmbConc1.GetCurSel());
	m_Data.MatlConcK2 = m_cmbConc2.GetItemData(m_cmbConc2.GetCurSel());
	m_Data.MatlConcK1Trans = m_cmbConc1Trans.GetItemData(m_cmbConc1Trans.GetCurSel());
	m_Data.MatlConcK2Trans = m_cmbConc2Trans.GetItemData(m_cmbConc2Trans.GetCurSel());
	m_Data.MatlSteelK = m_cmbSteel.GetItemData(m_cmbSteel.GetCurSel());
	
	m_Data.nAxialForce = m_nAxialForce;

	UINT AnalType,LoadCaseKey,MnMxType; 
	m_cmbLcom.GetSelectedLoad(AnalType,LoadCaseKey,&MnMxType);
	m_Data.MpccLcom.AnalType = AnalType;
	m_Data.MpccLcom.LoadCaseKey = LoadCaseKey;
	m_Data.MpccLcom.MnMxType = MnMxType;

	m_Data.dAxialForce = m_edtAxialForce.GetEditValue();
	m_Data.dAxisAngle = m_edtAxisAngle.GetEditValue();
	m_Data.nNumPoint = m_edtPoint.GetEditValue();

	if(m_bEvalGrup)
	{
		m_Data.dAxisAngle = 0.0;
		m_Data.nNumPoint = 50;
	}

	m_Data.bIdealize = m_bIdealized;
	m_Data.bCurvature = m_bUserCurve;
	m_Data.dCurvature = m_edtUserCurve.GetEditValue();
	m_Data.nIdealOpt = m_nIdealOpt;
	
	return TRUE;
}

void CDgnSeisMPhiItemDlg::DrawSection()
{
	m_SectWnd.Set_SectPointer(&m_Data);
	m_SectWnd.Invalidate();
}

void CDgnSeisMPhiItemDlg::UpdateName()
{
	CString strName;

	CString strEvalGrup;
	CString strEvalColm;
	CString strEvalPos;

	m_cmbEvalGrup.GetWindowText(strEvalGrup);
	m_cmbEvalColm.GetWindowText(strEvalColm);
	m_cmbEvalPos.GetWindowText(strEvalPos);

	strName.Format(_T("%s_%s_%s"), strEvalGrup, strEvalColm, strEvalPos);

	m_edtName.SetWindowText(strName);

	UpdateData(FALSE);
}

void CDgnSeisMPhiItemDlg::UpdateColumnName()
{
	UpdateData(TRUE);

	if(!m_bEvalGrup) return;
	if(m_cmbEvalGrup.GetCount() == 0) return;

	T_EVGP_K EvgpK;
	T_EVGP_D EvgpD; EvgpD.Initialize();

	EvgpK = m_cmbEvalGrup.GetItemData(m_cmbEvalGrup.GetCurSel());
	if(EvgpK == 0) return;
	if(!m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD)) { ASSERT(0); return; }

	InitEvgpColmCombo();
	UpdateSectKey();

	UpdateData(FALSE);
}

void CDgnSeisMPhiItemDlg::UpdateSectKey()
{
	UpdateData(TRUE);

	if(!m_bEvalGrup) return;
	if(m_cmbEvalGrup.GetCount() == 0) return;

	T_EVGP_K EvgpK;
	T_EVGP_D EvgpD; EvgpD.Initialize();

	EvgpK = m_cmbEvalGrup.GetItemData(m_cmbEvalGrup.GetCurSel());
	if(EvgpK == 0) return;
	if(!m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD)) { ASSERT(0); return; }

	int nEvgpColm = m_cmbEvalColm.GetCurSel();
	int nEvgpPos = m_cmbEvalPos.GetCurSel();

	T_ELEM_K ElemK;
	T_ELEM_D ElemD; ElemD.Initialize();

	if(nEvgpPos == 0) ElemK = m_pDoc->m_pAttrCtrl2->GetTopElemK(EvgpD, nEvgpColm);
	else              ElemK = m_pDoc->m_pAttrCtrl2->GetBotElemK(EvgpD, nEvgpColm);

	if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(0); return; }

	m_Data.SectK = ElemD.elpro;

	CArray<T_SECT_K, T_SECT_K> aSectK;
	m_pDoc->m_pAttrCtrl->GetSectKeyList(aSectK);

	for(int i=0; i<aSectK.GetSize(); i++)
	{
		if(aSectK[i] == m_Data.SectK) 
		{ 
			m_cmbSect.SetCurSel(i); 
			break; 
		}
	}

	UpdateData(FALSE);
}

void CDgnSeisMPhiItemDlg::UpdateAxialForcebyLcom()
{
	UpdateData(TRUE);

	if(!m_pDoc->IsPostMode()) return;

	if(!m_bEvalGrup) return;
	if(m_cmbEvalGrup.GetCount() == 0) return;

	T_EVGP_K EvgpK;
	T_EVGP_D EvgpD; EvgpD.Initialize();

	EvgpK = m_cmbEvalGrup.GetItemData(m_cmbEvalGrup.GetCurSel());
	if(EvgpK == 0) return;
	if(!m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD)) { ASSERT(0); return; }
	
	int nEvgpColm = m_cmbEvalColm.GetCurSel();
	int nEvgpPos = m_cmbEvalPos.GetCurSel();

	T_ELEM_K ElemK;
	T_ELEM_D ElemD; ElemD.Initialize();
	
	if(nEvgpPos == 0) ElemK = m_pDoc->m_pAttrCtrl2->GetTopElemK(EvgpD, nEvgpColm);
	else              ElemK = m_pDoc->m_pAttrCtrl2->GetBotElemK(EvgpD, nEvgpColm);
	if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(0); return; }

	UINT nLoadType;
	UINT nLoadKey;
	UINT nMnMxType;
	if(!m_cmbLcom.GetSelectedLoad(nLoadType, nLoadKey, &nMnMxType)) { ASSERT(0); return; }

	T_LCOM_D LcomD; LcomD.Initialize();
	if(!m_pDoc->m_pPostCtrl->MakeLoadComb(nLoadType, nLoadKey, LcomD)) { ASSERT(0); return; }
	m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);

	T_STRB_D StrbD; StrbD.Initialize();
	T_STRB_D StrbDMax; StrbDMax.Initialize();
	T_STRB_D StrbDMin; StrbDMin.Initialize();

	if(!m_pDoc->m_pPostCtrl->GetStrbNew(ElemK, &StrbDMax, &StrbDMin, &StrbD, TRUE)) { ASSERT(0); return; }

	if     (nMnMxType == 1) StrbD = StrbDMax;
	else if(nMnMxType == 2) StrbD = StrbDMin;

	T_NODE_D NodeD1; NodeD1.Initialize();
	T_NODE_D NodeD2; NodeD2.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD1)) { ASSERT(0); return; }
	if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD2)) { ASSERT(0); return; }

	double dAxialForce = 0.0;
	if(nEvgpPos == 0)
	{
		if(NodeD1.z > NodeD2.z) dAxialForce = -StrbD.dblForce[0][0];
		else                    dAxialForce = -StrbD.dblForce[3][6];
	}
	else
	{
		if(NodeD1.z > NodeD2.z) dAxialForce = -StrbD.dblForce[3][6];
		else                    dAxialForce = -StrbD.dblForce[0][0];
	}	

	m_edtAxialForce.SetEditUnit(dAxialForce, 3);

	UpdateData(FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// CDgnSeisMPhiItemDlg implementation functions
BEGIN_MESSAGE_MAP(CDgnSeisMPhiItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnSeisMPhiItemDlg)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_EVAL_CHK,                  OnDgnSeisMPhiEvalGrupChk)	
	ON_CBN_SELCHANGE(IDC_DGN_SEIS_MPHI_EVAL_CMB,               OnDgnSeisMPhiEvalGrupCmb)
	ON_CBN_SELCHANGE(IDC_DGN_SEIS_MPHI_EVAL_COLM_CMB,          OnDgnSeisMPhiEvalColmCmb)
	ON_CBN_SELCHANGE(IDC_DGN_SEIS_MPHI_EVAL_POS_CMB,           OnDgnSeisMPhiEvalPosCmb)
	ON_CBN_SELCHANGE(IDC_DGN_SEIS_MPHI_SECT_CMB,               OnDgnSeisMPhiSectCmb)
	ON_CBN_SELCHANGE(IDC_DGN_SEIS_MPHI_SECT_POS_CMB,           OnDgnSeisMPhiSectPosCmb)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_REBAR_BTN,                 OnDgnSeisMPhiRebarBtn)
	ON_CBN_SELCHANGE(IDC_DGN_SEIS_MPHI_INELAS_CONC_CMB1,       OnDgnSeisMPhiConcCmb)
	ON_CBN_SELCHANGE(IDC_DGN_SEIS_MPHI_INELAS_CONC_TRANS_CMB1, OnDgnSeisMPhiConcTransCmb)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_AXIAL_FORCE_AUTO_RDO,      OnDgnSeisMPhiAxialForceRdo)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_AXIAL_FORCE_USER_RDO,      OnDgnSeisMPhiAxialForceRdo)
	ON_CBN_SELCHANGE(IDC_DGN_SEIS_MPHI_L_CASE_COMB_CMB,        OnDgnSeisMPhiLcomCmb)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_L_CASE_COMB_BTN,           OnDgnSeisMPhiLcomBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_IDEAL_CHK,                 OnDgnSeisMPhiIdealizedChk)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_USER_CHK,                  OnDgnSeisMPhiUserCurveChk)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_IDEAL_OPT_GUIDE_BTN,       OnDgnSeisMPhiOptGuideBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_ECUC_BTN,                  OnDgnSeisMPhiECUCBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_CURVE_BTN,                 OnDgnSeisMPhiBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_OK_BTN,                    OnDgnSeisMPhiOKBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_CANCEL_BTN,                OnDgnSeisMPhiCancelBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_APPLY_BTN,                 OnDgnSeisMPhiApplyBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_IMATL_BTN,                      OnIMatlBtn)  
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisMPhiItemDlg message handlers

BOOL CDgnSeisMPhiItemDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	m_chkSect.InitControl(this, CDgnSeisMPhiItemDlg::IDD, IDC_DGN_SEIS_MPHI_SECT_GRP, TRUE, TRUE);
		
	InitUnit();
	InitCombo();
	
	Data2Dlg();
	
	EnableDisableControls();

	if(!m_bModify)
	{
		Dlg2Data();
		UpdateName();
		UpdateColumnName();
		UpdateAxialForcebyLcom();
	}
	
	DrawSection();

	if(!m_pDoc->IsPostMode())
	{
		m_nAxialForce = 1;
		UpdateData(FALSE);
	}		
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisMPhiItemDlg::OnDgnSeisMPhiEvalGrupChk()
{
	UpdateData(TRUE);

	if(m_bEvalGrup)
	{
		double dAxisAngle = 0.0;
		int    nPoint = 50;
		m_edtAxisAngle.SetEditUnit(dAxisAngle);
		m_edtPoint.SetEditUnit(nPoint);
	}

	Dlg2Data();
	UpdateColumnName();

	EnableDisableControls();

	DrawSection();
}

void CDgnSeisMPhiItemDlg::OnDgnSeisMPhiEvalGrupCmb()
{
	Dlg2Data();
	UpdateName();
	UpdateColumnName();

	EnableDisableControls();

	UpdateAxialForcebyLcom();

	DrawSection();
}

void CDgnSeisMPhiItemDlg::OnDgnSeisMPhiEvalColmCmb()
{
	Dlg2Data();
	UpdateName();
	UpdateSectKey();

	EnableDisableControls();

	UpdateAxialForcebyLcom();

	DrawSection();
}

void CDgnSeisMPhiItemDlg::OnDgnSeisMPhiEvalPosCmb()
{
	Dlg2Data();
	UpdateName();
	UpdateSectKey();

	EnableDisableControls();

	UpdateAxialForcebyLcom();

	DrawSection();
}

void CDgnSeisMPhiItemDlg::OnDgnSeisMPhiSectCmb()
{
	Dlg2Data();
	DrawSection();
}

void CDgnSeisMPhiItemDlg::OnDgnSeisMPhiSectPosCmb()
{
	Dlg2Data();
	DrawSection();
}

void CDgnSeisMPhiItemDlg::OnDgnSeisMPhiRebarBtn()
{
	// Combo Box 에서 선택된 Section 이 DB 단면이면 Rebar Input Data for Column Section 호출
	// Combo Box 에서 선택된 Section 이 General 단면이면 Rebar Input Data for General Section 호출
	UpdateData(TRUE);

	T_SECT_D SectD;  SectD.Initialize();

	if(m_pDoc->m_pAttrCtrl->GetSect(m_Data.SectK, SectD))
	{
		if(SectD.nStype == D_SECT_TYPE_REGULAR || SectD.nStype == D_SECT_TYPE_TAPERED)
		{
			if(SectD.SectBefore.Shape == D_SECT_SHAPE_REG_B   || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_P    || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SR   || 
			   SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SB  || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_OCT  || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SOCT ||
			   SectD.SectBefore.Shape == D_SECT_SHAPE_REG_TRK || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_STRK || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_HTRK)
			{
				AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DGN_CON_REBC,0));
			}
			else if(SectD.SectBefore.Shape == D_SECT_SHAPE_REG_GEN)
			{ 
				AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_SECTION_REBAR_INPUT,0));
			}
		}
		else if(SectD.nStype == D_SECT_TYPE_USER && SectD.SectBefore.Shape == D_SECT_SHAPE_REG_GEN)
		{ 
			AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_SECTION_REBAR_INPUT,0)); 
		}
	}
}

void CDgnSeisMPhiItemDlg::OnDgnSeisMPhiConcCmb()
{
	Dlg2Data();

	EnableDisableControls();	

	DrawSection();
}

void CDgnSeisMPhiItemDlg::OnDgnSeisMPhiConcTransCmb()
{
	Dlg2Data();

	EnableDisableControls();	

	DrawSection();
}

void CDgnSeisMPhiItemDlg::OnDgnSeisMPhiAxialForceRdo()
{
	EnableDisableControls();
	
	UpdateAxialForcebyLcom();
}

void CDgnSeisMPhiItemDlg::OnDgnSeisMPhiLcomCmb()
{
	UpdateAxialForcebyLcom();
}

void CDgnSeisMPhiItemDlg::OnDgnSeisMPhiLcomBtn()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_MAIN_RIBBON_SEISEVAL_LOAD_COMB,0)); 
}

void CDgnSeisMPhiItemDlg::OnDgnSeisMPhiIdealizedChk()
{
	EnableDisableControls();
}

void CDgnSeisMPhiItemDlg::OnDgnSeisMPhiUserCurveChk()
{
	EnableDisableControls();
}

void CDgnSeisMPhiItemDlg::OnDgnSeisMPhiOptGuideBtn()
{
	UpdateData(TRUE);

	CDgnSeisMPhiIdealGuideDlg dlg;
	dlg.SetParamData(m_nIdealOpt);
	dlg.DoModal();
}

void CDgnSeisMPhiItemDlg::OnDgnSeisMPhiECUCBtn()
{
	CDgnMPhiCurveConditionDlg dlg;
	dlg.DoModal();
}

void CDgnSeisMPhiItemDlg::OnDgnSeisMPhiBtn()
{
	if(!Dlg2Data()) return;

	CDgnSeisMPhiCurveDlg dlg;
	dlg.SetParamData(m_Data);
	dlg.DoModal();
}

void CDgnSeisMPhiItemDlg::OnDgnSeisMPhiOKBtn()
{
	if(!Dlg2Data()) return;

	m_SyefD.nDgnType = 1;

	if(m_bModify)
	{
		// Syef 먼저
		if (!m_pDoc->m_pAttrCtrl2->ExistSyef(m_Key))
		{
			if(!m_pDoc->m_pDataCtrl->AddSyef(m_Key, m_SyefD)) { ASSERT(0); }
		}
		else
		{
			if(!m_pDoc->m_pDataCtrl->ModifySyef(m_Key, m_Key, m_SyefD)) { ASSERT(0); }
		}

		if(!m_pDoc->m_pAttrCtrl2->ExistMpcc(m_Key))  // ... 버튼으로 연 대화상자에서의 연관관계 처리로 인해 자기자신이 지워지면 Add 해 주어야 함
		{
			if(!m_pDoc->m_pDataCtrl->AddMpcc(m_Key, m_Data)) return;
		}
		else
		{
			if(!m_pDoc->m_pDataCtrl->ModifyMpcc(m_Key, m_Key, m_Data)) return;
		}
	}
	else
	{
		m_Key = m_pDoc->m_pAttrCtrl2->GetStartNumMpcc();
		// Syef 먼저
		if(!m_pDoc->m_pDataCtrl->AddSyef(m_Key, m_SyefD)) { ASSERT(0); }
		if(!m_pDoc->m_pDataCtrl->AddMpcc(m_Key, m_Data)) return;
	}

	CDialogMove::OnOK();
}

void CDgnSeisMPhiItemDlg::OnDgnSeisMPhiCancelBtn()
{
	CDialogMove::OnCancel();
}

void CDgnSeisMPhiItemDlg::OnDgnSeisMPhiApplyBtn()
{
	if(!Dlg2Data()) return;

	if(m_bModify)
	{
		// Syef 먼저
		if(!m_pDoc->m_pAttrCtrl2->ExistSyef(m_Key))
		{
			if(!m_pDoc->m_pDataCtrl->AddSyef(m_Key, m_SyefD)) { ASSERT(0); }
		}
		else
		{
			if(!m_pDoc->m_pDataCtrl->ModifySyef(m_Key, m_Key, m_SyefD)) { ASSERT(0); }
		}

		if(!m_pDoc->m_pAttrCtrl2->ExistMpcc(m_Key))
		{
			if(!m_pDoc->m_pDataCtrl->AddMpcc(m_Key, m_Data)) return;
		}
		else
		{
			if(!m_pDoc->m_pDataCtrl->ModifyMpcc(m_Key, m_Key, m_Data)) return;
		}
	}
	else
	{
		m_Key = m_pDoc->m_pAttrCtrl2->GetStartNumMpcc();
		// Syef 먼저
		if(!m_pDoc->m_pDataCtrl->AddSyef(m_Key, m_SyefD)) { ASSERT(0); }
		if(!m_pDoc->m_pDataCtrl->AddMpcc(m_Key, m_Data)) return;
	}
}

void CDgnSeisMPhiItemDlg::OnIMatlBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,MAKEWPARAM(ID_STRUCT_PROP_FIBER_PROP_SEIS_CVL,0));
	SetInMatlCobx();
}

LRESULT CDgnSeisMPhiItemDlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	return 0L;
}
