// DgnGenUnderLcomTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenUnderLcomTypeDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#include "..\wg_db\DBCodeDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnGenUnderLcomTypeDlg dialog


CDgnGenUnderLcomTypeDlg::CDgnGenUnderLcomTypeDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnGenUnderLcomTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenUnderLcomTypeDlg)
	m_nOption = 0;
	m_nOptUnder = EN_ULCT_UNDERGROUND;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnGenUnderLcomTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenUnderLcomTypeDlg)
	DDX_Radio(pDX, IDC_DGN_UNDER_LCOM_AR, m_nOption);
	DDX_Radio(pDX, IDC_DGN_UNDER_LCOM_UNDER_LOAD_RDO, m_nOptUnder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnGenUnderLcomTypeDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnGenUnderLcomTypeDlg)
	ON_BN_CLICKED(IDC_DGN_UNDER_LCOM_AR, OnDgnLcomAr)
	ON_BN_CLICKED(IDC_DGN_UNDER_LCOM_DEL, OnDgnLcomDel)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenUnderLcomTypeDlg message handlers

BOOL CDgnGenUnderLcomTypeDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	Update_InitDataByCode();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnGenUnderLcomTypeDlg::OnDgnClose()
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnGenUnderLcomTypeDlg::OnDgnExecute()
{
	UpdateData(TRUE);

	//	Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	CArray<T_ELEM_K, T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	if (aSelKey.GetSize() > 0)
	{
		T_ELEM_D ElemD;
		T_MATL_D MatlD;
		if (m_nOption == 0)
		{
			for (int i = aSelKey.GetSize() - 1; i >= 0; i--)
			{
				T_ELEM_K ElemK = aSelKey[i];
				if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;
				if (!m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat, MatlD)) continue;			
				// Beam, Truss, Wall.
				if (!m_pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp) && !m_pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp)) continue;
				rSelKey.Add(ElemK);
			}
		}
		else
		{
			for (int i = aSelKey.GetSize() - 1; i >= 0; i--)
			{
				T_ELEM_K ElemK = aSelKey[i];
				if (!m_pDoc->m_pAttrCtrl->ExistUlct(ElemK)) continue;
				rSelKey.Add(ElemK);
			}
		}

		T_ULCT_D UlctD;
		UlctD.Initialize();
		UlctD.nType = m_nOptUnder;

		if (m_nOption == 0) m_pDoc->m_pDataCtrl->AddUlct(rSelKey, UlctD);
		else                m_pDoc->m_pDataCtrl->DelUlct(rSelKey);

		m_pDoc->m_pViewCtrl->UnselectAll(NULL);

		Update_InitDataByCode();
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
}

void CDgnGenUnderLcomTypeDlg::OnDgnLcomAr()
{
	m_nOption = 0;
	EnableCtrl(TRUE);
}

void CDgnGenUnderLcomTypeDlg::OnDgnLcomDel()
{
	m_nOption = 1;
	EnableCtrl(FALSE);
}

void CDgnGenUnderLcomTypeDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch (lHint)
	{
	case D_UPDATE_DEFAULT:
		// do something...
		break;
	case D_UPDATE_BUFFER_BEFORE:
		// do something...
		break;
	case D_UPDATE_BUFFER_AFTER:
	{
		// do something...
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

// Coded by Seungjun ('20070516) MNet:No.2831.
void CDgnGenUnderLcomTypeDlg::Update_InitDataByCode()
{
	CString strConCode = CDBLib::GetConCodeName();
	CString strStlCode = CDBLib::GetStlCodeName();
	BOOL bShow = FALSE;

	if (CDBLib::IsCodeUnderLcomType())
		bShow = TRUE;

	GetDlgItem(IDC_DGN_UNDER_LCOM_AR)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_UNDER_LCOM_DEL)->EnableWindow(bShow);
	EnableCtrl(m_nOption == 0 && bShow);
	GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(bShow);
}

void CDgnGenUnderLcomTypeDlg::EnableCtrl(BOOL bEnable)
{
	GetDlgItem(IDC_DGN_UNDER_LCOM_UNDER_LOAD_RDO)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_UNDER_LCOM_NONE_UNDER_LOAD_RDO)->EnableWindow(bEnable);
}
