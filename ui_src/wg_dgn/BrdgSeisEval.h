#pragma once
#include "..\wg_db\BrdgSeisEvalStruct.h"

#include "DgnMomentCurvatureDesign.h"

using namespace std;

typedef pair<UINT,UINT> PairKey;
typedef CArray<PairKey,PairKey&> arPairKey;

enum SAFE_DIV_RETURN
{
    _ZERO=0,
    _INFI2,
    _INFI4
};

struct TConcFailBcCalc;

class CBrdgSeisEval
{
public:
    CBrdgSeisEval(void);
    virtual ~CBrdgSeisEval(void);

public:
    BOOL Get_MPhiResult(const T_MPCC_K& Key, T_BRDGEVAL_MPHI_RES mphiResD[2], BOOL bRecal=TRUE);
	BOOL Get_MPhiResult(const T_MPCC_D& MpccD, T_BRDGEVAL_MPHI_RES mphiResD[2], BOOL bRecal = TRUE, double dAddAngle = 0.0);

    int CalcMomentCurvature(const T_MPCC_K& MpccK, BOOL bSave=FALSE);
	int CalcMomentCurvature(const T_MPCC_D& MpccD, BOOL bSave=FALSE, double dAngle = 0.0);
    void CalcMomentCurvature(CArray<T_MPCC_K, T_MPCC_K>& arMpccK, BOOL bSave=FALSE);

    BOOL Print_MPhiResult(CString strFileName, CArray<T_MPCC_D, T_MPCC_D&>& arMpccD);
    BOOL Print_MPhiResult(CString strFileName, CArray<T_MPCC_K, T_MPCC_K>& arMpccK);

    BOOL GetFailureData(const UINT& EvgpK, const UINT& iColm, const EN_LOCAL_DIR& enDir, const EN_POSITION& enPos, T_BRDGEVAL_FAILURE_MODE& FModeD, T_BRDGEVAL_CAPACITY& CapaD);

    //Typical인 경우인데 상대각도가 0이 아니면 Typical이 아닌 경우로 지정합니다.
	BOOL   IsTypicalBearingArrange(const T_BREV_K& BrevK, double dRelAngle = 0.0);
	BOOL   IsTypicalBearingArrange(const T_ANEV_D& AnevD, double dRelAngle = 0.0);

    virtual void CalcIeff(const T_MPCC_D& MpccD, double dIeffRat[2], T_BRDGEVAL_RESULT Result[2], BOOL bRecal=TRUE)=0; // 유효강성비 계산
    virtual void CalcHe(const T_MPCC_K& MpccTopK, const T_MPCC_K& MpccBotK, const EN_LOCAL_DIR& enDir, const EN_BEHAVIOR_TYPE& enBehavior, const int& nCalcType, const double& dHc, double dHe[2])=0; // 유효높이 계산
    virtual BOOL DoSeisEval()=0;
    virtual void DoSeisEvalBearing() = 0;
	virtual double Get_WallVn(const T_BRDGEVAL_SHR_CAPA& ShearCapaD)=0;
    virtual void CalcPlainConcCapacity(const UINT& EvgpK, const UINT& iColm, const EN_LOCAL_DIR& enDir, const EN_POSITION& enPos, const EN_FAILURE_MODE& enFalureMode, const T_BRDGEVAL_CALC_PARAM& ParamD, T_BRDGEVAL_EVALUATION& EvalD)=0;

    virtual BOOL Get_BearingParam(const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD, BOOL bDesign, BOOL bAnchorDirPosi=TRUE)=0; //bDesign=FALSE면 설계 부분 사용 안함
    virtual BOOL Calc_BearingAnchorForDlg(int nFailType, T_ANEV_D& AnevD, T_BRDGEVAL_BEARING& rResD)=0;
    virtual double Calc_BearingAnchor_Concrete_Ratio(T_ANEV_D& AnevD, T_BREV_D& BrevD, TConcFailBcCalc& MinConcFailCal, std::vector<std::pair<int, int>> aRepBearingIdx, T_BRDGEVAL_BEARING& rResD, const int& iDir, int nRatType[4])=0;
    virtual double Calc_BearingAnchor_Pryout_Ratio(T_ANEV_D& AnevD, T_BREV_D& BrevD, int nRepresentGrpIdx, std::vector<std::pair<int, int>> aRepBearingIdx, T_BRDGEVAL_BEARING& rResD, const int& iDir, int nRatType[4], BOOL bAnchorTenConc=FALSE)=0;
    virtual BOOL Check_Inter3Faces(double& dca1, const double& dca2, const double& dca2p, const double& dhcop, const double& dsa, const double& dLp) = 0;

