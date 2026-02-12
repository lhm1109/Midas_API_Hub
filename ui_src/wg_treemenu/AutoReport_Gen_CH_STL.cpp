// ReportAutoRegeneration.cpp : implementation file
//

#include "stdafx.h"
#include "AutoReport_Gen_CH_STL.h"
#include "ReportCommonFunc.h"
#include "TreeReportCtrl.h"
#include "..\wg_xl\ReportTableProcess.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_gps\GPSAMRUtil.h"
#include "LoadWordLib.h"
#include "..\wg_gr\GRAMRUDFData.h"
#include "..\wg_db\StagCtrl.h"
#include "..\wg_gr\GRenderView.h"
#include "..\wg_base\wg_baseAll.h"
#include "..\wg_base\wg_baseAllTemp.h"
#include "..\wg_gps\wg_gpsAll.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_db\StoryDisplDrift.h"
#include "..\wg_db\StoryData.h"
#include "..\wg_db\SelfWgt.h"
#include "..\wg_db\StoryLoad.h"
#include "..\wg_db\LateralLoad.h"
#include "..\wg_gps\DRW_DATA.H"
#include "..\wg_gr\I_GEN_GPSCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\DesignResult.h"
#include "..\wg_base\GraphFuncview.h"

//#include "..\wg_gr\I_GENModel.h"
//#include "..\wg_gr\GRDisplayDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const double cGen_Zero	  = 1.0E-07;

struct _GB17_Data_Ratio
{
	double dResRatio[6];	// RSx,RSy,RSz,RBy,RBz,Rcomb.
	void Initialize()
	{
		for(int i=0; i<6; i++)	{dResRatio[i] = 0.0;}
	}
};
void AutoReport_Gen_CH_STL::GetCodeInfo(CString &strCodeMatl, CString &strCodeWind, CString &strCodeQuake)
{
	CMap< CString, LPCTSTR, int, int> mapExistMatl;
	CMap< CString, LPCTSTR, int, int> mapExistWind;
	CMap< CString, LPCTSTR, int, int> mapExistQuake;
	T_KEY_LIST lstKeys;
	m_pDoc->m_pAttrCtrl->GetMatlKeyListUsedInElem(lstKeys);
	T_MATD_D matlD;
	for (int i=0; i<lstKeys.GetCount(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(lstKeys[i], matlD))
			continue;
		if(matlD.Data1.CodeName.GetLength() >0 && matlD.Data1.CodeName != _T("None") && mapExistMatl.PLookup(matlD.Data1.CodeName) == nullptr)
		{
			strCodeMatl += matlD.Data1.CodeName + _T(", ");
			mapExistMatl.SetAt(matlD.Data1.CodeName, 1);
		}
		if(matlD.Data2.CodeName.GetLength() >0 && matlD.Data2.CodeName != _T("None") && mapExistMatl.PLookup(matlD.Data2.CodeName) == nullptr)
		{
			strCodeMatl += matlD.Data2.CodeName + _T(", ");
			mapExistMatl.SetAt(matlD.Data2.CodeName, 1);
		}
	}
	strCodeMatl.TrimRight(',');

	int nWindCode = WIND_CODE_NUM;
	POSITION pos = m_pDoc->m_pAttrCtrl->GetStartWind();
	T_WIND_D windD;
	T_WIND_K windK;
	while (pos)
	{
		m_pDoc->m_pAttrCtrl->GetNextWind(pos, windK, windD);
		if(windD.nCodeType >= nWindCode)
		{
			ASSERT(FALSE);
			continue;
		}
		CString strCode = CDBLib::m_aDescWLCodeName[windD.nCodeType -1];
		if(mapExistWind.PLookup(strCode) == nullptr)
		{
			strCodeWind += strCode + _T(", ");
			mapExistWind.SetAt(strCode, 1);
		}
	}
	strCodeWind.TrimRight(',');
	//
	pos = m_pDoc->m_pAttrCtrl->GetStartSpfc();
	T_SPFC_D spfcD;
	T_SPFC_K spfcK;
	while (pos)
	{
		m_pDoc->m_pAttrCtrl->GetNextSpfc(pos, spfcK, spfcD);
		CString strCode = spfcD.SpecFuncName;
		if(mapExistWind.PLookup(strCode) == nullptr)
		{
			strCodeQuake += strCode + _T(", ");
			mapExistQuake.SetAt(strCode, 1);
		}
	}
	strCodeQuake.TrimRight(',');
}

struct _GB17_Data_Map
{
	CMap<UINT, const UINT&, _GB17_Data_Ratio, _GB17_Data_Ratio&> SortLcbData;
	void Initialize()
	{
		SortLcbData.RemoveAll();
	}
	_GB17_Data_Map()	{}
	_GB17_Data_Map(_GB17_Data_Map& sData)	{*this = sData;}
	_GB17_Data_Map& operator = (_GB17_Data_Map& sData)
	{
		_GB17_Data_Ratio SortData;
		UINT Index;
		POSITION Pos = sData.SortLcbData.GetStartPosition();
		while(Pos != NULL)
		{
			SortData.Initialize();
			sData.SortLcbData.GetNextAssoc(Pos,Index,SortData);
			SortLcbData.SetAt(Index,SortData);
		}
		return *this;
	}
};

Report_Para_CH* AutoReport_Gen_CH_STL::Para() const
{
	return AutoReport_Gen_Mgr::Inst()->Para();
}

CString AutoReport_Gen_CH_STL::MakePicture_1_1()
{
	CString strName = _T("3D Model");
	SetPreOrPostMode(FALSE);
	DrawNodeAll(FALSE);
	SetViewPoint(0);
	MakeImage_Current(strName, FALSE);
	return strName;
}

