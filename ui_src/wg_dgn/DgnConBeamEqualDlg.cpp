// DgnConBeamEqualDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConBeamEqualDlg.h"
#include "DgnDataCtrl.h"

//#include "..\wg_base\wg_base_NumericOptimizer.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_ArrayUtil.h"

#include "..\wg_base\TreeMenuMode.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_MathFunc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_MembCtrl.h"
#include "..\wg_db\wg_db_QSort.h"

// #include <math.h>

// #include "..\wg_main\wg_mainres2.h"	// For ID_DGN_GEN_CMFT

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamEqualDlg dialog
CDgnConBeamEqualDlg::CDgnConBeamEqualDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConBeamEqualDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConBeamEqualDlg)
	m_nOption = 0;
	m_nAssignType = 1;
	m_nSelectType = 1; 

	//}}AFX_DATA_INIT
	m_mapSelBeamKey.InitHashTable(HASHSIZEELEM);
	m_bSentMessage = FALSE;
	m_nFailureType = 0;
 	m_pDoc = CDBDoc::GetDocPoint();
}

CDgnConBeamEqualDlg::~CDgnConBeamEqualDlg()
{
}

void CDgnConBeamEqualDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConBeamEqualDlg)
	DDX_Radio  (pDX, IDC_DGN_BEAM_EQUAL_AR, m_nOption);
	DDX_Radio  (pDX, IDC_DGN_BEAM_EQUAL_ASSIGN_ALL, m_nAssignType);
	DDX_Radio  (pDX, IDC_DGN_BEAM_EQUAL_SELECT_ALL, m_nSelectType); 
	DDX_Control(pDX, IDC_DGN_BEAM_EQUAL_LIST, m_List);
	DDX_Control(pDX, IDC_DGN_BEAM_EQUAL_ASSIGN_NOTCONSIDER, m_ChkNotConsider);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConBeamEqualDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConBeamEqualDlg)
	ON_BN_CLICKED(IDC_DGN_BEAM_EQUAL_AR, OnDgnOption)
	ON_BN_CLICKED(IDC_DGN_BEAM_EQUAL_DEL, OnDgnOption)
	ON_BN_CLICKED(IDC_DGN_BEAM_EQUAL_SELECT_ALL, OnDgnMemberSelect)
	ON_BN_CLICKED(IDC_DGN_BEAM_EQUAL_SELECT_SEL, OnDgnMemberSelect)  
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_BEAM_EQUAL_LIST, OnItemchanged)	
	ON_BN_CLICKED(IDC_DGN_BEAM_EQUAL_ASSIGN_NOTCONSIDER, OnDgnNotConsider)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamEqualDlg message handlers
BOOL CDgnConBeamEqualDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	m_aCtrlAll.Add(IDC_DGN_BEAM_EQUAL_ASSIGN_ALL);
	m_aCtrlAll.Add(IDC_DGN_BEAM_EQUAL_ASSIGN_LINE);
	m_aCtrlAll.Add(IDC_DGN_BEAM_EQUAL_SELECT_ALL);
	m_aCtrlAll.Add(IDC_DGN_BEAM_EQUAL_SELECT_SEL);

	BOOL bDonotConsider=FALSE;
	CArray<T_BREQ_K, T_BREQ_K> aBreqK;
	m_pDoc->m_pAttrCtrl2->GetBreqKeyList(aBreqK);
	if(aBreqK.GetSize() > 0)
	{
		T_BREQ_D BreqD;
		if(m_pDoc->m_pAttrCtrl2->GetBreq(aBreqK[0], BreqD))
		{
			bDonotConsider = BreqD.bDonotConsider;
		}
	}

	CheckDlgButton(IDC_DGN_BEAM_EQUAL_ASSIGN_NOTCONSIDER, bDonotConsider);
	SetListCtrlHeader();
	MakeItemEx();
	
	CtrlManager();  
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConBeamEqualDlg::OnDgnOption() 
{
	CtrlManager();
}

void CDgnConBeamEqualDlg::OnDgnAssignType() 
{
	CtrlManager();
}

void CDgnConBeamEqualDlg::OnDgnMemberSelect()
{
	DisplaySelElemList();   // 선택된 요소를 Display한다.
}

//  CButton* AllBtn = (CButton*)GetDlgItem(IDC_DGN_BEAM_EQUAL_SELECT_ALL);  
//  AllBtn->SetCheck(FALSE);

void CDgnConBeamEqualDlg::CtrlManager() 
{
	UpdateData(TRUE);    

	BOOL bDonotConsider=FALSE;
	if(m_ChkNotConsider.GetCheck()) bDonotConsider=TRUE;  //체크박스의 상태 값을 얻어옴.

	if(bDonotConsider)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlAll, FALSE);  
		GetDlgItem(IDC_DGN_BEAM_EQUAL_LIST)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DGN_BEAM_EQUAL_LIST)->EnableWindow(TRUE);
		if(m_nOption == 0)
		{
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlAll, TRUE);
		}
		else if(m_nOption == 1)
		{
			CDlgUtil::CtrlEnableDisable(this, m_aCtrlAll, FALSE);    
		}
	}
	
	UpdateData(FALSE);
}

