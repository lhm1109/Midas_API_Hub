// ReportAutoRegeneration.cpp : implementation file
//

#include "stdafx.h"
#include "AutoReport_Gen_Mgr.h"
#include "AutoReport_Gen_CH_CD.h"
#include "AutoReport_Gen_CH_STL.h"
#include "AutoReport_Gen_CH_POOL.h"
#include "AutoReport_Gen_CH_PIPE.h"
#include "AutoReport_Gen_CH_SUBWAY.h"
#include "AutoReport_Gen_CH_ISGB.h"
#include "AutoReport_Gen_CH_DMGB.h"
#include "AutoReport_Gen_CH_REINFORCE.h"
#include "AutoReport_Gen_CH_APPRAISAL_Industry.h"
#include "AutoReport_Gen_CH_APPRAISAL_Civil.h"
#include "..\wg_dbLock\LockMgr.h"
#include "..\wg_dbLock\LockOption.h"
#include "LoadWordLib.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\FileCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Report_Para_CH::Report_Para_CH()
{
	Initial();
}
void Report_Para_CH::Initial()
{
	this->m_ForceOption.RemoveAll();
	this->m_lstCaseKeys.RemoveAll();
	this->m_lstGroups.RemoveAll();
	this->m_ListReport.RemoveAll();
	this->m_ListCurve.RemoveAll();
	this->m_ListSeisDir.RemoveAll();
	this->m_ListThisCase.RemoveAll();
	this->m_mapSegment.clear();
	InitCheckSegment();
	//initgroup
	Report_Key_LIST aKeys;
	aKeys.RemoveAll(); aKeys.Add(Report_Key(0, ARGC_POOL * 1000));		SetGroupKeys(aKeys, ARGC_POOL);
	aKeys.RemoveAll(); aKeys.Add(Report_Key(0, ARGC_PIPE * 1000));		SetGroupKeys(aKeys, ARGC_PIPE);
	aKeys.RemoveAll(); aKeys.Add(Report_Key(0, ARGC_SUBWAY * 1000));	SetGroupKeys(aKeys, ARGC_SUBWAY);
	aKeys.RemoveAll(); 
	aKeys.Add(Report_Key(0, ARGC_ISGB * 1000));		//隔震结构视图输出结构组,默认全部 勾选
	aKeys.Add(Report_Key(0, ARGC_ISGB * 1000 + 1));	//振型模态输出结构组 默认全部 勾选
	SetGroupKeys(aKeys, ARGC_ISGB);
	
	//
	m_mapLcom[D_LOADCASE_COMB_GENERAL] = D_LCOMTYPE_GENERAL;
	m_mapLcom[D_LOADCASE_COMB_STEEL] = D_LCOMTYPE_STEEL;
	m_mapLcom[D_LOADCASE_COMB_CONCRETE] = D_LCOMTYPE_CONCRETE;
	memset(m_nData, 0, 10);

	m_dKesi = 0.05;
	m_dEta = 0.9;
	m_bDetail = FALSE;
}
BOOL Report_Para_CH::SegmentIsCheck(int nChart, int nSeg)const
{
	auto itrCharpt = m_mapSegment.find(nChart);
	if (itrCharpt == m_mapSegment.end())
		return FALSE;
	auto itrSeg = itrCharpt->second.find(nSeg);
	if (itrSeg == itrCharpt->second.end())
		return FALSE;
	return TRUE;
}
BOOL Report_Para_CH::HaveSegmentCheck() const
{
	return m_mapSegment.size() > 0;
}
int  Report_Para_CH::GroupKeys(Report_Key_LIST &aKeys, int nType /* = 0 */)const
{
	aKeys.RemoveAll();
	for (int i = 0; i < m_lstGroups.GetCount(); i++)
	{
		if (nType == m_lstGroups[i].m_nType)
			aKeys.Add(m_lstGroups[i]);
	}
	return aKeys.GetCount();
}
//对于工况是工况类型如静力，反应谱等，对于组合是组合类别 一般+RC+SRC+STEEL
int  Report_Para_CH::AllCaseKeys(Report_Key_LIST &aKeys) const
{
	aKeys.Copy(m_lstCaseKeys);
	return aKeys.GetCount();
}
int  Report_Para_CH::CaseKeysOnly(Report_Key_LIST &aKeys) const
{
	aKeys.RemoveAll();
	for (int i = 0; i < m_lstCaseKeys.GetCount(); i++)
	{
		if (m_lstCaseKeys[i].m_nType == D_LOADCASE_SPECTRUM || m_lstCaseKeys[i].m_nType == D_LOADCASE_STATIC)
			aKeys.Add(m_lstCaseKeys[i]);
	}
	return aKeys.GetCount();
}
int  Report_Para_CH::LoadCaseKeys(UINT nType, Report_Key_LIST &aKeys) const
{
	aKeys.RemoveAll();
	for (int i = 0; i < m_lstCaseKeys.GetCount(); i++)
	{
		if (m_lstCaseKeys[i].m_nType == nType)
			aKeys.Add(m_lstCaseKeys[i]);
	}
	return aKeys.GetCount();
}
BOOL Report_Para_CH::IsUseGroup() const
{
	return m_bUseGroup;
}

