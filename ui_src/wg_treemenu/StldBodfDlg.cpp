// StldBodfDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldBodfDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#define BF_DATA_FMT _LS(IDS_WG_TREEMENU__g)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldBodfDlg dialog


CStldBodfDlg::CStldBodfDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldBodfDlg::IDD, pParent)
{
	m_pDoc = 0;
	//{{AFX_DATA_INIT(CStldBodfDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CStldBodfDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldBodfDlg)
	DDX_Control(pDX, IDC_TM_BF_LIST, m_List);
	DDX_Control(pDX, IDC_TM_BF_Z, m_FactorZ);
	DDX_Control(pDX, IDC_TM_BF_Y, m_FactorY);
	DDX_Control(pDX, IDC_TM_BF_X, m_FactorX);
	DDX_Control(pDX, IDC_TM_BF_CBO_LC, m_LoadCase);
	DDX_Control(pDX, IDC_TM_LDGR_CBO_LC, m_wndLdgrCobx);
	DDX_Control(pDX, IDC_WG_TREEMENU_STATIC6, m_wndPicture);
	//}}AFX_DATA_MAP
}

void CStldBodfDlg::Execute()
{
	OnCmdBfAdd();
}

BEGIN_MESSAGE_MAP(CStldBodfDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CStldBodfDlg)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose                            )
	ON_BN_CLICKED(IDC_TM_BF_BTN_LC, OnCmdBfBtnLc                     )
	ON_BN_CLICKED(IDC_TM_BF_ADD, OnCmdBfAdd                          )
	ON_BN_CLICKED(IDC_TM_BF_MODIFY, OnCmdBfModify                    )
	ON_BN_CLICKED(IDC_TM_BF_DELETE, OnCmdBfDelete                    )
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_BF_LIST, OnChangedItemSelection)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldBodfDlg message handlers

BOOL CStldBodfDlg::OnInitDialog() 
{
	MInitCombo initCombo;
	SetRedraw(FALSE);
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();

	m_FactorX.SetWindowText(_T("0"));
	m_FactorY.SetWindowText(_T("0"));
	m_FactorZ.SetWindowText(_T("0"));

	// ListCtrl의 헤더를 설정한다.
	SetListCtrlHeader();

	// DB에 저장된 Body Force 값을 가져와서 List에 보여준다.
	MakeItemEx();
	
	m_wndPicture.SetImage(_T("SVG\\illustration\\Dialog\\tm_bodf 1.svg"));
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldBodfDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CStldBodfDlg::SetListCtrlHeader()
{
	/*
	CString aTitle[] = {_LS(IDS_WG_TREEMENU_Load_Case), _T("X"), _T("Y"), _T("Z")};
	int nColWidth[] = {90, 35, 35, 35};
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < 4; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
	*/
	CStringArray aTitles;
	CArray<int,int> aWidths;
	CArray<float,float> aWRatio;
	aTitles.Add(_LS(IDS_WG_TREEMENU_Load_Case));
	aTitles.Add(_T("X"));aTitles.Add(_T("Y"));
	aTitles.Add(_T("Z"));aTitles.Add(_LS(IDS_WG_TREEMENU_ADD3_Group));
	CSize size;
#pragma warning( disable : 4305)
	aWRatio.Add((float)4.5/11.5);
	aWRatio.Add((float)1.4/11.5);
	aWRatio.Add((float)1.4/11.5);
	aWRatio.Add((float)1.4/11.5);
	aWRatio.Add((float)4.5/11.5);
#pragma warning( default : 4305 )


	/*
	for(int i = 0; i < 5; i++)
	{
		size = m_List.GetHeaderCtrl()->GetDC()->GetTextExtent(aTitles[i]);
		aWidths.Add(size.cx+10);
	}
	*/

	CDlgUtil::_SetListCtrlHeader(&m_List,aTitles,&aWRatio,NULL);
}

void CStldBodfDlg::OnCmdBfBtnLc() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CStldBodfDlg::OnCmdBfAdd() 
{
	// TODO: Add your control notification handler code here
	
	T_BODF_D data;
	double x, y, z;
	unsigned int nLoadCaseKey ;
	unsigned int nLoadCaseType;
	if (!m_LoadCase.GetSelectedLoad(nLoadCaseType, nLoadCaseKey)) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Load_case_was_not_selected_));
		return;
	}
	
	GetFactor(x, y, z);

	data.SX = x;
	data.SY = y;
	data.SZ = z;
	data.LoadCaseKey = nLoadCaseKey;
	
	T_LDGR_K LdgrK;
	m_wndLdgrCobx.GetSelectedLdgr(LdgrK);
	data.GroupKey = LdgrK;
		
	m_pDoc->m_pDataCtrl->AddBodf(data);
}

