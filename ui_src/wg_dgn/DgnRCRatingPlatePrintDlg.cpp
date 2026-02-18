// DgnRCRatingPlatePrintDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRCRatingPlatePrintDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnRCRatingPlatePrintDlg dialog

CDgnRCRatingPlatePrintDlg::CDgnRCRatingPlatePrintDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnRCRatingPlatePrintDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnRCRatingPlatePrintDlg)
	m_nOption = 0;	
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnRCRatingPlatePrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnRCRatingPlatePrintDlg)
	DDX_Radio  (pDX, IDC_DGN_OPTION_ADD_RDO,          m_nOption);
	DDX_Control(pDX, IDC_DGN_DEFINE_MEMB_CMB,         m_cmbMemb);
	//}}AFX_DATA_MAP
}

void CDgnRCRatingPlatePrintDlg::InitCombo()
{
	m_cmbMemb.ResetContent();

	CString strMemb;
	T_RRPL_D RrplD; RrplD.Initialize();
	CArray<T_RRPL_K, T_RRPL_K> aRrplK; aRrplK.RemoveAll();
	m_pDoc->m_pAttrCtrl2->GetRrplKeyList(aRrplK);

	for(int i=0; i<aRrplK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl2->GetRrpl(aRrplK[i], RrplD)) continue;
		strMemb.Format(_T("%s"), RrplD.strName);
		m_cmbMemb.SetItemData(m_cmbMemb.AddString(strMemb), aRrplK[i]);
	}

	m_cmbMemb.SetCurSel(0);
}

BOOL CDgnRCRatingPlatePrintDlg::Dlg2Data(T_RPRP_K &rKey, T_RPRP_D &rData)
{
	UpdateData(TRUE);

	rKey = m_cmbMemb.GetItemData(m_cmbMemb.GetCurSel());

	rData.nPrintChk = 1;
	
	// 필요시 Error Check 추가
	return TRUE;
}

void CDgnRCRatingPlatePrintDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DEFAULT:
	case D_UPDATE_BUFFER_BEFORE:
	case D_UPDATE_UNIT:
		// do something...
		break;
	case D_UPDATE_BUFFER_AFTER:
		{
			UpdateBuffer();
			break;
		}
	default:
		//ASSERT(FALSE)->ASSERT(TRUE)로 임시변경 ('2000.2.10)
		ASSERT(TRUE);
	}
}

void CDgnRCRatingPlatePrintDlg::UpdateBuffer()
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
		case(UR_RACD_ADD): // RC Rating Code
		case(UR_RACD_DEL): 
			{				
				// 현재는 코드 변경시 특별한 동작이 없지만 다른 기준이 추가 되면 이부분 수정 필요
				return;
			}		
			break;
		case (UR_RRPL_ADD):
		case (UR_RRPL_DEL):
			{
				InitCombo();
			}
			break;
		default:
			break;
		}
	}
}


BEGIN_MESSAGE_MAP(CDgnRCRatingPlatePrintDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnRCRatingPlatePrintDlg)
// 	ON_BN_CLICKED(IDC_DGN_EXECUTE,            OnDgnExecute)
// 	ON_BN_CLICKED(IDC_DGN_CLOSE,              OnDgnClose)
	ON_CBN_SELCHANGE(IDC_DGN_DEFINE_MEMB_CMB, OnDgnMembCmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnRCRatingPlatePrintDlg message handlers

BOOL CDgnRCRatingPlatePrintDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	InitCombo();
	OnDgnMembCmb();

	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnRCRatingPlatePrintDlg::Execute()
{
	T_RPRP_K Key;  Key = 0;
	T_RPRP_D Data; Data.Initialize();

	if(!Dlg2Data(Key, Data)) return;

	if(m_nOption == 0) 
	{
		if(m_pDoc->m_pAttrCtrl2->ExistRprp(Key))
		{
			m_pDoc->m_pDataCtrl->ModifyRprp(Key, Key, Data);
		}
		else
		{
			m_pDoc->m_pDataCtrl->AddRprp(Key, Data);
		}
	}
	else
	{
		m_pDoc->m_pDataCtrl->DelRprp(Key);
	}
}

// void CDgnRCRatingPlatePrintDlg::OnDgnClose() 
// {
// 	// TODO: Add your control notification handler code here
// 	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
// }

void CDgnRCRatingPlatePrintDlg::OnDgnMembCmb() 
{
	UpdateData(TRUE);

	T_RROP_K Key;
	Key = m_cmbMemb.GetItemData(m_cmbMemb.GetCurSel());

	T_RRPL_D RrplD; RrplD.Initialize();
	if(!m_pDoc->m_pAttrCtrl2->GetRrpl(Key, RrplD)) return;

	CArray<T_ELEM_K, T_ELEM_K> aSelElem;  aSelElem.RemoveAll();
	aSelElem.Copy(RrplD.arElemK);
	m_pDoc->m_pViewCtrl->SelectElem(NULL, aSelElem, FALSE, TRUE);
}