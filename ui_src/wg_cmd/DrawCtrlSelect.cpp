#include "Stdafx.h"
#include "wg_cmd.h"

// #include "../NS_lib/IProductInfo.h"
// #include "../NS_lib/IMSGInfo.h"
#include "../wg_db/MathFunc.h"

#include <set>

#include <FLOAT.H>
#include "DrawCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CDrawCtrl::GetIncludeKeyList(POINT Bound1, POINT Bound2, nrUINT& arKey, BOOL bIsSelect)
{
	arKey.RemoveAll();
	if(Bound1.x == 0 && Bound1.y == 0 && Bound2.x == 0 && Bound2.y == 0) return 0;

	CDrawBase* pDrawClass = NULL;
	int Boundary[4] = { min(Bound1.x, Bound2.x), min(Bound1.y, Bound2.y), max(Bound1.x, Bound2.x), max(Bound1.y, Bound2.y) };

	for(UINT iIndex = 1; iIndex <= m_FinalEntityNum; iIndex++)
	{
		pDrawClass = GetDrawClass(iIndex);

		if(pDrawClass != NULL)
		{
			if(bIsSelect && !pDrawClass->m_bIsSelect) continue;
			if(pDrawClass->GetClassType() == EN_DRCLASS_TYPE_LINE)
			{
				if(pDrawClass->GetBoundaryInclude(Boundary, FALSE) == 0) 	arKey.Add(iIndex); //선이면 포함으로 선택
			}
			else
			{
				if(pDrawClass->GetBoundaryInclude(Boundary, FALSE) == 1) 	arKey.Add(iIndex);	//그외에는 완전히...
			}
		}
	}
	return arKey.GetSize();
}

int CDrawCtrl::GetSelectedKeyList(nrUINT& arKey, BOOL bMoveOnly)
{
	arKey.RemoveAll();

	POSITION pos;
	UINT key;	
	pos = m_arSelectedDrawUnitKey.GetStartPosition();	
	UINT dummy;
	CDrawBase* pDrawClass = NULL;
	while(pos != NULL)
	{
		m_arSelectedDrawUnitKey.GetNextAssoc(pos, key, dummy);
		if(bMoveOnly)
		{
			pDrawClass = GetDrawClass(key);
			if(pDrawClass != NULL)		
			{ 
				if(pDrawClass->m_nSelectSubType != 1) continue; 
			}
		}
		arKey.Add(key);    
	}

	int nSize = arKey.GetSize();

	for(int i = 0; i < nSize - 1; i++)
	{
		for(int j = i + 1; j < nSize ; j++)
		{
			if(arKey[i] > arKey[j])
			{
				int nTemp = arKey[i];
				arKey[i] = arKey[j];
				arKey[j] = nTemp;				
			}
		}
	}
	return (int)arKey.GetSize();
}

int CDrawCtrl::GetSelectedLinkKeyList(nrUINT& arLinkKey, BOOL bMoveOnly)
{
	arLinkKey.RemoveAll();
	nrUINT arSelectKey;
	CMap<UINT, UINT, UINT, UINT> mapLinkKey;

	int nSize = GetSelectedKeyList(arSelectKey, bMoveOnly);
	if(nSize == 0) return 0;

	int i, j;
	CDrawBase* pDrawClass = NULL;
	for(i = 0; i < nSize; i++)
	{
		pDrawClass = GetDrawClass(arSelectKey[i]);
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
	nSize = arLinkKey.GetSize();
	for(i = 0; i < nSize - 1 ; i++)
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

void CDrawCtrl::SetSelectedKeyList(nrUINT& arKey)
{
	RemoveAllSelectedKey();

	for(auto i = 0; i < arKey.GetSize(); i++) 
	{
		m_arSelectedDrawUnitKey.SetAt(arKey[i], arKey[i]);
	}
}

void CDrawCtrl::SetSelectedLinkKeyList(nrUINT& arLinkKey)
{	
	RemoveAllSelectedKey();
	int nSize = arLinkKey.GetSize();

	CDrawBase* pDrawClass = NULL;

	for(UINT iIndex = m_FinalEntityNum; iIndex > 0; iIndex--)
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
						m_arSelectedDrawUnitKey.SetAt(iIndex, iIndex);
						break;
					}
				}
			}
		}
	}
}

