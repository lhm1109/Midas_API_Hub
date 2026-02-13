// DgnToolMPhi_SeismicReport.cpp: implementation of the CDgnToolMPhi_SeismicReport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DgnToolMPhi_SeismicReport.h"
 // for Message

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_dblib.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnToolMPhi_SeismicReport::CDgnToolMPhi_SeismicReport()
{
	m_pWord = NULL;

	m_nSub1Num = 0;
	m_nSub2Num = 0;
	m_nSub3Num = 0;
	m_nSub4Num = 0;

	m_pUnitCtrl = CDBDoc::GetDocPoint()->m_pUnitCtrl;

	m_pProDlg = NULL;
	m_nProTotal = m_nProCurr = 0;
}

CDgnToolMPhi_SeismicReport::~CDgnToolMPhi_SeismicReport()
{
	if(m_pWord)
	{
		delete m_pWord;		
		m_pWord	= NULL;	
	}

	m_pUnitCtrl = NULL;
}

void CDgnToolMPhi_SeismicReport::PrintReport(CString strPath, CArray<BOOL, BOOL>& arChk, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, CDgnProgressDlg* pProDlg)
{
	m_pProDlg = pProDlg;

	_DGN_RESERVE_LATERAL_CAPA_RES ResDataCopy = ResData;

	int nChkCnt  = arChk.GetSize();
	int nListCnt = ResData.List.GetSize();
	for(int i=nListCnt-1; i>=0; i--)
	{
		if(i >= nChkCnt)   ResDataCopy.List.RemoveAt(i);
		else if(!arChk[i]) ResDataCopy.List.RemoveAt(i);
		else if(!ResDataCopy.List[i].bIsRes) ResDataCopy.List.RemoveAt(i);
		else continue;
	}

	WriteReport(strPath, ResDataCopy);
	
	m_pProDlg = NULL;//m_pProDlg은 외부에서 생성관리함
	return;
}

void CDgnToolMPhi_SeismicReport::WriteReport()
{
	_DGN_RESERVE_LATERAL_CAPA_RES ResData;
	WriteReport(_T(""), ResData);
}

void CDgnToolMPhi_SeismicReport::WriteReport(CString strFilePath, _DGN_RESERVE_LATERAL_CAPA_RES& ResData)
{
	int nSize = ResData.List.GetSize();
	m_nProTotal = 3 + 2*nSize;
	int nSubSize = (ResData.bEarthquakeTypeI ? 3 : 0) + (ResData.bEarthquakeTypeII ? 3 : 0);
	for(int i=0 ; i<nSize ; i++)
	{
		if(ResData.List[i].bTraverse) m_nProTotal += nSubSize;
		if(ResData.List[i].bLongitude) m_nProTotal += nSubSize;
	}
	m_nProCurr = 0;
	if(m_pProDlg) 
	{ m_pProDlg->Update_ProgressDlg(m_nProCurr,m_nProTotal); }


	// Create Word
    if(CDBLib::IsWordRTF())
	    m_pWord = new CMSWordRTF(FALSE);
    else
        m_pWord = new CMSWord(FALSE);
	
	// Initialize Word
	m_pWord->WordPageSetup();
	m_pWord->SetPageBorder(); 
	m_pWord->SetPageSetUpMargin(1.27, 1.27, 2.0, 1.27);
	m_pWord->SetDefaultTabStop(2.0);
	m_pWord->WritePageNumber();  

#if(_DEBUG)
	m_pWord->SetVisible(TRUE);
#endif
	
	////////// Generate a Report
	// 모델도 bmp로 생성.
	//MakeModelImageBmp();  

	// 표지
	WriteReportCover(m_pWord);
	m_pWord->PageBreak();

	// 목차
	WriteReportTable(m_pWord, ResData);
	m_pWord->PageBreak();

	BOOL bChk = TRUE;
	m_nProCurr = 1;
	if(m_pProDlg) 
	{ if(m_pProDlg->Update_ProgressDlg(m_nProCurr,m_nProTotal))	{ bChk = FALSE; } }

	// 본문
	if(bChk) bChk = WriteReportContents(m_pWord, ResData);

	m_pWord->Finish2();
	if(bChk) m_pWord->SaveAs(strFilePath);
#if(!_DEBUG)
	m_pWord->WindowClose();
#endif

	delete m_pWord;
	m_pWord = NULL;

	return;
}

// 표지 출력
void CDgnToolMPhi_SeismicReport::WriteReportCover(CWordBase* pWord)
{
	pWord->SetSize(FONT_SIZE_COVER_TITLE2);
	pWord->WriteText(_T("\n\n"));
	pWord->SetSize(FONT_SIZE_COVER_TITLE1);
	pWord->SetAlignment(wdAlignTabCenter);
	pWord->SetBold(1);
	pWord->WriteText(_LS(IDS_DGN_RLC_BRIDGE_INTERNAL_DESIGN));

	pWord->SetSize(FONT_SIZE_COVER_TITLE2);
	pWord->WriteText(_T("\n\n\n"));
	pWord->SetUnderline(wdUnderlineSingle);
	pWord->WriteText(_LS(IDS_DGN_RLC_BUILDING_NAME));
	pWord->SetUnderline(wdUnderlineNone);
	pWord->SetBold(0);
	pWord->WriteText(_T("\n\n\n\n\n\n"));

	pWord->SetSize(FONT_SIZE_MAIN_TITLE);
	CTime CurrentTime = CTime::GetCurrentTime();
	CString strCurTime = _T("");
	strCurTime.Format(_LS(IDS_DGN_RLC__dY__dM__dD), CurrentTime.GetYear(), CurrentTime.GetMonth(), CurrentTime.GetDay());
	pWord->WriteText(strCurTime);
	pWord->WriteText(_T("\n"));

	pWord->SetSize(FONT_SIZE_BASE);
	pWord->SetAlignment(wdAlignTabLeft);
}

// 목차 출력
void CDgnToolMPhi_SeismicReport::WriteReportTable(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData)
{
	pWord->SetSize(FONT_SIZE_TABLE_TITLE);
	pWord->SetAlignment(wdAlignTabCenter);
	pWord->SetBold(1);
	pWord->WriteText(_LS(IDS_DGN_RLC_REPORT_TABLE));
	pWord->WriteText(_T("\n\n"));

	pWord->SetAlignment(wdAlignTabLeft);
	pWord->SetBold(0);

	// 1. 설계개요
	m_nSub1Num = m_nSub2Num = m_nSub3Num = m_nSub4Num = 0;
	WriteChap1Title(_LS(IDS_DGN_RLC_DESIGN_SUMMARY), pWord);
	WriteChap2Title(_LS(IDS_DGN_RLC_WORK_SUBJECT), pWord);
	WriteChap2Title(_LS(IDS_DGN_RLC_CONSTRUCTION_NAME), pWord);
	WriteChap2Title(_LS(IDS_DGN_RLC_DESIGN_STANDARD), pWord);
	WriteChap2Title(_LS(IDS_DGN_RLC_DESIGN_CONDITION), pWord);
	pWord->WriteText(_T("\n\n"));

	// 2. 구조모델의 조건
	m_nSub2Num = m_nSub3Num = 0;
	WriteChap1Title(_LS(IDS_DGN_RLC_CONSTRUCTION_MODEL_CONDITION), pWord);
	WriteChap2Title(_LS(IDS_DGN_RLC_MODEL_IMAGE_2D), pWord);
	WriteChap2Title(_LS(IDS_DGN_RLC_MODEL_IMAGE_3D), pWord);
	WriteChap2Title(_LS(IDS_DGN_RLC_MODEL_CONDITION), pWord);
	pWord->WriteText(_T("\n\n"));

	// 3. 해당 지진동에 대한 계산결과
	m_nSub2Num = m_nSub3Num = 0;
	int nListCnt = ResData.List.GetSize();  
	if(nListCnt <= 0)
		return;

	WriteChap1Title(_LS(IDS_DGN_RLC_EQ_CALCULATION_RESULT), pWord);
	CString strTemp=_T("");
	for(int i=0; i<nListCnt; i++)
	{
		strTemp.Format(_T("%s%s"), ResData.List[i].strName, _LS(IDS_DGN_RLC_STABILITY_CALCULATION_RESULT));
		WriteChap2Title(strTemp, pWord);

		m_nSub3Num = 0;
		if(ResData.List[i].bTraverse) 
		{
			WriteChap3Title(_LS(IDS_DGN_RLC_TRANSVERSE_STABILITY_EXAM_RESULT), pWord);
		}
		if(ResData.List[i].bLongitude) 
		{
			WriteChap3Title(_LS(IDS_DGN_RLC_LONGITUDE_STABILITY_EXAM_RESULT), pWord);
		}
		pWord->WriteText(_T("\n"));
	}
	pWord->WriteText(_T("\n"));

	// 4. XXXX의 안정검토
	for(int i=0; i<nListCnt; i++)
	{
		m_nSub2Num = m_nSub3Num = 0;
		strTemp.Format(_T("%s%s"), ResData.List[i].strName, _LS(IDS_DGN_RLC_STABILITY_EXAM));
		WriteChap1Title(strTemp, pWord);
		WriteChap2Title(_LS(IDS_DGN_RLC_MEMBER_CONDITION), pWord);
		WriteChap3Title(_LS(IDS_DGN_RLC_SUBSTITUTE_STRUCTURE), pWord);
		WriteChap3Title(_LS(IDS_DGN_RLC_MATERIAL_CONDITION), pWord);
		WriteChap3Title(_LS(IDS_DGN_RLC_SECTION_SHAPE), pWord);
		pWord->WriteText(_T("\n"));

		if(ResData.bEarthquakeTypeI && ResData.List[i].bTraverse)
		{
			m_nSub3Num = 0;
			WriteChap2Title(_LS(IDS_DGN_RLC_TYPE_1_EQ_TRAN_STABILITY_EXAM), pWord);
			WriteChap3Title(_LS(IDS_DGN_RLC_HORIZONTAL_INTER_FORCE_DISP_PRODUCTION), pWord);
			WriteChap3Title(_LS(IDS_DGN_RLC_SHEAR_INTER_FORCE_PRODUCTION), pWord);
			WriteChap3Title(_LS(IDS_DGN_RLC_DESTRUCTION_FORM_DECISION), pWord);
			WriteChap3Title(_LS(IDS_DGN_RLC_EQ_HORI_INTER_EXAM), pWord);
			if(ResData.nDgnLevel==1)
				WriteChap3Title(_LS(IDS_DGN_RLC_REMAINING_DISP_EXAM), pWord);
			pWord->WriteText(_T("\n"));
		}
		if(ResData.bEarthquakeTypeII && ResData.List[i].bTraverse)
		{
			m_nSub3Num = 0;
			WriteChap2Title(_LS(IDS_DGN_RLC_TYPE_2_EQ_TRAN_STABILITY_EXAM), pWord);
			WriteChap3Title(_LS(IDS_DGN_RLC_HORIZONTAL_INTER_FORCE_DISP_PRODUCTION), pWord);
			WriteChap3Title(_LS(IDS_DGN_RLC_SHEAR_INTER_FORCE_PRODUCTION), pWord);
			WriteChap3Title(_LS(IDS_DGN_RLC_DESTRUCTION_FORM_DECISION), pWord);
			WriteChap3Title(_LS(IDS_DGN_RLC_EQ_HORI_INTER_EXAM), pWord);
			if(ResData.nDgnLevel==1)
				WriteChap3Title(_LS(IDS_DGN_RLC_REMAINING_DISP_EXAM), pWord);
			pWord->WriteText(_T("\n"));
		}
		if(ResData.bEarthquakeTypeI && ResData.List[i].bLongitude)
		{
			m_nSub3Num = 0;
			WriteChap2Title(_LS(IDS_DGN_RLC_TYPE_1_EQ_LONG_STABILITY_EXAM), pWord);
			WriteChap3Title(_LS(IDS_DGN_RLC_HORIZONTAL_INTER_FORCE_DISP_PRODUCTION), pWord);
			WriteChap3Title(_LS(IDS_DGN_RLC_SHEAR_INTER_FORCE_PRODUCTION), pWord);
			WriteChap3Title(_LS(IDS_DGN_RLC_DESTRUCTION_FORM_DECISION), pWord);
			WriteChap3Title(_LS(IDS_DGN_RLC_EQ_HORI_INTER_EXAM), pWord);
			if(ResData.nDgnLevel==1)
				WriteChap3Title(_LS(IDS_DGN_RLC_REMAINING_DISP_EXAM), pWord);
			pWord->WriteText(_T("\n"));
		}
		if(ResData.bEarthquakeTypeII && ResData.List[i].bLongitude)
		{
			m_nSub3Num = 0;
			WriteChap2Title(_LS(IDS_DGN_RLC_TYPE_2_EQ_LONG_STABILITY_EXAM), pWord);
			WriteChap3Title(_LS(IDS_DGN_RLC_HORIZONTAL_INTER_FORCE_DISP_PRODUCTION), pWord);
			WriteChap3Title(_LS(IDS_DGN_RLC_SHEAR_INTER_FORCE_PRODUCTION), pWord);
			WriteChap3Title(_LS(IDS_DGN_RLC_DESTRUCTION_FORM_DECISION), pWord);
			WriteChap3Title(_LS(IDS_DGN_RLC_EQ_HORI_INTER_EXAM), pWord);
			if(ResData.nDgnLevel==1)
				WriteChap3Title(_LS(IDS_DGN_RLC_REMAINING_DISP_EXAM), pWord);
			pWord->WriteText(_T("\n"));
		}
		pWord->WriteText(_T("\n"));
	}

	pWord->SetSize(FONT_SIZE_BASE);
}

