#pragma once

#include "BrdgSeisEval.h"
#include "DgnSeisAnchorShapeMgr.h"

#include "HeaderPre.h"

class CDBDoc;
class __MY_EXT_CLASS__ CBrdgSeisEvalKISTEC2015 : public CBrdgSeisEval
{
public:
    CBrdgSeisEvalKISTEC2015(void);
    virtual ~CBrdgSeisEvalKISTEC2015(void);

public:
    void   CalcIeff(const T_MPCC_D& MpccD, double dIeffRat[2], T_BRDGEVAL_RESULT Result[2], BOOL bRecal=TRUE); // 유효강성비 계산
    void   CalcHe(const T_MPCC_K& MpccTopK, const T_MPCC_K& MpccBotK, const EN_LOCAL_DIR& enDir, const EN_BEHAVIOR_TYPE& enBehavior, const int& nCalcType, const double& dHc, double dHe[2]); // 유효높이 계산
    BOOL   DoSeisEval() final;
    
    // sa; longi - sax, trans - say
    virtual BOOL Check_Inter3Faces(double& dca1, const double& dca2, const double& dca2p, const double& dhcop, const double& dsa, const double& dLp);
    // iDir; longi - 0, trans - 1
    BOOL Check_Inter4Faces(double& dhef, const int& iDir, const double& dca1, const double& dca1p, const double& dca2, const double& dca2p, const double& dsax, const double& dsay, const int& nnB, const double& ds1, const double& ds2);

	double Get_WallVn(const T_BRDGEVAL_SHR_CAPA& ShearCapaD);
    // iDir; longi - 0 , trans - 1 
    double Calc_BearingAnchor_Concrete_Ratio(const T_BREV_K& BrevK, TConcFailBcCalc& MinConcFailCal, std::vector<std::pair<int, int>> aRepBearingIdx, T_BRDGEVAL_BEARING& rResD, const int& iDir, int nRatType[4]);
    double Calc_BearingAnchor_Pryout_Ratio(const T_BREV_K& BrevK, int nRepresentGrpIdx, std::vector<std::pair<int, int>> aRepBearingIdx, T_BRDGEVAL_BEARING& rResD, const int& iDir, int nRatType[4], BOOL bAnchorTenConc=FALSE);

    virtual BOOL Get_BearingParam(const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD, BOOL bDesign, BOOL bAnchorDirPosi=TRUE); //bDesign=FALSE면 설계 부분 사용 안함
	virtual BOOL Calc_BearingAnchorForDlg(int nFailType, T_ANEV_D& AnevD, T_BRDGEVAL_BEARING& rResD);
    virtual double Calc_BearingAnchor_Concrete_Ratio(T_ANEV_D& AnevD, T_BREV_D& BrevD, TConcFailBcCalc& MinConcFailCal, std::vector<std::pair<int, int>> aRepBearingIdx, T_BRDGEVAL_BEARING& rResD, const int& iDir, int nRatType[4]);
    virtual double Calc_BearingAnchor_Pryout_Ratio(T_ANEV_D& AnevD, T_BREV_D& BrevD, int nRepresentGrpIdx, std::vector<std::pair<int, int>> aRepBearingIdx, T_BRDGEVAL_BEARING& rResD, const int& iDir, int nRatType[4], BOOL bAnchorTenConc=FALSE);

protected:
    void CalcCapacity(const UINT& EvgpK, const UINT& iColm, const EN_LOCAL_DIR& enDir, const EN_POSITION& enPos, T_BRDGEVAL_CALC_PARAM& ParamD, T_BRDGEVAL_CAPACITY& CapaD);

    void CheckFailureType(const EN_LOCAL_DIR& enDir, const EN_POSITION& enPos, const T_BRDGEVAL_CAPACITY& CapaD, const CString strShape, T_BRDGEVAL_FAILURE_MODE& ModeD);
    void CalcReqCapacity(const UINT& EvgpK, const UINT& iColm, const EN_LOCAL_DIR& enDir, const EN_POSITION& enPos, const EN_FAILURE_MODE& enFalureMode, const T_BRDGEVAL_CALC_PARAM& ParamD, T_BRDGEVAL_EVAL_CAPA& EvalD);
    void Calc_ShearCapacity(const EN_LOCAL_DIR& enDir, T_BRDGEVAL_CALC_PARAM& ParamD, const T_BRDGEVAL_ULT_DISP_DUCT& UltDsipD, T_BRDGEVAL_SHR_CAPA& ShearCapaD);
    
    virtual EN_PIER_TYPE Get_PIER_TYPE(const EN_LOCAL_DIR& enDir, const int& nSplice, const CString& strShape, const int& nBHmethod, const double& dArea, const double& dB, const double& dH);
    double Get_BHrat(const int& nMethod, const CString& strShape, const double& dArea, const double& dB, const double& dH);

    EN_FAILURE_MODE GetFailureMode(const T_BRDGEVAL_BEND_CAPA& BendCapaD, const T_BRDGEVAL_SHR_CAPA& ShearCapaD, double& dDeltac, double& dVn);

