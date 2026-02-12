#ifndef __DB_MVCDU_DB_H__
#define __DB_MVCDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MVCDU
{
public:
	CDB_MVCDU()
	{
		m_mvcd.InitHashTable(HASHSIZEMVCD);
	}
	virtual ~CDB_MVCDU(){};

public:
	void Add(T_UDRD_KEY Key,T_MVCD_UDRD_D& rData)
		{m_mvcd.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mvcd.RemoveKey(Key);}
	void DelAll()
		{m_mvcd.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MVCD_UDRD_D& rData)
		{return m_mvcd.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MVCD_UDRD_D,T_MVCD_UDRD_D&>m_mvcd;
};

#endif