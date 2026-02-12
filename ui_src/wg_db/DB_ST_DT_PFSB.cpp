#include "stdafx.h"
#include "DB_ST_DT_PFSB.h"
#include "UnitCtrl.h"


T_PFSB_BASE_D::T_PFSB_BASE_D(void)
{
	Init();
}

T_PFSB_BASE_D::~T_PFSB_BASE_D(void)
{

}

void T_PFSB_BASE_D::Init(void)
{
	keySoilLayer = NULL;
	dThick = 5.0;
}

void T_PFSB_BASE_D::S2D(const ftmXmlNode* pXmlNode)
{
	ftm::xml::getVal(*pXmlNode, _T("keySoilLayer"), keySoilLayer);
	ftm::xml::getVal(*pXmlNode, _T("dThick"), dThick);
}

void T_PFSB_BASE_D::D2S(ftmXmlNode* pXmlNode) const
{
	ftm::xml::setVal(*pXmlNode, _T("keySoilLayer"), keySoilLayer);
	ftm::xml::setVal(*pXmlNode, _T("dThick"), dThick);
}

void T_PFSB_BASE_D::ConvertUintIn(CUnitCtrl* pUnitCtrl)
{
	dThick *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
}

void T_PFSB_BASE_D::ConvertUintOut(CUnitCtrl* pUnitCtrl)
{
	dThick /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
}

void T_PFSB_BASE_D::ConvertUnitPrev(CUnitCtrl* pUnitCtrl)
{
	dThick *= pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_LENGTH);
}
//////////////////////////////////////////////////////////////////////////
T_PFSB_D::T_PFSB_D(void)
{
	Init();
}

T_PFSB_D::~T_PFSB_D(void)
{

}

void T_PFSB_D::Init(void)
{
	strName      = _T("ZK1");
	vctSoilLayer.clear();
}

void T_PFSB_D::S2D(const ftmXmlNode* pXmlNode)
{
	ftm::xml::getVal(*pXmlNode, _T("strName"), strName);
	ftm::xmlExt::ReadVectorStruct<T_PFSB_BASE_D>(*pXmlNode, _T("vctSoilLayer"), vctSoilLayer);
}

void T_PFSB_D::D2S(ftmXmlNode* pXmlNode) const
{
	ftm::xml::setVal(*pXmlNode, _T("strName"), strName);
	ftm::xmlExt::WriteVectorStruct<T_PFSB_BASE_D>(*pXmlNode, _T("vctSoilLayer"), vctSoilLayer);
}

void T_PFSB_D::ConvertUintIn(CUnitCtrl* pUnitCtrl)
{
	for (auto itr = vctSoilLayer.begin() ; itr != vctSoilLayer.end() ; itr++)
	{
		itr->ConvertUintIn(pUnitCtrl);
	}
}

void T_PFSB_D::ConvertUintOut(CUnitCtrl* pUnitCtrl)
{
	for (auto itr = vctSoilLayer.begin(); itr != vctSoilLayer.end(); itr++)
	{
		itr->ConvertUintOut(pUnitCtrl);
	}
}

void T_PFSB_D::ConvertUnitPrev(CUnitCtrl* pUnitCtrl)
{
	for (auto itr = vctSoilLayer.begin(); itr != vctSoilLayer.end(); itr++)
	{
		itr->ConvertUnitPrev(pUnitCtrl);
	}
}

double T_PFSB_D::GetTotalDepth(void) const
{
	double dDepth = 0.0;

	for (auto itr:vctSoilLayer)
	{
		dDepth += itr.dThick;
	}
	return dDepth;
}

//////////////////////////////////////////////////////////////////////////

T_PFSB_D_CH::T_PFSB_D_CH(void)
{
	Init();
}

T_PFSB_D_CH::~T_PFSB_D_CH(void)
{

}

void T_PFSB_D_CH::Init(void)
{
	memset(strName, 0, sizeof(strName));
	arSoilLayer.RemoveAll();
}

void T_PFSB_D_CH::ConvertToString(T_PFSB_D& rData)
{
	ConvertCharStr(strName, rData.strName, sizeof(strName));
	rData.vctSoilLayer.clear();
	for (INT_PTR idx = 0 ; idx < arSoilLayer.GetCount() ; idx++)
	{
		rData.vctSoilLayer.push_back(arSoilLayer[idx]);
	}
}

void T_PFSB_D_CH::ConvertToChar(T_PFSB_D& rData)
{
	ConvertStrChar(rData.strName, strName, sizeof(strName));
	arSoilLayer.RemoveAll();
	for (auto itr:rData.vctSoilLayer)
	{
		arSoilLayer.Add(itr);
	}
}

//////////////////////////////////////////////////////////////////////////
void T_PFSB_UDRD_D_CH_RW::Init(void)
{
	memset(this, 0, sizeof(T_PFSB_UDRD_D_CH_RW));
}

void T_PFSB_UDRD_D_CH_RW::GetPfsb(T_PFSB_K& rKey, T_PFSB_D_CH& rData)
{
	rKey = key;
	memcpy(rData.strName, strName, sizeof(strName));
}

void T_PFSB_UDRD_D_CH_RW::SetPfsb(const T_PFSB_K rKey, const T_PFSB_D_CH& rData)
{
	key = rKey;
	memcpy(strName, rData.strName, sizeof(strName));
	uiSoilLayoutCount = (unsigned int)rData.arSoilLayer.GetCount();
}