// 본문 출력
BOOL CDgnToolMPhi_SeismicReport::WriteReportContents(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData)
{	
	////////////////////////////////////////////////////////////////////////////
	// 1. 설계개요
	m_nSub1Num = 0;
	m_nSub2Num = 0;
	m_nSub3Num = 0;

	WriteSub1Title(_LS(IDS_DGN_RLC_DESIGN_SUMMARY), pWord);
	
	CString strTemp=_T("");
	strTemp.Format(_T("%s : %s"), _LS(IDS_DGN_RLC_WORK_SUBJECT), ResData.strComp);
	WriteSub2Title(strTemp, pWord);
	pWord->WriteText(_T("\n"));
	
	strTemp.Format(_T("%s : %s"), _LS(IDS_DGN_RLC_CONSTRUCTION_NAME), ResData.strWork);
	WriteSub2Title(strTemp, pWord);
	pWord->WriteText(_T("\n"));
	
	strTemp.Format(_T("%s : %s"), _LS(IDS_DGN_RLC_DESIGN_STANDARD), ResData.strCode);
	WriteSub2Title(strTemp, pWord);
	pWord->WriteText(_T("\n"));
	
	WriteSub2Title(_LS(IDS_DGN_RLC_DESIGN_CONDITION), pWord);
	WriteDesignConditionTable(pWord, ResData);
	m_pWord->PageBreak();

	// Progress ===============================================================	
	m_nProCurr = 2;
	if(m_pProDlg)
	{ if(m_pProDlg->Update_ProgressDlg(m_nProCurr,m_nProTotal))	{ return FALSE; } }
	//=========================================================================

	////////////////////////////////////////////////////////////////////////////
	// 2. 구조모델의 조건
	m_nSub2Num = 0;
	m_nSub3Num = 0;

	WriteSub1Title(_LS(IDS_DGN_RLC_CONSTRUCTION_MODEL_CONDITION), pWord);
	WriteSub2Title(_LS(IDS_DGN_RLC_MODEL_IMAGE_2D), pWord);

	const int iCol = 1;
	float pColWidth[iCol] = { (float)350 };
	pWord->MakeTableExKR(1, iCol, pColWidth, 1);
	pWord->SetTableBorder(wdBorderTop   , wdLineStyleNone);
	pWord->SetTableBorder(wdBorderLeft  , wdLineStyleNone);
	pWord->SetTableBorder(wdBorderBottom, wdLineStyleNone);
	pWord->SetTableBorder(wdBorderRight , wdLineStyleNone);

	if(ResData.str2DimFileName!=_T(""))
		pWord->AddPicture(ResData.str2DimFileName);
	pWord->WriteText(_T("\n"));
	pWord->SetAlignment(wdAlignTabCenter);
	pWord->SetSize(FONT_SIZE_SMALL_TEXT);
	strTemp.Format(_LS(IDS_DGN_RLC_IMG__d__d_MODEL_IMAGE_2D), m_nSub1Num, m_nSub2Num);
	pWord->WriteText(strTemp);
	pWord->MoveDown();
	pWord->SetSize(FONT_SIZE_BASE);
	pWord->WriteText(_T("\n"));
	pWord->SetAlignment(wdAlignTabLeft);

	WriteSub2Title(_LS(IDS_DGN_RLC_MODEL_IMAGE_3D), pWord);
	pWord->MakeTableExKR(1, iCol, pColWidth, 1);
	pWord->SetTableBorder(wdBorderTop   , wdLineStyleNone);
	pWord->SetTableBorder(wdBorderLeft  , wdLineStyleNone);
	pWord->SetTableBorder(wdBorderBottom, wdLineStyleNone);
	pWord->SetTableBorder(wdBorderRight , wdLineStyleNone);

	if(ResData.str3DimFileName!=_T(""))
		pWord->AddPicture(ResData.str3DimFileName);
	pWord->WriteText(_T("\n"));
	pWord->SetAlignment(wdAlignTabCenter);
	pWord->SetSize(FONT_SIZE_SMALL_TEXT);
	strTemp.Format(_LS(IDS_DGN_RLC_IMG__d__d_MODEL_IMAGE_3D), m_nSub1Num, m_nSub2Num);
	pWord->WriteText(strTemp);
	pWord->MoveDown();
	pWord->SetSize(FONT_SIZE_BASE);
	pWord->WriteText(_T("\n"));
	pWord->SetAlignment(wdAlignTabLeft);

	WriteSub2Title(_LS(IDS_DGN_RLC_MODEL_CONDITION), pWord);
	WriteModelConditionTable(m_pWord, ResData);
	m_pWord->PageBreak();

	// Progress ===============================================================	
	m_nProCurr = 3;
	if(m_pProDlg)
	{
		m_pProDlg->m_strName.Format(_LS(IDS_DGN_RLC_REPORT_PRODUCING), _LS(IDS_DGN_RLC_EQ_CALCULATION_RESULT));
		if(m_pProDlg->Update_ProgressDlg(m_nProCurr,m_nProTotal))	{ return FALSE; }
	}
	//=========================================================================

	////////////////////////////////////////////////////////////////////////////
	// 3. 해당 지진동에 대한 계산결과
	m_nSub2Num = 0;
	m_nSub3Num = 0;

	int nListCnt = ResData.List.GetSize();  
	if(nListCnt <= 0)
		return FALSE;

	WriteSub1Title(_LS(IDS_DGN_RLC_EQ_CALCULATION_RESULT), pWord);
	for(int i=0; i<nListCnt; i++)
	{
		if(i>0)
			m_pWord->PageBreak();

		m_nSub3Num = 0;
		strTemp.Format(_T("%s%s"), ResData.List[i].strName, _LS(IDS_DGN_RLC_STABILITY_CALCULATION_RESULT));
		WriteSub2Title(strTemp, pWord);

		// 교축방향
		if(ResData.List[i].bTraverse)
		{
			WriteSub3Title(_LS(IDS_DGN_RLC_TRANSVERSE_STABILITY_EXAM_RESULT), pWord, FALSE);
			pWord->SetSize(FONT_SIZE_TABLE_TEXT);
			pWord->WriteText(_T("\t\t\t\t\t\t\t\t\t\t\t\t\t(kN, m)\n"));
			pWord->SetSize(FONT_SIZE_BASE);
			
			if(ResData.bEarthquakeTypeI && ResData.bEarthquakeTypeII)
				WriteExamResultTable1(pWord, ResData, 0, i);
			else if(ResData.bEarthquakeTypeI && !ResData.bEarthquakeTypeII)
				WriteExamResultTable2(pWord, ResData, 0, i, 0);
			else if(!ResData.bEarthquakeTypeI && ResData.bEarthquakeTypeII)
				WriteExamResultTable2(pWord, ResData, 0, i, 1);
			else  { ASSERT(0); }

			pWord->WriteText(_T("\n"));
		}

		// 교축직각방향
		if(ResData.List[i].bLongitude)
		{
			WriteSub3Title(_LS(IDS_DGN_RLC_LONGITUDE_STABILITY_EXAM_RESULT), pWord, FALSE);
			pWord->SetSize(FONT_SIZE_TABLE_TEXT);
			pWord->WriteText(_T("\t\t\t\t\t\t\t\t\t\t\t\t(kN, m)\n"));
			pWord->SetSize(FONT_SIZE_BASE);

			if(ResData.bEarthquakeTypeI && ResData.bEarthquakeTypeII)
				WriteExamResultTable1(pWord, ResData, 1, i);
			else if(ResData.bEarthquakeTypeI && !ResData.bEarthquakeTypeII)
				WriteExamResultTable2(pWord, ResData, 1, i, 0);
			else if(!ResData.bEarthquakeTypeI && ResData.bEarthquakeTypeII)
				WriteExamResultTable2(pWord, ResData, 1, i, 1);
			else  { ASSERT(0); }

			pWord->WriteText(_T("\n"));
		}
		
		// Progress ===============================================================	
		m_nProCurr++;
		if(m_pProDlg)
		{ if(m_pProDlg->Update_ProgressDlg(m_nProCurr,m_nProTotal))	{ return FALSE; }	}
		//=========================================================================
	}
	m_pWord->PageBreak();

	////////////////////////////////////////////////////////////////////////////
	// 4. XXXX의 안정검토 이후..
	// List 개수만큼 반복.
	for(int i=0; i<nListCnt; i++)
	{ 		
		// Progress ===============================================================	
		if(m_pProDlg)
		{ 
			m_pProDlg->m_strName.Format(_LS(IDS_DGN_RLC_REPORT_PRODUCING), ResData.List[i].strName);
			if(m_pProDlg->Update_ProgressDlg(m_nProCurr,m_nProTotal))	{ return FALSE; }
		}
		//=========================================================================
		m_nSub2Num = 0;
		m_nSub3Num = 0;

		strTemp.Format(_T("%s%s"), ResData.List[i].strName, _LS(IDS_DGN_RLC_STABILITY_EXAM));
		WriteSub1Title(strTemp, pWord);

		WriteSub2Title(_LS(IDS_DGN_RLC_MEMBER_CONDITION), pWord);
		WriteSub3Title(_LS(IDS_DGN_RLC_SUBSTITUTE_STRUCTURE), pWord);
		
		if(ResData.List[i].bTraverse && ResData.List[i].bLongitude)
			WriteStructureTable2(pWord, ResData, i);
		else if(ResData.List[i].bTraverse)
			WriteStructureTable1(pWord, ResData, 0, i);
		else if(ResData.List[i].bLongitude)
			WriteStructureTable1(pWord, ResData, 1, i);
		pWord->WriteText(_T("\n"));

		WriteSub3Title(_LS(IDS_DGN_RLC_MATERIAL_CONDITION), pWord);
		if(ResData.List[i].bTraverse && ResData.List[i].bLongitude)
			WriteMaterialConditionTable2(pWord, ResData, i);
		else if(ResData.List[i].bTraverse)
			WriteMaterialConditionTable1(pWord, ResData, 0, i);
		else if(ResData.List[i].bLongitude)
			WriteMaterialConditionTable1(pWord, ResData, 1, i);
		m_pWord->PageBreak();

		WriteSub3Title(_LS(IDS_DGN_RLC_SECTION_SHAPE), pWord);

		const int iCol = 1;
		float pColWidth[iCol] = { (float)200 };
		pWord->MakeTableExKR(1, iCol, pColWidth, 1);
		pWord->SetTableBorder(wdBorderTop   , wdLineStyleNone);
		pWord->SetTableBorder(wdBorderLeft  , wdLineStyleNone);
		pWord->SetTableBorder(wdBorderBottom, wdLineStyleNone);
		pWord->SetTableBorder(wdBorderRight , wdLineStyleNone);

		//ResData.List[i].strFilePath_ShapeFig = _T("D:\\test2.bmp");  // for image test
		pWord->AddPicture(ResData.List[i].strFilePath_ShapeFig);

		pWord->MoveDown();
		pWord->WriteText(_T("\n"));

		// Progress ===============================================================	
		m_nProCurr++;
		if(m_pProDlg)
		{ if(m_pProDlg->Update_ProgressDlg(m_nProCurr,m_nProTotal))	{ return FALSE; } }
		//=========================================================================
		
		if(ResData.bEarthquakeTypeI && ResData.List[i].bTraverse)
		{
			m_pWord->PageBreak();

			m_nSub3Num = 0;
			WriteSub2Title(_LS(IDS_DGN_RLC_TYPE_1_EQ_TRAN_STABILITY_EXAM), pWord);

			WriteSub3Title(_LS(IDS_DGN_RLC_HORIZONTAL_INTER_FORCE_DISP_PRODUCTION), pWord);
			WriteHorizontalForce(pWord, ResData, 0, i, 0);

			// Progress ===============================================================	
			m_nProCurr++;
			if(m_pProDlg)
			{ if(m_pProDlg->Update_ProgressDlg(m_nProCurr,m_nProTotal))	{ return FALSE; } }
			//=========================================================================

			WriteSub3Title(_LS(IDS_DGN_RLC_SHEAR_INTER_FORCE_PRODUCTION), pWord);
			WriteShearForce(pWord, ResData, 0, i, 0);

			WriteSub3Title(_LS(IDS_DGN_RLC_DESTRUCTION_FORM_DECISION), pWord);
			WriteFailureType(pWord, ResData, 0, i, 0);      
			m_pWord->PageBreak();

			
			// Progress ===============================================================	
			m_nProCurr++;
			if(m_pProDlg)
			{ if(m_pProDlg->Update_ProgressDlg(m_nProCurr,m_nProTotal))	{ return FALSE; } }
			//=========================================================================

			WriteSub3Title(_LS(IDS_DGN_RLC_EQ_HORI_INTER_EXAM), pWord);
			WriteEarthquakeExam(pWord, ResData, 0, i, 0);      

			if(ResData.nDgnLevel==1)
			{
				m_pWord->PageBreak();
				WriteSub3Title(_LS(IDS_DGN_RLC_REMAINING_DISP_EXAM), pWord);
				WriteRemainingDispExam(pWord, ResData, 0, i, 0);
			}			
			
			// Progress ===============================================================	
			m_nProCurr++;
			if(m_pProDlg)
			{ if(m_pProDlg->Update_ProgressDlg(m_nProCurr,m_nProTotal))	{ return FALSE; } }
			//=========================================================================
		}
	 if(ResData.bEarthquakeTypeII && ResData.List[i].bTraverse)
		{
			m_pWord->PageBreak();

			m_nSub3Num = 0;
			WriteSub2Title(_LS(IDS_DGN_RLC_TYPE_2_EQ_TRAN_STABILITY_EXAM), pWord);
			
			WriteSub3Title(_LS(IDS_DGN_RLC_HORIZONTAL_INTER_FORCE_DISP_PRODUCTION), pWord);
			WriteHorizontalForce(pWord, ResData, 0, i, 1);

			// Progress ===============================================================	
			m_nProCurr++;
			if(m_pProDlg)
			{ if(m_pProDlg->Update_ProgressDlg(m_nProCurr,m_nProTotal))	{ return FALSE; } }
			//=========================================================================
			
			WriteSub3Title(_LS(IDS_DGN_RLC_SHEAR_INTER_FORCE_PRODUCTION), pWord);
			WriteShearForce(pWord, ResData, 0, i, 1);
			
			WriteSub3Title(_LS(IDS_DGN_RLC_DESTRUCTION_FORM_DECISION), pWord);
			WriteFailureType(pWord, ResData, 0, i, 1);
			m_pWord->PageBreak();
			
			// Progress ===============================================================	
			m_nProCurr++;
			if(m_pProDlg)
			{ if(m_pProDlg->Update_ProgressDlg(m_nProCurr,m_nProTotal))	{ return FALSE; } }
			//=========================================================================

			WriteSub3Title(_LS(IDS_DGN_RLC_EQ_HORI_INTER_EXAM), pWord);
			WriteEarthquakeExam(pWord, ResData, 0, i, 1);
			
			if(ResData.nDgnLevel==1)
			{
				m_pWord->PageBreak();
				WriteSub3Title(_LS(IDS_DGN_RLC_REMAINING_DISP_EXAM), pWord);
				WriteRemainingDispExam(pWord, ResData, 0, i, 1);
			}
			
			// Progress ===============================================================	
			m_nProCurr++;
			if(m_pProDlg)
			{ if(m_pProDlg->Update_ProgressDlg(m_nProCurr,m_nProTotal))	{ return FALSE; } }
			//=========================================================================
		}
		if(ResData.bEarthquakeTypeI && ResData.List[i].bLongitude)
		{
			m_pWord->PageBreak();

			m_nSub3Num = 0;
			WriteSub2Title(_LS(IDS_DGN_RLC_TYPE_1_EQ_LONG_STABILITY_EXAM), pWord);
			
			WriteSub3Title(_LS(IDS_DGN_RLC_HORIZONTAL_INTER_FORCE_DISP_PRODUCTION), pWord);
			WriteHorizontalForce(pWord, ResData, 1, i, 0);
			
			// Progress ===============================================================	
			m_nProCurr++;
			if(m_pProDlg)
			{ if(m_pProDlg->Update_ProgressDlg(m_nProCurr,m_nProTotal))	{ return FALSE; } }
			//=========================================================================
			
			WriteSub3Title(_LS(IDS_DGN_RLC_SHEAR_INTER_FORCE_PRODUCTION), pWord);
			WriteShearForce(pWord, ResData, 1, i, 0);
			
			WriteSub3Title(_LS(IDS_DGN_RLC_DESTRUCTION_FORM_DECISION), pWord);
			WriteFailureType(pWord, ResData, 1, i, 0);      
			m_pWord->PageBreak();
			
			// Progress ===============================================================	
			m_nProCurr++;
			if(m_pProDlg)
			{ if(m_pProDlg->Update_ProgressDlg(m_nProCurr,m_nProTotal))	{ return FALSE; } }
			//=========================================================================
			
			WriteSub3Title(_LS(IDS_DGN_RLC_EQ_HORI_INTER_EXAM), pWord);
			WriteEarthquakeExam(pWord, ResData, 1, i, 0);
			
			if(ResData.nDgnLevel==1)
			{
				m_pWord->PageBreak();
				WriteSub3Title(_LS(IDS_DGN_RLC_REMAINING_DISP_EXAM), pWord);
				WriteRemainingDispExam(pWord, ResData, 1, i, 0);
			}
			
			// Progress ===============================================================	
			m_nProCurr++;
			if(m_pProDlg)
			{ if(m_pProDlg->Update_ProgressDlg(m_nProCurr,m_nProTotal))	{ return FALSE; } }
			//=========================================================================
		}
		if(ResData.bEarthquakeTypeII && ResData.List[i].bLongitude)
		{
			m_pWord->PageBreak();

			m_nSub3Num = 0;
			WriteSub2Title(_LS(IDS_DGN_RLC_TYPE_2_EQ_LONG_STABILITY_EXAM), pWord);
			
			WriteSub3Title(_LS(IDS_DGN_RLC_HORIZONTAL_INTER_FORCE_DISP_PRODUCTION), pWord);
			WriteHorizontalForce(pWord, ResData, 1, i, 1);
			
			// Progress ===============================================================	
			m_nProCurr++;
			if(m_pProDlg)
			{ if(m_pProDlg->Update_ProgressDlg(m_nProCurr,m_nProTotal))	{ return FALSE; } }
			//=========================================================================
			
			WriteSub3Title(_LS(IDS_DGN_RLC_SHEAR_INTER_FORCE_PRODUCTION), pWord);
			WriteShearForce(pWord, ResData, 1, i, 1);
			
			WriteSub3Title(_LS(IDS_DGN_RLC_DESTRUCTION_FORM_DECISION), pWord);
			WriteFailureType(pWord, ResData, 1, i, 1);
			m_pWord->PageBreak();
			
			// Progress ===============================================================	
			m_nProCurr++;
			if(m_pProDlg)
			{ if(m_pProDlg->Update_ProgressDlg(m_nProCurr,m_nProTotal))	{ return FALSE; } }
			//=========================================================================
			
			WriteSub3Title(_LS(IDS_DGN_RLC_EQ_HORI_INTER_EXAM), pWord);
			WriteEarthquakeExam(pWord, ResData, 1, i, 1);
			
			if(ResData.nDgnLevel==1)
			{
				m_pWord->PageBreak();
				WriteSub3Title(_LS(IDS_DGN_RLC_REMAINING_DISP_EXAM), pWord);
				WriteRemainingDispExam(pWord, ResData, 1, i, 1);
			}
			
			// Progress ===============================================================	
			m_nProCurr++;
			if(m_pProDlg)
			{ if(m_pProDlg->Update_ProgressDlg(m_nProCurr,m_nProTotal))	{ return FALSE; } }
			//=========================================================================
		}
		if(i<nListCnt-1)
			m_pWord->PageBreak();
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Word Style Function

void CDgnToolMPhi_SeismicReport::WriteSub1Title(CString strTitle, CWordBase* pWord, BOOL bBold/*=FALSE*/)
{
	pWord->SetSize(FONT_SIZE_CHAPTER_TITLE1);
	if(bBold) pWord->SetBold(1);  
	CString strSubNum=_T("");
	++m_nSub1Num;
	strSubNum.Format(_T("%d. "), m_nSub1Num);
	pWord->WriteText(strSubNum);
	pWord->WriteText(strTitle);
	pWord->WriteText(_T("\n"));
	pWord->SetSize(FONT_SIZE_BASE);  
	pWord->SetBold(0);
}

void CDgnToolMPhi_SeismicReport::WriteSub2Title(CString strTitle,CWordBase* pWord, BOOL bTab/*=TRUE*/, BOOL bBold/*=FALSE*/)
{
	pWord->SetSize(FONT_SIZE_CHAPTER_TITLE1);
	if(bBold) pWord->SetBold(1);
	if(bTab) pWord->WriteText(_T("\t"));
	CString strSubNum=_T("");
	++m_nSub2Num;
	strSubNum.Format(_T("%d.%d "), m_nSub1Num, m_nSub2Num);
	pWord->WriteText(strSubNum);
	pWord->WriteText(strTitle);
	pWord->WriteText(_T("\n"));
	pWord->SetSize(FONT_SIZE_BASE);  
	pWord->SetBold(0);
}

void CDgnToolMPhi_SeismicReport::WriteSub3Title(CString strTitle, CWordBase* pWord, BOOL bNewline/*=TRUE*/, BOOL bTab/*=TRUE*/, BOOL bBold/*=FALSE*/)
{
	pWord->SetSize(FONT_SIZE_CHAPTER_TITLE2);
	if(bBold) pWord->SetBold(1);
	if(bTab) pWord->WriteText(_T("\t\t"));
	else pWord->WriteText(_T("\t"));
	CString strSubNum=_T("");
	++m_nSub3Num;
	strSubNum.Format(_T("%d.%d.%d "), m_nSub1Num, m_nSub2Num, m_nSub3Num);
	pWord->WriteText(strSubNum);
	pWord->WriteText(strTitle);
	if(bNewline)  pWord->WriteText(_T("\n"));
	pWord->SetSize(FONT_SIZE_BASE);  
	pWord->SetBold(0);
}

void CDgnToolMPhi_SeismicReport::WriteSub4Title(CString strTitle, CWordBase* pWord, BOOL bTab/*=FALSE*/, BOOL bBold/*=FALSE*/)
{
	pWord->SetSize(FONT_SIZE_CHAPTER_TITLE2);
	if(bBold) pWord->SetBold(1);
	if(bTab) pWord->WriteText(_T("\t\t\t"));
	else pWord->WriteText(_T("\t\t"));
	CString strSubNum=_T("");
	++m_nSub4Num;
	strSubNum.Format(_T("(%d) "), m_nSub4Num);
	pWord->WriteText(strSubNum);
	pWord->WriteText(strTitle);
	pWord->WriteText(_T("\n"));
	pWord->SetSize(FONT_SIZE_BASE);  
	pWord->SetBold(0);
}

void CDgnToolMPhi_SeismicReport::WriteChap1Title(CString strTitle, CWordBase* pWord)
{
	pWord->SetSize(FONT_SIZE_CHAPTER_TITLE1);
	CString strSubNum=_T("");
	++m_nSub1Num;
	strSubNum.Format(_T("%d. "), m_nSub1Num);
	pWord->WriteText(_T("\t\t\t\t"));
	pWord->WriteText(strSubNum);
	pWord->WriteText(strTitle);
	pWord->WriteText(_T("\n"));
	pWord->SetSize(FONT_SIZE_BASE);  
}

void CDgnToolMPhi_SeismicReport::WriteChap2Title(CString strTitle, CWordBase* pWord)
{
	pWord->SetSize(FONT_SIZE_CHAPTER_TITLE1);
	CString strSubNum=_T("");
	++m_nSub2Num;
	strSubNum.Format(_T("%d.%d "), m_nSub1Num, m_nSub2Num);
	pWord->WriteText(_T("\t\t\t\t  "));
	pWord->WriteText(strSubNum);
	pWord->WriteText(strTitle);
	pWord->WriteText(_T("\n"));
	pWord->SetSize(FONT_SIZE_BASE);  
}

void CDgnToolMPhi_SeismicReport::WriteChap3Title(CString strTitle, CWordBase* pWord)
{
	pWord->SetSize(FONT_SIZE_CHAPTER_TITLE2);
	CString strSubNum=_T("");
	++m_nSub3Num;
	strSubNum.Format(_T("%d.%d.%d "), m_nSub1Num, m_nSub2Num, m_nSub3Num);
	pWord->WriteText(_T("\t\t\t\t    "));
	pWord->WriteText(strSubNum);
	pWord->WriteText(strTitle);
	pWord->WriteText(_T("\n"));
	pWord->SetSize(FONT_SIZE_BASE);  
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Report Data Function

void CDgnToolMPhi_SeismicReport::WriteDesignConditionTable(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData)
{
	pWord->SetSize(FONT_SIZE_TABLE_TEXT);
	pWord->SetAlignment(wdAlignTabCenter);

	const int iCol = 4;
	float pColWidth[iCol] = { (float)30, (float)180, (float)180, (float)70 };
	pWord->MakeTableExKR(8, iCol, pColWidth, 1);

	int iColorIndexGray = 16;  /*wdGray25*/
 
	pWord->SetCellShading(1, 1, iColorIndexGray);

	pWord->MoveRight();
	pWord->SetCellShading(1, 2, iColorIndexGray);
	pWord->MergeCells(1, 2);
	pWord->WriteCell(1, 2, _LS(IDS_DGN_RLC_DESIGN_CONDITION));

	pWord->MoveRight();
	pWord->SetCellShading(1, 3, iColorIndexGray);
	pWord->WriteCell(1, 3, _LS(IDS_DGN_RLC_REMARK));

	int nRowNum=0;
	CString strRowNum=_T("");
	strRowNum.Format(_T("%d"), ++nRowNum);

	pWord->MoveRight();
	pWord->SetCellShading(2, 1, iColorIndexGray);
	pWord->WriteCell(2, 1, strRowNum);

	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(2, 2, _LS(IDS_DGN_RLC_DESIGN_PROCESS));

	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(2, 3, ResData.strAnalysis);

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(3, 1, iColorIndexGray);
	strRowNum.Format(_T("%d"), ++nRowNum);
	pWord->WriteCell(3, 1, strRowNum);

	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(3, 2, _LS(IDS_DGN_RLC_BRIDGE_TYPE));
	
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(3, 3, ResData.strBridgeType);

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(4, 1, iColorIndexGray);
	strRowNum.Format(_T("%d"), ++nRowNum);
	pWord->WriteCell(4, 1, strRowNum);
	
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(4, 2, _LS(IDS_DGN_RLC_REGION_DIVISION));
	
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(4, 3, ResData.strRegionalKind);

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(5, 1, iColorIndexGray);
	strRowNum.Format(_T("%d"), ++nRowNum);
	pWord->WriteCell(5, 1, strRowNum);
	
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(5, 2, _LS(IDS_DGN_RLC_INTER_EXAM_DESIGN_GROUND_CLASS));
	
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(5, 3, ResData.strGroundKind);

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(6, 1, iColorIndexGray);
	strRowNum.Format(_T("%d"), ++nRowNum);
	pWord->WriteCell(6, 1, strRowNum);
	
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(6, 2, _LS(IDS_DGN_RLC_INTER_EXAM_PERFORMANCE));
	
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(6, 3, ResData.strDgnLevel);
	
	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(7, 1, iColorIndexGray);
	strRowNum.Format(_T("%d"), ++nRowNum);
	pWord->WriteCell(7, 1, strRowNum);
	
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(7, 2, _LS(IDS_DGN_RLC_EQ_TYPE));
	
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	CString strTemp=_T("");
	if(ResData.bEarthquakeTypeI && ResData.bEarthquakeTypeII)
	{
		strTemp = _LS(IDS_DGN_RLC_TYPE1_TYPE2);
	}
	else if(ResData.bEarthquakeTypeI)
	{
		strTemp = _LS(IDS_DGN_RLC_TYPE1);
	}
	else if(ResData.bEarthquakeTypeII)
	{
		strTemp = _LS(IDS_DGN_RLC_TYPE2);
	}
	else
	{
		strTemp = _T("");
	}
	pWord->WriteCell(7, 3, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(8, 1, iColorIndexGray);
	strRowNum.Format(_T("%d"), ++nRowNum);
	pWord->WriteCell(8, 1, strRowNum);
	
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(8, 2, _LS(IDS_DGN_RLC_STOCK_DECISION));
	
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);

	pWord->MoveDown();
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteText(_T("\n"));
	pWord->SetSize(FONT_SIZE_BASE);
}

void CDgnToolMPhi_SeismicReport::WriteModelConditionTable(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData)
{
	pWord->SetSize(FONT_SIZE_TABLE_TEXT);
	pWord->SetAlignment(wdAlignTabCenter);

	const int iCol = 6;
	float pColWidth[iCol] = { (float)60, (float)110, (float)80, (float)70, (float)70, (float)70 };
	int nListCnt = ResData.List.GetSize();
	pWord->MakeTableExKR(nListCnt+1, iCol, pColWidth, 1);

	int iColorIndexGray = 16;  /*wdGray25*/
 
	pWord->SetCellShading(1, 1, iColorIndexGray);
	pWord->WriteCell(1, 1, _LS(IDS_DGN_RLC_NAME));

	pWord->MoveRight();
	pWord->SetCellShading(1, 2, iColorIndexGray);
	pWord->WriteCell(1, 2, _LS(IDS_DGN_RLC_DECISION_DISTANCE));

	pWord->MoveRight();
	pWord->SetCellShading(1, 3, iColorIndexGray);
	pWord->WriteCell(1, 3, _LS(IDS_DGN_RLC_COLUMN_DIVISIOIN_COUNT));

	pWord->MoveRight();
	pWord->SetCellShading(1, 4, iColorIndexGray);
	pWord->WriteCell(1, 4, _LS(IDS_DGN_RLC_SECTION_SHAPE));

	pWord->MoveRight();
	pWord->SetCellShading(1, 5, iColorIndexGray);
	pWord->WriteCell(1, 5, _LS(IDS_DGN_RLC_WIDTH_cm));

	pWord->MoveRight();
	pWord->SetCellShading(1, 6, iColorIndexGray);
	pWord->WriteCell(1, 6, _LS(IDS_DGN_RLC_HEIGHT_cm));

	CString strTmp=_T("");
	double  dSrcLength=0.0, dSrcWidth=0.0, dSrcHeight=0.0;
	double  dTgtLength=0.0,dTgtWidth=0.0, dTgtHeight=0.0;
	for(int i=0; i<nListCnt; i++)
	{
		pWord->MoveRight();
		pWord->WriteCell(2+i, 1, ResData.List[i].strName);

		dSrcLength = ResData.List[i].dLength;
		dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																												D_UNITSYS_BASE_LENGTH, dSrcLength);
		pWord->MoveRight();
		strTmp.Format(_T("%.3f ~ %.3f"), 0.000, dTgtLength);
		pWord->WriteCell(2+i, 2, strTmp);

		pWord->MoveRight();
		strTmp.Format(_T("%d"), ResData.List[i].nDivSize);
		pWord->WriteCell(2+i, 3, strTmp);

		pWord->MoveRight();
		pWord->WriteCell(2+i, 4, ResData.List[i].strShape);

		dSrcWidth   = ResData.List[i].dWidth;
		dSrcHeight  = ResData.List[i].dHeight;

		dTgtWidth = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_CM, 0, ResData.DataUnit.nBase_Length, 
																												D_UNITSYS_BASE_LENGTH, dSrcWidth);
		
		dTgtHeight = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_CM, 0, ResData.DataUnit.nBase_Length, 
																												D_UNITSYS_BASE_LENGTH, dSrcHeight);

		pWord->MoveRight();
		strTmp.Format(_T("%g"), dTgtWidth);
		pWord->WriteCell(2+i, 5, strTmp);

		pWord->MoveRight();
		strTmp.Format(_T("%g"), dTgtHeight);
		pWord->WriteCell(2+i, 6, strTmp);
	}
	
	pWord->MoveDown();
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteText(_T("\n"));
	pWord->SetSize(FONT_SIZE_BASE);
}

