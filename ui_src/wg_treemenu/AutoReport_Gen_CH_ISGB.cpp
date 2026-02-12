#include "stdafx.h"
#include "AutoReport_Gen_CH_ISGB.h"
#include "ReportCommonFunc.h"
#include "TreeReportCtrl.h"
#include "LoadWordLib.h"
#include "..\wg_xl\ReportTableProcess.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_db\StagCtrl.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_db\StoryDisplDrift.h"
#include "..\wg_db\StoryData.h"
#include "..\wg_db\SelfWgt.h"
#include "..\wg_db\StoryLoad.h"
#include "..\wg_db\LateralLoad.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\DesignResult.h"
#include "..\wg_db\IsolatorTool.h"
#include "..\wg_db\ThrsAutoNamer.h"
#include "..\wg_db\DB_ST_DT_FIGR.h"
#include "..\wg_base\wg_baseAll.h"
#include "..\wg_base\wg_baseAllTemp.h"
#include "..\wg_base\GraphFuncview.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_cmd\wg_cmd.h"
#include "..\wg_cmd\ThfcItemDlg.h"
#include "..\wg_gps\GPSAMRUtil.h"
#include "..\wg_gps\GpsbarBmpMaker\GpsbarInitData.h"
#include "..\wg_gps\DRW_DATA.H"
#include "..\wg_gps\GPSAMRUDCData.h" 
#include "..\wg_gps\wg_gpsAll.h"
#include "..\wg_gps\GPSModalDampDlg.h"
#include "..\wg_gps\GPSThisSmartGraphDlg.h"
#include "..\wg_gps\ExportFunc.h"
#include "..\wg_gr\GRAMRUDFData.h"
#include "..\wg_gr\GRenderView.h"
#include "..\wg_gr\I_GEN_GPSCtrl.h"
#include "..\wg_base\TextFileOut.h"


//#include "..\wg_gr\I_GENModel.h"
//#include "..\wg_gr\GRDisplayDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const double cGen_Zero	  = 1.0E-07;

struct _ISGB_Data_Ratio
{
	double dResRatio[6];	// RSx,RSy,RSz,RBy,RBz,Rcomb.
	void Initialize()
	{
		for(int i=0; i<6; i++)	{dResRatio[i] = 0.0;}
	}
};

