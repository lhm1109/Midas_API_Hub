#include "stdafx.h"
#include "wg_dgn.h"
#include "RCDesignParamDlg.h"

#include "RCBeamParameter.h"
#include "RCColumnParameter.h"
#include "RCWallParameter.h"

#include "DgnDataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DBLib.h"

#include "..\wg_base\wg_base_Product.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\CheckDialogOpen.h"

#include "DgnConMatDlg.h"
#include "..\wg_base\wg_base_TreeMenuMode.h"
#include "..\wg_base\TreeMenuBarBase.h"

//#include "CMRebarPscCopyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRCDesignParamDlg dialog


CRCDesignParamDlg::CRCDesignParamDlg(CWnd* pParent /*=NULL*/)
	: CDgnDlgBase(CRCDesignParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRCDesignParamDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
//  m_Key = 0;
//  m_Data.Initialize();

	m_pDoc = 0;
	m_pDoc = CDBDoc::GetDocPoint();

	m_bModify = FALSE;

	m_pBeamDlg = 0;
	m_pColumnDlg = 0;
	m_pWallDlg = 0;
	m_bGroupMessage = FALSE;
}

CRCDesignParamDlg::~CRCDesignParamDlg()
{
	/*
	if(m_pBeamDlg!=NULL)
	{
		delete m_pBeamDlg;
		m_pBeamDlg=0;
	}
	if(m_pColumnDlg!=NULL)
	{
		delete m_pColumnDlg;
		m_pColumnDlg=0;
	}
	if(m_pWallDlg!=NULL)
	{
		delete m_pWallDlg;
		m_pWallDlg=0;
	}
	*/
	
}

/*
void CRCDesignParamDlg::SetParamData(T_RPSC_K RpscK)
{
	m_Key = RpscK;
	m_bModify = TRUE;
}
*/

void CRCDesignParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDgnDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRCDesignParamDlg)  
//IDC_RAO_NUMBER
	DDX_Control(pDX, IDC_TAB_RC_DESIGN, m_ctrlTab);
	DDX_Text(pDX, IDC_DGN_CODE_STA, m_strDesignCode);
	//}}AFX_DATA_MAP
}

#define CDialog CDgnDlgBase
BEGIN_MESSAGE_MAP(CRCDesignParamDlg, CDgnDlgBase)
#undef CDialog
	//{{AFX_MSG_MAP(CRCDesignParamDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnClose)  
	ON_BN_CLICKED(IDC_DGN_BASE_MODIFY_CONC_MATE_BTN, OnDgnModifyConcMateBtn)
	ON_BN_CLICKED(IDC_DGN_BASE_MODIFY_MEMBER_TYPE_BTN, OnDgnModifyMemberTypeBtn)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_RC_DESIGN, OnSelchangeCmdTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRCDesignParamDlg message handlers

BOOL CRCDesignParamDlg::OnInitDialog() 
{
	CDgnDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	/**/
	CCheckDialogOpen::SetSoftwareRendering();

	SetRCDesignCodeName();


	// Change by ZINU.('06.02.28). Auto-Setting Code by KSCE-USD05.
/*
	CString strCode = CDBLib::GetConCodeName();
	T_DCON_D DconD;
	if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetDcon(DconD))	DconD.Initialize();
	if(!(strCode==_T("KSCE-USD05") && DconD.bSpecialSeismic==FALSE))
	{
		DconD.DesignCode = _T("KSCE-USD05");
		DconD.bSpecialSeismic = FALSE;
		CDgnDataCtrl DataCtrl;
		double dPhi[5];
		if(!DataCtrl.Get_DgnConPhiDef(DconD.DesignCode, dPhi))	ASSERT(0);
		// Set Data.
	  for(int i=0; i<5; i++)	DconD.dPhi[i] = dPhi[i];
		if(!CDBDoc::GetDocPoint()->m_pDataCtrl->AddDcon(DconD))	ASSERT(0);
	}
	*/

	//this->LayoutChildDlg();
	LayoutChildDlg();
	  
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRCDesignParamDlg::SetRCDesignCodeName() 
{
	CString strCode = CDBLib::GetConCodeName();
	m_strDesignCode.Format( _T("%s%s"), _LS(IDS_DGN_DESIGN_CODE_), strCode );
	
	UpdateData(FALSE);
}

// add/replace
void CRCDesignParamDlg::OnOK() 
{
	int iTabIndex = m_ctrlTab.GetCurSel();
	//DB에 LoadCase 정보가 없다면
	bool bExistDB_LoadCase = false;

	if(iTabIndex == 0)//현재 활성화된 텝이 Beam
	{
		//DB에 LoadCase 정보가 없다면
		bExistDB_LoadCase = false;

		//Beam입력창의 LoadCase 정보를 저장함
		if(!bExistDB_LoadCase)
		{	
			m_pBeamDlg->Click_Btn_Apply();
		}
	}
	else if(iTabIndex == 1)//현재 활서화된 텝이 Column
	{
		//DB에 LoadCase 정보가 없다면
		bExistDB_LoadCase = false;

		//Column입력창의 LoadCase 정보를 저장함
		if(!bExistDB_LoadCase)
		{	
			m_pColumnDlg->Click_Btn_Apply();
		}
	}
	else//현재 활서화된 텝이 Wall
	{
		//DB에 LoadCase 정보가 없다면
		bExistDB_LoadCase = false;

		//Column입력창의 LoadCase 정보를 저장함
		if(!bExistDB_LoadCase)
		{	
			m_pWallDlg->Click_Btn_Apply();
		}
	}



	//설계 입력창을 띄어야함
}

// close
void CRCDesignParamDlg::OnClose() 
{
	// TODO: Add extra cleanup here
	DestroyWindow();
}

void CRCDesignParamDlg::LayoutChildDlg()
{
	CWnd* pPlaceHolder = GetDlgItem(IDC_BASE_PLACE_HOLDER);
	ASSERT(pPlaceHolder);
	m_ctrlTab.SetPlaceHolder(pPlaceHolder);

	m_pBeamDlg = new CRCBeamParameter(this);
	m_ctrlTab.AddTab(m_pBeamDlg, _LS(IDS_DGN_BEAM), CRCBeamParameter::IDD, TRUE);
	//m_ctrlTab.AddTab(m_pMomentDlg, _LS(IDS_CMD_RPSC_MAIN_REBAR), CCMRebarPscMoment::IDD, TRUE);

	m_pColumnDlg = new CRCColumnParameter(this);
	m_ctrlTab.AddTab(m_pColumnDlg, _LS(IDS_DGN_COLUMN), CRCColumnParameter::IDD, TRUE);

	m_pWallDlg = new CRCWallParameter(this);
	m_ctrlTab.AddTab(m_pWallDlg, _LS(IDS_DGN_WALL), CRCWallParameter::IDD, TRUE);

	//_LS(IDS_CMD_RPSC_SHEAR_REBAR)
	m_ctrlTab.ShowTab(0);  
	
	m_ctrlTab.DelTab(3);
	m_ctrlTab.DelTab(4);
}

BOOL CRCDesignParamDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	/**/
	CCheckDialogOpen::ResetSoftwareRendering();

	//m_wndGrid->OnTerminate(D_TB_SAVE_ALWAYS);
	return CDgnDlgBase::DestroyWindow();
}

