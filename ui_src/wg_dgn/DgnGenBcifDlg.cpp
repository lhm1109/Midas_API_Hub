// DgnGenBCifDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenBCifDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#include "..\wg_main\wg_mainres2.h"	// For ID_DGN_GEN_BCIF

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnGenBCifDlg dialog


CDgnGenBCifDlg::CDgnGenBCifDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnGenBCifDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenBCifDlg)
	m_nOption = -1;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnGenBCifDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenBCifDlg)
	DDX_Radio(pDX, IDC_DGN_BCIF_AR, m_nOption);
	DDX_Control(pDX, IDC_DGN_BCIF_ALPHA_Y_CMB, m_cmbAlphaY);
	DDX_Control(pDX, IDC_DGN_BCIF_ALPHA_Z_CMB, m_cmbAlphaZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnGenBCifDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnGenBCifDlg)	
	ON_BN_CLICKED(IDC_DGN_BCIF_AR, OnDgnBcifnifyAr)
	ON_BN_CLICKED(IDC_DGN_BCIF_DEL, OnDgnBcifnifyDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenBCifDlg message handlers

void CDgnGenBCifDlg::Execute() 
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
		T_BCIF_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
		{
			rData.nAlphaY = m_cmbAlphaY.GetItemData(m_cmbAlphaY.GetCurSel());
			rData.nAlphaZ = m_cmbAlphaZ.GetItemData(m_cmbAlphaZ.GetCurSel());
			if(rSelKey.GetSize() > 0)
			{
				if(m_pDoc->m_pDataCtrl->AddBcif(rSelKey,rData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
		}
		else	// Delete
		{
			// Initialize selected Element.
		  if(m_pDoc->m_pDataCtrl->DelBcif(rSelKey))	Initial_SelectItem();
		}	
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnGenBCifDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}


BOOL CDgnGenBCifDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	//  선택된 것들을 지우자 !!!
	if(m_pDoc==NULL)	return TRUE;
	InitCtrl();
//**/ if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

	// Default 값 초기화
	Initial_Data();

	// Apply Close Button Hide
	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnGenBCifDlg::InitCtrl()
{
	auto L_ComboSetting = [](MComboBox &rCombo)
	{
		CDlgUtil::CobxAddItem(rCombo, _LSX(a0), EN_STL_EC3_BCIF_A0);
		CDlgUtil::CobxAddItem(rCombo, _LSX(a), EN_STL_EC3_BCIF_A);
		CDlgUtil::CobxAddItem(rCombo, _LSX(b), EN_STL_EC3_BCIF_B);
		CDlgUtil::CobxAddItem(rCombo, _LSX(c), EN_STL_EC3_BCIF_C);
		CDlgUtil::CobxAddItem(rCombo, _LSX(d), EN_STL_EC3_BCIF_D);

		return FALSE;
	};

	L_ComboSetting(m_cmbAlphaY);
	L_ComboSetting(m_cmbAlphaZ);

	m_cmbAlphaY.SetCurSel(0);
	m_cmbAlphaZ.SetCurSel(0);
}

void CDgnGenBCifDlg::Initial_Data()
{
	m_nOption = 0;    // add/replace

	// Add by ZINU.('01.1.3).
	Update_InitDataByCode();

	UpdateData(FALSE);
}

void CDgnGenBCifDlg::OnDgnBcifnifyAr() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnGenBCifDlg::OnDgnBcifnifyDel() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

// Add by ZINU.('01.1.3).
void CDgnGenBCifDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
void CDgnGenBCifDlg::Update_InitDataByCode()
{
	CString strTitle = _T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleGen(ID_DGN_GEN_BCIF, strTitle, bShow);
	if(bShow)
	{
		GetDlgItem(IDC_DGN_BCIF_AR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_BCIF_DEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_BCIF_ALPHA_Y_CMB)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_BCIF_ALPHA_Z_CMB)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(TRUE);

	}
	else
	{
		GetDlgItem(IDC_DGN_BCIF_AR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_BCIF_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_BCIF_ALPHA_Y_CMB)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_BCIF_ALPHA_Z_CMB)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(FALSE);
	}
}