void CDgnConBeamEqualDlg::OnDgnClose() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnConBeamEqualDlg::OnDgnExecute() 
{
	UpdateData(TRUE);	

	// 1. 우선 DonotConsider에 대한 처리를 한다..
	CArray<T_BREQ_K, T_BREQ_K> aKeyOld;
	CArray<T_BREQ_K, T_BREQ_K> aKeyNew;
	CArray<T_BREQ_D, T_BREQ_D&> aDataNew;
	
	BOOL bDonotConsider=FALSE;;
	if(m_ChkNotConsider.GetCheck()==BST_CHECKED)  bDonotConsider=TRUE; //체크박스의 상태 값을 얻어옴.

	CArray<T_BREQ_K, T_BREQ_K> aBreqK;
	m_pDoc->m_pAttrCtrl2->GetBreqKeyList(aBreqK);
	
	int nBreqCount = aBreqK.GetSize();
	for (int j=0; j<nBreqCount; j++)
	{
		T_BREQ_D BreqD;
		if(!m_pDoc->m_pAttrCtrl2->GetBreq(aBreqK[j], BreqD)) { ASSERT(0); continue; }

		if(BreqD.bDonotConsider != bDonotConsider)
		{
			BreqD.bDonotConsider = bDonotConsider;

			aKeyOld.Add(aBreqK[j]);
			aKeyNew.Add(aBreqK[j]);
			aDataNew.Add(BreqD);
		}
	}

	if(aDataNew.GetSize() > 0)
	{
		if(!m_pDoc->m_pDataCtrl->ModifyBreq(aKeyOld, aKeyNew, aDataNew)) { ASSERT(0); return; }
	}

	if(bDonotConsider) return;


	// 2. DonotConsider가 적용되지 않았으면 선택된 member에 의해서 BREQ 데이터를 만들어 줌..
	CArray<T_ELEM_K,T_ELEM_K> aSelElemKey;   // Select된 Element List
	
	if(m_nOption == 0) // Add/Replace
	{
		if(m_nSelectType == 0 /*&& m_nAssignType == 1*/)  // Selection Type : All , Assign Type : Automatic
		{
			m_pDoc->m_pAttrCtrl->GetElemKeyList(aSelElemKey);           
		}  
		else if(m_nSelectType == 1) // Selection Type : By Selection
		{
			I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();      
	    pIGM->GetSelectedElemKeyListForDgn(aSelElemKey);
		}  
		else ASSERT(0);  
	}
	else if(m_nOption == 1)  // Delete  : Delete인 경우에는 Selected element만 지울 수 있도록 한다.
	{
		I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	  pIGM->GetSelectedElemKeyListForDgn(aSelElemKey);
	}

	if(aSelElemKey.GetSize() <= 0) 
	{
		GSaveHistoryFormatNF(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK); return; // Error : No selected element!
	}

	T_ELEM_K KeyElem;
	T_ELEM_D DataElem;  
	m_aSelBeamKey.RemoveAll();
	m_aNonBeamKey.RemoveAll();
	m_aDcbbBeamKey.RemoveAll();
	m_mapSelBeamKey.RemoveAll();

	CDgnDataCtrl DataCtrl;
	for(int i = 0; i < aSelElemKey.GetSize(); i++)  // 선택된 element중에서 frame type인 element만 모은다.
	{
		KeyElem = aSelElemKey.GetAt(i);		
		DataElem.Initialize();
		if(m_pDoc->m_pAttrCtrl->GetElem(KeyElem, DataElem))
		{
			T_MATL_D MatlD;
			if(!m_pDoc->m_pAttrCtrl->GetMatl(DataElem.elmat, MatlD)) { ASSERT(0); continue; }

			// MNET:XXXX-JWKWON-20140311 : Design Criteria for Rebars by Member가 설정된 부재는 제외
			if(m_pDoc->m_pAttrCtrl->ExistDcbb(KeyElem)) { m_aDcbbBeamKey.Add(KeyElem); continue; }

			// MNET:XXXX-JWKWON-20140305// MNET:XXXX-JWKWON-20140305
			// Element Type이 General Beam이고 Member Type이 Beam이고 Material이 Concrete여야 함..
			if(m_pDoc->m_pAttrCtrl->IsBeam(DataElem.eltyp) && DataCtrl.Get_DgnGenMbtp(KeyElem) == D_MBTP_BEAM &&
				 MatlD.Type == _T("C") /*&& !m_pDoc->m_pAttrCtrl->ExistMembAssigned(KeyElem)*/) 
			{
				m_aSelBeamKey.Add(KeyElem); // Beam
				m_mapSelBeamKey.SetAt(KeyElem, KeyElem);
			}
			else m_aNonBeamKey.Add(KeyElem);   // Beam 이외의 요소 리스트
		}
	}

	MemberDefine();
}

