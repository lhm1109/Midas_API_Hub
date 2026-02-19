// DgnPscSegmentDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnPscSegmentDlg.h"
#include "DgnDataCtrl.h"

//#include "..\wg_base\wg_base_NumericOptimizer.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\TreeMenuMode.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_MathFunc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_MembCtrl.h"

#include <math.h>

//#include "..\wg_main\wg_mainres2.h"	// For ID_DGN_GEN_CMFT

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define COLCOUNT 3
/////////////////////////////////////////////////////////////////////////////
// CDgnPscSegmentDlg dialog
CDgnPscSegmentDlg::CDgnPscSegmentDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnPscSegmentDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnPscSegmentDlg)
	m_nOption = 0;
	m_nAssignType = 0;
	m_nSelectType = 1; 
	m_nJointType = 0;
	//}}AFX_DATA_INIT
	m_mapSelFrmKey.InitHashTable(HASHSIZEELEM);
	m_bSentMessage = FALSE;
 	m_pDoc = CDBDoc::GetDocPoint();
}

CDgnPscSegmentDlg::~CDgnPscSegmentDlg()
{
}

void CDgnPscSegmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnPscSegmentDlg)
	DDX_Radio(pDX, IDC_DGN_SEGM_AR, m_nOption);  
	DDX_Control(pDX, IDC_DGN_SEGM_LIST, m_List);
	DDX_Control(pDX, IDC_DGN_SEGM_ALLOW_SINGLE_CHK, m_wndAllowSingleElemMember);
	DDX_Radio(pDX, IDC_DGN_SEGM_RESIN, m_nJointType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnPscSegmentDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnPscSegmentDlg)
	ON_BN_CLICKED(IDC_DGN_PSC_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_PSC_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_SEGM_AR, OnDgnOption)
	ON_BN_CLICKED(IDC_DGN_SEGM_DEL, OnDgnOption)
	
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_SEGM_LIST, OnItemchanged)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnPscSegmentDlg message handlers
BOOL CDgnPscSegmentDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	/*
	m_aCtrlAll.Add(IDC_DGN_MEMB_ASSIGN_MANUAL);
	m_aCtrlAll.Add(IDC_DGN_MEMB_ASSIGN_AUTO);
	m_aCtrlAll.Add(IDC_DGN_MEMB_SELECT_ALL);
	m_aCtrlAll.Add(IDC_DGN_MEMB_SELECT_SEL);
	m_aCtrlAll.Add(IDC_DGN_MEBM_ALLOW_SINGLE_CHK);
	*/
	GetDlgItem(IDC_DGN_PSC_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_PSC_CLOSE)->ShowWindow(SW_HIDE);

	m_wndAllowSingleElemMember.SetCheck(FALSE);

	SetListCtrlHeader();
	MakeItemEx();
	
	CtrlManager();  
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnPscSegmentDlg::OnDgnOption() 
{
	CtrlManager();
}

void CDgnPscSegmentDlg::OnDgnAssignType() 
{
	CtrlManager();
}

void CDgnPscSegmentDlg::OnDgnMemberSelect()
{
	DisplaySelElemList();   // 선택된 요소를 Display한다.
}

void CDgnPscSegmentDlg::CtrlManager() 
{
	/*
	UpdateData(TRUE);    
	if(m_nOption == 0)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlAll, TRUE);
		if(m_nAssignType == 0)   // Assign Type이 Manual인 경우 Selection Type의 All을 Disable시킨다.    
		{
			GetDlgItem(IDC_DGN_MEMB_SELECT_ALL)->EnableWindow(FALSE);
			if(m_nSelectType == 0) m_nSelectType = 1;
		}  
		else if(m_nAssignType == 1) 
		{
			GetDlgItem(IDC_DGN_MEMB_SELECT_ALL)->EnableWindow(TRUE);    
		}    
	}
	else if(m_nOption == 1)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlAll, FALSE);    
	}
	UpdateData(FALSE);
	*/
}