void CDrawCtrl::RemoveAllSelectedKey()
{
	m_arSelectedDrawUnitKey.RemoveAll();
}

void CDrawCtrl::AddSelectedKey(UINT key)
{
	m_arSelectedDrawUnitKey.SetAt(key, key);
}

void CDrawCtrl::AddSelectedKey(nrUINT& arKey)
{
	for(auto i = 0; i < arKey.GetSize(); i++) 
	{	
		AddSelectedKey(arKey[i]);
	}
}

void CDrawCtrl::DelSelectedKey(UINT key)
{
	m_arSelectedDrawUnitKey.RemoveKey(key);
}

void CDrawCtrl::DelSelectedKey(nrUINT& arKey)
{
	for(auto i = 0; i < arKey.GetSize(); i++) 
	{
		DelSelectedKey(arKey[i]);
	}
}

void CDrawCtrl::ReSelectedKey(UINT key)
{
	UINT nTempKey;
	if(m_arSelectedDrawUnitKey.Lookup(key, nTempKey))
	{
		DelSelectedKey(key);
	}
	else 
	{
		AddSelectedKey(key);
	}
}

void CDrawCtrl::ReSelectedKey(nrUINT& arKey)
{	
	for(auto i = 0; i < arKey.GetSize(); i++) 
	{
		ReSelectedKey(arKey[i]);
	}
}

BOOL CDrawCtrl::IsSelected(UINT key)
{
	UINT nTemp;
	return (m_arSelectedDrawUnitKey.Lookup(key, nTemp));
}


void CDrawCtrl::MergeLineByAngle(const nrUINT& arLineKey, nrLineR& arMergedLine,double dMergeAng)
{
	CDrawLine* pDrawClass = NULL;
	std::list<T_LINE_R> stlistLine;
	stlistLine.clear();
	for(UINT i = 0 ; i < arLineKey.GetSize() ; i++)
	{	
		T_LINE_R tEachLine;
		UINT nLinkKey = arLineKey[i];
		m_arLine.Lookup(nLinkKey, pDrawClass);
		if(pDrawClass->m_DataR.List.GetSize() != 1)
			ASSERT(0);
		tEachLine = pDrawClass->m_DataR.List[0];
		stlistLine.push_back(tEachLine);
	}

	std::list<T_LINE_R>::iterator listItr;
	listItr = stlistLine.begin();
	while( listItr != stlistLine.end() )
	{
		std::list<T_LINE_R>::iterator listItrNext;
		listItrNext = listItr;
		listItrNext ++;

		if(listItrNext == stlistLine.end())
			break;

		// 합칠지 판단
		BOOL bMerge = FALSE;
		double dEachAngle;
		dEachAngle = GetAngle(listItr->P1, listItr->P2, listItrNext->P1, listItrNext->P2);

		// 합치기
		if(dEachAngle <= dMergeAng)
		{			
			if(IsOverLap(listItr->P2, listItrNext->P1))
			{
				T_LINE_R tNewLine;
				tNewLine.P1 = listItr->P1;
				tNewLine.P2 = listItrNext->P2;

				listItr->P1 = tNewLine.P1;
				listItr->P2 = tNewLine.P2;
			}
			/*  // 방향성이 안맞는 경우가 있다면 여기 코드를 살려야 함
			else if(IsOverLap(listItr->P2, listItrNext->P2))
			{
				ASSERT(0);
				T_LINE_R tNewLine;
				tNewLine.P1 = listItr->P1;
				tNewLine.P2 = listItrNext->P1;

				listItr->P1 = tNewLine.P1;
				listItr->P2 = tNewLine.P2;
			}
			else if(IsOverLap(listItr->P1, listItrNext->P1))
			{
				ASSERT(0);
				T_LINE_R tNewLine;
				tNewLine.P1 = listItr->P2;
				tNewLine.P2 = listItrNext->P2;

				listItr->P1 = tNewLine.P1;
				listItr->P2 = tNewLine.P2;
			}
			else if(IsOverLap(listItr->P1, listItrNext->P2))
			{
				ASSERT(0);
				T_LINE_R tNewLine;
				tNewLine.P1 = listItr->P2;
				tNewLine.P2 = listItrNext->P1;

				listItr->P1 = tNewLine.P1;
				listItr->P2 = tNewLine.P2;
			}			*/
			else
			{
				ASSERT(0);
			}
			
			stlistLine.erase(listItrNext);

			bMerge = TRUE;
		}
		else
		{
			if(!IsOverLap(listItr->P2, listItrNext->P1))
			{
				ASSERT(0);  // 방향성이 안맞는 경우엔 여기에 들어옴... 웬지 그런 경우가 없는 것 같아서 이상황 고려 안했음..
				// 합치지 않으면 방향성만 맞춰 줘야 함..
			}
		}

		if(!bMerge)
			listItr++;
	}

	int nCnt = 0;
	arMergedLine.RemoveAll();
	arMergedLine.SetSize((int)stlistLine.size());
	for(listItr = stlistLine.begin() ; listItr != stlistLine.end() ; listItr++)
	{
		arMergedLine[nCnt] = *listItr;
		nCnt++;		
	}
}

