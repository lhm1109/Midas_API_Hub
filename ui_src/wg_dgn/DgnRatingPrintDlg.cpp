// DgnRationPrintDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRatingPrintDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "..\wg_main\wg_mainres2.h"	

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnRationPrintDlg dialog

constexpr int ELEM_TYPE_ELEMENT = 0;
constexpr int ELEM_TYPE_VBEAM = 1;

CDgnRationPrintDlg::CDgnRationPrintDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnRationPrintDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnRationPrintDlg)
	m_nOption = -1;	
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_nElemType = ELEM_TYPE_ELEMENT;

	m_aElemTypeCtrl.RemoveAll();
	m_aElemTypeCtrl.Add(IDC_DGN_PSC_ELEM_TYPE_GRP);
	m_aElemTypeCtrl.Add(IDC_DGN_PSC_ELEM_RDO);
	m_aElemTypeCtrl.Add(IDC_DGN_PSC_VELEM_RDO);

	m_aMoveCtrl.RemoveAll();
	m_aMoveCtrl.Add(IDC_POSITION_GROUP);
	m_aMoveCtrl.Add(IDC_DGN_I_RADIO);
	m_aMoveCtrl.Add(IDC_DGN_J_RADIO);
	m_aMoveCtrl.Add(IDC_DGN_IJ_RADIO);
}


void CDgnRationPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnRationPrintDlg)
	DDX_Radio(pDX, IDC_DGN_AR, m_nOption);
	DDX_Radio(pDX, IDC_DGN_PSC_ELEM_RDO, m_nElemType);
	DDX_Control(pDX, IDC_DGN_I_RADIO, m_ratioI);
	DDX_Control(pDX, IDC_DGN_J_RADIO, m_ratioJ);
	DDX_Control(pDX, IDC_DGN_IJ_RADIO, m_ratioIJ);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnRationPrintDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnRationPrintDlg)
	ON_BN_CLICKED(IDC_DGN_AR, OnDgnAr)
	ON_BN_CLICKED(IDC_DGN_DEL, OnDgnDel)	
	ON_BN_CLICKED(IDC_DGN_PSC_ELEM_RDO, OnDgnPSCElemTypeRdo)
	ON_BN_CLICKED(IDC_DGN_PSC_VELEM_RDO, OnDgnPSCElemTypeRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnRationPrintDlg message handlers

// void CDgnRationPrintDlg::OnDgnClose() 
// {
// 	// TODO: Add your control notification handler code here
// 	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
// }

void CDgnRationPrintDlg::OnDgnPSCElemTypeRdo()
{
	UpdateData(TRUE);

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->ChangeSelectionFilterCombo(m_nElemType == 0 ? EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM : EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_VBEM);
}

void CDgnRationPrintDlg::Execute()
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
			/*
			for(int i=0; i<aSelKey.GetSize(); i++)
			{
				T_ELEM_K key = aSelKey.GetAt(i);
				T_ELEM_D eData;
				eData.Initialize();
				if(!m_pDoc->m_pAttrCtrl->GetElem(key,eData)) continue;
				if(CDBLib::IsPscDgnFor1st(key,TRUE))	rSelKey.Add(key);
			}
			*/

			T_RKPO_D rData;
			rData.Initialize();
			if (m_nOption == 0)	// add/replace
			{
				if (m_ratioI.GetCheck())
					rData.iPrintChk = 0;
				else if (m_ratioJ.GetCheck())
					rData.iPrintChk = 1;
				else //if(m_ratioIJ.GetCheck())
					rData.iPrintChk = 2;

				if (aSelKey.GetSize() > 0)
				{
					// Initialize selected Element.
					if (m_pDoc->m_pDataCtrl->AddRkpo(aSelKey, rData))	Initial_SelectItem();
				}
				//else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
			}
			else	// Delete
			{
				// Initialize selected Element.
				if (m_pDoc->m_pDataCtrl->DelRkpo(aSelKey))	Initial_SelectItem();
			}
		}
		else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
	}
	else
	{
		pIGM->GetSelectedVbemKeyListForDgn(aSelKey);

		if (aSelKey.GetSize() > 0)
		{
			/*
			for(int i=0; i<aSelKey.GetSize(); i++)
			{
				T_ELEM_K key = aSelKey.GetAt(i);
				T_ELEM_D eData;
				eData.Initialize();
				if(!m_pDoc->m_pAttrCtrl->GetElem(key,eData)) continue;
				if(CDBLib::IsPscDgnFor1st(key,TRUE))	rSelKey.Add(key);
			}
			*/

			T_RKPV_D rData;
			rData.Initialize();
			if (m_nOption == 0)	// add/replace
			{
				if (m_ratioI.GetCheck())
					rData.iPrintChk = 0;
				else if (m_ratioJ.GetCheck())
					rData.iPrintChk = 1;
				else //if(m_ratioIJ.GetCheck())
					rData.iPrintChk = 2;

				if (aSelKey.GetSize() > 0)
				{
					// Initialize selected Element.
					if (m_pDoc->m_pDataCtrl->AddRkpv(aSelKey, rData))	Initial_SelectItem();
				}
				//else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
			}
			else	// Delete
			{
				// Initialize selected Element.
				if (m_pDoc->m_pDataCtrl->DelRkpv(aSelKey))	Initial_SelectItem();
			}
		}
		else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
	}
}

void CDgnRationPrintDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

BOOL CDgnRationPrintDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();	

	
	Initial_Data();
	if(!SetEnableByDgnCode())
		return TRUE;	
	
	SetEnableByCtrl();
	HideAndMoveControlByDB();

	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnRationPrintDlg::Initial_Data()
{
	m_nOption	= 0;      // add/replace
	m_ratioI.SetCheck(TRUE);
	m_ratioJ.SetCheck(FALSE);
	m_ratioIJ.SetCheck(FALSE);
	
	UpdateData(FALSE);
}

void CDgnRationPrintDlg::SetEnableByCtrl()
{
	UpdateData(TRUE);

	BOOL bStateAddMod = m_nOption == 0;	

	GetDlgItem(IDC_POSITION_GROUP)->EnableWindow(bStateAddMod);
	GetDlgItem(IDC_DGN_I_RADIO)->EnableWindow(bStateAddMod);
	GetDlgItem(IDC_DGN_J_RADIO)->EnableWindow(bStateAddMod);
	GetDlgItem(IDC_DGN_IJ_RADIO)->EnableWindow(bStateAddMod);		
}

BOOL CDgnRationPrintDlg::SetEnableByDgnCode()
{
	T_RACD_D Data;
	Data.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRacd(Data);
	
    switch ( Data.nRatingCode )
    {
    case KSCE_USD05_RATING:
    case KSCE_USD10_RATING:
    case KSCE_RAIL_USD11_RATING:
    case KSCE_LSD15_RATING:
    case AASHTO_LRFD12_PSC_RATING:
    case CS_454_20_PSC_RATING:
    case AASHTO_LRFD19_PSC_RATING:
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

void CDgnRationPrintDlg::HideAndMoveControlByDB()
{
	T_RACD_D Data;
	Data.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRacd(Data);

	CRect rRef;
	CRect rToMove;
	GetDlgItem(IDC_DGN_PSC_ELEM_TYPE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_POSITION_GROUP)->GetWindowRect(rToMove);

	BOOL bPSCDgnVbem = CDBLib::ActivatePscVBEM();
	int nDistY;

	if (Data.nRatingCode == AASHTO_LRFD12_PSC_RATING || Data.nRatingCode == AASHTO_LRFD19_PSC_RATING)
	{
		if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || !bPSCDgnVbem)
		{
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, FALSE);
			m_nElemType = ELEM_TYPE_ELEMENT;
		}
		else
		{
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(4);
			CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, TRUE);
		}
	}
	else
	{
		nDistY = rRef.top - rToMove.top;
		m_nElemType = ELEM_TYPE_ELEMENT;
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, FALSE);
	}

	CDlgUtil::CtrlMoveDistY(this, m_aMoveCtrl, nDistY);
}

void CDgnRationPrintDlg::OnDgnAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);

	SetEnableByCtrl();
}

void CDgnRationPrintDlg::OnDgnDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);

	SetEnableByCtrl();
}

void CDgnRationPrintDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDgnRationPrintDlg::UpdateBuffer()
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
		case(UR_RACD_ADD)://코드 변경시
		case(UR_RACD_DEL): 
			{
				if(!SetEnableByDgnCode())
				{
					return;
				}
				SetEnableByCtrl();
				HideAndMoveControlByDB();
				return;
			}		
		default:
		  break;
		}
	}
}
