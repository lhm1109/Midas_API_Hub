#include "stdafx.h"
#include "AutoReport_Gen_CH_APPRAISAL_Civil.h"
#include "ReportCommonFunc.h"
#include "TreeReportCtrl.h"
#include "LoadWordLib.h"
#include "..\wg_xl\ReportTableProcess.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_db\StoryDisplDrift.h"
#include "..\wg_db\LateralLoad.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\CFSDgnTool.h"
#include "..\wg_db\DesignResult.h"
#include "..\wg_db\ReinforceDgnMgr_CH.h"
#include "..\wg_base\wg_baseAll.h"
#include "..\wg_base\wg_baseAllTemp.h"
#include "..\wg_base\GraphFuncview.h"
#include "..\wg_gps\GPSAMRUtil.h"
#include "..\wg_gps\wg_gpsAll.h"
#include "..\wg_gps\GPSThisSmartGraphDlg.h"
#include "..\wg_gr\GRAMRUDFData.h"
#include "..\wg_gr\GRenderView.h"

//#include "..\wg_gr\I_GENModel.h"
//#include "..\wg_gr\GRDisplayDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const double cGen_Zero = 1.0E-07;

Report_Para_CH* AutoReport_Gen_CH_APPRAISAL_Civil::Para() const
{
	return AutoReport_Gen_Mgr::Inst()->Para();
}

CString AutoReport_Gen_CH_APPRAISAL_Civil::MakePicture_1_1()
{
	CString strName = _T("ALL-结构模型图");
	SetPreOrPostMode(FALSE);
	DrawNodeAll(FALSE);
	SetViewPoint(0);
	MakeImage_Current(strName, FALSE);
	return strName;
}

int AutoReport_Gen_CH_APPRAISAL_Civil::TempletID()
{
	return 0;
}
BOOL AutoReport_Gen_CH_APPRAISAL_Civil::PreMakeData()
{
	Initial();
	m_Segments.clear();
	CViewBase::GetCurView_ST()->DoInitialView();
	SetShape(FALSE);
	setTableFormatString();
	//
	GRenderView* pRenderView = (GRenderView*)(I_GENModelBase::GetCurMySelfST()->GetOwnerWnd()); ASSERT(pRenderView);
	CAutoRecoveryUnit unitSet(D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_FORCE_INDEX_N, m_pDoc->m_pUnitCtrl, TRUE);

	CViewBase* pView = static_cast<CViewBase*>(CViewBase::GetCurView_ST());
	I_GENModelBase* gm = I_GENModelBase::GetCurMySelfST();
	gm->GetDispSetting()->InitForInitialView();
	gm->GetDispSetting()->m_Node.bNode = FALSE;
	gm->GetDispSetting()->m_View.bVPoint = FALSE;

	CStlReinforceDesignMgr* m_pDesignMgr = dynamic_cast<CStlReinforceDesignMgr*>(m_pDoc->m_pPostCtrl->StlDgnMgr()->GetReinforceDgnMgr());

	GetStlList();
	GetCFSList();

	WriteCharpt1();
	WriteCharpt2();
	WriteCharpt3();
	WriteCharpt4();
	WriteCharpt5();
	WriteCharpt6();

	return TRUE;
}
void AutoReport_Gen_CH_APPRAISAL_Civil::InsertCoverPage()
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

	strTemp.Format(_T("[DRG_FONT]|FC| size=30 bold=true align=center [/DRG_FONT]%s\n[DRG_FONT]|FI|[/DRG_FONT]"), _T("鉴定报告"));
	strText += strTemp;
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=true align=center underline=0[/DRG_FONT]\n\n\n%s\n[DRG_FONT]|FI|[/DRG_FONT]"), _T("报告编号："));
	strText += strTemp;


	CString strDate = _T("");
	CTime time;
	time = CTime::GetCurrentTime();
	int nYear = time.GetYear();
	int nMonth = time.GetMonth();
	int nDate = time.GetDay();
	strDate.Format(_T("%d-%d-%d"), nYear, nMonth, nDate);
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=false align=left[/DRG_FONT]\n\n\n\n\n\n\n%s%s[DRG_FONT]|FI|[/DRG_FONT]"), FillBlank(_T(""), 13), _T("工程名称:"));
	strText += strTemp;
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=false align=left underline=1[/DRG_FONT]%s\n[DRG_FONT]|FI|[/DRG_FONT]"), FillBlank(data.strReviewName[0]));
	strText += strTemp;
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=false align=left[/DRG_FONT]%s%s[DRG_FONT]|FI|[/DRG_FONT]"), FillBlank(_T(""), 13), _T("委托单位:"));
	strText += strTemp;
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=false align=left underline=1[/DRG_FONT]%s\n[DRG_FONT]|FI|[/DRG_FONT]"), FillBlank(data.strReviewName[1]));
	strText += strTemp;
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=false align=left[/DRG_FONT]%s%s[DRG_FONT]|FI|[/DRG_FONT]"), FillBlank(_T(""), 13), _T("鉴定项目:"));
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
	pSrc.sExtInfo = _T("鉴定报告");
	pSrc.sFullPath = _T("");
	pSrc.sType = _LSX(TXT);
	CReportCommonFunc::ChangeStructStr2Char(&pSrc, &pTgt);
	CReportCommonFunc::ExportData2Word(&pTgt);
	CReportCommonFunc::ExportInsertPageBreak();
}
void AutoReport_Gen_CH_APPRAISAL_Civil::InsertCatalogPage()
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
void AutoReport_Gen_CH_APPRAISAL_Civil::MakeContent()
{
	for (int i = 0; i < m_Segments.size(); i++)
	{
		InsertSegmentElements(m_Segments[i]);
	}
}
void AutoReport_Gen_CH_APPRAISAL_Civil::WriteCharpt1()
{
	GenSegmentElements genElements;
	MakeTitle(genElements, _T("工程概况"), 1, 0);
	CString str;
	str.Format(_T("    该工程项目的基本情况如下表："));
	MakeText(genElements, str);
	str.Format(_T("[DRG_CELL_PROPERTY]color=0 align=%s shape=%d tbwidth=80%%[/DRG_CELL_PROPERTY]"), _T("center"), 16777215);

	T_TABLE_D TableD;
	TableD.Initialize();

	TableD.RecordD.aRecordData.SetSize(7);
	TableD.RecordD.aRecordData[0].Add(_T("项目名称"));
	TableD.RecordD.aRecordData[0].Add(str);

	TableD.RecordD.aRecordData[1].Add(_T("项目地址"));
	TableD.RecordD.aRecordData[1].Add(str);

	TableD.RecordD.aRecordData[2].Add(_T("建造年代"));
	TableD.RecordD.aRecordData[2].Add(str);

	TableD.RecordD.aRecordData[3].Add(_T("建筑面积"));
	TableD.RecordD.aRecordData[3].Add(str);

	TableD.RecordD.aRecordData[4].Add(_T("设计单位"));
	TableD.RecordD.aRecordData[4].Add(str);

	TableD.RecordD.aRecordData[5].Add(_T("施工单位"));
	TableD.RecordD.aRecordData[5].Add(str);

	TableD.RecordD.aRecordData[6].Add(_T("监理单位"));
	TableD.RecordD.aRecordData[6].Add(str);

	TableD.RecordD.nRecordRowCount = 7;
	TableD.RecordD.nRecordColCount = 2;
	MakeTableCenterText(genElements, TableD, _T(""), _T(""));

	str.Format(_T("    为了解结构的可靠性，委托单位委托检测单位对该结构进行安全性鉴定。"));
	MakeText(genElements, str);
	m_Segments.push_back(genElements);
}
void AutoReport_Gen_CH_APPRAISAL_Civil::WriteCharpt2()
{
	GenSegmentElements Elems;
	MakeTitle(Elems, _T("鉴定的基本情况"), 2, 0);
	WriteCharpt2_1(Elems);
	WriteCharpt2_2(Elems);
	m_Segments.push_back(Elems);
}
void AutoReport_Gen_CH_APPRAISAL_Civil::WriteCharpt2_1(GenSegmentElements& genElements)
{
	MakeTitle(genElements, _T("鉴定的目的和内容"), 2, 1);
}
void AutoReport_Gen_CH_APPRAISAL_Civil::WriteCharpt2_2(GenSegmentElements& genElements)
{
	MakeTitle(genElements, _T("鉴定依据的规范"), 2, 2);
	CString str;
	MakeAlignText(str, _T("(1)《建筑结构检测技术标准》"), _T("GB/T 50344-2019"));
	MakeAlignText(str, _T("(2)《钢结构现场检测技术标准》"), _T("GB/T 50621-2010"));
	MakeAlignText(str, _T("(3)《建筑结构可靠性设计统一标准》"), _T("GB 50068-2018"));
	MakeAlignText(str, _T("(4)《建筑结构荷载规范》"), _T("GB 50009-2001/2012"));
	MakeAlignText(str, _T("(5)《钢结构设计标准》"), _T("GB 50017-2017"));
	MakeAlignText(str, _T("(6)《冷弯薄壁型钢结构技术规范》"), _T("GB 50018-2002"));
	MakeAlignText(str, _T("(7)《工程结构通用规范》"), _T("GB 55001-2021"));
	MakeAlignText(str, _T("(8)《钢结构通用规范》"), _T("GB 55006-2021"));
	MakeAlignText(str, _T("(9)《建筑与市政工程抗震通用规范》"), _T("GB 55002-2021"));
	MakeAlignText(str, _T("(10)《既有建筑鉴定与加固通用规范》"), _T("GB 55021-2021"));
	MakeAlignText(str, _T("(11)《建筑工程抗震设防分类标准》"), _T("GB 50223-2008"));
	MakeAlignText(str, _T("(12)《建筑抗震设计规范》"), _T("GB 50011-2010（2016年版）"));
	MakeAlignText(str, _T("(13)《民用建筑可靠性鉴定标准》"), _T("GB 50292-2015"));
	MakeAlignText(str, _T("(14)《工业建筑可靠性鉴定标准》"), _T("GB 50144-2019"));
	MakeAlignText(str, _T("(15)《危险房屋鉴定标准》"), _T("JGJ 125-2016"));
	MakeAlignText(str, _T("(16)《建筑抗震鉴定标准》"), _T("GB 50023-2009"));

	MakeText(genElements, str);
}

