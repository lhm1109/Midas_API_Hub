// DgnConScolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConScolDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_db\DBCodeDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConScolDlg dialog


CDgnConScolDlg::CDgnConScolDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConScolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConScolDlg)
	//}}AFX_DATA_INIT

	m_nOption = 0;
	m_nType = EN_SCOL_PILOTI;

	m_pDoc = CDBDoc::GetDocPoint();
}

void CDgnConScolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConScolDlg)
	DDX_Radio(pDX, IDC_DGN_ADDREP_RDO,  m_nOption);
	DDX_Radio(pDX, IDC_DGN_PILOTI_COLUMN_RDO, m_nType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConScolDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConScolDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConScolDlg message handlers
void CDgnConScolDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnConScolDlg::OnDgnExecute() 
{
	UpdateData(TRUE);

	//	Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	if(aSelKey.GetSize() > 0)
	{
		T_ELEM_D ElemD;
		T_MATL_D MatlD;
		if(m_nOption == 0)
		{
			for(int i=aSelKey.GetSize()-1; i>=0; i--)
			{
				T_ELEM_K ElemK = aSelKey[i];
				if(m_pDoc->m_pAttrCtrl->GetMemberType(ElemK)!=D_MBTP_COLUMN) continue;
				if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK,ElemD)) continue;
				if(!m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat, MatlD)) continue;
				if(MatlD.Type!=_T("C")) continue;
				rSelKey.Add(ElemK);
			}
		}
		else
		{
			T_MATL_D MatlD;
			for(int i=aSelKey.GetSize()-1; i>=0; i--)
			{
				T_ELEM_K ElemK = aSelKey[i];
				if(!m_pDoc->m_pAttrCtrl2->ExistScol(ElemK)) continue;
				rSelKey.Add(ElemK);
			}
		}
		

		T_SCOL_D ScolD;
		ScolD.Initialize();
		ScolD.nType = m_nType;

		if(m_nOption == 0) m_pDoc->m_pDataCtrl->AddScol(rSelKey, ScolD);
		else               m_pDoc->m_pDataCtrl->DelScol(rSelKey);

		m_pDoc->m_pViewCtrl->UnselectAll(NULL);

		Update_InitDataByCode();
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

BOOL CDgnConScolDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	Update_InitDataByCode();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConScolDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DEFAULT:
		// 
		break;
	case D_UPDATE_BUFFER_BEFORE:
		// 
		break;
	case D_UPDATE_BUFFER_AFTER:
		{
			Update_InitDataByCode();
			break;
		}
	case D_UPDATE_UNIT:
		// 
		break;
	default:
		ASSERT(TRUE);
	}
}

void CDgnConScolDlg::Update_InitDataByCode()
{
	CString strConCode = CDBLib::GetConCodeName();
	BOOL bShow = FALSE;

	if(strConCode == CONCODE_KDS_41_30_2018 || strConCode == CONCODE_KDS_41_20_2022 )
		bShow = TRUE;

	GetDlgItem(IDC_DGN_OPT_GRP)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_ADDREP_RDO)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_DELETE_RDO)->EnableWindow(bShow);

	GetDlgItem(IDC_DGN_TYPE_COLUMN_GRP)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_PILOTI_COLUMN_RDO)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_SOFT_STORY_COLUMN_RDO)->EnableWindow(bShow);

	GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(bShow);
}