void CDgnPscSegmentDlg::OnDgnClose() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnPscSegmentDlg::OnDgnExecute() 
{
	UpdateData(TRUE);	
	CArray<T_ELEM_K,T_ELEM_K> aSelElemKey;   // Select된 Element List
	
	if(m_nOption == 0) // Add/Replace
	{
		I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();      
	  pIGM->GetSelectedElemKeyListForDgn(aSelElemKey);
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
	m_aSelFrmKey.RemoveAll();
	m_aNonFrmKey.RemoveAll();
	m_mapSelFrmKey.RemoveAll();

	for(int i = 0; i < aSelElemKey.GetSize(); i++)  // 선택된 element중에서 frame type인 element만 모은다.
	{
		KeyElem = aSelElemKey.GetAt(i);		
		DataElem.Initialize();
		if(m_pDoc->m_pAttrCtrl->GetElem(KeyElem, DataElem))
		{
			if(m_pDoc->m_pAttrCtrl->IsFrameType(DataElem.eltyp)) 
			{
				m_aSelFrmKey.Add(KeyElem); // Beam, Truss
				m_mapSelFrmKey.SetAt(KeyElem, KeyElem);
			}
			else m_aNonFrmKey.Add(KeyElem);   // Beam, Truss 이외의 요소 리스트
		}
	}

	MemberDefine();
}

void CDgnPscSegmentDlg::MemberDefine()
{   
	int nNumSelFrmKey = m_aSelFrmKey.GetSize();
	int nNumNonFrmKey = m_aNonFrmKey.GetSize();

	if(m_nOption == 0)  // Add/Replace
	{
		if(nNumSelFrmKey == 0)      GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_ELEM_TYPE)); // [오류] Frame Type 요소만 Segment 지정이 가능합니다.
//    else if(nNumSelFrmKey == 1) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_NUM_FRAME)); // [오류] Segment 지정에는 두 개 이상의 Frame Type 요소가 필요합니다.
		else if(nNumSelFrmKey >= 1)
		{ 
	    if(m_nAssignType == 0)      MemberDefineManual();
			else if(m_nAssignType == 1) MemberDefineAuto();  
		}
		else ASSERT(0);

		if(nNumNonFrmKey > 0 && nNumSelFrmKey > 0)
		{
			if(m_nSelectType == 1)  // Selection Type : By Selection 인 경우에만 아래 메시지 출력
			for(int i = 0; i < nNumNonFrmKey; i++)
				GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_NON_FRAME), m_aNonFrmKey.GetAt(i));  // 요소 %d은(는) Frame Type이 아니므로 Segment를 지정할 수 없습니다.
		}
	}
	else if(m_nOption == 1)	// Delete : 요소가 Segment의 구성요소 일때 그 요소가 속한 Segment를 삭제한다.
	{
		if(nNumSelFrmKey == 0)      GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_ONLY_FRAME)); // Frame Type 요소만 Segment 지정 및 해제가 가능합니다.
		else if(nNumSelFrmKey >= 1) DelAssignedMember();
		else ASSERT(0);
	}

//  MakeItemEx();
}

