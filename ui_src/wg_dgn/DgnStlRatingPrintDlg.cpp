// DgnStlRationPrintDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlRatingPrintDlg.h"

#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "..\wg_main\wg_mainres2.h"	

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnStlRationPrintDlg dialog
const int ELEM_TYPE_ELEMENT = 0;
const int ELEM_TYPE_VBEAM = 1;
const int OPTION_ADD_REPLACE = 0;
const int OPTION_DELETE = 1;

CDgnStlRationPrintDlg::CDgnStlRationPrintDlg(CWnd* pParent)
: CMenuBarChildDlg(CDgnStlRationPrintDlg::IDD, pParent)
{
	m_nOption  = OPTION_ADD_REPLACE;
	m_nElemType = ELEM_TYPE_ELEMENT;
	m_nPart    = 0;
	m_nFlexure = 0;
	m_nShear   = 0;
	m_nService = 0;

	m_pDoc = CDBDoc::GetDocPoint();	
}

void CDgnStlRationPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_DGN_AR,                 m_nOption);
	DDX_Radio(pDX, IDC_DGN_CPG_ELEM_RDO, m_nElemType);
	DDX_Radio(pDX, IDC_DGN_I_RADIO,            m_nPart);
	DDX_Radio(pDX, IDC_FILTER_FLEXURE_ALL_RDO, m_nFlexure);
	DDX_Radio(pDX, IDC_FILTER_SHEAR_ALL_RDO,   m_nShear);
	DDX_Radio(pDX, IDC_FILTER_SERVICE_ALL_RDO, m_nService);
	DDX_Control(pDX, IDC_FILTER_FLEXURE_GRP_CMB, m_cmbFlexure);
	DDX_Control(pDX, IDC_FILTER_SHEAR_GRP_CMB,   m_cmbShear);
	DDX_Control(pDX, IDC_FILTER_SERVICE_GRP_CMB, m_cmbService);

	DDX_Check(pDX, IDC_FATIGUE_MIN_CHK,  m_bFat_Min );
	DDX_Check(pDX, IDC_FATIGUE_EVL1_CHK, m_bFat_EVL1);
	DDX_Check(pDX, IDC_FATIGUE_EVL2_CHK, m_bFat_EVL2);
	DDX_Check(pDX, IDC_FATIGUE_MEAN_CHK, m_bFat_Mean);
}

void CDgnStlRationPrintDlg::InitCtrl()
{
	CArray<T_GRUP_K, T_GRUP_K> GroupKeyList;
	m_pDoc->m_pAttrCtrl->GetGrupKeyList(GroupKeyList);

	T_GRUP_D GroupData;	GroupData.Initialize();
	
	for(int i=0; i<GroupKeyList.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetGrup(GroupKeyList.GetAt(i), GroupData);
		m_cmbFlexure.AddString(GroupData.GroupName);
		m_cmbShear  .AddString(GroupData.GroupName);
		m_cmbService.AddString(GroupData.GroupName);
	}

	m_cmbFlexure.SetCurSel(0);
	m_cmbShear.SetCurSel(0);
	m_cmbService.SetCurSel(0);

	m_aElemTypeCtrl.RemoveAll();
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_TYPE_GRP);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_RDO);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_VELEM_RDO);

	m_aPositionCtrl.RemoveAll();
	m_aPositionCtrl.Add(IDC_POSITION_GROUP);
	m_aPositionCtrl.Add(IDC_DGN_I_RADIO);
	m_aPositionCtrl.Add(IDC_DGN_J_RADIO);
	m_aPositionCtrl.Add(IDC_DGN_IJ_RADIO);
	m_aPositionCtrl.Add(IDC_FILTER_GRP);
	m_aPositionCtrl.Add(IDC_FILTER_FLEXURE_GRP);
	m_aPositionCtrl.Add(IDC_FILTER_FLEXURE_ALL_RDO);
	m_aPositionCtrl.Add(IDC_FILTER_FLEXURE_GRP_RDO);
	m_aPositionCtrl.Add(IDC_FILTER_FLEXURE_GRP_CMB);
	m_aPositionCtrl.Add(IDC_FILTER_SHEAR_GRP);
	m_aPositionCtrl.Add(IDC_FILTER_SHEAR_ALL_RDO);
	m_aPositionCtrl.Add(IDC_FILTER_SHEAR_GRP_RDO);
	m_aPositionCtrl.Add(IDC_FILTER_SHEAR_GRP_CMB);
	m_aPositionCtrl.Add(IDC_FILTER_SERVICE_GRP);
	m_aPositionCtrl.Add(IDC_FILTER_SERVICE_ALL_RDO);
	m_aPositionCtrl.Add(IDC_FILTER_SERVICE_GRP_RDO);
	m_aPositionCtrl.Add(IDC_FILTER_SERVICE_GRP_CMB);
	m_aPositionCtrl.Add(IDC_FATIGUE_GROUP);
	m_aPositionCtrl.Add(IDC_FATIGUE_MIN_CHK);
	m_aPositionCtrl.Add(IDC_FATIGUE_EVL1_CHK);
	m_aPositionCtrl.Add(IDC_FATIGUE_EVL2_CHK);
	m_aPositionCtrl.Add(IDC_FATIGUE_MEAN_CHK);
	m_aPositionCtrl.Add(IDC_DGN_EXECUTE);
	m_aPositionCtrl.Add(IDC_DGN_CLOSE);
}

void CDgnStlRationPrintDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY=0;

	T_RCDS_D RcdsD; RcdsD.Initialize();	
	m_pDoc->m_pAttrCtrl2->GetDgnRcds(RcdsD);

	GetDlgItem(IDC_DGN_CPG_ELEM_TYPE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_POSITION_GROUP)->GetWindowRect(rToMove);
	int nDistX = rRef.left - rToMove.left;
	if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || RcdsD.nRatingCode != STL_AASHTO_LRFD19)
	{
		nDistY = rRef.top - rToMove.top;
	}
	else
	{
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(4);
	}

	CDlgUtil::CtrlMoveDistXY(this, m_aPositionCtrl, nDistX, nDistY);

	CArray<UINT, UINT> aControls; aControls.RemoveAll();
	aControls.Add(IDC_DGN_EXECUTE);
	aControls.Add(IDC_DGN_CLOSE);

	// show hide
	CArray<UINT, UINT> aControlsFat; aControlsFat.RemoveAll();
	aControlsFat.Add(IDC_FATIGUE_GROUP);
	aControlsFat.Add(IDC_FATIGUE_MIN_CHK);
	aControlsFat.Add(IDC_FATIGUE_EVL1_CHK);
	aControlsFat.Add(IDC_FATIGUE_EVL2_CHK);
	aControlsFat.Add(IDC_FATIGUE_MEAN_CHK);
	CDlgUtil::CtrlShowHide(this, aControlsFat,RcdsD.nRatingCode==STL_AASHTO_LRFD19);

	if(  RcdsD.nRatingCode==STL_AASHTO_LRFD19 )
	{
		GetDlgItem(IDC_FATIGUE_GROUP)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_EXECUTE)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
	}
	else if( RcdsD.nRatingCode==STL_AASHTO_LRFD12)
	{
		GetDlgItem(IDC_FILTER_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_EXECUTE)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
	}
	else
	{
		GetDlgItem(IDC_POSITION_GROUP)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_EXECUTE)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
	}
}

void CDgnStlRationPrintDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlEnableDisable(this, m_aPositionCtrl, (m_nOption == OPTION_ADD_REPLACE));

	if(m_nOption == OPTION_ADD_REPLACE)
	{
		CDlgUtil::CtrlEnableDisableByRect(this, IDC_FILTER_GRP,     TRUE, TRUE);

		m_cmbFlexure.EnableWindow(m_nFlexure==1);
		m_cmbShear.EnableWindow(m_nShear==1);
		m_cmbService.EnableWindow(m_nService==1);
	}
	else
	{

		CDlgUtil::CtrlEnableDisableByRect(this, IDC_FILTER_GRP,     FALSE, TRUE);
	}
	
	GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(TRUE);
	GetDlgItem(IDC_DGN_CLOSE)->EnableWindow(TRUE);
}

void CDgnStlRationPrintDlg::ControlsShowHide()
{
	T_RCDS_D RcdsD; RcdsD.Initialize();	
	m_pDoc->m_pAttrCtrl2->GetDgnRcds(RcdsD);

	if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || RcdsD.nRatingCode != STL_AASHTO_LRFD19)
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_HIDE);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_SHOW);
	}

	CDlgUtil::CtrlShowHideByRect(this, IDC_FILTER_GRP, (RcdsD.nRatingCode==STL_AASHTO_LRFD12 || RcdsD.nRatingCode==STL_AASHTO_LRFD19), TRUE);

	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_CLOSE  )->ShowWindow(SW_SHOW);
}

