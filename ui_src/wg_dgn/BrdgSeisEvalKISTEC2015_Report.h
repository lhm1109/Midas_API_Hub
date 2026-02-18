#if !defined(_SEIS_EVAL_XLOUT_H__)
#define _SEIS_EVAL_XLOUT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dgn_XLOut.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\BrdgSeisEvalStruct.h"
#include "BrdgSeisEvalKISTEC2015.h"
#include "DgnSeisMPhiCurveDlg.h"
#include "DgnSeisFailureModeDlg.h"

#include "HeaderPre.h"

struct _DGN_SEISEVAL_SHEAR_OUT
{
    T_EVGP_K EvgpK;
    UINT     iColm;
    CString  strEvgpName;
    EN_PIER_TYPE     enPierType[2]; // Longi, Trans
    EN_BEHAVIOR_TYPE enBehavior[2]; // Longi, Trans
    double dVc2[4];
    double dVc5[4];
    double dVs[4];
    double dVp[4];
    T_BRDGEVAL_RESULT ResultD[2][2];

    void initialize()
    {
        EvgpK = 0;
        iColm = 0;
        strEvgpName.Empty();
        
        for( int i=0; i<2; ++i )
        {
            enPierType[i] = EN_PIER_TYPE::NONE;
            enBehavior[i] = NONE_TYPE;
            for( int j=0; j<2; ++j )
            {
                int iIdx = i*2+j;
                dVc2[iIdx] = 0.0;
                dVc5[iIdx] = 0.0;
                dVs [iIdx] = 0.0;
                dVp [iIdx] = 0.0;
                ResultD[i][j].initialize();
            }
        }
    }

    _DGN_SEISEVAL_SHEAR_OUT(T_EVGP_K ValK, int ValColm, CString strVal, EN_PIER_TYPE enPire1, EN_PIER_TYPE enPire2, EN_BEHAVIOR_TYPE enBeha1, EN_BEHAVIOR_TYPE enBeha2,
                            double dVval1[4], double dVval2[4], double dVval3[4], double dVval4[4], T_BRDGEVAL_RESULT resD[2][2])
    {
        EvgpK = ValK;
        iColm = ValColm;
        strEvgpName = strVal;
        enPierType[0] = enPire1;
        enPierType[1] = enPire2;
        enBehavior[0] = enBeha1;
        enBehavior[1] = enBeha2;

        for( int i=0; i<2; ++i )
        {   
            for( int j=0; j<2; ++j )
            {
                int iIdx = i*2+j;
                dVc2[iIdx] = dVval1[iIdx];
                dVc5[iIdx] = dVval2[iIdx];
                dVs [iIdx] = dVval3[iIdx];
                dVp [iIdx] = dVval4[iIdx];
                ResultD[i][j] = resD[i][j];
            }
        }
    }

    _DGN_SEISEVAL_SHEAR_OUT& operator= (const _DGN_SEISEVAL_SHEAR_OUT& src)
    {
        EvgpK = src.EvgpK;
        iColm = src.iColm;
        strEvgpName = src.strEvgpName;

        for( int i=0; i<2; ++i )
        {
            enPierType[i] = src.enPierType[i];
            enBehavior[i] = src.enBehavior[i];
            for( int j=0; j<2; ++j )
            {
                int iIdx = i*2+j;
                dVc2[iIdx] = src.dVc2[iIdx];
                dVc5[iIdx] = src.dVc5[iIdx];
                dVs [iIdx] = src.dVs [iIdx];
                dVp [iIdx] = src.dVp [iIdx];
                ResultD[i][j].initialize();
            }
        }

        return *this;
    }
};

struct _DGN_SEISEVAL_SuppLengD
{
    CString strName;
    double dNC;
    double dND;
    double dRat;

    _DGN_SEISEVAL_SuppLengD()
    {
        strName.Empty();
        dNC = 0.0;
        dND = 0.0;
        dRat = 0.0;
    }
    _DGN_SEISEVAL_SuppLengD(const CString& strVal1, double dVal2, double dVal3, double dVal4)
    {
        strName = strVal1;
        dNC = dVal2;
        dND = dVal3;
        dRat = dVal4;
    }
    _DGN_SEISEVAL_SuppLengD& operator=(const _DGN_SEISEVAL_SuppLengD& rD)
    {
        strName	= rD.strName;
        dNC	    = rD.dNC	;
        dND		= rD.dND	;
        dRat	= rD.dRat	;

        return *this;
    }
};