struct _ISGB_Data_Map
{
	CMap<UINT, const UINT&, _ISGB_Data_Ratio, _ISGB_Data_Ratio&> SortLcbData;
	void Initialize()
	{
		SortLcbData.RemoveAll();
	}
	_ISGB_Data_Map()	{}
	_ISGB_Data_Map(_ISGB_Data_Map& sData)	{*this = sData;}
	_ISGB_Data_Map& operator = (_ISGB_Data_Map& sData)
	{
		_ISGB_Data_Ratio SortData;
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

Report_Para_CH* AutoReport_Gen_CH_ISGB::Para() const
{
		return AutoReport_Gen_Mgr::Inst()->Para();
}

CString AutoReport_Gen_CH_ISGB::MakePicture_1_1()
{
	CString strName = _T("ALL-结构模型图");
	SetPreOrPostMode(FALSE);
	DrawNodeAll(FALSE);
	SetViewPoint(0);
	MakeImage_Current(strName, FALSE);
	return strName;
}

int AutoReport_Gen_CH_ISGB::TempletID()
{
		return 0;
}
BOOL AutoReport_Gen_CH_ISGB::PreMakeData()
{
	Initial();
	m_Segments.clear();
	CViewBase::GetCurView_ST()->DoInitialView();
	SetShape(FALSE);
	//
	GRenderView* pRenderView = (GRenderView*)(I_GENModelBase::GetCurMySelfST()->GetOwnerWnd()); ASSERT(pRenderView);
	CAutoRecoveryUnit unitSet(D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_FORCE_INDEX_N, m_pDoc->m_pUnitCtrl,TRUE); 

	if (!m_pDoc->m_pAttrCtrl->GetIsolation(m_IsgbD))
		m_IsgbD.Initialize();

	CViewBase* pView = CViewBase::GetCurView_ST();
	I_GENModelBase* gm = I_GENModelBase::GetCurMySelfST();
	gm->GetDispSetting()->InitForInitialView();
	gm->GetDispSetting()->m_Node.bNode = FALSE;
	gm->GetDispSetting()->m_View.bVPoint = FALSE;
	
	WriteCharpt1();
	WriteCharpt2();
	WriteCharpt3();
	WriteCharpt4();
	WriteCharpt5();
	WriteCharpt6();
	WriteCharpt7();
	WriteCharpt8();
	return TRUE;
}
void AutoReport_Gen_CH_ISGB::InsertCoverPage()
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
void AutoReport_Gen_CH_ISGB::InsertCatalogPage()
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
void AutoReport_Gen_CH_ISGB::MakeContent()
{   
	for (int i=0; i<m_Segments.size(); i++)
	{
		InsertSegmentElements(m_Segments[i]);
	}    
}
void AutoReport_Gen_CH_ISGB::WriteCharpt1()
{
	GenSegmentElements genElements;
	MakeTitle(genElements, _T("工程概况"),1,0);
	
	CString strStruct, str1, str2, str3;
	switch (m_IsgbD.nStrutCategory)
	{
	case 0:	strStruct = _T("钢筋混凝土框架结构");	break;
	case 1:	strStruct = _T("钢筋混凝土框架-抗震墙结构");	break;
	case 2:	strStruct = _T("框架-核心筒结构");	break;
	case 3:	strStruct = _T("板柱-抗震墙结构");	break;
	case 4:	strStruct = _T("钢筋混凝土抗震墙结构");	break;
	case 5:	strStruct = _T("钢结构");	break;
	default:
		strStruct = _T("");	
		break;
	}
	str1.Format(_T("    本项目的结构类型为%s,"), GetBold(strStruct));

	CArray<T_MDUL_K, T_MDUL_K> rKeyList;
	m_pDoc->m_pAttrCtrl->GetMdulKeyList(rKeyList);
	int nModulNum = rKeyList.GetCount();
	int nStorNum = 0;
	T_MDUL_D MdulD;
	if (nModulNum > 1)
	{
		for (int i = 0; i < nModulNum; i++)
		{
			MdulD.Initialize();
			m_pDoc->m_pAttrCtrl->GetMdul(rKeyList[i], MdulD);
			nStorNum = MdulD.aStorK.GetCount();
			str2.Format(_T("%s楼层数为%d层，"), MdulD.strName, nStorNum);
		}
	}
	else
	{
		nStorNum = m_pDoc->m_pAttrCtrl->GetCountStor();
		str2.Format(_T("楼层数为%d层，"), nStorNum);
	}
	
	T_STOR_D StorD;
	m_pDoc->m_pAttrCtrl->GetStor(m_IsgbD.nSeisLayer, StorD);
	double dHeight = m_pDoc->m_pAttrCtrl->GetBldgHeight()/1000;
	str3.Format(_T("隔震层为%s，建筑结构高度为%.1f m。模型视图如图1-1"), GetBold(StorD.StoryName), dHeight);
	MakeText(genElements, str1+str2+str3);
	//
	Report_Key_LIST aGroupKey;
	Para()->GroupKeys(aGroupKey, ARGC_ISGB * 1000 + 0);
	CString strName, strGroup, strBookMark;
	T_GRUP_D GrupD;
	for (int i = 0; i < aGroupKey.GetSize(); i++)
	{
		if (m_pDoc->m_pAttrCtrl->GetGrup(aGroupKey[i].m_nKey, GrupD))
		{
			strGroup = GrupD.GroupName;
			ActiveObj(GrupD.arKeyElem);
		}
		else
		{
			VERIFY(aGroupKey[i].m_nKey == 0);
			strGroup = _T("全部");
			ActiveElemAll();
		}
		
		SetViewPoint(0);
		strName = strGroup + _T("结构模型图");
		MakeImage_Current(strName, FALSE);
		strBookMark.Format(_T("CHARPT_3_2_FIG%d"), i*4 + 1);
		MakePicture(genElements, strName, strBookMark);

		SetViewPoint(3);
		strName = strGroup + _T("顶视图");
		MakeImage_Current(strName, FALSE);
		strBookMark.Format(_T("CHARPT_3_2_FIG%d"), i * 4 +2);
		MakePicture(genElements, strName, strBookMark);

		SetViewPoint(1);
		strName = strGroup + _T("左立面图");
		MakeImage_Current(strName, FALSE);
		strBookMark.Format(_T("CHARPT_3_2_FIG%d"), i * 4 + 3);
		MakePicture(genElements, strName, strBookMark);

		SetViewPoint(2);
		strName = strGroup + _T("前立面图");
		MakeImage_Current(strName, FALSE);
		strBookMark.Format(_T("CHARPT_3_2_FIG%d"), i * 4 + 4);
		MakePicture(genElements, strName, _T("CHARPT_3_2_FIG3"));
	}
	m_Segments.push_back(genElements);
}
void AutoReport_Gen_CH_ISGB::WriteCharpt2()
{
	GenSegmentElements genElements;
	MakeTitle(genElements, _T("设计依据"), 2, 0);
	WriteCharpt2_1(genElements);
	WriteCharpt2_2(genElements);
	m_Segments.push_back(genElements);
}
void AutoReport_Gen_CH_ISGB::WriteCharpt2_1(GenSegmentElements &genElements)
{    
	MakeTitle(genElements, _T("设计规范及图集"), 2, 1);
	CString str;

	if (m_IsgbD.nCode == 1)
		MakeAlignText(str, _T("《建筑工程减隔震技术规程》北京市地方标准"), _T("DB11/2075-2022"));
	MakeAlignText(str,_T("《建筑抗震设防分类标准》"),_T("GB 50223-2008"));
	MakeAlignText(str,_T("《建筑结构荷载规范》"),_T("GB 50009-2012"));
	MakeAlignText(str,_T("《建筑抗震设计规范》"),_T("GB 50011-2010(2016修订版)"));
	MakeAlignText(str,_T("《建筑隔震设计标准》"),_T("GB/T 51408-2021"));
	MakeAlignText(str,_T("《叠层橡胶支座隔震技术规程》"),_T("CECS 126：2001"));
	MakeAlignText(str,_T("《橡胶支座 第3部分：建筑隔震橡胶支座》"),_T("GB 20688.3-2006"));
	MakeAlignText(str,_T("《混凝土结构设计规范》"),_T("GB 50010-2010(2015修订版)"));
	MakeAlignText(str,_T("《高层建筑混凝土结构技术规程》"),_T("JGJ 3-2010"));
	MakeAlignText(str,_T("《建筑地基基础设计规范》"),_T("GB 50007-2011"));
	MakeAlignText(str,_T("《建筑结构隔震构造详图》"),_T("03SG610－1"));
	MakeAlignText(str,_T("《工程结构通用规范》"),_T("GB 55001-2021"));
	MakeAlignText(str,_T("《建筑与市政工程抗震通用规范》"), _T("GB 55002-2021"));
	MakeText(genElements, str);
	return;
}
void AutoReport_Gen_CH_ISGB::WriteCharpt2_2(GenSegmentElements &genElements)
{
	MakeTitle(genElements, _T("设计标准"), 2, 2);
	//2.2.1
	MakeTitle(genElements, _T("结构设计使用年限"), 2, 2, _T(""), 1);
	CString str1;
	str1.Format(_T("    本工程设计基准100年,结构设计使用年限为100年。"));
	MakeText(genElements,str1);
	//2.2.2
	MakeTitle(genElements, _T("建筑安全等级和耐火等级"), 2, 2, _T(""), 2);
	str1.Format(_T("    建筑结构安全等级为一级，结构重要性系数γo=1.10。建筑耐火等级为一级。"));	
	MakeText(genElements,str1);
	//2.2.3
	{
		int nSFI[] = { 6,7,7,8,8,9 };
		double dSFI[] = { 0.05,0.10,0.15,0.20,0.30,0.40 };
		CString aStrSDC[] = { _T("一"),_T("二"),_T("三") };
		CString astrSC[] = { _T("I0"), _T("I1"),  _T("II"), _T("III"), _T("IV") };
		CString astrFortify[] = { _T("特殊设防甲类"), _T("重点设防乙类"),  _T("标准设防丙类") };
		int nSfi = nSFI[0];
		double dSfi = dSFI[0];
		CString strSC = astrSC[0];
		CString strSDC = aStrSDC[0];
		int nSeisFi = 0;
		int nSiteClass = 0;
		int nSeisDgnCategory = 0;
		BOOL bSplc = Para()->IsSpfc(m_pDoc, nSeisFi, nSiteClass, nSeisDgnCategory);
		if (bSplc)
		{
			nSfi = nSFI[nSeisFi];
			dSfi = dSFI[nSeisFi];
			strSC = astrSC[nSiteClass];
			strSDC = aStrSDC[nSeisDgnCategory];
		}

		MakeTitle(genElements, _T("结构抗震设计"), 2, 2, _T("结构抗震设计"), 3);
		str1.Format(_T("    本工程抗震设防烈度为%s度，设计地震分组为第%s组，设计基本地震加速度值为%sg，场地类别为%s类。"), GetBold(nSfi), GetBold(strSDC), GetBold(dSfi), GetBold(strSC));
		MakeText(genElements, str1);
		str1.Format(_T("    根据《建筑工程抗震设防分类标准》 GB 50223-2008，本属于%s，抗震构造措施按 %s度。"), GetBold(astrFortify[m_IsgbD.nFortificationCategory]), GetBold(nSfi));
		MakeText(genElements, str1);
	}	
	//2.2.4
	{
		CString strArr_RC[] = {_T("A"),_T("B"),_T("C"),_T("D")};
		CString strRC = strArr_RC[0];
		double dBWP = 0.3;
		POSITION pos = m_pDoc->m_pAttrCtrl->GetStartWind();
		T_WIND_K key;
		T_WIND_D data;
		while (pos != NULL)
		{
			m_pDoc->m_pAttrCtrl->GetNextWind(pos, key, data);
			int nRoughCategory = 0;
			Para()->GetWindPara(m_pDoc, nRoughCategory, dBWP);
			CString strArr_RC[] = { _T("A"),_T("B"),_T("C"),_T("D") };
			strRC = strArr_RC[nRoughCategory - 1];
		}
		MakeTitle(genElements, _T("结构抗风设计"), 2, 2, _T("结构抗风设计"), 4);
		str1.Format(_T("    基本风压:%s年重现期Wo=%skN/m2 ，地面粗糙度为%s类；本工程结构承载力按%s年重现期设计。基本雪压: W雪=%skN/m2 。"),
			GetBold(100),GetBold(dBWP),GetBold(strRC),GetBold(100),GetBold(0.7));	
		MakeText(genElements,str1);
	}
}

void AutoReport_Gen_CH_ISGB::WriteCharpt3  ()
{
	GenSegmentElements Elems;
	MakeTitle(Elems, _T("隔震支座布置"), 3, 0);
	WriteCharpt3_1(Elems);
	WriteCharpt3_2(Elems);
	m_Segments.push_back(Elems);
}
void AutoReport_Gen_CH_ISGB::WriteCharpt3_1(GenSegmentElements &genElements)
{
	MakeTitle(genElements, _T("隔震支座平面图布置") ,3, 1);  
	CString str1;
	str1.Format(_T("    隔震支座平面图布置如下："));
	MakeText(genElements, str1);

	SetViewPoint(3);
	CString strName;
	strName = _T("隔震支座平面图布置");
	T_KEY_LIST aNode, aElem;
	m_pDoc->m_pAttrCtrl->GetNodeKeyList(aNode);
	m_pDoc->m_pAttrCtrl->GetElemKeyList(aElem);
	ActiveObj(aElem, TRUE, &aNode);
	I_GENModelBase	*gm = I_GENModelBase::GetCurMySelfST();
	gm->GetDispSetting()->m_Bndr.bNonlinearLinkNo = TRUE;
	MakeImage_Current(strName, FALSE);
	MakePicture(genElements, strName, _T("CHARPT_3_1_FIG"));
	gm->GetDispSetting()->m_Bndr.bNonlinearLinkNo = FALSE;
	SetShape(FALSE);

	CAutoRecoveryUnit unitSet(D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_FORCE_INDEX_KN, m_pDoc->m_pUnitCtrl, TRUE);

	//Table data
	T_KEY_LIST lstNlnk, lstLNR, lstLRB, lstHDR, lstESB, lstFPS;
	m_pDoc->m_pAttrCtrl->GetNlnkKeyList(lstNlnk);
	T_ASGB_D AsgbD;
	T_NLNK_D NlnkD;
	T_NLLP_D NllpD;
	for (int i = 0; i < lstNlnk.GetCount(); i++)
	{
		NlnkD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstNlnk[i], NlnkD))
		{
			ASSERT(0);
			continue;
		}
		NllpD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
		{
			ASSERT(0);
			continue;
		}
		if (NllpD.nApplicationType != 2 || NllpD.nSeisType != 5)
			continue;
		AsgbD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD))
		{
			ASSERT(0);
			continue;
		}
		if (AsgbD.nPropType == 0)
		{
			lstLNR.Add(lstNlnk[i]);
		}
		else if (AsgbD.nPropType == 1)
		{
			lstLRB.Add(lstNlnk[i]);
		}
		else if (AsgbD.nPropType == 2)
		{
			lstHDR.Add(lstNlnk[i]);
		}
		else if (AsgbD.nPropType == 3)
		{
			lstESB.Add(lstNlnk[i]);
		}
		else if (AsgbD.nPropType == 4)
		{
			lstFPS.Add(lstNlnk[i]);
		}
	}
	//Table 1 - LNR
	int nIndex = 1;
	if (lstLNR.GetSize() > 0)
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 8;
		TableD.HeaderD.aTitleName.Add(_T("编号"));
		TableD.HeaderD.aTitleName.Add(_T("分类"));
		TableD.HeaderD.aTitleName.Add(_T("规格型号"));
		TableD.HeaderD.aTitleName.Add(_T("有效直径D(mm)"));
		TableD.HeaderD.aTitleName.Add(_T("竖向刚度Kvc(kN/mm)"));
		TableD.HeaderD.aTitleName.Add(_T("竖向刚度Kvt(kN/mm)"));
		TableD.HeaderD.aTitleName.Add(_T("水平刚度Kh(kN/mm)"));
		TableD.HeaderD.aTitleName.Add(_T("橡胶层总厚度Tr(mm)"));					
		//  
		TableD.RecordD.aRecordData.SetSize(lstLNR.GetSize());
		auto& record = TableD.RecordD.aRecordData;
		for (int i = 0; i < lstLNR.GetSize(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstLNR[i], NlnkD))
			{
				ASSERT(0);
				continue;
			}
			if (!m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
			{
				ASSERT(0);
				continue;
			}
			m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);
			m_strFormat.Format(_T("%d"), lstLNR[i]);
			record[i].Add(m_strFormat);
			record[i].Add(AsgbD.LNR.strClassify);
			record[i].Add(AsgbD.LNR.strInputType);
			record[i].Add(Double2String_f(AsgbD.LNR.dD,0));
			record[i].Add(Double2String_f(AsgbD.LNR.dKv,0));
			record[i].Add(Double2String_f(AsgbD.LNR.dKvt,0));
			record[i].Add(Double2String_f(AsgbD.LNR.dKh));
			record[i].Add(Double2String_f(AsgbD.LNR.dTr,0));
		}
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = lstLNR.GetSize();
		TableD.RecordD.nRecordColCount = 8;
		str1.Format(_T("CHARPT_3_1_TBL%d"), nIndex);
		MakeTableCenterText(genElements, TableD, _T("天然橡胶支座(LNR)表格统计"), str1);
		nIndex++;
	}
	//Table 2 - LRB
	if (lstLRB.GetSize() > 0)
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 12;
		TableD.HeaderD.aTitleName.Add(_T("编号"));
		TableD.HeaderD.aTitleName.Add(_T("分类"));
		TableD.HeaderD.aTitleName.Add(_T("规格型号"));
		TableD.HeaderD.aTitleName.Add(_T("有效直径D(mm)"));
		TableD.HeaderD.aTitleName.Add(_T("竖向刚度Kvc(kN/mm)"));
		TableD.HeaderD.aTitleName.Add(_T("竖向刚度Kvt(kN/mm)"));
		TableD.HeaderD.aTitleName.Add(_T("水平刚度Kh(kN/mm)"));
		TableD.HeaderD.aTitleName.Add(_T("等效阻尼比(100%)"));
		TableD.HeaderD.aTitleName.Add(_T("屈服前刚度K1(kN/mm)"));
		TableD.HeaderD.aTitleName.Add(_T("屈服后刚度Kd(kN/mm)"));
		TableD.HeaderD.aTitleName.Add(_T("屈服力Qd(kN)"));
		TableD.HeaderD.aTitleName.Add(_T("橡胶层总厚度Tr(mm)"));
		//  
		TableD.RecordD.aRecordData.SetSize(lstLRB.GetSize());
		auto& record = TableD.RecordD.aRecordData;
		for (int i = 0; i < lstLRB.GetSize(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstLRB[i], NlnkD))
			{
				ASSERT(0);
				continue;
			}
			if (!m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
			{
				ASSERT(0);
				continue;
			}
			m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);
			m_strFormat.Format(_T("%d"), lstLRB[i]);
			record[i].Add(m_strFormat);
			record[i].Add(AsgbD.LRB.strClassify);
			record[i].Add(AsgbD.LRB.strInputType);
			record[i].Add(Double2String_f(AsgbD.LRB.dD,0));
			record[i].Add(Double2String_f(AsgbD.LRB.dKv,0));
			record[i].Add(Double2String_f(AsgbD.LRB.dKvt,0));
			record[i].Add(Double2String_f(AsgbD.LRB.dHorEquiStiff));
			record[i].Add(Double2String_f(AsgbD.LRB.dEquiRot,0));
			record[i].Add(Double2String_f(AsgbD.LRB.dK1));
			record[i].Add(Double2String_f(AsgbD.LRB.dKd));
			record[i].Add(Double2String_f(AsgbD.LRB.dOd,0));
			record[i].Add(Double2String_f(AsgbD.LRB.dTr,0));
		}
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = lstLRB.GetSize();
		TableD.RecordD.nRecordColCount = 12;
		str1.Format(_T("CHARPT_3_1_TBL%d"), nIndex);
		MakeTableCenterText(genElements, TableD, _T("铅芯橡胶支座(LRB)表格统计"), str1);
		nIndex++;
	}
	//Table 3 - HDR
	if (lstHDR.GetSize() > 0)
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 11;
		TableD.HeaderD.aTitleName.Add(_T("编号"));
		TableD.HeaderD.aTitleName.Add(_T("规格型号"));
		TableD.HeaderD.aTitleName.Add(_T("有效直径D(mm)"));
		TableD.HeaderD.aTitleName.Add(_T("竖向刚度Kvc(kN/mm)"));
		TableD.HeaderD.aTitleName.Add(_T("竖向刚度Kvt(kN/mm)"));
		TableD.HeaderD.aTitleName.Add(_T("水平刚度Kh(kN/mm)"));
		TableD.HeaderD.aTitleName.Add(_T("等效阻尼比(100%)"));
		TableD.HeaderD.aTitleName.Add(_T("屈服前刚度K1(kN/mm)"));
		TableD.HeaderD.aTitleName.Add(_T("屈服后刚度Kd(kN/mm)"));
		TableD.HeaderD.aTitleName.Add(_T("屈服力Qd(kN)"));
		TableD.HeaderD.aTitleName.Add(_T("橡胶层总厚度Tr(mm)"));
		//  
		TableD.RecordD.aRecordData.SetSize(lstHDR.GetSize());
		auto& record = TableD.RecordD.aRecordData;
		for (int i = 0; i < lstHDR.GetSize(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstHDR[i], NlnkD))
			{
				ASSERT(0);
				continue;
			}
			if (!m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
			{
				ASSERT(0);
				continue;
			}
			m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);
			m_strFormat.Format(_T("%d"), lstHDR[i]);
			record[i].Add(m_strFormat);
			record[i].Add(AsgbD.HDR.strInputType);
			record[i].Add(Double2String_f(AsgbD.HDR.dD,0));
			record[i].Add(Double2String_f(AsgbD.HDR.dKv,0));
			record[i].Add(Double2String_f(AsgbD.HDR.dKvt,0));
			record[i].Add(Double2String_f(AsgbD.HDR.dHorEquiStiff));
			record[i].Add(Double2String_f(AsgbD.HDR.dEquiRot,0));
			record[i].Add(Double2String_f(AsgbD.HDR.dK1));
			record[i].Add(Double2String_f(AsgbD.HDR.dKd));
			record[i].Add(Double2String_f(AsgbD.HDR.dOd,0));
			record[i].Add(Double2String_f(AsgbD.HDR.dTr,0));
		}
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = lstHDR.GetSize();
		TableD.RecordD.nRecordColCount = 11;
		str1.Format(_T("CHARPT_3_1_TBL%d"), nIndex);
		MakeTableCenterText(genElements, TableD, _T("高阻尼橡胶支座(HDR)表格统计"), str1);
		nIndex++;
	}
	//Table 4 - ESB
	if (lstESB.GetSize() > 0)
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 7;
		TableD.HeaderD.aTitleName.Add(_T("编号"));
		TableD.HeaderD.aTitleName.Add(_T("规格型号"));
		TableD.HeaderD.aTitleName.Add(_T("有效直径D(mm)"));
		TableD.HeaderD.aTitleName.Add(_T("竖向刚度Kv(kN/mm)"));
		TableD.HeaderD.aTitleName.Add(_T("初始刚度K1(kN/mm)"));
		TableD.HeaderD.aTitleName.Add(_T("动摩擦系数μ"));
		TableD.HeaderD.aTitleName.Add(_T("橡胶层总厚度Tr(mm)"));
		//  
		TableD.RecordD.aRecordData.SetSize(lstESB.GetSize());
		auto& record = TableD.RecordD.aRecordData;
		for (int i = 0; i < lstESB.GetSize(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstESB[i], NlnkD))
			{
				ASSERT(0);
				continue;
			}
			if (!m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
			{
				ASSERT(0);
				continue;
			}
			m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);
			m_strFormat.Format(_T("%d"), lstESB[i]);
			record[i].Add(m_strFormat);
			record[i].Add(AsgbD.ESB.strClassify);
			record[i].Add(Double2String_f(AsgbD.ESB.dD,0));
			record[i].Add(Double2String_f(AsgbD.ESB.dKv,0));
			record[i].Add(Double2String_f(AsgbD.ESB.dK1,0));
			record[i].Add(Double2String_f(AsgbD.ESB.dU,2));
			record[i].Add(Double2String_f(AsgbD.ESB.dTr,0));
		}
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = lstESB.GetSize();
		TableD.RecordD.nRecordColCount = 7;
		str1.Format(_T("CHARPT_3_1_TBL%d"), nIndex);
		MakeTableCenterText(genElements, TableD, _T("弹性滑板橡胶支座(ESB)表格统计"), str1);
		nIndex++;
	}
	//Table 5 - FPS
	if (lstFPS.GetSize() > 0)
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 8;
		TableD.HeaderD.aTitleName.Add(_T("编号"));
		TableD.HeaderD.aTitleName.Add(_T("规格型号"));
		TableD.HeaderD.aTitleName.Add(_T("竖向承载力(kN)"));
		TableD.HeaderD.aTitleName.Add(_T("竖向刚度(kN/mm)"));
		TableD.HeaderD.aTitleName.Add(_T("动摩擦系数下限值(慢)"));
		TableD.HeaderD.aTitleName.Add(_T("动摩擦系数下限值(快)"));
		TableD.HeaderD.aTitleName.Add(_T("等效曲率半径R(mm)"));
		TableD.HeaderD.aTitleName.Add(_T("设计水平位移(mm)"));
		//  
		TableD.RecordD.aRecordData.SetSize(lstFPS.GetSize());
		auto& record = TableD.RecordD.aRecordData;
		for (int i = 0; i < lstFPS.GetSize(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstFPS[i], NlnkD))
			{
				ASSERT(0);
				continue;
			}
			if (!m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
			{
				ASSERT(0);
				continue;
			}
			m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);
			m_strFormat.Format(_T("%d"), lstFPS[i]);
			record[i].Add(m_strFormat);
			record[i].Add(AsgbD.FPS.strInputType);
			record[i].Add(Double2String_f(AsgbD.FPS.dVBearing,0));
			record[i].Add(Double2String_f(AsgbD.FPS.dKv,0));
			record[i].Add(Double2String_f(AsgbD.FPS.dUs,2));
			record[i].Add(Double2String_f(AsgbD.FPS.dUf,2));
			record[i].Add(Double2String_f(AsgbD.FPS.nR,0));
			record[i].Add(Double2String_f(AsgbD.FPS.dHorDis,0));
		}
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = lstFPS.GetSize();
		TableD.RecordD.nRecordColCount = 8;
		str1.Format(_T("CHARPT_3_1_TBL%d"), nIndex);
		MakeTableCenterText(genElements, TableD, _T("摩擦摆橡胶支座(FPS)表格统计"), str1);
		nIndex++;
	}
}
void AutoReport_Gen_CH_ISGB::WriteCharpt3_2(GenSegmentElements &genElements)
{
	MakeTitle(genElements, _T("隔震支座的使用数据统计"), 3, 2);
	
	//   
	//Table data
	T_KEY_LIST lstNlnk;
	CMap<UINT, UINT, int, int> mapNllp_Num;
	m_pDoc->m_pAttrCtrl->GetNlnkKeyList(lstNlnk);
	T_ASGB_D AsgbD;
	T_NLNK_D NlnkD;
	T_NLLP_D NllpD;
	//T_ASGB_K AsgbK;
	for (int i = 0; i < lstNlnk.GetCount(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstNlnk[i], NlnkD) || !m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
		{
			ASSERT(0);
			continue;
		}
		if (NllpD.nApplicationType != 2 || NllpD.nSeisType != 5)
			continue;
		mapNllp_Num[NlnkD.PropKey]++;
	}
	CString strType[] = { _T("天然橡胶支座"), _T("铅芯橡胶支座"), _T("高阻尼橡胶支座"), _T("弹性滑板支座"), _T("摩擦摆支座") };
	//
	T_TABLE_D TableD;
	TableD.Initialize();
	TableD.HeaderD.nHeaderRowCount = 1;
	TableD.HeaderD.nHeaderColCount = 4;
	TableD.HeaderD.aTitleName.Add(_T("支座名称"));
	TableD.HeaderD.aTitleName.Add(_T("支座类型"));
	TableD.HeaderD.aTitleName.Add(_T("规格型号"));
	TableD.HeaderD.aTitleName.Add(_T("支座数量"));
	auto& record = TableD.RecordD.aRecordData;
	int nCount = mapNllp_Num.GetCount();
	record.SetSize(nCount);
	POSITION pos = mapNllp_Num.GetStartPosition();
	T_NLLP_K NllpK;
	int nIndex = 0;
	int nNllpNum;
	while (pos)
	{
		mapNllp_Num.GetNextAssoc(pos, NllpK, nNllpNum);
		m_pDoc->m_pAttrCtrl->GetNllp(NllpK, NllpD);
		m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);
		ASSERT(NllpD.nApplicationType == 2 && NllpD.nSeisType == 5);
		record[nIndex].Add(AsgbD.PropName);
		record[nIndex].Add(strType[AsgbD.nPropType]);
		if (AsgbD.nPropType == 0)
			record[nIndex].Add(AsgbD.LNR.strInputType);
		else if (AsgbD.nPropType == 1)
			record[nIndex].Add(AsgbD.LRB.strInputType);
		else if (AsgbD.nPropType == 2)
			record[nIndex].Add(AsgbD.HDR.strInputType);
		else if (AsgbD.nPropType == 3)
			record[nIndex].Add(AsgbD.ESB.strInputType);
		else if (AsgbD.nPropType == 4)
			record[nIndex].Add(AsgbD.FPS.strInputType);
		else
			ASSERT(FALSE);
		m_strFormat.Format(_T("%d"), nNllpNum);
		record[nIndex++].Add(m_strFormat);
	}
	TableD.RecordD.aRecordData.FreeExtra();
	TableD.RecordD.nRecordRowCount = nIndex;
	TableD.RecordD.nRecordColCount = 4;
	MakeTableCenterText(genElements, TableD ,_T(""), _T("CHARPT_3_2_TBL"));  
}
//void AutoReport_Gen_CH_ISGB::WriteCharpt3_2(GenSegmentElements &genElements)
//{
//	MakeTitle(genElements, _T("隔震支座的使用数据统计"), 3, 2);
//	
//	//   
//	//Table data
//	T_KEY_LIST lstNlnk, lstLNR, lstLRB, lstHDR, lstESB, lstFPS;
//	m_pDoc->m_pAttrCtrl->GetNlnkKeyList(lstNlnk);
//	T_ASGB_D AsgbD;
//	T_NLNK_D NlnkD, NlnkD_temp;
//	T_NLLP_D NllpD, NllpD_temp;
//	T_ASGB_K AsgbK;
//	for (int i = 0; i < lstNlnk.GetCount(); i++)
//	{
//		NlnkD.Initialize();
//		if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstNlnk[i], NlnkD))
//		{
//			ASSERT(0);
//			continue;
//		}
//		NllpD.Initialize();
//		if (!m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
//		{
//			ASSERT(0);
//			continue;
//		}
//		if (NllpD.nApplicationType != 2 || NllpD.nSeisType != 5)
//			continue;
//		AsgbD.Initialize();
//		if (!m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD))
//		{
//			ASSERT(0);
//			continue;
//		}
//		if (AsgbD.nPropType == 0)
//		{
//			lstLNR.Add(lstNlnk[i]);
//		}
//		else if (AsgbD.nPropType == 1)
//		{
//			lstLRB.Add(lstNlnk[i]);
//		}
//		else if (AsgbD.nPropType == 2)
//		{
//			lstHDR.Add(lstNlnk[i]);
//		}
//		else if (AsgbD.nPropType == 3)
//		{
//			lstESB.Add(lstNlnk[i]);
//		}
//		else if (AsgbD.nPropType == 4)
//		{
//			lstFPS.Add(lstNlnk[i]);
//		}
//	}
//	CString strType[] = { _T("天然橡胶支座"), _T("铅芯橡胶支座"), _T("高阻尼橡胶支座"), _T("弹性滑板支座"), _T("摩擦摆支座") };
//	CArrayEx<CArrayEx<T_NLNK_K, T_NLNK_K>, CArrayEx<T_NLNK_K, T_NLNK_K>> arLNRKey, arLRBKey, arHDRKey, arESBKey, arFPSKey;
//	CArrayEx<T_NLNK_K, T_NLNK_K> arKey;
//	int nRow, nLNR, nLRB, nHDR, nESB, nFPS;
//	if (lstLNR.GetSize()>1)
//	{
//		for (int i = 0; i < lstLNR.GetSize(); i++)
//		{
//			arKey.RemoveAll();
//			NlnkD.Initialize();
//			m_pDoc->m_pAttrCtrl->GetNlnk(lstLNR[i], NlnkD);
//			NllpD.Initialize();
//			m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
//			AsgbK = NllpD.nSeisKey;
//			arKey.Add(lstLNR[i]);		
//			for (int j = lstLNR.GetSize()-1; j >0; j--)
//			{
//				NlnkD_temp.Initialize();
//				m_pDoc->m_pAttrCtrl->GetNlnk(lstLNR[j], NlnkD_temp);
//				NllpD_temp.Initialize();
//				m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD_temp);
//				if (AsgbK == NllpD_temp.nSeisKey)
//				{
//					arKey.Add(lstLNR[j]);
//					lstLNR.RemoveAt(j);
//				}
//			}
//			arLNRKey.Add(arKey);
//			lstLNR.RemoveAt(i);
//		}
//	}
//	nLNR = arLNRKey.GetSize();
//	if (lstLRB.GetSize() > 1)
//	{
//		for (int i = 0; i < lstLRB.GetSize(); i++)
//		{
//			arKey.RemoveAll();
//			NlnkD.Initialize();
//			m_pDoc->m_pAttrCtrl->GetNlnk(lstLRB[i], NlnkD);
//			NllpD.Initialize();
//			m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
//			AsgbK = NllpD.nSeisKey;
//			arKey.Add(lstLRB[i]);
//			for (int j = lstLRB.GetSize()-1; j >0; j--)
//			{
//				NlnkD_temp.Initialize();
//				m_pDoc->m_pAttrCtrl->GetNlnk(lstLRB[j], NlnkD_temp);
//				NllpD_temp.Initialize();
//				m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD_temp);
//				if (AsgbK == NllpD_temp.nSeisKey)
//				{
//					arKey.Add(lstLRB[j]);
//					lstLRB.RemoveAt(j);
//				}
//			}
//			arLRBKey.Add(arKey);
//			lstLRB.RemoveAt(i);
//		}
//	}
//	nLRB = arLRBKey.GetSize();
//	if (lstHDR.GetSize() > 1)
//	{
//		for (int i = 0; i < lstHDR.GetSize(); i++)
//		{
//			arKey.RemoveAll();
//			NlnkD.Initialize();
//			m_pDoc->m_pAttrCtrl->GetNlnk(lstHDR[i], NlnkD);
//			NllpD.Initialize();
//			m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
//			AsgbK = NllpD.nSeisKey;
//			arKey.Add(lstHDR[i]);			
//			for (int j = lstHDR.GetSize()-1; j > 0; j--)
//			{
//				NlnkD_temp.Initialize();
//				m_pDoc->m_pAttrCtrl->GetNlnk(lstHDR[j], NlnkD_temp);
//				NllpD_temp.Initialize();
//				m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD_temp);
//				if (AsgbK == NllpD_temp.nSeisKey)
//				{
//					arKey.Add(lstHDR[j]);
//					lstHDR.RemoveAt(j);
//				}
//			}
//			arHDRKey.Add(arKey);
//			lstHDR.RemoveAt(i);
//		}
//	}
//	nHDR = arHDRKey.GetSize();
//	if (lstESB.GetSize() > 1)
//	{
//		for (int i = 0; i < lstESB.GetSize(); i++)
//		{
//			arKey.RemoveAll();
//			NlnkD.Initialize();
//			m_pDoc->m_pAttrCtrl->GetNlnk(lstESB[i], NlnkD);
//			NllpD.Initialize();
//			m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
//			AsgbK = NllpD.nSeisKey;
//			arKey.Add(lstESB[i]);
//			for (int j = lstESB.GetSize()-1; j > 0; j--)
//			{
//				NlnkD_temp.Initialize();
//				m_pDoc->m_pAttrCtrl->GetNlnk(lstESB[j], NlnkD_temp);
//				NllpD_temp.Initialize();
//				m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD_temp);
//				if (AsgbK == NllpD_temp.nSeisKey)
//				{
//					arKey.Add(lstESB[j]);
//					lstESB.RemoveAt(j);
//				}
//			}
//			arESBKey.Add(arKey);
//			lstESB.RemoveAt(i);
//		}
//	}
//	nESB = lstESB.GetSize();
//	if (lstFPS.GetSize() > 1)
//	{
//		for (int i = 0; i < lstFPS.GetSize(); i++)
//		{
//			arKey.RemoveAll();
//			NlnkD.Initialize();
//			m_pDoc->m_pAttrCtrl->GetNlnk(lstFPS[i], NlnkD);
//			NllpD.Initialize();
//			m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
//			AsgbK = NllpD.nSeisKey;
//			arKey.Add(lstFPS[i]);
//			for (int j = lstFPS.GetSize()-1; j > 0; j--)
//			{
//				NlnkD_temp.Initialize();
//				m_pDoc->m_pAttrCtrl->GetNlnk(lstFPS[j], NlnkD_temp);
//				NllpD_temp.Initialize();
//				m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD_temp);
//				if (AsgbK == NllpD_temp.nSeisKey)
//				{
//					arKey.Add(lstFPS[j]);
//					lstFPS.RemoveAt(j);
//				}
//			}
//			arFPSKey.Add(arKey);
//			lstFPS.RemoveAt(i);
//		}
//	}
//	nFPS = arFPSKey.GetSize();
//	nRow = nLNR + nLRB + nHDR + nESB + nFPS;
//
//	//
//	T_TABLE_D TableD;
//	TableD.Initialize();
//	TableD.HeaderD.nHeaderRowCount = 1;
//	TableD.HeaderD.nHeaderColCount = 4;
//	TableD.HeaderD.aTitleName.Add(_T("支座名称"));
//	TableD.HeaderD.aTitleName.Add(_T("支座类型"));
//	TableD.HeaderD.aTitleName.Add(_T("规格型号"));
//	TableD.HeaderD.aTitleName.Add(_T("支座数量"));
//	TableD.RecordD.aRecordData.SetSize(nRow);
//	auto& record = TableD.RecordD.aRecordData;
//	for (int i = 0; i < arLNRKey.GetCount(); i++)
//	{
//		arKey.RemoveAll();
//		arKey = arLNRKey[i];
//		NlnkD.Initialize();
//		m_pDoc->m_pAttrCtrl->GetNlnk(arKey[0], NlnkD);
//		NllpD.Initialize();
//		m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
//		AsgbD.Initialize();		
//		m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);
//		record[i].Add(AsgbD.PropName);
//		record[i].Add(strType[AsgbD.nPropType]);
//		record[i].Add(AsgbD.LNR.strInputType);
//		m_strFormat.Format("%d", arKey.GetCount());
//		record[i].Add(m_strFormat);
//	}
//	for (int i = 0; i < arLRBKey.GetCount(); i++)
//	{
//		arKey.RemoveAll();
//		arKey = arLRBKey[i];
//		NlnkD.Initialize();
//		m_pDoc->m_pAttrCtrl->GetNlnk(arKey[0], NlnkD);
//		NllpD.Initialize();
//		m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
//		AsgbD.Initialize();
//		m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);
//		record[i].Add(AsgbD.PropName);
//		record[i].Add(strType[AsgbD.nPropType]);
//		record[i].Add(AsgbD.LRB.strInputType);
//		m_strFormat.Format("%d", arKey.GetCount());
//		record[i].Add(m_strFormat);
//	}
//	for (int i = 0; i < arHDRKey.GetCount(); i++)
//	{
//		arKey.RemoveAll();
//		arKey = arHDRKey[i];
//		NlnkD.Initialize();
//		m_pDoc->m_pAttrCtrl->GetNlnk(arKey[0], NlnkD);
//		NllpD.Initialize();
//		m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
//		AsgbD.Initialize();
//		m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);
//		record[i].Add(AsgbD.PropName);
//		record[i].Add(strType[AsgbD.nPropType]);
//		record[i].Add(AsgbD.HDR.strInputType);
//		m_strFormat.Format("%d", arKey.GetCount());
//		record[i].Add(m_strFormat);
//	}
//	for (int i = 0; i < nESB; i++)
//	{
//		arKey.RemoveAll();
//		arKey = arESBKey[i];
//		NlnkD.Initialize();
//		m_pDoc->m_pAttrCtrl->GetNlnk(lstESB[0], NlnkD);
//		NllpD.Initialize();
//		m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
//		AsgbD.Initialize();
//		m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);
//		record[i].Add(AsgbD.PropName);
//		record[i].Add(strType[AsgbD.nPropType]);
//		record[i].Add(AsgbD.ESB.strInputType);
//		m_strFormat.Format("%d", arKey.GetCount());
//		record[i].Add(m_strFormat);
//	}
//	for (int i = 0; i < nFPS; i++)
//	{
//		arKey.RemoveAll();
//		arKey = arFPSKey[i];
//		NlnkD.Initialize();
//		m_pDoc->m_pAttrCtrl->GetNlnk(arKey[0], NlnkD);
//		NllpD.Initialize();
//		m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
//		AsgbD.Initialize();
//		m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);
//		record[i].Add(AsgbD.PropName);
//		record[i].Add(strType[AsgbD.nPropType]);
//		record[i].Add(AsgbD.FPS.strInputType);
//		m_strFormat.Format("%d", arKey.GetCount());
//		record[i].Add(m_strFormat);
//	}
//	TableD.RecordD.aRecordData.FreeExtra();
//	TableD.RecordD.nRecordRowCount = nRow;
//	TableD.RecordD.nRecordColCount = 4;
//	MakeTableCenterText(genElements, TableD ,_T(""), "CHARPT_3_2_TBL");  
//}
void AutoReport_Gen_CH_ISGB::WriteCharpt4()
{
	GenSegmentElements Elems;
	MakeTitle(Elems, _T("荷载的输入"), 4, 0);
	WriteCharpt4_1(Elems);
	WriteCharpt4_2(Elems);
	m_Segments.push_back(Elems);
}
void AutoReport_Gen_CH_ISGB::WriteCharpt4_1(GenSegmentElements &genElements)
{
	MakeTitle(genElements, _T("反应谱荷载的输入"), 4, 1);
	CString str1;
	str1.Format(_T("    程序中定义的反应谱荷载工况如下："));
	MakeText(genElements, str1);

	//Table
	CString strSeisType[] = { _T("设防地震"), _T("罕遇地震"), _T("极罕遇地震") };
	double dMaxEQE, dTg;
	int nEarthResponse;
	BOOL bFrequent;
	CArray<T_SPLC_K, T_SPLC_K> rKeyList;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(rKeyList);
	T_SPLC_D SplcD;
	T_SPFC_D SpfcD;
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 5;
		TableD.HeaderD.aTitleName.Add(_T("工况名称"));
		TableD.HeaderD.aTitleName.Add(_T("作用方向"));
		TableD.HeaderD.aTitleName.Add(_T("地震类别"));
		TableD.HeaderD.aTitleName.Add(_LS(IDS_TM_ISGB_AUTO_WORD_ALPHA_MAX));//αmax
		TableD.HeaderD.aTitleName.Add(_T("特征周期Tg(s)"));
		//  
		TableD.RecordD.aRecordData.SetSize(rKeyList.GetSize());
		auto& record = TableD.RecordD.aRecordData;
		for (int i = 0; i < rKeyList.GetSize(); i++)
		{
			bFrequent = FALSE;
			SplcD.Initialize();
			m_pDoc->m_pAttrCtrl->GetSplc(rKeyList[i], SplcD);
			if (SplcD.nDirection == 1)
				m_strFormat.Format(_T("X-Y-%.0f"), SplcD.dblAngle);
			else
				m_strFormat = _T("Z");
			
			SpfcD.Initialize();
			m_pDoc->m_pAttrCtrl->GetSpfc(SplcD.aSpfcKey[0], SpfcD);
			if (SpfcD.nSpecCodeType == D_SPFC_CODE_CH2019)
			{
				dMaxEQE = SpfcD.CodeParam.CH2019.dMaxEQE;
				dTg = SpfcD.CodeParam.CH2019.dTg;
				nEarthResponse = SpfcD.CodeParam.CH2019.nEarthResponse;
				if (nEarthResponse == 0)
				{
					rKeyList.RemoveAt(i);
					continue;
				}
				//
				record[i].Add(SplcD.LoadCaseName);
				record[i].Add(m_strFormat);
				record[i].Add(strSeisType[nEarthResponse]);
				record[i].Add(Double2String_f(dMaxEQE));
				record[i].Add(Double2String_f(dTg));
			}
			else if (SpfcD.nSpecCodeType == D_SPFC_CODE_CH2010)
			{
				dMaxEQE = SpfcD.CodeParam.CH2010.dMaxEQE;
				dTg = SpfcD.CodeParam.CH2010.dTg;
				nEarthResponse = SpfcD.CodeParam.CH2010.nEarthResponse;
				if (nEarthResponse == 0)
				{
					rKeyList.RemoveAt(i);
					continue;
				}
				//
				record[i].Add(SplcD.LoadCaseName);
				record[i].Add(m_strFormat);
				record[i].Add(strSeisType[nEarthResponse]);
				record[i].Add(Double2String_f(dMaxEQE));
				record[i].Add(Double2String_f(dTg));
			}
			else if (SpfcD.nSpecCodeType == D_SPFC_CODE_CH_GB_T_51408_2021)
			{
				dMaxEQE = SpfcD.CodeParam.GB_T_51408_2021.dMaxEQE;
				dTg = SpfcD.CodeParam.GB_T_51408_2021.dTg;
				nEarthResponse = SpfcD.CodeParam.GB_T_51408_2021.nEarthResponse;
				//
				record[i].Add(SplcD.LoadCaseName);
				record[i].Add(m_strFormat);
				record[i].Add(strSeisType[nEarthResponse]);
				record[i].Add(Double2String_f(dMaxEQE));
				record[i].Add(Double2String_f(dTg));
			}
			else if (SpfcD.nSpecCodeType == D_SPFC_CODE_CH2002)
			{
				dMaxEQE = SpfcD.CodeParam.CH2002.dMaxEQE;
				dTg = SpfcD.CodeParam.CH2002.dTg;
				nEarthResponse = SpfcD.CodeParam.CH2002.nEarthResponse;
				if (nEarthResponse == 0)
				{
					rKeyList.RemoveAt(i);
					continue;
				}
				//
				record[i].Add(SplcD.LoadCaseName);
				record[i].Add(m_strFormat);
				record[i].Add(strSeisType[nEarthResponse]);
				record[i].Add(Double2String_f(dMaxEQE));
				record[i].Add(Double2String_f(dTg));
			}
			else
				ASSERT(FALSE);
			
			
		}
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = rKeyList.GetSize();
		TableD.RecordD.nRecordColCount = 5;
		MakeTableCenterText(genElements, TableD, _T("反应谱荷载工况"), _T("CHARPT_4_1_TBL"));
	}
}
void AutoReport_Gen_CH_ISGB::WriteCharpt4_2(GenSegmentElements &genElements)
{
	MakeTitle(genElements, _T("时程荷载的输入"), 4, 2);

	CString str, str1, str2, str3;
	str1.Format(_T("    《建筑抗震设计规范》（GB/T50011-2010，以下简称《抗规》）5.1.2条规定：采用时程分析法时，应按建筑场地类别和设计地震分组选用实际强震记录和人工模拟的加速度时程，"));
	str2.Format(_T("其中实际强震记录的数量不应少于总数的2/3，多组时程的平均地震影响系数曲线应与振型分解反应谱法所采用的地震影响系数曲线在统计意义上相符。"));
	str3.Format(_T("弹性时程分析时，每条时程计算的结构底部剪力不应小于振型分解反应谱计算结果的65%%，多条时程计算的结构底部剪力的平均值不应小于振型分解反应谱法计算结果的80%%。地震波取自弹性时程。"));
	str.Format(_T("%s%s%s"),str1, str2, str3);
	MakeText(genElements, str);
	str1.Format(_T("    地震波函数如下："));
	MakeText(genElements, str1);

	T_THFC_D ThfcD;
	CArray<T_THFC_K, T_THFC_K> aThfcKey;
	m_pDoc->m_pAttrCtrl->GetThfcKeyList(aThfcKey);

	CString strName;
	CString strBookMark;
	SetShape(FALSE);
	strName = _T("加速度时程曲线");
	ActiveElemAll();
	I_GENModelBase	*gm = I_GENModelBase::GetCurMySelfST();
	
	CThfcItemDlg dlg;	
	for (int i=0; i<aThfcKey.GetCount(); i++)
	{
		ThfcD.Initialize();
		if (m_pDoc->m_pAttrCtrl->GetThfc(aThfcKey.GetAt(i), ThfcD))
		{
			dlg.SetParamData(ThfcD);
			dlg.AutoBtnDyGen();
		}
	}

	CArray<T_UCHT_K, T_UCHT_K> aUchtK;  aUchtK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetUchtKeyList(aUchtK);
	int nUchtNum = aUchtK.GetSize();
	if (nUchtNum <= 0)
	{
		ASSERT(0); return;
	}
	T_UNIT_INDEX UnitIndex_cur;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex_cur);
	T_CHART_ITEM_D  ChartItemD; ChartItemD.Initialize();
	CGPSAMRUDCData  GenChart;
	CString strFilePath = _T("");
	T_UCHT_D UchtD;
	T_UCHT_K UchtK;
	int nSuffix = 1;
	for (int i = 0; i < aThfcKey.GetCount(); i++)
	{
		POSITION pos = m_pDoc->m_pAttrCtrl->GetStartUcht();
		while (pos)
		{
			m_pDoc->m_pAttrCtrl->GetNextUcht(pos, UchtK, UchtD);
			if (UchtD.nType == D_TYPE_TH_FUNC && UchtD.nKey == aThfcKey[i])
			{
				break;
			}
		}
		m_pDoc->m_pAttrCtrl->GetThfc(aThfcKey[i], ThfcD);
		strName.Format(_T("图 4.2-%d %s加速度时程曲线"), i + 1, ThfcD.ThisFuncName);	
		ChartItemD.Initialize();
		if (!GenChart.GetData(UchtK, ChartItemD, strFilePath))
			continue;
		m_pDoc->MakeGraphAndBmpImage(ChartItemD, strFilePath, &UchtD);
		strBookMark.Format(_T("CHARPT_4_2_FIG_%d"), nSuffix++);
		strName = strFilePath.Right(strFilePath.GetLength() - 1 - strFilePath.ReverseFind('\\'));
		strName = strName.Left(strName.Find('.'));
		MakePicture(genElements, strName, strBookMark);
	}

	//Table
	{
		T_THGA_D ThgaD;
		T_THFC_D ThfcD;
		T_THIS_D ThisD;
		CArray<T_THIS_K, T_THIS_K> aThisKey;
		m_pDoc->m_pAttrCtrl->GetThisKeyList(aThisKey, FALSE);
		CString csDir, csFuncName, csTime, csStep;
		//
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 5;
		TableD.HeaderD.aTitleName.Add(_T("工况名称"));
		TableD.HeaderD.aTitleName.Add(_T("作用方向"));
		TableD.HeaderD.aTitleName.Add(_T("时程函数名称"));
		TableD.HeaderD.aTitleName.Add(_T("作用时长(s)"));
		TableD.HeaderD.aTitleName.Add(_T("步长(s)"));
		//  
		TableD.RecordD.aRecordData.SetSize(aThisKey.GetSize());
		auto& record = TableD.RecordD.aRecordData;
		record.SetSize(3 * aThisKey.GetSize());
		for (int i = 0; i < aThisKey.GetCount(); i++)
		{
			ThisD.Initialize();
			ThgaD.Initialize();
			if (m_pDoc->m_pAttrCtrl->GetThis(aThisKey.GetAt(i), ThisD) && m_pDoc->m_pAttrCtrl->GetThga(aThisKey.GetAt(i), ThgaD))
			{
				csFuncName.Empty();
				csDir.Empty();
				csTime.Empty();
				csStep.Empty();
				for (int k = 0; k < 3; k++)
				{
					if (ThgaD.nThisFuncKey[k] == 0)
						continue;
					m_pDoc->m_pAttrCtrl->GetThfc(ThgaD.nThisFuncKey[k], ThfcD);
					csFuncName = ThfcD.ThisFuncName;
					if(k == 0)
						csDir.Format(_T("X-%d,"), ThgaD.dAngleHGA);
					else if(k == 1)
						csDir.Format(_T("Y-%d,"), ThgaD.dAngleHGA);
					else
						csDir = _T("Z");
					csTime.Format(_T("%.4f"), ThisD.dEndTime);
					csStep.Format(_T("%.4f"), ThisD.dDelta);
					record[i].Add(ThisD.LoadCaseName);
					record[i].Add(csDir);
					record[i].Add(csFuncName);
					record[i].Add(csTime);
					record[i].Add(csStep);
				}
			}
		}
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = aThisKey.GetSize();
		TableD.RecordD.nRecordColCount = 5;
		MakeTableCenterText(genElements, TableD, _T("时程荷载列表"), _T("CHARPT_4_2_TBL"));
	}
}
void AutoReport_Gen_CH_ISGB::WriteCharpt5()
{
	GenSegmentElements genElements;
	MakeTitle(genElements, _T("分析结果"), 5, 0);
	WriteCharpt5_1(genElements);
	WriteCharpt5_2(genElements);
	WriteCharpt5_3(genElements);
	m_Segments.push_back(genElements);
}
void AutoReport_Gen_CH_ISGB::WriteCharpt5_1(GenSegmentElements &genElements)
{
	CString strText;
	MakeTitle(genElements, _T("周期及振型结果"), 5, 1);
	if (!m_pDoc->IsPostMode())
		return;
	int nModeSize = m_pDoc->m_pPostCtrl->GetNumEigvPost();
	if (nModeSize == 0)
		return;
	//Report_Key_LIST aModelKeys;
	//CArray<int, int> aKey;
	CString str, strTemp;
	
	bool bModalDamp = true;
	if (m_pDoc->m_pPostCtrl->IsFromD2EResult())
	{
		bModalDamp = false;
	}
	if (bModalDamp)
	{
		T_GRDP_D GrdpD;
		if (!m_pDoc->m_pAttrCtrl->GetGrdp(GrdpD))GrdpD.Initialize();
		BOOL bIsModalDampingSolved = m_pDoc->m_pPostCtrl->IsModalDampSolved(GrdpD);
		bModalDamp = m_pDoc->m_pPostCtrl->GetNumEigvPost() > 0 && bIsModalDampingSolved &&(m_pDoc->m_pStagCtrl->IsFinalStag() || m_pDoc->m_pStagCtrl->IsBaseStag());
	}
	T_TABLE_D TableD;
	TableD.Initialize();
	//振型阻尼比
	if (bModalDamp)
	{
		CGPSModalDampEngine Eng;
		Eng.InitEngine();
		//Eng.GetKeyList(aKey);
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 7;
		TableD.HeaderD.aTitleName.Add(_T("振型号"));
		TableD.HeaderD.aTitleName.Add(_T("频率(Hz)"));
		TableD.HeaderD.aTitleName.Add(_T("周期(s)"));
		TableD.HeaderD.aTitleName.Add(_T("M.P.M X(%)"));
		TableD.HeaderD.aTitleName.Add(_T("M.P.M Y(%)"));
		TableD.HeaderD.aTitleName.Add(_T("M.P.M Z(%)"));
		TableD.HeaderD.aTitleName.Add(_T("振型阻尼比"));
		TableD.RecordD.aRecordData.SetSize(nModeSize);
		T_GPS_MDDP_D data;
		for (int i = 0; i < nModeSize; i++)
		{
			TableD.RecordD.aRecordData[i].SetSize(7);
			if (!Eng.GetData(i+1, data))
			{
				ASSERT(FALSE);
				data.Initialize();
			}
			TableD.RecordD.aRecordData[i][0].Format(_T("%d"), i + 1);
			TableD.RecordD.aRecordData[i][1].Format(_T("%.2f"), data.dFrequency);
			TableD.RecordD.aRecordData[i][2].Format(_T("%.2f"), data.dPeriod);
			TableD.RecordD.aRecordData[i][3].Format(_T("%.2f"), data.dModalPartMass[0]);
			TableD.RecordD.aRecordData[i][4].Format(_T("%.2f"), data.dModalPartMass[1]);
			TableD.RecordD.aRecordData[i][5].Format(_T("%.2f"), data.dModalPartMass[2]);
			TableD.RecordD.aRecordData[i][6].Format(_T("%.2f"), data.dModalDamp);
			strTemp.Format(_T("T%d=%.3gs,"), i + 1, data.dPeriod);
			str += strTemp;
		}
	}
	else
	{
		//平动质量取值
		T_MPAM_D MpamD;
		T_FREQ_D FreqD;
		CArray<T_SPFC_K, T_SPFC_K> aSpfcKeyList;
		T_SPFC_D SpfcD; SpfcD.Initialize();
		m_pDoc->m_pAttrCtrl->GetSpfcKeyList(aSpfcKeyList);
		if (!aSpfcKeyList.IsEmpty())
		{
			m_pDoc->m_pAttrCtrl->GetSpfc(aSpfcKeyList[0], SpfcD);
		}

		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 7;
		TableD.HeaderD.aTitleName.Add(_T("振型号"));
		TableD.HeaderD.aTitleName.Add(_T("频率(Hz)"));
		TableD.HeaderD.aTitleName.Add(_T("周期(s)"));
		TableD.HeaderD.aTitleName.Add(_T("X向平动质量系数"));
		TableD.HeaderD.aTitleName.Add(_T("Y向平动质量系数"));
		TableD.HeaderD.aTitleName.Add(_T("Z向平动质量系数"));
		TableD.HeaderD.aTitleName.Add(_T("阻尼比"));
		TableD.RecordD.aRecordData.SetSize(nModeSize);
		for (int i = 0; i < nModeSize; ++i)
		{
			int nMode = i + 1;
			if (!m_pDoc->m_pPostCtrl->GetEiMpam(nMode, MpamD) || !m_pDoc->m_pPostCtrl->GetEiFreq(nMode, FreqD))
			{
				ASSERT(FALSE);
				continue;
			}

			TableD.RecordD.aRecordData[i].SetSize(7);
			TableD.RecordD.aRecordData[i][0].Format(_T("%d"), i + 1);
			TableD.RecordD.aRecordData[i][1].Format(_T("%.2f"), FreqD.dblFreq[1]);
			TableD.RecordD.aRecordData[i][2].Format(_T("%.2f"), FreqD.dblFreq[2]);
			TableD.RecordD.aRecordData[i][3].Format(_T("%.2f"), MpamD.dblMpam[0]);
			TableD.RecordD.aRecordData[i][4].Format(_T("%.2f"), MpamD.dblMpam[1]);
			TableD.RecordD.aRecordData[i][5].Format(_T("%.2f"), MpamD.dblMpam[5]);
			TableD.RecordD.aRecordData[i][6].Format(_T("%.2f"), SpfcD.dDampingRatio);
			//strTemp.Format(_T("T%d=%.3gs,"), i + 1, data.dPeriod);
			//str += strTemp;
		}
	}
	TableD.RecordD.aRecordData.FreeExtra();
	TableD.RecordD.nRecordRowCount = nModeSize;
	TableD.RecordD.nRecordColCount = 7;
	//strText.Format(_T("    本结构第1~第%d阶自振周期为：%s,各阶振型如下："), i, str);
	//MakeText(genElements, strText);
	MakeTableCenterText(genElements, TableD, _T("模态特征周期"), _T("CHARPT_5_1_TBL"));
	
	//Figure 5.1
	{
		Report_Key_LIST aModelKeys, aGroupKey;
		CViewBase* pView = static_cast<CViewBase*>(CViewBase::GetCurView_ST());
		I_GPSModel* gm = pView->GetIPM();
		FigureRltGM(FALSE);

		Report_Key_LIST aReportKey;
		CString strBook;
		int nIndex = 0;
		if (Para()->LoadCaseKeys(D_LOADCASE_EIGEN, aModelKeys))
		{
			int nIndex = 0;
			//WriteModelRlt(genElements, gm, aModelKeys, aGroupKey, nIndex);
			Para()->GroupKeys(aGroupKey, ARGC_ISGB * 1000 + 1);//1:Mode Group
			WriteModelRlt(genElements, gm, aModelKeys, aGroupKey, nIndex);
		}
	}
}
void AutoReport_Gen_CH_ISGB::WriteCharpt5_2(GenSegmentElements &genElements)
{
	Report_Key_LIST aKeys;
	Para()->ListReportOption(aKeys, ARGC_ISGB);
	SetViewPoint(0);
	CViewBase* pView = static_cast<CViewBase*>(CViewBase::GetCurView_ST());
	I_GPSModel* gm = pView->GetIPM();
	CDC *pDC = pView->GetDC();
	//
	int nHPixel = pDC->GetDeviceCaps(HORZRES);
	double dFontRatio = 0.0;
	if (nHPixel >= 1280) dFontRatio = 1.1;
	else if (nHPixel >= 1152 && nHPixel < 1280) dFontRatio = 1.0;
	else if (nHPixel >= 1024 && nHPixel < 1152) dFontRatio = 1.0;
	else if (nHPixel < 1024) dFontRatio = nHPixel / 1400.0;
	gm->m_LegendEngine.InitFont(20 * dFontRatio, 14 * dFontRatio);
	//
	gm->m_LoadMinMaxType = LOADTYPE_NORMAL;
	gm->m_bLoadDataExist = TRUE;
	gm->m_bLoadDataSet = TRUE;
	gm->m_bContour = TRUE;
	gm->m_DOPT.LG.m_bDrawLegend = TRUE;//云图
	gm->m_bDeformed = FALSE;//是否变形
	gm->m_bAnimation = FALSE;
	gm->m_DOPT.DR.m_bDrawInactive = FALSE;
	gm->m_DOPT.TX.m_bOutputNumber = FALSE;//是否显示数值
	gm->m_bMdShpContour = TRUE;//Model
	gm->m_bAnimation = FALSE;//动画模式,是否随动变换等值线
	if (gm == NULL)
		return;
	
	Report_Key_LIST aReportKey;
	CString strBook, strName;
	int nIndex = 0;
	strName = _T("构件内力结果");
	MakeTitle(genElements, strName, 5, 2);
	strName = _T("    在典型荷载工况下，各构件典型内力结果如下：");
	MakeText(genElements, strName);

	for (int i = 0; i < aKeys.GetCount() / 4; i++)
	{
		aReportKey.RemoveAll();
		if (aKeys[i * 4].m_nKey == 0 || aKeys[i * 4].m_nKey == 1)//Bforce TForce
		{
			for (int j = 0; j < 4; j++)
				aReportKey.Add(aKeys[i * 4 + j]);
			strBook.Format(_T("CHARPT_5_2_FIG_%d"), ++nIndex);
			WriteForceAndDispRlt(genElements, gm, aReportKey, strBook);
		}
	}
	gm->m_LoadMinMaxType = LOADTYPE_NORMAL;
}
void AutoReport_Gen_CH_ISGB::WriteCharpt5_3(GenSegmentElements &genElements)
{
	Report_Key_LIST aKeys;
	Para()->ListReportOption(aKeys, ARGC_ISGB);
	SetViewPoint(0);
	CViewBase* pView = static_cast<CViewBase*>(CViewBase::GetCurView_ST());
	I_GPSModel* gm = pView->GetIPM();
	FigureRltGM(FALSE);

	Report_Key_LIST aReportKey;
	CString strBook, strName;
	int nIndex = 0;
	strName = _T("结构位移结果");
	MakeTitle(genElements, strName, 5, 3);
	strName = _T("    在典型荷载作用下，结构的位移如下：");
	MakeText(genElements, strName);
	for (int i = 0; i < aKeys.GetCount() / 4; i++)
	{
		aReportKey.RemoveAll();
		if (aKeys[i * 4].m_nKey == 2)//Deform Contour
		{
			for (int j = 0; j < 4; j++)
				aReportKey.Add(aKeys[i * 4 + j]);
			strBook.Format(_T("CHARPT_3_4_1_FIG_%d"), ++nIndex);
			WriteForceAndDispRlt(genElements, gm, aReportKey, strBook);
		}
	}
}

