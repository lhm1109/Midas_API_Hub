// StldPtmpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldPtmpDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_HPOS_Z1    0
#define D_HPOS_Z2    1
#define D_HPOS_Z3    2
/////////////////////////////////////////////////////////////////////////////
// CStldPtmpDlg dialog

CStldPtmpDlg::CStldPtmpDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldPtmpDlg::IDD, pParent)
{
	m_nDirection = 1;
	m_nOption = 0;
	m_nRefPosition = 0;
	m_pData = new T_PTMP_D;
}

CStldPtmpDlg::~CStldPtmpDlg()
{
	delete m_pData;
}


void CStldPtmpDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldPtmpDlg)
	DDX_Control(pDX, IDC_TM_CBO_LC,      m_SelectLC);
	DDX_Control(pDX, IDC_TM_LDGR_CBO_LC, m_CobxLdgr);

	DDX_Radio(pDX, IDC_TM_OPT_ADD,       m_nOption);
	DDX_Radio(pDX, IDC_TM_REF_POS_RADIO2, m_nRefPosition);

	DDX_Control(pDX, IDC_TM_T0_TEMP_UNIT, m_wndT0Unit);
	DDX_Control(pDX, IDC_TM_T0_TEMP_EDIT, m_wndT0Edit);
	DDX_Control(pDX, IDC_TM_H1_EDIT, m_wndH1Edit);
	DDX_Control(pDX, IDC_TM_H1_UNIT, m_wndH1Unit);
	DDX_Control(pDX, IDC_TM_T1_EDIT, m_wndT1Edit);
	DDX_Control(pDX, IDC_TM_T1_UNIT, m_wndT1Unit);

	DDX_Control(pDX, IDC_TM_STEMP_LIST,       m_wndSTempList);
	DDX_Control(pDX, IDC_TM_PICTURE,          m_wndPicture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStldPtmpDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CStldPtmpDlg)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	ON_BN_CLICKED(IDC_TM_BTN_LC,          OnTmBtnLc)
	ON_BN_CLICKED(IDC_TM_OPT_ADD,         OnTmOptChange)
	ON_BN_CLICKED(IDC_TM_OPT_DELETE,      OnTmOptChange)
	ON_BN_CLICKED(IDC_TM_OPT_REPLACE,     OnTmOptChange)
	ON_BN_CLICKED(IDC_TM_ADD_BUTTON,      OnTmAddButton)
	ON_BN_CLICKED(IDC_TM_DELETE_BUTTON,   OnTmDeleteButton)
	ON_BN_CLICKED(IDC_TM_MODIFY_BUTTON,   OnTmModifyButton)
	ON_BN_CLICKED(IDC_TM_EXECUTE,         OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE,           OnTmClose)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_STEMP_LIST, OnItemchangedTmStempList)
	ON_BN_CLICKED(IDC_TM_T0_TEMP_BTN,     OnTmT0TempBtn)
	ON_BN_CLICKED(IDC_TM_REF_POS_RADIO2,  OnTmRefPostRadio)
	ON_BN_CLICKED(IDC_TM_REF_POS_RADIO3,  OnTmRefPostRadio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CStldPtmpDlg message handlers

BOOL CStldPtmpDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	m_pData->Initialize();

	m_SelectLC.SetLoadType(D_LOADCASE_STATIC,TRUE); // D_SELECTLOAD_????? 사용할것

	MakeListHeader();

	m_wndH1Unit.SetUnitType(CUnitCtrl::m_PTMP_UNIT.dH);
	m_wndH1Edit.SetUnitType(CUnitCtrl::m_PTMP_UNIT.dH);
	m_wndT1Unit.SetUnitType(CUnitCtrl::m_PTMP_UNIT.dT);
	m_wndT1Edit.SetUnitType(CUnitCtrl::m_PTMP_UNIT.dT);
	m_wndT0Unit.SetUnitType(CUnitCtrl::m_STMP_UNIT.dblSysTemp);
	m_wndT0Edit.SetReadOnly(TRUE);

	UpdateData(FALSE);

	ChangeBitmap(m_nRefPosition);
	MakeListData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldPtmpDlg::MakeListHeader()
{
	this->m_wndSTempList;
	CStringArray HTitles;
	CArray<float, float> HRatio;
	HTitles.Add(_T("No.")); HRatio.Add(0.18f);
	//HTitles.Add(_T("Ref.")); HRatio.Add(0.20f);
	HTitles.Add(_T("H") ); HRatio.Add(0.30f);
	HTitles.Add(_T("T") ); HRatio.Add(0.30f);

	CDlgUtil::_SetListCtrlHeader(&m_wndSTempList,HTitles,&HRatio,NULL);
}

int CStldPtmpDlg::FindEndSectTemp()
{
	for(int i = 0;i < m_pData->SectTmp.GetSize(); i++)
	{
		if(m_pData->SectTmp[i].nRef == -1)
			return i;
	}
	
	return -1;  // Section Temperature Data 최대 개수 초과 
}

BOOL CStldPtmpDlg::GetSectTemp(T_PTMP_STMP * pSectTempD)
{
	pSectTempD->nRef     = m_nRefPosition;
	pSectTempD->dH1      = m_wndH1Edit.GetEditValue();
	pSectTempD->dT1      = m_wndT1Edit.GetEditValue();

	return TRUE;
}

BOOL CStldPtmpDlg::ModifySecTemp()
{
	UpdateData(TRUE);
	CArray<int, int> arRet;
	CDlgUtil::GetSelectedListItemIndex(&m_wndSTempList, arRet);
	int nRet = arRet.GetSize();
	if(nRet == 0) return FALSE;
	
	for(int i = 0;i < nRet; i++)
	{
		if(FALSE) // pStmp 데이터가 Release 에서만 존재한다; 그래서 Debug에서도 확인 가능하도록 수정하고 원래 부분은 그냥 남겨둠
		{
			T_PTMP_STMP * pStmp = (T_PTMP_STMP*)m_wndSTempList.GetItemData(arRet[i]);
			if (pStmp)
			{
				GetSectTemp(pStmp);
			}
		}
		else
		{
			GetSectTemp(&m_pData->SectTmp[arRet[i]]);
		}
	}

	MakeListData();   
	return TRUE;
}

BOOL CStldPtmpDlg::DeleteSecTemp()
{
	CArray<int, int> arRet;
	CDlgUtil::GetSelectedListItemIndex(&m_wndSTempList, arRet);
	int nRet = arRet.GetSize();
	if(nRet == 0) return FALSE;

	if (FALSE) // pStmp 데이터가 Release 에서만 존재한다; 그래서 Debug에서도 확인 가능하도록 수정하고 원래 부분은 그냥 남겨둠. 
	{
		T_PTMP_STMP * pStmp;
		for (int i = 0; i < nRet; i++)
		{
			pStmp = (T_PTMP_STMP*)m_wndSTempList.GetItemData(arRet[i]);
			if (pStmp)
				pStmp->Initialize();
		}

		for (int i = 0; i < m_pData->SectTmp.GetSize(); i++)
		{
			m_pData->SectTmp[i].Initialize();
		}

	}
	else
	{
		//1)
		CArray<T_PTMP_STMP, T_PTMP_STMP&> SectTmpTemp;
		SectTmpTemp.Copy(m_pData->SectTmp);
		for (int i = 0; i < nRet; i++) SectTmpTemp[arRet[i]].Initialize();

		//2)
		for (int i = 0; i < m_pData->SectTmp.GetSize(); i++)
		{
			m_pData->SectTmp[i].Initialize();
		}

		//3)
		int nIdx = 0;
		for (int i = 0; i < SectTmpTemp.GetSize(); i++)
		{
			if (SectTmpTemp[i].nRef > -1)
			{
				m_pData->SectTmp[nIdx] = SectTmpTemp[i];
				nIdx++;
			}
		}
	}

	MakeListData();   
	return TRUE;
}

BOOL CStldPtmpDlg::AddSecTemp()
{
	UpdateData(TRUE);
	int nEndSecTemp = FindEndSectTemp();
	if( nEndSecTemp < 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_ERROR_PTMP_SIZE_LIMIT), D_PTMP_NUMSTMP); // 지정할 수 있는 Plate Thickness Temperature Data개수는 %d개 입니다
		return FALSE;
	}

	GetSectTemp(&(m_pData->SectTmp[nEndSecTemp]));
	MakeListData();
	return TRUE;
}


