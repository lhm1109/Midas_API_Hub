// BndrOffsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrOffsDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBndrOffsDlg dialog

#define TYPE_GLOBAL		1
#define TYPE_ELEMENT	2

CBndrOffsDlg::CBndrOffsDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CBndrOffsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBndrOffsDlg)
	m_nOption = 0;
	//}}AFX_DATA_INIT
	m_pBitmap = 0;
}

CBndrOffsDlg::~CBndrOffsDlg()
{  
	if (m_pBitmap) delete m_pBitmap;
}

void CBndrOffsDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBndrOffsDlg)
	DDX_Control(pDX, IDC_TM_OFFS_TYPE, m_Type);
	DDX_Radio(pDX, IDC_TM_OFFS_OPT_REPLACE, m_nOption);
	DDX_Control(pDX, IDC_TM_GROUP_COMBO, m_wndGroupCombo);
	DDX_Control(pDX, IDC_WG_TREEMENU_STATIC3, m_wndPicture);

	//}}AFX_DATA_MAP
	UINT nID[6][3] = { 
		{IDC_TM_OFFS_RGDXi_TEXT, IDC_TM_OFFS_RGDXi, IDC_TM_OFFS_RGDXi_UNIT},
		{IDC_TM_OFFS_RGDYi_TEXT, IDC_TM_OFFS_RGDYi, IDC_TM_OFFS_RGDYi_UNIT},
		{IDC_TM_OFFS_RGDZi_TEXT, IDC_TM_OFFS_RGDZi, IDC_TM_OFFS_RGDZi_UNIT},
		{IDC_TM_OFFS_RGDXj_TEXT, IDC_TM_OFFS_RGDXj, IDC_TM_OFFS_RGDXj_UNIT},
		{IDC_TM_OFFS_RGDYj_TEXT, IDC_TM_OFFS_RGDYj, IDC_TM_OFFS_RGDYj_UNIT},
		{IDC_TM_OFFS_RGDZj_TEXT, IDC_TM_OFFS_RGDZj, IDC_TM_OFFS_RGDZj_UNIT},
	};
	for (int i = 0; i < 6; i++)
	{
	  DDX_Control(pDX, nID[i][0], m_Text[i]);
	  DDX_Control(pDX, nID[i][1], m_Value[i]);
	  DDX_Control(pDX, nID[i][2], m_Unit[i]);
	}
}

void CBndrOffsDlg::Execute()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	T_ELEM_D ElemData;

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedElemKeyList(aSelKey);
	for (int i = aSelKey.GetSize() - 1; i >= 0; i--)
	{
		m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
		if (!m_pDoc->m_pAttrCtrl->IsBeam(ElemData.eltyp)) aSelKey.RemoveAt(i);
	}

	if (aSelKey.GetSize() == 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_beam_element_));
		return;
	}
	T_OFFS_D data;
	data.Initialize();

	int nUnmodifiedType = m_Type.GetCurSel() + 1;

	if (nUnmodifiedType == 1)
	{
		data.nOffsetType = TYPE_GLOBAL;
	}
	else
	{
		data.nOffsetType = TYPE_ELEMENT;
	}

	if (data.nOffsetType == TYPE_GLOBAL) //Global
	{
		for (int i = 0; i < 6; i++)
		{
			data.Offset[i] = m_Value[i].GetEditValue();
		}
	}
	else //Element
	{
		if (nUnmodifiedType == 2) //SYMMETRY
		{
			data.Offset[1] = data.Offset[2] = m_Value[0].GetEditValue();
			data.Offset[4] = data.Offset[5] = m_Value[1].GetEditValue();
		}
		else if (nUnmodifiedType == 3) //ASYMMETRY
		{
			data.Offset[1] = m_Value[0].GetEditValue();
			data.Offset[2] = m_Value[1].GetEditValue();
			data.Offset[4] = m_Value[2].GetEditValue();
			data.Offset[5] = m_Value[3].GetEditValue();
		}
	}

	T_BNGR_K BngrK;
	m_wndGroupCombo.GetSelectedBngr(BngrK);
	data.GroupKey = BngrK;

	BOOL bSuccess = FALSE;
	if (m_nOption == 0) // Replace
		bSuccess = m_pDoc->m_pDataCtrl->AddOffs(aSelKey, data);
	else if (m_nOption == 1)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelOffs(aSelKey, BngrK);

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

