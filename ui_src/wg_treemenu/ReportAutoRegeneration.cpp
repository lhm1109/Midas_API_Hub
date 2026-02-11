// ReportAutoRegeneration.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ReportAutoRegeneration.h"
#include "ReportTreeText.h"
#include "ReportProgressbarDlg.h"
#include "LoadWordLib.h"
#include "ReportDefine.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_FileCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\DBLib.h"


#include "..\wg_gps\GPSAMRUtil.h"
#include "..\wg_gps\GPSAMRUDCData.h"

#include "..\wg_dgn\DgnAmrSectionTb.h"
#include "..\wg_dgn\DgnDrawSection.h"

#include "TreeReportctrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportAutoRegeneration
CReportAutoRegeneration::CReportAutoRegeneration(CTreeReportCtrl* pReportTree /*= nullptr*/)
{
	m_aReportTextCaption.RemoveAll();
	m_aReportTextStrName.RemoveAll();

	// CTreeReportCtrl도 같이 수정할 것!
	// Workstree에 보여줄 용도로..
	//CTreeReportCtrl* pReportTree = CTreeReportCtrlInstance::GetTreeReportCtrlInstance();
	if (pReportTree)
	{
		pReportTree->GetReportTextCaptions(m_aReportTextCaption);
		pReportTree->GetReportTextStrNames(m_aReportTextStrName);
	}
	else
	{
		CTreeReportCtrl* ptrReportTree = CTreeReportCtrlInstance::GetTreeReportCtrlInstance();
		if (ptrReportTree)
		{
			ptrReportTree->GetReportTextCaptions(m_aReportTextCaption);
			ptrReportTree->GetReportTextStrNames(m_aReportTextStrName);
		}
		else
		{
#if defined(_MGEN)
			m_aReportTextCaption.RemoveAll();
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Control_Data));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Static_Loadcase));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Story));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Node));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Boundary));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Floor_Diaphragm_Rigid_Link));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Beam));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Truss));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Wall));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Weight_Volumn_Surface_area_of_all_member));
			m_aReportTextCaption.Add(_LS(IDS_AMR_Wind_Load));
			m_aReportTextCaption.Add(_LS(IDS_AMR_Seismic_Load));
			m_aReportTextCaption.Add(_LS(IDS_AMR_Load_Combination));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Self_Weight));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Nodal_Load));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Finishing_Material_Load));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_System_Temperature));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Nodal_Temperature));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Element_Temperature));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Beam_Section_Temperature));
			//m_aReportTextCaption.Add(_LS(IDS_AMR_SUMMARY_RESPONSE_SPECTRUM_LOAD_DATA));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Pushover_Global_Control));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Pushover_Load_Case));
			m_aReportTextCaption.FreeExtra();

			ASSERT(D_REPORT_TEXT_CNT == m_aReportTextCaption.GetSize());

			// 추출정보이름으로 사용. 문자열이 최대 28자까지 허용함.
			m_aReportTextStrName.RemoveAll();
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Control_Data));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Static_Loadcase));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Story));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Node));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Boundary));
			m_aReportTextStrName.Add(_LS(IDS_TM_DYNA_REPORT_Floor_Diaphragm_Rigid_Link)); // 기호 포함
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Beam));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Truss));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Wall));
			m_aReportTextStrName.Add(_LS(IDS_TM_DYNA_REPORT_Weight_Volume_Surface_area)); // 기호 포함
			m_aReportTextStrName.Add(_LS(IDS_AMR_Wind_Load));
			m_aReportTextStrName.Add(_LS(IDS_AMR_Seismic_Load));
			m_aReportTextStrName.Add(_LS(IDS_AMR_Load_Combination));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Self_Weight));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Nodal_Load));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Finishing_Material_Load));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_System_Temperature));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Nodal_Temperature));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Element_Temperature));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Beam_Section_Temperature));
			//m_aReportTextStrName.Add(_LS(IDS_TM_DYNA_REPORT_Response_Spectrum_Load)); // 문자열 길이가 28자 이상이므로 줄임.
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Pushover_Global_Control));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Pushover_Load_Case));
			m_aReportTextStrName.FreeExtra();

#elif(_CIVIL)
			m_aReportTextCaption.RemoveAll();
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Control_Data));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Static_Loadcase));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Node));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Boundary));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Beam));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Truss));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Weight_Volumn_Surface_area_of_all_member));
			m_aReportTextCaption.Add(_LS(IDS_AMR_Load_Combination));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Self_Weight));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Nodal_Load));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_System_Temperature));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Nodal_Temperature));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Element_Temperature));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Beam_Section_Temperature));
			//m_aReportTextCaption.Add(_LS(IDS_AMR_SUMMARY_RESPONSE_SPECTRUM_LOAD_DATA));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Pushover_Global_Control));
			m_aReportTextCaption.Add(_LS(IDS_AMR_COMMON_Pushover_Load_Case));
			m_aReportTextCaption.FreeExtra();

			ASSERT(D_REPORT_TEXT_CNT - 6 == m_aReportTextCaption.GetSize());

			// 추출정보이름으로 사용. 문자열이 최대 28자까지 허용함.
			m_aReportTextStrName.RemoveAll();
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Control_Data));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Static_Loadcase));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Node));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Boundary));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Beam));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Truss));
			m_aReportTextStrName.Add(_LS(IDS_TM_DYNA_REPORT_Weight_Volume_Surface_area)); // 기호 포함
			m_aReportTextStrName.Add(_LS(IDS_AMR_Load_Combination));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Self_Weight));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Nodal_Load));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_System_Temperature));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Nodal_Temperature));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Element_Temperature));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Beam_Section_Temperature));
			//m_aReportTextStrName.Add(_LS(IDS_TM_DYNA_REPORT_Response_Spectrum_Load)); // 문자열 길이가 28자 이상이므로 줄임.
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Pushover_Global_Control));
			m_aReportTextStrName.Add(_LS(IDS_AMR_COMMON_Pushover_Load_Case));
			m_aReportTextStrName.FreeExtra();
#else
			ASSERT(0);
#endif
		}
	}
}

CReportAutoRegeneration::~CReportAutoRegeneration()
{
}

