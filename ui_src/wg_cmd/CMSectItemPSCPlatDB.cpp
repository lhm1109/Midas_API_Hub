// CMSectItemPSCPlatDB.cpp : implementation file
//
// 2004. 03. 04  by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSCPlatDB.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCMidDB 

CCMSectItemPSCPlatDB::CCMSectItemPSCPlatDB()
{
	m_aDBNameList.RemoveAll();
	m_aSize.RemoveAll();
	//m_aOffset.RemoveAll();

	MakeData();
}

CCMSectItemPSCPlatDB::~CCMSectItemPSCPlatDB()
{
	CArray<double, double>* pData;
	int nNum = m_aSize.GetSize();
	for (int i=0; i<nNum; i++)
	{
		pData = m_aSize.GetAt(i);
		delete pData;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Interface

int  CCMSectItemPSCPlatDB::GetDBNameList(CStringArray& aNameList)
{
	aNameList.Copy(m_aDBNameList);
	return aNameList.GetSize();
}

BOOL CCMSectItemPSCPlatDB::GetDBData(CString &csName, int &nOffset, CArray<double, double> &aSize)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	int nNum = m_aDBNameList.GetSize();
	int i = 0;
	for (i=0; i<nNum; i++)
		if (m_aDBNameList[i] == csName) break;
	if (i == nNum) return FALSE;

	nOffset = 0;

	CArray<double, double> aDBSize;
	CArray<double, double>* pSize;
	pSize = m_aSize.GetAt(i);
	aDBSize.SetSize(13);
	for (int k=0; k<13; k++)
	{
		// 현재 단위계로 변환 후 넘김 
		aDBSize[k] = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M,
																														 D_UNITSYS_BASE_LENGTH, pSize->GetAt(k));
	}
	// 치수는 JIS DB 데이터를 MIDAS SECTION DATA FORMAT에 맞게 변경하여 넘겨줌.
	Convert2MidasForamt(aDBSize, aSize);

	return TRUE;
}

void CCMSectItemPSCPlatDB::Convert2MidasForamt(CArray<double, double> &aDBSize, CArray<double, double> &aMSize)
{
	aMSize.RemoveAll();
	aMSize.SetSize(29);
	for (int i=0; i<29; i++) aMSize[i] = 0.0;

	aMSize[ 0]                 = aDBSize[5]+aDBSize[6]+aDBSize[7];        // H1 = F+G+H
	aMSize[ 1]                 = aDBSize[8];                              // H2 = I
	aMSize[ 2] = aMSize[ 2+14] = aDBSize[12]-(aDBSize[2]+aDBSize[3]);     // HOL1 = M-(C+D)
	aMSize[ 3] = aMSize[ 3+14] = aDBSize[2];                              // HOL2 = C
	aMSize[ 4] = aMSize[ 4+14] = aDBSize[3];                              // HOL3 = D
	aMSize[ 5] = aMSize[ 5+14] = aDBSize[0]/2;                            // BOL1 = A/2
	aMSize[ 6] = aMSize[ 6+14] = aDBSize[1];                              // BOL2 = B
	aMSize[ 7] = aMSize[ 7+14] = aDBSize[0]/2+aDBSize[1];                 // BOL3 = A/2+B
	aMSize[10] = aMSize[10+14] = aDBSize[5];                              // HIL1 = F
	aMSize[11] = aMSize[11+14] = aDBSize[7];                              // HIL2 = H
	aMSize[12] = aMSize[12+14] = aDBSize[0]/2-(aDBSize[10]+aDBSize[9]);   // BIL1 = A/2-(K+J)
	aMSize[13] = aMSize[13+14] = aDBSize[9];                              // BIL2 = J
	aMSize[14] = aMSize[14+14] = aDBSize[0]/2-aDBSize[10];                // BIL3 = A/2-K
	// 무조건 0
	//aMSize[29] = aMSize[30]    = aDBSize[0]/2-(aDBSize[10]+aDBSize[11]);  // BIL5 = A/2-(K+L)
}

/////////////////////////////////////////////////////////////////////////////

