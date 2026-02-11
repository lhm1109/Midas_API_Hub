// DB_JUDResultCtrl.cpp: implementation of the CDB_JUDResultCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_db.h"
#include "DB_JUDResultCtrl.h"

#include "wg_db_DBDoc.h"
#include "wg_db_AttrCtrl.h"
#include "wg_db_AttrCtrl2.h"
#include "wg_db_PostCtrl.h"
#include "wg_db_FlagCtrl.h"
#include "StoryDisplDrift.h"

#include "..\wg_base\wg_base_Msgdll.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDB_JUDResultCtrl::CDB_JUDResultCtrl(CWnd *pParent/*=NULL*/)
{  
	m_pDoc = CDBDoc::GetDocPoint();  
	
	m_mapFrameName  .InitHashTable(997);
	m_mapFrameItem_X.InitHashTable(997);   
	m_mapFrameItem_Y.InitHashTable(997);   
	m_mapStoryItem  .InitHashTable(997);
	m_mapElemKList_Frame.InitHashTable(HASHSIZEELEM);
	m_mapElemKList_Story.InitHashTable(HASHSIZEELEM);
	
	m_mapLcom_Anal.InitHashTable(997);
	m_mapLcom_STL .InitHashTable(997);
	m_mapLcom_RC  .InitHashTable(997);
	m_mapLcom_DS  .InitHashTable(997);
	m_mapLcom_Hor .InitHashTable(997);
}

CDB_JUDResultCtrl::~CDB_JUDResultCtrl()
{
	DeleteElemList();
}

BOOL CDB_JUDResultCtrl::MakeElemList()
{
	DeleteElemList();

	MakeFrameElemList();
	MakeStoryElemList();    

	return TRUE;
}

BOOL CDB_JUDResultCtrl::MakeStoryElemList()
{
	m_mapStoryItem.RemoveAll();
	
	CString strStory;
	CArray<T_STOR_BASE_TYPE_N,T_STOR_BASE_TYPE_N&> aStorBase;
	CStoryDisplDrift StroyDrift;
	StroyDrift.GetStoryBaseData_TypeN(aStorBase);
	for (int i=0; i<aStorBase.GetSize(); i++)
	{
		T_STOR_BASE_TYPE_N StorBase = aStorBase[i];    
		if(StorBase.strMdulName == "") strStory = StorBase.strStorName;
		else                           strStory.Format("%s:%s", StorBase.strMdulName, StorBase.strStorName);     
		m_mapStoryItem.SetAt(StorBase.StorK, strStory);
		
		CArray<T_ELEM_K,T_ELEM_K>* pElemKList;
		if(m_mapElemKList_Story.Lookup(StorBase.StorK, pElemKList))
		{
			ASSERT(0);
			delete pElemKList;
			pElemKList = NULL;
		}    
		pElemKList = new CArray<T_ELEM_K,T_ELEM_K>;  
		pElemKList->Append(StorBase.arBeamK);
		pElemKList->Append(StorBase.arBracK);
		pElemKList->Append(StorBase.arColmK);
		pElemKList->Append(StorBase.arWallK);    
		m_mapElemKList_Story.SetAt(StorBase.StorK, pElemKList);
	}
	
	return TRUE;
}

