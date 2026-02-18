// DgnConBndrElemWallIDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConEBarDesignMethodWallIDDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#include "..\wg_main\wg_mainres2.h"	

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConEBarDesignMethodWallIDDlg dialog


CDgnConEBarDesignMethodWallIDDlg::CDgnConEBarDesignMethodWallIDDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConEBarDesignMethodWallIDDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConEBarDesignMethodWallIDDlg)
	m_nOption	            = 0;
	m_bApplyToSameWallId = TRUE;
	m_nEBarMethod = 0;
	m_nBEEBarMethod = 0;
	m_pDoc = CDBDoc::GetDocPoint();

}


void CDgnConEBarDesignMethodWallIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConEBarDesignMethodWallIDDlg)
	DDX_Radio(pDX, IDC_DGN_CON_EBAR_METHOD_WALLID_AR_RDO,       m_nOption);
	DDX_Check(pDX, IDC_DGN_CON_EBAR_SAME_WALLID_CHK, m_bApplyToSameWallId);
	DDX_Radio(pDX, IDC_DGN_CON_EBAR_DESIGN_WALLID_M1_RDO, m_nEBarMethod);
	DDX_Radio(pDX, IDC_DGN_CON_EBAR_DESIGN_WALLID_MA_RDO, m_nBEEBarMethod);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConEBarDesignMethodWallIDDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConEBarDesignMethodWallIDDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE,    OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE,  OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_CON_EBAR_METHOD_WALLID_AR_RDO,        OnDgnBEMWAr)
	ON_BN_CLICKED(IDC_DGN_CON_EBAR_METHOD_WALLID_DEL_RDO,       OnDgnBEMWDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConEBarDesignMethodWallIDDlg message handlers
void CDgnConEBarDesignMethodWallIDDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnConEBarDesignMethodWallIDDlg::OnDgnExecute() 
{  
	// Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	CArray<T_EBMW_K, T_EBMW_K> rSelKey;
	rSelKey.RemoveAll();
	Dlg2Data();
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	CMap<UINT, UINT, BOOL, BOOL> mStorChkWallID; // 유효 Story 여부 검토 by WallID
	mStorChkWallID.RemoveAll();
	CArray<unsigned int, unsigned int> aInternalWallIdList;
	if (aSelKey.GetSize() > 0)
	{
		for (int i = 0; i < aSelKey.GetSize(); i++)
		{
			T_ELEM_D ElemData; ElemData.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetElem(aSelKey[i], ElemData))	ASSERT(0);

			T_MATD_D MatdData; MatdData.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemData.elmat, MatdData))	continue;

			if (MatdData.Type == _T("C"))
			{
				// 1=column, 2=beam, 3=brace, 0=wall
				int nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(aSelKey[i]);

				if (nMembType == D_MBTP_WALL) // Wall
				{
					unsigned int nInternalWallId = 0;
					if (!m_pDoc->m_pPostCtrl->GetInternalWallId(aSelKey[i], nInternalWallId)) { ASSERT(0); continue; }
					T_WALL_K iWallKey;
					if (!m_pDoc->m_pPostCtrl->GetWallKey(nInternalWallId, iWallKey.keymap)) { ASSERT(0); continue; }
					unsigned int  nWallId = iWallKey.key.wallid;

					if (!m_CurData.bApplyToWallID)
					{
						T_EBMW_K dcbwKey;

						dcbwKey.key.wallid = iWallKey.key.wallid;
						dcbwKey.key.storid = iWallKey.key.storid;

						rSelKey.Add(dcbwKey);
					}
					else //when apply to all walls of same wall id
					{
						CArray<unsigned int, unsigned int> aInternalWallIdList;
						if (m_pDoc->m_pPostCtrl->GetInternalWallIdList(nWallId, aInternalWallIdList))
						{
							CArray<T_EBMW_KEY, T_EBMW_KEY> aWallKeyList;
							int nWallCount = aInternalWallIdList.GetSize();
							for (int k = 0; k < nWallCount; k++)
							{
								T_EBMW_K iTempWallKey;
								T_WALL_D wallData;
								if (!m_pDoc->m_pPostCtrl->GetWallKey(aInternalWallIdList[k], iTempWallKey.keymap)) { ASSERT(0); continue; }
								m_pDoc->m_pPostCtrl->GetWallData(iTempWallKey.keymap, wallData);
								int nWallElemCount = wallData.arElemKey.GetSize();
								if(nWallElemCount == 0) continue;
								for (int i = 0; i < nWallElemCount; i++)
								{
									T_STOR_K iStorKey;
									//m_pDoc->m_pPostCtrl->GetStorByElem(wallData.arElemKey[i], iStorKey);
									m_pDoc->m_pPostCtrl->GetStorByElem(wallData.arElemKey[i], iStorKey);
									T_EBMW_K dcbwKey;
									dcbwKey.key.wallid = nWallId;
									dcbwKey.key.storid = iStorKey;

									rSelKey.Add(dcbwKey);
								}

							}
							//Add at storey id 1
							T_EBMW_K dcbwKey;
							dcbwKey.key.wallid = nWallId;
							dcbwKey.key.storid = 1;
							rSelKey.Add(dcbwKey);
						}
					}

				}
			}
		}

		UpdateData(TRUE);
		if (rSelKey.GetCount() > 0)
		{
			int nDummy = 0;
			if (m_nOption == 0)	// add/replace
			{
				Dlg2Data();
				if (!m_pDoc->m_pDataCtrl->AddEbmw(rSelKey, m_CurData)) { ASSERT(0); }
			}
			else	// Delete
			{
				if (!m_pDoc->m_pDataCtrl->DelEbmw(rSelKey)) { ASSERT(0); }
			}
			Initial_SelectItem();
		}
		else AfxMessageBox(_LS(IDS_DGN_CON_ELEM_WALL), MB_OK);
	}
	else	AfxMessageBox(_LS(IDS_DGN_CON_ELEM_WALL), MB_OK);
}

