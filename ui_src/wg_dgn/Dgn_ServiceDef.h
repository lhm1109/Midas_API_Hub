#ifndef  __DGN_SERVICEDEF_H__
#define  __DGN_SERVICEDEF_H__

#include "..\wg_db\wg_db_DBDoc.h"


struct _DGN_AGEN_DATA
{
	int aiIn[4];
	double adOut[4];
	void Initialize()
	{
		for(int i=0; i<4; i++)
		{
			aiIn[i]=0;
			adOut[i]=0.0;
		}
	}
};

struct _CRC_FYPC_K
{
	UINT ElemK;
	UINT DgnTdgrK;
	int nPos;
	void Initialize()
	{
		ElemK = 0;
		DgnTdgrK=0;
		nPos=0;
	}
};

struct _CRC_COMF_K
{
	UINT ElemK;
	UINT DgnLcomK;
	int nPos;
	void Initialize()
	{
		ElemK = 0;
		DgnLcomK=0;
		nPos=0;
	}

	operator unsigned long() {return (DgnLcomK<<24) + (ElemK<<1) + nPos; }

	BOOL operator==(const _CRC_COMF_K& rKey) const
	{
		if(ElemK == rKey.ElemK && DgnLcomK == rKey.DgnLcomK && nPos == rKey.nPos) 
			return TRUE;
		else 
			return FALSE;
	}

};

struct _CRC_COMF_D
{
	UINT OrgLcomK;
	int iKind;
	int iMax;
	double dFxx,dFyy,dFzz;
	double dMux,dMuy,dMuz;

	void Initialize()
	{
		OrgLcomK = 0;
		iKind = 0;
		iMax = 0;
		dFxx = dFyy = dFzz = 0.0;
		dMux = dMuy = dMuz = 0.0;
	}
};

//class CCRCDataCtrl;
//#include "CRCDataCtrl.h"

struct _CRC_IMP_DATA
{
	int iTotLineNum;
	ArrElemPairKey FysgKList;
	CArray<UINT, UINT> FypcKList;
	ArrElemPairKey FycmKList;
	ArrElemPairKey BdcwKList;
	ArrElemPairKey TbarKList;
	ArrElemPairKey BdcrKList;
	ArrElemPairKey FssgKList;
	ArrElemPairKey FsmsKList;
	ArrElemPairKey CwmsKList;
	ArrElemPairKey CumsKList;
	ArrElemPairKey CrmsKList;
	ArrElemPairKey FsmtKList;
	ArrElemPairKey CwmtKList;
	ArrElemPairKey CumtKList;
	ArrElemPairKey CrmtKList;
	CArray<UINT,UINT> FassCmpwKList;
	CArray<UINT,UINT> FmssCmpwKList;
	CArray<UINT,UINT> FprtCmpwKList;
	CArray<UINT,UINT> FwhlCmpwKList;

	CMap<UINT,UINT,UINT,UINT> DgnTdgrMap; //DgnTdgrK, TdgrK
	CMap<UINT,UINT,UINT,UINT> DgnTdnaMap; //DgnTdgrK, TdnaK
	CArray<_CRC_FYPC_K,_CRC_FYPC_K&> FypcBaseKList; 

	CMap<UINT, UINT, UINT, UINT> DgnLcomKMap;
	CMap<_CRC_COMF_K,_CRC_COMF_K,_CRC_COMF_D,_CRC_COMF_D&> ComfDMap;
	
	//CCRCDataCtrl* DataCtrl;

