#ifndef __DB_ST_DT_FIGR_H__
#define __DB_ST_DT_FIGR_H__

#define MIDAS_SDK

#include <float.h>
#pragma pack(push, 1)
#include "..\wg_base\AfxTemplEx.h"
// User Defined Figure
#define T_UFIG_K unsigned int
 
#define CHAR_ACTIVE_MAX 10000

#pragma warning ( disable : 4244 )

///// pre & common figure info.
enum EN_DPST_NODE
{
	/////////////////////// 파일에 저장되는 것이므로 변경시 버젼 분리해줄것!! ///////////////////////
	EN_DPST_NODE_bNode                    , // Node
	EN_DPST_NODE_bNodeNo                  , // Node Number
	EN_DPST_NODE_bNodeNoBorder            , // Node Number with Border
	EN_DPST_NODE_bStoryName               , // Story Name
	EN_DPST_NODE_bSkew                    , // Node Local Axis
														
	EN_DPST_NODE_END
};
enum EN_DPST_ELEM
{
	/////////////////////// 파일에 저장되는 것이므로 변경시 버젼 분리해줄것!! ///////////////////////
	EN_DPST_ELEM_bElemNo, // Element Number
	EN_DPST_ELEM_bElemNoBorder, // Element Number with Border
	EN_DPST_ELEM_bTypeNo, // Element Type Number
	EN_DPST_ELEM_bTypeName, // Element Type Name
	EN_DPST_ELEM_bWallType, // Wall Type
	EN_DPST_ELEM_bWallNumber, // Wall ID
	EN_DPST_ELEM_bGap, // Gap
	EN_DPST_ELEM_bHook, // Hook
	EN_DPST_ELEM_bCable, // Cable
	EN_DPST_ELEM_bLocalAxis, // Local Axis
	EN_DPST_ELEM_bLocalDir, // Local Direction
	EN_DPST_ELEM_bMember, // Design Tab : Member
	EN_DPST_ELEM_bMemberDir, // Design Tab : Member Direction
	EN_DPST_ELEM_bMemberNumber, // Design Tab : Member Number
	EN_DPST_ELEM_bColmCapital, // Column Capital
	EN_DPST_ELEM_bDropPanel, // Drop Panel
	EN_DPST_ELEM_bMstr, // Column Strip
	EN_DPST_ELEM_bLocalAxisLabel, // Local Axis Label
	EN_DPST_ELEM_bTdln, // Tendon Line
	EN_DPST_ELEM_bDstr, // Design Strip
	EN_DPST_ELEM_bSptl, // Support Line
	EN_DPST_ELEM_END,

	EN_DPST_ELEM_END_835 = 14,    // v835에서 EN_DPST_ELEM_bColmCapital,EN_DPST_ELEM_bDropPanel,EN_DPST_ELEM_bMstr 추가됨
	EN_DPST_ELEM_END_940 = 17,	  // v945에서 EN
	EN_DPST_ELEM_END_955 = 18,
	EN_DPST_ELEM_END_965 = 20,
};
enum EN_DPST_PROP
{
	/////////////////////// 파일에 저장되는 것이므로 변경시 버젼 분리해줄것!! ///////////////////////
	EN_DPST_PROP_bMatlNo                  , // Material Number
	EN_DPST_PROP_bMatlName                , // Material Name
	EN_DPST_PROP_bPropNo                  , // Property Number
	EN_DPST_PROP_bPropName                , // Property Name
	EN_DPST_PROP_bSectionShape            , // Section Shape
	EN_DPST_PROP_bTimeDepMat              , // Time Dependent Material Link
	EN_DPST_PROP_bTaperSGroup             , // Tapered Section Group
	EN_DPST_PROP_bVsecLocalAxis           , // (v960) Vsec Local Axis  
	EN_DPST_PROP_bVirtualBeamSection      , // (v960) Virtual Beam Section
	EN_DPST_PROP_END,

	EN_DPST_PROP_END_955 = 7,

};
enum EN_DPST_BNDR
{
	/////////////////////// 파일에 저장되는 것이므로 변경시 버젼 분리해줄것!! ///////////////////////
	EN_DPST_BNDR_bCons                    , // Support
	EN_DPST_BNDR_bConsDir                 , // Support by Direction
	EN_DPST_BNDR_bNspr                    , // Point Spring Support
	EN_DPST_BNDR_bNsprNon                 , // Point Spring Support (Comp/Tens)
	EN_DPST_BNDR_bNsprMulti               , // Point Spring Support (Multi-Linear)
	EN_DPST_BNDR_bNsprDir                 , // Point Spring Support by Direction
	EN_DPST_BNDR_bNsprNonDir              , // Point Spring Support by Direction (Comp/Tens)
	EN_DPST_BNDR_bNsprMultiDir            , // Point Spring Support by Direction (Multi-Linear)
	EN_DPST_BNDR_bSsps                    , // Surface Spring Support Type
	EN_DPST_BNDR_bSspsLin                 , // Surface Spring Support (Linear)
	EN_DPST_BNDR_bSspsNon                 , // Surface Spring Support (Comp/Tens)
	EN_DPST_BNDR_bGspr                    , // General Spring Support 
	EN_DPST_BNDR_bFrls                    , // Beam End release Symbol or Digit 둘 중 하나라도 체크됐을때 TRUE
	EN_DPST_BNDR_bFrlsTxt                 , // Beam End release Digit
	EN_DPST_BNDR_bFrlsSym                 , // Beam End release Symbol
	EN_DPST_BNDR_bOffs                    , // Beam End offset
	EN_DPST_BNDR_bPlateEndR               , // Plate End Release Symbol or Digit 둘 중 하나라도 체크됐을때 TRUE
	EN_DPST_BNDR_bPlateEndRTxt            , // Plate End Release Digit
	EN_DPST_BNDR_bPlateEndRSym            , // Plate End Release Symbol
	EN_DPST_BNDR_bRigd                    , // Rigid Link
	EN_DPST_BNDR_bMcon                    , // Linear Constraints
	EN_DPST_BNDR_bDiaDiscon               , // Diaphragm Disconnect
	EN_DPST_BNDR_bStoryDia                , // Story_Diaphragm
	EN_DPST_BNDR_bLineLane                , // Misc Tab : Line Lanes
	EN_DPST_BNDR_bLaneSupport             , // Misc Tab : Lane Support
	EN_DPST_BNDR_bLaneSupportReaction     , // Misc Tab : Lane Support Reaction
	EN_DPST_BNDR_bElasticLink             , // Elastic Link가 TRUE 일때, 밑의 옵션을 사용가능.
	EN_DPST_BNDR_bElasticLinkAxis         , // Elastic Link Local Axis
	EN_DPST_BNDR_bElasticLinkNo           , // Elastic Link Number
	EN_DPST_BNDR_bElasticLinkType         , // Elastic Link Type
	EN_DPST_BNDR_bCraneLane               , // Misc Tab : Crane Lane
	EN_DPST_BNDR_bCraneName               , // Misc Tab : Crane Name
	EN_DPST_BNDR_bSettlement              , // Misc Tab : Settlement Group      
	EN_DPST_BNDR_bSettlementValue         , // Misc Tab : Settlement Group Value
	EN_DPST_BNDR_bSurfaceLane             , // Misc Tab : Surface Lane
	EN_DPST_BNDR_bInfluenceSurf           , // Misc Tab : Influence Surface
	EN_DPST_BNDR_bConvectionBound         , // Misc Tab : Heat of Hydration Element Convection Boundary
	EN_DPST_BNDR_bSpecTemperature         , // Misc Tab : Heat of Hydration Prescribed Temperature
	EN_DPST_BNDR_bHeatSource              , // Misc Tab : Heat of Hydration Heat Source
	EN_DPST_BNDR_bPipeCoolingElem         , // Misc Tab : Heat of Hydration Pipe Cooling Element
	EN_DPST_BNDR_bConvectionBoundTxt      , // Misc Tab : Heat of Hydration Element Convection Boundary txt 내부변수
	EN_DPST_BNDR_bSpecTemperatureTxt      , // Misc Tab : Heat of Hydration Prescribed Temperature txt 내부변수
	EN_DPST_BNDR_bHeatSourceTxt           , // Misc Tab : Heat of Hydration Heat Source txt 내부변수
	EN_DPST_BNDR_bPipeCoolingElemTxt      , // Misc Tab : Heat of Hydration Pipe Cooling Element txt 내부변수
	EN_DPST_BNDR_bHydFnName               , // Misc Tab : Heat of Hydration Func Name
	EN_DPST_BNDR_bHydValue                , // Misc Tab : Heat of Hydration Value
	EN_DPST_BNDR_bReactionPosition        , // Reaction Position
	EN_DPST_BNDR_bNonlinearLink           , // General Link
	EN_DPST_BNDR_bNonlinearLinkAxis       , // General Link Axis
	EN_DPST_BNDR_bNonlinearLinkNo         , // General Link Number
	EN_DPST_BNDR_bNonlinearLinkType       , // General Link Type
	EN_DPST_BNDR_bCGLP                    , // Change General Link Properties
	EN_DPST_BNDR_bAllGroup                , // All : TRUE, Group Selection : FALSE
	EN_DPST_BNDR_bPushOverType            , // Design Tab : Pushover Hinge Type
	EN_DPST_BNDR_bPushOverSymbol          , // Design Tab : Pushover Hinge Symbol
	EN_DPST_BNDR_bPushOverName            , // Design Tab : Pushover Hinge Name
	EN_DPST_BNDR_bInelasticHingeSymbol    , // Property Tab : Inelastic Hinge Symbol
	EN_DPST_BNDR_bInelasticHingeName      , // Property Tab : Inelastic Hinge Name
	EN_DPST_BNDR_bLineLaneop              , // Misc Tab : Line Lane Optimization
	EN_DPST_BNDR_bSurfaceLaneop           , // Misc Tab : Surface Lane Optimization
	EN_DPST_BNDR_bOffs_Symbol, // Boundary Tab : Beam End offset Symbol
	EN_DPST_BNDR_bOffs_Digit, // Boundary Tab : Beam End offset Digit
	EN_DPST_BNDR_bLaneSupportAllForce, // Misc Tab : Lane Support-All Forces/Moments - v955 추가
	EN_DPST_BNDR_END,
	EN_DPST_BNDR_END_855 = 58, // bLineLaneop와 bSurfaceLaneop가 추가 됨
	EN_DPST_BNDR_END_940 = 60, // bLineLaneop와 bSurfaceLaneop가 추가 됨
	EN_DPST_BNDR_END_950 = 62
};
enum EN_DPST_LOAD
{
	/////////////////////// 파일에 저장되는 것이므로 변경시 버젼 분리해줄것!! ///////////////////////
	EN_DPST_LOAD_bStoryMassCenter         , // Misc Tab : Story Mass, Stiffness
	EN_DPST_LOAD_bCnld                    , // Nodal Load
	EN_DPST_LOAD_bBmld                    , // Beam Load
	EN_DPST_LOAD_bPres                    , // Pressure Load
	EN_DPST_LOAD_bFbld                    , // Floor Load
	EN_DPST_LOAD_bPrestress               , // Prestress Load
	EN_DPST_LOAD_bPretention              , // Pretension Load
	EN_DPST_LOAD_bDisplacement            , // Specified Displacement
	EN_DPST_LOAD_bNodalTemp               , // Nodal Temperature
	EN_DPST_LOAD_bTempGradiant            , // Temperature Gradient
	EN_DPST_LOAD_bNodalMass               , // Misc Tab : Nodal Mass
	EN_DPST_LOAD_bFbldDiaMass             , // Misc Tab : Floor Diaphragm Mass
	EN_DPST_LOAD_bWind                    , // Wind Load
	EN_DPST_LOAD_bSeismic                 , // Seismic Load
	EN_DPST_LOAD_bPseudoSeismic           , // Pseudo Seismic Load
	EN_DPST_LOAD_bLoadToMass              , // Misc Tab : Load to Mass
	EN_DPST_LOAD_bDynNodalLoad            , // Dynamic Nodal Load
	EN_DPST_LOAD_bFloorLoadName           , // Floor Load Name
	EN_DPST_LOAD_bFloorLoadArea           , // Floor Load Area
	EN_DPST_LOAD_bIFGS                    , // Misc Tab : Initial Forces for Geometric Stiffnes
	EN_DPST_LOAD_bElemTemp                , // Element_Temperature
	EN_DPST_LOAD_bTendonProp              , // Misc Tab : Tendon Profile Names or Point 둘중 하나라도 TRUE면 TRUE
	EN_DPST_LOAD_bTendonPrestress         , // Tendon Prestress
	EN_DPST_LOAD_bPlaneLoad               , // Plane Load
	EN_DPST_LOAD_bPlaneLoadName           , // Plane Load Name
	EN_DPST_LOAD_bBTMP                    , // Beam Section Temperature
	EN_DPST_LOAD_bFMLD                    , // Finishing Material Load
	EN_DPST_LOAD_bTHMS                    , // Multiple Support Excitation
	EN_DPST_LOAD_bTHMS_FuncName           , // Multiple Support Excitation Function Name
	EN_DPST_LOAD_bTHMS_FuncName_x         , // Dir X
	EN_DPST_LOAD_bTHMS_FuncName_y         , // Dir Y
	EN_DPST_LOAD_bTHMS_FuncName_z         , // Dir Z
	EN_DPST_LOAD_bCnldTxt                 , // 각 Load당 Load Value를 위해 내부에서 사용하는 변수인듯..
	EN_DPST_LOAD_bBmldTxt                 ,
	EN_DPST_LOAD_bPresTxt                 ,
	EN_DPST_LOAD_bFbldTxt                 ,
	EN_DPST_LOAD_bPrestressTxt            ,
	EN_DPST_LOAD_bPretentionTxt           ,
	EN_DPST_LOAD_bDisplacementTxt         ,
	EN_DPST_LOAD_bNodalTempTxt            ,
	EN_DPST_LOAD_bTempGradiantTxt         ,
	EN_DPST_LOAD_bNodalMassTxt            ,
	EN_DPST_LOAD_bFbldDiaMassTxt          ,
	EN_DPST_LOAD_bWindTxt                 ,
	EN_DPST_LOAD_bSeismicTxt              ,
	EN_DPST_LOAD_bPseudoSeismicTxt        ,
	EN_DPST_LOAD_bLoadToMassTxt           ,
	EN_DPST_LOAD_bDynNodalLoadTxt         ,
	EN_DPST_LOAD_bFloorLoadNameTxt        ,
	EN_DPST_LOAD_bElemTempTxt             ,
	EN_DPST_LOAD_bTendonPropTxt           ,
	EN_DPST_LOAD_bTendonPropPoint         ,
	EN_DPST_LOAD_bTendonPrestressTxt      ,
	EN_DPST_LOAD_bPlaneLoadNameTxt        ,
	EN_DPST_LOAD_bPlaneLoadTxt            ,
	EN_DPST_LOAD_bBTMPTxt                 ,
	EN_DPST_LOAD_bFMLDTxt                 ,
	EN_DPST_LOAD_bMGDR                    , // Misc Tab : Grid Model Main Girder
	EN_DPST_LOAD_bCBEM                    , // Misc Tab : Grid Model Cross Beam
	EN_DPST_LOAD_bLDLN                    , // Misc Tab : Grid Model Load Line
	EN_DPST_LOAD_bGIMP                    , // Misc Tab : Grid Model Impact Factor
	EN_DPST_LOAD_bGILD                    , // Grid Influence Line Load
	EN_DPST_LOAD_bGSTY                    , // Misc Tab : Element Structure Type
	EN_DPST_LOAD_bGMAS                    , // Misc Tab : Grid Mass
	EN_DPST_LOAD_bSSEI                    , // Superstructure Seismic Load
	EN_DPST_LOAD_bRPSC                    , // Property Tab : Reinforcement of Sections
	EN_DPST_LOAD_bMGDRTxt                 , // 각 Load당 Load Value를 위해 내부에서 사용하는 변수인듯..
	EN_DPST_LOAD_bCBEMTxt                 ,
	EN_DPST_LOAD_bLDLNTxt                 ,
	EN_DPST_LOAD_bBSPNTxt                 ,
	EN_DPST_LOAD_bGIMPTxt                 ,
	EN_DPST_LOAD_bGILDTxt                 ,
	EN_DPST_LOAD_bGSTYTxt                 ,
	EN_DPST_LOAD_bGMASTxt                 ,
	EN_DPST_LOAD_bSSEITxt                 ,
	EN_DPST_LOAD_bRPSCTxt                 ,
	EN_DPST_LOAD_bP1LATxt                 ,
	EN_DPST_LOAD_bLNKF_ELNKTxt            ,
	EN_DPST_LOAD_bLNKF_NLNKTxt            ,
	EN_DPST_LOAD_bNBOF                    , // Nodal Body Force
	EN_DPST_LOAD_bNBOFTxt                 , // 각 Load당 Load Value를 위해 내부에서 사용하는 변수인듯..
	EN_DPST_LOAD_bTxtAll                  , // Load Value check box
	EN_DPST_LOAD_bAllGroup                , // Load Case Group Selection일때만 FALSE
	EN_DPST_LOAD_bArpr		, // v885 추가  
	EN_DPST_LOAD_bArprTxt	,
	EN_DPST_LOAD_bEpst		, //Static Earth Pressure
	EN_DPST_LOAD_bEarthPTxt	,
	EN_DPST_LOAD_bEpse		, // Seismic Earth Pressure
	EN_DPST_LOAD_bDummy4	,
	EN_DPST_LOAD_bDummy5	,
	EN_DPST_LOAD_bDummy6	,
	EN_DPST_LOAD_bPTMP      , // 92, (v940) Beam Section Temperature
	EN_DPST_LOAD_bPTMPTxt   , // 93, (v940) Beam Section Temperature
	EN_DPST_LOAD_bTypeLaplLagr,
	EN_DPST_LOAD_bWindPArea,
	EN_DPST_LOAD_bWindPAreaName,
	EN_DPST_LOAD_bWindPBeam,
	EN_DPST_LOAD_bWindPNodal,
	EN_DPST_LOAD_bWindPNodalSh,
	EN_DPST_LOAD_bWindPFunc,
	EN_DPST_LOAD_bWindPFuncName,
	EN_DPST_LOAD_bWindPTxt,
	EN_DPST_LOAD_END,
	EN_DPST_LOAD_END_865 = 81,
	EN_DPST_LOAD_END_883 = 83,
	EN_DPST_LOAD_END_940 = 91
};
enum EN_DPST_DSGN
{
	/////////////////////// 파일에 저장되는 것이므로 변경시 버젼 분리해줄것!! ///////////////////////
	EN_DPST_DSGN_bULLy                    , // Unbraced Length Ly
	EN_DPST_DSGN_bULLz                    , // Unbraced Length Lz
	EN_DPST_DSGN_bULLb                    , // Lb
	EN_DPST_DSGN_bELKyValue               , // Effective Length Factor Ky Value(User)
	EN_DPST_DSGN_bELKzValue               , // Effective Length Factor Kz Value(User)
	EN_DPST_DSGN_bELKyContour             , // Effective Length Factor Ky(Auto)
	EN_DPST_DSGN_bELKzContour             , // Effective Length Factor Kz(Auto)
	EN_DPST_DSGN_bMFCmy                   , // Moment Factor Cmy
	EN_DPST_DSGN_bMFCmz                   , // Moment Factor Cmz
	EN_DPST_DSGN_bMMB1y                   , // Moment Magnifier B1y|Delta_by
	EN_DPST_DSGN_bMMB1z                   , // Moment Magnifier B1y|Delta_bz
	EN_DPST_DSGN_bMMB2y                   , // Moment Magnifier B2y|Delta_sy
	EN_DPST_DSGN_bMMB2z                   , // Moment Magnifier B2z|Delta_sy
	EN_DPST_DSGN_bValue                   , // Live Load R. Factor by Members
	EN_DPST_DSGN_bContour                 , // Live Load R. Factor by Stories
	EN_DPST_DSGN_bBCCb                    , // Bending Coefficient(C)
	EN_DPST_DSGN_bSCCv                    , // Shear Coefficient(Cv)
	EN_DPST_DSGN_bSlrs                    , // Seismic Load Resisting System by Member
	EN_DPST_DSGN_bSmsp                    , // Combined Strength Method of Solid Round/Pipe Section v890
	EN_DPST_DSGN_bSlcm                    , // Seismic Load Combination Type
	EN_DPST_DSGN_bSdgnAuto                , // Seismic Design Type(Auto)
	EN_DPST_DSGN_bSdgn                    , // Seismic Design Type(User)  
	EN_DPST_DSGN_bUlct                    , // Underground Load Combination Type
	EN_DPST_DSGN_bElemType                , // Member Type
	EN_DPST_DSGN_bWallMark                , // Wall Mark
	EN_DPST_DSGN_bSUFC                    , // Scale Up Factor for Column
	EN_DPST_DSGN_bSUEQ                    , // Scale Up Factor for EQ
	EN_DPST_DSGN_bGREQ                    , // Earthquake Resistant Grade
	EN_DPST_DSGN_bSlabRebarDir            ,
	EN_DPST_DSGN_bRebb                    ,
	EN_DPST_DSGN_bRebc                    ,
	EN_DPST_DSGN_bRebr                    ,
	EN_DPST_DSGN_bRebw                    ,
	EN_DPST_DSGN_bRbmw                    ,
	EN_DPST_DSGN_bRbms                    ,
	EN_DPST_DSGN_bRexc                    , // v811에서 추가. Rebar Exposure Condition
	EN_DPST_DSGN_bOcdf                    , // v825에서 추가. Operation Condition Factor
	EN_DPST_DSGN_bScol,
	EN_DPST_DSGN_bColc                    ,
	EN_DPST_DSGN_bCsec                    ,
	EN_DPST_DSGN_bMrft                    , // add in v865, Moment Redistribution Factor.
	EN_DPST_DSGN_bTrft                    , // add in v865, Torsion Reduction Factor.
	EN_DPST_DSGN_bSslc                    , // add in v865 
	EN_DPST_DSGN_bArib                    , 
	EN_DPST_DSGN_bAphi                    , 
	EN_DPST_DSGN_bAwed                    , 
	EN_DPST_DSGN_bAhtr                    , 
	EN_DPST_DSGN_bCsms                    ,	//Add by tss(2017.8.22)
	EN_DPST_DSGN_bCsos                    ,	//Add by tss(2017.8.22)					,
	EN_DPST_DSGN_bPurlin                , // v955 추가
	EN_DPST_DSGN_bPfmc					,
	EN_DPST_DSGN_bPfcm					,
	EN_DPST_DSGN_bPfdl					,
	EN_DPST_DSGN_bPfmp					,
	EN_DPST_DSGN_bPfnp					,
	EN_DPST_DSGN_bMcmb					, // Moment Calculation Method for Beam
	EN_DPST_DSGN_bDfba					, // Design Force for Beam Assigned as Member
	EN_DPST_DSGN_bMFCty, // v945 추가 Moment Factor Cty
	EN_DPST_DSGN_bMFCtz, // v945 추가 Moment Factor Ctz
	EN_DPST_DSGN_bImel, // v955 추가
	EN_DPST_DSGN_bRmfr, // v955 추가

	EN_DPST_DSGN_END,

	EN_DPST_DSGN_END_810 = 33,    // v811에서 EN_DPST_DSGN_bRexc 추가됨.
	EN_DPST_DSGN_END_821 = 34,    // v821에서 EN_DPST_DSGN_bOcdf 추가됨.
	EN_DPST_DSGN_END_841 = 35,    // v841에서 EN_DPST_DSGN_bColc,EN_DPST_DSGN_bCsec 추가됨.
	EN_DPST_DSGN_END_860 = 37,
	EN_DPST_DSGN_END_865 = 44,
	EN_DPST_DSGN_END_883 = 46,
	EN_DPST_DSGN_END_885 = 51,
	EN_DPST_DSGN_END_891 = 52,
	EN_DPST_DSGN_END_895 = 53,
	EN_DPST_DSGN_END_925 = 54,
	EN_DPST_DSGN_END_940 = 56,
	EN_DPST_DSGN_END_950 = 58
};

enum EN_DPST_VIEW
{
	/////////////////////// 파일에 저장되는 것이므로 변경시 버젼 분리해줄것!! ///////////////////////
	EN_DPST_VIEW_bUCS                     , // UCS Axis
	EN_DPST_VIEW_bVPoint                  , // View Point
	EN_DPST_VIEW_bDesc                    , // Description
	EN_DPST_VIEW_bLabelRotation           , // Label Orientation
	
	EN_DPST_VIEW_END
};
enum EN_DPST_D
{
	/////////////////////// 파일에 저장되는 것이므로 변경시 버젼 분리해줄것!! ///////////////////////
	EN_DPST_D_bLabelByGrup                , // Display by Group
	EN_DPST_D_bLabelBySelect              , // Display by Selection
	EN_DPST_D_bDisplayByMember            , // Display by Member
	EN_DPST_D_bIsRedrawModel              , // 내부에서 사용하고 있는 변수인듯..
	EN_DPST_D_bIsRebuildQTree             ,
	EN_DPST_D_bIsZoomAll                  ,
	EN_DPST_D_bIsInActiveVisibleChange    ,
	EN_DPST_D_bIsLabelFilterChange        ,
	EN_DPST_D_bIsLabelByGroupFilterChange ,
	EN_DPST_D_bIsHiddenLabel              , // Hidden Labels
	
	EN_DPST_D_END
};
enum EN_DPST_SEIS
{
	/////////////////////// 파일에 저장되는 것이므로 변경시 버젼 분리해줄것!! ///////////////////////
	EN_DPST_SEIS_bSech, // Clear Height of Column
	EN_DPST_SEIS_bSehk, // Use Seismic Hook
	EN_DPST_SEIS_bScmt, // Seismic Component Type
	EN_DPST_SEIS_bMfac, // m-Factor (User)
	EN_DPST_SEIS_bSerd, // Transverse Reinforcement Details
	EN_DPST_SEIS_bMisa, // Masonry Infill Strut
	EN_DPST_SEIS_bRccm, // RC Column M

	EN_DPST_SEIS_bPmcm, // P-M Curve Calculation Method
	EN_DPST_SEIS_bXfac, // XY Factor (User)
	EN_DPST_SEIS_bSebe, // Seismic Rebar Detail ( Beam )
	EN_DPST_SEIS_bSeco, // Seismic Rebar Detail ( Column )
	EN_DPST_SEIS_bSewa, // Special Boundary Element
	EN_DPST_SEIS_bHlfw, // Plastic Hinge Length for Fiber Wall
	EN_DPST_SEIS_bSesp, // Shear Span of Beam / Column
	EN_DPST_SEIS_bXxx10, // 이름 바꿔서 쓰세용
	EN_DPST_SEIS_bXxx11, // 이름 바꿔서 쓰세용
	EN_DPST_SEIS_bXxx12, // 이름 바꿔서 쓰세용
	EN_DPST_SEIS_bXxx13, // 이름 바꿔서 쓰세용
	EN_DPST_SEIS_bXxx14, // 이름 바꿔서 쓰세용

	EN_DPST_SEIS_END
};

struct T_DPST_NODE
{
	BOOL bDpstNode[EN_DPST_NODE_END];
	void Initialize()
	{
		for(int i=0; i<EN_DPST_NODE_END; i++)
			bDpstNode[i] = FALSE;
	}
	T_DPST_NODE() { Initialize(); }
	T_DPST_NODE(const T_DPST_NODE &src) { *this = src; }
};

struct T_DPST_ELEM_835
{                   
	BOOL bDpstElem[EN_DPST_ELEM_END_835];
	void Initialize()
	{
		for(int i=0; i<EN_DPST_ELEM_END_835; i++)
			bDpstElem[i] = FALSE;
	}
	T_DPST_ELEM_835() { Initialize(); }
	T_DPST_ELEM_835(const T_DPST_ELEM_835 &src) { *this = src; }
};

struct T_DPST_ELEM_940
{
	BOOL bDpstElem[EN_DPST_ELEM_END_940];
	void Initialize()
	{
		for (int i = 0; i < EN_DPST_ELEM_END_940; i++)
			bDpstElem[i] = FALSE;
	}
	T_DPST_ELEM_940() { Initialize(); }
	T_DPST_ELEM_940(const T_DPST_ELEM_940& src) { *this = src; }
};

struct T_DPST_ELEM_955
{
	BOOL bDpstElem[EN_DPST_ELEM_END_955];
	void Initialize()
	{
		for (int i = 0; i < EN_DPST_ELEM_END_955; i++)
			bDpstElem[i] = FALSE;
	}
	T_DPST_ELEM_955() { Initialize(); }
	T_DPST_ELEM_955(const T_DPST_ELEM_955& src) { *this = src; }
};

struct T_DPST_ELEM_965
{
	BOOL bDpstElem[EN_DPST_ELEM_END_965];
	void Initialize()
	{
		for (int i = 0; i < EN_DPST_ELEM_END_965; i++)
			bDpstElem[i] = FALSE;
	}
	T_DPST_ELEM_965() { Initialize(); }
	T_DPST_ELEM_965(const T_DPST_ELEM_965& src) { *this = src; }
};

struct T_DPST_ELEM
{
	BOOL bDpstElem[EN_DPST_ELEM_END];
	void Initialize()
	{
		for(int i=0; i<EN_DPST_ELEM_END; i++)
			bDpstElem[i] = FALSE;
	}
	T_DPST_ELEM() { Initialize(); }
	T_DPST_ELEM(const T_DPST_ELEM &src) { *this = src; }

	void Convert835(T_DPST_ELEM_835& src)
	{
		for(int i=0; i<EN_DPST_ELEM_END_835; i++)  bDpstElem[i] = src.bDpstElem[i];
	}
	void Convert940(T_DPST_ELEM_940& src)
	{
		for (int i = 0; i < EN_DPST_ELEM_END_940; i++)  bDpstElem[i] = src.bDpstElem[i];
	}
	void Convert955(T_DPST_ELEM_955& src)
	{
		for (int i = 0; i < EN_DPST_ELEM_END_955; i++)  bDpstElem[i] = src.bDpstElem[i];
	}
	void Convert965(T_DPST_ELEM_965& src)
	{
		for (int i = 0; i < EN_DPST_ELEM_END_965; i++)  bDpstElem[i] = src.bDpstElem[i];
	}
};
struct T_DPST_PROP_955
{
	BOOL bDpstProp[EN_DPST_PROP_END_955];
};

struct T_DPST_PROP
{                   
	BOOL bDpstProp[EN_DPST_PROP_END];
	void Initialize()
	{
		for(int i=0; i<EN_DPST_PROP_END; i++)
			bDpstProp[i] = FALSE;
	}
	T_DPST_PROP() { Initialize(); }
	T_DPST_PROP(const T_DPST_PROP &src) { *this = src; }

	void Convert955(T_DPST_PROP_955& src)
	{
		Initialize();
		for (int i = 0; i < EN_DPST_PROP_END_955; i++)  bDpstProp[i] = src.bDpstProp[i];
	}
};

struct T_DPST_BNDR_855
{
	CArray<UINT, UINT> aBngrK     ; // Boudary Keys (Group Selection 일때)
	BOOL bDpstBndr[EN_DPST_BNDR_END_855];

	void Initialize()
	{
		aBngrK.RemoveAll();
		for(int i=0; i<EN_DPST_BNDR_END_855; i++)	
			bDpstBndr[i] = FALSE;
	}
	T_DPST_BNDR_855() { Initialize(); }
	T_DPST_BNDR_855(const T_DPST_BNDR_855 &src) { *this = src; }
	T_DPST_BNDR_855& operator=(const T_DPST_BNDR_855 &src)
	{
		aBngrK.Copy(src.aBngrK);
		for(int i=0; i<EN_DPST_BNDR_END_855; i++)	
			bDpstBndr[i] = src.bDpstBndr[i];
		return *this;
	}
};

struct T_DPST_BNDR
{
	CArray<UINT, UINT> aBngrK     ; // Boudary Keys (Group Selection 일때)
	BOOL bDpstBndr[EN_DPST_BNDR_END];
	void Initialize()
	{
		aBngrK.RemoveAll();
		for(int i=0; i<EN_DPST_BNDR_END; i++)
			bDpstBndr[i] = FALSE;
	}
	T_DPST_BNDR() { Initialize(); }
	T_DPST_BNDR(const T_DPST_BNDR &src) { *this = src; }
	T_DPST_BNDR& operator=(const T_DPST_BNDR &src)
	{
		aBngrK.Copy(src.aBngrK);
		memcpy(bDpstBndr, src.bDpstBndr, sizeof(bDpstBndr));
		return *this;
	}
	void Convert855(T_DPST_BNDR_855& src)
	{
	  aBngrK.Copy(src.aBngrK);
	  for(int i=0; i<EN_DPST_BNDR_END_855; i++)  bDpstBndr[i] = src.bDpstBndr[i];
	}
};

struct T_DPST_BNDR_RW_855
{
	int  nCount_aBngrK;
	BOOL bDpstBndr[EN_DPST_BNDR_END_855];
	void Get(T_DPST_BNDR_855 &rData)
	{
		rData.aBngrK.SetSize(nCount_aBngrK);
		memcpy(rData.bDpstBndr, bDpstBndr, sizeof(rData.bDpstBndr));
	}
	void Set(T_DPST_BNDR_855 Data)
	{
		nCount_aBngrK = (int)Data.aBngrK.GetSize();
		memcpy(bDpstBndr, Data.bDpstBndr, sizeof(bDpstBndr));
	}
};

struct T_DPST_BNDR_RW_940
{
	int  nCount_aBngrK;
	BOOL bDpstBndr[EN_DPST_BNDR_END_940];
	void Get(T_DPST_BNDR& rData)
	{
		rData.aBngrK.SetSize(nCount_aBngrK);
		memcpy(rData.bDpstBndr, bDpstBndr, sizeof(rData.bDpstBndr));
	}
	void Set(T_DPST_BNDR Data)
	{
		nCount_aBngrK = (int)Data.aBngrK.GetSize();
		memcpy(bDpstBndr, Data.bDpstBndr, sizeof(bDpstBndr));
	}
};

struct T_DPST_BNDR_RW_950
{
	int  nCount_aBngrK;
	BOOL bDpstBndr[EN_DPST_BNDR_END_950];
	void Get(T_DPST_BNDR& rData)
	{
		rData.aBngrK.SetSize(nCount_aBngrK);
		memcpy(rData.bDpstBndr, bDpstBndr, sizeof(rData.bDpstBndr));
	}
	void Set(T_DPST_BNDR Data)
	{
		nCount_aBngrK = (int)Data.aBngrK.GetSize();
		memcpy(bDpstBndr, Data.bDpstBndr, sizeof(bDpstBndr));
	}
};

struct T_DPST_BNDR_RW
{
	int  nCount_aBngrK;
	BOOL bDpstBndr[EN_DPST_BNDR_END];
	void Get(T_DPST_BNDR &rData)
	{
		rData.aBngrK.SetSize(nCount_aBngrK);
		memcpy(rData.bDpstBndr, bDpstBndr, sizeof(rData.bDpstBndr));
	}
	void Set(T_DPST_BNDR Data)
	{
		nCount_aBngrK = (int)Data.aBngrK.GetSize();
		memcpy(bDpstBndr, Data.bDpstBndr, sizeof(bDpstBndr));
	}
	void Convert855(T_DPST_BNDR_RW_855& Data)
	{
		for (int i = 0; i < EN_DPST_BNDR_END; i++)
			bDpstBndr[i] = FALSE;

		nCount_aBngrK = Data.nCount_aBngrK;
		for ( int i = 0; i < EN_DPST_BNDR_END_855; i++ )  bDpstBndr[i] = Data.bDpstBndr[i];
	}
	void Convert940(T_DPST_BNDR_RW_940& Data)
	{
		for (int i = 0; i < EN_DPST_BNDR_END; i++)
			bDpstBndr[i] = FALSE;

		nCount_aBngrK = Data.nCount_aBngrK;
		for (int i = 0; i < EN_DPST_BNDR_END_940; i++)  bDpstBndr[i] = Data.bDpstBndr[i];
	}
	void Convert950_2(T_DPST_BNDR_RW_950& Data)
	{
		for (int i = 0; i < EN_DPST_BNDR_END; i++)
			bDpstBndr[i] = FALSE;

		nCount_aBngrK = Data.nCount_aBngrK;
		for (int i = 0; i < EN_DPST_BNDR_END_950; i++)  bDpstBndr[i] = Data.bDpstBndr[i];
	}
};

struct T_DPST_LOAD_865
{    
	int   nOption            ;  // 0: All, 1: Load Case Selection, 2: Load Group Selection
	int   nLoadCase          ;  // Loadcase Key
	int   nLoadCaseIndex     ;  // Loadcase combo list의 index
	UINT  nLoadCaseType      ;  // Loadcase Type
	CArray<UINT, UINT> aLdgrK;  // Load Group Key
	int   nFormatType        ;  // Load Value Output Format (0: Default, 1: Fixed, 2: Scientific)
	int   nPlace             ;  // Load Value Output Format의 Place
	BOOL bDpstLoad[EN_DPST_LOAD_END_865];
	void Initialize()
	{
		nOption               = 0;
		nLoadCase             = 0;
		nLoadCaseIndex        = 0;
		nLoadCaseType         = 0;
		aLdgrK                .RemoveAll();
		nFormatType           = 0;
		nPlace                = 0;
		for(int i=0; i<EN_DPST_LOAD_END_865; i++)
			bDpstLoad[i] = FALSE;
	}
	T_DPST_LOAD_865() { Initialize(); }
	T_DPST_LOAD_865(const T_DPST_LOAD_865 &src) { *this = src; }
	T_DPST_LOAD_865& operator=(const T_DPST_LOAD_865 &src)
	{
		nOption        = src.nOption       ;
		nLoadCase      = src.nLoadCase     ;
		nLoadCaseIndex = src.nLoadCaseIndex;
		nLoadCaseType  = src.nLoadCaseType ;
		aLdgrK         .Copy(src.aLdgrK )  ;
		nFormatType    = src.nFormatType   ;
		nPlace         = src.nPlace        ;
		memcpy(bDpstLoad, src.bDpstLoad, sizeof(bDpstLoad));
		return *this;
	}
};

struct T_DPST_LOAD_883
{    
	int   nOption            ;  // 0: All, 1: Load Case Selection, 2: Load Group Selection
	int   nLoadCase          ;  // Loadcase Key
	int   nLoadCaseIndex     ;  // Loadcase combo list의 index
	UINT  nLoadCaseType      ;  // Loadcase Type
	CArray<UINT, UINT> aLdgrK;  // Load Group Key
	int   nFormatType        ;  // Load Value Output Format (0: Default, 1: Fixed, 2: Scientific)
	int   nPlace             ;  // Load Value Output Format의 Place
	BOOL bDpstLoad[EN_DPST_LOAD_END_883];
	void Initialize()
	{
		nOption               = 0;
		nLoadCase             = 0;
		nLoadCaseIndex        = 0;
		nLoadCaseType         = 0;
		aLdgrK                .RemoveAll();
		nFormatType           = 0;
		nPlace                = 0;
		for(int i=0; i<EN_DPST_LOAD_END_883; i++)
			bDpstLoad[i] = FALSE;
	}
	T_DPST_LOAD_883() { Initialize(); }
	T_DPST_LOAD_883(const T_DPST_LOAD_883 &src) { *this = src; }
	T_DPST_LOAD_883& operator=(const T_DPST_LOAD_883 &src)
	{
		nOption        = src.nOption       ;
		nLoadCase      = src.nLoadCase     ;
		nLoadCaseIndex = src.nLoadCaseIndex;
		nLoadCaseType  = src.nLoadCaseType ;
		aLdgrK         .Copy(src.aLdgrK )  ;
		nFormatType    = src.nFormatType   ;
		nPlace         = src.nPlace        ;
		memcpy(bDpstLoad, src.bDpstLoad, sizeof(bDpstLoad));
		return *this;
	}
};

// struct T_DPST_LOAD_940
// {
// 	int   nOption;  // 0: All, 1: Load Case Selection, 2: Load Group Selection
// 	int   nLoadCase;  // Loadcase Key
// 	int   nLoadCaseIndex;  // Loadcase combo list의 index
// 	UINT  nLoadCaseType;  // Loadcase Type
// 	CArray<UINT, UINT> aLdgrK;  // Load Group Key
// 	int   nFormatType;  // Load Value Output Format (0: Default, 1: Fixed, 2: Scientific)
// 	int   nPlace;  // Load Value Output Format의 Place
// 	BOOL  bDpstLoad[EN_DPST_LOAD_END_940];
// 
// 	void Initialize()
// 	{
// 		nOption = 0;
// 		nLoadCase = 0;
// 		nLoadCaseIndex = 0;
// 		nLoadCaseType = 0;
// 		aLdgrK.RemoveAll();
// 		nFormatType = 0;
// 		nPlace = 0;
// 		memset(bDpstLoad, 0, sizeof(bDpstLoad));
// 	}
// 	T_DPST_LOAD_940() { Initialize(); }
// 	T_DPST_LOAD_940(const T_DPST_LOAD_940 &src) { *this = src; }
// 	T_DPST_LOAD_940& operator=(const T_DPST_LOAD_940 &src)
// 	{
// 		nOption = src.nOption;
// 		nLoadCase = src.nLoadCase;
// 		nLoadCaseIndex = src.nLoadCaseIndex;
// 		nLoadCaseType = src.nLoadCaseType;
// 		aLdgrK.Copy(src.aLdgrK);
// 		nFormatType = src.nFormatType;
// 		nPlace = src.nPlace;
// 		memcpy(bDpstLoad, src.bDpstLoad, sizeof(bDpstLoad));
// 		return *this;
// 	}
// };

struct T_DPST_LOAD
{
	int   nOption            ;  // 0: All, 1: Load Case Selection, 2: Load Group Selection
	int   nLoadCase          ;  // Loadcase Key
	int   nLoadCaseIndex     ;  // Loadcase combo list의 index
	UINT  nLoadCaseType      ;  // Loadcase Type
	CArray<UINT, UINT> aLdgrK;  // Load Group Key
	int   nFormatType        ;  // Load Value Output Format (0: Default, 1: Fixed, 2: Scientific)
	int   nPlace             ;  // Load Value Output Format의 Place
	BOOL  bDpstLoad[EN_DPST_LOAD_END];
	void Initialize()
	{
		nOption               = 0;
		nLoadCase             = 0;
		nLoadCaseIndex        = 0;
		nLoadCaseType         = 0;
		aLdgrK                .RemoveAll();
		nFormatType           = 0;
		nPlace                = 0;
		memset(bDpstLoad, 0, sizeof(bDpstLoad));
	}
	T_DPST_LOAD() { Initialize(); }
	T_DPST_LOAD(const T_DPST_LOAD &src) { *this = src; }
	T_DPST_LOAD& operator=(const T_DPST_LOAD &src)
	{
		nOption        = src.nOption       ;
		nLoadCase      = src.nLoadCase     ;
		nLoadCaseIndex = src.nLoadCaseIndex;
		nLoadCaseType  = src.nLoadCaseType ;
		aLdgrK         .Copy(src.aLdgrK )  ;
		nFormatType    = src.nFormatType   ;
		nPlace         = src.nPlace        ;
		memcpy(bDpstLoad, src.bDpstLoad, sizeof(bDpstLoad));
		return *this;
	}
// 	void Convert940(T_DPST_LOAD_940& src)
// 	{
// 		nOption        = src.nOption       ;
// 		nLoadCase      = src.nLoadCase     ;
// 		nLoadCaseIndex = src.nLoadCaseIndex;
// 		nLoadCaseType  = src.nLoadCaseType ;
// 		aLdgrK         .Copy(src.aLdgrK )  ;
// 		nFormatType    = src.nFormatType   ;
// 		nPlace         = src.nPlace        ;
// 
// 		for (int i = 0; i < EN_DPST_LOAD_END; i++)
// 		{
// 			if(i <EN_DPST_LOAD_END_940)
// 				bDpstLoad[i] = src.bDpstLoad[i];
// 			else
// 				bDpstLoad[i] = FALSE;
// 		}
// 	}
	void Convert883(T_DPST_LOAD_883& src)
	{
		nOption        = src.nOption       ;
		nLoadCase      = src.nLoadCase     ;
		nLoadCaseIndex = src.nLoadCaseIndex;
		nLoadCaseType  = src.nLoadCaseType ;
		aLdgrK         .Copy(src.aLdgrK )  ;
		nFormatType    = src.nFormatType   ;
		nPlace         = src.nPlace        ;
		memcpy(bDpstLoad, src.bDpstLoad, sizeof(src.bDpstLoad));
	}
	void Convert865(T_DPST_LOAD_865& src)
	{
		nOption        = src.nOption       ;
		nLoadCase      = src.nLoadCase     ;
		nLoadCaseIndex = src.nLoadCaseIndex;
		nLoadCaseType  = src.nLoadCaseType ;
		aLdgrK         .Copy(src.aLdgrK )  ;
		nFormatType    = src.nFormatType   ;
		nPlace         = src.nPlace        ;
		for(int i=0; i<EN_DPST_LOAD_bPseudoSeismic; i++)
		{
			bDpstLoad[i] = src.bDpstLoad[i];
		}
		bDpstLoad[EN_DPST_LOAD_bPseudoSeismic] = 0;
		for(int i=EN_DPST_LOAD_bLoadToMass; i<EN_DPST_LOAD_bPseudoSeismicTxt; i++)
		{
			bDpstLoad[i] = src.bDpstLoad[i-1];
		}
		bDpstLoad[EN_DPST_LOAD_bPseudoSeismicTxt] = 0;
		for(int i=EN_DPST_LOAD_bLoadToMassTxt; i<EN_DPST_LOAD_END; i++)
		{
			bDpstLoad[i] = src.bDpstLoad[i-2];
		}
	}
};
struct T_DPST_LOAD_RW_865
{
	int   nOption;
	int   nLoadCase;
	int   nLoadCaseIndex; 
	UINT  nLoadCaseType;
	int   nCount_aLdgrK;
	//char  strFormat[20];
	int   nFormatType;
	int   nPlace;
	BOOL  bDpstLoad[EN_DPST_LOAD_END_865];
	void Get(T_DPST_LOAD_865 &rData)
	{
		rData.nOption        = nOption;
		rData.nLoadCase      = nLoadCase;
		rData.nLoadCaseIndex = nLoadCaseIndex; 
		rData.nLoadCaseType  = nLoadCaseType;
		rData.aLdgrK.SetSize(nCount_aLdgrK);
		rData.nFormatType = nFormatType;
		rData.nPlace      = nPlace;
		memcpy(rData.bDpstLoad, bDpstLoad, sizeof(rData.bDpstLoad));
	}
	void Set(T_DPST_LOAD_865 Data)
	{
		nOption        = Data.nOption;
		nLoadCase      = Data.nLoadCase;
		nLoadCaseIndex = Data.nLoadCaseIndex; 
		nLoadCaseType  = Data.nLoadCaseType;
		nCount_aLdgrK  = (int)Data.aLdgrK.GetSize();
		nFormatType = Data.nFormatType;
		nPlace      = Data.nPlace;
		memcpy(bDpstLoad, Data.bDpstLoad, sizeof(bDpstLoad));
	}
};
struct T_DPST_LOAD_RW_883
{
	int   nOption;
	int   nLoadCase;
	int   nLoadCaseIndex; 
	UINT  nLoadCaseType;
	int   nCount_aLdgrK;
	int   nFormatType;
	int   nPlace;
	BOOL  bDpstLoad[EN_DPST_LOAD_END_883];
	void Get(T_DPST_LOAD_883 &rData)
	{
		rData.nOption        = nOption;
		rData.nLoadCase      = nLoadCase;
		rData.nLoadCaseIndex = nLoadCaseIndex; 
		rData.nLoadCaseType  = nLoadCaseType;
		rData.aLdgrK.SetSize(nCount_aLdgrK);
		rData.nFormatType = nFormatType;
		rData.nPlace      = nPlace;
		memcpy(rData.bDpstLoad, bDpstLoad, sizeof(rData.bDpstLoad));
	}
	void Set(T_DPST_LOAD_883 Data)
	{
		nOption        = Data.nOption;
		nLoadCase      = Data.nLoadCase;
		nLoadCaseIndex = Data.nLoadCaseIndex; 
		nLoadCaseType  = Data.nLoadCaseType;
		nCount_aLdgrK  = (int)Data.aLdgrK.GetSize();
		nFormatType = Data.nFormatType;
		nPlace      = Data.nPlace;
		memcpy(bDpstLoad, Data.bDpstLoad, sizeof(bDpstLoad));
	}
};

struct T_DPST_LOAD_RW_940
{
	int   nOption;
	int   nLoadCase;
	int   nLoadCaseIndex;
	UINT  nLoadCaseType;
	int   nCount_aLdgrK;
	//char  strFormat[20];
	int   nFormatType;
	int   nPlace;
	BOOL  bDpstLoad[EN_DPST_LOAD_END_940];
};

struct T_DPST_LOAD_RW
{
	int   nOption;
	int   nLoadCase;
	int   nLoadCaseIndex; 
	UINT  nLoadCaseType;
	int   nCount_aLdgrK;
	//char  strFormat[20];
	int   nFormatType;
	int   nPlace;
	BOOL  bDpstLoad[EN_DPST_LOAD_END];
	void Get(T_DPST_LOAD &rData)
	{
		rData.nOption        = nOption;
		rData.nLoadCase      = nLoadCase;
		rData.nLoadCaseIndex = nLoadCaseIndex; 
		rData.nLoadCaseType  = nLoadCaseType;
		rData.aLdgrK.SetSize(nCount_aLdgrK);
		rData.nFormatType = nFormatType;
		rData.nPlace      = nPlace;
		memcpy(rData.bDpstLoad, bDpstLoad, sizeof(rData.bDpstLoad));
	}
	void Set(T_DPST_LOAD Data)
	{
		nOption        = Data.nOption;
		nLoadCase      = Data.nLoadCase;
		nLoadCaseIndex = Data.nLoadCaseIndex; 
		nLoadCaseType  = Data.nLoadCaseType;
		nCount_aLdgrK  = (int)Data.aLdgrK.GetSize();
		nFormatType = Data.nFormatType;
		nPlace      = Data.nPlace;
		memcpy(bDpstLoad, Data.bDpstLoad, sizeof(bDpstLoad));
	}
	void Convert940(T_DPST_LOAD_RW_940& src)
	{
		nOption        = src.nOption;
		nLoadCase      = src.nLoadCase;
		nLoadCaseIndex = src.nLoadCaseIndex; 
		nLoadCaseType  = src.nLoadCaseType;
		nCount_aLdgrK  = src.nCount_aLdgrK;
		nFormatType    = src.nFormatType;
		nPlace         = src.nPlace;
		for (int i = 0; i < EN_DPST_LOAD_END; i++)
		{
			if(i < EN_DPST_LOAD_END_940)
				bDpstLoad[i] = src.bDpstLoad[i];
			else
				bDpstLoad[i] = FALSE;
		}
	}
	void Convert883(T_DPST_LOAD_RW_883& src)
	{
		nOption        = src.nOption;
		nLoadCase      = src.nLoadCase;
		nLoadCaseIndex = src.nLoadCaseIndex; 
		nLoadCaseType  = src.nLoadCaseType;
		nCount_aLdgrK  = src.nCount_aLdgrK;
		nFormatType    = src.nFormatType;
		nPlace         = src.nPlace;
		for (int i = 0; i < EN_DPST_LOAD_END; i++)
		{
			if (i < EN_DPST_LOAD_END_883)
				bDpstLoad[i] = src.bDpstLoad[i];
			else
				bDpstLoad[i] = FALSE;
		}
	}
	void Convert865(T_DPST_LOAD_RW_865& src)
	{
		nOption        = src.nOption;
		nLoadCase      = src.nLoadCase;
		nLoadCaseIndex = src.nLoadCaseIndex; 
		nLoadCaseType  = src.nLoadCaseType;
		nCount_aLdgrK  = src.nCount_aLdgrK;
		nFormatType    = src.nFormatType;
		nPlace         = src.nPlace;
		for (int i = 0; i < EN_DPST_LOAD_END; i++)
		{
			if (i < EN_DPST_LOAD_END_865)
				bDpstLoad[i] = src.bDpstLoad[i];
			else
				bDpstLoad[i] = FALSE;
		}
	}
};

struct T_DPST_DSGN_810
{                   
	BOOL bDpstDsgn[EN_DPST_DSGN_END_810];
	void Initialize()
	{
		for(int i=0; i<EN_DPST_DSGN_END_810; i++)
			bDpstDsgn[i] = FALSE;
	}
	T_DPST_DSGN_810() { Initialize(); }
	T_DPST_DSGN_810(const T_DPST_DSGN_810 &src) { *this = src; }
};

struct T_DPST_DSGN_821
{                   
	BOOL bDpstDsgn[EN_DPST_DSGN_END_821];
	void Initialize()
	{
		for(int i=0; i<EN_DPST_DSGN_END_821; i++)
			bDpstDsgn[i] = FALSE;
	}
	T_DPST_DSGN_821() { Initialize(); }
	T_DPST_DSGN_821(const T_DPST_DSGN_821 &src) { *this = src; }
};

struct T_DPST_DSGN_841
{                   
	BOOL bDpstDsgn[EN_DPST_DSGN_END_841];
	void Initialize()
	{
		for(int i=0; i<EN_DPST_DSGN_END_841; i++)
			bDpstDsgn[i] = FALSE;
	}
	T_DPST_DSGN_841() { Initialize(); }
	T_DPST_DSGN_841(const T_DPST_DSGN_841 &src) { *this = src; }
};

struct T_DPST_DSGN_860
{                   
	BOOL bDpstDsgn[EN_DPST_DSGN_END_860];
	void Initialize()
	{
		for(int i=0; i<EN_DPST_DSGN_END_860; i++)
			bDpstDsgn[i] = FALSE;
	}
	T_DPST_DSGN_860() { Initialize(); }
	T_DPST_DSGN_860(const T_DPST_DSGN_860 &src) { *this = src; }
};

struct T_DPST_DSGN_865
{                   
	BOOL bDpstDsgn[EN_DPST_DSGN_END_865];
	void Initialize()
	{
		for(int i=0; i<EN_DPST_DSGN_END_865; i++)
			bDpstDsgn[i] = FALSE;
	}
	T_DPST_DSGN_865() { Initialize(); }
	T_DPST_DSGN_865(const T_DPST_DSGN_865 &src) { *this = src; }
};

struct T_DPST_DSGN_883
{                   
	BOOL bDpstDsgn[EN_DPST_DSGN_END_883];
	void Initialize()
	{
		for(int i=0; i<EN_DPST_DSGN_END_883; i++)
			bDpstDsgn[i] = FALSE;
	}
	T_DPST_DSGN_883() { Initialize(); }
	T_DPST_DSGN_883(const T_DPST_DSGN_883 &src) { *this = src; }
};

struct T_DPST_DSGN_885
{                   
	BOOL bDpstDsgn[EN_DPST_DSGN_END_885];
	void Initialize()
	{ 
		for(int i=0; i<EN_DPST_DSGN_END_885; i++)
			bDpstDsgn[i] = FALSE;
	}
	T_DPST_DSGN_885() { Initialize(); }
	T_DPST_DSGN_885(const T_DPST_DSGN_885 &src) { *this = src; }
};

struct T_DPST_DSGN_891
{                   
	BOOL bDpstDsgn[EN_DPST_DSGN_END_891];
	void Initialize()
	{ 
		for(int i=0; i<EN_DPST_DSGN_END_891; i++)
			bDpstDsgn[i] = FALSE;
	}
	T_DPST_DSGN_891() { Initialize(); }
	T_DPST_DSGN_891(const T_DPST_DSGN_891 &src) { *this = src; }
};

struct T_DPST_DSGN_895
{                   
	BOOL bDpstDsgn[EN_DPST_DSGN_END_895];
	void Initialize()
	{ 
		for(int i=0; i< EN_DPST_DSGN_END_895; i++)
			bDpstDsgn[i] = FALSE;
	}
	T_DPST_DSGN_895() { Initialize(); }
	T_DPST_DSGN_895(const T_DPST_DSGN_895 &src) { *this = src; }
};

struct T_DPST_DSGN_925
{
	BOOL bDpstDsgn[EN_DPST_DSGN_END_925];
	void Initialize()
	{
		for (int i = 0; i < EN_DPST_DSGN_END_925; i++)
			bDpstDsgn[i] = FALSE;
	}
	T_DPST_DSGN_925() { Initialize(); }
	T_DPST_DSGN_925(const T_DPST_DSGN_925 &src) { *this = src; }
};

struct T_DPST_DSGN_940
{
	BOOL bDpstDsgn[EN_DPST_DSGN_END_940];
	void Initialize()
	{
		for (int i = 0; i < EN_DPST_DSGN_END_940; i++)
			bDpstDsgn[i] = FALSE;
	}
	T_DPST_DSGN_940() { Initialize(); }
	T_DPST_DSGN_940(const T_DPST_DSGN_940& src) { *this = src; }
};

struct T_DPST_DSGN_950
{
	BOOL bDpstDsgn[EN_DPST_DSGN_END_950];
	void Initialize()
	{
		for (int i = 0; i < EN_DPST_DSGN_END_950; i++)
			bDpstDsgn[i] = FALSE;
	}
	T_DPST_DSGN_950() { Initialize(); }
	T_DPST_DSGN_950(const T_DPST_DSGN_950& src) { *this = src; }
};

struct T_DPST_DSGN
{                   
	BOOL bDpstDsgn[EN_DPST_DSGN_END];
	void Initialize()
	{ 
		for(int i=0; i<EN_DPST_DSGN_END; i++)
			bDpstDsgn[i] = FALSE;
	}
	T_DPST_DSGN() { Initialize(); }
	T_DPST_DSGN(const T_DPST_DSGN &src) { *this = src; }
	void Convert950(T_DPST_DSGN_950& src)
	{
		for (int i = 0; i < EN_DPST_DSGN_END_950; i++)
		{
			bDpstDsgn[i] = src.bDpstDsgn[i];
		}

		for (int i = EN_DPST_DSGN_END_950; i < EN_DPST_DSGN_END; i++)
		{
			bDpstDsgn[i] = FALSE;
		}
	}
	void Convert940(T_DPST_DSGN_940& src)
	{
		for (int i = 0; i < EN_DPST_DSGN_END_940; i++)
		{
			bDpstDsgn[i] = src.bDpstDsgn[i];
		}

		for (int i = EN_DPST_DSGN_END_940; i < EN_DPST_DSGN_END; i++)
		{
			bDpstDsgn[i] = FALSE;
		}
	}
	void Convert925(T_DPST_DSGN_925& src)
	{
		for (int i = 0; i < EN_DPST_DSGN_END_925; i++)
		{
			bDpstDsgn[i] = src.bDpstDsgn[i];
		}

		for (int i = EN_DPST_DSGN_END_925; i < EN_DPST_DSGN_END; i++)
		{
			bDpstDsgn[i] = FALSE;
		}
	}
	void Convert895(T_DPST_DSGN_895& src)
	{
		for (int i = 0; i < EN_DPST_DSGN_bUlct; i++)
		{
			bDpstDsgn[i] = src.bDpstDsgn[i];
		}
		bDpstDsgn[EN_DPST_DSGN_bUlct] = FALSE; //896추가
		for (int i = EN_DPST_DSGN_bElemType; i < EN_DPST_DSGN_END_895; i++)
		{
			bDpstDsgn[i] = src.bDpstDsgn[i - 1];
		}
	}
	void Convert891(T_DPST_DSGN_891& src)
	{
		for(int i=0; i<EN_DPST_DSGN_bScol; i++)
		{
			bDpstDsgn[i] = src.bDpstDsgn[i];
		}
		bDpstDsgn[EN_DPST_DSGN_bScol] = FALSE; //895추가
		for(int i=EN_DPST_DSGN_bColc; i<EN_DPST_DSGN_END_891; i++)
		{
			bDpstDsgn[i] = src.bDpstDsgn[i-1];
		}
	}
	void Convert885(T_DPST_DSGN_885& src)
	{
		for(int i=0; i<EN_DPST_DSGN_bSmsp; i++)
		{
			bDpstDsgn[i] = src.bDpstDsgn[i];
		}
		bDpstDsgn[EN_DPST_DSGN_bSmsp] = FALSE; //890추가
		for(int i=EN_DPST_DSGN_bSlcm; i<EN_DPST_DSGN_END_885; i++)
		{
			bDpstDsgn[i] = src.bDpstDsgn[i-1];
		}
	}
	void Convert883(T_DPST_DSGN_883& src)
	{
		for(int i=0; i<EN_DPST_DSGN_END_883; i++)  bDpstDsgn[i] = src.bDpstDsgn[i];
	}
	void Convert865(T_DPST_DSGN_865& src)
	{
		for(int i=0; i<EN_DPST_DSGN_END_865; i++)  bDpstDsgn[i] = src.bDpstDsgn[i];
	}
	void Convert860(T_DPST_DSGN_860& src)
	{
		for(int i=0; i<EN_DPST_DSGN_END_860; i++)  bDpstDsgn[i] = src.bDpstDsgn[i];
	}
	void Convert841(T_DPST_DSGN_841& src)
	{
		for(int i=0; i<EN_DPST_DSGN_END_841; i++)  bDpstDsgn[i] = src.bDpstDsgn[i];
	}
	void Convert821(T_DPST_DSGN_821& src)
	{
		for(int i=0; i<EN_DPST_DSGN_END_821; i++)  bDpstDsgn[i] = src.bDpstDsgn[i];
	}
	void Convert810(T_DPST_DSGN_810& src)
	{
		for(int i=0; i<EN_DPST_DSGN_END_810; i++)  bDpstDsgn[i] = src.bDpstDsgn[i];
	}
};

struct T_DPST_VIEW_930
{
	CString   strDesc;  // Description 문자열
	LOGFONTA  lfDescFont;  // Font
	COLORREF  DescColor;  // Font color
	CRect     rectDesc;  // Description 영역
	double    dDescRatioX;  // Description X 비율
	double    dDescRatioY;  // Description Y 비율
	double    dRotationAngle;  // Label Orientation
	BOOL      bDpstView[EN_DPST_VIEW_END];
};

struct T_DPST_VIEW
{
	CString   strDesc        ;  // Description 문자열
	LOGFONT   lfDescFont     ;  // Font
	COLORREF  DescColor      ;  // Font color
	CRect     rectDesc       ;  // Description 영역
	double    dDescRatioX    ;  // Description X 비율
	double    dDescRatioY    ;  // Description Y 비율
	double    dRotationAngle ;  // Label Orientation
	BOOL      bDpstView[EN_DPST_VIEW_END];
	void Initialize()
	{
		strDesc        = _T("");
		LOGFONT lf;
		{
			//sprintf(lf.lfFaceName, "%s", _LS(IDS_WG_GR_FONT___MSG11));
			lf.lfWidth         = 0  ;
			lf.lfHeight        = 120; 
			lf.lfItalic        = FALSE;
			lf.lfWeight        = FW_BOLD;
			lf.lfUnderline     = FALSE;
			lf.lfStrikeOut     = FALSE;
			// Log Font Default
			lf.lfEscapement    = lf.lfOrientation = 0;
			lf.lfCharSet       = DEFAULT_CHARSET;
			lf.lfOutPrecision  = OUT_TT_ONLY_PRECIS;
			lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
			lf.lfQuality       = DEFAULT_QUALITY;
		}
		memcpy(&lfDescFont, &lf, sizeof(LOGFONT));
		DescColor      = 0;
		rectDesc       .SetRect(30,20,100,40);
		dDescRatioX    = 0.;
		dDescRatioY    = 0.;
		dRotationAngle = 0.;
		for(int i=0; i<EN_DPST_VIEW_END; i++)
			bDpstView[i] = FALSE;
	}
	T_DPST_VIEW() { Initialize(); }
	T_DPST_VIEW(const T_DPST_VIEW &src) { *this = src; }


	void Convert930(T_DPST_VIEW_930 Data)
	{
		strDesc = Data.strDesc;
		DescColor = Data.DescColor;
		rectDesc = Data.rectDesc;
		dDescRatioX = Data.dDescRatioX;
		dDescRatioY = Data.dDescRatioY;
		dRotationAngle = Data.dRotationAngle;
		memcpy(bDpstView, Data.bDpstView, sizeof(bDpstView));

		//////////////////////////////////////////////////////////////////////////
		//lfDescFont = Data.lfDescFont;
		lfDescFont.lfHeight = Data.lfDescFont.lfHeight;
		lfDescFont.lfWidth = Data.lfDescFont.lfWidth;
		lfDescFont.lfEscapement = Data.lfDescFont.lfEscapement;
		lfDescFont.lfOrientation = Data.lfDescFont.lfOrientation;
		lfDescFont.lfWeight = Data.lfDescFont.lfWeight;
		lfDescFont.lfItalic = Data.lfDescFont.lfItalic;
		lfDescFont.lfUnderline = Data.lfDescFont.lfUnderline;
		lfDescFont.lfStrikeOut = Data.lfDescFont.lfStrikeOut;
		lfDescFont.lfCharSet = Data.lfDescFont.lfCharSet;
		lfDescFont.lfOutPrecision = Data.lfDescFont.lfOutPrecision;
		lfDescFont.lfClipPrecision = Data.lfDescFont.lfClipPrecision;
		lfDescFont.lfQuality = Data.lfDescFont.lfQuality;
		lfDescFont.lfPitchAndFamily = Data.lfDescFont.lfPitchAndFamily;

		WCHAR strUnicode[LF_FACESIZE] = { 0, };
		CHAR strMultibyte[LF_FACESIZE] = { 0, };
		strcpy_s(strMultibyte, LF_FACESIZE, Data.lfDescFont.lfFaceName);
		int nLen = MultiByteToWideChar(CP_ACP, 0, strMultibyte, (int)strlen(strMultibyte), NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, strMultibyte, (int)strlen(strMultibyte), strUnicode, nLen);

		lstrcpy(lfDescFont.lfFaceName, strUnicode);
		//////////////////////////////////////////////////////////////////////////
	}
};
struct T_DPST_VIEW_CH_930
{
	char      strDesc[60];
	LOGFONTA  lfDescFont;
	COLORREF  DescColor;
	CRect     rectDesc;
	double    dDescRatioX;
	double    dDescRatioY;
	double    dRotationAngle;
	BOOL      bDpstView[EN_DPST_VIEW_END];
	
	void Get(T_DPST_VIEW_930 &rData)
	{
		ConvertCharStr(strDesc, rData.strDesc, sizeof(strDesc));
		rData.lfDescFont = lfDescFont;
		rData.DescColor = DescColor;
		rData.rectDesc = rectDesc;
		rData.dDescRatioX = dDescRatioX;
		rData.dDescRatioY = dDescRatioY;
		rData.dRotationAngle = dRotationAngle;
		memcpy(rData.bDpstView, bDpstView, sizeof(rData.bDpstView));
	}
	void Set(T_DPST_VIEW_930 Data)
	{
		ConvertStrChar(Data.strDesc, strDesc, sizeof(strDesc));
		lfDescFont = Data.lfDescFont;
		DescColor = Data.DescColor;
		rectDesc = Data.rectDesc;
		dDescRatioX = Data.dDescRatioX;
		dDescRatioY = Data.dDescRatioY;
		dRotationAngle = Data.dRotationAngle;
		memcpy(bDpstView, Data.bDpstView, sizeof(bDpstView));
	}
};

struct T_DPST_VIEW_CH
{
	char      strDesc[60];
	LOGFONT   lfDescFont;
	COLORREF  DescColor;
	CRect     rectDesc;
	double    dDescRatioX;
	double    dDescRatioY;
	double    dRotationAngle;
	BOOL      bDpstView[EN_DPST_VIEW_END];
	void Get(T_DPST_VIEW &rData)
	{
		ConvertCharStr(strDesc, rData.strDesc, sizeof(strDesc));
		rData.lfDescFont = lfDescFont;
		rData.DescColor  = DescColor;
		rData.rectDesc   = rectDesc;
		rData.dDescRatioX = dDescRatioX;
		rData.dDescRatioY = dDescRatioY;
		rData.dRotationAngle = dRotationAngle;
		memcpy(rData.bDpstView, bDpstView, sizeof(rData.bDpstView));
	}
	void Set(T_DPST_VIEW Data)
	{
		ConvertStrChar(Data.strDesc, strDesc, sizeof(strDesc));
		lfDescFont  = Data.lfDescFont;
		DescColor   = Data.DescColor;
		rectDesc    = Data.rectDesc;
		dDescRatioX = Data.dDescRatioX;
		dDescRatioY = Data.dDescRatioY;
		dRotationAngle = Data.dRotationAngle;
		memcpy(bDpstView, Data.bDpstView, sizeof(bDpstView));
	}
	void Convert930(T_DPST_VIEW_CH_930 Data)
	{
		memcpy(strDesc, Data.strDesc, sizeof(strDesc));
		DescColor   = Data.DescColor;
		rectDesc    = Data.rectDesc;
		dDescRatioX = Data.dDescRatioX;
		dDescRatioY = Data.dDescRatioY;
		dRotationAngle = Data.dRotationAngle;
		memcpy(bDpstView, Data.bDpstView, sizeof(bDpstView));

		//////////////////////////////////////////////////////////////////////////
		//lfDescFont = Data.lfDescFont;
		lfDescFont.lfHeight         = Data.lfDescFont.lfHeight;
		lfDescFont.lfWidth          = Data.lfDescFont.lfWidth;
		lfDescFont.lfEscapement     = Data.lfDescFont.lfEscapement;
		lfDescFont.lfOrientation    = Data.lfDescFont.lfOrientation;
		lfDescFont.lfWeight         = Data.lfDescFont.lfWeight;
		lfDescFont.lfItalic         = Data.lfDescFont.lfItalic;
		lfDescFont.lfUnderline      = Data.lfDescFont.lfUnderline;
		lfDescFont.lfStrikeOut      = Data.lfDescFont.lfStrikeOut;
		lfDescFont.lfCharSet        = Data.lfDescFont.lfCharSet;
		lfDescFont.lfOutPrecision   = Data.lfDescFont.lfOutPrecision;
		lfDescFont.lfClipPrecision  = Data.lfDescFont.lfClipPrecision;
		lfDescFont.lfQuality        = Data.lfDescFont.lfQuality;
		lfDescFont.lfPitchAndFamily = Data.lfDescFont.lfPitchAndFamily;
		
		WCHAR strUnicode[LF_FACESIZE] = { 0, };
		CHAR strMultibyte[LF_FACESIZE] = { 0, };
		strcpy_s(strMultibyte, LF_FACESIZE, Data.lfDescFont.lfFaceName);
		int nLen = MultiByteToWideChar(CP_ACP, 0, strMultibyte, (int)strlen(strMultibyte), NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, strMultibyte, (int)strlen(strMultibyte), strUnicode, nLen);

		lstrcpy(lfDescFont.lfFaceName, strUnicode);
		//////////////////////////////////////////////////////////////////////////
	}
};
// Seismic Evaluation
struct T_DPST_SEIS
{                   
	BOOL bDpstSeis[EN_DPST_SEIS_END];
	void Initialize()
	{ 
		memset(bDpstSeis, 0, sizeof(bDpstSeis));
	}
	T_DPST_SEIS() { Initialize(); }
	T_DPST_SEIS(const T_DPST_SEIS &src) { *this = src; }
};
// Display Setting
struct T_DPST_D_810
{
	UINT  GrupK;      // Group Key
	BOOL  bDpstD[EN_DPST_D_END];
	T_DPST_NODE Node; // Node
	T_DPST_ELEM_835 Elem; // Element
	T_DPST_PROP_955 Prop; // Property
	T_DPST_BNDR_855 Bndr; // Boundary
	T_DPST_LOAD_865 Load; // Load
	T_DPST_DSGN_810 Dsgn; // design
	T_DPST_VIEW_930 View; // View
	void Initialize()
	{
		GrupK = 0;
		for(int i=0; i<EN_DPST_D_END; i++)
			bDpstD[i] = FALSE;
	}
	T_DPST_D_810() { Initialize(); }
	T_DPST_D_810(const T_DPST_D_810 &src) { *this = src; }
	T_DPST_D_810& operator=(const T_DPST_D_810 &src)
	{
		GrupK     = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node      = src.Node;
		Elem      = src.Elem;
		Prop      = src.Prop;
		Bndr      = src.Bndr;
		Load      = src.Load;
		Dsgn      = src.Dsgn;
		View      = src.View;
		return *this;
	}
};
struct T_DPST_D_821
{
	UINT  GrupK;      // Group Key
	BOOL  bDpstD[EN_DPST_D_END];
	T_DPST_NODE Node; // Node
	T_DPST_ELEM_835 Elem; // Element
	T_DPST_PROP_955 Prop; // Property
	T_DPST_BNDR_855 Bndr; // Boundary
	T_DPST_LOAD_865 Load; // Load
	T_DPST_DSGN_821 Dsgn; // design
	T_DPST_VIEW_930 View; // View
	void Initialize()
	{
		GrupK = 0;
		for(int i=0; i<EN_DPST_D_END; i++)
			bDpstD[i] = FALSE;
	}
	T_DPST_D_821() { Initialize(); }
	T_DPST_D_821(const T_DPST_D_821 &src) { *this = src; }
	T_DPST_D_821& operator=(const T_DPST_D_821 &src)
	{
		GrupK     = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node      = src.Node;
		Elem      = src.Elem;
		Prop      = src.Prop;
		Bndr      = src.Bndr;
		Load      = src.Load;
		Dsgn      = src.Dsgn;
		View      = src.View;
		return *this;
	}
};
struct T_DPST_D_835
{
	UINT  GrupK;      // Group Key
	BOOL  bDpstD[EN_DPST_D_END];
	T_DPST_NODE Node; // Node
	T_DPST_ELEM_835 Elem; // Element
	T_DPST_PROP_955 Prop; // Property
	T_DPST_BNDR_855 Bndr; // Boundary
	T_DPST_LOAD_865 Load; // Load
	T_DPST_DSGN_841 Dsgn; // design
	T_DPST_VIEW_930 View; // View
	void Initialize()
	{
		GrupK = 0;
		for(int i=0; i<EN_DPST_D_END; i++)
			bDpstD[i] = FALSE;
	}
	T_DPST_D_835() { Initialize(); }
	T_DPST_D_835(const T_DPST_D_835 &src) { *this = src; }
	T_DPST_D_835& operator=(const T_DPST_D_835 &src)
	{
		GrupK     = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node      = src.Node;
		Elem      = src.Elem;
		Prop      = src.Prop;
		Bndr      = src.Bndr;
		Load      = src.Load;
		Dsgn      = src.Dsgn;
		View      = src.View;
		return *this;
	}
};
struct T_DPST_D_841
{
	UINT  GrupK;      // Group Key
	BOOL  bDpstD[EN_DPST_D_END];
	T_DPST_NODE Node; // Node
	T_DPST_ELEM Elem; // Element
	T_DPST_PROP Prop; // Property
	T_DPST_BNDR_855 Bndr; // Boundary
	T_DPST_LOAD_865 Load; // Load
	T_DPST_DSGN_841 Dsgn; // design
	T_DPST_VIEW_930 View; // View
	void Initialize()
	{
		GrupK = 0;
		for(int i=0; i<EN_DPST_D_END; i++)
			bDpstD[i] = FALSE;
	}
	T_DPST_D_841() { Initialize(); }
	T_DPST_D_841(const T_DPST_D_841 &src) { *this = src; }
	T_DPST_D_841& operator=(const T_DPST_D_841 &src)
	{
		GrupK     = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node      = src.Node;
		Elem      = src.Elem;
		Prop      = src.Prop;
		Bndr      = src.Bndr;
		Load      = src.Load;
		Dsgn      = src.Dsgn;
		View      = src.View;
		return *this;
	}
};
struct T_DPST_D_855
{
	UINT  GrupK;      // Group Key
	BOOL  bDpstD[EN_DPST_D_END];
	T_DPST_NODE Node; // Node
	T_DPST_ELEM Elem; // Element
	T_DPST_PROP Prop; // Property
	T_DPST_BNDR_855 Bndr; // Boundary
	T_DPST_LOAD_865 Load; // Load
	T_DPST_DSGN_860 Dsgn; // design
	T_DPST_VIEW_930 View; // View
	void Initialize()
	{
		GrupK = 0;
		for(int i=0; i<EN_DPST_D_END; i++)
			bDpstD[i] = FALSE;
	}
	T_DPST_D_855() { Initialize(); }
	T_DPST_D_855(const T_DPST_D_855 &src) { *this = src; }
	T_DPST_D_855& operator=(const T_DPST_D_855 &src)
	{
		GrupK     = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node      = src.Node;
		Elem      = src.Elem;
		Prop      = src.Prop;
		Bndr      = src.Bndr;
		Load      = src.Load;
		Dsgn      = src.Dsgn;
		View      = src.View;
		return *this;
	}
};

struct T_DPST_D_860
{
	UINT  GrupK;      // Group Key
	BOOL  bDpstD[EN_DPST_D_END];
	T_DPST_NODE Node; // Node
	T_DPST_ELEM Elem; // Element
	T_DPST_PROP Prop; // Property
	T_DPST_BNDR Bndr; // Boundary
	T_DPST_LOAD_865 Load; // Load
	T_DPST_DSGN_860 Dsgn; // design
	T_DPST_VIEW_930 View; // View
	void Initialize()
	{
		GrupK = 0;
		for(int i=0; i<EN_DPST_D_END; i++)
			bDpstD[i] = FALSE;
	}
	T_DPST_D_860() { Initialize(); }
	T_DPST_D_860(const T_DPST_D_860 &src) { *this = src; }
	T_DPST_D_860& operator=(const T_DPST_D_860 &src)
	{
		GrupK     = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node      = src.Node;
		Elem      = src.Elem;
		Prop      = src.Prop;
		Bndr      = src.Bndr;
		Load      = src.Load;
		Dsgn      = src.Dsgn;
		View      = src.View;
		return *this;
	}
};

struct T_DPST_D_865
{
	UINT  GrupK;      // Group Key
	BOOL  bDpstD[EN_DPST_D_END];
	T_DPST_NODE Node; // Node
	T_DPST_ELEM Elem; // Element
	T_DPST_PROP Prop; // Property
	T_DPST_BNDR Bndr; // Boundary
	T_DPST_LOAD_865 Load; // Load
	T_DPST_DSGN_865 Dsgn; // design
	T_DPST_VIEW_930 View; // View
	void Initialize()
	{
		GrupK = 0;
		for(int i=0; i<EN_DPST_D_END; i++)
			bDpstD[i] = FALSE;
	}
	T_DPST_D_865() { Initialize(); }
	T_DPST_D_865(const T_DPST_D_865 &src) { *this = src; }
	T_DPST_D_865& operator=(const T_DPST_D_865 &src)
	{
		GrupK     = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node      = src.Node;
		Elem      = src.Elem;
		Prop      = src.Prop;
		Bndr      = src.Bndr;
		Load      = src.Load;
		Dsgn      = src.Dsgn;
		View      = src.View;
		return *this;
	}
};

struct T_DPST_D_870
{
	UINT  GrupK;      // Group Key
	BOOL  bDpstD[EN_DPST_D_END];
	T_DPST_NODE Node; // Node
	T_DPST_ELEM Elem; // Element
	T_DPST_PROP Prop; // Property
	T_DPST_BNDR Bndr; // Boundary
	T_DPST_LOAD_883 Load; // Load
	T_DPST_DSGN_883 Dsgn; // design
	T_DPST_VIEW_930 View; // View
	void Initialize()
	{
		GrupK = 0;
		for(int i=0; i<EN_DPST_D_END; i++)
			bDpstD[i] = FALSE;
	}
	T_DPST_D_870() { Initialize(); }
	T_DPST_D_870(const T_DPST_D_870 &src) { *this = src; }
	T_DPST_D_870& operator=(const T_DPST_D_870 &src)
	{
		GrupK     = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node      = src.Node;
		Elem      = src.Elem;
		Prop      = src.Prop;
		Bndr      = src.Bndr;
		Load      = src.Load;
		Dsgn      = src.Dsgn;
		View      = src.View;
		return *this;
	}
};

struct T_DPST_D_883
{
	UINT  GrupK;      // Group Key
	BOOL  bDpstD[EN_DPST_D_END];
	T_DPST_NODE Node; // Node
	T_DPST_ELEM Elem; // Element
	T_DPST_PROP Prop; // Property
	T_DPST_BNDR Bndr; // Boundary
	T_DPST_LOAD_883 Load; // Load
	T_DPST_DSGN_883 Dsgn; // design
	T_DPST_VIEW_930 View; // View
	T_DPST_SEIS Seis; // Seismic Evaluation
	void Initialize()
	{
		GrupK = 0;
		for(int i=0; i<EN_DPST_D_END; i++)
			bDpstD[i] = FALSE;
	}
	T_DPST_D_883() { Initialize(); }
	T_DPST_D_883(const T_DPST_D_883 &src) { *this = src; }
	T_DPST_D_883& operator=(const T_DPST_D_883 &src)
	{
		GrupK     = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node      = src.Node;
		Elem      = src.Elem;
		Prop      = src.Prop;
		Bndr      = src.Bndr;
		Load      = src.Load;
		Dsgn      = src.Dsgn;
		View      = src.View;
		Seis      = src.Seis;
		return *this;
	}
};

struct T_DPST_D_885
{
	UINT  GrupK;      // Group Key
	BOOL  bDpstD[EN_DPST_D_END];
	T_DPST_NODE Node; // Node
	T_DPST_ELEM Elem; // Element
	T_DPST_PROP Prop; // Property
	T_DPST_BNDR Bndr; // Boundary
	T_DPST_LOAD Load; // Load
	T_DPST_DSGN_885 Dsgn; // design
	T_DPST_VIEW_930 View; // View
	T_DPST_SEIS Seis; // Seismic Evaluation
	void Initialize()
	{
		GrupK = 0;
		for(int i=0; i<EN_DPST_D_END; i++)
			bDpstD[i] = FALSE;
	}
	T_DPST_D_885() { Initialize(); }
	T_DPST_D_885(const T_DPST_D_885 &src) { *this = src; }
	T_DPST_D_885& operator=(const T_DPST_D_885 &src)
	{
		GrupK     = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node      = src.Node;
		Elem      = src.Elem;
		Prop      = src.Prop;
		Bndr      = src.Bndr;
		Load      = src.Load;
		Dsgn      = src.Dsgn;
		View      = src.View;
		Seis      = src.Seis;
		return *this;
	}
};

struct T_DPST_D_891
{
	UINT  GrupK;      // Group Key
	BOOL  bDpstD[EN_DPST_D_END];
	T_DPST_NODE Node; // Node
	T_DPST_ELEM Elem; // Element
	T_DPST_PROP Prop; // Property
	T_DPST_BNDR Bndr; // Boundary
	T_DPST_LOAD Load; // Load
	T_DPST_DSGN_891 Dsgn; // design
	T_DPST_VIEW_930 View; // View
	T_DPST_SEIS Seis; // Seismic Evaluation
	void Initialize()
	{
		GrupK = 0;
		for(int i=0; i<EN_DPST_D_END; i++)
			bDpstD[i] = FALSE;
	}
	T_DPST_D_891() { Initialize(); }
	T_DPST_D_891(const T_DPST_D_891 &src) { *this = src; }
	T_DPST_D_891& operator=(const T_DPST_D_891 &src)
	{
		GrupK     = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node      = src.Node;
		Elem      = src.Elem;
		Prop      = src.Prop;
		Bndr      = src.Bndr;
		Load      = src.Load;
		Dsgn      = src.Dsgn;
		View      = src.View;
		Seis      = src.Seis;
		return *this;
	}
};

struct T_DPST_D_895
{
	UINT  GrupK;      // Group Key
	BOOL  bDpstD[EN_DPST_D_END];
	T_DPST_NODE Node; // Node
	T_DPST_ELEM Elem; // Element
	T_DPST_PROP Prop; // Property
	T_DPST_BNDR Bndr; // Boundary
	T_DPST_LOAD Load; // Load
	T_DPST_DSGN_895 Dsgn; // design
	T_DPST_VIEW_930 View; // View
	T_DPST_SEIS Seis; // Seismic Evaluation
	void Initialize()
	{
		GrupK = 0;
		for (int i = 0; i < EN_DPST_D_END; i++)
			bDpstD[i] = FALSE;
	}
	T_DPST_D_895() { Initialize(); }
	T_DPST_D_895(const T_DPST_D_895 &src) { *this = src; }
	T_DPST_D_895& operator=(const T_DPST_D_895 &src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn = src.Dsgn;
		View = src.View;
		Seis = src.Seis;
		return *this;
	}
};

struct T_DPST_D_925
{
	UINT  GrupK;      // Group Key
	BOOL  bDpstD[EN_DPST_D_END];
	T_DPST_NODE Node; // Node
	T_DPST_ELEM Elem; // Element
	T_DPST_PROP Prop; // Property
	T_DPST_BNDR Bndr; // Boundary
	T_DPST_LOAD Load; // Load
	T_DPST_DSGN_925 Dsgn; // design
	T_DPST_VIEW_930 View; // View
	T_DPST_SEIS Seis; // Seismic Evaluation
	void Initialize()
	{
		GrupK = 0;
		for (int i = 0; i < EN_DPST_D_END; i++)
			bDpstD[i] = FALSE;
	}
	T_DPST_D_925() { Initialize(); }
	T_DPST_D_925(const T_DPST_D_925 &src) { *this = src; }
	T_DPST_D_925& operator=(const T_DPST_D_925 &src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn = src.Dsgn;
		View = src.View;
		Seis = src.Seis;
		return *this;
	}
};

struct T_DPST_D_930
{
	UINT  GrupK;      // Group Key
	BOOL  bDpstD[EN_DPST_D_END];
	T_DPST_NODE Node; // Node
	T_DPST_ELEM Elem; // Element
	T_DPST_PROP Prop; // Property
	T_DPST_BNDR Bndr; // Boundary
	T_DPST_LOAD Load; // Load
	T_DPST_DSGN_940 Dsgn; // design
	T_DPST_VIEW_930 View; // View
	T_DPST_SEIS Seis; // Seismic Evaluation
	void Initialize()
	{
		GrupK = 0;
		for (int i = 0; i < EN_DPST_D_END; i++)
			bDpstD[i] = FALSE;
	}
	T_DPST_D_930() { Initialize(); }
	T_DPST_D_930(const T_DPST_D_930 &src) { *this = src; }
	T_DPST_D_930& operator=(const T_DPST_D_930 &src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn = src.Dsgn;
		View = src.View;
		Seis = src.Seis;
		return *this;
	}
};

struct T_DPST_D_940
{
	UINT  GrupK;      // Group Key
	BOOL  bDpstD[EN_DPST_D_END];
	T_DPST_NODE Node; // Node
	T_DPST_ELEM Elem; // Element
	T_DPST_PROP Prop; // Property
	T_DPST_BNDR Bndr; // Boundary
	T_DPST_LOAD Load; // Load
	T_DPST_DSGN_940 Dsgn; // design
	T_DPST_VIEW View; // View
	T_DPST_SEIS Seis; // Seismic Evaluation
	void Initialize()
	{
		GrupK = 0;
		for (int i = 0; i < EN_DPST_D_END; i++)
			bDpstD[i] = FALSE;
	}
	T_DPST_D_940() { Initialize(); }
	T_DPST_D_940(const T_DPST_D_940 &src) { *this = src; }
	T_DPST_D_940& operator=(const T_DPST_D_940 &src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn = src.Dsgn;
		View = src.View;
		Seis = src.Seis;
		return *this;
	}
};

struct T_DPST_D_950
{
	UINT  GrupK;      // Group Key
	BOOL  bDpstD[EN_DPST_D_END];
	T_DPST_NODE Node; // Node
	T_DPST_ELEM Elem; // Element
	T_DPST_PROP Prop; // Property
	T_DPST_BNDR Bndr; // Boundary
	T_DPST_LOAD Load; // Load
	T_DPST_DSGN_950 Dsgn; // design
	T_DPST_VIEW View; // View
	T_DPST_SEIS Seis; // Seismic Evaluation
	void Initialize()
	{
		GrupK = 0;
		for (int i = 0; i < EN_DPST_D_END; i++)
			bDpstD[i] = FALSE;
	}
	T_DPST_D_950() { Initialize(); }
	T_DPST_D_950(const T_DPST_D_950& src) { *this = src; }
	T_DPST_D_950& operator=(const T_DPST_D_950& src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn = src.Dsgn;
		View = src.View;
		Seis = src.Seis;
		return *this;
	}
};

struct T_DPST_D
{
	UINT  GrupK;      // Group Key
	BOOL  bDpstD[EN_DPST_D_END];
	T_DPST_NODE Node; // Node
	T_DPST_ELEM Elem; // Element
	T_DPST_PROP Prop; // Property
	T_DPST_BNDR Bndr; // Boundary
	T_DPST_LOAD Load; // Load
	T_DPST_DSGN Dsgn; // design
	T_DPST_VIEW View; // View
	T_DPST_SEIS Seis; // Seismic Evaluation
	void Initialize()
	{
		GrupK = 0;
		for(int i=0; i<EN_DPST_D_END; i++)
			bDpstD[i] = FALSE;
	}
	T_DPST_D() { Initialize(); }
	T_DPST_D(const T_DPST_D &src) { *this = src; }
	T_DPST_D& operator=(const T_DPST_D &src)
	{
		GrupK     = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node      = src.Node;
		Elem      = src.Elem;
		Prop      = src.Prop;
		Bndr      = src.Bndr;
		Load      = src.Load;
		Dsgn      = src.Dsgn;
		View      = src.View;
		Seis      = src.Seis;
		return *this;
	}
	void Convert950(T_DPST_D_950 src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = (src.Load);
		Dsgn.Convert950(src.Dsgn);
		View = src.View;
		Seis = src.Seis;
	}
	void Convert940(T_DPST_D_940 src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = (src.Load);
		Dsgn.Convert940(src.Dsgn);
		View = src.View;
		Seis = src.Seis;
	}
	void Convert930(T_DPST_D_930 src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = (src.Load);
		Dsgn.Convert940(src.Dsgn);
		View.Convert930(src.View);
		Seis = src.Seis;
	}
	void Convert925(T_DPST_D_925 src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = (src.Load);
		Dsgn.Convert925(src.Dsgn);
		View.Convert930(src.View);
		Seis = src.Seis;
	}
	void Convert895(T_DPST_D_895 src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = (src.Load);
		Dsgn.Convert895(src.Dsgn);
		View.Convert930(src.View);
		Seis = src.Seis;
	}
	void Convert891(T_DPST_D_891 src)
	{
		GrupK     = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node      = src.Node;
		Elem = src.Elem;
		Prop      = src.Prop;
		Bndr      = src.Bndr;
		Load      =(src.Load);
		Dsgn.Convert891(src.Dsgn);
		View.Convert930(src.View);
		Seis      = src.Seis;
	}
	void Convert885(T_DPST_D_885& src)
	{
		GrupK     = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node      = src.Node;
		Elem = src.Elem;
		Prop      = src.Prop;
		Bndr      = src.Bndr;
		Load      =(src.Load);
		Dsgn.Convert885(src.Dsgn);
		View.Convert930(src.View);
		Seis      = src.Seis;
	}
	void Convert883(T_DPST_D_883& src)
	{
		GrupK     = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node      = src.Node;
		Elem = src.Elem;
		Prop      = src.Prop;
		Bndr      = src.Bndr;
		Load.Convert883(src.Load);
		Dsgn.Convert883(src.Dsgn);
		View.Convert930(src.View);
		Seis      = src.Seis;
	}
	void Convert870(T_DPST_D_870& src)
	{
		GrupK     = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node      = src.Node;
		Elem = src.Elem;
		Prop      = src.Prop;
		Bndr      = src.Bndr;
		Load.Convert883(src.Load);
		Dsgn.Convert883(src.Dsgn);
		View.Convert930(src.View);
	}
	void Convert865(T_DPST_D_865& src)
	{
		GrupK     = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node      = src.Node;
		Elem = src.Elem;
		Prop      = src.Prop;
		Bndr      = src.Bndr;
		Load.Convert865(src.Load);
		Dsgn.Convert865(src.Dsgn);
		View.Convert930(src.View);
	}
	void Convert860(T_DPST_D_860& src)
	{
		GrupK     = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node      = src.Node;
		Elem = src.Elem;
		Prop      = src.Prop;
		Bndr      = src.Bndr;
		Load.Convert865(src.Load);
		Dsgn.Convert860(src.Dsgn);
		View.Convert930(src.View);
	}
	void Convert855(T_DPST_D_855& src)
	{
	  GrupK     = src.GrupK;
	  memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
	  Node      = src.Node;
	  Elem = src.Elem;
	  Prop      = src.Prop;
	  Bndr.Convert855(src.Bndr);
	  Load.Convert865(src.Load);
	  Dsgn.Convert860(src.Dsgn);
	  View.Convert930(src.View);
	}
	void Convert841(T_DPST_D_841& src)
	{
		GrupK     = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node      = src.Node;
		Elem = src.Elem;
		Prop      = src.Prop;
		Bndr.Convert855(src.Bndr);
		Load.Convert865(src.Load);
		Dsgn.Convert841(src.Dsgn);
		View.Convert930(src.View);
	}
	void Convert835(T_DPST_D_835& src)
	{
		GrupK     = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node      = src.Node;
		Elem.Convert835(src.Elem);
		Prop.Convert955(src.Prop);
		Bndr.Convert855(src.Bndr);
		Load.Convert865(src.Load);
		Dsgn.Convert841(src.Dsgn);
		View.Convert930(src.View);
	}
	void Convert821(T_DPST_D_821& src)
	{
		GrupK     = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node      = src.Node;
		Elem.Convert835(src.Elem);
		Prop.Convert955(src.Prop);
		Bndr.Convert855(src.Bndr);
		Load.Convert865(src.Load);
		Dsgn.Convert821(src.Dsgn);
		View.Convert930(src.View);
	}
	void Convert810(T_DPST_D_810& src)
	{
		GrupK     = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node      = src.Node;
		Elem.Convert835(src.Elem);
		Prop.Convert955(src.Prop);
		Bndr.Convert855(src.Bndr);
		Load.Convert865(src.Load);
		Dsgn.Convert810(src.Dsgn);
		View.Convert930(src.View);
	}
};
struct T_DPST_D_CH_810
{
	UINT GrupK;  
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE    Node;
	T_DPST_ELEM_835    Elem;
	T_DPST_PROP_955    Prop;
	T_DPST_BNDR_855    Bndr;
	T_DPST_LOAD_865    Load;
	T_DPST_DSGN_810    Dsgn;
	T_DPST_VIEW_CH_930 View;

	void Get(T_DPST_D_810 &rData)
	{
		rData.GrupK = GrupK ;
		memcpy(rData.bDpstD, bDpstD, sizeof(bDpstD));
		rData.Node = Node ;
		rData.Elem = Elem ;
		rData.Prop = Prop ;
		rData.Bndr = Bndr ;
		rData.Load = Load ;
		rData.Dsgn = Dsgn ;
		View.Get(rData.View);
	}
	void Set(T_DPST_D_810 Data)
	{
		GrupK = Data.GrupK ;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node ;
		Elem = Data.Elem ;
		Prop = Data.Prop ;
		Bndr = Data.Bndr ;
		Load = Data.Load ;
		Dsgn = Data.Dsgn ;
		View.Set(Data.View) ;
	}

	T_DPST_D_CH_810() {}
	T_DPST_D_CH_810(const T_DPST_D_CH_810 &src) { *this = src; }
	T_DPST_D_CH_810& operator=(const T_DPST_D_CH_810 &src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn = src.Dsgn;
		View = src.View;
		return *this;
	}
};
struct T_DPST_D_CH_821
{
	UINT GrupK;  
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE    Node;
	T_DPST_ELEM_835    Elem;
	T_DPST_PROP_955    Prop;
	T_DPST_BNDR_855    Bndr;
	T_DPST_LOAD_865    Load;
	T_DPST_DSGN_821    Dsgn;
	T_DPST_VIEW_CH_930 View;
	
	void Get(T_DPST_D_821 &rData)
	{
		rData.GrupK = GrupK ;
		memcpy(rData.bDpstD, bDpstD, sizeof(bDpstD));
		rData.Node = Node ;
		rData.Elem = Elem ;
		rData.Prop = Prop ;
		rData.Bndr = Bndr ;
		rData.Load = Load ;
		rData.Dsgn = Dsgn ;
		View.Get(rData.View);
	}
	void Set(T_DPST_D_821 Data)
	{
		GrupK = Data.GrupK ;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node ;
		Elem = Data.Elem ;
		Prop = Data.Prop ;
		Bndr = Data.Bndr ;
		Load = Data.Load ;
		Dsgn = Data.Dsgn ;
		View.Set(Data.View) ;
	}
	
	T_DPST_D_CH_821() {}
	T_DPST_D_CH_821(const T_DPST_D_CH_821 &src) { *this = src; }
	T_DPST_D_CH_821& operator=(const T_DPST_D_CH_821 &src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn = src.Dsgn;
		View = src.View;
		return *this;
	}
};
struct T_DPST_D_CH_835
{
	UINT GrupK;  
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE    Node;
	T_DPST_ELEM_835    Elem;
	T_DPST_PROP_955    Prop;
	T_DPST_BNDR_855    Bndr;
	T_DPST_LOAD_865    Load;
	T_DPST_DSGN_841    Dsgn;
	T_DPST_VIEW_CH_930 View;

	void Get(T_DPST_D_835 &rData)
	{
		rData.GrupK = GrupK ;
		memcpy(rData.bDpstD, bDpstD, sizeof(bDpstD));
		rData.Node = Node ;
		rData.Elem = Elem ;
		rData.Prop = Prop ;
		rData.Bndr = Bndr ;
		rData.Load = Load ;
		rData.Dsgn = Dsgn ;
		View.Get(rData.View);
	}
	void Set(T_DPST_D_835 Data)
	{
		GrupK = Data.GrupK ;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node ;
		Elem = Data.Elem ;
		Prop = Data.Prop ;
		Bndr = Data.Bndr ;
		Load = Data.Load ;
		Dsgn = Data.Dsgn ;
		View.Set(Data.View) ;
	}

	T_DPST_D_CH_835() {}
	T_DPST_D_CH_835(const T_DPST_D_CH_835 &src) { *this = src; }
	T_DPST_D_CH_835& operator=(const T_DPST_D_CH_835 &src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn = src.Dsgn;
		View = src.View;
		return *this;
	}
};
struct T_DPST_D_CH_841
{
	UINT GrupK;  
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE    Node;
	T_DPST_ELEM    Elem;
	T_DPST_PROP    Prop;
	T_DPST_BNDR_855    Bndr;
	T_DPST_LOAD_865    Load;
	T_DPST_DSGN_841    Dsgn;
	T_DPST_VIEW_CH_930 View;

	void Get(T_DPST_D_841 &rData)
	{
		rData.GrupK = GrupK ;
		memcpy(rData.bDpstD, bDpstD, sizeof(bDpstD));
		rData.Node = Node ;
		rData.Elem = Elem ;
		rData.Prop = Prop ;
		rData.Bndr = Bndr ;
		rData.Load = Load ;
		rData.Dsgn = Dsgn ;
		View.Get(rData.View);
	}
	void Set(T_DPST_D_841 Data)
	{
		GrupK = Data.GrupK ;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node ;
		Elem = Data.Elem ;
		Prop = Data.Prop ;
		Bndr = Data.Bndr ;
		Load = Data.Load ;
		Dsgn = Data.Dsgn ;
		View.Set(Data.View) ;
	}

	T_DPST_D_CH_841() {}
	T_DPST_D_CH_841(const T_DPST_D_CH_841 &src) { *this = src; }
	T_DPST_D_CH_841& operator=(const T_DPST_D_CH_841 &src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn = src.Dsgn;
		View = src.View;
		return *this;
	}
};
struct T_DPST_D_CH_855
{
	UINT GrupK;  
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE    Node;
	T_DPST_ELEM    Elem;
	T_DPST_PROP    Prop;
	T_DPST_BNDR_855    Bndr;
	T_DPST_LOAD_865    Load;
	T_DPST_DSGN_860    Dsgn;
	T_DPST_VIEW_CH_930 View;

	void Get(T_DPST_D_855 &rData)
	{
		rData.GrupK = GrupK ;
		memcpy(rData.bDpstD, bDpstD, sizeof(bDpstD));
		rData.Node = Node ;
		rData.Elem = Elem ;
		rData.Prop = Prop ;
		rData.Bndr = Bndr ;
		rData.Load = Load ;
		rData.Dsgn = Dsgn ;
		View.Get(rData.View);
	}
	void Set(T_DPST_D_855 Data)
	{
		GrupK = Data.GrupK ;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node ;
		Elem = Data.Elem ;
		Prop = Data.Prop ;
		Bndr = Data.Bndr ;
		Load = Data.Load ;
		Dsgn = Data.Dsgn ;
		View.Set(Data.View) ;
	}

	T_DPST_D_CH_855() {}
	T_DPST_D_CH_855(const T_DPST_D_CH_855 &src) { *this = src; }
	T_DPST_D_CH_855& operator=(const T_DPST_D_CH_855 &src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn = src.Dsgn;
		View = src.View;
		return *this;
	}
};

struct T_DPST_D_CH_860
{
	UINT GrupK;  
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE    Node;
	T_DPST_ELEM    Elem;
	T_DPST_PROP    Prop;
	T_DPST_BNDR    Bndr;
	T_DPST_LOAD_865  Load;
	T_DPST_DSGN_860  Dsgn;
	T_DPST_VIEW_CH_930 View;

	void Get(T_DPST_D_860 &rData)
	{
		rData.GrupK = GrupK ;
		memcpy(rData.bDpstD, bDpstD, sizeof(bDpstD));
		rData.Node = Node ;
		rData.Elem = Elem ;
		rData.Prop = Prop ;
		rData.Bndr = Bndr ;
		rData.Load = Load ;
		rData.Dsgn = Dsgn ;
		View.Get(rData.View);
	}
	void Set(T_DPST_D_860 Data)
	{
		GrupK = Data.GrupK ;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node ;
		Elem = Data.Elem ;
		Prop = Data.Prop ;
		Bndr = Data.Bndr ;
		Load = Data.Load ;
		Dsgn = Data.Dsgn ;
		View.Set(Data.View) ;
	}

	T_DPST_D_CH_860() {}
	T_DPST_D_CH_860(const T_DPST_D_CH_860 &src) { *this = src; }
	T_DPST_D_CH_860& operator=(const T_DPST_D_CH_860 &src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn = src.Dsgn;
		View = src.View;
		return *this;
	}
};

struct T_DPST_D_CH_865
{
	UINT GrupK;  
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE    Node;
	T_DPST_ELEM    Elem;
	T_DPST_PROP    Prop;
	T_DPST_BNDR    Bndr;
	T_DPST_LOAD_865  Load;
	T_DPST_DSGN_865  Dsgn;
	T_DPST_VIEW_CH_930 View;

	void Get(T_DPST_D_865 &rData)
	{
		rData.GrupK = GrupK ;
		memcpy(rData.bDpstD, bDpstD, sizeof(bDpstD));
		rData.Node = Node ;
		rData.Elem = Elem ;
		rData.Prop = Prop ;
		rData.Bndr = Bndr ;
		rData.Load = Load ;
		rData.Dsgn = Dsgn ;
		View.Get(rData.View);
	}
	void Set(T_DPST_D_865 Data)
	{
		GrupK = Data.GrupK ;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node ;
		Elem = Data.Elem ;
		Prop = Data.Prop ;
		Bndr = Data.Bndr ;
		Load = Data.Load ;
		Dsgn = Data.Dsgn ;
		View.Set(Data.View) ;
	}

	T_DPST_D_CH_865() {}
	T_DPST_D_CH_865(const T_DPST_D_CH_865 &src) { *this = src; }
	T_DPST_D_CH_865& operator=(const T_DPST_D_CH_865 &src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn = src.Dsgn;
		View = src.View;
		return *this;
	}
};
struct T_DPST_D_CH_870
{
	UINT GrupK;  
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE    Node;
	T_DPST_ELEM    Elem;
	T_DPST_PROP    Prop;
	T_DPST_BNDR    Bndr;
	T_DPST_LOAD_883    Load;
	T_DPST_DSGN_883    Dsgn;
	T_DPST_VIEW_CH_930 View;

	void Get(T_DPST_D_870 &rData)
	{
		rData.GrupK = GrupK ;
		memcpy(rData.bDpstD, bDpstD, sizeof(bDpstD));
		rData.Node = Node ;
		rData.Elem = Elem ;
		rData.Prop = Prop ;
		rData.Bndr = Bndr ;
		rData.Load = Load ;
		rData.Dsgn = Dsgn ;
		View.Get(rData.View);
	}
	void Set(T_DPST_D_870 Data)
	{
		GrupK = Data.GrupK ;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node ;
		Elem = Data.Elem ;
		Prop = Data.Prop ;
		Bndr = Data.Bndr ;
		Load = Data.Load ;
		Dsgn = Data.Dsgn ;
		View.Set(Data.View) ;
	}

	T_DPST_D_CH_870() {}
	T_DPST_D_CH_870(const T_DPST_D_CH_870 &src) { *this = src; }
	T_DPST_D_CH_870& operator=(const T_DPST_D_CH_870 &src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn = src.Dsgn;
		View = src.View;
		return *this;
	}
};
struct T_DPST_D_CH_883
{
	UINT GrupK;  
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE    Node;
	T_DPST_ELEM    Elem;
	T_DPST_PROP    Prop;
	T_DPST_BNDR    Bndr;
	T_DPST_LOAD_883    Load;
	T_DPST_DSGN_883    Dsgn;
	T_DPST_VIEW_CH_930 View;
	T_DPST_SEIS    Seis;

	void Get(T_DPST_D_883 &rData)
	{
		rData.GrupK = GrupK ;
		memcpy(rData.bDpstD, bDpstD, sizeof(bDpstD));
		rData.Node = Node ;
		rData.Elem = Elem ;
		rData.Prop = Prop ;
		rData.Bndr = Bndr ;
		rData.Load = Load ;
		rData.Dsgn = Dsgn ;
		View.Get(rData.View);
		rData.Seis = Seis ;
	}
	void Set(T_DPST_D_883 Data)
	{
		GrupK = Data.GrupK ;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node ;
		Elem = Data.Elem ;
		Prop = Data.Prop ;
		Bndr = Data.Bndr ;
		Load = Data.Load ;
		Dsgn = Data.Dsgn ;
		View.Set(Data.View) ;
		Seis = Data.Seis ;
	}

	T_DPST_D_CH_883() {}
	T_DPST_D_CH_883(const T_DPST_D_CH_883 &src) { *this = src; }
	T_DPST_D_CH_883& operator=(const T_DPST_D_CH_883 &src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn = src.Dsgn;
		View = src.View;
		Seis = src.Seis;
		return *this;
	}
};

struct T_DPST_D_CH_885
{
	UINT GrupK;  
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE    Node;
	T_DPST_ELEM    Elem;
	T_DPST_PROP    Prop;
	T_DPST_BNDR    Bndr;
	T_DPST_LOAD    Load;
	T_DPST_DSGN_885    Dsgn;
	T_DPST_VIEW_CH_930 View;
	T_DPST_SEIS    Seis;
	void Get(T_DPST_D_885 &rData)
	{
		rData.GrupK = GrupK;
		memcpy(rData.bDpstD, bDpstD, sizeof(rData.bDpstD));
		rData.Node = Node;
		rData.Elem = Elem;
		rData.Prop = Prop;
		rData.Bndr = Bndr;
		rData.Load = Load;
		rData.Dsgn = Dsgn;
		rData.Seis = Seis;
		View.Get(rData.View);
	}
	void Set(T_DPST_D_885 Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem = Data.Elem;
		Prop = Data.Prop;
		Bndr = Data.Bndr;
		Load = Data.Load;
		Dsgn = Data.Dsgn;
		Seis = Data.Seis;
		View.Set(Data.View);
	}
	T_DPST_D_CH_885() {}
	T_DPST_D_CH_885(const T_DPST_D_CH_885 &src) { *this = src; }
	T_DPST_D_CH_885& operator=(const T_DPST_D_CH_885 &src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn = src.Dsgn;
		Seis = src.Seis;
		View = src.View;
		return *this;
	}
};

struct T_DPST_D_CH_891
{
	UINT GrupK;  
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE    Node;
	T_DPST_ELEM    Elem;
	T_DPST_PROP    Prop;
	T_DPST_BNDR    Bndr;
	T_DPST_LOAD    Load;
	T_DPST_DSGN_891    Dsgn;
	T_DPST_VIEW_CH_930 View;
	T_DPST_SEIS    Seis;
	void Get(T_DPST_D_891 &rData)
	{
		rData.GrupK = GrupK;
		memcpy(rData.bDpstD, bDpstD, sizeof(rData.bDpstD));
		rData.Node = Node;
		rData.Elem = Elem;
		rData.Prop = Prop;
		rData.Bndr = Bndr;
		rData.Load = Load;
		rData.Dsgn = Dsgn;
		rData.Seis = Seis;
		View.Get(rData.View);
	}
	void Set(T_DPST_D_891 Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem = Data.Elem;
		Prop = Data.Prop;
		Bndr = Data.Bndr;
		Load = Data.Load;
		Dsgn = Data.Dsgn;
		Seis = Data.Seis;
		View.Set(Data.View);
	}
	T_DPST_D_CH_891() {}
	T_DPST_D_CH_891(const T_DPST_D_CH_891 &src) { *this = src; }
	T_DPST_D_CH_891& operator=(const T_DPST_D_CH_891 &src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn = src.Dsgn;
		Seis = src.Seis;
		View = src.View;
		return *this;
	}
};

struct T_DPST_D_CH_895
{
	UINT GrupK;
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE			Node;
	T_DPST_ELEM			Elem;
	T_DPST_PROP			Prop;
	T_DPST_BNDR			Bndr;
	T_DPST_LOAD			Load;
	T_DPST_DSGN_895		Dsgn;
	T_DPST_VIEW_CH_930 	View;
	T_DPST_SEIS			Seis;
	void Get(T_DPST_D_895 &rData)
	{
		rData.GrupK = GrupK;
		memcpy(rData.bDpstD, bDpstD, sizeof(rData.bDpstD));
		rData.Node = Node;
		rData.Elem = Elem;
		rData.Prop = Prop;
		rData.Bndr = Bndr;
		rData.Load = Load;
		rData.Dsgn = Dsgn;
		rData.Seis = Seis;
		View.Get(rData.View);
	}
	void Set(T_DPST_D_895 Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem = Data.Elem;
		Prop = Data.Prop;
		Bndr = Data.Bndr;
		Load = Data.Load;
		Dsgn = Data.Dsgn;
		Seis = Data.Seis;
		View.Set(Data.View);
	}
	T_DPST_D_CH_895() {}
	T_DPST_D_CH_895(const T_DPST_D_CH_895 &src) { *this = src; }
	T_DPST_D_CH_895& operator=(const T_DPST_D_CH_895 &src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn = src.Dsgn;
		Seis = src.Seis;
		View = src.View;
		return *this;
	}
};
struct T_DPST_D_CH_925
{
	UINT GrupK;
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE			Node;
	T_DPST_ELEM			Elem;
	T_DPST_PROP			Prop;
	T_DPST_BNDR			Bndr;
	T_DPST_LOAD			Load;
	T_DPST_DSGN_925		Dsgn;
	T_DPST_VIEW_CH_930 	View;
	T_DPST_SEIS			Seis;
	void Get(T_DPST_D_925 &rData)
	{
		rData.GrupK = GrupK;
		memcpy(rData.bDpstD, bDpstD, sizeof(rData.bDpstD));
		rData.Node = Node;
		rData.Elem = Elem;
		rData.Prop = Prop;
		rData.Bndr = Bndr;
		rData.Load = Load;
		rData.Dsgn = Dsgn;
		rData.Seis = Seis;
		View.Get(rData.View);
	}
	void Set(T_DPST_D_925 Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem = Data.Elem;
		Prop = Data.Prop;
		Bndr = Data.Bndr;
		Load = Data.Load;
		Dsgn = Data.Dsgn;
		Seis = Data.Seis;
		View.Set(Data.View);
	}
	T_DPST_D_CH_925() {}
	T_DPST_D_CH_925(const T_DPST_D_CH_925 &src) { *this = src; }
	T_DPST_D_CH_925& operator=(const T_DPST_D_CH_925 &src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn = src.Dsgn;
		Seis = src.Seis;
		View = src.View;
		return *this;
	}
};
struct T_DPST_D_CH_930
{
	UINT GrupK;
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE			Node;
	T_DPST_ELEM			Elem;
	T_DPST_PROP			Prop;
	T_DPST_BNDR			Bndr;
	T_DPST_LOAD			Load;
	T_DPST_DSGN_940  	Dsgn;
	T_DPST_VIEW_CH_930 	View;
	T_DPST_SEIS			Seis;
	void Get(T_DPST_D_930 &rData)
	{
		rData.GrupK = GrupK;
		memcpy(rData.bDpstD, bDpstD, sizeof(rData.bDpstD));
		rData.Node = Node;
		rData.Elem = Elem;
		rData.Prop = Prop;
		rData.Bndr = Bndr;
		rData.Load = Load;
		rData.Dsgn = Dsgn;
		rData.Seis = Seis;
		View.Get(rData.View);
	}
	void Set(T_DPST_D_930 Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem = Data.Elem;
		Prop = Data.Prop;
		Bndr = Data.Bndr;
		Load = Data.Load;
		Dsgn = Data.Dsgn;
		Seis = Data.Seis;
		View.Set(Data.View);
	}
	T_DPST_D_CH_930() {}
	T_DPST_D_CH_930(const T_DPST_D_CH_930 &src) { *this = src; }
	T_DPST_D_CH_930& operator=(const T_DPST_D_CH_930 &src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn = src.Dsgn;
		Seis = src.Seis;
		View = src.View;
		return *this;
	}
};
struct T_DPST_D_CH_940
{
	UINT GrupK;
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE    Node;
	T_DPST_ELEM    Elem;
	T_DPST_PROP    Prop;
	T_DPST_BNDR    Bndr;
	T_DPST_LOAD    Load;
	T_DPST_DSGN_940  Dsgn;
	T_DPST_VIEW_CH View;
	T_DPST_SEIS    Seis;
	void Get(T_DPST_D_940 &rData)
	{
		rData.GrupK = GrupK;
		memcpy(rData.bDpstD, bDpstD, sizeof(rData.bDpstD));
		rData.Node = Node;
		rData.Elem = Elem;
		rData.Prop = Prop;
		rData.Bndr = Bndr;
		rData.Load = Load;
		rData.Dsgn = Dsgn;
		rData.Seis = Seis;
		View.Get(rData.View);
	}
	void Set(T_DPST_D_940 Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem = Data.Elem;
		Prop = Data.Prop;
		Bndr = Data.Bndr;
		Load = Data.Load;
		Dsgn = Data.Dsgn;
		Seis = Data.Seis;
		View.Set(Data.View);
	}
	T_DPST_D_CH_940() {}
	T_DPST_D_CH_940(const T_DPST_D_CH_940 &src) { *this = src; }
	T_DPST_D_CH_940& operator=(const T_DPST_D_CH_940 &src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn = src.Dsgn;
		Seis = src.Seis;
		View = src.View;
		return *this;
	}
};

struct T_DPST_D_CH_950
{
	UINT GrupK;
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE    Node;
	T_DPST_ELEM    Elem;
	T_DPST_PROP    Prop;
	T_DPST_BNDR    Bndr;
	T_DPST_LOAD    Load;
	T_DPST_DSGN_950  Dsgn;
	T_DPST_VIEW_CH View;
	T_DPST_SEIS    Seis;
	void Get(T_DPST_D_950& rData)
	{
		rData.GrupK = GrupK;
		memcpy(rData.bDpstD, bDpstD, sizeof(rData.bDpstD));
		rData.Node = Node;
		rData.Elem = Elem;
		rData.Prop = Prop;
		rData.Bndr = Bndr;
		rData.Load = Load;
		rData.Dsgn = Dsgn;
		rData.Seis = Seis;
		View.Get(rData.View);
	}
	void Set(T_DPST_D_950 Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem = Data.Elem;
		Prop = Data.Prop;
		Bndr = Data.Bndr;
		Load = Data.Load;
		Dsgn = Data.Dsgn;
		Seis = Data.Seis;
		View.Set(Data.View);
	}
	T_DPST_D_CH_950() {}
	T_DPST_D_CH_950(const T_DPST_D_CH_950& src) { *this = src; }
	T_DPST_D_CH_950& operator=(const T_DPST_D_CH_950& src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn = src.Dsgn;
		Seis = src.Seis;
		View = src.View;
		return *this;
	}
};

struct T_DPST_D_CH
{
	UINT GrupK;  
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE    Node;
	T_DPST_ELEM    Elem;
	T_DPST_PROP    Prop;
	T_DPST_BNDR    Bndr;
	T_DPST_LOAD    Load;
	T_DPST_DSGN    Dsgn;
	T_DPST_VIEW_CH View;
	T_DPST_SEIS    Seis;
	void Get(T_DPST_D &rData)
	{
		rData.GrupK = GrupK;
		memcpy(rData.bDpstD, bDpstD, sizeof(rData.bDpstD));
		rData.Node = Node;
		rData.Elem = Elem;
		rData.Prop = Prop;
		rData.Bndr = Bndr;
		rData.Load = Load;
		rData.Dsgn = Dsgn;
		rData.Seis = Seis;
		View.Get(rData.View);
	}
	void Set(T_DPST_D Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem = Data.Elem;
		Prop = Data.Prop;
		Bndr = Data.Bndr;
		Load = Data.Load;
		Dsgn = Data.Dsgn;
		Seis = Data.Seis;
		View.Set(Data.View);
	}
	T_DPST_D_CH() {}
	T_DPST_D_CH(const T_DPST_D_CH &src) { *this = src; }
	T_DPST_D_CH& operator=(const T_DPST_D_CH &src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn = src.Dsgn;
		Seis = src.Seis;
		View = src.View;
		return *this;
	}
	void Convert950(T_DPST_D_CH_950& src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn.Convert950(src.Dsgn);
		View = src.View;
		Seis = src.Seis;
	}
	void Convert940(T_DPST_D_CH_940& src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn.Convert940(src.Dsgn);
		View = src.View;
		Seis = src.Seis;
	}
	void Convert925(T_DPST_D_CH_925& src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn.Convert925(src.Dsgn);
		View.Convert930(src.View);//= src.View;
		Seis = src.Seis;
	}
	void Convert895(T_DPST_D_CH_895& src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn.Convert895(src.Dsgn);
		View.Convert930(src.View);
		Seis = src.Seis;
	}
	void Convert891(T_DPST_D_CH_891& src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn.Convert891(src.Dsgn);
		View.Convert930(src.View);
		Seis = src.Seis;
	}
	void Convert885(T_DPST_D_CH_885& src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load = src.Load;
		Dsgn.Convert885(src.Dsgn);
		View.Convert930(src.View);
		Seis = src.Seis;
	}
	void Convert883(T_DPST_D_CH_883& src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load.Convert883(src.Load);
		Dsgn.Convert883(src.Dsgn);
		View.Convert930(src.View);
		Seis = src.Seis;
	}
	void Convert870(T_DPST_D_CH_870& src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load.Convert883(src.Load);
		Dsgn.Convert883(src.Dsgn);
		View.Convert930(src.View);
	}
	void Convert865(T_DPST_D_CH_865& src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load.Convert865(src.Load);
		Dsgn.Convert865(src.Dsgn);
		View.Convert930(src.View);
	}
	void Convert860(T_DPST_D_CH_860& src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr = src.Bndr;
		Load.Convert865(src.Load);
		Dsgn.Convert860(src.Dsgn);
		View.Convert930(src.View);
	}
	void Convert855(T_DPST_D_CH_855& src)
	{
	  GrupK = src.GrupK;
	  memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
	  Node = src.Node;
	  Elem = src.Elem;
	  Prop = src.Prop;
	  Bndr.Convert855(src.Bndr);
	  Load.Convert865(src.Load);
	  Dsgn.Convert860(src.Dsgn);
	  View.Convert930(src.View);
	}
	void Convert841(T_DPST_D_CH_841& src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem = src.Elem;
		Prop = src.Prop;
		Bndr.Convert855(src.Bndr);
		Load.Convert865(src.Load);
		Dsgn.Convert841(src.Dsgn);
		View.Convert930(src.View);
	}
	void Convert835(T_DPST_D_CH_835& src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem.Convert835(src.Elem);
		Prop.Convert955(src.Prop);
		Bndr.Convert855(src.Bndr);
		Load.Convert865(src.Load);
		Dsgn.Convert841(src.Dsgn);
		View.Convert930(src.View);
	}
	void Convert821(T_DPST_D_CH_821& src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem.Convert835(src.Elem);
		Prop.Convert955(src.Prop);
		Bndr.Convert855(src.Bndr);
		Load.Convert865(src.Load);
		Dsgn.Convert821(src.Dsgn);
		View.Convert930(src.View);
	}
	void Convert810(T_DPST_D_CH_810& src)
	{
		GrupK = src.GrupK;
		memcpy(bDpstD, src.bDpstD, sizeof(bDpstD));
		Node = src.Node;
		Elem.Convert835(src.Elem);
		Prop.Convert955(src.Prop);
		Bndr.Convert855(src.Bndr);
		Load.Convert865(src.Load);
		Dsgn.Convert810(src.Dsgn);
		View.Convert930(src.View);
	}
};
struct T_DPST_D_CH_RW_810
{
	UINT GrupK;  
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE     Node;
	T_DPST_ELEM_835     Elem;
	T_DPST_PROP_955     Prop;
	T_DPST_BNDR_RW_855  Bndr;
	T_DPST_LOAD_RW_865  Load;
	T_DPST_DSGN_810     Dsgn;
	T_DPST_VIEW_CH_930  View;
	void Get(T_DPST_D_CH_810 &rData)
	{
		rData.GrupK = GrupK;
		memcpy(rData.bDpstD, bDpstD, sizeof(rData.bDpstD));
		rData.Node = Node;
		rData.Elem = Elem;
		rData.Prop = Prop;
		Bndr.Get(rData.Bndr);
		Load.Get(rData.Load);
		rData.Dsgn = Dsgn;
		rData.View = View;
	}
	void Set(T_DPST_D_CH_810 Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem = Data.Elem;
		Prop = Data.Prop;
		Bndr.Set(Data.Bndr);
		Load.Set(Data.Load);
		Dsgn = Data.Dsgn;
		View = Data.View;
	}
};   
struct T_DPST_D_CH_RW_821
{
	UINT GrupK;  
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE     Node;
	T_DPST_ELEM_835     Elem;
	T_DPST_PROP_955     Prop;
	T_DPST_BNDR_RW_855  Bndr;
	T_DPST_LOAD_RW_865  Load;
	T_DPST_DSGN_821     Dsgn;
	T_DPST_VIEW_CH_930  View;
	void Get(T_DPST_D_CH_821 &rData)
	{
		rData.GrupK = GrupK;
		memcpy(rData.bDpstD, bDpstD, sizeof(rData.bDpstD));
		rData.Node = Node;
		rData.Elem = Elem;
		rData.Prop = Prop;
		Bndr.Get(rData.Bndr);
		Load.Get(rData.Load);
		rData.Dsgn = Dsgn;
		rData.View = View;
	}
	void Set(T_DPST_D_CH_821 Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem = Data.Elem;
		Prop = Data.Prop;
		Bndr.Set(Data.Bndr);
		Load.Set(Data.Load);
		Dsgn = Data.Dsgn;
		View = Data.View;
	}
};   
struct T_DPST_D_CH_RW_835
{
	UINT GrupK;  
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE     Node;
	T_DPST_ELEM_835     Elem;
	T_DPST_PROP_955     Prop;
	T_DPST_BNDR_RW_855  Bndr;
	T_DPST_LOAD_RW_865  Load;
	T_DPST_DSGN_841     Dsgn;
	T_DPST_VIEW_CH_930  View;
	void Get(T_DPST_D_CH_835 &rData)
	{
		rData.GrupK = GrupK;
		memcpy(rData.bDpstD, bDpstD, sizeof(rData.bDpstD));
		rData.Node = Node;
		rData.Elem = Elem;
		rData.Prop = Prop;
		Bndr.Get(rData.Bndr);
		Load.Get(rData.Load);
		rData.Dsgn = Dsgn;
		rData.View = View;
	}
	void Set(T_DPST_D_CH_835 Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem = Data.Elem;
		Prop = Data.Prop;
		Bndr.Set(Data.Bndr);
		Load.Set(Data.Load);
		Dsgn = Data.Dsgn;
		View = Data.View;
	}
};   
struct T_DPST_D_CH_RW_841
{
	UINT GrupK;  
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE     Node;
	T_DPST_ELEM_940     Elem;
	T_DPST_PROP_955     Prop;
	T_DPST_BNDR_RW_855  Bndr;
	T_DPST_LOAD_RW_865  Load;
	T_DPST_DSGN_841     Dsgn;
	T_DPST_VIEW_CH_930  View;
	//void Get(T_DPST_D_CH_841 &rData)
	//{
	//	rData.GrupK = GrupK;
	//	memcpy(rData.bDpstD, bDpstD, sizeof(rData.bDpstD));
	//	rData.Node = Node;
	//	rData.Elem = Elem;
	//	rData.Prop = Prop;
	//	Bndr.Get(rData.Bndr);
	//	Load.Get(rData.Load);
	//	rData.Dsgn = Dsgn;
	//	rData.View = View;
	//}
	//void Set(T_DPST_D_CH_841 Data)
	//{
	//	GrupK = Data.GrupK;
	//	memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
	//	Node = Data.Node;
	//	Elem = Data.Elem;
	//	Prop = Data.Prop;
	//	Bndr.Set(Data.Bndr);
	//	Load.Set(Data.Load);
	//	Dsgn = Data.Dsgn;
	//	View = Data.View;
	//}
};   
struct T_DPST_D_CH_RW_855
{
	UINT GrupK;  
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE     Node;
	T_DPST_ELEM_940     Elem;
	T_DPST_PROP_955     Prop;
	T_DPST_BNDR_RW_855  Bndr;
	T_DPST_LOAD_RW_865  Load;
	T_DPST_DSGN_860 Dsgn;
	T_DPST_VIEW_CH_930  View;
	//void Get(T_DPST_D_CH_855 &rData)
	//{
	//	rData.GrupK = GrupK;
	//	memcpy(rData.bDpstD, bDpstD, sizeof(rData.bDpstD));
	//	rData.Node = Node;
	//	rData.Elem = Elem;
	//	rData.Prop = Prop;
	//	Bndr.Get(rData.Bndr);
	//	Load.Get(rData.Load);
	//	rData.Dsgn = Dsgn;
	//	rData.View = View;
	//}
	//void Set(T_DPST_D_CH_855 Data)
	//{
	//	GrupK = Data.GrupK;
	//	memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
	//	Node = Data.Node;
	//	Elem = Data.Elem;
	//	Prop = Data.Prop;
	//	Bndr.Set(Data.Bndr);
	//	Load.Set(Data.Load);
	//	Dsgn = Data.Dsgn;
	//	View = Data.View;
	//}
};

struct T_DPST_D_CH_RW_860
{
	UINT GrupK;  
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE     Node;
	T_DPST_ELEM_940     Elem;
	T_DPST_PROP_955     Prop;
	T_DPST_BNDR_RW_940  Bndr;
	T_DPST_LOAD_RW_865  Load;
	T_DPST_DSGN_860 Dsgn;
	T_DPST_VIEW_CH_930  View;
	//void Get(T_DPST_D_CH_860 &rData)
	//{
	//	rData.GrupK = GrupK;
	//	memcpy(rData.bDpstD, bDpstD, sizeof(rData.bDpstD));
	//	rData.Node = Node;
	//	rData.Elem = Elem;
	//	rData.Prop = Prop;
	//	Bndr.Get(rData.Bndr);
	//	Load.Get(rData.Load);
	//	rData.Dsgn = Dsgn;
	//	rData.View = View;
	//}
	//void Set(T_DPST_D_CH_860 Data)
	//{
	//	GrupK = Data.GrupK;
	//	memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
	//	Node = Data.Node;
	//	Elem = Data.Elem;
	//	Prop = Data.Prop;
	//	Bndr.Set(Data.Bndr);
	//	Load.Set(Data.Load);
	//	Dsgn = Data.Dsgn;
	//	View = Data.View;
	//}
};

struct T_DPST_D_CH_RW_865
{
	UINT GrupK;  
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE     Node;
	T_DPST_ELEM_940     Elem;
	T_DPST_PROP_955     Prop;
	T_DPST_BNDR_RW_940  Bndr;
	T_DPST_LOAD_RW_865  Load;
	T_DPST_DSGN_865 Dsgn;
	T_DPST_VIEW_CH_930  View;
	//void Get(T_DPST_D_CH_865 &rData)
	//{
	//	rData.GrupK = GrupK;
	//	memcpy(rData.bDpstD, bDpstD, sizeof(rData.bDpstD));
	//	rData.Node = Node;
	//	rData.Elem = Elem;
	//	rData.Prop = Prop;
	//	Bndr.Get(rData.Bndr);
	//	Load.Get(rData.Load);
	//	rData.Dsgn = Dsgn;
	//	rData.View = View;
	//}
	//void Set(T_DPST_D_CH_865 Data)
	//{
	//	GrupK = Data.GrupK;
	//	memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
	//	Node = Data.Node;
	//	Elem = Data.Elem;
	//	Prop = Data.Prop;
	//	Bndr.Set(Data.Bndr);
	//	Load.Set(Data.Load);
	//	Dsgn = Data.Dsgn;
	//	View = Data.View;
	//}
};
struct T_DPST_D_CH_RW_870
{
	UINT GrupK;  
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE     Node;
	T_DPST_ELEM_940     Elem;
	T_DPST_PROP_955     Prop;
	T_DPST_BNDR_RW_940  Bndr;
	T_DPST_LOAD_RW_883  Load;
	T_DPST_DSGN_883     Dsgn;
	T_DPST_VIEW_CH_930  View;
	//void Get(T_DPST_D_CH_870 &rData)
	//{
	//	rData.GrupK = GrupK;
	//	memcpy(rData.bDpstD, bDpstD, sizeof(rData.bDpstD));
	//	rData.Node = Node;
	//	rData.Elem = Elem;
	//	rData.Prop = Prop;
	//	Bndr.Get(rData.Bndr);
	//	Load.Get(rData.Load);
	//	rData.Dsgn = Dsgn;
	//	rData.View = View;
	//}
	//void Set(T_DPST_D_CH_870 Data)
	//{
	//	GrupK = Data.GrupK;
	//	memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
	//	Node = Data.Node;
	//	Elem = Data.Elem;
	//	Prop = Data.Prop;
	//	Bndr.Set(Data.Bndr);
	//	Load.Set(Data.Load);
	//	Dsgn = Data.Dsgn;
	//	View = Data.View;
	//}
};
struct T_DPST_D_CH_RW_883
{
	UINT GrupK;  
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE     Node;
	T_DPST_ELEM_940     Elem;
	T_DPST_PROP_955     Prop;
	T_DPST_BNDR_RW_940  Bndr;
	T_DPST_LOAD_RW_883  Load;
	T_DPST_DSGN_883     Dsgn;
	T_DPST_VIEW_CH_930  View;
	T_DPST_SEIS     Seis;
	//void Get(T_DPST_D_CH_883 &rData)
	//{
	//	rData.GrupK = GrupK;
	//	memcpy(rData.bDpstD, bDpstD, sizeof(rData.bDpstD));
	//	rData.Node = Node;
	//	rData.Elem = Elem;
	//	rData.Prop = Prop;
	//	Bndr.Get(rData.Bndr);
	//	Load.Get(rData.Load);
	//	rData.Dsgn = Dsgn;
	//	rData.View = View;
	//	rData.Seis = Seis;
	//}
	//void Set(T_DPST_D_CH_883 Data)
	//{
	//	GrupK = Data.GrupK;
	//	memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
	//	Node = Data.Node;
	//	Elem = Data.Elem;
	//	Prop = Data.Prop;
	//	Bndr.Set(Data.Bndr);
	//	Load.Set(Data.Load);
	//	Dsgn = Data.Dsgn;
	//	View = Data.View;
	//	Seis = Data.Seis;
	//}
};
struct T_DPST_D_CH_RW_885
{
	UINT GrupK;  
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE     Node;
	T_DPST_ELEM_940    Elem;
	T_DPST_PROP_955     Prop;
	T_DPST_BNDR_RW_940  Bndr;
	T_DPST_LOAD_RW_940  Load;
	T_DPST_DSGN_885 Dsgn;
	T_DPST_VIEW_CH_930  View;
	T_DPST_SEIS     Seis;
// 	void Get(T_DPST_D_CH_885 &rData)
// 	{
// 		rData.GrupK = GrupK;
// 		memcpy(rData.bDpstD, bDpstD, sizeof(rData.bDpstD));
// 		rData.Node = Node;
// 		rData.Elem = Elem;
// 		rData.Prop = Prop;
// 		Bndr.Get(rData.Bndr);
// 		Load.Get(rData.Load);
// 		rData.Dsgn = Dsgn;
// 		rData.View = View;
// 		rData.Seis = Seis;
// 	}
// 	void Set(T_DPST_D_CH_885 Data)
// 	{
// 		GrupK = Data.GrupK;
// 		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
// 		Node = Data.Node;
// 		Elem = Data.Elem;
// 		Prop = Data.Prop;
// 		Bndr.Set(Data.Bndr);
// 		Load.Set(Data.Load);
// 		Dsgn = Data.Dsgn;
// 		View = Data.View;
// 		Seis = Data.Seis;
// 	}
};
struct T_DPST_D_CH_RW_891
{
	UINT GrupK;  
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE     Node;
	T_DPST_ELEM_940     Elem;
	T_DPST_PROP_955     Prop;
	T_DPST_BNDR_RW_940  Bndr;
	T_DPST_LOAD_RW_940  Load;
	T_DPST_DSGN_891 Dsgn;
	T_DPST_VIEW_CH_930  View;
	T_DPST_SEIS     Seis;
// 	void Get(T_DPST_D_CH_891 &rData)
// 	{
// 		rData.GrupK = GrupK;
// 		memcpy(rData.bDpstD, bDpstD, sizeof(rData.bDpstD));
// 		rData.Node = Node;
// 		rData.Elem = Elem;
// 		rData.Prop = Prop;
// 		Bndr.Get(rData.Bndr);
// 		Load.Get(rData.Load);
// 		rData.Dsgn = Dsgn;
// 		rData.View = View;
// 		rData.Seis = Seis;
// 	}
// 	void Set(T_DPST_D_CH_891 Data)
// 	{
// 		GrupK = Data.GrupK;
// 		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
// 		Node = Data.Node;
// 		Elem = Data.Elem;
// 		Prop = Data.Prop;
// 		Bndr.Set(Data.Bndr);
// 		Load.Set(Data.Load);
// 		Dsgn = Data.Dsgn;
// 		View = Data.View;
// 		Seis = Data.Seis;
// 	}
};
struct T_DPST_D_CH_RW_895
{
	UINT GrupK;
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE     Node;
	T_DPST_ELEM_940     Elem;
	T_DPST_PROP_955     Prop;
	T_DPST_BNDR_RW_940  Bndr;
	T_DPST_LOAD_RW_940  Load;
	T_DPST_DSGN_895 Dsgn;
	T_DPST_VIEW_CH_930  View;
	T_DPST_SEIS     Seis;
// 	void Get(T_DPST_D_CH_895 &rData)
// 	{
// 		rData.GrupK = GrupK;
// 		memcpy(rData.bDpstD, bDpstD, sizeof(rData.bDpstD));
// 		rData.Node = Node;
// 		rData.Elem = Elem;
// 		rData.Prop = Prop;
// 		Bndr.Get(rData.Bndr);
// 		Load.Get(rData.Load);
// 		rData.Dsgn = Dsgn;
// 		rData.View = View;
// 		rData.Seis = Seis;
// 	}
// 	void Set(T_DPST_D_CH_895 Data)
// 	{
// 		GrupK = Data.GrupK;
// 		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
// 		Node = Data.Node;
// 		Elem = Data.Elem;
// 		Prop = Data.Prop;
// 		Bndr.Set(Data.Bndr);
// 		Load.Set(Data.Load);
// 		Dsgn = Data.Dsgn;
// 		View = Data.View;
// 		Seis = Data.Seis;
// 	}
};
struct T_DPST_D_CH_RW_925
{
	UINT GrupK;
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE     Node;
	T_DPST_ELEM_940     Elem;
	T_DPST_PROP_955     Prop;
	T_DPST_BNDR_RW_940  Bndr;
	T_DPST_LOAD_RW_940  Load;
	T_DPST_DSGN_925 Dsgn;
	T_DPST_VIEW_CH_930  View;
	T_DPST_SEIS     Seis;
// 	void Get(T_DPST_D_CH_925 &rData)
// 	{
// 		rData.GrupK = GrupK;
// 		memcpy(rData.bDpstD, bDpstD, sizeof(rData.bDpstD));
// 		rData.Node = Node;
// 		rData.Elem = Elem;
// 		rData.Prop = Prop;
// 		Bndr.Get(rData.Bndr);
// 		Load.Get(rData.Load);
// 		rData.Dsgn = Dsgn;
// 		rData.View = View;
// 		rData.Seis = Seis;
// 	}
// 	void Set(T_DPST_D_CH_925 Data)
// 	{
// 		GrupK = Data.GrupK;
// 		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
// 		Node = Data.Node;
// 		Elem = Data.Elem;
// 		Prop = Data.Prop;
// 		Bndr.Set(Data.Bndr);
// 		Load.Set(Data.Load);
// 		Dsgn = Data.Dsgn;
// 		View = Data.View;
// 		Seis = Data.Seis;
// 	}
};
struct T_DPST_D_CH_RW_930
{
	UINT GrupK;
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE     Node;
	T_DPST_ELEM_940     Elem;
	T_DPST_PROP_955     Prop;
	T_DPST_BNDR_RW_940  Bndr;
	T_DPST_LOAD_RW_940  Load;
	T_DPST_DSGN_940     Dsgn;
	T_DPST_VIEW_CH_930  View;
	T_DPST_SEIS     Seis;
// 	void Get(T_DPST_D_CH_930 &rData)
// 	{
// 		rData.GrupK = GrupK;
// 		memcpy(rData.bDpstD, bDpstD, sizeof(rData.bDpstD));
// 		rData.Node = Node;
// 		rData.Elem = Elem;
// 		rData.Prop = Prop;
// 		Bndr.Get(rData.Bndr);
// 		Load.Get(rData.Load);
// 		rData.Dsgn = Dsgn;
// 		rData.View = View;
// 		rData.Seis = Seis;
// 	}
// 	void Set(T_DPST_D_CH_930 Data)
// 	{
// 		GrupK = Data.GrupK;
// 		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
// 		Node = Data.Node;
// 		Elem = Data.Elem;
// 		Prop = Data.Prop;
// 		Bndr.Set(Data.Bndr);
// 		Load.Set(Data.Load);
// 		Dsgn = Data.Dsgn;
// 		View = Data.View;
// 		Seis = Data.Seis;
// 	}
};

struct T_DPST_D_CH_RW_940
{
	UINT GrupK;
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE     Node;
	T_DPST_ELEM_940     Elem;
	T_DPST_PROP_955     Prop;
	T_DPST_BNDR_RW_940  Bndr;
	T_DPST_LOAD_RW_940  Load;
	T_DPST_DSGN_940     Dsgn;
	T_DPST_VIEW_CH  View;
	T_DPST_SEIS     Seis;
};

struct T_DPST_D_CH_RW_950
{
	UINT GrupK;
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE     Node;
	T_DPST_ELEM_955 Elem;
	T_DPST_PROP_955 Prop;
	T_DPST_BNDR_RW_950  Bndr;
	T_DPST_LOAD_RW  Load;
	T_DPST_DSGN_950 Dsgn;
	T_DPST_VIEW_CH  View;
	T_DPST_SEIS     Seis;
};

struct T_DPST_D_CH_RW_950_2
{
	UINT GrupK;
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE     Node;
	T_DPST_ELEM_955 Elem;
	T_DPST_PROP_955 Prop;
	T_DPST_BNDR_RW_950  Bndr;
	T_DPST_LOAD_RW  Load;
	T_DPST_DSGN     Dsgn;
	T_DPST_VIEW_CH  View;
	T_DPST_SEIS     Seis;
};

struct T_DPST_D_CH_RW_955
{
	UINT GrupK;
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE     Node;
	T_DPST_ELEM_955 Elem;
	T_DPST_PROP_955 Prop;
	T_DPST_BNDR_RW  Bndr;
	T_DPST_LOAD_RW  Load;
	T_DPST_DSGN     Dsgn;
	T_DPST_VIEW_CH  View;
	T_DPST_SEIS     Seis;
};

struct T_DPST_D_CH_RW_965
{
	UINT GrupK;
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE     Node;
	T_DPST_ELEM_965 Elem;
	T_DPST_PROP     Prop;
	T_DPST_BNDR_RW  Bndr;
	T_DPST_LOAD_RW  Load;
	T_DPST_DSGN     Dsgn;
	T_DPST_VIEW_CH  View;
	T_DPST_SEIS     Seis;
};

struct T_DPST_D_CH_RW
{
	UINT GrupK;  
	BOOL bDpstD[EN_DPST_D_END];
	T_DPST_NODE     Node;
	T_DPST_ELEM     Elem;
	T_DPST_PROP     Prop;
	T_DPST_BNDR_RW  Bndr;
	T_DPST_LOAD_RW  Load;
	T_DPST_DSGN     Dsgn;
	T_DPST_VIEW_CH  View;
	T_DPST_SEIS     Seis;
	void Get(T_DPST_D_CH &rData)
	{
		rData.GrupK = GrupK;
		memcpy(rData.bDpstD, bDpstD, sizeof(rData.bDpstD));
		rData.Node = Node;
		rData.Elem = Elem;
		rData.Prop = Prop;
		Bndr.Get(rData.Bndr);
		Load.Get(rData.Load);
		rData.Dsgn = Dsgn;
		rData.View = View;
		rData.Seis = Seis;
	}
	void Set(T_DPST_D_CH Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem = Data.Elem;
		Prop = Data.Prop;
		Bndr.Set(Data.Bndr);
		Load.Set(Data.Load);
		Dsgn = Data.Dsgn;
		View = Data.View;
		Seis = Data.Seis;
	}
	void Convert965(T_DPST_D_CH_RW_965 Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem.Convert965(Data.Elem);
		Prop = Data.Prop;
		Bndr = Data.Bndr;
		Load = Data.Load;
		Dsgn = Data.Dsgn;
		View = Data.View;
		Seis = Data.Seis;
	}
	void Convert955(T_DPST_D_CH_RW_955 Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem.Convert955(Data.Elem);
		Prop.Convert955(Data.Prop);
		Bndr = Data.Bndr;
		Load = Data.Load;
		Dsgn = Data.Dsgn;
		View = Data.View;
		Seis = Data.Seis;
	}
	void Convert950_2(T_DPST_D_CH_RW_950_2& Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem.Convert955(Data.Elem);
		Prop.Convert955(Data.Prop);
		Bndr.Convert950_2(Data.Bndr);
		Load = Data.Load;
		Dsgn = Data.Dsgn;
		View = Data.View;
		Seis = Data.Seis;
	}
	void Convert950(T_DPST_D_CH_RW_950& Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem.Convert955(Data.Elem);
		Prop.Convert955(Data.Prop);
		Bndr.Convert950_2(Data.Bndr);
		Load = Data.Load;
		Dsgn.Convert950(Data.Dsgn);
		View = Data.View;
		Seis = Data.Seis;
	}
	void Convert940(T_DPST_D_CH_RW_940& Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem.Convert940(Data.Elem);
		Prop.Convert955(Data.Prop);
		Bndr.Convert940(Data.Bndr);
		Load.Convert940(Data.Load);
		Dsgn.Convert940(Data.Dsgn);
		View = Data.View;
		Seis = Data.Seis;
	}
	void Convert930(T_DPST_D_CH_RW_930& Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem.Convert940(Data.Elem);
		Prop.Convert955(Data.Prop);
		Bndr.Convert940(Data.Bndr);
		Load.Convert940(Data.Load);
		Dsgn.Convert940(Data.Dsgn);
		View.Convert930(Data.View);
		Seis = Data.Seis;
	}
	void Convert925(T_DPST_D_CH_RW_925& Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem.Convert940(Data.Elem);
		Prop.Convert955(Data.Prop);
		Bndr.Convert940(Data.Bndr);
		Load.Convert940(Data.Load);
		Dsgn.Convert925(Data.Dsgn);
		View.Convert930(Data.View);
		Seis = Data.Seis;
	}
	void Convert895(T_DPST_D_CH_RW_895& Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem.Convert940(Data.Elem);
		Prop.Convert955(Data.Prop);
		Bndr.Convert940(Data.Bndr);
		Load.Convert940(Data.Load);
		Dsgn.Convert895(Data.Dsgn);
		View.Convert930(Data.View);
		Seis = Data.Seis;
	}
	void Convert891(T_DPST_D_CH_RW_891& Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem.Convert940(Data.Elem);
		Prop.Convert955(Data.Prop);
		Bndr.Convert940(Data.Bndr);
		Load.Convert940(Data.Load);
		Dsgn.Convert891(Data.Dsgn);
		View.Convert930(Data.View);
		Seis = Data.Seis;
	}
	void Convert885(T_DPST_D_CH_RW_885& Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem.Convert940(Data.Elem);
		Prop.Convert955(Data.Prop);
		Bndr.Convert940(Data.Bndr);
		Load.Convert940(Data.Load);
		Dsgn.Convert885(Data.Dsgn);
		View.Convert930(Data.View);
		Seis = Data.Seis;
	}
	void Convert883(T_DPST_D_CH_RW_883& Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem.Convert940(Data.Elem);
		Prop.Convert955(Data.Prop);
		Bndr.Convert940(Data.Bndr);
		Load.Convert883(Data.Load);
		Dsgn.Convert883(Data.Dsgn);
		View.Convert930(Data.View);
		Seis = Data.Seis;
	}
	void Convert870(T_DPST_D_CH_RW_870& Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem.Convert940(Data.Elem);
		Prop.Convert955(Data.Prop);
		Bndr.Convert940(Data.Bndr);
		Load.Convert883(Data.Load);
		Dsgn.Convert883(Data.Dsgn);
		View.Convert930(Data.View);
	}
	void Convert865(T_DPST_D_CH_RW_865& Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem.Convert940(Data.Elem);
		Prop.Convert955(Data.Prop);
		Bndr.Convert940(Data.Bndr);
		Load.Convert865(Data.Load);
		Dsgn.Convert865(Data.Dsgn);
		View.Convert930(Data.View);
	}
	void Convert860(T_DPST_D_CH_RW_860& Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem.Convert940(Data.Elem);
		Prop.Convert955(Data.Prop);
		Bndr.Convert940(Data.Bndr);
		Load.Convert865(Data.Load);
		Dsgn.Convert860(Data.Dsgn);
		View.Convert930(Data.View);
	}
	void Convert855(T_DPST_D_CH_RW_855& Data)
	{
	  GrupK = Data.GrupK;
	  memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
	  Node = Data.Node;
	  Elem.Convert940(Data.Elem);
	  Prop.Convert955(Data.Prop);
	  Bndr.Convert855(Data.Bndr);
	  Load.Convert865(Data.Load);
	  Dsgn.Convert860(Data.Dsgn);
	  View.Convert930(Data.View);
	}
	void Convert841(T_DPST_D_CH_RW_841& Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem.Convert940(Data.Elem);
		Prop.Convert955(Data.Prop);
		Bndr.Convert855(Data.Bndr);
		Load.Convert865(Data.Load);
		Dsgn.Convert841(Data.Dsgn);
		View.Convert930(Data.View);
	}
	void Convert835(T_DPST_D_CH_RW_835& Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem.Convert835(Data.Elem);
		Prop.Convert955(Data.Prop);
		Bndr.Convert855(Data.Bndr);
		Load.Convert865(Data.Load);
		Dsgn.Convert841(Data.Dsgn);
		View.Convert930(Data.View);
	}
	void Convert821(T_DPST_D_CH_RW_821& Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem.Convert835(Data.Elem);
		Prop.Convert955(Data.Prop);
		Bndr.Convert855(Data.Bndr);
		Load.Convert865(Data.Load);
		Dsgn.Convert821(Data.Dsgn);
		View.Convert930(Data.View);
	}
	void Convert810(T_DPST_D_CH_RW_810& Data)
	{
		GrupK = Data.GrupK;
		memcpy(bDpstD, Data.bDpstD, sizeof(bDpstD));
		Node = Data.Node;
		Elem.Convert835(Data.Elem);
		Prop.Convert955(Data.Prop);
		Bndr.Convert855(Data.Bndr);
		Load.Convert865(Data.Load);
		Dsgn.Convert810(Data.Dsgn);
		View.Convert930(Data.View);
	}
};   

// Pre & Common Data
struct T_ACTIVE_IDENTITY_DB
{
	int   nActiveCategory;            // Active Identity's category (-1:None, 0:Active All, 1:Named Plane, 2:Boundary Group, 3:Member, 4:Story, 5:Group, 6:Load Group, 7:User, 8:Active UCS Plane)
	int   nOption;                    // Story Option
	CMapEx<UINT,UINT,UINT,UINT> mKey; // 해당 항목의 Keys (User: Element Keys,   None,Active All: 없음)

	T_ACTIVE_IDENTITY_DB()
	{
		Initialize();
	}
	void Initialize()
	{
		nActiveCategory = 0; // EN_ACTIVE_CAT_ALL
		nOption = 0;
		mKey.RemoveAll();
		//mKey.InitHashTable(D_MAP_MAX_SIZE);
	}
	T_ACTIVE_IDENTITY_DB(const T_ACTIVE_IDENTITY_DB &src) { *this = src; }
	T_ACTIVE_IDENTITY_DB& operator=(const T_ACTIVE_IDENTITY_DB& rData)
	{
		nActiveCategory = rData.nActiveCategory;
		nOption = rData.nOption;
		UINT nKey=0, nData=0;
		POSITION pos = rData.mKey.GetStartPosition();
		while(pos)
		{
			rData.mKey.GetNextAssoc(pos, nKey, nData);
			mKey.SetAt(nKey, nData);
		}
		return *this;
	}
};
// 저장할 때 쓰이도록 임시적으로 만듬 (일단 20개만 저장, 개선 필요)
#define MAX_SAVE 20
struct T_ACTIVE_IDENTITY_DB_RW
{
	int   nActiveCategory;
	int   nOption;
	UINT  mKey[MAX_SAVE];

	T_ACTIVE_IDENTITY_DB_RW()
	{
		Initialize();
	}
	void Initialize()
	{
		nActiveCategory = 0; // EN_ACTIVE_CAT_ALL
		nOption = 0;
		memset(mKey, 0, sizeof(mKey));
	}
	T_ACTIVE_IDENTITY_DB_RW(const T_ACTIVE_IDENTITY_DB_RW &src) { *this = src; }
	T_ACTIVE_IDENTITY_DB_RW& operator=(const T_ACTIVE_IDENTITY_DB_RW& rData)
	{
		nActiveCategory = rData.nActiveCategory;
		nOption = rData.nOption;
		memcpy(mKey, rData.mKey, sizeof(mKey));
		return *this;
	}
	void Get(T_ACTIVE_IDENTITY_DB& target)
	{
		target.nActiveCategory = nActiveCategory;
		target.nOption = nOption;
		for(int i=0 ; i<MAX_SAVE ; ++i)
		{
			if(mKey[i]==0) break;
			target.mKey.SetAt(mKey[i], mKey[i]);
		}
	}
	void Set(T_ACTIVE_IDENTITY_DB src)
	{
		nActiveCategory = src.nActiveCategory;
		nOption = src.nOption;
		memset(mKey, 0, sizeof(mKey));
		UINT nKey=0, nData=0;
		int i=0;
		POSITION pos = src.mKey.GetStartPosition();
		while(pos)
		{
			src.mKey.GetNextAssoc(pos, nKey, nData);
			mKey[i] = nKey;
			i++;
			if(i==MAX_SAVE) break;
		}
	}
};
#undef MAX_SAVE

struct T_GTransMatrix_D
{
	double M00,M01,M02,M03;
	double M10,M11,M12,M13;
	double M20,M21,M22,M23;
	double M30,M31,M32,M33;

	void Initialize()
	{
		M00 = 1.0;
		M01 = 0.;
		M02 = 0.;
		M03 = 0.;
		
		M10 = 0.;
		M11 = 1.0;
		M12 = 0.;
		M13 = 0.;
		
		M20 = 0.;
		M21 = 0.;
		M22 = 1.0;
		M23 = 0.;
		
		M30 = 0.;
		M31 = 0.;
		M32 = 0.;
	  M33 = 1.0;
	}
	T_GTransMatrix_D() { Initialize(); }
	T_GTransMatrix_D(const T_GTransMatrix_D &src) { *this = src; }
	T_GTransMatrix_D& operator=(const T_GTransMatrix_D &src)
	{
		M00 = src.M00;
		M01 = src.M01;
		M02 = src.M02;
		M03 = src.M03;
		
		M10 = src.M10;
		M11 = src.M11;
		M12 = src.M12;
		M13 = src.M13;
		
		M20 = src.M20;
		M21 = src.M21;
		M22 = src.M22;
		M23 = src.M23;
		
		M30 = src.M30;
		M31 = src.M31;
		M32 = src.M32;
	  M33 = src.M33;

		return *this;
	}
};

struct T_GPoint3D_D
{
	double x,y,z;  

	void Initialize()
	{
		x = 0.;
		y = 0.;
		z = 0.;
	}
	T_GPoint3D_D() { Initialize(); }
	T_GPoint3D_D(const T_GPoint3D_D &src) { *this = src; }
	T_GPoint3D_D& operator=(const T_GPoint3D_D &src)
	{
		x = src.x;
		y = src.y;
		z = src.z;
		
		return *this;
	}
};

struct T_GVector_D
{
	double x,y,z;  
	
	void Initialize()
	{
		x = 0.;
		y = 0.;
		z = 0.;
	}
	T_GVector_D() { Initialize(); }
	T_GVector_D(const T_GVector_D &src) { *this = src; }
	T_GVector_D& operator=(const T_GVector_D &src)
	{
		x = src.x;
		y = src.y;
		z = src.z;
		
		return *this;
	}
};

struct T_GRect_D
{
	double left;
	double bottom;
	double right;
	double top;
	double depth; // Z Depth
	
	void Initialize()
	{
		left   = 0.0;
		top    = 0.0;
		right  = 0.0;
		bottom = 0.0;
		depth  = 0.0;
	}
	T_GRect_D() { Initialize(); }
	T_GRect_D(const T_GRect_D &src) { *this = src; }
	T_GRect_D& operator=(const T_GRect_D &src)
	{
		left   = src.left  ;
		top    = src.top   ;
		right  = src.right ;
		bottom = src.bottom;
		depth  = src.depth ;
		
		return *this;
	}
};

#define D_MATRIX_NUM_4_BY_4 16 
#define D_MATRIX_NUM_4 4

// GRenderEngine의 일부 정보 (계산과정에서 생기는 데이터를 제외한)
struct T_RETextMod_D
{
	T_GTransMatrix_D                          NonScaledWcsToVcsM;
	T_GTransMatrix_D                          AScaleFactorM     ;
	T_GTransMatrix_D                          OWcsToVcsM        ;  
	T_GTransMatrix_D                          OVcsToWcsM        ;
	T_GTransMatrix_D                          WcsToVcsM         ;     
	T_GTransMatrix_D                          VcsToWcsM         ;     
	T_GTransMatrix_D                          PerspectiveM      ;
	T_GTransMatrix_D                          IPerspectiveM     ;
	T_GTransMatrix_D                          VcsToNdcM         ;         
	T_GTransMatrix_D                          NdcToVcsM         ;         
	T_GTransMatrix_D                          NdcToWindowM      ;	
	T_GTransMatrix_D                          WindowToNdcM      ;  
	T_GTransMatrix_D                          ViewPortToWindowM ;   
	T_GTransMatrix_D                          WindowToViewPortM ;   
	T_GTransMatrix_D                          ScreenToViewPortM ;
	T_GTransMatrix_D                          ViewPortToScreenM ;
	T_GTransMatrix_D                          WcsToUcsM         ;	        
	T_GTransMatrix_D                          UcsToWcsM         ;         
	T_GTransMatrix_D                          UcsToViewPortM    ;   
	T_GTransMatrix_D                          ViewPortToUcsM    ;
	T_GTransMatrix_D                          ViewPortToVcsM    ;
	T_GTransMatrix_D                          VcsToViewPortM    ;
	T_GTransMatrix_D                          WcsToViewPortM    ;
	T_GTransMatrix_D                          WcsToWindowM      ;
	T_GTransMatrix_D                          WindowToScreenM   ;
	T_GTransMatrix_D                          ScreenToWcsM      ;
	T_GTransMatrix_D                          WcsToScreenM      ;
	T_GTransMatrix_D                          VcsToScreenM      ;
	T_GTransMatrix_D                          ScreenToVcsM      ;
	T_GTransMatrix_D                          ScreenToNdcM      ;
	T_GTransMatrix_D                          WcsToNdcM         ;
	T_GTransMatrix_D                          NdcToWcsM         ;
	T_GTransMatrix_D                          NdcToScreenM      ;
	T_GTransMatrix_D                          GLNdcToREVcs      ;
	T_GTransMatrix_D                          XGL_IPerspectiveM ;
	T_GTransMatrix_D                          XGL__PerspectiveM ;
	T_GTransMatrix_D                          XGL_INdcScaleM    ;
	T_GTransMatrix_D                          XGL__NdcScaleM    ;
	T_GTransMatrix_D                          XGL_QcsTransM     ; // (QcsTransM * QcsM   == WcsToVcsM)
	T_GTransMatrix_D                          XGL_IQcsTransM    ; // (IQcsM * IQcsTransM == VcsToWcsM)
	T_GTransMatrix_D                          XGL_QcsM          ; // Quad Tree Coordinate System
	T_GTransMatrix_D                          XGL_IQcsM         ; // Inverse Quad Tree Coordinate System
	T_GTransMatrix_D                          XGL_QcsToVcsM     ;
	T_GTransMatrix_D                          XGL_VcsToQcsM     ;
	T_GTransMatrix_D                          XGL_QcsToScreenM  ;
	T_GTransMatrix_D                          XGL_ScreenToQcsM  ;
	T_GTransMatrix_D                          XGL__ProjectionM  ;
	T_GTransMatrix_D                          XGL_IProjectionM  ;
	T_GPoint3D_D                              XGL_RefPos        ;
	T_GPoint3D_D                              XGL_EyePos        ;
	T_GPoint3D_D                              XGL_UpVPos        ;
	double XGL_Near   ;
	double XGL_Far    ;
	double XGL_Left   ;
	double XGL_Bottom ; 
	double XGL_Right  ; 
	double XGL_Top    ;
	
	//--------------------------------------------------------------------
	// 연관된 GLView의 ModelView , Projection , Viewport Matrix를 저장한다. 
	float XGL_ModelViewMatrix [D_MATRIX_NUM_4_BY_4];
	float XGL_ProjectionMatrix[D_MATRIX_NUM_4_BY_4];
	int   XGL_ViewPort        [D_MATRIX_NUM_4];

	//--------------------------------------------------------------------
	// DATA COPY
	BOOL                        bIsApplyGridBoundary        ; 
	double                      GridXSpaceInUcs             ;
	double                      GridYSpaceInUcs             ;
	int                         GridSpaceLimitOnScreen      ;
	BOOL                        bIsShowPointGrid            ;
	BOOL                        bIsShowLineGrid             ;
	double CFS_WXL, CFS_WXH, CFS_WYL, CFS_WYH, CFS_WZL, CFS_WZH; // m_ClipperForScreen의 일부정보
	double CFC_WXL, CFC_WXH, CFC_WYL, CFC_WYH, CFC_WZL, CFC_WZH; // m_ClipperForClientWindow의 일부정보
	int                         nSmallCursorLength          ;
	BOOL                        bIsPolygonFill              ;
	BOOL                        bOrthogonalMode             ;
	BOOL                        bIsViewingMode              ;
	BOOL                        bIsDynamicViewMode          ;
	T_GPoint3D_D                VanishingPointInVcs         ;
	T_GPoint3D_D                NavigatorPosInVcs           ;
	T_GPoint3D_D                ViewSite                    ;
	T_GPoint3D_D                ViewPoint                   ;
	T_GPoint3D_D                ViewUpVectorPoint           ;
	T_GVector_D                 ViewDirectionVector         ;
	T_GVector_D                 ViewUpVector                ;
	double                      PerspectiveRatio            ;
	double                      PerspectiveRatioPercentage  ;
	T_GRect_D                   GRectViewPortOnClientWindow ;
	T_GRect_D                   GRectViewPortOnViewPort     ;
	T_GRect_D                   GRectScreenOnClientWindow   ;
	T_GRect_D                   GRectWindow                 ;
	T_GRect_D                   GRectWindowForZoomAll       ;
	T_GRect_D                   GRectViewPort               ;  
	T_GRect_D                   GRectScreen                 ;
	int                         nVCLM                       ;
	int                         nVCRM                       ;
	int                         nVCTM                       ;
	int                         nVCBM                       ;
	COLORREF                    crMarginFillColor           ;
	COLORREF                    crMarginOutLineColor        ;
	double                      ZoomAllMargin               ;
	BOOL                        bIsShade                    ;

	void Initialize()
	{
		NonScaledWcsToVcsM.Initialize();
		AScaleFactorM     .Initialize();
		OWcsToVcsM        .Initialize();
		OVcsToWcsM        .Initialize();
		WcsToVcsM         .Initialize();
		VcsToWcsM         .Initialize();
		PerspectiveM      .Initialize();
		IPerspectiveM     .Initialize();
		VcsToNdcM         .Initialize();
		NdcToVcsM         .Initialize();
		NdcToWindowM      .Initialize();
		WindowToNdcM      .Initialize();
		ViewPortToWindowM .Initialize();
	  WindowToViewPortM .Initialize();
		ScreenToViewPortM .Initialize();
		ViewPortToScreenM .Initialize();
		WcsToUcsM         .Initialize();
		UcsToWcsM         .Initialize();
		UcsToViewPortM    .Initialize();
		ViewPortToUcsM    .Initialize();
		ViewPortToVcsM    .Initialize();
		VcsToViewPortM    .Initialize();
		WcsToViewPortM    .Initialize();
		WcsToWindowM      .Initialize();
		WindowToScreenM   .Initialize();
		ScreenToWcsM      .Initialize();
		WcsToScreenM      .Initialize();
		VcsToScreenM      .Initialize();
		ScreenToVcsM      .Initialize();
		ScreenToNdcM      .Initialize();
		WcsToNdcM         .Initialize();
		NdcToWcsM         .Initialize();
		NdcToScreenM      .Initialize();
		GLNdcToREVcs      .Initialize();
		XGL_IPerspectiveM .Initialize();
		XGL__PerspectiveM .Initialize();
		XGL_INdcScaleM    .Initialize();
		XGL__NdcScaleM    .Initialize();
		XGL_QcsTransM     .Initialize();
		XGL_IQcsTransM    .Initialize();
		XGL_QcsM          .Initialize();
		XGL_IQcsM         .Initialize();
		XGL_QcsToVcsM     .Initialize();
		XGL_VcsToQcsM     .Initialize();
		XGL_QcsToScreenM  .Initialize();
		XGL_ScreenToQcsM  .Initialize();
		XGL__ProjectionM  .Initialize();
		XGL_IProjectionM  .Initialize();
		XGL_RefPos        .Initialize();
		XGL_EyePos        .Initialize();
		XGL_UpVPos        .Initialize();
		XGL_Near   = -1.;
		XGL_Far    = 1.;
		XGL_Left   = -1.;
		XGL_Bottom = -1.;
		XGL_Right  = 1.; 
		XGL_Top    = 1.; 

		for(int i=0; i<D_MATRIX_NUM_4_BY_4; i++)
		{
			XGL_ModelViewMatrix [i] = 0.0;
			XGL_ProjectionMatrix[i] = 0.0;
		}
		for(int i = 0; i<D_MATRIX_NUM_4; i++)
			XGL_ViewPort[i] = 0;

		//--------------------------------------------------------------------
		// DATA COPY
		bIsApplyGridBoundary        = FALSE; 
		GridXSpaceInUcs             = 0;
		GridYSpaceInUcs             = 0;
		GridSpaceLimitOnScreen      = 0;
		bIsShowPointGrid            = FALSE;
		bIsShowLineGrid             = FALSE;
		CFS_WXL                     = 0.;
		CFS_WXH                     = 0.;
		CFS_WYL                     = 0.;
		CFS_WYH                     = 0.;
		CFS_WZL                     = 0.;
		CFS_WZH                     = 0.;
		CFC_WXL                     = 0.;
		CFC_WXH                     = 0.;
		CFC_WYL                     = 0.;
		CFC_WYH                     = 0.;
		CFC_WZL                     = 0.;
		CFC_WZH                     = 0.;
		
		nSmallCursorLength          = 0;
		bIsPolygonFill              = FALSE;
		bOrthogonalMode             = FALSE;
		bIsViewingMode              = FALSE;
		bIsDynamicViewMode          = FALSE;
		VanishingPointInVcs         .Initialize();
		NavigatorPosInVcs           .Initialize();
		ViewSite                    .Initialize();
		ViewPoint                   .Initialize();
		ViewUpVectorPoint           .Initialize();
		ViewDirectionVector         .Initialize();
		ViewUpVector                .Initialize();
		PerspectiveRatio            = 1.;
		PerspectiveRatioPercentage  = 0.;
		GRectViewPortOnClientWindow .Initialize();
		GRectViewPortOnViewPort     .Initialize();
		GRectScreenOnClientWindow   .Initialize();
		GRectWindow                 .Initialize();
		GRectWindowForZoomAll       .Initialize();
		GRectViewPort               .Initialize();  
		GRectScreen                 .Initialize();
		nVCLM                       = 0;
		nVCRM                       = 0;
		nVCTM                       = 0;
		nVCBM                       = 0;
		crMarginFillColor           = RGB(0,0,0);
		crMarginOutLineColor        = RGB(0,0,0);
		ZoomAllMargin               = 0;
	  bIsShade                    = TRUE;
	}
	T_RETextMod_D() { Initialize(); }
	T_RETextMod_D(const T_RETextMod_D &src) { *this = src; }
	T_RETextMod_D& operator=(const T_RETextMod_D &src)
	{
		NonScaledWcsToVcsM  = src.NonScaledWcsToVcsM;
		AScaleFactorM       = src.AScaleFactorM     ;
		OWcsToVcsM          = src.OWcsToVcsM        ;
		OVcsToWcsM          = src.OVcsToWcsM        ;
		WcsToVcsM           = src.WcsToVcsM         ;
		VcsToWcsM           = src.VcsToWcsM         ;
		PerspectiveM        = src.PerspectiveM      ;
		IPerspectiveM       = src.IPerspectiveM     ;
		VcsToNdcM           = src.VcsToNdcM         ;
		NdcToVcsM           = src.NdcToVcsM         ;
		NdcToWindowM        = src.NdcToWindowM      ;
		WindowToNdcM        = src.WindowToNdcM      ;
		ViewPortToWindowM   = src.ViewPortToWindowM ;
	  WindowToViewPortM   = src.WindowToViewPortM ;
		ScreenToViewPortM   = src.ScreenToViewPortM ;
		ViewPortToScreenM   = src.ViewPortToScreenM ;
		WcsToUcsM           = src.WcsToUcsM         ;
		UcsToWcsM           = src.UcsToWcsM         ;
		UcsToViewPortM      = src.UcsToViewPortM    ;
		ViewPortToUcsM      = src.ViewPortToUcsM    ;
		ViewPortToVcsM      = src.ViewPortToVcsM    ;
		VcsToViewPortM      = src.VcsToViewPortM    ;
		WcsToViewPortM      = src.WcsToViewPortM    ;
		WcsToWindowM        = src.WcsToWindowM      ;
		WindowToScreenM     = src.WindowToScreenM   ;
		ScreenToWcsM        = src.ScreenToWcsM      ;
		WcsToScreenM        = src.WcsToScreenM      ;
		VcsToScreenM        = src.VcsToScreenM      ;
		ScreenToVcsM        = src.ScreenToVcsM      ;
		ScreenToNdcM        = src.ScreenToNdcM      ;
		WcsToNdcM           = src.WcsToNdcM         ;
		NdcToWcsM           = src.NdcToWcsM         ;
		NdcToScreenM        = src.NdcToScreenM      ;
		GLNdcToREVcs        = src.GLNdcToREVcs      ;
		XGL_IPerspectiveM   = src.XGL_IPerspectiveM ;
		XGL__PerspectiveM   = src.XGL__PerspectiveM ;
		XGL_INdcScaleM      = src.XGL_INdcScaleM    ;
		XGL__NdcScaleM      = src.XGL__NdcScaleM    ;
		XGL_QcsTransM       = src.XGL_QcsTransM     ;
		XGL_IQcsTransM      = src.XGL_IQcsTransM    ;
		XGL_QcsM            = src.XGL_QcsM          ;
		XGL_IQcsM           = src.XGL_IQcsM         ;
		XGL_QcsToVcsM       = src.XGL_QcsToVcsM     ;
		XGL_VcsToQcsM       = src.XGL_VcsToQcsM     ;
		XGL_QcsToScreenM    = src.XGL_QcsToScreenM  ;
		XGL_ScreenToQcsM    = src.XGL_ScreenToQcsM  ;
		XGL__ProjectionM    = src.XGL__ProjectionM  ;
		XGL_IProjectionM    = src.XGL_IProjectionM  ;
		XGL_RefPos          = src.XGL_RefPos        ;
		XGL_EyePos          = src.XGL_EyePos        ;
		XGL_UpVPos          = src.XGL_UpVPos        ;
		XGL_Near   = src.XGL_Near  ;
		XGL_Far    = src.XGL_Far   ;
		XGL_Left   = src.XGL_Left  ;
		XGL_Bottom = src.XGL_Bottom;
		XGL_Right  = src.XGL_Right ;
		XGL_Top    = src.XGL_Top   ;
		
		memcpy(XGL_ModelViewMatrix  ,src.XGL_ModelViewMatrix  ,sizeof(XGL_ModelViewMatrix[0])*D_MATRIX_NUM_4_BY_4);
		memcpy(XGL_ProjectionMatrix ,src.XGL_ProjectionMatrix ,sizeof(XGL_ProjectionMatrix[0])*D_MATRIX_NUM_4_BY_4);
		memcpy(XGL_ViewPort         ,src.XGL_ViewPort         ,sizeof(XGL_ViewPort[0])*D_MATRIX_NUM_4);

		//--------------------------------------------------------------------
		// DATA COPY
		bIsApplyGridBoundary        = src.bIsApplyGridBoundary  ; 
		GridXSpaceInUcs             = src.GridXSpaceInUcs       ; 
		GridYSpaceInUcs             = src.GridYSpaceInUcs       ; 
		GridSpaceLimitOnScreen      = src.GridSpaceLimitOnScreen; 
		bIsShowPointGrid            = src.bIsShowPointGrid      ; 
		bIsShowLineGrid             = src.bIsShowLineGrid       ; 
		CFS_WXL                     = src.CFS_WXL               ; 
		CFS_WXH                     = src.CFS_WXH               ; 
		CFS_WYL                     = src.CFS_WYL               ; 
		CFS_WYH                     = src.CFS_WYH               ; 
		CFS_WZL                     = src.CFS_WZL               ; 
		CFS_WZH                     = src.CFS_WZH               ; 
		CFC_WXL                     = src.CFC_WXL               ; 
		CFC_WXH                     = src.CFC_WXH               ; 
		CFC_WYL                     = src.CFC_WYL               ; 
		CFC_WYH                     = src.CFC_WYH               ; 
		CFC_WZL                     = src.CFC_WZL               ; 
		CFC_WZH                     = src.CFC_WZH               ; 
		
		nSmallCursorLength          = src.nSmallCursorLength    ; 
		bIsPolygonFill              = src.bIsPolygonFill        ;
		bOrthogonalMode             = src.bOrthogonalMode       ;
		bIsViewingMode              = src.bIsViewingMode        ;
		bIsDynamicViewMode          = src.bIsDynamicViewMode    ;
		VanishingPointInVcs         = src.VanishingPointInVcs   ;
		NavigatorPosInVcs           = src.NavigatorPosInVcs     ;
		ViewSite                    = src.ViewSite              ;
		ViewPoint                   = src.ViewPoint             ;
		ViewUpVectorPoint           = src.ViewUpVectorPoint     ;
		ViewDirectionVector         = src.ViewDirectionVector   ;
		ViewUpVector                = src.ViewUpVector          ;
		PerspectiveRatio            = src.PerspectiveRatio      ;
		PerspectiveRatioPercentage  = src.PerspectiveRatioPercentage;
		GRectViewPortOnClientWindow = src.GRectViewPortOnClientWindow;
		GRectViewPortOnViewPort     = src.GRectViewPortOnViewPort    ;
		GRectScreenOnClientWindow   = src.GRectScreenOnClientWindow  ;
		GRectWindow                 = src.GRectWindow                ;
		GRectWindowForZoomAll       = src.GRectWindowForZoomAll      ;
		GRectViewPort               = src.GRectViewPort              ;
		GRectScreen                 = src.GRectScreen                ;
		nVCLM                       = src.nVCLM                      ;
		nVCRM                       = src.nVCRM                      ;
		nVCTM                       = src.nVCTM                      ;
		nVCBM                       = src.nVCBM                      ;
		crMarginFillColor           = src.crMarginFillColor          ;
		crMarginOutLineColor        = src.crMarginOutLineColor       ;
		ZoomAllMargin               = src.ZoomAllMargin              ;
	  bIsShade                    = src.bIsShade                   ;

		return *this;
	}
};

struct T_GUcsDefineData_D
{
	int nUcsDefineType;
	T_GPoint3D_D  Origin;
	T_GPoint3D_D  XDir;
	T_GPoint3D_D  YDir;
	double  dXRot;
	double  dYRot;
	double  dZRot;
	double  dMinXBoundary;
	double  dMinYBoundary;
	double  dMaxXBoundary;
	double  dMaxYBoundary;
	BOOL    bIsByWcs;
	BOOL    bIsChangeViewDir;
	BOOL    bIsExistUcsBoundary;
	BOOL    bIsActiveOnThePlane;
	BOOL    bIsNamedPlane;
	double  dPlaneMinusWidth;
	double  dPlanePlusWidth;
	
	void Initialize()
	{
		nUcsDefineType = 100;
		Origin.Initialize();
		XDir  .Initialize();
		YDir  .Initialize();
		dXRot =0.;
		dYRot =0.;
		dZRot =0.;
		dMinXBoundary = -100.;
		dMinYBoundary = -100.;
		dMaxXBoundary =  100.;
		dMaxYBoundary =  100.;
		bIsByWcs            = TRUE          ;
		bIsChangeViewDir    = FALSE         ;
		bIsExistUcsBoundary = FALSE         ;
		bIsActiveOnThePlane = FALSE         ;
		bIsNamedPlane       = FALSE         ;
		dPlaneMinusWidth    = 0.;
		dPlanePlusWidth     = 0.;
	}
	T_GUcsDefineData_D() { Initialize(); }
	T_GUcsDefineData_D(const T_GUcsDefineData_D &src) { *this = src; }
};
struct T_PRUD_D_790
{
	double    dViewPoint[3]   ;  // ViewPoint x,y,z
	CString   strActiveElem   ;  // Active Element List
	CString   strActiveNode   ;  // Active Node List
	BOOL      bShrink         ;  // Shrink
	BOOL      bHidden         ;  // Hidden
	BOOL      bPerspective    ;  // Perspective
	T_DPST_D_810  DispOpt         ;  // Current Label (Display Option)
	CArray<UINT, UINT> aDimmK ;  // Dimension
	int       nActiveKind     ;  // Active Kind : 0:ALL, 1:Active Iden, 2:Grup, 3:Active UCS Plane
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD; // Active Identity
	//UINT      nGrupK          ; // Group Key
	CString   strGrupName; // Group Name (Name으로 비교, Name이 같으면 같은 group으로 간주함)
	T_RETextMod_D RETextD     ;
	int       nUcsGcsMode     ;  // 0: UCS, 1: GCS
	T_GUcsDefineData_D UcsDefineD; 

	void Initialize()
	{
		dViewPoint[0]    = 0.;
		dViewPoint[1]    = 0.;
		dViewPoint[2]    = 0.;
		strActiveElem    = _T("");
		strActiveNode    = _T("");
		bShrink          = FALSE;
		bHidden          = FALSE;
		bPerspective     = FALSE;
		DispOpt         .Initialize();
		aDimmK          .RemoveAll();
		nActiveKind      = 0;
		aActiveIdtD      .RemoveAll();
		strGrupName      = _T("");
		RETextD         .Initialize();
		nUcsGcsMode      = 1;
		UcsDefineD      .Initialize();
	}
	T_PRUD_D_790() { Initialize(); }
	T_PRUD_D_790(const T_PRUD_D_790 &src) { *this = src; }
	T_PRUD_D_790& operator=(const T_PRUD_D_790 &src)
	{
		memcpy(dViewPoint, src.dViewPoint, sizeof(dViewPoint));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt         = src.DispOpt         ;
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName   ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		return *this;
	}
};
// v810 -> v811 : DispOpt > Dsgn > bDpstDsgn의 배열 갯수가 변경됨.
struct T_PRUD_D_810
{
	double    dViewPoint[3]   ;  // ViewPoint x,y,z
	CString   strActiveElem   ;  // Active Element List
	CString   strActiveNode   ;  // Active Node List
	BOOL      bShrink         ;  // Shrink
	BOOL      bHidden         ;  // Hidden
	BOOL      bPerspective    ;  // Perspective
	T_DPST_D_810  DispOpt         ;  // Current Label (Display Option)
	CArray<UINT, UINT> aDimmK ;  // Dimension
	int       nActiveKind     ;  // Active Kind : 0:ALL, 1:Active Iden, 2:Grup, 3:Active UCS Plane
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD; // Active Identity
	//UINT      nGrupK          ; // Group Key
	CString   strGrupName; // Group Name (Name으로 비교, Name이 같으면 같은 group으로 간주함)
	T_RETextMod_D RETextD     ;
	int       nUcsGcsMode     ;  // 0: UCS, 1: GCS
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB    ; // 791에 추가. 20110418 KIMJM, (Import Reference DB로 됐을때 TRUE, Update to Current View 시에는 FALSE)
	
	void Initialize()
	{
		dViewPoint[0]    = 0.;
		dViewPoint[1]    = 0.;
		dViewPoint[2]    = 0.;
		strActiveElem    = _T("");
		strActiveNode    = _T("");
		bShrink          = FALSE;
		bHidden          = FALSE;
		bPerspective     = FALSE;
		DispOpt         .Initialize();
		aDimmK          .RemoveAll();
		nActiveKind      = 0;
		aActiveIdtD      .RemoveAll();
		strGrupName      = _T("");
		RETextD         .Initialize();
		nUcsGcsMode      = 1;
		UcsDefineD      .Initialize();
		bReferenceDB    = FALSE;
	}
	T_PRUD_D_810() { Initialize(); }
	T_PRUD_D_810(const T_PRUD_D_810 &src) { *this = src; }
	T_PRUD_D_810& operator=(const T_PRUD_D_810 &src)
	{
		memcpy(dViewPoint, src.dViewPoint, sizeof(dViewPoint));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt         = src.DispOpt         ;
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName   ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		bReferenceDB    = src.bReferenceDB;
		return *this;
	}
};
struct T_PRUD_D_821
{
	double    dViewPoint[3]   ;  // ViewPoint x,y,z
	CString   strActiveElem   ;  // Active Element List
	CString   strActiveNode   ;  // Active Node List
	BOOL      bShrink         ;  // Shrink
	BOOL      bHidden         ;  // Hidden
	BOOL      bPerspective    ;  // Perspective
	T_DPST_D_821  DispOpt         ;  // Current Label (Display Option)
	CArray<UINT, UINT> aDimmK ;  // Dimension
	int       nActiveKind     ;  // Active Kind : 0:ALL, 1:Active Iden, 2:Grup, 3:Active UCS Plane
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD; // Active Identity
	//UINT      nGrupK          ; // Group Key
	CString   strGrupName; // Group Name (Name으로 비교, Name이 같으면 같은 group으로 간주함)
	T_RETextMod_D RETextD     ;
	int       nUcsGcsMode     ;  // 0: UCS, 1: GCS
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB    ; // 791에 추가. 20110418 KIMJM, (Import Reference DB로 됐을때 TRUE, Update to Current View 시에는 FALSE)
	
	void Initialize()
	{
		dViewPoint[0]    = 0.;
		dViewPoint[1]    = 0.;
		dViewPoint[2]    = 0.;
		strActiveElem    = _T("");
		strActiveNode    = _T("");
		bShrink          = FALSE;
		bHidden          = FALSE;
		bPerspective     = FALSE;
		DispOpt         .Initialize();
		aDimmK          .RemoveAll();
		nActiveKind      = 0;
		aActiveIdtD      .RemoveAll();
		strGrupName      = _T("");
		RETextD         .Initialize();
		nUcsGcsMode      = 1;
		UcsDefineD      .Initialize();
		bReferenceDB    = FALSE;
	}
	T_PRUD_D_821() { Initialize(); }
	T_PRUD_D_821(const T_PRUD_D_821 &src) { *this = src; }
	T_PRUD_D_821& operator=(const T_PRUD_D_821 &src)
	{
		memcpy(dViewPoint, src.dViewPoint, sizeof(dViewPoint));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt         = src.DispOpt         ;
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName   ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		bReferenceDB    = src.bReferenceDB;
		return *this;
	}
};
struct T_PRUD_D_835
{
	double    dViewPoint[3]   ;  // ViewPoint x,y,z
	CString   strActiveElem   ;  // Active Element List
	CString   strActiveNode   ;  // Active Node List
	BOOL      bShrink         ;  // Shrink
	BOOL      bHidden         ;  // Hidden
	BOOL      bPerspective    ;  // Perspective
	T_DPST_D_835  DispOpt         ;  // Current Label (Display Option)
	CArray<UINT, UINT> aDimmK ;  // Dimension
	int       nActiveKind     ;  // Active Kind : 0:ALL, 1:Active Iden, 2:Grup, 3:Active UCS Plane
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD; // Active Identity
	//UINT      nGrupK          ; // Group Key
	CString   strGrupName; // Group Name (Name으로 비교, Name이 같으면 같은 group으로 간주함)
	T_RETextMod_D RETextD     ;
	int       nUcsGcsMode     ;  // 0: UCS, 1: GCS
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB    ; // 791에 추가. 20110418 KIMJM, (Import Reference DB로 됐을때 TRUE, Update to Current View 시에는 FALSE)

	void Initialize()
	{
		dViewPoint[0]    = 0.;
		dViewPoint[1]    = 0.;
		dViewPoint[2]    = 0.;
		strActiveElem    = _T("");
		strActiveNode    = _T("");
		bShrink          = FALSE;
		bHidden          = FALSE;
		bPerspective     = FALSE;
		DispOpt         .Initialize();
		aDimmK          .RemoveAll();
		nActiveKind      = 0;
		aActiveIdtD     .RemoveAll();
		strGrupName      = _T("");
		RETextD         .Initialize();
		nUcsGcsMode      = 1;
		UcsDefineD      .Initialize();
		bReferenceDB    = FALSE;
	}
	T_PRUD_D_835() { Initialize(); }
	T_PRUD_D_835(const T_PRUD_D_835 &src) { *this = src; }
	T_PRUD_D_835& operator=(const T_PRUD_D_835 &src)
	{
		memcpy(dViewPoint, src.dViewPoint, sizeof(dViewPoint));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt         = src.DispOpt         ;
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName   ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		bReferenceDB    = src.bReferenceDB;
		return *this;
	}
};
struct T_PRUD_D_841
{
	double    dViewPoint[3]   ;  // ViewPoint x,y,z
	CString   strActiveElem   ;  // Active Element List
	CString   strActiveNode   ;  // Active Node List
	BOOL      bShrink         ;  // Shrink
	BOOL      bHidden         ;  // Hidden
	BOOL      bPerspective    ;  // Perspective
	T_DPST_D_841  DispOpt         ;  // Current Label (Display Option)
	CArray<UINT, UINT> aDimmK ;  // Dimension
	int       nActiveKind     ;  // Active Kind : 0:ALL, 1:Active Iden, 2:Grup, 3:Active UCS Plane
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD; // Active Identity
	//UINT      nGrupK          ; // Group Key
	CString   strGrupName; // Group Name (Name으로 비교, Name이 같으면 같은 group으로 간주함)
	T_RETextMod_D RETextD     ;
	int       nUcsGcsMode     ;  // 0: UCS, 1: GCS
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB    ; // 791에 추가. 20110418 KIMJM, (Import Reference DB로 됐을때 TRUE, Update to Current View 시에는 FALSE)

	void Initialize()
	{
		dViewPoint[0]    = 0.;
		dViewPoint[1]    = 0.;
		dViewPoint[2]    = 0.;
		strActiveElem    = _T("");
		strActiveNode    = _T("");
		bShrink          = FALSE;
		bHidden          = FALSE;
		bPerspective     = FALSE;
		DispOpt         .Initialize();
		aDimmK          .RemoveAll();
		nActiveKind      = 0;
		aActiveIdtD      .RemoveAll();
		strGrupName      = _T("");
		RETextD         .Initialize();
		nUcsGcsMode      = 1;
		UcsDefineD      .Initialize();
		bReferenceDB    = FALSE;
	}
	T_PRUD_D_841() { Initialize(); }
	T_PRUD_D_841(const T_PRUD_D_841 &src) { *this = src; }
	T_PRUD_D_841& operator=(const T_PRUD_D_841 &src)
	{
		memcpy(dViewPoint, src.dViewPoint, sizeof(dViewPoint));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt         = src.DispOpt         ;
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName   ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		bReferenceDB    = src.bReferenceDB;
		return *this;
	}
};

struct T_PRUD_D_845
{
	double VCSVector[3][3];
	CString   strActiveElem   ;  // Active Element List
	CString   strActiveNode   ;  // Active Node List
	BOOL      bShrink         ;  // Shrink
	BOOL      bHidden         ;  // Hidden
	BOOL      bPerspective    ;  // Perspective
	T_DPST_D_855  DispOpt         ;  // Current Label (Display Option)
	CArray<UINT, UINT> aDimmK ;  // Dimension
	int       nActiveKind     ;  // Active Kind : 0:ALL, 1:Active Iden, 2:Grup, 3:Active UCS Plane
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD; // Active Identity
	//UINT      nGrupK          ; // Group Key
	CString   strGrupName; // Group Name (Name으로 비교, Name이 같으면 같은 group으로 간주함)
	T_RETextMod_D RETextD     ;
	int       nUcsGcsMode     ;  // 0: UCS, 1: GCS
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB    ; // 791에 추가. 20110418 KIMJM, (Import Reference DB로 됐을때 TRUE, Update to Current View 시에는 FALSE)

	void Initialize()
	{
		memset(VCSVector,0,sizeof(double)*9);
		strActiveElem    = _T("");
		strActiveNode    = _T("");
		bShrink          = FALSE;
		bHidden          = FALSE;
		bPerspective     = FALSE;
		DispOpt         .Initialize();
		aDimmK          .RemoveAll();
		nActiveKind      = 0;
		aActiveIdtD      .RemoveAll();
		strGrupName      = _T("");
		RETextD         .Initialize();
		nUcsGcsMode      = 1;
		UcsDefineD      .Initialize();
		bReferenceDB    = FALSE;
	}
	T_PRUD_D_845() { Initialize(); }
	T_PRUD_D_845(const T_PRUD_D_845 &src) { *this = src; }
	T_PRUD_D_845& operator=(const T_PRUD_D_845 &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt         = src.DispOpt         ;
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName   ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		bReferenceDB    = src.bReferenceDB;
		return *this;
	}
};

struct T_PRUD_D_855
{
	double VCSVector[3][3];
	CString   strActiveElem   ;  // Active Element List
	CString   strActiveNode   ;  // Active Node List
	BOOL      bShrink         ;  // Shrink
	BOOL      bHidden         ;  // Hidden
	BOOL      bPerspective    ;  // Perspective
	T_DPST_D_855  DispOpt         ;  // Current Label (Display Option)
	CArray<UINT, UINT> aDimmK ;  // Dimension
	int       nActiveKind     ;  // Active Kind : 0:ALL, 1:Active Iden, 2:Grup, 3:Active UCS Plane
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD; // Active Identity
	//UINT      nGrupK          ; // Group Key
	CString   strGrupName; // Group Name (Name으로 비교, Name이 같으면 같은 group으로 간주함)
	T_RETextMod_D RETextD     ;
	int       nUcsGcsMode     ;  // 0: UCS, 1: GCS
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB    ; // 791에 추가. 20110418 KIMJM, (Import Reference DB로 됐을때 TRUE, Update to Current View 시에는 FALSE)
	int       nUnitLength     ;   // 850에 추가. 저장당시 Length Unit Index, 단위변환 용도가 아니라 단위변경이 됐는지 체크하기 위한 목적

	void Initialize()
	{
		memset(VCSVector,0,sizeof(double)*9);
		strActiveElem    = _T("");
		strActiveNode    = _T("");
		bShrink          = FALSE;
		bHidden          = FALSE;
		bPerspective     = FALSE;
		DispOpt         .Initialize();
		aDimmK          .RemoveAll();
		nActiveKind      = 0;
		aActiveIdtD      .RemoveAll();
		strGrupName      = _T("");
		RETextD         .Initialize();
		nUcsGcsMode      = 1;
		UcsDefineD      .Initialize();
		bReferenceDB    = FALSE;
		nUnitLength     = -1;
	}
	T_PRUD_D_855() { Initialize(); }
	T_PRUD_D_855(const T_PRUD_D_855 &src) { *this = src; }
	T_PRUD_D_855& operator=(const T_PRUD_D_855 &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt         = src.DispOpt         ;
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName   ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		bReferenceDB    = src.bReferenceDB;
		nUnitLength     = src.nUnitLength;
		return *this;
	}
};

struct T_PRUD_D_860
{
	double VCSVector[3][3];
	CString   strActiveElem   ;  // Active Element List
	CString   strActiveNode   ;  // Active Node List
	BOOL      bShrink         ;  // Shrink
	BOOL      bHidden         ;  // Hidden
	BOOL      bPerspective    ;  // Perspective
	T_DPST_D_860  DispOpt         ;  // Current Label (Display Option)
	CArray<UINT, UINT> aDimmK ;  // Dimension
	int       nActiveKind     ;  // Active Kind : 0:ALL, 1:Active Iden, 2:Grup, 3:Active UCS Plane
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD; // Active Identity
	//UINT      nGrupK          ; // Group Key
	CString   strGrupName; // Group Name (Name으로 비교, Name이 같으면 같은 group으로 간주함)
	T_RETextMod_D RETextD     ;
	int       nUcsGcsMode     ;  // 0: UCS, 1: GCS
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB    ; // 791에 추가. 20110418 KIMJM, (Import Reference DB로 됐을때 TRUE, Update to Current View 시에는 FALSE)
	int       nUnitLength     ;   // 850에 추가. 저장당시 Length Unit Index, 단위변환 용도가 아니라 단위변경이 됐는지 체크하기 위한 목적

	void Initialize()
	{
		memset(VCSVector,0,sizeof(double)*9);
		strActiveElem    = _T("");
		strActiveNode    = _T("");
		bShrink          = FALSE;
		bHidden          = FALSE;
		bPerspective     = FALSE;
		DispOpt         .Initialize();
		aDimmK          .RemoveAll();
		nActiveKind      = 0;
		aActiveIdtD      .RemoveAll();
		strGrupName      = _T("");
		RETextD         .Initialize();
		nUcsGcsMode      = 1;
		UcsDefineD      .Initialize();
		bReferenceDB    = FALSE;
		nUnitLength     = -1;
	}
	T_PRUD_D_860() { Initialize(); }
	T_PRUD_D_860(const T_PRUD_D_860 &src) { *this = src; }
	T_PRUD_D_860& operator=(const T_PRUD_D_860 &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt         = src.DispOpt         ;
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName   ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		bReferenceDB    = src.bReferenceDB;
		nUnitLength     = src.nUnitLength;
		return *this;
	}
};

struct T_PRUD_D_865
{
	double VCSVector[3][3];
	CString   strActiveElem   ;  // Active Element List
	CString   strActiveNode   ;  // Active Node List
	BOOL      bShrink         ;  // Shrink
	BOOL      bHidden         ;  // Hidden
	BOOL      bPerspective    ;  // Perspective
	T_DPST_D_865  DispOpt         ;  // Current Label (Display Option)
	CArray<UINT, UINT> aDimmK ;  // Dimension
	int       nActiveKind     ;  // Active Kind : 0:ALL, 1:Active Iden, 2:Grup, 3:Active UCS Plane
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD; // Active Identity
	//UINT      nGrupK          ; // Group Key
	CString   strGrupName; // Group Name (Name으로 비교, Name이 같으면 같은 group으로 간주함)
	T_RETextMod_D RETextD     ;
	int       nUcsGcsMode     ;  // 0: UCS, 1: GCS
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB    ; // 791에 추가. 20110418 KIMJM, (Import Reference DB로 됐을때 TRUE, Update to Current View 시에는 FALSE)
	int       nUnitLength     ;   // 850에 추가. 저장당시 Length Unit Index, 단위변환 용도가 아니라 단위변경이 됐는지 체크하기 위한 목적

	void Initialize()
	{
		memset(VCSVector,0,sizeof(double)*9);
		strActiveElem    = _T("");
		strActiveNode    = _T("");
		bShrink          = FALSE;
		bHidden          = FALSE;
		bPerspective     = FALSE;
		DispOpt         .Initialize();
		aDimmK          .RemoveAll();
		nActiveKind      = 0;
		aActiveIdtD      .RemoveAll();
		strGrupName      = _T("");
		RETextD         .Initialize();
		nUcsGcsMode      = 1;
		UcsDefineD      .Initialize();
		bReferenceDB    = FALSE;
		nUnitLength     = -1;
	}
	T_PRUD_D_865() { Initialize(); }
	T_PRUD_D_865(const T_PRUD_D_865 &src) { *this = src; }
	T_PRUD_D_865& operator=(const T_PRUD_D_865 &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt         = src.DispOpt         ;
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName   ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		bReferenceDB    = src.bReferenceDB;
		nUnitLength     = src.nUnitLength;
		return *this;
	}
};
struct T_PRUD_D_870
{
	double VCSVector[3][3];
	CString   strActiveElem   ;  // Active Element List
	CString   strActiveNode   ;  // Active Node List
	BOOL      bShrink         ;  // Shrink
	BOOL      bHidden         ;  // Hidden
	BOOL      bPerspective    ;  // Perspective
	T_DPST_D_870  DispOpt         ;  // Current Label (Display Option)
	CArray<UINT, UINT> aDimmK ;  // Dimension
	int       nActiveKind     ;  // Active Kind : 0:ALL, 1:Active Iden, 2:Grup, 3:Active UCS Plane
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD; // Active Identity
	//UINT      nGrupK          ; // Group Key
	CString   strGrupName; // Group Name (Name으로 비교, Name이 같으면 같은 group으로 간주함)
	T_RETextMod_D RETextD     ;
	int       nUcsGcsMode     ;  // 0: UCS, 1: GCS
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB    ; // 791에 추가. 20110418 KIMJM, (Import Reference DB로 됐을때 TRUE, Update to Current View 시에는 FALSE)
	int       nUnitLength     ;   // 850에 추가. 저장당시 Length Unit Index, 단위변환 용도가 아니라 단위변경이 됐는지 체크하기 위한 목적

	void Initialize()
	{
		memset(VCSVector,0,sizeof(double)*9);
		strActiveElem    = _T("");
		strActiveNode    = _T("");
		bShrink          = FALSE;
		bHidden          = FALSE;
		bPerspective     = FALSE;
		DispOpt         .Initialize();
		aDimmK          .RemoveAll();
		nActiveKind      = 0;
		aActiveIdtD      .RemoveAll();
		strGrupName      = _T("");
		RETextD         .Initialize();
		nUcsGcsMode      = 1;
		UcsDefineD      .Initialize();
		bReferenceDB    = FALSE;
		nUnitLength     = -1;
	}
	T_PRUD_D_870() { Initialize(); }
	T_PRUD_D_870(const T_PRUD_D_870 &src) { *this = src; }
	T_PRUD_D_870& operator=(const T_PRUD_D_870 &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt         = src.DispOpt         ;
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName   ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		bReferenceDB    = src.bReferenceDB;
		nUnitLength     = src.nUnitLength;
		return *this;
	}
};

struct T_PRUD_D_883
{
	double VCSVector[3][3];
	CString   strActiveElem   ;  // Active Element List
	CString   strActiveNode   ;  // Active Node List
	BOOL      bShrink         ;  // Shrink
	BOOL      bHidden         ;  // Hidden
	BOOL      bPerspective    ;  // Perspective
	T_DPST_D_883  DispOpt         ;  // Current Label (Display Option)
	CArray<UINT, UINT> aDimmK ;  // Dimension
	int       nActiveKind     ;  // Active Kind : 0:ALL, 1:Active Iden, 2:Grup, 3:Active UCS Plane
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD; // Active Identity
	//UINT      nGrupK          ; // Group Key
	CString   strGrupName; // Group Name (Name으로 비교, Name이 같으면 같은 group으로 간주함)
	T_RETextMod_D RETextD     ;
	int       nUcsGcsMode     ;  // 0: UCS, 1: GCS
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB    ; // 791에 추가. 20110418 KIMJM, (Import Reference DB로 됐을때 TRUE, Update to Current View 시에는 FALSE)
	int       nUnitLength     ;   // 850에 추가. 저장당시 Length Unit Index, 단위변환 용도가 아니라 단위변경이 됐는지 체크하기 위한 목적

	void Initialize()
	{
		memset(VCSVector,0,sizeof(double)*9);
		strActiveElem    = _T("");
		strActiveNode    = _T("");
		bShrink          = FALSE;
		bHidden          = FALSE;
		bPerspective     = FALSE;
		DispOpt         .Initialize();
		aDimmK          .RemoveAll();
		nActiveKind      = 0;
		aActiveIdtD      .RemoveAll();
		strGrupName      = _T("");
		RETextD         .Initialize();
		nUcsGcsMode      = 1;
		UcsDefineD      .Initialize();
		bReferenceDB    = FALSE;
		nUnitLength     = -1;
	}
	T_PRUD_D_883() { Initialize(); }
	T_PRUD_D_883(const T_PRUD_D_883 &src) { *this = src; }
	T_PRUD_D_883& operator=(const T_PRUD_D_883 &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt         = src.DispOpt         ;
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName   ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		bReferenceDB    = src.bReferenceDB;
		nUnitLength     = src.nUnitLength;
		return *this;
	}
};

struct T_PRUD_D_885
{
	double    VCSVector[3][3];
	CString   strActiveElem   ;  // Active Element List
	CString   strActiveNode   ;  // Active Node List
	BOOL      bShrink         ;  // Shrink
	BOOL      bHidden         ;  // Hidden
	BOOL      bPerspective    ;  // Perspective
	T_DPST_D_885  DispOpt         ;  // Current Label (Display Option)
	CArray<UINT, UINT> aDimmK ;  // Dimension
	int       nActiveKind     ;  // Active Kind : 0:ALL, 1:Active Iden, 2:Grup, 3:Active UCS Plane
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD; // Active Identity
	//UINT      nGrupK          ; // Group Key
	CString   strGrupName; // Group Name (Name으로 비교, Name이 같으면 같은 group으로 간주함)
	T_RETextMod_D RETextD     ;
	int       nUcsGcsMode     ;  // 0: UCS, 1: GCS
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB    ;   // 791에 추가. 20110418 KIMJM, (Import Reference DB로 됐을때 TRUE, Update to Current View 시에는 FALSE)
	int       nUnitLength     ;   // 850에 추가. 저장당시 Length Unit Index, 단위변환 용도가 아니라 단위변경이 됐는지 체크하기 위한 목적

	void Initialize()
	{
		memset(VCSVector,0,sizeof(double)*9);
		strActiveElem    = _T("");
		strActiveNode    = _T("");
		bShrink          = FALSE;
		bHidden          = FALSE;
		bPerspective     = FALSE;
		DispOpt         .Initialize();
		aDimmK          .RemoveAll();
		nActiveKind      = 0;
		aActiveIdtD      .RemoveAll();
		strGrupName      = _T("");
		RETextD         .Initialize();
		nUcsGcsMode      = 1;
		UcsDefineD      .Initialize();
		bReferenceDB    = FALSE;
		nUnitLength     = -1;
	}
	T_PRUD_D_885() { Initialize(); }
	T_PRUD_D_885(const T_PRUD_D_885 &src) { *this = src; }
	T_PRUD_D_885& operator=(const T_PRUD_D_885 &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt         = src.DispOpt         ;
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName   ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		bReferenceDB    = src.bReferenceDB;
		nUnitLength     = src.nUnitLength;

		return *this;
	}
};

struct T_PRUD_D_891
{
	double    VCSVector[3][3];
	CString   strActiveElem   ;  // Active Element List
	CString   strActiveNode   ;  // Active Node List
	BOOL      bShrink         ;  // Shrink
	BOOL      bHidden         ;  // Hidden
	BOOL      bPerspective    ;  // Perspective
	T_DPST_D_891  DispOpt         ;  // Current Label (Display Option)
	CArray<UINT, UINT> aDimmK ;  // Dimension
	int       nActiveKind     ;  // Active Kind : 0:ALL, 1:Active Iden, 2:Grup, 3:Active UCS Plane
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD; // Active Identity
	//UINT      nGrupK          ; // Group Key
	CString   strGrupName; // Group Name (Name으로 비교, Name이 같으면 같은 group으로 간주함)
	T_RETextMod_D RETextD     ;
	int       nUcsGcsMode     ;  // 0: UCS, 1: GCS
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB    ;   // 791에 추가. 20110418 KIMJM, (Import Reference DB로 됐을때 TRUE, Update to Current View 시에는 FALSE)
	int       nUnitLength     ;   // 850에 추가. 저장당시 Length Unit Index, 단위변환 용도가 아니라 단위변경이 됐는지 체크하기 위한 목적

	void Initialize()
	{
		memset(VCSVector,0,sizeof(double)*9);
		strActiveElem    = _T("");
		strActiveNode    = _T("");
		bShrink          = FALSE;
		bHidden          = FALSE;
		bPerspective     = FALSE;
		DispOpt         .Initialize();
		aDimmK          .RemoveAll();
		nActiveKind      = 0;
		aActiveIdtD      .RemoveAll();
		strGrupName      = _T("");
		RETextD         .Initialize();
		nUcsGcsMode      = 1;
		UcsDefineD      .Initialize();
		bReferenceDB    = FALSE;
		nUnitLength     = -1;
	}
	T_PRUD_D_891() { Initialize(); }
	T_PRUD_D_891(const T_PRUD_D_891 &src) { *this = src; }
	T_PRUD_D_891& operator=(const T_PRUD_D_891 &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt         = src.DispOpt         ;
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName   ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		bReferenceDB    = src.bReferenceDB;
		nUnitLength     = src.nUnitLength;

		return *this;
	}
};

struct T_PRUD_D_895
{
	double    VCSVector[3][3];
	CString   strActiveElem;  // Active Element List
	CString   strActiveNode;  // Active Node List
	BOOL      bShrink;  // Shrink
	BOOL      bHidden;  // Hidden
	BOOL      bPerspective;  // Perspective
	T_DPST_D_895  DispOpt;  // Current Label (Display Option)
	CArray<UINT, UINT> aDimmK;  // Dimension
	int       nActiveKind;  // Active Kind : 0:ALL, 1:Active Iden, 2:Grup, 3:Active UCS Plane
	CArray<T_ACTIVE_IDENTITY_DB, T_ACTIVE_IDENTITY_DB&> aActiveIdtD; // Active Identity
	//UINT      nGrupK          ; // Group Key
	CString   strGrupName; // Group Name (Name으로 비교, Name이 같으면 같은 group으로 간주함)
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;  // 0: UCS, 1: GCS
	T_GUcsDefineData_D UcsDefineD;
	BOOL      bReferenceDB;   // 791에 추가. 20110418 KIMJM, (Import Reference DB로 됐을때 TRUE, Update to Current View 시에는 FALSE)
	int       nUnitLength;   // 850에 추가. 저장당시 Length Unit Index, 단위변환 용도가 아니라 단위변경이 됐는지 체크하기 위한 목적

	void Initialize()
	{
		memset(VCSVector, 0, sizeof(double) * 9);
		strActiveElem = _T("");
		strActiveNode = _T("");
		bShrink = FALSE;
		bHidden = FALSE;
		bPerspective = FALSE;
		DispOpt.Initialize();
		aDimmK.RemoveAll();
		nActiveKind = 0;
		aActiveIdtD.RemoveAll();
		strGrupName = _T("");
		RETextD.Initialize();
		nUcsGcsMode = 1;
		UcsDefineD.Initialize();
		bReferenceDB = FALSE;
		nUnitLength = -1;
	}
	T_PRUD_D_895() { Initialize(); }
	T_PRUD_D_895(const T_PRUD_D_895 &src) { *this = src; }
	T_PRUD_D_895& operator=(const T_PRUD_D_895 &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem = src.strActiveElem;
		strActiveNode = src.strActiveNode;
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt = src.DispOpt;
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		strGrupName = src.strGrupName;
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;

		return *this;
	}
};
struct T_PRUD_D_925
{
	double    VCSVector[3][3];
	CString   strActiveElem;  // Active Element List
	CString   strActiveNode;  // Active Node List
	BOOL      bShrink;  // Shrink
	BOOL      bHidden;  // Hidden
	BOOL      bPerspective;  // Perspective
	T_DPST_D_925  DispOpt;  // Current Label (Display Option)
	CArray<UINT, UINT> aDimmK;  // Dimension
	int       nActiveKind;  // Active Kind : 0:ALL, 1:Active Iden, 2:Grup, 3:Active UCS Plane
	CArray<T_ACTIVE_IDENTITY_DB, T_ACTIVE_IDENTITY_DB&> aActiveIdtD; // Active Identity
	//UINT      nGrupK          ; // Group Key
	CString   strGrupName; // Group Name (Name으로 비교, Name이 같으면 같은 group으로 간주함)
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;  // 0: UCS, 1: GCS
	T_GUcsDefineData_D UcsDefineD;
	BOOL      bReferenceDB;   // 791에 추가. 20110418 KIMJM, (Import Reference DB로 됐을때 TRUE, Update to Current View 시에는 FALSE)
	int       nUnitLength;   // 850에 추가. 저장당시 Length Unit Index, 단위변환 용도가 아니라 단위변경이 됐는지 체크하기 위한 목적

	void Initialize()
	{
		memset(VCSVector, 0, sizeof(double) * 9);
		strActiveElem = _T("");
		strActiveNode = _T("");
		bShrink = FALSE;
		bHidden = FALSE;
		bPerspective = FALSE;
		DispOpt.Initialize();
		aDimmK.RemoveAll();
		nActiveKind = 0;
		aActiveIdtD.RemoveAll();
		strGrupName = _T("");
		RETextD.Initialize();
		nUcsGcsMode = 1;
		UcsDefineD.Initialize();
		bReferenceDB = FALSE;
		nUnitLength = -1;
	}
	T_PRUD_D_925() { Initialize(); }
	T_PRUD_D_925(const T_PRUD_D_925 &src) { *this = src; }
	T_PRUD_D_925& operator=(const T_PRUD_D_925 &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem = src.strActiveElem;
		strActiveNode = src.strActiveNode;
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt = src.DispOpt;
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		strGrupName = src.strGrupName;
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;

		return *this;
	}
};
struct T_PRUD_D_930
{
	double    VCSVector[3][3];
	CString   strActiveElem;  // Active Element List
	CString   strActiveNode;  // Active Node List
	BOOL      bShrink;  // Shrink
	BOOL      bHidden;  // Hidden
	BOOL      bPerspective;  // Perspective
	T_DPST_D_930  DispOpt;  // Current Label (Display Option)
	CArray<UINT, UINT> aDimmK;  // Dimension
	int       nActiveKind;  // Active Kind : 0:ALL, 1:Active Iden, 2:Grup, 3:Active UCS Plane
	CArray<T_ACTIVE_IDENTITY_DB, T_ACTIVE_IDENTITY_DB&> aActiveIdtD; // Active Identity
	//UINT      nGrupK          ; // Group Key
	CString   strGrupName; // Group Name (Name으로 비교, Name이 같으면 같은 group으로 간주함)
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;  // 0: UCS, 1: GCS
	T_GUcsDefineData_D UcsDefineD;
	BOOL      bReferenceDB;   // 791에 추가. 20110418 KIMJM, (Import Reference DB로 됐을때 TRUE, Update to Current View 시에는 FALSE)
	int       nUnitLength;   // 850에 추가. 저장당시 Length Unit Index, 단위변환 용도가 아니라 단위변경이 됐는지 체크하기 위한 목적

	void Initialize()
	{
		memset(VCSVector, 0, sizeof(double) * 9);
		strActiveElem = _T("");
		strActiveNode = _T("");
		bShrink = FALSE;
		bHidden = FALSE;
		bPerspective = FALSE;
		DispOpt.Initialize();
		aDimmK.RemoveAll();
		nActiveKind = 0;
		aActiveIdtD.RemoveAll();
		strGrupName = _T("");
		RETextD.Initialize();
		nUcsGcsMode = 1;
		UcsDefineD.Initialize();
		bReferenceDB = FALSE;
		nUnitLength = -1;
	}
	T_PRUD_D_930() { Initialize(); }
	T_PRUD_D_930(const T_PRUD_D_930 &src) { *this = src; }
	T_PRUD_D_930& operator=(const T_PRUD_D_930 &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem = src.strActiveElem;
		strActiveNode = src.strActiveNode;
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt = src.DispOpt;
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		strGrupName = src.strGrupName;
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;

		return *this;
	}
};
struct T_PRUD_D_940
{
	double    VCSVector[3][3];
	CString   strActiveElem;  // Active Element List
	CString   strActiveNode;  // Active Node List
	BOOL      bShrink;  // Shrink
	BOOL      bHidden;  // Hidden
	BOOL      bPerspective;  // Perspective
	T_DPST_D_940  DispOpt;  // Current Label (Display Option)
	CArray<UINT, UINT> aDimmK;  // Dimension
	int       nActiveKind;  // Active Kind : 0:ALL, 1:Active Iden, 2:Grup, 3:Active UCS Plane
	CArray<T_ACTIVE_IDENTITY_DB, T_ACTIVE_IDENTITY_DB&> aActiveIdtD; // Active Identity
	//UINT      nGrupK          ; // Group Key
	CString   strGrupName; // Group Name (Name으로 비교, Name이 같으면 같은 group으로 간주함)
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;  // 0: UCS, 1: GCS
	T_GUcsDefineData_D UcsDefineD;
	BOOL      bReferenceDB;   // 791에 추가. 20110418 KIMJM, (Import Reference DB로 됐을때 TRUE, Update to Current View 시에는 FALSE)
	int       nUnitLength;   // 850에 추가. 저장당시 Length Unit Index, 단위변환 용도가 아니라 단위변경이 됐는지 체크하기 위한 목적

	void Initialize()
	{
		memset(VCSVector, 0, sizeof(double) * 9);
		strActiveElem = _T("");
		strActiveNode = _T("");
		bShrink = FALSE;
		bHidden = FALSE;
		bPerspective = FALSE;
		DispOpt.Initialize();
		aDimmK.RemoveAll();
		nActiveKind = 0;
		aActiveIdtD.RemoveAll();
		strGrupName = _T("");
		RETextD.Initialize();
		nUcsGcsMode = 1;
		UcsDefineD.Initialize();
		bReferenceDB = FALSE;
		nUnitLength = -1;
	}
	T_PRUD_D_940() { Initialize(); }
	T_PRUD_D_940(const T_PRUD_D_940 &src) { *this = src; }
	T_PRUD_D_940& operator=(const T_PRUD_D_940 &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem = src.strActiveElem;
		strActiveNode = src.strActiveNode;
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt = src.DispOpt;
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		strGrupName = src.strGrupName;
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;

		return *this;
	}
};

struct T_PRUD_D_950
{
	double    VCSVector[3][3];
	CString   strActiveElem;  // Active Element List
	CString   strActiveNode;  // Active Node List
	BOOL      bShrink;  // Shrink
	BOOL      bHidden;  // Hidden
	BOOL      bPerspective;  // Perspective
	T_DPST_D_950  DispOpt;  // Current Label (Display Option)
	CArray<UINT, UINT> aDimmK;  // Dimension
	int       nActiveKind;  // Active Kind : 0:ALL, 1:Active Iden, 2:Grup, 3:Active UCS Plane
	CArray<T_ACTIVE_IDENTITY_DB, T_ACTIVE_IDENTITY_DB&> aActiveIdtD; // Active Identity
	//UINT      nGrupK          ; // Group Key
	CString   strGrupName; // Group Name (Name으로 비교, Name이 같으면 같은 group으로 간주함)
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;  // 0: UCS, 1: GCS
	T_GUcsDefineData_D UcsDefineD;
	BOOL      bReferenceDB;   // 791에 추가. 20110418 KIMJM, (Import Reference DB로 됐을때 TRUE, Update to Current View 시에는 FALSE)
	int       nUnitLength;   // 850에 추가. 저장당시 Length Unit Index, 단위변환 용도가 아니라 단위변경이 됐는지 체크하기 위한 목적

	void Initialize()
	{
		memset(VCSVector, 0, sizeof(double) * 9);
		strActiveElem = _T("");
		strActiveNode = _T("");
		bShrink = FALSE;
		bHidden = FALSE;
		bPerspective = FALSE;
		DispOpt.Initialize();
		aDimmK.RemoveAll();
		nActiveKind = 0;
		aActiveIdtD.RemoveAll();
		strGrupName = _T("");
		RETextD.Initialize();
		nUcsGcsMode = 1;
		UcsDefineD.Initialize();
		bReferenceDB = FALSE;
		nUnitLength = -1;
	}
	T_PRUD_D_950() { Initialize(); }
	T_PRUD_D_950(const T_PRUD_D_950& src) { *this = src; }
	T_PRUD_D_950& operator=(const T_PRUD_D_950& src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem = src.strActiveElem;
		strActiveNode = src.strActiveNode;
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt = src.DispOpt;
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		strGrupName = src.strGrupName;
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;

		return *this;
	}
};

struct T_PRUD_D
{
	double    VCSVector[3][3];
	CString   strActiveElem   ;  // Active Element List
	CString   strActiveNode   ;  // Active Node List
	BOOL      bShrink         ;  // Shrink
	BOOL      bHidden         ;  // Hidden
	BOOL      bPerspective    ;  // Perspective
	T_DPST_D  DispOpt         ;  // Current Label (Display Option)
	CArray<UINT, UINT> aDimmK ;  // Dimension
	int       nActiveKind     ;  // Active Kind : 0:ALL, 1:Active Iden, 2:Grup, 3:Active UCS Plane
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD; // Active Identity
	//UINT      nGrupK          ; // Group Key
	CString   strGrupName; // Group Name (Name으로 비교, Name이 같으면 같은 group으로 간주함)
	T_RETextMod_D RETextD     ;
	int       nUcsGcsMode     ;  // 0: UCS, 1: GCS
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB    ;   // 791에 추가. 20110418 KIMJM, (Import Reference DB로 됐을때 TRUE, Update to Current View 시에는 FALSE)
	int       nUnitLength     ;   // 850에 추가. 저장당시 Length Unit Index, 단위변환 용도가 아니라 단위변경이 됐는지 체크하기 위한 목적

	void Initialize()
	{
		memset(VCSVector,0,sizeof(double)*9);
		strActiveElem    = _T("");
		strActiveNode    = _T("");
		bShrink          = FALSE;
		bHidden          = FALSE;
		bPerspective     = FALSE;
		DispOpt         .Initialize();
		aDimmK          .RemoveAll();
		nActiveKind      = 0;
		aActiveIdtD      .RemoveAll();
		strGrupName      = _T("");
		RETextD         .Initialize();
		nUcsGcsMode      = 1;
		UcsDefineD      .Initialize();
		bReferenceDB    = FALSE;
		nUnitLength     = -1;
	}
	T_PRUD_D() { Initialize(); }
	T_PRUD_D(const T_PRUD_D &src) { *this = src; }
	T_PRUD_D& operator=(const T_PRUD_D &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt         = src.DispOpt         ;
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName   ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		bReferenceDB    = src.bReferenceDB;
		nUnitLength     = src.nUnitLength;

		return *this;
	}
	void Convert790(T_PRUD_D_790& src)
	{
		memset(VCSVector,0,sizeof(VCSVector));
		memcpy(VCSVector[2],src.dViewPoint,sizeof(src.dViewPoint));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt.Convert810(src.DispOpt);
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName   ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		bReferenceDB    = FALSE;
	}
	void Convert810(T_PRUD_D_810& src)
	{
		memset(VCSVector,0,sizeof(VCSVector));
		memcpy(VCSVector[2],src.dViewPoint,sizeof(src.dViewPoint));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt.Convert810(src.DispOpt);
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName   ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		bReferenceDB    = FALSE;
	}
	void Convert821(T_PRUD_D_821& src)
	{
		memset(VCSVector,0,sizeof(VCSVector));
		memcpy(VCSVector[2],src.dViewPoint,sizeof(src.dViewPoint));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt.Convert821(src.DispOpt);
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName   ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		bReferenceDB    = FALSE;
	}
	void Convert835(T_PRUD_D_835& src)
	{
		memset(VCSVector,0,sizeof(VCSVector));
		memcpy(VCSVector[2],src.dViewPoint,sizeof(src.dViewPoint));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt.Convert835(src.DispOpt);
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName   ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		bReferenceDB    = FALSE;
	}
	void Convert841(T_PRUD_D_841& src)
	{
		memset(VCSVector,0,sizeof(VCSVector));
		memcpy(VCSVector[2],src.dViewPoint,sizeof(src.dViewPoint));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt.Convert841(src.DispOpt);
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind     ;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName     ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		bReferenceDB    = FALSE;
	}
	void Convert845(T_PRUD_D_845& src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt.Convert855(src.DispOpt);
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind     ;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName     ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		bReferenceDB    = src.bReferenceDB;
		nUnitLength     = -1;
	}
	void Convert855(T_PRUD_D_855& src)
	{
	  memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
	  strActiveElem   = src.strActiveElem   ;
	  strActiveNode   = src.strActiveNode   ;
	  bShrink         = src.bShrink         ;
	  bHidden         = src.bHidden         ;
	  bPerspective    = src.bPerspective    ;
	  DispOpt.Convert855(src.DispOpt);
	  aDimmK          .Copy(src.aDimmK)     ;
	  nActiveKind     = src.nActiveKind     ;
	  aActiveIdtD     .Copy(src.aActiveIdtD);
	  strGrupName     = src.strGrupName     ;
	  RETextD         = src.RETextD;
	  nUcsGcsMode     = src.nUcsGcsMode;
	  UcsDefineD      = src.UcsDefineD;
	  bReferenceDB    = src.bReferenceDB;
	  nUnitLength     = -1;
	}
	void Convert860(T_PRUD_D_860& src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt.Convert860(src.DispOpt);
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind     ;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName     ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		bReferenceDB    = src.bReferenceDB;
		nUnitLength     = -1;
	}
	void Convert865(T_PRUD_D_865& src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt.Convert865(src.DispOpt);
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind     ;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName     ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		bReferenceDB    = src.bReferenceDB;
		nUnitLength     = src.nUnitLength;
	}
	void Convert870(T_PRUD_D_870& src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt.Convert870(src.DispOpt);
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind     ;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName     ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		bReferenceDB    = src.bReferenceDB;
		nUnitLength     = src.nUnitLength;
	}
	void Convert883(T_PRUD_D_883& src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt.Convert883(src.DispOpt);
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind     ;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName     ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		bReferenceDB    = src.bReferenceDB;
		nUnitLength     = src.nUnitLength;
	}
	void Convert885(T_PRUD_D_885& src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt.Convert885(src.DispOpt);
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind     ;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName     ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		bReferenceDB    = src.bReferenceDB;
		nUnitLength     = src.nUnitLength;
	}
	void Convert891(T_PRUD_D_891& src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem   = src.strActiveElem   ;
		strActiveNode   = src.strActiveNode   ;
		bShrink         = src.bShrink         ;
		bHidden         = src.bHidden         ;
		bPerspective    = src.bPerspective    ;
		DispOpt.Convert891(src.DispOpt);
		aDimmK          .Copy(src.aDimmK)     ;
		nActiveKind     = src.nActiveKind     ;
		aActiveIdtD     .Copy(src.aActiveIdtD);
		strGrupName     = src.strGrupName     ;
		RETextD         = src.RETextD;
		nUcsGcsMode     = src.nUcsGcsMode;
		UcsDefineD      = src.UcsDefineD;
		bReferenceDB    = src.bReferenceDB;
		nUnitLength     = src.nUnitLength;
	}
	void Convert895(T_PRUD_D_895& src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem = src.strActiveElem;
		strActiveNode = src.strActiveNode;
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt.Convert895(src.DispOpt);
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		strGrupName = src.strGrupName;
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;
	}
	void Convert925(T_PRUD_D_925& src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem = src.strActiveElem;
		strActiveNode = src.strActiveNode;
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt.Convert925(src.DispOpt);
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		strGrupName = src.strGrupName;
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;
	}
	void Convert930(T_PRUD_D_930& src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem = src.strActiveElem;
		strActiveNode = src.strActiveNode;
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt.Convert930(src.DispOpt);
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		strGrupName = src.strGrupName;
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;
	}
	void Convert940(T_PRUD_D_940& src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem = src.strActiveElem;
		strActiveNode = src.strActiveNode;
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt.Convert940(src.DispOpt);
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		strGrupName = src.strGrupName;
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;
	}
	void Convert950(T_PRUD_D_950& src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		strActiveElem = src.strActiveElem;
		strActiveNode = src.strActiveNode;
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt.Convert950(src.DispOpt);
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		strGrupName = src.strGrupName;
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;
	}
};

struct T_PRUD_D_CH_790
{
	double    dViewPoint[3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_810 DispOpt;
	CArray<UINT, UINT> aDimmK;
	int       nActiveKind;
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	
	void Get(T_PRUD_D_790 &rData)
	{
		memcpy(rData.dViewPoint, dViewPoint, sizeof(rData.dViewPoint));
		ConvertCharStr(strActiveElem, rData.strActiveElem, sizeof(strActiveElem));
		ConvertCharStr(strActiveNode, rData.strActiveNode, sizeof(strActiveNode));
		ConvertCharStr(strGrupName, rData.strGrupName, sizeof(strGrupName));
		rData.bShrink      = bShrink;
		rData.bHidden      = bHidden;
		rData.bPerspective = bPerspective;
		DispOpt.Get(rData.DispOpt);
		rData.aDimmK.Copy(aDimmK);
		rData.nActiveKind = nActiveKind;
		rData.aActiveIdtD.Copy(aActiveIdtD);
		rData.RETextD = RETextD;
		rData.nUcsGcsMode = nUcsGcsMode;
		rData.UcsDefineD = UcsDefineD;
	}
	void Set(T_PRUD_D_790 Data)
	{
		memcpy(dViewPoint, Data.dViewPoint, sizeof(dViewPoint));
		ConvertStrChar(Data.strActiveElem, strActiveElem, sizeof(strActiveElem));
		ConvertStrChar(Data.strActiveNode, strActiveNode, sizeof(strActiveNode));
		ConvertStrChar(Data.strGrupName, strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Set(Data.DispOpt);
		aDimmK.Copy(Data.aDimmK);
		nActiveKind = Data.nActiveKind;
		aActiveIdtD.Copy(Data.aActiveIdtD);
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
	}
	T_PRUD_D_CH_790() {}
	T_PRUD_D_CH_790(const T_PRUD_D_CH_790 &src) { *this = src; }
	T_PRUD_D_CH_790& operator=(const T_PRUD_D_CH_790 &src)
	{
		memcpy(dViewPoint, src.dViewPoint, sizeof(dViewPoint));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt = src.DispOpt;
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		return *this;
	}
};
struct T_PRUD_D_CH_810
{
	double    dViewPoint[3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_810 DispOpt;
	CArray<UINT, UINT> aDimmK;
	int       nActiveKind;
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB    ; // 791에 추가. 20110418 KIMJM, (Import Reference DB로 됐을때 TRUE, Update to Current View 시에는 FALSE)
	
	void Get(T_PRUD_D_810 &rData)
	{
		memcpy(rData.dViewPoint, dViewPoint, sizeof(rData.dViewPoint));
		ConvertCharStr(strActiveElem, rData.strActiveElem, sizeof(strActiveElem));
		ConvertCharStr(strActiveNode, rData.strActiveNode, sizeof(strActiveNode));
		ConvertCharStr(strGrupName, rData.strGrupName, sizeof(strGrupName));
		rData.bShrink      = bShrink;
		rData.bHidden      = bHidden;
		rData.bPerspective = bPerspective;
		DispOpt.Get(rData.DispOpt);
		rData.aDimmK.Copy(aDimmK);
		rData.nActiveKind = nActiveKind;
		rData.aActiveIdtD.Copy(aActiveIdtD);
		rData.RETextD = RETextD;
		rData.nUcsGcsMode = nUcsGcsMode;
		rData.UcsDefineD  = UcsDefineD;
		rData.bReferenceDB = bReferenceDB;
	}
	void Set(T_PRUD_D_810 Data)
	{
		memcpy(dViewPoint, Data.dViewPoint, sizeof(dViewPoint));
		ConvertStrChar(Data.strActiveElem, strActiveElem, sizeof(strActiveElem));
		ConvertStrChar(Data.strActiveNode, strActiveNode, sizeof(strActiveNode));
		ConvertStrChar(Data.strGrupName, strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Set(Data.DispOpt);
		aDimmK.Copy(Data.aDimmK);
		nActiveKind = Data.nActiveKind;
		aActiveIdtD.Copy(Data.aActiveIdtD);
		RETextD = Data.RETextD;
		nUcsGcsMode  = Data.nUcsGcsMode;
		UcsDefineD   = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
	}
	T_PRUD_D_CH_810() {}
	T_PRUD_D_CH_810(const T_PRUD_D_CH_810 &src) { *this = src; }
	T_PRUD_D_CH_810& operator=(const T_PRUD_D_CH_810 &src)
	{
		memcpy(dViewPoint, src.dViewPoint, sizeof(dViewPoint));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt = src.DispOpt;
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD      = src.RETextD;
		nUcsGcsMode  = src.nUcsGcsMode;
		UcsDefineD   = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		return *this;
	}
};
struct T_PRUD_D_CH_821
{
	double    dViewPoint[3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_821 DispOpt;
	CArray<UINT, UINT> aDimmK;
	int       nActiveKind;
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB    ; // 791에 추가. 20110418 KIMJM, (Import Reference DB로 됐을때 TRUE, Update to Current View 시에는 FALSE)
	
	void Get(T_PRUD_D_821 &rData)
	{
		memcpy(rData.dViewPoint, dViewPoint, sizeof(rData.dViewPoint));
		ConvertCharStr(strActiveElem, rData.strActiveElem, sizeof(strActiveElem));
		ConvertCharStr(strActiveNode, rData.strActiveNode, sizeof(strActiveNode));
		ConvertCharStr(strGrupName, rData.strGrupName, sizeof(strGrupName));
		rData.bShrink      = bShrink;
		rData.bHidden      = bHidden;
		rData.bPerspective = bPerspective;
		DispOpt.Get(rData.DispOpt);
		rData.aDimmK.Copy(aDimmK);
		rData.nActiveKind = nActiveKind;
		rData.aActiveIdtD.Copy(aActiveIdtD);
		rData.RETextD = RETextD;
		rData.nUcsGcsMode = nUcsGcsMode;
		rData.UcsDefineD  = UcsDefineD;
		rData.bReferenceDB = bReferenceDB;
	}
	void Set(T_PRUD_D_821 Data)
	{
		memcpy(dViewPoint, Data.dViewPoint, sizeof(dViewPoint));
		ConvertStrChar(Data.strActiveElem, strActiveElem, sizeof(strActiveElem));
		ConvertStrChar(Data.strActiveNode, strActiveNode, sizeof(strActiveNode));
		ConvertStrChar(Data.strGrupName, strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Set(Data.DispOpt);
		aDimmK.Copy(Data.aDimmK);
		nActiveKind = Data.nActiveKind;
		aActiveIdtD.Copy(Data.aActiveIdtD);
		RETextD = Data.RETextD;
		nUcsGcsMode  = Data.nUcsGcsMode;
		UcsDefineD   = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
	}
	T_PRUD_D_CH_821() {}
	T_PRUD_D_CH_821(const T_PRUD_D_CH_821 &src) { *this = src; }
	T_PRUD_D_CH_821& operator=(const T_PRUD_D_CH_821 &src)
	{
		memcpy(dViewPoint, src.dViewPoint, sizeof(dViewPoint));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt = src.DispOpt;
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD      = src.RETextD;
		nUcsGcsMode  = src.nUcsGcsMode;
		UcsDefineD   = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		return *this;
	}
};
struct T_PRUD_D_CH_835
{
	double    dViewPoint[3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_835 DispOpt;
	CArray<UINT, UINT> aDimmK;
	int       nActiveKind;
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB    ; // 791에 추가. 20110418 KIMJM, (Import Reference DB로 됐을때 TRUE, Update to Current View 시에는 FALSE)

	void Get(T_PRUD_D_835 &rData)
	{
		memcpy(rData.dViewPoint, dViewPoint, sizeof(rData.dViewPoint));
		ConvertCharStr(strActiveElem, rData.strActiveElem, sizeof(strActiveElem));
		ConvertCharStr(strActiveNode, rData.strActiveNode, sizeof(strActiveNode));
		ConvertCharStr(strGrupName, rData.strGrupName, sizeof(strGrupName));
		rData.bShrink      = bShrink;
		rData.bHidden      = bHidden;
		rData.bPerspective = bPerspective;
		DispOpt.Get(rData.DispOpt);
		rData.aDimmK.Copy(aDimmK);
		rData.nActiveKind = nActiveKind;
		rData.aActiveIdtD.Copy(aActiveIdtD);
		rData.RETextD = RETextD;
		rData.nUcsGcsMode = nUcsGcsMode;
		rData.UcsDefineD  = UcsDefineD;
		rData.bReferenceDB = bReferenceDB;
	}
	void Set(T_PRUD_D_835 Data)
	{
		memcpy(dViewPoint, Data.dViewPoint, sizeof(dViewPoint));
		ConvertStrChar(Data.strActiveElem, strActiveElem, sizeof(strActiveElem));
		ConvertStrChar(Data.strActiveNode, strActiveNode, sizeof(strActiveNode));
		ConvertStrChar(Data.strGrupName, strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Set(Data.DispOpt);
		aDimmK.Copy(Data.aDimmK);
		nActiveKind = Data.nActiveKind;
		aActiveIdtD.Copy(Data.aActiveIdtD);
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
	}
	T_PRUD_D_CH_835() {}
	T_PRUD_D_CH_835(const T_PRUD_D_CH_835 &src) { *this = src; }
	T_PRUD_D_CH_835& operator=(const T_PRUD_D_CH_835 &src)
	{
		memcpy(dViewPoint, src.dViewPoint, sizeof(dViewPoint));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt = src.DispOpt;
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD      = src.RETextD;
		nUcsGcsMode  = src.nUcsGcsMode;
		UcsDefineD   = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		return *this;
	}
};
struct T_PRUD_D_CH_841
{
	double    dViewPoint[3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_841 DispOpt;
	CArray<UINT, UINT> aDimmK;
	int       nActiveKind;
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB    ; // 791에 추가. 20110418 KIMJM, (Import Reference DB로 됐을때 TRUE, Update to Current View 시에는 FALSE)

	void Get(T_PRUD_D_841 &rData)
	{
		memcpy(rData.dViewPoint, dViewPoint, sizeof(rData.dViewPoint));
		ConvertCharStr(strActiveElem, rData.strActiveElem, sizeof(strActiveElem));
		ConvertCharStr(strActiveNode, rData.strActiveNode, sizeof(strActiveNode));
		ConvertCharStr(strGrupName, rData.strGrupName, sizeof(strGrupName));
		rData.bShrink      = bShrink;
		rData.bHidden      = bHidden;
		rData.bPerspective = bPerspective;
		DispOpt.Get(rData.DispOpt);
		rData.aDimmK.Copy(aDimmK);
		rData.nActiveKind = nActiveKind;
		rData.aActiveIdtD.Copy(aActiveIdtD);
		rData.RETextD = RETextD;
		rData.nUcsGcsMode = nUcsGcsMode;
		rData.UcsDefineD  = UcsDefineD;
		rData.bReferenceDB = bReferenceDB;
	}
	void Set(T_PRUD_D_841 Data)
	{
		memcpy(dViewPoint, Data.dViewPoint, sizeof(dViewPoint));
		ConvertStrChar(Data.strActiveElem, strActiveElem, sizeof(strActiveElem));
		ConvertStrChar(Data.strActiveNode, strActiveNode, sizeof(strActiveNode));
		ConvertStrChar(Data.strGrupName, strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Set(Data.DispOpt);
		aDimmK.Copy(Data.aDimmK);
		nActiveKind = Data.nActiveKind;
		aActiveIdtD.Copy(Data.aActiveIdtD);
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
	}
	T_PRUD_D_CH_841() {}
	T_PRUD_D_CH_841(const T_PRUD_D_CH_841 &src) { *this = src; }
	T_PRUD_D_CH_841& operator=(const T_PRUD_D_CH_841 &src)
	{
		memcpy(dViewPoint, src.dViewPoint, sizeof(dViewPoint));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt = src.DispOpt;
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD      = src.RETextD;
		nUcsGcsMode  = src.nUcsGcsMode;
		UcsDefineD   = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		return *this;
	}
};

struct T_PRUD_D_CH_845
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_855 DispOpt;
	CArray<UINT, UINT> aDimmK;
	int       nActiveKind;
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB;

	void Get(T_PRUD_D_845 &rData)
	{
		memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
		ConvertCharStr(strActiveElem, rData.strActiveElem, sizeof(strActiveElem));
		ConvertCharStr(strActiveNode, rData.strActiveNode, sizeof(strActiveNode));
		ConvertCharStr(strGrupName, rData.strGrupName, sizeof(strGrupName));
		rData.bShrink      = bShrink;
		rData.bHidden      = bHidden;
		rData.bPerspective = bPerspective;
		DispOpt.Get(rData.DispOpt);
		rData.aDimmK.Copy(aDimmK);
		rData.nActiveKind = nActiveKind;
		rData.aActiveIdtD.Copy(aActiveIdtD);
		rData.RETextD = RETextD;
		rData.nUcsGcsMode = nUcsGcsMode;
		rData.UcsDefineD = UcsDefineD;
		rData.bReferenceDB = bReferenceDB;
	}
	void Set(T_PRUD_D_845 Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		ConvertStrChar(Data.strActiveElem, strActiveElem, sizeof(strActiveElem));
		ConvertStrChar(Data.strActiveNode, strActiveNode, sizeof(strActiveNode));
		ConvertStrChar(Data.strGrupName, strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Set(Data.DispOpt);
		aDimmK.Copy(Data.aDimmK);
		nActiveKind = Data.nActiveKind;
		aActiveIdtD.Copy(Data.aActiveIdtD);
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
	}
	T_PRUD_D_CH_845() {}
	T_PRUD_D_CH_845(const T_PRUD_D_CH_845 &src) { *this = src; }
	T_PRUD_D_CH_845& operator=(const T_PRUD_D_CH_845 &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt = src.DispOpt;
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		return *this;
	}
};

struct T_PRUD_D_CH_855
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_855 DispOpt;
	CArray<UINT, UINT> aDimmK;
	int       nActiveKind;
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB;
	int       nUnitLength;

	void Get(T_PRUD_D_855 &rData)
	{
		memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
		ConvertCharStr(strActiveElem, rData.strActiveElem, sizeof(strActiveElem));
		ConvertCharStr(strActiveNode, rData.strActiveNode, sizeof(strActiveNode));
		ConvertCharStr(strGrupName, rData.strGrupName, sizeof(strGrupName));
		rData.bShrink      = bShrink;
		rData.bHidden      = bHidden;
		rData.bPerspective = bPerspective;
		DispOpt.Get(rData.DispOpt);
		rData.aDimmK.Copy(aDimmK);
		rData.nActiveKind = nActiveKind;
		rData.aActiveIdtD.Copy(aActiveIdtD);
		rData.RETextD = RETextD;
		rData.nUcsGcsMode = nUcsGcsMode;
		rData.UcsDefineD = UcsDefineD;
		rData.bReferenceDB = bReferenceDB;
		rData.nUnitLength = nUnitLength;
	}
	void Set(T_PRUD_D_855 Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		ConvertStrChar(Data.strActiveElem, strActiveElem, sizeof(strActiveElem));
		ConvertStrChar(Data.strActiveNode, strActiveNode, sizeof(strActiveNode));
		ConvertStrChar(Data.strGrupName, strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Set(Data.DispOpt);
		aDimmK.Copy(Data.aDimmK);
		nActiveKind = Data.nActiveKind;
		aActiveIdtD.Copy(Data.aActiveIdtD);
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}
	T_PRUD_D_CH_855() {}
	T_PRUD_D_CH_855(const T_PRUD_D_CH_855 &src) { *this = src; }
	T_PRUD_D_CH_855& operator=(const T_PRUD_D_CH_855 &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt = src.DispOpt;
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;
		return *this;
	}
};

struct T_PRUD_D_CH_860
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_860 DispOpt;
	CArray<UINT, UINT> aDimmK;
	int       nActiveKind;
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB;
	int       nUnitLength;

	void Get(T_PRUD_D_860 &rData)
	{
		memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
		ConvertCharStr(strActiveElem, rData.strActiveElem, sizeof(strActiveElem));
		ConvertCharStr(strActiveNode, rData.strActiveNode, sizeof(strActiveNode));
		ConvertCharStr(strGrupName, rData.strGrupName, sizeof(strGrupName));
		rData.bShrink      = bShrink;
		rData.bHidden      = bHidden;
		rData.bPerspective = bPerspective;
		DispOpt.Get(rData.DispOpt);
		rData.aDimmK.Copy(aDimmK);
		rData.nActiveKind = nActiveKind;
		rData.aActiveIdtD.Copy(aActiveIdtD);
		rData.RETextD = RETextD;
		rData.nUcsGcsMode = nUcsGcsMode;
		rData.UcsDefineD = UcsDefineD;
		rData.bReferenceDB = bReferenceDB;
		rData.nUnitLength = nUnitLength;
	}
	void Set(T_PRUD_D_860 Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		ConvertStrChar(Data.strActiveElem, strActiveElem, sizeof(strActiveElem));
		ConvertStrChar(Data.strActiveNode, strActiveNode, sizeof(strActiveNode));
		ConvertStrChar(Data.strGrupName, strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Set(Data.DispOpt);
		aDimmK.Copy(Data.aDimmK);
		nActiveKind = Data.nActiveKind;
		aActiveIdtD.Copy(Data.aActiveIdtD);
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}
	T_PRUD_D_CH_860() {}
	T_PRUD_D_CH_860(const T_PRUD_D_CH_860 &src) { *this = src; }
	T_PRUD_D_CH_860& operator=(const T_PRUD_D_CH_860 &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt = src.DispOpt;
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;
		return *this;
	}
};

struct T_PRUD_D_CH_865
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_865 DispOpt;
	CArray<UINT, UINT> aDimmK;
	int       nActiveKind;
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB;
	int       nUnitLength;

	void Get(T_PRUD_D_865 &rData)
	{
		memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
		ConvertCharStr(strActiveElem, rData.strActiveElem, sizeof(strActiveElem));
		ConvertCharStr(strActiveNode, rData.strActiveNode, sizeof(strActiveNode));
		ConvertCharStr(strGrupName, rData.strGrupName, sizeof(strGrupName));
		rData.bShrink      = bShrink;
		rData.bHidden      = bHidden;
		rData.bPerspective = bPerspective;
		DispOpt.Get(rData.DispOpt);
		rData.aDimmK.Copy(aDimmK);
		rData.nActiveKind = nActiveKind;
		rData.aActiveIdtD.Copy(aActiveIdtD);
		rData.RETextD = RETextD;
		rData.nUcsGcsMode = nUcsGcsMode;
		rData.UcsDefineD = UcsDefineD;
		rData.bReferenceDB = bReferenceDB;
		rData.nUnitLength = nUnitLength;
	}
	void Set(T_PRUD_D_865 Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		ConvertStrChar(Data.strActiveElem, strActiveElem, sizeof(strActiveElem));
		ConvertStrChar(Data.strActiveNode, strActiveNode, sizeof(strActiveNode));
		ConvertStrChar(Data.strGrupName, strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Set(Data.DispOpt);
		aDimmK.Copy(Data.aDimmK);
		nActiveKind = Data.nActiveKind;
		aActiveIdtD.Copy(Data.aActiveIdtD);
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}
	T_PRUD_D_CH_865() {}
	T_PRUD_D_CH_865(const T_PRUD_D_CH_865 &src) { *this = src; }
	T_PRUD_D_CH_865& operator=(const T_PRUD_D_CH_865 &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt = src.DispOpt;
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;
		return *this;
	}
};
struct T_PRUD_D_CH_870
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_870 DispOpt;
	CArray<UINT, UINT> aDimmK;
	int       nActiveKind;
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB;
	int       nUnitLength;

	void Get(T_PRUD_D_870 &rData)
	{
		memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
		ConvertCharStr(strActiveElem, rData.strActiveElem, sizeof(strActiveElem));
		ConvertCharStr(strActiveNode, rData.strActiveNode, sizeof(strActiveNode));
		ConvertCharStr(strGrupName, rData.strGrupName, sizeof(strGrupName));
		rData.bShrink      = bShrink;
		rData.bHidden      = bHidden;
		rData.bPerspective = bPerspective;
		DispOpt.Get(rData.DispOpt);
		rData.aDimmK.Copy(aDimmK);
		rData.nActiveKind = nActiveKind;
		rData.aActiveIdtD.Copy(aActiveIdtD);
		rData.RETextD = RETextD;
		rData.nUcsGcsMode = nUcsGcsMode;
		rData.UcsDefineD = UcsDefineD;
		rData.bReferenceDB = bReferenceDB;
		rData.nUnitLength = nUnitLength;
	}
	void Set(T_PRUD_D_870 Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		ConvertStrChar(Data.strActiveElem, strActiveElem, sizeof(strActiveElem));
		ConvertStrChar(Data.strActiveNode, strActiveNode, sizeof(strActiveNode));
		ConvertStrChar(Data.strGrupName, strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Set(Data.DispOpt);
		aDimmK.Copy(Data.aDimmK);
		nActiveKind = Data.nActiveKind;
		aActiveIdtD.Copy(Data.aActiveIdtD);
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}
	T_PRUD_D_CH_870() {}
	T_PRUD_D_CH_870(const T_PRUD_D_CH_870 &src) { *this = src; }
	T_PRUD_D_CH_870& operator=(const T_PRUD_D_CH_870 &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt = src.DispOpt;
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;
		return *this;
	}
};
struct T_PRUD_D_CH_883
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_883 DispOpt;
	CArray<UINT, UINT> aDimmK;
	int       nActiveKind;
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB;
	int       nUnitLength;


	void Get(T_PRUD_D_883 &rData)
	{
		memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
		ConvertCharStr(strActiveElem, rData.strActiveElem, sizeof(strActiveElem));
		ConvertCharStr(strActiveNode, rData.strActiveNode, sizeof(strActiveNode));
		ConvertCharStr(strGrupName, rData.strGrupName, sizeof(strGrupName));
		rData.bShrink      = bShrink;
		rData.bHidden      = bHidden;
		rData.bPerspective = bPerspective;
		DispOpt.Get(rData.DispOpt);
		rData.aDimmK.Copy(aDimmK);
		rData.nActiveKind = nActiveKind;
		rData.aActiveIdtD.Copy(aActiveIdtD);
		rData.RETextD = RETextD;
		rData.nUcsGcsMode = nUcsGcsMode;
		rData.UcsDefineD = UcsDefineD;
		rData.bReferenceDB = bReferenceDB;
		rData.nUnitLength = nUnitLength;
	}
	void Set(T_PRUD_D_883 Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		ConvertStrChar(Data.strActiveElem, strActiveElem, sizeof(strActiveElem));
		ConvertStrChar(Data.strActiveNode, strActiveNode, sizeof(strActiveNode));
		ConvertStrChar(Data.strGrupName, strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Set(Data.DispOpt);
		aDimmK.Copy(Data.aDimmK);
		nActiveKind = Data.nActiveKind;
		aActiveIdtD.Copy(Data.aActiveIdtD);
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}
	T_PRUD_D_CH_883() {}
	T_PRUD_D_CH_883(const T_PRUD_D_CH_883 &src) { *this = src; }
	T_PRUD_D_CH_883& operator=(const T_PRUD_D_CH_883 &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt = src.DispOpt;
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;
		return *this;
	}
};

struct T_PRUD_D_CH_885
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_885 DispOpt;
	CArray<UINT, UINT> aDimmK;
	int       nActiveKind;
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB;
	int       nUnitLength;

	void Get(T_PRUD_D_885 &rData)
	{
		memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
		ConvertCharStr(strActiveElem, rData.strActiveElem, sizeof(strActiveElem));
		ConvertCharStr(strActiveNode, rData.strActiveNode, sizeof(strActiveNode));
		ConvertCharStr(strGrupName, rData.strGrupName, sizeof(strGrupName));
		rData.bShrink      = bShrink;
		rData.bHidden      = bHidden;
		rData.bPerspective = bPerspective;
		DispOpt.Get(rData.DispOpt);
		rData.aDimmK.Copy(aDimmK);
		rData.nActiveKind = nActiveKind;
		rData.aActiveIdtD.Copy(aActiveIdtD);
		rData.RETextD = RETextD;
		rData.nUcsGcsMode = nUcsGcsMode;
		rData.UcsDefineD = UcsDefineD;
		rData.bReferenceDB = bReferenceDB;
		rData.nUnitLength = nUnitLength;
	}
	void Set(T_PRUD_D_885 Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		ConvertStrChar(Data.strActiveElem, strActiveElem, sizeof(strActiveElem));
		ConvertStrChar(Data.strActiveNode, strActiveNode, sizeof(strActiveNode));
		ConvertStrChar(Data.strGrupName, strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Set(Data.DispOpt);
		aDimmK.Copy(Data.aDimmK);
		nActiveKind = Data.nActiveKind;
		aActiveIdtD.Copy(Data.aActiveIdtD);
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}
	T_PRUD_D_CH_885() {}
	T_PRUD_D_CH_885(const T_PRUD_D_CH_885 &src) { *this = src; }
	T_PRUD_D_CH_885& operator=(const T_PRUD_D_CH_885 &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt = src.DispOpt;
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;

		return *this;
	}
};

struct T_PRUD_D_CH_891
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_891 DispOpt;
	CArray<UINT, UINT> aDimmK;
	int       nActiveKind;
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB;
	int       nUnitLength;

	void Get(T_PRUD_D_891 &rData)
	{
		memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
		ConvertCharStr(strActiveElem, rData.strActiveElem, sizeof(strActiveElem));
		ConvertCharStr(strActiveNode, rData.strActiveNode, sizeof(strActiveNode));
		ConvertCharStr(strGrupName, rData.strGrupName, sizeof(strGrupName));
		rData.bShrink      = bShrink;
		rData.bHidden      = bHidden;
		rData.bPerspective = bPerspective;
		DispOpt.Get(rData.DispOpt);
		rData.aDimmK.Copy(aDimmK);
		rData.nActiveKind = nActiveKind;
		rData.aActiveIdtD.Copy(aActiveIdtD);
		rData.RETextD = RETextD;
		rData.nUcsGcsMode = nUcsGcsMode;
		rData.UcsDefineD = UcsDefineD;
		rData.bReferenceDB = bReferenceDB;
		rData.nUnitLength = nUnitLength;
	}
	void Set(T_PRUD_D_891 Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		ConvertStrChar(Data.strActiveElem, strActiveElem, sizeof(strActiveElem));
		ConvertStrChar(Data.strActiveNode, strActiveNode, sizeof(strActiveNode));
		ConvertStrChar(Data.strGrupName, strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Set(Data.DispOpt);
		aDimmK.Copy(Data.aDimmK);
		nActiveKind = Data.nActiveKind;
		aActiveIdtD.Copy(Data.aActiveIdtD);
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}
	T_PRUD_D_CH_891() {}
	T_PRUD_D_CH_891(const T_PRUD_D_CH_891 &src) { *this = src; }
	T_PRUD_D_CH_891& operator=(const T_PRUD_D_CH_891 &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt = src.DispOpt;
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;

		return *this;
	}
};
struct T_PRUD_D_CH_895
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_895 DispOpt;
	CArray<UINT, UINT> aDimmK;
	int       nActiveKind;
	CArray<T_ACTIVE_IDENTITY_DB, T_ACTIVE_IDENTITY_DB&> aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD;
	BOOL      bReferenceDB;
	int       nUnitLength;

	void Get(T_PRUD_D_895 &rData)
	{
		memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
		ConvertCharStr(strActiveElem, rData.strActiveElem, sizeof(strActiveElem));
		ConvertCharStr(strActiveNode, rData.strActiveNode, sizeof(strActiveNode));
		ConvertCharStr(strGrupName, rData.strGrupName, sizeof(strGrupName));
		rData.bShrink = bShrink;
		rData.bHidden = bHidden;
		rData.bPerspective = bPerspective;
		DispOpt.Get(rData.DispOpt);
		rData.aDimmK.Copy(aDimmK);
		rData.nActiveKind = nActiveKind;
		rData.aActiveIdtD.Copy(aActiveIdtD);
		rData.RETextD = RETextD;
		rData.nUcsGcsMode = nUcsGcsMode;
		rData.UcsDefineD = UcsDefineD;
		rData.bReferenceDB = bReferenceDB;
		rData.nUnitLength = nUnitLength;
	}
	void Set(T_PRUD_D_895 Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		ConvertStrChar(Data.strActiveElem, strActiveElem, sizeof(strActiveElem));
		ConvertStrChar(Data.strActiveNode, strActiveNode, sizeof(strActiveNode));
		ConvertStrChar(Data.strGrupName, strGrupName, sizeof(strGrupName));
		bShrink = Data.bShrink;
		bHidden = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Set(Data.DispOpt);
		aDimmK.Copy(Data.aDimmK);
		nActiveKind = Data.nActiveKind;
		aActiveIdtD.Copy(Data.aActiveIdtD);
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}
	T_PRUD_D_CH_895() {}
	T_PRUD_D_CH_895(const T_PRUD_D_CH_895 &src) { *this = src; }
	T_PRUD_D_CH_895& operator=(const T_PRUD_D_CH_895 &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt = src.DispOpt;
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;

		return *this;
	}
};
struct T_PRUD_D_CH_925
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_925 DispOpt;
	CArray<UINT, UINT> aDimmK;
	int       nActiveKind;
	CArray<T_ACTIVE_IDENTITY_DB, T_ACTIVE_IDENTITY_DB&> aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD;
	BOOL      bReferenceDB;
	int       nUnitLength;

	void Get(T_PRUD_D_925 &rData)
	{
		memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
		ConvertCharStr(strActiveElem, rData.strActiveElem, sizeof(strActiveElem));
		ConvertCharStr(strActiveNode, rData.strActiveNode, sizeof(strActiveNode));
		ConvertCharStr(strGrupName, rData.strGrupName, sizeof(strGrupName));
		rData.bShrink = bShrink;
		rData.bHidden = bHidden;
		rData.bPerspective = bPerspective;
		DispOpt.Get(rData.DispOpt);
		rData.aDimmK.Copy(aDimmK);
		rData.nActiveKind = nActiveKind;
		rData.aActiveIdtD.Copy(aActiveIdtD);
		rData.RETextD = RETextD;
		rData.nUcsGcsMode = nUcsGcsMode;
		rData.UcsDefineD = UcsDefineD;
		rData.bReferenceDB = bReferenceDB;
		rData.nUnitLength = nUnitLength;
	}
	void Set(T_PRUD_D_925 Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		ConvertStrChar(Data.strActiveElem, strActiveElem, sizeof(strActiveElem));
		ConvertStrChar(Data.strActiveNode, strActiveNode, sizeof(strActiveNode));
		ConvertStrChar(Data.strGrupName, strGrupName, sizeof(strGrupName));
		bShrink = Data.bShrink;
		bHidden = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Set(Data.DispOpt);
		aDimmK.Copy(Data.aDimmK);
		nActiveKind = Data.nActiveKind;
		aActiveIdtD.Copy(Data.aActiveIdtD);
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}
	T_PRUD_D_CH_925() {}
	T_PRUD_D_CH_925(const T_PRUD_D_CH_925 &src) { *this = src; }
	T_PRUD_D_CH_925& operator=(const T_PRUD_D_CH_925 &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt = src.DispOpt;
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;

		return *this;
	}
};
struct T_PRUD_D_CH_930
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_930 DispOpt;
	CArray<UINT, UINT> aDimmK;
	int       nActiveKind;
	CArray<T_ACTIVE_IDENTITY_DB, T_ACTIVE_IDENTITY_DB&> aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD;
	BOOL      bReferenceDB;
	int       nUnitLength;

	void Get(T_PRUD_D_930 &rData)
	{
		memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
		ConvertCharStr(strActiveElem, rData.strActiveElem, sizeof(strActiveElem));
		ConvertCharStr(strActiveNode, rData.strActiveNode, sizeof(strActiveNode));
		ConvertCharStr(strGrupName, rData.strGrupName, sizeof(strGrupName));
		rData.bShrink = bShrink;
		rData.bHidden = bHidden;
		rData.bPerspective = bPerspective;
		DispOpt.Get(rData.DispOpt);
		rData.aDimmK.Copy(aDimmK);
		rData.nActiveKind = nActiveKind;
		rData.aActiveIdtD.Copy(aActiveIdtD);
		rData.RETextD = RETextD;
		rData.nUcsGcsMode = nUcsGcsMode;
		rData.UcsDefineD = UcsDefineD;
		rData.bReferenceDB = bReferenceDB;
		rData.nUnitLength = nUnitLength;
	}
	void Set(T_PRUD_D_930 Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		ConvertStrChar(Data.strActiveElem, strActiveElem, sizeof(strActiveElem));
		ConvertStrChar(Data.strActiveNode, strActiveNode, sizeof(strActiveNode));
		ConvertStrChar(Data.strGrupName, strGrupName, sizeof(strGrupName));
		bShrink = Data.bShrink;
		bHidden = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Set(Data.DispOpt);
		aDimmK.Copy(Data.aDimmK);
		nActiveKind = Data.nActiveKind;
		aActiveIdtD.Copy(Data.aActiveIdtD);
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}
	T_PRUD_D_CH_930() {}
	T_PRUD_D_CH_930(const T_PRUD_D_CH_930 &src) { *this = src; }
	T_PRUD_D_CH_930& operator=(const T_PRUD_D_CH_930 &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt = src.DispOpt;
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;

		return *this;
	}
};
struct T_PRUD_D_CH_940
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_940 DispOpt;
	CArray<UINT, UINT> aDimmK;
	int       nActiveKind;
	CArray<T_ACTIVE_IDENTITY_DB, T_ACTIVE_IDENTITY_DB&> aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD;
	BOOL      bReferenceDB;
	int       nUnitLength;

	void Get(T_PRUD_D_940 &rData)
	{
		memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
		ConvertCharStr(strActiveElem, rData.strActiveElem, sizeof(strActiveElem));
		ConvertCharStr(strActiveNode, rData.strActiveNode, sizeof(strActiveNode));
		ConvertCharStr(strGrupName, rData.strGrupName, sizeof(strGrupName));
		rData.bShrink = bShrink;
		rData.bHidden = bHidden;
		rData.bPerspective = bPerspective;
		DispOpt.Get(rData.DispOpt);
		rData.aDimmK.Copy(aDimmK);
		rData.nActiveKind = nActiveKind;
		rData.aActiveIdtD.Copy(aActiveIdtD);
		rData.RETextD = RETextD;
		rData.nUcsGcsMode = nUcsGcsMode;
		rData.UcsDefineD = UcsDefineD;
		rData.bReferenceDB = bReferenceDB;
		rData.nUnitLength = nUnitLength;
	}
	void Set(T_PRUD_D_940 Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		ConvertStrChar(Data.strActiveElem, strActiveElem, sizeof(strActiveElem));
		ConvertStrChar(Data.strActiveNode, strActiveNode, sizeof(strActiveNode));
		ConvertStrChar(Data.strGrupName, strGrupName, sizeof(strGrupName));
		bShrink = Data.bShrink;
		bHidden = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Set(Data.DispOpt);
		aDimmK.Copy(Data.aDimmK);
		nActiveKind = Data.nActiveKind;
		aActiveIdtD.Copy(Data.aActiveIdtD);
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}
	T_PRUD_D_CH_940() {}
	T_PRUD_D_CH_940(const T_PRUD_D_CH_940 &src) { *this = src; }
	T_PRUD_D_CH_940& operator=(const T_PRUD_D_CH_940 &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt = src.DispOpt;
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;

		return *this;
	}
};

struct T_PRUD_D_CH_950
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_950 DispOpt;
	CArray<UINT, UINT> aDimmK;
	int       nActiveKind;
	CArray<T_ACTIVE_IDENTITY_DB, T_ACTIVE_IDENTITY_DB&> aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD;
	BOOL      bReferenceDB;
	int       nUnitLength;

	void Get(T_PRUD_D_950& rData)
	{
		memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
		ConvertCharStr(strActiveElem, rData.strActiveElem, sizeof(strActiveElem));
		ConvertCharStr(strActiveNode, rData.strActiveNode, sizeof(strActiveNode));
		ConvertCharStr(strGrupName, rData.strGrupName, sizeof(strGrupName));
		rData.bShrink = bShrink;
		rData.bHidden = bHidden;
		rData.bPerspective = bPerspective;
		DispOpt.Get(rData.DispOpt);
		rData.aDimmK.Copy(aDimmK);
		rData.nActiveKind = nActiveKind;
		rData.aActiveIdtD.Copy(aActiveIdtD);
		rData.RETextD = RETextD;
		rData.nUcsGcsMode = nUcsGcsMode;
		rData.UcsDefineD = UcsDefineD;
		rData.bReferenceDB = bReferenceDB;
		rData.nUnitLength = nUnitLength;
	}
	void Set(T_PRUD_D_950 Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		ConvertStrChar(Data.strActiveElem, strActiveElem, sizeof(strActiveElem));
		ConvertStrChar(Data.strActiveNode, strActiveNode, sizeof(strActiveNode));
		ConvertStrChar(Data.strGrupName, strGrupName, sizeof(strGrupName));
		bShrink = Data.bShrink;
		bHidden = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Set(Data.DispOpt);
		aDimmK.Copy(Data.aDimmK);
		nActiveKind = Data.nActiveKind;
		aActiveIdtD.Copy(Data.aActiveIdtD);
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}
	T_PRUD_D_CH_950() {}
	T_PRUD_D_CH_950(const T_PRUD_D_CH_950& src) { *this = src; }
	T_PRUD_D_CH_950& operator=(const T_PRUD_D_CH_950& src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt = src.DispOpt;
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;

		return *this;
	}
};

struct T_PRUD_D_CH
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH DispOpt;
	CArray<UINT, UINT> aDimmK;
	int       nActiveKind;
	CArray<T_ACTIVE_IDENTITY_DB,T_ACTIVE_IDENTITY_DB&> aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB;
	int       nUnitLength;

	void Get(T_PRUD_D &rData)
	{
		memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
		ConvertCharStr(strActiveElem, rData.strActiveElem, sizeof(strActiveElem));
		ConvertCharStr(strActiveNode, rData.strActiveNode, sizeof(strActiveNode));
		ConvertCharStr(strGrupName, rData.strGrupName, sizeof(strGrupName));
		rData.bShrink      = bShrink;
		rData.bHidden      = bHidden;
		rData.bPerspective = bPerspective;
		DispOpt.Get(rData.DispOpt);
		rData.aDimmK.Copy(aDimmK);
		rData.nActiveKind = nActiveKind;
		rData.aActiveIdtD.Copy(aActiveIdtD);
		rData.RETextD = RETextD;
		rData.nUcsGcsMode = nUcsGcsMode;
		rData.UcsDefineD = UcsDefineD;
		rData.bReferenceDB = bReferenceDB;
		rData.nUnitLength = nUnitLength;
	}
	void Set(T_PRUD_D Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		ConvertStrChar(Data.strActiveElem, strActiveElem, sizeof(strActiveElem));
		ConvertStrChar(Data.strActiveNode, strActiveNode, sizeof(strActiveNode));
		ConvertStrChar(Data.strGrupName, strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Set(Data.DispOpt);
		aDimmK.Copy(Data.aDimmK);
		nActiveKind = Data.nActiveKind;
		aActiveIdtD.Copy(Data.aActiveIdtD);
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}
	T_PRUD_D_CH() {}
	T_PRUD_D_CH(const T_PRUD_D_CH &src) { *this = src; }
	T_PRUD_D_CH& operator=(const T_PRUD_D_CH &src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt = src.DispOpt;
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;

		return *this;
	}
	void Convert790(T_PRUD_D_CH_790& src)
	{
		memset(VCSVector,0,sizeof(VCSVector));
		memcpy(VCSVector[2],src.dViewPoint,sizeof(src.dViewPoint));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt.Convert810(src.DispOpt);
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = FALSE;
	}
	void Convert810(T_PRUD_D_CH_810& src)
	{
		memset(VCSVector,0,sizeof(VCSVector));
		memcpy(VCSVector[2],src.dViewPoint,sizeof(src.dViewPoint));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt.Convert810(src.DispOpt);
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = FALSE;
	}
	void Convert821(T_PRUD_D_CH_821& src)
	{
		memset(VCSVector,0,sizeof(VCSVector));
		memcpy(VCSVector[2],src.dViewPoint,sizeof(src.dViewPoint));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt.Convert821(src.DispOpt);
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = FALSE;
	}
	void Convert835(T_PRUD_D_CH_835& src)
	{
		memset(VCSVector,0,sizeof(VCSVector));
		memcpy(VCSVector[2],src.dViewPoint,sizeof(src.dViewPoint));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt.Convert835(src.DispOpt);
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = FALSE;
	}
	void Convert841(T_PRUD_D_CH_841& src)
	{
		memset(VCSVector,0,sizeof(VCSVector));
		memcpy(VCSVector[2],src.dViewPoint,sizeof(src.dViewPoint));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt.Convert841(src.DispOpt);
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = FALSE;
	}
	void Convert845(T_PRUD_D_CH_845& src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt.Convert855(src.DispOpt);
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = -1;
	}
	void Convert855(T_PRUD_D_CH_855& src)
	{
	  memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
	  memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
	  memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
	  memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
	  bShrink = src.bShrink;
	  bHidden = src.bHidden;
	  bPerspective = src.bPerspective;
	  DispOpt.Convert855(src.DispOpt);
	  aDimmK.Copy(src.aDimmK);
	  nActiveKind = src.nActiveKind;
	  aActiveIdtD.Copy(src.aActiveIdtD);
	  RETextD = src.RETextD;
	  nUcsGcsMode = src.nUcsGcsMode;
	  UcsDefineD = src.UcsDefineD;
	  bReferenceDB = src.bReferenceDB;
	  nUnitLength = -1;
	}
	void Convert860(T_PRUD_D_CH_860& src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt.Convert860(src.DispOpt);
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = -1;
	}
	void Convert865(T_PRUD_D_CH_865& src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt.Convert865(src.DispOpt);
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;
	}
	void Convert870(T_PRUD_D_CH_870& src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt.Convert870(src.DispOpt);
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;
	}
	void Convert883(T_PRUD_D_CH_883& src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt.Convert883(src.DispOpt);
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;
	}
	void Convert885(T_PRUD_D_CH_885& src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt.Convert885(src.DispOpt);
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;
	}
	void Convert891(T_PRUD_D_CH_891& src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt.Convert891(src.DispOpt);
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;
	}
	void Convert895(T_PRUD_D_CH_895& src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt.Convert895(src.DispOpt);
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;
	}
	void Convert925(T_PRUD_D_CH_925& src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt.Convert925(src.DispOpt);
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;
	}
	void Convert940(T_PRUD_D_CH_940& src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt.Convert940(src.DispOpt);
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;
	}
	void Convert950(T_PRUD_D_CH_950& src)
	{
		memcpy(VCSVector, src.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, src.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, src.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, src.strGrupName, sizeof(strGrupName));
		bShrink = src.bShrink;
		bHidden = src.bHidden;
		bPerspective = src.bPerspective;
		DispOpt.Convert950(src.DispOpt);
		aDimmK.Copy(src.aDimmK);
		nActiveKind = src.nActiveKind;
		aActiveIdtD.Copy(src.aActiveIdtD);
		RETextD = src.RETextD;
		nUcsGcsMode = src.nUcsGcsMode;
		UcsDefineD = src.UcsDefineD;
		bReferenceDB = src.bReferenceDB;
		nUnitLength = src.nUnitLength;
	}
};
struct T_PRUD_D_CH_RW_790
{
	double    dViewPoint[3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_RW_810 DispOpt;
	int       nCount_aDimmK;
	int       nActiveKind;
	int       nCount_aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 

	void Get(T_PRUD_D_CH_790 &rData)
	{
		memcpy(rData.dViewPoint, dViewPoint, sizeof(rData.dViewPoint));
		memcpy(rData.strActiveElem, strActiveElem, sizeof(rData.strActiveElem));
		memcpy(rData.strActiveNode, strActiveNode, sizeof(rData.strActiveNode));
		memcpy(rData.strGrupName, strGrupName, sizeof(rData.strGrupName));
		rData.bShrink      = bShrink;
		rData.bHidden      = bHidden;
		rData.bPerspective = bPerspective;
		DispOpt.Get(rData.DispOpt);
		rData.aDimmK.SetSize(nCount_aDimmK);
		rData.nActiveKind = nActiveKind;
		rData.aActiveIdtD.SetSize(nCount_aActiveIdtD);
		rData.RETextD = RETextD;
		rData.nUcsGcsMode = nUcsGcsMode;
		rData.UcsDefineD = UcsDefineD;
	}
	void Set(T_PRUD_D_CH_790 Data)
	{
		memcpy(dViewPoint, Data.dViewPoint, sizeof(dViewPoint));
		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Set(Data.DispOpt);
		nCount_aDimmK = (int)Data.aDimmK.GetSize();
		nActiveKind = Data.nActiveKind;
		nCount_aActiveIdtD = (int)Data.aActiveIdtD.GetSize();
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
	}
};
struct T_PRUD_D_CH_RW_810
{
	double    dViewPoint[3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_RW_810 DispOpt;
	int       nCount_aDimmK;
	int       nActiveKind;
	int       nCount_aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB;
	
	void Get(T_PRUD_D_CH_810 &rData)
	{
		memcpy(rData.dViewPoint, dViewPoint, sizeof(rData.dViewPoint));
		memcpy(rData.strActiveElem, strActiveElem, sizeof(rData.strActiveElem));
		memcpy(rData.strActiveNode, strActiveNode, sizeof(rData.strActiveNode));
		memcpy(rData.strGrupName, strGrupName, sizeof(rData.strGrupName));
		rData.bShrink      = bShrink;
		rData.bHidden      = bHidden;
		rData.bPerspective = bPerspective;
		DispOpt.Get(rData.DispOpt);
		rData.aDimmK.SetSize(nCount_aDimmK);
		rData.nActiveKind = nActiveKind;
		rData.aActiveIdtD.SetSize(nCount_aActiveIdtD);
		rData.RETextD = RETextD;
		rData.nUcsGcsMode = nUcsGcsMode;
		rData.UcsDefineD = UcsDefineD;
		rData.bReferenceDB = bReferenceDB;
	}
	void Set(T_PRUD_D_CH_810 Data)
	{
		memcpy(dViewPoint, Data.dViewPoint, sizeof(dViewPoint));
		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Set(Data.DispOpt);
		nCount_aDimmK = (int)Data.aDimmK.GetSize();
		nActiveKind = Data.nActiveKind;
		nCount_aActiveIdtD = (int)Data.aActiveIdtD.GetSize();
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
	}
};
struct T_PRUD_D_CH_RW_821
{
	double    dViewPoint[3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_RW_821 DispOpt;
	int       nCount_aDimmK;
	int       nActiveKind;
	int       nCount_aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB;
	
	void Get(T_PRUD_D_CH_821 &rData)
	{
		memcpy(rData.dViewPoint, dViewPoint, sizeof(rData.dViewPoint));
		memcpy(rData.strActiveElem, strActiveElem, sizeof(rData.strActiveElem));
		memcpy(rData.strActiveNode, strActiveNode, sizeof(rData.strActiveNode));
		memcpy(rData.strGrupName, strGrupName, sizeof(rData.strGrupName));
		rData.bShrink      = bShrink;
		rData.bHidden      = bHidden;
		rData.bPerspective = bPerspective;
		DispOpt.Get(rData.DispOpt);
		rData.aDimmK.SetSize(nCount_aDimmK);
		rData.nActiveKind = nActiveKind;
		rData.aActiveIdtD.SetSize(nCount_aActiveIdtD);
		rData.RETextD = RETextD;
		rData.nUcsGcsMode = nUcsGcsMode;
		rData.UcsDefineD = UcsDefineD;
		rData.bReferenceDB = bReferenceDB;
	}
	void Set(T_PRUD_D_CH_821 Data)
	{
		memcpy(dViewPoint, Data.dViewPoint, sizeof(dViewPoint));
		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Set(Data.DispOpt);
		nCount_aDimmK = (int)Data.aDimmK.GetSize();
		nActiveKind = Data.nActiveKind;
		nCount_aActiveIdtD = (int)Data.aActiveIdtD.GetSize();
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
	}
};
struct T_PRUD_D_CH_RW_835
{
	double    dViewPoint[3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_RW_835 DispOpt;
	int       nCount_aDimmK;
	int       nActiveKind;
	int       nCount_aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB;

	void Get(T_PRUD_D_CH_835 &rData)
	{
		memcpy(rData.dViewPoint, dViewPoint, sizeof(rData.dViewPoint));
		memcpy(rData.strActiveElem, strActiveElem, sizeof(rData.strActiveElem));
		memcpy(rData.strActiveNode, strActiveNode, sizeof(rData.strActiveNode));
		memcpy(rData.strGrupName, strGrupName, sizeof(rData.strGrupName));
		rData.bShrink      = bShrink;
		rData.bHidden      = bHidden;
		rData.bPerspective = bPerspective;
		DispOpt.Get(rData.DispOpt);
		rData.aDimmK.SetSize(nCount_aDimmK);
		rData.nActiveKind = nActiveKind;
		rData.aActiveIdtD.SetSize(nCount_aActiveIdtD);
		rData.RETextD = RETextD;
		rData.nUcsGcsMode = nUcsGcsMode;
		rData.UcsDefineD = UcsDefineD;
		rData.bReferenceDB = bReferenceDB;
	}
	void Set(T_PRUD_D_CH_835 Data)
	{
		memcpy(dViewPoint, Data.dViewPoint, sizeof(dViewPoint));
		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Set(Data.DispOpt);
		nCount_aDimmK = (int)Data.aDimmK.GetSize();
		nActiveKind = Data.nActiveKind;
		nCount_aActiveIdtD = (int)Data.aActiveIdtD.GetSize();
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
	}
};
struct T_PRUD_D_CH_RW_841
{
	double    dViewPoint[3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_RW_841 DispOpt;
	int       nCount_aDimmK;
	int       nActiveKind;
	int       nCount_aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB;

	//void Get(T_PRUD_D_CH_841 &rData)
	//{
	//	memcpy(rData.dViewPoint, dViewPoint, sizeof(rData.dViewPoint));
	//	memcpy(rData.strActiveElem, strActiveElem, sizeof(rData.strActiveElem));
	//	memcpy(rData.strActiveNode, strActiveNode, sizeof(rData.strActiveNode));
	//	memcpy(rData.strGrupName, strGrupName, sizeof(rData.strGrupName));
	//	rData.bShrink      = bShrink;
	//	rData.bHidden      = bHidden;
	//	rData.bPerspective = bPerspective;
	//	DispOpt.Get(rData.DispOpt);
	//	rData.aDimmK.SetSize(nCount_aDimmK);
	//	rData.nActiveKind = nActiveKind;
	//	rData.aActiveIdtD.SetSize(nCount_aActiveIdtD);
	//	rData.RETextD = RETextD;
	//	rData.nUcsGcsMode = nUcsGcsMode;
	//	rData.UcsDefineD = UcsDefineD;
	//	rData.bReferenceDB = bReferenceDB;
	//}
	//void Set(T_PRUD_D_CH_841 Data)
	//{
	//	memcpy(dViewPoint, Data.dViewPoint, sizeof(dViewPoint));
	//	memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
	//	memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
	//	memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
	//	bShrink      = Data.bShrink;
	//	bHidden      = Data.bHidden;
	//	bPerspective = Data.bPerspective;
	//	DispOpt.Set(Data.DispOpt);
	//	nCount_aDimmK = (int)Data.aDimmK.GetSize();
	//	nActiveKind = Data.nActiveKind;
	//	nCount_aActiveIdtD = (int)Data.aActiveIdtD.GetSize();
	//	RETextD = Data.RETextD;
	//	nUcsGcsMode = Data.nUcsGcsMode;
	//	UcsDefineD = Data.UcsDefineD;
	//	bReferenceDB = Data.bReferenceDB;
	//}
};

struct T_PRUD_D_CH_RW_845
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_RW_855 DispOpt;
	int       nCount_aDimmK;
	int       nActiveKind;
	int       nCount_aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB;

	//void Get(T_PRUD_D_CH_845 &rData)
	//{
	//	memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
	//	memcpy(rData.strActiveElem, strActiveElem, sizeof(rData.strActiveElem));
	//	memcpy(rData.strActiveNode, strActiveNode, sizeof(rData.strActiveNode));
	//	memcpy(rData.strGrupName, strGrupName, sizeof(rData.strGrupName));
	//	rData.bShrink      = bShrink;
	//	rData.bHidden      = bHidden;
	//	rData.bPerspective = bPerspective;
	//	DispOpt.Get(rData.DispOpt);
	//	rData.aDimmK.SetSize(nCount_aDimmK);
	//	rData.nActiveKind = nActiveKind;
	//	rData.aActiveIdtD.SetSize(nCount_aActiveIdtD);
	//	rData.RETextD = RETextD;
	//	rData.nUcsGcsMode = nUcsGcsMode;
	//	rData.UcsDefineD = UcsDefineD;
	//	rData.bReferenceDB = bReferenceDB;
	//}
	//void Set(T_PRUD_D_CH_845 Data)
	//{
	//	memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
	//	memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
	//	memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
	//	memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
	//	bShrink      = Data.bShrink;
	//	bHidden      = Data.bHidden;
	//	bPerspective = Data.bPerspective;
	//	DispOpt.Set(Data.DispOpt);
	//	nCount_aDimmK = (int)Data.aDimmK.GetSize();
	//	nActiveKind = Data.nActiveKind;
	//	nCount_aActiveIdtD = (int)Data.aActiveIdtD.GetSize();
	//	RETextD = Data.RETextD;
	//	nUcsGcsMode = Data.nUcsGcsMode;
	//	UcsDefineD = Data.UcsDefineD;
	//	bReferenceDB = Data.bReferenceDB;
	//}
};

struct T_PRUD_D_CH_RW_855
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_RW_855 DispOpt;
	int       nCount_aDimmK;
	int       nActiveKind;
	int       nCount_aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB;
	int       nUnitLength;

	//void Get(T_PRUD_D_CH_855 &rData)
	//{
	//	memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
	//	memcpy(rData.strActiveElem, strActiveElem, sizeof(rData.strActiveElem));
	//	memcpy(rData.strActiveNode, strActiveNode, sizeof(rData.strActiveNode));
	//	memcpy(rData.strGrupName, strGrupName, sizeof(rData.strGrupName));
	//	rData.bShrink      = bShrink;
	//	rData.bHidden      = bHidden;
	//	rData.bPerspective = bPerspective;
	//	DispOpt.Get(rData.DispOpt);
	//	rData.aDimmK.SetSize(nCount_aDimmK);
	//	rData.nActiveKind = nActiveKind;
	//	rData.aActiveIdtD.SetSize(nCount_aActiveIdtD);
	//	rData.RETextD = RETextD;
	//	rData.nUcsGcsMode = nUcsGcsMode;
	//	rData.UcsDefineD = UcsDefineD;
	//	rData.bReferenceDB = bReferenceDB;
	//	rData.nUnitLength = nUnitLength;		
	//}
	//void Set(T_PRUD_D_CH_855 Data)
	//{
	//	memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
	//	memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
	//	memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
	//	memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
	//	bShrink      = Data.bShrink;
	//	bHidden      = Data.bHidden;
	//	bPerspective = Data.bPerspective;
	//	DispOpt.Set(Data.DispOpt);
	//	nCount_aDimmK = (int)Data.aDimmK.GetSize();
	//	nActiveKind = Data.nActiveKind;
	//	nCount_aActiveIdtD = (int)Data.aActiveIdtD.GetSize();
	//	RETextD = Data.RETextD;
	//	nUcsGcsMode = Data.nUcsGcsMode;
	//	UcsDefineD = Data.UcsDefineD;
	//	bReferenceDB = Data.bReferenceDB;
	//	nUnitLength = Data.nUnitLength;
	//}
};

struct T_PRUD_D_CH_RW_860
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_RW_860 DispOpt;
	int       nCount_aDimmK;
	int       nActiveKind;
	int       nCount_aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB;
	int       nUnitLength;

	//void Get(T_PRUD_D_CH_860 &rData)
	//{
	//	memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
	//	memcpy(rData.strActiveElem, strActiveElem, sizeof(rData.strActiveElem));
	//	memcpy(rData.strActiveNode, strActiveNode, sizeof(rData.strActiveNode));
	//	memcpy(rData.strGrupName, strGrupName, sizeof(rData.strGrupName));
	//	rData.bShrink      = bShrink;
	//	rData.bHidden      = bHidden;
	//	rData.bPerspective = bPerspective;
	//	DispOpt.Get(rData.DispOpt);
	//	rData.aDimmK.SetSize(nCount_aDimmK);
	//	rData.nActiveKind = nActiveKind;
	//	rData.aActiveIdtD.SetSize(nCount_aActiveIdtD);
	//	rData.RETextD = RETextD;
	//	rData.nUcsGcsMode = nUcsGcsMode;
	//	rData.UcsDefineD = UcsDefineD;
	//	rData.bReferenceDB = bReferenceDB;
	//	rData.nUnitLength = nUnitLength;		
	//}
	//void Set(T_PRUD_D_CH_860 Data)
	//{
	//	memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
	//	memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
	//	memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
	//	memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
	//	bShrink      = Data.bShrink;
	//	bHidden      = Data.bHidden;
	//	bPerspective = Data.bPerspective;
	//	DispOpt.Set(Data.DispOpt);
	//	nCount_aDimmK = (int)Data.aDimmK.GetSize();
	//	nActiveKind = Data.nActiveKind;
	//	nCount_aActiveIdtD = (int)Data.aActiveIdtD.GetSize();
	//	RETextD = Data.RETextD;
	//	nUcsGcsMode = Data.nUcsGcsMode;
	//	UcsDefineD = Data.UcsDefineD;
	//	bReferenceDB = Data.bReferenceDB;
	//	nUnitLength = Data.nUnitLength;
	//}
};

struct T_PRUD_D_CH_RW_865
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_RW_865 DispOpt;
	int       nCount_aDimmK;
	int       nActiveKind;
	int       nCount_aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB;
	int       nUnitLength;

	//void Get(T_PRUD_D_CH_865 &rData)
	//{
	//	memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
	//	memcpy(rData.strActiveElem, strActiveElem, sizeof(rData.strActiveElem));
	//	memcpy(rData.strActiveNode, strActiveNode, sizeof(rData.strActiveNode));
	//	memcpy(rData.strGrupName, strGrupName, sizeof(rData.strGrupName));
	//	rData.bShrink      = bShrink;
	//	rData.bHidden      = bHidden;
	//	rData.bPerspective = bPerspective;
	//	DispOpt.Get(rData.DispOpt);
	//	rData.aDimmK.SetSize(nCount_aDimmK);
	//	rData.nActiveKind = nActiveKind;
	//	rData.aActiveIdtD.SetSize(nCount_aActiveIdtD);
	//	rData.RETextD = RETextD;
	//	rData.nUcsGcsMode = nUcsGcsMode;
	//	rData.UcsDefineD = UcsDefineD;
	//	rData.bReferenceDB = bReferenceDB;
	//	rData.nUnitLength = nUnitLength;		
	//}
	//void Set(T_PRUD_D_CH_865 Data)
	//{
	//	memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
	//	memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
	//	memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
	//	memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
	//	bShrink      = Data.bShrink;
	//	bHidden      = Data.bHidden;
	//	bPerspective = Data.bPerspective;
	//	DispOpt.Set(Data.DispOpt);
	//	nCount_aDimmK = (int)Data.aDimmK.GetSize();
	//	nActiveKind = Data.nActiveKind;
	//	nCount_aActiveIdtD = (int)Data.aActiveIdtD.GetSize();
	//	RETextD = Data.RETextD;
	//	nUcsGcsMode = Data.nUcsGcsMode;
	//	UcsDefineD = Data.UcsDefineD;
	//	bReferenceDB = Data.bReferenceDB;
	//	nUnitLength = Data.nUnitLength;
	//}
};
struct T_PRUD_D_CH_RW_870
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_RW_870 DispOpt;
	int       nCount_aDimmK;
	int       nActiveKind;
	int       nCount_aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB;
	int       nUnitLength;

	//void Get(T_PRUD_D_CH_870 &rData)
	//{
	//	memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
	//	memcpy(rData.strActiveElem, strActiveElem, sizeof(rData.strActiveElem));
	//	memcpy(rData.strActiveNode, strActiveNode, sizeof(rData.strActiveNode));
	//	memcpy(rData.strGrupName, strGrupName, sizeof(rData.strGrupName));
	//	rData.bShrink      = bShrink;
	//	rData.bHidden      = bHidden;
	//	rData.bPerspective = bPerspective;
	//	DispOpt.Get(rData.DispOpt);
	//	rData.aDimmK.SetSize(nCount_aDimmK);
	//	rData.nActiveKind = nActiveKind;
	//	rData.aActiveIdtD.SetSize(nCount_aActiveIdtD);
	//	rData.RETextD = RETextD;
	//	rData.nUcsGcsMode = nUcsGcsMode;
	//	rData.UcsDefineD = UcsDefineD;
	//	rData.bReferenceDB = bReferenceDB;
	//	rData.nUnitLength = nUnitLength;		
	//}
	//void Set(T_PRUD_D_CH_870 Data)
	//{
	//	memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
	//	memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
	//	memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
	//	memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
	//	bShrink      = Data.bShrink;
	//	bHidden      = Data.bHidden;
	//	bPerspective = Data.bPerspective;
	//	DispOpt.Set(Data.DispOpt);
	//	nCount_aDimmK = (int)Data.aDimmK.GetSize();
	//	nActiveKind = Data.nActiveKind;
	//	nCount_aActiveIdtD = (int)Data.aActiveIdtD.GetSize();
	//	RETextD = Data.RETextD;
	//	nUcsGcsMode = Data.nUcsGcsMode;
	//	UcsDefineD = Data.UcsDefineD;
	//	bReferenceDB = Data.bReferenceDB;
	//	nUnitLength = Data.nUnitLength;
	//}
};
struct T_PRUD_D_CH_RW_883
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_RW_883 DispOpt;
	int       nCount_aDimmK;
	int       nActiveKind;
	int       nCount_aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB;
	int       nUnitLength;


	//void Get(T_PRUD_D_CH_883 &rData)
	//{
	//	memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
	//	memcpy(rData.strActiveElem, strActiveElem, sizeof(rData.strActiveElem));
	//	memcpy(rData.strActiveNode, strActiveNode, sizeof(rData.strActiveNode));
	//	memcpy(rData.strGrupName, strGrupName, sizeof(rData.strGrupName));
	//	rData.bShrink      = bShrink;
	//	rData.bHidden      = bHidden;
	//	rData.bPerspective = bPerspective;
	//	DispOpt.Get(rData.DispOpt);
	//	rData.aDimmK.SetSize(nCount_aDimmK);
	//	rData.nActiveKind = nActiveKind;
	//	rData.aActiveIdtD.SetSize(nCount_aActiveIdtD);
	//	rData.RETextD = RETextD;
	//	rData.nUcsGcsMode = nUcsGcsMode;
	//	rData.UcsDefineD = UcsDefineD;
	//	rData.bReferenceDB = bReferenceDB;
	//	rData.nUnitLength = nUnitLength;
	//}
	//void Set(T_PRUD_D_CH_883 Data)
	//{
	//	memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
	//	memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
	//	memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
	//	memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
	//	bShrink      = Data.bShrink;
	//	bHidden      = Data.bHidden;
	//	bPerspective = Data.bPerspective;
	//	DispOpt.Set(Data.DispOpt);
	//	nCount_aDimmK = (int)Data.aDimmK.GetSize();
	//	nActiveKind = Data.nActiveKind;
	//	nCount_aActiveIdtD = (int)Data.aActiveIdtD.GetSize();
	//	RETextD = Data.RETextD;
	//	nUcsGcsMode = Data.nUcsGcsMode;
	//	UcsDefineD = Data.UcsDefineD;
	//	bReferenceDB = Data.bReferenceDB;
	//	nUnitLength = Data.nUnitLength;
	//}
};

struct T_PRUD_D_CH_RW_885
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_RW_885 DispOpt;
	int       nCount_aDimmK;
	int       nActiveKind;
	int       nCount_aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB;
	int       nUnitLength;

// 	void Get(T_PRUD_D_CH_885 &rData)
// 	{
// 		memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
// 		memcpy(rData.strActiveElem, strActiveElem, sizeof(rData.strActiveElem));
// 		memcpy(rData.strActiveNode, strActiveNode, sizeof(rData.strActiveNode));
// 		memcpy(rData.strGrupName, strGrupName, sizeof(rData.strGrupName));
// 		rData.bShrink      = bShrink;
// 		rData.bHidden      = bHidden;
// 		rData.bPerspective = bPerspective;
// 		DispOpt.Get(rData.DispOpt);
// 		rData.aDimmK.SetSize(nCount_aDimmK);
// 		rData.nActiveKind = nActiveKind;
// 		rData.aActiveIdtD.SetSize(nCount_aActiveIdtD);
// 		rData.RETextD = RETextD;
// 		rData.nUcsGcsMode = nUcsGcsMode;
// 		rData.UcsDefineD = UcsDefineD;
// 		rData.bReferenceDB = bReferenceDB;
// 		rData.nUnitLength = nUnitLength;
// 	}
// 	void Set(T_PRUD_D_CH_885 Data)
// 	{
// 		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
// 		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
// 		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
// 		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
// 		bShrink      = Data.bShrink;
// 		bHidden      = Data.bHidden;
// 		bPerspective = Data.bPerspective;
// 		DispOpt.Set(Data.DispOpt);
// 		nCount_aDimmK = (int)Data.aDimmK.GetSize();
// 		nActiveKind = Data.nActiveKind;
// 		nCount_aActiveIdtD = (int)Data.aActiveIdtD.GetSize();
// 		RETextD = Data.RETextD;
// 		nUcsGcsMode = Data.nUcsGcsMode;
// 		UcsDefineD = Data.UcsDefineD;
// 		bReferenceDB = Data.bReferenceDB;
// 		nUnitLength = Data.nUnitLength;
// 	}
};

struct T_PRUD_D_CH_RW_891
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_RW_891 DispOpt;
	int       nCount_aDimmK;
	int       nActiveKind;
	int       nCount_aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB;
	int       nUnitLength;

// 	void Get(T_PRUD_D_CH_891 &rData)
// 	{
// 		memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
// 		memcpy(rData.strActiveElem, strActiveElem, sizeof(rData.strActiveElem));
// 		memcpy(rData.strActiveNode, strActiveNode, sizeof(rData.strActiveNode));
// 		memcpy(rData.strGrupName, strGrupName, sizeof(rData.strGrupName));
// 		rData.bShrink      = bShrink;
// 		rData.bHidden      = bHidden;
// 		rData.bPerspective = bPerspective;
// 		DispOpt.Get(rData.DispOpt);
// 		rData.aDimmK.SetSize(nCount_aDimmK);
// 		rData.nActiveKind = nActiveKind;
// 		rData.aActiveIdtD.SetSize(nCount_aActiveIdtD);
// 		rData.RETextD = RETextD;
// 		rData.nUcsGcsMode = nUcsGcsMode;
// 		rData.UcsDefineD = UcsDefineD;
// 		rData.bReferenceDB = bReferenceDB;
// 		rData.nUnitLength = nUnitLength;
// 	}
// 	void Set(T_PRUD_D_CH_891 Data)
// 	{
// 		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
// 		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
// 		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
// 		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
// 		bShrink      = Data.bShrink;
// 		bHidden      = Data.bHidden;
// 		bPerspective = Data.bPerspective;
// 		DispOpt.Set(Data.DispOpt);
// 		nCount_aDimmK = (int)Data.aDimmK.GetSize();
// 		nActiveKind = Data.nActiveKind;
// 		nCount_aActiveIdtD = (int)Data.aActiveIdtD.GetSize();
// 		RETextD = Data.RETextD;
// 		nUcsGcsMode = Data.nUcsGcsMode;
// 		UcsDefineD = Data.UcsDefineD;
// 		bReferenceDB = Data.bReferenceDB;
// 		nUnitLength = Data.nUnitLength;
// 	}
};
struct T_PRUD_D_CH_RW_895
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_RW_895 DispOpt;
	int       nCount_aDimmK;
	int       nActiveKind;
	int       nCount_aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD;
	BOOL      bReferenceDB;
	int       nUnitLength;

// 	void Get(T_PRUD_D_CH_895 &rData)
// 	{
// 		memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
// 		memcpy(rData.strActiveElem, strActiveElem, sizeof(rData.strActiveElem));
// 		memcpy(rData.strActiveNode, strActiveNode, sizeof(rData.strActiveNode));
// 		memcpy(rData.strGrupName, strGrupName, sizeof(rData.strGrupName));
// 		rData.bShrink = bShrink;
// 		rData.bHidden = bHidden;
// 		rData.bPerspective = bPerspective;
// 		DispOpt.Get(rData.DispOpt);
// 		rData.aDimmK.SetSize(nCount_aDimmK);
// 		rData.nActiveKind = nActiveKind;
// 		rData.aActiveIdtD.SetSize(nCount_aActiveIdtD);
// 		rData.RETextD = RETextD;
// 		rData.nUcsGcsMode = nUcsGcsMode;
// 		rData.UcsDefineD = UcsDefineD;
// 		rData.bReferenceDB = bReferenceDB;
// 		rData.nUnitLength = nUnitLength;
// 	}
// 	void Set(T_PRUD_D_CH_895 Data)
// 	{
// 		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
// 		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
// 		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
// 		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
// 		bShrink = Data.bShrink;
// 		bHidden = Data.bHidden;
// 		bPerspective = Data.bPerspective;
// 		DispOpt.Set(Data.DispOpt);
// 		nCount_aDimmK = (int)Data.aDimmK.GetSize();
// 		nActiveKind = Data.nActiveKind;
// 		nCount_aActiveIdtD = (int)Data.aActiveIdtD.GetSize();
// 		RETextD = Data.RETextD;
// 		nUcsGcsMode = Data.nUcsGcsMode;
// 		UcsDefineD = Data.UcsDefineD;
// 		bReferenceDB = Data.bReferenceDB;
// 		nUnitLength = Data.nUnitLength;
// 	}
};

struct T_PRUD_D_CH_RW_925
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_RW_925 DispOpt;
	int       nCount_aDimmK;
	int       nActiveKind;
	int       nCount_aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD;
	BOOL      bReferenceDB;
	int       nUnitLength;

// 	void Get(T_PRUD_D_CH_925 &rData)
// 	{
// 		memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
// 		memcpy(rData.strActiveElem, strActiveElem, sizeof(rData.strActiveElem));
// 		memcpy(rData.strActiveNode, strActiveNode, sizeof(rData.strActiveNode));
// 		memcpy(rData.strGrupName, strGrupName, sizeof(rData.strGrupName));
// 		rData.bShrink = bShrink;
// 		rData.bHidden = bHidden;
// 		rData.bPerspective = bPerspective;
// 		DispOpt.Get(rData.DispOpt);
// 		rData.aDimmK.SetSize(nCount_aDimmK);
// 		rData.nActiveKind = nActiveKind;
// 		rData.aActiveIdtD.SetSize(nCount_aActiveIdtD);
// 		rData.RETextD = RETextD;
// 		rData.nUcsGcsMode = nUcsGcsMode;
// 		rData.UcsDefineD = UcsDefineD;
// 		rData.bReferenceDB = bReferenceDB;
// 		rData.nUnitLength = nUnitLength;
// 	}
// 	void Set(T_PRUD_D_CH_925 Data)
// 	{
// 		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
// 		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
// 		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
// 		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
// 		bShrink = Data.bShrink;
// 		bHidden = Data.bHidden;
// 		bPerspective = Data.bPerspective;
// 		DispOpt.Set(Data.DispOpt);
// 		nCount_aDimmK = (int)Data.aDimmK.GetSize();
// 		nActiveKind = Data.nActiveKind;
// 		nCount_aActiveIdtD = (int)Data.aActiveIdtD.GetSize();
// 		RETextD = Data.RETextD;
// 		nUcsGcsMode = Data.nUcsGcsMode;
// 		UcsDefineD = Data.UcsDefineD;
// 		bReferenceDB = Data.bReferenceDB;
// 		nUnitLength = Data.nUnitLength;
// 	}
};
struct T_PRUD_D_CH_RW_930
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_RW_930 DispOpt;
	int       nCount_aDimmK;
	int       nActiveKind;
	int       nCount_aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD;
	BOOL      bReferenceDB;
	int       nUnitLength;

// 	void Get(T_PRUD_D_CH_930 &rData)
// 	{
// 		memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
// 		memcpy(rData.strActiveElem, strActiveElem, sizeof(rData.strActiveElem));
// 		memcpy(rData.strActiveNode, strActiveNode, sizeof(rData.strActiveNode));
// 		memcpy(rData.strGrupName, strGrupName, sizeof(rData.strGrupName));
// 		rData.bShrink = bShrink;
// 		rData.bHidden = bHidden;
// 		rData.bPerspective = bPerspective;
// 		DispOpt.Get(rData.DispOpt);
// 		rData.aDimmK.SetSize(nCount_aDimmK);
// 		rData.nActiveKind = nActiveKind;
// 		rData.aActiveIdtD.SetSize(nCount_aActiveIdtD);
// 		rData.RETextD = RETextD;
// 		rData.nUcsGcsMode = nUcsGcsMode;
// 		rData.UcsDefineD = UcsDefineD;
// 		rData.bReferenceDB = bReferenceDB;
// 		rData.nUnitLength = nUnitLength;
// 	}
// 	void Set(T_PRUD_D_CH_930 Data)
// 	{
// 		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
// 		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
// 		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
// 		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
// 		bShrink = Data.bShrink;
// 		bHidden = Data.bHidden;
// 		bPerspective = Data.bPerspective;
// 		DispOpt.Set(Data.DispOpt);
// 		nCount_aDimmK = (int)Data.aDimmK.GetSize();
// 		nActiveKind = Data.nActiveKind;
// 		nCount_aActiveIdtD = (int)Data.aActiveIdtD.GetSize();
// 		RETextD = Data.RETextD;
// 		nUcsGcsMode = Data.nUcsGcsMode;
// 		UcsDefineD = Data.UcsDefineD;
// 		bReferenceDB = Data.bReferenceDB;
// 		nUnitLength = Data.nUnitLength;
// 	}
};

struct T_PRUD_D_CH_RW_940
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_RW_940 DispOpt;
	int       nCount_aDimmK;
	int       nActiveKind;
	int       nCount_aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD;
	BOOL      bReferenceDB;
	int       nUnitLength;
};

struct T_PRUD_D_CH_RW_950
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_RW_950 DispOpt;
	int       nCount_aDimmK;
	int       nActiveKind;
	int       nCount_aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD;
	BOOL      bReferenceDB;
	int       nUnitLength;
};

struct T_PRUD_D_CH_RW_950_2
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_RW_950_2 DispOpt;
	int       nCount_aDimmK;
	int       nActiveKind;
	int       nCount_aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD;
	BOOL      bReferenceDB;
	int       nUnitLength;
};

struct T_PRUD_D_CH_RW_955
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_RW_955 DispOpt;
	int       nCount_aDimmK;
	int       nActiveKind;
	int       nCount_aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD;
	BOOL      bReferenceDB;
	int       nUnitLength;
};

struct T_PRUD_D_CH_RW_965
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_RW_965 DispOpt;
	int       nCount_aDimmK;
	int       nActiveKind;
	int       nCount_aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD;
	BOOL      bReferenceDB;
	int       nUnitLength;
};

struct T_PRUD_D_CH_RW
{
	double    VCSVector[3][3];
	char      strActiveElem[CHAR_ACTIVE_MAX];
	char      strActiveNode[CHAR_ACTIVE_MAX];
	BOOL      bShrink;
	BOOL      bHidden;
	BOOL      bPerspective;
	T_DPST_D_CH_RW DispOpt;
	int       nCount_aDimmK;
	int       nActiveKind;
	int       nCount_aActiveIdtD;
	char      strGrupName[60];
	T_RETextMod_D RETextD;
	int       nUcsGcsMode;
	T_GUcsDefineData_D UcsDefineD; 
	BOOL      bReferenceDB;
	int       nUnitLength;
	
	void Get(T_PRUD_D_CH &rData)
	{
		memcpy(rData.VCSVector, VCSVector, sizeof(rData.VCSVector));
		memcpy(rData.strActiveElem, strActiveElem, sizeof(rData.strActiveElem));
		memcpy(rData.strActiveNode, strActiveNode, sizeof(rData.strActiveNode));
		memcpy(rData.strGrupName, strGrupName, sizeof(rData.strGrupName));
		rData.bShrink      = bShrink;
		rData.bHidden      = bHidden;
		rData.bPerspective = bPerspective;
		DispOpt.Get(rData.DispOpt);
		rData.aDimmK.SetSize(nCount_aDimmK);
		rData.nActiveKind = nActiveKind;
		rData.aActiveIdtD.SetSize(nCount_aActiveIdtD);
		rData.RETextD = RETextD;
		rData.nUcsGcsMode = nUcsGcsMode;
		rData.UcsDefineD = UcsDefineD;
		rData.bReferenceDB = bReferenceDB;
		rData.nUnitLength = nUnitLength;
	}
	void Set(T_PRUD_D_CH Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Set(Data.DispOpt);
		nCount_aDimmK = (int)Data.aDimmK.GetSize();
		nActiveKind = Data.nActiveKind;
		nCount_aActiveIdtD = (int)Data.aActiveIdtD.GetSize();
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}
	void Convert790(T_PRUD_D_CH_RW_790& Data)
	{
		memset(VCSVector,0,sizeof(VCSVector));
		memcpy(VCSVector[2],Data.dViewPoint,sizeof(Data.dViewPoint));
		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Convert810(Data.DispOpt);
		nCount_aDimmK = Data.nCount_aDimmK;
		nActiveKind = Data.nActiveKind;
		nCount_aActiveIdtD = Data.nCount_aActiveIdtD;
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = FALSE;
	}
	void Convert810(T_PRUD_D_CH_RW_810& Data)
	{
		memset(VCSVector,0,sizeof(VCSVector));
		memcpy(VCSVector[2],Data.dViewPoint,sizeof(Data.dViewPoint));
		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Convert810(Data.DispOpt);
		nCount_aDimmK = Data.nCount_aDimmK;
		nActiveKind = Data.nActiveKind;
		nCount_aActiveIdtD = Data.nCount_aActiveIdtD;
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
	}
	void Convert821(T_PRUD_D_CH_RW_821& Data)
	{
		memset(VCSVector,0,sizeof(VCSVector));
		memcpy(VCSVector[2],Data.dViewPoint,sizeof(Data.dViewPoint));
		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Convert821(Data.DispOpt);
		nCount_aDimmK = Data.nCount_aDimmK;
		nActiveKind = Data.nActiveKind;
		nCount_aActiveIdtD = Data.nCount_aActiveIdtD;
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
	}
	void Convert835(T_PRUD_D_CH_RW_835& Data)
	{
		memset(VCSVector,0,sizeof(VCSVector));
		memcpy(VCSVector[2],Data.dViewPoint,sizeof(Data.dViewPoint));
		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Convert835(Data.DispOpt);
		nCount_aDimmK = Data.nCount_aDimmK;
		nActiveKind = Data.nActiveKind;
		nCount_aActiveIdtD = Data.nCount_aActiveIdtD;
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
	}
	void Convert841(T_PRUD_D_CH_RW_841& Data)
	{
		memset(VCSVector,0,sizeof(VCSVector));
		memcpy(VCSVector[2],Data.dViewPoint,sizeof(Data.dViewPoint));
		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Convert841(Data.DispOpt);
		nCount_aDimmK = Data.nCount_aDimmK;
		nActiveKind = Data.nActiveKind;
		nCount_aActiveIdtD = Data.nCount_aActiveIdtD;
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
	}
	void Convert845(T_PRUD_D_CH_RW_845& Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Convert855(Data.DispOpt);
		nCount_aDimmK = Data.nCount_aDimmK;
		nActiveKind = Data.nActiveKind;
		nCount_aActiveIdtD = Data.nCount_aActiveIdtD;
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = -1;
	}
	void Convert855(T_PRUD_D_CH_RW_855& Data)
	{
	  memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
	  memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
	  memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
	  memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
	  bShrink      = Data.bShrink;
	  bHidden      = Data.bHidden;
	  bPerspective = Data.bPerspective;
	  DispOpt.Convert855(Data.DispOpt);
	  nCount_aDimmK = Data.nCount_aDimmK;
	  nActiveKind = Data.nActiveKind;
	  nCount_aActiveIdtD = Data.nCount_aActiveIdtD;
	  RETextD = Data.RETextD;
	  nUcsGcsMode = Data.nUcsGcsMode;
	  UcsDefineD = Data.UcsDefineD;
	  bReferenceDB = Data.bReferenceDB;
	  nUnitLength = Data.nUnitLength;
	}
	void Convert860(T_PRUD_D_CH_RW_860& Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Convert860(Data.DispOpt);
		nCount_aDimmK = Data.nCount_aDimmK;
		nActiveKind = Data.nActiveKind;
		nCount_aActiveIdtD = Data.nCount_aActiveIdtD;
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}
	void Convert865(T_PRUD_D_CH_RW_865& Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Convert865(Data.DispOpt);
		nCount_aDimmK = Data.nCount_aDimmK;
		nActiveKind = Data.nActiveKind;
		nCount_aActiveIdtD = Data.nCount_aActiveIdtD;
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}
	void Convert870(T_PRUD_D_CH_RW_870& Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Convert870(Data.DispOpt);
		nCount_aDimmK = Data.nCount_aDimmK;
		nActiveKind = Data.nActiveKind;
		nCount_aActiveIdtD = Data.nCount_aActiveIdtD;
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}
	void Convert883(T_PRUD_D_CH_RW_883& Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Convert883(Data.DispOpt);
		nCount_aDimmK = Data.nCount_aDimmK;
		nActiveKind = Data.nActiveKind;
		nCount_aActiveIdtD = Data.nCount_aActiveIdtD;
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}

	void Convert885(T_PRUD_D_CH_RW_885& Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Convert885(Data.DispOpt);
		nCount_aDimmK = Data.nCount_aDimmK;
		nActiveKind = Data.nActiveKind;
		nCount_aActiveIdtD = Data.nCount_aActiveIdtD;
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}

	void Convert891(T_PRUD_D_CH_RW_891& Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		bShrink      = Data.bShrink;
		bHidden      = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Convert891(Data.DispOpt);
		nCount_aDimmK = Data.nCount_aDimmK;
		nActiveKind = Data.nActiveKind;
		nCount_aActiveIdtD = Data.nCount_aActiveIdtD;
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}

	void Convert895(T_PRUD_D_CH_RW_895& Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		bShrink = Data.bShrink;
		bHidden = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Convert895(Data.DispOpt);
		nCount_aDimmK = Data.nCount_aDimmK;
		nActiveKind = Data.nActiveKind;
		nCount_aActiveIdtD = Data.nCount_aActiveIdtD;
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}
	void Convert925(T_PRUD_D_CH_RW_925& Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		bShrink = Data.bShrink;
		bHidden = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Convert925(Data.DispOpt);
		nCount_aDimmK = Data.nCount_aDimmK;
		nActiveKind = Data.nActiveKind;
		nCount_aActiveIdtD = Data.nCount_aActiveIdtD;
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}
	void Convert930(T_PRUD_D_CH_RW_930& Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		bShrink = Data.bShrink;
		bHidden = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Convert930(Data.DispOpt);
		nCount_aDimmK = Data.nCount_aDimmK;
		nActiveKind = Data.nActiveKind;
		nCount_aActiveIdtD = Data.nCount_aActiveIdtD;
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}
	void Convert940(T_PRUD_D_CH_RW_940& Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		bShrink = Data.bShrink;
		bHidden = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Convert940(Data.DispOpt);
		nCount_aDimmK = Data.nCount_aDimmK;
		nActiveKind = Data.nActiveKind;
		nCount_aActiveIdtD = Data.nCount_aActiveIdtD;
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}
	void Convert950(T_PRUD_D_CH_RW_950& Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		bShrink = Data.bShrink;
		bHidden = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Convert950(Data.DispOpt);
		nCount_aDimmK = Data.nCount_aDimmK;
		nActiveKind = Data.nActiveKind;
		nCount_aActiveIdtD = Data.nCount_aActiveIdtD;
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}
	void Convert950_2(T_PRUD_D_CH_RW_950_2& Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		bShrink = Data.bShrink;
		bHidden = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Convert950_2(Data.DispOpt);
		nCount_aDimmK = Data.nCount_aDimmK;
		nActiveKind = Data.nActiveKind;
		nCount_aActiveIdtD = Data.nCount_aActiveIdtD;
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}
	void Convert955(T_PRUD_D_CH_RW_955& Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		bShrink = Data.bShrink;
		bHidden = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Convert955(Data.DispOpt);
		nCount_aDimmK = Data.nCount_aDimmK;
		nActiveKind = Data.nActiveKind;
		nCount_aActiveIdtD = Data.nCount_aActiveIdtD;
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}
	void Convert965(T_PRUD_D_CH_RW_965& Data)
	{
		memcpy(VCSVector, Data.VCSVector, sizeof(VCSVector));
		memcpy(strActiveElem, Data.strActiveElem, sizeof(strActiveElem));
		memcpy(strActiveNode, Data.strActiveNode, sizeof(strActiveNode));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		bShrink = Data.bShrink;
		bHidden = Data.bHidden;
		bPerspective = Data.bPerspective;
		DispOpt.Convert965(Data.DispOpt);
		nCount_aDimmK = Data.nCount_aDimmK;
		nActiveKind = Data.nActiveKind;
		nCount_aActiveIdtD = Data.nCount_aActiveIdtD;
		RETextD = Data.RETextD;
		nUcsGcsMode = Data.nUcsGcsMode;
		UcsDefineD = Data.UcsDefineD;
		bReferenceDB = Data.bReferenceDB;
		nUnitLength = Data.nUnitLength;
	}
};
///// post figure info.
enum EN_FDSP_D
{
	/////////////////////// 파일에 저장되는 것이므로 변경시 버젼 분리해줄것!! ///////////////////////
	EN_FDSP_D_bAnimate                      ,
	EN_FDSP_D_bContour                      ,
	EN_FDSP_D_bDrawContourEdge              , // Draw Contour Lines
	EN_FDSP_D_bGradient                     , // Gradient Fill
	EN_FDSP_D_bCoarseContour                , // Coarse Contour
	EN_FDSP_D_bExtrude                      , // Extrude
	EN_FDSP_D_bContourAnnotation            , // Contour Annotation
	EN_FDSP_D_bMonoLine                     , // Mono Line
	EN_FDSP_D_bReverse                      , // Reverse Contour
	EN_FDSP_D_bLegend                       ,
	EN_FDSP_D_bDeform                       ,
	EN_FDSP_D_bUndeformed                   ,
	EN_FDSP_D_bRelativeDispl                , // Relative Displacement
	EN_FDSP_D_bRealDispl                    , // Real Displacement
	EN_FDSP_D_bRealDeform                   , // FALSE: Nodal Deform, TRUE: Real Deform
	EN_FDSP_D_bDrawInfs3dVertLine           , // 3D Cont, Draw vertical lines
	EN_FDSP_D_bValue                        ,
	EN_FDSP_D_bValueNumberExponent          ,
	EN_FDSP_D_bValueExponential             ,
	EN_FDSP_D_bValueOutputMinMaxValueOnly   ,
	EN_FDSP_D_bMirrored                     ,
	EN_FDSP_D_bMdShp                        ,
	EN_FDSP_D_bMdShpContour                 ,
	EN_FDSP_D_bDrawMVLTRCLoad               , // Applied Loads(Moving Load Tracer)
	EN_FDSP_D_bYieldStatus                  , // Pushover Hinge
	EN_FDSP_D_bcon_YieldStatusBeam          ,
	EN_FDSP_D_bcon_YieldStatusWall          ,
	EN_FDSP_D_bcon_YieldStatusTruss         ,
	EN_FDSP_D_bcon_YieldStatusLink          ,
		
	EN_FDSP_D_END
};
enum EN_FOUT_D
{
	/////////////////////// 파일에 저장되는 것이므로 변경시 버젼 분리해줄것!! ///////////////////////
	EN_FOUT_D_bBForcOutputMax               ,
	EN_FOUT_D_bBForcOutputMinMax            ,
	EN_FOUT_D_bBForcOutputAll               ,
	EN_FOUT_D_bBForcOutputI                 ,
	EN_FOUT_D_bBForcOutputC                 ,
	EN_FOUT_D_bBForcOutputJ                 ,
	EN_FOUT_D_bBSfdBmdExact                 ,
	EN_FOUT_D_bBSfdBmdLineOnly              ,
	EN_FOUT_D_bVerticalLineFill             ,
	EN_FOUT_D_bWSfdBmdLineOnly              ,
	EN_FOUT_D_bAverage                      ,
	EN_FOUT_D_bAverageNodalActiveOnly       ,
	EN_FOUT_D_bPrintUCSAxis                 ,
	EN_FOUT_D_bVectorPositive               ,
	EN_FOUT_D_bVectorNegative               ,
	EN_FOUT_D_bMbDgrmBeamChk                ,
	EN_FOUT_D_bMbDgrmBrceChk                ,
	EN_FOUT_D_bMbDgrmColmChk                ,
	EN_FOUT_D_bMbDgrmWallChk                ,
	EN_FOUT_D_bMbDgrmExpChk                 ,
	EN_FOUT_D_bMbDgrmShowDgrm               ,
	EN_FOUT_D_bMbDgrmValAxial               ,
	EN_FOUT_D_bMbDgrmValShear               ,
	EN_FOUT_D_bMbDgrmValMoment              ,
	EN_FOUT_D_bBForcWithTForc               ,
	EN_FOUT_D_bBForcOnlyTForc               ,
	EN_FOUT_D_bMaxMinDgrm                   ,
	EN_FOUT_D_bPfudRvrs                     , // Plate Cutting Line Diagram (Reverse)
	EN_FOUT_D_bPfudValOutput                , // Plate Cutting Line Diagram (Value Output)
	EN_FOUT_D_bPfudMnMxOnly                 , // Plate Cutting Line Diagram (MinMax Only)
																					
	EN_FOUT_D_END
};

enum class LOADCASE_TYPE : unsigned int
{
	HISTORY = 0,
	JSONFILE,
	NONE
};

// MQC 5480 : Moving Load Tracer 스마트리포트용 옵션 추가
struct T_FDSP_D_821
{
	double  dContAnnotSpacing          ;  // Contour Annotation Spacing
	int     nCurrentRankNum            ;  // Number of Colors
	int     nCurrentColorTable         ;  // Color Table
	COLORREF colEdgeColor              ;  // Contour Line
	COLORREF colElemColor              ;  // Element Edge
	int     nContourFill               ;  // 0: Contour Fill, 1: Draw Contour Line Only
	double  dExtrudeFactor             ;  // Extrude scale
	int     nLegendPosition            ;
	int     nLegendDeciPt              ;
	int     nLegendValType             ;
	double  aRankMap[30]               ;  // Legend rank values (CContourEngine의 ECONTOUR_RANK_NUM)
	double  dDispScaleFactor           ;  // Deformation Scale Factor
	int     nValueNumberDecimalPoint   ;
	UINT    nValueOutputMinMaxType     ;
	double  dValueOutputMinMaxLimit    ;
	UINT    nValueSpecifiedAngle       ;
	int     nValueValueAngle           ;
	int     nMirrorState               ;
	int     nMirrorPlane[2]            ;
	double  dMirrorPlnLocation[2]      ;
	BOOL    bFdspD[EN_FDSP_D_END]      ;
	// Applied Loads(Moving Load Tracer)
	double  dMVLTRCScaleFactor          ;
	BOOL    bMVLTRCValue                ;
	int     nMVLTRCValueType            ;
	int     nMVLTRCValueDePt            ;
	
	void Initialize()
	{
		dContAnnotSpacing          = 0.;
		nCurrentRankNum            = 0;
		nCurrentColorTable         = 0;
		colEdgeColor               = 0;
		colElemColor               = 0;
		nContourFill               = 0;
		dExtrudeFactor             = 0.;
		nLegendPosition            = 0;
		nLegendDeciPt              = 0;
		nLegendValType             = 0;
		dDispScaleFactor           = 0.;
		nValueNumberDecimalPoint   = 0;
		nValueOutputMinMaxType     = 0;
		dValueOutputMinMaxLimit    = 0.;
		nValueSpecifiedAngle       = 0;
		nValueValueAngle           = 0;
		nMirrorState               = 0;
		nMirrorPlane[0]            = 0;
		nMirrorPlane[1]            = 0;
		dMirrorPlnLocation[0]      = 0.;
		dMirrorPlnLocation[1]      = 0.;
		dMVLTRCScaleFactor          = 0.;
		bMVLTRCValue                = FALSE;
		nMVLTRCValueType            = 0;
		nMVLTRCValueDePt            = 0;
		for(int i=0; i<30; i++)
			aRankMap[i] = 0;
		for(int i = 0; i<EN_FDSP_D_END; i++)
			bFdspD[i] = FALSE;
	}
	T_FDSP_D_821() { Initialize(); }
	T_FDSP_D_821(const T_FDSP_D_821 &src) { *this = src; }
};

struct T_FDSP_D_883
{
	double  dContAnnotSpacing          ;  // Contour Annotation Spacing
	int     nCurrentRankNum            ;  // Number of Colors
	int     nCurrentColorTable         ;  // Color Table
	COLORREF colEdgeColor              ;  // Contour Line
	COLORREF colElemColor              ;  // Element Edge
	int     nContourFill               ;  // 0: Contour Fill, 1: Draw Contour Line Only
	double  dExtrudeFactor             ;  // Extrude scale
	int     nLegendPosition            ;
	int     nLegendDeciPt              ;
	int     nLegendValType             ;
	double  aRankMap[30]               ;  // Legend rank values (CContourEngine의 ECONTOUR_RANK_NUM)
	double  dDispScaleFactor           ;  // Deformation Scale Factor
	int     nValueNumberDecimalPoint   ;
	UINT    nValueOutputMinMaxType     ;
	double  dValueOutputMinMaxLimit    ;
	UINT    nValueSpecifiedAngle       ;
	int     nValueValueAngle           ;
	int     nMirrorState               ;
	int     nMirrorPlane[2]            ;
	double  dMirrorPlnLocation[2]      ;
	BOOL    bFdspD[EN_FDSP_D_END]      ;
	// Applied Loads(Moving Load Tracer)
	double  dMVLTRCScaleFactor          ;
	BOOL    bMVLTRCValue                ;
	int     nMVLTRCValueType            ;
	int     nMVLTRCValueDePt            ;
	int     nMVLTRCDefmDispComponent    ; // DeformEngine에서 사용하는 값	

	void Initialize()
	{
		dContAnnotSpacing          = 0.;
		nCurrentRankNum            = 0;
		nCurrentColorTable         = 0;
		colEdgeColor               = 0;
		colElemColor               = 0;
		nContourFill               = 0;
		dExtrudeFactor             = 0.;
		nLegendPosition            = 0;
		nLegendDeciPt              = 0;
		nLegendValType             = 0;
		dDispScaleFactor           = 0.;
		nValueNumberDecimalPoint   = 0;
		nValueOutputMinMaxType     = 0;
		dValueOutputMinMaxLimit    = 0.;
		nValueSpecifiedAngle       = 0;
		nValueValueAngle           = 0;
		nMirrorState               = 0;
		nMirrorPlane[0]            = 0;
		nMirrorPlane[1]            = 0;
		dMirrorPlnLocation[0]      = 0.;
		dMirrorPlnLocation[1]      = 0.;
		dMVLTRCScaleFactor          = 0.;
		bMVLTRCValue                = FALSE;
		nMVLTRCValueType            = 0;
		nMVLTRCValueDePt            = 0;
		nMVLTRCDefmDispComponent    = 0;
		for(int i=0; i<30; i++)
			aRankMap[i] = 0;
		for(int i = 0; i<EN_FDSP_D_END; i++)
			bFdspD[i] = FALSE;
	}
	T_FDSP_D_883() { Initialize(); }
	T_FDSP_D_883(const T_FDSP_D_883 &src) { *this = src; }
};

struct T_FDSP_D
{
	double  dContAnnotSpacing          ;  // Contour Annotation Spacing
	int     nCurrentRankNum            ;  // Number of Colors
	int     nCurrentColorTable         ;  // Color Table
	COLORREF colEdgeColor              ;  // Contour Line
	COLORREF colElemColor              ;  // Element Edge
	int     nContourFill               ;  // 0: Contour Fill, 1: Draw Contour Line Only
	double  dExtrudeFactor             ;  // Extrude scale
	int     nLegendPosition            ;
	int     nLegendDeciPt              ;
	int     nLegendValType             ;
	double  aRankMap[30]               ;  // Legend rank values (CContourEngine의 ECONTOUR_RANK_NUM)
	double  dDispScaleFactor           ;  // Deformation Scale Factor
	int     nValueNumberDecimalPoint   ;
	UINT    nValueOutputMinMaxType     ;
	double  dValueOutputMinMaxLimit    ;
	UINT    nValueSpecifiedAngle       ;
	int     nValueValueAngle           ;
	int     nMirrorState               ;
	int     nMirrorPlane[2]            ;
	double  dMirrorPlnLocation[2]      ;
	BOOL    bFdspD[EN_FDSP_D_END]      ;
	// Applied Loads(Moving Load Tracer)
	double  dMVLTRCScaleFactor          ;
	BOOL    bMVLTRCValue                ;
	int     nMVLTRCValueType            ;
	int     nMVLTRCValueDePt            ;
	int     nMVLTRCDefmDispComponent    ; // DeformEngine에서 사용하는 값	
	BOOL	bDispOpt					;
	BOOL	bElemCenter					;
	int		nValOpt						; // 0:Max, 1:Elem Center

	void Initialize()
	{
		dContAnnotSpacing          = 0.;
		nCurrentRankNum            = 0;
		nCurrentColorTable         = 0;
		colEdgeColor               = 0;
		colElemColor               = 0;
		nContourFill               = 0;
		dExtrudeFactor             = 0.;
		nLegendPosition            = 0;
		nLegendDeciPt              = 0;
		nLegendValType             = 0;
		dDispScaleFactor           = 0.;
		nValueNumberDecimalPoint   = 0;
		nValueOutputMinMaxType     = 0;
		dValueOutputMinMaxLimit    = 0.;
		nValueSpecifiedAngle       = 0;
		nValueValueAngle           = 0;
		nMirrorState               = 0;
		nMirrorPlane[0]            = 0;
		nMirrorPlane[1]            = 0;
		dMirrorPlnLocation[0]      = 0.;
		dMirrorPlnLocation[1]      = 0.;
		dMVLTRCScaleFactor         = 0.;
		bMVLTRCValue               = FALSE;
		nMVLTRCValueType           = 0;
		nMVLTRCValueDePt           = 0;
		nMVLTRCDefmDispComponent   = 0;
		bDispOpt				   = FALSE;
		bElemCenter				   = FALSE;
		nValOpt					   = 0;
		for(int i=0; i<30; i++)
			aRankMap[i] = 0;
		for(int i = 0; i<EN_FDSP_D_END; i++)
			bFdspD[i] = FALSE;
	}
	T_FDSP_D() { Initialize(); }
	T_FDSP_D(const T_FDSP_D &src) { *this = src; }
	void Convert821(T_FDSP_D_821 &src)
	{
		dContAnnotSpacing        = src.dContAnnotSpacing       ;
		nCurrentRankNum          = src.nCurrentRankNum         ;
		nCurrentColorTable       = src.nCurrentColorTable      ;
		colEdgeColor             = src.colEdgeColor            ;
		colElemColor             = src.colElemColor            ;
		nContourFill             = src.nContourFill            ;
		dExtrudeFactor           = src.dExtrudeFactor          ;
		nLegendPosition          = src.nLegendPosition         ;
		nLegendDeciPt            = src.nLegendDeciPt           ;
		nLegendValType           = src.nLegendValType          ;
		dDispScaleFactor         = src.dDispScaleFactor        ;
		nValueNumberDecimalPoint = src.nValueNumberDecimalPoint;
		nValueOutputMinMaxType   = src.nValueOutputMinMaxType  ;
		dValueOutputMinMaxLimit  = src.dValueOutputMinMaxLimit ;
		nValueSpecifiedAngle     = src.nValueSpecifiedAngle    ;
		nValueValueAngle         = src.nValueValueAngle        ;
		nMirrorState             = src.nMirrorState            ;
		nMirrorPlane[0]          = src.nMirrorPlane[0]         ;
		nMirrorPlane[1]          = src.nMirrorPlane[1]         ;
		dMirrorPlnLocation[0]    = src.dMirrorPlnLocation[0]   ;
		dMirrorPlnLocation[1]    = src.dMirrorPlnLocation[1]   ;
		dMVLTRCScaleFactor       = src.dMVLTRCScaleFactor      ;
		bMVLTRCValue             = src.bMVLTRCValue            ;
		nMVLTRCValueType         = src.nMVLTRCValueType        ;
		nMVLTRCValueDePt         = src.nMVLTRCValueDePt        ;
		nMVLTRCDefmDispComponent = 0;
		bDispOpt				 = FALSE;
		bElemCenter				 = FALSE;
		nValOpt					 = 0;

		for(int i=0; i<30; i++)
			aRankMap[i] = src.aRankMap[i];
		for(int i = 0; i<EN_FDSP_D_END; i++)
			bFdspD[i] = src.bFdspD[i];
	}
	void Convert883(T_FDSP_D_883 &src)
	{
		dContAnnotSpacing        = src.dContAnnotSpacing       ;
		nCurrentRankNum          = src.nCurrentRankNum         ;
		nCurrentColorTable       = src.nCurrentColorTable      ;
		colEdgeColor             = src.colEdgeColor            ;
		colElemColor             = src.colElemColor            ;
		nContourFill             = src.nContourFill            ;
		dExtrudeFactor           = src.dExtrudeFactor          ;
		nLegendPosition          = src.nLegendPosition         ;
		nLegendDeciPt            = src.nLegendDeciPt           ;
		nLegendValType           = src.nLegendValType          ;
		dDispScaleFactor         = src.dDispScaleFactor        ;
		nValueNumberDecimalPoint = src.nValueNumberDecimalPoint;
		nValueOutputMinMaxType   = src.nValueOutputMinMaxType  ;
		dValueOutputMinMaxLimit  = src.dValueOutputMinMaxLimit ;
		nValueSpecifiedAngle     = src.nValueSpecifiedAngle    ;
		nValueValueAngle         = src.nValueValueAngle        ;
		nMirrorState             = src.nMirrorState            ;
		nMirrorPlane[0]          = src.nMirrorPlane[0]         ;
		nMirrorPlane[1]          = src.nMirrorPlane[1]         ;
		dMirrorPlnLocation[0]    = src.dMirrorPlnLocation[0]   ;
		dMirrorPlnLocation[1]    = src.dMirrorPlnLocation[1]   ;
		dMVLTRCScaleFactor       = src.dMVLTRCScaleFactor      ;
		bMVLTRCValue             = src.bMVLTRCValue            ;
		nMVLTRCValueType         = src.nMVLTRCValueType        ;
		nMVLTRCValueDePt         = src.nMVLTRCValueDePt        ;
		nMVLTRCDefmDispComponent = src.nMVLTRCDefmDispComponent;
		bDispOpt				 = FALSE;
		bElemCenter				 = FALSE;
		nValOpt					 = 0;

		for(int i=0; i<30; i++)
			aRankMap[i] = src.aRankMap[i];
		for(int i = 0; i<EN_FDSP_D_END; i++)
			bFdspD[i] = src.bFdspD[i];
	}
};

// v821 : Meshed Design 옵션 추가
struct T_FOUT_D_821
{
	int     nOutputLocation     ;
	double  dScFactor           ;
	double  dAdjustFactor       ;
	int     nAverage            ; // Element or Average 
	int     nLocalGlobal        ; // Local or UCS
	double  dMxxCoeff           ;
	double  dMyyCoeff           ;
	int     nMxxMyyMode         ;
	double  dMxyCoeff           ;
	UINT    NucsK               ; // for UCS Axis Display
	int     nPStrsSurface       ; // Top, Bottom, Both Sides, Abs Max
	UINT    nMbDgrmNPlanK       ; // Member Diagram..
	UINT    nMbDgrmStorK        ;
	int     nMbDgrmDeciPt       ;
	int     nMbDgrmDgrmType     ;
	double  dMbDgrmScFt         ;
	int     nMbdgrmDgrmComp     ;
	int     nMbDgrmOutType      ;
	UINT    nMbDgrmSelectMode   ;
	UINT    colValAxial         ;
	UINT    colValShear         ;
	UINT    colValMoment        ;
	int     nSSFROutType        ;
	double  dSSFRAngle          ;
	int     nThisComp           ; // Time History Selection (0: Displ, 1: Vel, 2: Accel)
	int     nThisFunction       ; // Time History Function
	UINT    nFunctionKey        ; // Time History Function
	int     nHingeTimeIndex     ; // Time History Step Combo
	double  dHingeTimeStep      ; // Time History Step
	int     nThisTimeIndex      ;
	double  dThisTimeStep       ;
	int     nHingeValType       ; // Pushover Type of Value
	int     nHingeDutl          ; // Pushover Type of Result
	CArray<UINT,UINT> aPfudClwpK; // Plate Cutting Line Diagram (checked list items key)
	int     nPfudComp           ; // Plate Cutting Line Diagram (Components)
	int     nPfudDir            ; // Plate Cutting Line Diagram (Options 0: Nomal, 1: In Plane)
	double  dEDgrmDefmScFc      ; // User Defined Diagram : Deformed Scale
	UINT    nEDgrmScFcType      ; // Scale Factor (0: Auto, 1: Ref. Value)
	double  dEDgrmAutoScFc      ; // Auto
	double  dEDgrmRefValue      ; // Ref. Value
	double  dEDgrmRefValueScFc  ; // Scale Factor
	CArray<UINT,UINT> aSelectedDiagK; // Selected Diagram Key
	BOOL    bFoutD[EN_FOUT_D_END];
	// Nodal Result of RS
	int     nInerForceOrAcc     ; // 0:Inertia Force , 1:Acceleration
	int     nUsedSerialModeIdx  ; // Mode Index
	BOOL    bMode               ; // Mode
	// Cutting Plane Detail
	BOOL    bOnCuttingLine      ;
	BOOL    bOnCuttingPlane     ;
	CArray<UINT,UINT&>  aCutPlanes;
	int		  nSolidFreeEdgeType  ;
	// IsoSurface Detail
	BOOL    bIsoSurface         ;
	CArray<double,double> aIsoValues;
	int	    nTransparentIntensity;
	int	    nIsoValueMode       ;
	BOOL	  bDrawPolyOutLine    ;
	BOOL	  bTransparent        ;
	// Cutting Diagram
	double  dCutLineScaleFactor ;
	BOOL    bCutLineOutputMinMax;
	CArray<UINT,UINT> aCutLineKeys;
	int     nCutPlaneHorizVert  ;
	COLORREF colCutPlaneColor   ;
	int     nCuttingLineDir     ; // 0: Normal, 1: In Plane
	COLORREF colCuttinLineColor ;
	BOOL    bDrawPolyLine       ;
	BOOL    bCutLineMinMaxOutOnly;
	BOOL    bCutLineReverse     ;
	BOOL    bShowCutLineGraph   ;
	int     nCutLineOrPlane     ; //0:Cutting Line,1:Cutting Plane
	// Influence Lines (Surfaces)
	UINT    nInflKeyNode        ;
	BOOL    bInflIclImpFct      ;
	BOOL    bLocalValue         ;
	// PSC Design Result Diagram
	double  dCritLineVal        ;
	int     nPSCCirteriaType    ;
	int     nFillType           ;
	double  dPSCDgrmScFc        ;
	int     nDgrmDirType        ;
	int     nDgrmDirOptn        ;
	BOOL    bPSCSignPos         ;
	BOOL    bPSCSignNeg         ;
	UINT    nResistanceLineCol  ;
	UINT    nResistanceLineThk  ;
	int     nPSCCompCH          ;
	int     nPSCCompJP          ;
	int     nPSCCode            ;
	int     nDgnCode            ;

	void Initialize()
	{
		nOutputLocation       = 0;
		dScFactor             = 1.0;
		dAdjustFactor         = 1.0;
		nAverage              = 0;
		nLocalGlobal          = 0;
		dMxxCoeff             = 1.0;
		dMyyCoeff             = 1.0;
		nMxxMyyMode           = 0;
		dMxyCoeff             = 1.0;
		NucsK                 = 0;
		nPStrsSurface         = 0;
		nMbDgrmNPlanK         = 0;
		nMbDgrmStorK          = 0;
		nMbDgrmDeciPt         = 0;
		nMbDgrmDgrmType       = 0;
		dMbDgrmScFt           = 0.;
		nMbdgrmDgrmComp       = 0;
		nMbDgrmOutType        = 0;
		nMbDgrmSelectMode     = 0;
		colValAxial           = 0;
		colValShear           = 0;
		colValMoment          = 0;
		nSSFROutType          = 0;
		dSSFRAngle            = 0.;    
		nThisComp             = 0;
		nThisFunction         = 0;
		nFunctionKey          = 0;
		nHingeTimeIndex       = 0;
		dHingeTimeStep        = 0.;
		nThisTimeIndex        = 0;
		dThisTimeStep         = 0.;
		nHingeValType         = 0;
		nHingeDutl            = 0;
		aPfudClwpK            .RemoveAll();
		nPfudComp             = 0;
		nPfudDir              = 0;
		dEDgrmDefmScFc        = 0.;
		nEDgrmScFcType        = 0;
		dEDgrmAutoScFc        = 0.;
		dEDgrmRefValue        = 0.;
		dEDgrmRefValueScFc    = 0.;
		aSelectedDiagK        .RemoveAll();
		for(int i=0; i<EN_FOUT_D_END; i++)
			bFoutD[i] = FALSE;
		nInerForceOrAcc       = 0;
		nUsedSerialModeIdx    = 0;
		bMode                 = TRUE;
		bOnCuttingLine        = FALSE;
		bOnCuttingPlane       = FALSE;
		aCutPlanes            .RemoveAll();
		nSolidFreeEdgeType    = 0;
		bIsoSurface           = FALSE;
		aIsoValues            .RemoveAll();
		nTransparentIntensity = 0;
		nIsoValueMode         = 0;
		bDrawPolyOutLine      = FALSE;
		bTransparent          = FALSE;
		dCutLineScaleFactor   = 0.;
		bCutLineOutputMinMax  = FALSE;
		aCutLineKeys          .RemoveAll();
		nCutPlaneHorizVert    = 0;
		colCutPlaneColor      = 0;
		nCuttingLineDir       = 0;
		colCuttinLineColor    = 0;
		bDrawPolyLine         = FALSE;
		bCutLineMinMaxOutOnly = FALSE;
		bCutLineReverse       = FALSE;
		bShowCutLineGraph     = FALSE;
		nCutLineOrPlane       = 0;
		nInflKeyNode          = 0;
		bInflIclImpFct        = FALSE;
		bLocalValue           = FALSE;
		dCritLineVal          = 0.;
		nPSCCirteriaType      = 0;
		nFillType             = 0;
		dPSCDgrmScFc          = 0.;
		nDgrmDirType          = 0;
		nDgrmDirOptn          = 0;
		bPSCSignPos           = FALSE;
		bPSCSignNeg           = FALSE;
		nResistanceLineCol    = 0;
		nResistanceLineThk    = 0;
		nPSCCompCH            = 0;
		nPSCCompJP            = 0;
		nPSCCode              = 0;
		nDgnCode              = 0;
	}
	T_FOUT_D_821() { Initialize(); }
	T_FOUT_D_821(const T_FOUT_D_821 &src) { *this = src; }
	T_FOUT_D_821& operator=(const T_FOUT_D_821 &src)
	{
		nOutputLocation       = src.nOutputLocation       ;
		dScFactor             = src.dScFactor             ;
		dAdjustFactor         = src.dAdjustFactor         ;
		nAverage              = src.nAverage              ;
		nLocalGlobal          = src.nLocalGlobal          ;
		dMxxCoeff             = src.dMxxCoeff             ;
		dMyyCoeff             = src.dMyyCoeff             ;
		nMxxMyyMode           = src.nMxxMyyMode           ;
		dMxyCoeff             = src.dMxyCoeff             ;
		NucsK                 = src.NucsK                 ;
		nPStrsSurface         = src.nPStrsSurface         ;
		nMbDgrmNPlanK         = src.nMbDgrmNPlanK         ;
		nMbDgrmStorK          = src.nMbDgrmStorK          ;
		nMbDgrmDeciPt         = src.nMbDgrmDeciPt         ;
		nMbDgrmDgrmType       = src.nMbDgrmDgrmType       ;
		dMbDgrmScFt           = src.dMbDgrmScFt           ;
		nMbdgrmDgrmComp       = src.nMbdgrmDgrmComp       ;
		nMbDgrmOutType        = src.nMbDgrmOutType        ;
		nMbDgrmSelectMode     = src.nMbDgrmSelectMode     ;
		colValAxial           = src.colValAxial           ;
		colValShear           = src.colValShear           ;
		colValMoment          = src.colValMoment          ;
		nSSFROutType          = src.nSSFROutType          ;
		dSSFRAngle            = src.dSSFRAngle            ;    
		nThisComp             = src.nThisComp             ;
		nThisFunction         = src.nThisFunction         ;
		nFunctionKey          = src.nFunctionKey          ;
		nHingeTimeIndex       = src.nHingeTimeIndex       ;
		dHingeTimeStep        = src.dHingeTimeStep        ;
		nThisTimeIndex        = src.nThisTimeIndex        ;
		dThisTimeStep         = src.dThisTimeStep         ;
		nHingeValType         = src.nHingeValType         ;
		nHingeDutl            = src.nHingeDutl            ;
		aPfudClwpK            .Copy(src.aPfudClwpK       );
		nPfudComp             = src.nPfudComp             ;
		nPfudDir              = src.nPfudDir              ;
		dEDgrmDefmScFc        = src.dEDgrmDefmScFc        ;
		nEDgrmScFcType        = src.nEDgrmScFcType        ;
		dEDgrmAutoScFc        = src.dEDgrmAutoScFc        ;
		dEDgrmRefValue        = src.dEDgrmRefValue        ;
		dEDgrmRefValueScFc    = src.dEDgrmRefValueScFc    ;
		aSelectedDiagK        .Copy(src.aSelectedDiagK   );
		memcpy(bFoutD, src.bFoutD, sizeof(bFoutD));
		nInerForceOrAcc       = src.nInerForceOrAcc       ;
		nUsedSerialModeIdx    = src.nUsedSerialModeIdx    ;
		bMode                 = src.bMode                 ;
		bOnCuttingLine        = src.bOnCuttingLine        ;
		bOnCuttingPlane       = src.bOnCuttingPlane       ;
		aCutPlanes            .Copy(src.aCutPlanes       );
		nSolidFreeEdgeType    = src.nSolidFreeEdgeType    ;
		bIsoSurface           = src.bIsoSurface           ;
		aIsoValues            .Copy(src.aIsoValues       );
		nTransparentIntensity = src.nTransparentIntensity ;
		nIsoValueMode         = src.nIsoValueMode         ;
		bDrawPolyOutLine      = src.bDrawPolyOutLine      ;
		bTransparent          = src.bTransparent          ;
		dCutLineScaleFactor   = src.dCutLineScaleFactor   ;
		bCutLineOutputMinMax  = src.bCutLineOutputMinMax  ;
		aCutLineKeys          .Copy(src.aCutLineKeys     );
		nCutPlaneHorizVert    = src.nCutPlaneHorizVert    ;
		colCutPlaneColor      = src.colCutPlaneColor      ;
		nCuttingLineDir       = src.nCuttingLineDir       ;
		colCuttinLineColor    = src.colCuttinLineColor    ;
		bDrawPolyLine         = src.bDrawPolyLine         ;
		bCutLineMinMaxOutOnly = src.bCutLineMinMaxOutOnly ;
		bCutLineReverse       = src.bCutLineReverse       ;
		bShowCutLineGraph     = src.bShowCutLineGraph     ;
		nCutLineOrPlane       = src.nCutLineOrPlane       ;
		nInflKeyNode          = src.nInflKeyNode          ;
		bInflIclImpFct        = src.bInflIclImpFct        ;
		bLocalValue           = src.bLocalValue           ;
		dCritLineVal          = src.dCritLineVal          ;
		nPSCCirteriaType      = src.nPSCCirteriaType      ;
		nFillType             = src.nFillType             ;
		dPSCDgrmScFc          = src.dPSCDgrmScFc          ;
		nDgrmDirType          = src.nDgrmDirType          ;
		nDgrmDirOptn          = src.nDgrmDirOptn          ;
		bPSCSignPos           = src.bPSCSignPos           ;
		bPSCSignNeg           = src.bPSCSignNeg           ;
		nResistanceLineCol    = src.nResistanceLineCol    ;
		nResistanceLineThk    = src.nResistanceLineThk    ;
		nPSCCompCH            = src.nPSCCompCH            ;
		nPSCCompJP            = src.nPSCCompJP            ;
		nPSCCode              = src.nPSCCode              ;
		nDgnCode              = src.nDgnCode              ;

		return *this;
	}
};

struct T_FOUT_D_825
{
	int     nOutputLocation     ;
	double  dScFactor           ;
	double  dAdjustFactor       ;
	int     nAverage            ; // Element or Average 
	int     nLocalGlobal        ; // Local or UCS
	double  dMxxCoeff           ;
	double  dMyyCoeff           ;
	int     nMxxMyyMode         ;
	double  dMxyCoeff           ;
	UINT    NucsK               ; // for UCS Axis Display
	int     nPStrsSurface       ; // Top, Bottom, Both Sides, Abs Max
	UINT    nMbDgrmNPlanK       ; // Member Diagram..
	UINT    nMbDgrmStorK        ;
	int     nMbDgrmDeciPt       ;
	int     nMbDgrmDgrmType     ;
	double  dMbDgrmScFt         ;
	int     nMbdgrmDgrmComp     ;
	int     nMbDgrmOutType      ;
	UINT    nMbDgrmSelectMode   ;
	UINT    colValAxial         ;
	UINT    colValShear         ;
	UINT    colValMoment        ;
	int     nSSFROutType        ;
	double  dSSFRAngle          ;
	int     nThisComp           ; // Time History Selection (0: Displ, 1: Vel, 2: Accel)
	int     nThisFunction       ; // Time History Function
	UINT    nFunctionKey        ; // Time History Function
	int     nHingeTimeIndex     ; // Time History Step Combo
	double  dHingeTimeStep      ; // Time History Step
	int     nThisTimeIndex      ;
	double  dThisTimeStep       ;
	int     nHingeValType       ; // Pushover Type of Value
	int     nHingeDutl          ; // Pushover Type of Result
	CArray<UINT,UINT> aPfudClwpK; // Plate Cutting Line Diagram (checked list items key)
	int     nPfudComp           ; // Plate Cutting Line Diagram (Components)
	int     nPfudDir            ; // Plate Cutting Line Diagram (Options 0: Nomal, 1: In Plane)
	double  dEDgrmDefmScFc      ; // User Defined Diagram : Deformed Scale
	UINT    nEDgrmScFcType      ; // Scale Factor (0: Auto, 1: Ref. Value)
	double  dEDgrmAutoScFc      ; // Auto
	double  dEDgrmRefValue      ; // Ref. Value
	double  dEDgrmRefValueScFc  ; // Scale Factor
	CArray<UINT,UINT> aSelectedDiagK; // Selected Diagram Key
	BOOL    bFoutD[EN_FOUT_D_END];
	// Nodal Result of RS
	int     nInerForceOrAcc     ; // 0:Inertia Force , 1:Acceleration
	int     nUsedSerialModeIdx  ; // Mode Index
	BOOL    bMode               ; // Mode
	// Cutting Plane Detail
	BOOL    bOnCuttingLine      ;
	BOOL    bOnCuttingPlane     ;
	CArray<UINT,UINT&>  aCutPlanes;
	int		  nSolidFreeEdgeType  ;
	// IsoSurface Detail
	BOOL    bIsoSurface         ;
	CArray<double,double> aIsoValues;
	int	    nTransparentIntensity;
	int	    nIsoValueMode       ;
	BOOL	  bDrawPolyOutLine    ;
	BOOL	  bTransparent        ;
	// Cutting Diagram
	double  dCutLineScaleFactor ;
	BOOL    bCutLineOutputMinMax;
	CArray<UINT,UINT> aCutLineKeys;
	int     nCutPlaneHorizVert  ;
	COLORREF colCutPlaneColor   ;
	int     nCuttingLineDir     ; // 0: Normal, 1: In Plane
	COLORREF colCuttinLineColor ;
	BOOL    bDrawPolyLine       ;
	BOOL    bCutLineMinMaxOutOnly;
	BOOL    bCutLineReverse     ;
	BOOL    bShowCutLineGraph   ;
	int     nCutLineOrPlane     ; //0:Cutting Line,1:Cutting Plane
	// Influence Lines (Surfaces)
	UINT    nInflKeyNode        ;
	BOOL    bInflIclImpFct      ;
	BOOL    bLocalValue         ;
	// PSC Design Result Diagram
	double  dCritLineVal        ;
	int     nPSCCirteriaType    ;
	int     nFillType           ;
	double  dPSCDgrmScFc        ;
	int     nDgrmDirType        ;
	int     nDgrmDirOptn        ;
	BOOL    bPSCSignPos         ;
	BOOL    bPSCSignNeg         ;
	UINT    nResistanceLineCol  ;
	UINT    nResistanceLineThk  ;
	int     nPSCCompCH          ;
	int     nPSCCompJP          ;
	int     nPSCCode            ;
	int     nDgnCode            ;
	int     nMeshedDgn_ElemAvg  ;
	int     nMeshedDgn_ElemCell ;
	double  dMeshedDgn_CellLen  ;
	int     nMeshedDgn_XDirYDir  ;
	int     nMeshedDgn_RebarRatio;

	void Initialize()
	{
		nOutputLocation       = 0;
		dScFactor             = 1.0;
		dAdjustFactor         = 1.0;
		nAverage              = 0;
		nLocalGlobal          = 0;
		dMxxCoeff             = 1.0;
		dMyyCoeff             = 1.0;
		nMxxMyyMode           = 0;
		dMxyCoeff             = 1.0;
		NucsK                 = 0;
		nPStrsSurface         = 0;
		nMbDgrmNPlanK         = 0;
		nMbDgrmStorK          = 0;
		nMbDgrmDeciPt         = 0;
		nMbDgrmDgrmType       = 0;
		dMbDgrmScFt           = 0.;
		nMbdgrmDgrmComp       = 0;
		nMbDgrmOutType        = 0;
		nMbDgrmSelectMode     = 0;
		colValAxial           = 0;
		colValShear           = 0;
		colValMoment          = 0;
		nSSFROutType          = 0;
		dSSFRAngle            = 0.;    
		nThisComp             = 0;
		nThisFunction         = 0;
		nFunctionKey          = 0;
		nHingeTimeIndex       = 0;
		dHingeTimeStep        = 0.;
		nThisTimeIndex        = 0;
		dThisTimeStep         = 0.;
		nHingeValType         = 0;
		nHingeDutl            = 0;
		aPfudClwpK            .RemoveAll();
		nPfudComp             = 0;
		nPfudDir              = 0;
		dEDgrmDefmScFc        = 0.;
		nEDgrmScFcType        = 0;
		dEDgrmAutoScFc        = 0.;
		dEDgrmRefValue        = 0.;
		dEDgrmRefValueScFc    = 0.;
		aSelectedDiagK        .RemoveAll();
		for(int i=0; i<EN_FOUT_D_END; i++)
			bFoutD[i] = FALSE;
		nInerForceOrAcc       = 0;
		nUsedSerialModeIdx    = 0;
		bMode                 = TRUE;
		bOnCuttingLine        = FALSE;
		bOnCuttingPlane       = FALSE;
		aCutPlanes            .RemoveAll();
		nSolidFreeEdgeType    = 0;
		bIsoSurface           = FALSE;
		aIsoValues            .RemoveAll();
		nTransparentIntensity = 0;
		nIsoValueMode         = 0;
		bDrawPolyOutLine      = FALSE;
		bTransparent          = FALSE;
		dCutLineScaleFactor   = 0.;
		bCutLineOutputMinMax  = FALSE;
		aCutLineKeys          .RemoveAll();
		nCutPlaneHorizVert    = 0;
		colCutPlaneColor      = 0;
		nCuttingLineDir       = 0;
		colCuttinLineColor    = 0;
		bDrawPolyLine         = FALSE;
		bCutLineMinMaxOutOnly = FALSE;
		bCutLineReverse       = FALSE;
		bShowCutLineGraph     = FALSE;
		nCutLineOrPlane       = 0;
		nInflKeyNode          = 0;
		bInflIclImpFct        = FALSE;
		bLocalValue           = FALSE;
		dCritLineVal          = 0.;
		nPSCCirteriaType      = 0;
		nFillType             = 0;
		dPSCDgrmScFc          = 0.;
		nDgrmDirType          = 0;
		nDgrmDirOptn          = 0;
		bPSCSignPos           = FALSE;
		bPSCSignNeg           = FALSE;
		nResistanceLineCol    = 0;
		nResistanceLineThk    = 0;
		nPSCCompCH            = 0;
		nPSCCompJP            = 0;
		nPSCCode              = 0;
		nDgnCode              = 0;
		nMeshedDgn_ElemAvg    = 0;
		nMeshedDgn_ElemCell   = 0;
		dMeshedDgn_CellLen    = 0;
		nMeshedDgn_XDirYDir   = 0;
		nMeshedDgn_RebarRatio = 0;
	}
	T_FOUT_D_825() { Initialize(); }
	T_FOUT_D_825(const T_FOUT_D_825 &src) { *this = src; }
	T_FOUT_D_825& operator=(const T_FOUT_D_825 &src)
	{
		nOutputLocation       = src.nOutputLocation       ;
		dScFactor             = src.dScFactor             ;
		dAdjustFactor         = src.dAdjustFactor         ;
		nAverage              = src.nAverage              ;
		nLocalGlobal          = src.nLocalGlobal          ;
		dMxxCoeff             = src.dMxxCoeff             ;
		dMyyCoeff             = src.dMyyCoeff             ;
		nMxxMyyMode           = src.nMxxMyyMode           ;
		dMxyCoeff             = src.dMxyCoeff             ;
		NucsK                 = src.NucsK                 ;
		nPStrsSurface         = src.nPStrsSurface         ;
		nMbDgrmNPlanK         = src.nMbDgrmNPlanK         ;
		nMbDgrmStorK          = src.nMbDgrmStorK          ;
		nMbDgrmDeciPt         = src.nMbDgrmDeciPt         ;
		nMbDgrmDgrmType       = src.nMbDgrmDgrmType       ;
		dMbDgrmScFt           = src.dMbDgrmScFt           ;
		nMbdgrmDgrmComp       = src.nMbdgrmDgrmComp       ;
		nMbDgrmOutType        = src.nMbDgrmOutType        ;
		nMbDgrmSelectMode     = src.nMbDgrmSelectMode     ;
		colValAxial           = src.colValAxial           ;
		colValShear           = src.colValShear           ;
		colValMoment          = src.colValMoment          ;
		nSSFROutType          = src.nSSFROutType          ;
		dSSFRAngle            = src.dSSFRAngle            ;    
		nThisComp             = src.nThisComp             ;
		nThisFunction         = src.nThisFunction         ;
		nFunctionKey          = src.nFunctionKey          ;
		nHingeTimeIndex       = src.nHingeTimeIndex       ;
		dHingeTimeStep        = src.dHingeTimeStep        ;
		nThisTimeIndex        = src.nThisTimeIndex        ;
		dThisTimeStep         = src.dThisTimeStep         ;
		nHingeValType         = src.nHingeValType         ;
		nHingeDutl            = src.nHingeDutl            ;
		aPfudClwpK            .Copy(src.aPfudClwpK       );
		nPfudComp             = src.nPfudComp             ;
		nPfudDir              = src.nPfudDir              ;
		dEDgrmDefmScFc        = src.dEDgrmDefmScFc        ;
		nEDgrmScFcType        = src.nEDgrmScFcType        ;
		dEDgrmAutoScFc        = src.dEDgrmAutoScFc        ;
		dEDgrmRefValue        = src.dEDgrmRefValue        ;
		dEDgrmRefValueScFc    = src.dEDgrmRefValueScFc    ;
		aSelectedDiagK        .Copy(src.aSelectedDiagK   );
		memcpy(bFoutD, src.bFoutD, sizeof(bFoutD));
		nInerForceOrAcc       = src.nInerForceOrAcc       ;
		nUsedSerialModeIdx    = src.nUsedSerialModeIdx    ;
		bMode                 = src.bMode                 ;
		bOnCuttingLine        = src.bOnCuttingLine        ;
		bOnCuttingPlane       = src.bOnCuttingPlane       ;
		aCutPlanes            .Copy(src.aCutPlanes       );
		nSolidFreeEdgeType    = src.nSolidFreeEdgeType    ;
		bIsoSurface           = src.bIsoSurface           ;
		aIsoValues            .Copy(src.aIsoValues       );
		nTransparentIntensity = src.nTransparentIntensity ;
		nIsoValueMode         = src.nIsoValueMode         ;
		bDrawPolyOutLine      = src.bDrawPolyOutLine      ;
		bTransparent          = src.bTransparent          ;
		dCutLineScaleFactor   = src.dCutLineScaleFactor   ;
		bCutLineOutputMinMax  = src.bCutLineOutputMinMax  ;
		aCutLineKeys          .Copy(src.aCutLineKeys     );
		nCutPlaneHorizVert    = src.nCutPlaneHorizVert    ;
		colCutPlaneColor      = src.colCutPlaneColor      ;
		nCuttingLineDir       = src.nCuttingLineDir       ;
		colCuttinLineColor    = src.colCuttinLineColor    ;
		bDrawPolyLine         = src.bDrawPolyLine         ;
		bCutLineMinMaxOutOnly = src.bCutLineMinMaxOutOnly ;
		bCutLineReverse       = src.bCutLineReverse       ;
		bShowCutLineGraph     = src.bShowCutLineGraph     ;
		nCutLineOrPlane       = src.nCutLineOrPlane       ;
		nInflKeyNode          = src.nInflKeyNode          ;
		bInflIclImpFct        = src.bInflIclImpFct        ;
		bLocalValue           = src.bLocalValue           ;
		dCritLineVal          = src.dCritLineVal          ;
		nPSCCirteriaType      = src.nPSCCirteriaType      ;
		nFillType             = src.nFillType             ;
		dPSCDgrmScFc          = src.dPSCDgrmScFc          ;
		nDgrmDirType          = src.nDgrmDirType          ;
		nDgrmDirOptn          = src.nDgrmDirOptn          ;
		bPSCSignPos           = src.bPSCSignPos           ;
		bPSCSignNeg           = src.bPSCSignNeg           ;
		nResistanceLineCol    = src.nResistanceLineCol    ;
		nResistanceLineThk    = src.nResistanceLineThk    ;
		nPSCCompCH            = src.nPSCCompCH            ;
		nPSCCompJP            = src.nPSCCompJP            ;
		nPSCCode              = src.nPSCCode              ;
		nDgnCode              = src.nDgnCode              ;
		nMeshedDgn_ElemAvg    = src.nMeshedDgn_ElemAvg    ;
		nMeshedDgn_ElemCell   = src.nMeshedDgn_ElemCell   ;
		dMeshedDgn_CellLen    = src.dMeshedDgn_CellLen    ;
		nMeshedDgn_XDirYDir   = src.nMeshedDgn_XDirYDir   ;
		nMeshedDgn_RebarRatio = src.nMeshedDgn_RebarRatio ;

		return *this;
	}
};

struct T_FOUT_D_851
{
	int     nOutputLocation     ;
	double  dScFactor           ;
	double  dAdjustFactor       ;
	int     nAverage            ; // Element or Average 
	int     nLocalGlobal        ; // Local or UCS
	double  dMxxCoeff           ;
	double  dMyyCoeff           ;
	int     nMxxMyyMode         ;
	double  dMxyCoeff           ;
	UINT    NucsK               ; // for UCS Axis Display
	int     nPStrsSurface       ; // Top, Bottom, Both Sides, Abs Max
	UINT    nMbDgrmNPlanK       ; // Member Diagram..
	UINT    nMbDgrmStorK        ;
	int     nMbDgrmDeciPt       ;
	int     nMbDgrmDgrmType     ;
	double  dMbDgrmScFt         ;
	int     nMbdgrmDgrmComp     ;
	int     nMbDgrmOutType      ;
	UINT    nMbDgrmSelectMode   ;
	UINT    colValAxial         ;
	UINT    colValShear         ;
	UINT    colValMoment        ;
	int     nSSFROutType        ;
	double  dSSFRAngle          ;
	int     nThisComp           ; // Time History Selection (0: Displ, 1: Vel, 2: Accel)
	int     nThisFunction       ; // Time History Function
	UINT    nFunctionKey        ; // Time History Function
	int     nHingeTimeIndex     ; // Time History Step Combo
	double  dHingeTimeStep      ; // Time History Step
	int     nThisTimeIndex      ;
	double  dThisTimeStep       ;
	int     nHingeValType       ; // Pushover Type of Value
	int     nHingeDutl          ; // Pushover Type of Result
	CArray<UINT,UINT> aPfudClwpK; // Plate Cutting Line Diagram (checked list items key)
	int     nPfudComp           ; // Plate Cutting Line Diagram (Components)
	int     nPfudDir            ; // Plate Cutting Line Diagram (Options 0: Nomal, 1: In Plane)
	double  dEDgrmDefmScFc      ; // User Defined Diagram : Deformed Scale
	UINT    nEDgrmScFcType      ; // Scale Factor (0: Auto, 1: Ref. Value)
	double  dEDgrmAutoScFc      ; // Auto
	double  dEDgrmRefValue      ; // Ref. Value
	double  dEDgrmRefValueScFc  ; // Scale Factor
	CArray<UINT,UINT> aSelectedDiagK; // Selected Diagram Key
	BOOL    bFoutD[EN_FOUT_D_END];
	// Nodal Result of RS
	int     nInerForceOrAcc     ; // 0:Inertia Force , 1:Acceleration
	int     nUsedSerialModeIdx  ; // Mode Index
	BOOL    bMode               ; // Mode
	// Cutting Plane Detail
	BOOL    bOnCuttingLine      ;
	BOOL    bOnCuttingPlane     ;
	CArray<UINT,UINT&>  aCutPlanes;
	int		  nSolidFreeEdgeType  ;
	// IsoSurface Detail
	BOOL    bIsoSurface         ;
	CArray<double,double> aIsoValues;
	int	    nTransparentIntensity;
	int	    nIsoValueMode       ;
	BOOL	  bDrawPolyOutLine    ;
	BOOL	  bTransparent        ;
	// Cutting Diagram
	double  dCutLineScaleFactor ;
	BOOL    bCutLineOutputMinMax;
	CArray<UINT,UINT> aCutLineKeys;
	int     nCutPlaneHorizVert  ;
	COLORREF colCutPlaneColor   ;
	int     nCuttingLineDir     ; // 0: Normal, 1: In Plane
	COLORREF colCuttinLineColor ;
	BOOL    bDrawPolyLine       ;
	BOOL    bCutLineMinMaxOutOnly;
	BOOL    bCutLineReverse     ;
	BOOL    bShowCutLineGraph   ;
	int     nCutLineOrPlane     ; //0:Cutting Line,1:Cutting Plane
	// Influence Lines (Surfaces)
	UINT    nInflKeyNode        ;
	BOOL    bInflIclImpFct      ;
	BOOL    bLocalValue         ;
	// PSC Design Result Diagram
	double  dCritLineVal        ;
	int     nPSCCirteriaType    ;
	int     nFillType           ;
	double  dPSCDgrmScFc        ;
	int     nDgrmDirType        ;
	int     nDgrmDirOptn        ;
	BOOL    bPSCSignPos         ;
	BOOL    bPSCSignNeg         ;
	UINT    nResistanceLineCol  ;
	UINT    nResistanceLineThk  ;
	int     nPSCCompCH          ;
	int     nPSCCompJP          ;
	int     nPSCCode            ;
	int     nDgnCode            ;
	int     nMeshedDgn_ElemAvg  ;
	int     nMeshedDgn_ElemCell ;
	double  dMeshedDgn_CellLen  ;
	int     nMeshedDgn_XDirYDir  ;
	int     nMeshedDgn_RebarRatio;
	double  dMeshedDgnOpt[10]   ; // Meshed Design일 경우 AgenD.adIn 옵션 저장
	int     nMeshedDgnOpt[10]   ; // Meshed Design일 경우 AgenD.aiIn 옵션 저장

	void Initialize()
	{
		nOutputLocation       = 0;
		dScFactor             = 1.0;
		dAdjustFactor         = 1.0;
		nAverage              = 0;
		nLocalGlobal          = 0;
		dMxxCoeff             = 1.0;
		dMyyCoeff             = 1.0;
		nMxxMyyMode           = 0;
		dMxyCoeff             = 1.0;
		NucsK                 = 0;
		nPStrsSurface         = 0;
		nMbDgrmNPlanK         = 0;
		nMbDgrmStorK          = 0;
		nMbDgrmDeciPt         = 0;
		nMbDgrmDgrmType       = 0;
		dMbDgrmScFt           = 0.;
		nMbdgrmDgrmComp       = 0;
		nMbDgrmOutType        = 0;
		nMbDgrmSelectMode     = 0;
		colValAxial           = 0;
		colValShear           = 0;
		colValMoment          = 0;
		nSSFROutType          = 0;
		dSSFRAngle            = 0.;    
		nThisComp             = 0;
		nThisFunction         = 0;
		nFunctionKey          = 0;
		nHingeTimeIndex       = 0;
		dHingeTimeStep        = 0.;
		nThisTimeIndex        = 0;
		dThisTimeStep         = 0.;
		nHingeValType         = 0;
		nHingeDutl            = 0;
		aPfudClwpK            .RemoveAll();
		nPfudComp             = 0;
		nPfudDir              = 0;
		dEDgrmDefmScFc        = 0.;
		nEDgrmScFcType        = 0;
		dEDgrmAutoScFc        = 0.;
		dEDgrmRefValue        = 0.;
		dEDgrmRefValueScFc    = 0.;
		aSelectedDiagK        .RemoveAll();
		for(int i=0; i<EN_FOUT_D_END; i++)
			bFoutD[i] = FALSE;
		nInerForceOrAcc       = 0;
		nUsedSerialModeIdx    = 0;
		bMode                 = TRUE;
		bOnCuttingLine        = FALSE;
		bOnCuttingPlane       = FALSE;
		aCutPlanes            .RemoveAll();
		nSolidFreeEdgeType    = 0;
		bIsoSurface           = FALSE;
		aIsoValues            .RemoveAll();
		nTransparentIntensity = 0;
		nIsoValueMode         = 0;
		bDrawPolyOutLine      = FALSE;
		bTransparent          = FALSE;
		dCutLineScaleFactor   = 0.;
		bCutLineOutputMinMax  = FALSE;
		aCutLineKeys          .RemoveAll();
		nCutPlaneHorizVert    = 0;
		colCutPlaneColor      = 0;
		nCuttingLineDir       = 0;
		colCuttinLineColor    = 0;
		bDrawPolyLine         = FALSE;
		bCutLineMinMaxOutOnly = FALSE;
		bCutLineReverse       = FALSE;
		bShowCutLineGraph     = FALSE;
		nCutLineOrPlane       = 0;
		nInflKeyNode          = 0;
		bInflIclImpFct        = FALSE;
		bLocalValue           = FALSE;
		dCritLineVal          = 0.;
		nPSCCirteriaType      = 0;
		nFillType             = 0;
		dPSCDgrmScFc          = 0.;
		nDgrmDirType          = 0;
		nDgrmDirOptn          = 0;
		bPSCSignPos           = FALSE;
		bPSCSignNeg           = FALSE;
		nResistanceLineCol    = 0;
		nResistanceLineThk    = 0;
		nPSCCompCH            = 0;
		nPSCCompJP            = 0;
		nPSCCode              = 0;
		nDgnCode              = 0;
		nMeshedDgn_ElemAvg    = 0;
		nMeshedDgn_ElemCell   = 0;
		dMeshedDgn_CellLen    = 1;
		nMeshedDgn_XDirYDir   = 0;
		nMeshedDgn_RebarRatio = 2;
		for(int i = 0; i<10; i++) { nMeshedDgnOpt[i]=0; dMeshedDgnOpt[i]=0.0; }
	}
	T_FOUT_D_851() { Initialize(); }
	T_FOUT_D_851(const T_FOUT_D_851 &src) { *this = src; }
	T_FOUT_D_851& operator=(const T_FOUT_D_851 &src)
	{
		nOutputLocation       = src.nOutputLocation       ;
		dScFactor             = src.dScFactor             ;
		dAdjustFactor         = src.dAdjustFactor         ;
		nAverage              = src.nAverage              ;
		nLocalGlobal          = src.nLocalGlobal          ;
		dMxxCoeff             = src.dMxxCoeff             ;
		dMyyCoeff             = src.dMyyCoeff             ;
		nMxxMyyMode           = src.nMxxMyyMode           ;
		dMxyCoeff             = src.dMxyCoeff             ;
		NucsK                 = src.NucsK                 ;
		nPStrsSurface         = src.nPStrsSurface         ;
		nMbDgrmNPlanK         = src.nMbDgrmNPlanK         ;
		nMbDgrmStorK          = src.nMbDgrmStorK          ;
		nMbDgrmDeciPt         = src.nMbDgrmDeciPt         ;
		nMbDgrmDgrmType       = src.nMbDgrmDgrmType       ;
		dMbDgrmScFt           = src.dMbDgrmScFt           ;
		nMbdgrmDgrmComp       = src.nMbdgrmDgrmComp       ;
		nMbDgrmOutType        = src.nMbDgrmOutType        ;
		nMbDgrmSelectMode     = src.nMbDgrmSelectMode     ;
		colValAxial           = src.colValAxial           ;
		colValShear           = src.colValShear           ;
		colValMoment          = src.colValMoment          ;
		nSSFROutType          = src.nSSFROutType          ;
		dSSFRAngle            = src.dSSFRAngle            ;    
		nThisComp             = src.nThisComp             ;
		nThisFunction         = src.nThisFunction         ;
		nFunctionKey          = src.nFunctionKey          ;
		nHingeTimeIndex       = src.nHingeTimeIndex       ;
		dHingeTimeStep        = src.dHingeTimeStep        ;
		nThisTimeIndex        = src.nThisTimeIndex        ;
		dThisTimeStep         = src.dThisTimeStep         ;
		nHingeValType         = src.nHingeValType         ;
		nHingeDutl            = src.nHingeDutl            ;
		aPfudClwpK            .Copy(src.aPfudClwpK       );
		nPfudComp             = src.nPfudComp             ;
		nPfudDir              = src.nPfudDir              ;
		dEDgrmDefmScFc        = src.dEDgrmDefmScFc        ;
		nEDgrmScFcType        = src.nEDgrmScFcType        ;
		dEDgrmAutoScFc        = src.dEDgrmAutoScFc        ;
		dEDgrmRefValue        = src.dEDgrmRefValue        ;
		dEDgrmRefValueScFc    = src.dEDgrmRefValueScFc    ;
		aSelectedDiagK        .Copy(src.aSelectedDiagK   );
		memcpy(bFoutD, src.bFoutD, sizeof(bFoutD));
		nInerForceOrAcc       = src.nInerForceOrAcc       ;
		nUsedSerialModeIdx    = src.nUsedSerialModeIdx    ;
		bMode                 = src.bMode                 ;
		bOnCuttingLine        = src.bOnCuttingLine        ;
		bOnCuttingPlane       = src.bOnCuttingPlane       ;
		aCutPlanes            .Copy(src.aCutPlanes       );
		nSolidFreeEdgeType    = src.nSolidFreeEdgeType    ;
		bIsoSurface           = src.bIsoSurface           ;
		aIsoValues            .Copy(src.aIsoValues       );
		nTransparentIntensity = src.nTransparentIntensity ;
		nIsoValueMode         = src.nIsoValueMode         ;
		bDrawPolyOutLine      = src.bDrawPolyOutLine      ;
		bTransparent          = src.bTransparent          ;
		dCutLineScaleFactor   = src.dCutLineScaleFactor   ;
		bCutLineOutputMinMax  = src.bCutLineOutputMinMax  ;
		aCutLineKeys          .Copy(src.aCutLineKeys     );
		nCutPlaneHorizVert    = src.nCutPlaneHorizVert    ;
		colCutPlaneColor      = src.colCutPlaneColor      ;
		nCuttingLineDir       = src.nCuttingLineDir       ;
		colCuttinLineColor    = src.colCuttinLineColor    ;
		bDrawPolyLine         = src.bDrawPolyLine         ;
		bCutLineMinMaxOutOnly = src.bCutLineMinMaxOutOnly ;
		bCutLineReverse       = src.bCutLineReverse       ;
		bShowCutLineGraph     = src.bShowCutLineGraph     ;
		nCutLineOrPlane       = src.nCutLineOrPlane       ;
		nInflKeyNode          = src.nInflKeyNode          ;
		bInflIclImpFct        = src.bInflIclImpFct        ;
		bLocalValue           = src.bLocalValue           ;
		dCritLineVal          = src.dCritLineVal          ;
		nPSCCirteriaType      = src.nPSCCirteriaType      ;
		nFillType             = src.nFillType             ;
		dPSCDgrmScFc          = src.dPSCDgrmScFc          ;
		nDgrmDirType          = src.nDgrmDirType          ;
		nDgrmDirOptn          = src.nDgrmDirOptn          ;
		bPSCSignPos           = src.bPSCSignPos           ;
		bPSCSignNeg           = src.bPSCSignNeg           ;
		nResistanceLineCol    = src.nResistanceLineCol    ;
		nResistanceLineThk    = src.nResistanceLineThk    ;
		nPSCCompCH            = src.nPSCCompCH            ;
		nPSCCompJP            = src.nPSCCompJP            ;
		nPSCCode              = src.nPSCCode              ;
		nDgnCode              = src.nDgnCode              ;
		nMeshedDgn_ElemAvg    = src.nMeshedDgn_ElemAvg    ;
		nMeshedDgn_ElemCell   = src.nMeshedDgn_ElemCell   ;
		dMeshedDgn_CellLen    = src.dMeshedDgn_CellLen    ;
		nMeshedDgn_XDirYDir   = src.nMeshedDgn_XDirYDir   ;
		nMeshedDgn_RebarRatio = src.nMeshedDgn_RebarRatio ;
		memcpy(dMeshedDgnOpt, src.dMeshedDgnOpt, sizeof(dMeshedDgnOpt));
		memcpy(nMeshedDgnOpt, src.nMeshedDgnOpt, sizeof(nMeshedDgnOpt));
		return *this;
	}
};
struct T_FOUT_D_940
{
	int     nOutputLocation;
	double  dScFactor;
	double  dAdjustFactor;
	int     nAverage; // Element or Average 
	int     nLocalGlobal; // Local or UCS
	double  dMxxCoeff;
	double  dMyyCoeff;
	int     nMxxMyyMode;
	double  dMxyCoeff;
	UINT    NucsK; // for UCS Axis Display
	int     nPStrsSurface; // Top, Bottom, Both Sides, Abs Max
	UINT    nMbDgrmNPlanK; // Member Diagram..
	UINT    nMbDgrmStorK;
	int     nMbDgrmDeciPt;
	int     nMbDgrmDgrmType;
	double  dMbDgrmScFt;
	int     nMbdgrmDgrmComp;
	int     nMbDgrmOutType;
	UINT    nMbDgrmSelectMode;
	UINT    colValAxial;
	UINT    colValShear;
	UINT    colValMoment;
	int     nSSFROutType;
	double  dSSFRAngle;
	int     nThisComp; // Time History Selection (0: Displ, 1: Vel, 2: Accel)
	int     nThisFunction; // Time History Function
	UINT    nFunctionKey; // Time History Function
	int     nHingeTimeIndex; // Time History Step Combo
	double  dHingeTimeStep; // Time History Step
	int     nThisTimeIndex;
	double  dThisTimeStep;
	int     nHingeValType; // Pushover Type of Value
	int     nHingeDutl; // Pushover Type of Result
	CArray<UINT, UINT> aPfudClwpK; // Plate Cutting Line Diagram (checked list items key)
	int     nPfudComp; // Plate Cutting Line Diagram (Components)
	int     nPfudDir; // Plate Cutting Line Diagram (Options 0: Nomal, 1: In Plane)
	double  dEDgrmDefmScFc; // User Defined Diagram : Deformed Scale
	UINT    nEDgrmScFcType; // Scale Factor (0: Auto, 1: Ref. Value)
	double  dEDgrmAutoScFc; // Auto
	double  dEDgrmRefValue; // Ref. Value
	double  dEDgrmRefValueScFc; // Scale Factor
	CArray<UINT, UINT> aSelectedDiagK; // Selected Diagram Key
	BOOL    bFoutD[EN_FOUT_D_END];
	// Nodal Result of RS
	int     nInerForceOrAcc; // 0:Inertia Force , 1:Acceleration
	int     nUsedSerialModeIdx; // Mode Index
	BOOL    bMode; // Mode
	// Cutting Plane Detail
	BOOL    bOnCuttingLine;
	BOOL    bOnCuttingPlane;
	CArray<UINT, UINT&>  aCutPlanes;
	int		  nSolidFreeEdgeType;
	// IsoSurface Detail
	BOOL    bIsoSurface;
	CArray<double, double> aIsoValues;
	int	    nTransparentIntensity;
	int	    nIsoValueMode;
	BOOL	  bDrawPolyOutLine;
	BOOL	  bTransparent;
	// Cutting Diagram
	double  dCutLineScaleFactor;
	BOOL    bCutLineOutputMinMax;
	CArray<UINT, UINT> aCutLineKeys;
	int     nCutPlaneHorizVert;
	COLORREF colCutPlaneColor;
	int     nCuttingLineDir; // 0: Normal, 1: In Plane
	COLORREF colCuttinLineColor;
	BOOL    bDrawPolyLine;
	BOOL    bCutLineMinMaxOutOnly;
	BOOL    bCutLineReverse;
	BOOL    bShowCutLineGraph;
	int     nCutLineOrPlane; //0:Cutting Line,1:Cutting Plane
	// Influence Lines (Surfaces)
	UINT    nInflKeyNode;
	BOOL    bInflIclImpFct;
	BOOL    bLocalValue;
	// PSC Design Result Diagram
	double  dCritLineVal;
	int     nPSCCirteriaType;
	int     nFillType;
	double  dPSCDgrmScFc;
	int     nDgrmDirType;
	int     nDgrmDirOptn;
	BOOL    bPSCSignPos;
	BOOL    bPSCSignNeg;
	UINT    nResistanceLineCol;
	UINT    nResistanceLineThk;
	int     nPSCCompCH;
	int     nPSCCompJP;
	int     nPSCCode;
	int     nDgnCode;
	int     nMeshedDgn_ElemAvg;
	int     nMeshedDgn_ElemCell;
	double  dMeshedDgn_CellLen;
	int     nMeshedDgn_XDirYDir;
	int     nMeshedDgn_RebarRatio;
	double  dMeshedDgnOpt[10]; // Meshed Design일 경우 AgenD.adIn 옵션 저장
	int     nMeshedDgnOpt[10]; // Meshed Design일 경우 AgenD.aiIn 옵션 저장

	int    nWoodArmerTopBot;  // Wood Armer Moment 정보
	double dWoodArmerAngle;   // Wood Armer Moment 정보

	void Initialize()
	{
		nOutputLocation = 0;
		dScFactor = 1.0;
		dAdjustFactor = 1.0;
		nAverage = 0;
		nLocalGlobal = 0;
		dMxxCoeff = 1.0;
		dMyyCoeff = 1.0;
		nMxxMyyMode = 0;
		dMxyCoeff = 1.0;
		NucsK = 0;
		nPStrsSurface = 0;
		nMbDgrmNPlanK = 0;
		nMbDgrmStorK = 0;
		nMbDgrmDeciPt = 0;
		nMbDgrmDgrmType = 0;
		dMbDgrmScFt = 0.;
		nMbdgrmDgrmComp = 0;
		nMbDgrmOutType = 0;
		nMbDgrmSelectMode = 0;
		colValAxial = 0;
		colValShear = 0;
		colValMoment = 0;
		nSSFROutType = 0;
		dSSFRAngle = 0.;
		nThisComp = 0;
		nThisFunction = 0;
		nFunctionKey = 0;
		nHingeTimeIndex = 0;
		dHingeTimeStep = 0.;
		nThisTimeIndex = 0;
		dThisTimeStep = 0.;
		nHingeValType = 0;
		nHingeDutl = 0;
		aPfudClwpK.RemoveAll();
		nPfudComp = 0;
		nPfudDir = 0;
		dEDgrmDefmScFc = 0.;
		nEDgrmScFcType = 0;
		dEDgrmAutoScFc = 0.;
		dEDgrmRefValue = 0.;
		dEDgrmRefValueScFc = 0.;
		aSelectedDiagK.RemoveAll();
		for (int i = 0; i < EN_FOUT_D_END; i++)
			bFoutD[i] = FALSE;
		nInerForceOrAcc = 0;
		nUsedSerialModeIdx = 0;
		bMode = TRUE;
		bOnCuttingLine = FALSE;
		bOnCuttingPlane = FALSE;
		aCutPlanes.RemoveAll();
		nSolidFreeEdgeType = 0;
		bIsoSurface = FALSE;
		aIsoValues.RemoveAll();
		nTransparentIntensity = 0;
		nIsoValueMode = 0;
		bDrawPolyOutLine = FALSE;
		bTransparent = FALSE;
		dCutLineScaleFactor = 0.;
		bCutLineOutputMinMax = FALSE;
		aCutLineKeys.RemoveAll();
		nCutPlaneHorizVert = 0;
		colCutPlaneColor = 0;
		nCuttingLineDir = 0;
		colCuttinLineColor = 0;
		bDrawPolyLine = FALSE;
		bCutLineMinMaxOutOnly = FALSE;
		bCutLineReverse = FALSE;
		bShowCutLineGraph = FALSE;
		nCutLineOrPlane = 0;
		nInflKeyNode = 0;
		bInflIclImpFct = FALSE;
		bLocalValue = FALSE;
		dCritLineVal = 0.;
		nPSCCirteriaType = 0;
		nFillType = 0;
		dPSCDgrmScFc = 0.;
		nDgrmDirType = 0;
		nDgrmDirOptn = 0;
		bPSCSignPos = FALSE;
		bPSCSignNeg = FALSE;
		nResistanceLineCol = 0;
		nResistanceLineThk = 0;
		nPSCCompCH = 0;
		nPSCCompJP = 0;
		nPSCCode = 0;
		nDgnCode = 0;
		nMeshedDgn_ElemAvg = 0;
		nMeshedDgn_ElemCell = 0;
		dMeshedDgn_CellLen = 1;
		nMeshedDgn_XDirYDir = 0;
		nMeshedDgn_RebarRatio = 2;
		for (int i = 0; i < 10; i++) { nMeshedDgnOpt[i] = 0; dMeshedDgnOpt[i] = 0.0; }

		nWoodArmerTopBot = 0;
		dWoodArmerAngle = 0.0;

	}
	T_FOUT_D_940() { Initialize(); }
	T_FOUT_D_940(const T_FOUT_D_940& src) { *this = src; }
	T_FOUT_D_940& operator=(const T_FOUT_D_940& src)
	{
		nOutputLocation = src.nOutputLocation;
		dScFactor = src.dScFactor;
		dAdjustFactor = src.dAdjustFactor;
		nAverage = src.nAverage;
		nLocalGlobal = src.nLocalGlobal;
		dMxxCoeff = src.dMxxCoeff;
		dMyyCoeff = src.dMyyCoeff;
		nMxxMyyMode = src.nMxxMyyMode;
		dMxyCoeff = src.dMxyCoeff;
		NucsK = src.NucsK;
		nPStrsSurface = src.nPStrsSurface;
		nMbDgrmNPlanK = src.nMbDgrmNPlanK;
		nMbDgrmStorK = src.nMbDgrmStorK;
		nMbDgrmDeciPt = src.nMbDgrmDeciPt;
		nMbDgrmDgrmType = src.nMbDgrmDgrmType;
		dMbDgrmScFt = src.dMbDgrmScFt;
		nMbdgrmDgrmComp = src.nMbdgrmDgrmComp;
		nMbDgrmOutType = src.nMbDgrmOutType;
		nMbDgrmSelectMode = src.nMbDgrmSelectMode;
		colValAxial = src.colValAxial;
		colValShear = src.colValShear;
		colValMoment = src.colValMoment;
		nSSFROutType = src.nSSFROutType;
		dSSFRAngle = src.dSSFRAngle;
		nThisComp = src.nThisComp;
		nThisFunction = src.nThisFunction;
		nFunctionKey = src.nFunctionKey;
		nHingeTimeIndex = src.nHingeTimeIndex;
		dHingeTimeStep = src.dHingeTimeStep;
		nThisTimeIndex = src.nThisTimeIndex;
		dThisTimeStep = src.dThisTimeStep;
		nHingeValType = src.nHingeValType;
		nHingeDutl = src.nHingeDutl;
		aPfudClwpK.Copy(src.aPfudClwpK);
		nPfudComp = src.nPfudComp;
		nPfudDir = src.nPfudDir;
		dEDgrmDefmScFc = src.dEDgrmDefmScFc;
		nEDgrmScFcType = src.nEDgrmScFcType;
		dEDgrmAutoScFc = src.dEDgrmAutoScFc;
		dEDgrmRefValue = src.dEDgrmRefValue;
		dEDgrmRefValueScFc = src.dEDgrmRefValueScFc;
		aSelectedDiagK.Copy(src.aSelectedDiagK);
		memcpy(bFoutD, src.bFoutD, sizeof(bFoutD));
		nInerForceOrAcc = src.nInerForceOrAcc;
		nUsedSerialModeIdx = src.nUsedSerialModeIdx;
		bMode = src.bMode;
		bOnCuttingLine = src.bOnCuttingLine;
		bOnCuttingPlane = src.bOnCuttingPlane;
		aCutPlanes.Copy(src.aCutPlanes);
		nSolidFreeEdgeType = src.nSolidFreeEdgeType;
		bIsoSurface = src.bIsoSurface;
		aIsoValues.Copy(src.aIsoValues);
		nTransparentIntensity = src.nTransparentIntensity;
		nIsoValueMode = src.nIsoValueMode;
		bDrawPolyOutLine = src.bDrawPolyOutLine;
		bTransparent = src.bTransparent;
		dCutLineScaleFactor = src.dCutLineScaleFactor;
		bCutLineOutputMinMax = src.bCutLineOutputMinMax;
		aCutLineKeys.Copy(src.aCutLineKeys);
		nCutPlaneHorizVert = src.nCutPlaneHorizVert;
		colCutPlaneColor = src.colCutPlaneColor;
		nCuttingLineDir = src.nCuttingLineDir;
		colCuttinLineColor = src.colCuttinLineColor;
		bDrawPolyLine = src.bDrawPolyLine;
		bCutLineMinMaxOutOnly = src.bCutLineMinMaxOutOnly;
		bCutLineReverse = src.bCutLineReverse;
		bShowCutLineGraph = src.bShowCutLineGraph;
		nCutLineOrPlane = src.nCutLineOrPlane;
		nInflKeyNode = src.nInflKeyNode;
		bInflIclImpFct = src.bInflIclImpFct;
		bLocalValue = src.bLocalValue;
		dCritLineVal = src.dCritLineVal;
		nPSCCirteriaType = src.nPSCCirteriaType;
		nFillType = src.nFillType;
		dPSCDgrmScFc = src.dPSCDgrmScFc;
		nDgrmDirType = src.nDgrmDirType;
		nDgrmDirOptn = src.nDgrmDirOptn;
		bPSCSignPos = src.bPSCSignPos;
		bPSCSignNeg = src.bPSCSignNeg;
		nResistanceLineCol = src.nResistanceLineCol;
		nResistanceLineThk = src.nResistanceLineThk;
		nPSCCompCH = src.nPSCCompCH;
		nPSCCompJP = src.nPSCCompJP;
		nPSCCode = src.nPSCCode;
		nDgnCode = src.nDgnCode;
		nMeshedDgn_ElemAvg = src.nMeshedDgn_ElemAvg;
		nMeshedDgn_ElemCell = src.nMeshedDgn_ElemCell;
		dMeshedDgn_CellLen = src.dMeshedDgn_CellLen;
		nMeshedDgn_XDirYDir = src.nMeshedDgn_XDirYDir;
		nMeshedDgn_RebarRatio = src.nMeshedDgn_RebarRatio;
		memcpy(dMeshedDgnOpt, src.dMeshedDgnOpt, sizeof(dMeshedDgnOpt));
		memcpy(nMeshedDgnOpt, src.nMeshedDgnOpt, sizeof(nMeshedDgnOpt));

		nWoodArmerTopBot = src.nWoodArmerTopBot;
		dWoodArmerAngle = src.dWoodArmerAngle;
		return *this;
	}
};
///////////////////////////////
struct T_FOUT_D
{
	int     nOutputLocation     ;
	double  dScFactor           ;
	double  dAdjustFactor       ;
	int     nAverage            ; // Element or Average 
	int     nLocalGlobal        ; // Local or UCS
	double  dMxxCoeff           ;
	double  dMyyCoeff           ;
	int     nMxxMyyMode         ;
	double  dMxyCoeff           ;
	UINT    NucsK               ; // for UCS Axis Display
	int     nPStrsSurface       ; // Top, Bottom, Both Sides, Abs Max
	UINT    nMbDgrmNPlanK       ; // Member Diagram..
	UINT    nMbDgrmStorK        ;
	int     nMbDgrmDeciPt       ;
	int     nMbDgrmDgrmType     ;
	double  dMbDgrmScFt         ;
	int     nMbdgrmDgrmComp     ;
	int     nMbDgrmOutType      ;
	UINT    nMbDgrmSelectMode   ;
	UINT    colValAxial         ;
	UINT    colValShear         ;
	UINT    colValMoment        ;
	int     nSSFROutType        ;
	double  dSSFRAngle          ;
	int     nThisComp           ; // Time History Selection (0: Displ, 1: Vel, 2: Accel)
	int     nThisFunction       ; // Time History Function
	UINT    nFunctionKey        ; // Time History Function
	int     nHingeTimeIndex     ; // Time History Step Combo
	double  dHingeTimeStep      ; // Time History Step
	int     nThisTimeIndex      ;
	double  dThisTimeStep       ;
	int     nHingeValType       ; // Pushover Type of Value
	int     nHingeDutl          ; // Pushover Type of Result
	CArray<UINT,UINT> aPfudClwpK; // Plate Cutting Line Diagram (checked list items key)
	int     nPfudComp           ; // Plate Cutting Line Diagram (Components)
	int     nPfudDir            ; // Plate Cutting Line Diagram (Options 0: Nomal, 1: In Plane)
	double  dEDgrmDefmScFc      ; // User Defined Diagram : Deformed Scale
	UINT    nEDgrmScFcType      ; // Scale Factor (0: Auto, 1: Ref. Value)
	double  dEDgrmAutoScFc      ; // Auto
	double  dEDgrmRefValue      ; // Ref. Value
	double  dEDgrmRefValueScFc  ; // Scale Factor
	CArray<UINT,UINT> aSelectedDiagK; // Selected Diagram Key
	BOOL    bFoutD[EN_FOUT_D_END];
	// Nodal Result of RS
	int     nInerForceOrAcc     ; // 0:Inertia Force , 1:Acceleration
	int     nUsedSerialModeIdx  ; // Mode Index
	BOOL    bMode               ; // Mode
	// Cutting Plane Detail
	BOOL    bOnCuttingLine      ;
	BOOL    bOnCuttingPlane     ;
	CArray<UINT,UINT&>  aCutPlanes;
	int		  nSolidFreeEdgeType  ;
	// IsoSurface Detail
	BOOL    bIsoSurface         ;
	CArray<double,double> aIsoValues;
	int	    nTransparentIntensity;
	int	    nIsoValueMode       ;
	BOOL	  bDrawPolyOutLine    ;
	BOOL	  bTransparent        ;
	// Cutting Diagram
	double  dCutLineScaleFactor ;
	BOOL    bCutLineOutputMinMax;
	CArray<UINT,UINT> aCutLineKeys;
	int     nCutPlaneHorizVert  ;
	COLORREF colCutPlaneColor   ;
	int     nCuttingLineDir     ; // 0: Normal, 1: In Plane
	COLORREF colCuttinLineColor ;
	BOOL    bDrawPolyLine       ;
	BOOL    bCutLineMinMaxOutOnly;
	BOOL    bCutLineReverse     ;
	BOOL    bShowCutLineGraph   ;
	int     nCutLineOrPlane     ; //0:Cutting Line,1:Cutting Plane
	// Influence Lines (Surfaces)
	UINT    nInflKeyNode        ;
	BOOL    bInflIclImpFct      ;
	BOOL    bLocalValue         ;
	// PSC Design Result Diagram
	double  dCritLineVal        ;
	int     nPSCCirteriaType    ;
	int     nFillType           ;
	double  dPSCDgrmScFc        ;
	int     nDgrmDirType        ;
	int     nDgrmDirOptn        ;
	BOOL    bPSCSignPos         ;
	BOOL    bPSCSignNeg         ;
	UINT    nResistanceLineCol  ;
	UINT    nResistanceLineThk  ;
	int     nPSCCompCH          ;
	int     nPSCCompJP          ;
	int     nPSCCode            ;
	int     nDgnCode            ;
	int     nMeshedDgn_ElemAvg  ;
	int     nMeshedDgn_ElemCell ;
	double  dMeshedDgn_CellLen  ;
	int     nMeshedDgn_XDirYDir  ;
	int     nMeshedDgn_RebarRatio;
	double  dMeshedDgnOpt[10]   ; // Meshed Design일 경우 AgenD.adIn 옵션 저장
	int     nMeshedDgnOpt[10]   ; // Meshed Design일 경우 AgenD.aiIn 옵션 저장

	int    nWoodArmerTopBot;  // Wood Armer Moment 정보
	double dWoodArmerAngle;   // Wood Armer Moment 정보

	// Resultant Force Diagram
	BOOL	bRFrcdTopAlignment;
	int		nRfrcdOutputPos;

	BOOL	bPsiFactor; // Moving Tracer Reaction Psi Factor

	// Mode Shape 
	BOOL	bCubicInterpolation;
	double	dCubicFactor;

	void Initialize()
	{
		nOutputLocation       = 0;
		dScFactor             = 1.0;
		dAdjustFactor         = 1.0;
		nAverage              = 0;
		nLocalGlobal          = 0;
		dMxxCoeff             = 1.0;
		dMyyCoeff             = 1.0;
		nMxxMyyMode           = 0;
		dMxyCoeff             = 1.0;
		NucsK                 = 0;
		nPStrsSurface         = 0;
		nMbDgrmNPlanK         = 0;
		nMbDgrmStorK          = 0;
		nMbDgrmDeciPt         = 0;
		nMbDgrmDgrmType       = 0;
		dMbDgrmScFt           = 0.;
		nMbdgrmDgrmComp       = 0;
		nMbDgrmOutType        = 0;
		nMbDgrmSelectMode     = 0;
		colValAxial           = 0;
		colValShear           = 0;
		colValMoment          = 0;
		nSSFROutType          = 0;
		dSSFRAngle            = 0.;    
		nThisComp             = 0;
		nThisFunction         = 0;
		nFunctionKey          = 0;
		nHingeTimeIndex       = 0;
		dHingeTimeStep        = 0.;
		nThisTimeIndex        = 0;
		dThisTimeStep         = 0.;
		nHingeValType         = 0;
		nHingeDutl            = 0;
		aPfudClwpK            .RemoveAll();
		nPfudComp             = 0;
		nPfudDir              = 0;
		dEDgrmDefmScFc        = 0.;
		nEDgrmScFcType        = 0;
		dEDgrmAutoScFc        = 0.;
		dEDgrmRefValue        = 0.;
		dEDgrmRefValueScFc    = 0.;
		aSelectedDiagK        .RemoveAll();
		for(int i=0; i<EN_FOUT_D_END; i++)
			bFoutD[i] = FALSE;
		nInerForceOrAcc       = 0;
		nUsedSerialModeIdx    = 0;
		bMode                 = TRUE;
		bOnCuttingLine        = FALSE;
		bOnCuttingPlane       = FALSE;
		aCutPlanes            .RemoveAll();
		nSolidFreeEdgeType    = 0;
		bIsoSurface           = FALSE;
		aIsoValues            .RemoveAll();
		nTransparentIntensity = 0;
		nIsoValueMode         = 0;
		bDrawPolyOutLine      = FALSE;
		bTransparent          = FALSE;
		dCutLineScaleFactor   = 0.;
		bCutLineOutputMinMax  = FALSE;
		aCutLineKeys          .RemoveAll();
		nCutPlaneHorizVert    = 0;
		colCutPlaneColor      = 0;
		nCuttingLineDir       = 0;
		colCuttinLineColor    = 0;
		bDrawPolyLine         = FALSE;
		bCutLineMinMaxOutOnly = FALSE;
		bCutLineReverse       = FALSE;
		bShowCutLineGraph     = FALSE;
		nCutLineOrPlane       = 0;
		nInflKeyNode          = 0;
		bInflIclImpFct        = FALSE;
		bLocalValue           = FALSE;
		dCritLineVal          = 0.;
		nPSCCirteriaType      = 0;
		nFillType             = 0;
		dPSCDgrmScFc          = 0.;
		nDgrmDirType          = 0;
		nDgrmDirOptn          = 0;
		bPSCSignPos           = FALSE;
		bPSCSignNeg           = FALSE;
		nResistanceLineCol    = 0;
		nResistanceLineThk    = 0;
		nPSCCompCH            = 0;
		nPSCCompJP            = 0;
		nPSCCode              = 0;
		nDgnCode              = 0;
		nMeshedDgn_ElemAvg    = 0;
		nMeshedDgn_ElemCell   = 0;
		dMeshedDgn_CellLen    = 1;
		nMeshedDgn_XDirYDir   = 0;
		nMeshedDgn_RebarRatio = 2;
		for(int i = 0; i<10; i++) { nMeshedDgnOpt[i]=0; dMeshedDgnOpt[i]=0.0; }

		nWoodArmerTopBot =0;  
		dWoodArmerAngle  =0.0;  

		bRFrcdTopAlignment = TRUE;
		nRfrcdOutputPos = 2;

		bPsiFactor = FALSE;

		bCubicInterpolation = FALSE;
		dCubicFactor = 0.5;
	}
	T_FOUT_D() { Initialize(); }
	T_FOUT_D(const T_FOUT_D &src) { *this = src; }
	T_FOUT_D& operator=(const T_FOUT_D &src)
	{
		nOutputLocation       = src.nOutputLocation       ;
		dScFactor             = src.dScFactor             ;
		dAdjustFactor         = src.dAdjustFactor         ;
		nAverage              = src.nAverage              ;
		nLocalGlobal          = src.nLocalGlobal          ;
		dMxxCoeff             = src.dMxxCoeff             ;
		dMyyCoeff             = src.dMyyCoeff             ;
		nMxxMyyMode           = src.nMxxMyyMode           ;
		dMxyCoeff             = src.dMxyCoeff             ;
		NucsK                 = src.NucsK                 ;
		nPStrsSurface         = src.nPStrsSurface         ;
		nMbDgrmNPlanK         = src.nMbDgrmNPlanK         ;
		nMbDgrmStorK          = src.nMbDgrmStorK          ;
		nMbDgrmDeciPt         = src.nMbDgrmDeciPt         ;
		nMbDgrmDgrmType       = src.nMbDgrmDgrmType       ;
		dMbDgrmScFt           = src.dMbDgrmScFt           ;
		nMbdgrmDgrmComp       = src.nMbdgrmDgrmComp       ;
		nMbDgrmOutType        = src.nMbDgrmOutType        ;
		nMbDgrmSelectMode     = src.nMbDgrmSelectMode     ;
		colValAxial           = src.colValAxial           ;
		colValShear           = src.colValShear           ;
		colValMoment          = src.colValMoment          ;
		nSSFROutType          = src.nSSFROutType          ;
		dSSFRAngle            = src.dSSFRAngle            ;    
		nThisComp             = src.nThisComp             ;
		nThisFunction         = src.nThisFunction         ;
		nFunctionKey          = src.nFunctionKey          ;
		nHingeTimeIndex       = src.nHingeTimeIndex       ;
		dHingeTimeStep        = src.dHingeTimeStep        ;
		nThisTimeIndex        = src.nThisTimeIndex        ;
		dThisTimeStep         = src.dThisTimeStep         ;
		nHingeValType         = src.nHingeValType         ;
		nHingeDutl            = src.nHingeDutl            ;
		aPfudClwpK            .Copy(src.aPfudClwpK       );
		nPfudComp             = src.nPfudComp             ;
		nPfudDir              = src.nPfudDir              ;
		dEDgrmDefmScFc        = src.dEDgrmDefmScFc        ;
		nEDgrmScFcType        = src.nEDgrmScFcType        ;
		dEDgrmAutoScFc        = src.dEDgrmAutoScFc        ;
		dEDgrmRefValue        = src.dEDgrmRefValue        ;
		dEDgrmRefValueScFc    = src.dEDgrmRefValueScFc    ;
		aSelectedDiagK        .Copy(src.aSelectedDiagK   );
		memcpy(bFoutD, src.bFoutD, sizeof(bFoutD));
		nInerForceOrAcc       = src.nInerForceOrAcc       ;
		nUsedSerialModeIdx    = src.nUsedSerialModeIdx    ;
		bMode                 = src.bMode                 ;
		bOnCuttingLine        = src.bOnCuttingLine        ;
		bOnCuttingPlane       = src.bOnCuttingPlane       ;
		aCutPlanes            .Copy(src.aCutPlanes       );
		nSolidFreeEdgeType    = src.nSolidFreeEdgeType    ;
		bIsoSurface           = src.bIsoSurface           ;
		aIsoValues            .Copy(src.aIsoValues       );
		nTransparentIntensity = src.nTransparentIntensity ;
		nIsoValueMode         = src.nIsoValueMode         ;
		bDrawPolyOutLine      = src.bDrawPolyOutLine      ;
		bTransparent          = src.bTransparent          ;
		dCutLineScaleFactor   = src.dCutLineScaleFactor   ;
		bCutLineOutputMinMax  = src.bCutLineOutputMinMax  ;
		aCutLineKeys          .Copy(src.aCutLineKeys     );
		nCutPlaneHorizVert    = src.nCutPlaneHorizVert    ;
		colCutPlaneColor      = src.colCutPlaneColor      ;
		nCuttingLineDir       = src.nCuttingLineDir       ;
		colCuttinLineColor    = src.colCuttinLineColor    ;
		bDrawPolyLine         = src.bDrawPolyLine         ;
		bCutLineMinMaxOutOnly = src.bCutLineMinMaxOutOnly ;
		bCutLineReverse       = src.bCutLineReverse       ;
		bShowCutLineGraph     = src.bShowCutLineGraph     ;
		nCutLineOrPlane       = src.nCutLineOrPlane       ;
		nInflKeyNode          = src.nInflKeyNode          ;
		bInflIclImpFct        = src.bInflIclImpFct        ;
		bLocalValue           = src.bLocalValue           ;
		dCritLineVal          = src.dCritLineVal          ;
		nPSCCirteriaType      = src.nPSCCirteriaType      ;
		nFillType             = src.nFillType             ;
		dPSCDgrmScFc          = src.dPSCDgrmScFc          ;
		nDgrmDirType          = src.nDgrmDirType          ;
		nDgrmDirOptn          = src.nDgrmDirOptn          ;
		bPSCSignPos           = src.bPSCSignPos           ;
		bPSCSignNeg           = src.bPSCSignNeg           ;
		nResistanceLineCol    = src.nResistanceLineCol    ;
		nResistanceLineThk    = src.nResistanceLineThk    ;
		nPSCCompCH            = src.nPSCCompCH            ;
		nPSCCompJP            = src.nPSCCompJP            ;
		nPSCCode              = src.nPSCCode              ;
		nDgnCode              = src.nDgnCode              ;
		nMeshedDgn_ElemAvg    = src.nMeshedDgn_ElemAvg    ;
		nMeshedDgn_ElemCell   = src.nMeshedDgn_ElemCell   ;
		dMeshedDgn_CellLen    = src.dMeshedDgn_CellLen    ;
		nMeshedDgn_XDirYDir   = src.nMeshedDgn_XDirYDir   ;
		nMeshedDgn_RebarRatio = src.nMeshedDgn_RebarRatio ;
		memcpy(dMeshedDgnOpt, src.dMeshedDgnOpt, sizeof(dMeshedDgnOpt));
		memcpy(nMeshedDgnOpt, src.nMeshedDgnOpt, sizeof(nMeshedDgnOpt));

		nWoodArmerTopBot =src.nWoodArmerTopBot;  
		dWoodArmerAngle  =src.dWoodArmerAngle;  

		bRFrcdTopAlignment = src.bRFrcdTopAlignment;
		nRfrcdOutputPos = src.nRfrcdOutputPos;
		bPsiFactor = src.bPsiFactor;
		bCubicInterpolation = src.bCubicInterpolation;
		dCubicFactor = src.dCubicFactor;
		return *this;
	}
	void Convert821(T_FOUT_D_821 &src)
	{
		nOutputLocation       = src.nOutputLocation       ;
		dScFactor             = src.dScFactor             ;
		dAdjustFactor         = src.dAdjustFactor         ;
		nAverage              = src.nAverage              ;
		nLocalGlobal          = src.nLocalGlobal          ;
		dMxxCoeff             = src.dMxxCoeff             ;
		dMyyCoeff             = src.dMyyCoeff             ;
		nMxxMyyMode           = src.nMxxMyyMode           ;
		dMxyCoeff             = src.dMxyCoeff             ;
		NucsK                 = src.NucsK                 ;
		nPStrsSurface         = src.nPStrsSurface         ;
		nMbDgrmNPlanK         = src.nMbDgrmNPlanK         ;
		nMbDgrmStorK          = src.nMbDgrmStorK          ;
		nMbDgrmDeciPt         = src.nMbDgrmDeciPt         ;
		nMbDgrmDgrmType       = src.nMbDgrmDgrmType       ;
		dMbDgrmScFt           = src.dMbDgrmScFt           ;
		nMbdgrmDgrmComp       = src.nMbdgrmDgrmComp       ;
		nMbDgrmOutType        = src.nMbDgrmOutType        ;
		nMbDgrmSelectMode     = src.nMbDgrmSelectMode     ;
		colValAxial           = src.colValAxial           ;
		colValShear           = src.colValShear           ;
		colValMoment          = src.colValMoment          ;
		nSSFROutType          = src.nSSFROutType          ;
		dSSFRAngle            = src.dSSFRAngle            ;    
		nThisComp             = src.nThisComp             ;
		nThisFunction         = src.nThisFunction         ;
		nFunctionKey          = src.nFunctionKey          ;
		nHingeTimeIndex       = src.nHingeTimeIndex       ;
		dHingeTimeStep        = src.dHingeTimeStep        ;
		nThisTimeIndex        = src.nThisTimeIndex        ;
		dThisTimeStep         = src.dThisTimeStep         ;
		nHingeValType         = src.nHingeValType         ;
		nHingeDutl            = src.nHingeDutl            ;
		aPfudClwpK            .Copy(src.aPfudClwpK       );
		nPfudComp             = src.nPfudComp             ;
		nPfudDir              = src.nPfudDir              ;
		dEDgrmDefmScFc        = src.dEDgrmDefmScFc        ;
		nEDgrmScFcType        = src.nEDgrmScFcType        ;
		dEDgrmAutoScFc        = src.dEDgrmAutoScFc        ;
		dEDgrmRefValue        = src.dEDgrmRefValue        ;
		dEDgrmRefValueScFc    = src.dEDgrmRefValueScFc    ;
		aSelectedDiagK        .Copy(src.aSelectedDiagK   );
		memcpy(bFoutD, src.bFoutD, sizeof(bFoutD));
		nInerForceOrAcc       = src.nInerForceOrAcc       ;
		nUsedSerialModeIdx    = src.nUsedSerialModeIdx    ;
		bMode                 = src.bMode                 ;
		bOnCuttingLine        = src.bOnCuttingLine        ;
		bOnCuttingPlane       = src.bOnCuttingPlane       ;
		aCutPlanes            .Copy(src.aCutPlanes       );
		nSolidFreeEdgeType    = src.nSolidFreeEdgeType    ;
		bIsoSurface           = src.bIsoSurface           ;
		aIsoValues            .Copy(src.aIsoValues       );
		nTransparentIntensity = src.nTransparentIntensity ;
		nIsoValueMode         = src.nIsoValueMode         ;
		bDrawPolyOutLine      = src.bDrawPolyOutLine      ;
		bTransparent          = src.bTransparent          ;
		dCutLineScaleFactor   = src.dCutLineScaleFactor   ;
		bCutLineOutputMinMax  = src.bCutLineOutputMinMax  ;
		aCutLineKeys          .Copy(src.aCutLineKeys     );
		nCutPlaneHorizVert    = src.nCutPlaneHorizVert    ;
		colCutPlaneColor      = src.colCutPlaneColor      ;
		nCuttingLineDir       = src.nCuttingLineDir       ;
		colCuttinLineColor    = src.colCuttinLineColor    ;
		bDrawPolyLine         = src.bDrawPolyLine         ;
		bCutLineMinMaxOutOnly = src.bCutLineMinMaxOutOnly ;
		bCutLineReverse       = src.bCutLineReverse       ;
		bShowCutLineGraph     = src.bShowCutLineGraph     ;
		nCutLineOrPlane       = src.nCutLineOrPlane       ;
		nInflKeyNode          = src.nInflKeyNode          ;
		bInflIclImpFct        = src.bInflIclImpFct        ;
		bLocalValue           = src.bLocalValue           ;
		dCritLineVal          = src.dCritLineVal          ;
		nPSCCirteriaType      = src.nPSCCirteriaType      ;
		nFillType             = src.nFillType             ;
		dPSCDgrmScFc          = src.dPSCDgrmScFc          ;
		nDgrmDirType          = src.nDgrmDirType          ;
		nDgrmDirOptn          = src.nDgrmDirOptn          ;
		bPSCSignPos           = src.bPSCSignPos           ;
		bPSCSignNeg           = src.bPSCSignNeg           ;
		nResistanceLineCol    = src.nResistanceLineCol    ;
		nResistanceLineThk    = src.nResistanceLineThk    ;
		nPSCCompCH            = src.nPSCCompCH            ;
		nPSCCompJP            = src.nPSCCompJP            ;
		nPSCCode              = src.nPSCCode              ;
		nDgnCode              = src.nDgnCode              ;
		nMeshedDgn_ElemAvg    = 0                         ;
		nMeshedDgn_ElemCell   = 0                         ;
		dMeshedDgn_CellLen    = 1.0                       ;
		nMeshedDgn_XDirYDir   = 0                         ;
		nMeshedDgn_RebarRatio = 1                       ;
		for(int i=0; i<10; i++) { nMeshedDgnOpt[i]=0; dMeshedDgnOpt[i]=0.0; }

		nWoodArmerTopBot =0;  
		dWoodArmerAngle  =0.0;  

		bRFrcdTopAlignment = TRUE;
		nRfrcdOutputPos = 2;

		bPsiFactor = FALSE;

		bCubicInterpolation = FALSE;
		dCubicFactor = 0.5;
	}
	void Convert825(T_FOUT_D_825 &src)
	{
		nOutputLocation       = src.nOutputLocation       ;
		dScFactor             = src.dScFactor             ;
		dAdjustFactor         = src.dAdjustFactor         ;
		nAverage              = src.nAverage              ;
		nLocalGlobal          = src.nLocalGlobal          ;
		dMxxCoeff             = src.dMxxCoeff             ;
		dMyyCoeff             = src.dMyyCoeff             ;
		nMxxMyyMode           = src.nMxxMyyMode           ;
		dMxyCoeff             = src.dMxyCoeff             ;
		NucsK                 = src.NucsK                 ;
		nPStrsSurface         = src.nPStrsSurface         ;
		nMbDgrmNPlanK         = src.nMbDgrmNPlanK         ;
		nMbDgrmStorK          = src.nMbDgrmStorK          ;
		nMbDgrmDeciPt         = src.nMbDgrmDeciPt         ;
		nMbDgrmDgrmType       = src.nMbDgrmDgrmType       ;
		dMbDgrmScFt           = src.dMbDgrmScFt           ;
		nMbdgrmDgrmComp       = src.nMbdgrmDgrmComp       ;
		nMbDgrmOutType        = src.nMbDgrmOutType        ;
		nMbDgrmSelectMode     = src.nMbDgrmSelectMode     ;
		colValAxial           = src.colValAxial           ;
		colValShear           = src.colValShear           ;
		colValMoment          = src.colValMoment          ;
		nSSFROutType          = src.nSSFROutType          ;
		dSSFRAngle            = src.dSSFRAngle            ;    
		nThisComp             = src.nThisComp             ;
		nThisFunction         = src.nThisFunction         ;
		nFunctionKey          = src.nFunctionKey          ;
		nHingeTimeIndex       = src.nHingeTimeIndex       ;
		dHingeTimeStep        = src.dHingeTimeStep        ;
		nThisTimeIndex        = src.nThisTimeIndex        ;
		dThisTimeStep         = src.dThisTimeStep         ;
		nHingeValType         = src.nHingeValType         ;
		nHingeDutl            = src.nHingeDutl            ;
		aPfudClwpK            .Copy(src.aPfudClwpK       );
		nPfudComp             = src.nPfudComp             ;
		nPfudDir              = src.nPfudDir              ;
		dEDgrmDefmScFc        = src.dEDgrmDefmScFc        ;
		nEDgrmScFcType        = src.nEDgrmScFcType        ;
		dEDgrmAutoScFc        = src.dEDgrmAutoScFc        ;
		dEDgrmRefValue        = src.dEDgrmRefValue        ;
		dEDgrmRefValueScFc    = src.dEDgrmRefValueScFc    ;
		aSelectedDiagK        .Copy(src.aSelectedDiagK   );
		memcpy(bFoutD, src.bFoutD, sizeof(bFoutD));
		nInerForceOrAcc       = src.nInerForceOrAcc       ;
		nUsedSerialModeIdx    = src.nUsedSerialModeIdx    ;
		bMode                 = src.bMode                 ;
		bOnCuttingLine        = src.bOnCuttingLine        ;
		bOnCuttingPlane       = src.bOnCuttingPlane       ;
		aCutPlanes            .Copy(src.aCutPlanes       );
		nSolidFreeEdgeType    = src.nSolidFreeEdgeType    ;
		bIsoSurface           = src.bIsoSurface           ;
		aIsoValues            .Copy(src.aIsoValues       );
		nTransparentIntensity = src.nTransparentIntensity ;
		nIsoValueMode         = src.nIsoValueMode         ;
		bDrawPolyOutLine      = src.bDrawPolyOutLine      ;
		bTransparent          = src.bTransparent          ;
		dCutLineScaleFactor   = src.dCutLineScaleFactor   ;
		bCutLineOutputMinMax  = src.bCutLineOutputMinMax  ;
		aCutLineKeys          .Copy(src.aCutLineKeys     );
		nCutPlaneHorizVert    = src.nCutPlaneHorizVert    ;
		colCutPlaneColor      = src.colCutPlaneColor      ;
		nCuttingLineDir       = src.nCuttingLineDir       ;
		colCuttinLineColor    = src.colCuttinLineColor    ;
		bDrawPolyLine         = src.bDrawPolyLine         ;
		bCutLineMinMaxOutOnly = src.bCutLineMinMaxOutOnly ;
		bCutLineReverse       = src.bCutLineReverse       ;
		bShowCutLineGraph     = src.bShowCutLineGraph     ;
		nCutLineOrPlane       = src.nCutLineOrPlane       ;
		nInflKeyNode          = src.nInflKeyNode          ;
		bInflIclImpFct        = src.bInflIclImpFct        ;
		bLocalValue           = src.bLocalValue           ;
		dCritLineVal          = src.dCritLineVal          ;
		nPSCCirteriaType      = src.nPSCCirteriaType      ;
		nFillType             = src.nFillType             ;
		dPSCDgrmScFc          = src.dPSCDgrmScFc          ;
		nDgrmDirType          = src.nDgrmDirType          ;
		nDgrmDirOptn          = src.nDgrmDirOptn          ;
		bPSCSignPos           = src.bPSCSignPos           ;
		bPSCSignNeg           = src.bPSCSignNeg           ;
		nResistanceLineCol    = src.nResistanceLineCol    ;
		nResistanceLineThk    = src.nResistanceLineThk    ;
		nPSCCompCH            = src.nPSCCompCH            ;
		nPSCCompJP            = src.nPSCCompJP            ;
		nPSCCode              = src.nPSCCode              ;
		nDgnCode              = src.nDgnCode              ;
		nMeshedDgn_ElemAvg    = src.nMeshedDgn_ElemAvg    ;
		nMeshedDgn_ElemCell   = src.nMeshedDgn_ElemCell   ;
		dMeshedDgn_CellLen    = src.dMeshedDgn_CellLen    ;
		nMeshedDgn_XDirYDir   = src.nMeshedDgn_XDirYDir   ;
		nMeshedDgn_RebarRatio = src.nMeshedDgn_RebarRatio ;
		for(int i=0; i<10; i++) { nMeshedDgnOpt[i]=0; dMeshedDgnOpt[i]=0.0; }

		nWoodArmerTopBot =0;  
		dWoodArmerAngle  =0.0;  

		bRFrcdTopAlignment = TRUE;
		nRfrcdOutputPos = 2;

		bPsiFactor = FALSE;

		bCubicInterpolation = FALSE;
		dCubicFactor = 0.5;
	}
	void Convert851(T_FOUT_D_851 &src)
	{
		nOutputLocation       = src.nOutputLocation       ;
		dScFactor             = src.dScFactor             ;
		dAdjustFactor         = src.dAdjustFactor         ;
		nAverage              = src.nAverage              ;
		nLocalGlobal          = src.nLocalGlobal          ;
		dMxxCoeff             = src.dMxxCoeff             ;
		dMyyCoeff             = src.dMyyCoeff             ;
		nMxxMyyMode           = src.nMxxMyyMode           ;
		dMxyCoeff             = src.dMxyCoeff             ;
		NucsK                 = src.NucsK                 ;
		nPStrsSurface         = src.nPStrsSurface         ;
		nMbDgrmNPlanK         = src.nMbDgrmNPlanK         ;
		nMbDgrmStorK          = src.nMbDgrmStorK          ;
		nMbDgrmDeciPt         = src.nMbDgrmDeciPt         ;
		nMbDgrmDgrmType       = src.nMbDgrmDgrmType       ;
		dMbDgrmScFt           = src.dMbDgrmScFt           ;
		nMbdgrmDgrmComp       = src.nMbdgrmDgrmComp       ;
		nMbDgrmOutType        = src.nMbDgrmOutType        ;
		nMbDgrmSelectMode     = src.nMbDgrmSelectMode     ;
		colValAxial           = src.colValAxial           ;
		colValShear           = src.colValShear           ;
		colValMoment          = src.colValMoment          ;
		nSSFROutType          = src.nSSFROutType          ;
		dSSFRAngle            = src.dSSFRAngle            ;    
		nThisComp             = src.nThisComp             ;
		nThisFunction         = src.nThisFunction         ;
		nFunctionKey          = src.nFunctionKey          ;
		nHingeTimeIndex       = src.nHingeTimeIndex       ;
		dHingeTimeStep        = src.dHingeTimeStep        ;
		nThisTimeIndex        = src.nThisTimeIndex        ;
		dThisTimeStep         = src.dThisTimeStep         ;
		nHingeValType         = src.nHingeValType         ;
		nHingeDutl            = src.nHingeDutl            ;
		aPfudClwpK            .Copy(src.aPfudClwpK       );
		nPfudComp             = src.nPfudComp             ;
		nPfudDir              = src.nPfudDir              ;
		dEDgrmDefmScFc        = src.dEDgrmDefmScFc        ;
		nEDgrmScFcType        = src.nEDgrmScFcType        ;
		dEDgrmAutoScFc        = src.dEDgrmAutoScFc        ;
		dEDgrmRefValue        = src.dEDgrmRefValue        ;
		dEDgrmRefValueScFc    = src.dEDgrmRefValueScFc    ;
		aSelectedDiagK        .Copy(src.aSelectedDiagK   );
		memcpy(bFoutD, src.bFoutD, sizeof(bFoutD));
		nInerForceOrAcc       = src.nInerForceOrAcc       ;
		nUsedSerialModeIdx    = src.nUsedSerialModeIdx    ;
		bMode                 = src.bMode                 ;
		bOnCuttingLine        = src.bOnCuttingLine        ;
		bOnCuttingPlane       = src.bOnCuttingPlane       ;
		aCutPlanes            .Copy(src.aCutPlanes       );
		nSolidFreeEdgeType    = src.nSolidFreeEdgeType    ;
		bIsoSurface           = src.bIsoSurface           ;
		aIsoValues            .Copy(src.aIsoValues       );
		nTransparentIntensity = src.nTransparentIntensity ;
		nIsoValueMode         = src.nIsoValueMode         ;
		bDrawPolyOutLine      = src.bDrawPolyOutLine      ;
		bTransparent          = src.bTransparent          ;
		dCutLineScaleFactor   = src.dCutLineScaleFactor   ;
		bCutLineOutputMinMax  = src.bCutLineOutputMinMax  ;
		aCutLineKeys          .Copy(src.aCutLineKeys     );
		nCutPlaneHorizVert    = src.nCutPlaneHorizVert    ;
		colCutPlaneColor      = src.colCutPlaneColor      ;
		nCuttingLineDir       = src.nCuttingLineDir       ;
		colCuttinLineColor    = src.colCuttinLineColor    ;
		bDrawPolyLine         = src.bDrawPolyLine         ;
		bCutLineMinMaxOutOnly = src.bCutLineMinMaxOutOnly ;
		bCutLineReverse       = src.bCutLineReverse       ;
		bShowCutLineGraph     = src.bShowCutLineGraph     ;
		nCutLineOrPlane       = src.nCutLineOrPlane       ;
		nInflKeyNode          = src.nInflKeyNode          ;
		bInflIclImpFct        = src.bInflIclImpFct        ;
		bLocalValue           = src.bLocalValue           ;
		dCritLineVal          = src.dCritLineVal          ;
		nPSCCirteriaType      = src.nPSCCirteriaType      ;
		nFillType             = src.nFillType             ;
		dPSCDgrmScFc          = src.dPSCDgrmScFc          ;
		nDgrmDirType          = src.nDgrmDirType          ;
		nDgrmDirOptn          = src.nDgrmDirOptn          ;
		bPSCSignPos           = src.bPSCSignPos           ;
		bPSCSignNeg           = src.bPSCSignNeg           ;
		nResistanceLineCol    = src.nResistanceLineCol    ;
		nResistanceLineThk    = src.nResistanceLineThk    ;
		nPSCCompCH            = src.nPSCCompCH            ;
		nPSCCompJP            = src.nPSCCompJP            ;
		nPSCCode              = src.nPSCCode              ;
		nDgnCode              = src.nDgnCode              ;
		nMeshedDgn_ElemAvg    = src.nMeshedDgn_ElemAvg    ;
		nMeshedDgn_ElemCell   = src.nMeshedDgn_ElemCell   ;
		dMeshedDgn_CellLen    = src.dMeshedDgn_CellLen    ;
		nMeshedDgn_XDirYDir   = src.nMeshedDgn_XDirYDir   ;
		nMeshedDgn_RebarRatio = src.nMeshedDgn_RebarRatio ;
		memcpy(dMeshedDgnOpt, src.dMeshedDgnOpt, sizeof(dMeshedDgnOpt));
		memcpy(nMeshedDgnOpt, src.nMeshedDgnOpt, sizeof(nMeshedDgnOpt));

		nWoodArmerTopBot =0;  
		dWoodArmerAngle  =0.0;  

		bRFrcdTopAlignment = TRUE;
		nRfrcdOutputPos = 2;

		bPsiFactor = FALSE;

		bCubicInterpolation = FALSE;
		dCubicFactor = 0,5;
	}
	void Convert940(T_FOUT_D_940& src)
	{
		nOutputLocation = src.nOutputLocation;
		dScFactor = src.dScFactor;
		dAdjustFactor = src.dAdjustFactor;
		nAverage = src.nAverage;
		nLocalGlobal = src.nLocalGlobal;
		dMxxCoeff = src.dMxxCoeff;
		dMyyCoeff = src.dMyyCoeff;
		nMxxMyyMode = src.nMxxMyyMode;
		dMxyCoeff = src.dMxyCoeff;
		NucsK = src.NucsK;
		nPStrsSurface = src.nPStrsSurface;
		nMbDgrmNPlanK = src.nMbDgrmNPlanK;
		nMbDgrmStorK = src.nMbDgrmStorK;
		nMbDgrmDeciPt = src.nMbDgrmDeciPt;
		nMbDgrmDgrmType = src.nMbDgrmDgrmType;
		dMbDgrmScFt = src.dMbDgrmScFt;
		nMbdgrmDgrmComp = src.nMbdgrmDgrmComp;
		nMbDgrmOutType = src.nMbDgrmOutType;
		nMbDgrmSelectMode = src.nMbDgrmSelectMode;
		colValAxial = src.colValAxial;
		colValShear = src.colValShear;
		colValMoment = src.colValMoment;
		nSSFROutType = src.nSSFROutType;
		dSSFRAngle = src.dSSFRAngle;
		nThisComp = src.nThisComp;
		nThisFunction = src.nThisFunction;
		nFunctionKey = src.nFunctionKey;
		nHingeTimeIndex = src.nHingeTimeIndex;
		dHingeTimeStep = src.dHingeTimeStep;
		nThisTimeIndex = src.nThisTimeIndex;
		dThisTimeStep = src.dThisTimeStep;
		nHingeValType = src.nHingeValType;
		nHingeDutl = src.nHingeDutl;
		aPfudClwpK.Copy(src.aPfudClwpK);
		nPfudComp = src.nPfudComp;
		nPfudDir = src.nPfudDir;
		dEDgrmDefmScFc = src.dEDgrmDefmScFc;
		nEDgrmScFcType = src.nEDgrmScFcType;
		dEDgrmAutoScFc = src.dEDgrmAutoScFc;
		dEDgrmRefValue = src.dEDgrmRefValue;
		dEDgrmRefValueScFc = src.dEDgrmRefValueScFc;
		aSelectedDiagK.Copy(src.aSelectedDiagK);
		memcpy(bFoutD, src.bFoutD, sizeof(bFoutD));
		nInerForceOrAcc = src.nInerForceOrAcc;
		nUsedSerialModeIdx = src.nUsedSerialModeIdx;
		bMode = src.bMode;
		bOnCuttingLine = src.bOnCuttingLine;
		bOnCuttingPlane = src.bOnCuttingPlane;
		aCutPlanes.Copy(src.aCutPlanes);
		nSolidFreeEdgeType = src.nSolidFreeEdgeType;
		bIsoSurface = src.bIsoSurface;
		aIsoValues.Copy(src.aIsoValues);
		nTransparentIntensity = src.nTransparentIntensity;
		nIsoValueMode = src.nIsoValueMode;
		bDrawPolyOutLine = src.bDrawPolyOutLine;
		bTransparent = src.bTransparent;
		dCutLineScaleFactor = src.dCutLineScaleFactor;
		bCutLineOutputMinMax = src.bCutLineOutputMinMax;
		aCutLineKeys.Copy(src.aCutLineKeys);
		nCutPlaneHorizVert = src.nCutPlaneHorizVert;
		colCutPlaneColor = src.colCutPlaneColor;
		nCuttingLineDir = src.nCuttingLineDir;
		colCuttinLineColor = src.colCuttinLineColor;
		bDrawPolyLine = src.bDrawPolyLine;
		bCutLineMinMaxOutOnly = src.bCutLineMinMaxOutOnly;
		bCutLineReverse = src.bCutLineReverse;
		bShowCutLineGraph = src.bShowCutLineGraph;
		nCutLineOrPlane = src.nCutLineOrPlane;
		nInflKeyNode = src.nInflKeyNode;
		bInflIclImpFct = src.bInflIclImpFct;
		bLocalValue = src.bLocalValue;
		dCritLineVal = src.dCritLineVal;
		nPSCCirteriaType = src.nPSCCirteriaType;
		nFillType = src.nFillType;
		dPSCDgrmScFc = src.dPSCDgrmScFc;
		nDgrmDirType = src.nDgrmDirType;
		nDgrmDirOptn = src.nDgrmDirOptn;
		bPSCSignPos = src.bPSCSignPos;
		bPSCSignNeg = src.bPSCSignNeg;
		nResistanceLineCol = src.nResistanceLineCol;
		nResistanceLineThk = src.nResistanceLineThk;
		nPSCCompCH = src.nPSCCompCH;
		nPSCCompJP = src.nPSCCompJP;
		nPSCCode = src.nPSCCode;
		nDgnCode = src.nDgnCode;
		nMeshedDgn_ElemAvg = src.nMeshedDgn_ElemAvg;
		nMeshedDgn_ElemCell = src.nMeshedDgn_ElemCell;
		dMeshedDgn_CellLen = src.dMeshedDgn_CellLen;
		nMeshedDgn_XDirYDir = src.nMeshedDgn_XDirYDir;
		nMeshedDgn_RebarRatio = src.nMeshedDgn_RebarRatio;
		memcpy(dMeshedDgnOpt, src.dMeshedDgnOpt, sizeof(dMeshedDgnOpt));
		memcpy(nMeshedDgnOpt, src.nMeshedDgnOpt, sizeof(nMeshedDgnOpt));

		nWoodArmerTopBot = src.nWoodArmerTopBot;
		dWoodArmerAngle = src.dWoodArmerAngle;

		bRFrcdTopAlignment = TRUE;
		nRfrcdOutputPos = 2;

		bPsiFactor = FALSE;

		bCubicInterpolation = FALSE;
		dCubicFactor = 0.5;
	}
};

// v821 : Meshed Design
struct T_FOUT_D_RW_821
{
	int     nOutputLocation     ;
	double  dScFactor           ;
	double  dAdjustFactor       ;
	int     nAverage            ;
	int     nLocalGlobal        ;
	double  dMxxCoeff           ;
	double  dMyyCoeff           ;
	int     nMxxMyyMode         ;
	double  dMxyCoeff           ;
	UINT    NucsK               ;
	int     nPStrsSurface       ;
	UINT    nMbDgrmNPlanK       ;
	UINT    nMbDgrmStorK        ;
	int     nMbDgrmDeciPt       ;
	int     nMbDgrmDgrmType     ;
	double  dMbDgrmScFt         ;
	int     nMbdgrmDgrmComp     ;
	int     nMbDgrmOutType      ;
	UINT    nMbDgrmSelectMode   ;
	UINT    colValAxial         ;
	UINT    colValShear         ;
	UINT    colValMoment        ;
	int     nSSFROutType        ;
	double  dSSFRAngle          ;
	int     nThisComp           ;
	int     nThisFunction       ;
	UINT    nFunctionKey        ;
	int     nHingeTimeIndex     ;
	double  dHingeTimeStep      ;
	int     nThisTimeIndex      ;
	double  dThisTimeStep       ;
	int     nHingeValType       ;
	int     nHingeDutl          ;
	int     nCount_aPfudClwpK   ;
	int     nPfudComp           ;
	int     nPfudDir            ;
	double  dEDgrmDefmScFc      ;
	UINT    nEDgrmScFcType      ;
	double  dEDgrmAutoScFc      ;
	double  dEDgrmRefValue      ;
	double  dEDgrmRefValueScFc  ;
	int     nCount_aSelectedDiagK;
	BOOL    bFoutD[EN_FOUT_D_END];
	int     nInerForceOrAcc     ;
	int     nUsedSerialModeIdx  ;
	BOOL    bMode               ;
	BOOL    bOnCuttingLine      ;
	BOOL    bOnCuttingPlane     ;
	int     nCount_aCutPlanes   ;
	int		  nSolidFreeEdgeType  ;
	BOOL    bIsoSurface         ;
	int     nCount_aIsoValues   ;
	int	    nTransparentIntensity;
	int	    nIsoValueMode       ;
	BOOL	  bDrawPolyOutLine    ;
	BOOL	  bTransparent        ;
	double  dCutLineScaleFactor ;
	BOOL    bCutLineOutputMinMax;
	int     nCount_aCutLineKeys ;
	int     nCutPlaneHorizVert  ;
	COLORREF colCutPlaneColor   ;
	int     nCuttingLineDir     ;
	COLORREF colCuttinLineColor ;
	BOOL    bDrawPolyLine       ;
	BOOL    bCutLineMinMaxOutOnly;
	BOOL    bCutLineReverse     ;
	BOOL    bShowCutLineGraph   ;
	int     nCutLineOrPlane     ;
	UINT    nInflKeyNode        ;
	BOOL    bInflIclImpFct      ;
	BOOL    bLocalValue         ;
	double  dCritLineVal        ;
	int     nPSCCirteriaType    ;
	int     nFillType           ;
	double  dPSCDgrmScFc        ;
	int     nDgrmDirType        ;
	int     nDgrmDirOptn        ;
	BOOL    bPSCSignPos         ;
	BOOL    bPSCSignNeg         ;
	UINT    nResistanceLineCol  ;
	UINT    nResistanceLineThk  ;
	int     nPSCCompCH          ;
	int     nPSCCompJP          ;
	int     nPSCCode            ;
	int     nDgnCode            ;

	void Get(T_FOUT_D_821 &rData)
	{
		rData.nOutputLocation        = nOutputLocation      ;
		rData.dScFactor              = dScFactor            ;
		rData.dAdjustFactor          = dAdjustFactor        ;
		rData.nAverage               = nAverage             ;
		rData.nLocalGlobal           = nLocalGlobal         ;
		rData.dMxxCoeff              = dMxxCoeff            ;
		rData.dMyyCoeff              = dMyyCoeff            ;
		rData.nMxxMyyMode            = nMxxMyyMode          ;
		rData.dMxyCoeff              = dMxyCoeff            ;
		rData.NucsK                  = NucsK                ;
		rData.nPStrsSurface          = nPStrsSurface        ;
		rData.nMbDgrmNPlanK          = nMbDgrmNPlanK        ;
		rData.nMbDgrmStorK           = nMbDgrmStorK         ;
		rData.nMbDgrmDeciPt          = nMbDgrmDeciPt        ;
		rData.nMbDgrmDgrmType        = nMbDgrmDgrmType      ;
		rData.dMbDgrmScFt            = dMbDgrmScFt          ;
		rData.nMbdgrmDgrmComp        = nMbdgrmDgrmComp      ;
		rData.nMbDgrmOutType         = nMbDgrmOutType       ;
		rData.nMbDgrmSelectMode      = nMbDgrmSelectMode    ;
		rData.colValAxial            = colValAxial          ;
		rData.colValShear            = colValShear          ;
		rData.colValMoment           = colValMoment         ;
		rData.nSSFROutType           = nSSFROutType         ;
		rData.dSSFRAngle             = dSSFRAngle           ;    
		rData.nThisComp              = nThisComp            ;
		rData.nThisFunction          = nThisFunction        ;
		rData.nFunctionKey           = nFunctionKey         ;
		rData.nHingeTimeIndex        = nHingeTimeIndex      ;
		rData.dHingeTimeStep         = dHingeTimeStep       ;
		rData.nThisTimeIndex         = nThisTimeIndex       ;
		rData.dThisTimeStep          = dThisTimeStep        ;
		rData.nHingeValType          = nHingeValType        ;
		rData.nHingeDutl             = nHingeDutl           ;
		rData.aPfudClwpK.SetSize(nCount_aPfudClwpK);
		rData.nPfudComp              = nPfudComp            ;
		rData.nPfudDir               = nPfudDir             ;
		rData.dEDgrmDefmScFc         = dEDgrmDefmScFc       ;
		rData.nEDgrmScFcType         = nEDgrmScFcType       ;
		rData.dEDgrmAutoScFc         = dEDgrmAutoScFc       ;
		rData.dEDgrmRefValue         = dEDgrmRefValue       ;
		rData.dEDgrmRefValueScFc     = dEDgrmRefValueScFc   ;
		rData.aSelectedDiagK.SetSize(nCount_aSelectedDiagK);
		memcpy(rData.bFoutD, bFoutD, sizeof(rData.bFoutD));
		rData.nInerForceOrAcc        = nInerForceOrAcc      ;
		rData.nUsedSerialModeIdx     = nUsedSerialModeIdx   ;
		rData.bMode                  = bMode                ;
		rData.bOnCuttingLine        = bOnCuttingLine        ;
		rData.bOnCuttingPlane       = bOnCuttingPlane       ;
		rData.aCutPlanes.SetSize(nCount_aCutPlanes);
		rData.nSolidFreeEdgeType    = nSolidFreeEdgeType    ;
		rData.bIsoSurface           = bIsoSurface           ;
		rData.aIsoValues.SetSize(nCount_aIsoValues);
		rData.nTransparentIntensity = nTransparentIntensity ;
		rData.nIsoValueMode         = nIsoValueMode         ;
		rData.bDrawPolyOutLine      = bDrawPolyOutLine      ;
		rData.bTransparent          = bTransparent          ;
		rData.dCutLineScaleFactor   = dCutLineScaleFactor   ;
		rData.bCutLineOutputMinMax  = bCutLineOutputMinMax  ;
		rData.aCutLineKeys.SetSize(nCount_aCutLineKeys);
		rData.nCutPlaneHorizVert    = nCutPlaneHorizVert    ;
		rData.colCutPlaneColor      = colCutPlaneColor      ;
		rData.nCuttingLineDir       = nCuttingLineDir       ;
		rData.colCuttinLineColor    = colCuttinLineColor    ;
		rData.bDrawPolyLine         = bDrawPolyLine         ;
		rData.bCutLineMinMaxOutOnly = bCutLineMinMaxOutOnly ;
		rData.bCutLineReverse       = bCutLineReverse       ;
		rData.bShowCutLineGraph     = bShowCutLineGraph     ;
		rData.nCutLineOrPlane       = nCutLineOrPlane       ;
		rData.nInflKeyNode          = nInflKeyNode          ;
		rData.bInflIclImpFct        = bInflIclImpFct        ;
		rData.bLocalValue           = bLocalValue           ;
		rData.dCritLineVal          = dCritLineVal          ;
		rData.nPSCCirteriaType      = nPSCCirteriaType      ;
		rData.nFillType             = nFillType             ;
		rData.dPSCDgrmScFc          = dPSCDgrmScFc          ;
		rData.nDgrmDirType          = nDgrmDirType          ;
		rData.nDgrmDirOptn          = nDgrmDirOptn          ;
		rData.bPSCSignPos           = bPSCSignPos           ;
		rData.bPSCSignNeg           = bPSCSignNeg           ;
		rData.nResistanceLineCol    = nResistanceLineCol    ;
		rData.nResistanceLineThk    = nResistanceLineThk    ;
		rData.nPSCCompCH            = nPSCCompCH            ;
		rData.nPSCCompJP            = nPSCCompJP            ;
		rData.nPSCCode              = nPSCCode              ;
		rData.nDgnCode              = nDgnCode              ;
	}
	void Set(T_FOUT_D_821 Data)
	{
		nOutputLocation       = Data.nOutputLocation       ;
		dScFactor             = Data.dScFactor             ;
		dAdjustFactor         = Data.dAdjustFactor         ;
		nAverage              = Data.nAverage              ;
		nLocalGlobal          = Data.nLocalGlobal          ;
		dMxxCoeff             = Data.dMxxCoeff             ;
		dMyyCoeff             = Data.dMyyCoeff             ;
		nMxxMyyMode           = Data.nMxxMyyMode           ;
		dMxyCoeff             = Data.dMxyCoeff             ;
		NucsK                 = Data.NucsK                 ;
		nPStrsSurface         = Data.nPStrsSurface         ;
		nMbDgrmNPlanK         = Data.nMbDgrmNPlanK         ;
		nMbDgrmStorK          = Data.nMbDgrmStorK          ;
		nMbDgrmDeciPt         = Data.nMbDgrmDeciPt         ;
		nMbDgrmDgrmType       = Data.nMbDgrmDgrmType       ;
		dMbDgrmScFt           = Data.dMbDgrmScFt           ;
		nMbdgrmDgrmComp       = Data.nMbdgrmDgrmComp       ;
		nMbDgrmOutType        = Data.nMbDgrmOutType        ;
		nMbDgrmSelectMode     = Data.nMbDgrmSelectMode     ;
		colValAxial           = Data.colValAxial           ;
		colValShear           = Data.colValShear           ;
		colValMoment          = Data.colValMoment          ;
		nSSFROutType          = Data.nSSFROutType          ;
		dSSFRAngle            = Data.dSSFRAngle            ;    
		nThisComp             = Data.nThisComp             ;
		nThisFunction         = Data.nThisFunction         ;
		nFunctionKey          = Data.nFunctionKey          ;
		nHingeTimeIndex       = Data.nHingeTimeIndex       ;
		dHingeTimeStep        = Data.dHingeTimeStep        ;
		nThisTimeIndex        = Data.nThisTimeIndex        ;
		dThisTimeStep         = Data.dThisTimeStep         ;
		nHingeValType         = Data.nHingeValType         ;
		nHingeDutl            = Data.nHingeDutl            ;
		nCount_aPfudClwpK = (int)Data.aPfudClwpK.GetSize();
		nPfudComp             = Data.nPfudComp             ;
		nPfudDir              = Data.nPfudDir              ;
		dEDgrmDefmScFc        = Data.dEDgrmDefmScFc        ;
		nEDgrmScFcType        = Data.nEDgrmScFcType        ;
		dEDgrmAutoScFc        = Data.dEDgrmAutoScFc        ;
		dEDgrmRefValue        = Data.dEDgrmRefValue        ;
		dEDgrmRefValueScFc    = Data.dEDgrmRefValueScFc    ;
		nCount_aSelectedDiagK = (int)Data.aSelectedDiagK.GetSize();
		memcpy(bFoutD, Data.bFoutD, sizeof(bFoutD));
		nInerForceOrAcc       = Data.nInerForceOrAcc       ;
		nUsedSerialModeIdx    = Data.nUsedSerialModeIdx    ;
		bMode                 = Data.bMode                 ;
		bOnCuttingLine        = Data.bOnCuttingLine        ;
		bOnCuttingPlane       = Data.bOnCuttingPlane       ;
		nCount_aCutPlanes     = (int)Data.aCutPlanes.GetSize()  ;
		nSolidFreeEdgeType    = Data.nSolidFreeEdgeType    ;
		bIsoSurface           = Data.bIsoSurface           ;
		nCount_aIsoValues     = (int)Data.aIsoValues.GetSize()  ;
		nTransparentIntensity = Data.nTransparentIntensity ;
		nIsoValueMode         = Data.nIsoValueMode         ;
		bDrawPolyOutLine      = Data.bDrawPolyOutLine      ;
		bTransparent          = Data.bTransparent          ;
		dCutLineScaleFactor   = Data.dCutLineScaleFactor   ;
		bCutLineOutputMinMax  = Data.bCutLineOutputMinMax  ;
		nCount_aCutLineKeys   = (int)Data.aCutLineKeys.GetSize();
		nCutPlaneHorizVert    = Data.nCutPlaneHorizVert    ;
		colCutPlaneColor      = Data.colCutPlaneColor      ;
		nCuttingLineDir       = Data.nCuttingLineDir       ;
		colCuttinLineColor    = Data.colCuttinLineColor    ;
		bDrawPolyLine         = Data.bDrawPolyLine         ;
		bCutLineMinMaxOutOnly = Data.bCutLineMinMaxOutOnly ;
		bCutLineReverse       = Data.bCutLineReverse       ;
		bShowCutLineGraph     = Data.bShowCutLineGraph     ;
		nCutLineOrPlane       = Data.nCutLineOrPlane       ;
		nInflKeyNode          = Data.nInflKeyNode          ;
		bInflIclImpFct        = Data.bInflIclImpFct        ;
		bLocalValue           = Data.bLocalValue           ;
		dCritLineVal          = Data.dCritLineVal          ;
		nPSCCirteriaType      = Data.nPSCCirteriaType      ;
		nFillType             = Data.nFillType             ;
		dPSCDgrmScFc          = Data.dPSCDgrmScFc          ;
		nDgrmDirType          = Data.nDgrmDirType          ;
		nDgrmDirOptn          = Data.nDgrmDirOptn          ;
		bPSCSignPos           = Data.bPSCSignPos           ;
		bPSCSignNeg           = Data.bPSCSignNeg           ;
		nResistanceLineCol    = Data.nResistanceLineCol    ;
		nResistanceLineThk    = Data.nResistanceLineThk    ;
		nPSCCompCH            = Data.nPSCCompCH            ;
		nPSCCompJP            = Data.nPSCCompJP            ;
		nPSCCode              = Data.nPSCCode              ;
		nDgnCode              = Data.nDgnCode              ;
	}
};

struct T_FOUT_D_RW_825
{
	int     nOutputLocation     ;
	double  dScFactor           ;
	double  dAdjustFactor       ;
	int     nAverage            ;
	int     nLocalGlobal        ;
	double  dMxxCoeff           ;
	double  dMyyCoeff           ;
	int     nMxxMyyMode         ;
	double  dMxyCoeff           ;
	UINT    NucsK               ;
	int     nPStrsSurface       ;
	UINT    nMbDgrmNPlanK       ;
	UINT    nMbDgrmStorK        ;
	int     nMbDgrmDeciPt       ;
	int     nMbDgrmDgrmType     ;
	double  dMbDgrmScFt         ;
	int     nMbdgrmDgrmComp     ;
	int     nMbDgrmOutType      ;
	UINT    nMbDgrmSelectMode   ;
	UINT    colValAxial         ;
	UINT    colValShear         ;
	UINT    colValMoment        ;
	int     nSSFROutType        ;
	double  dSSFRAngle          ;
	int     nThisComp           ;
	int     nThisFunction       ;
	UINT    nFunctionKey        ;
	int     nHingeTimeIndex     ;
	double  dHingeTimeStep      ;
	int     nThisTimeIndex      ;
	double  dThisTimeStep       ;
	int     nHingeValType       ;
	int     nHingeDutl          ;
	int     nCount_aPfudClwpK   ;
	int     nPfudComp           ;
	int     nPfudDir            ;
	double  dEDgrmDefmScFc      ;
	UINT    nEDgrmScFcType      ;
	double  dEDgrmAutoScFc      ;
	double  dEDgrmRefValue      ;
	double  dEDgrmRefValueScFc  ;
	int     nCount_aSelectedDiagK;
	BOOL    bFoutD[EN_FOUT_D_END];
	int     nInerForceOrAcc     ;
	int     nUsedSerialModeIdx  ;
	BOOL    bMode               ;
	BOOL    bOnCuttingLine      ;
	BOOL    bOnCuttingPlane     ;
	int     nCount_aCutPlanes   ;
	int		  nSolidFreeEdgeType  ;
	BOOL    bIsoSurface         ;
	int     nCount_aIsoValues   ;
	int	    nTransparentIntensity;
	int	    nIsoValueMode       ;
	BOOL	  bDrawPolyOutLine    ;
	BOOL	  bTransparent        ;
	double  dCutLineScaleFactor ;
	BOOL    bCutLineOutputMinMax;
	int     nCount_aCutLineKeys ;
	int     nCutPlaneHorizVert  ;
	COLORREF colCutPlaneColor   ;
	int     nCuttingLineDir     ;
	COLORREF colCuttinLineColor ;
	BOOL    bDrawPolyLine       ;
	BOOL    bCutLineMinMaxOutOnly;
	BOOL    bCutLineReverse     ;
	BOOL    bShowCutLineGraph   ;
	int     nCutLineOrPlane     ;
	UINT    nInflKeyNode        ;
	BOOL    bInflIclImpFct      ;
	BOOL    bLocalValue         ;
	double  dCritLineVal        ;
	int     nPSCCirteriaType    ;
	int     nFillType           ;
	double  dPSCDgrmScFc        ;
	int     nDgrmDirType        ;
	int     nDgrmDirOptn        ;
	BOOL    bPSCSignPos         ;
	BOOL    bPSCSignNeg         ;
	UINT    nResistanceLineCol  ;
	UINT    nResistanceLineThk  ;
	int     nPSCCompCH          ;
	int     nPSCCompJP          ;
	int     nPSCCode            ;
	int     nDgnCode            ;
	int     nMeshedDgn_ElemAvg  ;
	int     nMeshedDgn_ElemCell ;
	double  dMeshedDgn_CellLen  ;
	int     nMeshedDgn_XDirYDir  ;
	int     nMeshedDgn_RebarRatio;

	void Get(T_FOUT_D_825 &rData)
	{
		rData.nOutputLocation        = nOutputLocation      ;
		rData.dScFactor              = dScFactor            ;
		rData.dAdjustFactor          = dAdjustFactor        ;
		rData.nAverage               = nAverage             ;
		rData.nLocalGlobal           = nLocalGlobal         ;
		rData.dMxxCoeff              = dMxxCoeff            ;
		rData.dMyyCoeff              = dMyyCoeff            ;
		rData.nMxxMyyMode            = nMxxMyyMode          ;
		rData.dMxyCoeff              = dMxyCoeff            ;
		rData.NucsK                  = NucsK                ;
		rData.nPStrsSurface          = nPStrsSurface        ;
		rData.nMbDgrmNPlanK          = nMbDgrmNPlanK        ;
		rData.nMbDgrmStorK           = nMbDgrmStorK         ;
		rData.nMbDgrmDeciPt          = nMbDgrmDeciPt        ;
		rData.nMbDgrmDgrmType        = nMbDgrmDgrmType      ;
		rData.dMbDgrmScFt            = dMbDgrmScFt          ;
		rData.nMbdgrmDgrmComp        = nMbdgrmDgrmComp      ;
		rData.nMbDgrmOutType         = nMbDgrmOutType       ;
		rData.nMbDgrmSelectMode      = nMbDgrmSelectMode    ;
		rData.colValAxial            = colValAxial          ;
		rData.colValShear            = colValShear          ;
		rData.colValMoment           = colValMoment         ;
		rData.nSSFROutType           = nSSFROutType         ;
		rData.dSSFRAngle             = dSSFRAngle           ;    
		rData.nThisComp              = nThisComp            ;
		rData.nThisFunction          = nThisFunction        ;
		rData.nFunctionKey           = nFunctionKey         ;
		rData.nHingeTimeIndex        = nHingeTimeIndex      ;
		rData.dHingeTimeStep         = dHingeTimeStep       ;
		rData.nThisTimeIndex         = nThisTimeIndex       ;
		rData.dThisTimeStep          = dThisTimeStep        ;
		rData.nHingeValType          = nHingeValType        ;
		rData.nHingeDutl             = nHingeDutl           ;
		rData.aPfudClwpK.SetSize(nCount_aPfudClwpK);
		rData.nPfudComp              = nPfudComp            ;
		rData.nPfudDir               = nPfudDir             ;
		rData.dEDgrmDefmScFc         = dEDgrmDefmScFc       ;
		rData.nEDgrmScFcType         = nEDgrmScFcType       ;
		rData.dEDgrmAutoScFc         = dEDgrmAutoScFc       ;
		rData.dEDgrmRefValue         = dEDgrmRefValue       ;
		rData.dEDgrmRefValueScFc     = dEDgrmRefValueScFc   ;
		rData.aSelectedDiagK.SetSize(nCount_aSelectedDiagK);
		memcpy(rData.bFoutD, bFoutD, sizeof(rData.bFoutD));
		rData.nInerForceOrAcc        = nInerForceOrAcc      ;
		rData.nUsedSerialModeIdx     = nUsedSerialModeIdx   ;
		rData.bMode                  = bMode                ;
		rData.bOnCuttingLine        = bOnCuttingLine        ;
		rData.bOnCuttingPlane       = bOnCuttingPlane       ;
		rData.aCutPlanes.SetSize(nCount_aCutPlanes);
		rData.nSolidFreeEdgeType    = nSolidFreeEdgeType    ;
		rData.bIsoSurface           = bIsoSurface           ;
		rData.aIsoValues.SetSize(nCount_aIsoValues);
		rData.nTransparentIntensity = nTransparentIntensity ;
		rData.nIsoValueMode         = nIsoValueMode         ;
		rData.bDrawPolyOutLine      = bDrawPolyOutLine      ;
		rData.bTransparent          = bTransparent          ;
		rData.dCutLineScaleFactor   = dCutLineScaleFactor   ;
		rData.bCutLineOutputMinMax  = bCutLineOutputMinMax  ;
		rData.aCutLineKeys.SetSize(nCount_aCutLineKeys);
		rData.nCutPlaneHorizVert    = nCutPlaneHorizVert    ;
		rData.colCutPlaneColor      = colCutPlaneColor      ;
		rData.nCuttingLineDir       = nCuttingLineDir       ;
		rData.colCuttinLineColor    = colCuttinLineColor    ;
		rData.bDrawPolyLine         = bDrawPolyLine         ;
		rData.bCutLineMinMaxOutOnly = bCutLineMinMaxOutOnly ;
		rData.bCutLineReverse       = bCutLineReverse       ;
		rData.bShowCutLineGraph     = bShowCutLineGraph     ;
		rData.nCutLineOrPlane       = nCutLineOrPlane       ;
		rData.nInflKeyNode          = nInflKeyNode          ;
		rData.bInflIclImpFct        = bInflIclImpFct        ;
		rData.bLocalValue           = bLocalValue           ;
		rData.dCritLineVal          = dCritLineVal          ;
		rData.nPSCCirteriaType      = nPSCCirteriaType      ;
		rData.nFillType             = nFillType             ;
		rData.dPSCDgrmScFc          = dPSCDgrmScFc          ;
		rData.nDgrmDirType          = nDgrmDirType          ;
		rData.nDgrmDirOptn          = nDgrmDirOptn          ;
		rData.bPSCSignPos           = bPSCSignPos           ;
		rData.bPSCSignNeg           = bPSCSignNeg           ;
		rData.nResistanceLineCol    = nResistanceLineCol    ;
		rData.nResistanceLineThk    = nResistanceLineThk    ;
		rData.nPSCCompCH            = nPSCCompCH            ;
		rData.nPSCCompJP            = nPSCCompJP            ;
		rData.nPSCCode              = nPSCCode              ;
		rData.nDgnCode              = nDgnCode              ;
		rData.nMeshedDgn_ElemAvg    = nMeshedDgn_ElemAvg    ;
		rData.nMeshedDgn_ElemCell   = nMeshedDgn_ElemCell   ;
		rData.dMeshedDgn_CellLen    = dMeshedDgn_CellLen    ;
		rData.nMeshedDgn_XDirYDir   = nMeshedDgn_XDirYDir   ;
		rData.nMeshedDgn_RebarRatio = nMeshedDgn_RebarRatio ;
	}
	void Set(T_FOUT_D_825 Data)
	{
		nOutputLocation       = Data.nOutputLocation       ;
		dScFactor             = Data.dScFactor             ;
		dAdjustFactor         = Data.dAdjustFactor         ;
		nAverage              = Data.nAverage              ;
		nLocalGlobal          = Data.nLocalGlobal          ;
		dMxxCoeff             = Data.dMxxCoeff             ;
		dMyyCoeff             = Data.dMyyCoeff             ;
		nMxxMyyMode           = Data.nMxxMyyMode           ;
		dMxyCoeff             = Data.dMxyCoeff             ;
		NucsK                 = Data.NucsK                 ;
		nPStrsSurface         = Data.nPStrsSurface         ;
		nMbDgrmNPlanK         = Data.nMbDgrmNPlanK         ;
		nMbDgrmStorK          = Data.nMbDgrmStorK          ;
		nMbDgrmDeciPt         = Data.nMbDgrmDeciPt         ;
		nMbDgrmDgrmType       = Data.nMbDgrmDgrmType       ;
		dMbDgrmScFt           = Data.dMbDgrmScFt           ;
		nMbdgrmDgrmComp       = Data.nMbdgrmDgrmComp       ;
		nMbDgrmOutType        = Data.nMbDgrmOutType        ;
		nMbDgrmSelectMode     = Data.nMbDgrmSelectMode     ;
		colValAxial           = Data.colValAxial           ;
		colValShear           = Data.colValShear           ;
		colValMoment          = Data.colValMoment          ;
		nSSFROutType          = Data.nSSFROutType          ;
		dSSFRAngle            = Data.dSSFRAngle            ;    
		nThisComp             = Data.nThisComp             ;
		nThisFunction         = Data.nThisFunction         ;
		nFunctionKey          = Data.nFunctionKey          ;
		nHingeTimeIndex       = Data.nHingeTimeIndex       ;
		dHingeTimeStep        = Data.dHingeTimeStep        ;
		nThisTimeIndex        = Data.nThisTimeIndex        ;
		dThisTimeStep         = Data.dThisTimeStep         ;
		nHingeValType         = Data.nHingeValType         ;
		nHingeDutl            = Data.nHingeDutl            ;
		nCount_aPfudClwpK = (int)Data.aPfudClwpK.GetSize();
		nPfudComp             = Data.nPfudComp             ;
		nPfudDir              = Data.nPfudDir              ;
		dEDgrmDefmScFc        = Data.dEDgrmDefmScFc        ;
		nEDgrmScFcType        = Data.nEDgrmScFcType        ;
		dEDgrmAutoScFc        = Data.dEDgrmAutoScFc        ;
		dEDgrmRefValue        = Data.dEDgrmRefValue        ;
		dEDgrmRefValueScFc    = Data.dEDgrmRefValueScFc    ;
		nCount_aSelectedDiagK = (int)Data.aSelectedDiagK.GetSize();
		memcpy(bFoutD, Data.bFoutD, sizeof(bFoutD));
		nInerForceOrAcc       = Data.nInerForceOrAcc       ;
		nUsedSerialModeIdx    = Data.nUsedSerialModeIdx    ;
		bMode                 = Data.bMode                 ;
		bOnCuttingLine        = Data.bOnCuttingLine        ;
		bOnCuttingPlane       = Data.bOnCuttingPlane       ;
		nCount_aCutPlanes     = (int)Data.aCutPlanes.GetSize()  ;
		nSolidFreeEdgeType    = Data.nSolidFreeEdgeType    ;
		bIsoSurface           = Data.bIsoSurface           ;
		nCount_aIsoValues     = (int)Data.aIsoValues.GetSize()  ;
		nTransparentIntensity = Data.nTransparentIntensity ;
		nIsoValueMode         = Data.nIsoValueMode         ;
		bDrawPolyOutLine      = Data.bDrawPolyOutLine      ;
		bTransparent          = Data.bTransparent          ;
		dCutLineScaleFactor   = Data.dCutLineScaleFactor   ;
		bCutLineOutputMinMax  = Data.bCutLineOutputMinMax  ;
		nCount_aCutLineKeys   = (int)Data.aCutLineKeys.GetSize();
		nCutPlaneHorizVert    = Data.nCutPlaneHorizVert    ;
		colCutPlaneColor      = Data.colCutPlaneColor      ;
		nCuttingLineDir       = Data.nCuttingLineDir       ;
		colCuttinLineColor    = Data.colCuttinLineColor    ;
		bDrawPolyLine         = Data.bDrawPolyLine         ;
		bCutLineMinMaxOutOnly = Data.bCutLineMinMaxOutOnly ;
		bCutLineReverse       = Data.bCutLineReverse       ;
		bShowCutLineGraph     = Data.bShowCutLineGraph     ;
		nCutLineOrPlane       = Data.nCutLineOrPlane       ;
		nInflKeyNode          = Data.nInflKeyNode          ;
		bInflIclImpFct        = Data.bInflIclImpFct        ;
		bLocalValue           = Data.bLocalValue           ;
		dCritLineVal          = Data.dCritLineVal          ;
		nPSCCirteriaType      = Data.nPSCCirteriaType      ;
		nFillType             = Data.nFillType             ;
		dPSCDgrmScFc          = Data.dPSCDgrmScFc          ;
		nDgrmDirType          = Data.nDgrmDirType          ;
		nDgrmDirOptn          = Data.nDgrmDirOptn          ;
		bPSCSignPos           = Data.bPSCSignPos           ;
		bPSCSignNeg           = Data.bPSCSignNeg           ;
		nResistanceLineCol    = Data.nResistanceLineCol    ;
		nResistanceLineThk    = Data.nResistanceLineThk    ;
		nPSCCompCH            = Data.nPSCCompCH            ;
		nPSCCompJP            = Data.nPSCCompJP            ;
		nPSCCode              = Data.nPSCCode              ;
		nDgnCode              = Data.nDgnCode              ;
		nMeshedDgn_ElemAvg    = Data.nMeshedDgn_ElemAvg    ;
		nMeshedDgn_ElemCell   = Data.nMeshedDgn_ElemCell   ;
		dMeshedDgn_CellLen    = Data.dMeshedDgn_CellLen    ;
		nMeshedDgn_XDirYDir   = Data.nMeshedDgn_XDirYDir   ;
		nMeshedDgn_RebarRatio = Data.nMeshedDgn_RebarRatio ;
	}
};

struct T_FOUT_D_RW_851
{
	int     nOutputLocation     ;
	double  dScFactor           ;
	double  dAdjustFactor       ;
	int     nAverage            ;
	int     nLocalGlobal        ;
	double  dMxxCoeff           ;
	double  dMyyCoeff           ;
	int     nMxxMyyMode         ;
	double  dMxyCoeff           ;
	UINT    NucsK               ;
	int     nPStrsSurface       ;
	UINT    nMbDgrmNPlanK       ;
	UINT    nMbDgrmStorK        ;
	int     nMbDgrmDeciPt       ;
	int     nMbDgrmDgrmType     ;
	double  dMbDgrmScFt         ;
	int     nMbdgrmDgrmComp     ;
	int     nMbDgrmOutType      ;
	UINT    nMbDgrmSelectMode   ;
	UINT    colValAxial         ;
	UINT    colValShear         ;
	UINT    colValMoment        ;
	int     nSSFROutType        ;
	double  dSSFRAngle          ;
	int     nThisComp           ;
	int     nThisFunction       ;
	UINT    nFunctionKey        ;
	int     nHingeTimeIndex     ;
	double  dHingeTimeStep      ;
	int     nThisTimeIndex      ;
	double  dThisTimeStep       ;
	int     nHingeValType       ;
	int     nHingeDutl          ;
	int     nCount_aPfudClwpK   ;
	int     nPfudComp           ;
	int     nPfudDir            ;
	double  dEDgrmDefmScFc      ;
	UINT    nEDgrmScFcType      ;
	double  dEDgrmAutoScFc      ;
	double  dEDgrmRefValue      ;
	double  dEDgrmRefValueScFc  ;
	int     nCount_aSelectedDiagK;
	BOOL    bFoutD[EN_FOUT_D_END];
	int     nInerForceOrAcc     ;
	int     nUsedSerialModeIdx  ;
	BOOL    bMode               ;
	BOOL    bOnCuttingLine      ;
	BOOL    bOnCuttingPlane     ;
	int     nCount_aCutPlanes   ;
	int		  nSolidFreeEdgeType  ;
	BOOL    bIsoSurface         ;
	int     nCount_aIsoValues   ;
	int	    nTransparentIntensity;
	int	    nIsoValueMode       ;
	BOOL	  bDrawPolyOutLine    ;
	BOOL	  bTransparent        ;
	double  dCutLineScaleFactor ;
	BOOL    bCutLineOutputMinMax;
	int     nCount_aCutLineKeys ;
	int     nCutPlaneHorizVert  ;
	COLORREF colCutPlaneColor   ;
	int     nCuttingLineDir     ;
	COLORREF colCuttinLineColor ;
	BOOL    bDrawPolyLine       ;
	BOOL    bCutLineMinMaxOutOnly;
	BOOL    bCutLineReverse     ;
	BOOL    bShowCutLineGraph   ;
	int     nCutLineOrPlane     ;
	UINT    nInflKeyNode        ;
	BOOL    bInflIclImpFct      ;
	BOOL    bLocalValue         ;
	double  dCritLineVal        ;
	int     nPSCCirteriaType    ;
	int     nFillType           ;
	double  dPSCDgrmScFc        ;
	int     nDgrmDirType        ;
	int     nDgrmDirOptn        ;
	BOOL    bPSCSignPos         ;
	BOOL    bPSCSignNeg         ;
	UINT    nResistanceLineCol  ;
	UINT    nResistanceLineThk  ;
	int     nPSCCompCH          ;
	int     nPSCCompJP          ;
	int     nPSCCode            ;
	int     nDgnCode            ;
	int     nMeshedDgn_ElemAvg  ;
	int     nMeshedDgn_ElemCell ;
	double  dMeshedDgn_CellLen  ;
	int     nMeshedDgn_XDirYDir  ;
	int     nMeshedDgn_RebarRatio;
	double  dMeshedDgnOpt[10]   ; // Meshed Design일 경우 AgenD.adIn 옵션 저장
	int     nMeshedDgnOpt[10]   ; // Meshed Design일 경우 AgenD.aiIn 옵션 저장

	void Get(T_FOUT_D_851 &rData)
	{
		rData.nOutputLocation        = nOutputLocation      ;
		rData.dScFactor              = dScFactor            ;
		rData.dAdjustFactor          = dAdjustFactor        ;
		rData.nAverage               = nAverage             ;
		rData.nLocalGlobal           = nLocalGlobal         ;
		rData.dMxxCoeff              = dMxxCoeff            ;
		rData.dMyyCoeff              = dMyyCoeff            ;
		rData.nMxxMyyMode            = nMxxMyyMode          ;
		rData.dMxyCoeff              = dMxyCoeff            ;
		rData.NucsK                  = NucsK                ;
		rData.nPStrsSurface          = nPStrsSurface        ;
		rData.nMbDgrmNPlanK          = nMbDgrmNPlanK        ;
		rData.nMbDgrmStorK           = nMbDgrmStorK         ;
		rData.nMbDgrmDeciPt          = nMbDgrmDeciPt        ;
		rData.nMbDgrmDgrmType        = nMbDgrmDgrmType      ;
		rData.dMbDgrmScFt            = dMbDgrmScFt          ;
		rData.nMbdgrmDgrmComp        = nMbdgrmDgrmComp      ;
		rData.nMbDgrmOutType         = nMbDgrmOutType       ;
		rData.nMbDgrmSelectMode      = nMbDgrmSelectMode    ;
		rData.colValAxial            = colValAxial          ;
		rData.colValShear            = colValShear          ;
		rData.colValMoment           = colValMoment         ;
		rData.nSSFROutType           = nSSFROutType         ;
		rData.dSSFRAngle             = dSSFRAngle           ;    
		rData.nThisComp              = nThisComp            ;
		rData.nThisFunction          = nThisFunction        ;
		rData.nFunctionKey           = nFunctionKey         ;
		rData.nHingeTimeIndex        = nHingeTimeIndex      ;
		rData.dHingeTimeStep         = dHingeTimeStep       ;
		rData.nThisTimeIndex         = nThisTimeIndex       ;
		rData.dThisTimeStep          = dThisTimeStep        ;
		rData.nHingeValType          = nHingeValType        ;
		rData.nHingeDutl             = nHingeDutl           ;
		rData.aPfudClwpK.SetSize(nCount_aPfudClwpK);
		rData.nPfudComp              = nPfudComp            ;
		rData.nPfudDir               = nPfudDir             ;
		rData.dEDgrmDefmScFc         = dEDgrmDefmScFc       ;
		rData.nEDgrmScFcType         = nEDgrmScFcType       ;
		rData.dEDgrmAutoScFc         = dEDgrmAutoScFc       ;
		rData.dEDgrmRefValue         = dEDgrmRefValue       ;
		rData.dEDgrmRefValueScFc     = dEDgrmRefValueScFc   ;
		rData.aSelectedDiagK.SetSize(nCount_aSelectedDiagK);
		memcpy(rData.bFoutD, bFoutD, sizeof(rData.bFoutD));
		rData.nInerForceOrAcc        = nInerForceOrAcc      ;
		rData.nUsedSerialModeIdx     = nUsedSerialModeIdx   ;
		rData.bMode                  = bMode                ;
		rData.bOnCuttingLine        = bOnCuttingLine        ;
		rData.bOnCuttingPlane       = bOnCuttingPlane       ;
		rData.aCutPlanes.SetSize(nCount_aCutPlanes);
		rData.nSolidFreeEdgeType    = nSolidFreeEdgeType    ;
		rData.bIsoSurface           = bIsoSurface           ;
		rData.aIsoValues.SetSize(nCount_aIsoValues);
		rData.nTransparentIntensity = nTransparentIntensity ;
		rData.nIsoValueMode         = nIsoValueMode         ;
		rData.bDrawPolyOutLine      = bDrawPolyOutLine      ;
		rData.bTransparent          = bTransparent          ;
		rData.dCutLineScaleFactor   = dCutLineScaleFactor   ;
		rData.bCutLineOutputMinMax  = bCutLineOutputMinMax  ;
		rData.aCutLineKeys.SetSize(nCount_aCutLineKeys);
		rData.nCutPlaneHorizVert    = nCutPlaneHorizVert    ;
		rData.colCutPlaneColor      = colCutPlaneColor      ;
		rData.nCuttingLineDir       = nCuttingLineDir       ;
		rData.colCuttinLineColor    = colCuttinLineColor    ;
		rData.bDrawPolyLine         = bDrawPolyLine         ;
		rData.bCutLineMinMaxOutOnly = bCutLineMinMaxOutOnly ;
		rData.bCutLineReverse       = bCutLineReverse       ;
		rData.bShowCutLineGraph     = bShowCutLineGraph     ;
		rData.nCutLineOrPlane       = nCutLineOrPlane       ;
		rData.nInflKeyNode          = nInflKeyNode          ;
		rData.bInflIclImpFct        = bInflIclImpFct        ;
		rData.bLocalValue           = bLocalValue           ;
		rData.dCritLineVal          = dCritLineVal          ;
		rData.nPSCCirteriaType      = nPSCCirteriaType      ;
		rData.nFillType             = nFillType             ;
		rData.dPSCDgrmScFc          = dPSCDgrmScFc          ;
		rData.nDgrmDirType          = nDgrmDirType          ;
		rData.nDgrmDirOptn          = nDgrmDirOptn          ;
		rData.bPSCSignPos           = bPSCSignPos           ;
		rData.bPSCSignNeg           = bPSCSignNeg           ;
		rData.nResistanceLineCol    = nResistanceLineCol    ;
		rData.nResistanceLineThk    = nResistanceLineThk    ;
		rData.nPSCCompCH            = nPSCCompCH            ;
		rData.nPSCCompJP            = nPSCCompJP            ;
		rData.nPSCCode              = nPSCCode              ;
		rData.nDgnCode              = nDgnCode              ;
		rData.nMeshedDgn_ElemAvg    = nMeshedDgn_ElemAvg    ;
		rData.nMeshedDgn_ElemCell   = nMeshedDgn_ElemCell   ;
		rData.dMeshedDgn_CellLen    = dMeshedDgn_CellLen    ;
		rData.nMeshedDgn_XDirYDir   = nMeshedDgn_XDirYDir   ;
		rData.nMeshedDgn_RebarRatio = nMeshedDgn_RebarRatio ;
		memcpy(rData.dMeshedDgnOpt, dMeshedDgnOpt, sizeof(rData.dMeshedDgnOpt));
		memcpy(rData.nMeshedDgnOpt, nMeshedDgnOpt, sizeof(rData.nMeshedDgnOpt));
	}
	void Set(T_FOUT_D_851 Data)
	{
		nOutputLocation       = Data.nOutputLocation       ;
		dScFactor             = Data.dScFactor             ;
		dAdjustFactor         = Data.dAdjustFactor         ;
		nAverage              = Data.nAverage              ;
		nLocalGlobal          = Data.nLocalGlobal          ;
		dMxxCoeff             = Data.dMxxCoeff             ;
		dMyyCoeff             = Data.dMyyCoeff             ;
		nMxxMyyMode           = Data.nMxxMyyMode           ;
		dMxyCoeff             = Data.dMxyCoeff             ;
		NucsK                 = Data.NucsK                 ;
		nPStrsSurface         = Data.nPStrsSurface         ;
		nMbDgrmNPlanK         = Data.nMbDgrmNPlanK         ;
		nMbDgrmStorK          = Data.nMbDgrmStorK          ;
		nMbDgrmDeciPt         = Data.nMbDgrmDeciPt         ;
		nMbDgrmDgrmType       = Data.nMbDgrmDgrmType       ;
		dMbDgrmScFt           = Data.dMbDgrmScFt           ;
		nMbdgrmDgrmComp       = Data.nMbdgrmDgrmComp       ;
		nMbDgrmOutType        = Data.nMbDgrmOutType        ;
		nMbDgrmSelectMode     = Data.nMbDgrmSelectMode     ;
		colValAxial           = Data.colValAxial           ;
		colValShear           = Data.colValShear           ;
		colValMoment          = Data.colValMoment          ;
		nSSFROutType          = Data.nSSFROutType          ;
		dSSFRAngle            = Data.dSSFRAngle            ;    
		nThisComp             = Data.nThisComp             ;
		nThisFunction         = Data.nThisFunction         ;
		nFunctionKey          = Data.nFunctionKey          ;
		nHingeTimeIndex       = Data.nHingeTimeIndex       ;
		dHingeTimeStep        = Data.dHingeTimeStep        ;
		nThisTimeIndex        = Data.nThisTimeIndex        ;
		dThisTimeStep         = Data.dThisTimeStep         ;
		nHingeValType         = Data.nHingeValType         ;
		nHingeDutl            = Data.nHingeDutl            ;
		nCount_aPfudClwpK = (int)Data.aPfudClwpK.GetSize();
		nPfudComp             = Data.nPfudComp             ;
		nPfudDir              = Data.nPfudDir              ;
		dEDgrmDefmScFc        = Data.dEDgrmDefmScFc        ;
		nEDgrmScFcType        = Data.nEDgrmScFcType        ;
		dEDgrmAutoScFc        = Data.dEDgrmAutoScFc        ;
		dEDgrmRefValue        = Data.dEDgrmRefValue        ;
		dEDgrmRefValueScFc    = Data.dEDgrmRefValueScFc    ;
		nCount_aSelectedDiagK = (int)Data.aSelectedDiagK.GetSize();
		memcpy(bFoutD, Data.bFoutD, sizeof(bFoutD));
		nInerForceOrAcc       = Data.nInerForceOrAcc       ;
		nUsedSerialModeIdx    = Data.nUsedSerialModeIdx    ;
		bMode                 = Data.bMode                 ;
		bOnCuttingLine        = Data.bOnCuttingLine        ;
		bOnCuttingPlane       = Data.bOnCuttingPlane       ;
		nCount_aCutPlanes     = (int)Data.aCutPlanes.GetSize()  ;
		nSolidFreeEdgeType    = Data.nSolidFreeEdgeType    ;
		bIsoSurface           = Data.bIsoSurface           ;
		nCount_aIsoValues     = (int)Data.aIsoValues.GetSize()  ;
		nTransparentIntensity = Data.nTransparentIntensity ;
		nIsoValueMode         = Data.nIsoValueMode         ;
		bDrawPolyOutLine      = Data.bDrawPolyOutLine      ;
		bTransparent          = Data.bTransparent          ;
		dCutLineScaleFactor   = Data.dCutLineScaleFactor   ;
		bCutLineOutputMinMax  = Data.bCutLineOutputMinMax  ;
		nCount_aCutLineKeys   = (int)Data.aCutLineKeys.GetSize();
		nCutPlaneHorizVert    = Data.nCutPlaneHorizVert    ;
		colCutPlaneColor      = Data.colCutPlaneColor      ;
		nCuttingLineDir       = Data.nCuttingLineDir       ;
		colCuttinLineColor    = Data.colCuttinLineColor    ;
		bDrawPolyLine         = Data.bDrawPolyLine         ;
		bCutLineMinMaxOutOnly = Data.bCutLineMinMaxOutOnly ;
		bCutLineReverse       = Data.bCutLineReverse       ;
		bShowCutLineGraph     = Data.bShowCutLineGraph     ;
		nCutLineOrPlane       = Data.nCutLineOrPlane       ;
		nInflKeyNode          = Data.nInflKeyNode          ;
		bInflIclImpFct        = Data.bInflIclImpFct        ;
		bLocalValue           = Data.bLocalValue           ;
		dCritLineVal          = Data.dCritLineVal          ;
		nPSCCirteriaType      = Data.nPSCCirteriaType      ;
		nFillType             = Data.nFillType             ;
		dPSCDgrmScFc          = Data.dPSCDgrmScFc          ;
		nDgrmDirType          = Data.nDgrmDirType          ;
		nDgrmDirOptn          = Data.nDgrmDirOptn          ;
		bPSCSignPos           = Data.bPSCSignPos           ;
		bPSCSignNeg           = Data.bPSCSignNeg           ;
		nResistanceLineCol    = Data.nResistanceLineCol    ;
		nResistanceLineThk    = Data.nResistanceLineThk    ;
		nPSCCompCH            = Data.nPSCCompCH            ;
		nPSCCompJP            = Data.nPSCCompJP            ;
		nPSCCode              = Data.nPSCCode              ;
		nDgnCode              = Data.nDgnCode              ;
		nMeshedDgn_ElemAvg    = Data.nMeshedDgn_ElemAvg    ;
		nMeshedDgn_ElemCell   = Data.nMeshedDgn_ElemCell   ;
		dMeshedDgn_CellLen    = Data.dMeshedDgn_CellLen    ;
		nMeshedDgn_XDirYDir   = Data.nMeshedDgn_XDirYDir   ;
		nMeshedDgn_RebarRatio = Data.nMeshedDgn_RebarRatio ;
		memcpy(dMeshedDgnOpt, Data.dMeshedDgnOpt, sizeof(dMeshedDgnOpt));
		memcpy(nMeshedDgnOpt, Data.nMeshedDgnOpt, sizeof(nMeshedDgnOpt));
	}
};

struct T_FOUT_D_RW_940
{
	int     nOutputLocation;
	double  dScFactor;
	double  dAdjustFactor;
	int     nAverage;
	int     nLocalGlobal;
	double  dMxxCoeff;
	double  dMyyCoeff;
	int     nMxxMyyMode;
	double  dMxyCoeff;
	UINT    NucsK;
	int     nPStrsSurface;
	UINT    nMbDgrmNPlanK;
	UINT    nMbDgrmStorK;
	int     nMbDgrmDeciPt;
	int     nMbDgrmDgrmType;
	double  dMbDgrmScFt;
	int     nMbdgrmDgrmComp;
	int     nMbDgrmOutType;
	UINT    nMbDgrmSelectMode;
	UINT    colValAxial;
	UINT    colValShear;
	UINT    colValMoment;
	int     nSSFROutType;
	double  dSSFRAngle;
	int     nThisComp;
	int     nThisFunction;
	UINT    nFunctionKey;
	int     nHingeTimeIndex;
	double  dHingeTimeStep;
	int     nThisTimeIndex;
	double  dThisTimeStep;
	int     nHingeValType;
	int     nHingeDutl;
	int     nCount_aPfudClwpK;
	int     nPfudComp;
	int     nPfudDir;
	double  dEDgrmDefmScFc;
	UINT    nEDgrmScFcType;
	double  dEDgrmAutoScFc;
	double  dEDgrmRefValue;
	double  dEDgrmRefValueScFc;
	int     nCount_aSelectedDiagK;
	BOOL    bFoutD[EN_FOUT_D_END];
	int     nInerForceOrAcc;
	int     nUsedSerialModeIdx;
	BOOL    bMode;
	BOOL    bOnCuttingLine;
	BOOL    bOnCuttingPlane;
	int     nCount_aCutPlanes;
	int		  nSolidFreeEdgeType;
	BOOL    bIsoSurface;
	int     nCount_aIsoValues;
	int	    nTransparentIntensity;
	int	    nIsoValueMode;
	BOOL	  bDrawPolyOutLine;
	BOOL	  bTransparent;
	double  dCutLineScaleFactor;
	BOOL    bCutLineOutputMinMax;
	int     nCount_aCutLineKeys;
	int     nCutPlaneHorizVert;
	COLORREF colCutPlaneColor;
	int     nCuttingLineDir;
	COLORREF colCuttinLineColor;
	BOOL    bDrawPolyLine;
	BOOL    bCutLineMinMaxOutOnly;
	BOOL    bCutLineReverse;
	BOOL    bShowCutLineGraph;
	int     nCutLineOrPlane;
	UINT    nInflKeyNode;
	BOOL    bInflIclImpFct;
	BOOL    bLocalValue;
	double  dCritLineVal;
	int     nPSCCirteriaType;
	int     nFillType;
	double  dPSCDgrmScFc;
	int     nDgrmDirType;
	int     nDgrmDirOptn;
	BOOL    bPSCSignPos;
	BOOL    bPSCSignNeg;
	UINT    nResistanceLineCol;
	UINT    nResistanceLineThk;
	int     nPSCCompCH;
	int     nPSCCompJP;
	int     nPSCCode;
	int     nDgnCode;
	int     nMeshedDgn_ElemAvg;
	int     nMeshedDgn_ElemCell;
	double  dMeshedDgn_CellLen;
	int     nMeshedDgn_XDirYDir;
	int     nMeshedDgn_RebarRatio;
	double  dMeshedDgnOpt[10]; // Meshed Design일 경우 AgenD.adIn 옵션 저장
	int     nMeshedDgnOpt[10]; // Meshed Design일 경우 AgenD.aiIn 옵션 저장

	int    nWoodArmerTopBot;  // Wood Armer Moment 정보
	double dWoodArmerAngle;   // Wood Armer Moment 정보

	void Get(T_FOUT_D_940& rData)
	{
		rData.nOutputLocation = nOutputLocation;
		rData.dScFactor = dScFactor;
		rData.dAdjustFactor = dAdjustFactor;
		rData.nAverage = nAverage;
		rData.nLocalGlobal = nLocalGlobal;
		rData.dMxxCoeff = dMxxCoeff;
		rData.dMyyCoeff = dMyyCoeff;
		rData.nMxxMyyMode = nMxxMyyMode;
		rData.dMxyCoeff = dMxyCoeff;
		rData.NucsK = NucsK;
		rData.nPStrsSurface = nPStrsSurface;
		rData.nMbDgrmNPlanK = nMbDgrmNPlanK;
		rData.nMbDgrmStorK = nMbDgrmStorK;
		rData.nMbDgrmDeciPt = nMbDgrmDeciPt;
		rData.nMbDgrmDgrmType = nMbDgrmDgrmType;
		rData.dMbDgrmScFt = dMbDgrmScFt;
		rData.nMbdgrmDgrmComp = nMbdgrmDgrmComp;
		rData.nMbDgrmOutType = nMbDgrmOutType;
		rData.nMbDgrmSelectMode = nMbDgrmSelectMode;
		rData.colValAxial = colValAxial;
		rData.colValShear = colValShear;
		rData.colValMoment = colValMoment;
		rData.nSSFROutType = nSSFROutType;
		rData.dSSFRAngle = dSSFRAngle;
		rData.nThisComp = nThisComp;
		rData.nThisFunction = nThisFunction;
		rData.nFunctionKey = nFunctionKey;
		rData.nHingeTimeIndex = nHingeTimeIndex;
		rData.dHingeTimeStep = dHingeTimeStep;
		rData.nThisTimeIndex = nThisTimeIndex;
		rData.dThisTimeStep = dThisTimeStep;
		rData.nHingeValType = nHingeValType;
		rData.nHingeDutl = nHingeDutl;
		rData.aPfudClwpK.SetSize(nCount_aPfudClwpK);
		rData.nPfudComp = nPfudComp;
		rData.nPfudDir = nPfudDir;
		rData.dEDgrmDefmScFc = dEDgrmDefmScFc;
		rData.nEDgrmScFcType = nEDgrmScFcType;
		rData.dEDgrmAutoScFc = dEDgrmAutoScFc;
		rData.dEDgrmRefValue = dEDgrmRefValue;
		rData.dEDgrmRefValueScFc = dEDgrmRefValueScFc;
		rData.aSelectedDiagK.SetSize(nCount_aSelectedDiagK);
		memcpy(rData.bFoutD, bFoutD, sizeof(rData.bFoutD));
		rData.nInerForceOrAcc = nInerForceOrAcc;
		rData.nUsedSerialModeIdx = nUsedSerialModeIdx;
		rData.bMode = bMode;
		rData.bOnCuttingLine = bOnCuttingLine;
		rData.bOnCuttingPlane = bOnCuttingPlane;
		rData.aCutPlanes.SetSize(nCount_aCutPlanes);
		rData.nSolidFreeEdgeType = nSolidFreeEdgeType;
		rData.bIsoSurface = bIsoSurface;
		rData.aIsoValues.SetSize(nCount_aIsoValues);
		rData.nTransparentIntensity = nTransparentIntensity;
		rData.nIsoValueMode = nIsoValueMode;
		rData.bDrawPolyOutLine = bDrawPolyOutLine;
		rData.bTransparent = bTransparent;
		rData.dCutLineScaleFactor = dCutLineScaleFactor;
		rData.bCutLineOutputMinMax = bCutLineOutputMinMax;
		rData.aCutLineKeys.SetSize(nCount_aCutLineKeys);
		rData.nCutPlaneHorizVert = nCutPlaneHorizVert;
		rData.colCutPlaneColor = colCutPlaneColor;
		rData.nCuttingLineDir = nCuttingLineDir;
		rData.colCuttinLineColor = colCuttinLineColor;
		rData.bDrawPolyLine = bDrawPolyLine;
		rData.bCutLineMinMaxOutOnly = bCutLineMinMaxOutOnly;
		rData.bCutLineReverse = bCutLineReverse;
		rData.bShowCutLineGraph = bShowCutLineGraph;
		rData.nCutLineOrPlane = nCutLineOrPlane;
		rData.nInflKeyNode = nInflKeyNode;
		rData.bInflIclImpFct = bInflIclImpFct;
		rData.bLocalValue = bLocalValue;
		rData.dCritLineVal = dCritLineVal;
		rData.nPSCCirteriaType = nPSCCirteriaType;
		rData.nFillType = nFillType;
		rData.dPSCDgrmScFc = dPSCDgrmScFc;
		rData.nDgrmDirType = nDgrmDirType;
		rData.nDgrmDirOptn = nDgrmDirOptn;
		rData.bPSCSignPos = bPSCSignPos;
		rData.bPSCSignNeg = bPSCSignNeg;
		rData.nResistanceLineCol = nResistanceLineCol;
		rData.nResistanceLineThk = nResistanceLineThk;
		rData.nPSCCompCH = nPSCCompCH;
		rData.nPSCCompJP = nPSCCompJP;
		rData.nPSCCode = nPSCCode;
		rData.nDgnCode = nDgnCode;
		rData.nMeshedDgn_ElemAvg = nMeshedDgn_ElemAvg;
		rData.nMeshedDgn_ElemCell = nMeshedDgn_ElemCell;
		rData.dMeshedDgn_CellLen = dMeshedDgn_CellLen;
		rData.nMeshedDgn_XDirYDir = nMeshedDgn_XDirYDir;
		rData.nMeshedDgn_RebarRatio = nMeshedDgn_RebarRatio;
		memcpy(rData.dMeshedDgnOpt, dMeshedDgnOpt, sizeof(rData.dMeshedDgnOpt));
		memcpy(rData.nMeshedDgnOpt, nMeshedDgnOpt, sizeof(rData.nMeshedDgnOpt));

		rData.nWoodArmerTopBot = nWoodArmerTopBot;
		rData.dWoodArmerAngle = dWoodArmerAngle;
	}
	void Set(T_FOUT_D_940 Data)
	{
		nOutputLocation = Data.nOutputLocation;
		dScFactor = Data.dScFactor;
		dAdjustFactor = Data.dAdjustFactor;
		nAverage = Data.nAverage;
		nLocalGlobal = Data.nLocalGlobal;
		dMxxCoeff = Data.dMxxCoeff;
		dMyyCoeff = Data.dMyyCoeff;
		nMxxMyyMode = Data.nMxxMyyMode;
		dMxyCoeff = Data.dMxyCoeff;
		NucsK = Data.NucsK;
		nPStrsSurface = Data.nPStrsSurface;
		nMbDgrmNPlanK = Data.nMbDgrmNPlanK;
		nMbDgrmStorK = Data.nMbDgrmStorK;
		nMbDgrmDeciPt = Data.nMbDgrmDeciPt;
		nMbDgrmDgrmType = Data.nMbDgrmDgrmType;
		dMbDgrmScFt = Data.dMbDgrmScFt;
		nMbdgrmDgrmComp = Data.nMbdgrmDgrmComp;
		nMbDgrmOutType = Data.nMbDgrmOutType;
		nMbDgrmSelectMode = Data.nMbDgrmSelectMode;
		colValAxial = Data.colValAxial;
		colValShear = Data.colValShear;
		colValMoment = Data.colValMoment;
		nSSFROutType = Data.nSSFROutType;
		dSSFRAngle = Data.dSSFRAngle;
		nThisComp = Data.nThisComp;
		nThisFunction = Data.nThisFunction;
		nFunctionKey = Data.nFunctionKey;
		nHingeTimeIndex = Data.nHingeTimeIndex;
		dHingeTimeStep = Data.dHingeTimeStep;
		nThisTimeIndex = Data.nThisTimeIndex;
		dThisTimeStep = Data.dThisTimeStep;
		nHingeValType = Data.nHingeValType;
		nHingeDutl = Data.nHingeDutl;
		nCount_aPfudClwpK = (int)Data.aPfudClwpK.GetSize();
		nPfudComp = Data.nPfudComp;
		nPfudDir = Data.nPfudDir;
		dEDgrmDefmScFc = Data.dEDgrmDefmScFc;
		nEDgrmScFcType = Data.nEDgrmScFcType;
		dEDgrmAutoScFc = Data.dEDgrmAutoScFc;
		dEDgrmRefValue = Data.dEDgrmRefValue;
		dEDgrmRefValueScFc = Data.dEDgrmRefValueScFc;
		nCount_aSelectedDiagK = (int)Data.aSelectedDiagK.GetSize();
		memcpy(bFoutD, Data.bFoutD, sizeof(bFoutD));
		nInerForceOrAcc = Data.nInerForceOrAcc;
		nUsedSerialModeIdx = Data.nUsedSerialModeIdx;
		bMode = Data.bMode;
		bOnCuttingLine = Data.bOnCuttingLine;
		bOnCuttingPlane = Data.bOnCuttingPlane;
		nCount_aCutPlanes = (int)Data.aCutPlanes.GetSize();
		nSolidFreeEdgeType = Data.nSolidFreeEdgeType;
		bIsoSurface = Data.bIsoSurface;
		nCount_aIsoValues = (int)Data.aIsoValues.GetSize();
		nTransparentIntensity = Data.nTransparentIntensity;
		nIsoValueMode = Data.nIsoValueMode;
		bDrawPolyOutLine = Data.bDrawPolyOutLine;
		bTransparent = Data.bTransparent;
		dCutLineScaleFactor = Data.dCutLineScaleFactor;
		bCutLineOutputMinMax = Data.bCutLineOutputMinMax;
		nCount_aCutLineKeys = (int)Data.aCutLineKeys.GetSize();
		nCutPlaneHorizVert = Data.nCutPlaneHorizVert;
		colCutPlaneColor = Data.colCutPlaneColor;
		nCuttingLineDir = Data.nCuttingLineDir;
		colCuttinLineColor = Data.colCuttinLineColor;
		bDrawPolyLine = Data.bDrawPolyLine;
		bCutLineMinMaxOutOnly = Data.bCutLineMinMaxOutOnly;
		bCutLineReverse = Data.bCutLineReverse;
		bShowCutLineGraph = Data.bShowCutLineGraph;
		nCutLineOrPlane = Data.nCutLineOrPlane;
		nInflKeyNode = Data.nInflKeyNode;
		bInflIclImpFct = Data.bInflIclImpFct;
		bLocalValue = Data.bLocalValue;
		dCritLineVal = Data.dCritLineVal;
		nPSCCirteriaType = Data.nPSCCirteriaType;
		nFillType = Data.nFillType;
		dPSCDgrmScFc = Data.dPSCDgrmScFc;
		nDgrmDirType = Data.nDgrmDirType;
		nDgrmDirOptn = Data.nDgrmDirOptn;
		bPSCSignPos = Data.bPSCSignPos;
		bPSCSignNeg = Data.bPSCSignNeg;
		nResistanceLineCol = Data.nResistanceLineCol;
		nResistanceLineThk = Data.nResistanceLineThk;
		nPSCCompCH = Data.nPSCCompCH;
		nPSCCompJP = Data.nPSCCompJP;
		nPSCCode = Data.nPSCCode;
		nDgnCode = Data.nDgnCode;
		nMeshedDgn_ElemAvg = Data.nMeshedDgn_ElemAvg;
		nMeshedDgn_ElemCell = Data.nMeshedDgn_ElemCell;
		dMeshedDgn_CellLen = Data.dMeshedDgn_CellLen;
		nMeshedDgn_XDirYDir = Data.nMeshedDgn_XDirYDir;
		nMeshedDgn_RebarRatio = Data.nMeshedDgn_RebarRatio;
		memcpy(dMeshedDgnOpt, Data.dMeshedDgnOpt, sizeof(dMeshedDgnOpt));
		memcpy(nMeshedDgnOpt, Data.nMeshedDgnOpt, sizeof(nMeshedDgnOpt));

		nWoodArmerTopBot = Data.nWoodArmerTopBot;
		dWoodArmerAngle = Data.dWoodArmerAngle;
	}
};

struct T_FOUT_D_RW
{
	int     nOutputLocation     ;
	double  dScFactor           ;
	double  dAdjustFactor       ;
	int     nAverage            ;
	int     nLocalGlobal        ;
	double  dMxxCoeff           ;
	double  dMyyCoeff           ;
	int     nMxxMyyMode         ;
	double  dMxyCoeff           ;
	UINT    NucsK               ;
	int     nPStrsSurface       ;
	UINT    nMbDgrmNPlanK       ;
	UINT    nMbDgrmStorK        ;
	int     nMbDgrmDeciPt       ;
	int     nMbDgrmDgrmType     ;
	double  dMbDgrmScFt         ;
	int     nMbdgrmDgrmComp     ;
	int     nMbDgrmOutType      ;
	UINT    nMbDgrmSelectMode   ;
	UINT    colValAxial         ;
	UINT    colValShear         ;
	UINT    colValMoment        ;
	int     nSSFROutType        ;
	double  dSSFRAngle          ;
	int     nThisComp           ;
	int     nThisFunction       ;
	UINT    nFunctionKey        ;
	int     nHingeTimeIndex     ;
	double  dHingeTimeStep      ;
	int     nThisTimeIndex      ;
	double  dThisTimeStep       ;
	int     nHingeValType       ;
	int     nHingeDutl          ;
	int     nCount_aPfudClwpK   ;
	int     nPfudComp           ;
	int     nPfudDir            ;
	double  dEDgrmDefmScFc      ;
	UINT    nEDgrmScFcType      ;
	double  dEDgrmAutoScFc      ;
	double  dEDgrmRefValue      ;
	double  dEDgrmRefValueScFc  ;
	int     nCount_aSelectedDiagK;
	BOOL    bFoutD[EN_FOUT_D_END];
	int     nInerForceOrAcc     ;
	int     nUsedSerialModeIdx  ;
	BOOL    bMode               ;
	BOOL    bOnCuttingLine      ;
	BOOL    bOnCuttingPlane     ;
	int     nCount_aCutPlanes   ;
	int		  nSolidFreeEdgeType  ;
	BOOL    bIsoSurface         ;
	int     nCount_aIsoValues   ;
	int	    nTransparentIntensity;
	int	    nIsoValueMode       ;
	BOOL	  bDrawPolyOutLine    ;
	BOOL	  bTransparent        ;
	double  dCutLineScaleFactor ;
	BOOL    bCutLineOutputMinMax;
	int     nCount_aCutLineKeys ;
	int     nCutPlaneHorizVert  ;
	COLORREF colCutPlaneColor   ;
	int     nCuttingLineDir     ;
	COLORREF colCuttinLineColor ;
	BOOL    bDrawPolyLine       ;
	BOOL    bCutLineMinMaxOutOnly;
	BOOL    bCutLineReverse     ;
	BOOL    bShowCutLineGraph   ;
	int     nCutLineOrPlane     ;
	UINT    nInflKeyNode        ;
	BOOL    bInflIclImpFct      ;
	BOOL    bLocalValue         ;
	double  dCritLineVal        ;
	int     nPSCCirteriaType    ;
	int     nFillType           ;
	double  dPSCDgrmScFc        ;
	int     nDgrmDirType        ;
	int     nDgrmDirOptn        ;
	BOOL    bPSCSignPos         ;
	BOOL    bPSCSignNeg         ;
	UINT    nResistanceLineCol  ;
	UINT    nResistanceLineThk  ;
	int     nPSCCompCH          ;
	int     nPSCCompJP          ;
	int     nPSCCode            ;
	int     nDgnCode            ;
	int     nMeshedDgn_ElemAvg  ;
	int     nMeshedDgn_ElemCell ;
	double  dMeshedDgn_CellLen  ;
	int     nMeshedDgn_XDirYDir  ;
	int     nMeshedDgn_RebarRatio;
	double  dMeshedDgnOpt[10]   ; // Meshed Design일 경우 AgenD.adIn 옵션 저장
	int     nMeshedDgnOpt[10]   ; // Meshed Design일 경우 AgenD.aiIn 옵션 저장

	int    nWoodArmerTopBot;  // Wood Armer Moment 정보
	double dWoodArmerAngle;   // Wood Armer Moment 정보
	
	BOOL	bRFrcdTopAlignment;
	int		nRfrcdOutputPos;

	BOOL	bPsiFactor; // Moving Tracer Reaction Psi Factor

	// Mode Shape
	BOOL	bCubicInterpolation;
	double	dCubicFactor;

	void Get(T_FOUT_D &rData)
	{
		rData.nOutputLocation        = nOutputLocation      ;
		rData.dScFactor              = dScFactor            ;
		rData.dAdjustFactor          = dAdjustFactor        ;
		rData.nAverage               = nAverage             ;
		rData.nLocalGlobal           = nLocalGlobal         ;
		rData.dMxxCoeff              = dMxxCoeff            ;
		rData.dMyyCoeff              = dMyyCoeff            ;
		rData.nMxxMyyMode            = nMxxMyyMode          ;
		rData.dMxyCoeff              = dMxyCoeff            ;
		rData.NucsK                  = NucsK                ;
		rData.nPStrsSurface          = nPStrsSurface        ;
		rData.nMbDgrmNPlanK          = nMbDgrmNPlanK        ;
		rData.nMbDgrmStorK           = nMbDgrmStorK         ;
		rData.nMbDgrmDeciPt          = nMbDgrmDeciPt        ;
		rData.nMbDgrmDgrmType        = nMbDgrmDgrmType      ;
		rData.dMbDgrmScFt            = dMbDgrmScFt          ;
		rData.nMbdgrmDgrmComp        = nMbdgrmDgrmComp      ;
		rData.nMbDgrmOutType         = nMbDgrmOutType       ;
		rData.nMbDgrmSelectMode      = nMbDgrmSelectMode    ;
		rData.colValAxial            = colValAxial          ;
		rData.colValShear            = colValShear          ;
		rData.colValMoment           = colValMoment         ;
		rData.nSSFROutType           = nSSFROutType         ;
		rData.dSSFRAngle             = dSSFRAngle           ;    
		rData.nThisComp              = nThisComp            ;
		rData.nThisFunction          = nThisFunction        ;
		rData.nFunctionKey           = nFunctionKey         ;
		rData.nHingeTimeIndex        = nHingeTimeIndex      ;
		rData.dHingeTimeStep         = dHingeTimeStep       ;
		rData.nThisTimeIndex         = nThisTimeIndex       ;
		rData.dThisTimeStep          = dThisTimeStep        ;
		rData.nHingeValType          = nHingeValType        ;
		rData.nHingeDutl             = nHingeDutl           ;
		rData.aPfudClwpK.SetSize(nCount_aPfudClwpK);
		rData.nPfudComp              = nPfudComp            ;
		rData.nPfudDir               = nPfudDir             ;
		rData.dEDgrmDefmScFc         = dEDgrmDefmScFc       ;
		rData.nEDgrmScFcType         = nEDgrmScFcType       ;
		rData.dEDgrmAutoScFc         = dEDgrmAutoScFc       ;
		rData.dEDgrmRefValue         = dEDgrmRefValue       ;
		rData.dEDgrmRefValueScFc     = dEDgrmRefValueScFc   ;
		rData.aSelectedDiagK.SetSize(nCount_aSelectedDiagK);
		memcpy(rData.bFoutD, bFoutD, sizeof(rData.bFoutD));
		rData.nInerForceOrAcc        = nInerForceOrAcc      ;
		rData.nUsedSerialModeIdx     = nUsedSerialModeIdx   ;
		rData.bMode                  = bMode                ;
		rData.bOnCuttingLine        = bOnCuttingLine        ;
		rData.bOnCuttingPlane       = bOnCuttingPlane       ;
		rData.aCutPlanes.SetSize(nCount_aCutPlanes);
		rData.nSolidFreeEdgeType    = nSolidFreeEdgeType    ;
		rData.bIsoSurface           = bIsoSurface           ;
		rData.aIsoValues.SetSize(nCount_aIsoValues);
		rData.nTransparentIntensity = nTransparentIntensity ;
		rData.nIsoValueMode         = nIsoValueMode         ;
		rData.bDrawPolyOutLine      = bDrawPolyOutLine      ;
		rData.bTransparent          = bTransparent          ;
		rData.dCutLineScaleFactor   = dCutLineScaleFactor   ;
		rData.bCutLineOutputMinMax  = bCutLineOutputMinMax  ;
		rData.aCutLineKeys.SetSize(nCount_aCutLineKeys);
		rData.nCutPlaneHorizVert    = nCutPlaneHorizVert    ;
		rData.colCutPlaneColor      = colCutPlaneColor      ;
		rData.nCuttingLineDir       = nCuttingLineDir       ;
		rData.colCuttinLineColor    = colCuttinLineColor    ;
		rData.bDrawPolyLine         = bDrawPolyLine         ;
		rData.bCutLineMinMaxOutOnly = bCutLineMinMaxOutOnly ;
		rData.bCutLineReverse       = bCutLineReverse       ;
		rData.bShowCutLineGraph     = bShowCutLineGraph     ;
		rData.nCutLineOrPlane       = nCutLineOrPlane       ;
		rData.nInflKeyNode          = nInflKeyNode          ;
		rData.bInflIclImpFct        = bInflIclImpFct        ;
		rData.bLocalValue           = bLocalValue           ;
		rData.dCritLineVal          = dCritLineVal          ;
		rData.nPSCCirteriaType      = nPSCCirteriaType      ;
		rData.nFillType             = nFillType             ;
		rData.dPSCDgrmScFc          = dPSCDgrmScFc          ;
		rData.nDgrmDirType          = nDgrmDirType          ;
		rData.nDgrmDirOptn          = nDgrmDirOptn          ;
		rData.bPSCSignPos           = bPSCSignPos           ;
		rData.bPSCSignNeg           = bPSCSignNeg           ;
		rData.nResistanceLineCol    = nResistanceLineCol    ;
		rData.nResistanceLineThk    = nResistanceLineThk    ;
		rData.nPSCCompCH            = nPSCCompCH            ;
		rData.nPSCCompJP            = nPSCCompJP            ;
		rData.nPSCCode              = nPSCCode              ;
		rData.nDgnCode              = nDgnCode              ;
		rData.nMeshedDgn_ElemAvg    = nMeshedDgn_ElemAvg    ;
		rData.nMeshedDgn_ElemCell   = nMeshedDgn_ElemCell   ;
		rData.dMeshedDgn_CellLen    = dMeshedDgn_CellLen    ;
		rData.nMeshedDgn_XDirYDir   = nMeshedDgn_XDirYDir   ;
		rData.nMeshedDgn_RebarRatio = nMeshedDgn_RebarRatio ;
		memcpy(rData.dMeshedDgnOpt, dMeshedDgnOpt, sizeof(rData.dMeshedDgnOpt));
		memcpy(rData.nMeshedDgnOpt, nMeshedDgnOpt, sizeof(rData.nMeshedDgnOpt));

		rData.nWoodArmerTopBot   = nWoodArmerTopBot;
		rData.dWoodArmerAngle    = dWoodArmerAngle; 

		rData.bRFrcdTopAlignment = bRFrcdTopAlignment;
		rData.nRfrcdOutputPos = nRfrcdOutputPos;
		rData.bPsiFactor = bPsiFactor;

		rData.bCubicInterpolation = bCubicInterpolation;
		rData.dCubicFactor = dCubicFactor; 
	}
	void Set(T_FOUT_D Data)
	{
		nOutputLocation       = Data.nOutputLocation       ;
		dScFactor             = Data.dScFactor             ;
		dAdjustFactor         = Data.dAdjustFactor         ;
		nAverage              = Data.nAverage              ;
		nLocalGlobal          = Data.nLocalGlobal          ;
		dMxxCoeff             = Data.dMxxCoeff             ;
		dMyyCoeff             = Data.dMyyCoeff             ;
		nMxxMyyMode           = Data.nMxxMyyMode           ;
		dMxyCoeff             = Data.dMxyCoeff             ;
		NucsK                 = Data.NucsK                 ;
		nPStrsSurface         = Data.nPStrsSurface         ;
		nMbDgrmNPlanK         = Data.nMbDgrmNPlanK         ;
		nMbDgrmStorK          = Data.nMbDgrmStorK          ;
		nMbDgrmDeciPt         = Data.nMbDgrmDeciPt         ;
		nMbDgrmDgrmType       = Data.nMbDgrmDgrmType       ;
		dMbDgrmScFt           = Data.dMbDgrmScFt           ;
		nMbdgrmDgrmComp       = Data.nMbdgrmDgrmComp       ;
		nMbDgrmOutType        = Data.nMbDgrmOutType        ;
		nMbDgrmSelectMode     = Data.nMbDgrmSelectMode     ;
		colValAxial           = Data.colValAxial           ;
		colValShear           = Data.colValShear           ;
		colValMoment          = Data.colValMoment          ;
		nSSFROutType          = Data.nSSFROutType          ;
		dSSFRAngle            = Data.dSSFRAngle            ;    
		nThisComp             = Data.nThisComp             ;
		nThisFunction         = Data.nThisFunction         ;
		nFunctionKey          = Data.nFunctionKey          ;
		nHingeTimeIndex       = Data.nHingeTimeIndex       ;
		dHingeTimeStep        = Data.dHingeTimeStep        ;
		nThisTimeIndex        = Data.nThisTimeIndex        ;
		dThisTimeStep         = Data.dThisTimeStep         ;
		nHingeValType         = Data.nHingeValType         ;
		nHingeDutl            = Data.nHingeDutl            ;
		nCount_aPfudClwpK = (int)Data.aPfudClwpK.GetSize();
		nPfudComp             = Data.nPfudComp             ;
		nPfudDir              = Data.nPfudDir              ;
		dEDgrmDefmScFc        = Data.dEDgrmDefmScFc        ;
		nEDgrmScFcType        = Data.nEDgrmScFcType        ;
		dEDgrmAutoScFc        = Data.dEDgrmAutoScFc        ;
		dEDgrmRefValue        = Data.dEDgrmRefValue        ;
		dEDgrmRefValueScFc    = Data.dEDgrmRefValueScFc    ;
		nCount_aSelectedDiagK = (int)Data.aSelectedDiagK.GetSize();
		memcpy(bFoutD, Data.bFoutD, sizeof(bFoutD));
		nInerForceOrAcc       = Data.nInerForceOrAcc       ;
		nUsedSerialModeIdx    = Data.nUsedSerialModeIdx    ;
		bMode                 = Data.bMode                 ;
		bOnCuttingLine        = Data.bOnCuttingLine        ;
		bOnCuttingPlane       = Data.bOnCuttingPlane       ;
		nCount_aCutPlanes     = (int)Data.aCutPlanes.GetSize()  ;
		nSolidFreeEdgeType    = Data.nSolidFreeEdgeType    ;
		bIsoSurface           = Data.bIsoSurface           ;
		nCount_aIsoValues     = (int)Data.aIsoValues.GetSize()  ;
		nTransparentIntensity = Data.nTransparentIntensity ;
		nIsoValueMode         = Data.nIsoValueMode         ;
		bDrawPolyOutLine      = Data.bDrawPolyOutLine      ;
		bTransparent          = Data.bTransparent          ;
		dCutLineScaleFactor   = Data.dCutLineScaleFactor   ;
		bCutLineOutputMinMax  = Data.bCutLineOutputMinMax  ;
		nCount_aCutLineKeys   = (int)Data.aCutLineKeys.GetSize();
		nCutPlaneHorizVert    = Data.nCutPlaneHorizVert    ;
		colCutPlaneColor      = Data.colCutPlaneColor      ;
		nCuttingLineDir       = Data.nCuttingLineDir       ;
		colCuttinLineColor    = Data.colCuttinLineColor    ;
		bDrawPolyLine         = Data.bDrawPolyLine         ;
		bCutLineMinMaxOutOnly = Data.bCutLineMinMaxOutOnly ;
		bCutLineReverse       = Data.bCutLineReverse       ;
		bShowCutLineGraph     = Data.bShowCutLineGraph     ;
		nCutLineOrPlane       = Data.nCutLineOrPlane       ;
		nInflKeyNode          = Data.nInflKeyNode          ;
		bInflIclImpFct        = Data.bInflIclImpFct        ;
		bLocalValue           = Data.bLocalValue           ;
		dCritLineVal          = Data.dCritLineVal          ;
		nPSCCirteriaType      = Data.nPSCCirteriaType      ;
		nFillType             = Data.nFillType             ;
		dPSCDgrmScFc          = Data.dPSCDgrmScFc          ;
		nDgrmDirType          = Data.nDgrmDirType          ;
		nDgrmDirOptn          = Data.nDgrmDirOptn          ;
		bPSCSignPos           = Data.bPSCSignPos           ;
		bPSCSignNeg           = Data.bPSCSignNeg           ;
		nResistanceLineCol    = Data.nResistanceLineCol    ;
		nResistanceLineThk    = Data.nResistanceLineThk    ;
		nPSCCompCH            = Data.nPSCCompCH            ;
		nPSCCompJP            = Data.nPSCCompJP            ;
		nPSCCode              = Data.nPSCCode              ;
		nDgnCode              = Data.nDgnCode              ;
		nMeshedDgn_ElemAvg    = Data.nMeshedDgn_ElemAvg    ;
		nMeshedDgn_ElemCell   = Data.nMeshedDgn_ElemCell   ;
		dMeshedDgn_CellLen    = Data.dMeshedDgn_CellLen    ;
		nMeshedDgn_XDirYDir   = Data.nMeshedDgn_XDirYDir   ;
		nMeshedDgn_RebarRatio = Data.nMeshedDgn_RebarRatio ;
		memcpy(dMeshedDgnOpt, Data.dMeshedDgnOpt, sizeof(dMeshedDgnOpt));
		memcpy(nMeshedDgnOpt, Data.nMeshedDgnOpt, sizeof(nMeshedDgnOpt));

		nWoodArmerTopBot   = Data.nWoodArmerTopBot;
		dWoodArmerAngle    = Data.dWoodArmerAngle; 

		bRFrcdTopAlignment = Data.bRFrcdTopAlignment;
		nRfrcdOutputPos = Data.nRfrcdOutputPos;

		bPsiFactor = Data.bPsiFactor;

		bCubicInterpolation = Data.bCubicInterpolation;
		dCubicFactor = Data.dCubicFactor;
	}
	void Convert821(T_FOUT_D_RW_821& src)
	{
		nOutputLocation       = src.nOutputLocation       ;
		dScFactor             = src.dScFactor             ;
		dAdjustFactor         = src.dAdjustFactor         ;
		nAverage              = src.nAverage              ;
		nLocalGlobal          = src.nLocalGlobal          ;
		dMxxCoeff             = src.dMxxCoeff             ;
		dMyyCoeff             = src.dMyyCoeff             ;
		nMxxMyyMode           = src.nMxxMyyMode           ;
		dMxyCoeff             = src.dMxyCoeff             ;
		NucsK                 = src.NucsK                 ;
		nPStrsSurface         = src.nPStrsSurface         ;
		nMbDgrmNPlanK         = src.nMbDgrmNPlanK         ;
		nMbDgrmStorK          = src.nMbDgrmStorK          ;
		nMbDgrmDeciPt         = src.nMbDgrmDeciPt         ;
		nMbDgrmDgrmType       = src.nMbDgrmDgrmType       ;
		dMbDgrmScFt           = src.dMbDgrmScFt           ;
		nMbdgrmDgrmComp       = src.nMbdgrmDgrmComp       ;
		nMbDgrmOutType        = src.nMbDgrmOutType        ;
		nMbDgrmSelectMode     = src.nMbDgrmSelectMode     ;
		colValAxial           = src.colValAxial           ;
		colValShear           = src.colValShear           ;
		colValMoment          = src.colValMoment          ;
		nSSFROutType          = src.nSSFROutType          ;
		dSSFRAngle            = src.dSSFRAngle            ;    
		nThisComp             = src.nThisComp             ;
		nThisFunction         = src.nThisFunction         ;
		nFunctionKey          = src.nFunctionKey          ;
		nHingeTimeIndex       = src.nHingeTimeIndex       ;
		dHingeTimeStep        = src.dHingeTimeStep        ;
		nThisTimeIndex        = src.nThisTimeIndex        ;
		dThisTimeStep         = src.dThisTimeStep         ;
		nHingeValType         = src.nHingeValType         ;
		nHingeDutl            = src.nHingeDutl            ;
		nCount_aPfudClwpK     = src.nCount_aPfudClwpK;
		nPfudComp             = src.nPfudComp             ;
		nPfudDir              = src.nPfudDir              ;
		dEDgrmDefmScFc        = src.dEDgrmDefmScFc        ;
		nEDgrmScFcType        = src.nEDgrmScFcType        ;
		dEDgrmAutoScFc        = src.dEDgrmAutoScFc        ;
		dEDgrmRefValue        = src.dEDgrmRefValue        ;
		dEDgrmRefValueScFc    = src.dEDgrmRefValueScFc    ;
		nCount_aSelectedDiagK = src.nCount_aSelectedDiagK ;
		memcpy(bFoutD, src.bFoutD, sizeof(bFoutD));
		nInerForceOrAcc       = src.nInerForceOrAcc       ;
		nUsedSerialModeIdx    = src.nUsedSerialModeIdx    ;
		bMode                 = src.bMode                 ;
		bOnCuttingLine        = src.bOnCuttingLine        ;
		bOnCuttingPlane       = src.bOnCuttingPlane       ;
		nCount_aCutPlanes     = src.nCount_aCutPlanes     ;
		nSolidFreeEdgeType    = src.nSolidFreeEdgeType    ;
		bIsoSurface           = src.bIsoSurface           ;
		nCount_aIsoValues     = src.nCount_aIsoValues     ;
		nTransparentIntensity = src.nTransparentIntensity ;
		nIsoValueMode         = src.nIsoValueMode         ;
		bDrawPolyOutLine      = src.bDrawPolyOutLine      ;
		bTransparent          = src.bTransparent          ;
		dCutLineScaleFactor   = src.dCutLineScaleFactor   ;
		bCutLineOutputMinMax  = src.bCutLineOutputMinMax  ;
		nCount_aCutLineKeys   = src.nCount_aCutLineKeys   ;
		nCutPlaneHorizVert    = src.nCutPlaneHorizVert    ;
		colCutPlaneColor      = src.colCutPlaneColor      ;
		nCuttingLineDir       = src.nCuttingLineDir       ;
		colCuttinLineColor    = src.colCuttinLineColor    ;
		bDrawPolyLine         = src.bDrawPolyLine         ;
		bCutLineMinMaxOutOnly = src.bCutLineMinMaxOutOnly ;
		bCutLineReverse       = src.bCutLineReverse       ;
		bShowCutLineGraph     = src.bShowCutLineGraph     ;
		nCutLineOrPlane       = src.nCutLineOrPlane       ;
		nInflKeyNode          = src.nInflKeyNode          ;
		bInflIclImpFct        = src.bInflIclImpFct        ;
		bLocalValue           = src.bLocalValue           ;
		dCritLineVal          = src.dCritLineVal          ;
		nPSCCirteriaType      = src.nPSCCirteriaType      ;
		nFillType             = src.nFillType             ;
		dPSCDgrmScFc          = src.dPSCDgrmScFc          ;
		nDgrmDirType          = src.nDgrmDirType          ;
		nDgrmDirOptn          = src.nDgrmDirOptn          ;
		bPSCSignPos           = src.bPSCSignPos           ;
		bPSCSignNeg           = src.bPSCSignNeg           ;
		nResistanceLineCol    = src.nResistanceLineCol    ;
		nResistanceLineThk    = src.nResistanceLineThk    ;
		nPSCCompCH            = src.nPSCCompCH            ;
		nPSCCompJP            = src.nPSCCompJP            ;
		nPSCCode              = src.nPSCCode              ;
		nDgnCode              = src.nDgnCode              ;
		nMeshedDgn_ElemAvg    = 0                         ;
		nMeshedDgn_ElemCell   = 0                         ;
		dMeshedDgn_CellLen    = 1                         ;
		nMeshedDgn_XDirYDir   = 0                         ;
		nMeshedDgn_RebarRatio = 2                         ;
		for(int i=0; i<10; i++) { nMeshedDgnOpt[i]=0; dMeshedDgnOpt[i]=0.0; }
		nWoodArmerTopBot   = 0;
		dWoodArmerAngle    = 0.0; 
		bRFrcdTopAlignment = TRUE;
		nRfrcdOutputPos = 2;
		bPsiFactor = FALSE;
		bCubicInterpolation = FALSE;
		dCubicFactor = 0.5;
	}
	void Convert825(T_FOUT_D_RW_825& src)
	{
		nOutputLocation       = src.nOutputLocation       ;
		dScFactor             = src.dScFactor             ;
		dAdjustFactor         = src.dAdjustFactor         ;
		nAverage              = src.nAverage              ;
		nLocalGlobal          = src.nLocalGlobal          ;
		dMxxCoeff             = src.dMxxCoeff             ;
		dMyyCoeff             = src.dMyyCoeff             ;
		nMxxMyyMode           = src.nMxxMyyMode           ;
		dMxyCoeff             = src.dMxyCoeff             ;
		NucsK                 = src.NucsK                 ;
		nPStrsSurface         = src.nPStrsSurface         ;
		nMbDgrmNPlanK         = src.nMbDgrmNPlanK         ;
		nMbDgrmStorK          = src.nMbDgrmStorK          ;
		nMbDgrmDeciPt         = src.nMbDgrmDeciPt         ;
		nMbDgrmDgrmType       = src.nMbDgrmDgrmType       ;
		dMbDgrmScFt           = src.dMbDgrmScFt           ;
		nMbdgrmDgrmComp       = src.nMbdgrmDgrmComp       ;
		nMbDgrmOutType        = src.nMbDgrmOutType        ;
		nMbDgrmSelectMode     = src.nMbDgrmSelectMode     ;
		colValAxial           = src.colValAxial           ;
		colValShear           = src.colValShear           ;
		colValMoment          = src.colValMoment          ;
		nSSFROutType          = src.nSSFROutType          ;
		dSSFRAngle            = src.dSSFRAngle            ;    
		nThisComp             = src.nThisComp             ;
		nThisFunction         = src.nThisFunction         ;
		nFunctionKey          = src.nFunctionKey          ;
		nHingeTimeIndex       = src.nHingeTimeIndex       ;
		dHingeTimeStep        = src.dHingeTimeStep        ;
		nThisTimeIndex        = src.nThisTimeIndex        ;
		dThisTimeStep         = src.dThisTimeStep         ;
		nHingeValType         = src.nHingeValType         ;
		nHingeDutl            = src.nHingeDutl            ;
		nCount_aPfudClwpK     = src.nCount_aPfudClwpK;
		nPfudComp             = src.nPfudComp             ;
		nPfudDir              = src.nPfudDir              ;
		dEDgrmDefmScFc        = src.dEDgrmDefmScFc        ;
		nEDgrmScFcType        = src.nEDgrmScFcType        ;
		dEDgrmAutoScFc        = src.dEDgrmAutoScFc        ;
		dEDgrmRefValue        = src.dEDgrmRefValue        ;
		dEDgrmRefValueScFc    = src.dEDgrmRefValueScFc    ;
		nCount_aSelectedDiagK = src.nCount_aSelectedDiagK ;
		memcpy(bFoutD, src.bFoutD, sizeof(bFoutD));
		nInerForceOrAcc       = src.nInerForceOrAcc       ;
		nUsedSerialModeIdx    = src.nUsedSerialModeIdx    ;
		bMode                 = src.bMode                 ;
		bOnCuttingLine        = src.bOnCuttingLine        ;
		bOnCuttingPlane       = src.bOnCuttingPlane       ;
		nCount_aCutPlanes     = src.nCount_aCutPlanes     ;
		nSolidFreeEdgeType    = src.nSolidFreeEdgeType    ;
		bIsoSurface           = src.bIsoSurface           ;
		nCount_aIsoValues     = src.nCount_aIsoValues     ;
		nTransparentIntensity = src.nTransparentIntensity ;
		nIsoValueMode         = src.nIsoValueMode         ;
		bDrawPolyOutLine      = src.bDrawPolyOutLine      ;
		bTransparent          = src.bTransparent          ;
		dCutLineScaleFactor   = src.dCutLineScaleFactor   ;
		bCutLineOutputMinMax  = src.bCutLineOutputMinMax  ;
		nCount_aCutLineKeys   = src.nCount_aCutLineKeys   ;
		nCutPlaneHorizVert    = src.nCutPlaneHorizVert    ;
		colCutPlaneColor      = src.colCutPlaneColor      ;
		nCuttingLineDir       = src.nCuttingLineDir       ;
		colCuttinLineColor    = src.colCuttinLineColor    ;
		bDrawPolyLine         = src.bDrawPolyLine         ;
		bCutLineMinMaxOutOnly = src.bCutLineMinMaxOutOnly ;
		bCutLineReverse       = src.bCutLineReverse       ;
		bShowCutLineGraph     = src.bShowCutLineGraph     ;
		nCutLineOrPlane       = src.nCutLineOrPlane       ;
		nInflKeyNode          = src.nInflKeyNode          ;
		bInflIclImpFct        = src.bInflIclImpFct        ;
		bLocalValue           = src.bLocalValue           ;
		dCritLineVal          = src.dCritLineVal          ;
		nPSCCirteriaType      = src.nPSCCirteriaType      ;
		nFillType             = src.nFillType             ;
		dPSCDgrmScFc          = src.dPSCDgrmScFc          ;
		nDgrmDirType          = src.nDgrmDirType          ;
		nDgrmDirOptn          = src.nDgrmDirOptn          ;
		bPSCSignPos           = src.bPSCSignPos           ;
		bPSCSignNeg           = src.bPSCSignNeg           ;
		nResistanceLineCol    = src.nResistanceLineCol    ;
		nResistanceLineThk    = src.nResistanceLineThk    ;
		nPSCCompCH            = src.nPSCCompCH            ;
		nPSCCompJP            = src.nPSCCompJP            ;
		nPSCCode              = src.nPSCCode              ;
		nDgnCode              = src.nDgnCode              ;
		nMeshedDgn_ElemAvg    = src.nMeshedDgn_ElemAvg    ;
		nMeshedDgn_ElemCell   = src.nMeshedDgn_ElemCell   ;
		dMeshedDgn_CellLen    = src.dMeshedDgn_CellLen    ;
		nMeshedDgn_XDirYDir   = src.nMeshedDgn_XDirYDir   ;
		nMeshedDgn_RebarRatio = src.nMeshedDgn_RebarRatio ;
		for(int i=0; i<10; i++) { nMeshedDgnOpt[i]=0; dMeshedDgnOpt[i]=0.0; }
		nWoodArmerTopBot   = 0;
		dWoodArmerAngle    = 0.0; 
		bRFrcdTopAlignment = TRUE;
		nRfrcdOutputPos = 2;
		bPsiFactor = FALSE;
		bCubicInterpolation = FALSE;
		dCubicFactor = 0.5;
	}

	void Convert851(T_FOUT_D_RW_851& src)
	{
		nOutputLocation       = src.nOutputLocation       ;
		dScFactor             = src.dScFactor             ;
		dAdjustFactor         = src.dAdjustFactor         ;
		nAverage              = src.nAverage              ;
		nLocalGlobal          = src.nLocalGlobal          ;
		dMxxCoeff             = src.dMxxCoeff             ;
		dMyyCoeff             = src.dMyyCoeff             ;
		nMxxMyyMode           = src.nMxxMyyMode           ;
		dMxyCoeff             = src.dMxyCoeff             ;
		NucsK                 = src.NucsK                 ;
		nPStrsSurface         = src.nPStrsSurface         ;
		nMbDgrmNPlanK         = src.nMbDgrmNPlanK         ;
		nMbDgrmStorK          = src.nMbDgrmStorK          ;
		nMbDgrmDeciPt         = src.nMbDgrmDeciPt         ;
		nMbDgrmDgrmType       = src.nMbDgrmDgrmType       ;
		dMbDgrmScFt           = src.dMbDgrmScFt           ;
		nMbdgrmDgrmComp       = src.nMbdgrmDgrmComp       ;
		nMbDgrmOutType        = src.nMbDgrmOutType        ;
		nMbDgrmSelectMode     = src.nMbDgrmSelectMode     ;
		colValAxial           = src.colValAxial           ;
		colValShear           = src.colValShear           ;
		colValMoment          = src.colValMoment          ;
		nSSFROutType          = src.nSSFROutType          ;
		dSSFRAngle            = src.dSSFRAngle            ;    
		nThisComp             = src.nThisComp             ;
		nThisFunction         = src.nThisFunction         ;
		nFunctionKey          = src.nFunctionKey          ;
		nHingeTimeIndex       = src.nHingeTimeIndex       ;
		dHingeTimeStep        = src.dHingeTimeStep        ;
		nThisTimeIndex        = src.nThisTimeIndex        ;
		dThisTimeStep         = src.dThisTimeStep         ;
		nHingeValType         = src.nHingeValType         ;
		nHingeDutl            = src.nHingeDutl            ;
		nCount_aPfudClwpK     = src.nCount_aPfudClwpK;
		nPfudComp             = src.nPfudComp             ;
		nPfudDir              = src.nPfudDir              ;
		dEDgrmDefmScFc        = src.dEDgrmDefmScFc        ;
		nEDgrmScFcType        = src.nEDgrmScFcType        ;
		dEDgrmAutoScFc        = src.dEDgrmAutoScFc        ;
		dEDgrmRefValue        = src.dEDgrmRefValue        ;
		dEDgrmRefValueScFc    = src.dEDgrmRefValueScFc    ;
		nCount_aSelectedDiagK = src.nCount_aSelectedDiagK;
		memcpy(bFoutD, src.bFoutD, sizeof(bFoutD));
		nInerForceOrAcc       = src.nInerForceOrAcc       ;
		nUsedSerialModeIdx    = src.nUsedSerialModeIdx    ;
		bMode                 = src.bMode                 ;
		bOnCuttingLine        = src.bOnCuttingLine        ;
		bOnCuttingPlane       = src.bOnCuttingPlane       ;
		nCount_aCutPlanes     = src.nCount_aCutPlanes;
		nSolidFreeEdgeType    = src.nSolidFreeEdgeType    ;
		bIsoSurface           = src.bIsoSurface           ;
		nCount_aIsoValues     = src.nCount_aIsoValues;
		nTransparentIntensity = src.nTransparentIntensity ;
		nIsoValueMode         = src.nIsoValueMode         ;
		bDrawPolyOutLine      = src.bDrawPolyOutLine      ;
		bTransparent          = src.bTransparent          ;
		dCutLineScaleFactor   = src.dCutLineScaleFactor   ;
		bCutLineOutputMinMax  = src.bCutLineOutputMinMax  ;
		nCount_aCutLineKeys   = src.nCount_aCutLineKeys;
		nCutPlaneHorizVert    = src.nCutPlaneHorizVert    ;
		colCutPlaneColor      = src.colCutPlaneColor      ;
		nCuttingLineDir       = src.nCuttingLineDir       ;
		colCuttinLineColor    = src.colCuttinLineColor    ;
		bDrawPolyLine         = src.bDrawPolyLine         ;
		bCutLineMinMaxOutOnly = src.bCutLineMinMaxOutOnly ;
		bCutLineReverse       = src.bCutLineReverse       ;
		bShowCutLineGraph     = src.bShowCutLineGraph     ;
		nCutLineOrPlane       = src.nCutLineOrPlane       ;
		nInflKeyNode          = src.nInflKeyNode          ;
		bInflIclImpFct        = src.bInflIclImpFct        ;
		bLocalValue           = src.bLocalValue           ;
		dCritLineVal          = src.dCritLineVal          ;
		nPSCCirteriaType      = src.nPSCCirteriaType      ;
		nFillType             = src.nFillType             ;
		dPSCDgrmScFc          = src.dPSCDgrmScFc          ;
		nDgrmDirType          = src.nDgrmDirType          ;
		nDgrmDirOptn          = src.nDgrmDirOptn          ;
		bPSCSignPos           = src.bPSCSignPos           ;
		bPSCSignNeg           = src.bPSCSignNeg           ;
		nResistanceLineCol    = src.nResistanceLineCol    ;
		nResistanceLineThk    = src.nResistanceLineThk    ;
		nPSCCompCH            = src.nPSCCompCH            ;
		nPSCCompJP            = src.nPSCCompJP            ;
		nPSCCode              = src.nPSCCode              ;
		nDgnCode              = src.nDgnCode              ;
		nMeshedDgn_ElemAvg    = src.nMeshedDgn_ElemAvg    ;
		nMeshedDgn_ElemCell   = src.nMeshedDgn_ElemCell   ;
		dMeshedDgn_CellLen    = src.dMeshedDgn_CellLen    ;
		nMeshedDgn_XDirYDir   = src.nMeshedDgn_XDirYDir   ;
		nMeshedDgn_RebarRatio = src.nMeshedDgn_RebarRatio ;
		memcpy(dMeshedDgnOpt, src.dMeshedDgnOpt, sizeof(dMeshedDgnOpt));
		memcpy(nMeshedDgnOpt, src.nMeshedDgnOpt, sizeof(nMeshedDgnOpt));

		nWoodArmerTopBot   = 0;
		dWoodArmerAngle    = 0.0; 
		bRFrcdTopAlignment = TRUE;
		nRfrcdOutputPos = 2;
		bPsiFactor = FALSE;
		bCubicInterpolation = FALSE;
		dCubicFactor = 0.5;
	}
	void Convert940(T_FOUT_D_RW_940& src)
	{
		nOutputLocation = src.nOutputLocation;
		dScFactor = src.dScFactor;
		dAdjustFactor = src.dAdjustFactor;
		nAverage = src.nAverage;
		nLocalGlobal = src.nLocalGlobal;
		dMxxCoeff = src.dMxxCoeff;
		dMyyCoeff = src.dMyyCoeff;
		nMxxMyyMode = src.nMxxMyyMode;
		dMxyCoeff = src.dMxyCoeff;
		NucsK = src.NucsK;
		nPStrsSurface = src.nPStrsSurface;
		nMbDgrmNPlanK = src.nMbDgrmNPlanK;
		nMbDgrmStorK = src.nMbDgrmStorK;
		nMbDgrmDeciPt = src.nMbDgrmDeciPt;
		nMbDgrmDgrmType = src.nMbDgrmDgrmType;
		dMbDgrmScFt = src.dMbDgrmScFt;
		nMbdgrmDgrmComp = src.nMbdgrmDgrmComp;
		nMbDgrmOutType = src.nMbDgrmOutType;
		nMbDgrmSelectMode = src.nMbDgrmSelectMode;
		colValAxial = src.colValAxial;
		colValShear = src.colValShear;
		colValMoment = src.colValMoment;
		nSSFROutType = src.nSSFROutType;
		dSSFRAngle = src.dSSFRAngle;
		nThisComp = src.nThisComp;
		nThisFunction = src.nThisFunction;
		nFunctionKey = src.nFunctionKey;
		nHingeTimeIndex = src.nHingeTimeIndex;
		dHingeTimeStep = src.dHingeTimeStep;
		nThisTimeIndex = src.nThisTimeIndex;
		dThisTimeStep = src.dThisTimeStep;
		nHingeValType = src.nHingeValType;
		nHingeDutl = src.nHingeDutl;
		nCount_aPfudClwpK = src.nCount_aPfudClwpK;
		nPfudComp = src.nPfudComp;
		nPfudDir = src.nPfudDir;
		dEDgrmDefmScFc = src.dEDgrmDefmScFc;
		nEDgrmScFcType = src.nEDgrmScFcType;
		dEDgrmAutoScFc = src.dEDgrmAutoScFc;
		dEDgrmRefValue = src.dEDgrmRefValue;
		dEDgrmRefValueScFc = src.dEDgrmRefValueScFc;
		nCount_aSelectedDiagK = src.nCount_aSelectedDiagK;
		memcpy(bFoutD, src.bFoutD, sizeof(bFoutD));
		nInerForceOrAcc = src.nInerForceOrAcc;
		nUsedSerialModeIdx = src.nUsedSerialModeIdx;
		bMode = src.bMode;
		bOnCuttingLine = src.bOnCuttingLine;
		bOnCuttingPlane = src.bOnCuttingPlane;
		nCount_aCutPlanes = src.nCount_aCutPlanes;
		nSolidFreeEdgeType = src.nSolidFreeEdgeType;
		bIsoSurface = src.bIsoSurface;
		nCount_aIsoValues = src.nCount_aIsoValues;
		nTransparentIntensity = src.nTransparentIntensity;
		nIsoValueMode = src.nIsoValueMode;
		bDrawPolyOutLine = src.bDrawPolyOutLine;
		bTransparent = src.bTransparent;
		dCutLineScaleFactor = src.dCutLineScaleFactor;
		bCutLineOutputMinMax = src.bCutLineOutputMinMax;
		nCount_aCutLineKeys = src.nCount_aCutLineKeys;
		nCutPlaneHorizVert = src.nCutPlaneHorizVert;
		colCutPlaneColor = src.colCutPlaneColor;
		nCuttingLineDir = src.nCuttingLineDir;
		colCuttinLineColor = src.colCuttinLineColor;
		bDrawPolyLine = src.bDrawPolyLine;
		bCutLineMinMaxOutOnly = src.bCutLineMinMaxOutOnly;
		bCutLineReverse = src.bCutLineReverse;
		bShowCutLineGraph = src.bShowCutLineGraph;
		nCutLineOrPlane = src.nCutLineOrPlane;
		nInflKeyNode = src.nInflKeyNode;
		bInflIclImpFct = src.bInflIclImpFct;
		bLocalValue = src.bLocalValue;
		dCritLineVal = src.dCritLineVal;
		nPSCCirteriaType = src.nPSCCirteriaType;
		nFillType = src.nFillType;
		dPSCDgrmScFc = src.dPSCDgrmScFc;
		nDgrmDirType = src.nDgrmDirType;
		nDgrmDirOptn = src.nDgrmDirOptn;
		bPSCSignPos = src.bPSCSignPos;
		bPSCSignNeg = src.bPSCSignNeg;
		nResistanceLineCol = src.nResistanceLineCol;
		nResistanceLineThk = src.nResistanceLineThk;
		nPSCCompCH = src.nPSCCompCH;
		nPSCCompJP = src.nPSCCompJP;
		nPSCCode = src.nPSCCode;
		nDgnCode = src.nDgnCode;
		nMeshedDgn_ElemAvg = src.nMeshedDgn_ElemAvg;
		nMeshedDgn_ElemCell = src.nMeshedDgn_ElemCell;
		dMeshedDgn_CellLen = src.dMeshedDgn_CellLen;
		nMeshedDgn_XDirYDir = src.nMeshedDgn_XDirYDir;
		nMeshedDgn_RebarRatio = src.nMeshedDgn_RebarRatio;
		memcpy(dMeshedDgnOpt, src.dMeshedDgnOpt, sizeof(dMeshedDgnOpt));
		memcpy(nMeshedDgnOpt, src.nMeshedDgnOpt, sizeof(nMeshedDgnOpt));

		nWoodArmerTopBot = src.nWoodArmerTopBot;
		dWoodArmerAngle = src.dWoodArmerAngle;

		bRFrcdTopAlignment = TRUE;
		nRfrcdOutputPos = 2;
		bPsiFactor = FALSE;
		bCubicInterpolation = FALSE;
		dCubicFactor = 0.5;
	}
};
struct T_FLCS_D
{
	UINT nLoadMinMaxType ;
	UINT nLoadCaseType   ;
	UINT nLoadCaseKey    ;
	UINT nCurStep        ; // Step
	void Initialize()
	{
		nLoadMinMaxType = 0; 
		nLoadCaseType   = 0;
		nLoadCaseKey    = 0;
		nCurStep        = 0;
	}
	T_FLCS_D() { Initialize(); }
	T_FLCS_D(const T_FLCS_D &src) { *this = src; }
};

// Post Data
// v821 : Meshed Design 옵션 추가
struct T_FRSI_D_821
{
	int       nCurrentMode      ; // CDlgBarMode::DLGBAR_MODE_GPS_...
	int       nComponent        ; // Component
	int       nComponentSub     ; // Sub Component
	int       nPart             ; // Part
	T_FDSP_D_821  DisplayD          ; // Display Option
	T_FOUT_D_821  OutputD           ; // Output Location
	T_FLCS_D  LoadcaseD         ; // Load Case Option
	void Initialize()
	{
		nCurrentMode      = -1;
		nComponent        = 0;
		nComponentSub     = 0;
		nPart             = 0;
		DisplayD          .Initialize();
		OutputD           .Initialize();
		LoadcaseD         .Initialize();
	}
	T_FRSI_D_821() { Initialize(); }
	T_FRSI_D_821(const T_FRSI_D_821 &src) { *this = src; }
	T_FRSI_D_821& operator=(const T_FRSI_D_821 &src)
	{
		nCurrentMode      = src.nCurrentMode      ;
		nComponent        = src.nComponent        ;
		nComponentSub     = src.nComponentSub     ;
		nPart             = src.nPart             ;
		DisplayD          = src.DisplayD          ;
		OutputD           = src.OutputD           ;
		LoadcaseD         = src.LoadcaseD         ;
		return *this;
	}
};
struct T_FRSI_D_RW_821
{
	int           nCurrentMode      ;
	int           nComponent        ;
	int           nComponentSub     ;
	int           nPart             ;
	T_FDSP_D_821      DisplayD          ;
	T_FOUT_D_RW_821   OutputD           ;
	T_FLCS_D      LoadcaseD         ;
	void Get(T_FRSI_D_821 &rData)
	{
		rData.nCurrentMode       = nCurrentMode     ;
		rData.nComponent         = nComponent       ;
		rData.nComponentSub      = nComponentSub    ;
		rData.nPart              = nPart            ;
		rData.DisplayD           = DisplayD         ;
		OutputD.Get(rData.OutputD);
		rData.LoadcaseD          = LoadcaseD        ;
	}
	void Set(T_FRSI_D_821 Data)
	{
		nCurrentMode      = Data.nCurrentMode      ;
		nComponent        = Data.nComponent        ;
		nComponentSub     = Data.nComponentSub     ;
		nPart             = Data.nPart             ;
		DisplayD          = Data.DisplayD          ;
		OutputD.Set(Data.OutputD);
		LoadcaseD         = Data.LoadcaseD         ;
	}
};

struct T_FRSI_D_825
{
	int       nCurrentMode      ; // CDlgBarMode::DLGBAR_MODE_GPS_...
	int       nComponent        ; // Component
	int       nComponentSub     ; // Sub Component
	int       nPart             ; // Part
	T_FDSP_D_883  DisplayD          ; // Display Option
	T_FOUT_D_825  OutputD           ; // Output Location
	T_FLCS_D  LoadcaseD         ; // Load Case Option
	void Initialize()
	{
		nCurrentMode      = -1;
		nComponent        = 0;
		nComponentSub     = 0;
		nPart             = 0;
		DisplayD          .Initialize();
		OutputD           .Initialize();
		LoadcaseD         .Initialize();
	}
	T_FRSI_D_825() { Initialize(); }
	T_FRSI_D_825(const T_FRSI_D_825 &src) { *this = src; }
	T_FRSI_D_825& operator=(const T_FRSI_D_825 &src)
	{
		nCurrentMode      = src.nCurrentMode      ;
		nComponent        = src.nComponent        ;
		nComponentSub     = src.nComponentSub     ;
		nPart             = src.nPart             ;
		DisplayD          = src.DisplayD          ;
		OutputD           = src.OutputD           ;
		LoadcaseD         = src.LoadcaseD         ;
		return *this;
	}
};
struct T_FRSI_D_RW_825
{
	int           nCurrentMode      ;
	int           nComponent        ;
	int           nComponentSub     ;
	int           nPart             ;
	T_FDSP_D_883  DisplayD          ;
	T_FOUT_D_RW_825   OutputD           ;
	T_FLCS_D      LoadcaseD         ;
	void Get(T_FRSI_D_825 &rData)
	{
		rData.nCurrentMode       = nCurrentMode     ;
		rData.nComponent         = nComponent       ;
		rData.nComponentSub      = nComponentSub    ;
		rData.nPart              = nPart            ;
		rData.DisplayD           = DisplayD         ;
		OutputD.Get(rData.OutputD);
		rData.LoadcaseD          = LoadcaseD        ;
	}
	void Set(T_FRSI_D_825 Data)
	{
		nCurrentMode      = Data.nCurrentMode      ;
		nComponent        = Data.nComponent        ;
		nComponentSub     = Data.nComponentSub     ;
		nPart             = Data.nPart             ;
		DisplayD          = Data.DisplayD          ;
		OutputD.Set(Data.OutputD);
		LoadcaseD         = Data.LoadcaseD         ;
	}
};

struct T_FRSI_D_851
{
	int       nCurrentMode      ; // CDlgBarMode::DLGBAR_MODE_GPS_...
	int       nComponent        ; // Component
	int       nComponentSub     ; // Sub Component
	int       nPart             ; // Part
	T_FDSP_D_883  DisplayD          ; // Display Option
	T_FOUT_D_851  OutputD       ; // Output Location
	T_FLCS_D  LoadcaseD         ; // Load Case Option
	void Initialize()
	{
		nCurrentMode      = -1;
		nComponent        = 0;
		nComponentSub     = 0;
		nPart             = 0;
		DisplayD          .Initialize();
		OutputD           .Initialize();
		LoadcaseD         .Initialize();
	}
	T_FRSI_D_851() { Initialize(); }
	T_FRSI_D_851(const T_FRSI_D_851 &src) { *this = src; }
	T_FRSI_D_851& operator=(const T_FRSI_D_851 &src)
	{
		nCurrentMode      = src.nCurrentMode      ;
		nComponent        = src.nComponent        ;
		nComponentSub     = src.nComponentSub     ;
		nPart             = src.nPart             ;
		DisplayD          = src.DisplayD          ;
		OutputD           = src.OutputD           ;
		LoadcaseD         = src.LoadcaseD         ;
		return *this;
	}
};
struct T_FRSI_D_RW_851
{
	int           nCurrentMode      ;
	int           nComponent        ;
	int           nComponentSub     ;
	int           nPart             ;
	T_FDSP_D_883  DisplayD          ;
	T_FOUT_D_RW_851   OutputD       ;
	T_FLCS_D      LoadcaseD         ;
	void Get(T_FRSI_D_851 &rData)
	{
		rData.nCurrentMode       = nCurrentMode     ;
		rData.nComponent         = nComponent       ;
		rData.nComponentSub      = nComponentSub    ;
		rData.nPart              = nPart            ;
		rData.DisplayD           = DisplayD         ;
		OutputD.Get(rData.OutputD);
		rData.LoadcaseD          = LoadcaseD        ;
	}
	void Set(T_FRSI_D_851 Data)
	{
		nCurrentMode      = Data.nCurrentMode      ;
		nComponent        = Data.nComponent        ;
		nComponentSub     = Data.nComponentSub     ;
		nPart             = Data.nPart             ;
		DisplayD          = Data.DisplayD          ;
		OutputD.Set(Data.OutputD);
		LoadcaseD         = Data.LoadcaseD         ;
	}
};

struct T_FRSI_D_865
{
	int       nCurrentMode      ; // CDlgBarMode::DLGBAR_MODE_GPS_...
	int       nComponent        ; // Component
	int       nComponentSub     ; // Sub Component
	int       nPart             ; // Part
	T_FDSP_D_883 DisplayD          ; // Display Option
	T_FOUT_D_940  OutputD           ; // Output Location
	T_FLCS_D  LoadcaseD         ; // Load Case Option
	void Initialize()
	{
		nCurrentMode      = -1;
		nComponent        = 0;
		nComponentSub     = 0;
		nPart             = 0;
		DisplayD          .Initialize();
		OutputD           .Initialize();
		LoadcaseD         .Initialize();
	}
	T_FRSI_D_865() { Initialize(); }
	T_FRSI_D_865(const T_FRSI_D_865 &src) { *this = src; }
	T_FRSI_D_865& operator=(const T_FRSI_D_865 &src)
	{
		nCurrentMode      = src.nCurrentMode      ;
		nComponent        = src.nComponent        ;
		nComponentSub     = src.nComponentSub     ;
		nPart             = src.nPart             ;
		DisplayD          = src.DisplayD          ;
		OutputD           = src.OutputD           ;
		LoadcaseD         = src.LoadcaseD         ;
		return *this;
	}
};

struct T_FRSI_D_RW_865
{
	int           nCurrentMode      ;
	int           nComponent        ;
	int           nComponentSub     ;
	int           nPart             ;
	T_FDSP_D_883  DisplayD          ;
	T_FOUT_D_RW_940   OutputD           ;
	T_FLCS_D      LoadcaseD         ;
	void Get(T_FRSI_D_865 &rData)
	{
		rData.nCurrentMode       = nCurrentMode     ;
		rData.nComponent         = nComponent       ;
		rData.nComponentSub      = nComponentSub    ;
		rData.nPart              = nPart            ;
		rData.DisplayD           = DisplayD         ;
		OutputD.Get(rData.OutputD);
		rData.LoadcaseD          = LoadcaseD        ;
	}
	void Set(T_FRSI_D_865 Data)
	{
		nCurrentMode      = Data.nCurrentMode      ;
		nComponent        = Data.nComponent        ;
		nComponentSub     = Data.nComponentSub     ;
		nPart             = Data.nPart             ;
		DisplayD          = Data.DisplayD          ;
		OutputD.Set(Data.OutputD);
		LoadcaseD         = Data.LoadcaseD         ;
	}
};
struct T_FRSI_D_883
{
	int       nCurrentMode      ; // CDlgBarMode::DLGBAR_MODE_GPS_...
	int       nComponent        ; // Component
	int       nComponentSub     ; // Sub Component
	int       nPart             ; // Part
	int		  nComp7thDof		;
	T_FDSP_D_883 DisplayD       ; // Display Option
	T_FOUT_D_940  OutputD           ; // Output Location
	T_FLCS_D  LoadcaseD         ; // Load Case Option
	void Initialize()
	{
		nCurrentMode      = -1;
		nComponent        = 0;
		nComponentSub     = 0;
		nPart             = 0;
		nComp7thDof		  = 0;
		DisplayD          .Initialize();
		OutputD           .Initialize();
		LoadcaseD         .Initialize();
	}
	T_FRSI_D_883() { Initialize(); }
	T_FRSI_D_883(const T_FRSI_D_883 &src) { *this = src; }
	T_FRSI_D_883& operator=(const T_FRSI_D_883 &src)
	{
		nCurrentMode      = src.nCurrentMode      ;
		nComponent        = src.nComponent        ;
		nComponentSub     = src.nComponentSub     ;
		nPart             = src.nPart             ;
		nComp7thDof       = src.nComp7thDof       ;
		DisplayD          = src.DisplayD          ;
		OutputD           = src.OutputD           ;
		LoadcaseD         = src.LoadcaseD         ;
		return *this;
	}
};

struct T_FRSI_D_RW_883
{
	int           nCurrentMode      ;
	int           nComponent        ;
	int           nComponentSub     ;
	int           nPart             ;
	int			  nComp7thDof		;
	T_FDSP_D_883  DisplayD          ;
	T_FOUT_D_RW_940   OutputD           ;
	T_FLCS_D      LoadcaseD         ;
	void Get(T_FRSI_D_883 &rData)
	{
		rData.nCurrentMode       = nCurrentMode     ;
		rData.nComponent         = nComponent       ;
		rData.nComponentSub      = nComponentSub    ;
		rData.nPart              = nPart            ;
		rData.DisplayD           = DisplayD         ;
		OutputD.Get(rData.OutputD);
		rData.LoadcaseD          = LoadcaseD        ;
	}
	void Set(T_FRSI_D_883 Data)
	{
		nCurrentMode      = Data.nCurrentMode      ;
		nComponent        = Data.nComponent        ;
		nComponentSub     = Data.nComponentSub     ;
		nPart             = Data.nPart             ;
		DisplayD          = Data.DisplayD          ;
		OutputD.Set(Data.OutputD);
		LoadcaseD         = Data.LoadcaseD         ;
	}
};

struct T_FRSI_D_940
{
	int       nCurrentMode; // CDlgBarMode::DLGBAR_MODE_GPS_...
	int       nComponent; // Component
	int       nComponentSub; // Sub Component
	int       nPart; // Part
	int       nComp7thDof; // Comp7thDof
	T_FDSP_D  DisplayD; // Display Option
	T_FOUT_D_940  OutputD; // Output Location
	T_FLCS_D  LoadcaseD; // Load Case Option
	void Initialize()
	{
		nCurrentMode = -1;
		nComponent = 0;
		nComponentSub = 0;
		nPart = 0;
		nComp7thDof = 0;
		DisplayD.Initialize();
		OutputD.Initialize();
		LoadcaseD.Initialize();
	}
	T_FRSI_D_940() { Initialize(); }
	T_FRSI_D_940(const T_FRSI_D_940& src) { *this = src; }
	T_FRSI_D_940& operator=(const T_FRSI_D_940& src)
	{
		nCurrentMode = src.nCurrentMode;
		nComponent = src.nComponent;
		nComponentSub = src.nComponentSub;
		nPart = src.nPart;
		nComp7thDof = src.nComp7thDof;
		DisplayD = src.DisplayD;
		OutputD = src.OutputD;
		LoadcaseD = src.LoadcaseD;
		return *this;
	}
};

// Post Data
struct T_FRSI_D
{
	int       nCurrentMode      ; // CDlgBarMode::DLGBAR_MODE_GPS_...
	int       nComponent        ; // Component
	int       nComponentSub     ; // Sub Component
	int       nPart             ; // Part
	int       nComp7thDof       ; // Comp7thDof
	T_FDSP_D  DisplayD          ; // Display Option
	T_FOUT_D  OutputD           ; // Output Location
	T_FLCS_D  LoadcaseD         ; // Load Case Option
	void Initialize()
	{
		nCurrentMode      = -1;
		nComponent        = 0;
		nComponentSub     = 0;
		nPart             = 0;
		nComp7thDof       = 0;
		DisplayD          .Initialize();
		OutputD           .Initialize();
		LoadcaseD         .Initialize();
	}
	T_FRSI_D() { Initialize(); }
	T_FRSI_D(const T_FRSI_D &src) { *this = src; }
	T_FRSI_D& operator=(const T_FRSI_D &src)
	{
		nCurrentMode      = src.nCurrentMode      ;
		nComponent        = src.nComponent        ;
		nComponentSub     = src.nComponentSub     ;
		nPart             = src.nPart             ;
		nComp7thDof       = src.nComp7thDof       ;
		DisplayD          = src.DisplayD          ;
		OutputD           = src.OutputD           ;
		LoadcaseD         = src.LoadcaseD         ;
		return *this;
	}
	void Convert821(T_FRSI_D_821& src)
	{
		nCurrentMode      = src.nCurrentMode      ;
		nComponent        = src.nComponent        ;
		nComponentSub     = src.nComponentSub     ;
		nPart             = src.nPart             ;
		nComp7thDof       = 0                     ;
		DisplayD.Convert821(src.DisplayD)         ;
		OutputD.Convert821(src.OutputD)           ;
		LoadcaseD         = src.LoadcaseD         ;
	}
	void Convert825(T_FRSI_D_825& src)
	{
		nCurrentMode      = src.nCurrentMode      ;
		nComponent        = src.nComponent        ;
		nComponentSub     = src.nComponentSub     ;
		nPart             = src.nPart             ;
		nComp7thDof       = 0                     ;
		DisplayD.Convert883(src.DisplayD)         ;
		OutputD.Convert825(src.OutputD)           ;
		LoadcaseD         = src.LoadcaseD         ;
	}
	void Convert851(T_FRSI_D_851& src)
	{
		nCurrentMode      = src.nCurrentMode      ;
		nComponent        = src.nComponent        ;
		nComponentSub     = src.nComponentSub     ;
		nPart             = src.nPart             ;
		nComp7thDof       = 0                     ;
		DisplayD.Convert883(src.DisplayD)         ;
		OutputD.Convert851(src.OutputD)           ;
		LoadcaseD         = src.LoadcaseD         ;
	}
	void Convert865(T_FRSI_D_865& src)
	{
		nCurrentMode      = src.nCurrentMode      ;
		nComponent        = src.nComponent        ;
		nComponentSub     = src.nComponentSub     ;
		nPart             = src.nPart             ;
		nComp7thDof       = 0                     ;
		DisplayD.Convert883(src.DisplayD)         ;
		OutputD.Convert940(src.OutputD)           ;
		LoadcaseD         = src.LoadcaseD         ;
	}
	void Convert883(T_FRSI_D_883& src)
	{
		nCurrentMode      = src.nCurrentMode      ;
		nComponent        = src.nComponent        ;
		nComponentSub     = src.nComponentSub     ;
		nPart             = src.nPart             ;
		nComp7thDof       = src.nComp7thDof       ;
		DisplayD.Convert883(src.DisplayD)         ;
		OutputD.Convert940(src.OutputD);
		LoadcaseD         = src.LoadcaseD         ;
	}
	void Convert940(T_FRSI_D_940& src)
	{
		nCurrentMode = src.nCurrentMode;
		nComponent = src.nComponent;
		nComponentSub = src.nComponentSub;
		nPart = src.nPart;
		nComp7thDof = src.nComp7thDof;
		DisplayD = src.DisplayD;
		OutputD.Convert940(src.OutputD);
		LoadcaseD = src.LoadcaseD;
	}
};

struct T_FRSI_D_RW_940
{
	int           nCurrentMode;
	int           nComponent;
	int           nComponentSub;
	int           nPart;
	int           nComp7thDof;
	T_FDSP_D      DisplayD;
	T_FOUT_D_RW_940   OutputD;
	T_FLCS_D      LoadcaseD;
	void Get(T_FRSI_D_940& rData)
	{
		rData.nCurrentMode = nCurrentMode;
		rData.nComponent = nComponent;
		rData.nComponentSub = nComponentSub;
		rData.nPart = nPart;
		rData.nComp7thDof = nComp7thDof;
		rData.DisplayD = DisplayD;
		OutputD.Get(rData.OutputD);
		rData.LoadcaseD = LoadcaseD;
	}
	void Set(T_FRSI_D_940 Data)
	{
		nCurrentMode = Data.nCurrentMode;
		nComponent = Data.nComponent;
		nComponentSub = Data.nComponentSub;
		nPart = Data.nPart;
		nComp7thDof = Data.nComp7thDof;
		DisplayD = Data.DisplayD;
		OutputD.Set(Data.OutputD);
		LoadcaseD = Data.LoadcaseD;
	}
};

struct T_FRSI_D_RW
{
	int           nCurrentMode      ;
	int           nComponent        ;
	int           nComponentSub     ;
	int           nPart             ;
	int           nComp7thDof       ; 
	T_FDSP_D      DisplayD          ;
	T_FOUT_D_RW   OutputD           ;
	T_FLCS_D      LoadcaseD         ;
	void Get(T_FRSI_D &rData)
	{
		rData.nCurrentMode       = nCurrentMode     ;
		rData.nComponent         = nComponent       ;
		rData.nComponentSub      = nComponentSub    ;
		rData.nPart              = nPart            ;
		rData.nComp7thDof        = nComp7thDof      ;
		rData.DisplayD           = DisplayD         ;
		OutputD.Get(rData.OutputD);
		rData.LoadcaseD          = LoadcaseD        ;
	}
	void Set(T_FRSI_D Data)
	{
		nCurrentMode      = Data.nCurrentMode      ;
		nComponent        = Data.nComponent        ;
		nComponentSub     = Data.nComponentSub     ;
		nPart             = Data.nPart             ;
		nComp7thDof       = Data.nComp7thDof       ;
		DisplayD          = Data.DisplayD          ;
		OutputD.Set(Data.OutputD);
		LoadcaseD         = Data.LoadcaseD         ;
	}
	void Convert821(T_FRSI_D_RW_821& Data)
	{
		nCurrentMode      = Data.nCurrentMode      ;
		nComponent        = Data.nComponent        ;
		nComponentSub     = Data.nComponentSub     ;
		nPart             = Data.nPart             ;
		nComp7thDof       = 0                      ;
		DisplayD.Convert821(Data.DisplayD)         ;
		OutputD.Convert821(Data.OutputD)           ;
		LoadcaseD         = Data.LoadcaseD         ;
	}
	void Convert825(T_FRSI_D_RW_825& Data)
	{
		nCurrentMode      = Data.nCurrentMode      ;
		nComponent        = Data.nComponent        ;
		nComponentSub     = Data.nComponentSub     ;
		nPart             = Data.nPart             ;
		nComp7thDof       = 0                      ;
		DisplayD.Convert883(Data.DisplayD)         ;
		OutputD.Convert825(Data.OutputD)           ;
		LoadcaseD         = Data.LoadcaseD         ;
	}
	void Convert851(T_FRSI_D_RW_851& Data)
	{
		nCurrentMode      = Data.nCurrentMode      ;
		nComponent        = Data.nComponent        ;
		nComponentSub     = Data.nComponentSub     ;
		nPart             = Data.nPart             ;
		nComp7thDof       = 0                      ;
		DisplayD.Convert883(Data.DisplayD)         ;
		OutputD.Convert851(Data.OutputD)           ;
		LoadcaseD         = Data.LoadcaseD         ;
	}
	void Convert865(T_FRSI_D_RW_865& Data)
	{
		nCurrentMode      = Data.nCurrentMode      ;
		nComponent        = Data.nComponent        ;
		nComponentSub     = Data.nComponentSub     ;
		nPart             = Data.nPart             ;
		nComp7thDof       = 0                      ;
		DisplayD.Convert883(Data.DisplayD)         ;
		OutputD.Convert940(Data.OutputD);
		LoadcaseD         = Data.LoadcaseD         ;
	}
	void Convert883(T_FRSI_D_RW_883& Data)
	{
		nCurrentMode      = Data.nCurrentMode      ;
		nComponent        = Data.nComponent        ;
		nComponentSub     = Data.nComponentSub     ;
		nPart             = Data.nPart             ;
		nComp7thDof       = Data.nComp7thDof       ;
		DisplayD.Convert883(Data.DisplayD)         ;
		OutputD.Convert940(Data.OutputD)           ;
		LoadcaseD         = Data.LoadcaseD         ;
	}
	void Convert940(T_FRSI_D_RW_940& Data)
	{
		nCurrentMode = Data.nCurrentMode;
		nComponent = Data.nComponent;
		nComponentSub = Data.nComponentSub;
		nPart = Data.nPart;
		nComp7thDof = Data.nComp7thDof;
		DisplayD = Data.DisplayD;
		OutputD.Convert940(Data.OutputD);
		LoadcaseD = Data.LoadcaseD;
	}
};

///// user defined figure
// T_PRUD_D가 변경됨. 20110418 KIMJM
struct T_UFIG_D_790
{
	CString  strName;
	CString  strCaption;  // Caption
	BOOL     bBW;       // black & white
	int      nStagStatus; // 0:base/PostCS, 1:stage, 2:min/max
	T_STAG_K CurStagK; // nStagStatus=1일때 사용
	BOOL     bPre;
	T_PRUD_D_790 PrePrudD;  // pre & common
	T_FRSI_D_821 PostFrsiD; // post
	void Initialize()
	{
		strName     = _T("");
		strCaption  = _T("");
		bBW         = FALSE ;
		nStagStatus = 0;
		CurStagK    = 0;
		bPre        = FALSE;
		PrePrudD .Initialize();
		PostFrsiD.Initialize();
	}
	T_UFIG_D_790() { Initialize(); }
	T_UFIG_D_790(const T_UFIG_D_790 &src) { *this = src; }
	T_UFIG_D_790& operator=(const T_UFIG_D_790 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD    = src.PrePrudD;
		PostFrsiD   = src.PostFrsiD;
		return *this;
	}
};
struct T_UFIG_D_810
{
	CString  strName;
	CString  strCaption;  // Caption
	BOOL     bBW;       // black & white
	int      nStagStatus; // 0:base/PostCS, 1:stage, 2:min/max
	T_STAG_K CurStagK; // nStagStatus=1일때 사용
	BOOL     bPre;
	T_PRUD_D_810 PrePrudD;  // pre & common
	T_FRSI_D_821 PostFrsiD; // post
	void Initialize()
	{
		strName     = _T("");
		strCaption  = _T("");
		bBW         = FALSE ;
		nStagStatus = 0;
		CurStagK    = 0;
		bPre        = FALSE;
		PrePrudD .Initialize();
		PostFrsiD.Initialize();
	}
	T_UFIG_D_810() { Initialize(); }
	T_UFIG_D_810(const T_UFIG_D_810 &src) { *this = src; }
	T_UFIG_D_810& operator=(const T_UFIG_D_810 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD    = src.PrePrudD;
		PostFrsiD   = src.PostFrsiD;
		return *this;
	}
};
// v821 : Meshed Design 옵션 추가
struct T_UFIG_D_821
{
	CString  strName;
	CString  strCaption;  // Caption
	BOOL     bBW;       // black & white
	int      nStagStatus; // 0:base/PostCS, 1:stage, 2:min/max
	T_STAG_K CurStagK; // nStagStatus=1일때 사용
	BOOL     bPre;
	T_PRUD_D_821 PrePrudD;  // pre & common
	T_FRSI_D_821 PostFrsiD; // post
	void Initialize()
	{
		strName     = _T("");
		strCaption  = _T("");
		bBW         = FALSE ;
		nStagStatus = 0;
		CurStagK    = 0;
		bPre        = FALSE;
		PrePrudD .Initialize();
		PostFrsiD.Initialize();
	}
	T_UFIG_D_821() { Initialize(); }
	T_UFIG_D_821(const T_UFIG_D_821 &src) { *this = src; }
	T_UFIG_D_821& operator=(const T_UFIG_D_821 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD    = src.PrePrudD;
		PostFrsiD   = src.PostFrsiD;
		return *this;
	}
};
struct T_UFIG_D_825
{
	CString  strName;
	CString  strCaption;  // Caption
	BOOL     bBW;       // black & white
	int      nStagStatus; // 0:base/PostCS, 1:stage, 2:min/max
	T_STAG_K CurStagK; // nStagStatus=1일때 사용
	BOOL     bPre;
	T_PRUD_D_835 PrePrudD;  // pre & common
	T_FRSI_D_825 PostFrsiD; // post
	void Initialize()
	{
		strName     = _T("");
		strCaption  = _T("");
		bBW         = FALSE ;
		nStagStatus = 0;
		CurStagK    = 0;
		bPre        = FALSE;
		PrePrudD .Initialize();
		PostFrsiD.Initialize();
	}
	T_UFIG_D_825() { Initialize(); }
	T_UFIG_D_825(const T_UFIG_D_825 &src) { *this = src; }
	T_UFIG_D_825& operator=(const T_UFIG_D_825 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD    = src.PrePrudD;
		PostFrsiD   = src.PostFrsiD;
		return *this;
	}
};
struct T_UFIG_D_835
{
	CString  strName;
	CString  strCaption;  // Caption
	BOOL     bBW;       // black & white
	int      nStagStatus; // 0:base/PostCS, 1:stage, 2:min/max
	T_STAG_K CurStagK; // nStagStatus=1일때 사용
	BOOL     bPre;
	T_PRUD_D_835 PrePrudD;  // pre & common
	T_FRSI_D_851 PostFrsiD; // post
	void Initialize()
	{
		strName     = _T("");
		strCaption  = _T("");
		bBW         = FALSE ;
		nStagStatus = 0;
		CurStagK    = 0;
		bPre        = FALSE;
		PrePrudD .Initialize();
		PostFrsiD.Initialize();
	}
	T_UFIG_D_835() { Initialize(); }
	T_UFIG_D_835(const T_UFIG_D_835 &src) { *this = src; }
	T_UFIG_D_835& operator=(const T_UFIG_D_835 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD    = src.PrePrudD;
		PostFrsiD   = src.PostFrsiD;
		return *this;
	}
};
struct T_UFIG_D_841
{
	CString  strName;
	CString  strCaption;  // Caption
	BOOL     bBW;       // black & white
	int      nStagStatus; // 0:base/PostCS, 1:stage, 2:min/max
	T_STAG_K CurStagK; // nStagStatus=1일때 사용
	BOOL     bPre;
	T_PRUD_D_841 PrePrudD;  // pre & common
	T_FRSI_D_851 PostFrsiD; // post
	void Initialize()
	{
		strName     = _T("");
		strCaption  = _T("");
		bBW         = FALSE ;
		nStagStatus = 0;
		CurStagK    = 0;
		bPre        = FALSE;
		PrePrudD .Initialize();
		PostFrsiD.Initialize();
	}
	T_UFIG_D_841() { Initialize(); }
	T_UFIG_D_841(const T_UFIG_D_841 &src) { *this = src; }
	T_UFIG_D_841& operator=(const T_UFIG_D_841 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD    = src.PrePrudD;
		PostFrsiD   = src.PostFrsiD;
		return *this;
	}
};

struct T_UFIG_D_845
{
	CString  strName;
	CString  strCaption;  // Caption
	BOOL     bBW;       // black & white
	int      nStagStatus; // 0:base/PostCS, 1:stage, 2:min/max
	T_STAG_K CurStagK; // nStagStatus=1일때 사용
	BOOL     bPre;
	T_PRUD_D_845 PrePrudD;  // pre & common
	T_FRSI_D_851 PostFrsiD; // post
	void Initialize()
	{
		strName     = _T("");
		strCaption  = _T("");
		bBW         = FALSE ;
		nStagStatus = 0;
		CurStagK    = 0;
		bPre        = FALSE;
		PrePrudD .Initialize();
		PostFrsiD.Initialize();
	}
	T_UFIG_D_845() { Initialize(); }
	T_UFIG_D_845(const T_UFIG_D_845 &src) { *this = src; }
	T_UFIG_D_845& operator=(const T_UFIG_D_845 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD    = src.PrePrudD;
		PostFrsiD   = src.PostFrsiD;
		return *this;
	}
};

struct T_UFIG_D_851
{
	CString  strName;
	CString  strCaption;  // Caption
	BOOL     bBW;       // black & white
	int      nStagStatus; // 0:base/PostCS, 1:stage, 2:min/max
	T_STAG_K CurStagK; // nStagStatus=1일때 사용
	BOOL     bPre;
	T_PRUD_D_855 PrePrudD;  // pre & common
	T_FRSI_D_851 PostFrsiD; // post
	void Initialize()
	{
		strName     = _T("");
		strCaption  = _T("");
		bBW         = FALSE ;
		nStagStatus = 0;
		CurStagK    = 0;
		bPre        = FALSE;
		PrePrudD .Initialize();
		PostFrsiD.Initialize();
	}
	T_UFIG_D_851() { Initialize(); }
	T_UFIG_D_851(const T_UFIG_D_851 &src) { *this = src; }
	T_UFIG_D_851& operator=(const T_UFIG_D_851 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD    = src.PrePrudD;
		PostFrsiD   = src.PostFrsiD;
		return *this;
	}
};

struct T_UFIG_D_855
{
	CString  strName;
	CString  strCaption;  // Caption
	BOOL     bBW;       // black & white
	int      nStagStatus; // 0:base/PostCS, 1:stage, 2:min/max
	T_STAG_K CurStagK; // nStagStatus=1일때 사용
	BOOL     bPre;
	T_PRUD_D_855 PrePrudD;  // pre & common
	T_FRSI_D_865 PostFrsiD; // post
	void Initialize()
	{
		strName     = _T("");
		strCaption  = _T("");
		bBW         = FALSE ;
		nStagStatus = 0;
		CurStagK    = 0;
		bPre        = FALSE;
		PrePrudD .Initialize();
		PostFrsiD.Initialize();
	}
	T_UFIG_D_855() { Initialize(); }
	T_UFIG_D_855(const T_UFIG_D_855 &src) { *this = src; }
	T_UFIG_D_855& operator=(const T_UFIG_D_855 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD    = src.PrePrudD;
		PostFrsiD   = src.PostFrsiD;
		return *this;
	}
};

struct T_UFIG_D_860
{
	CString  strName;
	CString  strCaption;  // Caption
	BOOL     bBW;       // black & white
	int      nStagStatus; // 0:base/PostCS, 1:stage, 2:min/max
	T_STAG_K CurStagK; // nStagStatus=1일때 사용
	BOOL     bPre;
	T_PRUD_D_860 PrePrudD;  // pre & common
	T_FRSI_D_865 PostFrsiD; // post
	void Initialize()
	{
		strName     = _T("");
		strCaption  = _T("");
		bBW         = FALSE ;
		nStagStatus = 0;
		CurStagK    = 0;
		bPre        = FALSE;
		PrePrudD .Initialize();
		PostFrsiD.Initialize();
	}
	T_UFIG_D_860() { Initialize(); }
	T_UFIG_D_860(const T_UFIG_D_860 &src) { *this = src; }
	T_UFIG_D_860& operator=(const T_UFIG_D_860 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD    = src.PrePrudD;
		PostFrsiD   = src.PostFrsiD;
		return *this;
	}
};

struct T_UFIG_D_865
{
	CString  strName;
	CString  strCaption;  // Caption
	BOOL     bBW;       // black & white
	int      nStagStatus; // 0:base/PostCS, 1:stage, 2:min/max
	T_STAG_K CurStagK; // nStagStatus=1일때 사용
	BOOL     bPre;
	T_PRUD_D_865 PrePrudD;  // pre & common
	T_FRSI_D_865 PostFrsiD; // post
	void Initialize()
	{
		strName     = _T("");
		strCaption  = _T("");
		bBW         = FALSE ;
		nStagStatus = 0;
		CurStagK    = 0;
		bPre        = FALSE;
		PrePrudD .Initialize();
		PostFrsiD.Initialize();
	}
	T_UFIG_D_865() { Initialize(); }
	T_UFIG_D_865(const T_UFIG_D_865 &src) { *this = src; }
	T_UFIG_D_865& operator=(const T_UFIG_D_865 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD    = src.PrePrudD;
		PostFrsiD   = src.PostFrsiD;
		return *this;
	}
};
struct T_UFIG_D_870
{
	CString  strName;
	CString  strCaption;  // Caption
	BOOL     bBW;       // black & white
	int      nStagStatus; // 0:base/PostCS, 1:stage, 2:min/max
	T_STAG_K CurStagK; // nStagStatus=1일때 사용
	BOOL     bPre;
	T_PRUD_D_870 PrePrudD;  // pre & common
	T_FRSI_D_883 PostFrsiD; // post
	void Initialize()
	{
		strName     = _T("");
		strCaption  = _T("");
		bBW         = FALSE ;
		nStagStatus = 0;
		CurStagK    = 0;
		bPre        = FALSE;
		PrePrudD .Initialize();
		PostFrsiD.Initialize();
	}
	T_UFIG_D_870() { Initialize(); }
	T_UFIG_D_870(const T_UFIG_D_870 &src) { *this = src; }
	T_UFIG_D_870& operator=(const T_UFIG_D_870 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD    = src.PrePrudD;
		PostFrsiD   = src.PostFrsiD;
		return *this;
	}
};
struct T_UFIG_D_883
{
	CString  strName;
	CString  strCaption;  // Caption
	BOOL     bBW;       // black & white
	int      nStagStatus; // 0:base/PostCS, 1:stage, 2:min/max
	T_STAG_K CurStagK; // nStagStatus=1일때 사용
	BOOL     bPre;
	T_PRUD_D_883 PrePrudD;  // pre & common
	T_FRSI_D_883 PostFrsiD; // post
	void Initialize()
	{
		strName     = _T("");
		strCaption  = _T("");
		bBW         = FALSE ;
		nStagStatus = 0;
		CurStagK    = 0;
		bPre        = FALSE;
		PrePrudD .Initialize();
		PostFrsiD.Initialize();
	}
	T_UFIG_D_883() { Initialize(); }
	T_UFIG_D_883(const T_UFIG_D_883 &src) { *this = src; }
	T_UFIG_D_883& operator=(const T_UFIG_D_883 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD    = src.PrePrudD;
		PostFrsiD   = src.PostFrsiD;
		return *this;
	}
};

struct T_UFIG_D_885
{
	CString  strName;
	CString  strCaption;  // Caption
	BOOL     bBW;       // black & white
	int      nStagStatus; // 0:base/PostCS, 1:stage, 2:min/max
	T_STAG_K CurStagK; // nStagStatus=1일때 사용
	BOOL     bPre;
	T_PRUD_D_885 PrePrudD;  // pre & common
	T_FRSI_D PostFrsiD; // post
	void Initialize()
	{
		strName     = _T("");
		strCaption  = _T("");
		bBW         = FALSE ;
		nStagStatus = 0;
		CurStagK    = 0;
		bPre        = FALSE;
		PrePrudD .Initialize();
		PostFrsiD.Initialize();
	}
	T_UFIG_D_885() { Initialize(); }
	T_UFIG_D_885(const T_UFIG_D_885 &src) { *this = src; }
	T_UFIG_D_885& operator=(const T_UFIG_D_885 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD    = src.PrePrudD;
		PostFrsiD   = src.PostFrsiD;
		return *this;
	}
};

struct T_UFIG_D_891
{
	CString  strName;
	CString  strCaption;  // Caption
	BOOL     bBW;       // black & white
	int      nStagStatus; // 0:base/PostCS, 1:stage, 2:min/max
	T_STAG_K CurStagK; // nStagStatus=1일때 사용
	BOOL     bPre;
	T_PRUD_D_891 PrePrudD;  // pre & common
	T_FRSI_D PostFrsiD; // post
	void Initialize()
	{
		strName     = _T("");
		strCaption  = _T("");
		bBW         = FALSE ;
		nStagStatus = 0;
		CurStagK    = 0;
		bPre        = FALSE;
		PrePrudD .Initialize();
		PostFrsiD.Initialize();
	}
	T_UFIG_D_891() { Initialize(); }
	T_UFIG_D_891(const T_UFIG_D_891 &src) { *this = src; }
	T_UFIG_D_891& operator=(const T_UFIG_D_891 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD    = src.PrePrudD;
		PostFrsiD   = src.PostFrsiD;
		return *this;
	}
};
struct T_UFIG_D_895
{
	CString  strName;
	CString  strCaption;  // Caption
	BOOL     bBW;       // black & white
	int      nStagStatus; // 0:base/PostCS, 1:stage, 2:min/max
	T_STAG_K CurStagK; // nStagStatus=1일때 사용
	BOOL     bPre;
	T_PRUD_D_895 PrePrudD;  // pre & common
	T_FRSI_D PostFrsiD; // post
	void Initialize()
	{
		strName = _T("");
		strCaption = _T("");
		bBW = FALSE;
		nStagStatus = 0;
		CurStagK = 0;
		bPre = FALSE;
		PrePrudD.Initialize();
		PostFrsiD.Initialize();
	}
	T_UFIG_D_895() { Initialize(); }
	T_UFIG_D_895(const T_UFIG_D_895 &src) { *this = src; }
	T_UFIG_D_895& operator=(const T_UFIG_D_895 &src)
	{
		strName = src.strName;
		strCaption = src.strCaption;
		bBW = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK = src.CurStagK;
		bPre = src.bPre;
		PrePrudD = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
};
struct T_UFIG_D_925
{
	CString  strName;
	CString  strCaption;  // Caption
	BOOL     bBW;       // black & white
	int      nStagStatus; // 0:base/PostCS, 1:stage, 2:min/max
	T_STAG_K CurStagK; // nStagStatus=1일때 사용
	BOOL     bPre;
	T_PRUD_D_925 PrePrudD;  // pre & common
	T_FRSI_D PostFrsiD; // post
	void Initialize()
	{
		strName = _T("");
		strCaption = _T("");
		bBW = FALSE;
		nStagStatus = 0;
		CurStagK = 0;
		bPre = FALSE;
		PrePrudD.Initialize();
		PostFrsiD.Initialize();
	}
	T_UFIG_D_925() { Initialize(); }
	T_UFIG_D_925(const T_UFIG_D_925 &src) { *this = src; }
	T_UFIG_D_925& operator=(const T_UFIG_D_925 &src)
	{
		strName = src.strName;
		strCaption = src.strCaption;
		bBW = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK = src.CurStagK;
		bPre = src.bPre;
		PrePrudD = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
};

struct T_UFIG_D_930
{
	CString  strName;
	CString  strCaption;  // Caption
	BOOL     bBW;       // black & white
	int      nStagStatus; // 0:base/PostCS, 1:stage, 2:min/max
	T_STAG_K CurStagK; // nStagStatus=1일때 사용
	BOOL     bPre;
	T_PRUD_D_930 PrePrudD;  // pre & common
	T_FRSI_D PostFrsiD; // post
	void Initialize()
	{
		strName = _T("");
		strCaption = _T("");
		bBW = FALSE;
		nStagStatus = 0;
		CurStagK = 0;
		bPre = FALSE;
		PrePrudD.Initialize();
		PostFrsiD.Initialize();
	}
	T_UFIG_D_930() { Initialize(); }
	T_UFIG_D_930(const T_UFIG_D_930 &src) { *this = src; }
	T_UFIG_D_930& operator=(const T_UFIG_D_930 &src)
	{
		strName = src.strName;
		strCaption = src.strCaption;
		bBW = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK = src.CurStagK;
		bPre = src.bPre;
		PrePrudD = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
};

struct T_UFIG_D_940
{
	CString  strName;
	CString  strCaption;  // Caption
	BOOL     bBW;       // black & white
	int      nStagStatus; // 0:base/PostCS, 1:stage, 2:min/max
	T_STAG_K CurStagK; // nStagStatus=1일때 사용
	BOOL     bPre;
	T_PRUD_D_940 PrePrudD;  // pre & common
	T_FRSI_D PostFrsiD; // post
	void Initialize()
	{
		strName = _T("");
		strCaption = _T("");
		bBW = FALSE;
		nStagStatus = 0;
		CurStagK = 0;
		bPre = FALSE;
		PrePrudD.Initialize();
		PostFrsiD.Initialize();
	}
	T_UFIG_D_940() { Initialize(); }
	T_UFIG_D_940(const T_UFIG_D_940 &src) { *this = src; }
	T_UFIG_D_940& operator=(const T_UFIG_D_940 &src)
	{
		strName = src.strName;
		strCaption = src.strCaption;
		bBW = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK = src.CurStagK;
		bPre = src.bPre;
		PrePrudD = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
};

struct T_UFIG_D_950
{
	CString  strName;
	CString  strCaption;  // Caption
	BOOL     bBW;       // black & white
	int      nStagStatus; // 0:base/PostCS, 1:stage, 2:min/max
	T_STAG_K CurStagK; // nStagStatus=1일때 사용
	BOOL     bPre;
	T_PRUD_D_950 PrePrudD;  // pre & common
	T_FRSI_D PostFrsiD; // post
	void Initialize()
	{
		strName = _T("");
		strCaption = _T("");
		bBW = FALSE;
		nStagStatus = 0;
		CurStagK = 0;
		bPre = FALSE;
		PrePrudD.Initialize();
		PostFrsiD.Initialize();
	}
	T_UFIG_D_950() { Initialize(); }
	T_UFIG_D_950(const T_UFIG_D_950& src) { *this = src; }
	T_UFIG_D_950& operator=(const T_UFIG_D_950& src)
	{
		strName = src.strName;
		strCaption = src.strCaption;
		bBW = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK = src.CurStagK;
		bPre = src.bPre;
		PrePrudD = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
};

struct T_UFIG_D
{
	CString  strName;
	CString  strCaption;  // Caption
	BOOL     bBW;       // black & white
	int      nStagStatus; // 0:base/PostCS, 1:stage, 2:min/max
	T_STAG_K CurStagK; // nStagStatus=1일때 사용
	BOOL     bPre;
	T_PRUD_D PrePrudD;  // pre & common
	T_FRSI_D PostFrsiD; // post
	void Initialize()
	{
		strName     = _T("");
		strCaption  = _T("");
		bBW         = FALSE ;
		nStagStatus = 0;
		CurStagK    = 0;
		bPre        = FALSE;
		PrePrudD .Initialize();
		PostFrsiD.Initialize();
	}
	T_UFIG_D() { Initialize(); }
	T_UFIG_D(const T_UFIG_D &src) { *this = src; }
	T_UFIG_D& operator=(const T_UFIG_D &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD    = src.PrePrudD;
		PostFrsiD   = src.PostFrsiD;
		return *this;
	}
	void Convert790(T_UFIG_D_790 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD.Convert790(src.PrePrudD);
		PostFrsiD.Convert821(src.PostFrsiD);
	}
	void Convert821(T_UFIG_D_821 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD.Convert821(src.PrePrudD);
		PostFrsiD.Convert821(src.PostFrsiD);
	}
	void Convert825(T_UFIG_D_825 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD.Convert835(src.PrePrudD);
		PostFrsiD.Convert825(src.PostFrsiD);
	}
	void Convert835(T_UFIG_D_835 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD.Convert835(src.PrePrudD);
		PostFrsiD.Convert851(src.PostFrsiD);
	}
	void Convert841(T_UFIG_D_841 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD.Convert841(src.PrePrudD);
		PostFrsiD.Convert851(src.PostFrsiD);
	}
	void Convert845(T_UFIG_D_845 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD.Convert845(src.PrePrudD);
		PostFrsiD.Convert851(src.PostFrsiD);
	}
	void Convert851(T_UFIG_D_851 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD.Convert855(src.PrePrudD);
		PostFrsiD.Convert851(src.PostFrsiD);
	}
	void Convert855(T_UFIG_D_855 &src)
	{
	  strName     = src.strName;
	  strCaption  = src.strCaption;
	  bBW         = src.bBW;
	  nStagStatus = src.nStagStatus;
	  CurStagK    = src.CurStagK;
	  bPre        = src.bPre;
	  PrePrudD.Convert855(src.PrePrudD);
	  PostFrsiD.Convert865(src.PostFrsiD);
	}
	void Convert855(T_UFIG_D_860 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD.Convert860(src.PrePrudD);
		PostFrsiD.Convert865(src.PostFrsiD);
	}
	void Convert865(T_UFIG_D_865 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD.Convert865(src.PrePrudD);
		PostFrsiD.Convert865(src.PostFrsiD);
	}
	void Convert870(T_UFIG_D_870 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD.Convert870(src.PrePrudD);
		PostFrsiD.Convert883(src.PostFrsiD);
	}
	void Convert883(T_UFIG_D_883 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD.Convert883(src.PrePrudD);
		PostFrsiD.Convert883(src.PostFrsiD);
	} 
	void Convert885(T_UFIG_D_885 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD.Convert885(src.PrePrudD);
		PostFrsiD = src.PostFrsiD;
	} 
	void Convert891(T_UFIG_D_891 &src)
	{
		strName     = src.strName;
		strCaption  = src.strCaption;
		bBW         = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK    = src.CurStagK;
		bPre        = src.bPre;
		PrePrudD.Convert891(src.PrePrudD);
		PostFrsiD = src.PostFrsiD;
	} 
	void Convert895(T_UFIG_D_895 &src)
	{
		strName = src.strName;
		strCaption = src.strCaption;
		bBW = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK = src.CurStagK;
		bPre = src.bPre;
		PrePrudD.Convert895(src.PrePrudD);
		PostFrsiD = src.PostFrsiD;
	}
	void Convert925(T_UFIG_D_925 &src)
	{
		strName = src.strName;
		strCaption = src.strCaption;
		bBW = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK = src.CurStagK;
		bPre = src.bPre;
		PrePrudD.Convert925(src.PrePrudD);
		PostFrsiD = src.PostFrsiD;
	}
	void Convert940(T_UFIG_D_940 &src)
	{
		strName = src.strName;
		strCaption = src.strCaption;
		bBW = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK = src.CurStagK;
		bPre = src.bPre;
		PrePrudD.Convert940(src.PrePrudD);
		PostFrsiD = src.PostFrsiD;
	}
	void Convert950(T_UFIG_D_950& src)
	{
		strName = src.strName;
		strCaption = src.strCaption;
		bBW = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK = src.CurStagK;
		bPre = src.bPre;
		PrePrudD.Convert950(src.PrePrudD);
		PostFrsiD = src.PostFrsiD;
	}
};

struct T_UFIG_D_CH_790
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_790 PrePrudD;
	T_FRSI_D_821  PostFrsiD;
	void Get(T_UFIG_D_790 &rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		rData.PostFrsiD = PostFrsiD;
	}
	void Set(T_UFIG_D_790 Data)
	{
		ConvertStrChar(Data.strName, strName, sizeof(strName));
		ConvertStrChar(Data.strCaption, strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD = Data.PostFrsiD;
	}
	T_UFIG_D_CH_790() {  }
	T_UFIG_D_CH_790(const T_UFIG_D_CH_790 &src) { *this = src; }
	T_UFIG_D_CH_790& operator=(const T_UFIG_D_CH_790 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD  = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
};
struct T_UFIG_D_CH_810
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_810 PrePrudD;
	T_FRSI_D_821  PostFrsiD;
	void Get(T_UFIG_D_810 &rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		rData.PostFrsiD = PostFrsiD;
	}
	void Set(T_UFIG_D_810 Data)
	{
		ConvertStrChar(Data.strName, strName, sizeof(strName));
		ConvertStrChar(Data.strCaption, strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD = Data.PostFrsiD;
	}
	T_UFIG_D_CH_810() {  }
	T_UFIG_D_CH_810(const T_UFIG_D_CH_810 &src) { *this = src; }
	T_UFIG_D_CH_810& operator=(const T_UFIG_D_CH_810 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD  = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
};

struct T_UFIG_D_CH_821
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_821 PrePrudD;
	T_FRSI_D_821  PostFrsiD;
	void Get(T_UFIG_D_821 &rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		rData.PostFrsiD = PostFrsiD;
	}
	void Set(T_UFIG_D_821 Data)
	{
		ConvertStrChar(Data.strName, strName, sizeof(strName));
		ConvertStrChar(Data.strCaption, strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD = Data.PostFrsiD;
	}
	T_UFIG_D_CH_821() {  }
	T_UFIG_D_CH_821(const T_UFIG_D_CH_821 &src) { *this = src; }
	T_UFIG_D_CH_821& operator=(const T_UFIG_D_CH_821 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD  = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
};
struct T_UFIG_D_CH_825
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_835   PrePrudD;
	T_FRSI_D_825  PostFrsiD;
	void Get(T_UFIG_D_825 &rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		rData.PostFrsiD = PostFrsiD;
	}
	void Set(T_UFIG_D_825 Data)
	{
		ConvertStrChar(Data.strName, strName, sizeof(strName));
		ConvertStrChar(Data.strCaption, strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD = Data.PostFrsiD;
	}
	T_UFIG_D_CH_825() {  }
	T_UFIG_D_CH_825(const T_UFIG_D_CH_825 &src) { *this = src; }
	T_UFIG_D_CH_825& operator=(const T_UFIG_D_CH_825 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD  = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
};
struct T_UFIG_D_CH_835
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_835 PrePrudD;
	T_FRSI_D_851  PostFrsiD;
	void Get(T_UFIG_D_835 &rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		rData.PostFrsiD = PostFrsiD;
	}
	void Set(T_UFIG_D_835 Data)
	{
		ConvertStrChar(Data.strName, strName, sizeof(strName));
		ConvertStrChar(Data.strCaption, strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD = Data.PostFrsiD;
	}
	T_UFIG_D_CH_835() {  }
	T_UFIG_D_CH_835(const T_UFIG_D_CH_835 &src) { *this = src; }
	T_UFIG_D_CH_835& operator=(const T_UFIG_D_CH_835 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD  = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
};
struct T_UFIG_D_CH_841
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_841 PrePrudD;
	T_FRSI_D_851  PostFrsiD;
	void Get(T_UFIG_D_841 &rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		rData.PostFrsiD = PostFrsiD;
	}
	void Set(T_UFIG_D_841 Data)
	{
		ConvertStrChar(Data.strName, strName, sizeof(strName));
		ConvertStrChar(Data.strCaption, strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD = Data.PostFrsiD;
	}
	T_UFIG_D_CH_841() {  }
	T_UFIG_D_CH_841(const T_UFIG_D_CH_841 &src) { *this = src; }
	T_UFIG_D_CH_841& operator=(const T_UFIG_D_CH_841 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD  = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
};

struct T_UFIG_D_CH_845
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_845 PrePrudD;
	T_FRSI_D_851  PostFrsiD;
	void Get(T_UFIG_D_845 &rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		rData.PostFrsiD = PostFrsiD;
	}
	void Set(T_UFIG_D_845 Data)
	{
		ConvertStrChar(Data.strName, strName, sizeof(strName));
		ConvertStrChar(Data.strCaption, strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD = Data.PostFrsiD;
	}
	T_UFIG_D_CH_845() {  }
	T_UFIG_D_CH_845(const T_UFIG_D_CH_845 &src) { *this = src; }
	T_UFIG_D_CH_845& operator=(const T_UFIG_D_CH_845 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD  = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
};

struct T_UFIG_D_CH_851
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_855 PrePrudD;
	T_FRSI_D_851  PostFrsiD;
	void Get(T_UFIG_D_851 &rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		rData.PostFrsiD = PostFrsiD;
	}
	void Set(T_UFIG_D_851 Data)
	{
		ConvertStrChar(Data.strName, strName, sizeof(strName));
		ConvertStrChar(Data.strCaption, strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD = Data.PostFrsiD;
	}
	T_UFIG_D_CH_851() {  }
	T_UFIG_D_CH_851(const T_UFIG_D_CH_851 &src) { *this = src; }
	T_UFIG_D_CH_851& operator=(const T_UFIG_D_CH_851 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD  = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
};

struct T_UFIG_D_CH_855
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_855 PrePrudD;
	T_FRSI_D_865  PostFrsiD;
	void Get(T_UFIG_D_855 &rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		rData.PostFrsiD = PostFrsiD;
	}
	void Set(T_UFIG_D_855 Data)
	{
		ConvertStrChar(Data.strName, strName, sizeof(strName));
		ConvertStrChar(Data.strCaption, strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD = Data.PostFrsiD;
	}
	T_UFIG_D_CH_855() {  }
	T_UFIG_D_CH_855(const T_UFIG_D_CH_855 &src) { *this = src; }
	T_UFIG_D_CH_855& operator=(const T_UFIG_D_CH_855 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD  = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
};

struct T_UFIG_D_CH_860
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_860 PrePrudD;
	T_FRSI_D_865  PostFrsiD;
	void Get(T_UFIG_D_860 &rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		rData.PostFrsiD = PostFrsiD;
	}
	void Set(T_UFIG_D_860 Data)
	{
		ConvertStrChar(Data.strName, strName, sizeof(strName));
		ConvertStrChar(Data.strCaption, strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD = Data.PostFrsiD;
	}
	T_UFIG_D_CH_860() {  }
	T_UFIG_D_CH_860(const T_UFIG_D_CH_860 &src) { *this = src; }
	T_UFIG_D_CH_860& operator=(const T_UFIG_D_CH_860 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD  = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
};

struct T_UFIG_D_CH_865
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_865 PrePrudD;
	T_FRSI_D_865  PostFrsiD;
	void Get(T_UFIG_D_865 &rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		rData.PostFrsiD = PostFrsiD;
	}
	void Set(T_UFIG_D_865 Data)
	{
		ConvertStrChar(Data.strName, strName, sizeof(strName));
		ConvertStrChar(Data.strCaption, strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD = Data.PostFrsiD;
	}
	T_UFIG_D_CH_865() {  }
	T_UFIG_D_CH_865(const T_UFIG_D_CH_865 &src) { *this = src; }
	T_UFIG_D_CH_865& operator=(const T_UFIG_D_CH_865 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD  = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
};
struct T_UFIG_D_CH_870
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_870 PrePrudD;
	T_FRSI_D_883    PostFrsiD;
	void Get(T_UFIG_D_870 &rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		rData.PostFrsiD = PostFrsiD;
	}
	void Set(T_UFIG_D_870 Data)
	{
		ConvertStrChar(Data.strName, strName, sizeof(strName));
		ConvertStrChar(Data.strCaption, strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD = Data.PostFrsiD;
	}
	T_UFIG_D_CH_870() {  }
	T_UFIG_D_CH_870(const T_UFIG_D_CH_870 &src) { *this = src; }
	T_UFIG_D_CH_870& operator=(const T_UFIG_D_CH_870 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD  = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
};
struct T_UFIG_D_CH_883
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_883 PrePrudD;
	T_FRSI_D_883    PostFrsiD;
	void Get(T_UFIG_D_883 &rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		rData.PostFrsiD = PostFrsiD;
	}
	void Set(T_UFIG_D_883 Data)
	{
		ConvertStrChar(Data.strName, strName, sizeof(strName));
		ConvertStrChar(Data.strCaption, strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD = Data.PostFrsiD;
	}
	T_UFIG_D_CH_883() {  }
	T_UFIG_D_CH_883(const T_UFIG_D_CH_883 &src) { *this = src; }
	T_UFIG_D_CH_883& operator=(const T_UFIG_D_CH_883 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD  = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
};
struct T_UFIG_D_CH_885
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_885 PrePrudD;
	T_FRSI_D  PostFrsiD;
	void Get(T_UFIG_D_885 &rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		rData.PostFrsiD = PostFrsiD;
	}
	void Set(T_UFIG_D_885 Data)
	{
		ConvertStrChar(Data.strName, strName, sizeof(strName));
		ConvertStrChar(Data.strCaption, strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD = Data.PostFrsiD;
	}
	T_UFIG_D_CH_885() {  }
	T_UFIG_D_CH_885(const T_UFIG_D_CH_885 &src) { *this = src; }
	T_UFIG_D_CH_885& operator=(const T_UFIG_D_CH_885 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD  = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
};
struct T_UFIG_D_CH_891
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_891 PrePrudD;
	T_FRSI_D  PostFrsiD;
	void Get(T_UFIG_D_891 &rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		rData.PostFrsiD = PostFrsiD;
	}
	void Set(T_UFIG_D_891 Data)
	{
		ConvertStrChar(Data.strName, strName, sizeof(strName));
		ConvertStrChar(Data.strCaption, strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD = Data.PostFrsiD;
	}
	T_UFIG_D_CH_891() {  }
	T_UFIG_D_CH_891(const T_UFIG_D_CH_891 &src) { *this = src; }
	T_UFIG_D_CH_891& operator=(const T_UFIG_D_CH_891 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD  = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
};
struct T_UFIG_D_CH_895
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_895 PrePrudD;
	T_FRSI_D  PostFrsiD;
	void Get(T_UFIG_D_895 &rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		rData.bBW = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK = CurStagK;
		rData.bPre = bPre;
		PrePrudD.Get(rData.PrePrudD);
		rData.PostFrsiD = PostFrsiD;
	}
	void Set(T_UFIG_D_895 Data)
	{
		ConvertStrChar(Data.strName, strName, sizeof(strName));
		ConvertStrChar(Data.strCaption, strCaption, sizeof(strCaption));
		bBW = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK = Data.CurStagK;
		bPre = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD = Data.PostFrsiD;
	}
	T_UFIG_D_CH_895() {  }
	T_UFIG_D_CH_895(const T_UFIG_D_CH_895 &src) { *this = src; }
	T_UFIG_D_CH_895& operator=(const T_UFIG_D_CH_895 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK = src.CurStagK;
		bPre = src.bPre;
		PrePrudD = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
};
struct T_UFIG_D_CH_925
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_925 PrePrudD;
	T_FRSI_D  PostFrsiD;
	void Get(T_UFIG_D_925 &rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		rData.bBW = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK = CurStagK;
		rData.bPre = bPre;
		PrePrudD.Get(rData.PrePrudD);
		rData.PostFrsiD = PostFrsiD;
	}
	void Set(T_UFIG_D_925 Data)
	{
		ConvertStrChar(Data.strName, strName, sizeof(strName));
		ConvertStrChar(Data.strCaption, strCaption, sizeof(strCaption));
		bBW = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK = Data.CurStagK;
		bPre = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD = Data.PostFrsiD;
	}
	T_UFIG_D_CH_925() {  }
	T_UFIG_D_CH_925(const T_UFIG_D_CH_925 &src) { *this = src; }
	T_UFIG_D_CH_925& operator=(const T_UFIG_D_CH_925 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK = src.CurStagK;
		bPre = src.bPre;
		PrePrudD = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
};
struct T_UFIG_D_CH_930
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_930 PrePrudD;
	T_FRSI_D  PostFrsiD;
	void Get(T_UFIG_D_930 &rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		rData.bBW = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK = CurStagK;
		rData.bPre = bPre;
		PrePrudD.Get(rData.PrePrudD);
		rData.PostFrsiD = PostFrsiD;
	}
	void Set(T_UFIG_D_930 Data)
	{
		ConvertStrChar(Data.strName, strName, sizeof(strName));
		ConvertStrChar(Data.strCaption, strCaption, sizeof(strCaption));
		bBW = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK = Data.CurStagK;
		bPre = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD = Data.PostFrsiD;
	}
	T_UFIG_D_CH_930() {  }
	T_UFIG_D_CH_930(const T_UFIG_D_CH_930 &src) { *this = src; }
	T_UFIG_D_CH_930& operator=(const T_UFIG_D_CH_930 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK = src.CurStagK;
		bPre = src.bPre;
		PrePrudD = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
};

struct T_UFIG_D_CH_940
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_940 PrePrudD;
	T_FRSI_D  PostFrsiD;
	void Get(T_UFIG_D_940 &rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		rData.bBW = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK = CurStagK;
		rData.bPre = bPre;
		PrePrudD.Get(rData.PrePrudD);
		rData.PostFrsiD = PostFrsiD;
	}
	void Set(T_UFIG_D_940 Data)
	{
		ConvertStrChar(Data.strName, strName, sizeof(strName));
		ConvertStrChar(Data.strCaption, strCaption, sizeof(strCaption));
		bBW = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK = Data.CurStagK;
		bPre = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD = Data.PostFrsiD;
	}
	T_UFIG_D_CH_940() {  }
	T_UFIG_D_CH_940(const T_UFIG_D_CH_940 &src) { *this = src; }
	T_UFIG_D_CH_940& operator=(const T_UFIG_D_CH_940 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK = src.CurStagK;
		bPre = src.bPre;
		PrePrudD = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
};

struct T_UFIG_D_CH_950
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_950 PrePrudD;
	T_FRSI_D  PostFrsiD;
	void Get(T_UFIG_D_950& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		rData.bBW = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK = CurStagK;
		rData.bPre = bPre;
		PrePrudD.Get(rData.PrePrudD);
		rData.PostFrsiD = PostFrsiD;
	}
	void Set(T_UFIG_D_950 Data)
	{
		ConvertStrChar(Data.strName, strName, sizeof(strName));
		ConvertStrChar(Data.strCaption, strCaption, sizeof(strCaption));
		bBW = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK = Data.CurStagK;
		bPre = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD = Data.PostFrsiD;
	}
	T_UFIG_D_CH_950() {  }
	T_UFIG_D_CH_950(const T_UFIG_D_CH_950& src) { *this = src; }
	T_UFIG_D_CH_950& operator=(const T_UFIG_D_CH_950& src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK = src.CurStagK;
		bPre = src.bPre;
		PrePrudD = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
};

struct T_UFIG_D_CH
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH PrePrudD;
	T_FRSI_D  PostFrsiD;
	void Get(T_UFIG_D &rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		rData.PostFrsiD = PostFrsiD;
	}
	void Set(T_UFIG_D Data)
	{
		ConvertStrChar(Data.strName, strName, sizeof(strName));
		ConvertStrChar(Data.strCaption, strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD = Data.PostFrsiD;
	}
	T_UFIG_D_CH() {  }
	T_UFIG_D_CH(const T_UFIG_D_CH &src) { *this = src; }
	T_UFIG_D_CH& operator=(const T_UFIG_D_CH &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD  = src.PrePrudD;
		PostFrsiD = src.PostFrsiD;
		return *this;
	}
	void Convert790(T_UFIG_D_CH_790 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD.Convert790(src.PrePrudD);
		PostFrsiD.Convert821(src.PostFrsiD);
	}
	void Convert810(T_UFIG_D_CH_810 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD.Convert810(src.PrePrudD);
		PostFrsiD.Convert821(src.PostFrsiD);
	}
	void Convert821(T_UFIG_D_CH_821 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD.Convert821(src.PrePrudD);
		PostFrsiD.Convert821(src.PostFrsiD);
	}
	void Convert825(T_UFIG_D_CH_825 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD.Convert835(src.PrePrudD);
		PostFrsiD.Convert825(src.PostFrsiD);
	}
	void Convert835(T_UFIG_D_CH_835 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD.Convert835(src.PrePrudD);
		PostFrsiD.Convert851(src.PostFrsiD);
	}
	void Convert841(T_UFIG_D_CH_841 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD.Convert841(src.PrePrudD);
		PostFrsiD.Convert851(src.PostFrsiD);
	}
	void Convert845(T_UFIG_D_CH_845 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD.Convert845(src.PrePrudD);
		PostFrsiD.Convert851(src.PostFrsiD);
	}
	void Convert851(T_UFIG_D_CH_851 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD.Convert855(src.PrePrudD);
		PostFrsiD.Convert851(src.PostFrsiD);
	}
	void Convert855(T_UFIG_D_CH_855 &src)
	{
	  memcpy(strName, src.strName, sizeof(strName));
	  memcpy(strCaption, src.strCaption, sizeof(strCaption));
	  bBW       = src.bBW;
	  nStagStatus = src.nStagStatus;
	  CurStagK  = src.CurStagK;
	  bPre      = src.bPre;
	  PrePrudD.Convert855(src.PrePrudD);
	  PostFrsiD.Convert865(src.PostFrsiD);
	}
	void Convert855(T_UFIG_D_CH_860 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD.Convert860(src.PrePrudD);
		PostFrsiD.Convert865(src.PostFrsiD);
	}
	void Convert865(T_UFIG_D_CH_865 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD.Convert865(src.PrePrudD);
		PostFrsiD.Convert865(src.PostFrsiD);
	}
	void Convert870(T_UFIG_D_CH_870 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD.Convert870(src.PrePrudD);
		PostFrsiD.Convert883(src.PostFrsiD);
	}
	void Convert883(T_UFIG_D_CH_883 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD.Convert883(src.PrePrudD);
		PostFrsiD.Convert883(src.PostFrsiD);
	}
	void Convert885(T_UFIG_D_CH_885 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD.Convert885(src.PrePrudD);
		PostFrsiD = src.PostFrsiD;
	}
	void Convert891(T_UFIG_D_CH_891 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW       = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK  = src.CurStagK;
		bPre      = src.bPre;
		PrePrudD.Convert891(src.PrePrudD);
		PostFrsiD = src.PostFrsiD;
	}
	void Convert895(T_UFIG_D_CH_895 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK = src.CurStagK;
		bPre = src.bPre;
		PrePrudD.Convert895(src.PrePrudD);
		PostFrsiD = src.PostFrsiD;
	}
	void Convert925(T_UFIG_D_CH_925 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK = src.CurStagK;
		bPre = src.bPre;
		PrePrudD.Convert925(src.PrePrudD);
		PostFrsiD = src.PostFrsiD;
	}
	void Convert940(T_UFIG_D_CH_940 &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK = src.CurStagK;
		bPre = src.bPre;
		PrePrudD.Convert940(src.PrePrudD);
		PostFrsiD = src.PostFrsiD;
	}
	void Convert950(T_UFIG_D_CH_950& src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		bBW = src.bBW;
		nStagStatus = src.nStagStatus;
		CurStagK = src.CurStagK;
		bPre = src.bPre;
		PrePrudD.Convert950(src.PrePrudD);
		PostFrsiD = src.PostFrsiD;
	}
};

struct T_UFIG_D_CH_RW_790
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_RW_790 PrePrudD;
	T_FRSI_D_RW_821  PostFrsiD;
	void Get(T_UFIG_D_CH_790 &rData)
	{
		memcpy(rData.strName, strName, sizeof(rData.strName));
		memcpy(rData.strCaption, strCaption, sizeof(rData.strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		PostFrsiD.Get(rData.PostFrsiD);
	}
	void Set(T_UFIG_D_CH_790 Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD.Set(Data.PostFrsiD);
	}
};
struct T_UFIG_D_CH_RW_810
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_RW_810 PrePrudD;
	T_FRSI_D_RW_821  PostFrsiD;
	void Get(T_UFIG_D_CH_810 &rData)
	{
		memcpy(rData.strName, strName, sizeof(rData.strName));
		memcpy(rData.strCaption, strCaption, sizeof(rData.strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		PostFrsiD.Get(rData.PostFrsiD);
	}
	void Set(T_UFIG_D_CH_810 Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD.Set(Data.PostFrsiD);
	}
};
struct T_UFIG_D_CH_RW_821
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_RW_821 PrePrudD;
	T_FRSI_D_RW_821  PostFrsiD;
	void Get(T_UFIG_D_CH_821 &rData)
	{
		memcpy(rData.strName, strName, sizeof(rData.strName));
		memcpy(rData.strCaption, strCaption, sizeof(rData.strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		PostFrsiD.Get(rData.PostFrsiD);
	}
	void Set(T_UFIG_D_CH_821 Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD.Set(Data.PostFrsiD);
	}
};
struct T_UFIG_D_CH_RW_825
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_RW_835   PrePrudD;
	T_FRSI_D_RW_825  PostFrsiD;
	void Get(T_UFIG_D_CH_825 &rData)
	{
		memcpy(rData.strName, strName, sizeof(rData.strName));
		memcpy(rData.strCaption, strCaption, sizeof(rData.strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		PostFrsiD.Get(rData.PostFrsiD);
	}
	void Set(T_UFIG_D_CH_825 Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD.Set(Data.PostFrsiD);
	}
};
struct T_UFIG_D_CH_RW_835
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_RW_835 PrePrudD;
	T_FRSI_D_RW_851  PostFrsiD;
	void Get(T_UFIG_D_CH_835 &rData)
	{
		memcpy(rData.strName, strName, sizeof(rData.strName));
		memcpy(rData.strCaption, strCaption, sizeof(rData.strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		PostFrsiD.Get(rData.PostFrsiD);
	}
	void Set(T_UFIG_D_CH_835 Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD.Set(Data.PostFrsiD);
	}
};
struct T_UFIG_D_CH_RW_841
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_RW_841 PrePrudD;
	T_FRSI_D_RW_851  PostFrsiD;
	//void Get(T_UFIG_D_CH_841 &rData)
	//{
	//	memcpy(rData.strName, strName, sizeof(rData.strName));
	//	memcpy(rData.strCaption, strCaption, sizeof(rData.strCaption));
	//	rData.bBW         = bBW;
	//	rData.nStagStatus = nStagStatus;
	//	rData.CurStagK    = CurStagK;
	//	rData.bPre        = bPre;
	//	PrePrudD.Get(rData.PrePrudD);
	//	PostFrsiD.Get(rData.PostFrsiD);
	//}
	//void Set(T_UFIG_D_CH_841 Data)
	//{
	//	memcpy(strName, Data.strName, sizeof(strName));
	//	memcpy(strCaption, Data.strCaption, sizeof(strCaption));
	//	bBW         = Data.bBW;
	//	nStagStatus = Data.nStagStatus;
	//	CurStagK    = Data.CurStagK;
	//	bPre        = Data.bPre;
	//	PrePrudD.Set(Data.PrePrudD);
	//	PostFrsiD.Set(Data.PostFrsiD);
	//}
};

struct T_UFIG_D_CH_RW_845
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_RW_845 PrePrudD;
	T_FRSI_D_RW_851  PostFrsiD;
	/*void Get(T_UFIG_D_CH_845 &rData)
	{
		memcpy(rData.strName, strName, sizeof(rData.strName));
		memcpy(rData.strCaption, strCaption, sizeof(rData.strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		PostFrsiD.Get(rData.PostFrsiD);
	}
	void Set(T_UFIG_D_CH_845 Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD.Set(Data.PostFrsiD);
	}*/
};
struct T_UFIG_D_CH_RW_851
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_RW_855  PrePrudD;
	T_FRSI_D_RW_851 PostFrsiD;
	/*void Get(T_UFIG_D_CH_851 &rData)
	{
		memcpy(rData.strName, strName, sizeof(rData.strName));
		memcpy(rData.strCaption, strCaption, sizeof(rData.strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		PostFrsiD.Get(rData.PostFrsiD);
	}
	void Set(T_UFIG_D_CH_851 Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD.Set(Data.PostFrsiD);
	}*/
};
struct T_UFIG_D_CH_RW_855
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_RW_855  PrePrudD;
	T_FRSI_D_RW_865 PostFrsiD;
	/*void Get(T_UFIG_D_CH_855 &rData)
	{
		memcpy(rData.strName, strName, sizeof(rData.strName));
		memcpy(rData.strCaption, strCaption, sizeof(rData.strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		PostFrsiD.Get(rData.PostFrsiD);
	}
	void Set(T_UFIG_D_CH_855 Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD.Set(Data.PostFrsiD);
	}*/
};

struct T_UFIG_D_CH_RW_860
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_RW_860  PrePrudD;
	T_FRSI_D_RW_865 PostFrsiD;
	/*void Get(T_UFIG_D_CH_860 &rData)
	{
		memcpy(rData.strName, strName, sizeof(rData.strName));
		memcpy(rData.strCaption, strCaption, sizeof(rData.strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		PostFrsiD.Get(rData.PostFrsiD);
	}
	void Set(T_UFIG_D_CH_860 Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD.Set(Data.PostFrsiD);
	}*/
};

struct T_UFIG_D_CH_RW_865
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_RW_865 PrePrudD;
	T_FRSI_D_RW_865 PostFrsiD;
	/*void Get(T_UFIG_D_CH_865 &rData)
	{
		memcpy(rData.strName, strName, sizeof(rData.strName));
		memcpy(rData.strCaption, strCaption, sizeof(rData.strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		PostFrsiD.Get(rData.PostFrsiD);
	}
	void Set(T_UFIG_D_CH_865 Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD.Set(Data.PostFrsiD);
	}*/
};
struct T_UFIG_D_CH_RW_870
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_RW_870 PrePrudD;
	T_FRSI_D_RW_883    PostFrsiD;
	/*void Get(T_UFIG_D_CH_870 &rData)
	{
		memcpy(rData.strName, strName, sizeof(rData.strName));
		memcpy(rData.strCaption, strCaption, sizeof(rData.strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		PostFrsiD.Get(rData.PostFrsiD);
	}
	void Set(T_UFIG_D_CH_870 Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD.Set(Data.PostFrsiD);
	}*/
};

struct T_UFIG_D_CH_RW_883
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_RW_883 PrePrudD;
	T_FRSI_D_RW_883    PostFrsiD;
	/*void Get(T_UFIG_D_CH_883 &rData)
	{
		memcpy(rData.strName, strName, sizeof(rData.strName));
		memcpy(rData.strCaption, strCaption, sizeof(rData.strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		PostFrsiD.Get(rData.PostFrsiD);
	}
	void Set(T_UFIG_D_CH_883 Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD.Set(Data.PostFrsiD);
	}*/
};

struct T_UFIG_D_CH_RW_885
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_RW_885 PrePrudD;
	T_FRSI_D_RW_940  PostFrsiD;
// 	void Get(T_UFIG_D_CH_885 &rData)
// 	{
// 		memcpy(rData.strName, strName, sizeof(rData.strName));
// 		memcpy(rData.strCaption, strCaption, sizeof(rData.strCaption));
// 		rData.bBW         = bBW;
// 		rData.nStagStatus = nStagStatus;
// 		rData.CurStagK    = CurStagK;
// 		rData.bPre        = bPre;
// 		PrePrudD.Get(rData.PrePrudD);
// 		PostFrsiD.Get(rData.PostFrsiD);
// 	}
// 	void Set(T_UFIG_D_CH_885 Data)
// 	{
// 		memcpy(strName, Data.strName, sizeof(strName));
// 		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
// 		bBW         = Data.bBW;
// 		nStagStatus = Data.nStagStatus;
// 		CurStagK    = Data.CurStagK;
// 		bPre        = Data.bPre;
// 		PrePrudD.Set(Data.PrePrudD);
// 		PostFrsiD.Set(Data.PostFrsiD);
// 	}
};

struct T_UFIG_D_CH_RW_891
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_RW_891 PrePrudD;
	T_FRSI_D_RW_940  PostFrsiD;
// 	void Get(T_UFIG_D_CH_891 &rData)
// 	{
// 		memcpy(rData.strName, strName, sizeof(rData.strName));
// 		memcpy(rData.strCaption, strCaption, sizeof(rData.strCaption));
// 		rData.bBW         = bBW;
// 		rData.nStagStatus = nStagStatus;
// 		rData.CurStagK    = CurStagK;
// 		rData.bPre        = bPre;
// 		PrePrudD.Get(rData.PrePrudD);
// 		PostFrsiD.Get(rData.PostFrsiD);
// 	}
// 	void Set(T_UFIG_D_CH_891 Data)
// 	{
// 		memcpy(strName, Data.strName, sizeof(strName));
// 		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
// 		bBW         = Data.bBW;
// 		nStagStatus = Data.nStagStatus;
// 		CurStagK    = Data.CurStagK;
// 		bPre        = Data.bPre;
// 		PrePrudD.Set(Data.PrePrudD);
// 		PostFrsiD.Set(Data.PostFrsiD);
// 	}
};
struct T_UFIG_D_CH_RW_895
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_RW_895 PrePrudD;
	T_FRSI_D_RW_940  PostFrsiD;
// 	void Get(T_UFIG_D_CH_895 &rData)
// 	{
// 		memcpy(rData.strName, strName, sizeof(rData.strName));
// 		memcpy(rData.strCaption, strCaption, sizeof(rData.strCaption));
// 		rData.bBW = bBW;
// 		rData.nStagStatus = nStagStatus;
// 		rData.CurStagK = CurStagK;
// 		rData.bPre = bPre;
// 		PrePrudD.Get(rData.PrePrudD);
// 		PostFrsiD.Get(rData.PostFrsiD);
// 	}
// 	void Set(T_UFIG_D_CH_895 Data)
// 	{
// 		memcpy(strName, Data.strName, sizeof(strName));
// 		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
// 		bBW = Data.bBW;
// 		nStagStatus = Data.nStagStatus;
// 		CurStagK = Data.CurStagK;
// 		bPre = Data.bPre;
// 		PrePrudD.Set(Data.PrePrudD);
// 		PostFrsiD.Set(Data.PostFrsiD);
// 	}
};

struct T_UFIG_D_CH_RW_925
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_RW_925 PrePrudD;
	T_FRSI_D_RW_940  PostFrsiD;
// 	void Get(T_UFIG_D_CH_925 &rData)
// 	{
// 		memcpy(rData.strName, strName, sizeof(rData.strName));
// 		memcpy(rData.strCaption, strCaption, sizeof(rData.strCaption));
// 		rData.bBW = bBW;
// 		rData.nStagStatus = nStagStatus;
// 		rData.CurStagK = CurStagK;
// 		rData.bPre = bPre;
// 		PrePrudD.Get(rData.PrePrudD);
// 		PostFrsiD.Get(rData.PostFrsiD);
// 	}
// 	void Set(T_UFIG_D_CH_925 Data)
// 	{
// 		memcpy(strName, Data.strName, sizeof(strName));
// 		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
// 		bBW = Data.bBW;
// 		nStagStatus = Data.nStagStatus;
// 		CurStagK = Data.CurStagK;
// 		bPre = Data.bPre;
// 		PrePrudD.Set(Data.PrePrudD);
// 		PostFrsiD.Set(Data.PostFrsiD);
// 	}
};

struct T_UFIG_D_CH_RW_930
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_RW_930 PrePrudD;
	T_FRSI_D_RW_940  PostFrsiD;

// 	void Get(T_UFIG_D_CH_930 &rData)
// 	{
// 		memcpy(rData.strName, strName, sizeof(rData.strName));
// 		memcpy(rData.strCaption, strCaption, sizeof(rData.strCaption));
// 		rData.bBW = bBW;
// 		rData.nStagStatus = nStagStatus;
// 		rData.CurStagK = CurStagK;
// 		rData.bPre = bPre;
// 		PrePrudD.Get(rData.PrePrudD);
// 		PostFrsiD.Get(rData.PostFrsiD);
// 	}
// 	void Set(T_UFIG_D_CH_930 Data)
// 	{
// 		memcpy(strName, Data.strName, sizeof(strName));
// 		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
// 		bBW = Data.bBW;
// 		nStagStatus = Data.nStagStatus;
// 		CurStagK = Data.CurStagK;
// 		bPre = Data.bPre;
// 		PrePrudD.Set(Data.PrePrudD);
// 		PostFrsiD.Set(Data.PostFrsiD);
// 	}
};

struct T_UFIG_D_CH_RW_940
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_RW_940 PrePrudD;
	T_FRSI_D_RW_940  PostFrsiD;
};

struct T_UFIG_D_CH_RW_950
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_RW_950 PrePrudD;
	T_FRSI_D_RW  PostFrsiD;
};

struct T_UFIG_D_CH_RW_950_2
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_RW_950_2 PrePrudD;
	T_FRSI_D_RW  PostFrsiD;
};

struct T_UFIG_D_CH_RW_955
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_RW_955 PrePrudD;
	T_FRSI_D_RW  PostFrsiD;
};

struct T_UFIG_D_CH_RW_965
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_RW_965 PrePrudD;
	T_FRSI_D_RW  PostFrsiD;
};

struct T_UFIG_D_CH_RW
{
	char      strName[60];
	char      strCaption[60];
	BOOL      bBW;
	int       nStagStatus;
	T_STAG_K  CurStagK;
	BOOL      bPre;
	T_PRUD_D_CH_RW PrePrudD;
	T_FRSI_D_RW  PostFrsiD;
	void Get(T_UFIG_D_CH &rData)
	{
		memcpy(rData.strName, strName, sizeof(rData.strName));
		memcpy(rData.strCaption, strCaption, sizeof(rData.strCaption));
		rData.bBW         = bBW;
		rData.nStagStatus = nStagStatus;
		rData.CurStagK    = CurStagK;
		rData.bPre        = bPre;
		PrePrudD.Get(rData.PrePrudD);
		PostFrsiD.Get(rData.PostFrsiD);
	}
	void Set(T_UFIG_D_CH Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Set(Data.PrePrudD);
		PostFrsiD.Set(Data.PostFrsiD);
	}
	void Convert790(T_UFIG_D_CH_RW_790& Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Convert790(Data.PrePrudD);
		PostFrsiD.Convert821(Data.PostFrsiD);
	}
	void Convert810(T_UFIG_D_CH_RW_810& Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Convert810(Data.PrePrudD);
		PostFrsiD.Convert821(Data.PostFrsiD);
	}
	void Convert821(T_UFIG_D_CH_RW_821& Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Convert821(Data.PrePrudD);
		PostFrsiD.Convert821(Data.PostFrsiD);
	}
	void Convert825(T_UFIG_D_CH_RW_825& Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Convert835(Data.PrePrudD);
		PostFrsiD.Convert825(Data.PostFrsiD);
	}
	void Convert835(T_UFIG_D_CH_RW_835& Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Convert835(Data.PrePrudD);
		PostFrsiD.Convert851(Data.PostFrsiD);
	}
	void Convert841(T_UFIG_D_CH_RW_841& Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Convert841(Data.PrePrudD);
		PostFrsiD.Convert851(Data.PostFrsiD);
	}
	void Convert845(T_UFIG_D_CH_RW_845& Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Convert845(Data.PrePrudD);
		PostFrsiD.Convert851(Data.PostFrsiD);
	}
	void Convert851(T_UFIG_D_CH_RW_851& Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Convert855(Data.PrePrudD);
		PostFrsiD.Convert851(Data.PostFrsiD);
	}
	void Convert855(T_UFIG_D_CH_RW_855& Data)
	{
	  memcpy(strName, Data.strName, sizeof(strName));
	  memcpy(strCaption, Data.strCaption, sizeof(strCaption));
	  bBW         = Data.bBW;
	  nStagStatus = Data.nStagStatus;
	  CurStagK    = Data.CurStagK;
	  bPre        = Data.bPre;
	  PrePrudD.Convert855(Data.PrePrudD);
	  PostFrsiD.Convert865(Data.PostFrsiD);
	}
	void Convert860(T_UFIG_D_CH_RW_860& Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Convert860(Data.PrePrudD);
		PostFrsiD.Convert865(Data.PostFrsiD);
	}
	void Convert865(T_UFIG_D_CH_RW_865& Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Convert865(Data.PrePrudD);
		PostFrsiD.Convert865(Data.PostFrsiD);
	}
	void Convert870(T_UFIG_D_CH_RW_870& Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Convert870(Data.PrePrudD);
		PostFrsiD.Convert883(Data.PostFrsiD);
	}
	void Convert883(T_UFIG_D_CH_RW_883& Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Convert883(Data.PrePrudD);
		PostFrsiD.Convert883(Data.PostFrsiD);
	}
	void Convert885(T_UFIG_D_CH_RW_885& Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK = Data.CurStagK;
		bPre = Data.bPre;
		PrePrudD.Convert885(Data.PrePrudD);
		PostFrsiD.Convert940(Data.PostFrsiD);
	}
	void Convert891(T_UFIG_D_CH_RW_891& Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW         = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK    = Data.CurStagK;
		bPre        = Data.bPre;
		PrePrudD.Convert891(Data.PrePrudD);
		PostFrsiD.Convert940(Data.PostFrsiD);
	}
	void Convert895(T_UFIG_D_CH_RW_895& Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK = Data.CurStagK;
		bPre = Data.bPre;
		PrePrudD.Convert895(Data.PrePrudD);
		PostFrsiD.Convert940(Data.PostFrsiD);
	}
	void Convert925(T_UFIG_D_CH_RW_925& Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK = Data.CurStagK;
		bPre = Data.bPre;
		PrePrudD.Convert925(Data.PrePrudD);
		PostFrsiD.Convert940(Data.PostFrsiD);
	}
	void Convert930(T_UFIG_D_CH_RW_930& Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK = Data.CurStagK;
		bPre = Data.bPre;
		PrePrudD.Convert930(Data.PrePrudD);
		PostFrsiD.Convert940(Data.PostFrsiD);
	}
	void Convert940(T_UFIG_D_CH_RW_940& Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK = Data.CurStagK;
		bPre = Data.bPre;
		PrePrudD.Convert940(Data.PrePrudD);
		PostFrsiD.Convert940(Data.PostFrsiD);
	}
	void Convert950(T_UFIG_D_CH_RW_950& Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK = Data.CurStagK;
		bPre = Data.bPre;
		PrePrudD.Convert950(Data.PrePrudD);
		PostFrsiD = Data.PostFrsiD;
	}
	void Convert950_2(T_UFIG_D_CH_RW_950_2& Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK = Data.CurStagK;
		bPre = Data.bPre;
		PrePrudD.Convert950_2(Data.PrePrudD);
		PostFrsiD = Data.PostFrsiD;
	}
	void Convert955(T_UFIG_D_CH_RW_955& Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK = Data.CurStagK;
		bPre = Data.bPre;
		PrePrudD.Convert955(Data.PrePrudD);
		PostFrsiD = Data.PostFrsiD;
	}
	void Convert965(T_UFIG_D_CH_RW_965& Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		bBW = Data.bBW;
		nStagStatus = Data.nStagStatus;
		CurStagK = Data.CurStagK;
		bPre = Data.bPre;
		PrePrudD.Convert965(Data.PrePrudD);
		PostFrsiD = Data.PostFrsiD;
	}
};

//struct T_UFIG_UDRD_D_790
//{
//  T_UFIG_K key ;
//  T_UFIG_D_790 data;
//  void Initialize()
//  {
//    key  = 0;
//    data .Initialize();
//  }
//  T_UFIG_UDRD_D_790() { Initialize(); }
//  T_UFIG_UDRD_D_790(const T_UFIG_UDRD_D_790 &src) { *this = src; }
//  T_UFIG_UDRD_D_790& operator=(const T_UFIG_UDRD_D_790 &src)
//  {
//    key  = src.key ;
//    data = src.data;
//    return *this;
//  }
//};
//struct T_UFIG_UDRD_D_810
//{
//  T_UFIG_K key ;
//  T_UFIG_D_810 data;
//  void Initialize()
//  {
//    key  = 0;
//    data .Initialize();
//  }
//  T_UFIG_UDRD_D_810() { Initialize(); }
//  T_UFIG_UDRD_D_810(const T_UFIG_UDRD_D_810 &src) { *this = src; }
//  T_UFIG_UDRD_D_810& operator=(const T_UFIG_UDRD_D_810 &src)
//  {
//    key  = src.key ;
//    data = src.data;
//    return *this;
//  }
//};
//struct T_UFIG_UDRD_D_821
//{
//  T_UFIG_K key ;
//  T_UFIG_D_821 data;
//  void Initialize()
//  {
//    key  = 0;
//    data .Initialize();
//  }
//  T_UFIG_UDRD_D_821() { Initialize(); }
//  T_UFIG_UDRD_D_821(const T_UFIG_UDRD_D_821 &src) { *this = src; }
//  T_UFIG_UDRD_D_821& operator=(const T_UFIG_UDRD_D_821 &src)
//  {
//    key  = src.key ;
//    data = src.data;
//    return *this;
//  }
//};
struct T_UFIG_UDRD_D
{
	T_UFIG_K key ;
	T_UFIG_D data;
	void Initialize()
	{
		key  = 0;
		data .Initialize();
	}
	T_UFIG_UDRD_D() { Initialize(); }
	T_UFIG_UDRD_D(const T_UFIG_UDRD_D &src) { *this = src; }
	T_UFIG_UDRD_D& operator=(const T_UFIG_UDRD_D &src)
	{
		key  = src.key ;
		data = src.data;
		return *this;
	}
//   void Convert790(T_UFIG_UDRD_D_790 &src)
//   {
//     key  = src.key ;
//     data = src.data;
//   }
};
struct T_UFIG_UDRD_D_CH_790
{
	T_UFIG_K key;
	T_UFIG_D_CH_790 data;
};
struct T_UFIG_UDRD_D_CH_810
{
	T_UFIG_K key;
	T_UFIG_D_CH_810 data;
};
struct T_UFIG_UDRD_D_CH_821
{
	T_UFIG_K key;
	T_UFIG_D_CH_821 data;
};
struct T_UFIG_UDRD_D_CH_825
{
	T_UFIG_K key;
	T_UFIG_D_CH_825 data;
};
struct T_UFIG_UDRD_D_CH_835
{
	T_UFIG_K key;
	T_UFIG_D_CH_835 data;
};
struct T_UFIG_UDRD_D_CH_841
{
	T_UFIG_K key;
	T_UFIG_D_CH_841 data;
};
struct T_UFIG_UDRD_D_CH_845
{
	T_UFIG_K key;
	T_UFIG_D_CH_845 data;
};

struct T_UFIG_UDRD_D_CH_855
{
	T_UFIG_K key;
	T_UFIG_D_CH_855 data;
};

struct T_UFIG_UDRD_D_CH_860
{
	T_UFIG_K key;
	T_UFIG_D_CH_860 data;
};

struct T_UFIG_UDRD_D_CH
{
	T_UFIG_K key;
	T_UFIG_D_CH data;
};

struct T_UFIG_UDRD_D_CH_RW_965
{
	T_UFIG_K key;
	T_UFIG_D_CH_RW_965 data;
};

struct T_UFIG_UDRD_D_CH_RW_955
{
	T_UFIG_K key;
	T_UFIG_D_CH_RW_955 data;
};

struct T_UFIG_UDRD_D_CH_RW_950_2
{
	T_UFIG_K key;
	T_UFIG_D_CH_RW_950_2 data;
};

struct T_UFIG_UDRD_D_CH_RW_950
{
	T_UFIG_K key;
	T_UFIG_D_CH_RW_950 data;
};

struct T_UFIG_UDRD_D_CH_RW_940
{
	T_UFIG_K key;
	T_UFIG_D_CH_RW_940 data;
};

struct T_UFIG_UDRD_D_CH_RW_930
{
	T_UFIG_K key;
	T_UFIG_D_CH_RW_930 data;
// 	void GetUfig(T_UFIG_K &UfigK, T_UFIG_D_CH_930 &UfigD)
// 	{
// 		UfigK = key;
// 		data.Get(UfigD);
// 	}
// 	void SetUfig(const T_UFIG_K &UfigK, const T_UFIG_D_CH_930 &UfigD)
// 	{
// 		key = UfigK;
// 		data.Set(UfigD);
// 	}
};
struct T_UFIG_UDRD_D_CH_RW_925
{
	T_UFIG_K key;
	T_UFIG_D_CH_RW_925 data;
// 	void GetUfig(T_UFIG_K &UfigK, T_UFIG_D_CH_925 &UfigD)
// 	{
// 		UfigK = key;
// 		data.Get(UfigD);
// 	}
// 	void SetUfig(const T_UFIG_K &UfigK, const T_UFIG_D_CH_925 &UfigD)
// 	{
// 		key = UfigK;
// 		data.Set(UfigD);
// 	}
};
struct T_UFIG_UDRD_D_CH_RW_895
{
	T_UFIG_K key;
	T_UFIG_D_CH_RW_895 data;
// 	void GetUfig(T_UFIG_K &UfigK, T_UFIG_D_CH_895 &UfigD)
// 	{
// 		UfigK = key;
// 		data.Get(UfigD);
// 	}
// 	void SetUfig(const T_UFIG_K &UfigK, const T_UFIG_D_CH_895 &UfigD)
// 	{
// 		key = UfigK;
// 		data.Set(UfigD);
// 	}
};

struct T_UFIG_UDRD_D_CH_RW_891
{
	T_UFIG_K key;
	T_UFIG_D_CH_RW_891 data;
// 	void GetUfig(T_UFIG_K &UfigK, T_UFIG_D_CH_891 &UfigD)
// 	{
// 		UfigK = key;
// 		data.Get(UfigD);
// 	}
// 	void SetUfig(const T_UFIG_K &UfigK, const T_UFIG_D_CH_891 &UfigD)
// 	{
// 		key = UfigK;
// 		data.Set(UfigD);
// 	}
};

struct T_UFIG_UDRD_D_CH_RW_885
{
	T_UFIG_K key;
	T_UFIG_D_CH_RW_885 data;
// 	void GetUfig(T_UFIG_K &UfigK, T_UFIG_D_CH_885 &UfigD)
// 	{
// 		UfigK = key;
// 		data.Get(UfigD);
// 	}
// 	void SetUfig(const T_UFIG_K &UfigK, const T_UFIG_D_CH_885 &UfigD)
// 	{
// 		key = UfigK;
// 		data.Set(UfigD);
// 	}
};
struct T_UFIG_UDRD_D_CH_RW_883
{
	T_UFIG_K key;
	T_UFIG_D_CH_RW_883 data;
	//void GetUfig(T_UFIG_K &UfigK, T_UFIG_D_CH_883 &UfigD)
	//{
	//	UfigK = key;
	//	data.Get(UfigD);
	//}
	//void SetUfig(const T_UFIG_K &UfigK, const T_UFIG_D_CH_883 &UfigD)
	//{
	//	key = UfigK;
	//	data.Set(UfigD);
	//}
};
struct T_UFIG_UDRD_D_CH_RW_870
{
	T_UFIG_K key;
	T_UFIG_D_CH_RW_870 data;
	//void GetUfig(T_UFIG_K &UfigK, T_UFIG_D_CH_870 &UfigD)
	//{
	//	UfigK = key;
	//	data.Get(UfigD);
	//}
	//void SetUfig(const T_UFIG_K &UfigK, const T_UFIG_D_CH_870 &UfigD)
	//{
	//	key = UfigK;
	//	data.Set(UfigD);
	//}
};
struct T_UFIG_UDRD_D_CH_RW_865
{
	T_UFIG_K key;
	T_UFIG_D_CH_RW_865 data;
	//void GetUfig(T_UFIG_K &UfigK, T_UFIG_D_CH_865 &UfigD)
	//{
	//	UfigK = key;
	//	data.Get(UfigD);
	//}
	//void SetUfig(const T_UFIG_K &UfigK, const T_UFIG_D_CH_865 &UfigD)
	//{
	//	key = UfigK;
	//	data.Set(UfigD);
	//}
};
struct T_UFIG_UDRD_D_CH_RW_860
{
	T_UFIG_K key;
	T_UFIG_D_CH_RW_860 data;
	//void GetUfig(T_UFIG_K &UfigK, T_UFIG_D_CH_860 &UfigD)
	//{
	//	UfigK = key;
	//	data.Get(UfigD);
	//}
	//void SetUfig(const T_UFIG_K &UfigK, const T_UFIG_D_CH_860 &UfigD)
	//{
	//	key = UfigK;
	//	data.Set(UfigD);
	//}
};
struct T_UFIG_UDRD_D_CH_RW_855
{
	T_UFIG_K key;
	T_UFIG_D_CH_RW_855 data;
	//void GetUfig(T_UFIG_K &UfigK, T_UFIG_D_CH_855 &UfigD)
	//{
	//	UfigK = key;
	//	data.Get(UfigD);
	//}
	//void SetUfig(const T_UFIG_K &UfigK, const T_UFIG_D_CH_855 &UfigD)
	//{
	//	key = UfigK;
	//	data.Set(UfigD);
	//}
};
struct T_UFIG_UDRD_D_CH_RW_851
{
	T_UFIG_K key;
	T_UFIG_D_CH_RW_851 data;
	//void GetUfig(T_UFIG_K &UfigK, T_UFIG_D_CH_851 &UfigD)
	//{
	//	UfigK = key;
	//	data.Get(UfigD);
	//}
	//void SetUfig(const T_UFIG_K &UfigK, const T_UFIG_D_CH_851 &UfigD)
	//{
	//	key = UfigK;
	//	data.Set(UfigD);
	//}
};
struct T_UFIG_UDRD_D_CH_RW_845
{
	T_UFIG_K key;
	T_UFIG_D_CH_RW_845 data;
	//void GetUfig(T_UFIG_K &UfigK, T_UFIG_D_CH_845 &UfigD)
	//{
	//	UfigK = key;
	//	data.Get(UfigD);
	//}
	//void SetUfig(const T_UFIG_K &UfigK, const T_UFIG_D_CH_845 &UfigD)
	//{
	//	key = UfigK;
	//	data.Set(UfigD);
	//}
};
struct T_UFIG_UDRD_D_CH_RW_841
{
	T_UFIG_K key;
	T_UFIG_D_CH_RW_841 data;
	//void GetUfig(T_UFIG_K &UfigK, T_UFIG_D_CH_841 &UfigD)
	//{
	//	UfigK = key;
	//	data.Get(UfigD);
	//}
	//void SetUfig(const T_UFIG_K &UfigK, const T_UFIG_D_CH_841 &UfigD)
	//{
	//	key = UfigK;
	//	data.Set(UfigD);
	//}
};
struct T_UFIG_UDRD_D_CH_RW_835
{
	T_UFIG_K key;
	T_UFIG_D_CH_RW_835 data;
	void GetUfig(T_UFIG_K &UfigK, T_UFIG_D_CH_835 &UfigD)
	{
		UfigK = key;
		data.Get(UfigD);
	}
	void SetUfig(const T_UFIG_K &UfigK, const T_UFIG_D_CH_835 &UfigD)
	{
		key = UfigK;
		data.Set(UfigD);
	}
};
struct T_UFIG_UDRD_D_CH_RW_825
{
	T_UFIG_K key;
	T_UFIG_D_CH_RW_825 data;
	void GetUfig(T_UFIG_K &UfigK, T_UFIG_D_CH_825 &UfigD)
	{
		UfigK = key;
		data.Get(UfigD);
	}
	void SetUfig(const T_UFIG_K &UfigK, const T_UFIG_D_CH_825 &UfigD)
	{
		key = UfigK;
		data.Set(UfigD);
	}
};
struct T_UFIG_UDRD_D_CH_RW_821
{
	T_UFIG_K key;
	T_UFIG_D_CH_RW_821 data;
	void GetUfig(T_UFIG_K &UfigK, T_UFIG_D_CH_821 &UfigD)
	{
		UfigK = key;
		data.Get(UfigD);
	}
	void SetUfig(const T_UFIG_K &UfigK, const T_UFIG_D_CH_821 &UfigD)
	{
		key = UfigK;
		data.Set(UfigD);
	}
};
struct T_UFIG_UDRD_D_CH_RW_810
{
	T_UFIG_K key;
	T_UFIG_D_CH_RW_810 data;
	void GetUfig(T_UFIG_K &UfigK, T_UFIG_D_CH_810 &UfigD)
	{
		UfigK = key;
		data.Get(UfigD);
	}
	void SetUfig(const T_UFIG_K &UfigK, const T_UFIG_D_CH_810 &UfigD)
	{
		key = UfigK;
		data.Set(UfigD);
	}
};
struct T_UFIG_UDRD_D_CH_RW_790
{
	T_UFIG_K key;
	T_UFIG_D_CH_RW_790 data;
	void GetUfig(T_UFIG_K &UfigK, T_UFIG_D_CH_790 &UfigD)
	{
		UfigK = key;
		data.Get(UfigD);
	}
	void SetUfig(const T_UFIG_K &UfigK, const T_UFIG_D_CH_790 &UfigD)
	{
		key = UfigK;
		data.Set(UfigD);
	}
};
struct T_UFIG_UDRD_D_CH_RW
{
	T_UFIG_K key;
	T_UFIG_D_CH_RW data;
	void GetUfig(T_UFIG_K &UfigK, T_UFIG_D_CH &UfigD)
	{
		UfigK = key;
		data.Get(UfigD);
	}
	void SetUfig(const T_UFIG_K &UfigK, const T_UFIG_D_CH &UfigD)
	{
		key = UfigK;
		data.Set(UfigD);
	}
	void Convert965(T_UFIG_UDRD_D_CH_RW_965& rData)
	{
		key = rData.key;
		data.Convert965(rData.data);
	}
	void Convert955(T_UFIG_UDRD_D_CH_RW_955& rData)
	{
		key = rData.key;
		data.Convert955(rData.data);
	}
	void Convert950_2(T_UFIG_UDRD_D_CH_RW_950_2& rData)
	{
		key = rData.key;
		data.Convert950_2(rData.data);
	}
	void Convert950(T_UFIG_UDRD_D_CH_RW_950& rData)
	{
		key = rData.key;
		data.Convert950(rData.data);
	}
	void Convert940(T_UFIG_UDRD_D_CH_RW_940& rData)
	{
		key = rData.key;
		data.Convert940(rData.data);
	}
	void Convert930(T_UFIG_UDRD_D_CH_RW_930& rData)
	{
		key = rData.key;
		data.Convert930(rData.data);
	}
	void Convert925(T_UFIG_UDRD_D_CH_RW_925& rData)
	{
		key = rData.key;
		data.Convert925(rData.data);
	}
	void Convert895(T_UFIG_UDRD_D_CH_RW_895& rData)
	{
		key = rData.key;
		data.Convert895(rData.data);
	}
	void Convert891(T_UFIG_UDRD_D_CH_RW_891& rData)
	{
		key = rData.key;
		data.Convert891(rData.data);
	}
	void Convert885(T_UFIG_UDRD_D_CH_RW_885& rData)
	{
		key = rData.key;
		data.Convert885(rData.data);
	}
	void Convert883(T_UFIG_UDRD_D_CH_RW_883& rData)
	{
		key = rData.key;
		data.Convert883(rData.data);
	}
	void Convert870(T_UFIG_UDRD_D_CH_RW_870& rData)
	{
		key = rData.key;
		data.Convert870(rData.data);
	}
	void Convert865(T_UFIG_UDRD_D_CH_RW_865& rData)
	{
		key = rData.key;
		data.Convert865(rData.data);
	}
	void Convert860(T_UFIG_UDRD_D_CH_RW_860& rData)
	{
		key = rData.key;
		data.Convert860(rData.data);
	}
	void Convert855(T_UFIG_UDRD_D_CH_RW_855& rData)
	{
	  key = rData.key;
	  data.Convert855(rData.data);
	}
	void Convert851(T_UFIG_UDRD_D_CH_RW_851& rData)
	{
		key = rData.key;
		data.Convert851(rData.data);
	}
	void Convert845(T_UFIG_UDRD_D_CH_RW_845& rData)
	{
		key = rData.key;
		data.Convert845(rData.data);
	}
	void Convert841(T_UFIG_UDRD_D_CH_RW_841& rData)
	{
		key = rData.key;
		data.Convert841(rData.data);
	}
	void Convert835(T_UFIG_UDRD_D_CH_RW_835& rData)
	{
		key = rData.key;
		data.Convert835(rData.data);
	}
	void Convert825(T_UFIG_UDRD_D_CH_RW_825& rData)
	{
		key = rData.key;
		data.Convert825(rData.data);
	}
	void Convert821(T_UFIG_UDRD_D_CH_RW_821& rData)
	{
		key = rData.key;
		data.Convert821(rData.data);
	}
	void Convert810(T_UFIG_UDRD_D_CH_RW_810& rData)
	{
		key = rData.key;
		data.Convert810(rData.data);
	}
	void Convert790(T_UFIG_UDRD_D_CH_RW_790& rData)
	{
		key = rData.key;
		data.Convert790(rData.data);
	}
};
struct T_UFIG_UNIT
{
	T_UFIG_UNIT()
	{
		NoUnit=D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEUFIG 53

////////////////////////////////////////////////////////////////////////////////////////////////////////
// 보고서 Table 정보
// \wg_tb\TBAMRUDTData.h의 define도 함께 수정할것.
#define D_TB_COMP_CNT         20  // 선택된 component의 최대 개수
#define D_TB_HEADER_CNT_791   72  // 헤더 컬럼의 최대 개수
#define D_TB_HEADER_CNT      150  // 헤더 컬럼의 최대 개수
#define D_TB_SORT_CNT         20  // Sort 정보의 최대 개수
#define D_TB_SPOS_CNT         15
#define D_TB_STOR_BETA_CNT   150
#define D_TB_STOR_LCOM_CNT   150

struct T_UTBL_HEADER_D_791
{
	int nRowCount; // 헤더 row수
	int nAllColCount; // 헤더 전체 column수
	int nCurColCount; // 헤더 선택한 column수
	int nUnselColCount; // 헤더 선택하지 column수 (Post table에서 사용함)
	int aCurIndex[D_TB_HEADER_CNT_791]; // 선택한 component를 포함한 헤더 index
	int aUnselIndex[D_TB_HEADER_CNT_791]; // 선택하지 component를 포함한 헤더 index (Post table에서 사용함)
	
	T_UTBL_HEADER_D_791()
	{
		Initialize();
	}
	void Initialize()
	{
		nRowCount = 0;
		nAllColCount = 0;
		nCurColCount = 0;
		nUnselColCount = 0;
		for(int i=0; i<D_TB_HEADER_CNT_791; i++)
		{
			aCurIndex[i] = 0;
			aUnselIndex[i] = 0;
		}
	}
	T_UTBL_HEADER_D_791& operator=(const T_UTBL_HEADER_D_791 &src)
	{
		nRowCount = src.nRowCount;
		nAllColCount = src.nAllColCount;
		nCurColCount = src.nCurColCount;
		nUnselColCount = src.nUnselColCount;
		for(int i=0; i<D_TB_HEADER_CNT_791; i++)
		{
			aCurIndex[i] = src.aCurIndex[i];
			aUnselIndex[i] = src.aUnselIndex[i];
		}
		return *this;
	}
	T_UTBL_HEADER_D_791(const T_UTBL_HEADER_D_791 &src) { *this = src; }
};

struct T_UTBL_HEADER_D
{
	int nRowCount; // 헤더 row수
	int nAllColCount; // 헤더 전체 column수
	int nCurColCount; // 헤더 선택한 column수
	int nUnselColCount; // 헤더 선택하지 column수 (Post table에서 사용함)
	int aCurIndex[D_TB_HEADER_CNT]; // 선택한 component를 포함한 헤더 index
	int aUnselIndex[D_TB_HEADER_CNT]; // 선택하지 component를 포함한 헤더 index (Post table에서 사용함)

	T_UTBL_HEADER_D()
	{
		Initialize();
	}
	void Initialize()
	{
		nRowCount = 0;
		nAllColCount = 0;
		nCurColCount = 0;
		nUnselColCount = 0;
		for(int i=0; i<D_TB_HEADER_CNT; i++)
		{
			aCurIndex[i] = 0;
			aUnselIndex[i] = 0;
		}
	}
	T_UTBL_HEADER_D& operator=(const T_UTBL_HEADER_D &src)
	{
		nRowCount = src.nRowCount;
		nAllColCount = src.nAllColCount;
		nCurColCount = src.nCurColCount;
		nUnselColCount = src.nUnselColCount;
		for(int i=0; i<D_TB_HEADER_CNT; i++)
		{
			aCurIndex[i] = src.aCurIndex[i];
			aUnselIndex[i] = src.aUnselIndex[i];
		}
		return *this;
	}
	T_UTBL_HEADER_D(const T_UTBL_HEADER_D &src) { *this = src; }

	void Convert791(T_UTBL_HEADER_D_791& src)
	{
		nRowCount = src.nRowCount;
		nAllColCount = src.nAllColCount;
		nCurColCount = src.nCurColCount;
		nUnselColCount = src.nUnselColCount;

		for(int i=0; i<D_TB_HEADER_CNT; i++)
		{
			if(i<D_TB_HEADER_CNT_791)
			{
				aCurIndex[i] = src.aCurIndex[i];
				aUnselIndex[i] = src.aUnselIndex[i];
			}
			else
			{
				aCurIndex[i] = 0;
				aUnselIndex[i] = 0;
			}
		}
	}
};

struct T_UTBL_CELL_D
{
	int nRowCount ; // Cell row수
	
	T_UTBL_CELL_D()
	{
		Initialize();
	}
	void Initialize()
	{
		nRowCount = 0;
	}
};

struct T_UTBL_SORT_D
{
	int nSortCount;
	int  aSortFldID [D_TB_SORT_CNT]; // Column Number
	BOOL aSortAscend[D_TB_SORT_CNT];  // 오름차순, 내림차순
	int  aSortType  [D_TB_SORT_CNT]; // Sort Type (전처리 테이블 전용)

	T_UTBL_SORT_D()
	{
		Initialize();
	}
	void Initialize()
	{
		nSortCount = 0;
		for(int i=0; i<D_TB_SORT_CNT; i++)
		{
			aSortFldID [i] = 0;
			aSortAscend[i] = FALSE;
			aSortType  [i] = 0;
		}
	}
};

struct T_UTBL_STYLE_D
{
	int nStyleCount;
	int nStylePrefCount;
	DWORD    aStyleCol     [D_TB_HEADER_CNT];  // Style column 
	int      aStyleType    [D_TB_HEADER_CNT];  // Style column type
	CString  aStyleTitle   [D_TB_HEADER_CNT];  // Style column title name
	int      aPrefValueType[D_TB_HEADER_CNT];  // Style Preference : Value Type
	int      aPrefFormat   [D_TB_HEADER_CNT];  // Style Preference : Format
	int      aPrefPlace    [D_TB_HEADER_CNT];  // Style Preference : Place
	int      aPrefWidth    [D_TB_HEADER_CNT];  // Style Preference : Width

	T_UTBL_STYLE_D()
	{
		Initialize();
	}
	void Initialize()
	{
		nStyleCount = 0;
		nStylePrefCount = 0;
		for(int i=0; i<D_TB_HEADER_CNT; i++)
		{
			aStyleCol     [i] = 0;
			aStyleType    [i] = 0;
			aStyleTitle   [i] = "";
			aPrefValueType[i] = 0;
			aPrefFormat   [i] = 0;
			aPrefPlace    [i] = 0;
			aPrefWidth    [i] = 0;
		}
	}
	T_UTBL_STYLE_D& operator=(const T_UTBL_STYLE_D &src)
	{
		nStyleCount = src.nStyleCount;
		nStylePrefCount = src.nStylePrefCount;
		for(int i=0; i<D_TB_HEADER_CNT; i++)
		{
			aStyleCol     [i] = src.aStyleCol     [i];
			aStyleType    [i] = src.aStyleType    [i];
			aStyleTitle   [i] = src.aStyleTitle   [i];
			aPrefValueType[i] = src.aPrefValueType[i];
			aPrefFormat   [i] = src.aPrefFormat   [i];
			aPrefPlace    [i] = src.aPrefPlace    [i];
			aPrefWidth    [i] = src.aPrefWidth    [i];
		}
		return *this;
	}
	T_UTBL_STYLE_D(const T_UTBL_STYLE_D &src) { *this = src; }
};

struct T_UTBL_STYLE_D_CH_791
{
	int nStyleCount;
	int nStylePrefCount;
	DWORD    aStyleCol     [D_TB_HEADER_CNT_791];  // Style column 
	int      aStyleType    [D_TB_HEADER_CNT_791];  // Style column type
	char     aStyleTitle   [D_TB_HEADER_CNT_791][60];  // Style column title name
	int      aPrefValueType[D_TB_HEADER_CNT_791];  // Style Preference : Value Type
	int      aPrefFormat   [D_TB_HEADER_CNT_791];  // Style Preference : Format
	int      aPrefPlace    [D_TB_HEADER_CNT_791];  // Style Preference : Place
	int      aPrefWidth    [D_TB_HEADER_CNT_791];  // Style Preference : Width
};

struct T_UTBL_STYLE_D_CH
{
	int nStyleCount;
	int nStylePrefCount;
	DWORD    aStyleCol     [D_TB_HEADER_CNT];  // Style column 
	int      aStyleType    [D_TB_HEADER_CNT];  // Style column type
	char     aStyleTitle   [D_TB_HEADER_CNT][60];  // Style column title name
	int      aPrefValueType[D_TB_HEADER_CNT];  // Style Preference : Value Type
	int      aPrefFormat   [D_TB_HEADER_CNT];  // Style Preference : Format
	int      aPrefPlace    [D_TB_HEADER_CNT];  // Style Preference : Place
	int      aPrefWidth    [D_TB_HEADER_CNT];  // Style Preference : Width

	void Get(T_UTBL_STYLE_D &rData)
	{
		for(int i=0 ; i<D_TB_HEADER_CNT ; ++i)
		{
			ConvertCharStr(aStyleTitle[i], rData.aStyleTitle[i], sizeof(aStyleTitle[i]));
			rData.aStyleCol     [i] = aStyleCol     [i];
			rData.aStyleType    [i] = aStyleType    [i];
			rData.aPrefValueType[i] = aPrefValueType[i];
			rData.aPrefFormat   [i] = aPrefFormat   [i];
			rData.aPrefPlace    [i] = aPrefPlace    [i];
			rData.aPrefWidth    [i] = aPrefWidth    [i];
		}
		rData.nStyleCount = nStyleCount;
		rData.nStylePrefCount  = nStylePrefCount;
	}
	void Set(T_UTBL_STYLE_D Data)
	{
		for(int i=0 ; i<D_TB_HEADER_CNT ; ++i)
		{
			ConvertStrChar(Data.aStyleTitle[i], aStyleTitle[i], sizeof(aStyleTitle[i]));
			aStyleCol     [i] = Data.aStyleCol     [i];
			aStyleType    [i] = Data.aStyleType    [i];
			aPrefValueType[i] = Data.aPrefValueType[i];
			aPrefFormat   [i] = Data.aPrefFormat   [i];
			aPrefPlace    [i] = Data.aPrefPlace    [i];
			aPrefWidth    [i] = Data.aPrefWidth    [i];
		}
		nStyleCount = Data.nStyleCount;
		nStylePrefCount  = Data.nStylePrefCount;
	}

	void Convert791(T_UTBL_STYLE_D_CH_791& src)
	{
		nStyleCount = src.nStyleCount;
		nStylePrefCount  = src.nStylePrefCount;

		for(int i=0 ; i<D_TB_HEADER_CNT ; ++i)
		{
			if(i<D_TB_HEADER_CNT_791)
			{
				memcpy(aStyleTitle[i], src.aStyleTitle[i], sizeof(aStyleTitle[i]));
				aStyleCol     [i] = src.aStyleCol     [i];
				aStyleType    [i] = src.aStyleType    [i];
				aPrefValueType[i] = src.aPrefValueType[i];
				aPrefFormat   [i] = src.aPrefFormat   [i];
				aPrefPlace    [i] = src.aPrefPlace    [i];
				aPrefWidth    [i] = src.aPrefWidth    [i];
			}
			else
			{
				memcpy(aStyleTitle[i], "", sizeof(aStyleTitle[i]));
				aStyleCol     [i] = 0;
				aStyleType    [i] = 0;
				aPrefValueType[i] = 0;
				aPrefFormat   [i] = 0;
				aPrefPlace    [i] = 0;
				aPrefWidth    [i] = 0;
			}
		}
	}
};

struct T_UTBL_LCIX_STEP
{
	int lcix;
	int step;
};

struct T_UTBL_ACTIVE_D
{
	int                         nKeyType  ; // aKey에 저장되는 key type이 어떤것인지 나타냄. (0: none, 1: node, 2: element, 3: etc) -> D_TB_KEY_TYPE_XXXX
	int                         nChildType; // 상속받은 class를 구분함. aKey를 재가공하는데 필요함. (0: none, 1: CTBPostDS1, 2: CTBPostDS2) -> D_TB_CHILD_TYPE_XXXX
	CArray<UINT, UINT>          aKey    ; // Element 혹은 Node Key
	CArray<UINT, UINT>          aLoad   ; // Loadcase Key
	CArray<UINT, UINT>          aLkind  ; // Loadcase Type
	CArray<int, int>            aMinMax ; // Min Max
	CArray<int, int>            aStep   ; // Step
	CArray<T_UTBL_LCIX_STEP, T_UTBL_LCIX_STEP&> aLoadStep;  // Loadcase-Step 
	CArray<int, int>            aPart   ; // Part 0:i, 2:1/4, 4:1/2, 6:3/4, 7:j
	BOOL                        bNode[2]; // index: 0-Center, 1-Node
	CArray<int, int>            aCurEtc;  // Design Table의 option
	CArray<UINT, UINT>          aCurData; // 특정 테이블에서 저장하기 위한 정보 (CTBPostStrw:m_aCurKeyIx, CTBPostDFactElem:m_aCurUnitLoad)
	BOOL                        bAvgNodal; // Average Nodal을 사용할지를 결정
	BOOL						bMembMode; // Member Mode(Beam Force에서 사용)
	int							nDispOpt;  // Displacement Option (Analysis result/Result Tables/Displacements) CS 에서 활성화 ( 0: Accumulative, 1: Current, 2: Real)
	CArray<int, int>            aCurSectPos;
	CArray<int, int>            aOutputStep;
	BOOL						bAllFiberCell;
	CArray<UINT, UINT>          aTendonGroup;
	CArray<UINT, UINT>          aStage;

	T_UTBL_ACTIVE_D() {Initialize();}
	void Initialize()
	{
		nKeyType = 0;
		nChildType = 0;
		aKey.RemoveAll();
		aLoad.RemoveAll();
		aLkind.RemoveAll();
		aMinMax.RemoveAll();
		aStep.RemoveAll();
		aLoadStep.RemoveAll();
		aPart.RemoveAll();
		bNode[0] = FALSE;
		bNode[1] = FALSE;
		aCurEtc.RemoveAll();
		aCurData.RemoveAll();
		bAvgNodal = FALSE;
		bMembMode = FALSE;
		nDispOpt = 0;
		aCurSectPos.RemoveAll();
		aOutputStep.RemoveAll();
		bAllFiberCell = FALSE;
		aTendonGroup.RemoveAll();
		aStage.RemoveAll();
	}
	T_UTBL_ACTIVE_D& operator=(const T_UTBL_ACTIVE_D &src)
	{
		nKeyType = src.nKeyType;
		nChildType = src.nChildType;
		aKey.Copy(src.aKey);
		aLoad.Copy(src.aLoad);
		aLkind.Copy(src.aLkind);
		aMinMax.Copy(src.aMinMax);
		aStep.Copy(src.aStep);
		aLoadStep.Copy(src.aLoadStep);
		aPart.Copy(src.aPart);
		bNode[0] = src.bNode[0];
		bNode[1] = src.bNode[1];
		aCurEtc.Copy(src.aCurEtc);
		aCurData.Copy(src.aCurData);
		bAvgNodal =  src.bAvgNodal;
		bMembMode = src.bMembMode;
		nDispOpt = src.nDispOpt;
		aCurSectPos.Copy(src.aCurSectPos);
		aOutputStep.Copy(src.aOutputStep);
		bAllFiberCell = src.bAllFiberCell;
		aTendonGroup.Copy(src.aTendonGroup);
		aStage.Copy(src.aStage);
		return *this;
	}
	T_UTBL_ACTIVE_D(const T_UTBL_ACTIVE_D &src) { *this = src; }
};

struct T_UTBL_ACTIVE_D_CH_845
{
	int                         nKeyType  ; // aKey에 저장되는 key type이 어떤것인지 나타냄. (0: none, 1: node, 2: element, 3: etc) -> D_TB_KEY_TYPE_XXXX
	int                         nChildType; // 상속받은 class를 구분함. aKey를 재가공하는데 필요함. (0: none, 1: CTBPostDS1, 2: CTBPostDS2) -> D_TB_CHILD_TYPE_XXXX
	CArray<UINT, UINT>          aKey    ; // Element 혹은 Node Key
	CArray<UINT, UINT>          aLoad   ; // Loadcase Key
	CArray<UINT, UINT>          aLkind  ; // Loadcase Type
	CArray<int, int>            aMinMax ; // Min Max
	CArray<int, int>            aStep   ; // Step
	CArray<T_UTBL_LCIX_STEP, T_UTBL_LCIX_STEP&> aLoadStep;  // Loadcase-Step 
	CArray<int, int>            aPart   ; // Part 0:i, 2:1/4, 4:1/2, 6:3/4, 7:j
	BOOL                        bNode[2]; // index: 0-Center, 1-Node
	CArray<int, int>            aCurEtc;  // Design Table의 option
	CArray<UINT, UINT>          aCurData; // 특정 테이블에서 저장하기 위한 정보 (CTBPostStrw:m_aCurKeyIx, CTBPostDFactElem:m_aCurUnitLoad)
};
struct T_UTBL_ACTIVE_D_CH_885
{
	int                         nKeyType  ; // aKey에 저장되는 key type이 어떤것인지 나타냄. (0: none, 1: node, 2: element, 3: etc) -> D_TB_KEY_TYPE_XXXX
	int                         nChildType; // 상속받은 class를 구분함. aKey를 재가공하는데 필요함. (0: none, 1: CTBPostDS1, 2: CTBPostDS2) -> D_TB_CHILD_TYPE_XXXX
	CArray<UINT, UINT>          aKey    ; // Element 혹은 Node Key
	CArray<UINT, UINT>          aLoad   ; // Loadcase Key
	CArray<UINT, UINT>          aLkind  ; // Loadcase Type
	CArray<int, int>            aMinMax ; // Min Max
	CArray<int, int>            aStep   ; // Step
	CArray<T_UTBL_LCIX_STEP, T_UTBL_LCIX_STEP&> aLoadStep;  // Loadcase-Step 
	CArray<int, int>            aPart   ; // Part 0:i, 2:1/4, 4:1/2, 6:3/4, 7:j
	BOOL                        bNode[2]; // index: 0-Center, 1-Node
	CArray<int, int>            aCurEtc;  // Design Table의 option
	CArray<UINT, UINT>          aCurData; // 특정 테이블에서 저장하기 위한 정보 (CTBPostStrw:m_aCurKeyIx, CTBPostDFactElem:m_aCurUnitLoad)
	BOOL                        bAvgNodal; // Average Nodal을 사용할지를 결정
};
struct T_UTBL_ACTIVE_D_CH_945
{
	int                         nKeyType; // aKey에 저장되는 key type이 어떤것인지 나타냄. (0: none, 1: node, 2: element, 3: etc) -> D_TB_KEY_TYPE_XXXX
	int                         nChildType; // 상속받은 class를 구분함. aKey를 재가공하는데 필요함. (0: none, 1: CTBPostDS1, 2: CTBPostDS2) -> D_TB_CHILD_TYPE_XXXX
	CArray<UINT, UINT>          aKey; // Element 혹은 Node Key
	CArray<UINT, UINT>          aLoad; // Loadcase Key
	CArray<UINT, UINT>          aLkind; // Loadcase Type
	CArray<int, int>            aMinMax; // Min Max
	CArray<int, int>            aStep; // Step
	CArray<T_UTBL_LCIX_STEP, T_UTBL_LCIX_STEP&> aLoadStep;  // Loadcase-Step 
	CArray<int, int>            aPart; // Part 0:i, 2:1/4, 4:1/2, 6:3/4, 7:j
	BOOL                        bNode[2]; // index: 0-Center, 1-Node
	CArray<int, int>            aCurEtc;  // Design Table의 option
	CArray<UINT, UINT>          aCurData; // 특정 테이블에서 저장하기 위한 정보 (CTBPostStrw:m_aCurKeyIx, CTBPostDFactElem:m_aCurUnitLoad)
	BOOL                        bAvgNodal; // Average Nodal을 사용할지를 결정
	BOOL						bMembMode;
};
struct T_UTBL_ACTIVE_D_CH_950
{
	int                         nKeyType; // aKey에 저장되는 key type이 어떤것인지 나타냄. (0: none, 1: node, 2: element, 3: etc) -> D_TB_KEY_TYPE_XXXX
	int                         nChildType; // 상속받은 class를 구분함. aKey를 재가공하는데 필요함. (0: none, 1: CTBPostDS1, 2: CTBPostDS2) -> D_TB_CHILD_TYPE_XXXX
	CArray<UINT, UINT>          aKey; // Element 혹은 Node Key
	CArray<UINT, UINT>          aLoad; // Loadcase Key
	CArray<UINT, UINT>          aLkind; // Loadcase Type
	CArray<int, int>            aMinMax; // Min Max
	CArray<int, int>            aStep; // Step
	CArray<T_UTBL_LCIX_STEP, T_UTBL_LCIX_STEP&> aLoadStep;  // Loadcase-Step 
	CArray<int, int>            aPart; // Part 0:i, 2:1/4, 4:1/2, 6:3/4, 7:j
	BOOL                        bNode[2]; // index: 0-Center, 1-Node
	CArray<int, int>            aCurEtc;  // Design Table의 option
	CArray<UINT, UINT>          aCurData; // 특정 테이블에서 저장하기 위한 정보 (CTBPostStrw:m_aCurKeyIx, CTBPostDFactElem:m_aCurUnitLoad)
	BOOL                        bAvgNodal; // Average Nodal을 사용할지를 결정
	BOOL						bMembMode;
	int							nDispOpt;
};
struct T_UTBL_ACTIVE_D_CH_960
{
	int                         nKeyType; // aKey에 저장되는 key type이 어떤것인지 나타냄. (0: none, 1: node, 2: element, 3: etc) -> D_TB_KEY_TYPE_XXXX
	int                         nChildType; // 상속받은 class를 구분함. aKey를 재가공하는데 필요함. (0: none, 1: CTBPostDS1, 2: CTBPostDS2) -> D_TB_CHILD_TYPE_XXXX
	CArray<UINT, UINT>          aKey; // Element 혹은 Node Key
	CArray<UINT, UINT>          aLoad; // Loadcase Key
	CArray<UINT, UINT>          aLkind; // Loadcase Type
	CArray<int, int>            aMinMax; // Min Max
	CArray<int, int>            aStep; // Step
	CArray<T_UTBL_LCIX_STEP, T_UTBL_LCIX_STEP&> aLoadStep;  // Loadcase-Step 
	CArray<int, int>            aPart; // Part 0:i, 2:1/4, 4:1/2, 6:3/4, 7:j
	BOOL                        bNode[2]; // index: 0-Center, 1-Node
	CArray<int, int>            aCurEtc;  // Design Table의 option
	CArray<UINT, UINT>          aCurData; // 특정 테이블에서 저장하기 위한 정보 (CTBPostStrw:m_aCurKeyIx, CTBPostDFactElem:m_aCurUnitLoad)
	BOOL                        bAvgNodal; // Average Nodal을 사용할지를 결정
	BOOL						bMembMode;
	int							nDispOpt;
	CArray<int, int>            aCurSectPos;
	CArray<int, int>            aOutputStep;
	BOOL						bAllFiberCell;
};

struct T_UTBL_ACTIVE_D_CH
{
	int                         nKeyType  ; // aKey에 저장되는 key type이 어떤것인지 나타냄. (0: none, 1: node, 2: element, 3: etc) -> D_TB_KEY_TYPE_XXXX
	int                         nChildType; // 상속받은 class를 구분함. aKey를 재가공하는데 필요함. (0: none, 1: CTBPostDS1, 2: CTBPostDS2) -> D_TB_CHILD_TYPE_XXXX
	CArray<UINT, UINT>          aKey    ; // Element 혹은 Node Key
	CArray<UINT, UINT>          aLoad   ; // Loadcase Key
	CArray<UINT, UINT>          aLkind  ; // Loadcase Type
	CArray<int, int>            aMinMax ; // Min Max
	CArray<int, int>            aStep   ; // Step
	CArray<T_UTBL_LCIX_STEP, T_UTBL_LCIX_STEP&> aLoadStep;  // Loadcase-Step 
	CArray<int, int>            aPart   ; // Part 0:i, 2:1/4, 4:1/2, 6:3/4, 7:j
	BOOL                        bNode[2]; // index: 0-Center, 1-Node
	CArray<int, int>            aCurEtc;  // Design Table의 option
	CArray<UINT, UINT>          aCurData; // 특정 테이블에서 저장하기 위한 정보 (CTBPostStrw:m_aCurKeyIx, CTBPostDFactElem:m_aCurUnitLoad)
	BOOL                        bAvgNodal; // Average Nodal을 사용할지를 결정
	BOOL						bMembMode;
	int							nDispOpt;
	CArray<int, int>            aCurSectPos;
	CArray<int, int>            aOutputStep;
	BOOL						bAllFiberCell;
	CArray<UINT, UINT>          aTendonGroup;
	CArray<UINT, UINT>          aStage;

	T_UTBL_ACTIVE_D_CH() {}  
	void Get(T_UTBL_ACTIVE_D &rData)
	{
		rData.nKeyType = nKeyType;
		rData.nChildType = nChildType;
		rData.aKey.Copy(aKey);
		rData.aLoad.Copy(aLoad);
		rData.aLkind.Copy(aLkind);
		rData.aMinMax.Copy(aMinMax);
		rData.aStep.Copy(aStep);
		rData.aLoadStep.Copy(aLoadStep);
		rData.aPart.Copy(aPart);
		rData.bNode[0] = bNode[0];
		rData.bNode[1] = bNode[1];
		rData.aCurEtc.Copy(aCurEtc);
		rData.aCurData.Copy(aCurData);
		rData.bAvgNodal = bAvgNodal;
		rData.bMembMode = bMembMode;
		rData.nDispOpt = nDispOpt;
		rData.aCurSectPos.Copy(aCurSectPos);
		rData.aOutputStep.Copy(aOutputStep);
		rData.bAllFiberCell = bAllFiberCell;
		rData.aTendonGroup.Copy(aTendonGroup);
		rData.aStage.Copy(aStage);
	}
	void Set(T_UTBL_ACTIVE_D Data)
	{
		nKeyType = Data.nKeyType;
		nChildType = Data.nChildType;
		aKey.Copy(Data.aKey);
		aLoad.Copy(Data.aLoad);
		aLkind.Copy(Data.aLkind);
		aMinMax.Copy(Data.aMinMax);
		aStep.Copy(Data.aStep);
		aLoadStep.Copy(Data.aLoadStep);
		aPart.Copy(Data.aPart);
		bNode[0] = Data.bNode[0];
		bNode[1] = Data.bNode[1];
		aCurEtc.Copy(Data.aCurEtc);
		aCurData.Copy(Data.aCurData);
		bAvgNodal = Data.bAvgNodal;
		bMembMode = Data.bMembMode;
		nDispOpt = Data.nDispOpt;
		aCurSectPos.Copy(Data.aCurSectPos);
		aOutputStep.Copy(Data.aOutputStep);
		bAllFiberCell = Data.bAllFiberCell;
		aTendonGroup.Copy(Data.aTendonGroup);
		aStage.Copy(Data.aStage);
	}
	T_UTBL_ACTIVE_D_CH& operator=(const T_UTBL_ACTIVE_D_CH &src)
	{
		nKeyType = src.nKeyType;
		nChildType = src.nChildType;
		aKey.Copy(src.aKey);
		aLoad.Copy(src.aLoad);
		aLkind.Copy(src.aLkind);
		aMinMax.Copy(src.aMinMax);
		aStep.Copy(src.aStep);
		aLoadStep.Copy(src.aLoadStep);
		aPart.Copy(src.aPart);
		bNode[0] = src.bNode[0];
		bNode[1] = src.bNode[1];
		aCurEtc.Copy(src.aCurEtc);
		aCurData.Copy(src.aCurData);
		bAvgNodal = src.bAvgNodal;
		bMembMode = src.bMembMode;
		nDispOpt = src.nDispOpt;
		aCurSectPos.Copy(src.aCurSectPos);
		aOutputStep.Copy(src.aOutputStep);
		bAllFiberCell = src.bAllFiberCell;
		aTendonGroup.Copy(src.aTendonGroup);
		aStage.Copy(src.aStage);

		return *this;
	}
	T_UTBL_ACTIVE_D_CH(const T_UTBL_ACTIVE_D_CH& src) { *this = src; }
};

struct T_UTBL_ACTIVE_D_CH_RW_845
{
	int nKeyType  ;
	int nChildType;
	int nCount_aKey;
	int nCount_aLoad;
	int nCount_aLkind;
	int nCount_aMinMax;
	int nCount_aStep;
	int nCount_aLoadStep;
	int nCount_aPart;
	BOOL bNode[2];
	int nCount_aCurEtc;
	int nCount_aCurData;
};
struct T_UTBL_ACTIVE_D_CH_RW_885
{
	int nKeyType  ;
	int nChildType;
	int nCount_aKey;
	int nCount_aLoad;
	int nCount_aLkind;
	int nCount_aMinMax;
	int nCount_aStep;
	int nCount_aLoadStep;
	int nCount_aPart;
	BOOL bNode[2];
	int nCount_aCurEtc;
	int nCount_aCurData;
	BOOL bAvgNodal;
};
struct T_UTBL_ACTIVE_D_CH_RW_945
{
	int nKeyType;
	int nChildType;
	int nCount_aKey;
	int nCount_aLoad;
	int nCount_aLkind;
	int nCount_aMinMax;
	int nCount_aStep;
	int nCount_aLoadStep;
	int nCount_aPart;
	BOOL bNode[2];
	int nCount_aCurEtc;
	int nCount_aCurData;
	BOOL bAvgNodal;
	BOOL bMembMode;
};
struct T_UTBL_ACTIVE_D_CH_RW_950
{
	int nKeyType;
	int nChildType;
	int nCount_aKey;
	int nCount_aLoad;
	int nCount_aLkind;
	int nCount_aMinMax;
	int nCount_aStep;
	int nCount_aLoadStep;
	int nCount_aPart;
	BOOL bNode[2];
	int nCount_aCurEtc;
	int nCount_aCurData;
	BOOL bAvgNodal;
	BOOL bMembMode;
	int nDispOpt;
};

struct T_UTBL_ACTIVE_D_CH_RW_960
{
	int nKeyType;
	int nChildType;
	int nCount_aKey;
	int nCount_aLoad;
	int nCount_aLkind;
	int nCount_aMinMax;
	int nCount_aStep;
	int nCount_aLoadStep;
	int nCount_aPart;
	BOOL bNode[2];
	int nCount_aCurEtc;
	int nCount_aCurData;
	BOOL bAvgNodal;
	BOOL bMembMode;
	int nDispOpt;
	int nCount_aCurSectPos;
	int nCount_aOutputStep;
	BOOL bAllFiberCell;
};

struct T_UTBL_ACTIVE_D_CH_RW
{
	int nKeyType  ;
	int nChildType;
	int nCount_aKey;
	int nCount_aLoad;
	int nCount_aLkind;
	int nCount_aMinMax;
	int nCount_aStep;
	int nCount_aLoadStep;
	int nCount_aPart;
	BOOL bNode[2];
	int nCount_aCurEtc;
	int nCount_aCurData;
	BOOL bAvgNodal;
	BOOL bMembMode;
	int nDispOpt;
	int nCount_aCurSectPos;
	int nCount_aOutputStep;
	BOOL bAllFiberCell;
	int nCount_aTendonGroup;
	int nCount_aStage;
	void Get(T_UTBL_ACTIVE_D_CH &rData)
	{
		rData.nKeyType = nKeyType;
		rData.nChildType = nChildType;
		rData.bNode[0]      = bNode[0];
		rData.bNode[1]      = bNode[1];
		rData.bAvgNodal = bAvgNodal;
		rData.bMembMode	= bMembMode;
		rData.nDispOpt = nDispOpt;
		rData.bAllFiberCell = bAllFiberCell;

		rData.aKey.SetSize(nCount_aKey);
		rData.aLoad.SetSize(nCount_aLoad);
		rData.aLkind.SetSize(nCount_aLkind);
		rData.aMinMax.SetSize(nCount_aMinMax);
		rData.aStep.SetSize(nCount_aStep);
		rData.aLoadStep.SetSize(nCount_aLoadStep);
		rData.aPart.SetSize(nCount_aPart);
		rData.aCurEtc.SetSize(nCount_aCurEtc);
		rData.aCurData.SetSize(nCount_aCurData);
		rData.aCurSectPos.SetSize(nCount_aCurSectPos);
		rData.aOutputStep.SetSize(nCount_aOutputStep);
		rData.aTendonGroup.SetSize(nCount_aTendonGroup);
		rData.aStage.SetSize(nCount_aStage);
	}
	void Set(T_UTBL_ACTIVE_D_CH Data)
	{
		nKeyType = Data.nKeyType;
		nChildType = Data.nChildType;
		bNode[0]      = Data.bNode[0];
		bNode[1]      = Data.bNode[1];
		bAvgNodal     = Data.bAvgNodal;
		bMembMode = Data.bMembMode;
		nDispOpt = Data.nDispOpt;
		bAllFiberCell = Data.bAllFiberCell;

		nCount_aKey = (int)Data.aKey.GetSize();
		nCount_aLoad = (int)Data.aLoad.GetSize();
		nCount_aLkind = (int)Data.aLkind.GetSize();
		nCount_aMinMax = (int)Data.aMinMax.GetSize();
		nCount_aStep = (int)Data.aStep.GetSize();
		nCount_aLoadStep = (int)Data.aLoadStep.GetSize();
		nCount_aPart = (int)Data.aPart.GetSize();
		nCount_aCurEtc = (int)Data.aCurEtc.GetSize();
		nCount_aCurData = (int)Data.aCurData.GetSize();
		nCount_aCurSectPos = (int)Data.aCurSectPos.GetSize();
		nCount_aOutputStep = (int)Data.aOutputStep.GetSize();
		nCount_aTendonGroup = (int)Data.aTendonGroup.GetSize();
		nCount_aStage = (int)Data.aStage.GetSize();
	}	
	void Convert845(T_UTBL_ACTIVE_D_CH_RW_845& rData)
	{
		nKeyType = rData.nKeyType;
		nChildType = rData.nChildType;
		bNode[0]      = rData.bNode[0];
		bNode[1]      = rData.bNode[1];
		bAvgNodal  = FALSE;
		bMembMode = FALSE;
		nDispOpt = 0;
		bAllFiberCell = FALSE;

		nCount_aKey      = rData.nCount_aKey;     
		nCount_aLoad     = rData.nCount_aLoad;    
		nCount_aLkind    = rData.nCount_aLkind;   
		nCount_aMinMax   = rData.nCount_aMinMax;  
		nCount_aStep     = rData.nCount_aStep;    
		nCount_aLoadStep = rData.nCount_aLoadStep;
		nCount_aPart     = rData.nCount_aPart;    
		nCount_aCurEtc   = rData.nCount_aCurEtc;  
		nCount_aCurData  = rData.nCount_aCurData;
		nCount_aCurSectPos = 0;
		nCount_aOutputStep = 0;
		nCount_aTendonGroup = 0;
		nCount_aStage = 0;

//     nCount_aKey = rData.aKey.GetSize();
//     nCount_aLoad = rData.aLoad.GetSize();
//     nCount_aLkind = rData.aLkind.GetSize();
//     nCount_aMinMax = rData.aMinMax.GetSize();
//     nCount_aStep = rData.aStep.GetSize();
//     nCount_aLoadStep = rData.aLoadStep.GetSize();
//     nCount_aPart = rData.aPart.GetSize();
//     nCount_aCurEtc = rData.aCurEtc.GetSize();
//     nCount_aCurData = rData.aCurData.GetSize();

	}
	void Convert885(T_UTBL_ACTIVE_D_CH_RW_885& rData)
	{
		nKeyType = rData.nKeyType;
		nChildType = rData.nChildType;
		bNode[0] = rData.bNode[0];
		bNode[1] = rData.bNode[1];
		bAvgNodal = rData.bAvgNodal;
		bMembMode = FALSE;
		nDispOpt = 0;
		bAllFiberCell = FALSE;

		nCount_aKey      = rData.nCount_aKey;     
		nCount_aLoad     = rData.nCount_aLoad;    
		nCount_aLkind    = rData.nCount_aLkind;   
		nCount_aMinMax   = rData.nCount_aMinMax;  
		nCount_aStep     = rData.nCount_aStep;    
		nCount_aLoadStep = rData.nCount_aLoadStep;
		nCount_aPart     = rData.nCount_aPart;    
		nCount_aCurEtc   = rData.nCount_aCurEtc;  
		nCount_aCurData  = rData.nCount_aCurData;
		nCount_aCurSectPos = 0;
		nCount_aOutputStep = 0;
		nCount_aTendonGroup = 0;
		nCount_aStage = 0;
	}
	void Convert945(T_UTBL_ACTIVE_D_CH_RW_945& rData)
	{
		nKeyType = rData.nKeyType;
		nChildType = rData.nChildType;
		bNode[0] = rData.bNode[0];
		bNode[1] = rData.bNode[1];
		bAvgNodal = rData.bAvgNodal;
		bMembMode = FALSE;
		nDispOpt = 0;
		bAllFiberCell = FALSE;

		nCount_aKey = rData.nCount_aKey;
		nCount_aLoad = rData.nCount_aLoad;
		nCount_aLkind = rData.nCount_aLkind;
		nCount_aMinMax = rData.nCount_aMinMax;
		nCount_aStep = rData.nCount_aStep;
		nCount_aLoadStep = rData.nCount_aLoadStep;
		nCount_aPart = rData.nCount_aPart;
		nCount_aCurEtc = rData.nCount_aCurEtc;
		nCount_aCurData = rData.nCount_aCurData;
		nCount_aCurSectPos = 0;
		nCount_aOutputStep = 0;
		nCount_aTendonGroup = 0;
		nCount_aStage = 0;
	}
	void Convert950(T_UTBL_ACTIVE_D_CH_RW_950& rData)
	{
		nKeyType = rData.nKeyType;
		nChildType = rData.nChildType;
		bNode[0] = rData.bNode[0];
		bNode[1] = rData.bNode[1];
		bAvgNodal = rData.bAvgNodal;
		bMembMode = FALSE;
		nDispOpt = 0;
		bAllFiberCell = FALSE;

		nCount_aKey = rData.nCount_aKey;
		nCount_aLoad = rData.nCount_aLoad;
		nCount_aLkind = rData.nCount_aLkind;
		nCount_aMinMax = rData.nCount_aMinMax;
		nCount_aStep = rData.nCount_aStep;
		nCount_aLoadStep = rData.nCount_aLoadStep;
		nCount_aPart = rData.nCount_aPart;
		nCount_aCurEtc = rData.nCount_aCurEtc;
		nCount_aCurData = rData.nCount_aCurData;
		nCount_aCurSectPos = 0;
		nCount_aOutputStep = 0;
		nCount_aTendonGroup = 0;
		nCount_aStage = 0;
	}
	void Convert960(T_UTBL_ACTIVE_D_CH_RW_960& rData)
	{
		nKeyType = rData.nKeyType;
		nChildType = rData.nChildType;
		bNode[0] = rData.bNode[0];
		bNode[1] = rData.bNode[1];
		bAvgNodal = rData.bAvgNodal;
		bMembMode = FALSE;
		nDispOpt = 0;
		bAllFiberCell = FALSE;

		nCount_aKey = rData.nCount_aKey;
		nCount_aLoad = rData.nCount_aLoad;
		nCount_aLkind = rData.nCount_aLkind;
		nCount_aMinMax = rData.nCount_aMinMax;
		nCount_aStep = rData.nCount_aStep;
		nCount_aLoadStep = rData.nCount_aLoadStep;
		nCount_aPart = rData.nCount_aPart;
		nCount_aCurEtc = rData.nCount_aCurEtc;
		nCount_aCurData = rData.nCount_aCurData;
		nCount_aCurSectPos = rData.nCount_aCurSectPos;
		nCount_aOutputStep = rData.nCount_aOutputStep;
		nCount_aTendonGroup = 0;
		nCount_aStage = 0;
	}
};

struct T_UTBL_DESIGN_RC_D
{
	UINT MembK;    // Member Key

	double  dFlexureDist[6];       // Flexure - Dist
	CString strFlexureBarDia1[6];  // Flexure - 1Size Dia
	CString strFlexureBarDia2[6];  // Flexure - 2Size Dia
	double  dFlexureNum[6];        // Flexure - Num
	double  dFlexureCTC[6];        // Flexure - C.T.C
	CString strShearBarDia[6];     // Shear - Dia
	double  dShearLEngth[6];       // Shear - 다리수
	double  dShearSpacing[6];      // Shear - 간격(s)

	T_UTBL_DESIGN_RC_D() {Initialize();}
	void Initialize()
	{
		MembK = 0;
		for(int i=0; i<6; i++)
		{
			dFlexureDist[i] = 0.0;
			strFlexureBarDia1[i] = "";
			strFlexureBarDia2[i] = "";
			dFlexureNum[i] = 0.0;
			dFlexureCTC[i] = 0.0;
			strShearBarDia[i] = "";
			dShearLEngth[i] = 0.0;
			dShearSpacing[i] = 0.0;
		}
	}
	T_UTBL_DESIGN_RC_D& operator=(const T_UTBL_DESIGN_RC_D &src)
	{
		MembK = src.MembK;
		for(int i=0; i<6; i++)
		{
			dFlexureDist[i] = src.dFlexureDist[i];
			strFlexureBarDia1[i] = src.strFlexureBarDia1[i];
			strFlexureBarDia2[i] = src.strFlexureBarDia2[i];
			dFlexureNum[i] = src.dFlexureNum[i];
			dFlexureCTC[i] = src.dFlexureCTC[i];
			strShearBarDia[i] = src.strShearBarDia[i];
			dShearLEngth[i] = src.dShearLEngth[i];
			dShearSpacing[i] = src.dShearSpacing[i];
		}
		return *this;
	}
	T_UTBL_DESIGN_RC_D(const T_UTBL_DESIGN_RC_D &src) { *this = src; }
};

struct T_UTBL_DESIGN_RC_D_CH
{
	UINT MembK;    // Member Key

	double  dFlexureDist[6];           // Flexure - Dist
	char    strFlexureBarDia1[6][40];  // Flexure - 1Size Dia
	char    strFlexureBarDia2[6][40];  // Flexure - 2Size Dia
	double  dFlexureNum[6];            // Flexure - Num
	double  dFlexureCTC[6];            // Flexure - C.T.C
	char    strShearBarDia[6][40];     // Shear - Dia
	double  dShearLEngth[6];           // Shear - 다리수
	double  dShearSpacing[6];          // Shear - 간격(s)

	void ConvertToString(T_UTBL_DESIGN_RC_D& rData)
	{
		rData.MembK = MembK;
		
		for(int i=0; i<6; i++)
		{
			rData.dFlexureDist[i] = dFlexureDist[i];
			ConvertCharStr(strFlexureBarDia1[i], rData.strFlexureBarDia1[i], sizeof(strFlexureBarDia1[i]));
			ConvertCharStr(strFlexureBarDia2[i], rData.strFlexureBarDia2[i], sizeof(strFlexureBarDia2[i]));
			rData.dFlexureNum[i] = dFlexureNum[i];
			rData.dFlexureCTC[i] = dFlexureCTC[i];
			ConvertCharStr(strShearBarDia[i], rData.strShearBarDia[i], sizeof(strShearBarDia[i]));
			rData.dShearLEngth[i] = dShearLEngth[i];
			rData.dShearSpacing[i] = dShearSpacing[i];
		}
	}
	void ConvertToChar(T_UTBL_DESIGN_RC_D& Data)
	{
		MembK = Data.MembK;

		for(int i=0; i<6; i++)
		{
			dFlexureDist[i] = Data.dFlexureDist[i];      
			ConvertStrChar(Data.strFlexureBarDia1[i], strFlexureBarDia1[i], sizeof(strFlexureBarDia1[i]));
			ConvertStrChar(Data.strFlexureBarDia2[i], strFlexureBarDia2[i], sizeof(strFlexureBarDia2[i]));
			dFlexureNum[i] = Data.dFlexureNum[i];
			dFlexureCTC[i] = Data.dFlexureCTC[i];
			ConvertStrChar(Data.strShearBarDia[i], strShearBarDia[i], sizeof(strShearBarDia[i]));
			dShearLEngth[i] = Data.dShearLEngth[i];
			dShearSpacing[i] = Data.dShearSpacing[i];
		}
	}
};

struct T_UTBL_DESIGN_D
{
	int nRcsTypeKind;	    // BCHK, BDGN, CCHK, CDGN, RCHK, RDGN.
	int nSize;				    // RCSC_iLargeBeamChk=0, RCSC_iSmallBeamChk=1
	int nSortRadio;       // Sorted by Member(0) / Section(1)
	int nResultViewRadio; // Result View Option All(0) / OK(1) /NG(2)
	int nPrimaryRadio;    // Primary Sorting Option SECT(0) /MEMB(1)
	int nResRadio;        // Results Strength(0) /Serviceability(1)
	int nRCDesignKind;    // RC Beam Design - Flexure:0 , Shear:1
												// RC Column Design - Flexure:2 , Shear:3
												// RC Wall Design - Flexure:4 , Shear:5
												// RC Plate Design - Flexure:6 , Shear:7
	int nSTLDesignKind;   // Steel Design - KSCE-ADS05:0
	int nReBarType;       // Number(0) / C.T.C(1) 
	int nResultViewBERadio; // Result View B.E. Option All(0)/Req.(1)/Not Req.(2)
													// RC Wall Design, RC Wall Check 에서만 사용.
													// 다른 곳에서는 Defalt(0) 값으로 갖고 있으면 된다.
	int nMeshedDgn_ElemAvg;

	CArray<T_UTBL_DESIGN_RC_D, T_UTBL_DESIGN_RC_D&> aRCDesign; // RC Design Data

	T_UTBL_DESIGN_D() {Initialize();}
	void Initialize()
	{
		nRcsTypeKind = 0;
		nSize = 0;			
		nSortRadio = 0;
		nResultViewRadio = 0;
		nPrimaryRadio = 0;
		nResRadio = 0;
		nRCDesignKind = 0;
		nSTLDesignKind = 0;
		nReBarType = 0;
		nResultViewBERadio = 0;
		nMeshedDgn_ElemAvg = 0;

		aRCDesign.RemoveAll();
	}
	T_UTBL_DESIGN_D& operator=(const T_UTBL_DESIGN_D &src)
	{
		nRcsTypeKind = src.nRcsTypeKind;
		nSize = src.nSize;
		nSortRadio = src.nSortRadio;
		nResultViewRadio = src.nResultViewRadio;
		nPrimaryRadio = src.nPrimaryRadio;
		nResRadio = src.nResRadio;
		nRCDesignKind = src.nRCDesignKind;
		nSTLDesignKind = src.nSTLDesignKind;
		nReBarType = src.nReBarType;
		nResultViewBERadio = src.nResultViewBERadio;
		nMeshedDgn_ElemAvg = src.nMeshedDgn_ElemAvg;

		aRCDesign.Copy(src.aRCDesign);
		return *this;
	}
	T_UTBL_DESIGN_D(const T_UTBL_DESIGN_D &src) { *this = src; }
};

struct T_UTBL_DESIGN_D_CH
{
	int nRcsTypeKind;	    // BCHK, BDGN, CCHK, CDGN, RCHK, RDGN.
	int nSize;				    // RCSC_iLargeBeamChk=0, RCSC_iSmallBeamChk=1
	int nSortRadio;       // Sorted by Member(0) / Section(1)
	int nResultViewRadio; // Result View Option All(0) / OK(1) /NG(2)
	int nPrimaryRadio;    // Primary Sorting Option SECT(0) /MEMB(1)
	int nResRadio;        // Results Strength(0) /Serviceability(1)
	int nRCDesignKind;    // RC Beam Design - Flexure:0 , Shear:1
												// RC Column Design - Flexure:2 , Shear:3
												// RC Wall Design - Flexure:4 , Shear:5
												// RC Plate Design - Flexure:6 , Shear:7
	int nSTLDesignKind;   // Steel Design - KSCE-ADS05:0
	int nReBarType;       // Number(0) / C.T.C(1)
	int nResultViewBERadio; // Result View B.E. Option All(0)/Req.(1)/Not Req.(2)
													// RC Wall Design, RC Wall Check 에서만 사용.
													// 다른 곳에서는 Defalt(0) 값으로 갖고 있으면 된다.
	int nMeshedDgn_ElemAvg;

	CArray<T_UTBL_DESIGN_RC_D_CH, T_UTBL_DESIGN_RC_D_CH&> aRCDesign; // RC Design Data

	T_UTBL_DESIGN_D_CH() {}
	void Get(T_UTBL_DESIGN_D &rData)
	{
		rData.nRcsTypeKind = nRcsTypeKind;
		rData.nSize = nSize;
		rData.nSortRadio = nSortRadio;
		rData.nResultViewRadio = nResultViewRadio;
		rData.nPrimaryRadio = nPrimaryRadio;
		rData.nResRadio = nResRadio;
		rData.nRCDesignKind = nRCDesignKind;
		rData.nSTLDesignKind = nSTLDesignKind;
		rData.nReBarType = nReBarType;
		rData.nResultViewBERadio = nResultViewBERadio;
		rData.nMeshedDgn_ElemAvg = nMeshedDgn_ElemAvg;

		int nCount = (int)aRCDesign.GetSize();
		for(int i=0; i<nCount; i++)
		{
			aRCDesign[i].ConvertToString(rData.aRCDesign[i]);
		}
	}
	void Set(T_UTBL_DESIGN_D Data)
	{
		nRcsTypeKind = Data.nRcsTypeKind;
		nSize = Data.nSize;
		nSortRadio = Data.nSortRadio;
		nResultViewRadio = Data.nResultViewRadio;
		nPrimaryRadio = Data.nPrimaryRadio;
		nResRadio = Data.nResRadio;
		nRCDesignKind = Data.nRCDesignKind;
		nSTLDesignKind = Data.nSTLDesignKind;
		nReBarType = Data.nReBarType;
		nResultViewBERadio = Data.nResultViewBERadio;
		nMeshedDgn_ElemAvg = Data.nMeshedDgn_ElemAvg;

		int nCount = (int)aRCDesign.GetSize();
		for(int i=0; i<nCount; i++)
		{
			aRCDesign[i].ConvertToChar(Data.aRCDesign[i]);
		}
	}
	T_UTBL_DESIGN_D_CH& operator=(const T_UTBL_DESIGN_D_CH &src)
	{
		nRcsTypeKind = src.nRcsTypeKind;
		nSize = src.nSize;
		nSortRadio = src.nSortRadio;
		nResultViewRadio = src.nResultViewRadio;
		nPrimaryRadio = src.nPrimaryRadio;
		nResRadio = src.nResRadio;
		nRCDesignKind = src.nRCDesignKind;
		nSTLDesignKind = src.nSTLDesignKind;
		nReBarType = src.nReBarType;
		nResultViewBERadio = src.nResultViewBERadio;
		nMeshedDgn_ElemAvg = src.nMeshedDgn_ElemAvg;

		aRCDesign.Copy(src.aRCDesign);
		return *this;
	}
	T_UTBL_DESIGN_D_CH(const T_UTBL_DESIGN_D_CH& src) { *this = src; }
};

struct T_UTBL_DESIGN_D_CH_RW_821
{
	int nRcsTypeKind;	    // BCHK, BDGN, CCHK, CDGN, RCHK, RDGN.
	int nSize;		    		// RCSC_iLargeBeamChk=0, RCSC_iSmallBeamChk=1
	int nSortRadio;       // Sorted by Member(0) / Section(1)
	int nResultViewRadio; // Result View Option All(0) / OK(1) /NG(2)
	int nPrimaryRadio;    // Primary Sorting Option SECT(0) /MEMB(1)
	int nResRadio;        // Results Strength(0) /Serviceability(1)
	int nRCDesignKind;    // RC Beam Design - Flexure:0 , Shear:1
	// RC Column Design - Flexure:2 , Shear:3
	// RC Wall Design - Flexure:4 , Shear:5
	// RC Plate Design - Flexure:6 , Shear:7
	int nSTLDesignKind;   // Steel Design - KSCE-ADS05:0
	int nReBarType;       // Number(0) / C.T.C(1)
	int nResultViewBERadio; // Result View B.E. Option All(0)/Req.(1)/Not Req.(2)
	// RC Wall Design, RC Wall Check 에서만 사용.
	// 다른 곳에서는 Defalt(0) 값으로 갖고 있으면 된다.
	
	int nRCDesign; // RC Design Data  
};

struct T_UTBL_DESIGN_D_CH_RW
{
	int nRcsTypeKind;	    // BCHK, BDGN, CCHK, CDGN, RCHK, RDGN.
	int nSize;		    		// RCSC_iLargeBeamChk=0, RCSC_iSmallBeamChk=1
	int nSortRadio;       // Sorted by Member(0) / Section(1)
	int nResultViewRadio; // Result View Option All(0) / OK(1) /NG(2)
	int nPrimaryRadio;    // Primary Sorting Option SECT(0) /MEMB(1)
	int nResRadio;        // Results Strength(0) /Serviceability(1)
	int nRCDesignKind;    // RC Beam Design - Flexure:0 , Shear:1
												// RC Column Design - Flexure:2 , Shear:3
												// RC Wall Design - Flexure:4 , Shear:5
												// RC Plate Design - Flexure:6 , Shear:7
	int nSTLDesignKind;   // Steel Design - KSCE-ADS05:0
	int nReBarType;       // Number(0) / C.T.C(1)
	int nResultViewBERadio; // Result View B.E. Option All(0)/Req.(1)/Not Req.(2)
													// RC Wall Design, RC Wall Check 에서만 사용.
													// 다른 곳에서는 Defalt(0) 값으로 갖고 있으면 된다.
	int nMeshedDgn_ElemAvg;

	int nRCDesign; // RC Design Data

	void Get(T_UTBL_DESIGN_D_CH &rData)
	{
		rData.nRcsTypeKind = nRcsTypeKind;
		rData.nSize = nSize;
		rData.nSortRadio = nSortRadio;
		rData.nResultViewRadio = nResultViewRadio;
		rData.nPrimaryRadio = nPrimaryRadio;
		rData.nResRadio = nResRadio;
		rData.nRCDesignKind = nRCDesignKind;
		rData.nSTLDesignKind = nSTLDesignKind;
		rData.nReBarType = nReBarType;
		rData.nResultViewBERadio = nResultViewBERadio;
		rData.nMeshedDgn_ElemAvg = nMeshedDgn_ElemAvg;

		rData.aRCDesign.SetSize(nRCDesign);
	}
	void Set(T_UTBL_DESIGN_D_CH Data)
	{
		nRcsTypeKind = Data.nRcsTypeKind;
		nSize = Data.nSize;
		nSortRadio = Data.nSortRadio;
		nResultViewRadio = Data.nResultViewRadio;
		nPrimaryRadio = Data.nPrimaryRadio;
		nResRadio = Data.nResRadio;
		nRCDesignKind = Data.nRCDesignKind;
		nSTLDesignKind = Data.nSTLDesignKind;
		nReBarType = Data.nReBarType;
		nResultViewBERadio = Data.nResultViewBERadio;
		nMeshedDgn_ElemAvg = Data.nMeshedDgn_ElemAvg;

		nRCDesign = (int)Data.aRCDesign.GetSize();
	}
	void Convert821(T_UTBL_DESIGN_D_CH_RW_821& rData)
	{
		nRcsTypeKind        = rData.nRcsTypeKind;
		nSize               = rData.nSize;
		nSortRadio          = rData.nSortRadio;
		nResultViewRadio    = rData.nResultViewRadio;
		nPrimaryRadio       = rData.nPrimaryRadio;
		nResRadio           = rData.nResRadio;
		nRCDesignKind       = rData.nRCDesignKind;
		nSTLDesignKind      = rData.nSTLDesignKind;
		nReBarType          = rData.nReBarType;
		nResultViewBERadio  = rData.nResultViewBERadio;
		nRCDesign           = rData.nRCDesign;
		nMeshedDgn_ElemAvg = 0;
	}
};

struct T_UTBL_PUSHOVER_D
{
	UINT    PolcK;
	int     nStepForDemand;
	int     nStepForDemandByUser;
	int     nUserStep;
	double  dConfidenceFactor;
	BOOL    bComponent[4];
	BOOL    bGetStep;
	int     nDL;
	int     nSD;
	int     nNC;
	int     nMode;                    // nMode=0 : 선택한 Elem만, nMode=1 : All
	BOOL    bNotConsiConEffect;

	T_UTBL_PUSHOVER_D() {Initialize();}
	void Initialize()
	{
		// default value
		PolcK = 0;
		nStepForDemand = 1;
		nStepForDemandByUser = 1;
		nUserStep = 0;
		dConfidenceFactor = 1.0;
		for(int i=0 ; i<4 ; ++i) bComponent[i] = TRUE;
		bGetStep = FALSE;
		nDL = 0;
		nSD = 0;
		nNC = 0;
		nMode = 0;
		bNotConsiConEffect = FALSE;
	}
	T_UTBL_PUSHOVER_D& operator=(const T_UTBL_PUSHOVER_D &src)
	{
		PolcK                 = src.PolcK;
		nStepForDemand        = src.nStepForDemand;
		nStepForDemandByUser  = src.nStepForDemandByUser;
		nUserStep             = src.nUserStep;
		dConfidenceFactor     = src.dConfidenceFactor;
		for(int i=0 ; i<4 ; ++i) bComponent[i] = src.bComponent[i];
		bGetStep = src.bGetStep;
		nDL = src.nDL;
		nSD = src.nSD;
		nNC = src.nNC;
		nMode = src.nMode;
		bNotConsiConEffect = src.bNotConsiConEffect;
		return *this;
	}
	T_UTBL_PUSHOVER_D(const T_UTBL_PUSHOVER_D &src) { *this = src; }
};

struct T_UTBL_PUSHOVER_D_CH
{
	UINT    PolcK;
	int     nStepForDemand;
	int     nStepForDemandByUser;
	int     nUserStep;
	double  dConfidenceFactor;
	BOOL    bComponent[4];
	BOOL    bGetStep;
	int     nDL;
	int     nSD;
	int     nNC;
	int     nMode;                    // nMode=0 : 선택한 Elem만, nMode=1 : All
	BOOL    bNotConsiConEffect;

	T_UTBL_PUSHOVER_D_CH() {}
	void Get(T_UTBL_PUSHOVER_D &rData)
	{
		rData.PolcK = PolcK;
		rData.nStepForDemand = nStepForDemand;
		rData.nStepForDemandByUser = nStepForDemandByUser;
		rData.nUserStep = nUserStep;
		rData.dConfidenceFactor = dConfidenceFactor;
		for(int i=0 ; i<4 ; ++i) rData.bComponent[i] = bComponent[i];
		rData.bGetStep = bGetStep;
		rData.nDL = nDL;
		rData.nSD = nSD;
		rData.nNC = nNC;
		rData.nMode = nMode;
		rData.bNotConsiConEffect = bNotConsiConEffect;
	}
	void Set(T_UTBL_PUSHOVER_D Data)
	{
		PolcK = Data.PolcK;
		nStepForDemand = Data.nStepForDemand;
		nStepForDemandByUser = Data.nStepForDemandByUser;
		nUserStep = Data.nUserStep;
		dConfidenceFactor = Data.dConfidenceFactor;
		for(int i=0 ; i<4 ; ++i) bComponent[i] = Data.bComponent[i];
		bGetStep = Data.bGetStep;
		nDL = Data.nDL;
		nSD = Data.nSD;
		nNC = Data.nNC;
		nMode = Data.nMode;
		bNotConsiConEffect = Data.bNotConsiConEffect;
	}
	T_UTBL_PUSHOVER_D_CH& operator=(const T_UTBL_PUSHOVER_D_CH &src)
	{
		PolcK = src.PolcK;
		nStepForDemand = src.nStepForDemand;
		nStepForDemandByUser = src.nStepForDemandByUser;
		nUserStep = src.nUserStep;
		dConfidenceFactor = src.dConfidenceFactor;
		for(int i=0 ; i<4 ; ++i) bComponent[i] = src.bComponent[i];
		bGetStep = src.bGetStep;
		nDL = src.nDL;
		nSD = src.nSD;
		nNC = src.nNC;
		nMode = src.nMode;
		bNotConsiConEffect = src.bNotConsiConEffect;
		return *this;
	}
	T_UTBL_PUSHOVER_D_CH(const T_UTBL_PUSHOVER_D_CH& src) { *this = src; }
};

struct T_UTBL_PUSHOVER_D_CH_RW_955
{
	UINT    PolcK;
	int     nStepForDemand;
	int     nStepForDemandByUser;
	int     nUserStep;
	double  dConfidenceFactor;
	BOOL    bComponent[4];
	BOOL    bGetStep;
	int     nDL;
	int     nSD;
	int     nNC;
	int     nMode;                // nMode=0 : 선택한 Elem만, nMode=1 : All
};

struct T_UTBL_PUSHOVER_D_CH_RW
{
	UINT    PolcK;
	int     nStepForDemand;
	int     nStepForDemandByUser;
	int     nUserStep;
	double  dConfidenceFactor;
	BOOL    bComponent[4];
	BOOL    bGetStep;
	int     nDL;
	int     nSD;
	int     nNC;
	int     nMode;                // nMode=0 : 선택한 Elem만, nMode=1 : All
	BOOL    bNotConsiConEffect;
	
	void Get(T_UTBL_PUSHOVER_D_CH &rData)
	{
		rData.PolcK = PolcK;
		rData.nStepForDemand = nStepForDemand;
		rData.nStepForDemandByUser = nStepForDemandByUser;
		rData.nUserStep = nUserStep;
		rData.dConfidenceFactor = dConfidenceFactor;
		for(int i=0 ; i<4 ; ++i) rData.bComponent[i] = bComponent[i];
		rData.bGetStep = bGetStep;
		rData.nDL = nDL;
		rData.nSD = nSD;
		rData.nNC = nNC;
		rData.nMode = nMode;
		rData.bNotConsiConEffect = bNotConsiConEffect;
	}
	void Set(T_UTBL_PUSHOVER_D_CH Data)
	{
		PolcK = Data.PolcK;
		nStepForDemand = Data.nStepForDemand;
		nStepForDemandByUser = Data.nStepForDemandByUser;
		nUserStep = Data.nUserStep;
		dConfidenceFactor = Data.dConfidenceFactor;
		for(int i=0 ; i<4 ; ++i) bComponent[i] = Data.bComponent[i];
		bGetStep = Data.bGetStep;
		nDL = Data.nDL;
		nSD = Data.nSD;
		nNC = Data.nNC;
		nMode = Data.nMode;
		bNotConsiConEffect = Data.bNotConsiConEffect;
	}
	void Initialize()
	{
		// default value
		PolcK = 0;
		nStepForDemand = 1;
		nStepForDemandByUser = 1;
		nUserStep = 0;
		dConfidenceFactor = 1.0;
		for(int i=0 ; i<4 ; ++i) bComponent[i] = TRUE;
		bGetStep = FALSE;
		nDL = 0;
		nSD = 0;
		nNC = 0;
		nMode = 0;
		bNotConsiConEffect = FALSE;
	}
	void Convert955(T_UTBL_PUSHOVER_D_CH_RW_955& rData)
	{
		PolcK = rData.PolcK;
		nStepForDemand = rData.nStepForDemand;
		nStepForDemandByUser = rData.nStepForDemandByUser;
		nUserStep = rData.nUserStep;
		dConfidenceFactor = rData.dConfidenceFactor;
		for (int i = 0; i < 4; ++i) bComponent[i] = rData.bComponent[i];
		bGetStep = rData.bGetStep;
		nDL = rData.nDL;
		nSD = rData.nSD;
		nNC = rData.nNC;
		nMode = rData.nMode;
		bNotConsiConEffect = FALSE;
	}
};

struct T_UTBL_STORY_BETA_D
{
	T_STOR_K StorK;
	double   dBeta;
};

struct T_UTBL_STORY_LCOM_D
{
	unsigned int  AnalType;   //D_LCOM_???
	unsigned int  LoadCaseKey;
	double        Factor;
};

struct T_UTBL_STORY_D
{
	BOOL      bRMF;
	double    dRMF;     // Response Modification Factor
	double    dDAF;     // Deflection Amplification Factor
	double    dIMF;     // Importance Factor
	double    dSUF;     // Scale-up Factor
	double    dALR;     // Allowable Ratio
	double    dALR_ch;  // Allowable Value
	int       nFixUser; // 0:Fixed, 1:User
	T_UTBL_STORY_BETA_D aBeta[D_TB_STOR_BETA_CNT];
	T_UTBL_STORY_LCOM_D Combination[D_TB_STOR_LCOM_CNT];
	
	double    dAngle;
	int       nDrift;   // Story Drift Method
	int       nStiff;   // Story Stiffness Method
	int       nMethod;  // Torsional Irregularity Check
	T_NODE_K  NodeK[2]; // Torsional Irregularity Check

	BOOL      bMethod[7];      
	UINT      NodeKey[3];      // Drift of Selected Node Line인 경우 각 방향별 절점 Key
	BOOL      bSameSelNode[2]; // Drift of Selected Node Line인 경우의 Y-Dir및 Combined의 Same for X-Dir여부
	UINT      aNodeKey[3][D_TB_STOR_LCOM_CNT];  // Average Drift of Extreme Nodes Lines인 경우 각 방향별 절점 Key List, 임의로 우선 150개로 제한해두었음.
	BOOL      bSameAverage[2]; // Average Drift of Extreme Nodes Lines인 경우의 Y-Dir및 Combined의 Same for X-Dir여부
	BOOL      bUpStoryNodeK;   // maximum drift only : Maximum Drift가 발생하는 절점을 상부측에 해당하는 절점으로 저장할까?(중국 report에서만 사용한다는 군.)

	T_UTBL_STORY_D()
	{
		Initialize();
	}
	void Initialize()
	{
		bRMF = FALSE;
		dRMF = 1.0;
		dDAF = 1.0;
		dIMF = 1.5;
		dSUF = 1.0;
		nFixUser = 0;
		dALR = 0.015;
		dALR_ch = 550.;

		for(int i=0; i<D_TB_STOR_BETA_CNT; i++)
		{
			aBeta[i].StorK = 0;
			aBeta[i].dBeta = 1.0;
		}
		for(int i = 0; i<D_TB_STOR_LCOM_CNT; i++)
		{
			Combination[i].AnalType=0;  // D_LCOM_???
			Combination[i].LoadCaseKey=0;
			Combination[i].Factor=0.;
		}      

		dAngle    = 0.0;
		nDrift    = 0;
		nStiff    = 0;
		nMethod   = 0;
		NodeK[0]  = NodeK[1] = 0;

		for(int i = 0; i<7; i++) bMethod[i] = FALSE;
		for(int i = 0; i<3; i++) 
		{
			NodeKey[i] = 0;
			for(int k=0; k<D_TB_STOR_LCOM_CNT; k++)
				aNodeKey[i][k] = 0;
		}
		for(int i = 0; i<2; i++)
		{
			bSameSelNode[i] = TRUE;    
			bSameAverage[i] = TRUE;    
		}
		bUpStoryNodeK = FALSE;
	}
};

struct T_UTBL_VIEW_BY_LOAD_D
{
	int nMode;	//0, D_VIEW_MODE_BY_LC, D_VIEW_MODE_BY_MVI

	CArray<int, int>  aFldInfoIndex; // nCol<<16 | nRow

	CArray<UINT, UINT>  aLoad;   // load combination or load case key or mode key
	CArray<UINT, UINT>  aLkind;  // m_aLoad, m_aLkind pair. load type
	CArray<int, int>    aMinMax;   // -1 : min, 0 : normal, 1 : max 

	BOOL bUseAvgNodal;

	T_UTBL_VIEW_BY_LOAD_D() { Initialize(); }
	void Initialize()
	{
		// default value
		nMode = 0;

		aFldInfoIndex.RemoveAll();
		aLoad.RemoveAll();
		aLkind.RemoveAll();
		aMinMax.RemoveAll();

		bUseAvgNodal = FALSE;
	}
	T_UTBL_VIEW_BY_LOAD_D& operator=(const T_UTBL_VIEW_BY_LOAD_D &src)
	{
		nMode = src.nMode;

		aFldInfoIndex.Copy(src.aFldInfoIndex);
		aLoad.Copy(src.aLoad);
		aLkind.Copy(src.aLkind);
		aMinMax.Copy(src.aMinMax);

		bUseAvgNodal = src.bUseAvgNodal;

		return *this;
	}
	T_UTBL_VIEW_BY_LOAD_D(const T_UTBL_VIEW_BY_LOAD_D &src) { *this = src; }
};

struct T_UTBL_VIEW_BY_LOAD_D_RW
{
	int nMode;	//0, D_VIEW_MODE_BY_LC, D_VIEW_MODE_BY_MVI

	int nFldInfoIndex;

	int nLoad;   // load combination or load case key or mode key
	int nLkind;  // m_aLoad, m_aLkind pair. load type
	int nMinMax;   // -1 : min, 0 : normal, 1 : max 

	BOOL bUseAvgNodal;

	void Get(T_UTBL_VIEW_BY_LOAD_D &rData)
	{
		rData.nMode = nMode;
		rData.aFldInfoIndex.SetSize(nFldInfoIndex);
		rData.aLoad.SetSize(nLoad);
		rData.aLkind.SetSize(nLkind);
		rData.aMinMax.SetSize(nMinMax);
		rData.bUseAvgNodal = bUseAvgNodal;
	}
	void Set(T_UTBL_VIEW_BY_LOAD_D Data)
	{
		nMode = Data.nMode;
		nFldInfoIndex = Data.aFldInfoIndex.GetSize();
		nLoad = Data.aLoad.GetSize();
		nLkind = Data.aLkind.GetSize();
		nMinMax = Data.aMinMax.GetSize();
		bUseAvgNodal = Data.bUseAvgNodal;
	}
	void Initialize()
	{
		// default value
		nMode = 0;	//0, D_VIEW_MODE_BY_LC, D_VIEW_MODE_BY_MVI
		nFldInfoIndex = 0;
		nLoad = 0;   // load combination or load case key or mode key
		nLkind = 0;  // m_aLoad, m_aLkind pair. load type
		nMinMax = 0;   // -1 : min, 0 : normal, 1 : max 
		bUseAvgNodal = FALSE;
	}
};

#define   D_UTBL_TYPE_PRE   0 // 전처리 Table
#define   D_UTBL_TYPE_POST  1 // 후처리 Table
#define   D_UTBL_TYPE_PO    2 // Pushover Table
#define   D_UTBL_TYPE_DDGN  3 // Dialog내 Design Table
#define   D_UTBL_TYPE_DGN   4 // Design Table(전후처리 상관없이 보이는 table)
#define   D_UTBL_TYPE_PSC   5 // PSC Design Table (PSC 해석 후에 보이는 table)
#define   D_UTBL_TYPE_CMD   6 // Cmd Table
#define   D_UTBL_TYPE_BCG_PRE 7 // BCG 전용 전처리 Table
#define   D_UTBL_TYPE_BCG_POST 8 // BCG 전용 후처리 Table

#define T_UTBL_K unsigned int
struct T_UTBL_D
{
	UINT     nMenuID  ; // 테이블의 메뉴 ID
	int      nDSkind  ; // 테이블 종류 (Mode)
	CString  strTBKind; // 전처리 테이블 class kind (class CTBNode, class CTBElnk, ...)
	CString  strName  ; // Dynamic Report Table Name
	CString  strCaption;  // Caption
	BOOL     bAll     ; // Element or Node가 All인지 아닌지..
	//UINT     nGrupK   ; // Group Key (Actie Node or Element List를 Group으로 묶어 Key를 넘겨줌)
	CString  strGrupName; // Group Name (Name으로 비교, Name이 같으면 같은 group으로 간주함)
	int      nDataType; // None:0, Node:1, Element:2
	int      nTBMode  ; // 전처리, 후처리 Table 구분하기 위해. (D_UTBL_TYPE_XXXX)
											// 0: 전처리 Table, 1:후처리 Table, 2:Pushover Table, 3:Dialog내 Design Table, 4:Design Table(전후처리 상관없이 보이는 table), 5:PSC Design Table (PSC 해석 후에 보이는 table)

	// User Defined Unit(v831 추가)
	BOOL bDefinedUnit; // User defined unit system 사용 여부
	int  nUnitLength ; // bDefinedUnit==TRUE 일 때 사용
	int  nUnitForce  ;
	int  nUnitHeat 	 ;
	int  nUnitTemper ;

	T_UTBL_HEADER_D   header        ; // 헤더 정보
	T_UTBL_CELL_D     cell          ; // 셀 정보
	T_UTBL_SORT_D     sort          ; // Sorting 정보
	T_UTBL_STYLE_D    style         ; // Style 정보
	T_UTBL_ACTIVE_D   active        ; // Active Dlg 정보
	T_UTBL_DESIGN_D   design        ; // Design Table (Dialog) 정보
	T_UTBL_PUSHOVER_D pushover      ; // pushover Table 정보
	T_UTBL_STORY_D    story         ; // Story Table 정보, 795에 추가 20110706 KIMJM
	T_UTBL_VIEW_BY_LOAD_D viewByLoad; // View By LoadCase, View By Max Value 정보, 940 추가 20230201 pyj0827 'ㅅ'

	T_UTBL_D() { Initialize(); }
	void Initialize()
	{
		nMenuID = 0;
		nDSkind = 0; 
		strTBKind = "";
		strName = "";
		strCaption = "";
		bAll = FALSE;
		strGrupName = "";
		nDataType = 0;
		nTBMode = -1;
		bDefinedUnit = FALSE;
		nUnitLength  = 1;
		nUnitForce   = 1;
		nUnitHeat 	 = 1;
		nUnitTemper  = 1;

		header.Initialize();
		cell.Initialize();
		sort.Initialize();
		style.Initialize();
		active.Initialize();
		design.Initialize();
		pushover.Initialize();
		story.Initialize();
		viewByLoad.Initialize();
	}
	T_UTBL_D& operator=(const T_UTBL_D &src)
	{
		nMenuID = src.nMenuID;
		nDSkind = src.nDSkind;
		strTBKind = src.strTBKind;
		strName = src.strName;
		strCaption = src.strCaption;
		bAll = src.bAll;
		strGrupName = src.strGrupName;
		nDataType = src.nDataType;
		nTBMode = src.nTBMode;
		bDefinedUnit = src.bDefinedUnit;
		nUnitLength  = src.nUnitLength ;
		nUnitForce   = src.nUnitForce  ;
		nUnitHeat 	 = src.nUnitHeat 	 ;
		nUnitTemper  = src.nUnitTemper ;

		header = src.header;
		cell = src.cell;
		sort = src.sort;
		style = src.style;
		active = src.active;
		design = src.design;
		pushover = src.pushover;
		story = src.story;
		viewByLoad = src.viewByLoad;
		
		return *this;
	}
	T_UTBL_D(const T_UTBL_D &src) { *this = src; }

    int     GetUtblType() const { return nTBMode; }
    CString GetTBKind() const   { return strTBKind; }
    int     GetDSKind() const   { return nDSkind; }
};

struct T_UTBL_D_CH
{
	UINT     nMenuID  ; // 테이블의 메뉴 ID
	int      nDSkind  ; // 테이블 종류 (Mode)
	char     strTBKind[60]; // 전처리 테이블 class kind (class CTBNode, class CTBElnk, ...)
	char     strName[60]; // Dynamic Report Table Name
	char     strCaption[60];
	BOOL     bAll     ; // Element or Node가 All인지 아닌지..
	char     strGrupName[60];
	int      nDataType; // None:0, Node:1, Element:2
	int      nTBMode  ; // 전처리, 후처리 Table 구분하기 위해. (0: 전처리 Table, 1: 후처리 Table)

	// User Defined Unit(v831 추가)
	BOOL bDefinedUnit; // User defined unit system 사용 여부
	int  nUnitLength ; // bDefinedUnit==TRUE 일 때 사용
	int  nUnitForce  ;
	int  nUnitHeat 	 ;
	int  nUnitTemper ;

	T_UTBL_HEADER_D       header        ; // 헤더 정보
	T_UTBL_CELL_D         cell          ; // 셀 정보
	T_UTBL_SORT_D         sort          ; // Sorting 정보
	T_UTBL_STYLE_D_CH     style         ; // Style 정보
	T_UTBL_ACTIVE_D_CH    active        ; // Active Dlg 정보
	T_UTBL_DESIGN_D_CH    design        ; // Design Table (Dialog) 정보
	T_UTBL_PUSHOVER_D_CH  pushover      ; // Pushover Table 정보
	T_UTBL_STORY_D        story         ; // Story Table 정보, 795에 추가 20110706 KIMJM
	T_UTBL_VIEW_BY_LOAD_D viewByLoad	; // View By LoadCase, View By Max Value 정보, 940 추가 20230201 pyj0827 'ㅅ'

	T_UTBL_D_CH() {}
	void Get(T_UTBL_D &rData)
	{
		rData.nMenuID = nMenuID;
		rData.nDSkind = nDSkind;
		rData.bAll = bAll;
		rData.nDataType = nDataType;
		rData.nTBMode = nTBMode;

		rData.bDefinedUnit = bDefinedUnit;
		rData.nUnitLength  = nUnitLength ;
		rData.nUnitForce   = nUnitForce  ;
		rData.nUnitHeat 	 = nUnitHeat 	 ;
		rData.nUnitTemper  = nUnitTemper ;

		rData.header = header;
		rData.cell = cell;
		rData.sort = sort;

		ConvertCharStr(strTBKind, rData.strTBKind, sizeof(strTBKind));
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		ConvertCharStr(strGrupName, rData.strGrupName, sizeof(strGrupName));
		style.Get(rData.style);
		active.Get(rData.active);
		design.Get(rData.design);
		pushover.Get(rData.pushover);

		rData.story = story;
		rData.viewByLoad = viewByLoad;
	}
	void Set(T_UTBL_D Data)
	{
		nMenuID = Data.nMenuID;
		nDSkind = Data.nDSkind;
		bAll = Data.bAll;
		nDataType = Data.nDataType;
		nTBMode = Data.nTBMode;

		bDefinedUnit = Data.bDefinedUnit;
		nUnitLength  = Data.nUnitLength ;
		nUnitForce   = Data.nUnitForce  ;
		nUnitHeat 	 = Data.nUnitHeat 	;
		nUnitTemper  = Data.nUnitTemper ;

		header = Data.header;
		cell = Data.cell;
		sort = Data.sort;

		ConvertStrChar(Data.strTBKind, strTBKind, sizeof(strTBKind));
		ConvertStrChar(Data.strName, strName, sizeof(strName));
		ConvertStrChar(Data.strCaption, strCaption, sizeof(strCaption));
		ConvertStrChar(Data.strGrupName, strGrupName, sizeof(strGrupName));
		style.Set(Data.style);
		active.Set(Data.active);
		design.Set(Data.design);
		pushover.Set(Data.pushover);

		story = Data.story;
		viewByLoad = Data.viewByLoad;
	}
};

struct T_UTBL_D_CH_RW_785
{
	UINT     nMenuID  ; // 테이블의 메뉴 ID
	int      nDSkind  ; // 테이블 종류 (Mode)
	char     strTBKind[60]; // 전처리 테이블 class kind (class CTBNode, class CTBElnk, ...)
	char     strName[60]; // Dynamic Report Table Name
	char     strCaption[60];
	BOOL     bAll     ; // Element or Node가 All인지 아닌지..
	char     strGrupName[60];
	int      nDataType; // None:0, Node:1, Element:2
	int      nTBMode  ; // 전처리, 후처리 Table 구분하기 위해. (0: 전처리 Table, 1: 후처리 Table)
	
	T_UTBL_HEADER_D_791       header ; // 헤더 정보
	T_UTBL_CELL_D             cell   ; // 셀 정보
	T_UTBL_SORT_D             sort   ; // Sorting 정보
	T_UTBL_STYLE_D_CH_791     style  ; // Style 정보
	T_UTBL_ACTIVE_D_CH_RW_845     active ; // Active Dlg 정보
	T_UTBL_DESIGN_D_CH_RW_821 design ; // Design Table (Dialog) 정보
};

struct T_UTBL_D_CH_RW_791
{
	UINT     nMenuID  ; // 테이블의 메뉴 ID
	int      nDSkind  ; // 테이블 종류 (Mode)
	char     strTBKind[60]; // 전처리 테이블 class kind (class CTBNode, class CTBElnk, ...)
	char     strName[60]; // Dynamic Report Table Name
	char     strCaption[60];
	BOOL     bAll     ; // Element or Node가 All인지 아닌지..
	char     strGrupName[60];
	int      nDataType; // None:0, Node:1, Element:2
	int      nTBMode  ; // 전처리, 후처리 Table 구분하기 위해. (0: 전처리 Table, 1: 후처리 Table)
	
	T_UTBL_HEADER_D_791       header    ; // 헤더 정보
	T_UTBL_CELL_D             cell      ; // 셀 정보
	T_UTBL_SORT_D             sort      ; // Sorting 정보
	T_UTBL_STYLE_D_CH_791     style     ; // Style 정보
	T_UTBL_ACTIVE_D_CH_RW_845     active    ; // Active Dlg 정보
	T_UTBL_DESIGN_D_CH_RW_821 design    ; // Design Table (Dialog) 정보
	T_UTBL_PUSHOVER_D_CH_RW_955   pushover  ; // Pushover Table 정보
};

struct T_UTBL_D_CH_RW_821
{
	UINT     nMenuID  ; // 테이블의 메뉴 ID
	int      nDSkind  ; // 테이블 종류 (Mode)
	char     strTBKind[60]; // 전처리 테이블 class kind (class CTBNode, class CTBElnk, ...)
	char     strName[60]; // Dynamic Report Table Name
	char     strCaption[60];
	BOOL     bAll     ; // Element or Node가 All인지 아닌지..
	char     strGrupName[60];
	int      nDataType; // None:0, Node:1, Element:2
	int      nTBMode  ; // 전처리, 후처리 Table 구분하기 위해. (0: 전처리 Table, 1: 후처리 Table)
	
	T_UTBL_HEADER_D           header   ; // 헤더 정보
	T_UTBL_CELL_D             cell     ; // 셀 정보
	T_UTBL_SORT_D             sort     ; // Sorting 정보
	T_UTBL_STYLE_D_CH         style    ; // Style 정보
	T_UTBL_ACTIVE_D_CH_RW_845     active   ; // Active Dlg 정보
	T_UTBL_DESIGN_D_CH_RW_821 design   ; // Design Table (Dialog) 정보
	T_UTBL_PUSHOVER_D_CH_RW_955   pushover ; // Pushover Table 정보
	T_UTBL_STORY_D            story    ; // Story Table 정보, 795에 추가 20110706 KIMJM
};

struct T_UTBL_D_CH_RW_830
{
	UINT     nMenuID  ; // 테이블의 메뉴 ID
	int      nDSkind  ; // 테이블 종류 (Mode)
	char     strTBKind[60]; // 전처리 테이블 class kind (class CTBNode, class CTBElnk, ...)
	char     strName[60]; // Dynamic Report Table Name
	char     strCaption[60];
	BOOL     bAll     ; // Element or Node가 All인지 아닌지..
	char     strGrupName[60];
	int      nDataType; // None:0, Node:1, Element:2
	int      nTBMode  ; // 전처리, 후처리 Table 구분하기 위해. (0: 전처리 Table, 1: 후처리 Table)
	
	T_UTBL_HEADER_D         header        ; // 헤더 정보
	T_UTBL_CELL_D           cell          ; // 셀 정보
	T_UTBL_SORT_D           sort          ; // Sorting 정보
	T_UTBL_STYLE_D_CH       style         ; // Style 정보
	T_UTBL_ACTIVE_D_CH_RW_845   active        ; // Active Dlg 정보
	T_UTBL_DESIGN_D_CH_RW   design        ; // Design Table (Dialog) 정보
	T_UTBL_PUSHOVER_D_CH_RW_955 pushover      ; // Pushover Table 정보
	T_UTBL_STORY_D          story         ; // Story Table 정보, 795에 추가 20110706 KIMJM
};

struct T_UTBL_D_CH_RW_845
{
	UINT     nMenuID  ; // 테이블의 메뉴 ID
	int      nDSkind  ; // 테이블 종류 (Mode)
	char     strTBKind[60]; // 전처리 테이블 class kind (class CTBNode, class CTBElnk, ...)
	char     strName[60]; // Dynamic Report Table Name
	char     strCaption[60];
	BOOL     bAll     ; // Element or Node가 All인지 아닌지..
	char     strGrupName[60];
	int      nDataType; // None:0, Node:1, Element:2
	int      nTBMode  ; // 전처리, 후처리 Table 구분하기 위해. (0: 전처리 Table, 1: 후처리 Table)

	BOOL bDefinedUnit; // User defined unit system 사용 여부
	int  nUnitLength ; // bDefinedUnit==TRUE 일 때 사용
	int  nUnitForce  ;
	int  nUnitHeat 	 ;
	int  nUnitTemper ;

	T_UTBL_HEADER_D         header        ; // 헤더 정보
	T_UTBL_CELL_D           cell          ; // 셀 정보
	T_UTBL_SORT_D           sort          ; // Sorting 정보
	T_UTBL_STYLE_D_CH       style         ; // Style 정보
	T_UTBL_ACTIVE_D_CH_RW_845   active        ; // Active Dlg 정보
	T_UTBL_DESIGN_D_CH_RW   design        ; // Design Table (Dialog) 정보
	T_UTBL_PUSHOVER_D_CH_RW_955 pushover      ; // Pushover Table 정보
	T_UTBL_STORY_D          story         ; // Story Table 정보, 795에 추가 20110706 KIMJM
};

struct T_UTBL_D_CH_RW_885
{
	UINT     nMenuID  ; // 테이블의 메뉴 ID
	int      nDSkind  ; // 테이블 종류 (Mode)
	char     strTBKind[60]; // 전처리 테이블 class kind (class CTBNode, class CTBElnk, ...)
	char     strName[60]; // Dynamic Report Table Name
	char     strCaption[60];
	BOOL     bAll     ; // Element or Node가 All인지 아닌지..
	char     strGrupName[60];
	int      nDataType; // None:0, Node:1, Element:2
	int      nTBMode  ; // 전처리, 후처리 Table 구분하기 위해. (0: 전처리 Table, 1: 후처리 Table)

	BOOL bDefinedUnit; // User defined unit system 사용 여부
	int  nUnitLength ; // bDefinedUnit==TRUE 일 때 사용
	int  nUnitForce  ;
	int  nUnitHeat 	 ;
	int  nUnitTemper ;

	T_UTBL_HEADER_D         header        ; // 헤더 정보
	T_UTBL_CELL_D           cell          ; // 셀 정보
	T_UTBL_SORT_D           sort          ; // Sorting 정보
	T_UTBL_STYLE_D_CH       style         ; // Style 정보
	T_UTBL_ACTIVE_D_CH_RW_885   active        ; // Active Dlg 정보
	T_UTBL_DESIGN_D_CH_RW   design        ; // Design Table (Dialog) 정보
	T_UTBL_PUSHOVER_D_CH_RW_955 pushover      ; // Pushover Table 정보
	T_UTBL_STORY_D          story         ; // Story Table 정보, 795에 추가 20110706 KIMJM
};

struct T_UTBL_D_CH_RW_940
{
	UINT     nMenuID; // 테이블의 메뉴 ID
	int      nDSkind; // 테이블 종류 (Mode)
	char     strTBKind[60]; // 전처리 테이블 class kind (class CTBNode, class CTBElnk, ...)
	char     strName[60]; // Dynamic Report Table Name
	char     strCaption[60];
	BOOL     bAll; // Element or Node가 All인지 아닌지..
	char     strGrupName[60];
	int      nDataType; // None:0, Node:1, Element:2
	int      nTBMode; // 전처리, 후처리 Table 구분하기 위해. (0: 전처리 Table, 1: 후처리 Table)

	BOOL bDefinedUnit; // User defined unit system 사용 여부
	int  nUnitLength; // bDefinedUnit==TRUE 일 때 사용
	int  nUnitForce;
	int  nUnitHeat;
	int  nUnitTemper;

	T_UTBL_HEADER_D         header; // 헤더 정보
	T_UTBL_CELL_D           cell; // 셀 정보
	T_UTBL_SORT_D           sort; // Sorting 정보
	T_UTBL_STYLE_D_CH       style; // Style 정보
	T_UTBL_ACTIVE_D_CH_RW_945   active; // Active Dlg 정보
	T_UTBL_DESIGN_D_CH_RW   design; // Design Table (Dialog) 정보
	T_UTBL_PUSHOVER_D_CH_RW_955 pushover; // Pushover Table 정보
	T_UTBL_STORY_D          story; // Story Table 정보, 795에 추가 20110706 KIMJM
};

struct T_UTBL_D_CH_RW_945
{
	UINT     nMenuID; // 테이블의 메뉴 ID
	int      nDSkind; // 테이블 종류 (Mode)
	char     strTBKind[60]; // 전처리 테이블 class kind (class CTBNode, class CTBElnk, ...)
	char     strName[60]; // Dynamic Report Table Name
	char     strCaption[60];
	BOOL     bAll; // Element or Node가 All인지 아닌지..
	char     strGrupName[60];
	int      nDataType; // None:0, Node:1, Element:2
	int      nTBMode; // 전처리, 후처리 Table 구분하기 위해. (0: 전처리 Table, 1: 후처리 Table)

	BOOL bDefinedUnit; // User defined unit system 사용 여부
	int  nUnitLength; // bDefinedUnit==TRUE 일 때 사용
	int  nUnitForce;
	int  nUnitHeat;
	int  nUnitTemper;

	T_UTBL_HEADER_D				header; // 헤더 정보
	T_UTBL_CELL_D				cell; // 셀 정보
	T_UTBL_SORT_D				sort; // Sorting 정보
	T_UTBL_STYLE_D_CH			style; // Style 정보
	T_UTBL_ACTIVE_D_CH_RW_945	active; // Active Dlg 정보
	T_UTBL_DESIGN_D_CH_RW		design; // Design Table (Dialog) 정보
	T_UTBL_PUSHOVER_D_CH_RW_955		pushover; // Pushover Table 정보
	T_UTBL_STORY_D				story; // Story Table 정보, 795에 추가 20110706 KIMJM
	T_UTBL_VIEW_BY_LOAD_D_RW	viewByLoad; // View By LoadCase, View By Max Value 정보, 940 추가 20230201 pyj0827 'ㅅ'
};
struct T_UTBL_D_CH_RW_950
{
	UINT     nMenuID; // 테이블의 메뉴 ID
	int      nDSkind; // 테이블 종류 (Mode)
	char     strTBKind[60]; // 전처리 테이블 class kind (class CTBNode, class CTBElnk, ...)
	char     strName[60]; // Dynamic Report Table Name
	char     strCaption[60];
	BOOL     bAll; // Element or Node가 All인지 아닌지..
	char     strGrupName[60];
	int      nDataType; // None:0, Node:1, Element:2
	int      nTBMode; // 전처리, 후처리 Table 구분하기 위해. (0: 전처리 Table, 1: 후처리 Table)

	BOOL bDefinedUnit; // User defined unit system 사용 여부
	int  nUnitLength; // bDefinedUnit==TRUE 일 때 사용
	int  nUnitForce;
	int  nUnitHeat;
	int  nUnitTemper;

	T_UTBL_HEADER_D				header; // 헤더 정보
	T_UTBL_CELL_D				cell; // 셀 정보
	T_UTBL_SORT_D				sort; // Sorting 정보
	T_UTBL_STYLE_D_CH			style; // Style 정보
	T_UTBL_ACTIVE_D_CH_RW_950	active; // Active Dlg 정보
	T_UTBL_DESIGN_D_CH_RW		design; // Design Table (Dialog) 정보
	T_UTBL_PUSHOVER_D_CH_RW_955		pushover; // Pushover Table 정보
	T_UTBL_STORY_D				story; // Story Table 정보, 795에 추가 20110706 KIMJM
	T_UTBL_VIEW_BY_LOAD_D_RW	viewByLoad; // View By LoadCase, View By Max Value 정보, 940 추가 20230201 pyj0827 'ㅅ'
};

struct T_UTBL_D_CH_RW_955
{
	UINT     nMenuID; // 테이블의 메뉴 ID
	int      nDSkind; // 테이블 종류 (Mode)
	char     strTBKind[60]; // 전처리 테이블 class kind (class CTBNode, class CTBElnk, ...)
	char     strName[60]; // Dynamic Report Table Name
	char     strCaption[60];
	BOOL     bAll; // Element or Node가 All인지 아닌지..
	char     strGrupName[60];
	int      nDataType; // None:0, Node:1, Element:2
	int      nTBMode; // 전처리, 후처리 Table 구분하기 위해. (0: 전처리 Table, 1: 후처리 Table)

	BOOL bDefinedUnit; // User defined unit system 사용 여부
	int  nUnitLength; // bDefinedUnit==TRUE 일 때 사용
	int  nUnitForce;
	int  nUnitHeat;
	int  nUnitTemper;

	T_UTBL_HEADER_D				header; // 헤더 정보
	T_UTBL_CELL_D				cell; // 셀 정보
	T_UTBL_SORT_D				sort; // Sorting 정보
	T_UTBL_STYLE_D_CH			style; // Style 정보
	T_UTBL_ACTIVE_D_CH_RW_960	active; // Active Dlg 정보
	T_UTBL_DESIGN_D_CH_RW		design; // Design Table (Dialog) 정보
	T_UTBL_PUSHOVER_D_CH_RW_955		pushover; // Pushover Table 정보
	T_UTBL_STORY_D				story; // Story Table 정보, 795에 추가 20110706 KIMJM
	T_UTBL_VIEW_BY_LOAD_D_RW	viewByLoad; // View By LoadCase, View By Max Value 정보, 940 추가 20230201 pyj0827 'ㅅ'
};

struct T_UTBL_D_CH_RW_960
{
	UINT     nMenuID; // 테이블의 메뉴 ID
	int      nDSkind; // 테이블 종류 (Mode)
	char     strTBKind[60]; // 전처리 테이블 class kind (class CTBNode, class CTBElnk, ...)
	char     strName[60]; // Dynamic Report Table Name
	char     strCaption[60];
	BOOL     bAll; // Element or Node가 All인지 아닌지..
	char     strGrupName[60];
	int      nDataType; // None:0, Node:1, Element:2
	int      nTBMode; // 전처리, 후처리 Table 구분하기 위해. (0: 전처리 Table, 1: 후처리 Table)

	BOOL bDefinedUnit; // User defined unit system 사용 여부
	int  nUnitLength; // bDefinedUnit==TRUE 일 때 사용
	int  nUnitForce;
	int  nUnitHeat;
	int  nUnitTemper;

	T_UTBL_HEADER_D				header; // 헤더 정보
	T_UTBL_CELL_D				cell; // 셀 정보
	T_UTBL_SORT_D				sort; // Sorting 정보
	T_UTBL_STYLE_D_CH			style; // Style 정보
	T_UTBL_ACTIVE_D_CH_RW_960	active; // Active Dlg 정보
	T_UTBL_DESIGN_D_CH_RW		design; // Design Table (Dialog) 정보
	T_UTBL_PUSHOVER_D_CH_RW		pushover; // Pushover Table 정보
	T_UTBL_STORY_D				story; // Story Table 정보, 795에 추가 20110706 KIMJM
	T_UTBL_VIEW_BY_LOAD_D_RW	viewByLoad; // View By LoadCase, View By Max Value 정보, 940 추가 20230201 pyj0827 'ㅅ'
};

struct T_UTBL_D_CH_RW
{
	UINT     nMenuID  ; // 테이블의 메뉴 ID
	int      nDSkind  ; // 테이블 종류 (Mode)
	char     strTBKind[60]; // 전처리 테이블 class kind (class CTBNode, class CTBElnk, ...)
	char     strName[60]; // Dynamic Report Table Name
	char     strCaption[60];
	BOOL     bAll     ; // Element or Node가 All인지 아닌지..
	char     strGrupName[60];
	int      nDataType; // None:0, Node:1, Element:2
	int      nTBMode  ; // 전처리, 후처리 Table 구분하기 위해. (0: 전처리 Table, 1: 후처리 Table)
	
	BOOL bDefinedUnit; // User defined unit system 사용 여부
	int  nUnitLength ; // bDefinedUnit==TRUE 일 때 사용
	int  nUnitForce  ;
	int  nUnitHeat 	 ;
	int  nUnitTemper ;

	T_UTBL_HEADER_D				header        ; // 헤더 정보
	T_UTBL_CELL_D				cell          ; // 셀 정보
	T_UTBL_SORT_D				sort          ; // Sorting 정보
	T_UTBL_STYLE_D_CH			style         ; // Style 정보
	T_UTBL_ACTIVE_D_CH_RW		active        ; // Active Dlg 정보
	T_UTBL_DESIGN_D_CH_RW		design        ; // Design Table (Dialog) 정보
	T_UTBL_PUSHOVER_D_CH_RW		pushover      ; // Pushover Table 정보
	T_UTBL_STORY_D				story         ; // Story Table 정보, 795에 추가 20110706 KIMJM
	T_UTBL_VIEW_BY_LOAD_D_RW	viewByLoad	  ; // View By LoadCase, View By Max Value 정보, 940 추가 20230201 pyj0827 'ㅅ'

	void Get(T_UTBL_D_CH &rData)
	{
		rData.nMenuID = nMenuID;
		rData.nDSkind = nDSkind;
		rData.bAll = bAll;
		rData.nDataType = nDataType;
		rData.nTBMode = nTBMode;
		
		rData.bDefinedUnit = bDefinedUnit;
		rData.nUnitLength  = nUnitLength ;
		rData.nUnitForce   = nUnitForce  ;
		rData.nUnitHeat 	 = nUnitHeat 	 ;
		rData.nUnitTemper  = nUnitTemper ;

		rData.header = header;
		rData.cell = cell;
		rData.sort = sort;
		rData.style = style;
		memcpy(rData.strTBKind, strTBKind, sizeof(strTBKind));
		memcpy(rData.strName, strName, sizeof(strName));
		memcpy(rData.strCaption, strCaption, sizeof(strCaption));
		memcpy(rData.strGrupName, strGrupName, sizeof(strGrupName));
		
		active.Get(rData.active);
		design.Get(rData.design);
		pushover.Get(rData.pushover);

		rData.story = story;

		viewByLoad.Get(rData.viewByLoad);
	}
	void Set(T_UTBL_D_CH Data)
	{
		nMenuID = Data.nMenuID;
		nDSkind = Data.nDSkind;
		bAll = Data.bAll;
		nDataType = Data.nDataType;
		nTBMode = Data.nTBMode;
		
		bDefinedUnit = Data.bDefinedUnit;
		nUnitLength  = Data.nUnitLength ;
		nUnitForce   = Data.nUnitForce  ;
		nUnitHeat 	 = Data.nUnitHeat 	;
		nUnitTemper  = Data.nUnitTemper ;

		header = Data.header;
		cell = Data.cell;
		sort = Data.sort;
		style = Data.style;
		memcpy(strTBKind, Data.strTBKind, sizeof(strTBKind));
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		memcpy(strGrupName, Data.strGrupName, sizeof(strGrupName));
		
		active.Set(Data.active);
		design.Set(Data.design);
		pushover.Set(Data.pushover);

		story = Data.story;

		viewByLoad.Set(Data.viewByLoad);
	}
	void Convert785(T_UTBL_D_CH_RW_785& rData)
	{
		nMenuID = rData.nMenuID;
		nDSkind = rData.nDSkind;
		bAll = rData.bAll;
		nDataType = rData.nDataType;
		nTBMode = rData.nTBMode;
		
		header.Convert791(rData.header);
		cell = rData.cell;
		sort = rData.sort;
		style.Convert791(rData.style);
		memcpy(strTBKind, rData.strTBKind, sizeof(strTBKind));
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strCaption, rData.strCaption, sizeof(strCaption));
		memcpy(strGrupName, rData.strGrupName, sizeof(strGrupName));
		
		active.Convert845(rData.active);
		design.Convert821(rData.design);
		pushover.Initialize();
		story.Initialize();
		viewByLoad.Initialize();

		bDefinedUnit = FALSE;
		nUnitLength  = 1;
		nUnitForce   = 1;
		nUnitHeat 	 = 1;
		nUnitTemper  = 1;
	}
	void Convert791(T_UTBL_D_CH_RW_791& rData)
	{
		nMenuID = rData.nMenuID;
		nDSkind = rData.nDSkind;
		bAll = rData.bAll;
		nDataType = rData.nDataType;
		nTBMode = rData.nTBMode;
		
		header.Convert791(rData.header);
		cell = rData.cell;
		sort = rData.sort;
		style.Convert791(rData.style);
		memcpy(strTBKind, rData.strTBKind, sizeof(strTBKind));
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strCaption, rData.strCaption, sizeof(strCaption));
		memcpy(strGrupName, rData.strGrupName, sizeof(strGrupName));
		
		active.Convert845(rData.active);
		design.Convert821(rData.design);
		pushover.Convert955(rData.pushover);
		story.Initialize();
		viewByLoad.Initialize();

		bDefinedUnit = FALSE;
		nUnitLength  = 1;
		nUnitForce   = 1;
		nUnitHeat 	 = 1;
		nUnitTemper  = 1;
	}
	void Convert821(T_UTBL_D_CH_RW_821& rData)
	{
		nMenuID = rData.nMenuID;
		nDSkind = rData.nDSkind;
		bAll = rData.bAll;
		nDataType = rData.nDataType;
		nTBMode = rData.nTBMode;
		
		header = rData.header;
		cell = rData.cell;
		sort = rData.sort;
		style = rData.style;
		memcpy(strTBKind, rData.strTBKind, sizeof(strTBKind));
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strCaption, rData.strCaption, sizeof(strCaption));
		memcpy(strGrupName, rData.strGrupName, sizeof(strGrupName));
		
		active.Convert845(rData.active);
		design.Convert821(rData.design);
		pushover.Convert955(rData.pushover);
		story = rData.story;
		viewByLoad.Initialize();

		bDefinedUnit = FALSE;
		nUnitLength  = 1;
		nUnitForce   = 1;
		nUnitHeat 	 = 1;
		nUnitTemper  = 1;
	}
	void Convert830(T_UTBL_D_CH_RW_830& rData)
	{
		nMenuID = rData.nMenuID;
		nDSkind = rData.nDSkind;
		bAll = rData.bAll;
		nDataType = rData.nDataType;
		nTBMode = rData.nTBMode;
		
		header = rData.header;
		cell = rData.cell;
		sort = rData.sort;
		style = rData.style;
		memcpy(strTBKind, rData.strTBKind, sizeof(strTBKind));
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strCaption, rData.strCaption, sizeof(strCaption));
		memcpy(strGrupName, rData.strGrupName, sizeof(strGrupName));
		
		active.Convert845(rData.active);
		design = rData.design;
		pushover.Convert955(rData.pushover);
		story = rData.story;
		viewByLoad.Initialize();

		bDefinedUnit = FALSE;
		nUnitLength  = 1;
		nUnitForce   = 1;
		nUnitHeat 	 = 1;
		nUnitTemper  = 1;
	}
	void Convert845(T_UTBL_D_CH_RW_845& rData)
	{
		nMenuID = rData.nMenuID;
		nDSkind = rData.nDSkind;
		bAll = rData.bAll;
		nDataType = rData.nDataType;
		nTBMode = rData.nTBMode;

		bDefinedUnit = rData.bDefinedUnit;
		nUnitLength  = rData.nUnitLength ;
		nUnitForce   = rData.nUnitForce  ;
		nUnitHeat 	 = rData.nUnitHeat 	;
		nUnitTemper  = rData.nUnitTemper ;

		header = rData.header;
		cell = rData.cell;
		sort = rData.sort;
		style = rData.style;
		memcpy(strTBKind, rData.strTBKind, sizeof(strTBKind));
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strCaption, rData.strCaption, sizeof(strCaption));
		memcpy(strGrupName, rData.strGrupName, sizeof(strGrupName));

		active.Convert845(rData.active);
		design = rData.design;
		pushover.Convert955(rData.pushover);
		story = rData.story;

		viewByLoad.Initialize();
	}
	void Convert885(T_UTBL_D_CH_RW_885& rData)
	{
		nMenuID = rData.nMenuID;
		nDSkind = rData.nDSkind;
		bAll = rData.bAll;
		nDataType = rData.nDataType;
		nTBMode = rData.nTBMode;

		bDefinedUnit = rData.bDefinedUnit;
		nUnitLength  = rData.nUnitLength ;
		nUnitForce   = rData.nUnitForce  ;
		nUnitHeat 	 = rData.nUnitHeat 	;
		nUnitTemper  = rData.nUnitTemper ;

		header = rData.header;
		cell = rData.cell;
		sort = rData.sort;
		style = rData.style;
		memcpy(strTBKind, rData.strTBKind, sizeof(strTBKind));
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strCaption, rData.strCaption, sizeof(strCaption));
		memcpy(strGrupName, rData.strGrupName, sizeof(strGrupName));

		active.Convert885(rData.active);
		design = rData.design;
		pushover.Convert955(rData.pushover);
		story = rData.story;

		viewByLoad.Initialize();
	}
	void Convert940(T_UTBL_D_CH_RW_940& rData)
	{
		nMenuID = rData.nMenuID;
		nDSkind = rData.nDSkind;
		bAll = rData.bAll;
		nDataType = rData.nDataType;
		nTBMode = rData.nTBMode;

		bDefinedUnit = rData.bDefinedUnit;
		nUnitLength = rData.nUnitLength;
		nUnitForce = rData.nUnitForce;
		nUnitHeat = rData.nUnitHeat;
		nUnitTemper = rData.nUnitTemper;

		header = rData.header;
		cell = rData.cell;
		sort = rData.sort;
		style = rData.style;
		memcpy(strTBKind, rData.strTBKind, sizeof(strTBKind));
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strCaption, rData.strCaption, sizeof(strCaption));
		memcpy(strGrupName, rData.strGrupName, sizeof(strGrupName));

		active.Convert945(rData.active);
		design = rData.design;
		pushover.Convert955(rData.pushover);
		story = rData.story;

		viewByLoad.Initialize();
	}
	void Convert945(T_UTBL_D_CH_RW_945& rData)
	{
		nMenuID = rData.nMenuID;
		nDSkind = rData.nDSkind;
		bAll = rData.bAll;
		nDataType = rData.nDataType;
		nTBMode = rData.nTBMode;

		bDefinedUnit = rData.bDefinedUnit;
		nUnitLength = rData.nUnitLength;
		nUnitForce = rData.nUnitForce;
		nUnitHeat = rData.nUnitHeat;
		nUnitTemper = rData.nUnitTemper;

		header = rData.header;
		cell = rData.cell;
		sort = rData.sort;
		style = rData.style;
		memcpy(strTBKind, rData.strTBKind, sizeof(strTBKind));
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strCaption, rData.strCaption, sizeof(strCaption));
		memcpy(strGrupName, rData.strGrupName, sizeof(strGrupName));

		active.Convert945(rData.active);
		design = rData.design;
		pushover.Convert955(rData.pushover);
		story = rData.story;
		viewByLoad = rData.viewByLoad;
	}
	void Convert950(T_UTBL_D_CH_RW_950& rData)
	{
		nMenuID = rData.nMenuID;
		nDSkind = rData.nDSkind;
		bAll = rData.bAll;
		nDataType = rData.nDataType;
		nTBMode = rData.nTBMode;

		bDefinedUnit = rData.bDefinedUnit;
		nUnitLength = rData.nUnitLength;
		nUnitForce = rData.nUnitForce;
		nUnitHeat = rData.nUnitHeat;
		nUnitTemper = rData.nUnitTemper;

		header = rData.header;
		cell = rData.cell;
		sort = rData.sort;
		style = rData.style;
		memcpy(strTBKind, rData.strTBKind, sizeof(strTBKind));
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strCaption, rData.strCaption, sizeof(strCaption));
		memcpy(strGrupName, rData.strGrupName, sizeof(strGrupName));

		active.Convert950(rData.active);
		design = rData.design;
		pushover.Convert955(rData.pushover);
		story = rData.story;
		viewByLoad = rData.viewByLoad;
	}
	void Convert955(T_UTBL_D_CH_RW_955& rData)
	{
		nMenuID = rData.nMenuID;
		nDSkind = rData.nDSkind;
		bAll = rData.bAll;
		nDataType = rData.nDataType;
		nTBMode = rData.nTBMode;

		bDefinedUnit = rData.bDefinedUnit;
		nUnitLength = rData.nUnitLength;
		nUnitForce = rData.nUnitForce;
		nUnitHeat = rData.nUnitHeat;
		nUnitTemper = rData.nUnitTemper;

		header = rData.header;
		cell = rData.cell;
		sort = rData.sort;
		style = rData.style;
		memcpy(strTBKind, rData.strTBKind, sizeof(strTBKind));
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strCaption, rData.strCaption, sizeof(strCaption));
		memcpy(strGrupName, rData.strGrupName, sizeof(strGrupName));

		active.Convert960(rData.active);
		design = rData.design;
		pushover.Convert955(rData.pushover);
		story = rData.story;
		viewByLoad = rData.viewByLoad;
	}
	void Convert960(T_UTBL_D_CH_RW_960& rData)
	{
		nMenuID = rData.nMenuID;
		nDSkind = rData.nDSkind;
		bAll = rData.bAll;
		nDataType = rData.nDataType;
		nTBMode = rData.nTBMode;

		bDefinedUnit = rData.bDefinedUnit;
		nUnitLength = rData.nUnitLength;
		nUnitForce = rData.nUnitForce;
		nUnitHeat = rData.nUnitHeat;
		nUnitTemper = rData.nUnitTemper;

		header = rData.header;
		cell = rData.cell;
		sort = rData.sort;
		style = rData.style;
		memcpy(strTBKind, rData.strTBKind, sizeof(strTBKind));
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strCaption, rData.strCaption, sizeof(strCaption));
		memcpy(strGrupName, rData.strGrupName, sizeof(strGrupName));

		active.Convert960(rData.active);
		design = rData.design;
		pushover = rData.pushover;
		story = rData.story;
		viewByLoad = rData.viewByLoad;
	}
};

struct T_UTBL_UDRD_D
{
	T_UTBL_K key ;
	T_UTBL_D data;
	void Initialize()
	{
		key  = 0;
		data .Initialize();
	}
	T_UTBL_UDRD_D() { Initialize(); }
	T_UTBL_UDRD_D(const T_UTBL_UDRD_D &src) { *this = src; }
	T_UTBL_UDRD_D& operator=(const T_UTBL_UDRD_D &src)
	{
		key  = src.key ;
		data = src.data;
		return *this;
	}
};
struct T_UTBL_UDRD_D_CH
{
	T_UTBL_K key;
	T_UTBL_D_CH data;
};
struct T_UTBL_UDRD_D_CH_RW_785
{
	T_UTBL_K key;
	T_UTBL_D_CH_RW_785 data;
};
struct T_UTBL_UDRD_D_CH_RW_791
{
	T_UTBL_K key;
	T_UTBL_D_CH_RW_791 data;
};
struct T_UTBL_UDRD_D_CH_RW_821
{
	T_UTBL_K key;
	T_UTBL_D_CH_RW_821 data;
};
struct T_UTBL_UDRD_D_CH_RW_830
{
	T_UTBL_K key;
	T_UTBL_D_CH_RW_830 data;
};
struct T_UTBL_UDRD_D_CH_RW_845
{
	T_UTBL_K key;
	T_UTBL_D_CH_RW_845 data;
};
struct T_UTBL_UDRD_D_CH_RW_885
{
	T_UTBL_K key;
	T_UTBL_D_CH_RW_885 data;
};
struct T_UTBL_UDRD_D_CH_RW_940
{
	T_UTBL_K key;
	T_UTBL_D_CH_RW_940 data;
};
struct T_UTBL_UDRD_D_CH_RW_945
{
	T_UTBL_K key;
	T_UTBL_D_CH_RW_945 data;
};
struct T_UTBL_UDRD_D_CH_RW_950
{
	T_UTBL_K key;
	T_UTBL_D_CH_RW_950 data;
};
struct T_UTBL_UDRD_D_CH_RW_955
{
	T_UTBL_K key;
	T_UTBL_D_CH_RW_955 data;
};
struct T_UTBL_UDRD_D_CH_RW_960
{
	T_UTBL_K key;
	T_UTBL_D_CH_RW_960 data;
};
struct T_UTBL_UDRD_D_CH_RW
{
	T_UTBL_K key;
	T_UTBL_D_CH_RW data;
	void GetUtbl(T_UTBL_K &UTBLK, T_UTBL_D_CH &UTBLD)
	{
		UTBLK = key;
		data.Get(UTBLD);
	}
	void SetUtbl(const T_UTBL_K &UTBLK, const T_UTBL_D_CH &UTBLD)
	{
		key = UTBLK;
		data.Set(UTBLD);
	}
	void Convert785(T_UTBL_UDRD_D_CH_RW_785& rData)
	{
		key = rData.key;
		data.Convert785(rData.data);
	}
	void Convert791(T_UTBL_UDRD_D_CH_RW_791& rData)
	{
		key = rData.key;
		data.Convert791(rData.data);
	}
	void Convert821(T_UTBL_UDRD_D_CH_RW_821& rData)
	{
		key = rData.key;
		data.Convert821(rData.data);
	}
	void Convert830(T_UTBL_UDRD_D_CH_RW_830& rData)
	{
		key = rData.key;
		data.Convert830(rData.data);
	}
	void Convert845(T_UTBL_UDRD_D_CH_RW_845& rData)
	{
		key = rData.key;
		data.Convert845(rData.data);
	}
	void Convert885(T_UTBL_UDRD_D_CH_RW_885& rData)
	{
		key = rData.key;
		data.Convert885(rData.data);
	}
	void Convert940(T_UTBL_UDRD_D_CH_RW_940& rData)
	{
		key = rData.key;
		data.Convert940(rData.data);
	}
	void Convert945(T_UTBL_UDRD_D_CH_RW_945& rData)
	{
		key = rData.key;
		data.Convert945(rData.data);
	}
	void Convert950(T_UTBL_UDRD_D_CH_RW_950& rData)
	{
		key = rData.key;
		data.Convert950(rData.data);
	}
	void Convert955(T_UTBL_UDRD_D_CH_RW_955& rData)
	{
		key = rData.key;
		data.Convert955(rData.data);
	}
	void Convert960(T_UTBL_UDRD_D_CH_RW_960& rData)
	{
		key = rData.key;
		data.Convert960(rData.data);
	}
};
struct T_UTBL_UNIT
{
	T_UTBL_UNIT()
	{
		NoUnit=D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEUTBL 53

#undef D_TB_COMP_CNT
#undef D_TB_HEADER_CNT
#undef D_TB_SORT_CNT
#undef D_TB_SPOS_CNT

//////////////////////////////////////////////////////////////////////////
// User Defined Chart
#define D_TYPE_START       0
#define D_TYPE_RS_FUNC     0  // RS Func.
#define D_TYPE_TH_FUNC     1  // TH Func.
#define D_TYPE_PO_FUNC     2  // PO Incremental Func.
#define D_TYPE_TD_FUNC     3  // Time Dependent Mat'l func
#define D_TYPE_TH_GRAPH    4  // TH Graph
#define D_TYPE_PO_GRAPH    5  // PO Graph
#define D_TYPE_TH_STORY    6  // TH Story Graph
#define D_TYPE_PO_STORY    7  // PO Story Graph
#define D_TYPE_PO_CURVE    8  // PO Curve
#define D_TYPE_EARTHQUAKE  9  // Earthquake RS
#define D_TYPE_SS_GRAPH   10  // Stage/Step History Graph
#define D_TYPE_HS_FUNC    11  // Heat Source Func
#define D_TYPE_AT_FUNC    12  // Ambient Temp. Func
#define D_TYPE_CC_FUNC    13  // Convection Coefficient Func.
#define D_TYPE_HH_DIAG    14  // Heat of Hydration Analysis Diagram.
#define D_TYPE_CS_GRAPH   15  // Column shortening for C.S
#define D_TYPE_TD_GRAPH   16  // Tendon Time-dependent Loss Graph
#define D_TYPE_PR_CURVE   17  // Primary Curve (Hinge Property dialog box)
#define D_TYPE_IT_CURVE   18  // Interaction Curve (Yield Surface Property dialog box)
#define D_TYPE_CD_DIAGR   19  // Plate Cutting Line Diagram
#define D_TYPE_PL_DIAGR   20  // Plate forces/Moment Cutting Diagram
#define D_TYPE_ST_DIAGR   21  // Plane Stress/Plate Stress Cutting Diagram
#define D_TYPE_PS_DIAGR   22  // Plane Strain Stress Cutting Diagram
#define D_TYPE_MD_RATIO   23  // Modal Damping Ratio based on Group Damping
#define D_TYPE_FC_GRAPH   24  // FCM Camber Graph
#define D_TYPE_GC_GRAPH   25  // General Camber Graph
#define D_TYPE_BG_DIAGR   26  // Bridge Girder Diagrams
#define D_TYPE_TM_FUNC    27  // Time Dependent Material Function
#define D_TYPE_TE_FUNC    28  // Time Dependent Material Function(comp. Strength)
#define D_TYPE_ST_FUNC    29  // Time Dependent Material Function
#define D_TYPE_WD_PROF    30  // Wind Load Profile
#define D_TYPE_SM_PROF    31  // Seismic Load Profile
#define D_TYPE_SB_GRAPH   32  // Suspension Bridge Analysis Control > Define Hanger Bottom Point
#define D_TYPE_PM_CD_CB   33  // Plastic Material > Concrete-Damage > Compressive Behavior
#define D_TYPE_PM_CD_TB   34  // Plastic Material > Concrete-Damage > Tensile Behavior
#define D_TYPE_TH_ENRG    35  // Time History Energy Graph(Structure)
#define D_TYPE_TH_EGRP    36  // Time History Energy Graph(Group)
#define D_TYPE_SN_PLATE   37  // Plate Strain
#define D_TYPE_SN_SOLID   38  // Plate Strain
#define D_TYPE_ML_FUNC    39  // Forces-Deformation Function
#define D_TYPE_FLUE_FUNC    40  // Fire Flue-gas Fun
#define D_TYPE_STTF_FUNC    41  // Fire Steel Time-Temp Fun
#define D_TYPE_RT_C_FUNC	42 //Rising Temperature Com
#define D_TYPE_RT_E_FUNC	43 //Rising Temperature E
#define D_TYPE_TH_STDR_GRAPH 44 // Time History Story Drift Ratio Graph

#define D_TYPE_END        D_TYPE_TH_STDR_GRAPH


struct T_UCHT_GRPH_D_871
{
	UINT nKeyX;       // TH : T_THRD_K or T_THIS_K,   PO : T_PORD_K or T_POLC_K
	BOOL bTimeorFunc;
	BOOL bMergeList;
	int  nGraphType; // 0: TH Graph 1:Modal Max Graph 2: Serviceability Check(1991) 3: Serviceability Check(2004) 4: Serviceability Check(Octave Band)
	int  nOctBandOption;
	double dMaxFreq;
	double dMinFreq;
	void Initialize()
	{
		nKeyX = 0;
		bTimeorFunc = FALSE;
		bMergeList = FALSE;
		nGraphType = 0;
		nOctBandOption = 0;
		dMaxFreq = 0.0;
		dMinFreq = 0.0;
	}
};

// for Time History Graph && Pushover Graph
struct T_UCHT_GRPH_D
{
	UINT nKeyX;       // TH : T_THRD_K or T_THIS_K,   PO : T_PORD_K or T_POLC_K
	BOOL bTimeorFunc;
	BOOL bMergeList;
	int  nGraphType; // 0: TH Graph 1:Modal Max Graph 2: Serviceability Check(1991) 3: Serviceability Check(2004) 4: Serviceability Check(Octave Band)
	int  nOctBandOption;
	double dMaxFreq;
	double dMinFreq;
	BOOL bCumulDisplByInitLoad;
	void Initialize()
	{
		nKeyX = 0;
		bTimeorFunc = FALSE;
		bMergeList = FALSE;
		nGraphType = 0;
		nOctBandOption = 0;
		dMaxFreq = 0.0;
		dMinFreq = 0.0;
		bCumulDisplByInitLoad=FALSE;
	}
	void Convert871(const T_UCHT_GRPH_D_871& src)
	{
		nKeyX          = src.nKeyX         ;
		bTimeorFunc    = src.bTimeorFunc   ;
		bMergeList     = src.bMergeList    ;
		nGraphType     = src.nGraphType    ;
		nOctBandOption = src.nOctBandOption;
		dMaxFreq       = src.dMaxFreq      ;
		dMinFreq       = src.dMinFreq      ;
		bCumulDisplByInitLoad=FALSE;
	}
};

// for TH Story Graph && PO Story Graph				
struct T_UCHT_STGH_D
{				
	int  nCase;   // TH =>0:Story Shear/Overturning Mom.(by Step) 1:Story Shear(Max.)  2:Shear Factor 3:Story Displ/Vel/Accel				
								// PO =>0:Story Shear Graph 1:Member Shear Graph 2:Story Shear/Drift/Drift Ratio 				
	UINT nLoadCaseKey;
	int  nResultType;
	int  nMnMxType;   //0: Min, 1:Max, 2:AbsMax
	BOOL bDirX;
	BOOL bDirY;
	int  nRefPoint;

	int  nLoadDir;
	int  nElemRadio;
	BOOL bShear;
	BOOL bDrift;
	BOOL bDriftRatio;
//   CString csGraphTitle;
	void Initialize()
	{
		nCase = 0;
		nLoadCaseKey = 0;
		nResultType = 0;
		nMnMxType = 0;
		bDirX = FALSE;
		bDirY = FALSE;
		nRefPoint = 0;
		nLoadDir = 0;
		nElemRadio = 0;
		bShear = FALSE;
		bDrift = FALSE;
		bDriftRatio = FALSE; 
//     csGraphTitle = "";
	}
};

// for Pushover Curve
struct T_UCHT_POCV_D
{				
	UINT nLoadCaseK;  // T_POLC_K				
	int  nPlotType;
	BOOL bDemandSpectra;
	BOOL bConstPeriod;
	double dSpectDamping[4];
	double dConstPeriod[4];
	BOOL bShowSimbol;
	BOOL bShowRefLine;
	BOOL bProAorB;
	BOOL bAutoRange;
	double dXMnRange;
	double dXMxRange;
	double dYMnRange;
	double dYMxRange;
	void Initialize()
	{
		nLoadCaseK = 0;
		nPlotType = 0;
		bDemandSpectra = FALSE;
		bConstPeriod = FALSE;
		int i;
		for(i=0 ; i<4 ; ++i) dSpectDamping[i] = 0.;
		for(i=0 ; i<4 ; ++i) dConstPeriod[i] = 0.;
		bShowSimbol = FALSE;
		bShowRefLine = FALSE;
		bProAorB = FALSE;
		bAutoRange = FALSE;
		dXMnRange = 0.;
		dXMxRange = 0.;
		dYMnRange = 0.;
		dYMxRange = 0.;
	}
};

// for Earthquake Response Spectrum
struct T_UCHT_EQRS_D
{
	int  nSpectrumType; // 0:Pseudo Acceleration 1:Pseudo Velocity 2:Absolute Acceleration 3:Relative Velocity 4: Relative Displacement 5:Combined D-V-A
	int  nDataType;
	int  nDampingNum;
	double  aOutPeriod[3]; // 0: From 1: To 2: Step
	double  aDampRatio[6];
	double  dAmpliScale;
	double  dTimeScale;
	double  dGrav;
	void Initialize()
	{
		nDataType = 0;
		nDampingNum = 0;
		nSpectrumType = 0;
		dAmpliScale = 0.0;
		dTimeScale = 0.0;
		dGrav = 0.0;
		int i;
		for(i=0 ; i<3 ; ++i) aOutPeriod[i] = 0.0;
		for(i=0 ; i<6 ; ++i) aDampRatio[i] = 0.0;
	}
};

struct T_UCHT_EQRS_D_UNIT
{
	T_UCHT_EQRS_D_UNIT()
	{
		dGrav = D_UNITSYS_BASE_LENGTH;
	}
	double  dGrav;
};

///// for Stage/Step History Graph
struct T_UCHT_STHG_D
{
	UINT nLoadCaseK;  // T_THIS_K
	int  nMultiType;  // F : MultiFunc. ,  T: MultiLCase
	int  nStepOpt;    // 0 : All Step,     1: Last Step
	int  nXOpt;       // 0 : Stage/Step,   1: Time 
	UINT nLoadCaseType;
	void Initialize()
	{
		nLoadCaseK = 0;
		nMultiType = 0;
		nStepOpt = 0;
		nXOpt = 0;
		nLoadCaseType = 0;
	}
};

///// for Heat of Hydration Analysis Diagram
struct T_UCHT_HHAD_D
{
	BOOL bGraph[5];
	int  nXAxisType;
	int	 nCRGrpType;
	int  nSelectGraph;
	void Initialize()
	{
		for(int i=0 ; i<5 ; ++i) bGraph[i] = FALSE;
		nXAxisType = 0;
		nCRGrpType = 0;
		nSelectGraph = 0;
	}
};

struct T_UCHT_BRGD_D_815
{
	int  nBStrsComp;
	int  nBStrsCompSub;
	int  nXAxisType;
	int  nDiaType;
	int  nIdx;
	int  nMomentComp;
	BOOL bAllStage;
	BOOL bDrawBoundLine;
	double dCompValue;
	double dTensValue;
};

struct T_UCHT_BRGD_D_821
{
	int    nBStrsComp     ;
	int    nBStrsCompSub  ;
	int    nXAxisType     ;
	int    nDiaType       ;
	int    nIdx           ;
	int    nMomentComp    ;
	BOOL   bAllStage      ;
	BOOL   bDrawBoundLine ;
	double dCompValue     ;
	double dTensValue     ;
	int    nBStrsComp7thDof;

	void Initialize()
	{
		nBStrsComp        = 0;
		nBStrsCompSub     = 0;
		nXAxisType        = 0;
		nDiaType          = 0;
		nIdx              = -1;
		nMomentComp       = 0;
		bAllStage         = FALSE;
		bDrawBoundLine    = FALSE;
		dCompValue        = 0.0;
		dTensValue        = 0.0;
		nBStrsComp7thDof  = 0;
	}
};

///// for Bridge Girder Diagram
struct T_UCHT_BRGD_BATCH_D
{
	char     strName[60]    ;
	BOOL     bIsBatch       ;
	T_KEY    bodyelem_grup_k;
	BOOL     bAllStage      ;
	BOOL     bReport        ;
	int      nBStrsComp     ;
	int      nBStrsCompSub  ;
	int      nMomentComp    ;
	int      n7thDOFType    ;
	double   dScale         ;
	COLORREF RGBColor       ;
	
	void Initialize()
	{
		bIsBatch        = FALSE;
		bodyelem_grup_k = 0;
		bAllStage       = FALSE;
		bReport         = FALSE;
		nBStrsComp      = 0;
		nBStrsCompSub   = 0;
		nMomentComp     = 0;
		n7thDOFType     = 0;
		dScale          = 1;
		RGBColor        = RGB(0, 0, 0); // Black
	}
};

#define MAX_BRGD_OPT 30
struct T_UCHT_BRGD_D
{
	int    nXAxisType;
	int    nDiaType;
	int    nIdx;
	BOOL   bDrawBoundLine ;
	double dCompValue     ;
	double dTensValue     ;
	int    nOptCount;
	T_UCHT_BRGD_BATCH_D aOpt[MAX_BRGD_OPT];

	void Initialize()
	{
		nXAxisType     = 0;
		nDiaType       = 0;
		nIdx           = -1;
		bDrawBoundLine = FALSE;
		dCompValue     = 0.0;
		dTensValue     = 0.0;
		nOptCount      = 0;
		for(int i=0; i<MAX_BRGD_OPT; i++) aOpt[i].Initialize();
	}
	void Convert821(T_UCHT_BRGD_D_821& src)
	{
		nXAxisType = src.nXAxisType;
		nDiaType   = src.nDiaType;
		nIdx       = src.nIdx;
		nOptCount  = 1;
		aOpt[0].strName[0]      = NULL;
		aOpt[0].bIsBatch        = FALSE;
		aOpt[0].bodyelem_grup_k = 1;
		aOpt[0].bAllStage       = src.bAllStage;
		aOpt[0].bReport         = TRUE;
		aOpt[0].nBStrsComp      = src.nBStrsComp;
		aOpt[0].nBStrsCompSub   = src.nBStrsCompSub;
		aOpt[0].nMomentComp     = src.nMomentComp;
		aOpt[0].n7thDOFType     = 0;
		aOpt[0].dScale          = 1.0;
		aOpt[0].RGBColor        = RGB(0,0,0);
	}       
};

///// for Cutting Diagram
struct T_UCHT_CUTD_D
{
	int  nGPSMode;
	BOOL bAverageNodal;
	BOOL bAverageNodalActiveOnly;
	BOOL bVectorPositive;
	BOOL bVectorNegative;
	BOOL bHingeStatus;
	BOOL bOnCuttingLine;
	BOOL bOnCuttingPlane;
	BOOL bPnStrsDataSet;
	BOOL bIsoSurface;
	
	int  nCom;
	int  nComVec;
	int  nCutLineGraphDrawStyle;
	int  nGlobalLocal;
	
	double dOrg[3];
	double dVx[3];
	double dVy[3];
	
	void Initialize()
	{
		nGPSMode = 0;
		bAverageNodal = FALSE;
		bAverageNodalActiveOnly = FALSE;
		bVectorPositive = FALSE;
		bVectorNegative = FALSE;
		bHingeStatus = FALSE;
		bOnCuttingLine = FALSE;
		bOnCuttingPlane = FALSE;
		bPnStrsDataSet = FALSE;
		bIsoSurface = FALSE;
		
		nCom = 0;
		nComVec = 0;
		nCutLineGraphDrawStyle = 0;
		nGlobalLocal = 0;
		
		for(int i=0; i<3; i++) dOrg[i]=0.0; 
		for(int i=0; i<3; i++) dVx[i]=0.0; 
		for(int i=0; i<3; i++) dVy[i]=0.0; 
	}
};

enum ENRG_GRAPH_FILL_TYPE {
	ENRG_GRAPH_NO_FILL,
	ENRG_GRAPH_SOLID_FILL,
};
enum ENRG_GRAPH_DISP_TYPE {
	ENRG_GRAPH_DISP_VALUE,
	ENRG_GRAPH_DISP_PERCENT,
};

enum ENRG_IDX {
	ENRG_Eh,
	ENRG_Ek,
	ENRG_Es,
	ENRG_Ed,
	ENRG_Em,
	ENRG_Ev,
	ENRG_Et,
	ENRG_Eo,
	ENRG_Ep,
	ENRG_Ei,
	ENRG_COUNT
};

// for TH Energy Group Graph 
struct T_UCHT_EGRP_D
{				
	unsigned int aGrupK[20];
	unsigned int aBngrK[20];
	ENRG_IDX enrgType;
	BOOL bTotalEnrg;
	BOOL bCmValType;
	ENRG_GRAPH_DISP_TYPE nDispType;
	T_THIS_K ThisK;
	unsigned int nLCType;
	ENRG_GRAPH_FILL_TYPE nDispOpt;  

	void Initialize()
	{
		memset(aGrupK,0,sizeof(aGrupK));
		memset(aBngrK,0,sizeof(aBngrK));
		enrgType = ENRG_Ei;
		bTotalEnrg = TRUE;
		bCmValType = TRUE;
		nDispType = ENRG_GRAPH_DISP_PERCENT;
		ThisK = 0;
		nLCType = 0;
		nDispOpt = ENRG_GRAPH_SOLID_FILL;
	}
};

// for TH Energy Structure Graph
struct T_UCHT_ENRG_D_871
{				
	BOOL bEi;                 /* Input Energy                     */
	BOOL bEk;                 /* Kinetic Energy                   */
	BOOL bEs;                 /* Strain Energy                    */ 
	BOOL bEd;                 /* Damping Energy                   */
	BOOL bEm;                 /* Maxwell Damper Energy            */
	BOOL bEv;                 /* Velocity Dependent Device Energy */
	BOOL bEh;                 /* Strain Dependent Device Energy   */
	BOOL bEo;                 /* Isolator Device Energy           */
	BOOL bCmValType;
	T_THIS_K ThisK;
	unsigned int nLCType;
	int nDispOpt;             /* 0:No Fill, 1:Soild Fill          */
	void Initialize()
	{
		bEi = FALSE;
		bEk = FALSE;
		bEs = FALSE;
		bEd = FALSE;
		bEm = FALSE;
		bEv = FALSE;
		bEh = FALSE;
		bEo = FALSE;
		bCmValType = TRUE;
		ThisK = 0;
		nLCType  = 0;
		nDispOpt = ENRG_GRAPH_SOLID_FILL;
	}
};

// for TH Energy Structure Graph
struct T_UCHT_ENRG_D
{				
	BOOL bEi;                 /* Input Energy                     */
	BOOL bEk;                 /* Kinetic Energy                   */
	BOOL bEs;                 /* Strain Energy                    */ 
	BOOL bEd;                 /* Damping Energy                   */
	BOOL bEp;                 /* Plastic Energy                   */
	BOOL bEh;                 /* Dissipated Inelastic Energy      */
	BOOL bEm;                 /* Maxwell Damper Energy            */
	BOOL bEv;                 /* Velocity Dependent Device Energy */
	BOOL bEt;                 /* Strain Dependent Device Energy   */
	BOOL bEo;                 /* Isolator Device Energy           */
	BOOL bCmValType;
	T_THIS_K ThisK;
	unsigned int nLCType;
	int nDispOpt;             /* ENRG_GRAPH_FILL_TYPE             */
	int nDispType;            /* ENRG_GRAPH_DISP_TYPE             */

	void Initialize()
	{
		bEi = FALSE;
		bEk = FALSE;
		bEs = FALSE;
		bEd = FALSE;
		bEp = FALSE;
		bEh = FALSE;
		bEm = FALSE;
		bEv = FALSE;
		bEh = FALSE;
		bEo = FALSE;
		bCmValType = TRUE;
		ThisK = 0;
		nLCType  = 0;
		nDispOpt = ENRG_GRAPH_SOLID_FILL;
		nDispType = ENRG_GRAPH_DISP_PERCENT;
	}
	void Convert871(const T_UCHT_ENRG_D_871& src)
	{
		bEi         = src.bEi       ;
		bEk         = src.bEk       ;
		bEs         = src.bEs       ;
		bEd         = src.bEd       ;
		bEp         = FALSE;
		bEh         = FALSE;
		bEm         = src.bEm       ;
		bEv         = src.bEv       ;
		bEh         = src.bEh       ;
		bEo         = src.bEo       ;
		bCmValType  = src.bCmValType;
		ThisK       = src.ThisK     ;
		nLCType     = src.nLCType   ;
		nDispOpt    = src.nDispOpt  ;
		nDispType   = ENRG_GRAPH_DISP_PERCENT;
	}
};

struct T_UCHT_TH_STDR_D
{
	bool isXDirection;
	bool isYDirection;

	bool checkMin;
	bool checkMax;
	bool checkAbsMax;

	bool considerSign;

	LOADCASE_TYPE emphasizeTHLoadCaseType;
	UINT emphasizeTHLoadCaseKey;

	bool checkIO;
	double valueIO;

	bool checkLS;
	double valueLS;

	bool checkCP;
	double valueCP;

	void Initialize()
	{
		isXDirection = false;
		isYDirection = false;

		checkMin = false;
		checkMax = false;
		checkAbsMax = false;

		considerSign = false;

		emphasizeTHLoadCaseType = LOADCASE_TYPE::NONE;
		emphasizeTHLoadCaseKey = 0;

		checkIO = false;
		valueIO = 0.0;

		checkLS = false;
		valueLS = 0.0;

		checkCP = false;
		valueCP = 0.0;
	}
};

union T_UCHT_DATA_D_815
{
	T_UCHT_GRPH_D_871 Grph;
	T_UCHT_STGH_D Stgh;
	T_UCHT_POCV_D Pocv;
	T_UCHT_EQRS_D Eqrs;
	T_UCHT_STHG_D Sthg;
	T_UCHT_HHAD_D Hhad;
	T_UCHT_BRGD_D_815 Brgd;
	T_UCHT_CUTD_D Cutd;
};

union T_UCHT_DATA_D_821
{
	T_UCHT_GRPH_D_871 Grph;
	T_UCHT_STGH_D Stgh;
	T_UCHT_POCV_D Pocv;
	T_UCHT_EQRS_D Eqrs;
	T_UCHT_STHG_D Sthg;
	T_UCHT_HHAD_D Hhad;
	T_UCHT_BRGD_D_821 Brgd;
	T_UCHT_CUTD_D Cutd;
};

union T_UCHT_DATA_D_871
{
	T_UCHT_GRPH_D_871 Grph;
	T_UCHT_STGH_D Stgh;
	T_UCHT_POCV_D Pocv;
	T_UCHT_EQRS_D Eqrs;
	T_UCHT_STHG_D Sthg;
	T_UCHT_HHAD_D Hhad;
	T_UCHT_BRGD_D Brgd;
	T_UCHT_CUTD_D Cutd;
	T_UCHT_ENRG_D_871 Enrg;
};

union T_UCHT_DATA_D
{
	T_UCHT_GRPH_D Grph;
	T_UCHT_STGH_D Stgh;
	T_UCHT_POCV_D Pocv;
	T_UCHT_EQRS_D Eqrs;
	T_UCHT_STHG_D Sthg;
	T_UCHT_HHAD_D Hhad;
	T_UCHT_BRGD_D Brgd;
	T_UCHT_CUTD_D Cutd;
	T_UCHT_ENRG_D Enrg;
	T_UCHT_EGRP_D Egrp;
	T_UCHT_TH_STDR_D thStdr;

	void Convert821(T_UCHT_DATA_D_821& src)
	{
		Grph.Convert871(src.Grph);
		Stgh = src.Stgh;
		Pocv = src.Pocv;
		Eqrs = src.Eqrs;
		Sthg = src.Sthg;
		Hhad = src.Hhad;
		Brgd.Convert821(src.Brgd);
		Cutd = src.Cutd;
	}
	void Convert871(int nType, T_UCHT_DATA_D_871& src)
	{
		switch(nType)
		{
		case D_TYPE_TH_GRAPH:
		case D_TYPE_PO_GRAPH:
		case D_TYPE_MD_RATIO:
			Grph.Convert871(src.Grph);
			break;
		case D_TYPE_TH_STORY:
		case D_TYPE_PO_STORY:
			Stgh = src.Stgh;
			break;
		case D_TYPE_PO_CURVE:
			Pocv = src.Pocv;
			break;
		case D_TYPE_EARTHQUAKE:
			Eqrs = src.Eqrs;
			break;
		case D_TYPE_SS_GRAPH:
			Sthg = src.Sthg;
			break;
		case D_TYPE_HH_DIAG:
			Hhad = src.Hhad;
			break;
		case D_TYPE_CD_DIAGR:
		case D_TYPE_PL_DIAGR:
		case D_TYPE_ST_DIAGR:
		case D_TYPE_PS_DIAGR: 
			Cutd = src.Cutd;
			break;
		case D_TYPE_BG_DIAGR:
			Brgd = src.Brgd;
			break;
		case D_TYPE_TH_ENRG:
			Enrg.Convert871(src.Enrg);
			break;
		default:
			Grph.Convert871(src.Grph);
			break;
		}
	}
};

#define T_UCHT_K unsigned int

struct T_UCHT_D_821
{
	int      nType; // D_TYPE_XXX
	UINT     nKey;  // nType==D_TYPE_RS_FUNC:T_SPFC_K, D_TYPE_TH_FUNC:T_THFC_K, D_TYPE_PO_FUNC:T_POFC_K,  D_TYPE_TD_FUNC:T_TDMF_K
									// nType==D_TYPE_GC_GRAPH:T_GRUP_K, D_TYPE_SB_GRAPH:T_SBCT_D.aHangerZDisp의 index

	CString  strName;
	CString  strCaption;  // Caption
	CString  strGraphName;
	CString  strFuncName;
	CString  strLableX;
	CString  strLableY;
	BOOL     bExpX;
	BOOL     bExpY;
	BOOL     bLogX;
	BOOL     bLogY;

	int      nXDecPt;
	int      nYDecPt;
	int      nFuncType;   // nType==D_TYPE_HS_FUNC, D_TYPE_GC_GRAPH:direction

	double dTemper;
	double dMaxTemper;
	double dReactive;
	double dDelay;
	double dMeanTemper;

	// 타입별 데이터
	T_UCHT_DATA_D_821 Data;           // Grph, Stgh, Pocv, Eqrs, Sthg
	CArray<UINT, UINT> aKeyX;     // nType = 4 or 5 - TH : T_THRD_K, PO : T_PORD_K, nType = 14 : T_HHND_K
	CArray<UINT, UINT> aKeyY;     // nType = 4 or 5 - TH : T_THRD_K, PO : T_PORD_K
	CArray<UINT, UINT> aStoryK;   // nType = 6 or 7 - T_STOR_K
	
	void SetType(int nNewType) // 타입별 데이터 초기화 (타입을 변경시에는 얘를 불러준다)
	{
		nType = nNewType;
		switch(nType)
		{
		case D_TYPE_TH_GRAPH:
		case D_TYPE_PO_GRAPH:
		case D_TYPE_MD_RATIO:
			Data.Grph.Initialize();
			break;
		case D_TYPE_TH_STORY:
		case D_TYPE_PO_STORY:
			Data.Stgh.Initialize();
			break;
		case D_TYPE_PO_CURVE:
			Data.Pocv.Initialize();
			break;
		case D_TYPE_EARTHQUAKE:
			Data.Eqrs.Initialize();
			break;
		case D_TYPE_SS_GRAPH:
			Data.Sthg.Initialize();
			break;
		case D_TYPE_HH_DIAG:
			Data.Hhad.Initialize();
			break;
		case D_TYPE_CD_DIAGR:
		case D_TYPE_PL_DIAGR:
		case D_TYPE_ST_DIAGR:
		case D_TYPE_PS_DIAGR: 
			Data.Cutd.Initialize();
			break;
		case D_TYPE_BG_DIAGR:
			Data.Brgd.Initialize();
			break;
		default:
			Data.Grph.Initialize();
			break;
		}
	}

	T_UCHT_D_821() { Initialize(0); }
	void Initialize(int nInitType)
	{
		nType = nInitType;
		nKey = 0;
		strName = "";
		strCaption = "";
		strGraphName = "";
		strFuncName = "";
		strLableX = "";
		strLableY = "";
		bExpX = FALSE;
		bExpY = FALSE;
		bLogX = FALSE;
		bLogY = FALSE;
		nFuncType = 0;
		dTemper = 0.0;
		dMaxTemper = 0.0;
		dReactive = 0.0;
		dDelay = 0.0;
		dMeanTemper = 0.0;

		nXDecPt = 0;
		nYDecPt = 0;
		SetType(nInitType);
		aKeyX.RemoveAll();
		aKeyY.RemoveAll();
		aStoryK.RemoveAll();
	}
	T_UCHT_D_821& operator=(const T_UCHT_D_821 &src)
	{
		nType = src.nType;
		nKey = src.nKey;
		strName = src.strName;
		strCaption = src.strCaption;
		strGraphName = src.strGraphName;
		strFuncName = src.strFuncName;
		strLableX = src.strLableX;
		strLableY = src.strLableY;
		bExpX = src.bExpX;
		bExpY = src.bExpY;
		bLogX = src.bLogX;
		bLogY = src.bLogY;
		nXDecPt = src.nXDecPt;
		nYDecPt = src.nYDecPt;
		nFuncType = src.nFuncType;
		Data = src.Data;
		dTemper = src.dTemper;
		dMaxTemper = src.dMaxTemper;
		dReactive = src.dReactive;
		dDelay = src.dDelay;
		dMeanTemper = src.dMeanTemper;
		
		aKeyX.Copy(src.aKeyX);
		aKeyY.Copy(src.aKeyY);
		aStoryK.Copy(src.aStoryK);
		
		return *this;
	}
	T_UCHT_D_821(const T_UCHT_D_821 &src) { *this = src; }
};

struct T_UCHT_D_830
{
	int      nType; // D_TYPE_XXX
	UINT     nKey;  // nType==D_TYPE_RS_FUNC:T_SPFC_K, D_TYPE_TH_FUNC:T_THFC_K, D_TYPE_PO_FUNC:T_POFC_K,  D_TYPE_TD_FUNC:T_TDMF_K
									// nType==D_TYPE_GC_GRAPH:T_GRUP_K, D_TYPE_SB_GRAPH:T_SBCT_D.aHangerZDisp의 index

	CString  strName;
	CString  strCaption;  // Caption
	CString  strGraphName;
	CString  strFuncName;
	CString  strLableX;
	CString  strLableY;
	BOOL     bExpX;
	BOOL     bExpY;
	BOOL     bLogX;
	BOOL     bLogY;

	int      nXDecPt;
	int      nYDecPt;
	int      nFuncType;   // nType==D_TYPE_HS_FUNC, D_TYPE_GC_GRAPH:direction

	double dTemper;
	double dMaxTemper;
	double dReactive;
	double dDelay;
	double dMeanTemper;

	// 타입별 데이터
	T_UCHT_DATA_D Data;           // Grph, Stgh, Pocv, Eqrs, Sthg
	CArray<UINT, UINT> aKeyX;     // nType = 4 or 5 - TH : T_THRD_K, PO : T_PORD_K, nType = 14 : T_HHND_K
	CArray<UINT, UINT> aKeyY;     // nType = 4 or 5 - TH : T_THRD_K, PO : T_PORD_K
	CArray<UINT, UINT> aStoryK;   // nType = 6 or 7 - T_STOR_K
	
	void SetType(int nNewType) // 타입별 데이터 초기화 (타입을 변경시에는 얘를 불러준다)
	{
		nType = nNewType;
		switch(nType)
		{
		case D_TYPE_TH_GRAPH:
		case D_TYPE_PO_GRAPH:
		case D_TYPE_MD_RATIO:
			Data.Grph.Initialize();
			break;
		case D_TYPE_TH_STORY:
		case D_TYPE_PO_STORY:
			Data.Stgh.Initialize();
			break;
		case D_TYPE_PO_CURVE:
			Data.Pocv.Initialize();
			break;
		case D_TYPE_EARTHQUAKE:
			Data.Eqrs.Initialize();
			break;
		case D_TYPE_SS_GRAPH:
			Data.Sthg.Initialize();
			break;
		case D_TYPE_HH_DIAG:
			Data.Hhad.Initialize();
			break;
		case D_TYPE_CD_DIAGR:
		case D_TYPE_PL_DIAGR:
		case D_TYPE_ST_DIAGR:
		case D_TYPE_PS_DIAGR: 
			Data.Cutd.Initialize();
			break;
		case D_TYPE_BG_DIAGR:
			Data.Brgd.Initialize();
			break;
		default:
			Data.Grph.Initialize();
			break;
		}
	}

	T_UCHT_D_830() { Initialize(0); }
	void Initialize(int nInitType)
	{
		nType = nInitType;
		nKey = 0;
		strName = "";
		strCaption = "";
		strGraphName = "";
		strFuncName = "";
		strLableX = "";
		strLableY = "";
		bExpX = FALSE;
		bExpY = FALSE;
		bLogX = FALSE;
		bLogY = FALSE;
		nFuncType = 0;
		dTemper = 0.0;
		dMaxTemper = 0.0;
		dReactive = 0.0;
		dDelay = 0.0;
		dMeanTemper = 0.0;

		nXDecPt = 0;
		nYDecPt = 0;
		SetType(nInitType);
		aKeyX.RemoveAll();
		aKeyY.RemoveAll();
		aStoryK.RemoveAll();
	}
	T_UCHT_D_830& operator=(const T_UCHT_D_830 &src)
	{
		nType = src.nType;
		nKey = src.nKey;
		strName = src.strName;
		strCaption = src.strCaption;
		strGraphName = src.strGraphName;
		strFuncName = src.strFuncName;
		strLableX = src.strLableX;
		strLableY = src.strLableY;
		bExpX = src.bExpX;
		bExpY = src.bExpY;
		bLogX = src.bLogX;
		bLogY = src.bLogY;
		nXDecPt = src.nXDecPt;
		nYDecPt = src.nYDecPt;
		nFuncType = src.nFuncType;
		Data = src.Data;
		dTemper = src.dTemper;
		dMaxTemper = src.dMaxTemper;
		dReactive = src.dReactive;
		dDelay = src.dDelay;
		dMeanTemper = src.dMeanTemper;

		aKeyX.Copy(src.aKeyX);
		aKeyY.Copy(src.aKeyY);
		aStoryK.Copy(src.aStoryK);
		
		return *this;
	}
	T_UCHT_D_830(const T_UCHT_D_830 &src) { *this = src; }
};

struct T_UCHT_D_871
{
	int      nType; // D_TYPE_XXX
	UINT     nKey;  // nType==D_TYPE_RS_FUNC:T_SPFC_K, D_TYPE_TH_FUNC:T_THFC_K, D_TYPE_PO_FUNC:T_POFC_K,  D_TYPE_TD_FUNC:T_TDMF_K
									// nType==D_TYPE_GC_GRAPH:T_GRUP_K, D_TYPE_SB_GRAPH:T_SBCT_D.aHangerZDisp의 index

	CString  strName;
	CString  strCaption;  // Caption
	CString  strGraphName;
	CString  strFuncName;
	CString  strLableX;
	CString  strLableY;
	BOOL     bExpX;
	BOOL     bExpY;
	BOOL     bLogX;
	BOOL     bLogY;

	int      nXDecPt;
	int      nYDecPt;
	int      nFuncType;   // nType==D_TYPE_HS_FUNC, D_TYPE_GC_GRAPH:direction

	double dTemper;
	double dMaxTemper;
	double dReactive;
	double dDelay;
	double dMeanTemper;

	BOOL bDefinedUnit; // User defined unit system 사용 여부
	int  nUnitLength ; // bDefinedUnit==TRUE 일 때 사용
	int  nUnitForce  ;
	int  nUnitHeat 	 ;
	int  nUnitTemper ;

	// 타입별 데이터
	T_UCHT_DATA_D_871 Data;       // Grph, Stgh, Pocv, Eqrs, Sthg
	CArray<UINT, UINT> aKeyX;     // nType = 4 or 5 - TH : T_THRD_K, PO : T_PORD_K, nType = 14 : T_HHND_K
	CArray<UINT, UINT> aKeyY;     // nType = 4 or 5 - TH : T_THRD_K, PO : T_PORD_K
	CArray<UINT, UINT> aStoryK;   // nType = 6 or 7 - T_STOR_K

	void SetType(int nNewType) // 타입별 데이터 초기화 (타입을 변경시에는 얘를 불러준다)
	{
		nType = nNewType;
		switch(nType)
		{
		case D_TYPE_TH_GRAPH:
		case D_TYPE_PO_GRAPH:
		case D_TYPE_MD_RATIO:
			Data.Grph.Initialize();
			break;
		case D_TYPE_TH_STORY:
		case D_TYPE_PO_STORY:
			Data.Stgh.Initialize();
			break;
		case D_TYPE_PO_CURVE:
			Data.Pocv.Initialize();
			break;
		case D_TYPE_EARTHQUAKE:
			Data.Eqrs.Initialize();
			break;
		case D_TYPE_SS_GRAPH:
			Data.Sthg.Initialize();
			break;
		case D_TYPE_HH_DIAG:
			Data.Hhad.Initialize();
			break;
		case D_TYPE_CD_DIAGR:
		case D_TYPE_PL_DIAGR:
		case D_TYPE_ST_DIAGR:
		case D_TYPE_PS_DIAGR: 
			Data.Cutd.Initialize();
			break;
		case D_TYPE_BG_DIAGR:
			Data.Brgd.Initialize();
			break;
		case D_TYPE_TH_ENRG:
			Data.Enrg.Initialize();
			break;
		default:
			Data.Grph.Initialize();
			break;
		}
	}

	T_UCHT_D_871() { Initialize(0); }
	void Initialize(int nInitType)
	{
		nType = nInitType;
		nKey = 0;
		strName = "";
		strCaption = "";
		strGraphName = "";
		strFuncName = "";
		strLableX = "";
		strLableY = "";
		bExpX = FALSE;
		bExpY = FALSE;
		bLogX = FALSE;
		bLogY = FALSE;
		nFuncType = 0;
		dTemper = 0.0;
		dMaxTemper = 0.0;
		dReactive = 0.0;
		dDelay = 0.0;
		dMeanTemper = 0.0;

		bDefinedUnit = FALSE;
		nUnitLength  = 1;
		nUnitForce   = 1;
		nUnitHeat 	 = 1;
		nUnitTemper  = 1;

		nXDecPt = 0;
		nYDecPt = 0;
		SetType(nInitType);
		aKeyX.RemoveAll();
		aKeyY.RemoveAll();
		aStoryK.RemoveAll();
	}
	T_UCHT_D_871& operator=(const T_UCHT_D_871 &src)
	{
		nType = src.nType;
		nKey = src.nKey;
		strName = src.strName;
		strCaption = src.strCaption;
		strGraphName = src.strGraphName;
		strFuncName = src.strFuncName;
		strLableX = src.strLableX;
		strLableY = src.strLableY;
		bExpX = src.bExpX;
		bExpY = src.bExpY;
		bLogX = src.bLogX;
		bLogY = src.bLogY;
		nXDecPt = src.nXDecPt;
		nYDecPt = src.nYDecPt;
		nFuncType = src.nFuncType;
		Data = src.Data;
		dTemper = src.dTemper;
		dMaxTemper = src.dMaxTemper;
		dReactive = src.dReactive;
		dDelay = src.dDelay;
		dMeanTemper = src.dMeanTemper;

		bDefinedUnit = src.bDefinedUnit;
		nUnitLength  = src.nUnitLength ;
		nUnitForce   = src.nUnitForce  ;
		nUnitHeat 	 = src.nUnitHeat 	 ;
		nUnitTemper  = src.nUnitTemper ;

		aKeyX.Copy(src.aKeyX);
		aKeyY.Copy(src.aKeyY);
		aStoryK.Copy(src.aStoryK);

		return *this;
	}
	T_UCHT_D_871(const T_UCHT_D_871 &src) { *this = src; }
};

struct T_UCHT_D
{
	int      nType; // D_TYPE_XXX
	UINT     nKey;  // nType==D_TYPE_RS_FUNC:T_SPFC_K, D_TYPE_TH_FUNC:T_THFC_K, D_TYPE_PO_FUNC:T_POFC_K,  D_TYPE_TD_FUNC:T_TDMF_K
									// nType==D_TYPE_GC_GRAPH:T_GRUP_K, D_TYPE_SB_GRAPH:T_SBCT_D.aHangerZDisp의 index

	CString  strName;
	CString  strCaption;  // Caption
	CString  strGraphName;
	CString  strFuncName;
	CString  strLableX;
	CString  strLableY;
	BOOL     bExpX;
	BOOL     bExpY;
	BOOL     bLogX;
	BOOL     bLogY;

	int      nXDecPt;
	int      nYDecPt;
	int      nFuncType;   // nType==D_TYPE_HS_FUNC, D_TYPE_GC_GRAPH:direction

	double dTemper;
	double dMaxTemper;
	double dReactive;
	double dDelay;
	double dMeanTemper;

	// User Defined Unit(v831 추가)
	BOOL bDefinedUnit; // User defined unit system 사용 여부
	int  nUnitLength ; // bDefinedUnit==TRUE 일 때 사용
	int  nUnitForce  ;
	int  nUnitHeat 	 ;
	int  nUnitTemper ;

	// 타입별 데이터
	T_UCHT_DATA_D Data;           // Grph, Stgh, Pocv, Eqrs, Sthg
	CArray<UINT, UINT> aKeyX;     // nType = 4 or 5 - TH : T_THRD_K, PO : T_PORD_K, nType = 14 : T_HHND_K	// nType = 44 - T_THIS_K
	CArray<UINT, UINT> aKeyY;     // nType = 4 or 5 - TH : T_THRD_K, PO : T_PORD_K							// nType = 44 - T_THJI_K
	CArray<UINT, UINT> aStoryK;   // nType = 6 or 7 - T_STOR_K
	
	void SetType(int nNewType) // 타입별 데이터 초기화 (타입을 변경시에는 얘를 불러준다)
	{
		nType = nNewType;
		switch(nType)
		{
		case D_TYPE_TH_GRAPH:
		case D_TYPE_PO_GRAPH:
		case D_TYPE_MD_RATIO:
			Data.Grph.Initialize();
			break;
		case D_TYPE_TH_STORY:
		case D_TYPE_PO_STORY:
			Data.Stgh.Initialize();
			break;
		case D_TYPE_PO_CURVE:
			Data.Pocv.Initialize();
			break;
		case D_TYPE_EARTHQUAKE:
			Data.Eqrs.Initialize();
			break;
		case D_TYPE_SS_GRAPH:
			Data.Sthg.Initialize();
			break;
		case D_TYPE_HH_DIAG:
			Data.Hhad.Initialize();
			break;
		case D_TYPE_CD_DIAGR:
		case D_TYPE_PL_DIAGR:
		case D_TYPE_ST_DIAGR:
		case D_TYPE_PS_DIAGR: 
			Data.Cutd.Initialize();
			break;
		case D_TYPE_BG_DIAGR:
			Data.Brgd.Initialize();
			break;
		case D_TYPE_TH_ENRG:
			Data.Enrg.Initialize();
			break;
		case D_TYPE_TH_EGRP:
			Data.Egrp.Initialize();
			break;
		case D_TYPE_TH_STDR_GRAPH:
			Data.thStdr.Initialize();
			break;
		default:
			Data.Grph.Initialize();
			break;
		}
	}

	// 타입별 데이터 접근 함수 (ex - UchtD.THGraphData().nKeyX = 1)
	T_UCHT_GRPH_D& THGraphData()      { ASSERT(nType==4); return Data.Grph; }
	T_UCHT_GRPH_D& POGraphData()      { ASSERT(nType==5); return Data.Grph; }
	T_UCHT_STGH_D& THStoryGraphData() { ASSERT(nType==6); return Data.Stgh; }
	T_UCHT_STGH_D& POStoryGraphData() { ASSERT(nType==7); return Data.Stgh; }
	T_UCHT_POCV_D& POCurveData()      { ASSERT(nType==8); return Data.Pocv; }
	T_UCHT_EQRS_D& EqrthquakeData()   { ASSERT(nType==9); return Data.Eqrs; }
	T_UCHT_STHG_D& THStepGraphData()  { ASSERT(nType==10); return Data.Sthg; }
	T_UCHT_HHAD_D& HHAnalDiaData()    { ASSERT(nType==14); return Data.Hhad; }
	T_UCHT_BRGD_D& BGDiaData()        { ASSERT(nType==26); return Data.Brgd; }
	T_UCHT_ENRG_D& THEnrgGrphData()   { ASSERT(nType==D_TYPE_TH_ENRG); return Data.Enrg; }
	T_UCHT_EGRP_D& THEgrpGrphData()   { ASSERT(nType==D_TYPE_TH_EGRP); return Data.Egrp; }
	T_UCHT_TH_STDR_D& THStoryDriftRatioData() { ASSERT(nType== D_TYPE_TH_STDR_GRAPH); return Data.thStdr; }
	
	T_UCHT_D() { Initialize(0); }
	void Initialize(int nInitType)
	{
		nType = nInitType;
		nKey = 0;
		strName = "";
		strCaption = "";
		strGraphName = "";
		strFuncName = "";
		strLableX = "";
		strLableY = "";
		bExpX = FALSE;
		bExpY = FALSE;
		bLogX = FALSE;
		bLogY = FALSE;
		nFuncType = 0;
		dTemper = 0.0;
		dMaxTemper = 0.0;
		dReactive = 0.0;
		dDelay = 0.0;
		dMeanTemper = 0.0;

		bDefinedUnit = FALSE;
		nUnitLength  = 1;
		nUnitForce   = 1;
		nUnitHeat 	 = 1;
		nUnitTemper  = 1;

		nXDecPt = 0;
		nYDecPt = 0;
		SetType(nInitType);
		aKeyX.RemoveAll();
		aKeyY.RemoveAll();
		aStoryK.RemoveAll();
	}
	T_UCHT_D& operator=(const T_UCHT_D &src)
	{
		nType = src.nType;
		nKey = src.nKey;
		strName = src.strName;
		strCaption = src.strCaption;
		strGraphName = src.strGraphName;
		strFuncName = src.strFuncName;
		strLableX = src.strLableX;
		strLableY = src.strLableY;
		bExpX = src.bExpX;
		bExpY = src.bExpY;
		bLogX = src.bLogX;
		bLogY = src.bLogY;
		nXDecPt = src.nXDecPt;
		nYDecPt = src.nYDecPt;
		nFuncType = src.nFuncType;
		Data = src.Data;
		dTemper = src.dTemper;
		dMaxTemper = src.dMaxTemper;
		dReactive = src.dReactive;
		dDelay = src.dDelay;
		dMeanTemper = src.dMeanTemper;

		bDefinedUnit = src.bDefinedUnit;
		nUnitLength  = src.nUnitLength ;
		nUnitForce   = src.nUnitForce  ;
		nUnitHeat 	 = src.nUnitHeat 	 ;
		nUnitTemper  = src.nUnitTemper ;

		aKeyX.Copy(src.aKeyX);
		aKeyY.Copy(src.aKeyY);
		aStoryK.Copy(src.aStoryK);
		
		return *this;
	}
	T_UCHT_D(const T_UCHT_D &src) { *this = src; }
};

struct T_UCHT_D_CH_821
{
	int      nType;
	UINT     nKey;
	char     strName[60];
	char     strCaption[60];
	char     strGraphName[60];
	char     strFuncName[60];
	char     strLableX[60];
	char     strLableY[60];
	BOOL     bExpX;
	BOOL     bExpY;
	BOOL     bLogX;
	BOOL     bLogY;

	int      nXDecPt;
	int      nYDecPt;
	int      nFuncType;

	double dTemper;
	double dMaxTemper;
	double dReactive;
	double dDelay;
	double dMeanTemper;

	// 타입별 데이터
	T_UCHT_DATA_D_821 Data;           // Grph, Stgh, Pocv, Eqrs, Sthg
	CArray<UINT, UINT> aKeyX;     // nType = 4 or 5 - TH : T_THRD_K, PO : T_PORD_K
	CArray<UINT, UINT> aKeyY;     // nType = 4 or 5 - TH : T_THRD_K, PO : T_PORD_K
	CArray<UINT, UINT> aStoryK;   // nType = 6 or 7 - T_STOR_K
	
	T_UCHT_D_CH_821() {}
	void Get(T_UCHT_D_821 &rData)
	{
		rData.nType = nType;
		rData.nKey = nKey;
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		ConvertCharStr(strGraphName, rData.strGraphName, sizeof(strGraphName));
		ConvertCharStr(strFuncName, rData.strFuncName, sizeof(strFuncName));
		ConvertCharStr(strLableX, rData.strLableX, sizeof(strLableX));
		ConvertCharStr(strLableY, rData.strLableY, sizeof(strLableY));
		rData.bExpX = bExpX;
		rData.bExpY = bExpY;
		rData.bLogX = bLogX;
		rData.bLogY = bLogY;
		rData.nXDecPt = nXDecPt;
		rData.nYDecPt = nYDecPt;
		rData.nFuncType = nFuncType;
		rData.Data = Data;

		rData.dTemper = dTemper;
		rData.dMaxTemper = dMaxTemper;
		rData.dReactive = dReactive;
		rData.dDelay = dDelay;
		rData.dMeanTemper = dMeanTemper;

		rData.aKeyX.Copy(aKeyX);
		rData.aKeyY.Copy(aKeyY);
		rData.aStoryK.Copy(aStoryK);
	}
	void Set(T_UCHT_D_821 src)
	{
		nType = src.nType;
		nKey = src.nKey;
		ConvertStrChar(src.strName, strName, sizeof(strName));
		ConvertStrChar(src.strCaption, strCaption, sizeof(strCaption));
		ConvertStrChar(src.strGraphName, strGraphName, sizeof(strGraphName));
		ConvertStrChar(src.strFuncName, strFuncName, sizeof(strFuncName));
		ConvertStrChar(src.strLableX, strLableX, sizeof(strLableX));
		ConvertStrChar(src.strLableY, strLableY, sizeof(strLableY));

		bExpX = src.bExpX;
		bExpY = src.bExpY;
		bLogX = src.bLogX;
		bLogY = src.bLogY;
		nXDecPt = src.nXDecPt;
		nYDecPt = src.nYDecPt;
		nFuncType = src.nFuncType;

		dTemper = src.dTemper;
		dMaxTemper = src.dMaxTemper;
		dReactive = src.dReactive;
		dDelay = src.dDelay;
		dMeanTemper = src.dMeanTemper;

		Data = src.Data;
		aKeyX.Copy(src.aKeyX);
		aKeyY.Copy(src.aKeyY);
		aStoryK.Copy(src.aStoryK);
	}
	T_UCHT_D_CH_821& operator=(const T_UCHT_D_CH_821 &src)
	{
		nType = src.nType;
		nKey = src.nKey;
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		memcpy(strGraphName, src.strGraphName, sizeof(strGraphName));
		memcpy(strFuncName, src.strFuncName, sizeof(strFuncName));
		memcpy(strLableX, src.strLableX, sizeof(strLableX));
		memcpy(strLableY, src.strLableY, sizeof(strLableY));

		bExpX = src.bExpX;
		bExpY = src.bExpY;
		bLogX = src.bLogX;
		bLogY = src.bLogY;
		nXDecPt = src.nXDecPt;
		nYDecPt = src.nYDecPt;
		nFuncType = src.nFuncType;

		dTemper = src.dTemper;
		dMaxTemper = src.dMaxTemper;
		dReactive = src.dReactive;
		dDelay = src.dDelay;
		dMeanTemper = src.dMeanTemper;

		Data = src.Data;
		aKeyX.Copy(src.aKeyX);
		aKeyY.Copy(src.aKeyY);
		aStoryK.Copy(src.aStoryK);
		
		return *this;
	}
	T_UCHT_D_CH_821(const T_UCHT_D_CH_821 &src) { *this = src; }
};

struct T_UCHT_D_CH_830
{
	int      nType;
	UINT     nKey;
	char     strName[60];
	char     strCaption[60];
	char     strGraphName[60];
	char     strFuncName[60];
	char     strLableX[60];
	char     strLableY[60];
	BOOL     bExpX;
	BOOL     bExpY;
	BOOL     bLogX;
	BOOL     bLogY;

	int      nXDecPt;
	int      nYDecPt;
	int      nFuncType;

	double dTemper;
	double dMaxTemper;
	double dReactive;
	double dDelay;
	double dMeanTemper;

	// 타입별 데이터
	T_UCHT_DATA_D Data;           // Grph, Stgh, Pocv, Eqrs, Sthg
	CArray<UINT, UINT> aKeyX;     // nType = 4 or 5 - TH : T_THRD_K, PO : T_PORD_K
	CArray<UINT, UINT> aKeyY;     // nType = 4 or 5 - TH : T_THRD_K, PO : T_PORD_K
	CArray<UINT, UINT> aStoryK;   // nType = 6 or 7 - T_STOR_K
	
	T_UCHT_D_CH_830() {}
	void Get(T_UCHT_D_830 &rData)
	{
		rData.nType = nType;
		rData.nKey = nKey;
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		ConvertCharStr(strGraphName, rData.strGraphName, sizeof(strGraphName));
		ConvertCharStr(strFuncName, rData.strFuncName, sizeof(strFuncName));
		ConvertCharStr(strLableX, rData.strLableX, sizeof(strLableX));
		ConvertCharStr(strLableY, rData.strLableY, sizeof(strLableY));
		rData.bExpX = bExpX;
		rData.bExpY = bExpY;
		rData.bLogX = bLogX;
		rData.bLogY = bLogY;
		rData.nXDecPt = nXDecPt;
		rData.nYDecPt = nYDecPt;
		rData.nFuncType = nFuncType;
		rData.Data = Data;

		rData.dTemper = dTemper;
		rData.dMaxTemper = dMaxTemper;
		rData.dReactive = dReactive;
		rData.dDelay = dDelay;
		rData.dMeanTemper = dMeanTemper;

		rData.aKeyX.Copy(aKeyX);
		rData.aKeyY.Copy(aKeyY);
		rData.aStoryK.Copy(aStoryK);
	}
	void Set(T_UCHT_D_830 src)
	{
		nType = src.nType;
		nKey = src.nKey;
		ConvertStrChar(src.strName, strName, sizeof(strName));
		ConvertStrChar(src.strCaption, strCaption, sizeof(strCaption));
		ConvertStrChar(src.strGraphName, strGraphName, sizeof(strGraphName));
		ConvertStrChar(src.strFuncName, strFuncName, sizeof(strFuncName));
		ConvertStrChar(src.strLableX, strLableX, sizeof(strLableX));
		ConvertStrChar(src.strLableY, strLableY, sizeof(strLableY));

		bExpX = src.bExpX;
		bExpY = src.bExpY;
		bLogX = src.bLogX;
		bLogY = src.bLogY;
		nXDecPt = src.nXDecPt;
		nYDecPt = src.nYDecPt;
		nFuncType = src.nFuncType;

		dTemper = src.dTemper;
		dMaxTemper = src.dMaxTemper;
		dReactive = src.dReactive;
		dDelay = src.dDelay;
		dMeanTemper = src.dMeanTemper;

		Data = src.Data;
		aKeyX.Copy(src.aKeyX);
		aKeyY.Copy(src.aKeyY);
		aStoryK.Copy(src.aStoryK);
	}
	T_UCHT_D_CH_830& operator=(const T_UCHT_D_CH_830 &src)
	{
		nType = src.nType;
		nKey = src.nKey;
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		memcpy(strGraphName, src.strGraphName, sizeof(strGraphName));
		memcpy(strFuncName, src.strFuncName, sizeof(strFuncName));
		memcpy(strLableX, src.strLableX, sizeof(strLableX));
		memcpy(strLableY, src.strLableY, sizeof(strLableY));

		bExpX = src.bExpX;
		bExpY = src.bExpY;
		bLogX = src.bLogX;
		bLogY = src.bLogY;
		nXDecPt = src.nXDecPt;
		nYDecPt = src.nYDecPt;
		nFuncType = src.nFuncType;

		dTemper = src.dTemper;
		dMaxTemper = src.dMaxTemper;
		dReactive = src.dReactive;
		dDelay = src.dDelay;
		dMeanTemper = src.dMeanTemper;

		Data = src.Data;
		aKeyX.Copy(src.aKeyX);
		aKeyY.Copy(src.aKeyY);
		aStoryK.Copy(src.aStoryK);
		
		return *this;
	}
	T_UCHT_D_CH_830(const T_UCHT_D_CH_830 &src) { *this = src; }
};

struct T_UCHT_D_CH_871
{
	int      nType;
	UINT     nKey;
	char     strName[60];
	char     strCaption[60];
	char     strGraphName[60];
	char     strFuncName[60];
	char     strLableX[60];
	char     strLableY[60];
	BOOL     bExpX;
	BOOL     bExpY;
	BOOL     bLogX;
	BOOL     bLogY;

	int      nXDecPt;
	int      nYDecPt;
	int      nFuncType;

	double dTemper;
	double dMaxTemper;
	double dReactive;
	double dDelay;
	double dMeanTemper;

	// User Defined Unit(v831 추가)
	BOOL bDefinedUnit; // User defined unit system 사용 여부
	int  nUnitLength ; // bDefinedUnit==TRUE 일 때 사용
	int  nUnitForce  ;
	int  nUnitHeat 	 ;
	int  nUnitTemper ;

	// 타입별 데이터
	T_UCHT_DATA_D_871 Data;           // Grph, Stgh, Pocv, Eqrs, Sthg
	CArray<UINT, UINT> aKeyX;     // nType = 4 or 5 - TH : T_THRD_K, PO : T_PORD_K
	CArray<UINT, UINT> aKeyY;     // nType = 4 or 5 - TH : T_THRD_K, PO : T_PORD_K
	CArray<UINT, UINT> aStoryK;   // nType = 6 or 7 - T_STOR_K

	T_UCHT_D_CH_871() {}
	void Get(T_UCHT_D_871 &rData)
	{
		rData.nType = nType;
		rData.nKey = nKey;
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		ConvertCharStr(strGraphName, rData.strGraphName, sizeof(strGraphName));
		ConvertCharStr(strFuncName, rData.strFuncName, sizeof(strFuncName));
		ConvertCharStr(strLableX, rData.strLableX, sizeof(strLableX));
		ConvertCharStr(strLableY, rData.strLableY, sizeof(strLableY));
		rData.bExpX = bExpX;
		rData.bExpY = bExpY;
		rData.bLogX = bLogX;
		rData.bLogY = bLogY;
		rData.nXDecPt = nXDecPt;
		rData.nYDecPt = nYDecPt;
		rData.nFuncType = nFuncType;
		rData.Data = Data;

		rData.dTemper = dTemper;
		rData.dMaxTemper = dMaxTemper;
		rData.dReactive = dReactive;
		rData.dDelay = dDelay;
		rData.dMeanTemper = dMeanTemper;

		rData.bDefinedUnit = bDefinedUnit;
		rData.nUnitLength  = nUnitLength ;
		rData.nUnitForce   = nUnitForce  ;
		rData.nUnitHeat 	 = nUnitHeat 	 ;
		rData.nUnitTemper  = nUnitTemper ;

		rData.aKeyX.Copy(aKeyX);
		rData.aKeyY.Copy(aKeyY);
		rData.aStoryK.Copy(aStoryK);
	}
	void Set(T_UCHT_D_871 src)
	{
		nType = src.nType;
		nKey = src.nKey;
		ConvertStrChar(src.strName, strName, sizeof(strName));
		ConvertStrChar(src.strCaption, strCaption, sizeof(strCaption));
		ConvertStrChar(src.strGraphName, strGraphName, sizeof(strGraphName));
		ConvertStrChar(src.strFuncName, strFuncName, sizeof(strFuncName));
		ConvertStrChar(src.strLableX, strLableX, sizeof(strLableX));
		ConvertStrChar(src.strLableY, strLableY, sizeof(strLableY));

		bExpX = src.bExpX;
		bExpY = src.bExpY;
		bLogX = src.bLogX;
		bLogY = src.bLogY;
		nXDecPt = src.nXDecPt;
		nYDecPt = src.nYDecPt;
		nFuncType = src.nFuncType;

		dTemper = src.dTemper;
		dMaxTemper = src.dMaxTemper;
		dReactive = src.dReactive;
		dDelay = src.dDelay;
		dMeanTemper = src.dMeanTemper;

		bDefinedUnit = src.bDefinedUnit;
		nUnitLength  = src.nUnitLength ;
		nUnitForce   = src.nUnitForce  ;
		nUnitHeat 	 = src.nUnitHeat 	 ;
		nUnitTemper  = src.nUnitTemper ;

		Data = src.Data;
		aKeyX.Copy(src.aKeyX);
		aKeyY.Copy(src.aKeyY);
		aStoryK.Copy(src.aStoryK);
	}
	T_UCHT_D_CH_871& operator=(const T_UCHT_D_CH_871 &src)
	{
		nType = src.nType;
		nKey = src.nKey;
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		memcpy(strGraphName, src.strGraphName, sizeof(strGraphName));
		memcpy(strFuncName, src.strFuncName, sizeof(strFuncName));
		memcpy(strLableX, src.strLableX, sizeof(strLableX));
		memcpy(strLableY, src.strLableY, sizeof(strLableY));

		bExpX = src.bExpX;
		bExpY = src.bExpY;
		bLogX = src.bLogX;
		bLogY = src.bLogY;
		nXDecPt = src.nXDecPt;
		nYDecPt = src.nYDecPt;
		nFuncType = src.nFuncType;

		dTemper = src.dTemper;
		dMaxTemper = src.dMaxTemper;
		dReactive = src.dReactive;
		dDelay = src.dDelay;
		dMeanTemper = src.dMeanTemper;

		bDefinedUnit = src.bDefinedUnit;
		nUnitLength  = src.nUnitLength ;
		nUnitForce   = src.nUnitForce  ;
		nUnitHeat 	 = src.nUnitHeat 	 ;
		nUnitTemper  = src.nUnitTemper ;

		Data = src.Data;
		aKeyX.Copy(src.aKeyX);
		aKeyY.Copy(src.aKeyY);
		aStoryK.Copy(src.aStoryK);

		return *this;
	}
	T_UCHT_D_CH_871(const T_UCHT_D_CH_871 &src) { *this = src; }
};

struct T_UCHT_D_CH
{
	int      nType;
	UINT     nKey;
	char     strName[60];
	char     strCaption[60];
	char     strGraphName[60];
	char     strFuncName[60];
	char     strLableX[60];
	char     strLableY[60];
	BOOL     bExpX;
	BOOL     bExpY;
	BOOL     bLogX;
	BOOL     bLogY;

	int      nXDecPt;
	int      nYDecPt;
	int      nFuncType;

	double dTemper;
	double dMaxTemper;
	double dReactive;
	double dDelay;
	double dMeanTemper;

	// User Defined Unit(v831 추가)
	BOOL bDefinedUnit; // User defined unit system 사용 여부
	int  nUnitLength ; // bDefinedUnit==TRUE 일 때 사용
	int  nUnitForce  ;
	int  nUnitHeat 	 ;
	int  nUnitTemper ;

	// 타입별 데이터
	T_UCHT_DATA_D Data;           // Grph, Stgh, Pocv, Eqrs, Sthg
	CArray<UINT, UINT> aKeyX;     // nType = 4 or 5 - TH : T_THRD_K, PO : T_PORD_K
	CArray<UINT, UINT> aKeyY;     // nType = 4 or 5 - TH : T_THRD_K, PO : T_PORD_K
	CArray<UINT, UINT> aStoryK;   // nType = 6 or 7 - T_STOR_K
	
	T_UCHT_D_CH() {}
	void Get(T_UCHT_D &rData)
	{
		rData.nType = nType;
		rData.nKey = nKey;
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		ConvertCharStr(strGraphName, rData.strGraphName, sizeof(strGraphName));
		ConvertCharStr(strFuncName, rData.strFuncName, sizeof(strFuncName));
		ConvertCharStr(strLableX, rData.strLableX, sizeof(strLableX));
		ConvertCharStr(strLableY, rData.strLableY, sizeof(strLableY));
		rData.bExpX = bExpX;
		rData.bExpY = bExpY;
		rData.bLogX = bLogX;
		rData.bLogY = bLogY;
		rData.nXDecPt = nXDecPt;
		rData.nYDecPt = nYDecPt;
		rData.nFuncType = nFuncType;
		rData.Data = Data;

		rData.dTemper = dTemper;
		rData.dMaxTemper = dMaxTemper;
		rData.dReactive = dReactive;
		rData.dDelay = dDelay;
		rData.dMeanTemper = dMeanTemper;

		rData.bDefinedUnit = bDefinedUnit;
		rData.nUnitLength  = nUnitLength ;
		rData.nUnitForce   = nUnitForce  ;
		rData.nUnitHeat 	 = nUnitHeat 	 ;
		rData.nUnitTemper  = nUnitTemper ;

		rData.aKeyX.Copy(aKeyX);
		rData.aKeyY.Copy(aKeyY);
		rData.aStoryK.Copy(aStoryK);
	}
	void Set(T_UCHT_D src)
	{
		nType = src.nType;
		nKey = src.nKey;
		ConvertStrChar(src.strName, strName, sizeof(strName));
		ConvertStrChar(src.strCaption, strCaption, sizeof(strCaption));
		ConvertStrChar(src.strGraphName, strGraphName, sizeof(strGraphName));
		ConvertStrChar(src.strFuncName, strFuncName, sizeof(strFuncName));
		ConvertStrChar(src.strLableX, strLableX, sizeof(strLableX));
		ConvertStrChar(src.strLableY, strLableY, sizeof(strLableY));

		bExpX = src.bExpX;
		bExpY = src.bExpY;
		bLogX = src.bLogX;
		bLogY = src.bLogY;
		nXDecPt = src.nXDecPt;
		nYDecPt = src.nYDecPt;
		nFuncType = src.nFuncType;

		dTemper = src.dTemper;
		dMaxTemper = src.dMaxTemper;
		dReactive = src.dReactive;
		dDelay = src.dDelay;
		dMeanTemper = src.dMeanTemper;

		bDefinedUnit = src.bDefinedUnit;
		nUnitLength  = src.nUnitLength ;
		nUnitForce   = src.nUnitForce  ;
		nUnitHeat 	 = src.nUnitHeat 	 ;
		nUnitTemper  = src.nUnitTemper ;

		Data = src.Data;
		aKeyX.Copy(src.aKeyX);
		aKeyY.Copy(src.aKeyY);
		aStoryK.Copy(src.aStoryK);
	}
	T_UCHT_D_CH& operator=(const T_UCHT_D_CH &src)
	{
		nType = src.nType;
		nKey = src.nKey;
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		memcpy(strGraphName, src.strGraphName, sizeof(strGraphName));
		memcpy(strFuncName, src.strFuncName, sizeof(strFuncName));
		memcpy(strLableX, src.strLableX, sizeof(strLableX));
		memcpy(strLableY, src.strLableY, sizeof(strLableY));

		bExpX = src.bExpX;
		bExpY = src.bExpY;
		bLogX = src.bLogX;
		bLogY = src.bLogY;
		nXDecPt = src.nXDecPt;
		nYDecPt = src.nYDecPt;
		nFuncType = src.nFuncType;

		dTemper = src.dTemper;
		dMaxTemper = src.dMaxTemper;
		dReactive = src.dReactive;
		dDelay = src.dDelay;
		dMeanTemper = src.dMeanTemper;

		bDefinedUnit = src.bDefinedUnit;
		nUnitLength  = src.nUnitLength ;
		nUnitForce   = src.nUnitForce  ;
		nUnitHeat 	 = src.nUnitHeat 	 ;
		nUnitTemper  = src.nUnitTemper ;

		Data = src.Data;
		aKeyX.Copy(src.aKeyX);
		aKeyY.Copy(src.aKeyY);
		aStoryK.Copy(src.aStoryK);
		
		return *this;
	}
	T_UCHT_D_CH(const T_UCHT_D_CH &src) { *this = src; }
};

struct T_UCHT_D_CH_RW_815
{
	int      nType;
	UINT     nKey;
	char     strName[60];
	char     strCaption[60];
	char     strGraphName[60];
	char     strFuncName[60];
	char     strLableX[60];
	char     strLableY[60];
	BOOL     bExpX;
	BOOL     bExpY;
	BOOL     bLogX;
	BOOL     bLogY;
	int      nXDecPt;
	int      nYDecPt;
	int      nFuncType;
	
	double dTemper;
	double dMaxTemper;
	double dReactive;
	double dDelay;
	double dMeanTemper;
	
	// 타입별 데이터
	T_UCHT_DATA_D Data; // Grph, Stgh, Pocv, Eqrs, Sthg
	int aKeyXCount;
	int aKeyYCount;     // nType = 4 or 5 - TH : T_THRD_K, PO : T_PORD_K
	int aStoryKCount;   // nType = 6 or 7 - T_STOR_K
};

struct T_UCHT_D_CH_RW_821
{
	int      nType;
	UINT     nKey;
	char     strName[60];
	char     strCaption[60];
	char     strGraphName[60];
	char     strFuncName[60];
	char     strLableX[60];
	char     strLableY[60];
	BOOL     bExpX;
	BOOL     bExpY;
	BOOL     bLogX;
	BOOL     bLogY;
	int      nXDecPt;
	int      nYDecPt;
	int      nFuncType;

	double dTemper;
	double dMaxTemper;
	double dReactive;
	double dDelay;
	double dMeanTemper;

	// 타입별 데이터
	T_UCHT_DATA_D_821 Data; // Grph, Stgh, Pocv, Eqrs, Sthg
	int aKeyXCount;
	int aKeyYCount;     // nType = 4 or 5 - TH : T_THRD_K, PO : T_PORD_K
	int aStoryKCount;   // nType = 6 or 7 - T_STOR_K
	
	void Get(T_UCHT_D_CH_821 &rData)
	{
		rData.nType = nType;
		rData.nKey = nKey;
		memcpy(rData.strName, strName, sizeof(strName));
		memcpy(rData.strCaption, strCaption, sizeof(strCaption));
		memcpy(rData.strGraphName, strGraphName, sizeof(strGraphName));
		memcpy(rData.strFuncName, strFuncName, sizeof(strFuncName));
		memcpy(rData.strLableX, strLableX, sizeof(strLableX));
		memcpy(rData.strLableY, strLableY, sizeof(strLableY));
		rData.bExpX = bExpX;
		rData.bExpY = bExpY;
		rData.bLogX = bLogX;
		rData.bLogY = bLogY;
		rData.nXDecPt = nXDecPt;
		rData.nYDecPt = nYDecPt;
		rData.nFuncType = nFuncType;

		rData.dTemper = dTemper;
		rData.dMaxTemper = dMaxTemper;
		rData.dReactive = dReactive;
		rData.dDelay = dDelay;
		rData.dMeanTemper = dMeanTemper;

		rData.Data = Data;
	}
	void Set(T_UCHT_D_CH_821 src)
	{
		nType = src.nType;
		nKey = src.nKey;
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		memcpy(strGraphName, src.strGraphName, sizeof(strGraphName));
		memcpy(strFuncName, src.strFuncName, sizeof(strFuncName));
		memcpy(strLableX, src.strLableX, sizeof(strLableX));
		memcpy(strLableY, src.strLableY, sizeof(strLableY));
		bExpX = src.bExpX;
		bExpY = src.bExpY;
		bLogX = src.bLogX;
		bLogY = src.bLogY;
		nXDecPt = src.nXDecPt;
		nYDecPt = src.nYDecPt;
		nFuncType = src.nFuncType;

		dTemper = src.dTemper;
		dMaxTemper = src.dMaxTemper;
		dReactive = src.dReactive;
		dDelay = src.dDelay;
		dMeanTemper = src.dMeanTemper;

		Data = src.Data;
		aKeyXCount = (int)src.aKeyX.GetSize();
		aKeyYCount = (int)src.aKeyY.GetSize();
		aStoryKCount = (int)src.aStoryK.GetSize();
	}
};

struct T_UCHT_D_CH_RW_830
{
	int      nType;
	UINT     nKey;
	char     strName[60];
	char     strCaption[60];
	char     strGraphName[60];
	char     strFuncName[60];
	char     strLableX[60];
	char     strLableY[60];
	BOOL     bExpX;
	BOOL     bExpY;
	BOOL     bLogX;
	BOOL     bLogY;
	int      nXDecPt;
	int      nYDecPt;
	int      nFuncType;
	
	double dTemper;
	double dMaxTemper;
	double dReactive;
	double dDelay;
	double dMeanTemper;
	
	// 타입별 데이터
	T_UCHT_DATA_D Data; // Grph, Stgh, Pocv, Eqrs, Sthg
	int aKeyXCount;
	int aKeyYCount;     // nType = 4 or 5 - TH : T_THRD_K, PO : T_PORD_K
	int aStoryKCount;   // nType = 6 or 7 - T_STOR_K
	
	void Get(T_UCHT_D_CH_830 &rData)
	{
		rData.nType = nType;
		rData.nKey = nKey;
		memcpy(rData.strName, strName, sizeof(strName));
		memcpy(rData.strCaption, strCaption, sizeof(strCaption));
		memcpy(rData.strGraphName, strGraphName, sizeof(strGraphName));
		memcpy(rData.strFuncName, strFuncName, sizeof(strFuncName));
		memcpy(rData.strLableX, strLableX, sizeof(strLableX));
		memcpy(rData.strLableY, strLableY, sizeof(strLableY));
		rData.bExpX = bExpX;
		rData.bExpY = bExpY;
		rData.bLogX = bLogX;
		rData.bLogY = bLogY;
		rData.nXDecPt = nXDecPt;
		rData.nYDecPt = nYDecPt;
		rData.nFuncType = nFuncType;
		
		rData.dTemper = dTemper;
		rData.dMaxTemper = dMaxTemper;
		rData.dReactive = dReactive;
		rData.dDelay = dDelay;
		rData.dMeanTemper = dMeanTemper;
		
		rData.Data = Data;
	}
	void Set(T_UCHT_D_CH_830 src)
	{
		nType = src.nType;
		nKey = src.nKey;
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		memcpy(strGraphName, src.strGraphName, sizeof(strGraphName));
		memcpy(strFuncName, src.strFuncName, sizeof(strFuncName));
		memcpy(strLableX, src.strLableX, sizeof(strLableX));
		memcpy(strLableY, src.strLableY, sizeof(strLableY));
		bExpX = src.bExpX;
		bExpY = src.bExpY;
		bLogX = src.bLogX;
		bLogY = src.bLogY;
		nXDecPt = src.nXDecPt;
		nYDecPt = src.nYDecPt;
		nFuncType = src.nFuncType;
		
		dTemper = src.dTemper;
		dMaxTemper = src.dMaxTemper;
		dReactive = src.dReactive;
		dDelay = src.dDelay;
		dMeanTemper = src.dMeanTemper;
		
		Data = src.Data;
		aKeyXCount = (int)src.aKeyX.GetSize();
		aKeyYCount = (int)src.aKeyY.GetSize();
		aStoryKCount = (int)src.aStoryK.GetSize();
	}
};

struct T_UCHT_D_CH_RW_871
{
	int      nType;
	UINT     nKey;
	char     strName[60];
	char     strCaption[60];
	char     strGraphName[60];
	char     strFuncName[60];
	char     strLableX[60];
	char     strLableY[60];
	BOOL     bExpX;
	BOOL     bExpY;
	BOOL     bLogX;
	BOOL     bLogY;
	int      nXDecPt;
	int      nYDecPt;
	int      nFuncType;

	double dTemper;
	double dMaxTemper;
	double dReactive;
	double dDelay;
	double dMeanTemper;

	// User Defined Unit(v831 추가)
	BOOL bDefinedUnit; // User defined unit system 사용 여부
	int  nUnitLength ; // bDefinedUnit==TRUE 일 때 사용
	int  nUnitForce  ;
	int  nUnitHeat 	 ;
	int  nUnitTemper ;

	// 타입별 데이터
	T_UCHT_DATA_D_871 Data; // Grph, Stgh, Pocv, Eqrs, Sthg
	int aKeyXCount;
	int aKeyYCount;     // nType = 4 or 5 - TH : T_THRD_K, PO : T_PORD_K
	int aStoryKCount;   // nType = 6 or 7 - T_STOR_K

	void Get(T_UCHT_D_CH_871 &rData)
	{
		rData.nType = nType;
		rData.nKey = nKey;
		memcpy(rData.strName, strName, sizeof(strName));
		memcpy(rData.strCaption, strCaption, sizeof(strCaption));
		memcpy(rData.strGraphName, strGraphName, sizeof(strGraphName));
		memcpy(rData.strFuncName, strFuncName, sizeof(strFuncName));
		memcpy(rData.strLableX, strLableX, sizeof(strLableX));
		memcpy(rData.strLableY, strLableY, sizeof(strLableY));
		rData.bExpX = bExpX;
		rData.bExpY = bExpY;
		rData.bLogX = bLogX;
		rData.bLogY = bLogY;
		rData.nXDecPt = nXDecPt;
		rData.nYDecPt = nYDecPt;
		rData.nFuncType = nFuncType;

		rData.dTemper = dTemper;
		rData.dMaxTemper = dMaxTemper;
		rData.dReactive = dReactive;
		rData.dDelay = dDelay;
		rData.dMeanTemper = dMeanTemper;

		rData.bDefinedUnit = bDefinedUnit;
		rData.nUnitLength  = nUnitLength ;
		rData.nUnitForce   = nUnitForce  ;
		rData.nUnitHeat 	 = nUnitHeat 	 ;
		rData.nUnitTemper  = nUnitTemper ;

		rData.Data = Data;
	}
	void Set(T_UCHT_D_CH_871 src)
	{
		nType = src.nType;
		nKey = src.nKey;
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		memcpy(strGraphName, src.strGraphName, sizeof(strGraphName));
		memcpy(strFuncName, src.strFuncName, sizeof(strFuncName));
		memcpy(strLableX, src.strLableX, sizeof(strLableX));
		memcpy(strLableY, src.strLableY, sizeof(strLableY));
		bExpX = src.bExpX;
		bExpY = src.bExpY;
		bLogX = src.bLogX;
		bLogY = src.bLogY;
		nXDecPt = src.nXDecPt;
		nYDecPt = src.nYDecPt;
		nFuncType = src.nFuncType;

		dTemper = src.dTemper;
		dMaxTemper = src.dMaxTemper;
		dReactive = src.dReactive;
		dDelay = src.dDelay;
		dMeanTemper = src.dMeanTemper;

		bDefinedUnit = src.bDefinedUnit;
		nUnitLength  = src.nUnitLength ;
		nUnitForce   = src.nUnitForce  ;
		nUnitHeat 	 = src.nUnitHeat 	 ;
		nUnitTemper  = src.nUnitTemper ;

		Data = src.Data;
		aKeyXCount = (int)src.aKeyX.GetSize();
		aKeyYCount = (int)src.aKeyY.GetSize();
		aStoryKCount = (int)src.aStoryK.GetSize();
	}
};

struct T_UCHT_D_CH_RW
{
	int      nType;
	UINT     nKey;
	char     strName[60];
	char     strCaption[60];
	char     strGraphName[60];
	char     strFuncName[60];
	char     strLableX[60];
	char     strLableY[60];
	BOOL     bExpX;
	BOOL     bExpY;
	BOOL     bLogX;
	BOOL     bLogY;
	int      nXDecPt;
	int      nYDecPt;
	int      nFuncType;
	
	double dTemper;
	double dMaxTemper;
	double dReactive;
	double dDelay;
	double dMeanTemper;
	
	// User Defined Unit(v831 추가)
	BOOL bDefinedUnit; // User defined unit system 사용 여부
	int  nUnitLength ; // bDefinedUnit==TRUE 일 때 사용
	int  nUnitForce  ;
	int  nUnitHeat 	 ;
	int  nUnitTemper ;

	// 타입별 데이터
	T_UCHT_DATA_D Data; // Grph, Stgh, Pocv, Eqrs, Sthg
	int aKeyXCount;
	int aKeyYCount;     // nType = 4 or 5 - TH : T_THRD_K, PO : T_PORD_K
	int aStoryKCount;   // nType = 6 or 7 - T_STOR_K
	
	void Get(T_UCHT_D_CH &rData)
	{
		rData.nType = nType;
		rData.nKey = nKey;
		memcpy(rData.strName, strName, sizeof(strName));
		memcpy(rData.strCaption, strCaption, sizeof(strCaption));
		memcpy(rData.strGraphName, strGraphName, sizeof(strGraphName));
		memcpy(rData.strFuncName, strFuncName, sizeof(strFuncName));
		memcpy(rData.strLableX, strLableX, sizeof(strLableX));
		memcpy(rData.strLableY, strLableY, sizeof(strLableY));
		rData.bExpX = bExpX;
		rData.bExpY = bExpY;
		rData.bLogX = bLogX;
		rData.bLogY = bLogY;
		rData.nXDecPt = nXDecPt;
		rData.nYDecPt = nYDecPt;
		rData.nFuncType = nFuncType;
		
		rData.dTemper = dTemper;
		rData.dMaxTemper = dMaxTemper;
		rData.dReactive = dReactive;
		rData.dDelay = dDelay;
		rData.dMeanTemper = dMeanTemper;
		
		rData.bDefinedUnit = bDefinedUnit;
		rData.nUnitLength  = nUnitLength ;
		rData.nUnitForce   = nUnitForce  ;
		rData.nUnitHeat 	 = nUnitHeat 	 ;
		rData.nUnitTemper  = nUnitTemper ;

		rData.Data = Data;
	}
	void Set(T_UCHT_D_CH src)
	{
		nType = src.nType;
		nKey = src.nKey;
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		memcpy(strGraphName, src.strGraphName, sizeof(strGraphName));
		memcpy(strFuncName, src.strFuncName, sizeof(strFuncName));
		memcpy(strLableX, src.strLableX, sizeof(strLableX));
		memcpy(strLableY, src.strLableY, sizeof(strLableY));
		bExpX = src.bExpX;
		bExpY = src.bExpY;
		bLogX = src.bLogX;
		bLogY = src.bLogY;
		nXDecPt = src.nXDecPt;
		nYDecPt = src.nYDecPt;
		nFuncType = src.nFuncType;
		
		dTemper = src.dTemper;
		dMaxTemper = src.dMaxTemper;
		dReactive = src.dReactive;
		dDelay = src.dDelay;
		dMeanTemper = src.dMeanTemper;
		
		bDefinedUnit = src.bDefinedUnit;
		nUnitLength  = src.nUnitLength ;
		nUnitForce   = src.nUnitForce  ;
		nUnitHeat 	 = src.nUnitHeat 	 ;
		nUnitTemper  = src.nUnitTemper ;

		Data = src.Data;
		aKeyXCount = (int)src.aKeyX.GetSize();
		aKeyYCount = (int)src.aKeyY.GetSize();
		aStoryKCount = (int)src.aStoryK.GetSize();
	}
};

struct T_UCHT_UDRD_D
{
	T_UCHT_K key ;
	T_UCHT_D data;
	void Initialize()
	{
		key  = 0;
		data .Initialize(0);
	}
	T_UCHT_UDRD_D() { Initialize(); }
	T_UCHT_UDRD_D(const T_UCHT_UDRD_D &src) { *this = src; }
	T_UCHT_UDRD_D& operator=(const T_UCHT_UDRD_D &src)
	{
		key  = src.key ;
		data = src.data;
		return *this;
	}
};
struct T_UCHT_UDRD_D_CH
{
	T_UCHT_K key;
	T_UCHT_D_CH data;
};
struct T_UCHT_UDRD_D_CH_RW_815
{
	T_UCHT_K key;
	T_UCHT_D_CH_RW_815 data;
};
struct T_UCHT_UDRD_D_CH_RW_821
{
	T_UCHT_K key;
	T_UCHT_D_CH_RW_821 data;
};
struct T_UCHT_UDRD_D_CH_RW_830
{
	T_UCHT_K key;
	T_UCHT_D_CH_RW_830 data;
};
struct T_UCHT_UDRD_D_CH_RW_871
{
	T_UCHT_K key;
	T_UCHT_D_CH_RW_871 data;
};
struct T_UCHT_UDRD_D_CH_RW
{
	T_UCHT_K key;
	T_UCHT_D_CH_RW data;
	void GetUcht(T_UCHT_K &UCHTK, T_UCHT_D_CH &UCHTD)
	{
		UCHTK = key;
		data.Get(UCHTD);
	}
	void SetUcht(const T_UCHT_K &UCHTK, const T_UCHT_D_CH &UCHTD)
	{
		key = UCHTK;
		data.Set(UCHTD);
	}
	void Convert815(T_UCHT_UDRD_D_CH_RW_815& src)
	{
		key = src.key;
		data.nType = src.data.nType;
		data.nKey = src.data.nKey;
		memcpy(data.strName, src.data.strName, sizeof(data.strName));
		memcpy(data.strCaption, src.data.strCaption, sizeof(data.strCaption));
		memcpy(data.strGraphName, src.data.strGraphName, sizeof(data.strGraphName));
		memcpy(data.strFuncName, src.data.strFuncName, sizeof(data.strFuncName));
		memcpy(data.strLableX, src.data.strLableX, sizeof(data.strLableX));
		memcpy(data.strLableY, src.data.strLableY, sizeof(data.strLableY));
		data.bExpX = src.data.bExpX;
		data.bExpY = src.data.bExpY;
		data.bLogX = src.data.bLogX;
		data.bLogY = src.data.bLogY;
		data.nXDecPt = src.data.nXDecPt;
		data.nYDecPt = src.data.nYDecPt;
		data.nFuncType = src.data.nFuncType;
		
		data.dTemper = src.data.dTemper;
		data.dMaxTemper = src.data.dMaxTemper;
		data.dReactive = src.data.dReactive;
		data.dDelay = src.data.dDelay;
		data.dMeanTemper = src.data.dMeanTemper;
		
		data.Data = src.data.Data;
		data.aKeyXCount = src.data.aKeyXCount;
		data.aKeyYCount = src.data.aKeyYCount;
		data.aStoryKCount = src.data.aStoryKCount;    

		data.bDefinedUnit = FALSE;
		data.nUnitLength  = 1;
		data.nUnitForce   = 1;
		data.nUnitHeat 	  = 1;
		data.nUnitTemper  = 1;
	}
	void Convert821(T_UCHT_UDRD_D_CH_RW_821& src)
	{
		key = src.key;
		data.nType = src.data.nType;
		data.nKey = src.data.nKey;
		memcpy(data.strName, src.data.strName, sizeof(data.strName));
		memcpy(data.strCaption, src.data.strCaption, sizeof(data.strCaption));
		memcpy(data.strGraphName, src.data.strGraphName, sizeof(data.strGraphName));
		memcpy(data.strFuncName, src.data.strFuncName, sizeof(data.strFuncName));
		memcpy(data.strLableX, src.data.strLableX, sizeof(data.strLableX));
		memcpy(data.strLableY, src.data.strLableY, sizeof(data.strLableY));
		data.bExpX = src.data.bExpX;
		data.bExpY = src.data.bExpY;
		data.bLogX = src.data.bLogX;
		data.bLogY = src.data.bLogY;
		data.nXDecPt = src.data.nXDecPt;
		data.nYDecPt = src.data.nYDecPt;
		data.nFuncType = src.data.nFuncType;
		
		data.dTemper = src.data.dTemper;
		data.dMaxTemper = src.data.dMaxTemper;
		data.dReactive = src.data.dReactive;
		data.dDelay = src.data.dDelay;
		data.dMeanTemper = src.data.dMeanTemper;
		
		data.Data.Convert821(src.data.Data);
		data.aKeyXCount = src.data.aKeyXCount;
		data.aKeyYCount = src.data.aKeyYCount;
		data.aStoryKCount = src.data.aStoryKCount;    

		data.bDefinedUnit = FALSE;
		data.nUnitLength  = 1;
		data.nUnitForce   = 1;
		data.nUnitHeat 	  = 1;
		data.nUnitTemper  = 1;
	}
	void Convert830(T_UCHT_UDRD_D_CH_RW_830& src)
	{
		key = src.key;
		data.nType = src.data.nType;
		data.nKey = src.data.nKey;
		memcpy(data.strName, src.data.strName, sizeof(data.strName));
		memcpy(data.strCaption, src.data.strCaption, sizeof(data.strCaption));
		memcpy(data.strGraphName, src.data.strGraphName, sizeof(data.strGraphName));
		memcpy(data.strFuncName, src.data.strFuncName, sizeof(data.strFuncName));
		memcpy(data.strLableX, src.data.strLableX, sizeof(data.strLableX));
		memcpy(data.strLableY, src.data.strLableY, sizeof(data.strLableY));
		data.bExpX = src.data.bExpX;
		data.bExpY = src.data.bExpY;
		data.bLogX = src.data.bLogX;
		data.bLogY = src.data.bLogY;
		data.nXDecPt = src.data.nXDecPt;
		data.nYDecPt = src.data.nYDecPt;
		data.nFuncType = src.data.nFuncType;
		
		data.dTemper = src.data.dTemper;
		data.dMaxTemper = src.data.dMaxTemper;
		data.dReactive = src.data.dReactive;
		data.dDelay = src.data.dDelay;
		data.dMeanTemper = src.data.dMeanTemper;
		
		data.Data = src.data.Data;
		data.aKeyXCount = src.data.aKeyXCount;
		data.aKeyYCount = src.data.aKeyYCount;
		data.aStoryKCount = src.data.aStoryKCount; 

		data.bDefinedUnit = FALSE;
		data.nUnitLength  = 1;
		data.nUnitForce   = 1;
		data.nUnitHeat 	  = 1;
		data.nUnitTemper  = 1;
	}
	void Convert871(T_UCHT_UDRD_D_CH_RW_871& src)
	{
		key = src.key;
		data.nType = src.data.nType;
		data.nKey = src.data.nKey;
		memcpy(data.strName, src.data.strName, sizeof(data.strName));
		memcpy(data.strCaption, src.data.strCaption, sizeof(data.strCaption));
		memcpy(data.strGraphName, src.data.strGraphName, sizeof(data.strGraphName));
		memcpy(data.strFuncName, src.data.strFuncName, sizeof(data.strFuncName));
		memcpy(data.strLableX, src.data.strLableX, sizeof(data.strLableX));
		memcpy(data.strLableY, src.data.strLableY, sizeof(data.strLableY));
		data.bExpX = src.data.bExpX;
		data.bExpY = src.data.bExpY;
		data.bLogX = src.data.bLogX;
		data.bLogY = src.data.bLogY;
		data.nXDecPt = src.data.nXDecPt;
		data.nYDecPt = src.data.nYDecPt;
		data.nFuncType = src.data.nFuncType;

		data.dTemper = src.data.dTemper;
		data.dMaxTemper = src.data.dMaxTemper;
		data.dReactive = src.data.dReactive;
		data.dDelay = src.data.dDelay;
		data.dMeanTemper = src.data.dMeanTemper;

		data.Data.Convert871(data.nType, src.data.Data);
		data.aKeyXCount = src.data.aKeyXCount;
		data.aKeyYCount = src.data.aKeyYCount;
		data.aStoryKCount = src.data.aStoryKCount; 

		data.bDefinedUnit = FALSE;
		data.nUnitLength  = 1;
		data.nUnitForce   = 1;
		data.nUnitHeat 	  = 1;
		data.nUnitTemper  = 1;
	}
};
struct T_UCHT_UNIT
{
	T_UCHT_EQRS_D_UNIT Eqrs;

	// 이 값들은 그때그때 단위계가 달라지기 때문에 차트 생성 시 단위계도 설정해주세요.
	T_UCHT_UNIT()
	{
		dTemper     = D_UNITSYS_NONE;
		dMaxTemper  = D_UNITSYS_NONE;
		dReactive   = D_UNITSYS_NONE;
		dDelay      = D_UNITSYS_NONE;
		dMeanTemper = D_UNITSYS_NONE;
	}
	double dTemper;
	double dMaxTemper;
	double dReactive;
	double dDelay;
	double dMeanTemper;
};
#define HASHSIZEUCHT 53

//////////////////////////////////////////////////////////////////////////
// User Defined Table Template

#define T_UTPL_K unsigned int
struct T_UTPL_D
{
	CString  strName;
	CString  strCaption;  // Caption
	CString  strFilePath; // 상대경로 (모델위치에서 부터..)

	T_UTPL_D() { Initialize(); }
	void Initialize()
	{
		strName = "";
		strCaption = "";
		strFilePath = "";
	}
	T_UTPL_D& operator=(const T_UTPL_D &src)
	{
		strName = src.strName;
		strCaption = src.strCaption;
		strFilePath = src.strFilePath;
		
		return *this;
	}
	T_UTPL_D(const T_UTPL_D &src) { *this = src; }
};

struct T_UTPL_D_CH
{
	char     strName[60];
	char     strCaption[60];
	char     strFilePath[60];

	T_UTPL_D_CH() {}
	void Get(T_UTPL_D &rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		ConvertCharStr(strFilePath, rData.strFilePath, sizeof(strFilePath));
	}
	void Set(T_UTPL_D src)
	{
		ConvertStrChar(src.strName, strName, sizeof(strName));
		ConvertStrChar(src.strCaption, strCaption, sizeof(strCaption));
		ConvertStrChar(src.strFilePath, strFilePath, sizeof(strFilePath));
	}
	T_UTPL_D_CH& operator=(const T_UTPL_D_CH &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		memcpy(strFilePath, src.strFilePath, sizeof(strFilePath));
		
		return *this;
	}
	T_UTPL_D_CH(const T_UTPL_D_CH &src) { *this = src; }
};

struct T_UTPL_UDRD_D
{
	T_UTPL_K key ;
	T_UTPL_D data;
	void Initialize()
	{
		key  = 0;
		data .Initialize();
	}
	T_UTPL_UDRD_D() { Initialize(); }
	T_UTPL_UDRD_D(const T_UTPL_UDRD_D &src) { *this = src; }
	T_UTPL_UDRD_D& operator=(const T_UTPL_UDRD_D &src)
	{
		key  = src.key ;
		data = src.data;
		return *this;
	}
};
struct T_UTPL_UDRD_D_CH
{
	T_UTPL_K key;
	T_UTPL_D_CH data;
};
struct T_UTPL_UNIT
{
	T_UTPL_UNIT()
	{
		NoUnit=D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEUTPL 53

//////////////////////////////////////////////////////////////////////////
// Image Files

#define T_UIMG_K unsigned int
struct T_UIMG_D
{
	CString  strName;
	CString  strCaption;  // Caption
	CString  strFilePath; // 상대경로 (모델위치에서 부터..)
	
	T_UIMG_D() { Initialize(); }
	void Initialize()
	{
		strName = "";
		strCaption = "";
		strFilePath = "";
	}
	T_UIMG_D& operator=(const T_UIMG_D &src)
	{
		strName = src.strName;
		strCaption = src.strCaption;
		strFilePath = src.strFilePath;
		
		return *this;
	}
	T_UIMG_D(const T_UIMG_D &src) { *this = src; }
};

struct T_UIMG_D_CH
{
	char     strName[60];
	char     strCaption[60];
	char     strFilePath[60];
	
	T_UIMG_D_CH() {}
	void Get(T_UIMG_D &rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		ConvertCharStr(strFilePath, rData.strFilePath, sizeof(strFilePath));
	}
	void Set(T_UIMG_D src)
	{
		ConvertStrChar(src.strName, strName, sizeof(strName));
		ConvertStrChar(src.strCaption, strCaption, sizeof(strCaption));
		ConvertStrChar(src.strFilePath, strFilePath, sizeof(strFilePath));
	}
	T_UIMG_D_CH& operator=(const T_UIMG_D_CH &src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		memcpy(strCaption, src.strCaption, sizeof(strCaption));
		memcpy(strFilePath, src.strFilePath, sizeof(strFilePath));
		
		return *this;
	}
	T_UIMG_D_CH(const T_UIMG_D_CH &src) { *this = src; }
};

struct T_UIMG_UDRD_D
{
	T_UIMG_K key ;
	T_UIMG_D data;
	void Initialize()
	{
		key  = 0;
		data .Initialize();
	}
	T_UIMG_UDRD_D() { Initialize(); }
	T_UIMG_UDRD_D(const T_UIMG_UDRD_D &src) { *this = src; }
	T_UIMG_UDRD_D& operator=(const T_UIMG_UDRD_D &src)
	{
		key  = src.key ;
		data = src.data;
		return *this;
	}
};
struct T_UIMG_UDRD_D_CH
{
	T_UIMG_K key;
	T_UIMG_D_CH data;
};
struct T_UIMG_UNIT
{
	T_UIMG_UNIT()
	{
		NoUnit=D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEUIMG 53

////////////////////////////////////////////////////////////////////////////////////////////////////////
// 보고서 Text 정보 (Design Summary)
// \wg_tb\TBAMRUDTData.h의 define도 함께 수정할것.

#define T_USUM_K unsigned int
struct T_USUM_D
{
	CString strName         ; // Dynamic Report Text Name
	CString strCaption      ;  // Caption
	int     nRcsTypeKind    ; // BCHK, BDGN, CCHK, CDGN, RCHK, RDGN.
	int     nSize           ; // RCSC_iLargeBeamChk=0, RCSC_iSmallBeamChk=1
	int     nSortRadio      ; // Sorted by Member(0) / Section(1)
	int     nResultViewRadio; // Result View Option All(0) / OK(1) /NG(2)
	int     nPrimaryRadio   ; // Primary Sorting Option SECT(0) /MEMB(1)
	int     nResRadio       ; // Results Strength(0) /Serviceability(1)
	CArray<int,int&> arSelItem; // Seleted Item
	int nResultViewBERadio; // Result View B.E. Option All(0)/Req.(1)/Not Req.(2)
													// RC Wall Design, RC Wall Check 에서만 사용.
													// 다른 곳에서는 Defalt(0) 값으로 갖고 있으면 된다.

	T_USUM_D() { Initialize(); }
	void Initialize()
	{
		strName           = "";
		strCaption        = "";
		nRcsTypeKind      = 0;
		nSize             = 0;			
		nSortRadio        = 0;
		nResultViewRadio  = 0;
		nPrimaryRadio     = 0;
		nResRadio         = 0;
		arSelItem.RemoveAll();
		nResultViewBERadio= 0;
	}
	T_USUM_D& operator=(const T_USUM_D &src)
	{
		strName           = src.strName;
		strCaption        = src.strCaption;
		nRcsTypeKind      = src.nRcsTypeKind;
		nSize             = src.nSize;
		nSortRadio        = src.nSortRadio;
		nResultViewRadio  = src.nResultViewRadio;
		nPrimaryRadio     = src.nPrimaryRadio;
		nResRadio         = src.nResRadio;
		arSelItem.Copy(src.arSelItem);
		nResultViewBERadio= src.nResultViewBERadio;
		
		return *this;
	}
	T_USUM_D(const T_USUM_D &src) { *this = src; }
};

struct T_USUM_D_CH
{
	char    strName[60]     ;
	char    strCaption[60]  ;
	int     nRcsTypeKind    ;
	int     nSize           ;
	int     nSortRadio      ;
	int     nResultViewRadio;
	int     nPrimaryRadio   ;
	int     nResRadio       ;
	CArray<int,int&> arSelItem;
	int     nResultViewBERadio; // Result View B.E. Option All(0)/Req.(1)/Not Req.(2)
															// RC Wall Design, RC Wall Check 에서만 사용.
															// 다른 곳에서는 Defalt(0) 값으로 갖고 있으면 된다.

	T_USUM_D_CH() {}
	void Get(T_USUM_D &rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strCaption, rData.strCaption, sizeof(strCaption));
		rData.nRcsTypeKind     = nRcsTypeKind    ;
		rData.nSize            = nSize           ;
		rData.nSortRadio       = nSortRadio      ;
		rData.nResultViewRadio = nResultViewRadio;
		rData.nPrimaryRadio    = nPrimaryRadio   ;
		rData.nResRadio        = nResRadio       ;
		rData.arSelItem.Copy(arSelItem);
		rData.nResultViewBERadio = nResultViewBERadio;
	}
	void Set(T_USUM_D Data)
	{
		ConvertStrChar(Data.strName, strName, sizeof(strName));
		ConvertStrChar(Data.strCaption, strCaption, sizeof(strCaption));
		nRcsTypeKind     = Data.nRcsTypeKind    ;
		nSize            = Data.nSize           ;
		nSortRadio       = Data.nSortRadio      ;
		nResultViewRadio = Data.nResultViewRadio;
		nPrimaryRadio    = Data.nPrimaryRadio   ;
		nResRadio        = Data.nResRadio       ;
		arSelItem.Copy(Data.arSelItem);
		nResultViewBERadio = Data.nResultViewBERadio;
	}
};

struct T_USUM_D_CH_RW
{
	char    strName[60]     ;
	char    strCaption[60]  ;
	int     nRcsTypeKind    ;
	int     nSize           ;
	int     nSortRadio      ;
	int     nResultViewRadio;
	int     nPrimaryRadio   ;
	int     nResRadio       ;
	int     nCount_arSelItem;
	int     nResultViewBERadio; // Result View B.E. Option All(0)/Req.(1)/Not Req.(2)
															// RC Wall Design, RC Wall Check 에서만 사용.
															// 다른 곳에서는 Defalt(0) 값으로 갖고 있으면 된다.

	void Get(T_USUM_D_CH &rData)
	{
		memcpy(rData.strName, strName, sizeof(strName));
		memcpy(rData.strCaption, strCaption, sizeof(strCaption));
		rData.nRcsTypeKind     = nRcsTypeKind    ;
		rData.nSize            = nSize           ;
		rData.nSortRadio       = nSortRadio      ;
		rData.nResultViewRadio = nResultViewRadio;
		rData.nPrimaryRadio    = nPrimaryRadio   ;
		rData.nResRadio        = nResRadio       ;
		rData.arSelItem.SetSize(nCount_arSelItem);
		rData.nResultViewBERadio = nResultViewBERadio;
	}
	void Set(const T_USUM_D_CH& Data)
	{
		memcpy(strName, Data.strName, sizeof(strName));
		memcpy(strCaption, Data.strCaption, sizeof(strCaption));
		nRcsTypeKind     = Data.nRcsTypeKind    ;
		nSize            = Data.nSize           ;
		nSortRadio       = Data.nSortRadio      ;
		nResultViewRadio = Data.nResultViewRadio;
		nPrimaryRadio    = Data.nPrimaryRadio   ;
		nResRadio        = Data.nResRadio       ;
		nCount_arSelItem = (int)Data.arSelItem.GetSize();
		nResultViewBERadio = Data.nResultViewBERadio;
	}
};

struct T_USUM_UDRD_D
{
	T_USUM_K key ;
	T_USUM_D data;
	void Initialize()
	{
		key  = 0;
		data .Initialize();
	}
	T_USUM_UDRD_D() { Initialize(); }
	T_USUM_UDRD_D(const T_USUM_UDRD_D &src) { *this = src; }
	T_USUM_UDRD_D& operator=(const T_USUM_UDRD_D &src)
	{
		key  = src.key ;
		data = src.data;
		return *this;
	}
};
struct T_USUM_UDRD_D_CH
{
	T_USUM_K key;
	T_USUM_D_CH data;
};
struct T_USUM_UDRD_D_CH_RW
{
	T_USUM_K key;
	T_USUM_D_CH_RW data;
	void GetUsum(T_USUM_K &USUMK, T_USUM_D_CH &USUMD)
	{
		USUMK = key;
		data.Get(USUMD);
	}
	void SetUsum(const T_USUM_K &USUMK, const T_USUM_D_CH &USUMD)
	{
		key = USUMK;
		data.Set(USUMD);
	}
};
struct T_USUM_UNIT
{
	T_USUM_UNIT()
	{
		NoUnit=D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEUSUM 53

//////////////////////////////////////////////////////////////////////////
// Report Header/Footer

#define D_UFTR_START          0
#define D_UFTR_PROJECT_NAME   1
#define D_UFTR_REVISION       2
#define D_UFTR_USER_NAME      3
#define D_UFTR_EMAIL          4
#define D_UFTR_ADDRESS        5
#define D_UFTR_TELEPHONE      6
#define D_UFTR_FAX            7
#define D_UFTR_CLIENT         8
#define D_UFTR_TITLE          9
#define D_UFTR_FILE_NAME      10
#define D_UFTR_CREATED        11
#define D_UFTR_DIRECTORY      12
#define D_UFTR_MODIFIED       13
#define D_UFTR_FILE_SIZE      14
#define D_UFTR_END            15  // 30이 넘으면 aHeader, aFooter의 개수를 수정해야함. (DB 변경됨)

#define T_UFTR_K unsigned int
struct T_UFTR_D
{
	int nHeaderNum;
	int nFooterNum;
	int aHeader[30];  // Header로 선택한 번호 리스트 (순서있음, 정렬하지 말것)
	int aFooter[30];  // Footer로 선택한 번호 리스트 (순서있음, 정렬하지 말것)
	
	T_UFTR_D() { Initialize(); }
	void Initialize()
	{
		nHeaderNum = 0;
		nFooterNum = 0;
		memset(aHeader, 0, sizeof(aHeader));
		memset(aFooter, 0, sizeof(aFooter));
	}
	T_UFTR_D& operator=(const T_UFTR_D &src)
	{
		nHeaderNum = src.nHeaderNum;
		nFooterNum = src.nFooterNum;
		memcpy(aHeader, src.aHeader, sizeof(aHeader));
		memcpy(aFooter, src.aFooter, sizeof(aFooter));
		
		return *this;
	}
	T_UFTR_D(const T_UFTR_D &src) { *this = src; }
};

struct T_UFTR_UDRD_D
{
	T_UFTR_K key ;
	T_UFTR_D data;
	void Initialize()
	{
		key  = 0;
		data .Initialize();
	}
	T_UFTR_UDRD_D() { Initialize(); }
	T_UFTR_UDRD_D(const T_UFTR_UDRD_D &src) { *this = src; }
	T_UFTR_UDRD_D& operator=(const T_UFTR_UDRD_D &src)
	{
		key  = src.key ;
		data = src.data;
		return *this;
	}
};
struct T_UFTR_UNIT
{
	T_UFTR_UNIT()
	{
		NoUnit=D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEUFTR 53

#undef D_TB_COMP_CNT
#undef D_TB_HEADER_CNT
#undef D_TB_SORT_CNT
#undef D_TB_SPOS_CNT

// Named View : View Point만 저장함
#define T_NMDV_K UINT
struct T_NMDV_D
{
	char name[80];
	T_RETextMod_D RETextD;
	int nUnitLength;

	T_NMDV_D() { Initialize(); }
	void Initialize()
	{
		memset(name, 0, sizeof(name));
		RETextD.Initialize();
		nUnitLength = D_UNITSYS_LENGTH_INDEX_M;
	}
};

struct T_NMDV_UDRD_D
{
	T_NMDV_K key;
	T_NMDV_D data;
};

struct T_NMDV_UNIT
{
	T_NMDV_UNIT()
	{
		nNoUnit = D_UNITSYS_NONE;
	}
	int nNoUnit;
};
#define HASHSIZENMDV 53



enum E_TEXT_TYPE
{
	E_TEXT_TYPE_TH_DISP = 0,
	E_TEXT_TYPE_TH_VEL,
	E_TEXT_TYPE_TH_ACCE,
	E_TEXT_TYPE_TH_BEAM_F,
	E_TEXT_TYPE_TH_TRUSS_F,
	E_TEXT_TYPE_TH_PLATE_F,
	E_TEXT_TYPE_TH_PLATEUNIT_F,
	E_TEXT_TYPE_TH_WALL_F,
	E_TEXT_TYPE_TH_PLANE_STRESS_F,
	E_TEXT_TYPE_TH_PLANE_STRAIN_F,
	E_TEXT_TYPE_TH_SOLID_F,
	E_TEXT_TYPE_TH_BEAM_S,
	E_TEXT_TYPE_TH_TRUSS_S,
	E_TEXT_TYPE_TH_PLATE_S,
	E_TEXT_TYPE_TH_PLANE_STRESS_S,
	E_TEXT_TYPE_TH_PLANE_STRAIN_S,
	E_TEXT_TYPE_TH_SOLID_S,
	E_TEXT_TYPE_TH_GLINK_F,
	E_TEXT_TYPE_TH_GLINK_D,

	E_TEXT_TYPE_PO_DISP,
	E_TEXT_TYPE_PO_BEAM_F,
	E_TEXT_TYPE_PO_TRUSS_F,
	E_TEXT_TYPE_PO_WALL_F,
	E_TEXT_TYPE_PO_BEAM_S,
	E_TEXT_TYPE_PO_TRUSS_S,
	E_TEXT_TYPE_PO_GLINK_F,
	E_TEXT_TYPE_PO_GLINK_D,
	E_TEXT_TYPE_PO_ELINK_F,
	E_TEXT_TYPE_PO_ELINK_D,

	E_TEXT_TYPE_MAX
};

enum E_TEXT_TYPE_TH_NODE_REF_TYPE
{
	E_TEXT_TYPE_TH_NODE_REF_TYPE_GROUND = 1,
	E_TEXT_TYPE_TH_NODE_REF_TYPE_ADD_GROUND_MOTION,
	E_TEXT_TYPE_TH_NODE_REF_TYPE_ANOTHER_NODE
};

struct T_UTXT_D // Text Output을 API로 출력하기 위한 구조체임(DB 구조체 아님)
{
	CString strName;
	E_TEXT_TYPE  nType; // D_TYPE_XXX
	BOOL bAll;
	BOOL bPartAll;

	CArray<UINT, UINT> aKeys;
	CArray<UINT, UINT> aLCKeys;

	CString EXPORT_PATH;
	CString StrFormat;
	BOOL bStepUser;
	double dFrom;
	double dTo;
	int    nStep;

	int nRefType; // 1:Ground, 2:Add Ground Motion, 3:Another Node
	int nNodeNum;
	CStringArray Component;
	CStringArray Part;

	BOOL bDefinedUnit; // User defined unit system 사용 여부
	int  nUnitLength; // bDefinedUnit==TRUE 일 때 사용
	int  nUnitForce;
	int  nUnitHeat;
	int  nUnitTemper;

	T_UTXT_D() { Initialize(); }
	void Initialize()
	{
		strName = _T("");
		nType = E_TEXT_TYPE_TH_DISP;
		bAll = FALSE;
		bPartAll = FALSE;
		EXPORT_PATH = "";
		StrFormat = _T("%.3le");
		bStepUser = FALSE;
		dFrom = 0.0;
		dTo = 0.0;
		nStep = 1;

		nRefType = E_TEXT_TYPE_TH_NODE_REF_TYPE_GROUND;
		nNodeNum = 0;

		bDefinedUnit = FALSE;
		nUnitLength = 1;
		nUnitForce = 1;
		nUnitHeat = 1;
		nUnitTemper = 1;

		aKeys.RemoveAll();
		aLCKeys.RemoveAll();
		Part.RemoveAll();
		Component.RemoveAll();
	}
	bool IsTimeHistoryTxt() const
	{
		if(nType >= E_TEXT_TYPE_TH_DISP && nType < E_TEXT_TYPE_PO_DISP)
			return true;
		else
			return false;
	}
	bool IsNodeTypeTxt() const
	{
		if (nType >= E_TEXT_TYPE_TH_DISP && nType < E_TEXT_TYPE_TH_BEAM_F)
			return true;
		else if(nType == E_TEXT_TYPE_PO_DISP) return true;
		else
			return false;
	}
	bool IsElemTypeTxt() const
	{
		if (nType >= E_TEXT_TYPE_TH_BEAM_F && nType < E_TEXT_TYPE_TH_GLINK_F)
			return true;
		else if (nType >= E_TEXT_TYPE_PO_BEAM_F && nType < E_TEXT_TYPE_PO_GLINK_F)
			return true;
		else
			return false;
	}
	T_UTXT_D& operator=(const T_UTXT_D& src)
	{
		strName = src.strName;
		nType = src.nType;
		bAll = src.bAll;
		bPartAll= src.bPartAll;
		EXPORT_PATH = src.EXPORT_PATH;
		StrFormat = src.StrFormat;
		bStepUser = src.bStepUser;
		dFrom = src.dFrom;
		dTo = src.dTo;
		nStep = src.nStep;
		nRefType = src.nRefType;
		nNodeNum = src.nNodeNum;

		bDefinedUnit = src.bDefinedUnit;
		nUnitLength = src.nUnitLength;
		nUnitForce = src.nUnitForce;
		nUnitHeat = src.nUnitHeat;
		nUnitTemper = src.nUnitTemper;
		aKeys.Copy(src.aKeys);
		aLCKeys.Copy(src.aLCKeys);
		Part.Copy(src.Part);
		Component.Copy(src.Component);

		return *this;
	}
	T_UTXT_D(const T_UTXT_D& src) { *this = src; }
};


#pragma pack(pop)

#endif  // __DB_ST_DT_FIGR_H__
