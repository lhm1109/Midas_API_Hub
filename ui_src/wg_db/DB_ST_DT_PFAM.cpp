#include "stdafx.h"
#include "DB_ST_DT_PFAM.h"
#include "UnitCtrl.h"

T_PFAM_D::T_PFAM_D(void)
{
	Init();
}

T_PFAM_D::T_PFAM_D(const T_PFAM_D& src)
{
	*this = src;
}

T_PFAM_D::~T_PFAM_D(void)
{

}

void T_PFAM_D::Init(void)
{
	bSelect = TRUE;
	strName =_T("DT1");
	enGenerateType = EnGenerateType::EnGenerateType_ElemBoundary;
	enLayoutType   = EnLayoutType::EnLayoutType_Auto;
	memset(keyNodeCushionCapBottom, 0, sizeof(keyNodeCushionCapBottom));
	keySoilBoring = NULL;
	iLongitudinalPileCount = 2;
	iHorizonalPileCount = 2;
	dLongitudinalPileSpacing = 2.5;
	dHorizonalPileSpacing    = 3.0;
	keyPileMalt   = 1;
	dPileDiameter = 1.2;
	dPileLength = 20.0;
	dDivLength  = 2.0;
	dScouringLine2Top = 4.0;
	dTopLevel = 4.0;
	dScaleFactor = 1.0;
}

void T_PFAM_D::S2D(const ftmXmlNode* pXmlNode)
{
	ftm::xml::getVal(*pXmlNode, _T("bSelect"), bSelect);
	ftm::xml::getVal(*pXmlNode, _T("strName"), strName);
	ftm::xml::getVal(*pXmlNode, _T("enGenerateType"), (UINT&)enGenerateType);
	ftm::xml::getVal(*pXmlNode, _T("enLayoutType"), (UINT&)enLayoutType);
	ftm::xml::getVal(*pXmlNode, _T("keyNodeCushionCapBottom")  , keyNodeCushionCapBottom[0]);
	ftm::xml::getVal(*pXmlNode, _T("keyNodeCushionCapBottom1") , keyNodeCushionCapBottom[1]);
	ftm::xml::getVal(*pXmlNode, _T("keyNodeCushionCapBottom2") , keyNodeCushionCapBottom[2]);
	ftm::xml::getVal(*pXmlNode, _T("keyNodeCushionCapBottom3") , keyNodeCushionCapBottom[3]);
	ftm::xml::getVal(*pXmlNode, _T("keyNodeCushionCapBottom4") , keyNodeCushionCapBottom[4]);
	ftm::xml::getVal(*pXmlNode, _T("keyNodeCushionCapBottom5") , keyNodeCushionCapBottom[5]);
	ftm::xml::getVal(*pXmlNode, _T("keyNodeCushionCapBottom6") , keyNodeCushionCapBottom[6]);
	ftm::xml::getVal(*pXmlNode, _T("keyNodeCushionCapBottom7") , keyNodeCushionCapBottom[7]);
	ftm::xml::getVal(*pXmlNode, _T("keyNodeCushionCapBottom8") , keyNodeCushionCapBottom[8]);
	ftm::xml::getVal(*pXmlNode, _T("keyNodeCushionCapBottom9") , keyNodeCushionCapBottom[9]);
	ftm::xml::getVal(*pXmlNode, _T("keySoilBoring"), keySoilBoring);
	ftm::xml::getVal(*pXmlNode, _T("iLongitudinalPileCount"), iLongitudinalPileCount);
	ftm::xml::getVal(*pXmlNode, _T("iHorizonalPileCount"), iHorizonalPileCount);
	ftm::xml::getVal(*pXmlNode, _T("dLongitudinalPileSpacing"), dLongitudinalPileSpacing);
	ftm::xml::getVal(*pXmlNode, _T("dHorizonalPileSpacing"), dHorizonalPileSpacing);
	ftm::xml::getVal(*pXmlNode, _T("keyPileMalt"), keyPileMalt);
	ftm::xml::getVal(*pXmlNode, _T("dPileDiameter"), dPileDiameter);
	ftm::xml::getVal(*pXmlNode, _T("dPileLength"), dPileLength);
	ftm::xml::getVal(*pXmlNode, _T("dDivLength"), dDivLength);
	ftm::xml::getVal(*pXmlNode, _T("dScouringLine2Top"), dScouringLine2Top);
	ftm::xml::getVal(*pXmlNode, _T("dTopLevel"), dTopLevel);
	ftm::xml::getVal(*pXmlNode, _T("dScaleFactor"), dScaleFactor);
}