	void Initialize()
	{
		iTotLineNum=0;
		FysgKList.RemoveAll();
		FypcKList.RemoveAll();
		FycmKList.RemoveAll();
		BdcwKList.RemoveAll();
		TbarKList.RemoveAll();
		BdcrKList.RemoveAll();
		FssgKList.RemoveAll();
		FsmsKList.RemoveAll();
		CwmsKList.RemoveAll();
		CumsKList.RemoveAll();
		CrmsKList.RemoveAll();
		FsmtKList.RemoveAll();
		CwmtKList.RemoveAll();
		CumtKList.RemoveAll();
		CrmtKList.RemoveAll();
		FassCmpwKList.RemoveAll();
		FmssCmpwKList.RemoveAll();
		FprtCmpwKList.RemoveAll();
		FwhlCmpwKList.RemoveAll();

		DgnTdgrMap.RemoveAll();
		DgnTdnaMap.RemoveAll();
		FypcBaseKList.RemoveAll();

		DgnLcomKMap.RemoveAll();
		ComfDMap.RemoveAll();
	}

};

struct _PSC_KSCE_DATA
{
	int iTotLineNum;
	ArrElemPairKey FysgKList;
	ArrElemPairKey FycmKList;
	ArrElemPairKey BdcwKList;
	ArrElemPairKey TbarKList;
	ArrElemPairKey FssgKList;
	ArrElemPairKey FsmsKList;
	ArrElemPairKey FsmtKList;
	ArrElemPairKey FpmsJtgKList;
	ArrElemPairKey FpmtJtgKList;
	
	CArray<UINT,UINT> FypcKsceKList;
	ArrElemPairKey BdcyKsceKList;
	ArrElemPairKey BdczKsceKList;
	ArrElemPairKey CumsKsceKList;
	ArrElemPairKey CrmtKsceKList;
	CArray<UINT,UINT> TndnKsceKList;

	
	CMap<UINT, UINT, UINT, UINT> DgnLcomKMap;
	CMap<_CRC_COMF_K,_CRC_COMF_K,_CRC_COMF_D,_CRC_COMF_D&> ComfDMap;
	CMapEx<int, int, _CRC_COMF_K, _CRC_COMF_K&> mComfKey;

	void Initialize()
	{   
		iTotLineNum=0;
		FysgKList.RemoveAll();
		FycmKList.RemoveAll();
		BdcwKList.RemoveAll();
		TbarKList.RemoveAll();
		FssgKList.RemoveAll();
		FsmsKList.RemoveAll();
		FsmtKList.RemoveAll();
		FpmsJtgKList.RemoveAll();
		FpmtJtgKList.RemoveAll();

		FypcKsceKList.RemoveAll();
		BdcyKsceKList.RemoveAll();
		BdczKsceKList.RemoveAll();
		CumsKsceKList.RemoveAll();
		CrmtKsceKList.RemoveAll();
		TndnKsceKList.RemoveAll();

		DgnLcomKMap.RemoveAll();
		ComfDMap.RemoveAll();
		mComfKey.RemoveAll();
	}

};

struct _PSC_JTG_DATA
{
	int iTotLineNum;
	ArrElemPairKey FysgKList;
	ArrElemPairKey FycmJtgKList;
	ArrElemPairKey BdcwKList;
	CArray<UINT,UINT> FypcJtgKList;
	ArrElemPairKey FpmsJtgKList;
	ArrElemPairKey FpmtJtgKList;
	ArrElemPairKey TbarJtgKList;
	CArray<UINT,UINT> TndnJtgKList;
	ArrElemPairKey BdcyJtgKList;
	ArrElemPairKey CumsJtgKList;
	ArrElemPairKey CrmtJtgKList;
	ArrElemPairKey CscsJtgKList;//add by maxiao 2008.01.14

	CMap<UINT, UINT, UINT, UINT> DgnLcomKMap;
	CMap<_CRC_COMF_K,_CRC_COMF_K,_CRC_COMF_D,_CRC_COMF_D&> ComfDMap;

	void Initialize()
	{
		iTotLineNum=0;
		FysgKList.RemoveAll();
		FycmJtgKList.RemoveAll();
		BdcwKList.RemoveAll();    
		FypcJtgKList.RemoveAll();
		FpmsJtgKList.RemoveAll();
		FpmtJtgKList.RemoveAll();
		TbarJtgKList.RemoveAll();
		TndnJtgKList.RemoveAll();
		BdcyJtgKList.RemoveAll();
		CumsJtgKList.RemoveAll();
		CrmtJtgKList.RemoveAll();
		CscsJtgKList.RemoveAll();

		DgnLcomKMap.RemoveAll();
		ComfDMap.RemoveAll();
	}

};

