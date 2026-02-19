// DgnGenFMagDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenFMagDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#include "..\wg_main\wg_mainres2.h"	// For ID_DGN_GEN_FMAG
#include "..\wg_db\DBLib.h"
#include "..\wg_db\DBCodeDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int ELEM_TYPE_ELEMENT = 0;
const int ELEM_TYPE_VBEAM = 1;

/////////////////////////////////////////////////////////////////////////////
// CDgnGenFMagDlg dialog


CDgnGenFMagDlg::CDgnGenFMagDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnGenFMagDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenFMagDlg)
	m_nOption = -1;
	m_B1y = 0.0;
	m_B1z = 0.0;
	m_B2y = 0.0;
	m_B2z = 0.0;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_aElemTypeCtrl.RemoveAll();
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_TYPE_GRP);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_RDO);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_VELEM_RDO);

	m_aPositionCtrl.RemoveAll();
	m_aPositionCtrl.Add(IDC_WG_DGN_STATIC1);
	m_aPositionCtrl.Add(IDC_WG_DGN_STATIC2);
	m_aPositionCtrl.Add(IDC_WG_DGN_STATIC3);
	m_aPositionCtrl.Add(IDC_WG_DGN_STATIC4);
	m_aPositionCtrl.Add(IDC_WG_DGN_STATIC5);
	m_aPositionCtrl.Add(IDC_DGN_FMAGNIFY_B1Y);
	m_aPositionCtrl.Add(IDC_DGN_FMAGNIFY_B1Z);
	m_aPositionCtrl.Add(IDC_DGN_FMAGNIFY_B2Y);
	m_aPositionCtrl.Add(IDC_DGN_FMAGNIFY_B2Z);
	m_aPositionCtrl.Add(IDC_DGN_EXECUTE);
	m_aPositionCtrl.Add(IDC_DGN_CLOSE);
}


void CDgnGenFMagDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenFMagDlg)
	DDX_Radio(pDX, IDC_DGN_FMAGNIFY_AR, m_nOption);
	DDX_Text(pDX, IDC_DGN_FMAGNIFY_B1Y, m_B1y);
	DDX_Text(pDX, IDC_DGN_FMAGNIFY_B1Z, m_B1z);
	DDX_Text(pDX, IDC_DGN_FMAGNIFY_B2Y, m_B2y);
	DDX_Text(pDX, IDC_DGN_FMAGNIFY_B2Z, m_B2z);
	DDX_Radio(pDX, IDC_DGN_CPG_ELEM_RDO, m_nElemType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnGenFMagDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnGenFMagDlg)
	ON_BN_CLICKED(IDC_DGN_FMAGNIFY_AR, OnDgnFmagnifyAr)
	ON_BN_CLICKED(IDC_DGN_FMAGNIFY_DEL, OnDgnFmagnifyDel)
	ON_BN_CLICKED(IDC_DGN_CPG_ELEM_RDO, OnDgnCPGElemTypeRdo)
	ON_BN_CLICKED(IDC_DGN_CPG_VELEM_RDO, OnDgnCPGElemTypeRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenFMagDlg message handlers

void CDgnGenFMagDlg::Execute() 
{
	//	Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	if (m_nElemType == ELEM_TYPE_ELEMENT)
	{
		pIGM->GetSelectedElemKeyListForDgn(aSelKey);

		if (aSelKey.GetSize() > 0)
		{
			for (int i = 0; i < aSelKey.GetSize(); i++)
			{
				T_ELEM_K key = aSelKey.GetAt(i);
				T_ELEM_D eData;
				eData.Initialize();
				m_pDoc->m_pAttrCtrl->GetElem(key, eData);
				// Beam, Truss, Wall.
				if (m_pDoc->m_pAttrCtrl->IsFrameType(eData.eltyp) || m_pDoc->m_pAttrCtrl->IsWall(eData.eltyp))	rSelKey.Add(key);
			}
			T_FMAG_D rData;
			rData.Initialize();
			if (m_nOption == 0)	// add/replace
			{
				UpdateData(TRUE);
				rData.B1y = m_B1y;
				rData.B1z = m_B1z;
				rData.B2y = m_B2y;
				rData.B2z = m_B2z;
				if (rSelKey.GetSize() > 0)
				{
					// Initialize selected Element.
					if (m_pDoc->m_pDataCtrl->AddFmag(rSelKey, rData))	Initial_SelectItem();
				}
				else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE), MB_OK);
			}
			else	// Delete
			{
				// Initialize selected Element.
				if (m_pDoc->m_pDataCtrl->DelFmag(rSelKey))	Initial_SelectItem();
			}
		}
		else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
	}
	else
	{
		pIGM->GetSelectedVbemKeyListForDgn(aSelKey);

		T_FMAV_D rData;
		rData.Initialize();
		if (m_nOption == 0)	// add/replace
		{
			UpdateData(TRUE);
			rData.B1y = m_B1y;
			rData.B1z = m_B1z;
			rData.B2y = m_B2y;
			rData.B2z = m_B2z;
			if (aSelKey.GetSize() > 0)
			{
				// Initialize selected Element.
				if (m_pDoc->m_pDataCtrl->AddFmav(aSelKey, rData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE), MB_OK);
		}
		else	// Delete
		{
			// Initialize selected Element.
			if (m_pDoc->m_pDataCtrl->DelFmav(rSelKey))	Initial_SelectItem();
		}
	}
}

void CDgnGenFMagDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

BOOL CDgnGenFMagDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;

	if(m_B1y < 1. || m_B1y >= 10.)	bCheck = FALSE;
	if(m_B1z < 1. || m_B1z >= 10.)	bCheck = FALSE;
	if(m_B2y < 1. || m_B2y >= 10.)	bCheck = FALSE;
	if(m_B2z < 1. || m_B2z >= 10.)	bCheck = FALSE;

	if(!bCheck)	AfxMessageBox(_LS(IDS_DGN_CHK_MOM_MAG_FACTOR),MB_OK);

	return bCheck;
}

BOOL CDgnGenFMagDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	//  선택된 것들을 지우자 !!!
	if(m_pDoc==NULL)	return TRUE;
//**/ if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

	// Default 값 초기화
	Initial_Data();
	CString strConCode = CDBLib::GetConCodeName();
	if (strConCode == CONCODE_AS5100_5_17)
	{
		GetDlgItem(IDC_WG_DGN_STATIC2)->SetWindowText(_LSX(Delta_by));
		GetDlgItem(IDC_WG_DGN_STATIC3)->SetWindowText(_LSX(Delta_bz));
		GetDlgItem(IDC_WG_DGN_STATIC4)->SetWindowText(_LSX(Delta_sy));
		GetDlgItem(IDC_WG_DGN_STATIC5)->SetWindowText(_LSX(Delta_sz));
	}

	AlignControl();
	ShowHideControls();

	//// Apply Close Button Hide
	//GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnGenFMagDlg::AlignControl()
{
	T_CPGD_D CpgdD;
	CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnCpgd(CpgdD);

	CWnd* pElementTypeGroup = GetDlgItem(IDC_DGN_CPG_ELEM_TYPE_GRP);
	CWnd* pMomentFactorGroup = GetDlgItem(IDC_WG_DGN_STATIC1);

	if (pElementTypeGroup == nullptr || pMomentFactorGroup == nullptr)
		return;

	CRect rElementType, rMomentFactor;
	pElementTypeGroup->GetWindowRect(rElementType);
	pMomentFactorGroup->GetWindowRect(rMomentFactor);

	const int nDistX = rElementType.left - rMomentFactor.left;
	const int nDistY = (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || !m_pDoc->m_pAttrCtrl2->IsDgnVbeamCpgdCode(CpgdD.iDgnCode)) ?
		(rElementType.top - rMomentFactor.top) :
		(rElementType.bottom - rMomentFactor.top + globalUtils.ScaleByDPI(4));

	CDlgUtil::CtrlMoveDistXY(this, m_aPositionCtrl, nDistX, nDistY);
}

void CDgnGenFMagDlg::ShowHideControls()
{
	T_CPGD_D CpgdD; CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnCpgd(CpgdD);

	if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || !m_pDoc->m_pAttrCtrl2->IsDgnVbeamCpgdCode(CpgdD.iDgnCode))
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_HIDE);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_SHOW);
	}

	// Apply Close Button Hide
	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);
}

void CDgnGenFMagDlg::Initial_Data()
{
	m_nOption = 0;    // add/replace

	m_B1y = 0.;
	m_B1z = 0.;
	m_B2y = 1.;
	m_B2z = 1.;
	m_nElemType = ELEM_TYPE_ELEMENT;
	// Add by ZINU.('01.1.3).
	Update_InitDataByCode();

	UpdateData(FALSE);
}

void CDgnGenFMagDlg::OnDgnFmagnifyAr() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnGenFMagDlg::OnDgnFmagnifyDel() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

// Add by ZINU.('01.1.3).
void CDgnGenFMagDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
				// Add by ZINU.('01.1.3).
				Update_InitDataByCode();
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

// Add by ZINU.('01.1.3).
void CDgnGenFMagDlg::Update_InitDataByCode()
{
	CString strTitle = _T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleGen(ID_DGN_GEN_FMAG, strTitle, bShow);
	if(bShow)
	{
		GetDlgItem(IDC_DGN_FMAGNIFY_AR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_FMAGNIFY_DEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_FMAGNIFY_B1Y)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_FMAGNIFY_B1Z)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_FMAGNIFY_B2Y)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_FMAGNIFY_B2Z)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CPG_ELEM_RDO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CPG_VELEM_RDO)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_FMAGNIFY_AR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_FMAGNIFY_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_FMAGNIFY_B1Y)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_FMAGNIFY_B1Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_FMAGNIFY_B2Y)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_FMAGNIFY_B2Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CPG_ELEM_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CPG_VELEM_RDO)->EnableWindow(FALSE);
	}
}

void CDgnGenFMagDlg::OnDgnCPGElemTypeRdo()
{
	UpdateData(TRUE);

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->ChangeSelectionFilterCombo(m_nElemType == 0 ? EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM : EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_VBEM);
}