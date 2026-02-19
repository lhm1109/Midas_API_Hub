// DgnGenCmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenCmDlg.h"
#include "DgnDataCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\DBCodeDef.h"

#include "..\wg_main\wg_mainres2.h"	// For ID_DGN_GEN_CMFT

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnGenCmDlg dialog

const int ELEM_TYPE_ELEMENT = 0;
const int ELEM_TYPE_VBEAM = 1;
CDgnGenCmDlg::CDgnGenCmDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnGenCmDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenCmDlg)
	m_nOption = -1;
	m_nElemType = ELEM_TYPE_ELEMENT;
	m_Cmy = 0.0;
	m_Cmz = 0.0;
	m_bAutoCm = FALSE;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();


	m_aElemTypeCtrl.RemoveAll();
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_TYPE_GRP);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_RDO);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_VELEM_RDO);

	m_aPositionCtrl.RemoveAll();
	m_aPositionCtrl.Add(IDC_WG_DGN_STATIC0);
	m_aPositionCtrl.Add(IDC_DGN_CMY_STATIC);
	m_aPositionCtrl.Add(IDC_DGN_CM_CMY);
	m_aPositionCtrl.Add(IDC_DGN_CMZ_STATIC);
	m_aPositionCtrl.Add(IDC_DGN_CM_CMZ);
	m_aPositionCtrl.Add(IDC_DGN_CM_AUTOCM);
	m_aPositionCtrl.Add(IDC_DGN_EXECUTE);
	m_aPositionCtrl.Add(IDC_DGN_CLOSE);
}


void CDgnGenCmDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenCmDlg)
	DDX_Radio(pDX, IDC_DGN_CM_AR, m_nOption);
	DDX_Text(pDX, IDC_DGN_CM_CMY, m_Cmy);
	DDX_Text(pDX, IDC_DGN_CM_CMZ, m_Cmz);
	DDX_Check(pDX, IDC_DGN_CM_AUTOCM, m_bAutoCm);
	DDX_Radio(pDX, IDC_DGN_CPG_ELEM_RDO, m_nElemType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnGenCmDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnGenCmDlg)
	ON_BN_CLICKED(IDC_DGN_CM_AUTOCM, OnDgnCmAutocm)
	ON_BN_CLICKED(IDC_DGN_CM_AR, OnDgnCmAr)
	ON_BN_CLICKED(IDC_DGN_CM_DEL, OnDgnCmDel)
	ON_BN_CLICKED(IDC_DGN_CPG_ELEM_RDO, OnDgnCPGElemTypeRdo)
	ON_BN_CLICKED(IDC_DGN_CPG_VELEM_RDO, OnDgnCPGElemTypeRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenCmDlg message handlers

void CDgnGenCmDlg::Execute()
{
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
				if (m_pDoc->m_pAttrCtrl->IsFrameType(eData.eltyp) || m_pDoc->m_pAttrCtrl->IsWall(eData.eltyp))
					rSelKey.Add(key);
			}
			T_CMFT_D rData;
			rData.Initialize();
			if (m_nOption == 0) // add/replace
			{
				UpdateData(TRUE);
				rData.bAuto = m_bAutoCm;
				rData.Cmy = m_Cmy;
				rData.Cmz = m_Cmz;
				if (rSelKey.GetSize() > 0)
				{
					// Initialize selected Element.
					if (m_pDoc->m_pDataCtrl->AddCmft(rSelKey, rData)) Initial_SelectItem();
				}
				else AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE), MB_OK);
			}
			else // Delete
			{
				// Initialize selected Element.
				if (m_pDoc->m_pDataCtrl->DelCmft(rSelKey)) Initial_SelectItem();
			}
		}
		else AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
	}
	else
	{
		pIGM->GetSelectedVbemKeyListForDgn(aSelKey);

		T_CMFT_D rData;
		rData.Initialize();
		if (m_nOption == 0) // add/replace
		{
			UpdateData(TRUE);
			rData.bAuto = m_bAutoCm;
			rData.Cmy = m_Cmy;
			rData.Cmz = m_Cmz;
			if (aSelKey.GetSize() > 0)
			{
				// Initialize selected Element.
				if (m_pDoc->m_pDataCtrl->AddCmfv(aSelKey, rData)) Initial_SelectItem();
			}
			else AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE), MB_OK);
		}
		else // Delete
		{
			// Initialize selected Element.
			if (m_pDoc->m_pDataCtrl->DelCmfv(aSelKey)) Initial_SelectItem();
		}
	}
}