struct _PSC_TB_DATA
{
	int iTotLineNum;
	CArray<UINT,UINT> Beam_PSC_TBSeqKList;             
	CArray<UINT,UINT> Beam_cross_bend_TBSeqKList;      
	CArray<UINT,UINT> Beam_incline_shear_TBSeqKList;   
	CArray<UINT,UINT> Beam_cross_fracture_TBSeqKList;  
	CArray<UINT,UINT> Beam_incline_fracture_TBSeqKList;
	CArray<UINT,UINT> Beam_incline_tp_TBSeqKList;      
	CArray<UINT,UINT> Beam_con_c_SS_TBSeqKList;        
	CArray<UINT,UINT> Beam_con_t_SS_TBSeqKList;        
	CArray<UINT,UINT> Beam_tendon_SS_TBSeqKList;       
	CArray<UINT,UINT> Beam_delta_tendon_TBSeqKList;    
	CArray<UINT,UINT> Beam_shear_TBSeqKList;           
	CArray<UINT,UINT> Beam_crack_TBSeqKList;         
	CArray<UINT,UINT> Beam_tendon_CS_TBSeqKList;       
	CArray<UINT,UINT> Beam_con_CS_TBSeqKList;          
	CArray<UINT,UINT> Column_PSC_TBSeqKList;           
	CArray<UINT,UINT> Column_compressive_TBSeqKList;   
	CArray<UINT,UINT> Column_tension_TBSeqKList;       
	CArray<UINT,UINT> Column_cross_fracture_TBSeqKList;
	CArray<UINT,UINT> Column_con_c_SS_TBSeqKList;      
	CArray<UINT,UINT> Column_con_t_SS_TBSeqKList;      
	CArray<UINT,UINT> Column_tendon_SS_TBSeqKList;     
	CArray<UINT,UINT> Column_shear_TBSeqKList;         
	CArray<UINT,UINT> Column_tendon_CS_TBSeqKList;     
	CArray<UINT,UINT> Column_con_CS_TBSeqKList; 

	CMap<UINT, UINT, UINT, UINT> DgnLcomKMap;
	CMap<_CRC_COMF_K,_CRC_COMF_K,_CRC_COMF_D,_CRC_COMF_D&> ComfDMap;

	void Initialize()
	{
		iTotLineNum=0;
		Beam_PSC_TBSeqKList.RemoveAll();            
		Beam_cross_bend_TBSeqKList.RemoveAll();     
		Beam_incline_shear_TBSeqKList.RemoveAll();   
		Beam_cross_fracture_TBSeqKList.RemoveAll();  
		Beam_incline_fracture_TBSeqKList.RemoveAll();
		Beam_incline_tp_TBSeqKList.RemoveAll();      
		Beam_con_c_SS_TBSeqKList.RemoveAll();        
		Beam_con_t_SS_TBSeqKList.RemoveAll();        
		Beam_tendon_SS_TBSeqKList.RemoveAll();       
		Beam_delta_tendon_TBSeqKList.RemoveAll();    
		Beam_shear_TBSeqKList.RemoveAll();           
		Beam_crack_TBSeqKList.RemoveAll();         
		Beam_tendon_CS_TBSeqKList.RemoveAll();       
		Beam_con_CS_TBSeqKList.RemoveAll();          
		Column_PSC_TBSeqKList.RemoveAll();           
		Column_compressive_TBSeqKList.RemoveAll();   
		Column_tension_TBSeqKList.RemoveAll();       
		Column_cross_fracture_TBSeqKList.RemoveAll();
		Column_con_c_SS_TBSeqKList.RemoveAll();      
		Column_con_t_SS_TBSeqKList.RemoveAll();      
		Column_tendon_SS_TBSeqKList.RemoveAll();     
		Column_shear_TBSeqKList.RemoveAll();         
		Column_tendon_CS_TBSeqKList.RemoveAll();     
		Column_con_CS_TBSeqKList.RemoveAll(); 

		DgnLcomKMap.RemoveAll();
		ComfDMap.RemoveAll();
	}

};


