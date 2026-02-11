// ReportAutoRegeneration.cpp : implementation file
//

#include "stdafx.h"
#include "AutoReport_Gen_CH.h"
#include "ReportTreeText.h"
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
#include "..\wg_db\DispOptMgr.h"
#include "AutoReport_Gen_Mgr.h"
#include "..\wg_dgn\DgnAmrSectionTb.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTreeReportCtrl* AutoReport_Gen_Base::TreeReportCtrl()
{
	CTreeReportCtrl* pReportTree = CTreeReportCtrlInstance::GetTreeReportCtrlInstance(); ASSERT(pReportTree);
	return pReportTree;
}

void AutoReport_Gen_Base::InsertCoverPage()
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

	strTemp.Format(_T("[DRG_FONT]|FC| size=30 bold=true align=center underline=0[/DRG_FONT]\n\n\n%s\n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_CD_RPT_COVERPAGE));
	strText += strTemp;
	strTemp.Format(_T("[DRG_FONT]|FC| size=30 bold=true align=center [/DRG_FONT]%s\n[DRG_FONT]|FI|[/DRG_FONT]"), strProjectName);
	strText += strTemp;

	CString strDate = _T("");
	CTime time;
	time = CTime::GetCurrentTime();
	int nYear = time.GetYear();
	int nMonth = time.GetMonth();
	int nDate = time.GetDay();
	strDate.Format(_T("%d-%d-%d"), nYear, nMonth, nDate);
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=false align=left[/DRG_FONT]\n\n\n\n\n\n\n%s%s[DRG_FONT]|FI|[/DRG_FONT]"), FillBlank(_T(""), 13), _LS(IDS_CD_RPT_COVERPAGE_DESIGN));
	strText += strTemp;
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=false align=left underline=1[/DRG_FONT]%s\n[DRG_FONT]|FI|[/DRG_FONT]"), FillBlank(data.strReviewName[0]));
	strText += strTemp;
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=false align=left[/DRG_FONT]%s%s[DRG_FONT]|FI|[/DRG_FONT]"), FillBlank(_T(""), 14), _LS(IDS_CD_RPT_COVERPAGE_CHECK));
	strText += strTemp;
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=false align=left underline=1[/DRG_FONT]%s\n[DRG_FONT]|FI|[/DRG_FONT]"), FillBlank(data.strReviewName[1]));
	strText += strTemp;
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=false align=left[/DRG_FONT]%s%s[DRG_FONT]|FI|[/DRG_FONT]"), FillBlank(_T(""), 11), _LS(IDS_CD_RPT_COVERPAGE_APPROVED));
	strText += strTemp;
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=false align=left underline=1[/DRG_FONT]%s\n[DRG_FONT]|FI|[/DRG_FONT]"), FillBlank(data.strReviewName[4]));
	strText += strTemp;
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=false align=center [/DRG_FONT]\n\n%s\n[DRG_FONT]|FI|[/DRG_FONT]"), strDate);
	strText += strTemp;

	pSrc.sProductInfo = sProductInfo;
	pSrc.sCaption = _LS(IDS_CD_RPT_COVERPAGE);
	pSrc.sCaption += _LS(IDS_CD_RPT_COVERPAGE);
	pSrc.sContents = strText;
	pSrc.sExt = _T("");
	pSrc.sExtInfo = _LS(IDS_CD_RPT_COVERPAGE);
	pSrc.sFullPath = _T("");
	pSrc.sType = _LSX(TXT);
	CReportCommonFunc::ChangeStructStr2Char(&pSrc, &pTgt);
	CReportCommonFunc::ExportData2Word(&pTgt);
	CReportCommonFunc::ExportInsertPageBreak();
}


void AutoReport_Gen_Base::InsertCatalogPage()
{
	CString strText = _T("");
	strText = _T("");
	CString strTemp = _T("");

	//strTemp.Format("%s\n", _LS(IDS_AMR_COMMON_Control_Data));
	strTemp.Format(_T("[DRG_FONT]|FC| size=16 bold=true align=center[/DRG_FONT] %s \n[DRG_FONT]|FI|[/DRG_FONT]"), _LS(IDS_CD_RPT_CONTENTS));
	strText += strTemp;

	GEN_IFTAG_STR pSrc;
	GEN_IFTAG pTgt;
	CString sProductInfo = _LSX(GEN);


	pSrc.sProductInfo = sProductInfo;
	pSrc.sCaption = _LS(IDS_CD_RPT_CONTENTS);
	pSrc.sContents = strText;
	pSrc.sExt = _T(" ");
	pSrc.sExtInfo = _LS(IDS_CD_RPT_CONTENTS);
	pSrc.sFullPath = _T(" ");
	pSrc.sType = _LSX(TXT);
	CReportCommonFunc::ChangeStructStr2Char(&pSrc, &pTgt);
	CReportCommonFunc::ExportData2Word(&pTgt);
	CReportCommonFunc::ExportInsertCatalog(2);
	CReportCommonFunc::ExportInsertPageBreak();
}

void AutoReport_Gen_Base::Initial()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pDoc->m_pUnitCtrl->GetUnitNameCurrent(m_UnitName);
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(m_UnitIndex);
}
CString AutoReport_Gen_Base::ChangeLine()
{
	return _T("\r\n");
}
CString AutoReport_Gen_Base::ToString(double dVal, int nPos)
{
	CString strText;
	if (nPos > 3)
		strText.Format(_T("%.*g"), nPos, dVal);
	else
		strText.Format(_T("%.*f"), nPos, dVal);
	return strText;
}
CString AutoReport_Gen_Base::ToString(int nVal)
{
	CString strText;
	strText.Format(_T("%d"), nVal);
	return strText;
}
CString AutoReport_Gen_Base::BookMark(LPCTSTR strName, int idx)
{
	CString str = _T("");
	if (idx <= 0)
		str = strName;
	else
		str.Format(_T("%s%d"), strName, idx);
	str.Replace('&', '_');
	return str;
}
CString AutoReport_Gen_Base::CellRltDes(BOOL bOK)
{
	if (bOK)
		return _T("OK");
	else
		return _ULS('[DRG_CELL_PROPERTY]color=16711680[/DRG_CELL_PROPERTY]') + _T("NG");
}
double AutoReport_Gen_Base::CompVal(double dVal1, double dVal2, int nCompType)
{
	if (nCompType == 0)
		return fabs(dVal1) > fabs(dVal2) ? dVal1 : dVal2;
	else if (nCompType == 1)
		return max(dVal1, dVal2);
	else if (nCompType == 2)
		return min(dVal1, dVal2);
	else if (nCompType == 3)
		return fabs(dVal1) < fabs(dVal2) ? dVal1 : dVal2;
	else
	{
		ASSERT(FALSE);
		return 0;
	}
}
int  AutoReport_Gen_Base::ForceDot(LPCTSTR lpCurUnit)
{
	int nPos = 0;
	CString strCurUnit = lpCurUnit;
	if (strCurUnit == D_UNITSYS_FORCE_NAME_KN)
		nPos = 3;
	return nPos;
}
int  AutoReport_Gen_Base::LengthDot(LPCTSTR lpCurUnit)
{
	int nPos = 0;
	CString strCurUnit = lpCurUnit;
	if (strCurUnit == D_UNITSYS_LENGTH_NAME_M)
		nPos = 3;
	else if (strCurUnit == D_UNITSYS_LENGTH_NAME_CM)
		nPos = 1;
	else if (strCurUnit == D_UNITSYS_LENGTH_NAME_MM)
		nPos = 0;
	else
		nPos = 0;
	return nPos;
}
int AutoReport_Gen_Base::Dot(int nUnitType)
{
	CString strLen = m_UnitName.strLength[m_UnitIndex.nBase_Length];
	CString strForce = m_UnitName.strForce[m_UnitIndex.nBase_Force];
	int nDot = 0;
	int nLen = LengthDot(strLen);
	int nForce = ForceDot(strForce);
	if (nUnitType == D_UNITSYS_BASE_LENGTH)
		nDot = nLen;
	else if (nUnitType == D_UNITSYS_BASE_AREA)
		nDot = nLen * 2;
	else if (nUnitType == D_UNITSYS_BASE_DENSITY)
		nDot = min((nForce + fabs(3 - nLen) * 3), 6);
	else if (nUnitType == D_UNITSYS_BASE_STRESS)
		nDot = min((nForce + fabs(3 - nLen) * 2), 6);
	else if (nUnitType == D_UNITSYS_BASE_FORCE)
		nDot = nForce;
	else if (nUnitType == D_UNITSYS_BASE_MOMENT)
		nDot = min((nLen + nForce), 4);
	else
	{
		ASSERT(FALSE);
	}
	return nDot;
}
CString AutoReport_Gen_Base::Length(double dVal, BOOL bAddUnit, int nPos)
{
	CString strText;
	if (nPos >= 0)
		strText = ToString(dVal, nPos);
	else
	{
		nPos = Dot(D_UNITSYS_BASE_LENGTH);
		strText = ToString(dVal, nPos);
	}
	if (bAddUnit)
		strText += UnitDes(D_UNITSYS_BASE_LENGTH);
	return strText;
}