void CDgnConBeamEqualDlg::MemberDefine()
{   
	int nNumSelFrmKey = m_aSelBeamKey.GetSize();
	int nNumNonFrmKey = m_aNonBeamKey.GetSize();
	int nNumDcbbBeamkey = m_aDcbbBeamKey.GetSize();

	if(m_nOption == 0)  // Add/Replace
	{
		if(nNumSelFrmKey == 0)      GSaveHistoryFormatNF(_LS(IDS_DGN_ERROR_CON_BEAM_DESIGNATE_ONE)); // [오류] Frame Type 요소만 Member 지정이 가능합니다.
		else if(nNumSelFrmKey == 1) GSaveHistoryFormatNF(_LS(IDS_DGN_ERROR_CON_BEAM_DESIGNATE_TWO)); // [오류] Member 지정에는 두 개 이상의 Frame Type 요소가 필요합니다.
		else if(nNumSelFrmKey >= 1) EqualizeBeamRebarDefineAll();
		else ASSERT(0);

		if(nNumNonFrmKey > 0 && m_nSelectType == 1) // Selection Type : By Selection 인 경우에만 아래 메시지 출력
		{
			for(int i = 0; i < nNumNonFrmKey; i++)
				GSaveHistoryFormatNF(_LS(IDS_DGN_ERROR_CON_BEAM_DESIGNATE_THREE), m_aNonBeamKey.GetAt(i));  // 요소 %d은(는) Frame Type이 아니므로 Member를 지정할 수 없습니다.
		}

		if(nNumDcbbBeamkey > 0 && m_nSelectType == 1) // Selection Type : By Selection 인 경우에만 아래 메시지 출력
		{
			for(int i = 0; i < nNumDcbbBeamkey; i++)
				GSaveHistoryFormatNF(_LS(IDS_DGN_ERROR_CON_BEAM_DESIGNATE_FOUR), m_aDcbbBeamKey.GetAt(i));  // 요소 %d은(는) Frame Type이 아니므로 Member를 지정할 수 없습니다.
		}

	}

	else if(m_nOption == 1)	// Delete : 요소가 Member의 구성요소 일때 그 요소가 속한 Member를 삭제한다.
	{
		if(nNumSelFrmKey == 0)      GSaveHistoryFormatNF(_LS(IDS_DGN_ERROR_CON_BEAM_DESIGNATE_FIVE)); // Frame Type 요소만 Member 지정 및 해제가 가능합니다.
		else if(nNumSelFrmKey >= 1) DelAssignedMember();
		else ASSERT(0);
	}

//  MakeItemEx();
}

