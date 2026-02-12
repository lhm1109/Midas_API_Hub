// CMStorAutoGenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStorAutoGenDlg.h"

#include <math.h>
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\StorSubData.h"

#include "..\wg_base\CompFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMStorAutoGenDlg dialog


CCMStorAutoGenDlg::CCMStorAutoGenDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMStorAutoGenDlg::IDD, pParent)
{
	m_pDoc = 0;
	m_bBaseFloorExist = FALSE;
	//{{AFX_DATA_INIT(CCMStorAutoGenDlg)
	m_bEccenChk = TRUE;	
	m_bWindEccenChk = TRUE;
	//}}AFX_DATA_INIT
}


void CCMStorAutoGenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMStorAutoGenDlg)
	DDX_Control(pDX, IDC_CMD_ECCEN_EDIT, m_wndEccenEdit);
	DDX_Control(pDX, IDC_CMD_UNSEL_LIST, m_wndUnselList);
	DDX_Control(pDX, IDC_CMD_SEL_LIST, m_wndSelList);
	DDX_Check(pDX, IDC_CMD_ECCEN_CHK, m_bEccenChk);	
	DDX_Control(pDX, IDC_CMD_WIND_ECCEN_EDIT, m_wndWindEccenEdit);	  
	DDX_Check(pDX, IDC_CMD_WIND_ECCEN_CHK, m_bWindEccenChk);	
	//}}AFX_DATA_MAP
}

void CCMStorAutoGenDlg::GetSeisWidth(CArray<T_SEIS_WIDTH, T_SEIS_WIDTH&>& Data)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pFlagCtrl = m_pDoc->m_pFlagCtrl;

	if (!GenCandidate())
	{
		SendMessage(WM_CLOSE, 0, 0);
		return;
	}

	for (auto i = 0; i < m_aSeisWidth.GetSize(); i++)
	{
		Data.Add(m_aSeisWidth[i]);
	}
}

