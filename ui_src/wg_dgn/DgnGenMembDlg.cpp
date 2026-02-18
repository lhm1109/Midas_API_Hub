// DgnGenMembDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenMembDlg.h"
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

#include "..\wg_main\wg_mainres2.h"	// For ID_DGN_GEN_CMFT

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnGenMembDlg dialog
CDgnGenMembDlg::CDgnGenMembDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnGenMembDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenMembDlg)
	m_nOption = 0;
	m_nAssignType = 0;
	m_nSelectType = 1; 
	//}}AFX_DATA_INIT
	m_mapSelFrmKey.InitHashTable(HASHSIZEELEM);
	m_bSentMessage = FALSE;
	m_nFailureType = 0;
 	m_pDoc = CDBDoc::GetDocPoint();
}

CDgnGenMembDlg::~CDgnGenMembDlg()
{
}

void CDgnGenMembDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenMembDlg)
	DDX_Radio(pDX, IDC_DGN_MEMB_AR, m_nOption);
	DDX_Radio(pDX, IDC_DGN_MEMB_ASSIGN_MANUAL, m_nAssignType);
	DDX_Radio(pDX, IDC_DGN_MEMB_SELECT_ALL, m_nSelectType); 
	DDX_Control(pDX, IDC_DGN_MEBM_ALLOW_SINGLE_CHK, m_wndAllowSingleElemMember);
	DDX_Control(pDX, IDC_DGN_MEMB_LIST, m_List);
	DDX_Control(pDX, IDC_DGN_MEBM_TAPERED_CHK, m_wndTaperedMemb);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnGenMembDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnGenMembDlg)
	ON_BN_CLICKED(IDC_DGN_MEMB_ASSIGN_MANUAL, OnDgnAssignType)
	ON_BN_CLICKED(IDC_DGN_MEMB_ASSIGN_AUTO, OnDgnAssignType)
	ON_BN_CLICKED(IDC_DGN_MEMB_AR, OnDgnOption)
	ON_BN_CLICKED(IDC_DGN_MEMB_DEL, OnDgnOption)
	ON_BN_CLICKED(IDC_DGN_MEMB_SELECT_ALL, OnDgnMemberSelect)
	ON_BN_CLICKED(IDC_DGN_MEMB_SELECT_SEL, OnDgnMemberSelect)  
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_MEMB_LIST, OnItemchanged)	
	ON_BN_CLICKED(IDC_DGN_MEBM_TAPERED_CHK, OnChkAssignTaperedMemb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenMembDlg message handlers
BOOL CDgnGenMembDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	m_aCtrlAll.Add(IDC_DGN_MEMB_ASSIGN_MANUAL);
	m_aCtrlAll.Add(IDC_DGN_MEMB_ASSIGN_AUTO);
	m_aCtrlAll.Add(IDC_DGN_MEMB_SELECT_ALL);
	m_aCtrlAll.Add(IDC_DGN_MEMB_SELECT_SEL);
	m_aCtrlAll.Add(IDC_DGN_MEBM_ALLOW_SINGLE_CHK);

	m_wndAllowSingleElemMember.SetCheck(FALSE);
	m_wndTaperedMemb.SetCheck(FALSE);

#ifdef _MGEN_CH
	GetDlgItem(IDC_DGN_MEBM_TAPERED_CHK)->ShowWindow(TRUE);
#else
	GetDlgItem(IDC_DGN_MEBM_TAPERED_CHK)->ShowWindow(FALSE);
#endif

	SetListCtrlHeader();
	MakeItemEx();
	
	CtrlManager();  

	// Apply Close Button Hide
	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnGenMembDlg::OnDgnOption() 
{
	CtrlManager();
}

void CDgnGenMembDlg::OnDgnAssignType() 
{
	CtrlManager();
}

void CDgnGenMembDlg::OnDgnMemberSelect()
{
	DisplaySelElemList();   // 선택된 요소를 Display한다.
}

//  CButton* AllBtn = (CButton*)GetDlgItem(IDC_DGN_MEMB_SELECT_ALL);  
//  AllBtn->SetCheck(FALSE);

void CDgnGenMembDlg::CtrlManager() 
{
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
}

void CDgnGenMembDlg::OnChkAssignTaperedMemb()
{
	UpdateData(TRUE);
	if (m_wndTaperedMemb.GetCheck())
	{
		GetDlgItem(IDC_DGN_MEMB_ASSIGN_AUTO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_MEMB_SELECT_ALL)->EnableWindow(FALSE);
		if (m_nAssignType == 1)
			m_nAssignType = 0;
		if (m_nSelectType == 0)
			m_nSelectType = 1;
	}
	else
	{
		GetDlgItem(IDC_DGN_MEMB_ASSIGN_AUTO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_MEMB_SELECT_ALL)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}

void CDgnGenMembDlg::Execute() 
{
	UpdateData(TRUE);	
	CArray<T_ELEM_K,T_ELEM_K> aSelElemKey;   // Select된 Element List
	
	if(m_nOption == 0) // Add/Replace
	{
		if(m_nSelectType == 0 && m_nAssignType == 1)  // Selection Type : All , Assign Type : Automatic
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

void CDgnGenMembDlg::MemberDefine()
{   
	int nNumSelFrmKey = m_aSelFrmKey.GetSize();
	int nNumNonFrmKey = m_aNonFrmKey.GetSize();

	if(m_nOption == 0)  // Add/Replace
	{
		if(nNumSelFrmKey == 0)      GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_ELEM_TYPE)); // [오류] Frame Type 요소만 Member 지정이 가능합니다.
//    else if(nNumSelFrmKey == 1) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_NUM_FRAME)); // [오류] Member 지정에는 두 개 이상의 Frame Type 요소가 필요합니다.
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
				GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_NON_FRAME), m_aNonFrmKey.GetAt(i));  // 요소 %d은(는) Frame Type이 아니므로 Member를 지정할 수 없습니다.
		}
	}
	else if(m_nOption == 1)	// Delete : 요소가 Member의 구성요소 일때 그 요소가 속한 Member를 삭제한다.
	{
		if(nNumSelFrmKey == 0)      GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_ONLY_FRAME)); // Frame Type 요소만 Member 지정 및 해제가 가능합니다.
		else if(nNumSelFrmKey >= 1) DelAssignedMember();
		else ASSERT(0);
	}

//  MakeItemEx();
}

// Frame type의 element list로부터 member를 생성하는 함수
// Manual의 Add/Replace일 때 쓰임
void CDgnGenMembDlg::MemberDefineManual()
{
	T_MEMB_K KeyMemb;
	T_MEMB_D DataMemb;
	T_ELEM_K KeyElem, KeyTemp;
	T_ELEM_D DataElem, DElemNext;
	T_NODE_K KeyNode;
	CArray<T_ELEM_K, T_ELEM_K> aConnectedElem;
	CArray<T_MEMB_K, T_MEMB_K> aKeyMembDel;        // 최종적으로 삭제할 멤버의 리스트
	CArray<T_MEMB_D, T_MEMB_D&> aDataMemb;         // 최종적으로 선택된 멤버의 리스트
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapUsedElem;  // 한 번이라도 Member 지정에 사용된 요소의 map  
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K> mapUsedNode;  // Member의 Element List에 속하는 요소들의 절점의 map
	CMap<T_MEMB_K, T_MEMB_K, int, int> mapKeyMembDel; // 기존 요소가 멤버의 구성요소일 때 삭제할 멤버의 map  
	mapUsedElem.InitHashTable(HASHSIZEELEM);  
	mapKeyMembDel.InitHashTable(HASHSIZEMEMB);
	mapUsedNode.InitHashTable(51);
	mapUsedElem.RemoveAll();
	mapKeyMembDel.RemoveAll();  
	aKeyMembDel.RemoveAll();
	aDataMemb.RemoveAll();

	DataMemb.Initialize();
	DataElem.Initialize();
	DElemNext.Initialize();
	BOOL bAddMembFlag = FALSE; // 하나의 Member정의가 끝났음을 알리는 flag  
	BOOL bNextInclude = FALSE; // 현재 요소의 절점에 연결된 요소가 동일한 Member의 구성요소로 포함되었는지를 나타내는 flag  
	
	int nNumSelFrmKey = m_aSelFrmKey.GetSize();	 
	for (int i = 0; i < nNumSelFrmKey; i++)
	{
		DataMemb.Initialize();
		mapUsedNode.RemoveAll();   // 폐곡선을 막기 위해 쓰임
		KeyElem = m_aSelFrmKey.GetAt(i);
		if (!mapUsedElem.Lookup(KeyElem, KeyTemp)) // 기존에 한번이라도 Member로 지정되지 않은 요소이면
			DataMemb.aElemList.Add(KeyElem);        // 현재 지정할 Member의 요소 리스트에 추가한다.
		else continue; // 기존에 Member로 지정된 요소이면 현재 요소에 대한 Member 지정을 하지 않고 다음 요소에 대해 작업한다.

		if (m_pDoc->m_pAttrCtrl->GetElem(KeyElem, DataElem))
		{
			mapUsedNode.SetAt(DataElem.elnod[0], DataElem.elnod[0]);    // 기준 요소의 i노드를 사용된 노드 map에 추가한다.
			mapUsedNode.SetAt(DataElem.elnod[1], DataElem.elnod[1]);    // 기준 요소의 j노드를 사용된 노드 map에 추가한다.
			bNextInclude = TRUE;
			while (bNextInclude)  // j node 방향으로 연결된 요소에 대해 작업한다.
			{
				m_pDoc->m_pAttrCtrl->GetElemKeyListConnectedToNode(DataElem.elnod[1], aConnectedElem);
				for (int k = 0; k < aConnectedElem.GetSize(); k++)
				{
					bNextInclude = FALSE;  // 시작할때는 FALSE
					if (mapUsedElem.Lookup(aConnectedElem.GetAt(k), KeyTemp)) continue;   // 기존에 한번이라도 Member로 지정되지 않고
					if (!m_mapSelFrmKey.Lookup(aConnectedElem.GetAt(k), KeyTemp)) continue;  // 현재 요소가 선택된 프레임 요소 리스트에 속하고
					if (aConnectedElem.GetAt(k) == KeyElem) continue;      // 현재 요소가 기준 요소와 다르고           
					if (!m_pDoc->m_pAttrCtrl->GetElem(aConnectedElem.GetAt(k), DElemNext)) continue;  // 현재 요소의 데이터가 정의되어 있고
					if (DataElem.elnod[1] != DElemNext.elnod[0]) continue; // 기준 요소와 현재 요소의 node i, j 순서가 동일하고  	
					if (mapUsedNode.Lookup(DElemNext.elnod[1], KeyNode)) continue;  // 선택된 요소들이 폐곡선을 이루지 않아야 한다.  
					if (!IsMatchDirectionAttribute(KeyElem, aConnectedElem.GetAt(k))) continue; // 기준요소와 현재 요소가 동일한 속성이고

					DataMemb.aElemList.Add(aConnectedElem.GetAt(k));   // 위의 과정을 무사시 통과하면 Member의 구성요소가 된다.
					mapUsedNode.SetAt(DElemNext.elnod[1], DElemNext.elnod[1]);    // 현재 요소의 j노드를 사용된 노드 map에 추가한다.
					KeyElem = aConnectedElem.GetAt(k);        // 현재 추가된 요소를 기준 요소로 설정
					DataElem.elnod[1] = DElemNext.elnod[1];   // 현재 추가된 요소의 j절점이 새로운 기준요소의 i절점이 된다.          
					bNextInclude = TRUE;  // 새로운 기준요소의 j절점에 대해 작업을 계속함을 의미
					break;
				}
			}

			bNextInclude = TRUE;
			while (bNextInclude)   // i node 방향으로 연결된 요소에 대해 작업한다.
			{
				m_pDoc->m_pAttrCtrl->GetElemKeyListConnectedToNode(DataElem.elnod[0], aConnectedElem);
				for (int k = 0; k < aConnectedElem.GetSize(); k++)
				{
					bNextInclude = FALSE;  // 시작할 때는 FALSE
					if (mapUsedElem.Lookup(aConnectedElem.GetAt(k), KeyTemp)) continue;   // 기존에 한번이라도 Member로 지정되지 않고
					if (!m_mapSelFrmKey.Lookup(aConnectedElem.GetAt(k), KeyTemp)) continue;  // 현재 요소가 선택된 프레임 요소 리스트에 속하고
					if (aConnectedElem.GetAt(k) == KeyElem) continue;      // 현재 요소가 기준 요소와 다르고 
					if (!m_pDoc->m_pAttrCtrl->GetElem(aConnectedElem.GetAt(k), DElemNext)) continue;  // 현재 요소의 데이터가 정의되어 있고
					if (DataElem.elnod[0] != DElemNext.elnod[1]) continue; // 기준 요소와 현재 요소의 node i, j 순서가 동일하고          		 
					if (mapUsedNode.Lookup(DElemNext.elnod[0], KeyNode)) continue;  // 선택된 요소들이 폐곡선을 이루지 않아야 한다. 	
					if (!IsMatchDirectionAttribute(KeyElem, aConnectedElem.GetAt(k))) continue; // 기준요소와 현재 요소가 동일한 속성이고

					DataMemb.aElemList.Add(aConnectedElem.GetAt(k));   // 위의 과정을 무사시 통과하면 Member의 구성요소가 된다.
					mapUsedNode.SetAt(DElemNext.elnod[0], DElemNext.elnod[0]);    // 현재 요소의 j노드를 사용된 노드 map에 추가한다.
					KeyElem = aConnectedElem.GetAt(k);        // 현재 추가된 요소를 기준 요소로 설정
					DataElem.elnod[0] = DElemNext.elnod[0];   // 현재 추가된 요소의 i절점이 새로운 기준요소의 i절점이 된다.
					bNextInclude = TRUE;                      // 새로운 기준요소의 i절점에 대해 작업을 계속함을 의미
					break;
				}
			}
		}
		//    if(DataMemb.aElemList.GetSize() >= 1) 
		if (m_wndTaperedMemb.GetCheck() && DataMemb.aElemList.GetSize() >= 1)
		{
			for (int m = 0; m < DataMemb.aElemList.GetSize(); m++)
			{
				mapUsedElem.SetAt(DataMemb.aElemList[m], DataMemb.aElemList[m]); // 한번이라도 Member 지정에 사용된 요소의 map에 추가한다.
				if (m_pDoc->m_pAttrCtrl->GetMembAssigned(DataMemb.aElemList[m], KeyMemb))
				{ // 선택된 요소가 Member의 구성요소로 쓰일 때 삭제할 Member를 모은다.
					if (m_pDoc->m_pAttrCtrl->ExistMemb(KeyMemb)) mapKeyMembDel.SetAt(KeyMemb, 0);
				}
			}
			AlignElemListInOrder(DataMemb);
			aDataMemb.Add(DataMemb);  // 추가할 멤버를 모은다.
		}
		else
		{
			if ((m_wndAllowSingleElemMember.GetCheck() && DataMemb.aElemList.GetSize() >= 1) ||
				(!m_wndAllowSingleElemMember.GetCheck() && DataMemb.aElemList.GetSize() >= 2))
			{
				for (int m = 0; m < DataMemb.aElemList.GetSize(); m++)
				{
					mapUsedElem.SetAt(DataMemb.aElemList[m], DataMemb.aElemList[m]); // 한번이라도 Member 지정에 사용된 요소의 map에 추가한다.
					if (m_pDoc->m_pAttrCtrl->GetMembAssigned(DataMemb.aElemList[m], KeyMemb))
					{ // 선택된 요소가 Member의 구성요소로 쓰일 때 삭제할 Member를 모은다.
						if (m_pDoc->m_pAttrCtrl->ExistMemb(KeyMemb)) mapKeyMembDel.SetAt(KeyMemb, 0);
					}
				}
				AlignElemListInOrder(DataMemb);
				aDataMemb.Add(DataMemb);  // 추가할 멤버를 모은다.
			}
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
		if(!m_pDoc->m_pDataCtrl->DelMemb(aKeyMembDel)) return;
	}
	
	BOOL bAddMemb = TRUE;  
	BOOL bAddSingleElemMember = TRUE;  
	if(m_wndAllowSingleElemMember.GetCheck())  // Allow Single Element Member
	{
		if(!m_pDoc->m_pDataCtrl->AddMemb(aDataMemb)) bAddMemb = FALSE;  // 선택된 멤버 후보를 DB에 저장한다.    
		if(!AddSingleElemMember()) bAddSingleElemMember = FALSE;        // 멤버로 선정된 후보에 단일 요소 멤버를 새로이 추가한다.  
		if(bAddMemb && bAddSingleElemMember) InitSelectedItem();
		else return;    
	}
	else // does not allow single element member
	{    
		if(aDataMemb.GetSize() > 0)
		{
			if(!m_pDoc->m_pDataCtrl->AddMemb(aDataMemb, m_wndTaperedMemb.GetCheck())) bAddMemb = FALSE;  // 선택된 멤버 후보를 DB에 저장한다.
			if(bAddMemb) InitSelectedItem();
			else return;      
		}
		else 
		{
			if      (m_nFailureType == 0) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_SINGLE_FAIL)); // _T("선택된 요소중 Member 지정에 성공한 요소가 없습니다.") 
			else if (m_nFailureType == 1) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_SINGLE_FAIL_ELEMTYPE)); 
			else if (m_nFailureType == 2) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_SINGLE_FAIL_ELEMMATL)); 
			else if (m_nFailureType == 3) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_SINGLE_FAIL_ELEMPROP)); 
			//else if (m_nFailureType == 4) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_SINGLE_FAIL_TAPERSECT)); 
			else if (m_nFailureType == 5) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_SINGLE_FAIL_BETAANG)); 
			else if (m_nFailureType == 6) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_SINGLE_FAIL_MEMBTYPE)); 
		}
	}