// // 절점을 공유하는 Beam을 모두 동일 배근으로 지정..
// Manual의 Add/Replace일 때 쓰임
void CDgnConBeamEqualDlg::EqualizeBeamRebarDefineAll()
{
	T_BREQ_D DataBreq;
	T_ELEM_K KeyElem;
	T_ELEM_D DataElem;
	CArray<T_ELEM_K, T_ELEM_K> aConnectedElem;
	CArray<T_BREQ_D, T_BREQ_D&> aDataBreq;         // 최종적으로 선택된 멤버의 리스트
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapUsedElem;  // 한 번이라도 Breqer 지정에 사용된 요소의 map  
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K> mapUsedNode;  // Breqer의 Element List에 속하는 요소들의 절점의 map
	mapUsedElem.InitHashTable(HASHSIZEELEM);  
	mapUsedNode.InitHashTable(HASHSIZENODE);
	mapUsedElem.RemoveAll();
	aDataBreq.RemoveAll();

	DataBreq.Initialize();
	DataElem.Initialize();

	
	int nNumSelBeamKey = m_aSelBeamKey.GetSize();	 

	CDgnDataCtrl DataCtrl;
	for (int i=0; i<nNumSelBeamKey; i++)
	{
		DataBreq.Initialize();
		KeyElem = m_aSelBeamKey.GetAt(i);
		if(!m_pDoc->m_pAttrCtrl->GetElem(KeyElem, DataElem)) { ASSERT(0); continue; }

		mapUsedNode.SetAt(DataElem.elnod[0], DataElem.elnod[0]);    // 기준 요소의 i노드를 사용된 노드 map에 추가한다.
		mapUsedNode.SetAt(DataElem.elnod[1], DataElem.elnod[1]);    // 기준 요소의 j노드를 사용된 노드 map에 추가한다.
	}

	BOOL bDonotConsider=FALSE;;
	if(m_ChkNotConsider.GetCheck()==BST_CHECKED)  bDonotConsider=TRUE; //체크박스의 상태 값을 얻어옴.

	T_NODE_K NodeK, nTemp;
	POSITION pos = mapUsedNode.GetStartPosition();
	while(pos)
	{
		aConnectedElem.RemoveAll();
		mapUsedNode.GetNextAssoc(pos, NodeK, nTemp);

		m_pDoc->m_pAttrCtrl->GetElemKeyListConnectedToNode(NodeK, aConnectedElem);

		CArray<T_ELEM_K, T_ELEM_K> aSameBeamKey;
		CArrayUtil::IntersectUINT(m_aSelBeamKey, aConnectedElem, aSameBeamKey);
		if(aSameBeamKey.GetSize() < 2) continue;

		// MNET:XXXX-JWKWON-20140516 : 연결된 기둥 부재가 있어야 양단부를 같은 배근으로 하는 부분은 삭제함(MQC-AlphaBeat 4830-8)

//     if(!IsExistColumn(aConnectedElem)) 
//     {
//       CString strMembList, csElem;
//       for(int j=0; j < aSameBeamKey.GetSize() - 1; j++)
//       {
//         csElem.Format(_T("%d"), aSameBeamKey[j]);
//         strMembList += csElem + _T(", ");
//       }
//       if(aSameBeamKey.GetSize() > 0)
//       {
//         csElem.Format(_T("%d"), aSameBeamKey[j]);
//         strMembList += csElem;
//       }
//       GSaveHistoryFormatNF(_LS(IDS_DGN_ERROR_CON_BEAM_DESIGNATE_SIX), strMembList, NodeK); 
//       continue;
//     }


		mapUsedElem.RemoveAll();
		T_ELEM_K nTemp = 0;
		for(int k=0; k<aSameBeamKey.GetSize(); k++)
		{
			T_ELEM_K KeyElem_src = aSameBeamKey.GetAt(k);
			if(mapUsedElem.Lookup(KeyElem_src, nTemp)) continue;

			DataBreq.Initialize();
			DataBreq.NodeK = NodeK;
			DataBreq.aElemList.Add(KeyElem_src);

			mapUsedElem.SetAt(KeyElem_src, KeyElem_src);
			for (int l=0; l<aSameBeamKey.GetSize(); l++)
			{
				T_ELEM_K KeyElem_tgt = aSameBeamKey.GetAt(l);
				if(mapUsedElem.Lookup(KeyElem_tgt, nTemp)) continue;

				if(!IsMatchDirectionAttribute(KeyElem_src, KeyElem_tgt)) 
				{
					if      (m_nFailureType == 0) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_BREQ_SINGLE_FAIL)); // _T("선택된 요소중 Member 지정에 성공한 요소가 없습니다.") 
					else if (m_nFailureType == 1) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_BREQ_SINGLE_FAIL_ELEMTYPE)); 
					else if (m_nFailureType == 2) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_BREQ_SINGLE_FAIL_ELEMMATL)); 
					else if (m_nFailureType == 3) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_BREQ_SINGLE_FAIL_ELEMPROP)); 
					else if (m_nFailureType == 4) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_BREQ_SINGLE_FAIL_TAPERSECT)); 
					else if (m_nFailureType == 5) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_BREQ_SINGLE_FAIL_BETAANG)); 
					else if (m_nFailureType == 6) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_BREQ_SINGLE_FAIL_MEMBTYPE)); 
					else if (m_nFailureType == 7) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_BREQ_STRAIGHT_LINE)); 

					continue;
				}

				DataBreq.aElemList.Add(KeyElem_tgt);
				mapUsedElem.SetAt(KeyElem_tgt, KeyElem_tgt);
			}


			if(DataBreq.aElemList.GetSize() > 1)
			{
				// Member Assignment가 적용된 부재가 있는 경우는 해당 Member를 하나의 요소로 판단해서 적용 
				// MNET:XXXX-JWKWON-20140517 - MQC Alpha/Beta 4830-8
				CArray<T_ELEM_K, T_ELEM_K> aRemoveKey;

				int nElemCount = DataBreq.aElemList.GetSize();
				for (int i=0; i<nElemCount; i++)
				{
					T_MEMB_K MembK;
					T_MEMB_D MembD;
					if(!m_pDoc->m_pAttrCtrl->GetMembAssigned(DataBreq.aElemList[i], MembK)) continue;
					if(!m_pDoc->m_pAttrCtrl->GetMemb(MembK, MembD))               continue;

					CArray<T_ELEM_K, T_ELEM_K> aCommonKey;
					CArrayUtil::IntersectUINT(DataBreq.aElemList, MembD.aElemList, aCommonKey);

					if(aCommonKey.GetSize() > 1) aRemoveKey.Append(aCommonKey);
				}

				if(aRemoveKey.GetSize() > 0)
				{
					CArrayUtil::RemoveDup(aRemoveKey);

					CArray<T_ELEM_K, T_ELEM_K> aRemainKey;
					CArrayUtil::SubtractUINT(DataBreq.aElemList, aRemoveKey, aRemainKey);

					DataBreq.aElemList.RemoveAll();
					if(aRemainKey.GetSize() < 1) continue;
					DataBreq.aElemList.Copy(aRemainKey);
				}

				DataBreq.bDonotConsider = bDonotConsider;
				if(DataBreq.aElemList.GetSize() > 1) aDataBreq.Add(DataBreq);
			}
		}
	}
	
	if(aDataBreq.GetSize() > 0)
	{
		if(!m_pDoc->m_pDataCtrl->AddBreq(aDataBreq)) { ASSERT(0); }
	}
}