BOOL CDB_JUDResultCtrl::MakeFrameElemList()
{
	int nGrid = m_pDoc->m_pAttrCtrl->GetCountGdln();
	if(nGrid == 0) return FALSE;
	
	double dTol = m_pDoc->m_pFlagCtrl->GetIntsToleranceCurrentUnit();

	m_mapFrameName.RemoveAll();
	m_mapFrameItem_X.RemoveAll();
	m_mapFrameItem_Y.RemoveAll();  
	
	CString strGridName;
	T_GDLN_DT arXGrid;
	T_GDLN_DT arYGrid;  
	POSITION pos = m_pDoc->m_pAttrCtrl->GetStartGdln();
	while(pos != NULL)
	{
		arXGrid.RemoveAll();
		arYGrid.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetNextGdln(pos, strGridName, arXGrid, arYGrid);
		int nSize_X = arXGrid.GetSize();
		int nSize_Y = arYGrid.GetSize();
		if(nSize_X<2 || nSize_Y<2) continue; // X축과 Y축의 갯수가 2개 이상 일 때 Grid를 생성할 수 있다.
		T_GDLN_K gdlnK = m_pDoc->m_pAttrCtrl->GetGdlnKey(strGridName);
		if(gdlnK == 0) {ASSERT(0); continue;}
		m_mapFrameName.SetAt(gdlnK, strGridName);        

		double dStartX = arXGrid[0];
		double dEndX   = arXGrid[nSize_X-1];
		
		double dStartY = arYGrid[0];
		double dEndY   = arYGrid[nSize_Y-1];
		
		UINT2 nAxis;        
		CString strX, strY;
		double dPlanePt[3][3]; 
		for (int nIndex=0; nIndex<nSize_X; nIndex++) 
		{
			nAxis.i1 = gdlnK;
			nAxis.i2 = nIndex; 
			strX.Format("X%d", nIndex+1);  // X1부터 시작
			m_mapFrameItem_X.SetAt(nAxis, strX);  
			
			CArray<T_ELEM_K,T_ELEM_K>* pElemKList;
			if(m_mapElemKList_Frame.Lookup(nAxis, pElemKList))
			{
				ASSERT(0);
				delete pElemKList;
				pElemKList = NULL;
			}
			 
			dPlanePt[0][0] = arXGrid[nIndex];
			dPlanePt[0][1] = dStartY;
			dPlanePt[0][2] = 0.;

			dPlanePt[1][0] = arXGrid[nIndex];
			dPlanePt[1][1] = dEndY;
			dPlanePt[1][2] = 0.;

			dPlanePt[2][0] = arXGrid[nIndex];
			dPlanePt[2][1] = dEndY;
			dPlanePt[2][2] = 1.;

			pElemKList = new CArray<T_ELEM_K,T_ELEM_K>;       
			if(!m_pDoc->m_pAttrCtrl2->GetElemKeyListByPlane(dPlanePt, pElemKList, dTol)) 
			{
				delete pElemKList;
				pElemKList = NULL;
				continue;   
			}
			m_mapElemKList_Frame.SetAt(nAxis, pElemKList);                          
		}
		for (nIndex=0; nIndex<nSize_Y; nIndex++)  
		{
			nAxis.i1 = gdlnK;
			nAxis.i2 = nIndex + nSize_X;  // Y축은 X축에 이어서 생성
			strY.Format("Y%d", nIndex+1);
			m_mapFrameItem_Y.SetAt(nAxis, strY);

			CArray<T_ELEM_K,T_ELEM_K>* pElemKList;
			if(m_mapElemKList_Frame.Lookup(nAxis, pElemKList))
			{
				ASSERT(0);
				delete pElemKList;
				pElemKList = NULL;
			}
			
			dPlanePt[0][0] = dStartX;
			dPlanePt[0][1] = arYGrid[nIndex];
			dPlanePt[0][2] = 0.;
			
			dPlanePt[1][0] = dEndX;
			dPlanePt[1][1] = arYGrid[nIndex];
			dPlanePt[1][2] = 0.;
			
			dPlanePt[2][0] = dEndX;
			dPlanePt[2][1] = arYGrid[nIndex];
			dPlanePt[2][2] = 1.;
			
			pElemKList = new CArray<T_ELEM_K,T_ELEM_K>;       
			if(!m_pDoc->m_pAttrCtrl2->GetElemKeyListByPlane(dPlanePt, pElemKList, dTol)) 
			{
				delete pElemKList;
				pElemKList = NULL;
				continue;   
			}
			m_mapElemKList_Frame.SetAt(nAxis, pElemKList);         
		}        
	}

	return TRUE;
}

void CDB_JUDResultCtrl::DeleteElemList()
{
	UINT2 nKey2;         
	POSITION pos = m_mapElemKList_Frame.GetStartPosition();
	while (pos)
	{
		CArray<T_ELEM_K,T_ELEM_K>* pArray = NULL;
		m_mapElemKList_Frame.GetNextAssoc(pos, nKey2, pArray);
		if(pArray)
		{
			delete pArray;
			pArray = NULL;
		}
	}  

	UINT nKey;
	pos = m_mapElemKList_Story.GetStartPosition();
	while (pos)
	{
		CArray<T_ELEM_K,T_ELEM_K>* pArray = NULL;
		m_mapElemKList_Story.GetNextAssoc(pos, nKey, pArray);
		if(pArray)
		{
			delete pArray;
			pArray = NULL;
		}
	}
}

BOOL CDB_JUDResultCtrl::MakeLcomList()
{
	MakeLcomList_Anal();
	MakeLcomList_Sect();
	MakeLcomList_DS();
	MakeLcomList_Hor();
	
	return TRUE;
}

BOOL CDB_JUDResultCtrl::MakeLcomList_Anal()
{    
	m_mapLcom_Anal.RemoveAll();
	
	T_STLD_K StldK;
	T_STLD_D StldD;  
	CArray<T_STLD_K,T_STLD_K> aStldKey;
	
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldKey);
	int nCount = aStldKey.GetSize();
	for(int i=0; i<nCount; i++) 
	{
		StldK = aStldKey[i];
		StldD.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetStld(StldK, StldD)) {ASSERT(0); continue;}
		m_mapLcom_Anal.SetAt(StldK, StldD.LoadCaseName);
	} 
	
	return TRUE;
}

