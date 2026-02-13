//////////////////////////////////////////////////////////////////////
// RptTB_10002_3_05_CH.h: interface for the DgnJTG_D62_04 class.
// author: maxiao
// time  : 2006-12-08
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RPTTB_10002_3_05_CH_H__INCLUDED_)
#define AFX_RPTTB_10002_3_05_CH_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PrtKSCE_USD03.h"

#include "..\wg_xl\ReportBase.h"
#include "RCImportFile_CH.h"
#include "HeaderPre.h"

class CDBDoc;

class __MY_EXT_CLASS__ CRptTB_10002_3_05_CH : public CReportBase , public CPrtKSCE_USD03  
{
public:
	CRptTB_10002_3_05_CH();
	virtual ~CRptTB_10002_3_05_CH();

    virtual CString GetFilePathName();

protected:  
    CDBDoc* m_pDoc;
    CString m_strMpa;

    CRCDataCtrl_CH   m_DataCtrl;
    CCRCForceCtrl* m_pForcCtrl;
    
    CRCImportFile_CH* m_pTB_10002_3_05_CH;

    T_CHRP_D m_ChrcD;

    CDgnPscCommon* m_pPscCommon;
    int m_iElemNum;
    int m_iColumnElemNum;
    int m_iNodeNum;
    int m_iTendonNum;
    int m_iBoundaryNum;
    int m_iStageNum;
    CArray<T_ELEM_K,T_ELEM_K> m_arElemKeyList;
    CArray<T_ELEM_K,T_ELEM_K> m_arColumnElemKeyList;
    CArray<T_ELEM_K,T_ELEM_K> m_arMatElem;
    CArray<T_TDNA_K,T_TDNA_K> m_arTdnaK;
    CArray<T_TDNT_K,T_TDNT_K> m_arTdntK;
    CArray<T_STAG_K,T_STAG_K> m_arStagK; 
    CArray<T_GRUP_K,T_GRUP_K> m_arGroupKeyList;
    
/*  
    _PSC_MATL_ELEM m_MatlElem;
    _PSC_TNDN_TDNA m_TndnTdna;
    CMap<T_ELEM_K,T_ELEM_K,_PSC_MATL_ELEM,_PSC_MATL_ELEM&> m_amMatlElem;
    CMap<T_TDNA_K,T_TDNA_K,_PSC_TNDN_TDNA,_PSC_TNDN_TDNA&> m_amTndnTdna;
*/


protected:
    virtual BOOL MakeData();
    virtual BOOL MakeDefaultStyle();

    BOOL MakeDataFromDB();
    
    void InitializeData();

    // 0-1. 표지 Data 구성.
    BOOL MakeCoverData();

    // 0-2. 목차 Data를 구성.
    BOOL MakeContents();

    // 1. 설계기준 Data 구성.
    BOOL MakeCodes();

    // 2. 설계자료 Data 구성.
    BOOL MakeDesignParameter();

    // 3. 재질 Data 구성.
    BOOL MakeMaterialData();  

    // 4. 모델에 대한 간단한 소개 Data 구성.
    BOOL MakeModelData();  
    
    // 5. 하중조합 설명 Data 구성.
    BOOL MakeLComData();

    // 6. 검토결과 Table 구성.
    BOOL MakeCheckResultTable();

    // 7. 졺돨圈玖셕炬법넋
    BOOL MakeCheckBeamResultData();

    // 8. 料돨圈玖셕炬법넋
    BOOL MakeCheckColumnResultData();


    // S***********************************************************
    // Add by GAY.('06.05.13). Make Sub Chapter Data.

    // 3.1. 콘크리트 재질 Data 구성.
    //   iChap : 하중관련 출력부의 Chapter index.
    //   paraD : 구성된 Paragraph Data.
    //   RETURN : Data가 구성되면 TRUE, 아니면 FALSE.
    BOOL MakeConcData(int& iChapID, T_PARA_RPT& paraD);

    // 3.2. 프리스트레스 철근 Data 구성.
    //   iChap : 하중관련 출력부의 Chapter index.
    //   paraD : 구성된 Paragraph Data.
    //   RETURN : Data가 구성되면 TRUE, 아니면 FALSE.
    BOOL MakePrestressRebar(int& iChapID, T_PARA_RPT& paraD);

    // 3.3. 종방향 인장 보통철근 Data 구성.
    //   iChap : 하중관련 출력부의 Chapter index.
    //   paraD : 구성된 Paragraph Data.
    //   RETURN : Data가 구성되면 TRUE, 아니면 FALSE.
    BOOL MakeTensionRebar(int& iChapID, T_PARA_RPT& paraD);