// *^^* 데이터 추가되면 여기 수정 
void CCMSectItemPSCPlatDB::MakeData()
{
	// DB Name
	CString aDBName[] = { 
		_LS(IDS_CMD_PSC_I_NONE),
		_T("AS12"),_T("AS13"),_T("AS14"),_T("AS15"),_T("AS16"),_T("AS17"),_T("AS18"),_T("AS19"),_T("AS20"),_T("AS21"),_T("AS22"),_T("AS23"),_T("AS24"),
		_T("BS12"),_T("BS13"),_T("BS14"),_T("BS15"),_T("BS16"),_T("BS17"),_T("BS18"),_T("BS19"),_T("BS20"),_T("BS21"),_T("BS22"),_T("BS23"),_T("BS24"),
		_T("1S10"),_T("1S11"),_T("1S12"),_T("1S13"),_T("1S14"),_T("1S15"),_T("1S16"),_T("1S17"),_T("1S18"),_T("1S19"),_T("1S20"),_T("1S21"),
		_T("2S11"),_T("2S12"),_T("2S13"),_T("2S14"),_T("2S15"),_T("2S16"),_T("2S17"),_T("2S18"),_T("2S19"),_T("2S20"),_T("2S21")
	};
	// Size (단위 : mm)
	double aSize[][13] = 
	{
		// A, B, C, D, E, F, G, H, I, J, K, L, M
		{  0,  0,  0,  0,   0,  0,   0,   0,   0,  0,   0,   0,   0},
		{640, 30, 30, 70, 140, 50,  90, 110, 110, 50, 120, 200, 500},
		{640, 30, 30, 70, 140, 50,  90, 110, 110, 50, 120, 200, 500},
		{640, 30, 30, 70, 140, 50, 140, 110, 110, 50, 120, 200, 550},
		{640, 30, 30, 70, 140, 50, 190, 110, 110, 50, 120, 200, 600},
		{640, 30, 30, 70, 140, 50, 240, 110, 110, 50, 120, 200, 650},
		{640, 30, 30, 70, 140, 50, 290, 110, 110, 50, 120, 200, 700},
		{640, 30, 30, 70, 140, 50, 340, 110, 110, 50, 120, 200, 750},
		{640, 30, 30, 70, 160, 50, 370, 110, 110, 50, 120, 200, 800},
		{640, 30, 30, 70, 160, 50, 420, 110, 110, 50, 120, 200, 850},
		{640, 30, 30, 70, 160, 50, 470, 110, 110, 50, 120, 200, 900},
		{640, 30, 30, 70, 180, 50, 470, 110, 140, 50, 120, 200, 950},
		{640, 30, 30, 70, 200, 50, 500, 110, 140, 50, 120, 200,1000},
		{640, 30, 30, 70, 200, 50, 550, 110, 140, 50, 120, 200,1050},
		
		{640, 30, 30, 70, 140, 50, 140, 110, 140, 50, 120, 200, 550},
		{640, 30, 30, 70, 140, 50, 140, 110, 140, 50, 120, 200, 550},
		{640, 30, 30, 70, 140, 50, 190, 110, 140, 50, 120, 200, 600},
		{640, 30, 30, 70, 140, 50, 240, 110, 140, 50, 120, 200, 650},
		{640, 30, 30, 70, 140, 50, 240, 110, 140, 50, 120, 200, 650},
		{640, 30, 30, 70, 140, 50, 290, 110, 140, 50, 120, 200, 700},
		{640, 30, 30, 70, 140, 50, 340, 110, 140, 50, 120, 200, 750},
		{640, 30, 30, 70, 160, 50, 370, 110, 140, 50, 120, 200, 800},
		{640, 30, 30, 70, 160, 50, 420, 110, 140, 50, 120, 200, 850},
		{640, 30, 30, 70, 160, 50, 470, 110, 140, 50, 120, 200, 900},
		{640, 30, 30, 70, 180, 50, 470, 110, 140, 50, 120, 200, 950},
		{640, 30, 30, 70, 200, 50, 500, 110, 140, 50, 120, 200,1000},
		{640, 30, 30, 70, 200, 50, 550, 110, 140, 50, 120, 200,1050},

		{640, 30, 30, 70, 140, 50,  30, 110, 140, 50, 120, 200, 450},
		{640, 30, 30, 70, 140, 50,  55, 110, 140, 50, 120, 200, 475},
		{640, 30, 30, 70, 140, 50,  80, 110, 140, 50, 120, 200, 500},
		{640, 30, 30, 70, 140, 50, 105, 110, 140, 50, 120, 200, 525},
		{640, 30, 30, 70, 140, 50, 130, 110, 140, 50, 120, 200, 550},
		{640, 30, 30, 70, 160, 50, 135, 110, 140, 50, 120, 200, 575},
		{640, 30, 30, 70, 180, 50, 140, 110, 140, 50, 120, 200, 600},
		{640, 30, 30, 70, 210, 50, 135, 110, 140, 50, 120, 200, 625},
		{640, 30, 30, 70, 210, 50, 185, 110, 140, 50, 120, 200, 675},
		{640, 30, 30, 70, 210, 50, 235, 110, 140, 50, 120, 200, 725},
		{640, 30, 30, 70, 210, 50, 285, 110, 140, 50, 120, 200, 775},
		{640, 30, 30, 70, 210, 50, 335, 110, 140, 50, 120, 200, 825},

		{640, 30, 30, 70, 140, 50,  30, 110, 140, 50, 120, 200, 450},
		{640, 30, 30, 70, 140, 50,  55, 110, 140, 50, 120, 200, 475},
		{640, 30, 30, 70, 140, 50,  80, 110, 140, 50, 120, 200, 500},
		{640, 30, 30, 70, 140, 50,  80, 110, 140, 50, 120, 200, 525},
		{640, 30, 30, 70, 160, 50,  85, 110, 140, 50, 120, 200, 550},
		{640, 30, 30, 70, 180, 50,  90, 110, 140, 50, 120, 200, 575},
		{640, 30, 30, 70, 200, 50, 120, 110, 140, 50, 120, 200, 600},
		{640, 30, 30, 70, 200, 50, 145, 110, 140, 50, 120, 200, 625},
		{640, 30, 30, 70, 200, 50, 195, 110, 140, 50, 120, 200, 675},
		{640, 30, 30, 70, 200, 50, 245, 110, 140, 50, 120, 200, 725},
		{640, 30, 30, 70, 200, 50, 295, 110, 140, 50, 120, 200, 775},
	};

	// 치수는 모두 중립단위계로 저장 
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	double dSize;
	CArray<double, double>* pSize;
	int nNum = sizeof(aDBName) / sizeof(CString);
	ASSERT(sizeof(aSize)/(sizeof(double)*13) >= nNum);

	for (int i=0; i<nNum; i++)
	{
		m_aDBNameList.Add(aDBName[i]);

		pSize = new CArray<double, double>;
		pSize->SetSize(13);
		for (int k=0; k<13; k++) 
		{
			// 중립단위계로 단위변환 후 저장 
			dSize = pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, 
																														D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM,
																														D_UNITSYS_BASE_LENGTH, aSize[i][k]);
			pSize->SetAt(k, dSize);
		}
		m_aSize.Add(pSize);
	}
}