#include "stdafx.h"
#include "AutoReport_Gen_CH_DMGB.h"
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
#include "..\wg_db\DamperGBTool.h"
#include "..\wg_db\IsolatorTool.h"
#include "..\wg_db\ThrsAutoNamer.h"
#include "..\wg_db\DB_ST_DT_FIGR.h"
#include "..\wg_db\wg_db_InitCtrl.h"
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

//#include "..\wg_gr\I_GENModel.h"
//#include "..\wg_gr\GRDisplayDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const double cGen_Zero	  = 1.0E-07;

struct _DMGB_Data_Ratio
{
	double dResRatio[6];	// RSx,RSy,RSz,RBy,RBz,Rcomb.
	void Initialize()
	{
		for(int i=0; i<6; i++)	{dResRatio[i] = 0.0;}
	}
};

struct _DMGB_Data_Map
{
	CMap<UINT, const UINT&, _DMGB_Data_Ratio, _DMGB_Data_Ratio&> SortLcbData;
	void Initialize()
	{
		SortLcbData.RemoveAll();
	}
	_DMGB_Data_Map()	{}
	_DMGB_Data_Map(_DMGB_Data_Map& sData)	{*this = sData;}
	_DMGB_Data_Map& operator = (_DMGB_Data_Map& sData)
	{
		_DMGB_Data_Ratio SortData;
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

Report_Para_CH* AutoReport_Gen_CH_DMGB::Para() const
{
		return AutoReport_Gen_Mgr::Inst()->Para();
}

CString AutoReport_Gen_CH_DMGB::MakePicture_1_1()
{
	CString strName = _T("ALL-结构模型图");
	SetPreOrPostMode(FALSE);
	DrawNodeAll(FALSE);
	SetViewPoint(0);
	MakeImage_Current(strName, FALSE);
	return strName;
}

int AutoReport_Gen_CH_DMGB::TempletID()
{
		return 0;
}
BOOL AutoReport_Gen_CH_DMGB::PreMakeData()
{
	Initial();
	m_Segments.clear();
	CViewBase::GetCurView_ST()->DoInitialView();
	SetShape(FALSE);
	//
	GRenderView* pRenderView = (GRenderView*)(I_GENModelBase::GetCurMySelfST()->GetOwnerWnd()); ASSERT(pRenderView);
	CAutoRecoveryUnit unitSet(D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_FORCE_INDEX_N, m_pDoc->m_pUnitCtrl,TRUE); 

	if (!m_pDoc->m_pAttrCtrl->GetDamping(m_DagbD))
		m_DagbD.Initialize();

	I_GENModelBase	*gm = I_GENModelBase::GetCurMySelfST();
	gm->GetDispSetting()->InitForInitialView();
	gm->GetDispSetting()->m_Node.bNode = FALSE;
	gm->GetDispSetting()->m_View.bVPoint = FALSE;
	
	BOOL bDetail = FALSE;
	Para()->GetAdditionalDampRtoPara(m_dKesi, m_dEta, bDetail);

	WriteCharpt1();
	WriteCharpt2();
	WriteCharpt3();
	WriteCharpt4();
	WriteCharpt5();
	WriteCharpt6();
	WriteCharpt7();
	if (bDetail)
	{
		WriteAdditionalDamperRto();
	}
	return TRUE;
}
void AutoReport_Gen_CH_DMGB::InsertCoverPage()
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
void AutoReport_Gen_CH_DMGB::InsertCatalogPage()
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
void AutoReport_Gen_CH_DMGB::MakeContent()
{   
	for (int i=0; i<m_Segments.size(); i++)
	{
		InsertSegmentElements(m_Segments[i]);
	}    
}
void AutoReport_Gen_CH_DMGB::WriteCharpt1()
{
	GenSegmentElements genElements;
	MakeTitle(genElements, _T("工程概况"),1,0);
	
	CString strStruct, str1, str2, str3;
	switch (m_DagbD.nStrutCategory)
	{
	case 0:	strStruct = _T("钢筋混凝土框架结构");	break;
	case 1:	strStruct = _T("钢筋混凝土框架-抗震墙结构");	break;
	case 2:	strStruct = _T("框架-核心筒结构");	break;
	case 3:	strStruct = _T("板柱-抗震墙结构");	break;
	case 4:	strStruct = _T("钢筋混凝土抗震墙结构");	break;
	case 5:	strStruct = _T("筒中筒");	break;
	case 6:	strStruct = _T("钢筋混凝土框支层结构");	break;
	case 7:	strStruct = _T("多高层钢结构");	break;
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
	
	double dHeight = m_pDoc->m_pAttrCtrl->GetBldgHeight()/1000;
	str3.Format(_T("建筑结构高度为%.1f m。模型视图如图1-1"), dHeight);
	MakeText(genElements, str1+str2+str3);
	//
	Report_Key_LIST aGroupKey;
	Para()->GroupKeys(aGroupKey, ARGC_DMGB * 1000 + 0);
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
void AutoReport_Gen_CH_DMGB::WriteCharpt2()
{
	GenSegmentElements genElements;
	MakeTitle(genElements, _T("设计依据"), 2, 0);
	CString str;
	if (m_DagbD.nCode == 0)
		MakeAlignText(str, _T("(1)《基于保持建筑正常使用功能的抗震技术导则》"), _T("(RISN-TG046-2023)"));
	else
		MakeAlignText(str, _T("(1)《建筑工程减隔震技术规程》北京市地方标准"), _T("(DB11/2075-2022)"));
	MakeAlignText(str, _T("(2)《建筑消能减震技术规程》"), _T("(JGJ297-2013)"));
	MakeAlignText(str, _T("(3)《建筑消能阻尼器》"), _T("(JGT209-2012)"));
	MakeAlignText(str, _T("(4)《工程结构通用规范》"), _T("(GB55001－2021)"));
	MakeAlignText(str, _T("(5)《建筑抗震设防分类标准》"), _T("(GB50223－2008)"));
	MakeAlignText(str, _T("(6)《建筑结构荷载规范》"), _T("(GB50009－2012)"));
	MakeAlignText(str, _T("(7)《建筑结构可靠性设计统一标准》"), _T("(GB50068－2018)"));
	MakeAlignText(str, _T("(8)《建筑抗震设计规范》"), _T("(GB50011－2010（2016修订版）)"));
	MakeAlignText(str, _T("(9)《混凝土结构设计规范》"), _T("(GB50010－2010（2015修订版）)"));
	MakeAlignText(str, _T("(10)《高层建筑混凝土结构技术规程》"), _T("(JGJ3－2010)"));
	MakeAlignText(str, _T("(11)《建筑地基基础设计规范》"), _T("(GB50007－2011)"));

	MakeText(genElements, str);

	m_Segments.push_back(genElements);
}

void AutoReport_Gen_CH_DMGB::WriteCharpt3  ()
{
	GenSegmentElements Elems;
	MakeTitle(Elems, _T("消能器布置"), 3, 0);
	WriteCharpt3_1(Elems);
	WriteCharpt3_2(Elems);
	WriteCharpt3_3(Elems);
	m_Segments.push_back(Elems);
}
void AutoReport_Gen_CH_DMGB::WriteCharpt3_1(GenSegmentElements &genElements)
{
	MakeTitle(genElements, _T("消能器布置") ,3, 1);  

	//数据统计
	T_KEY_LIST lstNlnk, lstBRB, lstMYD, lstFD, lstVFD, lstHDRD, lstVED, lstHor, lstVer;
	m_pDoc->m_pAttrCtrl->GetNlnkKeyList(lstNlnk);
	T_DMGB_D DmgbD;
	T_NLNK_D NlnkD;
	T_NLLP_D NllpD;
	int nDir = 0;
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
		if (NllpD.nApplicationType != 2 || NllpD.nSeisType != 6)
			continue;
		DmgbD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD))
		{
			ASSERT(0);
			continue;
		}

		nDir = IDamperGBTool::Instance()->GetNlnkDamperType(lstNlnk[i]);
		if (nDir == 0)
			lstHor.Add(lstNlnk[i]);
		else if (nDir == 1)
			lstVer.Add(lstNlnk[i]);
		if (DmgbD.nPropType == 0)
		{
			lstBRB.Add(lstNlnk[i]);
		}
		else if (DmgbD.nPropType == 1)
		{
			lstMYD.Add(lstNlnk[i]);
		}
		else if (DmgbD.nPropType == 2)
		{
			lstFD.Add(lstNlnk[i]);
		}
		else if (DmgbD.nPropType == 3)
		{
			lstVFD.Add(lstNlnk[i]);
		}
		else if (DmgbD.nPropType == 4)
		{
			lstHDRD.Add(lstNlnk[i]);
		}
		else if (DmgbD.nPropType == 5)
		{
			lstVED.Add(lstNlnk[i]);
		}
	}
	int nSum = lstBRB.GetCount() + lstMYD.GetCount() + lstFD.GetCount() + lstVFD.GetCount() + lstHDRD.GetCount() + lstVED.GetCount();
	CString str1;
	str1.Format(_T("    结构中共布置消能器%d个，其中X向消能器%d个，Y向消能器%d个。具体布置如下图所示："), nSum, lstHor.GetCount(), lstVer.GetCount());
	MakeText(genElements, str1);

	SetViewPoint(0);
	CString strName;
	strName = _T("消能器布置图");
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
}
void AutoReport_Gen_CH_DMGB::WriteCharpt3_2(GenSegmentElements &genElements)
{
	MakeTitle(genElements, _T("消能器列表"), 3, 2);

	//数据统计
	T_KEY_LIST lstNlnk, lstBRB, lstMYD, lstFD, lstVFD, lstHDRD, lstVED, lstHor, lstVer;
	m_pDoc->m_pAttrCtrl->GetNlnkKeyList(lstNlnk);
	T_DMGB_D DmgbD;
	T_NLNK_D NlnkD;
	T_NLLP_D NllpD;
	int nDir = 0;
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
		if (NllpD.nApplicationType != 2 || NllpD.nSeisType != 6)
			continue;
		DmgbD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD))
		{
			ASSERT(0);
			continue;
		}

		nDir = IDamperGBTool::Instance()->GetNlnkDamperType(lstNlnk[i]);
		if (nDir == 0)
			lstHor.Add(lstNlnk[i]);
		else if (nDir == 1)
			lstVer.Add(lstNlnk[i]);
		if (DmgbD.nPropType == 0)
		{
			lstBRB.Add(lstNlnk[i]);
		}
		else if (DmgbD.nPropType == 1)
		{
			lstMYD.Add(lstNlnk[i]);
		}
		else if (DmgbD.nPropType == 2)
		{
			lstFD.Add(lstNlnk[i]);
		}
		else if (DmgbD.nPropType == 3)
		{
			lstVFD.Add(lstNlnk[i]);
		}
		else if (DmgbD.nPropType == 4)
		{
			lstHDRD.Add(lstNlnk[i]);
		}
		else if (DmgbD.nPropType == 5)
		{
			lstVED.Add(lstNlnk[i]);
		}
	}

	CAutoRecoveryUnit unitSet(D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_FORCE_INDEX_KN, m_pDoc->m_pUnitCtrl, TRUE);
	CString str1, strName;
	//Table 1 - BRB
	CString strDof[6] = { _T("DX"), _T("DY"), _T("DZ"), _T("RX"), _T("RY"), _T("RZ") };
	int nIndex = 1;
	if (lstBRB.GetSize() > 0)
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 6;
		TableD.HeaderD.aTitleName.Add(_T("编号"));
		TableD.HeaderD.aTitleName.Add(_T("名称"));
		TableD.HeaderD.aTitleName.Add(_T("自由度"));
		TableD.HeaderD.aTitleName.Add(_T("屈服强度(kN)"));
		TableD.HeaderD.aTitleName.Add(_T("屈服位移(mm)"));
		TableD.HeaderD.aTitleName.Add(_T("屈服后刚度比"));
		//  
		TableD.RecordD.aRecordData.SetSize(lstBRB.GetSize());
		auto& record = TableD.RecordD.aRecordData;
		for (int i = 0; i < lstBRB.GetSize(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstBRB[i], NlnkD))
			{
				ASSERT(0);
				continue;
			}
			if (!m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
			{
				ASSERT(0);
				continue;
			}
			m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD);
			m_strFormat.Format(_T("%d"), lstBRB[i]);
			record[i].Add(m_strFormat);
			record[i].Add(DmgbD.PropName);
			record[i].Add(strDof[DmgbD.BRB.nDof]);
			record[i].Add(Double2String_f(DmgbD.BRB.dFy, 0));
			record[i].Add(Double2String_f(DmgbD.BRB.dDisp, 2));
			record[i].Add(Double2String_f(DmgbD.BRB.dStiffRot, 3));
		}
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = lstBRB.GetSize();
		TableD.RecordD.nRecordColCount = 6;
		str1.Format(_T("CHARPT_3_1_1_TBL%d"), nIndex);
		MakeTableCenterText(genElements, TableD, _T("屈曲约束支撑(BRB)表格统计"), str1);
		nIndex++;
	}
	//Table 2 - MYD
	if (lstMYD.GetSize() > 0)
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 6;
		TableD.HeaderD.aTitleName.Add(_T("编号"));
		TableD.HeaderD.aTitleName.Add(_T("名称"));
		TableD.HeaderD.aTitleName.Add(_T("自由度"));
		TableD.HeaderD.aTitleName.Add(_T("屈服强度(kN)"));
		TableD.HeaderD.aTitleName.Add(_T("屈服位移(mm)"));
		TableD.HeaderD.aTitleName.Add(_T("屈服后刚度比"));

		//  
		TableD.RecordD.aRecordData.SetSize(lstMYD.GetSize());
		auto& record = TableD.RecordD.aRecordData;
		for (int i = 0; i < lstMYD.GetSize(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstMYD[i], NlnkD))
			{
				ASSERT(0);
				continue;
			}
			if (!m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
			{
				ASSERT(0);
				continue;
			}
			m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD);
			m_strFormat.Format(_T("%d"), lstMYD[i]);
			record[i].Add(m_strFormat);
			record[i].Add(DmgbD.PropName);
			record[i].Add(strDof[DmgbD.MYD.nDof]);
			record[i].Add(Double2String_f(DmgbD.MYD.dFy, 0));
			record[i].Add(Double2String_f(DmgbD.MYD.dDisp, 2));
			record[i].Add(Double2String_f(DmgbD.MYD.dStiffRot,3));

		}
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = lstMYD.GetSize();
		TableD.RecordD.nRecordColCount = 6;
		str1.Format(_T("CHARPT_3_1_1_TBL%d"), nIndex);
		MakeTableCenterText(genElements, TableD, _T("金属屈服型消能器(MYD)表格统计"), str1);
		nIndex++;
	}
	//Table 3 - FD
	if (lstFD.GetSize() > 0)
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 6;
		TableD.HeaderD.aTitleName.Add(_T("编号"));
		TableD.HeaderD.aTitleName.Add(_T("名称"));
		TableD.HeaderD.aTitleName.Add(_T("自由度"));
		TableD.HeaderD.aTitleName.Add(_T("起滑摩擦力(kN)"));
		TableD.HeaderD.aTitleName.Add(_T("起滑位移(mm)"));
		TableD.HeaderD.aTitleName.Add(_T("初始刚度(kN/mm)"));

		//  
		TableD.RecordD.aRecordData.SetSize(lstFD.GetSize());
		auto& record = TableD.RecordD.aRecordData;
		for (int i = 0; i < lstFD.GetSize(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstFD[i], NlnkD))
			{
				ASSERT(0);
				continue;
			}
			if (!m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
			{
				ASSERT(0);
				continue;
			}
			m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD);
			m_strFormat.Format(_T("%d"), lstFD[i]);
			record[i].Add(m_strFormat);
			record[i].Add(DmgbD.PropName);
			record[i].Add(strDof[DmgbD.FD.nDof]);
			record[i].Add(Double2String_f(DmgbD.FD.dForce, 0));
			record[i].Add(Double2String_f(DmgbD.FD.dDisp, 2));
			record[i].Add(Double2String_f(DmgbD.FD.dInitStiff, 2));
		}
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = lstFD.GetSize();
		TableD.RecordD.nRecordColCount = 6;
		str1.Format(_T("CHARPT_3_1_TBL%d"), nIndex);
		MakeTableCenterText(genElements, TableD, _T("摩擦消能器(FD)表格统计"), str1);
		nIndex++;
	}
	//Table 4 - HDRD
	if (lstHDRD.GetSize() > 0)
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 7;
		TableD.HeaderD.aTitleName.Add(_T("编号"));
		TableD.HeaderD.aTitleName.Add(_T("名称"));
		TableD.HeaderD.aTitleName.Add(_T("自由度"));
		TableD.HeaderD.aTitleName.Add(_T("屈服强度(kN)"));
		TableD.HeaderD.aTitleName.Add(_T("屈服后刚度比"));
		TableD.HeaderD.aTitleName.Add(_T("等效阻尼比100%(%)"));
		TableD.HeaderD.aTitleName.Add(_T("水平等效刚度100%(kN/mm)"));
		//  
		TableD.RecordD.aRecordData.SetSize(lstHDRD.GetSize());
		auto& record = TableD.RecordD.aRecordData;
		for (int i = 0; i < lstHDRD.GetSize(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstHDRD[i], NlnkD))
			{
				ASSERT(0);
				continue;
			}
			if (!m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
			{
				ASSERT(0);
				continue;
			}
			m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD);
			m_strFormat.Format(_T("%d"), lstHDRD[i]);
			record[i].Add(m_strFormat);
			record[i].Add(DmgbD.PropName);
			record[i].Add(strDof[DmgbD.HDRD.nDof]);
			record[i].Add(Double2String_f(DmgbD.HDRD.dFy, 0));
			record[i].Add(Double2String_f(DmgbD.HDRD.dYieldStiffRot, 3));
			record[i].Add(Double2String_f(DmgbD.HDRD.dEquiDampRto,2));
			record[i].Add(Double2String_f(DmgbD.HDRD.dEquiStiff, 2));
		}
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = lstHDRD.GetSize();
		TableD.RecordD.nRecordColCount = 7;
		str1.Format(_T("CHARPT_3_1_1_TBL%d"), nIndex);
		MakeTableCenterText(genElements, TableD, _T("高阻尼橡胶消能器(HDRD)表格统计"), str1);
		nIndex++;
	}
	strName = _T("2. 速度型阻尼器统计");
	MakeText(genElements, strName);
	nIndex = 1;
	//Table 1 - VFD
	if (lstVFD.GetSize() > 0)
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 7;
		TableD.HeaderD.aTitleName.Add(_T("编号"));
		TableD.HeaderD.aTitleName.Add(_T("名称"));
		TableD.HeaderD.aTitleName.Add(_T("自由度"));
		TableD.HeaderD.aTitleName.Add(_T("消能器阻尼(kN)"));
		TableD.HeaderD.aTitleName.Add(_T("速度指数"));
		TableD.HeaderD.aTitleName.Add(_T("设计阻尼力"));
		TableD.HeaderD.aTitleName.Add(_T("设计位移(mm)"));
		//  
		TableD.RecordD.aRecordData.SetSize(lstVFD.GetSize());
		auto& record = TableD.RecordD.aRecordData;
		for (int i = 0; i < lstVFD.GetSize(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstVFD[i], NlnkD))
			{
				ASSERT(0);
				continue;
			}
			if (!m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
			{
				ASSERT(0);
				continue;
			}
			m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD);
			m_strFormat.Format(_T("%d"), lstVFD[i]);
			record[i].Add(m_strFormat);
			record[i].Add(DmgbD.PropName);
			record[i].Add(strDof[DmgbD.VFD.nDof]);
			record[i].Add(Double2String_f(DmgbD.VFD.dDamperF, 0));
			record[i].Add(Double2String_f(DmgbD.VFD.dIndex));
			record[i].Add(Double2String_f(DmgbD.VFD.dDesignF, 0));
			record[i].Add(Double2String_f(DmgbD.VFD.dDisp, 2));
		}
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = lstVFD.GetSize();
		TableD.RecordD.nRecordColCount = 7;
		str1.Format(_T("CHARPT_3_1_2_TBL%d"), nIndex);
		MakeTableCenterText(genElements, TableD, _T("粘滞消能器(VFD)表格统计"), str1);
		nIndex++;
	}
	//Table 2 - VED
	if (lstVED.GetSize() > 0)
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 7;
		TableD.HeaderD.aTitleName.Add(_T("编号"));
		TableD.HeaderD.aTitleName.Add(_T("名称"));
		TableD.HeaderD.aTitleName.Add(_T("自由度"));
		TableD.HeaderD.aTitleName.Add(_T("消能器阻尼(kN)"));
		TableD.HeaderD.aTitleName.Add(_T("速度指数"));
		TableD.HeaderD.aTitleName.Add(_T("设计阻尼力(kN)"));
		TableD.HeaderD.aTitleName.Add(_T("有效刚度(kN/mm)"));
		//  
		TableD.RecordD.aRecordData.SetSize(lstVED.GetSize());
		auto& record = TableD.RecordD.aRecordData;
		for (int i = 0; i < lstVED.GetSize(); i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstVED[i], NlnkD))
			{
				ASSERT(0);
				continue;
			}
			if (!m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
			{
				ASSERT(0);
				continue;
			}
			m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD);
			m_strFormat.Format(_T("%d"), lstVED[i]);
			record[i].Add(m_strFormat);
			record[i].Add(DmgbD.PropName);
			record[i].Add(strDof[DmgbD.VED.nDof]);
			record[i].Add(Double2String_f(DmgbD.VED.dDamperF, 0));
			record[i].Add(Double2String_f(DmgbD.VED.dIndex));
			record[i].Add(Double2String_f(DmgbD.VED.dDesignF, 0));
			record[i].Add(Double2String_f(DmgbD.VED.dEffectStiff, 2));
		}
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = lstVED.GetSize();
		TableD.RecordD.nRecordColCount = 7;
		str1.Format(_T("CHARPT_3_1_2_TBL%d"), nIndex);
		MakeTableCenterText(genElements, TableD, _T("粘弹性消能器(VED)表格统计"), str1);
		//nIndex++;
	}
}
void AutoReport_Gen_CH_DMGB::WriteCharpt3_3(GenSegmentElements &genElements)
{
	MakeTitle(genElements, _T("消能器使用情况统计"), 3, 3);

	//   
	//Table data
	T_KEY_LIST lstNlnk;
	T_KEY_LIST lstDmgb;
	T_KEY_LIST lstH_BRB, lstV_BRB, lstH_MYD, lstV_MYD, lstH_FD, lstV_FD, lstH_HDRD, lstV_HDRD, lstH_VFD, lstV_VFD, lstH_VED, lstV_VED;
	CMap<UINT, UINT, int, int> mapNllp_Num;
	m_pDoc->m_pAttrCtrl->GetNlnkKeyList(lstNlnk);
	T_DMGB_D DmgbD;
	T_NLNK_D NlnkD;
	T_NLLP_D NllpD;
	int nDir = 0;
	for (int i = 0; i < lstNlnk.GetCount(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstNlnk[i], NlnkD) || !m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
		{
			ASSERT(0);
			continue;
		}
		if (NllpD.nApplicationType != 2 || NllpD.nSeisType != 6)
			continue;
		if (!m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD)) 
		{
			ASSERT(0);
			continue;
		}
		mapNllp_Num[NlnkD.PropKey]++;
		
		nDir = IDamperGBTool::Instance()->GetNlnkDamperType(lstNlnk[i]);
		if (nDir == 0)
		{
			switch (DmgbD.nPropType)
			{
			case 0: lstH_BRB.Add(lstNlnk[i]); break;
			case 1: lstH_MYD.Add(lstNlnk[i]); break;
			case 2: lstH_FD.Add(lstNlnk[i]); break;
			case 3: lstH_VFD.Add(lstNlnk[i]); break;
			case 4: lstH_HDRD.Add(lstNlnk[i]); break;
			case 5: lstH_VED.Add(lstNlnk[i]); break;
			}
			
		}
		else if (nDir == 1)
		{
			switch (DmgbD.nPropType)
			{
			case 0: lstV_BRB.Add(lstNlnk[i]); break;
			case 1: lstV_MYD.Add(lstNlnk[i]); break;
			case 2: lstV_FD.Add(lstNlnk[i]); break;
			case 3: lstV_VFD.Add(lstNlnk[i]); break;
			case 4: lstV_HDRD.Add(lstNlnk[i]); break;
			case 5: lstV_VED.Add(lstNlnk[i]); break;
			}	
		}
	}
	CString strType[] = { _T("屈曲约束支撑"), _T("金属屈服型消能器"), _T("摩擦消能器"),  _T("粘滞消能器"), _T("高阻尼橡胶消能器"), _T("粘弹性消能器") };
	//
	CString strName;
	T_DMGB_D data;
	int nCount1 = 0, nCount2 = 0;
	m_pDoc->m_pAttrCtrl->GetDmgbKeyList(lstDmgb);
	for (int i=0; i<lstDmgb.GetCount(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetDmgb(lstDmgb[i], data);
		if (!m_pDoc->m_pAttrCtrl->IsUserDMGB(lstDmgb[i]))
			continue;
		if (data.nPropType == 3 || data.nPropType == 5)
			nCount2 += 1;
		else
			nCount1 += 1;
	}
	for (int i = 0; i < 2; i++)
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 6;
		TableD.HeaderD.aTitleName.Add(_T("支座名称"));
		TableD.HeaderD.aTitleName.Add(_T("支座类型"));
		TableD.HeaderD.aTitleName.Add(_T("规格型号"));
		TableD.HeaderD.aTitleName.Add(_T("支座(X向)"));
		TableD.HeaderD.aTitleName.Add(_T("支座(Y向)"));
		TableD.HeaderD.aTitleName.Add(_T("总计"));
		auto& record = TableD.RecordD.aRecordData;	
		POSITION pos = mapNllp_Num.GetStartPosition();
		int nIndex = 0;
		int nNllpNum, nCountH, nCountV;
		CString strInputType;
		if (i == 0 && nCount1 > 0)
		{
			record.SetSize(nCount1);
			for (int k=0; k< lstDmgb.GetCount(); k++)
			{
				nCountH = 0;
				nCountV = 0;
				nNllpNum = 0;		
				m_pDoc->m_pAttrCtrl->GetDmgb(lstDmgb[k], data);
				if (data.nPropType == 3 || data.nPropType == 5)
					continue;
				if (data.nPropType == 0)
				{
					for (int n = 0; n < lstH_BRB.GetCount(); n++)
					{
						m_pDoc->m_pAttrCtrl->GetNlnk(lstH_BRB[n], NlnkD);
						m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
						m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD);
						if (data.PropName == DmgbD.PropName)
						{
							nCountH += 1;
						}
					}
					for (int n = 0; n < lstV_BRB.GetCount(); n++)
					{
						m_pDoc->m_pAttrCtrl->GetNlnk(lstV_BRB[n], NlnkD);
						m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
						m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD);
						if (data.PropName == DmgbD.PropName)
						{
							nCountV += 1;
						}
					}
					nNllpNum = nCountH + nCountV;
					strInputType = data.BRB.strInputType;
				}
				else if (data.nPropType == 1)
				{
					for (int n = 0; n < lstH_MYD.GetCount(); n++)
					{
						m_pDoc->m_pAttrCtrl->GetNlnk(lstH_MYD[n], NlnkD);
						m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
						m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD);
						if (data.PropName == DmgbD.PropName)
						{
							nCountH += 1;
						}
					}
					for (int n = 0; n < lstV_MYD.GetCount(); n++)
					{
						m_pDoc->m_pAttrCtrl->GetNlnk(lstV_MYD[n], NlnkD);
						m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
						m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD);
						if (data.PropName == DmgbD.PropName)
						{
							nCountV += 1;
						}
					}
					nNllpNum = nCountH + nCountV;
					strInputType = data.MYD.strInputType;
				}
				else if (data.nPropType == 2)
				{
					for (int n = 0; n < lstH_FD.GetCount(); n++)
					{
						m_pDoc->m_pAttrCtrl->GetNlnk(lstH_FD[n], NlnkD);
						m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
						m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD);
						if (data.PropName == DmgbD.PropName)
						{
							nCountH += 1;
						}
					}
					for (int n = 0; n < lstV_FD.GetCount(); n++)
					{
						m_pDoc->m_pAttrCtrl->GetNlnk(lstV_FD[n], NlnkD);
						m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
						m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD);
						if (data.PropName == DmgbD.PropName)
						{
							nCountV += 1;
						}
					}
					nNllpNum = nCountH + nCountV;
					strInputType = data.FD.strInputType;
				}
				else if (data.nPropType == 4)
				{
					for (int n = 0; n < lstH_HDRD.GetCount(); n++)
					{
						m_pDoc->m_pAttrCtrl->GetNlnk(lstH_HDRD[n], NlnkD);
						m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
						m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD);
						if (data.PropName == DmgbD.PropName)
						{
							nCountH += 1;
						}
					}
					for (int n = 0; n < lstV_HDRD.GetCount(); n++)
					{
						m_pDoc->m_pAttrCtrl->GetNlnk(lstV_HDRD[n], NlnkD);
						m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
						m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD);
						if (data.PropName == DmgbD.PropName)
						{
							nCountV += 1;
						}
					}
					nNllpNum = nCountH + nCountV;
					strInputType = data.HDRD.strInputType;
				}
				if (nNllpNum > 0)
				{
					record[nIndex].Add(data.PropName);
					record[nIndex].Add(strType[data.nPropType]);
					record[nIndex].Add(strInputType);
					m_strFormat.Format(_T("%d"), nCountH);
					record[nIndex].Add(m_strFormat);
					m_strFormat.Format(_T("%d"), nCountV);
					record[nIndex].Add(m_strFormat);
					m_strFormat.Format(_T("%d"), nNllpNum);
					record[nIndex++].Add(m_strFormat);
				}
				
			}

			TableD.RecordD.aRecordData.FreeExtra();
			TableD.RecordD.nRecordRowCount = nCount1;
			TableD.RecordD.nRecordColCount = 6;
			MakeTableCenterText(genElements, TableD, _T("位移型阻尼器统计"), _T("CHARPT_3_3_1_TBL"));

			nCountH = lstH_BRB.GetCount() + lstH_MYD.GetCount() + lstH_FD.GetCount() + lstH_HDRD.GetCount();
			nCountV = lstV_BRB.GetCount() + lstV_MYD.GetCount() + lstV_FD.GetCount() + lstV_HDRD.GetCount();
			strName.Format(_T("位移型阻尼器统计(X向)：%d个"), nCountH);
			MakeText(genElements, strName);
			strName.Format(_T("位移型阻尼器统计(Y向)：%d个"), nCountV);
			MakeText(genElements, strName);
			strName.Format(_T("位移型阻尼器总计：%d个"), nCountH + nCountV);
			MakeText(genElements, strName);
		}
		else if (i == 1 && nCount2 > 0)
		{
			record.SetSize(nCount2);
			for (int k = 0; k < lstDmgb.GetCount(); k++)
			{
				nCountH = 0;
				nCountV = 0;
				nNllpNum = 0;
				T_DMGB_D data;
				m_pDoc->m_pAttrCtrl->GetDmgb(lstDmgb[k], data);
				
				if (data.nPropType == 3)
				{
					for (int n = 0; n < lstH_VFD.GetCount(); n++)
					{
						m_pDoc->m_pAttrCtrl->GetNlnk(lstH_VFD[n], NlnkD);
						m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
						m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD);
						if (data.PropName == DmgbD.PropName)
						{
							nCountH += 1;
						}
					}
					for (int n = 0; n < lstV_VFD.GetCount(); n++)
					{
						m_pDoc->m_pAttrCtrl->GetNlnk(lstV_VFD[n], NlnkD);
						m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
						m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD);
						if (data.PropName == DmgbD.PropName)
						{
							nCountV += 1;
						}
					}
					nNllpNum = nCountH + nCountV;
					strInputType = data.VFD.strInputType;
				}
				else if (data.nPropType == 5)
				{
					for (int n = 0; n < lstH_VED.GetCount(); n++)
					{
						m_pDoc->m_pAttrCtrl->GetNlnk(lstH_VED[n], NlnkD);
						m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
						m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD);
						if (data.PropName == DmgbD.PropName)
						{
							nCountH += 1;
						}
					}
					for (int n = 0; n < lstV_VED.GetCount(); n++)
					{
						m_pDoc->m_pAttrCtrl->GetNlnk(lstV_VED[n], NlnkD);
						m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
						m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD);
						if (data.PropName == DmgbD.PropName)
						{
							nCountV += 1;
						}
					}
					nNllpNum = nCountH + nCountV;
					strInputType = data.VED.strInputType;
				}
				
				if (nNllpNum > 0)
				{
					record[nIndex].Add(data.PropName);
					record[nIndex].Add(strType[data.nPropType]);
					record[nIndex].Add(strInputType);
					m_strFormat.Format(_T("%d"), nCountH);
					record[nIndex].Add(m_strFormat);
					m_strFormat.Format(_T("%d"), nCountV);
					record[nIndex].Add(m_strFormat);
					m_strFormat.Format(_T("%d"), nNllpNum);
					record[nIndex++].Add(m_strFormat);
				}

			}

			TableD.RecordD.aRecordData.FreeExtra();
			TableD.RecordD.nRecordRowCount = nCount2;
			TableD.RecordD.nRecordColCount = 6;
			MakeTableCenterText(genElements, TableD, _T("速度型阻尼器统计"), _T("CHARPT_3_3_2_TBL"));

			nCountH = lstH_VFD.GetCount() + lstH_VED.GetCount();
			nCountV = lstV_VFD.GetCount() + lstV_VED.GetCount();
			strName.Format(_T("速度型阻尼器统计(X向)：%d个"), nCountH);
			MakeText(genElements, strName);
			strName.Format(_T("速度型阻尼器统计(Y向)：%d个"), nCountV);
			MakeText(genElements, strName);
			strName.Format(_T("速度型阻尼器总计：%d个"), nCountH + nCountV);
			MakeText(genElements, strName);
		}

	}
}

