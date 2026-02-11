#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdCheckNearNodesDlg.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_UndoCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\FlagCtrl.h"
#include "..\wg_db\ViewCtrl.h"
#include "..\wg_db\Qsort.h"
#include "..\wg_base\wg_base_DlgUtil.h"

/////////////////////////////////////////////////////////////////////////////
// CCmdCheckNearNodesDlg dialog

CmdCheckNode::CmdCheckNode()
{
	l_child = NULL;
	r_child = NULL;
}

CmdCheckNode::CmdCheckNode(unsigned int key, double val)
{
	m_Key = key;
	m_Val = val;

	l_child = NULL;
	r_child = NULL;
}

CmdCheckNode::~CmdCheckNode()
{
	if (l_child)
		delete l_child;
	if (r_child)
		delete r_child;
}

struct NodeDist
{
	T_NODE_K NodeK;
	double   dDist;
	static int Comp(const void* p1, const void* p2)
	{
		NodeDist* v1 = (NodeDist*)p1;
		NodeDist* v2 = (NodeDist*)p2;
		if (v1->dDist > v2->dDist) return 1;
		else if (v1->dDist < v2->dDist) return -1;
		return 0;
	}
};

void CmdCheckNode::Insert(unsigned int key, double value)
{
	CmdCheckNode* pCur;
	pCur = this;
	while (pCur)
	{
		if (value < pCur->m_Val) 
		{
			if (pCur->l_child) pCur = pCur->l_child;
			else 
			{
				pCur->l_child = new CmdCheckNode(key, value);
				break;
			}
		}
		else if (value > pCur->m_Val)
		{
			if (pCur->r_child) pCur = pCur->r_child;
			else
			{
				pCur->r_child = new CmdCheckNode(key, value);
				break;
			}
		}
		else
		{
			CmdCheckNode* temp = new CmdCheckNode(key, value);
			temp->r_child = pCur->r_child;
			pCur->r_child = temp;
			break;
		}
	}
}

void CmdCheckNode::FastInsertSortedList(CArray<UINT, UINT>& aKey, CArray<double, double>& aVal, int nStart, int nEnd)
{
	if (nStart <= nEnd)
	{
		int nMid = (nStart+nEnd)/2;
		Insert(aKey[nMid], aVal[nMid]);
		FastInsertSortedList(aKey, aVal, nStart, nMid-1);
		FastInsertSortedList(aKey, aVal, nMid+1, nEnd);
	}
}

void CmdCheckNode::SearchValueRange(CmdCheckNode *root, double lower_bound, double upper_bound,
								CArray <unsigned int, unsigned int> &node_list,
								unsigned int nCurKey)
{
	if (!root)
		return;
	// go down left
	if (root->m_Val >= lower_bound)
		SearchValueRange(root->l_child, lower_bound, upper_bound, node_list, nCurKey);

	// visit current node
	if ((root->m_Val >= lower_bound) && (root->m_Val <= upper_bound)) 
	{
		if(root->m_Key > nCurKey)
			node_list.Add(root->m_Key);
	}

	// go down right
	if (root->m_Val <= upper_bound)
		SearchValueRange(root->r_child, lower_bound, upper_bound, node_list, nCurKey);
}

CCmdCheckNearNodesDlg::CCmdCheckNearNodesDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCmdCheckNearNodesDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nOption = 0;
	m_Tree = 0;
}

CCmdCheckNearNodesDlg::~CCmdCheckNearNodesDlg()
{
	if (m_Tree)
		delete m_Tree;
}

void CCmdCheckNearNodesDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_CMD_OPTION_NUMBER, m_nOption);
	DDX_Control(pDX, IDC_CMD_TOLERANCE_UNIT, m_wndToleranceUnit);
	DDX_Control(pDX, IDC_CMD_TOLERANCE_EDIT, m_wndToleranceEdit);
	DDX_Control(pDX, IDC_CMD_TOLERANCE_UNIT2, m_wndToleranceUnit2);
	DDX_Control(pDX, IDC_CMD_TOLERANCE_EDIT2, m_wndToleranceEdit2);
}

BEGIN_MESSAGE_MAP(CCmdCheckNearNodesDlg, CCMDlgBase)
	//{{AFX_MSG_MAP(CCmdCheckNearNodesDlg)
	ON_BN_CLICKED(IDC_CMD_CHECK_NODES_APPLY_BTN , OnApplyBtn	)
	ON_BN_CLICKED(IDC_CMD_CHECK_NODES_CANCEL_BTN, OnCloseBtn	)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdCheckNearNodesDlg message handlers

