// Report_KSCE_USD03.cpp: implementation of the CReport_KSCE_USD03 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Report_KSCE_USD03.h"

#include "..\wg_xl\XmlDocument.h"
#include "..\wg_xl\XslDocument.h"
#include "..\wg_xl\ReportData.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_FileCtrl.h"

#if _MSC_VER >= 1700  // vc++11.0 //sjhuh-2014-02-21
using namespace std;
#include "..\MIT_Lib\rptostream.h"	  // for rptwofstream
#else
#include "..\MIT_Lib\rptostream.h"	// for rptwofstream
#endif

#include <direct.h>
#include <stdlib.h>
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReport_KSCE_USD03::CReport_KSCE_USD03()
{
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);

	CString strFilePathName = GetFilePathName();
	m_pXmlDoc->SetFilePathName(strFilePathName);
	m_pXslDoc->SetFilePathName(strFilePathName);

	m_TextPadding1CssK = 0;
	m_TextPadding2CssK = 0;
	m_TextValueCssK = 0;
	m_TextPicCaptionCssK = 0;
	m_TDCenterCssK = 0;
}

CReport_KSCE_USD03::~CReport_KSCE_USD03()
{

}

CString CReport_KSCE_USD03::GetFilePathName()
{
	CString strPathName = m_pDoc->GetPathName();
	CFileCtrl FileCtrl(strPathName);
	CString strFilePathName=FileCtrl.GetFilePathAndNameWithoutExtension() + _T("_rpt");

	return strFilePathName;
}

BOOL CReport_KSCE_USD03::MakeData()
{
	if(!CReportBase::MakeData()) return FALSE;

	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);

	// Change Unit : N-m
	CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M;
	CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);

	/*
	m_dConvertMUnit = 1.0;
	if(CngIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM)			m_dConvertMUnit = 1000;
	else if(CngIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM)	m_dConvertMUnit = 100;
	else if(CngIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_M)	m_dConvertMUnit = 1;
	else if(CngIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN)	m_dConvertMUnit = 100/2.54;
	else if(CngIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT)	m_dConvertMUnit = 100/30.48;
	*/


	if(!MakeDataFromDB()) return FALSE;

	InitializeData();
	if(!MakeCoverData()) return FALSE;


	if(!MakeWorkData()) return FALSE;

	if(!MakeGraphData()) return FALSE;

	/*
	if(!MakeSeismicCategoryData()) return FALSE;
	*/


	// Change to Original Unit
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);

	return TRUE;
}

BOOL CReport_KSCE_USD03::MakeDataFromDB()
{


	return TRUE;
}

void CReport_KSCE_USD03::InitializeData()
{
	m_pRptData->m_TitleType[3] = 1; // Title4의 형식을 (1)로 변경
	m_iChapNum = 1;
}

