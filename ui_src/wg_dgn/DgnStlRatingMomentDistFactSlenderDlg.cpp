// DgnStlRatingMomentDistFactSlenderDlg.cpp: implementation of the CDgnStlRatingMomentDistFactSlenderDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlRatingMomentDistFactSlenderDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


CDgnStlRatingMomentDistFactSlenderDlg::CDgnStlRatingMomentDistFactSlenderDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnStlRatingMomentDistFactSlenderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCPGTranStifDlg)
	m_nOption = 0;
	//}}AFX_DATA_INITz

	m_pDoc = CDBDoc::GetDocPoint();

	m_Data.Initialize();
}

void CDgnStlRatingMomentDistFactSlenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CDgnStlRatingMomentDistFactSlenderDlg)
	DDX_Radio  (pDX, IDC_DGN_OPTION_AR_RDO,  m_nOption);
	DDX_Control(pDX, IDC_DGN_SLENDER_LT_EDT, m_edtLambdaLT);
	//}}AFX_DATA_MAP
}

void CDgnStlRatingMomentDistFactSlenderDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDgnStlRatingMomentDistFactSlenderDlg::UpdateBuffer()
{
}

void CDgnStlRatingMomentDistFactSlenderDlg::InitUnit()
{
	m_edtLambdaLT.SetUnitType(D_UNITSYS_NONE);
}

void CDgnStlRatingMomentDistFactSlenderDlg::Initial_SelectItem()
{
	// Unselected All.
	if (m_pDoc == NULL)	return;
	if (m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
}

void CDgnStlRatingMomentDistFactSlenderDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_SLENDER_GRP)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_SLENDER_LT_STC)->EnableWindow(m_nOption == 0);
	GetDlgItem(IDC_DGN_SLENDER_LT_EDT)->EnableWindow(m_nOption == 0);
}

BOOL CDgnStlRatingMomentDistFactSlenderDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.Initialize();

	m_Data.dLambdaLT = m_edtLambdaLT.GetEditValue();

	return TRUE;
}

BEGIN_MESSAGE_MAP(CDgnStlRatingMomentDistFactSlenderDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnStlRatingMomentDistFactSlenderDlg)
	ON_BN_CLICKED(IDC_DGN_OPTION_AR_RDO,  OnDgnOption)
	ON_BN_CLICKED(IDC_DGN_OPTION_DEL_RDO, OnDgnOption)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CDgnStlRatingMomentDistFactSlenderDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	InitUnit();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnStlRatingMomentDistFactSlenderDlg::Execute()
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

			if (m_pDoc->m_pDataCtrl->AddMdfs(aSelKey, m_Data))	Initial_SelectItem();
		}
		else	// Delete
		{
			if (m_pDoc->m_pDataCtrl->DelMdfs(aSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_STL_RATING_BEAM_NON_MEMBER));	
}

void CDgnStlRatingMomentDistFactSlenderDlg::OnDgnOption()
{
	EnableDisableControls();
}
