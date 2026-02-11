// CMSihoElemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSihoElemDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_ArrayUtil.h"
#include "..\wg_base\wg_base_CompFunc.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_MembCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_common\wg_common_Query.h"

#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSihoElemDlg dialog

//IMPLEMENT_DYNCREATE(CCMSihoElemDlg, CCMChildBarBase)
enum LIST_IDX { IDX_ELEM, IDX_HINGE, IDX_FIBER };

CCMSihoElemDlg::CCMSihoElemDlg()
	: CCMChildBarBase(CCMSihoElemDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMSihoElemDlg)
	m_nElemType = ET_ALL;
	m_bFiberSectionResult = TRUE;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	m_bExistFiber = pDoc->m_pAttrCtrl->GetCountFibr(D_FIBR_TYPE_IEHP) + pDoc->m_pAttrCtrl->GetCountFibw(D_FIBR_TYPE_IEHP) > 0;
	//}}AFX_DATA_INIT
}


void CCMSihoElemDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSihoElemDlg)
	DDX_Radio(pDX, IDC_CMD_ALL_RDO, m_nElemType);
	DDX_Check(pDX, IDC_CMD_FIBER_RESULT_CHK, m_bFiberSectionResult);
	DDX_Control(pDX, IDC_CMD_HINGE_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSihoElemDlg, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMSihoElemDlg)
	ON_BN_CLICKED(IDC_CMD_ADD, OnAddBtn)
	ON_BN_CLICKED(IDC_CMD_DELETE, OnDeleteBtn)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnClose)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_HINGE_LIST, OnClickSelList)
	ON_BN_CLICKED(IDC_CMD_ALL_RDO  , OnElemTypeRdo)
	ON_BN_CLICKED(IDC_CMD_TRUSS_RDO, OnElemTypeRdo)
	ON_BN_CLICKED(IDC_CMD_BEAM_RDO , OnElemTypeRdo)
	ON_BN_CLICKED(IDC_CMD_WALL_RDO , OnElemTypeRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMSihoElemDlg message handlers

BOOL CCMSihoElemDlg::OnInitDialog() 
{
	CCMChildBarBase::OnInitDialog();	 
	InitList();
	MakeListItem();

	// GEN-1189 : Fiber 모델이 없는 경우 비활성화
	if(!m_bExistFiber)
	{
		m_bFiberSectionResult=FALSE;
		UpdateData(FALSE);
		GetDlgItem(IDC_CMD_FIBER_RESULT_CHK)->EnableWindow(m_bExistFiber);
	}

#if defined (_CIVIL)
	GetDlgItem(IDC_CMD_WALL_RDO)->ShowWindow(SW_HIDE);
#endif

	return TRUE;	// return TRUE unless you set the focus to a control
					// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSihoElemDlg::OnAddBtn() 
{
	UpdateData(TRUE);
 	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
 	GetSelectedElemKeyList(aSelKey);
 	if (aSelKey.GetSize() == 0) 
	{
		GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_No_element_is_selected_));  // No element is selected
		return;
	}
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	switch(m_nElemType)
	{
		case ET_TRUSS:// Truss
			GetElementByKind(aSelKey,TRUSS_EL);
			if (aSelKey.GetSize() == 0) 
			{
				GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_No_Truss_is_selected__));
				return;
			}
			break;
		case ET_BEAM:// Beam
			GetElementByKind(aSelKey,BEAM_EL);
			if (aSelKey.GetSize() == 0) 
			{
				GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_No_Beam_is_selected__));
				return;
			}
			break;
		case ET_WALL:// Wall
			{
				GetElementByKind(aSelKey,WALL_EL);
				if (aSelKey.GetSize() == 0) 
				{
					GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_No_Wall_is_selected__));
					return;
				}
				BOOL bRet = pDoc->m_pAttrCtrl->MakeRootWallElemKeyList(aSelKey);	//Root Elem 들만 aElemKeyList에 저장한다.
				if(!bRet)
				{
					GSaveHistoryFormatNF(_LS(IDS_DB_POSTCTRL_____MakePostData_WALL_STORYERROR)); // _T("층정보가 없어 할당이 되지 않습니다.");
					return;
				}

				CStoryData* pStoryData = pDoc->GetStoryData();

				for(int i=0; i<aSelKey.GetSize(); i++)
				{
					CArray<T_ELEM_K,T_ELEM_K> SortedWallIdElemList;
					CArray<T_ELEM_K,T_ELEM_K> ConnectedLinearElemList;
					CArray<T_ELEM_K,T_ELEM_K> CoordiLinearElemList;

					pDoc->m_pAttrCtrl->GetEqualWallIdElemList(SortedWallIdElemList,aSelKey[i], pStoryData);
					double dCompareLevel = -1;
					for(int j =0; j<SortedWallIdElemList.GetSize(); j++)
					{
						T_STOR_K StorK;
						T_STOR_D StorD;
						pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK,SortedWallIdElemList[j]);
						pDoc->m_pAttrCtrl->GetStor(StorK,StorD);
						if(dCompareLevel == -1)
							dCompareLevel = StorD.dStoryLevel;
						else if (dCompareLevel != StorD.dStoryLevel)
						{
							//error = 같은 층, 같은 Wall ID로 정렬 한 결과 중, 층 Level 이 다를 경우
							pDoc->DisplayHistoryMessage(_LS(IDS_DB_PO_PHGE_WARN_Cannot_Assign_DiffLevel));
							return;
						}
					}
					pDoc->m_pAttrCtrl->GetConnectedLinearElem(ConnectedLinearElemList,aSelKey[i],SortedWallIdElemList);
					if(ConnectedLinearElemList.GetSize() != SortedWallIdElemList.GetSize())
					{
						//error = 연결이 안되어 있는 경우 (중간중간이 빌 경우)
						pDoc->DisplayHistoryMessage(_LS(IDS_DB_PO_PHGE_WARN_Cannot_Assign_Connected));
						return;
					}
					pDoc->m_pAttrCtrl->GetEqualCoordLinearElem(ConnectedLinearElemList, CoordiLinearElemList);
					if(CoordiLinearElemList.GetSize() != ConnectedLinearElemList.GetSize())
					{
						//error = 연결은 되어있는지 꺾여서 연결되는 경우
						pDoc->DisplayHistoryMessage(_LS(IDS_DB_PO_PHGE_WARN_Cannot_Assign_Coordi));
						return;
					}
				}
			}
			break;
	}

	// 힌지 입력된 요소만 필터
	for(int i=aSelKey.GetSize()-1; i>=0; i--)
	{
		if(!pDoc->m_pAttrCtrl->ExistIehgElem(aSelKey[i]))
			aSelKey.RemoveAt(i);
	}

	T_SIHO_D SihoD;
	SihoD.bFiberResult = m_bFiberSectionResult;
	if(pDoc->m_pDataCtrl->AddSiho(aSelKey, SihoD))
		pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CCMSihoElemDlg::OnDeleteBtn() 
{
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	GetSelectedElemKeyList(aSelKey);
	if (aSelKey.GetSize() == 0) 
	{
		GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_No_element_is_selected_));  // No element is selected
		return;
	}

	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	// 힌지 입력된 요소만 필터
	for(int i=aSelKey.GetSize()-1; i>=0; i--)
	{
		if(!pDoc->m_pAttrCtrl->ExistIehgElem(aSelKey[i]))
			aSelKey.RemoveAt(i);
	}

	if(pDoc->m_pDataCtrl->DelSiho(aSelKey))
		pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CCMSihoElemDlg::OnClose() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);		
}

