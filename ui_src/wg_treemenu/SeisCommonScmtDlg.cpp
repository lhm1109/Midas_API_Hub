// SeisCommonScmtDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCommonScmtDlg.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_DataCtrl.h"
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
// CSeisCommonScmtDlg dialog

CSeisCommonScmtDlg::CSeisCommonScmtDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSeisCommonScmtDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisCommonScmtDlg)
	m_nAddDel = 0;
	m_nCompType = 0;
	m_pDoc = CDBDoc::GetDocPoint();
}

CSeisCommonScmtDlg::~CSeisCommonScmtDlg()
{

}

void CSeisCommonScmtDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisCommonScmtDlg)
	DDX_Radio(pDX, IDC_TM_ADDREP_RDO, m_nAddDel);
	DDX_Radio(pDX, IDC_TM_SCMT_PRI_RDO, m_nCompType);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CSeisCommonScmtDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CSeisCommonScmtDlg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisCommonScmtDlg message handlers

void CSeisCommonScmtDlg::Execute()
{
	UpdateData(TRUE);

	CArray<T_ELEM_K,T_ELEM_K> aElemK;
	m_pDoc->m_pViewCtrl->GetAllSelectedElem(aElemK);

	// RC 기둥,보에만 적용 가능
//   for(int i=aElemK.GetSize()-1; i>=0; i--)
//   {
//     T_ELEM_K ElemK = aElemK[i];
//     T_ELEM_D ElemD;
//     m_pDoc->m_pAttrCtrl->GetElem(ElemK,ElemD);
//     T_MATL_D MatlD;
//     m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat,MatlD);
//     if(MatlD.Type!=_T("C") || (m_pDoc->m_pAttrCtrl->GetMemberType(ElemK)!=1 && m_pDoc->m_pAttrCtrl->GetMemberType(ElemK)!=2))
//       aElemK.RemoveAt(i);
//   }

	T_SCMT_D ScmtD;
	ScmtD.Initialize();
	ScmtD.nType = m_nCompType;

	if(m_nAddDel == 0) m_pDoc->m_pDataCtrl->AddScmt(aElemK, ScmtD);
	else               m_pDoc->m_pDataCtrl->DelScmt(aElemK);

	if (m_nAddDel == 0)
	{
		IUsageCounter::Use(_T("SPE_SCMT"));
	}

	m_pDoc->m_pViewCtrl->UnselectAll(NULL);

	// 초기화
	m_nCompType=0;
	UpdateData(FALSE);
}

BOOL CSeisCommonScmtDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
//
// DB Update

void CSeisCommonScmtDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CSeisCommonScmtDlg::UpdateBuffer()
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
		case(UR_SCMT_ADD): 
		case(UR_SCMT_DEL): 
			break;
		default:
			break;
		}
	} // end of while
}

void CSeisCommonScmtDlg::PostNcDestroy()
{
	CMenuBarChildDlg::PostNcDestroy();
}