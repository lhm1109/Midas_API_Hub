// DgnGenMembDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGShearConnGroupDlg.h"
#include "DgnDataCtrl.h"

//#include "..\wg_base\wg_base_NumericOptimizer.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\TreeMenuMode.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_MathFunc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_MembCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include <math.h>

#include "..\wg_main\wg_mainres2.h"	// For ID_DGN_GEN_CMFT

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCPGShearConnGroupDlg dialog
CCPGShearConnGroupDlg::CCPGShearConnGroupDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CCPGShearConnGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCPGShearConnGroupDlg)
	m_nOption = 0;
	m_nGroupLengthOpt = 0;
	//}}AFX_DATA_INIT
	m_mapSelFrmKey.InitHashTable(HASHSIZEELEM);
	m_bSentMessage = FALSE;
	m_nFailureType = 0;
 	m_pDoc = CDBDoc::GetDocPoint();
}

CCPGShearConnGroupDlg::~CCPGShearConnGroupDlg()
{
}

void CCPGShearConnGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPGShearConnGroupDlg)
	DDX_Radio(pDX, IDC_DGN_CPG_SHEAR_CONN_GROUP_AR, m_nOption);
	DDX_Radio(pDX, IDC_DGN_CPG_SHEAR_CONN_GROUP_LENGTH_AUTO_OPT, m_nGroupLengthOpt);

	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_CONN_NUM_EDT, m_dNum);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_CONN_TENSILE_STRENGTH_EDT, m_dFu);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_CONN_DIAMETER_EDT, m_dDs);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_CONN_HEIGHT_EDT, m_dHsc);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_CONN_CEN_TO_CEN_EDT, m_dSt);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_CONN_GROUP_NAME_EDT, m_dGroupName);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_CONN_GROUP_LENGTH_EDT, m_dLength);        

	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_CONN_GROUP_UNIT_STRESS1_STC, m_dFuUnit);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_CONN_GROUP_UNIT_LENGTH1_STC, m_dDsUnit);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_CONN_GROUP_UNIT_LENGTH2_STC, m_dHscUnit);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_CONN_GROUP_UNIT_LENGTH3_STC, m_dStUnit);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_CONN_GROUP_UNIT_LENGTH4_STC, m_dLengthUnit);

	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_CONN_GROUP_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCPGShearConnGroupDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CCPGShearConnGroupDlg)
	ON_BN_CLICKED(IDC_DGN_CPG_SHEAR_CONN_GROUP_AR, OnDgnOption)
	ON_BN_CLICKED(IDC_DGN_CPG_SHEAR_CONN_GROUP_DEL, OnDgnOption)
	ON_BN_CLICKED(IDC_DGN_CPG_SHEAR_CONN_GROUP_LENGTH_AUTO_OPT, OnShearConnGroupLengthOption)
	ON_BN_CLICKED(IDC_DGN_CPG_SHEAR_CONN_GROUP_LENGTH_USER_OPT, OnShearConnGroupLengthOption)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_CPG_SHEAR_CONN_GROUP_LIST, OnItemchanged)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCPGShearConnGroupDlg message handlers
BOOL CCPGShearConnGroupDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	m_aCtrlAll.Add(IDC_DGN_CPG_SHEAR_CONN_NUM_EDT);
	m_aCtrlAll.Add(IDC_DGN_CPG_SHEAR_CONN_TENSILE_STRENGTH_EDT);
	m_aCtrlAll.Add(IDC_DGN_CPG_SHEAR_CONN_DIAMETER_EDT);
	m_aCtrlAll.Add(IDC_DGN_CPG_SHEAR_CONN_HEIGHT_EDT);
	m_aCtrlAll.Add(IDC_DGN_CPG_SHEAR_CONN_CEN_TO_CEN_EDT);
	m_aCtrlAll.Add(IDC_DGN_CPG_SHEAR_CONN_GROUP_NAME_EDT);

	m_aCtrlAll.Add(IDC_DGN_CPG_SHEAR_CONN_GROUP_LENGTH_AUTO_OPT);
	m_aCtrlAll.Add(IDC_DGN_CPG_SHEAR_CONN_GROUP_LENGTH_USER_OPT);
	m_aCtrlAll.Add(IDC_DGN_CPG_SHEAR_CONN_GROUP_LENGTH_EDT);

	SetInitUnit();
	SetListCtrlHeader();
	MakeItemEx();	
	CtrlManager();

	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE; 
}

void CCPGShearConnGroupDlg::OnDgnOption() 
{
	CtrlManager();
}

void CCPGShearConnGroupDlg::OnShearConnGroupLengthOption()
{
	UpdateData(TRUE);    
	BOOL bEnableCtrl = TRUE;

	if(m_nGroupLengthOpt == 0) {bEnableCtrl = FALSE;} // Auto Calc.  Group Length
	else if(m_nGroupLengthOpt == 1) {bEnableCtrl = TRUE;} // User Input Group Length 
	else ASSERT(0);

	GetDlgItem(IDC_DGN_CPG_SHEAR_CONN_GROUP_LENGTH_EDT)->EnableWindow(bEnableCtrl);

		UpdateData(FALSE);
}