BEGIN_MESSAGE_MAP(CBndrOffsDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CBndrOffsDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_OFFS_OPT_REPLACE, OnChangeOption)
	ON_CBN_SELCHANGE(IDC_TM_OFFS_TYPE, OnChangeType)
	ON_BN_CLICKED(IDC_TM_OFFS_OPT_DELETE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_DEFINE_GROUP_BUTTON, OnTmDefineGroupButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBndrOffsDlg message handlers

BOOL CBndrOffsDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	m_nOption = 0;
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	
	for (int i = 0; i < 6; i++)
	{
		m_Unit[i].SetUnitType(CUnitCtrl::m_OFFS_UNIT.Offset);
		m_Value[i].SetUnitType(CUnitCtrl::m_OFFS_UNIT.Offset);
	}
	m_Type.AddString(_LS(IDS_WG_TREEMENU_Global));
	m_Type.AddString(_LS(IDS_TM_OFFS_Element_SYM));
	m_Type.AddString(_LS(IDS_TM_OFFS_Element_ASYM));
	m_Type.SelectString(-1, _LS(IDS_WG_TREEMENU_Global));
	
	UpdateData(FALSE);	
	
	ChangeBitMap();

	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBndrOffsDlg::OnTmExecute() 
{
	Execute();
}

void CBndrOffsDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}


void CBndrOffsDlg::OnChangeOption() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_nOption == 1) // delete
	{
		EnableValueEdit(FALSE); // disable all value edit
		this->m_wndGroupCombo.ShowHideAll(TRUE);
	}
	else
	{
		EnableValueEdit(TRUE);	
		this->m_wndGroupCombo.ShowHideAll(FALSE);
	}
}

void CBndrOffsDlg::EnableValueEdit(BOOL bEnable)
{
	for (int i = 0; i < 6; i++)
	{
		m_Value[i].EnableWindow(bEnable);
	}
	m_Type.EnableWindow(bEnable);
}

void CBndrOffsDlg::OnChangeType() 
{
	// TODO: Add your control notification handler code here
	ChangeType(m_Type.GetCurSel());
}

void CBndrOffsDlg::ChangeType(int nType)
{
	BOOL bEnalbe[6];
	for (int i = 0; i < 6; i++)
	{
		bEnalbe[i] = TRUE;
	}

	if (nType != 0)
	{
		int nStart;
		if (nType == 1) nStart = 2;
		else nStart = 4;

		for (int i = nStart; i < 6; i++)
		{
			bEnalbe[i] = FALSE;
		}
	}

	for (int i = 0; i < 6; i++)
	{
		m_Text[i].ShowWindow(bEnalbe[i]);
		m_Value[i].ShowWindow(bEnalbe[i]);
		m_Unit[i].ShowWindow(bEnalbe[i]);
	}

	if (nType == 0) //Global
	{
		m_Text[0].SetWindowText(_LS(IDS_WG_TREEMENU_RGDXi));
		m_Text[1].SetWindowText(_LS(IDS_WG_TREEMENU_RGDYi));
		m_Text[2].SetWindowText(_LS(IDS_WG_TREEMENU_RGDZi));
		m_Text[3].SetWindowText(_LS(IDS_WG_TREEMENU_RGDXj));
	}
	else if (nType == 1) //Element(SYMMETRY)
	{
		m_Text[0].SetWindowText(_LS(IDS_WG_TREEMENU_RGDi));
		m_Text[1].SetWindowText(_LS(IDS_WG_TREEMENU_RGDj));
	}
	else //Element(ASYMMETRY)
	{
		m_Text[0].SetWindowText(_LS(IDS_WG_TREEMENU_RGDyi));
		m_Text[1].SetWindowText(_LS(IDS_WG_TREEMENU_RGDzi));
		m_Text[2].SetWindowText(_LS(IDS_WG_TREEMENU_RGDyj));
		m_Text[3].SetWindowText(_LS(IDS_WG_TREEMENU_RGDzj));
	}

	ChangeBitMap();
}