void AutoReport_Gen_CH_APPRAISAL_Civil::WriteCharpt3()
{
	GenSegmentElements Elems;
	MakeTitle(Elems, _T("结构的基本情况调查"), 3, 0);
	WriteCharpt3_1(Elems);
	WriteCharpt3_2(Elems);
	m_Segments.push_back(Elems);
}
void AutoReport_Gen_CH_APPRAISAL_Civil::WriteCharpt3_1(GenSegmentElements& genElements)
{
	MakeTitle(genElements, _T("结构的基本情况"), 3, 1);
	CString str;
	str.Format(_T("    建筑结构安全等级为一级，结构重要性系数γo=1.10。建筑耐火等级为一级。其三维模型如下图所示："));
	MakeText(genElements, str);

	SetViewPoint(0);
	CString strName = _T("结构模型三维视图");
	ActiveElemAll();
	MakeImage_Current(strName, FALSE);
	MakePicture(genElements, strName, _T("CHARPT_3_1_FIG"));

	str.Format(_T("    模型中使用的钢构件材料见下表："));
	MakeText(genElements, str);
	CArray<T_MATL_K, T_MATL_K> rKeyList, rStlKeyList;
	// 表3.1
	{
		m_pDoc->m_pAttrCtrl->GetMatlKeyList(rKeyList);
		int nSize = rKeyList.GetSize();
		int nStlSize = 0;
		T_MATD_D data;
		for (int i = 0; i < nSize; i++)
		{
			m_pDoc->m_pAttrCtrl->GetMatlDesign(rKeyList[i], data);
			if (data.Type == _T("S"))
			{
				rStlKeyList.Add(rKeyList[i]);
				nStlSize++;
			}
		}

		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 7;
		TableD.HeaderD.aTitleName.Add(_T("材料号"));
		TableD.HeaderD.aTitleName.Add(_T("规范号"));
		TableD.HeaderD.aTitleName.Add(_T("钢号"));
		TableD.HeaderD.aTitleName.Add(_T("厚度或直径(mm)"));
		TableD.HeaderD.aTitleName.Add(_T("抗拉、抗压和抗弯fy/f"));
		TableD.HeaderD.aTitleName.Add(_T("抗剪fv"));
		TableD.HeaderD.aTitleName.Add(_T("抗拉强度fu"));
		TableD.RecordD.aRecordData.SetSize(nStlSize * 5);
		int i = 0, k = -1;
		T_MATD_D MatdD;
		MakeStlMatlInfo();
		for (; i < nStlSize; i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(rStlKeyList[i], MatdD))
				continue;

			SetStlInfo(rKeyList[i], MatdD, TableD.RecordD.aRecordData, k);
		}
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = k + 1;//nStlSize*5;
		TableD.RecordD.nRecordColCount = 7;
		if (nStlSize > 0)
			MakeTableCenterText(genElements, TableD, _T("原钢材材料列表(单位 Mpa)"), _T("CHARPT_3_1_1_TBL"));

	}

}
void AutoReport_Gen_CH_APPRAISAL_Civil::WriteCharpt3_2(GenSegmentElements& genElements)
{
	MakeTitle(genElements, _T("结构的荷载情况"), 3, 2);
	CString str;
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
	CString strD, strL;
	m_pDoc->m_pAttrCtrl2->GetUsedStldKeyList(lstLoad);
	for (int k = 0; k < lstLoad.GetCount(); k++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetStld(lstLoad[k], StldD))
			continue;
		if (StldD.LoadCaseType == _T("D"))
		{
			tempLoad[0].Add(lstLoad[k]);
			strD += StldD.LoadCaseName + _T(" ");
		}
			
		else if (StldD.LoadCaseType == _T("L"))
		{
			tempLoad[1].Add(lstLoad[k]);
			strL += StldD.LoadCaseName + _T(" ");
		}		
		else if (StldD.LoadCaseType == _T("W"))
			windload.Add(lstLoad[k]);
	}
	gpsmode->m_LoadCaseType = D_LOADCASE_STATIC;
	int nIndex = 0;
	for (int i = 0;i < 2;i++)
	{
		if (i == 0 && !tempLoad[i].IsEmpty())
		{
			strName = _T("恒荷载");
			MakeTitle(genElements, strName, 3, 2, _T(""), 1);
			strName.Format(_T("    结构自重由软件自动计算考虑，除结构自重外，其他恒载：%s，恒荷载布置如下图所示（单位：kN,m）："), GetBold(strD));
			MakeText(genElements, strName);
		}
		else if (i == 1 && !tempLoad[i].IsEmpty())
		{
			strName = _T("活荷载");
			MakeTitle(genElements, strName, 3, 2, _T(""), 2);
			strName.Format(_T("    %s，活荷载布置如下图所示（单位：kN,m）："), GetBold(strL));
			MakeText(genElements, strName);
		}
		for (int j = 0;j < tempLoad[i].GetSize();j++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetStld(tempLoad[i][j], StldD))
				continue;
			strName.Format((i == 0 ? _T("恒荷载 %s") : _T("活荷载 %s")), StldD.LoadCaseName);
			strBook.Format(_T("CHARPT_3_2_%d_FIG"), ++nIndex);
			gpsmode->m_LoadCaseKey = tempLoad[i][j];
			MakeImage_Current(strName, FALSE);
			MakePicture(genElements, strName, strBook);
		}
	}
	//
	strName = _T("风荷载");
	MakeTitle(genElements, strName, 3, 2, _T(""), 3);
	for (int j = 0;j < windload.GetSize();j++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetStld(windload[j], StldD))
			continue;
		strName.Format(_T("风荷载 %s"), StldD.LoadCaseName);
		strBook.Format(_T("CHARPT_3_2_%d_FIG"), ++nIndex);
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
		gm->GetDispSetting()->m_Load.bPlaneLoad = FALSE;
		gm->GetDispSetting()->m_Load.bNodalTemp = FALSE;
		gm->GetDispSetting()->m_Load.bElemTemp = FALSE;
		//gm->GetDispSetting()->m_Load.bFireFipa		= FALSE;
		//gm->GetDispSetting()->m_Load.bFireFssf		= FALSE;
		gm->GetDispSetting()->m_Load.bTempGradiant = FALSE;
		gm->GetDispSetting()->m_Load.bPrestress = FALSE;
		gm->GetDispSetting()->m_Load.bPretention = FALSE;
		gm->GetDispSetting()->m_Load.bDisplacement = FALSE;
		gm->GetDispSetting()->m_Load.bWind = FALSE;
		gm->GetDispSetting()->m_Load.bSeismic = FALSE;
		gm->GetDispSetting()->m_Load.bPseudoSeismic = FALSE;
		gm->GetDispSetting()->m_Load.bWindPArea = FALSE;
		gm->GetDispSetting()->m_Load.bWindPBeam = FALSE;
		gm->GetDispSetting()->m_Load.bWindPNodal = FALSE;
		gm->GetDispSetting()->m_Load.bWindPFunc = FALSE;
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

	//4.2.5
	{
		strName = _T("4.荷载组合");
		MakeText(genElements, strName);
		strName = _T("    本工程采用的荷载组合如下表所示:");
		MakeText(genElements, strName);

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
		UINT aLCombType[] = {/*D_LCOMTYPE_GENERAL, D_LCOMTYPE_CONCRETE,*/ D_LCOMTYPE_STEEL, /*D_LCOMTYPE_SRC*/ };
		UINT aItemData[] = {/*D_LOADCASE_COMB_GENERAL, D_LOADCASE_COMB_CONCRETE,*/ D_LOADCASE_COMB_STEEL,/* D_LOADCASE_COMB_SRC*/ };
		CString aCombTypeStr[] = { _LS(IDS_CMD_LOADCOMBI_Add), _LS(IDS_CMD_LOADCOMBI_Envelope), _T("ABS"), _T("SRSS") };
		int nRowCount = 0;
		for (int i = 0; i < sizeof(aLCombType) / sizeof(UINT); i++)
		{
			const UINT nType = aLCombType[i];
			const UINT nRealType = aItemData[i];
			m_pDoc->m_pAttrCtrl->GetLcomKeyList(nType, lstLoad);
			record.SetSize(lstLoad.GetCount());
			for (int k = 0; k < lstLoad.GetCount(); k++)
			{
				const T_KEY nKey = lstLoad[k];
				if (!m_pDoc->m_pAttrCtrl->GetLcom(nType, nKey, LComD))
					continue;
				record[k].SetSize(3);
				record[k][0] = LComD.LoadCombName;
				record[k][1] = aCombTypeStr[LComD.LoadCombType];
				CString str, strTemp, strDesc;
				strDesc = _T("");
				for (int i = 0;i < LComD.aCombination.GetSize();i++)
				{
					if (!m_pDoc->m_pAttrCtrl2->GetLoadCaseNameAndDesc(LComD.aCombination[i].AnalType, LComD.aCombination[i].LoadCaseKey, strName, strTemp))
						continue;
					str.Format(_T("%.2f%s"), LComD.aCombination[i].Factor, strName);
					if (!strDesc.IsEmpty())
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
		MakeTableCenterText(genElements, TableD, _T("荷载组合表"), _T("CHARPT_3_4_TBL1"));
	}
}

void AutoReport_Gen_CH_APPRAISAL_Civil::WriteCharpt4()
{
	GenSegmentElements Elems;
	MakeTitle(Elems, _T("构件的可靠性鉴定"), 4, 0);
	CString str;
	str.Format(_T("根据《民用建筑可靠性鉴定标准》GB 50292-2015，钢构件的安全性等级按照承载能力、构造以及不适于承载的位移或变形等三个项目评定。"));
	MakeText(Elems, str);
	WriteCharpt4_1(Elems);
	WriteCharpt4_2(Elems);
	m_Segments.push_back(Elems);
}
void AutoReport_Gen_CH_APPRAISAL_Civil::WriteCharpt4_1(GenSegmentElements& genElements)
{
	MakeTitle(genElements, _T("构件的安全性鉴定"), 4, 1);
	CString str;
	str.Format(_T("    承载能力评级按照规范5.3.2条执行，具体评定规则见下表："));
	MakeText(genElements, str);
	//表4.1-1  钢构件承载能力评定等级
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 2;
		TableD.HeaderD.nHeaderColCount = 6;
		TableD.HeaderD.aTitleName.Add(_T("构件种类"));
		TableD.HeaderD.aTitleName.Add(_T("公式"));
		TableD.HeaderD.aTitleName.Add(_T("评定标准"));
		TableD.HeaderD.aTitleName.Add(_T("a"));
		TableD.HeaderD.aTitleName.Add(_T("b"));
		TableD.HeaderD.aTitleName.Add(_T("c"));
		TableD.HeaderD.aTitleName.Add(_T("d"));
		TableD.RecordD.aRecordData.SetSize(2);

		T_MERGE_UNIT_D merge;merge.Initialize();
		merge.nMergeCount = 2; merge.nStartColNumber = 1; merge.nStartRowNumber = 1;
		TableD.HeaderD.aMergeRowD.Add(merge);
		merge.nMergeCount = 2; merge.nStartColNumber = 2; merge.nStartRowNumber = 1;
		TableD.HeaderD.aMergeRowD.Add(merge);
		merge.Initialize();
		merge.nMergeCount = 4; merge.nStartColNumber = 3; merge.nStartRowNumber = 1;
		TableD.HeaderD.aMergeColD.Add(merge);
		//   
		auto& record = TableD.RecordD.aRecordData;
		CString strType[] = { _T("主要构件及节点、连接"), _T("一般构件") };
		T_TAIF_D TaifD;
		if (!m_pDoc->m_pAttrCtrl->GetTaif(TaifD))
			TaifD.Initialize();
		for (int k = 0; k < 2; k++)
		{
			double dGA = k == 0 ? TaifD.dMainGradeA : TaifD.dSecondaryGradeA;
			double dGB = k == 0 ? TaifD.dMainGradeB : TaifD.dSecondaryGradeB;
			double dGC = k == 0 ? TaifD.dMainGradeC : TaifD.dSecondaryGradeC;
			record[k].SetSize(6);
			record[k][0] = strType[k];
			record[k][1] = _T("R/(γ0S)");
			str.Format(_T("≥%.2f"), dGA);
			record[k][2] = str;
			str.Format(_T("＜%.2f，≥%.2f"), dGA, dGB);
			record[k][3] = str;
			str.Format(_T("＜%.2f，≥%.2f"), dGB, dGC);
			record[k][4] = str;
			if (k == 0)
				str.Format(_T("＜%.2f或当构件或连接出现脆性断裂、疲劳开裂或局部失稳变形迹象时"), dGC);
			else
				str.Format(_T("＜%.2f或当构件或连接出现脆性断裂、疲劳开裂或局部失稳变形迹象时"), dGC);
			record[k][5] = str;
		}

		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = 2;
		TableD.RecordD.nRecordColCount = 6;
		MakeTableCenterText(genElements, TableD, _T("钢构件承载能力评定等级"), _T("CHARPT_4_1_1_TBL1"));
	}
	
	str.Format(_T("    各构件的构造评定等级按照规范5.3.3条执行，具体评定规则见下表："));
	MakeText(genElements, str);
	//表4.1-2  钢构件构造评定等级
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 3;
		TableD.HeaderD.aTitleName.Add(_T("检查项目"));
		TableD.HeaderD.aTitleName.Add(_T("au级或bu级"));
		TableD.HeaderD.aTitleName.Add(_T("cu级或du级"));
		TableD.RecordD.aRecordData.SetSize(1);;
		auto& record = TableD.RecordD.aRecordData;
		record[0].SetSize(3);
		record[0][0] = _T("构件构造");
		record[0][1] = _T("构件组成形式、长细比或高跨比、宽厚比或高厚比等符合或基本符合国家现行标准规定；无缺陷或仅有局部表面缺陷；工作无异常");
		record[0][2] = _T("构件组成形式、长细比或高跨比、宽厚比或高厚比等不符合国家现行设计标准要求；存在明显缺陷，已影响或显著影响正常工作");
	
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = 1;
		TableD.RecordD.nRecordColCount = 3;
		MakeTableCenterText(genElements, TableD, _T("钢构件构造评定等级"), _T("CHARPT_4_1_2_TBL1"));
	}

	str.Format(_T("    根据规范的要求，结构中各构件的安全性评价等级结果见下表(仅输出安全性评价等级小于au级的构件)："));
	MakeText(genElements, str);
	
	if (!m_pDoc->m_pPostCtrl->GetDesignResult()->IsSteelDesignResultEnable() && !m_pDoc->m_pPostCtrl->GetDesignResult()->IsColdFormDesignResultEnable())
		return;

	T_IMEL_D ImelD;
	CString strMemb[] = { _T("主要构件"), _T("一般构件") };
	CString strGrade[] = { _T("au"), _T("bu"), _T("cu"), _T("du") };
	T_RSTL_GB50017_D ResData;
	T_RSTL_D SteelResult;
	CH_RCFS_D CfsData;
	T_DSAS_D data;
	//表4.1-3  钢梁构件承载能力评定等级
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 7;
		TableD.HeaderD.aTitleName.Add(_T("构件号"));
		TableD.HeaderD.aTitleName.Add(_T("构件种类"));
		TableD.HeaderD.aTitleName.Add(_T("承载力验算比"));
		TableD.HeaderD.aTitleName.Add(_T("承载能力评价结果"));
		TableD.HeaderD.aTitleName.Add(_T("长细比验算"));
		TableD.HeaderD.aTitleName.Add(_T("宽厚比验算"));
		TableD.HeaderD.aTitleName.Add(_T("构造评价结果"));
		TableD.RecordD.aRecordData.SetSize(m_lstBeam.GetCount() + m_lstBeamCFS.GetCount());
		auto& record = TableD.RecordD.aRecordData;

		for (int i=0; i<m_lstBeam.GetCount(); i++)
		{	
			if (!m_pDoc->m_pAttrCtrl->GetImel(m_lstBeam[i], ImelD))
				ImelD.Initialize();
			CReinforceDgnMgr_CH::Instance()->GetEvaluateSecurity(m_lstBeam[i], data);
			record[i].SetSize(7);
			record[i][0].Format(_T("%d"), m_lstBeam[i]);
			record[i][1] = ImelD.bImportant ? strMemb[0] : strMemb[1];
			if (m_pDoc->m_pPostCtrl->GetDesignResult()->ReadSteelDesignResult(m_lstBeam[i], 0, 1, 0, SteelResult) && SteelResult.bCheck)
			{

				if (m_pDoc->m_pPostCtrl->StlDgnMgr()->ReadDesignResult(m_lstBeam[i], ResData))
				{
					str.Format(_T("%.3g"), 1.0 / ResData.m_Sect.m_Data[T_RSTL_MEMB_SECT::RLT_COMP].GetRatio());
					record[i][2] = str;
					str = data.nEvaluateLevel[0] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[0]];
					record[i][3] = str;
					record[i][4] = ResData.m_Sect.m_Data[T_RSTL_MEMB_SECT::RLT_SLENDER].GetRatio() > 1.0 ? _T("NG") : _T("OK");
					str = (ResData.m_Sect.m_Data[T_RSTL_MEMB_SECT::RLT_WTRATIO_FLG].GetRatio() > 1.0 || ResData.m_Sect.m_Data[T_RSTL_MEMB_SECT::RLT_WTRATIO_WEB].GetRatio() > 1.0) ? _T("NG") : _T("OK");
					record[i][5] = str;
				}
				else
				{
					if (SteelResult.dblCodeRatio[5] > 1.0e-9)
						str.Format(_T("%.3g"), 1.0 / SteelResult.dblCodeRatio[5]);
					else
						str = _T("-");
					record[i][2] = str;
					str = data.nEvaluateLevel[0] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[0]];
					record[i][3] = str;
					str = _T("-");
					if (SteelResult.res_dSlenLimit[0] > 1.0e-9)
					{
						str = SteelResult.res_dSlenValue[0] / SteelResult.res_dSlenLimit[0] > 1.0 ? _T("NG") : _T("OK");
					}
					if (SteelResult.res_dSlenLimit[1] > 1.0e-9)
					{
						if (SteelResult.res_bSlenComp[0] && SteelResult.res_dSlenValue[0] / SteelResult.res_dSlenLimit[0] < SteelResult.res_dSlenValue[1] / SteelResult.res_dSlenLimit[1])
							str = SteelResult.res_dSlenValue[1] / SteelResult.res_dSlenLimit[1] > 1.0 ? _T("NG") : _T("OK");
					}
					record[i][4] = str;
					if (SteelResult.res_dRatWithoutLT[3] > 1.0e-9 || SteelResult.res_dRatWithoutLT[1] > 1.0e-9)
					{
						str = (SteelResult.res_dRatWithoutLT[2] / SteelResult.res_dRatWithoutLT[3] > 1.0 || SteelResult.res_dRatWithoutLT[0] / SteelResult.res_dRatWithoutLT[1] > 1.0) ? _T("NG") : _T("OK");
						record[i][5] = str;
					}
					else
						record[i][5] = _T("-");

				}
			}
			
			str = data.nEvaluateLevel[1] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[1]];
			record[i][6] = str;
		}
		int nIndex = m_lstBeam.GetCount();
		int k = 0;
		for (int i = 0; i < m_lstBeamCFS.GetCount(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetImel(m_lstBeamCFS[i], ImelD))
				ImelD.Initialize();
			ResData.Initial();
			k = i + nIndex;
			m_pDoc->m_pPostCtrl->CfsDgnMgr()->ReadDesignResult(m_lstBeamCFS[i], CfsData);
			CReinforceDgnMgr_CH::Instance()->GetEvaluateSecurity(m_lstBeamCFS[i], data);
			record[k].SetSize(7);
			record[k][0].Format(_T("%d"), m_lstBeamCFS[i]);
			record[k][1] = ImelD.bImportant ? strMemb[0] : strMemb[1];
			str.Format(_T("%.3g"), 1.0 / CfsData.m_Sect.m_Data[CH_RCFS_SECT::RLT_COMP].GetRatio());
			record[k][2] = str;
			str = data.nEvaluateLevel[0] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[0]];
			record[k][3] = str;
			str = (CfsData.m_Sect.m_Data[CH_RCFS_SECT::RLT_SLENDER_Y].GetRatio() > 1.0 || CfsData.m_Sect.m_Data[CH_RCFS_SECT::RLT_SLENDER_Z].GetRatio() > 1.0) ? _T("NG") : _T("OK");
			record[k][4] = str;
			str = (CfsData.m_Sect.m_Data[CH_RCFS_SECT::RLT_WHRTO_F].GetRatio() > 1.0 || CfsData.m_Sect.m_Data[CH_RCFS_SECT::RLT_WHRTO_W].GetRatio() > 1.0 || CfsData.m_Sect.m_Data[CH_RCFS_SECT::RLT_WHRTO_J].GetRatio() > 1.0) ? _T("NG") : _T("OK");
			record[k][5] = str;
			str = data.nEvaluateLevel[1] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[1]];
			record[k][6] = str;
		}
		
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = m_lstBeam.GetCount() + m_lstBeamCFS.GetCount();
		TableD.RecordD.nRecordColCount = 7;
		if (m_lstBeam.GetCount() + m_lstBeamCFS.GetCount() > 0)
			MakeTableCenterText(genElements, TableD, _T("钢梁构件承载能力评定等级"), _T("CHARPT_4_1_3_TBL1"));
	}

	//表4.1-4  钢柱构件承载能力评定等级
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 7;
		TableD.HeaderD.aTitleName.Add(_T("构件号"));
		TableD.HeaderD.aTitleName.Add(_T("构件种类"));
		TableD.HeaderD.aTitleName.Add(_T("承载力验算比"));
		TableD.HeaderD.aTitleName.Add(_T("承载能力评价结果"));
		TableD.HeaderD.aTitleName.Add(_T("长细比验算"));
		TableD.HeaderD.aTitleName.Add(_T("宽厚比验算"));
		TableD.HeaderD.aTitleName.Add(_T("构造评价结果"));
		TableD.RecordD.aRecordData.SetSize(m_lstCol.GetCount() + m_lstColCFS.GetCount());
		auto& record = TableD.RecordD.aRecordData;

		for (int i = 0; i < m_lstCol.GetCount(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetImel(m_lstCol[i], ImelD))
				ImelD.Initialize();
			CReinforceDgnMgr_CH::Instance()->GetEvaluateSecurity(m_lstCol[i], data);

			record[i].SetSize(7);
			record[i][0].Format(_T("%d"), m_lstCol[i]);
			record[i][1] = ImelD.bImportant ? strMemb[0] : strMemb[1];
			if (m_pDoc->m_pPostCtrl->GetDesignResult()->ReadSteelDesignResult(m_lstCol[i], 0, 1, 0, SteelResult) && SteelResult.bCheck)
			{
				if (m_pDoc->m_pPostCtrl->StlDgnMgr()->ReadDesignResult(m_lstCol[i], ResData))
				{
					str.Format(_T("%.3g"), 1.0 / ResData.m_Sect.m_Data[T_RSTL_MEMB_SECT::RLT_COMP].GetRatio());
					record[i][2] = str;
					str = data.nEvaluateLevel[0] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[0]];
					record[i][3] = str;
					record[i][4] = ResData.m_Sect.m_Data[T_RSTL_MEMB_SECT::RLT_SLENDER].GetRatio() > 1.0 ? _T("NG") : _T("OK");
					str = (ResData.m_Sect.m_Data[T_RSTL_MEMB_SECT::RLT_WTRATIO_FLG].GetRatio() > 1.0 || ResData.m_Sect.m_Data[T_RSTL_MEMB_SECT::RLT_WTRATIO_WEB].GetRatio() > 1.0) ? _T("NG") : _T("OK");
					record[i][5] = str;
				}
				else
				{
					if (SteelResult.dblCodeRatio[5] > 1.0e-9)
						str.Format(_T("%.3g"), 1.0 / SteelResult.dblCodeRatio[5]);
					else
						str = _T("-");
					record[i][2] = str;
					str = data.nEvaluateLevel[0] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[0]];
					record[i][3] = str;
					str = _T("-");
					if (SteelResult.res_dSlenLimit[0] > 1.0e-9)
					{
						str = SteelResult.res_dSlenValue[0] / SteelResult.res_dSlenLimit[0] > 1.0 ? _T("NG") : _T("OK");
					}
					if (SteelResult.res_dSlenLimit[1] > 1.0e-9)
					{
						if (SteelResult.res_bSlenComp[0] && SteelResult.res_dSlenValue[0] / SteelResult.res_dSlenLimit[0] < SteelResult.res_dSlenValue[1] / SteelResult.res_dSlenLimit[1])
							str = SteelResult.res_dSlenValue[1] / SteelResult.res_dSlenLimit[1] > 1.0 ? _T("NG") : _T("OK");
					}
					record[i][4] = str;
					if (SteelResult.res_dRatWithoutLT[3] > 1.0e-9 || SteelResult.res_dRatWithoutLT[1] > 1.0e-9)
					{
						str = (SteelResult.res_dRatWithoutLT[2] / SteelResult.res_dRatWithoutLT[3] > 1.0 || SteelResult.res_dRatWithoutLT[0] / SteelResult.res_dRatWithoutLT[1] > 1.0) ? _T("NG") : _T("OK");
						record[i][5] = str;
					}
					else
						record[i][5] = _T("-");

				}
			}			
			str = data.nEvaluateLevel[1] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[1]];
			record[i][6] = str;
		}
		int nIndex = m_lstCol.GetCount();
		int k = 0;
		for (int i = 0; i < m_lstColCFS.GetCount(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetImel(m_lstColCFS[i], ImelD))
				ImelD.Initialize();
			ResData.Initial();
			k = i + nIndex;
			m_pDoc->m_pPostCtrl->CfsDgnMgr()->ReadDesignResult(m_lstColCFS[i], CfsData);
			CReinforceDgnMgr_CH::Instance()->GetEvaluateSecurity(m_lstColCFS[i], data);
			record[k].SetSize(7);
			record[k][0].Format(_T("%d"), m_lstColCFS[i]);
			record[k][1] = ImelD.bImportant ? strMemb[0] : strMemb[1];
			str.Format(_T("%.3g"), 1.0 / CfsData.m_Sect.m_Data[CH_RCFS_SECT::RLT_COMP].GetRatio());
			record[k][2] = str;
			str = data.nEvaluateLevel[0] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[0]];
			record[k][3] = str;
			str = (CfsData.m_Sect.m_Data[CH_RCFS_SECT::RLT_SLENDER_Y].GetRatio() > 1.0 || CfsData.m_Sect.m_Data[CH_RCFS_SECT::RLT_SLENDER_Z].GetRatio() > 1.0) ? _T("NG") : _T("OK");
			record[k][4] = str;
			str = (CfsData.m_Sect.m_Data[CH_RCFS_SECT::RLT_WHRTO_F].GetRatio() > 1.0 || CfsData.m_Sect.m_Data[CH_RCFS_SECT::RLT_WHRTO_W].GetRatio() > 1.0 || CfsData.m_Sect.m_Data[CH_RCFS_SECT::RLT_WHRTO_J].GetRatio() > 1.0) ? _T("NG") : _T("OK");
			record[k][5] = str;
			str = data.nEvaluateLevel[1] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[1]];
			record[k][6] = str;
		}

		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = m_lstCol.GetCount() + m_lstColCFS.GetCount();
		TableD.RecordD.nRecordColCount = 7;
		if (m_lstCol.GetCount() + m_lstColCFS.GetCount() > 0)
			MakeTableCenterText(genElements, TableD, _T("钢柱构件承载能力评定等级"), _T("CHARPT_4_1_4_TBL1"));
	}

	//表4.1-5  钢支撑构件承载能力评定等级
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 7;
		TableD.HeaderD.aTitleName.Add(_T("构件号"));
		TableD.HeaderD.aTitleName.Add(_T("构件种类"));
		TableD.HeaderD.aTitleName.Add(_T("承载力验算比"));
		TableD.HeaderD.aTitleName.Add(_T("承载能力评价结果"));
		TableD.HeaderD.aTitleName.Add(_T("长细比验算"));
		TableD.HeaderD.aTitleName.Add(_T("宽厚比验算"));
		TableD.HeaderD.aTitleName.Add(_T("构造评价结果"));
		TableD.RecordD.aRecordData.SetSize(m_lstBraceCFS.GetCount() + m_lstBrace.GetCount());
		auto& record = TableD.RecordD.aRecordData;

		for (int i = 0; i < m_lstBrace.GetCount(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetImel(m_lstBrace[i], ImelD))
				ImelD.Initialize();
			CReinforceDgnMgr_CH::Instance()->GetEvaluateSecurity(m_lstBrace[i], data);

			record[i].SetSize(7);
			record[i][0].Format(_T("%d"), m_lstBrace[i]);
			record[i][1] = ImelD.bImportant ? strMemb[0] : strMemb[1];
			if (m_pDoc->m_pPostCtrl->GetDesignResult()->ReadSteelDesignResult(m_lstBrace[i], 0, 1, 0, SteelResult) && SteelResult.bCheck)
			{
				if (m_pDoc->m_pPostCtrl->StlDgnMgr()->ReadDesignResult(m_lstBrace[i], ResData))
				{
					str.Format(_T("%.3g"), 1.0 / ResData.m_Sect.m_Data[T_RSTL_MEMB_SECT::RLT_COMP].GetRatio());
					record[i][2] = str;
					str = data.nEvaluateLevel[0] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[0]];
					record[i][3] = str;
					record[i][4] = ResData.m_Sect.m_Data[T_RSTL_MEMB_SECT::RLT_SLENDER].GetRatio() > 1.0 ? _T("NG") : _T("OK");
					str = (ResData.m_Sect.m_Data[T_RSTL_MEMB_SECT::RLT_WTRATIO_FLG].GetRatio() > 1.0 || ResData.m_Sect.m_Data[T_RSTL_MEMB_SECT::RLT_WTRATIO_WEB].GetRatio() > 1.0) ? _T("NG") : _T("OK");
					record[i][5] = str;
				}
				else
				{
					if (SteelResult.dblCodeRatio[5] > 1.0e-9)
						str.Format(_T("%.3g"), 1.0 / SteelResult.dblCodeRatio[5]);
					else
						str = _T("-");
					record[i][2] = str;
					str = data.nEvaluateLevel[0] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[0]];
					record[i][3] = str;
					str = _T("-");
					if (SteelResult.res_dSlenLimit[0] > 1.0e-9)
					{
						str = SteelResult.res_dSlenValue[0] / SteelResult.res_dSlenLimit[0] > 1.0 ? _T("NG") : _T("OK");
					}
					if (SteelResult.res_dSlenLimit[1] > 1.0e-9)
					{
						if (SteelResult.res_bSlenComp[0] && SteelResult.res_dSlenValue[0] / SteelResult.res_dSlenLimit[0] < SteelResult.res_dSlenValue[1] / SteelResult.res_dSlenLimit[1])
							str = SteelResult.res_dSlenValue[1] / SteelResult.res_dSlenLimit[1] > 1.0 ? _T("NG") : _T("OK");
					}
					record[i][4] = str;
					if (SteelResult.res_dRatWithoutLT[3] > 1.0e-9 || SteelResult.res_dRatWithoutLT[1] > 1.0e-9)
					{
						str = (SteelResult.res_dRatWithoutLT[2] / SteelResult.res_dRatWithoutLT[3] > 1.0 || SteelResult.res_dRatWithoutLT[0] / SteelResult.res_dRatWithoutLT[1] > 1.0) ? _T("NG") : _T("OK");
						record[i][5] = str;
					}
					else
						record[i][5] = _T("-");

				}
			}		
			str = data.nEvaluateLevel[1] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[1]];
			record[i][6] = str;
		}
		int nIndex = m_lstBrace.GetCount();
		int k = 0;
		for (int i = 0; i < m_lstBraceCFS.GetCount(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetImel(m_lstBraceCFS[i], ImelD))
				ImelD.Initialize();
			ResData.Initial();
			k = i + nIndex;
			m_pDoc->m_pPostCtrl->CfsDgnMgr()->ReadDesignResult(m_lstBraceCFS[i], CfsData);
			CReinforceDgnMgr_CH::Instance()->GetEvaluateSecurity(m_lstBraceCFS[i], data);

			record[k].SetSize(7);
			record[k][0].Format(_T("%d"), m_lstBraceCFS[i]);
			record[k][1] = ImelD.bImportant ? strMemb[0] : strMemb[1];
			str.Format(_T("%.3g"), 1.0 / CfsData.m_Sect.m_Data[CH_RCFS_SECT::RLT_COMP].GetRatio());
			record[k][2] = str;
			str = data.nEvaluateLevel[0] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[0]];
			record[k][3] = str;
			str = (CfsData.m_Sect.m_Data[CH_RCFS_SECT::RLT_SLENDER_Y].GetRatio() > 1.0 || CfsData.m_Sect.m_Data[CH_RCFS_SECT::RLT_SLENDER_Z].GetRatio() > 1.0) ? _T("NG") : _T("OK");
			record[k][4] = str;
			str = (CfsData.m_Sect.m_Data[CH_RCFS_SECT::RLT_WHRTO_F].GetRatio() > 1.0 || CfsData.m_Sect.m_Data[CH_RCFS_SECT::RLT_WHRTO_W].GetRatio() > 1.0 || CfsData.m_Sect.m_Data[CH_RCFS_SECT::RLT_WHRTO_J].GetRatio() > 1.0) ? _T("NG") : _T("OK");
			record[k][5] = str;
			str = data.nEvaluateLevel[1] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[1]];
			record[k][6] = str;
		}

		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = m_lstBraceCFS.GetCount() + m_lstBrace.GetCount();
		TableD.RecordD.nRecordColCount = 7;
		if (m_lstBraceCFS.GetCount() + m_lstBrace.GetCount() > 0)
			MakeTableCenterText(genElements, TableD, _T("钢支撑构件承载能力评定等级"), _T("CHARPT_4_1_5_TBL1"));
	}

	//表4.1-6  钢桁架构件承载能力评定等级
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 7;
		TableD.HeaderD.aTitleName.Add(_T("构件号"));
		TableD.HeaderD.aTitleName.Add(_T("构件种类"));
		TableD.HeaderD.aTitleName.Add(_T("承载力验算比"));
		TableD.HeaderD.aTitleName.Add(_T("承载能力评价结果"));
		TableD.HeaderD.aTitleName.Add(_T("长细比验算"));
		TableD.HeaderD.aTitleName.Add(_T("宽厚比验算"));
		TableD.HeaderD.aTitleName.Add(_T("构造评价结果"));
		TableD.RecordD.aRecordData.SetSize(m_lstTrussCFS.GetCount() + m_lstTruss.GetCount());
		auto& record = TableD.RecordD.aRecordData;

		for (int i = 0; i < m_lstTruss.GetCount(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetImel(m_lstTruss[i], ImelD))
				ImelD.Initialize();
			CReinforceDgnMgr_CH::Instance()->GetEvaluateSecurity(m_lstTruss[i], data);

			record[i].SetSize(7);
			record[i][0].Format(_T("%d"), m_lstTruss[i]);
			record[i][1] = ImelD.bImportant ? strMemb[0] : strMemb[1];
			if (m_pDoc->m_pPostCtrl->GetDesignResult()->ReadSteelDesignResult(m_lstTruss[i], 0, 1, 0, SteelResult) && SteelResult.bCheck)
			{
				if (m_pDoc->m_pPostCtrl->StlDgnMgr()->ReadDesignResult(m_lstTruss[i], ResData))
				{
					str.Format(_T("%.3g"), 1.0 / ResData.m_Sect.m_Data[T_RSTL_MEMB_SECT::RLT_COMP].GetRatio());
					record[i][2] = str;
					str = data.nEvaluateLevel[0] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[0]];
					record[i][3] = str;
					record[i][4] = ResData.m_Sect.m_Data[T_RSTL_MEMB_SECT::RLT_SLENDER].GetRatio() > 1.0 ? _T("NG") : _T("OK");
					str = (ResData.m_Sect.m_Data[T_RSTL_MEMB_SECT::RLT_WTRATIO_FLG].GetRatio() > 1.0 || ResData.m_Sect.m_Data[T_RSTL_MEMB_SECT::RLT_WTRATIO_WEB].GetRatio() > 1.0) ? _T("NG") : _T("OK");
					record[i][5] = str;
				}
				else
				{
					if (SteelResult.dblCodeRatio[5] > 1.0e-9)
						str.Format(_T("%.3g"), 1.0 / SteelResult.dblCodeRatio[5]);
					else
						str = _T("-");
					record[i][2] = str;
					str = data.nEvaluateLevel[0] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[0]];
					record[i][3] = str;
					str = _T("-");
					if (SteelResult.res_dSlenLimit[0] > 1.0e-9)
					{
						str = SteelResult.res_dSlenValue[0] / SteelResult.res_dSlenLimit[0] > 1.0 ? _T("NG") : _T("OK");
					}
					if (SteelResult.res_dSlenLimit[1] > 1.0e-9)
					{
						if (SteelResult.res_bSlenComp[0] && SteelResult.res_dSlenValue[0] / SteelResult.res_dSlenLimit[0] < SteelResult.res_dSlenValue[1] / SteelResult.res_dSlenLimit[1])
							str = SteelResult.res_dSlenValue[1] / SteelResult.res_dSlenLimit[1] > 1.0 ? _T("NG") : _T("OK");
					}
					record[i][4] = str;
					if (SteelResult.res_dRatWithoutLT[3] > 1.0e-9 || SteelResult.res_dRatWithoutLT[1] > 1.0e-9)
					{
						str = (SteelResult.res_dRatWithoutLT[2] / SteelResult.res_dRatWithoutLT[3] > 1.0 || SteelResult.res_dRatWithoutLT[0] / SteelResult.res_dRatWithoutLT[1] > 1.0) ? _T("NG") : _T("OK");
						record[i][5] = str;
					}
					else
						record[i][5] = _T("-");

				}
			}
			
			str = data.nEvaluateLevel[1] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[1]];
			record[i][6] = str;
		}
		int nIndex = m_lstTruss.GetCount();
		int k = 0;
		for (int i = 0; i < m_lstTrussCFS.GetCount(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetImel(m_lstTrussCFS[i], ImelD))
				ImelD.Initialize();
			ResData.Initial();
			k = i + nIndex;
			m_pDoc->m_pPostCtrl->CfsDgnMgr()->ReadDesignResult(m_lstTrussCFS[i], CfsData);
			CReinforceDgnMgr_CH::Instance()->GetEvaluateSecurity(m_lstTrussCFS[i], data);

			record[k].SetSize(7);
			record[k][0].Format(_T("%d"), m_lstTrussCFS[i]);
			record[k][1] = ImelD.bImportant ? strMemb[0] : strMemb[1];
			str.Format(_T("%.3g"), 1.0 / CfsData.m_Sect.m_Data[CH_RCFS_SECT::RLT_COMP].GetRatio());
			record[k][2] = str;
			str = data.nEvaluateLevel[0] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[0]];
			record[k][3] = str;
			str = (CfsData.m_Sect.m_Data[CH_RCFS_SECT::RLT_SLENDER_Y].GetRatio() > 1.0 || CfsData.m_Sect.m_Data[CH_RCFS_SECT::RLT_SLENDER_Z].GetRatio() > 1.0) ? _T("NG") : _T("OK");
			record[k][4] = str;
			str = (CfsData.m_Sect.m_Data[CH_RCFS_SECT::RLT_WHRTO_F].GetRatio() > 1.0 || CfsData.m_Sect.m_Data[CH_RCFS_SECT::RLT_WHRTO_W].GetRatio() > 1.0 || CfsData.m_Sect.m_Data[CH_RCFS_SECT::RLT_WHRTO_J].GetRatio() > 1.0) ? _T("NG") : _T("OK");
			record[k][5] = str;
			str = data.nEvaluateLevel[1] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[1]];
			record[k][6] = str;
		}

		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = m_lstTrussCFS.GetCount() + m_lstTruss.GetCount();
		TableD.RecordD.nRecordColCount = 7;
		if (m_lstTruss.GetCount() + m_lstTrussCFS.GetCount() > 0)
			MakeTableCenterText(genElements, TableD, _T("钢桁架构件承载能力评定等级"), _T("CHARPT_4_1_6_TBL1"));
	}
}
void AutoReport_Gen_CH_APPRAISAL_Civil::WriteCharpt4_2(GenSegmentElements& genElements)
{
	MakeTitle(genElements, _T("构件的安全性评定等级"), 4, 2);
	CString str;
	str.Format(_T("    根据各构件的承载能力、构造以及不适于承载的位移或变形等评价等级，最终得到构件的安全性评级等级如下："));
	MakeText(genElements, str);

	T_IMEL_D ImelD;
	T_DSAS_D data;
	CString strMemb[] = { _T("主要构件"), _T("一般构件") };
	CString strGrade[] = { _T("au"),_T("bu") ,_T("cu") ,_T("du") };
	//4.2.1 钢梁构件安全性评定等级
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 7;
		TableD.HeaderD.aTitleName.Add(_T("构件号"));
		TableD.HeaderD.aTitleName.Add(_T("构件种类"));
		TableD.HeaderD.aTitleName.Add(_T("承载能力评价结果"));
		TableD.HeaderD.aTitleName.Add(_T("构件评价结果"));
		TableD.HeaderD.aTitleName.Add(_T("不适于承载的位移或变形"));
		TableD.HeaderD.aTitleName.Add(_T("其它"));
		TableD.HeaderD.aTitleName.Add(_T("安全性评价等级"));
		TableD.RecordD.aRecordData.SetSize(m_lstBeam_Dsau.GetCount() + m_lstBeamCFS_Dsau.GetCount());
		auto& record = TableD.RecordD.aRecordData;

		for (int i = 0; i < m_lstBeam_Dsau.GetCount(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetImel(m_lstBeam_Dsau[i], ImelD))
				ImelD.Initialize();
			CReinforceDgnMgr_CH::Instance()->GetEvaluateSecurity(m_lstBeam_Dsau[i], data);

			record[i].SetSize(7);
			record[i][0].Format(_T("%d"), m_lstBeam_Dsau[i]);
			record[i][1] = ImelD.bImportant ? strMemb[0] : strMemb[1];
			record[i][2] = data.nEvaluateLevel[0] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[0]];
			record[i][3] = data.nEvaluateLevel[1] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[1]];
			record[i][4] = data.nEvaluateLevel[2] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[2]];
			record[i][5] = data.nEvaluateLevel[3] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[3]];
			record[i][6] = data.GetLevel() == -1 ? _T("-") : strGrade[data.GetLevel()];
		}
		int nIndex = m_lstBeam_Dsau.GetCount();
		int k = 0;
		for (int i = 0; i < m_lstBeamCFS_Dsau.GetCount(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetImel(m_lstBeamCFS_Dsau[i], ImelD))
				ImelD.Initialize();
			CReinforceDgnMgr_CH::Instance()->GetEvaluateSecurity(m_lstBeamCFS_Dsau[i], data);
			k = i + nIndex;
			record[k].SetSize(7);
			record[k][0].Format(_T("%d"), m_lstBeamCFS_Dsau[i]);
			record[k][1] = ImelD.bImportant ? strMemb[0] : strMemb[1];
			record[i][2] = data.nEvaluateLevel[0] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[0]];
			record[i][3] = data.nEvaluateLevel[1] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[1]];
			record[i][4] = data.nEvaluateLevel[2] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[2]];
			record[i][5] = data.nEvaluateLevel[3] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[3]];
			record[i][6] = data.GetLevel() == -1 ? _T("-") : strGrade[data.GetLevel()];
		}

		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = m_lstBeam_Dsau.GetCount() + m_lstBeamCFS_Dsau.GetCount();
		TableD.RecordD.nRecordColCount = 7;
		if (m_lstBeam_Dsau.GetCount() + m_lstBeamCFS_Dsau.GetCount() > 0)
			MakeTableCenterText(genElements, TableD, _T("钢梁构件安全性评定等级"), _T("CHARPT_4_2_1_TBL"));
	}

	//表4.2.2  钢柱构件安全性评定等级
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 7;
		TableD.HeaderD.aTitleName.Add(_T("构件号"));
		TableD.HeaderD.aTitleName.Add(_T("构件种类"));
		TableD.HeaderD.aTitleName.Add(_T("承载能力评价结果"));
		TableD.HeaderD.aTitleName.Add(_T("构件评价结果"));
		TableD.HeaderD.aTitleName.Add(_T("不适于承载的位移或变形"));
		TableD.HeaderD.aTitleName.Add(_T("其它"));
		TableD.HeaderD.aTitleName.Add(_T("安全性评价等级"));
		TableD.RecordD.aRecordData.SetSize(m_lstCol_Dsau.GetCount() + m_lstColCFS_Dsau.GetCount());
		auto& record = TableD.RecordD.aRecordData;

		for (int i = 0; i < m_lstCol_Dsau.GetCount(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetImel(m_lstCol_Dsau[i], ImelD))
				ImelD.Initialize();
			CReinforceDgnMgr_CH::Instance()->GetEvaluateSecurity(m_lstCol_Dsau[i], data);
			record[i].SetSize(7);
			record[i][0].Format(_T("%d"), m_lstCol_Dsau[i]);
			record[i][1] = ImelD.bImportant ? strMemb[0] : strMemb[1];
			record[i][2] = data.nEvaluateLevel[0] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[0]];
			record[i][3] = data.nEvaluateLevel[1] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[1]];
			record[i][4] = data.nEvaluateLevel[2] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[2]];
			record[i][5] = data.nEvaluateLevel[3] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[3]];
			record[i][6] = data.GetLevel() == -1 ? _T("-") : strGrade[data.GetLevel()];
		}
		int nIndex = m_lstCol_Dsau.GetCount();
		int k = 0;
		for (int i = 0; i < m_lstColCFS_Dsau.GetCount(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetImel(m_lstColCFS_Dsau[i], ImelD))
				ImelD.Initialize();
			CReinforceDgnMgr_CH::Instance()->GetEvaluateSecurity(m_lstColCFS_Dsau[i], data);
			k = i + nIndex;
			record[k].SetSize(7);
			record[k][0].Format(_T("%d"), m_lstColCFS_Dsau[i]);
			record[k][1] = ImelD.bImportant ? strMemb[0] : strMemb[1];
			record[i][2] = data.nEvaluateLevel[0] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[0]];
			record[i][3] = data.nEvaluateLevel[1] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[1]];
			record[i][4] = data.nEvaluateLevel[2] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[2]];
			record[i][5] = data.nEvaluateLevel[3] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[3]];
			record[i][6] = data.GetLevel() == -1 ? _T("-") : strGrade[data.GetLevel()];
		}

		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = m_lstCol_Dsau.GetCount() + m_lstColCFS_Dsau.GetCount();
		TableD.RecordD.nRecordColCount = 7;
		if (m_lstCol_Dsau.GetCount() + m_lstColCFS_Dsau.GetCount() > 0)
			MakeTableCenterText(genElements, TableD, _T("钢柱构件安全性评定等级"), _T("CHARPT_4_2_2_TBL"));
	}

	//表4.2.3  钢支撑构件安全性评定等级
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 7;
		TableD.HeaderD.aTitleName.Add(_T("构件号"));
		TableD.HeaderD.aTitleName.Add(_T("构件种类"));
		TableD.HeaderD.aTitleName.Add(_T("承载能力评价结果"));
		TableD.HeaderD.aTitleName.Add(_T("构件评价结果"));
		TableD.HeaderD.aTitleName.Add(_T("不适于承载的位移或变形"));
		TableD.HeaderD.aTitleName.Add(_T("其它"));
		TableD.HeaderD.aTitleName.Add(_T("安全性评价等级"));
		TableD.RecordD.aRecordData.SetSize(m_lstBraceCFS_Dsau.GetCount() + m_lstBrace_Dsau.GetCount());
		auto& record = TableD.RecordD.aRecordData;

		for (int i = 0; i < m_lstBrace_Dsau.GetCount(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetImel(m_lstBrace_Dsau[i], ImelD))
				ImelD.Initialize();
			CReinforceDgnMgr_CH::Instance()->GetEvaluateSecurity(m_lstBrace_Dsau[i], data);
			record[i].SetSize(7);
			record[i][0].Format(_T("%d"), m_lstBrace_Dsau[i]);
			record[i][1] = ImelD.bImportant ? strMemb[0] : strMemb[1];
			record[i][2] = data.nEvaluateLevel[0] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[0]];
			record[i][3] = data.nEvaluateLevel[1] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[1]];
			record[i][4] = data.nEvaluateLevel[2] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[2]];
			record[i][5] = data.nEvaluateLevel[3] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[3]];
			record[i][6] = data.GetLevel() == -1 ? _T("-") : strGrade[data.GetLevel()];
		}
		int nIndex = m_lstBrace_Dsau.GetCount();
		int k = 0;
		for (int i = 0; i < m_lstBraceCFS_Dsau.GetCount(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetImel(m_lstBraceCFS_Dsau[i], ImelD))
				ImelD.Initialize();
			CReinforceDgnMgr_CH::Instance()->GetEvaluateSecurity(m_lstBraceCFS_Dsau[i], data);
			k = i + nIndex;
			record[k].SetSize(7);
			record[k][0].Format(_T("%d"), m_lstBraceCFS_Dsau[i]);
			record[k][1] = ImelD.bImportant ? strMemb[0] : strMemb[1];
			record[i][2] = data.nEvaluateLevel[0] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[0]];
			record[i][3] = data.nEvaluateLevel[1] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[1]];
			record[i][4] = data.nEvaluateLevel[2] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[2]];
			record[i][5] = data.nEvaluateLevel[3] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[3]];
			record[i][6] = data.GetLevel() == -1 ? _T("-") : strGrade[data.GetLevel()];
		}

		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = m_lstBraceCFS_Dsau.GetCount() + m_lstBrace_Dsau.GetCount();
		TableD.RecordD.nRecordColCount = 7;
		if (m_lstBraceCFS_Dsau.GetCount() + m_lstBrace_Dsau.GetCount() > 0)
			MakeTableCenterText(genElements, TableD, _T("钢支撑构件安全性评定等级"), _T("CHARPT_4_2_3_TBL"));
	}

	//表4.2.4  钢桁架构件使用性评定等级
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 7;
		TableD.HeaderD.aTitleName.Add(_T("构件号"));
		TableD.HeaderD.aTitleName.Add(_T("构件种类"));
		TableD.HeaderD.aTitleName.Add(_T("承载能力评价结果"));
		TableD.HeaderD.aTitleName.Add(_T("构件评价结果"));
		TableD.HeaderD.aTitleName.Add(_T("不适于承载的位移或变形"));
		TableD.HeaderD.aTitleName.Add(_T("其它"));
		TableD.HeaderD.aTitleName.Add(_T("安全性评价等级"));
		TableD.RecordD.aRecordData.SetSize(m_lstTruss_Dsau.GetCount() + m_lstTrussCFS_Dsau.GetCount());

		auto& record = TableD.RecordD.aRecordData;

		for (int i = 0; i < m_lstTruss_Dsau.GetCount(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetImel(m_lstTruss_Dsau[i], ImelD))
				ImelD.Initialize();
			CReinforceDgnMgr_CH::Instance()->GetEvaluateSecurity(m_lstTruss_Dsau[i], data);
			record[i].SetSize(7);
			record[i][0].Format(_T("%d"), m_lstTruss_Dsau[i]);
			record[i][1] = ImelD.bImportant ? strMemb[0] : strMemb[1];
			record[i][2] = data.nEvaluateLevel[0] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[0]];
			record[i][3] = data.nEvaluateLevel[1] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[1]];
			record[i][4] = data.nEvaluateLevel[2] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[2]];
			record[i][5] = data.nEvaluateLevel[3] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[3]];
			record[i][6] = data.GetLevel() == -1 ? _T("-") : strGrade[data.GetLevel()];
		}
		int nIndex = m_lstTruss_Dsau.GetCount();
		int k = 0;
		for (int i = 0; i < m_lstTrussCFS_Dsau.GetCount(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetImel(m_lstTrussCFS_Dsau[i], ImelD))
				ImelD.Initialize();
			CReinforceDgnMgr_CH::Instance()->GetEvaluateSecurity(m_lstTrussCFS_Dsau[i], data);
			k = i + nIndex;
			record[k].SetSize(7);
			record[k][0].Format(_T("%d"), m_lstTrussCFS_Dsau[i]);
			record[k][1] = ImelD.bImportant ? strMemb[0] : strMemb[1];
			record[i][2] = data.nEvaluateLevel[0] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[0]];
			record[i][3] = data.nEvaluateLevel[1] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[1]];
			record[i][4] = data.nEvaluateLevel[2] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[2]];
			record[i][5] = data.nEvaluateLevel[3] == -1 ? _T("-") : strGrade[data.nEvaluateLevel[3]];
			record[i][6] = data.GetLevel() == -1 ? _T("-") : strGrade[data.GetLevel()];
		}

		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = m_lstTruss_Dsau.GetCount() + m_lstTrussCFS_Dsau.GetCount();
		TableD.RecordD.nRecordColCount = 7;
		if (m_lstTruss_Dsau.GetCount() + m_lstTrussCFS_Dsau.GetCount() > 0)
			MakeTableCenterText(genElements, TableD, _T("钢桁架构件使用性评定等级"), _T("CHARPT_4_2_4_TBL"));
	}
}

