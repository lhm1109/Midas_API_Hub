// DgnStlRatingEffectLengUframeDlg.cpp: implementation of the CDgnStlRatingEffectLengUframeDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlRatingEffectLengUframeDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


CDgnStlRatingEffectLengUframeDlg::CDgnStlRatingEffectLengUframeDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnStlRatingEffectLengUframeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCPGTranStifDlg)
	m_nOption = 0;
	m_nConnectType = 0;
	m_nBeam = 0;
	m_nJoint = 0;
	//}}AFX_DATA_INITz

	m_pDoc = CDBDoc::GetDocPoint();

	m_Data.Initialize();
}

void CDgnStlRatingEffectLengUframeDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CDgnStlRatingEffectLengUframeDlg)
	DDX_Radio  (pDX, IDC_DGN_OPTION_AR_RDO, m_nOption);
	DDX_Radio  (pDX, IDC_DGN_BOTTOM_RDO,    m_nConnectType);
	DDX_Control(pDX, IDC_DGN_D1_EDT, m_edtd1);
	DDX_Control(pDX, IDC_DGN_D1_UNT, m_untd1);
	DDX_Control(pDX, IDC_DGN_D2_EDT, m_edtd2);
	DDX_Control(pDX, IDC_DGN_D2_UNT, m_untd2);
	DDX_Radio  (pDX, IDC_DGN_OUTER_RDO, m_nBeam);
	DDX_Control(pDX, IDC_DGN_B_EDT, m_edtB);
	DDX_Control(pDX, IDC_DGN_B_UNT, m_untB);
	DDX_Radio  (pDX, IDC_DGN_JOINT_A_RDO, m_nJoint);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_wndPicture);
	//}}AFX_DATA_MAP
}

void CDgnStlRatingEffectLengUframeDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch (lHint)
	{
	case D_UPDATE_DEFAULT:
	case D_UPDATE_BUFFER_BEFORE:
	case D_UPDATE_UNIT:
	case D_UPDATE_SEL_ADD:
	case D_UPDATE_SEL_DEL:
		// do something...
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CDgnStlRatingEffectLengUframeDlg::UpdateBuffer()
{
}

void CDgnStlRatingEffectLengUframeDlg::InitUnit()
{
	m_edtd1.SetUnitType(CUnitCtrl::m_ELUF_UNIT.dd1);
	m_untd1.SetUnitType(CUnitCtrl::m_ELUF_UNIT.dd1);
	m_edtd2.SetUnitType(CUnitCtrl::m_ELUF_UNIT.dd2);
	m_untd2.SetUnitType(CUnitCtrl::m_ELUF_UNIT.dd2);
	m_edtB .SetUnitType(CUnitCtrl::m_ELUF_UNIT.dB);
	m_untB .SetUnitType(CUnitCtrl::m_ELUF_UNIT.dB);
}

void CDgnStlRatingEffectLengUframeDlg::Initial_SelectItem()
{
	// Unselected All.
	if (m_pDoc == NULL)	return;
	if (m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
}

void CDgnStlRatingEffectLengUframeDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_U_FRAME_GRP)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_D1_D2_GRP)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_BOTTOM_RDO)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_USER_RDO)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_D1_STC)->EnableWindow(m_nOption == 0 && m_nConnectType == 1);
	GetDlgItem(IDC_DGN_D1_EDT)->EnableWindow(m_nOption == 0 && m_nConnectType == 1);
	GetDlgItem(IDC_DGN_D1_UNT)->EnableWindow(m_nOption == 0 && m_nConnectType == 1);
	GetDlgItem(IDC_DGN_D2_STC)->EnableWindow(m_nOption == 0 && m_nConnectType == 1);
	GetDlgItem(IDC_DGN_D2_EDT)->EnableWindow(m_nOption == 0 && m_nConnectType == 1);
	GetDlgItem(IDC_DGN_D2_UNT)->EnableWindow(m_nOption == 0 && m_nConnectType == 1);
	GetDlgItem(IDC_DGN_U_GRP)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_OUTER_RDO)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_INNER_RDO)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_B_GRP)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_B_EDT)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_B_UNT)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_JOINT_GRP)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_JOINT_A_RDO)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_JOINT_B_RDO)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_JOINT_C_RDO)->EnableWindow(m_nOption == 0);
}

BOOL CDgnStlRatingEffectLengUframeDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.Initialize();
	m_Data.nConnectType = m_nConnectType;
	m_Data.dd1 = m_edtd1.GetEditValue();
	m_Data.dd2 = m_edtd2.GetEditValue();
	m_Data.nBeam = m_nBeam;
	m_Data.dB = m_edtB.GetEditValue();
	m_Data.nJoint = m_nJoint;

	return TRUE;
}

BEGIN_MESSAGE_MAP(CDgnStlRatingEffectLengUframeDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnStlRatingEffectLengUframeDlg)
	ON_BN_CLICKED(IDC_DGN_OPTION_AR_RDO,  OnDgnOption)
	ON_BN_CLICKED(IDC_DGN_OPTION_DEL_RDO, OnDgnOption)
	ON_BN_CLICKED(IDC_DGN_BOTTOM_RDO, OnDgnConnectType)
	ON_BN_CLICKED(IDC_DGN_USER_RDO, OnDgnConnectType)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CDgnStlRatingEffectLengUframeDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\Effective_Length_of_U_frame.svg"));
	m_wndPicture.SetIgnoreLargeView();

	EnableDisableControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnStlRatingEffectLengUframeDlg::Execute()
{
	// Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	if (aSelKey.GetSize() > 0)
	{
		if (m_nOption == 0)	// add/replace
		{
			if (!Dlg2Data())
			{
				ASSERT(0);
				return;
			}

			if (m_pDoc->m_pDataCtrl->AddEluf(aSelKey, m_Data))	Initial_SelectItem();
		}
		else	// Delete
		{
			if (m_pDoc->m_pDataCtrl->DelEluf(aSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_STL_RATING_BEAM_NON_MEMBER));	
}

void CDgnStlRatingEffectLengUframeDlg::OnDgnOption()
{
	EnableDisableControls();
}

void CDgnStlRatingEffectLengUframeDlg::OnDgnConnectType()
{
	EnableDisableControls();
}