//class CDBDoc;
class __MY_EXT_CLASS__ CBrdgSeisEvalKISTEC2015_Report : public CBrdgSeisEvalKISTEC2015, public CDgn_XLOut
{
public:
    CBrdgSeisEvalKISTEC2015_Report(void);
    virtual ~CBrdgSeisEvalKISTEC2015_Report(void);

protected:
    CDBDoc* m_pDoc;
    BOOL m_bPrintItems[8];

public:
    void Print_SeisEval(CString strPath, int iPrintOpt);
    void SetPrintItems(BOOL bPrintItems[8]);

private:
    void SetCoverSheetPage(dgn::lib::IExcel* pExcel); // 표지
    void SetSummarySheetPage(dgn::lib::IExcel* pExcel, int& iIdx); //결과요약
    void SetBrdgInfoSheetPage(dgn::lib::IExcel* pExcel, int& iIdx); //교량현황
    void SetStructAnalSheetPage(dgn::lib::IExcel* pExcel, int& iIdx); //구조해석
    void SetNaturalFrequencySheetPage(dgn::lib::IExcel* pExcel, int& iIdx); //탄성지진력
    void SetMixedModeSheetPage(dgn::lib::IExcel* pExcel, int& iIdx); //복합모드
    void SetConditionSheetPage(dgn::lib::IExcel* pExcel, const CString& strName, int& iIdx); //검토조건
    void SetEvaluationSheetPage(dgn::lib::IExcel* pExcel, const CString& strName, const int& iIdx, int& iColIdx); //내진성능평가
    void DelDefaultSheetPage(dgn::lib::IExcel* pExcel);

    void SetBearingEvaluationSheetPage(dgn::lib::IExcel* pExcel, const CString& strName, const int& iIdx, int& iCntIdx);
    void SetBearingLengthEvaluationSheetPage(dgn::lib::IExcel* pExcel, int& iIdx);

protected:

    //결과 요약 부분
    BOOL GenerateSummarySheet_Info(dgn::lib::IExcel* pExcel, int& stRow);
    BOOL GenerateSummaryPierTableHead(dgn::lib::IExcel* pExcel, int& stRow, const CString& strEvgp);
    BOOL GenerateSummaryPierTableBody(dgn::lib::IExcel* pExcel, int& stRow, const PairKey& pEvgpCol, const CString& strColm);
    BOOL GenerateSummaryBearingTable(dgn::lib::IExcel* pExcel, int& stRow, const T_EVGP_K& EvgpK, const T_BREV_K& BrevK);

    //교량 현황 부분

    BOOL GenerateBrdgInfoSheet(dgn::lib::IExcel* pExcel, T_SBRI_D& BrdgInfo, int& stRow);
    BOOL GenerateBrdgStrengthInfoSheet(dgn::lib::IExcel* pExcel, int& stRow, const int& iStartStrength, double& dFck, double& dFy, const CString& strEvgp);
    BOOL GenerateBrdgElasticInfoSheet(dgn::lib::IExcel* pExcel, int& stRow, const int& iStartStrength,  double& dElastCon, double& dElastStl, const CString& strEvgp);
    BOOL GenerateBrdgElseInfoSheet(dgn::lib::IExcel* pExcel, int& stRow, const T_FIMP_D& FimpConcD1, const T_FIMP_D& FimpConcD3, const T_FIMP_D& FimpSteelD, const UINT& uncode);

    //구조해석 부분

    BOOL GenerateStructAnalCommonInfo(dgn::lib::IExcel* pExcel, int& stRow);
    BOOL GenerateStructAnalPCSupportInfo(dgn::lib::IExcel* pExcel, int& stRow, int iNumEVGP, const int& iMaxLink, const CArray<T_NODE_K, T_NODE_K>& aLink, const UINT& EvgpK, const UINT& iColm, const CString& strEvgp);
    BOOL GenerateStructAnalPCBotInfo(dgn::lib::IExcel* pExcel, int& stRow, T_BRDGEVAL_RESULT pResultD[2][2], int iNumEVGP, int& nPrtTitle, const CString& strEvgp);

    //탄성지진력 부분

    BOOL GenerateNaturalFrequencySheetRS(dgn::lib::IExcel* pExcel);
	BOOL GenerateNaturalFrequencySheetTH(dgn::lib::IExcel* pExcel);
	BOOL GenerateNaturalFrequencyMassTable(dgn::lib::IExcel* pExcel, int& stRow);

    //복합모드 부분

