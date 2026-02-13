// DgnAmrSectionTb.cpp: implementation of the CDgnAmrSectionTb class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DgnAmrSectionTb.h"
 // for Message

#include "..\wg_xl\ReportTableProcess.h" // for AMR table test using new XML document, 20090914 KIMJM

#include "..\wg_db\wg_db_FileCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
//#include "..\wg_db\wg_db_DBDoc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnAmrSectionTb::CDgnAmrSectionTb()
{
	m_StiffInfo.Initialize();
	m_AfterStiffInfo.Initialize();

	m_RpscInfoI.Initialize();
	m_RpscInfoJ.Initialize();

	m_aCscsInfo.RemoveAll();

	m_strArea=_T("");
	m_strLeng=_T("");
	m_strStif=_T("");
	m_strArea2=_T("");
	m_strLeng2=_T("");
	m_strStif2=_T("");
	CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_AREA  , m_strArea2);
	CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, m_strLeng2);
	CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_STIF  , m_strStif2);
//   m_strArea = _T("(")+m_strArea2+_T(")");
//   m_strLeng = _T("(")+m_strLeng2+_T(")");
//   m_strStif = _T("(")+m_strStif2+_T(")");

	m_strArea.Format(_T("([DRG_MATH]|SB|Sup(%s, 2)[/DRG_MATH])"), m_strLeng2);
	m_strLeng = _T("(")+m_strLeng2+_T(")");
	m_strStif.Format(_T("([DRG_MATH]|SB|Sup(%s, 4)[/DRG_MATH])"), m_strLeng2);
	
	m_nDecimal = AfxGetApp()->GetProfileInt(_LSX(DynaGen Report\\Table), _LSX(Special Decimal), 3);
	m_bExp     = AfxGetApp()->GetProfileInt(_LSX(DynaGen Report\\Table), _LSX(Special Exp), 0);
}

CDgnAmrSectionTb::~CDgnAmrSectionTb()
{
}

// void CDgnAmrSectionTb::SetSectTbData(UINT SectK)
// {
//   CDBDoc* pDoc = CDBDoc::GetDocPoint(); 
//   ASSERT(pDoc);
// 
//   m_StiffInfo.Initialize();
//   m_AfterStiffInfo.Initialize();
// 
//   if(!pDoc->m_pAttrCtrl->GetReportSectStiffInfo(SectK, m_StiffInfo, &m_AfterStiffInfo))
//     return;
// }

BOOL CDgnAmrSectionTb::GenerateSectionTable(CString strFilePathName, CStringArray& arImagePath, int nTableType, UINT SectK, UINT StagK)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); 
	ASSERT(pDoc);
	
	m_StiffInfo.Initialize();
	m_AfterStiffInfo.Initialize();

	m_RpscInfoI.Initialize();
	m_RpscInfoJ.Initialize();

	m_aCscsInfo.RemoveAll();
	
	BOOL bConsiderJ = FALSE;
	int nType=0;

	switch(nTableType)
	{
	case D_SECTION_TB_SS: // Section Summary
		if(pDoc->m_pAttrCtrl->GetReportSectStiffInfo(SectK, m_StiffInfo, &m_AfterStiffInfo))
			return GenerateSectionSum(strFilePathName, arImagePath);
		break;
	case D_SECTION_TB_RS: // Reinforcement of Section
		if(pDoc->m_pAttrCtrl->GetReportRpscInfo(SectK, bConsiderJ, m_RpscInfoI, m_RpscInfoJ))
			return GenerateSectionRein(strFilePathName, arImagePath, bConsiderJ);
		break;
	case D_SECTION_TB_CS: // Composite Section for C.S.
		nType = pDoc->m_pAttrCtrl->GetReportCscsInfo(SectK, StagK, m_aCscsInfo);
		if(nType==1 || nType==2)
			return GenerateSectionComp(strFilePathName, arImagePath, nType, SectK);
		break;
	default:
		ASSERT(0);
		break;
	}

	return FALSE;
}

BOOL CDgnAmrSectionTb::GenerateSectionSum(CString strFilePathName, CStringArray& arImagePath)
{
	int nType=0;
	nType = m_StiffInfo.nType;

	if(nType==1 || nType==3)
		return GenerateType1(strFilePathName, arImagePath);
	else if(nType==2)
		return GenerateType2(strFilePathName);
	else if(nType==4)
		return GenerateType4(strFilePathName, arImagePath);
	else if(nType==5)
		return GenerateType5(strFilePathName, arImagePath);
	else if(nType==6)
		return GenerateType6(strFilePathName, arImagePath);
	else if(nType==7)
		return GenerateType7(strFilePathName, arImagePath);
	else
		ASSERT(0);

	return FALSE;
}

CString CDgnAmrSectionTb::Format_StringDouble(CString csStr, double dValue)
{
	CString strResult, strFormat, strValue;

	if( m_bExp )
	{ // %e
		strFormat.Format(_T("%%.%de"), m_nDecimal);
		strValue.Format(strFormat, dValue);
		strResult = csStr + strValue;
	}
	else
	{ // %f
		strFormat.Format(_T("%%.%df"), m_nDecimal);
		strValue.Format(strFormat, dValue);
		strResult = csStr + strValue;
	}

	return strResult;
}

BOOL CDgnAmrSectionTb::GenerateType1(CString strFilePathName, CStringArray& arImagePath)
{
	if(m_StiffInfo.nType!=1 && m_StiffInfo.nType!=3)
		return FALSE;

	int nImageCnt = arImagePath.GetSize();
	if(nImageCnt<=0)
		return FALSE;

	CReportTableProcess ReportTB;
	
	T_TABLE_D TableD;
	TableD.Initialize();
	
	// TableD에 데이터 채우기
	// header
	TableD.HeaderD.bRepeatHeader = FALSE;
	TableD.HeaderD.nHeaderRowCount = 1;
	TableD.HeaderD.nHeaderColCount = 5;
	
	T_MERGE_UNIT_D MegerD;
	MegerD.nStartColNumber = 1;
	MegerD.nStartRowNumber = 1;
	MegerD.nMergeCount = 5;
	
	TableD.HeaderD.aMergeColD.Add(MegerD);
	
	CString strImagePath = arImagePath[0];
	CString strImage=_T("");
	strImage.Format(_ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=16777215 distwid=1[/DRG_CELL_PROPERTY][DRG_IMAGE|width=%d|height=%d]%s[/DRG_IMAGE]'), D_IMAGE_WIDTH, D_IMAGE_HEIGHT, strImagePath);
	TableD.HeaderD.aTitleName.Add(strImage);
	
	// record
	int nRowCount = 4;
	int nColCount = 5;
	TableD.RecordD.nRecordRowCount = nRowCount;
	TableD.RecordD.nRecordColCount = nColCount;
	TableD.RecordD.aRecordData.RemoveAll();
	TableD.RecordD.aRecordData.SetSize(nRowCount);
	
	for(int i=0; i<nRowCount; i++)
		TableD.RecordD.aRecordData[i].SetSize(nColCount);

	CString strShadeH=_T(""), strShadeC=_T("");
	strShadeH = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=14277081 tbwidth=20%[/DRG_CELL_PROPERTY]');
	strShadeC = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=16777215 tbwidth=20%[/DRG_CELL_PROPERTY]');

	TableD.RecordD.aRecordData[0][0] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_A)+m_strArea;
	TableD.RecordD.aRecordData[0][1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asy)+m_strArea;
	TableD.RecordD.aRecordData[0][2] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asz)+m_strArea;
	TableD.RecordD.aRecordData[0][3] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zp)+m_strLeng;
	TableD.RecordD.aRecordData[0][4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zm)+m_strLeng;
	
	TableD.RecordD.aRecordData[1][0] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Area);
	TableD.RecordD.aRecordData[1][1] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Asy);
	TableD.RecordD.aRecordData[1][2] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Asz);
	TableD.RecordD.aRecordData[1][3] = Format_StringDouble(strShadeC, m_StiffInfo.Si.zp );
	TableD.RecordD.aRecordData[1][4] = Format_StringDouble(strShadeC, m_StiffInfo.Si.zm );
	
	TableD.RecordD.aRecordData[2][0] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ixx)+m_strStif;
	TableD.RecordD.aRecordData[2][1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Iyy)+m_strStif;
	TableD.RecordD.aRecordData[2][2] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Izz)+m_strStif;
	TableD.RecordD.aRecordData[2][3] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_yp)+m_strLeng;
	TableD.RecordD.aRecordData[2][4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_ym)+m_strLeng;
	
	TableD.RecordD.aRecordData[3][0] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Ixx);
	TableD.RecordD.aRecordData[3][1] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Iyy);
	TableD.RecordD.aRecordData[3][2] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Izz);
	TableD.RecordD.aRecordData[3][3] = Format_StringDouble(strShadeC, m_StiffInfo.Si.yp );
	TableD.RecordD.aRecordData[3][4] = Format_StringDouble(strShadeC, m_StiffInfo.Si.ym );
	
	// Make xml file
	ReportTB.SetData(TableD);
	if(!ReportTB.GenerateReport(strFilePathName)) 
		return FALSE;
	
	return TRUE;
}

BOOL CDgnAmrSectionTb::GenerateType2(CString strFilePathName)
{
	if(m_StiffInfo.nType!=2)
		return FALSE;

	CReportTableProcess ReportTB;
	
	T_TABLE_D TableD;
	TableD.Initialize();
	
	// TableD에 데이터 채우기
	// header
	TableD.HeaderD.bRepeatHeader = FALSE;
	TableD.HeaderD.nHeaderRowCount = 0;
	TableD.HeaderD.nHeaderColCount = 0;
	
	// record
	int nRowCount = 4;
	int nColCount = 5;
	TableD.RecordD.nRecordRowCount = nRowCount;
	TableD.RecordD.nRecordColCount = nColCount;
	TableD.RecordD.aRecordData.RemoveAll();
	TableD.RecordD.aRecordData.SetSize(nRowCount);
	
	for(int i=0; i<nRowCount; i++)
		TableD.RecordD.aRecordData[i].SetSize(nColCount);

	CString strShadeH=_T(""), strShadeC=_T("");
	strShadeH = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=14277081 tbwidth=20% distwid=1[/DRG_CELL_PROPERTY]');
	strShadeC = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=16777215 tbwidth=20%[/DRG_CELL_PROPERTY]');
	
	TableD.RecordD.aRecordData[0][0] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_A)+m_strArea;
	TableD.RecordD.aRecordData[0][1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asy)+m_strArea;
	TableD.RecordD.aRecordData[0][2] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asz)+m_strArea;
	TableD.RecordD.aRecordData[0][3] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zp)+m_strLeng;
	TableD.RecordD.aRecordData[0][4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zm)+m_strLeng;
	
	TableD.RecordD.aRecordData[1][0] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Area);
	TableD.RecordD.aRecordData[1][1] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Asy);
	TableD.RecordD.aRecordData[1][2] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Asz);
	TableD.RecordD.aRecordData[1][3] = Format_StringDouble(strShadeC, m_StiffInfo.Si.zp );
	TableD.RecordD.aRecordData[1][4] = Format_StringDouble(strShadeC, m_StiffInfo.Si.zm );
	
	TableD.RecordD.aRecordData[2][0] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ixx)+m_strStif;
	TableD.RecordD.aRecordData[2][1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Iyy)+m_strStif;
	TableD.RecordD.aRecordData[2][2] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Izz)+m_strStif;
	TableD.RecordD.aRecordData[2][3] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_yp)+m_strLeng;
	TableD.RecordD.aRecordData[2][4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_ym)+m_strLeng;
	
	TableD.RecordD.aRecordData[3][0] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Ixx);
	TableD.RecordD.aRecordData[3][1] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Iyy);
	TableD.RecordD.aRecordData[3][2] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Izz);
	TableD.RecordD.aRecordData[3][3] = Format_StringDouble(strShadeC, m_StiffInfo.Si.yp );
	TableD.RecordD.aRecordData[3][4] = Format_StringDouble(strShadeC, m_StiffInfo.Si.ym );
	
	// Make xml file
	ReportTB.SetData(TableD);
	if(!ReportTB.GenerateReport(strFilePathName)) 
		return FALSE;
	
	return TRUE;
}

BOOL CDgnAmrSectionTb::GenerateType4(CString strFilePathName, CStringArray& arImagePath)
{
	if(m_StiffInfo.nType!=4)
		return FALSE;

	int nImageCnt = arImagePath.GetSize();
	if(nImageCnt<=0)
		return FALSE;

	CReportTableProcess ReportTB;
	
	T_TABLE_D TableD;
	TableD.Initialize();
	
	// TableD에 데이터 채우기
	// header
	TableD.HeaderD.bRepeatHeader = FALSE;
	TableD.HeaderD.nHeaderRowCount = 1;
	TableD.HeaderD.nHeaderColCount = 5;
	
	T_MERGE_UNIT_D MegerD;
	MegerD.nStartColNumber = 1;
	MegerD.nStartRowNumber = 1;
	MegerD.nMergeCount = 5;
	
	TableD.HeaderD.aMergeColD.Add(MegerD);
	
	CString strImagePath = arImagePath[0];
	CString strImage=_T("");
	strImage.Format(_ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=16777215 distwid=1[/DRG_CELL_PROPERTY][DRG_IMAGE|width=%d|height=%d]%s[/DRG_IMAGE]'), D_IMAGE_WIDTH, D_IMAGE_HEIGHT, strImagePath);
	TableD.HeaderD.aTitleName.Add(strImage);
	
	// record
	int nRowCount = 6;
	int nColCount = 5;
	TableD.RecordD.nRecordRowCount = nRowCount;
	TableD.RecordD.nRecordColCount = nColCount;
	TableD.RecordD.aRecordData.RemoveAll();
	TableD.RecordD.aRecordData.SetSize(nRowCount);
	
	for(int i=0; i<nRowCount; i++)
		TableD.RecordD.aRecordData[i].SetSize(nColCount);
	
	CString strShadeH=_T(""), strShadeC=_T("");
	strShadeH = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=14277081 tbwidth=20%[/DRG_CELL_PROPERTY]');
	strShadeC = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=16777215 tbwidth=20%[/DRG_CELL_PROPERTY]');

	TableD.RecordD.aRecordData[0][0] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Es_Ec);
	TableD.RecordD.aRecordData[0][1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Gs_Gc);
	TableD.RecordD.aRecordData[0][2] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ds_Dc);
	TableD.RecordD.aRecordData[0][3] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ps);
	TableD.RecordD.aRecordData[0][4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Pc);
	
	TableD.RecordD.aRecordData[1][0] = Format_StringDouble(strShadeC, m_StiffInfo.EsEc);
	TableD.RecordD.aRecordData[1][1] = Format_StringDouble(strShadeC, m_StiffInfo.GsGc);
	TableD.RecordD.aRecordData[1][2] = Format_StringDouble(strShadeC, m_StiffInfo.DsDc);
	TableD.RecordD.aRecordData[1][3] = Format_StringDouble(strShadeC, m_StiffInfo.Ps  );
	TableD.RecordD.aRecordData[1][4] = Format_StringDouble(strShadeC, m_StiffInfo.Pc  );
	
	TableD.RecordD.aRecordData[2][0] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_A)+m_strArea;
	TableD.RecordD.aRecordData[2][1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asy)+m_strArea;
	TableD.RecordD.aRecordData[2][2] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asz)+m_strArea;
	TableD.RecordD.aRecordData[2][3] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zp)+m_strLeng;
	TableD.RecordD.aRecordData[2][4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zm)+m_strLeng;
	
	TableD.RecordD.aRecordData[3][0] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Area);
	TableD.RecordD.aRecordData[3][1] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Asy);
	TableD.RecordD.aRecordData[3][2] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Asz);
	TableD.RecordD.aRecordData[3][3] = Format_StringDouble(strShadeC, m_StiffInfo.Si.zp );
	TableD.RecordD.aRecordData[3][4] = Format_StringDouble(strShadeC, m_StiffInfo.Si.zm );
	
	TableD.RecordD.aRecordData[4][0] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ixx)+m_strStif;
	TableD.RecordD.aRecordData[4][1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Iyy)+m_strStif;
	TableD.RecordD.aRecordData[4][2] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Izz)+m_strStif;
	TableD.RecordD.aRecordData[4][3] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_yp)+m_strLeng;
	TableD.RecordD.aRecordData[4][4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_ym)+m_strLeng;
	
	TableD.RecordD.aRecordData[5][0] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Ixx);
	TableD.RecordD.aRecordData[5][1] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Iyy);
	TableD.RecordD.aRecordData[5][2] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Izz);
	TableD.RecordD.aRecordData[5][3] = Format_StringDouble(strShadeC, m_StiffInfo.Si.yp );
	TableD.RecordD.aRecordData[5][4] = Format_StringDouble(strShadeC, m_StiffInfo.Si.ym );
	
	// Make xml file
	ReportTB.SetData(TableD);
	if(!ReportTB.GenerateReport(strFilePathName)) 
		return FALSE;
	
	return TRUE;
}