void AutoReport_Gen_CH_DMGB::WriteCharpt4()
{
	GenSegmentElements Elems;
	MakeTitle(Elems, _T("荷载的输入"), 4, 0);
	WriteCharpt4_1(Elems);
	WriteCharpt4_2(Elems);
	m_Segments.push_back(Elems);
}
void AutoReport_Gen_CH_DMGB::WriteCharpt4_1(GenSegmentElements &genElements)
{
	MakeTitle(genElements, _T("反应谱荷载的输入"), 4, 1);
	CString str1;
	str1.Format(_T("    程序中定义的反应谱荷载工况如下："));
	MakeText(genElements, str1);

	//Table
	CString strSeisType[] = { _T("多遇地震"), _T("设防地震"), _T("罕遇地震") };
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
		TableD.HeaderD.aTitleName.Add(_LS(IDS_TM_DMGB_AUTO_WORD_ALPHA_MAX));//αmax
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
				if (nEarthResponse == 2)
				{
					rKeyList.RemoveAt(i);
					continue;
				}
				//
				record[i].Add(SplcD.LoadCaseName);
				record[i].Add(m_strFormat);
				record[i].Add(strSeisType[nEarthResponse+1]);
				record[i].Add(Double2String_f(dMaxEQE));
				record[i].Add(Double2String_f(dTg));
			}
			else if (SpfcD.nSpecCodeType == D_SPFC_CODE_CH2002)
			{
				dMaxEQE = SpfcD.CodeParam.CH2002.dMaxEQE;
				dTg = SpfcD.CodeParam.CH2002.dTg;
				nEarthResponse = SpfcD.CodeParam.CH2002.nEarthResponse;
				//
				record[i].Add(SplcD.LoadCaseName);
				record[i].Add(m_strFormat);
				record[i].Add(strSeisType[nEarthResponse+1]);
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
void AutoReport_Gen_CH_DMGB::WriteCharpt4_2(GenSegmentElements &genElements)
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
void AutoReport_Gen_CH_DMGB::WriteCharpt5()
{
	GenSegmentElements genElements;
	MakeTitle(genElements, _T("分析结果"), 5, 0);
	WriteCharpt5_1(genElements);
	WriteCharpt5_2(genElements);
	WriteCharpt5_3(genElements);
	m_Segments.push_back(genElements);
}
void AutoReport_Gen_CH_DMGB::WriteCharpt5_1(GenSegmentElements &genElements)
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
			Para()->GroupKeys(aGroupKey, ARGC_DMGB * 1000 + 1);//1:Mode Group
			WriteModelRlt(genElements, gm, aModelKeys, aGroupKey, nIndex);
		}
	}
}
void AutoReport_Gen_CH_DMGB::WriteCharpt5_2(GenSegmentElements &genElements)
{
	Report_Key_LIST aKeys;
	Para()->ListReportOption(aKeys, ARGC_DMGB);
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
void AutoReport_Gen_CH_DMGB::WriteCharpt5_3(GenSegmentElements &genElements)
{
	Report_Key_LIST aKeys;
	Para()->ListReportOption(aKeys, ARGC_DMGB);
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

void AutoReport_Gen_CH_DMGB::WriteCharpt6()
{
	GenSegmentElements genElements;
	MakeTitle(genElements, _T("阻尼器验算结果"), 6, 0);
	if (!m_pDoc->IsPostMode())
		return;
	WriteCharpt6_1(genElements);
	WriteCharpt6_2(genElements);
	if (!m_pDoc->m_pAttrCtrl->GetDamping(m_DagbD))
		return;
	WriteCharpt6_3(genElements);
	WriteCharpt6_4(genElements);
	m_Segments.push_back(genElements);
}
void AutoReport_Gen_CH_DMGB::WriteCharpt6_1(GenSegmentElements &genElements)
{
	CString strText, strBook, strTemp, strTitle;
	strText = _T("附加阻尼比验算（规范法）");
	MakeTitle(genElements, strText, 6, 1);

	strText = _T("1.结构总应变能Ws的计算");
	MakeText(genElements, strText);
	strText = _T("    根据《建筑消能减震技术规程》JGJ 297-2013第6.3.2条：");
	MakeText(genElements, strText);
	strTemp = _LS(IDS_TM_DAGB_AUTO_WORD_Charpt6_1_1);
	strText.Format(_T("[DRG_FONT]|FC| size=14 align=center [/DRG_FONT]%s\n[DRG_FONT]|FI|[/DRG_FONT]"), strTemp);
	MakeText(genElements, strText);
	strText = _T("式中：");
	MakeText(genElements, strText);
	strText = _T("    Fi — 质点i的水平地震作用标准值；");
	MakeText(genElements, strText);
	strText = _T("    ui — 质点i对应于水平地震作用标准值的位移。");
	MakeText(genElements, strText);

	strText = _T("2.阻尼器耗能Wc");
	MakeText(genElements, strText);
	strText = _T("    根据《建筑消能减震技术规程》JGJ 297-2013第3.3.5条条文说明，对于摩擦消能器采用理想弹塑性模型:");
	MakeText(genElements, strText);
	strTemp = _LS(IDS_TM_DAGB_AUTO_WORD_Charpt6_1_2);
	strText.Format(_T("[DRG_FONT]|FC| size=14 align=center [/DRG_FONT]%s\n[DRG_FONT]|FI|[/DRG_FONT]"), strTemp);
	MakeText(genElements, strText);
	strText = _T("式中：");
	MakeText(genElements, strText);
	strText = _T("    Fdy — 摩擦消能器的起滑力；");
	MakeText(genElements, strText);
	strText = _T("    udy — 摩擦消能器的起滑位移；");
	MakeText(genElements, strText);
	strText = _T("    udmax — 摩擦消能器的设计位移。");
	MakeText(genElements, strText);

	strText = _T("    对于金属消能器和屈曲约束支撑可采用双线性模型:");
	MakeText(genElements, strText);
	strTemp = _LS(IDS_TM_DAGB_AUTO_WORD_Charpt6_1_3);
	strText.Format(_T("[DRG_FONT]|FC| size=14 align=center [/DRG_FONT]%s\n[DRG_FONT]|FI|[/DRG_FONT]"), strTemp);
	MakeText(genElements, strText);
	strText = _T("式中：");
	MakeText(genElements, strText);
	strText = _T("    Fdy — 消能器的设计屈服承载力；");
	MakeText(genElements, strText);
	strText = _T("    Δudy —— 消能器的设计屈服位移；");
	MakeText(genElements, strText);
	strText = _T("    Δudmax — 消能器的设计位移；");
	MakeText(genElements, strText);
	strText = _T("    Fdmax — 消能器的最大承载力。");
	MakeText(genElements, strText);

	strText = _T("    根据《建筑消能减震技术规程》JGJ 297-2013第6.3.2-5条，对于非线性粘滞消能器:");
	MakeText(genElements, strText);
	strTemp = _LS(IDS_TM_DAGB_AUTO_WORD_Charpt6_1_4);
	strText.Format(_T("[DRG_FONT]|FC| size=14 align=center [/DRG_FONT]%s\n[DRG_FONT]|FI|[/DRG_FONT]"), strTemp);
	MakeText(genElements, strText);
	strText = _T("式中：");
	MakeText(genElements, strText);
	strText = _T("    λ1 — 阻尼指数的函数，可按表取值；");
	MakeText(genElements, strText);
	strText = _T("    Fdjmax — 第j个消能器在水平地震作用下的最大阻尼力。");
	MakeText(genElements, strText);

	strText = _T("3.附加阻尼比的计算");
	MakeText(genElements, strText);
	strText = _T("    根据《建筑消能减震技术规程》JGJ 297-2013第6.3.2-2条，消能部件附加给结构的有效阻尼比可按下式计算：");
	MakeText(genElements, strText);
	strTemp = _LS(IDS_TM_DAGB_AUTO_WORD_Charpt6_1_5);
	strText.Format(_T("[DRG_FONT]|FC| size=14 align=center [/DRG_FONT]%s\n[DRG_FONT]|FI|[/DRG_FONT]"), strTemp);
	MakeText(genElements, strText);
	strText = _T("式中：");
	MakeText(genElements, strText);
	strText = _T("    ζd — 消能减震结构的附加有效阻尼比");
	MakeText(genElements, strText);
	strText = _T("    Wcj — 第j个消能部件在结构预期层间位移下往复循环一周所消耗的能量；");
	MakeText(genElements, strText);
	strText = _T("    Ws — 消能减震结构在水平地震作用下的总应变能。");
	MakeText(genElements, strText);
	strText = _T("    N — 消能部件的总个数。");
	MakeText(genElements, strText);


	WriteCharpt6_1_1(genElements);
	WriteCharpt6_1_2(genElements);
}
void AutoReport_Gen_CH_DMGB::WriteCharpt6_1_1(GenSegmentElements& genElements)
{
	CString strText, strBook, strTemp, strTitle;
	strText = _T("小震下附加有效阻尼比的计算");
	MakeTitle(genElements, strText, 6, 1, _T(""), 1);

	T_DAGB_D DagbD; DagbD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetDamping(DagbD))
		return;

	if (DagbD.aNonUseThisCase.GetCount() == 0)
		return;
	T_THIS_D ThisD;
	T_THGA_D ThgaD;
	T_THNL_K ThnlK;
	T_THNL_D ThnlD;
	CArray<T_THNL_K, T_THNL_K> rThnlList;
	m_pDoc->m_pAttrCtrl->GetThnlKeyList(rThnlList);

	T_KEY_LIST lstNonUseThisX, lstNonUseThisY;
	for (int k = 0; k < DagbD.aNonUseThisCase.GetCount(); k++)
	{
		m_pDoc->m_pAttrCtrl->GetThis(DagbD.aNonUseThisCase[k], ThisD);
		if (m_pDoc->m_pAttrCtrl->GetThga(DagbD.aNonUseThisCase[k], ThgaD))
		{
			if (ThgaD.nThisFuncKey[0] != 0 && ThgaD.nThisFuncKey[1] == 0 && ThgaD.nThisFuncKey[2] == 0)
			{
				lstNonUseThisX.Add(DagbD.aNonUseThisCase[k]);
			}
			else if (ThgaD.nThisFuncKey[0] == 0 && ThgaD.nThisFuncKey[1] != 0 && ThgaD.nThisFuncKey[2] == 0)
			{
				lstNonUseThisY.Add(DagbD.aNonUseThisCase[k]);
			}
		}
		else
		{
			for (int i = 0; i < rThnlList.GetCount(); i++)
			{
				ThnlK = rThnlList[i];
				if (!m_pDoc->m_pAttrCtrl->GetThnl(ThnlK, ThnlD)) { ASSERT(0); }
				if (ThnlD.nThisFuncKey == DagbD.aNonUseThisCase[k])
				{
					if (ThnlD.nDirection == 1)
					{
						lstNonUseThisX.Add(DagbD.aNonUseThisCase[k]);
					}
					else if (ThnlD.nDirection == 2)
					{
						lstNonUseThisY.Add(DagbD.aNonUseThisCase[k]);
					}
				}
			}
		}
	}

	//X向
	if (lstNonUseThisX.GetCount() > 0)
	{
		WriteAdditionalDamperRto_6_1(genElements, lstNonUseThisX, TRUE, TRUE);
	}
	
	//Y向
	if (lstNonUseThisY.GetCount() > 0)
	{
		WriteAdditionalDamperRto_6_1(genElements, lstNonUseThisY, FALSE, TRUE);
	}

}
void AutoReport_Gen_CH_DMGB::WriteCharpt6_1_2(GenSegmentElements& genElements)
{
	CString strText, strBook, strTemp, strTitle;
	strText = _T("中震下附加有效阻尼比的计算");
	MakeTitle(genElements, strText, 6, 1, _T(""), 2);

	T_DAGB_D DagbD; DagbD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetDamping(DagbD))
		return;

	if (DagbD.aResistThisCase.GetCount() == 0)
		return;
	T_THIS_D ThisD;
	T_THGA_D ThgaD;
	T_THNL_K ThnlK;
	T_THNL_D ThnlD;
	CArray<T_THNL_K, T_THNL_K> rThnlList;
	m_pDoc->m_pAttrCtrl->GetThnlKeyList(rThnlList);

	T_KEY_LIST lstResistThisX, lstResistThisY;
	for (int k = 0; k < DagbD.aResistThisCase.GetCount(); k++)
	{
		if (m_pDoc->m_pAttrCtrl->GetThga(DagbD.aResistThisCase[k], ThgaD))
		{
			if (ThgaD.nThisFuncKey[0] != 0 && ThgaD.nThisFuncKey[1] == 0 && ThgaD.nThisFuncKey[2] == 0)
			{
				lstResistThisX.Add(DagbD.aResistThisCase[k]);
			}
			else if (ThgaD.nThisFuncKey[0] == 0 && ThgaD.nThisFuncKey[1] != 0 && ThgaD.nThisFuncKey[2] == 0)
			{
				lstResistThisY.Add(DagbD.aResistThisCase[k]);
			}
		}
		else
		{
			for (int i = 0; i < rThnlList.GetCount(); i++)
			{
				ThnlK = rThnlList[i];
				if (!m_pDoc->m_pAttrCtrl->GetThnl(ThnlK, ThnlD)) { ASSERT(0); }
				if (ThnlD.nThisFuncKey == DagbD.aResistThisCase[k])
				{
					if (ThnlD.nDirection == 1)
					{
						lstResistThisX.Add(DagbD.aResistThisCase[k]);
					}
					else if (ThnlD.nDirection == 2)
					{
						lstResistThisY.Add(DagbD.aResistThisCase[k]);
					}
				}
			}
		}
	}


	//X向
	if (lstResistThisX.GetCount() > 0)
	{
		WriteAdditionalDamperRto_6_1(genElements, lstResistThisX, TRUE, FALSE);
	}

	//Y向
	if (lstResistThisY.GetCount() > 0)
	{
		WriteAdditionalDamperRto_6_1(genElements, lstResistThisY, FALSE, FALSE);
	}

}