void CDgnConEBarDesignMethodWallIDDlg::EnableControl()
{
	BOOL bEnable = m_pDoc->m_pPostCtrl->IsPostEnable();

	GetDlgItem(IDC_DGN_CON_EBAR_METHOD_WALLID_OPT_GRP)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_EBAR_METHOD_WALLID_AR_RDO)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_EBAR_METHOD_WALLID_DEL_RDO)->EnableWindow(bEnable);

	GetDlgItem(IDC_DGN_CON_EBAR_SAME_WALLID_CHK)->EnableWindow(bEnable);

	GetDlgItem(IDC_DGN_CON_EBAR_METHOD_WALLID_GRP)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_EBAR_DESIGN_WALLID_M1_RDO)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_EBAR_DESIGN_WALLID_M2_RDO)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_EBAR_DESIGN_WALLID_M3_RDO)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_EBAR_DESIGN_WALLID_M4_RDO)->EnableWindow(bEnable);

	GetDlgItem(IDC_DGN_CON_EBAR_BE_WALLID_GRP)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_EBAR_DESIGN_WALLID_MA_RDO)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_EBAR_DESIGN_WALLID_MB_RDO)->EnableWindow(bEnable);

	GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(bEnable);
}

void CDgnConEBarDesignMethodWallIDDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	if(m_pDoc->m_pViewCtrl->SelectedNodeCount()) m_pDoc->m_pViewCtrl->UnSelectAllNode(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

BOOL CDgnConEBarDesignMethodWallIDDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	Initial_Data();
	EnableControl();
	EnableByCode();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConEBarDesignMethodWallIDDlg::EnableByCode()
{
	T_DCON_D rData; rData.Initialize();
	m_pDoc->m_pAttrCtrl->GetDcon(rData);
	if (rData.DesignCode != CONCODE_IS456_2000)
	{
		GetDlgItem(IDC_DGN_CON_EBAR_BE_WALLID_GRP)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_EBAR_DESIGN_WALLID_MA_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_EBAR_DESIGN_WALLID_MB_RDO)->EnableWindow(FALSE);
#ifndef _MGEN_CH
		GetDlgItem(IDC_DGN_CON_EBAR_METHOD_WALLID_GRP)->SetWindowTextW(_LSX(End Rebar Design method));
#endif
	}
	else
	{
		GetDlgItem(IDC_DGN_CON_EBAR_BE_WALLID_GRP)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_EBAR_DESIGN_WALLID_MA_RDO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_EBAR_DESIGN_WALLID_MB_RDO)->EnableWindow(TRUE);

		GetDlgItem(IDC_DGN_CON_EBAR_METHOD_WALLID_GRP)->SetWindowTextW(_LSX(When B.E. is not required));
	}
}

void CDgnConEBarDesignMethodWallIDDlg::Initial_Data()
{
	m_nOption	= 0;      // add/replace
	m_bApplyToSameWallId = TRUE;
	m_nBEEBarMethod = 0;
	m_nEBarMethod= 0;	

	UpdateData(FALSE);
}

void CDgnConEBarDesignMethodWallIDDlg::OnDgnBEMWAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnConEBarDesignMethodWallIDDlg::OnDgnBEMWDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

void CDgnConEBarDesignMethodWallIDDlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_CurData.bApplyToWallID = m_bApplyToSameWallId;
	m_CurData.iEBarDesignMethod = m_nEBarMethod + 1;
	m_CurData.iBEEbarDesignMethod = m_nBEEBarMethod + 1;
}

void CDgnConEBarDesignMethodWallIDDlg::Data2Dlg()
{
	m_bApplyToSameWallId = m_CurData.bApplyToWallID;
	m_nEBarMethod = m_CurData.iEBarDesignMethod - 1;
	m_nBEEBarMethod = m_CurData.iBEEbarDesignMethod - 1;
	UpdateData(FALSE);
}

void CDgnConEBarDesignMethodWallIDDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
			// do something...
			EnableControl();
			EnableByCode();
		  break;
		case D_UPDATE_UNIT:
			// do something...
			break;
		default:
			ASSERT(TRUE);
	}
}