void CDrawCtrl::GetMergedConnectedLine(nrUINT &arkeySrc, CArray<nrLineR, nrLineR&> &arMergedConnectedLine, double dMergeAng)
{
	CDrawLine* pDrawClass = NULL;
	if(arkeySrc.GetSize() == 0)  {ASSERT(0); return;}
	if(!m_arLine.Lookup(arkeySrc[0], pDrawClass))	{ASSERT(0); return;}
	
	std::list<UINT> stListRemainKey;	
	std::list<CDrawLine*> stListRemainPLine;
	for(UINT i = 0 ; i < arkeySrc.GetSize() ; i++)
	{	
		stListRemainKey.push_back(arkeySrc[i]);
	}	

	CArray<UINT, UINT> tarRemainKey;
	CArray<CDrawLine*, CDrawLine*> tarRemainPLine;
	nrUINT arKeyConnected;	
	arMergedConnectedLine.RemoveAll();
	while(stListRemainKey.size()) 
	{
		// stRemainKey를 이용해서 Remain정보를 갱신
		stListRemainPLine.clear();
		tarRemainKey.RemoveAll();
		tarRemainKey.SetSize((int)stListRemainKey.size());
		int nCnt = 0;
		std::list<UINT>::iterator stListItr;
		for(stListItr = stListRemainKey.begin(); stListItr != stListRemainKey.end() ; stListItr++)
		{
			tarRemainKey[nCnt] = *stListItr;
			nCnt++;

			m_arLine.Lookup(*stListItr, pDrawClass);
			stListRemainPLine.push_back(pDrawClass);
		}

		tarRemainPLine.RemoveAll();
		tarRemainPLine.SetSize((int)stListRemainPLine.size());
		nCnt = 0;
		std::list<CDrawLine*>::iterator stListItrUnit;
		for(stListItrUnit = stListRemainPLine.begin(); stListItrUnit != stListRemainPLine.end() ; stListItrUnit++)
		{
			tarRemainPLine[nCnt] = *stListItrUnit;
			nCnt++;
		}

		// 남아 있는 Line중 연결된 녀석을 가져 와서 Mergy
		arKeyConnected.RemoveAll();
		GetConnectedLineKey(tarRemainKey[0], 0, tarRemainPLine, tarRemainKey, arKeyConnected, 360);
		if(arKeyConnected.GetSize() == 0)
			ASSERT(0);

		nrLineR EachMergedLine;
		MergeLineByAngle(arKeyConnected,EachMergedLine,dMergeAng);

		// 출력된 변수에 추가 한 다음 stListRemainKey에 Remain 정보 갱신
		ASSERT(0);
		//arMergedConnectedLine.Add(EachMergedLine);
		
		for(int i = 0 ; i < arKeyConnected.GetSize() ; i++)
		{
			std::list<UINT>::iterator stListItr = std::find(stListRemainKey.begin(),stListRemainKey.end(),arKeyConnected[i]);
			stListRemainKey.erase(stListItr);
		}

		if(stListRemainKey.size() == 0)
			break;
	}
}

