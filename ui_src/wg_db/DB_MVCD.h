#ifndef __DB_MVCD_DB_H__
#define __DB_MVCD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_MVCD
{
public:
	CDB_MVCD();
	virtual ~CDB_MVCD();
	CDBDoc* m_pDoc;

public:
	void Add(T_MVCD_D& rData);
	BOOL Del();

public:
	BOOL Get(T_MVCD_D& rData);
		//{return m_mvcd.Lookup(1,rData);}
	int GetCount();
		//{return m_mvcd.GetCount();}

protected:
	CMap<T_MVCD_K,T_MVCD_K,T_MVCD_D,T_MVCD_D&>m_mvcd;
};

#endif