//
void Report_Para_CH::SetThisForce(BOOL bIncForce)
{
	m_bIncForce = bIncForce;
}
BOOL Report_Para_CH::IsIncluThisForce() const
{
	return m_bIncForce;
}
void Report_Para_CH::SetIncludeThisForceResult(BOOL bIncThisRlt)
{
	m_bIncThisRlt = bIncThisRlt;
}
BOOL Report_Para_CH::IsIncludeThisForceResult() const
{
	return m_bIncThisRlt;
}
BOOL Report_Para_CH::IsSpfc(CDBDoc *pDoc, int &nSeisFi, int &nSiteClass, int &nnSeisDgnCategory) const
{
	BOOL bSplc = FALSE;
	CArray<T_SPLC_K, T_SPLC_K> aSplcK;
	T_SPLC_D SplcD;
	T_SPFC_D SpfcD;
	pDoc->m_pAttrCtrl->GetSplcKeyList(aSplcK);
	for (int i = 0; i < aSplcK.GetSize(); i++)
	{
		pDoc->m_pAttrCtrl->GetSplc(aSplcK[i], SplcD);
		for (int j = 0; j < SplcD.aSpfcKey.GetSize(); j++)
		{
			if (pDoc->m_pAttrCtrl->GetSpfc(SplcD.aSpfcKey[j], SpfcD) && (SpfcD.nSpecCodeType == D_SPFC_CODE_CH2019 || SpfcD.nSpecCodeType == D_SPFC_CODE_CH2010 ||
				SpfcD.nSpecCodeType == D_SPFC_CODE_CH2002 || SpfcD.nSpecCodeType == D_SPFC_CODE_CH_GB_T_51408_2021))
			{
				bSplc = TRUE;
				break;
			}
		}
		if (bSplc)
			break;
	}

	if (bSplc)
	{
		if (SpfcD.nSpecCodeType == D_SPFC_CODE_CH2019)
		{
			nSeisFi = SpfcD.CodeParam.CH2019.nSeisFortification;
			nSiteClass = SpfcD.CodeParam.CH2019.nSiteClass;
			nnSeisDgnCategory = SpfcD.CodeParam.CH2019.nSeisDgnCategory;
		}
		else if (SpfcD.nSpecCodeType == D_SPFC_CODE_CH2010)
		{
			nSeisFi = SpfcD.CodeParam.CH2010.nSeisFortification;
			nSiteClass = SpfcD.CodeParam.CH2010.nSiteClass;
			nnSeisDgnCategory = SpfcD.CodeParam.CH2010.nSeisDgnCategory;
		}
		else if (SpfcD.nSpecCodeType == D_SPFC_CODE_CH_GB_T_51408_2021)
		{
			nSeisFi = SpfcD.CodeParam.GB_T_51408_2021.nSeisFortification;
			nSiteClass = SpfcD.CodeParam.GB_T_51408_2021.nSiteClass;
			nnSeisDgnCategory = SpfcD.CodeParam.GB_T_51408_2021.nSeisDgnCategory;
		}
		else if (SpfcD.nSpecCodeType == D_SPFC_CODE_CH2002)
		{
			nSeisFi = SpfcD.CodeParam.CH2002.nSeisFortification;
			nSiteClass = SpfcD.CodeParam.CH2002.nSiteClass;
			nnSeisDgnCategory = SpfcD.CodeParam.CH2002.nSeisDgnCategory;
		}
		else
			ASSERT(FALSE);
	}
	return bSplc;
}
BOOL Report_Para_CH::GetWindPara(CDBDoc *pDoc, int &nRoughCategory, double &dBasicWindPressure) const
{
	T_WIND_D rData;
	T_WIND_K Key;
	POSITION pos = pDoc->m_pAttrCtrl->GetStartWind();
	while (pos != NULL)
	{
		pDoc->m_pAttrCtrl->GetNextWind(pos, Key, rData);
		if (rData.nCodeType == 23)
		{
			nRoughCategory = rData.CodeParam.CH2019.nRoughCategory;
			dBasicWindPressure = rData.CodeParam.CH2019.dBasicWindPressure;
			break;
		}
		else if (rData.nCodeType == 19)
		{
			nRoughCategory = rData.CodeParam.CH2012.nRoughCategory;
			dBasicWindPressure = rData.CodeParam.CH2012.dBasicWindPressure;
			break;
		}
		else if (rData.nCodeType == 9)
		{
			nRoughCategory = rData.CodeParam.CH2002.nRoughCategory;
			dBasicWindPressure = rData.CodeParam.CH2002.dBasicWindPressure;
			break;
		}
	}

	return TRUE;
}

