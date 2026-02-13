#pragma once

#include "..\..\dgnengine\idesign\DGN_def\IDgnPerformInputConverter.h"

#include "HeaderPre.h"

class IDgnPerformDataBase;

namespace dgn
{
    namespace def
    {
        enum class DataType : unsigned int;
        enum class enDgn;
        enum class enMemb;
        class CDgnInput;
        class CBindingData;
        struct TMainRebar2D;
        struct TShearDia;
        struct TDiaMatl;
        struct TDia;
    }
    namespace converter
    {
        class __MY_EXT_CLASS__ CDgnPerformInputConverter : public dgn::def::IDgnPerformInputConverter
        {
        public:
            CDgnPerformInputConverter(IDgnPerformDataBase* pDataBase);
            virtual ~CDgnPerformInputConverter();

        private:
            virtual bool AddPrefOption() override final;
            virtual bool AddDesignCase(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddDesignCode(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddStructureInfo() override final;
            virtual bool AddFrameType() override final;
            virtual bool AddStory() override final;
            virtual bool AddModule() override final;
            virtual bool AddNode(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddElem(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddMemb(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddSpvaData(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddSect(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddMatl(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddPosi(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddPosi2D(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddLcom(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddBoundary(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddMembGroup(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddSectProperty(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddUnbracedLength(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddEffectiveLength(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddLiveLoadReduction(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddScaleUpFactorEQ(const std::vector<long>& vKeyMemb) override final;            
            virtual bool AddOffset1D(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddMembLength(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddSeismicProperty(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddSpcVerLcom(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddUnderLcomType(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddMomentMagnifier(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddEquivalentMoment(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddDeflectionParameter(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddLimitOption(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddTorsionDesign(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddCheckDirection(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddPointLoad() override final;
            virtual bool AddCuttingLine() override final;
            virtual bool AddNodalCrackedDeform(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddScaleUpfactorRS() override final;
            virtual bool AddGeneralLineInfo(const std::vector<long>& vKeyMemb) override final;
			virtual bool AddDisp1DRelative(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddResponseModificationFactor(const std::vector<long>& vKeyMemb) override final;
            
            // RC
            virtual bool AddRebarBeam(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddRebarColumn(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddRebarGeneral(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddRebarBrace(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddRebarWall(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddRebarPlate(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddRebarSlab() override final;
            virtual bool AddMomentRedistribution(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddTorsionRedistribution(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddCheckOption(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddDesignCriteriaBeam(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddDesignCriteriaColumn(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddDesignCriteriaBrace(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddDesignCriteriaWall(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddDesignCriteriaSlab(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddDesignCriteriaAs(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddDesignCriteriaPlate(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddSameRebarAtJoint(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddHaunchedBeam(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddPMCalcOption(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddRebarRatio(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddProbableMomentStrengthBeam(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddDesignMomentStrengthBeam(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddWallID(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddWallOptions(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddSlabOptions(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddShellOptions(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddSandwichModel(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddForceOption2D(const std::vector<long>& vKeyMenb) override final;
            virtual bool AddSlabServParam(const std::vector<long>& vKeyMenb) override final;
            virtual bool AddSlabShearReinforcement() override final;
            virtual bool AddSlabColumnLocation() override final;
            virtual bool AddSlendernessApproximation(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddConcStressStrainType(const std::vector<long>& vKeyMemb) override final;
			virtual bool AddConcRebarStressMethod(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddColumnPiloti(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddBoundaryElementMethod(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddShearAngle(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddPartialFactor(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddStressCheck(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddCrackLimit(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddLimitSlendernessRatioParamEU(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddColumnCapital(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddCrackControlCheck(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddSkinBarSpaceCheck(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddBarExposureCondition(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddContinuousWall(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddDefStiffRatio(const std::vector<long>& vKeyMemb) override final;
			
            // STL
            virtual bool AddLimitSlendernessRatio(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddBendingCoefficient(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddShearCoefficient(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddSafetyFactor(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddCombinedRatioMethod(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddSteelGeneralSectParam(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddFatigueParameter(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddDeflectionParameterStl(const std::vector<long>& vKeyMemb) override final;
			virtual bool AddLongitudinalStiffener(const std::vector<long>& vKeyMemb) override final;
			virtual bool AddTransverseStiffener(const std::vector<long>& vKeyMemb) override final;
			virtual bool AddSteelOptions(const std::vector<long>& vKeyMemb) override final;
			virtual bool AddImperfectionFactor(const std::vector<long>& vKeyMemb) override final;
			virtual bool AddLoadPointMcr(const std::vector<long>& vKeyMemb) override final;

        private:
            virtual bool AddRptEnvironment(const std::vector<long>& vKeyMemb) override final;

        private:
            bool AddRebarBeam_Gen(const std::vector<long>& vKeyMemb, std::vector<dgn::def::CBindingData>& vBindingData);
            bool AddRebarBeam_Civil(const std::vector<long>& vKeyMemb, std::vector<dgn::def::CBindingData>& vBindingData);
            bool AddRebarColumn_Gen(const std::vector<long>& vKeyMemb, std::vector<dgn::def::CBindingData>& vBindingData);
            bool AddRebarColumn_Civil(const std::vector<long>& vKeyMemb, std::vector<dgn::def::CBindingData>& vBindingData);
            bool AddRebarBrace_Gen(const std::vector<long>& vKeyMemb, std::vector<dgn::def::CBindingData>& vBindingData);
            bool AddRebarBrace_Civil(const std::vector<long>& vKeyMemb, std::vector<dgn::def::CBindingData>& vBindingData);
            bool AddRebarWall_Gen(const std::vector<long>& vKeyMemb, std::vector<dgn::def::CBindingData>& vBindingData);
            bool AddRebarSlab_Gen(std::vector<dgn::def::CBindingData>& vBindingData);
            bool AddRebarSlab1way_Gen(std::vector<dgn::def::CBindingData>& vBindingData);
            bool AddRebarPlateBeam(const std::vector<long>& vKeyMemb, std::vector<dgn::def::CBindingData>& vBindingData);
            bool AddRebarPlateColumn(const std::vector<long>& vKeyMemb, std::vector<dgn::def::CBindingData>& vBindingData);
            bool AddRebarShell(const std::vector<long>& vKeyMemb, std::vector<dgn::def::CBindingData>& vBindingData);

            bool AddSectForDesign(const std::vector<long>& aKeyMemb, bool bDesign = false);
            bool AddSectPropertyForDesign(const std::vector<long>& aKeyMemb, bool bDesign = false);

        public:
            virtual bool AddRptCase(const std::vector<long>& vKeyMemb) override final;
            virtual bool AddRptCriticalInfo(const std::vector<long>& vKeyMemb) override final;

        private:
            double Get_UnitWidth(const int iDgnCode);
            double Get_ClearDistArray(const int iDgnCode, const double& dBarDia, bool bSRC);
            double Get_VerRebarSpaceByString(const int iDgnCode, CString strVerSpace);
            double Get_RebarSpaceIncrease(const int iDgnCode, int nSubCode) const;
            double Get_RebarSpaceMax(const int iDgnCode, int nSubCode) const;
            double Get_Es(const int& iDgnCode, const double& dfy) const;
            std::pair<double, double> GetRCMainRebarFyEs(T_ELEM_K ElemK);
            std::pair<double, double> GetRCSubRebarFyEs(T_ELEM_K ElemK);

            bool SetRebar2D(const CString strRebar, const CString strAddRebar, const double& dSpace, const double& dCover, const double& dfy, const double& dEs, dgn::def::TMainRebar2D& RebarD);
            bool SetRebar2D(const CString strRebar, const double& dSpace, const double& dCover, dgn::def::TMainRebar2D& RebarD);
            bool SetRebarMatl(const double& dfy, const double& dEs, dgn::def::TDiaMatl& RebarMatlD);
            
            bool SetShearRebar2D(const CString strRebar, const double& dSpace, dgn::def::TShearDia& RebarD);
            bool SetShearRebarMatl2D(const double& dfy, const double& dEs, dgn::def::TShearDia& RebarD);

            bool CalcMembLength1D(const T_ELEM_K ElemK, OUT double& rdLeng, OUT double& rdNetLy, OUT double& rdNetLz);
            bool CalcMembLengthWall(const T_ELEM_K ElemK, OUT double& rdLeng);

        private:
            virtual bool SetDataPool(const std::vector<dgn::def::CBindingData>& vBindingData, std::vector<DWORD_PTR>& vIter) override final;
            virtual bool SetDataPool(const std::vector<dgn::def::CBindingData>& vBindingData) override final;

            bool AddMembQueryFromElemQuery(std::vector<dgn::def::CBindingData>& vBindingData);

        private:
            dgn::def::TDia CreateRebarDia(const CString& strRebarName, const T_MATL_REBAR& RebarData);

        private:
            IDgnPerformDataBase* m_pDataBase;
        };
    }
}

#include "HeaderPost.h"