    BOOL GenerateMixedModeRSInfo(dgn::lib::IExcel* pExcel, int& stRow, const double& dAxial, const UINT& EvgpK, const UINT& iColm, int iNumEVGP, T_BRDGEVAL_RESULT pResultD[2][2], const CString& strEvgp);
    BOOL GenerateMixedModeCombInfo(dgn::lib::IExcel* pExcel, int& stRow, const double& dAxial, const UINT& EvgpK, const UINT& iColm, T_BRDGEVAL_RESULT pResultD[2][2], const CString& strEvgp);
    BOOL GenerateMixedModeEnvCombInfo(dgn::lib::IExcel* pExcel, int& stRow, const double& dAxial, const UINT& EvgpK, const UINT& iColm, T_BRDGEVAL_RESULT pResultD[2][2], const CString& strEvgp, const int nLoadType);

    //검토조건 부분

    BOOL GenerateStructInfo(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, const double* dB, const double* dH, T_BRDGEVAL_RESULT pResultD[2][2], T_MPCC_D* pMpccD, const T_RCHK_D& RchkData, const T_REBT_D& GenRbarD, const CString& strName, bool bPlainConc=false);
    BOOL GenerateStructInfoBot(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, const double* dB, const double* dH, T_BRDGEVAL_RESULT pResultD[2][2], T_MPCC_D* pMpccD, const T_RCHK_D& RchkData, const T_REBT_D& GenRbarD, const CString& strName, bool bPlainConc=false);
    BOOL GenerateStructInfoTop(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, const double* dB, const double* dH, T_BRDGEVAL_RESULT pResultD[2][2], T_MPCC_D* pMpccD, const T_RCHK_D& RchkData, const T_REBT_D& GenRbarD, const CString& strName, bool bPlainConc=false);

    BOOL GenerateRebarBaseShapeInfo(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const double* dB, const double* dH, T_BRDGEVAL_RESULT pResultD[2][2], const EN_PIER_TYPE* enPierType);
    BOOL GenerateRebarBaseShapeInfoTop(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const double* dB, const double* dH, T_BRDGEVAL_RESULT pResultD[2][2], const EN_PIER_TYPE* enPierType);

    BOOL GenerateAssumptions(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const double* dT);
    BOOL GenerateAssumptionsTop(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const double* dT);

    BOOL GenerateMaterialModelInfo(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const T_FIMP_D& FimpConcD1, const T_FIMP_D& FimpConc2D1, const T_FIMP_D& FimpConcD3, const T_FIMP_D& FimpConc2D3, const T_FIMP_D& FimpSteelD, T_BRDGEVAL_RESULT pResultD[2][2], T_MPCC_D* pMpccD);

    //내진성능평가 부분

    BOOL GenerateEvalSheet(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const EN_PIER_TYPE* enPierType, T_BRDGEVAL_RESULT pResultD[2][2]);

    BOOL GenerateIdealMomentPhi(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT pResultD[2][2], T_MPCC_D& MpccD1, T_MPCC_D& MpccD2, T_MPCC_K MpccK1, T_MPCC_K MpccK2);
    BOOL GenerateHe(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const T_EVPM_D& EvmpD);
    BOOL GeneratePeriod(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, T_BRDGEVAL_RESULT pResultD[2][2]);
    BOOL GenerateEffectiveYield(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT pResultD[2][2]);
    BOOL GenerateEffectiveSect(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT pResultD[2][2]);
    virtual BOOL GenerateEffectiveStiffJudgement(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT pResultD[2][2]);

    BOOL GenerateBendingStress(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT pResultD[2][2]);
    BOOL GenerateBendingDuct2015(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT pResultD[2][2]);
    BOOL GenerateBendingDuct2012(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT pResultD[2][2]);
    BOOL GenerateBendingResult(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT pResultD[2][2], const UINT& EvgpK, const UINT& iColm, const CString& strName);