void CReportAutoRegeneration::AutoRegeneration() 
{
#if defined(_CIVIL) && defined(_CH)   //china report
	AutoUpdateChinaReport();
	return;
#endif
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(!pDoc)
	{
		ASSERT(0);
		return;
	}

	CStringArray  aName    ; // 추출정보이름
	CStringArray  aFilePath; // *.bmp Full Path
	CStringArray  aType    ; // Report Item Type (IMG, TBL, CHT, TXT, IFL, TPL)
	CStringArray  aCaption ; // Caption
	CStringArray  aContents; // 내용
	aName    .RemoveAll();
	aFilePath.RemoveAll();
	aType    .RemoveAll();
	aCaption .RemoveAll();
	aContents.RemoveAll();

	CString strFilePath=_T(" ");

	// 현재 모델에 등록된 모든 UFIG Data 모으기

	/////////////////////////////////////////////////
	// Image
	T_UFIG_K  UfigK=0;
	T_UFIG_D  UfigD;  UfigD.Initialize();

	CArray<T_UFIG_K, T_UFIG_K> aUfigK;  
	aUfigK.RemoveAll();
	pDoc->m_pAttrCtrl->GetUfigKeyList(aUfigK);
	int nUfigCnt = aUfigK.GetSize();

	for(int nItem=0; nItem<nUfigCnt; nItem++)
	{
		UfigK = 0;
		UfigD.Initialize();
		strFilePath=_T("");

		UfigK = aUfigK[nItem];
		if(!pDoc->m_pAttrCtrl->GetUfig(UfigK, UfigD))
			continue;

		if(!pDoc->m_pPostCtrl->IsAnalysisResultEnable() && !UfigD.bPre && // 해석결과가 없고, 저장된 이미지가 후처리 이미지일때 
			UfigD.PostFrsiD.nCurrentMode>0)                                // 후처리 결과 이미지는 비활성
		{
			continue;
		}

		CString strTemp=_T("");
		strTemp = ReplaceSymbolByTempChar(UfigD.strName);
		aName    .Add(strTemp);
		aFilePath.Add(_T(" "));
		aCaption .Add(strTemp);
		aContents.Add(_T(" "));
		aType    .Add(_LSX(IMG));
	}

	/////////////////////////////////////////////////
	// Image File
	T_UIMG_K  UimgK=0;
	T_UIMG_D  UimgD;  UimgD.Initialize();

	CArray<T_UIMG_K, T_UIMG_K> aUimgK;  
	aUimgK.RemoveAll();
	pDoc->m_pAttrCtrl->GetUimgKeyList(aUimgK);
	int nUimgCnt = aUimgK.GetSize();

	CString strModelPath=_T("");
	strModelPath = CDBDoc::GetDocPoint()->GetPathName();

	CFileCtrl FileCtrl2(strModelPath);
	CString strFolder=_T("");
	strFolder = FileCtrl2.GetFilePath();

	for(int nItem=0; nItem<nUimgCnt; nItem++)
	{
		UimgK = 0;
		UimgD.Initialize();
		strFilePath=_T("");

		UimgK = aUimgK[nItem];
		if(!pDoc->m_pAttrCtrl->GetUimg(UimgK, UimgD))
			continue;

		strFilePath.Format(_LS(IDS_TM_DYNA_REPORT__s_Image_Files__s), strFolder, UimgD.strFilePath);
		ASSERT(strFilePath!=_T(""));
		CFileCtrl FileCtrl(strFilePath);
		if(!FileCtrl.FileExists(strFilePath))
		{
			ASSERT(0); continue;
		}

		CString strTemp=_T("");
		strTemp = ReplaceSymbolByTempChar(UimgD.strName);
		aName    .Add(strTemp);
		aFilePath.Add(_T(" "));
		aCaption .Add(strTemp);
		aContents.Add(_T(" "));
		aType    .Add(_LSX(IFL));
	}

	/////////////////////////////////////////////////
	// Table
	// User Defined Table
	T_UTBL_K  UtblK=0;
	T_UTBL_D  UtblD;  UtblD.Initialize();

	CArray<T_UTBL_K, T_UTBL_K> aUtblK;  
	aUtblK.RemoveAll();
	pDoc->m_pAttrCtrl->GetUtblKeyList(aUtblK);
	int nUtblCnt = aUtblK.GetSize();

	for(int nItem=0; nItem<nUtblCnt; nItem++)
	{
		UtblK = 0;
		UtblD.Initialize();
		strFilePath=_T("");

		UtblK = aUtblK[nItem];
		if(!pDoc->m_pAttrCtrl->GetUtbl(UtblK, UtblD))
			continue;

		// 후처리 테이블은 후처리 모드에서만 그리기
		if(UtblD.nTBMode==D_UTBL_TYPE_POST && !pDoc->IsPostMode())
			continue;

		if(UtblD.nTBMode==D_UTBL_TYPE_PO && !pDoc->m_pPostCtrl->IsPushOverResultEnable())
			continue;

		if(UtblD.nTBMode==D_UTBL_TYPE_PSC)
		{ 				
			CStringArray aPscCodeName;
			CDBLib::GetPscCodeNameList(aPscCodeName);
			if(aPscCodeName.GetSize()<=0)      
				continue;
		}

		CString strTemp=_T("");
		strTemp = ReplaceSymbolByTempChar(UtblD.strName);
		aName    .Add(strTemp);
		aFilePath.Add(_T(" "));//strFilePath);
		aCaption .Add(strTemp);
		aContents.Add(_T(" "));
		aType    .Add(_LSX(TBL));
	}

	// Special Tables - Section Summary
	T_SECT_K  SectK=0;
	T_SECT_D  SectD;  SectD.Initialize();

	CArray<T_SECT_K, T_SECT_K> aSectK;  
	aSectK.RemoveAll();
	pDoc->m_pAttrCtrl->GetSectKeyListUsedInElem(aSectK);
	int nSectCnt = aSectK.GetSize();

	T_RSCT_STIFF_D StiffInfo, AfterStiffInfo;
	int nType=0;  // section type

	for(int nItem=0; nItem<nSectCnt; nItem++)
	{
		SectK = 0;
		SectD.Initialize();
		strFilePath=_T("");

		SectK = aSectK[nItem];
		if(!pDoc->m_pAttrCtrl->GetSect(SectK, SectD))
			continue;

		if(!pDoc->m_pAttrCtrl->GetReportSectStiffInfo(SectD, StiffInfo, &AfterStiffInfo))
			continue;

		nType = StiffInfo.nType;
		if(nType<1 || nType>7)
		{
			ASSERT(0); continue;
		}

		CString strTemp=_T("");
		//aName.Add(ReplaceSymbolByTempChar(SectD.SName));
		strTemp.Format(_ULS('SS %d'), SectK);
		aName.Add(strTemp);
		aFilePath.Add(_T(" "));//strFilePath);
		aCaption .Add(strTemp);
		aContents.Add(_T(" "));
		aType    .Add(_LSX(TBL));
	}

	// Special Tables - Reinforcement of Sections
	aSectK.RemoveAll();
	pDoc->m_pAttrCtrl->GetRpscKeyList(aSectK);
	nSectCnt = aSectK.GetSize();

	BOOL bConsiderJ = FALSE;
	T_RSCT_RBAR_D  RpscInfoI, RpscInfoJ;

	for(int nItem=0; nItem<nSectCnt; nItem++)
	{
		SectK = 0;
		SectD.Initialize();
		strFilePath=_T("");

		SectK = aSectK[nItem];
		if(!pDoc->m_pAttrCtrl->GetSect(SectK, SectD))
			continue;

		if(!pDoc->m_pAttrCtrl->GetReportRpscInfo(SectK, bConsiderJ, RpscInfoI, RpscInfoJ))
			continue;

		CString strTemp=_T("");
		//aName.Add(ReplaceSymbolByTempChar(SectD.SName));
		strTemp.Format(_ULS('RS %d'), SectK);
		aName.Add(strTemp);
		aFilePath.Add(_T(" "));//strFilePath);
		aCaption .Add(strTemp);
		aContents.Add(_T(" "));
		aType    .Add(_LSX(TBL));
	}

	// Special Tables - Composite Section for C.S.
	CArray<T_CSCS_K, T_CSCS_K> aCscsK;
	pDoc->m_pAttrCtrl->GetCscsKeyList(aCscsK);
	int nCscsCnt = aCscsK.GetSize();
	T_CSCS_K CscsK=0;
	T_CSCS_D CscsD;
	T_STAG_K StagK=0;
	T_STAG_D StagD; StagD.Initialize();
	CArray<T_RSCT_CSCT_D, T_RSCT_CSCT_D&> aCscsInfo;
	aCscsInfo.RemoveAll();

	for(int nItem=0; nItem<nCscsCnt; nItem++)
	{
		CscsK = 0;
		CscsD.Initialize();
		strFilePath=_T("");

		CscsK = aCscsK[nItem];
		if(!pDoc->m_pAttrCtrl->GetCscs(CscsK, CscsD))
			continue;

		SectK = CscsD.SectKey;
		StagK = CscsD.ActStagKey;

		SectD.Initialize();
		if(!pDoc->m_pAttrCtrl->GetSect(SectK, SectD))
			continue;

		StagD.Initialize();
		if(!pDoc->m_pAttrCtrl->GetStag(StagK, StagD))
			continue;

		nType = pDoc->m_pAttrCtrl->GetReportCscsInfo(SectK, StagK, aCscsInfo);
		if(nType!=1 && nType!=2) continue;

		CString strTemp=_T("");
		//strTemp = ReplaceSymbolByTempChar(CscsD.SName);
		strTemp.Format(_ULS('CS %d'), CscsK);
		aName.Add(strTemp);
		aFilePath.Add(_T(" "));//strFilePath);
		aCaption .Add(strTemp);
		aContents.Add(_T(" "));
		aType    .Add(_LSX(TBL));
	}

	/////////////////////////////////////////////////
	// Chart
	T_UCHT_K  UchtK=0;
	T_UCHT_D  UchtD;  UchtD.Initialize(0);

	CArray<T_UCHT_K, T_UCHT_K> aUchtK;  
	aUchtK.RemoveAll();
	pDoc->m_pAttrCtrl->GetUchtKeyList(aUchtK);
	int nUchtCnt = aUchtK.GetSize();

	T_CHART_ITEM_D  ChartItemD; ChartItemD.Initialize();
	CGPSAMRUDCData  GenChart;

	for(int nItem=0; nItem<nUchtCnt; nItem++)
	{
		UchtK = 0;
		UchtD.Initialize(0);
		strFilePath=_T("");

		UchtK = aUchtK[nItem];
		if(!pDoc->m_pAttrCtrl->GetUcht(UchtK, UchtD))
			continue;

		// Pushover 결과 있는지 검사.
		if(!pDoc->m_pPostCtrl->IsPushOverResultEnable() && 
			( /*UchtD.nType==2 || */UchtD.nType==5 || UchtD.nType==7 || UchtD.nType==8 ))
			continue;

		ChartItemD.Initialize();
		if(!GenChart.GetData(UchtK, ChartItemD, strFilePath, 1))
			continue;

		CString strTemp=_T("");
		strTemp = ReplaceSymbolByTempChar(UchtD.strName);
		aName    .Add(strTemp);
		aFilePath.Add(_T(" "));//strFilePath);
		aCaption .Add(strTemp);
		aContents.Add(_T(" "));
		aType    .Add(_LSX(CHT));
	}

	/////////////////////////////////////////////////
	// Text
	CString strCaption=_T(" ");
	CString strText=_T(" ");
	CString strContents=_T(" ");

	CReportTreeText ReportTreeText;
	int nTextCnt=0;
#if defined(_MGEN)
	nTextCnt=D_REPORT_TEXT_CNT;
#elif(_CIVIL)
	nTextCnt=D_REPORT_TEXT_CNT-6;
#else
	ASSERT(0);
#endif


	for(int nItem=0; nItem<nTextCnt; nItem++)
	{
		strText = m_aReportTextStrName[nItem];

		if(IsUsedText(strText) != 1) continue;

		strCaption = m_aReportTextCaption[nItem];

		aName    .Add(strText);
		aFilePath.Add(_T(" "));
		aCaption .Add(strCaption);
		aContents.Add(strContents);
		aType    .Add(_LSX(TXT));
	}




	aName    .FreeExtra();
	aFilePath.FreeExtra();
	aType    .FreeExtra();
	aCaption .FreeExtra();
	aContents.FreeExtra();

	/////////////////////////////////////////////////
	// To Word

	// Make structure to export
	int nName = aName.GetSize();
	int nFilePath = aFilePath.GetSize();
	ASSERT(nName==nFilePath);
	int nItemCnt=0;
	nItemCnt = nName;

	CString	sProductInfo=_T(" "); // 제품정보
	CString	sCaption    =_T(" "); // Caption
	CString	sType       =_T(" "); // Type(image, table, image file, chart, text)
	CString	sFullPath   =_T(" "); // 파일경로 및 파일명(그림파일, 테이블Html파일)
	CString	sExtInfo    =_T(" "); // 추출정보
	CString	sContents   =_T(" "); // 내용(text일때 Word에 삽입할 내용)
	int		  iAppendFlag =0;   // 삽입삭제 구분 (삽입:0, 비교:1, 삭제:2, 제너레이션:9), default는 0
	CString	sExt        =_T(" "); // 향후를 대비한 예비 member. default는 NULL

	GEN_IFTAG* pIftag=NULL;
	pIftag = new GEN_IFTAG[nItemCnt];

#if defined(_CIVIL)
	sProductInfo  = _LSX(CIVIL);
#elif(_MGEN)
	sProductInfo  = _LSX(GEN);
#else
	ASSERT(0);
	sProductInfo  = _T(" ");
#endif

	iAppendFlag   = D_WORD_APPEND_COMPARE; 
	sExt          = _T(" ");

	CStringDeepCopyCollector SDCColl;
	for(int nItem=0; nItem<nItemCnt; nItem++)
	{
		sCaption      = aCaption[nItem];
		sType         = aType[nItem];
		sFullPath     = aFilePath[nItem];
		sContents     = aContents[nItem];
		sExtInfo      = aName[nItem];

		pIftag[nItem].sProductInfo = SDCColl.StringDeepCopy(sProductInfo);
		pIftag[nItem].sCaption     = SDCColl.StringDeepCopy(sCaption    );
		pIftag[nItem].sType        = SDCColl.StringDeepCopy(sType       );
		pIftag[nItem].sFullPath    = SDCColl.StringDeepCopy(sFullPath   );
		pIftag[nItem].sExtInfo     = SDCColl.StringDeepCopy(sExtInfo    );
		pIftag[nItem].sContents    = SDCColl.StringDeepCopy(sContents   );
		pIftag[nItem].sExt         = SDCColl.StringDeepCopy(sExt        );
		pIftag[nItem].iAppendFlag  = iAppendFlag;
	}

	int nRetCnt=0;
	GEN_IFTAG* pRetIftag=NULL;

	// Export to Word
	HINSTANCE hMod = CLoadWordLib::GetInstance()->GetWordLibrary();
	if(hMod!=NULL)	
	{
		// Word에서 선택한 항목 받아오기
		CWinApp* pApp = AfxGetApp();
		int nViewFlag = pApp->GetProfileInt(_LSX(DynaGen Report\\Auto Regeneration), _LSX(Auto Regeneration Item), 0);

		EditorImport pImport=NULL;
		pImport = (EditorImport) GetProcAddress(hMod, "EditorImport");
		int iRet = pImport(nItemCnt, (struct GEN_IFTAG**)&pIftag, nViewFlag, &nRetCnt, &pRetIftag);
		pImport=NULL;

		if(nRetCnt<=0 || iRet==-1) 
		{
			delete[] pIftag;
			return;
		}

		CStringArray aProductInfo ; aProductInfo.RemoveAll();
		CStringArray aType        ; aType.RemoveAll();
		CStringArray aExtInfo     ; aExtInfo.RemoveAll();

		for(int nItem=0; nItem<nRetCnt; nItem++)
		{
			sProductInfo = _T(" "); sType = _T(" "); sExtInfo = _T(" ");
			sProductInfo  = pRetIftag[nItem].sProductInfo;
			sType         = pRetIftag[nItem].sType;
			sExtInfo      = pRetIftag[nItem].sExtInfo;

			aProductInfo.Add(sProductInfo);
			aType.Add(sType);
			aExtInfo.Add(sExtInfo);
		}

		// Progress bar 추가하기
		//     CReportProgressbarDlg dlg;
		//     dlg.DoModal();

		// Word로 Regenerate할 항목 구성하기
		GEN_IFTAG* pRegIftag=NULL;
		pRegIftag = new GEN_IFTAG[nRetCnt];

		// (Unit Customizing) 현재단위 백업, 사용자 정의 단위계로 변경
		T_UNIT_INDEX UnitIndex_cur;
		pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex_cur);

		aName    .RemoveAll();
		aFilePath.RemoveAll();
		aContents.RemoveAll();

		CString strName=_T("");
		CString strType=_T("");
		strFilePath=_T("");
		strContents=_T("");

		CString strModelPath=_T("");
		strModelPath = CDBDoc::GetDocPoint()->GetPathName();

		CFileCtrl FileCtrl2(strModelPath);
		CString strFolder=_T("");
		strFolder = FileCtrl2.GetFilePath();

		CArray<BOOL, BOOL> aRepeat; aRepeat.RemoveAll();
		BOOL bRepeat = TRUE;

		for(int nItem=0; nItem<nRetCnt; nItem++)
		{
			strType.Format(_T("%s"), pRetIftag[nItem].sType);

			// User Defined Image
			if(_tcsicmp(strType, _LSX(IMG))==0)
			{
				strName.Format(_T("%s"), pRetIftag[nItem].sExtInfo);
				if(!pDoc->m_pAttrCtrl->ExistUfig(strName))
					continue;

				UfigK = pDoc->m_pAttrCtrl->GetUfigKey(strName);
				ASSERT(UfigK > 0);

				if(!CGPSAMRUtil::GetInstance()->GetMakeFigureAndPath(UfigK, strFilePath))
					continue;

				aName    .Add(strName);
				aFilePath.Add(strFilePath);
				aContents.Add(_T(" "));
			}
			// Image File
			else if(_tcsicmp(strType, _LSX(IFL))==0)
			{
				strName.Format(_T("%s"), pRetIftag[nItem].sExtInfo);
				if(!pDoc->m_pAttrCtrl->ExistUimg(strName))
					continue;

				T_UIMG_D UimgD; UimgD.Initialize();
				if(!pDoc->m_pAttrCtrl->GetUimg(strName, UimgD))
					continue;

				//strFilePath = UimgD.strFilePath;
				strFilePath.Format(_LS(IDS_TM_DYNA_REPORT__s_Image_Files__s), strFolder, UimgD.strFilePath);
				ASSERT(strFilePath!=_T(""));
				CFileCtrl FileCtrl(strFilePath);
				if(!FileCtrl.FileExists(strFilePath))
				{
					ASSERT(0); continue;
				}

				aName    .Add(strName);
				aFilePath.Add(strFilePath);
				aContents.Add(_T(" "));
			}
			// Table
			else if(_tcsicmp(strType, _LSX(TBL))==0)
			{
				strName.Format(_T("%s"), pRetIftag[nItem].sExtInfo);

				CString strTmp = strName;
				int r=-1;
				int nTableType=0;  // 1: User Defined Table, 2: Section Summary, 3: Reinforcement of Sections, 4: Composite Section for C.S.

				r = strTmp.Find(_ULS('SS ')); 
				if(r==0) 
				{
					nTableType = 2;
					strTmp.Replace(_ULS('SS '),_T(""));
				}

				if(r==-1)
				{
					r = strTmp.Find(_ULS('RS ')); 
					if(r==0) 
					{
						nTableType = 3;
						strTmp.Replace(_ULS('RS '),_T(""));
					}
				}

				if(r==-1)
				{
					r = strTmp.Find(_ULS('CS ')); 
					if(r==0) 
					{
						nTableType = 4;
						strTmp.Replace(_ULS('CS '),_T(""));
					}
				}

				if(r==-1)
				{
					nTableType = 1;
				}

				ASSERT(nTableType>0 && nTableType<5);

				CStringArray arImagePath;
				CDgnAmrSectionTb SectionTb;
				CString strImagePath=_T("");
				CDgnDrawSection DgnDrawSection;

				_DGN_DRAW_SECT_OPTION OptionD;
				OptionD.Initialize();
				OptionD.Canvas.SetRect(0,0,7500,7500);
				OptionD.DimSize = 500;

				// 1: User Defined Table
				if(nTableType==1) 
				{
					if(!pDoc->m_pAttrCtrl->ExistUtbl(strTmp))
						continue;

					UtblK = 0;
					UtblK = pDoc->m_pAttrCtrl->GetUtblKey(strTmp);

					if(UtblK <=0 )
						continue;

					// (Unit Customizing) Defined 단위계로 변경
					pDoc->m_pAttrCtrl->GetUtbl(UtblK, UtblD);
					if(UtblD.bDefinedUnit == TRUE)
					{
						T_UNIT_INDEX UnitIndex;
						UnitIndex.nBase_Length = UtblD.nUnitLength;
						UnitIndex.nBase_Force  = UtblD.nUnitForce ;
						UnitIndex.nBase_Heat   = UtblD.nUnitHeat 	;
						UnitIndex.nBase_Temper = UtblD.nUnitTemper;
						pDoc->m_pUnitCtrl->SetUnitIndexCurrentNew(UnitIndex);
					}

					// xml file name
					CString strFilePathName=_T("");
					CString strFullPath=_T("");
					//strFullPath = CDBDoc::GetDocPoint()->GetPathName();
					::GetTempPath(_MAX_PATH, strFullPath.GetBuffer(_MAX_PATH)); // 임시폴더에다 저장함. 20100429 KIMJM
					strFullPath.ReleaseBuffer();
					CFileCtrl FileCtrl(strFullPath);
					strFilePathName = FileCtrl.GetFilePathAndNameWithoutExtension();
					strFilePath.Format(_T("%s%s.xml"), strFilePathName, strTmp);

					bRepeat = TRUE;
					if(!pDoc->MakeReportTableXml(UtblK, strFilePath, bRepeat))
						continue;

					aRepeat.Add(bRepeat);
				}
				// 2: Section Summary
				else if(nTableType==2) 
				{
					T_RSCT_STIFF_D StiffInfo, AfterStiffInfo;
					int nType=0;  // section type

					SectK = _ttoi(strTmp);

					if(!pDoc->m_pAttrCtrl->ExistSect(SectK))
						continue;

					SectD.Initialize();
					if(!pDoc->m_pAttrCtrl->GetSect(SectK, SectD))
						continue;

					arImagePath.RemoveAll();

					if(!pDoc->m_pAttrCtrl->GetReportSectStiffInfo(SectD, StiffInfo, &AfterStiffInfo))
						continue;
					nType = StiffInfo.nType;

					// (Unit Customizing) Defined 단위계로 변경
					T_UNSP_D UnspD;
					if(pDoc->m_pAttrCtrl2->GetUnsp(1, UnspD))
					{
						if(UnspD.SectSumm.nUnitSys==1)
						{
							pDoc->m_pUnitCtrl->SetUnitIndexCurrentNew(UnspD.SectSumm.UnitIdx);
						}
					}

					// set file path name
					CString strFilePathName=_T("");
					strFilePathName = GetModelFullPathName();
					CString strSName=_T("");
					strSName = SectD.SName;
					FilterFileName(strSName);
					strImagePath.Format(_T("%s%s_%d.emf"), strFilePathName, strSName, SectK);
					strFilePath.Format(_T("%s%s_%d.xml"), strFilePathName, strSName, SectK);

					// make section image
					MakeSectionFigure(strImagePath, SectK, nType, arImagePath);

					if(!SectionTb.GenerateSectionTable(strFilePath, arImagePath, D_SECTION_TB_SS, SectK, 0))
						continue;

					CString strTemp = _T("");
					strTemp.Format(_T("%d : %s"), SectK, SectD.SName);

					pRetIftag[nItem].sCaption = SDCColl.StringDeepCopy(strTemp);
				}
				// 3: Reinforcement of Sections
				else if(nTableType==3) 
				{
					BOOL bConsiderJ = FALSE;
					T_RSCT_RBAR_D  RpscInfoI, RpscInfoJ;

					SectK = _ttoi(strTmp);

					if(!pDoc->m_pAttrCtrl->ExistSect(SectK))
						continue;

					SectD.Initialize();
					if(!pDoc->m_pAttrCtrl->GetSect(SectK, SectD))
						continue;

					if(!pDoc->m_pAttrCtrl->GetReportRpscInfo(SectK, bConsiderJ, RpscInfoI, RpscInfoJ))
						continue;

					// (Unit Customizing) Defined 단위계로 변경
					T_UNSP_D UnspD;
					if(pDoc->m_pAttrCtrl2->GetUnsp(1, UnspD))
					{
						if(UnspD.ReinForc.nUnitSys==1)
						{
							pDoc->m_pUnitCtrl->SetUnitIndexCurrentNew(UnspD.ReinForc.UnitIdx);
						}
					}

					// set file path name
					CString strFilePathName=_T("");
					strFilePathName = GetModelFullPathName();
					CString strSName=_T("");
					strSName = SectD.SName;
					FilterFileName(strSName);
					strImagePath.Format(_T("%s%s_%d.emf"), strFilePathName, strSName, SectK);
					strFilePath.Format(_T("%s%s_%d.xml"), strFilePathName, strSName, SectK);

					// make section image
					arImagePath.RemoveAll();
					if(bConsiderJ==FALSE)
					{
						DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 0/*int nIMJ*/, OptionD, FALSE, 1, FALSE);
						arImagePath.Add(strImagePath);
					}
					else if(bConsiderJ==TRUE)
					{
						DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 0, OptionD, FALSE, 1, FALSE);
						arImagePath.Add(strImagePath);
						DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 2, OptionD, FALSE, 1, FALSE);
						arImagePath.Add(strImagePath);
					}
					else
					{
						ASSERT(0); continue;
					}

					if(!SectionTb.GenerateSectionTable(strFilePath, arImagePath, D_SECTION_TB_RS, SectK, 0))
						continue;

					CString strTemp = _T("");
					strTemp.Format(_T("%dTTT : %s"), SectK, SectD.SName);

					pRetIftag[nItem].sCaption = SDCColl.StringDeepCopy(strTemp);
					
				}
				// 4: Composite Section for C.S.
				else if(nTableType==4) 
				{
					CArray<T_RSCT_CSCT_D, T_RSCT_CSCT_D&> aCscsInfo;
					int nType=0;  // section type

					T_CSCS_K CscsK=0;
					T_CSCS_D CscsD;
					T_STAG_K StagK=0;

					CscsK = _ttoi(strTmp);

					if(!pDoc->m_pAttrCtrl->ExistCscs(CscsK))
						continue;

					CscsD.Initialize();
					if(!pDoc->m_pAttrCtrl->GetCscs(CscsK, CscsD))
						continue;

					SectK = CscsD.SectKey;
					StagK = CscsD.ActStagKey;

					nType = pDoc->m_pAttrCtrl->GetReportCscsInfo(SectK, StagK, aCscsInfo);
					int nCscsNum = aCscsInfo.GetSize();

					// (Unit Customizing) Defined 단위계로 변경
					T_UNSP_D UnspD;
					if(pDoc->m_pAttrCtrl2->GetUnsp(1, UnspD))
					{
						if(UnspD.CompSect.nUnitSys==1)
						{
							pDoc->m_pUnitCtrl->SetUnitIndexCurrentNew(UnspD.CompSect.UnitIdx);
						}
					}

					// set file path name
					CString strFilePathName=_T("");
					strFilePathName = GetModelFullPathName();
					CString strSName=_T("");
					strSName = SectD.SName;
					FilterFileName(strSName);
					strImagePath.Format(_T("%s%s_%d.emf"), strFilePathName, strSName, SectK);
					strFilePath.Format(_T("%s%s_%d.xml"), strFilePathName, strSName, SectK);

					if(nType==1)
					{
						for(int i=0; i<nCscsNum; i++)
						{
							strImagePath.Format(_T("%s_%s_%d.emf"), strFilePathName, strSName, i);
							DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 0, OptionD, FALSE, 1, FALSE);
							arImagePath.Add(strImagePath);
						}
					}
					else if(nType==2)
					{
						arImagePath.RemoveAll();
					}
					else
					{
						ASSERT(0);
						continue;
					}

					if(!SectionTb.GenerateSectionTable(strFilePath, arImagePath, D_SECTION_TB_CS, SectK, StagK))
						continue;
				}

				aName    .Add(strName);
				aFilePath.Add(strFilePath);
				aContents.Add(_T(" "));
			}
			// Chart
			else if(_tcsicmp(strType, _LSX(CHT))==0)
			{
				strName.Format(_T("%s"), pRetIftag[nItem].sExtInfo);
				if(!pDoc->m_pAttrCtrl->ExistUcht(strName))
					continue;

				UchtK = 0;
				UchtK = pDoc->m_pAttrCtrl->GetUchtKey(strName);
				VERIFY(pDoc->m_pAttrCtrl->GetUcht(UchtK, UchtD));

				if(UchtK <=0 )
					continue;

				// (Unit Customizing) Defined 단위계로 변경
				if(UchtD.bDefinedUnit == TRUE)
				{
					T_UNIT_INDEX UnitIndex;
					UnitIndex.nBase_Length = UchtD.nUnitLength;
					UnitIndex.nBase_Force  = UchtD.nUnitForce ;
					UnitIndex.nBase_Heat   = UchtD.nUnitHeat 	;
					UnitIndex.nBase_Temper = UchtD.nUnitTemper;
					pDoc->m_pUnitCtrl->SetUnitIndexCurrentNew(UnitIndex);
				}

				ChartItemD.Initialize();
				if(!GenChart.GetData(UchtK, ChartItemD, strFilePath))
					continue;

				pDoc->MakeGraphAndBmpImage(ChartItemD, strFilePath, &UchtD); 

				aName    .Add(strName);
				aFilePath.Add(strFilePath);
				aContents.Add(_T(" "));
			}
			// Text
			else if(_tcsicmp(strType, _LSX(TXT))==0)
			{
				strName.Format(_T("%s"), pRetIftag[nItem].sExtInfo);
				strContents=_T(" ");

				if(!ReportTreeText.WriteReportText(strName, strContents))
					continue;



				aName    .Add(strName);
				aFilePath.Add(strFilePath);
				aContents.Add(strContents);
			}
			else
			{
				ASSERT(0); continue;
			}
			// (Unit Customizing) 백업해뒀던 현재 단위계 복원
			pDoc->m_pUnitCtrl->SetUnitIndexCurrentNew(UnitIndex_cur);
		}
		aName    .FreeExtra();
		aFilePath.FreeExtra();
		aContents.FreeExtra();

		nName=0;  nFilePath=0;
		nName = aName.GetSize();
		nFilePath = aFilePath.GetSize();
		ASSERT(nName==nFilePath);
		nItemCnt = nName;

		sCaption    =_T(""); // Caption
		strType     =_T("");
		sFullPath   =_T(""); // 파일경로 및 파일명(그림파일, 테이블Html파일)
		sContents   =_T(""); // 내용(text일때 Word에 삽입할 내용)

		// Word로 Regenerate할 항목 보내기
		CStringArray aCaption;  aCaption.SetSize(nItemCnt);
		CStringArray aType2;    aType2.SetSize(nItemCnt);
		CStringArray aFullPath; aFullPath.SetSize(nItemCnt);
		CStringArray aContents; aContents.SetSize(nItemCnt);
		CStringArray aExt;      aExt.SetSize(nItemCnt);
		for(int nItem=0; nItem<nItemCnt; nItem++)
		{
			aCaption[nItem].Format(_T("%s"), pRetIftag[nItem].sCaption);
			aCaption[nItem].Remove(' ');
			if(_tcsicmp(aCaption[nItem], _T(""))==0) aCaption[nItem]=_T(" ");
			else                                 aCaption[nItem].Format(_T("%s"), pRetIftag[nItem].sCaption);

			aType2[nItem].Format(_T("%s"), pRetIftag[nItem].sType);
			if(_tcsicmp(aType2[nItem], _LSX(IMG))==0 ||
				_tcsicmp(aType2[nItem], _LSX(TBL))==0 ||
				_tcsicmp(aType2[nItem], _LSX(TPL))==0 ||
				_tcsicmp(aType2[nItem], _LSX(CHT))==0 ||
				_tcsicmp(aType2[nItem], _LSX(IFL))==0 )
				aFullPath[nItem] = aFilePath[nItem];
			else
				aFullPath[nItem] = _T(" ");

			if(_tcsicmp(aType2[nItem], _LSX(TXT))==0)
				aContents[nItem]= aContents[nItem];
			else
				aContents[nItem] = _T(" ");

			if(nItem < aRepeat.GetSize())
			{
				aExt[nItem] = aRepeat[nItem]? _T(" ") : _T("0");
			}
			else
				aExt[nItem] = _T(" ");

			iAppendFlag = D_WORD_APPEND_REGENERATE;

			pRegIftag[nItem].sProductInfo = pRetIftag[nItem].sProductInfo;
			pRegIftag[nItem].sCaption     = (LPTSTR)(LPCTSTR)aCaption[nItem];
			pRegIftag[nItem].sType        = pRetIftag[nItem].sType       ;
			pRegIftag[nItem].sFullPath    = (LPTSTR)(LPCTSTR)aFullPath[nItem];
			pRegIftag[nItem].sExtInfo     = pRetIftag[nItem].sExtInfo    ;
			pRegIftag[nItem].sContents    = (LPTSTR)(LPCTSTR)aContents[nItem];
			pRegIftag[nItem].sExt         = (LPTSTR)(LPCTSTR)aExt[nItem];
			pRegIftag[nItem].iAppendFlag  = D_WORD_APPEND_REGENERATE;
		}

		EditorExport pExport=NULL;
		pExport = (EditorExport) GetProcAddress(hMod, "EditorExport");
		int nRet = pExport(nItemCnt, &pRegIftag);

		delete[] pRegIftag;

		// 임시파일 삭제
		CFileFind file;
		for(int nItem=0; nItem<nItemCnt; nItem++)
		{
			if(_tcsicmp(aType[nItem], _LSX(IFL))!=0 ) // Image File Item은 원본이 파일 경로이므로 삭제해서는 안된다. 20110331 KIMJM
			{
				sFullPath=_T("");
				sFullPath = aFilePath[nItem];
				if(file.FindFile(sFullPath))
					::DeleteFile(sFullPath);
			}
		}
		file.Close();
	}
	else
	{
		ASSERT(0);
		AfxMessageBox(_LS(IDS_TM_DYNA_REPORT_REPORT_ERROR));
	}

	delete[] pIftag;

	hMod = NULL;
}

