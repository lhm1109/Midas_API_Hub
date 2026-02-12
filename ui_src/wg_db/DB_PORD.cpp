#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_ELEM.h"
#include "DB_PORD.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PORD::CDB_PORD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_pord.InitHashTable(HASHSIZEPORD);
}

CDB_PORD::~CDB_PORD()
{
}

void CDB_PORD::Add(T_PORD_K Key,T_PORD_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	T_PORD_D Data;
	BOOL bExist=m_pord.Lookup(Key, Data);
	if(bExist)
	{
		if(Data.nGraphStep == 0)
		{
			if(Data.nFunctionType == 1 || Data.nFunctionType == 2)
			{
				if(pNode != NULL)VERIFY(pNode->DelListItem(Data.nEntity, LT_PORD_CMD, Key));
			}
			else if(Data.nFunctionType >= 3 && Data.nFunctionType <= 7)
			{
				if(pElem != NULL)VERIFY(pElem->DelListItem(Data.nEntity, LT_PORD_CMD, Key));
			}
			else if(Data.nFunctionType == 8)  // General Link
			{
			}
			else if(Data.nFunctionType == 9)  // Inelastic Hinge
			{
				if(Data.nElemType4Iehg == 0 || Data.nElemType4Iehg == 2 ||
					 Data.nElemType4Iehg == 3) // beam or truss or wall
				{
					if(pElem != NULL)VERIFY(pElem->DelListItem(Data.nEntity, LT_PORD_CMD, Key));
				}
			}
			else if(Data.nFunctionType == 10)  // PlaneStrain
			{
				if(pElem != NULL)VERIFY(pElem->DelListItem(Data.nEntity, LT_PORD_CMD, Key));
			}
			else if(Data.nFunctionType == 11)  // Load Increment History
			{
			}
			else
				ASSERT(FALSE);
		}
		else if(Data.nGraphStep == 1)
		{
			if(Data.nStepFunction == 1)  // Time Step
			{        
			}
			else if(Data.nStepFunction == 2)  // Displ/Vel/Accel
			{
				if(pNode != NULL)VERIFY(pNode->DelListItem(Data.nEntity, LT_PORD_CMD, Key));
			}
			else if(Data.nStepFunction == 3 || Data.nStepFunction == 4) // 3=Truss Force/Stress, 4=Beam Force/Stress
			{
				if(pElem != NULL)VERIFY(pElem->DelListItem(Data.nEntity, LT_PORD_CMD, Key));
			}
			else if(Data.nStepFunction == 5)  // Designated DOF
			{
			}
			else if(Data.nStepFunction == 6)  // General Link Force 
			{
			}
			else
				ASSERT(FALSE);
		}
	}
	m_pord.SetAt(Key,rData);

	if(rData.nGraphStep == 0)
	{
		if(rData.nFunctionType == 1 || rData.nFunctionType == 2)
		{
			if(pNode != NULL)pNode->AddListItem(rData.nEntity, LT_PORD_CMD, Key);
		}
		else if(rData.nFunctionType >= 3 && rData.nFunctionType <= 7)
		{
			if(pElem != NULL)pElem->AddListItem(rData.nEntity, LT_PORD_CMD, Key);
		}    
		else if(rData.nFunctionType == 9)
		{
			if(rData.nElemType4Iehg == 0 || rData.nElemType4Iehg == 2 ||
				 rData.nElemType4Iehg == 3 ) // beam or truss or wall
			{
				if(pElem != NULL)pElem->AddListItem(rData.nEntity, LT_PORD_CMD, Key);
			}
		}
		else if(rData.nFunctionType == 10)
		{
			if(pElem != NULL)pElem->AddListItem(rData.nEntity, LT_PORD_CMD, Key);
		}    
	}
	else if(rData.nGraphStep == 1)
	{
		if(rData.nStepFunction == 2)
		{
			if(pNode != NULL)pNode->AddListItem(rData.nEntity, LT_PORD_CMD, Key);
		}
		else if(rData.nStepFunction == 3 || rData.nStepFunction == 4)
		{
			if(pElem != NULL)pElem->AddListItem(rData.nEntity, LT_PORD_CMD, Key);
		}    
	}

	if(Key == m_nStartNum)
	{
		T_PORD_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	// Stage Mode에서도 Add/Del/Mod 될 수 있으므로
	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_PORD_CMD, Key);
	}
}

BOOL CDB_PORD::Del(T_PORD_K Key,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	T_PORD_D Data;
	m_pord.Lookup(Key, Data);
	BOOL ret=m_pord.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Data.nGraphStep == 0)
		{
			if(Data.nFunctionType == 1 || Data.nFunctionType == 2)
			{
				if(pNode != NULL)VERIFY(pNode->DelListItem(Data.nEntity, LT_PORD_CMD, Key));
			}
			else if(Data.nFunctionType >= 3 && Data.nFunctionType <= 7)
			{
				if(pElem != NULL)VERIFY(pElem->DelListItem(Data.nEntity, LT_PORD_CMD, Key));
			}
			else if(Data.nFunctionType == 9)
			{
				if(Data.nElemType4Iehg == 0 || Data.nElemType4Iehg == 2 ||
					 Data.nElemType4Iehg == 3) // beam or truss or wall
				{
					if(pElem != NULL)VERIFY(pElem->DelListItem(Data.nEntity, LT_PORD_CMD, Key));
				}
			}
			else if(Data.nFunctionType == 10)
			{
				if(pElem != NULL)VERIFY(pElem->DelListItem(Data.nEntity, LT_PORD_CMD, Key));
			}
		}
		else if(Data.nGraphStep == 1)
		{
			if(Data.nStepFunction == 2)
			{
				if(pNode != NULL)VERIFY(pNode->DelListItem(Data.nEntity, LT_PORD_CMD, Key));
			}
			else if(Data.nStepFunction == 3 || Data.nStepFunction == 4)
			{
				if(pElem != NULL)VERIFY(pElem->DelListItem(Data.nEntity, LT_PORD_CMD, Key));
			}      
		}
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_PORD_K key;
			T_PORD_D data;
			if(Get(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=GetStart();
				while(pos != NULL)
				{
					GetNext(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
		// Stage Mode에서도 Add/Del/Mod 될 수 있으므로
		// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			m_pDoc->m_pStagCtrl->DeactiveEtc(LT_PORD_CMD, Key);
		}
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_PORD::Get(T_PORD_K Key,T_PORD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_pord->Lookup(Key, uTemp)) return FALSE;
	}
	return m_pord.Lookup(Key,rData);
}

int CDB_PORD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_pord->GetCount();
	}
	return m_pord.GetCount();
}

POSITION CDB_PORD::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_pord->GetStartPosition();
	}
	return m_pord.GetStartPosition();
}

void CDB_PORD::GetNext(POSITION& rNextPosition,T_PORD_K& rKey,T_PORD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_pord->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_pord.Lookup(rKey, rData);
		return;
	}
	m_pord.GetNextAssoc(rNextPosition,rKey,rData);
}
