// StldGtmpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldGtmpDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldGtmpDlg dialog


CStldGtmpDlg::CStldGtmpDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldGtmpDlg::IDD, pParent)
{
	m_pBitmap = 0;
	//{{AFX_DATA_INIT(CStldGtmpDlg)
	m_nOption = 0;
	m_nElemType = 0;
	m_bUseSectionHz = FALSE;
	m_bUseSectionHy = FALSE;
	//}}AFX_DATA_INIT
	m_aDeleteCtrl.Add(IDC_TM_ELTYPE_BEAM);
	m_aDeleteCtrl.Add(IDC_TM_ELTYPE_PLATE);
	m_aDeleteCtrl.Add(IDC_TM_TZ_EDIT);
	m_aDeleteCtrl.Add(IDC_TM_HZ_EDIT);
	m_aDeleteCtrl.Add(IDC_TM_HZ_CHECK);
	m_aDeleteCtrl.Add(IDC_TM_TY_EDIT);
	m_aDeleteCtrl.Add(IDC_TM_HY_EDIT);
	m_aDeleteCtrl.Add(IDC_TM_HY_CHECK);

	m_aUseSecHzCtrl.Add(IDC_TM_HZ_EDIT);
	m_aUseSecHyCtrl.Add(IDC_TM_HY_EDIT);

	m_aBeamCtrl.Add(IDC_TM_TY_TITLE);
	m_aBeamCtrl.Add(IDC_TM_TY_EDIT);
	m_aBeamCtrl.Add(IDC_TM_TY_UNIT);
	m_aBeamCtrl.Add(IDC_TM_HY_CHECK);
	m_aBeamCtrl.Add(IDC_TM_HY_TITLE);
	m_aBeamCtrl.Add(IDC_TM_HY_EDIT);
	m_aBeamCtrl.Add(IDC_TM_HY_UNIT);
}

CStldGtmpDlg::~CStldGtmpDlg()
{
	if (m_pBitmap) delete m_pBitmap;
}

void CStldGtmpDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldGtmpDlg)
	DDX_Control(pDX, IDC_TM_TZ_UNIT, m_wndTzUnit);
	DDX_Control(pDX, IDC_TM_TY_UNIT, m_wndTyUnit);
	DDX_Control(pDX, IDC_TM_HZ_UNIT, m_wndHzUnit);
	DDX_Control(pDX, IDC_TM_HY_UNIT, m_wndHyUnit);
	DDX_Control(pDX, IDC_TM_TZ_EDIT, m_wndTzEdit);
	DDX_Control(pDX, IDC_TM_TY_EDIT, m_wndTyEdit);
	DDX_Control(pDX, IDC_TM_HZ_EDIT, m_wndHzEdit);
	DDX_Control(pDX, IDC_TM_HY_EDIT, m_wndHyEdit);
	DDX_Control(pDX, IDC_TM_CBO_LC, m_LoadCase);
	DDX_Radio(pDX, IDC_TM_OPT_ADD, m_nOption);
	DDX_Radio(pDX, IDC_TM_ELTYPE_BEAM, m_nElemType);
	DDX_Check(pDX, IDC_TM_HZ_CHECK, m_bUseSectionHz);
	DDX_Check(pDX, IDC_TM_HY_CHECK, m_bUseSectionHy);
	DDX_Control(pDX, IDC_TM_LDGR_CBO_LC, m_wndLdgrCobx);
	DDX_Control(pDX, IDC_TM_PICTURE, m_wndTypesPicture);
	//}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// CStldGtmpDlg implementations

