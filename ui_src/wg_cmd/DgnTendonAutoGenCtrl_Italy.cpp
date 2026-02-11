#include "StdAfx.h"

#include "DgnTendonAutoGenCtrl.h"
#include "DgnTendonTemplateCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

pAutoGenFunc CDgnTendonAutoGenCtrl::GetAutoGenFunc_Italy(const CString& strName, const CString& strNameSub, T_AUTOTNDN_ARG& rArg)
{
	if (strName == m_aStrList[IDX_STR_LIST_Italy][EN_SECT_TYPE_STRIDX_Italy_VH])
	{
		if (strNameSub.Find(_T("VH")) == 0 || strNameSub.Find(_T("VN")) == 0)
		{
			if (strNameSub == _T("VH80N")) { rArg.Init(0); return AutoGenFuncTndn_Italy_VH; }
			else if (strNameSub == _T("VH100N")) { rArg.Init(1); return AutoGenFuncTndn_Italy_VH; }
			else if (strNameSub == _T("VH130N")) { rArg.Init(2); return AutoGenFuncTndn_Italy_VH; }
			else if (strNameSub == _T("VH140")) { rArg.Init(3); return AutoGenFuncTndn_Italy_VH; }
			else if (strNameSub == _T("VH150")) { rArg.Init(4); return AutoGenFuncTndn_Italy_VH; }
		}
	}

	// 아직 정의되지 않은 자동 생성 타입이다...
	return NULL;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_Italy_VH(const T_AUTOTNDN_ARG& tArg)
{

	int nAdditionalNum = tArg.nAdditionalNum;

	int i;
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);


	if (nAdditionalNum == 0) // VH80N
	{
		//제일 아래 1줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.05, TRUE, FALSE);
		for (i = 0; i < 7; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.9 + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.9 + (i * 0.05), FALSE, TRUE);
		}
		STRAIGHT_DT(tempInfo, TRUE, 1.25, FALSE, TRUE);

		//제일 아래 2줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.75, TRUE, FALSE);
		for (i = 0; i < 3; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.45 + (i * 0.1), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.45 + (i * 0.1), FALSE, TRUE);
		}
	}
	else if (nAdditionalNum == 1) // VH100N
	{
		//제일 아래 1줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.05, TRUE, FALSE);
		for (i = 0; i < 6; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.95 + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.95 + (i * 0.05), FALSE, TRUE);
		}
		STRAIGHT_DT(tempInfo, TRUE, 1.25, FALSE, TRUE);

		//제일 아래 2줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.09, TRUE, FALSE);
		for (i = 0; i < 6; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.95 + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.95 + (i * 0.05), FALSE, TRUE);
		}
		STRAIGHT_DT(tempInfo, TRUE, 1.25, FALSE, TRUE);

		//제일 아래 3줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.13, TRUE, FALSE);
		for (i = 0; i < 6; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.95 + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.95 + (i * 0.05), FALSE, TRUE);
		}
		STRAIGHT_DT(tempInfo, TRUE, 1.25, FALSE, TRUE);

		//제일 아래 4줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.17, TRUE, FALSE);
		for (i = 0; i < 4; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.95 + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.95 + (i * 0.05), FALSE, TRUE);
		}

		//제일 아래 5줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.95, TRUE, FALSE);
		for (i = 0; i < 3; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.45 + (i * 0.1), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.45 + (i * 0.1), FALSE, TRUE);
		}
	}
	else if (nAdditionalNum == 2) // VH130
	{
		//제일 아래 1줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.05, TRUE, FALSE);
		for (i = 0; i < 7; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.9 + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.9 + (i * 0.05), FALSE, TRUE);
		}
		STRAIGHT_DT(tempInfo, TRUE, 1.25, FALSE, TRUE);

		//제일 아래 2줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.09, TRUE, FALSE);
		for (i = 0; i < 7; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.9 + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.9 + (i * 0.05), FALSE, TRUE);
		}
		STRAIGHT_DT(tempInfo, TRUE, 1.25, FALSE, TRUE);

		//제일 아래 3줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.13, TRUE, FALSE);
		for (i = 0; i < 7; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.9 + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.9 + (i * 0.05), FALSE, TRUE);
		}
		STRAIGHT_DT(tempInfo, TRUE, 1.25, FALSE, TRUE);

		//제일 아래 4줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.17, TRUE, FALSE);
		for (i = 0; i < 7; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.9 + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.9 + (i * 0.05), FALSE, TRUE);
		}
		STRAIGHT_DT(tempInfo, TRUE, 1.25, FALSE, TRUE);

		//제일 아래 5줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.21, TRUE, FALSE);
		STRAIGHT_DT(tempInfo, TRUE, 0.9, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.9, FALSE, TRUE);

		//제일 아래 6줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.5, TRUE, FALSE);
		STRAIGHT_DT(tempInfo, TRUE, 0.79, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.79, FALSE, TRUE);

		//제일 아래 7줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.9, TRUE, FALSE);
		STRAIGHT_DT(tempInfo, TRUE, 0.66, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.66, FALSE, TRUE);

		//제일 아래 8줄.
		STRAIGHT_DT(tempInfo, FALSE, 1.26, TRUE, FALSE);
		for (i = 0; i < 2; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.45 + (i * 0.1), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.45 + (i * 0.1), FALSE, TRUE);
		}
	}
	else if (nAdditionalNum == 3) // VH140
	{
		//제일 아래 1줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.05, TRUE, FALSE);
		for (i = 0; i < 8; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.85 + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.85 + (i * 0.05), FALSE, TRUE);
		}
		STRAIGHT_DT(tempInfo, TRUE, 1.25, FALSE, TRUE);

		//제일 아래 2줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.09, TRUE, FALSE);
		for (i = 0; i < 8; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.85 + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.85 + (i * 0.05), FALSE, TRUE);
		}
		STRAIGHT_DT(tempInfo, TRUE, 1.25, FALSE, TRUE);

		//제일 아래 3줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.13, TRUE, FALSE);
		for (i = 0; i < 8; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.85 + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.85 + (i * 0.05), FALSE, TRUE);
		}
		STRAIGHT_DT(tempInfo, TRUE, 1.25, FALSE, TRUE);

		//제일 아래 4줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.17, TRUE, FALSE);
		for (i = 0; i < 8; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.85 + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.85 + (i * 0.05), FALSE, TRUE);
		}
		STRAIGHT_DT(tempInfo, TRUE, 1.25, FALSE, TRUE);

		//제일 아래 5줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.21, TRUE, FALSE);
		for (i = 0; i < 2; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.85 + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.85 + (i * 0.05), FALSE, TRUE);
		}

		//제일 아래 6줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.5, TRUE, FALSE);
		STRAIGHT_DT(tempInfo, TRUE, 0.76, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.76, FALSE, TRUE);

		//제일 아래 7줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.9, TRUE, FALSE);
		STRAIGHT_DT(tempInfo, TRUE, 0.7, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.7, FALSE, TRUE);

		//제일 아래 8줄.
		STRAIGHT_DT(tempInfo, FALSE, 1.32, TRUE, FALSE);
		for (i = 0; i < 2; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.52 + (i * 0.1), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.52 + (i * 0.1), FALSE, TRUE);
		}
	}
	else if (nAdditionalNum == 4) // VH150
	{
		//제일 아래 1줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.05, TRUE, FALSE);
		for (i = 0; i < 8; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.85 + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.85 + (i * 0.05), FALSE, TRUE);
		}
		STRAIGHT_DT(tempInfo, TRUE, 1.25, FALSE, TRUE);

		//제일 아래 2줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.09, TRUE, FALSE);
		for (i = 0; i < 8; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.85 + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.85 + (i * 0.05), FALSE, TRUE);
		}
		STRAIGHT_DT(tempInfo, TRUE, 1.25, FALSE, TRUE);

		//제일 아래 3줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.13, TRUE, FALSE);
		for (i = 0; i < 8; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.85 + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.85 + (i * 0.05), FALSE, TRUE);
		}
		STRAIGHT_DT(tempInfo, TRUE, 1.25, FALSE, TRUE);

		//제일 아래 4줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.17, TRUE, FALSE);
		for (i = 0; i < 8; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.85 + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.85 + (i * 0.05), FALSE, TRUE);
		}
		STRAIGHT_DT(tempInfo, TRUE, 1.25, FALSE, TRUE);

		//제일 아래 5줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.21, TRUE, FALSE);
		for (i = 0; i < 2; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.85 + (i * 0.05), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.85 + (i * 0.05), FALSE, TRUE);
		}

		//제일 아래 6줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.5, TRUE, FALSE);
		STRAIGHT_DT(tempInfo, TRUE, 0.76, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.76, FALSE, TRUE);

		//제일 아래 7줄.
		STRAIGHT_DT(tempInfo, FALSE, 0.9, TRUE, FALSE);
		STRAIGHT_DT(tempInfo, TRUE, 0.7, TRUE, TRUE);
		STRAIGHT_DT(tempInfo, TRUE, 0.7, FALSE, TRUE);

		//제일 아래 8줄.
		STRAIGHT_DT(tempInfo, FALSE, 1.42, TRUE, FALSE);
		for (i = 0; i < 2; i++)
		{
			STRAIGHT_DT(tempInfo, TRUE, 0.51 + (i * 0.1), TRUE, TRUE);
			STRAIGHT_DT(tempInfo, TRUE, 0.51 + (i * 0.1), FALSE, TRUE);
		}
	}

	return TRUE;
}