    virtual void Calc_BendingCapacity(const SEIS_RES_KEY& SeisResK, const T_BRDGEVAL_CALC_PARAM& ParamD, T_BRDGEVAL_ULT_DISP_DUCT& UltDsipD, T_BRDGEVAL_BEND_CAPA& BendCapaD);
    virtual double Get_MaxSupplyDispDuct(const int& nSplice, const CString& strShape, const EN_LOCAL_DIR& enDir, const int& nBHmethod, const double& dArea, const double& dB, const double& dH, const double& dlsp);
    virtual void Get_WallVn(T_BRDGEVAL_CALC_PARAM& ParamD, T_BRDGEVAL_SHR_CAPA& ShearCapaD);
    virtual void CalcPlainConcCapacity(const UINT& EvgpK, const UINT& iColm, const EN_LOCAL_DIR& enDir, const EN_POSITION& enPos, const EN_FAILURE_MODE& enFalureMode, const T_BRDGEVAL_CALC_PARAM& ParamD, T_BRDGEVAL_EVALUATION& EvalD);

public:
    virtual void   DoSeisEvalBearing();

protected:    
    void Calc_BearingBody(const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD);
    void Calc_BearingAnchor(const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD);
    virtual void Calc_BearingAnchor_Steel(const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD);
    void Calc_BearingAnchor_Concrete(const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD);
	void Calc_BearingAnchor_ConcreteSub(const T_BREV_K& BrevK, BOOL bAnckorPos,  T_BRDGEVAL_BEARING& rResD);
	void Calc_BearingAnchor_ConcreteSubData(T_ANEV_D& AnevD, BOOL bAnckorPos, T_BRDGEVAL_BEARING& rResD, BOOL bDlg=FALSE);
    void Calc_BearingAnchor_Pryout(const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD);
	void Calc_BearingAnchor_PryoutSub(const T_BREV_K& BrevK, BOOL bAnckorPos, T_BRDGEVAL_BEARING& rResD);
	void Calc_BearingAnchor_PryoutSubData(T_ANEV_D& AnevD, BOOL bAnckorPos, T_BRDGEVAL_BEARING& rResD, BOOL bDlg = FALSE);
    void Calc_BearingSupport_Length(const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD);

    void Get_BeringForce(const T_BREV_K& BrevK, const double& dTheta, double& dVeL, double& dMeL, double& dVeT, double& dMeT); // dTheta; 받침 배치 각도(degree)
    void Get_Fn(const T_BREV_K& BrevK, double& dTheta, double& dfnL, double& dfnT, BOOL& bRoundSect); // dTheta; 받침 배치 각도(degree)
    void Get_Fn(const T_BREV_D& BrevD, double& dTheta, double& dfnL, double& dfnT, BOOL& bRoundSect); // dTheta; 받침 배치 각도(degree)

    void Get_Rs_LabdaDR(const T_BREV_K& BrevK, double& dRsL, double& dLDRL, double& dRsT, double& dLDRT);
    void Get_Disp(const T_BREV_K& BrevK, const double& dTheta, double& dDispL, double& dDispT, UINT& LinkL, UINT& LinkT);
    double Get_Ptop(const T_BREV_K& BrevK, bool bTotal=false);
    double Get_BearingNumb(const EN_LOCAL_DIR& enDir, const int nBI, const int nBJ, const int enBearing, const T_ANEV_D& AnevD, const T_BREV_D& BrevD, const CString strBearProp);

    // KISTEC2019
    void Calc_BearingAnchor_TenSteel(const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD);
    void Calc_BearingAnchor_TenConc (const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD);
	void Calc_BearingAnchor_TenConcSub(const T_BREV_K& BrevK, BOOL bAnckorPos, T_BRDGEVAL_BEARING& rResD);
    void Calc_BearingAnchor_TenPull (const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD);
    void Calc_BearingAnchor_TenSide (const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD);
    void Calc_BearingAnchor_TenRela (const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD);

    double Get_MaxTension(const T_BREV_K& BrevK, const T_LCOM_K& LcomK);
    double Get_MaxTensionMulti(const T_BREV_K& BrevK, const int nBI, const int nBJ, CArray<T_ELNK_K, T_ELNK_K>& aElnkK, CArray<T_NLNK_K, T_NLNK_K>& aNlnkK, const T_LCOM_K& LcomK);

    void ConvertAnchorParam(const int nBI, const int nBJ, const T_BREV_D& BrevD, const T_ANEV_D& AnevD, const T_BRPR_D& BrprD, T_BRDGEVAL_BEARING_PARAM ParamD, T_BRDGEVAL_ANCHOR_PARAM& rAnchorParam);

    virtual double Calc_Vb(const double& dFactor, const double& dle, const double& dap, const double& dfck, const double& dca1);

    virtual double Get_VbFactor();
    virtual double Calc_Vbmax(const double& dfck, const double& dca1);
    virtual double Calc_FACC(const double& dVcbg, const double& dPu);
    virtual bool   IsCode4PryoutRebar() { return false; }

    virtual double Get_Vsamax(T_BRDGEVAL_BEARING& rResD);
    virtual double Get_psi_cV(const T_ANEV_D& AnevD);
    virtual double Get_psi_cN(const EN_ANCH_TYPE& enAnchType);
};
#include "HeaderPost.h"
