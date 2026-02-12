#include "stdafx.h"
#include "wg_cmd.h"

#include "..\wg_db\VehlDef.h"

#include "CMMvhlStdEuroBSNewTempForRefactor.h"

int CMMvhlStdEuroBSNewTempForRefactor::GetGridListSize(T_MVHL_D &m_Data, int nIndex, BOOL bPatchType/*=FALSE*/)
{
	int nListSize = 0;
	int nbPatchTypeSize = 0;

	switch (m_Data.nStandardCode)
	{
	case D_MVHL_EURO_EN1991_2_2003_ROAD_BRG:
	case D_MVHL_EURO_EN1991_2_2003_FOOTWAY:
	case D_MVHL_EURO_EN1991_2_2003_ROAD_BRG_FTG:
	case D_MVHL_EURO_EN1991_2_2003_RAIL:
		if(nIndex == 0) // First Grid
		{
			if(m_Data.VehicleTypeName == _T("Load Model 3"))
			{
				nListSize = GetGridListSizeEuroModel3(m_Data, nListSize);
			}
			else if(m_Data.VehicleTypeName == _T("Load Model 3 (UK NA)"))
			{
				nListSize = GetGridListSizeSvSov(m_Data, nListSize);

			}
			else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 2 (280)"))         {nListSize = 2; nbPatchTypeSize = 2;}
			else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 2 (360)"))         {nListSize = 3; nbPatchTypeSize = 2;}
			else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 2 (630)"))         {nListSize = 5; nbPatchTypeSize = 3;}
			else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 2 (560)"))         {nListSize = 4; nbPatchTypeSize = 2;}
			else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 2 (610)"))         {nListSize = 5; nbPatchTypeSize = 3;}
			else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 3 (One Vehicle)"))  nListSize = 4;
			else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 3 (Two Vehicle)"))  nListSize = 4;
			else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 4 (200)"))         {nListSize = 2; nbPatchTypeSize = 2;} 
			else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 4 (310)"))         {nListSize = 3; nbPatchTypeSize = 2;}
			else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 4 (490)"))         {nListSize = 5; nbPatchTypeSize = 3;}
			else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 4 (390)"))         {nListSize = 4; nbPatchTypeSize = 2;}
			else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 4 (450)"))         {nListSize = 5; nbPatchTypeSize = 3;}		
			else nListSize = 0;

			if (bPatchType)
				nListSize = nbPatchTypeSize;

		}
		else if(nIndex == 1) // Second Grid
		{
			if(m_Data.VehicleTypeName == _T("Load Model 3"))
			{
				if     (m_Data.SelVehicle == _T("1200/150/200")) nListSize = 6;
				else if(m_Data.SelVehicle == _T("1500/150/200")) nListSize = 8;
				else if(m_Data.SelVehicle == _T("1800/150/200")) nListSize = 9;
				else if(m_Data.SelVehicle == _T("2400/200"))     nListSize = 12;
				else if(m_Data.SelVehicle == _T("3000/200"))     nListSize = 15;
				else if(m_Data.SelVehicle == _T("3600/200"))     nListSize = 18;
				else nListSize = 0;
			}
			else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 3 (Two Vehicle)")) nListSize = 4;
			else nListSize = 0;
		}
		else if(nIndex == 2) // Third Grid
		{
			// Nothing
		}
		else 
		{
			ASSERT(0);
		}
		break;
	case D_MVHL_BS_BD8611:
		if(nIndex == 0) // First Grid
		{
			nListSize = GetGridListSizeSvSov(m_Data, nListSize);
		}
		else if(nIndex == 1) // Second Grid
		{
			// Nothing
		}
		else if(nIndex == 2) // Third Grid
		{
			// Nothing
		}
		else 
		{
			ASSERT(0);
		}
		break;
	case D_MVHL_BS_CS454:
		if(nIndex == 0) // All Model 1
		{
			nListSize = GetGridListSizeCS454AllModel1(m_Data, nListSize);
		}
		else if(nIndex == 1) // All Model 2
		{
			// Nothing
		}
		else 
		{
			ASSERT(0);
		}
		break;
	case D_MVHL_BS_NETWORK_RAIL_STANDARDS:
		if (nIndex == 0) // First Grid
		{
			nListSize = GetGridListSizeNetworkRailStandards(m_Data, nListSize);
		}
		else if (nIndex == 1) // Second Grid
		{
			nListSize = GetGridListSizeNetworkRailStandards2(m_Data, nListSize);
		}
		else
		{
			ASSERT(0);
		}
		break;
	default:
		nListSize = 0;
	}

	return nListSize;
}