BOOL CCmdCheckNearNodesDlg::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	m_wndToleranceUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndToleranceEdit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndToleranceUnit2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndToleranceEdit2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndToleranceEdit.SetEditUnit(pDoc->m_pFlagCtrl->GetMergeToleranceCurrentUnit());
	m_wndToleranceEdit2.SetEditUnit(pDoc->m_pFlagCtrl->GetMergeToleranceCurrentUnit());
	return TRUE;
}

void CCmdCheckNearNodesDlg::OnApplyBtn() 
{
	UpdateData(TRUE);
	if(m_nOption == 0)
	{
		if(CheckNearNode())
		{
#ifndef _DEBUG
			OnCancel();//Close Cur Dialog
#endif
		}
	}
	else
		CheckShortBeam();
}

void CCmdCheckNearNodesDlg::OnCloseBtn() 
{
	CCMDlgBase::OnOK();
}

BOOL CCmdCheckNearNodesDlg::CheckNearNode()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double Tolerance = m_wndToleranceEdit.GetEditValue();
	CArray<T_NODE_K, T_NODE_K> aNodeK;
	CArray<T_NODE_K, T_NODE_K> aSearchNodeK;
	pDoc->m_pViewCtrl->GetAllSelectedNode(aNodeK);
	if(aNodeK.IsEmpty())
		pDoc->m_pAttrCtrl->GetNodeKeyList(aNodeK);
	else
	{
		//Sort SelectNodeK
		CQSort::QSortUInt(aNodeK.GetData(), aNodeK.GetSize());
	}
	InitTree(aNodeK);
	//ASSERT aNodeK is SortKey
	//Find only nodes in the tree that are greater than the current key
	pDoc->m_pUnitCtrl->ConvertUnitLengthIn(Tolerance);
	T_NODE_D Data;
	T_NODE_D DataComp;
	int nSize = 0;
	for(int i = 0;i < aNodeK.GetSize();i++)
	{
		VERIFY(pDoc->m_pAttrCtrl->GetNode(aNodeK[i], Data));
		double dblDistance=sqrt(Data.x*Data.x + Data.y*Data.y + Data.z*Data.z);
		FindSameDistNodeList(dblDistance,Tolerance,aSearchNodeK,aNodeK[i]);
		if(nSize < aSearchNodeK.GetSize())
		{
			for(int j = aSearchNodeK.GetSize()-1;j >= nSize;j--)
			{
				VERIFY(pDoc->m_pAttrCtrl->GetNode(aSearchNodeK[j], DataComp));
				if(fabs(Data.x-DataComp.x) > Tolerance || fabs(Data.y-DataComp.y) > Tolerance || fabs(Data.z-DataComp.z) > Tolerance)
					aSearchNodeK.RemoveAt(j);
			}
			if(nSize < aSearchNodeK.GetSize())
				aSearchNodeK.Add(aNodeK[i]);//Contain CurKey
		}
		nSize = aSearchNodeK.GetSize();
	}
	CMap<T_NODE_K,T_NODE_K,BOOL,BOOL> mapFiltKey;
	for(int i = aSearchNodeK.GetSize()-1 ;i >=0 ;i--)
	{
		if(mapFiltKey.PLookup(aSearchNodeK[i]))
			aSearchNodeK.RemoveAt(i);
		mapFiltKey[aSearchNodeK[i]] = TRUE;
	}
	nSize = aSearchNodeK.GetSize();
	CString strText;
	if(nSize > 0)
	{
		strText.Format(_LS(IDS_CMD_CHECKNODE_AUTO_MERGE_NEARNODES),nSize);
		if(AfxMessageBox(strText,MB_OKCANCEL) == MB_OKCANCEL)
		{
			//T_NODE_K nStartNum = 1;
			//T_NODE_K nEndNum = pDoc->m_pAttrCtrl->GetLastNumNode();
			if (!pDoc->m_pDataCtrl->MergeNode_Select(aSearchNodeK, Tolerance, TRUE))
			{ 
				return pDoc->DisplayHistoryMessage(_LS(IDS_CMD_CHECKNODE_CHECKNODE_FAIL));
			}
		}
		else
		{
			pDoc->m_pViewCtrl->UnSelectAllNode(NULL);
			pDoc->m_pViewCtrl->SelectNode(NULL, aSearchNodeK, FALSE, TRUE);
		}
		return TRUE;
	}
	else
	{
		strText.Format(_LS(IDS_CMD_CHECKNODE_NO_NEAR_NODES));
		AfxMessageBox(strText,MB_OK);
	}
	return FALSE;
}