CString AutoReport_Gen_Base::Stress(double dVal, BOOL bAddUnit, int nPos)
{
	CString strText;
	if (nPos >= 0)
		strText = ToString(dVal, nPos);
	else
	{
		nPos = Dot(D_UNITSYS_BASE_STRESS);
		strText = ToString(dVal, nPos);
	}
	if (bAddUnit)
		strText += UnitDes(D_UNITSYS_BASE_STRESS);
	return strText;
}
CString AutoReport_Gen_Base::Density(double dVal, BOOL bAddUnit, int nPos)
{
	CString strText;
	if (nPos >= 0)
		strText = ToString(dVal, nPos);
	else
	{
		nPos = Dot(D_UNITSYS_BASE_DENSITY);
		strText = ToString(dVal, nPos);
	}
	if (bAddUnit)
		strText += UnitDes(D_UNITSYS_BASE_DENSITY);
	return strText;
}
CString AutoReport_Gen_Base::Area(double dVal, BOOL bAddUnit, int nPos)
{
	CString strText;
	if (nPos >= 0)
		strText = ToString(dVal, nPos);
	else
	{
		nPos = Dot(D_UNITSYS_BASE_AREA);
		strText = ToString(dVal, nPos);
	}
	if (bAddUnit)
		strText += UnitDes(D_UNITSYS_BASE_AREA);
	return strText;
}
CString AutoReport_Gen_Base::Force(double dVal, BOOL bAddUnit, int nPos)
{
	CString strText;
	if (nPos >= 0)
		strText = ToString(dVal, nPos);
	else
	{
		nPos = Dot(D_UNITSYS_BASE_FORCE);
		strText = ToString(dVal, nPos);
	}
	if (bAddUnit)
		strText += UnitDes(D_UNITSYS_BASE_FORCE);
	return strText;
}
CString AutoReport_Gen_Base::Moment(double dVal, BOOL bAddUnit, int nPos)
{
	CString strText;
	if (nPos >= 0)
		strText = ToString(dVal, nPos);
	else
	{
		nPos = Dot(D_UNITSYS_BASE_MOMENT);
		strText = ToString(dVal, nPos);
	}
	if (bAddUnit)
		strText += UnitDes(D_UNITSYS_BASE_MOMENT);
	return strText;
}
CString AutoReport_Gen_Base::UnitDes(int nUnitType, BOOL bAddBrace)
{
	CString strLen = m_UnitName.strLength[m_UnitIndex.nBase_Length];
	CString strFor = m_UnitName.strForce[m_UnitIndex.nBase_Force];
	CString strDes = _T("");
	if (nUnitType == D_UNITSYS_BASE_LENGTH)
		strDes = strLen;
	else if (nUnitType == D_UNITSYS_BASE_AREA)
		strDes.Format(_T("%s^2"), strLen);
	else if (nUnitType == D_UNITSYS_BASE_DENSITY)
		strDes.Format(_T("%s/%s^3"), strFor, strLen);
	else if (nUnitType == D_UNITSYS_BASE_STRESS)
		strDes.Format(_T("%s/%s^2"), strFor, strLen);
	else if (nUnitType == D_UNITSYS_BASE_FORCE)
		strDes = strFor;
	else if (nUnitType == D_UNITSYS_BASE_MOMENT)
		strDes = strFor + _T("*") + strLen;
	else
	{
		ASSERT(FALSE);
	}
	if (bAddBrace)
		strDes = _T("(") + strDes + _T(")");
	return strDes;
}
BOOL AutoReport_Gen_Base::AutoGeneration(BOOL bUpDate)
{
	m_iAppendFlag = bUpDate ? D_WORD_APPEND_REGENERATE_CH : D_WORD_APPEND_INSERT;
	Initial();
	if (bUpDate == FALSE)
	{
		InsertCoverPage();
		InsertCatalogPage();
	}
	//
	MakeContent();
	//
	CReportCommonFunc::ExportInsertPageNumber(1, 1, true);
	CReportCommonFunc::ExportUpdateCatalog();
	return TRUE;
}

