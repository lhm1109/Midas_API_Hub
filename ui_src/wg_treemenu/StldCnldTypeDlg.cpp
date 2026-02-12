#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldCnldTypeDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"
#include "..\wg_db\MITC_CommonTool.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldCnldTypeDlg dialog


CStldCnldTypeDlg::CStldCnldTypeDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldCnldTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldCnldTypeDlg)
	m_nOption = -1;
	//}}AFX_DATA_INIT
}

CStldCnldTypeDlg::~CStldCnldTypeDlg()
{
	MITC_CommonPara::instance()->m_Cnld.Initialize();
	MITC_CommonPara::instance()->m_Cnld.strType = _T("");
}

void CStldCnldTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldCnldTypeDlg)
	DDX_Text(pDX, IDC_TM_NL_NAME, m_strTypeName);
	DDX_Control(pDX, IDC_TM_NL_CBO_LC, m_LoadCase);
	DDX_Radio(pDX, IDC_TM_NL_OPT_ADD, m_nOption);
	DDX_Control(pDX, IDC_TM_LDGR_CBO_LC, m_wndLdgrCobx);
	DDX_Control(pDX, IDC_WG_TREEMENU_STATIC9, m_wndPicture);
	//}}AFX_DATA_MAP
	UINT nID[6][2] = { 
		{IDC_TM_NL_FX, IDC_TM_NL_FX_UNIT},
		{IDC_TM_NL_FY, IDC_TM_NL_FY_UNIT},
		{IDC_TM_NL_FZ, IDC_TM_NL_FZ_UNIT},
		{IDC_TM_NL_MX, IDC_TM_NL_MX_UNIT},
		{IDC_TM_NL_MY, IDC_TM_NL_MY_UNIT},
		{IDC_TM_NL_MZ, IDC_TM_NL_MZ_UNIT},
	};
	for (int i = 0; i < 6; i++)
	{
	  DDX_Control(pDX, nID[i][0], m_Value[i]);
	  DDX_Control(pDX, nID[i][1], m_Unit[i]);
	}
}

void CStldCnldTypeDlg::Execute()
{
	OnTmExecute();
}

BEGIN_MESSAGE_MAP(CStldCnldTypeDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CStldCnldTypeDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_NL_BTN_LC, OnClickLoadCaseBtn)
	ON_BN_CLICKED(IDC_TM_NL_OPT_ADD, OnChangeOption)
	//ON_BN_CLICKED(IDC_TM_NL_OPT_REPLACE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_NL_OPT_DELETE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldCnldTypeDlg message handlers

BOOL CStldCnldTypeDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	
	for (int i = 0; i < 6; i++)
	{
		m_Unit[i].SetUnitType(CUnitCtrl::m_CNLD_UNIT.Force[i]);
		m_Value[i].SetUnitType(CUnitCtrl::m_CNLD_UNIT.Force[i]);
	}
	m_nOption = 0;
	m_wndPicture.SetImage(_T("SVG\\illustration\\Dialog\\tm_cnld 1.svg"));
	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);
	//Dlg2Data
	const T_CNLD_D& cnld = MITC_CommonPara::instance()->m_Cnld;
	m_strTypeName = cnld.strType.IsEmpty() ? GetNewTypeName() : cnld.strType;
	for(int i = 0;i < 6;i++)
	{
		m_Value[i].SetEditUnit(cnld.Force[i]);
	}
	m_LoadCase.ChangeSelect(D_LOADCASE_STATIC,cnld.LoadCaseKey);
	//
	UpdateData(FALSE);
	return TRUE;
}

void CStldCnldTypeDlg::UpdateChildWindow()
{
	const T_CNLD_D& cnld = MITC_CommonPara::instance()->m_Cnld;
	m_strTypeName = cnld.strType.IsEmpty() ? GetNewTypeName() : cnld.strType;
	for(int i = 0;i < 6;i++)
	{
		m_Value[i].SetEditUnit(cnld.Force[i]);
	}
	m_LoadCase.ChangeSelect(D_LOADCASE_STATIC,cnld.LoadCaseKey);
	UpdateData(FALSE);
	__super::UpdateChildWindow();
}

void CStldCnldTypeDlg::End()
{
	MITC_CommonPara::instance()->m_Cnld.Initialize();
	MITC_CommonPara::instance()->m_Cnld.strType = _T("");
	__super::End();
}