void CCPGShearConnGroupDlg::CtrlManager() 
{
	UpdateData(TRUE);    

	if(m_nOption == 0)  {CDlgUtil::CtrlEnableDisable(this, m_aCtrlAll, TRUE);  }
	else if(m_nOption == 1)  {CDlgUtil::CtrlEnableDisable(this, m_aCtrlAll, FALSE);}

	OnShearConnGroupLengthOption();

	UpdateData(FALSE);
}

// void CCPGShearConnGroupDlg::OnDgnClose() 
// {
// 	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
// }

void CCPGShearConnGroupDlg::Execute()
{
	UpdateData(TRUE);	
	CArray<T_ELEM_K,T_ELEM_K> aSelElemKey;   // Selected Element List
	
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

BOOL CCPGShearConnGroupDlg::MemberDefine()
{   
	int nNumSelFrmKey = m_aSelFrmKey.GetSize();
	int nNumNonFrmKey = m_aNonFrmKey.GetSize();

	if(m_nOption == 0)  // Add/Replace
	{
		if(nNumSelFrmKey == 0)      GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_ELEM_TYPE)); // [오류] Frame Type 요소만 Member 지정이 가능합니다.
		else if(nNumSelFrmKey >= 1)
		{ 
			if(!MemberDefineAuto()) return FALSE;
		}
		else ASSERT(0);

		if(nNumNonFrmKey > 0 && nNumSelFrmKey > 0)
		{
			for(int i = 0; i < nNumNonFrmKey; i++) {GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_NON_FRAME), m_aNonFrmKey.GetAt(i));}
			// 요소 %d은(는) Frame Type이 아니므로 Member를 지정할 수 없습니다.
		}
	}
	else if(m_nOption == 1)	// Delete : 요소가 Member의 구성요소 일때 그 요소가 속한 Member를 삭제한다.
	{
		if(nNumSelFrmKey == 0)      GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_MEMB_ONLY_FRAME)); // Frame Type 요소만 Member 지정 및 해제가 가능합니다.
		else if(nNumSelFrmKey >= 1) DelAssignedMember();
		else ASSERT(0);
	}

	return TRUE;
//  MakeItemEx();
}