// nTransverse==0 : 교축방향, nTransverse==1 : 교축직각방향
void CDgnToolMPhi_SeismicReport::WriteExamResultTable1(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx)
{
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}

	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES ResType1;
	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES ResType2;
	// 교축방향
	if(nTransverse==0)
	{
		ResType1 = ResData.List[nPierIdx].ResType1Trav;
		ResType2 = ResData.List[nPierIdx].ResType2Trav;
	}
	// 교축직각방향
	else if(nTransverse==1)
	{
		ResType1 = ResData.List[nPierIdx].ResType1Long;
		ResType2 = ResData.List[nPierIdx].ResType2Long;
	}
	else
	{
		ASSERT(0);
		return;
	}

	pWord->SetSize(FONT_SIZE_TABLE_TEXT);
	pWord->SetAlignment(wdAlignTabCenter);

	const int iCol = 6;
	float pColWidth[iCol] = { (float)20, (float)120, (float)80, (float)80, (float)80, (float)80 };
	int nRow=0;
	if(ResData.nDgnLevel==1)  nRow = 15;
	else                      nRow = 11;  // 잔류변위 조사항목은 nDgnLevel==1일 경우에만 표시함.
	pWord->MakeTableExKR(nRow, iCol, pColWidth, 1);
	
	int iColorIndexGray = 16;  /*wdGray25*/
	
	pWord->MergeCells(2, 2);
	pWord->SetCellBorder(1, 1, wdBorderDiagonalDown);
	pWord->SetAlignment(wdAlignTabRight);
	pWord->SetBold(1);
	pWord->WriteText(_LS(IDS_DGN_RLC_DESIGN_CONDITION));
	pWord->WriteText(_T("\n"));
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteText(_LS(IDS_DGN_RLC_INVESTIGATION));

	pWord->MoveRight();
	pWord->MergeCells(1, 4);
	CString strTrans=_T("");
	if      (nTransverse==0)  strTrans=_LS(IDS_DGN_RLC_TRANSVERSE_DIR);
	else if (nTransverse==1)  strTrans=_LS(IDS_DGN_RLC_LONGITUDE_DIR);
	else return;

	CString strTemp=_T("");
	strTemp.Format(_T("%s : %s, %s : %s, %s : %s")
								, _LS(IDS_DGN_RLC_ANALYSIS_PROCESS), strTrans
								, _LS(IDS_DGN_RLC_GROUND_CLASS), ResData.strGroundKind
								, _LS(IDS_DGN_RLC_REGION_DIVISION), ResData.strRegionalKind);
	pWord->SetAlignment(wdAlignTabCenter);
	pWord->SetBold(1);
	pWord->WriteCell(1, 2, strTemp);
	
	pWord->MoveDown();
	pWord->MoveLeft(3);
	pWord->SetBold(1);
	pWord->WriteCell(2, 2, _LS(IDS_DGN_RLC_TYPE1_EQ));
	pWord->MergeCells(1, 2);

	pWord->MoveRight();
	//pWord->SetColorIndex(6);
	pWord->SetBold(1);
	pWord->WriteCell(2, 3, _LS(IDS_DGN_RLC_TYPE2_EQ));
	pWord->MergeCells(1, 2);

	pWord->MoveRight();
	pWord->SetBold(0);
	pWord->SetCellVertAlignment(3, 1, 0);
	pWord->WriteCell(3, 1, _LS(IDS_DGN_RLC_HORI_INTER__n__n_EXAM));
	pWord->MergeCells(9, 1);
	
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s (Sub(P, a))"), _LS(IDS_DGN_RLC_DESTRUCTION_FORM));
	pWord->WriteCell(3, 2, strTemp);

	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->WriteCell(3, 3, ResType1.strFailureType);

	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	//pWord->SetColorIndex(6);
	pWord->WriteCell(3, 4, ResType2.strFailureType);

	pWord->MoveRight();
	pWord->MoveRight();
	strTemp.Format(_T("%s (Sub(k, hc))"), _LS(IDS_DGN_RLC_DESIGN_HORI_INTENSITY));
	pWord->WriteCell(4, 2, strTemp);
	pWord->SetAlignment(wdAlignTabLeft);

	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	strTemp.Format(_T("%g"), ResType1.dKhc);
	pWord->WriteCell(4, 3, strTemp);
	
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	//pWord->SetColorIndex(6);
	strTemp.Format(_T("%g"), ResType2.dKhc);
	pWord->WriteCell(4, 4, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	strTemp.Format(_T("%s (Sub(W, u))"), _LS(IDS_DGN_RLC_UPPER_WEIGHT));
	pWord->WriteCell(5, 2, strTemp);
	pWord->SetAlignment(wdAlignTabLeft);

	double  dSrcForce=0.0, dSrcLength=0.0;
	double  dTgtForce=0.0, dTgtLength=0.0;

	dSrcForce = ResData.List[nPierIdx].dWu;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(5, 3, strTemp);
	
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	//pWord->SetColorIndex(6);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(5, 4, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	strTemp.Format(_T("%s (Sub(W, p))"), _LS(IDS_DGN_RLC_COLUMN_BODY_WEIGHT));
	pWord->WriteCell(6, 2, strTemp);
	pWord->SetAlignment(wdAlignTabLeft);
	
	dSrcForce = ResData.List[nPierIdx].dWp;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(6, 3, strTemp);
	
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	//pWord->SetColorIndex(6);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(6, 4, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	strTemp.Format(_T("%s (Sub(W))"), _LS(IDS_DGN_RLC_EQUV_WEIGHT));
	pWord->WriteCell(7, 2, strTemp);
	pWord->SetAlignment(wdAlignTabLeft);
	
	dSrcForce = ResType1.dW;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(7, 3, strTemp);
	
	dSrcForce = ResType2.dW;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	//pWord->SetColorIndex(6);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(7, 4, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	strTemp.Format(_T("%s (Sub(k, HC)Sub(W))"), _LS(IDS_DGN_RLC_INERTIAL_FORCE));
	pWord->WriteCell(8, 2, strTemp);
	pWord->SetAlignment(wdAlignTabLeft);
	
	dSrcForce = ResType1.dKhc * ResType1.dW;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(8, 3, strTemp);
	
	dSrcForce = ResType2.dKhc * ResType2.dW;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	//pWord->SetColorIndex(6);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(8, 4, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	strTemp.Format(_T("%s (Sub(-3987, a))"), _LS(IDS_DGN_RLC_ALLOWANCE_RATIO));
	pWord->WriteCell(9, 2, strTemp);
	pWord->SetAlignment(wdAlignTabLeft);
	
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	strTemp.Format(_T("%g"), ResType1.dmua);
	pWord->WriteCell(9, 3, strTemp);
	
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	//pWord->SetColorIndex(6);
	strTemp.Format(_T("%g"), ResType2.dmua);
	pWord->WriteCell(9, 4, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	strTemp.Format(_T("%s (Sub(P, a))"), _LS(IDS_DGN_RLC_EQ_HORI_INTER_FORCE));
	pWord->WriteCell(10, 2, strTemp);
	pWord->SetAlignment(wdAlignTabLeft);
	
	dSrcForce = ResType1.dPa;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(10, 3, strTemp);
	
	dSrcForce = ResType2.dPa;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	//pWord->SetColorIndex(6);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(10, 4, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	strTemp.Format(_T("%s"), _LS(IDS_DGN_RLC_DECISION));
	pWord->WriteCell(11, 2, strTemp);
	pWord->SetAlignment(wdAlignTabLeft);
	
	pWord->MoveRight();
	if(ResType1.bCapaChk)
		strTemp = _T("Sub(P, a)Sub(-3917)Sub(k, HC)W");
	else
		strTemp = _T("Sub(P, a)<Sub(k, HC)W");
	pWord->WriteCell(11, 3, strTemp);

	pWord->MoveRight();
	if(ResType1.bCapaChk)
		strTemp = _T("OK");
	else
		strTemp = _T("NG");
	pWord->WriteCell(11, 4, strTemp);
	
	pWord->MoveRight();
	if(ResType2.bCapaChk)
		strTemp = _T("Sub(P, a)Sub(-3917)Sub(k, HC)W");
	else
		strTemp = _T("Sub(P, a)<Sub(k, HC)W");
	//pWord->SetColorIndex(6);
	pWord->WriteCell(11, 5, strTemp);
	
	pWord->MoveRight();
	if(ResType2.bCapaChk)
		strTemp = _T("OK");
	else
		strTemp = _T("NG");
	//pWord->SetColorIndex(6);
	pWord->WriteCell(11, 6, strTemp);

	if(ResData.nDgnLevel!=1)
	{
		pWord->MoveDown();
		pWord->WriteText(_T("\n"));
		pWord->SetAlignment(wdAlignTabLeft);
		pWord->SetSize(FONT_SIZE_BASE);

		return;
	}

	pWord->MoveRight();
	pWord->WriteCell(12, 1, _LS(IDS_DGN_RLC_REMAINING_DISP__n__n_EXAM));
	pWord->MergeCells(4, 1);
	pWord->SetCellVertAlignment(3, 1, 0);

	pWord->MoveRight();
	strTemp.Format(_T("%s (Sub(-3996, Ra))"), _LS(IDS_DGN_RLC_ALLOWANCE_REMAINING_DISP));
	pWord->WriteCell(12, 2, strTemp);
	pWord->SetAlignment(wdAlignTabLeft);
	
	dSrcLength = ResData.List[nPierIdx].ddeltaRa;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	strTemp.Format(_T("%g"), dTgtLength);
	pWord->WriteCell(12, 3, strTemp);
	
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	//pWord->SetColorIndex(6);
	strTemp.Format(_T("%g"), dTgtLength);
	pWord->WriteCell(12, 4, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	strTemp.Format(_T("%s (Sub(-3996, R))"), _LS(IDS_DGN_RLC_REMAINING_DISP));
	pWord->WriteCell(13, 2, strTemp);
	pWord->SetAlignment(wdAlignTabLeft);
	
	dSrcLength = ResType1.ddeltaR;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	strTemp.Format(_T("%g"), dTgtLength);
	pWord->WriteCell(13, 3, strTemp);
	
	dSrcLength = ResType2.ddeltaR;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	//pWord->SetColorIndex(6);
	strTemp.Format(_T("%g"), dTgtLength);
	pWord->WriteCell(13, 4, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	strTemp.Format(_T("%s (Sub(-3987, r))"), _LS(IDS_DGN_RLC_RESPONSE_RATIO));
	pWord->WriteCell(14, 2, strTemp);
	pWord->SetAlignment(wdAlignTabLeft);
	
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	strTemp.Format(_T("%g"), ResType1.dmur);
	pWord->WriteCell(14, 3, strTemp);
	
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	//pWord->SetColorIndex(6);
	strTemp.Format(_T("%g"), ResType2.dmur);
	pWord->WriteCell(14, 4, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	strTemp.Format(_T("%s"), _LS(IDS_DGN_RLC_DECISION));
	pWord->WriteCell(15, 2, strTemp);
	pWord->SetAlignment(wdAlignTabLeft);
	
	pWord->MoveRight();
	if(ResType1.bDisChk)
		strTemp = _T("Sub(-3996, Ra) Sub(-3917) Sub(-3996, R)");
	else
		strTemp = _T("Sub(-3996, Ra) < Sub(-3996, R)");
	pWord->WriteCell(15, 3, strTemp);
	
	pWord->MoveRight();
	if(ResType1.bDisChk)
		strTemp = _T("OK");
	else
		strTemp = _T("NG");
	pWord->WriteCell(15, 4, strTemp);
	
	pWord->MoveRight();
	if(ResType2.bCapaChk)
		strTemp = _T("Sub(-3996, Ra) Sub(-3917) Sub(-3996, R)");
	else
		strTemp = _T("Sub(-3996, Ra) < Sub(-3996, R)");
	//pWord->SetColorIndex(6);
	pWord->WriteCell(15, 5, strTemp);
	
	pWord->MoveRight();
	if(ResType2.bCapaChk)
		strTemp = _T("OK");
	else
		strTemp = _T("NG");
	//pWord->SetColorIndex(6);
	pWord->WriteCell(15, 6, strTemp);

	pWord->MoveDown();
	pWord->WriteText(_T("\n"));
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->SetSize(FONT_SIZE_BASE);
}

// nTransverse==0 : 교축방향, nTransverse==1 : 교축직각방향
// nType==0: 타입 I, nType==1: 타입 II
void CDgnToolMPhi_SeismicReport::WriteExamResultTable2(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType)
{
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}

	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES ResType1;
	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES ResType2;
	// 교축방향
	if(nTransverse==0)
	{
		ResType1 = ResData.List[nPierIdx].ResType1Trav;
		ResType2 = ResData.List[nPierIdx].ResType2Trav;
	}
	// 교축직각방향
	else if(nTransverse==1)
	{
		ResType1 = ResData.List[nPierIdx].ResType1Long;
		ResType2 = ResData.List[nPierIdx].ResType2Long;
	}
	else
	{
		ASSERT(0);
		return;
	}

	if(nType!=0 && nType!=1)
	{
		ASSERT(0);
		return;
	}

	pWord->SetSize(FONT_SIZE_TABLE_TEXT);
	pWord->SetAlignment(wdAlignTabCenter);

	const int iCol = 4;
	float pColWidth[iCol] = { (float)20, (float)120, (float)160, (float)160 };
	int nRow=0;
	if(ResData.nDgnLevel==1)  nRow = 15;
	else                      nRow = 11;  // 잔류변위 조사항목은 nDgnLevel==1일 경우에만 표시함.
	pWord->MakeTableExKR(nRow, iCol, pColWidth, 1);
	
	int iColorIndexGray = 16;  /*wdGray25*/
	
	pWord->MergeCells(2, 2);
	pWord->SetCellBorder(1, 1, wdBorderDiagonalDown);
	pWord->SetAlignment(wdAlignTabRight);
	pWord->SetBold(1);
	pWord->WriteText(_LS(IDS_DGN_RLC_DESIGN_CONDITION));
	pWord->WriteText(_T("\n"));
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteText(_LS(IDS_DGN_RLC_INVESTIGATION));

	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	CString strTrans=_T("");
	if      (nTransverse==0)  strTrans=_LS(IDS_DGN_RLC_TRANSVERSE_DIR);
	else if (nTransverse==1)  strTrans=_LS(IDS_DGN_RLC_LONGITUDE_DIR);
	else return;

	CString strTemp=_T("");
	strTemp.Format(_T("%s : %s, %s : %s, %s : %s")
								, _LS(IDS_DGN_RLC_ANALYSIS_PROCESS), strTrans
								, _LS(IDS_DGN_RLC_GROUND_CLASS), ResData.strGroundKind
								, _LS(IDS_DGN_RLC_REGION_DIVISION), ResData.strRegionalKind);
	pWord->SetAlignment(wdAlignTabCenter);
	pWord->SetBold(1);
	pWord->WriteCell(1, 2, strTemp);
	
	if      (nType==0)  strTemp.Format(_T("%s"), _LS(IDS_DGN_RLC_TYPE1_EQ));
	else if (nType==1)  strTemp.Format(_T("%s"), _LS(IDS_DGN_RLC_TYPE2_EQ));
	else  return;
	pWord->MoveDown();
	pWord->MoveLeft(1);
	pWord->SetBold(1);
	pWord->WriteCell(2, 2, strTemp);
	pWord->MergeCells(1, 2);

	pWord->MoveRight();
	pWord->SetBold(0);
	pWord->SetCellVertAlignment(3, 1, 0);
	pWord->WriteCell(3, 1, _LS(IDS_DGN_RLC_HORI_INTER__n__n_EXAM));
	pWord->MergeCells(9, 1);
	
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s (Sub(P, a))"), _LS(IDS_DGN_RLC_DESTRUCTION_FORM));
	pWord->WriteCell(3, 2, strTemp);

	if      (nType==0)  strTemp.Format(_T("%s"), ResType1.strFailureType);
	else if (nType==1)  strTemp.Format(_T("%s"), ResType2.strFailureType);
	else  return;
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->WriteCell(3, 3, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	strTemp.Format(_T("%s (Sub(k, hc))"), _LS(IDS_DGN_RLC_DESIGN_HORI_INTENSITY));
	pWord->WriteCell(4, 2, strTemp);
	pWord->SetAlignment(wdAlignTabLeft);

	if      (nType==0)  strTemp.Format(_T("%g"), ResType1.dKhc);
	else if (nType==1)  strTemp.Format(_T("%g"), ResType2.dKhc);
	else  return;
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->WriteCell(4, 3, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	strTemp.Format(_T("%s (Sub(W, u))"), _LS(IDS_DGN_RLC_UPPER_WEIGHT));
	pWord->WriteCell(5, 2, strTemp);
	pWord->SetAlignment(wdAlignTabLeft);

	double  dSrcForce=0.0, dSrcLength=0.0;
	double  dTgtForce=0.0, dTgtLength=0.0;

	dSrcForce = ResData.List[nPierIdx].dWu;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(5, 3, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	strTemp.Format(_T("%s (Sub(W, p))"), _LS(IDS_DGN_RLC_COLUMN_BODY_WEIGHT));
	pWord->WriteCell(6, 2, strTemp);
	pWord->SetAlignment(wdAlignTabLeft);
	
	dSrcForce = ResData.List[nPierIdx].dWp;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(6, 3, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	strTemp.Format(_T("%s (Sub(W))"), _LS(IDS_DGN_RLC_EQUV_WEIGHT));
	pWord->WriteCell(7, 2, strTemp);
	pWord->SetAlignment(wdAlignTabLeft);

	if      (nType==0)  dSrcForce = ResType1.dW;
	else if (nType==1)  dSrcForce = ResType2.dW;
	else  return;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(7, 3, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	strTemp.Format(_T("%s (Sub(k, HC)Sub(W))"), _LS(IDS_DGN_RLC_INERTIAL_FORCE));
	pWord->WriteCell(8, 2, strTemp);
	pWord->SetAlignment(wdAlignTabLeft);

	if      (nType==0)  dSrcForce = ResType1.dKhc * ResType1.dW;
	else if (nType==1)  dSrcForce = ResType2.dKhc * ResType2.dW;
	else  return;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(8, 3, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	strTemp.Format(_T("%s (Sub(-3987, a))"), _LS(IDS_DGN_RLC_ALLOWANCE_RATIO));
	pWord->WriteCell(9, 2, strTemp);
	pWord->SetAlignment(wdAlignTabLeft);
	
	if      (nType==0)  strTemp.Format(_T("%g"), ResType1.dmua);
	else if (nType==1)  strTemp.Format(_T("%g"), ResType2.dmua);
	else  return;
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->WriteCell(9, 3, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	strTemp.Format(_T("%s (Sub(P, a))"), _LS(IDS_DGN_RLC_EQ_HORI_INTER_FORCE));
	pWord->WriteCell(10, 2, strTemp);
	pWord->SetAlignment(wdAlignTabLeft);
	
	if      (nType==0)  dSrcForce = ResType1.dPa;
	else if (nType==1)  dSrcForce = ResType2.dPa;
	else  return;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(10, 3, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	strTemp.Format(_T("%s"), _LS(IDS_DGN_RLC_DECISION));
	pWord->WriteCell(11, 2, strTemp);
	pWord->SetAlignment(wdAlignTabLeft);

	BOOL bCapaChk=FALSE;
	if      (nType==0)  bCapaChk = ResType1.bCapaChk;
	else if (nType==1)  bCapaChk = ResType2.bCapaChk;
	else  return;
	pWord->MoveRight();
	if(bCapaChk)
		strTemp = _T("Sub(P, a)Sub(-3917)Sub(k, HC)W");
	else
		strTemp = _T("Sub(P, a)<Sub(k, HC)W");
	pWord->WriteCell(11, 3, strTemp);

	pWord->MoveRight();
	if(bCapaChk)
		strTemp = _T("OK");
	else
		strTemp = _T("NG");
	pWord->WriteCell(11, 4, strTemp);

	if(ResData.nDgnLevel!=1)
	{
		pWord->MoveDown();
		pWord->WriteText(_T("\n"));
		pWord->SetAlignment(wdAlignTabLeft);
		pWord->SetSize(FONT_SIZE_BASE);

		return;
	}

	// 잔류변위의 조사
	pWord->MoveRight();
	pWord->WriteCell(12, 1, _LS(IDS_DGN_RLC_REMAINING_DISP__n__n_EXAM));
	pWord->MergeCells(4, 1);
	pWord->SetCellVertAlignment(3, 1, 0);

	pWord->MoveRight();
	strTemp.Format(_T("%s (Sub(-3996, Ra))"), _LS(IDS_DGN_RLC_ALLOWANCE_REMAINING_DISP));
	pWord->WriteCell(12, 2, strTemp);
	pWord->SetAlignment(wdAlignTabLeft);
	
	dSrcLength = ResData.List[nPierIdx].ddeltaRa;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	strTemp.Format(_T("%g"), dTgtLength);
	pWord->WriteCell(12, 3, strTemp);
	
	pWord->MoveRight();
	pWord->MoveRight();
	strTemp.Format(_T("%s (Sub(-3996, R))"), _LS(IDS_DGN_RLC_REMAINING_DISP));
	pWord->WriteCell(13, 2, strTemp);
	pWord->SetAlignment(wdAlignTabLeft);
	
	if      (nType==0)  dSrcLength = ResType1.ddeltaR;
	else if (nType==1)  dSrcLength = ResType2.ddeltaR;
	else  return;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	strTemp.Format(_T("%g"), dTgtLength);
	pWord->WriteCell(13, 3, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	strTemp.Format(_T("%s (Sub(-3987, r))"), _LS(IDS_DGN_RLC_RESPONSE_RATIO));
	pWord->WriteCell(14, 2, strTemp);
	pWord->SetAlignment(wdAlignTabLeft);
	
	if      (nType==0)  strTemp.Format(_T("%g"), ResType1.dmur);
	else if (nType==1)  strTemp.Format(_T("%g"), ResType2.dmur);
	else  return;
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->WriteCell(14, 3, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	strTemp.Format(_T("%s"), _LS(IDS_DGN_RLC_DECISION));
	pWord->WriteCell(15, 2, strTemp);
	pWord->SetAlignment(wdAlignTabLeft);
	
	BOOL bDisChk=FALSE;
	if      (nType==0)  bDisChk = ResType1.bDisChk;
	else if (nType==1)  bDisChk = ResType2.bDisChk;
	else  return;
	pWord->MoveRight();
	if(bDisChk)
		strTemp = _T("Sub(-3996, Ra) Sub(-3917) Sub(-3996, R)");
	else
		strTemp = _T("Sub(-3996, Ra) < Sub(-3996, R)");
	pWord->WriteCell(15, 3, strTemp);
	
	pWord->MoveRight();
	if(bDisChk)
		strTemp = _T("OK");
	else
		strTemp = _T("NG");
	pWord->WriteCell(15, 4, strTemp);

	pWord->MoveDown();
	pWord->WriteText(_T("\n"));
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->SetSize(FONT_SIZE_BASE);
}

// nTransverse==0 : 교축방향, nTransverse==1 : 교축직각방향
void CDgnToolMPhi_SeismicReport::WriteStructureTable1(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx)
{
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}

	// 교축방향 or 교축직각방향 중 하나를 선택해야 함.
	if(nTransverse!=0 && nTransverse!=1)
	{
		ASSERT(0);
		return;
	}

	pWord->SetSize(FONT_SIZE_TABLE_TEXT);
	pWord->SetAlignment(wdAlignTabCenter);

	const int iCol = 5;
	float pColWidth[iCol] = { (float)20, (float)45, (float)240, (float)110, (float)45 };
	pWord->MakeTableExKR(11, iCol, pColWidth, 1);
	
	int iColorIndexGray = 16;  /*wdGray25*/
	
	pWord->SetCellShading(1, 1, iColorIndexGray);

	pWord->MoveRight();
	pWord->SetCellShading(1, 2, iColorIndexGray);
	pWord->MergeCells(1, 3);
	pWord->SetBold(1);  
	pWord->WriteCell(1, 2, _LS(IDS_DGN_RLC_STRUCTURE_INPUT_CONDITION));

	pWord->MoveRight();
	pWord->SetCellShading(1, 3, iColorIndexGray);
	pWord->SetBold(1);
	pWord->WriteCell(1, 3, _LS(IDS_DGN_RLC_REMARK));

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(2, 1, iColorIndexGray);
	int nRowNum=0;
	CString strTemp=_T("");
	strTemp.Format(_T("%d"), ++nRowNum);
	pWord->SetBold(1);
	pWord->WriteCell(2, 1, strTemp);

	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(2, 2, _LS(IDS_DGN_RLC_HORI_INTER_FORCE_DESIGN_PROCESS));

	pWord->MoveRight();
	pWord->WriteCell(2, 3, ResData.List[nPierIdx].strType);

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(3, 1, iColorIndexGray);
	strTemp.Format(_T("%d"), ++nRowNum);
	pWord->SetBold(1);
	pWord->WriteCell(3, 1, strTemp);
	
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(3, 2, _LS(IDS_DGN_RLC_ANALYSIS_PROCESS));
	
	pWord->MoveRight();
	if      (nTransverse==0)  strTemp = _LS(IDS_DGN_RLC_TRANSVERSE_DIR);
	else if (nTransverse==1)  strTemp = _LS(IDS_DGN_RLC_LONGITUDE_DIR);
	else { ASSERT(0); return; }
	pWord->WriteCell(3, 3, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->MoveRight();
	pWord->MergeCells(3, 1);
	pWord->SetCellShading(4, 1, iColorIndexGray);
	strTemp.Format(_T("%d"), ++nRowNum);
	pWord->SetBold(1);
	pWord->WriteCell(4, 1, strTemp);
	
	pWord->MoveRight();
	pWord->MergeCells(3, 1);
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(4, 2, _LS(IDS_DGN_RLC_EQUV_WEIGHT));
	
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(W, u) (kN)"), _LS(IDS_DGN_RLC_UPPER_STRUCTURE_WEIGHT));
	pWord->WriteCell(4, 3, strTemp);

	double  dSrcForce=0.0, dSrcMoment=0.0, dSrcLength=0.0;
	double  dTgtForce=0.0, dTgtMoment=0.0, dTgtLength=0.0;

	dSrcForce = ResData.List[nPierIdx].dWu;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(4, 4, strTemp);

	pWord->MoveDown();
	pWord->MoveLeft(1);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(W, p)' (kN)"), _LS(IDS_DGN_RLC_PIER_COLUMN_BODY_WEIGHT));
	pWord->WriteCell(5, 3, strTemp);

	dSrcForce = ResData.List[nPierIdx].dWp_Beam;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(5, 4, strTemp);

	pWord->MoveDown();
	pWord->MoveLeft(1);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(W, p) (kN)"), _LS(IDS_DGN_RLC_PIER_BODY_WEIGHT));
	pWord->WriteCell(6, 3, strTemp);
	
	dSrcForce = ResData.List[nPierIdx].dWp;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(6, 4, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(7, 1, iColorIndexGray);
	strTemp.Format(_T("%d"), ++nRowNum);
	pWord->SetBold(1);
	pWord->WriteCell(7, 1, strTemp);

	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(M, o) (kN Sub(-3881) m)"), _LS(IDS_DGN_RLC_UPPER_STRUCTURE_DEADLOAD_MOMENT));
	pWord->WriteCell(7, 2, strTemp);
	
	dSrcMoment = ResData.List[nPierIdx].dM0[nTransverse];
	dTgtMoment = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, ResData.DataUnit.nBase_Force, ResData.DataUnit.nBase_Length, 
																											D_UNITSYS_BASE_MOMENT, dSrcMoment);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtMoment);
	pWord->WriteCell(7, 3, strTemp);
	
	pWord->MoveRight();
	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(8, 1, iColorIndexGray);
	strTemp.Format(_T("%d"), ++nRowNum);
	pWord->SetBold(1);
	pWord->WriteCell(8, 1, strTemp);
	
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(h) (m)"), _LS(IDS_DGN_RLC_PIER_WORK_POSITION_DISTANCE));
	pWord->WriteCell(8, 2, strTemp);
	
	dSrcLength = ResData.List[nPierIdx].dActHeight;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtLength);
	pWord->WriteCell(8, 3, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(9, 1, iColorIndexGray);
	strTemp.Format(_T("%d"), ++nRowNum);
	pWord->SetBold(1);
	pWord->WriteCell(9, 1, strTemp);
	
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(RD) (kN)"), _LS(IDS_DGN_RLC_DEADLOAD_REACTION));
	pWord->WriteCell(9, 2, strTemp);
	
	dSrcForce = ResData.List[nPierIdx].dRD;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(9, 3, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(10, 1, iColorIndexGray);
	strTemp.Format(_T("%d"), ++nRowNum);
	pWord->SetBold(1);
	pWord->WriteCell(10, 1, strTemp);
	
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(-3996)Ra (m)"), _LS(IDS_DGN_RLC_PIER_ALLOW_REMAINING_DISP));
	pWord->WriteCell(10, 2, strTemp);
	
	dSrcLength = ResData.List[nPierIdx].ddeltaRa;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtLength);
	pWord->WriteCell(10, 3, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(11, 1, iColorIndexGray);
	strTemp.Format(_T("%d"), ++nRowNum);
	pWord->SetBold(1);
	pWord->WriteCell(11, 1, strTemp);
	
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(T) (sec)"), _LS(IDS_DGN_RLC_EQ_PIER_NATURE_CYCLE));
	pWord->WriteCell(11, 2, strTemp);
	
	pWord->MoveRight();
	strTemp.Format(_T("%g"), ResData.List[nPierIdx].dT[nTransverse]);
	pWord->WriteCell(11, 3, strTemp);

	pWord->MoveDown();
	pWord->WriteText(_T("\n"));
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->SetSize(FONT_SIZE_BASE);
}

void CDgnToolMPhi_SeismicReport::WriteStructureTable2(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nPierIdx)
{
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}

	pWord->SetSize(FONT_SIZE_TABLE_TEXT);
	pWord->SetAlignment(wdAlignTabCenter);
	
	const int iCol = 6;
	float pColWidth[iCol] = { (float)20, (float)45, (float)240, (float)50, (float)60, (float)45 };
	pWord->MakeTableExKR(11, iCol, pColWidth, 1);
	
	int iColorIndexGray = 16;  /*wdGray25*/
	
	pWord->SetCellShading(1, 1, iColorIndexGray);

	pWord->MoveRight();
	pWord->SetCellShading(1, 2, iColorIndexGray);
	pWord->MergeCells(1, 4);
	pWord->SetBold(1);
	pWord->WriteCell(1, 2, _LS(IDS_DGN_RLC_STRUCTURE_INPUT_CONDITION));

	pWord->MoveRight();
	pWord->SetCellShading(1, 3, iColorIndexGray);
	pWord->SetBold(1);
	pWord->WriteCell(1, 3, _LS(IDS_DGN_RLC_REMARK));

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(2, 1, iColorIndexGray);
	int nRowNum=0;
	CString strTemp=_T("");
	strTemp.Format(_T("%d"), ++nRowNum);
	pWord->SetBold(1);
	pWord->WriteCell(2, 1, strTemp);

	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(2, 2, _LS(IDS_DGN_RLC_HORI_INTER_FORCE_DESIGN_PROCESS));

	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->WriteCell(2, 3, ResData.List[nPierIdx].strType);

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(3, 1, iColorIndexGray);
	strTemp.Format(_T("%d"), ++nRowNum);
	pWord->SetBold(1);
	pWord->WriteCell(3, 1, strTemp);
	
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(3, 2, _LS(IDS_DGN_RLC_ANALYSIS_PROCESS));
	
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->WriteCell(3, 3, _LS(IDS_DGN_RLC_TRANS_DIR_LONG_DIR));

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->MoveRight();
	pWord->MergeCells(3, 1);
	pWord->SetCellShading(4, 1, iColorIndexGray);
	strTemp.Format(_T("%d"), ++nRowNum);
	pWord->SetBold(1);
	pWord->WriteCell(4, 1, strTemp);
	
	pWord->MoveRight();
	pWord->MergeCells(3, 1);
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(4, 2, _LS(IDS_DGN_RLC_EQUV_WEIGHT));
	
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(W, u) (kN)"), _LS(IDS_DGN_RLC_UPPER_STRUCTURE_WEIGHT));
	pWord->WriteCell(4, 3, strTemp);

	double  dSrcForce=0.0, dSrcMoment=0.0, dSrcLength=0.0;
	double  dTgtForce=0.0, dTgtMoment=0.0, dTgtLength=0.0;
	
	dSrcForce = ResData.List[nPierIdx].dWu;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(4, 4, strTemp);

	pWord->MoveDown();
	pWord->MoveLeft(2);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(W, p)' (kN)"), _LS(IDS_DGN_RLC_PIER_COLUMN_BODY_WEIGHT));
	pWord->WriteCell(5, 3, strTemp);

	dSrcForce = ResData.List[nPierIdx].dWp_Beam;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(5, 4, strTemp);

	pWord->MoveDown();
	pWord->MoveLeft(2);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(W, p) (kN)"), _LS(IDS_DGN_RLC_PIER_BODY_WEIGHT));
	pWord->WriteCell(6, 3, strTemp);
	
	dSrcForce = ResData.List[nPierIdx].dWp;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(6, 4, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(7, 1, iColorIndexGray);
	strTemp.Format(_T("%d"), ++nRowNum);
	pWord->SetBold(1);
	pWord->WriteCell(7, 1, strTemp);

	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(M, o) (kN Sub(-3881) m)"), _LS(IDS_DGN_RLC_UPPER_STRUCTURE_DEADLOAD_MOMENT));
	pWord->WriteCell(7, 2, strTemp);
	
	dSrcMoment = ResData.List[nPierIdx].dM0[0];
	dTgtMoment = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, ResData.DataUnit.nBase_Force, ResData.DataUnit.nBase_Length, 
																											 D_UNITSYS_BASE_MOMENT, dSrcMoment);
	pWord->MoveRight();
	strTemp.Format(_T("%s\n%g"), _LS(IDS_DGN_RLC_TRANSVERSE_DIR), dTgtMoment);
	pWord->WriteCell(7, 3, strTemp);

	dSrcMoment = ResData.List[nPierIdx].dM0[1];
	dTgtMoment = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, ResData.DataUnit.nBase_Force, ResData.DataUnit.nBase_Length, 
																											 D_UNITSYS_BASE_MOMENT, dSrcMoment);
	pWord->MoveRight();
	strTemp.Format(_T("%s\n%g"), _LS(IDS_DGN_RLC_LONGITUDE_DIR), dTgtMoment);
	pWord->WriteCell(7, 4, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(8, 1, iColorIndexGray);
	strTemp.Format(_T("%d"), ++nRowNum);
	pWord->SetBold(1);
	pWord->WriteCell(8, 1, strTemp);
	
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(h) (m)"), _LS(IDS_DGN_RLC_PIER_WORK_POSITION_DISTANCE));
	pWord->WriteCell(8, 2, strTemp);
	
	dSrcLength = ResData.List[nPierIdx].dActHeight;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																											D_UNITSYS_BASE_LENGTH, dSrcLength);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	strTemp.Format(_T("%g"), dTgtLength);
	pWord->WriteCell(8, 3, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(9, 1, iColorIndexGray);
	strTemp.Format(_T("%d"), ++nRowNum);
	pWord->SetBold(1);
	pWord->WriteCell(9, 1, strTemp);
	
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(RD) (kN)"), _LS(IDS_DGN_RLC_DEADLOAD_REACTION));
	pWord->WriteCell(9, 2, strTemp);
	
	dSrcForce = ResData.List[nPierIdx].dRD;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(9, 3, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(10, 1, iColorIndexGray);
	strTemp.Format(_T("%d"), ++nRowNum);
	pWord->SetBold(1);
	pWord->WriteCell(10, 1, strTemp);
	
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(-3996)Ra (m)"), _LS(IDS_DGN_RLC_PIER_ALLOW_REMAINING_DISP));
	pWord->WriteCell(10, 2, strTemp);
	
	dSrcLength = ResData.List[nPierIdx].ddeltaRa;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	strTemp.Format(_T("%g"), dTgtLength);
	pWord->WriteCell(10, 3, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(11, 1, iColorIndexGray);
	strTemp.Format(_T("%d"), ++nRowNum);
	pWord->SetBold(1);
	pWord->WriteCell(11, 1, strTemp);
	
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(T) (sec)"), _LS(IDS_DGN_RLC_EQ_PIER_NATURE_CYCLE));
	pWord->WriteCell(11, 2, strTemp);
	
	pWord->MoveRight();
	strTemp.Format(_T("%s\n%g"), _LS(IDS_DGN_RLC_TRANSVERSE_DIR), ResData.List[nPierIdx].dT[0]);
	pWord->WriteCell(11, 3, strTemp);
	
	pWord->MoveRight();
	strTemp.Format(_T("%s\n%g"), _LS(IDS_DGN_RLC_LONGITUDE_DIR), ResData.List[nPierIdx].dT[1]);
	pWord->WriteCell(11, 4, strTemp);

	pWord->MoveDown();
	pWord->WriteText(_T("\n"));
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->SetSize(FONT_SIZE_BASE);
}

// nTransverse==0 : 교축방향, nTransverse==1 : 교축직각방향
void CDgnToolMPhi_SeismicReport::WriteMaterialConditionTable1(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx)
{
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}

	// 교축방향 or 교축직각방향 중 하나를 선택해야 함.
	if(nTransverse!=0 && nTransverse!=1)
	{
		ASSERT(0);
		return;
	}

	pWord->SetSize(FONT_SIZE_TABLE_TEXT);
	pWord->SetAlignment(wdAlignTabCenter);

	const int iCol = 5;
	float pColWidth[iCol] = { (float)20, (float)55, (float)230, (float)110, (float)45 };
	pWord->MakeTableExKR(10, iCol, pColWidth, 1);
	
	int iColorIndexGray = 16;  /*wdGray25*/
	
	pWord->SetCellShading(1, 1, iColorIndexGray);

	pWord->MoveRight();
	pWord->SetCellShading(1, 2, iColorIndexGray);
	pWord->MergeCells(1, 3);
	pWord->SetBold(1);
	pWord->WriteCell(1, 2, _LS(IDS_DGN_RLC_STRUCTURE_INPUT_CONDITION));

	pWord->MoveRight();
	pWord->SetCellShading(1, 3, iColorIndexGray);
	pWord->SetBold(1);
	pWord->WriteCell(1, 3, _LS(IDS_DGN_RLC_REMARK));

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(2, 1, iColorIndexGray);
	pWord->MergeCells(3, 1);
	int nRowNum=0;
	CString strTemp=_T("");
	strTemp.Format(_T("%d"), ++nRowNum);
	pWord->SetBold(1);
	pWord->WriteCell(2, 1, strTemp);

	pWord->MoveRight();
	pWord->MergeCells(3, 1);
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(2, 2, _LS(IDS_DGN_RLC_CONCRETE));

	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(-3993) (N/Sup(mm, 3))"), _LS(IDS_DGN_RLC_IRON_CONC_UNIT_VOLUMN_WEIGHT));
	pWord->WriteCell(2, 3, strTemp);

	double  dSrcForce=0.0, dSrcLength=0.0;
	double  dTgtForce=0.0, dTgtLength=0.0;

	dSrcForce = ResData.List[nPierIdx].dWeightDensity;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, ResData.DataUnit.nBase_Force, ResData.DataUnit.nBase_Length,
																											D_UNITSYS_BASE_DENSITY, dSrcForce);
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(2, 4, strTemp);

	pWord->MoveDown();
	pWord->MoveLeft(1);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(E, c) (N/Sup(mm, 2))"), _LS(IDS_DGN_RLC_CONCRETE_ELAST_COEFFICIENT));
	pWord->WriteCell(3, 3, strTemp);

	dSrcForce = ResData.List[nPierIdx].dEc_conc;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, ResData.DataUnit.nBase_Force, ResData.DataUnit.nBase_Length, 
																											D_UNITSYS_BASE_ELAST, dSrcForce);
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(3, 4, strTemp);

	pWord->MoveDown();
	pWord->MoveLeft(1);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(-3981,ck) (N/Sup(mm, 2))"), _LS(IDS_DGN_RLC_CONCRETE_DESIGN_STANDARD_INTENSITY));
	pWord->WriteCell(4, 3, strTemp);
	
	dSrcForce = ResData.List[nPierIdx].dfck_conc;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, ResData.DataUnit.nBase_Force, ResData.DataUnit.nBase_Length, 
																											D_UNITSYS_BASE_STRESS, dSrcForce);
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(4, 4, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(5, 1, iColorIndexGray);
	pWord->MergeCells(2, 1);
	strTemp.Format(_T("%d"), ++nRowNum);
	pWord->SetBold(1);
	pWord->WriteCell(5, 1, strTemp);
	
	pWord->MoveRight();
	pWord->MergeCells(2, 1);
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(5, 2, _LS(IDS_DGN_RLC_AXIS_DIR_IRON));
	
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(E,s) (N/Sup(mm, 2))"), _LS(IDS_DGN_RLC_ELASITY_COEFFICIENT));
	pWord->WriteCell(5, 3, strTemp);

	dSrcForce = ResData.List[nPierIdx].dEs_rebar;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, ResData.DataUnit.nBase_Force, ResData.DataUnit.nBase_Length, 
																											D_UNITSYS_BASE_ELAST, dSrcForce);
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(5, 4, strTemp);

	pWord->MoveDown();
	pWord->MoveLeft(1);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(-3981,sy) (N/Sup(mm, 2))"), _LS(IDS_DGN_RLC_SOME_POINT));
	pWord->WriteCell(6, 3, strTemp);
	
	dSrcForce = ResData.List[nPierIdx].dfsy_rebar;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, ResData.DataUnit.nBase_Force, ResData.DataUnit.nBase_Length, 
																											D_UNITSYS_BASE_STRESS, dSrcForce);
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(6, 4, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(7, 1, iColorIndexGray);
	pWord->MergeCells(4, 1);
	strTemp.Format(_T("%d"), ++nRowNum);
	pWord->SetBold(1);
	pWord->WriteCell(7, 1, strTemp);
	
	pWord->MoveRight();
	pWord->MergeCells(4, 1);
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(7, 2, _LS(IDS_DGN_RLC_BAND_IRON));
	
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(7, 3, _LS(IDS_DGN_RLC_INTERVAL_a_cm));
	
	dSrcLength = ResData.List[nPierIdx].da_tie;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_CM, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%g"), dTgtLength);
	pWord->WriteCell(7, 4, strTemp);
	
	pWord->MoveDown();
	pWord->MoveLeft(1);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s (degree)"), _LS(IDS_DGN_RLC_BAND_IRON_VERTICAL_ANGLE));
	pWord->WriteCell(8, 3, strTemp);
	
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%g"), ResData.List[nPierIdx].dAngle_tie);
	pWord->WriteCell(8, 4, strTemp);

	pWord->MoveDown();
	pWord->MoveLeft(1);
	pWord->SetAlignment(wdAlignTabLeft);
	CString strTemp2=_T("");
	strTemp2.Format(_LS(IDS_DGN_RLC_BAND_IRON_AREA), _T("Sub(-3994)"));
	strTemp.Format(_T("%s Sub(A, w) (Sup(cm, 2))"), strTemp2);
	pWord->WriteCell(9, 3, strTemp);
	
	dSrcLength = ResData.List[nPierIdx].dAw_tie[nTransverse];
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_CM, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_AREA, dSrcLength);
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%g"), dTgtLength);
	pWord->WriteCell(9, 4, strTemp);

	pWord->MoveDown();
	pWord->MoveLeft(1);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(-3981,sy) (N/Sup(mm, 2))"), _LS(IDS_DGN_RLC_SOME_POINT));
	pWord->WriteCell(10, 3, strTemp);
	
	dSrcForce = ResData.List[nPierIdx].dfsy_tie;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, ResData.DataUnit.nBase_Force, ResData.DataUnit.nBase_Length, 
																											D_UNITSYS_BASE_STRESS, dSrcForce);
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(10, 4, strTemp); 

	pWord->MoveDown();
	pWord->WriteText(_T("\n"));
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->SetSize(FONT_SIZE_BASE);
}

// nTransverse==0 : 교축방향, nTransverse==1 : 교축직각방향
void CDgnToolMPhi_SeismicReport::WriteMaterialConditionTable2(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nPierIdx)
{
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}

	pWord->SetSize(FONT_SIZE_TABLE_TEXT);
	pWord->SetAlignment(wdAlignTabCenter);

	const int iCol = 6;
	float pColWidth[iCol] = { (float)20, (float)55, (float)230, (float)50, (float)60, (float)45 };
	pWord->MakeTableExKR(10, iCol, pColWidth, 1);

	int iColorIndexGray = 16;  /*wdGray25*/
	
	pWord->SetCellShading(1, 1, iColorIndexGray);

	pWord->MoveRight();
	pWord->SetCellShading(1, 2, iColorIndexGray);
	pWord->MergeCells(1, 4);
	pWord->SetBold(1);
	pWord->WriteCell(1, 2, _LS(IDS_DGN_RLC_STRUCTURE_INPUT_CONDITION));

	pWord->MoveRight();
	pWord->SetCellShading(1, 3, iColorIndexGray);
	pWord->SetBold(1);
	pWord->WriteCell(1, 3, _LS(IDS_DGN_RLC_REMARK));

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(2, 1, iColorIndexGray);
	pWord->MergeCells(3, 1);
	int nRowNum=0;
	CString strTemp=_T("");
	strTemp.Format(_T("%d"), ++nRowNum);
	pWord->SetBold(1);
	pWord->WriteCell(2, 1, strTemp);

	pWord->MoveRight();
	pWord->MergeCells(3, 1);
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(2, 2, _LS(IDS_DGN_RLC_CONCRETE));

	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(-3993) (N/Sup(mm, 3))"), _LS(IDS_DGN_RLC_IRON_CONC_UNIT_VOLUMN_WEIGHT));
	pWord->WriteCell(2, 3, strTemp);

	double  dSrcForce=0.0, dSrcLength=0.0;
	double  dTgtForce=0.0, dTgtLength=0.0;

	dSrcForce = ResData.List[nPierIdx].dWeightDensity;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, ResData.DataUnit.nBase_Force, ResData.DataUnit.nBase_Length,
																											D_UNITSYS_BASE_DENSITY, dSrcForce);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(2, 4, strTemp);

	pWord->MoveDown();
	pWord->MoveLeft(2);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(E, c) (N/Sup(mm, 2))"), _LS(IDS_DGN_RLC_CONCRETE_ELAST_COEFFICIENT));
	pWord->WriteCell(3, 3, strTemp);

	dSrcForce = ResData.List[nPierIdx].dEc_conc;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, ResData.DataUnit.nBase_Force, ResData.DataUnit.nBase_Length, 
																											D_UNITSYS_BASE_ELAST, dSrcForce);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(3, 4, strTemp);

	pWord->MoveDown();
	pWord->MoveLeft(1);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(-3981,ck) (N/Sup(mm, 2))"), _LS(IDS_DGN_RLC_CONCRETE_DESIGN_STANDARD_INTENSITY));
	pWord->WriteCell(4, 3, strTemp);
	
	dSrcForce = ResData.List[nPierIdx].dfck_conc;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, ResData.DataUnit.nBase_Force, ResData.DataUnit.nBase_Length, 
																											D_UNITSYS_BASE_STRESS, dSrcForce);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(4, 4, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(5, 1, iColorIndexGray);
	pWord->MergeCells(2, 1);
	strTemp.Format(_T("%d"), ++nRowNum);
	pWord->SetBold(1);
	pWord->WriteCell(5, 1, strTemp);
	
	pWord->MoveRight();
	pWord->MergeCells(2, 1);
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(5, 2, _LS(IDS_DGN_RLC_AXIS_DIR_IRON));
	
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(E,s) (N/Sup(mm, 2))"), _LS(IDS_DGN_RLC_ELASITY_COEFFICIENT));
	pWord->WriteCell(5, 3, strTemp);

	dSrcForce = ResData.List[nPierIdx].dEs_rebar;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, ResData.DataUnit.nBase_Force, ResData.DataUnit.nBase_Length, 
																											D_UNITSYS_BASE_ELAST, dSrcForce);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(5, 4, strTemp);

	pWord->MoveDown();
	pWord->MoveLeft(1);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(-3981,sy) (N/Sup(mm, 2))"), _LS(IDS_DGN_RLC_SOME_POINT));
	pWord->WriteCell(6, 3, strTemp);
	
	dSrcForce = ResData.List[nPierIdx].dfsy_rebar;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, ResData.DataUnit.nBase_Force, ResData.DataUnit.nBase_Length, 
																											D_UNITSYS_BASE_STRESS, dSrcForce);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(6, 4, strTemp);

	pWord->MoveRight();
	pWord->MoveRight();
	pWord->MoveRight();
	pWord->SetCellShading(7, 1, iColorIndexGray);
	pWord->MergeCells(4, 1);
	strTemp.Format(_T("%d"), ++nRowNum);
	pWord->SetBold(1);
	pWord->WriteCell(7, 1, strTemp);
	
	pWord->MoveRight();
	pWord->MergeCells(4, 1);
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(7, 2, _LS(IDS_DGN_RLC_BAND_IRON));
	
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->WriteCell(7, 3, _LS(IDS_DGN_RLC_INTERVAL_a_cm));
	
	dSrcLength = ResData.List[nPierIdx].da_tie;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_CM, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%g"), dTgtLength);
	pWord->WriteCell(7, 4, strTemp);
	
	pWord->MoveDown();
	pWord->MoveLeft(1);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s (degree)"), _LS(IDS_DGN_RLC_BAND_IRON_VERTICAL_ANGLE));
	pWord->WriteCell(8, 3, strTemp);
	
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%g"), ResData.List[nPierIdx].dAngle_tie);
	pWord->WriteCell(8, 4, strTemp);

	pWord->MoveDown();
	pWord->MoveLeft(1);
	pWord->SetAlignment(wdAlignTabLeft);
	CString strTemp2=_T("");
	strTemp2.Format(_LS(IDS_DGN_RLC_BAND_IRON_AREA), _T("Sub(-3994)"));
	strTemp.Format(_T("%s Sub(A, w) (Sup(cm, 2))"), strTemp2);
	pWord->WriteCell(9, 3, strTemp);
	
	dSrcLength = ResData.List[nPierIdx].dAw_tie[0];
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_CM, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_AREA, dSrcLength);
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s\n%g"), _LS(IDS_DGN_RLC_TRANSVERSE_DIR), dTgtLength);
	pWord->WriteCell(9, 4, strTemp);

	dSrcLength = ResData.List[nPierIdx].dAw_tie[1];
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_CM, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_AREA, dSrcLength);
	pWord->MoveRight();
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s\n%g"), _LS(IDS_DGN_RLC_LONGITUDE_DIR), dTgtLength);
	pWord->WriteCell(9, 5, strTemp);

	pWord->MoveDown();
	pWord->MoveLeft(2);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%s Sub(-3981,sy) (N/Sup(mm, 2))"), _LS(IDS_DGN_RLC_SOME_POINT));
	pWord->WriteCell(10, 3, strTemp);
	
	dSrcForce = ResData.List[nPierIdx].dfsy_tie;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, ResData.DataUnit.nBase_Force, ResData.DataUnit.nBase_Length, 
																											D_UNITSYS_BASE_STRESS, dSrcForce);
	pWord->MoveRight();
	pWord->MergeCells(1, 2);
	pWord->SetAlignment(wdAlignTabLeft);
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(10, 4, strTemp); 

	pWord->MoveDown();
	pWord->WriteText(_T("\n"));
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->SetSize(FONT_SIZE_BASE);
}

