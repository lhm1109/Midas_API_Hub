// CmdBarNmrdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdBarNmrdDlg.h"

#include "..\wg_base\DlgUtil.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_TreeMenuMode.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"


#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_StagCtrl.h"

#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdBarNmrdDlg dialog
IMPLEMENT_DYNCREATE(CCmdBarNmrdDlg, CCMDlgBarBase)


	CCmdBarNmrdDlg::CCmdBarNmrdDlg()
	: CCMDlgBarBase(CCmdBarNmrdDlg::IDD)
{
	//{{AFX_DATA_INIT(CCmdBarNmrdDlg)
	m_nAutoUser   = 0;
	m_csUserWidth = _T("0.0");
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint(); 
}


void CCmdBarNmrdDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdBarNmrdDlg)
	DDX_Control(pDX, IDC_NMRD_SUPPORT_LIST, m_lstSupport);
	DDX_Control(pDX, IDC_NMRD_NODE_LIST, m_edtNodeList);
	DDX_Control(pDX, IDC_NMRD_MM_STATIC, m_stcLength);
	DDX_Control(pDX, IDC_NMRD_LOADCASE_TO, m_lstLdcTo);
	DDX_Control(pDX, IDC_NMRD_LOADCASE_FROM, m_lstLdcFrom);
	DDX_Control(pDX, IDC_NMRD_GDGRUP_CBX, m_cbxGrup);
	DDX_Radio(pDX, IDC_NMRD_AUTO_RADIO, m_nAutoUser);
	DDX_Text(pDX, IDC_NMRD_USER_EDIT, m_csUserWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdBarNmrdDlg, CCMDlgBarBase)
	//{{AFX_MSG_MAP(CCmdBarNmrdDlg)
	ON_BN_CLICKED(IDC_NMRD_DEL, OnNmrdDel)
	ON_BN_CLICKED(IDC_NMRD_ADDREP, OnNmrdAddrep)
	ON_BN_CLICKED(IDC_NMRD_AUTO_RADIO, OnNmrdAutoUserRadio)
	ON_BN_CLICKED(IDC_NMRD_LDCSEL_BTN, OnNmrdLdcselBtn)
	ON_BN_CLICKED(IDC_NMRD_LDCUNSEL_BTN, OnNmrdLdcunselBtn)
	ON_CBN_SELCHANGE(IDC_NMRD_GDGRUP_CBX, OnSelchangeNmrdGdgrupCbx)
	ON_BN_CLICKED(IDC_NMRD_GDGRUP_DET, OnNmrdGdgrupDet)
	ON_BN_CLICKED(IDC_NMRD_USER_RADIO, OnNmrdAutoUserRadio)
	ON_BN_CLICKED(IDC_NMRD_DELETE_DATA, OnNmrdDeleteData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdBarNmrdDlg message handlers

void CCmdBarNmrdDlg::OnInitialUpdate() 
{
	CCMDlgBarBase::OnInitialUpdate();

	m_data.Initialize();
	BOOL bExistNmrd = m_pDoc->m_pAttrCtrl->GetNmrd(m_data);

	m_edtNodeList.Connect(SC_ID_NODE, &m_edtNodeList);
	MakeListHead();
	MakeLoadList();
	m_stcLength.SetUnitType(D_UNITSYS_BASE_LENGTH);
	UpdateData(FALSE);

	OnNmrdAutoUserRadio();
	MakeListData();
	MakeGrupData(); 
	GetDlgItem(IDC_NMRD_DELETE_DATA)->EnableWindow(bExistNmrd);
	GetDlgItem(IDC_NMRD_OK)->ShowWindow(FALSE);
	GetDlgItem(IDC_NMRD_CANCEL)->ShowWindow(FALSE);
}


void CCmdBarNmrdDlg::MakeGrupData()
{
	CArray<CString, CString&>strGrupList;
	CArray<T_GRUP_K, T_GRUP_K>GrupKeyList;
	m_pDoc->m_pAttrCtrl->GetGrupKeyList(GrupKeyList);
	m_pDoc->m_pAttrCtrl->GetGrupList(strGrupList);
	m_cbxGrup.ResetContent(); 

	int nIdx ;
	int nSelIdx = 0;
	for(int i=0; i<GrupKeyList.GetSize(); i++)
	{
		nIdx = m_cbxGrup.AddString(strGrupList[i]);
		m_cbxGrup.SetItemData(nIdx,GrupKeyList[i]);

		if(GrupKeyList[i] == m_data.GrupK)
		{
			nSelIdx = i;
		}
	}
	m_cbxGrup.SetCurSel(nSelIdx);
	OnSelchangeNmrdGdgrupCbx();
}


void CCmdBarNmrdDlg::MakeListHead()
{
	CStringArray Contents;
	CArray<float, float> HRatio;

	Contents.Add(_LS(IDS_CMD_NMRD_No));    HRatio.Add(0.2f);
	Contents.Add(_LS(IDS_CMD_NMRD_Node));   HRatio.Add(0.3f);
	Contents.Add(_LS(IDS_CMD_NMRD_Width));  HRatio.Add(0.5f);

	CDlgUtil::_SetListCtrlHeader(&m_lstSupport,Contents,&HRatio,NULL);
}


void CCmdBarNmrdDlg::GetTotalLoadCase(T_SELECTEDLOAD_LIST &aLoadList)
{
	CSelectLC::GetSelectedLoadName(D_SELECTLOAD_STATIC | D_SELECTLOAD_MOVING | D_SELECTLOAD_GRIDMODEL,aLoadList);

	T_STCT_D StctD; StctD.Initialize();
	if(m_pDoc->m_pAttrCtrl->ExistStct()) m_pDoc->m_pAttrCtrl->GetStct(StctD);

	UINT DL;
	CString str;
	str.Format(_T("%s"),_LS(IDS_DB_SGLD_DL));  DL=m_pDoc->m_pAttrCtrl->GetSgldKey(str);

	// 시공단계 Dead Load만 사용함 
	for(int i=aLoadList.m_aLoad.GetSize()-1; i>=0; i--)
	{
		if(aLoadList.m_aLoad[i].nLoadType==D_LOADCASE_STAGE)
		{
			if(aLoadList.m_aLoad[i].nLoadKey!=DL)
			{
				aLoadList.m_aLoad.RemoveAt(i);
				aLoadList.m_aLoadName.RemoveAt(i);
				continue; 
			}
		}
		if(aLoadList.m_aLoad[i].nLoadType==D_LOADCASE_COMB_GENERAL  || 
			aLoadList.m_aLoad[i].nLoadType==D_LOADCASE_COMB_STEEL    || 
			aLoadList.m_aLoad[i].nLoadType==D_LOADCASE_COMB_CONCRETE || 
			aLoadList.m_aLoad[i].nLoadType==D_LOADCASE_COMB_FDN      ||
			aLoadList.m_aLoad[i].nLoadType==D_LOADCASE_COMB_SRC      ||
			aLoadList.m_aLoad[i].nLoadType==D_LOADCASE_COMB_ALUMINUM ||
			aLoadList.m_aLoad[i].nLoadType==D_LOADCASE_COMB_STLCOMP)
		{
			aLoadList.m_aLoad.RemoveAt(i);
			aLoadList.m_aLoadName.RemoveAt(i);
			continue; 
		}
	}

	if(m_pDoc->m_pAttrCtrl->ExistConstStag())
	{
		CString sLoadName;
		T_SELECTEDLOAD SelectLoad;

		SelectLoad.nLoadKey     = DL;
		SelectLoad.nLoadMaxMin  = 0;
		SelectLoad.nLoadType    = D_LOADCASE_STAGE;
		sLoadName.Format(_T("CS: %s"),_LS(IDS_DB_SGLD_DL));
		aLoadList.m_aLoad.Add(SelectLoad);
		aLoadList.m_aLoadName.Add(sLoadName);

		for(int i=0; i<StctD.aErection.GetSize(); i++)
		{
			if(StctD.aErection[i].nNumStctCase > 0)
			{
				str.Format(_T("%s"), StctD.aErection[i].strErection);

				SelectLoad.nLoadKey     = m_pDoc->m_pAttrCtrl->GetSgldKey(str);
				SelectLoad.nLoadMaxMin  = 0;
				SelectLoad.nLoadType    = D_LOADCASE_STAGE;
				sLoadName.Format(_T("CS: %s"), StctD.aErection[i].strErection);
				aLoadList.m_aLoad.Add(SelectLoad);
				aLoadList.m_aLoadName.Add(sLoadName);
			}
		}
	}
}

void CCmdBarNmrdDlg::OnNmrdCancel() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);		
}

