#pragma once

class CDBDoc;
class CDB_ELEM;

class CDB_SP16CombinedSectParams {
private:
	CDBDoc* doc;
	CMap<T_SP16CombinedSectParams_K, T_SP16CombinedSectParams_K, T_SP16CombinedSectParams_D, T_SP16CombinedSectParams_D&> key_to_data;

public:
	CDB_SP16CombinedSectParams(CDBDoc* new_doc) : doc{ new_doc } { ASSERT(doc); }
	virtual ~CDB_SP16CombinedSectParams() {}

	void Add(T_SP16CombinedSectParams_K key, const T_SP16CombinedSectParams_D& data, CDB_ELEM* elem);
	bool Del(T_SP16CombinedSectParams_K key, CDB_ELEM* elem);

	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& out_pos, T_SP16CombinedSectParams_K& out_key, T_SP16CombinedSectParams_D& out_data);
	bool Get(T_SP16CombinedSectParams_K key, T_SP16CombinedSectParams_D& out_data);
};
