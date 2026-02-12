//////////
// StldBtmpDefCodeProvUtil.cpp : implementation file
// 
#include "stdafx.h"

#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\Postctrl.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\DataCtrl.h"
#include "..\wg_db\SectUtil.h"
#include "..\wg_db\InitCtrl.h"
#include "..\wg_db\SectDB.h"

#include "StldBtmpDefCodeProvUtil.h"

#include <algorithm>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CStldBtmpDefCodeProvUtil::CStldBtmpDefCodeProvUtil()
{

}

BOOL CStldBtmpDefCodeProvUtil::CheckSectType(BTMP_DEF_CODE_PROV_D& InputData, CArray<UINT, UINT>& aElemKey)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	if (aElemKey.GetSize() == 0)
	{
		AfxMessageBox(_LS(IDS_NO_SEL_FRAME_ELEMENT));
		return FALSE;
	}   

	int nStype;
	T_ELEM_D ElemD;
	T_SECT_D SectD;
	BOOL bFindValid = FALSE;
	for(int i = 0; i < aElemKey.GetSize(); i++)
	{
		pDoc->m_pAttrCtrl->GetElem(aElemKey[i], ElemD);
		pDoc->m_pAttrCtrl->GetSect(ElemD.elpro ,SectD);
		
		if(SectD.nStype==D_SECT_TYPE_TAPERED)
		{
			nStype = SectD.SectBefore.nStype;
		}
		else
		{
			nStype = SectD.nStype;
		}

		if(InputData.nDgnCode == D_BTMP_CODE_SNIP_2050384 || InputData.nDgnCode == D_BTMP_CODE_SP_35_13330_2011) // Snip 2.05.03-84
		{
			if(InputData.Snip2050284.nSectType == 0) // Steel Composite
			{
				if(nStype == D_SECT_TYPE_COMPO_B || nStype == D_SECT_TYPE_COMPO_I || nStype == D_SECT_TYPE_COMPO_TUB || 
					nStype == D_SECT_TYPE_COMPO_STLG_B || nStype == D_SECT_TYPE_COMPO_STLG_I || nStype == D_SECT_TYPE_COMPO_STLG_TUB)
				{
					bFindValid = TRUE;
					break;
				}
			}
			else
			{
				ASSERT(0);
				return FALSE;
			}
		}
		else if(InputData.nDgnCode == D_BTMP_CODE_JTG_D60_2015)
		{
			if (InputData.Jtg2015_2018.nSectType == 2) // SRC Dumbbell
			{
				if (nStype != D_SECT_TYPE_SRC) 
					continue;

				if (InputData.Jtg2015_2018.nDumbType == 0)
				{
					if(SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_STUB)
						bFindValid = TRUE;
				}
				else if (InputData.Jtg2015_2018.nDumbType == 1)
				{
					if (SectD.SectBefore.Shape == D_SECT_SHAPE_SRC_DUMB)
						bFindValid = TRUE;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				return TRUE; // JTG 는 딱히 단면 type 에서 에러를 걸 필요 없음
			}
		}
        else if(InputData.nDgnCode == D_BTMP_CODE_IRC6_2017) // IRC 6 2017
        {
            return TRUE; // JTG 는 딱히 단면 type 에서 에러를 걸 필요 없음
        }
		else if(InputData.nDgnCode == D_BTMP_CODE_AASHTO_LRFD_2020)
		{
			return TRUE;
		}
		else if(InputData.nDgnCode == D_BTMP_CODE_EUROCODE)
		{
			return TRUE;
		}
		else if(InputData.nDgnCode == D_BTMP_CODE_AS)
		{
			return TRUE;
		}
		else
		{
			ASSERT(0);
			return FALSE;
		}
	}

	if(!bFindValid)
	{
		AfxMessageBox(_LS(IDS_DEF_CODE_PROV_SECT_TYPE_ERROR));
		return FALSE;
	}

	return TRUE;
}

BOOL CStldBtmpDefCodeProvUtil::MakeStmpByCode( BTMP_DEF_CODE_PROV_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, T_BTMP_D& BtmpD, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg )
{  
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_SECT_D ConsiderTaperedSectD;
	if(SectD.nStype==D_SECT_TYPE_TAPERED)
	{
		double dElemLen = pDoc->calcLAVElem(ElemK);
		T_TSGR_D TsgrD; TsgrD.Initialize();
		TsgrD.GroupName.Format(_T("TSGR"));
		TsgrD.nVariation_z = 0;   // linear
		TsgrD.nQuadEnd_z   = 0;   // i절점 대칭
		TsgrD.aElemList.Add(ElemK);
		
		if(!pDoc->m_pSectDB->CalcTaperedSection(SectD, TsgrD, dElemLen, dElemLen/2.0, dElemLen, 0, ConsiderTaperedSectD))
		{
			ASSERT(0);
			return -1;
		}
	}
	else
	{
		ConsiderTaperedSectD = SectD;
	}

	// GenStmp는 Top기준으로 Data를 만듬
	switch (InputData.nDgnCode)
	{
	case D_BTMP_CODE_SNIP_2050384:
	case D_BTMP_CODE_SP_35_13330_2011:		
		ASSERT(InputData.Snip2050284.nSectType == 0);
		return MakeStmpSnip2050384Compo(InputData.Snip2050284, ElemK, ConsiderTaperedSectD, BtmpD.nType, GenStmp, aErrorMsg);
		break;
	case D_BTMP_CODE_JTG_D60_2015:		
		return MakeStmpJTGD602015(InputData.Jtg2015_2018, ElemK, ConsiderTaperedSectD, BtmpD.nType, GenStmp, aErrorMsg);
		break;
    case D_BTMP_CODE_IRC6_2017:
        return MakeStmpIRC62017(InputData.IRC6_2017, ElemK, ConsiderTaperedSectD, BtmpD.nType, GenStmp, aErrorMsg);        
        break;
	case D_BTMP_CODE_AASHTO_LRFD_2020:
		return MakeStmpAASHTO2020(InputData.AASHTO_2020, ElemK, ConsiderTaperedSectD, BtmpD.nType, GenStmp, aErrorMsg);        
		break;
	case D_BTMP_CODE_EUROCODE:
		return MakeStmpEurocode(InputData.EuroCode, ElemK, ConsiderTaperedSectD, BtmpD.nType, GenStmp, aErrorMsg);        
		break;
	case D_BTMP_CODE_AS:
		return MakeStmpAs(InputData.AsCode, ElemK, ConsiderTaperedSectD, BtmpD.nType, GenStmp, aErrorMsg);        
		break;
	default:
		break;
	}	

	return TRUE;
}

BOOL CStldBtmpDefCodeProvUtil::MakeBtmpByCode( T_BTMP_D& BtmpD, BTMP_DEF_CODE_PROV_D& InputData, CArray<UINT, UINT>& aElemKey, CArray<T_BTMP_D, T_BTMP_D&>& aGenBtmp, CArray<CString>& aErrorMsg )
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_ELEM_D ElemD;
	T_SECT_D SectD;

	T_BTMP_D BtmpTempD;

	aGenBtmp.RemoveAll();
	for(int i = 0; i < aElemKey.GetSize(); i++)
	{
		pDoc->m_pAttrCtrl->GetElem(aElemKey[i], ElemD);
		pDoc->m_pAttrCtrl->GetSect(ElemD.elpro ,SectD);

		BtmpTempD = BtmpD;
		BtmpTempD.nDirection = 1;
		BtmpTempD.nRefPosition = 1;

		if(!MakeStmpByCode(InputData, aElemKey[i], SectD, BtmpD, BtmpTempD.SectTmp, aErrorMsg))
		{
			aElemKey.RemoveAt(i);
			i--;
			continue;
		}
		aGenBtmp.Add(BtmpTempD);
	}
	return TRUE;
}

int CStldBtmpDefCodeProvUtil::GetBestDivNumSnip2050384Compo(double dHw, double dBestSize, int nMaxSize)
{
	if(dHw / dBestSize >= nMaxSize) return nMaxSize;

	int nBestDivNum = 1;
	double dBestDif = DBL_MAX;
	for(int i = 1; i < nMaxSize; i++)
	{
		double dDif = fabs((dHw / (double)i) - dBestSize);
		if(dBestDif > dDif)
		{
			dBestDif = dDif;
			nBestDivNum = i;
		}
	}
	return nBestDivNum;
}

BOOL CStldBtmpDefCodeProvUtil::MakeStmpSnip2050384Compo( BTMP_DEF_CODE_PROV_SNIP2050284_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();

	GenStmp.RemoveAll();
	int nStype;

	if(SectD.nStype==D_SECT_TYPE_TAPERED)
	{
		nStype = SectD.SectBefore.nStype;
	}
	else
	{
		nStype = SectD.nStype;
	}

	if(nStype != D_SECT_TYPE_COMPO_B && nStype != D_SECT_TYPE_COMPO_I && nStype != D_SECT_TYPE_COMPO_TUB && 
		nStype != D_SECT_TYPE_COMPO_STLG_B && nStype != D_SECT_TYPE_COMPO_STLG_I && nStype != D_SECT_TYPE_COMPO_STLG_TUB)
	{
		CString strTemp;
		strTemp.Format(_LS(IDS_DB_WARNING_BTMP_IRG_PSC_COMPO_SECT), ElemK);
		aErrorMsg.Add(strTemp);
		//ASSERT(0);
		return FALSE;
	}

	// Tapered일때는 이전에 중간단면으로 넘겨줬기 때문에 무조건 I단으로 계산하면 된다.
	double dTc = SectD.SectAfter.SectJ.Size[1];
	double dHh = SectD.SectAfter.SectJ.Size[2];

	double dTopFlange;
	double dBotFlange;
	switch (nStype)
	{
	case D_SECT_TYPE_COMPO_B:
		dTopFlange = SectD.SectBefore.SectI.Size[4]; 
		dBotFlange = SectD.SectBefore.SectI.Size[7]; 
		break;
	case D_SECT_TYPE_COMPO_I:
		dTopFlange = SectD.SectBefore.SectI.Size[3]; 
		dBotFlange = SectD.SectBefore.SectI.Size[5]; 
		break;
	case D_SECT_TYPE_COMPO_TUB:
		dTopFlange = SectD.SectBefore.SectI.Size[4]; 
		dBotFlange = SectD.SectBefore.SectI.Size[7]; 
		break;
	case D_SECT_TYPE_COMPO_STLG_B:
		dTopFlange = SectD.SectBefore.SectI.Size[7]; 
		dBotFlange = SectD.SectBefore.SectI.Size[8];
		break;
	case D_SECT_TYPE_COMPO_STLG_I:
		dTopFlange = SectD.SectBefore.SectI.Size[5]; 
		dBotFlange = SectD.SectBefore.SectI.Size[6];
		break;
	case D_SECT_TYPE_COMPO_STLG_TUB:
		dTopFlange = SectD.SectBefore.SectI.Size[7]; 
		dBotFlange = SectD.SectBefore.SectI.Size[8];
		break;
	default:
		ASSERT(0);
	}

	double dDimhi, dDimbi, dDimhj, dDimbj;
	if(!pDoc->m_pAttrCtrl->GetSectDimen(SectD, dDimhi, dDimbi, dDimhj, dDimbj, NULL, FALSE)) return FALSE;
	
	CArray<double, double> aGenTmp;
	CArray<double, double> aGenTmpPos;
	CArray<double, double> aH;
	CArray<double, double> aZ, aZDivBySect;
	if(InputData.nTempGrad == 0)
	{
		// Girder부분에만 들어감 (상부Flange와Web까지는 수식으로 들어가고, 하부Flange는 0.3이 들어 감)
		double d10Cm = M_InitValueCurUnit(10.0, N, CM, D_UNITSYS_BASE_LENGTH);// 쪼개는 간격이 10cm에 최대한 가깝게 해달라고 했음
		double dHw = dDimhi - dTc - dHh - dTopFlange - dBotFlange;
		int nDivNum = GetBestDivNumSnip2050384Compo(dHw, d10Cm, D_BTMP_NUMSTMP - 1);
		double dDivVal = dHw / nDivNum;    
		double dZ = 0;
		double dEachTmp = 0.0;

		aGenTmp.Add(dEachTmp);
		aGenTmpPos.Add(dTc + dHh + dTopFlange + dZ);

		for(int i = 0; i < nDivNum - 1; i++)
		{
			dZ += dDivVal;
			
			FuncSnip2050384CompoType1(InputData.dTMax, dZ, dHw, dEachTmp);
			aGenTmp.Add(dEachTmp);
			aGenTmpPos.Add(dTc + dHh + dTopFlange + dZ);
		}

		aGenTmp.Add(0.3 * InputData.dTMax);
		aGenTmpPos.Add(dDimhi - dBotFlange);

		aGenTmp.Add(0.3 * InputData.dTMax);
		aGenTmpPos.Add(dDimhi);

		ASSERT(aGenTmp.GetSize() == nDivNum + 2);
	}
	else if(InputData.nTempGrad == 1)
	{
		// 상부부터 50cm가 입력됨
		double d50Cm = M_InitValueCurUnit(50.0, N, CM, D_UNITSYS_BASE_LENGTH);
		
		double dDivVal = d50Cm / 10.0; // 5cm단위로 쪼개기로 협의함
		double dZ = 0;
		double dEachTmp;

		// Section모양에 따른 분할점을 추가 함..
		aZ.Add(0.0);
		for(int i = 0; i < 9; i++)
		{
			dZ += dDivVal;
			aZ.Add(dZ);
		}
		aZ.Add(d50Cm);

		for(int i = 0; i < aZ.GetSize() - 1; i++)
		{    
			aH.RemoveAll();
			CSectUtil::GetPscCompoBtmpHDist(SectD, aZ[i], aZ[i + 1], aH, FALSE);
			for(int j = 0; j < aH.GetSize() - 1; j++) // 제일 끝은 마지막에만 넣어줘야 중복이 안됨
			{
				aZDivBySect.Add(aH[j]);
			}
		}
		aZDivBySect.Add(aZ[aZ.GetSize() - 1]);

		//H값 계산
		for(int i = 0; i < aZDivBySect.GetSize() - 1; i++)
		{
			if(dDimhi > aZDivBySect[i] || fabs(dDimhi - aZDivBySect[i]) < dZero)
			{
				FuncSnip2050384CompoType2(InputData.dTMax, aZDivBySect[i], dEachTmp);
				aGenTmp.Add(dEachTmp);
				aGenTmpPos.Add(aZDivBySect[i]);
			}
		}
		if(dDimhi > d50Cm || fabs(dDimhi - d50Cm) < dZero)
		{
			aGenTmp.Add(0.0);
			aGenTmpPos.Add(d50Cm);
		}
	}
	else
	{
		ASSERT(0);
		return FALSE;
	}
	
	ASSERT(aGenTmp.GetSize() == aGenTmpPos.GetSize());  
	
	GenStmp.SetSize(aGenTmp.GetSize() - 1);
	for(int i = 0; i < aGenTmp.GetSize() - 1; i++)
	{
		GenStmp[i].Initialize();
		
		GenStmp[i].nMaterial = 1;
		GenStmp[i].nRef = 0;
		GenStmp[i].nB = 0;
		GenStmp[i].nH1 = 3;
		GenStmp[i].nH2 = 3;

		GenStmp[i].dH1 = aGenTmpPos[i];
		GenStmp[i].dT1 = aGenTmp[i];
		GenStmp[i].dH2 = aGenTmpPos[i + 1];
		GenStmp[i].dT2 = aGenTmp[i + 1];
	}

	if(nBtmpType == 0) // GeneralType일때 nB값은 고려하지 않고 dB값으로 쓰기 때문에 dB값을 계산해 준다.
	{
		std::vector<std::pair<double, double>> aPairH;
		CArray<int, int> aPartNo;
		CArray<double, double> aB;
		if(!CSectUtil::GetCompoBtmpB(SectD, dDimhi, aGenTmpPos, aPairH, aB, aPartNo, FALSE))
		{
			ASSERT(0);
			return FALSE;
		}
		ASSERT(GenStmp.GetSize() + 1 == aB.GetSize());

		for(int i = 0; i < GenStmp.GetSize(); i++)
		{
			GenStmp[i].dB = aB[i];
		}
	}

	if(InputData.nTempGrad == 0)
	{
		double dTemp;
		// Type1번일 때는 Bot기준입력으로 바꿔준다.
		double dHMax = GenStmp[GenStmp.GetSize() - 1].dH2;
		// 뒤집은다음.. dH와 dT값을 Swap한다음 H에서 dHMax를 빼주면 됨..
		std::reverse(&GenStmp[0],&GenStmp[0]+GenStmp.GetSize());
		for(int i = 0; i < GenStmp.GetSize(); i++)
		{
			dTemp = GenStmp[i].dH1;
			GenStmp[i].dH1 = GenStmp[i].dH2;
			GenStmp[i].dH2 = dTemp;
			
			dTemp = GenStmp[i].dT1;
			GenStmp[i].dT1 = GenStmp[i].dT2;
			GenStmp[i].dT2 = dTemp;

			GenStmp[i].dH1 = dHMax - GenStmp[i].dH1;
			GenStmp[i].dH2 = dHMax - GenStmp[i].dH2;

			GenStmp[i].nRef = 1;
		}
	}


	T_BTMP_STMP BlankBtmp;
	BlankBtmp.Initialize();
	BlankBtmp.nMaterial = 0;

	int nAddNum = D_BTMP_NUMSTMP - GenStmp.GetSize();
	for(int i = 0; i < nAddNum; i++)
	{
		GenStmp.Add(BlankBtmp);
	}

	return TRUE;
}