BOOL CDgnAmrSectionTb::GenerateType5(CString strFilePathName, CStringArray& arImagePath)
{
	if(m_StiffInfo.nType!=5)
		return FALSE;

	int nImageCnt = arImagePath.GetSize();
	if(nImageCnt<=1)
		return FALSE;

	CReportTableProcess ReportTB;
	
	T_TABLE_D TableD;
	TableD.Initialize();
	
	// TableD에 데이터 채우기
	// header
	TableD.HeaderD.bRepeatHeader = FALSE;
	TableD.HeaderD.nHeaderRowCount = 2;
	TableD.HeaderD.nHeaderColCount = 10;
	
	T_MERGE_UNIT_D MegerD;
	MegerD.nStartColNumber = 1;
	MegerD.nStartRowNumber = 1;
	MegerD.nMergeCount = 5;
	TableD.HeaderD.aMergeColD.Add(MegerD);

	MegerD.nStartColNumber = 6;
	MegerD.nStartRowNumber = 1;
	MegerD.nMergeCount = 5;
	TableD.HeaderD.aMergeColD.Add(MegerD);

	MegerD.nStartColNumber = 1;
	MegerD.nStartRowNumber = 2;
	MegerD.nMergeCount = 5;
	TableD.HeaderD.aMergeColD.Add(MegerD);

	MegerD.nStartColNumber = 6;
	MegerD.nStartRowNumber = 2;
	MegerD.nMergeCount = 5;
	TableD.HeaderD.aMergeColD.Add(MegerD);

	CString strImagePath1 = arImagePath[0];
	CString strImagePath2 = arImagePath[1];
	CString strImage1=_T(""), strImage2=_T("");
	strImage1.Format(_ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=16777215 distwid=1[/DRG_CELL_PROPERTY][DRG_IMAGE|width=%d|height=%d]%s[/DRG_IMAGE]'), D_IMAGE_WIDTH, D_IMAGE_HEIGHT, strImagePath1);
	strImage2.Format(_ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=16777215[/DRG_CELL_PROPERTY][DRG_IMAGE|width=%d|height=%d]%s[/DRG_IMAGE]'), D_IMAGE_WIDTH, D_IMAGE_HEIGHT, strImagePath2);
	
	TableD.HeaderD.aTitleName.Add(_ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=14277081[/DRG_CELL_PROPERTY]Before Composite'));
	TableD.HeaderD.aTitleName.Add(_ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=14277081[/DRG_CELL_PROPERTY]After Composite'));
	TableD.HeaderD.aTitleName.Add(strImage1);
	TableD.HeaderD.aTitleName.Add(strImage2);
	
	// record
	int nRowCount = 6;
	int nColCount = 10;
	TableD.RecordD.nRecordRowCount = nRowCount;
	TableD.RecordD.nRecordColCount = nColCount;
	TableD.RecordD.aRecordData.RemoveAll();
	TableD.RecordD.aRecordData.SetSize(nRowCount);
	
	for(int i=0; i<nRowCount; i++)
		TableD.RecordD.aRecordData[i].SetSize(nColCount);
	
	CString strShadeH=_T(""), strShadeC=_T("");
	strShadeH = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=14277081 tbwidth=10%[/DRG_CELL_PROPERTY]');
	strShadeC = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=16777215 tbwidth=10%[/DRG_CELL_PROPERTY]');

	TableD.RecordD.aRecordData[0][0] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_A)+m_strArea;
	TableD.RecordD.aRecordData[0][1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asy)+m_strArea;
	TableD.RecordD.aRecordData[0][2] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asz)+m_strArea;
	TableD.RecordD.aRecordData[0][3] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zp)+m_strLeng;
	TableD.RecordD.aRecordData[0][4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zm)+m_strLeng;
	TableD.RecordD.aRecordData[0][5] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_A)+m_strArea;
	TableD.RecordD.aRecordData[0][6] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asy)+m_strArea;
	TableD.RecordD.aRecordData[0][7] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asz)+m_strArea;
	TableD.RecordD.aRecordData[0][8] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zp)+m_strLeng;
	TableD.RecordD.aRecordData[0][9] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zm)+m_strLeng;
	
	TableD.RecordD.aRecordData[1][0] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Area);
	TableD.RecordD.aRecordData[1][1] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Asy);
	TableD.RecordD.aRecordData[1][2] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Asz);
	TableD.RecordD.aRecordData[1][3] = Format_StringDouble(strShadeC, m_StiffInfo.Si.zp );
	TableD.RecordD.aRecordData[1][4] = Format_StringDouble(strShadeC, m_StiffInfo.Si.zm );
	TableD.RecordD.aRecordData[1][5] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Si.Area);
	TableD.RecordD.aRecordData[1][6] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Si.Asy);
	TableD.RecordD.aRecordData[1][7] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Si.Asz);
	TableD.RecordD.aRecordData[1][8] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Si.zp );
	TableD.RecordD.aRecordData[1][9] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Si.zm );
	
	TableD.RecordD.aRecordData[2][0] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ixx)+m_strStif;
	TableD.RecordD.aRecordData[2][1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Iyy)+m_strStif;
	TableD.RecordD.aRecordData[2][2] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Izz)+m_strStif;
	TableD.RecordD.aRecordData[2][3] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_yp)+m_strLeng;
	TableD.RecordD.aRecordData[2][4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_ym)+m_strLeng;
	TableD.RecordD.aRecordData[2][5] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ixx)+m_strStif;
	TableD.RecordD.aRecordData[2][6] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Iyy)+m_strStif;
	TableD.RecordD.aRecordData[2][7] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Izz)+m_strStif;
	TableD.RecordD.aRecordData[2][8] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_yp)+m_strLeng;
	TableD.RecordD.aRecordData[2][9] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_ym)+m_strLeng;
	
	TableD.RecordD.aRecordData[3][0] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Ixx);
	TableD.RecordD.aRecordData[3][1] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Iyy);
	TableD.RecordD.aRecordData[3][2] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Izz);
	TableD.RecordD.aRecordData[3][3] = Format_StringDouble(strShadeC, m_StiffInfo.Si.yp );
	TableD.RecordD.aRecordData[3][4] = Format_StringDouble(strShadeC, m_StiffInfo.Si.ym );
	TableD.RecordD.aRecordData[3][5] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Si.Ixx);
	TableD.RecordD.aRecordData[3][6] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Si.Iyy);
	TableD.RecordD.aRecordData[3][7] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Si.Izz);
	TableD.RecordD.aRecordData[3][8] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Si.yp );
	TableD.RecordD.aRecordData[3][9] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Si.ym );
	
	TableD.RecordD.aRecordData[4][0] = strShadeC+_T("-");
	TableD.RecordD.aRecordData[4][1] = strShadeC+_T("-");
	TableD.RecordD.aRecordData[4][2] = strShadeC+_T("-");
	TableD.RecordD.aRecordData[4][3] = strShadeC+_T("-");
	TableD.RecordD.aRecordData[4][4] = strShadeC+_T("-");
	TableD.RecordD.aRecordData[4][5] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Es_Ec);
	TableD.RecordD.aRecordData[4][6] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Gs_Gc);
	TableD.RecordD.aRecordData[4][7] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ds_Dc);
	TableD.RecordD.aRecordData[4][8] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ps);
	TableD.RecordD.aRecordData[4][9] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Pc);
	
	TableD.RecordD.aRecordData[5][0].Format(_T("%s-"), strShadeC);
	TableD.RecordD.aRecordData[5][1].Format(_T("%s-"), strShadeC);
	TableD.RecordD.aRecordData[5][2].Format(_T("%s-"), strShadeC);
	TableD.RecordD.aRecordData[5][3].Format(_T("%s-"), strShadeC);
	TableD.RecordD.aRecordData[5][4].Format(_T("%s-"), strShadeC);
	TableD.RecordD.aRecordData[5][5] = Format_StringDouble(strShadeC, m_StiffInfo.EsEc);
	TableD.RecordD.aRecordData[5][6] = Format_StringDouble(strShadeC, m_StiffInfo.GsGc);
	TableD.RecordD.aRecordData[5][7] = Format_StringDouble(strShadeC, m_StiffInfo.DsDc);
	TableD.RecordD.aRecordData[5][8] = Format_StringDouble(strShadeC, m_StiffInfo.Ps  );
	TableD.RecordD.aRecordData[5][9] = Format_StringDouble(strShadeC, m_StiffInfo.Pc  );

	// Make xml file
	ReportTB.SetData(TableD);
	if(!ReportTB.GenerateReport(strFilePathName)) 
		return FALSE;
	
	return TRUE;
}

BOOL CDgnAmrSectionTb::GenerateType6(CString strFilePathName, CStringArray& arImagePath)
{
	if(m_StiffInfo.nType!=6)
		return FALSE;
	
	int nImageCnt = arImagePath.GetSize();
	if(nImageCnt<=1)
		return FALSE;

	CReportTableProcess ReportTB;
	
	T_TABLE_D TableD;
	TableD.Initialize();
	
	// TableD에 데이터 채우기
	// header
	TableD.HeaderD.bRepeatHeader = FALSE;
	TableD.HeaderD.nHeaderRowCount = 2;
	TableD.HeaderD.nHeaderColCount = 10;
	
	T_MERGE_UNIT_D MegerD;
	MegerD.nStartColNumber = 1;
	MegerD.nStartRowNumber = 1;
	MegerD.nMergeCount = 5;
	TableD.HeaderD.aMergeColD.Add(MegerD);
	
	MegerD.nStartColNumber = 6;
	MegerD.nStartRowNumber = 1;
	MegerD.nMergeCount = 5;
	TableD.HeaderD.aMergeColD.Add(MegerD);
	
	MegerD.nStartColNumber = 1;
	MegerD.nStartRowNumber = 2;
	MegerD.nMergeCount = 5;
	TableD.HeaderD.aMergeColD.Add(MegerD);
	
	MegerD.nStartColNumber = 6;
	MegerD.nStartRowNumber = 2;
	MegerD.nMergeCount = 5;
	TableD.HeaderD.aMergeColD.Add(MegerD);
	
	CString strImagePath1 = arImagePath[0];
	CString strImagePath2 = arImagePath[1];
	CString strImage1=_T(""), strImage2=_T("");
	strImage1.Format(_ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=16777215 distwid=1[/DRG_CELL_PROPERTY][DRG_IMAGE|width=%d|height=%d]%s[/DRG_IMAGE]'), D_IMAGE_WIDTH, D_IMAGE_HEIGHT, strImagePath1);
	strImage2.Format(_ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=16777215[/DRG_CELL_PROPERTY][DRG_IMAGE|width=%d|height=%d]%s[/DRG_IMAGE]'), D_IMAGE_WIDTH, D_IMAGE_HEIGHT, strImagePath2);
	
	TableD.HeaderD.aTitleName.Add(_ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=14277081[/DRG_CELL_PROPERTY]I-End'));
	TableD.HeaderD.aTitleName.Add(_ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=14277081[/DRG_CELL_PROPERTY]J-End'));
	TableD.HeaderD.aTitleName.Add(strImage1);
	TableD.HeaderD.aTitleName.Add(strImage2);
	
	// record
	int nRowCount = 4;
	int nColCount = 10;
	TableD.RecordD.nRecordRowCount = nRowCount;
	TableD.RecordD.nRecordColCount = nColCount;
	TableD.RecordD.aRecordData.RemoveAll();
	TableD.RecordD.aRecordData.SetSize(nRowCount);
	
	for(int i=0; i<nRowCount; i++)
		TableD.RecordD.aRecordData[i].SetSize(nColCount);
	
	CString strShadeH=_T(""), strShadeC=_T("");
	strShadeH = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=14277081 tbwidth=10%[/DRG_CELL_PROPERTY]');
	strShadeC = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=16777215 tbwidth=10%[/DRG_CELL_PROPERTY]');

	TableD.RecordD.aRecordData[0][0] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_A)+m_strArea;
	TableD.RecordD.aRecordData[0][1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asy)+m_strArea;
	TableD.RecordD.aRecordData[0][2] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asz)+m_strArea;
	TableD.RecordD.aRecordData[0][3] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zp)+m_strLeng;
	TableD.RecordD.aRecordData[0][4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zm)+m_strLeng;
	TableD.RecordD.aRecordData[0][5] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_A)+m_strArea;
	TableD.RecordD.aRecordData[0][6] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asy)+m_strArea;
	TableD.RecordD.aRecordData[0][7] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asz)+m_strArea;
	TableD.RecordD.aRecordData[0][8] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zp)+m_strLeng;
	TableD.RecordD.aRecordData[0][9] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zm)+m_strLeng;
	
	TableD.RecordD.aRecordData[1][0] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Area);
	TableD.RecordD.aRecordData[1][1] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Asy);
	TableD.RecordD.aRecordData[1][2] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Asz);
	TableD.RecordD.aRecordData[1][3] = Format_StringDouble(strShadeC, m_StiffInfo.Si.zp );
	TableD.RecordD.aRecordData[1][4] = Format_StringDouble(strShadeC, m_StiffInfo.Si.zm );
	TableD.RecordD.aRecordData[1][5] = Format_StringDouble(strShadeC, m_StiffInfo.Sj.Area);
	TableD.RecordD.aRecordData[1][6] = Format_StringDouble(strShadeC, m_StiffInfo.Sj.Asy);
	TableD.RecordD.aRecordData[1][7] = Format_StringDouble(strShadeC, m_StiffInfo.Sj.Asz);
	TableD.RecordD.aRecordData[1][8] = Format_StringDouble(strShadeC, m_StiffInfo.Sj.zp );
	TableD.RecordD.aRecordData[1][9] = Format_StringDouble(strShadeC, m_StiffInfo.Sj.zm );
	
	TableD.RecordD.aRecordData[2][0] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ixx)+m_strStif;
	TableD.RecordD.aRecordData[2][1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Iyy)+m_strStif;
	TableD.RecordD.aRecordData[2][2] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Izz)+m_strStif;
	TableD.RecordD.aRecordData[2][3] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_yp)+m_strLeng;
	TableD.RecordD.aRecordData[2][4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_ym)+m_strLeng;
	TableD.RecordD.aRecordData[2][5] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ixx)+m_strStif;
	TableD.RecordD.aRecordData[2][6] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Iyy)+m_strStif;
	TableD.RecordD.aRecordData[2][7] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Izz)+m_strStif;
	TableD.RecordD.aRecordData[2][8] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_yp)+m_strLeng;
	TableD.RecordD.aRecordData[2][9] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_ym)+m_strLeng;
	
	TableD.RecordD.aRecordData[3][0] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Ixx);
	TableD.RecordD.aRecordData[3][1] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Iyy);
	TableD.RecordD.aRecordData[3][2] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Izz);
	TableD.RecordD.aRecordData[3][3] = Format_StringDouble(strShadeC, m_StiffInfo.Si.yp );
	TableD.RecordD.aRecordData[3][4] = Format_StringDouble(strShadeC, m_StiffInfo.Si.ym );
	TableD.RecordD.aRecordData[3][5] = Format_StringDouble(strShadeC, m_StiffInfo.Sj.Ixx);
	TableD.RecordD.aRecordData[3][6] = Format_StringDouble(strShadeC, m_StiffInfo.Sj.Iyy);
	TableD.RecordD.aRecordData[3][7] = Format_StringDouble(strShadeC, m_StiffInfo.Sj.Izz);
	TableD.RecordD.aRecordData[3][8] = Format_StringDouble(strShadeC, m_StiffInfo.Sj.yp );
	TableD.RecordD.aRecordData[3][9] = Format_StringDouble(strShadeC, m_StiffInfo.Sj.ym );

	// Make xml file
	ReportTB.SetData(TableD);
	if(!ReportTB.GenerateReport(strFilePathName)) 
		return FALSE;
	
	return TRUE;
}