BOOL CReport_KSCE_USD03::MakeDefaultStyle()
{
	m_pRptData->InitializeRptData();
	if(!m_pRptData->MakeDefaultStyle()) return FALSE;

	CString strCssName = _T("");
	T_CSS_K CssK=0;
	T_CSS_RPT CssD; CssD.Initialize();
	//Cover SuperTitle
	CssK = 0;
	CssD.Initialize();
	strCssName = _T("SuperTitle");
	m_pRptData->GetNameToCssK(strCssName, CssK);
	if(m_pRptData->GetCssData(CssK,CssD))
	{
		CssD.strFontSize   = _T("20pt");
		CssD.strFontStyle  = _T("normal");
		CssD.strFontWeight = _T("bold");
		CssD.strLineHeight = _T("20px");
	  CssD.strTopMargin  = _T("100px");
		m_pRptData->AddCssData(CssK, CssD);
	}

	//Cover Title
	CssK = 0;
	CssD.Initialize();
	strCssName = _T("Title");
	m_pRptData->GetNameToCssK(strCssName, CssK);
	if(m_pRptData->GetCssData(CssK,CssD))
	{
		CssD.strFontSize    = _T("36pt");
		CssD.strFontFamily  = _T("arial");
		CssD.strFontWeight  = _T("bold");
		CssD.strtxtDeco     = _T("underline");
		CssD.strLineHeight   = _T("40px");
	  CssD.strBottomMargin = _T("300px");
		m_pRptData->AddCssData(CssK, CssD);
	}

	CssK = 0;
	T_CSS_RPT DefaultCellCssD; DefaultCellCssD.Initialize();
	strCssName = _T("Cell");
	m_pRptData->GetNameToCssK(strCssName, CssK);
	if(m_pRptData->GetCssData(CssK,DefaultCellCssD))
	{
	  DefaultCellCssD.strColor     = _T("#0000ff");
		m_pRptData->AddCssData(CssK, DefaultCellCssD);
	}
	////////////////////////////Only Seismic Design Report/////////////////////////////////////////////
	strCssName = _T("Text");
	CssK = 0;
	m_pRptData->GetNameToCssK(strCssName, CssK);
	T_CSS_RPT DefaultTextCssD; DefaultTextCssD.Initialize();
	if(m_pRptData->GetCssData(CssK,DefaultTextCssD))
	{
		T_CSS_RPT TextValueCssD; 
		TextValueCssD = DefaultTextCssD;
		TextValueCssD.strStyleName = _T("TextValue");
	  TextValueCssD.strColor     = _T("#0000ff");
		m_pRptData->AddCssData(m_TextValueCssK, TextValueCssD);

		T_CSS_RPT TextPadding1CssD; TextPadding1CssD = DefaultTextCssD;
		TextPadding1CssD.strStyleName = _T("TextPadding1");
	  TextPadding1CssD.strColor     = _T("#0000ff");
		TextPadding1CssD.strLeftMargin = _T("40px");
		m_pRptData->AddCssData(m_TextPadding1CssK, TextPadding1CssD);

		T_CSS_RPT TextPadding2CssD; TextPadding2CssD = DefaultTextCssD;
		TextPadding2CssD.strStyleName = _T("TextPadding2");
	  TextPadding2CssD.strColor     = _T("#0000ff");
		TextPadding2CssD.strLeftMargin = _T("50px");
		m_pRptData->AddCssData(m_TextPadding2CssK, TextPadding2CssD);
	}

	strCssName = _T("Caption");
	CssK = 0;
	m_pRptData->GetNameToCssK(strCssName, CssK);
	T_CSS_RPT DefaultCaptionCssD; DefaultCaptionCssD.Initialize();
	if(m_pRptData->GetCssData(CssK,DefaultCaptionCssD))
	{
		T_CSS_RPT TextPicCaptionCssD; TextPicCaptionCssD = DefaultCaptionCssD;
		TextPicCaptionCssD.strStyleName = _T("TextPicCaption");
		TextPicCaptionCssD.strTextAlign = _T("center");
		m_pRptData->AddCssData(m_TextPicCaptionCssK, TextPicCaptionCssD);
	}

	strCssName = _T("Cell");
	CssK = 0;
	m_pRptData->GetNameToCssK(strCssName, CssK);
	T_CSS_RPT DefaultTDCssD; DefaultTDCssD.Initialize();
	if(m_pRptData->GetCssData(CssK,DefaultTDCssD))
	{
		T_CSS_RPT TDCenterCssD; TDCenterCssD = DefaultTDCssD;
		TDCenterCssD.strStyleName = _T("Cell_Center");
		TDCenterCssD.strTextAlign = _T("center");
		m_pRptData->AddCssData(m_TDCenterCssK, TDCenterCssD);
	}

	T_CSS_K ColCssK=0;
	T_CSS_RPT ColCss; ColCss.Initialize();
	ColCss.strStyleName=_T("Div2Col");
	ColCss.strWidth = _T("50%");
	m_pRptData->AddCssData(ColCssK, ColCss);

	return TRUE;
}

