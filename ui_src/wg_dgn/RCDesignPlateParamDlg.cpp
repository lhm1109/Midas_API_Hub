#include "stdafx.h"
#include "wg_dgn.h"
#include "RCDesignPlateParamDlg.h"

#include "RCPlateParameter.h"

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
// CRCDesignPlateParamDlg dialog


CRCDesignPlateParamDlg::CRCDesignPlateParamDlg(CWnd* pParent /*=NULL*/)
	: CDgnDlgBase(CRCDesignPlateParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRCDesignPlateParamDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
//  m_Key = 0;
//  m_Data.Initialize();

	m_pDoc = 0;
	m_pDoc = CDBDoc::GetDocPoint();

	m_bModify = FALSE;

	m_pPlateDlg = 0;

	m_bGroupMessage = FALSE;
}

CRCDesignPlateParamDlg::~CRCDesignPlateParamDlg()
{
	
}

void CRCDesignPlateParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDgnDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRCDesignPlateParamDlg)  
//IDC_RAO_NUMBER
	DDX_Control(pDX, IDC_TAB_RC_DESIGN, m_ctrlTab);
	DDX_Text(pDX, IDC_DGN_CODE_STA, m_strDesignCode);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRCDesignPlateParamDlg, CDgnDlgBase)
	//{{AFX_MSG_MAP(CRCDesignPlateParamDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnClose)  
	ON_BN_CLICKED(IDC_DGN_BASE_MODIFY_CONC_MATE_BTN, OnDgnModifyConcMateBtn)
	ON_BN_CLICKED(IDC_DGN_BASE_MODIFY_MEMBER_TYPE_BTN, OnDgnModifyMemberTypeBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRCDesignPlateParamDlg message handlers

BOOL CRCDesignPlateParamDlg::OnInitDialog() 
{
	CDgnDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	/**/
	CCheckDialogOpen::SetSoftwareRendering();

	SetRCDesignCodeName();

	GetDlgItem(IDC_DGN_PLATE_MODIFY_MEMBER_TYPE_STA)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_BASE_MODIFY_MEMBER_TYPE_BTN)->ShowWindow(SW_HIDE);
	
	UpdateData(FALSE);

	LayoutChildDlg();
	  
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRCDesignPlateParamDlg::SetRCDesignCodeName() 
{
	CString strCode = CDBLib::GetConCodeName();
	m_strDesignCode.Format( _T("%s%s"), _LS(IDS_DGN_DESIGN_CODE_), strCode );
	
	UpdateData(FALSE);
}

// add/replace
void CRCDesignPlateParamDlg::OnOK() 
{
	int iTabIndex = m_ctrlTab.GetCurSel();
	//DB에 LoadCase 정보가 없다면
	bool bExistDB_LoadCase = false;

	if(iTabIndex == 0)//현재 활성화된 텝이 Plate
	{
		//DB에 LoadCase 정보가 없다면
		bExistDB_LoadCase = false;

		//Beam입력창의 LoadCase 정보를 저장함
		if(!bExistDB_LoadCase)
		{	
			m_pPlateDlg->Click_Btn_Apply();
		}
	}
	else
		ASSERT(0);



	//설계 입력창을 띄어야함
}

// close
void CRCDesignPlateParamDlg::OnClose() 
{
	// TODO: Add extra cleanup here
	DestroyWindow();
}

void CRCDesignPlateParamDlg::LayoutChildDlg()
{
	CWnd* pPlaceHolder = GetDlgItem(IDC_BASE_PLACE_HOLDER);
	ASSERT(pPlaceHolder);
	m_ctrlTab.SetPlaceHolder(pPlaceHolder);

	m_pPlateDlg = new CRCPlateParameter(this);
	m_ctrlTab.AddTab(m_pPlateDlg, _LS(IDS_DGN_PLATE), CRCPlateParameter::IDD, TRUE);
	
	m_ctrlTab.ShowTab(0);  

	m_ctrlTab.DelTab(1);
	m_ctrlTab.DelTab(2);
	m_ctrlTab.DelTab(3);
	m_ctrlTab.DelTab(4);
}

BOOL CRCDesignPlateParamDlg::DestroyWindow() 
{
	CCheckDialogOpen::ResetSoftwareRendering();

	return CDgnDlgBase::DestroyWindow();
}

void CRCDesignPlateParamDlg::OnDgnModifyConcMateBtn() 
{
	CDgnConMatDlg dlg;
	dlg.DoModal();	
}

void CRCDesignPlateParamDlg::OnDgnModifyMemberTypeBtn() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_DGN_GEN_MBTP__);	
}

void CRCDesignPlateParamDlg::ChangeLoadCombination()
{
	if(m_ctrlTab.GetCurSel() == 0) 
	{
		m_pPlateDlg->ChangeLoadCombination();
	}	
}

void CRCDesignPlateParamDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
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
		UpdateBuffer();
		hWnd = GetSafeHwnd();
		if (!hWnd || !IsWindow(hWnd)) return;
		if(m_ctrlTab.GetCurSel() == 0)
	    m_pPlateDlg->UpdateBuffer();    
	  break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

//add 060323 sshan
void CRCDesignPlateParamDlg::UpdateBuffer()
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
		//case(UR_MEMB_ADD)://MEMB변경시 창을 닫는다.
		//case(UR_MEMB_DEL): 
		//case(UR_MEMB_MFD): 
		//case(UR_MEMB_MFS):		
		case(UR_ELEM_DEL): 
		case(UR_ELEM_MFD): 
		case(UR_ELEM_MFS):
		case(UR_NODE_ADD)://노드값이 변경되면 창을 닫는다.
		case(UR_NODE_DEL): 
		case(UR_NODE_MFD): 
		case(UR_NODE_MFS):
		//case(UR_NUCS_DEL)://UCS값이 변경되면 창을 닫는다. 		
			{
				DestroyWindow();
				return;
			}
		case(UR_DCON_ADD):// Design Code 변경
		case(UR_DCON_DEL):
			{
				SetRCDesignCodeName();
				return;
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
				ChangeLoadCombination();
				break;
			}
		default:
		  break;
		}
	}
}

//add 060324 sshan
void CRCDesignPlateParamDlg::EnableCalcBtn(bool Enable)
{
	if(m_ctrlTab.GetCurSel() == 0) //Beam
	{
		m_pPlateDlg->EnableCalcBtn(Enable);
	}	
}