void AutoReport_Gen_CH_APPRAISAL_Civil::WriteCharpt5()
{
	GenSegmentElements Elems;
	MakeTitle(Elems, _T("结构的抗震鉴定"), 5, 0);
	int nSFI[] = { 6,7,7,8,8,9 };
	double dSFI[] = { 0.05,0.10,0.15,0.20,0.30,0.40 };
	CString aStrSDC[] = { _T("一"),_T("二"),_T("三") };
	CString astrSC[] = { _T("I0"), _T("I1"),  _T("II"), _T("III"), _T("IV") };
	int nSfi = nSFI[0];
	double dSfi = dSFI[0];
	CString strSC = astrSC[0];
	CString strSDC = aStrSDC[0];
	int nSeisFi = 0;
	int nSiteClass = 0;
	int nSeisDgnCategory = 0;;
	BOOL bSplc = Para()->IsSpfc(m_pDoc, nSeisFi, nSiteClass, nSeisDgnCategory);
	if (bSplc)
	{
		nSfi = nSFI[nSeisFi];
		dSfi = dSFI[nSeisFi];
		strSC = astrSC[nSiteClass];
		strSDC = aStrSDC[nSeisDgnCategory];
	}
	CString str;
	str.Format(_T("    本工程抗震设防烈度为%s度，设计地震分组为第%s组，设计基本地震加速度值为%sg，场地类别为%s类。"), GetBold(nSfi), GetBold(strSDC), GetBold(dSfi), GetBold(strSC));
	MakeText(Elems, str);
	//
	str.Format(_T("    根据《建筑工程抗震设防分类标准》 GB 50223-2008，属于标准设防%s，抗震构造措施按 %s度。"), GetBold(_T("丙类")), GetBold(nSfi));
	MakeText(Elems, str);
	m_Segments.push_back(Elems);
}