CString CReportAutoRegeneration::ReplaceSymbolByTempChar(LPCTSTR lpStr)
{
	CString strTemp = lpStr;
	strTemp.Replace(_T("_"), _T(" ")); 
	//strTemp.Replace("\n", " "); 
	//   sParam.Replace("\\","_");// 
	strTemp.Replace(_T("~"),_T(" "));
	strTemp.Replace(_T("!"),_T(" "));
	strTemp.Replace(_T("@"),_T(" "));
	strTemp.Replace(_T("#"),_T(" "));
	strTemp.Replace(_T("$"),_T(" "));
	strTemp.Replace(_T("%"),_T(" "));
	strTemp.Replace(_T("^"),_T(" "));
	strTemp.Replace(_T("&"),_T(" "));
	strTemp.Replace(_T("*"),_T(" "));
	strTemp.Replace(_T("("),_T(" "));
	strTemp.Replace(_T(")"),_T(" "));
	strTemp.Replace(_T("-"),_T(" "));
	strTemp.Replace(_T("+"),_T(" "));
	strTemp.Replace(_T("="),_T(" "));
	strTemp.Replace(_T("\\"),_T(" "));
	strTemp.Replace(_T("|"),_T(" "));
	strTemp.Replace(_T("["),_T(" "));
	strTemp.Replace(_T("{"),_T(" "));
	strTemp.Replace(_T("]"),_T(" "));
	strTemp.Replace(_T("}"),_T(" "));
	strTemp.Replace(_T(":"),_T(" "));
	strTemp.Replace(_T(";"),_T(" "));
	strTemp.Replace(_T("\'"),_T(" "));
	strTemp.Replace(_T("\""),_T(" "));
	strTemp.Replace(_T("<"),_T(" "));
	strTemp.Replace(_T(","),_T(" "));
	strTemp.Replace(_T("."),_T(" "));
	strTemp.Replace(_T(">"),_T(" "));
	strTemp.Replace(_T("/"),_T(" "));
	strTemp.Replace(_T("?"),_T(" "));

	return strTemp; 
}