void AutoReport_Gen_CH_DMGB::WriteAdditionalDamperRto_6_1(GenSegmentElements &genElements, T_KEY_LIST &lstThisCase, BOOL bXDir, BOOL bChapOne)
{
	CString strTitle, strBook, strText;
	T_TABLE_D TableD;
	TableD.Initialize();
	TableD.HeaderD.nHeaderRowCount = 1;
	TableD.HeaderD.nHeaderColCount = 4;
	TableD.HeaderD.aTitleName.Add(_T("工况"));
	TableD.HeaderD.aTitleName.Add(_T("阻尼器耗能Wc"));
	TableD.HeaderD.aTitleName.Add(_T("结构总应变能Ws"));
	TableD.HeaderD.aTitleName.Add(_T("阻尼器附加阻尼比ζ(%)"));
	int k = 0;
	double dSumWcS = 0., dSumWcD = 0., dSumWs = 0.;
	double dKesi = 0., dSumKesi = 0.;
	double dFi = 0., dUi = 0.;
	TableD.RecordD.aRecordData.SetSize(lstThisCase.GetCount());
	auto& record = TableD.RecordD.aRecordData;

	CAutoRecoveryUnit unitSet(D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_FORCE_INDEX_KN, m_pDoc->m_pUnitCtrl, TRUE);

	T_THIS_D ThisD;
	for (k = 0; k < lstThisCase.GetCount(); k++)
	{
		ThisD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetThis(lstThisCase[k], ThisD)) { ASSERT(0); return; }
		dSumWcD = IDamperGBTool::Instance()->GetWcSum_Disp(lstThisCase[k], 0) + IDamperGBTool::Instance()->GetWcSum_Disp(lstThisCase[k], 1);
		dSumWcS = IDamperGBTool::Instance()->GetWcSum_Speed(lstThisCase[k], 0) + IDamperGBTool::Instance()->GetWcSum_Speed(lstThisCase[k], 1);
		dSumWs = IDamperGBTool::Instance()->GetWs(lstThisCase[k], 0, dFi, dUi) + IDamperGBTool::Instance()->GetWs(lstThisCase[k], 1, dFi, dUi);
		record[k].Add(ThisD.LoadCaseName);
		record[k].Add(Double2String_f(dSumWcD + dSumWcS));
		record[k].Add(Double2String_f(dSumWs));
		if (fabs(dSumWs) > 0)
			dKesi = (dSumWcD + dSumWcS) / (4 * PI * dSumWs) * 100;
		else
			dKesi = 0.0;
		record[k].Add(Double2String_f(dKesi));
		dSumKesi += dKesi;
	}
	TableD.RecordD.aRecordData.FreeExtra();
	TableD.RecordD.nRecordRowCount = lstThisCase.GetCount();
	TableD.RecordD.nRecordColCount = 4;
	
	if (bXDir)
	{
		strTitle = bChapOne ? _T("小震下X向结构附加阻尼比计算(kN,m)") : _T("中震下X向结构附加阻尼比计算(kN,m)");
		strBook = bChapOne ? _T("CHARPT_6_1_1_1_TBL") : _T("CHARPT_6_1_2_1_TBL");
	}
	else
	{
		strTitle = bChapOne ? _T("小震下Y向结构附加阻尼比计算(kN,m)") : _T("中震下Y向结构附加阻尼比计算(kN,m)");
		strBook = bChapOne ? _T("CHARPT_6_1_1_2_TBL") : _T("CHARPT_6_1_2_2_TBL");
	}
	MakeTableCenterText(genElements, TableD, strTitle, strBook);
	double dTemp = dSumKesi / lstThisCase.GetCount();
	strText.Format(_T("附加阻尼比平均值为：%.2f %%"), dTemp);
	MakeText(genElements, strText);
}