BOOL CStldBtmpDefCodeProvUtil::MakeStmpJTGD602015(BTMP_DEF_CODE_PROV_JTG_2015_2018_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg)
{
	if(InputData.nSectType == 0)
	{
		return MakeStmpJTGD602015PSC(InputData, ElemK, SectD, nBtmpType, GenStmp, aErrorMsg);
	}
	else if(InputData.nSectType == 1) 
	{
		return MakeStmpJTGD602015Stlg(InputData, ElemK, SectD, nBtmpType, GenStmp, aErrorMsg);
	}
	else if (InputData.nSectType == 2)
	{
		if (InputData.nDumbType == 0)
			return MakeStmpJTGD602015Stub(InputData, ElemK, SectD, nBtmpType, GenStmp, aErrorMsg);
		else if (InputData.nDumbType == 1)
			return MakeStmpJTGD602015Dumb(InputData, ElemK, SectD, nBtmpType, GenStmp, aErrorMsg);
		else
			ASSERT(0);
	}
	ASSERT(0);
	return FALSE;
}

BOOL CStldBtmpDefCodeProvUtil::MakeStmpJTGD602015PSC(BTMP_DEF_CODE_PROV_JTG_2015_2018_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();
	CString strTemp;

	GenStmp.RemoveAll();
	int nStype;

	if(SectD.nStype==D_SECT_TYPE_TAPERED)
	{
		nStype = SectD.SectBefore.nStype;
	}
	else
	{
		nStype = SectD.nStype;
	}

	double dDimhi, dDimbi, dDimhj, dDimbj;
	if(!pDoc->m_pAttrCtrl->GetSectDimen(SectD, dDimhi, dDimbi, dDimhj, dDimbj, NULL, FALSE)) return FALSE;

	CArray<double, double> aGenTmp;
	CArray<double, double> aGenTmpPos;
	CArray<double, double> aH;
	CArray<double, double> aZ, aZDivBySect;

	double d100mm = M_InitValueCurUnit(100.0, N, MM, D_UNITSYS_BASE_LENGTH);	
	double d400mm = M_InitValueCurUnit(400.0, N, MM, D_UNITSYS_BASE_LENGTH);	

	if(dDimhi < d100mm)
	{		
		strTemp.Format(_LS(IDS_DB_WARNING_BTMP_JTG_H_SHORT), ElemK);
		aErrorMsg.Add(strTemp);
		return FALSE;
	}
	else if(dDimhi < d400mm)
	{
		aGenTmpPos.Add(0.0);
		aGenTmpPos.Add(d100mm);
		aGenTmpPos.Add(dDimhi);

		aGenTmp.Add(InputData.dT1);
		aGenTmp.Add(InputData.dT2);
		aGenTmp.Add(0.0);
	}
	else
	{
		aGenTmpPos.Add(0.0);
		aGenTmpPos.Add(d100mm);
		aGenTmpPos.Add(d400mm);
		aGenTmpPos.Add(dDimhi);

		aGenTmp.Add(InputData.dT1);
		aGenTmp.Add(InputData.dT2);
		aGenTmp.Add(0.0);
		aGenTmp.Add(0.0);
	}
	
	ASSERT(aGenTmp.GetSize() == aGenTmpPos.GetSize());  

	GenStmp.SetSize(aGenTmp.GetSize() - 1);
	for(int i = 0; i < aGenTmp.GetSize() - 1; i++)
	{
		GenStmp[i].Initialize();

		GenStmp[i].nMaterial = 1;
		GenStmp[i].nRef = 0;
		GenStmp[i].nB = 0;
		GenStmp[i].nH1 = 3;
		GenStmp[i].nH2 = 3;

		GenStmp[i].dH1 = aGenTmpPos[i];
		GenStmp[i].dT1 = aGenTmp[i];
		GenStmp[i].dH2 = aGenTmpPos[i + 1];
		GenStmp[i].dT2 = aGenTmp[i + 1];
	}

	T_BTMP_STMP BlankBtmp;
	BlankBtmp.Initialize();
	BlankBtmp.nMaterial = 0;

	int nAddNum = D_BTMP_NUMSTMP - GenStmp.GetSize();
	for(int i = 0; i < nAddNum; i++)
	{
		GenStmp.Add(BlankBtmp);
	}

	return TRUE;
}

BOOL CStldBtmpDefCodeProvUtil::MakeStmpJTGD602015Stlg(BTMP_DEF_CODE_PROV_JTG_2015_2018_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();

	GenStmp.RemoveAll();
	int nStype;

	if(SectD.nStype==D_SECT_TYPE_TAPERED)
	{
		nStype = SectD.SectBefore.nStype;
	}
	else
	{
		nStype = SectD.nStype;
	}

	double dDimhi, dDimbi, dDimhj, dDimbj;
	if(!pDoc->m_pAttrCtrl->GetSectDimen(SectD, dDimhi, dDimbi, dDimhj, dDimbj, NULL, FALSE)) return FALSE;

	double d100mmCurUnit = M_InitValueCurUnit(100.0, N, MM, D_UNITSYS_BASE_LENGTH);	
	double d300mmCurUnit = M_InitValueCurUnit(300.0, N, MM, D_UNITSYS_BASE_LENGTH);
	double dSmallTCurUnit = M_InitValueCurUnit(InputData.dSmallT, N, MM, D_UNITSYS_BASE_LENGTH);

	if(dSmallTCurUnit < -dZero)
	{
		aErrorMsg.Add(_LS(IDS_ERR_BTMP_CH_SMALL_T));
		return FALSE;
	}
	else if(dSmallTCurUnit > d100mmCurUnit + d300mmCurUnit + dZero)
	{
		aErrorMsg.Add(_LS(IDS_ERR_BTMP_CH_SMALL_T));
		return FALSE;
	}

	CArray<double, double> aGenTmp;
	CArray<double, double> aGenTmpPos;
	CArray<double, double> aH;
	CArray<double, double> aZ, aZDivBySect;
	if(fabs(dSmallTCurUnit) < dZero)
	{
		aGenTmp.Add(InputData.dT1);
		aGenTmp.Add(InputData.dT1);

		aGenTmpPos.Add(0.0);
		aGenTmpPos.Add(dDimhi);
	}
	else if(fabs(dSmallTCurUnit - InputData.dT2) < dZero)
	{
		aGenTmp.Add(InputData.dT1);
		aGenTmp.Add(InputData.dT2);
		aGenTmp.Add(InputData.dT2);

		aGenTmpPos.Add(0.0);
		aGenTmpPos.Add(d100mmCurUnit);
		aGenTmpPos.Add(dDimhi);
	}
	else if(fabs(dSmallTCurUnit - d100mmCurUnit - d300mmCurUnit) < dZero)
	{
		aGenTmp.Add(InputData.dT1);
		aGenTmp.Add(InputData.dT2);
		aGenTmp.Add(0.0);

		aGenTmpPos.Add(0.0);
		aGenTmpPos.Add(d100mmCurUnit);
		aGenTmpPos.Add(d100mmCurUnit + d300mmCurUnit);
	}
	else if(dSmallTCurUnit < d100mmCurUnit)
	{
		double dDiv;
		double dSmallTCalc, dStartT;
		
		// 0 ~ 100mm 영역에 포함
		dDiv = (InputData.dT2 - InputData.dT1) / d100mmCurUnit * -1;
		dSmallTCalc = dSmallTCurUnit;
		dStartT = InputData.dT1;
		
		double dMidT, dMidPos;
		dMidT = dStartT + dDiv * dSmallTCalc;
		dMidPos = dSmallTCurUnit;
		
		aGenTmp.Add(InputData.dT1);
		aGenTmp.Add(dMidT);
		aGenTmp.Add(dMidT);

		aGenTmpPos.Add(0.0);
		aGenTmpPos.Add(dMidPos);
		aGenTmpPos.Add(dDimhi);
	}
	else
	{
		double dDiv;
		double dSmallTCalc, dStartT, dStartPos;

		// 100mm ~ 400mm 영역에 포함
		dDiv = InputData.dT2 / d300mmCurUnit * -1;
		dSmallTCalc = dSmallTCurUnit - d100mmCurUnit;
		dStartT = InputData.dT2;
		dStartPos = d100mmCurUnit;

		double dMidT, dMidPos;
		dMidT = dStartT + dDiv * dSmallTCalc;
		dMidPos = dSmallTCurUnit;

		aGenTmp.Add(InputData.dT1);		
		aGenTmp.Add(InputData.dT2);
		aGenTmp.Add(dMidT);
		aGenTmp.Add(dMidT);

		aGenTmpPos.Add(0.0);		
		aGenTmpPos.Add(d100mmCurUnit);
		aGenTmpPos.Add(dMidPos);
		aGenTmpPos.Add(dDimhi);
	}
	ASSERT(aGenTmp.GetSize() == aGenTmpPos.GetSize());  

	GenStmp.SetSize(aGenTmp.GetSize() - 1);
	for(int i = 0; i < aGenTmp.GetSize() - 1; i++)
	{
		GenStmp[i].Initialize();

		GenStmp[i].nMaterial = 1;
		GenStmp[i].nRef = 0;
		GenStmp[i].nB = 0;
		GenStmp[i].nH1 = 3;
		GenStmp[i].nH2 = 3;

		GenStmp[i].dH1 = aGenTmpPos[i];
		GenStmp[i].dT1 = aGenTmp[i];
		GenStmp[i].dH2 = aGenTmpPos[i + 1];
		GenStmp[i].dT2 = aGenTmp[i + 1];
	}

	T_BTMP_STMP BlankBtmp;
	BlankBtmp.Initialize();
	BlankBtmp.nMaterial = 0;

	int nAddNum = D_BTMP_NUMSTMP - GenStmp.GetSize();
	for(int i = 0; i < nAddNum; i++)
	{
		GenStmp.Add(BlankBtmp);
	}

	return TRUE;
}

BOOL CStldBtmpDefCodeProvUtil::MakeStmpJTGD602015Dumb(BTMP_DEF_CODE_PROV_JTG_2015_2018_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();

	GenStmp.RemoveAll();
	int nStype;

	if (SectD.nStype == D_SECT_TYPE_TAPERED)
	{
		nStype = SectD.SectBefore.nStype;
	}
	else
	{
		nStype = SectD.nStype;
	}

	double dDimhi, dDimbi, dDimhj, dDimbj;
	if (!pDoc->m_pAttrCtrl->GetSectDimen(SectD, dDimhi, dDimbi, dDimhj, dDimbj, NULL, FALSE)) return FALSE;

	double D1 = SectD.SectBefore.SectI.Size[2];
	double D2 = SectD.SectBefore.SectI.Size[3];

	CArray<double, double> aGenTmp;
	CArray<double, double> aGenTmpPos;

	aGenTmp.Add(InputData.dT1_Dumb);
	aGenTmp.Add(InputData.dT1_Dumb);
// 	aGenTmp.Add(InputData.dT2_Dumb);
// 	aGenTmp.Add(InputData.dT2_Dumb);

	aGenTmpPos.Add(0.0);
	aGenTmpPos.Add(D1);
// 	aGenTmpPos.Add(D1);
// 	aGenTmpPos.Add(dDimhi);

	//////////////////////////////////////////////////////////////////////////
	ASSERT(aGenTmp.GetSize() == aGenTmpPos.GetSize());

	GenStmp.SetSize(aGenTmp.GetSize() - 1);
	for (int i = 0; i < aGenTmp.GetSize() - 1; i++)
	{
		GenStmp[i].Initialize();

		GenStmp[i].nMaterial = 1;
		GenStmp[i].nRef = 0;
		GenStmp[i].nB = 0;
		GenStmp[i].nH1 = 3;
		GenStmp[i].nH2 = 3;

		GenStmp[i].dH1 = aGenTmpPos[i];
		GenStmp[i].dT1 = aGenTmp[i];
		GenStmp[i].dH2 = aGenTmpPos[i + 1];
		GenStmp[i].dT2 = aGenTmp[i + 1];
	}

	T_BTMP_STMP BlankBtmp;
	BlankBtmp.Initialize();
	BlankBtmp.nMaterial = 0;

	int nAddNum = D_BTMP_NUMSTMP - GenStmp.GetSize();
	for (int i = 0; i < nAddNum; i++)
	{
		GenStmp.Add(BlankBtmp);
	}

	return TRUE;
}