void CCmdBarNmrdDlg::OnTmExecute() 
{
	if(!m_pDoc->m_pDataCtrl->AddNmrd(m_data))return; 
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);		
}

void CCmdBarNmrdDlg::OnTmClose()
{
	if (CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CCmdBarNmrdDlg::OnNmrdDel() 
{
	CArray<int,int> arRet;
	CDlgUtil::GetListItemByMask(&m_lstSupport,arRet,LVIS_SELECTED);
	if(!arRet.GetSize())return;

	m_lstSupport.DeleteAllItems();

	for(int i=arRet.GetSize()-1; i>=0; i--)
	{
		for(int j=m_data.aNodeInfo.GetSize()-1; j>=0; j--)
			if(j==arRet[i])
			{
				m_data.aNodeInfo.RemoveAt(j);
			}
	}
	MakeListData(); 
}

void CCmdBarNmrdDlg::OnNmrdAddrep() 
{
	UpdateData(TRUE); 

	CArray<UINT,UINT> arNodeList;
	GetSelectedNode(arNodeList);
	if(!arNodeList.GetSize())return;

	T_NMRD_NODE_BASE NmrdNode;

	if(m_nAutoUser==0)  
	{
		NmrdNode.bAuto  = TRUE; 
		NmrdNode.dWidth = 0.0;
	}
	else                
	{
		NmrdNode.bAuto  = FALSE; 
		NmrdNode.dWidth = _tstof(m_csUserWidth);
	}

	for(int i=0; i<arNodeList.GetSize(); i++)
	{
		BOOL bExist=FALSE;
		NmrdNode.NodeK = arNodeList[i];
		for(int j=0; j<m_data.aNodeInfo.GetSize(); j++)
		{
			if(m_data.aNodeInfo[j].NodeK==arNodeList[i])
			{
				bExist = TRUE; 
				m_data.aNodeInfo.SetAt(j,NmrdNode);
				break; 
			}
		}
		if(bExist)continue; 
		m_data.aNodeInfo.Add(NmrdNode);
	}
	MakeListData();
}

void CCmdBarNmrdDlg::MakeListData()
{
	CString sTmp;
	CStringArray Contents;
	for(int i=0; i<m_data.aNodeInfo.GetSize(); i++)
	{
		Contents.RemoveAll(); 
		sTmp.Format(_T("%d"),i+1);
		Contents.Add(sTmp);
		sTmp.Format(_T("%d"),m_data.aNodeInfo[i].NodeK);
		Contents.Add(sTmp);
		if(m_data.aNodeInfo[i].bAuto)
			sTmp=_T("Auto");
		else  
			sTmp.Format(_T("%g"),m_data.aNodeInfo[i].dWidth);
		Contents.Add(sTmp);
		CDlgUtil::SetListItem(&m_lstSupport,i,Contents,NULL);
	}
}

void CCmdBarNmrdDlg::OnNmrdAutoUserRadio() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_NMRD_USER_EDIT)->EnableWindow(m_nAutoUser);	
}