void AutoReport_Gen_CH_DMGB::WriteCharpt6_2(GenSegmentElements &genElements)
{
	CString strText, strTemp, strTitle, strBook;
	strText = _T("附加阻尼比验算（能量法）");
	MakeTitle(genElements, strText, 6, 2);
	strText = _T("    根据《建筑消能减震及隔震技术标准》（DG/TJ08-2326-2020）第6.3.2条，消能部件附加给结构的有效阻尼比如下：");
	MakeText(genElements, strText);
	strTemp = _LS(IDS_TM_DAGB_AUTO_WORD_Charpt6_2_1);
	strText.Format(_T("[DRG_FONT]|FC| size=14 align=center [/DRG_FONT]%s\n[DRG_FONT]|FI|[/DRG_FONT]"), strTemp);
	MakeText(genElements, strText);
	strText = _T("式中:");
	MakeText(genElements, strText);
	strText = _T("    ξ0 -- 消能减震主体结构的固有模态阻尼比;");
	MakeText(genElements, strText);
	strText = _T("    Ed(t) -- 消能减震结构消能器累积耗能时程;");
	MakeText(genElements, strText);
	strText = _T("    Ec(t) -- 消能减震主体结构固有模态阻尼累积耗能时程;");
	MakeText(genElements, strText);
	strText = _T("    η2 --有效阻尼比折减系数,一般取0.9。");
	MakeText(genElements, strText);

	CAutoRecoveryUnit unitSet(D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_FORCE_INDEX_KN, m_pDoc->m_pUnitCtrl, TRUE);

	T_DAGB_D DagbD; DagbD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetDamping(DagbD))
		return;
	
	if (DagbD.aNonUseThisCase.GetCount() + DagbD.aResistThisCase.GetCount() + DagbD.aRareEThisCase.GetCount() == 0)
		return;
	T_THIS_K ThisK;
	T_THIS_D ThisD;

	T_ENRG_RT_D EnrgRtD;
	CArray<double, double> aXValue;

	//小震
	T_TABLE_D TableD;
	TableD.Initialize();
	TableD.HeaderD.nHeaderRowCount = 1;
	TableD.HeaderD.nHeaderColCount = 4;
	TableD.HeaderD.aTitleName.Add(_T("荷载工况"));
	TableD.HeaderD.aTitleName.Add(_T("Ed(t)"));
	TableD.HeaderD.aTitleName.Add(_T("Ec(t)"));
	TableD.HeaderD.aTitleName.Add(_T("阻尼器附加阻尼比ζ(%)"));

	TableD.RecordD.aRecordData.SetSize(DagbD.aNonUseThisCase.GetCount());
	auto& record = TableD.RecordD.aRecordData;
	double dEd = 0., dKesi = 0.;

	for (int i=0; i< DagbD.aNonUseThisCase.GetCount(); i++)
	{
		ThisD.Initialize();
		ThisK = DagbD.aNonUseThisCase[i];
		if (!m_pDoc->m_pAttrCtrl->GetThis(ThisK, ThisD)) { ASSERT(0); return; }
		m_pDoc->m_pPostCtrl->SelectThis(ThisK);
		aXValue.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetTimeStep(ThisK, aXValue);
		int nCount = aXValue.GetCount() - 1;
		m_pDoc->m_pPostCtrl->GetEnrgHistory(nCount, EnrgRtD);

		dEd = EnrgRtD.dEm + EnrgRtD.dEv + EnrgRtD.dEt;
		record[i].Add(ThisD.LoadCaseName);
		record[i].Add(Double2String_f(dEd));
		record[i].Add(Double2String_f(EnrgRtD.dEd));
		if (fabs(EnrgRtD .dEd) < 1.0e-7)
			record[i].Add(_T("-"));
		else
		{
			dKesi = m_dEta * m_dKesi * dEd / EnrgRtD.dEd * 100;
			record[i].Add(Double2String_f(dKesi));
		}
			
	}
	TableD.RecordD.aRecordData.FreeExtra();
	TableD.RecordD.nRecordRowCount = DagbD.aNonUseThisCase.GetCount();
	TableD.RecordD.nRecordColCount = 4;
	strTitle = _T("表6.2-1 附加阻尼比计算（小震）(kN,m)");
	strBook = _T("CHARPT_6_2_1_TBL");
	MakeTableCenterText(genElements, TableD, strTitle, strBook);
	
	//中震
	TableD.Initialize();
	TableD.HeaderD.nHeaderRowCount = 1;
	TableD.HeaderD.nHeaderColCount = 4;
	TableD.HeaderD.aTitleName.Add(_T("荷载工况"));
	TableD.HeaderD.aTitleName.Add(_T("Ed(t)"));
	TableD.HeaderD.aTitleName.Add(_T("Ec(t)"));
	TableD.HeaderD.aTitleName.Add(_T("阻尼器附加阻尼比ζ(%)"));

	TableD.RecordD.aRecordData.SetSize(DagbD.aResistThisCase.GetCount());
	for (int i = 0; i < DagbD.aResistThisCase.GetCount(); i++)
	{
		ThisD.Initialize();
		ThisK = DagbD.aResistThisCase[i];
		if (!m_pDoc->m_pAttrCtrl->GetThis(ThisK, ThisD)) { ASSERT(0); return; }
		aXValue.RemoveAll();
		m_pDoc->m_pPostCtrl->SelectThis(ThisK);
		m_pDoc->m_pAttrCtrl->GetTimeStep(ThisK, aXValue);
		int nCount = aXValue.GetCount() - 1;
		m_pDoc->m_pPostCtrl->GetEnrgHistory(nCount, EnrgRtD);
		dEd = EnrgRtD.dEm + EnrgRtD.dEv + EnrgRtD.dEt;
		record[i].Add(ThisD.LoadCaseName);
		record[i].Add(Double2String_f(dEd));
		record[i].Add(Double2String_f(EnrgRtD.dEd));
		if (fabs(EnrgRtD.dEd) < 1.0e-7)
			record[i].Add(_T("-"));
		else
		{
			dKesi = m_dEta * m_dKesi * dEd / EnrgRtD.dEd * 100;
			record[i].Add(Double2String_f(dKesi));
		}
	}
	TableD.RecordD.aRecordData.FreeExtra();
	TableD.RecordD.nRecordRowCount = DagbD.aResistThisCase.GetCount();
	TableD.RecordD.nRecordColCount = 4;
	strTitle = _T("表6.2-2 附加阻尼比计算（中震）(kN,m)");
	strBook = _T("CHARPT_6_2_2_TBL");
	MakeTableCenterText(genElements, TableD, strTitle, strBook);


	//大震
	TableD.Initialize();
	TableD.HeaderD.nHeaderRowCount = 1;
	TableD.HeaderD.nHeaderColCount = 4;
	TableD.HeaderD.aTitleName.Add(_T("荷载工况"));
	TableD.HeaderD.aTitleName.Add(_T("Ed(t)"));
	TableD.HeaderD.aTitleName.Add(_T("Ec(t)"));
	TableD.HeaderD.aTitleName.Add(_T("阻尼器附加阻尼比ζ(%)"));

	TableD.RecordD.aRecordData.SetSize(DagbD.aRareEThisCase.GetCount());
	for (int i = 0; i < DagbD.aRareEThisCase.GetCount(); i++)
	{
		ThisD.Initialize();
		ThisK = DagbD.aRareEThisCase[i];
		if (!m_pDoc->m_pAttrCtrl->GetThis(ThisK, ThisD)) { ASSERT(0); return; }
		aXValue.RemoveAll();
		m_pDoc->m_pPostCtrl->SelectThis(ThisK);
		if (!m_pDoc->m_pAttrCtrl->GetTimeStep(ThisK, aXValue)) return;
		if (!m_pDoc->m_pPostCtrl->GetEnrgHistory(aXValue.GetCount() - 1, EnrgRtD)) return;
		dEd = EnrgRtD.dEm + EnrgRtD.dEv + EnrgRtD.dEt;
		record[i].Add(ThisD.LoadCaseName);
		record[i].Add(Double2String_f(dEd));
		record[i].Add(Double2String_f(EnrgRtD.dEd));
		if (fabs(EnrgRtD.dEd) < 1.0e-7)
			record[i].Add(_T("-"));
		else
		{
			dKesi = m_dEta * m_dKesi * dEd / EnrgRtD.dEd * 100;
			record[i].Add(Double2String_f(dKesi));
		}
	}
	TableD.RecordD.aRecordData.FreeExtra();
	TableD.RecordD.nRecordRowCount = DagbD.aRareEThisCase.GetCount();
	TableD.RecordD.nRecordColCount = 4;
	strTitle = _T("表6.2-3 附加阻尼比计算（大震）(kN,m)");
	strBook = _T("CHARPT_6_2_3_TBL");
	MakeTableCenterText(genElements, TableD, strTitle, strBook);

}