// Frame type의 element list로부터 member를 생성하는 함수
// Manual의 Add/Replace일 때 쓰임
void CDgnPscSegmentDlg::MemberDefineManual()
{
	T_PSEG_K KeyMemb;
	T_PSEG_D DataMemb;
	T_ELEM_K KeyElem, KeyTemp;
	T_ELEM_D DataElem, DElemNext;
	T_NODE_K KeyNode;
	CArray<T_ELEM_K, T_ELEM_K> aConnectedElem;
	CArray<T_PSEG_K, T_PSEG_K> aKeyMembDel;        // 최종적으로 삭제할 멤버의 리스트
	CArray<T_PSEG_D, T_PSEG_D&> aDataMemb;         // 최종적으로 선택된 멤버의 리스트
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapUsedElem;  // 한 번이라도 Segment 지정에 사용된 요소의 map  
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K> mapUsedNode;  // Segment의 Element List에 속하는 요소들의 절점의 map
	CMap<T_PSEG_K, T_PSEG_K, int, int> mapKeyMembDel; // 기존 요소가 멤버의 구성요소일 때 삭제할 멤버의 map  
	mapUsedElem.InitHashTable(HASHSIZEELEM);  
	mapKeyMembDel.InitHashTable(HASHSIZEMEMB);
	mapUsedNode.InitHashTable(53);
	mapUsedElem.RemoveAll();
	mapKeyMembDel.RemoveAll();  
	aKeyMembDel.RemoveAll();
	aDataMemb.RemoveAll();

	DataMemb.Initialize();
	DataElem.Initialize();
	DElemNext.Initialize();
	BOOL bAddMembFlag = FALSE; // 하나의 Segment정의가 끝났음을 알리는 flag  
	BOOL bNextInclude = FALSE; // 현재 요소의 절점에 연결된 요소가 동일한 Segment의 구성요소로 포함되었는지를 나타내는 flag  
	
	int nNumSelFrmKey = m_aSelFrmKey.GetSize();	 
	for(int i = 0; i < nNumSelFrmKey; i++) 
	{    
		DataMemb.Initialize();
		mapUsedNode.RemoveAll();   // 폐곡선을 막기 위해 쓰임
		KeyElem = m_aSelFrmKey.GetAt(i);
		if(!mapUsedElem.Lookup(KeyElem, KeyTemp)) // 기존에 한번이라도 Segment로 지정되지 않은 요소이면
			DataMemb.aElemList.Add(KeyElem);        // 현재 지정할 Segment의 요소 리스트에 추가한다.
		else continue; // 기존에 Segment로 지정된 요소이면 현재 요소에 대한 Segment 지정을 하지 않고 다음 요소에 대해 작업한다.

		if(m_pDoc->m_pAttrCtrl->GetElem(KeyElem, DataElem))
		{ 
			mapUsedNode.SetAt(DataElem.elnod[0], DataElem.elnod[0]);    // 기준 요소의 i노드를 사용된 노드 map에 추가한다.
			mapUsedNode.SetAt(DataElem.elnod[1], DataElem.elnod[1]);    // 기준 요소의 j노드를 사용된 노드 map에 추가한다.
			bNextInclude = TRUE;
			while(bNextInclude)  // j node 방향으로 연결된 요소에 대해 작업한다.
			{
				m_pDoc->m_pAttrCtrl->GetElemKeyListConnectedToNode(DataElem.elnod[1], aConnectedElem);
				for(int k = 0; k < aConnectedElem.GetSize(); k++)
				{
					bNextInclude = FALSE;  // 시작할때는 FALSE
					if(mapUsedElem.Lookup(aConnectedElem.GetAt(k), KeyTemp)) continue;   // 기존에 한번이라도 Segment로 지정되지 않고
					if(!m_mapSelFrmKey.Lookup(aConnectedElem.GetAt(k), KeyTemp)) continue;  // 현재 요소가 선택된 프레임 요소 리스트에 속하고
					if(aConnectedElem.GetAt(k) == KeyElem) continue;      // 현재 요소가 기준 요소와 다르고           
					if(!m_pDoc->m_pAttrCtrl->GetElem(aConnectedElem.GetAt(k), DElemNext)) continue;  // 현재 요소의 데이터가 정의되어 있고
					if(DataElem.elnod[1] != DElemNext.elnod[0]) continue; // 기준 요소와 현재 요소의 node i, j 순서가 동일하고          
					if(mapUsedNode.Lookup(DElemNext.elnod[1], KeyNode)) continue;  // 선택된 요소들이 폐곡선을 이루지 않아야 한다.  
					if(!IsMatchDirectionAttribute(KeyElem, aConnectedElem.GetAt(k))) continue; // 기준요소와 현재 요소가 동일한 속성이고
					
					DataMemb.aElemList.Add(aConnectedElem.GetAt(k));   // 위의 과정을 무사시 통과하면 Segment의 구성요소가 된다.
					mapUsedNode.SetAt(DElemNext.elnod[1], DElemNext.elnod[1]);    // 현재 요소의 j노드를 사용된 노드 map에 추가한다.
					KeyElem = aConnectedElem.GetAt(k);        // 현재 추가된 요소를 기준 요소로 설정
					DataElem.elnod[1] = DElemNext.elnod[1];   // 현재 추가된 요소의 j절점이 새로운 기준요소의 i절점이 된다.          
					bNextInclude = TRUE;  // 새로운 기준요소의 j절점에 대해 작업을 계속함을 의미
					break;
				}
			}

			bNextInclude = TRUE;
			while(bNextInclude)   // i node 방향으로 연결된 요소에 대해 작업한다.
			{
				m_pDoc->m_pAttrCtrl->GetElemKeyListConnectedToNode(DataElem.elnod[0], aConnectedElem);
				for(int k = 0; k < aConnectedElem.GetSize(); k++)
				{
					bNextInclude = FALSE;  // 시작할 때는 FALSE
					if(mapUsedElem.Lookup(aConnectedElem.GetAt(k), KeyTemp)) continue;   // 기존에 한번이라도 Segment로 지정되지 않고
					if(!m_mapSelFrmKey.Lookup(aConnectedElem.GetAt(k), KeyTemp)) continue;  // 현재 요소가 선택된 프레임 요소 리스트에 속하고
					if(aConnectedElem.GetAt(k) == KeyElem) continue;      // 현재 요소가 기준 요소와 다르고 
					if(!m_pDoc->m_pAttrCtrl->GetElem(aConnectedElem.GetAt(k), DElemNext)) continue;  // 현재 요소의 데이터가 정의되어 있고
					if(DataElem.elnod[0] != DElemNext.elnod[1]) continue; // 기준 요소와 현재 요소의 node i, j 순서가 동일하고          
					if(mapUsedNode.Lookup(DElemNext.elnod[0], KeyNode)) continue;  // 선택된 요소들이 폐곡선을 이루지 않아야 한다.  
					if(!IsMatchDirectionAttribute(KeyElem, aConnectedElem.GetAt(k))) continue; // 기준요소와 현재 요소가 동일한 속성이고
					
					DataMemb.aElemList.Add(aConnectedElem.GetAt(k));   // 위의 과정을 무사시 통과하면 Segment의 구성요소가 된다.
					mapUsedNode.SetAt(DElemNext.elnod[0], DElemNext.elnod[0]);    // 현재 요소의 j노드를 사용된 노드 map에 추가한다.
					KeyElem = aConnectedElem.GetAt(k);        // 현재 추가된 요소를 기준 요소로 설정
					DataElem.elnod[0] = DElemNext.elnod[0];   // 현재 추가된 요소의 i절점이 새로운 기준요소의 i절점이 된다.
					bNextInclude = TRUE;                      // 새로운 기준요소의 i절점에 대해 작업을 계속함을 의미
					break;
				}        
			}
		}

		//if(DataMemb.aElemList.GetSize() >= 1) 
	    if((m_wndAllowSingleElemMember.GetCheck() && DataMemb.aElemList.GetSize() >= 1) ||
		   (!m_wndAllowSingleElemMember.GetCheck() && DataMemb.aElemList.GetSize() >= 2))    
			{ 
				for(int m = 0; m < DataMemb.aElemList.GetSize(); m++)
				{
					mapUsedElem.SetAt(DataMemb.aElemList[m], DataMemb.aElemList[m]); // 한번이라도 Segment 지정에 사용된 요소의 map에 추가한다.
					if(m_pDoc->m_pAttrCtrl->GetPsegAssigned(DataMemb.aElemList[m], KeyMemb))
					{ // 선택된 요소가 Segment의 구성요소로 쓰일 때 삭제할 Segment를 모은다.
						if(m_pDoc->m_pAttrCtrl->ExistPseg(KeyMemb)) mapKeyMembDel.SetAt(KeyMemb, 0);
					}
				}
				AlignElemListInOrder(DataMemb);
				DataMemb.nJointType = m_nJointType;
				aDataMemb.Add(DataMemb);  // 추가할 멤버를 모은다.
			}
	}

	if(mapKeyMembDel.GetCount() > 0)
	{
		int nTemp;
		POSITION pos = mapKeyMembDel.GetStartPosition();
		while(pos)
		{
			mapKeyMembDel.GetNextAssoc(pos, KeyMemb, nTemp);
			aKeyMembDel.Add(KeyMemb);
		}  
		if(!m_pDoc->m_pDataCtrl->DelPseg(aKeyMembDel)) return;
	}
	
	BOOL bAddMemb = TRUE;  
	BOOL bAddSingleElemMember = TRUE;  
	//if(aDataMemb.GetSize() > 0)
	//{
	//	if(!m_pDoc->m_pDataCtrl->AddPseg(aDataMemb)) bAddMemb = FALSE;  // 선택된 멤버 후보를 DB에 저장한다.
	//	if(bAddMemb) InitSelectedItem();
	//	else return;      
	//}
	//else GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_SINGLE_FAIL)); // _T("선택된 요소중 Segment 지정에 성공한 요소가 없습니다.") 

	if (m_wndAllowSingleElemMember.GetCheck())  // Allow Single Element Member
	{
		if (!m_pDoc->m_pDataCtrl->AddPseg(aDataMemb)) bAddMemb = FALSE;  // 선택된 멤버 후보를 DB에 저장한다.    
		if (!AddSingleElemMember()) bAddSingleElemMember = FALSE;        // 멤버로 선정된 후보에 단일 요소 멤버를 새로이 추가한다.  
		if (bAddMemb && bAddSingleElemMember) InitSelectedItem();
		else return;
	}
	else // does not allow single element member
	{
		if (aDataMemb.GetSize() > 0)
		{
			if (!m_pDoc->m_pDataCtrl->AddPseg(aDataMemb)) bAddMemb = FALSE;  // 선택된 멤버 후보를 DB에 저장한다.
			if (bAddMemb) InitSelectedItem();
			else return;
		}
		else GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_SINGLE_FAIL)); // _T("선택된 요소중 Member 지정에 성공한 요소가 없습니다.") 
	}