BOOL CDgnAmrSectionTb::GenerateType7(CString strFilePathName, CStringArray& arImagePath)
{
	if(m_StiffInfo.nType!=7)
		return FALSE;
	
	int nImageCnt = arImagePath.GetSize();
	if(nImageCnt<=3)
		return FALSE;

	CReportTableProcess ReportTB;
	
	T_TABLE_D TableD;
	TableD.Initialize();
	
	// TableD에 데이터 채우기
	// header
	TableD.HeaderD.bRepeatHeader = FALSE;
	TableD.HeaderD.nHeaderRowCount = 0;
	TableD.HeaderD.nHeaderColCount = 0;

	// record
	int nRowCount = 18;
	int nColCount = 10;
	TableD.RecordD.nRecordRowCount = nRowCount;
	TableD.RecordD.nRecordColCount = nColCount;
	TableD.RecordD.aRecordData.RemoveAll();
	TableD.RecordD.aRecordData.SetSize(nRowCount);
	
	for(int i=0; i<2; i++)
		TableD.RecordD.aRecordData[i].SetSize(65);

	CString strShadeH=_T(""), strShadeC=_T("");
	strShadeH = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=14277081 tbwidth=10% distwid=1[/DRG_CELL_PROPERTY]');
	strShadeC = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=16777215 tbwidth=10%[/DRG_CELL_PROPERTY]');

	T_MERGE_UNIT_D MegerD;
	MegerD.nStartColNumber = 10;
	MegerD.nStartRowNumber = 9;
	MegerD.nMergeCount = 1;
	TableD.RecordD.aMergeRowD.Add(MegerD);

	MegerD.nStartColNumber = 1;
	MegerD.nStartRowNumber = 1;
	MegerD.nMergeCount = 10;
	TableD.RecordD.aMergeColD.Add(MegerD);
	
	MegerD.nStartColNumber = 1;
	MegerD.nStartRowNumber = 2;
	MegerD.nMergeCount = 5;
	TableD.RecordD.aMergeColD.Add(MegerD);
	
	MegerD.nStartColNumber = 6;
	MegerD.nStartRowNumber = 2;
	MegerD.nMergeCount = 5;
	TableD.RecordD.aMergeColD.Add(MegerD);

	MegerD.nStartColNumber = 1;
	MegerD.nStartRowNumber = 3;
	MegerD.nMergeCount = 5;
	TableD.RecordD.aMergeColD.Add(MegerD);
	
	MegerD.nStartColNumber = 6;
	MegerD.nStartRowNumber = 3;
	MegerD.nMergeCount = 5;
	TableD.RecordD.aMergeColD.Add(MegerD);

	MegerD.nStartColNumber = 10;
	MegerD.nStartRowNumber = 9;
	MegerD.nMergeCount = 1;
	TableD.RecordD.aMergeColD.Add(MegerD);

	CString strImagePath1 = arImagePath[0];
	CString strImagePath2 = arImagePath[1];
	CString strImagePath3 = arImagePath[2];
	CString strImagePath4 = arImagePath[3];
	CString strImage1=_T(""), strImage2=_T(""), strImage3=_T(""), strImage4=_T("");
	strImage1.Format(_ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=16777215[/DRG_CELL_PROPERTY][DRG_IMAGE|width=%d|height=%d]%s[/DRG_IMAGE]'), D_IMAGE_WIDTH, D_IMAGE_HEIGHT, strImagePath1);
	strImage2.Format(_ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=16777215[/DRG_CELL_PROPERTY][DRG_IMAGE|width=%d|height=%d]%s[/DRG_IMAGE]'), D_IMAGE_WIDTH, D_IMAGE_HEIGHT, strImagePath2);
	strImage3.Format(_ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=16777215[/DRG_CELL_PROPERTY][DRG_IMAGE|width=%d|height=%d]%s[/DRG_IMAGE]'), D_IMAGE_WIDTH, D_IMAGE_HEIGHT, strImagePath3);
	strImage4.Format(_ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=16777215[/DRG_CELL_PROPERTY][DRG_IMAGE|width=%d|height=%d]%s[/DRG_IMAGE]'), D_IMAGE_WIDTH, D_IMAGE_HEIGHT, strImagePath4);

	// Before Composite
	TableD.RecordD.aRecordData[0][0] = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=14277081[/DRG_CELL_PROPERTY]Before Composite');
	TableD.RecordD.aRecordData[0][1] = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=14277081[/DRG_CELL_PROPERTY]I-End');
	TableD.RecordD.aRecordData[0][2] = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=14277081[/DRG_CELL_PROPERTY]J-End');
	TableD.RecordD.aRecordData[0][3] = strImage1;
	TableD.RecordD.aRecordData[0][4] = strImage2;

	TableD.RecordD.aRecordData[0][5+0] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_A)+m_strArea;
	TableD.RecordD.aRecordData[0][5+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asy)+m_strArea;
	TableD.RecordD.aRecordData[0][5+2] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asz)+m_strArea;
	TableD.RecordD.aRecordData[0][5+3] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zp)+m_strLeng;
	TableD.RecordD.aRecordData[0][5+4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zm)+m_strLeng;
	TableD.RecordD.aRecordData[0][5+5] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_A)+m_strArea;
	TableD.RecordD.aRecordData[0][5+6] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asy)+m_strArea;
	TableD.RecordD.aRecordData[0][5+7] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asz)+m_strArea;
	TableD.RecordD.aRecordData[0][5+8] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zp)+m_strLeng;
	TableD.RecordD.aRecordData[0][5+9] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zm)+m_strLeng;
	
	TableD.RecordD.aRecordData[0][15+0] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Area);
	TableD.RecordD.aRecordData[0][15+1] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Asy);
	TableD.RecordD.aRecordData[0][15+2] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Asz);
	TableD.RecordD.aRecordData[0][15+3] = Format_StringDouble(strShadeC, m_StiffInfo.Si.zp );
	TableD.RecordD.aRecordData[0][15+4] = Format_StringDouble(strShadeC, m_StiffInfo.Si.zm );
	TableD.RecordD.aRecordData[0][15+5] = Format_StringDouble(strShadeC, m_StiffInfo.Sj.Area);
	TableD.RecordD.aRecordData[0][15+6] = Format_StringDouble(strShadeC, m_StiffInfo.Sj.Asy);
	TableD.RecordD.aRecordData[0][15+7] = Format_StringDouble(strShadeC, m_StiffInfo.Sj.Asz);
	TableD.RecordD.aRecordData[0][15+8] = Format_StringDouble(strShadeC, m_StiffInfo.Sj.zp );
	TableD.RecordD.aRecordData[0][15+9] = Format_StringDouble(strShadeC, m_StiffInfo.Sj.zm );
	
	TableD.RecordD.aRecordData[0][25+0] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ixx)+m_strStif;
	TableD.RecordD.aRecordData[0][25+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Iyy)+m_strStif;
	TableD.RecordD.aRecordData[0][25+2] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Izz)+m_strStif;
	TableD.RecordD.aRecordData[0][25+3] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_yp)+m_strLeng;
	TableD.RecordD.aRecordData[0][25+4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_ym)+m_strLeng;
	TableD.RecordD.aRecordData[0][25+5] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ixx)+m_strStif;
	TableD.RecordD.aRecordData[0][25+6] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Iyy)+m_strStif;
	TableD.RecordD.aRecordData[0][25+7] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Izz)+m_strStif;
	TableD.RecordD.aRecordData[0][25+8] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_yp)+m_strLeng;
	TableD.RecordD.aRecordData[0][25+9] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_ym)+m_strLeng;
	
	TableD.RecordD.aRecordData[0][35+0] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Ixx);
	TableD.RecordD.aRecordData[0][35+1] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Iyy);
	TableD.RecordD.aRecordData[0][35+2] = Format_StringDouble(strShadeC, m_StiffInfo.Si.Izz);
	TableD.RecordD.aRecordData[0][35+3] = Format_StringDouble(strShadeC, m_StiffInfo.Si.yp );
	TableD.RecordD.aRecordData[0][35+4] = Format_StringDouble(strShadeC, m_StiffInfo.Si.ym );
	TableD.RecordD.aRecordData[0][35+5] = Format_StringDouble(strShadeC, m_StiffInfo.Sj.Ixx);
	TableD.RecordD.aRecordData[0][35+6] = Format_StringDouble(strShadeC, m_StiffInfo.Sj.Iyy);
	TableD.RecordD.aRecordData[0][35+7] = Format_StringDouble(strShadeC, m_StiffInfo.Sj.Izz);
	TableD.RecordD.aRecordData[0][35+8] = Format_StringDouble(strShadeC, m_StiffInfo.Sj.yp );
	TableD.RecordD.aRecordData[0][35+9] = Format_StringDouble(strShadeC, m_StiffInfo.Sj.ym );

	TableD.RecordD.aRecordData[0][45+0] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Es_Ec);
	TableD.RecordD.aRecordData[0][45+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Gs_Gc);
	TableD.RecordD.aRecordData[0][45+2] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ds_Dc);
	TableD.RecordD.aRecordData[0][45+3] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ps);
	TableD.RecordD.aRecordData[0][45+4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Pc);
	TableD.RecordD.aRecordData[0][45+5] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Es_Ec);
	TableD.RecordD.aRecordData[0][45+6] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Gs_Gc);
	TableD.RecordD.aRecordData[0][45+7] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ds_Dc);
	TableD.RecordD.aRecordData[0][45+8] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ps);
	TableD.RecordD.aRecordData[0][45+9] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Pc);
	
	TableD.RecordD.aRecordData[0][55+0].Format(_T("%s-"), strShadeC);
	TableD.RecordD.aRecordData[0][55+1].Format(_T("%s-"), strShadeC);
	TableD.RecordD.aRecordData[0][55+2].Format(_T("%s-"), strShadeC);
	TableD.RecordD.aRecordData[0][55+3].Format(_T("%s-"), strShadeC);
	TableD.RecordD.aRecordData[0][55+4].Format(_T("%s-"), strShadeC);
	TableD.RecordD.aRecordData[0][55+5].Format(_T("%s-"), strShadeC);
	TableD.RecordD.aRecordData[0][55+6].Format(_T("%s-"), strShadeC);
	TableD.RecordD.aRecordData[0][55+7].Format(_T("%s-"), strShadeC);
	TableD.RecordD.aRecordData[0][55+8].Format(_T("%s-"), strShadeC);
	TableD.RecordD.aRecordData[0][55+9].Format(_T("%s-"), strShadeC);

	// After Composite
	TableD.RecordD.aRecordData[1][0] = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=14277081[/DRG_CELL_PROPERTY]After Composite');
	TableD.RecordD.aRecordData[1][1] = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=14277081[/DRG_CELL_PROPERTY]I-End');
	TableD.RecordD.aRecordData[1][2] = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=14277081[/DRG_CELL_PROPERTY]J-End');
	TableD.RecordD.aRecordData[1][3] = strImage3;
	TableD.RecordD.aRecordData[1][4] = strImage4;
	
	TableD.RecordD.aRecordData[1][5+0] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_A)+m_strArea;
	TableD.RecordD.aRecordData[1][5+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asy)+m_strArea;
	TableD.RecordD.aRecordData[1][5+2] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asz)+m_strArea;
	TableD.RecordD.aRecordData[1][5+3] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zp)+m_strLeng;
	TableD.RecordD.aRecordData[1][5+4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zm)+m_strLeng;
	TableD.RecordD.aRecordData[1][5+5] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_A)+m_strArea;
	TableD.RecordD.aRecordData[1][5+6] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asy)+m_strArea;
	TableD.RecordD.aRecordData[1][5+7] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asz)+m_strArea;
	TableD.RecordD.aRecordData[1][5+8] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zp)+m_strLeng;
	TableD.RecordD.aRecordData[1][5+9] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zm)+m_strLeng;
	
	TableD.RecordD.aRecordData[1][15+0] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Si.Area);
	TableD.RecordD.aRecordData[1][15+1] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Si.Asy);
	TableD.RecordD.aRecordData[1][15+2] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Si.Asz);
	TableD.RecordD.aRecordData[1][15+3] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Si.zp );
	TableD.RecordD.aRecordData[1][15+4] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Si.zm );
	TableD.RecordD.aRecordData[1][15+5] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Sj.Area);
	TableD.RecordD.aRecordData[1][15+6] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Sj.Asy);
	TableD.RecordD.aRecordData[1][15+7] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Sj.Asz);
	TableD.RecordD.aRecordData[1][15+8] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Sj.zp );
	TableD.RecordD.aRecordData[1][15+9] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Sj.zm );
	
	TableD.RecordD.aRecordData[1][25+0] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ixx)+m_strStif;
	TableD.RecordD.aRecordData[1][25+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Iyy)+m_strStif;
	TableD.RecordD.aRecordData[1][25+2] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Izz)+m_strStif;
	TableD.RecordD.aRecordData[1][25+3] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_yp)+m_strLeng;
	TableD.RecordD.aRecordData[1][25+4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_ym)+m_strLeng;
	TableD.RecordD.aRecordData[1][25+5] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ixx)+m_strStif;
	TableD.RecordD.aRecordData[1][25+6] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Iyy)+m_strStif;
	TableD.RecordD.aRecordData[1][25+7] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Izz)+m_strStif;
	TableD.RecordD.aRecordData[1][25+8] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_yp)+m_strLeng;
	TableD.RecordD.aRecordData[1][25+9] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_ym)+m_strLeng;
	
	TableD.RecordD.aRecordData[1][35+0] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Si.Ixx);
	TableD.RecordD.aRecordData[1][35+1] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Si.Iyy);
	TableD.RecordD.aRecordData[1][35+2] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Si.Izz);
	TableD.RecordD.aRecordData[1][35+3] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Si.yp );
	TableD.RecordD.aRecordData[1][35+4] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Si.ym );
	TableD.RecordD.aRecordData[1][35+5] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Sj.Ixx);
	TableD.RecordD.aRecordData[1][35+6] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Sj.Iyy);
	TableD.RecordD.aRecordData[1][35+7] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Sj.Izz);
	TableD.RecordD.aRecordData[1][35+8] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Sj.yp );
	TableD.RecordD.aRecordData[1][35+9] = Format_StringDouble(strShadeC, m_AfterStiffInfo.Sj.ym );

	TableD.RecordD.aRecordData[1][45+0] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Es_Ec);
	TableD.RecordD.aRecordData[1][45+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Gs_Gc);
	TableD.RecordD.aRecordData[1][45+2] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ds_Dc);
	TableD.RecordD.aRecordData[1][45+3] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ps);
	TableD.RecordD.aRecordData[1][45+4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Pc);
	TableD.RecordD.aRecordData[1][45+5] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Es_Ec);
	TableD.RecordD.aRecordData[1][45+6] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Gs_Gc);
	TableD.RecordD.aRecordData[1][45+7] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ds_Dc);
	TableD.RecordD.aRecordData[1][45+8] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ps);
	TableD.RecordD.aRecordData[1][45+9] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Pc);
	
	TableD.RecordD.aRecordData[1][55+0] = Format_StringDouble(strShadeC, m_StiffInfo.EsEc);
	TableD.RecordD.aRecordData[1][55+1] = Format_StringDouble(strShadeC, m_StiffInfo.GsGc);
	TableD.RecordD.aRecordData[1][55+2] = Format_StringDouble(strShadeC, m_StiffInfo.DsDc);
	TableD.RecordD.aRecordData[1][55+3] = Format_StringDouble(strShadeC, m_StiffInfo.Ps  );
	TableD.RecordD.aRecordData[1][55+4] = Format_StringDouble(strShadeC, m_StiffInfo.Pc  );
	TableD.RecordD.aRecordData[1][55+5] = Format_StringDouble(strShadeC, m_StiffInfo.EsEc);
	TableD.RecordD.aRecordData[1][55+6] = Format_StringDouble(strShadeC, m_StiffInfo.GsGc);
	TableD.RecordD.aRecordData[1][55+7] = Format_StringDouble(strShadeC, m_StiffInfo.DsDc);
	TableD.RecordD.aRecordData[1][55+8] = Format_StringDouble(strShadeC, m_StiffInfo.Ps  );
	TableD.RecordD.aRecordData[1][55+9] = Format_StringDouble(strShadeC, m_StiffInfo.Pc  );