void AutoReport_Gen_CH_DMGB::WriteAdditionalDamperRto()
{
	GenSegmentElements genElements;
	MakeTitle(genElements, _T("附件：阻尼器附加阻尼比计算"), 8, 0);
	if (!m_pDoc->IsPostMode())
		return;
	
	CString strText, strBook, strTemp, strTitle;

	int nIndex = 1;
	T_KEY_LIST lstNlnk;
	T_KEY_LIST lstH_BRB, lstV_BRB, lstH_MYD, lstV_MYD, lstH_FD, lstV_FD, lstH_HDRD, lstV_HDRD, lstH_VFD, lstV_VFD, lstH_VED, lstV_VED;
	T_KEY_LIST lstDamperX, lstDamperY;
	CMap<UINT, UINT, int, int> mapNllp_Num;
	m_pDoc->m_pAttrCtrl->GetNlnkKeyList(lstNlnk);
	T_DMGB_D DmgbD;
	T_NLNK_D NlnkD;
	T_NLLP_D NllpD;

	int nDir = -1;
	for (int i = 0; i < lstNlnk.GetCount(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstNlnk[i], NlnkD) || !m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
		{
			ASSERT(0);
			continue;
		}
		if (NllpD.nApplicationType != 2 || NllpD.nSeisType != 6)
			continue;
		if (!m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD))
		{
			ASSERT(0);
			continue;
		}
		nDir = IDamperGBTool::Instance()->GetNlnkDamperType(lstNlnk[i]);
		if (nDir == 0)
		{
			lstDamperX.Add(lstNlnk[i]);
			switch (DmgbD.nPropType)
			{
			case 0: lstH_BRB.Add(lstNlnk[i]); break;
			case 1: lstH_MYD.Add(lstNlnk[i]); break;
			case 2: lstH_FD.Add(lstNlnk[i]); break;
			case 3: lstH_VFD.Add(lstNlnk[i]); break;
			case 4: lstH_HDRD.Add(lstNlnk[i]); break;
			case 5: lstH_VED.Add(lstNlnk[i]); break;
			}
		}
		else if (nDir == 1)
		{
			lstDamperY.Add(lstNlnk[i]);
			switch (DmgbD.nPropType)
			{
			case 0: lstV_BRB.Add(lstNlnk[i]); break;
			case 1: lstV_MYD.Add(lstNlnk[i]); break;
			case 2: lstV_FD.Add(lstNlnk[i]); break;
			case 3: lstV_VFD.Add(lstNlnk[i]); break;
			case 4: lstV_HDRD.Add(lstNlnk[i]); break;
			case 5: lstV_VED.Add(lstNlnk[i]); break;
			}
		}
	}

	int nHorDisp = lstH_BRB.GetCount() + lstH_MYD.GetCount() + lstH_FD.GetCount() + lstH_HDRD.GetCount();
	int nVerDisp = lstV_BRB.GetCount() + lstV_MYD.GetCount() + lstV_FD.GetCount() + lstV_HDRD.GetCount();
	int nHorSpeed = lstH_VFD.GetCount() + lstH_VED.GetCount();
	int nVerSpeed = lstV_VFD.GetCount() + lstV_VED.GetCount();

	CArray<T_THIS_K, T_THIS_K> arThis;
	T_DAGB_D DagbD; DagbD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetDamping(DagbD))
		return;
	for (int i=0; i<DagbD.aNonUseThisCase.GetCount(); i++)
	{
		arThis.Add(DagbD.aNonUseThisCase[i]);
	}
	for (int i = 0; i < DagbD.aResistThisCase.GetCount(); i++)
	{
		arThis.Add(DagbD.aResistThisCase[i]);
	}
	int nCount = arThis.GetCount();
	if (nCount == 0)
		return;

	T_THIS_K ThisK;
	T_THIS_D ThisD;
	CArray<T_STOR_K, T_STOR_K> arStor;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(arStor);
	T_STOR_D StorD;
	T_THSS_D ThssD;
	T_STOR_DSP DispD;
	CStoryDisplDrift m_StDisp;
	CArray<T_STOR_DSP, T_STOR_DSP&>* pStorDisp;
	
	pStorDisp = new CArray<T_STOR_DSP, T_STOR_DSP&>[nCount];
	T_LCOM_D LcomData;

	double dFi = 0., dUi = 0., dWsX = 0., dWsY = 0., dFdmax = 0., dUmax = 0., dWc = 0., dWs = 0.;
	int k;
	CAutoRecoveryUnit unitSet(D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_FORCE_INDEX_KN, m_pDoc->m_pUnitCtrl, TRUE);
	T_TABLE_D TableD;
	auto& record = TableD.RecordD.aRecordData;
	for (k = 0; k < arThis.GetCount(); k++)
	{
		ThisK = arThis[k];
		ThisD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetThis(arThis[k], ThisD)) { ASSERT(0); return; }

		strText.Format(_T("时程工况%d:%s"), k + 1, ThisD.LoadCaseName);
		MakeText(genElements, strText);

#pragma region 结构总应变能Ws
		
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount = 1;
		TableD.HeaderD.nHeaderColCount = 8;
		TableD.HeaderD.aTitleName.Add(_T("层号"));
		TableD.HeaderD.aTitleName.Add(_T("塔号"));
		TableD.HeaderD.aTitleName.Add(_T("X向楼层剪力"));
		TableD.HeaderD.aTitleName.Add(_T("X向层位移"));
		TableD.HeaderD.aTitleName.Add(_T("X向应变能"));
		TableD.HeaderD.aTitleName.Add(_T("Y向楼层剪力"));
		TableD.HeaderD.aTitleName.Add(_T("Y向层位移"));
		TableD.HeaderD.aTitleName.Add(_T("Y向应变能"));
		TableD.RecordD.aRecordData.SetSize(arStor.GetCount() - 1);
		auto& record = TableD.RecordD.aRecordData;

		if (!m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_HISTORY, ThisK, LcomData)) return;
		if (!m_StDisp.GetDSP(LcomData, pStorDisp[k], 0)) return;

		for (int m = 0; m < arStor.GetCount() - 1; m++)
		{
			StorD.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetStor(arStor[m], StorD)) { ASSERT(0); continue; }
			if (m_pDoc->m_pAttrCtrl->GetLastNumStor() == arStor[m]) { continue; }
			m_pDoc->m_pPostCtrl->GetThisStoryShear(arStor[m], ThisK, ThssD);
			dWsX = IDamperGBTool::Instance()->GetWs(ThisK, 0, dFi, dUi);

			record[m].Add(StorD.StoryName);
			record[m].Add(m_pDoc->m_pAttrCtrl->GetModuleNameOfStor(arStor[m]));
			dFi = fabs(ThssD.dMax_X) > fabs(ThssD.dMin_X) ? ThssD.dMax_X : ThssD.dMin_X;
			record[m].Add(Double2String_f(dFi, 4));
			DispD = pStorDisp[k].GetAt(m + 1);
			dUi = DispD.dAvgDsp[0];
			record[m].Add(Double2String_f(dUi, 4));
			record[m].Add(Double2String_f(dWsX, 2));

			dWsY = IDamperGBTool::Instance()->GetWs(ThisK, 1, dFi, dUi);
			dFi = fabs(ThssD.dMax_Y) > fabs(ThssD.dMin_Y) ? ThssD.dMax_Y : ThssD.dMin_Y;
			record[m].Add(Double2String_f(dFi, 4));
			dUi = DispD.dAvgDsp[1];
			record[m].Add(Double2String_f(dUi, 4));
			record[m].Add(Double2String_f(dWsY, 2));
		}
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = arStor.GetCount() - 1;
		TableD.RecordD.nRecordColCount = 8;
		strTitle.Format(_T("表8.1-%d 结构总应变能表(kN,mm)"), nIndex);
		strBook.Format(_T("CHARPT_8_1_%d_TBL"), nIndex);
		MakeTableCenterText(genElements, TableD, strTitle, strBook);
		nIndex++;

		strText.Format(_T("X向总应变能：%.5fkN.mm"), dWsX);
		MakeText(genElements, strText);
		strText.Format(_T("Y向总应变能：%.5fkN.mm"), dWsY);
		MakeText(genElements, strText);
#pragma endregion

#pragma region 位移型阻尼器耗能Wc
		
		double dSumWcX = 0., dSumWcY = 0.;
		int i;
		//table X
		int nNum = 0;
		if ((lstH_BRB.GetCount() + lstH_MYD.GetCount() + lstH_FD.GetCount() + lstH_HDRD.GetCount()) > 0)
		{
			TableD.Initialize();
			TableD.HeaderD.nHeaderRowCount = 1;
			TableD.HeaderD.nHeaderColCount = 7;
			TableD.HeaderD.aTitleName.Add(_T("序号"));
			TableD.HeaderD.aTitleName.Add(_T("阻尼器编号"));
			TableD.HeaderD.aTitleName.Add(_T("屈服力Fdy"));
			TableD.HeaderD.aTitleName.Add(_T("屈服位移Udy"));
			TableD.HeaderD.aTitleName.Add(_T("最大阻尼力Fdmax"));
			TableD.HeaderD.aTitleName.Add(_T("最大相对变形Umax"));
			TableD.HeaderD.aTitleName.Add(_T("阻尼器耗能Wc"));
			TableD.RecordD.aRecordData.SetSize(lstH_BRB.GetCount() + lstH_MYD.GetCount() + lstH_FD.GetCount() + lstH_HDRD.GetCount());


			for (int m = 0; m < lstNlnk.GetCount(); m++)
			{
				for (i = 0; i < lstH_BRB.GetCount(); i++)
				{
					if (lstH_BRB[i] == lstNlnk[m])
					{
						if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstH_BRB[i], NlnkD) || !m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
						{
							ASSERT(0);
							continue;
						}
						if (!m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD))
						{
							ASSERT(0);
							continue;
						}
						dWc = IDamperGBTool::Instance()->GetWc(arThis[k], lstH_BRB[i], 0, dFdmax, dUmax);
						dSumWcX += dWc;
						strText.Format(_T("%d"), nNum + 1);
						record[nNum].Add(strText);
						strText.Format(_T("%d"), lstH_BRB[i]);
						record[nNum].Add(strText);
						strText.Format(_T("%.2f"), DmgbD.BRB.dFy);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), DmgbD.BRB.dDisp);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dFdmax);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dUmax);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dWc);
						record[nNum].Add(strText);
						nNum++;
						break;
					}
				}

				for (i = 0; i < lstH_MYD.GetCount(); i++)
				{
					if (lstH_MYD[i] == lstNlnk[m])
					{
						if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstH_MYD[i], NlnkD) || !m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
						{
							ASSERT(0);
							continue;
						}
						if (!m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD))
						{
							ASSERT(0);
							continue;
						}
						dWc = IDamperGBTool::Instance()->GetWc(arThis[k], lstH_MYD[i], 0, dFdmax, dUmax);
						dSumWcX += dWc;
						strText.Format(_T("%d"), nNum + 1);
						record[nNum].Add(strText);
						strText.Format(_T("%d"), lstH_MYD[i]);
						record[nNum].Add(strText);
						strText.Format(_T("%.2f"), DmgbD.MYD.dFy);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), DmgbD.MYD.dDisp);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dFdmax);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dUmax);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dWc);
						record[nNum].Add(strText);

						nNum++;
						break;
					}
				}

				for (i = 0; i < lstH_FD.GetCount(); i++)
				{
					if (lstH_FD[i] == lstNlnk[m])
					{
						if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstH_FD[i], NlnkD) || !m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
						{
							ASSERT(0);
							continue;
						}
						if (!m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD))
						{
							ASSERT(0);
							continue;
						}
						dWc = IDamperGBTool::Instance()->GetWc(arThis[k], lstH_FD[i], 0, dFdmax, dUmax);
						dSumWcX += dWc;
						strText.Format(_T("%d"), nNum + 1);
						record[nNum].Add(strText);
						strText.Format(_T("%d"), lstH_FD[i]);
						record[nNum].Add(strText);
						strText.Format(_T("%.2f"), DmgbD.FD.dForce);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), DmgbD.FD.dDisp);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dFdmax);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dUmax);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dWc);
						record[nNum].Add(strText);

						nNum++;
						break;
					}
				}

				for (i = 0; i < lstH_HDRD.GetCount(); i++)
				{
					if (lstH_HDRD[i] == lstNlnk[m])
					{
						if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstH_HDRD[i], NlnkD) || !m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
						{
							ASSERT(0);
							continue;
						}
						if (!m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD))
						{
							ASSERT(0);
							continue;
						}
						dWc = IDamperGBTool::Instance()->GetWc(arThis[k], lstH_HDRD[i], 0, dFdmax, dUmax);
						dSumWcX += dWc;
						strText.Format(_T("%d"), nNum + 1);
						record[nNum].Add(strText);
						strText.Format(_T("%d"), lstH_HDRD[i]);
						record[nNum].Add(strText);
						strText.Format(_T("%.2f"), DmgbD.HDRD.dFy);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), DmgbD.HDRD.dFy / DmgbD.HDRD.dInitStiff);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dFdmax);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dUmax);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dWc);
						record[nNum].Add(strText);

						nNum++;
						break;
					}

				}
			}
			TableD.RecordD.aRecordData.FreeExtra();
			TableD.RecordD.nRecordRowCount = lstH_BRB.GetCount() + lstH_MYD.GetCount() + lstH_FD.GetCount() + lstH_HDRD.GetCount();
			TableD.RecordD.nRecordColCount = 7;
			strTitle.Format(_T("X向位移型阻尼器耗能统计表格(%s)(kN,mm)"), ThisD.LoadCaseName);
			strBook.Format(_T("CHARPT_6_1_%d_TBL"), nIndex);
			MakeTableCenterText(genElements, TableD, strTitle, strBook);
			nIndex++;

			strText.Format(_T("X向位移型阻尼器耗能Wc：%.5fkN.mm"), dSumWcX);
			MakeText(genElements, strText);
		}


		//table Y
		if (lstV_BRB.GetCount() + lstV_MYD.GetCount() + lstV_FD.GetCount() + lstV_HDRD.GetCount() > 0)
		{
			TableD.Initialize();
			TableD.HeaderD.nHeaderRowCount = 1;
			TableD.HeaderD.nHeaderColCount = 7;
			TableD.HeaderD.aTitleName.Add(_T("序号"));
			TableD.HeaderD.aTitleName.Add(_T("阻尼器编号"));
			TableD.HeaderD.aTitleName.Add(_T("屈服力Fdy"));
			TableD.HeaderD.aTitleName.Add(_T("屈服位移Udy"));
			TableD.HeaderD.aTitleName.Add(_T("最大阻尼力Fdmax"));
			TableD.HeaderD.aTitleName.Add(_T("最大相对变形Umax"));
			TableD.HeaderD.aTitleName.Add(_T("阻尼器耗能Wc"));
			TableD.RecordD.aRecordData.SetSize(lstV_BRB.GetCount() + lstV_MYD.GetCount() + lstV_FD.GetCount() + lstV_HDRD.GetCount());
			dSumWcY = 0.;
			nNum = 0;
			for (int m = 0; m < lstNlnk.GetCount(); m++)
			{
				for (i = 0; i < lstV_BRB.GetCount(); i++)
				{
					if (lstV_BRB[i] == lstNlnk[m])
					{
						if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstV_BRB[i], NlnkD) || !m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
						{
							ASSERT(0);
							continue;
						}
						if (!m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD))
						{
							ASSERT(0);
							continue;
						}
						dWc = IDamperGBTool::Instance()->GetWc(arThis[k], lstV_BRB[i], 1, dFdmax, dUmax);
						dSumWcY += dWc;
						strText.Format(_T("%d"), nNum + 1);
						record[nNum].Add(strText);
						strText.Format(_T("%d"), lstV_BRB[i]);
						record[nNum].Add(strText);
						strText.Format(_T("%.2f"), DmgbD.BRB.dFy);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), DmgbD.BRB.dDisp);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dFdmax);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dUmax);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dWc);
						record[nNum].Add(strText);
						nNum++;
						break;
					}
				}

				for (i = 0; i < lstV_MYD.GetCount(); i++)
				{
					if (lstV_MYD[i] == lstNlnk[m])
					{
						if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstV_MYD[i], NlnkD) || !m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
						{
							ASSERT(0);
							continue;
						}
						if (!m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD))
						{
							ASSERT(0);
							continue;
						}
						dWc = IDamperGBTool::Instance()->GetWc(arThis[k], lstV_MYD[i], 1, dFdmax, dUmax);
						dSumWcY += dWc;
						strText.Format(_T("%d"), nNum + 1);
						record[nNum].Add(strText);
						strText.Format(_T("%d"), lstV_MYD[i]);
						record[nNum].Add(strText);
						strText.Format(_T("%.2f"), DmgbD.MYD.dFy);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), DmgbD.MYD.dDisp);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dFdmax);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dUmax);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dWc);
						record[nNum].Add(strText);
						nNum++;
						break;
					}
				}

				for (i = 0; i < lstV_FD.GetCount(); i++)
				{
					if (lstV_FD[i] == lstNlnk[m])
					{
						if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstV_FD[i], NlnkD) || !m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
						{
							ASSERT(0);
							continue;
						}
						if (!m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD))
						{
							ASSERT(0);
							continue;
						}
						dWc = IDamperGBTool::Instance()->GetWc(arThis[k], lstV_FD[i], 1, dFdmax, dUmax);
						dSumWcY += dWc;
						strText.Format(_T("%d"), nNum + 1);
						record[nNum].Add(strText);
						strText.Format(_T("%d"), lstV_FD[i]);
						record[nNum].Add(strText);
						strText.Format(_T("%.2f"), DmgbD.FD.dForce);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), DmgbD.FD.dDisp);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dFdmax);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dUmax);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dWc);
						record[nNum].Add(strText);
						nNum++;
						break;
					}
				}

				for (i = 0; i < lstV_HDRD.GetCount(); i++)
				{
					if (lstV_HDRD[i] == lstNlnk[m])
					{
						if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstV_HDRD[i], NlnkD) || !m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
						{
							ASSERT(0);
							continue;
						}
						if (!m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD))
						{
							ASSERT(0);
							continue;
						}
						dWc = IDamperGBTool::Instance()->GetWc(arThis[k], lstV_HDRD[i], 1, dFdmax, dUmax);
						dSumWcY += dWc;
						strText.Format(_T("%d"), nNum + 1);
						record[nNum].Add(strText);
						strText.Format(_T("%d"), lstV_HDRD[i]);
						record[nNum].Add(strText);
						strText.Format(_T("%.2f"), DmgbD.HDRD.dFy);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), DmgbD.HDRD.dFy / DmgbD.HDRD.dInitStiff);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dFdmax);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dUmax);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dWc);
						record[nNum].Add(strText);
						nNum++;
						break;
					}
				}
			}

			TableD.RecordD.aRecordData.FreeExtra();
			TableD.RecordD.nRecordRowCount = lstV_BRB.GetCount() + lstV_MYD.GetCount() + lstV_FD.GetCount() + lstV_HDRD.GetCount();
			TableD.RecordD.nRecordColCount = 7;
			strTitle.Format(_T("Y向位移型阻尼器耗能统计表格(%s)(kN,mm)"), ThisD.LoadCaseName);
			strBook.Format(_T("CHARPT_6_1_%d_TBL"), nIndex);
			MakeTableCenterText(genElements, TableD, strTitle, strBook);
			nIndex++;
			strText.Format(_T("Y向位移型阻尼器耗能Wc：%.5f kN.mm"), dSumWcY);
			MakeText(genElements, strText);
		}