void AutoReport_Gen_CH_APPRAISAL_Civil::WriteCharpt6()
{
	GenSegmentElements genElements;
	MakeTitle(genElements, _T("处理建议"), 6, 0);
	CString str;
	str.Format(_T("    根据规范3.3.1条，对于安全性评级为cu级的单个构件，其安全性不符合本标准对au级的规定，显著影响承载能力。应该采取措施，结构中安全性评级为cu级的构件见下表："));
	MakeText(genElements, str);
	
	GetTypeList();
	int nCountC = m_lstTypeC.GetCount();
	int nCountD = m_lstTypeD.GetCount();
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 4;
		TableD.HeaderD.aTitleName.Add(_T("构件号"));
		TableD.HeaderD.aTitleName.Add(_T("构件类型"));
		TableD.HeaderD.aTitleName.Add(_T("构件种类"));
		TableD.HeaderD.aTitleName.Add(_T("安全评级"));
		TableD.RecordD.aRecordData.SetSize(nCountC);
		int i = 0, k = -1;
		T_MATD_D MatdD;
		MakeStlMatlInfo();
		for (; i < nCountC; i++)
		{
			SetRltInfo(m_lstTypeC[i], TableD.RecordD.aRecordData, k);
		}
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = k + 1;;
		TableD.RecordD.nRecordColCount = 4;
		if (nCountC > 0)
			MakeTable(genElements, TableD, _T("可靠性评级为cu级的构件"), _T("CHARPT_6_1_TBL"));

	}

	str.Format(_T("    对于安全性评级为du级的构件，其安全性不符合本标准对au级的规定，已严重影响承载能力。必须及时或立即采取措施，结构中安全性评级为du的构件见下表："));
	MakeText(genElements, str);

	//
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 4;
		TableD.HeaderD.aTitleName.Add(_T("构件号"));
		TableD.HeaderD.aTitleName.Add(_T("构件类型"));
		TableD.HeaderD.aTitleName.Add(_T("构件种类"));
		TableD.HeaderD.aTitleName.Add(_T("安全评级"));
		TableD.RecordD.aRecordData.SetSize(nCountD);
		int i = 0, k = -1;
		T_MATD_D MatdD;
		MakeStlMatlInfo();
		for (; i < nCountD; i++)
		{
			SetRltInfo(m_lstTypeD[i], TableD.RecordD.aRecordData, k);
		}
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = k + 1;;
		TableD.RecordD.nRecordColCount = 4;
		if (nCountD > 0)
			MakeTable(genElements, TableD, _T("可靠性评级为du级的构件"), _T("CHARPT_6_2_TBL"));

	}

	m_Segments.push_back(genElements);
}

