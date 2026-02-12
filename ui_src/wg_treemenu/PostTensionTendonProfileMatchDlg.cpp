#include "stdafx.h"
#include "wg_treemenu.h"
#include "PostTensionTendonProfileMatchDlg.h"

#include "..\wg_base\TreeMenuBarBase.h"
#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\ServiceProvider.h"

#include "..\wg_gr\Gr_ServiceDef.h"
//#include "..\wg_gr\I_GENModel.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "..\wg_cmd\ExportFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum
{
    EN_ELEM_BEAM = 0,
    EN_ELEM_SLAB,
    EN_ELEM_WALL,
};

CPostTensionTendonProfileMatchDlg::CPostTensionTendonProfileMatchDlg(CWnd* pParent)
    : CMenuBarChildDlg(CPostTensionTendonProfileMatchDlg::IDD, pParent)
{
    m_pDoc = CDBDoc::GetDocPoint();

    m_nElemType = 0;
}

CPostTensionTendonProfileMatchDlg::~CPostTensionTendonProfileMatchDlg()
{
    InitSelectionFilter();
}

void CPostTensionTendonProfileMatchDlg::DoDataExchange(CDataExchange* pDX)
{
    CMenuBarChildDlg::DoDataExchange(pDX);
    // Element Type
    DDX_Radio(pDX, IDC_WG_TREEMENU_MATCH_PROFILE_TYPE_BEAM_RDO, m_nElemType);
    // Tendon Profile
	DDX_Control(pDX, IDC_WG_TREEMENU_MATCH_PROFILE_REF_PROFILE_CMB, m_ComboTendon);
    // Tendon Stressing
	DDX_Control(pDX, IDC_WG_TREEMENU_MATCH_PROFILE_REF_STRESSING_CMB, m_ComboStressing);
}

BEGIN_MESSAGE_MAP(CPostTensionTendonProfileMatchDlg, CMenuBarChildDlg)
	// Element Type
	ON_BN_CLICKED(IDC_WG_TREEMENU_MATCH_PROFILE_TYPE_BEAM_RDO, OnClickElemTypeRadio)
	ON_BN_CLICKED(IDC_WG_TREEMENU_MATCH_PROFILE_TYPE_SLAB_RDO, OnClickElemTypeRadio)
	ON_BN_CLICKED(IDC_WG_TREEMENU_MATCH_PROFILE_TYPE_WALL_RDO, OnClickElemTypeRadio)
    // Tendon Profile
    ON_BN_CLICKED(IDC_WG_TREEMENU_MATCH_PROFILE_REF_PROFILE_BTN, OnClickTendonProfileBtn)
	// Tendon Stressing
    ON_BN_CLICKED(IDC_WG_TREEMENU_MATCH_PROFILE_REF_STRESSING_BTN, OnClickTendonStressingBtn)
END_MESSAGE_MAP()

BOOL CPostTensionTendonProfileMatchDlg::OnInitDialog()
{
    CMenuBarChildDlg::OnInitDialog();

    SetInitComboBoxString();

    UpdateData(FALSE);

    return TRUE;
}

void CPostTensionTendonProfileMatchDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    HWND hWnd;
    if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

    switch (lHint)
    {
    case D_UPDATE_DBALL:
    case D_UPDATE_DEFAULT:
    case D_UPDATE_BUFFER_BEFORE:
        break;
    case D_UPDATE_BUFFER_AFTER:
        UpdateBuffer();
        break;
    case D_UPDATE_UNIT:
        break;
    case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
        break;
    case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
        break;
    default:
        //ASSERT(FALSE);
        break;
    }
}

void CPostTensionTendonProfileMatchDlg::UpdateBuffer()
{
    CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
    int nCount = pViewBuff->GetCount();
    if (nCount == 0) return;
    ASSERT(nCount == 1);

    T_UDRD_BUFFER buffer_ur;
    POSITION pos = pViewBuff->GetStartBuffer();

    BOOL bMFD = FALSE;
    while (pos != NULL)
    {
        buffer_ur = pViewBuff->GetNextBuffer(pos);

        int nCmd = buffer_ur.nCmd;
        int nKey = buffer_ur.nKey;

        switch (nCmd)
        {
        case(UR_TDST_ADD):
        case(UR_TDST_DEL):
        case(UR_TDST_MFD):
        case(UR_TDST_MFS):
        case(UR_TDPF_ADD):
        case(UR_TDPF_DEL):
        case(UR_TDPF_MFD):
        case(UR_TDPF_MFS):
            SetInitComboBoxString();
            break;
        default:
            break;
        }
    }
}

