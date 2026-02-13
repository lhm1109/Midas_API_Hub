// DgnDynData.cpp: implementation of the CDgnDynData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnDynData.h"
	// _ULS.
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\DB_ST_DT.h"					// for HASHSIZEELEM.

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnDynData::CDgnDynData()
{
	m_arKfacData.InitHashTable(HASHSIZEELEM);
	m_arReduData.InitHashTable(HASHSIZEELEM);
	m_arReduKind.InitHashTable(HASHSIZEELEM);
	m_arQfacData.InitHashTable(HASHSIZEELEM);
}

CDgnDynData::~CDgnDynData()
{
}

CDgnDynData* CDgnDynData::GetDgnDynData()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(pDoc==NULL)	return 0;

	CDgnDynData* pDgnDynData = (CDgnDynData*)pDoc->m_pDynModuleMgr->GetDynModule(_ULS(CDgnDynData));
	if(pDgnDynData==NULL)
	{
		pDgnDynData = new CDgnDynData;
		pDgnDynData->Initialize();
		pDoc->m_pDynModuleMgr->AddDynModule(_ULS(CDgnDynData), pDgnDynData);
	}
	return pDgnDynData;
}

void CDgnDynData::Initialize()
{
	m_arKfacData.RemoveAll();
	m_arReduData.RemoveAll();
	m_arReduKind.RemoveAll();
	m_arQfacData.RemoveAll();
}

void CDgnDynData::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_SEL_POST:
#ifdef _MGEN_CH
		m_arReduData.RemoveAll();
		m_arReduKind.RemoveAll();
		m_arQfacData.RemoveAll();
#else
		Initialize();
#endif
		break;
	default:
		break;
	}
}

void CDgnDynData::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;	
	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	while(pos)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
			case(UR_DCTL_ADD):
			case(UR_DCTL_DEL):
				Initialize();
				break;
			default:
				break;
		}
	}
}