    BOOL GenerateShearResult(dgn::lib::IExcel* pExcel,  CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut);
    BOOL GenerateShearResult_Longi(dgn::lib::IExcel* pExcel,  CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut);
	BOOL GenerateShearResult_Longi_Mod(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut);
    BOOL GenerateShearResult_Longi_Wall(dgn::lib::IExcel* pExcel,  CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut, int nLongTran = 0);
	BOOL GenerateShearResult_Longi_Wall_byTranData(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut);
    BOOL GenerateShearResult_Longi_Else(dgn::lib::IExcel* pExcel,  CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut, int nLongTran = 0);
	BOOL GenerateShearResult_Longi_Else_byTranData(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut);
    BOOL GenerateShearResult_Longi_Else_Conc(dgn::lib::IExcel* pExcel,  CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut, int nLongTran = 0);
    BOOL GenerateShearResult_Longi_Else_Rebar(dgn::lib::IExcel* pExcel,  CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut, int nLongTran = 0);
    BOOL GenerateShearResult_Longi_Else_Load(dgn::lib::IExcel* pExcel,  CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut, int nLongTran = 0);
    BOOL GenerateShearResult_Longi_Strn(dgn::lib::IExcel* pExcel,  CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut);
    BOOL GenerateShearResult_Longi_Strn_Wall(dgn::lib::IExcel* pExcel,  CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut);
    BOOL GenerateShearResult_Longi_Strn_Else(dgn::lib::IExcel* pExcel,  CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut);
    BOOL GenerateShearResult_Trans(dgn::lib::IExcel* pExcel,  CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut);
	BOOL GenerateShearResult_Trans_Mod(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut);
    BOOL GenerateShearResult_Trans_Wall(dgn::lib::IExcel* pExcel,  CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut, int nLongTran = 1);
	BOOL GenerateShearResult_Trans_Wall_byLongiData(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut);
    BOOL GenerateShearResult_Trans_Else(dgn::lib::IExcel* pExcel,  CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut, int nLongTran = 1);
	BOOL GenerateShearResult_Trans_Else_byLongiData(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut);
    BOOL GenerateShearResult_Trans_Else_Conc(dgn::lib::IExcel* pExcel,  CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut, int nLongTran = 1);
    BOOL GenerateShearResult_Trans_Else_Rebar(dgn::lib::IExcel* pExcel,  CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut, int nLongTran = 1);
    BOOL GenerateShearResult_Trans_Else_Load(dgn::lib::IExcel* pExcel,  CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut, int nLongTran = 1);
    BOOL GenerateShearResult_Trans_Strn(dgn::lib::IExcel* pExcel,  CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut);
    BOOL GenerateShearResult_Trans_Strn_Wall(dgn::lib::IExcel* pExcel,  CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut);
    BOOL GenerateShearResult_Trans_Strn_Else(dgn::lib::IExcel* pExcel,  CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut);

    BOOL GenerateShearResult_Longi_Wall_2019(dgn::lib::IExcel* pExcel,  CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut, int nLongTran = 0);
	BOOL GenerateShearResult_Longi_Wall_2019_byTranData(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut);

    BOOL GenerateFailureBegin(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, const EN_FAILURE_MODE* enFailure, T_BRDGEVAL_RESULT pResultD[2][2]);

    BOOL GenerateFailureMode(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, const EN_FAILURE_MODE* enFailure, T_BRDGEVAL_RESULT pResultD[2][2], const T_MPCC_D& MpccD);

    BOOL GenerateEvalResult(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, const EN_FAILURE_MODE* enFailure, T_BRDGEVAL_RESULT pResultD[2][2]);
    BOOL GenerateEvalResultTable(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, const EN_FAILURE_MODE* enFailure, T_BRDGEVAL_RESULT pResultD[2][2]);
       
    virtual BOOL GenerateEvalPlainConcSheet(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT pResultD[2][2]);

    //이미지 프린트 하는 부분

    BOOL PrintSectionFig(dgn::lib::IExcel* pExcel, CString& strPicPath, CString& strSectFigFileName, int& stRow);
    BOOL PrintConcreteGraph(dgn::lib::IExcel* pExcel, int nConType, int& stRow);
    BOOL PrintSteelGraph(dgn::lib::IExcel* pExcel, int nSteelType, int& stRow);
    BOOL PrintModelView(dgn::lib::IExcel* pExcel, CString& strModelViewName, int& stRow);
    BOOL PrintIdealGraph(dgn::lib::IExcel* pExcel, int nOption, int& stRow);
    BOOL PrintMphiGraph(dgn::lib::IExcel* pExcel, CString& strMPhiFileName, const int& stRow);
    BOOL PrintBearingFig(dgn::lib::IExcel* pExcel, CString& strMPhiFileName, const int& stRow);
    BOOL PrintBearingProjFig(dgn::lib::IExcel* pExcel, CString& strMPhiFileName, const int& stRow);

    BOOL PrintMomentShearGraph(dgn::lib::IExcel* pExcel, CString& strMomentShearFileName, const int& stRow, const int& stCol);
    BOOL PrintMomentShearFailureGraph(dgn::lib::IExcel* pExcel, CString& strMomentShearFileName, const int& stRow);

    CArray<CString, CString> m_arDetailSheetName;
    void SetDetailSheetPage(dgn::lib::IExcel* pExcel, CString strNameCurrent);

    BOOL Make_SectionFig(CString strFileName, T_MPCC_D* pMpccD);
    BOOL WaitForFile(CString strFileName);

    virtual void GetMPhiGraphFileNames(CString aResName[3]);