void AutoReport_Gen_CH_ISGB::WriteCharpt6()
{
	GenSegmentElements genElements;
	MakeTitle(genElements, _T("隔震结构分析结果"), 6, 0);
	WriteCharpt6_1(genElements);
	WriteCharpt6_2(genElements);
	WriteCharpt6_3(genElements);
	m_Segments.push_back(genElements);
}
void AutoReport_Gen_CH_ISGB::WriteCharpt6_1(GenSegmentElements &genElements)
{
	CString strText, strBook;
	strText = _T("隔震与非隔震模型的隔震层底部剪力比");
	MakeTitle(genElements, strText, 6, 1);
	strText = _T("    注：隔震与非隔震模型的隔震层底部剪力比均来自中震反应谱结果。");
	MakeText(genElements, strText);
	
	int nIndex = 1;
	CArray<T_STOR_K, T_STOR_K> arStor, arUpStor;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(arStor);
	
	int nStorNum = arUpStor.GetCount();
	CArray<T_SSRT_K, T_SSRT_K> aSsrtK;


	T_SPLC_D SplcD;
	CArray<SeisLayerShear, SeisLayerShear&> arShearX, arShearY;
	CArray<T_STOR_SSFC, T_STOR_SSFC&> aSSFC;	
	T_TABLE_D TableD;
	TableD.Initialize();
	TableD.HeaderD.nHeaderRowCount = 1;
	TableD.HeaderD.nHeaderColCount = 5;
	TableD.HeaderD.aTitleName.Add(_T("层号"));
	TableD.HeaderD.aTitleName.Add(_T("塔号"));
	TableD.HeaderD.aTitleName.Add(_T("隔震结构隔震层底部剪力(kN)"));
	TableD.HeaderD.aTitleName.Add(_T("非隔震结构隔震层底部剪力(kN)"));
	TableD.HeaderD.aTitleName.Add(_T("底部剪力比"));
	CAutoRecoveryUnit unitSet(D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_FORCE_INDEX_KN, m_pDoc->m_pUnitCtrl, TRUE);
	for (int i = 0; i < m_IsgbD.aResistSplcCase.GetCount(); i++)
	{
		SplcD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetSplc(m_IsgbD.aResistSplcCase[i], SplcD)) { ASSERT(0); return; }
		if (!OpenSSFCFile(Para()->GetFilePath()))
			return;
		ReadSSFCData(SplcD.LoadCaseName, arShearX, arShearY);
		fclose(m_rfp);
		/**********************只要一个方向的**********************/
		//for (int j = 0; j < 2; j++)
		{
			int dir = 1;
			int dblAngle = (int(SplcD.dblAngle) + 360) % 360;
			if (dblAngle <= 45 || dblAngle > 315 || (dblAngle > 135 && dblAngle < 225))
			{
				dir = 0;
			}
			const CArray<SeisLayerShear, SeisLayerShear&>& arShear = dir == 0 ? arShearX : arShearY;
			aSSFC.RemoveAll();			
			CStoryDisplDrift m_Ssfc;
			if (!m_Ssfc.MakeShearWeightCoeff(m_IsgbD.aResistSplcCase[i], aSSFC, TRUE))
				continue;
			if (arShear.GetSize() != aSSFC.GetSize())
			{
				GSaveHistoryNF(_LS(IDS_MAIN_DYGEN_REPORT_ISGB_COMPARISON_ERROR));
				continue;
			}
			TableD.RecordD.aRecordData.SetSize(aSSFC.GetCount());
			CString str, strTemp;
			for (int k = 0; k < aSSFC.GetCount(); k++)
			{
				//if (arStor[k] < m_IsgbD.nBeginLayer)
				//	continue;
				TableD.RecordD.aRecordData[k].SetSize(5);
				TableD.RecordD.aRecordData[k][0] = arShear[k].strModle;
				TableD.RecordD.aRecordData[k][1] = arShear[k].strStor;
				TableD.RecordD.aRecordData[k][2].Format(_T("%.2f"), aSSFC[k].dStoryShear[dir]);
				TableD.RecordD.aRecordData[k][3].Format(_T("%.2f"), arShear[k].dShear);
				TableD.RecordD.aRecordData[k][4].Format(_T("%.2f"), aSSFC[k].dStoryShear[dir] / arShear[k].dShear);
			}
			TableD.RecordD.aRecordData.FreeExtra();
			TableD.RecordD.nRecordRowCount = aSSFC.GetCount();
			TableD.RecordD.nRecordColCount = 5;
			CString strTitle;
			strTitle.Format(_T("表6.1-%d %s工况下隔震与非隔震模型的隔震层底部剪力比"), nIndex, SplcD.LoadCaseName);
			strBook.Format(_T("CHARPT_6_1_%d_TBL"), nIndex);
			MakeTableCenterText(genElements, TableD, strTitle, strBook);
			nIndex++;
		}
	}
}
void AutoReport_Gen_CH_ISGB::WriteCharpt6_2(GenSegmentElements &genElements)
{
	CString strText;
	CString strTitle, strBookTip;
	strText = _T("隔震结构层间位移角");
	MakeTitle(genElements, strText, 6, 2);
	if (!m_pDoc->IsPostMode())
		return;
	if (m_IsgbD.nCode == 0)
	{
		strText = _T("    根据《建筑隔震设计标准》GB/T 51408-2021第4.5节内容，层间位移角验算结果如下：");
		MakeText(genElements, strText);
	}
	
	int nSplc1 = m_IsgbD.aResistSplcCase.GetCount();
	int nSplc2 = m_IsgbD.aRareESplcCase.GetCount();
	int nSplc3 = m_IsgbD.aExtreRareESplcCase.GetCount();
	int nThis1 = m_IsgbD.aResistThisCase.GetCount();
	int nThis2 = m_IsgbD.aRareEThisCase.GetCount();
	int nThis3 = m_IsgbD.aExtreRareEThisCase.GetCount();
	int nIndex = 0;
	int nStorIndex = 0;
	CStoryDisplDrift StDrft;
	CArray<T_STOR_DFT, T_STOR_DFT&> arStorDrift;
	//T_STOR_DFT Drift;
	CArray<T_STOR_DFT_TH, T_STOR_DFT_TH&> arStorDriftTH;
	T_STOR_DFT_TH DriftTH;
	//wangjing:only all
	//CArray<int, int> aAllMinMax; aAllMinMax.RemoveAll();
	//aAllMinMax.Add(0);	// all (例措蔼 奴 巴)
	//aAllMinMax.Add(1);	// max
	//aAllMinMax.Add(-1);	// min
	T_DRIFT_METHOD ActiveMethod;
	ActiveMethod.Initialize();
	ActiveMethod.bMethod[0] = TRUE;
	int nMinMax = 0;
	int i, j;
	T_SPLC_D SplcD;
	T_THIS_D ThisD;
	T_SPLC_K SplcK;
	T_THIS_K ThisK;
	T_STOR_D StorD;
	T_LCOM_D LcomData;
	CArray<T_STOR_K, T_STOR_K> arStor;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(arStor);
	
	CString strStruct = _T("");
	switch (m_IsgbD.nStrutCategory)
	{
	case 0: strStruct = _T("钢筋混凝土框架结构"); break;
	case 1: strStruct = _T("钢筋混凝土框架-抗震墙结构"); break;
	case 2: strStruct = _T("框架-核心筒结构"); break;
	case 3: strStruct = _T("板柱-抗震墙结构"); break;
	case 4: strStruct = _T("钢筋混凝土抗震墙"); break;	
	case 5: strStruct = _T("钢结构"); break;
	default:
		break;
	}

	CString strBldCategory = _T("");
	switch (m_IsgbD.nBldCategory)
	{
	case 0: strBldCategory = _T("Ⅰ类"); break;
	case 1: strBldCategory = _T("Ⅱ类"); break;
	case 2: strBldCategory = _T("一般"); break;
	default:
		break;
	}

	BOOL bBJGeneral = m_IsgbD.nCode == 1 && m_IsgbD.nBldCategory == 2;

	//Table list 1
	if (nSplc1 > 0 && !bBJGeneral)
	{
		strText.Format(_T("    根据《建筑工程减隔震技术规程》北京市地方标准 DB11/2075-2022第5.4.1条，%s建筑，该%s在设防地震作用下层间位移角验算结果如下："), strBldCategory, strStruct);
		for (i = 0; i < nSplc1; i++)
		{
			arStorDrift.RemoveAll();
			SplcD.Initialize();
			SplcK = m_IsgbD.aResistSplcCase[i];
			m_pDoc->m_pAttrCtrl->GetSplc(SplcK, SplcD);
			if (SplcD.nDirection == 2)
				continue;
			m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_SPECTRUM, SplcK, LcomData);
			if (!StDrft.GetDFT(LcomData, 0, ActiveMethod, arStorDrift)) continue;

			T_TABLE_D TableD;
			TableD.Initialize();
			TableD.HeaderD.nHeaderRowCount = 1;
			TableD.HeaderD.nHeaderColCount = 7;
			TableD.HeaderD.aTitleName.Add(_T("楼层"));
			TableD.HeaderD.aTitleName.Add(_T("层高度"));
			TableD.HeaderD.aTitleName.Add(_T("节点"));
			TableD.HeaderD.aTitleName.Add(_T("层间位移(mm)"));
			TableD.HeaderD.aTitleName.Add(_T("层间位移角"));
			TableD.HeaderD.aTitleName.Add(_T("层间位移角限值"));
			TableD.HeaderD.aTitleName.Add(_T("结果"));
			TableD.RecordD.aRecordData.SetSize(arStorDrift.GetCount());

			nStorIndex = 0;
			for (j = arStorDrift.GetCount() - 1;j >= 0;j--)
			{
				BOOL bUp = arStor[j] >= (T_STOR_K)m_IsgbD.nBeginLayer;
				int nType = IIsolatorTool::Instance()->GetInterStoryDispAngleLimtTypeSplc(SplcK);
				double dLmt = IIsolatorTool::Instance()->GetInterStoryDispAngleLimtValue(nType, bUp) + 0.5;
				//Drift = arStorDrift[j];
				CStringArray& aStringDrift = TableD.RecordD.aRecordData[nStorIndex++];
				aStringDrift.Add(arStorDrift[j].strStorName);
				aStringDrift.Add(Double2String_f(arStorDrift[j].dHeight));
				StorD.Initialize();
				m_pDoc->m_pAttrCtrl->GetStor(m_IsgbD.nSeisLayer, StorD);
				if (arStorDrift[j].strStorName.Compare(StorD.StoryName) == 0)
				{
					aStringDrift.Add(_T("-"));
					aStringDrift.Add(_T("-"));
					aStringDrift.Add(_T("-"));
					aStringDrift.Add(_T("-"));
					aStringDrift.Add(_T("-"));
				}
				else
				{
					strText.Format(_T("%d"), arStorDrift[j].Max[2].NodeK);
					aStringDrift.Add(strText);
					aStringDrift.Add(Double2String_f(arStorDrift[j].Max[2].dDrift, 4));
					if (fabs(arStorDrift[j].dAllowableRatio) > 1.0E-7)
					{
						strText.Format(_T("1/%d"), int(1 / arStorDrift[j].Max[2].dRatio + 0.5));
						aStringDrift.Add(strText);
					}
					strText.Format(_T("1/%d"), (int)dLmt);
					aStringDrift.Add(strText);

					if (dLmt > 1 / arStorDrift[j].Max[2].dRatio)
						strText = _T("NG");
					else
						strText = _T("OK");
					aStringDrift.Add(strText);
				}
			}
			TableD.RecordD.aRecordData.FreeExtra();
			TableD.RecordD.nRecordRowCount = arStorDrift.GetCount();
			TableD.RecordD.nRecordColCount = 7;

			strTitle.Format(_T("层间位移角验算结果(设防地震：%s)"), SplcD.LoadCaseName);
			strBookTip.Format(_T("CHARPT_6_2_TBL%d"), ++nIndex);
			MakeTableCenterText(genElements, TableD, strTitle, strBookTip);
		}

	}
	
	//Table list 2
	if (nThis1 > 0 && !bBJGeneral)
	{
		strText.Format(_T("    根据《建筑工程减隔震技术规程》北京市地方标准 DB11/2075-2022第5.4.1条，对于%s建筑，该%s在设防地震作用下层间位移角验算结果如下："), strBldCategory, strStruct);
		for (i = 0; i < nThis1; i++)
		{
			ThisD.Initialize();
			ThisK = m_IsgbD.aResistThisCase[i];
			m_pDoc->m_pAttrCtrl->GetThis(ThisK, ThisD);
			T_TABLE_D TableD;
			TableD.Initialize();
			TableD.HeaderD.nHeaderRowCount = 1;
			TableD.HeaderD.nHeaderColCount = 7;
			TableD.HeaderD.aTitleName.Add(_T("楼层"));
			TableD.HeaderD.aTitleName.Add(_T("层高度"));
			TableD.HeaderD.aTitleName.Add(_T("节点"));
			TableD.HeaderD.aTitleName.Add(_T("层间位移(mm)"));
			TableD.HeaderD.aTitleName.Add(_T("层间位移角"));
			TableD.HeaderD.aTitleName.Add(_T("层间位移角限值"));
			TableD.HeaderD.aTitleName.Add(_T("结果"));
			m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_HISTORY, ThisK, LcomData);

			arStorDriftTH.RemoveAll();
			//for (int m = 0; m < aAllMinMax.GetSize(); m++)
			{
				nStorIndex = 0;
				nMinMax = 0;// aAllMinMax[m];
				if (!StDrft.GetDFT4TH(LcomData, nMinMax, arStorDriftTH)) continue;
				TableD.RecordD.aRecordData.SetSize(arStorDriftTH.GetCount());
				for (j = arStorDriftTH.GetCount() - 1; j >= 0; j--)
				{
					BOOL bUp = arStor[j] >= (T_STOR_K)m_IsgbD.nBeginLayer;
					int nType = IIsolatorTool::Instance()->GetInterStoryDispAngleLimtTypeThis(ThisK);
					int nLmt = IIsolatorTool::Instance()->GetInterStoryDispAngleLimtValue(nType, bUp);

					DriftTH = arStorDriftTH[j];
					CStringArray& aStringDrift = TableD.RecordD.aRecordData[nStorIndex++];
					aStringDrift.Add(DriftTH.strStorName);
					aStringDrift.Add(Double2String_f(DriftTH.dHeight));
					aStringDrift.Add(_T("-"));
					StorD.Initialize();
					m_pDoc->m_pAttrCtrl->GetStor(m_IsgbD.nSeisLayer, StorD);
					if (DriftTH.strStorName.Compare(StorD.StoryName) == 0)
					{
						aStringDrift.Add(_T("-"));
						aStringDrift.Add(_T("-"));
						aStringDrift.Add(_T("-"));
						aStringDrift.Add(_T("-"));
					}
					else
					{
						if (fabs(DriftTH.Axis[0].dRatio - DriftTH.Axis[1].dRatio) >= 0)
						{
							aStringDrift.Add(Double2String_f(fabs(DriftTH.Axis[0].dDrift), 4));
							if (fabs(DriftTH.dAllowableRatio) > 1.0E-7)
							{
								strText.Format(_T("1/%.0f"), 1. / fabs(DriftTH.Axis[0].dRatio));
								aStringDrift.Add(strText);
							}
							strText.Format(_T("1/%d"), nLmt);
							aStringDrift.Add(strText);
							strText = (DriftTH.Axis[0].nRemark == 0) ? _T("OK") : _T("NG");
							aStringDrift.Add(strText);
						}
						else
						{
							aStringDrift.Add(Double2String_f(fabs(DriftTH.Axis[1].dDrift), 4));
							if (fabs(DriftTH.dAllowableRatio) > 1.0E-7)
							{
								strText.Format(_T("1/%.0f"), 1. / fabs(DriftTH.Axis[1].dRatio));
								aStringDrift.Add(strText);
							}
							strText.Format(_T("1/%d"), nLmt);
							aStringDrift.Add(strText);
							strText = (DriftTH.Axis[1].nRemark == 0) ? _T("OK") : _T("NG");
							aStringDrift.Add(strText);
						}
					}
				}
			}

			TableD.RecordD.aRecordData.FreeExtra();
			TableD.RecordD.nRecordRowCount = arStorDriftTH.GetCount();
			TableD.RecordD.nRecordColCount = 7;

			strTitle.Format(_T("层间位移角验算结果(设防地震：%s)"), ThisD.LoadCaseName);
			strBookTip.Format(_T("CHARPT_6_2_TBL%d"), ++nIndex);
			MakeTableCenterText(genElements, TableD, strTitle, strBookTip);
		}
	}
	
	//Table list 3
	if (nSplc2 > 0 && !bBJGeneral)
	{
		if (m_IsgbD.nCode == 1)
		{
			if (m_IsgbD.nBldCategory != 2)
				strText.Format(_T("    根据《建筑工程减隔震技术规程》北京市地方标准 DB11/2075-2022第4.5.1条，对于%s建筑，在设防地震作用下层间位移角验算结果如下："), strStruct);
		}		
		else
			strText.Format(_T("    根据《建筑工程减隔震技术规程》北京市地方标准 DB11/2075-2022第5.4.1条，对于%s建筑，该%s在罕遇地震作用下层间位移角验算结果如下："), strBldCategory, strStruct);
	
		for (i = 0; i < nSplc2; i++)
		{
			arStorDrift.RemoveAll();
			SplcD.Initialize();
			SplcK = m_IsgbD.aRareESplcCase[i];
			m_pDoc->m_pAttrCtrl->GetSplc(SplcK, SplcD);
			if (SplcD.nDirection == 2)
				continue;
			m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_SPECTRUM, SplcK, LcomData);
			if (!StDrft.GetDFT(LcomData, 0, ActiveMethod, arStorDrift)) continue;
			int nType = IIsolatorTool::Instance()->GetInterStoryDispAngleLimtTypeSplc(SplcK);
			T_TABLE_D TableD;
			TableD.Initialize();
			TableD.HeaderD.nHeaderRowCount = 1;
			TableD.HeaderD.nHeaderColCount = 7;
			TableD.HeaderD.aTitleName.Add(_T("楼层"));
			TableD.HeaderD.aTitleName.Add(_T("层高度"));
			TableD.HeaderD.aTitleName.Add(_T("节点"));
			TableD.HeaderD.aTitleName.Add(_T("层间位移(mm)"));
			TableD.HeaderD.aTitleName.Add(_T("层间位移角"));
			TableD.HeaderD.aTitleName.Add(_T("层间位移角限值"));
			TableD.HeaderD.aTitleName.Add(_T("结果"));
			TableD.RecordD.aRecordData.SetSize(arStorDrift.GetCount());
			nStorIndex = 0;
			for (j = arStorDrift.GetCount() - 1; j >= 0; j--)
			{
				BOOL bUp = arStor[j] >= (T_STOR_K)m_IsgbD.nBeginLayer;
				int nLmt = IIsolatorTool::Instance()->GetInterStoryDispAngleLimtValue(nType, bUp);
				//Drift = arStorDrift[j];
				CStringArray& aStringDrift = TableD.RecordD.aRecordData[nStorIndex++];
				aStringDrift.Add(arStorDrift[j].strStorName);
				aStringDrift.Add(Double2String_f(arStorDrift[j].dHeight));
				StorD.Initialize();
				m_pDoc->m_pAttrCtrl->GetStor(m_IsgbD.nSeisLayer, StorD);
				if (arStorDrift[j].strStorName.Compare(StorD.StoryName) == 0)
				{
					aStringDrift.Add(_T("-"));
					aStringDrift.Add(_T("-"));
					aStringDrift.Add(_T("-"));
					aStringDrift.Add(_T("-"));
					aStringDrift.Add(_T("-"));
				}
				else
				{
					strText.Format(_T("%d"), arStorDrift[j].Max[2].NodeK);
					aStringDrift.Add(strText);
					aStringDrift.Add(Double2String_f(arStorDrift[j].Max[2].dDrift, 4));
					if (fabs(arStorDrift[j].dAllowableRatio) > 1.0E-7)
					{
						strText.Format(_T("1/%d"), int(1 / arStorDrift[j].Max[2].dRatio));
						aStringDrift.Add(strText);
					}
					strText.Format(_T("1/%d"), nLmt);
					aStringDrift.Add(strText);
					strText = (arStorDrift[j].Cen[2].nRemark == D_STOR_DFT_REMARK_OK) ? _T("OK") : _T("NG");
					aStringDrift.Add(strText);
				}
			}
			TableD.RecordD.aRecordData.FreeExtra();
			TableD.RecordD.nRecordRowCount = arStorDrift.GetCount();
			TableD.RecordD.nRecordColCount = 7;

			if (m_IsgbD.nCode == 1)
				strTitle.Format(_T("层间位移角验算结果(设防地震：%s)"), SplcD.LoadCaseName);
			else
				strTitle.Format(_T("层间位移角验算结果(罕遇地震：%s)"), SplcD.LoadCaseName);
			strBookTip.Format(_T("CHARPT_6_2_TBL%d"), ++nIndex);
			MakeTableCenterText(genElements, TableD, strTitle, strBookTip);
		}
	}
	

	//Table list 4
	if (nThis2 > 0 && !bBJGeneral)
	{
		if (m_IsgbD.nCode == 1)
		{
			if (m_IsgbD.nBldCategory != 2)
				strText.Format(_T("    根据《建筑工程减隔震技术规程》北京市地方标准 DB11/2075-2022第4.5.1条，对于%s建筑，在设防地震作用下层间位移角验算结果如下：："), strStruct);
		}	
		else
			strText.Format(_T("    根据《建筑工程减隔震技术规程》北京市地方标准 DB11/2075-2022第5.4.1条，对于%s建筑，该%s在罕遇地震作用下层间位移角验算结果如下："), strBldCategory, strStruct);
	
		for (i = 0; i < nThis2; i++)
		{
			ThisD.Initialize();
			ThisK = m_IsgbD.aRareEThisCase[i];
			m_pDoc->m_pAttrCtrl->GetThis(ThisK, ThisD);
			T_TABLE_D TableD;
			TableD.Initialize();
			TableD.HeaderD.nHeaderRowCount = 1;
			TableD.HeaderD.nHeaderColCount = 7;
			TableD.HeaderD.aTitleName.Add(_T("楼层"));
			TableD.HeaderD.aTitleName.Add(_T("层高度"));
			TableD.HeaderD.aTitleName.Add(_T("节点"));
			TableD.HeaderD.aTitleName.Add(_T("层间位移(mm)"));
			TableD.HeaderD.aTitleName.Add(_T("层间位移角"));
			TableD.HeaderD.aTitleName.Add(_T("层间位移角限值"));
			TableD.HeaderD.aTitleName.Add(_T("结果"));
			//int nCount = arStor.GetCount() * 1;
			//TableD.RecordD.aRecordData.SetSize(nCount);

			m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_HISTORY, ThisK, LcomData);
			//for (int m = 0; m < aAllMinMax.GetSize(); m++)
			{
				nStorIndex = 0;
				nMinMax = 0;// aAllMinMax[m];
				if (!StDrft.GetDFT4TH(LcomData, ThisK, arStorDriftTH)) continue;
				TableD.RecordD.aRecordData.SetSize(arStorDriftTH.GetCount());

				for (j = arStorDriftTH.GetCount() - 1; j >= 0; j--)
				{
					BOOL bUp = arStor[j] >= (T_STOR_K)m_IsgbD.nBeginLayer;
					int nType = IIsolatorTool::Instance()->GetInterStoryDispAngleLimtTypeThis(ThisK);
					int nLmt = IIsolatorTool::Instance()->GetInterStoryDispAngleLimtValue(nType, bUp);

					DriftTH = arStorDriftTH[j];
					CStringArray& aStringDrift = TableD.RecordD.aRecordData[nStorIndex++];
					aStringDrift.Add(DriftTH.strStorName);
					aStringDrift.Add(Double2String_f(DriftTH.dHeight));
					aStringDrift.Add(_T("-"));
					StorD.Initialize();
					m_pDoc->m_pAttrCtrl->GetStor(m_IsgbD.nSeisLayer, StorD);
					if (DriftTH.strStorName.Compare(StorD.StoryName) == 0)
					{
						aStringDrift.Add(_T("-"));
						aStringDrift.Add(_T("-"));
						aStringDrift.Add(_T("-"));
						aStringDrift.Add(_T("-"));
					}
					else
					{
						if (fabs(DriftTH.Axis[0].dRatio - DriftTH.Axis[1].dRatio) >= 0)
						{
							aStringDrift.Add(Double2String_f(fabs(DriftTH.Axis[0].dDrift), 4));
							if (fabs(DriftTH.dAllowableRatio) > 1.0E-7)
							{
								strText.Format(_T("1/%.0f"), 1. / fabs(DriftTH.Axis[0].dRatio));
								aStringDrift.Add(strText);
							}
							strText.Format(_T("1/%d"), nLmt);
							aStringDrift.Add(strText);
							strText = (DriftTH.Axis[0].nRemark == 0) ? _T("OK") : _T("NG");
							aStringDrift.Add(strText);
						}
						else
						{
							aStringDrift.Add(Double2String_f(fabs(DriftTH.Axis[1].dDrift), 4));
							if (fabs(DriftTH.dAllowableRatio) > 1.0E-7)
							{
								strText.Format(_T("1/%.0f"), 1. / fabs(DriftTH.Axis[1].dRatio));
								aStringDrift.Add(strText);
							}
							strText.Format(_T("1/%d"), nLmt);
							aStringDrift.Add(strText);
							strText = (DriftTH.Axis[1].nRemark == 0) ? _T("OK") : _T("NG");
							aStringDrift.Add(strText);
						}
					}

				}
			}

			TableD.RecordD.aRecordData.FreeExtra();
			TableD.RecordD.nRecordRowCount = arStorDriftTH.GetCount();
			TableD.RecordD.nRecordColCount = 7;

			if (m_IsgbD.nCode == 1)
				strTitle.Format(_T("层间位移角验算结果(设防地震：%s)"), ThisD.LoadCaseName);
			else
				strTitle.Format(_T("层间位移角验算结果(罕遇地震：%s)"), ThisD.LoadCaseName);
			strBookTip.Format(_T("CHARPT_6_2_TBL%d"), ++nIndex);
			MakeTableCenterText(genElements, TableD, strTitle, strBookTip);
		}
	}
	

	//Table list 5
	if (nSplc3 > 0)
	{
		for (i = 0; i < nSplc3; i++)
		{
			arStorDrift.RemoveAll();
			SplcD.Initialize();
			SplcK = m_IsgbD.aExtreRareESplcCase[i];
			m_pDoc->m_pAttrCtrl->GetSplc(SplcK, SplcD);
			if (SplcD.nDirection == 2)
				continue;
			T_TABLE_D TableD;
			TableD.Initialize();
			TableD.HeaderD.nHeaderRowCount = 1;
			TableD.HeaderD.nHeaderColCount = 7;
			TableD.HeaderD.aTitleName.Add(_T("楼层"));
			TableD.HeaderD.aTitleName.Add(_T("层高度"));
			TableD.HeaderD.aTitleName.Add(_T("节点"));
			TableD.HeaderD.aTitleName.Add(_T("层间位移(mm)"));
			TableD.HeaderD.aTitleName.Add(_T("层间位移角"));
			TableD.HeaderD.aTitleName.Add(_T("层间位移角限值"));
			TableD.HeaderD.aTitleName.Add(_T("结果"));

			m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_SPECTRUM, SplcK, LcomData);
			if (!StDrft.GetDFT(LcomData, 0, ActiveMethod, arStorDrift)) continue;
			int nType = IIsolatorTool::Instance()->GetInterStoryDispAngleLimtTypeSplc(SplcK);
			TableD.RecordD.aRecordData.SetSize(arStorDrift.GetCount());
			nStorIndex = 0;
			for (j = arStorDrift.GetCount() - 1; j >= 0; j--)
			{
				BOOL bUp = arStor[j] >= (T_STOR_K)m_IsgbD.nBeginLayer;

				double dLmt = IIsolatorTool::Instance()->GetInterStoryDispAngleLimtValue(nType, bUp) + 0.5;
				m_pDoc->m_pAttrCtrl->GetStor(arStor[j], StorD);
				//Drift = arStorDrift[j];
				CStringArray& aStringDrift = TableD.RecordD.aRecordData[nStorIndex++];
				aStringDrift.Add(arStorDrift[j].strStorName);
				aStringDrift.Add(Double2String_f(arStorDrift[j].dHeight));
				StorD.Initialize();
				m_pDoc->m_pAttrCtrl->GetStor(m_IsgbD.nSeisLayer, StorD);
				if (arStorDrift[j].strStorName.Compare(StorD.StoryName) == 0)
				{
					aStringDrift.Add(_T("-"));
					aStringDrift.Add(_T("-"));
					aStringDrift.Add(_T("-"));
					aStringDrift.Add(_T("-"));
					aStringDrift.Add(_T("-"));
				}
				else
				{
					strText.Format(_T("%d"), arStorDrift[j].Max[2].NodeK);
					aStringDrift.Add(strText);
					aStringDrift.Add(Double2String_f(arStorDrift[j].Max[2].dDrift, 4));
					if (fabs(arStorDrift[j].dAllowableRatio) > 1.0E-7)
					{
						strText.Format(_T("1/%d"), int(1 / arStorDrift[j].Max[2].dRatio));
						aStringDrift.Add(strText);
					}
					strText.Format(_T("1/%d"), (int)dLmt);
					aStringDrift.Add(strText);
					strText = (arStorDrift[j].Cen[2].nRemark == D_STOR_DFT_REMARK_OK) ? _T("OK") : _T("NG");
					aStringDrift.Add(strText);
				}
			}
			TableD.RecordD.aRecordData.FreeExtra();
			TableD.RecordD.nRecordRowCount = arStorDrift.GetCount();
			TableD.RecordD.nRecordColCount = 7;

			if (m_IsgbD.nCode == 0)
				strTitle.Format(_T("层间位移角验算结果(极罕遇地震：%s)"), SplcD.LoadCaseName);
			else
				strTitle.Format(_T("层间位移角验算结果(罕遇地震：%s)"), SplcD.LoadCaseName);

			strBookTip.Format(_T("CHARPT_6_2_TBL%d"), ++nIndex);
			MakeTableCenterText(genElements, TableD, strTitle, strBookTip);
		}
	}
	
	//Table list 6
	if (nThis3 > 0)
	{
		for (i = 0; i < nThis3; i++)
		{
			ThisD.Initialize();
			ThisK = m_IsgbD.aExtreRareEThisCase[i];
			m_pDoc->m_pAttrCtrl->GetThis(ThisK, ThisD);
			T_TABLE_D TableD;
			TableD.Initialize();
			TableD.HeaderD.nHeaderRowCount = 1;
			TableD.HeaderD.nHeaderColCount = 7;
			TableD.HeaderD.aTitleName.Add(_T("楼层"));
			TableD.HeaderD.aTitleName.Add(_T("层高度"));
			TableD.HeaderD.aTitleName.Add(_T("节点"));
			TableD.HeaderD.aTitleName.Add(_T("层间位移(mm)"));
			TableD.HeaderD.aTitleName.Add(_T("层间位移角"));
			TableD.HeaderD.aTitleName.Add(_T("层间位移角限值"));
			TableD.HeaderD.aTitleName.Add(_T("结果"));
			//int nCount = arStor.GetCount() * 1;
			//TableD.RecordD.aRecordData.SetSize(nCount);

			m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_HISTORY, ThisK, LcomData);

			//for (int m = 0; m < aAllMinMax.GetSize(); m++)
			{
				nStorIndex = 0;
				nMinMax = 0;// aAllMinMax[m];
				if (!StDrft.GetDFT4TH(LcomData, ThisK, arStorDriftTH)) continue;
				TableD.RecordD.aRecordData.SetSize(arStorDriftTH.GetCount());
				for (j = arStorDriftTH.GetCount() - 1; j >= 0; j--)
				{
					BOOL bUp = arStor[j] >= (T_STOR_K)m_IsgbD.nBeginLayer;
					int nType = IIsolatorTool::Instance()->GetInterStoryDispAngleLimtTypeThis(ThisK);
					double dLmt = IIsolatorTool::Instance()->GetInterStoryDispAngleLimtValue(nType, bUp) + 0.5;

					DriftTH = arStorDriftTH[j];
					CStringArray& aStringDrift = TableD.RecordD.aRecordData[nStorIndex++];
					aStringDrift.Add(DriftTH.strStorName);
					aStringDrift.Add(Double2String_f(DriftTH.dHeight));
					aStringDrift.Add(_T("-"));
					StorD.Initialize();
					m_pDoc->m_pAttrCtrl->GetStor(m_IsgbD.nSeisLayer, StorD);
					if (DriftTH.strStorName.Compare(StorD.StoryName) == 0)
					{
						aStringDrift.Add(_T("-"));
						aStringDrift.Add(_T("-"));
						aStringDrift.Add(_T("-"));
						aStringDrift.Add(_T("-"));
					}
					else
					{
						if (fabs(DriftTH.Axis[0].dRatio - DriftTH.Axis[1].dRatio) >= 0)
						{
							aStringDrift.Add(Double2String_f(fabs(DriftTH.Axis[0].dDrift), 4));
							if (fabs(DriftTH.dAllowableRatio) > 1.0E-7)
							{
								strText.Format(_T("1/%.0f"), 1. / fabs(DriftTH.Axis[0].dRatio));
								aStringDrift.Add(strText);
							}
							strText.Format(_T("1/%d"), (int)dLmt);
							aStringDrift.Add(strText);
							strText = (DriftTH.Axis[0].nRemark == 0) ? _T("OK") : _T("NG");
							aStringDrift.Add(strText);
						}
						else
						{
							aStringDrift.Add(Double2String_f(fabs(DriftTH.Axis[1].dDrift), 4));
							if (fabs(DriftTH.dAllowableRatio) > 1.0E-7)
							{
								strText.Format(_T("1/%.0f"), 1. / fabs(DriftTH.Axis[1].dRatio));
								aStringDrift.Add(strText);
							}
							strText.Format(_T("1/%d"), (int)dLmt);
							TableD.RecordD.aRecordData[j].Add(strText);
							strText = (DriftTH.Axis[1].nRemark == 0) ? _T("OK") : _T("NG");
							aStringDrift.Add(strText);
						}
					}

				}
			}

			TableD.RecordD.aRecordData.FreeExtra();
			TableD.RecordD.nRecordRowCount = arStorDriftTH.GetCount();
			TableD.RecordD.nRecordColCount = 7;

			if (m_IsgbD.nCode == 0)
				strTitle.Format(_T("层间位移角验算结果(极罕遇地震：%s)"), ThisD.LoadCaseName);
			else
				strTitle.Format(_T("层间位移角验算结果(罕遇地震：%s)"), ThisD.LoadCaseName);
			
			strBookTip.Format(_T("CHARPT_6_2_TBL%d"), ++nIndex);
			MakeTableCenterText(genElements, TableD, strTitle, strBookTip);
		}
	}
	
}
void AutoReport_Gen_CH_ISGB::WriteCharpt6_3(GenSegmentElements &genElements)
{
	CString strText;
	strText = _T("隔震结构的倾覆弯矩验算");
	MakeTitle(genElements, strText, 6, 3);
	if (!m_pDoc->IsPostMode())
		return;
	if (m_IsgbD.nCode == 0)
		strText = _T("    根据《建筑隔震设计标准》GB/T 51408-2021第4.6.9条，隔震结构的倾覆弯矩验算结果如下：");
	else
		strText = _T("    根据《建筑工程减隔震技术规程》北京市地方标准 DB11/2075-2022第11.2.8条，隔震结构的倾覆弯矩验算结果如下：");
	MakeText(genElements, strText);

	CAutoRecoveryUnit unitSet(D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_FORCE_INDEX_KN, m_pDoc->m_pUnitCtrl, TRUE);

	T_TABLE_D TableD;
	TableD.Initialize();
	TableD.HeaderD.nHeaderRowCount = 1;
	TableD.HeaderD.nHeaderColCount = 5;
	TableD.HeaderD.aTitleName.Add(_T("荷载工况"));
	TableD.HeaderD.aTitleName.Add(_T("抗倾覆弯矩ROTM"));
	TableD.HeaderD.aTitleName.Add(_T("倾覆弯矩OTM"));
	TableD.HeaderD.aTitleName.Add(_T("比值ROTM/OTM"));
	TableD.HeaderD.aTitleName.Add(_T("结果"));

	int nCount = m_IsgbD.aRareESplcCase.GetCount();
	TableD.RecordD.aRecordData.SetSize(nCount);
	T_SPLC_D SplcD;
	int nIndex = 0;
	double dROTM=0.0, dOTM=0.0, dRto=0.0;
	double dLmtRto = m_IsgbD.nCode == 1 ? 1.2 : 1.1;
	for (int i=0; i<nCount; i++)
	{
		SplcD.Initialize();
		m_pDoc->m_pAttrCtrl->GetSplc(m_IsgbD.aRareESplcCase[i], SplcD);
		//if (SplcD.nDirection == 1)
		//{
			dRto = IIsolatorTool::Instance()->ChkGetAntOverturningCalc(m_IsgbD.aRareESplcCase[i], dROTM, dOTM);

			TableD.RecordD.aRecordData[i].Add(SplcD.LoadCaseName);
			TableD.RecordD.aRecordData[i].Add(Double2String_f(dROTM));
			TableD.RecordD.aRecordData[i].Add(Double2String_f(dOTM));
			TableD.RecordD.aRecordData[i].Add(Double2String_f(dRto));
			strText = (dRto > dLmtRto) ? _T("OK") : _T("NG");
			TableD.RecordD.aRecordData[i].Add(strText);
			nIndex++;
		//}
		//else
		//	continue;
	}
	TableD.RecordD.aRecordData.FreeExtra();
	TableD.RecordD.nRecordRowCount = nIndex;
	TableD.RecordD.nRecordColCount = 5;

	MakeTableCenterText(genElements, TableD, _T("倾覆弯矩验算结果"), _T("CHARPT_6_3_TBL"));
}

