// DgnConBndrElemWallIDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConBndrElemWallIDDlg.h"

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
// CDgnConBndrElemWallIDDlg dialog


CDgnConBndrElemWallIDDlg::CDgnConBndrElemWallIDDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConBndrElemWallIDDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConBndrElemWallIDDlg)
	m_nOption	            = 0;
	m_bBndrElemMethod     = FALSE;
	m_nBndrElemMethodOpt  = 0;
	m_bBotStor            = FALSE;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

}


void CDgnConBndrElemWallIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConBndrElemWallIDDlg)
	DDX_Radio(pDX, IDC_DGN_CON_BNDR_ELEM_WALLID_AR_RDO,       m_nOption);
	DDX_Radio(pDX, IDC_DGN_CON_BNDR_ELEM_WALLID_DBM_RDO,      m_nBndrElemMethodOpt);
	DDX_Check(pDX, IDC_DGN_CON_BNDR_ELEM_WALLID_BEM_CHK,      m_bBndrElemMethod);
	DDX_Check(pDX, IDC_DGN_CON_BNDR_ELEM_WALLID_BOT_STOR_CHK, m_bBotStor);
	DDX_Control(pDX, IDC_DGN_CON_BNDR_ELEM_WALLID_BOT_STOR_CMB, m_wndBotStorCmb);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConBndrElemWallIDDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConBndrElemWallIDDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE,    OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE,  OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_CON_BNDR_ELEM_WALLID_AR_RDO,        OnDgnBEMWAr)
	ON_BN_CLICKED(IDC_DGN_CON_BNDR_ELEM_WALLID_DEL_RDO,       OnDgnBEMWDel)
	ON_BN_CLICKED(IDC_DGN_CON_BNDR_ELEM_WALLID_DBM_RDO,       OnDgnBEMWDbm)
	ON_BN_CLICKED(IDC_DGN_CON_BNDR_ELEM_WALLID_SBM_RDO,       OnDgnBEMWSbm)
	ON_BN_CLICKED(IDC_DGN_CON_BNDR_ELEM_WALLID_BEM_CHK,       OnDgnBEMWChk)
	ON_BN_CLICKED(IDC_DGN_CON_BNDR_ELEM_WALLID_BOT_STOR_CHK,  OnDgnBEMWBotStorChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConBndrElemWallIDDlg message handlers
void CDgnConBndrElemWallIDDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnConBndrElemWallIDDlg::OnDgnExecute() 
{  
	// Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	//CArray<T_BEMW_K, T_BEMW_K> rSelKey;
	CMap<T_BEMW_K, T_BEMW_K, int, int> mSelKey;
	mSelKey.RemoveAll();
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	T_STOR_K StorK;
	int num = m_wndBotStorCmb.GetCurSel();
	StorK = m_wndBotStorCmb.GetItemData(num);

	if(!m_pDoc->m_pAttrCtrl->ExistStor(StorK)) { ASSERT(0); return;}

	T_BEMW_D BemwD; BemwD.Initialize();
	BemwD.bBndrElemMethod = m_bBndrElemMethod;
	BemwD.nMethodType     = m_nBndrElemMethodOpt;
	BemwD.bBotStor        = m_bBotStor;
	BemwD.nStorK          = StorK;

	CMap<UINT, UINT, BOOL, BOOL> mStorChkWallID; // 유효 Story 여부 검토 by WallID
	mStorChkWallID.RemoveAll();
	CArray<unsigned int, unsigned int> aInternalWallIdList;
	if(aSelKey.GetSize() > 0)
	{
		for(int i=0; i<aSelKey.GetSize(); i++)
		{
			T_ELEM_D ElemData; ElemData.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetElem(aSelKey[i], ElemData))	ASSERT(0);

			T_MATD_D MatdData; MatdData.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemData.elmat, MatdData))	continue;

			if(MatdData.Type == _T("C"))
			{
				// 1=column, 2=beam, 3=brace, 0=wall
				int nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(aSelKey[i]);
				
				if(nMembType == D_MBTP_WALL) // Wall
				{
					int nDummy = 0;
					unsigned int nInternalWallId = 0;
					if(!m_pDoc->m_pPostCtrl->GetInternalWallId(aSelKey[i], nInternalWallId)) {ASSERT(0); continue;}
					T_WALL_K iWallKey;
					if(!m_pDoc->m_pPostCtrl->GetWallKey(nInternalWallId, iWallKey.keymap)) {ASSERT(0); continue;}
					unsigned int  nWallId = iWallKey.key.wallid;
					
					if(m_bBotStor)
					{
						BOOL bStorOk = FALSE;
						if(!mStorChkWallID.Lookup(nWallId, bStorOk))
						{
							// 유효 Story 여부 검사
							if(m_pDoc->m_pPostCtrl->GetInternalWallIdList(nWallId, aInternalWallIdList))
							{
								int nWallCount = aInternalWallIdList.GetSize();
								for(int k=0; k<nWallCount; k++)
								{
									T_WALL_K iTempWallKey;
									if(!m_pDoc->m_pPostCtrl->GetWallKey(aInternalWallIdList[k], iTempWallKey.keymap)) {ASSERT(0); continue;}
									if(StorK == iTempWallKey.key.storid) bStorOk = TRUE;
								}
								mStorChkWallID.SetAt(nWallId, bStorOk);
							}
							if(bStorOk==FALSE)
							{
								CString strErrMsg = _T("");
								T_STOR_D Data; Data.Initialize();
								if(!m_pDoc->m_pAttrCtrl->GetStor(StorK, Data)) ASSERT(0);
								strErrMsg.Format(_LS(IDS_DGN_CON_ELEM_WALLID_STOR_CHK_ERR_MSG), Data.StoryName, nWallId);
								
								GSaveHistoryFormatNF(strErrMsg);
							}
						}
						if(bStorOk==FALSE) continue;
					}

					if(!mSelKey.Lookup(nWallId, nDummy)) mSelKey.SetAt(nWallId, nDummy);
				}
			}
		}
		
		UpdateData(TRUE);
		if(mSelKey.GetCount()>0)
		{
			int nDummy = 0;
			T_BEMW_K BemwK = NULL;
			if(m_nOption==0)	// add/replace
			{
				POSITION  pos = mSelKey.GetStartPosition();
				while (pos) 
				{
					mSelKey.GetNextAssoc(pos, BemwK, nDummy);
					m_pDoc->m_pDataCtrl->AddBemw(BemwK, BemwD);
				}
			}
			else	// Delete
			{
				POSITION  pos = mSelKey.GetStartPosition();
				while (pos) 
				{
					mSelKey.GetNextAssoc(pos, BemwK, nDummy);
					if(m_pDoc->m_pAttrCtrl->ExistBemw(BemwK))
					{
						m_pDoc->m_pDataCtrl->DelBemw(BemwK);
					}
				}
			}
			Initial_SelectItem();
		}
		else AfxMessageBox(_LS(IDS_DGN_CON_ELEM_WALL),MB_OK);
	}
	else	AfxMessageBox(_LS(IDS_DGN_CON_ELEM_WALL),MB_OK);
}