//----------------------------------------------------------------------
// Implementation
void CCMStorAutoGenDlg::SetHeaderTitle(BOOL bUnsel)
{
	CString aTitle[][4] = {
		{_LS(IDS_WG_CMD__ADDD__No), _LS(IDS_WG_CMD__ADDD__Level), _T(""), _T("")},
		{_LS(IDS_WG_CMD__ADDD__No), _LS(IDS_WG_CMD__ADDD__Name), _LS(IDS_WG_CMD__ADDD__Level), _LS(IDS_WG_CMD__ADDD__Height),}, 
	};
	int aAlign[][4] = {
		{LVCFMT_RIGHT, LVCFMT_RIGHT, 0, 0},
		{LVCFMT_RIGHT, LVCFMT_LEFT, LVCFMT_RIGHT, LVCFMT_RIGHT},
	};

	int nColWidth[][4] = { {30, 70, 0, 0}, {30, 50, 70, 70}, };
	int nCount[] = {2, 4};

	// Select/Unselect flag
	int nIx;
	CListCtrl* pList;
	if (bUnsel) 
	{
		nIx = 0;
		pList = &m_wndUnselList;
	}
	else
	{
		nIx = 1;
		pList = &m_wndSelList;
	}

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(pList->GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(pList->GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < nCount[nIx]; i++)
	{
		title = aTitle[nIx][i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = aAlign[nIx][i];

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[nIx][i]);
		lvcolumn.pszText = title.GetBuffer(0);
		pList->InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMStorAutoGenDlg::MakeItemEx(BOOL bUnsel)
{
	CListCtrl* pList;

	if (bUnsel) pList = &m_wndUnselList;
	else pList = &m_wndSelList;

	int nCount = m_aSelFlag.GetSize();

	pList->DeleteAllItems();
	if (bUnsel)
	{
		int iItem = 0;
		for (int i = 0; i < nCount; i++)
		{
			if (!m_aSelFlag[i]) 
			{
				pList->InsertItem(iItem, _T(""));
				SetItem(bUnsel, iItem++, i, m_aStory[i]);        
			}
		}    
	}
	
	else
	{    
		int iItem = 0;
		int nTopStory = 0;
		double dNextLevel = 0.0;
		double dHeight = 0.0;    
		double dTopLevel = 0.0;
		
		MakeStoryName();
//    dTopLevel = m_aStory[nCount - 1].dStoryLevel;   // 건물 최상층의 Level
		for(int i = 0; i < nCount; i++)
		{
			if(m_aSelFlag[i]) nTopStory = i;       // 선택된 층 중에서 최상층을 찾음
		}
		dTopLevel = m_aStory[nTopStory].dStoryLevel;   // 건물 최상층의 Level

		for(int i = 0; i < nCount; i++)
		{       
			if(m_aSelFlag[i])    // story 개념 변경 반영
			{
				for(int j = i; j < nCount; j++)
				{
					if(m_aSelFlag[j]) 
					{
						if(m_aStory[j].dStoryLevel > m_aStory[i].dStoryLevel) 
						{ dNextLevel = m_aStory[j].dStoryLevel; break;}      // NextLevel은 해당층 바로 윗 층의 Level                            
					}
				}
				if(i == nTopStory) dHeight = dTopLevel - m_aStory[i].dStoryLevel;
				else               dHeight = dNextLevel - m_aStory[i].dStoryLevel;
				pList->InsertItem(iItem, _T(""));
				SetItem(bUnsel, iItem++, i, m_aStory[i], dHeight);
			}
		}
	}
}


/*/////////////
		for(int i = 0; i < nCount; i++)
		{
			if(m_aSelFlag[i]) 
				if(m_aStory[i].dStoryLevel > dPrevLevel) dPrevLevel = m_aStory[i].dStoryLevel;      
		}

		pTempList->DeleteAllItems();
		for(i = nCount - 1; i >= 0; i--)   // roof부터 내림차순으로 정열
		{
			if(m_aSelFlag[i])    // story 개념 변경 반영
			{
				dHeight = dPrevLevel - m_aStory[i].dStoryLevel;          
				pTempList->InsertItem(iItem, _T(""));
				SetItem(bUnsel, iItem++, i, m_aStory[i], dHeight);
				dPrevLevel = m_aStory[i].dStoryLevel;        
			}
		}
		
		for(i = 0; i < nCount; i++)   // 오름차순으로 층 정열
		{
			if(m_aSelFlag[i])
			{
				pList->InsertItem(iItem, _T(""));
				SetItem(bUnsel, iItem++, i, m_aStory[i], dHeight);
			}
		}
		// 바뀐것
		for (int i = 0; i < nCount; i++)
		{
			if (m_aSelFlag[i])    // story 개념 변경 반영
			{
				if(i == nCount - 1) dHeight = 0.0;   // roof층은 층고를 0으로 
				else dHeight = m_aStory[i+1].dStoryLevel - dPrevLevel;
				pList->InsertItem(iItem, _T(""));
				SetItem(bUnsel, iItem++, i, m_aStory[i], dHeight);
				if(i == nCount - 1) dPrevLevel = m_aStory[i].dStoryLevel;
				else dPrevLevel = m_aStory[i+1].dStoryLevel;
			}
		}
		// 이전 
		for (int i = 0; i < nCount; i++)
		{
			if (m_aSelFlag[i])
			{
				if (iItem == 0) dHeight = 0.0;
				else dHeight = m_aStory[i].dStoryLevel - dPrevLevel;

				pList->InsertItem(iItem, _T(""));
				SetItem(bUnsel, iItem++, i, m_aStory[i], dHeight);
				dPrevLevel = m_aStory[i].dStoryLevel;

			}
		}
*////////////////

BOOL CCMStorAutoGenDlg::SetItem(BOOL bUnsel, int iItem, int nNo, T_STOR_D &Data, double dHeight)
{
	CString str;
	CListCtrl* pList;

	int nColCount;
	if (bUnsel) 
	{
		nColCount = 2;
		pList = &m_wndUnselList;
	}
	else
	{
		nColCount = 4;
		pList = &m_wndSelList;
	}

	for(int i = 0; i < nColCount; i++)
	{
		str = DataToStr(bUnsel, i, nNo, Data, dHeight);
		pList->SetItemText(iItem, i, str);
	}
	pList->SetItemData(iItem, nNo);
	
	return TRUE;
}

CString CCMStorAutoGenDlg::DataToStr(BOOL bUnsel, int i, int nNo, T_STOR_D &Data, double dHeight)
{
	CString str;
	if (bUnsel)
	{
		if (i == 0) str.Format(_T("%d"), nNo+1);
		else if (i == 1) str.Format(_T("%g"), Data.dStoryLevel);
		else str = _LS(IDS_WG_CMD__ADDD__error);
	}
	else
	{
		if (i == 0) str.Format(_T("%d"), nNo+1);
		else if (i == 1) str = Data.StoryName;
		else if (i == 2) str.Format(_T("%g"), Data.dStoryLevel);
		else if (i == 3) str.Format(_T("%g"), dHeight);
		else str = _LS(IDS_WG_CMD__ADDD__error);
	}
	return str;
}

/* Old version : by jbseon 2002. 06.05 (Story 개념 변경)
BOOL CCMStorAutoGenDlg::GenCandidate()
{
	int nNodeCount = m_pDoc->m_pAttrCtrl->GetCountNode();
	if (nNodeCount == 0) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Numbef_of_nodes_is_0__Input_node_));
		return FALSE;
	}

	CArray<T_NODE_D, T_NODE_D&> aNode;
	aNode.SetSize(nNodeCount);
	T_NODE_K key;
	POSITION pos = m_pDoc->m_pAttrCtrl->GetStartNode();
	int i = 0;
	while (pos != NULL) 
		m_pDoc->m_pAttrCtrl->GetNextNode(pos, key, aNode[i++]);

	// Sorting
	qsort(aNode.GetData(), nNodeCount, sizeof(T_NODE_D), CompAscNodeZ);

	// Story 분리 및 Min, Max 찾기.
	CArray<double, double> aZ, aMinX, aMinY, aMaxX, aMaxY;
	double dblTol = GetStoryTolerance(aNode);
	int nStory = 0;
	double dblBase = 0;
	i = nNodeCount-1;
	dblBase = aNode[i].z;
	aZ.Add(dblBase);
	aMinX.Add(aNode[i].x); aMaxX.Add(aNode[i].x);
	aMinY.Add(aNode[i].y); aMaxY.Add(aNode[i].y);
	for (i--; i >= 0; i--)
	{
		if (aNode[i].z < dblBase-dblTol) 
		{
			dblBase = aNode[i].z;
			aZ.Add(dblBase);
			aMinX.Add(aNode[i].x); aMaxX.Add(aNode[i].x);
			aMinY.Add(aNode[i].y); aMaxY.Add(aNode[i].y);
			nStory++;
		}
		else
		{
			if (aNode[i].x < aMinX[nStory]) aMinX[nStory] = aNode[i].x;
			else if (aNode[i].x > aMaxX[nStory]) aMaxX[nStory] = aNode[i].x;
			if (aNode[i].y < aMinY[nStory]) aMinY[nStory] = aNode[i].y;
			else if (aNode[i].y > aMaxY[nStory]) aMaxY[nStory] = aNode[i].y;
		}
	}
	aZ.SetAtGrow(nStory, aNode[0].z); // Base는 가장 낮은 좌표로 재 설정

	//------------------
	// Story Data 생성
	//------------------

	// Base Level Detection
	double dblBaseLevel = GetBaseLevel();
	for (i = 0; i <= nStory; i++)
		if (aZ[i] <= dblBaseLevel) break;

	if (i == nStory+1)  // Base가 모델보다 밑에 설정된 경우
		i = nStory;

	// 지하층을 만든다.
	CString Name; 
	Name = _LS(IDS_WG_CMD__ADDD__Base);

	BuildStoryData(m_aStory, Name, aZ[nStory], FALSE, 
								 aMinX[nStory], aMaxX[nStory], aMinY[nStory], aMaxY[nStory]);
	m_aStoryKind.Add(-2); // -2:Base, -1:B??, 1:??F, 2:Roof

	int nStoryNo = nStory-i;
	int nUnderNo = 0;
	for (int j = nStory-1; j >= i; j--)
	{
		Name.Format(_T("B%d"), nStoryNo--);
		BuildStoryData(m_aStory, Name, aZ[j], TRUE, 
									 aMinX[j], aMaxX[j], aMinY[j], aMaxY[j]);
		m_aStoryKind.Add(-1);
		nUnderNo++;
	}
	if (nUnderNo != 0) m_bBaseFloorExist = TRUE;
	else m_bBaseFloorExist = FALSE;

	// 지상층을 만든다.
	nStoryNo = 1;
//pig  if (nUnderNo <= 0) nStoryNo = 2;  // 지하 없으면 2F부터
	for (j = i-1; j > 0; j--)
	{
		Name.Format(_T("%dF"), nStoryNo++);
		BuildStoryData(m_aStory, Name, aZ[j], TRUE, 
									 aMinX[j], aMaxX[j], aMinY[j], aMaxY[j]);
		m_aStoryKind.Add(1);
	}

	// Roof를 만든다.
	if (i > 0)  // Base가 모델보다 위에 설정된 경우가 아니라면 Roof가 존재
	{
		Name = _LS(IDS_WG_CMD__ADDD__Roof);
		//GSaveHistoryFormatNF(_T("%s:Level=%f, Height=%f, Width(x,y)=(%f,%f)"),
		//                   Name, aZ[0], (double)0.0,
		//                   aMaxX[0]-aMinX[0], aMaxY[0]-aMinY[0]);
		BuildStoryData(m_aStory, Name, aZ[0], TRUE, 
									 aMinX[0], aMaxX[0], aMinY[0], aMaxY[0]);
		m_aStoryKind.Add(2);
	}

	int nStoryNum = m_aStory.GetSize();
	if (nStoryNum == 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_generated_story_));
		return FALSE;
	}

	// Select Flag 조정  : All selected
	m_aSelFlag.SetSize(nStoryNum);
	for (int k = 0; k < nStoryNum; k++) m_aSelFlag[k] = TRUE;

	return TRUE;
}
*/
// New version : by jbseon 2002.06.05
BOOL CCMStorAutoGenDlg::GenCandidate()
{
	int nNodeCount = m_pDoc->m_pAttrCtrl->GetCountNode();
	if (nNodeCount == 0) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Numbef_of_nodes_is_0__Input_node_));
		return FALSE;
	}

	CArray<T_NODE_K, T_NODE_K>  aNodeK;
	CArray<T_NODE_D, T_NODE_D&> aNode;
	aNodeK.SetSize(nNodeCount);
	aNode.SetSize(nNodeCount);
	//T_NODE_K key;
	POSITION pos = m_pDoc->m_pAttrCtrl->GetStartNode();
	int i = 0;
	while (pos != NULL) 
	{
		m_pDoc->m_pAttrCtrl->GetNextNode(pos, aNodeK[i], aNode[i]);
		i++;
	}

	if(m_pDoc->EnableJUD())
	{// iGen일경우 주각에 사용된 node를 사용하지 않습니다.
		T_NODE_K NodeK;
		CArray<T_BPLT_K, T_BPLT_K> aBpltK;
		CArray<T_NODE_K, T_NODE_K> aBpltNodeK;
		int nSize = m_pDoc->m_pAttrCtrl2->GetBpltKeyList(aBpltK);
		aBpltNodeK.SetSize(nSize);
		for(i=0 ; i<nSize ; i++)
			m_pDoc->m_pAttrCtrl2->GetRigidNodeKey(aBpltK[i], aBpltNodeK[i], NodeK);

		if(nSize > 0)
		{
			for(i=nNodeCount-1 ; i>=0 ; i--)
			{
				for(int j=0 ; j<nSize ; j++)
				{
					if(aNodeK[i] == aBpltNodeK[j])
					{ 
						aNodeK.RemoveAt(i);
						aNode.RemoveAt(i); 
						break;
					}
				}
			}
		}
		nNodeCount = aNode.GetSize();
	}
	
	// Sorting
	qsort(aNode.GetData(), nNodeCount, sizeof(T_NODE_D), CompAscNodeZ);

	// Story 분리 및 Min, Max 찾기.
	CArray<double, double> aZ, aMinX, aMinY, aMaxX, aMaxY;
//  double dblTol = GetStoryTolerance(aNode);
	double dblTol=m_pFlagCtrl->GetGeneralCoordinateToleranceCurrentUnit();
	int nStory = 0;
	double dblBase = 0;
	i = nNodeCount-1;
	dblBase = aNode[i].z;
	aZ.Add(dblBase);
	aMinX.Add(aNode[i].x); aMaxX.Add(aNode[i].x);
	aMinY.Add(aNode[i].y); aMaxY.Add(aNode[i].y);
	for (i--; i >= 0; i--)
	{
		if (aNode[i].z <= dblBase-dblTol) 
		{
			dblBase = aNode[i].z;
			aZ.Add(dblBase);
			aMinX.Add(aNode[i].x); aMaxX.Add(aNode[i].x);
			aMinY.Add(aNode[i].y); aMaxY.Add(aNode[i].y);
			nStory++;
		}
		else
		{
			if (aNode[i].x < aMinX[nStory])
				aMinX[nStory] = aNode[i].x;
			else if (aNode[i].x > aMaxX[nStory]) 
				aMaxX[nStory] = aNode[i].x;

			if (aNode[i].y < aMinY[nStory])
				aMinY[nStory] = aNode[i].y;
			else if (aNode[i].y > aMaxY[nStory])
				aMaxY[nStory] = aNode[i].y;
		}
	}
	aZ.SetAtGrow(nStory, aNode[0].z); // Base는 가장 낮은 좌표로 재 설정

	//------------------
	// Story Data 생성
	// aZ가 내림차순으로 정렬되어 있음
	// m_aStoryKind : -1:B??, 1:??F, 2:Roof
	//------------------
	int nStoryNum = aZ.GetSize();

	// Base Level Detection
	double dblBaseLevel = 0.;
	dblBaseLevel = m_pDoc->m_pAttrCtrl->GetGroundLevel();

	// 지하 1층을 찾아낸다.
	const double dRegardZero = 1.0e-10;
	int nBase = 0;
	for (nBase = 0; nBase < nStoryNum; nBase++)
	{
		if (aZ[nBase] < (dblBaseLevel-dRegardZero)) break;   // MNET:XXXX-HSSHIM-20100419. tolerance 적용.
	}

	int nBaseNum = nStoryNum - nBase;
	// 모두 지하이면 지하 최상부는 Roof가 되므로 1 뺀다.
	if (aZ[0] < dblBaseLevel) nBaseNum--;
	int nFloorNum = nStoryNum - nBaseNum - 1;

	// [2009-10-11] Kim, Geun Young (Tel: 2042, gykim@midasit.com)
	// Sbdo의 z level data를 만든다.  
	BOOL bSel;
	BOOL bSbdoData = FALSE;
	UINT nTemp = 0;
	CMap<double,double,UINT,UINT> mapSbdoLevel; mapSbdoLevel.InitHashTable(1001);   
	CArray<T_SBDO_K,T_SBDO_K> aSbdoList;
	m_pDoc->m_pAttrCtrl->GetSbdoKeyList(aSbdoList);
	int nSbdoCount = aSbdoList.GetSize();
	if(nSbdoCount > 0)
	{ 
		CArray<double,double&> aSbdoLevel;
		for(i=0; i<nSbdoCount; i++)
		{  
			m_pDoc->m_pAttrCtrl->GetSlabMatTypeSbdoLevel(aSbdoList[i], aSbdoLevel);  
			for (int j=0; j<aSbdoLevel.GetSize(); j++)      
				mapSbdoLevel.SetAt(aSbdoLevel[j], nTemp);      
		}
		if(mapSbdoLevel.GetCount() > 0)
			bSbdoData = TRUE;
	}  

	// 지하층을 만든다.
	CString Name; 
	int ix = 0;
	for (i = 0; i < nBaseNum; i++)
	{
		ix = nStoryNum-1-i;
		Name.Format(_T("B%d"), nBaseNum-i);
		if(i == 0)     
		{
			BuildStoryData(m_aStory, Name, aZ[ix], FALSE);   
			bSel = TRUE;
			m_aSelFlag.Add(bSel);   
		}
		else       
		{
			if(bSbdoData)
			{
				if(mapSbdoLevel.Lookup(aZ[ix], nTemp)) 
				{
					BuildStoryData(m_aStory, Name, aZ[ix], FALSE);    
					bSel = TRUE;                    
				}
				else
				{
					BuildStoryData(m_aStory, Name, aZ[ix], TRUE);
					bSel = FALSE;
				}
				m_aSelFlag.Add(bSel); 
			}
			else      
				BuildStoryData(m_aStory, Name, aZ[ix], TRUE);         
		}
		m_aStoryKind.Add(-1);
		// Seismic Eccentricity를 위한 건물의 폭
		BuildSeisWidth(m_aSeisWidth, aMinX[ix], aMaxX[ix], aMinY[ix], aMaxY[ix]);    
	}
	if (nBaseNum > 0) m_bBaseFloorExist = TRUE;
	else m_bBaseFloorExist = FALSE;

	// 지상층을 만든다.
	int nStoryNo = 0;
	for (i = 0; i < nFloorNum; i++)
	{
		ix = nStoryNum-1-nBaseNum-i;
		Name.Format(_T("%dF"), i+1);
		// Do not consider story diaphragm at 1st floor, and consider story diaphragm above 1st floor
		if(nBaseNum == 0 && i == 0)  
		{
			BuildStoryData(m_aStory, Name, aZ[ix], FALSE);      
			bSel = TRUE;
			m_aSelFlag.Add(bSel);   
		}
		else  
		{
			if(bSbdoData)
			{
				if(mapSbdoLevel.Lookup(aZ[ix], nTemp)) 
				{
					BuildStoryData(m_aStory, Name, aZ[ix], FALSE);    
					bSel = TRUE;        
				}
				else
				{
					BuildStoryData(m_aStory, Name, aZ[ix], TRUE);
					bSel = FALSE;  
				}
				m_aSelFlag.Add(bSel);   
			}
			else     
				BuildStoryData(m_aStory, Name, aZ[ix], TRUE);                 
		}
		m_aStoryKind.Add(1);
		// Seismic Eccentricity를 위한 건물의 폭
		BuildSeisWidth(m_aSeisWidth, aMinX[ix], aMaxX[ix], aMinY[ix], aMaxY[ix]);
	}

	// Roof를 만든다.(항상 존재)
	Name = _LS(IDS_WG_CMD__ADDD__Roof);
	if(bSbdoData)
	{
		if(mapSbdoLevel.Lookup(aZ[0], nTemp)) 
		{
			BuildStoryData(m_aStory, Name, aZ[0], FALSE);    
			bSel = TRUE;        
		}
		else
		{
			BuildStoryData(m_aStory, Name, aZ[0], TRUE);
			bSel = FALSE;  
		}
		m_aSelFlag.Add(bSel);   
	}
	else
		BuildStoryData(m_aStory, Name, aZ[0], TRUE);
 
	m_aStoryKind.Add(2);
	// Seismic Eccentricity를 위한 건물의 폭
	BuildSeisWidth(m_aSeisWidth, aMinX[0], aMaxX[0], aMinY[0], aMaxY[0]);  
	
	nStoryNum = m_aStory.GetSize();
	if (nStoryNum == 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_generated_story_));
		return FALSE;
	} 
	
	if(!bSbdoData)
	{
		// Select Flag 조정  : All selected
		m_aSelFlag.SetSize(nStoryNum);
		for (int k = 0; k < nStoryNum; k++) m_aSelFlag[k] = TRUE;
	}
 
	return TRUE;
}