/*
	// 멤버로 선정된 후보에 대해 거더와 보를 구분하여 보는 후보에서 제외하고
	// 단일 요소 멤버는 새로이 추가한다.
	if(!GetFinalSurvivedMember(aDataMemb)) return;
	if(m_pDoc->m_pDataCtrl->AddMemb(aDataMemb)) InitSelectedItem();  // 마지막으로 생존한 멤버를 DB에 저장한다.  
	else return;
*/  
}

 BOOL CDgnGenMembDlg::IsReleaseMyz_CH(T_ELEM_K elemK1, T_KEY elemK2)
 {
#ifdef _MGEN_CH
		 T_ELEM_D DataElem, DElemNext;
		 if(!m_pDoc->m_pAttrCtrl->GetElem(elemK1, DataElem) || !m_pDoc->m_pAttrCtrl->GetElem(elemK2, DElemNext))
				 return FALSE;
		 BOOL bNode_I[2] = {TRUE, TRUE};
		 T_ELEM_K elemK[2] = {elemK1, elemK2};
		 bNode_I[0] = (DataElem.elnod[0] == DElemNext.elnod[0] || DataElem.elnod[0] == DElemNext.elnod[1]);
		 bNode_I[1] = (DataElem.elnod[0] == DElemNext.elnod[0] || DataElem.elnod[1] == DElemNext.elnod[0]);
		 T_FRLS_D frlsD;
		 for (int i=0; i<2; i++)
		 {
				 if(!m_pDoc->m_pAttrCtrl->GetFrlsElem(elemK[i], frlsD))
						 continue;
				 int nIdx = bNode_I[i] ? 0 : 1;
				 if(frlsD.Release[nIdx][4] == '1')
						 return TRUE;
				 if(frlsD.Release[nIdx][5] == '1')
						 return TRUE;
		 }
		 return FALSE;
#endif
		return FALSE;
 }

