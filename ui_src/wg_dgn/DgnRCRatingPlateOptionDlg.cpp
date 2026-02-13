// DgnRCRatingPlateOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRCRatingPlateOptionDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnRCRatingPlateOptionDlg dialog

CDgnRCRatingPlateOptionDlg::CDgnRCRatingPlateOptionDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnRCRatingPlateOptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnRCRatingPlateOptionDlg)
	m_nOption = 0;	
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();

	m_aDroop.RemoveAll();
	m_aDroop.Add(IDC_DGN_DROOP_STC);
	m_aDroop.Add(IDC_DGN_DROOP_EDT);
	m_aDroop.Add(IDC_DGN_DROOP_UNT);
	
	m_aStrain.RemoveAll();
	m_aStrain.Add(IDC_DGN_STRAIN_STC);
	m_aStrain.Add(IDC_DGN_STRAIN_EDT);
	m_aStrain.Add(IDC_DGN_STRAIN_UNT);
}


void CDgnRCRatingPlateOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnRCRatingPlateOptionDlg)
	DDX_Radio  (pDX, IDC_DGN_OPTION_ADD_RDO,          m_nOption);
	DDX_Control(pDX, IDC_DGN_DEFINE_MEMB_CMB,         m_cmbMemb);
	DDX_Control(pDX, IDC_DGN_DROOP_EDT,               m_edtDroop);
	DDX_Control(pDX, IDC_DGN_DROOP_UNT,               m_untDroop);
	DDX_Control(pDX, IDC_DGN_STRAIN_EDT,              m_edtStrain);
	DDX_Control(pDX, IDC_DGN_IMPACT_FACT_EDT,         m_edtImpact);
	DDX_Control(pDX, IDC_DGN_ADD_RESPONSE_FACT_EDT,   m_edtAddResponseF);
	//}}AFX_DATA_MAP
}

void CDgnRCRatingPlateOptionDlg::InitUnit()
{
	m_edtDroop.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_untDroop.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_edtStrain.SetUnitType(D_UNITSYS_NONE);	
	m_edtImpact.SetUnitType(D_UNITSYS_NONE);	
	m_edtAddResponseF.SetUnitType(D_UNITSYS_NONE);	

	CString szTemp;
	szTemp.Format(_T("%s%s"), _LS(IDS_DGN_CHAR_MU), _LS(IDS_DGN_CHAR_EPSILON));
	GetDlgItem(IDC_DGN_STRAIN_UNT)->SetWindowText(szTemp);
}

void CDgnRCRatingPlateOptionDlg::InitCombo()
{
	m_cmbMemb.ResetContent();

	CString strMemb;
	T_RRPL_D RrplD; RrplD.Initialize();
	CArray<T_RRPL_K, T_RRPL_K> aRrplK; aRrplK.RemoveAll();
	m_pDoc->m_pAttrCtrl2->GetRrplKeyList(aRrplK);

	for(int i=0; i<aRrplK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl2->GetRrpl(aRrplK[i], RrplD)) continue;
		strMemb.Format(_T("%s"), RrplD.strName);
		m_cmbMemb.SetItemData(m_cmbMemb.AddString(strMemb), aRrplK[i]);
	}

	m_cmbMemb.SetCurSel(0);
}

void CDgnRCRatingPlateOptionDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistX = 0;
	int nDistY = 0;	

	GetDlgItem(IDC_DGN_DROOP_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_STRAIN_STC)->GetWindowRect(rToMove);

	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aStrain, nDistX, nDistY);
}

void CDgnRCRatingPlateOptionDlg::ControlsShowHide()
{
	// RC Rating Code
	T_RCDC_D RcdcD; RcdcD.Initialize();	
	m_pDoc->m_pAttrCtrl2->GetDgnRcdc(RcdcD);

	T_RPAC_D RpacD;
	if(!m_pDoc->m_pAttrCtrl2->GetRpac(RpacD)) RpacD.Initialize(RcdcD.nRatingCode);

	CDlgUtil::CtrlShowHide(this, m_aDroop,  RpacD.nSurveyMeth==0);
	CDlgUtil::CtrlShowHide(this, m_aStrain, RpacD.nSurveyMeth==1);
}

void CDgnRCRatingPlateOptionDlg::EnableDisableControls()
{
	CDlgUtil::CtrlEnableDisableByRect(this, IDC_FACTOR_GRP, m_nOption==0, TRUE);
}