BOOL CCPGShearConnGroupDlg::MemberDefineAuto()
{  
	T_SCGD_K KeyMemb;
	T_SCGD_D DataMemb;
	T_ELEM_K KeyElem, KeyTemp;
	T_ELEM_D DataElem, DElemNext;
	T_NODE_K KeyNode;  
	CArray<T_ELEM_K, T_ELEM_K> aConnectedElem;
	CArray<T_SCGD_K, T_SCGD_K> aKeyMembDel;        // 최종적으로 삭제할 멤버의 리스트
	CArray<T_SCGD_D, T_SCGD_D&> aDataMemb;         // 최종적으로 선택된 멤버의 리스트
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapUsedElem;  // 한 번이라도 Member 지정에 사용된 요소의 map  
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K> mapUsedNode;  // Member의 Element List에 속하는 요소들의 절점의 map
	CMap<T_SCGD_K, T_SCGD_K, int, int> mapKeyMembDel; // 기존 요소가 멤버의 구성요소일 때 삭제할 멤버의 map
	mapUsedElem.InitHashTable(HASHSIZEELEM);
	mapKeyMembDel.InitHashTable(HASHSIZESCGD);
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
			DataMemb.arGroupElemKey.Add(KeyElem);        // 현재 지정할 Member의 요소 리스트에 추가한다.
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
						
						DataMemb.arGroupElemKey.Add(aConnectedElem.GetAt(k));   // 위의 과정을 무사히 통과하면 Member의 구성요소가 된다.
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
						
						DataMemb.arGroupElemKey.Add(aConnectedElem.GetAt(k));   // 위의 과정을 무사히 통과하면 Member의 구성요소가 된다.
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
				
		if(DataMemb.arGroupElemKey.GetSize() >= 1 )  
		{ 
			for(int m = 0; m < DataMemb.arGroupElemKey.GetSize(); m++)
			{
				mapUsedElem.SetAt(DataMemb.arGroupElemKey[m], DataMemb.arGroupElemKey[m]); // 한번이라도 Member 지정에 사용된 요소의 map에 추가한다.
				if(m_pDoc->m_pAttrCtrl2->GetScgdAssigned(DataMemb.arGroupElemKey[m], KeyMemb))
				{ // 선택된 요소가 Member의 구성요소로 쓰일 때 삭제할 Member를 모은다.
					if(m_pDoc->m_pAttrCtrl2->ExistScgd(KeyMemb)) mapKeyMembDel.SetAt(KeyMemb, 0);
				}
			}

			AlignElemListInOrder(DataMemb);
			if(!Dlg2ShearConnData(DataMemb)) return FALSE;
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
		if(!m_pDoc->m_pDataCtrl->DelScgd(aKeyMembDel)) return FALSE;  //
	}

	BOOL bAddMemb = TRUE;
	BOOL bDelInferiorMember = TRUE;
	BOOL bAddSingleElemMember = TRUE;
	if(DataMemb.arGroupElemKey.GetSize() == 1) // 해당 요소가 하나인 경우, Single Element Group을 지원함
	{
		if(!m_pDoc->m_pDataCtrl->AddScgd(aDataMemb)) bAddMemb = FALSE;  // 선택된 멤버 후보를 DB에 저장한다.  
		if(!DelInferiorMember())  bDelInferiorMember = FALSE;           // 거더와 보를 구분하여 보의 멤버 지정을 해제한다.
		if(!AddSingleElemMember()) bAddSingleElemMember = FALSE;        // 멤버로 선정된 후보에 단일 요소 멤버를 새로이 추가한다.  
		if(bAddMemb && bDelInferiorMember && bAddSingleElemMember) InitSelectedItem();
		else return FALSE;
	}
	else
	{    
		if(aDataMemb.GetSize() > 0)
		{
			if(!m_pDoc->m_pDataCtrl->AddScgd(aDataMemb)) bAddMemb = FALSE;  // 선택된 멤버 후보를 DB에 저장한다.  
			if(!DelInferiorMember())  bDelInferiorMember = FALSE;           // 거더와 보를 구분하여 보의 멤버 지정을 해제한다.
			if(bAddMemb && bDelInferiorMember) InitSelectedItem();
			else return FALSE;
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

	return TRUE;
}

// Frame type의 element list로부터 이들이 속한 member를 삭제하는 함수
// Manual 및 Automatic의 Delete일 때 쓰임
void CCPGShearConnGroupDlg::DelAssignedMember()
{
	T_SCGD_K KeyMemb;
	T_SCGD_D DataMemb;
	CArray<T_SCGD_K, T_SCGD_K> aKeyMemb;
	CMap<T_SCGD_K, T_SCGD_K, T_SCGD_K, T_SCGD_K> mapKeyMemb;
	mapKeyMemb.InitHashTable(HASHSIZESCGD);  
	mapKeyMemb.RemoveAll();
	aKeyMemb.RemoveAll();    
	int nNumSelFrmKey = m_aSelFrmKey.GetSize();	
	for(int i = 0; i < nNumSelFrmKey; i++)
	{
		if(m_pDoc->m_pAttrCtrl2->GetScgdAssigned(m_aSelFrmKey.GetAt(i), KeyMemb))
		{ // 선택된 요소중 멤버가 할당된 요소를 map에 모은다.      
			if(m_pDoc->m_pAttrCtrl2->ExistScgdAssigned(KeyMemb)) mapKeyMemb.SetAt(KeyMemb, KeyMemb);
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

		if(m_pDoc->m_pDataCtrl->DelScgd(aKeyMemb)) InitSelectedItem();
	}

	if(aSelItem.GetSize() > 0) SetListFocus(aSelItem);  // Member 삭제후 리스트의 포커스 설정
}

// Member의 요소를 i, j순서대로 다시 배열하는 함수
void CCPGShearConnGroupDlg::AlignElemListInOrder(T_SCGD_D& rData)
{
	T_ELEM_K KeyElem;
	T_ELEM_D DataElem;
	CMap<T_NODE_K, T_NODE_K, T_ELEM_K, T_ELEM_K> mapNodeiElem;
	CMap<T_NODE_K, T_NODE_K, T_ELEM_K, T_ELEM_K> mapNodejElem;
	CMap<T_NODE_K, T_NODE_K, int, int> mapInnerNode;       
	CArray<T_ELEM_D, T_ELEM_D&> aDataElem;    
	mapNodeiElem.InitHashTable(51);
	mapNodejElem.InitHashTable(51);
	mapInnerNode.InitHashTable(51);
	mapNodeiElem.RemoveAll();
	mapNodejElem.RemoveAll();
	mapInnerNode.RemoveAll();
	aDataElem.RemoveAll();

	int i, j, nTemp;
	int nElem = rData.arGroupElemKey.GetSize();
	BOOL bSuccess = FALSE;
	ASSERT(nElem > 0);
		
	for(i = 0; i < nElem; i++)
	{        
		if(m_pDoc->m_pAttrCtrl->GetElem(rData.arGroupElemKey[i], DataElem)) aDataElem.Add(DataElem);    
	}
	ASSERT(nElem == aDataElem.GetSize());
	
	int nMaxNum = nElem*2;
	if(nMaxNum <= 0) {ASSERT(0); return;}
	int *nNodeIndex = new int[nMaxNum];   
	for(i = 0; i < nMaxNum; i++) nNodeIndex[i] = 0;    
	for(i = 0; i < nElem; i++)
	{    
		m_pDoc->m_pAttrCtrl->GetElem(rData.arGroupElemKey[i], DataElem);
		{
			nNodeIndex[2 * i] = DataElem.elnod[0];
			nNodeIndex[2 * i + 1] = DataElem.elnod[1];
			mapNodeiElem.SetAt(DataElem.elnod[0], rData.arGroupElemKey[i]);
			mapNodejElem.SetAt(DataElem.elnod[1], rData.arGroupElemKey[i]);
		}    
	}
	
	int nNumNode = 2 * rData.arGroupElemKey.GetSize();
	for(i = 0; i < nNumNode; i++)    // 현재 Member의 내부 절점을 구한다.
	{
		for(j = i; j < nNumNode; j++)
		{
			if(j != i && nNodeIndex[i] == nNodeIndex[j]) {mapInnerNode.SetAt(nNodeIndex[i], 0); break;}
		}
	}
	
	int nStartNode, nLastNode;  // 연속성을 파악할 첫번째 요소의 i 노드 및 마지막 요소의 j 노드
	for(i = 0; i < nElem; i++)
	{
		m_pDoc->m_pAttrCtrl->GetElem(rData.arGroupElemKey[i], DataElem);
		if(!mapInnerNode.Lookup(DataElem.elnod[0], nTemp)) nStartNode = DataElem.elnod[0];
		if(!mapInnerNode.Lookup(DataElem.elnod[1], nTemp)) nLastNode = DataElem.elnod[1];
	}
	
	for(i = 0; i < nElem; i++)  rData.arGroupElemKey[i] = 0;  // 멤버의 요소 리스트를 초기화한다.
	mapNodeiElem.Lookup(nStartNode, KeyElem);  // 멤버의 첫번째 요소를 찾는다.
	rData.arGroupElemKey[0] = KeyElem;    // 멤버의 첫번째 요소를 설정한다.  
	for(i = 0; i < nElem; i++)
	{
		m_pDoc->m_pAttrCtrl->GetElem(KeyElem, DataElem);
		if(i < nElem - 1) 
		{
			mapNodeiElem.Lookup(DataElem.elnod[1], KeyElem);
			rData.arGroupElemKey[i + 1] = KeyElem;
		}
		if(i == nElem - 1 && DataElem.elnod[1] == nLastNode) bSuccess = TRUE;
	}
	ASSERT(bSuccess);

	delete []nNodeIndex;
}

void CCPGShearConnGroupDlg::SetInitUnit()
{
	m_dNum.SetUnitType(D_UNITSYS_NONE);
	m_dFu.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_dDs.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dHsc.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dSt.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dLength.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_dFuUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_dDsUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_dHscUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_dStUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_dLengthUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
}
void CCPGShearConnGroupDlg::InitSelectedItem() // 선택된 요소와 절점을 해제한다.
{  
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CCPGShearConnGroupDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
			UpdataCtrlUnit();
			break;
		case D_UPDATE_SEL_ADD: 
		case D_UPDATE_SEL_DEL:
			break;
		default: ASSERT(TRUE);
	}
}

void CCPGShearConnGroupDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_SCGD_K Key;
	T_SCGD_D Data;

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_SCGD_ADD):
			{
				pViewBuff->GetScgd(nKey, Key, Data);
				InsertItem(Key);
			}
			break;
		case(UR_SCGD_DEL):
			{
				pViewBuff->GetScgd(nKey, Key, Data);
				DeleteItem(Key);
			}
			break;
		case(UR_SCGD_MFS):
		case(UR_SCGD_MFD):
			{
				pViewBuff->GetScgd(nKey, Key, Data);
				DeleteItem(Key);
				InsertItem(Key);        
			}
			break;
		default:
			break;
		}
	} 
}