void AutoReport_Gen_CH_ISGB::WriteCharpt7()
{
	GenSegmentElements genElements;
	MakeTitle(genElements, _T("隔震层和隔震支座的验算"), 7, 0);
	WriteCharpt7_1(genElements);
	WriteCharpt7_2(genElements);
	WriteCharpt7_3(genElements);
	m_Segments.push_back(genElements);
}
void AutoReport_Gen_CH_ISGB::WriteCharpt7_1(GenSegmentElements &genElements)
{
	CString strText, strTemp;
	strText = _T("隔震层的等效刚度和等效阻尼比验算");
	MakeTitle(genElements, strText, 7, 1);
	if (!m_pDoc->IsPostMode())
		return;
	strText = _T("    根据《建筑隔震设计标准》GB/T 51408-2021第4.6.4条，隔震层的水平等效刚度和等效阻尼比计算公式如下：");
	MakeText(genElements, strText);
	strTemp = _LS(IDS_TM_ISGB_AUTO_WORD_Charpt7_1_1);
	strText.Format(_T("[DRG_FONT]|FC| size=14 align=center [/DRG_FONT]%s\n[DRG_FONT]|FI|[/DRG_FONT]"), strTemp);
	MakeText(genElements, strText);
	strTemp = _LS(IDS_TM_ISGB_AUTO_WORD_Charpt7_1_2);
	strText.Format(_T("[DRG_FONT]|FC| size=14 align=center [/DRG_FONT]%s\n[DRG_FONT]|FI|[/DRG_FONT]"), strTemp);
	MakeText(genElements, strText);
	CString strStor = m_pDoc->m_pAttrCtrl->GetStorNameWithModuleName(m_IsgbD.nSeisLayer);
	strText.Format(_T("    对于隔震层（%s）,各隔震支座的有效刚度和有效阻尼比如下表所示："), strStor);
	MakeText(genElements, strText);

	//Figure
	{
		SetViewPoint(3);
		CString strName;
		strName = _T("图7.1 隔震支座布置图");
		T_KEY_LIST aNode, aElem;
		m_pDoc->m_pAttrCtrl->GetNodeKeyList(aNode);
		m_pDoc->m_pAttrCtrl->GetElemKeyList(aElem);
		ActiveObj(aElem,TRUE,&aNode);
		I_GENModelBase	*gm = I_GENModelBase::GetCurMySelfST();
		gm->GetDispSetting()->m_Bndr.bNonlinearLinkNo = TRUE;
		MakeImage_Current(strName, FALSE);
		MakePicture(genElements, strName, _T("CHARPT_7_1_FIG"));
		gm->GetDispSetting()->m_Bndr.bNonlinearLinkNo = FALSE;
		SetShape(FALSE);
	}
	//Table
	{
		CAutoRecoveryUnit unitSet(D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_FORCE_INDEX_KN, m_pDoc->m_pUnitCtrl, TRUE);
		CArray<T_NLNK_K, T_NLNK_K> aNlnK;
		CDBDoc::GetDocPoint()->m_pAttrCtrl->GetKeyListStorUserNlnk(m_IsgbD.nSeisLayer, 1, &aNlnK);
		if (aNlnK.IsEmpty())
			return;
		CString strProp[] = { _T("天然橡胶支座"),_T("铅芯橡胶支座"), _T("高阻尼橡胶支座"), _T("弹性滑板支座"), _T("摩擦摆支座") };
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 5;
		TableD.HeaderD.aTitleName.Add(_T("支座编号"));
		TableD.HeaderD.aTitleName.Add(_T("支座名称"));
		TableD.HeaderD.aTitleName.Add(_T("支座类型"));
		TableD.HeaderD.aTitleName.Add(_T("水平刚度(kN/mm)"));
		TableD.HeaderD.aTitleName.Add(_T("阻尼比(%)"));
		//  
		TableD.RecordD.aRecordData.SetSize(aNlnK.GetSize());
		auto& record = TableD.RecordD.aRecordData;
		T_NLNK_D NlnkD;
		T_NLLP_D NllpD;
		T_ASGB_D AsgbD;
		double dStiff = 0.0, dRto = 0.0;
		double dStiffSum = 0.0, dRotSum = 0.0;
		for (int i = 0; i < aNlnK.GetSize(); i++)
		{
			NlnkD.Initialize();
			m_pDoc->m_pAttrCtrl->GetNlnk(aNlnK[i], NlnkD);
			NllpD.Initialize();
			m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
			AsgbD.Initialize();
			if (NllpD.nApplicationType == 2 && NllpD.nSeisType == 5)
				m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);
			else
				continue;
			m_strFormat.Format(_T("%d"), aNlnK[i]);
			record[i].Add(m_strFormat);
			record[i].Add(AsgbD.PropName);
			record[i].Add(strProp[AsgbD.nPropType]);
			dStiff = (NllpD.LDP_dEffStiff[1] + NllpD.LDP_dEffStiff[2]) / 2.0;
			dRto = CalcDampingRto(aNlnK[i], NllpD.LDP_dEffDamping[1], NllpD.LDP_dEffDamping[2]);
			record[i].Add(Double2String_f(NllpD.LDP_dEffStiff[1], 0));
			record[i].Add(Double2String_f(dRto*100));
			dStiffSum += dStiff;
			dRotSum += dStiff * dRto;
		}
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = aNlnK.GetSize();
		TableD.RecordD.nRecordColCount = 5;
		MakeTableCenterText(genElements, TableD, _T("隔震支座"), _T("CHARPT_7_1_TBL1"));

		strText.Format(_LS(IDS_TM_ISGB_AUTO_WORD_Charpt7_1_3), dStiffSum);
		MakeText(genElements, strText);
		strText.Format(_LS(IDS_TM_ISGB_AUTO_WORD_Charpt7_1_4), dStiffSum < 1.0e-19 ? -1 : dRotSum * 100 /dStiffSum);
		MakeText(genElements, strText);
	}
}
void AutoReport_Gen_CH_ISGB::WriteCharpt7_2(GenSegmentElements &genElements)
{
	CString strText;
	strText = _T("隔震层验算");
	MakeTitle(genElements, strText, 7, 2);
	if (!m_pDoc->IsPostMode())
		return;
	WriteCharpt7_2_1(genElements);
	WriteCharpt7_2_2(genElements);
	WriteCharpt7_2_3(genElements);
}
void AutoReport_Gen_CH_ISGB::WriteCharpt7_2_1(GenSegmentElements &genElements)
{
	CString strText, strTemp;
	strText = _T("隔震层抗风承载力验算");
	MakeTitle(genElements, strText, 7, 2,_T(""),1);
	if (m_IsgbD.nCode == 0)
		strText = _T("    隔震层必须具备足够的屈服前刚度和屈服承载力，以满足风荷载和微振动的要求。根据《建筑隔震设计标准》GB/T 51408-2021第4.6.8条");
	else
		strText = _T("    隔震层必须具备足够的屈服前刚度和屈服承载力，以满足风荷载和微振动的要求。根据《建筑工程减隔震技术规程》北京市地方标准 DB11/2075-2022第11.2.7条：");
	MakeText(genElements, strText);
	strTemp = _LS(IDS_TM_ISGB_AUTO_WORD_Charpt7_2_1);
	strText.Format(_T("[DRG_FONT]|FC| size=14 align=center [/DRG_FONT]%s\n[DRG_FONT]|FI|[/DRG_FONT]"), strTemp);
	MakeText(genElements, strText);
	strText = _T("    式中：VRw-抗风装置的水平承载力设计值。当不单独设抗风装置时，取隔震支座的屈服荷载设计值：");
	MakeText(genElements, strText);
	strText = _T("    rw-风荷载分项系数，取1.5；");
	MakeText(genElements, strText);
	strText = _T("    Vwk-风荷载作用下隔震层的水平剪力标准值。");
	MakeText(genElements, strText);
	//
	{
		CAutoRecoveryUnit unitSet(D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_FORCE_INDEX_KN, m_pDoc->m_pUnitCtrl, TRUE);
		CArray<T_STLD_K, T_STLD_K> aStld, aStld_New;
		T_STLD_K StldKey;
		T_STLD_D StldData;
		m_pDoc->m_pAttrCtrl->GetStldKeyList(aStld);
		for (int i = 0; i < aStld.GetSize(); i++)
		{
			StldKey = aStld.GetAt(i);
			StldData.Initialize();
			m_pDoc->m_pAttrCtrl->GetStld(StldKey, StldData);
			if (StldData.LoadCaseType == _T("W"))
			{
				aStld_New.Add(aStld[i]);
			}
		}

		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 5;
		TableD.HeaderD.aTitleName.Add(_T("工况"));
		TableD.HeaderD.aTitleName.Add(_T("VRw(kN)"));
		TableD.HeaderD.aTitleName.Add(_T("Vwk(kN)"));
		TableD.HeaderD.aTitleName.Add(_T("(γwVwk)/ Vrw"));
		TableD.HeaderD.aTitleName.Add(_T("结果"));
		//  
		TableD.RecordD.aRecordData.SetSize(aStld_New.GetCount());
		auto& record = TableD.RecordD.aRecordData;
		double dV_Rw = 0.0, dV_wk = 0.0, dRto = 0.0;
		for (int i=0; i<aStld_New.GetCount(); i++)
		{
			StldData.Initialize();
			m_pDoc->m_pAttrCtrl->GetStld(aStld_New[i], StldData);
			dRto = IIsolatorTool::Instance()->ChkWindResistanceBearingCalc(m_IsgbD.nSeisLayer, aStld_New[i], dV_Rw, dV_wk);
			record[i].Add(StldData.LoadCaseName);
			record[i].Add(Double2String_f(dV_Rw));
			record[i].Add(Double2String_f(dV_wk));
			record[i].Add(Double2String_f(dRto));
			strText = (dRto > 1.0) ? _T("NG") : _T("OK");
			record[i].Add(strText);
		}

		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = aStld_New.GetSize();
		TableD.RecordD.nRecordColCount = 5;
		strText.Format(_T("隔震层%d抗风承载力验算"), m_IsgbD.nSeisLayer);
		MakeTableCenterText(genElements, TableD, strText, _T("CHARPT_7_2_1_TBL"));
	}
	strText = _T("注: Vwk: 风荷载作用下隔震层水平剪力标准值；");
	MakeText(genElements, strText);
	strText = _T("    Vrw: 隔震层抗风承载力设计值。");
	MakeText(genElements, strText);
}
void AutoReport_Gen_CH_ISGB::WriteCharpt7_2_2(GenSegmentElements &genElements)
{
	CString strText, strBook;
	strText = _T("隔震支座应力验算");
	MakeTitle(genElements, strText, 7, 2, _T(""), 2);

	T_NLNK_D NlnkD;
	T_NLLP_D NllpD;
	T_ASGB_D AsgbD;
	CString strProp[] = { _T("天然橡胶支座(LNR)"),_T("铅芯橡胶支座(LRB)"), _T("高阻尼橡胶支座(HDR)"), _T("弹性滑板支座(ESB)"), _T("摩擦摆支座(FPS)") };
	int nIndex;

	//重力荷载代表值
	if (m_IsgbD.nCode == 0)
		strText = _T("    根据《建筑隔震设计标准》GB/T 51408-2021第4.6.3条，在重力荷载代表值下.竖向压应力设计值不应超过规范表格4.6.3。隔震支座在重力荷载代表之下的压应力验算结果如下：");
	else
		strText = _T("    根据《建筑工程减隔震技术规程》北京市地方标准 DB11/2075-2022第11.2.4条，在重力荷载代表值下.竖向压应力设计值不应超过规范表格11.2.4-1、11.2.4-2。隔震支座在重力荷载代表之下的压应力验算结果如下:");
	MakeText(genElements, strText);	
	{
		CArray<T_NLNK_K, T_NLNK_K> aNlnK, aNlnK2;
		CDBDoc::GetDocPoint()->m_pAttrCtrl->GetKeyListStorUserNlnk(m_IsgbD.nSeisLayer, 1, &aNlnK);

		for (int i = 0; i < aNlnK.GetSize(); i++)
		{
			NlnkD.Initialize();
			m_pDoc->m_pAttrCtrl->GetNlnk(aNlnK[i], NlnkD);
			NllpD.Initialize();
			m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
			AsgbD.Initialize();
			if (NllpD.nApplicationType == 2 && NllpD.nSeisType == 5)
				aNlnK2.Add(aNlnK[i]);
		}
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 6;
		TableD.HeaderD.aTitleName.Add(_T("支座编号"));
		TableD.HeaderD.aTitleName.Add(_T("支座名称"));
		TableD.HeaderD.aTitleName.Add(_T("支座类型"));
		TableD.HeaderD.aTitleName.Add(_T("重力荷载代表值下的压应力(MPa)"));
		TableD.HeaderD.aTitleName.Add(_T("应力限值(MPa)"));
		TableD.HeaderD.aTitleName.Add(_T("验算结果"));
		//  
		TableD.RecordD.aRecordData.SetSize(aNlnK2.GetSize());
		auto& record = TableD.RecordD.aRecordData;
		double dStiff = 0.0, dRto = 0.0;
		double dStiffSum = 0.0, dRotSum = 0.0;
		T_ISO_RATIO IsoRatio;
		nIndex = 0;
		for (int i = 0; i < aNlnK2.GetSize(); i++)
		{
			NlnkD.Initialize();
			m_pDoc->m_pAttrCtrl->GetNlnk(aNlnK2[i], NlnkD);
			NllpD.Initialize();
			m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
			AsgbD.Initialize();
			m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);

			if (!IIsolatorTool::Instance()->GetNlnkIsolationData(aNlnK[i], IsoRatio, 0, m_IsgbD.nLcomG, D_LOADCASE_COMB_GENERAL, LOADTYPE_NORMAL))
			{
				ASSERT(0);
				continue;
			}
			m_strFormat.Format(_T("%d"), IsoRatio.nlnkK);
			record[i].Add(m_strFormat);
			record[i].Add(AsgbD.PropName);
			record[i].Add(strProp[AsgbD.nPropType]);
			record[i].Add(Double2String_f(IsoRatio.dVal));
			record[i].Add(Double2String_f(IsoRatio.dLimit));
			strText = (IsoRatio.dRatio > 1.0) ? _T("NG") : _T("OK");
			record[i].Add(strText);
			nIndex++;
		}

		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = nIndex;
		TableD.RecordD.nRecordColCount = 6;
		MakeTableCenterText(genElements, TableD, _T("隔震支座压应力验算(重力荷载代表值下的验算)"), _T("CHARPT_7_2_2_1_TBL"));
	}

	//罕遇地震
	CArray<T_ISO_RATIO, T_ISO_RATIO&> aIsoRatio;
	CArray<T_SPLC_K, T_SPLC_K> arSplcH, arSplcV;
	T_SPLC_D SplcD, SplcD_V;
	T_SPLC_K SplcK;
	T_THIS_D ThisD;
	T_THIS_K ThisK;
	for (int i=0; i<m_IsgbD.aRareESplcCase.GetCount(); i++)
	{
		SplcD.Initialize();
		SplcK = m_IsgbD.aRareESplcCase[i];
		m_pDoc->m_pAttrCtrl->GetSplc(SplcK, SplcD);
		if (SplcD.nDirection == 1)
			arSplcH.Add(SplcK);
		else
			arSplcV.Add(SplcK);
	}
	
	int nSum = 2;
	//罕遇地震-压应力
	if (m_IsgbD.nCode == 0)
		strText = _T("    根据《建筑隔震设计标准》GB/T 51408-2021第6.2.1条，在罕遇地震作用下.竖向压应力设计值不应超过规范表格6.2.1。隔震支座在罕遇地震作用下的压应力验算结果如下：");
	else
		strText = _T("    根据《建筑工程减隔震技术规程》北京市地方标准 DB11/2075-2022第11.2.4条，在罕遇地震作用下，竖向压应力设计值不应超过规范表格11.2.4-3。隔震支座在罕遇地震作用下的压应力验算结果如下:");
	MakeText(genElements, strText);
	{
		if (arSplcV.GetCount() <1 )
		{
			for (int i=0; i<arSplcH.GetCount(); i++)
			{
				aIsoRatio.RemoveAll();
				if (!IIsolatorTool::Instance()->GetNlnkIsolationData(aIsoRatio, 1, arSplcH[i], D_LOADCASE_SPECTRUM, LOADTYPE_MAX))
				{
					continue;
				}
				SplcD.Initialize();
				m_pDoc->m_pAttrCtrl->GetSplc(arSplcH[i], SplcD);
				nIndex = 0;
				T_TABLE_D TableD;
				TableD.Initialize();
				TableD.HeaderD.nHeaderRowCount = 1;
				TableD.HeaderD.nHeaderColCount = 6;
				TableD.HeaderD.aTitleName.Add(_T("支座编号"));
				TableD.HeaderD.aTitleName.Add(_T("支座名称"));
				TableD.HeaderD.aTitleName.Add(_T("支座类型"));
				TableD.HeaderD.aTitleName.Add(_T("罕遇地震下的压应力(MPa)"));
				TableD.HeaderD.aTitleName.Add(_T("应力限值(MPa)"));
				TableD.HeaderD.aTitleName.Add(_T("验算结果"));
				//  
				TableD.RecordD.aRecordData.SetSize(aIsoRatio.GetSize());
				auto& record = TableD.RecordD.aRecordData;
				for (int i = 0; i < aIsoRatio.GetSize(); i++)
				{
					NlnkD.Initialize();
					m_pDoc->m_pAttrCtrl->GetNlnk(aIsoRatio[i].nlnkK, NlnkD);
					NllpD.Initialize();
					m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
					AsgbD.Initialize();
					m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);

					m_strFormat.Format(_T("%d"), aIsoRatio[i].nlnkK);
					record[i].Add(m_strFormat);
					record[i].Add(AsgbD.PropName);
					record[i].Add(strProp[AsgbD.nPropType]);
					record[i].Add(Double2String_f(aIsoRatio[i].dVal));
					record[i].Add(Double2String_f(aIsoRatio[i].dLimit));
					strText = (aIsoRatio[i].dRatio > 1.0) ? _T("NG") : _T("OK");
					record[i].Add(strText);
					nIndex++;
				}

				TableD.RecordD.aRecordData.FreeExtra();
				TableD.RecordD.nRecordRowCount = nIndex;
				TableD.RecordD.nRecordColCount = 6;
				strText.Format(_T("隔震支座压应力验算(罕遇作用下的验算)工况%s"), SplcD.LoadCaseName);
				strBook.Format(_T("CHARPT_7_2_2_%d_TBL"), nSum);
				MakeTableCenterText(genElements, TableD, strText, strBook);
				nSum++;
			}
		}
		else
		{
			for (int k=0; k<arSplcV.GetCount(); k++)
			{
				for (int i = 0; i < arSplcH.GetCount(); i++)
				{
					aIsoRatio.RemoveAll();
					if (!IIsolatorTool::Instance()->GetNlnkIsolationData(aIsoRatio, 1, arSplcH[i], D_LOADCASE_SPECTRUM, LOADTYPE_MAX, arSplcV[k],D_LOADCASE_SPECTRUM, LOADTYPE_MAX))
					{
						ASSERT(0); 
					}
					SplcD.Initialize();
					SplcD_V.Initialize();
					m_pDoc->m_pAttrCtrl->GetSplc(arSplcH[i], SplcD);
					m_pDoc->m_pAttrCtrl->GetSplc(arSplcV[k], SplcD_V);
					nIndex = 0;
					T_TABLE_D TableD;
					TableD.Initialize();
					TableD.HeaderD.nHeaderRowCount = 1;
					TableD.HeaderD.nHeaderColCount = 6;
					TableD.HeaderD.aTitleName.Add(_T("支座编号"));
					TableD.HeaderD.aTitleName.Add(_T("支座名称"));
					TableD.HeaderD.aTitleName.Add(_T("支座类型"));
					TableD.HeaderD.aTitleName.Add(_T("罕遇地震下的压应力(MPa)"));
					TableD.HeaderD.aTitleName.Add(_T("应力限值(MPa)"));
					TableD.HeaderD.aTitleName.Add(_T("验算结果"));
					//  
					TableD.RecordD.aRecordData.SetSize(aIsoRatio.GetSize());
					auto& record = TableD.RecordD.aRecordData;
					for (int i = 0; i < aIsoRatio.GetSize(); i++)
					{
						NlnkD.Initialize();
						m_pDoc->m_pAttrCtrl->GetNlnk(aIsoRatio[i].nlnkK, NlnkD);
						NllpD.Initialize();
						m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
						AsgbD.Initialize();
						m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);

						m_strFormat.Format(_T("%d"), aIsoRatio[i].nlnkK);
						record[i].Add(m_strFormat);
						record[i].Add(AsgbD.PropName);
						record[i].Add(strProp[AsgbD.nPropType]);
						record[i].Add(Double2String_f(aIsoRatio[i].dVal));
						record[i].Add(Double2String_f(aIsoRatio[i].dLimit));
						strText = (aIsoRatio[i].dRatio > 1.0) ? _T("NG") : _T("OK");
						record[i].Add(strText);
						nIndex++;
					}

					TableD.RecordD.aRecordData.FreeExtra();
					TableD.RecordD.nRecordRowCount = nIndex;
					TableD.RecordD.nRecordColCount = 6;
					strText.Format(_T("隔震支座压应力验算(罕遇作用下的验算)工况%s+%s"), SplcD.LoadCaseName,SplcD_V.LoadCaseName);
					strBook.Format(_T("CHARPT_7_2_2_%d_TBL"), nSum);
					MakeTableCenterText(genElements, TableD, strText, strBook);
					nSum++;
				}
			}
		}

		for (int i = 0; i < m_IsgbD.aRareEThisCase.GetCount(); i++)
		{
			aIsoRatio.RemoveAll();
			ThisK = m_IsgbD.aRareEThisCase[i];
			if (!IIsolatorTool::Instance()->GetNlnkIsolationData(aIsoRatio, 1, ThisK, D_LOADCASE_HISTORY, LOADTYPE_MIN))
			{
				continue;
			}
			ThisD.Initialize();
			m_pDoc->m_pAttrCtrl->GetThis(ThisK, ThisD);
			nIndex = 0;
			T_TABLE_D TableD;
			TableD.Initialize();
			TableD.HeaderD.nHeaderRowCount = 1;
			TableD.HeaderD.nHeaderColCount = 6;
			TableD.HeaderD.aTitleName.Add(_T("支座编号"));
			TableD.HeaderD.aTitleName.Add(_T("支座名称"));
			TableD.HeaderD.aTitleName.Add(_T("支座类型"));
			TableD.HeaderD.aTitleName.Add(_T("罕遇地震下的压应力(MPa)"));
			TableD.HeaderD.aTitleName.Add(_T("应力限值(MPa)"));
			TableD.HeaderD.aTitleName.Add(_T("验算结果"));
			//  
			TableD.RecordD.aRecordData.SetSize(aIsoRatio.GetSize());
			auto& record = TableD.RecordD.aRecordData;
			for (int i = 0; i < aIsoRatio.GetSize(); i++)
			{
				NlnkD.Initialize();
				m_pDoc->m_pAttrCtrl->GetNlnk(aIsoRatio[i].nlnkK, NlnkD);
				NllpD.Initialize();
				m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
				AsgbD.Initialize();
				m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);

				m_strFormat.Format(_T("%d"), aIsoRatio[i].nlnkK);
				record[i].Add(m_strFormat);
				record[i].Add(AsgbD.PropName);
				record[i].Add(strProp[AsgbD.nPropType]);
				record[i].Add(Double2String_f(aIsoRatio[i].dVal));
				record[i].Add(Double2String_f(aIsoRatio[i].dLimit));
				strText = (aIsoRatio[i].dRatio > 1.0) ? _T("NG") : _T("OK");
				record[i].Add(strText);
				nIndex++;
			}

			TableD.RecordD.aRecordData.FreeExtra();
			TableD.RecordD.nRecordRowCount = nIndex;
			TableD.RecordD.nRecordColCount = 6;
			strText.Format(_T("隔震支座压应力验算(罕遇作用下的验算)工况%s"), ThisD.LoadCaseName);
			strBook.Format(_T("CHARPT_7_2_2_%d_TBL"), nSum);
			MakeTableCenterText(genElements, TableD, strText, strBook);
			nSum++;
		}
	}

	//罕遇地震-拉应力
	if (m_IsgbD.nCode == 0)
		strText = _T("    根据《建筑隔震设计标准》GB/T 51408-2021第6.2.1条，在罕遇地震作用下.竖向拉应力设计值不应超过规范表格6.2.1。隔震支座在罕遇地震作用下的拉应力验算结果如下：");
	else
		strText = _T("    根据《建筑工程减隔震技术规程》北京市地方标准 DB11/2075-2022第11.2.1-2条，在罕遇地震作用下，橡胶隔震支座的拉应力不应大于1MPa，弹性滑板支座、摩擦摆支座或其它不能承受竖向拉力的支座，宜保持受压状态:");
	MakeText(genElements, strText);
	{
		if (arSplcV.GetCount() < 1)
		{
			for (int i = 0; i < arSplcH.GetCount(); i++)
			{
				aIsoRatio.RemoveAll();
				if (!IIsolatorTool::Instance()->GetNlnkIsolationData(aIsoRatio, 2, arSplcH[i], D_LOADCASE_SPECTRUM, LOADTYPE_MAX))
				{
					continue;
				}
				SplcD.Initialize();
				m_pDoc->m_pAttrCtrl->GetSplc(arSplcH[i], SplcD);
				nIndex = 0;
				T_TABLE_D TableD;
				TableD.Initialize();
				TableD.HeaderD.nHeaderRowCount = 1;
				TableD.HeaderD.nHeaderColCount = 6;
				TableD.HeaderD.aTitleName.Add(_T("支座编号"));
				TableD.HeaderD.aTitleName.Add(_T("支座名称"));
				TableD.HeaderD.aTitleName.Add(_T("支座类型"));
				TableD.HeaderD.aTitleName.Add(_T("罕遇地震下的拉应力(MPa)"));
				TableD.HeaderD.aTitleName.Add(_T("应力限值(MPa)"));
				TableD.HeaderD.aTitleName.Add(_T("验算结果"));
				//  
				TableD.RecordD.aRecordData.SetSize(aIsoRatio.GetSize());
				auto& record = TableD.RecordD.aRecordData;
				for (int i = 0; i < aIsoRatio.GetSize(); i++)
				{
					NlnkD.Initialize();
					m_pDoc->m_pAttrCtrl->GetNlnk(aIsoRatio[i].nlnkK, NlnkD);
					NllpD.Initialize();
					m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
					AsgbD.Initialize();
					m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);

					m_strFormat.Format(_T("%d"), aIsoRatio[i].nlnkK);
					record[i].Add(m_strFormat);
					record[i].Add(AsgbD.PropName);
					record[i].Add(strProp[AsgbD.nPropType]);
					record[i].Add(Double2String_f(aIsoRatio[i].dVal));
					record[i].Add(Double2String_f(aIsoRatio[i].dLimit));
					if (aIsoRatio[i].dRatio < 999998)
					{
						strText = (aIsoRatio[i].dRatio > 1.0) ? _T("NG") : _T("OK");
					}
					else if(aIsoRatio[i].dVal < 1.0e-11)
						strText = _T("OK");
					else
						strText = _T("NG");
					record[i].Add(strText);
					nIndex++;
				}

				TableD.RecordD.aRecordData.FreeExtra();
				TableD.RecordD.nRecordRowCount = nIndex;
				TableD.RecordD.nRecordColCount = 6;
				strText.Format(_T("隔震支座拉应力验算(罕遇作用下的验算)工况%s"), SplcD.LoadCaseName);
				strBook.Format(_T("CHARPT_7_2_2_%d_TBL"), nSum);
				MakeTableCenterText(genElements, TableD, strText, strBook);
				nSum++;
			}
		}
		else
		{
			for (int k = 0; k < arSplcV.GetCount(); k++)
			{
				for (int i = 0; i < arSplcH.GetCount(); i++)
				{
					aIsoRatio.RemoveAll();
					if (!IIsolatorTool::Instance()->GetNlnkIsolationData(aIsoRatio, 2, arSplcH[i], D_LOADCASE_SPECTRUM, LOADTYPE_MAX, arSplcV[k], D_LOADCASE_SPECTRUM, LOADTYPE_MAX))
					{
						continue;
					}
					SplcD.Initialize();
					SplcD_V.Initialize();
					m_pDoc->m_pAttrCtrl->GetSplc(arSplcH[i], SplcD);
					m_pDoc->m_pAttrCtrl->GetSplc(arSplcV[k], SplcD_V);
					nIndex = 0;
					T_TABLE_D TableD;
					TableD.Initialize();
					TableD.HeaderD.nHeaderRowCount = 1;
					TableD.HeaderD.nHeaderColCount = 6;
					TableD.HeaderD.aTitleName.Add(_T("支座编号"));
					TableD.HeaderD.aTitleName.Add(_T("支座名称"));
					TableD.HeaderD.aTitleName.Add(_T("支座类型"));
					TableD.HeaderD.aTitleName.Add(_T("罕遇地震下的拉应力(MPa)"));
					TableD.HeaderD.aTitleName.Add(_T("应力限值(MPa)"));
					TableD.HeaderD.aTitleName.Add(_T("验算结果"));
					//  
					TableD.RecordD.aRecordData.SetSize(aIsoRatio.GetSize());
					auto& record = TableD.RecordD.aRecordData;
					for (int i = 0; i < aIsoRatio.GetSize(); i++)
					{
						NlnkD.Initialize();
						m_pDoc->m_pAttrCtrl->GetNlnk(aIsoRatio[i].nlnkK, NlnkD);
						NllpD.Initialize();
						m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
						AsgbD.Initialize();
						m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);

						m_strFormat.Format(_T("%d"), aIsoRatio[i].nlnkK);
						record[i].Add(m_strFormat);
						record[i].Add(AsgbD.PropName);
						record[i].Add(strProp[AsgbD.nPropType]);
						record[i].Add(Double2String_f(aIsoRatio[i].dVal));
						record[i].Add(Double2String_f(aIsoRatio[i].dLimit));
						if (aIsoRatio[i].dRatio < 999998)
						{
							strText = (aIsoRatio[i].dRatio > 1.0) ? _T("NG") : _T("OK");
						}
						else if (aIsoRatio[i].dVal < 1.0e-11)
							strText = _T("OK");
						else
							strText = _T("NG");
						record[i].Add(strText);
						nIndex++;
					}

					TableD.RecordD.aRecordData.FreeExtra();
					TableD.RecordD.nRecordRowCount = nIndex;
					TableD.RecordD.nRecordColCount = 6;
					strText.Format(_T("隔震支座拉应力验算(罕遇作用下的验算)工况%s+%s"), SplcD.LoadCaseName, SplcD_V.LoadCaseName);
					strBook.Format(_T("CHARPT_7_2_2_%d_TBL"), nSum);
					MakeTableCenterText(genElements, TableD, strText, strBook);
					nSum++;
				}
			}
		}

		for (int i = 0; i < m_IsgbD.aRareEThisCase.GetCount(); i++)
		{
			aIsoRatio.RemoveAll();
			ThisK = m_IsgbD.aRareEThisCase[i];
			if (!IIsolatorTool::Instance()->GetNlnkIsolationData(aIsoRatio, 2, ThisK, D_LOADCASE_HISTORY, LOADTYPE_MAX))
			{
				continue;
			}
			ThisD.Initialize();
			m_pDoc->m_pAttrCtrl->GetThis(ThisK, ThisD);
			nIndex = 0;
			T_TABLE_D TableD;
			TableD.Initialize();
			TableD.HeaderD.nHeaderRowCount = 1;
			TableD.HeaderD.nHeaderColCount = 6;
			TableD.HeaderD.aTitleName.Add(_T("支座编号"));
			TableD.HeaderD.aTitleName.Add(_T("支座名称"));
			TableD.HeaderD.aTitleName.Add(_T("支座类型"));
			TableD.HeaderD.aTitleName.Add(_T("罕遇地震下的拉应力(MPa)"));
			TableD.HeaderD.aTitleName.Add(_T("应力限值(MPa)"));
			TableD.HeaderD.aTitleName.Add(_T("验算结果"));
			//  
			TableD.RecordD.aRecordData.SetSize(aIsoRatio.GetSize());
			auto& record = TableD.RecordD.aRecordData;
			for (int i = 0; i < aIsoRatio.GetSize(); i++)
			{
				NlnkD.Initialize();
				m_pDoc->m_pAttrCtrl->GetNlnk(aIsoRatio[i].nlnkK, NlnkD);
				NllpD.Initialize();
				m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
				AsgbD.Initialize();
				m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);

				m_strFormat.Format(_T("%d"), aIsoRatio[i].nlnkK);
				record[i].Add(m_strFormat);
				record[i].Add(AsgbD.PropName);
				record[i].Add(strProp[AsgbD.nPropType]);
				record[i].Add(Double2String_f(aIsoRatio[i].dVal));
				record[i].Add(Double2String_f(aIsoRatio[i].dLimit));
				if (aIsoRatio[i].dRatio < 999998)
				{
					strText = (aIsoRatio[i].dRatio > 1.0) ? _T("NG") : _T("OK");
				}
				else if (aIsoRatio[i].dVal < 1.0e-11)
					strText = _T("OK");
				else
					strText = _T("NG");
				record[i].Add(strText);
				nIndex++;
			}

			TableD.RecordD.aRecordData.FreeExtra();
			TableD.RecordD.nRecordRowCount = nIndex;
			TableD.RecordD.nRecordColCount = 6;
			strText.Format(_T("隔震支座拉应力验算(罕遇作用下的验算)工况%s"), ThisD.LoadCaseName);
			strBook.Format(_T("CHARPT_7_2_2_%d_TBL"), nSum);
			MakeTableCenterText(genElements, TableD, strText, strBook);
			nSum++;
		}
	}
}
void AutoReport_Gen_CH_ISGB::WriteCharpt7_2_3(GenSegmentElements &genElements)
{
	CString strText, strBook;
	strText = _T("隔震支座位移验算");
	MakeTitle(genElements, strText, 7, 2, _T(""), 3);
	if (m_IsgbD.nCode == 0)
		strText = _T("    根据《建筑隔震设计标准》GB/T 51408-2021第4.6.6条，在罕遇地震作用下.或对于极罕遇特殊设防建筑，位移验算结果如下：");
	else
		strText = _T("    根据《建筑工程减隔震技术规程》北京市地方标准 DB11/2075-2022第11.2.6条，在罕遇地震作用下，位移验算结果如下:");
	MakeText(genElements, strText);
	
	CString strProp[] = { _T("天然橡胶支座(LNR)"),_T("铅芯橡胶支座(LRB)"), _T("高阻尼橡胶支座(HDR)"), _T("弹性滑板支座(ESB)"), _T("摩擦摆支座(FPS)") };
	int nIndex;
	CArray<T_ISO_RATIO, T_ISO_RATIO&> aIsoRatio;
	T_SPLC_D SplcD;
	T_SPLC_K SplcK;
	T_THIS_D ThisD;
	T_THIS_K ThisK;
	T_NLNK_D NlnkD;
	T_NLLP_D NllpD;
	T_ASGB_D AsgbD;
	int nSum = 1;
	
	for (int i = 0; i < m_IsgbD.aRareESplcCase.GetCount(); i++)
	{
		aIsoRatio.RemoveAll();
		SplcK = m_IsgbD.aRareESplcCase[i];
		if (!IIsolatorTool::Instance()->GetNlnkIsolationData(aIsoRatio, 3, SplcK, D_LOADCASE_SPECTRUM, LOADTYPE_MAX))
		{
			continue;
		}
		SplcD.Initialize();
		m_pDoc->m_pAttrCtrl->GetSplc(SplcK, SplcD);
		nIndex = 0;
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 6;
		TableD.HeaderD.aTitleName.Add(_T("支座编号"));
		TableD.HeaderD.aTitleName.Add(_T("支座名称"));
		TableD.HeaderD.aTitleName.Add(_T("支座类型"));
		TableD.HeaderD.aTitleName.Add(_T("支座位移结果"));
		TableD.HeaderD.aTitleName.Add(_T("位移限值"));
		TableD.HeaderD.aTitleName.Add(_T("验算结果"));
		//  
		TableD.RecordD.aRecordData.SetSize(aIsoRatio.GetSize());
		auto& record = TableD.RecordD.aRecordData;
		for (int i = 0; i < aIsoRatio.GetSize(); i++)
		{
			NlnkD.Initialize();
			m_pDoc->m_pAttrCtrl->GetNlnk(aIsoRatio[i].nlnkK, NlnkD);
			NllpD.Initialize();
			m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
			AsgbD.Initialize();
			m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);

			m_strFormat.Format(_T("%d"), aIsoRatio[i].nlnkK);
			record[i].Add(m_strFormat);
			record[i].Add(AsgbD.PropName);
			record[i].Add(strProp[AsgbD.nPropType]);
			record[i].Add(Double2String_f(aIsoRatio[i].dVal));
			if (AsgbD.nPropType == 3)
			{
				record[i].Add(_T("-"));
				record[i].Add(_T("-"));
			}
			else
			{
				record[i].Add(Double2String_f(aIsoRatio[i].dLimit));
				strText = (aIsoRatio[i].dRatio > 1.0) ? _T("NG") : _T("OK");
				record[i].Add(strText);
			}
			nIndex++;
		}

		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = nIndex;
		TableD.RecordD.nRecordColCount = 6;
		strText.Format(_T("隔震支座位移验算(罕遇作用下的验算)工况%s"), SplcD.LoadCaseName);
		strBook.Format(_T("CHARPT_7_2_3_%d_TBL"), nSum);
		MakeTableCenterText(genElements, TableD, strText, strBook);
		nSum++;
	}

	for (int i = 0; i < m_IsgbD.aRareEThisCase.GetCount(); i++)
	{
		aIsoRatio.RemoveAll();
		ThisK = m_IsgbD.aRareEThisCase[i];
		if (!IIsolatorTool::Instance()->GetNlnkIsolationData(aIsoRatio, 3, ThisK, D_LOADCASE_HISTORY, LOADTYPE_MAX))
		{
			continue;
		}
		ThisD.Initialize();
		m_pDoc->m_pAttrCtrl->GetThis(ThisK, ThisD);
		nIndex = 0;
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 6;
		TableD.HeaderD.aTitleName.Add(_T("支座编号"));
		TableD.HeaderD.aTitleName.Add(_T("支座名称"));
		TableD.HeaderD.aTitleName.Add(_T("支座类型"));
		TableD.HeaderD.aTitleName.Add(_T("支座位移结果"));
		TableD.HeaderD.aTitleName.Add(_T("位移限值"));
		TableD.HeaderD.aTitleName.Add(_T("验算结果"));
		//  
		TableD.RecordD.aRecordData.SetSize(aIsoRatio.GetSize());
		auto& record = TableD.RecordD.aRecordData;
		for (int i = 0; i < aIsoRatio.GetSize(); i++)
		{
			NlnkD.Initialize();
			m_pDoc->m_pAttrCtrl->GetNlnk(aIsoRatio[i].nlnkK, NlnkD);
			NllpD.Initialize();
			m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
			AsgbD.Initialize();
			m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);

			m_strFormat.Format(_T("%d"), aIsoRatio[i].nlnkK);
			record[i].Add(m_strFormat);
			record[i].Add(AsgbD.PropName);
			record[i].Add(strProp[AsgbD.nPropType]);
			record[i].Add(Double2String_f(aIsoRatio[i].dVal));
			if (AsgbD.nPropType == 3)
			{
				record[i].Add(_T("-"));
				record[i].Add(_T("-"));
			}
			else
			{
				record[i].Add(Double2String_f(aIsoRatio[i].dLimit));
				strText = (aIsoRatio[i].dRatio > 1.0) ? _T("NG") : _T("OK");
				record[i].Add(strText);
			}
			nIndex++;
		}

		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = nIndex;
		TableD.RecordD.nRecordColCount = 6;
		strText.Format(_T("隔震支座位移验算(罕遇作用下的验算)工况%s"), ThisD.LoadCaseName);
		strBook.Format(_T("CHARPT_7_2_3_%d_TBL"), nSum);
		MakeTableCenterText(genElements, TableD, strText, strBook);
		nSum++;
	}

	for (int i = 0; i < m_IsgbD.aExtreRareESplcCase.GetCount(); i++)
	{
		aIsoRatio.RemoveAll();
		SplcK = m_IsgbD.aExtreRareESplcCase[i];
		if (!IIsolatorTool::Instance()->GetNlnkIsolationData(aIsoRatio, 3, SplcK, D_LOADCASE_SPECTRUM, LOADTYPE_MAX))
		{
			continue;
		}
		SplcD.Initialize();
		m_pDoc->m_pAttrCtrl->GetSplc(SplcK, SplcD);
		nIndex = 0;
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 6;
		TableD.HeaderD.aTitleName.Add(_T("支座编号"));
		TableD.HeaderD.aTitleName.Add(_T("支座名称"));
		TableD.HeaderD.aTitleName.Add(_T("支座类型"));
		TableD.HeaderD.aTitleName.Add(_T("支座位移结果"));
		TableD.HeaderD.aTitleName.Add(_T("位移限值"));
		TableD.HeaderD.aTitleName.Add(_T("验算结果"));
		//  
		TableD.RecordD.aRecordData.SetSize(aIsoRatio.GetSize());
		auto& record = TableD.RecordD.aRecordData;
		for (int i = 0; i < aIsoRatio.GetSize(); i++)
		{
			NlnkD.Initialize();
			m_pDoc->m_pAttrCtrl->GetNlnk(aIsoRatio[i].nlnkK, NlnkD);
			NllpD.Initialize();
			m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
			AsgbD.Initialize();
			m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);

			m_strFormat.Format(_T("%d"), aIsoRatio[i].nlnkK);
			record[i].Add(m_strFormat);
			record[i].Add(AsgbD.PropName);
			record[i].Add(strProp[AsgbD.nPropType]);
			record[i].Add(Double2String_f(aIsoRatio[i].dVal));
			if (AsgbD.nPropType == 3)
			{
				record[i].Add(_T("-"));
				record[i].Add(_T("-"));
			}
			else
			{
				record[i].Add(Double2String_f(aIsoRatio[i].dLimit));
				strText = (aIsoRatio[i].dRatio > 1.0) ? _T("NG") : _T("OK");
				record[i].Add(strText);
			}
			nIndex++;
		}

		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = nIndex;
		TableD.RecordD.nRecordColCount = 6;
		strText.Format(_T("隔震支座位移验算(极罕遇作用下的验算)工况%s"), SplcD.LoadCaseName);
		strBook.Format(_T("CHARPT_7_2_3_%d_TBL"), nSum);
		MakeTableCenterText(genElements, TableD, strText, strBook);
		nSum++;
	}

	for (int i = 0; i < m_IsgbD.aExtreRareEThisCase.GetCount(); i++)
	{
		aIsoRatio.RemoveAll();
		ThisK = m_IsgbD.aExtreRareEThisCase[i];
		if (!IIsolatorTool::Instance()->GetNlnkIsolationData(aIsoRatio, 3, ThisK, D_LOADCASE_HISTORY, LOADTYPE_MAX))
		{
			continue;
		}
		ThisD.Initialize();
		m_pDoc->m_pAttrCtrl->GetThis(ThisK, ThisD);
		nIndex = 0;
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 6;
		TableD.HeaderD.aTitleName.Add(_T("支座编号"));
		TableD.HeaderD.aTitleName.Add(_T("支座名称"));
		TableD.HeaderD.aTitleName.Add(_T("支座类型"));
		TableD.HeaderD.aTitleName.Add(_T("支座位移结果"));
		TableD.HeaderD.aTitleName.Add(_T("位移限值"));
		TableD.HeaderD.aTitleName.Add(_T("验算结果"));
		//  
		TableD.RecordD.aRecordData.SetSize(aIsoRatio.GetSize());
		auto& record = TableD.RecordD.aRecordData;
		for (int i = 0; i < aIsoRatio.GetSize(); i++)
		{
			NlnkD.Initialize();
			m_pDoc->m_pAttrCtrl->GetNlnk(aIsoRatio[i].nlnkK, NlnkD);
			NllpD.Initialize();
			m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
			AsgbD.Initialize();
			m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);

			m_strFormat.Format(_T("%d"), aIsoRatio[i].nlnkK);
			record[i].Add(m_strFormat);
			record[i].Add(AsgbD.PropName);
			record[i].Add(strProp[AsgbD.nPropType]);
			record[i].Add(Double2String_f(aIsoRatio[i].dVal));
			if (AsgbD.nPropType == 3)
			{
				record[i].Add(_T("-"));
				record[i].Add(_T("-"));
			}
			else
			{
				record[i].Add(Double2String_f(aIsoRatio[i].dLimit));
				strText = (aIsoRatio[i].dRatio > 1.0) ? _T("NG") : _T("OK");
				record[i].Add(strText);
			}
			nIndex++;
		}

		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = nIndex;
		TableD.RecordD.nRecordColCount = 6;
		strText.Format(_T("隔震支座位移验算(极罕遇作用下的验算)工况%s"), ThisD.LoadCaseName);
		strBook.Format(_T("CHARPT_7_2_3_%d_TBL"), nSum);
		MakeTableCenterText(genElements, TableD, strText, strBook);
		nSum++;
	}
}
void AutoReport_Gen_CH_ISGB::WriteCharpt7_3(GenSegmentElements &genElements)
{
	CString strText;
	strText = _T("隔震支座结果统计");
	MakeTitle(genElements, strText, 7, 3);
	if (!m_pDoc->IsPostMode())
		return;
	WriteCharpt7_3_1(genElements);
	//WriteCharpt7_3_2(genElements);
}
void AutoReport_Gen_CH_ISGB::WriteCharpt7_3_1(GenSegmentElements &genElements)
{
	CString strText, strBook;
	strText = _T("支座内力统计");
	MakeTitle(genElements, strText, 7, 3, _T(""), 1);

	if (!Para()->IsIncludeThisForceResult())
	{
		strText = _T("支座应力统计");
		MakeTitle(genElements, strText, 7, 3, _T(""), 2);
		return;
	}
	//CArray<T_THFC_K, T_THFC_K> arThfcK;
	CArray<T_NLNK_K, T_NLNK_K> arNlnkK;
	CArray<T_NLNK_D, T_NLNK_D&> arNlnkD;
	CArray<T_THIS_K, T_THIS_K> arThisK;
	//m_pDoc->m_pAttrCtrl->GetThfcKeyList(arThfcK);
	int nIsgbNlnkCount = IIsolatorTool::Instance()->GetIsgbGBNlnkList(arNlnkK,arNlnkD);
	if (nIsgbNlnkCount == 0)
	{
		strText = _T("支座应力统计");
		MakeTitle(genElements, strText, 7, 3, _T(""), 2);
		return;
	}
	m_pDoc->m_pAttrCtrl->GetThisKeyList(arThisK);
	T_NLNK_K NlnkK;
	T_NLNK_D NlnkD;
	T_NLLP_D NllpD;
	T_ASGB_D AsgbD;
	T_STEK_D Stek;
	T_THFC_D ThfcD;
	T_THGA_D ThgaD;	//包含多个地震波 -- 地面加速度(Thga的key即为This时程工况的key)
	//T_THNL_D ThnlD;	//包含单个地震波 -- 节点动力荷载
	T_LCOM_D LcomData;
	int nForceType = Para()->GetBufferData(0);
	/***********************************MakeData*************************************/
	T_KEY_LIST lstThfcKey;
	Report_Key_LIST lstDoubleThisKey;
	int IsgbThfcCount = Para()->ListChkThisCase(lstThfcKey);
	if (lstThfcKey.IsEmpty())
	{
		strText = _T("支座应力统计");
		MakeTitle(genElements, strText, 7, 3, _T(""), 2);
		return;
	}
	Para()->ListDoubleSeisDir(lstDoubleThisKey);
	struct IsgbForce
	{
		double value[3];
	};
	typedef std::map<int, IsgbForce> mapThfc2force;
	/*mapThfc2force*指针分配2个数据(地震波的最大值).0位内力.1为应力.应力数据中0和1代表max和min*/
	std::map<int, mapThfc2force*> mapNlnk2Thfc;
	T_THFC_K ThfcK;
	CArray<T_ISO_RATIO, T_ISO_RATIO&> aIsoRatio;
	double dvalue[3];
	for (int j = 0; j < arThisK.GetSize();j++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetThga(arThisK[j], ThgaD))
		{
			continue;
		}
		//双向地震
		ThfcK = 0;
		if (ThgaD.nThisFuncKey[0] != 0 && ThgaD.nThisFuncKey[1] != 0)
		{
			//过滤选择主方向的双向地震;
			for (int k = 0; k < lstDoubleThisKey.GetSize(); k++)
			{
				if (lstDoubleThisKey[k].m_nKey == arThisK[j])
				{
					ThfcK = ThgaD.nThisFuncKey[lstDoubleThisKey[k].m_nType];
					break;
				}
			}
		}
		else if (ThgaD.nThisFuncKey[2] == 0)
		{
			ThfcK = ThgaD.nThisFuncKey[1] == 0 ? ThgaD.nThisFuncKey[0] : ThgaD.nThisFuncKey[1];
		}
		if (ThfcK == 0)
		{
			continue;
		}
		bool bCheck = false;
		for (int k = 0; k < lstThfcKey.GetSize(); k++)
		{
			if (ThfcK == lstThfcKey[k])
			{
				bCheck = true;
				break;
			}
		}
		if(!bCheck)
			continue;
		m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_HISTORY, arThisK[j], LcomData);
		m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomData);
		for (int i = 0; i < nIsgbNlnkCount; i++)
		{
			NlnkK = arNlnkK[i];
			if (nForceType == 0)//all
				m_pDoc->m_pPostCtrl->GetNlnkNew(NlnkK, 0, 0, &Stek);
			else if (nForceType == 1)//max
				m_pDoc->m_pPostCtrl->GetNlnkNew(NlnkK, &Stek, 0, 0);
			else if (nForceType == 2)//min
				m_pDoc->m_pPostCtrl->GetNlnkNew(NlnkK, 0, &Stek, 0);
			else
			{
				ASSERT(FALSE);
				continue;
			}
			dvalue[0] = (Stek.dblForce[0][0] + Stek.dblForce[1][0]) / 2.0;
			dvalue[1] = (Stek.dblForce[0][1] + Stek.dblForce[1][1]) / 2.0;
			dvalue[2] = (Stek.dblForce[0][2] + Stek.dblForce[1][2]) / 2.0;
			mapThfc2force* pmap = mapNlnk2Thfc[NlnkK];
			if (pmap == 0)
			{
				pmap = mapNlnk2Thfc[NlnkK] = new mapThfc2force[2];
			}
			if (pmap->find(ThfcK) == pmap->end())
			{
				IsgbForce& TempForce = pmap[0][ThfcK];
				TempForce.value[0] = dvalue[0];
				TempForce.value[1] = dvalue[1];
				TempForce.value[2] = dvalue[2];

				IsgbForce& TempStress = pmap[1][ThfcK];
				TempStress.value[0] = TempStress.value[1] = dvalue[0];
			}
			else
			{
				//内力最大值
				IsgbForce& TempForce = pmap[0][ThfcK];
				TempForce.value[0] = GetFabsMax(TempForce.value[0], dvalue[0]);
				TempForce.value[1] = GetFabsMax(TempForce.value[1], dvalue[1]);
				TempForce.value[2] = GetFabsMax(TempForce.value[2], dvalue[2]);
				
				//拉压应力最大值-保存的内力,最后结果需/面积
				IsgbForce& TempStress = pmap[1][ThfcK];
				TempStress.value[0] = max(TempStress.value[0], dvalue[0]);
				TempStress.value[1] = min(TempStress.value[1], dvalue[0]);
			}
		}
	}

	int nIndex = 0;
	CString strProp[] = { _T("LNR"),_T("LRB"), _T("高阻尼橡胶支座(HDR)"), _T("弹性滑板支座(ESB)"), _T("摩擦摆支座(FPS)") };
	CString str;
	/**********************************TABLE 7.3.1-1**************************************/
	if(!mapNlnk2Thfc.empty())
	{
		mapThfc2force* pmap = mapNlnk2Thfc[arNlnkK[0]];
		int nThfcCount = pmap[0].size();
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 7;
		TableD.HeaderD.aTitleName.Add(_T("号"));
		TableD.HeaderD.aTitleName.Add(_T("类型"));
		TableD.HeaderD.aTitleName.Add(_T("名称"));
		TableD.HeaderD.aTitleName.Add(_T("地震波"));
		TableD.HeaderD.aTitleName.Add(_T("轴力N(kN)"));
		TableD.HeaderD.aTitleName.Add(_T("剪力Vy(kN)"));
		TableD.HeaderD.aTitleName.Add(_T("剪力Vz(kN)"));
		TableD.RecordD.aRecordData.SetSize(nThfcCount * nIsgbNlnkCount);
		auto& record = TableD.RecordD.aRecordData;
		//  
		for (auto it : mapNlnk2Thfc)
		{
			NlnkK = it.first;
			pmap = it.second;
			AsgbD.Initialize();
			m_pDoc->m_pAttrCtrl->GetNlnk(NlnkK, NlnkD);
			m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
			ASSERT(NllpD.nApplicationType == 2 && NllpD.nSeisType == 5);
			m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);
			for (auto ifoce : pmap[0])
			{
				ThfcK = ifoce.first;
				const IsgbForce& tempForce = ifoce.second;
				m_pDoc->m_pAttrCtrl->GetThfc(ThfcK, ThfcD);
				record[nIndex].Add(ToString(it.first));
				record[nIndex].Add(AsgbD.PropName);
				record[nIndex].Add(strProp[AsgbD.nPropType]);
				record[nIndex].Add(ThfcD.ThisFuncName);
				record[nIndex].Add(Double2String_f(tempForce.value[0]/1000.0));
				record[nIndex].Add(Double2String_f(tempForce.value[1]/1000.0));
				record[nIndex].Add(Double2String_f(tempForce.value[2]/1000.0));
				nIndex++;
			}
		}

		TableD.RecordD.aRecordData.FreeExtra();
		ASSERT(nThfcCount * nIsgbNlnkCount == nIndex);
		TableD.RecordD.nRecordRowCount = nIndex;
		TableD.RecordD.nRecordColCount = 7;
		strText = _T("各地震波下支座内力统计");
		MakeTableCenterText(genElements, TableD, strText, _T("CHARPT_7_3_1_1_TBL"));
	}

	/********************************TABLE 7.3.1-2 ****************************************/
	if (!mapNlnk2Thfc.empty())
	{
		nIndex = 0;
		mapThfc2force* pmap = mapNlnk2Thfc[arNlnkK[0]];
		int nThfcCount = pmap[0].size();
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 6;
		TableD.HeaderD.aTitleName.Add(_T("号"));
		TableD.HeaderD.aTitleName.Add(_T("类型"));
		TableD.HeaderD.aTitleName.Add(_T("名称"));
		TableD.HeaderD.aTitleName.Add(_T("轴力N(kN)"));
		TableD.HeaderD.aTitleName.Add(_T("剪力Vy(kN)"));
		TableD.HeaderD.aTitleName.Add(_T("剪力Vz(kN)"));
		TableD.RecordD.aRecordData.SetSize(nIsgbNlnkCount);
		auto& record = TableD.RecordD.aRecordData;
		//  
		
		for (auto it : mapNlnk2Thfc)
		{
			NlnkK = it.first;
			pmap = it.second;
			AsgbD.Initialize();
			m_pDoc->m_pAttrCtrl->GetNlnk(NlnkK, NlnkD);
			m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
			ASSERT(NllpD.nApplicationType == 2 && NllpD.nSeisType == 5);
			m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);
			double value[3] = { 0 };
			for (auto ifoce : pmap[0])
			{
				ThfcK = ifoce.first;
				const IsgbForce& tempForce = ifoce.second;
				value[0] = GetFabsMax(value[0], tempForce.value[0]);
				value[1] = GetFabsMax(value[1], tempForce.value[1]);
				value[2] = GetFabsMax(value[2], tempForce.value[2]);
			}

			m_pDoc->m_pAttrCtrl->GetThfc(ThfcK, ThfcD);
			record[nIndex].Add(ToString(it.first));
			record[nIndex].Add(AsgbD.PropName);
			record[nIndex].Add(strProp[AsgbD.nPropType]);
			record[nIndex].Add(Double2String_f(value[0]/1000.0));
			record[nIndex].Add(Double2String_f(value[1]/1000.0));
			record[nIndex].Add(Double2String_f(value[2]/1000.0));
			nIndex++;
		}

		TableD.RecordD.aRecordData.FreeExtra();
		ASSERT(nIsgbNlnkCount == nIndex);
		TableD.RecordD.nRecordRowCount = nIndex;
		TableD.RecordD.nRecordColCount = 6;
		strText = _T("隔震支座内力统计");
		MakeTableCenterText(genElements, TableD, strText, _T("CHARPT_7_3_1_2_TBL"));
	}
	
	strText = _T("支座应力统计");
	MakeTitle(genElements, strText, 7, 3, _T(""), 2);

	/********************************TABLE 7.3.2-1 ****************************************/
	CString strStrss[4];
	if (!mapNlnk2Thfc.empty())
	{
		nIndex = 0;
		mapThfc2force* pmap = mapNlnk2Thfc[arNlnkK[0]];
		int nThfcCount = pmap[1].size();
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 9;
		TableD.HeaderD.aTitleName.Add(_T("号"));
		TableD.HeaderD.aTitleName.Add(_T("类型"));
		TableD.HeaderD.aTitleName.Add(_T("名称"));
		TableD.HeaderD.aTitleName.Add(_T("面积(mm)"));
		TableD.HeaderD.aTitleName.Add(_T("地震波"));
		TableD.HeaderD.aTitleName.Add(_T("拉力N(+)(kN)"));
		TableD.HeaderD.aTitleName.Add(_T("压力N(-)(kN)"));
		TableD.HeaderD.aTitleName.Add(_T("拉应力σ(+)(Mpa)"));
		TableD.HeaderD.aTitleName.Add(_T("压应力σ(-)(Mpa)"));
		TableD.RecordD.aRecordData.SetSize(nThfcCount * nIsgbNlnkCount);
		auto& record = TableD.RecordD.aRecordData;
		//  
		for (auto it : mapNlnk2Thfc)
		{
			NlnkK = it.first;
			pmap = it.second;
			AsgbD.Initialize();
			m_pDoc->m_pAttrCtrl->GetNlnk(NlnkK, NlnkD);
			m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
			ASSERT(NllpD.nApplicationType == 2 && NllpD.nSeisType == 5);
			m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);
			double dArea = 0.;
			switch (AsgbD.nPropType)
			{
			case 0: dArea = PI * pow(AsgbD.LNR.dD, 2) / 4.0;	break;
			case 1: dArea = PI * pow(AsgbD.LRB.dD, 2) / 4.0;	break;
			case 2: dArea = PI * pow(AsgbD.HDR.dD, 2) / 4.0;	break;
			case 3: dArea = PI * pow(AsgbD.ESB.dD, 2) / 4.0;	break;
			case 4: dArea = PI * pow(AsgbD.FPS.nR, 2);	break;
			default:
				dArea = 1.0;
				break;
			}
			for (auto ifoce : pmap[1])
			{
				ThfcK = ifoce.first;
				const IsgbForce& tempForce = ifoce.second;
				m_pDoc->m_pAttrCtrl->GetThfc(ThfcK, ThfcD);
				//拉应力
				if (tempForce.value[0] > 0)
				{
					strStrss[0] = Double2String_f(tempForce.value[0]/1000.0);
					strStrss[2] = Double2String_f(tempForce.value[0]/dArea);
				}
				else
				{
					strStrss[0] = strStrss[2] = _T("-");
				}
				//压应力
				if (tempForce.value[1] < 0)
				{
					strStrss[1] = Double2String_f(tempForce.value[1]/1000.0);
					strStrss[3] = Double2String_f(fabs(tempForce.value[1]) / dArea);
				}
				else
				{
					strStrss[1] = strStrss[3] = _T("-");
				}
				record[nIndex].Add(ToString(it.first));
				record[nIndex].Add(AsgbD.PropName);
				record[nIndex].Add(strProp[AsgbD.nPropType]);
				record[nIndex].Add(Double2String_f(dArea));
				record[nIndex].Add(ThfcD.ThisFuncName);
				record[nIndex].Add(strStrss[0]);
				record[nIndex].Add(strStrss[1]);
				record[nIndex].Add(strStrss[2]);
				record[nIndex].Add(strStrss[3]);
				nIndex++;
			}
		}

		TableD.RecordD.aRecordData.FreeExtra();
		ASSERT(nThfcCount * nIsgbNlnkCount == nIndex);
		TableD.RecordD.nRecordRowCount = nIndex;
		TableD.RecordD.nRecordColCount = 9;
		strText = _T("各地震波下支座应力统计");
		MakeTableCenterText(genElements, TableD, strText, _T("CHARPT_7_3_2_1_TBL"));
	}

	/********************************TABLE 7.3.2-2 ****************************************/
	if (!mapNlnk2Thfc.empty())
	{
		nIndex = 0;
		mapThfc2force* pmap = mapNlnk2Thfc[arNlnkK[0]];
		int nThfcCount = pmap[1].size();
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 8;
		TableD.HeaderD.aTitleName.Add(_T("号"));
		TableD.HeaderD.aTitleName.Add(_T("类型"));
		TableD.HeaderD.aTitleName.Add(_T("名称"));
		TableD.HeaderD.aTitleName.Add(_T("面积(mm)"));
		TableD.HeaderD.aTitleName.Add(_T("拉力N(+)(kN)"));
		TableD.HeaderD.aTitleName.Add(_T("压力N(-)(kN)"));
		TableD.HeaderD.aTitleName.Add(_T("拉应力σ(+)(Mpa)"));
		TableD.HeaderD.aTitleName.Add(_T("压应力σ(-)(Mpa)"));
		TableD.RecordD.aRecordData.SetSize(nThfcCount * nIsgbNlnkCount);
		auto& record = TableD.RecordD.aRecordData;
		//  
		for (auto it : mapNlnk2Thfc)
		{
			NlnkK = it.first;
			pmap = it.second;
			AsgbD.Initialize();
			m_pDoc->m_pAttrCtrl->GetNlnk(NlnkK, NlnkD);
			m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
			ASSERT(NllpD.nApplicationType == 2 && NllpD.nSeisType == 5);
			m_pDoc->m_pAttrCtrl->GetAsgb(NllpD.nSeisKey, AsgbD);
			double dArea = 0.;
			switch (AsgbD.nPropType)
			{
			case 0: dArea = PI * pow(AsgbD.LNR.dD, 2) / 4.0;	break;
			case 1: dArea = PI * pow(AsgbD.LRB.dD, 2) / 4.0;	break;
			case 2: dArea = PI * pow(AsgbD.HDR.dD, 2) / 4.0;	break;
			case 3: dArea = PI * pow(AsgbD.ESB.dD, 2) / 4.0;	break;
			case 4: dArea = PI * pow(AsgbD.FPS.nR, 2);	break;
			default:
				dArea = 1.0;
				break;
			}
			double dN_max = -1.0;
			double dN_min = 1.0;
			for (auto ifoce : pmap[1])
			{
				ThfcK = ifoce.first;
				const IsgbForce& tempForce = ifoce.second;
				//拉应力
				if (tempForce.value[0] > 0)
				{
					dN_max = max(dN_max, tempForce.value[0]);
				}
				//压应力
				if (tempForce.value[1] < 0)
				{
					dN_min = min(dN_min, tempForce.value[1]);
				}
			}

			//拉应力
			if (dN_max > 0)
			{
				strStrss[0] = Double2String_f(dN_max/1000.0);
				strStrss[2] = Double2String_f(dN_max / dArea);
			}
			else
			{
				strStrss[0] = strStrss[2] = _T("-");
			}
			//压应力
			if (dN_min < 0)
			{
				strStrss[1] = Double2String_f(fabs(dN_min)/1000.0);
				strStrss[3] = Double2String_f(fabs(dN_min) / dArea);
			}
			else
			{
				strStrss[1] = strStrss[3] = _T("-");
			}
			record[nIndex].Add(ToString(it.first));
			record[nIndex].Add(AsgbD.PropName);
			record[nIndex].Add(strProp[AsgbD.nPropType]);
			record[nIndex].Add(Double2String_f(dArea));
			record[nIndex].Add(strStrss[0]);
			record[nIndex].Add(strStrss[1]);
			record[nIndex].Add(strStrss[2]);
			record[nIndex].Add(strStrss[3]);
			nIndex++;
			
		}

		TableD.RecordD.aRecordData.FreeExtra();
		ASSERT(nIsgbNlnkCount == nIndex);
		TableD.RecordD.nRecordRowCount = nIndex;
		TableD.RecordD.nRecordColCount = 8;
		strText = _T("隔震支座应力统计");
		MakeTableCenterText(genElements, TableD, strText, _T("CHARPT_7_3_2_2_TBL"));
	}

	/********************************ClearBuff****************************************/
	for (auto it : mapNlnk2Thfc)
	{
		delete []it.second;
	}
	mapNlnk2Thfc.clear();
}

