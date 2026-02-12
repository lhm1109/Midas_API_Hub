// SeisCommonSechDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCommonSechDlg.h"

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
// CSeisCommonSechDlg dialog

CSeisCommonSechDlg::CSeisCommonSechDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSeisCommonSechDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisCommonSechDlg)
	m_nAddDel = 0;
	m_pDoc = CDBDoc::GetDocPoint();
}

CSeisCommonSechDlg::~CSeisCommonSechDlg()
{

}

void CSeisCommonSechDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisCommonSechDlg)
	DDX_Radio(pDX, IDC_TM_ADDREP_RDO, m_nAddDel);
	DDX_Control(pDX, IDC_TM_SECH_H0Y_EDT, m_edth0y);
	DDX_Control(pDX, IDC_TM_SECH_H0Y_UNT, m_unth0y);
	DDX_Control(pDX, IDC_TM_SECH_H0Z_EDT, m_edth0z);
	DDX_Control(pDX, IDC_TM_SECH_H0Z_UNT, m_unth0z);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CSeisCommonSechDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CSeisCommonSechDlg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisCommonSechDlg message handlers

void CSeisCommonSechDlg::Execute()
{
	UpdateData(TRUE);

	CArray<T_ELEM_K,T_ELEM_K> aElemK;
	m_pDoc->m_pViewCtrl->GetAllSelectedElem(aElemK);

	// RC 기둥에만 적용 가능
	for(int i=aElemK.GetSize()-1; i>=0; i--)
	{
		T_ELEM_K ElemK = aElemK[i];
		T_ELEM_D ElemD;
		m_pDoc->m_pAttrCtrl->GetElem(ElemK,ElemD);
		T_MATL_D MatlD;
		m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat,MatlD);
		if(MatlD.Type!=_T("C") || m_pDoc->m_pAttrCtrl->GetMemberType(ElemK)!=1)
			aElemK.RemoveAt(i);
	}

	T_SECH_D SechD;
	SechD.Initialize();
	SechD.dh0y = m_edth0y.GetEditValue();
	SechD.dh0z = m_edth0z.GetEditValue();

	if(m_nAddDel == 0) m_pDoc->m_pDataCtrl->AddSech(aElemK, SechD);
	else               m_pDoc->m_pDataCtrl->DelSech(aElemK);

	if (m_nAddDel == 0)
	{
		if (m_pDoc->m_pAttrCtrl2->IsSemeMethod_KISTEC_Series() || m_pDoc->m_pAttrCtrl2->IsSemeMethod_MOE_Series())
		{
			IUsageCounter::Use(_T("SPE_SECH"));
		}
		else if (m_pDoc->m_pAttrCtrl2->IsSemeMethod_AIK_2021())
		{
			IUsageCounter::Use(_T("SPD_SECH"));
		}
		else
		{
			ASSERT(0);
		}
	}

	m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

BOOL CSeisCommonSechDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	m_unth0y.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unth0z.SetUnitType(D_UNITSYS_BASE_LENGTH);
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
//
// DB Update

void CSeisCommonSechDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CSeisCommonSechDlg::UpdateBuffer()
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
		case(UR_SECH_ADD): 
		case(UR_SECH_DEL): 
			break;
		default:
			break;
		}
	} // end of while
}

void CSeisCommonSechDlg::PostNcDestroy()
{
	CMenuBarChildDlg::PostNcDestroy();
}