BOOL CStldPtmpDlg::GetParameter(T_PTMP_D * pPtmpD)
{
	UpdateData();

	unsigned int nLoadCaseType;
	if (!m_SelectLC.GetSelectedLoad(nLoadCaseType, pPtmpD->LoadCaseKey))
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___Loadcase_is_not_selected_));
		return FALSE;
	}

	T_LDGR_K LdgrK;
	m_CobxLdgr.GetSelectedLdgr(LdgrK);
	pPtmpD->GroupKey     = LdgrK;
	pPtmpD->nRefPosition = m_nRefPosition;

	for (int i = 0; i < pPtmpD->SectTmp.GetSize(); i++)
	{
		if (m_pData->SectTmp[i].nRef == -1)
			break;

		pPtmpD->SectTmp[i].nRef = m_nRefPosition;
	}

	return TRUE;
}

void CStldPtmpDlg::MakeListData()
{
	m_wndSTempList.DeleteAllItems();
	CStringArray Contents;
	CString      StrTemp ;
	for(int i = 0;i < m_pData->SectTmp.GetSize(); i++)
	{
		if(m_pData->SectTmp[i].nRef == -1)
			break;
		Contents.RemoveAll();

		StrTemp.Format(_T("%d"),i+1);
		Contents.Add(StrTemp);

		StrTemp.Format(_T("%g"), m_pData->SectTmp[i].dH1);
		Contents.Add(StrTemp);
		StrTemp.Format(_T("%g"), m_pData->SectTmp[i].dT1);
		Contents.Add(StrTemp);

		CDlgUtil::SetListItem(&m_wndSTempList,i,Contents,(LONG_PTR)((m_pData->SectTmp.GetData())+i));
	}
}


