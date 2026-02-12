#include "stdafx.h"
#include "wg_treemenu.h"
#include "PostTensionDrawTendonLineBeamDlg.h"

#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\I_GENModelBase.h"
//#include "..\wg_base\ViewBase.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DispOptMgr.h"

#include "TreeWorkCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPostTensionDrawTendonLineBeamDlg::CPostTensionDrawTendonLineBeamDlg(CWnd* pParent)
	: CMenuBarChildDlg(CPostTensionDrawTendonLineBeamDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();

	m_nEndType = 0;

	m_Data.Initialize();
}

void CPostTensionDrawTendonLineBeamDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TENDON_LINE_BEAM_NO_EDT, m_edtTendonNum);
	// Tendon Property
	DDX_Control(pDX, IDC_TENDON_LINE_BEAM_TENDON_PROP_STC, m_stcTendonProp);
	DDX_Control(pDX, IDC_TENDON_LINE_BEAM_TENDON_PROP_CMB, m_cmbTendonProp);
	DDX_Control(pDX, IDC_TENDON_LINE_BEAM_TENDON_PROP_BTN, m_btnTendonProp);
	// Tendon Profile
	DDX_Control(pDX, IDC_TENDON_LINE_BEAM_TENDON_PROF_STC, m_stcTendonProf);
	DDX_Control(pDX, IDC_TENDON_LINE_BEAM_TENDON_PROF_CMB, m_cmbTendonProf);
	DDX_Control(pDX, IDC_TENDON_LINE_BEAM_TENDON_PROF_BTN, m_btnTendonProf);
	// Tendon Property
	DDX_Control(pDX, IDC_TENDON_LINE_BEAM_TENDON_STRS_STC, m_stcTendonStrs);
	DDX_Control(pDX, IDC_TENDON_LINE_BEAM_TENDON_STRS_CMB, m_cmbTendonStrs);
	DDX_Control(pDX, IDC_TENDON_LINE_BEAM_TENDON_STRS_BTN, m_btnTendonStrs);
	DDX_Radio(pDX, IDC_TENDON_LINE_BEAM_END_TYPE_LF_RDO, m_nEndType);
	DDX_Control(pDX, IDC_TENDON_LINE_BEAM_ELEM_EDT, m_edtElem);
}

void CPostTensionDrawTendonLineBeamDlg::Dlg2Data()
{
	UpdateData(TRUE);

	CString strTendonNum;
	m_edtTendonNum.GetWindowText(strTendonNum);
	if (!strTendonNum.IsEmpty())
	{
		BOOL bValidNumber = TRUE;
		for (int i = 0; i < strTendonNum.GetLength(); i++)
		{
			if (!_istdigit(strTendonNum[i]))
			{
				bValidNumber = FALSE;
				break;
			}
		}

		if (bValidNumber)
		{
			m_Data.nTendonNo = _ttoi(strTendonNum);
		}
		else
		{
			AfxMessageBox(_T("텐던 번호는 숫자만 입력해야 합니다."));
			m_Data.nTendonNo = 0;
		}
	}

	// Tendon Property - 콤보박스 인덱스로부터 TdntK 가져오기
	int nPropSel = m_cmbTendonProp.GetCurSel();
	if (nPropSel >= 0)
	{
		CArray<T_TDNT_K, T_TDNT_K> aTdntK;
		m_pDoc->m_pAttrCtrl->GetTdntKeyList(aTdntK);
		if (nPropSel < aTdntK.GetSize())
		{
			m_Data.TdntK = aTdntK[nPropSel];
		}
	}
	// Tendon Profile - 콤보박스 인덱스로부터 TdpfK 가져오기
	int nProfSel = m_cmbTendonProf.GetCurSel();
	if (nProfSel >= 0)
	{
		CArray<T_TDPF_K, T_TDPF_K> aTdpfK;
		m_pDoc->m_pAttrCtrl2->GetTdpfKeyList(aTdpfK);
		if (nProfSel < aTdpfK.GetSize())
		{
			m_Data.TdpfK = aTdpfK[nProfSel];
		}
	}
	// Tendon Stressing - 콤보박스 인덱스로부터 TdstrK 가져오기
	int nStrsSel = m_cmbTendonStrs.GetCurSel();
	if (nStrsSel >= 0)
	{
		CArray<T_TDST_K, T_TDST_K> aTdstrK;
		m_pDoc->m_pAttrCtrl2->GetTdstKeyList(aTdstrK);
		if (nStrsSel < aTdstrK.GetSize())
		{
			m_Data.TdstK = aTdstrK[nStrsSel];
		}
	}

	m_Data.nEndType = m_nEndType;

	CArray<int, int> aElemK;
	aElemK.RemoveAll();

	CString strText;
	m_edtElem.GetWindowText(strText);
	if (!CStrParser::ParsingListByTo(strText, aElemK)) return;

	for ( auto i = 0; i < aElemK.GetSize(); i++ )
	{
		if (!m_pDoc->m_pAttrCtrl->IsBeamTypeElem(aElemK[i])) continue;

		m_Data.aElemK.Add(aElemK[i]);
	}
}

void CPostTensionDrawTendonLineBeamDlg::Execute()
{
	UpdateData(TRUE);

	m_Data.Initialize();

	Dlg2Data();

	T_TDBM_K TdbmK = m_pDoc->m_pDataCtrl->GetTdbmKey();

	// Todo HJM : aElemK 의 Story 들을 비교해서 하나라도 다르면 Add 되지 않도록 처리 필요
	if (!m_pDoc->m_pDataCtrl->AddTdbm(TdbmK, m_Data))
	{
		ASSERT(0); return;
	}
}

void CPostTensionDrawTendonLineBeamDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch (lHint)
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
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD:
	case D_UPDATE_SEL_DEL:
		break;
	default: ASSERT(TRUE);
	}
}

void CPostTensionDrawTendonLineBeamDlg::UpdateBuffer()
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

		switch (nCmd)
		{
		case(UR_TDNT_ADD):
		case(UR_TDNT_DEL):
		case(UR_TDNT_MFD):
		case(UR_TDNT_MFS):
			{
				// Tendon Property
				UpdateTendonPropertyList();
			}
			break;
		case(UR_TDPF_ADD):
		case(UR_TDPF_DEL):
		case(UR_TDPF_MFD):
		case(UR_TDPF_MFS):
			{
				// Tendon Profile
				UpdateTendonProfileList();
			}
			break;
		case(UR_TDST_ADD):
		case(UR_TDST_DEL):
		case(UR_TDST_MFD):
		case(UR_TDST_MFS):
			{
				// Tendon Stressing
				UpdateTendonStressingList();
			}
			break;
		default:
			break;
		}
	}
}

BEGIN_MESSAGE_MAP(CPostTensionDrawTendonLineBeamDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CPostTensionDrawTendonLineBeamDlg)
	// Tendon Property
	ON_BN_CLICKED(IDC_TENDON_LINE_BEAM_TENDON_PROP_BTN, OnClickTendonPropBtn)
	// Tendon Profile
	ON_BN_CLICKED(IDC_TENDON_LINE_BEAM_TENDON_PROF_BTN, OnClickTendonProfBtn)
	// Tendon Stressing
	ON_BN_CLICKED(IDC_TENDON_LINE_BEAM_TENDON_STRS_BTN, OnClickTendonStrsBtn)
	ON_BN_CLICKED(IDC_TENDON_LINE_BEAM_END_TYPE_LF_RDO, OnEndTypeRadioClicked)
	ON_BN_CLICKED(IDC_TENDON_LINE_BEAM_END_TYPE_LL_RDO, OnEndTypeRadioClicked)
	ON_BN_CLICKED(IDC_TENDON_LINE_BEAM_END_TYPE_FL_RDO, OnEndTypeRadioClicked)
	////}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CPostTensionDrawTendonLineBeamDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	m_edtElem.Disconnect();

	// Text Preserve
	CString strPreviousText = _T("");
	m_edtElem.GetWindowText(strPreviousText);
	m_edtElem.Connect(SC_ID_ELEM, &m_edtElem);
	m_edtElem.SetWindowTextW(strPreviousText);

	// Model Select
	CString strBlank = _T("");
	m_edtElem.SelectByStr(strBlank);
	m_edtElem.SelectByStr(strPreviousText);

	// Tendon Property
	UpdateTendonPropertyList();

	// Tendon Profile
	UpdateTendonProfileList();

	// Tendon Stressing
	UpdateTendonStressingList();

	return TRUE;
}

void CPostTensionDrawTendonLineBeamDlg::OnClickTendonPropBtn()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_TENDON_PROP, 0));
}

void CPostTensionDrawTendonLineBeamDlg::OnClickTendonProfBtn()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_PTDN_TENDON_PROFILE_SET, 0));
}

void CPostTensionDrawTendonLineBeamDlg::OnClickTendonStrsBtn()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_PTDN_TENDON_STRESSING, 0));
}

void CPostTensionDrawTendonLineBeamDlg::OnEndTypeRadioClicked()
{
	UpdateData(TRUE);

	switch (m_nEndType)
	{
	case 0:
		{

		}
		break;
	case 1:
		{

		}
		break;
	case 2:
		{

		}
		break;
	default:
		{
			ASSERT(0); return;
		}
	}
}

void CPostTensionDrawTendonLineBeamDlg::UpdateTendonPropertyList()
{
	m_cmbTendonProp.ResetContent();

	CArray<T_TDNT_K, T_TDNT_K> aTdntK;
	m_pDoc->m_pAttrCtrl->GetTdntKeyList(aTdntK);
	T_TDNT_D TdntD;
	for (auto i = 0; i < aTdntK.GetSize(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetTdnt(aTdntK[i], TdntD))
		{
			ASSERT(0); return;
		}
		m_cmbTendonProp.AddString(TdntD.TendonTypeName);
	}

	m_cmbTendonProp.SetCurSel(0);
}

void CPostTensionDrawTendonLineBeamDlg::UpdateTendonProfileList()
{
	m_cmbTendonProf.ResetContent();

	CArray<T_TDPF_K, T_TDPF_K> aTdpfK;
	m_pDoc->m_pAttrCtrl2->GetTdpfKeyList(aTdpfK);
	T_TDPF_D TdpfD;
	for (auto i = 0; i < aTdpfK.GetSize(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl2->GetTdpf(aTdpfK[i], TdpfD))
		{
			ASSERT(0); return;
		}
		m_cmbTendonProf.AddString(TdpfD.strName);
	}

	m_cmbTendonProf.SetCurSel(0);
}

void CPostTensionDrawTendonLineBeamDlg::UpdateTendonStressingList()
{
	m_cmbTendonStrs.ResetContent();

	CArray<T_TDST_K, T_TDST_K> aTdstK;
	m_pDoc->m_pAttrCtrl2->GetTdstKeyList(aTdstK);
	T_TDST_D TdstD;
	for (auto i = 0; i < aTdstK.GetSize(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl2->GetTdst(aTdstK[i], TdstD))
		{
			ASSERT(0); return;
		}
		m_cmbTendonStrs.AddString(TdstD.strName);
	}

	m_cmbTendonStrs.SetCurSel(0);
}