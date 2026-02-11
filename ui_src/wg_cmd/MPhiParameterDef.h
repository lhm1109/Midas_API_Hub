#if !defined(AFX_MPhiParameterDef_H__8CF357FC_BFF2_429E_BC8E_83C992D560D3__INCLUDED_)
#define AFX_MPhiParameterDef_H__8CF357FC_BFF2_429E_BC8E_83C992D560D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"

struct MPhiDataAll
{
	T_MPGB_D MpgbD;
	CArray<T_MPHG_K, T_MPHG_K> aMphgK;
	CArray<T_MPHG_D, T_MPHG_D&> aMphgD;
	
	CArray<T_MATL_K, T_MATL_K> aMatdK;
	CArray<T_MATD_D, T_MATD_D&> aMatdD;
	
	CArray<T_MATD_REIN_K, T_MATD_REIN_K> aMatdReinK;
	CArray<T_MATD_REIN_D, T_MATD_REIN_D&> aMatdReinD;
	
	CArray<T_MPST_K, T_MPST_K> aMpstK;
	CArray<T_MPST_D, T_MPST_D&> aMpstD;

	void Init()
	{
		MpgbD.Initialize();
		aMphgK.RemoveAll();
		aMphgD.RemoveAll();
		aMatdK.RemoveAll();
		aMatdD.RemoveAll();
		aMatdReinK.RemoveAll();
		aMatdReinD.RemoveAll();
		aMpstK.RemoveAll();
		aMpstD.RemoveAll();
	}

	BOOL GetMatd(const T_MATL_K& key, OUT T_MATD_D& rData)
	{
		for (int i = 0; i < aMatdK.GetSize(); i++) {
			if (aMatdK[i] == key) {
				rData = aMatdD[i];
				return TRUE;
			}
		}
		return FALSE;
	}

	void SetMatd(const T_MATL_K& key, IN T_MATD_D& data)
	{
		for (int i = 0; i < aMatdK.GetSize(); i++) {
			if (aMatdK[i] == key) {
				aMatdD[i] = data;
				return;
			}
		}
		aMatdK.Add(key);
		aMatdD.Add(data);
	}

	BOOL ExistMatdRein(const T_MATD_REIN_K& key)
	{
		for (int i = 0; i < aMatdReinK.GetSize(); i++) {
			if (aMatdReinK[i] == key) {
				// 삭제된 것들은 초기화 되어 있어 kMatl 값이 0 이다.
				return (aMatdReinD[i].kMatl != 0);
			}
		}
		return FALSE;
	}

	BOOL GetMatdRein(const T_MATD_REIN_K& key, OUT T_MATD_REIN_D& rData)
	{
		for (int i = 0; i < aMatdReinK.GetSize(); i++) {
			if (aMatdReinK[i] == key) {
				rData = aMatdReinD[i];
				return TRUE;
			}
		}
		return FALSE;
	}

	void SetMatdRein(const T_MATD_REIN_K& key, IN T_MATD_REIN_D& data)
	{
		for (int i = 0; i < aMatdReinK.GetSize(); i++) {
			if (aMatdReinK[i] == key) {
				aMatdReinD[i] = data;
				return;
			}
		}
		aMatdReinK.Add(key);
		aMatdReinD.Add(data);
	}

	void DeleteMatdRein(const T_MATD_REIN_K& key)
	{
		for (int i = 0; i < aMatdReinK.GetSize(); i++) {
			if (aMatdReinK[i] == key) {
				// 리스트에서 삭제하지 말고 초기화 해 놓고 CMPhiParameterDlg_JP::OnOK() 에서
				// 초기화 된 것은 삭제 하도록 한다.
				aMatdReinD[i].Initialize();
				break;
			}
		}
	}
};

#endif // !defined(AFX_MPhiParameterDef_H__8CF357FC_BFF2_429E_BC8E_83C992D560D3__INCLUDED_)
