#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenDampSpecifySpecialTypeDlg.h"

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
#include "..\wg_db\DampDgnMgr_CH.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDgnGenDampSpecifySpecialTypeDlg::CDgnGenDampSpecifySpecialTypeDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnGenDampSpecifySpecialTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenDampSpecifySpecialTypeDlg)
	m_nOption = 0;
	m_nOptType = 0;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnGenDampSpecifySpecialTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenDampSpecifySpecialTypeDlg)
	DDX_Radio(pDX, IDC_DGN_DAMP_DSST_AR,			m_nOption);
	DDX_Radio(pDX, IDC_DGN_DAMP_DSST_ORIGINAL_RDO,	m_nOptType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnGenDampSpecifySpecialTypeDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnGenDampSpecifySpecialTypeDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CDgnGenDampSpecifySpecialTypeDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();
	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);
	Update_InitDataByCode();
	//UpdateData(FALSE);
	return TRUE;
}

void CDgnGenDampSpecifySpecialTypeDlg::OnDgnClose()
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnGenDampSpecifySpecialTypeDlg::OnDgnExecute()
{
	UpdateData(TRUE);

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
				if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;
				if (!m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat, MatlD)) continue;
				// Beam, Truss, Wall.
				if (!m_pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp) && !m_pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp)) continue;
				rSelKey.Add(ElemK);
			}
		}

		if(CDampDesignMgr_CH::Instance()->GetDampIsoCategoryType() == 0)
		{
			T_DSST_D DsstD;
			DsstD.Initialize();
			DsstD.nDampType = m_nOptType;

			if(m_nOption == 0)
				m_pDoc->m_pDataCtrl->AddDsst(rSelKey, DsstD);
			else
				m_pDoc->m_pDataCtrl->DelDsst(rSelKey);
		}
		else
		{
			T_ISST_D IsstD;
			IsstD.Initialize();
			IsstD.nIsolationType = m_nOptType;

			if(m_nOption == 0)
				m_pDoc->m_pDataCtrl->AddIsst(rSelKey, IsstD);
			else
				m_pDoc->m_pDataCtrl->DelIsst(rSelKey);
		}


		m_pDoc->m_pViewCtrl->UnselectAll(NULL);

		//Update_InitDataByCode();
	}
	else	
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
}

void CDgnGenDampSpecifySpecialTypeDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch (lHint)
	{
	case D_UPDATE_REMOVE_DAMP:
		Update_InitDataByCode();
	break;
	default:
		//ASSERT(FALSE)->ASSERT(TRUE)로 임시변경 ('2000.2.10)
		ASSERT(TRUE);
	}
}

void CDgnGenDampSpecifySpecialTypeDlg::Update_InitDataByCode()
{
	BOOL bEnable = FALSE;
	if(CDampDesignMgr_CH::Instance()->GetDampIsoCategoryType() == 0)
	{
		if(CDampDesignMgr_CH::Instance()->GetDampIsoCodeType() == 0)
			bEnable = TRUE;
	}
	GetDlgItem(IDC_DGN_DAMP_DSST_BASE_RDO)->EnableWindow(bEnable);
	UpdateData(TRUE);
	if(!bEnable && m_nOptType == 3)
	{
		m_nOptType = 0;
		UpdateData(FALSE);
	}
}

void CDgnGenDampSpecifySpecialTypeDlg::EnableCtrl(BOOL bEnable)
{
	//GetDlgItem(IDC_DGN_UNDER_LCOM_UNDER_LOAD_RDO)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_UNDER_LCOM_NONE_UNDER_LOAD_RDO)->EnableWindow(bEnable);
}
