// SeisCommonSehkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCommonSehkDlg.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\IUsageCounter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisCommonSehkDlg dialog

CSeisCommonSehkDlg::CSeisCommonSehkDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSeisCommonSehkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisCommonSehkDlg)
	m_nAddDel = 0;
	m_bUseHook = FALSE;
	m_pDoc = CDBDoc::GetDocPoint();
}

CSeisCommonSehkDlg::~CSeisCommonSehkDlg()
{

}

void CSeisCommonSehkDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisCommonSehkDlg)
	DDX_Radio(pDX, IDC_TM_ADDREP_RDO, m_nAddDel);
	DDX_Check(pDX, IDC_TM_SECH_USE_CHK, m_bUseHook);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CSeisCommonSehkDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CSeisCommonSehkDlg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisCommonSehkDlg message handlers

void CSeisCommonSehkDlg::Execute()
{
	UpdateData(TRUE);

	CArray<T_ELEM_K,T_ELEM_K> aElemK;
	m_pDoc->m_pViewCtrl->GetAllSelectedElem(aElemK);
 
	// RC 기둥,보에만 적용 가능
	for(int i=aElemK.GetSize()-1; i>=0; i--)
	{
		T_ELEM_K ElemK = aElemK[i];
		T_ELEM_D ElemD;
		m_pDoc->m_pAttrCtrl->GetElem(ElemK,ElemD);
		T_MATL_D MatlD;
		m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat,MatlD);
		if(MatlD.Type!=_T("C") || (m_pDoc->m_pAttrCtrl->GetMemberType(ElemK)!=1 && m_pDoc->m_pAttrCtrl->GetMemberType(ElemK)!=2))
			aElemK.RemoveAt(i);
	}

	T_SEHK_D SehkD;
	SehkD.Initialize();
	SehkD.bUseHook = m_bUseHook;

	if(m_nAddDel == 0) m_pDoc->m_pDataCtrl->AddSehk(aElemK, SehkD);
	else               m_pDoc->m_pDataCtrl->DelSehk(aElemK);

	if (m_nAddDel == 0)
	{
		IUsageCounter::Use(_T("SPE_SEHK"));
	}

	m_pDoc->m_pViewCtrl->UnselectAll(NULL);

	// 초기화
	m_bUseHook=FALSE;
	UpdateData(FALSE);
}

BOOL CSeisCommonSehkDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	T_SEME_D SemeD;

	m_pDoc->m_pAttrCtrl2->GetSeme(SemeD);
	SetDescriptionByCode(SemeD.nMethod);

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
//
// DB Update

void CSeisCommonSehkDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CSeisCommonSehkDlg::UpdateBuffer()
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
		case(UR_SEHK_ADD): 
		case(UR_SEHK_DEL):
			break;
		case(UR_SEME_ADD):
			{
				T_SEME_D SemeD;

				pViewBuff->GetSeme(nKey, SemeD);
				SetDescriptionByCode(SemeD.nMethod);
			}
			break;
		default:
			break;
		}
	} // end of while
}

void CSeisCommonSehkDlg::PostNcDestroy()
{
	CMenuBarChildDlg::PostNcDestroy();
}

void CSeisCommonSehkDlg::SetDescriptionByCode(int nMethod)
{
	if(nMethod == D_SEME_METHOD_KISTEC2013)
		SetDlgItemText(IDC_TM_SECH_USE_STC, _LS(IDS_IDD_TM_SEISCOMMON_SEHK_IDC_TM_SECH_USE_STC));
	else
		SetDlgItemText(IDC_TM_SECH_USE_STC, _LS(IDS_IDD_TM_SEISCOMMON_SEHK_IDC_TM_SECH_USE_STC_BEAM));
}