    template<typename T> T SafeDiv( const T& num, const T& den, SAFE_DIV_RETURN enType=_ZERO)
    {
        if(fabs(den) < cDGN_Zero)
        {
            switch (enType)
            {
            case _ZERO:
                return static_cast<T>(0.0);
                break;
            case _INFI2:
                return static_cast<T>(99.99);
                break;
            case _INFI4:
                return static_cast<T>(9999.99);
                break;
            default:
                break;
            }
        }

        return static_cast<T>(num / den);
    };

protected:
    BOOL GetMatlData(const UINT& ElemK, T_MATD_D& MatlD);
    UINT GetSectKey(const UINT& ElemK);
    BOOL GetSectData(const UINT& ElemK, T_SECT_D& SectD);
    void GetCalcParam(const UINT& iGrup, const UINT& iColm, const EN_LOCAL_DIR& enDir, const EN_POSITION& enPos, const bool& bPlainConc, T_BRDGEVAL_CALC_PARAM& CalcParamD);
    void GetCalcParam_Force(const UINT& MpccK, const UINT& iColm, const EN_LOCAL_DIR& enDir, const UINT& LcomK, const int& nMinMax, T_BRDGEVAL_CALC_PARAM& CalcParamD);
    void GetCalcParam_Force(const UINT& EvgpK, const UINT& iColm, const EN_LOCAL_DIR& enDir, const EN_POSITION& enPos, const UINT& LcomK, const int& nMinMax, T_BRDGEVAL_CALC_PARAM& CalcParamD);
    void SetCalcParam_AxialForce(const double& dPcol, const double& dPcoltop, const double& dPcolbot, T_BRDGEVAL_FORCE& ForceD);

    EN_BEHAVIOR_TYPE GetBehaviorType(const UINT& iGrup, const UINT& iColm, const EN_LOCAL_DIR& enDir);
    UINT GetMpccKey(const UINT& iGrup, const UINT& iColm, const EN_POSITION& enPos);

    void Write_GDisplayAnalString(CString strTxt, BOOL bClearPre=TRUE, BOOL bSpacePost=TRUE);

    // Calculate Force
    double Calc_PierCapSuppEnvAxialForce(const T_EVGP_K& EvgpK, const int& iColm, CMapEx<T_ELNK_K, T_ELNK_K, double, double&>& mapElnkRect, CMapEx<T_NLNK_K, T_NLNK_K, double, double&>& mapNlnkRect, BOOL bReport=FALSE); // Ptop; 교각 상부 축력
    double Calc_PierColmBotEnvAxialForce(const T_EVGP_K& EvgpK, const UINT& iColm); // Pcolbot; 기둥 하부 축력
    double Calc_PierColmTopEnvAxialForce(const T_EVGP_K& EvgpK, const UINT& iColm); // Pcoltop; 기둥 상부 축력
    void Calc_PierSeisEnvForce(const T_EVGP_K& EvgpK, const UINT& iColm, const EN_POSITION& enPos, const int& nType, T_BRDGEVAL_FORCE& ForceD); // Evgp위치에 따라 Mpcc가 정의되므로 - 0:evn, 1:Longi, 2:Trans, 3:Vert
    void Calc_PierSeisEnvDisp(const T_EVGP_K& EvgpK, const UINT& iColm, const EN_LOCAL_DIR& enDir, const int& nType, double& dMaxDisp, UINT& MaxLinK, BOOL bBearing=FALSE, double dTheta=0.0); // Evgp위치에 따라 Mpcc가 정의되므로 - 0:evn, 1:Longi, 2:Trans, 3:Vert

    T_MPCC_K Get_GovernMpcc(const T_EVGP_K& EvgpK, const UINT& iColm);
    BOOL Get_GravityLoadCase(const T_EVGP_K& EvgpK, const UINT& iColm, int& nLoadCaseType, int& nLoadCaseKey);
    int  Get_GravityLoadCase(const T_EVGP_K& EvgpK, int& nLoadCaseType, int& nLoadCaseKey);
    BOOL Set_Lcom4Post(const int& nLoadCaseType, const int& nLoadCaseKey);
    BOOL Set_Lcom4Post(const T_EVGP_K& EvgpK, const UINT& iColm);
    int  Set_Lcom4Post(const T_EVGP_K& EvgpK);

    int  Get_RSListByDir(CArray<T_SPLC_K, T_SPLC_K>& raXSplcK, CArray<T_SPLC_K, T_SPLC_K>& raYSplcK, CArray<T_SPLC_K, T_SPLC_K>& raZSplcK);
    int  Get_RSList(CArray<T_SPLC_K, T_SPLC_K>& raSplcKey);
    int  Get_RSList(const T_LCOM_K& LcomK, CArray<T_SPLC_K, T_SPLC_K>& raSplcKey);

    void Calc_PierSeisRsForce(const T_EVGP_K& EvgpK, const UINT& iColm, const int& nSplcK, T_BRDGEVAL_FORCE ForceD[2]);
    void Calc_PierSeisRsDisp(const T_EVGP_K& EvgpK, const UINT& iColm, const int& nSplcK, double& dDispLong, double& dDsipTrans);