void CCMSihoElemDlg::GetElementByKind(CArray<UINT,UINT>& aKey, int nElemType)
{
	CQueryCtrl QueryCtrl;
	CString strElement;
	strElement.Format(_T("%ld"),nElemType);
	CArray<UINT,UINT> ElemKeys;
	CArray<UINT,UINT> SelKeys;
	SelKeys.Copy(aKey);
	aKey.RemoveAll();

	CQueryCondition QCond;          // 요소의 종류에 따른 쿼리 조건을 만든다.
	QCond.m_nCategory = CAT_ELEM_TYPE;
	QCond.m_nOperation = QC_OP_AND;
	QCond.m_aValue.Add(strElement);

	QueryCtrl.SelectKeyFromElem(ElemKeys,QCond);
	QueryCtrl.IntersectKey(ElemKeys,SelKeys,aKey);
}	

void CCMSihoElemDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
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
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		break;
	}
}

void CCMSihoElemDlg::UpdateBuffer()
{
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	CViewBuff* pViewBuff = pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bSiho = FALSE;
	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_SIHO_ADD): case(UR_SIHO_DEL):
			bSiho = TRUE;
			break;
		default:
			break;
		}
	} // end of while

	if(bSiho)
		MakeListItem();
}

void CCMSihoElemDlg::InitList()
{
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	int size[] = { globalUtils.ScaleByDPI(55), globalUtils.ScaleByDPI(100), globalUtils.ScaleByDPI(40) };
	m_list.InsertColumn(IDX_ELEM , _LS(IDS_CMSihoElemDlg_Element),          LVCFMT_CENTER, size[0]);  // _T("Element")
	m_list.InsertColumn(IDX_HINGE, _LS(IDS_CMSihoElemDlg_Hinge_Properties), LVCFMT_CENTER, size[1]); // _T("Hinge Properties")
	m_list.InsertColumn(IDX_FIBER, _LS(IDS_CMSihoElemDlg_Fiber),            LVCFMT_CENTER, size[2]);    // _T("Fiber")
}