// Frame type의 element list로부터 이들이 속한 member를 삭제하는 함수
// Manual 및 Automatic의 Delete일 때 쓰임
void CDgnConBeamEqualDlg::DelAssignedMember()
{
	T_BREQ_K KeyBreq;
	T_BREQ_D DataBreq;
	CArray<T_BREQ_K, T_BREQ_K> aKeyBreq;
	CMap<T_BREQ_K, T_BREQ_K, T_BREQ_K, T_BREQ_K> mapKeyBreq;
	mapKeyBreq.InitHashTable(HASHSIZEBREQ);  
	mapKeyBreq.RemoveAll();
	aKeyBreq.RemoveAll();    
	int nNumSelFrmKey = m_aSelBeamKey.GetSize();	
	for(int i = 0; i < nNumSelFrmKey; i++)
	{
		if(m_pDoc->m_pAttrCtrl2->GetBreqAssigned(m_aSelBeamKey.GetAt(i), aKeyBreq))
		{ // 선택된 요소중 멤버가 할당된 요소를 map에 모은다. 
			for (int j=0; j<aKeyBreq.GetSize(); j++)
			{
				if(m_pDoc->m_pAttrCtrl2->ExistBreq(aKeyBreq[j])) mapKeyBreq.SetAt(aKeyBreq[j], aKeyBreq[j]);
			}
			
		}
	}
	
	aKeyBreq.RemoveAll();    
	POSITION pos = mapKeyBreq.GetStartPosition();
	while(pos)
	{
		mapKeyBreq.GetNextAssoc(pos, KeyBreq, KeyBreq);
		aKeyBreq.Add(KeyBreq);
	}

	CArray<int, int> aSelItem;
	aSelItem.RemoveAll();
	int nDelBreqCount = aKeyBreq.GetSize();
	if(nDelBreqCount == 0) GSaveHistoryFormatNF(_LS(IDS_DGN_ERROR_CON_BEAM_DESIGNATE_SEVEN));  // 선택된 요소 중에서 Breqer의 구성요소로 지정된 요소가 없습니다. Among selected elements, no element is used as a constituent of breqer
	else if(nDelBreqCount >= 1)
	{    
		GetSelectedItemList(&m_List, aSelItem);  // Breqer 삭제후 리스트의 포커스 재설정을 위해 List박스의 선택된 리스트를 모은다.

		if(m_pDoc->m_pDataCtrl->DelBreq(aKeyBreq)) InitSelectedItem();
	}

	if(aSelItem.GetSize() > 0) SetListFocus(aSelItem);  // Breqer 삭제후 리스트의 포커스 설정
}



void CDgnConBeamEqualDlg::InitSelectedItem() // 선택된 요소와 절점을 해제한다.
{  
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CDgnConBeamEqualDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;
	
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
		case D_UPDATE_SEL_ADD: 
		case D_UPDATE_SEL_DEL:
			break;
		default: ASSERT(TRUE);
	}
}

void CDgnConBeamEqualDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_BREQ_K Key;
	T_BREQ_D Data;

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_BREQ_ADD):
			{
				pViewBuff->GetBreq(nKey, Key, Data);
				InsertItem(Key);
			}
			break;
		case(UR_BREQ_DEL):
			{
				pViewBuff->GetBreq(nKey, Key, Data);
				DeleteItem(Key);
			}
			break;
		case(UR_BREQ_MFS):
		case(UR_BREQ_MFD):
			{
				pViewBuff->GetBreq(nKey, Key, Data);
				DeleteItem(Key);
				InsertItem(Key);        
			}
			break;
		default:
			break;
		}
	} 
}

void CDgnConBeamEqualDlg::InsertItem(T_BREQ_K Key)
{
	// 삽입 위치를 찾는다.
	T_BREQ_D Data;
	int nSize = m_List.GetItemCount();
	if (!m_pDoc->m_pAttrCtrl2->GetBreq(Key, Data)) return;
	CString strBreqIndex;     
	int nNodeKey;  
    int nCount = 0;
	for(nCount = 0; nCount < nSize; nCount++)
	{
		strBreqIndex = m_List.GetItemText(nCount, 0);
		nNodeKey = _ttoi(strBreqIndex);
		if(nNodeKey > Data.NodeK) break;
	}  
	strBreqIndex.Format(_T("%d"), Data.NodeK);
	int nIndex = m_List.InsertItem(nCount, strBreqIndex);
	CString csItem;
	CString strElemList = _T("");

    int i = 0;
	for(i = 0; i < Data.aElemList.GetSize() - 1; i++)
	{
		BOOL bMemberAssign=FALSE;
		if(m_pDoc->m_pAttrCtrl->ExistMembAssigned(Data.aElemList[i])) bMemberAssign = TRUE;
		
		if(bMemberAssign) csItem.Format(_T("%d*"), Data.aElemList[i]);
		else csItem.Format(_T("%d"), Data.aElemList[i]);
		strElemList += csItem + _T(", ");
	}
	if(Data.aElemList.GetSize() > 0)
	{
		BOOL bMemberAssign=FALSE;
		if(m_pDoc->m_pAttrCtrl->ExistMembAssigned(Data.aElemList[i])) bMemberAssign = TRUE;

		if(bMemberAssign) csItem.Format(_T("%d*"), Data.aElemList[i]);
		else csItem.Format(_T("%d"), Data.aElemList[i]);
		strElemList += csItem;
	}
	m_List.SetItemText(nIndex, 1, strElemList);      
	m_List.SetItemData(nIndex, (DWORD)Key);
}

void CDgnConBeamEqualDlg::DeleteItem(T_BREQ_K Key)
{
	int nSize = m_List.GetItemCount();
    int nCount = 0;
	for(nCount = 0; nCount < nSize; nCount++)
	{
		if(m_List.GetItemData(nCount) == Key) break;
	}  
	if(nCount == nSize) return; //GSaveHistoryFormatNF(_T("Error : Member not found!"));
	else m_List.DeleteItem(nCount);  
}


// 아래층에 기둥이나 벽체가 있는지 확인하는 함수
BOOL CDgnConBeamEqualDlg::IsExistColumn(CArray<T_ELEM_K, T_ELEM_K>& aConnectedElem)
{
	T_ELEM_K KeyElem;
	T_ELEM_D DataElem;

	CDgnDataCtrl DataCtrl;
	for(int i = 0; i < aConnectedElem.GetSize(); i++)
	{
		KeyElem = aConnectedElem.GetAt(i);
		DataElem.Initialize();
				
		if(m_pDoc->m_pAttrCtrl->GetElem(KeyElem, DataElem))
		{
			if(m_pDoc->m_pAttrCtrl->IsFrameType(DataElem.eltyp))
			{
				if(DataCtrl.Get_DgnGenMbtp(KeyElem) == D_MBTP_COLUMN) return TRUE;  // 아래층에 기둥이 있으면
			}
		}
	}
	return FALSE;
}