BEGIN_MESSAGE_MAP(CDgnStlRationPrintDlg, CMenuBarChildDlg)
	ON_BN_CLICKED(IDC_DGN_AR,                 OnDgnArDel)
	ON_BN_CLICKED(IDC_DGN_DEL,                OnDgnArDel)	
	ON_BN_CLICKED(IDC_FILTER_FLEXURE_ALL_RDO, OnDgnFlexureRdo)	
	ON_BN_CLICKED(IDC_FILTER_FLEXURE_GRP_RDO, OnDgnFlexureRdo)	
	ON_BN_CLICKED(IDC_FILTER_SHEAR_ALL_RDO,   OnDgnShearRdo)	
	ON_BN_CLICKED(IDC_FILTER_SHEAR_GRP_RDO,   OnDgnShearRdo)	
	ON_BN_CLICKED(IDC_FILTER_SERVICE_ALL_RDO, OnDgnServiceRdo)	
	ON_BN_CLICKED(IDC_FILTER_SERVICE_GRP_RDO, OnDgnServiceRdo)
	ON_BN_CLICKED(IDC_DGN_CPG_ELEM_RDO, OnDgnCPGElemTypeRdo)
	ON_BN_CLICKED(IDC_DGN_CPG_VELEM_RDO, OnDgnCPGElemTypeRdo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlRationPrintDlg message handlers

BOOL CDgnStlRationPrintDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();	
	
	InitCtrl();
	AlignControl();
	EnableDisableControls();
	ControlsShowHide();

	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnStlRationPrintDlg::Execute()
{
	auto SetCommonData = [this](auto& rData) {

		UpdateData(TRUE);
		
		rData.nPrintChk = m_nPart;
		rData.nFlexure = m_nFlexure;
		rData.nShear = m_nShear;
		rData.nService = m_nService;

		CString strGroup = _T("");
		m_cmbFlexure.GetLBText(m_cmbFlexure.GetCurSel(), strGroup);
		rData.nFlexure_G = m_pDoc->m_pAttrCtrl->GetGrupKey(strGroup);
		m_cmbShear.GetLBText(m_cmbShear.GetCurSel(), strGroup);
		rData.nShear_G = m_pDoc->m_pAttrCtrl->GetGrupKey(strGroup);
		m_cmbService.GetLBText(m_cmbService.GetCurSel(), strGroup);
		rData.nService_G = m_pDoc->m_pAttrCtrl->GetGrupKey(strGroup);
		rData.bFaT_MIN = m_bFat_Min;
		rData.bFaT_EVL1 = m_bFat_EVL1;
		rData.bFaT_EVL2 = m_bFat_EVL2;
		rData.bFaT_MEAN = m_bFat_Mean;
	};

	auto ExcuteAddReplace = [this, &SetCommonData](auto& selKeys, bool isElement) {
		if (isElement)
		{
			T_RPRS_D rData;
			rData.Initialize();
			SetCommonData(rData);
			if (m_pDoc->m_pDataCtrl->AddRprs(selKeys, rData)) Initial_SelectItem();
		}
		else
		{
			T_RPRV_D rData;
			rData.Initialize();
			SetCommonData(rData);
			if (m_pDoc->m_pDataCtrl->AddRprv(selKeys, rData)) Initial_SelectItem();
		}
	};

	auto ExcuteDelete = [this](auto& selKeys, bool isElement) {
		bool success = isElement ?
			m_pDoc->m_pDataCtrl->DelRprs(selKeys) :
			m_pDoc->m_pDataCtrl->DelRprv(selKeys);
		if (success) Initial_SelectItem();
	};

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;

	if (m_nElemType == ELEM_TYPE_ELEMENT)
	{
		pIGM->GetSelectedElemKeyListForDgn(aSelKey);

		if (aSelKey.GetSize() == 0) {
			AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
			return;
		}
		if (m_nOption == OPTION_ADD_REPLACE)
			ExcuteAddReplace(aSelKey, true);
		else
			ExcuteDelete(aSelKey, true);
	}
	else if (m_nElemType == ELEM_TYPE_VBEAM)
	{
		pIGM->GetSelectedVbemKeyListForDgn(aSelKey);

		if (aSelKey.GetSize() == 0) {
			AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
			return;
		}

		if (m_nOption == OPTION_ADD_REPLACE)
			ExcuteAddReplace(aSelKey, false);
		else
			ExcuteDelete(aSelKey, false);
	}
}

void CDgnStlRationPrintDlg::Initial_SelectItem()
{
	// Unselected All.
	if (m_pDoc == NULL)	return;
	if (m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
}

void CDgnStlRationPrintDlg::OnDgnArDel() 
{
	UpdateData(TRUE);
	EnableDisableControls();
}

void CDgnStlRationPrintDlg::OnDgnFlexureRdo() 
{
	UpdateData(TRUE);
	EnableDisableControls();
}

void CDgnStlRationPrintDlg::OnDgnShearRdo() 
{
	UpdateData(TRUE);
	EnableDisableControls();
}

void CDgnStlRationPrintDlg::OnDgnServiceRdo() 
{
	UpdateData(TRUE);
	EnableDisableControls();
}

void CDgnStlRationPrintDlg::OnDgnCPGElemTypeRdo()
{
	UpdateData(TRUE);

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->ChangeSelectionFilterCombo(m_nElemType == ELEM_TYPE_ELEMENT ? EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM : EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_VBEM);

	EnableDisableControls();
}

void CDgnStlRationPrintDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;
	
	switch(lHint)
	{
		case D_UPDATE_DEFAULT:
			// do something...
			break;
		case D_UPDATE_BUFFER_BEFORE:
			// do something...
			break;
		case D_UPDATE_BUFFER_AFTER:
			{
				UpdateBuffer();
				break;
			}
		case D_UPDATE_UNIT:
			// do something...
			break;
		default:
			//ASSERT(FALSE)->ASSERT(TRUE)로 임시변경 ('2000.2.10)
			ASSERT(TRUE);
	}
}

void CDgnStlRationPrintDlg::UpdateBuffer()
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
		case(UR_RCDS_ADD)://코드 변경시
		case(UR_RCDS_DEL): 
			{
				AlignControl();
				EnableDisableControls();
				ControlsShowHide();
				return;
			}		
		default:
		  break;
		}
	}
} 