BOOL CStldBtmpDefCodeProvUtil::MakeStmpJTGD602015Stub(BTMP_DEF_CODE_PROV_JTG_2015_2018_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();

	GenStmp.RemoveAll();
	int nStype;

	if (SectD.nStype == D_SECT_TYPE_TAPERED)
	{
		nStype = SectD.SectBefore.nStype;
	}
	else
	{
		nStype = SectD.nStype;
	}

	double dDimhi, dDimbi, dDimhj, dDimbj;
	if (!pDoc->m_pAttrCtrl->GetSectDimen(SectD, dDimhi, dDimbi, dDimhj, dDimbj, NULL, FALSE)) return FALSE;

	double dD4 = dDimhi / 4.0;

	CArray<double, double> aGenTmp;
	CArray<double, double> aGenTmpPos;

	aGenTmp.Add(InputData.dT1_Dumb);
	aGenTmp.Add(0.0);
	aGenTmp.Add(0.0);
	aGenTmp.Add(InputData.dT2_Dumb);

	aGenTmpPos.Add(0.0);
	aGenTmpPos.Add(dD4);
	aGenTmpPos.Add(dDimhi- dD4);
	aGenTmpPos.Add(dDimhi);

	//////////////////////////////////////////////////////////////////////////
	ASSERT(aGenTmp.GetSize() == aGenTmpPos.GetSize());
	GenStmp.SetSize(aGenTmp.GetSize() - 1);

	for (int i = 0; i < aGenTmp.GetSize() - 1; i++)
	{
		GenStmp[i].Initialize();

		GenStmp[i].nMaterial = 1;
		GenStmp[i].nRef = 0;
		GenStmp[i].nB = 0;
		GenStmp[i].nH1 = 3;
		GenStmp[i].nH2 = 3;

		GenStmp[i].dH1 = aGenTmpPos[i];
		GenStmp[i].dT1 = aGenTmp[i];
		GenStmp[i].dH2 = aGenTmpPos[i + 1];
		GenStmp[i].dT2 = aGenTmp[i + 1];
	}

	// 2번째는 온도가 0이어서 skip
	GenStmp[1] = GenStmp[2];
	GenStmp[2].Initialize();

	T_BTMP_STMP BlankBtmp;
	BlankBtmp.Initialize();
	BlankBtmp.nMaterial = 0;

	int nAddNum = D_BTMP_NUMSTMP - GenStmp.GetSize();
	for (int i = 0; i < nAddNum; i++)
	{
		GenStmp.Add(BlankBtmp);
	}

	return TRUE;
}

BOOL CStldBtmpDefCodeProvUtil::FuncSnip2050384CompoType1(double dTmax, double dZ, double dHw, double& dOut)
{
	dOut = dTmax * sqrt(3.91 * dZ / dHw - 3.82 * pow(dZ / dHw , 2));

	return TRUE;
}

BOOL CStldBtmpDefCodeProvUtil::FuncSnip2050384CompoType2(double dTmax, double dZ, double& dOut)
{
	double d50Cm = M_InitValueCurUnit(50.0, N, CM, D_UNITSYS_BASE_LENGTH);;
	dOut = dTmax * pow(dZ / d50Cm - 1.0, 2);

	return TRUE;
}

BOOL CStldBtmpDefCodeProvUtil::AddBtmpByCodeProv(T_BTMP_D& BtmpD, BTMP_DEF_CODE_PROV_D& InputData, CArray<UINT, UINT>& aElemKey, BOOL bReplace )
{
	if(!CheckSectType(InputData, aElemKey))
	{
		return FALSE;
	}

	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CArray<UINT, UINT> aElemKeyCopy; // MakeBtmpByCode 에서 Type에 안맞는 ElemKey는 삭제하기 때문에 복사본을 넘김
	aElemKeyCopy.Copy(aElemKey);

	CArray<T_BTMP_D, T_BTMP_D&> aGenBtmp;
	CArray<CString> aErrorMsg;
	if(!MakeBtmpByCode(BtmpD, InputData, aElemKeyCopy, aGenBtmp, aErrorMsg))
	{
		return FALSE;
	}

	ASSERT(aElemKeyCopy.GetSize() == aGenBtmp.GetSize());
	if(aGenBtmp.GetSize() == 0)
	{
		for (int i = 0; i < aErrorMsg.GetSize(); i++)
		{
			pDoc->DisplayHistoryMessage(aErrorMsg[i]);
		}
		return FALSE;
	}

	BOOL bReturnVal = pDoc->m_pDataCtrl->AddBtmp(aElemKeyCopy, aGenBtmp,  bReplace);

	for (int i = 0; i < aErrorMsg.GetSize(); i++)
	{
		pDoc->DisplayHistoryMessage(aErrorMsg[i]); // 트렌젝션을 여는순간 Error Message가 다 지워지기 떄문에 저장해뒀다가 마지막에 출력한다.
	}

	return bReturnVal;
}

BOOL CStldBtmpDefCodeProvUtil::MakeStmpIRC62017(BTMP_DEF_CODE_PROV_IRC6_2017_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg)
{
    if(InputData.nSectType == 0)
    {
        return MakeStmpIRC62017PSC(InputData, ElemK, SectD, nBtmpType, GenStmp, aErrorMsg);
    }
    else if(InputData.nSectType == 1) 
    {
        return MakeStmpIRC62017Stlg(InputData, ElemK, SectD, nBtmpType, GenStmp, aErrorMsg);
    }
    ASSERT(0);
    return FALSE;
}

BOOL CStldBtmpDefCodeProvUtil::MakeStmpIRC62017PSC(BTMP_DEF_CODE_PROV_IRC6_2017_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();
    CString strTemp;

    GenStmp.RemoveAll();
    int nStype;

    if(SectD.nStype==D_SECT_TYPE_TAPERED)
    {
        nStype = SectD.SectBefore.nStype;
    }
    else
    {
        nStype = SectD.nStype;
    }

    double dDimhi, dDimbi, dDimhj, dDimbj;
    if(!pDoc->m_pAttrCtrl->GetSectDimen(SectD, dDimhi, dDimbi, dDimhj, dDimbj, NULL, FALSE)) return FALSE;

    CArray<double, double> aGenTmp;
    CArray<double, double> aGenTmpPos;
    CArray<double, double> aH;
    CArray<double, double> aZ, aZDivBySect;

    if (InputData.nTempGrad == 0)
    {
        //Positive Gradient
        double d150mm = M_InitValueCurUnit(150.0, N, MM, D_UNITSYS_BASE_LENGTH);	
        double d250mm = M_InitValueCurUnit(250.0, N, MM, D_UNITSYS_BASE_LENGTH);	
        double d50mm  = M_InitValueCurUnit(50.0, N, MM, D_UNITSYS_BASE_LENGTH);
        
        if(dDimhi < min(0.3*dDimhi,d150mm))
        {		
            aGenTmpPos.Add(0.0);        
            aGenTmpPos.Add(min(0.3*dDimhi,d150mm));

            aGenTmp.Add(InputData.dT1);
            aGenTmp.Add(InputData.dT2);

        }
        else if(dDimhi < d150mm+min(0.3*dDimhi,d250mm))
        {
            aGenTmpPos.Add(0.0);
            aGenTmpPos.Add(min(0.3*dDimhi,d150mm));
            aGenTmpPos.Add(d150mm+min(0.3*dDimhi,d250mm));       

            aGenTmp.Add(InputData.dT1);
            aGenTmp.Add(InputData.dT2);
            aGenTmp.Add(InputData.dT3);
        }  
        else if(dDimhi < dDimhi - min(0.15*dDimhi,d150mm))
        {
            aGenTmpPos.Add(0.0);
            aGenTmpPos.Add(min(0.3*dDimhi,d150mm));
            aGenTmpPos.Add(d150mm+min(0.3*dDimhi,d250mm)); 
            aGenTmpPos.Add(dDimhi - min(0.15*dDimhi,d150mm));      

            aGenTmp.Add(InputData.dT1);
            aGenTmp.Add(InputData.dT2);
            aGenTmp.Add(InputData.dT3);
            aGenTmp.Add(InputData.dT3);
        }
        else
        {
            aGenTmpPos.Add(0.0);
            aGenTmpPos.Add(min(0.3*dDimhi,d150mm));
            aGenTmpPos.Add(d150mm+min(0.3*dDimhi,d250mm)); 
            aGenTmpPos.Add(dDimhi - min(0.15*dDimhi,d150mm)); 
            aGenTmpPos.Add(dDimhi);

            aGenTmp.Add(InputData.dT1);
            aGenTmp.Add(InputData.dT2);
            aGenTmp.Add(InputData.dT3);
            aGenTmp.Add(InputData.dT3);
            aGenTmp.Add(InputData.dT4);
        }
    }
    else
    {
        //Reverse Gradient
        double d200mm = M_InitValueCurUnit(200.0, N, MM, D_UNITSYS_BASE_LENGTH);	
        double d250mm = M_InitValueCurUnit(250.0, N, MM, D_UNITSYS_BASE_LENGTH);	
        double d50mm  = M_InitValueCurUnit(50.0, N, MM, D_UNITSYS_BASE_LENGTH);        
    
        if(dDimhi < min(0.2*dDimhi,d250mm))
        {		
            aGenTmpPos.Add(0.0);
            aGenTmpPos.Add(min(0.2*dDimhi,d250mm));
            
            aGenTmp.Add(InputData.dT1);
            aGenTmp.Add(InputData.dT2);
           

        }
        else if(dDimhi < min(0.2*dDimhi,d250mm)+min(0.25*dDimhi,d250mm))
        {
            aGenTmpPos.Add(0.0);
            aGenTmpPos.Add(min(0.2*dDimhi,d250mm));
            aGenTmpPos.Add(min(0.2*dDimhi,d250mm)+min(0.25*dDimhi,d250mm)); 
            
            aGenTmp.Add(InputData.dT1);
            aGenTmp.Add(InputData.dT2);
            aGenTmp.Add(0.0);
            
        }
        else if(dDimhi < dDimhi-min(0.25*dDimhi,d250mm)-min(0.25*dDimhi,d250mm))
        {
            aGenTmpPos.Add(0.0);
            aGenTmpPos.Add(min(0.2*dDimhi,d250mm));
            aGenTmpPos.Add(min(0.2*dDimhi,d250mm)+min(0.25*dDimhi,d250mm)); 
            aGenTmpPos.Add(dDimhi-min(0.25*dDimhi,d250mm)-min(0.25*dDimhi,d250mm));           

            aGenTmp.Add(InputData.dT1);
            aGenTmp.Add(InputData.dT2);
            aGenTmp.Add(0.0);
            aGenTmp.Add(0.0);
            
        }
        else if(dDimhi < dDimhi-min(0.25*dDimhi,d250mm))
        {
            aGenTmpPos.Add(0.0);
            aGenTmpPos.Add(min(0.2*dDimhi,d250mm));
            aGenTmpPos.Add(min(0.2*dDimhi,d250mm)+min(0.25*dDimhi,d250mm)); 
            aGenTmpPos.Add(dDimhi-min(0.25*dDimhi,d250mm)-min(0.25*dDimhi,d250mm)); 
            aGenTmpPos.Add(dDimhi-min(0.25*dDimhi,d250mm));
            
            aGenTmp.Add(InputData.dT1);
            aGenTmp.Add(InputData.dT2);
            aGenTmp.Add(0.0);
            aGenTmp.Add(0.0);
            aGenTmp.Add(InputData.dT3);           

        }
        else
        {
            aGenTmpPos.Add(0.0);
            aGenTmpPos.Add(min(0.2*dDimhi,d250mm));
            aGenTmpPos.Add(min(0.2*dDimhi,d250mm)+min(0.25*dDimhi,d250mm)); 
            aGenTmpPos.Add(dDimhi-min(0.25*dDimhi,d250mm)-min(0.25*dDimhi,d250mm)); 
            aGenTmpPos.Add(dDimhi-min(0.25*dDimhi,d250mm));
            aGenTmpPos.Add(dDimhi);

            aGenTmp.Add(InputData.dT1);
            aGenTmp.Add(InputData.dT2);
            aGenTmp.Add(0.0);
            aGenTmp.Add(0.0);
            aGenTmp.Add(InputData.dT3);
            aGenTmp.Add(InputData.dT4);
        }
    }

    ASSERT(aGenTmp.GetSize() == aGenTmpPos.GetSize());  

    GenStmp.SetSize(aGenTmp.GetSize() - 1);
    for(int i = 0; i < aGenTmp.GetSize() - 1; i++)
    {
        GenStmp[i].Initialize();

        GenStmp[i].nMaterial = 1;
        GenStmp[i].nRef = 0;
        GenStmp[i].nB = 0;
        GenStmp[i].nH1 = 3;
        GenStmp[i].nH2 = 3;

        GenStmp[i].dH1 = aGenTmpPos[i];
        GenStmp[i].dT1 = aGenTmp[i];
        GenStmp[i].dH2 = aGenTmpPos[i + 1];
        GenStmp[i].dT2 = aGenTmp[i + 1];
    }

    T_BTMP_STMP BlankBtmp;
    BlankBtmp.Initialize();
    BlankBtmp.nMaterial = 0;

    int nAddNum = D_BTMP_NUMSTMP - GenStmp.GetSize();
    for(int i = 0; i < nAddNum; i++)
    {
        GenStmp.Add(BlankBtmp);
    }

    return TRUE;
}