void CPostTensionTendonProfileMatchDlg::SetInitComboBoxString()
{
    // 현재 Element Type에 맞는 선택 필터 재설정
    enum EN_SELECT_ELEM_FILTER enFilter;
    switch (m_nElemType)
    {
    case EN_ELEM_BEAM:
        enFilter = EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_TDBM;
        break;
    case EN_ELEM_SLAB:
        enFilter = EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_TDSL;
        break;
    case EN_ELEM_WALL:
    default:
        enFilter = EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_TDBM;
        break;
    }

    // 선택 필터 변경
    I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
    if (pIGM)
    {
        // 선택 필터 변경
        ChangeSelectionFilter(enFilter);

        // 모든 선택 해제
        pIGM->UnselectAllElem();

    }

    // 콤보박스 초기화
    SetInitCmbStringProfile();
    SetInitCmbStringStressing();

    // View 갱신하여 Select 데이터 생성 (On_SelectModeChanged 트리거)
    // OnSelElemFilterCombo와 동일하게 D_UPDATE_NOTIFY 사용
    T_UPDATE_HINT updateHint;
    switch (m_nElemType)
    {
    case EN_ELEM_BEAM:
        updateHint.csNotifyCmd = _LSX(ELEM_FILTER_TDBM);
        break;
    case EN_ELEM_SLAB:
        updateHint.csNotifyCmd = _LSX(ELEM_FILTER_TDSL);
        break;
    case EN_ELEM_WALL:
        updateHint.csNotifyCmd = _LSX(ELEM_FILTER_TDBM);
        break;
    default:
        updateHint.csNotifyCmd = _LSX(ELEM_FILTER_NODE_ELEM);
        break;
    }
    m_pDoc->UpdateAllViews(NULL, D_UPDATE_NOTIFY, &updateHint);

    // Dialog 초기화 시 View를 완전히 갱신
    // D_UPDATE_REFRESH로 강제 갱신하여 TDSL/TDBM이 올바르게 표시되도록 함
    m_pDoc->UpdateAllViews(NULL, D_UPDATE_REFRESH, &updateHint);
}

void CPostTensionTendonProfileMatchDlg::SetInitCmbStringProfile()
{
    m_ComboTendon.ResetContent();

    CArray<T_TDPF_K, T_TDPF_K> aTdpfK;
    aTdpfK.RemoveAll();
    m_pDoc->m_pAttrCtrl2->GetTdpfKeyList(aTdpfK);

    T_TDPF_D TdpfD;

    for (auto i = 0; i < aTdpfK.GetSize(); i++)
    {
        if (!m_pDoc->m_pAttrCtrl2->GetTdpf(aTdpfK[i], TdpfD))
        {
            ASSERT(0); return;
        }

        if (m_nElemType == TdpfD.nElemType)
        {
            m_ComboTendon.AddString(TdpfD.strName);
        }
    }

    m_ComboTendon.SetCurSel(0);
}

void CPostTensionTendonProfileMatchDlg::SetInitCmbStringStressing()
{
    m_ComboStressing.ResetContent();

    CArray<T_TDST_K, T_TDST_K> aTdstK;
    aTdstK.RemoveAll();
    m_pDoc->m_pAttrCtrl2->GetTdstKeyList(aTdstK);

    T_TDST_D TdstD;

    for (auto i = 0; i < aTdstK.GetSize(); i++)
    {
        if (!m_pDoc->m_pAttrCtrl2->GetTdst(aTdstK[i], TdstD))
        {
            ASSERT(0); return;
        }

        m_ComboStressing.AddString(TdstD.strName);
    }

    m_ComboStressing.SetCurSel(0);
}

void CPostTensionTendonProfileMatchDlg::InitSelectionFilter()
{
    I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
    pIGM->ChangeSelectionFilterCombo(EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM);
}

void CPostTensionTendonProfileMatchDlg::ChangeSelectionFilter(EN_SELECT_ELEM_FILTER enFilter)
{
    I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
    pIGM->ChangeSelectionFilterCombo(enFilter);
}

void CPostTensionTendonProfileMatchDlg::GetSelectedTendonLineKeyList(CArray<T_TDBM_K, T_TDBM_K>& aKey)
{
    I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
    if (!pIGM)
    {
        ASSERT(0); return;
    }

    CArray<T_TDBM_K, T_TDBM_K> aTdbmK;
    m_pDoc->m_pAttrCtrl2->GetTdbmKeyList(aTdbmK);

    CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance();
    ASSERT(pServiceHeadOffice);

    for (auto i = 0; i < aTdbmK.GetSize(); i++)
    {
        if (pServiceHeadOffice->ReqService(_ULS(gr), D_GR_GET_SELECTED_TDBM, (void*)(&aTdbmK[i])))
        {
            aKey.Add(aTdbmK[i]);
        }
    }
}