void AutoReport_Gen_CH_ISGB::WriteCharpt8()
{
	GenSegmentElements genElements;
	MakeTitle(genElements, _T("非线性时程分析结果"), 8, 0);
	WriteCharpt8_1(genElements);
	WriteCharpt8_2(genElements);
	m_Segments.push_back(genElements);
}
void AutoReport_Gen_CH_ISGB::WriteCharpt8_1(GenSegmentElements &genElements)
{
	CString strText, strName;
	strText = _T("隔震支座滞回曲线");
	MakeTitle(genElements, strText, 8, 1);
	if (!m_pDoc->IsPostMode())
		return;
	Report_Key_LIST aKeys;
	Para()->ListHysteresisCurveOption(aKeys);
	if (aKeys.IsEmpty())
		return;
	T_EXPFUNC_THISRES_CALL_D callData;
	callData.nType = GPS_TH_SDRES;
	callData.key = 0;
	CArray<T_KEY, T_KEY> arKey;
	CArray<CString, CString> arFullPathName;
	for (int i=0; i<aKeys.GetCount(); i++)
	{
		arKey.Add(aKeys[i].m_nKey);
		strText = CThrsAutoNamer::GetThrsAutoName(aKeys[i].m_nKey);
		arFullPathName.Add(GetImagePath(strText));
	}
	GPSCreateOrActivateThisDlg(m_pDoc, CGPSThisSmartGraphDlg::IDD, arKey, arFullPathName, NULL, (void*)(&callData));
	CString strBookMark;
	for (int i=0; i< aKeys.GetCount(); i++)
	{
		arKey.Add(aKeys[i].m_nKey);
		strText = CThrsAutoNamer::GetThrsAutoName(aKeys[i].m_nKey);
		strBookMark.Format(_T("CHARPT_8_1_%d_FIG"), i + 1);
		MakePicture(genElements, strText, strBookMark);
	}
}
void AutoReport_Gen_CH_ISGB::WriteCharpt8_2(GenSegmentElements &genElements)
{
	CString strText, strBookMark;
	strText = _T("能量图结果");
	MakeTitle(genElements, strText, 8, 2);
	if (!m_pDoc->IsPostMode())
		return;
	T_THGC_D ThgcD;
	m_pDoc->m_pAttrCtrl2->GetThgc(ThgcD);
	if (!ThgcD.bEnergyResult)
	{
		return;
	}
	
	if(!m_pDoc->m_pAttrCtrl->ExistThis_Nonlinear_DiSt())
		return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_RESULT_TIMEHISTORY_ENGR_GRPH, 0));
	I_GENModelBase	*gm = I_GENModelBase::GetCurMySelfST();
	CGPSBarTHEnrgGrphContainer* pThEnrgGrph = CGPSBarTHEnrgGrphContainer::m_pMyself;
	pThEnrgGrph->AutoBtnDyGen();
	T_THIS_D ThisD;
	int nIndex = 0;
	
	CArray<T_THIS_K, T_THIS_K> arThisK;
	m_pDoc->m_pAttrCtrl->GetThisKeyList(arThisK);
	for (int i=0; i<arThisK.GetCount(); i++)
	{	
		m_pDoc->m_pAttrCtrl->GetThis(arThisK[i], ThisD);
#ifdef _MEC
		if (ThisD.nAnalType != 2 || (ThisD.nAnalMethod != 2 && ThisD.nAnalMethod != 3) ||
			(m_pDoc->m_pAttrCtrl2->HasThgcDummyInitLoad() && m_pDoc->IsDummyInitLoadThis(arThisK[i])))
			continue;
#else
		if (ThisD.nAnalType != 2 || (ThisD.nAnalMethod != 2 && ThisD.nAnalMethod != 3) || 
			(m_pDoc->m_pAttrCtrl2->HasThgcDummyInitLoad() && arThisK[i] == ThgcD.ThisK_DummyInitLoad))
			continue;
#endif
		strBookMark.Format(_T("CHARPT_8_2_%d_FIG"), ++nIndex);
		MakePicture(genElements, ThisD.LoadCaseName, strBookMark);
	}
}