void CRCDesignParamDlg::OnDgnModifyConcMateBtn() 
{
	CDgnConMatDlg dlg;
	dlg.DoModal();	
}

void CRCDesignParamDlg::OnDgnModifyMemberTypeBtn() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_DGN_GEN_MBTP__);	
}

void CRCDesignParamDlg::OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	/*
	if(m_ctrlTab.GetCurSel() == 1)
		AfxMessageBox(_T("3월중 업데이트 예정입니다."), MB_OK);
	*/
}

void CRCDesignParamDlg::ChangeGroupandLoadCombination()
{
	if(m_ctrlTab.GetCurSel() == 0) //Beam
	{
		m_pBeamDlg->ChangeLoadCombination();
	}
	else if(m_ctrlTab.GetCurSel() == 1)//Column
	{
		m_pColumnDlg->ChangeGroupandLoadCombination();
	}
	else if(m_ctrlTab.GetCurSel() == 2)//Wall
	{
		m_pWallDlg->ChangeGroupandLoadCombination();
	}		
}

void CRCDesignParamDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch(lHint)
	{  
	case D_UPDATE_SEL_PRE:
		EnableCalcBtn(false);
		break;
	case D_UPDATE_SEL_POST:
		EnableCalcBtn(true);
		break;
	case D_UPDATE_DGN_FORCE_CHANGED: //Load Conbination이 변경되었을 경우 해당 입력창을 닫아줌.
		{
			//DestroyWindow();
			break;
		}
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:    
		if(m_ctrlTab.GetCurSel() == 0) //Beam
	    m_pBeamDlg->UpdateBuffer();
		if(m_ctrlTab.GetCurSel() == 1) //Column
	    m_pColumnDlg->UpdateBuffer();
		if(m_ctrlTab.GetCurSel() == 2) //Wall
	    m_pWallDlg->UpdateBuffer();
		UpdateBuffer();
	  break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

//add 060323 sshan
void CRCDesignParamDlg::UpdateBuffer()
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
		case(UR_MEMB_ADD)://MEMB변경시 창을 닫는다.
		case(UR_MEMB_DEL): 
		case(UR_MEMB_MFD): 
		case(UR_MEMB_MFS):
		//case(UR_ELEM_ADD)://요소값이 변경되면 창을 닫는다.
		//case(UR_ELEM_DEL): 
		//case(UR_ELEM_MFD): 
		//case(UR_ELEM_MFS):
		case(UR_MBTP_ADD)://Modify MemberType
		case(UR_MBTP_DEL):    
		case(UR_TSGR_ADD):
		case(UR_TSGR_MFS):
		case(UR_TSGR_MFD):
			{
				DestroyWindow();
				return;
			}		
		case(UR_DCON_ADD):// Design Code 변경
		case(UR_DCON_DEL):	
			{
				SetRCDesignCodeName();
				break;
			}		
		case(UR_LCCO_ADD)://하중조합 변경시 반영
		case(UR_LCCO_DEL): 
		case(UR_LCCO_MFD): 
		case(UR_LCCO_MFS):
		case(UR_GRUP_ADD)://GRUP변경시 반영
		case(UR_GRUP_DEL): 
		case(UR_GRUP_MFD): 
		case(UR_GRUP_MFS):
			{
				ChangeGroupandLoadCombination();
				break;
			}
		default:
		  break;
		}
	}
}

//add 060324 sshan
void CRCDesignParamDlg::EnableCalcBtn(bool Enable)
{
	if(m_ctrlTab.GetCurSel() == 0) //Beam
	{
		m_pBeamDlg->EnableCalcBtn(Enable);
	}
	else if(m_ctrlTab.GetCurSel() == 1)//Column
	{
		m_pColumnDlg->EnableCalcBtn(Enable);
	}
	else if(m_ctrlTab.GetCurSel() == 2)//Wall
	{
		m_pWallDlg->EnableCalcBtn(Enable);
	}
}