void CCMStorAutoGenDlg::BuildStoryData(CArray<T_STOR_D, T_STOR_D&> &aStory, 
																CString &Name, double dblLevel, BOOL bDiaph)
{
//#ifdef _MDEMO
//  bDiaph = FALSE;
//#endif

	T_STOR_D data;
	data.Initialize();
	data.StoryName = Name;
	data.dStoryLevel = dblLevel;
	data.bFloorDiaphragm = bDiaph;  
	aStory.Add(data);
}

void CCMStorAutoGenDlg::BuildSeisWidth(CArray<T_SEIS_WIDTH, T_SEIS_WIDTH&> &aSeisWidth, 
												double dblMinX, double dblMaxX, double dblMinY, double dblMaxY)
{
	T_SEIS_WIDTH data;
	data.Initialize();  
	data.dSeisWidthX = dblMaxX - dblMinX;
	data.dSeisWidthY = dblMaxY - dblMinY; 
	double dCompTol = 1.0e-10; // MQC-5057 : gykim
	if(fabs(data.dSeisWidthX) < dCompTol) data.dSeisWidthX = 0.;
	if(fabs(data.dSeisWidthY) < dCompTol) data.dSeisWidthY = 0.;  
	aSeisWidth.Add(data);
}

double CCMStorAutoGenDlg::GetStoryTolerance(CArray<T_NODE_D, T_NODE_D&>& aNode)
{
	int nLast = aNode.GetSize()-1;
	return (aNode[nLast].z - aNode[0].z)/100000.0;
}