// Frame type의 element list로부터 member를 생성하는 함수
// Automatic의 Add/Replace일 때 쓰임
void CDgnGenMembDlg::MemberDefineAuto()
{  
	T_MEMB_K KeyMemb;
	T_MEMB_D DataMemb;
	T_ELEM_K KeyElem, KeyTemp;
	T_ELEM_D DataElem, DElemNext;
	T_NODE_K KeyNode;  
	CArray<T_ELEM_K, T_ELEM_K> aConnectedElem;
	CArray<T_MEMB_K, T_MEMB_K> aKeyMembDel;        // 최종적으로 삭제할 멤버의 리스트
	CArray<T_MEMB_D, T_MEMB_D&> aDataMemb;         // 최종적으로 선택된 멤버의 리스트
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapUsedElem;  // 한 번이라도 Member 지정에 사용된 요소의 map  
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K> mapUsedNode;  // Member의 Element List에 속하는 요소들의 절점의 map
	CMap<T_MEMB_K, T_MEMB_K, int, int> mapKeyMembDel; // 기존 요소가 멤버의 구성요소일 때 삭제할 멤버의 map
	mapUsedElem.InitHashTable(HASHSIZEELEM);
	mapKeyMembDel.InitHashTable(HASHSIZEMEMB);
	mapUsedNode.InitHashTable(51);
	mapUsedElem.RemoveAll();
	mapKeyMembDel.RemoveAll();
	aConnectedElem.RemoveAll();
	aKeyMembDel.RemoveAll();
	aDataMemb.RemoveAll();
	
	DataMemb.Initialize();
	DataElem.Initialize();
	DElemNext.Initialize();
	BOOL bAddMembFlag = FALSE; // 하나의 Member정의가 끝났음을 알리는 flag  
	BOOL bNextInclude = FALSE; // 현재 요소의 절점에 연결된 요소가 동일한 Member의 구성요소로 포함되었는지를 나타내는 flag  
	CDgnDataCtrl DataCtrl;

	int nNumSelFrmKey = m_aSelFrmKey.GetSize();	    
	for(int i = 0; i < nNumSelFrmKey; i++) 
	{    
		DataMemb.Initialize();
		mapUsedNode.RemoveAll();   // 폐곡선을 막기 위해 쓰임
		KeyElem = m_aSelFrmKey.GetAt(i);
		if(!mapUsedElem.Lookup(KeyElem, KeyTemp)) // 기존에 한번이라도 Member로 지정되지 않은 요소이면
			DataMemb.aElemList.Add(KeyElem);        // 현재 지정할 Member의 요소 리스트에 추가한다.
		else continue; // 기존에 Member로 지정된 요소이면 현재 요소에 대한 Member 지정을 하지 않고 다음 요소에 대해 작업한다.
		
		if(m_pDoc->m_pAttrCtrl->GetElem(KeyElem, DataElem))
		{       
			mapUsedNode.SetAt(DataElem.elnod[0], DataElem.elnod[0]);    // 기준 요소의 i노드를 사용된 노드 map에 추가한다.
			mapUsedNode.SetAt(DataElem.elnod[1], DataElem.elnod[1]);    // 기준 요소의 j노드를 사용된 노드 map에 추가한다.
			bNextInclude = TRUE;      
			if(IsSupportAndColumnWallUpperExist(KeyElem, DataElem.elnod[1])) bNextInclude = FALSE;
			while(bNextInclude)  // j node 방향으로 연결된 요소에 대해 작업한다.
			{
				m_pDoc->m_pAttrCtrl->GetElemKeyListConnectedToNode(DataElem.elnod[1], aConnectedElem);
				// 현재 요소가 기둥이 아닌 경우에는 아래층에 기둥 또는 벽체가 존재하지 않아야 함, 또는
				// 현재 요소가 기둥인 경우에는 i 절점이 층 level과 같지 않아야 함 
				if((DataCtrl.Get_DgnGenMbtp(KeyElem) != D_MBTP_COLUMN && !IsExistColumnWallLower(DataElem.elnod[1], aConnectedElem)) ||
					 (DataCtrl.Get_DgnGenMbtp(KeyElem) == D_MBTP_COLUMN && !IsStoryLevel(DataElem.elnod[1])))   
				{ 
					for(int k = 0; k < aConnectedElem.GetSize(); k++)
					{
						bNextInclude = FALSE;  // 시작할때는 FALSE
						if(mapUsedElem.Lookup(aConnectedElem.GetAt(k), KeyTemp)) continue;   // 기존에 한번이라도 Member로 지정되지 않고
						if(!m_mapSelFrmKey.Lookup(aConnectedElem.GetAt(k), KeyTemp)) continue;  // 현재 요소가 선택된 프레임 요소 리스트에 속하고
						if(aConnectedElem.GetAt(k) == KeyElem) continue;      // 현재 요소가 기준 요소와 다르고 
						if(!m_pDoc->m_pAttrCtrl->GetElem(aConnectedElem.GetAt(k), DElemNext)) continue;  // 현재 요소의 데이터가 정의되어 있고
						if(DataElem.elnod[1] != DElemNext.elnod[0]) continue; // 기준 요소와 현재 요소의 node i, j 순서가 동일하고
						if(mapUsedNode.Lookup(DElemNext.elnod[1], KeyNode)) continue;  // 선택된 요소들이 폐곡선을 이루지 않아야 한다.  
						if(!IsMatchDirectionAttribute(KeyElem, aConnectedElem.GetAt(k))) continue; // 기준요소와 현재 요소가 동일한 속성이고
						if(IsReleaseMyz_CH(KeyElem, aConnectedElem.GetAt(k))) continue;
						
						DataMemb.aElemList.Add(aConnectedElem.GetAt(k));   // 위의 과정을 무사히 통과하면 Member의 구성요소가 된다.
						mapUsedNode.SetAt(DElemNext.elnod[1], DElemNext.elnod[1]);    // 현재 요소의 j노드를 사용된 노드 map에 추가한다.
						KeyElem = aConnectedElem.GetAt(k);        // 현재 추가된 요소를 기준 요소로 설정
						DataElem.elnod[1] = DElemNext.elnod[1];   // 현재 추가된 요소의 j절점이 새로운 기준요소의 i절점이 된다.
						bNextInclude = TRUE;                      // 새로운 기준요소의 j절점에 대해 작업을 계속함을 의미                                    
						if(IsSupportAndColumnWallUpperExist(KeyElem, DataElem.elnod[1])) bNextInclude = FALSE;
						break;
					}
				}
				else bNextInclude = FALSE;  // 아래층에 기둥 또는 벽체가 존재하면 현재 절점에 대한 작업 중단
			}
			
			bNextInclude = TRUE;      
			if(IsSupportAndColumnWallUpperExist(KeyElem, DataElem.elnod[0])) bNextInclude = FALSE;
			while(bNextInclude)   // i node 방향으로 연결된 요소에 대해 작업한다.
			{
				m_pDoc->m_pAttrCtrl->GetElemKeyListConnectedToNode(DataElem.elnod[0], aConnectedElem);
				// 현재 요소가 기둥이 아닌 경우에는 아래층에 기둥 또는 벽체가 존재하지 않아야 함, 또는
				// 현재 요소가 기둥인 경우에는 i 절점이 층 level과 같지 않아야 함 
				if((DataCtrl.Get_DgnGenMbtp(KeyElem) != D_MBTP_COLUMN && !IsExistColumnWallLower(DataElem.elnod[0], aConnectedElem)) ||
					 (DataCtrl.Get_DgnGenMbtp(KeyElem) == D_MBTP_COLUMN && !IsStoryLevel(DataElem.elnod[0])))   
				{
					for(int k = 0; k < aConnectedElem.GetSize(); k++)
					{
						bNextInclude = FALSE;  // 시작할 때는 FALSE
						if(mapUsedElem.Lookup(aConnectedElem.GetAt(k), KeyTemp)) continue;   // 기존에 한번이라도 Member로 지정되지 않고
						if(!m_mapSelFrmKey.Lookup(aConnectedElem.GetAt(k), KeyTemp)) continue;  // 현재 요소가 선택된 프레임 요소 리스트에 속하고
						if(aConnectedElem.GetAt(k) == KeyElem) continue;      // 현재 요소가 기준 요소와 다르고 
						if(!m_pDoc->m_pAttrCtrl->GetElem(aConnectedElem.GetAt(k), DElemNext)) continue;  // 현재 요소의 데이터가 정의되어 있고
						if(DataElem.elnod[0] != DElemNext.elnod[1]) continue; // 기준 요소와 현재 요소의 node i, j 순서가 동일하고          
						if(mapUsedNode.Lookup(DElemNext.elnod[0], KeyNode)) continue;  // 선택된 요소들이 폐곡선을 이루지 않아야 한다.  
						if(!IsMatchDirectionAttribute(KeyElem, aConnectedElem.GetAt(k))) continue; // 기준요소와 현재 요소가 동일한 속성이고
						if(IsReleaseMyz_CH(KeyElem, aConnectedElem.GetAt(k))) continue;

						DataMemb.aElemList.Add(aConnectedElem.GetAt(k));   // 위의 과정을 무사히 통과하면 Member의 구성요소가 된다.
						mapUsedNode.SetAt(DElemNext.elnod[0], DElemNext.elnod[0]);    // 현재 요소의 j노드를 사용된 노드 map에 추가한다.
						KeyElem = aConnectedElem.GetAt(k);        // 현재 추가된 요소를 기준 요소로 설정
						DataElem.elnod[0] = DElemNext.elnod[0];   // 현재 추가된 요소의 i절점이 새로운 기준요소의 i절점이 된다.
						bNextInclude = TRUE;                      // 새로운 기준요소의 i절점에 대해 작업을 계속함을 의미                        
						if(IsSupportAndColumnWallUpperExist(KeyElem, DataElem.elnod[0])) bNextInclude = FALSE;
						break;
					}
				}        
				else bNextInclude = FALSE;  // 아래층에 기둥 또는 벽체가 존재하면 현재 절점에 대한 작업 중단
			}
		}  
				
		if((m_wndAllowSingleElemMember.GetCheck() && DataMemb.aElemList.GetSize() >= 1) ||
			 (!m_wndAllowSingleElemMember.GetCheck() && DataMemb.aElemList.GetSize() >= 2))  
		{ 
			for(int m = 0; m < DataMemb.aElemList.GetSize(); m++)
			{
				mapUsedElem.SetAt(DataMemb.aElemList[m], DataMemb.aElemList[m]); // 한번이라도 Member 지정에 사용된 요소의 map에 추가한다.
				if(m_pDoc->m_pAttrCtrl->GetMembAssigned(DataMemb.aElemList[m], KeyMemb))
				{ // 선택된 요소가 Member의 구성요소로 쓰일 때 삭제할 Member를 모은다.
					if(m_pDoc->m_pAttrCtrl->ExistMemb(KeyMemb)) mapKeyMembDel.SetAt(KeyMemb, 0);
				}
			}
			AlignElemListInOrder(DataMemb);
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
		if(!m_pDoc->m_pDataCtrl->DelMemb(aKeyMembDel)) return;  //
	}

	BOOL bAddMemb = TRUE;
	BOOL bDelInferiorMember = TRUE;
	BOOL bAddSingleElemMember = TRUE;
	if(m_wndAllowSingleElemMember.GetCheck())
	{
		if(!m_pDoc->m_pDataCtrl->AddMemb(aDataMemb)) bAddMemb = FALSE;  // 선택된 멤버 후보를 DB에 저장한다.  
		if(!DelInferiorMember())  bDelInferiorMember = FALSE;           // 거더와 보를 구분하여 보의 멤버 지정을 해제한다.
		if(!AddSingleElemMember()) bAddSingleElemMember = FALSE;        // 멤버로 선정된 후보에 단일 요소 멤버를 새로이 추가한다.  
		if(bAddMemb && bDelInferiorMember && bAddSingleElemMember) InitSelectedItem();
		else return;
	}
	else
	{    
		if(aDataMemb.GetSize() > 0)
		{
			if(!m_pDoc->m_pDataCtrl->AddMemb(aDataMemb)) bAddMemb = FALSE;  // 선택된 멤버 후보를 DB에 저장한다.  
			if(!DelInferiorMember())  bDelInferiorMember = FALSE;           // 거더와 보를 구분하여 보의 멤버 지정을 해제한다.
			if(bAddMemb && bDelInferiorMember) InitSelectedItem();
			else return;
		}
		else 
		{
			if      (m_nFailureType == 0) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_SINGLE_FAIL)); // _T("선택된 요소중 Member 지정에 성공한 요소가 없습니다.") 
			else if (m_nFailureType == 1) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_SINGLE_FAIL_ELEMTYPE)); 
			else if (m_nFailureType == 2) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_SINGLE_FAIL_ELEMMATL)); 
			else if (m_nFailureType == 3) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_SINGLE_FAIL_ELEMPROP)); 
			//else if (m_nFailureType == 4) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_SINGLE_FAIL_TAPERSECT)); 
			else if (m_nFailureType == 5) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_SINGLE_FAIL_BETAANG)); 
			else if (m_nFailureType == 6) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_SINGLE_FAIL_MEMBTYPE)); 
		}
	}
/*
	// 멤버로 선정된 후보에 대해 거더와 보를 구분하여 보는 후보에서 제외하고
	// 단일 요소 멤버는 새로이 추가한다.
	if(!GetFinalSurvivedMember(aDataMemb)) return;
	if(m_pDoc->m_pDataCtrl->AddMemb(aDataMemb)) InitSelectedItem();  // 마지막으로 생존한 멤버를 DB에 저장한다.  
	else return;
*/
}


// Frame type의 element list로부터 이들이 속한 member를 삭제하는 함수
// Manual 및 Automatic의 Delete일 때 쓰임
void CDgnGenMembDlg::DelAssignedMember()
{
	T_MEMB_K KeyMemb;
	T_MEMB_D DataMemb;
	CArray<T_MEMB_K, T_MEMB_K> aKeyMemb;
	CMap<T_MEMB_K, T_MEMB_K, T_MEMB_K, T_MEMB_K> mapKeyMemb;
	mapKeyMemb.InitHashTable(HASHSIZEMEMB);  
	mapKeyMemb.RemoveAll();
	aKeyMemb.RemoveAll();    
	int nNumSelFrmKey = m_aSelFrmKey.GetSize();	
	for(int i = 0; i < nNumSelFrmKey; i++)
	{
		if(m_pDoc->m_pAttrCtrl->GetMembAssigned(m_aSelFrmKey.GetAt(i), KeyMemb))
		{ // 선택된 요소중 멤버가 할당된 요소를 map에 모은다.      
			if(m_pDoc->m_pAttrCtrl->ExistMemb(KeyMemb)) mapKeyMemb.SetAt(KeyMemb, KeyMemb);
		}
	}
	
	POSITION pos = mapKeyMemb.GetStartPosition();
	while(pos)
	{
		mapKeyMemb.GetNextAssoc(pos, KeyMemb, KeyMemb);
		aKeyMemb.Add(KeyMemb);
	}

	CArray<int, int> aSelItem;
	aSelItem.RemoveAll();
	int nDelMembCount = aKeyMemb.GetSize();
	if(nDelMembCount == 0) GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_NO_CONSTITUENT));  // 선택된 요소 중에서 Member의 구성요소로 지정된 요소가 없습니다. Among selected elements, no element is used as a constituent of member
	else if(nDelMembCount >= 1)
	{    
		GetSelectedItemList(&m_List, aSelItem);  // Member 삭제후 리스트의 포커스 재설정을 위해 List박스의 선택된 리스트를 모은다.

		if(m_pDoc->m_pDataCtrl->DelMemb(aKeyMemb)) InitSelectedItem();
	}

	if(aSelItem.GetSize() > 0) SetListFocus(aSelItem);  // Member 삭제후 리스트의 포커스 설정
}