void CReportAutoRegeneration::BatchInsertWithoutEditor() 
{
	// 사용하지 않음.
	ASSERT(0);
	return;
}

void CReportAutoRegeneration::BatchOut() 
{
	// 사용하지 않음.
	ASSERT(0);
	return;
}

CString CReportAutoRegeneration::GetModelFullPathName()
{
	CString strFilePathName=_T("");
	CString strFullPath=_T("");

	//strFullPath = CDBDoc::GetDocPoint()->GetPathName();
	::GetTempPath(_MAX_PATH, strFullPath.GetBuffer(_MAX_PATH)); // 임시폴더에다 저장함. 20100429 KIMJM
	strFullPath.ReleaseBuffer();
	CFileCtrl FileCtrl(strFullPath);
	strFilePathName = FileCtrl.GetFilePathAndNameWithoutExtension();
	strFilePathName.Replace(_T("\\"), _T("\\\\"));

	return strFilePathName;
}

// Section Summary의 이미지 파일을 만든다.
void CReportAutoRegeneration::MakeSectionFigure(CString strImagePath, UINT SectK, int nType, CStringArray& arImagePath)
{
	arImagePath.RemoveAll();

	CDgnDrawSection DgnDrawSection;
	_DGN_DRAW_SECT_OPTION OptionD;
	OptionD.Initialize();
	OptionD.Canvas.SetRect(0,0,7500,7500);
	OptionD.DimSize = 500;

	if(nType==1 || nType==3 || nType==4)
	{
		DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 0/*int nIMJ*/, OptionD, FALSE, 1, FALSE);
		arImagePath.Add(strImagePath);
	}
	else if(nType==2)
	{
		strImagePath=_T("");
		arImagePath.RemoveAll();
	}
	else if(nType==5)
	{
		CString strTmp=_T(""), strTmp2=_T("");
		strTmp = strImagePath;
		strTmp.Replace(_T(".emf"), _T(""));
		strTmp2.Format(_T("%s_2.emf"), strTmp);

		DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 0, OptionD, FALSE, 1, FALSE);
		arImagePath.Add(strImagePath);
		DgnDrawSection.Make_SectionFig_For_SectK(strTmp2, SectK, 0, OptionD, FALSE, 1, TRUE);
		arImagePath.Add(strTmp2);
	}
	else if(nType==6)
	{
		CString strTmp=_T(""), strTmp2=_T("");
		strTmp = strImagePath;
		strTmp.Replace(_T(".emf"), _T(""));
		strTmp2.Format(_T("%s_2.emf"), strTmp);

		DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 0, OptionD, FALSE, 1, FALSE);
		arImagePath.Add(strImagePath);
		DgnDrawSection.Make_SectionFig_For_SectK(strTmp2, SectK, 2, OptionD, FALSE, 1, FALSE);
		arImagePath.Add(strTmp2);
	}
	else if(nType==7)
	{
		CString strTmp=_T(""), strTmp2=_T(""), strTmp3=_T(""), strTmp4=_T("");
		strTmp = strImagePath;
		strTmp.Replace(_T(".emf"), _T(""));
		strTmp2.Format(_T("%s_2.emf"), strTmp);
		strTmp3.Format(_T("%s_3.emf"), strTmp);
		strTmp4.Format(_T("%s_4.emf"), strTmp);

		DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 0, OptionD, FALSE, 1, FALSE);
		arImagePath.Add(strImagePath);
		DgnDrawSection.Make_SectionFig_For_SectK(strTmp2, SectK, 2, OptionD, FALSE, 1, FALSE);
		arImagePath.Add(strTmp2);
		DgnDrawSection.Make_SectionFig_For_SectK(strTmp3, SectK, 0, OptionD, FALSE, 1, TRUE);
		arImagePath.Add(strTmp3);
		DgnDrawSection.Make_SectionFig_For_SectK(strTmp4, SectK, 2, OptionD, FALSE, 1, TRUE);
		arImagePath.Add(strTmp4);
	}
	else
	{
		ASSERT(0);
	}
}