// 타입 I : nType==0, 타입 II : nType==1
// 교축방향 : nTransverse==0 , 교축직각방향 : nTransverse==1
void CDgnToolMPhi_SeismicReport::WriteHorizontalForce(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType)
{
	if(nTransverse != 0 && nTransverse != 1)
		return;
	if(nType != 0 && nType != 1)
		return;
	
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}

	m_nSub4Num = 0;

	int iColorIndexGray = 16;  /*wdGray25*/

	WriteHorizontalForce1(pWord, ResData, nTransverse, nPierIdx, nType);
	WriteHorizontalForce2(pWord, ResData, nTransverse, nPierIdx, nType);
	WriteHorizontalForce3(pWord, ResData, nTransverse, nPierIdx, nType);
	WriteHorizontalForce4(pWord, ResData, nTransverse, nPierIdx, nType);
	WriteHorizontalForce5(pWord, ResData, nTransverse, nPierIdx, nType);
	pWord->PageBreak();
	WriteHorizontalForce6(pWord, ResData, nTransverse, nPierIdx, nType);
}

// 타입 I : nType==0, 타입 II : nType==1
// 교축방향 : nTransverse==0 , 교축직각방향 : nTransverse==1
void CDgnToolMPhi_SeismicReport::WriteHorizontalForce1(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType)
{
	if(nTransverse != 0 && nTransverse != 1)
		return;
	if(nType != 0 && nType != 1)
		return;
	
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}

	int iColorIndexGray = 16;  /*wdGray25*/
	
	WriteSub4Title(_LS(IDS_DGN_RLC_DL_HORI_INTERNAL_FORCE_DIST_RELATION), m_pWord);

	pWord->SetSize(FONT_SIZE_TABLE_TEXT);
	pWord->SetAlignment(wdAlignTabCenter);

	const int iCol = 4;
	float pColWidth[iCol] = { (float)115, (float)115, (float)115, (float)115 };
	pWord->MakeTableExKR(2, iCol, pColWidth, 1);
	
	pWord->SetCellShading(1, 1, iColorIndexGray);
	pWord->SetBold(1);
	CString strTemp=_T("");
	strTemp.Format(_T("%s Sub(M, O)\n(kN Sub(-3881) m)"), _LS(IDS_DGN_RLC_BEND_MOMENT));
	pWord->WriteCell(1, 1, strTemp); 
	
	pWord->MoveRight();
	pWord->SetCellShading(1, 2, iColorIndexGray);
	pWord->SetBold(1);
	strTemp.Format(_T("%s Sub(-3994, 0)\n(1/m)"), _LS(IDS_DGN_RLC_CURVATURE));
	pWord->WriteCell(1, 2, strTemp); 
	
	pWord->MoveRight();
	pWord->SetCellShading(1, 3, iColorIndexGray);
	pWord->SetBold(1);
	strTemp.Format(_T("%s Sub(P, 0)\n(kN)"), _LS(IDS_DGN_RLC_HORI_INTER_FORCE));
	pWord->WriteCell(1, 3, strTemp); 
	
	pWord->MoveRight();
	pWord->SetCellShading(1, 4, iColorIndexGray);
	pWord->SetBold(1);
	strTemp.Format(_T("%s Sub(-3996, 0)\n(m)"), _LS(IDS_DGN_RLC_HORI_DIST));
	pWord->WriteCell(1, 4, strTemp); 
	
	double  dSrcForce=0.0, dSrcMoment=0.0, dSrcLength=0.0;
	double  dTgtForce=0.0, dTgtMoment=0.0, dTgtLength=0.0;

	dSrcMoment = ResData.List[nPierIdx].dM0[nTransverse];
	dTgtMoment = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, ResData.DataUnit.nBase_Force, ResData.DataUnit.nBase_Length, 
																											 D_UNITSYS_BASE_MOMENT, dSrcMoment);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtMoment);
	pWord->WriteCell(2, 1, strTemp); 
	
	dSrcLength = ResData.List[nPierIdx].dPhi0[nTransverse];
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_PERLENGTH, dSrcLength);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtLength);
	pWord->WriteCell(2, 2, strTemp); 
	
	dSrcForce = ResData.List[nPierIdx].dP0[nTransverse];
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(2, 3, strTemp); 
	
	dSrcLength = ResData.List[nPierIdx].ddelta0[nTransverse];
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtLength);
	pWord->WriteCell(2, 4, strTemp); 
	
	pWord->MoveDown();
	pWord->WriteText(_T("\n"));
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->SetSize(FONT_SIZE_BASE);
}