int Report_Para_CH::AddListChkThisCase(const T_KEY_LIST& lstKeys)
{
	m_ListThisCase.Copy(lstKeys);
	return m_ListThisCase.GetCount();
}
int Report_Para_CH::ListChkThisCase(T_KEY_LIST &lstKeys) const
{
	lstKeys.RemoveAll();
	for (int i = 0; i < m_ListThisCase.GetSize(); i ++)
	{
		lstKeys.Add(m_ListThisCase[i]);
	}

	return lstKeys.GetCount();
}

int  Report_Para_CH::ListDoubleSeisDir(Report_Key_LIST& lstKeys) const
{
	lstKeys.RemoveAll();
	for (int i = 0; i < m_ListSeisDir.GetSize(); i += 2)
	{
		for (int j=0; j<2; j++)
		{
			lstKeys.Add(m_ListSeisDir[i+j]);
		}
		
	}

	return lstKeys.GetCount();
}
int Report_Para_CH::AddListDoubleSeisDir(const Report_Key_LIST& aKeys)
{
	m_ListSeisDir.Copy(aKeys);
	return m_ListSeisDir.GetCount();
}

int  Report_Para_CH::ListHysteresisCurveOption(Report_Key_LIST &aKeys)
{
	aKeys.RemoveAll();

	for (int i = 0; i < m_ListCurve.GetCount(); i++)
	{
		aKeys.Add(m_ListCurve[i]);
	}
	return aKeys.GetCount();
}
int Report_Para_CH::SetHysteresisCurveKeys(const Report_Key_LIST &aKeys)
{
	m_ListCurve.Copy(aKeys);
	return m_ListCurve.GetCount();
}
CString Report_Para_CH::SetFilePath(CString strPath)
{
	m_strPath = strPath;
	return m_strPath;
}
CString Report_Para_CH::GetFilePath()
{
	return m_strPath;
}