// 1:항목있음, 2:항목없음
int CReportAutoRegeneration::IsUsedText(CString strTextItem)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return FALSE;
	}

	int nRet=2;
	int nSize=0;

	if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Control_Data))==0)
	{
		nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Static_Loadcase))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountStld();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Story))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountStor();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Node))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountNode();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Boundary))==0)
	{
		int nCons = pDoc->m_pAttrCtrl->GetCountCons();  // support
		int nNspr = pDoc->m_pAttrCtrl->GetCountNspr();  // point spring support
		nSize = nCons + nNspr;
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_TM_DYNA_REPORT_Floor_Diaphragm_Rigid_Link))==0)
	{
		// floor diaphragm
		CArray<T_STOR_K, T_STOR_K> aStorK, aDStorK;
		T_STOR_D StorD; StorD.Initialize();
		pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);
		for (int i=aStorK.GetSize()-1; i>=0; i--)
		{
			pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD);
			if (StorD.bFloorDiaphragm) aDStorK.Add(aStorK[i]);
		}
		int nDiap = aDStorK.GetSize();

		// rigid link
		int nRigd = pDoc->m_pAttrCtrl->GetCountRigd();

		nSize = nDiap + nRigd;
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Beam))==0)
	{
		CArray<T_ELEM_K, T_ELEM_K> aElemK, aBeamK;
		T_ELEM_D ElemD; ElemD.Initialize();
		pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
		for (int i=0; i<aElemK.GetSize(); i++)
		{
			pDoc->m_pAttrCtrl->GetElem(aElemK[i], ElemD);
			if (pDoc->m_pAttrCtrl->IsBeam(ElemD.eltyp))
				aBeamK.Add(aElemK[i]);
		}
		nSize = aBeamK.GetSize();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Truss))==0)
	{
		CArray<T_ELEM_K, T_ELEM_K> aElemK, aTrusK;
		T_ELEM_D ElemD; ElemD.Initialize();
		pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
		for (int i=0; i<aElemK.GetSize(); i++)
		{
			pDoc->m_pAttrCtrl->GetElem(aElemK[i], ElemD);
			if (pDoc->m_pAttrCtrl->IsTruss(ElemD.eltyp))
				aTrusK.Add(aElemK[i]);
		}
		nSize = aTrusK.GetSize();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Wall))==0)
	{
		CArray<T_ELEM_K, T_ELEM_K> aElemK, aWallK;
		T_ELEM_D ElemD; ElemD.Initialize();
		pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
		for (int i=0; i<aElemK.GetSize(); i++)
		{
			pDoc->m_pAttrCtrl->GetElem(aElemK[i], ElemD);
			if (pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp))
				aWallK.Add(aElemK[i]);
		}
		nSize = aWallK.GetSize();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_TM_DYNA_REPORT_Weight_Volume_Surface_area))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountSect();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_Wind_Load))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountWind();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_Seismic_Load))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountSeis();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_Load_Combination))==0)
	{
		CArray<T_LCOM_K, T_LCOM_K> aGenLcomK, aStlLComK, aConLcomK, aSRCLcomK, aFdnLcomK, aAluLcomK, aScmLcomK;
		pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_GENERAL,  aGenLcomK);
		pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_STEEL,    aStlLComK);
		pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, aConLcomK);
		pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_SRC,      aSRCLcomK);
		pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_FDN,      aFdnLcomK);
		pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_ALUMINUM, aAluLcomK);
		pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_STLCOMP,  aScmLcomK);
		int nGenSize = aGenLcomK.GetSize();
		int nStlSize = aStlLComK.GetSize();
		int nConSize = aConLcomK.GetSize();
		int nSRCSize = aSRCLcomK.GetSize();
		int nFdnSize = aFdnLcomK.GetSize();
		int nAluSize = aAluLcomK.GetSize();
		int nScmSize = aScmLcomK.GetSize();
		nSize = nGenSize+nStlSize+nConSize+nSRCSize+nFdnSize+nAluSize+nScmSize; 
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Self_Weight))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountBodf();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Nodal_Load))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountCnld();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Finishing_Material_Load))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountFmld();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_System_Temperature))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountStmp();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Nodal_Temperature))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountNtmp();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Element_Temperature))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountEtmp();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Beam_Section_Temperature))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountBtmp();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_TM_DYNA_REPORT_Response_Spectrum_Load))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountSplc();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Pushover_Global_Control))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountPolc();
		if(nSize>0) nRet = 1;
	}
	else if(_tcsicmp(strTextItem, _LS(IDS_AMR_COMMON_Pushover_Load_Case))==0)
	{
		nSize = pDoc->m_pAttrCtrl->GetCountPolc();
		if(nSize>0) nRet = 1;
	}
	else
	{
#if defined(_CH) && defined(_CIVIL)
		{
			//       BOOL bfind = FALSE;
			//        for(int i=0;i<m_aReportTextCaption.GetSize();i++)
			//        {
			//          if(m_aReportTextCaption.GetAt(i) == strTextItem)
			//          {
			//            bfind = TRUE;
			//            break;
			//          }
			//        }
			//        if(bfind)
			//          nRet = 1;
			//        else
			//        {
			//           ASSERT(0); nRet = 2;
			//        }
			nRet = 1;
		}
#else
		ASSERT(0); nRet = 2;
#endif
	}

	return nRet;
}

// 파일 이름에 (\, /, :, *, ?, ", <, >, |)를 사용할 수 없으므로, 이러한 문자가 있을시에 없애줌.
void CReportAutoRegeneration::FilterFileName(CString& strFileName)
{
	strFileName.Replace(_T("\\"), _T(""));
	strFileName.Replace(_T("/"),_T(""));
	strFileName.Replace(_T(":"),_T(""));
	strFileName.Replace(_T("*"),_T(""));
	strFileName.Replace(_T("?"),_T(""));
	strFileName.Replace(_T("\""),_T(""));
	strFileName.Replace(_T("<"),_T(""));
	strFileName.Replace(_T(">"),_T(""));
	strFileName.Replace(_T("|"),_T(""));
}

void CReportAutoRegeneration::AutoGenerateChinaReport()
{
	CTreeReportCtrl* pReportTree = CTreeReportCtrlInstance::GetTreeReportCtrlInstance();
	if(pReportTree)
	{
		pReportTree->SetInsertState(TRUE);
		pReportTree->InsertChinaReportItemAll();
	}
}

void CReportAutoRegeneration::AutoUpdateChinaReport()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(!pDoc)
	{
		ASSERT(0);
		return;
	}

	CStringArray  aName    ; // 추출정보이름
	CStringArray  aFilePath; // *.bmp Full Path
	CStringArray  aType    ; // Report Item Type (IMG, TBL, CHT, TXT, IFL, TPL)
	CStringArray  aCaption ; // Caption
	CStringArray  aContents; // 내용
	aName    .RemoveAll();
	aFilePath.RemoveAll();
	aType    .RemoveAll();
	aCaption .RemoveAll();
	aContents.RemoveAll();

	CString strFilePath=_T(" ");

	// 현재 모델에 등록된 모든 UFIG Data 모으기

	/////////////////////////////////////////////////
	// Image
	T_UFIG_K  UfigK=0;
	T_UFIG_D  UfigD;  UfigD.Initialize();

	CArray<T_UFIG_K, T_UFIG_K> aUfigK;  
	aUfigK.RemoveAll();
	pDoc->m_pAttrCtrl->GetUfigKeyList(aUfigK);
	int nUfigCnt = aUfigK.GetSize();

	for(int nItem=0; nItem<nUfigCnt; nItem++)
	{
		UfigK = 0;
		UfigD.Initialize();
		strFilePath=_T("");

		UfigK = aUfigK[nItem];
		if(!pDoc->m_pAttrCtrl->GetUfig(UfigK, UfigD))
			continue;

		if(!pDoc->m_pPostCtrl->IsAnalysisResultEnable() && !UfigD.bPre && // 해석결과가 없고, 저장된 이미지가 후처리 이미지일때 
			UfigD.PostFrsiD.nCurrentMode>0)                                // 후처리 결과 이미지는 비활성
		{
			continue;
		}

		CString strTemp=_T("");
		strTemp = ReplaceSymbolByTempChar(UfigD.strName);
		aName    .Add(strTemp);
		aFilePath.Add(_T(" "));
		aCaption .Add(strTemp);
		aContents.Add(_T(" "));
		aType    .Add(_LSX(IMG));
	}

	/////////////////////////////////////////////////
	// Image File
	T_UIMG_K  UimgK=0;
	T_UIMG_D  UimgD;  UimgD.Initialize();

	CArray<T_UIMG_K, T_UIMG_K> aUimgK;  
	aUimgK.RemoveAll();
	pDoc->m_pAttrCtrl->GetUimgKeyList(aUimgK);
	int nUimgCnt = aUimgK.GetSize();

	CString strModelPath=_T("");
	strModelPath = CDBDoc::GetDocPoint()->GetPathName();

	CFileCtrl FileCtrl2(strModelPath);
	CString strFolder=_T("");
	strFolder = FileCtrl2.GetFilePath();

	for(int nItem=0; nItem<nUimgCnt; nItem++)
	{
		UimgK = 0;
		UimgD.Initialize();
		strFilePath=_T("");

		UimgK = aUimgK[nItem];
		if(!pDoc->m_pAttrCtrl->GetUimg(UimgK, UimgD))
			continue;

		strFilePath.Format(_LS(IDS_TM_DYNA_REPORT__s_Image_Files__s), strFolder, UimgD.strFilePath);
		ASSERT(strFilePath!=_T(""));
		CFileCtrl FileCtrl(strFilePath);
		if(!FileCtrl.FileExists(strFilePath))
		{
			ASSERT(0); continue;
		}

		CString strTemp=_T("");
		strTemp = ReplaceSymbolByTempChar(UimgD.strName);
		aName    .Add(strTemp);
		aFilePath.Add(_T(" "));
		aCaption .Add(strTemp);
		aContents.Add(_T(" "));
		aType    .Add(_LSX(IFL));
	}

	/////////////////////////////////////////////////
	// Table
	// User Defined Table
	T_UTBL_K  UtblK=0;
	T_UTBL_D  UtblD;  UtblD.Initialize();

	CArray<T_UTBL_K, T_UTBL_K> aUtblK;  
	aUtblK.RemoveAll();
	pDoc->m_pAttrCtrl->GetUtblKeyList(aUtblK);
	int nUtblCnt = aUtblK.GetSize();

	for(int nItem=0; nItem<nUtblCnt; nItem++)
	{
		UtblK = 0;
		UtblD.Initialize();
		strFilePath=_T("");

		UtblK = aUtblK[nItem];
		if(!pDoc->m_pAttrCtrl->GetUtbl(UtblK, UtblD))
			continue;

		// 후처리 테이블은 후처리 모드에서만 그리기
		if(UtblD.nTBMode==D_UTBL_TYPE_POST && !pDoc->IsPostMode())
			continue;

		if(UtblD.nTBMode==D_UTBL_TYPE_PO && !pDoc->m_pPostCtrl->IsPushOverResultEnable())
			continue;

		if(UtblD.nTBMode==D_UTBL_TYPE_PSC)
		{ 				
			CStringArray aPscCodeName;
			CDBLib::GetPscCodeNameList(aPscCodeName);
			if(aPscCodeName.GetSize()<=0)      
				continue;
		}

		CString strTemp=_T("");
		strTemp = ReplaceSymbolByTempChar(UtblD.strName);
		aName    .Add(strTemp);
		aFilePath.Add(_T(" "));//strFilePath);
		aCaption .Add(strTemp);
		aContents.Add(_T(" "));
		aType    .Add(_LSX(TBL));
	}

	// Special Tables - Section Summary
	T_SECT_K  SectK=0;
	T_SECT_D  SectD;  SectD.Initialize();

	CArray<T_SECT_K, T_SECT_K> aSectK;  
	aSectK.RemoveAll();
	pDoc->m_pAttrCtrl->GetSectKeyListUsedInElem(aSectK);
	int nSectCnt = aSectK.GetSize();

	T_RSCT_STIFF_D StiffInfo, AfterStiffInfo;
	int nType=0;  // section type

	for(int nItem=0; nItem<nSectCnt; nItem++)
	{
		SectK = 0;
		SectD.Initialize();
		strFilePath=_T("");

		SectK = aSectK[nItem];
		if(!pDoc->m_pAttrCtrl->GetSect(SectK, SectD))
			continue;

		if(!pDoc->m_pAttrCtrl->GetReportSectStiffInfo(SectD, StiffInfo, &AfterStiffInfo))
			continue;

		nType = StiffInfo.nType;
		if(nType<1 || nType>7)
		{
			ASSERT(0); continue;
		}

		CString strTemp=_T("");
		//aName.Add(ReplaceSymbolByTempChar(SectD.SName));
		strTemp.Format(_ULS('SS %d'), SectK);
		aName.Add(strTemp);
		aFilePath.Add(_T(" "));//strFilePath);
		aCaption .Add(strTemp);
		aContents.Add(_T(" "));
		aType    .Add(_LSX(TBL));
	}

	// Special Tables - Reinforcement of Sections
	aSectK.RemoveAll();
	pDoc->m_pAttrCtrl->GetRpscKeyList(aSectK);
	nSectCnt = aSectK.GetSize();

	BOOL bConsiderJ = FALSE;
	T_RSCT_RBAR_D  RpscInfoI, RpscInfoJ;

	for(int nItem=0; nItem<nSectCnt; nItem++)
	{
		SectK = 0;
		SectD.Initialize();
		strFilePath=_T("");

		SectK = aSectK[nItem];
		if(!pDoc->m_pAttrCtrl->GetSect(SectK, SectD))
			continue;

		if(!pDoc->m_pAttrCtrl->GetReportRpscInfo(SectK, bConsiderJ, RpscInfoI, RpscInfoJ))
			continue;

		CString strTemp=_T("");
		//aName.Add(ReplaceSymbolByTempChar(SectD.SName));
		strTemp.Format(_ULS('RS %d'), SectK);
		aName.Add(strTemp);
		aFilePath.Add(_T(" "));//strFilePath);
		aCaption .Add(strTemp);
		aContents.Add(_T(" "));
		aType    .Add(_LSX(TBL));
	}

	// Special Tables - Composite Section for C.S.
	CArray<T_CSCS_K, T_CSCS_K> aCscsK;
	pDoc->m_pAttrCtrl->GetCscsKeyList(aCscsK);
	int nCscsCnt = aCscsK.GetSize();
	T_CSCS_K CscsK=0;
	T_CSCS_D CscsD;
	T_STAG_K StagK=0;
	T_STAG_D StagD; StagD.Initialize();
	CArray<T_RSCT_CSCT_D, T_RSCT_CSCT_D&> aCscsInfo;
	aCscsInfo.RemoveAll();

	for(int nItem=0; nItem<nCscsCnt; nItem++)
	{
		CscsK = 0;
		CscsD.Initialize();
		strFilePath=_T("");

		CscsK = aCscsK[nItem];
		if(!pDoc->m_pAttrCtrl->GetCscs(CscsK, CscsD))
			continue;

		SectK = CscsD.SectKey;
		StagK = CscsD.ActStagKey;

		SectD.Initialize();
		if(!pDoc->m_pAttrCtrl->GetSect(SectK, SectD))
			continue;

		StagD.Initialize();
		if(!pDoc->m_pAttrCtrl->GetStag(StagK, StagD))
			continue;

		nType = pDoc->m_pAttrCtrl->GetReportCscsInfo(SectK, StagK, aCscsInfo);
		if(nType!=1 && nType!=2) continue;

		CString strTemp=_T("");
		//strTemp = ReplaceSymbolByTempChar(CscsD.SName);
		strTemp.Format(_ULS('CS %d'), CscsK);
		aName.Add(strTemp);
		aFilePath.Add(_T(" "));//strFilePath);
		aCaption .Add(strTemp);
		aContents.Add(_T(" "));
		aType    .Add(_LSX(TBL));
	}

	/////////////////////////////////////////////////
	// Chart
	T_UCHT_K  UchtK=0;
	T_UCHT_D  UchtD;  UchtD.Initialize(0);

	CArray<T_UCHT_K, T_UCHT_K> aUchtK;  
	aUchtK.RemoveAll();
	pDoc->m_pAttrCtrl->GetUchtKeyList(aUchtK);
	int nUchtCnt = aUchtK.GetSize();

	T_CHART_ITEM_D  ChartItemD; ChartItemD.Initialize();
	CGPSAMRUDCData  GenChart;

	for(int nItem=0; nItem<nUchtCnt; nItem++)
	{
		UchtK = 0;
		UchtD.Initialize(0);
		strFilePath=_T("");

		UchtK = aUchtK[nItem];
		if(!pDoc->m_pAttrCtrl->GetUcht(UchtK, UchtD))
			continue;

		// Pushover 결과 있는지 검사.
		if(!pDoc->m_pPostCtrl->IsPushOverResultEnable() && 
			( /*UchtD.nType==2 || */UchtD.nType==5 || UchtD.nType==7 || UchtD.nType==8 ))
			continue;

		ChartItemD.Initialize();
		if(!GenChart.GetData(UchtK, ChartItemD, strFilePath, 1))
			continue;

		CString strTemp=_T("");
		strTemp = ReplaceSymbolByTempChar(UchtD.strName);
		aName    .Add(strTemp);
		aFilePath.Add(_T(" "));//strFilePath);
		aCaption .Add(strTemp);
		aContents.Add(_T(" "));
		aType    .Add(_LSX(CHT));
	}

	/////////////////////////////////////////////////
	// Text
	CString strCaption=_T(" ");
	CString strText=_T(" ");
	CString strContents=_T(" ");

	CReportTreeText ReportTreeText;
	int nTextCnt=0;
