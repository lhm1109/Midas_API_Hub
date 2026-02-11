// CMSectItemPSCTeeDB.cpp : implementation file
//
// 2004. 03. 04  by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSCTeeDB.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCMidDB 

CCMSectItemPSCTeeDB::CCMSectItemPSCTeeDB()
{
	m_aDBNameList.RemoveAll();
	m_aSize.RemoveAll();
	//m_aOffset.RemoveAll();

	MakeData();
}

CCMSectItemPSCTeeDB::~CCMSectItemPSCTeeDB()
{
	CArray<double, double>* pData;
	int nNum = m_aSize.GetSize();
	for (int i=0; i<nNum; i++)
	{
		pData = m_aSize.GetAt(i);
		delete pData;
	}
	nNum = m_aSize2.GetSize();
	for (int i = 0; i < nNum; i++)
	{
		pData = m_aSize2.GetAt(i);
		delete pData;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Interface

int  CCMSectItemPSCTeeDB::GetDBNameList(CStringArray& aNameList)
{
	aNameList.Copy(m_aDBNameList);
	return aNameList.GetSize();
}

BOOL CCMSectItemPSCTeeDB::GetDBData(CString &csName, int &nOffset, CArray<double, double> &aSize)
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
	int nLength;
	if(i>=m_aSize.GetSize())
	{
		nLength=31;
		pSize = m_aSize2.GetAt(i-m_aSize.GetSize());
		nOffset = m_aOffset2[i-m_aSize.GetSize()];
	}
	else
	{
		nLength=9;
		pSize = m_aSize.GetAt(i);
		nOffset = m_aOffset[i];
	}
	
	aDBSize.SetSize(nLength);
	for (int k=0; k<nLength; k++)
	{
		// 현재 단위계로 변환 후 넘김 
		aDBSize[k] = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M,
																														 D_UNITSYS_BASE_LENGTH, pSize->GetAt(k));
	}
	// 치수는 JIS DB 데이터를 MIDAS SECTION DATA FORMAT에 맞게 변경하여 넘겨줌.
	if(nLength==9)
	{
		Convert2MidasForamt(aDBSize, aSize);
	}
	else
	{

		aSize.Copy(aDBSize);
	}

	return TRUE;
}

void CCMSectItemPSCTeeDB::Convert2MidasForamt(CArray<double, double> &aDBSize, CArray<double, double> &aMSize)
{
	aMSize.RemoveAll();
	aMSize.SetSize(31);
	for (int i=0; i<31; i++) aMSize[i] = 0.0;

	aMSize[1] = aMSize[1+15] = aDBSize[2];    // HL1 = C
	aMSize[2] = aMSize[2+15] = aDBSize[3];    // HL2 = D
	aMSize[3] = aMSize[3+15] = aDBSize[5];    // HL3 = F
	aMSize[4] = aMSize[4+15] = aDBSize[7]/2;  // BL1 = H/2
	aMSize[5] = aMSize[5+15] = (aDBSize[1]+aDBSize[0]/2)-(aDBSize[7]/2+aDBSize[6]);  // BL2 = (B+A/2)-(H/2+G)
	aMSize[6] = aMSize[6+15] = aDBSize[6];    // BL3 = G
	aMSize[7] = aMSize[7+15] = aDBSize[0]/2;  // BL4 = A/2
}

/////////////////////////////////////////////////////////////////////////////