void CCPGShearConnGroupDlg::InsertItem(T_SCGD_K Key)
{
	// 삽입 위치를 찾는다.
	T_SCGD_D Data;
	int nSize = m_List.GetItemCount();
	if (!m_pDoc->m_pAttrCtrl2->GetScgd(Key, Data)) return;
	CString strMembIndex;     
	int nElemKey;  
    int nCount = 0;
	for( nCount = 0; nCount < nSize; nCount++)
	{
		strMembIndex = m_List.GetItemText(nCount, 0);
		nElemKey = _ttoi(strMembIndex);
		if(nElemKey > Data.ScgdKey) break;
	}  
	int nIndex = 0;
	// Column 0 : Group Index
	strMembIndex.Format(_T("%d"), Data.ScgdKey); // Group Index는 Shear Connector Group Key로 표현
	nIndex = m_List.InsertItem(nCount, strMembIndex);      

	// Column 1 : Group Name
	CString csGroupNameItem;
	csGroupNameItem.Format(_T("%s"), Data.strShearConnGroupName);
	m_List.SetItemText(nIndex, 1, csGroupNameItem);		
	m_List.SetItemData(nIndex, (DWORD)Key);

	// Column 2 : Group Length
	CString csGroupLengthItem;
	csGroupLengthItem.Format(_T("%.2f"), double(Data.dLength));
	m_List.SetItemText(nIndex, 2, csGroupLengthItem);		
	m_List.SetItemData(nIndex, (DWORD)Key);

	// Column 3 : Group Start Elem Key
	CString csStartElemNo;
	csStartElemNo.Format(_T("%d"), Data.StartElemKey);
	m_List.SetItemText(nIndex, 3, csStartElemNo);		
	m_List.SetItemData(nIndex, (DWORD)Key);

	// Column 4 : Group End Elem Key
	CString csEndElemNo;
	csEndElemNo.Format(_T("%d"), Data.EndElemKey);
	m_List.SetItemText(nIndex, 4, csEndElemNo);		
	m_List.SetItemData(nIndex, (DWORD)Key);

	// Column 5 : Group Element List
	CString csElemListItem;
	CString strElemList = _T("");
	for(int i = 0; i < Data.arGroupElemKey.GetSize(); i++)
	{
		csElemListItem.Format(_T("%d"), Data.arGroupElemKey[i]);
		if(i == 0)     strElemList += csElemListItem;
		else      strElemList +=  _T(", ") + csElemListItem;
	}

	m_List.SetItemText(nIndex, 5, strElemList);		
	m_List.SetItemData(nIndex, (DWORD)Key);
}


