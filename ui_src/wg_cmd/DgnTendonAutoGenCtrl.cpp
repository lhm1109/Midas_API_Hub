#include "StdAfx.h"

#include "DgnTendonAutoGenCtrl.h"
#include "DgnTendonTemplateCtrl.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\UnitCtrl.h"

#include "..\wg_base\wg_base_MsgDll.h"
#include "CMSectItemPSC_I_DB.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_db\CMSectImportSPC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDgnTendonAutoGenCtrl::CDgnTendonAutoGenCtrl(CAttrCtrl* pAttrCtrl, CDgnTendonTemplateCtrl* pTemplateCtrl)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pAttrCtrl = pAttrCtrl;
	m_pTemplateCtrl = pTemplateCtrl;

	SetEpsilonUnit();

	m_aStrListCode.SetSize(IDX_STR_LIST_SIZE);
	m_aStrListCode[IDX_STR_LIST_UK]  = _T("UK");
	m_aStrListCode[IDX_STR_LIST_Italy] = _T("Italy");
	m_aStrListCode[IDX_STR_LIST_USA] = _T("USA");
	m_aStrListCode[IDX_STR_LIST_AS]  = _T("AS");
	m_aStrListCode[IDX_STR_LIST_NZ]  = _T("NZ");

	m_aStrList[IDX_STR_LIST_UK].RemoveAll();
	m_aStrList[IDX_STR_LIST_UK].SetSize(EN_SECT_TYPE_STRIDX_UK_NUBMER);

	m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_M    ] = _T("UK-M");
	m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_SY   ] = _T("UK-SY");
	m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_U    ] = _T("UK-U");
	m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_Y    ] = _T("UK-Y");
	m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_YE   ] = _T("UK-YE");	
	m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_W    ] = _T("UK-W");	
	m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_UMB  ] = _T("UK-UMB");
	m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_MY   ] = _T("UK-MY");
	m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_MYE  ] = _T("UK-MYE");
	m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_SD   ] = _T("UK-Solid Box");
	m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_SYE  ] = _T("UK-SYE");
	m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_TYE_S] = _T("UK-TYE");
	m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_TYE_B] = _T("UK-TYE(Rebate)");
	m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_TY_S ] = _T("UK-TY");
	m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_TY_B ] = _T("UK-TY(Rebate)");
	m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_T    ] = _T("UK-T");  
	m_aStrList[IDX_STR_LIST_UK][EN_SECT_TYPE_STRIDX_UK_U_SU ] = _T("UK-U_SU");

	m_aStrList[IDX_STR_LIST_Italy].RemoveAll();
	m_aStrList[IDX_STR_LIST_Italy].SetSize(EN_SECT_TYPE_STRIDX_Italy_NUMBER);
	m_aStrList[IDX_STR_LIST_Italy][EN_SECT_TYPE_STRIDX_Italy_VH] = _T("Italy-VH");

	m_aStrList[IDX_STR_LIST_USA].RemoveAll();
	m_aStrList[IDX_STR_LIST_USA].SetSize(EN_SECT_TYPE_STRIDX_USA_NUBMER);
	m_aStrList[IDX_STR_LIST_USA][EN_SECT_TYPE_STRIDX_USA_AASHTO]  = _T("AASHTO");
	m_aStrList[IDX_STR_LIST_USA][EN_SECT_TYPE_STRIDX_USA_ILDOT]   = _T("ILDOT");
	m_aStrList[IDX_STR_LIST_USA][EN_SECT_TYPE_STRIDX_USA_massDOT] = _T("massDOT");
	m_aStrList[IDX_STR_LIST_USA][EN_SECT_TYPE_STRIDX_USA_MoDOT]   = _T("MoDOT");
	m_aStrList[IDX_STR_LIST_USA][EN_SECT_TYPE_STRIDX_USA_LADOT]   = _T("LADOT");
	m_aStrList[IDX_STR_LIST_USA][EN_SECT_TYPE_STRIDX_USA_ODOT]    = _T("ODOT");
	m_aStrList[IDX_STR_LIST_USA][EN_SECT_TYPE_STRIDX_USA_RIDOT]   = _T("RIDOT");
	m_aStrList[IDX_STR_LIST_USA][EN_SECT_TYPE_STRIDX_USA_TXDOT]   = _T("TXDOT");

	m_aStrList[IDX_STR_LIST_AS].RemoveAll();
	m_aStrList[IDX_STR_LIST_AS].SetSize(EN_SECT_TYPE_STRIDX_AS_NUBMER);
	m_aStrList[IDX_STR_LIST_AS][EN_SECT_TYPE_STRIDX_AS_PSC_Deck_Unit_TMR_QLD] = _T("AS-PSC_Deck_Unit_TMR_QLD");
	m_aStrList[IDX_STR_LIST_AS][EN_SECT_TYPE_STRIDX_AS_SUPER_T_RMS_2019] = _T("AS-Super-T_RMS_2019");
	m_aStrList[IDX_STR_LIST_AS][EN_SECT_TYPE_STRIDX_AS_SUPER_T]          = _T("AS-Super-T_RMS_Pre2019");
	m_aStrList[IDX_STR_LIST_AS][EN_SECT_TYPE_STRIDX_AS_PLANK_GIRDER]     = _T("AS-Plank-Girder");
	

	m_aStrList[IDX_STR_LIST_NZ].RemoveAll();
	m_aStrList[IDX_STR_LIST_NZ].SetSize(EN_SECT_TYPE_STRIDX_NZ_NUBMER);
	m_aStrList[IDX_STR_LIST_NZ][EN_SECT_TYPE_STRIDX_NZ_SUPER_T]     = _T("NZ-Super-T");
	m_aStrList[IDX_STR_LIST_NZ][EN_SECT_TYPE_STRIDX_NZ_I_GIRDER]    = _T("NZ-I-Girder");
	m_aStrList[IDX_STR_LIST_NZ][EN_SECT_TYPE_STRIDX_NZ_HOLLOW_CORE] = _T("NZ-Hollow-Core");
	m_aStrList[IDX_STR_LIST_NZ][EN_SECT_TYPE_STRIDX_NZ_SUPERHI] = _T("NZ-SuperHI");
}