/*
	// 멤버로 선정된 후보에 대해 거더와 보를 구분하여 보는 후보에서 제외하고
	// 단일 요소 멤버는 새로이 추가한다.
	if(!GetFinalSurvivedMember(aDataMemb)) return;
	if(m_pDoc->m_pDataCtrl->AddMemb(aDataMemb)) InitSelectedItem();  // 마지막으로 생존한 멤버를 DB에 저장한다.  
	else return;
*/  
}

// Frame type의 element list로부터 member를 생성하는 함수
// Automatic의 Add/Replace일 때 쓰임
void CDgnPscSegmentDlg::MemberDefineAuto()
{
}

// Frame type의 element list로부터 이들이 속한 member를 삭제하는 함수
// Manual 및 Automatic의 Delete일 때 쓰임
void CDgnPscSegmentDlg::DelAssignedMember()
{
	T_PSEG_K KeyMemb;
	T_PSEG_D DataMemb;
	CArray<T_PSEG_K, T_PSEG_K> aKeyMemb;
	CMap<T_PSEG_K, T_PSEG_K, T_PSEG_K, T_PSEG_K> mapKeyMemb;
	mapKeyMemb.InitHashTable(HASHSIZEMEMB);  
	mapKeyMemb.RemoveAll();
	aKeyMemb.RemoveAll();    
	int nNumSelFrmKey = m_aSelFrmKey.GetSize();	
	for(int i = 0; i < nNumSelFrmKey; i++)
	{
		if(m_pDoc->m_pAttrCtrl->GetPsegAssigned(m_aSelFrmKey.GetAt(i), KeyMemb))
		{ // 선택된 요소중 멤버가 할당된 요소를 map에 모은다.      
			if(m_pDoc->m_pAttrCtrl->ExistPseg(KeyMemb))
			{
				mapKeyMemb.SetAt(KeyMemb, KeyMemb);
				aKeyMemb.Add(KeyMemb);
			}
		}
	}  

	CArray<int, int> aSelItem;
	aSelItem.RemoveAll();
	int nDelMembCount = aKeyMemb.GetSize();
	if(nDelMembCount == 0) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_NO_CONSTITUENT));  // 선택된 요소 중에서 Segment의 구성요소로 지정된 요소가 없습니다. Among selected elements, no element is used as a constituent of member
	else if(nDelMembCount >= 1)
	{    
		GetSelectedItemList(&m_List, aSelItem);  // Segment 삭제후 리스트의 포커스 재설정을 위해 List박스의 선택된 리스트를 모은다.

		if(m_pDoc->m_pDataCtrl->DelPseg(aKeyMemb)) InitSelectedItem();
	}

	if(aSelItem.GetSize() > 0) SetListFocus(aSelItem);  // Segment 삭제후 리스트의 포커스 설정
}