///////////////
	/*
		// Before Composite
	TableD.RecordD.aRecordData[0][0] = _T("Before Composite");
	TableD.RecordD.aRecordData[0][1] = _T("Before Composite");
	TableD.RecordD.aRecordData[1][0] = _T("I-End");
	TableD.RecordD.aRecordData[1][1] = _T("J-End");
	TableD.RecordD.aRecordData[2][0] = strImage1;
	TableD.RecordD.aRecordData[2][1] = strImage2;

	TableD.RecordD.aRecordData[3][0] = _T("A(m2)");
	TableD.RecordD.aRecordData[3][1] = _T("Asy(m2)");
	TableD.RecordD.aRecordData[3][2] = _T("Asz(m2)");
	TableD.RecordD.aRecordData[3][3] = _T("z(+)(m)");
	TableD.RecordD.aRecordData[3][4] = _T("z(-)(m)");
	TableD.RecordD.aRecordData[3][5] = _T("A(m2)");
	TableD.RecordD.aRecordData[3][6] = _T("Asy(m2)");
	TableD.RecordD.aRecordData[3][7] = _T("Asz(m2)");
	TableD.RecordD.aRecordData[3][8] = _T("z(+)(m)");
	TableD.RecordD.aRecordData[3][9] = _T("z(-)(m)");
	
	TableD.RecordD.aRecordData[4][0].Format(_T("%.3f"), m_StiffInfo.Si.Area);
	TableD.RecordD.aRecordData[4][1].Format(_T("%.3f"), m_StiffInfo.Si.Asy);
	TableD.RecordD.aRecordData[4][2].Format(_T("%.3f"), m_StiffInfo.Si.Asz);
	TableD.RecordD.aRecordData[4][3].Format(_T("%.3f"), m_StiffInfo.Si.zp );
	TableD.RecordD.aRecordData[4][4].Format(_T("%.3f"), m_StiffInfo.Si.zm );
	TableD.RecordD.aRecordData[4][5].Format(_T("%.3f"), m_StiffInfo.Sj.Area);
	TableD.RecordD.aRecordData[4][6].Format(_T("%.3f"), m_StiffInfo.Sj.Asy);
	TableD.RecordD.aRecordData[4][7].Format(_T("%.3f"), m_StiffInfo.Sj.Asz);
	TableD.RecordD.aRecordData[4][8].Format(_T("%.3f"), m_StiffInfo.Sj.zp );
	TableD.RecordD.aRecordData[4][9].Format(_T("%.3f"), m_StiffInfo.Sj.zm );
	
	TableD.RecordD.aRecordData[5][0] = _T("Ixx(m4)");
	TableD.RecordD.aRecordData[5][1] = _T("Iyy(m4)");
	TableD.RecordD.aRecordData[5][2] = _T("Izz(m4)");
	TableD.RecordD.aRecordData[5][3] = _T("y(+)(m)");
	TableD.RecordD.aRecordData[5][4] = _T("y(-)(m)");
	TableD.RecordD.aRecordData[5][5] = _T("Ixx(m4)");
	TableD.RecordD.aRecordData[5][6] = _T("Iyy(m4)");
	TableD.RecordD.aRecordData[5][7] = _T("Izz(m4)");
	TableD.RecordD.aRecordData[5][8] = _T("y(+)(m)");
	TableD.RecordD.aRecordData[5][9] = _T("y(-)(m)");
	
	TableD.RecordD.aRecordData[6][0].Format(_T("%.3f"), m_StiffInfo.Si.Ixx);
	TableD.RecordD.aRecordData[6][1].Format(_T("%.3f"), m_StiffInfo.Si.Iyy);
	TableD.RecordD.aRecordData[6][2].Format(_T("%.3f"), m_StiffInfo.Si.Izz);
	TableD.RecordD.aRecordData[6][3].Format(_T("%.3f"), m_StiffInfo.Si.yp );
	TableD.RecordD.aRecordData[6][4].Format(_T("%.3f"), m_StiffInfo.Si.ym );
	TableD.RecordD.aRecordData[6][5].Format(_T("%.3f"), m_StiffInfo.Sj.Ixx);
	TableD.RecordD.aRecordData[6][6].Format(_T("%.3f"), m_StiffInfo.Sj.Iyy);
	TableD.RecordD.aRecordData[6][7].Format(_T("%.3f"), m_StiffInfo.Sj.Izz);
	TableD.RecordD.aRecordData[6][8].Format(_T("%.3f"), m_StiffInfo.Sj.yp );
	TableD.RecordD.aRecordData[6][9].Format(_T("%.3f"), m_StiffInfo.Sj.ym );

	// After Composite
	TableD.RecordD.aRecordData[7+0][0] = _T("After Composite");
	TableD.RecordD.aRecordData[7+0][1] = _T("After Composite");
	TableD.RecordD.aRecordData[7+1][0] = _T("I-End");
	TableD.RecordD.aRecordData[7+1][1] = _T("J-End");
	TableD.RecordD.aRecordData[7+2][0] = strImage1;
	TableD.RecordD.aRecordData[7+2][1] = strImage2;
	
	TableD.RecordD.aRecordData[7+3][0] = _T("A(m2)");
	TableD.RecordD.aRecordData[7+3][1] = _T("Asy(m2)");
	TableD.RecordD.aRecordData[7+3][2] = _T("Asz(m2)");
	TableD.RecordD.aRecordData[7+3][3] = _T("z(+)(m)");
	TableD.RecordD.aRecordData[7+3][4] = _T("z(-)(m)");
	TableD.RecordD.aRecordData[7+3][5] = _T("A(m2)");
	TableD.RecordD.aRecordData[7+3][6] = _T("Asy(m2)");
	TableD.RecordD.aRecordData[7+3][7] = _T("Asz(m2)");
	TableD.RecordD.aRecordData[7+3][8] = _T("z(+)(m)");
	TableD.RecordD.aRecordData[7+3][9] = _T("z(-)(m)");
	
	TableD.RecordD.aRecordData[7+4][0].Format(_T("%.3f"), m_AfterStiffInfo.Si.Area);
	TableD.RecordD.aRecordData[7+4][1].Format(_T("%.3f"), m_AfterStiffInfo.Si.Asy);
	TableD.RecordD.aRecordData[7+4][2].Format(_T("%.3f"), m_AfterStiffInfo.Si.Asz);
	TableD.RecordD.aRecordData[7+4][3].Format(_T("%.3f"), m_AfterStiffInfo.Si.zp );
	TableD.RecordD.aRecordData[7+4][4].Format(_T("%.3f"), m_AfterStiffInfo.Si.zm );
	TableD.RecordD.aRecordData[7+4][5].Format(_T("%.3f"), m_AfterStiffInfo.Sj.Area);
	TableD.RecordD.aRecordData[7+4][6].Format(_T("%.3f"), m_AfterStiffInfo.Sj.Asy);
	TableD.RecordD.aRecordData[7+4][7].Format(_T("%.3f"), m_AfterStiffInfo.Sj.Asz);
	TableD.RecordD.aRecordData[7+4][8].Format(_T("%.3f"), m_AfterStiffInfo.Sj.zp );
	TableD.RecordD.aRecordData[7+4][9].Format(_T("%.3f"), m_AfterStiffInfo.Sj.zm );
	
	TableD.RecordD.aRecordData[7+5][0] = _T("Ixx(m4)");
	TableD.RecordD.aRecordData[7+5][1] = _T("Iyy(m4)");
	TableD.RecordD.aRecordData[7+5][2] = _T("Izz(m4)");
	TableD.RecordD.aRecordData[7+5][3] = _T("y(+)(m)");
	TableD.RecordD.aRecordData[7+5][4] = _T("y(-)(m)");
	TableD.RecordD.aRecordData[7+5][5] = _T("Ixx(m4)");
	TableD.RecordD.aRecordData[7+5][6] = _T("Iyy(m4)");
	TableD.RecordD.aRecordData[7+5][7] = _T("Izz(m4)");
	TableD.RecordD.aRecordData[7+5][8] = _T("y(+)(m)");
	TableD.RecordD.aRecordData[7+5][9] = _T("y(-)(m)");
	
	TableD.RecordD.aRecordData[7+6][0].Format(_T("%.3f"), m_AfterStiffInfo.Si.Ixx);
	TableD.RecordD.aRecordData[7+6][1].Format(_T("%.3f"), m_AfterStiffInfo.Si.Iyy);
	TableD.RecordD.aRecordData[7+6][2].Format(_T("%.3f"), m_AfterStiffInfo.Si.Izz);
	TableD.RecordD.aRecordData[7+6][3].Format(_T("%.3f"), m_AfterStiffInfo.Si.yp );
	TableD.RecordD.aRecordData[7+6][4].Format(_T("%.3f"), m_AfterStiffInfo.Si.ym );
	TableD.RecordD.aRecordData[7+6][5].Format(_T("%.3f"), m_AfterStiffInfo.Sj.Ixx);
	TableD.RecordD.aRecordData[7+6][6].Format(_T("%.3f"), m_AfterStiffInfo.Sj.Iyy);
	TableD.RecordD.aRecordData[7+6][7].Format(_T("%.3f"), m_AfterStiffInfo.Sj.Izz);
	TableD.RecordD.aRecordData[7+6][8].Format(_T("%.3f"), m_AfterStiffInfo.Sj.yp );
	TableD.RecordD.aRecordData[7+6][9].Format(_T("%.3f"), m_AfterStiffInfo.Sj.ym );

	TableD.RecordD.aRecordData[7+7][0] = _LS(IDS_DGN_DYNA_REPORT_Es_Ec);
	TableD.RecordD.aRecordData[7+7][1] = _LS(IDS_DGN_DYNA_REPORT_Gs_Gc);
	TableD.RecordD.aRecordData[7+7][2] = _LS(IDS_DGN_DYNA_REPORT_Ds_Dc);
	TableD.RecordD.aRecordData[7+7][3] = _LS(IDS_DGN_DYNA_REPORT_Ps);
	TableD.RecordD.aRecordData[7+7][4] = _LS(IDS_DGN_DYNA_REPORT_Pc);
	TableD.RecordD.aRecordData[7+7][5] = _LS(IDS_DGN_DYNA_REPORT_Es_Ec);
	TableD.RecordD.aRecordData[7+7][6] = _LS(IDS_DGN_DYNA_REPORT_Gs_Gc);
	TableD.RecordD.aRecordData[7+7][7] = _LS(IDS_DGN_DYNA_REPORT_Ds_Dc);
	TableD.RecordD.aRecordData[7+7][8] = _LS(IDS_DGN_DYNA_REPORT_Ps);
	TableD.RecordD.aRecordData[7+7][9] = _LS(IDS_DGN_DYNA_REPORT_Pc);
	
	TableD.RecordD.aRecordData[7+8][0].Format(_T("%.3f"), m_AfterStiffInfo.EsEc);
	TableD.RecordD.aRecordData[7+8][1].Format(_T("%.3f"), m_AfterStiffInfo.GsGc);
	TableD.RecordD.aRecordData[7+8][2].Format(_T("%.3f"), m_AfterStiffInfo.DsDc);
	TableD.RecordD.aRecordData[7+8][3].Format(_T("%.3f"), m_AfterStiffInfo.Ps  );
	TableD.RecordD.aRecordData[7+8][4].Format(_T("%.3f"), m_AfterStiffInfo.Pc  );
	TableD.RecordD.aRecordData[7+8][5].Format(_T("%.3f"), m_AfterStiffInfo.EsEc);
	TableD.RecordD.aRecordData[7+8][6].Format(_T("%.3f"), m_AfterStiffInfo.GsGc);
	TableD.RecordD.aRecordData[7+8][7].Format(_T("%.3f"), m_AfterStiffInfo.DsDc);
	TableD.RecordD.aRecordData[7+8][8].Format(_T("%.3f"), m_AfterStiffInfo.Ps  );
	TableD.RecordD.aRecordData[7+8][9].Format(_T("%.3f"), m_AfterStiffInfo.Pc  );
	*/

	// Make xml file
	ReportTB.SetData(TableD);
	if(!ReportTB.GenerateReport(strFilePathName)) 
		return FALSE;
	
	return TRUE;
}

BOOL CDgnAmrSectionTb::GenerateSectionRein(CString strFilePathName, CStringArray& arImagePath, BOOL bConsiderJ)
{
	if(!bConsiderJ)
		return GenerateReinI(strFilePathName, arImagePath);
	else
		return GenerateReinIJ(strFilePathName, arImagePath);
	
	return FALSE;
}