void CCPGShearConnGroupDlg::DeleteItem(T_SCGD_K Key)
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
BOOL CCPGShearConnGroupDlg::DelInferiorMember()
{
	T_ELEM_D DataElem;
	T_SCGD_K KeyMemb;
	T_SCGD_D DataMemb, rData;  
	CArray<T_ELEM_K, T_ELEM_K> arKeyElem;
	CArray<T_SCGD_K, T_SCGD_K> arMembDel;
	CArray<T_SCGD_K, T_SCGD_K> rKeyList;
	CMap<T_SCGD_K, T_SCGD_K, int, int> mapMembDel;    // 전체 멤버에 대해 삭제할 멤버
	CMap<T_SCGD_K, T_SCGD_K, T_SCGD_K, T_SCGD_K> mapSelMembKey; // 현재 선택된 요소가 속하는 멤버의 map

	DataElem.Initialize();
	DataMemb.Initialize();
	rData.Initialize();
	mapMembDel.InitHashTable(HASHSIZESCGD);  
	mapSelMembKey.InitHashTable(HASHSIZESCGD);
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
		if(m_pDoc->m_pAttrCtrl2->GetScgdAssigned(m_aSelFrmKey.GetAt(i), KeyMemb)) mapSelMembKey.SetAt(KeyMemb, KeyMemb);
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
			m_pDoc->m_pAttrCtrl2->GetScgd(rKeyList[l], rData);
			int nNumNode = 2 * rData.arGroupElemKey.GetSize();
			double dLength = 0., dAvgLength = 0.;
						
			int nMaxNum = rData.arGroupElemKey.GetSize()*2;
			if(nMaxNum <= 0) {ASSERT(0); continue;}
			int *nNodeIndex = new int[nMaxNum];   
			for(i = 0; i < nMaxNum; i++) nNodeIndex[i] = 0;    
			for(i = 0; i < rData.arGroupElemKey.GetSize(); i++)
			{    
				m_pDoc->m_pAttrCtrl->GetElem(rData.arGroupElemKey.GetAt(i), DataElem);
				{
					nNodeIndex[2 * i] = DataElem.elnod[0];
					nNodeIndex[2 * i + 1] = DataElem.elnod[1];
					dLength += m_pDoc->calcLAVElem(DataElem);  // 현재 Member의 전체 길이
				}    
			}
			dAvgLength = dLength / rData.arGroupElemKey.GetSize();   // 현재 Member를 이루는 요소의 평균 길이
			
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
			CMap<T_SCGD_K, T_SCGD_K, int, int> mapMembDelTemp;  
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
							m_pDoc->m_pAttrCtrl2->GetScgd(KeyMemb, DataMemb)) // 선택된 멤버의 멤버 데이터가 존재하면              
						{
							// 선택된 멤버가 내부 절점을 가지고 있고(두 개 이상의 요소로 이루어져 있고)
							// 이 내부 절점이 고려중인 내부 절점과 일치하면 작업을 계속한다.              
							if(DataMemb.arGroupElemKey.GetSize() >= 2)
							{
								BOOL bCross1 = FALSE;
								BOOL bCross2 = FALSE;              
								for(int m = 0; m < DataMemb.arGroupElemKey.GetSize(); m++)
								{
									if(m_pDoc->m_pAttrCtrl->GetElem(DataMemb.arGroupElemKey[m], DataElem))
									{
										if(DataElem.elnod[0] == aInnerNode[i]) bCross1 = TRUE;
										if(DataElem.elnod[1] == aInnerNode[i]) bCross2 = TRUE;
									}
								}
								if(!(bCross1 && bCross2)) continue;  // 선택된 멤버의 내부절점과 고려중인 멤버의 내부절점이 일치하지 않으면 선택된 멤버에 대한 작업을 중지한다.
							
								mapMembDelTemp.SetAt(KeyMemb, 0);     // 선택된 멤버를 삭제될 리스트에 넣는다.      
								dLengthOld = 0.;                      // 선택된 멤버의 길이를 초기화 한다.
								for(k = 0; k < DataMemb.arGroupElemKey.GetSize(); k++) // 기존 Member의 전체 길이를 구한다.
								{
									if(m_pDoc->m_pAttrCtrl->GetElem(DataMemb.arGroupElemKey[k], DataElem))
										dLengthOld += m_pDoc->calcLAVElem(DataElem);
								}
								dAvgLengthOld = dLengthOld / DataMemb.arGroupElemKey.GetSize(); // 선택된 멤버의 요소의 평균길이를 구한다.
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
			if(m_pDoc->m_pAttrCtrl2->ExistScgd(KeyMemb))  arMembDel.Add(KeyMemb);
		}
		m_pDoc->m_pDataCtrl->DelScgd(arMembDel);  // 삭제할 멤버(보)를 삭제한다.
	}
	return bDelete;
}