struct LOAD_RATE_DATA
{
	ArrElemPairKey arBlrdElemKList;
	CArray<UINT,UINT> arBlrdMembKList;
	CArray<UINT,UINT> arTndnElemKList;
	CArray<UINT,UINT> arStreRatingCaseKList;
	CArray<UINT,UINT> arServRatingCaseKList;

	void Initialize()
	{
		arBlrdElemKList.RemoveAll();
		arBlrdMembKList.RemoveAll();
		arTndnElemKList.RemoveAll();
		arStreRatingCaseKList.RemoveAll();
		arServRatingCaseKList.RemoveAll();
	}
};

struct PLATE_GIRDER_DATA
{
	CArray<UINT,UINT> arPgdElemKList;  
	CArray<UINT,UINT> arStrePgdCaseKList;
	CArray<UINT,UINT> arServPgdCaseKList;

	void Initialize()
	{
		arPgdElemKList.RemoveAll();    
		arStrePgdCaseKList.RemoveAll();
		arServPgdCaseKList.RemoveAll();
	}
};

struct STL_ORTHDECK_DATA
{
	CArray<UINT,UINT> aSodElemKList;  
	CArray<UINT,UINT> aStrnSodCaseKList;
	CArray<UINT,UINT> aFatiSodCaseKList;
	
	void Initialize()
	{
		aSodElemKList.RemoveAll();    
		aStrnSodCaseKList.RemoveAll();
		aFatiSodCaseKList.RemoveAll();
	}
	STL_ORTHDECK_DATA() {Initialize();}
};

struct STL_COMP_GIRDER_DATA
{
	CArray<UINT,UINT> aStlCompElemKList;  
	CArray<UINT,UINT> aStrnStlCompCaseKList;
	CArray<UINT,UINT> aFatiStlCompCaseKList;
	CArray<UINT,UINT> aServStlCompCaseKList;

	void Initialize()
	{
		aStlCompElemKList.RemoveAll();    
		aStrnStlCompCaseKList.RemoveAll();
		aFatiStlCompCaseKList.RemoveAll();
		aServStlCompCaseKList.RemoveAll();
	}
	STL_COMP_GIRDER_DATA() {Initialize();}
};


// [31/10/2006 : maxiao] For CIVIL(CH) 2006                                        
struct _CHRC_JTG_DATA
{
	int iTotLineNum;
	CArray<UINT,UINT> CbcsBeamKList;//弯
	CArray<UINT,UINT> CsisBeamKList;//剪
	CArray<UINT,UINT> CtssBeamKList;//扭
	CArray<UINT,UINT> CcwsBeamKList;//裂缝宽度
	CArray<UINT,UINT> CsccBeamKList;//正截面法向应力
	CArray<UINT,UINT> CtstBeamKList;//钢筋
	CArray<UINT,UINT> CsncBeamKList;//中性轴主拉应力
	CArray<UINT,UINT> BrlrKList;//梁纵筋估算
	CArray<UINT,UINT> BrcrKList;//梁的普通箍筋

	CArray<UINT,UINT> CcwsColumnKList;//柱的裂缝宽度
	CArray<UINT,UINT> CtcsColumnKList;//抗拉
	CArray<UINT,UINT> CscsColumnKList;//抗压
	CArray<UINT,UINT> CrlrKList;//柱纵筋估算

	CMap<UINT, UINT, UINT, UINT> DgnLcomKMap;
	CMap<_CRC_COMF_K,_CRC_COMF_K,_CRC_COMF_D,_CRC_COMF_D&> ComfDMap;