BOOL CDgnAmrSectionTb::GenerateReinI(CString strFilePathName, CStringArray& arImagePath)
{
	int nImageCnt = arImagePath.GetSize();
	if(nImageCnt<=0)
		return FALSE;

	int nLRbarCnt = m_RpscInfoI.aLRbar.GetSize();
//   if(nLRbarCnt<=0)
//     return FALSE;
	
	CReportTableProcess ReportTB;
	
	T_TABLE_D TableD;
	TableD.Initialize();
	
	// TableD에 데이터 채우기
	// header
	TableD.HeaderD.bRepeatHeader = FALSE;
	TableD.HeaderD.nHeaderRowCount = 3;
	TableD.HeaderD.nHeaderColCount = 7;
	
	T_MERGE_UNIT_D MegerD;
	MegerD.nStartColNumber = 6;
	MegerD.nStartRowNumber = 2;
	MegerD.nMergeCount = 2;
	TableD.HeaderD.aMergeRowD.Add(MegerD);

	MegerD.nStartColNumber = 7;
	MegerD.nStartRowNumber = 2;
	MegerD.nMergeCount = 2;
	TableD.HeaderD.aMergeRowD.Add(MegerD);
	
	MegerD.nStartColNumber = 1;
	MegerD.nStartRowNumber = 1;
	MegerD.nMergeCount = 7;
	TableD.HeaderD.aMergeColD.Add(MegerD);

	MegerD.nStartColNumber = 1;
	MegerD.nStartRowNumber = 2;
	MegerD.nMergeCount = 5;
	TableD.HeaderD.aMergeColD.Add(MegerD);

	MegerD.nStartColNumber = 6;
	MegerD.nStartRowNumber = 2;
	MegerD.nMergeCount = 2;
	TableD.HeaderD.aMergeColD.Add(MegerD);

	CString strImagePath = arImagePath[0];
	CString strImage=_T("");
	strImage.Format(_ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=16777215[/DRG_CELL_PROPERTY][DRG_IMAGE|width=%d|height=%d]%s[/DRG_IMAGE]'), D_IMAGE_WIDTH, D_IMAGE_HEIGHT, strImagePath);
	TableD.HeaderD.aTitleName.Add(strImage);

	CString strShadeH=_T(""), strShadeC=_T("");
	strShadeH = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=14277081[/DRG_CELL_PROPERTY]');
	strShadeC = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=16777215[/DRG_CELL_PROPERTY]');

	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Longitudinal_Reinforcement));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Shear_Reinforcement));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_No_));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Part));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Size));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Num));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Spacing)+m_strLeng);
	
	// record
	int nRowCount=0;
	BOOL bExistLongt=TRUE;
	if(nLRbarCnt<=0) bExistLongt=FALSE;   // Longitudinal Data가 없음, '-'로 표시한다. 20100713 KIMJM
	if(nLRbarCnt<4) nRowCount = 4;
	else            nRowCount = nLRbarCnt;
	int nColCount = 7;
	TableD.RecordD.nRecordRowCount = nRowCount;
	TableD.RecordD.nRecordColCount = nColCount;
	TableD.RecordD.aRecordData.RemoveAll();
	TableD.RecordD.aRecordData.SetSize(nRowCount);
	
	for(int i=0; i<nRowCount; i++)
		TableD.RecordD.aRecordData[i].SetSize(nColCount);

	for(int i=0; i<nRowCount; i++)
	{
		if(i<nLRbarCnt && bExistLongt)
		{
			TableD.RecordD.aRecordData[i][0].Format(_T("%s%d"), strShadeC, i+1);
			TableD.RecordD.aRecordData[i][1].Format(_T("%s-"), strShadeC);
			TableD.RecordD.aRecordData[i][2].Format(_T("%s%s"), strShadeC, m_RpscInfoI.aLRbar[i].strSize);
			TableD.RecordD.aRecordData[i][3].Format(_T("%s%d"), strShadeC, m_RpscInfoI.aLRbar[i].nNum);
			TableD.RecordD.aRecordData[i][4].Format(_T("%s%g"), strShadeC, m_RpscInfoI.aLRbar[i].dSpacing);
		}
		else
		{
			TableD.RecordD.aRecordData[i][0] = strShadeC+_T("");
			TableD.RecordD.aRecordData[i][1] = strShadeC+_T("");
			TableD.RecordD.aRecordData[i][2] = strShadeC+_T("");
			TableD.RecordD.aRecordData[i][3] = strShadeC+_T("");
			TableD.RecordD.aRecordData[i][4] = strShadeC+_T("");
		}

		CString strTmp=_T("");
		if(i==0)  
		{
			TableD.RecordD.aRecordData[i][5] = strShadeC+_LS(IDS_DGN_DYNA_REPORT_Diagonal_Reinforcement);
			strTmp = m_RpscInfoI.bShearDiagonal? _T("O"):_T("-");
			TableD.RecordD.aRecordData[i][6] = strShadeC+strTmp;
		}
		else if(i==1)  
		{
			TableD.RecordD.aRecordData[i][5] = strShadeC+_LS(IDS_DGN_DYNA_REPORT_Steel_Bar_for_Web);
			strTmp = m_RpscInfoI.bShearWeb? _T("O"):_T("-");
			TableD.RecordD.aRecordData[i][6] = strShadeC+strTmp;
		}
		else if(i==2)  
		{
			TableD.RecordD.aRecordData[i][5] = strShadeC+_LS(IDS_DGN_DYNA_REPORT_Torsion_Reinforcement);
			strTmp = m_RpscInfoI.bShearTorsion? _T("O"):_T("-");
			TableD.RecordD.aRecordData[i][6] = strShadeC+strTmp;
		}
		else if(i==3)  
		{
			TableD.RecordD.aRecordData[i][5] = strShadeC+_LS(IDS_DGN_DYNA_REPORT_Enclosing_Stirrup);
			strTmp = m_RpscInfoI.bShearEnclosing? _T("O"):_T("-");
			TableD.RecordD.aRecordData[i][6] = strShadeC+strTmp;
		}
		else
		{
			TableD.RecordD.aRecordData[i][5] = strShadeC+_T("");
			TableD.RecordD.aRecordData[i][6] = strShadeC+_T("");
		}
	}
		
	// Make xml file
	ReportTB.SetData(TableD);
	if(!ReportTB.GenerateReport(strFilePathName)) 
		return FALSE;
	
	return TRUE;
}

BOOL CDgnAmrSectionTb::GenerateReinIJ(CString strFilePathName, CStringArray& arImagePath)
{
	int nImageCnt = arImagePath.GetSize();
	if(nImageCnt<=1)
		return FALSE;

	int nLRbarCnt = m_RpscInfoI.aLRbar.GetSize();
//   if(nLRbarCnt<=0)
//     return FALSE;

	int nLRbarJCnt = m_RpscInfoJ.aLRbar.GetSize();
//   if(nLRbarJCnt<=0)
//     return FALSE;

	ASSERT(nLRbarCnt==nLRbarJCnt);

	CReportTableProcess ReportTB;
	
	T_TABLE_D TableD;
	TableD.Initialize();
	
	// TableD에 데이터 채우기
	// header
	TableD.HeaderD.bRepeatHeader = FALSE;
	TableD.HeaderD.nHeaderRowCount = 3;
	TableD.HeaderD.nHeaderColCount = 14;
	
	T_MERGE_UNIT_D MegerD;
	MegerD.nStartColNumber = 6;
	MegerD.nStartRowNumber = 2;
	MegerD.nMergeCount = 2;
	TableD.HeaderD.aMergeRowD.Add(MegerD);

	MegerD.nStartColNumber = 7;
	MegerD.nStartRowNumber = 2;
	MegerD.nMergeCount = 2;
	TableD.HeaderD.aMergeRowD.Add(MegerD);

	MegerD.nStartColNumber = 13;
	MegerD.nStartRowNumber = 2;
	MegerD.nMergeCount = 2;
	TableD.HeaderD.aMergeRowD.Add(MegerD);
	
	MegerD.nStartColNumber = 14;
	MegerD.nStartRowNumber = 2;
	MegerD.nMergeCount = 2;
	TableD.HeaderD.aMergeRowD.Add(MegerD);

	MegerD.nStartColNumber = 1;
	MegerD.nStartRowNumber = 1;
	MegerD.nMergeCount = 7;
	TableD.HeaderD.aMergeColD.Add(MegerD);

	MegerD.nStartColNumber = 8;
	MegerD.nStartRowNumber = 1;
	MegerD.nMergeCount = 7;
	TableD.HeaderD.aMergeColD.Add(MegerD);

	MegerD.nStartColNumber = 1;
	MegerD.nStartRowNumber = 2;
	MegerD.nMergeCount = 5;
	TableD.HeaderD.aMergeColD.Add(MegerD);

	MegerD.nStartColNumber = 6;
	MegerD.nStartRowNumber = 2;
	MegerD.nMergeCount = 2;
	TableD.HeaderD.aMergeColD.Add(MegerD);

	MegerD.nStartColNumber = 8;
	MegerD.nStartRowNumber = 2;
	MegerD.nMergeCount = 5;
	TableD.HeaderD.aMergeColD.Add(MegerD);
	
	MegerD.nStartColNumber = 13;
	MegerD.nStartRowNumber = 2;
	MegerD.nMergeCount = 2;
	TableD.HeaderD.aMergeColD.Add(MegerD);

	CString strImagePath1 = arImagePath[0];
	CString strImagePath2 = arImagePath[1];
	CString strImage1=_T(""), strImage2=_T("");
	strImage1.Format(_ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=16777215[/DRG_CELL_PROPERTY][DRG_IMAGE|width=%d|height=%d]%s[/DRG_IMAGE]'), D_IMAGE_WIDTH, D_IMAGE_HEIGHT, strImagePath1);
	strImage2.Format(_ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=16777215[/DRG_CELL_PROPERTY][DRG_IMAGE|width=%d|height=%d]%s[/DRG_IMAGE]'), D_IMAGE_WIDTH, D_IMAGE_HEIGHT, strImagePath2);
	TableD.HeaderD.aTitleName.Add(strImage1);
	TableD.HeaderD.aTitleName.Add(strImage2);

	CString strShadeH=_T(""), strShadeC=_T("");
	strShadeH = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=14277081[/DRG_CELL_PROPERTY]');
	strShadeC = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=16777215[/DRG_CELL_PROPERTY]');

	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Longitudinal_Reinforcement));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Shear_Reinforcement));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Longitudinal_Reinforcement));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Shear_Reinforcement));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_No_));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Part));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Size));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Num));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Spacing)+m_strLeng);
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_No_));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Part));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Size));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Num));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Spacing)+m_strLeng);
	
	// record
	int nRowCount=0;
	BOOL bExistLongt=TRUE;
	if(nLRbarCnt<=0) bExistLongt=FALSE;   // Longitudinal Data가 없음, '-'로 표시한다. 20100713 KIMJM
	if(nLRbarCnt<4) nRowCount = 4;
	else            nRowCount = nLRbarCnt;
	int nColCount = 14;
	TableD.RecordD.nRecordRowCount = nRowCount;
	TableD.RecordD.nRecordColCount = nColCount;
	TableD.RecordD.aRecordData.RemoveAll();
	TableD.RecordD.aRecordData.SetSize(nRowCount);
	
	for(int i=0; i<nRowCount; i++)
		TableD.RecordD.aRecordData[i].SetSize(nColCount);

	for(int i=0; i<nRowCount; i++)
	{
		if(i<nLRbarCnt && bExistLongt)
		{
			TableD.RecordD.aRecordData[i][0 ].Format(_T("%s%d"), strShadeC, i+1);
			TableD.RecordD.aRecordData[i][1 ].Format(_T("%s-"), strShadeC);
			TableD.RecordD.aRecordData[i][2 ].Format(_T("%s%s"), strShadeC, m_RpscInfoI.aLRbar[i].strSize);
			TableD.RecordD.aRecordData[i][3 ].Format(_T("%s%d"), strShadeC, m_RpscInfoI.aLRbar[i].nNum);
			TableD.RecordD.aRecordData[i][4 ].Format(_T("%s%g"), strShadeC, m_RpscInfoI.aLRbar[i].dSpacing);
			TableD.RecordD.aRecordData[i][7 ].Format(_T("%s%d"), strShadeC, i+1);
			TableD.RecordD.aRecordData[i][8 ].Format(_T("%s-"), strShadeC);
			TableD.RecordD.aRecordData[i][9 ].Format(_T("%s%s"), strShadeC, m_RpscInfoJ.aLRbar[i].strSize);
			TableD.RecordD.aRecordData[i][10].Format(_T("%s%d"), strShadeC, m_RpscInfoJ.aLRbar[i].nNum);
			TableD.RecordD.aRecordData[i][11].Format(_T("%s%g"), strShadeC, m_RpscInfoJ.aLRbar[i].dSpacing);
		}
		else
		{
			TableD.RecordD.aRecordData[i][0 ] = strShadeC+_T("");
			TableD.RecordD.aRecordData[i][1 ] = strShadeC+_T("");
			TableD.RecordD.aRecordData[i][2 ] = strShadeC+_T("");
			TableD.RecordD.aRecordData[i][3 ] = strShadeC+_T("");
			TableD.RecordD.aRecordData[i][4 ] = strShadeC+_T("");
			TableD.RecordD.aRecordData[i][7 ] = strShadeC+_T("");
			TableD.RecordD.aRecordData[i][8 ] = strShadeC+_T("");
			TableD.RecordD.aRecordData[i][9 ] = strShadeC+_T("");
			TableD.RecordD.aRecordData[i][10] = strShadeC+_T("");
			TableD.RecordD.aRecordData[i][11] = strShadeC+_T("");
		}

		CString strTmpI=_T(""), strTmpJ=_T("");
		if(i==0)  
		{
			strTmpI = m_RpscInfoI.bShearDiagonal? _T("O"):_T("-");
			strTmpJ = m_RpscInfoJ.bShearDiagonal? _T("O"):_T("-");
			TableD.RecordD.aRecordData[i][5 ] = strShadeC+_LS(IDS_DGN_DYNA_REPORT_Diagonal_Reinforcement);
			TableD.RecordD.aRecordData[i][6 ] = strShadeC+strTmpI;
			TableD.RecordD.aRecordData[i][12] = strShadeC+_LS(IDS_DGN_DYNA_REPORT_Diagonal_Reinforcement);
			TableD.RecordD.aRecordData[i][13] = strShadeC+strTmpJ;
		}
		else if(i==1)  
		{
			strTmpI = m_RpscInfoI.bShearWeb? _T("O"):_T("-");
			strTmpJ = m_RpscInfoJ.bShearWeb? _T("O"):_T("-");
			TableD.RecordD.aRecordData[i][5 ] = strShadeC+_LS(IDS_DGN_DYNA_REPORT_Steel_Bar_for_Web);
			TableD.RecordD.aRecordData[i][6 ] = strShadeC+strTmpI;
			TableD.RecordD.aRecordData[i][12] = strShadeC+_LS(IDS_DGN_DYNA_REPORT_Steel_Bar_for_Web);
			TableD.RecordD.aRecordData[i][13] = strShadeC+strTmpJ;
		}
		else if(i==2)  
		{
			strTmpI = m_RpscInfoI.bShearTorsion? _T("O"):_T("-");
			strTmpJ = m_RpscInfoJ.bShearTorsion? _T("O"):_T("-");
			TableD.RecordD.aRecordData[i][5 ] = strShadeC+_LS(IDS_DGN_DYNA_REPORT_Torsion_Reinforcement);
			TableD.RecordD.aRecordData[i][6 ] = strShadeC+strTmpI;
			TableD.RecordD.aRecordData[i][12] = strShadeC+_LS(IDS_DGN_DYNA_REPORT_Torsion_Reinforcement);
			TableD.RecordD.aRecordData[i][13] = strShadeC+strTmpJ;
		}
		else if(i==3)  
		{
			strTmpI = m_RpscInfoI.bShearEnclosing? _T("O"):_T("-");
			strTmpJ = m_RpscInfoJ.bShearEnclosing? _T("O"):_T("-");
			TableD.RecordD.aRecordData[i][5 ] = strShadeC+_LS(IDS_DGN_DYNA_REPORT_Enclosing_Stirrup);
			TableD.RecordD.aRecordData[i][6 ] = strShadeC+strTmpI;
			TableD.RecordD.aRecordData[i][12] = strShadeC+_LS(IDS_DGN_DYNA_REPORT_Enclosing_Stirrup);
			TableD.RecordD.aRecordData[i][13] = strShadeC+strTmpJ;
		}
		else
		{
			TableD.RecordD.aRecordData[i][5 ] = strShadeC+_T("");
			TableD.RecordD.aRecordData[i][6 ] = strShadeC+_T("");
			TableD.RecordD.aRecordData[i][12] = strShadeC+_T("");
			TableD.RecordD.aRecordData[i][13] = strShadeC+_T("");
		}
	}
		
	// Make xml file
	ReportTB.SetData(TableD);
	if(!ReportTB.GenerateReport(strFilePathName)) 
		return FALSE;
	
	return TRUE;
}