CString CMMvhlStdEuroBSNewTempForRefactor::GetDescListTxt( T_MVHL_D &m_Data, int nIndex )
{
	CString strDescTxt;

	switch (m_Data.nStandardCode)
	{
	case D_MVHL_EURO_EN1991_2_2003_ROAD_BRG:
	case D_MVHL_EURO_EN1991_2_2003_FOOTWAY:
	case D_MVHL_EURO_EN1991_2_2003_ROAD_BRG_FTG:
	case D_MVHL_EURO_EN1991_2_2003_RAIL:
		if(nIndex == 0) // First Grid Txt
		{
			if(m_Data.VehicleTypeName == _T("Load Model 3 (UK NA)"))
			{
				strDescTxt = GetDescListTxtSvSov(m_Data, strDescTxt);
			}
			else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 2 (280)")) strDescTxt = _T("* 280");
			else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 2 (360)")) strDescTxt = _T("* 360");
			else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 2 (630)")) strDescTxt = _T("* 630");
			else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 2 (560)")) strDescTxt = _T("* 560");
			else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 2 (610)")) strDescTxt = _T("* 610");
			else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 3 (One Vehicle)")) strDescTxt = _T("One Vehicle");
			else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 3 (Two Vehicle)")) strDescTxt = _T("First Vehicle");
			else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 4 (200)")) strDescTxt = _T("* 200");
			else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 4 (310)")) strDescTxt = _T("* 310");
			else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 4 (490)")) strDescTxt = _T("* 490");
			else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 4 (390)")) strDescTxt = _T("* 390");
			else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 4 (450)")) strDescTxt = _T("* 450");		
			else strDescTxt = _T("");
		}
		else if(nIndex == 1) // Second Grid Txt
		{
			if(m_Data.VehicleTypeName == _T("Fatigue Load Model 3 (Two Vehicle)")) strDescTxt = _T("Second Vehicle");
		}
		else if(nIndex == 2) // Third Grid Txt
		{
			// Nothing
		}
		else 
		{
			ASSERT(0);
		}
		break;
	case D_MVHL_BS_BD8611:
		if(nIndex == 0) // First Grid Txt
		{
			strDescTxt = GetDescListTxtSvSov(m_Data, strDescTxt);
		}
		else if(nIndex == 1) // Second Grid Txt
		{
			// Nothing
		}
		else if(nIndex == 2) // Third Grid Txt
		{
			// Nothing
		}
		else 
		{
			ASSERT(0);
		}
		break;
	case D_MVHL_BS_CS454:
		if(nIndex == 0) // All Model 1		
		{
			strDescTxt = GetDescListTxtCS454AllModel1(m_Data, strDescTxt);
		}
		else if(nIndex == 1) // All Model 2
		{
			// Nothing
		}
		else 
		{
			ASSERT(0);
		}
		break;
	case D_MVHL_BS_NETWORK_RAIL_STANDARDS:
		if (nIndex == 0) // All Model 1		
		{
			strDescTxt = GetDescListTxtNetworkRailStandards(m_Data, strDescTxt);
		}
		else if (nIndex == 1) // All Model 2
		{
			strDescTxt = GetDescListTxtNetworkRailStandards2(m_Data, strDescTxt);
		}
		else
		{
			ASSERT(0);
		}
		break;
	default:
		strDescTxt = _T("");
		break;
	}


	return strDescTxt;
}

