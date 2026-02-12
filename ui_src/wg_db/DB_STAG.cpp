#include "stdafx.h"
#include "DB_GRUP.h"
#include "DB_BNGR.h"
#include "DB_LDGR.h"
#include "DB_STAG.h"

CDB_STAG::CDB_STAG()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_nLastStag = 0;
	m_stag.InitHashTable(HASHSIZESTAG);
}

CDB_STAG::~CDB_STAG()
{
}

void CDB_STAG::Add(T_STAG_K Key,T_STAG_D& rData,CDB_GRUP* pGrup,CDB_BNGR* pBngr,CDB_LDGR* pLdgr)
{
	T_STAG_D Data;
	BOOL bExist=m_stag.Lookup(Key, Data);
	CMap<T_GRUP_K, T_GRUP_K, T_GRUP_K, T_GRUP_K>mapGrupOld;
	CMap<T_GRUP_K, T_GRUP_K, T_GRUP_K, T_GRUP_K>mapGrupNew;
	CMap<T_BNGR_K, T_BNGR_K, T_BNGR_K, T_BNGR_K>mapBngrOld;
	CMap<T_BNGR_K, T_BNGR_K, T_BNGR_K, T_BNGR_K>mapBngrNew;
	CMap<T_LDGR_K, T_LDGR_K, T_LDGR_K, T_LDGR_K>mapLdgrOld;
	CMap<T_LDGR_K, T_LDGR_K, T_LDGR_K, T_LDGR_K>mapLdgrNew;
	T_GRUP_K KeyGrupTemp;
	T_BNGR_K KeyBngrTemp;
	T_LDGR_K KeyLdgrTemp;

	mapGrupNew.InitHashTable(HASHSIZEGRUP);
	mapBngrNew.InitHashTable(HASHSIZEBNGR);
	mapLdgrNew.InitHashTable(HASHSIZELDGR);
	for(int i = 0; i<rData.aActiveElem.GetSize(); i++)mapGrupNew.SetAt(rData.aActiveElem[i].GrupKey, rData.aActiveElem[i].GrupKey);
	for(int i = 0; i<rData.aDeactiveElem.GetSize(); i++)mapGrupNew.SetAt(rData.aDeactiveElem[i].GrupKey, rData.aDeactiveElem[i].GrupKey);
	for(int i = 0; i<rData.aActiveBndr.GetSize(); i++)mapBngrNew.SetAt(rData.aActiveBndr[i].BngrKey, rData.aActiveBndr[i].BngrKey);
	for(int i = 0; i<rData.aDeactiveBndr.GetSize(); i++)mapBngrNew.SetAt(rData.aDeactiveBndr[i], rData.aDeactiveBndr[i]);
	for(int i = 0; i<rData.aActiveLoad.GetSize(); i++)mapLdgrNew.SetAt(rData.aActiveLoad[i].LdgrKey, rData.aActiveLoad[i].LdgrKey);
	for(int i = 0; i<rData.aDeactiveLoad.GetSize(); i++)mapLdgrNew.SetAt(rData.aDeactiveLoad[i].LdgrKey, rData.aDeactiveLoad[i].LdgrKey);

	if(bExist)
	{
		mapGrupOld.InitHashTable(HASHSIZEGRUP);
		mapBngrOld.InitHashTable(HASHSIZEBNGR);
		mapLdgrOld.InitHashTable(HASHSIZELDGR);
		for(int i = 0; i<Data.aActiveElem.GetSize(); i++)mapGrupOld.SetAt(Data.aActiveElem[i].GrupKey, Data.aActiveElem[i].GrupKey);
		for(int i = 0; i<Data.aDeactiveElem.GetSize(); i++)mapGrupOld.SetAt(Data.aDeactiveElem[i].GrupKey, Data.aDeactiveElem[i].GrupKey);
		for(int i = 0; i<Data.aActiveBndr.GetSize(); i++)mapBngrOld.SetAt(Data.aActiveBndr[i].BngrKey, Data.aActiveBndr[i].BngrKey);
		for(int i = 0; i<Data.aDeactiveBndr.GetSize(); i++)mapBngrOld.SetAt(Data.aDeactiveBndr[i], Data.aDeactiveBndr[i]);
		for(int i = 0; i<Data.aActiveLoad.GetSize(); i++)mapLdgrOld.SetAt(Data.aActiveLoad[i].LdgrKey, Data.aActiveLoad[i].LdgrKey);
		for(int i = 0; i<Data.aDeactiveLoad.GetSize(); i++)mapLdgrOld.SetAt(Data.aDeactiveLoad[i].LdgrKey, Data.aDeactiveLoad[i].LdgrKey);

		POSITION pos;
		T_GRUP_K GrupKey;
		pos = mapGrupOld.GetStartPosition();
		while(pos)
		{
			mapGrupOld.GetNextAssoc(pos, GrupKey, GrupKey);
			if(!mapGrupNew.Lookup(GrupKey, KeyGrupTemp))
				VERIFY(pGrup->DelListItem(GrupKey, LT_STAG_CMD, Key));
		}
		T_BNGR_K BngrKey;
		pos = mapBngrOld.GetStartPosition();
		while(pos)
		{
			mapBngrOld.GetNextAssoc(pos, BngrKey, BngrKey);
			if(!mapBngrNew.Lookup(BngrKey, KeyBngrTemp))
				VERIFY(pBngr->DelListItem(BngrKey, LT_STAG_CMD, Key));
		}
		T_LDGR_K LdgrKey;
		pos = mapLdgrOld.GetStartPosition();
		while(pos)
		{
			mapLdgrOld.GetNextAssoc(pos, LdgrKey, LdgrKey);
			if(!mapLdgrNew.Lookup(LdgrKey, KeyLdgrTemp))
				VERIFY(pLdgr->DelListItem(LdgrKey, LT_STAG_CMD, Key));
		}
	}
	else
	{
		T_STAG_K key;
		T_STAG_D data;
		POSITION pos = GetStart();
		while (pos)
		{
			GetNext(pos, key, data);
			if (data.nStageId >= rData.nStageId) 
			{
				data.nStageId++;
				m_stag.SetAt(key, data);
			}
		}
	}
	m_stag.SetAt(Key,rData);

	if(Key == m_nStartNum)
	{
		T_STAG_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	POSITION pos;
	T_GRUP_K GrupKey;
	pos = mapGrupNew.GetStartPosition();
	while (pos)
	{
		mapGrupNew.GetNextAssoc(pos, GrupKey, GrupKey);
		if (bExist)
		{
			if (!mapGrupOld.Lookup(GrupKey, KeyGrupTemp))
				pGrup->AddListItem(GrupKey, LT_STAG_CMD, Key);
		}
		else
			pGrup->AddListItem(GrupKey, LT_STAG_CMD, Key);
	}
	T_BNGR_K BngrKey;
	pos = mapBngrNew.GetStartPosition();
	while (pos)
	{
		mapBngrNew.GetNextAssoc(pos, BngrKey, BngrKey);
		if (bExist)
		{
			if (!mapBngrOld.Lookup(BngrKey, KeyBngrTemp))
				pBngr->AddListItem(BngrKey, LT_STAG_CMD, Key);
		}
		else
			pBngr->AddListItem(BngrKey, LT_STAG_CMD, Key);
	}
	T_LDGR_K LdgrKey;
	pos = mapLdgrNew.GetStartPosition();
	while (pos)
	{
		mapLdgrNew.GetNextAssoc(pos, LdgrKey, LdgrKey);
		if (bExist)
		{
			if (!mapLdgrOld.Lookup(LdgrKey, KeyLdgrTemp))
				pLdgr->AddListItem(LdgrKey, LT_STAG_CMD, Key);
		}
		else
			pLdgr->AddListItem(LdgrKey, LT_STAG_CMD, Key);
	}

	// Last Stage 유지
	if (rData.nStageId >= m_stag.GetCount()) m_nLastStag = Key;
}

BOOL CDB_STAG::Del(T_STAG_K Key,CDB_GRUP* pGrup,CDB_BNGR* pBngr,CDB_LDGR* pLdgr)
{
	T_STAG_D Data;
	BOOL bExist=m_stag.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_stag.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			CMap<T_GRUP_K, T_GRUP_K, T_GRUP_K, T_GRUP_K>mapGrupOld;
			CMap<T_BNGR_K, T_BNGR_K, T_BNGR_K, T_BNGR_K>mapBngrOld;
			CMap<T_LDGR_K, T_LDGR_K, T_LDGR_K, T_LDGR_K>mapLdgrOld;
			for(int i = 0; i<Data.aActiveElem.GetSize(); i++)mapGrupOld.SetAt(Data.aActiveElem[i].GrupKey, Data.aActiveElem[i].GrupKey);
			for(int i = 0; i<Data.aDeactiveElem.GetSize(); i++)mapGrupOld.SetAt(Data.aDeactiveElem[i].GrupKey, Data.aDeactiveElem[i].GrupKey);
			for(int i = 0; i<Data.aActiveBndr.GetSize(); i++)mapBngrOld.SetAt(Data.aActiveBndr[i].BngrKey, Data.aActiveBndr[i].BngrKey);
			for(int i = 0; i<Data.aDeactiveBndr.GetSize(); i++)mapBngrOld.SetAt(Data.aDeactiveBndr[i], Data.aDeactiveBndr[i]);
			for(int i = 0; i<Data.aActiveLoad.GetSize(); i++)mapLdgrOld.SetAt(Data.aActiveLoad[i].LdgrKey, Data.aActiveLoad[i].LdgrKey);
			for(int i = 0; i<Data.aDeactiveLoad.GetSize(); i++)mapLdgrOld.SetAt(Data.aDeactiveLoad[i].LdgrKey, Data.aDeactiveLoad[i].LdgrKey);

			POSITION pos;
			T_GRUP_K GrupKey;
			pos = mapGrupOld.GetStartPosition();
			while(pos)
			{
				mapGrupOld.GetNextAssoc(pos, GrupKey, GrupKey);
				VERIFY(pGrup->DelListItem(GrupKey, LT_STAG_CMD, Key));
			}
			T_BNGR_K BngrKey;
			pos = mapBngrOld.GetStartPosition();
			while(pos)
			{
				mapBngrOld.GetNextAssoc(pos, BngrKey, BngrKey);
				VERIFY(pBngr->DelListItem(BngrKey, LT_STAG_CMD, Key));
			}
			T_LDGR_K LdgrKey;
			pos = mapLdgrOld.GetStartPosition();
			while(pos)
			{
				mapLdgrOld.GetNextAssoc(pos, LdgrKey, LdgrKey);
				VERIFY(pLdgr->DelListItem(LdgrKey, LT_STAG_CMD, Key));
			}

			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_STAG_K key;
				T_STAG_D data;
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

			// Last Stage 유지(1)
			BOOL bLastStagDeleted = (Key == m_nLastStag);
			int nStagNum = m_stag.GetCount();
			T_STAG_K LastStag;

			// Stag ID 유지
			T_STAG_K key;
			T_STAG_D data;
			pos = GetStart();
			while (pos)
			{
				GetNext(pos, key, data);
				if (data.nStageId > Data.nStageId)
				{
					data.nStageId--;
					m_stag.SetAt(key, data);
				}
				// Last Stage 유지(2)
				if (bLastStagDeleted && data.nStageId == nStagNum) LastStag = key;
			}
			// Last Stage 유지(3)
			if (bLastStagDeleted)
			{
				if (m_stag.GetCount() == 0) m_nLastStag = 0;
				else m_nLastStag = LastStag;
			}
		}
		return ret;
	}
	return bExist;
}