BOOL CDgnAmrSectionTb::GenerateSectionComp(CString strFilePathName, CStringArray& arImagePath, int nType, UINT SectK)
{
	if(nType==1)
		return GenerateCompType1(strFilePathName, arImagePath, SectK);
	else if(nType==2)
		return GenerateCompType2(strFilePathName);
	else
		ASSERT(0);
	
	return FALSE;
}

BOOL CDgnAmrSectionTb::GenerateCompType1(CString strFilePathName, CStringArray& arImagePath, UINT SectK)
{
	int nImageCnt = arImagePath.GetSize();
	int nCscsInfoCnt = m_aCscsInfo.GetSize();
	if(nImageCnt<nCscsInfoCnt)
	{
		ASSERT(0);
		return FALSE;
	}
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); 
	ASSERT(pDoc);

	ASSERT(SectK>0);
	T_SECT_D  SectD;  SectD.Initialize();
	if(!pDoc->m_pAttrCtrl->GetSect(SectK, SectD))
		return FALSE;

	CString strSectName=_T("");
	strSectName.Format(_T("%d : %s"), SectK, SectD.SName);
	ASSERT(strSectName!=_T(""));

	CReportTableProcess ReportTB;
	
	T_TABLE_D TableD;
	TableD.Initialize();
	
	// TableD에 데이터 채우기
	// header
	TableD.HeaderD.bRepeatHeader = FALSE;
	TableD.HeaderD.nHeaderRowCount = 1;
	TableD.HeaderD.nHeaderColCount = 11;
	
	T_MERGE_UNIT_D MegerD;
	MegerD.nStartColNumber = 6;
	MegerD.nStartRowNumber = 1;
	MegerD.nMergeCount = 3;
	TableD.HeaderD.aMergeColD.Add(MegerD);

	MegerD.nStartColNumber = 9;
	MegerD.nStartRowNumber = 1;
	MegerD.nMergeCount = 3;
	TableD.HeaderD.aMergeColD.Add(MegerD);

	CString strShadeH=_T(""), strShadeC=_T("");
	strShadeH = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=14277081[/DRG_CELL_PROPERTY]');
	strShadeC = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=16777215[/DRG_CELL_PROPERTY]');

	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Part));
	TableD.HeaderD.aTitleName.Add(strShadeH+strSectName);
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Material));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Stage));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Age));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Stiffness_I_end));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Stiffness_J_end));
	
	// record
	int nRowCount = nCscsInfoCnt * 10;
	int nColCount = 11;
	TableD.RecordD.nRecordRowCount = nRowCount;
	TableD.RecordD.nRecordColCount = nColCount;
	TableD.RecordD.aRecordData.RemoveAll();
	TableD.RecordD.aRecordData.SetSize(nCscsInfoCnt);
	
	for(int i=1; i<=5; i++)
	{
		MegerD.nStartColNumber = i;
		MegerD.nStartRowNumber = 1;
		MegerD.nMergeCount = 10;
		TableD.RecordD.aMergeRowD.Add(MegerD);
	}
//   MegerD.nStartColNumber = 11;
//   MegerD.nStartRowNumber = 10;
//   MegerD.nMergeCount = 1;
//   TableD.RecordD.aMergeRowD.Add(MegerD);
// 
//   MegerD.nStartColNumber = 11;
//   MegerD.nStartRowNumber = 10;
//   MegerD.nMergeCount = 1;
//   TableD.RecordD.aMergeColD.Add(MegerD);

	for(int i=0; i<nCscsInfoCnt; i++)
		TableD.RecordD.aRecordData[i].SetSize(65);

	CString strImagePath=_T("");
	CString strImage=_T("");

	for(int i=0; i<nCscsInfoCnt; i++)
	{
		if(m_aCscsInfo[i].strPart==_T(""))
			TableD.RecordD.aRecordData[i][0] = strShadeC+_T("-");
		else
			TableD.RecordD.aRecordData[i][0] = strShadeC+m_aCscsInfo[i].strPart;
		strImagePath = arImagePath[i];
		strImage.Format(_ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=16777215[/DRG_CELL_PROPERTY][DRG_IMAGE|width=%d|height=%d]%s[/DRG_IMAGE]'), D_IMAGE_WIDTH, D_IMAGE_HEIGHT, strImagePath);
		TableD.RecordD.aRecordData[i][1] = strImage;
		TableD.RecordD.aRecordData[i][2] = strShadeC+m_aCscsInfo[i].strMatl;
		TableD.RecordD.aRecordData[i][3] = strShadeC+m_aCscsInfo[i].strStage;
		TableD.RecordD.aRecordData[i][4].Format(_T("%s%g"), strShadeC, m_aCscsInfo[i].dAge);

		int k=0;
		TableD.RecordD.aRecordData[i][4+k*6+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_A);
		TableD.RecordD.aRecordData[i][4+k*6+2] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Si.Area);
		TableD.RecordD.aRecordData[i][4+k*6+3] = strShadeC+m_strArea2;
		TableD.RecordD.aRecordData[i][4+k*6+4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_A);
		TableD.RecordD.aRecordData[i][4+k*6+5] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Sj.Area);
		TableD.RecordD.aRecordData[i][4+k*6+6] = strShadeC+m_strArea2;

		k=1;
		TableD.RecordD.aRecordData[i][4+k*6+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asy);
		TableD.RecordD.aRecordData[i][4+k*6+2] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Si.Asy);
		TableD.RecordD.aRecordData[i][4+k*6+3] = strShadeC+m_strArea2;
		TableD.RecordD.aRecordData[i][4+k*6+4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asy);
		TableD.RecordD.aRecordData[i][4+k*6+5] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Sj.Asy);
		TableD.RecordD.aRecordData[i][4+k*6+6] = strShadeC+m_strArea2;

		k=2;
		TableD.RecordD.aRecordData[i][4+k*6+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asz);
		TableD.RecordD.aRecordData[i][4+k*6+2] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Si.Asz);
		TableD.RecordD.aRecordData[i][4+k*6+3] = strShadeC+m_strArea2;
		TableD.RecordD.aRecordData[i][4+k*6+4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asz);
		TableD.RecordD.aRecordData[i][4+k*6+5] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Sj.Asz);
		TableD.RecordD.aRecordData[i][4+k*6+6] = strShadeC+m_strArea2;
		
		k=3;
		TableD.RecordD.aRecordData[i][4+k*6+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ixx);
		TableD.RecordD.aRecordData[i][4+k*6+2] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Si.Ixx);
		TableD.RecordD.aRecordData[i][4+k*6+3] = strShadeC+m_strStif2;
		TableD.RecordD.aRecordData[i][4+k*6+4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ixx);
		TableD.RecordD.aRecordData[i][4+k*6+5] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Sj.Ixx);
		TableD.RecordD.aRecordData[i][4+k*6+6] = strShadeC+m_strStif2;

		k=4;
		TableD.RecordD.aRecordData[i][4+k*6+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Iyy);
		TableD.RecordD.aRecordData[i][4+k*6+2] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Si.Iyy);
		TableD.RecordD.aRecordData[i][4+k*6+3] = strShadeC+m_strStif2;
		TableD.RecordD.aRecordData[i][4+k*6+4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Iyy);
		TableD.RecordD.aRecordData[i][4+k*6+5] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Sj.Iyy);
		TableD.RecordD.aRecordData[i][4+k*6+6] = strShadeC+m_strStif2;

		k=5;
		TableD.RecordD.aRecordData[i][4+k*6+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Izz);
		TableD.RecordD.aRecordData[i][4+k*6+2] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Si.Izz);
		TableD.RecordD.aRecordData[i][4+k*6+3] = strShadeC+m_strStif2;
		TableD.RecordD.aRecordData[i][4+k*6+4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Izz);
		TableD.RecordD.aRecordData[i][4+k*6+5] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Sj.Izz);
		TableD.RecordD.aRecordData[i][4+k*6+6] = strShadeC+m_strStif2;

		k=6;
		TableD.RecordD.aRecordData[i][4+k*6+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zp);
		TableD.RecordD.aRecordData[i][4+k*6+2] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Si.zp);
		TableD.RecordD.aRecordData[i][4+k*6+3] = strShadeC+m_strLeng2;
		TableD.RecordD.aRecordData[i][4+k*6+4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zp);
		TableD.RecordD.aRecordData[i][4+k*6+5] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Sj.zp);
		TableD.RecordD.aRecordData[i][4+k*6+6] = strShadeC+m_strLeng2;

		k=7;
		TableD.RecordD.aRecordData[i][4+k*6+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zm);
		TableD.RecordD.aRecordData[i][4+k*6+2] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Si.zm);
		TableD.RecordD.aRecordData[i][4+k*6+3] = strShadeC+m_strLeng2;
		TableD.RecordD.aRecordData[i][4+k*6+4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zm);
		TableD.RecordD.aRecordData[i][4+k*6+5] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Sj.zm);
		TableD.RecordD.aRecordData[i][4+k*6+6] = strShadeC+m_strLeng2;

		k=8;
		TableD.RecordD.aRecordData[i][4+k*6+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_yp);
		TableD.RecordD.aRecordData[i][4+k*6+2] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Si.yp);
		TableD.RecordD.aRecordData[i][4+k*6+3] = strShadeC+m_strLeng2;
		TableD.RecordD.aRecordData[i][4+k*6+4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_yp);
		TableD.RecordD.aRecordData[i][4+k*6+5] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Sj.yp);
		TableD.RecordD.aRecordData[i][4+k*6+6] = strShadeC+m_strLeng2;

		k=9;
		TableD.RecordD.aRecordData[i][4+k*6+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_ym);
		TableD.RecordD.aRecordData[i][4+k*6+2] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Si.ym);
		TableD.RecordD.aRecordData[i][4+k*6+3] = strShadeC+m_strLeng2;
		TableD.RecordD.aRecordData[i][4+k*6+4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_ym);
		TableD.RecordD.aRecordData[i][4+k*6+5] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Sj.ym);
		TableD.RecordD.aRecordData[i][4+k*6+6] = strShadeC+m_strLeng2;
	}

	// Make xml file
	ReportTB.SetData(TableD);
	if(!ReportTB.GenerateReport(strFilePathName)) 
		return FALSE;
	
	return TRUE;
}

BOOL CDgnAmrSectionTb::GenerateCompType2(CString strFilePathName)
{
	int nCscsInfoCnt = m_aCscsInfo.GetSize();
	if(nCscsInfoCnt<=0)
	{
		ASSERT(0);
		return FALSE;
	}
	
	CReportTableProcess ReportTB;
	
	T_TABLE_D TableD;
	TableD.Initialize();
	
	// TableD에 데이터 채우기
	// header
	TableD.HeaderD.bRepeatHeader = FALSE;
	TableD.HeaderD.nHeaderRowCount = 1;
	TableD.HeaderD.nHeaderColCount = 14;
	
	CString strShadeH=_T(""), strShadeC=_T("");
	strShadeH = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=14277081[/DRG_CELL_PROPERTY]');
	strShadeC = _ULS('[DRG_CELL_PROPERTY]color=0 align=center shape=16777215[/DRG_CELL_PROPERTY]');

	T_MERGE_UNIT_D MegerD;
	MegerD.nStartColNumber = 7;
	MegerD.nStartRowNumber = 1;
	MegerD.nMergeCount = 3;
	TableD.HeaderD.aMergeColD.Add(MegerD);

	MegerD.nStartColNumber = 12;
	MegerD.nStartRowNumber = 1;
	MegerD.nMergeCount = 3;
	TableD.HeaderD.aMergeColD.Add(MegerD);

	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Part));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Material));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Stage));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Age));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Cy_I_end)+m_strLeng);
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Cz_I_end)+m_strLeng);
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Stiffness_I_end));
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Cy_J_end)+m_strLeng);
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Cz_J_end)+m_strLeng);
	TableD.HeaderD.aTitleName.Add(strShadeH+_LS(IDS_DGN_DYNA_REPORT_Stiffness_J_end));
	
	// record
	int nRowCount = nCscsInfoCnt * 10;
	int nColCount = 14;
	TableD.RecordD.nRecordRowCount = nRowCount;
	TableD.RecordD.nRecordColCount = nColCount;
	TableD.RecordD.aRecordData.RemoveAll();
	TableD.RecordD.aRecordData.SetSize(nCscsInfoCnt);
	
	for(int i=1; i<=6; i++)
	{
		MegerD.nStartColNumber = i;
		MegerD.nStartRowNumber = 1;
		MegerD.nMergeCount = 10;
		TableD.RecordD.aMergeRowD.Add(MegerD);
	}
	for(int i=10; i<=11; i++)
	{
		MegerD.nStartColNumber = i;
		MegerD.nStartRowNumber = 1;
		MegerD.nMergeCount = 10;
		TableD.RecordD.aMergeRowD.Add(MegerD);
	}

	for(int i=0; i<nCscsInfoCnt; i++)
		TableD.RecordD.aRecordData[i].SetSize(68);

	for(int i=0; i<nCscsInfoCnt; i++)
	{
		TableD.RecordD.aRecordData[i][ 0] = strShadeC+m_aCscsInfo[i].strPart;
		TableD.RecordD.aRecordData[i][ 1] = strShadeC+m_aCscsInfo[i].strMatl;
		TableD.RecordD.aRecordData[i][ 2] = strShadeC+m_aCscsInfo[i].strStage;
		TableD.RecordD.aRecordData[i][ 3].Format(_T("%s%g"), strShadeC, m_aCscsInfo[i].dAge);
		TableD.RecordD.aRecordData[i][ 4].Format(_T("%s%g"), strShadeC, m_aCscsInfo[i].cyi);
		TableD.RecordD.aRecordData[i][ 5].Format(_T("%s%g"), strShadeC, m_aCscsInfo[i].czi);
		TableD.RecordD.aRecordData[i][ 9].Format(_T("%s%g"), strShadeC, m_aCscsInfo[i].cyj);
		TableD.RecordD.aRecordData[i][10].Format(_T("%s%g"), strShadeC, m_aCscsInfo[i].czj);

		int k=0;
		TableD.RecordD.aRecordData[i][5+k*6+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_A);
		TableD.RecordD.aRecordData[i][5+k*6+2] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Si.Area);
		TableD.RecordD.aRecordData[i][5+k*6+3] = strShadeC+m_strArea2;
		TableD.RecordD.aRecordData[i][5+k*6+6] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_A);
		TableD.RecordD.aRecordData[i][5+k*6+7] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Sj.Area);
		TableD.RecordD.aRecordData[i][5+k*6+8] = strShadeC+m_strArea2;

		k=1;
		TableD.RecordD.aRecordData[i][7+k*6+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asy);
		TableD.RecordD.aRecordData[i][7+k*6+2] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Si.Asy);
		TableD.RecordD.aRecordData[i][7+k*6+3] = strShadeC+m_strArea2;
		TableD.RecordD.aRecordData[i][7+k*6+4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asy);
		TableD.RecordD.aRecordData[i][7+k*6+5] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Sj.Asy);
		TableD.RecordD.aRecordData[i][7+k*6+6] = strShadeC+m_strArea2;

		k=2;
		TableD.RecordD.aRecordData[i][7+k*6+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asz);
		TableD.RecordD.aRecordData[i][7+k*6+2] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Si.Asz);
		TableD.RecordD.aRecordData[i][7+k*6+3] = strShadeC+m_strArea2;
		TableD.RecordD.aRecordData[i][7+k*6+4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Asz);
		TableD.RecordD.aRecordData[i][7+k*6+5] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Sj.Asz);
		TableD.RecordD.aRecordData[i][7+k*6+6] = strShadeC+m_strArea2;

		k=3;
		TableD.RecordD.aRecordData[i][7+k*6+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ixx);
		TableD.RecordD.aRecordData[i][7+k*6+2] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Si.Ixx);
		TableD.RecordD.aRecordData[i][7+k*6+3] = strShadeC+m_strStif2;
		TableD.RecordD.aRecordData[i][7+k*6+4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Ixx);
		TableD.RecordD.aRecordData[i][7+k*6+5] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Sj.Ixx);
		TableD.RecordD.aRecordData[i][7+k*6+6] = strShadeC+m_strStif2;

		k=4;
		TableD.RecordD.aRecordData[i][7+k*6+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Iyy);
		TableD.RecordD.aRecordData[i][7+k*6+2] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Si.Iyy);
		TableD.RecordD.aRecordData[i][7+k*6+3] = strShadeC+m_strStif2;
		TableD.RecordD.aRecordData[i][7+k*6+4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Iyy);
		TableD.RecordD.aRecordData[i][7+k*6+5] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Sj.Iyy);
		TableD.RecordD.aRecordData[i][7+k*6+6] = strShadeC+m_strStif2;

		k=5;
		TableD.RecordD.aRecordData[i][7+k*6+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Izz);
		TableD.RecordD.aRecordData[i][7+k*6+2] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Si.Izz);
		TableD.RecordD.aRecordData[i][7+k*6+3] = strShadeC+m_strStif2;
		TableD.RecordD.aRecordData[i][7+k*6+4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_Izz);
		TableD.RecordD.aRecordData[i][7+k*6+5] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Sj.Izz);
		TableD.RecordD.aRecordData[i][7+k*6+6] = strShadeC+m_strStif2;

		k=6;
		TableD.RecordD.aRecordData[i][7+k*6+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zp);
		TableD.RecordD.aRecordData[i][7+k*6+2] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Si.zp);
		TableD.RecordD.aRecordData[i][7+k*6+3] = strShadeC+m_strLeng2;
		TableD.RecordD.aRecordData[i][7+k*6+4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zp);
		TableD.RecordD.aRecordData[i][7+k*6+5] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Sj.zp);
		TableD.RecordD.aRecordData[i][7+k*6+6] = strShadeC+m_strLeng2;

		k=7;
		TableD.RecordD.aRecordData[i][7+k*6+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zm);
		TableD.RecordD.aRecordData[i][7+k*6+2] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Si.zm);
		TableD.RecordD.aRecordData[i][7+k*6+3] = strShadeC+m_strLeng2;
		TableD.RecordD.aRecordData[i][7+k*6+4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_zm);
		TableD.RecordD.aRecordData[i][7+k*6+5] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Sj.zm);
		TableD.RecordD.aRecordData[i][7+k*6+6] = strShadeC+m_strLeng2;

		k=8;
		TableD.RecordD.aRecordData[i][7+k*6+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_yp);
		TableD.RecordD.aRecordData[i][7+k*6+2] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Si.yp);
		TableD.RecordD.aRecordData[i][7+k*6+3] = strShadeC+m_strLeng2;
		TableD.RecordD.aRecordData[i][7+k*6+4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_yp);
		TableD.RecordD.aRecordData[i][7+k*6+5] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Sj.yp);
		TableD.RecordD.aRecordData[i][7+k*6+6] = strShadeC+m_strLeng2;

		k=9;
		TableD.RecordD.aRecordData[i][7+k*6+1] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_ym);
		TableD.RecordD.aRecordData[i][7+k*6+2] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Si.ym);
		TableD.RecordD.aRecordData[i][7+k*6+3] = strShadeC+m_strLeng2;
		TableD.RecordD.aRecordData[i][7+k*6+4] = strShadeH+_LS(IDS_DGN_DYNA_REPORT_ym);
		TableD.RecordD.aRecordData[i][7+k*6+5] = Format_StringDouble(strShadeC, m_aCscsInfo[i].Sj.ym);
		TableD.RecordD.aRecordData[i][7+k*6+6] = strShadeC+m_strLeng2;
	}

	// Make xml file
	ReportTB.SetData(TableD);
	if(!ReportTB.GenerateReport(strFilePathName)) 
		return FALSE;
	
	return TRUE;
}