void CStldBodfDlg::OnCmdBfModify() 
{
	/*
	// TODO: Add your control notification handler code here
	T_BODF_K key, keyOld;
	T_BODF_D data;
	data.Initialize();
	double x, y, z;

	if (!GetSelectedBodyForce(keyOld))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_self_weight_));
		return;
	}

	unsigned int nLoadCaseType;
	if (!m_LoadCase.GetSelectedLoad(nLoadCaseType, key)) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Load_case_was_not_selected_));
		return;
	}
	GetFactor(x, y, z);

	data.SX = x;
	data.SY = y;
	data.SZ = z;
	m_pDoc->m_pDataCtrl->ModifyBodf(keyOld, data);
	*/

	T_BODF_K KeyOld;
	T_BODF_D data;
	double x, y, z;
	unsigned int nLoadCaseKey ;
	unsigned int nLoadCaseType;
	
	if (!GetSelectedBodyForce(KeyOld))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_self_weight_));
		return;
	}

	if (!m_LoadCase.GetSelectedLoad(nLoadCaseType, nLoadCaseKey)) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Load_case_was_not_selected_));
		return;
	}
	
	GetFactor(x, y, z);

	data.SX = x;
	data.SY = y;
	data.SZ = z;
	data.LoadCaseKey = nLoadCaseKey;
	
	T_LDGR_K LdgrK;
	m_wndLdgrCobx.GetSelectedLdgr(LdgrK);
	data.GroupKey = LdgrK;
		
	m_pDoc->m_pDataCtrl->ModifyBodf(KeyOld,data);
}

void CStldBodfDlg::OnCmdBfDelete() 
{
	// TODO: Add your control notification handler code here

	T_BODF_K key;

	if (!GetSelectedBodyForce(key))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_self_weight_));
		return;
	}
	m_pDoc->m_pDataCtrl->DelBodf(key);
}

void CStldBodfDlg::OnChangedItemSelection(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	T_BODF_K key;
	T_BODF_D data;
	if (!GetSelectedBodyForce(key))
	{
		data.SX = 0.0; data.SY = 0.0; data.SZ = 0.0;
	}
	else
	{
		if (!m_pDoc->m_pAttrCtrl->GetBodf(key, data)) return;
		// Load case 선택을 변경한다.
		int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
		CString sLoadCaseName = m_List.GetItemText(iItem, 0);
		//m_LoadCase.SelectString(-1, sLoadCaseName);
		T_BODF_K nLoadCaseKey = m_pDoc->m_pAttrCtrl->GetStldKey(sLoadCaseName);
		m_LoadCase.ChangeSelect(D_LOADCASE_STATIC, nLoadCaseKey);
		m_wndLdgrCobx.ChangeSelect(data.GroupKey);
	}
	// 값을 설정한다.
	CString sValue;
	sValue.Format(BF_DATA_FMT, data.SX);
	m_FactorX.SetWindowText(sValue);
	sValue.Format(BF_DATA_FMT, data.SY);
	m_FactorY.SetWindowText(sValue);
	sValue.Format(BF_DATA_FMT, data.SZ);
	m_FactorZ.SetWindowText(sValue);
	
	*pResult = 0;
}

void CStldBodfDlg::GetFactor(double &x, double &y, double &z)
{
	CString value;
	m_FactorX.GetWindowText(value);
	GetFloatNumber(value, x);
	m_FactorY.GetWindowText(value);
	GetFloatNumber(value, y);
	m_FactorZ.GetWindowText(value);
	GetFloatNumber(value, z);
}