BOOL CBndrOffsDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	T_OFFS_K Key;
	T_OFFS_D data;
	Key.keymap = key;
	m_pDoc->m_pAttrCtrl->GetOffs(Key, data);

#ifdef _MGEN_CH
	if(data.nOffsetType == 1)
		m_Type.SetCurSel(0);
	else
	{
		if(fabs(data.Offset[1] - data.Offset[2]) < 1.0e-9 && fabs(data.Offset[4] - data.Offset[5]) < 1.0e-9)
			m_Type.SetCurSel(1);
		else
			m_Type.SetCurSel(2);
	}
	OnChangeType();
	if(data.nOffsetType == 1) //Global
	{
		for(int i = 0; i < 6; i++)
			m_Value[i].SetEditUnit(data.Offset[i]);
	}
	else if(m_Type.GetCurSel() == 1)
	{
		m_Value[0].SetEditUnit(data.Offset[1]);
		m_Value[1].SetEditUnit(data.Offset[4]);
	}
	else
	{
		m_Value[0].SetEditUnit(data.Offset[1]);
		m_Value[1].SetEditUnit(data.Offset[2]);
		m_Value[2].SetEditUnit(data.Offset[4]);
		m_Value[3].SetEditUnit(data.Offset[5]);
	}
#else
	m_Type.SetCurSel(data.nOffsetType-1);
	ChangeType(data.nOffsetType == 1);

	if (data.nOffsetType == 1) //Global
	{
		for (int i = 0; i < 6; i++)
			m_Value[i].SetEditUnit(data.Offset[i]);
	}
	else if (data.nOffsetType == 2) //Element(SYMMETRY)
	{
		m_Value[0].SetEditUnit(data.Offset[1]);
		m_Value[1].SetEditUnit(data.Offset[4]);
	}
	else //Element(ASYMMETRY)
	{
		m_Value[0].SetEditUnit(data.Offset[1]);
		m_Value[1].SetEditUnit(data.Offset[2]);
		m_Value[2].SetEditUnit(data.Offset[4]);
		m_Value[3].SetEditUnit(data.Offset[5]);
	}
#endif

	// Groupµµ Setting
	T_BNGR_D  bngrD;
	m_pDoc->m_pAttrCtrl->GetBngr(data.GroupKey, bngrD);
	for (int i=0; i<m_wndGroupCombo.GetCount(); i++)
	{
		CString str;
		m_wndGroupCombo.GetLBText(i, str);
		if(str.CompareNoCase(bngrD.GroupName)==0) { m_wndGroupCombo.SetCurSel(i); break; }
	}

	m_nOption = 0;
	UpdateData(FALSE);
	
	return TRUE;
}

void CBndrOffsDlg::OnTmDefineGroupButton() 
{
	CreateOrActivateDlg(m_pDoc,CCMBndrGrupDefDlg::IDD);
}

void CBndrOffsDlg::ChangeBitMap()
{
	CString aPicture[] = {
		_T("SVG\\illustration\\Dialog\\tm_offs.svg"),
		_T("SVG\\illustration\\Dialog\\tm_offs_symm.svg"),
		_T("SVG\\illustration\\Dialog\\tm_offs_asym.svg"),
	};

	int nTypeIndex = m_Type.GetCurSel();
	if(nTypeIndex > 2) { ASSERT(0); return; }

	m_wndPicture.SetImage(aPicture[nTypeIndex]);
}