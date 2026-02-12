#include "stdafx.h"
#include "wg_treemenu.h"
#include "PostTensionTendonElemDeleteDlg.h"

#include "..\wg_gr\Gr_ServiceDef.h"

#include "..\wg_base\ServiceProvider.h"
#include "..\wg_base\I_GENStateDefine.h"

#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

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

CPostTensionTendonElemDeleteDlg::CPostTensionTendonElemDeleteDlg(CWnd* pParent)
    : CMenuBarChildDlg(CPostTensionTendonElemDeleteDlg::IDD, pParent)
{
    m_pDoc = CDBDoc::GetDocPoint();

    m_nElemType = 0;
}

CPostTensionTendonElemDeleteDlg::~CPostTensionTendonElemDeleteDlg()
{
    ChangeSelectionFilter(EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM);
}

void CPostTensionTendonElemDeleteDlg::DoDataExchange(CDataExchange* pDX)
{
    CMenuBarChildDlg::DoDataExchange(pDX);
    DDX_Radio(pDX, IDC_TM_TENDON_ELEM_TYPE_BEAM_RDO, m_nElemType);
}

BEGIN_MESSAGE_MAP(CPostTensionTendonElemDeleteDlg, CMenuBarChildDlg)
    ON_BN_CLICKED(IDC_TM_TENDON_ELEM_TYPE_BEAM_RDO, OnClickElemTypeRadio)
    ON_BN_CLICKED(IDC_TM_TENDON_ELEM_TYPE_SLAB_RDO, OnClickElemTypeRadio)
    ON_BN_CLICKED(IDC_TM_TENDON_ELEM_TYPE_WALL_RDO, OnClickElemTypeRadio)
END_MESSAGE_MAP()

BOOL CPostTensionTendonElemDeleteDlg::OnInitDialog()
{
    CMenuBarChildDlg::OnInitDialog();

    InitSelectionFilter();

    return FALSE;
}

void CPostTensionTendonElemDeleteDlg::OnClickElemTypeRadio()
{
    UpdateData(TRUE);

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
            ASSERT(0);
            break;
    }

    ChangeSelectionFilter(enFilter);
}

void CPostTensionTendonElemDeleteDlg::Execute4Beam()
{
    T_TDBM_K_LIST aTdbmK;
    aTdbmK.RemoveAll();
    GetSelectedTendonLineKeyList(D_GR_GET_SELECTED_TDBM, aTdbmK);

    if (!m_pDoc->m_pDataCtrl->DelTdbm(aTdbmK))
    {
        ASSERT(0); return;
    }
}

void CPostTensionTendonElemDeleteDlg::Execute4Slab()
{
    T_TDSL_K_LIST aTdslK;
    aTdslK.RemoveAll();
    GetSelectedTendonLineKeyList(D_GR_GET_SELECTED_TDSL, aTdslK);

    if (!m_pDoc->m_pDataCtrl->DelTdsl(aTdslK))
    {
        ASSERT(0); return;
    }
}

void CPostTensionTendonElemDeleteDlg::Execute4Wall()
{
    ASSERT(0); return;
}

void CPostTensionTendonElemDeleteDlg::GetSelectedTendonLineKeyList(UINT enTendonType, CArray<UINT, UINT>& aKey)
{
    I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
    if (!pIGM)
    {
        ASSERT(0); return;
    }

    CArray<UINT, UINT> aTendonLineK;
    switch (enTendonType)
    {
    case D_GR_GET_SELECTED_TDBM:
        m_pDoc->m_pAttrCtrl2->GetTdbmKeyList(aTendonLineK);
        break;
    case D_GR_GET_SELECTED_TDSL:
        m_pDoc->m_pAttrCtrl2->GetTdslKeyList(aTendonLineK);
        break;
    default:
        ASSERT(0); return;
    }

    CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance();
    ASSERT(pServiceHeadOffice);

    for (auto i = 0; i < aTendonLineK.GetSize(); i++)
    {
        if (pServiceHeadOffice->ReqService(_ULS(gr), enTendonType, (void*)(&aTendonLineK[i])))
        {
            aKey.Add(aTendonLineK[i]);
        }
    }
}

void CPostTensionTendonElemDeleteDlg::Execute()
{
    UpdateData(TRUE);

    switch (m_nElemType)
    {
    case EN_ELEM_BEAM:
        Execute4Beam();
        break;
    case EN_ELEM_SLAB:
        Execute4Slab();
        break;
    case EN_ELEM_WALL:
        Execute4Wall();
        break;
    default:
        {
            ASSERT(0); return;
        }
    }
}

void CPostTensionTendonElemDeleteDlg::InitSelectionFilter()
{
    I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
    pIGM->ChangeSelectionFilterCombo(EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_TDBM);
}

void CPostTensionTendonElemDeleteDlg::ChangeSelectionFilter(EN_SELECT_ELEM_FILTER enFilter)
{
    I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
    pIGM->ChangeSelectionFilterCombo(enFilter);
}