// Member의 요소를 i, j순서대로 다시 배열하는 함수
void CDgnGenMembDlg::AlignElemListInOrder(T_MEMB_D& rData)
{
	m_pDoc->m_pAttrCtrl->AlignElemListInOrder(rData);

// m_pAttrCtrl->AlignElemListInOrder(rData) 로 이동
// 	T_ELEM_K KeyElem;
// 	T_ELEM_D DataElem;
// 	CMap<T_NODE_K, T_NODE_K, T_ELEM_K, T_ELEM_K> mapNodeiElem;
// 	CMap<T_NODE_K, T_NODE_K, T_ELEM_K, T_ELEM_K> mapNodejElem;
// 	CMap<T_NODE_K, T_NODE_K, int, int> mapInnerNode;       
// 	CArray<T_ELEM_D, T_ELEM_D&> aDataElem;    
// 	mapNodeiElem.InitHashTable(51);
// 	mapNodejElem.InitHashTable(51);
// 	mapInnerNode.InitHashTable(51);
// 	mapNodeiElem.RemoveAll();
// 	mapNodejElem.RemoveAll();
// 	mapInnerNode.RemoveAll();
// 	aDataElem.RemoveAll();
// 
// 	int i, j, nTemp;
// 	int nElem = rData.aElemList.GetSize();
// 	BOOL bSuccess = FALSE;
// 	ASSERT(nElem > 0);
// 		
// 	for(i = 0; i < nElem; i++)
// 	{        
// 		if(m_pDoc->m_pAttrCtrl->GetElem(rData.aElemList[i], DataElem)) aDataElem.Add(DataElem);    
// 	}
// 	ASSERT(nElem == aDataElem.GetSize());
// 	
// 	int nMaxNum = nElem*2;
// 	if(nMaxNum <= 0) {ASSERT(0); return;}
// 	int *nNodeIndex = new int[nMaxNum];   
// 	for(i = 0; i < nMaxNum; i++) nNodeIndex[i] = 0;    
// 	for(i = 0; i < nElem; i++)
// 	{    
// 		m_pDoc->m_pAttrCtrl->GetElem(rData.aElemList[i], DataElem);
// 		{
// 			nNodeIndex[2 * i] = DataElem.elnod[0];
// 			nNodeIndex[2 * i + 1] = DataElem.elnod[1];
// 			mapNodeiElem.SetAt(DataElem.elnod[0], rData.aElemList[i]);
// 			mapNodejElem.SetAt(DataElem.elnod[1], rData.aElemList[i]);
// 		}    
// 	}
// 	
// 	int nNumNode = 2 * rData.aElemList.GetSize();
// 	for(i = 0; i < nNumNode; i++)    // 현재 Member의 내부 절점을 구한다.
// 	{
// 		for(j = i; j < nNumNode; j++)
// 		{
// 			if(j != i && nNodeIndex[i] == nNodeIndex[j]) {mapInnerNode.SetAt(nNodeIndex[i], 0); break;}
// 		}
// 	}
// 	
// 	int nStartNode, nLastNode;  // 연속성을 파악할 첫번째 요소의 i 노드 및 마지막 요소의 j 노드
// 	for(i = 0; i < nElem; i++)
// 	{
// 		m_pDoc->m_pAttrCtrl->GetElem(rData.aElemList[i], DataElem);
// 		if(!mapInnerNode.Lookup(DataElem.elnod[0], nTemp)) nStartNode = DataElem.elnod[0];
// 		if(!mapInnerNode.Lookup(DataElem.elnod[1], nTemp)) nLastNode = DataElem.elnod[1];
// 	}
// 	
// 	for(i = 0; i < nElem; i++)  rData.aElemList[i] = 0;  // 멤버의 요소 리스트를 초기화한다.
// 	mapNodeiElem.Lookup(nStartNode, KeyElem);  // 멤버의 첫번째 요소를 찾는다.
// 	rData.aElemList[0] = KeyElem;    // 멤버의 첫번째 요소를 설정한다.  
// 	for(i = 0; i < nElem; i++)
// 	{
// 		m_pDoc->m_pAttrCtrl->GetElem(KeyElem, DataElem);
// 		if(i < nElem - 1) 
// 		{
// 			mapNodeiElem.Lookup(DataElem.elnod[1], KeyElem);
// 			rData.aElemList[i + 1] = KeyElem;
// 		}
// 		if(i == nElem - 1 && DataElem.elnod[1] == nLastNode) bSuccess = TRUE;
// 	}
// 	ASSERT(bSuccess);
// 
// 	delete []nNodeIndex;
}

void CDgnGenMembDlg::InitSelectedItem() // 선택된 요소와 절점을 해제한다.
{  
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CDgnGenMembDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDgnGenMembDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_MEMB_K Key;
	T_MEMB_D Data;

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_MEMB_ADD):
			{
				pViewBuff->GetMemb(nKey, Key, Data);
				InsertItem(Key);
			}
			break;
		case(UR_MEMB_DEL):
			{
				pViewBuff->GetMemb(nKey, Key, Data);
				DeleteItem(Key);
			}
			break;
		case(UR_MEMB_MFS):
		case(UR_MEMB_MFD):
			{
				pViewBuff->GetMemb(nKey, Key, Data);
				DeleteItem(Key);
				InsertItem(Key);        
			}
			break;
		default:
			break;
		}
	} 
}

void CDgnGenMembDlg::InsertItem(T_MEMB_K Key)
{
	// 삽입 위치를 찾는다.
	T_MEMB_D Data;
	int nSize = m_List.GetItemCount();
	if (!m_pDoc->m_pAttrCtrl->GetMemb(Key, Data)) return;
	CString strMembIndex;     
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
	m_List.SetItemText(nIndex, 1, strElemList);      
	m_List.SetItemData(nIndex, (DWORD)Key);
}

void CDgnGenMembDlg::DeleteItem(T_MEMB_K Key)
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

//=========================================================================
//============= Add/Replace 시 Member의 적합성 판단 관련 함수 =============
//=========================================================================
BOOL CDgnGenMembDlg::DelInferiorMember()
{
	T_ELEM_D DataElem;
	T_MEMB_K KeyMemb;
	T_MEMB_D DataMemb, rData;  
	CArray<T_ELEM_K, T_ELEM_K> arKeyElem;
	CArray<T_MEMB_K, T_MEMB_K> arMembDel;
	CArray<T_MEMB_K, T_MEMB_K> rKeyList;
	CMap<T_MEMB_K, T_MEMB_K, int, int> mapMembDel;    // 전체 멤버에 대해 삭제할 멤버
	CMap<T_MEMB_K, T_MEMB_K, T_MEMB_K, T_MEMB_K> mapSelMembKey; // 현재 선택된 요소가 속하는 멤버의 map
	DataElem.Initialize();
	DataMemb.Initialize();
	rData.Initialize();
	mapMembDel.InitHashTable(HASHSIZEMEMB);  
	mapSelMembKey.InitHashTable(HASHSIZEMEMB);
	arKeyElem.RemoveAll();
	arMembDel.RemoveAll();
	rKeyList.RemoveAll();
	mapMembDel.RemoveAll();
	mapSelMembKey.RemoveAll();
	BOOL bDelete = FALSE;  
	BOOL bContinuous = FALSE;
	int i, j, k, l, nTemp;
	
	int nNumSelFrmKey = m_aSelFrmKey.GetSize();	    
	for(i = 0; i < nNumSelFrmKey; i++)
	{
		if(m_pDoc->m_pAttrCtrl->GetMembAssigned(m_aSelFrmKey.GetAt(i), KeyMemb)) mapSelMembKey.SetAt(KeyMemb, KeyMemb);
	}

	POSITION pos = mapSelMembKey.GetStartPosition();
	while(pos)
	{
		mapSelMembKey.GetNextAssoc(pos, KeyMemb, KeyMemb);
		rKeyList.Add(KeyMemb);
	}
	
	int nCountKeyList = rKeyList.GetSize();
	for(l = 0; l < nCountKeyList; l++)
	{  
		bContinuous = FALSE;
		if(mapMembDel.Lookup(rKeyList[l], nTemp)) continue;  // 현재 멤버가 삭제할 멤버 리스트에 있으면 다음 멤버로 넘어감
		else   // 현재 멤버가 삭제할 멤버 리스트에 없으면 연속성 여부를 따져 삭제할지 판단함
		{
			m_pDoc->m_pAttrCtrl->GetMemb(rKeyList[l], rData);
			int nNumNode = 2 * rData.aElemList.GetSize();
			double dLength = 0., dAvgLength = 0.;
						
			int nMaxNum = rData.aElemList.GetSize()*2;
			if(nMaxNum <= 0) {ASSERT(0); continue;}
			int *nNodeIndex = new int[nMaxNum];   
			for(i = 0; i < nMaxNum; i++) nNodeIndex[i] = 0;    
			for(i = 0; i < rData.aElemList.GetSize(); i++)
			{    
				m_pDoc->m_pAttrCtrl->GetElem(rData.aElemList.GetAt(i), DataElem);
				{
					nNodeIndex[2 * i] = DataElem.elnod[0];
					nNodeIndex[2 * i + 1] = DataElem.elnod[1];
					dLength += m_pDoc->calcLAVElem(DataElem);  // 현재 Member의 전체 길이
				}    
			}
			dAvgLength = dLength / rData.aElemList.GetSize();   // 현재 Member를 이루는 요소의 평균 길이
			
			CArray<T_NODE_K, T_NODE_K> aInnerNode;
			aInnerNode.RemoveAll();
			for(i = 0; i < nNumNode; i++)    // 현재 Member의 내부 절점을 구한다.
			{
				for(j = i; j < nNumNode; j++)
				{      
					if(j != i && nNodeIndex[i] == nNodeIndex[j]) {aInnerNode.Add(nNodeIndex[i]); break;}
				}
			}
			
			double dLengthOld, dAvgLengthOld, dMinAvgLengthOld;          
			CMap<T_MEMB_K, T_MEMB_K, int, int> mapMembDelTemp;  
			mapMembDelTemp.InitHashTable(51);  
			mapMembDelTemp.RemoveAll();  // 각 멤버의 키에 대해 삭제할 멤버
			int nInnerNode = aInnerNode.GetSize(); 
			// 현재 멤버와 내부에서 교차하는 모든 기존 멤버를 비교하여 어느 멤버가 거더인지 확인한다.
			// 현재 멤버가 거더가 아니라고 판단되는 시점에서 작업을 멈추고 연속이 아님을 리턴한다.  
			// 만약, 현재 멤버가 거더이면 이와 교차하는 기존 멤버를 map에 모으고 추후 이의 멤버 설정을 해제한다.        
			for(i = 0; i < nInnerNode; i++)  
			{
				bContinuous = FALSE;        
				dAvgLengthOld = 0.;        // 멤버를 이루는 요소의 평균길이
				dMinAvgLengthOld = 9.e+6;  // 한 내부절점에 연결된 멤버를 이루는 요소의 평균길이의 최소값
				m_pDoc->m_pAttrCtrl->GetElemKeyListConnectedToNode(aInnerNode[i], arKeyElem);  // 현재 Member의 내부 절점에 연결된 요소리스트를 구한다.  
				for(j = 0; j < arKeyElem.GetSize(); j++)   // 각 내부 절점에 연결된 모든 요소에 대해
				{
					if(m_pDoc->m_pAttrCtrl->GetMembAssigned(arKeyElem.GetAt(j), KeyMemb))  // 요소가 멤버의 구성요소이면
					{ 
						if(!mapMembDelTemp.Lookup(KeyMemb, nTemp) &&       // 선택된 멤버가 삭제될 리스트에 없고
							KeyMemb != rKeyList[l] &&                        // 선택된 멤버가 현재 멤버와 다르고
							m_pDoc->m_pAttrCtrl->GetMemb(KeyMemb, DataMemb)) // 선택된 멤버의 멤버 데이터가 존재하면              
						{
							// 선택된 멤버가 내부 절점을 가지고 있고(두 개 이상의 요소로 이루어져 있고)
							// 이 내부 절점이 고려중인 내부 절점과 일치하면 작업을 계속한다.              
							if(DataMemb.aElemList.GetSize() >= 2)
							{
								BOOL bCross1 = FALSE;
								BOOL bCross2 = FALSE;              
								for(int m = 0; m < DataMemb.aElemList.GetSize(); m++)
								{
									if(m_pDoc->m_pAttrCtrl->GetElem(DataMemb.aElemList[m], DataElem))
									{
										if(DataElem.elnod[0] == aInnerNode[i]) bCross1 = TRUE;
										if(DataElem.elnod[1] == aInnerNode[i]) bCross2 = TRUE;
									}
								}
								if(!(bCross1 && bCross2)) continue;  // 선택된 멤버의 내부절점과 고려중인 멤버의 내부절점이 일치하지 않으면 선택된 멤버에 대한 작업을 중지한다.
							
								mapMembDelTemp.SetAt(KeyMemb, 0);     // 선택된 멤버를 삭제될 리스트에 넣는다.      
								dLengthOld = 0.;                      // 선택된 멤버의 길이를 초기화 한다.
								for(k = 0; k < DataMemb.aElemList.GetSize(); k++) // 기존 Member의 전체 길이를 구한다.
								{
									if(m_pDoc->m_pAttrCtrl->GetElem(DataMemb.aElemList[k], DataElem))
										dLengthOld += m_pDoc->calcLAVElem(DataElem);
								}
								dAvgLengthOld = dLengthOld / DataMemb.aElemList.GetSize(); // 선택된 멤버의 요소의 평균길이를 구한다.
								dMinAvgLengthOld = min(dAvgLengthOld, dMinAvgLengthOld);   // 기존 멤버를 이루는 요소의 평균길이의 최소값
							}
						}
					}
				}
				// 현재의 내부 절점에 연결된 모든 멤버에 대해 
				// 현재 멤버의 평균길이가 기존 멤버의 평균길이보다 짧으면 현재 멤버는 거더(연속부재)이다. 
				if(dAvgLength <= dMinAvgLengthOld) bContinuous = TRUE;
			}
			if(bContinuous) // 현재 멤버가 거더이면 현재 멤버와 내부에서 교차하는 다른 멤버(보)를 삭제할 멤버 map에 모은다.
			{
				POSITION pos = mapMembDelTemp.GetStartPosition();
				while(pos != NULL)
				{
					mapMembDelTemp.GetNextAssoc(pos, KeyMemb, nTemp);  // 각 멤버에 대해 삭제할 멤버르 하나씩 불러온다.
					mapMembDel.SetAt(KeyMemb, KeyMemb); // 각 멤버에 대해 삭제할 멤버를 전체 멤버에 대해 삭제할 멤버의 map에 모은다.
				}
				bDelete = TRUE;      // return 값 설정
			}
			delete []nNodeIndex; 
		}    
	} 
	// 현재 Member가 연속이면(거더이면) 이 Member와 내부에서 교차하는 다른 Member(보)를 해제한다.
	if(bDelete) // 삭제할 멤버(보)가 존재하면
	{
		arMembDel.RemoveAll();
		POSITION pos = mapMembDel.GetStartPosition();
		while(pos != NULL)
		{
			mapMembDel.GetNextAssoc(pos, KeyMemb, nTemp);
			if(m_pDoc->m_pAttrCtrl->ExistMemb(KeyMemb))  arMembDel.Add(KeyMemb);
		}
		m_pDoc->m_pDataCtrl->DelMemb(arMembDel);  // 삭제할 멤버(보)를 삭제한다.
	}
	return bDelete;
}