void AutoReport_Gen_Base::InsertSegmentElements(GenSegmentElements& segs, BOOL bDeleteTempFile)
{
	CString  sProductInfo = _LSX(GEN);
	for (auto pitSeg = segs.m_SegmentElements.begin(); pitSeg != segs.m_SegmentElements.end(); ++pitSeg)
	{
		CH_GEN_IFTAG_STR& Iftag_str = *pitSeg;
		Iftag_str.sProductInfo = sProductInfo;
		Iftag_str.iAppendFlag = m_iAppendFlag;
		GEN_IFTAG Iftag;
		CReportCommonFunc::ChangeStructStr2Char(&Iftag_str, &Iftag);
		if (m_iAppendFlag == D_WORD_APPEND_INSERT)
		{
			CReportCommonFunc::ExportData2Word_ByStyle(&Iftag, Iftag_str.iLevel);
		}
		else
			CReportCommonFunc::ExportData2Word(&Iftag);
	}
	if (bDeleteTempFile)
		DeleteTempFiles(segs);
}

void AutoReport_Gen_Base::InsertReportItemSSTable(GenSegmentElements &Elems)
{
	CArray<T_SECT_K, T_SECT_K> aSectK;
	T_SECT_D SectD;
	CString strImagePath = _T("");
	CString strFilePath = _T("");
	int nType = 0;  // section type
	T_RSCT_STIFF_D StiffInfo, AfterStiffInfo;
	CStringArray arImagePath;
	CDgnAmrSectionTb SectionTb;
	m_pDoc->m_pAttrCtrl->GetSectKeyListUsedInElem(aSectK);
	CH_GEN_IFTAG_STR Iftag;
	for (int i = 0; i < aSectK.GetSize(); i++)
	{

		StiffInfo.Initialize();
		AfterStiffInfo.Initialize();
		strImagePath = _T("");
		nType = 0;
		arImagePath.RemoveAll();
		if (!m_pDoc->m_pAttrCtrl->GetSect(aSectK[i], SectD))
			continue;
		if (!m_pDoc->m_pAttrCtrl->GetReportSectStiffInfo(SectD, StiffInfo, &AfterStiffInfo))
			continue;
		nType = StiffInfo.nType;
		if (nType < 1 || nType>7)
		{
			ASSERT(0); continue;
		}
		// set file path name
		CString strFilePathName = _T("");
		//strFilePathName = GetModelFullPathName();
		CString strSName = _T("");
		strSName = SectD.SName;
		//FilterFileName(strSName);
		strImagePath.Format(_T("%s%s_%d.emf"), strFilePathName, strSName, aSectK[i]);
		strFilePath.Format(_T("%s%s_%d.xml"), strFilePathName, strSName, aSectK[i]);

		// make section image
		TreeReportCtrl()->MakeSectionFigure(strImagePath, aSectK[i], nType, arImagePath);
		if (!SectionTb.GenerateSectionTable(strFilePath, arImagePath, D_SECTION_TB_SS, aSectK[i], 0))
			continue;
		CString strTemp = _T("");
		strTemp.Format(_T("%d : %s"), aSectK[i], SectD.SName);
		Iftag.sProductInfo = _LSX(GEN);
		Iftag.sCaption = strTemp;
		Iftag.sType = _LSX(TBL);
		Iftag.sFullPath = strFilePath;
		Iftag.sExtInfo = _ULS('SS ') + strTemp;
		Iftag.sContents = _T("");
		Iftag.sExt = _T("");
		Iftag.iAppendFlag = D_WORD_APPEND_INSERT;

		Elems.m_SegmentElements.push_back(Iftag);
	}
}

