// RptJTG_D62_04_Seismic.cpp: implementation of the CRptJTG_D62_04_Seismic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "RptJTG_D62_04_Seismic.h"
#include <winbase.h>


#include "..\wg_xl\XmlDocument.h"
#include "..\wg_xl\XslDocument.h"
#include "..\wg_xl\ReportData.h"

//#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_FileCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_StageInfo.h"
#include "..\wg_db\wg_db_AnalysisResult.h"
#include "..\wg_db\wg_db_LoadDB.h"
#include "..\ReleaseInfo.h"
#include "..\wg_db\mvload.h"


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
CRptJTG_D62_04_Seismic::CRptJTG_D62_04_Seismic()
{
    m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
    m_SeismicD.Initialize();
    
    T_CHRP_D rData; rData.Initialize();
    m_pDoc->m_pAttrCtrl->GetChrp(rData);  
    m_SeismicD.iLoadType  = rData.iEarthquakeType;
    m_SeismicD.iCheckType = rData.iBridgeType;
    for (int i=0; i<12; i++)
    {
        m_SeismicD.bDesignTpye[i] = rData.bDesignPrint[i];
    }
	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);

	// Set Code Unit.
	T_UNIT_INDEX CodeUnit = CurrUnit; 
	CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
    CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
	m_DataCtrl.Set_UnitIndex(CodeUnit);	
    m_strStresUnit  = _T("N/mm©÷");
    m_strForceUnit  = _T("kN");
    m_strMomentUnit = _T("kN¡¤m");
    
    Set_PrintFlag(TRUE);
    /////////////////////////////////////////////////
    CString strFilePathName = GetFilePathName();
    m_pXmlDoc->SetFilePathName(strFilePathName);
    m_pXslDoc->SetFilePathName(strFilePathName);

    m_TextPadding1CssK = 0;
    m_TextPadding2CssK = 0;
    m_TextValueCssK = 0;
    m_TextPicCaptionCssK = 0;
    m_TDCenterCssK = 0;

    m_strLength = _T("mm");
    m_strForce  = _T("kN");
    m_strMoment = _T("kN¡¤m");

    m_dForcekN   = 0.001;    
    m_dMomentkNM = 0.000001;   
    m_dLengthM   = 0.001;
    m_dDensitykNM = m_dForcekN / pow(m_dLengthM, 3.0);
    
}

CRptJTG_D62_04_Seismic::~CRptJTG_D62_04_Seismic()
{  
}

CString CRptJTG_D62_04_Seismic::GetFilePathName()
{
    CString strPathName = m_pDoc->GetPathName();
    CFileCtrl FileCtrl(strPathName);
    CString strFilePathName=FileCtrl.GetFilePathAndNameWithoutExtension() + _T("_rpt");
    return strFilePathName;
}

//!/
BOOL CRptJTG_D62_04_Seismic::MakeData()
{
    if(!CReportBase::MakeData()) return FALSE;
    
    T_UNIT_INDEX CodeIndex;
    CodeIndex = m_DataCtrl.Get_CodeUnitIndex();
    
    m_strForceUnit = CUnitCtrl::m_strForce;
    m_strMomentUnit = CUnitCtrl::m_strMoment;
    m_strLengthUnit = CUnitCtrl::m_strLength;

    if(CodeIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM && CodeIndex.nBase_Force == D_UNITSYS_FORCE_INDEX_N)			
    {
        m_strForce  = _T("kN");
        m_strForceN = _T("N");
        m_strMoment = _T("kN-m");
        m_strMomentN= _T("N-mm");
        m_strLength = _T("mm");
        m_strStress = _T("Mpa");

        m_strLengthUnit = _T("(mm)");
        m_strLengthUnitM = _T("(m)");
        m_strAngleUnitRad = _T("(rad)");
        m_strForceUnit = _T("(kN)");
        m_strMomentUnit = _T("(kN-m)");
        m_strStressUnit = _T("(MPa)");
        m_strForceUnitN = _T("(N)");
        m_strMomentUnitN = _T("(N-mm)");
        m_strDensityUnit = _T("(kN/m3)");

        m_arStressUnit.RemoveAll();
        AddChar(m_arStressUnit, _T("(N/mm2)"), 1, 5, 1);
        m_arstrAreaUnit.RemoveAll();
        AddChar(m_arstrAreaUnit, _T("(mm2)"), 1, 3, 1);
        m_arstrStifUnit.RemoveAll();
        AddChar(m_arstrStifUnit, _T("(mm4)"), 1, 3, 1);
        m_arstrSectUnit.RemoveAll();
        AddChar(m_arstrSectUnit, _T("(mm3)"), 1, 3, 1);  

    }
    else ASSERT(0);

    InitialarCharK();

    if(!MakeDataFromDB()) return FALSE;

    InitializeData();
    if(!MakeCoverData()) return FALSE;
    if(!MakeContents()) return FALSE;
    if(!MakeCodes()) return FALSE;
    if(!MakeDesignParameter()) return FALSE;
    MakeMaterialData();
    if(!MakeModelData()) return FALSE;
    if(!MakeLComData()) return FALSE;
    if(!MakeCheckResultTable()) return FALSE;
    if(!MakeCheckReportResultData()) return FALSE; //report data
    return TRUE;
}

BOOL CRptJTG_D62_04_Seismic::MakeDataFromDB()
{
    // Element.  
    m_iElemNum = 0;
	m_iGeneralLinkNum = 0;
	m_iElasticLinkNum = 0;
    m_arElemKeyList.RemoveAll();
	m_arGeneralLinkList.RemoveAll();
    m_arElasticLinkList.RemoveAll();
	m_DataCtrl.Get_ElemListForSeismicDgn(m_arElemKeyList);
	m_DataCtrl.Get_GeneralLinkListForSeismicDgn(m_arGeneralLinkList);
	m_DataCtrl.Get_ElasticLinkListForSeismicDgn(m_arElasticLinkList);
    m_iElemNum = m_arElemKeyList.GetSize();
    m_iGeneralLinkNum = m_arGeneralLinkList.GetSize();
    m_iElasticLinkNum = m_arElasticLinkList.GetSize();

	//
    GetMaterialProperty();
    // Node.
    CArray<T_NODE_K, T_NODE_K> NodeKList; NodeKList.RemoveAll();
    m_pDoc->m_pPostCtrl->GetNodeKeyListPost(NodeKList);
    m_iNodeNum = NodeKList.GetSize();
    // Boundary.  
    m_iBoundaryNum = m_pDoc->m_pAttrCtrl->GetCountCons();;
    // Stage.  
    m_arStagK.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetStagKeyList(m_arStagK);
    m_iStageNum = m_arStagK.GetSize();

    // Group.
    m_arGroupKeyList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetGrupKeyList(m_arGroupKeyList);

    return TRUE;
}

void CRptJTG_D62_04_Seismic::InitializeData()
{
    m_pRptData->m_TitleType[3] = 1; // Title4ÀÇ Çü½ÄÀ» (1)·Î º¯°æ
    m_iChapNum = 1;
}

BOOL CRptJTG_D62_04_Seismic::MakeDefaultStyle()
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
        CssD.strLineHeight   = _T("30pt");//_T("40px");
	  CssD.strBottomMargin = _T("50pt");//_T("300px");
        CssD.strTopMargin    = _T("3.75pt");//_T("150px");
        m_pRptData->AddCssData(CssK, CssD);
    }

    // Add by GAY.('06.05.30). Redefine Cover Sub Title. 
    CssK = 0;
    CssD.Initialize();
    strCssName = _T("SubTitle");
    m_pRptData->GetNameToCssK(strCssName, CssK);
    if(m_pRptData->GetCssData(CssK,CssD))
    {
        CssD.strFontSize    = _T("15pt");
        CssD.strFontFamily  = _T("arial");
        CssD.strFontWeight  = _T("bold");    
        CssD.strLineHeight   = _T("30pt");//_T("40px");
	  CssD.strBottomMargin = _T("0pt");//_T("300px");
        CssD.strTopMargin    = _T("0pt");//_T("150px");
        m_pRptData->AddCssData(CssK, CssD);
    }

    ////////////////////////////Only JTG_seismic Report/////////////////////////////////////////////
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

    // Add by GAY.('06.05.30). Set Background color as white.
    strCssName = _T("RowHeader");
    CssK = 0;
    m_pRptData->GetNameToCssK(strCssName, CssK);
    if(m_pRptData->GetCssData(CssK, CssD))
    {
        CssD.strBgColor = _LSX(#FFFFFF);
        m_pRptData->AddCssData(CssK, CssD);
    }

    // Add by GAY.('06.05.30). Set table cell text color as black.
    strCssName = _T("Cell_Center");
    CssK = 0;
    m_pRptData->GetNameToCssK(strCssName, CssK);
    if(m_pRptData->GetCssData(CssK, CssD))
    {
        CssD.strColor = _LSX(#000000);
        m_pRptData->AddCssData(CssK, CssD);
    }

    // Add by GAY.('06.05.30). Set Page-Break-Before Attribute : FALSE.
    strCssName = _T("Á¦¸ñ1");
    CssK = 0;
    CssD.Initialize();
    m_pRptData->GetNameToCssK(strCssName, CssK);
    if(m_pRptData->GetCssData(CssK, CssD))
    {
        CssD.bPageBreak = FALSE;
        m_pRptData->AddCssData(CssK, CssD);
    }

    // Á¦¸ñ1 + Set Page-Break-Before Attribute : TRUE.  
    CssK = 0;
    CssD.strStyleName = _T("Á¦¸ñ1_PB");
    CssD.bPageBreak = TRUE;  
    m_pRptData->AddCssData(CssK, CssD);
    
    return TRUE;
}

// 0-1. Ç¥Áö Data ±¸¼º.
BOOL CRptJTG_D62_04_Seismic::MakeCoverData()
{
    T_REPORT_RPT reportD; 
    reportD.Initialize();
    m_pRptData->GetReportData(reportD);

    T_PJCF_D pjcfD;   
    if(!m_pDoc->m_pAttrCtrl->GetPjcf(pjcfD)) 
    {
        pjcfD.Initialize();
    }

    reportD.strTitle.Format(_T("<br/>%s %s"), pjcfD.strProject, _LS(IDS_DGN_PSC_RPTJTG_Report));
    reportD.TitleCssK = m_pRptData->m_DefaultCss.CoverTitleCssK;
        
    // Change by GAY.('06.05.29). Project Information¿¡ µ¥ÀÌÅÍ°¡ ÀÔ·ÂµÇÁö ¾Ê¾ÒÀ» °æ¿ì¿¡´Â _T("XXX")·Î Ç¥±â ¿ä±¸. (From China)
    CString strName1 = _T(" : ") + ((pjcfD.strReviewName[0].GetLength() > 0)? pjcfD.strReviewName[0] : _T("XXX"));
    CString strName2 = _T(" : ") + ((pjcfD.strReviewName[1].GetLength() > 0)? pjcfD.strReviewName[1] : _T("XXX"));
    CString strName3 = _T(" : ") + ((pjcfD.strReviewName[4].GetLength() > 0)? pjcfD.strReviewName[4] : _T("XXX"));

    CString strText = _T("<br/><br/><br/>");
    strText += _LS(IDS_DGN_PSC_RPTJTG_Engineer) + strName1 + _T("<br/>");  
    strText += _LS(IDS_DGN_PSC_RPTJTG_Examination) + strName2 + _T("<br/>");
    strText += _LS(IDS_DGN_PSC_RPTJTG_Approved) + strName3 + _T("<br/>");
    strText += _T("<br/><br/><br/><br/><br/><br/>") + pjcfD.strUser + _T("<br/>");
    strText += GetFileDate() + _T("<br/>");
    
    reportD.strSubTitle = strText;
    reportD.SubTitleCssK = m_pRptData->m_DefaultCss.CoverSubTitleCssK;

    reportD.strDate = GetFileDate();   
    reportD.DateCssK = m_pRptData->m_DefaultCss.CoverDateCssK;

    m_pRptData->ModifyReportData(reportD);

    return TRUE;  
}

// 0-2. ¸ñÂ÷ Data¸¦ ±¸¼º.
BOOL CRptJTG_D62_04_Seismic::MakeContents()
{
    return TRUE;  
}

// 1. ¼³°è±âÁØ Data¸¦ ±¸¼ºÇÔ.
BOOL CRptJTG_D62_04_Seismic::MakeCodes()
{
    T_CHAP_K ChapK, Chap2K;
    T_CHAP_RPT ChapD; ChapD.Initialize();
    T_CHAP_RPT Chap2D; Chap2D.Initialize();

    T_CSS_K cssK;
    CString cssName = _T("Á¦¸ñ1_PB");
    m_pRptData->GetNameToCssK(cssName, cssK);

    ChapD = MakeChapter(1, m_iChapNum++, _LS(IDS_DGN_PSC_RPTJTG_Design_Standard), cssK);

    CStringArray arStringCode;
    arStringCode.SetSize(6);
    arStringCode[0] = _LS(IDS_DGN_PSC_RPTJTG_St_Name_B01_2004_Include_Bracket);
    arStringCode[1] = _LS(IDS_DGN_PSC_RPTJTG_St_Name_D60_2004_Info);
    arStringCode[2] = _LS(IDS_DGN_PSC_RPTJTG_St_Name_D62_2004_Info);
    arStringCode[3] = _LS(IDS_DGN_PSC_RPTCJJ_St_Name_JTG_D63_2007_Include_Bracket);
    arStringCode[4] = _LS(IDS_DGN_PSC_RPTCJJ_St_Name_JTGTB_02_01_2008_Include_Bracket);
    arStringCode[5] = _LS(IDS_DGN_PSC_RPTCJJ_St_Name_166_2011_Include_Bracket);

    int iChap2DCount = 1;    
    for(int i=0; i<6; i++)
    {
        Chap2D = MakeChapter(2, iChap2DCount, arStringCode[i]);

        m_pRptData->AddChapData(Chap2K, Chap2D);
        ChapD.arSubChapK.Add(Chap2K);
        iChap2DCount++;
    }

    m_pRptData->AddChapData(ChapK, ChapD);

    T_REPORT_RPT Report; Report.Initialize();
    m_pRptData->GetReportData(Report);
    Report.arChapK.Add(ChapK);
    m_pRptData->ModifyReportData(Report);

    return TRUE;
}

// 2. ¼³°èÀÚ·á Data¸¦ ±¸¼ºÇÔ.
BOOL CRptJTG_D62_04_Seismic::MakeDesignParameter()
{
    T_CHAP_K ChapK, Chap2K;  
    T_CHAP_RPT ChapD; ChapD.Initialize();
    T_CHAP_RPT Chap2D; Chap2D.Initialize();
            
    CStringArray arString; arString.RemoveAll();
    GetDesignStringPara(arString);
    if(arString.GetSize()==0) return FALSE;

    ChapD = MakeChapter(1, m_iChapNum++, _LS(IDS_DGN_PSC_RPTJTG_Design_Information));

    CStringArray arStrParam;
    arStrParam.SetSize(5);
    CString strProductNameVer;
    strProductNameVer.Format(CString(_LS(IDS_DGN_PSC_RPTJTG_MIDAS_Civil_Title))+_LS(IDS_DGN_PSC_RPTJTG_Release_Number), D_RELEASE_NO);
    arStrParam[0].Format(_T("%s : %s"), _LS(IDS_DGN_PSC_RPTJTG_Used_Program), strProductNameVer);
    arStrParam[1].Format(_T("%s : %s"), _LS(IDS_DGN_PSC_RPTJTG_Member_Force), arString.GetAt(0));
    arStrParam[2].Format(_T("%s : %s"), _LS(IDS_DGN_PSC_RPTJTG_Bridge_Type), arString.GetAt(1));
    arStrParam[3].Format(_T("%s : %s"), _T("µØÕð×÷ÓÃµÈ¼¶"), arString.GetAt(2));

    int iChap2DCount = 1;    
    for(int i=0; i<4; i++)
    {
        Chap2D = MakeChapter(2, iChap2DCount, arStrParam[i]);

        m_pRptData->AddChapData(Chap2K, Chap2D);
        ChapD.arSubChapK.Add(Chap2K);
        iChap2DCount++;
    }

    m_pRptData->AddChapData(ChapK, ChapD);

    T_REPORT_RPT Report; Report.Initialize();
    m_pRptData->GetReportData(Report);
    Report.arChapK.Add(ChapK);
    m_pRptData->ModifyReportData(Report);

    return TRUE;
}

// 3. ÀçÁú Data¸¦ ±¸¼ºÇÔ.
BOOL CRptJTG_D62_04_Seismic::MakeMaterialData()
{
    if(m_arMatElem.GetSize()==0) return FALSE;

    CString strTitle, strText;

    T_CHAP_K ChapK, Chap2K;
    T_PARA_K ParaK;
    T_CHAP_RPT ChapD; ChapD.Initialize();
    T_CHAP_RPT Chap2D; Chap2D.Initialize();
    T_PARA_RPT ParaD; ParaD.Initialize();  

    CStringArray arString; arString.RemoveAll();
    GetDesignStringPara(arString);
    if(arString.GetSize()==0) return FALSE;
    
    ChapD = MakeChapter(1, m_iChapNum++, _LS(IDS_DGN_PSC_RPTJTG_Material));

    int iChap2DCount = 1;

    Chap2D = MakeChapter(2, iChap2DCount, _LS(IDS_DGN_PSC_RPTJTG_Concrete));    
    if(MakeConcData(iChap2DCount, ParaD))
    {
        m_pRptData->AddParaData(ParaK, ParaD);
        Chap2D.arParaK.Add(ParaK);      
        m_pRptData->AddChapData(Chap2K, Chap2D);
        ChapD.arSubChapK.Add(Chap2K);
    }

    // Changed by GAY.('06.05.30). China requested merged rebar table. (MainRebar + SubRebar).
    Chap2D = MakeChapter(2, iChap2DCount, _LS(IDS_DGN_PSC_RPTJTG_Normal_Rebar));
    if(MakeRebar(iChap2DCount, ParaD))
    {
        m_pRptData->AddParaData(ParaK, ParaD);
        Chap2D.arParaK.Add(ParaK);
        m_pRptData->AddChapData(Chap2K, Chap2D);
        ChapD.arSubChapK.Add(Chap2K);
    }

    m_pRptData->AddChapData(ChapK, ChapD);

    T_REPORT_RPT Report; Report.Initialize();
    m_pRptData->GetReportData(Report);
    Report.arChapK.Add(ChapK);
    m_pRptData->ModifyReportData(Report);

    return TRUE;
}

// 4. ¸ðµ¨¿¡ ´ëÇÑ °£´ÜÇÑ ¼Ò°³ Data »ý¼º.
BOOL CRptJTG_D62_04_Seismic::MakeModelData()
{
    CString strTitle, strText;

    T_CHAP_K ChapK, Chap2K;
    T_PARA_K ParaK;
    T_CHAP_RPT ChapD; ChapD.Initialize();
    T_CHAP_RPT Chap2D; Chap2D.Initialize();
    T_PARA_RPT ParaD; ParaD.Initialize();

    CStringArray arString; arString.RemoveAll();
    GetDesignStringPara(arString);
    if(arString.GetSize()==0) return FALSE;

    ChapD = MakeChapter(1, m_iChapNum++, _LS(IDS_DGN_PSC_RPTJTG_Model_Simple_Intro));

    int iChap2DCount = 1;  
/*    
    // Beam Element Number.
    strText.Format(_LS(IDS_DGN_PSC_RPTJTG_Beam_Element_Num), m_iElemNum);
    Chap2D = MakeChapter(2, iChap2DCount++, strText);
    m_pRptData->AddChapData(Chap2K, Chap2D);
    ChapD.arSubChapK.Add(Chap2K);
    */
    //Beam and Column Element Number.
    strText.Format(_T("µ¥ÔªÊýÁ¿ : µ¥Ôª %d ¸ö"), m_iElemNum);
    Chap2D = MakeChapter(2, iChap2DCount++, strText);
    m_pRptData->AddChapData(Chap2K, Chap2D);
    ChapD.arSubChapK.Add(Chap2K);

    // Node Number.
    strText.Format(_LS(IDS_DGN_PSC_RPTJTG_Node_Num), m_iNodeNum);
    Chap2D = MakeChapter(2, iChap2DCount++, strText);
    m_pRptData->AddChapData(Chap2K, Chap2D);
    ChapD.arSubChapK.Add(Chap2K);
/*
    // Tendon Number.
    strText.Format(_LS(IDS_DGN_PSC_RPTJTG_Tendon_Num), m_iTendonNum);
    Chap2D = MakeChapter(2, iChap2DCount++, strText);
    m_pRptData->AddChapData(Chap2K, Chap2D);
    ChapD.arSubChapK.Add(Chap2K);
*/
    // Support Number.
    strText.Format(_LS(IDS_DGN_PSC_RPTJTG_Support_Num), m_iBoundaryNum);
    Chap2D = MakeChapter(2, iChap2DCount++, strText);
    m_pRptData->AddChapData(Chap2K, Chap2D);
    ChapD.arSubChapK.Add(Chap2K);
    
    // Stage Number.
    if(m_iStageNum > 0)
    {
        strText.Format(_LS(IDS_DGN_PSC_RPTJTG_Stage_Num), m_iStageNum);
        Chap2D = MakeChapter(2, iChap2DCount++, strText);

        if(MakeStageGroupSummaryData(iChap2DCount, ParaD))
        {
            m_pRptData->AddParaData(ParaK, ParaD);
            Chap2D.arParaK.Add(ParaK);      
        }

        m_pRptData->AddChapData(Chap2K, Chap2D);
        ChapD.arSubChapK.Add(Chap2K);
    }
 
// [15/12/2006 : maxiao] For CIVIL(CH) 2006
//  MakeServiceStageLoadData(iChap2DCount++, ChapD);
        
    m_pRptData->AddChapData(ChapK, ChapD);

    T_REPORT_RPT Report; Report.Initialize();
    m_pRptData->GetReportData(Report);
    Report.arChapK.Add(ChapK);
    m_pRptData->ModifyReportData(Report);

    return TRUE;
}

// 5. ÇÏÁßÁ¶ÇÕ ¼³¸í Data ±¸¼ºÇÔ.
BOOL CRptJTG_D62_04_Seismic::MakeLComData()
{
    CString strText;
    T_CHAP_K ChapK, Chap2K, Chap3K;
    T_PARA_K ParaK;
    T_CHAP_RPT ChapD; ChapD.Initialize();
    T_CHAP_RPT Chap2D; Chap2D.Initialize();
    T_CHAP_RPT Chap3D; Chap3D.Initialize();
    T_PARA_RPT ParaD; ParaD.Initialize();

    CStringArray arString; arString.RemoveAll();
    GetDesignStringPara(arString);
    if(arString.GetSize()==0) return FALSE;

    ChapD = MakeChapter(1, m_iChapNum++, _LS(IDS_DGN_PSC_RPTJTG_Load_Combination_Description));

    int iChap2DCount = 1;
    Chap2D = MakeChapter(2, iChap2DCount++, _LS(IDS_DGN_PSC_RPTJTG_Load_Case_Description));

    int iChap3DCount = 1;

    // Static Load Cases.
    Chap3D = MakeChapter(3, iChap3DCount, _LS(IDS_DGN_PSC_RPTJTG_Static_Load_Cases));
    if(MakeStaticLoadCaseData(iChap3DCount, ParaD))
    {
        m_pRptData->AddParaData(ParaK, ParaD);
        Chap3D.arParaK.Add(ParaK);      

        m_pRptData->AddChapData(Chap3K, Chap3D);
        Chap2D.arSubChapK.Add(Chap3K);
    }

    // Response Spectrum.
    Chap3D = MakeChapter(3, iChap3DCount, _LS(IDS_DGN_PSC_RPTJTG_Response_Spectrum_Load_Cases));
    if(MakeResponseSpectrumData(iChap3DCount, ParaD))
    {
        m_pRptData->AddParaData(ParaK, ParaD);
        Chap3D.arParaK.Add(ParaK);  
        
        m_pRptData->AddChapData(Chap3K, Chap3D);
        Chap2D.arSubChapK.Add(Chap3K);
    }

    // Time History Analysis.
    Chap3D = MakeChapter(3, iChap3DCount, _LS(IDS_DGN_PSC_RPTJTG_Time_History_Load_Cases));
    if(MakeTimeHistoryData(iChap3DCount, ParaD))
    {
        m_pRptData->AddParaData(ParaK, ParaD);
        Chap3D.arParaK.Add(ParaK);

        m_pRptData->AddChapData(Chap3K, Chap3D);
        Chap2D.arSubChapK.Add(Chap3K);
    }

    // Moving Load Cases.
    Chap3D = MakeChapter(3, iChap3DCount, _LS(IDS_DGN_PSC_RPTJTG_Moving_Load_Cases));
    if(MakeMovingLoadData(iChap3DCount, ParaD))
    {
        m_pRptData->AddParaData(ParaK, ParaD);
        Chap3D.arParaK.Add(ParaK);

        m_pRptData->AddChapData(Chap3K, Chap3D);
        Chap2D.arSubChapK.Add(Chap3K);
    }

    // Settlement Load Cases.
    Chap3D = MakeChapter(3, iChap3DCount, _LS(IDS_DGN_PSC_RPTJTG_Settlement_Load_Cases));
    if(MakeSettlementData(iChap3DCount, ParaD))
    {
        m_pRptData->AddParaData(ParaK, ParaD);
        Chap3D.arParaK.Add(ParaK);

        m_pRptData->AddChapData(Chap3K, Chap3D);
        Chap2D.arSubChapK.Add(Chap3K);
    }

    // Grid Load Cases.
    Chap3D = MakeChapter(3, iChap3DCount, _LS(IDS_DGN_PSC_RPTJTG_Grid_Load_Cases));
    if(MakeGridLoadData(iChap3DCount, ParaD))
    {
        m_pRptData->AddParaData(ParaK, ParaD);
        Chap3D.arParaK.Add(ParaK);

        m_pRptData->AddChapData(Chap3K, Chap3D);
        Chap2D.arSubChapK.Add(Chap3K);
    }

    m_pRptData->AddChapData(Chap2K, Chap2D);
    ChapD.arSubChapK.Add(Chap2K);  

    /////////////////////////////////////////////
    Chap2D = MakeChapter(2, iChap2DCount++, _LS(IDS_DGN_PSC_RPTJTG_Load_Combination_Description));
    iChap3DCount = 1;
        
    Chap3D = MakeChapter(3, iChap3DCount, _LS(IDS_DGN_PSC_RPTJTG_LoadComb_CaseNames));
    if(MakeLoadCombCaseData(iChap3DCount, ParaD))
    {
        m_pRptData->AddParaData(ParaK, ParaD);
        Chap3D.arParaK.Add(ParaK);

        m_pRptData->AddChapData(Chap3K, Chap3D);
        Chap2D.arSubChapK.Add(Chap3K);
    }

    Chap3D = MakeChapter(3, iChap3DCount, _LS(IDS_DGN_PSC_RPTJTG_LoadComb));
    if(MakeLoadCombData(iChap3DCount, ParaD))
    {
        m_pRptData->AddParaData(ParaK, ParaD);
        Chap3D.arParaK.Add(ParaK);

        m_pRptData->AddChapData(Chap3K, Chap3D);
        Chap2D.arSubChapK.Add(Chap3K);
    }

    m_pRptData->AddChapData(Chap2K, Chap2D);
    ChapD.arSubChapK.Add(Chap2K);

    m_pRptData->AddChapData(ChapK, ChapD);

    T_REPORT_RPT Report; Report.Initialize();
    m_pRptData->GetReportData(Report);
    Report.arChapK.Add(ChapK);
    m_pRptData->ModifyReportData(Report);

    return TRUE;
}

// 6. °ËÅä°á°ú Table Data ±¸¼º.
BOOL CRptJTG_D62_04_Seismic::MakeCheckResultTable()
{
        if(!CheckExistResultTableItem()) return TRUE;
	//++++++++++++++++++++++++++++++++++++++++++++
	// <Remember> Get Data by Code Units.
	T_UNIT_INDEX CurIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
    T_UNIT_INDEX CodeIndex = m_DataCtrl.Get_CodeUnitIndex();
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeIndex);
	//++++++++++++++++++++++++++++++++++++++++++++

    CString strTitle, strText;
    T_CHAP_K ChapK, Chap2K;
    T_PARA_K ParaK;
    T_CHAP_RPT ChapD; ChapD.Initialize();
    T_CHAP_RPT Chap2D; Chap2D.Initialize();  
    T_PARA_RPT ParaD; ParaD.Initialize();

    ChapD = MakeChapter(1, m_iChapNum++, _LS(IDS_DGN_PSC_RPTJTG_CheckResult_Table));
    int iChap2DCount = 1;
	//////////////////////////////////////////////////////////////////////////
	BOOL bE1 = (m_SeismicD.iLoadType == 0)? TRUE : FALSE;
    // 0-ÇÅ¶ÕÇ¿¶È; 1-ÇÅÌ¨Ç¿¶È; 2-»ù´¡Ç¿¶È; 3-¸ÇÁºÇ¿¶È; 4-Ö÷¹°È¦Ç¿¶È; 5-ÇÅ¶ÕËÜÐÔ½ÂÇø¿¹¼ôÇ¿¶È;
    // 6-Ö§×ùºñ¶È£¨°åÊ½Ïð½ºÖ§×ù£©; 7-Ö§×ù¿¹»¬ÎÈ¶¨ÐÔ£¨°åÊ½Ïð½ºÖ§×ù£©; 8-Ö§×ùË®Æ½Î»ÒÆ£¨»î¶¯ÅèÊ½Ö§×ù£©; 9-Ö§×ùË®Æ½³ÐÔØÁ¦£¨¹Ì¶¨ÅèÊ½Ö§×ù£©; 10-¶Õ¶¥Î»ÒÆ(¹æÔòÇÅÁº); 11-ËÜÐÔ×ª½ÇÎ»ÒÆ(·Ç¹æÔòÇÅÁº); 
	//6.1.1ÇÅ¶Õµ¥ÔªÇ¿¶È
    if(m_SeismicD.bDesignTpye[0])//Need todo
    {
        Chap2D = MakeChapter(2, iChap2DCount, _T("ÇÅ¶Õµ¥ÔªÇ¿¶ÈÑéËã"));
        if(MakeSeiemicPierCompCheckData(iChap2DCount, ParaD, bE1, TRUE))
        {
            m_pRptData->AddParaData(ParaK, ParaD);
            Chap2D.arParaK.Add(ParaK);
            m_pRptData->AddChapData(Chap2K, Chap2D);
            ChapD.arSubChapK.Add(Chap2K);
        }
    }
    //6.1.2ÇÅÌ¨µ¥ÔªÇ¿¶È
    if(m_SeismicD.bDesignTpye[1])//Need todo
    {
        Chap2D = MakeChapter(2, iChap2DCount, _T("ÇÅÌ¨µ¥ÔªÇ¿¶ÈÑéËã"));
        if(MakeSeiemicPierCompCheckData(iChap2DCount, ParaD, bE1, FALSE))
        {
            m_pRptData->AddParaData(ParaK, ParaD);
            Chap2D.arParaK.Add(ParaK);
            m_pRptData->AddChapData(Chap2K, Chap2D);
            ChapD.arSubChapK.Add(Chap2K);
        }
    }
    //6.2Ö÷¹°È¦Ç¿¶È-¿¹Íä
    if(m_SeismicD.bDesignTpye[4])//Need todo
    {
        Chap2D = MakeChapter(2, iChap2DCount, _T("Ö÷¹°È¦Ç¿¶È-¿¹ÍäÑéËã"));
        if(MakeSeiemicArchCrossCheckData(iChap2DCount, ParaD, bE1))
        {
            m_pRptData->AddParaData(ParaK, ParaD);
            Chap2D.arParaK.Add(ParaK);
            m_pRptData->AddChapData(Chap2K, Chap2D);
            ChapD.arSubChapK.Add(Chap2K);
        }
    }

	//6.3Ö÷¹°È¦Ç¿¶È-Æ«Ñ¹
    if(m_SeismicD.bDesignTpye[4])//Need todo
    {
        Chap2D = MakeChapter(2, iChap2DCount, _T("Ö÷¹°È¦Ç¿¶È-Æ«Ñ¹ÑéËã"));
        if(MakeSeiemicArchCompCheckData(iChap2DCount, ParaD, bE1))
        {
            m_pRptData->AddParaData(ParaK, ParaD);
            Chap2D.arParaK.Add(ParaK);
            m_pRptData->AddChapData(Chap2K, Chap2D);
            ChapD.arSubChapK.Add(Chap2K);
        }
    }

	//6.4¸ÇÁºÇ¿¶È-¿¹Íä
    if(m_SeismicD.bDesignTpye[3])//Need todo
    {
        Chap2D = MakeChapter(2, iChap2DCount, _T("¸ÇÁºÇ¿¶È-¿¹ÍäÑéËã"));
        if(MakeSeiemicBentCapBendCheckData(iChap2DCount, ParaD, bE1))
        {
            m_pRptData->AddParaData(ParaK, ParaD);
            Chap2D.arParaK.Add(ParaK);
            m_pRptData->AddChapData(Chap2K, Chap2D);
            ChapD.arSubChapK.Add(Chap2K);
        }
    }

	//6.5¸ÇÁºÇ¿¶È-¿¹¼ô
    if(m_SeismicD.bDesignTpye[3])//Need todo
    {
        Chap2D = MakeChapter(2, iChap2DCount, _T("¸ÇÁºÇ¿¶È-¿¹¼ôÑéËã"));
        if(MakeSeiemicBentCapShearCheckData(iChap2DCount, ParaD, bE1))
        {
            m_pRptData->AddParaData(ParaK, ParaD);
            Chap2D.arParaK.Add(ParaK);
            m_pRptData->AddChapData(Chap2K, Chap2D);
            ChapD.arSubChapK.Add(Chap2K);
        }
    }

    //6.6»ù´¡Ç¿¶ÈÑéËã
    if(m_SeismicD.bDesignTpye[2])//Need todo
    {
        Chap2D = MakeChapter(2, iChap2DCount, _T("»ù´¡Ç¿¶ÈÑéËãÑéËã"));
        if(MakeSeiemicFoundationCheckData(iChap2DCount, ParaD, bE1))
        {
            m_pRptData->AddParaData(ParaK, ParaD);
            Chap2D.arParaK.Add(ParaK);
            m_pRptData->AddChapData(Chap2K, Chap2D);
            ChapD.arSubChapK.Add(Chap2K);
        }
    }

	//6.7ÇÅ¶ÕËÜÐÔ½ÂÇø¿¹¼ôÇ¿¶È
    if(m_SeismicD.bDesignTpye[5])//Need todo
    {
        Chap2D = MakeChapter(2, iChap2DCount, _T("ÇÅ¶ÕËÜÐÔ½ÂÇø¿¹¼ôÇ¿¶ÈÑéËã"));
        if(MakeSeiemicPierShearCheckData(iChap2DCount, ParaD))
        {
            m_pRptData->AddParaData(ParaK, ParaD);
            Chap2D.arParaK.Add(ParaK);
            m_pRptData->AddChapData(Chap2K, Chap2D);
            ChapD.arSubChapK.Add(Chap2K);
        }
    }

	//6.8Ö§×ùºñ¶È£¨°åÊ½Ïð½º£©
    if(m_SeismicD.bDesignTpye[6])//Need todo
    {
        Chap2D = MakeChapter(2, iChap2DCount, _T("Ö§×ùºñ¶È£¨°åÊ½Ïð½º£©ÑéËã"));
        if(MakeSeismicRubberThickCheckData(iChap2DCount, ParaD, bE1))
        {
            m_pRptData->AddParaData(ParaK, ParaD);
            Chap2D.arParaK.Add(ParaK);
            m_pRptData->AddChapData(Chap2K, Chap2D);
            ChapD.arSubChapK.Add(Chap2K);
        }
    }

	//6.9Ö§×ù¿¹»¬ÎÈ¶¨ÐÔ£¨°åÊ½Ïð½º£©
    if(m_SeismicD.bDesignTpye[7])//Need todo
    {
        Chap2D = MakeChapter(2, iChap2DCount, _T("Ö§×ù¿¹»¬ÎÈ¶¨ÐÔ£¨°åÊ½Ïð½ºÖ§×ù£©ÑéËã"));
        if(MakeSeismicRubberSlipCheckData(iChap2DCount, ParaD, bE1))
        {
            m_pRptData->AddParaData(ParaK, ParaD);
            Chap2D.arParaK.Add(ParaK);
            m_pRptData->AddChapData(Chap2K, Chap2D);
            ChapD.arSubChapK.Add(Chap2K);
        }
    }

	//6.10Ö§×ùË®Æ½Î»ÒÆ£¨ÅèÊ½»î¶¯Ö§×ù£©
    if(m_SeismicD.bDesignTpye[8])//Need todo
    {
        Chap2D = MakeChapter(2, iChap2DCount, _T("Ö§×ùË®Æ½Î»ÒÆ£¨»î¶¯ÅèÊ½Ö§×ù£©ÑéËã"));
        if(MakeSeismicActiveHoriDispCheckData(iChap2DCount, ParaD, bE1))
        {
            m_pRptData->AddParaData(ParaK, ParaD);
            Chap2D.arParaK.Add(ParaK);
            m_pRptData->AddChapData(Chap2K, Chap2D);
            ChapD.arSubChapK.Add(Chap2K);
        }
    }
	
	//6.11Ö§×ùË®Æ½³ÐÔØÁ¦£¨ÅèÊ½¹Ì¶¨Ö§×ù£©
    if(m_SeismicD.bDesignTpye[9])//Need todo
    {
        Chap2D = MakeChapter(2, iChap2DCount, _T("Ö§×ùË®Æ½³ÐÔØÁ¦£¨¹Ì¶¨ÅèÊ½Ö§×ù£©ÑéËã"));
        if(MakeSeismicStatHoriForceCheckData(iChap2DCount, ParaD, bE1))
        {
            m_pRptData->AddParaData(ParaK, ParaD);
            Chap2D.arParaK.Add(ParaK);
            m_pRptData->AddChapData(Chap2K, Chap2D);
            ChapD.arSubChapK.Add(Chap2K);
        }
    }

	//6.12¶Õ¶¥Î»ÒÆ£¨¹æÔòÇÅÁº£©SFTD_JT
    if(m_SeismicD.bDesignTpye[10])//Need todo
    {
        Chap2D = MakeChapter(2, iChap2DCount, _T("¶Õ¶¥Î»ÒÆ(¹æÔòÇÅÁº)ÑéËã"));
        if(MakeSeismicFrustaTopDispCheckData(iChap2DCount, ParaD))
        {
            m_pRptData->AddParaData(ParaK, ParaD);
            Chap2D.arParaK.Add(ParaK);
            m_pRptData->AddChapData(Chap2K, Chap2D);
            ChapD.arSubChapK.Add(Chap2K);
        }
    }

	//6.13ËÜÐÎ×ª½ÇÎ»ÒÆ£¨·Ç¹æÔòÇÅÁº£©SCND_JT
    if(m_SeismicD.bDesignTpye[11])//Need todo
    {
        Chap2D = MakeChapter(2, iChap2DCount, _T("ËÜÐÔ×ª½ÇÎ»ÒÆ(·Ç¹æÔòÇÅÁº)ÑéËã"));
        if(MakeSeismicCornerDispCheckData(iChap2DCount, ParaD))
        {
            m_pRptData->AddParaData(ParaK, ParaD);
            Chap2D.arParaK.Add(ParaK);
            m_pRptData->AddChapData(Chap2K, Chap2D);
            ChapD.arSubChapK.Add(Chap2K);
        }
    }
    
	m_pRptData->AddChapData(ChapK, ChapD);

    T_REPORT_RPT Report; Report.Initialize();
    m_pRptData->GetReportData(Report);
    Report.arChapK.Add(ChapK);
    m_pRptData->ModifyReportData(Report);
	//++++++++++++++++++++++++++++++++++++++++++++
	// <Remember> Set Data by Current Units.
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
	//++++++++++++++++++++++++++++++++++++++++++++
    return TRUE;
}