void  CCmdBarNmrdDlg::GetSelectedNode(CArray<UINT,UINT> &arNodeList)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	arNodeList.RemoveAll(); 
	pIGM->GetSelectedNodeKeyList(arNodeList);
}

//--------------------------------------------------------------------------
// Load Case관련 
//--------------------------------------------------------------------------
void CCmdBarNmrdDlg::MakeLoadList()
{
	T_SELECTEDLOAD_LIST aLoadList;
	GetTotalLoadCase(aLoadList);

	m_lstLdcTo.ResetContent();
	m_lstLdcFrom.ResetContent();
	m_arLTypeFrom.RemoveAll();
	m_arLTypeTo.RemoveAll();

	int nToIdx = 0, nFromIdx = 0;
	for(int i=0; i<aLoadList.m_aLoad.GetSize(); i++)
	{
		BOOL bExist=FALSE;
		for(int j=0; j<m_data.aCaseInfo.GetSize(); j++)
		{
			if(m_data.aCaseInfo[j].LoadCaseK==aLoadList.m_aLoad[i].nLoadKey && 
				m_data.aCaseInfo[j].nLoadType==GetDBLcomType(aLoadList.m_aLoad[i].nLoadType) )
			{
				m_lstLdcTo.AddString(GetLoadCaseName(aLoadList.m_aLoadName[i],aLoadList.m_aLoad[i].nLoadType));
				m_lstLdcTo.SetItemData(nToIdx,aLoadList.m_aLoad[i].nLoadKey);
				m_arLTypeTo.Add(aLoadList.m_aLoad[i].nLoadType);
				nToIdx ++ ;
				bExist = TRUE; 
				break; 
			}
		}
		if(bExist)continue; 
		m_lstLdcFrom.AddString(GetLoadCaseName(aLoadList.m_aLoadName[i],aLoadList.m_aLoad[i].nLoadType));
		m_lstLdcFrom.SetItemData(nFromIdx,aLoadList.m_aLoad[i].nLoadKey);
		m_arLTypeFrom.Add(aLoadList.m_aLoad[i].nLoadType);
		nFromIdx ++ ;
	}
	MakeListBoxHorzScroll();
}