	void Initialize()
	{
		iTotLineNum=0;
		CbcsBeamKList.RemoveAll();
		CsisBeamKList.RemoveAll();
		CtssBeamKList.RemoveAll();    
		CcwsBeamKList.RemoveAll();
		CsccBeamKList.RemoveAll();
		CtstBeamKList.RemoveAll();
		CsncBeamKList.RemoveAll();
		CcwsColumnKList.RemoveAll();
		CtcsColumnKList.RemoveAll();
		CscsColumnKList.RemoveAll();
		BrlrKList.RemoveAll();
		CrlrKList.RemoveAll();
		BrcrKList.RemoveAll();
		DgnLcomKMap.RemoveAll();
		ComfDMap.RemoveAll();
	}
};

// [02/02/2007 : maxiao] For CIVIL(CH Railway Design) 2007
struct _CHRC_RAIL_DATA
{
	int iTotLineNum;
	CArray<UINT,UINT> BeamConKList   ;//铁路梁混凝土压应力
	CArray<UINT,UINT> BeamShearKList ;//铁路梁混凝土剪应力
	CArray<UINT,UINT> BeamRebarKList ;//铁路梁钢筋拉应力
	CArray<UINT,UINT> BeamSheartKList;//铁路梁上翼缘剪应力
	CArray<UINT,UINT> BeamShearbKList;//铁路梁下翼缘剪应力
	CArray<UINT,UINT> BeamCrackKList ;//铁路梁裂缝宽度
	
	CArray<UINT,UINT> ColumnConKList   ;//铁路柱混凝土压应力
	CArray<UINT,UINT> ColumnShearKList ;//铁路柱混凝土剪应力
	CArray<UINT,UINT> ColumnRebarcKList;//铁路柱钢筋压应力
	CArray<UINT,UINT> ColumnRebarsKList;//铁路柱钢筋拉应力
	CArray<UINT,UINT> ColumnStabKList  ;//铁路柱稳定性
	CArray<UINT,UINT> ColumnCrackKList ;//铁路柱裂缝宽度
	//----Seismic
	CArray<UINT,UINT> SeismicConKList   ;//铁路柱混凝土压应力抗震
	CArray<UINT,UINT> SeismicRebarcKList;//铁路柱钢筋压应力抗震
	CArray<UINT,UINT> SeismicRebarsKList;//铁路柱钢筋拉应力抗震
	CArray<UINT,UINT> SeismicEKList     ;//铁路柱偏心距抗震
	CArray<UINT,UINT> SeismicStabKList  ;//铁路柱稳定性抗震
	CArray<UINT,UINT> SeismicDuctKList  ;//铁路柱延性比抗震Ductility
 
	CMap<UINT, UINT, UINT, UINT> DgnLcomKMap;
	CMap<_CRC_COMF_K,_CRC_COMF_K,_CRC_COMF_D,_CRC_COMF_D&> ComfDMap;

	void Initialize()
	{
		iTotLineNum=0;
		BeamConKList.RemoveAll();
		BeamShearKList.RemoveAll();
		BeamRebarKList.RemoveAll();    
		BeamSheartKList.RemoveAll();
		BeamShearbKList.RemoveAll();
		BeamCrackKList.RemoveAll();

		ColumnConKList.RemoveAll();
		ColumnShearKList.RemoveAll();
		ColumnRebarcKList.RemoveAll();
		ColumnRebarsKList.RemoveAll();
		ColumnStabKList.RemoveAll();
		ColumnCrackKList.RemoveAll(); 
		//----seismic
		SeismicConKList.RemoveAll()   ;//铁路柱混凝土压应力抗震
		SeismicRebarcKList.RemoveAll();//铁路柱钢筋压应力抗震
		SeismicRebarsKList.RemoveAll();//铁路柱钢筋拉应力抗震
		SeismicEKList.RemoveAll()     ;//铁路柱偏心距抗震
		SeismicStabKList.RemoveAll()  ;//铁路柱稳定性抗震
		SeismicDuctKList.RemoveAll()  ;//铁路柱延性比抗震Ductility

	}
};