int CCMStorAutoGenDlg::CompAscNodeZ(const void* a1, const void* a2)
{
	T_NODE_D* pNode1 = (T_NODE_D*)a1;
	T_NODE_D* pNode2 = (T_NODE_D*)a2;

	if (pNode1->z > pNode2->z) return 1;
	else if (pNode1->z < pNode2->z) return -1;
	else return 0;
}

void CCMStorAutoGenDlg::MakeStoryName()
{
	int nCount = m_aSelFlag.GetSize();
	int i = 0, j=0;
	for (int i = 0; i < nCount; i++)
	{
		if (m_aSelFlag[i] && m_aStoryKind[i] > 0) break;
	}
	int nFloor = 1;
	for (j = i-1; j >= 0; j--)
	{
		if (m_aSelFlag[j] && m_aStoryKind[j] == -1) // Select된 지하층
			m_aStory[j].StoryName.Format(_T("B%d"), nFloor++);
	}
//pig  if (m_bBaseFloorExist) nFloor = 1;
//pig  else nFloor = 2;
	nFloor = 1;
	for (j = i; j < nCount; j++)
	{
		if (m_aSelFlag[j] && m_aStoryKind[j] == 1) // Select된 지상층
			m_aStory[j].StoryName.Format(_T("%dF"), nFloor++);
	}
}