void CStldCnldTypeDlg::OnTmExecute()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CArray<T_NODE_K, T_NODE_K> aSelKey;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedNodeKeyList(aSelKey);
	if (aSelKey.GetSize() == 0) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_));
		return;
	}
	T_CNLD_D data;
	unsigned int nLoadCaseType, nLoadCaseKey;
	data.Initialize();

	m_LoadCase.GetSelectedLoad(nLoadCaseType, nLoadCaseKey);
	data.LoadCaseKey = nLoadCaseKey;
	data.strType = m_strTypeName;
	for (int i = 0; i < 6; i++)
		data.Force[i] = m_Value[i].GetEditValue();

	T_LDGR_K LdgrK;
	m_wndLdgrCobx.GetSelectedLdgr(LdgrK);

	BOOL bSuccess=FALSE;
	if (m_nOption == 0)
	{
		//CNLDTypeKey PreKey;
		T_CNLD_K CnldK;
		T_CNLD_D OriData;
		CArray<T_CNLD_KEY, T_CNLD_KEY&> pKeyList;
		if(m_pDoc->m_pAttrCtrl->GetCnldType(data.strType,pKeyList) && !pKeyList.IsEmpty())
		{
			BOOL bSame = TRUE;
			CnldK.keymap = pKeyList[0];
			m_pDoc->m_pAttrCtrl->GetCnld(CnldK,OriData);
			for(int i = 0;i < 6;i++)
			{
				if(fabs(OriData.Force[i] - data.Force[i]) > 1e-9)
				{
					bSame = FALSE;
					break;
				}
			}
			if(bSame)
			{
				data.GroupKey = LdgrK;
				bSuccess = m_pDoc->m_pDataCtrl->AddCnld(aSelKey, data, TRUE);	
			}
			else
			{
				CString str;
				str.Format(_LS(IDS_WG_TREEMENU_Nodal_Loads_Type_Exist_SameName),data.strType);
				if(AfxMessageBox(str,MB_OKCANCEL) == MB_OKCANCEL)
				{
					bSuccess = m_pDoc->m_pDataCtrl->AddCnldType(aSelKey, data);	
				}
			}

		}
		else
		{
			data.GroupKey = LdgrK;
			bSuccess = m_pDoc->m_pDataCtrl->AddCnld(aSelKey, data, TRUE);	
		}
	}
	else if (m_nOption == 1)   // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelCnld(aSelKey, nLoadCaseKey,LdgrK);

	if (bSuccess)
	{
		m_strTypeName = GetNewTypeName();
		UpdateData(FALSE);
		m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	}
}

void CStldCnldTypeDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CStldCnldTypeDlg::OnClickLoadCaseBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CStldCnldTypeDlg::OnChangeOption() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_nOption == 2) // delete 
	{
		EnableValueEdit(FALSE);
		this->m_wndLdgrCobx.ShowHideAll(TRUE);
	}
	else
	{
		EnableValueEdit(TRUE);
		this->m_wndLdgrCobx.ShowHideAll(FALSE);
	}
}

void CStldCnldTypeDlg::EnableValueEdit(BOOL bEnable)
{
	UINT nID[6] = {
		IDC_TM_NL_FX, IDC_TM_NL_FY, IDC_TM_NL_FZ,
		IDC_TM_NL_MX, IDC_TM_NL_MY, IDC_TM_NL_MZ,
	};
	CWnd* pWnd;
	for (int i = 0; i < 6; i++)
	{
		pWnd = GetDlgItem(nID[i]);
		ASSERT(pWnd);
		pWnd->EnableWindow(bEnable);
	}
}

void CStldCnldTypeDlg::OnTmDefineLdgrBtn() 
{
	// TODO: Add your control notification handler code here
	CreateOrActivateDlg(m_pDoc,CCMLoadGroupDlg::IDD);		
}

CString CStldCnldTypeDlg::GetNewTypeName()
{
	CString str;
	int nCnldTypeCount = m_pDoc->m_pAttrCtrl->GetCnldTypeCount();
	int nIndex = 1;
	str.Format(_LS(IDS_WG_TREEMENU_Node__load__d),nIndex);
	CArray<T_CNLD_KEY, T_CNLD_KEY&> pKeyList;
	while(m_pDoc->m_pAttrCtrl->GetCnldType(str,pKeyList))
	{
		str.Format(_LS(IDS_WG_TREEMENU_Node__load__d),nIndex);
		nIndex++;
	}
	return str;
}