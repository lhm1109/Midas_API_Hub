// SeisCommonRccmDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCommonRccmDlg.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\SeisEvalDataTool.h"
#include "..\wg_db\IUsageCounter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CSeisCommonRccmDlg 대화 상자입니다.

CSeisCommonRccmDlg::CSeisCommonRccmDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSeisCommonRccmDlg::IDD, pParent)
{
	m_nAddDel		= 0;
	m_nColmType		= 0;
	m_pDoc = CDBDoc::GetDocPoint();
}

CSeisCommonRccmDlg::~CSeisCommonRccmDlg()
{
}

void CSeisCommonRccmDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_TM_ADDREP_RDO, m_nAddDel);
	DDX_Radio(pDX, IDC_TM_RCCM_COLUMN_M_RDO, m_nColmType);
}


#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CSeisCommonRccmDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CSeisCommonSerdDlg)
END_MESSAGE_MAP()


// CSeisCommonRccmDlg 메시지 처리기입니다.

void CSeisCommonRccmDlg::Execute()
{
	UpdateData(TRUE);
	CArray<T_ELEM_K,T_ELEM_K> aSelectedColmK;
	GetSelctedColumnList(aSelectedColmK);

	T_RCCM_D RccmD;
	RccmD.Initialize();
	RccmD.nLocalYType = m_nColmType;
	RccmD.nLocalZType = m_nColmType;

	if(m_nAddDel == 0)	m_pDoc->m_pDataCtrl->AddRccm(aSelectedColmK, RccmD);
	else				m_pDoc->m_pDataCtrl->DelRccm(aSelectedColmK);

	if (m_nAddDel == 0)
	{
		IUsageCounter::Use(_T("SPE_RCCM"));
	}

	m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	UpdateData(FALSE);
}

BOOL CSeisCommonRccmDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSeisCommonRccmDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
		{
			UpdateBuffer();
			T_UPDATE_HINT Hint;
			Hint.csNotifyCmd = _LSX(RCCM_UPDATE);
			m_pDoc->UpdateAllViews(NULL, D_UPDATE_NOTIFY, &Hint);
		}
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CSeisCommonRccmDlg::UpdateBuffer()
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
		case(UR_RCCM_ADD): 
		case(UR_RCCM_DEL): 
			break;
		default:
			break;
		}
	} // end of while
}

void CSeisCommonRccmDlg::GetSelctedRccmKeyList(CArray<T_ELEM_K,T_ELEM_K>& aSelectedRccmK)
{
	CArray<T_ELEM_K,T_ELEM_K> aSelectElemK;
	m_pDoc->m_pViewCtrl->GetAllSelectedElem(aSelectElemK);

	CArray<T_ELEM_K,T_ELEM_K> aRccmK;
	//현재 모델의 RC Column M(RCCM) Elem를 모두 가져옴
	GetRccmKeyList(aRccmK);

	CArray<T_ELEM_K,T_ELEM_K> aElemK;
	CArray<T_MEMB_K,T_MEMB_K> aMembK;

	//RCCM이면서 선택된 요소이면 리스트에 넣음
	for(int i = 0; i<aRccmK.GetCount(); i++)
	{
		for(int j = 0; j<aSelectElemK.GetCount(); j++)
		{
			if(aRccmK[i] == aSelectElemK[j])
			{
				aSelectedRccmK.Add(aRccmK[i]);
				break;
			}
		}
	}
}

void CSeisCommonRccmDlg::GetRccmKeyList(CArray<T_ELEM_K,T_ELEM_K>& aRccmK)
{
	CArray<T_ELEM_K,T_ELEM_K> aElemK;
    CSeisEvalDataTool Tool;
    Tool.GetEvalInfilledConColmKeyList(aElemK);
	for(int i = 0; i<aElemK.GetCount(); i++)
	{
		// Member가 아닌 요소나, Member의 대표요소만 필터링
		T_MEMB_K MembK=0;
		if(m_pDoc->m_pAttrCtrl->GetMembAssigned(aElemK[i], MembK) && m_pDoc->m_pAttrCtrl->GetFirstElemKInMemb(aElemK[i]) != aElemK[i])
			continue;
		aRccmK.Add(aElemK[i]);
	}
}

void CSeisCommonRccmDlg::GetSelctedColumnList(CArray<T_ELEM_K,T_ELEM_K>& aColmK)
{
	CArray<T_ELEM_K,T_ELEM_K> aSelectElemK;

	m_pDoc->m_pViewCtrl->GetAllSelectedElem(aSelectElemK);

	for(int i = 0; i<aSelectElemK.GetCount(); i++)
	{
		if(m_pDoc->m_pAttrCtrl->IsColumn(aSelectElemK[i]))
			aColmK.Add(aSelectElemK[i]);
	}
}

void CSeisCommonRccmDlg::PostNcDestroy()
{
	CMenuBarChildDlg::PostNcDestroy();
}