// 타입 I : nType==0, 타입 II : nType==1
// 교축방향 : nTransverse==0 , 교축직각방향 : nTransverse==1
void CDgnToolMPhi_SeismicReport::WriteHorizontalForce2(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType)
{
	if(nTransverse != 0 && nTransverse != 1)
		return;
	if(nType != 0 && nType != 1)
		return;
	
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}

	int iColorIndexGray = 16;  /*wdGray25*/
	
	WriteSub4Title(_LS(IDS_DGN_RLC_CRACK_HORI_INTERNAL_FORCE_DIST_RELATION), m_pWord);

	pWord->SetSize(FONT_SIZE_TABLE_TEXT);
	pWord->SetAlignment(wdAlignTabCenter);
	
	const int iCol = 4;
	float pColWidth[iCol] = { (float)115, (float)115, (float)115, (float)115 };
	pWord->MakeTableExKR(2, iCol, pColWidth, 1);
	
	pWord->SetCellShading(1, 1, iColorIndexGray);
	pWord->SetAlignment(wdAlignTabCenter);
	pWord->SetBold(1);
	CString strTemp=_T("");
	strTemp.Format(_T("%s Sub(M, c)\n(kN Sub(-3881) m)"), _LS(IDS_DGN_RLC_BEND_MOMENT));
	pWord->WriteCell(1, 1, strTemp); 

	pWord->MoveRight();
	pWord->SetCellShading(1, 2, iColorIndexGray);
	pWord->SetBold(1);
	strTemp.Format(_T("%s Sub(-3994, c)\n(1/m)"), _LS(IDS_DGN_RLC_CURVATURE));
	pWord->WriteCell(1, 2, strTemp); 
	
	pWord->MoveRight();
	pWord->SetCellShading(1, 3, iColorIndexGray);
	pWord->SetBold(1);
	strTemp.Format(_T("%s Sub(P, c)\n(kN)"), _LS(IDS_DGN_RLC_HORI_INTER_FORCE));
	pWord->WriteCell(1, 3, strTemp); 
	
	pWord->MoveRight();
	pWord->SetCellShading(1, 4, iColorIndexGray);
	pWord->SetBold(1);
	strTemp.Format(_T("%s Sub(-3996, c)\n(m)"), _LS(IDS_DGN_RLC_HORI_DIST));
	pWord->WriteCell(1, 4, strTemp); 
	
	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES ResType;
	if      (nType==0 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType1Trav;  //타입1지진동에 대한 교축방향 안정검토
	else if (nType==0 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType1Long;  //타입1지진동에 대한 교축직각방향 안정검토
	else if (nType==1 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType2Trav;  //타입2지진동에 대한 교축방향 안정검토
	else if (nType==1 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType2Long;  //타입2지진동에 대한 교축직각방향 안정검토

	double  dSrcForce=0.0, dSrcMoment=0.0, dSrcLength=0.0;
	double  dTgtForce=0.0, dTgtMoment=0.0, dTgtLength=0.0;
	
	dSrcMoment = ResType.dMc;
	dTgtMoment = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, ResData.DataUnit.nBase_Force, ResData.DataUnit.nBase_Length, 
																											 D_UNITSYS_BASE_MOMENT, dSrcMoment);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtMoment);
	pWord->WriteCell(2, 1, strTemp); 
	
	dSrcLength = ResType.dPhic;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_PERLENGTH, dSrcLength);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtLength);
	pWord->WriteCell(2, 2, strTemp); 
	
	dSrcForce = ResType.dPc;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(2, 3, strTemp); 
	
	dSrcLength = ResType.ddeltac;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtLength);
	pWord->WriteCell(2, 4, strTemp); 
	
	pWord->MoveDown();
	pWord->WriteText(_T("\n"));
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->SetSize(FONT_SIZE_BASE);
}