void AutoReport_Gen_Base::DeleteTempFiles(const GenSegmentElements& segs)
{
	CFileFind file;
	for (std::vector<CH_GEN_IFTAG_STR>::const_iterator pit1 = segs.m_SegmentElements.begin();
		pit1 != segs.m_SegmentElements.end(); ++pit1)
	{
		if (file.FindFile(pit1->sFullPath))
			::DeleteFile(pit1->sFullPath);
	}

	file.Close();
}

void AutoReport_Gen_Base::MakeTitle(GenSegmentElements &genElements, LPCTSTR lpTitle, int iChapterID, int iSegmentID, LPCTSTR lpBookMark, int iSubId /*= 0*/, int iSubSubId  /* = 0 */)
{
	CString strTitleIdx = _T("");
	int iLevel = 0;
	if (iChapterID == 0 && iSegmentID != 0)
	{
		strTitleIdx.Format(_T("%d) "), iSegmentID);
		iLevel = 2;
	}
	else if (iChapterID != 0 && iSegmentID == 0)
	{
		strTitleIdx.Format(_T("%d. "), iChapterID);
		iLevel = 1;
	}
	else if (iChapterID != 0 && iSegmentID != 0)
	{
		strTitleIdx.Format(_T("%d.%d   "), iChapterID, iSegmentID);
		iLevel = 2;
	}
	if (iSubId != 0)
	{
		if (iSubSubId != 0)
		{
			strTitleIdx.Format(_T("%d.%d.%d.%d"), iChapterID, iSegmentID, iSubId, iSubSubId);
			iLevel = 4;
		}
		else
		{
			strTitleIdx.Format(_T("%d.%d.%d"), iChapterID, iSegmentID, iSubId);
			iLevel = 3;
		}
	}
	CString strContents = _T("");
	if (iLevel == 1)
		strContents.Format(_T("[DRG_FONT]|FC| size=14 bold=false align=left[/DRG_FONT] %s [DRG_FONT]|FI|[/DRG_FONT]"), strTitleIdx + lpTitle);
	else
		strContents.Format(_T("[DRG_FONT]|FC| size=15 bold=false align=left[/DRG_FONT] %s [DRG_FONT]|FI|[/DRG_FONT]"), strTitleIdx + lpTitle);

	genElements.m_strSegmentName = lpTitle;
	CString strBookMark = lpBookMark;
	if (strBookMark.GetLength() < 1)
		strBookMark = lpTitle;
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = lpTitle;
	genFtagStr.sType = _LSX(TXT);
	genFtagStr.sFullPath = _T(" ");
	genFtagStr.sExtInfo = strBookMark;
	genFtagStr.sContents = strContents;
	genFtagStr.iAppendFlag = D_WORD_APPEND_INSERT;
	genFtagStr.sExt = _T(" ");
	genFtagStr.iLevel = iLevel;
	genElements.m_SegmentElements.push_back(genFtagStr);
}