    // 3.4. 전단철근 및 구조철근 Data 구성.
    //   iChap : 하중관련 출력부의 Chapter index.
    //   paraD : 구성된 Paragraph Data.
    //   RETURN : Data가 구성되면 TRUE, 아니면 FALSE.
    BOOL MakeShearRebar(int& iChapID, T_PARA_RPT& paraD);

    // 3.3. 보통철근 Data의 구성.
    //   iChap : 하중관련 출력부의 Chapter index.
    //   paraD : 구성된 Paragraph Data.
    //   RETURN : Data가 구성되면 TRUE, 아니면 FALSE.
    BOOL MakeRebar(int& iChapID, T_PARA_RPT& paraD);

    // 4.5. Stage Data 구성.
    //   iChap : 하중관련 출력부의 Chapter index.
    //   paraD : 구성된 Paragraph Data.
    //   RETURN : Data가 구성되면 TRUE, 아니면 FALSE.
    BOOL MakeStageGroupSummaryData(int& iChapID, T_PARA_RPT& paraD);

    // 4.5.1. Element Group Activation Summary Data 구성.
    //   iChap : 출력부의 Chapter index.
    //   paraD : 구성된 Paragraph Data.
    //   RETURN : Data가 구성되면 TRUE, 아니면 FALSE.
    BOOL MakeElemGroupData(int& iChapID, T_PARA_RPT& paraD);

    // 4.5.2. Boundary Group Activation Summary Data 구성.
    //   iChap : 출력부의 Chapter index.
    //   paraD : 구성된 Paragraph Data.
    //   RETURN : Data가 구성되면 TRUE, 아니면 FALSE.
    BOOL MakeBoundaryGroupData(int& iChapID, T_PARA_RPT& paraD);

    // 4.5.3. Load Group Activation Summary Data 구성.
    //   iChap : 출력부의 Chapter index.
    //   paraD : 구성된 Paragraph Data.
    //   RETURN : Data가 구성되면 TRUE, 아니면 FALSE.
    BOOL MakeLoadGroupData(int& iChapID, T_PARA_RPT& paraD);
    
    // 4.6. 사용단계 Data 구성.
    //   iChap : 하중관련 출력부의 Chapter index.
    //   chapD : 구성된 Chapter Data.
    BOOL MakeServiceStageLoadData(int iChapID, T_CHAP_RPT& chapD);

    // 4.6.1. 활하중 Type Data를 구성함.
    //   tableD : 구성된 활하중의 Table Data.  
    BOOL MakeLiveLoadData(T_PARA_RPT& paraD);

    // 4.6.3.1) System Temerature Load Data를 구성함.
    //   iChapID : Chapter index. Data가 구성되는 경우에는 iChapID++로 return.
    //   paraD : 구성된 Paragraph Data.
    //   RETURN : Data가 구성되면 TRUE, 아니면 FALSE.
    BOOL MakeSystemTempData(int& iChapID, T_PARA_RPT& paraD);

    // 4.6.3.2) Nodal Temperature Load Data를 구성함.
    //   iChapID : Chapter index. Data가 구성되는 경우에는 iChapID++로 return.
    //   paraD : 구성된 Paragraph Data.
    //   RETURN : Data가 구성되면 TRUE, 아니면 FALSE.  
    BOOL MakeNodalTempData(int& iChapID, T_PARA_RPT& paraD);

    // 4.6.3.3). Element Temperature Load Data를 구성함.
    //   iChapID : Chapter index. Data가 구성되는 경우에는 iChapID++로 return.
    //   paraD : 구성된 Paragraph Data.
    //   RETURN : Data가 구성되면 TRUE, 아니면 FALSE.  
    BOOL MakeElemTempData(int& iChapID, T_PARA_RPT& paraD);

    // 4.6.3.4) Temperature Gradient Data를 구성함.
    //   iChapID : Chapter index. Data가 구성되는 경우에는 iChapID++로 return.
    //   paraD : 구성된 Paragraph Data.
    //   RETURN : Data가 구성되면 TRUE, 아니면 FALSE.  
    BOOL MakeTempGradientData(int& iChapID, T_PARA_RPT& paraD);