void CStldGtmpDlg::ChangeBitmap(int nBitmap)
{
#if 0
	// BitmapÀ» Oneway, Twoway·Î ¹Ù²Û´Ù.
	UINT aBitmapID[] = { 
		IDB_TM_GTMP0, IDB_TM_GTMP1,
	};

	if (m_pBitmap != 0) delete m_pBitmap;
	CBCGPStatic* pImage = (CBCGPStatic*)GetDlgItem(IDC_TM_PICTURE);
	m_pBitmap = new CBitmap;
	m_pBitmap->LoadBitmap(aBitmapID[nBitmap]);
	pImage->SetBitmap(HBITMAP(*m_pBitmap));
#else
	CString aTypePictures[] = {
		_T("SVG\\illustration\\Dialog\\tm_gtmp0.svg"),
		_T("SVG\\illustration\\Dialog\\tm_gtmp1.svg"),
	};

	m_wndTypesPicture.SetImage(aTypePictures[nBitmap]);

#endif
}


BEGIN_MESSAGE_MAP(CStldGtmpDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CStldGtmpDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_BTN_LC, OnClickLoadCaseBtn)
	ON_BN_CLICKED(IDC_TM_OPT_ADD, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_ELTYPE_BEAM, OnChangeElemType)
	ON_BN_CLICKED(IDC_TM_OPT_DELETE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_OPT_REPLACE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_ELTYPE_PLATE, OnChangeElemType)
	ON_BN_CLICKED(IDC_TM_HZ_CHECK, OnToggleUseSection)
	ON_BN_CLICKED(IDC_TM_HY_CHECK, OnToggleUseSection)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldGtmpDlg message handlers

BOOL CStldGtmpDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_LoadCase.SetLoadType(D_LOADCASE_STATIC, TRUE);
	m_wndTzEdit.SetUnitType(CUnitCtrl::m_GTMP_UNIT.dTz);
	m_wndTzUnit.SetUnitType(CUnitCtrl::m_GTMP_UNIT.dTz);
	m_wndTyEdit.SetUnitType(CUnitCtrl::m_GTMP_UNIT.dTy);
	m_wndTyUnit.SetUnitType(CUnitCtrl::m_GTMP_UNIT.dTy);
	m_wndHzEdit.SetUnitType(CUnitCtrl::m_GTMP_UNIT.dHz);
	m_wndHzUnit.SetUnitType(CUnitCtrl::m_GTMP_UNIT.dHz);
	m_wndHyEdit.SetUnitType(CUnitCtrl::m_GTMP_UNIT.dHy);
	m_wndHyUnit.SetUnitType(CUnitCtrl::m_GTMP_UNIT.dHy);
	
	// Set Default Values
	m_nOption = 0;  // Default : Add
	m_nElemType = 0;  // Default : Beam
	m_bUseSectionHz = TRUE; // Default : FALSE
	m_bUseSectionHy = TRUE;

	ChangeBitmap(m_nElemType);

	// Enable/Disable/Show/Hide/Align controls
	CtrlEnableDisable(m_aUseSecHzCtrl, !m_bUseSectionHz);
	CtrlEnableDisable(m_aUseSecHyCtrl, !m_bUseSectionHy);

	if (GetDlgItem(IDC_TM_EXECUTE))
		GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);

	if (GetDlgItem(IDC_TM_CLOSE))
		GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldGtmpDlg::OnTmExecute() 
{
	// TODO: Add your control notification handler code here
	UINT nLoadCaseType, nLoadCaseKey;
	if (!m_LoadCase.GetSelectedLoad(nLoadCaseType, nLoadCaseKey))
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___Loadcase_is_not_selected_));
		return;
	}

	CArray<UINT, UINT> aSelKey;
	T_ELEM_D ElemData;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyList(aSelKey);
	for (int i = aSelKey.GetSize()-1; i >= 0; i--)
	{
		m_pDoc->m_pAttrCtrl->GetElem(aSelKey[i], ElemData);
		if (m_nElemType == 0) // beam 
		{
			if (!m_pDoc->m_pAttrCtrl->IsBeam(ElemData.eltyp))
				aSelKey.RemoveAt(i);
		}
		else  // plate
		{
			if (!m_pDoc->m_pAttrCtrl->IsPlate(ElemData.eltyp))
				aSelKey.RemoveAt(i);
		}
	}
	if (aSelKey.GetSize() == 0)
	{
		if (m_nElemType == 0)
			AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___No_selected_beam_element_));
		else AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___No_selected_plate_element_));
		return;
	}

	T_LDGR_K LdgrK;
	m_wndLdgrCobx.GetSelectedLdgr(LdgrK);

	T_GTMP_D data;
	if (m_nOption == 0 || m_nOption == 1)
	{
		data.Initialize();
		
		
		data.GroupKey = LdgrK;

		data.LoadCaseKey = nLoadCaseKey;
		data.nType = m_nElemType+1;
		data.dTz = m_wndTzEdit.GetEditValue();
		data.bUsePropHz = m_bUseSectionHz;
		if (!m_bUseSectionHz) data.dHz = m_wndHzEdit.GetEditValue();
		if (m_nElemType == 0) // Beam Type
		{
			data.dTy = m_wndTyEdit.GetEditValue();
			data.bUsePropHy = m_bUseSectionHy;
			if (!m_bUseSectionHy) data.dHy = m_wndHyEdit.GetEditValue();
		}
	}
	
	BOOL bSuccess;
	if (m_nOption == 0 || m_nOption == 1) // add or replace
		bSuccess = m_pDoc->m_pDataCtrl->AddGtmp(aSelKey, data, m_nOption == 1);
	else  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelGtmp(aSelKey, nLoadCaseKey,LdgrK);

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CStldGtmpDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CStldGtmpDlg::OnClickLoadCaseBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));	
}