void AutoReport_Gen_Base::MakeText(GenSegmentElements &genElements, LPCTSTR lpText, LPCTSTR lpLog /* = _T("") */)
{
	CH_GEN_IFTAG_STR genFtagStr;
	CString strContents;
	strContents.Format(_T("[DRG_FONT]|FC| size=11 bold=false align=left[/DRG_FONT]%s[DRG_FONT]|FI|[/DRG_FONT]"), lpText);
	genFtagStr.sContents = strContents;
	genFtagStr.sType = _LSX(TXT);
	genFtagStr.sFullPath = _T(" ");
	genFtagStr.sExtInfo = lpLog;
	genFtagStr.iAppendFlag = D_WORD_APPEND_INSERT;
	genFtagStr.sExt = _T(" ");
	genFtagStr.iLevel = 0;
	genElements.m_SegmentElements.push_back(genFtagStr);
}
void AutoReport_Gen_Base::MakeTable(GenSegmentElements &genElements, T_TABLE_D &TableD, LPCTSTR lpCaption, LPCTSTR lpBookMark)
{
	if (TableD.RecordD.nRecordRowCount <= 0)
		return;
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = lpCaption;
	genFtagStr.sType = _LSX(TBL);
	CReportTableProcess ReportTable;
	ReportTable.SetData(TableD);
	ReportTable.SetFilePathName(ReportTable.GetFilePathName() + genFtagStr.sCaption + _T(".xml"));
	ReportTable.GenerateReport();

	genFtagStr.sFullPath = ReportTable.GetFilePathName() + genFtagStr.sCaption + _T(".xml");
	genFtagStr.sExtInfo = lpBookMark == nullptr ? genFtagStr.sCaption : lpBookMark;
	genFtagStr.sContents = _T(" ");
	genFtagStr.iAppendFlag = D_WORD_APPEND_INSERT;
	genFtagStr.sExt = _T(" ");
	genFtagStr.iLevel = 0;
	genElements.m_SegmentElements.push_back(genFtagStr);
}
void AutoReport_Gen_Base::MakeTableCenterText(GenSegmentElements &genElements, T_TABLE_D &TableD, LPCTSTR lpCaption, LPCTSTR lpBookMark)
{
	if (TableD.RecordD.nRecordRowCount <= 0)
		return;
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = lpCaption;
	genFtagStr.sType = _LSX(TBL);
	CReportTableProcess ReportTable;
	CString strFormat = _T("[DRG_CELL_PROPERTY]color=0 align=center shape=16777215 tbwidth=10%%[/DRG_CELL_PROPERTY]");
	for (int i = 0; i < TableD.RecordD.nRecordRowCount; i++)
	{
		CStringArray& aString = TableD.RecordD.aRecordData[i];
		for (int j = 0; j < aString.GetSize(); j++)
			aString[j] = strFormat + aString[j];
	}
	ReportTable.SetData(TableD);
	ReportTable.SetFilePathName(ReportTable.GetFilePathName() + genFtagStr.sCaption + _T(".xml"));
	ReportTable.GenerateReport();

	//strContents = _T("[DRG_FONT]|FC| size=11 bold=false align=center[/DRG_FONT]");
	//genFtagStrTile.sContents = strContents;

	genFtagStr.sFullPath = ReportTable.GetFilePathName() + genFtagStr.sCaption + _T(".xml");
	genFtagStr.sExtInfo = lpBookMark == nullptr ? genFtagStr.sCaption : lpBookMark;
	genFtagStr.sContents = _T("");
	genFtagStr.iAppendFlag = D_WORD_APPEND_INSERT;
	genFtagStr.sExt = _T(" ");
	genFtagStr.iLevel = 0;
	genElements.m_SegmentElements.push_back(genFtagStr);
}


void AutoReport_Gen_Base::MakePicture(GenSegmentElements &genElements, LPCTSTR lpPicName, LPCTSTR lpBookMark)
{
	CString strPath = GetImagePath(lpPicName);
	CFileFind file;
	if (file.FindFile(strPath) == FALSE)
	{
		ASSERT(FALSE);
		return;
	}
	file.Close();
	//
	CH_GEN_IFTAG_STR genFtagStr;
	genFtagStr.sCaption = lpPicName;
	genFtagStr.sType = _LSX(IMG);
	genFtagStr.sFullPath = strPath;
	genFtagStr.sExtInfo = lpBookMark;
	genFtagStr.sContents = lpPicName;
	genFtagStr.iAppendFlag = D_WORD_APPEND_INSERT;
	genFtagStr.sExt = _T("");
	genFtagStr.iLevel = 0;
	genElements.m_SegmentElements.push_back(genFtagStr);
}

void AutoReport_Gen_Base::styleSetUp(LPCTSTR sStype)
{
	// Make structure to export
	CString	sProductInfo = _T(" "); // 제품정보
	GEN_IFTAG* pIftag = NULL;
	pIftag = new GEN_IFTAG[1];
	GEN_IFTAG_STR* pIftagStr = NULL;
	pIftagStr = new GEN_IFTAG_STR[1];

#if defined(_CIVIL)
	sProductInfo = _LSX(CIVIL);
#elif(_MGEN)
	sProductInfo = _LSX(GEN);
#else
	ASSERT(0);
	sProductInfo = _T(" ");
#endif

	pIftagStr[0].sCaption = _T(" ");
	pIftagStr[0].sType = _LSX(SET);
	pIftagStr[0].sFullPath = _T(" ");
	pIftagStr[0].sExtInfo = _T(" ");
	pIftagStr[0].sContents = sStype;
	pIftagStr[0].iAppendFlag = D_WORD_APPEND_INSERT;
	pIftagStr[0].sExt = _T(" ");

	CReportCommonFunc::ChangeStructStr2Char(pIftagStr, pIftag, 1);
	CReportCommonFunc::ExportData2Word(pIftag, 1);

	delete[]pIftagStr;
	delete[]pIftag;
}
// Inset Image Items
CString AutoReport_Gen_Base::GetImagePath(LPCTSTR strName)
{
	CString strFilePath;

	CString strFilePathName = _T("");
	CString strFullPath = _T("");
	::GetTempPath(_MAX_PATH, strFullPath.GetBuffer(_MAX_PATH)); // 임시폴더에다 저장함. 20100429 KIMJM
	strFullPath.ReleaseBuffer();
	CFileCtrl FileCtrl(strFullPath);
	strFilePathName = FileCtrl.GetFilePathAndNameWithoutExtension();
	strFilePath.Format(_T("%s%s.bmp"), strFilePathName, strName);
	return strFilePath;
}


BOOL AutoReport_Gen_Base::MakeImage_Current(LPCTSTR lpPicName, BOOL bBW)
{
	CString strFilePath = GetImagePath(lpPicName);
	if (!CGPSAMRUtil::GetInstance()->MakeAMRFigureImg(strFilePath, bBW))
	{
		ASSERT(FALSE);
		return FALSE;
	}
	return TRUE;
}