BOOL CDgnConBeamEqualDlg::IsMatchDirectionAttribute(T_ELEM_K KeyRef, T_ELEM_K KeyNew)
{  
	if(m_nAssignType == 1 && !CompareElemDirection(KeyRef, KeyNew)) { m_nFailureType = 7; return FALSE;  }// Automatic인 경우만 요소의 직선여부를 체크한다. 
	if(!CompareElemAttribute(KeyRef, KeyNew)) return FALSE;
	return TRUE;
}

BOOL CDgnConBeamEqualDlg::CompareElemDirection(T_ELEM_K KeyRef, T_ELEM_K KeyNew)
{
	T_ELEM_D DataElem;    
	double ux1, uy1, uz1, ux2, uy2, uz2;  // directional vector of element

	if(m_pDoc->m_pAttrCtrl->GetElem(KeyRef, DataElem)) GetFrmDirectionVector(DataElem, ux1, uy1, uz1);
	else return FALSE;
	if(m_pDoc->m_pAttrCtrl->GetElem(KeyNew, DataElem)) GetFrmDirectionVector(DataElem, ux2, uy2, uz2);	  
	else return FALSE;
	
	double Vec1[3], Vec2[3]; 
	Vec1[0] = ux1, Vec1[1] = uy1, Vec1[2] = uz1;
	Vec2[0] = ux2, Vec2[1] = uy2, Vec2[2] = uz2;
	double dAngle = CMathFunc::mathCrossAngle(Vec1, Vec2);
	double dTol = 0.001;

	switch(m_nAssignType)
	{
		case 0:  // Manual : 직선상에 위치하지 않아도 허용한다.(각도 고려하지 않음)
//      if(fabs(dAngle) < 90 - dTol) return TRUE;
//      else return FALSE;
			return TRUE;
			break;
		case 1:  // Automatic : 직선상에 위치할 때만 허용(단, 각도 1도 미만일때는 허용)
			if(fabs(dAngle) <= 1) return TRUE;
			else return FALSE;
			break;
		default:
			return FALSE;
			break;
	}  
}

BOOL CDgnConBeamEqualDlg::CompareElemAttribute(T_ELEM_K KeyRef, T_ELEM_K KeyNew)
{
	CDgnDataCtrl DataCtrl;
	T_ELEM_D DataElemRef, DataElemNew;
	T_SECT_D DataSectRef, DataSectNew;
	
	if(m_pDoc->m_pAttrCtrl->GetElem(KeyRef, DataElemRef) && m_pDoc->m_pAttrCtrl->GetElem(KeyNew, DataElemNew))
	{ 
		if(DataElemRef.eltyp != DataElemNew.eltyp) {m_nFailureType = 1; return FALSE;}// 타입
		if(DataElemRef.elmat != DataElemNew.elmat) {m_nFailureType = 2; return FALSE;}// 재질
//     if(DataElemRef.elpro != DataElemNew.elpro) {m_nFailureType = 3; return FALSE;}// 단면

		m_pDoc->m_pAttrCtrl->GetSect(DataElemRef.elpro, DataSectRef);
		m_pDoc->m_pAttrCtrl->GetSect(DataElemNew.elpro, DataSectNew); // Same Section 이지만, Tapered Sect인경우 
		if (DataSectRef.nStype==D_SECT_TYPE_TAPERED || DataSectNew.nStype==D_SECT_TYPE_TAPERED) {m_nFailureType = 4; return FALSE;}
		else if( (DataSectRef.nStype==D_SECT_TYPE_REGULAR && DataSectNew.nStype==D_SECT_TYPE_REGULAR) || (DataSectRef.nStype==D_SECT_TYPE_USER && DataSectNew.nStype==D_SECT_TYPE_USER) )
		{
			// Section이 Regular이거나 User 일 때는 Section Key가 다르더라도 같은 Section 이면 양단부 배근을 같이 할 수 있도록 함
			// MNET:XXXX-JWKWON-20140624 : 이해주 대리님 요청

			if(DataSectRef.SectBefore.nStype       != DataSectNew.SectBefore.nStype)        {m_nFailureType = 3; return FALSE;}
			if(DataSectRef.SectBefore.Shape        != DataSectNew.SectBefore.Shape)         {m_nFailureType = 3; return FALSE;}
			if(DataSectRef.SectBefore.nOffsetPoint != DataSectNew.SectBefore.nOffsetPoint)  {m_nFailureType = 3; return FALSE;}
			if(DataSectRef.SectBefore.SectI.Shape  != DataSectNew.SectBefore.SectI.Shape)   {m_nFailureType = 3; return FALSE;}
			for (int k=0; k<D_SECT_SIZE_NUM_MAX; k++)
			{
				if(fabs(DataSectRef.SectBefore.SectI.Size[k] - DataSectNew.SectBefore.SectI.Size[k]) >  1.0e-6) {m_nFailureType = 3; return FALSE;}
			}
		}
		else
		{
			if(DataElemRef.elpro != DataElemNew.elpro) {m_nFailureType = 3; return FALSE;}
		}
		
	
		if(fabs(DataElemRef.angle - DataElemNew.angle - (int)((DataElemRef.angle - DataElemNew.angle)/360.)*360.) > 1.0e-6 &&
			 fabs(DataElemRef.angle - (int)(DataElemRef.angle/360.)*360. - DataElemNew.angle + (int)(DataElemNew.angle/360.)*360.) > 1.0e-6) 
	  {m_nFailureType = 5; return FALSE;}// beta angle
		if(DataCtrl.Get_DgnGenMbtp(KeyRef) != DataCtrl.Get_DgnGenMbtp(KeyNew)) 
		{m_nFailureType = 6; return FALSE;}// member type
	}
	else return FALSE;
	return TRUE;
}