// *^^* 데이터 추가되면 여기 수정 
void CCMSectItemPSCTeeDB::MakeData()
{
	int aOffset[] = {0,0,0,0,0,0,0,0,0,0,
									 0,0,0,0,0,0,0,0,0,0,
									 0,0,0,0,0,0,0,0,0,0,0
	};
	int aOffset2[] = {0x00000004,0x00000004,0x00000004,0x00000004,0x00000004,0x00000004
	};    

	// DB Name
	CString aDBName[] = { 
		_LS(IDS_CMD_PSC_I_NONE),
		_T("AG18"), _T("AG19"), _T("AG20"), _T("AG21"), _T("AG22"), _T("AG23"), _T("AG24"), 
		_T("BG18"), _T("BG19"), _T("BG20"), _T("BG21"), _T("BG22"), _T("BG23"), _T("BG24"), 
		_T("1B14"), _T("1B15"), _T("1B16"), _T("1B17"), _T("1B18"), _T("1B19"), _T("1B20"), _T("1B21"),
		_T("2B14"), _T("2B15"), _T("2B16"), _T("2B17"), _T("2B18"), _T("2B19"), _T("2B20"), _T("2B21")
	};
		
	CString aDBName2[] = {
			_LS(IDS_CMD_PSC_TEE_CALTRANS_A1_6),_LS(IDS_CMD_PSC_TEE_CALTRANS_A2_0),_LS(IDS_CMD_PSC_TEE_CALTRANS_A2_8),
		_LS(IDS_CMD_PSC_TEE_CALTRANS_B_6),_LS(IDS_CMD_PSC_TEE_CALTRANS_B2_0),_LS(IDS_CMD_PSC_TEE_CALTRANS_B2_8)
	};
	// Size (단위 : mm)
	double aSize[][9] = 
	{
		// A, B, C, D, E, F, G, H I
		{  0,  0,   0,  0, 0,   0,   0,   0,    0},
		{760, 20, 160, 35, 0, 705, 250, 300,  900},
		{760, 20, 160, 35, 0, 805, 250, 300, 1000},
		{760, 20, 160, 35, 0, 805, 250, 300, 1000},
		{760, 20, 160, 35, 0, 905, 250, 300, 1100},
		{760, 20, 160, 35, 0, 905, 250, 300, 1100},
		{760, 20, 160, 35, 0,1005, 250, 300, 1200},
		{760, 20, 160, 35, 0,1005, 250, 300, 1200},
		
		{760, 20, 160, 35, 0, 805, 250, 300, 1000},
		{760, 20, 160, 35, 0, 805, 250, 300, 1000},
		{760, 20, 160, 35, 0, 905, 250, 300, 1100},
		{760, 20, 160, 35, 0, 905, 250, 300, 1100},
		{760, 20, 160, 35, 0,1005, 250, 300, 1200},
		{760, 20, 160, 35, 0,1005, 250, 300, 1200},
		{760, 20, 160, 35, 0,1105, 250, 300, 1300},

		{710, 20, 160, 35, 0, 605, 255, 240,  800},
		{710, 20, 160, 35, 0, 655, 255, 240,  850},
		{710, 20, 160, 35, 0, 705, 255, 240,  900},
		{710, 20, 160, 35, 0, 755, 255, 240,  950},
		{710, 20, 160, 35, 0, 805, 255, 240, 1000},
		{710, 20, 160, 35, 0, 855, 255, 240, 1050},
		{710, 20, 160, 35, 0, 855, 255, 240, 1050},
		{710, 20, 160, 35, 0, 855, 255, 240, 1050},

		{710, 20, 160, 35, 0, 555, 255, 240,  750},
		{710, 20, 160, 35, 0, 605, 255, 240,  800},
		{710, 20, 160, 35, 0, 655, 255, 240,  850},
		{710, 20, 160, 35, 0, 705, 255, 240,  900},
		{710, 20, 160, 35, 0, 755, 255, 240,  950},
		{710, 20, 160, 35, 0, 805, 255, 240, 1000},
		{710, 20, 160, 35, 0, 855, 255, 240, 1050},
		{710, 20, 160, 35, 0, 855, 255, 240, 1050},
	};
	double aSize2[][31] = 
	{ 
		{0, 2, 3, 13, 2, 2, 20, 24, 0, 0, 0, 0, 0, 0, 0, 17, 2, 3, 13, 2, 2, 20, 24, 0, 0, 0, 0, 0, 0, 0, 17},
		{0, 2, 3, 19, 2, 2, 20, 24, 0, 0, 0, 0, 0, 0, 0, 17, 2, 3, 19, 2, 2, 20, 24, 0, 0, 0, 0, 0, 0, 0, 17}, 
		{0, 2, 3, 27, 2, 2, 20, 24, 0, 0, 0, 0, 0, 0, 0, 17, 2, 3, 27, 2, 2, 20, 24, 0, 0, 0, 0, 0, 0, 0, 17}, 
		{0, 2, 3, 13, 3, 2, 19, 24, 0, 0, 0, 0, 0, 0, 0, 16, 2, 3, 13, 3, 2, 19, 24, 0, 0, 0, 0, 0, 0, 0, 16},
		{0, 2, 3, 19, 3, 2, 19, 24, 0, 0, 0, 0, 0, 0, 0, 16, 2, 3, 19, 3, 2, 19, 24, 0, 0, 0, 0, 0, 0, 0, 16},
		{0, 2, 3, 27, 3, 2, 19, 24, 0, 0, 0, 0, 0, 0, 0, 16, 2, 3, 27, 3, 2, 19, 24, 0, 0, 0, 0, 0, 0, 0, 16},
	};


	// 치수는 모두 중립단위계로 저장 
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	double dSize;
	CArray<double, double>* pSize;
	int nNum = sizeof(aDBName) / sizeof(CString);
	ASSERT(sizeof(aSize)/(sizeof(double)*9) >= nNum);
	int i = 0;
	for (i=0; i<nNum; i++)
	{
		m_aDBNameList.Add(aDBName[i]);

		pSize = new CArray<double, double>;
		pSize->SetSize(9);
		for (int k=0; k<9; k++) 
		{
			// 중립단위계로 단위변환 후 저장 
			dSize = pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, 
																														D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM,
																														D_UNITSYS_BASE_LENGTH, aSize[i][k]);
			pSize->SetAt(k, dSize);
		}
		m_aSize.Add(pSize);
		m_aOffset.Add(aOffset[i]); 
	}

	double dSize2;
	CArray<double, double>* pSize2;
	int nNum2 = sizeof(aDBName2) / sizeof(CString);
	ASSERT(sizeof(aSize2)/(sizeof(double)*31) >= nNum2);

	for (i=0; i<nNum2; i++)
	{
		m_aDBNameList.Add(aDBName2[i]);

		pSize2 = new CArray<double, double>;
		pSize2->SetSize(31);
		for (int k=0; k<31; k++) 
		{
			// 중립단위계로 단위변환 후 저장 
			dSize2 = pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, 
																														D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_IN,
																														D_UNITSYS_BASE_LENGTH, aSize2[i][k]);
			pSize2->SetAt(k, dSize2);
		}
		m_aSize2.Add(pSize2);
		m_aOffset2.Add(aOffset2[i]); 
	}
}