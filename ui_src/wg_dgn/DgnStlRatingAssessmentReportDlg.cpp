// DgnStlRatingAssessmentReportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlRatingAssessmentReportDlg.h"

#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
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
// CDgnStlRatingAssessmentReportDlg dialog

const int ELEM_TYPE_ELEMENT = 0;
const int ELEM_TYPE_VBEAM = 1;
const int OPTION_ADD_REPLACE = 0;
const int OPTION_DELETE = 1;

CDgnStlRatingAssessmentReportDlg::CDgnStlRatingAssessmentReportDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnStlRatingAssessmentReportDlg::IDD, pParent)
{
	m_nOption = OPTION_ADD_REPLACE;
	m_nElemType = ELEM_TYPE_ELEMENT;
	m_pDoc = CDBDoc::GetDocPoint();	            
}

void CDgnStlRatingAssessmentReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_DGN_AR, m_nOption);
	DDX_Radio(pDX, IDC_DGN_CPG_ELEM_RDO, m_nElemType);
	DDX_Control(pDX, IDC_DGN_I_RADIO, m_ratioI);
	DDX_Control(pDX, IDC_DGN_J_RADIO, m_ratioJ);
	DDX_Control(pDX, IDC_DGN_IJ_RADIO, m_ratioIJ);
}

BEGIN_MESSAGE_MAP(CDgnStlRatingAssessmentReportDlg, CMenuBarChildDlg)
	ON_BN_CLICKED(IDC_DGN_AR, OnDgnAr)
	ON_BN_CLICKED(IDC_DGN_DEL, OnDgnDel)
	ON_BN_CLICKED(IDC_DGN_CPG_ELEM_RDO, OnDgnCPGElemTypeRdo)
	ON_BN_CLICKED(IDC_DGN_CPG_VELEM_RDO, OnDgnCPGElemTypeRdo)
END_MESSAGE_MAP()


void CDgnStlRatingAssessmentReportDlg::InitCtrl()
{
	m_aElemTypeCtrl.RemoveAll();
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_TYPE_GRP);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_RDO);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_VELEM_RDO);

	m_aPositionCtrl.RemoveAll();
	m_aPositionCtrl.Add(IDC_POSITION_GROUP);
	m_aPositionCtrl.Add(IDC_DGN_I_RADIO);
	m_aPositionCtrl.Add(IDC_DGN_J_RADIO);
	m_aPositionCtrl.Add(IDC_DGN_IJ_RADIO);
	m_aPositionCtrl.Add(IDC_DGN_EXECUTE);
	m_aPositionCtrl.Add(IDC_DGN_CLOSE);
}

void CDgnStlRatingAssessmentReportDlg::AlignControl()
{
	T_RCDS_D Data;
	Data.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRcds(Data);

	CRect rRef;
	CRect rToMove;
	GetDlgItem(IDC_DGN_CPG_ELEM_TYPE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_POSITION_GROUP)->GetWindowRect(rToMove);
	int nDistX = rRef.left - rToMove.left;

	int nDistY;
	if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || Data.nRatingCode != STL_CS454_20)
	{
		nDistY = rRef.top - rToMove.top;
	}
	else
	{
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(4);
	}

	CDlgUtil::CtrlMoveDistXY(this, m_aPositionCtrl, nDistX, nDistY);
}