    BOOL Check_MPhiRes(const T_EVGP_K& EvgpK, const UINT& iColm, const EN_LOCAL_DIR& enDir, const EN_POSITION& enPos);

    double GetLinkForce(const UINT& LinkK, const int& nMinMax, int nType); // 0:elastic, 1:general

    BOOL IsRAHMEN(const T_EVGP_K& EvgpK, const UINT& iColm);
    void GetManderShearData(const T_EVGP_K& EvgpK, const UINT& iColm, const EN_POSITION& enPos, const EN_LOCAL_DIR& enDir, double& dAsp, double& dAv, double& ds, int& nRebarnum, double& dLongiRebarRatio);

    double Get_fck(const T_EVGP_K& EvgpK, const UINT& iColm, const EN_POSITION& enPos, const EN_LOCAL_DIR& enDir);
	void Get_fck_ftk_Shear(const T_EVGP_K& EvgpK, const UINT& iColm, const EN_POSITION& enPos, const EN_LOCAL_DIR& enDir, double& dfckS, double& dfctkS, double& dAxialForceS);
    double Get_fck(const T_FIMP_K& FimpConcK);
    double Get_fyh(const T_EVGP_K& EvgpK, const UINT& iColm, const EN_POSITION& enPos);
    double Get_Ec(const T_EVGP_K& EvgpK, const UINT& iColm, const EN_POSITION& enPos, const EN_LOCAL_DIR& enDir);
    double Get_Ec(const T_FIMP_K& FimpConcK);
    double Get_Es(const T_EVGP_K& EvgpK, const UINT& iColm, const EN_POSITION& enPos);

    BOOL GetEvpm(const T_EVGP_K& EvgpK, const UINT& iCol, T_EVPM_D& rEvpmD);
    T_ELEM_K GetEvgpElemK(const T_EVGP_K& EvgpK, const UINT& iCol, const int& nEvgpPos);
    T_ELEM_K GetEvgpElemK(const T_EVGP_K& EvgpK, const UINT& iCol, const int& nEvgpPos, T_EVGP_D& EvgpD);

    void   Get_ArrangeEdgeDimsConc(const T_BREV_K& BrevK, const EN_LOCAL_DIR& enDir, double& dca1, double& dca1p, double& dca2, double& dca2p);
    void   Get_ArrangeEdgeDimsConc(const T_ANEV_D& AnevD, const EN_LOCAL_DIR& enDir, double& dca1, double& dca1p, double& dca2, double& dca2p);
    void   Get_ArrangeEdgeDimsPryo(const T_BREV_K& BrevK, const EN_LOCAL_DIR& enDir, double& dca1, double& dca1p, double& dca2, double& dca2p);
    void   Get_ArrangeEdgeDimsPryo(const T_ANEV_D& AnevD, const EN_LOCAL_DIR& enDir, double& dca1, double& dca1p, double& dca2, double& dca2p);
    double Get_Avc(const double& dca1, const double& dca2, const double& dca2p, const double& ds1, const double& dsax, const double& nBC, const double& nA, const double& dha,
                   int& nAvcType, double& dLp);
    double Get_Anc(const double& dca1, const double& dca1p, const double& dca2, double dca2p, const double& dhef,
                   const double& ds1, const double& ds2, const double& dSax, const double& dSay,
                   const double& nBC, int nBCT, const double& nA, const double& nB,
                   int& nANcType);
    int    Get_NumOfResistSupport(const int&iTypex, const int& iTypey, const int& nBCx, const int& nBCy);

    T_ANEV_K GetAnevByBrev(const T_BREV_K& BrevK, T_ANEV_D& rData, BOOL bAnchorDirPosi=TRUE );
    T_SUPL_K GetSuplByBrev(const T_BREV_K& BrevK, T_SUPL_D& rData);
    T_SUPL_K GetSuplByAnev(const T_ANEV_K& BrevK, T_SUPL_D& rData);

	void ConvertAnevToNegativeDir(T_ANEV_D& rData);
    void ConvertToBearing(const double& dDegree, double& dVal1, double& dVal2);

    bool ExistRebar(const UINT& iGrup, const UINT& iColm, const EN_POSITION& enPos);
    bool Get_DeadLoadLcom(T_LCOM_D& LcomD);

private:  
    void SetMPhiRes(const T_MPCC_D& MpccD, const UINT& iColm, const EN_LOCAL_DIR& enDir, const int& nIdeal, const int& nError, const DGN_CALC_MPHI_RESULT& MPhiResD, BOOL bSave=FALSE);
    BOOL IsHorizontalLink(const T_NODE_K& NodeK1, const T_NODE_K& NodeK2);

protected:
    CDBDoc* m_pDoc;

    T_UNIT_INDEX m_CodeUnit;
    T_UNIT_INDEX m_CurrUnit;	

    CDgnMomentCurvatureDesign m_MCCalc;
};