void CPostTensionTendonProfileMatchDlg::GetSelectedTendonSlabKeyList(CArray<T_TDSL_K, T_TDSL_K>& aKey)
{
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	if (!pIGM)
	{
		ASSERT(0); return;
	}

	CArray<T_TDSL_K, T_TDSL_K> aTdslK;
	m_pDoc->m_pAttrCtrl2->GetTdslKeyList(aTdslK);

	CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance();
	ASSERT(pServiceHeadOffice);

	for (auto i = 0; i < aTdslK.GetSize(); i++)
	{
		if (pServiceHeadOffice->ReqService(_ULS(gr), D_GR_GET_SELECTED_TDSL, (void*)(&aTdslK[i])))
		{
			aKey.Add(aTdslK[i]);
		}
	}
}

void CPostTensionTendonProfileMatchDlg::OnClickElemTypeRadio()
{
    UpdateData(TRUE);

    SetInitComboBoxString();

    T_UPDATE_HINT updateHint;
    switch (m_nElemType)
    {
    case EN_ELEM_BEAM:
        updateHint.csNotifyCmd = _LSX(ELEM_FILTER_TDBM);
        break;
    case EN_ELEM_SLAB:
        updateHint.csNotifyCmd = _LSX(ELEM_FILTER_TDSL);
        break;
    case EN_ELEM_WALL:
        updateHint.csNotifyCmd = _LSX(ELEM_FILTER_TDBM);
        break;
    default:
        ASSERT(false);
        break;
    }

    // 라디오 버튼 변경 시 화면 갱신
    m_pDoc->UpdateAllViews(NULL, D_UPDATE_REFRESH, &updateHint);

    UpdateData(FALSE);
}

void CPostTensionTendonProfileMatchDlg::OnClickTendonProfileBtn()
{
    CMD_DoModal1(D_CMD_PT_SET_TENDON_PROFILE_DLG);
}

void CPostTensionTendonProfileMatchDlg::OnClickTendonStressingBtn()
{
    CMD_DoModal1(D_CMD_PT_SET_TENDON_STRESSING_DLG);
}

void CPostTensionTendonProfileMatchDlg::Execute()
{
	switch (m_nElemType)
	{
	case EN_ELEM_BEAM:
        ExecuteBeam();
		break;
	case EN_ELEM_SLAB:
        ExecuteSlab();
		break;
	default:
		ASSERT(0);
		break;
	}
}

void CPostTensionTendonProfileMatchDlg::ExecuteBeam()
{
	T_TDBM_K_LIST aTdbmK;
	aTdbmK.RemoveAll();
	GetSelectedTendonLineKeyList(aTdbmK);

	CString strTendonProfile;
	auto nCurSelProfile = m_ComboTendon.GetCurSel();
	m_ComboTendon.GetLBText(nCurSelProfile, strTendonProfile);
	T_TDPF_K TdpfK = m_pDoc->m_pAttrCtrl2->GetTdpfKey(strTendonProfile);

	CString strTendonStressing;
	auto nCurSelStressing = m_ComboStressing.GetCurSel();
	m_ComboStressing.GetLBText(nCurSelStressing, strTendonStressing);
	T_TDST_K TdstK = m_pDoc->m_pAttrCtrl2->GetTdstKey(strTendonStressing);

	T_TDBM_D TdbmD;
	for (auto i = 0; i < aTdbmK.GetSize(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl2->GetTdbm(aTdbmK[i], TdbmD))
		{
			ASSERT(0); return;
		}

		TdbmD.TdpfK = TdpfK;
		TdbmD.TdstK = TdstK;

		if (!m_pDoc->m_pDataCtrl->ModifyTdbm(aTdbmK[i], aTdbmK[i], TdbmD))
		{
			ASSERT(0); return;
		}
	}
}

void CPostTensionTendonProfileMatchDlg::ExecuteSlab()
{
	T_TDSL_K_LIST aTdslK;
	GetSelectedTendonSlabKeyList(aTdslK);

	CString strTendonProfile;
	auto nCurSelProfile = m_ComboTendon.GetCurSel();
	m_ComboTendon.GetLBText(nCurSelProfile, strTendonProfile);
	T_TDPF_K TdpfK = m_pDoc->m_pAttrCtrl2->GetTdpfKey(strTendonProfile);

	CString strTendonStressing;
	auto nCurSelStressing = m_ComboStressing.GetCurSel();
	m_ComboStressing.GetLBText(nCurSelStressing, strTendonStressing);
	T_TDST_K TdstK = m_pDoc->m_pAttrCtrl2->GetTdstKey(strTendonStressing);

	T_TDSL_D TdslD;
	for (auto i = 0; i < aTdslK.GetSize(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl2->GetTdsl(aTdslK[i], TdslD))
		{
			ASSERT(0); return;
		}

		TdslD.TdpfK = TdpfK;
        TdslD.TdstK = TdstK;

		if (!m_pDoc->m_pDataCtrl->ModifyTdsl(aTdslK[i], aTdslK[i], TdslD))
		{
			ASSERT(0); return;
		}
	}
}