void CDgnGenCmDlg::Initial_SelectItem()
{
	// Unselected All.
	if (m_pDoc == NULL) return;
	if (m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

BOOL CDgnGenCmDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();
	if (m_pDoc == NULL) return TRUE;
	//**/  if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

	Initial_Data();

	CString strConCode = CDBLib::GetConCodeName();
	if (strConCode == CONCODE_AS5100_5_17)
	{
		GetDlgItem(IDC_WG_DGN_STATIC0)->SetWindowText(_LS(IDS_DGN_MOMENT_CORRECTION_FACTOR_KM));
		GetDlgItem(IDC_DGN_CMY_STATIC)->SetWindowText(_LSX(Kmy  ));
		GetDlgItem(IDC_DGN_CMZ_STATIC)->SetWindowText(_LSX(Kmz  ));
	}

	AlignControl();
	ShowHideControls();
	//// Apply Close Button Hide
	//GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnGenCmDlg::AlignControl()
{
	T_CPGD_D CpgdD;
	CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnCpgd(CpgdD);

	CWnd* pElementTypeGroup = GetDlgItem(IDC_DGN_CPG_ELEM_TYPE_GRP);
	CWnd* pMomentFactorGroup = GetDlgItem(IDC_WG_DGN_STATIC0);

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

void CDgnGenCmDlg::ShowHideControls()
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

void CDgnGenCmDlg::Initial_Data()
{
	m_nOption = 0; // add/replace
	m_bAutoCm = FALSE;
	m_Cmy = 0.;
	m_Cmz = 0.;
	m_nElemType = ELEM_TYPE_ELEMENT;
	// Add by ZINU.('01.1.3).
	Update_InitDataByCode();

	UpdateData(FALSE);
}

BOOL CDgnGenCmDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;

	if (!m_bAutoCm) //	Not Automation
	{
		if (m_Cmy < 0.4 || m_Cmy > 1.) bCheck = FALSE;
		if (m_Cmz < 0.4 || m_Cmz > 1.) bCheck = FALSE;
	}
	if (!bCheck) AfxMessageBox(_LS(IDS_DGN_CHK_MOM_FACTOR),MB_OK);

	return bCheck;
}

void CDgnGenCmDlg::OnDgnCmAutocm()
{
	UpdateData(TRUE);

	if (m_bAutoCm)
	{
		m_Cmy = 0.;
		m_Cmz = 0.;

		GetDlgItem(IDC_DGN_CM_CMY)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CM_CMZ)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CM_CMY)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CM_CMZ)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}

void CDgnGenCmDlg::OnDgnCmAr()
{
	// TODO: Add your control notification handler code here
	m_nOption = 0; //	add/replace
	UpdateData(FALSE);
}

void CDgnGenCmDlg::OnDgnCmDel()
{
	// TODO: Add your control notification handler code here
	m_nOption = 1; //	Delete
	UpdateData(FALSE);
}

// Add by ZINU.('01.1.3).
void CDgnGenCmDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
			// Add by ZINU.('01.1.3).
			Update_InitDataByCode();
			break;
		}
	case D_UPDATE_UNIT:
		// do something...
		break;
	default:
		//ASSERT(FALSE)->ASSERT(TRUE)�� �ӽú��� ('2000.2.10)
		ASSERT(TRUE);
	}
}

// Add by ZINU.('01.1.3).
void CDgnGenCmDlg::Update_InitDataByCode()
{

	AlignControl();
	ShowHideControls();

	CString strInitCmy = _T("Cmy"), strInitCmz = _T("Cmz");
	CString strCmy = m_pDoc->GetDesignParamNa(strInitCmy);
	CString strCmz = m_pDoc->GetDesignParamNa(strInitCmz);
	GetDlgItem(IDC_DGN_CMY_STATIC)->SetWindowText(strCmy);
	GetDlgItem(IDC_DGN_CMZ_STATIC)->SetWindowText(strCmz);

	CString strTitle = _T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleGen(ID_DGN_GEN_CMFT, strTitle, bShow);
	if (bShow)
	{
		GetDlgItem(IDC_DGN_CM_AR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CM_DEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CM_CMY)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CM_CMZ)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CM_AUTOCM)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CPG_ELEM_RDO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CPG_VELEM_RDO)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CM_AR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CM_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CM_CMY)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CM_CMZ)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CM_AUTOCM)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CPG_ELEM_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CPG_VELEM_RDO)->EnableWindow(FALSE);
	}
}

void CDgnGenCmDlg::OnDgnCPGElemTypeRdo()
{
	UpdateData(TRUE);

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->ChangeSelectionFilterCombo(m_nElemType == 0 ? EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM : EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_VBEM);
}