// 3.1. »ìÄýÍÁÉè¼Æ²ÄÁÏ.
// China request Elastic Modulus and Density Output.
BOOL CRptJTG_D62_04_Seismic::MakeConcData(int& iChapID, T_PARA_RPT& paraD)
{
    int iExistCount = 0;
    paraD.Initialize();

    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");

    CString strText;
    CArray<T_CHAR_K, T_CHAR_K> arCharK;  
    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;

    // Table Header.
    thData.CssK = m_pRptData->m_DefaultCss.THCssK;

    thData.arTDK.RemoveAll();
    strText = _LS(IDS_DGN_PSC_RPTJTG_Strength_Grade);   //_T("°­µµµî±Þ")
    thData.arTDK.Add(AddTDString(strText, 1, 1, 2));   
    strText = _LS(IDS_DGN_PSC_RPTJTG_Elastic_Modulus);
    thData.arTDK.Add(AddTDwithUnit(strText, m_strStressUnit, 1, 1, 2));  // _T("Åº¼º°è¼ö")  
    arCharK.RemoveAll();
    AddChar(arCharK, m_strDensityUnit, 1, 5, 1); 
    strText = _LS(IDS_DGN_PSC_RPTJTG_Density);
    thData.arTDK.Add(AddTDwithUnit(strText, arCharK, 1, 1, 2));  // _T("´ÜÀ§Áß·®")    
    strText = _LS(IDS_DGN_PSC_RPTJTG_Thermal_Coeff);    // _T("¼±ÆØÃ¢°è¼ö")
    thData.arTDK.Add(AddTDString(strText, 1, 1, 2));  
    strText = _LS(IDS_DGN_PSC_RPTJTG_Standard_Value);   //_T("Ç¥ÁØÄ¡")
    thData.arTDK.Add(AddTDString(strText, 1, 2, 1));
    strText = _LS(IDS_DGN_PSC_RPTJTG_Design_Value);     //_T("¼³°èÄ¡")
    thData.arTDK.Add(AddTDString(strText, 1, 2, 1));
    
    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);
    
    thData.arTDK.RemoveAll();
    arCharK.RemoveAll();
    //strText.Format(,m_strStressUnit);
    AddChar(arCharK, _T("fck"), 2, 1, 2);
    thData.arTDK.Add(AddTDwithUnit(arCharK, m_strStressUnit, 1));
    arCharK.RemoveAll();  
    AddChar(arCharK, _T("ftk"), 2, 1, 2);
    thData.arTDK.Add(AddTDwithUnit(arCharK, m_strStressUnit, 1));
    arCharK.RemoveAll();  
    AddChar(arCharK, _T("fcd"), 2, 1, 2);
    thData.arTDK.Add(AddTDwithUnit(arCharK, m_strStressUnit, 1));
    arCharK.RemoveAll();  
    AddChar(arCharK, _T("ftd"), 2, 1, 2);
    thData.arTDK.Add(AddTDwithUnit(arCharK, m_strStressUnit, 1));  

    m_pRptData->AddTRData(thK, thData);  
    tableD.arTHK.Add(thK);
    CArray<CString,CString> myArray;  //-----2007.01.06
    for(int i=0; i<m_arMatElem.GetSize(); i++)
    {
        
        _DGN_RC_MATL_CH MatlElem; MatlElem.Initialize();
        m_DataCtrl.Get_Matl(m_arMatElem.GetAt(i), MatlElem);
        CString strConc, strfck, strftk, strfcd, strftd, strThermal, strEc, strDensity;
        //+++++++++++
//    T_ELEM_D ElemD;  ElemD.Initialize();
//    m_pDoc->m_pAttrCtrl->GetElem(m_arMatElem.GetAt(i),ElemD);
        T_MATD_D MatdD;	MatdD.Initialize();
//    m_DataCtrl.Get_ConMatd(ElemD.elmat,MatdD);
        m_DataCtrl.Get_ConMatd(m_arMatElem.GetAt(i),MatdD);
        //+++++++++++
        strConc = MatdD.Data1.CodeMatlName;
        BOOL bsame = FALSE;
        for (int j=0;j<myArray.GetSize();j++) {//-------2007.01.06
            if (strConc == myArray.GetAt(j)) bsame = TRUE;
        }
        if (bsame) continue;
        myArray.Add(strConc);    //--------2007.01.06
        strfck.Format(_T("%.2f"), MatlElem.dfck1);
        strftk.Format(_T("%.2f"), MatlElem.dftk1);
        strfcd.Format(_T("%.2f"), MatlElem.dfcd);
        strftd.Format(_T("%.2f"), MatlElem.dftd);
        strThermal.Format(_T("%.3e"), MatdD.Data1.Analysis.Thermal);
        strEc.Format(_T("%.2f"), MatlElem.dEc);//MatdD.Data1.Analysis.Elast*m_dForcekN);
        strDensity.Format(_T("%.2f"), MatdD.Data1.Analysis.Density * m_dDensitykNM );//*m_dMomentkNM);
        
        tdData.Initialize();
        tdData.arTDK.Add(AddTDString(strConc, 0, 1, 1, m_TDCenterCssK));
        tdData.arTDK.Add(AddTDString(strEc, 0, 1, 1, m_TDCenterCssK));
        tdData.arTDK.Add(AddTDString(strDensity, 0, 1, 1, m_TDCenterCssK));
        tdData.arTDK.Add(AddTDString(strThermal, 0, 1, 1, m_TDCenterCssK));
        tdData.arTDK.Add(AddTDString(strfck, 0, 1, 1, m_TDCenterCssK));
        tdData.arTDK.Add(AddTDString(strftk, 0, 1, 1, m_TDCenterCssK));
        tdData.arTDK.Add(AddTDString(strfcd, 0, 1, 1, m_TDCenterCssK));
        tdData.arTDK.Add(AddTDString(strftd, 0, 1, 1, m_TDCenterCssK));        
        m_pRptData->AddTRData(tdK, tdData);
        tableD.arTRK.Add(tdK);

        iExistCount += 1;
    }

    if(iExistCount > 0)
    {
        iChapID++;
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// 3.3. º¸ÅëÃ¶±Ù DataÀÇ ±¸¼º.
BOOL CRptJTG_D62_04_Seismic::MakeRebar(int& iChapID, T_PARA_RPT& paraD)
{
    int iExistCount = 0;
    paraD.Initialize();

    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");

    CString strText;
    CArray<T_CHAR_K, T_CHAR_K> arCharK;
    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;

    // Table Header.
    thData.Initialize();
    thData.arTDK.Add(AddTDString(_LS(IDS_DGN_PSC_RPTJTG_Normal_Rebar), 1));

    strText = _LS(IDS_DGN_PSC_RPTJTG_Elastic_Modulus);  
    thData.arTDK.Add(AddTDwithUnit(strText, m_strStressUnit, 1));
    strText = _LS(IDS_DGN_PSC_RPTJTG_Density);
    arCharK.RemoveAll();
    AddChar(arCharK, m_strDensityUnit, 1, 5, 1); 
    thData.arTDK.Add(AddTDwithUnit(strText, arCharK, 1));
    
    arCharK.RemoveAll();  
    AddChar(arCharK, _T("fsk"), 2, 1, 2);
    thData.arTDK.Add(AddTDwithUnit(arCharK, m_strStressUnit, 1));
    arCharK.RemoveAll();  
    AddChar(arCharK, _T("fsd"), 2, 1, 2);
    thData.arTDK.Add(AddTDwithUnit(arCharK, m_strStressUnit, 1));
    arCharK.RemoveAll();  
    AddChar(arCharK, _T("f'sd"), 2, 2, 2);
    thData.arTDK.Add(AddTDwithUnit(arCharK, m_strStressUnit, 1));
    thData.arTDK.Add(AddTDString(_T("¦Åy"), 1));

    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);

    _DGN_RC_MATL_CH MatlElem; MatlElem.Initialize();  
    CString strfsk, strfsdt, strfsdc, strEs, strDensity, strThermal, strEpsilony;      
    CArray<CString,CString> myArray;  //-----2007.01.06
    int iCount = m_arMatElem.GetSize();
    for(int i=0; i<iCount; i++)
    {
        if(m_DataCtrl.Get_Matl(m_arMatElem[i], MatlElem))
        {
            //+++++++++++
            T_MATD_D MatdD;	MatdD.Initialize();
            m_DataCtrl.Get_ConMatd(m_arMatElem.GetAt(i),MatdD);
            //+++++++++++
            strEs.Format(_T("%.2lf"), MatlElem.dEs);
            strDensity.Format(_T("%.2lf"), MatdD.MainRebarData.B_Density * m_dDensitykNM );//*m_dMomentkNM);
            // Main Rebar
            CString strName = MatdD.MainRebar_RebarName;

            BOOL bsame = FALSE;//-------2007.01.06
            for (int j=0;j<myArray.GetSize();j++) {
                if (strName == myArray.GetAt(j)) bsame = TRUE;
            }
            if (bsame) continue;
            myArray.Add(strName);    //--------2007.01.06

            strfsk.Format(_T("%.2lf"), MatlElem.dfsk);
            strfsdt.Format(_T("%.2lf"), MatlElem.dfsd);
            strfsdc.Format(_T("%.2lf"), MatlElem.dfsd1);
            strEpsilony.Format(_T("%.6lf"), MatlElem.dfsk/MatlElem.dEs);

            tdData.Initialize();
            tdData.arTDK.Add(AddTDString(strName, 0, 1, 1, m_TDCenterCssK));
            tdData.arTDK.Add(AddTDString(strEs,  0, 1, 1, m_TDCenterCssK));
            tdData.arTDK.Add(AddTDString(strDensity, 0, 1, 1, m_TDCenterCssK));
            tdData.arTDK.Add(AddTDString(strfsk,  0, 1, 1, m_TDCenterCssK));
            tdData.arTDK.Add(AddTDString(strfsdt, 0, 1, 1, m_TDCenterCssK));
            tdData.arTDK.Add(AddTDString(strfsdc, 0, 1, 1, m_TDCenterCssK));        
            tdData.arTDK.Add(AddTDString(strEpsilony, 0, 1, 1, m_TDCenterCssK));         
            m_pRptData->AddTRData(tdK, tdData);
            tableD.arTRK.Add(tdK);

            // Sub Rebar
            strName = MatdD.SubRebar_RebarName;

            for (int j=0;j<myArray.GetSize();j++) {
                if (strName == myArray.GetAt(j)) bsame = TRUE;
            }
            if (bsame) continue;
            myArray.Add(strName);    //--------2007.01.06

            strfsk.Format(_T("%.2lf"), MatlElem.dfsky);
            strfsdt.Format(_T("%.2lf"), MatlElem.dfsyd);
            strfsdc.Format(_T("%.2lf"), MatlElem.dfsyd);
            strEpsilony.Format(_T("%.6lf"), MatlElem.dfsky/MatlElem.dEsy);
            strDensity.Format(_T("%.2lf"), MatdD.SubRebarData.B_Density* m_dDensitykNM);
            strEs.Format(_T("%.2lf"), MatlElem.dEsy);

            tdData.Initialize();
            tdData.arTDK.Add(AddTDString(strName, 0, 1, 1, m_TDCenterCssK));
            tdData.arTDK.Add(AddTDString(strEs,  0, 1, 1, m_TDCenterCssK));
            tdData.arTDK.Add(AddTDString(strDensity, 0, 1, 1, m_TDCenterCssK));
            tdData.arTDK.Add(AddTDString(strfsk,  0, 1, 1, m_TDCenterCssK));
            tdData.arTDK.Add(AddTDString(strfsdt, 0, 1, 1, m_TDCenterCssK));
            tdData.arTDK.Add(AddTDString(strfsdc, 0, 1, 1, m_TDCenterCssK));   
            tdData.arTDK.Add(AddTDString(strEpsilony, 0, 1, 1, m_TDCenterCssK));         
            m_pRptData->AddTRData(tdK, tdData);
            tableD.arTRK.Add(tdK);

            iExistCount += 1;
        }
    }

    if(iExistCount > 0)
    {
        iChapID++;
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


// 4.5. Stage Data ±¸¼º.
BOOL CRptJTG_D62_04_Seismic::MakeStageGroupSummaryData(int& iChapID, T_PARA_RPT& paraD)
{
    int iExistCount = 0;
    paraD.Initialize();

    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");

    CString strText;  
    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;

    // Table Header.
    thData.Initialize();
    thData.arTDK.Add(AddTDString(_LS(IDS_DGN_PSC_RPTJTG_Stage_Name), 1, 1, 2));
    CString aHeadNames1[] = { _LS(IDS_DGN_PSC_RPTJTG_Element_Group), 
        _LS(IDS_DGN_PSC_RPTJTG_Boundary_Group), _LS(IDS_DGN_PSC_RPTJTG_Load_Group) };
    for(int i=0; i<3; i++)
    {
        thData.arTDK.Add(AddTDString(aHeadNames1[i], 1, 2, 1));
    }
    
    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);
    

    thData.Initialize();  
    for(int i=0; i<3; i++)
    {
        thData.arTDK.Add(AddTDString(_LS(IDS_DGN_PSC_RPTJTG_Active), 1, 1, 1));
        thData.arTDK.Add(AddTDString(_LS(IDS_DGN_PSC_RPTJTG_Deactive), 1, 1, 1));
    }
    
    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);

    for(int i=0; i<m_iStageNum; i++)
    {
        tdData.Initialize();

        CString strActive;
        CString strDeactive;
        
        T_STAG_D stagD;
        if(!m_pDoc->m_pAttrCtrl->GetStag(m_arStagK[i], stagD)) stagD.Initialize();

        tdData.arTDK.Add(AddTDString(stagD.StageName, 0, 1, 1, m_TDCenterCssK));

        CArray<T_TEXT_K, T_TEXT_K> arActiveK;
        CArray<T_TEXT_K, T_TEXT_K> arDeactiveK;
        GetElementGroupList(m_arStagK[i], arActiveK, arDeactiveK);           
        tdData.arTDK.Add(AddTD(arActiveK, 0, 1, 1, m_TDCenterCssK));    
        tdData.arTDK.Add(AddTD(arDeactiveK, 0, 1, 1, m_TDCenterCssK));

        GetBoundaryGroupList(m_arStagK[i], arActiveK, arDeactiveK);         
        tdData.arTDK.Add(AddTD(arActiveK, 0, 1, 1, m_TDCenterCssK));
        tdData.arTDK.Add(AddTD(arDeactiveK, 0, 1, 1, m_TDCenterCssK));

        GetLoadGroupList(m_arStagK[i], arActiveK, arDeactiveK);          
        tdData.arTDK.Add(AddTD(arActiveK, 0, 1, 1, m_TDCenterCssK));
        tdData.arTDK.Add(AddTD(arDeactiveK, 0, 1, 1, m_TDCenterCssK));

        tdData.CssK = m_TDCenterCssK;
        m_pRptData->AddTRData(tdK, tdData);
        tableD.arTRK.Add(tdK);   

        iExistCount += 1;
    }
    
    if(iExistCount > 0)
    {
        iChapID++;
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// 4.6. »ç¿ë´Ü°è¿¡ ´ëÇÑ Data ±¸¼º.
BOOL CRptJTG_D62_04_Seismic::MakeServiceStageLoadData(int iChapID, T_CHAP_RPT& chapD)
{  
    T_CHAP_K chap2K, chap3K;
    T_CHAP_RPT chap2D = MakeChapter(2, iChapID, _LS(IDS_DGN_PSC_RPTJTG_Service_Stage));
    T_CHAP_RPT chap3D;
        
    // È°ÇÏÁß Type.
    int iChapCount = 01;
    CString strText = _T(""); 

    T_PARA_K paraK;
    T_PARA_RPT paraD;
    paraD.Initialize();
    paraD.SeqID = 1;
    paraD.strTitle = _T("Paragraph");  

    if(MakeLiveLoadData(paraD))
    {
        chap3D = MakeChapter(3, iChapCount++, _LS(IDS_DGN_PSC_RPTJTG_LiveLoad_Type));

        m_pRptData->AddParaData(paraK, paraD);
        chap3D.arParaK.Add(paraK);
        m_pRptData->AddChapData(chap3K, chap3D);
        chap2D.arSubChapK.Add(chap3K);

        // Ãæ°Ý°è¼ö.    
        strText.Format(_T("%s : %s"), _LS(IDS_DGN_PSC_RPTJTG_Impact_Coeff), GetImpactCoefficient());
        chap3D = MakeChapter(3, iChapCount++, strText);
        m_pRptData->AddChapData(chap3K, chap3D);
        chap2D.arSubChapK.Add(chap3K);
    }

    // ¿ÂµµÇÏÁß
    int iSubChapCount = 1;
    if(CheckExistTemperatureLoad())
    {
        chap3D = MakeChapter(3, iChapCount++, _LS(IDS_DGN_PSC_RPTJTG_Temperature_Load));
    
        MakeSystemTempData(iSubChapCount, paraD);
        MakeNodalTempData(iSubChapCount, paraD);
        MakeElemTempData(iSubChapCount, paraD);
        MakeTempGradientData(iSubChapCount, paraD);
        MakeBeamSectTempData(iSubChapCount, paraD);

        m_pRptData->AddParaData(paraK, paraD);
        chap3D.arParaK.Add(paraK);

        m_pRptData->AddChapData(chap3K, chap3D);
        chap2D.arSubChapK.Add(chap3K);
    }

    // ÁöÁ¡Ä§ÇÏ.
    if(MakeSpecifiedDispData(paraD))
    {
        chap3D = MakeChapter(3, iChapCount++, _LS(IDS_DGN_PSC_RPTJTG_Settlement_Support));
    
        m_pRptData->AddParaData(paraK, paraD);
        chap3D.arParaK.Add(paraK);
        m_pRptData->AddChapData(chap3K, chap3D);
        chap2D.arSubChapK.Add(chap3K);
    }

    m_pRptData->AddChapData(chap2K, chap2D);
    chapD.arSubChapK.Add(chap2K);
            
    return TRUE;
}

// 4.6.1. È°ÇÏÁß Type Table ±¸¼º.
BOOL CRptJTG_D62_04_Seismic::MakeLiveLoadData(T_PARA_RPT& paraD)
{
    int iExistCount = 0;

    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");

    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;

    // Table Header.
    thData.Initialize();    
    thData.arTDK.Add(AddTDString(_LS(IDS_DGN_PSC_RPTJTG_Vehicular_Load_Name), 1, 1, 1));  
    thData.arTDK.Add(AddTDString(_LS(IDS_DGN_PSC_RPTJTG_Vehicular_Load_Type), 1, 1, 1));
    thData.arTDK.Add(AddTDString(_LS(IDS_DGN_PSC_RPTJTG_Description), 1, 1, 1));

    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    m_pRptData->AddTRData(thK, thData);  
    tableD.arTHK.Add(thK);

    // Table Data.
    tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;

    T_MVHL_D mvhlD;
    CArray<T_MVHL_K, T_MVHL_K> arMvhlKeyList;
    m_pDoc->m_pAttrCtrl->GetMvhlKeyList(arMvhlKeyList);
 
    int iCount = arMvhlKeyList.GetSize();
    for(int i=0; i<iCount; i++)
    {
        if(m_pDoc->m_pAttrCtrl->GetMvhl(arMvhlKeyList[i], mvhlD))
        {
            tdData.Initialize();

            CString strDescript = GetLiveLoadTypeDescription(mvhlD.VehicleTypeName);
            tdData.arTDK.Add(AddTDString(mvhlD.VehicleLoadName, 0, 1, 1, m_TDCenterCssK));
            tdData.arTDK.Add(AddTDString(mvhlD.VehicleTypeName, 0, 1, 1, m_TDCenterCssK));
            tdData.arTDK.Add(AddTDString(strDescript, 0, 1, 1, m_TDCenterCssK));
                        
            m_pRptData->AddTRData(tdK, tdData);
            tableD.arTRK.Add(tdK);

            iExistCount += 1;
        }
    }

    if(iExistCount > 0)
    {
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);

        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

// 4.6.3.1) System Temperature Load Data ±¸¼º.
BOOL CRptJTG_D62_04_Seismic::MakeSystemTempData(int& iChapID, T_PARA_RPT& paraD)
{  
    int iExistCount = 0;

    T_TEXT_K textK;
    T_TEXT_RPT textD;
    textD.CssK = m_pRptData->m_DefaultCss.TextCssK;
    textD.nCheck = 1;
    textD.strTitle = _T("Text");
        
    paraD.Initialize();
     
    T_STMP_K stmpK;
    T_STMP_D stmpD;
    CString strText = _T("");

    CArray<T_STLD_K, T_STLD_K> arStldKeyList;
    m_pDoc->m_pAttrCtrl->GetStldKeyList(arStldKeyList);

    int iCount = arStldKeyList.GetSize();
    for(int i=0; i<iCount; i++)
    {
        if(m_pDoc->m_pAttrCtrl->GetStmp(arStldKeyList[i], stmpK, stmpD))
        {
            if(strText.GetLength() > 0)
            {
                strText += _T(", ");
            }
            strText.Format(_T("%s%.1f %s"), strText, stmpD.dblSysTemp, _LS(IDS_DGN_CELSIUS_TEMP_UNIT));
            iExistCount += 1;
        }
    }

    if(iExistCount > 0)
    {
        textD.strText.Format(_T("%d) %s : %s"), iChapID++, _LS(IDS_DGN_PSC_RPTJTG_System_Temperature), strText);
        m_pRptData->AddTextData(textK, textD);
        paraD.arContentK.Add(textK);

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// 4.6.3.2) Nodal Temperature Load Data ±¸¼º.
BOOL CRptJTG_D62_04_Seismic::MakeNodalTempData(int& iChapID, T_PARA_RPT& paraD)
{
    int iExistCount = 0;

    CString strText = _T("");

    T_TEXT_K textK;
    T_TEXT_RPT textD;
    textD.CssK = m_pRptData->m_DefaultCss.TextCssK;
    textD.nCheck = 1;
    textD.strTitle = _T("Text");
    
    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");

    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;

    // Table Header.
    thData.Initialize();

    thData.arTDK.Add(AddTDString(_LS(IDS_TB_NTMP_Node), 1, 1, 1));
    thData.arTDK.Add(AddTDString(_LS(IDS_TB_NTMP_LoadCase), 1, 1, 1));
    strText.Format(_T("%s (%s)"), _LS(IDS_TB_NTMP_Temperature), _LS(IDS_DGN_CELSIUS_TEMP_UNIT));
    thData.arTDK.Add(AddTDString(strText, 1, 1, 1));

    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);

    // Table Data.
    tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;

    CArray<T_NODE_K, T_NODE_K> arNodeKeyList;
    m_pDoc->m_pAttrCtrl->GetNodeKeyList(arNodeKeyList);

    int iCount = arNodeKeyList.GetSize();
    for(int i=0; i<iCount; i++)
    {
        CArray<T_NTMP_K, T_NTMP_K> arNtmpKeyList;
        CArray<T_NTMP_D, T_NTMP_D&> arNtmpDataList;
        
        m_pDoc->m_pAttrCtrl->GetNtmp(arNodeKeyList[i], arNtmpKeyList, arNtmpDataList);
         
        int iNtmpCount = arNtmpKeyList.GetSize();
        for(int j=0; j<iNtmpCount; j++)
        { 
            tdData.arTDK.RemoveAll();

            strText.Format(_T("%d"), arNodeKeyList[i]);        
            tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

            strText = GetStaticLoadCaseName(arNtmpDataList[j].LoadCaseKey);
            tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

            strText.Format(_T("%.1f"), arNtmpDataList[j].dblNodalTemp);
            tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

            m_pRptData->AddTRData(tdK, tdData);
            tableD.arTRK.Add(tdK);

            iExistCount += 1;
        }
    }

    if(iExistCount > 0)
    {    
        textD.strText.Format(_T("%d) %s"), iChapID++, _LS(IDS_DGN_PSC_RPTJTG_Nodal_Temperature));
        m_pRptData->AddTextData(textK, textD);
        paraD.arContentK.Add(textK);

        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// 4.6.3.3) Element Temperature Load Data ±¸¼º.
BOOL CRptJTG_D62_04_Seismic::MakeElemTempData(int& iChapID, T_PARA_RPT& paraD)
{
    int iExistCount = 0;

    CString strText = _T("");

    T_TEXT_K textK;
    T_TEXT_RPT textD;
    textD.CssK = m_pRptData->m_DefaultCss.TextCssK;
    textD.nCheck = 1;
    textD.strTitle = _T("Text");
    
    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");

    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;

    // Table Header. 
    thData.Initialize();

    thData.arTDK.Add(AddTDString(_LS(IDS_TB_ELEM), 1, 1, 1));
    thData.arTDK.Add(AddTDString(_LS(IDS_TB_LOADCASE), 1, 1, 1));
    strText.Format(_T("%s (%s)"), _LS(IDS_TB_TEMPERATURE), _LS(IDS_DGN_CELSIUS_TEMP_UNIT));
    thData.arTDK.Add(AddTDString(strText, 1, 1, 1));

    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);

    // Table Data.
    tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;

    CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
    m_pDoc->m_pAttrCtrl->GetElemKeyList(arElemKeyList);

    int iCount = arElemKeyList.GetSize();
    for(int i=0; i<iCount; i++)
    {
        CArray<T_ETMP_K, T_ETMP_K> arEtmpKeyList;
        CArray<T_ETMP_D, T_ETMP_D&> arEtmpDataList;
        
        m_pDoc->m_pAttrCtrl->GetEtmp(arElemKeyList[i], arEtmpKeyList, arEtmpDataList);
         
        int iEtmpCount = arEtmpKeyList.GetSize();
        for(int j=0; j<iEtmpCount; j++)
        { 
            tdData.arTDK.RemoveAll();

            strText.Format(_T("%d"), arElemKeyList[i]);        
            tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

            strText = GetStaticLoadCaseName(arEtmpDataList[j].LoadCaseKey);
            tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

            strText.Format(_T("%.1f"), arEtmpDataList[j].dblElemTemp);
            tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

            m_pRptData->AddTRData(tdK, tdData);
            tableD.arTRK.Add(tdK);

            iExistCount += 1;
        }
    }

    if(iExistCount > 0)
    {    
        textD.strText.Format(_T("%d) %s"), iChapID++, _LS(IDS_DGN_PSC_RPTJTG_Elem_Temperature));
        m_pRptData->AddTextData(textK, textD);
        paraD.arContentK.Add(textK);

        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// 4.6.3.4) Temperature Gradient Data ±¸¼º.
BOOL CRptJTG_D62_04_Seismic::MakeTempGradientData(int& iChapID, T_PARA_RPT& paraD)
{
    int iExistCount = 0;

    CString strText = _T("");

    T_TEXT_K textK;
    T_TEXT_RPT textD;
    textD.CssK = m_pRptData->m_DefaultCss.TextCssK;
    textD.nCheck = 1;
    textD.strTitle = _T("Text");
    
    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");

    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;

    // Table Header.
    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    thData.arTDK.RemoveAll();

    thData.arTDK.Add(AddTDString(_LS(IDS_TB_GTMP_Elem), 1, 1, 1));
    thData.arTDK.Add(AddTDString(_LS(IDS_TB_GTMP_LoadCase), 1, 1, 1));
    strText.Format(_T("%s (%s)"), _LS(IDS_TB_GTMP_Tz), _LS(IDS_DGN_CELSIUS_TEMP_UNIT));
    thData.arTDK.Add(AddTDString(strText, 1, 1, 1));
    strText.Format(_T("%s (%s)"), _LS(IDS_TB_GTMP_Ty), _LS(IDS_DGN_CELSIUS_TEMP_UNIT));
    thData.arTDK.Add(AddTDString(strText, 1, 1, 1));
    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);

    // Table Data.
    tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;

    CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
    m_pDoc->m_pAttrCtrl->GetElemKeyList(arElemKeyList);

    int iCount = arElemKeyList.GetSize();
    for(int i=0; i<iCount; i++)
    {
        CArray<T_GTMP_K, T_GTMP_K> arGtmpKeyList;
        CArray<T_GTMP_D, T_GTMP_D&> arGtmpDataList;
        
        m_pDoc->m_pAttrCtrl->GetGtmp(arElemKeyList[i], arGtmpKeyList, arGtmpDataList);
         
        int iEtmpCount = arGtmpKeyList.GetSize();
        for(int j=0; j<iEtmpCount; j++)
        { 
            tdData.arTDK.RemoveAll();

            strText.Format(_T("%d"), arElemKeyList[i]);        
            tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

            strText = GetStaticLoadCaseName(arGtmpDataList[j].LoadCaseKey);
            tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

            strText.Format(_T("%.1f"), arGtmpDataList[j].dTz);
            tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

            strText.Format(_T("%.1f"), arGtmpDataList[j].dTy);
            tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

            m_pRptData->AddTRData(tdK, tdData);
            tableD.arTRK.Add(tdK);

            iExistCount += 1;
        }
    }

    if(iExistCount > 0)
    {    
        textD.strText.Format(_T("%d) %s"), iChapID++, _LS(IDS_DGN_PSC_RPTJTG_Temp_Gradient));
        m_pRptData->AddTextData(textK, textD);
        paraD.arContentK.Add(textK);

        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// 4.6.3.4) Beam Section Temperature Data ±¸¼º.
BOOL CRptJTG_D62_04_Seismic::MakeBeamSectTempData(int& iChapID, T_PARA_RPT& paraD)
{
    int iExistCount = 0;

    CString strText = _T("");

    T_TEXT_K textK;
    T_TEXT_RPT textD;
    textD.CssK = m_pRptData->m_DefaultCss.TextCssK;
    textD.nCheck = 1;
    textD.strTitle = _T("Text");
    
    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");

    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;

    // Table Header.
    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    thData.arTDK.RemoveAll();
    thData.arTDK.Add(AddTDString(_LS(IDS_TB_ELEM), 1, 1, 2));
    thData.arTDK.Add(AddTDString(_LS(IDS_TB_LOADCASE), 1, 1, 2));
    thData.arTDK.Add(AddTDString(_LS(IDS_TB_BTMP_Direction), 1, 1, 2));
    
    CString aHeadNames[] = {_T("1st T"), _T("2rd T"), _T("3rd T"), _T("4th T"), _T("5th T")};
    for(int i=0; i<5; i++)
    {
        strText.Format(_T("%s1 (%s)"), aHeadNames[i], _LS(IDS_DGN_CELSIUS_TEMP_UNIT));
        thData.arTDK.Add(AddTDString(strText, 1, 1, 1));
    }
    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);

    thData.arTDK.RemoveAll();
    for(int i=0; i<5; i++)
    {
        strText.Format(_T("%s2 (%s)"), aHeadNames[i], _LS(IDS_DGN_CELSIUS_TEMP_UNIT));
        thData.arTDK.Add(AddTDString(strText, 1, 1, 1));
    }
    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);

    // Table Data.
    tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;

    CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
    m_pDoc->m_pAttrCtrl->GetElemKeyList(arElemKeyList);

    int iCount = arElemKeyList.GetSize();
    for(int i=0; i<iCount; i++)
    {
        CArray<T_BTMP_K, T_BTMP_K> arBtmpKeyList;
        CArray<T_BTMP_D, T_BTMP_D&> arBtmpDataList;
        
        m_pDoc->m_pAttrCtrl->GetBtmp(arElemKeyList[i], arBtmpKeyList, arBtmpDataList);
         
        int iEtmpCount = arBtmpKeyList.GetSize();
        for(int j=0; j<iEtmpCount; j++)
        { 
            tdData.arTDK.RemoveAll();

            strText.Format(_T("%d"), arElemKeyList[i]);        
            tdData.arTDK.Add(AddTDString(strText, 0, 1, 2, m_TDCenterCssK));

            strText = GetStaticLoadCaseName(arBtmpDataList[j].LoadCaseKey);
            tdData.arTDK.Add(AddTDString(strText, 0, 1, 2, m_TDCenterCssK));
            tdData.arTDK.Add(AddTDString(GetBeamSectTempLoadDirection(arBtmpDataList[j].nDirection), 0, 1, 2, m_TDCenterCssK));

            for(int k=0; k<5; k++)
            {
                strText.Format(_T("%.1f"), arBtmpDataList[j].SectTmp[k].dT1);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
            }
            m_pRptData->AddTRData(tdK, tdData);
            tableD.arTRK.Add(tdK);
            
            tdData.arTDK.RemoveAll();
            for(int k=0; k<5; k++)
            {
                strText.Format(_T("%.1f"), arBtmpDataList[j].SectTmp[k].dT2);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
            }
            m_pRptData->AddTRData(tdK, tdData);
            tableD.arTRK.Add(tdK);

            iExistCount += 1;
        }
    }

    if(iExistCount > 0)
    {    
        textD.strText.Format(_T("%d) %s"), iChapID++, _LS(IDS_DGN_PSC_RPTJTG_BeamSect_Temperature));
        m_pRptData->AddTextData(textK, textD);
        paraD.arContentK.Add(textK);

        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// 4.6.4. ÁöÁ¡Ä§ÇÏ Data¸¦ ±¸¼ºÇÔ.
BOOL CRptJTG_D62_04_Seismic::MakeSpecifiedDispData(T_PARA_RPT& paraD)
{
    int iExistCount = 0;

    CString strText = _T("");
    paraD.Initialize();
    
    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");

    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;

    // Table Header.
    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    thData.arTDK.RemoveAll();
    
    CString aHeadNames[] = { _LS(IDS_TB_SDSP_Node), _LS(IDS_TB_SDSP_LoadCase),
        _LS(IDS_TB_SDSP_DX_n), _LS(IDS_TB_SDSP_DY_n), _LS(IDS_TB_SDSP_DZ_n),
        _LS(IDS_TB_SDSP_RX_n), _LS(IDS_TB_SDSP_RY_n), _LS(IDS_TB_SDSP_RZ_n) };
    for(int i=0; i<8; i++)
    {
        if(i<2)
        {
            thData.arTDK.Add(AddTDString(aHeadNames[i], 1, 1, 1));
        }
        else if(i>4)
        {
            thData.arTDK.Add(AddTDwithUnit(aHeadNames[i], m_strAngleUnitRad, 1, 1, 1));
        }
        else
        {
            thData.arTDK.Add(AddTDwithUnit(aHeadNames[i], m_strLengthUnitM, 1, 1, 1));
        }    
    }

    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);

    // Table Data.
    tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;

    CArray<T_NODE_K, T_NODE_K> arNodeKeyList;
    m_pDoc->m_pAttrCtrl->GetNodeKeyList(arNodeKeyList);

    int iCount = arNodeKeyList.GetSize();
    for(int i=0; i<iCount; i++)
    {
        CArray<T_SDSP_K, T_SDSP_K> arSdspKeyList;
        CArray<T_SDSP_D, T_SDSP_D&> arSdspDataList;
        
        m_pDoc->m_pAttrCtrl->GetSdsp(arNodeKeyList[i], arSdspKeyList, arSdspDataList);
         
        int iSdspCount = arSdspKeyList.GetSize();
        for(int j=0; j<iSdspCount; j++)
        { 
            tdData.arTDK.RemoveAll();

            strText.Format(_T("%d"), arNodeKeyList[i]);        
            tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

            strText = GetStaticLoadCaseName(arSdspDataList[j].LoadCaseKey);
            tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

            for(int k=0; k<6; k++)
            {
                double dFactor = (k<3)? m_dLengthM : 1.0;
                strText.Format(_T("%.3f"), arSdspDataList[j].Displacement[k] * dFactor);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
            }

            m_pRptData->AddTRData(tdK, tdData);
            tableD.arTRK.Add(tdK);

            iExistCount += 1;
        }
    }

    if(iExistCount > 0)
    {    
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);

        return TRUE;
    }
    else
    {
        return FALSE;
    }  
}

// 5.1.1. Static Load Cases ¼³¸í
BOOL CRptJTG_D62_04_Seismic::MakeStaticLoadCaseData(int& iChapID, T_PARA_RPT& paraD)
{
    int iExistCount = FALSE;

    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");

    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;

    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    CString aHeadNames[] = {_LS(IDS_DGN_PSC_RPTJTG_Number), _LS(IDS_DGN_PSC_RPTJTG_Name), 
        _LS(IDS_DGN_PSC_RPTJTG_Type), _LS(IDS_DGN_PSC_RPTJTG_Description)};
    for(int i=0; i<4; i++)
    {
        thData.arTDK.Add(AddTDString(aHeadNames[i], 1));
    }

    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);
    
    CArray<T_STLD_K, T_STLD_K> arStldKeyList;
    m_pDoc->m_pAttrCtrl->GetStldKeyList(arStldKeyList);
    
    int iCount = arStldKeyList.GetSize();
    for(int i=0; i<iCount; i++)
    {
        T_STLD_K stldK = arStldKeyList.GetAt(i);
        T_STLD_D stldD; 
        stldD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetStld(stldK, stldD)) continue;

        CString LoadCaseId = _T("");
        LoadCaseId.Format(_T("%d"), stldD.LoadCaseId);
        CString LoadCaseName = stldD.LoadCaseName;
        CString LoadCaseType = m_pDoc->m_pLoadDB->GetLoadTypeName(stldD.LoadCaseType);        
        CString Description  = stldD.Description;        

        tdData.Initialize();
        tdData.arTDK.Add(AddTDString(LoadCaseId, 0, 1, 1, m_TDCenterCssK));
        tdData.arTDK.Add(AddTDString(LoadCaseName, 0, 1, 1, m_TDCenterCssK));
        tdData.arTDK.Add(AddTDString(LoadCaseType, 0, 1, 1, m_TDCenterCssK));
        tdData.arTDK.Add(AddTDString(Description));        
        m_pRptData->AddTRData(tdK, tdData);
        tableD.arTRK.Add(tdK);

        iExistCount += 1;
    }

    if(iExistCount > 0)
    {
        iChapID++;
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// 5.1.2. Response Spectrum Data¸¦ ±¸¼ºÇÔ.
BOOL CRptJTG_D62_04_Seismic::MakeResponseSpectrumData(int& iChapID, T_PARA_RPT& paraD)
{
    int iExistCount = 0;  
    paraD.Initialize();

    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");

    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;
    
    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    CString aHeadNames[] = {_LS(IDS_DGN_PSC_RPTJTG_Number), _LS(IDS_DGN_PSC_RPTJTG_Name),
        _LS(IDS_DGN_PSC_RPTJTG_Spectrum_Function) };

    for(int i=0; i<3; i++)
    {
        thData.arTDK.Add(AddTDString(aHeadNames[i], 1));
    }

    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);
    
    CArray<T_SPLC_K, T_SPLC_K> arSplcKeyList;
    m_pDoc->m_pAttrCtrl->GetSplcKeyList(arSplcKeyList);

    int iCount = arSplcKeyList.GetSize();
    for(int i=0; i<iCount; i++)
    {
        T_SPLC_K splcK = arSplcKeyList.GetAt(i);
        T_SPLC_D splcD; splcD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetSplc(splcK, splcD)) continue;
                        
        CString strSpecFuncName=_T("");
        int iSubCount = splcD.aSpfcKey.GetSize();
        for(int j=0; j<iSubCount; j++)
        {
            T_SPFC_K spfcK = splcD.aSpfcKey.GetAt(j);
            T_SPFC_D spfcD; spfcD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetSpfc(spfcK, spfcD)) continue;

            if(j==0) strSpecFuncName = spfcD.SpecFuncName;
            else     strSpecFuncName += _T(", ")+spfcD.SpecFuncName;
        }
        
        CString strLoadCaseId = _T("");
        strLoadCaseId.Format(_T("%d"), splcD.LoadCaseId);

        tdData.Initialize();
        tdData.arTDK.Add(AddTDString(strLoadCaseId, 0, 1, 1, m_TDCenterCssK));
        tdData.arTDK.Add(AddTDString(splcD.LoadCaseName, 0, 1, 1, m_TDCenterCssK));        
        tdData.arTDK.Add(AddTDString(strSpecFuncName, 0, 1, 1, m_TDCenterCssK));        
        m_pRptData->AddTRData(tdK, tdData);
        tableD.arTRK.Add(tdK);

        iExistCount += 1;
    }

    if(iExistCount > 0)
    {
        iChapID++;
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// 5.1.3. Time History Load Cases Data¸¦ ±¸¼ºÇÔ.
BOOL CRptJTG_D62_04_Seismic::MakeTimeHistoryData(int& iChapID, T_PARA_RPT& paraD)
{
    int iExistCount = FALSE;
    paraD.Initialize();

    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");

    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;
    
    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    CString aHeadNames[] = { _LS(IDS_DGN_PSC_RPTJTG_Number), _LS(IDS_DGN_PSC_RPTJTG_Name), 
        _LS(IDS_DGN_PSC_RPTJTG_Description) };
    for(int i=0; i<3; i++)
    {
        thData.arTDK.Add(AddTDString(aHeadNames[i], 1));
    }

    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);

    CArray<T_THIS_K, T_THIS_K> arThisKeyList;
    m_pDoc->m_pAttrCtrl->GetThisKeyList(arThisKeyList);
    
    int iCount = arThisKeyList.GetSize();
    for(int i=0; i<iCount; i++)
    {
        T_THIS_K thisK = arThisKeyList.GetAt(i);
        T_THIS_D thisD; thisD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetThis(thisK, thisD)) continue;        
        
        CString strLoadCaseId = _T("");
        strLoadCaseId.Format(_T("%d"), thisD.LoadCaseId);

        tdData.Initialize();
        tdData.arTDK.Add(AddTDString(strLoadCaseId, 0, 1, 1, m_TDCenterCssK));
        tdData.arTDK.Add(AddTDString(thisD.LoadCaseName, 0, 1, 1, m_TDCenterCssK));        
        tdData.arTDK.Add(AddTDString(thisD.Description, 0, 1, 1, m_TDCenterCssK));
        m_pRptData->AddTRData(tdK, tdData);
        tableD.arTRK.Add(tdK);

        iExistCount++;
    }

    if(iExistCount > 0)
    {
        iChapID++;
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// 5.1.4. Moving Load Case Data¸¦ ±¸¼ºÇÔ.
BOOL CRptJTG_D62_04_Seismic::MakeMovingLoadData(int& iChapID, T_PARA_RPT& paraD)
{
    int iExistCount = FALSE;
    paraD.Initialize();

    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");

    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;
    
    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    CString aHeadNames[] = {_LS(IDS_DGN_PSC_RPTJTG_Number), _LS(IDS_DGN_PSC_RPTJTG_Name),
        _LS(IDS_DGN_PSC_RPTJTG_Description)};

    for(int i=0; i<3; i++)
    {
        thData.arTDK.Add(AddTDString(aHeadNames[i], 1));
    }

    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);

    CString strLoadCaseId = _T("");      
    CString strLoadCaseName = _T("");
    CString strDescription  = _T("");

    // Get Moving Load Code  
    T_MVCD_D MvcdD;
    m_pDoc->m_pAttrCtrl->GetMvcd(MvcdD);
    int nCodeType = MvcdD.nCodeType;
    
    if(nCodeType==D_MOVE_CODE_JAPAN)
    {
        CArray<T_MVLDjp_K, T_MVLDjp_K> arMvldjpKeyList;
        m_pDoc->m_pAttrCtrl->GetMvldjpKeyList(arMvldjpKeyList);
    
        int iCount = arMvldjpKeyList.GetSize();
        for(int i=0; i<arMvldjpKeyList.GetSize(); i++)
        {
            T_MVLDjp_K mvldjpK = arMvldjpKeyList.GetAt(i);
            T_MVLDjp_D mvldjpD; 
            if(!m_pDoc->m_pAttrCtrl->GetMvldjp(mvldjpK, mvldjpD)) continue;        
                            
            strLoadCaseId.Format(_T("%d"), mvldjpD.LoadCaseId);
            strLoadCaseName = mvldjpD.LoadCaseName;                
            strDescription  = mvldjpD.Description;   
            
            iExistCount += 1;
        }
    }
    else if(nCodeType==D_MOVE_CODE_CHINA)
    {
        CArray<T_MVLDch_K, T_MVLDch_K> arMvldchKeyList;
        m_pDoc->m_pAttrCtrl->GetMvldchKeyList(arMvldchKeyList);
    
        int iCount = arMvldchKeyList.GetSize();
        for(int i=0; i<iCount; i++)
        {
            T_MVLDch_K mvldchK = arMvldchKeyList.GetAt(i);
            T_MVLDch_D mvldchD; mvldchD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetMvldch(mvldchK, mvldchD)) continue;        
                            
            strLoadCaseId.Format(_T("%d"), mvldchD.LoadCaseId);
            strLoadCaseName = mvldchD.LoadCaseName;                
            strDescription  = mvldchD.Description;     
            
            iExistCount += 1;
        }
    }
    else if(nCodeType==D_MOVE_CODE_INDIA) // India
    {
        CArray<T_MVLDid_K, T_MVLDid_K> arMvldidKeyList;
        m_pDoc->m_pAttrCtrl->GetMvldidKeyList(arMvldidKeyList);
    
        int iCount = arMvldidKeyList.GetSize();
        for(int i=0; i<iCount; i++)
        {
            T_MVLDid_K mvldidK = arMvldidKeyList.GetAt(i);
            T_MVLDid_D mvldidD; mvldidD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetMvldid(mvldidK, mvldidD)) continue;        
                            
            strLoadCaseId.Format(_T("%d"), mvldidD.LoadCaseId);
            strLoadCaseName = mvldidD.LoadCaseName;                
            strDescription  = mvldidD.Description;        

            iExistCount += 1;
        }
    }
    else if(nCodeType==D_MOVE_CODE_BS) // Bs
    {
        CArray<T_MVLDbs_K, T_MVLDbs_K> arMvldbsKeyList;
        m_pDoc->m_pAttrCtrl->GetMvldbsKeyList(arMvldbsKeyList);
    
        int iCount = arMvldbsKeyList.GetSize();
        for(int i=0; i<iCount; i++)
        {
            T_MVLDbs_K mvldbsK = arMvldbsKeyList.GetAt(i);
            T_MVLDbs_D mvldbsD; mvldbsD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetMvldbs(mvldbsK, mvldbsD)) continue;        
                            
            strLoadCaseId.Format(_T("%d"), mvldbsD.LoadCaseId);
            strLoadCaseName = mvldbsD.LoadCaseName;                
            strDescription  = mvldbsD.Description;  
            
            iExistCount += 1;
        }
    }
    else if(nCodeType==D_MOVE_CODE_FRANCE) // France
    {
        CArray<T_MVLDfr_K, T_MVLDfr_K> arMvldfrKeyList;
        m_pDoc->m_pAttrCtrl->GetMvldfrKeyList(arMvldfrKeyList);
    
        int iCount = arMvldfrKeyList.GetSize();
        for(int i=0; i<iCount; i++)
        {
            T_MVLDfr_K mvldfrK = arMvldfrKeyList.GetAt(i);
            T_MVLDfr_D mvldfrD; mvldfrD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetMvldfr(mvldfrK, mvldfrD)) continue;
                            
            strLoadCaseId.Format(_T("%d"), mvldfrD.LoadCaseId);
            strLoadCaseName = mvldfrD.LoadCaseName;
            strDescription  = mvldfrD.Description;
            
            iExistCount += 1;
        }
    }
    else if(nCodeType==D_MOVE_CODE_TRANS) // transverse
    {
        CArray<T_MVLDtr_K, T_MVLDtr_K> arMvldtrKeyList;
        m_pDoc->m_pAttrCtrl->GetMvldtrKeyList(arMvldtrKeyList);
    
        int iCount = arMvldtrKeyList.GetSize();
        for(int i=0; i<iCount; i++)
        {
            T_MVLDtr_K mvldtrK = arMvldtrKeyList.GetAt(i);
            T_MVLDtr_D mvldtrD; mvldtrD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetMvldtr(mvldtrK, mvldtrD)) continue;        
                            
            strLoadCaseId.Format(_T("%d"), mvldtrD.LoadCaseId);
            strLoadCaseName = mvldtrD.LoadCaseName;                
            strDescription  = mvldtrD.Description;       
            
            iExistCount += 1;
        }
    }
    else
    {
        CArray<T_MVLD_K, T_MVLD_K> arMvldKeyList;
        m_pDoc->m_pAttrCtrl->GetMvldKeyList(arMvldKeyList);
    
        int iCount = arMvldKeyList.GetSize();
        for(int i=0; i<iCount; i++)
        {
            T_MVLD_K mvldK = arMvldKeyList.GetAt(i);
            T_MVLD_D mvldD; mvldD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetMvld(mvldK, mvldD)) continue;        
                            
            strLoadCaseId.Format(_T("%d"), mvldD.LoadCaseId);
            strLoadCaseName = mvldD.LoadCaseName;                
            strDescription  = mvldD.Description;    
            
            iExistCount += 1;
        }
    }

    tdData.Initialize();
    tdData.arTDK.Add(AddTDString(strLoadCaseId, 0, 1, 1, m_TDCenterCssK));
    tdData.arTDK.Add(AddTDString(strLoadCaseName, 0, 1, 1, m_TDCenterCssK));                
    tdData.arTDK.Add(AddTDString(strDescription, 0, 1, 1, m_TDCenterCssK));
    m_pRptData->AddTRData(tdK, tdData);
    tableD.arTRK.Add(tdK);
        
    if(iExistCount > 0)
    {
        iChapID++;
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// 5.1.5. Settlement Load Cases Data¸¦ ±¸¼ºÇÔ.
BOOL CRptJTG_D62_04_Seismic::MakeSettlementData(int& iChapID, T_PARA_RPT& paraD)
{
    int iExistCount = FALSE;
    paraD.Initialize();

    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");

    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;
    
    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    CString aHeadNames[] = {_LS(IDS_DGN_PSC_RPTJTG_Number), _LS(IDS_DGN_PSC_RPTJTG_Name),
        _LS(IDS_DGN_PSC_RPTJTG_Description)};

    for(int i=0; i<3; i++)
    {
        thData.arTDK.Add(AddTDString(aHeadNames[i], 1));
    }

    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);

    CArray<T_SMLC_K, T_SMLC_K> arSmlcKeyList;
    m_pDoc->m_pAttrCtrl->GetSmlcKeyList(arSmlcKeyList);

    int iCount = arSmlcKeyList.GetSize();
    for(int i=0; i<iCount; i++)
    {
        T_SMLC_K smlcK = arSmlcKeyList.GetAt(i);
        T_SMLC_D smlcD; smlcD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetSmlc(smlcK, smlcD)) continue;
     
        CString strLoadCaseId = _T("");
        strLoadCaseId.Format(_T("%d"), smlcD.LoadCaseId);
     
        tdData.Initialize();
        tdData.arTDK.Add(AddTDString(strLoadCaseId, 0, 1, 1, m_TDCenterCssK));
        tdData.arTDK.Add(AddTDString(smlcD.LoadCaseName, 0, 1, 1, m_TDCenterCssK));                
        tdData.arTDK.Add(AddTDString(smlcD.Description, 0, 1, 1, m_TDCenterCssK));
        m_pRptData->AddTRData(tdK, tdData);
        tableD.arTRK.Add(tdK);

        iExistCount += 1;
    }

    if(iExistCount > 0)
    {
        iChapID++;
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// 5.1.6. Grid Load Cases Data¸¦ ±¸¼ºÇÔ.
BOOL CRptJTG_D62_04_Seismic::MakeGridLoadData(int& iChapID, T_PARA_RPT& paraD)
{
    int iExistCount = FALSE;
    paraD.Initialize();

    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");

    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;
    
    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    CString aHeadNames[] = {_LS(IDS_DGN_PSC_RPTJTG_Number), _LS(IDS_DGN_PSC_RPTJTG_Name),
        _LS(IDS_DGN_PSC_RPTJTG_Type), _LS(IDS_DGN_PSC_RPTJTG_Description)};
    for(int i=0; i<3; i++)
    {
        thData.arTDK.Add(AddTDString(aHeadNames[i], 1));
    }

    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);

    CArray<T_GILC_K, T_GILC_K> arGilcKeyList;
    m_pDoc->m_pAttrCtrl->GetGilcKeyList(arGilcKeyList);
    
    CString aLoadType[5] = {_LS(IDS_TMWK_GRID_LOAD_DEAD), _LS(IDS_TMWK_GRID_LOAD_ODEAD),
        _LS(IDS_TMWK_GRID_LOAD_VLIVE), _LS(IDS_TMWK_GRID_LOAD_CLIVE), _LS(IDS_TMWK_GRID_LOAD_OLIVE)};
        
    int iCount = arGilcKeyList.GetSize();
    for(int i=0; i<iCount; i++)
    {
        T_GILC_K gilcK = arGilcKeyList.GetAt(i);
        T_GILC_D gilcD; gilcD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetGilc(gilcK, gilcD)) continue;

        CString strLoadCaseId = _T("");
        strLoadCaseId.Format(_T("%d"), gilcD.LoadCaseId);

        tdData.Initialize();
        tdData.arTDK.Add(AddTDString(strLoadCaseId, 0, 1, 1, m_TDCenterCssK));
        tdData.arTDK.Add(AddTDString(gilcD.LoadCaseName, 0, 1, 1, m_TDCenterCssK));                
        tdData.arTDK.Add(AddTDString(aLoadType[gilcD.nLoadCaseType], 0, 1, 1, m_TDCenterCssK));
        tdData.arTDK.Add(AddTDString(gilcD.Description, 0, 1, 1, m_TDCenterCssK));
        m_pRptData->AddTRData(tdK, tdData);
        tableD.arTRK.Add(tdK);

        iExistCount += 1;
    }

    if(iExistCount > 0)
    {
        iChapID++;
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// 5.2.1 ÇÏÁßÁ¶ÇÕ Case List Data¸¦ ±¸¼ºÇÔ.
//   iChapID : Chapter index. Data°¡ ±¸¼ºµÇ´Â °æ¿ì¿¡´Â iChapID++·Î return.
//   paraD : ±¸¼ºµÈ Paragraph Data.
//   RETURN : Data°¡ ±¸¼ºµÇ¸é TRUE, ¾Æ´Ï¸é FALSE.   
BOOL CRptJTG_D62_04_Seismic::MakeLoadCombCaseData(int& iChapID, T_PARA_RPT& paraD)
{
    int iExistCount = FALSE;
    paraD.Initialize();

    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");

    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;

    // Table Header.
    thData.Initialize();
    
    CString aHeadNames[] = { _LS(IDS_DGN_PSC_RPTJTG_Name), _LS(IDS_DGN_PSC_RPTJTG_Description) };
    for(int i=0; i<2; i++)
    {
        thData.arTDK.Add(AddTDString(aHeadNames[i], 1, 1, 1));
    }

    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);

    CMap<CString, LPCTSTR, CString, CString&> mapLcom;  
    GetLoadCombinationCases(mapLcom);

    // Table Data.
    CString strKey  = _T("");
    CString strData = _T("");
    tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;

    POSITION pos = mapLcom.GetStartPosition();  
    while(pos != NULL)
    {
        mapLcom.GetNextAssoc(pos, strKey, strData);
        tdData.Initialize();

        tdData.arTDK.Add(AddTDString(strKey, 0, 1, 1, m_TDCenterCssK));
        tdData.arTDK.Add(AddTDString(strData, 0, 1, 1, m_TDCenterCssK));
        
        m_pRptData->AddTRData(tdK, tdData);
        tableD.arTRK.Add(tdK);

        iExistCount += 1;
    }

    if(iExistCount > 0)
    {
        iChapID++;
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


BOOL CRptJTG_D62_04_Seismic::IsEqLoad_JTG_B02(T_LCOM_D lcomD)
{
    T_LCOM_D_UL	LcomUlData;      LcomUlData.Initialize();
    if(lcomD.LoadCombType == 3)//ABS
    {
        BOOL bOnlyE = TRUE;
        LcomUlData = lcomD;
        int iNum = LcomUlData.Combination.GetSize();
        if(iNum<=0) bOnlyE = FALSE;
        for(int i=0; i<iNum; i++)
        {
            unsigned int iAnalType = LcomUlData.Combination[i].AnalType;
            if(iAnalType!=D_LCOM_SPECTRUM && iAnalType!=D_LCOM_ECCENSPEC) bOnlyE = FALSE;
        }
        return bOnlyE; 
    }
    else if(lcomD.LoadCombType == 0)//0=Add(Default), 1=Envelope, 2=ABS, 3=SRSS.
    {
        int iLcomType=D_LCOMTYPE_CONCRETE;  
        if(!m_pDoc->m_pAttrCtrl->NormalizeLcom(lcomD,LcomUlData))	{ASSERT(0); return FALSE;}
        int iNum = LcomUlData.Combination.GetSize();
        for(int i=0; i<iNum; i++)
        {
            unsigned int iAnalType = LcomUlData.Combination[i].AnalType;
            if(iAnalType==D_LCOM_CB_CONCRETE) 
            {
                T_LCOM_D alcomD;    
                if(m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, LcomUlData.Combination[i].LoadCaseKey, alcomD)) 
                {
                    return IsEqLoad_JTG_B02(alcomD);
                }        
            }
        }    
    }
    return FALSE;
}


// 5.2 ÇÏÁßÁ¶ÇÕ ¼³¸í Data¸¦ ±¸¼ºÇÔ.
BOOL CRptJTG_D62_04_Seismic::MakeLoadCombData(int& iChapID, T_PARA_RPT& paraD)
{
    int iExistCount = FALSE;
    paraD.Initialize();

    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");

    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;

    // Table Header.
    thData.Initialize();
    
    CString aHeadNames[] = { _LS(IDS_DGN_PSC_RPTJTG_Name), _LS(IDS_DGN_PSC_RPTJTG_Active),
        _LS(IDS_TB_ELASTIC), _LS(IDS_DGN_PSC_RPTJTG_Description) };
    for(int i=0; i<4; i++)
    {
        thData.arTDK.Add(AddTDString(aHeadNames[i], 1, 1, 1));
    }

    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);

    // Table Data.
    CString strText = _T("");
    tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;

    CArray<T_LCOM_K, T_LCOM_K> arLcomKeyList;
    m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, arLcomKeyList);
    
    int iCount = arLcomKeyList.GetSize();
    for(int i=0; i<iCount; i++)
    { 
        T_LCOM_D lcomD;    
        if(m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, arLcomKeyList[i], lcomD))
        {
            if (!IsEqLoad_JTG_B02(lcomD))  continue;//only Seiemic×éºÏ

            tdData.Initialize();
            tdData.arTDK.Add(AddTDString(lcomD.LoadCombName, 0, 1, 1, m_TDCenterCssK));
            tdData.arTDK.Add(AddTDString(GetLcombActiveType(lcomD.nActive), 0, 1, 1, m_TDCenterCssK));
            strText = (lcomD.bElasticStage)? _T("Yes") : _T("No");
            tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
            tdData.arTDK.Add(AddTDString(lcomD.Description, 0, 1, 1));

            tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;
            m_pRptData->AddTRData(tdK, tdData);
            tableD.arTRK.Add(tdK);

            iExistCount += 1;
        }    
    }

    if(iExistCount > 0)
    {
        iChapID++;
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//6.1ÇÅ¶Õ¡¢ÇÅÌ¨µ¥ÔªÇ¿¶È
//bPier: true-pier; false-Abutment
BOOL CRptJTG_D62_04_Seismic::MakeSeiemicPierCompCheckData(int& iChapID, T_PARA_RPT& paraD, BOOL bE1, BOOL bPier)
{
    int iExistCount = FALSE;
    paraD.Initialize();

    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");

    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;
    CArray<T_CHAR_K, T_CHAR_K> arCharK;

    // Table Header.
    thData.Initialize();  
    CString aHeadNames1[] = { _LS(IDS_TB_PSCD_BDCY_JTG_ELEM), _LS(IDS_TB_PSCD_BDCY_JTG_PART), _LS(IDS_TB_PSCD_BDCW_JTG_LOAD), 
        _LS(IDS_TB_PSCD_BDCW_JTG_TYPE),_LS(IDS_TB_PSCD_BDCY_JTG_OK),
        _T("x"),_T("rNd"), _T("e"), _T("e'"),  _T("Nn")};
    CString aMaxMinName[]= {_T("ÖáÐÄ-Fxmin"),_T("Æ«ÐÄ-Fxmin(My)"),_T("Æ«ÐÄ-Fxmin(Mz)"),_T("Æ«ÐÄ-Mymax"),_T("Æ«ÐÄ-Mymin")};

    for(int i=0; i<10; i++)
    {  
        if(i==5|| i==7 || i==8)
        {
            thData.arTDK.Add(AddTDwithUnit(aHeadNames1[i], m_strLengthUnit, 1, 1, 1));
        }else if(i==6 || i==9)
        {
            thData.arTDK.Add(AddTDwithUnit(aHeadNames1[i], m_strForceUnit, 1, 1, 1));
        }
        else
        {
            thData.arTDK.Add(AddTDString(aHeadNames1[i], 1, 1, 1));
        }    
    }
             
    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);

    CString strText = _T("");
    //get pier or abutment elemk
    CArray<UINT, UINT> aElemKeyList;
    m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetPierCompKeyList(aElemKeyList, bE1,bPier);  
    for(int i=0; i<aElemKeyList.GetSize(); i++)
    {
        T_CSTCS_CH Cscs;      
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetPierCompData(aElemKeyList.GetAt(i), Cscs, bE1);
        for(int j=0; j<10; j++)
        {
            if (j== 2 || j ==7) continue;//×¢ÊÍµôMzµÄÑéËã
            tdData.Initialize();
            T_CSTCS_CH_BASE cscsBase = Cscs.cstcsBase[j];
            if(cscsBase.bCHK)
            {
             strText.Format(_T("%d"), m_arElemKeyList[i]);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                strText = (j<5)? _T("I") : _T("J");
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                strText = GetLoadCombName(cscsBase.LcomK);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                strText =aMaxMinName[j%5];
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                strText = (cscsBase.bOK)? _T("OK") : _T("NG");
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                strText.Format(_T("%.3f"), cscsBase.dx);//dx
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                for(int k=0; k<4; k++)
                {
                    if (cscsBase.drNd < 0 ) {//is tensile
                        if (k==0) strText.Format(_T("%.3f"),cscsBase.drNd * m_dForcekN); else strText = _T("-");
                    }else{
                        if (k ==0) strText.Format(_T("%.3f"), cscsBase.drNd * m_dForcekN);
                        if (k ==1) strText.Format(_T("%.3f"), cscsBase.drNd < 1.0E-07 ? 0.0 : cscsBase.drNde /cscsBase.drNd);
                        if (k ==2) strText.Format(_T("%.3f"), cscsBase.drNd < 1.0E-07 ? 0.0 : cscsBase.drNde1 /cscsBase.drNd);
                        if (k ==3) strText.Format(_T("%.3f"),cscsBase.dNn* m_dForcekN);
                    }
                    tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                }      
                tdData.CssK = m_TDCenterCssK;
                m_pRptData->AddTRData(tdK, tdData);
                tableD.arTRK.Add(tdK);
                iExistCount += 1;
            }
        }    
    }
    if(iExistCount > 0)
    {
        iChapID++;
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
	//6.2Ö÷¹°È¦Ç¿¶È-¿¹Íä
BOOL CRptJTG_D62_04_Seismic::MakeSeiemicArchCrossCheckData(int& iChapID, T_PARA_RPT& paraD, BOOL bE1)
{
    int iExistCount = FALSE;
    paraD.Initialize();
    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");

    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;
    CArray<T_CHAR_K, T_CHAR_K> arCharK;

    // Table Header.
    thData.Initialize();  
    CString aHeadNames1[] = { _LS(IDS_TB_PSCD_BDCY_JTG_ELEM), _LS(IDS_TB_PSCD_BDCY_JTG_PART), 
        _LS(IDS_TB_PSCD_BDCY_JTG_MAX_MIN), _LS(IDS_TB_PSCD_BDCY_JTG_LOAD), 
        _LS(IDS_TB_PSCD_BDCY_JTG_TYPE), _LS(IDS_TB_PSCD_BDCY_JTG_OK) };    
    for(int i=0; i<6; i++)
    {
        thData.arTDK.Add(AddTDString(aHeadNames1[i], 1, 1, 1));
    }
    CString aHeadNames2[] = { _LS(IDS_TB_PSCD_BDCY_JTG_MUY), _LS(IDS_TB_PSCD_BDCY_JTG_MNY) };
    for(int i=0; i<2; i++)
    {
        thData.arTDK.Add(AddTDwithUnit(aHeadNames2[i], m_strMomentUnit, 1, 1, 1));
    }       
    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);

    CString strText = _T("");
    for(int i=0; i<m_iElemNum; i++)
    {
        T_CBCS_CH cbcs;      
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetArchCrossData(m_arElemKeyList[i], cbcs, bE1);        
        for(int j=0; j<4; j++)
        {
            tdData.Initialize();
            T_CBCS_CH_BASE cbcsBase = cbcs.cbcsBase[j];
            if(cbcsBase.bCHK)
            {
                strText.Format(_T("%d"), m_arElemKeyList[i]);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                strText = (j<2)? _T("I") : _T("J");
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                int index = (j%2==0)? 0 : 1;
                strText = GetMaxMinType(index);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                strText = GetLoadCombName(cbcsBase.LcomK);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                strText = GetLoadCombType(cbcsBase.iMax);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                strText = (cbcsBase.bOK)? _T("OK") : _T("NG");
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                double dFP1[] = { cbcsBase.drMuy, cbcsBase.dMny };
                for(int k=0; k<2; k++)
                {
                    strText.Format(_T("%.3f"), dFP1[k] * m_dMomentkNM);
                    tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                }      
                tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;
                m_pRptData->AddTRData(tdK, tdData);
                tableD.arTRK.Add(tdK);
                iExistCount += 1;
            }
        }    
    }
    if(iExistCount > 0)
    {
        iChapID++;
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
	//6.3Ö÷¹°È¦Ç¿¶È-Æ«Ñ¹
BOOL CRptJTG_D62_04_Seismic::MakeSeiemicArchCompCheckData(int& iChapID, T_PARA_RPT& paraD, BOOL bE1)
{
    int iExistCount = FALSE;
    paraD.Initialize();

    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");

    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;
    CArray<T_CHAR_K, T_CHAR_K> arCharK;

    // Table Header.
    thData.Initialize();  
    CString aHeadNames1[] = { _LS(IDS_TB_PSCD_BDCY_JTG_ELEM), _LS(IDS_TB_PSCD_BDCY_JTG_PART), _LS(IDS_TB_PSCD_BDCW_JTG_LOAD), 
        _LS(IDS_TB_PSCD_BDCW_JTG_TYPE),_LS(IDS_TB_PSCD_BDCY_JTG_OK),
        _T("x"),_T("rNd"), _T("e"), _T("e'"),  _T("Nn")};
    CString aMaxMinName[]= {_T("ÖáÐÄ-Fxmin"),_T("Æ«ÐÄ-Fxmin(My)"),_T("Æ«ÐÄ-Fxmin(Mz)"),_T("Æ«ÐÄ-Mymax"),_T("Æ«ÐÄ-Mymin")};

    for(int i=0; i<10; i++)
    {  
        if(i==5|| i==7 || i==8)
        {
            thData.arTDK.Add(AddTDwithUnit(aHeadNames1[i], m_strLengthUnit, 1, 1, 1));
        }else if(i==6 || i==9)
        {
            thData.arTDK.Add(AddTDwithUnit(aHeadNames1[i], m_strForceUnit, 1, 1, 1));
        }
        else
        {
            thData.arTDK.Add(AddTDString(aHeadNames1[i], 1, 1, 1));
        }    
    }
             
    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);

    CString strText = _T("");
    for(int i=0; i<m_iElemNum; i++)
    {
        T_CSTCS_CH Cscs;      
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetArchCompData(m_arElemKeyList[i], Cscs, bE1);
        for(int j=0; j<10; j++)
        {
            if (j== 2 || j ==7) continue;//×¢ÊÍµôMzµÄÑéËã
            tdData.Initialize();
            T_CSTCS_CH_BASE cscsBase = Cscs.cstcsBase[j];
            if(cscsBase.bCHK)
            {
             strText.Format(_T("%d"), m_arElemKeyList[i]);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                strText = (j<5)? _T("I") : _T("J");
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                strText = GetLoadCombName(cscsBase.LcomK);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                strText =aMaxMinName[j%5];
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                strText = (cscsBase.bOK)? _T("OK") : _T("NG");
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                strText.Format(_T("%.3f"), cscsBase.dx);//dx
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                for(int k=0; k<4; k++)
                {
                    if (cscsBase.drNd < 0 ) {//is tensile
                        if (k==0) strText.Format(_T("%.3f"),cscsBase.drNd * m_dForcekN); else strText = _T("-");
                    }else{
                        if (k ==0) strText.Format(_T("%.3f"), cscsBase.drNd * m_dForcekN);
                        if (k ==1) strText.Format(_T("%.3f"), cscsBase.drNd < 1.0E-07 ? 0.0 : cscsBase.drNde /cscsBase.drNd);
                        if (k ==2) strText.Format(_T("%.3f"), cscsBase.drNd < 1.0E-07 ? 0.0 : cscsBase.drNde1 /cscsBase.drNd);
                        if (k ==3) strText.Format(_T("%.3f"),cscsBase.dNn* m_dForcekN);
                    }
                    tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                }      
                tdData.CssK = m_TDCenterCssK;
                m_pRptData->AddTRData(tdK, tdData);
                tableD.arTRK.Add(tdK);
                iExistCount += 1;
            }
        }    
    }
    if(iExistCount > 0)
    {
        iChapID++;
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
	//6.4¸ÇÁºÇ¿¶È-¿¹Íä
BOOL CRptJTG_D62_04_Seismic::MakeSeiemicBentCapBendCheckData(int& iChapID, T_PARA_RPT& paraD, BOOL bE1)
{
    int iExistCount = FALSE;
    paraD.Initialize();
    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");

    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;
    CArray<T_CHAR_K, T_CHAR_K> arCharK;

    // Table Header.
    thData.Initialize();  
    CString aHeadNames1[] = { _LS(IDS_TB_PSCD_BDCY_JTG_ELEM), _LS(IDS_TB_PSCD_BDCY_JTG_PART), 
        _LS(IDS_TB_PSCD_BDCY_JTG_MAX_MIN), _LS(IDS_TB_PSCD_BDCY_JTG_LOAD), 
        _LS(IDS_TB_PSCD_BDCY_JTG_TYPE), _LS(IDS_TB_PSCD_BDCY_JTG_OK) };    
    for(int i=0; i<6; i++)
    {
        thData.arTDK.Add(AddTDString(aHeadNames1[i], 1, 1, 1));
    }
    CString aHeadNames2[] = { _LS(IDS_TB_PSCD_BDCY_JTG_MUY), _LS(IDS_TB_PSCD_BDCY_JTG_MNY) };
    for(int i=0; i<2; i++)
    {
        thData.arTDK.Add(AddTDwithUnit(aHeadNames2[i], m_strMomentUnit, 1, 1, 1));
    }       
    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);

    CString strText = _T("");
    for(int i=0; i<m_iElemNum; i++)
    {
        T_CBCS_CH cbcs;      
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetBentCapBendData(m_arElemKeyList[i], cbcs, bE1);        
        for(int j=0; j<4; j++)
        {
            tdData.Initialize();
            T_CBCS_CH_BASE cbcsBase = cbcs.cbcsBase[j];
            if(cbcsBase.bCHK)
            {
                strText.Format(_T("%d"), m_arElemKeyList[i]);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                strText = (j<2)? _T("I") : _T("J");
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                int index = (j%2==0)? 0 : 1;
                strText = GetMaxMinType(index);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                strText = GetLoadCombName(cbcsBase.LcomK);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                strText = GetLoadCombType(cbcsBase.iMax);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                strText = (cbcsBase.bOK)? _T("OK") : _T("NG");
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                double dFP1[] = { cbcsBase.drMuy, cbcsBase.dMny };
                for(int k=0; k<2; k++)
                {
                    strText.Format(_T("%.3f"), dFP1[k] * m_dMomentkNM);
                    tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                }      
                tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;
                m_pRptData->AddTRData(tdK, tdData);
                tableD.arTRK.Add(tdK);
                iExistCount += 1;
            }
        }    
    }
    if(iExistCount > 0)
    {
        iChapID++;
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
	//6.5¸ÇÁºÇ¿¶È-¿¹¼ô
BOOL CRptJTG_D62_04_Seismic::MakeSeiemicBentCapShearCheckData(int& iChapID, T_PARA_RPT& paraD, BOOL bE1)
{
    int iExistCount = FALSE;
    paraD.Initialize();
    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");
    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;
    CArray<T_CHAR_K, T_CHAR_K> arCharK;
    // Table Header.
    thData.Initialize();  
    CString aHeadNames1[] = { _LS(IDS_TB_PSCD_CUMS_JTG_ELEM), _LS(IDS_TB_PSCD_CUMS_JTG_PART), 
        _LS(IDS_TB_PSCD_CUMS_JTG_MAX_MIN), _LS(IDS_TB_PSCD_CUMS_JTG_LOAD), 
        _LS(IDS_TB_PSCD_CUMS_JTG_TYPE), _LS(IDS_TB_PSCD_CUMS_JTG_OK), 
        _LS(IDS_TB_PSCD_CUMS_JTG_VU), _LS(IDS_TB_PSCD_CUMS_JTG_VN),
        _LS(IDS_TB_PSCD_CUMS_JTG_SECT_OK) , _LS(IDS_TB_PSCD_CUMS_JTG_SHRLOAD)};
    for(int i=0; i<10; i++)
    {
        if(i==6 || i==7)
        {
            thData.arTDK.Add(AddTDwithUnit(aHeadNames1[i], m_strForceUnit, 1, 1, 1));
        }
        else
        {
            thData.arTDK.Add(AddTDString(aHeadNames1[i], 1, 1, 1));
        }    
    }       
    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);
    CString strText = _T("");
    for(int i=0; i<m_iElemNum; i++)
    {
        T_CSIS_CH csis; 
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetBentCapShearData(m_arElemKeyList[i], csis, bE1);        
        for(int j=0; j<4; j++)
        {
            tdData.Initialize();
            T_CSIS_CH_BASE csisBase = csis.CsisBase[j];
            if(csisBase.bCHK)
            {
                strText.Format(_T("%d"), m_arElemKeyList[i]);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

                strText = (j<2)? _T("I") : _T("J");
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

                int index = (j%2==0)? 0 : 1;
                strText = GetMaxMinType(index);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

                strText = GetLoadCombName(csisBase.LcomK);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

                strText = GetLoadCombType(csisBase.iMax);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

                strText = (csisBase.bOK)? _T("OK") : _T("NG");
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

                double dFP1[] = { csisBase.drVu, csisBase.dVn };
                for(int k=0; k<2; k++)
                {
                    strText.Format(_T("%.3f"), dFP1[k] * m_dForcekN);
                    tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                }      

                strText = fabs(csisBase.drVu)<=csisBase.dShrSect ? _T("OK ") : _T("NG");
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

                strText = (fabs(csisBase.drVu) <= csisBase.dShrLoad)? _LS(IDS_TB_PSCD_CUMS_JTG_SHRLOAD_SKIP) : _LS(IDS_TB_PSCD_CUMS_JTG_SHRLOAD_CHECK);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

                tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;
                m_pRptData->AddTRData(tdK, tdData);
                tableD.arTRK.Add(tdK);
                iExistCount += 1;
            }
        }    
    }
    if(iExistCount > 0)
    {
        iChapID++;
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
    //6.6»ù´¡Ç¿¶ÈÑéËã
BOOL CRptJTG_D62_04_Seismic::MakeSeiemicFoundationCheckData(int& iChapID, T_PARA_RPT& paraD, BOOL bE1)
{
    int iExistCount = FALSE;
    paraD.Initialize();
    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");
    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;
    CArray<T_CHAR_K, T_CHAR_K> arCharK;
    // Table Header.
    thData.Initialize();  
    CString aHeadNames1[] = { _T("µ¥Ôª"), _T("Î»ÖÃ"), _T("×éºÏÃû³Æ"),_T("ÑéËã"), _T("ÖáÑ¹"),  _T("Fa"), _T("Æ«Ñ¹"),_T("¦ÃFa")};
    for(int i=0; i<8; i++)
    {
        if(i > 3)
        {
            thData.arTDK.Add(AddTDwithUnit(aHeadNames1[i], m_strStressUnit, 1, 1, 1));
        }
        else
        {
            thData.arTDK.Add(AddTDString(aHeadNames1[i], 1, 1, 1));
        }    
    }       
    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);
    CString strText = _T("");
    for(int i=0; i<m_iElemNum; i++)
    {
        T_FOUND_CH found; 
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFoundationData(m_arElemKeyList[i], found, bE1);        
        for(int j=0; j<2; j++)
        {
            tdData.Initialize();
            T_FOUND_CH_BASE dataBase = found.FoundBase[j];
            if(dataBase.bCHK)
            {
                strText.Format(_T("%d"), m_arElemKeyList[i]);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

                strText = (j<2)? _T("I") : _T("J");
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

                strText = GetLoadCombName(dataBase.LcomK);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

                strText = (dataBase.bOK)? _T("OK") : _T("NG");
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

                double dFP1[] = { dataBase.dPu1, dataBase.dPn1, dataBase.dPu2, dataBase.dPn2 };
                for(int k=0; k<4; k++)
                {
                    strText.Format(_T("%.3f"), dFP1[k]);
                    tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                }      

                tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;
                m_pRptData->AddTRData(tdK, tdData);
                tableD.arTRK.Add(tdK);
                iExistCount += 1;
            }
        }    
    }
    if(iExistCount > 0)
    {
        iChapID++;
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
	//6.7ÇÅ¶ÕËÜÐÔ½ÂÇø¿¹¼ôÇ¿¶È
BOOL CRptJTG_D62_04_Seismic::MakeSeiemicPierShearCheckData(int& iChapID, T_PARA_RPT& paraD)
{
    int iExistCount = FALSE;
    paraD.Initialize();
    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");
    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;
    CArray<T_CHAR_K, T_CHAR_K> arCharK;
    // Table Header.
    thData.Initialize();  
    CString aHeadNames1[] = { _LS(IDS_TB_PSCD_CUMS_JTG_ELEM), _LS(IDS_TB_PSCD_CUMS_JTG_PART), 
        _T("¼ôÁ¦·½Ïò"), _LS(IDS_TB_PSCD_CUMS_JTG_LOAD), 
        _LS(IDS_TB_PSCD_CUMS_JTG_OK), _T("Vc0"), _LS(IDS_TB_PSCD_CUMS_JTG_VN)};
    for(int i=0; i<7; i++)
    {
        if(i==5 || i==6)
        {
            thData.arTDK.Add(AddTDwithUnit(aHeadNames1[i], m_strForceUnit, 1, 1, 1));
        }
        else
        {
            thData.arTDK.Add(AddTDString(aHeadNames1[i], 1, 1, 1));
        }    
    }       
    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);
    CString strText = _T("");
    for(int i=0; i<m_iElemNum; i++)
    {
        T_CSIS_CH csis; 
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetPierShearData(m_arElemKeyList[i], csis);        
        for(int j=0; j<4; j++)
        {
            tdData.Initialize();
            T_CSIS_CH_BASE csisBase = csis.CsisBase[j];
            if(csisBase.bCHK)
            {
                strText.Format(_T("%d"), m_arElemKeyList[i]);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

                strText = (j<2)? _T("I") : _T("J");
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

                int index = (j%2==0)? 0 : 1;
                strText = (j%2==0) ? _T("Ë³ÇÅÏò") : _T("ºáÇÅÏò");//GetMaxMinType(index);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

                strText = GetLoadCombName(csisBase.LcomK);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

//        strText = GetLoadCombType(csisBase.iMax);
//        tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

                strText = (csisBase.bOK)? _T("OK") : _T("NG");
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

                double dFP1[] = { csisBase.drVu, csisBase.dVn };
                for(int k=0; k<2; k++)
                {
                    strText.Format(_T("%.3f"), dFP1[k] * m_dForcekN);
                    tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                }      

//        strText = fabs(csisBase.drVu)<=csisBase.dShrSect ? _T("OK ") : _T("NG");
//        tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

//        strText = (fabs(csisBase.drVu) <= csisBase.dShrLoad)? _LS(IDS_TB_PSCD_CUMS_JTG_SHRLOAD_SKIP) : _LS(IDS_TB_PSCD_CUMS_JTG_SHRLOAD_CHECK);
//        tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

                tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;
                m_pRptData->AddTRData(tdK, tdData);
                tableD.arTRK.Add(tdK);
                iExistCount += 1;
            }
        }    
    }
    if(iExistCount > 0)
    {
        iChapID++;
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

	//6.8Ö§×ùºñ¶È£¨°åÊ½Ïð½º£©
BOOL CRptJTG_D62_04_Seismic::MakeSeismicRubberThickCheckData(int& iChapID, T_PARA_RPT& paraD, BOOL bE1)
{
    int iExistCount = FALSE;
    paraD.Initialize();
    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");
    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;
    CArray<T_CHAR_K, T_CHAR_K> arCharK;
    // Table Header.
    thData.Initialize();  
    CString aHeadNames1[] = { _T("Ö§×ù±àºÅ"),_T("ÑéËã"),_T("Ë³ÇÅÏò±äÐÎ"),_T("ºáÇÅÏò±äÐÎ"),_T("Ïð½º²ã×Üºñ¶È")};
    for(int i=0; i<5; i++)
    {
        if(i>1)//µ¥Î»
        {
            thData.arTDK.Add(AddTDwithUnit(aHeadNames1[i], m_strLengthUnit, 1, 1, 1));//mm
        }
        else
        {
            thData.arTDK.Add(AddTDString(aHeadNames1[i], 1, 1, 1));
        }    
    }       
    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);
    CString strText = _T("");
    for(int i=0; i<m_iElasticLinkNum; i++)
    {
        T_SUPPORTDISP_CH dataBase; 
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetRubberThickData(m_arElasticLinkList[i], dataBase, bE1);        
        if(dataBase.bCHK)
		{
			tdData.Initialize();
			strText.Format(_T("%d"), dataBase.iKey);
			tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

            strText = (dataBase.bOK)? _T("OK") : _T("NG");
			tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

			double dFP1[] = {dataBase.dHoriDispX, dataBase.dHoriDispY, dataBase.dThick};				
			for(int k=0; k<3; k++)
			{
				strText.Format(_T("%.4f"), dFP1[k]);
				tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
			}      

			tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;
			m_pRptData->AddTRData(tdK, tdData);
			tableD.arTRK.Add(tdK);
			iExistCount += 1;
        }    
    }
    if(iExistCount > 0)
    {
        iChapID++;
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
	//6.9Ö§×ù¿¹»¬ÎÈ¶¨ÐÔ£¨°åÊ½Ïð½º£©
BOOL CRptJTG_D62_04_Seismic::MakeSeismicRubberSlipCheckData(int& iChapID, T_PARA_RPT& paraD, BOOL bE1)
{
    int iExistCount = FALSE;
    paraD.Initialize();
    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");
    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;
    CArray<T_CHAR_K, T_CHAR_K> arCharK;
    // Table Header.
    thData.Initialize();  
    CString aHeadNames1[] = { _T("Ö§×ù±àºÅ"), _T("ÑéËã"), _T("Ë³ÇÅÏòË®Æ½Á¦"),_T("ºáÇÅÏòË®Æ½Á¦"),_T("Ë®Æ½Á¦ÈÝÐíÖµ")};
    for(int i=0; i<5; i++)
    {
        if(i>1)//µ¥Î»
        {
            thData.arTDK.Add(AddTDwithUnit(aHeadNames1[i], m_strForceUnit, 1, 1, 1));
        }
        else
        {
            thData.arTDK.Add(AddTDString(aHeadNames1[i], 1, 1, 1));
        }    
    }       
    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);
    CString strText = _T("");
    for(int i=0; i<m_iElasticLinkNum; i++)
    {
        T_SUPPORTFORCE_CH dataBase; 
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetRubberSlipData(m_arElasticLinkList[i], dataBase, bE1);        
        
		tdData.Initialize();
        if(dataBase.bCHK)
        {
            strText.Format(_T("%d"), dataBase.iKey);
            tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

            strText = (dataBase.bOK)? _T("OK") : _T("NG");
            tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

            double dFP1[] = {dataBase.dHoriForceX, dataBase.dHoriForceY,dataBase.dAllowForce};				
            for(int k=0; k<3; k++)
            {
                strText.Format(_T("%.4f"), dFP1[k] * m_dForcekN);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
            }      

            tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;
            m_pRptData->AddTRData(tdK, tdData);
            tableD.arTRK.Add(tdK);
            iExistCount += 1;
        }    
    }
    if(iExistCount > 0)
    {
        iChapID++;
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
	//6.10Ö§×ùË®Æ½Î»ÒÆ£¨ÅèÊ½»î¶¯Ö§×ù£©
BOOL CRptJTG_D62_04_Seismic::MakeSeismicActiveHoriDispCheckData(int& iChapID, T_PARA_RPT& paraD, BOOL bE1)
{
    int iExistCount = FALSE;
    paraD.Initialize();
    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");
    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;
    CArray<T_CHAR_K, T_CHAR_K> arCharK;
    // Table Header.
    thData.Initialize();  
    CString aHeadNames1[] = { _T("Ö§×ù±àºÅ"),_T("ÑéËã"), _T("Ë³ÇÅÏòÎ»ÒÆ"),_T("ºáÇÅÏòÎ»ÒÆ"), _T("Ë®Æ½Î»ÒÆÔÊÐíÖµ")};
    for(int i=0; i<5; i++)
    {
        if(i>1)//µ¥Î»
        {
            thData.arTDK.Add(AddTDwithUnit(aHeadNames1[i], m_strLengthUnit, 1, 1, 1));
        }
        else
        {
            thData.arTDK.Add(AddTDString(aHeadNames1[i], 1, 1, 1));
        }    
    }       
    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);
    CString strText = _T("");
    for(int i=0; i<m_iGeneralLinkNum; i++)
    {
        T_SUPPORTDISP_CH dataBase; 
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetActiveHoriDispData(m_arGeneralLinkList[i], dataBase, bE1);        

		tdData.Initialize();		
		if(dataBase.bCHK)
        {
            strText.Format(_T("%d"), dataBase.iKey);
            tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
			
            strText = (dataBase.bOK)? _T("OK") : _T("NG");
            tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

            double dFP1[] = {dataBase.dHoriDispX, dataBase.dHoriDispY, dataBase.dThick};				
            for(int k=0; k<3; k++)
            {
                strText.Format(_T("%.4f"), dFP1[k]);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
            }      

            tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;
            m_pRptData->AddTRData(tdK, tdData);
            tableD.arTRK.Add(tdK);
            iExistCount += 1;    
        }    
    }
    if(iExistCount > 0)
    {
        iChapID++;
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
	//6.11Ö§×ùË®Æ½³ÐÔØÁ¦£¨ÅèÊ½¹Ì¶¨Ö§×ù£©
BOOL CRptJTG_D62_04_Seismic::MakeSeismicStatHoriForceCheckData(int& iChapID, T_PARA_RPT& paraD, BOOL bE1)
{
    int iExistCount = FALSE;
    paraD.Initialize();
    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");
    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;
    CArray<T_CHAR_K, T_CHAR_K> arCharK;
    // Table Header.
    thData.Initialize();  
    CString aHeadNames1[] = { _T("Ö§×ù±àºÅ"), _T("ÑéËã"), _T("Ë³ÇÅÏòË®Æ½Á¦"),_T("ºáÇÅÏòË®Æ½Á¦"), _T("Ë®Æ½Á¦ÈÝÐíÖµ")};
    for(int i=0; i<5; i++)
    {
        if(i>1)//µ¥Î»
        {
            thData.arTDK.Add(AddTDwithUnit(aHeadNames1[i], m_strForceUnit, 1, 1, 1));
        }
        else
        {
            thData.arTDK.Add(AddTDString(aHeadNames1[i], 1, 1, 1));
        }    
    }       
    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);
    CString strText = _T("");
    for(int i=0; i<m_iElasticLinkNum; i++)
    {
        T_SUPPORTFORCE_CH dataBase; 
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetStatHoriForceData(m_arElasticLinkList[i], dataBase, bE1);        

		tdData.Initialize();  
		if(dataBase.bCHK)
        {
            strText.Format(_T("%d"), dataBase.iKey);
            tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

            strText = (dataBase.bOK)? _T("OK") : _T("NG");
            tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

            double dFP1[] = {dataBase.dHoriForceX, dataBase.dHoriForceY, dataBase.dAllowForce};				
            for(int k=0; k<3; k++)
            {
                strText.Format(_T("%.4f"), dFP1[k] * m_dForcekN);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
            }      

            tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;
            m_pRptData->AddTRData(tdK, tdData);
            tableD.arTRK.Add(tdK);
            iExistCount += 1;
        }    
    }
    if(iExistCount > 0)
    {
        iChapID++;
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
	//6.12¶Õ¶¥Î»ÒÆ£¨¹æÔòÇÅÁº£©SFTD_JT
BOOL CRptJTG_D62_04_Seismic::MakeSeismicFrustaTopDispCheckData(int& iChapID, T_PARA_RPT& paraD)
{
    int iExistCount = FALSE;
    paraD.Initialize();
    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");
    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;
    CArray<T_CHAR_K, T_CHAR_K> arCharK;
    // Table Header.
    thData.Initialize();  
    CString aHeadNames1[] = { _T("¹¹¼þ"), _T("±äÐÎ·½Ïò"),  _T("ÑéËã"),  _T("¦¤d"),_T("¦¤u")};

    for(int i=0; i<5; i++)
    {
        if(i>2)//µ¥Î»
        {
            thData.arTDK.Add(AddTDwithUnit(aHeadNames1[i], m_strLengthUnit, 1, 1, 1));
        }
        else
        {
            thData.arTDK.Add(AddTDString(aHeadNames1[i], 1, 1, 1));
        }    
    }       
    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);
    CString strText = _T("");
    for(int i=0; i<m_iElemNum; i++)
    {
        T_FRUSTATOPDISP_CH data;
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFrustaTopDispData(m_arElemKeyList[i], data);        
        for(int j=0; j<2; j++)
        {
            tdData.Initialize();
            T_FRUSTATOPDISP_CH_BASE dataBase = data.DataBase[j];
            if(dataBase.bCHK)
            {
                strText.Format(_T("%d"), dataBase.iKey);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText = (dataBase.iDirect ==0)?  _T("Ë³ÇÅÏò(z)") : _T("ºáÇÅÏò(y)");
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

                strText = (dataBase.bOK)? _T("OK") : _T("NG");
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

                double dFP1[] = { dataBase.dHoriDisp,dataBase.dAlwDisp};				
                for(int k=0; k<2; k++)
                {
                    strText.Format(_T("%.4f"), dFP1[k]);
                    tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                } 

                tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;
                m_pRptData->AddTRData(tdK, tdData);
                tableD.arTRK.Add(tdK);
                iExistCount += 1;
            }
        }    
    }
    if(iExistCount > 0)
    {
        iChapID++;
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
	//6.13ËÜÐÎ×ª½ÇÎ»ÒÆ£¨·Ç¹æÔòÇÅÁº£©SCND_JT
BOOL CRptJTG_D62_04_Seismic::MakeSeismicCornerDispCheckData(int& iChapID, T_PARA_RPT& paraD)
{
    int iExistCount = FALSE;
    paraD.Initialize();
    T_TABLE_K tableK;
    T_TABLE_RPT tableD;
    tableD.Initialize();
    tableD.nCheck = 1;
    tableD.strTitle = _T("Table");
    T_TR_K thK, tdK;
    T_TR_RPT thData, tdData;
    CArray<T_CHAR_K, T_CHAR_K> arCharK;
    // Table Header.
    thData.Initialize();  
    CString aHeadNames1[] = {_T("¹¹¼þ"), _T("±äÐÎ·½Ïò"),  _T("ÑéËã"), _T("¦Èp"),_T("¦Èu")};

    for(int i=0; i<5; i++)
    {
		thData.arTDK.Add(AddTDString(aHeadNames1[i], 1, 1, 1));
    }       
    thData.CssK = m_pRptData->m_DefaultCss.THCssK;
    m_pRptData->AddTRData(thK, thData);
    tableD.arTHK.Add(thK);
    CString strText = _T("");
    for(int i=0; i<m_iElemNum; i++)
    {
        T_CORNERDISP_CH data;
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCornerDispData(m_arElemKeyList[i], data);        
        for(int j=0; j<2; j++)
        {
            tdData.Initialize();
            T_CORNERDISP_CH_BASE dataBase = data.DataBase[j];
            if(dataBase.bCHK)
            {
                strText.Format(_T("%d"), dataBase.iKey);
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

				strText = (dataBase.iDirect ==0)? _T("Ë³ÇÅÏò(z)") : _T("ºáÇÅÏò(y)");
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

                strText = (dataBase.bOK)? _T("OK") : _T("NG");
                tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));

//				strText = GetLoadCombName(dataBase.LcomK);
//        tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                double dFP1[] = { dataBase.dHoriDisp,dataBase.dAlwDisp};				
                for(int k=0; k<2; k++)
                {
                    strText.Format(_T("%.4f"), dFP1[k]);
                    tdData.arTDK.Add(AddTDString(strText, 0, 1, 1, m_TDCenterCssK));
                }      

                tdData.CssK = m_pRptData->m_DefaultCss.TDCssK;
                m_pRptData->AddTRData(tdK, tdData);
                tableD.arTRK.Add(tdK);
                iExistCount += 1;
            }
        }    
    }
    if(iExistCount > 0)
    {
        iChapID++;
        m_pRptData->AddTableData(tableK, tableD);
        paraD.arContentK.Add(tableK);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//***************************************************************
// Get Data.
//***************************************************************

// ÇöÀç Data ÆÄÀÏÀÇ ³¯Â¥¸¦ °¡Á®¿È.
CString CRptJTG_D62_04_Seismic::GetFileDate()
{ 
    CString strDate = _T("");
    CString strPath = m_pDoc->GetPathName();

    if(!strPath.IsEmpty())
    {
        CFileCtrl file(strPath);
        CFileStatus status;
        file.GetFileStatus(status);

        int iYY = status.m_mtime.GetYear();
        int iMM = status.m_mtime.GetMonth();

#if defined(_CH) || defined(_ORG)
        strDate = GetFileDateForChina(iYY, iMM);
#else
        strDate.Format(_LS(IDS_DGN_PSC_RPTJTG_Date), iYY, iMM);
#endif        
    }
    
    return strDate;
}

// Ãæ°Ý°è¼ö °ªÀ» °¡Á®¿È.
CString CRptJTG_D62_04_Seismic::GetImpactCoefficient()
{ 
    // Modify by GAY. MNET:2404. ('06.08.22). 
    CMvLoad mvLoad;

    CString strText = _T("");
    strText.Format(_T("%.3f"), mvLoad.CalcMovingImpactFactor());
        
    return strText;
}

// Beam Section Temperature LoadÀÇ ¹æÇâ¼º ±¸ÇÔ.
CString CRptJTG_D62_04_Seismic::GetBeamSectTempLoadDirection(int iDir)
{
    CString strText = _T("");
    if(iDir == 0)
    {
        strText = _LS(IDS_TB_BTMP_Local_y);
    }
    else if(iDir == 1)
    {
        strText = _LS(IDS_TB_BTMP_Local_z);
    } 
    else
    {
        strText = _LS(IDS_TB_BTMP_None);
    }
    return strText;
}

// ÇÏÁßÁ¶ÇÕÀÇ Active Type String Çü½ÄÀ¸·Î ±¸¼º.
CString CRptJTG_D62_04_Seismic::GetLcombActiveType(int iActiveType)
{
    CString strText = _T("");
    
    if(iActiveType == 1)
    {
        strText = _LS(IDS_DGN_PSC_RPTJTG_Lcom_StrengthStress);
    }
    else if(iActiveType == 2)
    {
        strText = _LS(IDS_DGN_PSC_RPTJTG_Lcom_Servicability);
    }
    else
    {
        strText = _T("Not Defined");
    }
    
    return strText;
}

// StageÀÇ Activation TypeÀ» String Çü½ÄÀ¸·Î º¯È¯.
CString CRptJTG_D62_04_Seismic::GetStageActivationType(int iCondition)
{
    CString strText = _T("");

    if(iCondition==1)       strText= _T("O");
    else if(iCondition==-1) strText= _T("X");
    else if(iCondition==0)  strText= _T("");

    return strText;
}

// StageÀÇ Total TypeÀ» String Çü½ÄÀ¸·Î º¯È¯.
CString CRptJTG_D62_04_Seismic::GetStageTotalType(int iSummary)
{
    return (iSummary == 1)? _T("O") : _T("");
}

// ¼³°è¿ë ÇÏÁßÁ¶ÇÕ Key¸¦ ÀÔ·Â¹Þ¾Æ ½ÇÁ¦ ÇÏÁßÁ¶ÇÕÀÇ ÀÌ¸§ ±¸ÇÔ.
CString CRptJTG_D62_04_Seismic::GetLoadCombName(T_LCOM_K lcomK)
{
    T_LCOM_K orglcomK;
    T_LCOM_D lcomD;
    m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCHRCOrgLcomK(lcomK, orglcomK);
    if(m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, orglcomK, lcomD))
    {
        return lcomD.LoadCombName;
    }
    else
    {
        return _T("-");
    }
}

// Static Load CaseÀÇ ÀÌ¸§ ±¸ÇÔ.
//   stldK : Static Load CaseÀÇ Key.
CString CRptJTG_D62_04_Seismic::GetStaticLoadCaseName(T_STLD_K stldK)
{
    T_STLD_D stldD;
    if(m_pDoc->m_pAttrCtrl->GetStld(stldK, stldD))
    {
        return stldD.LoadCaseName;
    }
    else
    {
        return _T("-");
    }
}


// ÇÏÁßÁ¶ÇÕÀÇ Type À» String Çü½ÄÀ¸·Î º¯È¯.
//   iMaxType : ÇÏÁßÁ¶ÇÕ Type (ÀÌµ¿ÇÏÁßÀÌ Æ÷ÇÔµÈ ÇÏÁßÁ¶ÇÕ¿¡¼­ ÃÖ´ë, ÃÖ¼Ò ºÎÀç·Â ¼ººÐÀ» Ãâ·Â).
CString CRptJTG_D62_04_Seismic::GetLoadCombType(int iMaxType)
{
    LPCTSTR aType[] = {_T("-"), _LS(IDS_TB_PSCD_FX_MAX), _LS(IDS_TB_PSCD_FX_MIN), _LS(IDS_TB_PSCD_FY_MAX), _LS(IDS_TB_PSCD_FY_MIN), 
                                         _LS(IDS_TB_PSCD_FZ_MAX), _LS(IDS_TB_PSCD_FZ_MIN), _LS(IDS_TB_PSCD_MX_MAX), _LS(IDS_TB_PSCD_MX_MIN),
                                         _LS(IDS_TB_PSCD_MY_MAX), _LS(IDS_TB_PSCD_MY_MIN), _LS(IDS_TB_PSCD_MZ_MAX), _LS(IDS_TB_PSCD_MZ_MIN)};

    return aType[iMaxType];
}


// À§Ä¡ÀÇ TypeÀ» String Çü½ÄÀ¸·Î º¯È¯.
CString CRptJTG_D62_04_Seismic::GetTopBottomType(int iLocType)
{
    LPCTSTR aType[] = { _LS(IDS_TB_PSCD_TOP), _LS(IDS_TB_PSCD_BOTTOM) };
    return (iLocType < 0 || iLocType >= 2)? _T("Not Exist") : aType[iLocType];
}


// Min/Max TypeÀ» String Çü½ÄÀ¸·Î º¯È¯.
CString CRptJTG_D62_04_Seismic::GetMaxMinType(int iMaxType)
{
    LPCTSTR aType[]= {_LS(IDS_TB_PSCD_Max), _LS(IDS_TB_PSCD_Min)};
    return (iMaxType < 0 || iMaxType >= 2)? _T("Not Exist") : aType[iMaxType];
}


// ºñÆ²¸² °ËÅä½Ã, Min/Max TypeÀ» String Çü½ÄÀ¸·Î º¯È¯.
//   iMaxType : ÃÖ´ë/ÃÖ¼Ò Type.
CString CRptJTG_D62_04_Seismic::GetMaxMinTypeShear(int iMaxType)
{
    LPCTSTR aType[]= {_LS(IDS_TB_PSCD_VMax), _LS(IDS_TB_PSCD_VMin),_LS(IDS_TB_PSCD_TMax)};
    return (iMaxType < 0 || iMaxType >= 3)? _T("Not Exist") : aType[iMaxType];
}

// ÇØ´ç stage¿¡¼­ÀÇ Element Group List¸¦ ¾òÀ½.
void CRptJTG_D62_04_Seismic::GetElementGroupList(T_STAG_K stagK, CArray<T_TEXT_K, T_TEXT_K>& arActiveK, CArray<T_TEXT_K, T_TEXT_K>& arDeactiveK)
{
    arActiveK.RemoveAll();
    arDeactiveK.RemoveAll();

    T_TEXT_K textK;
    T_TEXT_RPT textD; 

    T_STAG_D stagD;
    if(!m_pDoc->m_pAttrCtrl->GetStag(stagK, stagD)) return;

    int iCount = m_arGroupKeyList.GetSize();
    for(int j=0; j<iCount; j++)
    {
        T_GRUP_K grupK = m_arGroupKeyList[j];
        CString strGrupName = m_pDoc->m_pAttrCtrl->GetGrupName(grupK);
        
        int nActive = stagD.aActiveElem.GetSize();
        for(int k=0; k<nActive; k++)
        {
            if(stagD.aActiveElem[k].GrupKey == grupK)        
            {        
                textD.Initialize();
                textD.strText = strGrupName;
                m_pRptData->AddTextData(textK, textD);        
                arActiveK.Add(textK);
            }
        }
        int nDeactive = stagD.aDeactiveElem.GetSize();
        for(int k=0; k<nDeactive; k++)
        {
            if(stagD.aDeactiveElem[k].GrupKey == grupK) 
            {
                textD.Initialize();
                textD.strText = strGrupName;
                m_pRptData->AddTextData(textK, textD);        
                arDeactiveK.Add(textK);
            }
        }
    }

    if(arActiveK.GetSize() < 1)
    {
        textD.Initialize();
        textD.strText = _T("-");
        m_pRptData->AddTextData(textK, textD);        
        arActiveK.Add(textK);
    }

    if(arDeactiveK.GetSize() < 1)
    {
        textD.Initialize();
        textD.strText = _T("-");
        m_pRptData->AddTextData(textK, textD);        
        arDeactiveK.Add(textK);
    }
}


// ÇØ´ç stage¿¡¼­ÀÇ Boundary Group List¸¦ ¾òÀ½.
void CRptJTG_D62_04_Seismic::GetBoundaryGroupList(T_STAG_K stagK, CArray<T_TEXT_K, T_TEXT_K>& arActiveK, CArray<T_TEXT_K, T_TEXT_K>& arDeactiveK)
{
    arActiveK.RemoveAll();
    arDeactiveK.RemoveAll();

    T_TEXT_K textK;
    T_TEXT_RPT textD; 

    T_STAG_D stagD;
    if(!m_pDoc->m_pAttrCtrl->GetStag(stagK, stagD)) return;
    
    CArray<T_BNGR_K, T_BNGR_K> arBngrKeyList; arBngrKeyList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetBngrKeyList(arBngrKeyList);
    int iBngrCount = arBngrKeyList.GetSize();
    
    for(int i=0; i<iBngrCount; i++)
    {
        T_BNGR_K bngrK = arBngrKeyList[i];
        CString strGrupName = m_pDoc->m_pAttrCtrl->GetBngrName(bngrK);

        int nActive = stagD.aActiveBndr.GetSize();
        for(int k=0; k<nActive; k++)
        {
            if(stagD.aActiveBndr[k].BngrKey == bngrK)
            {
                textD.Initialize();
                textD.strText = strGrupName;
                m_pRptData->AddTextData(textK, textD);        
                arActiveK.Add(textK);
            }
        }

        int nDeactive = stagD.aDeactiveBndr.GetSize();
        for(int k=0; k<nDeactive; k++)
        {
            if(stagD.aDeactiveBndr[k]== bngrK)
            {
                textD.Initialize();
                textD.strText = strGrupName;
                m_pRptData->AddTextData(textK, textD);        
                arDeactiveK.Add(textK);
            }
        }
    }

    if(arActiveK.GetSize() < 1)
    {
        textD.Initialize();
        textD.strText = _T("-");
        m_pRptData->AddTextData(textK, textD);        
        arActiveK.Add(textK);
    }

    if(arDeactiveK.GetSize() < 1)
    {
        textD.Initialize();
        textD.strText = _T("-");
        m_pRptData->AddTextData(textK, textD);        
        arDeactiveK.Add(textK);
    }
}

// ÇØ´ç stage¿¡¼­ÀÇ Load Group List¸¦ ¾òÀ½.
void CRptJTG_D62_04_Seismic::GetLoadGroupList(T_STAG_K stagK, CArray<T_TEXT_K, T_TEXT_K>& arActiveK, CArray<T_TEXT_K, T_TEXT_K>& arDeactiveK)
{
    arActiveK.RemoveAll();
    arDeactiveK.RemoveAll();

    T_TEXT_K textK;
    T_TEXT_RPT textD; 

    T_STAG_D stagD;
    if(!m_pDoc->m_pAttrCtrl->GetStag(stagK, stagD)) return;
    
    CArray<T_BNGR_K, T_BNGR_K> arLdgrKeyList; arLdgrKeyList.RemoveAll();  
    m_pDoc->m_pAttrCtrl->GetLdgrKeyList(arLdgrKeyList);
    int iLdgrCount = arLdgrKeyList.GetSize();
    
    for(int i=0; i<iLdgrCount; i++)
    {
        T_LDGR_K ldgrK = arLdgrKeyList[i];
        CString strGrupName = m_pDoc->m_pAttrCtrl->GetLdgrName(ldgrK);

        int nActive = stagD.aActiveLoad.GetSize();
        for(int k=0; k<nActive; k++)
        {
            if(stagD.aActiveLoad[k].LdgrKey == ldgrK)
            {
                textD.Initialize();
                textD.strText = strGrupName;
                m_pRptData->AddTextData(textK, textD);        
                arActiveK.Add(textK);
            }
        }

        int nDeactive = stagD.aDeactiveLoad.GetSize();
        for(int k=0; k<nDeactive; k++)
        {
            if(stagD.aDeactiveLoad[k].LdgrKey == ldgrK)
            {
                textD.Initialize();
                textD.strText = strGrupName;
                m_pRptData->AddTextData(textK, textD);        
                arDeactiveK.Add(textK);
            }
        }
    }
    
    if(arActiveK.GetSize() < 1)
    {
        textD.Initialize();
        textD.strText = _T("-");
        m_pRptData->AddTextData(textK, textD);        
        arActiveK.Add(textK);
    }

    if(arDeactiveK.GetSize() < 1)
    {
        textD.Initialize();
        textD.strText = _T("-");
        m_pRptData->AddTextData(textK, textD);        
        arDeactiveK.Add(textK);
    }
}

// ÇØ´ç ¼ýÀÚ¸¦ Áß±¹ÇÑÀÚ·Î ¹Ù²Ù¾îÁÜ.
//   iNumber : ¹Ù²Ü ¼ýÀÚ.
CString CRptJTG_D62_04_Seismic::GetNumberCharacterForChina(int iNumber)
{
    CString result = _T("");
    switch(iNumber)
    {
    case 0:
        result = _T("Áã");
        break;
    case 1:
        result = _T("Ò»");
        break;
    case 2:
        result = _T("¶þ");
        break;
    case 3:
        result = _T("Èý");
        break;
    case 4:
        result = _T("ËÄ");
        break;
    case 5:
        result = _T("Îå");
        break;
    case 6:
        result = _T("Áù");
        break;
    case 7:
        result = _T("Æß");
        break;
    case 8:
        result = _T("°Ë");
        break;
    case 9:
        result = _T("¾Å");
        break;
    default:
        result = _T("-");
        break;
    }
    return result; 
}

// ÇöÀç Data ÆÄÀÏÀÇ ³¯Â¥ Ç¥±â. (Áß±¹Version).
//   iYY : ³âµµ.
//   iMM : ´Þ.
CString CRptJTG_D62_04_Seismic::GetFileDateForChina(int iYY, int iMM)
{  
    TCHAR i_str[32] = _T("");
    wsprintf(i_str, _T("%d"), iYY);

    CString result = _T("");  
    CString str = i_str;      
    for(int i=0; i<str.GetLength(); i++)
    {    
        CString strEach = str.GetAt(i);
        int index = _ttoi(strEach);
        result += GetNumberCharacterForChina(index);
    }
    result += _T("Äê ");

    wsprintf(i_str, _T("%d"), iMM);
    str = i_str;
    for(int i=0; i<str.GetLength(); i++)
    {
        CString strEach = str.GetAt(i);
        int index = _ttoi(strEach);
        result += GetNumberCharacterForChina(index);
    }
    result += _T("ÔÂ");
    
    return result;
}


// È°ÇÏÁß TypeÀÇ ¼³¸íÀ» °¡Á®¿È.
CString CRptJTG_D62_04_Seismic::GetLiveLoadTypeDescription(CString strTypeName)
{
    CString result = _T("");

    int index = -1;

    CString aTypeNames1[] = { _T("CH-CD"), _T("CH-CL"), _T("CH-RQ") };
    CString aTypeDecripts1[] = { _LS(IDS_DGN_PSC_RPTJTG_LiveType_CH_CD), 
        _LS(IDS_DGN_PSC_RPTJTG_LiveType_CH_CL),
        _LS(IDS_DGN_PSC_RPTJTG_LiveType_CH_RQ)};

    for(int i=0; i<3; i++)
    {
        if(strTypeName == aTypeNames1[i])
        {
            index = i;
            break;
        }
    }
    if(index > -1 && index < 3) return aTypeDecripts1[index];

    CString aTypeNames2[] = { _T("QC-10"), _T("QC-15"), _T("QC-20"), _T("QC-C20"), _T("LD-50"), _T("GC-80"), _T("GC-100"), _T("GC-120")};
    CString aTypeDescripts2[] = { _LS(IDS_DGN_PSC_RPTJTG_LiveType_QC_10),
        _LS(IDS_DGN_PSC_RPTJTG_LiveType_QC_15), _LS(IDS_DGN_PSC_RPTJTG_LiveType_QC_20),
        _LS(IDS_DGN_PSC_RPTJTG_LiveType_QC_C20), _LS(IDS_DGN_PSC_RPTJTG_LiveType_LD_50),
        _LS(IDS_DGN_PSC_RPTJTG_LiveType_GC_80), _LS(IDS_DGN_PSC_RPTJTG_LiveType_GC_100),
        _LS(IDS_DGN_PSC_RPTJTG_LiveType_GC_120)};

    for(int i=0; i<8; i++)
    {
        if(strTypeName == aTypeNames2[i])
        {
            index = i;
            break;
        }
    }
    if(index > -1 && index < 8) return aTypeDescripts2[index];

    CString aTypeNames3[] = { _T("C-AL"), _T("C-BL"), _T("C-AD(20)"), _T("C-BD(20)"), _T("C-AD(150)"), _T("C-BD(15)"), _T("CH-SL"), _T("CH-NL"), _T("CH-HL")};
    CString aTypeDescripts3[] = { _LS(IDS_DGN_PSC_RPTJTG_LiveType_C_AL), 
        _LS(IDS_DGN_PSC_RPTJTG_LiveType_C_BL), _LS(IDS_DGN_PSC_RPTJTG_LiveType_C_AD_20), 
        _LS(IDS_DGN_PSC_RPTJTG_LiveType_C_BD_20), _LS(IDS_DGN_PSC_RPTJTG_LiveType_C_AD_150), 
        _LS(IDS_DGN_PSC_RPTJTG_LiveType_C_BD_150), _LS(IDS_DGN_PSC_RPTJTG_LiveType_CH_SL),
        _LS(IDS_DGN_PSC_RPTJTG_LiveType_CH_NL), _LS(IDS_DGN_PSC_RPTJTG_LiveType_CH_HL) };

    for(int i=0; i<9; i++)
    {
        if(strTypeName == aTypeNames3[i])
        {
            index = i;
            break;
        }
    }
    if(index > -1 && index < 8) return aTypeDescripts3[index];

    return result;
}

// ÇÏÁßÁ¶ÇÕ½Ã, °¢ ÇÏÁßÁ¶ÇÕ¿¡ ¾²ÀÌ´Â ±âÈ£ÀÇ ¼³¸í.
CString CRptJTG_D62_04_Seismic::GetLoadCaseDescription(CString strCaseName)
{
    CString result = _T("");
    if     (strCaseName == _T("USER"))  result = _LS(IDS_DBLIB__ADDED__User_Defined_Load);
#if defined(_CIVIL)
    else if(strCaseName == _T("D"))     result = _LS(IDS_DBLIB__ADDED__Dead_Load);
#else
    else if(strCaseName == _T("D"))     result = _LS(IDS_DBLIB__ADDED__Dead_Load_Gen);
#endif
    else if(strCaseName == _T("DC"))    result = _LS(IDS_DBLIB__ADDED__Dead_Load_of_Component_and_Attachment);
    else if(strCaseName == _T("DW"))    result = _LS(IDS_DBLIB__ADDED__Dead_Load_of_Wearing_Surfaces_and_Utilities);
    else if(strCaseName == _T("DD"))    result = _LS(IDS_DBLIB__ADDED__Downdrag);
    else if(strCaseName == _T("LS"))    result = _LS(IDS_DBLIB__ADDED__Live_Load);
    else if(strCaseName == _T("LC"))    result = _LS(IDS_DBLIB__ADDED__Crawler_Induced_Live_Load);  
    else if(strCaseName == _T("EP"))	  result = _LS(IDS_DBLIB__ADDED__Earth_Pressure);
    else if(strCaseName == _T("EH"))    result = _LS(IDS_DBLIB__ADDED__Horizontal_Earth_Pressure);
    else if(strCaseName == _T("EV"))    result = _LS(IDS_DBLIB__ADDED__Vertical_Earth_Pressure);
    else if(strCaseName == _T("ES"))    result = _LS(IDS_DBLIB__ADDED__Earth_Surcharge_Load);
    else if(strCaseName == _T("EL"))    result = _LS(IDS_DBLIB__ADDED__Locked_in_Erection_Stresses);
#if defined(_CIVIL)
    else if(strCaseName == _T("L"))     result = _LS(IDS_DBLIB__ADDED__Live_Load);
#else
    else if(strCaseName == _T("L"))     result = _LS(IDS_DBLIB__ADDED__Live_Load_Gen);
#endif
    else if(strCaseName == _T("IL"))    result = _LS(IDS_DBLIB__ADDED__Live_Load_Impact);
    else if(strCaseName == _T("LC"))    result = _LS(IDS_DBLIB__ADDED__Crawler_Induced_Live_Load);
    else if(strCaseName == _T("LSC"))   result = _LS(IDS_DBLIB__ADDED__Crawler_Induced_Earth_Pressure);
    else if(strCaseName == _T("LS"))    result = _LS(IDS_DBLIB__ADDED__Live_Load_Surcharge);
    else if(strCaseName == _T("LP"))    result = _LS(IDS_DBLIB__ADDED__Live_Load_Overload);
    else if(strCaseName == _T("ILP"))   result = _LS(IDS_DBLIB__ADDED__Live_Load_Impact_Overload);
    else if(strCaseName == _T("W"))     result = _LS(IDS_DBLIB__ADDED__Wind_Load_on_Structure);
    else if(strCaseName == _T("WL"))    result = _LS(IDS_DBLIB__ADDED__Wind_Load_on_Live_Load);
    else if(strCaseName == _T("BRK"))   result = _LS(IDS_DBLIB__ADDED__Braking_Load);
    else if(strCaseName == _T("E"))     result = _LS(IDS_DBLIB__ADDED__Earthquake);
    else if(strCaseName == _T("CF"))    result = _LS(IDS_DBLIB__ADDED__Centrifugal_Force);
    else if(strCaseName == _T("CO"))    result = _LS(IDS_DBLIB__ADDED__Collision_Load);
    else if(strCaseName == _T("CT"))    result = _LS(IDS_DBLIB__ADDED__Vehicular_Collision_Force);
    else if(strCaseName == _T("CV"))    result = _LS(IDS_DBLIB__ADDED__Vessel_Collision_Force);
    else if(strCaseName == _T("STL"))   result = _LS(IDS_DBLIB__ADDED__Settlement);
    else if(strCaseName == _T("CR"))    result = _LS(IDS_DBLIB__ADDED__Creep);
    else if(strCaseName == _T("SH"))    result = _LS(IDS_DBLIB__ADDED__Shrinkage);
    else if(strCaseName == _T("T"))     result = _LS(IDS_DBLIB__ADDED__Temperature);
    else if(strCaseName == _T("TPG"))   result = _LS(IDS_DBLIB__ADDED__Temperature_Gradient);
    else if(strCaseName == _T("B"))     result = _LS(IDS_DBLIB__ADDED__Buoyancy);
    else if(strCaseName == _T("WP"))    result = _LS(IDS_DBLIB__ADDED__Ground_Water_Pressure);
    else if(strCaseName == _T("FP"))    result = _LS(IDS_DBLIB__ADDED__Fluid_Pressure);
    else if(strCaseName == _T("SF"))    result = _LS(IDS_DBLIB__ADDED__Stream_Flow_Pressure);
    else if(strCaseName == _T("WPR"))   result = _LS(IDS_DBLIB__ADDED__Wave_Pressure);
    else if(strCaseName == _T("PS"))    result = _LS(IDS_DBLIB__ADDED__Prestress);
    else if(strCaseName == _T("S"))     result = _LS(IDS_DBLIB__ADDED__Snow_Load);
    else if(strCaseName == _T("ER"))    result = _LS(IDS_DBLIB__ADDED__Erection_Load);
    else if(strCaseName == _T("CRL"))   result = _LS(IDS_DBLIB__ADDED__Crowd_Load);
    else if(strCaseName == _T("IP"))    result = _LS(IDS_DBLIB__ADDED__Ice_Pressure);
    else if(strCaseName == _T("FR"))    result = _LS(IDS_DBLIB__ADDED__Friction);
    else if(strCaseName == _T("RS"))    result = _LS(IDS_DBLIB__ADDED__Rib_Shortening);
    else if(strCaseName == _T("BK"))    result = _LS(IDS_DBLIB__ADDED__Longitudinal_Force_from_Live_Load);
    else if(strCaseName == _T("GE"))    result = _LS(IDS_DBLIB__ADDED__Grade_Effect);  
    else if (strCaseName == _T("LF"))    result = _LS(IDS_DBLIB__ADDED__LONGI_FORCE);
    else if (strCaseName == _T("RF"))    result = _LS(IDS_DBLIB__ADDED__RAKING_FORCE);
    else                            result = _T("Not Defined");

    return result;
}



// ÇÏÁßÁ¶ÇÕÀÇ ±âÈ£ ¼³¸í.
//   mapLcom : ÇÏÁßÁ¶ÇÕ ±âÈ£ ¹× ¼³¸íÀ» ´ã°í ÀÖ´Â Map.
void CRptJTG_D62_04_Seismic::GetLoadCombinationCases(CMap<CString, LPCTSTR, CString, CString&>& mapLcom)
{
    CString strDescript = _T("");
    CString strText     = _T("");

    // D_LCOM_STATIC : Static Load Case.
    CArray<T_STLD_K, T_STLD_K> arStldKeyList;
    m_pDoc->m_pAttrCtrl->GetStldKeyList(arStldKeyList);

    int iStld = arStldKeyList.GetSize();
    for(int i=0; i<iStld; i++)
    {
        T_STLD_D stld;
        if(m_pDoc->m_pAttrCtrl->GetStld(arStldKeyList[i], stld) && stld.LoadCaseType != _LSX(CS))
        {      
            mapLcom.SetAt(stld.LoadCaseType, stld.LoadCaseName);
        }
    } 

    //D_LCOM_SPECTRUM	: Response Spectrum Case.
    CArray<T_SPLC_K, T_SPLC_K> arSplcKeyList;
    m_pDoc->m_pAttrCtrl->GetSplcKeyList(arSplcKeyList);

    int iSplc = arSplcKeyList.GetSize();
    for(int i=0; i<iSplc; i++)
    {
        T_SPLC_D splcD;
        if(m_pDoc->m_pAttrCtrl->GetSplc(arSplcKeyList[i], splcD))
        {
            strText.Format(_T("RS[%d]"), splcD.LoadCaseId);
            mapLcom.SetAt(strText, splcD.LoadCaseName);
        }
    }

    //D_LCOM_HISTORY : Time Histroy Analysis.  
    CArray<T_THIS_K, T_THIS_K> arThisKeyList;
    m_pDoc->m_pAttrCtrl->GetThisKeyList(arThisKeyList);
    
    int iThis = arThisKeyList.GetSize();
    for (int i=0; i<iThis; i++)
    {
        T_THIS_D thisD;    
        if(m_pDoc->m_pAttrCtrl->GetThis(arThisKeyList[i], thisD))
        {      
            strText.Format(_T("TH[%d]"), thisD.LoadCaseId);
            mapLcom.SetAt(strText, thisD.LoadCaseName);
        }
    }
        
    //D_LCOM_MOVING : Moving Load Anlaysis.
    T_MVCD_D mvcdD;
    if(!m_pDoc->m_pAttrCtrl->GetMvcd(mvcdD)) mvcdD.Initialize();
    switch (mvcdD.nCodeType)
    {
    case D_MOVE_CODE_NONE:
        break;
    case D_MOVE_CODE_AASHTO_STAN:
    case D_MOVE_CODE_AASHTO_LRFD:
    case D_MOVE_CODE_TAIWAN:
    case D_MOVE_CODE_KOREA:
    case D_MOVE_CODE_CANADA:
    case D_MOVE_CODE_PENDOT:
    case D_MOVE_CODE_EURO_BS:
	case D_MOVE_CODE_RUSSIA:
    case D_MOVE_CODE_KOREA_LRFD_2011:
	case D_MOVE_CODE_AUSTRALIA:
	case D_MOVE_CODE_POLAND:
	case D_MOVE_CODE_SOUTH_AFRICA:
    case D_MOVE_CODE_NEWZEALAND:
    case D_MOVE_CODE_BRAZIL:
        {
            CArray<T_MVLD_K, T_MVLD_K> arMvldKeyList;
            m_pDoc->m_pAttrCtrl->GetMvldKeyList(arMvldKeyList);

            int iMvld = arMvldKeyList.GetSize();
            for(int i=0; i<iMvld; i++)
            {
                T_MVLD_D mvldD;
                if(m_pDoc->m_pAttrCtrl->GetMvld(arMvldKeyList[i], mvldD))
                {          
                    strText.Format(_T("M[%d]"), mvldD.LoadCaseId);
                    mapLcom.SetAt(strText, mvldD.LoadCaseName);
                }
            }
        }
        break;
    case D_MOVE_CODE_CHINA:
        {
            CArray<T_MVLDch_K, T_MVLDch_K> arMvldchKeyList;
            m_pDoc->m_pAttrCtrl->GetMvldchKeyList(arMvldchKeyList);

            int iMvldch = arMvldchKeyList.GetSize();
            for(int i=0; i<iMvldch; i++)
            {
                T_MVLDch_D mvldchD;
                if(m_pDoc->m_pAttrCtrl->GetMvldch(arMvldchKeyList[i], mvldchD))
                {
                    strText.Format(_T("M[%d]"), mvldchD.LoadCaseId);
                    mapLcom.SetAt(strText, mvldchD.LoadCaseName);
                }
            }
        }
        break;
    case D_MOVE_CODE_JAPAN:
        {
            CArray<T_MVLDjp_K, T_MVLDjp_K> arMvldjpKeyList;
            m_pDoc->m_pAttrCtrl->GetMvldjpKeyList(arMvldjpKeyList);

            int iMvldjp = arMvldjpKeyList.GetSize();
            for(int i=0; i<iMvldjp; i++)
            {
                T_MVLDjp_D mvldjpD;
                if(m_pDoc->m_pAttrCtrl->GetMvldjp(arMvldjpKeyList[i], mvldjpD))
                {
                    strText.Format(_T("M[%d]"), mvldjpD.LoadCaseId);
                    mapLcom.SetAt(strText, mvldjpD.LoadCaseName);
                }
            }
        }
        break;
    case D_MOVE_CODE_INDIA:
        {
            CArray<T_MVLDid_K, T_MVLDid_K> arMvldidKeyList;
            m_pDoc->m_pAttrCtrl->GetMvldidKeyList(arMvldidKeyList);

            int iMvldid = arMvldidKeyList.GetSize();
            for(int i=0; i<iMvldid; i++)
            {
                T_MVLDid_D mvldidD;
                if(m_pDoc->m_pAttrCtrl->GetMvldid(arMvldidKeyList[i], mvldidD))
                {
                    strText.Format(_T("M[%d]"), mvldidD.LoadCaseId);
                    mapLcom.SetAt(strText, mvldidD.LoadCaseName);
                }
            }
        }
        break;
    case D_MOVE_CODE_BS:
        {
            CArray<T_MVLDbs_K, T_MVLDbs_K> arMvldbsKeyList;
            m_pDoc->m_pAttrCtrl->GetMvldbsKeyList(arMvldbsKeyList);

            int iMvldbs = arMvldbsKeyList.GetSize();
            for(int i=0; i<iMvldbs; i++)
            {
                T_MVLDbs_D mvldbsD;
                if(m_pDoc->m_pAttrCtrl->GetMvldbs(arMvldbsKeyList[i], mvldbsD))
                {
                    strText.Format(_T("M[%d]"), mvldbsD.LoadCaseId);
                    mapLcom.SetAt(strText, mvldbsD.LoadCaseName);
                }
            }
        }
        break;
    case D_MOVE_CODE_FRANCE:
        {
            CArray<T_MVLDfr_K, T_MVLDfr_K> arMvldfrKeyList;
            m_pDoc->m_pAttrCtrl->GetMvldfrKeyList(arMvldfrKeyList);

            int iMvldfr = arMvldfrKeyList.GetSize();
            for(int i=0; i<iMvldfr; i++)
            {
                T_MVLDfr_D mvldfrD;
                if(m_pDoc->m_pAttrCtrl->GetMvldfr(arMvldfrKeyList[i], mvldfrD))
                {
                    strText.Format(_T("M[%d]"), mvldfrD.LoadCaseId);
                    mapLcom.SetAt(strText, mvldfrD.LoadCaseName);
                }
            }
        }
        break;
    case D_MOVE_CODE_TRANS:
        {
            CArray<T_MVLDtr_K, T_MVLDtr_K> arMvldtrKeyList;
            m_pDoc->m_pAttrCtrl->GetMvldtrKeyList(arMvldtrKeyList);

            int iMvldtr = arMvldtrKeyList.GetSize();
            for(int i=0; i<iMvldtr; i++)
            {
                T_MVLDtr_D mvldtrD;
                if(m_pDoc->m_pAttrCtrl->GetMvldtr(arMvldtrKeyList[i], mvldtrD))
                {
                    strText.Format(_T("M[%d]"), mvldtrD.LoadCaseId);
                    mapLcom.SetAt(strText, mvldtrD.LoadCaseName);
                }
            } 
        }
        break;
    default:
        ASSERT(0);
        break;
    } 

    //D_LCOM_SETTLEMENT	: Settlement Displacement.
    CArray<T_SMLC_K, T_SMLC_K> arSmlcKeyList;
    m_pDoc->m_pAttrCtrl->GetSmlcKeyList(arSmlcKeyList);

    int iSmlc = arSmlcKeyList.GetSize();
    for (int i=0; i<iSmlc; i++)
    {
        T_SMLC_D smlcD;
        if(m_pDoc->m_pAttrCtrl->GetSmlc(arSmlcKeyList[i], smlcD))
        {
            mapLcom.SetAt(_T("SM"), smlcD.LoadCaseName);
        }
    }
    
    //D_LCOM_STAGE : Construction Stage Analysis.
    CArray<T_SGLD_K, T_SGLD_K> arSgldKeyList;
    m_pDoc->m_pAttrCtrl->GetSgldKeyList(arSgldKeyList);
    
    int iSgld = arSgldKeyList.GetSize();  
    for (int i=0; i<iSgld; i++)
    {    
        T_SGLD_D sgldD;
        if(m_pDoc->m_pAttrCtrl->GetSgld(arSgldKeyList[i], sgldD))
        {      
            CString strText = GetConstructionStageLoadCaseSymbol(arSgldKeyList[i]);
            mapLcom.SetAt(strText, sgldD.LoadCaseName + _LSX((CS)));
        }    
    }

    //D_LCOM_ECCENSPEC : ¿ì¹ßÆí½É ÀÀ´ä ½ºÆåÆ®·³ ÇÏÁß Á¶°Ç
    CArray<T_ESPC_K, T_ESPC_K> arEspcKeyList;
    m_pDoc->m_pAttrCtrl->GetEspcKeyList(arEspcKeyList);

    int iEspc = arEspcKeyList.GetSize();  
    for(int i=0; i<iEspc; i++)
    {
        T_ESPC_D espcD;    
        if(m_pDoc->m_pAttrCtrl->GetEspc(arEspcKeyList[i], espcD))
        {
            mapLcom.SetAt(espcD.LoadCaseName, espcD.LoadCaseName + _LSX((ES)));    
        }    
    }

    //D_LCOM_GRIDMODEL  : °ÝÀÚ¸ðµ¨ ÇÏÁß Á¶°Ç    
    CArray<T_GILC_K, T_GILC_K> arGilcKeyList;
    m_pDoc->m_pAttrCtrl->GetGilcKeyList(arGilcKeyList);

    int iGlic = arGilcKeyList.GetSize();  
    for(int i=0; i<iGlic; i++)
    {
        T_GILC_D gilcD;
        if(m_pDoc->m_pAttrCtrl->GetGilc(arGilcKeyList[i], gilcD))
        {
            mapLcom.SetAt(gilcD.LoadCaseName, gilcD.LoadCaseName + _LSX((GI)));    
        }
    }  
}

// ½Ã°ø´Ü°è ÇÏÁß CaseÀÇ ±âÈ£ ¾ò¾î¿È.
CString CRptJTG_D62_04_Seismic::GetConstructionStageLoadCaseSymbol(T_SGLD_K sgldK)
{
	CString aSymbols[] = { _T("cD"), _T("cEL1"), _T("cEL2"), _T("cEL3"), _T("cEL4"), _T("cEL5"), _T("cEL6"), _T("cEL7"), _T("cEL8"), _T("cEL9"), _T("cEL10"), _T("cEL11"), _T("cEL12"), _T("cEL13"), _T("cEL14"), _T("cEL15"), _T("cEL16"), _T("cEL17"), _T("cEL18"), _T("cEL19"), _T("cEL20"),
					  _T("cTP"), _T("cTS"), _T("cCR"), _T("cCR"), _T("cSH"), _T("cSH"), _T("SUM") };
	return (sgldK >= D_SGLD_DL_CVL && sgldK <= D_SGLD_SUM_CVL)? aSymbols[sgldK-1] : _T("");
}

// ¿ÂµµÇÏÁß Á¶°ÇÀÌ Á¸ÀçÇÏ´Â°¡..  
BOOL CRptJTG_D62_04_Seismic::CheckExistTemperatureLoad()
{  
    int iTempCount = 0;
    CArray<T_STLD_K, T_STLD_K> arStldKeyList;
    m_pDoc->m_pAttrCtrl->GetStldKeyList(arStldKeyList);

    // System Temperature.
    T_STMP_K stmpK;
    T_STMP_D stmpD;
    int iCount = arStldKeyList.GetSize();
    for(int i=0; i<iCount; i++)
    {
        if(m_pDoc->m_pAttrCtrl->GetStmp(arStldKeyList[i], stmpK, stmpD))
        {
            iTempCount += 1;
        }
    }
    
    // Nodal Temperature.
    CArray<T_NODE_K, T_NODE_K> arNodeKeyList;
    m_pDoc->m_pAttrCtrl->GetNodeKeyList(arNodeKeyList);

    iCount = arNodeKeyList.GetSize();
    for(int i=0; i<iCount; i++)
    {
        CArray<T_NTMP_K, T_NTMP_K> arNtmpKeyList;
        CArray<T_NTMP_D, T_NTMP_D&> arNtmpDataList;
        
        m_pDoc->m_pAttrCtrl->GetNtmp(arNodeKeyList[i], arNtmpKeyList, arNtmpDataList);
        iTempCount += arNtmpKeyList.GetSize();    
    }

    // Element Temperature.
    CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
    m_pDoc->m_pAttrCtrl->GetElemKeyList(arElemKeyList);

    iCount = arElemKeyList.GetSize();
    for(int i=0; i<iCount; i++)
    {
        CArray<T_ETMP_K, T_ETMP_K> arEtmpKeyList;
        CArray<T_ETMP_D, T_ETMP_D&> arEtmpDataList;
        
        m_pDoc->m_pAttrCtrl->GetEtmp(arElemKeyList[i], arEtmpKeyList, arEtmpDataList);
        iTempCount += arEtmpKeyList.GetSize();

        CArray<T_GTMP_K, T_GTMP_K> arGtmpKeyList;
        CArray<T_GTMP_D, T_GTMP_D&> arGtmpDataList;
        
        m_pDoc->m_pAttrCtrl->GetGtmp(arElemKeyList[i], arGtmpKeyList, arGtmpDataList);
        iTempCount += arGtmpKeyList.GetSize();

        CArray<T_BTMP_K, T_BTMP_K> arBtmpKeyList;
        CArray<T_BTMP_D, T_BTMP_D&> arBtmpDataList;
        
        m_pDoc->m_pAttrCtrl->GetBtmp(arElemKeyList[i], arBtmpKeyList, arBtmpDataList);
        iTempCount += arBtmpKeyList.GetSize();
    }

    return (iTempCount > 0)? TRUE : FALSE;
}


BOOL CRptJTG_D62_04_Seismic::CheckExistResultTableItem()
{
    int iTableCount = 0;
    for(int i=0; i<12; i++)
    {
        if(m_SeismicD.bDesignTpye[i]) iTableCount += 1;
    }
    return (iTableCount > 0)? TRUE : FALSE;
}


//***************************************************************
// Some Helper Functions.
//***************************************************************

// T_CHAP_RPT Data »ý¼º.
T_CHAP_RPT CRptJTG_D62_04_Seismic::MakeChapter(int iLevel, int iChapID, CString strText, T_CSS_K cssK/*0*/)
{
    T_CHAP_RPT chapD;
    chapD.Initialize();
    chapD.nLevel = iLevel;
    chapD.SeqID  = iChapID;
    chapD.TitlK  = AddTitle(strText, iLevel, cssK);  
    return chapD;
}

BOOL CRptJTG_D62_04_Seismic::GetDesignStringPara(CStringArray& arrString)
{ 
    CString strText;
    //force
    strText = _T("3D"); // _T("3Â÷¿ø")// _T("2D or 3D")
    arrString.Add(strText);

 //ÇÅÐÍ
    strText = _T("ÆÕÍ¨»ìÄýÍÁÇÅÁº"); // _T("Fully PSC")
    arrString.Add(strText);
    
    // µµ·Î±³ÇÔ ¾ÈÀüµî±Þ
    if(m_SeismicD.iLoadType==0)
    {
        strText = _T("E1×÷ÓÃ"); // _T("1±Þ")
        arrString.Add(strText);
    }
    else if(m_SeismicD.iLoadType==1)
    {
        strText = _T("E2µ¯ÐÔ×÷ÓÃ"); //_T("2±Þ")
        arrString.Add(strText);
    }
    else if(m_SeismicD.iLoadType==2)
    {
        strText = _T("E2µ¯ËÜÐÔ×÷ÓÃ"); //_T("2±Þ")
        arrString.Add(strText);
    }
    else ASSERT(0);
    
    return TRUE;
}

BOOL CRptJTG_D62_04_Seismic::GetMaterialProperty()
{  
	if(m_arElemKeyList.GetSize()==0) return FALSE; // m_arElemK´Â PSC ¼³°è ´ë»ó ¿ä¼Ò¸¸. 
    m_arMatElem.RemoveAll();

    // Concrete, Rebar
    CString strHeadName; 	
    T_MATD_D MatdD;
    std::map<CString,T_ELEM_K> mapMatl2Elem;
    for(int i=0; i<m_arElemK.GetSize(); i++)
    {
        T_ELEM_K ElemK = m_arElemK.GetAt(i).first;
        MatdD.Initialize();
        if(!m_DataCtrl.Get_ConMatd(ElemK, MatdD))	continue;
     
        mapMatl2Elem[MatdD.Name] = ElemK;
    }
    
    for (std::map<CString,T_ELEM_K>::iterator pit = mapMatl2Elem.begin();pit != mapMatl2Elem.end();++pit)
    {
        m_arMatElem.Add(pit->second);
    }

    m_arTdnaK.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetTdnaKeyList(m_arTdnaK);

    m_arTdntK.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetTdntKeyList(m_arTdntK);

    return TRUE;
}

CString CRptJTG_D62_04_Seismic::GetModifyString(int iMaxLength, CString strText)
{
    CString str = strText;
    int iCurrLength = strText.GetLength();
    for(int i=0; i<iMaxLength-iCurrLength; i++)
        str += _T("¡¡"); 

    return str;
}

T_TD_K CRptJTG_D62_04_Seismic::AddTDwithUnit(CArray<T_CHAR_K, T_CHAR_K>& arCharK, CString strUnit, int iTDType/*0*/, int iMergeCols/*1*/, int iMergeRows/*1*/, T_CSS_K CssK/*0*/)
{
    CArray<T_TEXT_K, T_TEXT_K> arTextK; arTextK.RemoveAll();

    T_TEXT_K TextK;
    T_TEXT_RPT TextD; TextD.Initialize();
    for(int i=0; i<arCharK.GetSize(); i++) TextD.arCharK.Add(arCharK.GetAt(i));
    m_pRptData->AddTextData(TextK, TextD);
    arTextK.Add(TextK);

    TextD.Initialize();
    TextD.strText = strUnit;
    m_pRptData->AddTextData(TextK, TextD);
    arTextK.Add(TextK);

    return AddTD(arTextK, iTDType, iMergeCols, iMergeRows, CssK);
}

T_TD_K CRptJTG_D62_04_Seismic::AddTDwithUnit(CArray<T_CHAR_K, T_CHAR_K>& arCharK, CArray<T_CHAR_K, T_CHAR_K>& arCharUnitK, int iTDType/*0*/, int iMergeCols/*1*/, int iMergeRows/*1*/, T_CSS_K CssK/*0*/)
{
    CArray<T_TEXT_K, T_TEXT_K> arTextK; arTextK.RemoveAll();

    T_TEXT_K TextK;
    T_TEXT_RPT TextD; TextD.Initialize();
    for(int i=0; i<arCharK.GetSize(); i++) TextD.arCharK.Add(arCharK.GetAt(i));
    m_pRptData->AddTextData(TextK, TextD);
    arTextK.Add(TextK);

    TextD.Initialize();
    for(int i=0; i<arCharUnitK.GetSize(); i++) TextD.arCharK.Add(arCharUnitK.GetAt(i));
    m_pRptData->AddTextData(TextK, TextD);
    arTextK.Add(TextK);

    return AddTD(arTextK, iTDType, iMergeCols, iMergeRows, CssK);
}

T_TD_K CRptJTG_D62_04_Seismic::AddTDwithUnit(LPCTSTR strText, CString strUnit, int iTDType/*0*/, int iMergeCols/*1*/, int iMergeRows/*1*/, T_CSS_K CssK/*0*/)
{
    CArray<T_TEXT_K, T_TEXT_K> arTextK; arTextK.RemoveAll();

    T_TEXT_K TextK;
    T_TEXT_RPT TextD; TextD.Initialize();
    TextD.strText = strText;
    m_pRptData->AddTextData(TextK, TextD);
    arTextK.Add(TextK);

    TextD.Initialize();
    TextD.strText = strUnit;
    m_pRptData->AddTextData(TextK, TextD);
    arTextK.Add(TextK);

    return AddTD(arTextK, iTDType, iMergeCols, iMergeRows, CssK);
}

T_TD_K CRptJTG_D62_04_Seismic::AddTDwithUnit(LPCTSTR strText, CArray<T_CHAR_K, T_CHAR_K>& arCharUnitK, int iTDType/*0*/, int iMergeCols/*1*/, int iMergeRows/*1*/, T_CSS_K CssK/*0*/)
{
    CArray<T_TEXT_K, T_TEXT_K> arTextK; arTextK.RemoveAll();

    T_TEXT_K TextK;
    T_TEXT_RPT TextD; TextD.Initialize();
    TextD.strText = strText;
    m_pRptData->AddTextData(TextK, TextD);
    arTextK.Add(TextK);

    TextD.Initialize();
    for(int i=0; i<arCharUnitK.GetSize(); i++) TextD.arCharK.Add(arCharUnitK.GetAt(i));
    m_pRptData->AddTextData(TextK, TextD);
    arTextK.Add(TextK);

    return AddTD(arTextK, iTDType, iMergeCols, iMergeRows, CssK);
}

void CRptJTG_D62_04_Seismic::InitialarCharK()
{
    CString strText;
    
    m_arCharK_TndnComp.RemoveAll();
    m_arCharK_MnConR.RemoveAll();
    m_arCharK_MnConT.RemoveAll();
    m_arCharK_MnStl.RemoveAll();
    m_arCharK_MnPs.RemoveAll();

    m_arCharK_MnStlT.RemoveAll();
    m_arCharK_MnPsT.RemoveAll();
    m_arCharK_MnStlT2.RemoveAll();
    m_arCharK_MnPsT2.RemoveAll();
    m_arCharK_MnPs2.RemoveAll();
    
    // f'pd-¥ò'p0
    strText = _T("f'pd - ");
    AddChar(m_arCharK_TndnComp, strText, 2, 2, 2);
    strText = _T("¥ò'p0");
    AddChar(m_arCharK_TndnComp, strText, 2, 3, 2);  

    // Mn_Con Rect
    strText = _T("fcdbx(");
    AddChar(m_arCharK_MnConR, strText, 2, 1, 2);
    strText = _T("h0 - x/2)"); 
    AddChar(m_arCharK_MnConR, strText, 2, 1, 1);
    
    // Mn_Con T
    strText = _T("fcd");
    AddChar(m_arCharK_MnConT, strText, 2, 1, 2);
    strText = _T("[bx(h0 - x/2) + ("); 
    AddChar(m_arCharK_MnConT, strText, 2, 5, 1);
    strText = _T("b'f - b)");
    AddChar(m_arCharK_MnConT, strText, 2, 2, 1);
    strText = _T("h'f(");
    AddChar(m_arCharK_MnConT, strText, 2, 2, 1);
    strText = _T("h0 - ");
    AddChar(m_arCharK_MnConT, strText, 2, 1, 1);
    strText = _T("h'f/2 )]");
    AddChar(m_arCharK_MnConT, strText, 2, 2, 1);

    // Mn_stl
    strText = _T("f'sd");
    AddChar(m_arCharK_MnStl, strText, 2, 2, 2);
    strText = _T("A's");
    AddChar(m_arCharK_MnStl, strText, 2, 2, 1);
    strText = _T("(h0 - ");
    AddChar(m_arCharK_MnStl, strText, 2, 2, 1);
    strText = _T("a's)");
    AddChar(m_arCharK_MnStl, strText, 2, 2, 1);

    // Mn_ps
    strText = _T("(f'pd - ");
    AddChar(m_arCharK_MnPs, strText, 2, 3, 2);
    strText = _T("¥ò'p0)");
    AddChar(m_arCharK_MnPs, strText, 2, 3, 2);
    strText = _T("A'p ");
    AddChar(m_arCharK_MnPs, strText, 2, 2, 1);
    strText = _T("(h0 - ");
    AddChar(m_arCharK_MnPs, strText, 2, 2, 1);
    strText = _T("a'p)");
    AddChar(m_arCharK_MnPs, strText, 2, 2, 1);
    
    // Mn_psT
    strText = _T("fpd");
    AddChar(m_arCharK_MnPsT, strText, 2, 1, 2);
    strText = _T("Ap");
    AddChar(m_arCharK_MnPsT, strText, 2, 1, 1);
    strText = _T("(h - ap - a')");
    AddChar(m_arCharK_MnPsT, strText, 2, 6, 1);
    
    // Mn_stlT
    strText = _T("fsd");
    AddChar(m_arCharK_MnStlT, strText, 2, 1, 2);
    strText = _T("As");
    AddChar(m_arCharK_MnStlT, strText, 2, 1, 1);
    strText = _T("(h - as - a'");
    AddChar(m_arCharK_MnStlT, strText, 2, 6, 1);

    // Mn_psT2
    strText = _T("fpd");
    AddChar(m_arCharK_MnPsT2, strText, 2, 1, 2);
    strText = _T("Ap");
    AddChar(m_arCharK_MnPsT2, strText, 2, 1, 1);
    strText = _T("(h - ap - ");
    AddChar(m_arCharK_MnPsT2, strText, 2, 6, 1);
    strText = _T("a's)");
    AddChar(m_arCharK_MnPsT2, strText, 2, 2, 1);

    // Mn_stlT2
    strText = _T("fsd");
    AddChar(m_arCharK_MnStlT2, strText, 2, 1, 2);
    strText = _T("As");
    AddChar(m_arCharK_MnStlT2, strText, 2, 1, 1);
    strText = _T("(h - as - ");
    AddChar(m_arCharK_MnStlT2, strText, 2, 6, 1);
    strText = _T("a's");
    AddChar(m_arCharK_MnStlT2, strText, 2, 2, 1);

    // Mn_ps2
    strText = _T("(f'pd - ");
    AddChar(m_arCharK_MnPs2, strText, 2, 3, 2);
    strText = _T("¥ò'p0)");
    AddChar(m_arCharK_MnPs2, strText, 2, 3, 2);
    strText = _T("A'p ");
    AddChar(m_arCharK_MnPs2, strText, 2, 2, 1);
    strText = _T("(a'p - ");
    AddChar(m_arCharK_MnPs2, strText, 2, 3, 1);
    strText = _T("a's)");
    AddChar(m_arCharK_MnPs2, strText, 2, 2, 1);

    // Vn
    m_arCharK_Vn.RemoveAll();
    strText = _T("Vcs + ");
    AddChar(m_arCharK_Vn, strText, 2, 1, 2);
    strText = _T("Vsb + ");
    AddChar(m_arCharK_Vn, strText, 2, 1, 2);
    strText = _T("Vpb");
    AddChar(m_arCharK_Vn, strText, 2, 1, 2);

    m_arCharK_Vcs.RemoveAll();
    // Vcs
    strText = _T("Vcs = ");
    AddChar(m_arCharK_Vcs, strText, 2, 1, 2);
    strText = _T("¥á1");
    AddChar(m_arCharK_Vcs, strText, 2, 2, 1);
    strText = _T("¥á2");
    AddChar(m_arCharK_Vcs, strText, 2, 2, 1);
    strText = _T("¥á3");
    AddChar(m_arCharK_Vcs, strText, 2, 2, 1);
    strText = _T("(0.45¡¿10^ -3)");
    m_arCharK_Vcs.Add(AddChar(strText));
    strText = _T("bh0");
    AddChar(m_arCharK_Vcs, strText, 2, 2, 1);
    strText = _T("¡î((2+0.6P)¡î(");
    m_arCharK_Vcs.Add(AddChar(strText));
    strText = _T("fcu,k)");
    AddChar(m_arCharK_Vcs, strText, 2, 1, 4);
    strText = _T("¥ñsv");
    AddChar(m_arCharK_Vcs, strText, 2, 2, 2);
    strText = _T("fsv)");
    AddChar(m_arCharK_Vcs, strText, 2, 1, 2);

    m_arCharK_Vsb.RemoveAll();
    // Vsb
    strText = _T("Vsb = 0.75¡¿");
    AddChar(m_arCharK_Vsb, strText, 2, 1, 2);
    strText = _T("10-3");
    AddChar(m_arCharK_Vsb, strText, 1, 2, 2);
    strText = _T("fsd¥Ò");
    AddChar(m_arCharK_Vsb, strText, 2, 1, 2);
    strText = _T("Asb");
    AddChar(m_arCharK_Vsb, strText, 2, 1, 2);
    strText = _T("Sin¥Ès");
    AddChar(m_arCharK_Vsb, strText, 2, 5, 1);

    m_arCharK_Vpb.RemoveAll();
    // Vpb
    strText = _T("Vpb = 0.75¡¿");
    AddChar(m_arCharK_Vpb, strText, 2, 1, 2);
    strText = _T("10-3");
    AddChar(m_arCharK_Vpb, strText, 1, 2, 2);
    strText = _T("fpd¥Ò");
    AddChar(m_arCharK_Vpb, strText, 2, 1, 2);
    strText = _T("Apb");
    AddChar(m_arCharK_Vpb, strText, 2, 1, 2);
    strText = _T("Sin¥Èp");
    AddChar(m_arCharK_Vpb, strText, 2, 5, 1);

    // Sect Chk
    m_arCharK_SectChk.RemoveAll();
    strText = _T("0.51¡¿");
    m_arCharK_SectChk.Add(AddChar(strText));
    strText = _T("10-3¡î(");
    AddChar(m_arCharK_SectChk, strText, 1, 2, 2);
    strText = _T("fcu,k)");
    AddChar(m_arCharK_SectChk, strText, 2, 1, 4);
    strText =_T("bh0");
    AddChar(m_arCharK_SectChk, strText, 2, 2, 1); 

    // Load Chk
    m_arCharK_LoadChk.RemoveAll();
    strText = _T("0.50¡¿");
    m_arCharK_LoadChk.Add(AddChar(strText));
    strText = _T("10-3");
    AddChar(m_arCharK_LoadChk, strText, 1, 2, 2);
    strText = _T("¥á2");
    AddChar(m_arCharK_LoadChk, strText, 2, 2, 1);
    strText = _T("ftd");
    AddChar(m_arCharK_LoadChk, strText, 2, 1, 2);
    strText =_T("bh0");
    AddChar(m_arCharK_LoadChk, strText, 2, 2, 1); 

    // Pure Tn1
    m_arCharK_PureTn1.RemoveAll();
    strText = _T("0.35");
    m_arCharK_PureTn1.Add(AddChar(strText));
    strText = _T("¥âa");
    AddChar(m_arCharK_PureTn1, strText, 2, 2, 1);
    strText = _T("ftd");
    AddChar(m_arCharK_PureTn1, strText, 2, 1, 2);
    strText = _T("Wt + 1.2¡î(¥æ)");
    m_arCharK_PureTn1.Add(AddChar(strText));
    strText = _T("fsv");
    AddChar(m_arCharK_PureTn1, strText, 2, 1, 2);
    strText =_T("Asv1");
    AddChar(m_arCharK_PureTn1, strText, 2, 1, 3); 
    strText =_T("Acor/");
    AddChar(m_arCharK_PureTn1, strText, 2, 1, 3); 
    strText =_T("Sv");
    AddChar(m_arCharK_PureTn1, strText, 2, 1, 1); 

    // Pure Tn2
    m_arCharK_PureTn2.RemoveAll();
    strText = _T("0.35");
    m_arCharK_PureTn2.Add(AddChar(strText));
    strText = _T("¥âa");
    AddChar(m_arCharK_PureTn2, strText, 2, 2, 1);
    strText = _T("ftd");
    AddChar(m_arCharK_PureTn2, strText, 2, 1, 2);
    strText = _T("Wt + 1.2¡î(¥æ)");
    m_arCharK_PureTn2.Add(AddChar(strText));
    strText = _T("fsv");
    AddChar(m_arCharK_PureTn2, strText, 2, 1, 2);
    strText =_T("Asv1");
    AddChar(m_arCharK_PureTn2, strText, 2, 1, 3); 
    strText =_T("Acor/");
    AddChar(m_arCharK_PureTn2, strText, 2, 1, 3); 
    strText =_T("Sv + 0.05(");
    AddChar(m_arCharK_PureTn2, strText, 2, 1, 1); 
    strText = _T("Np0/");
    AddChar(m_arCharK_PureTn2, strText, 2, 1, 2);
    strText = _T("A0)");
    AddChar(m_arCharK_PureTn2, strText, 2, 1, 1);
    strText = _T("Wt");
    AddChar(m_arCharK_PureTn2, strText, 2, 1, 1);

    // Zeta
    m_arCharK_Zeta.RemoveAll();
    strText = _T("¥æ = ");
    m_arCharK_Zeta.Add(AddChar(strText));
    strText = _T("fsd");
    AddChar(m_arCharK_Zeta, strText, 2, 1, 2);
    strText =_T("Ast");
    AddChar(m_arCharK_Zeta, strText, 2, 1, 2); 
    strText =_T("Sv/(");
    AddChar(m_arCharK_Zeta, strText, 2, 1, 1); 
    strText = _T("fsv");
    AddChar(m_arCharK_Zeta, strText, 2, 1, 2);
    strText =_T("Asv1");
    AddChar(m_arCharK_Zeta, strText, 2, 1, 3); 
    strText =_T("Ucor)");
    AddChar(m_arCharK_Zeta, strText, 2, 1, 3); 

    // VTLoad
    m_arCharK_VTLoad.RemoveAll();
    strText = _T("¥ã0");
    AddChar(m_arCharK_VTLoad, strText, 2, 2, 1);
    strText =_T("Vd/(b");
    AddChar(m_arCharK_VTLoad, strText, 2, 1, 1); 
    strText =_T("h0) + ");
    AddChar(m_arCharK_VTLoad, strText, 2, 1, 1); 
    strText = _T("¥ã0");
    AddChar(m_arCharK_VTLoad, strText, 2, 2, 1);
    strText =_T("Td/(");
    AddChar(m_arCharK_VTLoad, strText, 2, 1, 1); 
    strText =_T("Wt)");
    AddChar(m_arCharK_VTLoad, strText, 2, 1, 1); 

    // TorSect Chk
    m_arCharK_TorSectChk.RemoveAll();
    strText = _T("0.51¡¿");
    m_arCharK_TorSectChk.Add(AddChar(strText));
    strText = _T("10-3¡î(");
    AddChar(m_arCharK_TorSectChk, strText, 1, 2, 2);
    strText = _T("fcu,k)");
    AddChar(m_arCharK_TorSectChk, strText, 2, 1, 4);

    // TorLoad Chk
    m_arCharK_TorLoadChk.RemoveAll();
    strText = _T("0.50¡¿");
    m_arCharK_TorLoadChk.Add(AddChar(strText));
    strText = _T("10-3");
    AddChar(m_arCharK_TorLoadChk, strText, 1, 2, 2);
    strText = _T("¥á2");
    AddChar(m_arCharK_TorLoadChk, strText, 2, 2, 1);
    strText = _T("ftd");
    AddChar(m_arCharK_TorLoadChk, strText, 2, 1, 2);

    // T_Vn
    m_arCharK_TorVn.RemoveAll();  
    strText = _T("¥á1");
    AddChar(m_arCharK_TorVn, strText, 2, 2, 1);
    strText = _T("¥á2");
    AddChar(m_arCharK_TorVn, strText, 2, 2, 1);
    strText = _T("¥á3");
    AddChar(m_arCharK_TorVn, strText, 2, 2, 1);
    strText = _T("((10-2");
    m_arCharK_TorVn.Add(AddChar(strText));
    strText = _T("¥ât)/20)");
    AddChar(m_arCharK_TorVn, strText, 2, 2, 1);
    strText = _T("bh0");
    AddChar(m_arCharK_TorVn, strText, 2, 2, 1);  
    strText = _T("¡î((2+0.6P)¡î(");
    m_arCharK_TorVn.Add(AddChar(strText));
    strText = _T("fcu,k)");
    AddChar(m_arCharK_TorVn, strText, 2, 1, 4);
    strText = _T("¥ñsv");
    AddChar(m_arCharK_TorVn, strText, 2, 2, 2);
    strText = _T("fsv)");
    AddChar(m_arCharK_TorVn, strText, 2, 1, 2);
    
    //  Tn
    m_arCharK_Tn.RemoveAll();
    strText = _T("¥ât(");
    AddChar(m_arCharK_Tn, strText, 2, 2, 1);
    strText = _T("0.35");
    m_arCharK_Tn.Add(AddChar(strText));
    strText = _T("¥âa");
    AddChar(m_arCharK_Tn, strText, 2, 2, 1);
    strText = _T("ftd + 0.05");
    AddChar(m_arCharK_Tn, strText, 2, 1, 2);
    strText = _T("Np0/");
    AddChar(m_arCharK_Tn, strText, 2, 1, 2);
    strText = _T("A0)");
    AddChar(m_arCharK_Tn, strText, 2, 1, 1);
    strText = _T("Wt + 1.2¡î(¥æ)");
    AddChar(m_arCharK_Tn, strText, 2, 1, 1);
    strText = _T("fsv");
    AddChar(m_arCharK_Tn, strText, 2, 1, 2);
    strText =_T("Asv1");
    AddChar(m_arCharK_Tn, strText, 2, 1, 3); 
    strText =_T("Acor/");
    AddChar(m_arCharK_Tn, strText, 2, 1, 3); 
    strText =_T("Sv");
    AddChar(m_arCharK_Tn, strText, 2, 1, 1); 

    //  Tn(¹Ú½ºÇü)
    m_arCharK_TnBox.RemoveAll();
    strText = _T("¥ât(");
    AddChar(m_arCharK_TnBox, strText, 2, 2, 1);
    strText = _T("0.35");
    m_arCharK_TnBox.Add(AddChar(strText));
    strText = _T("¥âa");
    AddChar(m_arCharK_TnBox, strText, 2, 2, 1);
    strText = _T("ftd + 0.05");
    AddChar(m_arCharK_TnBox, strText, 2, 1, 2);
    strText = _T("Np0/");
    AddChar(m_arCharK_TnBox, strText, 2, 1, 2);
    strText = _T("A0)");
    AddChar(m_arCharK_TnBox, strText, 2, 1, 1);
    strText = _T("¥âa");
    AddChar(m_arCharK_TnBox, strText, 2, 2, 1);
    strText = _T("Wt + 1.2¡î(¥æ)");
    AddChar(m_arCharK_TnBox, strText, 2, 1, 1);
    strText = _T("fsv");
    AddChar(m_arCharK_TnBox, strText, 2, 1, 2);
    strText =_T("Asv1");
    AddChar(m_arCharK_TnBox, strText, 2, 1, 3); 
    strText =_T("Acor/");
    AddChar(m_arCharK_TnBox, strText, 2, 1, 3); 
    strText =_T("Sv");
    AddChar(m_arCharK_TnBox, strText, 2, 1, 1); 
}
// 7.ÁºµÄÏêÏ¸¼ÆËã¹ý³Ì.
BOOL CRptJTG_D62_04_Seismic::MakeCheckReportResultData()
{
    CString strPathName=m_pDoc->GetPathName();
    CFileCtrl FileCtrl(strPathName);
    CString strInputFilePathName = FileCtrl.GetFilePathAndNameWithoutExtension()+_T("_seismic.mrpt");
    //++++++++++++++++++++++++++++++
    CFileFind finder;
    // start working for files
    BOOL bWorking = finder.FindFile(strInputFilePathName);
    if (!bWorking) {return TRUE;}
    finder.FindNextFile();
    int ilength = finder.GetLength(); 
    if (ilength <=0) {return TRUE; }
    //+++++++++++++++++++++++++++++++++++++++++++
    T_CHAP_K chap1K;
    T_CHAP_RPT chap1D;  chap1D.Initialize();  
    T_CHAP_K chap2K;
    T_CHAP_RPT chap2D;  chap2D.Initialize();
//  T_CHAP_K chap3K;
//  T_CHAP_RPT chap3D;  chap3D.Initialize();
    T_PARA_K paraK;
    T_PARA_RPT paraD;   paraD.Initialize();
    T_TEXT_K textK;
    T_TEXT_RPT textD;   textD.Initialize();
    int iSubChapIdx = 1;
    chap1D = MakeChapter(1, m_iChapNum++, _T("¿¹ÕðÏêÏ¸¼ÆËã¹ý³Ì"));
    //////////////////////////////////////////////////////////////////////////
    CString strline;
    wifstream OpenFile(strInputFilePathName); 
    while(!OpenFile.eof()) 
    { 
         TCHAR buf[1024] =  { 0 };
         OpenFile.getline(buf, sizeof(buf));
         strline = buf;

         if (strline.Left(7)== _T("**BEGIN") ) {
             // Start Chapter Level2-1.
             strline.Replace(_T("**BEGIN"),_T(""));
             chap2D = MakeChapter(2, iSubChapIdx++, strline);  
             paraD.Initialize();
             textD.Initialize();
             textD.CssK = m_pRptData->m_DefaultCss.TextCssK;  
         }else if (strline.Left(5)== _T("**END")) {
             chap2D.arParaK.Add(paraK);
             m_pRptData->AddChapData(chap2K, chap2D); 
             chap1D.arSubChapK.Add(chap2K);
         }else{
             strline.Replace(_T("  "),_T("&#160;&#160;"));
             strline.Replace(_T("<"),_T("£¼"));
             textD.strText = strline;
             m_pRptData->AddTextData(textK, textD);
             paraD.arContentK.Add(textK);
             m_pRptData->AddParaData(paraK, paraD);
         }
    } 
    OpenFile.close(); 
    m_pRptData->AddChapData(chap1K, chap1D);  
//  AddChapterToReport(chap1K);
        T_REPORT_RPT rptD; rptD.Initialize();
    m_pRptData->GetReportData(rptD);
    rptD.arChapK.Add(chap1K);
    m_pRptData->ModifyReportData(rptD);    
    return TRUE;
}