//
void Report_Para_CH::SetUseGroup(BOOL bUseGroup)
{
	m_bUseGroup = bUseGroup;
}
int  Report_Para_CH::LoadCombKeys(UINT LCombType, Report_Key_LIST &aKeys) const
{
	aKeys.RemoveAll();
	for (int i = 0; i < m_lstCaseKeys.GetCount(); i++)
	{
		if ((m_lstCaseKeys[i].m_nType == D_LOADCASE_COMB_GENERAL && LCombType == D_LCOMTYPE_GENERAL) ||
			(m_lstCaseKeys[i].m_nType == D_LOADCASE_COMB_STEEL && LCombType == D_LCOMTYPE_STEEL) ||
			(m_lstCaseKeys[i].m_nType == D_LOADCASE_COMB_CONCRETE && LCombType == D_LCOMTYPE_CONCRETE) ||
			(m_lstCaseKeys[i].m_nType == D_LOADCASE_COMB_SRC && LCombType == D_LCOMTYPE_SRC))
			aKeys.Add(m_lstCaseKeys[i]);
	}
	return aKeys.GetCount();
}
int  Report_Para_CH::LoadCombKeys(Report_Key_LIST &aKeys) const
{
	aKeys.RemoveAll();
	for (int i = 0; i < m_lstCaseKeys.GetCount(); i++)
	{
		if (m_lstCaseKeys[i].m_nType == D_LOADCASE_COMB_GENERAL ||
			m_lstCaseKeys[i].m_nType == D_LOADCASE_COMB_STEEL ||
			m_lstCaseKeys[i].m_nType == D_LOADCASE_COMB_CONCRETE ||
			m_lstCaseKeys[i].m_nType == D_LOADCASE_COMB_SRC)
			aKeys.Add(m_lstCaseKeys[i]);
	}
	return aKeys.GetCount();
}
int  Report_Para_CH::ForceOption(Report_Key_LIST &aKeys) const
{
	aKeys.Copy(m_ForceOption);
	return aKeys.GetCount();
}
int  Report_Para_CH::ListReportOption(Report_Key_LIST &aKeys, ReportType nType/* = ARGC_STL*/) /*const*/
{
	aKeys.RemoveAll();
	Report_Key_LIST aRelKeys;
	T_LCOM_D lcom;
	int nLcomType;
	for (int i = 0; i < m_ListReport.GetSize(); i += 4)
	{
		if (!m_mapLcom.Lookup(m_ListReport[i + 3].m_nType, nLcomType))
			continue;
		if (CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLcom(nLcomType, m_ListReport[i + 3].m_nKey, lcom) || m_ListReport[i + 3].m_nKey == 0)
		{
			for (int k = 0; k < 4; k++)
			{
				aRelKeys.Add(m_ListReport[i + k]);
			}
		}
	}
	for (int i = 0; i < aRelKeys.GetSize(); i += 4)
	{
		if (aRelKeys[i].m_nType == nType)
		{
			for (int j = 0; j < 4; j++)
				aKeys.Add(aRelKeys[i + j]);
		}
	}
	return aKeys.GetCount();
}
void Report_Para_CH::GetAdditionalDampRtoPara(double &dKesi, double &dEta, BOOL &bChk)
{
	dKesi = m_dKesi;
	dEta = m_dEta;
	bChk = m_bDetail;
}

void Report_Para_CH::SetAdditionalDampRtoPara(double dKesi, double dEta, BOOL bChk)
{
	m_dKesi = dKesi;
	m_dEta = dEta;
	m_bDetail = bChk;
}

void Report_Para_CH::GetReinforceStructureType(int& nType)
{
	nType = m_nStructureType;
}