void AutoReport_Gen_Base::SetShape(BOOL bShape)
{
	GRenderView* pRenderView = (GRenderView*)(I_GENModelBase::GetCurMySelfST()->GetOwnerWnd());
	pRenderView->SetShape(bShape);
}
void AutoReport_Gen_Base::SetViewPoint(int nFlag)
{
	GRenderView* pRenderView = (GRenderView*)(I_GENModelBase::GetCurMySelfST()->GetOwnerWnd());
	if (nFlag == 0)
		pRenderView->SetViewPointISO();
	else if (nFlag == 1)
		pRenderView->SetViewPointMx(); // left
	else if (nFlag == 2)
		pRenderView->SetViewPointMy();// front
	else if (nFlag == 3)
		pRenderView->SetViewPointMz(); // top
	else if (nFlag == 4)
		pRenderView->SetViewPointPx();
	else if (nFlag == 5)
		pRenderView->SetViewPointPy();
	else if (nFlag == 6)
		pRenderView->SetViewPointPz();
}
void AutoReport_Gen_Base::SetViewPoint(double xyzNormal[3])
{
	GRenderView* pRenderView = (GRenderView*)(I_GENModelBase::GetCurMySelfST()->GetOwnerWnd());
	pRenderView->SetViewPoint(xyzNormal[0], xyzNormal[1], xyzNormal[2]);
}
void AutoReport_Gen_Base::SetPreOrPostMode(BOOL bPost)
{
	if (m_pDoc->IsPostMode() == bPost)
		return;
	if (bPost)
		m_pDoc->SetModePost();
	else
		m_pDoc->SetModePre();
}
void AutoReport_Gen_Base::ActiveObj(const T_KEY_LIST &Elems, BOOL bActive, const T_KEY_LIST*lsNodes)
{
	CArray<unsigned int, unsigned int>  EKeyArray;
	EKeyArray.Copy(Elems);
	CArray<unsigned int, unsigned int>  NKeyArray;
	if (lsNodes != nullptr)
		NKeyArray.Copy(*lsNodes);
	if (bActive)
		I_GENModelBase::GetCurMySelfST()->ActiveThisObjects(NKeyArray, EKeyArray);
	else
		I_GENModelBase::GetCurMySelfST()->InActiveThisObjects(NKeyArray, EKeyArray);
}
int AutoReport_Gen_Base::FilteElemByType(T_KEY_LIST &lstElems, int nType)
{
	T_ELEM_D ElemD;
	int nSize = lstElems.GetCount() - 1;
	for (int i = nSize; i >= 0; i--)
	{
		if (!m_pDoc->m_pAttrCtrl->GetElem(lstElems[i], ElemD))
		{
			lstElems.RemoveAt(i);
			continue;
		}
		if (ElemD.eltyp != nType)
		{
			lstElems.RemoveAt(i);
			continue;
		}
	}
	return lstElems.GetCount();
}
int  AutoReport_Gen_Base::FilteElemByMembType(T_KEY_LIST &lstElems, int nType  /*D_MBTP_BEAM*/)
{
	int nSize = lstElems.GetCount() - 1;
	for (int i = nSize; i >= 0; i--)
	{
		if (m_pDoc->m_pAttrCtrl->GetMemberType(lstElems[i]) != nType)
		{
			lstElems.RemoveAt(i);
			continue;
		}
	}
	return lstElems.GetCount();
}
void AutoReport_Gen_Base::AdjustSlabView(T_KEY_LIST& lstElems)
{
	double xyzNormal[3];
	if (!CalcSlabVector(xyzNormal, lstElems))
		SetViewPoint(0);
	else
		SetViewPoint(xyzNormal);
}
BOOL AutoReport_Gen_Base::IsVaildLoadCom(T_KEY key, int nType)
{
	T_MLCB_D MlcbD; MlcbD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->Get_DefaultMlcb(MlcbD))
		MlcbD.Initialize();
	if (nType == 0)
	{
		for (int i = 0; i < MlcbD.aSlabStreK.GetCount(); i++)
		{
			if (MlcbD.aSlabStreK[i] == key)
			{
				return TRUE;
			}
		}
	}
	else if (nType == 1)
	{
		for (int i = 0; i < MlcbD.aSlabServK.GetCount(); i++)
		{
			if (MlcbD.aSlabStreK[i] == key)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}
BOOL AutoReport_Gen_Base::CalcSlabVector(double xyz[3], T_KEY_LIST &lstElems)
{
	T_ELEM_D ElemD;
	T_NODE_D NodeD[3];
	int nSize = lstElems.GetCount() - 1;
	for (int i = nSize; i >= 0; i--)
	{
		m_pDoc->m_pAttrCtrl->GetElem(lstElems[i], ElemD);
		if (ElemD.eltyp != PLATE_EL)
		{
			lstElems.RemoveAt(i);
			continue;
		}
	}
	if (lstElems.IsEmpty())
		return FALSE;
	//
	double xyz1[3], xyz2[3], xyzout[3];
	auto Calcxyz = [&]()
	{
		xyz1[0] = NodeD[0].x - NodeD[1].x;
		xyz1[1] = NodeD[0].y - NodeD[1].y;
		xyz1[2] = NodeD[0].z - NodeD[1].z;

		xyz2[0] = NodeD[0].x - NodeD[2].x;
		xyz2[1] = NodeD[0].y - NodeD[2].y;
		xyz2[2] = NodeD[0].z - NodeD[2].z;
	};
	//

	m_pDoc->m_pAttrCtrl->GetElem(lstElems[0], ElemD);
	m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD[0]);
	m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD[1]);
	m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[2], NodeD[2]);
	//xyz1 = NodeD[0] - NodeD[1];xyz2 = NodeD[0] - NodeD[2];
	Calcxyz();
	CMathFunc::mathCross(xyz1, xyz2, xyz);
	CMathFunc::mathNormalize(xyz, xyz);
	for (int i = 1; i < lstElems.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetElem(lstElems[i], ElemD);
		m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD[0]);
		m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD[1]);
		m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[2], NodeD[2]);
		//xyz1 = NodeD[0] - NodeD[1];xyz2 = NodeD[0] - NodeD[2];
		Calcxyz();
		CMathFunc::mathCross(xyz1, xyz2, xyzout);
		CMathFunc::mathNormalize(xyzout, xyzout);
		if (fabsf(CMathFunc::mathDot(xyzout, xyz)) < 1 - 1e-9)
			return FALSE;
	}
	return TRUE;
}