void AutoReport_Gen_CH_ISGB::FigureRltGM(BOOL bAnimation)
{
	SetViewPoint(0);
	CViewBase* pView = static_cast<CViewBase*>(CViewBase::GetCurView_ST());
	I_GPSModel* gm = pView->GetIPM();
	CDC *pDC = pView->GetDC();
	//
	int nHPixel = pDC->GetDeviceCaps(HORZRES);
	double dFontRatio = 0.0;
	if (nHPixel >= 1280) dFontRatio = 1.1;
	else if (nHPixel >= 1152 && nHPixel < 1280) dFontRatio = 1.0;
	else if (nHPixel >= 1024 && nHPixel < 1152) dFontRatio = 1.0;
	else if (nHPixel < 1024) dFontRatio = nHPixel / 1400.0;
	gm->m_LegendEngine.InitFont(20 * dFontRatio, 14 * dFontRatio);
	//
	gm->m_LoadMinMaxType = LOADTYPE_NORMAL;
	gm->m_bLoadDataExist = TRUE;
	gm->m_bLoadDataSet = TRUE;
	gm->m_bContour = TRUE;
	gm->m_DOPT.LG.m_bDrawLegend = TRUE;//云图
	gm->m_bDeformed = FALSE;//是否变形
	gm->m_bAnimation = FALSE;
	gm->m_DOPT.DR.m_bDrawInactive = FALSE;
	gm->m_DOPT.TX.m_bOutputNumber = FALSE;//是否显示数值
	gm->m_bMdShpContour = TRUE;//Model
	gm->m_bAnimation = bAnimation;//动画模式,是否随动变换等值线
	if (gm == NULL)
		return;
}
void AutoReport_Gen_CH_ISGB::WriteForceAndDispRlt(GenSegmentElements &Elems,I_GPSModel* gm,const Report_Key_LIST& aKeys,const CString& strBook)
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
void AutoReport_Gen_CH_ISGB::WriteModelRlt(GenSegmentElements &Elems,I_GPSModel* gm,const Report_Key_LIST& aKeys,Report_Key_LIST& aGroupKeys,int& nIndex)
{
	CString strRltName,strGroup,strDir,strLoad;
	int nMode = 0;
	T_GRUP_D GrupD;
	CString str,strBook;
	//if(aGroupKeys.IsEmpty())
	//	aGroupKeys.InsertAt(0,Report_Key(0,0));
	/**********************wangjing:要用户控制全部**********************/
	if (aGroupKeys.IsEmpty())
		return;
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
			strBook.Format(_T("CHARPT_5_1_FIG_%d"),++nIndex);
			MakeImage_Current(str, FALSE);
			//
			MakePicture(Elems, str,strBook);
		}
	}
	gm->m_bDeformed							= FALSE;
}

