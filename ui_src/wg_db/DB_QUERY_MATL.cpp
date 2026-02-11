// DB_QUERY_MATL.cpp: implementation of the query class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DB_QUERY_MATL.h"

#include "DBDoc.h"
#include "Qsort.h"
#include "DataMemb.h"
#include "UnitCtrl.h"

#include "DB_MATD_REIN.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define COMBO_SHEET_CF_High_Strength_Type1 _LS(IDS_SHEATH_CARBON_MATE_SEAT1_01)
#define COMBO_SHEET_CF_High_Strength_Type2 _LS(IDS_SHEATH_CARBON_MATE_SEAT1_02)
#define COMBO_SHEET_CF_Medium_Elasticity_Type _LS(IDS_SHEATH_CARBON_MATE_SEAT1_03)
#define COMBO_SHEET_CF_High_Elasticity_Type _LS(IDS_SHEATH_CARBON_MATE_SEAT1_04)

#define COMBO_SHEET_AFRT_aramid1 _LS(IDS_SHEATH_CARBON_MATE_SEAT2_01)
#define COMBO_SHEET_AFRT_aramid2_E78400 _LS(IDS_SHEATH_CARBON_MATE_SEAT2_02)
#define COMBO_SHEET_AFRT_aramid2_E78000 _LS(IDS_SHEATH_CARBON_MATE_SEAT2_03)

#define COMBO_VOLUME_200  _T("200")
#define COMBO_VOLUME_235  _T("235")
#define COMBO_VOLUME_280  _T("280")
#define COMBO_VOLUME_300  _T("300")
#define COMBO_VOLUME_300_1  _T("300-1")
#define COMBO_VOLUME_300_2  _T("300-2")
#define COMBO_VOLUME_350  _T("350")
#define COMBO_VOLUME_400  _T("400")
#define COMBO_VOLUME_415  _T("415")
#define COMBO_VOLUME_450  _T("450")
#define COMBO_VOLUME_525  _T("525")
#define COMBO_VOLUME_600  _T("600")
#define COMBO_VOLUME_623  _T("623")
#define COMBO_VOLUME_700  _T("700")
#define COMBO_VOLUME_830  _T("830")

//////////////////////////////////////////////////////////////////////
#pragma region QMatdRein
//////////////////////////////////////////////////////////////////////
QMatdRein::QMatdRein(CDBDoc* pDoc, CDataMemb* pDataMemb)
{
	m_pUnitCtrl = pDoc->m_pUnitCtrl;
	m_pMatdRein = pDataMemb->m_pMatdRein;
}

QMatdRein::~QMatdRein()
{
	m_pUnitCtrl = NULL;
	m_pMatdRein = NULL;
}

int QMatdRein::GetCount()
{
	return m_pMatdRein->GetCount();
}

POSITION QMatdRein::GetStart()
{
	return m_pMatdRein->GetStart();
}

void QMatdRein::GetNext(POSITION& rNextPosition, T_MATD_REIN_K& rKey, T_MATD_REIN_D& rData)
{
	m_pMatdRein->GetNext(rNextPosition, rKey, rData);
	m_pUnitCtrl->ConvertUnitMatdReinOut(rData);
}

BOOL QMatdRein::Exist(T_MATD_REIN_K Key)
{
	T_MATD_REIN_D Data;
	return m_pMatdRein->Get(Key, Data);
}

void QMatdRein::GetKeyList(CArray<T_MATD_REIN_K, T_MATD_REIN_K>& rKeyList)
{
	rKeyList.RemoveAll();

	POSITION pos;
	T_MATD_REIN_K Key;
	T_MATD_REIN_D Data;
	pos = GetStart();
	while (pos != NULL)
	{
		GetNext(pos, Key, Data);
		rKeyList.Add(Key);
	}
	if (rKeyList.GetSize() > 0)
		CQSort::QSortUInt(rKeyList.GetData(), rKeyList.GetSize());
}