#if defined(_MGEN)
	nTextCnt=D_REPORT_TEXT_CNT;
#elif(_CIVIL)
	nTextCnt=D_REPORT_TEXT_CNT-6;
#else
	ASSERT(0);
#endif

	std::vector<GenSegmentElements> segs;
	std::map<CString,CH_GEN_IFTAG_STR> mapExtInfo2IFAG;

#if defined(_CIVIL) && defined(_CH)
	CTreeReportCtrl* pReportTree = CTreeReportCtrlInstance::GetTreeReportCtrlInstance();
	if(pReportTree)
	{
		pReportTree->SetInsertState(FALSE);
		pReportTree->InsertChinaReportItemAll();
		pReportTree->SetInsertState(TRUE);

		pReportTree->GetAllGenSegments(segs);



		for (vector<GenSegmentElements>::iterator pit = segs.begin();pit!=segs.end();++pit)
		{
			GenSegmentElements& segs = *pit;
			for (std::vector<CH_GEN_IFTAG_STR>::iterator pitSeg = segs.m_SegmentElements.begin();pitSeg!=segs.m_SegmentElements.end();++pitSeg)
			{
				CH_GEN_IFTAG_STR& Iftag_str = *pitSeg;
				if(Iftag_str.iLevel != 0)    //빔쫠깃痙뫘劤
					continue;
				aName    .Add(Iftag_str.sExtInfo);
				aFilePath.Add(Iftag_str.sFullPath);
				aCaption .Add(Iftag_str.sCaption);
				aContents.Add(Iftag_str.sContents);
				aType    .Add(Iftag_str.sType);

				mapExtInfo2IFAG.insert(make_pair(Iftag_str.sExtInfo,Iftag_str));

			}

		}

	}
#else
	for(int nItem=0; nItem<nTextCnt; nItem++)
	{
		strText = m_aReportTextStrName[nItem];

		if(IsUsedText(strText) != 1) continue;

		strCaption = m_aReportTextCaption[nItem];

		aName    .Add(strText);
		aFilePath.Add(_T(" "));
		aCaption .Add(strCaption);
		aContents.Add(strContents);
		aType    .Add(_LSX(TXT));
	}
#endif



	aName    .FreeExtra();
	aFilePath.FreeExtra();
	aType    .FreeExtra();
	aCaption .FreeExtra();
	aContents.FreeExtra();

	/////////////////////////////////////////////////
	// To Word

	// Make structure to export
	int nName = aName.GetSize();
	int nFilePath = aFilePath.GetSize();
	ASSERT(nName==nFilePath);
	int nItemCnt=0;
	nItemCnt = nName;

	CString	sProductInfo=_T(" "); // 제품정보
	CString	sCaption    =_T(" "); // Caption
	CString	sType       =_T(" "); // Type(image, table, image file, chart, text)
	CString	sFullPath   =_T(" "); // 파일경로 및 파일명(그림파일, 테이블Html파일)
	CString	sExtInfo    =_T(" "); // 추출정보
	CString	sContents   =_T(" "); // 내용(text일때 Word에 삽입할 내용)
	int		  iAppendFlag =0;   // 삽입삭제 구분 (삽입:0, 비교:1, 삭제:2, 제너레이션:9), default는 0
	CString	sExt        =_T(" "); // 향후를 대비한 예비 member. default는 NULL

	GEN_IFTAG* pIftag=NULL;
	pIftag = new GEN_IFTAG[nItemCnt];

#if defined(_CIVIL)
	sProductInfo  = _LSX(CIVIL);
#elif(_MGEN)
	sProductInfo  = _LSX(GEN);
#else
	ASSERT(0);
	sProductInfo  = _T(" ");