void AutoReport_Gen_Base::ActiveElemAll(BOOL bActive)
{
	T_KEY_LIST  EKeyArray;
	T_KEY_LIST  NKeyArray;
	m_pDoc->m_pAttrCtrl->GetElemKeyList(EKeyArray);
	ActiveObj(EKeyArray, bActive, &NKeyArray);
}
void AutoReport_Gen_Base::DrawNodeAll(BOOL bActive)
{
	GRenderView* pRenderView = (GRenderView*)(I_GENModelBase::GetCurMySelfST()->GetOwnerWnd());
	pRenderView->DrawNode(bActive, TRUE);
}
void AutoReport_Gen_Base::AddToText(CString &strText, LPCTSTR Fmt, ...)
{
	va_list argptr;
	va_start(argptr, Fmt);
	CString strTemp;
	strTemp.FormatV(Fmt, argptr);
	va_end(argptr);
	strText += strTemp;
}
CString AutoReport_Gen_Base::FillBlank(LPCTSTR str, int nTolLength, int nFillFlag)
{
	CString strText = str;
	int nBlank = nTolLength - strText.GetLength();
	if (nBlank <= 0)
		return strText;
	if (nFillFlag == 0)
	{
		for (int i = 0; i < nFillFlag; i++)
		{
			strText += _T(" ");
		}
	}
	else if (nFillFlag == 1)
	{
		int nBlankLeft = nBlank / 2;
		int nBlankRaigth = nBlank - nBlankLeft;
		for (int i = 0; i < nBlankRaigth; i++)
		{
			strText += _T(" ");
		}
		for (int i = 0; i < nBlankLeft; i++)
		{
			strText = _T(" ") + strText;
		}
	}
	else
	{
		for (int i = 0; i < nFillFlag; i++)
		{
			strText = _T(" ") + strText;
		}
	}
	return strText;
}
void  AutoReport_Gen_Base::MergeCellCol(T_TABLE_D &TableD, int nCol, int nRow, int nNum)
{
	T_MERGE_UNIT_D merge;
	merge.nMergeCount = nNum;
	merge.nStartColNumber = nCol;
	merge.nStartRowNumber = nRow;
	TableD.RecordD.aMergeColD.Add(merge);
}
void  AutoReport_Gen_Base::MergeCellRow(T_TABLE_D &TableD, int nCol, int nRow, int nNum)
{
	T_MERGE_UNIT_D merge;
	merge.nMergeCount = nNum;
	merge.nStartColNumber = nCol;
	merge.nStartRowNumber = nRow;
	TableD.RecordD.aMergeRowD.Add(merge);
}
void  AutoReport_Gen_Base::MergeHeadCol(T_TABLE_D &TableD, int nCol, int nRow, int nNum)
{
	T_MERGE_UNIT_D merge;
	merge.nMergeCount = nNum;
	merge.nStartColNumber = nCol;
	merge.nStartRowNumber = nRow;
	TableD.HeaderD.aMergeColD.Add(merge);
}
void  AutoReport_Gen_Base::MergeHeadRow(T_TABLE_D &TableD, int nCol, int nRow, int nNum)
{
	T_MERGE_UNIT_D merge;
	merge.nMergeCount = nNum;
	merge.nStartColNumber = nCol;
	merge.nStartRowNumber = nRow;
	TableD.HeaderD.aMergeRowD.Add(merge);
}
void AutoReport_Gen_Base::MakeAlignText(CString& strContent, CString&& str1, CString&& str2, int n /* = 40 */)
{
	int nLength = str1.GetLength();
	std::string arrStrTmp = CW2A(str1);
	int byteCount = arrStrTmp.length();

	if (byteCount > n)
	{
		n = byteCount + 2;
	}
	else
		n -= byteCount - nLength;

	CString strRlt, st;
	st.Format(_T("    %%-%ds"), n);
	strRlt.Format(st, str1);
	//int nsize = strRlt.GetLength();
	strContent += strRlt + str2 + _T("\n");
}