void CCmdBarNmrdDlg::OnNmrdLdcselBtn() 
{
	T_NMRD_CASE_BASE CaseBase;
	int nCount = m_lstLdcFrom.GetCount(); 
	if(m_arLTypeFrom.GetSize()!=nCount){ ASSERT(0); return; }

	BOOL bChanged=FALSE;
	for(int i=0; i<nCount; i++)
	{
		if(m_lstLdcFrom.GetSel(i)>0)
		{
			CaseBase.LoadCaseK = m_lstLdcFrom.GetItemData(i);
			CaseBase.nLoadType = GetDBLcomType(m_arLTypeFrom[i]);
			m_data.aCaseInfo.Add(CaseBase);
			bChanged = TRUE;
		}
	}
	if(bChanged)MakeLoadList();
}

void CCmdBarNmrdDlg::OnNmrdLdcunselBtn() 
{
	int nCount = m_lstLdcTo.GetCount(); 
	if(m_arLTypeTo.GetSize()!=nCount){ ASSERT(0); return; }

	BOOL bChanged = FALSE; 
	for(int i=nCount-1; i>=0; i--)
	{
		if(m_lstLdcTo.GetSel(i)>0)
		{
			m_data.aCaseInfo.RemoveAt(i);
			bChanged = TRUE; 
		}
	}
	if(bChanged)MakeLoadList();
}

UINT CCmdBarNmrdDlg::GetDBLcomType(UINT nSelLcomType)
{
	UINT nLCombType;
	switch(nSelLcomType)
	{
	case D_LOADCASE_STATIC:
		nLCombType = D_LCOM_STATIC;
		break; 
	case D_LOADCASE_MOVING:
		nLCombType = D_LCOM_MOVING ;
		break; 
	case D_LOADCASE_STAGE:
		nLCombType = D_LCOM_STAGE;
		break; 
	case D_LOADCASE_GRIDMODEL:
		nLCombType = D_LCOM_GRIDMODEL ;
		break; 
	default:
		ASSERT(0);
		break; 
	}
	return nLCombType;
}


void CCmdBarNmrdDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	BOOL bUpdate=FALSE;
	BOOL bClose =FALSE;
	if(!::IsWindow(GetSafeHwnd()))return; 

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		bUpdate = UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		bUpdate = UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	case D_UPDATE_POST_STAGE_CHANGED:
		if(!CDBDoc::GetDocPoint()->m_pStagCtrl->IsFinalStag())
		{
			bClose = TRUE;
		}
		break;
	case D_UPDATE_STAGE_CHANGED:
		if(!CDBDoc::GetDocPoint()->m_pStagCtrl->IsBaseStag())
		{
			bClose = TRUE;
		}
		break; 
	case D_UPDATE_SEL_PRE:
	case D_UPDATE_SEL_POST:
		bClose = TRUE;
		break;
	default:
		//ASSERT(FALSE);
		break;
	}

	if(bClose)
	{
		CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);		
		return;
	}

	if(bUpdate)
	{
		T_SELECTEDLOAD_LIST aLoadList;  
		GetTotalLoadCase(aLoadList);
		CMap<UINT,UINT,UINT,UINT> mapLoadCaseK;
		mapLoadCaseK.InitHashTable(1001);

		UINT LoadK;
		BOOL bRemoved=FALSE;
		for(int i=0; i<aLoadList.m_aLoad.GetSize(); i++)
		{ 
			mapLoadCaseK.SetAt(aLoadList.m_aLoad[i].nLoadKey,aLoadList.m_aLoad[i].nLoadKey);  
		}
		for(int i=m_data.aCaseInfo.GetSize()-1; i>=0; i--)
		{
			if(!mapLoadCaseK.Lookup(m_data.aCaseInfo[i].LoadCaseK,LoadK))
			{
				m_data.aCaseInfo.RemoveAt(i);
				//        bRemoved = TRUE; 
			}
		}
		//if(bRemoved)MakeLoadList();
		MakeLoadList(); 
	}
}