// 선택된 모든 요소중에서 멤버로 지정되지 않은 요소들을
// 하나씩 다시 멤버로 지정하는 함수
BOOL CDgnGenMembDlg::AddSingleElemMember()
{
	T_MEMB_K KeyMemb;
	T_ELEM_K KeyElem;
	T_MEMB_D DataMemb;
	CArray<T_MEMB_D, T_MEMB_D&> aDataMemb;
	aDataMemb.RemoveAll();  
	
	int nNumSelFrmKey = m_aSelFrmKey.GetSize();	    
	for(int i = 0; i < nNumSelFrmKey; i++) 
	{
		DataMemb.Initialize();
		KeyElem = m_aSelFrmKey[i];
		if(!m_pDoc->m_pAttrCtrl->GetMembAssigned(KeyElem, KeyMemb))  // 멤버로 지정되지 않은 요소만 멤버로 지정
		{
			DataMemb.aElemList.Add(KeyElem);
			aDataMemb.Add(DataMemb);
		}
	}
	if(!m_pDoc->m_pDataCtrl->AddMemb(aDataMemb)) return FALSE;
	return TRUE;
}

// 아래층에 기둥이나 벽체가 있는지 확인하는 함수
BOOL CDgnGenMembDlg::IsExistColumnWallLower(T_NODE_K KeyNode, CArray<T_ELEM_K, T_ELEM_K>& aConnectedElem)
{
	T_NODE_D DataNode;
	T_ELEM_K KeyElem;
	T_ELEM_D DataElem;
	DataNode.Initialize();

	double dLevelRef, dLevelNew, dLevelNew0, dLevelNew1, dLevelNew2, dLevelNew3;
		
	if(!m_pDoc->m_pAttrCtrl->GetNode(KeyNode, DataNode)) return FALSE;
	dLevelRef = DataNode.z;

	CDgnDataCtrl DataCtrl;
	for(int i = 0; i < aConnectedElem.GetSize(); i++)
	{
		KeyElem = aConnectedElem.GetAt(i);
		DataElem.Initialize();
				
		if(m_pDoc->m_pAttrCtrl->GetElem(KeyElem, DataElem))
		{
			if(m_pDoc->m_pAttrCtrl->IsFrameType(DataElem.eltyp))
			{
				if(m_pDoc->m_pAttrCtrl->GetNode(DataElem.elnod[1], DataNode)) dLevelNew0 = DataNode.z;
				else return FALSE;
				if(m_pDoc->m_pAttrCtrl->GetNode(DataElem.elnod[0], DataNode)) dLevelNew1 = DataNode.z;
				else return FALSE;
				dLevelNew = min(dLevelNew0, dLevelNew1);
				if(dLevelNew < dLevelRef && DataCtrl.Get_DgnGenMbtp(KeyElem) == D_MBTP_COLUMN) return TRUE;  // 아래층에 기둥이 있으면
			}
			else if(m_pDoc->m_pAttrCtrl->IsWall(DataElem.eltyp))
			{
				if(m_pDoc->m_pAttrCtrl->GetNode(DataElem.elnod[0], DataNode)) dLevelNew0 = DataNode.z;
				else return FALSE;
				if(m_pDoc->m_pAttrCtrl->GetNode(DataElem.elnod[1], DataNode)) dLevelNew1 = DataNode.z;
				else return FALSE;
				if(m_pDoc->m_pAttrCtrl->GetNode(DataElem.elnod[2], DataNode)) dLevelNew2 = DataNode.z;
				else return FALSE;
				if(m_pDoc->m_pAttrCtrl->GetNode(DataElem.elnod[3], DataNode)) dLevelNew3 = DataNode.z;
				else return FALSE;
				dLevelNew = min(dLevelNew0, dLevelNew1);
				dLevelNew = min(dLevelNew, dLevelNew2);
				dLevelNew = min(dLevelNew, dLevelNew3);
				if(dLevelNew < dLevelRef) return TRUE;  // 아래층에 벽체가 있으면
			}
			else return FALSE;
		}
	}
	return FALSE;
}

// 현재 절점이 층에 속하는지를 확인하는 함수
BOOL CDgnGenMembDlg::IsStoryLevel(T_NODE_K KeyNode)
{
	T_NODE_D DataNode;
	T_STOR_D DataStor;
	CArray<T_STOR_K, T_STOR_K> aStorKeyList;  // Story level로 정열된 리스트
	
	m_pDoc->m_pAttrCtrl->GetNode(KeyNode, DataNode);
	m_pDoc->m_pAttrCtrl->GetStorKeyList(aStorKeyList);
	double dTol = 0.00001;

	for(int i = 0; i < aStorKeyList.GetSize(); i++)
	{
		if(m_pDoc->m_pAttrCtrl->GetStor(aStorKeyList.GetAt(i), DataStor))
			if(fabs(DataStor.dStoryLevel - DataNode.z) < dTol) return TRUE;
	}
	return FALSE;
}

BOOL CDgnGenMembDlg::IsMatchDirectionAttribute(T_ELEM_K KeyRef, T_ELEM_K KeyNew)
{  
	if(m_nAssignType == 1 && !CompareElemDirection(KeyRef, KeyNew)) return FALSE;  // Automatic인 경우만 요소의 직선여부를 체크한다. 
	if(!CompareElemAttribute(KeyRef, KeyNew)) return FALSE;
	return TRUE;
}

BOOL CDgnGenMembDlg::CompareElemDirection(T_ELEM_K KeyRef, T_ELEM_K KeyNew)
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

BOOL CDgnGenMembDlg::CompareElemAttribute(T_ELEM_K KeyRef, T_ELEM_K KeyNew)
{
	CDgnDataCtrl DataCtrl;
	T_ELEM_D DataElemRef, DataElemNew;
	T_SECT_D DataSectRef, DataSectNew;
	
	if(m_pDoc->m_pAttrCtrl->GetElem(KeyRef, DataElemRef) && m_pDoc->m_pAttrCtrl->GetElem(KeyNew, DataElemNew))
	{ 
		if(DataElemRef.eltyp != DataElemNew.eltyp) {m_nFailureType = 1; return FALSE;}// 타입
		if(DataElemRef.elmat != DataElemNew.elmat) {m_nFailureType = 2; return FALSE;}// 재질

		m_pDoc->m_pAttrCtrl->GetSect(DataElemRef.elpro, DataSectRef);
		m_pDoc->m_pAttrCtrl->GetSect(DataElemNew.elpro, DataSectNew); // Same Section 이지만, Tapered Sect인경우 
//     if (DataSectRef.nStype==D_SECT_TYPE_TAPERED || DataSectNew.nStype==D_SECT_TYPE_TAPERED)
//     {m_nFailureType = 4; return FALSE;}

		if (CProduct::GetMovingType() == D_PRODUCT_MOVING_CH && m_wndTaperedMemb.GetCheck())
		{
			if (DataSectRef.SectBefore.Shape != DataSectNew.SectBefore.Shape) 
			{ 
				m_nFailureType = 3; 
				return FALSE; 
			}
		}
		else
			if (DataElemRef.elpro != DataElemNew.elpro) { m_nFailureType = 3; return FALSE; }// 단면

		
		if(fabs(DataElemRef.angle - DataElemNew.angle - (int)((DataElemRef.angle - DataElemNew.angle)/360.)*360.) > 1.0e-6 &&
			 fabs(DataElemRef.angle - (int)(DataElemRef.angle/360.)*360. - DataElemNew.angle + (int)(DataElemNew.angle/360.)*360.) > 1.0e-6) 
	  {m_nFailureType = 5; return FALSE;}// beta angle
		if(DataCtrl.Get_DgnGenMbtp(KeyRef) != DataCtrl.Get_DgnGenMbtp(KeyNew)) 
		{m_nFailureType = 6; return FALSE;}// member type
	}
	else return FALSE;
	return TRUE;
}

void CDgnGenMembDlg::GetFrmDirectionVector(T_ELEM_D& DataElem, double& Ux, double& Uy, double& Uz)
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


// 수평부재(보나 브레이스)에서 Z 방향의 구속이나 스프링이 있고
// 상부에 수직부재(기둥, 벽체)가 있을 경우 member를 분할한다.
BOOL CDgnGenMembDlg::IsSupportAndColumnWallUpperExist(T_ELEM_K KeyElem, T_NODE_K KeyNode)
{
	if(IsSupportExist(KeyElem, KeyNode) && IsExistColumnWallUpper(KeyNode)) 
		return TRUE;
	else 
		return FALSE;
}

// 수평 부재(보나 가새)의 절점에 Z 방향의 스프링이나 구속이 있는지 확인하는 함수
BOOL CDgnGenMembDlg::IsSupportExist(T_ELEM_K KeyElem, T_NODE_K KeyNode)
{
	T_CONS_D DataCons;
	//T_NSPR_D DataNspr;
	T_GSPR_D DataGspr;
	T_GSTP_D DataGstp;  
	CArray<T_GSPR_K, T_GSPR_K> aGsprKey; 
	CArray<T_GSPR_D, T_GSPR_D&> aGsprData; 
	CDgnDataCtrl DataCtrl;
	
	if(DataCtrl.Get_DgnGenMbtp(KeyElem) == D_MBTP_BEAM || DataCtrl.Get_DgnGenMbtp(KeyElem) == D_MBTP_BRACE)
	{
		if(m_pDoc->m_pAttrCtrl->GetConsNode(KeyNode, DataCons))
		{
			if(DataCons.Constraint[2] == '1') return TRUE;    // Z 방향의 구속이 있음
		}
		CArray<T_NSPR_K,T_NSPR_K> arNsprKeyList;
		CArray<T_NSPR_D,T_NSPR_D&> arNsprDataList;
		m_pDoc->m_pAttrCtrl->GetNsprNode(KeyNode,arNsprKeyList,arNsprDataList);
		if(arNsprKeyList.GetSize()>0)
		{
			for(int i=0;i<arNsprDataList.GetSize();i++)
			{
				if(arNsprDataList[i].Spring[2] != 0.) return TRUE;         // Z 방향의 스프링이 있음
			}
			
		}
		m_pDoc->m_pAttrCtrl->GetGsprNode(KeyNode, aGsprKey, aGsprData);
		if(aGsprKey.GetSize() > 0)
		{
			for(int i = 0; i < aGsprKey.GetSize(); i++)
			{
				if(!m_pDoc->m_pAttrCtrl->GetGspr(aGsprKey.GetAt(i), DataGspr)) continue;
				if(!m_pDoc->m_pAttrCtrl->GetGstp(DataGspr.SpringTypeKey, DataGstp)) continue;
				if(DataGstp.Spring[2] != 0.) return TRUE;   // Z 방향의 스프링이 있음          
			}
		}
	}


			
	return FALSE;  // Z 방향의 구속이나 스프링이 없으면 member 분할하지 않음
}