// 선택된 모든 요소중에서 멤버로 지정되지 않은 요소들을
// 하나씩 다시 멤버로 지정하는 함수
BOOL CCPGShearConnGroupDlg::AddSingleElemMember()
{
	T_SCGD_K KeyMemb;
	T_ELEM_K KeyElem;
	T_SCGD_D DataMemb;
	CArray<T_SCGD_D, T_SCGD_D&> aDataMemb;
	aDataMemb.RemoveAll();  
	
	int nNumSelFrmKey = m_aSelFrmKey.GetSize();	    
	for(int i = 0; i < nNumSelFrmKey; i++) 
	{
		DataMemb.Initialize();
		KeyElem = m_aSelFrmKey[i];
		if(!m_pDoc->m_pAttrCtrl2->GetScgdAssigned(KeyElem, KeyMemb))  // 멤버로 지정되지 않은 요소만 멤버로 지정
		{
			DataMemb.arGroupElemKey.Add(KeyElem);
			aDataMemb.Add(DataMemb);
		}
	}
	if(!m_pDoc->m_pDataCtrl->AddScgd(aDataMemb)) return FALSE;
	return TRUE;
}

// 아래층에 기둥이나 벽체가 있는지 확인하는 함수
BOOL CCPGShearConnGroupDlg::IsExistColumnWallLower(T_NODE_K KeyNode, CArray<T_ELEM_K, T_ELEM_K>& aConnectedElem)
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
BOOL CCPGShearConnGroupDlg::IsStoryLevel(T_NODE_K KeyNode)
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

BOOL CCPGShearConnGroupDlg::IsMatchDirectionAttribute(T_ELEM_K KeyRef, T_ELEM_K KeyNew)
{  
	if(!CompareElemDirection(KeyRef, KeyNew)) return FALSE;  // Automatic인 경우만 요소의 직선여부를 체크한다. 
	if(!CompareElemAttribute(KeyRef, KeyNew)) return FALSE;
	return TRUE;
}

BOOL CCPGShearConnGroupDlg::CompareElemDirection(T_ELEM_K KeyRef, T_ELEM_K KeyNew)
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

	if(fabs(dAngle) > 1) return FALSE; // Automatic : 직선상에 위치할 때만 허용(단, 각도 1도 미만일때는 허용)
	return TRUE;
}

BOOL CCPGShearConnGroupDlg::CompareElemAttribute(T_ELEM_K KeyRef, T_ELEM_K KeyNew)
{
	CDgnDataCtrl DataCtrl;
	T_ELEM_D DataElemRef, DataElemNew;
	T_SECT_D DataSectRef, DataSectNew;
	
	if(m_pDoc->m_pAttrCtrl->GetElem(KeyRef, DataElemRef) && m_pDoc->m_pAttrCtrl->GetElem(KeyNew, DataElemNew))
	{ 
		if(DataElemRef.eltyp != DataElemNew.eltyp) {m_nFailureType = 1; return FALSE;}// 타입
		if(DataElemRef.elmat != DataElemNew.elmat) {m_nFailureType = 2; return FALSE;}// 재질
		if(DataElemRef.elpro != DataElemNew.elpro) {m_nFailureType = 3; return FALSE;}// 단면

		m_pDoc->m_pAttrCtrl->GetSect(DataElemRef.elpro, DataSectRef);
		m_pDoc->m_pAttrCtrl->GetSect(DataElemNew.elpro, DataSectNew); // Same Section 이지만, Tapered Sect인경우 
//     if (DataSectRef.nStype==D_SECT_TYPE_TAPERED || DataSectNew.nStype==D_SECT_TYPE_TAPERED)
//     {m_nFailureType = 4; return FALSE;}
	
		if(fabs(DataElemRef.angle - DataElemNew.angle - (int)((DataElemRef.angle - DataElemNew.angle)/360.)*360.) > 1.0e-6 &&
			 fabs(DataElemRef.angle - (int)(DataElemRef.angle/360.)*360. - DataElemNew.angle + (int)(DataElemNew.angle/360.)*360.) > 1.0e-6) 
	  {m_nFailureType = 5; return FALSE;}// beta angle
		if(DataCtrl.Get_DgnGenMbtp(KeyRef) != DataCtrl.Get_DgnGenMbtp(KeyNew)) 
		{m_nFailureType = 6; return FALSE;}// member type
	}
	else return FALSE;
	return TRUE;
}

void CCPGShearConnGroupDlg::GetFrmDirectionVector(T_ELEM_D& DataElem, double& Ux, double& Uy, double& Uz)
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
BOOL CCPGShearConnGroupDlg::IsSupportAndColumnWallUpperExist(T_ELEM_K KeyElem, T_NODE_K KeyNode)
{
	if(IsSupportExist(KeyElem, KeyNode) && IsExistColumnWallUpper(KeyNode)) 
		return TRUE;
	else 
		return FALSE;
}

// 수평 부재(보나 가새)의 절점에 Z 방향의 스프링이나 구속이 있는지 확인하는 함수
BOOL CCPGShearConnGroupDlg::IsSupportExist(T_ELEM_K KeyElem, T_NODE_K KeyNode)
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
BOOL CCPGShearConnGroupDlg::IsExistColumnWallUpper(T_NODE_K KeyNode)
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
void CCPGShearConnGroupDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_DGN_CHK_INDEX), _LS(IDS_DGN_NAME), _LS(IDS_DGN_SCGD_LENGTH),
																			_LS(IDS_DGN_START_ELEM), _LS(IDS_DGN_EMD_ELEM), _LS(IDS_DGN_CHK_ELEM_LIST)};  
	// _T("Group Index") , _T("Group Name"), _T("Group Length") , _T(" Start Elem No.") , _T("End Elem No."), Elem List_T("
	int nColWidth[] = {55, 55, 95, 90, 90, 90};
	int nAlign[] = {LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_RIGHT, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_LEFT};
	int nColNum = sizeof(nColWidth)/sizeof(int);
	CString title;  
	LV_COLUMN lvcolumn;
	
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	m_List.GetHeaderCtrl()->EnableWindow(FALSE); // Header Column Size 고정
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

void CCPGShearConnGroupDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl2->GetCountScgd();
	if(nItemCount == 0) return;

	POSITION pos;
	int nCount, nIndex;  

	if(nItemCount)
	{
		T_SCGD_K Key;
		T_SCGD_D Data;
		T_ELEM_K KeyElem;
		CString strMembIndex, strElemList;;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl2->GetStartScgd();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl2->GetNextScgd(pos, Key, Data);		  
			KeyBuf[nCount++] = Data.arGroupElemKey[0];
		}
		qsort(KeyBuf, nItemCount, sizeof(T_ELEM_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  KeyElem = KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl2->GetScgdAssigned(KeyElem, Key);
			m_pDoc->m_pAttrCtrl2->GetScgd(Key, Data);

			CString strMembIndex;

			// Column 0 : Group Index
			strMembIndex.Format(_T("%d"), Data.ScgdKey); // Group Index는 Shear Connector Group Key로 표현
			nIndex = m_List.InsertItem(nCount, strMembIndex);      

			// Column 1 : Group Name
			CString csGroupNameItem;
			csGroupNameItem.Format(_T("%s"), Data.strShearConnGroupName);
			m_List.SetItemText(nIndex, 1, csGroupNameItem);		
			m_List.SetItemData(nIndex, (DWORD)Key);

			// Column 2 : Group Length
			CString csGroupLengthItem;
			csGroupLengthItem.Format(_T("%.2f"), double(Data.dLength));
			m_List.SetItemText(nIndex, 2, csGroupLengthItem);		
			m_List.SetItemData(nIndex, (DWORD)Key);

			// Column 3 : Group Start Elem Key
			CString csStartElemNo;
			csStartElemNo.Format(_T("%d"), Data.StartElemKey);
			m_List.SetItemText(nIndex, 3, csStartElemNo);		
			m_List.SetItemData(nIndex, (DWORD)Key);

			// Column 4 : Group End Elem Key
			CString csEndElemNo;
			csEndElemNo.Format(_T("%d"), Data.EndElemKey);
			m_List.SetItemText(nIndex, 4, csEndElemNo);		
			m_List.SetItemData(nIndex, (DWORD)Key);

			// Column 5 : Group Element List
			CString csElemListItem;
			strElemList = _T("");
			for(int i = 0; i < Data.arGroupElemKey.GetSize(); i++)
			{
				csElemListItem.Format(_T("%d"), Data.arGroupElemKey[i]);
				if(i == 0)  strElemList += csElemListItem;
				else  strElemList +=  _T(", ") + csElemListItem;
			}

			m_List.SetItemText(nIndex, 5, strElemList);		
			m_List.SetItemData(nIndex, (DWORD)Key);
		}
		delete []KeyBuf;
	}
}
			
void CCPGShearConnGroupDlg::OnDelKeyPressedInList()
{  
	CArray<T_SCGD_K, T_SCGD_K> aKeyMemb;
	CArray<int, int> aSelItem;
	aSelItem.RemoveAll();
	aKeyMemb.RemoveAll();  

	GetSelectedItemList(&m_List, aSelItem);
	int nSize = aSelItem.GetSize();
	if(nSize == 0) return;

	for(int i = 0; i < nSize; i++)
	{
		DWORD(KeyMemb) = m_List.GetItemData(aSelItem[i]);    
		if(m_pDoc->m_pAttrCtrl2->ExistScgd(KeyMemb)) aKeyMemb.Add(KeyMemb);
	}  
	if(m_pDoc->m_pDataCtrl->DelScgd(aKeyMemb)) InitSelectedItem();

	SetListFocus(aSelItem);
}

// member를 삭제한 경우 List 박스의 Focus를 다시 맞춤
void CCPGShearConnGroupDlg::SetListFocus(CArray<int, int>& aSelItem)
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

BOOL CCPGShearConnGroupDlg::ShearConnData2Dlg(const T_SCGD_D &crData)
{
	m_dNum.SetEditUnit(crData.nNum);
	m_dFu.SetEditUnit(crData.dfu);
	m_dDs.SetEditUnit(crData.dDia);
	m_dHsc.SetEditUnit(crData.dhsc);
	m_dSt.SetEditUnit(crData.dSpace);
	m_dGroupName.SetWindowText(crData.strShearConnGroupName);
	m_dLength.SetEditUnit(crData.dLength);

 UpdateData(FALSE);

	return TRUE;
}

	void CCPGShearConnGroupDlg::UpdataCtrlUnit()
	{
		int nListItemNum = m_List.GetItemCount();
		if(nListItemNum < 1) return;

		for(int i = 0; i < nListItemNum; ++i)
		{
			CString strLength = m_List.GetItemText(i,2); // Length
			double dLength = _ttoi(strLength);
			dLength = m_pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(CUnitCtrl::m_SCGD_UNIT.dLength, dLength);
			strLength.Format(_T("%.2f"), dLength);
			m_List.SetItemText(i,2,strLength);
		}    
		return;
	}