CString CMMvhlStdEuroBSNewTempForRefactor::GetDescListChk( T_MVHL_D &m_Data, int nIndex )
{
	CString strDescChk;

	if(nIndex == 0) // First Grid Chk
	{
		if(m_Data.VehicleTypeName == _T("Load Model 3"))
		{
			if     (m_Data.SelVehicle == _T("600/150"))      strDescChk = _T("600/150");
			else if(m_Data.SelVehicle == _T("900/150"))      strDescChk = _T("900/150");
			else if(m_Data.SelVehicle == _T("1200/150/200")) strDescChk = _T("1200/150");
			else if(m_Data.SelVehicle == _T("1500/150/200")) strDescChk = _T("1500/150");
			else if(m_Data.SelVehicle == _T("1800/150/200")) strDescChk = _T("1800/150");
			else if(m_Data.SelVehicle == _T("2400/200"))     strDescChk = _T("2400/200");
			else if(m_Data.SelVehicle == _T("3000/200"))     strDescChk = _T("3000/200");
			else if(m_Data.SelVehicle == _T("3600/200"))     strDescChk = _T("3600/200");
			else strDescChk = _T("");
		}
	}
	else if(nIndex == 1) // Second Grid Chk
	{
		if(m_Data.VehicleTypeName == _T("Load Model 3"))
		{
			if     (m_Data.SelVehicle == _T("1200/150/200")) strDescChk = _T("1200/200");
			else if(m_Data.SelVehicle == _T("1500/150/200")) strDescChk = _T("1500/200");
			else if(m_Data.SelVehicle == _T("1800/150/200")) strDescChk = _T("1800/200");
			else if(m_Data.SelVehicle == _T("2400/200"))     strDescChk = _T("2400/200/200");
			else if(m_Data.SelVehicle == _T("3000/200"))     strDescChk = _T("3000/200/200");
			else if(m_Data.SelVehicle == _T("3600/200"))     strDescChk = _T("3600/200/200");
			else strDescChk = _T("");
		}
	}
	else if(nIndex == 1) // Second Grid Chk
	{
		// Nothing
	}
	else if(nIndex == 2) // Third Grid Chk
	{
		// Nothing
	}
	else 
	{
		ASSERT(0);
	}

	return strDescChk;
}

CString CMMvhlStdEuroBSNewTempForRefactor::GetDescListTxtSvSov( T_MVHL_D &m_Data, CString strDescTxt )
{
	if     (m_Data.VehicleTypeName == _T("SV 80"))     strDescTxt = _T("* SV 80");
	else if(m_Data.VehicleTypeName == _T("SV 100"))    strDescTxt = _T("* SV 100");
	else if(m_Data.VehicleTypeName == _T("SV 150"))    strDescTxt = _T("* SV 150");
	else if(m_Data.VehicleTypeName == _T("SV 196"))    strDescTxt = _T("* SV 196");
	else if(m_Data.VehicleTypeName == _T("SV-Train"))  strDescTxt = _T("* SV-Train");
	else if(m_Data.VehicleTypeName == _T("SV TT"))     strDescTxt = _T("* SV TT");
	else if(m_Data.VehicleTypeName == _T("SOV 250"))   strDescTxt = _T("* SOV-250");
	else if(m_Data.VehicleTypeName == _T("SOV 350"))   strDescTxt = _T("* SOV-350");
	else if(m_Data.VehicleTypeName == _T("SOV 450"))   strDescTxt = _T("* SOV-450");
	else if(m_Data.VehicleTypeName == _T("SOV 600"))   strDescTxt = _T("* SOV-600");
	else strDescTxt = _T("");	return strDescTxt;
}