// 타입 I : nType==0, 타입 II : nType==1
// 교축방향 : nTransverse==0 , 교축직각방향 : nTransverse==1
void CDgnToolMPhi_SeismicReport::WriteHorizontalForce3(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType)
{
	if(nTransverse != 0 && nTransverse != 1)
		return;
	if(nType != 0 && nType != 1)
		return;
	
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}
	
	int iColorIndexGray = 16;  /*wdGray25*/
	
	WriteSub4Title(_LS(IDS_DGN_RLC_INITIAL_YIELD_HORI_INTERNAL_FORCE_DIST_RELATION), m_pWord);
	
	pWord->SetSize(FONT_SIZE_TABLE_TEXT);
	pWord->SetAlignment(wdAlignTabCenter);
	
	const int iCol = 4;
	float pColWidth[iCol] = { (float)115, (float)115, (float)115, (float)115 };
	pWord->MakeTableExKR(2, iCol, pColWidth, 1);
	
	pWord->SetCellShading(1, 1, iColorIndexGray);
	pWord->SetAlignment(wdAlignTabCenter);
	pWord->SetBold(1);
	CString strTemp=_T("");
	strTemp.Format(_T("%s Sub(M, y0)\n(kN Sub(-3881) m)"), _LS(IDS_DGN_RLC_BEND_MOMENT));
	pWord->WriteCell(1, 1, strTemp); 
	
	pWord->MoveRight();
	pWord->SetCellShading(1, 2, iColorIndexGray);
	pWord->SetBold(1);
	strTemp.Format(_T("%s Sub(-3994, y0)\n(1/m)"), _LS(IDS_DGN_RLC_CURVATURE));
	pWord->WriteCell(1, 2, strTemp); 
	
	pWord->MoveRight();
	pWord->SetCellShading(1, 3, iColorIndexGray);
	pWord->SetBold(1);
	strTemp.Format(_T("%s Sub(P, y0)\n(kN)"), _LS(IDS_DGN_RLC_HORI_INTER_FORCE));
	pWord->WriteCell(1, 3, strTemp); 
	
	pWord->MoveRight();
	pWord->SetCellShading(1, 4, iColorIndexGray);
	pWord->SetBold(1);
	strTemp.Format(_T("%s Sub(-3996, y0)\n(m)"), _LS(IDS_DGN_RLC_HORI_DIST));
	pWord->WriteCell(1, 4, strTemp); 
	
	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES ResType;
	if      (nType==0 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType1Trav;  //타입1지진동에 대한 교축방향 안정검토
	else if (nType==0 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType1Long;  //타입1지진동에 대한 교축직각방향 안정검토
	else if (nType==1 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType2Trav;  //타입2지진동에 대한 교축방향 안정검토
	else if (nType==1 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType2Long;  //타입2지진동에 대한 교축직각방향 안정검토

	double  dSrcForce=0.0, dSrcMoment=0.0, dSrcLength=0.0;
	double  dTgtForce=0.0, dTgtMoment=0.0, dTgtLength=0.0;
	
	dSrcMoment = ResType.dMy0;
	dTgtMoment = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, ResData.DataUnit.nBase_Force, ResData.DataUnit.nBase_Length, 
																											 D_UNITSYS_BASE_MOMENT, dSrcMoment);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtMoment);
	pWord->WriteCell(2, 1, strTemp); 
	
	dSrcLength = ResType.dPhiy0;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_PERLENGTH, dSrcLength);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtLength);
	pWord->WriteCell(2, 2, strTemp); 
	
	dSrcForce = ResType.dPy0;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(2, 3, strTemp); 
	
	dSrcLength = ResType.ddeltay0;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtLength);
	pWord->WriteCell(2, 4, strTemp); 
	
	pWord->MoveDown();
	pWord->WriteText(_T("\n"));
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->SetSize(FONT_SIZE_BASE);
}

// 타입 I : nType==0, 타입 II : nType==1
// 교축방향 : nTransverse==0 , 교축직각방향 : nTransverse==1
void CDgnToolMPhi_SeismicReport::WriteHorizontalForce4(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType)
{
	if(nTransverse != 0 && nTransverse != 1)
		return;
	if(nType != 0 && nType != 1)
		return;
	
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}
	
	int iColorIndexGray = 16;  /*wdGray25*/
	
	WriteSub4Title(_LS(IDS_DGN_RLC_YIELD_HORI_INTERNAL_FORCE_DIST_RELATION), m_pWord);

	pWord->SetSize(FONT_SIZE_TABLE_TEXT);
	pWord->SetAlignment(wdAlignTabCenter);
	
	const int iCol = 4;
	float pColWidth[iCol] = { (float)115, (float)115, (float)115, (float)115 };
	pWord->MakeTableExKR(2, iCol, pColWidth, 1);
	
	pWord->SetCellShading(1, 1, iColorIndexGray);
	pWord->SetAlignment(wdAlignTabCenter);
	pWord->SetBold(1);
	CString strTemp=_T("");
	strTemp.Format(_T("%s Sub(M, y)\n(kN Sub(-3881) m)"), _LS(IDS_DGN_RLC_BEND_MOMENT));
	pWord->WriteCell(1, 1, strTemp); 
	
	pWord->MoveRight();
	pWord->SetCellShading(1, 2, iColorIndexGray);
	pWord->SetBold(1);
	strTemp.Format(_T("%s Sub(-3994, y)\n(1/m)"), _LS(IDS_DGN_RLC_CURVATURE));
	pWord->WriteCell(1, 2, strTemp); 
	
	pWord->MoveRight();
	pWord->SetCellShading(1, 3, iColorIndexGray);
	pWord->SetBold(1);
	strTemp.Format(_T("%s Sub(P, y)\n(kN)"), _LS(IDS_DGN_RLC_HORI_INTER_FORCE));
	pWord->WriteCell(1, 3, strTemp); 
	
	pWord->MoveRight();
	pWord->SetCellShading(1, 4, iColorIndexGray);
	pWord->SetBold(1);
	strTemp.Format(_T("%s Sub(-3996, y)\n(m)"), _LS(IDS_DGN_RLC_HORI_DIST));
	pWord->WriteCell(1, 4, strTemp); 
	
	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES ResType;
	if      (nType==0 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType1Trav;  //타입1지진동에 대한 교축방향 안정검토
	else if (nType==0 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType1Long;  //타입1지진동에 대한 교축직각방향 안정검토
	else if (nType==1 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType2Trav;  //타입2지진동에 대한 교축방향 안정검토
	else if (nType==1 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType2Long;  //타입2지진동에 대한 교축직각방향 안정검토

	double  dSrcForce=0.0, dSrcMoment=0.0, dSrcLength=0.0;
	double  dTgtForce=0.0, dTgtMoment=0.0, dTgtLength=0.0;
	
	dSrcMoment = ResType.dMy;
	dTgtMoment = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, ResData.DataUnit.nBase_Force, ResData.DataUnit.nBase_Length, 
																											 D_UNITSYS_BASE_MOMENT, dSrcMoment);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtMoment);
	pWord->WriteCell(2, 1, strTemp); 
	
	dSrcLength = ResType.dPhiy;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_PERLENGTH, dSrcLength);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtLength);
	pWord->WriteCell(2, 2, strTemp); 
	
	dSrcForce = ResType.dPy;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(2, 3, strTemp); 
	
	dSrcLength = ResType.ddeltay;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtLength);
	pWord->WriteCell(2, 4, strTemp); 
	
	pWord->MoveDown();
	pWord->WriteText(_T("\n"));
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->SetSize(FONT_SIZE_BASE);
}

// 타입 I : nType==0, 타입 II : nType==1
// 교축방향 : nTransverse==0 , 교축직각방향 : nTransverse==1
void CDgnToolMPhi_SeismicReport::WriteHorizontalForce5(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType)
{
	if(nTransverse != 0 && nTransverse != 1)
		return;
	if(nType != 0 && nType != 1)
		return;
	
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}
	
	int iColorIndexGray = 16;  /*wdGray25*/
	
	WriteSub4Title(_LS(IDS_DGN_RLC_EXTREME_HORI_INTERNAL_FORCE_DIST_RELATION), m_pWord);
	
	pWord->SetSize(FONT_SIZE_TABLE_TEXT);
	pWord->SetAlignment(wdAlignTabCenter);
	
	const int iCol = 4;
	float pColWidth[iCol] = { (float)115, (float)115, (float)115, (float)115 };
	pWord->MakeTableExKR(2, iCol, pColWidth, 1);
	
	pWord->SetCellShading(1, 1, iColorIndexGray);
	pWord->SetAlignment(wdAlignTabCenter);
	pWord->SetBold(1);
	CString strTemp=_T("");
	strTemp.Format(_T("%s Sub(M, u)\n(kN Sub(-3881) m)"), _LS(IDS_DGN_RLC_BEND_MOMENT));
	pWord->WriteCell(1, 1, strTemp); 
	
	pWord->MoveRight();
	pWord->SetCellShading(1, 2, iColorIndexGray);
	pWord->SetBold(1);
	strTemp.Format(_T("%s Sub(-3994, u)\n(1/m)"), _LS(IDS_DGN_RLC_CURVATURE));
	pWord->WriteCell(1, 2, strTemp); 
	
	pWord->MoveRight();
	pWord->SetCellShading(1, 3, iColorIndexGray);
	pWord->SetBold(1);
	strTemp.Format(_T("%s Sub(P, u)\n(kN)"), _LS(IDS_DGN_RLC_HORI_INTER_FORCE));
	pWord->WriteCell(1, 3, strTemp); 
	
	pWord->MoveRight();
	pWord->SetCellShading(1, 4, iColorIndexGray);
	pWord->SetBold(1);
	strTemp.Format(_T("%s Sub(-3996, u)\n(m)"), _LS(IDS_DGN_RLC_HORI_DIST));
	pWord->WriteCell(1, 4, strTemp); 
	
	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES ResType;
	if      (nType==0 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType1Trav;  //타입1지진동에 대한 교축방향 안정검토
	else if (nType==0 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType1Long;  //타입1지진동에 대한 교축직각방향 안정검토
	else if (nType==1 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType2Trav;  //타입2지진동에 대한 교축방향 안정검토
	else if (nType==1 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType2Long;  //타입2지진동에 대한 교축직각방향 안정검토
	
	double  dSrcForce=0.0, dSrcMoment=0.0, dSrcLength=0.0;
	double  dTgtForce=0.0, dTgtMoment=0.0, dTgtLength=0.0;
	
	dSrcMoment = ResType.dMu;
	dTgtMoment = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, ResData.DataUnit.nBase_Force, ResData.DataUnit.nBase_Length, 
																											 D_UNITSYS_BASE_MOMENT, dSrcMoment);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtMoment);
	pWord->WriteCell(2, 1, strTemp); 
	
	dSrcLength = ResType.dPhiu;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_PERLENGTH, dSrcLength);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtLength);
	pWord->WriteCell(2, 2, strTemp); 
	
	dSrcForce = ResType.dPu;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtForce);
	pWord->WriteCell(2, 3, strTemp); 
	
	dSrcLength = ResType.ddeltau;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	pWord->MoveRight();
	strTemp.Format(_T("%g"), dTgtLength);
	pWord->WriteCell(2, 4, strTemp); 
	
	pWord->MoveDown();
	pWord->WriteText(_T("\n"));
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->SetSize(FONT_SIZE_BASE);

	// Equation
	pWord->WriteText(_T("\t\t\t\t"));
	strTemp = _T("EQ Sub1(P,u) = \\F(Sub1(M,u), Sub1(h))");
	pWord->WriteEquation(strTemp);
	CString s=_T("Sub(-3881)");
	CString strDot=_T("");
//   strDot.Format(_T("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s")
//                 ,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s);
//   pWord->WriteText(strDot);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t"));
	strTemp = _T("EQ Sub1(-3996, u) = Sub1(-3996, y) + (Sub1(-3994, u) - Sub1(-3994, y))Sub1(L, p)(Sub1(h) - Sub1(L, p)/2)");
	pWord->WriteEquation(strTemp);
//   strDot.Format(_T("%s%s%s%s%s%s%s%s%s%s")
//                 ,s,s,s,s,s,s,s,s,s,s);
//   pWord->WriteText(strDot);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t"));
	strTemp = _T("EQ Sub1(L, p) = 0.2Sub1(h) - 0.1Sub1(D)");
	pWord->WriteEquation(strTemp);
//   strDot.Format(_T("%s%s%s") ,s,s,s);
//   pWord->WriteText(strDot);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(L, p)%s 0.1Sub1(D) Sub1(-3933) Sub1(L, p) Sub1(-3933) 0.5Sub1(D)"), _LS(IDS_DGN_RLC_PLASTICITY_HINGE));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t"));
	strTemp = _T("EQ Sub1(L, p) = 0.2Sub1(h) - 0.1Sub1(D)");
	pWord->WriteEquation(strTemp);
	pWord->WriteText(_T("\n"));

	double dActHeight=0.;

	dSrcLength = ResData.List[nPierIdx].dActHeight;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	dActHeight = dTgtLength;

	double dHeightOrWidth=0.0;
	if      (nTransverse==0) dSrcLength = ResData.List[nPierIdx].dHeight;
	else if (nTransverse==1) dSrcLength = ResData.List[nPierIdx].dWidth;
	else { ASSERT(0); return; }
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	dHeightOrWidth = dTgtLength;

	pWord->WriteText(_T("\t\t\t\t"));
	pWord->WriteText(_T("   "));
	strTemp.Format(_T("EQ = 0.2 Sub(-3916) %g - 0.1 Sub(-3916) %g"), dActHeight, dHeightOrWidth);
	pWord->WriteEquation(strTemp);
	pWord->WriteText(_T("\n"));

	if(ResData.List[nPierIdx].nLpType[nTransverse]==2)
	{
		pWord->WriteText(_T("\t\t\t\t"));
		pWord->WriteText(_T("   "));
		strTemp.Format(_T("EQ = %g m > 0.5Sub1(D) = 0.5 Sub(-3916) %g = %g m"), 0.2*dActHeight-0.1*dHeightOrWidth, dHeightOrWidth, 0.5*dHeightOrWidth);
		pWord->WriteEquation(strTemp);
		pWord->WriteText(_T("\n"));
	}
	else if(ResData.List[nPierIdx].nLpType[nTransverse]==1)
	{
		pWord->WriteText(_T("\t\t\t\t"));
		pWord->WriteText(_T("   "));
		strTemp.Format(_T("EQ = %g m < 0.1Sub1(D) = 0.1 Sub(-3916) %g = %g m"), 0.2*dActHeight-0.1*dHeightOrWidth, dHeightOrWidth, 0.1*dHeightOrWidth);
		pWord->WriteEquation(strTemp);
		pWord->WriteText(_T("\n"));
	}
	else if(ResData.List[nPierIdx].nLpType[nTransverse]==0)
	{
		pWord->WriteText(_T("\t\t\t\t"));
		pWord->WriteText(_T("   "));
		strTemp.Format(_T("EQ = %g m"), 0.2*dActHeight-0.1*dHeightOrWidth);
		pWord->WriteEquation(strTemp);
		pWord->WriteText(_T("\n"));
	}
	else
	{
		ASSERT(0); return;
	}

	dSrcLength = ResData.List[nPierIdx].dLp[nTransverse];
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	pWord->WriteText(_T("\n"));
	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("EQ %s Sub1(L,p) = %g m"), _LS(IDS_DGN_RLC_SO_THAT), dTgtLength);
	pWord->WriteEquation(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\n"));
	pWord->SetSize(FONT_SIZE_SMALL_TEXT);
	pWord->WriteText(_T("\t\t\t\t\t"));
	strTemp.Format(_T("Sub1(D)\t: %s"), _LS(IDS_DGN_RLC_SECTION_HEIGHT_mm));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));
	strTemp.Format(_T("\t\t\t\t\t\t %s"), _LS(IDS_DGN_RLC_SECTION_NUMERICAL_VALUE));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t\t"));
	strTemp.Format(_T("Sub1(-3994, y)\t: %s"), _LS(IDS_DGN_RLC_PIER_SECTIOIN_YIELD_CURVATURE_1_mm));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t\t"));
	strTemp.Format(_T("Sub1(-3994, u)\t: %s"), _LS(IDS_DGN_RLC_PIER_SECTIOIN_EXTREME_CURVATURE_1_mm));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));
	
	pWord->SetSize(FONT_SIZE_BASE);
	pWord->WriteText(_T("\n"));
}