void CDB_STAG::AddItem(T_STAG_ITEM_UDRD_D& rItem,CDB_GRUP* pGrup,CDB_BNGR* pBngr,CDB_LDGR* pLdgr)
{
	T_STAG_K Key = rItem.key;
	T_STAG_D Data;
	if(!m_stag.Lookup(Key, Data))
	{
		ASSERT(FALSE);
		return;
	}

	CMap<T_GRUP_K, T_GRUP_K, T_GRUP_K, T_GRUP_K>mapGrupOld;
	CMap<T_BNGR_K, T_BNGR_K, T_BNGR_K, T_BNGR_K>mapBngrOld;
	CMap<T_LDGR_K, T_LDGR_K, T_LDGR_K, T_LDGR_K>mapLdgrOld;
	for(int i = 0; i<Data.aActiveElem.GetSize(); i++)mapGrupOld.SetAt(Data.aActiveElem[i].GrupKey, Data.aActiveElem[i].GrupKey);
	for(int i = 0; i<Data.aDeactiveElem.GetSize(); i++)mapGrupOld.SetAt(Data.aDeactiveElem[i].GrupKey, Data.aDeactiveElem[i].GrupKey);
	for(int i = 0; i<Data.aActiveBndr.GetSize(); i++)mapBngrOld.SetAt(Data.aActiveBndr[i].BngrKey, Data.aActiveBndr[i].BngrKey);
	for(int i = 0; i<Data.aDeactiveBndr.GetSize(); i++)mapBngrOld.SetAt(Data.aDeactiveBndr[i], Data.aDeactiveBndr[i]);
	for(int i = 0; i<Data.aActiveLoad.GetSize(); i++)mapLdgrOld.SetAt(Data.aActiveLoad[i].LdgrKey, Data.aActiveLoad[i].LdgrKey);
	for(int i = 0; i<Data.aDeactiveLoad.GetSize(); i++)mapLdgrOld.SetAt(Data.aDeactiveLoad[i].LdgrKey, Data.aDeactiveLoad[i].LdgrKey);

// 기존 데이터에 없는 Group Key만 들어와야 됨
	if(rItem.ActiveElem.GrupKey != 0)
	{
		Data.aActiveElem.Add(rItem.ActiveElem);
		m_stag.SetAt(Key,Data);
		if (!mapGrupOld.Lookup(rItem.ActiveElem.GrupKey, rItem.ActiveElem.GrupKey))
		{
			pGrup->AddListItem(rItem.ActiveElem.GrupKey, LT_STAG_CMD, Key);
			mapGrupOld.SetAt(rItem.ActiveElem.GrupKey, rItem.ActiveElem.GrupKey);
		}
	}
	if(rItem.DeactiveElem.GrupKey != 0)
	{
		Data.aDeactiveElem.Add(rItem.DeactiveElem);
		m_stag.SetAt(Key,Data);
		if (!mapGrupOld.Lookup(rItem.DeactiveElem.GrupKey, rItem.DeactiveElem.GrupKey))
		{
			pGrup->AddListItem(rItem.DeactiveElem.GrupKey, LT_STAG_CMD, Key);
			mapGrupOld.SetAt(rItem.DeactiveElem.GrupKey, rItem.DeactiveElem.GrupKey);
		}
	}
	if(rItem.ActiveBndr.BngrKey != 0)
	{
		Data.aActiveBndr.Add(rItem.ActiveBndr);
		m_stag.SetAt(Key,Data);
		if (!mapBngrOld.Lookup(rItem.ActiveBndr.BngrKey, rItem.ActiveBndr.BngrKey))
		{
			pBngr->AddListItem(rItem.ActiveBndr.BngrKey, LT_STAG_CMD, Key);
			mapBngrOld.SetAt(rItem.ActiveBndr.BngrKey, rItem.ActiveBndr.BngrKey);
		}
	}
	if(rItem.DeactiveBndr != 0)
	{
		Data.aDeactiveBndr.Add(rItem.DeactiveBndr);
		m_stag.SetAt(Key,Data);
		if (!mapBngrOld.Lookup(rItem.DeactiveBndr, rItem.DeactiveBndr))
		{
			pBngr->AddListItem(rItem.DeactiveBndr, LT_STAG_CMD, Key);
			mapBngrOld.SetAt(rItem.DeactiveBndr, rItem.DeactiveBndr);
		}
	}
	if(rItem.ActiveLoad.LdgrKey != 0)
	{
		Data.aActiveLoad.Add(rItem.ActiveLoad);
		m_stag.SetAt(Key,Data);
		if (!mapLdgrOld.Lookup(rItem.ActiveLoad.LdgrKey, rItem.ActiveLoad.LdgrKey))
		{
			pLdgr->AddListItem(rItem.ActiveLoad.LdgrKey, LT_STAG_CMD, Key);
			mapLdgrOld.SetAt(rItem.ActiveLoad.LdgrKey, rItem.ActiveLoad.LdgrKey);
		}
	}
	if(rItem.DeactiveLoad.LdgrKey != 0)
	{
		Data.aDeactiveLoad.Add(rItem.DeactiveLoad);
		m_stag.SetAt(Key,Data);
		if (!mapLdgrOld.Lookup(rItem.DeactiveLoad.LdgrKey, rItem.DeactiveLoad.LdgrKey))
		{
			pLdgr->AddListItem(rItem.DeactiveLoad.LdgrKey, LT_STAG_CMD, Key);
			mapLdgrOld.SetAt(rItem.DeactiveLoad.LdgrKey, rItem.DeactiveLoad.LdgrKey);
		}
	}
}

