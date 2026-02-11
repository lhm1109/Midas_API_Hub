#include "stdafx.h"
#include "DB_SIGR_LOWER.h"
#include "DB_NODE.h"
#include "DB_ELEM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SIGR_LOWER::CDB_SIGR_LOWER()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sigrLower.InitHashTable(HASHSIZESIGR_LOWER);
	m_nStartNum = 1;
	m_nLastNum = 0;
}

CDB_SIGR_LOWER::~CDB_SIGR_LOWER()
{

}

void CDB_SIGR_LOWER::Add(T_SIGR_LOWER_K Key, T_SIGR_LOWER_D& rData, CDB_NODE* pNode, CDB_ELEM* pElem)
{
	T_SIGR_LOWER_D Data;
	BOOL bExist = m_sigrLower.Lookup(Key, Data);

	T_NODE_D dNode;
	T_ELEM_D dElem;
	int nCount = 0;

	if (bExist)
	{
		nCount = Data.BEAR_D.GetCount();
		for (int i = 0; i < nCount; i++) {
			T_SIGR_LOWER_BEAR_D& bear = Data.BEAR_D[i];
			for (int j = 0; j < bear.aPosition.GetSize(); j++) {
				if (pNode) {
					if (pNode->Get(bear.aPosition[j], dNode))
						pNode->DelListItem(bear.aPosition[j], LT_SIGR_LOWER_CMD, Key);
				}
			}
		}

		nCount = Data.COLM_D.GetCount();
		for (int i = 0; i < nCount; i++) {
			T_SIGR_LOWER_COLM_D& colm = Data.COLM_D[i];
			for (int j = 0; j < colm.aInvestigatedElem.GetSize(); j++) {
				if (pElem) {
					if (pElem->Get(colm.aInvestigatedElem[j], dElem))
						pElem->DelListItem(colm.aInvestigatedElem[j], LT_SIGR_LOWER_CMD, Key);
				}
			}
		}

		nCount = Data.BEAM_D.GetCount();
		for (int i = 0; i < nCount; i++) {
			T_SIGR_LOWER_BEAM_D& beam = Data.BEAM_D[i];
			for (int j = 0; j < beam.aInvestigatedElem.GetSize(); j++) {
				if (pElem) {
					if (pElem->Get(beam.aInvestigatedElem[j], dElem))
						pElem->DelListItem(beam.aInvestigatedElem[j], LT_SIGR_LOWER_CMD, Key);
				}
			}
		}

		nCount = Data.FOOT_D.GetCount();
		for (int i = 0; i < nCount; i++) {
			T_SIGR_LOWER_FOOT_D& foot = Data.FOOT_D[i];
			for (int j = 0; j < foot.aInvestigatedElem.GetSize(); j++) {
				if (pElem) {
					if (pElem->Get(foot.aInvestigatedElem[j], dElem))
						pElem->DelListItem(foot.aInvestigatedElem[j], LT_SIGR_LOWER_CMD, Key);
				}
			}
		}

		if (pNode) {
			if (pNode->Get(Data.kNodeAxis, dNode)) 
				pNode->DelListItem(Data.kNodeAxis, LT_SIGR_LOWER_CMD, Key);
			if (pNode->Get(Data.kNodePerp, dNode)) 
				pNode->DelListItem(Data.kNodePerp, LT_SIGR_LOWER_CMD, Key);
		}
	}

	m_sigrLower.SetAt(Key, rData);

	nCount = rData.BEAR_D.GetCount();
	for (int i = 0; i < nCount; i++) {
		T_SIGR_LOWER_BEAR_D& bear = rData.BEAR_D[i];
		for (int j = 0; j < bear.aPosition.GetSize(); j++) {
			if (pNode) {
				pNode->AddListItem(bear.aPosition[j], LT_SIGR_LOWER_CMD, Key);
			}
		}
	}

	nCount = rData.COLM_D.GetCount();
	for (int i = 0; i < nCount; i++) {
		T_SIGR_LOWER_COLM_D& colm = rData.COLM_D[i];
		for (int j = 0; j < colm.aInvestigatedElem.GetSize(); j++) {
			if (pElem) {
				pElem->AddListItem(colm.aInvestigatedElem[j], LT_SIGR_LOWER_CMD, Key);
			}
		}
	}

	nCount = rData.BEAM_D.GetCount();
	for (int i = 0; i < nCount; i++) {
		T_SIGR_LOWER_BEAM_D& beam = rData.BEAM_D[i];
		for (int j = 0; j < beam.aInvestigatedElem.GetSize(); j++) {
			if (pElem) {
				pElem->AddListItem(beam.aInvestigatedElem[j], LT_SIGR_LOWER_CMD, Key);
			}
		}
	}

	nCount = rData.FOOT_D.GetCount();
	for (int i = 0; i < nCount; i++) {
		T_SIGR_LOWER_FOOT_D& foot = rData.FOOT_D[i];
		for (int j = 0; j < foot.aInvestigatedElem.GetSize(); j++) {
			if (pElem) {
				pElem->AddListItem(foot.aInvestigatedElem[j], LT_SIGR_LOWER_CMD, Key);
			}
		}
	}

	if (pNode) {
		if (pNode->Get(rData.kNodeAxis, dNode)) pNode->AddListItem(rData.kNodeAxis, LT_SIGR_LOWER_CMD, Key);
		if (pNode->Get(rData.kNodePerp, dNode)) pNode->AddListItem(rData.kNodePerp, LT_SIGR_LOWER_CMD, Key);
	}

	if (Key == m_nStartNum)
	{
		T_SIGR_LOWER_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!m_sigrLower.Lookup(m_nStartNum, data))break;
		}
	}
	if (Key > m_nLastNum)m_nLastNum = Key;
}