BOOL CReport_KSCE_USD03::MakeCoverData()
{
	T_REPORT_RPT ReportD; ReportD.Initialize();
	m_pRptData->GetReportData(ReportD);
	ReportD.strTitle = _T("Write Title");
	ReportD.TitleCssK = m_pRptData->m_DefaultCss.CoverTitleCssK;
	ReportD.strSuperTitle = _T("Write Super Title");
	ReportD.SuperTitleCssK = m_pRptData->m_DefaultCss.CoverSupTitleCssK;
	//ReportD.strDate = _T("");
	//ReportD.DateCssK = m_pRptData->m_DefaultCss.CoverDateCssK;
	//ReportD.strCoverBgFileName = _T("image\\ReportCover.jpg");
	//ReportD.CoverBgCssK = pRptData->m_DefaultCss.CoverPicCssK;
	m_pRptData->ModifyReportData(ReportD);

	return TRUE;
}

BOOL CReport_KSCE_USD03::MakeWorkData()
{

	CString strTitle, strText;

	T_CHAP_K ChapK;
	T_PARA_K ParaK;
	T_CHAP_RPT ChapD; ChapD.Initialize();
	T_PARA_RPT ParaD; ParaD.Initialize();
	T_CHAR_RPT CharD; CharD.Initialize();
	T_TABLE_K TableK; T_TABLE_RPT Table;
	T_TR_K TRK; T_TR_RPT TRData; 
	CArray<T_CHAR_K, T_CHAR_K> arCharK;


	ChapD.nLevel = 1;
	ChapD.SeqID = m_iChapNum;
	m_iChapNum++;

	strTitle = _T("Design Work Table");
	ChapD.TitlK = AddTitle(strTitle);

		ParaD.Initialize();
		ParaD.SeqID = 1;
		ParaD.strTitle = _T("Paragraph");

		Table.Initialize();
		Table.nCheck = 1;
		Table.strTitle = _T("Table");


		//Seismic Zone
		TRData.Initialize();
		TRData.arTDK.Add(AddTDString(_T("제품명")));
		TRData.arTDK.Add(AddTDString(_T("작업내용")));
		TRData.arTDK.Add(AddTDString(_T("기존 작업자")));
		TRData.arTDK.Add(AddTDString(_T("인수자")));
		TRData.arTDK.Add(AddTDString(_T("인수일")));
		TRData.arTDK.Add(AddTDString(_T("확인")));
		m_pRptData->AddTRData(TRK, TRData);
		Table.arTRK.Add(TRK);

		TRData.Initialize();
		TRData.arTDK.Add(AddTDString(_T("Gen"), 1, 1, 3));
		TRData.arTDK.Add(AddTDString(_T("1")));
		TRData.arTDK.Add(AddTDString(_T("2")));
		TRData.arTDK.Add(AddTDString(_T("3")));
		TRData.arTDK.Add(AddTDString(_T("4")));
		TRData.arTDK.Add(AddTDString(_T("OK")));
		m_pRptData->AddTRData(TRK, TRData);
		Table.arTRK.Add(TRK);

		TRData.Initialize();
		TRData.arTDK.Add(AddTDString(_T("11")));
		TRData.arTDK.Add(AddTDString(_T("12")));
		TRData.arTDK.Add(AddTDString(_T("13")));
		TRData.arTDK.Add(AddTDString(_T("14")));
		TRData.arTDK.Add(AddTDString(_T("OK")));
		m_pRptData->AddTRData(TRK, TRData);
		Table.arTRK.Add(TRK);

		TRData.Initialize();
		TRData.arTDK.Add(AddTDString(_T("21")));
		TRData.arTDK.Add(AddTDString(_T("22")));
		TRData.arTDK.Add(AddTDString(_T("23")));
		TRData.arTDK.Add(AddTDString(_T("24")));
		TRData.arTDK.Add(AddTDString(_T("OK")));
		m_pRptData->AddTRData(TRK, TRData);
		Table.arTRK.Add(TRK);

		m_pRptData->AddTableData(TableK, Table);
		ParaD.arContentK.Add(TableK);

		m_pRptData->AddParaData(ParaK, ParaD);
		ChapD.arParaK.Add(ParaK);

	m_pRptData->AddChapData(ChapK, ChapD);

	T_REPORT_RPT Report; Report.Initialize();
	m_pRptData->GetReportData(Report);
	Report.arChapK.Add(ChapK);
	m_pRptData->ModifyReportData(Report);

	return TRUE;
}

