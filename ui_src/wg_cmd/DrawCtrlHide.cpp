#include "Stdafx.h"
#include "wg_cmd.h"

// #include "../NS_lib/IProductInfo.h"
// #include "../NS_lib/IMSGInfo.h"
#include "../wg_db/MathFunc.h"

#include <FLOAT.H>
#include "DrawCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////

int CDrawCtrl::GetHideKeyList(nrUINT& arKey)
{
	arKey.RemoveAll();

	POSITION pos;
	UINT key;	
	pos = m_arHideDrawUnitKey.GetStartPosition();	
	UINT dummy;
	while(pos != NULL)
	{
		m_arHideDrawUnitKey.GetNextAssoc(pos, key, dummy);
		arKey.Add(key);    
	}
	int nSize = arKey.GetSize();

	int i, j;
	for(i = 0; i < nSize - 1; i++)
	{
		for(j = i + 1; j < nSize; j++)
		{
			if(arKey[i] > arKey[j])
			{
				int nTemp = arKey[i];
				arKey[i] = arKey[j];
				arKey[j] = nTemp;				
			}
		}
	}
	return arKey.GetSize();
}

int CDrawCtrl::GetHideLinkKeyList(nrUINT& arLinkKey)
{
	arLinkKey.RemoveAll();
	nrUINT arHideKey;
	CMap<UINT, UINT, UINT, UINT> mapLinkKey;
	int nSize = GetHideKeyList(arHideKey);
	if(nSize == 0) return 0;

	int i, j;
	CDrawBase* pDrawClass = NULL;
	for(i = 0; i < nSize ; i++)
	{
		pDrawClass = GetDrawClass(arHideKey[i]);
		mapLinkKey.SetAt(pDrawClass->m_LinkKey, pDrawClass->m_LinkKey);
	}

	POSITION pos;
	UINT key;	
	pos = mapLinkKey.GetStartPosition();	
	UINT dummy;
	while(pos != NULL)
	{
		mapLinkKey.GetNextAssoc(pos, key, dummy);
		arLinkKey.Add(key);    
	}
	nSize =arLinkKey.GetSize();
	for(i = 0; i < nSize - 1; i++)
	{
		for(j = i + 1; j < nSize; j++)
		{
			if(arLinkKey[i] > arLinkKey[j])
			{
				UINT nTemp = arLinkKey[i];
				arLinkKey[i] = arLinkKey[j];
				arLinkKey[j] = nTemp;				
			}
		}
	}

	return (int)arLinkKey.GetSize();
}

void CDrawCtrl::SetHideKeyList(nrUINT& arKey)
{
	RemoveAllHideKey();
	for(auto i = 0; i < arKey.GetSize(); i++) 
	{
		m_arHideDrawUnitKey.SetAt(arKey[i], arKey[i]);
	}
}

void CDrawCtrl::SetHideLinkKeyList(nrUINT& arLinkKey)
{	
	RemoveAllHideKey();

	CDrawBase* pDrawClass = NULL;

	for(UINT iIndex = m_FinalEntityNum ; iIndex>0 ; iIndex--)
	{
		pDrawClass = GetDrawClass(iIndex);
		if(pDrawClass != NULL)
		{
			if(!pDrawClass->m_bIsSelect) continue;

			if(pDrawClass->m_LinkKey > 0)
			{
				for(auto i = 0; i < arLinkKey.GetSize(); i++) 
				{
					if(pDrawClass->m_LinkKey == arLinkKey[i])
					{
						m_arHideDrawUnitKey.SetAt(iIndex, iIndex);
						break;
					}
				}
			}
		}
	}
}

void CDrawCtrl::RemoveAllHideKey()
{
	m_arHideDrawUnitKey.RemoveAll();
}

void CDrawCtrl::AddHideKey(UINT key)
{
	m_arHideDrawUnitKey.SetAt(key, key);
}

void CDrawCtrl::AddHideKey(nrUINT& arKey)
{
	for(auto i = 0; i < arKey.GetSize(); i++) 
	{
		AddHideKey(arKey[i]);
	}
}

void CDrawCtrl::DelHideKey(UINT key)
{
	m_arHideDrawUnitKey.RemoveKey(key);
}

void CDrawCtrl::DelHideKey(nrUINT& arKey)
{
	for(auto i = 0; i < arKey.GetSize(); i++) 
	{
		DelHideKey(arKey[i]);
	}
}

BOOL CDrawCtrl::IsHideKey(UINT key)
{
	UINT nTemp;
	return (m_arHideDrawUnitKey.Lookup(key, nTemp));
}

//////////////////////////////////////////////////////////////////////////