    void GetCellIDByName(dgn::lib::IExcel* pExcel, CString sRangeName, short& rowCnt, short& colCnt);

private:
    dgn::lib::IExcel*  SetExcel();
    BOOL     GetResult(const PairKey& pEvgpColm, T_BRDGEVAL_RESULT rData[2][2]);
    INT_PTR  GetPrintList(CArray< PairKey, PairKey& >& raEvgpColm);
    INT_PTR  GetBearingPrintList(CArray<UINT,UINT>& raEvgpBearing, CArray<UINT,UINT>& raBrev);
    CString Get_PierColmName(const T_EVGP_D& EvgpD, const int& iColIdx);
    CString Get_PierColmNameMixed(const T_EVGP_D& EvgpD, const int& iColIdx);

    BOOL Print_CoverTab      (dgn::lib::IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot);
    BOOL Print_SummaryTab    (dgn::lib::IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot, 
                              int& iSheetIdx, arPairKey& arEvgpColm, CArray<UINT,UINT>& arEvgpBearing, CArray<UINT,UINT>& arBrev);
    BOOL Print_BrdgStateTab  (dgn::lib::IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot,
                              int& iSheetIdx, arPairKey& arEvgpColm, CArray<UINT,UINT>& arEvgpBearing);
    BOOL Print_AnalResTab    (dgn::lib::IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot,
                              int& iSheetIdx, arPairKey& arEvgpColm);
	BOOL Print_ElSuppTab(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot,
							  int& iSheetIdx, int nLoadType);
    BOOL Print_ComplexModeTab(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot,
                              int& iSheetIdx, arPairKey& arEvgpColm, int nLoadType);
    BOOL Print_CndtNResTab   (dgn::lib::IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot,
                              int& iSheetIdx, arPairKey& arEvgpColm);

    BOOL Print_BearingTab(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot, int& iSheetIdx, CArray<UINT,UINT>& arEvgpBearing, CArray<UINT,UINT>& arBrev);
    BOOL Print_BearingSuppLengTab(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot, int& iSheetIdx, CArray<UINT,UINT>& arEvgpBearing, CArray<UINT,UINT>& arBrev);

    void Get_AnchorTypeName(const EN_ANCH_TYPE& enType, CString& strType1, CString& strType2);
    
    // for Bearing
    BOOL Print_BearingBody              (dgn::lib::IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot, const T_BRDGEVAL_BEARING& ResD, int& stRow);
    virtual BOOL Print_BearingAnchorStl (dgn::lib::IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot, const T_BREV_K& BrevK, const T_BRDGEVAL_BEARING& ResD, int& stRow);
    BOOL Print_BearingAnchorStl_Single  (dgn::lib::IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot, const T_BREV_K& BrevK, const T_BRDGEVAL_BEARING& ResD, int& stRow);
    BOOL Print_BearingAnchorStl_Multi   (dgn::lib::IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot, const T_BREV_K& BrevK, const T_BRDGEVAL_BEARING& ResD, int& stRow);
    BOOL Print_BearingAnchorConc        (dgn::lib::IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot, const T_BREV_K& BrevK, const T_BRDGEVAL_BEARING& ResD, int& stRow);
    BOOL Print_BearingAnchorPryout      (dgn::lib::IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot, const T_BREV_K& BrevK, const T_BRDGEVAL_BEARING& ResD, int& stRow);
    BOOL Print_BearingAnchorSummary     (dgn::lib::IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot, const T_BRDGEVAL_BEARING& ResD, int& stRow);

    BOOL Print_BearingSuppLen       (dgn::lib::IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot, const T_BRDGEVAL_BEARING& ResD, int& stRow);
    BOOL Print_BearingSuppLenSummary(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot, const CArray<_DGN_SEISEVAL_SuppLengD, _DGN_SEISEVAL_SuppLengD&>& arResD, int& iIdx, int& stRow);
    
    // KISTEC2019
    BOOL Print_BearingAnchorTenStl(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot, const T_BRDGEVAL_BEARING& ResD, int& stRow);
    BOOL Print_BearingAnchorTenConc(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot, const T_BRDGEVAL_BEARING& ResD, int& stRow);
    BOOL Print_BearingAnchorTenPull(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot, const T_BRDGEVAL_BEARING& ResD, int& stRow);
    BOOL Print_BearingAnchorTenSide(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot, const T_BRDGEVAL_BEARING& ResD, int& stRow);
    BOOL Print_BearingAnchorTenRela(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot, const T_BRDGEVAL_BEARING& ResD, int& stRow);

    BOOL IsPrintMark(const double& dVal1, const double& dVal2);
};

#include "HeaderPost.h"

#endif // !defined(_SEIS_EVAL_XLOUT_H__)