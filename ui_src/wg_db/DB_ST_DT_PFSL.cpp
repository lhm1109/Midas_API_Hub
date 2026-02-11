#include "stdafx.h"
#include "DB_ST_DT_PFSL.h"
#include "UnitCtrl.h"

T_PFSL_D::T_PFSL_D(void)
{
	Init();
}

T_PFSL_D::~T_PFSL_D(void)
{

}

void T_PFSL_D::Init(void)
{
	strName = _T("TC1");
	 dM      = 5200.0;
	 dM0     = 5200.0;
	 bWater  = FALSE;
	 dfa0    = 500.0;
	 dGama   = 20.0;
	 dGamab  = 18.6;
	 dPsi    = 0.0;
	 dEs     = 5000.0;
	 dc      = 0.0;
	 dqik    = 50.0;
	 dBetasi = 0.0;
	 dBetap  = 0.0;
	 dqrs    = 1150.0;
	 dAlphai = 0.6;
	 dAlphar = 0.6;
	 dqrk    = 1150.0;
	 dc2i    = 0.0;
	 dc1     = 0.0;
	 dfrk    = 2000.0;
}

void T_PFSL_D::S2D(const ftmXmlNode* pXmlNode)
{
	ftm::xml::getVal(*pXmlNode, _T("strName"), strName);
	ftm::xml::getVal(*pXmlNode, _T("dM "), dM);
	ftm::xml::getVal(*pXmlNode, _T("dM0 "), dM0);
	ftm::xml::getVal(*pXmlNode, _T("bWater"), bWater);
	ftm::xml::getVal(*pXmlNode, _T("dfa0"), dfa0);
	ftm::xml::getVal(*pXmlNode, _T("dGama"), dGama);
	ftm::xml::getVal(*pXmlNode, _T("dGamab"), dGamab);
	ftm::xml::getVal(*pXmlNode, _T("dPsi"), dPsi);
	ftm::xml::getVal(*pXmlNode, _T("dEs"), dEs);
	ftm::xml::getVal(*pXmlNode, _T("dc"), dc);
	ftm::xml::getVal(*pXmlNode, _T("dqik"), dqik);
	ftm::xml::getVal(*pXmlNode, _T("dBetasi"), dBetasi);
	ftm::xml::getVal(*pXmlNode, _T("dBetap"), dBetap);
	ftm::xml::getVal(*pXmlNode, _T("dqrs"), dqrs);
	ftm::xml::getVal(*pXmlNode, _T("dAlphai"), dAlphai);
	ftm::xml::getVal(*pXmlNode, _T("dAlphar"), dAlphar);
	ftm::xml::getVal(*pXmlNode, _T("dqrk"), dqrk);
	ftm::xml::getVal(*pXmlNode, _T("dc2i"), dc2i);
	ftm::xml::getVal(*pXmlNode, _T("dc1"), dc1);
	ftm::xml::getVal(*pXmlNode, _T("dfrk"), dfrk);
}

void T_PFSL_D::D2S(ftmXmlNode* pXmlNode) const
{
	ftm::xml::setVal(*pXmlNode, _T("strName"), strName);
	ftm::xml::setVal(*pXmlNode, _T("dM "), dM);
	ftm::xml::setVal(*pXmlNode, _T("dM0 "), dM0);
	ftm::xml::setVal(*pXmlNode, _T("bWater"), bWater);
	ftm::xml::setVal(*pXmlNode, _T("dfa0"), dfa0);
	ftm::xml::setVal(*pXmlNode, _T("dGama"), dGama);
	ftm::xml::setVal(*pXmlNode, _T("dGamab"), dGamab);
	ftm::xml::setVal(*pXmlNode, _T("dPsi"), dPsi);
	ftm::xml::setVal(*pXmlNode, _T("dEs"), dEs);
	ftm::xml::setVal(*pXmlNode, _T("dc"), dc);
	ftm::xml::setVal(*pXmlNode, _T("dqik"), dqik);
	ftm::xml::setVal(*pXmlNode, _T("dBetasi"), dBetasi);
	ftm::xml::setVal(*pXmlNode, _T("dBetap"), dBetap);
	ftm::xml::setVal(*pXmlNode, _T("dqrs"), dqrs);
	ftm::xml::setVal(*pXmlNode, _T("dAlphai"), dAlphai);
	ftm::xml::setVal(*pXmlNode, _T("dAlphar"), dAlphar);
	ftm::xml::setVal(*pXmlNode, _T("dqrk"), dqrk);
	ftm::xml::setVal(*pXmlNode, _T("dc2i"), dc2i);
	ftm::xml::setVal(*pXmlNode, _T("dc1"), dc1);
	ftm::xml::setVal(*pXmlNode, _T("dfrk"), dfrk);
}