BOOL CCPGShearConnGroupDlg::Dlg2ShearConnData(T_SCGD_D &rData)
{
	 UpdateData(TRUE);

	 rData.nNum = m_dNum.GetEditValue();
	 rData.dfu = m_dFu.GetEditValue();
	 rData.dDia = m_dDs.GetEditValue();
	 rData.dhsc = m_dHsc.GetEditValue();
	 rData.dSpace = m_dSt.GetEditValue();
	 m_dGroupName.GetWindowText(rData.strShearConnGroupName);

	 if(m_nGroupLengthOpt == 0) // Auto
	 {
		 GetElemLength(rData.arGroupElemKey, rData.dLength);
	 }
	 else if(m_nGroupLengthOpt == 1) // User
	 {
		 rData.dLength = m_dLength.GetEditValue();
	 }
	 else ASSERT(0);

	 int nElemSize = rData.arGroupElemKey.GetSize();
	 if(nElemSize < 1) return FALSE;
	 else
	 {
		 rData.StartElemKey = rData.arGroupElemKey[0];
		 rData.EndElemKey = rData.arGroupElemKey[nElemSize-1];
	 }

	 if(!CheckInData(rData)) return FALSE;   

	return TRUE;
}

BOOL CCPGShearConnGroupDlg::CheckInData(const T_SCGD_D &crData)
{
	double dZero = CMathFunc::m_NormalZero;
	if(crData.nNum < dZero) {GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_SCGD_ERR_NUM)); return FALSE;}
	if(crData.dDia < dZero) {GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_SCGD_ERR_DIA)); return FALSE;}
	if(crData.dfu < dZero) {GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_SCGD_ERR_FU)); return FALSE;}
	if(crData.dhsc < dZero) {GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_SCGD_ERR_STUD_HEIGHT)); return FALSE;}
	if(crData.dLength < dZero)  {GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_SCGD_ERR_GROUP_LENGTH)); return FALSE;}
	if(crData.dSpace < dZero) {GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_SCGD_ERR_LONGI_SPACE)); return FALSE;}
	if(crData.strShearConnGroupName.IsEmpty()) {GSaveHistoryFormatNF(_LS(IDS_DGN_CHK_SCGD_ERR_LONGI_SPACE)); return FALSE;}
	return TRUE;
}

	BOOL CCPGShearConnGroupDlg::GetElemLength(CArray<T_ELEM_K, T_ELEM_K>& aSelElem, double &dLength)
	{
		dLength = 0.0;
		int nElemNum = aSelElem.GetSize();
		for(int k = 0; k < nElemNum; ++k)
		{
				dLength += m_pDoc->calcLAVElem(aSelElem[k]);
		}
		
		return TRUE;
	}

BOOL CCPGShearConnGroupDlg::PreTranslateMessage(MSG* pMsg) 
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

void CCPGShearConnGroupDlg::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;	 
	if (pNMListView->iSubItem != 999)
	{
		if (!m_bSentMessage)
		{
			m_TempListView = *pNMListView;
			m_TempListView.iSubItem = 999;
			PostMessage(WM_NOTIFY, IDC_DGN_CPG_SHEAR_CONN_GROUP_LIST, (LPARAM)&m_TempListView);
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
void CCPGShearConnGroupDlg::DisplaySelElemList()
{
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelElem;  
	aSelElem.RemoveAll();
	GetSelElemList(aSelElem);  
	m_pDoc->m_pViewCtrl->SelectElem(NULL, aSelElem, FALSE, TRUE);
}

void CCPGShearConnGroupDlg::GetSelElemList(CArray<T_ELEM_K, T_ELEM_K>& aSelElem)
{
	T_SCGD_D DataMemb;
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
	m_pDoc->m_pAttrCtrl2->GetKeyListScgdUser(&aMembKey, 0, &aSelElem);
}      

void CCPGShearConnGroupDlg::GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem)
{
	int  nItem = -1;
	UINT uSelectedCount = pList->GetSelectedCount();

	CArray<T_SCGD_K, T_SCGD_K> arGroupDKey; arGroupDKey.RemoveAll();
	aSelItem.SetSize(uSelectedCount);
	arGroupDKey.SetSize(uSelectedCount);
	CString strScgdKey;


	for(UINT i = 0; i < uSelectedCount; i++)
	{
		nItem = pList->GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		aSelItem[i] = nItem;

			strScgdKey = m_List.GetItemText(nItem,0);
#ifdef _UNICODE
			arGroupDKey[i] = _wtoi(strScgdKey);
#else
			arGroupDKey[i] = _ttoi(strScgdKey);
#endif
	}

	if(uSelectedCount < 1) return; // 선택된 아이템이 없으면 리턴
	 T_SCGD_K ScgdKey = 0; 
	ScgdKey = arGroupDKey[uSelectedCount-1]; // 가장 마지막으로 선택된 Group 정보를 대표로 Display 해 줌.

	T_SCGD_D ScgdD;
	if(!m_pDoc->m_pAttrCtrl2->GetScgd(ScgdKey, ScgdD)) return;
	ShearConnData2Dlg(ScgdD);

}