int AutoReport_Gen_CH_APPRAISAL_Civil::SetRltInfo(int key, CArray<CStringArray, CStringArray&>& strArray, int& nIndex)
{
	CString strKey;
	strKey.Format(_T("%d"), key);
	strKey = m_strFormat + strKey;
	CString strElem, strType;
	CString strGrade[] = { _T("au"),_T("bu") ,_T("cu") ,_T("du") };
	int iMembType = m_pDoc->m_pAttrCtrl->GetMemberType(key, FALSE);
	T_DSAS_D data;
	CReinforceDgnMgr_CH::Instance()->GetEvaluateSecurity(key, data);
	switch (iMembType)
	{
	case 1:	strElem = _T("柱");
		break;
	case 2:	strElem = _T("梁");
		break;
	case 3:	strElem = _T("支撑");
		break;
	case 5:	strElem = _T("桁架");
		break;
	default:
		break;
	}
	T_IMEL_D ImelD; 
	if (!m_pDoc->m_pAttrCtrl->GetImel(key, ImelD))
		ImelD.Initialize();
	if (ImelD.bImportant)
		strType = _T("主要构件");
	else
		strType = _T("一般构件") ;

	strArray[++nIndex].Add(strKey);
	strArray[nIndex].Add(m_strFormat + strElem);
	strArray[nIndex].Add(m_strFormat + strType);
	if (data.GetLevel() == -1)
		strArray[nIndex].Add(m_strFormat + _T("-"));
	else
		strArray[nIndex].Add(m_strFormat + strGrade[data.GetLevel()]);

	return nIndex;
}
int AutoReport_Gen_CH_APPRAISAL_Civil::SetStlInfo(int key, const T_MATD_D& data, CArray<CStringArray, CStringArray&>& strArray, int& nIndex)
{
	CString strKey;
	strKey.Format(_T("%d"), key);
	strKey = m_strFormat + strKey;
	auto& vstr = m_mapstlInfo[std::pair<CString, CString>(data.Data1.CodeName, data.Data1.CodeMatlName)];
	if (vstr.empty())
	{
		strArray[++nIndex].Add(strKey);
		strArray[nIndex].Add(m_strFormat + data.Data1.CodeName);
		strArray[nIndex].Add(m_strFormat + data.Name);
		double dfy = 0.0, dfv = 0.0;
		T_MATD_D designdata;
		CString strfy, strfv;
		if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(key, designdata))
		{
			strfy = m_strFormat + _T("-");
			strfy = m_strFormat + _T("-");
		}
		else
		{
			strfy.Format(_T("%d"), (int)designdata.Data1.Design.S_Fy1);
			strfv.Format(_T("%d"), (int)(designdata.Data1.Design.S_Fy1 / 1.732));
			strfy = m_strFormat + strfy;
			strfv = m_strFormat + strfv;
		}
		strArray[nIndex].Add(strfy);
		strArray[nIndex].Add(strfv);
		strArray[nIndex].Add(m_strFormat + _T("-"));
		return nIndex;
	}
	int nCol = 0;
	for (auto it = vstr.begin();it != vstr.end();it++, nCol++)
	{
		if (it->empty())
			break;
		if (nCol % 4 == 0)
		{
			strArray[++nIndex].Add(strKey);
			strArray[nIndex].Add(m_strFormat + data.Data1.CodeName);
			strArray[nIndex].Add(m_strFormat + data.Data1.CodeMatlName);
		}
		strArray[nIndex].Add(m_strFormat + it->c_str());
	}
	return nIndex;
}
void AutoReport_Gen_CH_APPRAISAL_Civil::setTableFormatString(int nlocation /* = 0 */, int ncolor /* = 16777215 */)
{
	CString str[] = { _T("center"),_T("left"),_T("right") };
	m_strFormat.Format(_T("[DRG_CELL_PROPERTY]color=0 align=%s shape=%d tbwidth=10%%[/DRG_CELL_PROPERTY]"), str[nlocation], ncolor);
}
void AutoReport_Gen_CH_APPRAISAL_Civil::MakeStlMatlInfo()
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
	auto setstr = [&](const CString& str1, const CString& str2, int nIndex)
		{
			std::vector<wstring> strarray;
			for (int i = 0;i < 20;i++)
			{
				strarray.push_back(str[nIndex][i]);
			}
			m_mapstlInfo[std::pair<CString, CString>(str1, str2)] = move(strarray);
		};
	setstr(MATLCODE_STL_GB03, _T("Q235"), 0);
	setstr(MATLCODE_STL_GB03, _T("Q345"), 1);
	setstr(MATLCODE_STL_GB03, _T("Q390"), 2);
	setstr(MATLCODE_STL_GB03, _T("Q420"), 3);

	setstr(MATLCODE_STL_GB50017_17, _T("Q235"), 4);
	setstr(MATLCODE_STL_GB50017_17, _T("Q345"), 5);
	setstr(MATLCODE_STL_GB50017_17, _T("Q355"), 6);
	setstr(MATLCODE_STL_GB50017_17, _T("Q345GJ"), 7);
	setstr(MATLCODE_STL_GB50017_17, _T("Q390"), 8);
	setstr(MATLCODE_STL_GB50017_17, _T("Q420"), 9);
	setstr(MATLCODE_STL_GB50017_17, _T("Q460"), 10);

	setstr(MATLCODE_STL_JGJ2015, _T("Q235"), 11);
	setstr(MATLCODE_STL_JGJ2015, _T("Q345"), 12);
	setstr(MATLCODE_STL_JGJ2015, _T("Q345GJ"), 13);
	setstr(MATLCODE_STL_JGJ2015, _T("Q390"), 14);
	setstr(MATLCODE_STL_JGJ2015, _T("Q420"), 15);
}

