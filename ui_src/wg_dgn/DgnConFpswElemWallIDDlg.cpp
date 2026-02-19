// DgnConFpswElemWallIDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConFpswElemWallIDDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\DBCodeDef.h"
#include "..\wg_db\DBLib.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConFpswElemWallIDDlg dialog


CDgnConFpswElemWallIDDlg::CDgnConFpswElemWallIDDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConFpswElemWallIDDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConFpswElemWallIDDlg)
	m_nOption	    = 0;
	m_bAutoCheck	= FALSE;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnConFpswElemWallIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConFpswElemWallIDDlg)
	DDX_Radio(pDX, IDC_DGN_CON_FPSW_ELEM_WALLID_AR_RDO,   m_nOption);
	DDX_Check(pDX, IDC_DGN_CON_FPSW_ELEM_WALLID_AUTO_CHK, m_bAutoCheck);
	DDX_Control(pDX, IDC_DGN_CON_FPSW_ELEM_WALLID_T1_EDT, m_edtT1);
	DDX_Control(pDX, IDC_DGN_CON_FPSW_ELEM_WALLID_T1_UNT, m_untT1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConFpswElemWallIDDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConFpswElemWallIDDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE,    OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE,  OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_CON_FPSW_ELEM_WALLID_AR_RDO,        OnDgnFPSWAr)
	ON_BN_CLICKED(IDC_DGN_CON_FPSW_ELEM_WALLID_DEL_RDO,       OnDgnFPSWDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConFpswElemWallIDDlg message handlers
void CDgnConFpswElemWallIDDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnConFpswElemWallIDDlg::OnDgnExecute() 
{  
	// Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	//CArray<T_FPSW_K, T_FPSW_K> rSelKey;
	CMap<T_FPSW_K, T_FPSW_K, int, int> mSelKey;
	mSelKey.RemoveAll();
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	T_FPSW_D FpswD; FpswD.Initialize();
	FpswD.bAutoCalc      = m_bAutoCheck;
	FpswD.dT1            = m_edtT1.GetEditValue();

	if(aSelKey.GetSize() > 0)
	{
		for(int i=0; i<aSelKey.GetSize(); i++)
		{
			T_ELEM_D ElemData; ElemData.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetElem(aSelKey[i], ElemData))	ASSERT(0);

			// 1=column, 2=beam, 3=brace, 0=wall
			int nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(aSelKey[i]);
			if (nMembType != D_MBTP_WALL) continue; // Wall

			T_MATD_D MatdData; MatdData.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemData.elmat, MatdData))	continue;

			if(MatdData.Type != _T("C")) continue;
			
			int nDummy = 0;
			unsigned int nInternalWallId = 0;
			if (!m_pDoc->m_pPostCtrl->GetInternalWallId(aSelKey[i], nInternalWallId)) { ASSERT(0); continue; }
			T_WALL_K iWallKey;
			if (!m_pDoc->m_pPostCtrl->GetWallKey(nInternalWallId, iWallKey.keymap)) { ASSERT(0); continue; }
			unsigned int  nWallId = iWallKey.key.wallid;
			
		}
		
		UpdateData(TRUE);
		if(mSelKey.GetCount()>0)
		{
			int nDummy = 0;
			T_FPSW_K FpswK = NULL;
			if(m_nOption==0)	// add/replace
			{
				POSITION  pos = mSelKey.GetStartPosition();
				while (pos) 
				{
					mSelKey.GetNextAssoc(pos, FpswK, nDummy);
					m_pDoc->m_pDataCtrl->AddFpsw(FpswK, FpswD);
				}
			}
			else	// Delete
			{
				POSITION  pos = mSelKey.GetStartPosition();
				while (pos) 
				{
					mSelKey.GetNextAssoc(pos, FpswK, nDummy);
					if(m_pDoc->m_pAttrCtrl->ExistFpsw(FpswK))
					{
						m_pDoc->m_pDataCtrl->DelFpsw(FpswK);
					}
				}
			}
			Initial_SelectItem();
		}
		else AfxMessageBox(_LS(IDS_DGN_CON_ELEM_WALL),MB_OK);
	}
	else	AfxMessageBox(_LS(IDS_DGN_CON_ELEM_WALL),MB_OK);
}

void CDgnConFpswElemWallIDDlg::EnableControl()
{
	BOOL bEnable = GetEnableCtrl();

	GetDlgItem(IDC_DGN_CON_FPSW_ELEM_WALLID_OPT_GRP)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_FPSW_ELEM_WALLID_AR_RDO)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_FPSW_ELEM_WALLID_DEL_RDO)->EnableWindow(bEnable);

	GetDlgItem(IDC_DGN_CON_FPSW_ELEM_WALLID_PERIOD_GRP)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_FPSW_ELEM_WALLID_T1_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_FPSW_ELEM_WALLID_T1_EDT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_FPSW_ELEM_WALLID_T1_UNT)->EnableWindow(bEnable);

	GetDlgItem(IDC_DGN_CON_FPSW_ELEM_WALLID_AUTO_CHK)->EnableWindow(bEnable);

	GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(bEnable);	
}

BOOL CDgnConFpswElemWallIDDlg::GetEnableCtrl()
{
	BOOL bEnable = m_pDoc->m_pPostCtrl->IsPostEnable();
	if (!bEnable) return FALSE;
	
	const CString& strCode = CDBLib::GetConCodeName();
	const int nCode = CDBLib::GetConSubCode();
	if (strCode != CONCODE_EC2_04) return FALSE;

	T_DCON_D DconD;
	if (!m_pDoc->m_pAttrCtrl->GetDcon(DconD)) { ASSERT(0); DconD.Initialize(); }
	
	if (DconD.nNationalAnnex==dgn::def::enNationalAnnex::Italy && DconD.bNTCFlag && DconD.bBCJointDgn &&
		(DconD.nNTCType ==RCS_NTC18 || DconD.nNTCType ==RCS_NTC12 || DconD.nNTCType ==RCS_NTC08))
		return TRUE;
	if (DconD.bBCJointDgn) return TRUE;
	
	return FALSE;
}
void CDgnConFpswElemWallIDDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	if(m_pDoc->m_pViewCtrl->SelectedNodeCount()) m_pDoc->m_pViewCtrl->UnSelectAllNode(NULL,FALSE);

	Initial_Data();
}

BOOL CDgnConFpswElemWallIDDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	Initial_Unit();
	Initial_Data();
	EnableControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConFpswElemWallIDDlg::Initial_Unit()
{
	m_edtT1.SetUnitType(D_UNITSYS_BASE_TIME);
	m_untT1.SetUnitType(D_UNITSYS_BASE_TIME);
}

void CDgnConFpswElemWallIDDlg::Initial_Data()
{
	m_edtT1.SetEditUnit(0.0);

	m_nOption	= 0;      // add/replace
	UpdateData(FALSE);
}
void CDgnConFpswElemWallIDDlg::OnDgnFPSWAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnConFpswElemWallIDDlg::OnDgnFPSWDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

void CDgnConFpswElemWallIDDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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