void Report_Para_CH::SetReinforceStructureType(int nType)
{
	m_nStructureType = nType;
}
//
void Report_Para_CH::CheckSegment(int nChart, int nSeg, BOOL bCheck)
{
	auto itrCharpt = m_mapSegment.find(nChart);
	if (bCheck == FALSE && SegmentIsCheck(nChart, nSeg) == TRUE)
	{
		auto itrCharpt = m_mapSegment.find(nChart);
		itrCharpt->second.erase(nSeg);
	}
	else if (bCheck && SegmentIsCheck(nChart, nSeg) == FALSE)
	{
		auto itrCharpt = m_mapSegment.find(nChart);
		if (itrCharpt == m_mapSegment.end())
		{
			m_mapSegment.insert(make_pair(nChart, std::map<int, int>()));
			itrCharpt = m_mapSegment.find(nChart);
		}
		auto itrSeg = itrCharpt->second.find(nSeg);
		if (itrSeg == itrCharpt->second.end())
		{
			itrCharpt->second.insert(make_pair(nSeg, 1));
		}
	}

}
void Report_Para_CH::InitCheckSegment()
{
	//
	CheckSegment(1 + 1000 * ARGC_STL, 0, TRUE);
	CheckSegment(2 + 1000 * ARGC_STL, 0, TRUE);
	CheckSegment(2 + 1000 * ARGC_STL, 1, TRUE);
	CheckSegment(2 + 1000 * ARGC_STL, 2, TRUE);
	CheckSegment(2 + 1000 * ARGC_STL, 3, TRUE);
	CheckSegment(3 + 1000 * ARGC_STL, 0, TRUE);
	CheckSegment(3 + 1000 * ARGC_STL, 1, TRUE);
	CheckSegment(3 + 1000 * ARGC_STL, 2, TRUE);
	CheckSegment(3 + 1000 * ARGC_STL, 3, TRUE);
	CheckSegment(3 + 1000 * ARGC_STL, 4, TRUE);
	CheckSegment(3 + 1000 * ARGC_STL, 5, TRUE);
	CheckSegment(3 + 1000 * ARGC_STL, 6, TRUE);
	//
	CheckSegment(1000 * ARGC_POOL + 1, 0, TRUE);
	CheckSegment(1000 * ARGC_POOL + 1, 1, TRUE);
	CheckSegment(1000 * ARGC_POOL + 1, 2, TRUE);
	CheckSegment(1000 * ARGC_POOL + 1, 3, TRUE);
	CheckSegment(1000 * ARGC_POOL + 1, 4, TRUE);
	CheckSegment(1000 * ARGC_POOL + 2, 0, TRUE);
	CheckSegment(1000 * ARGC_POOL + 3, 0, TRUE);
	CheckSegment(1000 * ARGC_POOL + 4, 0, TRUE);
	CheckSegment(1000 * ARGC_POOL + 5, 0, TRUE);
	CheckSegment(1000 * ARGC_POOL + 5, 1, TRUE);
	CheckSegment(1000 * ARGC_POOL + 5, 2, TRUE);
	CheckSegment(1000 * ARGC_POOL + 6, 0, TRUE);
	CheckSegment(1000 * ARGC_POOL + 6, 1, TRUE);
	CheckSegment(1000 * ARGC_POOL + 6, 2, TRUE);
	//
	CheckSegment(1000 * ARGC_PIPE + 1, 0, TRUE);
	CheckSegment(1000 * ARGC_PIPE + 2, 0, TRUE);
	CheckSegment(1000 * ARGC_PIPE + 3, 0, TRUE);
	CheckSegment(1000 * ARGC_PIPE + 4, 0, TRUE);
	CheckSegment(1000 * ARGC_PIPE + 5, 0, TRUE);
	CheckSegment(1000 * ARGC_PIPE + 5, 1, TRUE);
	CheckSegment(1000 * ARGC_PIPE + 5, 2, TRUE);
	CheckSegment(1000 * ARGC_PIPE + 6, 0, TRUE);
	CheckSegment(1000 * ARGC_PIPE + 6, 1, TRUE);
	CheckSegment(1000 * ARGC_PIPE + 6, 2, TRUE);
	CheckSegment(1000 * ARGC_PIPE + 6, 3, TRUE);
	CheckSegment(1000 * ARGC_PIPE + 7, 0, TRUE);
	CheckSegment(1000 * ARGC_PIPE + 7, 1, TRUE);
	CheckSegment(1000 * ARGC_PIPE + 7, 2, TRUE);
	CheckSegment(1000 * ARGC_PIPE + 8, 0, TRUE);
	CheckSegment(1000 * ARGC_PIPE + 8, 1, TRUE);
	CheckSegment(1000 * ARGC_PIPE + 8, 2, TRUE);
	CheckSegment(1000 * ARGC_PIPE + 9, 0, TRUE);
	CheckSegment(1000 * ARGC_PIPE + 9, 1, TRUE);
	CheckSegment(1000 * ARGC_PIPE + 9, 2, TRUE);
	CheckSegment(1000 * ARGC_PIPE + 9, 3, TRUE);
	CheckSegment(1000 * ARGC_PIPE + 9, 4, TRUE);
	CheckSegment(1000 * ARGC_PIPE + 9, 5, TRUE);
	//
	CheckSegment(1000 * ARGC_SUBWAY + 1, 0, TRUE);
	CheckSegment(1000 * ARGC_SUBWAY + 1, 1, TRUE);
	CheckSegment(1000 * ARGC_SUBWAY + 1, 2, TRUE);
	CheckSegment(1000 * ARGC_SUBWAY + 2, 0, TRUE);
	CheckSegment(1000 * ARGC_SUBWAY + 2, 1, TRUE);
	CheckSegment(1000 * ARGC_SUBWAY + 2, 2, TRUE);
	CheckSegment(1000 * ARGC_SUBWAY + 2, 3, TRUE);
	CheckSegment(1000 * ARGC_SUBWAY + 2, 4, TRUE);
	CheckSegment(1000 * ARGC_SUBWAY + 3, 0, TRUE);
	CheckSegment(1000 * ARGC_SUBWAY + 3, 1, TRUE);
	CheckSegment(1000 * ARGC_SUBWAY + 3, 2, TRUE);
	CheckSegment(1000 * ARGC_SUBWAY + 3, 3, TRUE);
	CheckSegment(1000 * ARGC_SUBWAY + 3, 4, TRUE);
	CheckSegment(1000 * ARGC_SUBWAY + 4, 0, TRUE);
	CheckSegment(1000 * ARGC_SUBWAY + 4, 1, TRUE);
	CheckSegment(1000 * ARGC_SUBWAY + 4, 2, TRUE);
	CheckSegment(1000 * ARGC_SUBWAY + 4, 3, TRUE);
	CheckSegment(1000 * ARGC_SUBWAY + 5, 0, TRUE);
	CheckSegment(1000 * ARGC_SUBWAY + 5, 1, TRUE);
	CheckSegment(1000 * ARGC_SUBWAY + 5, 2, TRUE);
	CheckSegment(1000 * ARGC_SUBWAY + 6, 0, TRUE);
	CheckSegment(1000 * ARGC_SUBWAY + 6, 1, TRUE);
	CheckSegment(1000 * ARGC_SUBWAY + 6, 2, TRUE);
	CheckSegment(1000 * ARGC_SUBWAY + 6, 3, TRUE);
	//
	CheckSegment(1000 * ARGC_ISGB + 1, 0, TRUE);
	CheckSegment(1000 * ARGC_ISGB + 2, 0, TRUE);
	CheckSegment(1000 * ARGC_ISGB + 2, 1, TRUE);
	CheckSegment(1000 * ARGC_ISGB + 2, 2, TRUE);
	CheckSegment(1000 * ARGC_ISGB + 3, 0, TRUE);
	CheckSegment(1000 * ARGC_ISGB + 3, 1, TRUE);
	CheckSegment(1000 * ARGC_ISGB + 3, 2, TRUE);
	CheckSegment(1000 * ARGC_ISGB + 4, 0, TRUE);
	CheckSegment(1000 * ARGC_ISGB + 4, 1, TRUE);
	CheckSegment(1000 * ARGC_ISGB + 4, 2, TRUE);
	CheckSegment(1000 * ARGC_ISGB + 5, 0, TRUE);
	CheckSegment(1000 * ARGC_ISGB + 5, 1, TRUE);
	CheckSegment(1000 * ARGC_ISGB + 5, 2, TRUE);
	CheckSegment(1000 * ARGC_ISGB + 5, 3, TRUE);
	CheckSegment(1000 * ARGC_ISGB + 6, 0, TRUE);
	CheckSegment(1000 * ARGC_ISGB + 6, 1, TRUE);
	CheckSegment(1000 * ARGC_ISGB + 6, 2, TRUE);
	CheckSegment(1000 * ARGC_ISGB + 6, 3, TRUE);
	CheckSegment(1000 * ARGC_ISGB + 7, 0, TRUE);
	CheckSegment(1000 * ARGC_ISGB + 7, 1, TRUE);
	CheckSegment(1000 * ARGC_ISGB + 7, 2, TRUE);
	CheckSegment(1000 * ARGC_ISGB + 7, 3, TRUE);
	CheckSegment(1000 * ARGC_ISGB + 8, 0, TRUE);
	CheckSegment(1000 * ARGC_ISGB + 8, 1, TRUE);
	CheckSegment(1000 * ARGC_ISGB + 8, 2, TRUE);
}
int  Report_Para_CH::SetLoadCaseKeys(const Report_Key_LIST &aKeys)
{
	m_lstCaseKeys.Copy(aKeys);
	return m_lstCaseKeys.GetCount();
}