BOOL CDB_STAG::DelItem(T_STAG_ITEM_UDRD_D& rItem,CDB_GRUP* pGrup,CDB_BNGR* pBngr,CDB_LDGR* pLdgr)
{
	T_STAG_K Key = rItem.key;
	T_STAG_D Data;
	if(!m_stag.Lookup(Key, Data))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if(rItem.ActiveElem.GrupKey != 0)
	{
		int nGroup=Data.aActiveElem.GetSize();
		for(int i=0; i<nGroup; i++)
		{
			if(Data.aActiveElem[i].GrupKey == rItem.ActiveElem.GrupKey)
			{
				Data.aActiveElem.RemoveAt(i);
				m_stag.SetAt(Key,Data);
				VERIFY(pGrup->DelListItem(rItem.ActiveElem.GrupKey, LT_STAG_CMD, Key));
			}
		}
	}
	if(rItem.DeactiveElem.GrupKey != 0)
	{
		int nGroup=Data.aDeactiveElem.GetSize();
		for(int i=0; i<nGroup; i++)
		{
			if(Data.aDeactiveElem[i].GrupKey == rItem.DeactiveElem.GrupKey)
			{
				Data.aDeactiveElem.RemoveAt(i);
				m_stag.SetAt(Key,Data);
				VERIFY(pGrup->DelListItem(rItem.DeactiveElem.GrupKey, LT_STAG_CMD, Key));
			}
		}
	}
	if(rItem.ActiveBndr.BngrKey != 0)
	{
		int nGroup=Data.aActiveBndr.GetSize();
		for(int i=0; i<nGroup; i++)
		{
			if(Data.aActiveBndr[i].BngrKey == rItem.ActiveBndr.BngrKey)
			{
				Data.aActiveBndr.RemoveAt(i);
				m_stag.SetAt(Key,Data);
				VERIFY(pBngr->DelListItem(rItem.ActiveBndr.BngrKey, LT_STAG_CMD, Key));
			}
		}
	}
	if(rItem.DeactiveBndr != 0)
	{
		int nGroup=Data.aDeactiveBndr.GetSize();
		for(int i=0; i<nGroup; i++)
		{
			if(Data.aDeactiveBndr[i] == rItem.DeactiveBndr)
			{
				Data.aDeactiveBndr.RemoveAt(i);
				m_stag.SetAt(Key,Data);
				VERIFY(pBngr->DelListItem(rItem.DeactiveBndr, LT_STAG_CMD, Key));
			}
		}
	}
	if(rItem.ActiveLoad.LdgrKey != 0)
	{
		int nGroup=Data.aActiveLoad.GetSize();
		for(int i=0; i<nGroup; i++)
		{
			if(Data.aActiveLoad[i].LdgrKey == rItem.ActiveLoad.LdgrKey)
			{
				Data.aActiveLoad.RemoveAt(i);
				m_stag.SetAt(Key,Data);
				VERIFY(pLdgr->DelListItem(rItem.ActiveLoad.LdgrKey, LT_STAG_CMD, Key));
			}
		}
	}
	if(rItem.DeactiveLoad.LdgrKey != 0)
	{
		int nGroup=Data.aDeactiveLoad.GetSize();
		for(int i=0; i<nGroup; i++)
		{
			if(Data.aDeactiveLoad[i].LdgrKey == rItem.DeactiveLoad.LdgrKey)
			{
				Data.aDeactiveLoad.RemoveAt(i);
				m_stag.SetAt(Key,Data);
				VERIFY(pLdgr->DelListItem(rItem.DeactiveLoad.LdgrKey, LT_STAG_CMD, Key));
			}
		}
	}
	return TRUE;
}