BOOL CStldBodfDlg::GetSelectedBodyForce(T_BODF_K &key)
{
	
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	key = (T_BODF_K)m_List.GetItemData(iItem);
	/*
	CString sLoadCaseName = m_List.GetItemText(iItem, 0);
	key = (T_BODF_K)m_pDoc->m_pAttrCtrl->GetStldKey(sLoadCaseName);
	*/

	return TRUE;
}

void CStldBodfDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountBodf();
	if(nItemCount == 0) return;

	T_BODF_K Key;
	T_BODF_D Data;
	CArray<UINT, UINT> KeyBuffer;
	POSITION Pos;
	Pos = m_pDoc->m_pAttrCtrl->GetStartBodf();
	while(Pos != NULL)
	{
		m_pDoc->m_pAttrCtrl->GetNextBodf(Pos,Key,Data);
		KeyBuffer.Add(Key);
	}
	
	int nSize = KeyBuffer.GetSize();
	CStringArray Contents;
	CString      TStr;
	T_STLD_D     LcData  ; // Load Case   
	T_LDGR_D     LdgrData; // Load Group Data
	if(nSize)
	{
		qsort(KeyBuffer.GetData(),nItemCount,sizeof(T_BODF_K), CCompFunc::UINTAsc);
		for(int i = 0 ;i < nSize; i++)
		{
			if(m_pDoc->m_pAttrCtrl->GetBodf(KeyBuffer[i],Data))
			{
				//Data.LoadCaseKey;
				if(m_pDoc->m_pAttrCtrl->GetStld(Data.LoadCaseKey,LcData))
				{
					Contents.RemoveAll();
					Contents.Add(LcData.LoadCaseName);
					TStr.Format(BF_DATA_FMT,Data.SX);
					Contents.Add(TStr);
					TStr.Format(BF_DATA_FMT,Data.SY);
					Contents.Add(TStr);
					TStr.Format(BF_DATA_FMT,Data.SZ);
					Contents.Add(TStr);
					
					if(m_pDoc->m_pAttrCtrl->GetLdgr(Data.GroupKey,LdgrData))
					{
						Contents.Add(LdgrData.GroupName);
					}
					else
					{
						Contents.Add(_LS(IDS_WG_TREEMENU_ADD3_Default)); 
					}
					CDlgUtil::SetListItem(&m_List,i,Contents,(DWORD)KeyBuffer[i]);
				}
			}
		}
	}
}

CString CStldBodfDlg::DataToStr(int i, T_BODF_K key, T_BODF_D &data)
{
	CString value;
	T_STLD_D LoadData;
	if (i == 0) 
	{
		VERIFY(m_pDoc->m_pAttrCtrl->GetStld((T_STLD_K)key, LoadData));
		value = LoadData.LoadCaseName;
	}
	else if (i == 1) value.Format(BF_DATA_FMT, data.SX);
	else if (i == 2) value.Format(BF_DATA_FMT, data.SY);
	else if (i == 3) value.Format(BF_DATA_FMT, data.SZ);
	return value;
}

void CStldBodfDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		MakeItemEx();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		MakeItemEx();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

BOOL CStldBodfDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	T_BODF_D data;

	m_pDoc->m_pAttrCtrl->GetBodf(key, data);
	CString value;
	value.Format(_LS(IDS_WG_TREEMENU__g), data.SX);
	m_FactorX.SetWindowText(value);
	value.Format(_LS(IDS_WG_TREEMENU__g), data.SY);
	m_FactorY.SetWindowText(value);
	value.Format(_LS(IDS_WG_TREEMENU__g), data.SZ);
	m_FactorZ.SetWindowText(value);

	UpdateData(FALSE);
	return TRUE;
}

void CStldBodfDlg::OnTmDefineLdgrBtn() 
{
	// TODO: Add your control notification handler code here
	CreateOrActivateDlg(m_pDoc,CCMLoadGroupDlg::IDD);	
}