double AutoReport_Gen_CH_ISGB::CalcDampingRto(T_NLNK_K NlnkK, double dDamp1, double dDamp2)
{
	double dKesi = 0.0;
	double dc = (dDamp1 + dDamp2) / 2;
	T_FREQ_D data; data.Initialize();
	m_pDoc->m_pPostCtrl->GetEiFreq(1, data);
	double dT1 = data.dblFreq[2];
	T_STEK_D Stek;
	T_LCOM_D LcomD;
	double dPress = 0.0;
	m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_COMB_GENERAL, m_IsgbD.nLcomG, LcomD);
	m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);
	if (!m_pDoc->m_pPostCtrl->GetNlnkNew(NlnkK, &Stek, 0, 0))
	{
		ASSERT(FALSE);
		return 0.0;
	}
	if (Stek.dblForce[0][0] > 0.0)
	{
		//轴力应该都是负值.
		ASSERT(FALSE);
	}
	double dmi = fabs(Stek.dblForce[0][0])/ 9.8;
	if (dmi < 1.0e-19)
		dKesi = 1.0;
	else
		dKesi = dc * dT1 * 1000 / (4 * PI*dmi);
	return dKesi;
}

BOOL AutoReport_Gen_CH_ISGB::OpenSSFCFile(CString fName)
{
	if (fName.IsEmpty())
		return FALSE;

	USES_CONVERSION;
	m_rfp = fopen(CT2A((LPCTSTR)fName), "rt");
	if (!m_rfp)
	{
		ErrMsg(fName, _LS(IDS_CONV_GT_FILE_ERR1));
		return FALSE;
	}
	return TRUE;
}