void CCMStorAutoGenDlg::GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem)
{
	int  nItem = -1;

	// Update all of the selected items.
	UINT uSelectedCount = pList->GetSelectedCount();
	aSelItem.SetSize(uSelectedCount);
	for (UINT i=0; i < uSelectedCount; i++)
	{
		nItem = pList->GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		aSelItem[i] = nItem;
	}
}

BEGIN_MESSAGE_MAP(CCMStorAutoGenDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMStorAutoGenDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_SEL_LIST, OnDblclkCmdSelList)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_UNSEL_LIST, OnDblclkCmdUnselList)
	ON_BN_CLICKED(IDC_CMD_ECCEN_CHK, OnCmdEccenChk)
	ON_BN_CLICKED(IDC_CMD_WIND_ECCEN_CHK, OnCmdWindEccenChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMStorAutoGenDlg message handlers

BOOL CCMStorAutoGenDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	m_pFlagCtrl=m_pDoc->m_pFlagCtrl;

	// TODO: Add extra initialization here
	if (!GenCandidate())
	{
		SendMessage(WM_CLOSE, 0, 0);
		return TRUE;
	}

	SetHeaderTitle(TRUE);
	SetHeaderTitle(FALSE);

	MakeItemEx(TRUE);
	MakeItemEx(FALSE);
	
#if defined(_JP)   // 일본에서는 우발편심을 고려하지 않음
	m_bEccenChk = FALSE;  
#endif

	T_SECC_D SeccD;   
	if (!m_pDoc->m_pAttrCtrl->GetSecc(SeccD))   SeccD.Initialize();
	m_bEccenChk = SeccD.bIncludeEccSeis;	
	m_bWindEccenChk = SeccD.bIncludeEccWind;

	if(CProduct::GetMovingType() == D_PRODUCT_MOVING_CH)
	{
		m_bWindEccenChk = FALSE; // 800_CH에서는 IBC 삭제
		GetDlgItem(IDC_CMD_WIND_ECCEN_CHK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_WIND_ECCEN_STC)->ShowWindow(SW_HIDE);    
		m_wndWindEccenEdit.ShowWindow(SW_HIDE);
	}
	
	m_wndEccenEdit.EnableWindow(m_bEccenChk);
	m_wndEccenEdit.SetEditUnit(SeccD.dEccSeis);  //m_wndEccenEdit.SetEditUnit(5.0);
	m_wndWindEccenEdit.EnableWindow(m_bWindEccenChk);
	m_wndWindEccenEdit.SetEditUnit(SeccD.dEccWind);  //m_wndWindEccenEdit.SetEditUnit(15.0);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMStorAutoGenDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	CArray<int, int> aItem;
	GetSelectedItemList(&m_wndUnselList, aItem);
	if (aItem.GetSize() == 0) return;

	int nIx;
	int nSize = aItem.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		nIx = m_wndUnselList.GetItemData(aItem[i]);
		m_aSelFlag[nIx] = TRUE;
	}
	MakeItemEx(TRUE);
	MakeItemEx(FALSE);
}

void CCMStorAutoGenDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	CArray<int, int> aItem;
	GetSelectedItemList(&m_wndSelList, aItem);
	if (aItem.GetSize() == 0) return;

	int nIx;
	int nSize = aItem.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		nIx = m_wndSelList.GetItemData(aItem[i]);
		m_aSelFlag[nIx] = FALSE;
	}
	MakeItemEx(TRUE);
	MakeItemEx(FALSE);
}

void CCMStorAutoGenDlg::OnDblclkCmdSelList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnDelete();
	*pResult = 0;
}

void CCMStorAutoGenDlg::OnDblclkCmdUnselList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnAdd();
	*pResult = 0;
}

void CCMStorAutoGenDlg::OnOK() 
{
	int nNodeCount = m_pDoc->m_pAttrCtrl->GetCountNode();
	
	CArray<T_NODE_D, T_NODE_D&> aNode;
	aNode.SetSize(nNodeCount);
	T_NODE_K key;
	POSITION pos = m_pDoc->m_pAttrCtrl->GetStartNode();
	int i = 0;
	while (pos != NULL) 
		m_pDoc->m_pAttrCtrl->GetNextNode(pos, key, aNode[i++]);

	// Sorting
	qsort(aNode.GetData(), nNodeCount, sizeof(T_NODE_D), CompAscNodeZ);

	// Story 분리 및 Min, Max 찾기.
	CArray<double, double> aZ, aMinX, aMinY, aMaxX, aMaxY;
//  double dblTol = GetStoryTolerance(aNode);
	double dblTol=m_pFlagCtrl->GetGeneralCoordinateToleranceCurrentUnit();
	int nStory = 0;
	double dblBase = 0;
	i = nNodeCount-1;
	dblBase = aNode[i].z;
	aZ.Add(dblBase);
	aMinX.Add(aNode[i].x); aMaxX.Add(aNode[i].x);
	aMinY.Add(aNode[i].y); aMaxY.Add(aNode[i].y);
	for (i--; i >= 0; i--)
	{
		if (aNode[i].z <= dblBase-dblTol) 
		{
			dblBase = aNode[i].z;
			aMinX.Add(aNode[i].x); aMaxX.Add(aNode[i].x);
			aMinY.Add(aNode[i].y); aMaxY.Add(aNode[i].y);
			nStory++;
		}
		else
		{
			if      (aNode[i].x < aMinX[nStory]-dblTol) aMinX[nStory] = aNode[i].x;
			else if (aNode[i].x > aMaxX[nStory]+dblTol) aMaxX[nStory] = aNode[i].x;

			if      (aNode[i].y < aMinY[nStory]-dblTol) aMinY[nStory] = aNode[i].y;
			else if (aNode[i].y > aMaxY[nStory]+dblTol) aMaxY[nStory] = aNode[i].y;
		}
	}
	
	double dEccentricity = m_wndEccenEdit.GetEditValue();   // 우발편심값
	double dWindEccentricity = m_wndWindEccenEdit.GetEditValue();   // 풍하중에 대한 편심비율

	if(dEccentricity < 0)
	{ AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Eccentricity));  return; }
	
	if(dWindEccentricity < 0)
	{ AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Eccentricity));  return; }  

	T_SECC_D SeccD;
	if (!m_pDoc->m_pAttrCtrl->GetSecc(SeccD))  SeccD.Initialize();
	SeccD.bIncludeEccSeis = m_bEccenChk;
	SeccD.bIncludeEccWind = m_bWindEccenChk;
	SeccD.dEccSeis = dEccentricity;
	SeccD.dEccWind = dWindEccentricity;
	if (!m_pDoc->m_pDataCtrl->AddSecc(SeccD))  { ASSERT(0);  return; }


	CArray<T_STOR_D, T_STOR_D&> aStory;
	int nU;  
	int nSize = m_aSelFlag.GetSize();
	for (i = 0; i < nSize; i++)
	{
		if(m_bEccenChk)  // 우발편심을 고려할 때
		{
			m_aStory[i].SeisEccentX = m_aSeisWidth[i].dSeisWidthX * dEccentricity / 100.;  
			m_aStory[i].SeisEccentY = m_aSeisWidth[i].dSeisWidthY * dEccentricity / 100.;
		}
		else   // 우발편심을 고려하지 않을 때
		{
			m_aStory[i].SeisEccentX = 0.;  
			m_aStory[i].SeisEccentY = 0.;
		}    

		if(m_bWindEccenChk)  
		{
			m_aStory[i].WindEccentX = m_aSeisWidth[i].dSeisWidthX * dWindEccentricity / 100.;  
			m_aStory[i].WindEccentY = m_aSeisWidth[i].dSeisWidthY * dWindEccentricity / 100.;
		}
		else  
		{
			m_aStory[i].WindEccentX = 0.;  
			m_aStory[i].WindEccentY = 0.;
		}    
		
		BOOL bFindUpperStory = FALSE;
		// 사용되고 있는 바로 위층을 찾는다.
		if(i < nSize - 1)
		{
			for(int j = i + 1; j < nSize; j++)
			{
				if(m_aSelFlag[j]) 
				{
					nU = j;

					// PMS:2689-HSSHIM-20090629
					// [다탑 형태를 위한 예외처리] 위층과 MBR이 겹치는지 비교한다.
					double dCompTol = 1.0e-10; // MQC-5057 : gykim
					int nIndex_cur = nSize-1-i ;   ASSERT(nIndex_cur > 0);
					int nIndex_up  = nSize-1-nU;   ASSERT(nIndex_up  >= 0);
					if (CCompFunc::CompRealTol(aMinX[nIndex_cur], aMaxX[nIndex_up], dCompTol) <= 0 &&
						CCompFunc::CompRealTol(aMaxX[nIndex_cur], aMinX[nIndex_up], dCompTol) >= 0 &&
						CCompFunc::CompRealTol(aMinY[nIndex_cur], aMaxY[nIndex_up], dCompTol) <= 0 &&
						CCompFunc::CompRealTol(aMaxY[nIndex_cur], aMinY[nIndex_up], dCompTol) >= 0)
					{
						bFindUpperStory = TRUE;          
						break;
					}
				}
			}
		}

		if(bFindUpperStory)  // 사용되고 있는 바로 위층과 현재층을 비교하여 풍압폭을 계산한다.  
			BuildWindWidth(m_aStory[i], aMinX[nSize-1-i], aMaxX[nSize-1-i], aMinY[nSize-1-i], aMaxY[nSize-1-i], 
																	aMinX[nSize-1-nU], aMaxX[nSize-1-nU], aMinY[nSize-1-nU], aMaxY[nSize-1-nU]);          
		else                 // 최상층이거나 사용되고 있는 위층이 없는 경우에는 자신의 층만으로 풍압폭을 계산한다.
			BuildWindWidth(m_aStory[i], aMinX[nSize-1-i], aMaxX[nSize-1-i], aMinY[nSize-1-i], aMaxY[nSize-1-i], 
																	aMinX[nSize-1-i], aMaxX[nSize-1-i], aMinY[nSize-1-i], aMaxY[nSize-1-i]);


		if (m_aSelFlag[i])
			aStory.Add(m_aStory[i]);
	}

	if (aStory.GetSize() > 0) 
	{
		if (!m_pDoc->m_pDataCtrl->AddStor(aStory, TRUE))  return;
	}

	CStorSubData::GetInstance().AddAllStorKeyForLineGridIfCheckedAll();
	CStorSubData::GetInstance().SetNeedUpdateStor(true);
	CDialogMove::OnOK();
}

