// DgnDynData.h: interface for the CDgnDynData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DGNDYNDATA_H__)
#define __DGNDYNDATA_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_DynModule.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_KFactor.h"
#include "..\wg_db\wg_db_QFactor.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnDynData : public CDynModule, CDBUpdateConnector
{
// Member Variables.
public:
	CMap<T_ELEM_K,T_ELEM_K,stKFactor,stKFactor&>	m_arKfacData;
	CMap<T_ELEM_K,T_ELEM_K,double,double>					m_arReduData;
	CMap<T_ELEM_K,T_ELEM_K,int,int>								m_arReduKind;
	CMap<T_QFAC_K,T_QFAC_K&,Q_FACTOR,Q_FACTOR&>   m_arQfacData;  //Add by GAY.('06.02.24).

// Member Functions.
public:
	CDgnDynData();
	virtual ~CDgnDynData();

	static CDgnDynData* GetDgnDynData();
	void Initialize();

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
};

#include "HeaderPost.h"

#endif // !defined(__DGNDYNDATA_H__)