    // 4.6.3.5) Beam Section Gradient Data를 구성함.
    //   iChapID : Chapter index. Data가 구성되는 경우에는 iChapID++로 return.
    //   paraD : 구성된 Paragraph Data.
    //   RETURN : Data가 구성되면 TRUE, 아니면 FALSE.  
    BOOL MakeBeamSectTempData(int& iChapID, T_PARA_RPT& paraD);

    // 4.6.4. 지점침하 Data를 구성함.
    //   paraD : 구성된 Paragraph Data.
    //   RETURN : Data가 구성되면 TRUE, 아니면 FALSE. 
    BOOL MakeSpecifiedDispData(T_PARA_RPT& paraD);

    // 5.1.1. Static Load Cases Data를 구성함.
    //   iChapID : Chapter index. Data가 구성되는 경우에는 iChapID++로 return.
    //   paraD : 구성된 Paragraph Data.
    //   RETURN : Data가 구성되면 TRUE, 아니면 FALSE.  
    BOOL MakeStaticLoadCaseData(int& iChapID, T_PARA_RPT& paraD);

    // 5.1.2. Response Spectrum Data를 구성함.
    //   iChapID : Chapter index. Data가 구성되는 경우에는 iChapID++로 return.
    //   paraD : 구성된 Paragraph Data.
    //   RETURN : Data가 구성되면 TRUE, 아니면 FALSE.  
    BOOL MakeResponseSpectrumData(int& iChapID, T_PARA_RPT& paraD);

    // 5.1.3. Time History Load Case Data를 구성함.
    //   iChapID : Chapter index. Data가 구성되는 경우에는 iChapID++로 return.
    //   paraD : 구성된 Paragraph Data.
    //   RETURN : Data가 구성되면 TRUE, 아니면 FALSE.  
    BOOL MakeTimeHistoryData(int& iChapID, T_PARA_RPT& paraD);

    // 5.1.4. Moving Load Case Data를 구성함.
    //   iChapID : Chapter index. Data가 구성되는 경우에는 iChapID++로 return.
    //   paraD : 구성된 Paragraph Data.
    //   RETURN : Data가 구성되면 TRUE, 아니면 FALSE.  
    BOOL MakeMovingLoadData(int& iChapID, T_PARA_RPT& paraD);

    // 5.1.5. Settlement Load Data를 구성함.
    //   iChapID : Chapter index. Data가 구성되는 경우에는 iChapID++로 return.
    //   paraD : 구성된 Paragraph Data.
    //   RETURN : Data가 구성되면 TRUE, 아니면 FALSE.  
    BOOL MakeSettlementData(int& iChapID, T_PARA_RPT& paraD);

    // 5.1.6. Grid Load Cases Data를 구성함.
    //   iChapID : Chapter index. Data가 구성되는 경우에는 iChapID++로 return.
    //   paraD : 구성된 Paragraph Data.
    //   RETURN : Data가 구성되면 TRUE, 아니면 FALSE.   
    BOOL MakeGridLoadData(int& iChapID, T_PARA_RPT& paraD);

    // 5.2.1. 하중조합 Case List Data를 구성함.
    //   iChapID : Chapter index. Data가 구성되는 경우에는 iChapID++로 return.
    //   paraD : 구성된 Paragraph Data.
    //   RETURN : Data가 구성되면 TRUE, 아니면 FALSE.   
    BOOL MakeLoadCombCaseData(int& iChapID, T_PARA_RPT& paraD);

    // 5.2.2. 하중조합 설명 Data를 구성함.
    //   iChapID : Chapter index. Data가 구성되는 경우에는 iChapID++로 return.
    //   paraD : 구성된 Paragraph Data.
    //   RETURN : Data가 구성되면 TRUE, 아니면 FALSE.   
    BOOL MakeLoadCombData(int& iChapID, T_PARA_RPT& paraD);

    // 6.1. 屆쨌졺살퀸皐箕壇제駱炬   
    BOOL MakeBeam_concrete_c(int& iChapID, T_PARA_RPT& paraD);

    // 6.2. 屆쨌졺櫓昑菉뇹숏壇제駱炬 
    BOOL MakeBeam_shear(int& iChapID, T_PARA_RPT& paraD);


    // 6.3. 屆쨌졺살퀸皐箕壇제駱炬 
    BOOL MakeBeam_rebar_s(int& iChapID, T_PARA_RPT& paraD);

    //6.4 屆쨌졺죙룹욱똑駱炬
    BOOL MakeBeam_crack(int& iChapID, T_PARA_RPT& paraD);

    //6.5 屆쨌졺 菊岑돔尖┒┭鏞?
    BOOL MakeBeam_shear_t(int& iChapID, T_PARA_RPT& paraD);