void CCMStorAutoGenDlg::OnCmdEccenChk() 
{
	UpdateData(TRUE);
	m_wndEccenEdit.EnableWindow(m_bEccenChk);  
}

void CCMStorAutoGenDlg::OnCmdWindEccenChk() 
{
	UpdateData(TRUE);
	m_wndWindEccenEdit.EnableWindow(m_bWindEccenChk);  
}

void CCMStorAutoGenDlg::BuildWindWidth(T_STOR_D& StorData,                       
											double dblMinXC, double dblMaxXC, double dblMinYC, double dblMaxYC,
											double dblMinXU, double dblMaxXU, double dblMinYU, double dblMaxYU)
{  
	double dWindWidthXC = dblMaxXC - dblMinXC;
	double dWindWidthXU = dblMaxXU - dblMinXU;
	double dWindWidthYC = dblMaxYC - dblMinYC;
	double dWindWidthYU = dblMaxYU - dblMinYU;
	BOOL bCurrentX = FALSE, bCurrentY = FALSE;
	if(dWindWidthXC <= dWindWidthXU) bCurrentX = TRUE;
	if(dWindWidthYC <= dWindWidthYU) bCurrentY = TRUE;

	StorData.WindWidthX = min(dWindWidthXC, dWindWidthXU);
	StorData.WindWidthY = min(dWindWidthYC, dWindWidthYU);  
	
	StorData.WindCenterX = (dblMaxXC + dblMinXC) / 2.0;   // 풍하중 재하 위치는 항상 현재층의 가운데로 한다.
	StorData.WindCenterY = (dblMaxYC + dblMinYC) / 2.0;
	
	double dCompTol = 1.0e-10; // MQC-5057 : gykim
	if(fabs(StorData.WindWidthX ) < dCompTol) StorData.WindWidthX  = 0.;
	if(fabs(StorData.WindWidthY ) < dCompTol) StorData.WindWidthY  = 0.;  
	if(fabs(StorData.WindCenterX) < dCompTol) StorData.WindCenterX = 0.;
	if(fabs(StorData.WindCenterY) < dCompTol) StorData.WindCenterY = 0.;  

//  if(bCurrentX) StorData.WindCenterX = (dblMaxXC + dblMinXC) / 2.0;
//  else StorData.WindCenterX = (dblMaxXU + dblMinXU) / 2.0;
	
//  if(bCurrentY) StorData.WindCenterY = (dblMaxYC + dblMinYC) / 2.0;
//  else StorData.WindCenterY = (dblMaxYU + dblMinYU) / 2.0;  
}