BOOL CCmdCheckNearNodesDlg::CheckShortBeam()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double Tolerance = m_wndToleranceEdit2.GetEditValue();
	T_ELEM_K_LIST aElemK;
	T_ELEM_K_LIST aSearchElemK;
	pDoc->m_pViewCtrl->GetAllSelectedElem(aElemK);
	T_ELEM_D ElemD;T_NODE_D StartNodeD,EndNodeD;
	for(int i = aElemK.GetSize()-1;i >= 0;i--)
	{
		pDoc->m_pAttrCtrl->GetElem(aElemK[i],ElemD);
		if(!pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp))
			aElemK.RemoveAt(i);
	}
	if(aElemK.IsEmpty())
	{
		pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
		for(int i = aElemK.GetSize()-1;i >=0;i--)
		{
			pDoc->m_pAttrCtrl->GetElem(aElemK[i],ElemD);
			if(!pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp))
				aElemK.RemoveAt(i);
		}
		if(aElemK.IsEmpty())
			return FALSE;
	}
	pDoc->m_pUnitCtrl->ConvertUnitLengthIn(Tolerance);

	for(int i = 0;i < aElemK.GetSize();i++)
	{
		pDoc->m_pAttrCtrl->GetElem(aElemK[i],ElemD);
		pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0],StartNodeD);
		pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1],EndNodeD);
		if(fabs(StartNodeD.x-EndNodeD.x) < Tolerance && fabs(StartNodeD.y-EndNodeD.y) < Tolerance && fabs(StartNodeD.z-EndNodeD.z) < Tolerance)
			aSearchElemK.Add(aElemK[i]);
	}
	int nSize = aSearchElemK.GetSize();
	CString strText;
	if(nSize > 0)
	{
		strText.Format(_LS(IDS_CMD_CHECKNODE_AUTO_MERGE_SHORTBEAM),nSize);
		if(AfxMessageBox(strText,MB_OKCANCEL) == MB_OKCANCEL)
		{
			return MergeShortBeam(aSearchElemK);
		}
		else
		{
			pDoc->m_pViewCtrl->UnSelectAllElem(NULL,NULL);
			pDoc->m_pViewCtrl->SelectElem(NULL, aSearchElemK, FALSE, TRUE);
		}
		return TRUE;
	}
	else
	{
		strText.Format(_LS(IDS_CMD_CHECKNODE_NO_SHORT_BEAM));
		AfxMessageBox(strText,MB_OK);
		return TRUE;
	}
	return FALSE;
}

BOOL CCmdCheckNearNodesDlg::MergeShortBeam(T_ELEM_K_LIST& aSearchElemK)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CArray<T_NODE_K, T_NODE_K> aNodeDelK;
	T_ELEM_D ElemD;
	T_NODE_D StartNodeD,EndNodeD;
	int nSize = aSearchElemK.GetSize();
	pDoc->m_pViewCtrl->UnSelectAllElem(NULL,NULL);
	//if(!pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_CMD_CHECKNODE_MERGE_SHORTBEAM)), CMDTYPE_REMOVE_ANALYSIS))return FALSE;
	if(!pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_CMD_CHECKNODE_MERGE_SHORTBEAM)), CMDTYPE_REMOVE_ANALYSIS))return FALSE;
	CMap<T_ELEM_K, T_ELEM_K, UINT, UINT> mapNeedDelElem;
	for(int i=0; i<nSize; i++)
	{
		if(mapNeedDelElem.PLookup(aSearchElemK[i]))
			continue;
		mapNeedDelElem.SetAt(aSearchElemK[i], 1);
		if (!pDoc->m_pAttrCtrl->GetElem(aSearchElemK[i], ElemD))
		{
			ASSERT(FALSE);
			continue;
		}
		pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0],StartNodeD);
		pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1],EndNodeD);
		T_ELEM_K_LIST aNeedModifyElemK;
		T_ELEM_D TempElemD;
		pDoc->m_pAttrCtrl->GetElemKeyListConnectedToNode(ElemD.elnod[1],aNeedModifyElemK);
		aNodeDelK.Add(ElemD.elnod[1]);
		for(int j = 0;j < aNeedModifyElemK.GetSize();j++)
		{
			if(aNeedModifyElemK[j] == aSearchElemK[i])
				continue;
			pDoc->m_pAttrCtrl->GetElem(aNeedModifyElemK[j],TempElemD);
			if(TempElemD.elnod[0] == ElemD.elnod[1])
			{
				TempElemD.elnod[0] = ElemD.elnod[0];
			}
			else if(TempElemD.elnod[1] == ElemD.elnod[1])
			{
				TempElemD.elnod[1] = ElemD.elnod[0];
			}
			else
			{
				ASSERT(FALSE);
				continue;
			}
			//-----Check ModifyElem,if nodes of the changed modifyelem Exist,Dell it-----//
			T_ELEM_D CheckElemD;
			T_ELEM_K_LIST aCheckElemK;
			pDoc->m_pAttrCtrl->GetElemKeyListConnectedToNode(ElemD.elnod[0],aCheckElemK);
			BOOL bExist = FALSE;
			for(int k = 0;k < aCheckElemK.GetSize();k++)
			{
				if(aNeedModifyElemK[j] == aCheckElemK[k])
					continue;
				pDoc->m_pAttrCtrl->GetElem(aCheckElemK[k],CheckElemD);
				if((CheckElemD.elnod[0] == TempElemD.elnod[0] && CheckElemD.elnod[1] == TempElemD.elnod[1]) ||
					(CheckElemD.elnod[1] == TempElemD.elnod[0] && CheckElemD.elnod[0] == TempElemD.elnod[1]))
				{
					bExist = TRUE;
					break;
				}
			}
			if(bExist)
			{
				mapNeedDelElem.SetAt(aNeedModifyElemK[j], 1);
				continue;
			}
			//-----****************************************************************-----//
			if(!pDoc->m_pEditData->ModifyElem(aNeedModifyElemK[j],aNeedModifyElemK[j], TempElemD))
			{
				ASSERT(FALSE);
				pDoc->m_pUndoCtrl->CancelEditDB();
				return FALSE;
			}
		}
	}
	POSITION pos = mapNeedDelElem.GetStartPosition();
	T_ELEM_K ElemK;
	UINT nTemp;
	while (pos)
	{
		mapNeedDelElem.GetNextAssoc(pos, ElemK, nTemp);
		if (!pDoc->m_pEditData->DelElem(ElemK))
		{
			ASSERT(FALSE);
			pDoc->m_pUndoCtrl->CancelEditDB();
			return FALSE;
		}
	}
	for(int i=0; i<aNodeDelK.GetSize(); i++)
	{
		if(!pDoc->m_pEditData->DelNode(aNodeDelK[i]))
		{
			ASSERT(FALSE);
			pDoc->m_pUndoCtrl->CancelEditDB();
			return FALSE;
		}
	}
	pDoc->m_pUndoCtrl->CloseEditDB(TRUE);
	return TRUE;
}