BOOL CStldBtmpDefCodeProvUtil::MakeStmpIRC62017Stlg(BTMP_DEF_CODE_PROV_IRC6_2017_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();

    GenStmp.RemoveAll();
    int nStype;

    if(SectD.nStype==D_SECT_TYPE_TAPERED)
    {
        nStype = SectD.SectBefore.nStype;
    }
    else
    {
        nStype = SectD.nStype;
    }

    double dDimhi, dDimbi, dDimhj, dDimbj;
    if(!pDoc->m_pAttrCtrl->GetSectDimen(SectD, dDimhi, dDimbi, dDimhj, dDimbj, NULL, FALSE)) return FALSE;
    //Slab
    double Bc[2]  = {SectD.SectAfter.SectJ.Size[0],   SectD.SectAfter.SectJ.Size[0]}; // I단과 J단 치수가 같으면 [1]에도 같은 값을 넣어줌
    double tc[2]  = {SectD.SectAfter.SectJ.Size[1],   SectD.SectAfter.SectJ.Size[1]};
    double Hh[2]  = {SectD.SectAfter.SectJ.Size[2],   SectD.SectAfter.SectJ.Size[2]};
    //Girder
    double Sg[2]  = {SectD.SectAfter.SectI.Size[3] ,  SectD.SectAfter.SectJ.Size[3]};
    double Top[2] = {SectD.SectAfter.SectI.Size[4] ,  SectD.SectAfter.SectJ.Size[4]};
    double Bot[2] = {SectD.SectAfter.SectI.Size[5] ,  SectD.SectAfter.SectJ.Size[5]};
    double dHw[2]  = {SectD.SectBefore.SectI.Size[0],  SectD.CmpTapJ.Size[0]};
    double dtw[2]  = {SectD.SectBefore.SectI.Size[1],  SectD.CmpTapJ.Size[1]};
    double dB1[2]  = {SectD.SectBefore.SectI.Size[2],  SectD.CmpTapJ.Size[2]};
    double dtf1[2]  = {SectD.SectBefore.SectI.Size[3],  SectD.CmpTapJ.Size[3]};
    double dB2[2]  = {SectD.SectBefore.SectI.Size[4],  SectD.CmpTapJ.Size[4]};
    double dtf2[2]  = {SectD.SectBefore.SectI.Size[5],  SectD.CmpTapJ.Size[5]};
    double H[2]   = {SectD.SectBefore.SectI.Size[6],  SectD.CmpTapJ.Size[6]};
    double t1[2]  = {SectD.SectBefore.SectI.Size[7],  SectD.CmpTapJ.Size[7]};
    double t2[2]  = {SectD.SectBefore.SectI.Size[8],  SectD.CmpTapJ.Size[8]};
    double tw1[2] = {SectD.SectBefore.SectI.Size[9],  SectD.CmpTapJ.Size[9]};
    double tw2[2] = {SectD.SectBefore.SectI.Size[10], SectD.CmpTapJ.Size[10]};
   
    double dEndH_Positive = M_InitValueCurUnit(0.4, N, M, D_UNITSYS_BASE_LENGTH) + tc[0];
    double dEndH2_Positive = 0.6* tc[0];
    double dTotalDepth = tc[0] + dtf1[0] + dHw[0] + dtf2[0] ;
    double dEndH_Neg    = tc[0] + dtf1[0];

    double d100mmCurUnit = M_InitValueCurUnit(100.0, N, MM, D_UNITSYS_BASE_LENGTH);	
    double d300mmCurUnit = M_InitValueCurUnit(300.0, N, MM, D_UNITSYS_BASE_LENGTH);
    double dSmallTCurUnit = M_InitValueCurUnit(10., N, MM, D_UNITSYS_BASE_LENGTH);   

    CArray<double, double> aGenTmp;
    CArray<double, double> aGenTmpPos;
    CArray<double, double> aH;
    CArray<double, double> aZ, aZDivBySect;
   
     if (InputData.nTempGrad == 0)
    {
        //Positive Gradient
       
        aGenTmp.Add(MakeStmpIRC62017Stlg_PosGrd_Part1(tc[0]));		
        aGenTmp.Add(InputData.dT2);
        aGenTmp.Add(MakeStmpIRC62017Stlg_PosGrd_Part2(dEndH2_Positive,dEndH_Positive,tc[0]));
        aGenTmp.Add(MakeStmpIRC62017Stlg_PosGrd_Part2(dEndH2_Positive,dEndH_Positive,tc[0]+dtf1[0]));
        aGenTmp.Add(0);

        aGenTmpPos.Add(0.0);		
        aGenTmpPos.Add(dEndH2_Positive);
        aGenTmpPos.Add(tc[0]);
        aGenTmpPos.Add(tc[0]+dtf1[0]);
        aGenTmpPos.Add(dEndH_Positive);
    }
     else
     {
         //Negative Gradient
         aGenTmp.Add(InputData.dT1);		
         aGenTmp.Add(InputData.dT2);//aGenTmp.Add(MakeStmpIRC62017Stlg_NegGrd_Part1(tc[0],dEndH2_Positive));Discussed with Pratap. Total 2 points with 0 value
         aGenTmp.Add(InputData.dT2);
         aGenTmp.Add(MakeStmpIRC62017Stlg_NegGrd_Part2(tc[0] ,dEndH_Positive ,dEndH_Neg));         
         aGenTmp.Add(InputData.dT4);
         aGenTmp.Add(InputData.dT4);
         aGenTmp.Add(InputData.dT4);

         aGenTmpPos.Add(0.0);		
         aGenTmpPos.Add(dEndH2_Positive);
         aGenTmpPos.Add(tc[0]);
         aGenTmpPos.Add(tc[0]+dtf1[0]);         
         aGenTmpPos.Add(dEndH_Positive);
         aGenTmpPos.Add(dTotalDepth-dtf2[0]);
         aGenTmpPos.Add(dTotalDepth);
     }
    ASSERT(aGenTmp.GetSize() == aGenTmpPos.GetSize());  

    GenStmp.SetSize(aGenTmp.GetSize() - 1);
    for(int i = 0; i < aGenTmp.GetSize() - 1; i++)
    {
        GenStmp[i].Initialize();

        GenStmp[i].nMaterial = 1;
        GenStmp[i].nRef = 0;
        GenStmp[i].nB = 0;
        GenStmp[i].nH1 = 3;
        GenStmp[i].nH2 = 3;

        GenStmp[i].dH1 = aGenTmpPos[i];
        GenStmp[i].dT1 = aGenTmp[i];
        GenStmp[i].dH2 = aGenTmpPos[i + 1];
        GenStmp[i].dT2 = aGenTmp[i + 1];
    }

    T_BTMP_STMP BlankBtmp;
    BlankBtmp.Initialize();
    BlankBtmp.nMaterial = 0;

    int nAddNum = D_BTMP_NUMSTMP - GenStmp.GetSize();
    for(int i = 0; i < nAddNum; i++)
    {
        GenStmp.Add(BlankBtmp);
    }

    return TRUE;
}

double CStldBtmpDefCodeProvUtil::MakeStmpIRC62017Stlg_PosGrd_Part1(double dSlabDepth)
{
	double dCal = 0.;
	if (dSlabDepth < 0.2) return 18.0;
	else if (dSlabDepth > 0.3) return 20.5;
	else
	{
		//Interpolate
		dCal = 18.0 + ((20.5-18.0)*(dSlabDepth-0.2)/(0.3-0.2));
		return dCal;

	}

}


double CStldBtmpDefCodeProvUtil::MakeStmpIRC62017Stlg_PosGrd_Part2(double dstartht,double dendht,double dSlabDepth)
{
	double dCal = 0.;
	if (dSlabDepth < dstartht) return 4.0;
	else if (dSlabDepth > dendht) return 0.0;
	else
	{
		//Interpolate
		dCal = 4. + ((0.-4.)*(dSlabDepth-dstartht)/(dendht-dstartht));
		return dCal;
	}

}

double CStldBtmpDefCodeProvUtil::MakeStmpIRC62017Stlg_NegGrd_Part1(double dendht,double dSlabDepth)
{
	double dCal = 0.;
	if (dSlabDepth == 0.) return -4.4;
	else if (dSlabDepth > dendht) return 0.0;
	else
	{
		//Interpolate
		dCal = -4.4 + ((0.0+4.4)*(dSlabDepth-0.0)/(dendht-0.0));
		return dCal;

	}

}

double CStldBtmpDefCodeProvUtil::MakeStmpIRC62017Stlg_NegGrd_Part2(double dstartht,double dendht,double dSlabDepth)
{
	double dCal = 0.;
	if (dSlabDepth < dstartht) return 0.0;
	else if (dSlabDepth > dendht) return -8.0;
	else
	{
		//Interpolate
		dCal = 0. + ((-8.-0.)*(dSlabDepth-dstartht)/(dendht-dstartht));
		return dCal;
	}

}


BOOL CStldBtmpDefCodeProvUtil::MakeStmpAASHTO2020(BTMP_DEF_CODE_PROV_AASHTO_2020_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg)
{
	if(InputData.nSectType == 0)
	{
		return MakeStmpAASHTO2020PSC(InputData, ElemK, SectD, nBtmpType, GenStmp, aErrorMsg);
	}
	else if(InputData.nSectType == 1) 
	{
		return MakeStmpAASHTO2020Stlg(InputData, ElemK, SectD, nBtmpType, GenStmp, aErrorMsg);
	}
	ASSERT(0);

	return FALSE;
}

