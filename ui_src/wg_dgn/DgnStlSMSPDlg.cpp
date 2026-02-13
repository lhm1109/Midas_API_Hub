// DgnStlSMSPDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlSMSPDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"

#include "..\wg_main\wg_mainres.h"

// CDgnStlSMSPDlg 대화 상자입니다.

CDgnStlSMSPDlg::CDgnStlSMSPDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnStlSMSPDlg::IDD, pParent)
{
	m_nAddDel	= 0;
	m_nMethod	= 0;
	m_pDoc = CDBDoc::GetDocPoint();
}

CDgnStlSMSPDlg::~CDgnStlSMSPDlg()
{
}

void CDgnStlSMSPDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		//UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		//UpdateBuffer();
		break;
	case D_UPDATE_NOTIFY:
		if(pHint!=NULL)
		{
			CString strCommand = ((T_UPDATE_HINT*)pHint)->csNotifyCmd;
			if(strCommand == _LSX(STLCODE_UPDATE)) CloseDlg();
		}
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CDgnStlSMSPDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_SMSP_ADD): 
		case(UR_SMSP_DEL): 
			break;
		default:
			break;
		}
	} // end of while
}

void CDgnStlSMSPDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_DGN_AR_RDO, m_nAddDel);
	DDX_Radio(pDX, IDC_DGN_SMSP_SRSS_RDO, m_nMethod);
}

BEGIN_MESSAGE_MAP(CDgnStlSMSPDlg, CMenuBarChildDlg)
END_MESSAGE_MAP()

void CDgnStlSMSPDlg::Execute()
{
	UpdateData(TRUE);
	CArray<T_ELEM_K,T_ELEM_K> aSelectElemK;
	CArray<T_ELEM_K,T_ELEM_K> aSmspK;

	m_pDoc->m_pViewCtrl->GetAllSelectedElem(aSelectElemK);
	
	for(int i = 0; i<aSelectElemK.GetCount(); i++)
	{
		T_ELEM_K ElemK = aSelectElemK[i];
		
		if(!m_pDoc->m_pAttrCtrl->IsSmsp(ElemK)) continue;

		aSmspK.Add(ElemK);
	}

	T_SMSP_D SmspD;
	SmspD.Initialize();
	SmspD.nMethod = m_nMethod;

	if(m_nAddDel == 0)	m_pDoc->m_pDataCtrl->AddSmsp(aSmspK, SmspD);
	else				m_pDoc->m_pDataCtrl->DelSmsp(aSmspK);

	m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	UpdateData(FALSE);
}

BOOL CDgnStlSMSPDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	if(m_pDoc==NULL)	return TRUE;

	// Apply Close Button Hide
	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;
}
