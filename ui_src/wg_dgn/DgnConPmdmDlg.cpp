// DgnConPmdmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConPmdmDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConPmdmDlg dialog


CDgnConPmdmDlg::CDgnConPmdmDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConPmdmDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConPmdmDlg)
	//}}AFX_DATA_INIT

	m_nOption = 0;
	m_nElemType = 0;
	m_nCalcMethod = D_DGN_PMDM_E;

	m_pDoc = CDBDoc::GetDocPoint();
}

void CDgnConPmdmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConPmdmDlg)
	DDX_Radio(pDX, IDC_DGN_ADDREP_RDO,  m_nOption);
	DDX_Radio(pDX, IDC_DGN_COLUMN_RDO, m_nElemType);
	DDX_Radio(pDX, IDC_DGN_KEEP_RDO1, m_nCalcMethod);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConPmdmDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConPmdmDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConPmdmDlg message handlers
void CDgnConPmdmDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnConPmdmDlg::OnDgnExecute() 
{
	UpdateData(TRUE);

	//	Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	if(aSelKey.GetSize() > 0)
	{
		if(m_nElemType==0)
		{
			T_ELEM_D ElemD;
			T_MATL_D MatlD;
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
		else if(m_nElemType==1)
		{
			T_ELEM_D ElemD;
			T_MATL_D MatlD;
			for(int i=aSelKey.GetSize()-1; i>=0; i--)
			{
				T_ELEM_K ElemK = aSelKey[i];
				if(m_pDoc->m_pAttrCtrl->GetMemberType(ElemK)!=D_MBTP_BRACE) continue;
				if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK,ElemD)) continue;
				if(!m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat, MatlD)) continue;
				if(MatlD.Type!=_T("C")) continue;
				rSelKey.Add(ElemK);
			}
		}
		else
		{
			for(int i=aSelKey.GetSize()-1; i>=0; i--)
			{
				T_ELEM_K ElemK = aSelKey[i];
				if(m_pDoc->m_pAttrCtrl->GetMemberType(ElemK)!=D_MBTP_WALL) continue;
				rSelKey.Add(ElemK);
			}
		}

		T_PMDM_D PmdmD;
		PmdmD.Initialize();
		PmdmD.nCalcMethod = m_nCalcMethod;

		if(m_nOption == 0) m_pDoc->m_pDataCtrl->AddPmdm(rSelKey, PmdmD);
		else               m_pDoc->m_pDataCtrl->DelPmdm(rSelKey);

		m_pDoc->m_pViewCtrl->UnselectAll(NULL);
		
		Update_InitDataByCode();
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

BOOL CDgnConPmdmDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	Update_InitDataByCode();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConPmdmDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDgnConPmdmDlg::Update_InitDataByCode()
{
	BOOL bShow = CDBLib::IsConCodeForPMCalcMethod();

	GetDlgItem(IDC_DGN_OPT_GRP)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_ADDREP_RDO)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_DELETE_RDO)->EnableWindow(bShow);

	GetDlgItem(IDC_DGN_TYPE_GRP)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_COLUMN_RDO)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_BRACE_RDO)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_WALL_RDO)->EnableWindow(bShow);

	GetDlgItem(IDC_DGN_GRP)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_KEEP_RDO1)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_KEEP_RDO2)->EnableWindow(bShow);

	GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(bShow);
}