// 특정 절점의 상부에 기둥이나 벽체가 있는지 확인하는 함수
BOOL CDgnGenMembDlg::IsExistColumnWallUpper(T_NODE_K KeyNode)
// T_NODE_K KeyNode, CArray<T_ELEM_K, T_ELEM_K>& aConnectedElem)
{  
	T_NODE_D DataNode;
	T_ELEM_K KeyElem;
	T_ELEM_D DataElem;
	DataNode.Initialize();
	
	CArray<T_ELEM_K, T_ELEM_K> aConnectedElem;
	aConnectedElem.RemoveAll();
	double dLevelRef, dLevelNew, dLevelNew0, dLevelNew1, dLevelNew2, dLevelNew3;
	
	m_pDoc->m_pAttrCtrl->GetElemKeyListConnectedToNode(KeyNode, aConnectedElem);
		
	if(!m_pDoc->m_pAttrCtrl->GetNode(KeyNode, DataNode)) return FALSE;
	dLevelRef = DataNode.z;

	CDgnDataCtrl DataCtrl;
	for(int i = 0; i < aConnectedElem.GetSize(); i++)
	{
		KeyElem = aConnectedElem.GetAt(i);
		DataElem.Initialize();
				
		if(m_pDoc->m_pAttrCtrl->GetElem(KeyElem, DataElem))
		{
			if(m_pDoc->m_pAttrCtrl->IsFrameType(DataElem.eltyp))
			{
				if(m_pDoc->m_pAttrCtrl->GetNode(DataElem.elnod[1], DataNode)) dLevelNew0 = DataNode.z;
				else return FALSE;
				if(m_pDoc->m_pAttrCtrl->GetNode(DataElem.elnod[0], DataNode)) dLevelNew1 = DataNode.z;
				else return FALSE;
				dLevelNew = max(dLevelNew0, dLevelNew1);
				if(dLevelNew > dLevelRef && DataCtrl.Get_DgnGenMbtp(KeyElem) == D_MBTP_COLUMN) return TRUE;  // 윗층에 기둥이 있으면
			}
			else if(m_pDoc->m_pAttrCtrl->IsWall(DataElem.eltyp))
			{
				if(m_pDoc->m_pAttrCtrl->GetNode(DataElem.elnod[0], DataNode)) dLevelNew0 = DataNode.z;
				else return FALSE;
				if(m_pDoc->m_pAttrCtrl->GetNode(DataElem.elnod[1], DataNode)) dLevelNew1 = DataNode.z;
				else return FALSE;
				if(m_pDoc->m_pAttrCtrl->GetNode(DataElem.elnod[2], DataNode)) dLevelNew2 = DataNode.z;
				else return FALSE;
				if(m_pDoc->m_pAttrCtrl->GetNode(DataElem.elnod[3], DataNode)) dLevelNew3 = DataNode.z;
				else return FALSE;
				dLevelNew = max(dLevelNew0, dLevelNew1);
				dLevelNew = max(dLevelNew, dLevelNew2);
				dLevelNew = max(dLevelNew, dLevelNew3);
				if(dLevelNew > dLevelRef) return TRUE;  // 윗층에 벽체가 있으면
			}
			else return FALSE;
		}
	}
	return FALSE;
}

//================================================================
//================= List 관련 작업 함수 ==========================
//================================================================
void CDgnGenMembDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_DGN_CHK_INDEX), _LS(IDS_DGN_CHK_ELEM_LIST)};  // _T("Index"), _T("Element List")
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

void CDgnGenMembDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountMemb();
	if(nItemCount == 0) return;

	POSITION pos;
	int nCount, nIndex;  

	if(nItemCount)
	{
		T_MEMB_K Key;
		T_MEMB_D Data;
		T_ELEM_K KeyElem;
		CString strMembIndex, strElemList;;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartMemb();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextMemb(pos, Key, Data);		  
			KeyBuf[nCount++] = Data.aElemList[0];
		}
		qsort(KeyBuf, nItemCount, sizeof(T_ELEM_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  KeyElem = KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetMembAssigned(KeyElem, Key);
			m_pDoc->m_pAttrCtrl->GetMemb(Key, Data);
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
				csItem.Format(_T("%d"), Data.aElemList[Data.aElemList.GetSize()-1]);
				strElemList += csItem;
			}
			m_List.SetItemText(nIndex, 1, strElemList);		
			m_List.SetItemData(nIndex, (DWORD)Key);
		}
		delete []KeyBuf;
	}
}
			
void CDgnGenMembDlg::OnDelKeyPressedInList()
{  
	CArray<T_MEMB_K, T_MEMB_K> aKeyMemb;
	CArray<int, int> aSelItem;
	aSelItem.RemoveAll();
	aKeyMemb.RemoveAll();  

	GetSelectedItemList(&m_List, aSelItem);
	int nSize = aSelItem.GetSize();
	if(nSize == 0) return;

	for(int i = 0; i < nSize; i++)
	{
		DWORD(KeyMemb) = m_List.GetItemData(aSelItem[i]);    
		if(m_pDoc->m_pAttrCtrl->ExistMemb(KeyMemb)) aKeyMemb.Add(KeyMemb);
	}  
	if(m_pDoc->m_pDataCtrl->DelMemb(aKeyMemb)) InitSelectedItem();

	SetListFocus(aSelItem);
}

// member를 삭제한 경우 List 박스의 Focus를 다시 맞춤
void CDgnGenMembDlg::SetListFocus(CArray<int, int>& aSelItem)
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

BOOL CDgnGenMembDlg::PreTranslateMessage(MSG* pMsg) 
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

void CDgnGenMembDlg::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
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
void CDgnGenMembDlg::DisplaySelElemList()
{
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelElem;  
	aSelElem.RemoveAll();
	GetSelElemList(aSelElem);  
	m_pDoc->m_pViewCtrl->SelectElem(NULL, aSelElem, FALSE, TRUE);
}

void CDgnGenMembDlg::GetSelElemList(CArray<T_ELEM_K, T_ELEM_K>& aSelElem)
{
	T_MEMB_D DataMemb;
	CArray<int, int> aSelItem;
	CArray<T_MEMB_K, T_MEMB_K> aMembKey;
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
	m_pDoc->m_pAttrCtrl->GetKeyListMembUser(&aMembKey, 0, &aSelElem);
}      

void CDgnGenMembDlg::GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem)
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