    // 6.6. 屆쨌졺苟訥鍍숏壇제駱炬
    BOOL MakeBeam_shear_b(int& iChapID, T_PARA_RPT& paraD);

    //////////////////////////////////////////////////////////////////////////
    // 6.7. 屆쨌料살퀸皐箕壇제駱炬
    BOOL MakeColumn_concrete_c(int& iChapID, T_PARA_RPT& paraD,BOOL seismic);

    // 6.8.屆쨌料살퀸皐寮윗壇제駱炬
    BOOL MakeColumn_shear(int& iChapID, T_PARA_RPT& paraD);
    
    // 6.9.屆쨌料멀쏘箕壇제駱炬
    BOOL MakeColumn_rebar_c(int& iChapID, T_PARA_RPT& paraD,BOOL seismic);

    // 6.10.屆쨌料멀쏘윗壇제駱炬
    BOOL MakeColumn_rebar_s(int& iChapID, T_PARA_RPT& paraD,BOOL seismic);

    // 6.11.屆쨌料죙룹욱똑駱炬
    BOOL MakeColumn_crack(int& iChapID, T_PARA_RPT& paraD);

    // 6.12.屆쨌料洸땍昑駱炬
    BOOL MakeColumn_stablity(int& iChapID, T_PARA_RPT& paraD,BOOL seismic);

	//예驢튤懃약駱炬
    BOOL MakeColumn_e(int& iChapID, T_PARA_RPT& paraD,BOOL seismic);

	//예驢儺昑궐駱炬
    BOOL MakeSeistic_duct(int& iChapID, T_PARA_RPT& paraD,BOOL seismic);

    
	// S***********************************************************
    // Add by GAY.('06.05.15). Get Data.
    // 현재 Data 파일의 날짜를 가져옴.
    //   RETURN : 날짜 형태를 "YY/MM" 의 형식으로 표기.
    CString GetFileDate();

    // 충격계수 값을 가져옴.
    CString GetImpactCoefficient();

    // Beam Section Temperature Load의 방향성 구함.
    //   iDir : 방향성을 나타내는 변수. (0=local-y, 1=local-z).
    CString GetBeamSectTempLoadDirection(int iDir);

    // Load Combination Active Type 을 String 형식으로 변환.
    //   iActiveType : Active Type.
    // ※ 참고 : wg_db > DB_ST_DT_CASE.h > struct T_LCOM_D
    CString GetLcombActiveType(int iActiveType);

    // Stage의 Activation Type을 String 형식으로 변환.
    //   iCondition : Activation Type.
    CString GetStageActivationType(int iCondition);
    
    // Stage의 Total Type을 String 형식으로 변환.
    //   iSummary : Total Type.
    CString GetStageTotalType(int iSummary);

    // 설계용 하중조합 Key를 입력받아 실제 하중조합의 이름 구함.
    //   lcomK : 설계용 하중조합.
    CString GetLoadCombName(T_LCOM_K lcomK);

    // Static Load Case의 이름 구함.
    //   stldK : Static Load Case의 Key.
    CString GetStaticLoadCaseName(T_STLD_K stldK);

    // 하중조합의 Type 을 String 형식으로 변환.
    //   iMaxType : 하중조합 Type (이동하중이 포함된 하중조합에서 최대, 최소 부재력 성분을 출력).
    CString GetLoadCombType(int iMaxType);

    // 위치의 Type을 String 형식으로 변환.
    //   iLocType : 위치 Type.
    CString GetTopBottomType(int iLocType);

    // Min/Max Type을 String 형식으로 변환.
    //   iMaxType : 최대/최소 Type.
    CString GetMaxMinType(int iMaxType);

    // 비틀림 검토시, Min/Max Type을 String 형식으로 변환.
    //   iMaxType : 최대/최소 Type.
    CString GetMaxMinTypeShear(int iMaxType);

    // 해당 stage에서의 Element Group List를 얻음.
    //   stagK : stage Key.
    //   arActiveK : Active Group List for Text Key Array.
    //   arDeactiveK : Deactive Group List for Text Key Array.
    void GetElementGroupList(T_STAG_K stagK, CArray<T_TEXT_K, T_TEXT_K>& arActiveK, CArray<T_TEXT_K, T_TEXT_K>& arDeactiveK);
    