void CStldGtmpDlg::OnChangeOption() 
{
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	switch(LOWORD(pMsg->wParam))
	{
	case IDC_TM_OPT_ADD: m_nOption = 0; break;
	case IDC_TM_OPT_REPLACE: m_nOption = 1; break;
	case IDC_TM_OPT_DELETE: m_nOption = 2; break;
	}

	if (m_nOption == 2) // Delete
	{
	  CtrlEnableDisable(m_aDeleteCtrl, FALSE);
		this->m_wndLdgrCobx.ShowHideAll(TRUE);
	}
	else 
	{
		CtrlEnableDisable(m_aDeleteCtrl, TRUE);
		CtrlEnableDisable(m_aUseSecHzCtrl, !m_bUseSectionHz);
		if (m_nElemType == 0)
			CtrlEnableDisable(m_aUseSecHyCtrl, !m_bUseSectionHy);
		else
			CtrlEnableDisable(m_aBeamCtrl, FALSE);
		
		m_wndLdgrCobx.ShowHideAll(FALSE);
	}
}

void CStldGtmpDlg::OnChangeElemType() 
{
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	switch(LOWORD(pMsg->wParam))
	{
	case IDC_TM_ELTYPE_BEAM: m_nElemType = 0; break;
	case IDC_TM_ELTYPE_PLATE: m_nElemType = 1; break;
	}
	ChangeBitmap(m_nElemType);
	if (m_nElemType == 0)
	{
		CtrlEnableDisable(m_aBeamCtrl, TRUE);
		CtrlEnableDisable(m_aUseSecHyCtrl, !m_bUseSectionHy);
	}
	else
	{
		CtrlEnableDisable(m_aBeamCtrl, FALSE);
	}
}


void CStldGtmpDlg::OnToggleUseSection() 
{
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	if (LOWORD(pMsg->wParam) == IDC_TM_HZ_CHECK)
	{
		m_bUseSectionHz = !m_bUseSectionHz;
		CtrlEnableDisable(m_aUseSecHzCtrl, !m_bUseSectionHz);
	}
	else
	{
		m_bUseSectionHy = !m_bUseSectionHy;
		CtrlEnableDisable(m_aUseSecHyCtrl, !m_bUseSectionHy);
	}
}

void CStldGtmpDlg::OnTmDefineLdgrBtn()
{
	CreateOrActivateDlg(m_pDoc,CCMLoadGroupDlg::IDD);		
}
