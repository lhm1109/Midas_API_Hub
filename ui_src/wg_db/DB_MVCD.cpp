#include "stdafx.h"
#include "DB_MVCD.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MVCD::CDB_MVCD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_mvcd.InitHashTable(HASHSIZEMVCD);
}

CDB_MVCD::~CDB_MVCD()
{

}

void CDB_MVCD::Add(T_MVCD_D& rData)
{
	m_mvcd.SetAt(1,rData);
}

BOOL CDB_MVCD::Del()
{
	T_MVCD_D Data;
	BOOL bExist=m_mvcd.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_mvcd.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_MVCD::Get(T_MVCD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_mvcd.Lookup(1,rData);
}

int CDB_MVCD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mvcd.GetCount();
}
