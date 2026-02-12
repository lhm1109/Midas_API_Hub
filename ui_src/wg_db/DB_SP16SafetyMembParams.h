
#pragma once

class CDBDoc;
class CDB_ELEM;

class CDB_SP16SafetyMembParams {
private:
	CDBDoc* doc;
	CMap<T_SP16SafetyMembParams_K, T_SP16SafetyMembParams_K, T_SP16SafetyMembParams_D, T_SP16SafetyMembParams_D&> key_to_data;

public:
	CDB_SP16SafetyMembParams(CDBDoc* new_doc) : doc{ new_doc } { ASSERT(doc); }
	virtual ~CDB_SP16SafetyMembParams() {}

	void Add(T_SP16SafetyMembParams_K key, const T_SP16SafetyMembParams_D& data, CDB_ELEM* elem);
	bool Del(T_SP16SafetyMembParams_K key, CDB_ELEM* elem);

	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& out_pos, T_SP16SafetyMembParams_K& out_key, T_SP16SafetyMembParams_D& out_data);
	bool Get(T_SP16SafetyMembParams_K key, T_SP16SafetyMembParams_D& out_data);
};