CDgnTendonAutoGenCtrl::~CDgnTendonAutoGenCtrl(void)
{
}

BOOL CDgnTendonAutoGenCtrl::GetSectTypeName(T_SECT_K tSectKey, CString& strCode, CString& strType, CString& strName)
{
	T_SECT_D tSectData;
	if(!m_pAttrCtrl->GetSect(tSectKey, tSectData))
		return FALSE;

	//현재는 PSC_VALUE (DB단면), Composite PSC 만 지원하자.
	if(!IsValidSect(tSectData)) return FALSE;

	if(!GetSectSize(tSectData, m_dSectSizeH, m_dSectSizeW)) return FALSE;

	int i;
	for(i = 0 ; i < IDX_STR_LIST_SIZE; i++)
	{
		if(AutoGenCheck(tSectData, m_aStrList[i], strType, strName))
		{
			if(!tSectData.SectBefore.SectI.SName.IsEmpty()) 
			{
				strName =tSectData.SectBefore.SectI.SName;
			}
			strCode = m_aStrListCode[i];
			return TRUE;
		}
	}

	if(AutoGenCheck_Custom(tSectData, strCode, strType, strName)) return TRUE;

	return FALSE;
}

BOOL CDgnTendonAutoGenCtrl::FindOffset(T_SECT_K tSectKey, T_AUTOTNDN_INFO& autoInfo)
{
	CString strFileName = m_pDoc->GetProgramPath() + m_pDoc->GetDBDirectory() + autoInfo.strDBName + _T(".sec");

	CFileCtrl cF(strFileName);
	CArray<T_SECT_D, T_SECT_D&> aData;
	CArray<UINT, UINT>          aID;
	int i;
	int nSize = 0;
	double dSizeHeight, dSizeWidth, dSizeHeightData, dSizeWidthData;

	if(cF.FileExists())
	{
		BOOL bImport;
		CCMSectImportSPC Importer;
		try
		{
			bImport = Importer.Import(strFileName);
		}
		catch (...)
		{
			AfxMessageBox(_LS(IDS_CMD_ERROR__NOT_EXIST_PSC_SECTDB));
			return FALSE;
		}
		
		nSize = Importer.GetList(aID, aData);
	}

	for(i = 0 ; i < nSize; i++)
	{
		T_SECT_D& tSect = aData[i];

		if(tSect.SName == autoInfo.strDBNameSub)
		{
			if(!GetSectSize(tSect, dSizeHeight, dSizeWidth, FALSE)) continue;

			m_dSectSizeH_Org = dSizeHeight;
			m_dSectSizeW_Org = dSizeWidth;

			T_SECT_D tSectData;
			if(!m_pAttrCtrl->GetSect(tSectKey, tSectData)) continue;

			if(!GetSectSize(tSectData, dSizeHeightData, dSizeWidthData, TRUE, TRUE)) continue;

			autoInfo.dOffsetLeft   = autoInfo.dOrigin[0];
			autoInfo.dOffsetBottom = autoInfo.dOrigin[1];
			autoInfo.dOffsetRight  = dSizeWidthData -  (autoInfo.dOrigin[0] + dSizeWidth );
			autoInfo.dOffsetTop    = dSizeHeightData - (autoInfo.dOrigin[1] + dSizeHeight);

			return TRUE;
		}
	}

	//위에서 없다면 Custom...
	if(FindOffset_Custom(tSectKey, m_AutoInfo))
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CDgnTendonAutoGenCtrl::FindOffset_Custom(T_SECT_K tSectKey, T_AUTOTNDN_INFO& autoInfo)
{
	CString strDBName;
	if(autoInfo.strDBName == m_aStrList[IDX_STR_LIST_USA][EN_SECT_TYPE_STRIDX_USA_ODOT])
	{
		strDBName = _LS(IDS_CMD_PSC_I_ODOT);
		if(GetOffsect_PSC_I(strDBName, tSectKey, autoInfo)) return TRUE;
	}
	else if(autoInfo.strDBName == m_aStrList[IDX_STR_LIST_USA][EN_SECT_TYPE_STRIDX_USA_AASHTO])
	{
		strDBName = _LS(IDS_CMD_PSC_I_AASHTO);
		if(GetOffsect_PSC_I(strDBName, tSectKey, autoInfo)) return TRUE;
	}

	return FALSE;
}

BOOL CDgnTendonAutoGenCtrl::GetOffsect_PSC_I(CString& strDBName, T_SECT_K tSectKey, T_AUTOTNDN_INFO& autoInfo)
{
	CCMSectItemPSCIDB SectDB;
	CStringArray aDBNameList;
	double dSizeHeight, dSizeWidth, dSizeHeightData, dSizeWidthData;
	int i, nSize;

	nSize = SectDB.GetDBNameList(strDBName, aDBNameList);

	for(i = 0 ; i < nSize; i++)
	{
		if(aDBNameList[i] == autoInfo.strDBNameSub)
		{
			T_SECT_D tSect;
			tSect.Initialize();
			tSect.SectBefore.bSymmetric = TRUE;
			tSect.nStype = D_SECT_TYPE_PSC;
			tSect.SectBefore.Shape = D_SECT_SHAPE_PSC_I;
			tSect.SectBefore.nJoint = 0;
			CString csName;
			CArray<BOOL, BOOL> aOffset;
			CArray<double, double> aSize;
			if(!SectDB.GetDBData(aDBNameList[i], aOffset, aSize)) continue;

			int j, nSize2;
			nSize2 = aSize.GetSize();
			for(j = 0 ; j < nSize2; j++)
			{
				tSect.SectBefore.SectI.Size[j] = aSize[j];
			}

			if(!GetSectSize(tSect, dSizeHeight, dSizeWidth, FALSE)) continue;

			m_dSectSizeH_Org = dSizeHeight;
			m_dSectSizeW_Org = dSizeWidth;

			T_SECT_D tSectData;
			if(!m_pAttrCtrl->GetSect(tSectKey, tSectData)) continue;

			if(!GetSectSize(tSectData, dSizeHeightData, dSizeWidthData, TRUE, TRUE)) continue;

			autoInfo.dOffsetLeft   = autoInfo.dOrigin[0];
			autoInfo.dOffsetBottom = autoInfo.dOrigin[1];
			autoInfo.dOffsetRight  = dSizeWidthData -  (autoInfo.dOrigin[0] + dSizeWidth );
			autoInfo.dOffsetTop    = dSizeHeightData - (autoInfo.dOrigin[1] + dSizeHeight);

			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDgnTendonAutoGenCtrl::AutoGeneration( T_SECT_K tSectKey, T_TDNT_K tdntKey, T_TDGR_K tdgrKey, const CString& namePrefix, const T_AUTOTNDN_INFO& autoInfo )
{
	m_AutoInfo = autoInfo;

	T_SECT_D tSectData;
	if(!m_pAttrCtrl->GetSect(tSectKey, tSectData))
		return FALSE;

	if(!GetSectSize(tSectData, m_dSectSizeH, m_dSectSizeW))
		return FALSE;

	m_keyTdnt = tdntKey;
	m_keyTdgr = tdgrKey;
	m_strPrefix = namePrefix;

	if(!FindOffset(tSectKey, m_AutoInfo))
	{
		ASSERT(0); 
		// 적용하려는 텐던 템플릿 자동생성 단면을 가져올 수없다...
		// 이렇게되면 텐던이 찌그러져서 들어가게될것이다.
	}

	T_AUTOTNDN_ARG tArg(this);
	pAutoGenFunc pFunc = GetAutoGenFunc(m_AutoInfo.strDBCode, m_AutoInfo.strDBName, m_AutoInfo.strDBNameSub, tArg);
	if(pFunc == NULL)
	{
		ASSERT(0);
		return FALSE;
	}

	return pFunc(tArg);
}

BOOL CDgnTendonAutoGenCtrl::AutoGenCheck( const T_SECT_D& tSectData, const CStringArray& aStrList, CString& strDBName, CString& strDBNameSub )
{
	int i, j, k;

	if(!IsValidSect(tSectData)) return FALSE;

	if(tSectData.SectBefore.SectI.aOuterPolygon.GetSize() == 0) return FALSE;

	int nSize_SectPolygon;
	int nSize_j;
	int nSize_i = aStrList.GetSize();

	double dSizeHeight;
	double dSizeWidth;

	BOOL bImport;
	CCMSectImportSPC Importer;
	CString strFileName;

	for(i = 0 ; i < nSize_i; i++)
	{
		strFileName = m_pDoc->GetProgramPath() + m_pDoc->GetDBDirectory() + aStrList[i] + _T(".sec");

		CFileCtrl cF(strFileName);
		if(!cF.FileExists()) continue;

		try
		{
			bImport = Importer.Import(strFileName);
		}
		catch (...)
		{
			bImport = FALSE;
		}

		if(!bImport) continue;

		nSize_j = Importer.GetList(m_raID, m_raData);

		for(j = 0 ; j < nSize_j; j++)
		{
			T_SECT_D& tSect = m_raData[j];

			if(tSectData.SectBefore.Shape == D_SECT_SHAPE_PSC_VALUE)
			{ //단면 사이즈를 먼저 구해서 다르면 넘어간다. Composite의 경우는 다를 수 있음.
				if(!GetSectSize(tSect, dSizeHeight, dSizeWidth, FALSE))
					continue;
				if(fabs(dSizeHeight - m_dSectSizeH) >= m_dCurUnitTolValu
					|| fabs(dSizeWidth - m_dSectSizeW) >= m_dCurUnitTolValu)
					continue;
			}

			nSize_SectPolygon = tSect.SectBefore.SectI.aOuterPolygon.GetSize();
			//이게 아닌 DB단면이 있을까... 일단 UK DB단면은 모두 1개.
			if(nSize_SectPolygon != 1) ASSERT(0);

			nSize_SectPolygon = tSectData.SectBefore.SectI.aOuterPolygon.GetSize();

			for(k = 0 ; k < nSize_SectPolygon; k++)
			{
				if(IsSamePolygon(tSect.SectBefore.SectI.aOuterPolygon[0], tSectData.SectBefore.SectI.aOuterPolygon[k]))
				{
					strDBName = aStrList[i];
					strDBNameSub = tSect.SName;

					if(strDBName == m_aStrList[IDX_STR_LIST_USA][EN_SECT_TYPE_STRIDX_USA_TXDOT])
					{
						if(tSect.SectBefore.SectI.aInnerPolygon.GetSize() == 1 &&
							tSectData.SectBefore.SectI.aInnerPolygon.GetSize() == 1)
						{
							if(IsSamePolygon(tSect.SectBefore.SectI.aInnerPolygon[0], tSectData.SectBefore.SectI.aInnerPolygon[0]))
								return TRUE;
						}
						continue;
					}

					return TRUE;
				}
			}
		}
	}	

	return FALSE;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenCheck_Custom(const T_SECT_D& tSectData, CString& strCode, CString& strType, CString& strName)
{
	//ODOT PSC I의 DB단면...
	if(tSectData.nStype == D_SECT_TYPE_PSC && tSectData.SectBefore.Shape == D_SECT_SHAPE_PSC_I)
	{
		CString strDBName;

		strDBName = _LS(IDS_CMD_PSC_I_ODOT);
		if(AutoGenCheck_PSC_I(strDBName, tSectData, strName))
		{
			strCode = m_aStrListCode[IDX_STR_LIST_USA];
			strType = m_aStrList[IDX_STR_LIST_USA][EN_SECT_TYPE_STRIDX_USA_ODOT];
		}

		strDBName = _LS(IDS_CMD_PSC_I_AASHTO);
		if(AutoGenCheck_PSC_I(strDBName, tSectData, strName))
		{
			strCode = m_aStrListCode[IDX_STR_LIST_USA];
			strType = m_aStrList[IDX_STR_LIST_USA][EN_SECT_TYPE_STRIDX_USA_AASHTO];
		}	
	}

	return FALSE;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenCheck_PSC_I(CString& strDBName, const T_SECT_D& tSectData, CString& strName)
{
	CCMSectItemPSCIDB SectDB;
	int nSize, i;
	BOOL bSame;
	CStringArray aDBNameList;
	CArray<double, double> aSize;
	CArray<BOOL, BOOL> aOffet;

	nSize = SectDB.GetDBNameList(strDBName, aDBNameList);

	for(i = 0 ; i < nSize; i++)
	{
		if (!SectDB.GetDBData(aDBNameList[i], aOffet, aSize)) continue;

		bSame = FALSE;
		if(tSectData.SectBefore.bSymmetric && IsSameSectWithSize(17, tSectData.SectBefore.SectI.Size, aSize.GetData()))
		{
			if(tSectData.SectBefore.nJoint | 0x00000004 && tSectData.SectBefore.nJoint | 0x00000008)
			{
				bSame = TRUE;
			}
			if(bSame)
			{
				strName = aDBNameList[i];
				return TRUE;
			}
		}
	}
	return FALSE;
}

void CDgnTendonAutoGenCtrl::SetEpsilonUnit()
{
	m_dCurUnitTolValu = m_pDoc->m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, TOL_VALU_COMP);
	m_dCurUnitTolSect = m_pDoc->m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, TOL_SECT_COMP);
}

BOOL CDgnTendonAutoGenCtrl::IsSamePolygon(const T_GSEC_POLYGON& rPolygonOrg, const T_GSEC_POLYGON& rPolygonCmp)
{
	int nSize_Org = rPolygonOrg.aVertex.GetSize();
	int nSize_Cmp = rPolygonCmp.aVertex.GetSize();

	if(nSize_Org != nSize_Cmp || nSize_Cmp <= 2)
		return FALSE;

	BOOL bReverse;
	int nIdxStart;
	double dDiffX;
	double dDiffY;

	if(!GetStartIdxAndDir(bReverse, nIdxStart, dDiffX, dDiffY,
		rPolygonOrg.aVertex, rPolygonCmp.aVertex))
	{
		return FALSE;
	}

	int l1 = 0;
	int l2 = nIdxStart;

	double dxData, dyData;
	double dx, dy;
	double dTempCalc;
	BOOL bSame = TRUE;

	while(bSame)
	{
		dxData = rPolygonCmp.aVertex[l2].dX;
		dyData = rPolygonCmp.aVertex[l2].dY;
		dx = rPolygonOrg.aVertex[l1].dX;
		dy = rPolygonOrg.aVertex[l1].dY;

		dTempCalc = dx - dxData + dDiffX;					
		if(fabs(dTempCalc) >= m_dCurUnitTolSect)
		{
			bSame = FALSE;
			break;
		}
		dTempCalc = dy - dyData + dDiffY;
		if(fabs(dTempCalc) >= m_dCurUnitTolSect)
		{
			bSame = FALSE;
			break;
		}

		l1++;
		if(bReverse) l2--;
		else            l2++;          

		l2 += nSize_Org;
		l2 %= nSize_Org;

		if(l2 == nIdxStart)
			break;
	}

	return bSame;
}

void CDgnTendonAutoGenCtrl::SetTempAry( int& nSize, double* dTempLen, double dFirst, ... )
{
	nSize = 0;

	va_list marker; // ...으로 넘어온 매개변수를 가리키는 포인터.   
	va_start( marker, dFirst ); // ...으로 넘어온 첫 번째 매개변수를 가리킴. 
	double dTemp = dFirst;
	while( fabs(dTemp - TEMP_ARY_END) > TOL_VALU_COMP) 
	{ 
		dTempLen[nSize++] = dTemp;
		dTemp = va_arg( marker, double ); // marker가 가리키는 내용을 가져오고, 
		// 다음 매개변수로 넘어감. 
	} 
	va_end( marker ); // marker를 리셋한다. (단지 0을 대입) 
}

long CDgnTendonAutoGenCtrl::GetStringsName(const CString& strCode, const CString& strType, CStringArray& strAry)
{
	CString strFileName = m_pDoc->GetProgramPath() + m_pDoc->GetDBDirectory() + strType + _T(".sec");

	strAry.RemoveAll();

	BOOL bImport;
	CCMSectImportSPC Importer;
	CFileCtrl cF(strFileName);
	CArray<T_SECT_D, T_SECT_D&> aData;
	CArray<UINT, UINT>          aID;
	int i;
	int nSize = 0;
	
	if(cF.FileExists())
	{
		try
		{
			bImport = Importer.Import(strFileName);
		}
		catch (...)
		{
			return 0;
		}
		if(!bImport) return 0;

		nSize = Importer.GetList(aID, aData);
	}

	T_AUTOTNDN_ARG tArg(this);

	strAry.RemoveAll();
	for(i = 0 ; i < nSize; i++)
	{
		if(GetAutoGenFunc(strCode, strType, aData[i].SName, tArg))
		{
			strAry.Add(aData[i].SName);
		}
	}

	CString strDBName;
	if(strType == m_aStrList[IDX_STR_LIST_USA][EN_SECT_TYPE_STRIDX_USA_ODOT])
	{
		strDBName = _LS(IDS_CMD_PSC_I_ODOT);
		GetStringsName_PSC_I(strCode, strType, strDBName, strAry);
	}
	else if(strType == m_aStrList[IDX_STR_LIST_USA][EN_SECT_TYPE_STRIDX_USA_AASHTO])
	{
		strDBName = _LS(IDS_CMD_PSC_I_AASHTO);
		GetStringsName_PSC_I(strCode, strType, strDBName, strAry);
	}

	return (long)strAry.GetSize();
}

long CDgnTendonAutoGenCtrl::GetStringsName_PSC_I(const CString& strCode, const CString& strType, CString& strDBName, CStringArray& strAry)
{
	CCMSectItemPSCIDB SectDB;
	CStringArray aDBNameList;
	T_AUTOTNDN_ARG tArg(this);
	CString strTemp;
	  
	long i;
	long lSize = SectDB.GetDBNameList(strDBName, aDBNameList);
	for(i = 0 ; i < lSize; i++)
	{
		if(GetAutoGenFunc(strCode, strType, aDBNameList[i], tArg))
		{
			strTemp.Format(_T("%s"), aDBNameList[i]);
			strAry.Add(strTemp);
		}
	}
	return lSize;
}

long CDgnTendonAutoGenCtrl::GetStringsType(const CString& strCode, CStringArray& strAry)
{
	strAry.RemoveAll();

	int i, nSize;
	nSize = m_aStrListCode.GetSize();
	for(i = 0 ; i < nSize; i++)
	{
		if(m_aStrListCode[i] == strCode)
		{
			int j, nTempSize;
			nTempSize = m_aStrList[i].GetSize();
			strAry.SetSize(nTempSize);
			for(j = 0 ; j < nTempSize; j++)
			{
				strAry[j] = m_aStrList[i][j];
			}
			return (long)nTempSize;
		}
	}

	return 0;
}

BOOL CDgnTendonAutoGenCtrl::GetSectSize( T_SECT_D& tSectData, double& dHeight, double& dWidth, BOOL bDBSect, BOOL bComposite )
{
	double hi, bi, hj, bj;
	if(bDBSect)
	{
		if(bComposite)
		{
			if(!m_pAttrCtrl->GetSectDimen(tSectData, hi, bi, hj, bj)) return FALSE;
		}
		else
		{
			if(!m_pAttrCtrl->GetSectDimen(tSectData, hi, bi, hj, bj, NULL, TRUE, TRUE)) return FALSE;
		}
		if(fabs(hi - hj) >= m_dCurUnitTolValu) return FALSE;
		if(fabs(bi - bj) >= m_dCurUnitTolValu) return FALSE;
		dHeight = hi; //높이
		dWidth = bj; //너비    
	}
	else
	{
		if(tSectData.nStype == D_SECT_TYPE_PSC && tSectData.SectBefore.Shape == D_SECT_SHAPE_PSC_I)
		{
			if(m_pAttrCtrl->GetSectDimen(tSectData, hi, bi, hj, bj, NULL, TRUE, TRUE))
			{
				if(fabs(hi - hj) >= m_dCurUnitTolValu) return FALSE;
				if(fabs(bi - bj) >= m_dCurUnitTolValu) return FALSE;
				dHeight = hi; //높이
				dWidth = bj; //너비    
			}
			else return FALSE;
		}
		else
		{
			int i, j;
			int nSize_i, nSize_j;
			double dMinX, dMaxX, dMinY, dMaxY;

			nSize_i = tSectData.SectBefore.SectI.aOuterPolygon.GetSize();
			if(nSize_i != 0 && tSectData.SectBefore.SectI.aOuterPolygon[0].aVertex.GetSize() != 0)
			{ //초기값 설정.
				dMinX = tSectData.SectBefore.SectI.aOuterPolygon[0].aVertex[0].dX;
				dMaxX = tSectData.SectBefore.SectI.aOuterPolygon[0].aVertex[0].dX;
				dMinY = tSectData.SectBefore.SectI.aOuterPolygon[0].aVertex[0].dY;
				dMaxY = tSectData.SectBefore.SectI.aOuterPolygon[0].aVertex[0].dY;
			}
			else return FALSE;

			for(i = 0; i < nSize_i; i++)
			{
				nSize_j = tSectData.SectBefore.SectI.aOuterPolygon[i].aVertex.GetSize();
				for(j = 0 ; j < nSize_j; j++)
				{
					if(dMinX > tSectData.SectBefore.SectI.aOuterPolygon[i].aVertex[j].dX)
						dMinX = tSectData.SectBefore.SectI.aOuterPolygon[i].aVertex[j].dX;
					if(dMaxX < tSectData.SectBefore.SectI.aOuterPolygon[i].aVertex[j].dX)
						dMaxX = tSectData.SectBefore.SectI.aOuterPolygon[i].aVertex[j].dX;
					if(dMinY > tSectData.SectBefore.SectI.aOuterPolygon[i].aVertex[j].dY)
						dMinY = tSectData.SectBefore.SectI.aOuterPolygon[i].aVertex[j].dY;
					if(dMaxY < tSectData.SectBefore.SectI.aOuterPolygon[i].aVertex[j].dY)
						dMaxY = tSectData.SectBefore.SectI.aOuterPolygon[i].aVertex[j].dY;
				}
			}
			dHeight = dMaxY - dMinY; //높이
			dWidth = dMaxX - dMinX; //너비
		}
	}

	dHeight = m_pDoc->m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, dHeight);
	dWidth  = m_pDoc->m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, dWidth);

	return TRUE;  
}

BOOL CDgnTendonAutoGenCtrl::GetStartIdxAndDir( BOOL& bReverse, int& nIdxStart, double& dDiffX, double& dDiffY, 
											  const CArray<T_GSEC_VERTEX, T_GSEC_VERTEX&>& aVertexOrg,
											  const CArray<T_GSEC_VERTEX, T_GSEC_VERTEX&>& aVertexComp )
{
	int i, nSize_Org, nSize_Comp, nTemp;

	nSize_Org = aVertexOrg.GetSize();
	nSize_Comp = aVertexComp.GetSize();

	if(nSize_Org != nSize_Comp)
		return FALSE;

	double dDiffXTemp, dDiffYTemp;
	BOOL bFind = FALSE;
	BOOL bInit = FALSE;

	for(i = 0 ; i < nSize_Org; i++)
	{
		dDiffXTemp = aVertexComp[i].dX - aVertexOrg[0].dX;
		dDiffYTemp = aVertexComp[i].dY - aVertexOrg[0].dY;
		bFind = FALSE;

		nTemp = i + 1;
		nTemp %= nSize_Org;
		if(fabs(aVertexComp[nTemp].dX - aVertexOrg[1].dX - dDiffXTemp) <= m_dCurUnitTolSect &&
			fabs(aVertexComp[nTemp].dY - aVertexOrg[1].dY - dDiffYTemp) <= m_dCurUnitTolSect)
		{
			if(bInit == FALSE || fabs(dDiffXTemp) < fabs(dDiffX))
			{
				nIdxStart = i;
				bReverse = FALSE;
				dDiffX = dDiffXTemp;
				dDiffY = dDiffYTemp;
				bFind = TRUE;
				bInit = TRUE;
			}
		}

		nTemp = i - 1 + nSize_Org;
		nTemp %= nSize_Org;
		if(fabs(aVertexComp[nTemp].dX - aVertexOrg[1].dX - dDiffXTemp) <= m_dCurUnitTolSect &&
			fabs(aVertexComp[nTemp].dY - aVertexOrg[1].dY - dDiffYTemp) <= m_dCurUnitTolSect)
		{
			if(bInit == FALSE || fabs(dDiffYTemp) < fabs(dDiffY))
			{
				nIdxStart = i;
				bReverse = TRUE;
				dDiffX = dDiffXTemp;
				dDiffY = dDiffYTemp;
				bFind = TRUE;
				bInit = TRUE;
			}
		}
	}

	if(bInit)
	{
		return TRUE;
	}

	nIdxStart = 0;
	bReverse = FALSE;
	dDiffX = 0.0;
	dDiffY = 0.0;
	return FALSE;  
}

void CDgnTendonAutoGenCtrl::SetInit(unsigned int tdgrK, unsigned int tdntK, TndnTemplate* tndnInfo)
{
	TndnTemplate& tempInfo = *tndnInfo;
	tempInfo.nType = 1;
	tempInfo.keyGroup = tdgrK;
	tempInfo.keyProperty = tdntK;

	tempInfo.tndnSubXY.bSameSpanInfo = TRUE;
	tempInfo.tndnSubXY.nType = TNDN_TEMPLATE_TYPE_STRAIGHT;
	m_pTemplateCtrl->GetSameSpanStr(tempInfo, TRUE, tempInfo.tndnSubXY.strSpanLength);
	tempInfo.tndnSubXY.SetSpanNum(1);

	tempInfo.tndnSubXZ.bSameSpanInfo = TRUE;
	tempInfo.tndnSubXZ.nType = TNDN_TEMPLATE_TYPE_STRAIGHT;
	m_pTemplateCtrl->GetSameSpanStr(tempInfo, FALSE, tempInfo.tndnSubXZ.strSpanLength);
	tempInfo.tndnSubXZ.SetSpanNum(1);
}

void CDgnTendonAutoGenCtrl::SetLine(double dH, double* dInterval, int nSize, TndnTemplate* tndnInfo, BOOL bSymm)
{
	double dTemp = 0.0;
	int i;
	TndnTemplate& tndnTemp = *tndnInfo;
	STRAIGHT_DT(tndnTemp, FALSE, dH, TRUE, FALSE);
	for(i = 0 ; i < nSize; i++)
	{
		dTemp += dInterval[i];
		STRAIGHT_DT(tndnTemp, TRUE, dTemp, bSymm, TRUE);
	}
}

pAutoGenFunc CDgnTendonAutoGenCtrl::GetAutoGenFunc(const CString& strCode, const CString& strName, const CString& strNameSub, T_AUTOTNDN_ARG& rArg)
{
	static BOOL bTest = FALSE;
	if(bTest) return AutoGenFuncTndn_TEST;

	for(int i = 0 ; i < IDX_STR_LIST_SIZE; i++)
	{
		if(m_aStrListCode[i] == strCode)
		{
			if(i == IDX_STR_LIST_UK)			return GetAutoGenFunc_UK(strName, strNameSub, rArg);
			else if(i == IDX_STR_LIST_Italy)   return GetAutoGenFunc_Italy(strName, strNameSub, rArg);
			else if(i == IDX_STR_LIST_USA)		return GetAutoGenFunc_USA(strName, strNameSub, rArg);
			else if(i == IDX_STR_LIST_AS)		return GetAutoGenFunc_AS(strName, strNameSub, rArg);
			else if(i == IDX_STR_LIST_NZ)		return GetAutoGenFunc_NZ(strName, strNameSub, rArg);
			else ASSERT(0); //함수 포인터 찾는 코드를 추가한다.
		}
	}

	return NULL;
}

BOOL CDgnTendonAutoGenCtrl::IsSameSectWithSize(int nSize, const double* dSect, const double* dComp)
{
	int i;
	for(i = 0 ; i < nSize; i++)
	{
		if(fabs(dSect[i] - dComp[i]) >= m_dCurUnitTolSect)
			return FALSE;
	}
	return TRUE;
}

BOOL CDgnTendonAutoGenCtrl::IsValidSect(const T_SECT_D& tSect)
{
	if(tSect.SectBefore.Shape == D_SECT_SHAPE_PSC_VALUE)  return TRUE;
	if(tSect.SectBefore.Shape == D_SECT_SHAPE_COMPO_PC)   return TRUE;
	if(tSect.SectBefore.Shape == D_SECT_SHAPE_COMPO_G)    return TRUE;
	if(tSect.nStype == D_SECT_TYPE_PSC
		&& tSect.SectBefore.Shape == D_SECT_SHAPE_PSC_I)  return TRUE;

	return FALSE;
}

BOOL CDgnTendonAutoGenCtrl::AutoGenTndn_TEST( const T_AUTOTNDN_ARG& tArg )
{
	TndnTemplate tempInfo;
	SetInit(m_keyTdgr, m_keyTdnt, &tempInfo);

	STRAIGHT_DT(tempInfo, FALSE, 0.01, FALSE, FALSE); // 높이 설정 젤 위줄.
	STRAIGHT_DT(tempInfo, TRUE,  0.02, TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE,  0.03, FALSE, TRUE);

	STRAIGHT_DT(tempInfo, FALSE, 0.01, TRUE, FALSE); // 높이 설정 젤 위줄.
	STRAIGHT_DT(tempInfo, TRUE,  0.02, TRUE, TRUE);
	STRAIGHT_DT(tempInfo, TRUE,  0.03, FALSE, TRUE);

	return TRUE;
}