    // 해당 stage에서의 Boundary Group List를 얻음.
    //   stagK : stage Key.
    //   arActiveK : Active Group List for Text Key Array.
    //   arDeactiveK : Deactive Group List for Text Key Array.
    void GetBoundaryGroupList(T_STAG_K stagK, CArray<T_TEXT_K, T_TEXT_K>& arActiveK, CArray<T_TEXT_K, T_TEXT_K>& arDeactiveK);

    // 해당 stage에서의 Load Group List를 얻음.
    //   stagK : stage Key.
    //   arActiveK : Active Group List for Text Key Array.
    //   arDeactiveK : Deactive Group List for Text Key Array.
    void GetLoadGroupList(T_STAG_K stagK, CArray<T_TEXT_K, T_TEXT_K>& arActiveK, CArray<T_TEXT_K, T_TEXT_K>& arDeactiveK);

    // 해당 숫자를 중국한자로 바꾸어줌.
    //   iNumber : 바꿀 숫자.
    CString GetNumberCharacterForChina(int iNumber);

    // 현재 Data 파일의 날짜 표기. (중국Version).
    //   iYY : 년도.
    //   iMM : 달.
    CString GetFileDateForChina(int iYY, int iMM);

    // Tendon의 재료특성치 가져옴.
    //   strTndnName : Tendon의 이름.
    //   dEc : 탄성계수 (For Code Unit). 
    //   dDensity : 단위중량 (For Code Unit). 
    //   dThermal : 선팽창계수.
    BOOL GetTendonMatlData(CString strTndnName, double& dEc, double& dDensity, double& dThermal);

    // 활하중 Type의 설명을 가져옴.
    //   strTypeName : 활하중 Type의 설명.
    CString GetLiveLoadTypeDescription(CString strTypeName);

    // 하중조합시, 각 하중조합에 쓰이는 기호의 설명.
    //   strCaseName : 하중조합시, 기호.
    CString GetLoadCaseDescription(CString strCaseName);

    // 하중조합의 기호 설명.  
    //   arLcomMap : 하중조합 기호 및 설명을 담고 있는 Map.
    void GetLoadCombinationCases(CMap<CString, LPCTSTR, CString, CString&>& mapLcom);

    // 시공단계 하중 Case의 기호 얻어옴.
    //    sgldK : 시공단계 하중 Case의 key.
    //  ※ 참고 : CLoadCombCtrl::CheckCS_SgldK(...).
    CString GetConstructionStageLoadCaseSymbol(T_SGLD_K sgldK);

    // 온도하중 조건이 존재하는가..  
    BOOL CheckExistTemperatureLoad();

    // Add by GAY. MNET:2444. ('06.09.08).
    // 검토결과 Table을 출력할 Item이 존재하는가..
    BOOL CheckExistResultTableItem();
    
    // E***********************************************************

    // S***********************************************************
    // Add by GAY.('06.05.13). Some Helper Methods.
    
    // Chapter Data를 구성함.
    //   iLevel  : Chapter의 Level. ex) 1.=1, 1.1=2, 1.1.1=3, ...
    //   iChapID : Chapter의 index.
    //   strText : Chapter의 내용.
    //   cssK    : CSS의 Key.
    T_CHAP_RPT MakeChapter(int iLevel, int iChapID, CString strText, T_CSS_K cssK=0);

    // E***********************************************************
 
    CString GetModifyString(int iMaxLength, CString strText);

    T_TD_K AddTDwithUnit(LPCTSTR strText, CString strUnit, int iTDType=0, int iMergeCols=1, int iMergeRows=1, T_CSS_K CssK=0);
    T_TD_K AddTDwithUnit(LPCTSTR strText, CArray<T_CHAR_K, T_CHAR_K>& arCharUnitK, int iTDType=0, int iMergeCols=1, int iMergeRows=1, T_CSS_K CssK=0);
    T_TD_K AddTDwithUnit(CArray<T_CHAR_K, T_CHAR_K>& arCharK, CString strUnit, int iTDType=0, int iMergeCols=1, int iMergeRows=1, T_CSS_K CssK=0);
    T_TD_K AddTDwithUnit(CArray<T_CHAR_K, T_CHAR_K>& arCharK, CArray<T_CHAR_K, T_CHAR_K>& arCharUnitK, int iTDType=0, int iMergeCols=1, int iMergeRows=1, T_CSS_K CssK=0);  

    
protected:

    CString m_strForceN;
    CString m_strMomentN;
    
    double m_dForcekN; // 0.001
    double m_dMomentkNM; // 0.000001
    double m_dLengthM;
    double m_dDensitykNM;
    