BOOL CDgnRCRatingPlateOptionDlg::Dlg2Data(T_RROP_K &rKey, T_RROP_D &rData)
{
	UpdateData(TRUE);

	rKey = m_cmbMemb.GetItemData(m_cmbMemb.GetCurSel());

	rData.nRatingChk = 1;
	rData.dDefl = m_edtDroop.GetEditValue();
	rData.dStrain = m_edtStrain.GetEditValue();
	rData.dImpactFactor = m_edtImpact.GetEditValue();
	rData.dAdjustmentFactor = m_edtAddResponseF.GetEditValue();

	// 필요시 Error Check 추가
	return TRUE;
}

void CDgnRCRatingPlateOptionDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DEFAULT:
	case D_UPDATE_BUFFER_BEFORE:
	case D_UPDATE_UNIT:
		// do something...
		break;
	case D_UPDATE_BUFFER_AFTER:
		{
			UpdateBuffer();
			break;
		}
	default:
		//ASSERT(FALSE)->ASSERT(TRUE)로 임시변경 ('2000.2.10)
		ASSERT(TRUE);
	}
}

void CDgnRCRatingPlateOptionDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bSpfcChanged = FALSE;
	BOOL bStorChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		switch(nCmd)
		{
		case(UR_RACD_ADD): // RC Rating Code
		case(UR_RACD_DEL): 
			{				
				// 현재는 코드 변경시 특별한 동작이 없지만 다른 기준이 추가 되면 이부분 수정 필요
				return;
			}		
			break;
		case (UR_RPAC_ADD): // RC Rating Param.
		case (UR_RPAC_DEL):
			{
				ControlsShowHide();
				EnableDisableControls();
			}
			break;
		case (UR_RRPL_ADD):
		case (UR_RRPL_DEL):
			{
				InitCombo();
			}
			break;
		default:
			break;
		}
	}
}


BEGIN_MESSAGE_MAP(CDgnRCRatingPlateOptionDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnRCRatingPlateOptionDlg)
// 	ON_BN_CLICKED(IDC_DGN_EXECUTE,            OnDgnExecute)
// 	ON_BN_CLICKED(IDC_DGN_CLOSE,              OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_OPTION_ADD_RDO,     OnDgnAddBtn)
	ON_BN_CLICKED(IDC_DGN_OPTION_DEL_RDO,     OnDgnDelBtn)
	ON_CBN_SELCHANGE(IDC_DGN_DEFINE_MEMB_CMB, OnDgnMembCmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnRCRatingPlateOptionDlg message handlers

BOOL CDgnRCRatingPlateOptionDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	InitUnit();
	InitCombo();
	AlignControl();
	ControlsShowHide();
	EnableDisableControls();

	OnDgnMembCmb();

	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnRCRatingPlateOptionDlg::Execute()
{
	T_RROP_K Key;  Key = 0;
	T_RROP_D Data; Data.Initialize();

	if(!Dlg2Data(Key, Data)) return;

	if(m_nOption == 0) 
	{
		if(m_pDoc->m_pAttrCtrl2->ExistRrop(Key))
		{
			m_pDoc->m_pDataCtrl->ModifyRrop(Key, Key, Data);
		}
		else
		{
			m_pDoc->m_pDataCtrl->AddRrop(Key, Data);
		}
	}
	else
	{
		m_pDoc->m_pDataCtrl->DelRrop(Key);
	}
}

// void CDgnRCRatingPlateOptionDlg::OnDgnClose() 
// {
// 	// TODO: Add your control notification handler code here
// 	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
// }

void CDgnRCRatingPlateOptionDlg::OnDgnAddBtn() 
{
	UpdateData(TRUE);
	EnableDisableControls();
}

void CDgnRCRatingPlateOptionDlg::OnDgnDelBtn() 
{
	UpdateData(TRUE);
	EnableDisableControls();
}

void CDgnRCRatingPlateOptionDlg::OnDgnMembCmb() 
{
	UpdateData(TRUE);

	T_RROP_K Key;
	Key = m_cmbMemb.GetItemData(m_cmbMemb.GetCurSel());

	T_RRPL_D RrplD; RrplD.Initialize();
	if(!m_pDoc->m_pAttrCtrl2->GetRrpl(Key, RrplD)) return;

	CArray<T_ELEM_K, T_ELEM_K> aSelElem;  aSelElem.RemoveAll();
	aSelElem.Copy(RrplD.arElemK);
	m_pDoc->m_pViewCtrl->SelectElem(NULL, aSelElem, FALSE, TRUE);
}