// Segment의 요소를 i, j순서대로 다시 배열하는 함수
void CDgnPscSegmentDlg::AlignElemListInOrder(T_PSEG_D& rData)
{
	T_ELEM_K KeyElem;
	T_ELEM_D DataElem;
	CMap<T_NODE_K, T_NODE_K, T_ELEM_K, T_ELEM_K> mapNodeiElem;
	CMap<T_NODE_K, T_NODE_K, T_ELEM_K, T_ELEM_K> mapNodejElem;
	CMap<T_NODE_K, T_NODE_K, int, int> mapInnerNode;       
	CArray<T_ELEM_D, T_ELEM_D&> aDataElem;    
	mapNodeiElem.InitHashTable(53);
	mapNodejElem.InitHashTable(53);
	mapInnerNode.InitHashTable(53);
	mapNodeiElem.RemoveAll();
	mapNodejElem.RemoveAll();
	mapInnerNode.RemoveAll();
	aDataElem.RemoveAll();

	int i, j, nTemp;
	int nElem = rData.aElemList.GetSize();
	BOOL bSuccess = FALSE;
	ASSERT(nElem > 0);
		
	for(i = 0; i < nElem; i++)
	{        
		if(m_pDoc->m_pAttrCtrl->GetElem(rData.aElemList[i], DataElem)) aDataElem.Add(DataElem);    
	}
	ASSERT(nElem == aDataElem.GetSize());
	
	int nMaxNum = nElem*2;
	if(nMaxNum <= 0) {ASSERT(0); return;}
	int *nNodeIndex = new int[nMaxNum];   
	for(i = 0; i < nMaxNum; i++) nNodeIndex[i] = 0;    
	for(i = 0; i < nElem; i++)
	{    
		m_pDoc->m_pAttrCtrl->GetElem(rData.aElemList[i], DataElem);
		{
			nNodeIndex[2 * i] = DataElem.elnod[0];
			nNodeIndex[2 * i + 1] = DataElem.elnod[1];
			mapNodeiElem.SetAt(DataElem.elnod[0], rData.aElemList[i]);
			mapNodejElem.SetAt(DataElem.elnod[1], rData.aElemList[i]);
		}    
	}
	
	int nNumNode = 2 * rData.aElemList.GetSize();
	for(i = 0; i < nNumNode; i++)    // 현재 Segment의 내부 절점을 구한다.
	{
		for(j = i; j < nNumNode; j++)
		{
			if(j != i && nNodeIndex[i] == nNodeIndex[j]) {mapInnerNode.SetAt(nNodeIndex[i], 0); break;}
		}
	}
	
	int nStartNode, nLastNode;  // 연속성을 파악할 첫번째 요소의 i 노드 및 마지막 요소의 j 노드
	for(i = 0; i < nElem; i++)
	{
		m_pDoc->m_pAttrCtrl->GetElem(rData.aElemList[i], DataElem);
		if(!mapInnerNode.Lookup(DataElem.elnod[0], nTemp)) nStartNode = DataElem.elnod[0];
		if(!mapInnerNode.Lookup(DataElem.elnod[1], nTemp)) nLastNode = DataElem.elnod[1];
	}
	
	for(i = 0; i < nElem; i++)  rData.aElemList[i] = 0;  // 멤버의 요소 리스트를 초기화한다.
	mapNodeiElem.Lookup(nStartNode, KeyElem);  // 멤버의 첫번째 요소를 찾는다.
	rData.aElemList[0] = KeyElem;    // 멤버의 첫번째 요소를 설정한다.  
	for(i = 0; i < nElem; i++)
	{
		m_pDoc->m_pAttrCtrl->GetElem(KeyElem, DataElem);
		if(i < nElem - 1) 
		{
			mapNodeiElem.Lookup(DataElem.elnod[1], KeyElem);
			rData.aElemList[i + 1] = KeyElem;
		}
		if(i == nElem - 1 && DataElem.elnod[1] == nLastNode) bSuccess = TRUE;
	}
	ASSERT(bSuccess);

	delete []nNodeIndex;
}