BOOL QMatdRein::Get(T_MATD_REIN_K Key, T_MATD_REIN_D& rData)
{
	BOOL bReturn = m_pMatdRein->Get(Key, rData);
	if (bReturn) m_pUnitCtrl->ConvertUnitMatdReinOut(rData);
	return bReturn;
}

T_MATD_REIN_K QMatdRein::GetStartNum()
{
	return m_pMatdRein->m_nStartNum;
}

void QMatdRein::GetReinCfDoubleData(const CString &strMainType, const CString& strSubType,  std::tuple<double, double, double>& rData)
{
	std::map<CString, std::tuple<double, double, double>> arFiber1 = {
		{COMBO_VOLUME_200,	{0.000111 , 230000000000.0 , 3400000000.0 }},
		{COMBO_VOLUME_300,	{0.000167 , 230000000000.0 , 3400000000.0 }},
		{COMBO_VOLUME_400,	{0.000222 , 230000000000.0 , 3400000000.0 }},
		{COMBO_VOLUME_450,	{0.000250 , 230000000000.0 , 3400000000.0 }},
		{COMBO_VOLUME_600,	{0.000333 , 230000000000.0 , 3400000000.0 }},
	};
	std::map<CString, std::tuple<double, double, double>> arFiber2 = {
		{COMBO_VOLUME_200,	{0.000111 , 245000000000.0 , 3400000000.0 }},
		{COMBO_VOLUME_300,	{0.000167 , 245000000000.0 , 3400000000.0 }},
		{COMBO_VOLUME_400,	{0.000222 , 245000000000.0 , 3400000000.0 }},
		{COMBO_VOLUME_450,	{0.000250 , 245000000000.0 , 3400000000.0 }},
		{COMBO_VOLUME_600,	{0.000333 , 245000000000.0 , 3400000000.0 }},
	};
	std::map<CString, std::tuple<double, double, double>> arFiber3 = {
		{COMBO_VOLUME_300_1,	{0.000165 , 390000000000.0 , 2900000000.0 }},
		{COMBO_VOLUME_300_2,	{0.000163 , 440000000000.0 , 2400000000.0 }},
	};
	std::map<CString, std::tuple<double, double, double>> arFiber4 = {
		{COMBO_VOLUME_300,	{0.000143 , 640000000000.0 , 1900000000.0 }},
		{COMBO_VOLUME_400,	{0.000190 , 640000000000.0 , 1900000000.0 }},
	};
	std::map<CString, std::tuple<double, double, double>> arAfrp1 = {
		{COMBO_VOLUME_280,	{0.000193 , 118000000000.0 , 2060000000.0 }},
		{COMBO_VOLUME_415,	{0.000286 , 118000000000.0 , 2060000000.0 }},
		{COMBO_VOLUME_623,	{0.000430 , 118000000000.0 , 2060000000.0 }},
		{COMBO_VOLUME_830,	{0.000572 , 118000000000.0 , 2060000000.0 }},
	};
	std::map<CString, std::tuple<double, double, double>> arAfrp2_E78400 = {
		{COMBO_VOLUME_235,	{0.000169 , 78400000000.0 , 2350000000.0 }},
		{COMBO_VOLUME_350,	{0.000252 , 78400000000.0 , 2350000000.0 }},
		{COMBO_VOLUME_525,	{0.000378 , 78400000000.0 , 2350000000.0 }},
		{COMBO_VOLUME_700,	{0.000504 , 78400000000.0 , 2350000000.0 }},
	};
	std::map<CString, std::tuple<double, double, double>> arAfrp2_E78000 = {
		{COMBO_VOLUME_235,	{0.000169 , 78000000000.0 , 2350000000.0 }},
		{COMBO_VOLUME_350,	{0.000252 , 78000000000.0 , 2350000000.0 }},
		{COMBO_VOLUME_525,	{0.000378 , 78000000000.0 , 2350000000.0 }},
		{COMBO_VOLUME_700,	{0.000504 , 78000000000.0 , 2350000000.0 }},
	};

	T_MATD_REIN_D Data;
	if (strMainType == COMBO_SHEET_CF_High_Strength_Type1)
	{
		if (arFiber1.find(strSubType) != arFiber1.end())
		{
			rData = arFiber1[strSubType];
			Data.CoverCF.dThickness = std::get<0>(rData);
			Data.CoverCF.dElast = std::get<1>(rData);
			Data.CoverCF.dFu = std::get<2>(rData);
		}
		else rData = std::make_tuple(0.0, 0.0, 0.0);
	}
	else if (strMainType == COMBO_SHEET_CF_High_Strength_Type2)
	{
		if (arFiber2.find(strSubType) != arFiber2.end())
		{
			rData = arFiber2[strSubType];
			Data.CoverCF.dThickness = std::get<0>(rData);
			Data.CoverCF.dElast = std::get<1>(rData);
			Data.CoverCF.dFu = std::get<2>(rData);
		}
		else rData = std::make_tuple(0.0, 0.0, 0.0);
	}
	else if (strMainType == COMBO_SHEET_CF_Medium_Elasticity_Type)
	{
		if (arFiber3.find(strSubType) != arFiber3.end())
		{
			rData = arFiber3[strSubType];
			Data.CoverCF.dThickness = std::get<0>(rData);
			Data.CoverCF.dElast = std::get<1>(rData);
			Data.CoverCF.dFu = std::get<2>(rData);
		}
		else rData = std::make_tuple(0.0, 0.0, 0.0);
	}
	else if (strMainType == COMBO_SHEET_CF_High_Elasticity_Type)
	{
		if (arFiber4.find(strSubType) != arFiber4.end())
		{
			rData = arFiber4[strSubType];
			Data.CoverCF.dThickness = std::get<0>(rData);
			Data.CoverCF.dElast = std::get<1>(rData);
			Data.CoverCF.dFu = std::get<2>(rData);
		}
		else rData = std::make_tuple(0.0, 0.0, 0.0);
	}
	else if (strMainType == COMBO_SHEET_AFRT_aramid1)
	{
		if (arAfrp1.find(strSubType) != arAfrp1.end())
		{
			rData = arAfrp1[strSubType];
			Data.CoverCF.dThickness = std::get<0>(rData);
			Data.CoverCF.dElast = std::get<1>(rData);
			Data.CoverCF.dFu = std::get<2>(rData);
		}
		else rData = std::make_tuple(0.0, 0.0, 0.0);
	}
	else if (strMainType == COMBO_SHEET_AFRT_aramid2_E78400)
	{
		if (arAfrp2_E78400.find(strSubType) != arAfrp2_E78400.end())
		{
			rData = arAfrp2_E78400[strSubType];
			Data.CoverCF.dThickness = std::get<0>(rData);
			Data.CoverCF.dElast = std::get<1>(rData);
			Data.CoverCF.dFu = std::get<2>(rData);
		}
		else rData = std::make_tuple(0.0, 0.0, 0.0);
	}

	else if (strMainType == COMBO_SHEET_AFRT_aramid2_E78000)
	{
		if (arAfrp2_E78000.find(strSubType) != arAfrp2_E78000.end())
		{
			rData = arAfrp2_E78000[strSubType];
			Data.CoverCF.dThickness = std::get<0>(rData);
			Data.CoverCF.dElast = std::get<1>(rData);
			Data.CoverCF.dFu = std::get<2>(rData);
		}
		else rData = std::make_tuple(0.0, 0.0, 0.0);
	}

	if (std::get<0>(rData) == 0 || std::get<1>(rData) == 0 || std::get<2>(rData) == 0) return;
	m_pUnitCtrl->ConvertUnitMatdReinOut(Data);
	rData = std::make_tuple(
		Data.CoverCF.dThickness,
		Data.CoverCF.dElast,
		Data.CoverCF.dFu
	);
}
#pragma endregion