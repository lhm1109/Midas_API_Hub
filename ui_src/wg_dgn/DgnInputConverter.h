#pragma once

#include "HeaderPre.h"

class IDgnDataBase;

namespace dgn
{
    namespace def
    {
        enum class DataType : unsigned int;
        enum class enDgn;
        enum class enMemb;
        class CDgnInput;
        class CBindingData;
    }
    namespace converter
    {
        class __MY_EXT_CLASS__ CDgnInputConverter
        {
        public:
            CDgnInputConverter();
            virtual ~CDgnInputConverter();

        public:
            bool AddInputData(const dgn::def::DataType& Type, const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputMembType(const dgn::def::enDgn& enDgnType, const dgn::def::enMemb& enMembType, dgn::def::CDgnInput& Input);

        private:
			bool AddInputDesignCode(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputMemberInfoArray(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputSectionShape(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputSection(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputMaterial(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputSectionProperty(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputMaterialSteel(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputEffectiveLength(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputSectionStrength(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputBendingCoefficient(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputShearCoefficient(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputSafetyFactor(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputSeismicProperty(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputUnbracedLength(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputLcomInfo(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputForce1D(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputForce1DArray(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputPartialFactor(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputMembLength(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputForceUbln(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
			bool AddInputForce1DMember(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputSteelCompositeSection(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputStatus(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputMaterialConcrete(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputMaterialSteelHybrid(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputStressCheck(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputMomentLoadType(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
			bool AddInputTorsionDesign(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
			bool AddInputSteelOptions(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputLateralTorsionalBuckling(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
            bool AddInputLoadApplicationType(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
			bool AddInputShearConnector(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
			bool AddInputSteelCompositeRebar(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
			bool AddInputLongitudinalStiffener(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
			bool AddInputTransverseStiffener(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
			bool AddInputDamageEquivalenceFactor(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
			bool AddInputDamageEquivalenceFactorMember(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
			bool AddInputStressLimitationFactor(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
			bool AddInputStressSteelComposite(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
			bool AddInputConstrainStressComposite(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
			bool AddInputMemoizationKey(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
			bool AddInputImperfectionFactor(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
			bool AddInputCompositeSectionProperty(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
			bool AddInputPanelInfoArray(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
			bool AddInputLimitSlendernessRatio(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
			bool AddInputResidualStressType(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
			bool AddInputStiffnessScaleFactor(const IDgnDataBase* pDgnDataBase, dgn::def::CDgnInput& Input);
		};
    }
}

#include "HeaderPost.h"