struct _PSC_AASHTO_DATA
{
	int iTotLineNum;
	/*
	CArray<UINT,UINT> FysgKList;
	CArray<UINT,UINT> FycmKList;
	CArray<UINT,UINT> BdcwKList;
	CArray<UINT,UINT> TbarKList;
	CArray<UINT,UINT> FssgKList;
	CArray<UINT,UINT> FsmsKList;
	CArray<UINT,UINT> FsmtKList;
	CArray<UINT,UINT> FpmsJtgKList;
	CArray<UINT,UINT> FpmtJtgKList;
	
	CArray<UINT,UINT> FypcKsceKList;
	CArray<UINT,UINT> BdcyKsceKList;
	CArray<UINT,UINT> BdczKsceKList;
	CArray<UINT,UINT> CumsKsceKList;
	CArray<UINT,UINT> CrmtKsceKList;
	CArray<UINT,UINT> TndnKsceKList;
	*/
	
	CMap<UINT, UINT, UINT, UINT> DgnLcomKMap;
	CMap<_CRC_COMF_K,_CRC_COMF_K,_CRC_COMF_D,_CRC_COMF_D&> ComfDMap;
	CMapEx<int, int, _CRC_COMF_K, _CRC_COMF_K&> mComfKey;

	void Initialize()
	{   
		iTotLineNum=0;
		/*
		FysgKList.RemoveAll();
		FycmKList.RemoveAll();
		BdcwKList.RemoveAll();
		TbarKList.RemoveAll();
		FssgKList.RemoveAll();
		FsmsKList.RemoveAll();
		FsmtKList.RemoveAll();
		FpmsJtgKList.RemoveAll();
		FpmtJtgKList.RemoveAll();

		FypcKsceKList.RemoveAll();
		BdcyKsceKList.RemoveAll();
		BdczKsceKList.RemoveAll();
		CumsKsceKList.RemoveAll();
		CrmtKsceKList.RemoveAll();
		TndnKsceKList.RemoveAll();
		*/

		DgnLcomKMap.RemoveAll();
		ComfDMap.RemoveAll();
		mComfKey.RemoveAll();
	}

};

struct T_FYPC_STRESS
{
	double dFDL1;	  //啊汲吝 览仿档			[Unit=Stress]
									//器胶飘刨记(iLoadType)牢 版快=沥馒备俊辑狼 览仿
	double dFDL2;	  //啊汲吝 览仿档			[Unit=Stress]
						//器胶飘刨记(iLoadType)牢 版快=没馒厘摹狼 劝悼俊 狼茄 颊角狼 场 何盒俊辑狼 览仿
						//橇府刨记牢(iLoadType) 版快=荤侩救窃.
	double dFLL;	  //荤侩窍吝矫 览仿档		[Unit=Stress]

	void Initialize()
	{
		dFDL1=dFDL2=dFLL=0.0;
	}

};
struct _TNDN_STRESS
{
	CMap<UINT,UINT,T_FYPC_STRESS,T_FYPC_STRESS> mapTndnStress;
	
	void Initialize()
	{
		mapTndnStress.RemoveAll();
	}  
};


struct _MATD_RPT_D
{
	UINT MatlK;
	T_MATD_RPT MatdRptD;  

	void Initialize()
	{
		MatlK = 0;
		MatdRptD.Initialize();
	}

	void SetMatd(T_MATD_RPT& rData)
	{
		MatdRptD = rData;
	}
	
};