#endif

	iAppendFlag   = D_WORD_APPEND_COMPARE; 
	sExt          = _T(" ");

	CStringDeepCopyCollector SDCColl;
	for(int nItem=0; nItem<nItemCnt; nItem++)
	{
		sCaption      = aCaption[nItem];
		sType         = aType[nItem];
		sFullPath     = aFilePath[nItem];
		sContents     = aContents[nItem];
		sExtInfo      = aName[nItem];

		pIftag[nItem].sProductInfo = SDCColl.StringDeepCopy(sProductInfo);
		pIftag[nItem].sCaption     = SDCColl.StringDeepCopy(sCaption    );
		pIftag[nItem].sType        = SDCColl.StringDeepCopy(sType       );
		pIftag[nItem].sFullPath    = SDCColl.StringDeepCopy(sFullPath   );
		pIftag[nItem].sExtInfo     = SDCColl.StringDeepCopy(sExtInfo    );
		pIftag[nItem].sContents    = SDCColl.StringDeepCopy(sContents   );
		pIftag[nItem].sExt         = SDCColl.StringDeepCopy(sExt        );
		pIftag[nItem].iAppendFlag  = iAppendFlag;
	}

	int nRetCnt=0;
	GEN_IFTAG* pRetIftag=NULL;

	// Export to Word
	HINSTANCE hMod = CLoadWordLib::GetInstance()->GetWordLibrary();
	if(hMod!=NULL)	
	{
		// Word에서 선택한 항목 받아오기
		CWinApp* pApp = AfxGetApp();
		int nViewFlag = pApp->GetProfileInt(_LSX(DynaGen Report\\Auto Regeneration), _LSX(Auto Regeneration Item), 0);

		EditorImport pImport=NULL;
		pImport = (EditorImport) GetProcAddress(hMod, "EditorImport");
		int iRet = pImport(nItemCnt, (struct GEN_IFTAG**)&pIftag, nViewFlag, &nRetCnt, &pRetIftag);
		pImport=NULL;

		if(nRetCnt<=0 || iRet==-1) 
		{
			delete[] pIftag;
			return;
		}

		CStringArray aProductInfo ; aProductInfo.RemoveAll();
		CStringArray aType        ; aType.RemoveAll();
		CStringArray aExtInfo     ; aExtInfo.RemoveAll();

		for(int nItem=0; nItem<nRetCnt; nItem++)
		{
			sProductInfo = _T(" "); sType = _T(" "); sExtInfo = _T(" ");
			sProductInfo  = pRetIftag[nItem].sProductInfo;
			sType         = pRetIftag[nItem].sType;
			sExtInfo      = pRetIftag[nItem].sExtInfo;

			aProductInfo.Add(sProductInfo);
			aType.Add(sType);
			aExtInfo.Add(sExtInfo);
		}

		// Progress bar 추가하기
		//     CReportProgressbarDlg dlg;
		//     dlg.DoModal();

		// Word로 Regenerate할 항목 구성하기
		GEN_IFTAG* pRegIftag=NULL;
		pRegIftag = new GEN_IFTAG[nRetCnt];

		aName    .RemoveAll();
		aFilePath.RemoveAll();
		aContents.RemoveAll();

		CString strName=_T("");
		CString strType=_T("");
		strFilePath=_T("");
		strContents=_T("");

		CString strModelPath=_T("");
		strModelPath = CDBDoc::GetDocPoint()->GetPathName();

		CFileCtrl FileCtrl2(strModelPath);
		CString strFolder=_T("");
		strFolder = FileCtrl2.GetFilePath();

		CArray<BOOL, BOOL> aRepeat; aRepeat.RemoveAll();
		BOOL bRepeat = TRUE;

		for(int nItem=0; nItem<nRetCnt; nItem++)
		{
			strType.Format(_T("%s"), pRetIftag[nItem].sType);

			// User Defined Image
			if(_tcsicmp(strType, _LSX(IMG))==0)
			{
				strName.Format(_T("%s"), pRetIftag[nItem].sExtInfo);
				if(pDoc->m_pAttrCtrl->ExistUfig(strName))
				{
					UfigK = pDoc->m_pAttrCtrl->GetUfigKey(strName);
					ASSERT(UfigK > 0);

					if(!CGPSAMRUtil::GetInstance()->GetMakeFigureAndPath(UfigK, strFilePath))
						continue;

					aName    .Add(strName);
					aFilePath.Add(strFilePath);
					aContents.Add(_T(" "));
				}
				else
				{

					std::map<CString,CH_GEN_IFTAG_STR>::iterator pFind = mapExtInfo2IFAG.find(strName);
					if(pFind != mapExtInfo2IFAG.end())
					{     
						aName    .Add(pFind->second.sExtInfo);
						aFilePath.Add(pFind->second.sFullPath);
						aContents.Add(pFind->second.sContents);

					}
					else
					{
						ASSERT(0);
					}
				}
			}
			// Image File
			else if(_tcsicmp(strType, _LSX(IFL))==0)
			{
				strName.Format(_T("%s"), pRetIftag[nItem].sExtInfo);
				if(pDoc->m_pAttrCtrl->ExistUimg(strName))
				{
					T_UIMG_D UimgD; UimgD.Initialize();
					if(!pDoc->m_pAttrCtrl->GetUimg(strName, UimgD))
						continue;

					//strFilePath = UimgD.strFilePath;
					strFilePath.Format(_LS(IDS_TM_DYNA_REPORT__s_Image_Files__s), strFolder, UimgD.strFilePath);
					ASSERT(strFilePath!=_T(""));
					CFileCtrl FileCtrl(strFilePath);
					if(!FileCtrl.FileExists(strFilePath))
					{
						ASSERT(0); continue;
					}

					aName    .Add(strName);
					aFilePath.Add(strFilePath);
					aContents.Add(_T(" "));
				}
				else
				{
					std::map<CString,CH_GEN_IFTAG_STR>::iterator pFind = mapExtInfo2IFAG.find(strName);
					if(pFind != mapExtInfo2IFAG.end())
					{     
						aName    .Add(pFind->second.sExtInfo);
						aFilePath.Add(pFind->second.sFullPath);
						aContents.Add(pFind->second.sContents);

					}
					else
					{
						ASSERT(0);
					}

				}


			}
			// Table
			else if(_tcsicmp(strType, _LSX(TBL))==0)
			{
				strName.Format(_T("%s"), pRetIftag[nItem].sExtInfo);

				CString strTmp = strName;
				int r=-1;
				int nTableType=0;  // 1: User Defined Table, 2: Section Summary, 3: Reinforcement of Sections, 4: Composite Section for C.S.

				r = strTmp.Find(_ULS('SS ')); 
				if(r==0) 
				{
					nTableType = 2;
					strTmp.Replace(_ULS('SS '),_T(""));
				}

				if(r==-1)
				{
					r = strTmp.Find(_ULS('RS ')); 
					if(r==0) 
					{
						nTableType = 3;
						strTmp.Replace(_ULS('RS '),_T(""));
					}
				}

				if(r==-1)
				{
					r = strTmp.Find(_ULS('CS ')); 
					if(r==0) 
					{
						nTableType = 4;
						strTmp.Replace(_ULS('CS '),_T(""));
					}
				}

				if(r==-1)
				{
					nTableType = 1;
				}

				ASSERT(nTableType>0 && nTableType<5);

				CStringArray arImagePath;
				CDgnAmrSectionTb SectionTb;
				CString strImagePath=_T("");
				CDgnDrawSection DgnDrawSection;

				_DGN_DRAW_SECT_OPTION OptionD;
				OptionD.Initialize();
				OptionD.Canvas.SetRect(0,0,7500,7500);
				OptionD.DimSize = 500;

				// 1: User Defined Table
				if(nTableType==1) 
				{
					if(pDoc->m_pAttrCtrl->ExistUtbl(strTmp))
					{
						UtblK = 0;
						UtblK = pDoc->m_pAttrCtrl->GetUtblKey(strTmp);

						if(UtblK <=0 )
							continue;

						// xml file name
						CString strFilePathName=_T("");
						CString strFullPath=_T("");
						//strFullPath = CDBDoc::GetDocPoint()->GetPathName();
						::GetTempPath(_MAX_PATH, strFullPath.GetBuffer(_MAX_PATH)); // 임시폴더에다 저장함. 20100429 KIMJM
						strFullPath.ReleaseBuffer();
						CFileCtrl FileCtrl(strFullPath);
						strFilePathName = FileCtrl.GetFilePathAndNameWithoutExtension();
						strFilePath.Format(_T("%s%s.xml"), strFilePathName, strTmp);

						bRepeat = TRUE;
						if(!pDoc->MakeReportTableXml(UtblK, strFilePath, bRepeat))
							continue;

						aRepeat.Add(bRepeat);
					}
					else
					{
						std::map<CString,CH_GEN_IFTAG_STR>::iterator pFind = mapExtInfo2IFAG.find(strName);
						if(pFind != mapExtInfo2IFAG.end())
						{
							strFilePath = pFind->second.sFullPath;
							aRepeat.Add(TRUE);
						}
						else
						{
							ASSERT(0);
						}

					}


				}
				// 2: Section Summary
				else if(nTableType==2) 
				{
					T_RSCT_STIFF_D StiffInfo, AfterStiffInfo;
					int nType=0;  // section type

					SectK = _ttoi(strTmp);

					if(!pDoc->m_pAttrCtrl->ExistSect(SectK))
						continue;

					SectD.Initialize();
					if(!pDoc->m_pAttrCtrl->GetSect(SectK, SectD))
						continue;

					arImagePath.RemoveAll();

					if(!pDoc->m_pAttrCtrl->GetReportSectStiffInfo(SectD, StiffInfo, &AfterStiffInfo))
						continue;
					nType = StiffInfo.nType;

					// set file path name
					CString strFilePathName=_T("");
					strFilePathName = GetModelFullPathName();
					CString strSName=_T("");
					strSName = SectD.SName;
					FilterFileName(strSName);
					strImagePath.Format(_T("%s%s_%d.emf"), strFilePathName, strSName, SectK);
					strFilePath.Format(_T("%s%s_%d.xml"), strFilePathName, strSName, SectK);

					// make section image
					MakeSectionFigure(strImagePath, SectK, nType, arImagePath);

					if(!SectionTb.GenerateSectionTable(strFilePath, arImagePath, D_SECTION_TB_SS, SectK, 0))
						continue;
				}
				// 3: Reinforcement of Sections
				else if(nTableType==3) 
				{
					BOOL bConsiderJ = FALSE;
					T_RSCT_RBAR_D  RpscInfoI, RpscInfoJ;

					SectK = _ttoi(strTmp);

					if(!pDoc->m_pAttrCtrl->ExistSect(SectK))
						continue;

					SectD.Initialize();
					if(!pDoc->m_pAttrCtrl->GetSect(SectK, SectD))
						continue;

					if(!pDoc->m_pAttrCtrl->GetReportRpscInfo(SectK, bConsiderJ, RpscInfoI, RpscInfoJ))
						continue;

					// set file path name
					CString strFilePathName=_T("");
					strFilePathName = GetModelFullPathName();
					CString strSName=_T("");
					strSName = SectD.SName;
					FilterFileName(strSName);
					strImagePath.Format(_T("%s%s_%d.emf"), strFilePathName, strSName, SectK);
					strFilePath.Format(_T("%s%s_%d.xml"), strFilePathName, strSName, SectK);

					// make section image
					arImagePath.RemoveAll();
					if(bConsiderJ==FALSE)
					{
						DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 0/*int nIMJ*/, OptionD, FALSE, 1, FALSE);
						arImagePath.Add(strImagePath);
					}
					else if(bConsiderJ==TRUE)
					{
						DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 0, OptionD, FALSE, 1, FALSE);
						arImagePath.Add(strImagePath);
						DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 2, OptionD, FALSE, 1, FALSE);
						arImagePath.Add(strImagePath);
					}
					else
					{
						ASSERT(0); continue;
					}

					if(!SectionTb.GenerateSectionTable(strFilePath, arImagePath, D_SECTION_TB_RS, SectK, 0))
						continue;
				}
				// 4: Composite Section for C.S.
				else if(nTableType==4) 
				{
					CArray<T_RSCT_CSCT_D, T_RSCT_CSCT_D&> aCscsInfo;
					int nType=0;  // section type

					T_CSCS_K CscsK=0;
					T_CSCS_D CscsD;
					T_STAG_K StagK=0;

					CscsK = _ttoi(strTmp);

					if(!pDoc->m_pAttrCtrl->ExistCscs(CscsK))
						continue;

					CscsD.Initialize();
					if(!pDoc->m_pAttrCtrl->GetCscs(CscsK, CscsD))
						continue;

					SectK = CscsD.SectKey;
					StagK = CscsD.ActStagKey;

					nType = pDoc->m_pAttrCtrl->GetReportCscsInfo(SectK, StagK, aCscsInfo);
					int nCscsNum = aCscsInfo.GetSize();

					// set file path name
					CString strFilePathName=_T("");
					strFilePathName = GetModelFullPathName();
					CString strSName=_T("");
					strSName = SectD.SName;
					FilterFileName(strSName);
					strImagePath.Format(_T("%s%s_%d.emf"), strFilePathName, strSName, SectK);
					strFilePath.Format(_T("%s%s_%d.xml"), strFilePathName, strSName, SectK);

					if(nType==1)
					{
						for(int i=0; i<nCscsNum; i++)
						{
							strImagePath.Format(_T("%s_%s_%d.emf"), strFilePathName, strSName, i);
							DgnDrawSection.Make_SectionFig_For_SectK(strImagePath, SectK, 0, OptionD, FALSE, 1, FALSE);
							arImagePath.Add(strImagePath);
						}
					}
					else if(nType==2)
					{
						arImagePath.RemoveAll();
					}
					else
					{
						ASSERT(0);
						continue;
					}

					if(!SectionTb.GenerateSectionTable(strFilePath, arImagePath, D_SECTION_TB_CS, SectK, StagK))
						continue;
				}

				aName    .Add(strName);
				aFilePath.Add(strFilePath);
				aContents.Add(_T(" "));
			}
			// Chart
			else if(_tcsicmp(strType, _LSX(CHT))==0)
			{
				strName.Format(_T("%s"), pRetIftag[nItem].sExtInfo);
				if(!pDoc->m_pAttrCtrl->ExistUcht(strName))
					continue;

				UchtK = 0;
				UchtK = pDoc->m_pAttrCtrl->GetUchtKey(strName);
				VERIFY(pDoc->m_pAttrCtrl->GetUcht(UchtK, UchtD));

				if(UchtK <=0 )
					continue;

				ChartItemD.Initialize();
				if(!GenChart.GetData(UchtK, ChartItemD, strFilePath))
					continue;

				pDoc->MakeGraphAndBmpImage(ChartItemD, strFilePath, &UchtD); 

				aName    .Add(strName);
				aFilePath.Add(strFilePath);
				aContents.Add(_T(" "));
			}
			// Text
			else if(_tcsicmp(strType, _LSX(TXT))==0)
			{
				strName.Format(_T("%s"), pRetIftag[nItem].sExtInfo);
				strContents=_T(" ");
#if defined(_CIVIL) && defined(_CH)
				std::map<CString,CH_GEN_IFTAG_STR>::iterator pFind = mapExtInfo2IFAG.find(strName);
				if(pFind != mapExtInfo2IFAG.end())
				{
					strContents = pFind->second.sContents;
				}
#else
				if(!ReportTreeText.WriteReportText(strName, strContents))
					continue;
#endif


				aName    .Add(strName);
				aFilePath.Add(strFilePath);
				aContents.Add(strContents);
			}
			else
			{
				ASSERT(0); continue;
			}
		}
		aName    .FreeExtra();
		aFilePath.FreeExtra();
		aContents.FreeExtra();

		nName=0;  nFilePath=0;
		nName = aName.GetSize();
		nFilePath = aFilePath.GetSize();
		ASSERT(nName==nFilePath);
		nItemCnt = nName;

		sCaption    =_T(""); // Caption
		strType     =_T("");
		sFullPath   =_T(""); // 파일경로 및 파일명(그림파일, 테이블Html파일)
		sContents   =_T(""); // 내용(text일때 Word에 삽입할 내용)

		// Word로 Regenerate할 항목 보내기
		for(int nItem=0; nItem<nItemCnt; nItem++)
		{
			sCaption.Format(_T("%s"), pRetIftag[nItem].sCaption);
			sCaption.Remove(' ');
			if(_tcsicmp(sCaption, _T(""))==0) sCaption=_T(" ");
			else                          sCaption.Format(_T("%s"), pRetIftag[nItem].sCaption);

			strType.Format(_T("%s"), pRetIftag[nItem].sType);
			if(_tcsicmp(strType, _LSX(IMG))==0 ||
				_tcsicmp(strType, _LSX(TBL))==0 ||
				_tcsicmp(strType, _LSX(TPL))==0 ||
				_tcsicmp(strType, _LSX(CHT))==0 ||
				_tcsicmp(strType, _LSX(IFL))==0 )
				sFullPath = aFilePath[nItem];
			else
				sFullPath = _T(" ");

			if(_tcsicmp(strType, _LSX(TXT))==0)
				sContents = aContents[nItem];
			else
				sContents = _T(" ");

			if(nItem < aRepeat.GetSize())
			{
				sExt = aRepeat[nItem]? _T(" ") : _T("0");
			}
			else
				sExt = _T(" ");

			iAppendFlag = D_WORD_APPEND_REGENERATE;

			pRegIftag[nItem].sProductInfo = pRetIftag[nItem].sProductInfo;
			pRegIftag[nItem].sCaption     = SDCColl.StringDeepCopy(sCaption    );
			pRegIftag[nItem].sType        = pRetIftag[nItem].sType       ;
			pRegIftag[nItem].sFullPath    = SDCColl.StringDeepCopy(sFullPath   );
			pRegIftag[nItem].sExtInfo     = pRetIftag[nItem].sExtInfo    ;
			pRegIftag[nItem].sContents    = SDCColl.StringDeepCopy(sContents   );
			pRegIftag[nItem].sExt         = SDCColl.StringDeepCopy(sExt        );
			pRegIftag[nItem].iAppendFlag  = D_WORD_APPEND_REGENERATE;
		}

		EditorExport pExport=NULL;
		pExport = (EditorExport) GetProcAddress(hMod, "EditorExport");
		int nRet = pExport(nItemCnt, &pRegIftag);

		delete[] pRegIftag;

		// 임시파일 삭제
		CFileFind file;
		for(int nItem=0; nItem<nItemCnt; nItem++)
		{
			if(_tcsicmp(aType[nItem], _LSX(IFL))!=0 ) // Image File Item은 원본이 파일 경로이므로 삭제해서는 안된다. 20110331 KIMJM
			{
				sFullPath=_T("");
				sFullPath = aFilePath[nItem];
				if(file.FindFile(sFullPath))
					::DeleteFile(sFullPath);
			}
		}
		file.Close();
	}
	else
	{
		ASSERT(0);
		AfxMessageBox(_LS(IDS_TM_DYNA_REPORT_REPORT_ERROR));
	}

	delete[] pIftag;

	hMod = NULL;
}