void CDgnConBeamEqualDlg::GetFrmDirectionVector(T_ELEM_D& DataElem, double& Ux, double& Uy, double& Uz)
{
	T_NODE_D DataNode;
	DataNode.Initialize();

	double Coor_i[3], Coor_j[3], Vec[3], NormVec[3];
	if(m_pDoc->m_pAttrCtrl->GetNodeFromBaseDB(DataElem.elnod[0], DataNode))
		{Coor_i[0]=DataNode.x;	  Coor_i[1]=DataNode.y;	  Coor_i[2]=DataNode.z;}
	if(m_pDoc->m_pAttrCtrl->GetNodeFromBaseDB(DataElem.elnod[1], DataNode))
		{Coor_j[0]=DataNode.x;	  Coor_j[1]=DataNode.y;	  Coor_j[2]=DataNode.z;}
	
	for(int i= 0; i < 3; i++) Vec[i] = (Coor_j[i] - Coor_i[i]);
	CMathFunc::mathNormalize(Vec, NormVec);
	Ux = NormVec[0];
	Uy = NormVec[1];
	Uz = NormVec[2];
}





//================================================================
//================= List 관련 작업 함수 ==========================
//================================================================
void CDgnConBeamEqualDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_DGN_CPG_NODE), _LS(IDS_DGN_CHK_ELEM_LIST)};  // _T("Index_T(", ")Element List")
	int nColWidth[] = {50, 180};
	int nAlign[] = {LVCFMT_CENTER, LVCFMT_LEFT};
	int nColNum = sizeof(nColWidth)/sizeof(int);
	CString title;  
	LV_COLUMN lvcolumn;
	
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	for(int i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = nAlign[i];

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CDgnConBeamEqualDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	CArray<T_BREQ_K, T_BREQ_K> aKeyList;
	m_pDoc->m_pAttrCtrl2->GetBreqKeyList(aKeyList);  //Key 번호로 정렬된 리스트

	int nBreqCount =  aKeyList.GetSize();
	if(nBreqCount == 0) return;

	CArray<int, int> aNodeList;
	aNodeList.SetSize(nBreqCount);

	T_BREQ_D BreqD;
	for (int i=0; i<nBreqCount; ++i)
	{
		if (!m_pDoc->m_pAttrCtrl2->GetBreq(aKeyList[i], BreqD))   { ASSERT(0); }
		aNodeList[i] = BreqD.NodeK;
	}

	CQSort::QSortIntWithIndex((int*)aKeyList.GetData(), aNodeList.GetData(), aNodeList.GetSize());
	/*CQSort::QSortIntWithIndex(aNodeList.GetData(), (int*)aKeyList.GetData(), aKeyList.GetSize());*/

	for(int j=0 ; j < nBreqCount ; j++)
	{
		CString strBreqIndex, strElemList;

		T_BREQ_K Key = aKeyList[j];
		m_pDoc->m_pAttrCtrl2->GetBreq(Key, BreqD);
		strBreqIndex.Format(_T("%d"), BreqD.NodeK);
		int nIndex = m_List.InsertItem(nBreqCount, strBreqIndex);      
		//CNumericOptimizer optimizer;
		//strElemList = optimizer.Optimize((long*)Data.aElemList.GetData(), Data.aElemList.GetSize());
		CString csItem;
		strElemList = _T("");
        int i = 0;
		for(i = 0; i < BreqD.aElemList.GetSize() - 1; i++)
		{
			csItem.Format(_T("%d"), BreqD.aElemList[i]);
			strElemList += csItem + _T(", ");
		}
		if(BreqD.aElemList.GetSize() > 0)
		{
			csItem.Format(_T("%d"), BreqD.aElemList[i]);
			strElemList += csItem;
		}
		m_List.SetItemText(nIndex, 1, strElemList);		
		m_List.SetItemData(nIndex, (DWORD)Key);
	}
}
			
void CDgnConBeamEqualDlg::OnDelKeyPressedInList()
{  
	CArray<T_BREQ_K, T_BREQ_K> aKeyBreq;
	CArray<int, int> aSelItem;
	aSelItem.RemoveAll();
	aKeyBreq.RemoveAll();  

	GetSelectedItemList(&m_List, aSelItem);
	int nSize = aSelItem.GetSize();
	if(nSize == 0) return;

	T_BREQ_K KeyBreq;
	for(int i = 0; i < nSize; i++)
	{
		KeyBreq = m_List.GetItemData(aSelItem[i]);    
		if(m_pDoc->m_pAttrCtrl2->ExistBreq(KeyBreq)) aKeyBreq.Add(KeyBreq);
	}  
	if(m_pDoc->m_pDataCtrl->DelBreq(aKeyBreq)) InitSelectedItem();

	SetListFocus(aSelItem);
}