int AutoReport_Gen_CH_STL::TempletID()
{
	return 0;
}
BOOL AutoReport_Gen_CH_STL::PreMakeData()
{
	Initial();
	m_Segments.clear();
	CViewBase::GetCurView_ST()->DoInitialView();
	SetShape(FALSE);
	setTableFormatString();
	//
	GRenderView* pRenderView = (GRenderView*)(I_GENModelBase::GetCurMySelfST()->GetOwnerWnd()); ASSERT(pRenderView);
	CAutoRecoveryUnit unitSet(D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_FORCE_INDEX_N, m_pDoc->m_pUnitCtrl,TRUE); 

	WriteCharpt1();
	WriteCharpt2();
	WriteCharpt3();
	return TRUE;
}
void AutoReport_Gen_CH_STL::InsertCoverPage()
{
	GEN_IFTAG_STR pSrc;
	GEN_IFTAG pTgt;
	CString sProductInfo = _LSX(GEN);
	CString strText = _T("");
	CString strTemp=_T("");  
	T_PJCF_D data;
	if (!m_pDoc->m_pAttrCtrl->GetPjcf(data)) 
		data.Initialize();
	CString strProjectName = data.strProject;


	if(data.strProject.IsEmpty() || data.strProject == _T(""))
	{
		CString csPath = m_pDoc->GetPathName();
		CFileCtrl file(csPath);
		CFileStatus status;
		file.GetFileStatus(status);
		strProjectName = file.GetFileNameWithoutExtension(); 
	}

	strTemp.Format(_T("[DRG_FONT]|FC| size=30 bold=true align=center [/DRG_FONT]%s\n[DRG_FONT]|FI|[/DRG_FONT]"), strProjectName);
	strText += strTemp;
	strTemp.Format(_T("[DRG_FONT]|FC| size=30 bold=true align=center underline=0[/DRG_FONT]\n\n\n%s\n[DRG_FONT]|FI|[/DRG_FONT]"), _T("计算书"));
	strText += strTemp;


	CString strDate = _T("");
	CTime time;
	time=CTime::GetCurrentTime();
	int nYear = time.GetYear ();
	int nMonth = time.GetMonth ();
	int nDate = time.GetDay ();
	strDate.Format(_T("%d-%d-%d"),nYear,nMonth,nDate);
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=false align=left[/DRG_FONT]\n\n\n\n\n\n\n%s%s[DRG_FONT]|FI|[/DRG_FONT]"),FillBlank(_T(""),13), _T("设计:"));
	strText += strTemp;
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=false align=left underline=1[/DRG_FONT]%s\n[DRG_FONT]|FI|[/DRG_FONT]"), FillBlank(data.strReviewName[0]));
	strText += strTemp;
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=false align=left[/DRG_FONT]%s%s[DRG_FONT]|FI|[/DRG_FONT]"),FillBlank(_T(""),13), _T("校对:"));
	strText += strTemp;
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=false align=left underline=1[/DRG_FONT]%s\n[DRG_FONT]|FI|[/DRG_FONT]"), FillBlank(data.strReviewName[1]));
	strText += strTemp;
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=false align=left[/DRG_FONT]%s%s[DRG_FONT]|FI|[/DRG_FONT]"),FillBlank(_T(""),13),_T("审核:"));
	strText += strTemp;
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=false align=left underline=1[/DRG_FONT]%s\n[DRG_FONT]|FI|[/DRG_FONT]"), FillBlank(data.strReviewName[4]));
	strText += strTemp;
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=false align=center [/DRG_FONT]\n\n%s\n[DRG_FONT]|FI|[/DRG_FONT]"), strDate);
	strText += strTemp;

	pSrc.sProductInfo = sProductInfo;
	pSrc.sCaption = _T("计算书");
	pSrc.sCaption += _T("计算书");
	pSrc.sContents = strText;
	pSrc.sExt = _T("");
	pSrc.sExtInfo = _T("计算书");
	pSrc.sFullPath = _T("");
	pSrc.sType = _LSX(TXT);
	CReportCommonFunc::ChangeStructStr2Char(&pSrc,&pTgt);
	CReportCommonFunc::ExportData2Word(&pTgt);
	CReportCommonFunc::ExportInsertPageBreak();
}
void AutoReport_Gen_CH_STL::InsertCatalogPage()
{
	CString strText = _T("");
	strText = _T("");
	CString strTemp=_T("");

	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=true align=center[/DRG_FONT] %s \n[DRG_FONT]|FI|[/DRG_FONT]"), _T("目录"));
	strText += strTemp;

	GEN_IFTAG_STR pSrc;
	GEN_IFTAG pTgt;
	CString sProductInfo = _LSX(GEN);


	pSrc.sProductInfo = sProductInfo;
	pSrc.sCaption = _T("目录");
	pSrc.sContents = strText;
	pSrc.sExt = _T(" ");
	pSrc.sExtInfo = _T("目录");
	pSrc.sFullPath = _T(" ");
	pSrc.sType = _LSX(TXT);
	CReportCommonFunc::ChangeStructStr2Char(&pSrc,&pTgt);
	CReportCommonFunc::ExportData2Word(&pTgt);
	CReportCommonFunc::ExportInsertCatalog(2);
	CReportCommonFunc::ExportInsertPageBreak();
}
void AutoReport_Gen_CH_STL::MakeContent()
{   
	for (int i=0; i<m_Segments.size(); i++)
	{
		InsertSegmentElements(m_Segments[i]);
	}    
}
void AutoReport_Gen_CH_STL::MakeTable_CH(GenSegmentElements &genElements, T_TABLE_D &TableD,  LPCTSTR lpCaption, LPCTSTR lpBookMark)
{
	if(TableD.RecordD.nRecordRowCount <= 0)
		return;
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption		=  _T("");
	genFtagStr.sType         = _LSX(TBL);
	CReportTableProcess ReportTable;
	ReportTable.SetData(TableD);
	ReportTable.SetFilePathName(ReportTable.GetFilePathName() + lpCaption + _T(".xml"));
	ReportTable.GenerateReport();

	genFtagStr.sFullPath     = ReportTable.GetFilePathName() + lpCaption + _T(".xml");
	genFtagStr.sExtInfo      = lpBookMark == nullptr ? lpCaption : lpBookMark;
	genFtagStr.sContents	 = _T("");
	genFtagStr.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStr.sExt          = _T(" ");
	genFtagStr.iLevel        = 0;
	CH_GEN_IFTAG_STR genFtagStrTile;
	CString strContents      ;
	strContents.Format(_T("[DRG_FONT]|FC| size=11 bold=false align=center[/DRG_FONT]%s \n[DRG_FONT]|FI|[/DRG_FONT]"), lpCaption);
	genFtagStrTile.sContents     =  strContents;
	genFtagStrTile.sType         = _LSX(TXT);
	genFtagStrTile.sFullPath     = _T(" ");
	genFtagStrTile.sExtInfo      = _T("");
	genFtagStrTile.iAppendFlag   = D_WORD_APPEND_INSERT; 
	genFtagStrTile.sExt          = _T(" ");
	genFtagStrTile.iLevel        = 0;
	genElements.m_SegmentElements.push_back(genFtagStrTile);
	genElements.m_SegmentElements.push_back(genFtagStr);
}
void AutoReport_Gen_CH_STL::WriteCharpt1()
{
	if(Para()->SegmentIsCheck(1,0) == FALSE)
		return;
	GenSegmentElements genElements;
	MakeTitle(genElements, _T("工程概况"),1,0);
	m_Segments.push_back(genElements);
}
void AutoReport_Gen_CH_STL::WriteCharpt2()
{
	if(Para()->SegmentIsCheck(2,0) == FALSE)
		return;
	GenSegmentElements Elems;
	MakeTitle(Elems, _T("设计依据"), 2, 0);
	WriteCharpt2_1(Elems);
	WriteCharpt2_2(Elems);
	WriteCharpt2_3(Elems);
	m_Segments.push_back(Elems);
}
void AutoReport_Gen_CH_STL::WriteCharpt2_1(GenSegmentElements &Elems)
{    
	if(Para()->SegmentIsCheck(2,1) == FALSE)
		return;
	MakeTitle(Elems, _T("设计规范、规程及国际和地方标准"), 2, 1);  
	CString str;
	MakeAlignText(str,_T("《建筑结构可靠度设计统一标准》"),_T("GB 50068-2017"));
	MakeAlignText(str,_T("《工程结构可靠度设计统一标准》"),_T("GB 50153-2008"));
	MakeAlignText(str,_T("《建筑结构荷载规范》"),_T("GB 50009-2012"));
	MakeAlignText(str,_T("《建筑工程抗震设防分类标准》"),_T("GB 50223-2008"));
	MakeAlignText(str,_T("《建筑抗震设计规范》"),_T("GB 50011-2010(2016版)"));
	MakeAlignText(str,_T("《高层民用建筑钢结构技术规程》"),_T("JGJ99-2015"));
	MakeAlignText(str,_T("《空间网格结构技术规程》"),_T("JGJ7-2010"));
	MakeAlignText(str,_T("《钢结构设计标准》"),_T("GB 50017-2017"));
	MakeAlignText(str,_T("《建筑结构用钢板》"),_T("GB/T 19879-2015"));
	MakeAlignText(str,_T("《低合金高强度结构钢》"),_T("GB/T 1591-2018"));
	MakeAlignText(str,_T("《铸钢结构技术规程》"),_T("JGJ/T 395-2017"));
	MakeAlignText(str,_T("《建筑设计防火规范》"),_T("GB 50016-2017(2019版)"));
	MakeAlignText(str,_T("《建筑钢结构防火技术规范》"),_T("GB 51249-2017"));
	MakeText(Elems, str);
	return;
}
void AutoReport_Gen_CH_STL::WriteCharpt2_2(GenSegmentElements &Elems)
{
	if(Para()->SegmentIsCheck(2,2) == FALSE)
		return;
	MakeTitle(Elems, _T("设计标准"), 2, 2);  
	T_DSTL_D dstlD;
	if(!m_pDoc->m_pAttrCtrl->GetDstl(dstlD))
		dstlD.Initialize();
	int nSafeLevel[] = {100,50,30};
	CString strSafeLevel[] = {_T("一"),_T("二"),_T("三")};
	double dSafeLevel[] = {1.1,1.0,0.9};
	CString str1;
	str1.Format(_T("    本工程设计基准%s年,结构设计使用年限为%s年"),GetBold(nSafeLevel[dstlD.nSafeLevel]),GetBold(nSafeLevel[dstlD.nSafeLevel]));
	MakeTitle(Elems,_T("结构设计使用年限"),2,2,_T(""),1);
	MakeText(Elems,str1);
	//
	str1.Format(_T("    建筑结构安全等级为%s级，结构重要性系数γo=%s。建筑耐火等级为%s级。"),GetBold(strSafeLevel[dstlD.nSafeLevel]),GetBold(dSafeLevel[dstlD.nSafeLevel]),GetBold(_T("一")));
	MakeTitle(Elems,_T("建筑安全等级和耐火等级"),2,2,_T(""),2);
	MakeText(Elems,str1);
	//
	int nSFI[] = {6,7,7,8,8,9};
	double dSFI[] = {0.05,0.10,0.15,0.20,0.30,0.40};
	CString aStrSDC[] = {_T("一"),_T("二"),_T("三")};
	CString astrSC[]	 = {_T("I0"), _T("I1"),  _T("II"), _T("III"), _T("IV")};
	int nSfi = nSFI[0];
	double dSfi = dSFI[0];
	CString strSC = astrSC[0];
	CString strSDC = aStrSDC[0];
	int nSeisFi = 0;
	int nSiteClass = 0;
	int nSeisDgnCategory = 0;;
	BOOL bSplc = Para()->IsSpfc(m_pDoc, nSeisFi, nSiteClass, nSeisDgnCategory);
	if(bSplc)
	{
		nSfi = nSFI[nSeisFi];
		dSfi = dSFI[nSeisFi];
		strSC = astrSC[nSiteClass];
		strSDC = aStrSDC[nSeisDgnCategory];
	}
	MakeTitle(Elems,_T("结构抗震、抗风设计"),2,2,_T(""),3);
	//2.2.3.1   2.2.3.2
	{
		str1.Format(_T("    本工程抗震设防烈度为%s度，设计地震分组为第%s组，设计基本地震加速度值为%sg，场地类别为%s类。"),GetBold(nSfi),GetBold(strSDC),GetBold(dSfi),GetBold(strSC));
		MakeTitle(Elems,_T("设防烈度"),2,2,_T(""),3,1);
		MakeText(Elems,str1);
		//
		str1.Format(_T("    根据《建筑工程抗震设防分类标准》 GB 50223-2008，本属于标准设防%s，抗震构造措施按 %s度。"),GetBold(_T("丙类")),GetBold(nSfi));
		MakeTitle(Elems,_T("抗震设防类别"),2,2,_T(""),3,2);
		MakeText(Elems,str1);
		//
	}
	//2.2.3.3
	{
		CString strArr_RC[] = {_T("A"),_T("B"),_T("C"),_T("D")};
		CString strRC = strArr_RC[0];
		double dBWP = 0.3;
		//CArray<T_WVEP_K, T_WVEP_K> aKeyList;
		//T_WVEP_D rData;
		//m_pDoc->m_pAttrCtrl2->GetWvepKeyList(aKeyList);
		//for(int i = 0;i < aKeyList.GetSize();i++)
		//{
		//	m_pDoc->m_pAttrCtrl2->GetWvep(aKeyList[i],rData);
		//	if(rData.nCodeType == 23)//CH2019
		//	{
		//		strRC = strArr_RC[rData.CH2019.nRoughCategory];
		//		dBWP = rData.CH2019.dBasicWindPressure;
		//	}
		//	else if(rData.nCodeType == 19)//CH2012
		//	{
		//		strRC = strArr_RC[rData.CH2012.nRoughCategory];
		//		dBWP = rData.CH2012.dBasicWindPressure;
		//	}
		//	else if(rData.nCodeType == 9)//CH2001
		//	{
		//		strRC = strArr_RC[rData.CH2001.nRoughCategory];
		//		dBWP = rData.CH2001.dBasicWindPressure;
		//	}
		//	else ASSERT(FALSE);
		//}
		T_WIND_D rData;
		T_WIND_K Key;
		POSITION pos = m_pDoc->m_pAttrCtrl->GetStartWind();
		while(pos !=NULL)
		{			
			m_pDoc->m_pAttrCtrl->GetNextWind(pos, Key, rData);
			if(rData.nCodeType == 23)
			{
				strRC = strArr_RC[rData.CodeParam.CH2019.nRoughCategory-1];
				dBWP = rData.CodeParam.CH2019.dBasicWindPressure;
				break;
			}
			else if(rData.nCodeType == 19)
			{
				strRC = strArr_RC[rData.CodeParam.CH2012.nRoughCategory-1];
				dBWP = rData.CodeParam.CH2012.dBasicWindPressure;
				break;
			}
			else if(rData.nCodeType == 9)
			{
				strRC = strArr_RC[rData.CodeParam.CH2002.nRoughCategory-1];
				dBWP = rData.CodeParam.CH2002.dBasicWindPressure;
				break;
			}
		}

		str1.Format(_T("    基本风压:%s年重现期Wo=%skN/m2 ，地面粗糙度为%s类；本工程结构承载力按%s年重现期设计。基本雪压: W雪=%skN/m2 。"),
			GetBold(100),GetBold(dBWP),GetBold(strRC),GetBold(nSafeLevel[dstlD.nSafeLevel]),GetBold(0.7));
		MakeTitle(Elems,_T("基本风压、基本雪压"),2,2,_T(""),3,3);
		MakeText(Elems,str1);
	}
	//2.2.3.4
	{
		MakeTitle(Elems,_T("构件受弯挠度控制"),2,2,_T(""),3,4);
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 2;
		TableD.HeaderD.aTitleName.Add(_T("构件"));
		TableD.HeaderD.aTitleName.Add(_T("1.0D+1.0L"));

		TableD.RecordD.aRecordData.SetSize(3);
		TableD.RecordD.aRecordData[0].Add(_T("悬挑构件"));
		TableD.RecordD.aRecordData[0].Add(_T("L/200(悬挑长度)"));

		TableD.RecordD.aRecordData[1].Add(_T("主梁"));
		TableD.RecordD.aRecordData[1].Add(_T("L/400"));

		TableD.RecordD.aRecordData[2].Add(_T("次梁"));
		TableD.RecordD.aRecordData[2].Add(_T("L/250"));
		TableD.RecordD.nRecordRowCount = 3;
		TableD.RecordD.nRecordColCount = 2;
		MakeTable(Elems, TableD ,_T("构件受弯挠度控制"), _T("CHARPT_2_1_TBL1"));  

	}
	//
	{
		MakeTitle(Elems,_T("钢结构材料"),2,2,_T(""),3,5);
		MakeStlMatlInfo();
		T_TABLE_D StlTableD;
		//ConTableD.Initialize();
		StlTableD.Initialize();
		StlTableD.HeaderD.nHeaderRowCount = 2;
		StlTableD.HeaderD.nHeaderColCount = 6;
		StlTableD.HeaderD.aTitleName.Add(_T("材料号"));
		StlTableD.HeaderD.aTitleName.Add(_T("钢材"));
		StlTableD.HeaderD.aTitleName.Add(_T("抗拉、抗压和抗弯fy/f"));
		StlTableD.HeaderD.aTitleName.Add(_T("抗剪fv"));
		StlTableD.HeaderD.aTitleName.Add(_T("端面承压(刨平顶紧)fce"));
		StlTableD.HeaderD.aTitleName.Add(_T("钢号"));
		StlTableD.HeaderD.aTitleName.Add(_T("厚度或直径(mm)"));
		T_MERGE_UNIT_D merge;merge.Initialize();
		merge.nMergeCount = 2; merge.nStartColNumber=1; merge.nStartRowNumber=1;
		StlTableD.HeaderD.aMergeRowD.Add(merge);
		merge.nMergeCount = 2; merge.nStartColNumber=4; merge.nStartRowNumber=1;
		StlTableD.HeaderD.aMergeRowD.Add(merge);
		merge.nMergeCount = 2; merge.nStartColNumber=5; merge.nStartRowNumber=1;
		StlTableD.HeaderD.aMergeRowD.Add(merge);
		merge.nMergeCount = 2; merge.nStartColNumber=6; merge.nStartRowNumber=1;
		StlTableD.HeaderD.aMergeRowD.Add(merge);
		merge.Initialize();
		merge.nMergeCount = 2; merge.nStartColNumber=2; merge.nStartRowNumber=1;
		StlTableD.HeaderD.aMergeColD.Add(merge);

		CArray<T_MATL_K, T_MATL_K> rKeyList;
		m_pDoc->m_pAttrCtrl->GetMatlKeyListUsedInElem(rKeyList);
		int nSize = rKeyList.GetSize();
		int nConSize = 0,nStlSize = 0;
		T_MATD_D data;
		CString str;
		for (int i = 0; i < nSize; i++)
		{
			m_pDoc->m_pAttrCtrl->GetMatlDesign(rKeyList[i], data);
			//strArray.Add(data.Name);
			if(data.Type == _T("C"))
				nConSize++;
			else if(data.Type == _T("S"))
				nStlSize++;

		}
		StlTableD.RecordD.aRecordData.SetSize(nStlSize*5);

		int k = -1;
		for (int i = 0; i < nSize; i++)
		{
			m_pDoc->m_pAttrCtrl->GetMatlDesign(rKeyList[i], data);

			if(data.Type == _T("C"))
			{
				//SetConcInfo(data,ConTableD.RecordD.aRecordData[j++]);
			}
			else if(data.Type == _T("S"))
			{
				SetStlInfo(rKeyList[i],data,StlTableD.RecordD.aRecordData,k);
			}
		}    
		StlTableD.RecordD.aRecordData.FreeExtra();
		StlTableD.RecordD.nRecordRowCount = k+1;//nStlSize*5;
		StlTableD.RecordD.nRecordColCount = 6;
		/*		if(nConSize > 0)
		MakeTable(Elems, ConTableD     , _T("混凝土材料列表(单位 Mpa)"), _T("CHARPT_2_2_TBL1")); */ 
		if(nStlSize > 0)
			MakeTable(Elems, StlTableD     , _T("钢材材料列表(单位 Mpa)"), _T("CHARPT_2_2_TBL1")); 
	}
	//
	return ;
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 2;
		TableD.HeaderD.aTitleName.Add(_T("截面号"));
		TableD.HeaderD.aTitleName.Add(_T("截面名称"));
		//   
		CArray<T_SECT_K, T_SECT_K> aSectK;
		T_SECT_D SectD;
		aSectK.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetSectKeyListUsedInElem(aSectK);
		TableD.RecordD.aRecordData.SetSize(aSectK.GetSize());
		auto& record = TableD.RecordD.aRecordData;
		for(int i = 0;i < aSectK.GetSize();i++)
		{
			record[i].SetSize(2);
			m_pDoc->m_pAttrCtrl->GetSect(aSectK[i],SectD);
			record[i][0].Format(_T("%s%d"),m_strFormat,aSectK[i]);
			record[i][1] = m_strFormat+SectD.SName;
		}
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = aSectK.GetSize();
		TableD.RecordD.nRecordColCount = 2;
		MakeTable(Elems, TableD ,_T("截面名称和截面号列表"), _T("CHARPT_2_3_TBL1"));  
	}
	return ;
}
void AutoReport_Gen_CH_STL::WriteCharpt2_3(GenSegmentElements &Elems)
{
	if(Para()->SegmentIsCheck(2,3) == FALSE)
		return;
	MakeTitle(Elems, _T("主要结构计算软件"), 2, 3);  
	MakeText(Elems,_T("    钢结构分析采用大型建筑结构通用有限元分析与计算软件midas Gen （2026 V1.1版本）计算。"));
	return;
}
void AutoReport_Gen_CH_STL::WriteCharpt3  ()
{
	if(Para()->SegmentIsCheck(3,0) == FALSE)
		return;
	GenSegmentElements Elems;
	MakeTitle(Elems, _T("钢结构计算"), 3, 0);
	WriteCharpt3_1(Elems);
	WriteCharpt3_2(Elems);
	WriteCharpt3_3(Elems);
	WriteCharpt3_4(Elems);
	m_Segments.push_back(Elems);
}
void AutoReport_Gen_CH_STL::WriteCharpt3_1(GenSegmentElements &genElements)
{
	if(Para()->SegmentIsCheck(3,1) == FALSE)
		return;
	MakeTitle(genElements, _T("计算模型") ,3, 1);   
	//DrawNodeAll(FALSE);
	CString strName;
	{
		SetViewPoint(0);
		 strName = _T("结构模型图");
		ActiveElemAll();
		MakeImage_Current(strName, FALSE);
		MakePicture(genElements, strName, _T("CHARPT_5_2_FIG"));

		SetViewPoint(3);
		strName = _T("顶视图");
		//ActiveElemAll();
		MakeImage_Current(strName, FALSE);
		MakePicture(genElements, strName, _T("CHARPT_5_2_FIG1"));

		SetViewPoint(1);
		strName = _T("左立面图");
		//ActiveElemAll();
		MakeImage_Current(strName, FALSE);
		MakePicture(genElements, strName, _T("CHARPT_5_2_FIG2"));

		SetViewPoint(2);
		strName = _T("前立面图");
		//ActiveElemAll();
		MakeImage_Current(strName, FALSE);
		MakePicture(genElements, strName, _T("CHARPT_5_2_FIG3"));
	}


	if(Para()->SegmentIsCheck(3,5) == FALSE)//结构组
		return;

	SetViewPoint(0);
	Report_Key_LIST aKeys;
	Para()->GroupKeys(aKeys, ARGC_STL * 1000 +1);
	for(int j = 0,i = 0;j < aKeys.GetSize();j++)
	{
		T_GRUP_D GrupD;
		CString strBookMark;
		m_pDoc->m_pAttrCtrl->GetGrup(aKeys[j].m_nKey,GrupD);
		ActiveObj(GrupD.arKeyElem);
		strBookMark.Format(_T("CHARPT_5_2_FIG_%d"),i++);
		strName.Format(_T("结构组 %s"),GrupD.GroupName);
		MakeImage_Current(strName, FALSE);
		MakePicture(genElements, strName, strBookMark);
	}
}
void AutoReport_Gen_CH_STL::WriteCharpt3_2(GenSegmentElements &genElements)
{
	if(Para()->SegmentIsCheck(3,2) == FALSE)
		return;
	MakeTitle(genElements, _T("构件截面"), 3, 2);
	//D_DATATYPE_1ST_FLD
	//InsertReportItemSSTable(genElements);
	T_TABLE_D TableD;
	TableD.Initialize();
	TableD.HeaderD.nHeaderRowCount = 1;
	TableD.HeaderD.nHeaderColCount = 2;
	TableD.HeaderD.aTitleName.Add(_T("截面号"));
	TableD.HeaderD.aTitleName.Add(_T("截面名称"));
	//   
	CArray<T_SECT_K, T_SECT_K> aSectK;
	T_SECT_D SectD;
	aSectK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetSectKeyListUsedInElem(aSectK);
	TableD.RecordD.aRecordData.SetSize(aSectK.GetSize());
	auto& record = TableD.RecordD.aRecordData;
	for(int i = 0;i < aSectK.GetSize();i++)
	{
		record[i].SetSize(2);
		m_pDoc->m_pAttrCtrl->GetSect(aSectK[i],SectD);
		record[i][0].Format(_T("%s%d"),m_strFormat,aSectK[i]);
		record[i][1] = m_strFormat+SectD.SName;
	}
	TableD.RecordD.aRecordData.FreeExtra();
	TableD.RecordD.nRecordRowCount = aSectK.GetSize();
	TableD.RecordD.nRecordColCount = 2;
	MakeTable(genElements, TableD ,_T("截面名称和截面号列表"), _T("CHARPT_2_2_TBL1"));  
	CString strName;
	SetShape(TRUE);
	strName = _T("结构截面布置图");
	ActiveElemAll();
	I_GENModelBase	*gm = I_GENModelBase::GetCurMySelfST();
	gm->GetDispSetting()->m_Prop.bPropNo = TRUE;
	MakeImage_Current(strName, FALSE);
	MakePicture(genElements, strName, _T("CHARPT_2_3_FIG"));
	gm->GetDispSetting()->m_Prop.bPropNo = FALSE;
	SetShape(FALSE);
}
void AutoReport_Gen_CH_STL::WriteCharpt3_3(GenSegmentElements &genElements)
{
	if(Para()->SegmentIsCheck(3,3) == FALSE)
		return;
	CAutoRecoveryUnit unitSet(D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_FORCE_INDEX_KN, m_pDoc->m_pUnitCtrl, TRUE);
	MakeTitle(genElements, _T("荷载与边界条件"), 3, 3);
	CString strName;
	CString strBook;
	//SetShape(TRUE);
	//ActiveElemAll();
	CViewBase* pView = static_cast<CViewBase*>(CViewBase::GetCurView_ST());
	I_GPSModel* gpsmode = pView->GetIPM();
	I_GENModelBase* gm = I_GENModelBase::GetCurMySelfST();
	{
		gm->GetDispSetting()->m_Load.bCnld = TRUE;
		gm->GetDispSetting()->m_Load.bCnldTxt = TRUE;
		gm->GetDispSetting()->m_Load.bBmld = TRUE;
		gm->GetDispSetting()->m_Load.bPres = TRUE;
		gm->GetDispSetting()->m_Load.bArpr = TRUE;
		gm->GetDispSetting()->m_Load.bFbld = TRUE;
		gm->GetDispSetting()->m_Load.bFloorLoadArea = TRUE;
		gm->GetDispSetting()->m_Load.bPlaneLoad = TRUE;
		gm->GetDispSetting()->m_Load.bNodalTemp = TRUE;
		gm->GetDispSetting()->m_Load.bElemTemp = TRUE;
		gm->GetDispSetting()->m_Load.bFireFipa = FALSE;
		gm->GetDispSetting()->m_Load.bFireFssf = FALSE;
		gm->GetDispSetting()->m_Load.bTempGradiant = TRUE;
		gm->GetDispSetting()->m_Load.bPrestress = TRUE;
		gm->GetDispSetting()->m_Load.bPretention = TRUE;
		gm->GetDispSetting()->m_Load.bDisplacement = TRUE;
		gm->GetDispSetting()->m_Load.bWind = TRUE;
		gm->GetDispSetting()->m_Load.bSeismic = TRUE;
		gm->GetDispSetting()->m_Load.bPseudoSeismic = TRUE;
		gm->GetDispSetting()->m_Load.bWindPArea = TRUE;
		gm->GetDispSetting()->m_Load.bWindPBeam = TRUE;
		gm->GetDispSetting()->m_Load.bWindPNodal = TRUE;
		gm->GetDispSetting()->m_Load.bWindPFunc = TRUE;

		gm->GetDispSetting()->m_Load.bCnldTxt = TRUE;
		gm->GetDispSetting()->m_Load.bBmldTxt = TRUE;
		gm->GetDispSetting()->m_Load.bPresTxt = TRUE;
		gm->GetDispSetting()->m_Load.bArprTxt = TRUE;
		gm->GetDispSetting()->m_Load.bFbldTxt = TRUE;
		gm->GetDispSetting()->m_Load.bFloorLoadArea = TRUE;
		gm->GetDispSetting()->m_Load.bPlaneLoadTxt = TRUE;
		gm->GetDispSetting()->m_Load.bNodalTempTxt = TRUE;
		gm->GetDispSetting()->m_Load.bElemTempTxt = TRUE;
		gm->GetDispSetting()->m_Load.bTempGradiantTxt = TRUE;
		gm->GetDispSetting()->m_Load.bPrestressTxt = TRUE;
		gm->GetDispSetting()->m_Load.bPretentionTxt = TRUE;
		gm->GetDispSetting()->m_Load.bDisplacementTxt = TRUE;
		gm->GetDispSetting()->m_Load.bWindTxt = TRUE;
		gm->GetDispSetting()->m_Load.bSeismicTxt = TRUE;
		gm->GetDispSetting()->m_Load.bPseudoSeismicTxt = TRUE;
		gm->GetDispSetting()->m_Load.bWindPArea = TRUE;
		gm->GetDispSetting()->m_Load.bWindPBeam = TRUE;
		gm->GetDispSetting()->m_Load.bWindPNodal = TRUE;
		gm->GetDispSetting()->m_Load.bWindPFunc = TRUE;
		//gm->GetDispSetting()->m_Load.bTxtAll  = TRUE;
	}
	T_KEY_LIST lstLoad;
	T_KEY_LIST tempLoad[2];
	T_KEY_LIST windload;
	T_STLD_D StldD;
	m_pDoc->m_pAttrCtrl2->GetUsedStldKeyList(lstLoad);
	for (int k=0; k<lstLoad.GetCount(); k++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetStld(lstLoad[k], StldD))
			continue;
		if(StldD.LoadCaseType == _T("D"))
			tempLoad[0].Add(lstLoad[k]);
		else if(StldD.LoadCaseType == _T("L"))
			tempLoad[1].Add(lstLoad[k]);
		else if(StldD.LoadCaseType == _T("W"))
			windload.Add(lstLoad[k]);
	}
	gpsmode->m_LoadCaseType = D_LOADCASE_STATIC;
	int nIndex = 0;
	for(int i = 0;i < 2;i++)
	{
		if(i == 0 && !tempLoad[i].IsEmpty())
		{
			strName = _T("恒荷载");
			MakeTitle(genElements,strName,3,3,_T(""),1);
			strName.Format(_T("    结构自重由软件自动计算考虑，除结构自重外，其他恒载：%s，恒荷载布置如下图所示（单位：kN,m）："),GetBold(_T("#用户自行输入#")));
			MakeText(genElements,strName);
		}
		else if(i == 1 && !tempLoad[i].IsEmpty())
		{
			strName = _T("活荷载");
			MakeTitle(genElements,strName,3,3,_T(""),2);
			strName.Format(_T("    %s，活荷载布置如下图所示（单位：kN,m）："),GetBold(_T("#用户自行输入#")));
			MakeText(genElements,strName);
		}
		for(int j = 0;j < tempLoad[i].GetSize();j++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetStld(tempLoad[i][j], StldD))
				continue;
			strName.Format((i == 0 ? _T("恒荷载 %s") : _T("活荷载 %s")),StldD.LoadCaseName);
			strBook.Format(_T("CHARPT_3_3_%d_FIG"),++nIndex);
			gpsmode->m_LoadCaseKey = tempLoad[i][j];
			MakeImage_Current(strName, FALSE);
			MakePicture(genElements, strName, strBook);
		}
	}
	//
	strName = _T("温度荷载");
	MakeTitle(genElements,strName,3,3,_T(""),3	);
	strName.Format(_T("    %s"),GetBold(_T("#用户自行输入#")));
	MakeText(genElements,strName);
	//
	strName = _T("风荷载");
	MakeTitle(genElements,strName,3,3,_T(""),4	);
	for(int j = 0;j < windload.GetSize();j++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetStld(windload[j], StldD))
			continue;
		strName.Format(_T("风荷载 %s"),StldD.LoadCaseName);
		strBook.Format(_T("CHARPT_3_3_%d_FIG"),++nIndex);
		gpsmode->m_LoadCaseKey = windload[j];
		MakeImage_Current(strName, FALSE);
		MakePicture(genElements, strName, strBook);
	}
	{
		gm->GetDispSetting()->m_Load.bCnld = FALSE;
		gm->GetDispSetting()->m_Load.bBmld = FALSE;
		gm->GetDispSetting()->m_Load.bPres = FALSE;
		gm->GetDispSetting()->m_Load.bArpr = FALSE;
		gm->GetDispSetting()->m_Load.bFbld = FALSE;
		gm->GetDispSetting()->m_Load.bFloorLoadArea = FALSE;
		gm->GetDispSetting()->m_Load.bPlaneLoad		= FALSE;
		gm->GetDispSetting()->m_Load.bNodalTemp		= FALSE;
		gm->GetDispSetting()->m_Load.bElemTemp		= FALSE;
		//gm->GetDispSetting()->m_Load.bFireFipa		= FALSE;
		//gm->GetDispSetting()->m_Load.bFireFssf		= FALSE;
		gm->GetDispSetting()->m_Load.bTempGradiant	= FALSE;
		gm->GetDispSetting()->m_Load.bPrestress		= FALSE;
		gm->GetDispSetting()->m_Load.bPretention	= FALSE;
		gm->GetDispSetting()->m_Load.bDisplacement	= FALSE;
		gm->GetDispSetting()->m_Load.bWind			= FALSE;
		gm->GetDispSetting()->m_Load.bSeismic		= FALSE;
		gm->GetDispSetting()->m_Load.bPseudoSeismic = FALSE;
		gm->GetDispSetting()->m_Load.bWindPArea		= FALSE;
		gm->GetDispSetting()->m_Load.bWindPBeam		= FALSE;
		gm->GetDispSetting()->m_Load.bWindPNodal	= FALSE;
		gm->GetDispSetting()->m_Load.bWindPFunc		= FALSE;
		//gm->GetDispSetting()->m_Load.bTxtAll		= FALSE;

		gm->GetDispSetting()->m_Load.bCnldTxt = FALSE;
		gm->GetDispSetting()->m_Load.bBmldTxt = FALSE;
		gm->GetDispSetting()->m_Load.bPresTxt = FALSE;
		gm->GetDispSetting()->m_Load.bArprTxt = FALSE;
		gm->GetDispSetting()->m_Load.bFbldTxt = FALSE;
		gm->GetDispSetting()->m_Load.bFloorLoadArea = FALSE;
		gm->GetDispSetting()->m_Load.bPlaneLoadTxt = FALSE;
		gm->GetDispSetting()->m_Load.bNodalTempTxt = FALSE;
		gm->GetDispSetting()->m_Load.bElemTempTxt = FALSE;
		gm->GetDispSetting()->m_Load.bTempGradiantTxt = FALSE;
		gm->GetDispSetting()->m_Load.bPrestressTxt = FALSE;
		gm->GetDispSetting()->m_Load.bPretentionTxt = FALSE;
		gm->GetDispSetting()->m_Load.bDisplacementTxt = FALSE;
		gm->GetDispSetting()->m_Load.bWindTxt = FALSE;
		gm->GetDispSetting()->m_Load.bSeismicTxt = FALSE;
		gm->GetDispSetting()->m_Load.bPseudoSeismicTxt = FALSE;
		gm->GetDispSetting()->m_Load.bWindPArea = FALSE;
		gm->GetDispSetting()->m_Load.bWindPBeam = FALSE;
		gm->GetDispSetting()->m_Load.bWindPNodal = FALSE;
		gm->GetDispSetting()->m_Load.bWindPFunc = FALSE;
	}
	//3.3.5
	{
		strName = _T("荷载组合");
		MakeTitle(genElements,strName,3,3,_T(""),5	);
		MakeText(genElements,_T("    本工程采用的荷载组合如下表所示:"));

		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 3;
		TableD.HeaderD.aTitleName.Add(_T("名称"));
		TableD.HeaderD.aTitleName.Add(_T("类型"));
		TableD.HeaderD.aTitleName.Add(_T("说明"));
		//   
		auto& record = TableD.RecordD.aRecordData;
		T_LCOM_D LComD;
		UINT aLCombType[] = {/*D_LCOMTYPE_GENERAL, D_LCOMTYPE_CONCRETE,*/ D_LCOMTYPE_STEEL, /*D_LCOMTYPE_SRC*/};
		UINT aItemData[]  = {/*D_LOADCASE_COMB_GENERAL, D_LOADCASE_COMB_CONCRETE,*/ D_LOADCASE_COMB_STEEL,/* D_LOADCASE_COMB_SRC*/};
		CString aCombTypeStr[] = {_LS(IDS_CMD_LOADCOMBI_Add), _LS(IDS_CMD_LOADCOMBI_Envelope), _T("ABS"), _T("SRSS")};
		int nRowCount = 0;
		for (int i=0; i<sizeof(aLCombType)/sizeof(UINT); i++)
		{
			const UINT nType = aLCombType[i];
			const UINT nRealType = aItemData[i];
			m_pDoc->m_pAttrCtrl->GetLcomKeyList(nType, lstLoad);
			record.SetSize(lstLoad.GetCount());
			for (int k=0; k<lstLoad.GetCount(); k++)
			{
				const T_KEY nKey = lstLoad[k];           
				if(!m_pDoc->m_pAttrCtrl->GetLcom(nType, nKey, LComD))
					continue;
				record[k].SetSize(3);
				record[k][0] = LComD.LoadCombName;
				record[k][1] = aCombTypeStr[LComD.LoadCombType];
				CString str,strTemp,strDesc;
				strDesc = _T("");
				for(int i = 0;i < LComD.aCombination.GetSize();i++)
				{
					if(!m_pDoc->m_pAttrCtrl2->GetLoadCaseNameAndDesc(LComD.aCombination[i].AnalType,LComD.aCombination[i].LoadCaseKey,strName,strTemp))
						continue;
					str.Format(_T("%.2f%s"),LComD.aCombination[i].Factor,strName);
					if(!strDesc.IsEmpty())
						strDesc += _T("+");
					strDesc += str;
				}
				record[k][2] = strDesc;
				nRowCount++;
			}
		}
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = nRowCount;
		TableD.RecordD.nRecordColCount = 3;
		MakeTable(genElements, TableD ,_T("荷载组合表"), _T("CHARPT_3_3_TBL1"));  
	}
	//3.3.6
	{
		gm->GetDispSetting()->m_Bndr.bAllGroup = TRUE;
		strName = _T("边界条件");
		MakeTitle(genElements,strName,3,3,_T(""),6	);
		MakeText(genElements,_T("    本工程采用的边界条件如下表所示:"));
		//一般支承
		strName = _T("边界条件布置图(一般支承)");
		gm->GetDispSetting()->m_Bndr.bCons = TRUE;
		MakeImage_Current(strName, FALSE);
		strBook.Format(_T("CHARPT_3_3_%d_FIG"), ++nIndex);
		MakePicture(genElements, strName, strBook);
		gm->GetDispSetting()->m_Bndr.bCons = FALSE;
		//一般支承
		if(m_pDoc->m_pAttrCtrl->GetCountElnk()) 
		{
			strName = _T("边界条件布置图(节点弹性支承)");
			gm->GetDispSetting()->m_Bndr.bNspr = TRUE;
			MakeImage_Current(strName, FALSE);
			strBook.Format(_T("CHARPT_3_3_%d_FIG"), ++nIndex);
			MakePicture(genElements, strName, strBook);
			gm->GetDispSetting()->m_Bndr.bNspr = FALSE;
		}
		//弹性连接
		if(m_pDoc->m_pAttrCtrl->GetCountElnk()) 
		{
			strName = _T("弹性连接");
			gm->GetDispSetting()->m_Bndr.bElasticLink = TRUE;
			MakeImage_Current(strName, FALSE);
			strBook.Format(_T("CHARPT_3_3_%d_FIG"), ++nIndex);
			MakePicture(genElements, strName, strBook);
			gm->GetDispSetting()->m_Bndr.bElasticLink = FALSE;
		}
		//释放梁端约束
		if(m_pDoc->m_pAttrCtrl->GetCountPrls()) 
		{
			strName = _T("释放梁端约束");
			gm->GetDispSetting()->m_Bndr.bPlateEndR = TRUE;
			MakeImage_Current(strName, FALSE);
			strBook.Format(_T("CHARPT_3_3_%d_FIG"), ++nIndex);
			MakePicture(genElements, strName, strBook);
			gm->GetDispSetting()->m_Bndr.bPlateEndRSym = FALSE;
		}
		gm->GetDispSetting()->m_Bndr.bAllGroup = FALSE;
	}


}
void AutoReport_Gen_CH_STL::WriteCharpt3_4(GenSegmentElements &genElements)
{
	if(Para()->SegmentIsCheck(3,4) == FALSE)
		return;
	MakeTitle(genElements, _T("计算结果"), 3, 4);   
	CString strName;
	Report_Key_LIST aKeys,aModelKeys;
	Para()->ListReportOption(aKeys);
	SetViewPoint(0);
	CViewBase* pView = static_cast<CViewBase*>(CViewBase::GetCurView_ST());
	I_GPSModel* gm = pView->GetIPM();
	CDC *pDC = pView->GetDC();
	//
	int nHPixel  = pDC->GetDeviceCaps(HORZRES);
	double dFontRatio=0.0;
	if(nHPixel >= 1280 ) dFontRatio = 1.1;
	else if(nHPixel >= 1152 && nHPixel < 1280) dFontRatio = 1.0;
	else if(nHPixel >= 1024 && nHPixel < 1152) dFontRatio = 1.0;
	else if(nHPixel < 1024) dFontRatio = nHPixel/1400.0;
	gm->m_LegendEngine.InitFont(20*dFontRatio,14*dFontRatio);
	//
	gm->m_LoadMinMaxType	= LOADTYPE_NORMAL;
	gm->m_bLoadDataExist	= TRUE;
	gm->m_bLoadDataSet		= TRUE;
	gm->m_bContour							= TRUE;
	gm->m_DOPT.LG.m_bDrawLegend				= TRUE;//云图
	gm->m_bDeformed							= FALSE;//是否变形
	gm->m_bAnimation						= FALSE;
	gm->m_DOPT.DR.m_bDrawInactive			= FALSE;
	gm->m_DOPT.TX.m_bOutputNumber			= FALSE;//是否显示数值
	gm->m_bMdShpContour						= TRUE;//Model
	gm->m_bAnimation = FALSE;//动画模式,是否随动变换等值线
	if(gm == NULL)
		return;
	Report_Key_LIST aReportKey;
	CString strBook;
	int nIndex = 0;
	//3.4.1
	{
		strName = _T("位移结果");
		MakeTitle(genElements,strName,3,4,_T(""),1	);
		strName.Format(_T("    在1.0D+1.0L荷载作用下，结构竖向位移如下图所示。其最大相对位移为%s，位移比%s（<1/250），满足要求"),GetBold(_T("15mm")),GetBold(_T("1/600")));
		MakeText(genElements,strName);
		for(int i = 0;i < aKeys.GetCount()/4;i++)
		{
			aReportKey.RemoveAll();
			if(aKeys[i*4].m_nKey == 2)//Deform Contour
			{
				for(int j = 0;j < 4;j++)
					aReportKey.Add(aKeys[i*4+j]);
				strBook.Format(_T("CHARPT_3_4_1_FIG_%d"),++nIndex);
				WriteRlt(genElements,gm,aReportKey,strBook);
			}
		}
	}
	//3.4.2
	{
		nIndex = 0;
		strName = _T("周期及振型结果");
		MakeTitle(genElements,strName,3,4,_T(""),2	);
		WriteTable3_4_2(genElements);
		Report_Key_LIST aGroupKey;
		if(Para()->LoadCaseKeys(D_LOADCASE_EIGEN,aModelKeys))
		{
			int nIndex = 0;
			WriteModelRlt(genElements,gm,aModelKeys,aGroupKey,nIndex);
			if(Para()->SegmentIsCheck(3,6))
			{
				Para()->GroupKeys(aGroupKey, ARGC_STL * 1000 + 2);//1:Mode Group
				if(!aGroupKey.IsEmpty())
					WriteModelRlt(genElements,gm,aModelKeys,aGroupKey,nIndex);
			}
		}
	}
	//3.4.3
	{
		nIndex = 0;
		strName = _T("构件内力结果");
		MakeTitle(genElements,strName,3,4,_T(""),3	);
		strName = _T("    在典型荷载工况下，各构件典型内力结果如下:");
		MakeText(genElements,strName);
		for(int i = 0;i < aKeys.GetCount()/4;i++)
		{
			aReportKey.RemoveAll();
			if(aKeys[i*4].m_nKey == 0 || aKeys[i*4].m_nKey == 1)//Bforce TForce
			{
				for(int j = 0;j < 4;j++)
					aReportKey.Add(aKeys[i*4+j]);
				strBook.Format(_T("CHARPT_3_4_3_FIG_%d"),++nIndex);
				WriteRlt(genElements,gm,aReportKey,strBook);
			}
		}
	}
	gm->m_LoadMinMaxType	= LOADTYPE_NORMAL;
	//3.4.4
	{
		nIndex = 0;
		strName = _T("构件承载力验算比结果");
		MakeTitle(genElements,strName,3,4,_T(""),4	);
		if(!m_pDoc->m_pPostCtrl->GetDesignResult()->IsSteelDesignResultEnable())
			return;
		strName = _T("    本结构承载能力验算比结果如下:");
		MakeText(genElements,strName);
		//aReportKey.Add(Report_Key(0,D_LOADCASE_NONE));//包络
		//strBook.Format(_T("CHARPT_3_4_4_FIG_%d"),++nIndex);
		for(int i = 0;i < aKeys.GetCount()/4;i++)
		{
			aReportKey.RemoveAll();
			if(aKeys[i*4].m_nKey == 3)//Design
			{
				for(int j = 0;j < 4;j++)
					aReportKey.Add(aKeys[i*4+j]);
				strBook.Format(_T("CHARPT_3_4_4_FIG_%d"),++nIndex);
				WriteDesignRlt(genElements,gm,aReportKey,strBook);
			}
		}
	}
	//Stress Graph
	{
		aReportKey.RemoveAll();
		CMap<int,int,int,int> map_Design;
		for(int i = 0;i < aKeys.GetCount()/4;i++)
		{
			if(aKeys[i*4+3].m_nType == D_LOADCASE_COMB_STEEL && aKeys[i*4] == 3)//Design
			{
				aReportKey.Add(aKeys[i*4+1]);
				aReportKey.Add(aKeys[i*4+2]);
				aReportKey.Add(aKeys[i*4+3]);
			}
		}
		WriteDesignGraph(genElements,aReportKey);
	}
}
void AutoReport_Gen_CH_STL::WriteRlt(GenSegmentElements &Elems,I_GPSModel* gm,const Report_Key_LIST& aKeys,const CString& strBook)
{
	int nForceType[] = {0,1,2,3,4,5};
	int nGPSMode[] = {GPS_COMMAND_TFORC,GPS_COMMAND_BFORC,GPS_COMMAND_DCONTR};
	int nDisp[] = {DISP_COMPONENT_DX,DISP_COMPONENT_DY,DISP_COMPONENT_DZ,DISP_COMPONENT_RX,DISP_COMPONENT_RY,DISP_COMPONENT_RZ,DISP_COMPONENT_DXY,DISP_COMPONENT_DYZ,DISP_COMPONENT_DXZ,DISP_COMPONENT_DXYZ};
	CString strRltTypeName[] = {_T("桁架单元内力"),_T("梁单元内力"),_T("位移等值线")};
	CString strBForceType[] = {_T("FX"),_T("FY"),_T("FZ"),_T("MX"),_T("MY"),_T("MZ")};
	CString strTForceType[] = {_T("全部"),_T("受拉"),_T("受压")};
	CString strDeformType[] = {_T("DX"),_T("DY"),_T("DZ"),_T("RX"),_T("RY"),_T("RZ"),_T("DXY"),_T("DYZ"),_T("DXZ"),_T("DXYZ")};
	CString strRltName,strGroup,strDir,strLoad;
	int nMode = 0;
	T_GRUP_D GrupD;
	CString str;
	gm->m_GPSMode = gm->m_GPSInstruction = nGPSMode[aKeys[0].m_nKey]; //GPS_COMMAND_BFORC;
	gm->m_LoadCaseType		= aKeys[3].m_nType;
	gm->m_LoadCaseKey		= aKeys[3].m_nKey;
	gm->m_LoadMinMaxType	= aKeys[3].m_nSubType;

	CForceEngine ForceD(gm);
	CDeformEngine DeformD(gm);
	
	CForceEngine* pForceD = gm->m_pForceEngine;
	CDeformEngine* pDeformD = gm->m_pDeformEngine;
	if(!pForceD)
		pForceD = &CGPSBarContainer::m_ForceEngine;
	if(!pDeformD)
		pDeformD = &CGPSBarContainer::m_DeformEngine;
	if(!pForceD->m_pContourEngine)
		pForceD->m_pContourEngine = &CGPSBarContainer::m_ContourEngine;
	if(!pDeformD->m_pContourEngine)
		pDeformD->m_pContourEngine = &CGPSBarContainer::m_ContourEngine;
	ForceD.ImportEngine(pForceD);
	DeformD.ImportEngine(pDeformD);

	ForceD.m_BForcComponent	= nForceType[aKeys[2].m_nKey];
	ForceD.m_TForcComponent	= aKeys[2].m_nKey;//桁架单元默认全部内力
	ForceD.m_bYieldPoint		= FALSE;//Yield Point
	ForceD.m_bBForcWithTForc	= FALSE;//Show Truss Forces
	ForceD.m_nBForcPscPart	= 0;//Part
	ForceD.m_bDeformedShapeContour	= FALSE;
	ForceD.m_bBForcOutputMax		= TRUE;
	ForceD.m_bBForcOutputMinMax		= FALSE;
	ForceD.m_bBForcOutputAll		= FALSE;
	ForceD.m_bBForcOutputI			= FALSE;
	ForceD.m_bBForcOutputC			= FALSE;
	ForceD.m_bBForcOutputJ			= FALSE;
	ForceD.m_bBForcOutputByMember	= FALSE;
	DeformD.m_nThisComp			= 0;
	DeformD.m_bContourUpdate		= TRUE;
	nMode = nGPSMode[aKeys[0].m_nKey];
	ForceD.InitEngine();
	DeformD.InitEngine();//荷载组合变化时初始化
	DeformD.m_DispComponent = nDisp[aKeys[2].m_nKey];
	DeformD.m_bResultDataSet		= TRUE;

	strRltName = strRltTypeName[aKeys[0].m_nKey];

	int nMinMax = 0;
	if (aKeys[3].m_nSubType == LOAD_MAX)
		nMinMax = 1;
	else if (aKeys[3].m_nSubType == LOAD_MIN)
		nMinMax = -1;
	strLoad = CDBLib::GetLoadCaseNameByKey(aKeys[3].m_nType, aKeys[3].m_nKey, nMinMax);
	int nIndexForce = aKeys[2].m_nKey;

	if(nMode == GPS_COMMAND_TFORC)//Truss Force
	{
		if(aKeys[2].m_nKey != 0)
			return;
		pForceD->m_bTForcDataSet			= TRUE;
		strDir = strTForceType[nIndexForce];
	}
	else
	{
		pForceD->m_bBForcDataSet			= TRUE;
		if(nMode == GPS_COMMAND_BFORC)//Beam Force
		{
			if(nIndexForce > 5)
				return;
			strDir = strBForceType[nIndexForce];
		}
		else// Deform Contour
		{
			strDir = strDeformType[nIndexForce];
		}
	}

	gm->m_bInitialView = FALSE;
	gm->ResetEngines();
	gm->m_pForceEngine = new CForceEngine(gm);
	gm->m_pForceEngine->ImportEngine(&ForceD);
	gm->m_pDeformEngine = new CDeformEngine(gm);
	gm->m_pDeformEngine->ImportEngine(&DeformD);

	if(m_pDoc->m_pAttrCtrl->GetGrup(aKeys[1].m_nKey,GrupD))
	{
		strGroup= GrupD.GroupName;
		ActiveObj(GrupD.arKeyElem);
	}
	else
	{
		VERIFY(aKeys[1].m_nKey == 0);
		strGroup = _T("全部");
		ActiveElemAll();
	}
	str.Format(_T("%s_%s_%s_%s"),strRltName,strGroup,strDir,strLoad);
	Replace_StrPath(str);
	//if (pView->IsHiddenMode()) CViewBase::GetCurView_ST()->ToggleHidden();
	gm->m_pGPSCtrl->XGR_SetRebuildGeom(TRUE);
	//gm->ActiveModelViewFrame();
	gm->ResetLegendRect();
	gm->m_pGPSCtrl->SetShowVPointIcon(0);
	CViewBase::GetCurView_ST()->RestoreFrameTitle();
	gm->m_pGPSCtrl->UpdateAllWcsVcsMBR();
	gm->m_InvalidateFlag = TRUE;
	//gm->GPSRender();
	MakeImage_Current(str, FALSE);
	//
	MakePicture(Elems, str,strBook);
}
void AutoReport_Gen_CH_STL::WriteModelRlt(GenSegmentElements &Elems,I_GPSModel* gm,const Report_Key_LIST& aKeys,Report_Key_LIST& aGroupKeys,int& nIndex)
{
	CString strRltName,strGroup,strDir,strLoad;
	int nMode = 0;
	T_GRUP_D GrupD;
	CString str,strBook;
	if(aGroupKeys.IsEmpty())
		aGroupKeys.InsertAt(0,Report_Key(0,0));
	gm->m_bDeformed							= TRUE;
	for(int j = 0;j < aGroupKeys.GetSize();j++)
	{
		for(int i = 0;i < aKeys.GetSize();i++)
		{
			//gm->m_Preference.Format.nMoment = //暂时不需要
			gm->m_GPSMode = gm->m_GPSInstruction = GPS_COMMAND_EGMDSHP;
			gm->m_AppliedLoadCaseKey = aKeys[i].m_nKey+1;
			gm->m_LoadCaseType		= aKeys[i].m_nType;
			gm->m_LoadCaseKey		= aKeys[i].m_nKey+1;
			gm->m_LoadNameStr.Format(_LS(IDS_DB_LOAD_CASE_MODE) + _T(" %d"),aKeys[i].m_nKey+1);
			CForceEngine ForceD(gm);
			CDeformEngine DeformD(gm);
			
			CForceEngine* pForceTemp = gm->m_pForceEngine;
			CDeformEngine* pDeformTemp = gm->m_pDeformEngine;
			if(!pForceTemp)
				pForceTemp = &CGPSBarContainer::m_ForceEngine;
			if(!pDeformTemp)
				pDeformTemp = &CGPSBarContainer::m_DeformEngine;
			if(!pForceTemp->m_pContourEngine)
				pForceTemp->m_pContourEngine = &CGPSBarContainer::m_ContourEngine;
			if(!pDeformTemp->m_pContourEngine)
				pDeformTemp->m_pContourEngine = &CGPSBarContainer::m_ContourEngine;
			ForceD.ImportEngine(pForceTemp);
			DeformD.ImportEngine(pDeformTemp);
			ForceD.InitEngine();
			DeformD.m_bContourUpdate		= TRUE;
			DeformD.InitEngine();
			DeformD.m_DispComponent = DISP_COMPONENT_DXYZ;//nDisp[aKeys[i].m_nKey];
			DeformD.m_bResultDataSet		= TRUE;
			//pView->ReleaseDC(pDC); 
			//gm->m_AnimEngine.TerminateAnimation();
			//gm->m_pGPSCtrl->UnselectAllObject();
			//gm->m_bFigureDfltMode = FALSE;
			//gm->m_bFigureMode = FALSE;
			gm->m_bInitialView = FALSE;
			gm->ResetEngines();
			gm->m_pForceEngine = new CForceEngine(gm);
			gm->m_pForceEngine->ImportEngine(&ForceD);
			gm->m_pDeformEngine = new CDeformEngine(gm);
			gm->m_pDeformEngine->ImportEngine(&DeformD);

			if(aGroupKeys[j].m_nKey == 0)
			{
				strGroup = _T("全部");
				ActiveElemAll();
			}
			else if(m_pDoc->m_pAttrCtrl->GetGrup(aGroupKeys[j].m_nKey,GrupD))
			{
				strGroup= GrupD.GroupName;
				ActiveObj(GrupD.arKeyElem);
			}
			str.Format(_T("模态%d_%s_云图"),aKeys[i].m_nKey+1,strGroup);
			//if (pView->IsHiddenMode()) CViewBase::GetCurView_ST()->ToggleHidden();
			gm->m_pGPSCtrl->XGR_SetRebuildGeom(TRUE);
			//gm->ActiveModelViewFrame();
			gm->ResetLegendRect();
			gm->m_pGPSCtrl->SetShowVPointIcon(0);
			CViewBase::GetCurView_ST()->RestoreFrameTitle();
			gm->m_pGPSCtrl->UpdateAllWcsVcsMBR();
			gm->m_InvalidateFlag = TRUE;
			//gm->GPSRender();
			strBook.Format(_T("CHARPT_3_4_2_FIG_%d"),++nIndex);
			MakeImage_Current(str, FALSE);
			//
			MakePicture(Elems, str,strBook);
		}
	}
	gm->m_bDeformed							= FALSE;
}
void AutoReport_Gen_CH_STL::WriteDesignRlt(GenSegmentElements &Elems,I_GPSModel* gm,const Report_Key_LIST& aKeys,const CString& strBook)
{
	int nDisp[] = {DISP_COMPONENT_DX,DISP_COMPONENT_DY,DISP_COMPONENT_DZ,DISP_COMPONENT_DXY,DISP_COMPONENT_DYZ,DISP_COMPONENT_DXZ,DISP_COMPONENT_DXYZ};
	CString strRltTypeName[] = {_T("强度"),_T("剪切-y"),_T("剪切-z"),_T("稳定-y"),_T("稳定-z"),_T("组合"),_T("长细比"),_T("板件宽厚比"),_T("截面等级")};
	CString strRltName,strGroup,strDir,strLoad;
	int nMode = 0;
	T_GRUP_D GrupD;
	CString str;
	//for(int i = 0;i < aKeys.GetCount()/4;i++)
	//{
	//gm->m_Preference.Format.nMoment = //暂时不需要
	gm->m_GPSMode = gm->m_GPSInstruction = GPS_COMMAND_DSGNST;
	gm->m_LoadCaseType		= aKeys[3].m_nType;
	gm->m_LoadCaseKey		= aKeys[3].m_nKey;
	if(aKeys[2].m_nKey > 6)
	{
		gm->m_DOPT.LG.m_bDrawLegend		= FALSE;//云图
		gm->m_DOPT.TX.m_bOutputNumber	= TRUE;
	}
	CDesignEngine DesignD(gm);
	DesignD.ResetEngine();
	if (gm->m_pDsgnEngine)
		DesignD.ImportEngine(gm->m_pDsgnEngine);
	if(!DesignD.m_pContourEngine)
		DesignD.m_pContourEngine = &CGPSBarContainer::m_ContourEngine;
	DesignD.m_DsgnComponent	= aKeys[2].m_nKey;
	DesignD.m_dCRLmtVal			= 1.; 
	DesignD.m_bStShowBeam		= TRUE;
	DesignD.m_bStShowColm		= TRUE;
	DesignD.m_bStShowBrce		= TRUE;
	DesignD.m_bStShowTruss		= TRUE;
	DesignD.m_dColmSectScFc		= 1.;

	DesignD.m_bStShowService = FALSE;;
	DesignD.m_bStShowDeflection = FALSE;
	DesignD.m_bStShowBeam2   = TRUE;
	DesignD.m_bStShowColm2   = TRUE;
	gm->m_bInitialView = FALSE;
	gm->ResetEngines();
	gm->m_pDsgnEngine = new CDesignEngine(gm);
	gm->m_pDsgnEngine->ImportEngine(&DesignD);
	strRltName = strRltTypeName[aKeys[0].m_nKey];
	if (aKeys[3].m_nKey == 0)
	{
		strLoad = _T("组合");
		gm->m_LoadNameStr = _LS(IDS_DB_ALL_COMBINATION);
	}
	else
		strLoad = CDBLib::GetLoadCaseNameByKey(aKeys[3].m_nType, aKeys[3].m_nKey, aKeys[3].m_nSubType);
	if(m_pDoc->m_pAttrCtrl->GetGrup(aKeys[1].m_nKey,GrupD))
	{
		strGroup= GrupD.GroupName;
		ActiveObj(GrupD.arKeyElem);
	}
	else
	{
		VERIFY(aKeys[1].m_nKey == 0);
		strGroup = _T("全部");
		ActiveElemAll();
	}
	str.Format(_T("%s_%s_%s_验算比结果"),strGroup,strRltTypeName[aKeys[2].m_nKey],strLoad);
	//if (pView->IsHiddenMode()) CViewBase::GetCurView_ST()->ToggleHidden();
	gm->m_pGPSCtrl->XGR_SetRebuildGeom(TRUE);
	//gm->ActiveModelViewFrame();
	gm->ResetLegendRect();
	gm->m_pGPSCtrl->SetShowVPointIcon(0);
	CViewBase::GetCurView_ST()->RestoreFrameTitle();
	gm->m_pGPSCtrl->UpdateAllWcsVcsMBR();
	gm->m_InvalidateFlag = TRUE;
	//gm->GPSRender();
	MakeImage_Current(str, FALSE);
	//
	MakePicture(Elems, str,strBook);

	if(aKeys[2].m_nKey > 6)
	{
		gm->m_DOPT.LG.m_bDrawLegend		= TRUE;//云图
		gm->m_DOPT.TX.m_bOutputNumber	= FALSE;
	}
	//}
}
void AutoReport_Gen_CH_STL::WriteTable3_4_2(GenSegmentElements &Elems)
{
	T_TABLE_D TableD;
	TableD.Initialize();

	TableD.HeaderD.nHeaderRowCount = 2;
	TableD.HeaderD.nHeaderColCount = 4;
	TableD.HeaderD.aTitleName.Add(_T("模态号"));
	TableD.HeaderD.aTitleName.Add(_T("频率"));
	TableD.HeaderD.aTitleName.Add(_T("周期"));
	TableD.HeaderD.aTitleName.Add(_T("rad/sec"));
	TableD.HeaderD.aTitleName.Add(_T("cycle/sec"));
	T_MERGE_UNIT_D merge;merge.Initialize();
	merge.nMergeCount = 2; merge.nStartColNumber=2; merge.nStartRowNumber=1;
	TableD.HeaderD.aMergeColD.Add(merge);
	merge.nMergeCount = 2; merge.nStartColNumber=1; merge.nStartRowNumber=1;
	TableD.HeaderD.aMergeRowD.Add(merge);
	merge.nMergeCount = 2; merge.nStartColNumber=4; merge.nStartRowNumber=1;
	TableD.HeaderD.aMergeRowD.Add(merge);

	T_FREQ_D data;
	int nModeNum = m_pDoc->m_pPostCtrl->GetNumEigvPost();
	TableD.RecordD.aRecordData.SetSize(nModeNum);
	CString str,strTemp;
	int i = 0;
	for (; i < nModeNum; i++)
	{
		TableD.RecordD.aRecordData[i].SetSize(4);
		m_pDoc->m_pPostCtrl->GetEiFreq(i+1, data);
		TableD.RecordD.aRecordData[i][0].Format(_T("%d"),i+1);
		TableD.RecordD.aRecordData[i][1].Format(_T("%.3g"),data.dblFreq[0]);
		TableD.RecordD.aRecordData[i][2].Format(_T("%.3g"),data.dblFreq[1]);
		TableD.RecordD.aRecordData[i][3].Format(_T("%.3g"),data.dblFreq[2]);
		strTemp.Format(_T("T%d=%.3gs,"),i+1,data.dblFreq[2]);
		str += strTemp;
	}
	TableD.RecordD.aRecordData.SetSize(nModeNum);

	TableD.RecordD.nRecordRowCount = nModeNum;
	TableD.RecordD.nRecordColCount = 4;
	CString strText;


	strText.Format(_T("    本结构第1~第%d阶自振周期为：%s,各阶振型如下："),i,str);
	MakeText(Elems,strText);
	MakeTable(Elems, TableD ,_T("模态特征周期"), _T("CHARPT_3_4_2_TBL"));  
}
void AutoReport_Gen_CH_STL::WriteDesignGraph(GenSegmentElements &Elems,const Report_Key_LIST & aKey)
{
	if(aKey.IsEmpty())
		return;
	T_LCOM_MAXMIN_K lstLcoms;
	T_ELEM_K_LIST lstElems;
	CString strPicName,strBookName;
	CString strRltTypeName[6] = {_T("强度"),_T("剪切-y"),_T("剪切-z"),_T("稳定-y"),_T("稳定-z"),_T("组合")};
	CString strLoad;
	CString strGroupName;
	T_GRUP_D GrupD;
	for(int i = 0;i < aKey.GetSize()/3;i++)
	{
		if(aKey[i*3+1].m_nKey > 5)
			continue;
		lstElems.RemoveAll();

		if(m_pDoc->m_pAttrCtrl->GetGrup(aKey[i*3].m_nKey,GrupD))
		{
			lstElems.Copy(GrupD.arKeyElem);
			strGroupName = GrupD.GroupName;
		}
		else if(aKey[i*3].m_nKey == 0)
		{
			m_pDoc->m_pAttrCtrl->GetElemKeyList(lstElems);   
			strGroupName = _T("全部");
		}
		else
		{
			ASSERT(FALSE);
			continue;
		}

		lstLcoms.keymap = aKey[i*3+2].m_nKey;
		if(aKey[i*3+2].m_nKey == 0)
			strLoad = _T("全部");
		else
			strLoad = CDBLib::GetLoadCaseNameByKey(aKey[i*3+2].m_nType, aKey[i*3+2].m_nKey, 0);
		strPicName.Format(_T("钢结构验算比柱状图%s_%s_%s"),strGroupName,strRltTypeName[aKey[i*3+1].m_nKey],strLoad);//结构组_内力分量_荷载工况
		strBookName.Format(_T("DesignGraph_%d"),i+1);
		MakeDesignGraph(lstElems,lstLcoms,strPicName,aKey[i*3+1].m_nKey);
		MakePicture(Elems, strPicName,strBookName);
	}

	CGraphFuncView* pOldView = FindGraphView(RUNTIME_CLASS(CGraphFuncView), m_pDoc, _LS(IDS_ACS_VIEW_RES_RAT_VIEW));
	if(pOldView)
	{
		CMDIChildWnd* pFrame = (CMDIChildWnd*)pOldView->GetParentFrame();
		if(pFrame && pFrame->GetSafeHwnd() && ::IsWindow(pFrame->GetSafeHwnd()))
			pFrame->SendMessage(WM_CLOSE, 0, 0);
	}
}
void AutoReport_Gen_CH_STL::MakeDesignGraph(const T_KEY_LIST& lstElems,T_LCOM_MAXMIN_K LcomK,const CString& strPicName,int nLoadIndex /* = 5 */)
{
	unsigned int GraphType = CX_GRAPH_VBAR;//条形图
	CArray<double, double> RatioList;
	CMap<UINT, const UINT&, _GB17_Data_Map, _GB17_Data_Map&> map_SortData;
	map_SortData.RemoveAll();
	_GB17_Data_Ratio	SortData;
	_GB17_Data_Map	 SortLcbData;
	T_RSTL_GB50017_D DResult;
	T_RESULT_ITEM    item;
	T_KEY_LIST		RltElems;
	for(int i=0; i < lstElems.GetSize(); i++)
	{           
		T_ELEM_K temp_SelNo = lstElems[i];
		SortLcbData.Initialize();
		if(!m_pDoc->m_pPostCtrl->StlDgnMgr()->ReadDesignResult(temp_SelNo, DResult, LcomK.key.lcomkey,LcomK.key.maxmin))
			continue;
		SortData.Initialize();
		DResult.GetMaxRltByItem(T_RSTL_MEMB_SECT::RLT_AXIS, item);
		SortData.dResRatio[0] = item.GetRatio();
		DResult.GetMaxRltByItem(T_RSTL_MEMB_SECT::RLT_SHEAR_Y, item);
		SortData.dResRatio[1] = item.GetRatio();
		DResult.GetMaxRltByItem(T_RSTL_MEMB_SECT::RLT_SHEAR_Z, item);
		SortData.dResRatio[2] = item.GetRatio();
		DResult.GetMaxRltByItem(T_RSTL_MEMB_SECT::RLT_BEND_Y, item);
		SortData.dResRatio[3] = item.GetRatio();
		DResult.GetMaxRltByItem(T_RSTL_MEMB_SECT::RLT_BEND_Z, item);
		SortData.dResRatio[4] = item.GetRatio();
		DResult.GetMaxRltByItem(T_RSTL_MEMB_SECT::RLT_COMP  , item);
		SortData.dResRatio[5] = item.GetRatio();
		SortLcbData.SortLcbData.SetAt(LcomK.keymap, SortData);
		map_SortData.SetAt(temp_SelNo, SortLcbData);
	}	
	
	for (int i=0; i < lstElems.GetSize(); i++)
	{
		T_ELEM_K nElemK = lstElems.GetAt(i);
		auto itr = map_SortData.PLookup(nElemK);
		if(itr == nullptr)
		{
			continue;
		}
		//
		auto itrData = itr->value.SortLcbData.PLookup(LcomK.keymap);
		if(itrData == nullptr)
		{
			ASSERT(FALSE);
			continue;
		}
		//
		double dRatio= itrData->value.dResRatio[nLoadIndex];
		//if(dRatio >= 0. && dRatio <= 1.)
		//{
			RatioList.Add(dRatio);
			RltElems.Add(nElemK);
		//}  
	}

	if(RltElems.GetCount() <1 || RltElems.GetCount() != RatioList.GetCount())
	{
		ASSERT(FALSE);
		return;
	}


	T_GRAPH_D* pGraphData	= new T_GRAPH_D;
	// Set X-Axis Index.
	T_GRAPH_ITEM_D* IndexColX = new T_GRAPH_ITEM_D;
	IndexColX->csTitle = _LS(IDS_ACS_TABLE_ELEM_NO);	// Element No.
	IndexColX->csUnit  = _T("");
	IndexColX->nType	 = 0; // Integer Type.
	CString strValue	 = _T("");
	for(int i=0; i<RltElems.GetSize(); i++)
	{
		strValue.Format(_T("%d"), RltElems.GetAt(i));
		IndexColX->aValue.Add(strValue);
	}
	pGraphData->aCol.Add(IndexColX);
	// Set Y-Axis Value.
	T_GRAPH_ITEM_D* IndexColY = new T_GRAPH_ITEM_D;
	IndexColY->csTitle = _LS(IDS_ACS_VIEW_RESULT_RAT);	// Result Ratio.
	IndexColY->csUnit	 = _T("");
	IndexColY->nType 	 = 1; // Double Type.
	strValue = _T("");
	for(int i=0; i < RatioList.GetSize(); i++)
	{
		strValue.Format(_T("%8.4f"), RatioList.GetAt(i));
		IndexColY->aValue.Add(strValue);
	}
	pGraphData->aCol.Add(IndexColY);

	CGraphFuncView* pOldView = FindGraphView(RUNTIME_CLASS(CGraphFuncView), m_pDoc, _LS(IDS_ACS_VIEW_RES_RAT_VIEW));
	if(pOldView)
	{
		CMDIChildWnd* pFrame = (CMDIChildWnd*)pOldView->GetParentFrame();
		if(pFrame && pFrame->GetSafeHwnd() && ::IsWindow(pFrame->GetSafeHwnd()))
			pFrame->SendMessage(WM_CLOSE, 0, 0);
	}
	CGraphFuncView* pGraphView = m_pDoc->CreateGraphView(_LS(IDS_ACS_VIEW_RES_RAT_VIEW));
	// X-Axis Title.
	CString strXAxis=_T("");
	// Y-Axis Title.
	CString strYAxis=GetTitle(nLoadIndex);

	CString strTitle = _LS(IDS_ACS_VIEW_CHK_RES_RAT);				// Steel Code Checking Result
	strTitle += GetTitle(nLoadIndex);
	// Draw Graph.		
	pGraphView->SetGraphDataDirect(pGraphData,GraphType,0,strTitle,strXAxis,strYAxis); 
	
	CString strPath = GetImagePath(strPicName);

	//设置灰色_
	{
		//pGraphView->SetGrpahColorType(FALSE);	
		//pGraphView->GetGrphFuncTypePtr()->SetGrphType();
		//pGraphView->GetGrphFuncTypePtr()->SetDataGray();
	}

	pGraphView->SaveBMPFile(strPath);
}
CGraphFuncView* AutoReport_Gen_CH_STL::FindGraphView(CRuntimeClass* pViewClass, CDocument* pDoc, CString szWindowTitle)
{
	if(pDoc!=NULL && pViewClass->IsDerivedFrom(RUNTIME_CLASS(CTitleCtrlView)))
	{
		CView* pView;
		POSITION pos = pDoc->GetFirstViewPosition();
		while(pos!=NULL)
		{
			pView = pDoc->GetNextView(pos);
			if(pView->IsKindOf(pViewClass))
			{
				CTitleCtrlView* pTitleView = (CTitleCtrlView*)pView;
				if(pTitleView->GetWindowTitleName()==szWindowTitle)
				{
					return (CGraphFuncView*)pTitleView;
				}
			}
		}
	}
	return NULL;
}
CString AutoReport_Gen_CH_STL::GetTitle(int idx)
{
	CString strTitle = _T("");
	if(idx==0)	    strTitle = _LS(IDS_ACS_VIEW_RAT_RSX__17);	// Ratio.(Axial).
	else if(idx==3)	strTitle = _LS(IDS_ACS_VIEW_RAT_RBY__17);	// Ratio.(Bend-y). 
	else if(idx==4)	strTitle = _LS(IDS_ACS_VIEW_RAT_RBZ__17);	// Ratio.(Bend-z). 
	else if(idx==1)	strTitle = _LS(IDS_ACS_VIEW_RAT_RSY__17);	// Ratio.(Shear-y).
	else if(idx==2)	strTitle = _LS(IDS_ACS_VIEW_RAT_RSZ__17);	// Ratio.(Shear-z).
	else if(idx==5)	strTitle = _LS(IDS_ACS_VIEW_RAT_RCOM_17);	// Ratio.(Combined).
	return strTitle;
}