void CDrawCtrl::GetConnectedLineKey(UINT keySrc, int nSrcIdx, CArray<CDrawLine*, CDrawLine*> &arWholePLine, CArray<UINT, UINT> &arWholeKey, nrUINT &arKeyConnected, double dAngle)
{
	// hslee : 선이 두갈래 이상으로 갈라질 수 없다고 가정 했음..
	std::list<UINT> stlistConKeyForward;  // 선의 순서를 지켜주기 위해 list를 활용 함..
	std::list<UINT> stlistConKeyBackward;

	std::set<UINT>	setConnectedKey;
	setConnectedKey.clear();
	setConnectedKey.insert(keySrc);
	GetConnectedLineKeyOneDir(nSrcIdx, setConnectedKey, stlistConKeyForward, arWholePLine, arWholeKey, TRUE, dAngle);	
	GetConnectedLineKeyOneDir(nSrcIdx, setConnectedKey, stlistConKeyBackward, arWholePLine, arWholeKey, FALSE, dAngle);		

	stlistConKeyForward.push_front(keySrc);
	stlistConKeyBackward.reverse();

	arKeyConnected.SetSize((int)setConnectedKey.size());
	int nCnt = 0;
	std::list<UINT>::iterator listItr;
	for(listItr = stlistConKeyBackward.begin() ; listItr != stlistConKeyBackward.end() ; listItr++)
	{
		UINT uiKey = *listItr;
		arKeyConnected[nCnt] = uiKey;
		nCnt++;		
	}
	for(listItr = stlistConKeyForward.begin() ; listItr != stlistConKeyForward.end() ; listItr++)
	{		
		UINT uiKey = *listItr;
		arKeyConnected[nCnt] = uiKey;
		nCnt++;	
	}
}

void CDrawCtrl::GetConnectedLineKey(UINT keySrc, nrUINT &arKeyConnected, double dAngle)
{
	CDrawLine* pDrawClass = NULL;
	arKeyConnected.RemoveAll();
	if(!m_arLine.Lookup(keySrc, pDrawClass))	{ return; }

	CDrawLine *pDC_EachLine;
	CArray<CDrawLine*, CDrawLine*> arPLine;
	CArray<UINT, UINT>	   arKey;
	arPLine.SetSize(m_arLine.GetCount());
	arKey.SetSize(m_arLine.GetCount());
	POSITION Pos = m_arLine.GetStartPosition();
	UINT uiKey = 0;
	int nCnt = 0;
	int nSrcIdx = -1;

	while(Pos)
	{
		m_arLine.GetNextAssoc(Pos, uiKey, pDC_EachLine);

		arPLine[nCnt] = pDC_EachLine;
		arKey[nCnt] = uiKey;
		if(uiKey == keySrc)
			nSrcIdx = nCnt;
		nCnt++;		
	}
	if(nSrcIdx == -1)
		ASSERT(0);

	GetConnectedLineKey(keySrc, nSrcIdx, arPLine, arKey, arKeyConnected, dAngle);
}