#pragma endregion

#pragma region 速度型阻尼器耗能Wc
		
		//table X
		if (lstH_VFD.GetCount() + lstH_VED.GetCount() > 0)
		{
			TableD.Initialize();
			TableD.HeaderD.nHeaderRowCount = 1;
			TableD.HeaderD.nHeaderColCount = 6;
			TableD.HeaderD.aTitleName.Add(_T("序号"));
			TableD.HeaderD.aTitleName.Add(_T("阻尼器编号"));
			TableD.HeaderD.aTitleName.Add(_T("λ1"));
			TableD.HeaderD.aTitleName.Add(_T("最大阻尼力Fdmax"));
			TableD.HeaderD.aTitleName.Add(_T("最大相对变形Umax"));
			TableD.HeaderD.aTitleName.Add(_T("阻尼器耗能Wc"));
			TableD.RecordD.aRecordData.SetSize(lstH_VFD.GetCount() + lstH_VED.GetCount());

			dSumWcX = 0.;
			nNum = 0;
			for (int m = 0; m < lstNlnk.GetCount(); m++)
			{
				for (i = 0; i < lstH_VFD.GetCount(); i++)
				{
					if (lstH_VFD[i] == lstNlnk[m])
					{
						if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstH_VFD[i], NlnkD) || !m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
						{
							ASSERT(0);
							continue;
						}
						if (!m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD))
						{
							ASSERT(0);
							continue;
						}
						dWc = IDamperGBTool::Instance()->GetWc(arThis[k], lstH_VFD[i], 0, dFdmax, dUmax);
						dSumWcX += dWc;
						strText.Format(_T("%d"), nNum + 1);
						record[nNum].Add(strText);
						strText.Format(_T("%d"), lstH_VFD[i]);
						record[nNum].Add(strText);
						strText.Format(_T("%.2f"), IDamperGBTool::Instance()->CalLamda1(DmgbD.VFD.dIndex));
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dFdmax);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dUmax);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dWc);
						record[nNum].Add(strText);

						nNum++;
						break;
					}
				}

				for (i = 0; i < lstH_VED.GetCount(); i++)
				{
					if (lstH_VED[i] == lstNlnk[m])
					{
						if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstH_VED[i], NlnkD) || !m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
						{
							ASSERT(0);
							continue;
						}
						if (!m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD))
						{
							ASSERT(0);
							continue;
						}
						dWc = IDamperGBTool::Instance()->GetWc(arThis[k], lstH_VED[i], 0, dFdmax, dUmax);
						dSumWcX += dWc;
						strText.Format(_T("%d"), nNum + 1);
						record[nNum].Add(strText);
						strText.Format(_T("%d"), lstH_VED[i]);
						record[nNum].Add(strText);
						strText.Format(_T("%.2f"), IDamperGBTool::Instance()->CalLamda1(DmgbD.VED.dIndex));
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dFdmax);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dUmax);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dWc);
						record[nNum].Add(strText);

						nNum++;
						break;
					}
				}
			}

			TableD.RecordD.aRecordData.FreeExtra();
			TableD.RecordD.nRecordRowCount = lstH_VFD.GetCount() + lstH_VED.GetCount();
			TableD.RecordD.nRecordColCount = 6;
			strTitle.Format(_T("X向速度型阻尼器耗能统计表格(%s)(kN,mm)"), ThisD.LoadCaseName);
			strBook.Format(_T("CHARPT_6_1_%d_TBL"), nIndex);
			MakeTableCenterText(genElements, TableD, strTitle, strBook);
			nIndex++;
			strText.Format(_T("X向速度型阻尼器耗能Wc：%.5fkN.mm"), dSumWcX);
			MakeText(genElements, strText);
		}

		//table Y
		if (lstV_VFD.GetCount() + lstV_VED.GetCount())
		{
			TableD.Initialize();
			TableD.HeaderD.nHeaderRowCount = 1;
			TableD.HeaderD.nHeaderColCount = 6;
			TableD.HeaderD.aTitleName.Add(_T("序号"));
			TableD.HeaderD.aTitleName.Add(_T("阻尼器编号"));
			TableD.HeaderD.aTitleName.Add(_T("λ1"));
			TableD.HeaderD.aTitleName.Add(_T("最大阻尼力Fdmax"));
			TableD.HeaderD.aTitleName.Add(_T("最大相对变形Umax"));
			TableD.HeaderD.aTitleName.Add(_T("阻尼器耗能Wc"));
			TableD.RecordD.aRecordData.SetSize(lstV_VFD.GetCount() + lstV_VED.GetCount());
			dSumWcY = 0.;
			nNum = 0;

			for (int m = 0; m < lstNlnk.GetCount(); m++)
			{
				for (i = 0; i < lstV_VFD.GetCount(); i++)
				{
					if (lstV_VFD[i] == lstNlnk[m])
					{
						if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstV_VFD[i], NlnkD) || !m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
						{
							ASSERT(0);
							continue;
						}
						if (!m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD))
						{
							ASSERT(0);
							continue;
						}
						dWc = IDamperGBTool::Instance()->GetWc(arThis[k], lstV_VFD[i], 1, dFdmax, dUmax);
						dSumWcY += dWc;
						strText.Format(_T("%d"), nNum + 1);
						record[nNum].Add(strText);
						strText.Format(_T("%d"), lstV_VFD[i]);
						record[nNum].Add(strText);
						strText.Format(_T("%.2f"), IDamperGBTool::Instance()->CalLamda1(DmgbD.VFD.dIndex));
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dFdmax);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dUmax);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dWc);
						record[nNum].Add(strText);

						nNum++;
						break;
					}
				}

				for (i = 0; i < lstV_VED.GetCount(); i++)
				{
					if (lstV_VED[i] == lstNlnk[m])
					{
						if (!m_pDoc->m_pAttrCtrl->GetNlnk(lstV_VED[i], NlnkD) || !m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD))
						{
							ASSERT(0);
							continue;
						}
						if (!m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD))
						{
							ASSERT(0);
							continue;
						}
						dWc = IDamperGBTool::Instance()->GetWc(arThis[k], lstV_VED[i], 1, dFdmax, dUmax);
						dSumWcY += dWc;
						strText.Format(_T("%d"), nNum + 1);
						record[nNum].Add(strText);
						strText.Format(_T("%d"), lstV_VED[i]);
						record[nNum].Add(strText);
						strText.Format(_T("%.2f"), IDamperGBTool::Instance()->CalLamda1(DmgbD.VED.dIndex));
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dFdmax);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dUmax);
						record[nNum].Add(strText);
						strText.Format(_T("%.4f"), dWc);
						record[nNum].Add(strText);

						nNum++;
						break;
					}
				}
			}

			TableD.RecordD.aRecordData.FreeExtra();
			TableD.RecordD.nRecordRowCount = lstV_VFD.GetCount() + lstV_VED.GetCount();
			TableD.RecordD.nRecordColCount = 6;
			strTitle.Format(_T("Y向速度型阻尼器耗能统计表格(%s)(kN,mm)"), ThisD.LoadCaseName);
			strBook.Format(_T("CHARPT_6_1_%d_TBL"), nIndex);
			MakeTableCenterText(genElements, TableD, strTitle, strBook);
			nIndex++;
			strText.Format(_T("Y向速度型阻尼器耗能Wc：%.5fkN.mm"), dSumWcY);
			MakeText(genElements, strText);
		}

#pragma endregion

#pragma region 附加阻尼比的计算
		
		if (lstDamperX.GetCount() > 0)
		{
			WriteCharptAdditionalDamperRtoTable(genElements, arThis[k], lstDamperX, TRUE, nIndex);
			nIndex++;
		}

		if (lstDamperY.GetCount() > 0)
		{
			WriteCharptAdditionalDamperRtoTable(genElements, arThis[k], lstDamperY, FALSE, k + 7);
			nIndex++;
		}

#pragma endregion
	}

	m_Segments.push_back(genElements);
}
void AutoReport_Gen_CH_DMGB::WriteCharptAdditionalDamperRtoTable(GenSegmentElements& genElements, UINT ThisK, T_KEY_LIST& lstKey, BOOL bXDir, int nIndex)
{
	T_NLNK_D NlnkD;
	T_NLLP_D NllpD;
	T_DMGB_D DmgbD;

	T_TABLE_D TableD;
	TableD.Initialize();
	TableD.HeaderD.nHeaderRowCount = 1;
	TableD.HeaderD.nHeaderColCount = 5;
	TableD.HeaderD.aTitleName.Add(_T("名称"));
	TableD.HeaderD.aTitleName.Add(_T("类型"));
	TableD.HeaderD.aTitleName.Add(_T("阻尼器耗能Wc"));
	TableD.HeaderD.aTitleName.Add(_T("结构总应变能Ws"));
	TableD.HeaderD.aTitleName.Add(_T("阻尼器附加阻尼比ζ(%)"));
	int k = 0, nSumS = 0, nSumD = 0;
	double dSumWcS = 0., dSumWcD = 0.;
	double dKesi = 0.;
	double dFi = 0., dUi = 0.;
	double dWc = 0., dWs = 0.;
	CString strDampType[] = { _T("BRB"), _T("MYD") , _T("FD") , _T("VFD") , _T("HDRD") , _T("VED") };
	CString strType[3] = { _T("位移型阻尼器") ,_T("速度型阻尼器") ,_T("合计") };

	TableD.RecordD.aRecordData.SetSize(lstKey.GetCount() + 5);
	auto& record = TableD.RecordD.aRecordData;

	int nDir = bXDir ? 0 : 1;
	for (k = 0; k < lstKey.GetCount(); k++)
	{
		NlnkD.Initialize();
		m_pDoc->m_pAttrCtrl->GetNlnk(lstKey[k], NlnkD);
		NllpD.Initialize();
		m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD);
		DmgbD.Initialize();
		m_pDoc->m_pAttrCtrl->GetDmgb(NllpD.nSeisKey, DmgbD);
		dWc = IDamperGBTool::Instance()->GetWc(ThisK, lstKey[k], nDir, dFi, dUi);
		dWs = IDamperGBTool::Instance()->GetWs(ThisK, nDir, dFi, dUi);

		record[k].Add(DmgbD.PropName);
		record[k].Add(strDampType[DmgbD.nPropType]);
		record[k].Add(Double2String_f(dWc));
		record[k].Add(Double2String_f(dWs));
		if (fabs(dWs) > 0)
			dKesi = dWc / (4 * PI * dWs) * 100;
		else
			dKesi = 0.0;
		record[k].Add(Double2String_f(dKesi));

		if (DmgbD.nPropType == 3 || DmgbD.nPropType == 5)
		{
			nSumS += 1;
			dSumWcS += dWc;
		}
		else
		{
			nSumD += 1;
			dSumWcD += dWc;
		}
	}
	//record.SetSize(1);
	record[k].Add(_T("结果统计"));
	record[k].Add(_T(""));
	record[k].Add(_T(""));
	record[k].Add(_T(""));
	record[k].Add(_T(""));
	k++;
	//TableD.RecordD.aRecordData.SetSize(5);
	record[k].Add(_T("类型"));
	record[k].Add(_T("数量"));
	record[k].Add(_T("阻尼器耗能Wc"));
	record[k].Add(_T("结构总应变能Ws"));
	record[k].Add(_T("阻尼器附加阻尼比ζ(%)"));
	k++;
	record[k].Add(strType[0]);
	record[k].Add(Double2String_f(nSumD, 0));
	record[k].Add(Double2String_f(dSumWcD));
	record[k].Add(Double2String_f(dWs));
	if (fabs(dWs) > 0)
		dKesi = dSumWcD / (4 * PI * dWs) * 100;
	else
		dKesi = 0.00;
	record[k].Add(Double2String_f(dKesi));
	k++;
	record[k].Add(strType[1]);
	record[k].Add(Double2String_f(nSumS, 0));
	record[k].Add(Double2String_f(dSumWcS));
	record[k].Add(Double2String_f(dWs));
	if (fabs(dWs) > 0)
		dKesi = dSumWcS / (4 * PI * dWs) * 100;
	else
		dKesi = 0.00;
	record[k].Add(Double2String_f(dKesi));
	k++;
	record[k].Add(strType[2]);
	record[k].Add(Double2String_f(nSumD + nSumS, 0));
	record[k].Add(Double2String_f(dSumWcD + dSumWcS));
	record[k].Add(Double2String_f(dWs));
	if (fabs(dWs) > 0)
		dKesi = (dSumWcD + dSumWcS) / (4 * PI * (dWs)) * 100;
	else
		dKesi = 0.00;
	record[k].Add(Double2String_f(dKesi));
	CString strTitle, strBook;
	TableD.RecordD.aRecordData.FreeExtra();
	TableD.RecordD.nRecordRowCount = lstKey.GetCount() + 5;
	TableD.RecordD.nRecordColCount = 5;
	strTitle = bXDir ? _T("X方向上的附加阻尼比(kN,mm)") : _T("Y方向上的附加阻尼比(kN,mm)");
	T_THIS_D ThisD;
	m_pDoc->m_pAttrCtrl->GetThis(ThisK, ThisD);
	CString strTemp;
	strTemp.Format(_T("%s(%s)"), strTitle, ThisD.LoadCaseName);
	strBook.Format(_T("CHARPT_6_1_%d_TBL"), nIndex);
	MakeTableCenterText(genElements, TableD, strTemp, strBook);


}