void CDgnConBndrElemWallIDDlg::EnableControl()
{
	BOOL bEnable = m_pDoc->m_pPostCtrl->IsPostEnable();

	GetDlgItem(IDC_DGN_CON_BNDR_ELEM_WALLID_OPT_GRP)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_BNDR_ELEM_WALLID_AR_RDO)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_BNDR_ELEM_WALLID_DEL_RDO)->EnableWindow(bEnable);

	GetDlgItem(IDC_DGN_CON_BNDR_ELEM_WALLID_BEM_CHK)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_BNDR_ELEM_WALLID_BEM_GRP)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_BNDR_ELEM_WALLID_DBM_RDO)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_BNDR_ELEM_WALLID_SBM_RDO)->EnableWindow(bEnable);

	GetDlgItem(IDC_DGN_CON_BNDR_ELEM_WALLID_BOT_STOR_CHK)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_BNDR_ELEM_WALLID_BOT_STOR_CMB)->EnableWindow(bEnable);

	GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(bEnable);
	
	if(bEnable) EnableByBEMWChk();
}

void CDgnConBndrElemWallIDDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	if(m_pDoc->m_pViewCtrl->SelectedNodeCount()) m_pDoc->m_pViewCtrl->UnSelectAllNode(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

BOOL CDgnConBndrElemWallIDDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	Initial_Data();
	EnableControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConBndrElemWallIDDlg::Initial_Data()
{
	m_nOption	= 0;      // add/replace
	m_bBndrElemMethod = FALSE;
	m_nBndrElemMethodOpt = 0;
	m_bBotStor = FALSE;
	
	if(m_wndBotStorCmb.GetCount() > 0)	m_wndBotStorCmb.ResetContent();

	CArray<T_STOR_K, T_STOR_K> aStorKey;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(aStorKey);
	T_STOR_D Data;
	int ix;
	int nSize = aStorKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		m_pDoc->m_pAttrCtrl->GetStor(aStorKey[i], Data);		
		ix = m_wndBotStorCmb.AddString(Data.StoryName);
		m_wndBotStorCmb.SetItemData(ix, aStorKey[i]);
	} 
	m_wndBotStorCmb.SetCurSel(0);	

	EnableByBEMWChk();
	EnableByBEMWBotStorChk();

	UpdateData(FALSE);
}

void CDgnConBndrElemWallIDDlg::OnDgnBEMWAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnConBndrElemWallIDDlg::OnDgnBEMWDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

void CDgnConBndrElemWallIDDlg::OnDgnBEMWDbm() 
{
	m_nBndrElemMethodOpt  = 0;  // Displacement Based Method
	UpdateData(FALSE);
}

void CDgnConBndrElemWallIDDlg::OnDgnBEMWSbm() 
{
	m_nBndrElemMethodOpt  = 1;  // Stress Based Method
	UpdateData(FALSE);
}

void CDgnConBndrElemWallIDDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
		  break;
		case D_UPDATE_UNIT:
			// do something...
			break;
		default:
			ASSERT(TRUE);
	}
}

void CDgnConBndrElemWallIDDlg::OnDgnBEMWChk()
{
	UpdateData(TRUE);
	EnableByBEMWChk();
	UpdateData(FALSE);
}

void CDgnConBndrElemWallIDDlg::OnDgnBEMWBotStorChk()
{
	UpdateData(TRUE);
	EnableByBEMWBotStorChk();
	UpdateData(FALSE);
}

void CDgnConBndrElemWallIDDlg::EnableByBEMWChk()
{
	if(m_bBndrElemMethod)
	{
		GetDlgItem(IDC_DGN_CON_BNDR_ELEM_WALLID_DBM_RDO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_BNDR_ELEM_WALLID_SBM_RDO)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CON_BNDR_ELEM_WALLID_DBM_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_BNDR_ELEM_WALLID_SBM_RDO)->EnableWindow(FALSE);
	}
}

void CDgnConBndrElemWallIDDlg::EnableByBEMWBotStorChk()
{
	if(m_bBotStor)
	{
		GetDlgItem(IDC_DGN_CON_BNDR_ELEM_WALLID_BOT_STOR_CMB)->EnableWindow(TRUE);    
	}
	else
	{
		GetDlgItem(IDC_DGN_CON_BNDR_ELEM_WALLID_BOT_STOR_CMB)->EnableWindow(FALSE);
	}
}

