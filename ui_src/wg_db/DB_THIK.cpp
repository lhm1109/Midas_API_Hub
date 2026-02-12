#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_THIK.h"
#include "DBDoc.h"
#include "DispOptColorObj.h"
#include "DispOptMgr.h"

#include "..\wg_base\RandomColorUtil.h"   // for CRandomColorUtil

CDB_THIK::CDB_THIK()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_thik.InitHashTable(HASHSIZETHIK);
	m_thiklist.InitHashTable(HASHSIZELISTTHIK);
	m_thikcolor.InitHashTable(HASHSIZELISTTHIK);
}

CDB_THIK::~CDB_THIK()
{
	DelAllList();
}

void CDB_THIK::Add(T_THIK_K Key,T_THIK_D& rData)
{
	T_THIK_D data;
	if(!m_thik.Lookup(Key, data))
	{
		T_COLOR_D DataColor;
		DataColor.Initialize();

		CRandomColorUtil randomColorUtil;
		
		if (CDispOptMgr::GetFlagAssignRandomColor())
		{
			//       DataColor.WireFrame  = randomColorUtil.GetRandomColor();
			//       DataColor.HiddenFill = randomColorUtil.GetRandomColor();
			//       DataColor.HiddenEdge = randomColorUtil.GetRandomColor();
			randomColorUtil.GetRandomColor(DataColor.HiddenFill, DataColor.HiddenEdge, DataColor.WireFrame);
		}
		else
		{
			COptColorObjNames COCN;
			DataColor.WireFrame = RGB(111, 142, 91);
			DataColor.HiddenFill = RGB(159, 205, 131);
			DataColor.HiddenEdge = RGB(111, 142, 91);
		}
		m_thikcolor.SetAt(Key, DataColor);
	}
	m_thik.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_THIK::Del(T_THIK_K Key)
{
	BOOL ret=m_thik.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(m_thikcolor.RemoveKey(Key));
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_THIK_K key;
			T_THIK_D data;
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
	}
	return ret;
}

void CDB_THIK::AddList(T_THIK_K Key)
{
	ThikList* pList = new ThikList;
	m_thiklist.SetAt(Key, pList);
}

BOOL CDB_THIK::DelList(T_THIK_K Key)
{
	ThikList* pList;
	BOOL bExist=m_thiklist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_thiklist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_THIK::DelAllList()
{
	T_THIK_K Key;
	ThikList* pList;
	POSITION pos=m_thiklist.GetStartPosition();
	while(pos != NULL)
	{
		m_thiklist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_thiklist.RemoveAll();
}

void CDB_THIK::AddListItem(T_THIK_K Key, int nCmd, int nKey)
{
	ThikList* pList;
	if(!GetList(Key, pList))return;

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_THIK::DelListItem(T_THIK_K Key, int nCmd, int nKey)
{
	ThikList* pList;
	if(!GetList(Key, pList))return TRUE;
	if(pList->GetCount()<=0)return TRUE;

	T_LIST_DATA ListData;
	POSITION pos=pList->GetHeadPosition(), posPrv;
	while(pos != NULL)
	{
		posPrv=pos;
		ListData=pList->GetNext(pos);
		if(ListData.nCmd == nCmd && ListData.nKey == nKey)
		{
			pList->RemoveAt(posPrv);
			return TRUE;
		}
	}
	return FALSE;
}