void AutoReport_Gen_CH_DMGB::WriteCharpt6_3(GenSegmentElements &genElements)
{
	CString strText;
	CString strTitle, strBookTip;
	strText = _T("层间位移角验算");
	MakeTitle(genElements, strText, 6, 3);
	
	CString strStruct = _T("");
	switch (m_DagbD.nStrutCategory)
	{
	case 0: strStruct = _T("钢筋混凝土框架结构"); break;
	case 1: strStruct = _T("钢筋混凝土框架-抗震墙结构"); break;
	case 2: strStruct = _T("框架-核心筒结构"); break;
	case 3: strStruct = _T("板柱-抗震墙结构"); break;
	case 4: strStruct = _T("钢筋混凝土抗震墙"); break;
	case 5: strStruct = _T("筒中筒"); break;
	case 6: strStruct = _T("钢筋混凝土框支层结构"); break;
	case 7: strStruct = _T("多高层钢结构"); break;
	default:
		break;
	}

	CString strBldCategory = _T("");
	switch (m_DagbD.nBldCategory)
	{
	case 0: strBldCategory = _T("Ⅰ类"); break;
	case 1: strBldCategory = _T("Ⅱ类"); break;
	case 2: strBldCategory = _T("一般"); break;
	default:
		break;
	}
	
	int nSplc1 = m_DagbD.aResistSplcCase.GetCount();
	int nSplc2 = m_DagbD.aRareESplcCase.GetCount();
	int nThis1 = m_DagbD.aResistThisCase.GetCount();
	int nThis2 = m_DagbD.aRareEThisCase.GetCount();
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

	//Table list 1
	int nLmtType;
	int nLmtValue;
	BOOL bBJGeneral = m_DagbD.nCode == 1 && m_DagbD.nBldCategory == 2;
	if (bBJGeneral)
	{
		int nSplc = m_DagbD.aNonUseSplcCase.GetCount();

		if (nSplc > 0)
		{
			nLmtType = IDamperGBTool::Instance()->GetInterStoryDispAngleLimtTypeSplc(m_DagbD.aNonUseSplcCase[0]);
			nLmtValue = IDamperGBTool::Instance()->GetInterStoryDispAngleLimtValue_Splc(nLmtType);
			strText.Format(_T("    根据《建筑工程减隔震技术规程》北京市地方标准 DB11/2075-2022第4.5.1条，对于%s建筑，在多遇地震作用下的弹性层间位移角限值为1/%d，层间位移角验算结果如下："), strStruct, nLmtValue);
			MakeText(genElements, strText);

			for (i = 0; i < nSplc; i++)
			{
				arStorDrift.RemoveAll();
				SplcD.Initialize();
				SplcK = m_DagbD.aNonUseSplcCase[i];
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
				for (j = arStorDrift.GetCount() - 1; j >= 0; j--)
				{
					CStringArray& aStringDrift = TableD.RecordD.aRecordData[nStorIndex++];
					aStringDrift.Add(arStorDrift[j].strStorName);
					aStringDrift.Add(Double2String_f(arStorDrift[j].dHeight));
					StorD.Initialize();
					strText.Format(_T("%d"), arStorDrift[j].Max[2].NodeK);
					aStringDrift.Add(strText);
					aStringDrift.Add(Double2String_f(arStorDrift[j].Max[2].dDrift, 4));
					if (fabs(arStorDrift[j].dAllowableRatio) > cGen_Zero)
					{
						strText.Format(_T("1/%d"), int(1 / arStorDrift[j].Max[2].dRatio + 0.5));
						aStringDrift.Add(strText);
					}
					strText.Format(_T("1/%d"), nLmtValue);
					aStringDrift.Add(strText);

					if (nLmtValue > 1 / arStorDrift[j].Max[2].dRatio)
						strText = _T("NG");
					else
						strText = _T("OK");
					aStringDrift.Add(strText);
				}
				TableD.RecordD.aRecordData.FreeExtra();
				TableD.RecordD.nRecordRowCount = arStorDrift.GetCount();
				TableD.RecordD.nRecordColCount = 7;

				strTitle.Format(_T("层间位移角验算结果(多遇地震：%s)"), SplcD.LoadCaseName);
				strBookTip.Format(_T("CHARPT_6_3_TBL%d"), ++nIndex);
				MakeTableCenterText(genElements, TableD, strTitle, strBookTip);
			}
		}
	}
	if (nSplc1 > 0)
	{
		nLmtType = IDamperGBTool::Instance()->GetInterStoryDispAngleLimtTypeSplc(m_DagbD.aResistSplcCase[0]);
		nLmtValue = IDamperGBTool::Instance()->GetInterStoryDispAngleLimtValue_Splc(nLmtType);

		if (m_DagbD.nCode == 1)
		{
			strText.Format(_T("    根据《建筑工程减隔震技术规程》北京市地方标准 DB11/2075-2022第5.4.1条，对于%s建筑，该%s在设防地震作用下的弹塑性层间位移角限值为1/%d，层间位移角验算结果如下："), strBldCategory, strStruct, nLmtValue);
		}
		else
			strText.Format(_T("    根据《基于保持建筑正常使用功能的抗震技术导则》RISN-TG046-2023第4.3节，对于%s建筑，该%s在设防地震作用下的弹塑性层间位移角限值为1/%d，层间位移角验算结果如下："), strBldCategory, strStruct, nLmtValue);
		MakeText(genElements, strText);

		for (i = 0; i < nSplc1; i++)
		{
			arStorDrift.RemoveAll();
			SplcD.Initialize();
			SplcK = m_DagbD.aResistSplcCase[i];
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
			for (j = arStorDrift.GetCount() - 1; j >= 0; j--)
			{
				CStringArray& aStringDrift = TableD.RecordD.aRecordData[nStorIndex++];
				aStringDrift.Add(arStorDrift[j].strStorName);
				aStringDrift.Add(Double2String_f(arStorDrift[j].dHeight));
				StorD.Initialize();
				strText.Format(_T("%d"), arStorDrift[j].Max[2].NodeK);
				aStringDrift.Add(strText);
				aStringDrift.Add(Double2String_f(arStorDrift[j].Max[2].dDrift, 4));
				if (fabs(arStorDrift[j].dAllowableRatio) > cGen_Zero)
				{
					strText.Format(_T("1/%d"), int(1 / arStorDrift[j].Max[2].dRatio + 0.5));
					aStringDrift.Add(strText);
				}
				strText.Format(_T("1/%d"), nLmtValue);
				aStringDrift.Add(strText);

				if (nLmtValue > 1 / arStorDrift[j].Max[2].dRatio)
					strText = _T("NG");
				else
					strText = _T("OK");
				aStringDrift.Add(strText);
			}
			TableD.RecordD.aRecordData.FreeExtra();
			TableD.RecordD.nRecordRowCount = arStorDrift.GetCount();
			TableD.RecordD.nRecordColCount = 7;

			if (m_DagbD.nCode == 1 && m_DagbD.nBldCategory == 2)
				strTitle.Format(_T("层间位移角验算结果(多遇地震：%s)"), SplcD.LoadCaseName);
			else
				strTitle.Format(_T("层间位移角验算结果(设防地震：%s)"), SplcD.LoadCaseName);
			strBookTip.Format(_T("CHARPT_6_3_TBL%d"), ++nIndex);
			MakeTableCenterText(genElements, TableD, strTitle, strBookTip);
		}
	}	
	

	//Table list 2
	if (nSplc2 > 0)
	{
		nLmtType = IDamperGBTool::Instance()->GetInterStoryDispAngleLimtTypeSplc(m_DagbD.aRareESplcCase[0]);
		nLmtValue = IDamperGBTool::Instance()->GetInterStoryDispAngleLimtValue_Splc(nLmtType);

		if (m_DagbD.nCode == 1)
		{
			if (m_DagbD.nBldCategory == 2)
				strText.Format(_T("    根据《建筑工程减隔震技术规程》北京市地方标准 DB11/2075-2022第4.5.1条，对于%s建筑，在罕遇地震作用下的弹性层间位移角限值为1/%d，层间位移角验算结果如下：："), strStruct, nLmtValue);
			else
				strText.Format(_T("    根据《建筑工程减隔震技术规程》北京市地方标准 DB11/2075-2022第5.4.1条，对于%s建筑，该%s在罕遇地震作用下的弹塑性层间位移角限值为1/%d，层间位移角验算结果如下："), strBldCategory, strStruct, nLmtValue);
		}
		else
			strText.Format(_T("    根据《基于保持建筑正常使用功能的抗震技术导则》RISN-TG046-2023第4.3节，对于%s建筑，该%s在罕遇地震作用下的弹塑性层间位移角限值为1/%d，层间位移角验算结果如下："), strBldCategory, strStruct, nLmtValue);
		MakeText(genElements, strText);
		for (i = 0; i < nSplc2; i++)
		{
			arStorDrift.RemoveAll();
			SplcD.Initialize();
			SplcK = m_DagbD.aRareESplcCase[i];
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
			for (j = arStorDrift.GetCount() - 1; j >= 0; j--)
			{
				//Drift = arStorDrift[j];
				CStringArray& aStringDrift = TableD.RecordD.aRecordData[nStorIndex++];
				aStringDrift.Add(arStorDrift[j].strStorName);
				aStringDrift.Add(Double2String_f(arStorDrift[j].dHeight));

				strText.Format(_T("%d"), arStorDrift[j].Max[2].NodeK);
				aStringDrift.Add(strText);
				aStringDrift.Add(Double2String_f(arStorDrift[j].Max[2].dDrift, 4));
				if (fabs(arStorDrift[j].dAllowableRatio) > cGen_Zero)
				{
					strText.Format(_T("1/%d"), int(1 / arStorDrift[j].Max[2].dRatio));
					aStringDrift.Add(strText);
				}
				strText.Format(_T("1/%d"), nLmtValue);
				aStringDrift.Add(strText);
				strText = (arStorDrift[j].Cen[2].nRemark == D_STOR_DFT_REMARK_OK) ? _T("OK") : _T("NG");
				aStringDrift.Add(strText);
			}
			TableD.RecordD.aRecordData.FreeExtra();
			TableD.RecordD.nRecordRowCount = arStorDrift.GetCount();
			TableD.RecordD.nRecordColCount = 7;

			strTitle.Format(_T("层间位移角验算结果(罕遇地震：%s)"), SplcD.LoadCaseName);
			strBookTip.Format(_T("CHARPT_6_3_TBL%d"), ++nIndex);
			MakeTableCenterText(genElements, TableD, strTitle, strBookTip);
		}
	}	
	

	///////////////this
	//Table list 3
	if (nThis1 > 0 && !bBJGeneral)
	{
		nLmtType = IDamperGBTool::Instance()->GetInterStoryDispAngleLimtTypeThis(m_DagbD.aResistThisCase[0]);
		nLmtValue = IDamperGBTool::Instance()->GetInterStoryDispAngleLimtValue_This(nLmtType);

		if (m_DagbD.nCode == 1)
		{
			if (m_DagbD.nBldCategory == 2)
				strText.Format(_T("    根据《建筑工程减隔震技术规程》北京市地方标准 DB11/2075-2022第4.5.1条，对于%s建筑，在多遇地震作用下的弹性层间位移角限值为1/%d，层间位移角验算结果如下："), strStruct, nLmtValue);
			else
				strText.Format(_T("    根据《建筑工程减隔震技术规程》北京市地方标准 DB11/2075-2022第5.4.1条，对于%s建筑，该%s在设防地震作用下的弹塑性层间位移角限值为1/%d，层间位移角验算结果如下："), strBldCategory, strStruct, nLmtValue);
		}
		else
			strText.Format(_T("    根据《基于保持建筑正常使用功能的抗震技术导则》RISN-TG046-2023第4.3节，对于%s建筑，该%s在设防地震作用下的弹塑性层间位移角限值为1/%d，层间位移角验算结果如下："), strBldCategory, strStruct, nLmtValue);
		MakeText(genElements, strText);
		for (i = 0; i < nThis1; i++)
		{
			ThisD.Initialize();
			ThisK = m_DagbD.aResistThisCase[i];
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
			nStorIndex = 0;
			nMinMax = 0;// aAllMinMax[m];
			if (!StDrft.GetDFT4TH(LcomData, nMinMax, arStorDriftTH)) continue;
			TableD.RecordD.aRecordData.SetSize(arStorDriftTH.GetCount());
			for (j = arStorDriftTH.GetCount() - 1; j >= 0; j--)
			{
				DriftTH = arStorDriftTH[j];
				CStringArray& aStringDrift = TableD.RecordD.aRecordData[nStorIndex++];
				aStringDrift.Add(DriftTH.strStorName);
				aStringDrift.Add(Double2String_f(DriftTH.dHeight));
				aStringDrift.Add(_T("-"));
				if (fabs(DriftTH.Axis[0].dRatio - DriftTH.Axis[1].dRatio) >= 0)
				{
					aStringDrift.Add(Double2String_f(fabs(DriftTH.Axis[0].dDrift), 4));
					if (fabs(DriftTH.dAllowableRatio) > cGen_Zero)
					{
						strText.Format(_T("1/%.0f"), 1. / fabs(DriftTH.Axis[0].dRatio));
						aStringDrift.Add(strText);
					}
					strText.Format(_T("1/%d"), nLmtValue);
					aStringDrift.Add(strText);
					strText = (DriftTH.Axis[0].nRemark == 0) ? _T("OK") : _T("NG");
					aStringDrift.Add(strText);
				}
				else
				{
					aStringDrift.Add(Double2String_f(fabs(DriftTH.Axis[1].dDrift), 4));
					if (fabs(DriftTH.dAllowableRatio) > cGen_Zero)
					{
						strText.Format(_T("1/%.0f"), 1. / fabs(DriftTH.Axis[1].dRatio));
						aStringDrift.Add(strText);
					}
					strText.Format(_T("1/%d"), nLmtValue);
					aStringDrift.Add(strText);
					strText = (DriftTH.Axis[1].nRemark == 0) ? _T("OK") : _T("NG");
					aStringDrift.Add(strText);
				}
			}

			TableD.RecordD.aRecordData.FreeExtra();
			TableD.RecordD.nRecordRowCount = arStorDriftTH.GetCount();
			TableD.RecordD.nRecordColCount = 7;
			if (m_DagbD.nCode == 1 && m_DagbD.nBldCategory == 2)
				strTitle.Format(_T("层间位移角验算结果(多遇地震：%s)"), ThisD.LoadCaseName);
			else
				strTitle.Format(_T("层间位移角验算结果(设防地震：%s)"), ThisD.LoadCaseName);
			strBookTip.Format(_T("CHARPT_6_3_TBL%d"), ++nIndex);
			MakeTableCenterText(genElements, TableD, strTitle, strBookTip);
		}

	}	
	
	//Table list 4
	if (nThis2 > 0)
	{
		nLmtType = IDamperGBTool::Instance()->GetInterStoryDispAngleLimtTypeThis(m_DagbD.aRareEThisCase[0]);
		nLmtValue = IDamperGBTool::Instance()->GetInterStoryDispAngleLimtValue_This(nLmtType);

		if (m_DagbD.nCode == 1)
		{
			if (m_DagbD.nBldCategory == 2)
				strText.Format(_T("    根据《建筑工程减隔震技术规程》北京市地方标准 DB11/2075-2022第4.5.1条，对于%s建筑，在罕遇地震作用下的弹性层间位移角限值为1/%d，层间位移角验算结果如下：："), strStruct, nLmtValue);
			else
				strText.Format(_T("    根据《建筑工程减隔震技术规程》北京市地方标准 DB11/2075-2022第5.4.1条，对于%s建筑，该%s在罕遇地震作用下的弹塑性层间位移角限值为1/%d，层间位移角验算结果如下："), strBldCategory, strStruct, nLmtValue);
		}
		else
			strText.Format(_T("    根据《基于保持建筑正常使用功能的抗震技术导则》RISN-TG046-2023第4.3节，对于%s建筑，该%s在罕遇地震作用下的弹塑性层间位移角限值为1/%d，层间位移角验算结果如下："), strBldCategory, strStruct, nLmtValue);
		MakeText(genElements, strText);
		for (i = 0; i < nThis2; i++)
		{
			ThisD.Initialize();
			ThisK = m_DagbD.aRareEThisCase[i];
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
			nStorIndex = 0;
			nMinMax = 0;// aAllMinMax[m];
			if (!StDrft.GetDFT4TH(LcomData, nMinMax, arStorDriftTH)) continue;
			TableD.RecordD.aRecordData.SetSize(arStorDriftTH.GetCount());

			for (j = arStorDriftTH.GetCount() - 1; j >= 0; j--)
			{
				DriftTH = arStorDriftTH[j];
				CStringArray& aStringDrift = TableD.RecordD.aRecordData[nStorIndex++];
				aStringDrift.Add(DriftTH.strStorName);
				aStringDrift.Add(Double2String_f(DriftTH.dHeight));
				aStringDrift.Add(_T("-"));
				if (fabs(DriftTH.Axis[0].dRatio - DriftTH.Axis[1].dRatio) >= 0)
				{
					aStringDrift.Add(Double2String_f(fabs(DriftTH.Axis[0].dDrift), 4));
					if (fabs(DriftTH.dAllowableRatio) > cGen_Zero)
					{
						strText.Format(_T("1/%.0f"), 1. / fabs(DriftTH.Axis[0].dRatio));
						aStringDrift.Add(strText);
					}
					strText.Format(_T("1/%d"), nLmtValue);
					aStringDrift.Add(strText);
					strText = (DriftTH.Axis[0].nRemark == 0) ? _T("OK") : _T("NG");
					aStringDrift.Add(strText);
				}
				else
				{
					aStringDrift.Add(Double2String_f(fabs(DriftTH.Axis[1].dDrift), 4));
					if (fabs(DriftTH.dAllowableRatio) > cGen_Zero)
					{
						strText.Format(_T("1/%.0f"), 1. / fabs(DriftTH.Axis[1].dRatio));
						aStringDrift.Add(strText);
					}
					strText.Format(_T("1/%d"), nLmtValue);
					aStringDrift.Add(strText);
					strText = (DriftTH.Axis[1].nRemark == 0) ? _T("OK") : _T("NG");
					aStringDrift.Add(strText);
				}

			}

			TableD.RecordD.aRecordData.FreeExtra();
			TableD.RecordD.nRecordRowCount = arStorDriftTH.GetCount();
			TableD.RecordD.nRecordColCount = 7;

			strTitle.Format(_T("层间位移角验算结果(罕遇地震：%s)"), ThisD.LoadCaseName);
			strBookTip.Format(_T("CHARPT_6_3_TBL%d"), ++nIndex);
			MakeTableCenterText(genElements, TableD, strTitle, strBookTip);
		}
	}
	
}
void AutoReport_Gen_CH_DMGB::WriteCharpt6_4(GenSegmentElements &genElements)
{
	if (m_DagbD.nCode == 1 && m_DagbD.nBldCategory == 2)
		return;

	CString strText, strTemp;
	strText = _T("最大楼面水平加速度");
	MakeTitle(genElements, strText, 6, 4);

	CString strBldCategory = _T("");
	switch (m_DagbD.nBldCategory)
	{
	case 0: strBldCategory = _T("Ⅰ类"); break;
	case 1: strBldCategory = _T("Ⅱ类"); break;
	case 2: strBldCategory = _T("一般"); break;
	default:
		break;
	}

	CString strSeisType[2] = { _LS(IDS_TB_DS_DAMPER_GB_FLOOR_HORACCE_EARTHTYPE1), _LS(IDS_TB_DS_DAMPER_GB_FLOOR_HORACCE_EARTHTYPE2) };

	if (m_DagbD.nCode == 0)
	strText.Format(_T("   根据《基于保持建筑正常使用功能的抗震技术导则》RISN-TG046-2023第4.4.1条，对于%s建筑，地震正常使用建筑的最大楼面水平加速度限值验算结果如下:"), strBldCategory);
	else
	strText.Format(_T("    根据《建筑工程减隔震技术规程》北京市地方标准 DB11/2075-2022第5.4.2条，对于%s建筑，地震正常使用建筑的最大楼面水平加速度限值验算结果如下:"), strBldCategory);
	MakeText(genElements, strText);

	CAutoRecoveryUnit unitSet(D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_FORCE_INDEX_KN, m_pDoc->m_pUnitCtrl, TRUE);

	T_THIS_D ThisD;
	T_THIS_K ThisK;
	_Disp_HorAcce Disp, CenDisp;
	T_LCOM_D LcomData;

	T_TABLE_D TableD;
	TableD.Initialize();
	TableD.HeaderD.nHeaderRowCount = 1;
	TableD.HeaderD.nHeaderColCount = 8;
	TableD.HeaderD.aTitleName.Add(_T("荷载工况"));
	TableD.HeaderD.aTitleName.Add(_T("地震类型"));
	TableD.HeaderD.aTitleName.Add(_T("最大楼面水平加速度X(g)"));
	TableD.HeaderD.aTitleName.Add(_T("最大楼面水平加速度Y(g)"));
	TableD.HeaderD.aTitleName.Add(_T("楼面质心水平加速度X(g)"));
	TableD.HeaderD.aTitleName.Add(_T("楼面质心水平加速度Y(g)"));
	TableD.HeaderD.aTitleName.Add(_T("限值(g)"));
	TableD.HeaderD.aTitleName.Add(_T("结果"));
	TableD.RecordD.aRecordData.SetSize(m_DagbD.aResistThisCase.GetCount() + m_DagbD.aRareEThisCase.GetCount());
	double dLimit = 0.0;
	int nCase = 0;
	int nSeisType = 0;
	for (int i = 0; i < m_DagbD.aResistThisCase.GetCount(); i++)
	{
		ThisD.Initialize();
		ThisK = m_DagbD.aResistThisCase[i];
		m_pDoc->m_pAttrCtrl->GetThis(ThisK, ThisD);
		CalcCharpt6_4_DampFloorHorAcce(ThisK, Disp, CenDisp);
		m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_HISTORY, ThisK, LcomData);
		nSeisType = IDamperGBTool::Instance()->GetInterStoryDispAngleLimtTypeThis(ThisK);
		dLimit = IDamperGBTool::Instance()->GetDampFloorHorAcceLimitValue(nSeisType, m_DagbD.nBldCategory);
		CStringArray& aString = TableD.RecordD.aRecordData[nCase++];
		aString.Add(ThisD.LoadCaseName + _LS(IDS_TB_RESULTVIEW__TH_all_));
		aString.Add(strSeisType[nSeisType-1]);
		strText.Format(_T("%.2f"), Disp.dDisp[0]);
		aString.Add(strText);
		strText.Format(_T("%.2f"), Disp.dDisp[1]);
		aString.Add(strText);
		strText.Format(_T("%.2f"), CenDisp.dDisp[0]);
		aString.Add(strText);
		strText.Format(_T("%.2f"), CenDisp.dDisp[1]);
		aString.Add(strText);
		if (nSeisType == 2 && m_DagbD.nBldCategory == 1)
			strText = _T("-");
		else
			strText.Format(_T("%.15g"), dLimit);
		aString.Add(strText);
		BOOL bNG = (CenDisp.dDisp[0] > dLimit || CenDisp.dDisp[1] > dLimit) ? TRUE : FALSE;
		strText = bNG ? _T("NG") : _T("OK");
		aString.Add(strText);
	}

	for (int i = 0; i < m_DagbD.aRareEThisCase.GetCount(); i++)
	{
		ThisD.Initialize();
		ThisK = m_DagbD.aRareEThisCase[i];
		m_pDoc->m_pAttrCtrl->GetThis(ThisK, ThisD);
		CalcCharpt6_4_DampFloorHorAcce(ThisK, Disp, CenDisp);
		m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_HISTORY, ThisK, LcomData);
		nSeisType = IDamperGBTool::Instance()->GetInterStoryDispAngleLimtTypeThis(ThisK);
		dLimit = IDamperGBTool::Instance()->GetDampFloorHorAcceLimitValue(nSeisType, m_DagbD.nBldCategory);
		CStringArray& aString = TableD.RecordD.aRecordData[nCase++];
		aString.Add(ThisD.LoadCaseName + _LS(IDS_TB_RESULTVIEW__TH_all_));
		aString.Add(strSeisType[nSeisType-1]);
		strText.Format(_T("%.2f"), Disp.dDisp[0]);
		aString.Add(strText);
		strText.Format(_T("%.2f"), Disp.dDisp[1]);
		aString.Add(strText);
		strText.Format(_T("%.2f"), CenDisp.dDisp[0]);
		aString.Add(strText);
		strText.Format(_T("%.2f"), CenDisp.dDisp[1]);
		aString.Add(strText);
		if (nSeisType == 2 && m_DagbD.nBldCategory == 1)
			strText = _T("-");
		else
			strText.Format(_T("%.15g"), dLimit);
		aString.Add(strText);
		BOOL bNG = (CenDisp.dDisp[0] > dLimit || CenDisp.dDisp[1] > dLimit) ? TRUE : FALSE;
		strText = bNG ? _T("NG") : _T("OK");
		aString.Add(strText);
	}
	TableD.RecordD.aRecordData.FreeExtra();
	TableD.RecordD.nRecordRowCount = m_DagbD.aResistThisCase.GetCount() + m_DagbD.aRareEThisCase.GetCount();
	TableD.RecordD.nRecordColCount = 8;

	CString strTitle, strBookTip;
	strTitle.Format( ThisD.LoadCaseName);
	strBookTip = _T("CHARPT_6_4_TBL");
	MakeTableCenterText(genElements, TableD, _T("最大楼面水平加速度"), strBookTip);
}