void CDgnStlRatingAssessmentReportDlg::ShowHideControls()
{
	T_RCDS_D RcdsD; RcdsD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRcds(RcdsD);

	if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || RcdsD.nRatingCode != STL_CS454_20)
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_HIDE);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_SHOW);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDgnStlRatingAssessmentReportDlg message handlers
void CDgnStlRatingAssessmentReportDlg::Execute()
{
	auto SetCommonData = [this](auto& rData) {
		UpdateData(TRUE);
		if (m_ratioI.GetCheck())
			rData.iPrintChk = 0;
		else if (m_ratioJ.GetCheck())
			rData.iPrintChk = 1;
		else //if(m_ratioIJ.GetCheck())
			rData.iPrintChk = 2;
	};

	auto ExcuteAddReplace = [this, &SetCommonData](auto& selKeys, bool isElement) {
		if (isElement)
		{
			T_SRAR_D rData;
			rData.Initialize();
			SetCommonData(rData);
			if (m_pDoc->m_pDataCtrl->AddSrar(selKeys, rData)) Initial_SelectItem();
		}
		else
		{
			T_SRRV_D rData;
			rData.Initialize();
			SetCommonData(rData);
			if (m_pDoc->m_pDataCtrl->AddSrrv(selKeys, rData)) Initial_SelectItem();
		}
	};

	auto ExcuteDelete = [this](auto& selKeys, bool isElement) {
		bool success = isElement ?
			m_pDoc->m_pDataCtrl->DelSrar(selKeys) :
			m_pDoc->m_pDataCtrl->DelSrrv(selKeys);
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

void CDgnStlRatingAssessmentReportDlg::Initial_SelectItem()
{
	// Unselected All.
	if (m_pDoc == NULL)	return;
	if (m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
	// Initialize Data.
	Initial_Data();
}

BOOL CDgnStlRatingAssessmentReportDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	Initial_Data();
	if (!SetEnableByDgnCode())
		return TRUE;

	InitCtrl();
	AlignControl();
	ShowHideControls();
	EnableDisableControls();

	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnStlRatingAssessmentReportDlg::Initial_Data()
{
	m_nOption = OPTION_ADD_REPLACE;
	m_nElemType = ELEM_TYPE_ELEMENT;
	m_ratioI.SetCheck(TRUE);
	m_ratioJ.SetCheck(FALSE);
	m_ratioIJ.SetCheck(FALSE);

	UpdateData(FALSE);
}

void CDgnStlRatingAssessmentReportDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlEnableDisable(this, m_aPositionCtrl, (m_nOption == OPTION_ADD_REPLACE));
}

BOOL CDgnStlRatingAssessmentReportDlg::SetEnableByDgnCode()
{
	T_RCDS_D Data;
	Data.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRcds(Data);

	switch (Data.nRatingCode)
	{
	case STL_CS454_20:
	case STL_NR_GN_CIV_025_06:
		GetDlgItem(IDC_DGN_OPTION_GROUP)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_AR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_DEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_POSITION_GROUP)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_I_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_J_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_IJ_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(TRUE);
		return TRUE;
	default:
		GetDlgItem(IDC_DGN_OPTION_GROUP)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_AR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_POSITION_GROUP)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_I_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_J_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_IJ_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(FALSE);
		return FALSE;
		break;
	}
}

void CDgnStlRatingAssessmentReportDlg::OnDgnAr()
{
	UpdateData(TRUE);

	EnableDisableControls();
}

void CDgnStlRatingAssessmentReportDlg::OnDgnDel()
{
	UpdateData(TRUE);

	EnableDisableControls();
}

void CDgnStlRatingAssessmentReportDlg::OnDgnCPGElemTypeRdo()
{
	UpdateData(TRUE);

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->ChangeSelectionFilterCombo(m_nElemType == ELEM_TYPE_ELEMENT ? EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM : EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_VBEM);

	EnableDisableControls();
}

void CDgnStlRatingAssessmentReportDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch (lHint)
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

void CDgnStlRatingAssessmentReportDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bSpfcChanged = FALSE;
	BOOL bStorChanged = FALSE;
	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		switch (nCmd)
		{
		case(UR_RACD_ADD)://코드 변경시
		case(UR_RACD_DEL):
		{
			AlignControl();
			ShowHideControls();
			if (!SetEnableByDgnCode())
			{
				return;
			}
			EnableDisableControls();
			return;
		}
		default:
			break;
		}
	}
}