BOOL CStldBtmpDefCodeProvUtil::MakeStmpAASHTO2020PSC(BTMP_DEF_CODE_PROV_AASHTO_2020_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();
	CString strTemp;

	GenStmp.RemoveAll();
	int nStype;

	if(SectD.nStype==D_SECT_TYPE_TAPERED)
	{
		nStype = SectD.SectBefore.nStype;
	}
	else
	{
		nStype = SectD.nStype;
	}

	if(nStype != D_SECT_TYPE_PSC && 
       nStype != D_SECT_TYPE_COMPO_CI && nStype != D_SECT_TYPE_COMPO_CT && nStype != D_SECT_TYPE_COMPO_PC && nStype != D_SECT_TYPE_COMPO_G)
	{
		CString strTemp;
		strTemp.Format(_LS(IDS_DB_WARNING_BTMP_IRG_PSC_COMPO_SECT), ElemK);
		aErrorMsg.Add(strTemp);
		//ASSERT(0);
		return FALSE;
	}

	double dDimhi, dDimbi, dDimhj, dDimbj;
	if(!pDoc->m_pAttrCtrl->GetSectDimen(SectD, dDimhi, dDimbi, dDimhj, dDimbj, NULL, FALSE)) return FALSE;

	CArray<double, double> aGenTmp;
	CArray<double, double> aGenTmpPos;
	CArray<double, double> aH;
	CArray<double, double> aZ, aZDivBySect;

	// Tapered일때는 이전에 중간단면으로 넘겨줬기 때문에 무조건 I단으로 계산하면 된다.

	double d4_Cur = M_InitValueCurUnit(4.0, KIP, IN, D_UNITSYS_BASE_LENGTH);	
	double d8_Cur = M_InitValueCurUnit(8.0, KIP, IN, D_UNITSYS_BASE_LENGTH);	
	double d16_Cur = M_InitValueCurUnit(16.0, KIP, IN, D_UNITSYS_BASE_LENGTH);	

	double dT1 = InputData.dT1;
	double dT2 = InputData.dT2;
	double dT3 = InputData.dT3;

	double dA_in = 0.0;
	if(InputData.nA==0)
	{
		if(dDimhi >= d16_Cur-dZero) dA_in = 12.0; //in.
		else                        dA_in =  4.0; //in.
	}
	else
	{
		dA_in = InputData.dA;
	}

	double dA_Cur = M_InitValueCurUnit(dA_in, KIP, IN, D_UNITSYS_BASE_LENGTH);	

	double dH_Sum =  d4_Cur + dA_Cur + d8_Cur;

	if(dDimhi < dH_Sum)
	{		
		strTemp.Format(_LS(IDS_DB_ERROR_BTMP_HEIGHT_SUM), ElemK);
		aErrorMsg.Add(strTemp);
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	aGenTmp.Add(InputData.dT1);
	aGenTmp.Add(InputData.dT2);
	aGenTmp.Add(0.0);

	aGenTmpPos.Add(0.0);
	aGenTmpPos.Add(d4_Cur);
	aGenTmpPos.Add(d4_Cur + dA_Cur);

	if (dDimhi <= d4_Cur + dA_Cur + d8_Cur + dZero)
	{
		aGenTmp.Add(InputData.dT3);
		aGenTmpPos.Add(d4_Cur + dA_Cur + d8_Cur); // = dDimhi
	}
	else
	{
		aGenTmp.Add(0.0);
		aGenTmp.Add(InputData.dT3);

		aGenTmpPos.Add(dDimhi - d8_Cur);
		aGenTmpPos.Add(dDimhi);
	}
	//////////////////////////////////////////////////////////////////////////

	ASSERT(aGenTmp.GetSize() == aGenTmpPos.GetSize());  

	GenStmp.SetSize(aGenTmp.GetSize() - 1);
	for(int i = 0; i < aGenTmp.GetSize() - 1; i++)
	{
		GenStmp[i].Initialize();

		GenStmp[i].nMaterial = 1;
		GenStmp[i].nRef = 0;
		GenStmp[i].nB = 0;
		GenStmp[i].nH1 = 3;
		GenStmp[i].nH2 = 3;

		GenStmp[i].dH1 = aGenTmpPos[i];
		GenStmp[i].dT1 = aGenTmp[i];
		GenStmp[i].dH2 = aGenTmpPos[i + 1];
		GenStmp[i].dT2 = aGenTmp[i + 1];
	}

	T_BTMP_STMP BlankBtmp;
	BlankBtmp.Initialize();
	BlankBtmp.nMaterial = 0;

	int nAddNum = D_BTMP_NUMSTMP - GenStmp.GetSize();
	for(int i = 0; i < nAddNum; i++)
	{
		GenStmp.Add(BlankBtmp);
	}

	return TRUE;
}

BOOL CStldBtmpDefCodeProvUtil::MakeStmpAASHTO2020PSCOld(BTMP_DEF_CODE_PROV_AASHTO_2020_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg)
{
	// t 고려 안함.
	return TRUE;

// 	CDBDoc* pDoc = CDBDoc::GetDocPoint();
// 	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();
// 	CString strTemp;
// 
// 	GenStmp.RemoveAll();
// 	int nStype;
// 
// 	if (SectD.nStype == D_SECT_TYPE_TAPERED)
// 	{
// 		nStype = SectD.SectBefore.nStype;
// 	}
// 	else
// 	{
// 		nStype = SectD.nStype;
// 	}
// 
// 	if (nStype != D_SECT_TYPE_PSC &&
// 		nStype != D_SECT_TYPE_COMPO_CI && nStype != D_SECT_TYPE_COMPO_CT && nStype != D_SECT_TYPE_COMPO_PC && nStype != D_SECT_TYPE_COMPO_G)
// 	{
// 		CString strTemp;
// 		strTemp.Format(_LS(IDS_DB_WARNING_BTMP_IRG_PSC_COMPO_SECT), ElemK);
// 		aErrorMsg.Add(strTemp);
// 		//ASSERT(0);
// 		return FALSE;
// 	}
// 
// 	double dDimhi, dDimbi, dDimhj, dDimbj;
// 	if (!pDoc->m_pAttrCtrl->GetSectDimen(SectD, dDimhi, dDimbi, dDimhj, dDimbj, NULL, FALSE)) return FALSE;
// 
// 	CArray<double, double> aGenTmp;
// 	CArray<double, double> aGenTmpPos;
// 	CArray<double, double> aH;
// 	CArray<double, double> aZ, aZDivBySect;
// 
// 	// Tapered일때는 이전에 중간단면으로 넘겨줬기 때문에 무조건 I단으로 계산하면 된다.
// 
// 	double dTcL = 0.0;
// 	double dTcR = 0.0;
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	// dSmallTCurUnit
// 	T_ELEM_D ElemD;
// 	ElemD.Initialize();
// 	if (!pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))	return FALSE;
// 
// 	BOOL bPosI = TRUE;
// 	BOOL bClosedSect = FALSE;
// 	int  nDgnSectType = -1;
// 	double dKt = 0.0;
// 	double aKt[6] = { 0.0 };
// 	double dWt = 0.0;
// 	double dPyKt[6] = { 0.,0.,0.,0.,0.,0. };
// 	double dPzKt[6] = { 0.,0.,0.,0.,0.,0. };
// 	double dFlgThk[2] = { 0.0, 0.0 }; // Flange Thickness (0=Top,1=Bot).
// 	BOOL bDivSect4Tor = FALSE;
// 	int nDivPart = 0;
// 	double dCutPos[3] = { 0.,0.,0. };
// 
// 	double dTc = 0.0;
// 	if (nStype == D_SECT_TYPE_COMPO_G)
// 	{
// 		// OutPolygon이 두개인 것만! 그중에서 두번째 outpolygon이 슬래브로 가정.
// 		// 차라리 사용자 입력으로 하면...
// 		if (InputData.nSmallT == 0)
// 		{
// 			if (SectD.SectBefore.SectI.aOuterPolygon.GetSize() != 2)
// 			{
// 				aErrorMsg.Add(_LS(IDS_ERR_BTMP_AASHTO_SMALL_T));
// 				return FALSE;
// 			}
// 			double dMaxY = -DBL_MAX;
// 			double dMinY = DBL_MAX;
// 
// 			int nNum = SectD.SectBefore.SectI.aOuterPolygon[1].aVertex.GetSize();
// 			for (int i = 0; i < nNum; i++)
// 			{
// 				double dY = SectD.SectBefore.SectI.aOuterPolygon[1].aVertex[i].dY;
// 				dMaxY = max(dMaxY, dY);
// 				dMinY = min(dMinY, dY);
// 			}
// 			dTc = fabs(dMaxY - dMinY);
// 		}
// 	}
// 	else if (nStype == D_SECT_TYPE_COMPO_PC || nStype == D_SECT_TYPE_COMPO_CI || nStype == D_SECT_TYPE_COMPO_CT)
// 	{
// 		dTc = SectD.SectAfter.SectJ.Size[1];
// 	}
// 	else
// 	{
// 		CSectUtil::GetPscKtAmPointBySect(SectD, bPosI, bClosedSect, nDgnSectType, dKt, aKt, dWt, dPyKt, dPzKt, dFlgThk, bDivSect4Tor, nDivPart, dCutPos);
// 
// 		dTc = dFlgThk[0];
// 	}
// 
// 	double dSmallTCurUnit = 0.0;
// 	if (InputData.nSmallT == 0)  dSmallTCurUnit = dTc;
// 	else                         dSmallTCurUnit = M_InitValueCurUnit(InputData.dSmallT, KIP, IN, D_UNITSYS_BASE_LENGTH);
// 	//////////////////////////////////////////////////////////////////////////
// 
// 
// 	double d4_Cur = M_InitValueCurUnit(4.0, KIP, IN, D_UNITSYS_BASE_LENGTH);
// 	double d8_Cur = M_InitValueCurUnit(8.0, KIP, IN, D_UNITSYS_BASE_LENGTH);
// 	double d16_Cur = M_InitValueCurUnit(16.0, KIP, IN, D_UNITSYS_BASE_LENGTH);
// 
// 	double dT1 = InputData.dT1;
// 	double dT2 = InputData.dT2;
// 	double dT3 = InputData.dT3;
// 
// 	// 온도는 변환하지 않고 그대로 적용!
// // 	T_PREFERENCE rPreference;
// // 	pDoc->m_pInitCtrl->GetPreference(rPreference);
// // 
// // 	if(rPreference.Unit.nTemperUnit == 0)
// // 	{
// // 		dT1 = (dT1-32)/1.8;
// // 		dT2 = (dT2-32)/1.8;
// // 		dT3 = (dT3-32)/1.8;
// // 	}
// 
// 	double dA_in = 0.0;
// 	if (InputData.nA == 0)
// 	{
// 		if (dDimhi >= d16_Cur - dZero) dA_in = 12.0; //in.
// 		else                        dA_in = 4.0; //in.
// 	}
// 	else
// 	{
// 		dA_in = InputData.dA;
// 	}
// 
// 	double dA_Cur = M_InitValueCurUnit(dA_in, KIP, IN, D_UNITSYS_BASE_LENGTH);
// 
// 	if (fabs(dSmallTCurUnit) < dZero)
// 	{
// 		aErrorMsg.Add(_LS(IDS_ERR_BTMP_AASHTO_SMALL_T));
// 		return FALSE;
// 	}
// 	if (dSmallTCurUnit > d4_Cur + dA_Cur + dZero)
// 	{
// 		aErrorMsg.Add(_LS(IDS_ERR_BTMP_AASHTO_SMALL_T));
// 		return FALSE;
// 	}
// 
// 	double dH_Sum = d4_Cur + dA_Cur + d8_Cur;
// 
// 	if (dDimhi < dH_Sum)
// 	{
// 		strTemp.Format(_LS(IDS_DB_ERROR_BTMP_HEIGHT_SUM), ElemK);
// 		aErrorMsg.Add(strTemp);
// 		return FALSE;
// 	}
// 	if (dDimhi < dSmallTCurUnit)
// 	{
// 		strTemp.Format(_LS(IDS_DB_ERROR_BTMP_HEIGHT_SUM), ElemK);
// 		aErrorMsg.Add(strTemp);
// 		return FALSE;
// 	}
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	// dSmallTCurUnit
// 	if (fabs(dSmallTCurUnit - d4_Cur) < dZero)  // t = 4
// 	{
// 		aGenTmp.Add(InputData.dT1);
// 		aGenTmp.Add(InputData.dT2);
// 		aGenTmp.Add(0.0);
// 
// 		aGenTmpPos.Add(0.0);
// 		aGenTmpPos.Add(d4_Cur);
// 		aGenTmpPos.Add(d4_Cur + dA_Cur);
// 
// 		if (dDimhi <= d4_Cur + dA_Cur + d8_Cur + dZero)
// 		{
// 			aGenTmp.Add(InputData.dT3);
// 			aGenTmpPos.Add(d4_Cur + dA_Cur + d8_Cur); // = dDimhi
// 		}
// 		else
// 		{
// 			aGenTmp.Add(0.0);
// 			aGenTmp.Add(InputData.dT3);
// 
// 			aGenTmpPos.Add(dDimhi - d8_Cur);
// 			aGenTmpPos.Add(dDimhi);
// 		}
// 	}
// 	else if (fabs(dSmallTCurUnit - d4_Cur - dA_Cur) < dZero) // t = 4 + A
// 	{
// 		aGenTmp.Add(InputData.dT1);
// 		aGenTmp.Add(InputData.dT2);
// 		aGenTmp.Add(0.0);
// 
// 		aGenTmpPos.Add(0.0);
// 		aGenTmpPos.Add(d4_Cur);
// 		aGenTmpPos.Add(d4_Cur + dA_Cur);
// 
// 		if (dDimhi <= d4_Cur + dA_Cur + d8_Cur + dZero)
// 		{
// 			aGenTmp.Add(InputData.dT3);
// 			aGenTmpPos.Add(d4_Cur + dA_Cur + d8_Cur);
// 		}
// 		else
// 		{
// 			aGenTmp.Add(0.0);
// 			aGenTmp.Add(InputData.dT3);
// 
// 			aGenTmpPos.Add(dDimhi - d8_Cur);
// 			aGenTmpPos.Add(dDimhi);
// 		}
// 	}
// 	// 구간
// 	else if (dSmallTCurUnit < d4_Cur)
// 	{
// 		double dFS = (dT2 - dT1) / d4_Cur;
// 		double dMidT = dT1 + dFS * dSmallTCurUnit;
// 		double dMidPos = dSmallTCurUnit;
// 
// 		aGenTmp.Add(InputData.dT1);
// 		aGenTmp.Add(dMidT);
// 		aGenTmp.Add(InputData.dT2);
// 		aGenTmp.Add(0.0);
// 
// 		aGenTmpPos.Add(0.0);
// 		aGenTmpPos.Add(dSmallTCurUnit);
// 		aGenTmpPos.Add(d4_Cur);
// 		aGenTmpPos.Add(d4_Cur + dA_Cur);
// 
// 		if (dDimhi <= d4_Cur + dA_Cur + d8_Cur + dZero)
// 		{
// 			aGenTmp.Add(InputData.dT3);
// 			aGenTmpPos.Add(d4_Cur + dA_Cur + d8_Cur);
// 		}
// 		else
// 		{
// 			aGenTmp.Add(0.0);
// 			aGenTmp.Add(InputData.dT3);
// 
// 			aGenTmpPos.Add(dDimhi - d8_Cur);
// 			aGenTmpPos.Add(dDimhi);
// 		}
// 
// 	}
// 	else if (dSmallTCurUnit < d4_Cur + dA_Cur) // 4in ~ A 영역에 포함
// 	{
// 		double dFS = (0.0 - dT2) / dA_Cur;
// 		double dMidT = dT2 + dFS * (dSmallTCurUnit - d4_Cur);
// 		double dMidPos = dSmallTCurUnit;
// 
// 		aGenTmp.Add(InputData.dT1);
// 		aGenTmp.Add(InputData.dT2);
// 		aGenTmp.Add(dMidT);
// 		aGenTmp.Add(0.0);
// 
// 		aGenTmpPos.Add(0.0);
// 		aGenTmpPos.Add(d4_Cur);
// 		aGenTmpPos.Add(dMidPos);
// 		aGenTmpPos.Add(d4_Cur + dA_Cur);
// 
// 		if (dDimhi <= d4_Cur + dA_Cur + d8_Cur + dZero)
// 		{
// 			aGenTmp.Add(InputData.dT3);
// 			aGenTmpPos.Add(d4_Cur + dA_Cur + d8_Cur);
// 		}
// 		else
// 		{
// 			aGenTmp.Add(0.0);
// 			aGenTmp.Add(InputData.dT3);
// 
// 			aGenTmpPos.Add(dDimhi - d8_Cur);
// 			aGenTmpPos.Add(dDimhi);
// 		}
// 	}
// 	else
// 	{
// 		ASSERT(0);
// 	}
// 
// 	ASSERT(aGenTmp.GetSize() == aGenTmpPos.GetSize());
// 
// 	GenStmp.SetSize(aGenTmp.GetSize() - 1);
// 	for (int i = 0; i < aGenTmp.GetSize() - 1; i++)
// 	{
// 		GenStmp[i].Initialize();
// 
// 		GenStmp[i].nMaterial = 1;
// 		GenStmp[i].nRef = 0;
// 		GenStmp[i].nB = 0;
// 		GenStmp[i].nH1 = 3;
// 		GenStmp[i].nH2 = 3;
// 
// 		GenStmp[i].dH1 = aGenTmpPos[i];
// 		GenStmp[i].dT1 = aGenTmp[i];
// 		GenStmp[i].dH2 = aGenTmpPos[i + 1];
// 		GenStmp[i].dT2 = aGenTmp[i + 1];
// 	}
// 
// 	T_BTMP_STMP BlankBtmp;
// 	BlankBtmp.Initialize();
// 	BlankBtmp.nMaterial = 0;
// 
// 	int nAddNum = D_BTMP_NUMSTMP - GenStmp.GetSize();
// 	for (int i = 0; i < nAddNum; i++)
// 	{
// 		GenStmp.Add(BlankBtmp);
// 	}
// 
// 	return TRUE;
}

BOOL CStldBtmpDefCodeProvUtil::MakeStmpAASHTO2020Stlg(BTMP_DEF_CODE_PROV_AASHTO_2020_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();
	CString strTemp;

	GenStmp.RemoveAll();
	int nStype;

	if(SectD.nStype==D_SECT_TYPE_TAPERED)
	{
		nStype = SectD.SectBefore.nStype;
	}
	else
	{
		nStype = SectD.nStype;
	}

	if(nStype != D_SECT_TYPE_COMPO_B      && nStype != D_SECT_TYPE_COMPO_I      && nStype != D_SECT_TYPE_COMPO_TUB && 
	   nStype != D_SECT_TYPE_COMPO_STLG_B && nStype != D_SECT_TYPE_COMPO_STLG_I && nStype != D_SECT_TYPE_COMPO_STLG_TUB)
	{
		CString strTemp;
		strTemp.Format(_LS(IDS_DB_WARNING_BTMP_IRG_PSC_COMPO_SECT), ElemK);
		aErrorMsg.Add(strTemp);
		//ASSERT(0);
		return FALSE;
	}

	double dDimhi, dDimbi, dDimhj, dDimbj;
	if(!pDoc->m_pAttrCtrl->GetSectDimen(SectD, dDimhi, dDimbi, dDimhj, dDimbj, NULL, FALSE)) return FALSE;

	//////////////////////////////////////////////////////////////////////////

	double d4_Cur = M_InitValueCurUnit(4.0, KIP, IN, D_UNITSYS_BASE_LENGTH);	
	double d8_Cur = M_InitValueCurUnit(8.0, KIP, IN, D_UNITSYS_BASE_LENGTH);	

	double dT1 = InputData.dT1;
	double dT2 = InputData.dT2;
	double dT3 = InputData.dT3;

	// Tapered일때는 이전에 중간단면으로 넘겨줬기 때문에 무조건 I단으로 계산하면 된다.
	double dTc = SectD.SectAfter.SectJ.Size[1];
  //double dHh = SectD.SectAfter.SectJ.Size[2];
	double dSmallTCurUnit = 0.0;
	if(InputData.nSmallT==0)  dSmallTCurUnit = dTc;
	else                      dSmallTCurUnit = M_InitValueCurUnit(InputData.dSmallT, KIP, IN, D_UNITSYS_BASE_LENGTH);	

	double dA_in = 0.0;
	if(InputData.nA==0) dA_in = 12.0; //in.
	else                dA_in = InputData.dA;
	double dA_Cur = M_InitValueCurUnit(dA_in, KIP, IN, D_UNITSYS_BASE_LENGTH);	

	double dH_Sum =  d4_Cur + dA_Cur;
	if(dDimhi < dH_Sum)
	{		
		strTemp.Format(_LS(IDS_DB_ERROR_BTMP_HEIGHT_SUM), ElemK);
		aErrorMsg.Add(strTemp);
		return FALSE;
	}
// 	if(dSmallTCurUnit < d4_Cur - dZero)
// 	{
// 		aErrorMsg.Add(_LS(IDS_ERR_BTMP_AASHTO_SMALL_T));
// 		return FALSE;
// 	}
	if(dSmallTCurUnit > d4_Cur + dA_Cur + dZero)
	{
		aErrorMsg.Add(_LS(IDS_ERR_BTMP_AASHTO_SMALL_T));
		return FALSE;
	}

	if (dDimhi < dSmallTCurUnit)
	{
		strTemp.Format(_LS(IDS_DB_ERROR_BTMP_HEIGHT_SUM), ElemK);
		aErrorMsg.Add(strTemp);
		return FALSE;
	}

	CArray<double, double> aGenTmp;
	CArray<double, double> aGenTmpPos;
	CArray<double, double> aH;
	CArray<double, double> aZ, aZDivBySect;

	if(fabs(dSmallTCurUnit) < dZero)
	{
		aGenTmp.Add(InputData.dT1);
		aGenTmp.Add(InputData.dT1);

		aGenTmpPos.Add(0.0);
		aGenTmpPos.Add(dDimhi);
	}
	else if(fabs(dSmallTCurUnit - d4_Cur) < dZero)
	{
		aGenTmp.Add(InputData.dT1);
		aGenTmp.Add(InputData.dT2);
		aGenTmp.Add(InputData.dT2);

		aGenTmpPos.Add(0.0);
		aGenTmpPos.Add(d4_Cur);
		aGenTmpPos.Add(dDimhi);
	}
	else if(fabs(dSmallTCurUnit - d4_Cur - dA_Cur) < dZero)
	{
		aGenTmp.Add(InputData.dT1);
		aGenTmp.Add(InputData.dT2);
		aGenTmp.Add(0.0);

		aGenTmpPos.Add(0.0);
		aGenTmpPos.Add(d4_Cur);
		aGenTmpPos.Add(d4_Cur + dA_Cur);
	}
	// 구간
	else if(dSmallTCurUnit < d4_Cur)
	{
		double dFS    = (dT2 - dT1)/d4_Cur;
		double dMidT  = dT1 + dFS*dSmallTCurUnit;
		double dMidPos= dSmallTCurUnit;

		aGenTmp.Add(InputData.dT1);
		aGenTmp.Add(dMidT);
		aGenTmp.Add(dMidT);

		aGenTmpPos.Add(0.0);
		aGenTmpPos.Add(dSmallTCurUnit);
		aGenTmpPos.Add(dDimhi);
	}
	else
	{
		// 4in ~ A 영역에 포함
		double dFS    = (0.0 - dT2)/dA_Cur;
		double dMidT  = dT2 + dFS*(dSmallTCurUnit - d4_Cur);
		double dMidPos= dSmallTCurUnit;

		aGenTmp.Add(InputData.dT1);		
		aGenTmp.Add(InputData.dT2);
		aGenTmp.Add(dMidT);
		aGenTmp.Add(dMidT);

		aGenTmpPos.Add(0.0);		
		aGenTmpPos.Add(d4_Cur);
		aGenTmpPos.Add(dMidPos);
		aGenTmpPos.Add(dDimhi);
	}
	ASSERT(aGenTmp.GetSize() == aGenTmpPos.GetSize());  


	GenStmp.SetSize(aGenTmp.GetSize() - 1);
	for(int i = 0; i < aGenTmp.GetSize() - 1; i++)
	{
		GenStmp[i].Initialize();

		GenStmp[i].nMaterial = 1;
		GenStmp[i].nRef = 0;
		GenStmp[i].nB = 0;
		GenStmp[i].nH1 = 3;
		GenStmp[i].nH2 = 3;

		GenStmp[i].dH1 = aGenTmpPos[i];
		GenStmp[i].dT1 = aGenTmp[i];
		GenStmp[i].dH2 = aGenTmpPos[i + 1];
		GenStmp[i].dT2 = aGenTmp[i + 1];
	}

	T_BTMP_STMP BlankBtmp;
	BlankBtmp.Initialize();
	BlankBtmp.nMaterial = 0;

	int nAddNum = D_BTMP_NUMSTMP - GenStmp.GetSize();
	for(int i = 0; i < nAddNum; i++)
	{
		GenStmp.Add(BlankBtmp);
	}

	return TRUE;
}

BOOL CStldBtmpDefCodeProvUtil::MakeStmpEurocode( BTMP_DEF_CODE_PROV_EUROCODE_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg)
{
	if(InputData.nSectType == 0)
	{
		return MakeStmpEurocodePSC(InputData, ElemK, SectD, nBtmpType, GenStmp, aErrorMsg);
	}
	else if(InputData.nSectType == 1) 
	{
		return MakeStmpEurocodeStlg(InputData, ElemK, SectD, nBtmpType, GenStmp, aErrorMsg);
	}
	ASSERT(0);

	return TRUE;
}
BOOL CStldBtmpDefCodeProvUtil::MakeStmpEurocodePSC( BTMP_DEF_CODE_PROV_EUROCODE_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();
	CString strTemp;

	GenStmp.RemoveAll();
	int nStype;

	if(SectD.nStype==D_SECT_TYPE_TAPERED)
	{
		nStype = SectD.SectBefore.nStype;
	}
	else
	{
		nStype = SectD.nStype;
	}

	if(nStype != D_SECT_TYPE_PSC && 
		nStype != D_SECT_TYPE_COMPO_CI && nStype != D_SECT_TYPE_COMPO_CT && nStype != D_SECT_TYPE_COMPO_PC && nStype != D_SECT_TYPE_COMPO_G)
	{
		CString strTemp;
		strTemp.Format(_LS(IDS_DB_WARNING_BTMP_IRG_PSC_COMPO_SECT), ElemK);
		aErrorMsg.Add(strTemp);
		//ASSERT(0);
		return FALSE;
	}

	double dDimhi, dDimbi, dDimhj, dDimbj;
	if(!pDoc->m_pAttrCtrl->GetSectDimen(SectD, dDimhi, dDimbi, dDimhj, dDimbj, NULL, FALSE)) return FALSE;

	CArray<double, double> aGenTmp;
	CArray<double, double> aGenTmpPos;
	CArray<double, double> aH;
	CArray<double, double> aZ, aZDivBySect;

	double d015m = M_InitValueCurUnit(0.15, N, M, D_UNITSYS_BASE_LENGTH);	
	double d010m = M_InitValueCurUnit(0.10, N, M, D_UNITSYS_BASE_LENGTH);	
	double d020m = M_InitValueCurUnit(0.20, N, M, D_UNITSYS_BASE_LENGTH);	
	double d025m = M_InitValueCurUnit(0.25, N, M, D_UNITSYS_BASE_LENGTH);	

	double dH1 = M_InitValueCurUnit(InputData.dH1, N, M, D_UNITSYS_BASE_LENGTH);	
	double dH2 = M_InitValueCurUnit(InputData.dH2, N, M, D_UNITSYS_BASE_LENGTH);	
	double dH3 = M_InitValueCurUnit(InputData.dH3, N, M, D_UNITSYS_BASE_LENGTH);	
	double dH4 = M_InitValueCurUnit(InputData.dH4, N, M, D_UNITSYS_BASE_LENGTH);	

	double dH1_Auto = (InputData.nTempGrad==0)? min(0.30*dDimhi,d015m)                    :  min(0.20*dDimhi,d025m);
	double dH2_Auto = (InputData.nTempGrad==0)? max(d010m,  min(0.30*dDimhi,d025m))       :  max(0.25*dDimhi,d020m);
	double dH3_Auto = (InputData.nTempGrad==0)? 0.30*dDimhi                               :  max(0.25*dDimhi,d020m);
	double dH4_Auto = min(0.20*dDimhi,d025m);

	double dH1_Cur = (InputData.nH1==0)? dH1_Auto : dH1;
	double dH2_Cur = (InputData.nH2==0)? dH2_Auto : dH2;
	double dH3_Cur = (InputData.nH3==0)? dH3_Auto : dH3;
	double dH4_Cur = (InputData.nH4==0)? dH4_Auto : dH4;

	double dT1 = InputData.dT1;
	double dT2 = InputData.dT2;
	double dT3 = InputData.dT3;
	double dT4 = InputData.dT4;

	double dH_Sum = (InputData.nTempGrad==0)? dH1+dH2+dH3 : dH1+dH2+dH3+dH4;
	if(dDimhi < dH_Sum)
	{		
		strTemp.Format(_LS(IDS_DB_ERROR_BTMP_HEIGHT_SUM), ElemK);
		aErrorMsg.Add(strTemp);
		return FALSE;
	}


	// 경계
	if(fabs(dDimhi - dH1_Cur) < dZero)
	{
		aGenTmp.Add(InputData.dT1);
		aGenTmp.Add(InputData.dT2);

		aGenTmpPos.Add(0.0);
		aGenTmpPos.Add(dH1_Cur);
	}
	else if(fabs(dDimhi - dH1_Cur - dH2_Cur) < dZero)
	{
		aGenTmp.Add(InputData.dT1);
		aGenTmp.Add(InputData.dT2);
		aGenTmp.Add(0.0);

		aGenTmpPos.Add(0.0);
		aGenTmpPos.Add(dH1_Cur);
		aGenTmpPos.Add(dH1_Cur + dH2_Cur);
	}
	else if(fabs(dDimhi - dH1_Cur - dH2_Cur - dH3_Cur) < dZero)
	{
		aGenTmp.Add(InputData.dT1);
		aGenTmp.Add(InputData.dT2);
		aGenTmp.Add(0.0);
		aGenTmp.Add(InputData.dT3);

		aGenTmpPos.Add(0.0);
		aGenTmpPos.Add(dH1_Cur);
		aGenTmpPos.Add(dH1_Cur + dH2_Cur);
		aGenTmpPos.Add(dH1_Cur + dH2_Cur + dH3_Cur);
	}
	else if(InputData.nTempGrad==1 && fabs(dDimhi - dH1_Cur - dH2_Cur - dH3_Cur - dH4_Cur) < dZero)
	{
		aGenTmp.Add(InputData.dT1);
		aGenTmp.Add(InputData.dT2);
		aGenTmp.Add(0.0);
		aGenTmp.Add(InputData.dT3);
		aGenTmp.Add(InputData.dT4);

		aGenTmpPos.Add(0.0);
		aGenTmpPos.Add(dH1_Cur);
		aGenTmpPos.Add(dH1_Cur + dH2_Cur);
		aGenTmpPos.Add(dH1_Cur + dH2_Cur + dH3_Cur);
		aGenTmpPos.Add(dH1_Cur + dH2_Cur + dH3_Cur + dH4_Cur);
	}
 	// 구간 내 있을 때
	else
	{
		GenStmp.RemoveAll();
		T_BTMP_STMP StmpD;
		if(InputData.nTempGrad==0)
		{
			// top
			aGenTmpPos.Add(0.0);
			aGenTmpPos.Add(dH1_Cur);
			aGenTmpPos.Add(dH1_Cur + dH2_Cur);
			aGenTmp.Add(InputData.dT1);
			aGenTmp.Add(InputData.dT2);
			aGenTmp.Add(0.0);
			for (int i = 0; i < aGenTmp.GetSize() - 1; i++)
			{
				StmpD.Initialize();
				StmpD.dH1 = aGenTmpPos[i];
				StmpD.dT1 = aGenTmp[i];
				StmpD.dH2 = aGenTmpPos[i+1];
				StmpD.dT2 = aGenTmp[i+1];
				StmpD.nRef = 0;

				GenStmp.Add(StmpD);
			}

			// bottom
			StmpD.Initialize();
			StmpD.dH2 = dH3_Cur;
			StmpD.dH1 = 0.0;
			StmpD.dT2 = 0.0;
			StmpD.dT1 = InputData.dT3;
			StmpD.nRef = 1;
			GenStmp.Add(StmpD);
		}
		else if(InputData.nTempGrad==1)
		{
			// top
			aGenTmpPos.Add(0.0);
			aGenTmpPos.Add(dH1_Cur);
			aGenTmpPos.Add(dH1_Cur + dH2_Cur);
			aGenTmp.Add(InputData.dT1);
			aGenTmp.Add(InputData.dT2);
			aGenTmp.Add(0.0);
			for (int i = 0; i < aGenTmp.GetSize() - 1; i++)
			{
				StmpD.Initialize();
				StmpD.dH1 = aGenTmpPos[i];
				StmpD.dT1 = aGenTmp[i];
				StmpD.dH2 = aGenTmpPos[i+1];
				StmpD.dT2 = aGenTmp[i+1];
				StmpD.nRef = 0;

				GenStmp.Add(StmpD);
			}
			
			// bottom
			StmpD.Initialize();
			StmpD.dH2 = dH4_Cur + dH3_Cur;
			StmpD.dH1 = dH4_Cur;
			StmpD.dT2 = 0.0;
			StmpD.dT1 = InputData.dT3;
			StmpD.nRef = 1;
			GenStmp.Add(StmpD);

			StmpD.Initialize();
			StmpD.dH2 = dH4_Cur;
			StmpD.dH1 = 0;
			StmpD.dT2 = InputData.dT3;
			StmpD.dT1 = InputData.dT4;
			StmpD.nRef = 1;
			GenStmp.Add(StmpD);
		}

		for (int i = 0; i < GenStmp.GetSize(); i++)
		{
			GenStmp[i].nMaterial = 1;
			GenStmp[i].nB = 0;
			GenStmp[i].nH1 = 3;
			GenStmp[i].nH2 = 3;
		}

		T_BTMP_STMP BlankBtmp;
		BlankBtmp.Initialize();
		BlankBtmp.nMaterial = 0;
		int nAddNum = D_BTMP_NUMSTMP - GenStmp.GetSize();
		for (int i = 0; i < nAddNum; i++)
		{
			GenStmp.Add(BlankBtmp);
		}

		return TRUE;
	}

	ASSERT(aGenTmp.GetSize() == aGenTmpPos.GetSize());  

	GenStmp.SetSize(aGenTmp.GetSize() - 1);
	for(int i = 0; i < aGenTmp.GetSize() - 1; i++)
	{
		GenStmp[i].Initialize();

		GenStmp[i].nMaterial = 1;
		GenStmp[i].nRef = 0;
		GenStmp[i].nB = 0;
		GenStmp[i].nH1 = 3;
		GenStmp[i].nH2 = 3;

		GenStmp[i].dH1 = aGenTmpPos[i];
		GenStmp[i].dT1 = aGenTmp[i];
		GenStmp[i].dH2 = aGenTmpPos[i + 1];
		GenStmp[i].dT2 = aGenTmp[i + 1];
	}

	T_BTMP_STMP BlankBtmp;
	BlankBtmp.Initialize();
	BlankBtmp.nMaterial = 0;

	int nAddNum = D_BTMP_NUMSTMP - GenStmp.GetSize();
	for(int i = 0; i < nAddNum; i++)
	{
		GenStmp.Add(BlankBtmp);
	}

	return TRUE;
}

BOOL CStldBtmpDefCodeProvUtil::MakeStmpEurocodeStlg( BTMP_DEF_CODE_PROV_EUROCODE_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();
	CString strTemp;

	GenStmp.RemoveAll();
	int nStype;

	if(SectD.nStype==D_SECT_TYPE_TAPERED)
	{
		nStype = SectD.SectBefore.nStype;
	}
	else
	{
		nStype = SectD.nStype;
	}

	if(nStype != D_SECT_TYPE_COMPO_B      && nStype != D_SECT_TYPE_COMPO_I      && nStype != D_SECT_TYPE_COMPO_TUB && 
	   nStype != D_SECT_TYPE_COMPO_STLG_B && nStype != D_SECT_TYPE_COMPO_STLG_I && nStype != D_SECT_TYPE_COMPO_STLG_TUB)
	{
		CString strTemp;
		strTemp.Format(_LS(IDS_DB_WARNING_BTMP_IRG_PSC_COMPO_SECT), ElemK);
		aErrorMsg.Add(strTemp);
		//ASSERT(0);
		return FALSE;
	}

	double dDimhi, dDimbi, dDimhj, dDimbj;
	if(!pDoc->m_pAttrCtrl->GetSectDimen(SectD, dDimhi, dDimbi, dDimhj, dDimbj, NULL, FALSE)) return FALSE;

	//////////////////////////////////////////////////////////////////////////

	// Tapered일때는 이전에 중간단면으로 넘겨줬기 때문에 무조건 I단으로 계산하면 된다.
	double dTc = SectD.SectAfter.SectJ.Size[1];

 	double d040m = M_InitValueCurUnit(0.40, N, M, D_UNITSYS_BASE_LENGTH);	

	double dH0 = M_InitValueCurUnit(InputData.dH0, N, M, D_UNITSYS_BASE_LENGTH);	
	double dH1 = M_InitValueCurUnit(InputData.dH1, N, M, D_UNITSYS_BASE_LENGTH);	
	double dH2 = M_InitValueCurUnit(InputData.dH2, N, M, D_UNITSYS_BASE_LENGTH);	

	double dH0_Auto = dTc;
	double dH0_Cur = 0.0;
	if (InputData.nProcedure == 1) // Simple
	{	
		dH0_Cur = dTc;
	}
	else
	{
		dH0_Cur = (InputData.dH0 == 0) ? dH0_Auto : dH0;
	}

	double dH1_Auto = 0.6*dH0_Cur;
	double dH1_Cur = (InputData.dH1==0)? dH1_Auto : dH1;

	double dH2_Auto = d040m;
	double dH2_Cur = (InputData.dH2==0)? dH2_Auto : dH2;

	if(InputData.nProcedure==0) 
	{
		if(dH1_Cur > dH0_Cur)
		{
			strTemp.Format(_LS(IDS_DB_ERROR_BTMP_H_H1), ElemK);
			aErrorMsg.Add(strTemp);
			return FALSE;
		}
	}

	double dT1 = InputData.dT1;
	double dT2 = InputData.dT2;
	double dT3 = InputData.dT3;
	double dT4 = InputData.dT4;

	double dH_Sum =  dH0_Cur + dH2_Cur;
	if     (InputData.nProcedure==0) dH_Sum =  dH0_Cur + dH2_Cur;
	else if(InputData.nProcedure==1) dH_Sum =  dH0_Cur;

	if(dDimhi < dH_Sum)
	{		
		strTemp.Format(_LS(IDS_DB_ERROR_BTMP_HEIGHT_SUM), ElemK);
		aErrorMsg.Add(strTemp);
		return FALSE;
	}

	CArray<double, double> aGenTmp;
	CArray<double, double> aGenTmpPos;

	if(InputData.nProcedure==0) // Normal
	{
		if(InputData.nTempGrad==0) // Heat
		{
			aGenTmp.Add(InputData.dT1);
			aGenTmp.Add(InputData.dT2);
			aGenTmp.Add(0.0);

			aGenTmpPos.Add(0.0);
			aGenTmpPos.Add(dH1_Cur);
			aGenTmpPos.Add(dH0_Cur + dH2_Cur);
		}
		else if(InputData.nTempGrad==1) // Cool
		{
			// 경계
			if(fabs(dDimhi - dH0_Cur - dH2_Cur) < dZero)
			{
				aGenTmp.Add(InputData.dT1);
				aGenTmp.Add(0.0);
				aGenTmp.Add(0.0);
				aGenTmp.Add(InputData.dT2);

				aGenTmpPos.Add(0.0);
				aGenTmpPos.Add(dH1_Cur);
				aGenTmpPos.Add(dH0_Cur);
				aGenTmpPos.Add(dH0_Cur + dH2_Cur);
			}
			else
			{
				aGenTmp.Add(InputData.dT1);
				aGenTmp.Add(0.0);
				aGenTmp.Add(0.0);
				aGenTmp.Add(InputData.dT2);
				aGenTmp.Add(InputData.dT2);

				aGenTmpPos.Add(0.0);
				aGenTmpPos.Add(dH1_Cur);
				aGenTmpPos.Add(dH0_Cur);
				aGenTmpPos.Add(dH0_Cur + dH2_Cur);
				aGenTmpPos.Add(dDimhi);
			}
		}
	}
	else if(InputData.nProcedure==1) // Simple
	{
		aGenTmp.Add(InputData.dT1);
		aGenTmp.Add(InputData.dT1);

		aGenTmpPos.Add(0.0);
		aGenTmpPos.Add(dH0_Cur);
	}

	ASSERT(aGenTmp.GetSize() == aGenTmpPos.GetSize());  

	GenStmp.SetSize(aGenTmp.GetSize() - 1);
	for(int i = 0; i < aGenTmp.GetSize() - 1; i++)
	{
		GenStmp[i].Initialize();

		GenStmp[i].nMaterial = 1;
		GenStmp[i].nRef = 0;
		GenStmp[i].nB = 0;
		GenStmp[i].nH1 = 3;
		GenStmp[i].nH2 = 3;

		GenStmp[i].dH1 = aGenTmpPos[i];
		GenStmp[i].dT1 = aGenTmp[i];
		GenStmp[i].dH2 = aGenTmpPos[i + 1];
		GenStmp[i].dT2 = aGenTmp[i + 1];
	}

	T_BTMP_STMP BlankBtmp;
	BlankBtmp.Initialize();
	BlankBtmp.nMaterial = 0;

	int nAddNum = D_BTMP_NUMSTMP - GenStmp.GetSize();
	for(int i = 0; i < nAddNum; i++)
	{
		GenStmp.Add(BlankBtmp);
	}

	return TRUE;
}

BOOL CStldBtmpDefCodeProvUtil::MakeStmpAs( BTMP_DEF_CODE_PROV_AS_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg)
{
	if(InputData.nSectType == 0 )
	{
		if(InputData.nTempGrad == 0)
			return MakeStmpAsPSCPostive(InputData, ElemK, SectD, nBtmpType, GenStmp, aErrorMsg);
		else if(InputData.nTempGrad == 1)
			return MakeStmpAsPSCNegative(InputData, ElemK, SectD, nBtmpType, GenStmp, aErrorMsg);

	}
	else if(InputData.nSectType == 1) 
	{
		if(InputData.nTempGrad == 0)
			return MakeStmpAsStlgPostive(InputData, ElemK, SectD, nBtmpType, GenStmp, aErrorMsg);
		else if(InputData.nTempGrad == 1)
			return MakeStmpAsStlgNegative(InputData, ElemK, SectD, nBtmpType, GenStmp, aErrorMsg);
	}
	ASSERT(0);

	return TRUE;
}

BOOL CStldBtmpDefCodeProvUtil::FuncAsPscPositive(double dTmax, double dZ, double d1200mm, double& dOut)
{
	dOut = dTmax * pow(1 - dZ/d1200mm , 5);

	return TRUE;
}
BOOL CStldBtmpDefCodeProvUtil::MakeStmpAsPSCPostive( BTMP_DEF_CODE_PROV_AS_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();
	CString strTemp;

	GenStmp.RemoveAll();
	int nStype;

	if(SectD.nStype==D_SECT_TYPE_TAPERED)
	{
		nStype = SectD.SectBefore.nStype;
	}
	else
	{
		nStype = SectD.nStype;
	}

	if(nStype != D_SECT_TYPE_PSC && 
		nStype != D_SECT_TYPE_COMPO_CI && nStype != D_SECT_TYPE_COMPO_CT && nStype != D_SECT_TYPE_COMPO_PC && nStype != D_SECT_TYPE_COMPO_G)
	{
		CString strTemp;
		strTemp.Format(_LS(IDS_DB_WARNING_BTMP_IRG_PSC_COMPO_SECT), ElemK);
		aErrorMsg.Add(strTemp);
		//ASSERT(0);
		return FALSE;
	}

	double dDimhi, dDimbi, dDimhj, dDimbj;
	if(!pDoc->m_pAttrCtrl->GetSectDimen(SectD, dDimhi, dDimbi, dDimhj, dDimbj, NULL, FALSE)) return FALSE;

	CArray<double, double> aGenTmp;
	CArray<double, double> aGenTmpPos;
	CArray<double, double> aH;
	CArray<double, double> aZ, aZDivBySect;

	double d200mm  = M_InitValueCurUnit( 200.0, N, MM, D_UNITSYS_BASE_LENGTH);
	double d300mm  = M_InitValueCurUnit( 300.0, N, MM, D_UNITSYS_BASE_LENGTH);
	double d1200mm = M_InitValueCurUnit(1200.0, N, MM, D_UNITSYS_BASE_LENGTH);

//	double dH1 = M_InitValueCurUnit(InputData.dH1, N, MM, D_UNITSYS_BASE_LENGTH);	
	double dH2 = M_InitValueCurUnit(InputData.dH2, N, MM, D_UNITSYS_BASE_LENGTH);	
	double dH2_Cur = /*(InputData.nH2==0)? d200mm :*/ dH2;
	double dT1 = InputData.dT1;
	double dT2 = InputData.dT2;

	if(dDimhi < d1200mm + dH2_Cur)
	{		 
		strTemp.Format(_LS(IDS_DB_WARNING_BTMP_AS_HEIGHT), ElemK);
		aErrorMsg.Add(strTemp);
	}

	int nDivNum = 10;
	double dZ = 0.0;
	double dEachTmp = 0.0;

	//////////////////////////////////////////////////////////////////////////
	// 경계
	if(fabs(dDimhi - d1200mm) < dZero)
	{
		double dDiv = d1200mm / nDivNum;
		for(int i = 0; i < nDivNum; i++)
		{
			dZ = i * dDiv;
			FuncAsPscPositive(dT1, dZ, d1200mm, dEachTmp);

			aGenTmp.Add(dEachTmp);
			aGenTmpPos.Add(dZ);
		}
		aGenTmp.Add(0.0);
		aGenTmpPos.Add(d1200mm);
	}
	else if(fabs(dDimhi - d1200mm - dH2_Cur) < dZero)
	{
		double dDiv = d1200mm / nDivNum;
		for(int i = 0; i < nDivNum; i++)
		{
			dZ = i * dDiv;
			FuncAsPscPositive(dT1, dZ, d1200mm, dEachTmp);

			aGenTmp.Add(dEachTmp);
			aGenTmpPos.Add(dZ);
		}
		aGenTmp.Add(0.0);
		aGenTmpPos.Add(d1200mm);

		aGenTmp.Add(dT2);
		aGenTmpPos.Add(d1200mm + dH2_Cur);
	}
	// 구간 내 있을 때
	else if(dDimhi < d1200mm)
	{
		double dDiv = dDimhi / nDivNum;
		for(int i = 0; i < nDivNum+1; i++)
		{
			dZ = i * dDiv;
			FuncAsPscPositive(dT1, dZ, d1200mm, dEachTmp);

			aGenTmp.Add(dEachTmp);
			aGenTmpPos.Add(dZ);
		}
	}
	else 
	{
		double dDiv = d1200mm / nDivNum;
		for(int i = 0; i < nDivNum; i++)
		{
			dZ = i * dDiv;
			FuncAsPscPositive(dT1, dZ, d1200mm, dEachTmp);

			aGenTmp.Add(dEachTmp);
			aGenTmpPos.Add(dZ);
		}
		aGenTmp.Add(0.0);
		aGenTmpPos.Add(d1200mm);

		if(dH2_Cur > 0.0)
		{
			aGenTmp.Add(0.0);
			aGenTmpPos.Add(dDimhi - dH2_Cur);

			aGenTmp.Add(dT2);
			aGenTmpPos.Add(dDimhi);
		}
	}

	ASSERT(aGenTmp.GetSize() == aGenTmpPos.GetSize());  
	//////////////////////////////////////////////////////////////////////////

	GenStmp.SetSize(aGenTmp.GetSize() - 1);
	for(int i = 0; i < aGenTmp.GetSize() - 1; i++)
	{
		GenStmp[i].Initialize();

		GenStmp[i].nMaterial = 1;
		GenStmp[i].nRef = 0;
		GenStmp[i].nB = 0;
		GenStmp[i].nH1 = 3;
		GenStmp[i].nH2 = 3;

		GenStmp[i].dH1 = aGenTmpPos[i];
		GenStmp[i].dT1 = aGenTmp[i];
		GenStmp[i].dH2 = aGenTmpPos[i + 1];
		GenStmp[i].dT2 = aGenTmp[i + 1];
	}

	T_BTMP_STMP BlankBtmp;
	BlankBtmp.Initialize();
	BlankBtmp.nMaterial = 0;

	int nAddNum = D_BTMP_NUMSTMP - GenStmp.GetSize();
	for(int i = 0; i < nAddNum; i++)
	{
		GenStmp.Add(BlankBtmp);
	}

	return TRUE;
}

BOOL CStldBtmpDefCodeProvUtil::MakeStmpAsPSCNegative( BTMP_DEF_CODE_PROV_AS_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg)
{

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();
	CString strTemp;

	GenStmp.RemoveAll();
	int nStype;

	if(SectD.nStype==D_SECT_TYPE_TAPERED)
	{
		nStype = SectD.SectBefore.nStype;
	}
	else
	{
		nStype = SectD.nStype;
	}

	if(nStype != D_SECT_TYPE_PSC && 
		nStype != D_SECT_TYPE_COMPO_CI && nStype != D_SECT_TYPE_COMPO_CT && nStype != D_SECT_TYPE_COMPO_PC && nStype != D_SECT_TYPE_COMPO_G)
	{
		CString strTemp;
		strTemp.Format(_LS(IDS_DB_WARNING_BTMP_IRG_PSC_COMPO_SECT), ElemK);
		aErrorMsg.Add(strTemp);
		//ASSERT(0);
		return FALSE;
	}

	double dDimhi, dDimbi, dDimhj, dDimbj;
	if(!pDoc->m_pAttrCtrl->GetSectDimen(SectD, dDimhi, dDimbi, dDimhj, dDimbj, NULL, FALSE)) return FALSE;

	CArray<double, double> aGenTmp;
	CArray<double, double> aGenTmpPos;
	CArray<double, double> aH;
	CArray<double, double> aZ, aZDivBySect;

	double d200mm  = M_InitValueCurUnit( 200.0, N, MM, D_UNITSYS_BASE_LENGTH);
	double d300mm  = M_InitValueCurUnit( 300.0, N, MM, D_UNITSYS_BASE_LENGTH);
	double d1200mm = M_InitValueCurUnit(1200.0, N, MM, D_UNITSYS_BASE_LENGTH);

	double dH1 = M_InitValueCurUnit(InputData.dH1, N, MM, D_UNITSYS_BASE_LENGTH);	
	//double dH2 = M_InitValueCurUnit(InputData.dH2, N, MM, D_UNITSYS_BASE_LENGTH);	 사용안함.
	double dH1_Cur = /*(InputData.nH1==0)? d300mm :*/ dH1;
	double dT1 = -1.0*InputData.dT1; //InputData.T1>0

	if(dDimhi < dH1_Cur)
	{		
		strTemp.Format(_LS(IDS_DB_ERROR_BTMP_HEIGHT_SUM), ElemK);
		aErrorMsg.Add(strTemp);
	}

	//////////////////////////////////////////////////////////////////////////
	aGenTmp.Add(0.4*dT1);
	aGenTmp.Add(0.0);

	aGenTmpPos.Add(0.0);
	aGenTmpPos.Add(dH1_Cur);

	ASSERT(aGenTmp.GetSize() == aGenTmpPos.GetSize());  
	//////////////////////////////////////////////////////////////////////////

	GenStmp.SetSize(aGenTmp.GetSize() - 1);
	for(int i = 0; i < aGenTmp.GetSize() - 1; i++)
	{
		GenStmp[i].Initialize();

		GenStmp[i].nMaterial = 1;
		GenStmp[i].nRef = 0;
		GenStmp[i].nB = 0;
		GenStmp[i].nH1 = 3;
		GenStmp[i].nH2 = 3;

		GenStmp[i].dH1 = aGenTmpPos[i];
		GenStmp[i].dT1 = aGenTmp[i];
		GenStmp[i].dH2 = aGenTmpPos[i + 1];
		GenStmp[i].dT2 = aGenTmp[i + 1];
	}

	T_BTMP_STMP BlankBtmp;
	BlankBtmp.Initialize();
	BlankBtmp.nMaterial = 0;

	int nAddNum = D_BTMP_NUMSTMP - GenStmp.GetSize();
	for(int i = 0; i < nAddNum; i++)
	{
		GenStmp.Add(BlankBtmp);
	}

	return TRUE;
}

BOOL CStldBtmpDefCodeProvUtil::MakeStmpAsStlgPostive( BTMP_DEF_CODE_PROV_AS_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();
	CString strTemp;

	GenStmp.RemoveAll();
	int nStype;

	if(SectD.nStype==D_SECT_TYPE_TAPERED)
	{
		nStype = SectD.SectBefore.nStype;
	}
	else
	{
		nStype = SectD.nStype;
	}

	if(nStype != D_SECT_TYPE_COMPO_B      && nStype != D_SECT_TYPE_COMPO_I      && nStype != D_SECT_TYPE_COMPO_TUB && 
      nStype != D_SECT_TYPE_COMPO_STLG_B && nStype != D_SECT_TYPE_COMPO_STLG_I && nStype != D_SECT_TYPE_COMPO_STLG_TUB)
	{
		CString strTemp;
		strTemp.Format(_LS(IDS_DB_WARNING_BTMP_IRG_PSC_COMPO_SECT), ElemK);
		aErrorMsg.Add(strTemp);
		//ASSERT(0);
		return FALSE;
	}

	double dDimhi, dDimbi, dDimhj, dDimbj;
	if(!pDoc->m_pAttrCtrl->GetSectDimen(SectD, dDimhi, dDimbi, dDimhj, dDimbj, NULL, FALSE)) return FALSE;

	CArray<double, double> aGenTmp;
	CArray<double, double> aGenTmpPos;
	CArray<double, double> aH;
	CArray<double, double> aZ, aZDivBySect;

	double d300mm  = M_InitValueCurUnit( 300.0, N, MM, D_UNITSYS_BASE_LENGTH);
	double d1200mm = M_InitValueCurUnit(1200.0, N, MM, D_UNITSYS_BASE_LENGTH);

	//	double dH1 = M_InitValueCurUnit(InputData.dH1, N, MM, D_UNITSYS_BASE_LENGTH);	
	double dH2 = M_InitValueCurUnit(InputData.dH2, N, MM, D_UNITSYS_BASE_LENGTH);	
	double dH2_Cur = (InputData.nH2==0)? d300mm : dH2;
	double dT1 = InputData.dT1;
	double dT2 = InputData.dT2;

	// Tapered일때는 이전에 중간단면으로 넘겨줬기 때문에 무조건 I단으로 계산하면 된다.
	double dTc = SectD.SectAfter.SectJ.Size[1];
	double dSmallTCurUnit = 0.0;
	if(InputData.nSmallT==0)  dSmallTCurUnit = dTc;
	else                      dSmallTCurUnit = M_InitValueCurUnit(InputData.dSmallT, N, MM, D_UNITSYS_BASE_LENGTH);	

	if(dDimhi < dSmallTCurUnit)
	{		
		strTemp.Format(_LS(IDS_DB_WARNING_BTMP_AS_HEIGHT), ElemK);
		aErrorMsg.Add(strTemp);
	}

	int nDivNum = 10;
	double dZ = 0.0;
	double dEachTmp = 0.0;

	//////////////////////////////////////////////////////////////////////////
	// 경계
	if(fabs(dSmallTCurUnit - d1200mm) < dZero)
	{
		double dDiv = d1200mm / nDivNum;
		for(int i = 0; i < nDivNum; i++)
		{
			dZ = i * dDiv;
			FuncAsPscPositive(dT1+5, dZ, d1200mm, dEachTmp);

			aGenTmp.Add(dEachTmp);
			aGenTmpPos.Add(dZ);
		}
		aGenTmp.Add(0.0);
		aGenTmpPos.Add(d1200mm);
	}
	// 구간 내 있을 때
	else 
	{
		double dDiv = dSmallTCurUnit / nDivNum;
		for(int i = 0; i < nDivNum+1; i++)
		{
			dZ = i * dDiv;
			FuncAsPscPositive(dT1+5, dZ, d1200mm, dEachTmp);

			aGenTmp.Add(dEachTmp);
			aGenTmpPos.Add(dZ);
		}

		aGenTmp.Add(dEachTmp);
		aGenTmpPos.Add(dDimhi);
	}

	ASSERT(aGenTmp.GetSize() == aGenTmpPos.GetSize());  
	//////////////////////////////////////////////////////////////////////////

	GenStmp.SetSize(aGenTmp.GetSize() - 1);
	for(int i = 0; i < aGenTmp.GetSize() - 1; i++)
	{
		GenStmp[i].Initialize();

		GenStmp[i].nMaterial = 1;
		GenStmp[i].nRef = 0;
		GenStmp[i].nB = 0;
		GenStmp[i].nH1 = 3;
		GenStmp[i].nH2 = 3;

		GenStmp[i].dH1 = aGenTmpPos[i];
		GenStmp[i].dT1 = aGenTmp[i];
		GenStmp[i].dH2 = aGenTmpPos[i + 1];
		GenStmp[i].dT2 = aGenTmp[i + 1];
	}

	T_BTMP_STMP BlankBtmp;
	BlankBtmp.Initialize();
	BlankBtmp.nMaterial = 0;

	int nAddNum = D_BTMP_NUMSTMP - GenStmp.GetSize();
	for(int i = 0; i < nAddNum; i++)
	{
		GenStmp.Add(BlankBtmp);
	}

	return TRUE;
}
BOOL CStldBtmpDefCodeProvUtil::MakeStmpAsStlgNegative( BTMP_DEF_CODE_PROV_AS_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();
	CString strTemp;

	GenStmp.RemoveAll();
	int nStype;

	if(SectD.nStype==D_SECT_TYPE_TAPERED)
	{
		nStype = SectD.SectBefore.nStype;
	}
	else
	{
		nStype = SectD.nStype;
	}

	if(nStype != D_SECT_TYPE_COMPO_B      && nStype != D_SECT_TYPE_COMPO_I      && nStype != D_SECT_TYPE_COMPO_TUB && 
       nStype != D_SECT_TYPE_COMPO_STLG_B && nStype != D_SECT_TYPE_COMPO_STLG_I && nStype != D_SECT_TYPE_COMPO_STLG_TUB)
	{
		CString strTemp;
		strTemp.Format(_LS(IDS_DB_WARNING_BTMP_IRG_PSC_COMPO_SECT), ElemK);
		aErrorMsg.Add(strTemp);
		//ASSERT(0);
		return FALSE;
	}

	double dDimhi, dDimbi, dDimhj, dDimbj;
	if(!pDoc->m_pAttrCtrl->GetSectDimen(SectD, dDimhi, dDimbi, dDimhj, dDimbj, NULL, FALSE)) return FALSE;

	CArray<double, double> aGenTmp;
	CArray<double, double> aGenTmpPos;
	CArray<double, double> aH;
	CArray<double, double> aZ, aZDivBySect;

	double d300mm  = M_InitValueCurUnit( 300.0, N, MM, D_UNITSYS_BASE_LENGTH);

	// Tapered일때는 이전에 중간단면으로 넘겨줬기 때문에 무조건 I단으로 계산하면 된다.
	double dTc = SectD.SectAfter.SectJ.Size[1];
	double dSmallTCurUnit = 0.0;
	if(InputData.nSmallT==0)  dSmallTCurUnit = min(dTc, d300mm);
	else                      dSmallTCurUnit = M_InitValueCurUnit(InputData.dSmallT, N, MM, D_UNITSYS_BASE_LENGTH);	

	double dT1 = InputData.dT1;  //InputData.dT1>0
	double dT1Cal = (-1)*(0.4*dT1 - 3);
	if(dDimhi < dSmallTCurUnit)
	{		
		strTemp.Format(_LS(IDS_DB_ERROR_BTMP_HEIGHT_SUM), ElemK);
		aErrorMsg.Add(strTemp);
	}

	//////////////////////////////////////////////////////////////////////////
	// 경계
	if(fabs(dSmallTCurUnit - d300mm) < dZero)
	{
		aGenTmp.Add(dT1Cal);
		aGenTmpPos.Add(0.0);

		aGenTmp.Add(0.0);
		aGenTmpPos.Add(d300mm);
	}
	// 구간 내 있을 때
	else
	{
		double dFS     = (0.0 - dT1Cal)/d300mm;
		double dT_mid = dT1Cal + dFS*(dSmallTCurUnit);

		aGenTmp.Add(dT1Cal);
		aGenTmpPos.Add(0.0);

		aGenTmp.Add(dT_mid);
		aGenTmpPos.Add(dSmallTCurUnit);

		aGenTmp.Add(dT_mid);
		aGenTmpPos.Add(dDimhi);
	}

	ASSERT(aGenTmp.GetSize() == aGenTmpPos.GetSize());  
	//////////////////////////////////////////////////////////////////////////

	GenStmp.SetSize(aGenTmp.GetSize() - 1);
	for(int i = 0; i < aGenTmp.GetSize() - 1; i++)
	{
		GenStmp[i].Initialize();

		GenStmp[i].nMaterial = 1;
		GenStmp[i].nRef = 0;
		GenStmp[i].nB = 0;
		GenStmp[i].nH1 = 3;
		GenStmp[i].nH2 = 3;

		GenStmp[i].dH1 = aGenTmpPos[i];
		GenStmp[i].dT1 = aGenTmp[i];
		GenStmp[i].dH2 = aGenTmpPos[i + 1];
		GenStmp[i].dT2 = aGenTmp[i + 1];
	}

	T_BTMP_STMP BlankBtmp;
	BlankBtmp.Initialize();
	BlankBtmp.nMaterial = 0;

	int nAddNum = D_BTMP_NUMSTMP - GenStmp.GetSize();
	for(int i = 0; i < nAddNum; i++)
	{
		GenStmp.Add(BlankBtmp);
	}

	return TRUE;
}