BOOL CCmdBarNmrdDlg::UpdateBuffer()
{
	BOOL bUpdate=FALSE;
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return bUpdate; 
	ASSERT(nCount == 1);

	// 현재 Row, Col을 저장 : 나중에 복구하기 위함
	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bModifiedNmrd=FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_STLD_ADD):
		case(UR_STLD_DEL):
		case(UR_STLD_MFD):
		case(UR_MVLD_ADD):
		case(UR_MVLD_DEL):
		case(UR_MVLD_MFD):
		case(UR_MVLDch_ADD):
		case(UR_MVLDch_DEL):
		case(UR_MVLDch_MFD):
		case(UR_MVLDbs_ADD):
		case(UR_MVLDbs_DEL):
		case(UR_MVLDbs_MFD):        
		case(UR_MVLDjp_ADD):
		case(UR_MVLDjp_DEL):
		case(UR_MVLDjp_MFD):
		case(UR_GILC_ADD):
		case(UR_GILC_DEL):
		case(UR_GILC_MFD):
			bUpdate = TRUE; 
			break;
		case(UR_NMRD_DEL):
			bModifiedNmrd = TRUE;
			break; 
		case(UR_NMRD_ADD):
			bModifiedNmrd = TRUE; 
			break;
		case(UR_GRUP_DEL): 
		case(UR_GRUP_ADD): 
			MakeGrupData();
			break;
		default:
			break;
		}
	} 
	if(bModifiedNmrd)
	{
		m_data.Initialize(); 
		BOOL bExistNmrd = m_pDoc->m_pAttrCtrl->GetNmrd(m_data);
		MakeLoadList();
		UpdateData(FALSE);
		OnNmrdAutoUserRadio();
		m_lstSupport.DeleteAllItems(); 
		MakeListData();
		MakeGrupData(); 
		GetDlgItem(IDC_NMRD_DELETE_DATA)->EnableWindow(bExistNmrd);
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		if(!pDoc) { ASSERT(0); return FALSE; }
		pDoc->m_pViewCtrl->UnselectAll(NULL);	
	}

	return bUpdate;
}

CString CCmdBarNmrdDlg::GetLoadCaseName(CString sLoadName,UINT nType)
{
	switch(nType)
	{
	case D_LOADCASE_STATIC:
		//sLoadName += _T("(ST)");
		break; 
	case D_LOADCASE_MOVING:
		//sLoadName += _T("(MV)");
		break; 
	case D_LOADCASE_STAGE:
		//sLoadName += _T("(CS)");
		break; 
	case D_LOADCASE_GRIDMODEL:
		//sLoadName += _T("(GI)");
		break; 
	default:
		ASSERT(0);
		break; 
	}
	return sLoadName; 
}

void CCmdBarNmrdDlg::OnSelchangeNmrdGdgrupCbx() 
{
	int nSelIdx   = m_cbxGrup.GetCurSel(); 
	m_data.GrupK  = m_cbxGrup.GetItemData(nSelIdx);
}

void CCmdBarNmrdDlg::MakeListBoxHorzScroll()
{
	// Find the longest string in the list box.
	CString str;
	CSize   sz;
	int     dx=0;
	CDC*    pDC = m_lstLdcFrom.GetDC();
	for (int i=0;i < m_lstLdcFrom.GetCount();i++)
	{
		m_lstLdcFrom.GetText( i, str );
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx) dx = sz.cx;
	}
	m_lstLdcFrom.ReleaseDC(pDC);
	m_lstLdcFrom.SetHorizontalExtent(dx);

	dx=0;
	pDC = m_lstLdcTo.GetDC();
	for (int i=0;i < m_lstLdcTo.GetCount();i++)
	{
		m_lstLdcTo.GetText( i, str );
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx) dx = sz.cx;
	}
	m_lstLdcTo.ReleaseDC(pDC);
	m_lstLdcTo.SetHorizontalExtent(dx);
}
void CCmdBarNmrdDlg::OnNmrdGdgrupDet() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_GROUP,0));  	
}

void CCmdBarNmrdDlg::OnNmrdDeleteData() 
{
	m_pDoc->m_pDataCtrl->DelNmrd();	
	//GetDlgItem(IDC_NMRD_DELETE_DATA)->EnableWindow(FALSE);
}