void AutoReport_Gen_CH_STL::SetConcInfo(const T_MATD_D& data,CStringArray& strArray)
{
	double dFc;
	dFc = data.Data1.Design.C_fc;
	const int nGB_RC = 14;
	//CString GB_RC_Name[nGB_RC] =
	//{"C15", "C20", "C25", "C30", "C35", "C40", "C45", "C50", "C55", "C60",
	//"C65", "C70", "C75", "C80"};
	double GB_RC_fc[nGB_RC] =
	{15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 55.0, 60.0,
	65.0, 70.0, 75.0, 80.0};
	double DB_RC_S_fck[nGB_RC] =
	{10.0, 13.4, 16.7, 20.1, 23.4, 26.8, 29.6, 32.4, 35.5, 38.5,
	41.5, 44.5, 47.4, 50.2};
	double DB_RC_S_ftk[nGB_RC] =
	{1.27, 1.54, 1.78, 2.01, 2.20, 2.39, 2.51, 2.64, 2.74, 2.85,
	2.93, 2.99, 3.05, 3.11};
	double DB_RC_D_fc[nGB_RC] =
	{ 7.2,  9.6, 11.9, 14.3, 16.7, 19.1, 21.1, 23.1, 25.3, 27.5,
	29.7, 31.8, 33.8, 35.9};
	double DB_RC_D_ft[nGB_RC] =
	{0.91, 1.10, 1.27, 1.43, 1.57, 1.71, 1.80, 1.89, 1.96, 2.04,
	2.09, 2.14, 2.18, 2.22};

	int nIndex = 0;
	double dTemp = 20.0 - cGen_Zero;
	while(dTemp < dFc)
	{
		nIndex++;
		dTemp += 5.0;
	}
	if(nIndex < 0 || nIndex > 13){ASSERT(FALSE);nIndex = 0;};

	CString str;
	str.Format(_T("%.2g"),GB_RC_fc[nIndex]);
	strArray.Add(str);
	str.Format(_T("%.2g"),DB_RC_D_ft[nIndex]);
	strArray.Add(str);
	str.Format(_T("%.2g"),DB_RC_S_fck[nIndex]);
	strArray.Add(str);
	str.Format(_T("%.2g"),DB_RC_S_ftk[nIndex]);
	strArray.Add(str);
}
int AutoReport_Gen_CH_STL::SetStlInfo(int key,const T_MATD_D& data,CArray<CStringArray, CStringArray &>& strArray,int& nIndex)
{
	CString strKey;
	strKey.Format(_T("%d"),key);
	strKey = m_strFormat + strKey;
	auto& vstr = m_mapstlInfo[std::pair<CString,CString>(data.Data1.CodeName,data.Data1.CodeMatlName)];
	if(vstr.empty())
	{
		strArray[++nIndex].Add(strKey);
		strArray[nIndex].Add(m_strFormat+data.Data1.CodeMatlName);
		strArray[nIndex].Add(m_strFormat+_T("-"));
		double dfy =0.0,dfv=0.0;
		T_MATD_D designdata;
		CString strfy,strfv;
		if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(key, designdata))
		{
			strfy = m_strFormat+_T("-");
			strfy = m_strFormat+_T("-");
		}
		else
		{
			strfy.Format(_T("%d"),(int)designdata.Data1.Design.S_Fy1);
			strfv.Format(_T("%d"),(int)(designdata.Data1.Design.S_Fy1/1.732));
			strfy = m_strFormat + strfy;
			strfv = m_strFormat + strfv;
		}
		strArray[nIndex].Add(strfy);
		strArray[nIndex].Add(strfv);
		strArray[nIndex].Add(m_strFormat+_T("-"));
		return nIndex;
	}
	int nCol = 0;
	for(auto it = vstr.begin();it != vstr.end();it++,nCol++)
	{
		if(it->empty())
			break;
		if(nCol %4 == 0)
		{
			strArray[++nIndex].Add(strKey);
			strArray[nIndex].Add(m_strFormat+data.Data1.CodeMatlName);
		}
		strArray[nIndex].Add(m_strFormat+it->c_str());
	}
	return nIndex;
}
void AutoReport_Gen_CH_STL::setTableFormatString(int nlocation /* = 0 */,int ncolor /* = 16777215 */)
{
	CString str[] = {_T("center"),_T("left"),_T("right")};
	m_strFormat.Format(_T("[DRG_CELL_PROPERTY]color=0 align=%s shape=%d tbwidth=20%%[/DRG_CELL_PROPERTY]"),str[nlocation],ncolor);
}
void AutoReport_Gen_CH_STL::MakeStlMatlInfo()
{
	m_mapstlInfo.clear();
	wstring str[][20] = {
					//GB03(S)
						//Q235
						{
							_T("≤16"),		_T("235/215"),_T("125"),_T("325"),
							_T(">16～40"),	_T("225/205"),_T("120"),_T("325"),
							_T(">40～60"),	_T("215/200"),_T("115"),_T("325"),
							_T(">60～100"),	_T("205/190"),_T("110"),_T("325"),
						},
						//Q345
						{
							_T("≤16"),		_T("345/310"),_T("180"),_T("400"),
							_T(">16～35"),	_T("325/295"),_T("170"),_T("400"),
							_T(">35～50"),	_T("295/265"),_T("155"),_T("400"),
							_T(">50～100"),	_T("275/250"),_T("145"),_T("400"),
						},
						//Q390
						{
							_T("≤16"),		_T("390/350"),_T("205"),_T("400"),
							_T(">16～35"),	_T("370/335"),_T("190"),_T("400"),
							_T(">35～50"),	_T("350/315"),_T("180"),_T("400"),
							_T(">50～100"),	_T("330/295"),_T("170"),_T("400"),
						},
						//Q420
						{
							_T("≤16"),		_T("420/380"),_T("220"),_T("400"),
							_T(">16～35"),	_T("400/360"),_T("210"),_T("400"),
							_T(">35～50"),	_T("380/340"),_T("195"),_T("400"),
							_T(">50～100"),	_T("360/325"),_T("185"),_T("400"),
						},
					//GB17(S)
						//Q235
						{
							_T("≤16"),		_T("235/215"),_T("125"),_T("320"),
							_T(">16～40"),	_T("225/205"),_T("120"),_T("320"),
							_T(">40～100"),	_T("215/200"),_T("115"),_T("320"),
						},
						//Q345
						{
							_T("≤16"),		_T("345/305"),_T("175"),_T("400"),
							_T(">16～40"),	_T("335/295"),_T("170"),_T("400"),
							_T(">40～63"),	_T("325/290"),_T("165"),_T("400"),
							_T(">63～80"),	_T("315/280"),_T("160"),_T("400"),
							_T(">80～100"),	_T("305/270"),_T("155"),_T("400"),
						},
						//Q355
						{
							_T("≤16"),		_T("355/305"),_T("175"),_T("400"),
							_T(">16～40"),		_T("345/295"),_T("170"),_T("400"),
							_T(">40～63"),		_T("335/290"),_T("165"),_T("400"),
							_T(">63～80"),		_T("325/280"),_T("160"),_T("400"),
							_T(">80～100"),	_T("315/270"),_T("155"),_T("400"),
						},
						//Q345GJ
						{
							_T(">16~50"),		_T("345/325"),_T("190"),_T("415"),
							_T(">50～100"),	_T("335/300"),_T("175"),_T("415")
						},
						//Q390
						{
							_T("≤16"),		_T("390/345"),_T("200"),_T("415"),
							_T(">16～40"),		_T("380/330"),_T("190"),_T("415"),
							_T(">40～63"),		_T("360/310"),_T("180"),_T("415"),
							_T(">63～100"),	_T("340/295"),_T("170"),_T("415"),
						},
						//Q420
						{
							_T("≤16"),		_T("420/375"),_T("215"),_T("440"),
							_T(">16～40"),		_T("410/355"),_T("205"),_T("440"),
							_T(">40～63"),		_T("390/320"),_T("185"),_T("440"),
							_T(">63～100"),	_T("370/305"),_T("175"),_T("440"),
						},
						//Q460
						{
							_T("≤16"),		_T("460/410"),_T("235"),_T("470"),
							_T(">16～40"),		_T("450/390"),_T("225"),_T("470"),
							_T(">40～50"),		_T("430/355"),_T("205"),_T("470"),
							_T(">63～100"),	_T("410/340"),_T("195"),_T("470")
						},
					//JGJ2015(S)
						//Q235
						{
							_T("≤16"),		_T("235/215"),_T("125"),_T("320"),
							_T(">16～40"),		_T("225/205"),_T("120"),_T("325"),
							_T(">40～100"),	_T("215/200"),_T("115"),_T("325")
						},
						//Q345
						{
							_T("≤16"),		_T("345/305"),_T("175"),_T("400"),
							_T(">16～40"),		_T("335/295"),_T("170"),_T("400"),
							_T(">40～63"),		_T("325/290"),_T("165"),_T("400"),
							_T(">63～80"),		_T("315/280"),_T("160"),_T("400"),
							_T(">80～100"),	_T("305/270"),_T("155"),_T("400")
						},
						//Q345GJ
						{
							_T(">16~50"),		_T("345/325"),_T("190"),_T("415"),
							_T(">50～100"),	_T("335/300"),_T("175"),_T("415")
						},
						//Q390
						{
							_T("≤16"),		_T("390/345"),_T("200"),_T("415"),
							_T(">16～40"),	_T("370/330"),_T("190"),_T("415"),
							_T(">40～63"),	_T("350/310"),_T("180"),_T("415"),
							_T(">63～100"),	_T("330/295"),_T("170"),_T("415"),
						},
						//Q420
						{
							_T("≤16"),		_T("420/375"),_T("215"),_T("415"),
							_T(">16～40"),	_T("400/355"),_T("205"),_T("415"),
							_T(">40～63"),	_T("380/320"),_T("185"),_T("415"),
							_T(">63～100"),	_T("360/305"),_T("175"),_T("415")
						},
	
	};
	auto setstr = [&](const CString& str1,const CString& str2,int nIndex)
	{
		std::vector<wstring> strarray;
		for(int i = 0;i < 20;i++)
		{
			strarray.push_back(str[nIndex][i]);
		}
		m_mapstlInfo[std::pair<CString,CString>(str1,str2)] = move(strarray);
	};
	setstr(MATLCODE_STL_GB03,_T("Q235"),0);
	setstr(MATLCODE_STL_GB03,_T("Q345"),1);
	setstr(MATLCODE_STL_GB03,_T("Q390"),2);
	setstr(MATLCODE_STL_GB03,_T("Q420"),3);

	setstr(MATLCODE_STL_GB50017_17,_T("Q235"),4);
	setstr(MATLCODE_STL_GB50017_17,_T("Q345"),5);
	setstr(MATLCODE_STL_GB50017_17,_T("Q355"),6);
	setstr(MATLCODE_STL_GB50017_17,_T("Q345GJ"),7);
	setstr(MATLCODE_STL_GB50017_17,_T("Q390"),8);
	setstr(MATLCODE_STL_GB50017_17,_T("Q420"),9);
	setstr(MATLCODE_STL_GB50017_17,_T("Q460"),10);

	setstr(MATLCODE_STL_JGJ2015,_T("Q235"),11);
	setstr(MATLCODE_STL_JGJ2015,_T("Q345"),12);
	setstr(MATLCODE_STL_JGJ2015,_T("Q345GJ"),13);
	setstr(MATLCODE_STL_JGJ2015,_T("Q390"),14);
	setstr(MATLCODE_STL_JGJ2015,_T("Q420"),15);
}