CString CMMvhlStdEuroBSNewTempForRefactor::GetDescListTxtCS454AllModel1( T_MVHL_D &m_Data, CString strDescTxt )
{
	if     (m_Data.SelVehicle == _T("A-4AXLE")  )   strDescTxt = _T("* A-4AXLE")  ;
	else if(m_Data.SelVehicle == _T("B-4AXLE")  )   strDescTxt = _T("* B-4AXLE")  ;
	else if(m_Data.SelVehicle == _T("C-5AXLE")  )   strDescTxt = _T("* C-5AXLE")  ;
	else if(m_Data.SelVehicle == _T("D-5AXLE_1"))   strDescTxt = _T("* D-5AXLE_1");
	else if(m_Data.SelVehicle == _T("D-5AXLE_2"))   strDescTxt = _T("* D-5AXLE_2");
	else if(m_Data.SelVehicle == _T("E-5AXLE_1"))   strDescTxt = _T("* E-5AXLE_1");
	else if(m_Data.SelVehicle == _T("E-5AXLE_2"))   strDescTxt = _T("* E-5AXLE_2");
	else if(m_Data.SelVehicle == _T("F-6AXLE_1"))   strDescTxt = _T("* F-6AXLE_1");
	else if(m_Data.SelVehicle == _T("F-6AXLE_2"))   strDescTxt = _T("* F-6AXLE_2");
	else if(m_Data.SelVehicle == _T("G-6AXLE_1"))   strDescTxt = _T("* G-6AXLE_1");
	else if(m_Data.SelVehicle == _T("G-6AXLE_2"))   strDescTxt = _T("* G-6AXLE_2");
	else if(m_Data.SelVehicle == _T("H-5AXLE_1"))   strDescTxt = _T("* H-5AXLE_1");
	else if(m_Data.SelVehicle == _T("H-5AXLE_2"))   strDescTxt = _T("* H-5AXLE_2");
	else if(m_Data.SelVehicle == _T("I-3AXLE")  )   strDescTxt = _T("* I-3AXLE")  ;
	else if(m_Data.SelVehicle == _T("J-3AXLE")  )   strDescTxt = _T("* J-3AXLE")  ;
	else if(m_Data.SelVehicle == _T("K-3AXLE_1"))   strDescTxt = _T("* K-3AXLE_1");
	else if(m_Data.SelVehicle == _T("K-3AXLE_2"))   strDescTxt = _T("* K-3AXLE_2");
	else if(m_Data.SelVehicle == _T("L-3AXLE_1"))   strDescTxt = _T("* L-3AXLE_1");
	else if(m_Data.SelVehicle == _T("L-3AXLE_2"))   strDescTxt = _T("* L-3AXLE_2");
	else if(m_Data.SelVehicle == _T("M-2AXLE")  )   strDescTxt = _T("* M-2AXLE")  ;
	else if(m_Data.SelVehicle == _T("N-2AXLE")  )   strDescTxt = _T("* N-2AXLE")  ;
	else if(m_Data.SelVehicle == _T("O-2AXLE")  )   strDescTxt = _T("* O-2AXLE")  ;
	else strDescTxt = _T("");	return strDescTxt;
}


CString CMMvhlStdEuroBSNewTempForRefactor::GetDescListTxtNetworkRailStandards(T_MVHL_D& m_Data, CString strDescTxt)
{
	if (m_Data.VehicleTypeName == _T("Type RAI Loading"))  strDescTxt = _T("* RA Loading");
	else if (m_Data.VehicleTypeName == _T("Wagon"))  strDescTxt = _T("* Wagon");
	else strDescTxt = _T("");	return strDescTxt;
}

CString CMMvhlStdEuroBSNewTempForRefactor::GetDescListTxtNetworkRailStandards2(T_MVHL_D& m_Data, CString strDescTxt)
{
	if (m_Data.VehicleTypeName == _T("Type RAI Loading"))  strDescTxt = _T("* Short length");
	else strDescTxt = _T("");	return strDescTxt;
}

int CMMvhlStdEuroBSNewTempForRefactor::GetGridListSizeEuroModel3( T_MVHL_D &m_Data, int nListSize )
{
	if     (m_Data.SelVehicle == _T("600/150"))      nListSize = 4;
	else if(m_Data.SelVehicle == _T("900/150"))      nListSize = 6;
	else if(m_Data.SelVehicle == _T("1200/150/200")) nListSize = 8;
	else if(m_Data.SelVehicle == _T("1500/150/200")) nListSize = 10;
	else if(m_Data.SelVehicle == _T("1800/150/200")) nListSize = 12;
	else if(m_Data.SelVehicle == _T("2400/200"))     nListSize = 12;
	else if(m_Data.SelVehicle == _T("3000/200"))     nListSize = 15;
	else if(m_Data.SelVehicle == _T("3600/200"))     nListSize = 18;
	else nListSize = 0;	return nListSize;
}

