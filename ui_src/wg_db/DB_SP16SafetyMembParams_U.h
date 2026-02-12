
#pragma once

#include "DB_ST_DT.h"

class CDB_SP16SafetyMembParams_U {
public:
	CDB_SP16SafetyMembParams_U() { key_to_data.InitHashTable(HASHSIZESP16SafetyMembParams); }
	virtual ~CDB_SP16SafetyMembParams_U() {};

	void Add(T_UDRD_KEY key, T_SP16SafetyMembParams_UDRD_D& data) { key_to_data.SetAt(key, data); }
	BOOL Del(T_UDRD_KEY key) { return key_to_data.RemoveKey(key); }
	void DelAll() { key_to_data.RemoveAll(); }
	BOOL Get(T_UDRD_KEY key, T_SP16SafetyMembParams_UDRD_D& data) { return key_to_data.Lookup(key, data); }

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SP16SafetyMembParams_UDRD_D, T_SP16SafetyMembParams_UDRD_D&> key_to_data;
};