CString AutoReport_Gen_Base::LoadCombTypeDes(UINT nLcomType, int nType)
{
	if (nLcomType == D_LCOMTYPE_GENERAL)
	{
		if (nType == 0)
			return _T("Add");
		else if (nType == 1)
			return _T("Envelope");
		else if (nType == 2)
			return _T("Abs");
		else if (nType == 3)
			return _T("SRSS");
	}
	else
	{
		if (nType == 0)
			return _T("Add");
		else if (nType == 1)
			return _T("Envelope");
		else if (nType == 2)
			return _T("SRSS");
	}
	return _T("");
}
CString AutoReport_Gen_Base::LoadCombActiveDes(UINT nLcomType, int nActive)
{
	CString  strActive = _T("");
	CString  IDS_CMD_LCOM_INACTIVE = _T("Inactive");
	CString  IDS_CMD_LCOM_ACTIVE = _T("Active");
	CString  IDS_CMD_LCOM_STRENGTH = _T("Strength/Stress");
	CString  IDS_CMD_LCOM_SERVICE = _T("Serviceability");
	CString  IDS_CMD_LCOM_SPECIAL = _T("Special");
	CString  IDS_CMD_LCOM_VERTICAL = _T("Vertical");
	if (nLcomType == D_LCOMTYPE_STEEL || nLcomType == D_LCOMTYPE_CONCRETE)
	{
		if (nActive == 0)      strActive = IDS_CMD_LCOM_INACTIVE;
		else if (nActive == 1) strActive = IDS_CMD_LCOM_STRENGTH;
		else if (nActive == 2) strActive = IDS_CMD_LCOM_SERVICE;
		else if (nActive == 3) strActive = IDS_CMD_LCOM_SPECIAL;
		else if (nActive == 4) strActive = IDS_CMD_LCOM_VERTICAL;
	}
	else
	{
		if (nActive == 0)
			strActive = IDS_CMD_LCOM_INACTIVE;
		else if (nActive == 1)
			strActive = IDS_CMD_LCOM_ACTIVE;
	}
	return strActive;
}

BOOL AutoReport_Gen_Base::MakeLoadComb(const Report_Key &LoadK, T_LCOM_D &LComD)
{
	if (!m_pDoc->m_pPostCtrl->MakeLoadComb(LoadK.m_nType, LoadK.m_nKey, LComD))
	{
		ASSERT(FALSE);
		return FALSE;
	}
	return TRUE;
}

//xuezc
CString AutoReport_Gen_Base::GetBold(const CString& str)
{
	CString strText;
	strText.Format(_T("[DRG_FONT]|FC| size=11 bold=true align=left [/DRG_FONT]%s[DRG_FONT]|FC| size=11 bold=false [/DRG_FONT]"), str);
	return strText;
}
CString AutoReport_Gen_Base::GetBold(int nValue)
{
	CString strText;
	strText.Format(_T("[DRG_FONT]|FC| size=11 bold=true align=left [/DRG_FONT]%d[DRG_FONT]|FC| size=11 bold=false [/DRG_FONT]"), nValue);
	return strText;
}
CString AutoReport_Gen_Base::GetBold(double dValue, int n /* = 2 */)
{
	CString strText;
	strText.Format(_T("[DRG_FONT]|FC| size=11 bold=true align=left [/DRG_FONT]%.2f[DRG_FONT]|FC| size=11 bold=false [/DRG_FONT]"), dValue);
	return strText;
}
void AutoReport_Gen_Base::Replace_StrPath(CString& str)
{
	str.Replace(_T(":"), _T("_"));
	str.Replace(_T("/"), _T("_"));
}
CString AutoReport_Gen_Base::Double2String_g(double dValue, int n)
{
	CString strTemp;
	double dTemp = CMathFunc::mathRoundOff(dValue, n);
	strTemp.Format(_T("%g"), dTemp);
	return strTemp;
}
CString AutoReport_Gen_Base::Double2String_f(double dValue, int n)
{
	CString strTemp,str;
	str.Format(_T("%%.%df"),n);
	strTemp.Format(str, dValue);
	return strTemp;
}

void AutoReport_Gen_Base::MakeCompleteText(CString& Elems, const CString& str1, int n /* = 4 */)
{
	Elems += str1 + _T("\n");
}
void AutoReport_Gen_Base::PushDspOpt()
{
	//
	GDisplaySetting dspOpt;
	ExportDisplaySetting::GET(&dspOpt);
	m_dspOpt = dspOpt;
	dspOpt.m_Node.SetDefault();
	dspOpt.m_Elem.SetDefault();
	dspOpt.m_Prop.SetDefault();
	dspOpt.m_Bndr.SetDefault();
	dspOpt.m_Load.SetDefault();
	dspOpt.m_View.SetDefault();
	//dspOpt.m_Seis.SetDefault();
	//dspOpt.m_Factor.SetDefault();
	//dspOpt.m_TextLabel.SetDefault();
	//dspOpt.m_Color.SetDefault();
	//dspOpt.m_Hidden.SetDefault();
	//dspOpt.m_Etc.SetDefault();
	//dspOpt.m_Print.SetDefault();
	//dspOpt.m_Dgn.SetDefault();

	dspOpt.m_Node.bNode = FALSE;
	dspOpt.m_View.bUCS = FALSE;
	dspOpt.m_View.bVPoint = FALSE;
	dspOpt.m_View.bViewCtrl = FALSE;
	//COLORREF bc1 = dspOpt.m_Color.m_ViewColor.m_MV_BackColor;
	//COLORREF bc2 = dspOpt.m_Color.m_ViewColor.m_MV_BackColor2;
	dspOpt.m_Color.m_ViewColor.m_MV_BackColor = RGB(255, 255, 255);
	dspOpt.m_Color.m_ViewColor.m_MV_BackColor2 = RGB(255, 255, 255);
	ExportDisplaySetting::SET(&dspOpt);

}
void AutoReport_Gen_Base::PopDspOpt()
{
	ExportDisplaySetting::SET(&m_dspOpt);
}