void CCmdCheckNearNodesDlg::FindSameDistNodeList(double distance, double tolerance,CArray<T_NODE_K, T_NODE_K>& node_list,T_NODE_K nCurKey)
{
	double    lower_bound, upper_bound;
	CmdCheckNode  *root;

	if (distance < 0.0)
		distance = -distance;

	lower_bound = distance - fabs(tolerance);
	upper_bound = distance + fabs(tolerance);
	root = m_Tree;
	m_Tree->SearchValueRange(root, lower_bound, upper_bound, node_list, nCurKey);
}

void CCmdCheckNearNodesDlg::InitTree(const CArray<T_NODE_K, T_NODE_K>& aNodeK)
{
	if(aNodeK.IsEmpty())
		return;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CArray<NodeDist, NodeDist&> aNodeDist;
	aNodeDist.SetSize(aNodeK.GetSize());
	T_NODE_D NodeD;
	for(int i = 0;i < aNodeK.GetSize();i++)
	{		
		pDoc->m_pAttrCtrl->GetNode(aNodeK[i],NodeD);
		aNodeDist[i].NodeK = aNodeK[i];
		aNodeDist[i].dDist = sqrt(NodeD.x*NodeD.x+NodeD.y*NodeD.y+NodeD.z*NodeD.z);
	}
	qsort(aNodeDist.GetData(), aNodeDist.GetSize(), sizeof(NodeDist), NodeDist::Comp);
	CArray<UINT, UINT> aKey;
	CArray<double, double> aDist;
	aKey.SetSize(aNodeDist.GetSize());
	aDist.SetSize(aNodeDist.GetSize());
	for (int i = 0; i < aNodeDist.GetSize(); i++)
	{
		aKey[i] = aNodeDist[i].NodeK;
		aDist[i] = aNodeDist[i].dDist;
	}
	InitSortedList(aKey,aDist);
}

void CCmdCheckNearNodesDlg::InitSortedList(CArray<UINT, UINT>& aKey, CArray<double, double>& aDist)
{
	int nSize = aKey.GetSize();
	int nMid = nSize/2;
	if(m_Tree)
	{
		delete m_Tree;
		m_Tree = nullptr;
	}
	m_Tree = new CmdCheckNode(aKey[nMid], aDist[nMid]);
	m_Tree->FastInsertSortedList(aKey, aDist, 0, nMid-1);
	m_Tree->FastInsertSortedList(aKey, aDist, nMid+1, nSize-1);
	//m_NodeNum = nSize;
}

