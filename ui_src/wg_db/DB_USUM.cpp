#include "stdafx.h"
#include "DB_USUM.h"

#include "DBDoc.h"

CDB_USUM::CDB_USUM()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_usum.InitHashTable(HASHSIZEUSUM);
}

CDB_USUM::~CDB_USUM()
{
}

void CDB_USUM::Add(T_USUM_K Key,T_USUM_D& rData)
{
//   T_USUM_D Data;
//   BOOL bExist = m_usum.Lookup(Key, Data);
//   if (rData.nType == 0 || rData.nType == 1) // 새것이 Node
//   {
//     CArray<UINT, UINT> aKey;
//     if (Data.nType == 0 || Data.nType == 1) aKey.Copy(Data.arNodeElemKey);  // 이전 것이 Node
//     else
//     {
//       if (pElem) pElem->DelListItem(Data.arNodeElemKey, LT_USUM_CMD, Key);
//     }
//     if (pNode) pNode->AddListItem(rData.arNodeElemKey, aKey, LT_USUM_CMD, Key, bExist);
//   }
//   else  // 새것이 Elem
//   {
//     CArray<UINT, UINT> aKey;
//     if (!(Data.nType == 0 || Data.nType == 1)) aKey.Copy(Data.arNodeElemKey); // 이전 것이 Elem
//     else
//     {
//       if (pNode) pNode->DelListItem(Data.arNodeElemKey, LT_USUM_CMD, Key);
//     }
//     if (pElem) pElem->AddListItem(rData.arNodeElemKey, aKey, LT_USUM_CMD, Key, bExist);
//   }
	m_usum.SetAt(Key, rData);
	
	if(Key == m_nStartNum)
	{
		T_USUM_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
//   // Final Stage에서 Add/Del/Mod 될 수 있으므로
//   // Current Stage가 Final Stage이면 Stage DB를 Update한다.
//   if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
//   {
//     if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_USUM_CMD, Key);
//   }
}

BOOL CDB_USUM::Del(T_USUM_K Key)
{
//   T_USUM_D Data;
//   m_usum.Lookup(Key, Data);
	BOOL ret=m_usum.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
//     if (Data.nType == 0 || Data.nType == 1)
//     {
//       if (pNode)
//       {
//         for (int i = 0; i < Data.arNodeElemKey.GetSize(); i++)
//           VERIFY(pNode->DelListItem(Data.arNodeElemKey[i], LT_USUM_CMD, Key));
//       }
//     }
//     else
//     {
//       if (pElem)
//       {
//         for (int i = 0; i < Data.arNodeElemKey.GetSize(); i++)
//           VERIFY(pElem->DelListItem(Data.arNodeElemKey[i], LT_USUM_CMD, Key));
//       }
//     }
		
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_USUM_K key;
			T_USUM_D data;
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
//     // Final Stage에서 Add/Del/Mod 될 수 있으므로
//     // Current Stage가 Final Stage이면 Stage DB를 Update한다.
//     if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
//     {
//       m_pDoc->m_pStagCtrl->DeactiveEtc(LT_ULFC_CMD, Key);
//     }
	}
	return ret;
}