/*
void CDgnGenMembDlg::GetSelElemList(CArray<T_ELEM_K, T_ELEM_K>& aSelElem)
{
	T_MEMB_D DataMemb;
	CArray<int, int> aSelItem;
	aSelItem.RemoveAll();
	aSelElem.RemoveAll();    

	GetSelectedItemList(&m_List, aSelItem);

	int nSize = aSelItem.GetSize();
	if(nSize == 0) return;
	for(int i = 0; i < nSize; i++)
	{
		DWORD(KeyMemb) = m_List.GetItemData(aSelItem[i]);    
		DataMemb.Initialize();
		if(m_pDoc->m_pAttrCtrl->GetMemb(KeyMemb, DataMemb))
		{
			for(int j = 0; j < DataMemb.aElemList.GetSize(); j++)        
				aSelElem.Add(DataMemb.aElemList[j]);        
		}        
	}  
}

//================================================================
//================= List 관련 작업 함수 ==========================
//================================================================
void CDgnGenMembDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_DGN_CHK_INDEX), _LS(IDS_DGN_CHK_ELEM_LIST)};  // _T("Index"), _T("Element List")
	int nColWidth[] = {50, 130};
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

void CDgnGenMembDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountMemb();
	if(nItemCount == 0) return;

	POSITION pos;
	int nCount;  

	if(nItemCount)
	{
		T_MEMB_K Key;
		T_MEMB_D Data;
		T_ELEM_K KeyElem;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartMemb();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextMemb(pos, Key, Data);
		  KeyBuf[nCount++] = Data.aElemList[0];
			KeyBuf[nCount++] = Data.aElemList[0];
		}
		qsort(KeyBuf, nItemCount, sizeof(T_ELEM_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  KeyElem=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetMembAssigned(KeyElem, Key);
			m_pDoc->m_pAttrCtrl->GetMemb(Key,Data);      
			CString strMember;
			strMember.Format(_T("%d"), Data.aElemList[0]);
			int nIndex = m_List.InsertItem(nCount, strMember);
			CString strElemList;
			CNumericOptimizer optimizer;
			strElemList = optimizer.Optimize((long*)Data.aElemList.GetData(), Data.aElemList.GetSize());
			m_List.SetItemText(nIndex, 1, strElemList);		
		}
		delete []KeyBuf;
	}
}

void CDgnGenMembDlg::OnDelKeyPressedInList()
{  
	CArray<T_MEMB_K, T_MEMB_K> aKeyMemb;
	CArray<int, int> aSelItem;
	T_ELEM_K KeyElem;
	T_MEMB_K KeyMemb;
	aKeyMemb.RemoveAll();
	CString strKeyElem;

	GetSelectedItemList(&m_List, aSelItem);
	int nSize = aSelItem.GetSize();
	if(nSize == 0) return;
	for(int i = 0; i < nSize; i++)
	{
		strKeyElem = m_List.GetItemText(aSelItem[i], 0);
		KeyElem = _ttoi(strKeyElem);
		if(m_pDoc->m_pAttrCtrl->GetMembAssigned(KeyElem, KeyMemb)) aKeyMemb.Add(KeyMemb);    
	}  
	if(m_pDoc->m_pDataCtrl->DelMemb(aKeyMemb)) InitSelectedItem();
}

BOOL CDgnGenMembDlg::PreTranslateMessage(MSG* pMsg) 
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

void CDgnGenMembDlg::GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem)
{
	int  nItem = -1;
	// Update all of the selected items.
	UINT uSelectedCount = pList->GetSelectedCount();
	aSelItem.SetSize(uSelectedCount);
	for (UINT i=0; i < uSelectedCount; i++)
	{
		nItem = pList->GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		aSelItem[i] = nItem;
	}
}

void CDgnGenMembDlg::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;	 
	DisplaySelElemList();   // 선택된 요소를 Display한다.  
	*pResult = 0;
}

//  CButton* SelBtn = (CButton*)GetDlgItem(IDC_DGN_MEMB_CHG_SELECT_SEL);  
void CDgnGenMembDlg::DisplaySelElemList()
{
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelElem;  
	aSelElem.RemoveAll();
	GetSelElemList(aSelElem);  
	m_pDoc->m_pViewCtrl->SelectElem(NULL, aSelElem, FALSE, TRUE);
}

void CDgnGenMembDlg::GetSelElemList(CArray<T_ELEM_K, T_ELEM_K>& aSelElem)
{
	T_ELEM_K KeyElem;
	T_MEMB_K KeyMemb;
	T_MEMB_D DataMemb;
	CArray<int, int> aSelItem;
	aSelItem.RemoveAll();
	aSelElem.RemoveAll();  
	CString strKeyElem = _T("");

	GetSelectedItemList(&m_List, aSelItem);

	int nSize = aSelItem.GetSize();
	if(nSize == 0) return;
	for(int i = 0; i < nSize; i++)
	{
		strKeyElem = m_List.GetItemText(aSelItem[i], 0);
		KeyElem = _ttoi(strKeyElem);
		if(m_pDoc->m_pAttrCtrl->GetMembAssigned(KeyElem, KeyMemb)) 
		{
			DataMemb.Initialize();
			if(m_pDoc->m_pAttrCtrl->GetMemb(KeyMemb, DataMemb))
			{
				for(int j = 0; j < DataMemb.aElemList.GetSize(); j++)        
					aSelElem.Add(DataMemb.aElemList[j]);        
			}
		}    
	}  
}


*/
/*
BOOL CDgnGenMembDlg::IsContinuousMember(T_MEMB_D& rData)
{
	T_ELEM_D DataElem;
	T_MEMB_K KeyMemb;
	T_MEMB_D DataMemb;
	CArray<T_NODE_K, T_NODE_K> aInnerNode;
	CArray<T_ELEM_K, T_ELEM_K> arKeyElem;
	CArray<T_MEMB_K, T_MEMB_K> arKeyMemb;
	DataElem.Initialize();
	DataMemb.Initialize();
	aInnerNode.RemoveAll();
	arKeyElem.RemoveAll();
	arKeyMemb.RemoveAll();
	BOOL bContinuous = TRUE;
		
	int nNumNode = 2 * rData.aElemList.GetSize();
	double dLengthNew = 0., dAvgLengthNew = 0.;
	const int nMaxNum = 100;
	int i, j, k;
	int nNodeIndex[nMaxNum];
	for(i = 0; i < nMaxNum; i++) nNodeIndex[i] = 0;
		
	for(i = 0; i < rData.aElemList.GetSize(); i++)
	{    
		m_pDoc->m_pAttrCtrl->GetElem(rData.aElemList.GetAt(i), DataElem);
		{
			nNodeIndex[2 * i] = DataElem.elnod[0];
			nNodeIndex[2 * i + 1] = DataElem.elnod[1];
			dLengthNew += m_pDoc->calcLAVElem(DataElem);  // 새로 들어온 Member의 전체 길이
		}    
	}
	dAvgLengthNew = dLengthNew / rData.aElemList.GetSize();   // 새로 들어온 Member를 이루는 요소의 평균 길이

	for(i = 0; i < nNumNode; i++)    // 새로 들어온 Member의 내부 절점을 구한다.
	{
		for(j = 0; j < nNumNode; j++)
		{      
			if(j != i && nNodeIndex[i] == nNodeIndex[j]) aInnerNode.Add(nNodeIndex[i]);
		}
	}
	int nInnerNode = aInnerNode.GetSize();  
	ASSERT(nInnerNode >= 1);   // Member는 최소 두 개의 요소로 구성되므로 내부 절점이 1개 이상이다.
	
	CMap<T_MEMB_K, T_MEMB_K, T_MEMB_K, T_MEMB_K> mapMemb;  
	mapMemb.InitHashTable(51);  
	mapMemb.RemoveAll();  
	double dLengthOld = 0.;
	double dAvgLengthOld = 0.;
	double dMinAvgLengthOld = 9.e+6;
	
	// 새로 들어온 멤버와 내부에서 교차하는 모든 기존 멤버를 비교하여 어느 멤버가 거더인지 확인한다.
	// 새로 들어온 멤버가 거더가 아니라고 판단되는 시점에서 작업을 멈추고 연속이 아님을 리턴한다.  
	// 만약, 새로 들어온 멤버가 거더이면 이와 교차하는 기존 멤버의 멤버 설정을 해제한다.  
	for(i = 0; i < nInnerNode; i++)  
	{
		m_pDoc->m_pAttrCtrl->GetElemKeyListConnectedToNode(aInnerNode.GetAt(i), arKeyElem);  // 새로 들어온 Member의 내부 절점에 연결된 요소리스트를 구한다.  
		for(j = 0; j < arKeyElem.GetSize(); j++)   // 각 내부 절점에 연결된 모든 요소에 대해
		{
			if(m_pDoc->m_pAttrCtrl->GetMembAssigned(arKeyElem.GetAt(j), KeyMemb))  // 요소가 멤버의 구성요소이면
			{        
				if(m_pDoc->m_pAttrCtrl->GetMemb(KeyMemb, DataMemb))  // 멤버 데이터가 존재하면
				{
					mapMemb.SetAt(KeyMemb, KeyMemb);    // 새로 들어온 멤버가 거더인 경우 기존 멤버를 해제하기 위하여 기존 멤버의 키를 보관한다.
					for(k = 0; k < DataMemb.aElemList.GetSize(); k++) // 기존 Member의 전체 길이를 구한다.
					{
						if(m_pDoc->m_pAttrCtrl->GetElem(DataMemb.aElemList.GetAt(k), DataElem))
							dLengthOld += m_pDoc->calcLAVElem(DataElem);  
					}
					dAvgLengthOld = dLengthOld / DataMemb.aElemList.GetSize(); // 기존 Member를 이루는 요소의 평균 길이          
					dMinAvgLengthOld = min(dAvgLengthOld, dMinAvgLengthOld);   // 기존 멤버를 이루는 요소의 평균길이의 최소값
				}
			}
		}
		// 새 멤버의 평균 길이가 기존 멤버의 평균길이보다 길면 새 멤버는 거더(연속부재)가 아니다. 
		if(dAvgLengthNew >= dMinAvgLengthOld) {bContinuous = FALSE; break;}      
	}
	
	// 새로 들어온 Member가 연속이면(거더이면) 이 Member와 내부에서 교차하는 다른 Member(보)를 해제한다.
	if(bContinuous) 
	{
		arKeyMemb.RemoveAll();
		POSITION pos = mapMemb.GetStartPosition();
		while(pos != NULL)
		{
			mapMemb.GetNextAssoc(pos, KeyMemb, KeyMemb);
			if(m_pDoc->m_pAttrCtrl->ExistMemb(KeyMemb))  arKeyMemb.Add(KeyMemb);
		}
		m_pDoc->m_pDataCtrl->DelMemb(arKeyMemb);
	}
	return bContinuous;
}

// 멤버로 선정된 후보에 대해 거더와 보를 구분하여 보는 후보에서 제외하고
// 단일 요소 멤버는 새로이 추가하는 작업을 하는 함수   
BOOL CDgnGenMembDlg::GetFinalSurvivedMember(CArray<T_MEMB_D, T_MEMB_D&>& aDataMemb)
{  
	if(m_nAssignType == 1) // Automatic일 경우 보와 거더를 판단하여 보를 멤버에서 해제
	{
		if(!GetSuperiorMember(aDataMemb)) return FALSE;
	}
	if(!AddSingleElemMember(aDataMemb)) return FALSE;  // 단일 요소 멤버 추가
	return TRUE;
}

BOOL CDgnGenMembDlg::GetSuperiorMember(CArray<T_MEMB_D, T_MEMB_D&>& aDataMemb)
{
	T_ELEM_D DataElem;
	T_MEMB_D DataMemb, rData; 
	T_MEMB_K KeyMemb;
	CArray<T_ELEM_K, T_ELEM_K> arKeyElem;
	CArray<int, int> arMembDel;
	CMap<int, int, int, int> mapMembDel;  // 전체 멤버에 대해 삭제할 멤버  
	DataElem.Initialize();
	DataMemb.Initialize();
	rData.Initialize();
	mapMembDel.InitHashTable(HASHSIZEMEMB);  
	arKeyElem.RemoveAll();
	arMembDel.RemoveAll();  
	mapMembDel.RemoveAll();  
	BOOL bDelete = FALSE;  
	BOOL bContinuous = FALSE;
	int i, j, k, l, nTemp;
	int nKeyMemb, nKeyMembAssigned;
	
	int nCountMembList = aDataMemb.GetSize();

	CMap<T_ELEM_K, T_ELEM_K, int, int> mapMembAssigned;  // 요소가 속한 멤버의 인덱스  
	mapMembAssigned.InitHashTable(HASHSIZEELEM);
	mapMembAssigned.RemoveAll();  
	for(int nIndex = 0; nIndex < nCountMembList; nIndex++)
	{    
		for(int n = 0; n < aDataMemb.GetAt(nIndex).aElemList.GetSize(); n++)
			mapMembAssigned.SetAt(aDataMemb.GetAt(nIndex).aElemList[n], nIndex);
	}

	for(l = 0; l < nCountMembList; l++)
	{  
		bContinuous = FALSE;
		nKeyMemb = l;
		if(mapMembDel.Lookup(nKeyMemb, nTemp)) continue;  // 현재 멤버가 삭제할 멤버 리스트에 있으면 다음 멤버로 넘어감
		else   // 현재 멤버가 삭제할 멤버 리스트에 없으면 연속성 여부를 따져 삭제할지 판단함
		{      
			rData = aDataMemb.GetAt(l);
			int nNumNode = 2 * rData.aElemList.GetSize();
			double dLength = 0., dAvgLength = 0.;
			const int nMaxNum = 100;
	
			int nNodeIndex[nMaxNum];
			for(i = 0; i < nMaxNum; i++) nNodeIndex[i] = 0;    
			for(i = 0; i < rData.aElemList.GetSize(); i++)
			{    
				m_pDoc->m_pAttrCtrl->GetElem(rData.aElemList.GetAt(i), DataElem);
				{
					nNodeIndex[2 * i] = DataElem.elnod[0];
					nNodeIndex[2 * i + 1] = DataElem.elnod[1];
					dLength += m_pDoc->calcLAVElem(DataElem);  // 현재 Member의 전체 길이
				}    
			}
			dAvgLength = dLength / rData.aElemList.GetSize();   // 현재 Member를 이루는 요소의 평균 길이
			
			CArray<T_NODE_K, T_NODE_K> aInnerNode;
			aInnerNode.RemoveAll();
			for(i = 0; i < nNumNode; i++)    // 현재 Member의 내부 절점을 구한다.
			{
				for(j = i; j < nNumNode; j++)
				{      
					if(j != i && nNodeIndex[i] == nNodeIndex[j]) {aInnerNode.Add(nNodeIndex[i]); break;}
				}
			}
			
			double dLengthOld, dAvgLengthOld, dMinAvgLengthOld;          
			CMap<int, int, int, int> mapMembDelTemp;  // 각 멤버에 대해 삭제할 멤버의 인덱스를 모은다
			mapMembDelTemp.InitHashTable(51);  
			mapMembDelTemp.RemoveAll();  
			int nInnerNode = aInnerNode.GetSize(); 
			// 현재 멤버와 내부에서 교차하는 모든 기존 멤버를 비교하여 어느 멤버가 거더인지 확인한다.
			// 현재 멤버가 거더가 아니라고 판단되는 시점에서 작업을 멈추고 연속이 아님을 리턴한다.  
			// 만약, 현재 멤버가 거더이면 이와 교차하는 기존 멤버를 map에 모으고 추후 이의 멤버 설정을 해제한다.        
			for(i = 0; i < nInnerNode; i++)  
			{
				bContinuous = FALSE;
				dAvgLengthOld = 0.;        // 멤버를 이루는 요소의 평균길이
				dMinAvgLengthOld = 9.e+6;  // 한 내부절점에 연결된 멤버를 이루는 요소의 평균길이의 최소값
				m_pDoc->m_pAttrCtrl->GetElemKeyListConnectedToNode(aInnerNode[i], arKeyElem);  // 현재 Member의 내부 절점에 연결된 요소리스트를 구한다.  
				for(j = 0; j < arKeyElem.GetSize(); j++)   // 각 내부 절점에 연결된 모든 요소에 대해
				{
					if(mapMembAssigned.Lookup(arKeyElem.GetAt(j), nKeyMembAssigned))    // 요소가 멤버의 구성요소이면             
					{ 
						if(!mapMembDelTemp.Lookup(nKeyMembAssigned, nTemp) &&  // 선택된 멤버가 삭제될 리스트에 없고
							nKeyMembAssigned != nKeyMemb)                        // 선택된 멤버가 현재 멤버와 다르면              
						{
							// 선택된 멤버가 내부 절점을 가지고 있고(두 개 이상의 요소로 이루어져 있고)
							// 이 내부 절점이 고려중인 내부 절점과 일치하면 작업을 계속한다.              
							DataMemb = aDataMemb.GetAt(nKeyMembAssigned);
							if(DataMemb.aElemList.GetSize() >= 2)
							{
								BOOL bCross1 = FALSE;
								BOOL bCross2 = FALSE;              
								for(int m = 0; m < DataMemb.aElemList.GetSize(); m++)
								{
									if(m_pDoc->m_pAttrCtrl->GetElem(DataMemb.aElemList[m], DataElem))
									{
										if(DataElem.elnod[0] == aInnerNode[i]) bCross1 = TRUE;
										if(DataElem.elnod[1] == aInnerNode[i]) bCross2 = TRUE;
									}
								}
								if(!(bCross1 && bCross2)) continue;  // 선택된 멤버의 내부절점과 고려중인 멤버의 내부절점이 일치하지 않으면 선택된 멤버에 대한 작업을 중지한다.
							
								mapMembDelTemp.SetAt(nKeyMembAssigned, 0);     // 선택된 멤버를 삭제될 리스트에 넣는다.      
								dLengthOld = 0.;                      // 선택된 멤버의 길이를 초기화 한다.
								for(k = 0; k < DataMemb.aElemList.GetSize(); k++) // 기존 Member의 전체 길이를 구한다.
								{
									if(m_pDoc->m_pAttrCtrl->GetElem(DataMemb.aElemList[k], DataElem))
										dLengthOld += m_pDoc->calcLAVElem(DataElem);
								}
								dAvgLengthOld = dLengthOld / DataMemb.aElemList.GetSize(); // 선택된 멤버의 요소의 평균길이를 구한다.
								dMinAvgLengthOld = min(dAvgLengthOld, dMinAvgLengthOld);   // 기존 멤버를 이루는 요소의 평균길이의 최소값
							}
						}
					}
				}
				// 현재의 내부 절점에 연결된 모든 멤버에 대해 
				// 현재 멤버의 평균길이가 기존 멤버의 평균길이보다 짧으면 현재 멤버는 거더(연속부재)이다. 
				if(dAvgLength <= dMinAvgLengthOld) bContinuous = TRUE;
			}
			if(bContinuous) // 현재 멤버가 거더이면 현재 멤버와 내부에서 교차하는 다른 멤버(보)를 삭제할 멤버 map에 모은다.
			{
				POSITION pos = mapMembDelTemp.GetStartPosition();
				while(pos != NULL)
				{
					mapMembDelTemp.GetNextAssoc(pos, nKeyMemb, nTemp);  // 각 멤버에 대해 삭제할 멤버를 하나씩 불러온다.
					mapMembDel.SetAt(nKeyMemb, nKeyMemb); // 각 멤버에 대해 삭제할 멤버를 전체 멤버에 대해 삭제할 멤버의 map에 모은다.
				}
				bDelete = TRUE;      // return 값 설정
			}
		}
 
	} 
	// 현재 Member가 연속이면(거더이면) 이 Member와 내부에서 교차하는 다른 Member(보)를 해제한다.  
	CArray<T_MEMB_D, T_MEMB_D&> aSuperiorMemb; 
	aSuperiorMemb.RemoveAll();
	if(bDelete) // 삭제할 멤버(보)가 존재하면
	{
		for(i = 0; i < nCountMembList; i++)
			if(!mapMembDel.Lookup(i, nTemp)) aSuperiorMemb.Add(aDataMemb.GetAt(i));
	}
	aDataMemb.RemoveAll();
	for(i = 0; i < aSuperiorMemb.GetSize(); i++) aDataMemb.Add(aSuperiorMemb.GetAt(i));
	return TRUE;
}

// 선택된 모든 요소중에서 멤버로 지정되지 않은 요소들을
// 단일 요소 멤버로 지정하는 함수
BOOL CDgnGenMembDlg::AddSingleElemMember(CArray<T_MEMB_D, T_MEMB_D&>& aDataMemb)
{
	T_ELEM_K KeyElem;
	T_MEMB_D DataMemb;
	T_MEMB_K KeyMemb;
	int nKeyMembAssigned;

	int nCountMembList = aDataMemb.GetSize();
	CMap<T_ELEM_K, T_ELEM_K, int, int> mapMembAssigned;  // 요소가 속한 멤버의 인덱스  
	mapMembAssigned.InitHashTable(HASHSIZEELEM);
	mapMembAssigned.RemoveAll(); 
	
	for(int nIndex = 0; nIndex < nCountMembList; nIndex++)
	{    
		for(int n = 0; n < aDataMemb.GetAt(nIndex).aElemList.GetSize(); n++)
			mapMembAssigned.SetAt(aDataMemb.GetAt(nIndex).aElemList[n], nIndex);
	}

	int nNumSelFrmKey = m_aSelFrmKey.GetSize();	    
	for(int i = 0; i < nNumSelFrmKey; i++) 
	{
		DataMemb.Initialize();
		KeyElem = m_aSelFrmKey[i];
		if(!mapMembAssigned.Lookup(KeyElem, nKeyMembAssigned))  // 멤버로 지정되지 않은 요소만 멤버로 지정
		{
			DataMemb.aElemList.Add(KeyElem);
			aDataMemb.Add(DataMemb);
		}
	}  
	return TRUE;
}

BOOL CDgnGenMembDlg::DelInferiorMember()
{
	T_ELEM_D DataElem;
	T_MEMB_K KeyMemb;
	T_MEMB_D DataMemb, rData;  
	CArray<T_ELEM_K, T_ELEM_K> arKeyElem;
	CArray<T_MEMB_K, T_MEMB_K> arMembDel;
	CArray<T_MEMB_K, T_MEMB_K> rKeyList;
	CMap<T_MEMB_K, T_MEMB_K, int, int> mapMembDel;  // 전체 멤버에 대해 삭제할 멤버
	DataElem.Initialize();
	DataMemb.Initialize();
	rData.Initialize();
	mapMembDel.InitHashTable(HASHSIZEMEMB);  
	arKeyElem.RemoveAll();
	arMembDel.RemoveAll();
	rKeyList.RemoveAll();
	mapMembDel.RemoveAll();
	BOOL bDelete = FALSE;  
	BOOL bContinuous = FALSE;
	int i, j, k, l, nTemp;

	m_pDoc->m_pAttrCtrl->GetMembKeyList(rKeyList);
	int nCountKeyList = rKeyList.GetSize();
	for(l = 0; l < nCountKeyList; l++)
	{  
		bContinuous = FALSE;
		if(mapMembDel.Lookup(rKeyList[l], nTemp)) continue;  // 현재 멤버가 삭제할 멤버 리스트에 있으면 다음 멤버로 넘어감
		else   // 현재 멤버가 삭제할 멤버 리스트에 없으면 연속성 여부를 따져 삭제할지 판단함
		{
			m_pDoc->m_pAttrCtrl->GetMemb(rKeyList[l], rData);
			int nNumNode = 2 * rData.aElemList.GetSize();
			double dLength = 0., dAvgLength = 0.;
			const int nMaxNum = 100;
	
			int nNodeIndex[nMaxNum];
			for(i = 0; i < nMaxNum; i++) nNodeIndex[i] = 0;    
			for(i = 0; i < rData.aElemList.GetSize(); i++)
			{    
				m_pDoc->m_pAttrCtrl->GetElem(rData.aElemList.GetAt(i), DataElem);
				{
					nNodeIndex[2 * i] = DataElem.elnod[0];
					nNodeIndex[2 * i + 1] = DataElem.elnod[1];
					dLength += m_pDoc->calcLAVElem(DataElem);  // 현재 Member의 전체 길이
				}    
			}
			dAvgLength = dLength / rData.aElemList.GetSize();   // 현재 Member를 이루는 요소의 평균 길이
			
			CArray<T_NODE_K, T_NODE_K> aInnerNode;
			aInnerNode.RemoveAll();
			for(i = 0; i < nNumNode; i++)    // 현재 Member의 내부 절점을 구한다.
			{
				for(j = i; j < nNumNode; j++)
				{      
					if(j != i && nNodeIndex[i] == nNodeIndex[j]) {aInnerNode.Add(nNodeIndex[i]); break;}
				}
			}
			
			double dLengthOld, dAvgLengthOld, dMinAvgLengthOld;          
			CMap<T_MEMB_K, T_MEMB_K, int, int> mapMembDelTemp;  
			mapMembDelTemp.InitHashTable(51);  
			mapMembDelTemp.RemoveAll();  // 각 멤버의 키에 대해 삭제할 멤버
			int nInnerNode = aInnerNode.GetSize(); 
			// 현재 멤버와 내부에서 교차하는 모든 기존 멤버를 비교하여 어느 멤버가 거더인지 확인한다.
			// 현재 멤버가 거더가 아니라고 판단되는 시점에서 작업을 멈추고 연속이 아님을 리턴한다.  
			// 만약, 현재 멤버가 거더이면 이와 교차하는 기존 멤버를 map에 모으고 추후 이의 멤버 설정을 해제한다.        
			for(i = 0; i < nInnerNode; i++)  
			{
				bContinuous = FALSE;        
				dAvgLengthOld = 0.;        // 멤버를 이루는 요소의 평균길이
				dMinAvgLengthOld = 9.e+6;  // 한 내부절점에 연결된 멤버를 이루는 요소의 평균길이의 최소값
				m_pDoc->m_pAttrCtrl->GetElemKeyListConnectedToNode(aInnerNode[i], arKeyElem);  // 현재 Member의 내부 절점에 연결된 요소리스트를 구한다.  
				for(j = 0; j < arKeyElem.GetSize(); j++)   // 각 내부 절점에 연결된 모든 요소에 대해
				{
					if(m_pDoc->m_pAttrCtrl->GetMembAssigned(arKeyElem.GetAt(j), KeyMemb))  // 요소가 멤버의 구성요소이면
					{ 
						if(!mapMembDelTemp.Lookup(KeyMemb, nTemp) &&       // 선택된 멤버가 삭제될 리스트에 없고
							KeyMemb != rKeyList[l] &&                        // 선택된 멤버가 현재 멤버와 다르고
							m_pDoc->m_pAttrCtrl->GetMemb(KeyMemb, DataMemb)) // 선택된 멤버의 멤버 데이터가 존재하면              
						{
							// 선택된 멤버가 내부 절점을 가지고 있고(두 개 이상의 요소로 이루어져 있고)
							// 이 내부 절점이 고려중인 내부 절점과 일치하면 작업을 계속한다.              
							if(DataMemb.aElemList.GetSize() >= 2)
							{
								BOOL bCross1 = FALSE;
								BOOL bCross2 = FALSE;              
								for(int m = 0; m < DataMemb.aElemList.GetSize(); m++)
								{
									if(m_pDoc->m_pAttrCtrl->GetElem(DataMemb.aElemList[m], DataElem))
									{
										if(DataElem.elnod[0] == aInnerNode[i]) bCross1 = TRUE;
										if(DataElem.elnod[1] == aInnerNode[i]) bCross2 = TRUE;
									}
								}
								if(!(bCross1 && bCross2)) continue;  // 선택된 멤버의 내부절점과 고려중인 멤버의 내부절점이 일치하지 않으면 선택된 멤버에 대한 작업을 중지한다.
							
								mapMembDelTemp.SetAt(KeyMemb, 0);     // 선택된 멤버를 삭제될 리스트에 넣는다.      
								dLengthOld = 0.;                      // 선택된 멤버의 길이를 초기화 한다.
								for(k = 0; k < DataMemb.aElemList.GetSize(); k++) // 기존 Member의 전체 길이를 구한다.
								{
									if(m_pDoc->m_pAttrCtrl->GetElem(DataMemb.aElemList[k], DataElem))
										dLengthOld += m_pDoc->calcLAVElem(DataElem);
								}
								dAvgLengthOld = dLengthOld / DataMemb.aElemList.GetSize(); // 선택된 멤버의 요소의 평균길이를 구한다.
								dMinAvgLengthOld = min(dAvgLengthOld, dMinAvgLengthOld);   // 기존 멤버를 이루는 요소의 평균길이의 최소값
							}
						}
					}
				}
				// 현재의 내부 절점에 연결된 모든 멤버에 대해 
				// 현재 멤버의 평균길이가 기존 멤버의 평균길이보다 짧으면 현재 멤버는 거더(연속부재)이다. 
				if(dAvgLength <= dMinAvgLengthOld) bContinuous = TRUE;
			}
			if(bContinuous) // 현재 멤버가 거더이면 현재 멤버와 내부에서 교차하는 다른 멤버(보)를 삭제할 멤버 map에 모은다.
			{
				POSITION pos = mapMembDelTemp.GetStartPosition();
				while(pos != NULL)
				{
					mapMembDelTemp.GetNextAssoc(pos, KeyMemb, nTemp);  // 각 멤버에 대해 삭제할 멤버르 하나씩 불러온다.
					mapMembDel.SetAt(KeyMemb, KeyMemb); // 각 멤버에 대해 삭제할 멤버를 전체 멤버에 대해 삭제할 멤버의 map에 모은다.
				}
				bDelete = TRUE;      // return 값 설정
			}
		}
 
	} 
	// 현재 Member가 연속이면(거더이면) 이 Member와 내부에서 교차하는 다른 Member(보)를 해제한다.
	if(bDelete) // 삭제할 멤버(보)가 존재하면
	{
		arMembDel.RemoveAll();
		POSITION pos = mapMembDel.GetStartPosition();
		while(pos != NULL)
		{
			mapMembDel.GetNextAssoc(pos, KeyMemb, nTemp);
			if(m_pDoc->m_pAttrCtrl->ExistMemb(KeyMemb))  arMembDel.Add(KeyMemb);
		}
		m_pDoc->m_pDataCtrl->DelMemb(arMembDel);  // 삭제할 멤버(보)를 삭제한다.
	}
	return bDelete;
}
*/