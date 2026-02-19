// CDgnSeisAnchorEvalItem2NewSubArrangeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchorEvalItem2NewSubArrangeDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItem2NewSubArrangeDlg dialog

CDgnSeisAnchorEvalItem2NewSubArrangeDlg::CDgnSeisAnchorEvalItem2NewSubArrangeDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnSeisAnchorEvalItem2NewSubArrangeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisAnchorEvalItem2NewSubArrangeDlg)	
	//}}AFX_DATA_INIT
	m_pParent = (CDgnSeisAnchorEvalItem2NewDlg*)pParent;

	m_bRelAngle = FALSE;

	m_pDoc = CDBDoc::GetDocPoint();

	m_pGrid = new CDgnSeisAnchor2ArrangeGrid(this);

	m_IsGuideView = FALSE;
}

void CDgnSeisAnchorEvalItem2NewSubArrangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisAnchorEvalItem2NewSubArrangeDlg)

	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_B_EDT, m_edtB);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_B_UNT, m_untB);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_H_EDT, m_edtH);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_H_UNT, m_untH);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_RELANGLE_EDT, m_edtRelAngle); 
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_RELANGLE_UNT, m_untRelAngle);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_THETA_1_EDT, m_edtTheta1);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_THETA_1_UNT, m_untTheta1);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_THETA_2_EDT, m_edtTheta2);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_THETA_2_UNT, m_untTheta2);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_THETA_COPING_EDT, m_edtThetaCoping);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_THETA_COPING_UNT, m_untThetaCoping);

	DDX_Check(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_RELANGLE_CHK, m_bRelAngle);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_ARRANGE2_GRID, *m_pGrid);

	//}}AFX_DATA_MAP
}


void CDgnSeisAnchorEvalItem2NewSubArrangeDlg::SetArrangeData(T_ANEV_D* pData)
{
	m_pData = pData;
	m_AnevD = *pData;
	//CalRelAngle();
}

void CDgnSeisAnchorEvalItem2NewSubArrangeDlg::EnableCtrl()
{
	if (m_bRelAngle == TRUE)  // User
	{
		GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_RELANGLE_EDT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_SEIS_ANCHOR_EVAL_RELANGLE_EDT)->EnableWindow(FALSE);
	}

	GetDlgItem(IDC_DGN_SEIS_SHOW_PREVIEW_BTN)->EnableWindow(!m_IsGuideView);
}

void CDgnSeisAnchorEvalItem2NewSubArrangeDlg::CalRelAngle()
{
	if (!m_bRelAngle) 
	{
		double dAlpha = m_pDoc->m_pAttrCtrl2->GetAlpha(m_pData->BrevK);
		m_edtRelAngle.SetEditUnit(dAlpha);
	}

	if(!Dlg2Data()) return;
	m_pParent->SetViewData();
}

void CDgnSeisAnchorEvalItem2NewSubArrangeDlg::InitUnit()
{
	m_edtB.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untB.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtH.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untH.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtRelAngle.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dRelAngle);
	m_untRelAngle.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dRelAngle);
	m_edtTheta1.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dTheta1);
	m_untTheta1.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dTheta1);
	m_edtTheta2.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dTheta2);
	m_untTheta2.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dTheta2);
	m_edtThetaCoping.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dCopingAngle);
	m_untThetaCoping.SetUnitType(CUnitCtrl::m_ANEV_UNIT.dCopingAngle);
}

void CDgnSeisAnchorEvalItem2NewSubArrangeDlg::Data2Dlg() 
{ 
	m_bRelAngle = m_pData->bRelAngle;
	m_edtRelAngle.SetEditUnit(m_pData->dRelAngle);
	m_edtTheta1.SetEditUnit(m_pData->dTheta1);
	m_edtTheta2.SetEditUnit(m_pData->dTheta2);
	m_edtB.SetEditUnit(m_pData->dB);
	m_edtH.SetEditUnit(m_pData->dH);
	m_edtThetaCoping.SetEditUnit(m_pData->dCopingAngle);

	//m_pGrid->SetData2Grid(*m_pData);
	m_pGrid->MakeItemEx();

	UpdateData(FALSE);

	CalRelAngle();
}