int  Report_Para_CH::SetForceOption(const Report_Key_LIST &aKeys)
{
	m_ForceOption.Copy(aKeys);
	return m_ForceOption.GetCount();
}
int  Report_Para_CH::SetGroupKeys(const Report_Key_LIST &aKeys, ReportType nType)
{
	int nLast = m_lstGroups.GetSize() - 1;
	for (int i = nLast; i >= 0; i--)
	{
		if (m_lstGroups[i].m_nType / 1000 == nType)
		{
			m_lstGroups.RemoveAt(i);
		}
	}
	m_lstGroups.Append(aKeys);
	return m_lstGroups.GetCount();
}
int Report_Para_CH::AddListReportKeys(const Report_Key_LIST& aKeys, ReportType nType)
{
	int nLast = m_ListReport.GetSize() - 1;
	for (int i = nLast; i >= 0; i -= 4)
	{
		if (m_ListReport[i - 3].m_nType == nType)
		{
			for (int k = 0; k < 4; k++)
				m_ListReport.RemoveAt(i - k);
		}
	}
	//过滤aKeys,对于错误的荷载组合不在添加;同时应该在ListReportOption的时候也过滤
	Report_Key_LIST aRelKeys;
	T_LCOM_D lcom;
	int nLcomType;
	for (int i = 0; i < aKeys.GetSize(); i += 4)
	{
		if (!m_mapLcom.Lookup(aKeys[i + 3].m_nType, nLcomType))
			continue;
		if (CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLcom(nLcomType, aKeys[i + 3].m_nKey, lcom) || aKeys[i + 3].m_nKey == 0)
		{
			for (int k = 0; k < 4; k++)
			{
				aRelKeys.Add(aKeys[i + k]);
			}
		}
	}

	m_ListReport.Append(aRelKeys);
	return m_ListReport.GetCount();
}

