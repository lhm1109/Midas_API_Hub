// DgnGenPMcrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenPMcrDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#include "..\wg_main\wg_mainres2.h"	// For ID_DGN_GEN_PMCR

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnGenPMcrDlg dialog


CDgnGenPmcrDlg::CDgnGenPmcrDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnGenPmcrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenPMcrDlg)
	m_nOption = -1;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnGenPmcrDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenPMcrDlg)
	DDX_Radio(pDX, IDC_DGN_PMCR_AR, m_nOption);
	DDX_Control(pDX, IDC_DGN_PMCR_USE_EDT, m_edtUserZg);
	DDX_Control(pDX, IDC_DGN_PMCR_USE_UNT, m_untUserZg);
	DDX_Control(pDX, IDC_DGN_STEEL_IMG, m_wndSteelImg);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnGenPmcrDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnGenPMcrDlg)
	ON_BN_CLICKED(IDC_DGN_PMCR_AR, OnDgnPmcrnifyAr)
	ON_BN_CLICKED(IDC_DGN_PMCR_DEL, OnDgnPmcrnifyDel)
	ON_BN_CLICKED(IDC_DGN_PMCR_TOP_RDO, OnDgnPmcrTypeRdo)
	ON_BN_CLICKED(IDC_DGN_PMCR_CEN_RDO, OnDgnPmcrTypeRdo)
	ON_BN_CLICKED(IDC_DGN_PMCR_BOT_RDO, OnDgnPmcrTypeRdo)
	ON_BN_CLICKED(IDC_DGN_PMCR_USE_RDO, OnDgnPmcrTypeRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenPMcrDlg message handlers
void CDgnGenPmcrDlg::Execute()
{
	//	Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);
	UpdateData(TRUE);
	if(aSelKey.GetSize() > 0)
	{
		for(int i=0; i<aSelKey.GetSize(); i++)
		{
			T_ELEM_K key = aSelKey.GetAt(i);
			T_ELEM_D eData;
			eData.Initialize();
			m_pDoc->m_pAttrCtrl->GetElem(key,eData);
			
			T_MATL_D MatlD;
			if (!m_pDoc->m_pAttrCtrl->GetMatl(eData.elmat, MatlD)) { ASSERT(0); continue; }

			if(m_pDoc->m_pAttrCtrl->IsFrameType(eData.eltyp) && MatlD.Type == _T("S"))	rSelKey.Add(key);
		}
		T_PMCR_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
		{
			CDlgUtil::CtrlRadioGetCheck(this, m_aPMCRRdo, rData.nPMCR_TYPE);
			rData.dUserZg = m_edtUserZg.GetEditValue();
			if(rSelKey.GetSize() > 0)
			{
				if(m_pDoc->m_pDataCtrl->AddPmcr(rSelKey,rData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
		}
		else	// Delete
		{
			// Initialize selected Element.
		  if(m_pDoc->m_pDataCtrl->DelPmcr(rSelKey))	Initial_SelectItem();
		}	
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnGenPmcrDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}


BOOL CDgnGenPmcrDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	//  선택된 것들을 지우자 !!!
	if(m_pDoc==NULL)	return TRUE;
	InitCtrl();

	// Default 값 초기화
	Initial_Data();
	OnDgnPmcrTypeRdo();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnGenPmcrDlg::InitCtrl()
{
	m_aPMCRRdo.RemoveAll();
	m_aPMCRRdo.Add(IDC_DGN_PMCR_TOP_RDO);
	m_aPMCRRdo.Add(IDC_DGN_PMCR_CEN_RDO);
	m_aPMCRRdo.Add(IDC_DGN_PMCR_BOT_RDO);
	m_aPMCRRdo.Add(IDC_DGN_PMCR_USE_RDO);

	m_aUserZg.RemoveAll();
	m_aUserZg.Add(IDC_DGN_PMCR_USE_EDT);
	m_aUserZg.Add(IDC_DGN_PMCR_USE_UNT);

	m_edtUserZg.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untUserZg.SetUnitType(D_UNITSYS_BASE_LENGTH);

	T_PMCR_D PmcrD;
	PmcrD.Initialize();
	CDlgUtil::CtrlRadioSetCheck(this, m_aPMCRRdo, PmcrD.nPMCR_TYPE);
	m_edtUserZg.SetEditUnit(PmcrD.dUserZg);

	m_wndSteelImg.SetImage(_T("SVG\\Illustration\\Dialog\\Steel_image_s.svg"));
}

void CDgnGenPmcrDlg::Initial_Data()
{
	m_nOption = 0;    // add/replace

	// Add by ZINU.('01.1.3).
	Update_InitDataByCode();

	UpdateData(FALSE);
}

void CDgnGenPmcrDlg::OnDgnPmcrnifyAr() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnGenPmcrDlg::OnDgnPmcrnifyDel() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

void CDgnGenPmcrDlg::OnDgnPmcrTypeRdo()
{
	int nPmcrType = EN_STL_EC3_PMCR_TOP;
	CDlgUtil::CtrlRadioGetCheck(this, m_aPMCRRdo, nPmcrType);
	CDlgUtil::CtrlEnableDisable(this, m_aUserZg, nPmcrType == EN_STL_EC3_PMCR_USER);
}

void CDgnGenPmcrDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
void CDgnGenPmcrDlg::Update_InitDataByCode()
{
	CString strTitle = _T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleGen(ID_DGN_GEN_PMCR, strTitle, bShow);
	if(bShow)
	{
		GetDlgItem(IDC_DGN_PMCR_AR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_PMCR_DEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_PMCR_TOP_RDO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_PMCR_CEN_RDO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_PMCR_BOT_RDO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_PMCR_USE_RDO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_PMCR_USE_EDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_PMCR_USE_UNT)->EnableWindow(TRUE);

		//GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(TRUE);

	}
	else
	{
		GetDlgItem(IDC_DGN_PMCR_AR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_PMCR_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_PMCR_TOP_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_PMCR_CEN_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_PMCR_BOT_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_PMCR_USE_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_PMCR_USE_EDT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_PMCR_USE_UNT)->EnableWindow(FALSE);

		//GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(FALSE);
	}
}