int AutoReport_Gen_CH_ISGB::lex(FILE *fp, char *yytext, CStringArray& aString)
{
	int		c, index,k;
	index = 0;
	k = 0;
	c = getc(fp);

	// Discard white characters
	while ((c == ' ') || (c == '\r') || (c == '\t') || (c == '\n')) {
		c = getc(fp);
	}

	if (c == EOF) return -1;
	aString.RemoveAll();
	char	yycheck[120];
	//double dchecktemp;
	while (c != '\n')
	{
		if (c != ' ' && c != '\t')
		{
			yytext[index++] = c;
		}

		if (c == ' ' || c == '\t')
		{
			if (k != 0)
			{
				yycheck[k++] = '\0';
				aString.Add(yycheck);
				k = 0;
			}
			c = getc(fp);
			continue;
		}
		yycheck[k++] = c;
		c = _gettc(fp);
		if (c == EOF) return -1;
	}

	yytext[index++] = '\0';
	return 1;
}

BOOL AutoReport_Gen_CH_ISGB::ReadSSFCData(CString strRSLoadName, CArray<SeisLayerShear, SeisLayerShear &> &arShearX, CArray<SeisLayerShear, SeisLayerShear &> &arShearY)
{
	CStringA strCase[2];
	//int			num = 0, idx = 0;
	//char		str[500];
	//CString		s;
	arShearX.RemoveAll();
	arShearY.RemoveAll();
	char	yytext[120];
	char	yycheck[120];
	CStringArray aString;
	SeisLayerShear seisShear;
	//for (int i = 0; i < 2; i++)
	//{
	strCase[0].Format("%s-X-Dir", CTextFileOut::CString2Mbcs(strRSLoadName));
	strCase[1].Format("%s-Y-Dir", CTextFileOut::CString2Mbcs(strRSLoadName));
	bool bstartread = false;
	double dchecktemp;
	CArray<SeisLayerShear, SeisLayerShear &>* pSeis = NULL;
	while (lex(m_rfp, yytext, aString) == 1)
	{
		if (bstartread)
		{
			if(yytext[0] == '(')
				bstartread = false;
			else if (aString.GetCount() == 6)
			{
				bool bcheck = true;
				for (int i = 2; i < 4; i++)
				{
					dchecktemp = _ttof(aString[i].GetBuffer(0));
					//CString str;
					//str.IsEmpty()SpanIncluding strJudgePos
					if (fabs(dchecktemp) < 1.0e-20)
					{
						bcheck = false;
						break;
					}
				}
				if (bcheck)
				{
					seisShear.strModle = aString[0];
					seisShear.strStor = aString[1];
					seisShear.dShear = _ttof(aString[3].GetBuffer(0));
					pSeis->Add(seisShear);
				}
				continue;
			}
		}
		if (yytext[0] != '(')
			continue;
		pSeis = NULL;
		int m = 0;
		bool bstartcopoy = false;
		for (int k = 0; k < 120; k++)
		{
			if (yytext[k] == ')')
			{
				bstartcopoy = true;
				continue;
			}
			if (bstartcopoy)
			{
				yycheck[m++] = yytext[k];
				if (yytext[k] == '\0')
					break;
			}
		}
		if (!bstartcopoy)
			continue;
		if (strcmp(yycheck, strCase[0]) == 0)
			pSeis = &arShearX;
		else if (strcmp(yycheck, strCase[1]) == 0)
			pSeis = &arShearY;
		if (pSeis == NULL)
			continue;
		bstartread = true;
	}
	return TRUE;
}

BOOL AutoReport_Gen_CH_ISGB::ReadNonlistValueFromDataLine(CString dataLine, CArray<CString, CString> &nlValue)
{
	CString	sTmp = dataLine;
	int	len = dataLine.GetLength();
	CString temp = _T("");
	for (int i = 2; i < len; i++)
	{
		if (dataLine[i] != ' ') 
		{
			temp += dataLine[i];
		}
		else
		{
			if (temp != ' ')
			{
				nlValue.Add(temp);
			}
		}
	}
		
	return TRUE;
}

BOOL AutoReport_Gen_CH_ISGB::IsBlankLine(CString dataLine)
{
	for (int i = 0; i < dataLine.GetLength(); i++)
		if (dataLine[i] != ' ' &&	dataLine[i] != '\0' && dataLine[i] != '\n') return FALSE;

	return TRUE;
}

BOOL AutoReport_Gen_CH_ISGB::IsSingleLine(CString dataLine)
{
	if (dataLine.Find(_LS(IDS_MAIN_REPORT_TXT_LINE_SINGLE)) >= 0)
		return TRUE;
	return FALSE;
}

BOOL AutoReport_Gen_CH_ISGB::GetDataLine(char *str, CString &dataLine, BOOL bData/* = FALSE*/)
{
	int	ret = 0, len = 0;

	dataLine.Format(_T("%s"), str);
	if (!bData)
		dataLine.Remove(' ');

	//dataLine.Replace('\n', ' ');
	return TRUE;
}

BOOL AutoReport_Gen_CH_ISGB::GetConnectStringBySlush(CString &dataLine)
{
	int ret = 0, len = 0;
	char tstr[500];

	dataLine.Replace('\n', ' ');
	if ((ret = dataLine.Find(_T(" - "))) >= 0)
	{
		CString tmp;
		dataLine.Remove('\n');
		len = dataLine.GetLength();
		if (ret != 0)dataLine.Delete(ret, len - ret - 1);
		//fgetpos(m_rfp,&m_fpos);
		fgets(tstr, 500, m_rfp);
		GetDataLine(tstr, tmp);	// recursive
		dataLine += (LPCTSTR)tmp;
		return TRUE;
	}
	return FALSE;
}

BOOL AutoReport_Gen_CH_ISGB::DevideLine(CString dataLine, CArray<CString, CString> &nlValue)
{
	int		i = 0, len = 0;
	CString	val;
	CString	sTmp = dataLine;

	// Tab key甫 傍归巩磊肺 官厕
	len = sTmp.GetLength();
	for (int i = 0; i < len; i++) if (sTmp[i] == '\t') sTmp.SetAt(i, ' ');

	if (sTmp.Find(';') >= 0) // 霓付肺 备盒等 单捞磐扼搁
	{
		for (;;)
		{
			i = sTmp.Find(';');
			if (i >= 0)
			{
				sTmp.SetAt(i, '\0');
				val = sTmp.Left(i);
				val += '\0';
				int len = val.GetLength();
				nlValue.Add(val);
				sTmp.Delete(0, i + 1);
			}
			else
			{
				val = sTmp;
				val.Remove(';');
				nlValue.Add(val);
				break;
			}
		}
	}
	else
	{
		nlValue.Add(sTmp);
	}
	return TRUE;
}

BOOL AutoReport_Gen_CH_ISGB::ErrMsg(CString sLine, CString cmt)
{
	CString ccm;
	sLine.Remove('\n');
	cmt.Remove('\n');
	ccm.Format(_T(" %s    %s"), sLine, cmt);
	return TRUE;
}

double AutoReport_Gen_CH_ISGB::GetFabsMax(double v1, double v2)
{
	if (fabs(v1) > fabs(v2))
		return v1;
	return v2;
}