BOOL AutoReport_Gen_CH_APPRAISAL_Civil::GetStlList()
{
	T_KEY_LIST lstStl;
	CString strCode = CDBLib::GetStlCodeName();
	if (strCode == _T("GB50017-17"))
	{
		if (!m_pDoc->m_pPostCtrl->StlDgnMgr()->GetHaveDgnKeys(lstStl))
			return FALSE;
	}
	else
	{
		if (!m_pDoc->m_pPostCtrl->StlDgnMgr()->GetDgnKeys(lstStl))
			return FALSE;
	}

	T_DSAS_D data;
	for (int i = 0; i < lstStl.GetCount(); i++)
	{
		data.Initialize();
		CReinforceDgnMgr_CH::Instance()->GetEvaluateSecurity(lstStl[i], data);
		if (data.GetLevel() < 1)
			continue;
		int iMembType = m_pDoc->m_pAttrCtrl->GetMemberType(lstStl[i], FALSE);
		switch (iMembType)
		{
		case 1:	m_lstCol.Add(lstStl[i]);	break;
		case 2:	m_lstBeam.Add(lstStl[i]);	break;
		case 3:	m_lstBrace.Add(lstStl[i]);	break;
		case 5:	m_lstTruss.Add(lstStl[i]);	break;
		default:
			break;
		}
	}

	T_DSAU_D uData;
	for (int i = 0; i < lstStl.GetCount(); i++)
	{
		uData.Initialize();
		CReinforceDgnMgr_CH::Instance()->GetEvaluateUsability(lstStl[i], uData);
		if (uData.GetLevel() < 1)
			continue;
		int iMembType = m_pDoc->m_pAttrCtrl->GetMemberType(lstStl[i], FALSE);
		switch (iMembType)
		{
		case 1:	m_lstCol_Dsau.Add(lstStl[i]);	break;
		case 2:	m_lstBeam_Dsau.Add(lstStl[i]);	break;
		case 3:	m_lstBrace_Dsau.Add(lstStl[i]);	break;
		case 5:	m_lstTruss_Dsau.Add(lstStl[i]);	break;
		default:
			break;
		}
	}

	return TRUE;
}
BOOL AutoReport_Gen_CH_APPRAISAL_Civil::GetCFSList()
{
	T_KEY_LIST lstCFS;
	if (!m_pDoc->m_pPostCtrl->CfsDgnMgr()->GetHaveDgnKeys(lstCFS))
		return FALSE;

	T_DSAS_D data;
	for (int i = 0; i < lstCFS.GetCount(); i++)
	{
		data.Initialize();
		CReinforceDgnMgr_CH::Instance()->GetEvaluateSecurity(lstCFS[i], data);
		if (data.GetLevel() < 1)
			continue;
		int iMembType = m_pDoc->m_pAttrCtrl->GetMemberType(lstCFS[i], FALSE);
		switch (iMembType)
		{
		case 1: m_lstColCFS.Add(lstCFS[i]);	break;
		case 2: m_lstBeamCFS.Add(lstCFS[i]);	break;
		case 3: m_lstBraceCFS.Add(lstCFS[i]);	break;
		case 5: m_lstTrussCFS.Add(lstCFS[i]);	break;
		default:
			break;
		}
	}

	T_DSAU_D uData;
	for (int i = 0; i < lstCFS.GetCount(); i++)
	{
		uData.Initialize();
		CReinforceDgnMgr_CH::Instance()->GetEvaluateUsability(lstCFS[i], uData);
		if (uData.GetLevel() < 1)
			continue;
		int iMembType = m_pDoc->m_pAttrCtrl->GetMemberType(lstCFS[i], FALSE);
		switch (iMembType)
		{
		case 1:	m_lstColCFS_Dsau.Add(lstCFS[i]);	break;
		case 2:	m_lstBeamCFS_Dsau.Add(lstCFS[i]);	break;
		case 3:	m_lstBraceCFS_Dsau.Add(lstCFS[i]);	break;
		case 5:	m_lstTrussCFS_Dsau.Add(lstCFS[i]);	break;
		default:
			break;
		}
	}

	return TRUE;
}

BOOL AutoReport_Gen_CH_APPRAISAL_Civil::GetTypeList()
{
	T_KEY_LIST lstStl;
	CString strCode = CDBLib::GetStlCodeName();
	if (strCode == _T("GB50017-17"))
	{
		if (!m_pDoc->m_pPostCtrl->StlDgnMgr()->GetHaveDgnKeys(lstStl))
			return FALSE;
	}
	else
	{
		if (!m_pDoc->m_pPostCtrl->StlDgnMgr()->GetDgnKeys(lstStl))
			return FALSE;
	}
	T_DSAS_D sData;
	m_lstTypeC.RemoveAll();
	m_lstTypeD.RemoveAll();
	for (int i = 0; i < lstStl.GetCount(); i++)
	{
		CReinforceDgnMgr_CH::Instance()->GetEvaluateSecurity(lstStl[i], sData);

		if (sData.GetLevel() == 2)
			m_lstTypeC.Add(lstStl[i]);
		else if (sData.GetLevel() == 3)
			m_lstTypeD.Add(lstStl[i]);
		else
			continue;
	}

	return TRUE;
}