/*
BOOL CDgnAmrSectionTb::GenerateType1(CString strFilePathName, CString strImagePath, int nImgWidth, int nImgHeight)
{
	CReportTableProcess ReportTB;
	
	T_TABLE_D TableD;
	TableD.Initialize();
	
	// TableD에 데이터 채우기
	// header
	TableD.HeaderD.nHeaderRowCount = 1;
	TableD.HeaderD.nHeaderColCount = 5;

	T_MERGE_UNIT_D MegerD;
	MegerD.nStartColNumber = 1;
	MegerD.nStartRowNumber = 1;
	MegerD.nMergeCount = 5;
	
	TableD.HeaderD.aMergeColD.Add(MegerD);

	CString strImage=_T("");
	strImage.Format(_T("[DRG_IMAGE|width=%d|height=%d]%s[/DRG_IMAGE]"), nImgWidth, nImgHeight, strImagePath);
	TableD.HeaderD.aTitleName.Add(strImage);

	// record
	int nRowCount = 4;
	int nColCount = 5;
	TableD.RecordD.nRecordRowCount = nRowCount;
	TableD.RecordD.nRecordColCount = nColCount;
	TableD.RecordD.aRecordData.RemoveAll();
	TableD.RecordD.aRecordData.SetSize(nRowCount);
	
	for(int i=0; i<nRowCount; i++)
		TableD.RecordD.aRecordData[i].SetSize(nColCount);

	TableD.RecordD.aRecordData[0][0] = _T("A(m2)");
	TableD.RecordD.aRecordData[0][1] = _T("Asy(m2)");
	TableD.RecordD.aRecordData[0][2] = _T("Asz(m2)");
	TableD.RecordD.aRecordData[0][3] = _T("z(+)(m)");
	TableD.RecordD.aRecordData[0][4] = _T("z(-)(m)");

	TableD.RecordD.aRecordData[1][0].Format(_T("%.3f"), m_SectTbD.Unit1.A  );
	TableD.RecordD.aRecordData[1][1].Format(_T("%.3f"), m_SectTbD.Unit1.Asy);
	TableD.RecordD.aRecordData[1][2].Format(_T("%.3f"), m_SectTbD.Unit1.Asz);
	TableD.RecordD.aRecordData[1][3].Format(_T("%.3f"), m_SectTbD.Unit1.zp );
	TableD.RecordD.aRecordData[1][4].Format(_T("%.3f"), m_SectTbD.Unit1.zm );

	TableD.RecordD.aRecordData[2][0] = _T("Ixx(m4)");
	TableD.RecordD.aRecordData[2][1] = _T("Iyy(m4)");
	TableD.RecordD.aRecordData[2][2] = _T("Izz(m4)");
	TableD.RecordD.aRecordData[2][3] = _T("y(+)(m)");
	TableD.RecordD.aRecordData[2][4] = _T("y(-)(m)");

	TableD.RecordD.aRecordData[3][0].Format(_T("%.3f"), m_SectTbD.Unit1.Ixx);
	TableD.RecordD.aRecordData[3][1].Format(_T("%.3f"), m_SectTbD.Unit1.Iyy);
	TableD.RecordD.aRecordData[3][2].Format(_T("%.3f"), m_SectTbD.Unit1.Izz);
	TableD.RecordD.aRecordData[3][3].Format(_T("%.3f"), m_SectTbD.Unit1.yp );
	TableD.RecordD.aRecordData[3][4].Format(_T("%.3f"), m_SectTbD.Unit1.ym );

	// Make xml file
	ReportTB.SetData(TableD);
	if(!ReportTB.GenerateReport(strFilePathName)) 
		return FALSE;

	return TRUE;
}

BOOL CDgnAmrSectionTb::GenerateType2(CString strFilePathName)
{
	CReportTableProcess ReportTB;
	
	T_TABLE_D TableD;
	TableD.Initialize();
	
	// TableD에 데이터 채우기
	// header
	TableD.HeaderD.nHeaderRowCount = 0;
	TableD.HeaderD.nHeaderColCount = 0;
	
	// record
	int nRowCount = 4;
	int nColCount = 5;
	TableD.RecordD.nRecordRowCount = nRowCount;
	TableD.RecordD.nRecordColCount = nColCount;
	TableD.RecordD.aRecordData.RemoveAll();
	TableD.RecordD.aRecordData.SetSize(nRowCount);
	
	for(int i=0; i<nRowCount; i++)
		TableD.RecordD.aRecordData[i].SetSize(nColCount);
	
	TableD.RecordD.aRecordData[0][0] = _T("A(m2)");
	TableD.RecordD.aRecordData[0][1] = _T("Asy(m2)");
	TableD.RecordD.aRecordData[0][2] = _T("Asz(m2)");
	TableD.RecordD.aRecordData[0][3] = _T("z(+)(m)");
	TableD.RecordD.aRecordData[0][4] = _T("z(-)(m)");
	
	TableD.RecordD.aRecordData[1][0].Format(_T("%.3f"), m_SectTbD.Unit1.A  );
	TableD.RecordD.aRecordData[1][1].Format(_T("%.3f"), m_SectTbD.Unit1.Asy);
	TableD.RecordD.aRecordData[1][2].Format(_T("%.3f"), m_SectTbD.Unit1.Asz);
	TableD.RecordD.aRecordData[1][3].Format(_T("%.3f"), m_SectTbD.Unit1.zp );
	TableD.RecordD.aRecordData[1][4].Format(_T("%.3f"), m_SectTbD.Unit1.zm );
	
	TableD.RecordD.aRecordData[2][0] = _T("Ixx(m4)");
	TableD.RecordD.aRecordData[2][1] = _T("Iyy(m4)");
	TableD.RecordD.aRecordData[2][2] = _T("Izz(m4)");
	TableD.RecordD.aRecordData[2][3] = _T("y(+)(m)");
	TableD.RecordD.aRecordData[2][4] = _T("y(-)(m)");
	
	TableD.RecordD.aRecordData[3][0].Format(_T("%.3f"), m_SectTbD.Unit1.Ixx);
	TableD.RecordD.aRecordData[3][1].Format(_T("%.3f"), m_SectTbD.Unit1.Iyy);
	TableD.RecordD.aRecordData[3][2].Format(_T("%.3f"), m_SectTbD.Unit1.Izz);
	TableD.RecordD.aRecordData[3][3].Format(_T("%.3f"), m_SectTbD.Unit1.yp );
	TableD.RecordD.aRecordData[3][4].Format(_T("%.3f"), m_SectTbD.Unit1.ym );
	
	// Make xml file
	ReportTB.SetData(TableD);
	if(!ReportTB.GenerateReport(strFilePathName)) 
		return FALSE;
	
	return TRUE;
}

BOOL CDgnAmrSectionTb::GenerateType4(CString strFilePathName, CString strImagePath, int nImgWidth, int nImgHeight)
{
	CReportTableProcess ReportTB;
	
	T_TABLE_D TableD;
	TableD.Initialize();
	
	// TableD에 데이터 채우기
	// header
	TableD.HeaderD.nHeaderRowCount = 1;
	TableD.HeaderD.nHeaderColCount = 5;
	
	T_MERGE_UNIT_D MegerD;
	MegerD.nStartColNumber = 1;
	MegerD.nStartRowNumber = 1;
	MegerD.nMergeCount = 5;
	
	TableD.HeaderD.aMergeColD.Add(MegerD);
	
	CString strImage=_T("");
	strImage.Format(_T("[DRG_IMAGE|width=%d|height=%d]%s[/DRG_IMAGE]"), nImgWidth, nImgHeight, strImagePath);
	TableD.HeaderD.aTitleName.Add(strImage);
	
	// record
	int nRowCount = 6;
	int nColCount = 5;
	TableD.RecordD.nRecordRowCount = nRowCount;
	TableD.RecordD.nRecordColCount = nColCount;
	TableD.RecordD.aRecordData.RemoveAll();
	TableD.RecordD.aRecordData.SetSize(nRowCount);
	
	for(int i=0; i<nRowCount; i++)
		TableD.RecordD.aRecordData[i].SetSize(nColCount);

	TableD.RecordD.aRecordData[0][0] = _LS(IDS_DGN_DYNA_REPORT_Es_Ec);
	TableD.RecordD.aRecordData[0][1] = _LS(IDS_DGN_DYNA_REPORT_Gs_Gc);
	TableD.RecordD.aRecordData[0][2] = _LS(IDS_DGN_DYNA_REPORT_Ds_Dc);
	TableD.RecordD.aRecordData[0][3] = _LS(IDS_DGN_DYNA_REPORT_Ps);
	TableD.RecordD.aRecordData[0][4] = _LS(IDS_DGN_DYNA_REPORT_Pc);
	
	TableD.RecordD.aRecordData[1][0].Format(_T("%.3f"), m_SectTbD.Unit1.EsEc);
	TableD.RecordD.aRecordData[1][1].Format(_T("%.3f"), m_SectTbD.Unit1.GsGc);
	TableD.RecordD.aRecordData[1][2].Format(_T("%.3f"), m_SectTbD.Unit1.DsDc);
	TableD.RecordD.aRecordData[1][3].Format(_T("%.3f"), m_SectTbD.Unit1.Ps  );
	TableD.RecordD.aRecordData[1][4].Format(_T("%.3f"), m_SectTbD.Unit1.Pc  );

	TableD.RecordD.aRecordData[2][0] = _T("A(m2)");
	TableD.RecordD.aRecordData[2][1] = _T("Asy(m2)");
	TableD.RecordD.aRecordData[2][2] = _T("Asz(m2)");
	TableD.RecordD.aRecordData[2][3] = _T("z(+)(m)");
	TableD.RecordD.aRecordData[2][4] = _T("z(-)(m)");
	
	TableD.RecordD.aRecordData[3][0].Format(_T("%.3f"), m_SectTbD.Unit1.A  );
	TableD.RecordD.aRecordData[3][1].Format(_T("%.3f"), m_SectTbD.Unit1.Asy);
	TableD.RecordD.aRecordData[3][2].Format(_T("%.3f"), m_SectTbD.Unit1.Asz);
	TableD.RecordD.aRecordData[3][3].Format(_T("%.3f"), m_SectTbD.Unit1.zp );
	TableD.RecordD.aRecordData[3][4].Format(_T("%.3f"), m_SectTbD.Unit1.zm );
	
	TableD.RecordD.aRecordData[4][0] = _T("Ixx(m4)");
	TableD.RecordD.aRecordData[4][1] = _T("Iyy(m4)");
	TableD.RecordD.aRecordData[4][2] = _T("Izz(m4)");
	TableD.RecordD.aRecordData[4][3] = _T("y(+)(m)");
	TableD.RecordD.aRecordData[4][4] = _T("y(-)(m)");
	
	TableD.RecordD.aRecordData[5][0].Format(_T("%.3f"), m_SectTbD.Unit1.Ixx);
	TableD.RecordD.aRecordData[5][1].Format(_T("%.3f"), m_SectTbD.Unit1.Iyy);
	TableD.RecordD.aRecordData[5][2].Format(_T("%.3f"), m_SectTbD.Unit1.Izz);
	TableD.RecordD.aRecordData[5][3].Format(_T("%.3f"), m_SectTbD.Unit1.yp );
	TableD.RecordD.aRecordData[5][4].Format(_T("%.3f"), m_SectTbD.Unit1.ym );

	// Make xml file
	ReportTB.SetData(TableD);
	if(!ReportTB.GenerateReport(strFilePathName)) 
		return FALSE;
	
	return TRUE;
}

BOOL CDgnAmrSectionTb::GenerateType5(CString strFilePathName, CString strImagePath1, CString strImagePath2, int nImgWidth, int nImgHeight)
{
	CReportTableProcess ReportTB;
	
	T_TABLE_D TableD;
	TableD.Initialize();
	
	// TableD에 데이터 채우기
	// header
	TableD.HeaderD.nHeaderRowCount = 2;
	TableD.HeaderD.nHeaderColCount = 10;
	
	T_MERGE_UNIT_D MegerD;
	MegerD.nStartColNumber = 1;
	MegerD.nStartRowNumber = 1;
	MegerD.nMergeCount = 5;
	TableD.HeaderD.aMergeColD.Add(MegerD);

	MegerD.nStartColNumber = 6;
	MegerD.nStartRowNumber = 1;
	MegerD.nMergeCount = 5;
	TableD.HeaderD.aMergeColD.Add(MegerD);

	MegerD.nStartColNumber = 1;
	MegerD.nStartRowNumber = 2;
	MegerD.nMergeCount = 5;
	TableD.HeaderD.aMergeColD.Add(MegerD);

	MegerD.nStartColNumber = 6;
	MegerD.nStartRowNumber = 2;
	MegerD.nMergeCount = 5;
	TableD.HeaderD.aMergeColD.Add(MegerD);

	CString strImage1=_T(""), strImage2=_T("");
	strImage1.Format(_T("[DRG_IMAGE|width=%d|height=%d]%s[/DRG_IMAGE]"), nImgWidth, nImgHeight, strImagePath1);
	strImage2.Format(_T("[DRG_IMAGE|width=%d|height=%d]%s[/DRG_IMAGE]"), nImgWidth, nImgHeight, strImagePath2);
	
	TableD.HeaderD.aTitleName.Add(_T("Before Composite"));
	TableD.HeaderD.aTitleName.Add(_T("After Composite"));
	TableD.HeaderD.aTitleName.Add(strImage1);
	TableD.HeaderD.aTitleName.Add(strImage2);
	
	// record
	int nRowCount = 6;
	int nColCount = 10;
	TableD.RecordD.nRecordRowCount = nRowCount;
	TableD.RecordD.nRecordColCount = nColCount;
	TableD.RecordD.aRecordData.RemoveAll();
	TableD.RecordD.aRecordData.SetSize(nRowCount);
	
	for(int i=0; i<nRowCount; i++)
		TableD.RecordD.aRecordData[i].SetSize(nColCount);
	
	TableD.RecordD.aRecordData[0][0] = _T("A(m2)");
	TableD.RecordD.aRecordData[0][1] = _T("Asy(m2)");
	TableD.RecordD.aRecordData[0][2] = _T("Asz(m2)");
	TableD.RecordD.aRecordData[0][3] = _T("z(+)(m)");
	TableD.RecordD.aRecordData[0][4] = _T("z(-)(m)");
	TableD.RecordD.aRecordData[0][5] = _T("A(m2)");
	TableD.RecordD.aRecordData[0][6] = _T("Asy(m2)");
	TableD.RecordD.aRecordData[0][7] = _T("Asz(m2)");
	TableD.RecordD.aRecordData[0][8] = _T("z(+)(m)");
	TableD.RecordD.aRecordData[0][9] = _T("z(-)(m)");
	
	TableD.RecordD.aRecordData[1][0].Format(_T("%.3f"), m_SectTbD.Unit1.A  );
	TableD.RecordD.aRecordData[1][1].Format(_T("%.3f"), m_SectTbD.Unit1.Asy);
	TableD.RecordD.aRecordData[1][2].Format(_T("%.3f"), m_SectTbD.Unit1.Asz);
	TableD.RecordD.aRecordData[1][3].Format(_T("%.3f"), m_SectTbD.Unit1.zp );
	TableD.RecordD.aRecordData[1][4].Format(_T("%.3f"), m_SectTbD.Unit1.zm );
	TableD.RecordD.aRecordData[1][5].Format(_T("%.3f"), m_SectTbD.Unit2.A  );
	TableD.RecordD.aRecordData[1][6].Format(_T("%.3f"), m_SectTbD.Unit2.Asy);
	TableD.RecordD.aRecordData[1][7].Format(_T("%.3f"), m_SectTbD.Unit2.Asz);
	TableD.RecordD.aRecordData[1][8].Format(_T("%.3f"), m_SectTbD.Unit2.zp );
	TableD.RecordD.aRecordData[1][9].Format(_T("%.3f"), m_SectTbD.Unit2.zm );
	
	TableD.RecordD.aRecordData[2][0] = _T("Ixx(m4)");
	TableD.RecordD.aRecordData[2][1] = _T("Iyy(m4)");
	TableD.RecordD.aRecordData[2][2] = _T("Izz(m4)");
	TableD.RecordD.aRecordData[2][3] = _T("y(+)(m)");
	TableD.RecordD.aRecordData[2][4] = _T("y(-)(m)");
	TableD.RecordD.aRecordData[2][5] = _T("Ixx(m4)");
	TableD.RecordD.aRecordData[2][6] = _T("Iyy(m4)");
	TableD.RecordD.aRecordData[2][7] = _T("Izz(m4)");
	TableD.RecordD.aRecordData[2][8] = _T("y(+)(m)");
	TableD.RecordD.aRecordData[2][9] = _T("y(-)(m)");
	
	TableD.RecordD.aRecordData[3][0].Format(_T("%.3f"), m_SectTbD.Unit1.Ixx);
	TableD.RecordD.aRecordData[3][1].Format(_T("%.3f"), m_SectTbD.Unit1.Iyy);
	TableD.RecordD.aRecordData[3][2].Format(_T("%.3f"), m_SectTbD.Unit1.Izz);
	TableD.RecordD.aRecordData[3][3].Format(_T("%.3f"), m_SectTbD.Unit1.yp );
	TableD.RecordD.aRecordData[3][4].Format(_T("%.3f"), m_SectTbD.Unit1.ym );
	TableD.RecordD.aRecordData[3][5].Format(_T("%.3f"), m_SectTbD.Unit2.Ixx);
	TableD.RecordD.aRecordData[3][6].Format(_T("%.3f"), m_SectTbD.Unit2.Iyy);
	TableD.RecordD.aRecordData[3][7].Format(_T("%.3f"), m_SectTbD.Unit2.Izz);
	TableD.RecordD.aRecordData[3][8].Format(_T("%.3f"), m_SectTbD.Unit2.yp );
	TableD.RecordD.aRecordData[3][9].Format(_T("%.3f"), m_SectTbD.Unit2.ym );
	
	TableD.RecordD.aRecordData[4][0] = _T("-");
	TableD.RecordD.aRecordData[4][1] = _T("-");
	TableD.RecordD.aRecordData[4][2] = _T("-");
	TableD.RecordD.aRecordData[4][3] = _T("-");
	TableD.RecordD.aRecordData[4][4] = _T("-");
	TableD.RecordD.aRecordData[4][5] = _LS(IDS_DGN_DYNA_REPORT_Es_Ec);
	TableD.RecordD.aRecordData[4][6] = _LS(IDS_DGN_DYNA_REPORT_Gs_Gc);
	TableD.RecordD.aRecordData[4][7] = _LS(IDS_DGN_DYNA_REPORT_Ds_Dc);
	TableD.RecordD.aRecordData[4][8] = _LS(IDS_DGN_DYNA_REPORT_Ps);
	TableD.RecordD.aRecordData[4][9] = _LS(IDS_DGN_DYNA_REPORT_Pc);
	
	TableD.RecordD.aRecordData[5][0].Format(_T("-"));
	TableD.RecordD.aRecordData[5][1].Format(_T("-"));
	TableD.RecordD.aRecordData[5][2].Format(_T("-"));
	TableD.RecordD.aRecordData[5][3].Format(_T("-"));
	TableD.RecordD.aRecordData[5][4].Format(_T("-"));
	TableD.RecordD.aRecordData[5][5].Format(_T("%.3f"), m_SectTbD.Unit2.EsEc);
	TableD.RecordD.aRecordData[5][6].Format(_T("%.3f"), m_SectTbD.Unit2.GsGc);
	TableD.RecordD.aRecordData[5][7].Format(_T("%.3f"), m_SectTbD.Unit2.DsDc);
	TableD.RecordD.aRecordData[5][8].Format(_T("%.3f"), m_SectTbD.Unit2.Ps  );
	TableD.RecordD.aRecordData[5][9].Format(_T("%.3f"), m_SectTbD.Unit2.Pc  );

	// Make xml file
	ReportTB.SetData(TableD);
	if(!ReportTB.GenerateReport(strFilePathName)) 
		return FALSE;
	
	return TRUE;
}

BOOL CDgnAmrSectionTb::GenerateType6(CString strFilePathName, CString strImagePath1, CString strImagePath2, int nImgWidth, int nImgHeight)
{
	CReportTableProcess ReportTB;
	
	T_TABLE_D TableD;
	TableD.Initialize();
	
	// TableD에 데이터 채우기
	// header
	TableD.HeaderD.nHeaderRowCount = 2;
	TableD.HeaderD.nHeaderColCount = 10;
	
	T_MERGE_UNIT_D MegerD;
	MegerD.nStartColNumber = 1;
	MegerD.nStartRowNumber = 1;
	MegerD.nMergeCount = 5;
	TableD.HeaderD.aMergeColD.Add(MegerD);
	
	MegerD.nStartColNumber = 6;
	MegerD.nStartRowNumber = 1;
	MegerD.nMergeCount = 5;
	TableD.HeaderD.aMergeColD.Add(MegerD);
	
	MegerD.nStartColNumber = 1;
	MegerD.nStartRowNumber = 2;
	MegerD.nMergeCount = 5;
	TableD.HeaderD.aMergeColD.Add(MegerD);
	
	MegerD.nStartColNumber = 6;
	MegerD.nStartRowNumber = 2;
	MegerD.nMergeCount = 5;
	TableD.HeaderD.aMergeColD.Add(MegerD);
	
	CString strImage1=_T(""), strImage2=_T("");
	strImage1.Format(_T("[DRG_IMAGE|width=%d|height=%d]%s[/DRG_IMAGE]"), nImgWidth, nImgHeight, strImagePath1);
	strImage2.Format(_T("[DRG_IMAGE|width=%d|height=%d]%s[/DRG_IMAGE]"), nImgWidth, nImgHeight, strImagePath2);
	
	TableD.HeaderD.aTitleName.Add(_T("I-End"));
	TableD.HeaderD.aTitleName.Add(_T("J-End"));
	TableD.HeaderD.aTitleName.Add(strImage1);
	TableD.HeaderD.aTitleName.Add(strImage2);
	
	// record
	int nRowCount = 4;
	int nColCount = 10;
	TableD.RecordD.nRecordRowCount = nRowCount;
	TableD.RecordD.nRecordColCount = nColCount;
	TableD.RecordD.aRecordData.RemoveAll();
	TableD.RecordD.aRecordData.SetSize(nRowCount);
	
	for(int i=0; i<nRowCount; i++)
		TableD.RecordD.aRecordData[i].SetSize(nColCount);
	
	TableD.RecordD.aRecordData[0][0] = _T("A(m2)");
	TableD.RecordD.aRecordData[0][1] = _T("Asy(m2)");
	TableD.RecordD.aRecordData[0][2] = _T("Asz(m2)");
	TableD.RecordD.aRecordData[0][3] = _T("z(+)(m)");
	TableD.RecordD.aRecordData[0][4] = _T("z(-)(m)");
	TableD.RecordD.aRecordData[0][5] = _T("A(m2)");
	TableD.RecordD.aRecordData[0][6] = _T("Asy(m2)");
	TableD.RecordD.aRecordData[0][7] = _T("Asz(m2)");
	TableD.RecordD.aRecordData[0][8] = _T("z(+)(m)");
	TableD.RecordD.aRecordData[0][9] = _T("z(-)(m)");
	
	TableD.RecordD.aRecordData[1][0].Format(_T("%.3f"), m_SectTbD.Unit1.A  );
	TableD.RecordD.aRecordData[1][1].Format(_T("%.3f"), m_SectTbD.Unit1.Asy);
	TableD.RecordD.aRecordData[1][2].Format(_T("%.3f"), m_SectTbD.Unit1.Asz);
	TableD.RecordD.aRecordData[1][3].Format(_T("%.3f"), m_SectTbD.Unit1.zp );
	TableD.RecordD.aRecordData[1][4].Format(_T("%.3f"), m_SectTbD.Unit1.zm );
	TableD.RecordD.aRecordData[1][5].Format(_T("%.3f"), m_SectTbD.Unit2.A  );
	TableD.RecordD.aRecordData[1][6].Format(_T("%.3f"), m_SectTbD.Unit2.Asy);
	TableD.RecordD.aRecordData[1][7].Format(_T("%.3f"), m_SectTbD.Unit2.Asz);
	TableD.RecordD.aRecordData[1][8].Format(_T("%.3f"), m_SectTbD.Unit2.zp );
	TableD.RecordD.aRecordData[1][9].Format(_T("%.3f"), m_SectTbD.Unit2.zm );
	
	TableD.RecordD.aRecordData[2][0] = _T("Ixx(m4)");
	TableD.RecordD.aRecordData[2][1] = _T("Iyy(m4)");
	TableD.RecordD.aRecordData[2][2] = _T("Izz(m4)");
	TableD.RecordD.aRecordData[2][3] = _T("y(+)(m)");
	TableD.RecordD.aRecordData[2][4] = _T("y(-)(m)");
	TableD.RecordD.aRecordData[2][5] = _T("Ixx(m4)");
	TableD.RecordD.aRecordData[2][6] = _T("Iyy(m4)");
	TableD.RecordD.aRecordData[2][7] = _T("Izz(m4)");
	TableD.RecordD.aRecordData[2][8] = _T("y(+)(m)");
	TableD.RecordD.aRecordData[2][9] = _T("y(-)(m)");
	
	TableD.RecordD.aRecordData[3][0].Format(_T("%.3f"), m_SectTbD.Unit1.Ixx);
	TableD.RecordD.aRecordData[3][1].Format(_T("%.3f"), m_SectTbD.Unit1.Iyy);
	TableD.RecordD.aRecordData[3][2].Format(_T("%.3f"), m_SectTbD.Unit1.Izz);
	TableD.RecordD.aRecordData[3][3].Format(_T("%.3f"), m_SectTbD.Unit1.yp );
	TableD.RecordD.aRecordData[3][4].Format(_T("%.3f"), m_SectTbD.Unit1.ym );
	TableD.RecordD.aRecordData[3][5].Format(_T("%.3f"), m_SectTbD.Unit2.Ixx);
	TableD.RecordD.aRecordData[3][6].Format(_T("%.3f"), m_SectTbD.Unit2.Iyy);
	TableD.RecordD.aRecordData[3][7].Format(_T("%.3f"), m_SectTbD.Unit2.Izz);
	TableD.RecordD.aRecordData[3][8].Format(_T("%.3f"), m_SectTbD.Unit2.yp );
	TableD.RecordD.aRecordData[3][9].Format(_T("%.3f"), m_SectTbD.Unit2.ym );

	// Make xml file
	ReportTB.SetData(TableD);
	if(!ReportTB.GenerateReport(strFilePathName)) 
		return FALSE;
	
	return TRUE;
}
*/