int CMMvhlStdEuroBSNewTempForRefactor::GetGridListSizeSvSov( T_MVHL_D &m_Data, int nListSize )
{
	CString strSelVehicleUpper;
	if(m_Data.nStandardCode == D_MVHL_BS_BD8611) strSelVehicleUpper = m_Data.VehicleTypeName;
	else                                         strSelVehicleUpper = m_Data.SelVehicle;

	strSelVehicleUpper.TrimRight(); 
	strSelVehicleUpper.MakeUpper();// 이 함수를 부르는 부분을 따라가 보면 대문자로 바꾼다음에 비교하는 짓을 하고 있어서 어쩔수 없어 나도 따라함...

	if     (strSelVehicleUpper == CString(_T("SV 80")).TrimRight().MakeUpper())     nListSize = 6;
	else if(strSelVehicleUpper == CString(_T("SV 100")).TrimRight().MakeUpper())    nListSize = 6;
	else if(strSelVehicleUpper == CString(_T("SV 150")).TrimRight().MakeUpper())    nListSize = 10;
	else if(strSelVehicleUpper == CString(_T("SV 196")).TrimRight().MakeUpper())    nListSize = 12;
	else if(strSelVehicleUpper == CString(_T("SV-Train")).TrimRight().MakeUpper())  nListSize = 13;
	else if(strSelVehicleUpper == CString(_T("SV TT")).TrimRight().MakeUpper())     nListSize = 5;
	else if(strSelVehicleUpper == CString(_T("SOV 250")).TrimRight().MakeUpper())   nListSize = 19;
	else if(strSelVehicleUpper == CString(_T("SOV 350")).TrimRight().MakeUpper())   nListSize = 24;
	else if(strSelVehicleUpper == CString(_T("SOV 450")).TrimRight().MakeUpper())   nListSize = 28;
	else if(strSelVehicleUpper == CString(_T("SOV 600")).TrimRight().MakeUpper())   nListSize = 35;
	else nListSize = 0;	return nListSize;
}

int CMMvhlStdEuroBSNewTempForRefactor::GetGridListSizeCS454AllModel1( T_MVHL_D &m_Data, int nListSize )
{
	CString strSelVehicleUpper;
	strSelVehicleUpper = m_Data.SelVehicle;

	strSelVehicleUpper.TrimRight(); 
	strSelVehicleUpper.MakeUpper();

	if     (strSelVehicleUpper == CString(_T("A-4AXLE")  ).TrimRight().MakeUpper())   nListSize = 4;
	else if(strSelVehicleUpper == CString(_T("B-4AXLE")  ).TrimRight().MakeUpper())   nListSize = 4;
	else if(strSelVehicleUpper == CString(_T("C-5AXLE")  ).TrimRight().MakeUpper())   nListSize = 5;
	else if(strSelVehicleUpper == CString(_T("D-5AXLE_1")).TrimRight().MakeUpper())   nListSize = 5;
	else if(strSelVehicleUpper == CString(_T("D-5AXLE_2")).TrimRight().MakeUpper())   nListSize = 5;
	else if(strSelVehicleUpper == CString(_T("E-5AXLE_1")).TrimRight().MakeUpper())   nListSize = 5;
	else if(strSelVehicleUpper == CString(_T("E-5AXLE_2")).TrimRight().MakeUpper())   nListSize = 5;
	else if(strSelVehicleUpper == CString(_T("F-6AXLE_1")).TrimRight().MakeUpper())   nListSize = 6;
	else if(strSelVehicleUpper == CString(_T("F-6AXLE_2")).TrimRight().MakeUpper())   nListSize = 6;
	else if(strSelVehicleUpper == CString(_T("G-6AXLE_1")).TrimRight().MakeUpper())   nListSize = 6;
	else if(strSelVehicleUpper == CString(_T("G-6AXLE_2")).TrimRight().MakeUpper())   nListSize = 6;
	else if(strSelVehicleUpper == CString(_T("H-5AXLE_1")).TrimRight().MakeUpper())   nListSize = 5;
	else if(strSelVehicleUpper == CString(_T("H-5AXLE_2")).TrimRight().MakeUpper())   nListSize = 5;
	else if(strSelVehicleUpper == CString(_T("I-3AXLE")  ).TrimRight().MakeUpper())   nListSize = 3;
	else if(strSelVehicleUpper == CString(_T("J-3AXLE")  ).TrimRight().MakeUpper())   nListSize = 3;
	else if(strSelVehicleUpper == CString(_T("K-3AXLE_1")).TrimRight().MakeUpper())   nListSize = 3;
	else if(strSelVehicleUpper == CString(_T("K-3AXLE_2")).TrimRight().MakeUpper())   nListSize = 3;
	else if(strSelVehicleUpper == CString(_T("L-3AXLE_1")).TrimRight().MakeUpper())   nListSize = 3;
	else if(strSelVehicleUpper == CString(_T("L-3AXLE_2")).TrimRight().MakeUpper())   nListSize = 3;
	else if(strSelVehicleUpper == CString(_T("M-2AXLE")  ).TrimRight().MakeUpper())   nListSize = 2;
	else if(strSelVehicleUpper == CString(_T("N-2AXLE")  ).TrimRight().MakeUpper())   nListSize = 2;
	else if(strSelVehicleUpper == CString(_T("O-2AXLE")  ).TrimRight().MakeUpper())   nListSize = 2;
	else nListSize = 0;	return nListSize;
}