//seismic design
struct _CHRC_JTGSEIS_DATA
{
	int iTotLineNum;
	//seismic design
	CArray<UINT,UINT> E1Pier_Comp_KList;                 //E1桥墩、桥台单元强度
	CArray<UINT,UINT> E1Arch_Cross_KList;                //E1主拱圈强度-抗弯 
	CArray<UINT,UINT> E1Arch_Comp_KList;                 //E1主拱圈强度-偏压
	CArray<UINT,UINT> E1BentCap_Bend_KList;              //E1盖梁强度-抗弯
	CArray<UINT,UINT> E1BentCap_Shear_KList;             //E1盖梁强度-抗剪
	CArray<UINT,UINT> E1Foundation_KList;                //E1基础强度验算
	CArray<UINT,UINT> E1RubberThick_KList;      //E1支座厚度（板式橡胶）
	CArray<UINT,UINT> E1RubberSlip_KList;       //E1支座抗滑稳定性（板式橡胶）
	CArray<UINT,UINT> E1ActiveHoriDisp_KList;   //E1支座水平位移（盆式活动支座）
	CArray<UINT,UINT> E1StatHoriForce_KList;    //E1支座水平承载力（盆式固定支座）
	//E2
	CArray<UINT,UINT> E2Pier_Comp_KList;                 //E2桥墩、桥台单元强度
	CArray<UINT,UINT> E2Arch_Cross_KList;                //E2主拱圈强度-抗弯 
	CArray<UINT,UINT> E2Arch_Comp_KList;                 //E2主拱圈强度-偏压
	CArray<UINT,UINT> E2BentCap_Bend_KList;              //E2盖梁强度-抗弯
	CArray<UINT,UINT> E2BentCap_Shear_KList;             //E2盖梁强度-抗剪
	CArray<UINT,UINT> E2Foundation_KList;                //E2基础强度验算
	CArray<UINT,UINT> E2Pier_Shear_KList;                //E2桥墩塑性铰区抗剪强度
	CArray<UINT,UINT> E2RubberThick_KList;          //E2支座厚度（板式橡胶）
	CArray<UINT,UINT> E2RubberSlip_KList;           //E2支座抗滑稳定性（板式橡胶）
	CArray<UINT,UINT> E2ActiveHoriDisp_KList;       //E2支座水平位移（盆式活动支座）
	CArray<UINT,UINT> E2StatHoriForce_KList;        //E2支座水平承载力（盆式固定支座）
	CArray<UINT,UINT> E2FrustaTopDisp_KList;        //E2墩顶位移（规则桥梁）
	CArray<UINT,UINT> E2CornerDisp_KList;           //E2塑形转角位移（非规则桥梁）
	
	CMap<UINT, UINT, UINT, UINT> DgnLcomKMap;
	CMap<_CRC_COMF_K,_CRC_COMF_K,_CRC_COMF_D,_CRC_COMF_D&> ComfDMap;
	
	void Initialize()
	{
		iTotLineNum=0;
		
		E1Pier_Comp_KList.RemoveAll();
		E1Arch_Cross_KList.RemoveAll();    
		E1Arch_Comp_KList.RemoveAll();     
		E1BentCap_Bend_KList.RemoveAll();  
		E1BentCap_Shear_KList.RemoveAll(); 
		E1Foundation_KList.RemoveAll();    
		E1RubberThick_KList.RemoveAll();   
		E1RubberSlip_KList.RemoveAll();    
		E1ActiveHoriDisp_KList.RemoveAll();
		E1StatHoriForce_KList.RemoveAll(); 
		//E2
		E2Pier_Comp_KList.RemoveAll();     
		E2Arch_Cross_KList.RemoveAll();    
		E2Arch_Comp_KList.RemoveAll();     
		E2BentCap_Bend_KList.RemoveAll();  
		E2BentCap_Shear_KList.RemoveAll(); 
		E2Foundation_KList.RemoveAll();    
		E2Pier_Shear_KList.RemoveAll();      		
		E2RubberThick_KList.RemoveAll();   
		E2RubberSlip_KList.RemoveAll();    
		E2ActiveHoriDisp_KList.RemoveAll();
		E2StatHoriForce_KList.RemoveAll();
		E2FrustaTopDisp_KList.RemoveAll(); 
		E2CornerDisp_KList.RemoveAll();    
		
		DgnLcomKMap.RemoveAll();
		ComfDMap.RemoveAll();
	}
};

struct _SE_REPORT_DATA
{
	T_ELEM_K_LIST aElemK;
	T_LCOM_K LcomK;  /// 0 = Max.