BOOL CDgnSeisAnchorEvalItem2NewSubArrangeDlg::Dlg2Data(BOOL bIgnoreUpdateData/* = FALSE*/)
{
	if(!bIgnoreUpdateData)
	{
		UpdateData(TRUE);
	}

	m_pData->bRelAngle = m_bRelAngle;
	m_pData->dRelAngle = m_edtRelAngle.GetEditValue();
	m_pData->dTheta1 = m_edtTheta1.GetEditValue();
	m_pData->dTheta2 = m_edtTheta2.GetEditValue();
	m_pData->dB = m_edtB.GetEditValue();
	m_pData->dH = m_edtH.GetEditValue();
	m_pData->dCopingAngle = m_edtThetaCoping.GetEditValue();

	if(m_pData->aLayer.GetSize() == 0)
	{
		m_pData->BearingProp.Initialize();
		m_pData->SuppType.Initialize();
		m_pData->TensChk.Initialize();
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	// 비어있는 곳 채움.  - 채우면 그림이 안그려짐 ㅜㅜ 우선 안채우고 넘김
// 	CDBDoc* pDoc = CDBDoc::GetDocPoint();
// 	pDoc->m_pAttrCtrl2->GetModifiedAnevD(m_pData);
	//////////////////////////////////////////////////////////////////////////

	CArray<double, double> aSay; aSay.RemoveAll();
 	T_ANEV_LAYER_D Layer1, Layer2;
	if(m_pData->aLayer.GetSize()==1)
	{
		aSay.Add(0.0);
	}
	else
	{
		for (int i = 0; i < m_pData->aLayer.GetSize() - 1; i++)
		{
			Layer1 = m_pData->aLayer[i];
			Layer2 = m_pData->aLayer[i + 1];
			double dSay = fabs(Layer2.dSay - Layer1.dSay);
			aSay.Add(dSay);
		}
	}

 	// 임시로 값 채움. - 개선 필요!!
 	// 계산 과정 중에서 m_pData->aSax, m_pData->aSay를 사용하는 부분에 대한 검토가 필요!!
 	//m_pData->aSax.Copy(aSax);
 	//m_pData->aSay.Copy(aSay);

	T_ANEV_TENS_CHK_D OutTens;
	T_ANEV_REG_SUPP_TYPE_D SuppType; SuppType.Initialize();
	if(!m_pDoc->m_pAttrCtrl2->SetAnevSupportNew2(m_pData->BrevK, m_pData->aLayer, m_pData->SuppType, SuppType, OutTens))
	{
		for(int nN2=0; nN2<OutTens.aElnkNodeKs.GetSize(); nN2++)
		{
			for(int nN1=0; nN1<OutTens.aElnkNodeKs[nN2].aNodeK1.GetSize(); nN1++)
			{
				BOOL bNg = FALSE;
				if (OutTens.aElnkNodeKs[nN2].aNodeK1[nN1] == 0) bNg = TRUE;
				if (OutTens.aElnkNodeKs[nN2].aNodeK2[nN1] == 0) bNg = TRUE;
				
				if(bNg)
				{
					//AfxMessageBox(_T("Bearing 배치가 Link정보와 맞지 않습니다."));
					//return FALSE;
				}
			}
		}
	}
	m_pData->SuppType.Initialize();
	m_pData->SuppType = SuppType;

	T_ANEV_BRPR_D BearingProp; BearingProp.Initialize();
	m_pDoc->m_pAttrCtrl2->SetAnevPropertyNew2(m_pData->aLayer, m_pData->BearingProp, BearingProp);
	m_pData->BearingProp.Initialize();
	m_pData->BearingProp = BearingProp;

	if(m_pData->bTensChk)
	{
		T_ANEV_TENS_CHK_D TensChk; TensChk.Initialize();
		if(!m_pDoc->m_pAttrCtrl2->SetAnevTensionNew2(m_pData->aLayer, m_pData->BrevK, m_pData->TensChk, TensChk))
			return FALSE;
		m_pData->TensChk.Initialize();
		m_pData->TensChk = TensChk;
	}

	return TRUE;
}

void CDgnSeisAnchorEvalItem2NewSubArrangeDlg::SetViewData()
{
	if(!Dlg2Data()) return;
	m_pParent->SetViewData();
}

void CDgnSeisAnchorEvalItem2NewSubArrangeDlg::ModifyLayerData()
{
	for(int i = 0; i< m_pData->aLayer.GetSize(); i++)
	{
		T_ANEV_LAYER_D Layer = m_pData->aLayer[i];

		double dSaxSum = 0.0;
		for (int k = 0; k < Layer.aSax.GetSize(); k++)
		{
			dSaxSum += Layer.aSax[k];
		}
		Layer.dD1_q = m_pData->dB - (dSaxSum + Layer.dD1);

		double dY = m_pData->dH - (Layer.dD2 + Layer.dD2_q);
		Layer.dD2   += dY/2;
		Layer.dD2_q += dY/2;

		m_pData->aLayer[i] = Layer;
	}
}

BEGIN_MESSAGE_MAP(CDgnSeisAnchorEvalItem2NewSubArrangeDlg, CChildDialog)
	//{{AFX_MSG_MAP(CCMSectionManagerStiffnessDlg)

	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_RELANGLE_EDT, OnSeisAnchorEvalPropRelAngle)
	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_THETA_1_EDT, OnSeisAnchorEvalPropTheta1Edt)
	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_THETA_2_EDT, OnSeisAnchorEvalPropTheta2Edt)
	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_B_EDT, OnSeisAnchorEvalPropBEdt)
	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_H_EDT, OnSeisAnchorEvalPropHEdt)
	ON_EN_CHANGE(IDC_DGN_SEIS_ANCHOR_EVAL_THETA_COPING_EDT, OnSeisAnchorEvalPropThetaCopingEdt)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_RELANGLE_CHK, OnSeisAnchorEvalRelAngleChk)
	ON_BN_CLICKED(IDC_DGN_SEIS_SHOW_PREVIEW_BTN, OnSeisAnchorEvalGuideBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItem2NewSubArrangeDlg message handlers

BOOL CDgnSeisAnchorEvalItem2NewSubArrangeDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	InitUnit();

	m_pGrid->Initialize(m_pData);


	Data2Dlg();
	EnableCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDgnSeisAnchorEvalItem2NewSubArrangeDlg::OnSeisAnchorEvalPropRelAngle()
{
	if(!Dlg2Data()) return;
	m_pParent->SetViewData();
}

void CDgnSeisAnchorEvalItem2NewSubArrangeDlg::OnSeisAnchorEvalPropTheta1Edt()
{
	if(!Dlg2Data()) return;
	m_pParent->SetViewData();
}

void CDgnSeisAnchorEvalItem2NewSubArrangeDlg::OnSeisAnchorEvalPropTheta2Edt()
{
	if(!Dlg2Data()) return;
	m_pParent->SetViewData();
}

void CDgnSeisAnchorEvalItem2NewSubArrangeDlg::OnSeisAnchorEvalPropBEdt()
{
	if(!Dlg2Data()) return;
	ModifyLayerData();
	m_pGrid->MakeItemEx();
	m_pParent->SetViewData();
}

void CDgnSeisAnchorEvalItem2NewSubArrangeDlg::OnSeisAnchorEvalPropHEdt()
{
	if(!Dlg2Data()) return;
	ModifyLayerData();
	m_pGrid->MakeItemEx();
	m_pParent->SetViewData();
}

void CDgnSeisAnchorEvalItem2NewSubArrangeDlg::OnSeisAnchorEvalPropThetaCopingEdt()
{
	if(!Dlg2Data()) return;
	m_pParent->SetViewData();
}

void CDgnSeisAnchorEvalItem2NewSubArrangeDlg::OnSeisAnchorEvalRelAngleChk()
{
	UpdateData(TRUE);
    
	EnableCtrl();
	CalRelAngle();
}

void CDgnSeisAnchorEvalItem2NewSubArrangeDlg::OnSeisAnchorEvalGuideBtn()
{
	m_pGuideDlg = new CDgnSeisAnchorEvalGuide2Dlg(this, 1);
	m_pGuideDlg->SetInitPos(D_INIT_POS_RT);
	m_pGuideDlg->Create(IDD_DGN_SEIS_ANCHOR_EVAL_GUIDE2_DLG, this);
	m_pGuideDlg->ShowWindow(SW_SHOW);

	m_IsGuideView = TRUE;
	EnableCtrl();
}

void CDgnSeisAnchorEvalItem2NewSubArrangeDlg::OnCloseGuideDlg()
{
	if (m_pGuideDlg != NULL)
	{
		m_pGuideDlg->DestroyWindow();
		m_pGuideDlg = NULL;

		m_IsGuideView = FALSE;
	}
	EnableCtrl();
}