int CMMvhlStdEuroBSNewTempForRefactor::GetGridListSizeNetworkRailStandards(T_MVHL_D& m_Data, int nListSize)
{
	CString strSelVehicleUpper;
	strSelVehicleUpper = m_Data.VehicleTypeName;

	strSelVehicleUpper.TrimRight();
	strSelVehicleUpper.MakeUpper();

	if (strSelVehicleUpper == CString(_T("TYPE RAI LOADING")).TrimRight().MakeUpper())   nListSize = 16;
	else if (strSelVehicleUpper == _T("WAGON"))   nListSize = 4;
	else nListSize = 0;	return nListSize;
}

int CMMvhlStdEuroBSNewTempForRefactor::GetGridListSizeNetworkRailStandards2(T_MVHL_D& m_Data, int nListSize)
{
	CString strSelVehicleUpper;
	strSelVehicleUpper = m_Data.VehicleTypeName;

	strSelVehicleUpper.TrimRight();
	strSelVehicleUpper.MakeUpper();

	if (strSelVehicleUpper == CString(_T("TYPE RAI LOADING")).TrimRight().MakeUpper())   nListSize = 2;
	else nListSize = 0;	return nListSize;
}

BOOL CMMvhlStdEuroBSNewTempForRefactor::IsPatchTypeFatigueVehicle(T_MVHL_D& m_Data, int nIndex)
{
	BOOL bFatiguChk = FALSE;

	switch (m_Data.nStandardCode)
	{
	case D_MVHL_EURO_EN1991_2_2003_ROAD_BRG_FTG:
		if (nIndex == 0) // First Grid
		{
			if      (m_Data.VehicleTypeName == _T("Fatigue Load Model 2 (280)")) bFatiguChk = TRUE;
			else if (m_Data.VehicleTypeName == _T("Fatigue Load Model 2 (360)")) bFatiguChk = TRUE;
			else if (m_Data.VehicleTypeName == _T("Fatigue Load Model 2 (630)")) bFatiguChk = TRUE;
			else if (m_Data.VehicleTypeName == _T("Fatigue Load Model 2 (560)")) bFatiguChk = TRUE;
			else if (m_Data.VehicleTypeName == _T("Fatigue Load Model 2 (610)")) bFatiguChk = TRUE;
			else if (m_Data.VehicleTypeName == _T("Fatigue Load Model 4 (200)")) bFatiguChk = TRUE;
			else if (m_Data.VehicleTypeName == _T("Fatigue Load Model 4 (310)")) bFatiguChk = TRUE;
			else if (m_Data.VehicleTypeName == _T("Fatigue Load Model 4 (490)")) bFatiguChk = TRUE;
			else if (m_Data.VehicleTypeName == _T("Fatigue Load Model 4 (390)")) bFatiguChk = TRUE;
			else if (m_Data.VehicleTypeName == _T("Fatigue Load Model 4 (450)")) bFatiguChk = TRUE;
			else bFatiguChk = FALSE;
		}
		break;
	default:
		bFatiguChk = FALSE;
	}

	// Patch load 타입이 0보다 클때만 고려
//	if (!m_Data.PatchLoad.bPatchLoad)
//	{
//		bFatiguChk = FALSE;
//	}

	return bFatiguChk;
}

CString CMMvhlStdEuroBSNewTempForRefactor::VehiclePatchTypeString(int nType)
{
	CString strType;

	switch (nType)
	{
	case 1: strType = _T("A"); break;
	case 2: strType = _T("B"); break;
	case 3: strType = _T("C"); break;
	}
	return strType;

}