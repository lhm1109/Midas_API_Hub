
#pragma once

class CDBDoc;
class CDB_ELEM;

class CDB_SP14SeisMembParams {
private:
	CDBDoc* doc;
	CMap<T_SP14SeisMembParams_K, T_SP14SeisMembParams_K, T_SP14SeisMembParams_D, T_SP14SeisMembParams_D&> key_to_data;

public:
	CDB_SP14SeisMembParams(CDBDoc* new_doc) : doc{ new_doc } { ASSERT(doc); }
	virtual ~CDB_SP14SeisMembParams() {}

	void Add(T_SP14SeisMembParams_K key, const T_SP14SeisMembParams_D& data, CDB_ELEM* elem);
	bool Del(T_SP14SeisMembParams_K key, CDB_ELEM* elem);

	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& out_pos, T_SP14SeisMembParams_K& out_key, T_SP14SeisMembParams_D& out_data);
	bool Get(T_SP14SeisMembParams_K key, T_SP14SeisMembParams_D& out_data);
};