void T_PFAM_D::D2S(ftmXmlNode* pXmlNode) const
{
	ftm::xml::setVal(*pXmlNode, _T("bSelect"), bSelect);
	ftm::xml::setVal(*pXmlNode, _T("strName"), strName);
	ftm::xml::setVal(*pXmlNode, _T("enGenerateType"), (UINT)enGenerateType);
	ftm::xml::setVal(*pXmlNode, _T("enLayoutType"), (UINT)enLayoutType);
	ftm::xml::setVal(*pXmlNode, _T("keyNodeCushionCapBottom"),   keyNodeCushionCapBottom[0]);
	ftm::xml::setVal(*pXmlNode, _T("keyNodeCushionCapBottom1"),  keyNodeCushionCapBottom[1]);
	ftm::xml::setVal(*pXmlNode, _T("keyNodeCushionCapBottom2"),  keyNodeCushionCapBottom[2]);
	ftm::xml::setVal(*pXmlNode, _T("keyNodeCushionCapBottom3"),  keyNodeCushionCapBottom[3]);
	ftm::xml::setVal(*pXmlNode, _T("keyNodeCushionCapBottom4"),  keyNodeCushionCapBottom[4]);
	ftm::xml::setVal(*pXmlNode, _T("keyNodeCushionCapBottom5"),  keyNodeCushionCapBottom[5]);
	ftm::xml::setVal(*pXmlNode, _T("keyNodeCushionCapBottom6"),  keyNodeCushionCapBottom[6]);
	ftm::xml::setVal(*pXmlNode, _T("keyNodeCushionCapBottom7"),  keyNodeCushionCapBottom[7]);
	ftm::xml::setVal(*pXmlNode, _T("keyNodeCushionCapBottom8"),  keyNodeCushionCapBottom[8]);
	ftm::xml::setVal(*pXmlNode, _T("keyNodeCushionCapBottom9"),  keyNodeCushionCapBottom[9]);
	ftm::xml::setVal(*pXmlNode, _T("keySoilBoring"), keySoilBoring);
	ftm::xml::setVal(*pXmlNode, _T("iLongitudinalPileCount"), iLongitudinalPileCount);
	ftm::xml::setVal(*pXmlNode, _T("iHorizonalPileCount"), iHorizonalPileCount);
	ftm::xml::setVal(*pXmlNode, _T("dLongitudinalPileSpacing"), dLongitudinalPileSpacing);
	ftm::xml::setVal(*pXmlNode, _T("dHorizonalPileSpacing"), dHorizonalPileSpacing);
	ftm::xml::setVal(*pXmlNode, _T("keyPileMalt"), keyPileMalt);
	ftm::xml::setVal(*pXmlNode, _T("dPileDiameter"), dPileDiameter);
	ftm::xml::setVal(*pXmlNode, _T("dPileLength"), dPileLength);
	ftm::xml::setVal(*pXmlNode, _T("dDivLength"), dDivLength);
	ftm::xml::setVal(*pXmlNode, _T("dScouringLine2Top"), dScouringLine2Top);
	ftm::xml::setVal(*pXmlNode, _T("dTopLevel"), dTopLevel);
	ftm::xml::setVal(*pXmlNode, _T("dScaleFactor"), dScaleFactor);
}

void T_PFAM_D::ConvertUintIn(CUnitCtrl* pUnitCtrl)
{
	dLongitudinalPileSpacing *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dHorizonalPileSpacing *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dPileDiameter *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dPileLength *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dDivLength *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dScouringLine2Top *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dTopLevel *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
}

void T_PFAM_D::ConvertUintOut(CUnitCtrl* pUnitCtrl)
{
	dLongitudinalPileSpacing /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dHorizonalPileSpacing /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dPileDiameter /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dPileLength /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dDivLength /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dScouringLine2Top /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dTopLevel /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
}

void T_PFAM_D::ConvertUnitPrev(CUnitCtrl* pUnitCtrl)
{
	dLongitudinalPileSpacing *= pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_LENGTH);
	dHorizonalPileSpacing *= pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_LENGTH);
	dPileDiameter *= pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_LENGTH);
	dPileLength *= pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_LENGTH);
	dDivLength *= pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_LENGTH);
	dScouringLine2Top *= pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_LENGTH);
	dTopLevel *= pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_LENGTH);
}