// 타입 I : nType==0, 타입 II : nType==1
// 교축방향 : nTransverse==0 , 교축직각방향 : nTransverse==1
void CDgnToolMPhi_SeismicReport::WriteHorizontalForce6(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType)
{
	if(nTransverse != 0 && nTransverse != 1)
		return;
	if(nType != 0 && nType != 1)
		return;
	
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}
	
	WriteSub4Title(_LS(IDS_DGN_RLC_CONCRETE_STRESS_TRANSFORM_RATIO_RELATION), m_pWord);

	pWord->SetAlignment(wdAlignTabCenter);

	const int iCol = 2;
	float pColWidth[iCol] = { (float)200, (float)200 };
	pWord->MakeTableExKR(1, iCol, pColWidth, 1);
	pWord->SetTableBorder(wdBorderTop       , wdLineStyleNone);
	pWord->SetTableBorder(wdBorderLeft      , wdLineStyleNone);
	pWord->SetTableBorder(wdBorderBottom    , wdLineStyleNone);
	pWord->SetTableBorder(wdBorderRight     , wdLineStyleNone);
	pWord->SetTableBorder(wdBorderVertical  , wdLineStyleNone);

	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES ResType;
	if      (nType==0 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType1Trav;  //타입1지진동에 대한 교축방향 안정검토
	else if (nType==0 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType1Long;  //타입1지진동에 대한 교축직각방향 안정검토
	else if (nType==1 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType2Trav;  //타입2지진동에 대한 교축방향 안정검토
	else if (nType==1 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType2Long;  //타입2지진동에 대한 교축직각방향 안정검토

	//ResType.strFilePath_MPhiFig   = _T("D:\\test3.bmp");  // for image test
	//ResType.strFilePath_PDeltaFig = _T("D:\\test3.bmp");  // for image test

	pWord->AddPicture(ResType.strFilePath_MPhiFig);
	pWord->WriteText(_T("\n"));
	pWord->SetSize(FONT_SIZE_SMALL_TEXT);
	pWord->WriteText(_LS(IDS_DGN_RLC_AS_PIER_BODY_SECTION)); 
	pWord->WriteText(_T("\n"));
	CString strTemp=_T("");
	strTemp.Format(_LS(IDS_DGN_RLC_BEND_MOMENT_M_CURVATURE__s_RELATION), _T("Sub(-3994)"));
	pWord->WriteText(strTemp);

	pWord->MoveRight();
	pWord->AddPicture(ResType.strFilePath_PDeltaFig);
	pWord->WriteText(_T("\n"));
	pWord->SetSize(FONT_SIZE_SMALL_TEXT);
	pWord->WriteText(_LS(IDS_DGN_RLC_AS_UPPER_STRUCTURE_INERTIAL_FORCE_ACTION_POS)); 
	pWord->WriteText(_T("\n"));
	strTemp.Format(_LS(IDS_DGN_RLC_HORI_FORCE_P_HORI_DISP__s_RELATION), _T("Sub(-3996)"));
	pWord->WriteText(strTemp);

	pWord->MoveDown();
	pWord->WriteText(_T("\n"));
	pWord->SetAlignment(wdAlignTabLeft);
	pWord->SetSize(FONT_SIZE_BASE);

	pWord->SetSize(FONT_SIZE_SMALL_TEXT);
	pWord->WriteText(_T("\t\t\t\t\t"));
	strTemp.Format(_T("%s \t\t: Sub1(-3982, s) = %g")
								, _LS(IDS_DGN_RLC_WIDTH_RESTRICTION_VOLUME_RATIO), ResData.List[nPierIdx].dRhos[nTransverse]);
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	double  dSrcForce=0.0;
	double  dTgtForce=0.0;

	dSrcForce = ResData.List[nPierIdx].dEdes[nTransverse];
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, ResData.DataUnit.nBase_Force, ResData.DataUnit.nBase_Length, 
																											D_UNITSYS_BASE_STRESS, dSrcForce);
	pWord->WriteText(_T("\t\t\t\t\t"));
	strTemp.Format(_T("%s \t\t\t\t: Sub1(E, des) = %g N/Sup(mm, 2)")
								, _LS(IDS_DGN_RLC_DESCENT_SLOPE), dTgtForce);
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t\t"));
	strTemp.Format(_T("%s \t: Sub1(-3995, cc) = %g")
								, _LS(IDS_DGN_RLC_MAX_COMP_STRESS_TRANS_RATIO), ResData.List[nPierIdx].decc[nTransverse]);
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t\t"));
	strTemp.Format(_T("%s \t\t\t: Sub1(-3995, cu) = %g")
								, _LS(IDS_DGN_RLC_MAX_TRANS_RATIO), ResData.List[nPierIdx].decu[nTransverse]);
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	dSrcForce = ResData.List[nPierIdx].dSigcc[nTransverse];
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, ResData.DataUnit.nBase_Force, ResData.DataUnit.nBase_Length, 
																											D_UNITSYS_BASE_STRESS, dSrcForce);
	pWord->WriteText(_T("\t\t\t\t\t"));
	strTemp.Format(_T("%s \t\t\t: Sub1(-3981, cc) = %g N/Sup(mm, 2)")
								, _LS(IDS_DGN_RLC_MAX_COMP_INTENSITY), dTgtForce);
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\n"));
	pWord->SetSize(FONT_SIZE_BASE);
}

// 타입 I : nType==0, 타입 II : nType==1
// 교축방향 : nTransverse==0 , 교축직각방향 : nTransverse==1
void CDgnToolMPhi_SeismicReport::WriteShearForce(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType)
{
	if(nTransverse != 0 && nTransverse != 1)
		return;
	if(nType != 0 && nType != 1)
		return;
	
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}

	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES ResType;
	if      (nType==0 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType1Trav;  //타입1지진동에 대한 교축방향 안정검토
	else if (nType==0 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType1Long;  //타입1지진동에 대한 교축직각방향 안정검토
	else if (nType==1 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType2Trav;  //타입2지진동에 대한 교축방향 안정검토
	else if (nType==1 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType2Long;  //타입2지진동에 대한 교축직각방향 안정검토

	_DGN_RESERVE_LATERAL_CAPA_MEMB_RES MembRes;
	MembRes = ResData.List[nPierIdx];

	CString strTemp=_T("");
	pWord->WriteText(_T("\t\t\t"));
	strTemp.Format(_T("Sub1(S, c) = 1000 Sub1(c, c) Sub1(c, e) Sub1(c, pt) Sub1(-3980, c) Sub1(b) Sub1(d)"));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	double  dSrcForce=0.0, dSrcLength=0.0;
	double  dTgtForce=0.0, dTgtLength=0.0;

	double dtc=0.0, dbw=0.0, dDeff=0.0, dSc=0.0;

	dSrcForce = ResType.dtc;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, ResData.DataUnit.nBase_Force, ResData.DataUnit.nBase_Length, 
																											D_UNITSYS_BASE_STRESS, dSrcForce);
	dtc = dTgtForce;

	dSrcLength = ResType.dbw;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	dbw = dTgtLength;

	dSrcLength = ResType.dDeff;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	dDeff = dTgtLength;

	dSrcForce = ResType.dSc;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	dSc = dTgtForce;

	pWord->WriteText(_T("\t\t\t"));
	strTemp.Format(_T("   = 1000 Sub(-3916) %g Sub(-3916) %g Sub(-3916) %g Sub(-3916) %g Sub(-3916) %g Sub(-3916) %g = %g kN")
									, ResType.dcc, ResType.dce, ResType.dcpt, dtc, dbw, dDeff, dSc);
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t"));
	strTemp.Format(_T("EQ Sub1(S, s) = \\F(Sub1(A,w) Sub1(-3981,sy) Sub1(d) ( Sub1(sin)Sub1(-3983) Sub(-4053) Sub1(cos)Sub1(-3983) ),1000 Sub(-3916) 1.15a)"));
	pWord->WriteEquation(strTemp);
	pWord->WriteText(_T("\n"));

	double dAw_tie=0.0, dfsy_tie=0.0, da_tie=0.0, dSs=0.0;

	dSrcLength = MembRes.dAw_tie[nTransverse];
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_MM, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_AREA, dSrcLength);
	dAw_tie = dTgtLength;

	dSrcForce = MembRes.dfsy_tie;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, ResData.DataUnit.nBase_Force, ResData.DataUnit.nBase_Length, 
																											D_UNITSYS_BASE_STRESS, dSrcForce);
	dfsy_tie = dTgtForce;

	dSrcLength = MembRes.da_tie;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	da_tie = dTgtLength;

	dSrcForce = ResType.dSs;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	dSs = dTgtForce;

	double dSin = sin(MembRes.dAngle_tie);
	double dCos = cos(MembRes.dAngle_tie);
	strTemp.Format(_T("EQ = \\F(%g Sub(-3916) %g Sub(-3916) %g ( Sub1(sin)%g Sub(-4053) Sub1(cos)%g ),1000 Sub(-3916) 1.15 Sub(-3916) %g) = %g kN")
									, dAw_tie, dfsy_tie, dDeff, MembRes.dAngle_tie, MembRes.dAngle_tie, da_tie, dSs);
	pWord->WriteText(_T("\t\t\t   "));
	pWord->WriteEquation(strTemp);
	pWord->WriteText(_T("\n"));
	pWord->WriteText(_T("\n"));

	double dPs=0.0;
	dSrcForce = ResType.dPs;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	dPs = dTgtForce;
	pWord->WriteText(_T("\t\t\t"));
	strTemp.Format(_T("Sub1(P, s) = Sub1(S, c) Sub(-4053) Sub1(S, s) = %g Sub(-4053) %g = %g kN")
									, dSc, dSs, dPs);
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));
	pWord->WriteText(_T("\n"));

	double dPs0=0.0;
	dSrcForce = ResType.dPs0;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	dPs0 = dTgtForce;
	pWord->WriteText(_T("\t\t\t"));
	strTemp.Format(_T("Sub1(P, s0) = Sub1(S, c) / Sub1(c, c) Sub(-4053) Sub1(S, s) = %g / %g Sub(-4053) %g = %g kN")
									, dSc, ResType.dcc, dSs, dPs0);
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t"));
	pWord->WriteText(_LS(IDS_DGN_RLC_FROM_HERE));
	pWord->WriteText(_T("\n"));

	pWord->SetSize(FONT_SIZE_SMALL_TEXT);

	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(P, s)\t: %s (kN)"), _LS(IDS_DGN_RLC_SHEAR_INTER_FORCE));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));
	
	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(P, s0)\t: %s (kN)"), _LS(IDS_DGN_RLC_RIGHT_N_WRONG_ACTION_SHEAR_FORCE));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(S, c)\t: %s (kN)"), _LS(IDS_DGN_RLC_CONCRETE_SHEAR_FORCE));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(S, s)\t: %s (kN)"), _LS(IDS_DGN_RLC_IRON_SHEAR_FORCE));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(c, c)\t: %s"), _LS(IDS_DGN_RLC_LOAD_RIGHT_N_WRONG_ACTION_COEFF));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t"));
	CString strTemp2=_T("");
	strTemp2.Format(_LS(IDS_DGN_RLC_PIER_BODY_SECTION_VALID_HEIGHT__s_COEFF), _T("Sub1(d)"));
	strTemp.Format(_T("Sub1(c, e)\t: %s"), strTemp2);
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(c, pt)\t: %s"), _LS(IDS_DGN_RLC_AXIS_DIR_IRON_RATIO_Pt_COEFF));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(-3980, c)\t: %s (N/Sup(mm, 2))"), _LS(IDS_DGN_RLC_CONCRETE_AVERAGE_SHEAR_STRESS));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(b)\t: %s (m)"), _LS(IDS_DGN_RLC_PIER_BODY_SECTION_WIDTH));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(d)\t: %s (m)"), _LS(IDS_DGN_RLC_PIER_BODY_SECTION_VALID_HEIGHT));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t"));
	strTemp2.Format(_LS(IDS_DGN_RLC_DIST__s_ANGLE__s_BAND_IRON_AREA), _T("Sub1(a)"), _T("Sub1(-3983)"));
	strTemp.Format(_T("Sub1(A, w)\t: %s (Sup(mm, 2))"), strTemp2);
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(-3981, sy)\t: %s (N/Sup(mm, 2))"), _LS(IDS_DGN_RLC_BAND_IRON_YIELD_POINT));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(-3983)\t: %s"), _LS(IDS_DGN_RLC_BAND_IRON_VERTICAL_AXIS_ANGLE));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(a)\t: %s (m)"), _LS(IDS_DGN_RLC_BAND_IRON_DIST));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));
	pWord->WriteText(_T("\n"));

	pWord->SetSize(FONT_SIZE_BASE);
}