double AutoReport_Gen_CH_STL::GetBz()
{
	WindCodeData WindCodeDX, WindCodeDY;
	CLateralLoad laterLoad;
	double DimFAC;
	double dBz = 0.0;
	T_UNIT_INDEX UnitIndex; 
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	int nLengthUnit;
	nLengthUnit = UnitIndex.nBase_Length;
	if(nLengthUnit == 0) DimFAC=1000.0 ;           // mm TO m
	if(nLengthUnit == 1) DimFAC=100.0  ;           // cm TO m
	if(nLengthUnit == 2) DimFAC=1.0    ;           //  m TO m
	if(nLengthUnit == 3) DimFAC=100.0/2.540;       // in TO m
	if(nLengthUnit == 4) DimFAC=100.0/2.540/12.0 ; // ft TO m

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountWind();
	if(nItemCount == 0) return dBz;
	POSITION pos;
	CString str;
	if(nItemCount)
	{
		T_WIND_K Key;
		T_WIND_D rDataWind;
		double BXC,BYC;
		CArray<T_STOR_D, T_STOR_D&> rarStorList;
		m_pDoc->m_pAttrCtrl->GetStorData(rarStorList);
		double dAll0 =.0 , dAll1 =.0;
		WindLoadCalInfo WindCalInfo;
		int nCount = rarStorList.GetSize();
		double dBaseLevel;
		T_BLDC_D DataBLDC;
		if(!m_pDoc->m_pAttrCtrl->GetBldc(DataBLDC)) DataBLDC.Initialize();
		if(DataBLDC.bUseBaseLevel)
		{
			dBaseLevel = DataBLDC.dBaseLevel;
		}
		else // find the lowest z value;      // CIVIL牢 版快客 Gen狼 老何 版快
		{
			T_NODE_K key;
			T_NODE_D DataNode;
			POSITION pos;
			pos = m_pDoc->m_pAttrCtrl->GetStartNode();
			double dLowest = 1e10;
			while(pos!=NULL)
			{
				m_pDoc->m_pAttrCtrl->GetNextNode(pos,key,DataNode);
				if(dLowest > DataNode.z) dLowest = DataNode.z;
			}
			dBaseLevel = dLowest;
		}


		double Zh1 = laterLoad.GetBldgHeightAboveBaseLevel();
		for(int i =0;i < nCount;i++)
		{
			dAll0 +=rarStorList[i].WindWidthX;
			dAll1 +=rarStorList[i].WindWidthY;
		}
		pos = m_pDoc->m_pAttrCtrl->GetStartWind();
		while(pos !=NULL)
		{
			m_pDoc->m_pAttrCtrl->GetNextWind(pos, Key, rDataWind);
			if(rDataWind.nCodeType != 23 && rDataWind.nCodeType != 19 && rDataWind.nCodeType != 9)
				continue;
			for(int i =0;i < rarStorList.GetSize();i++)
			{
				double ZC = rarStorList[i].dStoryLevel - dBaseLevel;
				BXC = rarStorList[i].WindWidthX;  
				BYC = rarStorList[i].WindWidthY;
				WindCodeDX.m_dTolH       = WindCodeDY.m_dTolH       = Zh1 / DimFAC;
				WindCodeDX.m_nGroundKind = WindCodeDY.m_nGroundKind = rDataWind.CodeParam.CH2019.nRoughCategory;// + 1;
				WindCodeDX.m_dW0         = WindCodeDY.m_dW0         = rDataWind.CodeParam.CH2019.dBasicWindPressure;
				WindCodeDX.m_dDamta      = WindCodeDY.m_dDamta      = rDataWind.CodeParam.CH2019.dDampingRatio;
				WindCodeDX.m_dUseUzRat   = WindCodeDY.m_dUseUzRat   = rDataWind.CodeParam.CH2019.dModifyFactor;
				WindCodeDX.m_bAutoSFBz   = WindCodeDY.m_bAutoSFBz   = rDataWind.CodeParam.CH2019.bAlongVibration;
				WindCodeDX.m_dCalH       = WindCodeDY.m_dCalH       = ZC  / DimFAC;
				WindCodeDX.m_dTowerRat   = WindCodeDY.m_dTowerRat   = 0.0;
				WindCodeDX.m_dTt1        = WindCodeDY.m_dTt1        = rDataWind.CodeParam.CH2019.dTorsionPeriod;
				WindCodeDX.m_dVibPress   = WindCodeDY.m_dVibPress   = rDataWind.CodeParam.CH2019.dCheckWindPressure;
				WindCodeDX.m_dVibZNB     = WindCodeDY.m_dVibZNB     = rDataWind.CodeParam.CH2019.dCheckDampingRatio;
				WindCodeDX.m_nStructMethod = WindCodeDY.m_nStructMethod = rDataWind.CodeParam.CH2019.nStructMethod;
				WindCodeDX.m_nAlongType = WindCodeDY.m_nAlongType = rDataWind.CodeParam.CH2019.nAlongType;

				WindCodeDX.m_dT1       = rDataWind.CodeParam.CH2019.dPeriod_X;
				WindCodeDX.m_dFigRat   = rDataWind.CodeParam.CH2019.bPartitionCoef ? (rarStorList[i].dWindwardCoefX - rarStorList[i].dLeewardCoefX)
					: (rDataWind.CodeParam.CH2019.dWindwardCoefX - rDataWind.CodeParam.CH2019.dLeewardCoefX);
				WindCodeDX.m_dBotWidth = (dAll1/nCount) / DimFAC;
				WindCodeDX.m_dBotThik  = (dAll0/nCount) / DimFAC;
				WindCodeDX.m_dCalWidTh =  BYC / DimFAC;
				WindCodeDX.m_dCalThik  =  BXC / DimFAC;
				WindCodeDX.m_dOpenRat  = rDataWind.CodeParam.CH2019.bWindShield ? rarStorList[i].dShieldCoefX: 1.0;
				WindCodeDX.m_dT2       = rDataWind.CodeParam.CH2019.nSectType == 1 ? rDataWind.CodeParam.CH2019.dCrossPeriod_X : 0.5;
				WindCodeDX.m_dbB       = rDataWind.CodeParam.CH2019.nSectType == 0 ? rDataWind.CodeParam.CH2019.dCrossPeriod_X : 0.0;

				INewLoadCodeTool::CalWindLoad_S(WindCodeDX , WindCalInfo);
				dBz = max(dBz,WindCalInfo.GetData(1));
				INewLoadCodeTool::CalWindLoad_S(WindCodeDY , WindCalInfo);
				dBz = max(dBz,WindCalInfo.GetData(1));
			}
		}
	}
	return dBz;
}
