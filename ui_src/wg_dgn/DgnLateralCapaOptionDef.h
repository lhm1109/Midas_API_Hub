#if !defined(AFX_DgnLateralCapaOptionDef_H__8CF357FC_BFF2_429E_BC8E_83C992D560D3__INCLUDED_)
#define AFX_DgnLateralCapaOptionDef_H__8CF357FC_BFF2_429E_BC8E_83C992D560D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"

struct LateralCapaDataAll
{
	void Init()
	{
		MpgbD.Initialize();
		RlcoD.Initialize();
		aMphgK.RemoveAll();
		aMphgD.RemoveAll();
		aMpstK.RemoveAll();
		aMpstD.RemoveAll();
	}
	T_MPGB_D MpgbD;
	T_RLCO_D RlcoD;
	CArray<T_MPHG_K, T_MPHG_K> aMphgK;
	CArray<T_MPHG_D, T_MPHG_D&> aMphgD;
	CArray<T_MPST_K, T_MPST_K> aMpstK;
	CArray<T_MPST_D, T_MPST_D&> aMpstD;
};

#endif // !defined(AFX_DgnLateralCapaOptionDef_H__8CF357FC_BFF2_429E_BC8E_83C992D560D3__INCLUDED_)