	void Initialize()
	{
		aElemK.RemoveAll();
		LcomK = 0;
	}

};

struct _DGN_GEN_SEIS_FACTOR_DATA
{
	BOOL bHideSpecialGroup;
	CString strConCode; // Concrete Design Code Name

	void Initialize()
	{
		bHideSpecialGroup=FALSE;
		strConCode.Empty();
	}

};

// ServiceDef.h
// SERVICE NUMBER DEFINE
enum  EN_DGNSA_DEF
{
	EN_DGNSA_GEN_KFAC = 1,
	EN_DGNSA_GEN_LENG,
	EN_DGNSA_CRC_EXPORT,
	EN_DGNSA_CRC_IMPORT,
	EN_DGNSA_CRC_XPRINT,
	EN_DGNSA_PSC_DESIGN_KSCE,
	EN_DGNSA_PSC_DESIGN_JTG,
	EN_DGNSA_GEN_SEIS_FACTOR,
	EN_DGNSA_BRIDGE_LOAD_RATING,
	EN_DGNSA_PLATE_GIRDER,
	EN_DGNSA_CHRC_BEAM_DESIGN_JTG,
	EN_DGNSA_CHRC_COLUMN_DESIGN_JTG,
	EN_DGNSA_CHRAILRC_BEAM_DESIGN_JTG,
	EN_DGNSA_CHRAILRC_COLUMN_DESIGN_JTG,
	EN_DGNSA_PSC_BEAM_DESIGN_TB,
	EN_DGNSA_PSC_COLUMN_DESIGN_TB,
	EN_DGNSA_PLATE_GIRDER_PRINT,
	EN_DGNSA_PSC_TENDON_STRESS,
	EN_DGNSA_MATD_DATA,
	EN_DGNSA_PSC_DESIGN_AASHTO,
	EN_DGNSA_PSC_CHECK_ELEMENT_LIST,
	EN_DGNSA_CH_SEISMIC_DESIGN,
	EN_DGNSA_PLATE_GIRDER_PRINT_XL,
	EN_DGNSA_PSC_RT_PRINT,
	EN_DGNSA_PLATE_GIRDER_RT_PRINT_XL,
	EN_DGNSA_RATING_PRINT_XL,
	EN_DGNSA_GET_DSTL,
	EN_DGNSA_GET_DCON,
	EN_DGNSA_GET_PSCD,
	EN_DGNSA_GET_DSRC,
	EN_DGNSA_OHTOTROPIC_DECK_DESIGN,
	EN_DGNSA_OHTOTROPIC_DECK_PRINT,
	EN_DGNSA_STL_COMP_GIRDER_DESIGN,
	EN_DGNSA_STL_COMP_GIRDER_PRINT,
	EN_DGNSA_BRDG_SEIS_EVAL,
	EN_DGNSA_BRDG_SEIS_EVAL_PRINT,
	EN_DGNSA_BRDG_SEIS_EVAL_BEARING,
	EN_DGNSA_SEIS_EVAL_REPORT_1STCON,
	EN_DGNSA_SEIS_EVAL_REPORT_1STSTL,
	EN_DGNSA_SEIS_EVAL_REPORT_1STMAS,
	EN_DGNSA_SEIS_EVAL_REPORT_UPHGCON,
	EN_DGNSA_SEIS_EVAL_REPORT_UPHGSTL,
	EN_DGNSA_SEIS_EVAL_REPORT_UPHGMAS,
	EN_DGNSA_CLEAR_MPHI_RESULT_ALL,
	EN_DGNSA_CLEAR_MPHI_RESULT_FRAM,
	EN_DGNSA_CLEAR_MPHI_RESULT_WALL,
	EN_DGNSA_DELETE_SEISMIC_MANAGER,
	EN_DGNSA_DELETE_MEMOIZATION_TOWER,
	EN_DGNSA_BRDG_SEIS_EVAL_JP,
	EN_DGNSA_BRDG_SEIS_EVAL_PRINT_JP,
};



#endif