BOOL CDB_SIGR_LOWER::Del(T_SIGR_LOWER_K Key, CDB_NODE* pNode, CDB_ELEM* pElem)
{
	T_SIGR_LOWER_D Data;
	BOOL bExist = m_sigrLower.Lookup(Key, Data);
	if (!bExist) return FALSE;

	BOOL ret = m_sigrLower.RemoveKey(Key);
	if (ret)
	{
		T_NODE_D dNode;
		T_ELEM_D dElem;
		int nCount = 0;

		nCount = Data.BEAR_D.GetCount();
		for (int i = 0; i < nCount; i++) {
			T_SIGR_LOWER_BEAR_D& bear = Data.BEAR_D[i];
			for (int j = 0; j < bear.aPosition.GetSize(); j++) {
				if (pNode) {
					if (pNode->Get(bear.aPosition[j], dNode))
						VERIFY(pNode->DelListItem(bear.aPosition[j], LT_SIGR_LOWER_CMD, Key));
				}
			}
		}

		nCount = Data.COLM_D.GetCount();
		for (int i = 0; i < nCount; i++) {
			T_SIGR_LOWER_COLM_D& colm = Data.COLM_D[i];
			for (int j = 0; j < colm.aInvestigatedElem.GetSize(); j++) {
				if (pElem) {
					if (pElem->Get(colm.aInvestigatedElem[j], dElem))
						VERIFY(pElem->DelListItem(colm.aInvestigatedElem[j], LT_SIGR_LOWER_CMD, Key));
				}
			}
		}

		nCount = Data.BEAM_D.GetCount();
		for (int i = 0; i < nCount; i++) {
			T_SIGR_LOWER_BEAM_D& beam = Data.BEAM_D[i];
			for (int j = 0; j < beam.aInvestigatedElem.GetSize(); j++) {
				if (pElem) {
					if (pElem->Get(beam.aInvestigatedElem[j], dElem))
						VERIFY(pElem->DelListItem(beam.aInvestigatedElem[j], LT_SIGR_LOWER_CMD, Key));
				}
			}
		}

		nCount = Data.FOOT_D.GetCount();
		for (int i = 0; i < nCount; i++) {
			T_SIGR_LOWER_FOOT_D& foot = Data.FOOT_D[i];
			for (int j = 0; j < foot.aInvestigatedElem.GetSize(); j++) {
				if (pElem) {
					if (pElem->Get(foot.aInvestigatedElem[j], dElem))
						VERIFY(pElem->DelListItem(foot.aInvestigatedElem[j], LT_SIGR_LOWER_CMD, Key));
				}
			}
		}

		if (pNode) {
			if (pNode->Get(Data.kNodeAxis, dNode)) VERIFY(pNode->DelListItem(Data.kNodeAxis, LT_SIGR_LOWER_CMD, Key));
			if (pNode->Get(Data.kNodePerp, dNode)) VERIFY(pNode->DelListItem(Data.kNodePerp, LT_SIGR_LOWER_CMD, Key));
		}
	}

	if (Key < m_nStartNum)m_nStartNum = Key;
	if (Key == m_nLastNum)
	{
		T_SIGR_LOWER_K key;
		T_SIGR_LOWER_D data;
		if (m_sigrLower.Lookup(m_nLastNum - 1, data))
		{
			m_nLastNum--;
		}
		else
		{
			m_nLastNum = 0;
			POSITION pos = m_sigrLower.GetStartPosition();
			while (pos != NULL)
			{
				m_sigrLower.GetNextAssoc(pos, key, data);
				if (key > m_nLastNum)m_nLastNum = key;
			}
		}
	}

	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_SIGR_LOWER::Get(T_SIGR_LOWER_K Key, T_SIGR_LOWER_D& rData)
{
	return m_sigrLower.Lookup(Key, rData);
}

int CDB_SIGR_LOWER::GetCount()
{
	return m_sigrLower.GetCount();
}

POSITION CDB_SIGR_LOWER::GetStart()
{
	return m_sigrLower.GetStartPosition();
}

void CDB_SIGR_LOWER::GetNext(POSITION& rNextPosition, T_SIGR_LOWER_K& rKey, T_SIGR_LOWER_D& rData)
{
	m_sigrLower.GetNextAssoc(rNextPosition, rKey, rData);
}