void CDgnPscSegmentDlg::InitSelectedItem() // 선택된 요소와 절점을 해제한다.
{  
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CDgnPscSegmentDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDgnPscSegmentDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_PSEG_K Key;
	T_PSEG_D Data;

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_PSEG_ADD):
			{
				pViewBuff->GetPseg(nKey, Key, Data);
				InsertItem(Key);
			}
			break;
		case(UR_PSEG_DEL):
			{
				pViewBuff->GetPseg(nKey, Key, Data);
				DeleteItem(Key);
			}
			break;
		case(UR_PSEG_MFS):
		case(UR_PSEG_MFD):
			{
				pViewBuff->GetPseg(nKey, Key, Data);
				DeleteItem(Key);
				InsertItem(Key);        
			}
			break;
		default:
			break;
		}
	} 
}

void CDgnPscSegmentDlg::InsertItem(T_PSEG_K Key)
{
	// 삽입 위치를 찾는다.
	T_PSEG_D Data;
	int nSize = m_List.GetItemCount();
	if (!m_pDoc->m_pAttrCtrl->GetPseg(Key, Data)) return;
	CString strMembIndex, strJointType;     
	int nElemKey; 
    int nCount = 0;
	for(nCount = 0; nCount < nSize; nCount++)
	{
		strMembIndex = m_List.GetItemText(nCount, 0);
		nElemKey = _ttoi(strMembIndex);
		if(nElemKey > Data.aElemList[0]) break;
	}  
	strMembIndex.Format(_T("%d"), Data.aElemList[0]);
	int nIndex = m_List.InsertItem(nCount, strMembIndex);
	CString csItem;
	CString strElemList = _T("");
	for(int i = 0; i < Data.aElemList.GetSize() - 1; i++)
	{
		csItem.Format(_T("%d"), Data.aElemList[i]);
		strElemList += csItem + _T(", ");
	}
	if(Data.aElemList.GetSize() > 0)
	{
		csItem.Format(_T("%d"), Data.aElemList[Data.aElemList.GetSize()-1]);
		strElemList += csItem;
	}
	strJointType = _T("");
	switch (Data.nJointType)
	{
	case 0: //resin mortar
		strJointType = _LS(IDS_DGN_CHK_RESIN);
		break;
	case 1: //cement mortar
		strJointType = _LS(IDS_DGN_CHK_CEMENT);
		break;
	default:
		ASSERT(0);
		break;
	}
	m_List.SetItemText(nIndex, 1, strElemList);      
	m_List.SetItemText(nIndex, 2, strJointType);      
	m_List.SetItemData(nIndex, (DWORD)Key);
}

void CDgnPscSegmentDlg::DeleteItem(T_PSEG_K Key)
{
	int nSize = m_List.GetItemCount();
    int nCount = 0;
	for(nCount = 0; nCount < nSize; nCount++)
	{
		if(m_List.GetItemData(nCount) == Key) break;
	}  
	if(nCount == nSize) return; //GSaveHistoryFormatNF(_T("Error : Segment not found!"));
	else m_List.DeleteItem(nCount);  
}