BOOL CDB_JUDResultCtrl::MakeLcomList_Sect()
{
	m_mapLcom_STL.RemoveAll();
	m_mapLcom_RC .RemoveAll();
	
	CString strAll = "ALL COMBINATION";  

	T_LCOM_K LcomK;
	T_LCOM_D LcomD;    
	CArray<T_LCOM_K,T_LCOM_K> aLcomKey;
	
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_STEEL, aLcomKey);  
	int nCount = aLcomKey.GetSize();
	for(int i=0; i<nCount; i++) 
	{
		LcomK = aLcomKey[i];
		LcomD.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, LcomK, LcomD)) {ASSERT(0); continue;}
		if(LcomD.nActive == 0 || LcomD.LoadCombType != 0) continue; // Inactive이거나 Add Type이 아닌 경우
		if(!m_pDoc->m_pAttrCtrl->IsStaticLCComb(LcomD))   continue;
		m_mapLcom_STL.SetAt(LcomK, LcomD.LoadCombName);
	} 
	if(nCount > 0) m_mapLcom_STL.SetAt(0, strAll);
	
	aLcomKey.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, aLcomKey);  
	nCount = aLcomKey.GetSize();
	for(i=0; i<nCount; i++) 
	{
		LcomK = aLcomKey[i];
		LcomD.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, LcomK, LcomD)) {ASSERT(0); continue;}
		if(LcomD.nActive == 0 || LcomD.LoadCombType != 0) continue;    
		if(!m_pDoc->m_pAttrCtrl->IsStaticLCComb(LcomD))   continue;    
		m_mapLcom_RC.SetAt(LcomK, LcomD.LoadCombName);
	} 
	if(nCount > 0) m_mapLcom_RC.SetAt(0, strAll);
	
	return TRUE;
}

BOOL CDB_JUDResultCtrl::MakeLcomList_DS()
{
	return TRUE;
}

BOOL CDB_JUDResultCtrl::MakeLcomList_Hor()
{
	return TRUE;
}

BOOL CDB_JUDResultCtrl::GetFrameElemList(UINT2 FrameK, CArray<T_ELEM_K,T_ELEM_K>& aElemK)
{
	aElemK.RemoveAll();
	CArray<T_ELEM_K,T_ELEM_K>* pElemArray;
	if(!m_mapElemKList_Frame.Lookup(FrameK, pElemArray)) {ASSERT(0); return FALSE;}  
	aElemK.Copy(*pElemArray);
	return TRUE;
}

int CDB_JUDResultCtrl::GetFrameCount()
{
	return m_mapFrameName.GetCount();
}

int CDB_JUDResultCtrl::GetFrameKeyList(CArray<UINT,UINT>& aFrameKey)
{
	aFrameKey.RemoveAll();
	return m_mapFrameName.GetSortedKeyListforUintK(aFrameKey);
}

BOOL CDB_JUDResultCtrl::GetFrameName(UINT FrameK, CString& strName)
{
	strName = "";
	if(!m_mapFrameName.Lookup(FrameK, strName)) return FALSE;  
	return TRUE;
}

int CDB_JUDResultCtrl::GetFrameAxisList(CArray<UINT2,UINT2>& aFrameKey, int nAxis)
{
	aFrameKey.RemoveAll();
	if(nAxis == D_FRAME_X_AXIS)
	{
		return m_mapFrameItem_X.GetKeyList(aFrameKey);  
	}
	else if(nAxis == D_FRAME_Y_AXIS)
	{
		return m_mapFrameItem_Y.GetKeyList(aFrameKey);
	}
	else  
		ASSERT(0);

	return 0;
}

BOOL CDB_JUDResultCtrl::GetFrameItem(UINT2 FrameK, CString& strItem, int nAxis)
{
	strItem = "";
	if(nAxis == D_FRAME_X_AXIS)
	{
		if(!m_mapFrameItem_X.Lookup(FrameK, strItem)) {ASSERT(0); return FALSE;}  
	}
	else if(nAxis == D_FRAME_Y_AXIS)
	{
		if(!m_mapFrameItem_Y.Lookup(FrameK, strItem)) {ASSERT(0); return FALSE;}  
	}
	else
		ASSERT(0);

	return TRUE;
}

BOOL CDB_JUDResultCtrl::GetStoryElemList(UINT StorK, CArray<T_ELEM_K,T_ELEM_K>& aElemK)
{
	aElemK.RemoveAll();
	CArray<T_ELEM_K,T_ELEM_K>* pElemArray;
	if(!m_mapElemKList_Story.Lookup(StorK, pElemArray)) {ASSERT(0); return FALSE;} 
	aElemK.Copy(*pElemArray);
	return TRUE;
}