void CStldPtmpDlg::OnTmDefineLdgrBtn() 
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(),CCMLoadGroupDlg::IDD);		
}

void CStldPtmpDlg::OnTmBtnLc() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));	
}

void CStldPtmpDlg::OnTmOptChange() 
{
	UpdateData();
}

void CStldPtmpDlg::OnTmExecute() 
{
	CArray<UINT, UINT> aSelKey;
	T_ELEM_D ElemData;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyList(aSelKey);
	for (int i = aSelKey.GetSize()-1; i >= 0; i--)
	{
		CDBDoc::GetDocPoint()->m_pAttrCtrl->GetElem(aSelKey[i], ElemData);
		if (!CDBDoc::GetDocPoint()->m_pAttrCtrl->IsPlate(ElemData.eltyp))
			aSelKey.RemoveAt(i);
	}
	
	if (aSelKey.GetSize() == 0)
	{
		AfxMessageBox(_T("No selected frame element"));
		return;
	}

	BOOL bSuccess = FALSE;
	if(m_nOption == 0 || m_nOption == 1) //  Add/Replace
	{
		if(!GetParameter(m_pData))
		{
			return;
		}

		bSuccess = CDBDoc::GetDocPoint()->m_pDataCtrl->AddPtmp(aSelKey, *m_pData, m_nOption == 1);
	}
	else // Delete
	{
		T_STLD_K KeyStld;
		T_LDGR_K KeyLdgr;
		unsigned int nLoadCaseType;
		m_SelectLC.GetSelectedLoad(nLoadCaseType,KeyStld);
		m_CobxLdgr.GetSelectedLdgr(KeyLdgr);
		bSuccess = CDBDoc::GetDocPoint()->m_pDataCtrl->DelPtmp(aSelKey, KeyStld,KeyLdgr);
	}

	if (bSuccess) CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
}

void CStldPtmpDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CStldPtmpDlg::OnTmAddButton() 
{
	UpdateData();
	AddSecTemp();
}

void CStldPtmpDlg::OnTmDeleteButton() 
{
	UpdateData();
	DeleteSecTemp();
}

void CStldPtmpDlg::OnTmModifyButton() 
{
	UpdateData();
	ModifySecTemp();
}

BOOL CStldPtmpDlg::SetSectTemp(T_PTMP_STMP * pSectTempD)
{
	UpdateData(FALSE);

	m_wndH1Edit  .SetEditUnit(pSectTempD->dH1       );
	m_wndT1Edit  .SetEditUnit(pSectTempD->dT1       );

	return TRUE;
}

void CStldPtmpDlg::OnItemchangedTmStempList(NMHDR* pNMHDR, LRESULT* pResult) 
{
/*
	CArray<int, int> arRet;
	CDlgUtil::GetSelectedListItemIndex(&m_wndSTempList, arRet);
	int nRet = arRet.GetSize();
	if(nRet != 1) return;

	SetSectTemp((T_PTMP_STMP*)m_wndSTempList.GetItemData(arRet[0]));
*/

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if(pNMListView->uNewState==0 || pNMListView->uNewState==1)
	{
		*pResult = 0;
		return;
	}
	
	int nItem = pNMListView->iItem;
	if(nItem==-1)
	{
		*pResult = 0;
		return;
	}
	
	SetSectTemp(&m_pData->SectTmp[nItem]);
}

void CStldPtmpDlg::OnTmT0TempBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_CONFIG,0));
}


void CStldPtmpDlg::DoUnitChange()
{
	for(int i = 0;i < m_pData->SectTmp.GetSize(); i++)
	{
		if(m_pData->SectTmp[i].nRef == -1)
			break;
		m_pData->SectTmp[i].dH1 = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitLengthPrevious(m_pData->SectTmp[i].dH1);
	}
	MakeListData();
}


void CStldPtmpDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
		HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		//UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		//UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		DoUnitChange();
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

void CStldPtmpDlg::UpdateBuffer()
{
}


void CStldPtmpDlg::OnTmRefPostRadio()
{
	UpdateData(TRUE);
	m_pData->nRefPosition = m_nRefPosition;
	ChangeBitmap(m_nRefPosition);	
	MakeListData();
}

void CStldPtmpDlg::ChangeBitmap(int nPos)
{ 
	CString aBitmapID[] = { _T("SVG\\Illustration\\Dialog\\tm_btmp6.svg"), _T("SVG\\Illustration\\Dialog\\tm_btmp4.svg") }; // , IDB_TM_BTMP5, IDB_TM_BTMP6, IDB_TM_BTMP3, IDB_TM_BTMP4, IDB_TM_BTMP_PSC

	m_wndPicture.SetImage(aBitmapID[nPos]);
}