void CCMSihoElemDlg::MakeListItem()
{
	// 선택한 요소타입의 모든 요소 표시. 힌지설명 유무와 관계없이 선택 가능
	m_list.DeleteAllItems();
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	CArray<T_SIHO_K,T_SIHO_K> aSihoK;
	pDoc->m_pAttrCtrl->GetSihoKeyList(aSihoK);
	
	T_IEHG_D IehgD;
	T_IEHP_D IehpD;
	for(int i=0, nIdx=0; i<aSihoK.GetSize(); i++)
	{
		if(!pDoc->m_pAttrCtrl->GetIehgByElemK(aSihoK[i], IehgD))
		{
			ASSERT(0);
			continue;
		}
		pDoc->m_pAttrCtrl->GetIehp(IehgD.IehpKey, IehpD);

		CString strElem, strHinge, strFiber;
		switch(IehpD.nHingeType)
		{
		case D_IEHP_HTYP_BEAM_LUMP:
		case D_IEHP_HTYP_BEAM_DIST:
			strElem.Format(_T("B%d"), aSihoK[i]);
			break;
		case D_IEHP_HTYP_TRUSS:
			strElem.Format(_T("T%d"), aSihoK[i]);
			break;
		case D_IEHP_HTYP_WALL:
			strElem.Format(_T("W%d"), aSihoK[i]);
			break;
		case D_IEHP_HTYP_SPRING:
		case D_IEHP_HTYP_POINTSPRING:
			continue; // Pspr은 별도 대화상자로 입력
		default: ASSERT(0);
		}
		strHinge = IehpD.strName;

		T_SIHO_D SihoD;
		pDoc->m_pAttrCtrl->GetSiho(aSihoK[i], SihoD);
		strFiber = SihoD.bFiberResult ? _T("O") : _T("-");

		m_list.InsertItem(nIdx, strElem);
		m_list.SetItemText(nIdx, IDX_HINGE, strHinge);
		m_list.SetItemText(nIdx, IDX_FIBER, strFiber);
		nIdx++;
	}
}

void CCMSihoElemDlg::OnClickSelList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// 리스트 선택된 요소를 뷰에서 선택
	CArray<T_SIHO_K,T_SIHO_K> aKey;
	for(int i=0; i<m_list.GetItemCount(); i++)
	{
		if (m_list.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED)
		{
			CString strText = m_list.GetItemText(i,0);
			strText.Delete(0); // 앞에 한칸은 지운다
			aKey.Add(_ttoi(strText));
		}
	}
	CDBDoc::GetDocPoint()->m_pViewCtrl->SelectElem(NULL, aKey, FALSE, TRUE);

	*pResult = 0;
}

void CCMSihoElemDlg::OnElemTypeRdo()
{
	if(!m_bExistFiber)
		return;

	// GEN-1189 : Truss일 때는 비활성화
	UpdateData();
	BOOL bEnable = m_nElemType!=ET_TRUSS;
	if(!bEnable)
	{
		m_bFiberSectionResult = FALSE;
		UpdateData(FALSE);
	}
	GetDlgItem(IDC_CMD_FIBER_RESULT_CHK)->EnableWindow(m_nElemType!=ET_TRUSS); 
}