// 선택된 모든 요소중에서 멤버로 지정되지 않은 요소들을
// 하나씩 다시 멤버로 지정하는 함수
BOOL CDgnPscSegmentDlg::AddSingleElemMember()
{
	T_PSEG_K KeyMemb;
	T_ELEM_K KeyElem;
	T_PSEG_D DataMemb;
	CArray<T_PSEG_D, T_PSEG_D&> aDataMemb;
	aDataMemb.RemoveAll();  
	
	int nNumSelFrmKey = m_aSelFrmKey.GetSize();	    
	for(int i = 0; i < nNumSelFrmKey; i++) 
	{
		DataMemb.Initialize();
		KeyElem = m_aSelFrmKey[i];
		if(!m_pDoc->m_pAttrCtrl->GetPsegAssigned(KeyElem, KeyMemb))  // 멤버로 지정되지 않은 요소만 멤버로 지정
		{
			DataMemb.aElemList.Add(KeyElem);
			aDataMemb.Add(DataMemb);
		}
	}
	if(!m_pDoc->m_pDataCtrl->AddPseg(aDataMemb)) return FALSE;
	return TRUE;
}

//=========================================================================
//============= Add/Replace 시 Segment의 적합성 판단 관련 함수 =============
//=========================================================================

BOOL CDgnPscSegmentDlg::IsMatchDirectionAttribute(T_ELEM_K KeyRef, T_ELEM_K KeyNew)
{  
	if(m_nAssignType == 1 && !CompareElemDirection(KeyRef, KeyNew)) return FALSE;  // Automatic인 경우만 요소의 직선여부를 체크한다. 
	if(!CompareElemAttribute(KeyRef, KeyNew)) return FALSE;
	return TRUE;
}

BOOL CDgnPscSegmentDlg::CompareElemDirection(T_ELEM_K KeyRef, T_ELEM_K KeyNew)
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

BOOL CDgnPscSegmentDlg::CompareElemAttribute(T_ELEM_K KeyRef, T_ELEM_K KeyNew)
{
	CDgnDataCtrl DataCtrl;
	T_ELEM_D DataElemRef, DataElemNew;
	
	if(m_pDoc->m_pAttrCtrl->GetElem(KeyRef, DataElemRef) && m_pDoc->m_pAttrCtrl->GetElem(KeyNew, DataElemNew))
	{ 
		if(DataElemRef.eltyp != DataElemNew.eltyp) return FALSE;   // 타입
		if(DataElemRef.elmat != DataElemNew.elmat) return FALSE;   // 재질
		//if(DataElemRef.elpro != DataElemNew.elpro) return FALSE;   // 단면

		// PSC 단면일경우만 허용. 
		T_SECT_K SectRefK = DataElemRef.elpro;
		T_SECT_K SectNewK = DataElemNew.elpro;
		T_SECT_D SectRefD, SectNewD;
		if(!m_pDoc->m_pAttrCtrl->GetSect(SectRefK,SectRefD)) return FALSE;
		if(!m_pDoc->m_pAttrCtrl->GetSect(SectNewK,SectNewD)) return FALSE;

        switch ( SectRefD.nStype )
        {
        case D_SECT_TYPE_TAPERED:
            if ( SectRefD.SectBefore.nStype != D_SECT_TYPE_PSC )
            {
                return FALSE;
            }
            break;
        case D_SECT_TYPE_PSC:
        case D_SECT_TYPE_COMPO_CI:
        case D_SECT_TYPE_COMPO_CT:
        case D_SECT_TYPE_COMPO_PC:
        case D_SECT_TYPE_COMPO_G:
        case D_SECT_TYPE_COMPO_TUB:
            break;
        default:
            return FALSE;
            break;
        }

        switch ( SectNewD.nStype )
        {
        case D_SECT_TYPE_TAPERED:
            if ( SectNewD.SectBefore.nStype != D_SECT_TYPE_PSC )
            {
                return FALSE;
            }
            break;
        case D_SECT_TYPE_PSC:
        case D_SECT_TYPE_COMPO_CI:
        case D_SECT_TYPE_COMPO_CT:
        case D_SECT_TYPE_COMPO_PC:
        case D_SECT_TYPE_COMPO_G:
        case D_SECT_TYPE_COMPO_TUB:
            break;
        default:
            return FALSE;
            break;
        }

		if(fabs(DataElemRef.angle - DataElemNew.angle - (int)((DataElemRef.angle - DataElemNew.angle)/360.)*360.) > 1.0e-6 &&
			 fabs(DataElemRef.angle - (int)(DataElemRef.angle/360.)*360. - DataElemNew.angle + (int)(DataElemNew.angle/360.)*360.) > 1.0e-6) 
	  return FALSE;   // beta angle
		if(DataCtrl.Get_DgnGenMbtp(KeyRef) != DataCtrl.Get_DgnGenMbtp(KeyNew)) return FALSE;  // member type
	}
	else return FALSE;
	return TRUE;
}