void AutoReport_Gen_CH_DMGB::CalcCharpt6_4_DampFloorHorAcce(UINT ThisK, _Disp_HorAcce &Disp, _Disp_HorAcce &CenDisp)
{
	CArray<UINT, UINT> sKey;  sKey.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetNodeKeyList(sKey);

	CMap<T_STOR_K, T_STOR_K, T_StoryMass, T_StoryMass&> mapStoryMass;
	CMap<T_NODE_K, T_NODE_K, T_StoryMass, T_StoryMass&> mapNodalMass;
	mapNodalMass.InitHashTable(HASHSIZENODE);
	CStoryLoad storyMass;
	storyMass.Calc_Nodal_Mass(mapStoryMass, mapNodalMass);
	T_StoryMass mass;

	m_pDoc->m_pPostCtrl->SelectThis(ThisK);
	T_NODE_K NodeKey;
	double dDX_max =0, dDY_max = 0;
	double dSum_Mx = 0.0, dSum_My = 0.0;
	double dSum_AMx = 0.0, dSum_AMy = 0.0;
	T_DISP_D DispAbs[2];
	for (int i=0; i<sKey.GetCount(); i++)
	{
		NodeKey = sKey[i];
		m_pDoc->m_pPostCtrl->GetThisInfo()->GetRAcclWithTime(NodeKey, 0, 0, &DispAbs[0], 0, 0, &DispAbs[1]);
		if (fabs(DispAbs[0].dblDisp[0]) > fabs(dDX_max))
			dDX_max = DispAbs[0].dblDisp[0];
		if (fabs(DispAbs[0].dblDisp[1]) > fabs(dDY_max))
			dDY_max = DispAbs[0].dblDisp[1];

		if (!mapNodalMass.Lookup(sKey[i], mass)) continue;

		dSum_Mx += mass.Sum.Mass[0];
		dSum_My += mass.Sum.Mass[1];

		dSum_AMx += mass.Sum.Mass[0] * fabs(DispAbs[0].dblDisp[0]);
		dSum_AMy += mass.Sum.Mass[1] * fabs(DispAbs[0].dblDisp[1]);
	}
	
	T_STYP_D DataStyp;
	if (!m_pDoc->m_pAttrCtrl->GetStyp(DataStyp))
	{
		DataStyp.Initialize();
		DataStyp.dblGravity = m_pDoc->m_pInitCtrl->DefaultGravity();
	}

	Disp.dDisp[0] = fabs(dDX_max / DataStyp.dblGravity);
	Disp.dDisp[1] = fabs(dDY_max / DataStyp.dblGravity);

	CenDisp.dDisp[0] = fabs(dSum_AMx / dSum_Mx / DataStyp.dblGravity);
	CenDisp.dDisp[1] = fabs(dSum_AMy / dSum_My / DataStyp.dblGravity);
}

void AutoReport_Gen_CH_DMGB::WriteCharpt7()
{
	GenSegmentElements genElements;
	MakeTitle(genElements, _T("非线性时程分析结果"), 7, 0);
	WriteCharpt7_1(genElements);
	WriteCharpt7_2(genElements);
	m_Segments.push_back(genElements);
}
void AutoReport_Gen_CH_DMGB::WriteCharpt7_1(GenSegmentElements &genElements)
{
	CString strText, strName;
	strText = _T("隔震支座滞回曲线");
	MakeTitle(genElements, strText, 7, 1);
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
		strBookMark.Format(_T("CHARPT_7_1_%d_FIG"), i + 1);
		MakePicture(genElements, strText, strBookMark);
	}
}
void AutoReport_Gen_CH_DMGB::WriteCharpt7_2(GenSegmentElements &genElements)
{
	CString strText, strBookMark;
	strText = _T("能量图结果");
	MakeTitle(genElements, strText, 7, 2);
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
		strBookMark.Format(_T("CHARPT_7_2_%d_FIG"), ++nIndex);
		MakePicture(genElements, ThisD.LoadCaseName, strBookMark);
	}
}

void AutoReport_Gen_CH_DMGB::FigureRltGM(BOOL bAnimation)
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
void AutoReport_Gen_CH_DMGB::WriteForceAndDispRlt(GenSegmentElements &Elems,I_GPSModel* gm,const Report_Key_LIST& aKeys,const CString& strBook)
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
void AutoReport_Gen_CH_DMGB::WriteModelRlt(GenSegmentElements &Elems,I_GPSModel* gm,const Report_Key_LIST& aKeys,Report_Key_LIST& aGroupKeys,int& nIndex)
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

int AutoReport_Gen_CH_DMGB::lex(FILE *fp, char *yytext, CStringArray& aString)
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
	//if (aString.GetCount() == 6)
	//{
	//	bool bcheck = true;
	//	for (int i = 2; i < 6; i++)
	//	{
	//		dchecktemp = _ttof(aString[i].GetBuffer(0));
	//		if (fabs(dchecktemp) < 1.0e-20)
	//		{
	//			bcheck = false;
	//			break;
	//		}
	//	}
	//	if (bcheck)
	//		return 1;
	//	else
	//	{
	//		aString.RemoveAll();
	//		k = 0;
	//	}
	//}
	yytext[index++] = '\0';
	return 1;
}

BOOL AutoReport_Gen_CH_DMGB::ReadNonlistValueFromDataLine(CString dataLine, CArray<CString, CString> &nlValue)
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

BOOL AutoReport_Gen_CH_DMGB::IsBlankLine(CString dataLine)
{
	for (int i = 0; i < dataLine.GetLength(); i++)
		if (dataLine[i] != ' ' &&	dataLine[i] != '\0' && dataLine[i] != '\n') return FALSE;

	return TRUE;
}

BOOL AutoReport_Gen_CH_DMGB::IsSingleLine(CString dataLine)
{
	if (dataLine.Find(_LS(IDS_MAIN_REPORT_TXT_LINE_SINGLE)) >= 0)
		return TRUE;
	return FALSE;
}

BOOL AutoReport_Gen_CH_DMGB::GetDataLine(char *str, CString &dataLine, BOOL bData/* = FALSE*/)
{
	int	ret = 0, len = 0;

	dataLine.Format(_T("%s"), str);
	if (!bData)
		dataLine.Remove(' ');

	//dataLine.Replace('\n', ' ');
	return TRUE;
}

BOOL AutoReport_Gen_CH_DMGB::GetConnectStringBySlush(CString &dataLine)
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

BOOL AutoReport_Gen_CH_DMGB::DevideLine(CString dataLine, CArray<CString, CString> &nlValue)
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

BOOL AutoReport_Gen_CH_DMGB::ErrMsg(CString sLine, CString cmt)
{
	CString ccm;
	sLine.Remove('\n');
	cmt.Remove('\n');
	ccm.Format(_T(" %s    %s"), sLine, cmt);
	return TRUE;
}

double AutoReport_Gen_CH_DMGB::GetFabsMax(double v1, double v2)
{
	if (fabs(v1) > fabs(v2))
		return v1;
	return v2;
}