// member를 삭제한 경우 List 박스의 Focus를 다시 맞춤
void CDgnConBeamEqualDlg::SetListFocus(CArray<int, int>& aSelItem)
{  
	int nSize = aSelItem.GetSize();
	if(nSize == 0) return;
	int nFocusItem = aSelItem[0];
	for(int i = 0; i < nSize; i++)
	{    
		if(aSelItem[i] < nFocusItem) nFocusItem = aSelItem[i];
	}
	int nCount = m_List.GetItemCount();
	if(nFocusItem >= nCount) nFocusItem -= 1;
	if(nFocusItem >= 0)
		m_List.SetItemState(nFocusItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

BOOL CDgnConBeamEqualDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_DELETE && GetFocus() == &m_List)
		{
			OnDelKeyPressedInList();
			return TRUE;
		}
	}
	return CMenuBarChildDlg::PreTranslateMessage(pMsg);
}

void CDgnConBeamEqualDlg::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;	 
	if (pNMListView->iSubItem != 999)
	{
		if (!m_bSentMessage)
		{
			m_TempListView = *pNMListView;
			m_TempListView.iSubItem = 999;
			PostMessage(WM_NOTIFY, IDC_DGN_MEMB_LIST, (LPARAM)&m_TempListView);
			m_bSentMessage = TRUE;
		}
		*pResult = 0;
		return;
	}
	DisplaySelElemList();  // 선택된 요소를 Display한다. 
	m_bSentMessage = FALSE;
	*pResult = 0;
}

//  CButton* SelBtn = (CButton*)GetDlgItem(IDC_DGN_MEMB_CHG_SELECT_SEL);  
void CDgnConBeamEqualDlg::DisplaySelElemList()
{
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelElem;  
	aSelElem.RemoveAll();
	GetSelElemList(aSelElem);  
	m_pDoc->m_pViewCtrl->SelectElem(NULL, aSelElem, FALSE, TRUE);
}

void CDgnConBeamEqualDlg::GetSelElemList(CArray<T_ELEM_K, T_ELEM_K>& aSelElem)
{
	T_BREQ_D DataBreq;
	CArray<int, int> aSelItem;
	CArray<T_BREQ_K, T_BREQ_K> aBreqKey;
	aBreqKey.RemoveAll();
	aSelItem.RemoveAll();
	aSelElem.RemoveAll();    

	GetSelectedItemList(&m_List, aSelItem);

	T_BREQ_K KeyBreq=0;
	int nSize = aSelItem.GetSize();
	if(nSize == 0) return;
	for(int i = 0; i < nSize; i++)
	{
		KeyBreq = m_List.GetItemData(aSelItem[i]);    
		aBreqKey.Add(KeyBreq);
	} 
	m_pDoc->m_pAttrCtrl2->GetKeyListBreqUser(&aBreqKey, 0, &aSelElem);
}      

void CDgnConBeamEqualDlg::GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem)
{
	int  nItem = -1;
	UINT uSelectedCount = pList->GetSelectedCount();
	aSelItem.SetSize(uSelectedCount);
	for(UINT i = 0; i < uSelectedCount; i++)
	{
		nItem = pList->GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		aSelItem[i] = nItem;
	}
}

void CDgnConBeamEqualDlg::OnDgnNotConsider()
{
	CtrlManager(); 
//   UpdateData(TRUE);    // m_ChkBox에 현재 체크박스의 상태 값이 저장됨. 체크 상태인지... 아닌지....
//   int iState;
//   iState = m_ChkNotConsider.GetCheck();  //체크박스의 상태 값을 얻어옴.
//   
//   switch(iState)
//   {
//   case BST_UNCHECKED:
//     GetDlgItem(IDC_DGN_BEAM_EQUAL_SELECT_ALL)->EnableWindow(TRUE);
//     GetDlgItem(IDC_DGN_BEAM_EQUAL_SELECT_SEL)->EnableWindow(TRUE);
//     
//     GetDlgItem(IDC_DGN_BEAM_EQUAL_ASSIGN_ALL)->EnableWindow(TRUE);
//     GetDlgItem(IDC_DGN_BEAM_EQUAL_ASSIGN_LINE)->EnableWindow(TRUE);
//     
//     GetDlgItem(IDC_DGN_BEAM_EQUAL_LIST)->EnableWindow(TRUE);
//     break;
//   case BST_CHECKED:
//     GetDlgItem(IDC_DGN_BEAM_EQUAL_SELECT_ALL)->EnableWindow(FALSE);
//     GetDlgItem(IDC_DGN_BEAM_EQUAL_SELECT_SEL)->EnableWindow(FALSE);
//     
//     GetDlgItem(IDC_DGN_BEAM_EQUAL_ASSIGN_ALL)->EnableWindow(FALSE);
//     GetDlgItem(IDC_DGN_BEAM_EQUAL_ASSIGN_LINE)->EnableWindow(FALSE);
//     
//     GetDlgItem(IDC_DGN_BEAM_EQUAL_LIST)->EnableWindow(FALSE);
//     break;
//   case BST_INDETERMINATE:
//     break;
//   }

}