CChinaReportSetting::CChinaReportSetting()
{
	m_mapItems.insert(make_pair(_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_BASICINFO     ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_AMR_CHRP_BasicInfo_General     ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_AMR_CHRP_BasicInfo_Standard    ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_AMR_CHRP_BasicInfo_Code        ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_AMR_CHRP_BasicInfo_Structure   ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_AMR_CHRP_BasicInfo_Material    ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_AMR_CHRP_BasicInfo_Calculation ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_MODEL     ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_AMR_CHRP_Model_Calculation     ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_AMR_CHRP_Model_Steel    ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_AMR_CHRP_Model_Section        ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_AMR_CHRP_Model_Load   ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_LOADCAPACITY     ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_AMR_CHRP_LoadCapacity_ComDepth     ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_AMR_CHRP_LoadCapacity_Bending    ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_AMR_CHRP_LoadCapacity_Shearing        ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_AMR_CHRP_LoadCapacity_Torsion   ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_AMR_CHRP_LoadCapacity_Reforce   ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_SERVICEABILITY     ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_AMR_CHRP_ServiceAbility_Normal     ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_AMR_CHRP_ServiceAbility_Oblique    ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_STRESS     ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_AMR_CHRP_Stress_NormalComp     ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_AMR_CHRP_Stress_Tension    ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_AMR_CHRP_Stress_PrincipalComp    ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_SHORTTERMSTRESS     ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_AMR_CHRP_Stress_ShortTerm     ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_TM_DYNA_REPORT_DEFINED_CHRP_FORCE     ),TRUE));
	m_mapItems.insert(make_pair(_LS(IDS_AMR_CHRP_Force_MemberForce     ),TRUE));

	m_StandradSectKey.RemoveAll();

	m_bWholeCalProcess = FALSE;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(pDoc)
	{
		CArray<T_SECT_K, T_SECT_K> rKeyList;
		pDoc->m_pAttrCtrl->GetSectKeyList(rKeyList);
		if(rKeyList.GetSize() > 0)
		{
			m_StandradSectKey.Add(rKeyList.GetAt(0))  ;
		}

	}

	m_StandradForcetypeID.Add(5);  //my

}
CChinaReportSetting::~CChinaReportSetting()
{

}

CChinaReportSetting* CChinaReportSetting::GetChinaReportSetting()
{
	static  CChinaReportSetting data;
	return &data;
}

BOOL CChinaReportSetting::GetChecked(const CString& strItem) const
{
	if(IsExistItem(strItem))
	{
		std::map<CString,BOOL>::const_iterator pFind = m_mapItems.find(strItem);
		return pFind->second;
	}
	ASSERT(0);
	return FALSE;
}

void CChinaReportSetting::ModifyCheck( const CString& strItem,BOOL bCheck )
{
	if(IsExistItem(strItem))
		m_mapItems[strItem]=bCheck;
	else
		AddCheck(strItem,bCheck);
}

void CChinaReportSetting::AddCheck( const CString& strItem,BOOL bCheck )
{
	if(!IsExistItem(strItem))
	{
		m_mapItems.insert(make_pair(strItem,bCheck));
	}
	else
		m_mapItems[strItem]=bCheck;
}

BOOL CChinaReportSetting::IsExistItem( const CString& strItem ) const
{
	std::map<CString,BOOL>::const_iterator pFind = m_mapItems.find(strItem);
	return pFind != m_mapItems.end();
}

BOOL CChinaReportSetting::IsWholeCalProcess() const
{
	return m_bWholeCalProcess;
}

void CChinaReportSetting::SetWholeCalProcess( BOOL bCheck )
{
	m_bWholeCalProcess = bCheck;
}

void CChinaReportSetting::GetStandradSectKey(CArray<int,int>& sectkey) const
{
	sectkey.Copy(m_StandradSectKey);
}

void CChinaReportSetting::GetStandradLoadCombKey(CArray<sLoadCombIDType,sLoadCombIDType>& _StandradLoadCombKey ) const
{
	_StandradLoadCombKey.Copy(m_StandradLoadCombKey);
}

void CChinaReportSetting::SetStandradSectKey(const CArray<int,int>& sectkey)
{
	m_StandradSectKey.Copy(sectkey) ;
}

void CChinaReportSetting::SetStandradLoadCombKey( const CArray<sLoadCombIDType,sLoadCombIDType>& _StandradLoadCombKey )
{
	m_StandradLoadCombKey.Copy(_StandradLoadCombKey);
}

void CChinaReportSetting::GetStandradElemKey( CArray<T_GRUP_K,T_GRUP_K>& Elemkey ) const
{
	Elemkey.Copy(m_StandradElemKey);
}

void CChinaReportSetting::SetStandradElemKey( const CArray<T_GRUP_K,T_GRUP_K>& Elemkey )
{
	m_StandradElemKey.Copy(Elemkey);
}

void CChinaReportSetting::GetStandradForcetype( CArray<int,int>& forcetypes ) const
{
	forcetypes .Copy(m_StandradForcetypeID);
}

void CChinaReportSetting::SetStandradForcetype( const CArray<int,int>& forcetypes )
{
	m_StandradForcetypeID.Copy(forcetypes);
}

BOOL CChinaReportSetting::GetOutPutElemList( CArray<T_ELEM_K,T_ELEM_K>& aElemK )
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	aElemK.RemoveAll();
	CArray<T_ELEM_K,T_ELEM_K> aSelElemK;

	if(m_StandradElemKey.GetSize() > 0)
	{
		T_GRUP_D rGrupData;
		rGrupData.Initialize();
		for (int i=0;i<m_StandradElemKey.GetSize();i++)
		{
			T_GRUP_K groupKey = m_StandradElemKey.GetAt(i);
			if(!pDoc->m_pAttrCtrl->GetGrup(groupKey,rGrupData))
				continue;
			aElemK.Append(rGrupData.arKeyElem);
		}
		return TRUE;
	}


	int iTotElemNum =pDoc->m_pAttrCtrl->GetCountElem();
	for(int i=0; i<iTotElemNum; i++)
	{
		T_ELEM_K ElemK = pDoc->m_pPostCtrl->GetOriginElem(i+1); 
		int iNum=0; BOOL bGetOK = FALSE;
		if (TRUE) {
			T_POSC_D PoscD; PoscD.Initialize();
			iNum = pDoc->m_pAttrCtrl->GetCountPosc();
			bGetOK = pDoc->m_pAttrCtrl->GetPosc(ElemK, PoscD);
		}else{
			T_DORC_D DorcD; DorcD.Initialize();
			iNum = pDoc->m_pAttrCtrl->GetCountDorc();
			bGetOK = pDoc->m_pAttrCtrl->GetDorc(ElemK,DorcD);
		}
		BOOL bIncludeElem = TRUE;
		if (!bGetOK && iNum>0) bIncludeElem = FALSE;
		if (bIncludeElem) aSelElemK.Add(ElemK);
	}
	for(int i=0; i<aSelElemK.GetSize(); i++)
	{
		T_ELEM_K ElemK = aSelElemK.GetAt(i);
		if (TRUE) {
			if(!CDBLib::IsPscDgnFor1st(ElemPairK(ElemK, EN_EL_BEAM),TRUE))	continue;
			if(pDoc->m_pAttrCtrl->GetMemberType(ElemK)!=D_MBTP_BEAM)	continue;
		}else{
			if(!CDBLib::IsRcColumnDgn(ElemK,TRUE))	continue;    
			if(pDoc->m_pAttrCtrl->GetMemberType(ElemK)!=D_MBTP_COLUMN)	continue;
		}
		aElemK.Add(ElemK);
	}  
	if(aElemK.GetSize() > 0)	return TRUE;
	else											return FALSE;
}