void Report_Para_CH::SetBufferData(int nPos, int nValue)
{
	if (nPos > 9)
	{
		ASSERT(FALSE);
		return;
	}
	m_nData[nPos] = nValue;
}
int Report_Para_CH::GetBufferData(int nPos)
{
	if (nPos > 9)
	{
		ASSERT(FALSE);
		return -1;
	}
	return m_nData[nPos];
}
//////////////////////////////////////////////////////////////////////////
AutoReport_Gen_Mgr* AutoReport_Gen_Mgr::Inst()
{
	static AutoReport_Gen_Mgr g_AutoReport_Gen_CH;
	return &g_AutoReport_Gen_CH;
}

AutoReport_Gen_Mgr::AutoReport_Gen_Mgr(ReportType nType /* = General_CH */)
{
	Para()->Initial();
	m_nType = nType;
	m_pReport = nullptr;
}
AutoReport_Gen_Mgr::~AutoReport_Gen_Mgr()
{
	ClearReporter();
}
void AutoReport_Gen_Mgr::ClearReporter()
{
	if (m_pReport)
		delete m_pReport;
	m_pReport = nullptr;
}
Report_Para_CH* AutoReport_Gen_Mgr::Para()
{
	return &m_Para;
}

BOOL AutoReport_Gen_Mgr::IsUseAutoReport(BOOL bCD /* = TRUE */)
{
#ifdef _DEBUG
	return TRUE;
#endif

#ifdef _MGEN
#ifdef _CH    
	if (!bCD)
		return CLockMgr::GetInstance()->AllowCommand(D_OPTN_ID_DYNAMICREPORT);
#endif
	return CProduct::GetTestEnvValue(_T("CD Report Test")) == _T("yes");
#endif
	return FALSE;
}

BOOL AutoReport_Gen_Mgr::HaveSegmentCheck()
{
	return m_Para.HaveSegmentCheck();
}