T_PFAM_D& T_PFAM_D::operator=(const T_PFAM_D& src)
{
	if (this != &src)
	{
		this->bSelect = src.bSelect;
		this->strName = src.strName;
		this->enGenerateType = src.enGenerateType;
		this->enLayoutType = src.enLayoutType;
		memcpy(this->keyNodeCushionCapBottom,src.keyNodeCushionCapBottom,sizeof(src.keyNodeCushionCapBottom));
		this->keySoilBoring = src.keySoilBoring;
		this->iLongitudinalPileCount = src.iLongitudinalPileCount;
		this->iHorizonalPileCount = src.iHorizonalPileCount;
		this->dLongitudinalPileSpacing = src.dLongitudinalPileSpacing;
		this->dHorizonalPileSpacing = src.dHorizonalPileSpacing;
		this->keyPileMalt = src.keyPileMalt;
		this->dPileDiameter = src.dPileDiameter;
		this->dPileLength = src.dPileLength;
		this->dDivLength = src.dDivLength;
		this->dScouringLine2Top = src.dScouringLine2Top;
		this->dTopLevel = src.dTopLevel;
		this->dScaleFactor = src.dScaleFactor;
	}

	return *this;
}

//////////////////////////////////////////////////////////////////////////
void T_PFAM_D_CH_9300::Init(void)
{
	memset(this, 0, sizeof(T_PFAM_D_CH_9300));
}

void T_PFAM_D_CH_9300::ConvertToString(T_PFAM_D& rData)
{
	rData.Init();
	rData.bSelect = this->bSelect;
	ConvertCharStr(strName, rData.strName, sizeof(strName));
	rData.enGenerateType           = (T_PFAM_D::EnGenerateType)this->enGenerateType;
	rData.enLayoutType             = (T_PFAM_D::EnLayoutType)this->enLayoutType;
	rData.keyNodeCushionCapBottom[0] = this->keyNodeCushionCapBottom;
	rData.keySoilBoring            = this->keySoilBoring;
	rData.iLongitudinalPileCount   = this->iLongitudinalPileCount;
	rData.iHorizonalPileCount      = this->iHorizonalPileCount;
	rData.dLongitudinalPileSpacing = this->dLongitudinalPileSpacing;
	rData.dHorizonalPileSpacing    = this->dHorizonalPileSpacing;
	rData.keyPileMalt              = this->keyPileMalt;
	rData.dPileDiameter            = this->dPileDiameter;
	rData.dPileLength              = this->dPileLength;
	rData.dDivLength               = this->dDivLength;
	rData.dScouringLine2Top        = this->dScouringLine2Top;
	rData.dTopLevel                = this->dTopLevel;
}
//////////////////////////////////////////////////////////////////////////
void T_PFAM_D_CH_9301::Init(void)
{
	memset(this, 0, sizeof(T_PFAM_D_CH_9301));
}

void T_PFAM_D_CH_9301::ConvertToString(T_PFAM_D& rData)
{
	rData.Init();
	rData.bSelect = this->bSelect;
	ConvertCharStr(strName, rData.strName, sizeof(strName));
	rData.enGenerateType = (T_PFAM_D::EnGenerateType)this->enGenerateType;
	rData.enLayoutType = (T_PFAM_D::EnLayoutType)this->enLayoutType;
	memcpy(rData.keyNodeCushionCapBottom, this->keyNodeCushionCapBottom, sizeof(keyNodeCushionCapBottom));
	rData.keySoilBoring = this->keySoilBoring;
	rData.iLongitudinalPileCount = this->iLongitudinalPileCount;
	rData.iHorizonalPileCount = this->iHorizonalPileCount;
	rData.dLongitudinalPileSpacing = this->dLongitudinalPileSpacing;
	rData.dHorizonalPileSpacing = this->dHorizonalPileSpacing;
	rData.keyPileMalt = this->keyPileMalt;
	rData.dPileDiameter = this->dPileDiameter;
	rData.dPileLength = this->dPileLength;
	rData.dDivLength = this->dDivLength;
	rData.dScouringLine2Top = this->dScouringLine2Top;
	rData.dTopLevel = this->dTopLevel;
}

//////////////////////////////////////////////////////////////////////////
void T_PFAM_D_CH::Init(void)
{
	memset(this, 0, sizeof(T_PFAM_D_CH));
}

