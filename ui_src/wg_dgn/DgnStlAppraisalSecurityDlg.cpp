#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlAppraisalSecurityDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\ViewCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDgnStlAppraisalSecurityDlg::CDgnStlAppraisalSecurityDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnStlAppraisalSecurityDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnStlAppraisalSecurityDlg)
	m_nOption = 0;
	m_nType = 0;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnStlAppraisalSecurityDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlAppraisalSecurityDlg)
	DDX_Radio(pDX, IDC_DGN_FACTOR_AR, m_nOption);
	DDX_Radio(pDX, IDC_DGN_LIVE_FACTOR_RADIO1, m_nType);
	DDX_Control(pDX, IDC_DGN_1_COMBO, m_LevelCombox);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnStlAppraisalSecurityDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnStlAppraisalSecurityDlg)
	//ON_BN_CLICKED(IDC_DGN_IMEL_AR, OnDgnImelAr)
	//ON_BN_CLICKED(IDC_DGN_IMEL_DEL, OnDgnImelDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CDgnStlAppraisalSecurityDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	T_TAIF_D TaifD;
	TaifD.Initialize();
	m_pDoc->m_pAttrCtrl->GetTaif(TaifD);
	m_LevelCombox.ResetContent();
	if (TaifD.nBldType == 0)
	{
		CString strTemp[] = { _T("a"),_T("b"),_T("c"),_T("d") };
		for (int i = 0; i < 4; i++)
		{
			m_LevelCombox.SetItemData(m_LevelCombox.AddString(strTemp[i]), i);
		}
		GetDlgItem(IDC_DGN_LIVE_FACTOR_RADIO3)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_LIVE_FACTOR_RADIO4)->EnableWindow(FALSE);
	}
	else
	{
		CString strTemp[] = { _T("au"),_T("bu"),_T("cu"),_T("du") };
		for (int i = 0; i < 4; i++)
		{
			m_LevelCombox.SetItemData(m_LevelCombox.AddString(strTemp[i]), i);
		}
		GetDlgItem(IDC_DGN_LIVE_FACTOR_RADIO3)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_LIVE_FACTOR_RADIO4)->EnableWindow(TRUE);
	}
	m_LevelCombox.SetCurSel(0);

	return TRUE;
}

void CDgnStlAppraisalSecurityDlg::Execute() 
{
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);
	if(aSelKey.IsEmpty())
		return;

	if (m_nOption == 0)
	{
		T_DSAS_D rData;
		rData.Initialize();
		rData.nEvaluateLevel[m_nType] = m_LevelCombox.GetCurSel();
		if(m_pDoc->m_pDataCtrl->AddDsas(aSelKey, rData))
			m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
	}
	else
	{
		if(m_pDoc->m_pDataCtrl->DelDsas(aSelKey))
			m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
	}
}

void CDgnStlAppraisalSecurityDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
			{
				Update_InitDataByTaif();
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

void CDgnStlAppraisalSecurityDlg::Update_InitDataByTaif()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0)
		return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bTaifChanged = FALSE;

	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;

		switch (nCmd)
		{
		case(UR_TAIF_ADD):
		case(UR_TAIF_DEL):
		case(UR_TAIF_MFS):
		case(UR_TAIF_MFD):
			bTaifChanged = TRUE;
			break;
		}
	}
	if (bTaifChanged)
	{
		T_TAIF_D TaifD;
		TaifD.Initialize();
		m_pDoc->m_pAttrCtrl->GetTaif(TaifD);
		int nCursel = m_LevelCombox.GetCurSel();
		m_LevelCombox.ResetContent();
		if (TaifD.nBldType == 0)
		{
			CString strTemp[] = { _T("a"),_T("b"),_T("c"),_T("d") };
			for (int i = 0; i < 4; i++)
			{
				m_LevelCombox.SetItemData(m_LevelCombox.AddString(strTemp[i]), i);
			}
			GetDlgItem(IDC_DGN_LIVE_FACTOR_RADIO3)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_LIVE_FACTOR_RADIO4)->EnableWindow(FALSE);
			UpdateData(TRUE);
			if (m_nType > 1)
			{
				m_nType = 0;
				UpdateData(FALSE);
			}
		}
		else
		{
			CString strTemp[] = { _T("au"),_T("bu"),_T("cu"),_T("du") };
			for (int i = 0; i < 4; i++)
			{
				m_LevelCombox.SetItemData(m_LevelCombox.AddString(strTemp[i]), i);
			}
			GetDlgItem(IDC_DGN_LIVE_FACTOR_RADIO3)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_LIVE_FACTOR_RADIO4)->EnableWindow(TRUE);
		}
		m_LevelCombox.SetCurSel(nCursel);
	}
}
