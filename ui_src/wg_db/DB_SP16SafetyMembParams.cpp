
#include "StdAfx.h"
#include "DB_SP16SafetyMembParams.h"

#include "DBDoc.h"
#include "DB_ELEM.h"
#include "StagCtrl.h"

void CDB_SP16SafetyMembParams::Add(T_SP16SafetyMembParams_K key, const T_SP16SafetyMembParams_D& data, CDB_ELEM* elem) {
	auto temp_data = T_SP16SafetyMembParams_D{};
	if (key_to_data.Lookup(key, temp_data) && elem) {
		VERIFY(elem->DelListItem(key, LT_SP16SafetyMembParams_CMD, key));
	}
	temp_data = data;
	key_to_data.SetAt(key, temp_data);
	if (elem) {
		elem->AddListItem(key, LT_SP16SafetyMembParams_CMD, key);
	}
}

bool CDB_SP16SafetyMembParams::Del(T_SP16SafetyMembParams_K key, CDB_ELEM* elem) {
	auto temp_data = T_SP16SafetyMembParams_D{};
	const auto key_exists = key_to_data.Lookup(key, temp_data);
	if (key_exists) {
		const auto ok = key_to_data.RemoveKey(key);
		ASSERT(ok);
		if (ok && elem) {
			VERIFY(elem->DelListItem(key, LT_SP16SafetyMembParams_CMD, key));
		}
	}
	return key_exists;
}

int CDB_SP16SafetyMembParams::GetCount() {
	if (doc->m_pStagCtrl->GetCurStag() != 0 && !doc->m_pStagCtrl->IsFinalStag()) {
		return 0;
	} else {
		return key_to_data.GetCount();
	}
}

POSITION CDB_SP16SafetyMembParams::GetStart() {
	return key_to_data.GetStartPosition();
}

void CDB_SP16SafetyMembParams::GetNext(POSITION& out_pos, T_SP16SafetyMembParams_K& out_key, T_SP16SafetyMembParams_D& out_data) {
	key_to_data.GetNextAssoc(out_pos, out_key, out_data);
}

bool CDB_SP16SafetyMembParams::Get(T_SP16SafetyMembParams_K key, T_SP16SafetyMembParams_D& out_data) {
	if (doc->m_pStagCtrl->GetCurStag() != 0 && !doc->m_pStagCtrl->IsFinalStag()) {
		return false;
	} else {
		return key_to_data.Lookup(key, out_data);
	}
}