    CString m_strForceUnit;
    CString m_strForceUnitN;
    CString m_strMomentUnit;
    CString m_strMomentUnitN;
    CString m_strLengthUnit;
    CString m_strLengthUnitM;
    CString m_strAngleUnitRad;
    CString m_strStressUnit;  
    CString m_strDensityUnit;
    CArray<UINT,UINT> m_arstrAreaUnit;
    CArray<UINT,UINT> m_arstrStifUnit;
    CArray<UINT,UINT> m_arstrSectUnit;
    CArray<UINT,UINT> m_arStressUnit;


// 현 보고서에만 사용되는 스타일 추가
protected:
    T_CSS_K m_TextPadding1CssK;     //들여쓰기 1칸을 나타내는 스타일
    T_CSS_K m_TextPadding2CssK;     //들여쓰기 2칸을 나타내는 스타일
    T_CSS_K m_TextValueCssK;        //Text의 내용중 변수값에 해당되는 부분을 나타내는 스타일
    T_CSS_K m_TextPicCaptionCssK;   // 그림의 캡션을 나타내는 스타일
    T_CSS_K m_TDCenterCssK;         // 가운데 정렬된 셀을 나타내는 스타일

    CArray<T_CHAR_K, T_CHAR_K> m_arCharK_TndnComp;
    CArray<T_CHAR_K, T_CHAR_K> m_arCharK_MnConR;
    CArray<T_CHAR_K, T_CHAR_K> m_arCharK_MnConT;
    CArray<T_CHAR_K, T_CHAR_K> m_arCharK_MnStl;
    CArray<T_CHAR_K, T_CHAR_K> m_arCharK_MnPs;

    CArray<T_CHAR_K, T_CHAR_K> m_arCharK_MnStlT;
    CArray<T_CHAR_K, T_CHAR_K> m_arCharK_MnPsT;

    CArray<T_CHAR_K, T_CHAR_K> m_arCharK_MnStlT2;
    CArray<T_CHAR_K, T_CHAR_K> m_arCharK_MnPsT2;
    CArray<T_CHAR_K, T_CHAR_K> m_arCharK_MnPs2;

    CArray<T_CHAR_K, T_CHAR_K> m_arCharK_Vn;
    CArray<T_CHAR_K, T_CHAR_K> m_arCharK_SectChk;
    CArray<T_CHAR_K, T_CHAR_K> m_arCharK_LoadChk;
    CArray<T_CHAR_K, T_CHAR_K> m_arCharK_Vcs;
    CArray<T_CHAR_K, T_CHAR_K> m_arCharK_Vsb;
    CArray<T_CHAR_K, T_CHAR_K> m_arCharK_Vpb;

    CArray<T_CHAR_K, T_CHAR_K> m_arCharK_PureTn1;
    CArray<T_CHAR_K, T_CHAR_K> m_arCharK_PureTn2;
    CArray<T_CHAR_K, T_CHAR_K> m_arCharK_Zeta;
    CArray<T_CHAR_K, T_CHAR_K> m_arCharK_VTLoad;
    CArray<T_CHAR_K, T_CHAR_K> m_arCharK_TorSectChk;
    CArray<T_CHAR_K, T_CHAR_K> m_arCharK_TorLoadChk;
    CArray<T_CHAR_K, T_CHAR_K> m_arCharK_TorVn;
    CArray<T_CHAR_K, T_CHAR_K> m_arCharK_Tn;
    CArray<T_CHAR_K, T_CHAR_K> m_arCharK_TnBox;

protected:

    void InitialarCharK();
    
    BOOL GetDesignStringPara(CStringArray& arrString);
    BOOL GetMaterialProperty();
    /*
    double Cng_LenFixM(double dLength, int iUp);
    double Cng_MomFix(double dMoment);
    double Cng_CodeLenFixM(double dLength, int iUp);
    double Cng_CodeMomFix(double dMoment);
    */

    // Result Detail.
	//BOOL Get_DetailTbarPos(T_ELEM_K ElemK);
	//BOOL Get_DetailTbarNeg(T_ELEM_K ElemK);
	BOOL Get_DetailBdcyPos(T_ELEM_K ElemK);
    BOOL Get_DetailBdcyNeg(T_ELEM_K ElemK);
	//BOOL Get_DetailCums(   T_ELEM_K ElemK);
	//BOOL Get_DetailCrmt(   T_ELEM_K ElemK);
    
    
};

#include "HeaderPost.h"

#endif // !defined(AFX_RPTTB_10002_3_05_CH_H__INCLUDED_)