// 타입 I : nType==0, 타입 II : nType==1
// 교축방향 : nTransverse==0 , 교축직각방향 : nTransverse==1
void CDgnToolMPhi_SeismicReport::WriteFailureType(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType)
{
	if(nTransverse != 0 && nTransverse != 1)
		return;
	if(nType != 0 && nType != 1)
		return;
	
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}

	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES ResType;
	if      (nType==0 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType1Trav;  //타입1지진동에 대한 교축방향 안정검토
	else if (nType==0 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType1Long;  //타입1지진동에 대한 교축직각방향 안정검토
	else if (nType==1 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType2Trav;  //타입2지진동에 대한 교축방향 안정검토
	else if (nType==1 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType2Long;  //타입2지진동에 대한 교축직각방향 안정검토

	double  dSrcForce=0.0;
	double  dTgtForce=0.0;

	double dPu=0.0, dPs=0.0, dPc=0.0, dPs0=0.0;

	dSrcForce = ResType.dPu;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	dPu = dTgtForce;

	dSrcForce = ResType.dPs;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	dPs = dTgtForce;

	dSrcForce = ResType.dPc;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	dPc = dTgtForce;

	dSrcForce = ResType.dPs0;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	dPs0 = dTgtForce;

	CString strTemp=_T("");
	if(ResType.nFailureType==0) // 휨파괴형
	{
		pWord->WriteText(_T("\t\t\t"));
		strTemp.Format(_T("Sub1(P, u) = %g kN Sub(-3933) Sub1(P, s) = %g kN"), dPu, dPs);
		pWord->WriteText(strTemp);
		pWord->WriteText(_T("\n"));

		pWord->WriteText(_T("\t\t\t"));
		strTemp.Format(_T("Sub1(P, c) = %g kN < Sub1(P, u) = %g kN"), dPc, dPu);
		pWord->WriteText(strTemp);
		pWord->WriteText(_T("\n"));

		pWord->WriteText(_T("\t\t"));
		strTemp.Format(_LS(IDS_DGN_RLC_AS__s_IS_DECIDED), ResType.strFailureType);
		pWord->WriteText(strTemp);
		pWord->WriteText(_T("\n"));
	}
	else if(ResType.nFailureType==1 || ResType.nFailureType==2)
	{
		pWord->WriteText(_T("\t\t\t"));
		strTemp.Format(_T("Sub1(P, u) = %g kN > Sub1(P, s) = %g kN"), dPu, dPs);
		pWord->WriteText(strTemp);
		pWord->WriteText(_T("\n"));
		
		CString strSign=_T("");
		if      (ResType.nFailureType==1) strSign = _T("Sub(-3933)");
		else if (ResType.nFailureType==2) strSign = _T(">");

		pWord->WriteText(_T("\t\t\t"));
		strTemp.Format(_T("Sub1(P, u) = %g kN %s Sub1(P, s0) = %g kN"), dPu, strSign, dPs0);
		pWord->WriteText(strTemp);
		pWord->WriteText(_T("\n"));
		
		pWord->WriteText(_T("\t\t"));
		strTemp.Format(_LS(IDS_DGN_RLC_AS__s_IS_DECIDED), ResType.strFailureType);
		pWord->WriteText(strTemp);
		pWord->WriteText(_T("\n"));
	}

	pWord->WriteText(_T("\n"));
}

// 타입 I : nType==0, 타입 II : nType==1
// 교축방향 : nTransverse==0 , 교축직각방향 : nTransverse==1
void CDgnToolMPhi_SeismicReport::WriteEarthquakeExam(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType)
{
	if(nTransverse != 0 && nTransverse != 1)
		return;
	if(nType != 0 && nType != 1)
		return;
	
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}
	
	m_nSub4Num = 0;
	
	WriteEarthquakeExam1(pWord, ResData, nTransverse, nPierIdx, nType);
	WriteEarthquakeExam2(pWord, ResData, nTransverse, nPierIdx, nType);
	WriteEarthquakeExam3(pWord, ResData, nTransverse, nPierIdx, nType);
	WriteEarthquakeExam4(pWord, ResData, nTransverse, nPierIdx, nType);
	WriteEarthquakeExam5(pWord, ResData, nTransverse, nPierIdx, nType);
	WriteEarthquakeExam6(pWord, ResData, nTransverse, nPierIdx, nType);
}

// 타입 I : nType==0, 타입 II : nType==1
// 교축방향 : nTransverse==0 , 교축직각방향 : nTransverse==1
void CDgnToolMPhi_SeismicReport::WriteEarthquakeExam1(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType)
{
	if(nTransverse != 0 && nTransverse != 1)
		return;
	if(nType != 0 && nType != 1)
		return;
	
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}
	
	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES ResType;
	if      (nType==0 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType1Trav;  //타입1지진동에 대한 교축방향 안정검토
	else if (nType==0 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType1Long;  //타입1지진동에 대한 교축직각방향 안정검토
	else if (nType==1 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType2Trav;  //타입2지진동에 대한 교축방향 안정검토
	else if (nType==1 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType2Long;  //타입2지진동에 대한 교축직각방향 안정검토
	
	WriteSub4Title(_LS(IDS_DGN_RLC_DESIGN_HORI_INTENSITY), m_pWord);

	CString strTemp=_T("");
	pWord->WriteText(_T("\t\t\t"));
	strTemp.Format(_T("Sub1(k, hc) = Sub1(c, s) Sub1(c, z) Sub1(k, hc0) = %g Sub(-3916) %g Sub(-3916) %g = %g"), ResType.dcs, ResType.dcz, ResType.dKhc0, ResType.dKhc);
	pWord->WriteText(strTemp);
	double dMin=0.0;
	if      (nType==0)  dMin = ResData.dczKhc0_LimType1;
	else if (nType==1)  dMin = ResData.dczKhc0_LimType2;
	//strTemp.Format(_T("\t\t( %g Sub(-3933) Sub1(c, z) Sub1(k, hc0) )"), dMin);
	strTemp.Format(_T("\t\t( %g Sub(-3933) Sub1(c, z) Sub1(k, hc0) ) , ( %g Sub1(c, z) Sub(-3933) Sub1(k, hc) )"), dMin, 0.4);
	//pWord->WriteText(strTemp);
	//pWord->WriteText(_T("\n"));
	//strTemp.Format(_T("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t( %g Sub1(c, z) Sub(-3933) Sub1(k, hc) )"), 0.4);
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n\n"));

	pWord->WriteText(_T("\t\t\t"));
	pWord->WriteText(_LS(IDS_DGN_RLC_FROM_HERE));
	pWord->WriteText(_T("\n"));

	pWord->SetSize(FONT_SIZE_SMALL_TEXT);

	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(k, hc)\t: %s"), _LS(IDS_DGN_RLC_EQ_HORI_INTER_FORCE_DESIGN_HORI_INTENSITY));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(k, hc0)\t: %s"), _LS(IDS_DGN_RLC_EQ_HORI_INTER_FORCE_DESIGN_HORI_INTENSITY_STANDARD));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(c, s)\t: %s"), _LS(IDS_DGN_RLC_STRUCTURE_PROPERTY_COEFF));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(c, z)\t: %s"), _LS(IDS_DGN_RLC_REGION_COEFF));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\n"));
	pWord->SetSize(FONT_SIZE_BASE);
}

// 타입 I : nType==0, 타입 II : nType==1
// 교축방향 : nTransverse==0 , 교축직각방향 : nTransverse==1
void CDgnToolMPhi_SeismicReport::WriteEarthquakeExam2(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType)
{
	if(nTransverse != 0 && nTransverse != 1)
		return;
	if(nType != 0 && nType != 1)
		return;
	
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}
	
	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES ResType;
	if      (nType==0 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType1Trav;  //타입1지진동에 대한 교축방향 안정검토
	else if (nType==0 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType1Long;  //타입1지진동에 대한 교축직각방향 안정검토
	else if (nType==1 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType2Trav;  //타입2지진동에 대한 교축방향 안정검토
	else if (nType==1 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType2Long;  //타입2지진동에 대한 교축직각방향 안정검토
	
	WriteSub4Title(_LS(IDS_DGN_RLC_EQUV_WEIGHT), m_pWord);
	
	double dWu=0.0, dWp=0.0, dW=0.0;
		
	double  dSrcForce=0.0;
	double  dTgtForce=0.0;

	dSrcForce = ResData.List[nPierIdx].dWu;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	dWu = dTgtForce;

	dSrcForce = ResData.List[nPierIdx].dWp;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	dWp = dTgtForce;

	dSrcForce = ResType.dW;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	dW = dTgtForce;

	CString strTemp=_T("");
	pWord->WriteText(_T("\t\t\t"));
	strTemp.Format(_T("Sub1(W) = Sub1(W, u) Sub(-4053) Sub1(c, p) Sub1(W, p) = %g Sub(-4053) %g Sub(-3916) %g = %g kN")
									, dWu, ResType.dcp, dWp, dW);
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n\n"));
	
	pWord->WriteText(_T("\t\t\t"));
	pWord->WriteText(_LS(IDS_DGN_RLC_FROM_HERE));
	pWord->WriteText(_T("\n"));
	
	pWord->SetSize(FONT_SIZE_SMALL_TEXT);
	
	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(W)\t: %s (kN)"), _LS(IDS_DGN_RLC_EQ_HORI_INTER_FORCE_EQUV_WEIGHT));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));
	
	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(W, u)\t: %s (kN)"), _LS(IDS_DGN_RLC_PIER_BODY_UPPER_STRUCTURE_WEIGHT));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));
	
	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(W, p)\t: %s (kN)"), _LS(IDS_DGN_RLC_PIER_BODY_WEIGHT));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));
	
	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(c, p)\t: %s"), _LS(IDS_DGN_RLC_EQUV_WEIGHT_PROD_COEFF));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));
	
	pWord->WriteText(_T("\n"));
	pWord->SetSize(FONT_SIZE_BASE);
}

// 타입 I : nType==0, 타입 II : nType==1
// 교축방향 : nTransverse==0 , 교축직각방향 : nTransverse==1
void CDgnToolMPhi_SeismicReport::WriteEarthquakeExam3(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType)
{
	if(nTransverse != 0 && nTransverse != 1)
		return;
	if(nType != 0 && nType != 1)
		return;
	
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}
	
	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES ResType;
	if      (nType==0 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType1Trav;  //타입1지진동에 대한 교축방향 안정검토
	else if (nType==0 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType1Long;  //타입1지진동에 대한 교축직각방향 안정검토
	else if (nType==1 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType2Trav;  //타입2지진동에 대한 교축방향 안정검토
	else if (nType==1 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType2Long;  //타입2지진동에 대한 교축직각방향 안정검토
	
	WriteSub4Title(_LS(IDS_DGN_RLC_STRUCTURE_PROPERTY_COEFF), m_pWord);
	
	CString strTemp=_T("");
	pWord->WriteText(_T("\t\t\t"));
	strTemp.Format(_T("EQ Sub1(c, s) = \\F(1,\\R(2 Sub1(-3987,a) - 1)) = \\F(1,\\R(2 Sub(-3916) %g - 1)) = %g"), ResType.dmua, ResType.dcs);
	pWord->WriteEquation(strTemp);
	pWord->WriteText(_T("\n\n"));
	
	pWord->WriteText(_T("\t\t\t"));
	pWord->WriteText(_LS(IDS_DGN_RLC_FROM_HERE));
	pWord->WriteText(_T("\n"));
	
	pWord->SetSize(FONT_SIZE_SMALL_TEXT);
	
	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(c, s)\t: %s"), _LS(IDS_DGN_RLC_STRUCTURE_PROPERTY_COEFF));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));
	
	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(-3987,a)\t: %s"), _LS(IDS_DGN_RLC_IRON_CONCRETE_PIER_ALLOW_RATIO));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));
	
	pWord->WriteText(_T("\n"));
	pWord->SetSize(FONT_SIZE_BASE);
}

// 타입 I : nType==0, 타입 II : nType==1
// 교축방향 : nTransverse==0 , 교축직각방향 : nTransverse==1
void CDgnToolMPhi_SeismicReport::WriteEarthquakeExam4(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType)
{
	if(nTransverse != 0 && nTransverse != 1)
		return;
	if(nType != 0 && nType != 1)
		return;
	
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}
	
	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES ResType;
	if      (nType==0 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType1Trav;  //타입1지진동에 대한 교축방향 안정검토
	else if (nType==0 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType1Long;  //타입1지진동에 대한 교축직각방향 안정검토
	else if (nType==1 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType2Trav;  //타입2지진동에 대한 교축방향 안정검토
	else if (nType==1 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType2Long;  //타입2지진동에 대한 교축직각방향 안정검토

	if(ResType.nFailureType!=0 && ResType.nFailureType!=1 && ResType.nFailureType!=2)
	{ ASSERT(0); return; }

	WriteSub4Title(_LS(IDS_DGN_RLC_ALLOWANCE_RATIO), m_pWord);
	
	CString strTemp=_T("");
	
	if(ResType.nFailureType==0)
	{
		double ddeltau=0.0, ddeltay=0.0;

		double  dSrcLength=0.0;
		double  dTgtLength=0.0;

		dSrcLength = ResType.ddeltau;
		dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																														D_UNITSYS_BASE_LENGTH, dSrcLength);
		ddeltau = dTgtLength;

		dSrcLength = ResType.ddeltay;
		dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																														D_UNITSYS_BASE_LENGTH, dSrcLength);
		ddeltay = dTgtLength;

		pWord->WriteText(_T("\t\t\t"));
		strTemp.Format(_T("EQ Sub1(-3987,a) = 1 Sub(-4053) \\F(Sub1(-3996,u) Sub(-4051) Sub1(-3996,y),Sub1(-3999) Sub1(-3996,y)) = 1 Sub(-4053) \\F(%g Sub(-4051) %g,%g Sub(-3916) %g) = %g")
										, ddeltau, ddeltay, ResType.dalpha, ddeltay, ResType.dmua);
		pWord->WriteEquation(strTemp);
		pWord->WriteText(_T("\n\n"));
		
		pWord->WriteText(_T("\t\t\t"));
		pWord->WriteText(_LS(IDS_DGN_RLC_FROM_HERE));
		pWord->WriteText(_T("\n"));
		
		pWord->SetSize(FONT_SIZE_SMALL_TEXT);
		
		pWord->WriteText(_T("\t\t\t\t"));
		strTemp.Format(_T("Sub1(-3987,a)\t: %s"), _LS(IDS_DGN_RLC_IRON_CONCRETE_PIER_ALLOW_RATIO));
		pWord->WriteText(strTemp);
		pWord->WriteText(_T("\n"));
		
		pWord->WriteText(_T("\t\t\t\t"));
		strTemp.Format(_T("Sub1(-3996,u)\t: %s (m)"), _LS(IDS_DGN_RLC_IRON_CONCRETE_PIER_EXT_DIST));
		pWord->WriteText(strTemp);
		pWord->WriteText(_T("\n"));

		pWord->WriteText(_T("\t\t\t\t"));
		strTemp.Format(_T("Sub1(-3996,y)\t: %s (m)"), _LS(IDS_DGN_RLC_IRON_CONCRETE_PIER_YIELD_DIST));
		pWord->WriteText(strTemp);
		pWord->WriteText(_T("\n"));

		pWord->WriteText(_T("\t\t\t\t"));
		strTemp.Format(_T("Sub1(-3999)\t: %s"), _LS(IDS_DGN_RLC_PIER_ALLOW_RATIO_PROD_SAFE_COEFF));
		pWord->WriteText(strTemp);
		pWord->WriteText(_T("\n"));

		pWord->WriteText(_T("\n"));
		pWord->SetSize(FONT_SIZE_BASE);
	}
	else if(ResType.nFailureType==1 || ResType.nFailureType==2)
	{
		pWord->WriteText(_T("\t\t\t"));
		strTemp.Format(_T("Sub1(-3987,a) = 1.0"));
		pWord->WriteText(strTemp);
		pWord->WriteText(_T("\n\n"));

		pWord->WriteText(_T("\t\t\t"));
		pWord->WriteText(_LS(IDS_DGN_RLC_FROM_HERE));
		pWord->WriteText(_T("\n"));
		
		pWord->SetSize(FONT_SIZE_SMALL_TEXT);
		
		pWord->WriteText(_T("\t\t\t\t"));
		strTemp.Format(_T("Sub1(-3987,a)\t: %s"), _LS(IDS_DGN_RLC_IRON_CONCRETE_PIER_ALLOW_RATIO));
		pWord->WriteText(strTemp);
		pWord->WriteText(_T("\n"));

		pWord->WriteText(_T("\n"));
		pWord->SetSize(FONT_SIZE_BASE);
	}
}

// 타입 I : nType==0, 타입 II : nType==1
// 교축방향 : nTransverse==0 , 교축직각방향 : nTransverse==1
void CDgnToolMPhi_SeismicReport::WriteEarthquakeExam5(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType)
{
	if(nTransverse != 0 && nTransverse != 1)
		return;
	if(nType != 0 && nType != 1)
		return;
	
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}
	
	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES ResType;
	if      (nType==0 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType1Trav;  //타입1지진동에 대한 교축방향 안정검토
	else if (nType==0 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType1Long;  //타입1지진동에 대한 교축직각방향 안정검토
	else if (nType==1 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType2Trav;  //타입2지진동에 대한 교축방향 안정검토
	else if (nType==1 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType2Long;  //타입2지진동에 대한 교축직각방향 안정검토
	
	if(ResType.nFailureType!=0 && ResType.nFailureType!=1 && ResType.nFailureType!=2)
	{ ASSERT(0); return; }
	
	WriteSub4Title(_LS(IDS_DGN_RLC_EQ_HORI_INTER_FORCE), m_pWord);
	
	double  dSrcForce=0.0;
	double  dTgtForce=0.0;
	
	dSrcForce = ResType.dPa;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);

	CString strTemp=_T("");
	if(ResType.nFailureType==0 || ResType.nFailureType==1)
	{
		pWord->WriteText(_T("\t\t\t"));
		strTemp.Format(_T("Sub1(P, a) = Sub1(P, u) = %g kN"), dTgtForce);
		pWord->WriteText(strTemp);
		pWord->WriteText(_T("\n\n"));
		
		pWord->WriteText(_T("\t\t\t"));
		pWord->WriteText(_LS(IDS_DGN_RLC_FROM_HERE));
		pWord->WriteText(_T("\n"));
		
		pWord->SetSize(FONT_SIZE_SMALL_TEXT);
		
		pWord->WriteText(_T("\t\t\t\t"));
		strTemp.Format(_T("Sub1(P, a)\t: %s (kN)"), _LS(IDS_DGN_RLC_IRON_CONCRETE_PIER_EQ_HORI_INTER_FORCE));
		pWord->WriteText(strTemp);
		pWord->WriteText(_T("\n"));
		
		pWord->WriteText(_T("\n"));
		pWord->SetSize(FONT_SIZE_BASE);
	}
	else if(ResType.nFailureType==2)
	{
		pWord->WriteText(_T("\t\t\t"));
		strTemp.Format(_T("Sub1(P, a) = Sub1(P, s0) = %g kN"), dTgtForce);
		pWord->WriteText(strTemp);
		pWord->WriteText(_T("\n\n"));
		
		pWord->WriteText(_T("\t\t\t"));
		pWord->WriteText(_LS(IDS_DGN_RLC_SHEAR_INTER_FORCE));
		pWord->WriteText(_T("\n"));
		
		pWord->SetSize(FONT_SIZE_SMALL_TEXT);
		
		pWord->WriteText(_T("\t\t\t\t"));
		strTemp.Format(_T("Sub1(P, a)\t: %s (kN)"), _LS(IDS_DGN_RLC_IRON_CONCRETE_PIER_EQ_HORI_INTER_FORCE));
		pWord->WriteText(strTemp);
		pWord->WriteText(_T("\n"));
		
		pWord->WriteText(_T("\n"));
		pWord->SetSize(FONT_SIZE_BASE);
	}
}

// 타입 I : nType==0, 타입 II : nType==1
// 교축방향 : nTransverse==0 , 교축직각방향 : nTransverse==1
void CDgnToolMPhi_SeismicReport::WriteEarthquakeExam6(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType)
{
	if(nTransverse != 0 && nTransverse != 1)
		return;
	if(nType != 0 && nType != 1)
		return;
	
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}
	
	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES ResType;
	if      (nType==0 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType1Trav;  //타입1지진동에 대한 교축방향 안정검토
	else if (nType==0 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType1Long;  //타입1지진동에 대한 교축직각방향 안정검토
	else if (nType==1 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType2Trav;  //타입2지진동에 대한 교축방향 안정검토
	else if (nType==1 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType2Long;  //타입2지진동에 대한 교축직각방향 안정검토
	
	WriteSub4Title(_LS(IDS_DGN_RLC_EQ_HORI_INTER_EXAM), m_pWord);
	
	CString strTemp=_T("");
	
	double dW=0.0, dPa=0.0; 

	double  dSrcForce=0.0;
	double  dTgtForce=0.0;
	
	dSrcForce = ResType.dW;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	dW = dTgtForce;

	dSrcForce = ResType.dPa;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	dPa = dTgtForce;


	pWord->WriteText(_T("\t\t\t"));
	CString strSign=_T("");
	if(ResType.bCapaChk)  strSign=_T("Sub(-3917)");
	else                  strSign=_T("<");
	strTemp.Format(_T("Sub1(P, a) = %g kN %s Sub1(k, hc) Sub1(W) = %g Sub(-3916) %g = %g kN")
									, dPa, strSign, ResType.dKhc, dW, ResType.dKhc*dW);
	pWord->WriteText(strTemp);
//   pWord->WriteText(_T("\n\n"));
//   
//   pWord->WriteText(_T("\t\t\t"));
//   pWord->WriteText(_LS(IDS_DGN_RLC_FROM_HERE));
//   pWord->WriteText(_T("\n"));
//   
//   pWord->SetSize(FONT_SIZE_SMALL_TEXT);
//   
//   pWord->WriteText(_T("\t\t\t\t"));
//   strTemp.Format(_T("Sub1(P, a)\t: %s (kN)"), _LS(IDS_DGN_RLC_IRON_CONCRETE_PIER_EQ_HORI_INTER_FORCE));
//   pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));
	
	pWord->WriteText(_T("\n"));
	pWord->SetSize(FONT_SIZE_BASE);
}

// 타입 I : nType==0, 타입 II : nType==1
// 교축방향 : nTransverse==0 , 교축직각방향 : nTransverse==1
void CDgnToolMPhi_SeismicReport::WriteRemainingDispExam(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType)
{
	if(ResData.nDgnLevel!=1)
		return;
	if(nTransverse != 0 && nTransverse != 1)
		return;
	if(nType != 0 && nType != 1)
		return;
	
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}
	
	m_nSub4Num = 0;
	
	WriteRemainingDispExam1(pWord, ResData, nTransverse, nPierIdx, nType);
	WriteRemainingDispExam2(pWord, ResData, nTransverse, nPierIdx, nType);
	WriteRemainingDispExam3(pWord, ResData, nTransverse, nPierIdx, nType);
}

// 타입 I : nType==0, 타입 II : nType==1
// 교축방향 : nTransverse==0 , 교축직각방향 : nTransverse==1
void CDgnToolMPhi_SeismicReport::WriteRemainingDispExam1(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType)
{
	if(ResData.nDgnLevel!=1)
		return;
	if(nTransverse != 0 && nTransverse != 1)
		return;
	if(nType != 0 && nType != 1)
		return;
	
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}
	
	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES ResType;
	if      (nType==0 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType1Trav;  //타입1지진동에 대한 교축방향 안정검토
	else if (nType==0 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType1Long;  //타입1지진동에 대한 교축직각방향 안정검토
	else if (nType==1 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType2Trav;  //타입2지진동에 대한 교축방향 안정검토
	else if (nType==1 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType2Long;  //타입2지진동에 대한 교축직각방향 안정검토
	
	WriteSub4Title(_LS(IDS_DGN_RLC_PIER_RESPONSE_RATIO), m_pWord);
	
	double dW=0.0, dPa=0.0;
	
	double  dSrcForce=0.0;
	double  dTgtForce=0.0;
	
	dSrcForce = ResType.dW;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	dW = dTgtForce;

	dSrcForce = ResType.dPa;
	dTgtForce = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KN, 0, ResData.DataUnit.nBase_Force, 0, 
																											D_UNITSYS_BASE_FORCE, dSrcForce);
	dPa = dTgtForce;

	CString strTemp=_T("");
	pWord->WriteText(_T("\t\t\t"));
	strTemp.Format(_T("EQ Sub1(-3987,r) = \\F(1,2) { ( \\F(Sub1(c,z) Sub1(k,hc0) Sub1(W),Sub1(P,a)) )Sup(, 2) Sub(-4053) 1 } = \\F(1,2) { ( \\F(%g Sub(-3916) %g Sub(-3916) %g,%g) )Sup(, 2) Sub(-4053) 1 } = %g")
									, ResType.dcz, ResType.dKhc0, dW, dPa, ResType.dmur);
	pWord->WriteEquation(strTemp);
	pWord->WriteText(_T("\n\n"));
	
	pWord->WriteText(_T("\t\t\t"));
	pWord->WriteText(_LS(IDS_DGN_RLC_FROM_HERE));
	pWord->WriteText(_T("\n"));
	
	pWord->SetSize(FONT_SIZE_SMALL_TEXT);
	
	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(-3987,r)\t: %s"), _LS(IDS_DGN_RLC_PIER_RESPONSE_RATIO));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));
	
	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(k, hc0)\t: %s"), _LS(IDS_DGN_RLC_EQ_HORI_INTER_FORCE_DESIGN_HORI_INTENSITY_STANDARD));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(c, z)\t: %s"), _LS(IDS_DGN_RLC_REGION_COEFF));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(W)\t: %s (kN)"), _LS(IDS_DGN_RLC_EQ_HORI_INTER_FORCE_EQUV_WEIGHT));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));
	
	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(P, a)\t: %s (kN)"), _LS(IDS_DGN_RLC_PIER_EQ_HORI_INTER_FORCE));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));
	
	pWord->WriteText(_T("\n"));
	pWord->SetSize(FONT_SIZE_BASE);
}

// 타입 I : nType==0, 타입 II : nType==1
// 교축방향 : nTransverse==0 , 교축직각방향 : nTransverse==1
void CDgnToolMPhi_SeismicReport::WriteRemainingDispExam2(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType)
{
	if(ResData.nDgnLevel!=1)
		return;
	if(nTransverse != 0 && nTransverse != 1)
		return;
	if(nType != 0 && nType != 1)
		return;
	
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}
	
	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES ResType;
	if      (nType==0 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType1Trav;  //타입1지진동에 대한 교축방향 안정검토
	else if (nType==0 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType1Long;  //타입1지진동에 대한 교축직각방향 안정검토
	else if (nType==1 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType2Trav;  //타입2지진동에 대한 교축방향 안정검토
	else if (nType==1 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType2Long;  //타입2지진동에 대한 교축직각방향 안정검토
	
	WriteSub4Title(_LS(IDS_DGN_RLC_REMAINING_DISP), m_pWord);
	
	double ddeltay=0.0, ddelta0=0.0, ddeltaR=0.0;
	
	double  dSrcLength=0.0;
	double  dTgtLength=0.0;
	
	dSrcLength = ResType.ddeltay;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	ddeltay = dTgtLength;

	dSrcLength = ResData.List[nPierIdx].ddelta0[nTransverse];
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	ddelta0 = dTgtLength;

	dSrcLength = ResType.ddeltaR;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	ddeltaR = dTgtLength;

	CString strTemp=_T("");
	pWord->WriteText(_T("\t\t\t"));
	strTemp.Format(_T("Sub1(-3996, R) = Sub1(c, R) (Sub1(-3987,r) Sub(-4051) 1) (1 Sub(-4051) Sub1(r)) (Sub(-3996,y) Sub(-4051) Sub(-3996,0)) = %g Sub(-3916) (%g Sub(-4051) 1) (1 Sub(-4051) %g) (%g Sub(-4051) %g)")
									, ResType.dcR, ResType.dmur, ResType.dr, ddeltay, ddelta0);
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t"));
	strTemp.Format(_T("   = %g m"), ddeltaR);
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n\n"));

	pWord->WriteText(_T("\t\t\t"));
	pWord->WriteText(_LS(IDS_DGN_RLC_FROM_HERE));
	pWord->WriteText(_T("\n"));
	
	pWord->SetSize(FONT_SIZE_SMALL_TEXT);
	
	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(-3996, R)\t: %s (m)"), _LS(IDS_DGN_RLC_PIER_REMAINING_DISP));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));
	
	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(c, R)\t: %s"), _LS(IDS_DGN_RLC_REMAINING_DISP_REVISION_COEFF));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));
	
	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(-3987,r)\t: %s"), _LS(IDS_DGN_RLC_PIER_RESPONSE_RATIO));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));
	
	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(r)\t: %s"), _LS(IDS_DGN_RLC_PIER_YIELD_STIFFNESS_RATIO));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));
	
	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(-3996,y)\t: %s (m)"), _LS(IDS_DGN_RLC_PIER_YIELD_DISP));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(-3996,0)\t: %s (m)"), _LS(IDS_DGN_RLC_PIER_DEADLOAD_DISP));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\n"));
	pWord->SetSize(FONT_SIZE_BASE);
}

// 타입 I : nType==0, 타입 II : nType==1
// 교축방향 : nTransverse==0 , 교축직각방향 : nTransverse==1
void CDgnToolMPhi_SeismicReport::WriteRemainingDispExam3(CWordBase* pWord, _DGN_RESERVE_LATERAL_CAPA_RES& ResData, int nTransverse, int nPierIdx, int nType)
{
	if(ResData.nDgnLevel!=1)
		return;
	if(nTransverse != 0 && nTransverse != 1)
		return;
	if(nType != 0 && nType != 1)
		return;
	
	int nListCnt = ResData.List.GetSize();
	if(nPierIdx >= nListCnt)
	{
		ASSERT(0); return;
	}
	
	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES ResType;
	if      (nType==0 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType1Trav;  //타입1지진동에 대한 교축방향 안정검토
	else if (nType==0 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType1Long;  //타입1지진동에 대한 교축직각방향 안정검토
	else if (nType==1 && nTransverse==0)  ResType = ResData.List[nPierIdx].ResType2Trav;  //타입2지진동에 대한 교축방향 안정검토
	else if (nType==1 && nTransverse==1)  ResType = ResData.List[nPierIdx].ResType2Long;  //타입2지진동에 대한 교축직각방향 안정검토
	
	WriteSub4Title(_LS(IDS_DGN_RLC_REMAINING_DISP_COMPUTATION), m_pWord);
	
	double ddeltaR=0.0, dActHeight=0.0, ddeltaRa=0.0;
	
	double  dSrcLength=0.0;
	double  dTgtLength=0.0;
	
	dSrcLength = ResType.ddeltaR;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	ddeltaR = dTgtLength;

	dSrcLength = ResData.List[nPierIdx].dActHeight;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	dActHeight = dTgtLength;

	dSrcLength = ResData.List[nPierIdx].ddeltaRa;
	dTgtLength = m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_M, 0, ResData.DataUnit.nBase_Length, 
																													D_UNITSYS_BASE_LENGTH, dSrcLength);
	ddeltaRa = dTgtLength;

	CString strTemp=_T("");
	pWord->WriteText(_T("\t\t\t"));
	CString strSign=_T("");
	if(ResType.bCapaChk)  strSign=_T("Sub(-3933)");
	else                  strSign=_T(">");
	strTemp.Format(_T("Sub1(-3996, R) = %g m %s Sub1(-3996, Ra) = Sub1(h) / 100 = %g / 100 = %g m")
									, ddeltaR, strSign, dActHeight, ddeltaRa);
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n\n"));
	
	pWord->WriteText(_T("\t\t\t"));
	pWord->WriteText(_LS(IDS_DGN_RLC_FROM_HERE));
	pWord->WriteText(_T("\n"));
	
	pWord->SetSize(FONT_SIZE_SMALL_TEXT);
	
	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(-3996, Ra)\t: %s"), _LS(IDS_DGN_RLC_PIER_ALLOW_REMAINING_DISP_m_PIER_LOWER_UPPER_STRUCTURE));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("%s"), _LS(IDS_DGN_RLC_INERTIAL_FORCE_ACTION_POS_HEIGHT_1_100));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));

	pWord->WriteText(_T("\t\t\t\t"));
	strTemp.Format(_T("Sub1(h)\t: %s (m)"), _LS(IDS_DGN_RLC_PIER_UPPER_STRUCTURE_INERTIAL_FORCE_ACTION_POS_HEIGHT));
	pWord->WriteText(strTemp);
	pWord->WriteText(_T("\n"));
	
	pWord->WriteText(_T("\n"));
	pWord->SetSize(FONT_SIZE_BASE);
}