void CDgnPscSegmentDlg::GetFrmDirectionVector(T_ELEM_D& DataElem, double& Ux, double& Uy, double& Uz)
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
void CDgnPscSegmentDlg::SetListCtrlHeader()
{
	CString aTitle[] = { _LS(IDS_DGN_CHK_INDEX), _LS(IDS_DGN_CHK_ELEM_LIST), _LS(IDS_DGN_CHK_JOINT_TYPE) };  // _T("Index"), _T("Element List"), _T("Joint Type")
	int nColWidth[] = {40,85,75};
	int nAlign[] = {LVCFMT_CENTER, LVCFMT_LEFT, LVCFMT_CENTER};
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

void CDgnPscSegmentDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountPseg();
	if(nItemCount == 0) return;

	POSITION pos;
	int nCount, nIndex;  

	if(nItemCount)
	{
		T_PSEG_K Key;
		T_PSEG_D Data;
		T_ELEM_K KeyElem;
		CString strMembIndex, strElemList, strJointType;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartPseg();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextPseg(pos, Key, Data);		  
			KeyBuf[nCount++] = Data.aElemList[0];
		}
		qsort(KeyBuf, nItemCount, sizeof(T_ELEM_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  KeyElem = KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetPsegAssigned(KeyElem, Key);
			m_pDoc->m_pAttrCtrl->GetPseg(Key, Data);
			strMembIndex.Format(_T("%d"), Data.aElemList[0]);
			nIndex = m_List.InsertItem(nCount, strMembIndex);      
			//CNumericOptimizer optimizer;
			//strElemList = optimizer.Optimize((long*)Data.aElemList.GetData(), Data.aElemList.GetSize());
			CString csItem;
			strElemList = _T("");
			for(int i = 0; i < Data.aElemList.GetSize() - 1; i++)
			{
				csItem.Format(_T("%d"), Data.aElemList[i]);
				strElemList += csItem + _T(", ");
			}
			if(Data.aElemList.GetSize() > 0)
			{
				csItem.Format(_T("%d"), Data.aElemList[Data.aElemList.GetSize() - 1]);
				strElemList += csItem;
			}
			strJointType = _T("");
			switch (Data.nJointType)
			{
			case 0: //resin mortar
				strJointType = _LS(IDS_DGN_CHK_RESIN);
				break;
			case 1: //cement mortar
				strJointType = _LS(IDS_DGN_CHK_CEMENT);
				break;
			default:
				ASSERT(0);
				break;
			}
			m_List.SetItemText(nIndex, 1, strElemList);		
			m_List.SetItemText(nIndex, 2, strJointType);		
			m_List.SetItemData(nIndex, (DWORD)Key);
		}
		delete []KeyBuf;
	}
}
			
void CDgnPscSegmentDlg::OnDelKeyPressedInList()
{  
	CArray<T_PSEG_K, T_PSEG_K> aKeyMemb;
	CArray<int, int> aSelItem;
	aSelItem.RemoveAll();
	aKeyMemb.RemoveAll();  

	GetSelectedItemList(&m_List, aSelItem);
	int nSize = aSelItem.GetSize();
	if(nSize == 0) return;

	for(int i = 0; i < nSize; i++)
	{
		DWORD(KeyMemb) = m_List.GetItemData(aSelItem[i]);    
		if(m_pDoc->m_pAttrCtrl->ExistPseg(KeyMemb)) aKeyMemb.Add(KeyMemb);
	}  
	if(m_pDoc->m_pDataCtrl->DelPseg(aKeyMemb)) InitSelectedItem();

	SetListFocus(aSelItem);
}

// member를 삭제한 경우 List 박스의 Focus를 다시 맞춤
void CDgnPscSegmentDlg::SetListFocus(CArray<int, int>& aSelItem)
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

BOOL CDgnPscSegmentDlg::PreTranslateMessage(MSG* pMsg) 
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

void CDgnPscSegmentDlg::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
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
void CDgnPscSegmentDlg::DisplaySelElemList()
{
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelElem;  
	aSelElem.RemoveAll();
	GetSelElemList(aSelElem);  
	m_pDoc->m_pViewCtrl->SelectElem(NULL, aSelElem, FALSE, TRUE);
}

void CDgnPscSegmentDlg::GetSelElemList(CArray<T_ELEM_K, T_ELEM_K>& aSelElem)
{
	T_PSEG_D DataMemb;
	CArray<int, int> aSelItem;
	CArray<T_PSEG_K, T_PSEG_K> aMembKey;
	aMembKey.RemoveAll();
	aSelItem.RemoveAll();
	aSelElem.RemoveAll();    

	GetSelectedItemList(&m_List, aSelItem);

	int nSize = aSelItem.GetSize();
	if(nSize == 0) return;
	for(int i = 0; i < nSize; i++)
	{
		DWORD(KeyMemb) = m_List.GetItemData(aSelItem[i]);    
		aMembKey.Add(KeyMemb);
	} 
	m_pDoc->m_pAttrCtrl->GetKeyListPsegUser(&aMembKey, 0, &aSelElem);
}      

void CDgnPscSegmentDlg::GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem)
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

