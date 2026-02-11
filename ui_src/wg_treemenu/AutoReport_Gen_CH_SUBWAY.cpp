#include "stdafx.h"
#include "AutoReport_Gen_CH_SUBWAY.h"
#include "ReportTreeText.h"
#include "ReportCommonFunc.h"
#include "..\wg_xl\ReportTableProcess.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_gps\GPSAMRUtil.h"
#include "..\wg_gr\GRAMRUDFData.h"
#include "..\wg_gr\GRenderView.h"
#include "..\wg_base\wg_baseAll.h"
#include "..\wg_base\wg_baseAllTemp.h"
#include "..\wg_gps\wg_gpsAll.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_db\MatlDB.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\EditData.h"
#include "..\wg_db\MembCtrl.h"
#include "..\wg_gr\I_GEN_GPSCtrl.h"
#include "..\wg_db\DesignResult.h"
#include "..\wg_base\GraphFuncview.h"
#include "..\wg_rcs\RCSCodeCheck.h"
#include "..\wg_rcs\RCSDataBase.h"
#include "..\wg_dgn\DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable:4566)

Report_Para_CH* AutoReport_Gen_CH_SUBWAY::Para() const
{
	return AutoReport_Gen_Mgr::Inst()->Para();
}
 

int AutoReport_Gen_CH_SUBWAY::TempletID()
{
	return 0;
}
BOOL AutoReport_Gen_CH_SUBWAY::PreMakeData()
{
	Initial();
	m_Segments.clear();
	CViewBase::GetCurView_ST()->DoInitialView();
	SetShape(FALSE);
	setTableFormatString();
	//
	GRenderView* pRenderView = (GRenderView*)(I_GENModelBase::GetCurMySelfST()->GetOwnerWnd()); ASSERT(pRenderView);
	CAutoRecoveryUnit unitSet(D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_FORCE_INDEX_N, m_pDoc->m_pUnitCtrl, TRUE);
	PushDspOpt();

	WriteCharpt1();
	WriteCharpt2();
	WriteCharpt3();
	WriteCharpt4();
	WriteCharpt5();
	WriteCharpt6();

	PopDspOpt();
	CViewBase::GetCurView_ST()->DoInitialView();
	return TRUE;
}
void AutoReport_Gen_CH_SUBWAY::InsertCoverPage()
{
	GEN_IFTAG_STR pSrc;
	GEN_IFTAG pTgt;
	CString sProductInfo = _LSX(GEN);
	CString strText = _T("");
	CString strTemp = _T("");
	T_PJCF_D data;
	if (!m_pDoc->m_pAttrCtrl->GetPjcf(data))
		data.Initialize();
	CString strProjectName = data.strProject;


	if (data.strProject.IsEmpty() || data.strProject == _T(""))
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
	time = CTime::GetCurrentTime();
	int nYear = time.GetYear();
	int nMonth = time.GetMonth();
	int nDate = time.GetDay();
	strDate.Format(_T("%d-%d-%d"), nYear, nMonth, nDate);
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=false align=left[/DRG_FONT]\n\n\n\n\n\n\n%s%s[DRG_FONT]|FI|[/DRG_FONT]"), FillBlank(_T(""), 13), _T("设计:"));
	strText += strTemp;
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=false align=left underline=1[/DRG_FONT]%s\n[DRG_FONT]|FI|[/DRG_FONT]"), FillBlank(data.strReviewName[0]));
	strText += strTemp;
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=false align=left[/DRG_FONT]%s%s[DRG_FONT]|FI|[/DRG_FONT]"), FillBlank(_T(""), 13), _T("校对:"));
	strText += strTemp;
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=false align=left underline=1[/DRG_FONT]%s\n[DRG_FONT]|FI|[/DRG_FONT]"), FillBlank(data.strReviewName[1]));
	strText += strTemp;
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=false align=left[/DRG_FONT]%s%s[DRG_FONT]|FI|[/DRG_FONT]"), FillBlank(_T(""), 13), _T("审核:"));
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
	CReportCommonFunc::ChangeStructStr2Char(&pSrc, &pTgt);
	CReportCommonFunc::ExportData2Word(&pTgt);
	CReportCommonFunc::ExportInsertPageBreak();
}
void AutoReport_Gen_CH_SUBWAY::InsertCatalogPage()
{
	CString strText = _T("");
	strText = _T("");
	CString strTemp = _T("");

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
	CReportCommonFunc::ChangeStructStr2Char(&pSrc, &pTgt);
	CReportCommonFunc::ExportData2Word(&pTgt);
	CReportCommonFunc::ExportInsertCatalog(2);
	CReportCommonFunc::ExportInsertPageBreak();
}
void AutoReport_Gen_CH_SUBWAY::MakeContent()
{
	for (int i = 0; i < m_Segments.size(); i++)
	{
		InsertSegmentElements(m_Segments[i]);
	}
}
void AutoReport_Gen_CH_SUBWAY::MakeTable_CH(GenSegmentElements &genElements, T_TABLE_D &TableD, LPCTSTR lpCaption, LPCTSTR lpBookMark)
{
	if (TableD.RecordD.nRecordRowCount <= 0)
		return;
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = _T("");
	genFtagStr.sType = _LSX(TBL);
	CReportTableProcess ReportTable;
	ReportTable.SetData(TableD);
	ReportTable.SetFilePathName(ReportTable.GetFilePathName() + lpCaption + _T(".xml"));
	ReportTable.GenerateReport();

	genFtagStr.sFullPath = ReportTable.GetFilePathName() + lpCaption + _T(".xml");
	genFtagStr.sExtInfo = lpBookMark == nullptr ? lpCaption : lpBookMark;
	genFtagStr.sContents = _T("");
	genFtagStr.iAppendFlag = D_WORD_APPEND_INSERT;
	genFtagStr.sExt = _T(" ");
	genFtagStr.iLevel = 0;
	CH_GEN_IFTAG_STR genFtagStrTile;
	CString strContents;
	strContents.Format(_T("[DRG_FONT]|FC| size=11 bold=false align=center[/DRG_FONT]%s \n[DRG_FONT]|FI|[/DRG_FONT]"), lpCaption);
	genFtagStrTile.sContents = strContents;
	genFtagStrTile.sType = _LSX(TXT);
	genFtagStrTile.sFullPath = _T(" ");
	genFtagStrTile.sExtInfo = _T("");
	genFtagStrTile.iAppendFlag = D_WORD_APPEND_INSERT;
	genFtagStrTile.sExt = _T(" ");
	genFtagStrTile.iLevel = 0;
	genElements.m_SegmentElements.push_back(genFtagStrTile);
	genElements.m_SegmentElements.push_back(genFtagStr);
}
void AutoReport_Gen_CH_SUBWAY::WriteCharpt1()
{
	if (Para()->SegmentIsCheck(1000 * ARGC_SUBWAY + 1, 0) == FALSE)
		return;
	GenSegmentElements Elems;
	MakeTitle(Elems, _T("设计依据"), 1, 0);
	MakeTitle(Elems, _T("支撑性文件"), 1, 1);
	CString str;
	str.Format(_T("    %s"), GetBold(_T("#用户自行输入#")));
	MakeText(Elems, str);

	MakeTitle(Elems, _T("执行和参考的规范、规程"), 1, 2);
	{
		CString str;
		MakeAlignText(str, _T("《地铁设计规范》"), _T("GB50157-2013"));
		MakeAlignText(str, _T("《城市轨道交通技术规范》"), _T("GB 50490-2009"));
		MakeAlignText(str, _T("《地下铁道工程施工及验收规范》"), _T("GB50299-1999（2003年版）"));
		MakeAlignText(str, _T("《建筑结构荷载规范》"), _T("GB50009-2012"));
		MakeAlignText(str, _T("《建筑抗震设防分类标准》"), _T("GB50223-2008"));
		MakeAlignText(str, _T("《建筑抗震设计规范》"), _T("GB/T50011-2010（2016年版）"));
		MakeAlignText(str, _T("《混凝土结构设计规范》"), _T("GB50010-2010（2015年版）"));
		MakeAlignText(str, _T("《混凝土结构工程施工质量验收规范》"), _T("GB 50204-2015"));
		MakeAlignText(str, _T("《钢结构设计规范》"), _T("GB50017-2003"));
		MakeAlignText(str, _T("《混凝土结构耐久性设计规范》"), _T("GB/T 50476-2008"));
		MakeAlignText(str, _T("《地下工程防水技术规范》"), _T("GB50108-2008"));
		MakeAlignText(str, _T("《铁路工程抗震设计规范》"), _T("GB50111-2006（2009年版）"));
		MakeAlignText(str, _T("《人民防空工程设计规范》"), _T("GB50225-2005"));
		MakeAlignText(str, _T("《轨道交通工程人民防空设计规范》"), _T("RFJ02-2009"));
		MakeAlignText(str, _T("《补偿收缩混凝土应用技术规程》"), _T("JGJ-T178-2009"));
		MakeAlignText(str, _T("《钢筋机械连接技术规程》"), _T("JGJ107-2016"));
		MakeAlignText(str, _T("《铁路混凝土结构耐久性设计规范》"), _T("TB 10005-2010"));
		MakeAlignText(str, _T("《钢结构焊接规范》"), _T("GB50661-2011"));
		MakeAlignText(str, _T("《建筑地基基础设计规范》"), _T("GB50007-2011"));
		MakeAlignText(str, _T("《城市轨道交通结构抗震设计规范》"), _T("GB50909-2014"));
		MakeAlignText(str, _T("《地铁杂散电流腐蚀防护技术规程》"), _T("CJJ49-1992"));
		MakeAlignText(str, _T("《非合金钢及细晶粒钢焊条》"), _T("GB/T5117-2012"));
		MakeAlignText(str, _T("《热强刚焊条》"), _T("GB/T5118-2012"));
		MakeAlignText(str, _T("《建筑结构可靠度设计统一标准》"), _T("GB50068-2001"));
		MakeText(Elems, str);
		MakeText(Elems, _T("《混凝土结构施工图平面整体表示方法制图规则和构造详图》16G101-1图集"));
	}
	m_Segments.push_back(Elems);
}
void AutoReport_Gen_CH_SUBWAY::WriteCharpt2()
{
	if (Para()->SegmentIsCheck(1000 * ARGC_SUBWAY + 2, 0) == FALSE)
		return;
	GenSegmentElements Elems;
	CString str;
	MakeTitle(Elems, _T("工程概况及工程地质、水文地质"), 2, 0);

	if (Para()->SegmentIsCheck(1000 * ARGC_SUBWAY + 2, 1))
	{
		MakeTitle(Elems, _T("地形地貌"), 2, 1);
		str.Format(_T("    %s"), GetBold(_T("#用户自行输入#")));
		MakeText(Elems, str);
	}

	if (Para()->SegmentIsCheck(1000 * ARGC_SUBWAY + 2, 2))
	{
		MakeTitle(Elems, _T("工程地质情况"), 2, 2);
		str.Format(_T("    %s"), GetBold(_T("#用户自行输入#")));
		MakeText(Elems, str);
	} 

	if (Para()->SegmentIsCheck(1000 * ARGC_SUBWAY + 2, 3))
	{
		MakeTitle(Elems, _T("水文地质情况"), 2, 3);
		str.Format(_T("    %s"), GetBold(_T("#用户自行输入#")));
		MakeText(Elems, str);
	}

	if (Para()->SegmentIsCheck(1000 * ARGC_SUBWAY + 2, 4))
	{
		MakeTitle(Elems, _T("场地和地基的地震效应"), 2, 4);
		BOOL bSplc = FALSE;
		CArray<T_SPLC_K, T_SPLC_K> aSplcK;
		T_SPLC_D SplcD;
		T_SPFC_D SpfcD;
		m_pDoc->m_pAttrCtrl->GetSplcKeyList(aSplcK);
		for (int i = 0; i < aSplcK.GetSize(); i++)
		{
			m_pDoc->m_pAttrCtrl->GetSplc(aSplcK[i], SplcD);
			for (int j = 0; j < SplcD.aSpfcKey.GetSize(); j++)
			{
				if (m_pDoc->m_pAttrCtrl->GetSpfc(SplcD.aSpfcKey[j], SpfcD) && (SpfcD.nSpecCodeType == D_SPFC_CODE_CH2019 || SpfcD.nSpecCodeType == D_SPFC_CODE_CH2010 ||
					SpfcD.nSpecCodeType == D_SPFC_CODE_CH2002 || SpfcD.nSpecCodeType == D_SPFC_CODE_CH_GB_T_51408_2021))
				{
					bSplc = TRUE;
					break;
				}
			}
			if (bSplc)
				break;
		}
		int nSFI[] = { 6,7,7,8,8,9 };
		double dSFI[] = { 0.05,0.10,0.15,0.20,0.30,0.40 };
		CString aStrSDC[] = { _T("一"),_T("二"),_T("三") };
		CString astrSC[] = { _T("I0"), _T("I1"),  _T("II"), _T("III"), _T("IV") };
		int nSfi = nSFI[0];
		double dSfi = dSFI[0];
		CString strSC = astrSC[0];
		CString strSDC = aStrSDC[0];
		double dTg = 0.2;
		if (bSplc)
		{
			if (SpfcD.nSpecCodeType == D_SPFC_CODE_CH2019)
			{
				nSfi = nSFI[SpfcD.CodeParam.CH2019.nSeisFortification];
				dSfi = dSFI[SpfcD.CodeParam.CH2019.nSeisFortification];
				strSC = astrSC[SpfcD.CodeParam.CH2019.nSiteClass];
				strSDC = aStrSDC[SpfcD.CodeParam.CH2019.nSeisDgnCategory];
				dTg = SpfcD.CodeParam.CH2019.dTg;
			}
			else if (SpfcD.nSpecCodeType == D_SPFC_CODE_CH2010)
			{
				nSfi = nSFI[SpfcD.CodeParam.CH2010.nSeisFortification];
				dSfi = dSFI[SpfcD.CodeParam.CH2010.nSeisFortification];
				strSC = astrSC[SpfcD.CodeParam.CH2010.nSiteClass];
				strSDC = aStrSDC[SpfcD.CodeParam.CH2010.nSeisDgnCategory];
				dTg = SpfcD.CodeParam.CH2019.dTg;
			}
			else if (SpfcD.nSpecCodeType == D_SPFC_CODE_CH_GB_T_51408_2021)
			{
				nSfi = nSFI[SpfcD.CodeParam.GB_T_51408_2021.nSeisFortification];
				dSfi = dSFI[SpfcD.CodeParam.GB_T_51408_2021.nSeisFortification];
				strSC = astrSC[SpfcD.CodeParam.GB_T_51408_2021.nSiteClass];
				strSDC = aStrSDC[SpfcD.CodeParam.GB_T_51408_2021.nSeisDgnCategory];
				dTg = SpfcD.CodeParam.GB_T_51408_2021.dTg;
			}
			else if (SpfcD.nSpecCodeType == D_SPFC_CODE_CH2002)
			{
				nSfi = nSFI[SpfcD.CodeParam.CH2002.nSeisFortification];
				dSfi = dSFI[SpfcD.CodeParam.CH2002.nSeisFortification];
				strSC = astrSC[SpfcD.CodeParam.CH2002.nSiteClass];
				strSDC = aStrSDC[SpfcD.CodeParam.CH2002.nSeisDgnCategory];
				dTg = SpfcD.CodeParam.CH2019.dTg;
			}
			else
				ASSERT(FALSE);
		}
		//
		CString str;
		str.Format(_T("    根据《建筑抗震设计规范》（GB/T50011-2010）,《中国地震动峰值加速度区划图》XX省区划一览表及按《铁路工程抗震设计规范》\
GB50111-2006，2009版）、《建筑抗震设计规范》（GB/T50011-2010，2016年版）、《城市轨道交通结构抗震设计规范》（GB50909-2014），\
设计地震分组属第%s组,抗震设防烈度为%s度，场地类别为%s类,地震影响评级为__，设计特征周期为%ss。按__类建筑物进行抗震设计。"), \
			GetBold(strSDC), GetBold(nSfi), GetBold(strSC),GetBold(dTg));
		CString strBook = _T("CHARPT_2_TXT");
		MakeText(Elems, str, strBook);
	}
	m_Segments.push_back(Elems);
}
void AutoReport_Gen_CH_SUBWAY::WriteCharpt3()
{
	if (Para()->SegmentIsCheck(1000 * ARGC_SUBWAY + 3, 0) == FALSE)
		return;

	GenSegmentElements Elems;
	CString str;
	MakeTitle(Elems, _T("计算模型及荷载分类"), 3, 0);
	if (Para()->SegmentIsCheck(1000 * ARGC_SUBWAY + 3, 1))
	{
		MakeTitle(Elems, _T("计算模型确定"), 3, 1);
		MakeCompleteText(str, _T("    [DRG_FONT]|FC| size=11 bold=false align=left[/DRG_FONT]地铁车站标准段为长通道结构，横向尺寸远小于纵向尺寸，故可简化为平面问题求解。采用荷载—结构相互作用模型进行分析，弹性抗力按winkler理论确定，其公式为："));
		
		str += _T("[DRG_FONT]|FC|size=11 bold=false align=center[/DRG_FONT]σ = Kdi \n[DRG_FONT]|FI|[/DRG_FONT]");
		//MakeText(Elems, str);
		//str.Empty();
		//str.Format(_T("[DRG_FONT]|FC|  align=center[/DRG_FONT]%s[DRG_FONT]|FI|[/DRG_FONT]"), _T("σ = Kdi"));
		MakeCompleteText(str, _T("    [DRG_FONT]|FC| size=11 bold=false align=left[/DRG_FONT]其中，弹性抗力系数K由土工试验得到。"));
		MakeCompleteText(str, _T("    计算时分别用水平弹簧、竖向弹簧来模拟土体对墙体（底板）水平位移和垂直位移的约束作用，弹簧只能受压，\
且注意弹簧的计算反力不应大于地基的承载力。框架柱按刚度等效的原则换算为沿纵向设置的矩形截面墙考虑。"));
		MakeCompleteText(str, _T("    车站主体结构按底板支撑在弹性地基上的平面框架进行内力分析，取纵向1m的标准段为一个计算单元，采用有限元计算软件midas Gen<2021 V2.1版本>进行结构计算。"));
		MakeCompleteText(str, _T("    计为进一步分析纵梁和框架柱的内力，按多跨连续平面框架模型，并考虑纵梁承受其两侧各半跨的荷载进行计算，采用底层纵梁下设弹簧的方法模拟结构与土层相互作用。"));
		MakeText(Elems, str);
	}

	if (Para()->SegmentIsCheck(1000 * ARGC_SUBWAY + 3, 2))
	{
		MakeTitle(Elems, _T("荷载分类"), 3, 2);
		str.Empty();
		MakeCompleteText(str, GetBold(_T("  永久荷载")));
		MakeCompleteText(str, _T("   1.结构自重：钢筋混凝土结构重度γ＝25kN/m3，输入截面尺寸及材料参数后，软件在计算时自动计算及施加。"));
		MakeCompleteText(str, _T("   2.地层压力：土容重取各土层加权平均值，覆土厚度取施工完成后回填的实际厚度。"));
		MakeCompleteText(str, _T("    垂直荷载：明挖法施工的车站按顶板以上全部土柱重量计算。"));
		MakeCompleteText(str, _T("    水平荷载：根据结构受力过程中墙体位移与地层间的相关关系，可选取主动土压力、静止土压力、被动土压力计算。"));
		MakeCompleteText(str, _T("   计算中应计及地面荷载和邻近建筑物以及施工机械等引起的附加水平侧向力。"));
		MakeCompleteText(str, _T("   3.设备重量：内部设备按等效荷载8kPa，特殊设备另计。"));
		MakeCompleteText(str, _T("   4.水压力及浮力：水容重γ＝10kN/m3，抗浮设防水位按取至地面计算。"));
		MakeCompleteText(str, GetBold(_T("  可变荷载")));
		MakeCompleteText(str, _T("   1.站厅层、站台层、楼梯和车站人员管理用房等部位的人群荷载取4kPa；"));
		MakeCompleteText(str, _T("   2.标准段地面超载按等效均布荷载取20kPa，盾构端头井附近的地面超载按等效均布荷载取30kPa；"));
		MakeCompleteText(str, _T("   3.地铁列车荷载考虑其冲击力，折算按等效均布荷载取20kPa。"));
		MakeCompleteText(str, GetBold(_T("  偶然荷载")));
		MakeCompleteText(str, _T("   1.该地区抗震设防烈度按__度，一般地下结构的地震反应宜采用反应位移法计算。"));
		MakeCompleteText(str, _T("   2.按__类人防工程设计，工程防核武器抗力为__级，防常规武器抗力级别6级，并采取相应的构造措施。地面冲击波超压峰值ΔPm=   MPa，\
在核爆动荷载作用下，其动力分析采用等效静荷载法，按弹塑性工作阶段设计，不验算结构变形和裂缝宽度。"));
		MakeCompleteText(str, _T("   3.地铁列车荷载考虑其冲击力，折算按等效均布荷载取20kPa。"));
		MakeText(Elems, str);

		{
			T_TABLE_D TableD;
			TableD.Initialize();
			//
			TableD.HeaderD.nHeaderRowCount = 1;
			TableD.HeaderD.nHeaderColCount = 3;
			TableD.HeaderD.aTitleName.Add(_T("荷载类型"));
			TableD.HeaderD.aTitleName.Add(_T("荷载名称"));
			TableD.HeaderD.aTitleName.Add(_T("荷载标准值"));

			TableD.RecordD.aRecordData.SetSize(11);
			auto& record = TableD.RecordD.aRecordData;
			for (int i = 0; i < 5; i++)
			{
				record[i].Add(m_strFormat + _T("永久荷载"));
				record[i].Add(m_strFormat + _T(""));
				record[i].Add(m_strFormat + _T(""));
			}
			for (int i = 0; i < 5; i++)
			{
				record[i+5].Add(m_strFormat + _T("基本可变荷载"));
				record[i+5].Add(m_strFormat + _T(""));
				record[i+5].Add(m_strFormat + _T(""));
			}
			record[10].Add(m_strFormat + _T("其他可变荷载"));
			record[10].Add(m_strFormat + _T(""));
			record[10].Add(m_strFormat + _T(""));

			TableD.RecordD.nRecordRowCount = TableD.RecordD.aRecordData.GetSize();
			TableD.RecordD.nRecordColCount = 3;
			MakeTable(Elems, TableD, _T("荷载类型"), _T("CHARPT_3_2_TBL1"));
		}
	}

	if (Para()->SegmentIsCheck(1000 * ARGC_SUBWAY + 3, 3))
	{
		MakeTitle(Elems, _T("荷载工况及组合"), 3, 3);

		CArray<T_STLD_K, T_STLD_K> rKeyList;
		T_STLD_D stldD;
		m_pDoc->m_pAttrCtrl->GetStldKeyList(rKeyList);

		{
			T_TABLE_D TableD;
			TableD.Initialize();
			//
			TableD.HeaderD.nHeaderRowCount = 1;
			TableD.HeaderD.nHeaderColCount = 4;
			TableD.HeaderD.aTitleName.Add(_T("编号"));
			TableD.HeaderD.aTitleName.Add(_T("名称"));
			TableD.HeaderD.aTitleName.Add(_T("类型"));
			TableD.HeaderD.aTitleName.Add(_T("说明"));

			TableD.RecordD.aRecordData.SetSize(rKeyList.GetSize());
			auto& record = TableD.RecordD.aRecordData;
			for (int i = 0; i < rKeyList.GetSize(); i++)
			{
				m_pDoc->m_pAttrCtrl->GetStld(rKeyList[i], stldD);
				str.Format(_T("%d"), rKeyList[i]);
				record[i].Add(m_strFormat + str);
				record[i].Add(m_strFormat + stldD.LoadCaseName);
				record[i].Add(m_strFormat + stldD.LoadCaseType);
				record[i].Add(m_strFormat + stldD.Description);
			}
			TableD.RecordD.nRecordRowCount = TableD.RecordD.aRecordData.GetSize();
			TableD.RecordD.nRecordColCount = 4;
			MakeTable(Elems, TableD, _T("荷载工况"), _T("CHARPT_3_3_TBL1"));
		}

		{
			T_KEY_LIST lstLoad;
			m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, lstLoad);

			T_TABLE_D TableD;
			TableD.Initialize();
			//
			TableD.HeaderD.nHeaderRowCount = 1;
			TableD.HeaderD.nHeaderColCount = 3;
			TableD.HeaderD.aTitleName.Add(_T("编号"));
			TableD.HeaderD.aTitleName.Add(_T("名称"));
			TableD.HeaderD.aTitleName.Add(_T("说明"));

			TableD.RecordD.aRecordData.SetSize(lstLoad.GetSize());
			auto& record = TableD.RecordD.aRecordData;

			T_LCOM_D LComD;
			for (int k = 0; k < lstLoad.GetCount(); k++)
			{
				const T_KEY nKey = lstLoad[k];
				if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, nKey, LComD))
				{
					ASSERT(FALSE);
					continue;
				}
				str.Format(_T("%d"), lstLoad[k]);
				record[k].Add(m_strFormat + str);
				record[k].Add(m_strFormat + LComD.LoadCombName);
				record[k].Add(m_strFormat + LComD.Description);
			}
			TableD.RecordD.nRecordRowCount = TableD.RecordD.aRecordData.GetSize();
			TableD.RecordD.nRecordColCount = 3;
			MakeTable(Elems, TableD, _T("荷载组合"), _T("CHARPT_3_3_TBL2"));
		}
	}

	if (Para()->SegmentIsCheck(1000 * ARGC_SUBWAY + 3, 4))
	{
		MakeTitle(Elems, _T("结构材料"), 3, 4);

		{
			CAutoRecoveryUnit unitSet(D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_FORCE_INDEX_KN, m_pDoc->m_pUnitCtrl);
			T_TABLE_D TableD;
			TableD.Initialize();
			TableD.HeaderD.nHeaderRowCount = 1;
			TableD.HeaderD.nHeaderColCount = 5;
			TableD.HeaderD.aTitleName.Add(_T("材料名称"));
			TableD.HeaderD.aTitleName.Add(_T("弹性模量(Mpa)"));
			TableD.HeaderD.aTitleName.Add(_T("泊松比"));
			TableD.HeaderD.aTitleName.Add(_T("线膨胀系数"));
			TableD.HeaderD.aTitleName.Add(_T("容重(KN/m3)"));

			CArray<T_MATL_K, T_MATL_K> rKeyList;
			m_pDoc->m_pAttrCtrl->GetMatlKeyListUsedInElem(rKeyList);
			int nSize = rKeyList.GetSize();
			int nConSize = 0;
			T_MATD_D data;
			CString str;
			for (int i = 0; i < nSize; i++)
			{
				m_pDoc->m_pAttrCtrl->GetMatlDesign(rKeyList[i], data);
				if (data.Type == _T("C"))
					nConSize++;
			}
			TableD.RecordD.aRecordData.SetSize(nConSize);
			auto& record = TableD.RecordD.aRecordData;
			for (int i = 0; i < nSize; i++)
			{
				m_pDoc->m_pAttrCtrl->GetMatlDesign(rKeyList[i], data);

				if (data.Type == _T("C"))
				{
					record[i].Add(data.Name);
					record[i].Add(Double2String_g(data.Data1.Analysis.Elast / 1000.));
					record[i].Add(Double2String_g(data.Data1.Analysis.Poisson));
					record[i].Add(Double2String_g(data.Data1.Analysis.Thermal));
					record[i].Add(Double2String_g(data.Data1.Analysis.Density));
				}
			}
			TableD.RecordD.nRecordRowCount = nConSize;
			TableD.RecordD.nRecordColCount = 5;
			MakeTable(Elems, TableD, _T("混凝土强度"), _T("CHARPT_3_4_TBL1"));
		}

		{
			T_DCRB_D dcrbD;
			T_MSRB_D msrbD;
			if (!m_pDoc->m_pAttrCtrl->GetDcrb(dcrbD))
			{
				dcrbD.Initialize();
			}
			if (!m_pDoc->m_pAttrCtrl->GetMsrb(msrbD))
			{
				msrbD.Initialize();
			}
			//
			T_TABLE_D TableD;
			TableD.Initialize();
			TableD.HeaderD.nHeaderRowCount = 0;
			TableD.HeaderD.nHeaderColCount = 4;

			CString strTitle[][2] = {
				{_T("顶板上层"),_T("顶板下层")},
				{_T("底板上层"),_T("底板下层")},
				{_T("壁板内侧"),_T("壁板外侧")},
				{_T("顶板梁外侧"),_T("顶板梁内测")},
				{_T("底板梁外侧"),_T("底板梁内测")},
				{_T("中板梁"),_T("中板")},
				{_T("柱"),_T("")},
			};
			auto CompValue = [&](double dValue)
			{
				if (dValue < 1e-7)
					return 30.0;
				else
					return dValue;
			};
			double dTemp[][2] = {
				{msrbD.dT_Slab_X,msrbD.dB_Slab_X },
				{msrbD.dT_Slab_X,msrbD.dB_Slab_X },
				{msrbD.dT_Slab_X,msrbD.dB_Slab_X },
				{CompValue(dcrbD.dT_Beam),CompValue(dcrbD.dB_Beam) },
				{CompValue(dcrbD.dT_Beam),CompValue(dcrbD.dB_Beam) },
				{CompValue(dcrbD.dT_Beam),CompValue(dcrbD.dB_Beam) },
				{CompValue(dcrbD.d0_Column),0.0},
			};
			TableD.RecordD.aRecordData.SetSize(7);
			auto& record = TableD.RecordD.aRecordData;
			for (int i = 0; i < 7; i++)
			{
				record[i].Add(m_strFormat + strTitle[i][0]);
				record[i].Add(m_strFormat + Double2String_g(dTemp[i][0]));
				if (i != 6)
				{
					record[i].Add(m_strFormat + strTitle[i][1]);
					record[i].Add(m_strFormat + Double2String_g(dTemp[i][1]));
				}
			}
			TableD.RecordD.nRecordRowCount = 7;
			TableD.RecordD.nRecordColCount = 4;
			MakeTable(Elems, TableD, _T("砼保护层厚度（单位：mm）"), _T("CHARPT_3_4_TBL2"));
		}

		MakeText(Elems, _T("    裂缝宽度允许值：_______mm。"));

		{
			CAutoRecoveryUnit unitSet(D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_FORCE_INDEX_KN, m_pDoc->m_pUnitCtrl);
			T_TABLE_D TableD;
			TableD.Initialize();
			T_MERGE_UNIT_D merge; merge.Initialize();
			merge.nMergeCount = 2; merge.nStartColNumber = 1; merge.nStartRowNumber = 1;
			TableD.RecordD.aMergeRowD.Add(merge);
			//
			CArray<T_MATL_K, T_MATL_K> rKeyList;
			m_pDoc->m_pAttrCtrl->GetMatlKeyListUsedInElem(rKeyList);
			int nSize = rKeyList.GetSize();
			int nConSize = 0, nStlSize = 0;
			T_MATD_D data;
			CString str;
			for (int i = 0; i < nSize; i++)
			{
				m_pDoc->m_pAttrCtrl->GetMatlDesign(rKeyList[i], data);
				if (data.Type == _T("C"))
					nConSize++;
			}
			//
			TableD.HeaderD.nHeaderRowCount = 1;
			TableD.HeaderD.nHeaderColCount = 4;
			TableD.HeaderD.aTitleName.Add(_T("混凝土名称"));
			TableD.HeaderD.aTitleName.Add(_T("钢筋类型"));
			TableD.HeaderD.aTitleName.Add(_T("钢筋等级"));
			TableD.HeaderD.aTitleName.Add(_T("屈服强度(Mpa)"));

			TableD.RecordD.aRecordData.SetSize(nConSize * 2);
			auto& record = TableD.RecordD.aRecordData;
			for (int i = 0; i < nSize; i++)
			{
				m_pDoc->m_pAttrCtrl->GetMatlDesign(rKeyList[i], data);
				if (data.Type == _T("C"))
				{
					record[i * 2].Add(m_strFormat + data.Name);
					record[i * 2].Add(m_strFormat + _T("主筋"));
					str = data.MainRebar_RebarName;
					record[i * 2].Add(m_strFormat + (str.IsEmpty() ? _T("-") : str));
					record[i * 2].Add(m_strFormat + (str.IsEmpty() ? _T("-") : Double2String_g(data.MainRebarData.B_fy / 1000.0)));
					//record[i*2].Add(_T(""));
					record[i * 2].Add(m_strFormat + _T("箍筋"));
					str = data.SubRebar_RebarName;
					record[i * 2].Add(m_strFormat + (str.IsEmpty() ? _T("-") : str));
					record[i * 2].Add(m_strFormat + (str.IsEmpty() ? _T("-") : Double2String_g(data.SubRebarData.B_fy / 1000.0)));
				}
			}
			TableD.RecordD.nRecordRowCount = TableD.RecordD.aRecordData.GetSize();
			TableD.RecordD.nRecordColCount = 4;
			MakeTable(Elems, TableD, _T("设计用钢筋参数"), _T("CHARPT_3_4_TBL3"));
		}
	}

	m_Segments.push_back(Elems);
}
void AutoReport_Gen_CH_SUBWAY::WriteCharpt4()
{
	if (Para()->SegmentIsCheck(1000 * ARGC_SUBWAY + 4, 0) == FALSE)
		return;
	GenSegmentElements Elems;
	MakeTitle(Elems, _T("断面内力计算与设计"), 4, 0);
	WriteCharpt4_1(Elems);
	if (Para()->SegmentIsCheck(1000 * ARGC_SUBWAY + 4, 2))
	{
		MakeTitle(Elems, _T("计算参数"), 4, 2);
		CString str;
		str.Format(_T("    %s"), GetBold(_T("#用户自行输入#")));
		MakeText(Elems, str);
	}
	WriteCharpt4_3(Elems);
	m_Segments.push_back(Elems);
}
void AutoReport_Gen_CH_SUBWAY::WriteCharpt4_1(GenSegmentElements &Elems)
{
	if (Para()->SegmentIsCheck(1000 * ARGC_SUBWAY + 4, 1) == FALSE)
		return;
	MakeTitle(Elems, _T("模型计算简图"), 4, 1);
	
	{
		//
		CArray<T_GRUP_K, T_GRUP_K> aGroupK;
		T_GRUP_D groupD;
		{
			Report_Key_LIST aGroupKey;
			Para()->GroupKeys(aGroupKey, 1000 * ARGC_SUBWAY);
			for (int i = 0; i < aGroupKey.GetSize(); i++)
				aGroupK.Add(aGroupKey[i].m_nKey);
		}

		GDisplaySetting dspOpt;
		ExportDisplaySetting::GET(&dspOpt);
		dspOpt.m_Node.SetDefault();
		dspOpt.m_Node.bNode = FALSE;
		dspOpt.m_Elem.SetDefault();
		dspOpt.m_Prop.SetDefault();
		dspOpt.m_Bndr.SetDefault();
		dspOpt.m_Load.SetDefault();
		ExportDisplaySetting::SET(&dspOpt);
		int nIndex = 0;
		CString strBook;
		CString strGroupName;
		CString str;
		T_STLD_D stldD;
		for (int j = 0; j < aGroupK.GetSize(); j++)
		{
			if (aGroupK[j] != 0 && !m_pDoc->m_pAttrCtrl->GetGrup(aGroupK[j], groupD))
				continue;
			T_KEY_LIST  EKeyArray;
			if (aGroupK[j] == 0)
			{
				strGroupName = _T("全部");
				ActiveElemAll();
				m_pDoc->m_pAttrCtrl->GetElemKeyList(EKeyArray);
			}
			else
			{
				strGroupName = groupD.GroupName;
				ActiveObj(groupD.arKeyElem);
				EKeyArray.Copy(groupD.arKeyElem);
			}
			AdjustSlabView(EKeyArray);

			str.Format(_T("%s_计算简图"), strGroupName);
			MakeImage_Current(str, FALSE);
			MakePicture(Elems, str, strBook);
		}
	}
}
void AutoReport_Gen_CH_SUBWAY::WriteCharpt4_3(GenSegmentElements &Elems)
{
	if (Para()->SegmentIsCheck(1000 * ARGC_SUBWAY + 4, 3) == FALSE)
		return;
	MakeTitle(Elems, _T("断面内力及配筋计算"), 4, 3);
	CString str;
	str.Format(_T("    %s"), GetBold(_T("#用户自行输入#")));
	MakeText(Elems, str);
	
}
void AutoReport_Gen_CH_SUBWAY::WriteCharpt5()
{
	if (Para()->SegmentIsCheck(1000 * ARGC_SUBWAY + 5, 0) == FALSE)
		return;
	GenSegmentElements Elems;
	CString str;
	MakeTitle(Elems, _T("纵梁与柱的计算"), 5, 0);
	WriteCharpt5_1(Elems);
	if (Para()->SegmentIsCheck(1000 * ARGC_SUBWAY + 5, 2))
	{
		MakeTitle(Elems, _T("框架柱设计"), 5, 2);
		str.Format(_T("    %s"), GetBold(_T("#用户自行输入#")));
		MakeText(Elems, str);
	}
	m_Segments.push_back(Elems);
}
void AutoReport_Gen_CH_SUBWAY::WriteCharpt5_1(GenSegmentElements &Elems)
{
	if (Para()->SegmentIsCheck(1000 * ARGC_SUBWAY + 5, 1) == FALSE)
		return;
	MakeTitle(Elems, _T("纵梁计算"), 5, 1);
	Report_Key_LIST aKeys;
	Para()->ListReportOption(aKeys, ARGC_SUBWAY);
	if (aKeys.IsEmpty())
		return;
	Report_Key_LIST aReportKeys;
	{
		for (int i = 0; i < aKeys.GetSize(); i += 4)
		{
			if (aKeys[i].m_nKey != 4)
				continue;
			for (int j = 0; j < 4; j++)
				aReportKeys.Add(aKeys[i + j]);
		}
		if (aReportKeys.IsEmpty())
			return;
	}

	int nForceType[] = { 0,1,2,3,4,5 };
	CString strBForceType[] = { _T("FX"),_T("FY"),_T("FZ"),_T("MX"),_T("MY"),_T("MZ") };
	CString strRltName, strGroup, strDir, strLoad,strBook;
	T_GRUP_D GrupD;
	CString str;
	CViewBase* pView = static_cast<CViewBase*>(CViewBase::GetCurView_ST());
	I_GPSModel* gm = pView->GetIPM();

	BOOL bLoadDataExist = gm->m_bLoadDataExist;
	BOOL bLoadDataSet = gm->m_bLoadDataSet;
	BOOL bContour = gm->m_bContour;
	BOOL bDrawLegend = gm->m_DOPT.LG.m_bDrawLegend;
	BOOL m_bDeformed = gm->m_bDeformed;
	BOOL m_bAnimation = gm->m_bAnimation;
	
	gm->m_bLoadDataExist = TRUE;
	gm->m_bLoadDataSet = TRUE;
	gm->m_bContour = TRUE;
	gm->m_DOPT.LG.m_bDrawLegend = TRUE;//云图
	gm->m_bDeformed = FALSE;//是否变形
	gm->m_bAnimation = FALSE;
	//
	gm->m_DOPT.DR.m_bDrawInactive = FALSE;
	strRltName = _T("梁单元内力");
	int nIndex = 0;
	SetViewPoint(0);
	for (int i = 0; i < aReportKeys.GetSize(); i += 4)
	{
		//
		gm->m_GPSMode = gm->m_GPSInstruction = GPS_COMMAND_BFORC;
		gm->m_LoadCaseType = aReportKeys[i+3].m_nType;
		gm->m_LoadCaseKey = aReportKeys[i+3].m_nKey;
		gm->m_LoadMinMaxType = aReportKeys[i+3].m_nSubType;

		CForceEngine ForceD(gm);
		CDeformEngine DeformD(gm);

		CForceEngine* pForceD = gm->m_pForceEngine;
		CDeformEngine* pDeformD = gm->m_pDeformEngine;
		if (!pForceD)
			pForceD = &CGPSBarContainer::m_ForceEngine;
		if (!pDeformD)
			pDeformD = &CGPSBarContainer::m_DeformEngine;
		if (!pForceD->m_pContourEngine)
			pForceD->m_pContourEngine = &CGPSBarContainer::m_ContourEngine;
		if (!pDeformD->m_pContourEngine)
			pDeformD->m_pContourEngine = &CGPSBarContainer::m_ContourEngine;
		ForceD.ImportEngine(pForceD);
		DeformD.ImportEngine(pDeformD);

		ForceD.m_BForcComponent = nForceType[aReportKeys[i+2].m_nKey];
		ForceD.m_TForcComponent = aReportKeys[i+2].m_nKey;//桁架单元默认全部内力
		ForceD.m_bYieldPoint = FALSE;//Yield Point
		ForceD.m_bBForcWithTForc = FALSE;//Show Truss Forces
		ForceD.m_nBForcPscPart = 0;//Part
		ForceD.m_bDeformedShapeContour = FALSE;
		ForceD.m_bBForcOutputMax = TRUE;
		ForceD.m_bBForcOutputMinMax = FALSE;
		ForceD.m_bBForcOutputAll = FALSE;
		ForceD.m_bBForcOutputI = FALSE;
		ForceD.m_bBForcOutputC = FALSE;
		ForceD.m_bBForcOutputJ = FALSE;
		ForceD.m_bBForcOutputByMember = FALSE;
		DeformD.m_nThisComp = 0;
		DeformD.m_bContourUpdate = TRUE;
		ForceD.InitEngine();
		DeformD.InitEngine();//荷载组合变化时初始化
		//DeformD.m_DispComponent = nDisp[aReportKeys[2].m_nKey];
		DeformD.m_bResultDataSet = TRUE;
		pForceD->m_bBForcDataSet = TRUE;
		//
		int nMinMax = 0;
		if (aReportKeys[i+3].m_nSubType == LOAD_MAX)
			nMinMax = 1;
		else if (aReportKeys[i+3].m_nSubType == LOAD_MIN)
			nMinMax = -1;
		strLoad = CDBLib::GetLoadCaseNameByKey(aReportKeys[i+3].m_nType, aReportKeys[i+3].m_nKey, nMinMax);
		int nIndexForce = aReportKeys[i+2].m_nKey;
		
		if (nIndexForce > 5)
		{
			ASSERT(FALSE);
			continue;
		}
		strDir = strBForceType[nIndexForce];

		gm->m_bInitialView = FALSE;
		gm->ResetEngines();
		gm->m_pForceEngine = new CForceEngine(gm);
		gm->m_pForceEngine->ImportEngine(&ForceD);
		gm->m_pDeformEngine = new CDeformEngine(gm);
		gm->m_pDeformEngine->ImportEngine(&DeformD);

		if (m_pDoc->m_pAttrCtrl->GetGrup(aReportKeys[i+1].m_nKey, GrupD))
		{
			strGroup = GrupD.GroupName;
			ActiveObj(GrupD.arKeyElem);
		}
		else
		{
			VERIFY(aReportKeys[i+1].m_nKey == 0);
			strGroup = _T("全部");
			ActiveElemAll();
		}
		str.Format(_T("%s_%s_%s_%s"), strRltName, strGroup, strDir, strLoad);
		Replace_StrPath(str);
		gm->m_pGPSCtrl->XGR_SetRebuildGeom(TRUE);
		gm->ResetLegendRect();
		gm->m_pGPSCtrl->SetShowVPointIcon(0);
		CViewBase::GetCurView_ST()->RestoreFrameTitle();
		gm->m_pGPSCtrl->UpdateAllWcsVcsMBR();
		gm->m_InvalidateFlag = TRUE;
		MakeImage_Current(str, FALSE);
		strBook.Format(_T("CHARPT_5_1_FIG_%d"), ++nIndex);
		MakePicture(Elems, str, strBook);
	}
	gm->m_GPSMode = gm->m_GPSInstruction = GPS_COMMAND_NONE;
	gm->m_bLoadDataExist = bLoadDataExist;
	gm->m_bLoadDataSet = bLoadDataSet;
	gm->m_bContour = bContour;
	gm->m_DOPT.LG.m_bDrawLegend = bDrawLegend;
	gm->m_bDeformed = m_bDeformed;
	gm->m_bAnimation = m_bAnimation;
}
void AutoReport_Gen_CH_SUBWAY::WriteCharpt6()
{
	if (Para()->SegmentIsCheck(1000 * ARGC_SUBWAY + 6, 0) == FALSE)
		return;
	GenSegmentElements Elems;
	CString str;
	MakeTitle(Elems, _T("端头井段内力计算与设计"), 6, 0);

	str.Format(_T("    %s"), _T("采用midas Gen<2021 V2.1版本>软件计算，采用梁单元模拟梁、柱；板单元模拟侧墙、各板。梁、板单元的截面特性及材料按结构实际取值，节点位置取结构中心点。单元按1m一个单元建模。"));
	MakeText(Elems, str);
	//
	str.Format(_T("    %s"), GetBold(_T("    计算模型如下图所示：")));
	MakeText(Elems, str);
	{
		CViewBase::GetCurView_ST()->DoInitialView();
		//
		CArray<T_GRUP_K, T_GRUP_K> aGroupK;
		T_GRUP_D groupD;
		{
			Report_Key_LIST aGroupKey;
			Para()->GroupKeys(aGroupKey, 1000 * ARGC_SUBWAY);
			for (int i = 0; i < aGroupKey.GetSize(); i++)
				aGroupK.Add(aGroupKey[i].m_nKey);
		}

		GDisplaySetting dspOpt;
		ExportDisplaySetting::GET(&dspOpt);


		dspOpt.m_Elem.SetDefault();
		dspOpt.m_Prop.SetDefault();
		dspOpt.m_Bndr.SetDefault();
		dspOpt.m_Load.SetDefault();
		ExportDisplaySetting::SET(&dspOpt);

		int nIndex = 0;
		CString strBook;
		CString strGroupName;
		CString str;
		T_STLD_D stldD;
		for (int j = 0; j < aGroupK.GetSize(); j++)
		{
			if (aGroupK[j] != 0 && !m_pDoc->m_pAttrCtrl->GetGrup(aGroupK[j], groupD))
				continue;
			T_KEY_LIST  EKeyArray;
			if (aGroupK[j] == 0)
			{
				strGroupName = _T("全部");
				ActiveElemAll();
				m_pDoc->m_pAttrCtrl->GetElemKeyList(EKeyArray);
			}
			else
			{
				strGroupName = groupD.GroupName;
				ActiveObj(groupD.arKeyElem);
				EKeyArray.Copy(groupD.arKeyElem);
			}
			AdjustSlabView(EKeyArray);

			str.Format(_T("%s"), strGroupName);
			Replace_StrPath(str);
			strBook.Format(_T("CHARPT_6_0_FIG_%d"), ++nIndex);
			MakeImage_Current(str, FALSE);
			MakePicture(Elems, str, strBook);
		}
	}
	
	WriteCharpt6_Force(Elems);
	WriteCharpt6_Dgn(Elems);
	WriteCharpt6_Serv(Elems);
	m_Segments.push_back(Elems);
}
void AutoReport_Gen_CH_SUBWAY::WriteCharpt6_Force(GenSegmentElements &Elems)
{
	if (Para()->SegmentIsCheck(1000 * ARGC_SUBWAY + 6, 1) == FALSE)
		return;
	MakeTitle(Elems, _T("板内力计算结果"), 6, 1);
	//-----------------------------------------------------------------------//
	Report_Key_LIST aKeys;
	Para()->ListReportOption(aKeys, ARGC_SUBWAY);
	if (aKeys.IsEmpty())
		return;
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
	gm->m_bLoadDataExist = TRUE;
	gm->m_bLoadDataSet = TRUE;
	gm->m_bContour = TRUE;
	gm->m_DOPT.LG.m_bDrawLegend = TRUE;//云图
	gm->m_bDeformed = FALSE;//是否变形
	gm->m_bAnimation = FALSE;
	gm->m_DOPT.DR.m_bDrawInactive = FALSE;
	gm->m_DOPT.TX.m_bOutputNumber = FALSE;//是否显示数值
	gm->m_DOPT.TX.m_bNumberExponent = FALSE;//指数
	gm->m_bMdShpContour = TRUE;//Model
	gm->m_bAnimation = FALSE;//动画模式,是否随动变换等值线
	if (gm == NULL)
		return;
	//-----------------------------------------------------------------------//
	//int nForceType[] = { 0,1,2,3,4,5 };
	int nForce[] = { PFORC_COMPONENT_FXX,PFORC_COMPONENT_FYY,PFORC_COMPONENT_FXY,PFORC_COMPONENT_FMX,PFORC_COMPONENT_FMN,PFORC_COMPONENT_FMX_ABS,PFORC_COMPONENT_MXX,PFORC_COMPONENT_MYY,PFORC_COMPONENT_MXY,
		PFORC_COMPONENT_MMX,PFORC_COMPONENT_MMN,PFORC_COMPONENT_MMX_ABS,PFORC_COMPONENT_VXX,PFORC_COMPONENT_VYY,PFORC_COMPONENT_VMX_ABS,PFORC_COMPONENT_FVT,PFORC_COMPONENT_MVT };
	CString strRltTypeName[] = { _T("板单元内力(中心值)"),_T("板单元内力(最大值)") };
	CString strForce[] = { _T("Fxx"),_T("Fyy"),_T("Fxy"),_T("Fmax"),_T("Fmin"),_T("FMax"),_T("Mxx"),_T("Myy"),_T("Mxy"),_T("Mmax"),_T("Mmin"),_T("MMax"),_T("Vxx"),_T("Vyy"),_T("VMax"),_T("主轴力"),_T("主弯矩") };
	CString strRltName, strGroup, strDir, strLoad, strBook;

	T_GRUP_D GrupD;
	CString str;
	gm->m_GPSMode = gm->m_GPSInstruction = GPS_COMMAND_PFORC;
	CForceEngine ForceD(gm);
	CDeformEngine DeformD(gm);
	if (gm->m_pForceEngine)
		ForceD.ImportEngine(gm->m_pForceEngine);
	if (gm->m_pDeformEngine)
		DeformD.ImportEngine(gm->m_pDeformEngine);
	//内力
	int nIndex = 0;
	for (int i = 0; i < aKeys.GetSize(); i += 4)
	{
		if (!(aKeys[i].m_nKey == 0 || aKeys[i].m_nKey == 1))
			continue;
		//
		ForceD.m_PForcComponent = nForce[aKeys[i + 2].m_nKey];
		ForceD.m_TForcComponent = aKeys[i + 2].m_nKey;//桁架单元默认全部内力
		ForceD.m_bYieldPoint = FALSE;//Yield Point
		ForceD.m_bBForcWithTForc = FALSE;//Show Truss Forces
		ForceD.m_nBForcPscPart = 0;//Part
		ForceD.m_bDeformedShapeContour = FALSE;
		ForceD.m_bBForcOutputMax = TRUE;
		ForceD.m_bBForcOutputMinMax = FALSE;
		ForceD.m_bBForcOutputAll = FALSE;
		ForceD.m_bBForcOutputI = FALSE;
		ForceD.m_bBForcOutputC = FALSE;
		ForceD.m_bBForcOutputJ = FALSE;
		ForceD.m_bBForcOutputByMember = FALSE;
		DeformD.m_nThisComp = 0;
		DeformD.m_bContourUpdate = TRUE;
		gm->m_OptAvg = PSTRS_VALUE_ELEMENT;
		gm->m_nValOpt = 1- aKeys[i].m_nKey;//0 单元中心值 1 单元最大值 
		gm->m_bElemCenter = aKeys[i].m_nKey == 0 ? TRUE : FALSE;
		gm->m_bInitialView = FALSE;
		gm->ResetEngines();
		gm->m_pForceEngine = new CForceEngine(gm);
		gm->m_pDeformEngine = new CDeformEngine(gm);

		gm->m_LoadCaseType = aKeys[i + 3].m_nType;
		gm->m_LoadCaseKey = aKeys[i + 3].m_nKey;
		gm->m_LoadMinMaxType = aKeys[i + 3].m_nSubType;

		ForceD.InitEngine();
		DeformD.InitEngine();//荷载组合变化时初始化
		DeformD.m_DispComponent = 0;//
		DeformD.m_bResultDataSet = TRUE;

		strRltName = strRltTypeName[aKeys[i].m_nKey];
		int nMinMax = 0;
		if (aKeys[i + 3].m_nSubType == LOAD_MAX)
			nMinMax = 1;
		else if (aKeys[i + 3].m_nSubType == LOAD_MIN)
			nMinMax = -1;
		strLoad = CDBLib::GetLoadCaseNameByKey(aKeys[i + 3].m_nType, aKeys[i + 3].m_nKey, nMinMax);
		int nIndexForce = aKeys[i + 2].m_nKey;

		gm->m_pDeformEngine->ImportEngine(&DeformD);
		gm->m_pForceEngine->ImportEngine(&ForceD);
		//
		T_KEY_LIST  EKeyArray;
		if (m_pDoc->m_pAttrCtrl->GetGrup(aKeys[i + 1].m_nKey, GrupD))
		{
			strGroup = GrupD.GroupName;
			ActiveObj(GrupD.arKeyElem);
			EKeyArray.Copy(GrupD.arKeyElem);
		}
		else
		{
			VERIFY(aKeys[1].m_nKey == 0);
			strGroup = _T("全部");
			ActiveElemAll();
			m_pDoc->m_pAttrCtrl->GetElemKeyList(EKeyArray);
		}
		AdjustSlabView(EKeyArray);

		strDir = strForce[nIndexForce];
		str.Format(_T("%s_%s_%s_%s"), strRltName, strGroup, strDir, strLoad);
		Replace_StrPath(str);
		gm->m_pGPSCtrl->XGR_SetRebuildGeom(TRUE);
		gm->ResetLegendRect();
		gm->m_pGPSCtrl->SetShowVPointIcon(0);
		CViewBase::GetCurView_ST()->RestoreFrameTitle();
		gm->m_pGPSCtrl->UpdateAllWcsVcsMBR();
		gm->m_InvalidateFlag = TRUE;
		MakeImage_Current(str, FALSE);
		strBook.Format(_T("CHARPT_6_1_FIG_%d"), ++nIndex);
		MakePicture(Elems, str, strBook);
	}
	gm->m_GPSMode = gm->m_GPSInstruction = GPS_COMMAND_NONE;
}
void AutoReport_Gen_CH_SUBWAY::WriteCharpt6_Dgn(GenSegmentElements &Elems)
{
	if (Para()->SegmentIsCheck(1000 * ARGC_SUBWAY + 6, 2) == FALSE)
		return;
	MakeTitle(Elems, _T("板配筋计算结果"), 6, 2);
	//-----------------------------------------------------------------------//
	Report_Key_LIST aKeys;
	Para()->ListReportOption(aKeys, ARGC_SUBWAY);
	if (aKeys.IsEmpty())
		return;
	Report_Key_LIST aReportKeys[2];
	{
		T_MLCB_D MlcbD; MlcbD.Initialize();
		CDBDoc *pDoc = CDBDoc::GetDocPoint();
		if (!pDoc->m_pAttrCtrl->Get_DefaultMlcb(MlcbD))
			MlcbD.Initialize();
		if (MlcbD.aSlabStreK.IsEmpty())
			return;
		BOOL bInValid = TRUE;
		auto ValidComb = [&](int key)
		{
			for (int i = 0; i < MlcbD.aSlabStreK.GetSize(); i++)
			{
				if (MlcbD.aSlabStreK[i] == key)
				{
					return TRUE;
				}
			}
			return FALSE;
		};
		for (int i = 0; i < aKeys.GetSize(); i += 4)
		{
			if (aKeys[i].m_nKey != 2)
				continue;
			if (aKeys[i + 3].m_nKey != 0 && !ValidComb(aKeys[i + 3].m_nKey))
				continue;
			for (int j = 0; j < 4; j++)
				aReportKeys[aKeys[i + 2].m_nKey].Add(aKeys[i + j]);
		}
		if (aReportKeys[0].IsEmpty() && aReportKeys[1].IsEmpty())
			return;
	}

	//
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
	//
	CRCSCodeCheck* pCodeCheck = new CRCSCodeCheck;
	CRCSDataBase* pDataBase = new CRCSDataBase;
	pDataBase->m_iRcsTypeKind = RCS_SDGN;//81;
	pDataBase->m_bSlabDesignCheck = TRUE;
	pDataBase->m_nTopBottom = 2;//0 板顶 1 板底 2 顶底
	pDataBase->m_nCellOption = 0;//
	pDataBase->m_nLegendType = 1;//  0: Rebar 1:As Value, 2:ratio_value //计算配筋
	pDataBase->m_bOneWayMomentCheck = FALSE;//m_bCuttingLine;
	pDataBase->m_nCuttingDiagramLeftRight = 0;//m_nCutPosi
	pDataBase->m_dCellSize = 0.0;
	pDataBase->m_nCrackSpec = 0;//
	pDataBase->m_bCrackCalc = FALSE;//
	pDataBase->m_bShowRebarType = FALSE;//
	pDataBase->m_dCrackLim = 0.0;//
	//
	pCodeCheck->Set_MyDBPointer(pDataBase);
	pCodeCheck->m_pDoc = m_pDoc;
	pCodeCheck->Set_DgnCtrlDataforCodeCheck();
	std::vector<T_SBDO_K> aSbdoKeys;
	int nSbdo = pCodeCheck->m_pMyDB->m_pDgnDataCtrl->Get_DgnConSbdoKList(EN_DGNSBDO_SLABMAT, aSbdoKeys);
	if (nSbdo == 0)
		return;

	if (gm == NULL)
		return;
	gm->m_LegendEngine.InitFont(20 * dFontRatio, 14 * dFontRatio);
	gm->m_LoadMinMaxType = LOADTYPE_NORMAL;
	gm->m_bLoadDataExist = TRUE;
	gm->m_bLoadDataSet = TRUE;
	gm->m_bContour = TRUE;
	gm->m_DOPT.LG.m_bDrawLegend = TRUE;//云图
	gm->m_bDeformed = FALSE;//是否变形
	gm->m_bAnimation = FALSE;
	gm->m_DOPT.DR.m_bDrawInactive = FALSE;
	gm->m_DOPT.TX.m_bOutputNumber = FALSE;//是否显示数值
	gm->m_DOPT.TX.m_bNumberExponent = FALSE;//指数
	gm->m_bMdShpContour = TRUE;//Model
	gm->m_bAnimation = FALSE;//动画模式,是否随动变换等值线
	gm->m_bInitialView = FALSE;
	gm->m_GPSMode = gm->m_GPSInstruction = GPS_COMMAND_SLAB_DGN;

	//-----------------------------------------------------------------------//
	CDesignEngine DesignD(gm);
	DesignD.ResetEngine();
	if (gm->m_pDsgnEngine)
		DesignD.ImportEngine(gm->m_pDsgnEngine);
	if (!DesignD.m_pContourEngine)
		DesignD.m_pContourEngine = &CGPSBarContainer::m_ContourEngine;
	DesignD.m_nRebarRatio = 1;//计算配筋
	DesignD.m_nPosi = pDataBase->m_nTopBottom;
	DesignD.m_bSlabDgnResult = TRUE;
	gm->ResetLegendRect();
	gm->ResetEngines();
	gm->m_pDsgnEngine = new CDesignEngine(gm);
	gm->m_pDsgnEngine->ImportEngine(&DesignD);
	//-----------------------------------------------------------------------//
	for (int k = 0; k < 2; k++)
	{
		if (aReportKeys[k].IsEmpty())
			continue;
		//
		T_TABLE_D TableD;
		TableD.Initialize();
		//
		{
			TableD.HeaderD.nHeaderRowCount = 1;
			TableD.HeaderD.nHeaderColCount = 11;
			TableD.HeaderD.aTitleName.Add(_T("水池区域"));
			TableD.HeaderD.aTitleName.Add(_T("板厚(mm)"));
			TableD.HeaderD.aTitleName.Add(_T("位置"));
			TableD.HeaderD.aTitleName.Add(_T("单元号"));
			TableD.HeaderD.aTitleName.Add(_T("荷载组合"));
			TableD.HeaderD.aTitleName.Add(_T("内力M"));
			TableD.HeaderD.aTitleName.Add(_T("内力N"));
			TableD.HeaderD.aTitleName.Add(_T("计算As"));
			TableD.HeaderD.aTitleName.Add(_T("实配As"));
			TableD.HeaderD.aTitleName.Add(_T("实际配筋"));
			TableD.HeaderD.aTitleName.Add(_T("配筋率(%)"));
			//size: top dir1;top dir2;bot dir1,bot dir2;
			TableD.RecordD.aRecordData.SetSize(aReportKeys[k].GetCount() * nSbdo);
		}
		int nIndex = 0;
		T_SBDO_D SbdoD;
		T_ELEM_K_LIST aSbdoElem;
		CString strDir, strLoad, strBook, str;
		CString strAverage[] = { _T("单元中心值"),_T("单元最大值") };
		CStringArray aStrBook, aStrPath;
		for (int i = 0; i < aReportKeys[k].GetSize(); i += 4)
		{
			//
			pDataBase->m_bAverage = aReportKeys[k][i + 2].m_nKey;//m_nElemAvg;//0 中心值 1 最大值
			pDataBase->m_nSelectLcomK = aReportKeys[k][i + 3].m_nKey;//m_lckey;
			gm->m_LoadCaseType = aReportKeys[k][i + 3].m_nType;
			gm->m_LoadCaseKey = aReportKeys[k][i + 3].m_nKey;
			pDataBase->m_nSlabDsgDir = 0;
			pDataBase->m_PForcComponent = pDataBase->m_nSlabDsgDir == 0 ? 6 : 7;
			//All components are activated for calculation, and each area is activated separately
			ActiveElemAll();
			pCodeCheck->Check_Code(0);
			//
			if (aReportKeys[k][i + 3].m_nKey == 0)
			{
				strLoad = _T("组合");
				gm->m_LoadNameStr = _LS(IDS_DB_ALL_COMBINATION);
			}
			else
				strLoad = CDBLib::GetLoadCaseNameByKey(aReportKeys[k][i + 3].m_nType, aReportKeys[k][i + 3].m_nKey, aReportKeys[k][i + 3].m_nSubType);
			DesignD.m_nElemAvg = pDataBase->m_bAverage;
			//
			for (int m = 0; m < nSbdo; m++)
			{
				const T_SBDO_K SbdoK = aSbdoKeys[m];
				m_pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD);
				int nSbdoElem = m_pDoc->m_pAttrCtrl->GetElemListSbdoK(SbdoK, aSbdoElem);
				if (nSbdoElem == 0)
					continue;
				ActiveObj(aSbdoElem);
				AdjustSlabView(aSbdoElem);

				for (int n = 0; n < 2; n++)
				{
					strDir = n == 0 ? _T("方向1") : _T("方向2");
					str.Format(_T("%s_%s_%s_%s配筋云图"), SbdoD.strName, strDir, strAverage[(int)pDataBase->m_bAverage], strLoad);
					Replace_StrPath(str);
					//
					DesignD.m_nXDirYDir = n;//方向
					gm->m_pDsgnEngine->ImportEngine(&DesignD);
					gm->m_pDsgnEngine->InitEngine();
					//
					gm->m_pGPSCtrl->XGR_SetRebuildGeom(TRUE);
					gm->ResetLegendRect();
					gm->m_pGPSCtrl->SetShowVPointIcon(0);
					CViewBase::GetCurView_ST()->RestoreFrameTitle();
					gm->m_pGPSCtrl->UpdateAllWcsVcsMBR();
					gm->m_InvalidateFlag = TRUE;
					MakeImage_Current(str, FALSE);
					WriteCharpt6_Dgn_TableMaxElemInSbdo(TableD.RecordD.aRecordData, SbdoD, aSbdoElem, nIndex, strLoad, n,pCodeCheck);
					strBook.Format(_T("CHARPT_6_2_FIG_%d"), ++nIndex);
					aStrBook.Add(strBook);
					aStrPath.Add(str);
				}
			}
		}

		TableD.RecordD.nRecordRowCount = TableD.RecordD.aRecordData.GetSize();
		TableD.RecordD.nRecordColCount = 11;
		str = strAverage[k] + _T("单位 内力 M KN*m/m N kN/m; 配筋面积 cm^2/m");
		MakeText(Elems, str);
		if (k == 0)
			MakeTable(Elems, TableD, _T("单元中心值配筋结果"), _T("CHARPT_6_Dgn_TBL1"));
		else
			MakeTable(Elems, TableD, _T("单元最大值配筋结果"), _T("CHARPT_6_Dgn_TBL2"));
		for (int i = 0; i < aStrPath.GetSize(); i++)
		{
			MakePicture(Elems, aStrPath[i], aStrBook[i]);
		}
	}
	gm->m_GPSMode = gm->m_GPSInstruction = GPS_COMMAND_NONE;
}
void AutoReport_Gen_CH_SUBWAY::WriteCharpt6_Dgn_TableMaxElemInSbdo(CArray<CStringArray, CStringArray &>& recond, const T_SBDO_D& SbdoD, const T_ELEM_K_LIST& aSbdoElem,
	int nIndex, const CString& strLoad, int nDir, CRCSCodeCheck* pCodeCheck)
{
	T_RSL2_D slabMaxRes, slabRes;
	T_ELEM_K aElemk[2];//for get maxSlab thicnkess
	double dCover[2] = { 0.0 };
	slabMaxRes.Initialize();
	for (int i = 0; i < aSbdoElem.GetSize(); i++)
	{
		if (!m_pDoc->m_pPostCtrl->GetDesignResult()->Get_2waySlabResult(aSbdoElem[i], nDir, slabRes))
		{
			ASSERT(FALSE);
			continue;
		}
		if (slabMaxRes.Top[0].dAsReq < slabRes.Top[0].dAsReq)
		{
			aElemk[0] = aSbdoElem[i];
			slabMaxRes.Top[0] = slabRes.Top[0];
			dCover[0] = slabRes.dT;
		}
		if (slabMaxRes.Bot[0].dAsReq < slabRes.Bot[0].dAsReq)
		{
			aElemk[1] = aSbdoElem[i];
			slabMaxRes.Bot[0] = slabRes.Bot[0];
			dCover[1] = slabRes.dB;
		}

	}
	T_SLAB_POS aPos[2] = { slabMaxRes.Top[0],slabMaxRes.Bot[0] };
	T_ELEM_D ElemD;
	double dThik = 0.0;

	CString strPos[2] = { _T("板顶"),_T("板底") };
	CString strDir = nDir == 0 ? _T("(方向一)") : _T("(方向二)");
	CArray<T_RBMS_D, T_RBMS_D&>aRbmsD;
	for (int i = 0; i < 2; i++)
	{
		m_pDoc->m_pAttrCtrl->GetElem(aElemk[i], ElemD);
		if (!m_pDoc->m_pAttrCtrl->GetThikForDesign(ElemD.elpro, dThik))
		{
			ASSERT(FALSE);
		}
		recond[nIndex * 2 + i].Add(m_strFormat + SbdoD.strName);
		recond[nIndex * 2 + i].Add(m_strFormat + Double2String_g(dThik));
		recond[nIndex * 2 + i].Add(m_strFormat + strPos[i] + strDir);
		recond[nIndex * 2 + i].Add(m_strFormat + Double2String_g(aElemk[i]));
		//
		T_LCOM_D LcomD;
		m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, aPos[i].iLCBNo, LcomD);
		recond[nIndex * 2 + i].Add(m_strFormat + LcomD.LoadCombName);
		//
		CString strAsUse, strRebar, strRebarDia, strRhoCom;
		if (pCodeCheck->m_pMyDB->m_pDgnDataCtrl->Get_DgnConRbms(aElemk[i], aRbmsD, pCodeCheck->m_pMyDB->m_nCode_Length))
		{
			double dAsUse = 0.0;
			const T_RBMS_D& rbmsD = aRbmsD[i + nDir * 2];
			if (!rbmsD.strBasicRebarName.IsEmpty())
			{
				strRebarDia = rbmsD.strAdd1RebarName;
				dAsUse = m_pDoc->m_pMatlDB->Get_RebarArea4Slab(strRebarDia)* (1.0 / rbmsD.dBasicRebarSpace);
				strRebar = strRebarDia + _T("@") + Double2String_g(rbmsD.dBasicRebarSpace);
			}
			if (!rbmsD.strAdd1RebarName.IsEmpty())
			{
				strRebarDia = rbmsD.strAdd1RebarName;
				if (!rbmsD.strAdd2RebarName.IsEmpty())
					strRebarDia += _T("+") + rbmsD.strAdd2RebarName;
				CString strAddRebar;
				strAddRebar = strRebarDia + _T("@") + Double2String_g(rbmsD.dAdd1RebarSpace);
				if (rbmsD.strBasicRebarName.IsEmpty())
					strRebar = strAddRebar;
				else
					strRebar = _T("(") + strAddRebar + _T(")");
				dAsUse += m_pDoc->m_pMatlDB->Get_RebarArea4Slab(strRebarDia)* (1.0 / rbmsD.dAdd1RebarSpace);
			}
			double dRhoCom = dAsUse / (1.0 * (dThik - dCover[i]));
			strAsUse = Double2String_g(dAsUse * 10, 3);
			strRhoCom = Double2String_g(dRhoCom * 100, 4);
		}
		else
		{
			strAsUse = _T("-");
			strRebar = _T("-");
			strRhoCom = _T("-");
		}
		//
		recond[nIndex * 2 + i].Add(m_strFormat + Double2String_g(aPos[i].dMu / 1000, 4));
		recond[nIndex * 2 + i].Add(m_strFormat + Double2String_g(aPos[i].dN, 4));
		recond[nIndex * 2 + i].Add(m_strFormat + Double2String_g(aPos[i].dAsReq * 10, 3));
		recond[nIndex * 2 + i].Add(m_strFormat + strAsUse);
		recond[nIndex * 2 + i].Add(m_strFormat + strRebar);
		recond[nIndex * 2 + i].Add(m_strFormat + strRhoCom);
	}
}
void AutoReport_Gen_CH_SUBWAY::WriteCharpt6_Serv(GenSegmentElements &Elems)
{
	if (Para()->SegmentIsCheck(1000 * ARGC_SUBWAY + 6, 3) == FALSE)
		return;
	MakeTitle(Elems, _T("裂缝计算结果"), 6, 3);
	//-----------------------------------------------------------------------//
	Report_Key_LIST aKeys;
	Para()->ListReportOption(aKeys, ARGC_SUBWAY);
	if (aKeys.IsEmpty())
		return;
	//
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
	if (gm == NULL)
		return;
	gm->m_LegendEngine.InitFont(20 * dFontRatio, 14 * dFontRatio);
	//
	Report_Key_LIST aReportKeys[2];
	{
		T_MLCB_D MlcbD; MlcbD.Initialize();
		CDBDoc *pDoc = CDBDoc::GetDocPoint();
		if (!pDoc->m_pAttrCtrl->Get_DefaultMlcb(MlcbD))
			MlcbD.Initialize();
		if (MlcbD.aSlabServK.IsEmpty())
			return;
		BOOL bInValid = TRUE;
		auto ValidComb = [&](int key)
		{
			for (int i = 0; i < MlcbD.aSlabServK.GetSize(); i++)
			{
				if (MlcbD.aSlabServK[i] == key)
				{
					return TRUE;
				}
			}
			return FALSE;
		};

		for (int i = 0; i < aKeys.GetSize(); i += 4)
		{
			if (aKeys[i].m_nKey != 3)
				continue;
			if (aKeys[i + 3].m_nKey != 0 && !ValidComb(aKeys[i + 3].m_nKey))
				continue;
			for (int j = 0; j < 4; j++)
				aReportKeys[aKeys[i + 2].m_nKey].Add(aKeys[i + j]);
		}
		if (aReportKeys[0].IsEmpty() && aReportKeys[1].IsEmpty())
			return;
	}

	//
	CRCSCodeCheck* pCodeCheck = new CRCSCodeCheck;
	CRCSDataBase* pDataBase = new CRCSDataBase;
	pDataBase->m_iRcsTypeKind = RCS_SSEV;
	pDataBase->m_bSlabDesignCheck = TRUE;
	pDataBase->m_nTopBottom = 2;//0 板顶 1 板底 2 顶底
	pDataBase->m_nCellOption = 0;//
	pDataBase->m_nLegendType = 1;
	pDataBase->m_bOneWayMomentCheck = FALSE;//m_bCuttingLine;
	pDataBase->m_nCuttingDiagramLeftRight = 0;//m_nCutPosi
	pDataBase->m_dCellSize = 0.0;
	pDataBase->m_nCrackSpec = 0;//混凝土规范
	pDataBase->m_bCrackCalc = FALSE;//
	pDataBase->m_bShowRebarType = FALSE;//
	pDataBase->m_dCrackLim = 0.0;//
	//
	pCodeCheck->Set_MyDBPointer(pDataBase);
	pCodeCheck->m_pDoc = m_pDoc;
	pCodeCheck->Set_DgnCtrlDataforCodeCheck();
	ActiveElemAll();
	std::vector<T_SBDO_K> aSbdoKeys;
	int nSbdo = pCodeCheck->m_pMyDB->m_pDgnDataCtrl->Get_DgnConSbdoKList(EN_DGNSBDO_SLABMAT, aSbdoKeys);
	if (nSbdo == 0)
		return;
	//Check Plate
	CArray<T_ELEM_K, T_ELEM_K> aDgnElemList;
	if (pDataBase->IsRcsSlab() || pDataBase->IsRcsMeshWall() || pDataBase->IsRcsShell())
	{
		pDataBase->m_pDgnDataCtrl->m_pMembCtrl->GetPlateListForDgn(aDgnElemList, pDataBase->IsRcsMeshWall());
	}
	else
		ASSERT(FALSE);
	if (!pDataBase->Check_TotalDesignOrCheckFlag(aDgnElemList) && !pDataBase->ShowMessageForSlabStrip())
	{
		return;
	}

	gm->m_bInitialView = FALSE;
	gm->m_LoadMinMaxType = LOADTYPE_NORMAL;
	gm->m_bLoadDataExist = TRUE;
	gm->m_bLoadDataSet = TRUE;
	gm->m_bContour = TRUE;
	gm->m_DOPT.LG.m_bDrawLegend = TRUE;//云图
	gm->m_bDeformed = FALSE;//是否变形
	gm->m_bAnimation = FALSE;
	gm->m_DOPT.DR.m_bDrawInactive = FALSE;
	gm->m_DOPT.TX.m_bOutputNumber = FALSE;//是否显示数值
	gm->m_DOPT.TX.m_bNumberExponent = FALSE;//指数
	gm->m_bMdShpContour = TRUE;//Model
	gm->m_bAnimation = FALSE;//动画模式,是否随动变换等值线
	gm->m_GPSMode = gm->m_GPSInstruction = GPS_COMMAND_SLABSERV_CHK;


	//-----------------------------------------------------------------------//
	CDesignEngine DesignD(gm);
	DesignD.ResetEngine();
	if (gm->m_pDsgnEngine)
		DesignD.ImportEngine(gm->m_pDsgnEngine);
	if (!DesignD.m_pContourEngine)
		DesignD.m_pContourEngine = &CGPSBarContainer::m_ContourEngine;
	DesignD.m_nRebarRatio = 0;//裂缝宽度
	DesignD.m_nMode = 1;//裂缝
	DesignD.m_nPosi = pDataBase->m_nTopBottom;
	DesignD.m_bSlabDgnResult = TRUE;
	gm->ResetLegendRect();
	gm->ResetEngines();
	gm->m_pDsgnEngine = new CDesignEngine(gm);
	gm->m_pDsgnEngine->ImportEngine(&DesignD);
	//-----------------------------------------------------------------------//
	for (int k = 0; k < 2; k++)
	{
		if (aReportKeys[k].IsEmpty())
			continue;
		//
		T_TABLE_D TableD;
		TableD.Initialize();
		//
		{
			TableD.HeaderD.nHeaderRowCount = 1;
			TableD.HeaderD.nHeaderColCount = 10;
			TableD.HeaderD.aTitleName.Add(_T("水池区域"));
			TableD.HeaderD.aTitleName.Add(_T("板厚(mm)"));
			TableD.HeaderD.aTitleName.Add(_T("位置"));
			TableD.HeaderD.aTitleName.Add(_T("单元号"));
			TableD.HeaderD.aTitleName.Add(_T("荷载组合"));
			TableD.HeaderD.aTitleName.Add(_T("内力M"));
			TableD.HeaderD.aTitleName.Add(_T("内力N"));
			TableD.HeaderD.aTitleName.Add(_T("实配As"));
			TableD.HeaderD.aTitleName.Add(_T("实际配筋"));
			TableD.HeaderD.aTitleName.Add(_T("裂缝ω"));
			//size: top dir1;top dir2;bot dir1,bot dir2;
			TableD.RecordD.aRecordData.SetSize(aReportKeys[k].GetCount() * nSbdo / 2);
		}
		int nIndex = 0;
		T_SBDO_D SbdoD;
		T_ELEM_K_LIST aSbdoElem;
		CString strDir, strLoad, strBook, str;
		CString strAverage[] = { _T("单元中心值"),_T("单元最大值") };
		CStringArray aStrBook, aStrPath;
		for (int i = 0; i < aReportKeys[k].GetSize(); i += 4)
		{
			//
			pDataBase->m_bAverage = aReportKeys[k][i + 2].m_nKey;//m_nElemAvg;//0 中心值 1 最大值
			pDataBase->m_nSelectLcomK = aReportKeys[k][i + 3].m_nKey;//m_lckey;
			gm->m_LoadCaseType = aReportKeys[k][i + 3].m_nType;
			gm->m_LoadCaseKey = aReportKeys[k][i + 3].m_nKey;
			pDataBase->m_nSlabDsgDir = 0;
			pDataBase->m_PForcComponent = pDataBase->m_nSlabDsgDir == 0 ? 6 : 7;
			//All components are activated for calculation, and each area is activated separately
			ActiveElemAll();
			pCodeCheck->Check_Code(0);
			//
			if (aReportKeys[k][i + 3].m_nKey == 0)
			{
				strLoad = _T("组合");
				gm->m_LoadNameStr = _LS(IDS_DB_ALL_COMBINATION);
			}
			else
				strLoad = CDBLib::GetLoadCaseNameByKey(aReportKeys[k][i + 3].m_nType, aReportKeys[k][i + 3].m_nKey, aReportKeys[k][i + 3].m_nSubType);
			DesignD.m_nElemAvg = pDataBase->m_bAverage;
			//
			for (int m = 0; m < nSbdo; m++)
			{
				const T_SBDO_K SbdoK = aSbdoKeys[m];
				m_pDoc->m_pAttrCtrl->GetSbdo(SbdoK, SbdoD);
				int nSbdoElem = m_pDoc->m_pAttrCtrl->GetElemListSbdoK(SbdoK, aSbdoElem);
				if (nSbdoElem == 0)
					continue;
				ActiveObj(aSbdoElem);
				AdjustSlabView(aSbdoElem);

				for (int n = 0; n < 2; n++)
				{
					if(!WriteCharpt6_Serv_TableMaxElemInSbdo(TableD.RecordD.aRecordData, SbdoD, aSbdoElem, nIndex, strLoad, n, pCodeCheck))
						continue;
					strDir = n == 0 ? _T("方向1") : _T("方向2");
					str.Format(_T("%s_%s_%s_%s裂缝云图"), SbdoD.strName, strDir, strAverage[(int)pDataBase->m_bAverage], strLoad);
					Replace_StrPath(str);
					//
					DesignD.m_nXDirYDir = n;//方向
					gm->m_pDsgnEngine->ImportEngine(&DesignD);
					gm->m_pDsgnEngine->InitEngine();
					gm->m_LoadNameStr = strLoad;
					//
					gm->m_pGPSCtrl->XGR_SetRebuildGeom(TRUE);
					gm->ResetLegendRect();
					gm->m_pGPSCtrl->SetShowVPointIcon(0);
					CViewBase::GetCurView_ST()->RestoreFrameTitle();
					gm->m_pGPSCtrl->UpdateAllWcsVcsMBR();
					gm->m_InvalidateFlag = TRUE;
					MakeImage_Current(str, FALSE);
					strBook.Format(_T("CHARPT_6_2_FIG_%d"), ++nIndex);
					aStrBook.Add(strBook);
					aStrPath.Add(str);
				}
			}
		}

		TableD.RecordD.nRecordRowCount = nIndex;// TableD.RecordD.aRecordData.GetSize();
		TableD.RecordD.nRecordColCount = 10;
		str = strAverage[k] + _T("单位 内力 M KN*m/m N kN/m; 配筋面积 cm^2/m");
		MakeText(Elems, str);
		if (k == 0)
			MakeTable(Elems, TableD, _T("单元中心值裂缝"), _T("CHARPT_6_Serv_TBL1"));
		else
			MakeTable(Elems, TableD, _T("单元最大值裂缝"), _T("CHARPT_6_Serv_TBL2"));
		for (int i = 0; i < aStrPath.GetSize(); i++)
		{
			MakePicture(Elems, aStrPath[i], aStrBook[i]);
		}
	}
	gm->m_GPSMode = gm->m_GPSInstruction = GPS_COMMAND_NONE;
}
BOOL AutoReport_Gen_CH_SUBWAY::WriteCharpt6_Serv_TableMaxElemInSbdo(CArray<CStringArray, CStringArray &>& recond, const T_SBDO_D& SbdoD, const T_ELEM_K_LIST& aSbdoElem,
	int nIndex, const CString& strLoad, int nDir, CRCSCodeCheck* pCodeCheck)
{
	BOOL bSbdoDHaveRebar = FALSE;
	T_RSSE_D slabRes;
	T_ELEM_K Elemk = 0;//for get maxSlab thicnkess
	T_SSER_POS slabMaxRes;
	slabMaxRes.Initialize();
	int nPos = 0;
	for (int i = 0; i < aSbdoElem.GetSize(); i++)
	{
		if (!m_pDoc->m_pPostCtrl->GetDesignResult()->Get_SlabServResult(aSbdoElem[i], nDir, slabRes))
		{
			continue;
		}
		bSbdoDHaveRebar = TRUE;
		if (slabMaxRes.dwk < max(slabRes.Bot[0].dwk, slabRes.Top[0].dwk))
		{
			Elemk = aSbdoElem[i];
			if (slabRes.Bot[0].dwk < slabRes.Top[0].dwk)
			{
				slabMaxRes = slabRes.Top[0];
				nPos = 0;
			}
			else
			{
				nPos = 1;
				slabMaxRes = slabRes.Bot[0];
			}

		}
	}
	if (Elemk == 0)
		return FALSE;
	if (!bSbdoDHaveRebar)
		return FALSE;
	T_ELEM_D ElemD;
	double dThik = 0.0;

	CString strPos[2] = { _T("板顶"),_T("板底") };
	CString strDir = nDir == 0 ? _T("(方向一)") : _T("(方向二)");
	int nCheck = 0;
	T_LCOM_D LcomD;
	if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, slabMaxRes.iCrkLCBNo, LcomD))
	{
		return FALSE;;
	}
	nCheck++;
	m_pDoc->m_pAttrCtrl->GetElem(Elemk, ElemD);
	if (!m_pDoc->m_pAttrCtrl->GetThikForDesign(ElemD.elpro, dThik))
	{
		ASSERT(FALSE);
	}
	recond[nIndex].Add(m_strFormat + SbdoD.strName);
	recond[nIndex].Add(m_strFormat + Double2String_g(dThik));
	recond[nIndex].Add(m_strFormat + strPos[nPos] + strDir);
	recond[nIndex].Add(m_strFormat + Double2String_g(Elemk));


	recond[nIndex].Add(m_strFormat + LcomD.LoadCombName);

	recond[nIndex].Add(m_strFormat + Double2String_g(slabMaxRes.dCrkMu / 1000.));
	recond[nIndex].Add(m_strFormat + Double2String_g(slabMaxRes.dCrkN));
	double dArea = 0.0;
	if (!slabMaxRes.Rebar.strSize.IsEmpty())
	{
		dArea = pCodeCheck->m_pMyDB->m_pDoc->m_pMatlDB->Get_RebarArea4Slab(slabMaxRes.Rebar.strSize) * (1.0 / slabMaxRes.Rebar.dSpace);
		if (!slabMaxRes.AddRebar.strSize.IsEmpty())
			dArea += pCodeCheck->m_pMyDB->m_pDoc->m_pMatlDB->Get_RebarArea4Slab(slabMaxRes.AddRebar.strSize) * (1.0 / slabMaxRes.AddRebar.dSpace);
	}

	recond[nIndex].Add(m_strFormat + Double2String_g(dArea * 10, 3));
	CString strRebarInfo;
	if (slabMaxRes.Rebar.dSpace < 1e-9)
		strRebarInfo = _T("-");
	else if (slabMaxRes.AddRebar.dSpace < 1e-9)
		strRebarInfo = slabMaxRes.Rebar.strSize + _T("@") + Double2String_g(slabMaxRes.Rebar.dSpace);
	else
		strRebarInfo = slabMaxRes.Rebar.strSize + _T("@") + Double2String_g(slabMaxRes.Rebar.dSpace) +
		'(' + slabMaxRes.AddRebar.strSize + '@' + Double2String_g(slabMaxRes.AddRebar.dSpace) + ')';
	recond[nIndex].Add(m_strFormat + strRebarInfo);
	recond[nIndex].Add(m_strFormat + Double2String_g(slabMaxRes.dwk, 4));
	if (nCheck != 1)
	{
		ASSERT(FALSE);
	}

	return TRUE;
}

void AutoReport_Gen_CH_SUBWAY::setTableFormatString(int nlocation /* = 0 */, int ncolor /* = 16777215 */)
{
	CString str[] = { _T("center"),_T("left"),_T("right") };
	m_strFormat.Format(_T("[DRG_CELL_PROPERTY]color=0 align=%s shape=%d tbwidth=20%%[/DRG_CELL_PROPERTY]"), str[nlocation], ncolor);
}