void T_PFSL_D::ConvertUintIn(CUnitCtrl* pUnitCtrl)
{
	dM     *= (pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_STRESS) / pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_AREA));
	dM0    *= (pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_STRESS) / pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_AREA));
	dfa0   *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_STRESS) ; 
	dGama  *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_DENSITY);
	dGamab *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_DENSITY);
	dPsi   *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_RADIAN) ; 
	dEs    *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_STRESS) ;
	dc     *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_STRESS) ; 
	dqik   *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_STRESS) ; 
	dqrs   *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_STRESS) ; 
	dqrk   *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_STRESS) ; 
	dfrk   *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_STRESS) ; 
}

void T_PFSL_D::ConvertUintOut(CUnitCtrl* pUnitCtrl)
{
	dM     /= (pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_STRESS) / pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_AREA));
	dM0    /= (pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_STRESS) / pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_AREA));
	dfa0   /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_STRESS) ; 
	dGama  /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_DENSITY);
	dGamab /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_DENSITY);
	dPsi   /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_RADIAN) ; 
	dEs    /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_STRESS) ;
	dc     /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_STRESS) ; 
	dqik   /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_STRESS) ; 
	dqrs   /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_STRESS) ; 
	dqrk   /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_STRESS) ; 
	dfrk   /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_STRESS) ; 
}

void T_PFSL_D::ConvertUnitPrev(CUnitCtrl* pUnitCtrl)
{
	dM     *=(pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_STRESS) / pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_AREA));
	dM0    *=(pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_STRESS) / pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_AREA));
	dfa0   *= pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_STRESS) ; 
	dGama  *= pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_DENSITY);
	dGamab *= pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_DENSITY);
	dPsi   *= pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_RADIAN) ; 
	dEs    *= pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_STRESS) ;
	dc     *= pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_STRESS) ; 
	dqik   *= pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_STRESS) ; 
	dqrs   *= pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_STRESS) ; 
	dqrk   *= pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_STRESS) ; 
	dfrk   *= pUnitCtrl->GetConvertFactorPrevious(D_UNITSYS_BASE_STRESS) ; 
}

void T_PFSL_D_CH::Init(void)
{
	memset(this, 0, sizeof(T_PFSL_D_CH));
}

void T_PFSL_D_CH::ConvertToString(T_PFSL_D& rData)
{
	ConvertCharStr(strName, rData.strName, sizeof(strName));
	rData.dM      = this->dM     ;      
	rData.dM0     = this->dM0    ;     
	rData.bWater  = this->bWater ;  
	rData.dfa0    = this->dfa0   ;    
	rData.dGama   = this->dGama  ;   
	rData.dGamab  = this->dGamab ;  
	rData.dPsi    = this->dPsi   ;    
	rData.dEs     = this->dEs    ;     
	rData.dc      = this->dc     ;      
	rData.dqik    = this->dqik   ;    
	rData.dBetasi = this->dBetasi; 
	rData.dBetap  = this->dBetap ;  
	rData.dqrs    = this->dqrs   ;    
	rData.dAlphai = this->dAlphai; 
	rData.dAlphar = this->dAlphar; 
	rData.dqrk    = this->dqrk   ;    
	rData.dc2i    = this->dc2i   ;    
	rData.dc1     = this->dc1    ;     
	rData.dfrk    = this->dfrk   ; 
}

void T_PFSL_D_CH::ConvertToChar(T_PFSL_D& rData)
{
	ConvertStrChar(rData.strName,strName, sizeof(strName));
	this->dM      = rData.dM     ;      
	this->dM0     = rData.dM0    ;     
	this->bWater  = rData.bWater ;  
	this->dfa0    = rData.dfa0   ;    
	this->dGama   = rData.dGama  ;   
	this->dGamab  = rData.dGamab ;  
	this->dPsi    = rData.dPsi   ;    
	this->dEs     = rData.dEs    ;     
	this->dc      = rData.dc     ;      
	this->dqik    = rData.dqik   ;    
	this->dBetasi = rData.dBetasi; 
	this->dBetap  = rData.dBetap ;  
	this->dqrs    = rData.dqrs   ;    
	this->dAlphai = rData.dAlphai; 
	this->dAlphar = rData.dAlphar; 
	this->dqrk    = rData.dqrk   ;    
	this->dc2i    = rData.dc2i   ;    
	this->dc1     = rData.dc1    ;     
	this->dfrk    = rData.dfrk   ; 
}