void CDrawCtrl::GetConnectedLineKeyOneDir(int nSrcIdx, std::set<UINT> &setConnectedKey, std::list<UINT> &stlistConKey, CArray<CDrawLine*, CDrawLine*> &arWholePLine, CArray<UINT, UINT> &arWholeKey,
	BOOL bDirectForward ,double dAngle)
{
	stlistConKey.clear();
	BOOL bConnectForward = bDirectForward;  // 직전의 연결이 한점의 끝점과 다른점의 시작점이 였다면 TRUE, 한점의 끝점과 다른점의 끝점이였으면 FALSE

	std::set<UINT> setAddedKey;  // 이미 추가 했던것에 대해서는 연결성을 검사하지 않기 위해서 ...
	
	int nFromLineIdx = nSrcIdx;
	std::set<UINT>::iterator sitr;
	while(1)
	{
		T_POINT tpFrom;
		if(bConnectForward)
			tpFrom = arWholePLine[nFromLineIdx]->m_DataR.List[0].P2;
		else
			tpFrom = arWholePLine[nFromLineIdx]->m_DataR.List[0].P1;

		BOOL bConnected = FALSE;
		for(int i = 0; i < arWholePLine.GetSize() ; i++)
		{			
			if(i == nFromLineIdx)
				continue;

			sitr = setAddedKey.find(arWholeKey[i]);
			if(sitr != setAddedKey.end())
				continue;

			const T_POINT &tpToStart = arWholePLine[i]->m_DataR.List[0].P1;
			const T_POINT &tpToEnd   = arWholePLine[i]->m_DataR.List[0].P2;
			if(IsOverLap(tpFrom, tpToStart))  
			{
				double dEachAngle = GetAngle(arWholePLine[nFromLineIdx]->m_DataR.List[0].P1, arWholePLine[nFromLineIdx]->m_DataR.List[0].P2, tpToStart, tpToEnd);
				if(dEachAngle - dAngle <= CMathFunc::m_NormalZero)
				{
					bConnected = TRUE;
					bConnectForward = TRUE;
				}
			}
			if(IsOverLap(tpFrom, tpToEnd))
			{
				/*
				if(bConnected) ASSERT(0); // 선의 시작점과 끝점이 거의 동일한 위치에 있을 경우 여기에 걸림..
				*/
				double dEachAngle = GetAngle(arWholePLine[nFromLineIdx]->m_DataR.List[0].P1, arWholePLine[nFromLineIdx]->m_DataR.List[0].P2, tpToStart, tpToEnd);
				if(dEachAngle - dAngle <= CMathFunc::m_NormalZero)
				{
					bConnected = TRUE;
					bConnectForward = FALSE;
				}
			}

			if(bConnected)
			{
				sitr = setConnectedKey.find(arWholeKey[i]);
				if(sitr == setConnectedKey.end())
				{
					setConnectedKey.insert(arWholeKey[i]);
					setAddedKey.insert(arWholeKey[i]);
					stlistConKey.push_back(arWholeKey[i]);
					nFromLineIdx = i;
				}
				else
				{
					bConnected = FALSE;	// 모든 Polyline이 연결된 경우					 
				}

				break;				
			}
		}

		if(!bConnected)
			break;
	}
}

BOOL CDrawCtrl::GetSelectDrawUnitKey(POINT currPoint, UINT& SelectKey)
{
	CDrawBase* pDrawClass = NULL;

	T_DRAW_FORMAT DrawFormat;  
	T_TEXT_FORMAT TextFormat;

	int nDrawFormatType;
	int iTol = 1;
	BOOL bFill = FALSE;

	for(UINT iIndex = m_FinalEntityNum; iIndex > 0; iIndex--)
	{
		pDrawClass = GetDrawClass(iIndex);
		if(pDrawClass != NULL)
		{
			nDrawFormatType = pDrawClass->GetFormatType();
			if(nDrawFormatType == EN_DRFORMAT_TYPE_DRAW)
			{				
				if(!m_arDrawFormat.Lookup(pDrawClass->m_DrawFormatKey, DrawFormat)) DrawFormat.Init();
				bFill = DrawFormat.bFill;
				iTol  = max(1, m_nSelectDis) + DrawFormat.Width / 2;
			}
			else if(nDrawFormatType == EN_DRFORMAT_TYPE_TEXT)
			{				
				if(!m_arTextFormat.Lookup(pDrawClass->m_DrawFormatKey, TextFormat)) TextFormat.Init();
				bFill = TextFormat.bFill;
			}
			else if(nDrawFormatType == EN_DRFORMAT_TYPE_CHART)
			{
				bFill = TRUE;
			}
			else bFill = FALSE;

			if(pDrawClass->IsSelectCheck(currPoint, bFill, iTol)) 
			{
				SelectKey = iIndex;
				return TRUE;
			}
		}
	}
	return FALSE;
}