void T_PFAM_D_CH::ConvertToString(T_PFAM_D& rData)
{
	rData.Init();
	rData.bSelect = this->bSelect;
	ConvertCharStr(strName, rData.strName, sizeof(strName));
	rData.enGenerateType           = (T_PFAM_D::EnGenerateType)this->enGenerateType;
	rData.enLayoutType             = (T_PFAM_D::EnLayoutType)this->enLayoutType;
	memcpy(rData.keyNodeCushionCapBottom, this->keyNodeCushionCapBottom, sizeof(keyNodeCushionCapBottom));
	rData.keySoilBoring            = this->keySoilBoring;
	rData.iLongitudinalPileCount   = this->iLongitudinalPileCount;
	rData.iHorizonalPileCount      = this->iHorizonalPileCount;
	rData.dLongitudinalPileSpacing = this->dLongitudinalPileSpacing;
	rData.dHorizonalPileSpacing    = this->dHorizonalPileSpacing;
	rData.keyPileMalt              = this->keyPileMalt;
	rData.dPileDiameter            = this->dPileDiameter;
	rData.dPileLength              = this->dPileLength;
	rData.dDivLength               = this->dDivLength;
	rData.dScouringLine2Top        = this->dScouringLine2Top;
	rData.dTopLevel                = this->dTopLevel;
}

void T_PFAM_D_CH::ConvertToChar(T_PFAM_D& rData)
{
	this->bSelect                  = rData.bSelect;
	ConvertStrChar(rData.strName,strName, sizeof(strName));
	this->enGenerateType           = rData.enGenerateType          ;
	this->enLayoutType             = rData.enLayoutType            ;
	memcpy(this->keyNodeCushionCapBottom, rData.keyNodeCushionCapBottom, sizeof(keyNodeCushionCapBottom));
	this->keySoilBoring            = rData.keySoilBoring           ;
	this->iLongitudinalPileCount   = rData.iLongitudinalPileCount  ;
	this->iHorizonalPileCount      = rData.iHorizonalPileCount     ;
	this->dLongitudinalPileSpacing = rData.dLongitudinalPileSpacing;
	this->dHorizonalPileSpacing    = rData.dHorizonalPileSpacing   ;
	this->keyPileMalt              = rData.keyPileMalt             ;
	this->dPileDiameter             = rData.dPileDiameter          ;
	this->dPileLength              = rData.dPileLength             ;
	this->dDivLength               = rData.dDivLength              ;
	this->dScouringLine2Top        = rData.dScouringLine2Top       ;
	this->dTopLevel                = rData.dTopLevel               ;
	this->dScaleFactor             = rData.dScaleFactor            ;
}

void T_PFAM_D_CH::Convert9300(const T_PFAM_D_CH_9300& rData)
{
	Init();
	this->bSelect                  = rData.bSelect;
	memcpy(strName, rData.strName, sizeof(strName));
	this->enGenerateType           = rData.enGenerateType          ;
	this->enLayoutType             = rData.enLayoutType            ;
	keyNodeCushionCapBottom[0]     = rData.keyNodeCushionCapBottom;
	this->keySoilBoring            = rData.keySoilBoring           ;
	this->iLongitudinalPileCount   = rData.iLongitudinalPileCount  ;
	this->iHorizonalPileCount      = rData.iHorizonalPileCount     ;
	this->dLongitudinalPileSpacing = rData.dLongitudinalPileSpacing;
	this->dHorizonalPileSpacing    = rData.dHorizonalPileSpacing   ;
	this->keyPileMalt              = rData.keyPileMalt             ;
	this->dPileDiameter            = rData.dPileDiameter          ;
	this->dPileLength              = rData.dPileLength             ;
	this->dDivLength               = rData.dDivLength              ;
	this->dScouringLine2Top        = rData.dScouringLine2Top       ;
	this->dTopLevel                = rData.dTopLevel               ;
}

void T_PFAM_D_CH::Convert9301(const T_PFAM_D_CH_9301& rData)
{
	Init();
	this->bSelect = rData.bSelect;
	memcpy(strName, rData.strName, sizeof(strName));
	this->enGenerateType = rData.enGenerateType;
	this->enLayoutType = rData.enLayoutType;
	memcpy(keyNodeCushionCapBottom, rData.keyNodeCushionCapBottom, sizeof(keyNodeCushionCapBottom));
	this->keySoilBoring = rData.keySoilBoring;
	this->iLongitudinalPileCount = rData.iLongitudinalPileCount;
	this->iHorizonalPileCount = rData.iHorizonalPileCount;
	this->dLongitudinalPileSpacing = rData.dLongitudinalPileSpacing;
	this->dHorizonalPileSpacing = rData.dHorizonalPileSpacing;
	this->keyPileMalt = rData.keyPileMalt;
	this->dPileDiameter = rData.dPileDiameter;
	this->dPileLength = rData.dPileLength;
	this->dDivLength = rData.dDivLength;
	this->dScouringLine2Top = rData.dScouringLine2Top;
	this->dTopLevel = rData.dTopLevel;
}