int CDB_JUDResultCtrl::GetStorKeyList(CArray<UINT,UINT>& aStoryKey)
{
	aStoryKey.RemoveAll();
	return m_mapElemKList_Story.GetSortedKeyListforUintK(aStoryKey);
}

BOOL CDB_JUDResultCtrl::GetStorItem(UINT StorK, CString& strItem)
{
	strItem = "";
	if(!m_mapStoryItem.Lookup(StorK, strItem)) {ASSERT(0); return FALSE;}  
	return TRUE;
}

int CDB_JUDResultCtrl::GetElemKeyList(int nMode, CArray<UINT,UINT>& aElemK)
{
	aElemK.RemoveAll();
	POSITION pos;
	CArray<T_ELEM_K,T_ELEM_K>* pArray;
	if(nMode == 0)
	{
		UINT2 nKey;
		pos = m_mapElemKList_Frame.GetStartPosition();
		while (pos)
		{
			pArray = NULL;
			m_mapElemKList_Frame.GetNextAssoc(pos, nKey, pArray);
			if(!pArray) continue;
			aElemK.Append(*pArray);
		}
	}
	else if(nMode == 1)
	{
		UINT nKey;
		pos = m_mapElemKList_Story.GetStartPosition();
		while (pos)
		{
			pArray = NULL;
			m_mapElemKList_Story.GetNextAssoc(pos, nKey, pArray);
			if(!pArray) continue;
			aElemK.Append(*pArray);
		}
	}
	else
	{
		ASSERT(0);
		return 0;
	}

	return aElemK.GetSize();
}

int CDB_JUDResultCtrl::GetNodeKeyList(int nMode, CArray<UINT,UINT>& aNodeK)
{
	aNodeK.RemoveAll();
	POSITION pos;
	CArray<T_NODE_K,T_NODE_K> aElemNodeKey;  
	CArray<T_ELEM_K,T_ELEM_K>* pArray;
	if(nMode == 0)
	{
		UINT2 nKey;
		pos = m_mapElemKList_Frame.GetStartPosition();
		while (pos)
		{
			pArray = NULL;
			m_mapElemKList_Frame.GetNextAssoc(pos, nKey, pArray);
			if(!pArray) continue;
			m_pDoc->m_pAttrCtrl->GetElemNodeKeyList(*pArray, aElemNodeKey);
			aNodeK.Append(aElemNodeKey);
		}
	}
	else if(nMode == 1)
	{
		UINT nKey;
		pos = m_mapElemKList_Story.GetStartPosition();
		while (pos)
		{
			pArray = NULL;
			m_mapElemKList_Story.GetNextAssoc(pos, nKey, pArray);
			if(!pArray) continue;
			m_pDoc->m_pAttrCtrl->GetElemNodeKeyList(*pArray, aElemNodeKey);
			aNodeK.Append(aElemNodeKey);
		}
	}
	else
	{
		ASSERT(0);
		return 0;
	}
	
	return aNodeK.GetSize();
}

int CDB_JUDResultCtrl::GetLcomKeyList(CArray<UINT,UINT>& aLcomKey, int nType)
{
	aLcomKey.RemoveAll();
	switch(nType)
	{
	case D_LOADCASE_STATIC:
		return m_mapLcom_Anal.GetSortedKeyListforUintK(aLcomKey);    
	case D_LOADCASE_COMB_STEEL:
		return m_mapLcom_STL.GetSortedKeyListforUintK(aLcomKey);
	case D_LOADCASE_COMB_CONCRETE:
		return m_mapLcom_RC.GetSortedKeyListforUintK(aLcomKey);
	case 3:
		return m_mapLcom_DS.GetSortedKeyListforUintK(aLcomKey);
	case 4:
		return m_mapLcom_Hor.GetSortedKeyListforUintK(aLcomKey);
		break;
	}

	return 0;
}

BOOL CDB_JUDResultCtrl::GetLcomItem(UINT LcomK, CString& strItem, int nType)
{
	strItem = "";
	switch(nType)
	{
	case D_LOADCASE_STATIC:
		return m_mapLcom_Anal.Lookup(LcomK, strItem);    
	case D_LOADCASE_COMB_STEEL:
		return m_mapLcom_STL.Lookup(LcomK, strItem);
	case D_LOADCASE_COMB_CONCRETE:
		return m_mapLcom_RC.Lookup(LcomK, strItem);
	case 3:
		return m_mapLcom_DS.Lookup(LcomK, strItem);
	case 4:
		return m_mapLcom_Hor.Lookup(LcomK, strItem);
		break;
	}
	
	return TRUE;
}