BOOL AutoReport_Gen_Mgr::PreMakeData(int nTemplet)
{
#ifndef _CH
	return TRUE;
#endif

	if (!IsUseAutoReport() || !HaveSegmentCheck())
		return TRUE;
#ifndef _DEBUG
	try
#endif
	{
		m_pReport = Creat(nTemplet);
		if (m_pReport == nullptr)
			return FALSE;
		BOOL bOK = m_pReport->PreMakeData();
		return bOK;
	}
#ifndef _DEBUG
	catch (...)
	{
		ASSERT(FALSE);
		return FALSE;
	}
#endif
}
CString AutoReport_Gen_Mgr::GetTemplate()
{
	CString sFilePath = CProduct::GetTestEnvValue(_T("ChRptTemplate"));
	if (sFilePath.GetLength() > 0)
	{
		if (CFileCtrl::FileExists(sFilePath))
		{
			CFileCtrl FileCtrl(CString(CDBDoc::GetDocPoint()->GetPathName()));
			CString sNewFile;
			int idx = 1;
			while (true)
			{
				sNewFile.Format(_T("%s-%d.docx"), FileCtrl.GetFilePathAndNameWithoutExtension(), idx++);
				if (CFileCtrl::FileExists(sNewFile))
					CFile::Remove(sNewFile);
				if (CFileCtrl::FileExists(sNewFile) == FALSE)
				{
					::CopyFile(sFilePath, sNewFile, FALSE);
					if (CFileCtrl::FileExists(sNewFile))
						return sNewFile;
					else
						return _T("");
				}
			}			
		}
	}
	return _T("");
}
BOOL AutoReport_Gen_Mgr::UpdateReport(int nTemplet)
{
	HINSTANCE hMod = CLoadWordLib::GetInstance()->GetWordLibrary();
	if (hMod == NULL)
		return FALSE;
	if (!PreMakeData(nTemplet))
		return FALSE;
	return AutoGeneration(nTemplet, TRUE);
}
BOOL AutoReport_Gen_Mgr::AutoGeneration(int nTemplet, BOOL bUpDate)
{
	if (!IsUseAutoReport() || !HaveSegmentCheck())
		return TRUE;
#ifndef _DEBUG
	try
#endif
	{
		m_pReport = Creat(nTemplet);
		if (m_pReport == nullptr)
			return FALSE;
		BOOL bOK = m_pReport->AutoGeneration(bUpDate);
		ClearReporter();
		return bOK;
	}
#ifndef _DEBUG
	catch (...)
	{
		ASSERT(FALSE);
		ClearReporter();
		return FALSE;
	}
#endif
}
AutoReport_Gen_Base* AutoReport_Gen_Mgr::Creat(int nTemplet)
{
	if (m_pReport != nullptr)
	{
		if (m_pReport->TempletID() == nTemplet)
			return m_pReport;
		else
		{
			delete m_pReport;
			m_pReport = nullptr;
		}
	}
	if (m_pReport == nullptr)
	{
		if (nTemplet == 0)
		{
			if (m_nType == ARGC_STL)
				m_pReport = new AutoReport_Gen_CH_STL();
			else if (m_nType == ARGC_POOL)
				m_pReport = new AutoReport_Gen_CH_POOL();
			else if (m_nType == ARGC_PIPE)
				m_pReport = new AutoReport_Gen_CH_PIPE();
			else if (m_nType == ARGC_SUBWAY)
				m_pReport = new AutoReport_Gen_CH_SUBWAY();
			else if (m_nType == ARGC_ISGB)
				m_pReport = new AutoReport_Gen_CH_ISGB();
			else if (m_nType == ARGC_DMGB)
				m_pReport = new AutoReport_Gen_CH_DMGB();
			else if (m_nType == ARGC_REIN)
				m_pReport = new AutoReport_Gen_CH_REINFORCE();
			else if (m_nType == ARGC_INDUSTRY)
				m_pReport = new AutoReport_Gen_CH_APPRAISAL_Industry();
			else if (m_nType == ARGC_CIVIL)
				m_pReport = new AutoReport_Gen_CH_APPRAISAL_Civil();
			else if (m_nType == ChuanDian_Ch)
				m_pReport = new AutoReport_Gen_CD();
		}
	}
	return m_pReport;
}