BOOL CReport_KSCE_USD03::MakeGraphData()
{
	CString strTitle, strText;

	CString strURL;
	T_CHAP_K ChapK, Chap2K;
	T_PARA_K ParaK;
	//T_PICT_K PictK;
	T_TEXT_K TextK;
	T_CHAP_RPT ChapD; ChapD.Initialize();
	T_CHAP_RPT Chap2D; Chap2D.Initialize();
	T_PARA_RPT ParaD; ParaD.Initialize();
	T_CHAR_RPT CharD; CharD.Initialize();
	T_TABLE_K TableK; T_TABLE_RPT Table;
	T_TR_K TRK; T_TR_RPT TRData; 
	CArray<T_CHAR_K, T_CHAR_K> arCharK;


	CString strTitleX = _T("X");
	CString strTitleY = _T("Y");
	CArray<double, double> arX; arX.RemoveAll();
	CArray<double, double> arY; arY.RemoveAll();



	int nSize = 10;
	for(int i=0; i<nSize; i++)
	{
		arX.Add(3*i);
		arY.Add(1.5*i);
	}

	CString strPathName = m_pDoc->GetPathName();
	CFileCtrl FileCtrl(strPathName);
	CString csDir = FileCtrl.GetFileNameWithoutExtension()+_T("_image");

	strURL.Format(_T("%s\\%s_Graph.jpg"), csDir, FileCtrl.GetFileNameWithoutExtension());
	CString strGraphFileName = _T("");
	strGraphFileName.Format(_T("%s%s\\%s_Graph.jpg"),FileCtrl.GetFilePath(), csDir, FileCtrl.GetFileNameWithoutExtension());
	if(!MakeGraph(strGraphFileName, strTitleX, strTitleY, arX, arY)) return FALSE;

	ChapD.nLevel = 1;
	ChapD.SeqID = m_iChapNum;
	m_iChapNum++;
	strTitle = _T("Examples");
	ChapD.TitlK = AddTitle(strTitle);

		int iChap2Count = 1;
		/*
		//Write Graph
		Chap2D.Initialize();
		Chap2D.nLevel = 2;
		Chap2D.SeqID = iChap2Count;

			strTitle = _T("Example : Graph");
			Chap2D.TitlK = AddTitle(strTitle);

			ParaD.Initialize();
			ParaD.SeqID = 1;
			ParaD.strTitle = _T("Paragraph");

			// Picture test
			strTitle = _T("Graph");
			strText = _T("");
			PictK = AddPict(strTitle, strURL, strText);
			ParaD.arContentK.Add(PictK);

			m_pRptData->AddParaData(ParaK, ParaD);
			Chap2D.arParaK.Add(ParaK);

		m_pRptData->AddChapData(Chap2K, Chap2D);
		ChapD.arSubChapK.Add(Chap2K);
		iChap2Count++;
		*/

		// Text test
		Chap2D.Initialize();
		Chap2D.nLevel = 2;
		Chap2D.SeqID = iChap2Count;

			strTitle = _T("Example : Text");
			Chap2D.TitlK = AddTitle(strTitle);

			ParaD.Initialize();
			ParaD.SeqID = 1;
			ParaD.strTitle = _T("Paragraph");
			strTitle=_T("text");
			strText = _T("Graph Show Caption");
			TextK = AddText(strTitle, strText);
			ParaD.arContentK.Add(TextK);

			m_pRptData->AddParaData(ParaK, ParaD);
			Chap2D.arParaK.Add(ParaK);

		m_pRptData->AddChapData(Chap2K, Chap2D);
		ChapD.arSubChapK.Add(Chap2K);
		iChap2Count++;

		//Example SubScript
		Chap2D.Initialize();
		Chap2D.nLevel = 2;
		Chap2D.SeqID = iChap2Count;

			strTitle = _T("Example : SubScript");
			Chap2D.TitlK = AddTitle(strTitle);

			ParaD.Initialize();
			ParaD.SeqID = 1;
			ParaD.strTitle = _T("Paragraph");
			strTitle=_T("text");
			arCharK.RemoveAll();
			AddChar(arCharK, _T("cm2"), 2, 2, 1); 
			TextK = AddText(strTitle, arCharK, m_TextValueCssK);
			ParaD.arContentK.Add(TextK);

			m_pRptData->AddParaData(ParaK, ParaD);
			Chap2D.arParaK.Add(ParaK);

		m_pRptData->AddChapData(Chap2K, Chap2D);
		ChapD.arSubChapK.Add(Chap2K);
		iChap2Count++;

		//Example SuperScript
		Chap2D.Initialize();
		Chap2D.nLevel = 2;
		Chap2D.SeqID = iChap2Count;

			strTitle = _T("Example : SuperScript");
			Chap2D.TitlK = AddTitle(strTitle);

			ParaD.Initialize();
			ParaD.SeqID = 1;
			ParaD.strTitle = _T("Paragraph");
			strTitle=_T("text");
			arCharK.RemoveAll();
			AddChar(arCharK, _T("cm2"), 1, 2, 1); 
			TextK = AddText(strTitle, arCharK, m_TextValueCssK);
			ParaD.arContentK.Add(TextK);

			m_pRptData->AddParaData(ParaK, ParaD);
			Chap2D.arParaK.Add(ParaK);

		m_pRptData->AddChapData(Chap2K, Chap2D);
		ChapD.arSubChapK.Add(Chap2K);
		iChap2Count++;

		//Example Table including Picture & Text
		Chap2D.Initialize();
		Chap2D.nLevel = 2;
		Chap2D.SeqID = iChap2Count;

			strTitle = _T("Example : Table including Picture and Text");
			Chap2D.TitlK = AddTitle(strTitle);

			ParaD.Initialize();
			ParaD.SeqID = 1;
			ParaD.strTitle = _T("Paragraph");

			Table.Initialize();
			Table.nCheck = 1;
			Table.strTitle = _T("Table");

			TRData.Initialize();
			TRData.arTDK.Add(AddTDString(_T("Picture")));
			TRData.arTDK.Add(AddTDString(_T("Text")));
			m_pRptData->AddTRData(TRK, TRData);
			Table.arTRK.Add(TRK);

			TRData.Initialize();
			strTitle = _T("Graph");
			strText = _T("");
			TRData.arTDK.Add(AddTDPicture(strTitle, strURL, strText));
			TRData.arTDK.Add(AddTDString(_T("ABCDEFGHIJKLMNOPQRSTUVWXYZ")));
			m_pRptData->AddTRData(TRK, TRData);
			Table.arTRK.Add(TRK);

			m_pRptData->AddTableData(TableK, Table);
			ParaD.arContentK.Add(TableK);

			m_pRptData->AddParaData(ParaK, ParaD);
			Chap2D.arParaK.Add(ParaK);

		m_pRptData->AddChapData(Chap2K, Chap2D);
		ChapD.arSubChapK.Add(Chap2K);
		iChap2Count++;

	m_pRptData->AddChapData(ChapK, ChapD);

	T_REPORT_RPT Report; Report.Initialize();
	m_pRptData->GetReportData(Report);
	Report.arChapK.Add(ChapK);
	m_pRptData->ModifyReportData(Report);

	return TRUE;
}