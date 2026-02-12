#ifndef __DB_ST_DT_SECT_H__
#define __DB_ST_DT_SECT_H__

#define MIDAS_SDK

#pragma pack(push, 1)

#define T_SECT_K unsigned int
#define T_SECT_K_LIST CArray<UINT, UINT>

typedef std::pair<unsigned int, UINT> SectPairK;

// 파일에 저장되므로 변경하지 말것
#define D_SECT_TYPE_REGULAR         1
#define D_SECT_TYPE_SRC             2
#define D_SECT_TYPE_USER            3
#define D_SECT_TYPE_COMBINED        4
#define D_SECT_TYPE_TAPERED         5
#define D_SECT_TYPE_CONSTRUCTION    6
#define D_SECT_TYPE_HIBEAM          7
#define D_SECT_TYPE_COLDFORMED      8
#define D_SECT_TYPE_COMPO_B         9       // Composite Box
#define D_SECT_TYPE_COMPO_I         10      // Composite I
#define D_SECT_TYPE_PSC             11
#define D_SECT_TYPE_COMPO_CI        12      // Composite Concrete-I
#define D_SECT_TYPE_COMPO_CT        13      // Composite Concrete-T
#define D_SECT_TYPE_COMPO_PC        14      // Composite Section (거더가 PSC단면을 사용하는 것들)
#define D_SECT_TYPE_COMPO_G         15      // Composite General
#define D_SECT_TYPE_COMPO_TUB       16      // Composite TUB
#define D_SECT_TYPE_STLG_B          17      // Steel Girder Box
#define D_SECT_TYPE_STLG_I          18      // Steel Girder I
#define D_SECT_TYPE_COMPO_STLG_B    19      // Composite Steel Girder Box
#define D_SECT_TYPE_COMPO_STLG_I    20      // Composite Steel Girder I
#define D_SECT_TYPE_COMPO_STLG_TUB  21      // Composite Steel Girder Tub
#define D_SECT_TYPE_STLG_MCELL      22      // Steel Girder Multi Cell

#define D_SECT_TYPE_TAP_REG       101     // for construction section
#define D_SECT_TYPE_TAP_USER      102     // for construction section


//Regular
#define D_SECT_SHAPE_REG_L          _T("L")     // Angle
#define D_SECT_SHAPE_REG_C          _T("C")     // Channel
#define D_SECT_SHAPE_REG_H          _T("H")     // H-Section
#define D_SECT_SHAPE_REG_T          _T("T")     // T-Section
#define D_SECT_SHAPE_REG_B          _T("B")     // Box
#define D_SECT_SHAPE_REG_P          _T("P")     // Pipe
#define D_SECT_SHAPE_REG_SB         _T("SB")    // Solid Rectangle
#define D_SECT_SHAPE_REG_SR         _T("SR")    // Solide Round
#define D_SECT_SHAPE_REG_2L         _T("2L")    // Double Angle
#define D_SECT_SHAPE_REG_2C         _T("2C")    // Double Channel
#define D_SECT_SHAPE_REG_2CB        _T("2CB")   // Double Channel (box type)
#define D_SECT_SHAPE_REG_CC         _T("CC")    // Cold Formed Channel
#define D_SECT_SHAPE_REG_URIB       _T("URIB")  // U-RIB
#define D_SECT_SHAPE_REG_OCT        _T("OCT")   // Octagon
#define D_SECT_SHAPE_REG_SOCT       _T("SOCT")  // Solide Octagon
#define D_SECT_SHAPE_REG_TRK        _T("TRK")   // Trac
#define D_SECT_SHAPE_REG_STRK       _T("STRK")  // Solid Trac
#define D_SECT_SHAPE_REG_HTRK       _T("HTRK")  // Half Trac
#define D_SECT_SHAPE_REG_CL         _T("CL")    // Cross Angle
#define D_SECT_SHAPE_REG_ROCT       _T("ROCT")  // R-Octagon
#define D_SECT_SHAPE_REG_BSTF       _T("BSTF")  // Box with stiffener
#define D_SECT_SHAPE_REG_PSTF       _T("PSTF")  // Pipe with stiffener
#define D_SECT_SHAPE_REG_GEN        _T("GEN")   // General Section (Value에서만 사용)
#define D_SECT_SHAPE_REG_UDT        _T("UDT")   // Inverted T-Section
#define D_SECT_SHAPE_REG_CB         _T("CB")    
#define D_SECT_SHAPE_REG_UP         _T("UP")    // Upright
#define D_SECT_SHAPE_REG_Z          _T("Z")     // Z
#define D_SECT_SHAPE_REG_CCH        _T("CCH")   // Cold Formed Channel (Hat Type)


//src
#define D_SECT_SHAPE_SRC_RBO  _T("RBO")
#define D_SECT_SHAPE_SRC_RBC  _T("RBC")
#define D_SECT_SHAPE_SRC_RPO  _T("RPO")
#define D_SECT_SHAPE_SRC_RPC  _T("RPC")
#define D_SECT_SHAPE_SRC_CBO  _T("CBO")
#define D_SECT_SHAPE_SRC_CBC  _T("CBC")
#define D_SECT_SHAPE_SRC_CPO  _T("CPO")
#define D_SECT_SHAPE_SRC_CPC  _T("CPC")
#define D_SECT_SHAPE_SRC_EBC  _T("EBC")   // Empty Box Closed : Box filled with concrete
#define D_SECT_SHAPE_SRC_EPC  _T("EPC")   // Empty Pipe Closed
#define D_SECT_SHAPE_SRC_EPO  _T("EPO")   // Concrete Pipe in Empty Pipe
#define D_SECT_SHAPE_SRC_RIB  _T("RHB")   // Rectangle i-beam
#define D_SECT_SHAPE_SRC_CIB  _T("CHB")   // circle i-beam
#define D_SECT_SHAPE_SRC_RH2T _T("RH2T")  // Rectangle Combined H2T(Cross H)
#define D_SECT_SHAPE_SRC_RHT  _T("RHT")   // Rectangle Combined HT (Combined T)
#define D_SECT_SHAPE_SRC_BSTF _T("SBSF")  // SRC Box with Stiffener
#define D_SECT_SHAPE_SRC_PSTF _T("SPSF")  // SRC Pipe with Stiffener
#define D_SECT_SHAPE_SRC_STUB _T("STUB")  // SRC Sing Tube - v960
#define D_SECT_SHAPE_SRC_DUMB _T("DUMB")  // SRC Dumbbell  - v960


//combined
#define D_SECT_SHAPE_COMA_2H  _T("2H")
#define D_SECT_SHAPE_COMA_HC1 _T("HC1")
#define D_SECT_SHAPE_COMA_HC2 _T("HC2")
#define D_SECT_SHAPE_COMA_HT  _T("HT")
#define D_SECT_SHAPE_COMA_2T1 _T("2T1")
#define D_SECT_SHAPE_COMA_2T2 _T("2T2")
#define D_SECT_SHAPE_COMA_H2T _T("H2T")
#define D_SECT_SHAPE_COMA_WOH _T("WOH")
#define D_SECT_SHAPE_COMA_FOH _T("FOH")		// Web Opened H with Stiffener

#define D_SECT_SHAPE_COMB_2CW1  _T("2CW1")
#define D_SECT_SHAPE_COMB_2CW2  _T("2CW2")
#define D_SECT_SHAPE_COMB_1CW1  _T("1CW1")
#define D_SECT_SHAPE_COMB_1CW2  _T("1CW2")
#define D_SECT_SHAPE_COMB_4L    _T("4L")
#define D_SECT_SHAPE_COMB_HP    _T("HP")
//cold formed
#define D_SECT_SHAPE_COLF_CC         _T("CC")
#define D_SECT_SHAPE_COLF_2CCI       _T("2CCI")
#define D_SECT_SHAPE_COLF_2CCB       _T("2CCB")
#define D_SECT_SHAPE_COLF_3CC        _T("3CC")
#define D_SECT_SHAPE_COLF_4CC        _T("4CC")

// Composite Section B, I
#define D_SECT_SHAPE_COMPO_B        _T("CP_B")
#define D_SECT_SHAPE_COMPO_I        _T("CP_I")
#define D_SECT_SHAPE_COMPO_CI       _T("CPCI")
#define D_SECT_SHAPE_COMPO_CT       _T("CPCT")
#define D_SECT_SHAPE_COMPO_PC       _T("CPPC")
#define D_SECT_SHAPE_COMPO_G        _T("CP_G")
#define D_SECT_SHAPE_COMPO_TUB      _T("CP_T")
#define D_SECT_SHAPE_COMPO_STLG_B   _T("CSGB")
#define D_SECT_SHAPE_COMPO_STLG_I   _T("CSGI")
#define D_SECT_SHAPE_COMPO_STLG_TUB _T("CSGT")

// PSC
#define D_SECT_SHAPE_PSC_1CELL      _T("1CEL")
#define D_SECT_SHAPE_PSC_2CELL      _T("2CEL")
#define D_SECT_SHAPE_PSC_3CELL      _T("3CEL")
#define D_SECT_SHAPE_PSC_4CELL      _T("NCEL")
#define D_SECT_SHAPE_PSC_MID        _T("PSCM")
#define D_SECT_SHAPE_PSC_HALF       _T("PSCH")
#define D_SECT_SHAPE_PSC_T          _T("PSCT")
#define D_SECT_SHAPE_PSC_BOX        _T("PSCB")
#define D_SECT_SHAPE_PSC_I          _T("PSCI")
#define D_SECT_SHAPE_PSC_NCEL2      _T("NCE2")
#define D_SECT_SHAPE_PSC_VALUE      _T("VALU")
#define D_SECT_SHAPE_PSC_CMPWEB     _T("CMPW")  // 파형강판

// Steel Girder
#define D_SECT_SHAPE_STLG_B         _T("STLB")
#define D_SECT_SHAPE_STLG_I         _T("STLI")
#define D_SECT_SHAPE_STLG_MCELL     _T("SGMC")

#define D_SECT_SRC_STEEL        1
#define D_SECT_SRC_CONCRETE     2

#define D_SECT_BUILT_BUILTUP    1
#define D_SECT_BUILT_ROLLED     2

struct T_SECT_STIFFNESS_600
{
	double Area,Asy,Asz,Rxx,Ryy,Rzz,Cyp,Cym,Czp,Czm,Qyb,Qzb,WArea;  //L^2,L^2,L^2,L^4,L^4,L^4,L,L,L,L,L^2,L^2,L^2
	void Initialize(double dInitVal=0.0)
	{
		Area=dInitVal;
		Asy=dInitVal;
		Asz=dInitVal;
		Rxx=dInitVal;
		Ryy=dInitVal;
		Rzz=dInitVal;
		Cyp=dInitVal;
		Cym=dInitVal;
		Czp=dInitVal;
		Czm=dInitVal;
		Qyb=dInitVal;
		Qzb=dInitVal;
		WArea=dInitVal;
	}
};
struct T_SECT_STIFFNESS_815
{
	double Area,Asy,Asz,Rxx,Ryy,Rzz,Cyp,Cym,Czp,Czm,Qyb,Qzb,WArea;  //L^2,L^2,L^2,L^4,L^4,L^4,L,L,L,L,L^2,L^2,L^2
	double dx1, dy1, dx2, dy2, dx3, dy3, dx4, dy4;
	void Initialize(double dInitVal=0.0)
	{
		Area=dInitVal;
		Asy=dInitVal;
		Asz=dInitVal;
		Rxx=dInitVal;
		Ryy=dInitVal;
		Rzz=dInitVal;
		Cyp=dInitVal;
		Cym=dInitVal;
		Czp=dInitVal;
		Czm=dInitVal;
		Qyb=dInitVal;
		Qzb=dInitVal;
		WArea=dInitVal;
		dx1 = dInitVal;
		dx2 = dInitVal;
		dx3 = dInitVal;
		dx4 = dInitVal;
		dy1 = dInitVal;
		dy2 = dInitVal;
		dy3 = dInitVal;
		dy4 = dInitVal;
	}
};
/**
@brief 단면의 성능 정보
*/
struct T_SECT_STIFFNESS
{
	double Area;/**< 단면적(L^2) */
	double Asy; /**< 강축 방향의 전단면적(L^2) */
	double Asz; /**< 약축 방향의 전단면적(L^2) */
	double Rxx; /**< 비틀림 저항 계수(L^4) */ 
	double Ryy; /**< 강축에 대한 단면 2차 모멘트(L^4) */
	double Rzz; /**< 약축에 대한 단면 2차 모멘트(L^4) */
	double Cyp; /**< 도심에서 +x 방향 최외단 거리(L) */
	double Cym; /**< 도심에서 -x 방향 최외단 거리(L) */
	double Czp; /**< 도심에서 +y 방향 최외단 거리(L) */ 
	double Czm; /**< 도심에서 -y 방향 최외단 거리(L) */
	double Qyb; /**< 강축 전단력에 대한 전단계수(L^2) */
	double Qzb; /**< 약축 전단력에 대한 전단계수(L^2) */
	double WArea;/**< 단면적(L^2) */
	double dx1; /**< Z-Zs of point C */
	double dy1;	/**< Y-Ys of point C */
	double dx2;	/**< 단면적(L^2) */
	double dy2;	/**< 단면적(L^2) */
	double dx3;	/**< 단면적(L^2) */
	double dy3;	/**< 단면적(L^2) */
	double dx4;	/**< 단면적(L^2) */
	double dy4;	/**< 단면적(L^2) */
	double dIw, dw1, dw2, dw3, dw4, dxy1, dxy2, dxy3, dxy4, dxz1, dxz2, dxz3, dxz4; // 8.2.0 추가 (w:Warping Function) (dxy, dxz:Torsional Shear Stress Coff.)
	double dGeoParam1, dGeoParam2, dGeoParam3, dGeoParam4; // 8.2.0 추가
	double dYcy, dYcz, dYsy, dYsz, dIp; // 8.2.0 추가

	template <class T>
	void SetData(T& rData)
	{
		WArea = Area = rData["Area"];
		Asy  = rData["Asy"];
		Asz  = rData["Asz"];
		Rxx  = rData["Rxx"];
		Ryy	 = rData["Ryy"];
		Rzz	 = rData["Rzz"];
		Cyp  = rData["Cyp"];
		Cym	 = rData["Cym"];
		Czp	 = rData["Czp"];
		Czm  = rData["Czm"];
		Qyb	 = rData["Qyb"];
		Qzb	 = rData["Qzb"];
	}

	template <class T>
	void SetCsvHead(T& rData)
	{
		rData["Area"] = "Arrange:-10.00E+00~1.00E+25 UNIT:m^2";
		rData["Asy"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m^2";
		rData["Asz"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m^2";
		rData["Rxx"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m^4";
		rData["Ryy"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m^4";
		rData["Rzz"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m^4";
		rData["Cyp"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m";
		rData["Cym"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m";
		rData["Czp"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m";
		rData["Czm"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m";
		rData["Qyb"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m^2";
		rData["Qzb"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m^2";
	}

	template <class T>
	void SetCsvData(T& rData)
	{
		rData["Area"] = Area;
		rData["Asy"]  = Asy;
		rData["Asz"]  = Asz;
		rData["Rxx"]  = Rxx;
		rData["Ryy"]  = Ryy;
		rData["Rzz"]  = Rzz;
		rData["Cyp"]  = Cyp;
		rData["Cym"]  = Cym;
		rData["Czp"]  = Czp;
		rData["Czm"]  = Czm;
		rData["Qyb"]  = Qyb;
		rData["Qzb"]  = Qzb;
	}

	void Initialize(double dInitVal=0.0)
	{
		if (dInitVal == 0.0)
		{
			//cklee@midasit.com double 값의 bit pattern은 0x0000000000000000임 따라서 0.0일 경우 
			//memset으로 처리 가능함. 변수의 갯수가 많아서 아래 개별 값 Assign 보다 얘가 훨씬 빠르다. 
			memset(this, 0, sizeof(T_SECT_STIFFNESS));
		}
		else
		{
		Area=dInitVal;
		Asy=dInitVal;
		Asz=dInitVal;
		Rxx=dInitVal;
		Ryy=dInitVal;
		Rzz=dInitVal;
		Cyp=dInitVal;
		Cym=dInitVal;
		Czp=dInitVal;
		Czm=dInitVal;
		Qyb=dInitVal;
		Qzb=dInitVal;
		WArea=dInitVal;
		dx1 = dInitVal;
		dx2 = dInitVal;
		dx3 = dInitVal;
		dx4 = dInitVal;
		dy1 = dInitVal;
		dy2 = dInitVal;
		dy3 = dInitVal;
		dy4 = dInitVal;
		dIw = dInitVal;
		dw1 = dInitVal;
		dw2 = dInitVal;
		dw3 = dInitVal;
		dw4 = dInitVal;
		dxy1 = dInitVal;
		dxy2 = dInitVal;
		dxy3 = dInitVal;
		dxy4 = dInitVal;
		dxz1 = dInitVal;
		dxz2 = dInitVal;
		dxz3 = dInitVal;
		dxz4 = dInitVal;
		dGeoParam1 = dInitVal;
		dGeoParam2 = dInitVal;
		dGeoParam3 = dInitVal;
		dGeoParam4 = dInitVal;
		dYcy = dInitVal;
		dYcz = dInitVal;
		dYsy = dInitVal;
		dYsz = dInitVal;
		dIp = dInitVal;
	}
	}
	T_SECT_STIFFNESS() {}
	T_SECT_STIFFNESS(const T_SECT_STIFFNESS& src) {*this = src;}
	T_SECT_STIFFNESS& operator=(const T_SECT_STIFFNESS& src)
	{
		Area =src.Area;  
		Asy  =src.Asy;
		Asz  =src.Asz; 
		Rxx  =src.Rxx;
		Ryy  =src.Ryy; 
		Rzz  =src.Rzz; 
		Cyp  =src.Cyp; 
		Cym  =src.Cym; 
		Czp  =src.Czp; 
		Czm  =src.Czm; 
		Qyb  =src.Qyb; 
		Qzb  =src.Qzb; 
		WArea=src.WArea;
		dx1  =src.dx1;  
		dx2  =src.dx2;  
		dx3  =src.dx3;  
		dx4  =src.dx4;  
		dy1  =src.dy1;  
		dy2  =src.dy2;  
		dy3  =src.dy3;  
		dy4  =src.dy4;  
		dIw  =src.dIw;
		dw1  =src.dw1;
		dw2  =src.dw2;
		dw3  =src.dw3;
		dw4  =src.dw4;
		dxy1 =src.dxy1;
		dxy2 =src.dxy2;
		dxy3 =src.dxy3;
		dxy4 =src.dxy4;
		dxz1 =src.dxz1;
		dxz2 =src.dxz2;
		dxz3 =src.dxz3;
		dxz4 =src.dxz4;
		dGeoParam1 = src.dGeoParam1;
		dGeoParam2 = src.dGeoParam2;
		dGeoParam3 = src.dGeoParam3;
		dGeoParam4 = src.dGeoParam4;
		dYcy = src.dYcy;
		dYcz = src.dYcz;
		dYsy = src.dYsy;
		dYsz = src.dYsz;
		dIp = src.dIp;
		return *this;
	}
	void Convert600(T_SECT_STIFFNESS_600& rData)
	{
		Initialize(0.0);
		Area = rData.Area;    
		Asy=rData.Asy;
		Asz=rData.Asz;
		Rxx=rData.Rxx;
		Ryy=rData.Ryy;
		Rzz=rData.Rzz;
		Cyp=rData.Cyp;
		Cym=rData.Cym;
		Czp=rData.Czp;
		Czm=rData.Czm;
		Qyb=rData.Qyb;
		Qzb=rData.Qzb;
		WArea=rData.WArea;
	}
	void Convert815(T_SECT_STIFFNESS_815& rData)
	{
		Initialize(0.0);
		Area = rData.Area;    
		Asy=rData.Asy;
		Asz=rData.Asz;
		Rxx=rData.Rxx;
		Ryy=rData.Ryy;
		Rzz=rData.Rzz;
		Cyp=rData.Cyp;
		Cym=rData.Cym;
		Czp=rData.Czp;
		Czm=rData.Czm;
		Qyb=rData.Qyb;
		Qzb=rData.Qzb;
		WArea=rData.WArea;
		dx1=rData.dx1;
		dx2=rData.dx2;
		dx3=rData.dx3;
		dx4=rData.dx4;
		dy1=rData.dy1;
		dy2=rData.dy2;
		dy3=rData.dy3;
		dy4=rData.dy4;
	}

	void GetStiff(double& dArea, double& dAsy, double& dAsz, double& dRxx, double& dRyy, double& dRzz, 
								double& dCyp, double& dCym, double& dCzp, double& dCzm, double& dQyb, double& dQzb, double& dWArea)
	{
		dArea = Area;
		dAsy  = Asy  ;
		dAsz  = Asz  ;
		dRxx  = Rxx  ;
		dRyy  = Ryy  ;
		dRzz  = Rzz  ;
		dCyp  = Cyp  ;
		dCym  = Cym  ;
		dCzp  = Czp  ;
		dCzm  = Czm  ;
		dQyb  = Qyb  ;
		dQzb  = Qzb  ;
		dWArea= WArea;
	}
	void GetPosition(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3, double& x4, double& y4)
	{
		x1 = dx1;
		x2 = dx2;
		x3 = dx3;
		x4 = dx4;
		y1 = dy1;
		y2 = dy2;
		y3 = dy3;
		y4 = dy4;    
	}

	void SetStiff(double dArea, double dAsy, double dAsz, double dRxx, double dRyy, double dRzz, 
								double dCyp, double dCym, double dCzp, double dCzm, double dQyb, double dQzb, double dWArea)
	{
		Area = dArea ;
		Asy  = dAsy  ;
		Asz  = dAsz  ;
		Rxx  = dRxx  ;
		Ryy  = dRyy  ;
		Rzz  = dRzz  ;
		Cyp  = dCyp  ;
		Cym  = dCym  ;
		Czp  = dCzp  ;
		Czm  = dCzm  ;
		Qyb  = dQyb  ;
		Qzb  = dQzb  ;
		WArea= dWArea;
	}
	void SetPosition(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
	{
		dx1 = x1;
		dx2 = x2;
		dx3 = x3;
		dx4 = x4;
		dy1 = y1;
		dy2 = y2;
		dy3 = y3;
		dy4 = y4;    
	}
	void SetStiffWarping(T_SECT_STIFFNESS Stiff)
	{
		dIw        =Stiff.dIw;
		dw1        =Stiff.dw1;
		dw2        =Stiff.dw2;
		dw3        =Stiff.dw3;
		dw4        =Stiff.dw4;
		dxy1       =Stiff.dxy1;
		dxy2       =Stiff.dxy2;
		dxy3       =Stiff.dxy3;
		dxy4       =Stiff.dxy4;
		dxz1       =Stiff.dxz1;
		dxz2       =Stiff.dxz2;
		dxz3       =Stiff.dxz3;
		dxz4       =Stiff.dxz4;
		dGeoParam1 = Stiff.dGeoParam1;
		dGeoParam2 = Stiff.dGeoParam2;
		dGeoParam3 = Stiff.dGeoParam3;
		dGeoParam4 = Stiff.dGeoParam4;
		dYcy       = Stiff.dYcy;
		dYcz       = Stiff.dYcz;
		dYsy       = Stiff.dYsy;
		dYsz       = Stiff.dYsz;
		dIp        = Stiff.dIp;
	}
};
struct T_SECT_STIFFNESS_UNIT
{
	T_SECT_STIFFNESS_UNIT()
	{
		Area =D_UNITSYS_BASE_AREA;
		Asy  =D_UNITSYS_BASE_AREA;
		Asz  =D_UNITSYS_BASE_AREA;
		Rxx  =D_UNITSYS_BASE_STIF;
		Ryy  =D_UNITSYS_BASE_STIF;
		Rzz  =D_UNITSYS_BASE_STIF;
		Cyp  =D_UNITSYS_BASE_LENGTH;
		Cym  =D_UNITSYS_BASE_LENGTH;
		Czp  =D_UNITSYS_BASE_LENGTH;
		Czm  =D_UNITSYS_BASE_LENGTH;
		Qyb  =D_UNITSYS_BASE_AREA;
		Qzb  =D_UNITSYS_BASE_AREA;
		WArea=D_UNITSYS_BASE_AREA;
		dx1 = D_UNITSYS_BASE_LENGTH;
		dx2 = D_UNITSYS_BASE_LENGTH;
		dx3 = D_UNITSYS_BASE_LENGTH;
		dx4 = D_UNITSYS_BASE_LENGTH;
		dy1 = D_UNITSYS_BASE_LENGTH;
		dy2 = D_UNITSYS_BASE_LENGTH;
		dy3 = D_UNITSYS_BASE_LENGTH;
		dy4 = D_UNITSYS_BASE_LENGTH;
		dIw = D_UNITSYS_BASE_L6;
		dw1 = D_UNITSYS_BASE_AREA;
		dw2 = D_UNITSYS_BASE_AREA;
		dw3 = D_UNITSYS_BASE_AREA;
		dw4 = D_UNITSYS_BASE_AREA;
		dxy1 = D_UNITSYS_BASE_LENGTH;
		dxy2 = D_UNITSYS_BASE_LENGTH;
		dxy3 = D_UNITSYS_BASE_LENGTH;
		dxy4 = D_UNITSYS_BASE_LENGTH;
		dxz1 = D_UNITSYS_BASE_LENGTH;
		dxz2 = D_UNITSYS_BASE_LENGTH;
		dxz3 = D_UNITSYS_BASE_LENGTH;
		dxz4 = D_UNITSYS_BASE_LENGTH;
		dGeoParam1 = D_UNITSYS_BASE_AREA;
		dGeoParam2 = D_UNITSYS_BASE_LENGTH;
		dGeoParam3 = D_UNITSYS_BASE_LENGTH;
		dGeoParam4 = D_UNITSYS_NONE;
		dYcy = D_UNITSYS_BASE_LENGTH;
		dYcz = D_UNITSYS_BASE_LENGTH;
		dYsy = D_UNITSYS_BASE_LENGTH;
		dYsz = D_UNITSYS_BASE_LENGTH;
		dIp = D_UNITSYS_BASE_STIF;
	}
	int Area,Asy,Asz,Rxx,Ryy,Rzz,Cyp,Cym,Czp,Czm,Qyb,Qzb,WArea;
	int dx1, dy1, dx2, dy2, dx3, dy3, dx4, dy4;
	int dIw, dw1, dw2, dw3, dw4, dxy1, dxy2, dxy3, dxy4, dxz1, dxz2, dxz3, dxz4;
	int dGeoParam1, dGeoParam2, dGeoParam3, dGeoParam4;
	int dYcy, dYcz, dYsy, dYsz, dIp;
};
/**
@brief 단면의 성능 정보
*/
struct T_SECT_DESIGN
{
	double YBar; /**< 강축에 대한 도심까지의 거리(L) */
	double ZBar; /**< 약축에 대한 도심까지의 거리(L) */
	double Syy,Szz;//L^3,L^3
	double Zyy;  /**< 강축에 대한 소성 단면계수(L^3) */
	double Zzz;  /**< 약축에 대한 소성 단면계수(L^3) */
	double Roy,Roz,Cwp,TanA,Rop,XO,RO,Beta;   //L,L,L^4,rad,L,L,L,NONE

	template <class T>
	void SetData(T& rData)
	{
		YBar  = rData["Ybar"];
		ZBar  = rData["Zbar"];
		Syy  = rData["Syy"];
		Szz	 = rData["Szz"];
		Zyy	 = rData["Zyy"];
		Zzz  = rData["Zzz"];
		Roy	 = rData["Roy"];
		Roz	 = rData["Roz"];
		Cwp  = rData["Cwp"];
		TanA	 = rData["TanA"];
		Rop	 = rData["Rop"];
		XO	 = rData["XO"];
		RO	 = rData["RO"];
		Beta	 = rData["Beta"];
	}

	template <class T>
	void SetCsvHead(T& rData)
	{
		rData["Ybar"] = "Arrange:-10.00E+00~1.00E+25 UNIT:m";
		rData["Zbar"] = "Arrange:-10.00E+00~1.00E+25 UNIT:m";
		rData["Syy"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m^3";
		rData["Szz"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m^3";
		rData["Zyy"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m^3";
		rData["Zzz"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m^3";
		rData["Roy"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m";
		rData["Roz"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m";
		rData["Cwp"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m^6";
		rData["TanA"] = "Arrange:-10.00E+00~1.00E+25";
		rData["Rop"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m";
		rData["XO"]	  = "Arrange:-10.00E+00~1.00E+25 UNIT:m";
		rData["RO"]   = "Arrange:-10.00E+00~1.00E+25 UNIT:m";
		rData["Beta"] = "Arrange:-10.00E+00~1.00E+25";
	}

	template <class T>
	void SetCsvData(T& rData)
	{
		rData["Ybar"] = YBar;
		rData["Zbar"] = ZBar;
		rData["Syy"]  = Syy;
		rData["Szz"]  = Szz;
		rData["Zyy"]  = Zyy;
		rData["Zzz"]  = Zzz;
		rData["Roy"]  = Roy;
		rData["Roz"]  = Roz;
		rData["Cwp"]  = Cwp;
		rData["TanA"] = TanA;
		rData["Rop"]  = Rop;
		rData["XO"]   = XO;
		rData["RO"]   = RO;
		rData["Beta"] = Beta;
	}

	void Initialize()
	{
		YBar=0.;
		ZBar=0.;
		Syy=0.;
		Szz=0.;
		Zyy=0.;
		Zzz=0.;
		Roy=0.;
		Roz=0.;
		Cwp=0.;
		TanA=0.;
		Rop=0.;
		XO=0.;
		RO=0.;
		Beta=0.;
	}
	T_SECT_DESIGN() {}
	T_SECT_DESIGN(const T_SECT_DESIGN& src) {*this = src;}
	T_SECT_DESIGN& operator=(const T_SECT_DESIGN& src)
	{
		YBar =src.YBar;  
		ZBar =src.ZBar;  
		Syy =src.Syy;  
		Szz =src.Szz;  
		Zyy =src.Zyy;  
		Zzz =src.Zzz;  
		Roy =src.Roy;  
		Roz =src.Roz;  
		Cwp =src.Cwp;  
		TanA =src.TanA;  
		Rop =src.Rop;  
		XO =src.XO;  
		RO =src.RO;  
		Beta =src.Beta;  
		return *this;
	}
};
struct T_SECT_DESIGN_UNIT
{
	T_SECT_DESIGN_UNIT()
	{
		YBar=D_UNITSYS_BASE_LENGTH;
		ZBar=D_UNITSYS_BASE_LENGTH;
		Syy =D_UNITSYS_BASE_VOLUME;
		Szz =D_UNITSYS_BASE_VOLUME;
		Zyy =D_UNITSYS_BASE_VOLUME;
		Zzz =D_UNITSYS_BASE_VOLUME;
		Roy =D_UNITSYS_BASE_LENGTH;
		Roz =D_UNITSYS_BASE_LENGTH;
		Cwp =D_UNITSYS_BASE_L6;
		TanA=D_UNITSYS_BASE_RADIAN;
		Rop =D_UNITSYS_BASE_LENGTH;
		XO  =D_UNITSYS_BASE_LENGTH;
		RO  =D_UNITSYS_BASE_LENGTH;
		Beta=D_UNITSYS_NONE;
	}
	int YBar,ZBar,Syy,Szz,Zyy,Zzz,Roy,Roz,Cwp,TanA,Rop,XO,RO,Beta;
};

struct T_GSEC_LINE_721  // Value-General Section에서 라인 정보로 사용
{
	int v1, v2;   // Vertex ID
	double dThik; // 라인 두께
	int nAlign;   // 선의 Align 정보 (0:위쪽,1:가운데,2:아래쪽)
};
struct T_GSEC_LINE  // Value-General Section에서 라인 정보로 사용
{
	int v1, v2;   // Vertex ID
	double dThik; // 라인 두께
	int nAlign;   // 선의 Align 정보 (0:위쪽,1:가운데,2:아래쪽)
	double dA;    // 회전거리
	double dVF;   // 전단응력
	void Initialize()
	{
		v1 = v2 = 0;
		dThik = 0.0;
		nAlign = 0;
		dA = 0.0;
		dVF = 0.0;
	}
	void Convert721(const T_GSEC_LINE_721 &line721)
	{
		v1     = line721.v1;
		v2     = line721.v2;
		dThik  = line721.dThik;
		nAlign = line721.nAlign;
		dA     = 0.0;
		dVF    = 0.0;
	}
};

struct T_GSEC_VERTEX  // Polygon을 형성하는 점의 좌표
{
	double dX, dY;
	T_GSEC_VERTEX() {}
	T_GSEC_VERTEX(double x, double y) { dX = x; dY = y; }
	T_GSEC_VERTEX& Set(double x, double y) { dX = x; dY = y; return *this; }
	void Initialize() { dX = dY = 0.0; }
};

struct T_GSEC_POLYGON  // 임의 형상 단면의 Polygon
{
	T_GSEC_POLYGON() {}
	T_GSEC_POLYGON(const T_GSEC_POLYGON& crData) { *this = crData; }
	T_GSEC_POLYGON& operator=(const T_GSEC_POLYGON& crData) 
	{
		aVertex.Copy(crData.aVertex);
		return (*this);
	}
	CArray<T_GSEC_VERTEX, T_GSEC_VERTEX&> aVertex;
	void Initialize()
	{
		aVertex.RemoveAll();
	}
	void Transfer(T_GSEC_VERTEX dOffset)
	{
		INT_PTR nSize = aVertex.GetSize();
		for (INT_PTR i = 0; i < nSize; i++)
		{
			aVertex[i].dX += dOffset.dX;
			aVertex[i].dY += dOffset.dY;
		}
	}
};

// Add, Jaeoh(07.05.29)
struct T_GSEC_LDGN
{  
	CString sName;
	int iBoundary;      // 0=자유돌출판, 1=양연지지판 
	double db;          // length,        Unit:Length
	double dt;          // thickness, Unit:Length
	int iPanNum;        // Panel number  
	double dy1,dz1;     // 양끝단의 도심에서 떨어진 거리.
	double dy2,dz2;
	CArray<T_GSEC_LINE, T_GSEC_LINE&> aLine;
	BOOL bExcept;       // 7.4.0 추가, 응력검토시 제외
	BOOL bStiffener;    // 7.9.5 추가, Stiffener 인지 판단
	
	void Initialize()
	{
		sName.Empty();
		iBoundary = 0;
		db = 0.0;
		dt = 0.0;
		iPanNum = 0;
		aLine.RemoveAll();
		bExcept=FALSE;
		bStiffener=FALSE;
	}

	T_GSEC_LDGN() {}
	T_GSEC_LDGN(const T_GSEC_LDGN& crData) { *this = crData; }
	T_GSEC_LDGN& operator=(const T_GSEC_LDGN& crData) 
	{
		sName = crData.sName;
		iBoundary = crData.iBoundary;
		db = crData.db;
		dt = crData.dt;
		iPanNum = crData.iPanNum;
		dy1 = crData.dy1;
		dz1 = crData.dz1;
		dy2 = crData.dy2;
		dz2 = crData.dz2;
		aLine.Copy(crData.aLine);
		bExcept = crData.bExcept;
		bStiffener = crData.bStiffener;
		return (*this);
	}
};

struct T_GSEC_LSHAP
{
	CString sName;
	int iShape;   // 0=Top flange, 1=Bottom flange, 2=Left web, 3=Right web, 4=Center web
	CArray<T_GSEC_LDGN,T_GSEC_LDGN&> aLdgn;

	void Initialize()
	{
		sName.Empty();
		iShape = 0;
		aLdgn.RemoveAll();
	}
	T_GSEC_LSHAP() {}
	T_GSEC_LSHAP(const T_GSEC_LSHAP& crData) { *this = crData; }
	T_GSEC_LSHAP& operator=(const T_GSEC_LSHAP& crData) 
	{
		sName = crData.sName;
		iShape = crData.iShape;
		aLdgn.Copy(crData.aLdgn);
		return (*this);
	}
};
struct T_GSEC_PART_761 // 740 추가 , General Composite Section 의 각 파트별 데이터
{
	T_SECT_STIFFNESS_815 Stiffness;
	T_SECT_STIFFNESS_815 StiffnessSum;
	double PeriIn;
	double PeriOut;
	double PeriInSum;
	double PeriOutSum;
	double YBar;
	double ZBar;
	double YBarSum;
	double ZBarSum;
	int IdxStart[5]; // T_SECT_SECTBASE_D 구조체의 OutPolygon, InPolygon, Line, Shape, aLineLoop 의 해당 파트 시작인덱스
	int IdxEnd[5]; 
};
struct T_GSEC_PART_815
{
	T_SECT_STIFFNESS_815 Stiffness;
	T_SECT_STIFFNESS_815 StiffnessSum;
	double PeriIn;
	double PeriOut;
	double PeriInSum;
	double PeriOutSum;
	double YBar;
	double ZBar;
	double YBarSum;
	double ZBarSum;
	int IdxStart[5]; // T_SECT_SECTBASE_D 구조체의 OutPolygon, InPolygon, Line, Shape, aLineLoop 의 해당 파트 시작인덱스
	int IdxEnd[5];   // 이 파트의 마지막 인덱스 (해당 인덱스 이전 번호 까지이므로 해당 인덱스는 포함되지 않음)
	  
	double dDencityRatio;  
	BOOL   bBaseMaterial; 
	BOOL   bPlane;
	double dElastic;  
	double dPoisson; 
};
struct T_GSEC_PART // 740 추가 , General Composite Section 의 각 파트별 데이터
{
	T_SECT_STIFFNESS Stiffness;
	T_SECT_STIFFNESS StiffnessSum;
	double PeriIn;
	double PeriOut;
	double PeriInSum;
	double PeriOutSum;
	double YBar;
	double ZBar;
	double YBarSum;
	double ZBarSum;
	int IdxStart[5]; // T_SECT_SECTBASE_D 구조체의 OutPolygon, InPolygon, Line, Shape, aLineLoop 의 해당 파트 시작인덱스
	int IdxEnd[5];   // 이 파트의 마지막 인덱스 (해당 인덱스 이전 번호 까지이므로 해당 인덱스는 포함되지 않음)

	// MNET:XXXX-CJJEONG-20090401 (Tel : 2124) 
	double dDencityRatio;  
	BOOL   bBaseMaterial; 
	BOOL   bPlane;
	double dElastic;  
	double dPoisson; 

	void Initialize()
	{
		Stiffness.Initialize();
		StiffnessSum.Initialize();
		PeriIn=0.;
		PeriOut=0.;
		PeriInSum=0.;
		PeriOutSum=0.;
		YBar=0.;
		ZBar=0.;
		YBarSum=0.;
		ZBarSum=0.;
		memset(IdxStart, 0, sizeof(IdxStart));
		memset(IdxEnd, 0, sizeof(IdxEnd));
		
		dDencityRatio = 1.0;
		bBaseMaterial = FALSE;
		bPlane        = TRUE;
		dElastic      = 1.0;
		dPoisson      = 0.3;
	}
	T_GSEC_PART() {}
	T_GSEC_PART(const T_GSEC_PART& crData) { *this = crData; }
	T_GSEC_PART& operator=(const T_GSEC_PART& crData) 
	{
		Stiffness = crData.Stiffness;
		StiffnessSum = crData.StiffnessSum;
		PeriIn = crData.PeriIn;
		PeriOut = crData.PeriOut;
		PeriInSum = crData.PeriInSum;
		PeriOutSum = crData.PeriOutSum;
		YBar = crData.YBar;
		ZBar = crData.ZBar;
		YBarSum = crData.YBarSum;
		ZBarSum = crData.ZBarSum;
		memcpy(IdxStart, crData.IdxStart, sizeof(IdxStart));
		memcpy(IdxEnd, crData.IdxEnd, sizeof(IdxEnd));

		dDencityRatio = crData.dDencityRatio;
		bBaseMaterial = crData.bBaseMaterial;
		bPlane        = crData.bPlane;
		dElastic      = crData.dElastic;
		dPoisson      = crData.dPoisson;

		return (*this);
	}
	void Convert761(T_GSEC_PART_761& rData)
	{
		Initialize();

		Stiffness.Convert815(rData.Stiffness);
		StiffnessSum.Convert815(rData.StiffnessSum);
		PeriIn        = rData.PeriIn;
		PeriOut       = rData.PeriOut;
		PeriInSum     = rData.PeriInSum;
		PeriOutSum    = rData.PeriOutSum;
		YBar          = rData.YBar;
		ZBar          = rData.ZBar;
		YBarSum       = rData.YBarSum;
		ZBarSum       = rData.ZBarSum;
		memcpy(IdxStart, rData.IdxStart, sizeof(IdxStart));
		memcpy(IdxEnd, rData.IdxEnd, sizeof(IdxEnd));    
	}
	void Convert815(T_GSEC_PART_815& rData)
	{
		Initialize();
		
		Stiffness.Convert815(rData.Stiffness);
		StiffnessSum.Convert815(rData.StiffnessSum);
		PeriIn        = rData.PeriIn;
		PeriOut       = rData.PeriOut;
		PeriInSum     = rData.PeriInSum;
		PeriOutSum    = rData.PeriOutSum;
		YBar          = rData.YBar;
		ZBar          = rData.ZBar;
		YBarSum       = rData.YBarSum;
		ZBarSum       = rData.ZBarSum;
		memcpy(IdxStart, rData.IdxStart, sizeof(IdxStart));
		memcpy(IdxEnd, rData.IdxEnd, sizeof(IdxEnd));
		dDencityRatio = rData.dDencityRatio;
		bBaseMaterial = rData.bBaseMaterial;
		bPlane        = rData.bPlane;
		dElastic      = rData.dElastic;
		dPoisson      = rData.dPoisson;		
	}
};

struct T_STIFF_SHAPE_D
{
	CString strName;
	int     nType;    // 0:I, 1:T, 2:U-Rib, 3:L
	double  dSize[8];

	T_STIFF_SHAPE_D() {}
	T_STIFF_SHAPE_D(const T_STIFF_SHAPE_D& rData) { *this = rData; }
	T_STIFF_SHAPE_D& operator=(const T_STIFF_SHAPE_D& rData) 
	{
		strName = rData.strName;
		nType = rData.nType;
		for(int i=0; i<8; ++i)
		{
			dSize[i] = rData.dSize[i];
		}
		return (*this);
	}

	void Initialize()
	{
		strName.Empty();
		nType = 0;
		for(int i=0; i<8; ++i) dSize[i] = 0.0;    
	}
};

struct T_STIFF_PROFILE_D
{
	double dSpacing; 
	int    nShape;   // T_STIFF_SHAPE_D 배열의 Index
	BOOL   bCalc;    // 단면 강성 계산시 포함 여부 결정

	T_STIFF_PROFILE_D() {}
	T_STIFF_PROFILE_D(const T_STIFF_PROFILE_D& rData) { *this = rData; }
	T_STIFF_PROFILE_D& operator=(const T_STIFF_PROFILE_D& rData) 
	{
		dSpacing = rData.dSpacing;
		nShape = rData.nShape;
		bCalc = rData.bCalc;
		return (*this);
	}
	
	void Initialize()
	{
		dSpacing = 0.0;
		nShape = -1;
		bCalc = TRUE;
	}
};

struct T_SECT_STIFF_D
{
	int nPosition;    // 0:Both 1:Left 2:Right
	int nStiffNum[4]; // T_STIFF_PROFILE_D 배열 크기 - 0:Left, 1:Right, 2:Bottom, 3:Top 
	int nRefPos2;      // 0: Top  1: Bottom

	CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> aStiffShape;
	CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffLeft;
	CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffRight;
	CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffBottom;
	CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> aStiffTop;

	T_SECT_STIFF_D() {}
	T_SECT_STIFF_D(const T_SECT_STIFF_D& rData) { *this = rData; }
	T_SECT_STIFF_D& operator=(const T_SECT_STIFF_D& rData) 
	{
		nPosition = rData.nPosition;
		for(int i=0; i<4; ++i) nStiffNum[i] = rData.nStiffNum[i];
		nRefPos2 = rData.nRefPos2;

		aStiffShape.Copy(rData.aStiffShape);
		aStiffLeft.Copy(rData.aStiffLeft);
		aStiffRight.Copy(rData.aStiffRight);
		aStiffBottom.Copy(rData.aStiffBottom);
		aStiffTop.Copy(rData.aStiffTop);

		return (*this);
	}

	void Initialize()
	{
		nPosition = 0;
		for(int i=0; i<4; ++i) nStiffNum[i] = 0;    
		nRefPos2 = 0;

		aStiffShape.RemoveAll();
		aStiffLeft.RemoveAll();
		aStiffRight.RemoveAll();
		aStiffBottom.RemoveAll();
		aStiffTop.RemoveAll();
	}
};

struct T_SECT_SOD_STIFF_DATA_D
{
	BOOL    bCalc;
	double  dSpacing;
	CString strShapeName; // T_SECT_SOD_STIFF_D 구조체 aStiffShape 의 Name
	int     nStiffPos;    // 0:Left/Top 1:Right/Bottom 2:Both
	CString strStiffName;
	int     nStiffDir;
	
	T_SECT_SOD_STIFF_DATA_D() {}
	T_SECT_SOD_STIFF_DATA_D(const T_SECT_SOD_STIFF_DATA_D& rData) { *this = rData; }
	T_SECT_SOD_STIFF_DATA_D& operator=(const T_SECT_SOD_STIFF_DATA_D& rData) 
	{
		bCalc = rData.bCalc;
		dSpacing = rData.dSpacing;
		strShapeName = rData.strShapeName;
		nStiffPos = rData.nStiffPos;
		strStiffName = rData.strStiffName;
		nStiffDir = rData.nStiffDir;
	
		return (*this);
	}
	
	void Initialize()
	{
		bCalc = TRUE;
		dSpacing = 0.0;
		strShapeName.Empty();
		nStiffPos = 0;
		strStiffName.Empty();
		nStiffDir = 0;
	}
};

struct T_SECT_SOD_STIFF_SUB_D
{
	int     nDeckPos;
	int     nDeckPart;
	CString strDeckName;
	int     nRefPos;	
	int     nNum;
	CArray<T_SECT_SOD_STIFF_DATA_D, T_SECT_SOD_STIFF_DATA_D&> aStiffData;

	T_SECT_SOD_STIFF_SUB_D() {}
	T_SECT_SOD_STIFF_SUB_D(const T_SECT_SOD_STIFF_SUB_D& rData) { *this = rData; }
	T_SECT_SOD_STIFF_SUB_D& operator=(const T_SECT_SOD_STIFF_SUB_D& rData) 
	{
		nDeckPos = rData.nDeckPos;
		nDeckPart = rData.nDeckPart;
		strDeckName = rData.strDeckName;
		nRefPos = rData.nRefPos;
		nNum = rData.nNum;
		aStiffData.Copy(rData.aStiffData);
		
		return (*this);
	}

	void Initialize()
	{
		nDeckPos = 0;
		nDeckPart = 0;
		strDeckName.Empty();
		nRefPos = 0;
		nNum = 0;
		aStiffData.RemoveAll();
	}
};

struct T_SECT_SOD_STIFF_D
{
	CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> aStiffShape;
	CArray<T_SECT_SOD_STIFF_SUB_D, T_SECT_SOD_STIFF_SUB_D&> aSODStiffSub;

	T_SECT_SOD_STIFF_D() {}
	T_SECT_SOD_STIFF_D(const T_SECT_SOD_STIFF_D& rData) { *this = rData; }
	T_SECT_SOD_STIFF_D& operator=(const T_SECT_SOD_STIFF_D& rData) 
	{
		aStiffShape.Copy(rData.aStiffShape);
		aSODStiffSub.Copy(rData.aSODStiffSub);
		
		return (*this);
	}
	
	void Initialize()
	{
		aStiffShape.RemoveAll();
		aSODStiffSub.RemoveAll();
	}
};

struct T_SECT_STIFF_POS_D
{
	BOOL bUseStiff;
	CString strShapeName; // T_SECT_RIVET_STIFF_D 구조체 aStiffShape 의 Name

	T_SECT_STIFF_POS_D() {}
	T_SECT_STIFF_POS_D(const T_SECT_STIFF_POS_D& rData) { *this = rData; }
	T_SECT_STIFF_POS_D& operator=(const T_SECT_STIFF_POS_D& rData)
	{
		bUseStiff = rData.bUseStiff;
		strShapeName = rData.strShapeName;

		return (*this);
	}

	void Initialize()
	{
		bUseStiff = FALSE;
		strShapeName.Empty();
	}
};


#define RIVET_TL   0
#define RIVET_TR   1
#define RIVET_WEB  2
#define RIVET_BL   3
#define RIVET_BR   4

struct T_SECT_RIVET_POS_D_960_Temp
{
	int    nComponent; // 0:Top Left, 1:Top Right, 2:Web, 3:Bot Left, 4:Bot Right
	int    nAngle;     // T_SECT_STIFF_POS_D 배열 ID (Steel Girder : 0~7)
	int    nRefPos;    // 0:Left/Top  1: Right/Bottom
	double dDist;
	double dDia;
};
struct T_SECT_RIVET_POS_D
{
	BOOL   bUser;
	int    nComponent; // 0:Top Left, 1:Top Right, 2:Web, 3:Bot Left, 4:Bot Right
	int    nAngle;     // T_SECT_STIFF_POS_D 배열 ID (Steel Girder : 0~7),  100 : None
	int    nRefPos;    // 0:Left/Top  1: Right/Bottom
	double dDist;
	double dDia;

	T_SECT_RIVET_POS_D() { Initialize(); }
	T_SECT_RIVET_POS_D(const T_SECT_RIVET_POS_D& rData) { *this = rData; }
	T_SECT_RIVET_POS_D& operator=(const T_SECT_RIVET_POS_D& rData)
	{
		bUser      = rData.bUser;
		nComponent = rData.nComponent;
		nAngle     = rData.nAngle;
		nRefPos    = rData.nRefPos;
		dDist      = rData.dDist;
		dDia       = rData.dDia;

		return (*this);
	}

	void Initialize()
	{
		bUser = FALSE;
		nComponent = RIVET_TL;
		nAngle = 0;
		nRefPos = 0;
		dDist = 0.0;
		dDia = 0.0;
	}
	void Convert960Temp(T_SECT_RIVET_POS_D_960_Temp& rData)
	{
		bUser      = FALSE;
		nComponent = rData.nComponent;
		nAngle     = rData.nAngle;
		nRefPos    = rData.nRefPos;
		dDist      = rData.dDist;
		dDia       = rData.dDia;
	}
};

struct T_SECT_RIVET_STIFF_D
{
	CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> aStiffShape;

	BOOL bSymmetric;
	CArray<T_SECT_STIFF_POS_D, T_SECT_STIFF_POS_D&> aStiffPosD;

	BOOL bRivet;
	CArray<T_SECT_RIVET_POS_D, T_SECT_RIVET_POS_D&> aRivetPosD;

	T_SECT_RIVET_STIFF_D() {}
	T_SECT_RIVET_STIFF_D(const T_SECT_RIVET_STIFF_D& rData) { *this = rData; }
	T_SECT_RIVET_STIFF_D& operator=(const T_SECT_RIVET_STIFF_D& rData)
	{
		aStiffShape.Copy(rData.aStiffShape);

		bSymmetric = rData.bSymmetric;
		aStiffPosD.Copy(rData.aStiffPosD);

		bRivet = rData.bRivet;
		aRivetPosD.Copy(rData.aRivetPosD);

		return (*this);
	}

	void Initialize()
	{
		aStiffShape.RemoveAll();

		bSymmetric = TRUE;
		aStiffPosD.RemoveAll();

		bRivet = TRUE;
		aRivetPosD.RemoveAll();
	}
};

struct T_SECT_STIFFNESS_CF_BASE_851
{
	double dA_eff;
	double dIy_eff;
	double dIz_eff;
	double dWy_eff;
	double dWz_eff;
	double dIt_eff;
	double dIw_eff;
	double dRy_eff;
	double dRz_eff;
};

struct T_SECT_STIFFNESS_CF_BASE
{
	double dA_eff;
	double dA_net;
	double dIy_eff;
	double dIz_eff;
	double dWy_eff;
	double dWz_eff;
	double dIt_eff;
	double dIw_eff;
	double dRy_eff;
	double dRz_eff;

	T_SECT_STIFFNESS_CF_BASE() {}
	T_SECT_STIFFNESS_CF_BASE(const T_SECT_STIFFNESS_CF_BASE& rData) { *this = rData; }
	T_SECT_STIFFNESS_CF_BASE& operator=(const T_SECT_STIFFNESS_CF_BASE& rData) 
	{
		dA_eff  = rData.dA_eff;
		dA_net = rData.dA_net;
		dIy_eff = rData.dIy_eff;
		dIz_eff = rData.dIz_eff;
		dWy_eff = rData.dWy_eff;
		dWz_eff = rData.dWz_eff;
		dIt_eff = rData.dIt_eff;
		dIw_eff = rData.dIw_eff;
		dRy_eff = rData.dRy_eff;
		dRz_eff = rData.dRz_eff;

		return (*this);
	}

	void Initialize()
	{
		dA_eff   = 0.0;
		dA_net  = 0.0;
		dIy_eff  = 0.0;
		dIz_eff  = 0.0;
		dWy_eff  = 0.0;
		dWz_eff  = 0.0;
		dIt_eff  = 0.0;
		dIw_eff  = 0.0;
		dRy_eff  = 0.0;
		dRz_eff  = 0.0;
	}

	void Convert851(T_SECT_STIFFNESS_CF_BASE_851& rData)
	{
		Initialize();
		dA_eff  = rData.dA_eff;
		/// 기존 모델에 대해서 는 eff 사용
		dA_net  = rData.dA_eff;
		dIy_eff = rData.dIy_eff;
		dIz_eff = rData.dIz_eff;
		dWy_eff = rData.dWy_eff;
		dWz_eff = rData.dWz_eff;
		dIt_eff = rData.dIt_eff;
		dIw_eff = rData.dIw_eff;
		dRy_eff = rData.dRy_eff;
		dRz_eff = rData.dRz_eff;
	}
};

struct T_SECT_STIFFNESS_CF_851
{
	T_SECT_STIFFNESS_CF_BASE_851 CFStiffULS;
	T_SECT_STIFFNESS_CF_BASE_851 CFStiffSLS;  // 사용안함.
};

struct T_SECT_STIFFNESS_CF
{
	T_SECT_STIFFNESS_CF_BASE CFStiffULS;
	T_SECT_STIFFNESS_CF_BASE CFStiffSLS; // 사용안함.

	T_SECT_STIFFNESS_CF() {Initialize();}
	T_SECT_STIFFNESS_CF(const T_SECT_STIFFNESS_CF& rData) { *this = rData; }
	T_SECT_STIFFNESS_CF& operator=(const T_SECT_STIFFNESS_CF& rData) 
	{
		CFStiffULS = rData.CFStiffULS;
		CFStiffSLS = rData.CFStiffSLS;

		return (*this);
	}

	void Initialize()
	{
		CFStiffULS.Initialize();
		CFStiffSLS.Initialize();
	}

	void Convert851(T_SECT_STIFFNESS_CF_851& rData)
	{
		CFStiffULS.Convert851(rData.CFStiffULS);
		CFStiffSLS.Convert851(rData.CFStiffSLS);
	}
};

#define D_SECT_SIZE_NUM_MAX       60
struct T_SECT_SECTBASE_D
{
	CString Shape;  // D_SECT_SHAPE_???
	CString DBName;
	CString SName;  // section name
	CString SName2;
	double Size[D_SECT_SIZE_NUM_MAX];/**< 형상의 수치 정보, 단면 타입 별로 인덱스의 수치 역할은 다르다. */ // 6.4.0부터 60으로 변경
	double PeriIn; /**< 내부 둘레(Inner Perimeter) */
	double PeriOut; /**< 외부 둘레(Outer Perimeter) */
	int BuiltUpFlag;  // 1=Builtup Section(D_SECT_BUILT_BUILTUP)  2=Rolled Section(D_SECT_BUILT_ROLLED)
	T_SECT_STIFFNESS Stiffness;
	T_SECT_DESIGN Design;

	CArray<T_GSEC_POLYGON, T_GSEC_POLYGON&> aOuterPolygon; // 6.7.0 추가 
	CArray<T_GSEC_POLYGON, T_GSEC_POLYGON&> aInnerPolygon; // 6.7.0 추가
	CArray<T_GSEC_LINE, T_GSEC_LINE&> aLine; // 7.0.0 추가 (Value-General Line Type);
	CArray<int, int> aLineLoop;  // 7.0.0 추가 (Line Type에서 Loop)
	// aLineLoop : 갯수(n1), LineIx1, LineIx2, LineIx(n1), 갯수(n2), LineIx1, LineIx2, ..., LineIx(n2), ...
	// LineIx가 -인 것은 방향이 반대라는 의미
	//(Value-General Line Type인 경우 aOuterPolygon의 첫번째 Polygon이 Vertex List 가짐)
	CArray<T_GSEC_LSHAP,T_GSEC_LSHAP&> aShape;
	int ShapeIndex[4];       // v740 추가 / 1, 2, 3, 4 / CArray<T_GSEC_LSHAP,T_GSEC_LSHAP&> aShape의 Index
	int LdgnIndex[4];        // v740 추가 / 1, 2, 3, 4 / CArray<T_GSEC_LDGN,T_GSEC_LDGN&> aLdgn의 Index
	int nBeforePart;         // v740 추가 (General Composite) Before 의 Part 수 ( ex> 1+2 -> 2 )
	CArray<T_GSEC_PART, T_GSEC_PART&> aGeneralPart; // v740 추가 (General Composite) 각 Part Data

	T_SECT_STIFF_D Stiffener; // 8.3.0 추가 
	BOOL bSymAutoCalc; // 8.4.0 추가
	T_SECT_SOD_STIFF_D SODStiffener; // 8.4.0 추가
	T_SECT_STIFFNESS_CF StiffnessCF; // 8.5.0 추가
	T_SECT_RIVET_STIFF_D RivetStiff; // 9.6.0 추가

	template <class T>
	void SetData(T& rData)
	{
		SName		= rData["Name"];
		Shape		= rData["Shape"];
		Size[0]     = rData["Size 1"];
		Size[1]     = rData["Size 2"];
		Size[2]     = rData["Size 3"];
		Size[3]     = rData["Size 4"];
		Size[4]     = rData["Size 5"];
		Size[5]     = rData["Size 6"];
		Size[6]     = rData["Size 7"];
		Size[7]     = rData["Size 8"];
		PeriIn		= rData["PeriIn"];
		PeriOut		= rData["PeriOut"];
		BuiltUpFlag = rData["nBuilt"];
		Stiffness.SetData(rData);
		Design.SetData(rData);
	}

	template <class T>
	void SetCsvHead(T& rData, int nCase)
	{
		rData["Name"]	 = "Length:30";
		rData["Shape"]   = "Length:30";
		rData["Size 1"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m";
		rData["Size 2"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m";
		rData["Size 3"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m";
		rData["Size 4"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m";
		rData["Size 5"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m";
		rData["Size 6"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m";
		rData["Size 7"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m";
		rData["Size 8"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m";
		rData["PeriIn"]	 = "Arrange:-10.00E+00~1.00E+25 UNIT:m";
		rData["PeriOut"] = "Arrange:-10.00E+00~1.00E+25 UNIT:m";
		rData["PeriIn"]  = "Arrange:-10.00E+00~1.00E+25 UNIT:m";
		rData["nBuilt"]  = "Arrange:0~2";
		Stiffness.SetCsvHead(rData);
		Design.SetCsvHead(rData);
	}

	template <class T>
	void SetCsvData(T& rData)
	{
		rData["Name"]	 = SName;
		rData["Shape"]   = Shape;
		rData["Size 1"]  = Size[0];
		rData["Size 2"]  = Size[1];
		rData["Size 3"]  = Size[2];
		rData["Size 4"]  = Size[3];
		rData["Size 5"]  = Size[4];
		rData["Size 6"]  = Size[5];
		rData["Size 7"]  = Size[6];
		rData["Size 8"]  = Size[7];
		rData["PeriIn"]  = PeriIn;
		rData["PeriOut"] = PeriOut;
		rData["nBuilt"]  = BuiltUpFlag;
		Stiffness.SetCsvData(rData);
		Design.SetCsvData(rData);
	}

	T_SECT_SECTBASE_D() {}
	T_SECT_SECTBASE_D(const T_SECT_SECTBASE_D& crData) { *this = crData; }
	T_SECT_SECTBASE_D& operator=(const T_SECT_SECTBASE_D& rData)
	{
		Shape = rData.Shape;
		DBName = rData.DBName;
		SName = rData.SName;
		SName2 = rData.SName2;
		memcpy(Size, rData.Size, sizeof(Size));
		PeriIn = rData.PeriIn;
		PeriOut = rData.PeriOut;
		BuiltUpFlag = rData.BuiltUpFlag;
		Stiffness = rData.Stiffness;
		Design = rData.Design;

		aOuterPolygon.Copy(rData.aOuterPolygon);
		aInnerPolygon.Copy(rData.aInnerPolygon);
		aLine.Copy(rData.aLine);
		aLineLoop.Copy(rData.aLineLoop);
		aShape.Copy(rData.aShape);
		memcpy(ShapeIndex, rData.ShapeIndex, sizeof(ShapeIndex));
		memcpy(LdgnIndex, rData.LdgnIndex, sizeof(LdgnIndex));
		nBeforePart = rData.nBeforePart;
		aGeneralPart.Copy(rData.aGeneralPart);

		Stiffener = rData.Stiffener;
		bSymAutoCalc = rData.bSymAutoCalc;
		SODStiffener = rData.SODStiffener;
		StiffnessCF  = rData.StiffnessCF;
		RivetStiff = rData.RivetStiff;

		return *this;
	}
	void Initialize()
	{
		Shape.Empty();
		DBName.Empty();
		SName = "";
		SName2 = "";
		for(int i=0; i<D_SECT_SIZE_NUM_MAX; i++)Size[i]=0.;
		PeriIn=0.0;
		PeriOut=0.0;
		BuiltUpFlag=0;
		Stiffness.Initialize();
		Design.Initialize();

		aOuterPolygon.RemoveAll();
		aInnerPolygon.RemoveAll();
		aLine.RemoveAll();
		aLineLoop.RemoveAll();
		aShape.RemoveAll();
		for(int i=0 ; i<4 ; ++i) ShapeIndex[i]=0;
		for(int i=0 ; i<4 ; ++i) LdgnIndex[i]=0;
		nBeforePart=0;
		aGeneralPart.RemoveAll();

		Stiffener.Initialize();
		bSymAutoCalc = FALSE;
		SODStiffener.Initialize();
		StiffnessCF.Initialize();
		RivetStiff.Initialize();
	}
};
struct T_SECT_SECTBASE_UNIT
{
	T_SECT_SECTBASE_UNIT()
	{
		Shape  =D_UNITSYS_NONE;
		DBName =D_UNITSYS_NONE;
		SName  =D_UNITSYS_NONE;
		Size   =D_UNITSYS_BASE_LENGTH;
		PeriIn =D_UNITSYS_BASE_LENGTH;
		PeriOut=D_UNITSYS_BASE_LENGTH;
		BuiltUpFlag=D_UNITSYS_NONE;
		dX     =D_UNITSYS_BASE_LENGTH;
		dY     =D_UNITSYS_BASE_LENGTH;
		dThik  =D_UNITSYS_BASE_LENGTH;
		dA     =D_UNITSYS_BASE_LENGTH;
		dVF    =D_UNITSYS_BASE_STRESS;
	}
	int Shape;
	int DBName;
	int SName;
	int Size;
	int PeriIn;
	int PeriOut;
	int BuiltUpFlag;
	T_SECT_STIFFNESS_UNIT Stiffness;
	T_SECT_DESIGN_UNIT Design;
	int dX;
	int dY;
	int dThik;
	int dA;
	int dVF;
};
/**
@brief 단면의 형상 정보
*/
struct T_SECT_SECTION_D
{
	int nStype;    // D_SECT_TYPE_??
	CString SName; // section name
	CString Shape; /**< 단면 모양 */ // D_SECT_SHAPE_??
	int nOffsetPoint; /**< 기점(Cardinal Point 1~9 left->right, top->bottom) */
	T_SECT_SECTBASE_D SectI;
	T_SECT_SECTBASE_D SectJ;
	int nLytype;    // 1-3 (Tapered)
	int nLztype;    // 1-3 (Tapered)
	int nSRCtype;       // D_SECT_SRC_STEEL = 1; D_SECT_SRC_CONCRETE=2 (SRC)
	double Matl_Elast;  // SRC
	double Matl_Density;  // SRC  
	double Matl_Poisson_S;  // SRC
	double Matl_Poisson_C;  // SRC
	double Matl_StiffFactor;  // SRC(600에서 추가)
	double Matl_Thermal;
	double PeriIn;
	double PeriOut;
	int BuiltUpFlag;  // 1=Builtup Section(D_SECT_BUILT_BUILTUP)  2=Rolled Section(D_SECT_BUILT_ROLLED)
	int nJoint;       // PSC(Bit encoding)
	int nCellType;    // 0:Left, 1:Right (PSC만 사용) 
										// 0:None, 1:Circle, 2:Polygon(Mid인 경우 Left-Shape)
										// 0:Half, 1:1Cell, 2:2Cell(Box인 경우)
										// 0:vertical, 1:slope, 2:chamfer(4Cell)
										// 0:Polygon, 1:Circle (nCel2) (6.4.0에서 추가)
										// N2(BSTF) (6.5.0에서 추가)
	int nCellShape;   // 0:None, 1:Circle, 2:Polygon (PSC만 사용)
										// 0:None, 1:Circel, 2:Polygon(Mid인 경우 Right-Shape)
										// 0:Left, 1:Right(Box Half)
										// 1:Circle, 2:Polygon(Box 1Cell)
										// Number of girders(4Cell)
										// Number of cells  (nCel2) or REG_ROCT
										// N1 or N (BSTF, PSTF) (6.5.0에서 추가)
	BOOL bSymmetric;  // I, J 공통으로 적용(6.4.0에서 추가) 
	int  nOffsetCenter;   // Offset Center의 의미 0:Centroid, 1:Size(6.4.0에서 추가)
	int  nHorzOffsetOpt;  // Offset Left, Right의 조건 : 0:Size, 1:User(6.4.0에서 추가)
	int  nVertOffsetOpt;  // Offset Top, Bottom의 조건 : 0:Size, 1:User(6.4.0에서 추가)
	double dUserDefOffsetY[2]; // I, J 각각(6.4.0에서 추가)
	double dUserDefOffsetZ[2]; // I, J 각각(6.4.0에서 추가)
	int nUserOffsetRef;      // 0:Centroid, 1:Size(6.7.0에서 추가)
	BOOL bSmallHole;  // PSC nCell2에서 작은 구멍의 존재 여부: I,J 공통적용 (6.4.0에서 추가)
										// PSC CMPWEB의 경우 bHunch로 사용(6.7.1)
	BOOL bShearCheck; // PSC인 경우만 사용(6.4.0에서 추가)
	double dShearCheckPos[2][3];  // I, J 각각 3개(상,중,하 순) (6.4.0에서 추가)
	BOOL   bAutoShearCheckPos[2][3]; // I, J 각각 3개 (6.4.1에서 추가)
	double dQy[2][3];             // I, J 각각 3개(상,중,하 순) (6.7.0에서 추가)
	BOOL   bAutoQy[2][3];         // I, J 각각 3개(상,중,하 순) (6.7.0에서 추가)
	double dWebThk[2];            // 복부 최소 두께(for Torsion) - I,J 각각 1개 (6.4.0에서 추가)
	BOOL   bAutoWebThk[2];        // 복부 최소 두께(for Torsion) 자동 계산 - I, J 각각 1개(6.5.0에서 추가)
	double dWebThkShear[2][3];    // 복부 최소 두께(for Shear) - I,J 각 3개(상,중,하 순) (6.4.1에서 추가, 6.5.0에서 변경)
	BOOL   bAutoWebThkShear[2][3];// 복부 최소 두께(for Shear) 자동 계산 - I,J 각 3개(상,중,하 순) (6.4.1에서 추가, 6.5.0에서 변경)
	double dKt[2];                // Kt값 - I, J 각각 1개(강성처럼 자동 계산된 값 저장) (6.4.0에서 추가)
	BOOL bConsiderShearDeform;    // 전단변형 고려여부(고려하지 않을 경우 Asy, Asz 는 0으로 설정)  (6.4.0에서 추가)
	BOOL   bUserDefMeshSize;      // PSC 단면 강성 계산용 Mesh Size 지정(6.4.0에서 추가)
	double dMeshSize;             // PSC 단면 강성 계산용 Mesh Size (6.4.0에서 추가)
	BOOL   bUserInputStiffness;   // PSC 단면의 경우 사용자가 직접 강성 입력한 여부(6.4.0에서 추가)
	T_SECT_STIFFNESS Stiffness;
	T_SECT_DESIGN Design;
	int    nCalcStiffOpt; // 0:FEM, 1:Equation  // 7.0.0에서 추가 (value type의 line type import시만 사용)
	BOOL   bMultipleElastic;      // v720추가, Composite단면의 경우 Long Term및 Shrinkage에 대한 탄성계수비
	double dLongTermESEC;         // v720추가, Composite단면의 경우 Long Term에 대한 탄성계수비
	double dShrinkageESEC;        // v720추가, Composite단면의 경우 Shrinkage에 대한 탄성계수비
	int    nStiffNum[4];          // v730추가, Composite 단면의 stiffner 개수(v730이전에서는 nLytype, nLztype 사용했음)
	BOOL bConsiderWarpingEffect;
	int  nWarpingCheck[2];           // v850 추가, I/J  0:Auto  1:User
	double dWarpingCheckPosI[2][6];  // v850 추가, I  (x,y)(6개)
	double dWarpingCheckPosJ[2][6];  // v850 추가, J  (x,y)(6개)
	int nCellNum;                    // v875 추가
	int nFlangeShape;                // v875 추가

	BOOL bHumblyChk;    // v930추가

	void Initialize()
	{
		nStype=0;
		Shape.Empty();
		SName.Empty();
		nOffsetPoint=5;
		SectI.Initialize();
		SectJ.Initialize();
		nLytype=1;
		nLztype=1;
		nSRCtype=0;
		Matl_Elast=0.;
		Matl_Density=0.;    
		Matl_Poisson_S=0.;
		Matl_Poisson_C=0.;
// MNET:XXXX-HSSHIM-20100310.  
// SRC 단면 강성 합성시에 0.8을 써야하는 근거가 더이상 없고,
// 사용자에게 혼동을 줄 수 있어서 Civil과 동일하게 1.0으로 변경 
// #if defined(_CIVIL)
//     Matl_StiffFactor = 1.0;
// #else
//     Matl_StiffFactor = 0.8;
// #endif
		Matl_StiffFactor = 1.0;
		Matl_Thermal=1.0;
		PeriIn=0.0;
		PeriOut=0.0;
		BuiltUpFlag=0;
		nJoint = 0;
		nCellType = 0;
		nCellShape = 0;
		Stiffness.Initialize();
		Design.Initialize();
		bSymmetric = TRUE;
		nOffsetCenter = 0;
		nHorzOffsetOpt = 0;
		nVertOffsetOpt = 0;
		dUserDefOffsetY[0] = dUserDefOffsetY[1] = 0.0;
		dUserDefOffsetZ[0] = dUserDefOffsetZ[1] = 0.0;
		nUserOffsetRef = 0;
		bSmallHole = FALSE;
		bShearCheck = TRUE;
		memset(dShearCheckPos, 0, sizeof(dShearCheckPos));
		//memset(bAutoShearCheckPos, TRUE, sizeof(bAutoShearCheckPos));
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 3; j++)
				bAutoShearCheckPos[i][j] = TRUE;
		memset(dQy, 0, sizeof(dQy));
		memset(bAutoQy, FALSE, sizeof(bAutoQy));
		dWebThk[0] = dWebThk[1] = 0.0;
		bAutoWebThk[0] = bAutoWebThk[1] = FALSE;
		memset(dWebThkShear, 0, sizeof(dWebThkShear));
		memset(bAutoWebThkShear, FALSE, sizeof(bAutoWebThkShear));
		dKt[0] = dKt[1] = 0.0;
		bConsiderShearDeform = TRUE;
		bUserDefMeshSize = FALSE;
		dMeshSize = 0.0;
		bUserInputStiffness = FALSE;
		nCalcStiffOpt = 1;
		bMultipleElastic = FALSE;
		dLongTermESEC = 0.0;
		dShrinkageESEC = 0.0;
		nStiffNum[0] = nStiffNum[1] = nStiffNum[2] = nStiffNum[3] = 0;
		bConsiderWarpingEffect = FALSE;
		memset(nWarpingCheck, 0, sizeof(nWarpingCheck));
		memset(dWarpingCheckPosI, 0, sizeof(dWarpingCheckPosI));
		memset(dWarpingCheckPosJ, 0, sizeof(dWarpingCheckPosJ));
		nCellNum = 1;
		nFlangeShape = 0;
		bHumblyChk = FALSE;
	}
};
struct T_SECT_SECTION_UNIT
{
	T_SECT_SECTION_UNIT()
	{
		nStype=D_UNITSYS_NONE;
		SName=D_UNITSYS_NONE;
		Shape=D_UNITSYS_NONE;
		nOffsetPoint=D_UNITSYS_NONE;
		nLytype=D_UNITSYS_NONE;
		nLztype=D_UNITSYS_NONE;
		PeriIn=D_UNITSYS_BASE_LENGTH;
		PeriOut=D_UNITSYS_BASE_LENGTH;
		dUserDefOffsetY = D_UNITSYS_BASE_LENGTH;
		dUserDefOffsetZ = D_UNITSYS_BASE_LENGTH;
		dShearCheckPos = D_UNITSYS_BASE_LENGTH;
		dWebThk        = D_UNITSYS_BASE_LENGTH;
		dWebThkShear   = D_UNITSYS_BASE_LENGTH;
		dKt            = D_UNITSYS_BASE_VOLUME;
		dMeshSize      = D_UNITSYS_BASE_LENGTH;
		dQy            = D_UNITSYS_BASE_VOLUME;
		dWarpingCheckPos= D_UNITSYS_BASE_LENGTH;
	}
	int nStype;
	int SName;
	int Shape;
	int nOffsetPoint;
	T_SECT_SECTBASE_UNIT SectI;
	T_SECT_SECTBASE_UNIT SectJ;
	int nLytype;
	int nLztype;
	int nSRCtype;
	int Matl_Elast;
	int Matl_Density;
	int Matl_Poisson_S;
	int Matl_Poisson_C;
	int PeriIn;
	int PeriOut;
	int BuiltUpFlag;
	T_SECT_STIFFNESS_UNIT Stiffness;
	T_SECT_DESIGN_UNIT Design;
	int dUserDefOffsetY;
	int dUserDefOffsetZ;
	int dShearCheckPos;
	int dWebThk;
	int dWebThkShear;
	int dKt;
	int dMeshSize;
	int dQy;
	int dWarpingCheckPos;
};
/**
@brief 단면 정보를 가지고 있는 구조체.
*/
struct T_SECT_D
{
	int nStype; /**< 단면 종류(타입) */    // D_SECT_TYPE_??
	CString SName;/**< 단면 이름 */  // section name
	T_SECT_SECTION_D SectBefore;    
	T_SECT_SECTION_D SectAfter;     //시공 전.후 단면일경우에만 사용

	T_SECT_SECTBASE_D CmpTapJ; // v730 추가, 합성단면 변단면에서 SizeJ 처리를 위한 변수

	BOOL bHSQType;    // v820 추가. HSQ Type인가? // MNET:XXXX-HSSHIM-20130428
										// Box형 단면이고, User로 선택한 경우에만 HSQ Type check 가능. DB를 선택한 경우에는 Hide
										// 우선 설계기능때문에 Gen에만 반영 (설계는 Eurocode3:05 + Sweden National Annex인 경우에만 지원)

	double dCoatingThik;  // 콜드폼 단면에서 사용.
	int nFormingType;     // 콜드폼 단면에서 사용. 0: Roll Forming, 1: Other Methods of Forming

	BOOL bRoundType;		// v945에 추가(Box 단면이고, User User로 선택한 경우에만 HSQ Type check 가능. DB를 선택한 경우에는 Hide)
	BOOL bPricipalAxis;		// v945에 추가(L형 단면이고, User로 선택한 경우에만 HSQ Type check 가능. DB를 선택한 경우에는 Hide)

	int nSectAType;
	BOOL bInputSec;
	BOOL bInfusionChk;  // v960
	void Initialize()
	{
		nStype=0;
		SName.Empty();
		SectBefore.Initialize();
		SectAfter.Initialize();
		CmpTapJ.Initialize();
		bHSQType = FALSE;
		dCoatingThik = 0.;
		nFormingType = 0;
		bRoundType = FALSE;
		bPricipalAxis = FALSE;
		nSectAType = 0;
		bInputSec = FALSE;
		bInfusionChk = FALSE;
	}
	static BOOL ConvertShearCheck670(T_SECT_D& rData)
	{
		if ((rData.nStype == D_SECT_TYPE_PSC) || 
				(rData.nStype == D_SECT_TYPE_TAPERED && rData.SectBefore.nStype == D_SECT_TYPE_PSC))
		{
			if (rData.SectBefore.bShearCheck == FALSE)
			{
				rData.SectBefore.bShearCheck = TRUE;
				for (int i = 0; i < 2; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						rData.SectBefore.bAutoShearCheckPos[i][j] = TRUE;
						rData.SectBefore.bAutoQy[i][j] = TRUE;
						rData.SectBefore.bAutoWebThkShear[i][j] = TRUE;
					}
					rData.SectBefore.bAutoWebThk[i] = TRUE;
				}
				return TRUE;  // 강성계산 다시해야 값들이 계산됨
			}
		}
		return FALSE;
	}
};
struct T_SECT_UNIT
{
	T_SECT_UNIT()
	{
		nStype=D_UNITSYS_NONE;
		SName =D_UNITSYS_NONE;
		dCoatingThik = D_UNITSYS_BASE_LENGTH;
	}
	int nStype;
	int SName;
	T_SECT_SECTION_UNIT SectBefore;
	T_SECT_SECTION_UNIT SectAfter;
	int dCoatingThik;
};
struct T_SECT_UDRD_D
{
	T_SECT_K key;
	T_SECT_D data;
};

// 가능하면 NewSect.h, cpp 있는 T_SECT_D_PSC_???? 사용할 것
// 없어질 예정임
struct T_SECT_SIZE_PSC  // Normal(1CELL, 2CELL), Half
{
	BOOL JO1, JO2, JO3, JI1, JI2, JI3, JI4, JI5;
	double HO1, HO2, HO21, HO22, HO3, HO31;
	double BO1, BO11, BO12, BO2, BO21, BO3;
	double HI1, HI2, HI21, HI22, HI3, HI31, HI4, HI41, HI42, HI5;
	double BI1, BI11, BI12, BI21, BI3, BI31, BI32, BI4;
	void Set(double Size[D_SECT_SIZE_NUM_MAX], int nJoint)
	{
		HO1 = Size[0]; HO2 = Size[1]; HO21 = Size[2]; HO22 = Size[3]; 
		HO3 = Size[4]; HO31 = Size[5];
		BO1 = Size[6]; BO11 = Size[7]; BO12 = Size[8]; BO2 = Size[9]; 
		BO21 = Size[10]; BO3 = Size[11];
		HI1 = Size[12]; HI2 = Size[13]; HI21 = Size[14]; HI22 = Size[15]; 
		HI3 = Size[16]; HI31 = Size[17]; HI4 = Size[18]; HI41 = Size[19];
		HI42 = Size[20]; HI5 = Size[21];
		BI1 = Size[22]; BI11 = Size[23]; BI12 = Size[24]; BI21 = Size[25];
		BI3 = Size[26]; BI31 = Size[27]; BI32 = Size[28]; BI4 = Size[29];

		JO1 = (nJoint & 0x0001) != 0;
		JO2 = (nJoint & 0x0002) != 0;
		JO3 = (nJoint & 0x0004) != 0;
		JI1 = (nJoint & 0x0008) != 0;
		JI2 = (nJoint & 0x0010) != 0;
		JI3 = (nJoint & 0x0020) != 0;
		JI4 = (nJoint & 0x0040) != 0;
		JI5 = (nJoint & 0x0080) != 0;
	}
	void Get(double Size[D_SECT_SIZE_NUM_MAX], int& nJoint)
	{
		Size[0] = HO1; Size[1] = HO2;  Size[2] = HO21;  Size[3] = HO22; 
		Size[4] = HO3; Size[5] = HO31;
		Size[6] = BO1; Size[7] = BO11; Size[8] = BO12; Size[9] = BO2; 
		Size[10] = BO21; Size[11] = BO3;
		Size[12] = HI1; Size[13] = HI2; Size[14] = HI21; Size[15] = HI22; 
		Size[16] = HI3; Size[17] = HI31; Size[18] = HI4; Size[19] = HI41;
		Size[20] = HI42; Size[21] = HI5;
		Size[22] = BI1; Size[23] = BI11; Size[24] = BI12; Size[25] = BI21;
		Size[26] = BI3; Size[27] = BI31; Size[28] = BI32; Size[29] = BI4;

		int Set=0x01;
		nJoint = 0;
		if (JO1) nJoint |= Set; Set <<= 1;
		if (JO2) nJoint |= Set; Set <<= 1;
		if (JO3) nJoint |= Set; Set <<= 1;
		if (JI1) nJoint |= Set; Set <<= 1;
		if (JI2) nJoint |= Set; Set <<= 1;
		if (JI3) nJoint |= Set; Set <<= 1;
		if (JI4) nJoint |= Set; Set <<= 1;
		if (JI5) nJoint |= Set; Set <<= 1;
	}
};

// 가능하면 NewSect.h, cpp 있는 T_SECT_D_PSC_???? 사용할 것
// 없어질 예정임
struct T_SECT_SIZE_PSC_MID
{
	BOOL J1, JL1, JL2, JL3, JL4, JR1, JR2, JR3, JR4;
	double H1;
	double HL1, HL2, HL21, HL22, HL3, HL4, HL41, HL42, HL5;
	double BL1, BL2, BL21, BL22, BL41, BL42;
	double HR1, HR2, HR21, HR22, HR3, HR4, HR41, HR42, HR5;
	double BR1, BR2, BR21, BR22, BR41, BR42;
	void Set(double Size[D_SECT_SIZE_NUM_MAX], int nJoint)
	{
		H1 = Size[0];
		HL1 = Size[1]; HL2 = Size[2]; HL21 = Size[3]; HL22 = Size[4];
		HL3 = Size[5]; HL4 = Size[6]; HL41 = Size[7]; HL42 = Size[8];
		HL5 = Size[9];
		BL1 = Size[10]; BL2 = Size[11]; BL21 = Size[12]; BL22 = Size[13];
		BL41 = Size[14]; BL42 = Size[15];
		HR1 = Size[16]; HR2 = Size[17]; HR21 = Size[18]; HR22 = Size[19];
		HR3 = Size[20]; HR4 = Size[21]; HR41 = Size[22]; HR42 = Size[23];
		HR5 = Size[24];
		BR1 = Size[25]; BR2 = Size[26]; BR21 = Size[27]; BR22 = Size[28];
		BR41 = Size[29]; BR42 = Size[30];

		J1  = (nJoint & 0x0001) != 0;
		JL1 = (nJoint & 0x0002) != 0;
		JL2 = (nJoint & 0x0004) != 0;
		JL3 = (nJoint & 0x0008) != 0;
		JL4 = (nJoint & 0x0010) != 0;
		JR1 = (nJoint & 0x0020) != 0;
		JR2 = (nJoint & 0x0040) != 0;
		JR3 = (nJoint & 0x0080) != 0;
		JR4 = (nJoint & 0x0100) != 0;
	}
	void Get(double Size[D_SECT_SIZE_NUM_MAX], int& nJoint)
	{
		Size[0] = H1;
		Size[1] = HL1; Size[2] = HL2; Size[3] = HL21; Size[4] = HL22;
		Size[5] = HL3; Size[6] = HL4; Size[7] = HL41; Size[8] = HL42;
		Size[9] = HL5;
		Size[10] = BL1; Size[11] = BL2; Size[12] = BL21; Size[13] = BL22;
		Size[14] = BL41; Size[15] = BL42;
		Size[16] = HR1; Size[17] = HR2; Size[18] = HR21; Size[19] = HR22;
		Size[20] = HR3; Size[21] = HR4; Size[22] = HR41; Size[23] = HR42;
		Size[24] = HR5;
		Size[25] = BR1; Size[26] = BR2; Size[27] = BR21; Size[28] = BR22;
		Size[29] = BR41; Size[30] = BR42;

		int Set=0x01;
		nJoint = 0;
		if (J1 ) nJoint |= Set; Set <<= 1;
		if (JL1) nJoint |= Set; Set <<= 1;
		if (JL2) nJoint |= Set; Set <<= 1;
		if (JL3) nJoint |= Set; Set <<= 1;
		if (JL4) nJoint |= Set; Set <<= 1;
		if (JR1) nJoint |= Set; Set <<= 1;
		if (JR2) nJoint |= Set; Set <<= 1;
		if (JR3) nJoint |= Set; Set <<= 1;
		if (JR4) nJoint |= Set; Set <<= 1;
	}
};

// 가능하면 NewSect.h, cpp 있는 T_SECT_D_PSC_???? 사용할 것
// 없어질 예정임
struct T_SECT_SIZE_PSC_3CELL
{
	BOOL JI1, JI2, JI3, JI4, JI5, JI6, JI7, JI8, JI9, JI10, JI11, JO1, JO2;
	double HI1, HI2, HI3, HI4, HI5, HI6, HI7, HI8, HI9, HI10;
	double BI1, BI2, BI3, BI4, BI5, BI6, BI7;
	double HO1, HO2;
	double BO1, BO2, BO3;
	double HO21, HO22, HI21, HI22, HI41, HI42, HI61, HI62, HI63, HI81, HI82, HI91, HI92;
	double BO21, BO22, BI11, BI12, BI31, BI32, BI33, BI41, BI42, BI61, BI62, BI71, BI72;
	void Set(double Size[D_SECT_SIZE_NUM_MAX], int nJoint)
	{
		HI1 = Size[0]; HI2 = Size[1]; HI3 = Size[2]; HI4 = Size[3];
		HI5 = Size[4]; HI6 = Size[5]; HI7 = Size[6]; HI8 = Size[7];
		HI9 = Size[8]; HI10 = Size[9];
		BI1 = Size[10]; BI2 = Size[11]; BI3 = Size[12]; BI4 = Size[13];
		BI5 = Size[14]; BI6 = Size[15]; BI7 = Size[16];
		HO1 = Size[17]; HO2 = Size[18];
		BO1 = Size[19]; BO2 = Size[20]; BO3 = Size[21];
		HO21 = Size[22]; HO22 = Size[23]; HI21 = Size[24]; HI22 = Size[25];
		HI41 = Size[26]; HI42 = Size[27]; HI61 = Size[28]; HI62 = Size[29];
		HI63 = Size[30]; HI81 = Size[33]; HI82 = Size[34]; HI91 = Size[31];
		HI92 = Size[32]; 
		BO21 = Size[35]; BO22 = Size[36]; BI11 = Size[37]; BI12 = Size[38];
		BI31 = Size[41]; BI32 = Size[42]; BI33 = Size[43]; BI41 = Size[39];
		BI42 = Size[40]; BI61 = Size[46]; BI62 = Size[47]; BI71 = Size[44];
		BI72 = Size[45];
		
		JI1 = (nJoint & 0x0001) != 0;
		JI2 = (nJoint & 0x0002) != 0;
		JI3 = (nJoint & 0x0004) != 0;
		JI4 = (nJoint & 0x0008) != 0;
		JI5 = (nJoint & 0x0010) != 0;
		JI6 = (nJoint & 0x0020) != 0;
		JI7 = (nJoint & 0x0040) != 0;
		JI8 = (nJoint & 0x0080) != 0;
		JI9 = (nJoint & 0x0100) != 0;
		JI10= (nJoint & 0x0200) != 0;
		JI11= (nJoint & 0x0400) != 0;
		JO1 = (nJoint & 0x0800) != 0;
		JO2 = (nJoint & 0x1000) != 0;
	}
	void Get(double Size[D_SECT_SIZE_NUM_MAX], int& nJoint)
	{
		Size[0] = HI1; Size[1] = HI2; Size[2] = HI3; Size[3] = HI4;
		Size[4] = HI5; Size[5] = HI6; Size[6] = HI7; Size[7] = HI8;
		Size[8] = HI9; Size[9] = HI10;
		Size[10] = BI1; Size[11] = BI2; Size[12] = BI3; Size[13] = BI4;
		Size[14] = BI5; Size[15] = BI6; Size[16] = BI7;
		Size[17] = HO1; Size[18] = HO2;
		Size[19] = BO1; Size[20] = BO2; Size[21] = BO3;
		Size[22] = HO21; Size[23] = HO22; Size[24] = HI21; Size[25] = HI22;
		Size[26] = HI41; Size[27] = HI42; Size[28] = HI61; Size[29] = HI62;
		Size[30] = HI63; Size[33] = HI81; Size[34] = HI82; Size[31] = HI91;
		Size[32] = HI92; 
		Size[35] = BO21; Size[36] = BO22; Size[37] = BI11; Size[38] = BI12;
		Size[41] = BI31; Size[42] = BI32; Size[43] = BI33; Size[39] = BI41;
		Size[40] = BI42; Size[46] = BI61; Size[47] = BI62; Size[44] = BI71;
		Size[45] = BI72;
		
		int Set=0x01;
		nJoint = 0;
		if (JI1) nJoint |= Set; Set <<= 1;
		if (JI2) nJoint |= Set; Set <<= 1;
		if (JI3) nJoint |= Set; Set <<= 1;
		if (JI4) nJoint |= Set; Set <<= 1;
		if (JI5) nJoint |= Set; Set <<= 1;
		if (JI6) nJoint |= Set; Set <<= 1;
		if (JI7) nJoint |= Set; Set <<= 1;
		if (JI8) nJoint |= Set; Set <<= 1;
		if (JI9) nJoint |= Set; Set <<= 1;
		if (JI10) nJoint |= Set; Set <<= 1;
		if (JI11) nJoint |= Set; Set <<= 1;
		if (JO1) nJoint |= Set; Set <<= 1;
		if (JO2) nJoint |= Set; Set <<= 1;
	}
};

// 가능하면 NewSect.h, cpp 있는 T_SECT_D_PSC_???? 사용할 것
// 없어질 예정임
struct T_SECT_SIZE_PSC_4CELL
{
	int nType;  // 0:vertical, 1:slope, 2:chamfer
	int nGirder;
	double H1, H2, H3, H4, H5;
	double B1, B2, B3, B4, B5, B6, B7, B8;
	void Set(const double Size[D_SECT_SIZE_NUM_MAX], int nCellType, int nGirderNum)
	{
		H1 = Size[0]; H2 = Size[1]; H3 = Size[2]; H4 = Size[3]; H5 = Size[4];
		B1 = Size[5]; B2 = Size[6]; B3 = Size[7]; B4 = Size[8]; B5 = Size[9];
		B6 = Size[10]; B7 = Size[11], B8 = Size[12];
		nType = nCellType;
		nGirder = nGirderNum;
		if (nType == 0) B7 = B8 = 0.0;
		else if (nType == 1) B8 = 0.0;
	}
	void Get(double Size[D_SECT_SIZE_NUM_MAX], int& nCellType, int& nGirderNum)
	{
		Size[0] = H1; Size[1] = H2; Size[2] = H3; Size[3] = H4; Size[4] = H5;
		Size[5] = B1; Size[6] = B2; Size[7] = B3; Size[8] = B4; Size[9] = B5;
		Size[10] = B6; Size[11] = B7; Size[12] = B8;
		nCellType = nType;
		nGirderNum = nGirder;
	}
};

// 가능하면 NewSect.h, cpp 있는 T_SECT_D_PSC_???? 사용할 것
// 없어질 예정임
struct T_SECT_PSC_4CELL
{
	int nGirder;
	BOOL J;
	double H1, H2, H3, H4, H5, H11;
	double B1, B2, B3, B4, B5, B6, B7, B8, B21;
};


// 가능하면 NewSect.h, cpp 있는 T_SECT_D_PSC_???? 사용할 것
// 없어질 예정임
struct T_SECT_SIZE_PSC_NCEL2
{
	int nNumberOfCells; // (nCellShape)
	BOOL bSymmetric;
	BOOL JO, JI;        // (nJoint)
	int nCellType;  // 0:Polygon, 1:Circle (nCellType)
	BOOL bSmallHole;
	double dSlabWidth; // (Size[0])
	struct 
	{
		double HO1, HO2, HO21, HO3, HO4;
		double BO1, BO11, BO2, BO3, BO4, BO5;
		double HI1, HI2, HI21, HI3, HI4, HI5, HI6;
		double BI1, BI2, BI21, BI3, BI4, BI5, BI6;
		double R1, R2;
	} Left, Right;
	struct
	{
		double BI7, BI8;  // Middle (Size[28], Size[29]
	} Mid;
	void Set(const T_SECT_SECTION_D& crSection, BOOL bEndI=TRUE)
	{
		nNumberOfCells = crSection.nCellShape;
		bSymmetric = crSection.bSymmetric;
		JO = crSection.nJoint & 0x01;
		JI = crSection.nJoint & 0x02;
		nCellType = crSection.nCellType;
		bSmallHole = crSection.bSmallHole;
		const T_SECT_SECTBASE_D* pSect;
		if (bEndI) pSect = &crSection.SectI;
		else       pSect = &crSection.SectJ;
		SetSize(pSect->Size);
		if (bSymmetric) Right = Left;
		else Right.HO4 = Left.HO1 + Left.HO2 + Left.HO3 + Left.HO4 - Right.HO1 - Right.HO2 - Right.HO3;
	  if (nNumberOfCells < 3) 
		{
			// circle 2cell은 제외
			if (!(nNumberOfCells == 2 && nCellType == 1))
			{
		    dSlabWidth = Left.BO1  + Left.BO2  + Left.BO3  + Left.BO4
		               + Right.BO1 + Right.BO2 + Right.BO3 + Right.BO4;
			}
		}
		if (nCellType == 0) // Polygon
		{
			Right.HI1 = Left.HI1;
			Right.HI3 = Left.HI3;
			Right.HI4 = Left.HI4;
			Right.HI6 = Left.HI6;
			Right.BI6 = Left.BI6;
		}
		else if (nCellType == 1)  // Circle
		{
			Right.HI1 = Left.HI1;
			Right.HI2 = Left.HI2;
		}
	}
	void SetSize(const double dSize[D_SECT_SIZE_NUM_MAX])
	{
		dSlabWidth = dSize[0];
		#define Map(x, y) Left.x = dSize[y];
		Map(HO1, 1); Map(HO2, 2); Map(HO21, 3); Map(HO3, 4); Map(HO4, 5);
		Map(BO1, 6); Map(BO11, 7); Map(BO2, 8); Map(BO3, 9); Map(BO4, 10); Map(BO5, 11);
		Map(HI1, 12); Map(HI2, 13); Map(HI21, 14); Map(HI3, 15); Map(HI4, 16); Map(HI5, 17); Map(HI6, 18);
		Map(BI1, 19); Map(BI2, 20); Map(BI21, 21); Map(BI3, 22); Map(BI4, 23); Map(BI5, 24); Map(BI6, 25);
		Map(R1, 28); Map(R2, 29);
		#undef Map
		#define Map(x, y) Mid.x = dSize[y];
		Map(BI7, 26); Map(BI8, 27);
		#undef Map
		#define Map(x, y) Right.x = dSize[y];
		Map(HO1, 30); Map(HO2, 31); Map(HO21, 32); Map(HO3, 33); Map(HO4, 34);
		Map(BO1, 35); Map(BO11, 36); Map(BO2, 37); Map(BO3, 38); Map(BO4, 39); Map(BO5, 40);
		Map(HI1, 41); Map(HI2, 42); Map(HI21, 43); Map(HI3, 44); Map(HI4, 45); Map(HI5, 46); Map(HI6, 47);
		Map(BI1, 48); Map(BI2, 49); Map(BI21, 50); Map(BI3, 51); Map(BI4, 52); Map(BI5, 53); Map(BI6, 54);
		Map(R1, 57); Map(R2, 58);
		#undef Map
	}
	void Get(T_SECT_SECTION_D& rSection, BOOL bEndI=TRUE)
	{
		if (bSymmetric) Right = Left; // Symmetric이면 먼저 Right를 Left와 통일
		rSection.nCellShape = nNumberOfCells;
		rSection.bSymmetric = bSymmetric;
		rSection.nJoint = 0;
		if (JO) rSection.nJoint |= 0x01;
		if (JI) rSection.nJoint |= 0x02;
		rSection.nCellType = nCellType;
		rSection.bSmallHole = bSmallHole;
		T_SECT_SECTBASE_D* pSect;
		if (bEndI) pSect = &rSection.SectI;
		else       pSect = &rSection.SectJ;
		GetSize(pSect->Size);
	}
	void GetSize(double dSize[D_SECT_SIZE_NUM_MAX])
	{
		dSize[0] = dSlabWidth;
		#define Map(x, y) dSize[y] = Left.x;
		Map(HO1, 1); Map(HO2, 2); Map(HO21, 3); Map(HO3, 4); Map(HO4, 5);
		Map(BO1, 6); Map(BO11, 7); Map(BO2, 8); Map(BO3, 9); Map(BO4, 10); Map(BO5, 11);
		Map(HI1, 12); Map(HI2, 13); Map(HI21, 14); Map(HI3, 15); Map(HI4, 16); Map(HI5, 17); Map(HI6, 18);
		Map(BI1, 19); Map(BI2, 20); Map(BI21, 21); Map(BI3, 22); Map(BI4, 23); Map(BI5, 24); Map(BI6, 25);
		Map(R1, 28); Map(R2, 29);
		#undef Map
		#define Map(x, y) dSize[y] = Mid.x;
		Map(BI7, 26); Map(BI8, 27);
		#undef Map
		#define Map(x, y) dSize[y] = Right.x;
		Map(HO1, 30); Map(HO2, 31); Map(HO21, 32); Map(HO3, 33); Map(HO4, 34);
		Map(BO1, 35); Map(BO11, 36); Map(BO2, 37); Map(BO3, 38); Map(BO4, 39); Map(BO5, 40);
		Map(HI1, 41); Map(HI2, 42); Map(HI21, 43); Map(HI3, 44); Map(HI4, 45); Map(HI5, 46); Map(HI6, 47);
		Map(BI1, 48); Map(BI2, 49); Map(BI21, 50); Map(BI3, 51); Map(BI4, 52); Map(BI5, 53); Map(BI6, 54);
		Map(R1, 57); Map(R2, 58);
		#undef Map
	}
};

// 가능하면 NewSect.h, cpp 있는 T_SECT_D_PSC_???? 사용할 것
// 없어질 예정임
struct T_SECT_SIZE_PSC_T
{
	BOOL J1, JL1, JL2, JL3, JL4, JR1, JR2, JR3, JR4;
	double H1;
	double HL1, HL2, HL3, BL1, BL2, BL3, BL4;
	double HL21, HL22, HL31, HL32, BL21, BL22, BL31, BL32;
	double HR1, HR2, HR3, BR1, BR2, BR3, BR4;
	double HR21, HR22, HR31, HR32, BR21, BR22, BR31, BR32;
	void Set(double Size[D_SECT_SIZE_NUM_MAX], int nJoint)
	{
		H1 = Size[0]; 
		HL1 = Size[1]; HL2 = Size[2]; HL3 = Size[3];
		BL1 = Size[4]; BL2 = Size[5]; BL3 = Size[6]; BL4 = Size[7];
		HL21 = Size[8]; HL22 = Size[9]; HL31 = Size[10]; HL32 = Size[11];
		BL21 = Size[12]; BL22 = Size[13]; BL31 = Size[14]; BL32 = Size[15];
		HR1 = Size[16]; HR2 = Size[17]; HR3 = Size[18];
		BR1 = Size[19]; BR2 = Size[20]; BR3 = Size[21]; BR4 = Size[22];
		HR21 = Size[23]; HR22 = Size[24]; HR31 = Size[25]; HR32 = Size[26];
		BR21 = Size[27]; BR22 = Size[28]; BR31 = Size[29]; BR32 = Size[30];

		J1  = (nJoint & 0x0001) != 0;
		JL1 = (nJoint & 0x0002) != 0;
		JL2 = (nJoint & 0x0004) != 0;
		JL3 = (nJoint & 0x0008) != 0;
		JL4 = (nJoint & 0x0010) != 0;
		JR1 = (nJoint & 0x0020) != 0;
		JR2 = (nJoint & 0x0040) != 0;
		JR3 = (nJoint & 0x0080) != 0;
		JR4 = (nJoint & 0x0100) != 0;
	}
	void Get(double Size[D_SECT_SIZE_NUM_MAX], int& nJoint)
	{
		Size[0] = H1; 
		Size[1] = HL1; Size[2] = HL2; Size[3] = HL3;
		Size[4] = BL1; Size[5] = BL2; Size[6] = BL3; Size[7] = BL4;
		Size[8] = HL21; Size[9] = HL22; Size[10] = HL31; Size[11] = HL32;
		Size[12] = BL21; Size[13] = BL22; Size[14] = BL31; Size[15] = BL32;
		Size[16] = HR1; Size[17] = HR2; Size[18] = HR3;
		Size[19] = BR1; Size[20] = BR2; Size[21] = BR3; Size[22] = BR4;
		Size[23] = HR21; Size[24] = HR22; Size[25] = HR31; Size[26] = HR32;
		Size[27] = BR21; Size[28] = BR22; Size[29] = BR31; Size[30] = BR32;

		int Set=0x01;
		nJoint = 0;
		if (J1 ) nJoint |= Set; Set <<= 1;
		if (JL1) nJoint |= Set; Set <<= 1;
		if (JL2) nJoint |= Set; Set <<= 1;
		if (JL3) nJoint |= Set; Set <<= 1;
		if (JL4) nJoint |= Set; Set <<= 1;
		if (JR1) nJoint |= Set; Set <<= 1;
		if (JR2) nJoint |= Set; Set <<= 1;
		if (JR3) nJoint |= Set; Set <<= 1;
		if (JR4) nJoint |= Set; Set <<= 1;
	}
};

// 가능하면 NewSect.h, cpp 있는 T_SECT_D_PSC_???? 사용할 것
// 없어질 예정임
struct T_SECT_SIZE_PSC_BOX
{
	BOOL JL1, JR1;
	double H1, H2;
	double HOL1, HOL2, HOL3;
	double BOL1, BOL2, BOL3;
	double HOL11, BOL11;
	double HIL1, HIL2;
	double BIL1, BIL2, BIL3, BIL4;
	double HOR1, HOR2, HOR3;
	double BOR1, BOR2, BOR3;
	double HOR11, BOR11;
	double HIR1, HIR2;
	double BIR1, BIR2, BIR3;
	void Set(double Size[D_SECT_SIZE_NUM_MAX], int nJoint)
	{
		H1 = Size[0]; H2 = Size[1];
		HOL1 = Size[2]; HOL2 = Size[3]; HOL3 = Size[4];
		BOL1 = Size[5]; BOL2 = Size[6]; BOL3 = Size[7];
		HOL11 = Size[8]; BOL11 = Size[9];
		HIL1 = Size[10]; HIL2 = Size[11];
		BIL1 = Size[12]; BIL2 = Size[13]; BIL3 = Size[14]; BIL4 = Size[15];
		HOR1 = Size[16]; HOR2 = Size[17]; HOR3 = Size[18];
		BOR1 = Size[19]; BOR2 = Size[20]; BOR3 = Size[21];
		HOR11 = Size[22]; BOR11 = Size[23];
		HIR1 = Size[24]; HIR2 = Size[25];
		BIR1 = Size[26]; BIR2 = Size[27]; BIR3 = Size[28];

		JL1 = (nJoint & 0x0001) != 0;
		JR1 = (nJoint & 0x0002) != 0;
	}
	void Get(double Size[D_SECT_SIZE_NUM_MAX], int& nJoint)
	{
		Size[0] = H1; Size[1] = H2;
		Size[2] = HOL1; Size[3] = HOL2; Size[4] = HOL3;
		Size[5] = BOL1; Size[6] = BOL2; Size[7] = BOL3;
		Size[8] = HOL11; Size[9] = BOL11;
		Size[10] = HIL1; Size[11] = HIL2;
		Size[12] = BIL1; Size[13] = BIL2; Size[14] = BIL3; Size[15] = BIL4;
		Size[16] = HOR1; Size[17] = HOR2; Size[18] = HOR3;
		Size[19] = BOR1; Size[20] = BOR2; Size[21] = BOR3;
		Size[22] = HOR11; Size[23] = BOR11;
		Size[24] = HIR1; Size[25] = HIR2;
		Size[26] = BIR1; Size[27] = BIR2; Size[28] = BIR3;

		int Set=0x01;
		nJoint = 0;
		if (JL1) nJoint |= Set; Set <<= 1;
		if (JR1) nJoint |= Set; Set <<= 1;
	}
};

// 가능하면 NewSect.h, cpp 있는 T_SECT_D_PSC_???? 사용할 것
// 없어질 예정임
struct T_SECT_SIZE_PSC_I
{
	BOOL J1, JL1, JL2, JL3, JL4, JR1, JR2, JR3, JR4;
	double H1;
	double HL1, HL2, HL21, HL22, HL3, HL4, HL41, HL42, HL5;
	double BL1, BL2, BL21, BL22, BL4, BL41, BL42;
	double HR1, HR2, HR21, HR22, HR3, HR4, HR41, HR42, HR5;
	double BR1, BR2, BR21, BR22, BR4, BR41, BR42;
	void Set(double Size[D_SECT_SIZE_NUM_MAX], int nJoint)
	{
		H1 = Size[0];
		HL1 = Size[1]; HL2 = Size[2]; HL21 = Size[3]; HL22 = Size[4];
		HL3 = Size[5]; HL4 = Size[6]; HL41 = Size[7]; HL42 = Size[8];
		HL5 = Size[9];
		BL1 = Size[10]; BL2 = Size[11]; BL21 = Size[12]; BL22 = Size[13];
		BL4 = Size[14]; BL41 = Size[15]; BL42 = Size[16];
		HR1 = Size[17]; HR2 = Size[18]; HR21 = Size[19]; HR22 = Size[20];
		HR3 = Size[21]; HR4 = Size[22]; HR41 = Size[23]; HR42 = Size[24];
		HR5 = Size[25];
		BR1 = Size[26]; BR2 = Size[27]; BR21 = Size[28]; BR22 = Size[29];
		BR4 = Size[30]; BR41 = Size[31]; BR42 = Size[32];

		J1  = (nJoint & 0x0001) != 0;
		JL1 = (nJoint & 0x0002) != 0;
		JL2 = (nJoint & 0x0004) != 0;
		JL3 = (nJoint & 0x0008) != 0;
		JL4 = (nJoint & 0x0010) != 0;
		JR1 = (nJoint & 0x0020) != 0;
		JR2 = (nJoint & 0x0040) != 0;
		JR3 = (nJoint & 0x0080) != 0;
		JR4 = (nJoint & 0x0100) != 0;
	}
	void Get(double Size[D_SECT_SIZE_NUM_MAX], int& nJoint)
	{
		Size[0] = H1;
		Size[1] = HL1; Size[2] = HL2; Size[3] = HL21; Size[4] = HL22;
		Size[5] = HL3; Size[6] = HL4; Size[7] = HL41; Size[8] = HL42;
		Size[9] = HL5;
		Size[10] = BL1; Size[11] = BL2; Size[12] = BL21; Size[13] = BL22;
		Size[14] = BL4; Size[15] = BL41; Size[16] = BL42;
		Size[17] = HR1; Size[18] = HR2; Size[19] = HR21; Size[20] = HR22;
		Size[21] = HR3; Size[22] = HR4; Size[23] = HR41; Size[24] = HR42;
		Size[25] = HR5;
		Size[26] = BR1; Size[27] = BR2; Size[28] = BR21; Size[29] = BR22;
		Size[30] = BR4; Size[31] = BR41; Size[32] = BR42;

		int Set=0x01;
		nJoint = 0;
		if (J1 ) nJoint |= Set; Set <<= 1;
		if (JL1) nJoint |= Set; Set <<= 1;
		if (JL2) nJoint |= Set; Set <<= 1;
		if (JL3) nJoint |= Set; Set <<= 1;
		if (JL4) nJoint |= Set; Set <<= 1;
		if (JR1) nJoint |= Set; Set <<= 1;
		if (JR2) nJoint |= Set; Set <<= 1;
		if (JR3) nJoint |= Set; Set <<= 1;
		if (JR4) nJoint |= Set; Set <<= 1;
	}
};

struct T_SECT_COMPO_B
{
	CString csShape;
	double dWidth;
	int nNum;
	double dCTC;
	struct {
		double Bc, tc, Hh;
	} Slab;
	struct {
		double Hw, tw, B1, Bf1, tf1, B2, Bf2, tf2;
		int N1, N2;
		double Hr1, Hr2, tr1, tr2;
		double EsEc, DsDc;
	} Girder;

	void Set(const T_SECT_D& rSectD)
	{
		csShape = rSectD.SectBefore.Shape;
		dWidth = rSectD.SectAfter.SectI.Size[0];
		dCTC = rSectD.SectAfter.SectI.Size[1];
		nNum = rSectD.SectAfter.SectI.BuiltUpFlag;

		Slab.Bc = rSectD.SectAfter.SectJ.Size[0];
		Slab.tc = rSectD.SectAfter.SectJ.Size[1];
		Slab.Hh = rSectD.SectAfter.SectJ.Size[2];

		Girder.Hw = rSectD.SectBefore.SectI.Size[0];
		Girder.tw = rSectD.SectBefore.SectI.Size[1];
		Girder.B1 = rSectD.SectBefore.SectI.Size[2];
		Girder.Bf1 = rSectD.SectBefore.SectI.Size[3];
		Girder.tf1 = rSectD.SectBefore.SectI.Size[4];
		Girder.B2 = rSectD.SectBefore.SectI.Size[5];
		Girder.Bf2 = rSectD.SectBefore.SectI.Size[6];
		Girder.tf2 = rSectD.SectBefore.SectI.Size[7];

		Girder.N1 = rSectD.SectBefore.nLytype;
		Girder.N2 = rSectD.SectBefore.nLztype;
		Girder.Hr1 = rSectD.SectBefore.SectJ.Size[2];
		Girder.Hr2 = rSectD.SectBefore.SectJ.Size[3];
		Girder.tr1 = rSectD.SectBefore.SectJ.Size[4];
		Girder.tr2 = rSectD.SectBefore.SectJ.Size[5];
		Girder.EsEc = rSectD.SectBefore.Matl_Elast   ;
		Girder.DsDc = rSectD.SectBefore.Matl_Density ;
	}
	void Get(T_SECT_D& rSectD)
	{
		rSectD.Initialize();
		rSectD.nStype = D_SECT_TYPE_COMPO_B;

		rSectD.SectBefore.Shape = csShape;
		rSectD.SectAfter.SectI.Size[0] = dWidth;
		rSectD.SectAfter.SectI.Size[1] = dCTC;
		rSectD.SectAfter.SectI.BuiltUpFlag = nNum;

		rSectD.SectAfter.SectJ.Size[0] = Slab.Bc;
		rSectD.SectAfter.SectJ.Size[1] = Slab.tc;
		rSectD.SectAfter.SectJ.Size[2] = Slab.Hh;

		rSectD.SectBefore.SectI.Size[0] = Girder.Hw;
		rSectD.SectBefore.SectI.Size[1] = Girder.tw;
		rSectD.SectBefore.SectI.Size[2] = Girder.B1;
		rSectD.SectBefore.SectI.Size[3] = Girder.Bf1;
		rSectD.SectBefore.SectI.Size[4] = Girder.tf1;
		rSectD.SectBefore.SectI.Size[5] = Girder.B2;
		rSectD.SectBefore.SectI.Size[6] = Girder.Bf2;
		rSectD.SectBefore.SectI.Size[7] = Girder.tf2;

		rSectD.SectBefore.nLytype = Girder.N1;
		rSectD.SectBefore.nLztype = Girder.N2;
		rSectD.SectBefore.SectJ.Size[2] = Girder.Hr1;
		rSectD.SectBefore.SectJ.Size[3] = Girder.Hr2;
		rSectD.SectBefore.SectJ.Size[4] = Girder.tr1;
		rSectD.SectBefore.SectJ.Size[5] = Girder.tr2;
		rSectD.SectBefore.Matl_Elast    = Girder.EsEc;
		rSectD.SectBefore.Matl_Density  = Girder.DsDc;
	}
};

struct T_SECT_COMPO_I
{
	CString csShape;
	double dWidth;
	int nNum;
	double dCTC;
	struct {
		double Bc, tc, Hh;
	} Slab;
	struct {
		double Hw, tw, B1, tf1, B2, tf2;
		double EsEc, DsDc;
	} Girder;

	void Set(const T_SECT_D& rSectD)
	{
		csShape = rSectD.SectBefore.Shape;
		dWidth = rSectD.SectAfter.SectI.Size[0];
		dCTC = rSectD.SectAfter.SectI.Size[1];
		nNum = rSectD.SectAfter.SectI.BuiltUpFlag;

		Slab.Bc = rSectD.SectAfter.SectJ.Size[0];
		Slab.tc = rSectD.SectAfter.SectJ.Size[1];
		Slab.Hh = rSectD.SectAfter.SectJ.Size[2];

		Girder.Hw = rSectD.SectBefore.SectI.Size[0];
		Girder.tw = rSectD.SectBefore.SectI.Size[1];
		Girder.B1 = rSectD.SectBefore.SectI.Size[2];
		Girder.tf1 = rSectD.SectBefore.SectI.Size[3];
		Girder.B2 = rSectD.SectBefore.SectI.Size[4];
		Girder.tf2 = rSectD.SectBefore.SectI.Size[5];

		Girder.EsEc = rSectD.SectBefore.Matl_Elast   ;
		Girder.DsDc = rSectD.SectBefore.Matl_Density ;
	}
	void Get(T_SECT_D& rSectD)
	{
		rSectD.Initialize();
		rSectD.nStype = D_SECT_TYPE_COMPO_I;

		rSectD.SectBefore.Shape = csShape;
		rSectD.SectAfter.SectI.Size[0] = dWidth;
		rSectD.SectAfter.SectI.Size[1] = dCTC;
		rSectD.SectAfter.SectI.BuiltUpFlag = nNum;

		rSectD.SectAfter.SectJ.Size[0] = Slab.Bc;
		rSectD.SectAfter.SectJ.Size[1] = Slab.tc;
		rSectD.SectAfter.SectJ.Size[2] = Slab.Hh;

		rSectD.SectBefore.SectI.Size[0] = Girder.Hw;
		rSectD.SectBefore.SectI.Size[1] = Girder.tw;
		rSectD.SectBefore.SectI.Size[2] = Girder.B1;
		rSectD.SectBefore.SectI.Size[3] = Girder.tf1;
		rSectD.SectBefore.SectI.Size[4] = Girder.B2;
		rSectD.SectBefore.SectI.Size[5] = Girder.tf2;

		rSectD.SectBefore.Matl_Elast    = Girder.EsEc;
		rSectD.SectBefore.Matl_Density  = Girder.DsDc;
	}
};

struct T_SECT_COMPO_TUB
{
	CString csShape;
	double dWidth;
	int nNum;
	double dCTC;
	struct {
		double Bc, tc, Hh;
	} Slab;
	struct {
		double Hw, tw, B1, Bf1, tf1, B2, Bf2, tf2, Bf3;
		double EsEc, DsDc;
	} Girder;
	
	void Set(const T_SECT_D& rSectD)
	{
		csShape = rSectD.SectBefore.Shape;
		dWidth = rSectD.SectAfter.SectI.Size[0];
		dCTC = rSectD.SectAfter.SectI.Size[1];
		nNum = rSectD.SectAfter.SectI.BuiltUpFlag;
		
		Slab.Bc = rSectD.SectAfter.SectJ.Size[0];
		Slab.tc = rSectD.SectAfter.SectJ.Size[1];
		Slab.Hh = rSectD.SectAfter.SectJ.Size[2];
		
		Girder.Hw = rSectD.SectBefore.SectI.Size[0];
		Girder.tw = rSectD.SectBefore.SectI.Size[1];
		Girder.B1 = rSectD.SectBefore.SectI.Size[2];
		Girder.Bf1 = rSectD.SectBefore.SectI.Size[3];
		Girder.tf1 = rSectD.SectBefore.SectI.Size[4];
		Girder.B2 = rSectD.SectBefore.SectI.Size[5];
		Girder.Bf2 = rSectD.SectBefore.SectI.Size[6];
		Girder.tf2 = rSectD.SectBefore.SectI.Size[7];
		Girder.Bf3 = rSectD.SectBefore.SectI.Size[8];
		
		Girder.EsEc = rSectD.SectBefore.Matl_Elast   ;
		Girder.DsDc = rSectD.SectBefore.Matl_Density ;
	}
	void Get(T_SECT_D& rSectD)
	{
		rSectD.Initialize();
		rSectD.nStype = D_SECT_TYPE_COMPO_B;
		
		rSectD.SectBefore.Shape = csShape;
		rSectD.SectAfter.SectI.Size[0] = dWidth;
		rSectD.SectAfter.SectI.Size[1] = dCTC;
		rSectD.SectAfter.SectI.BuiltUpFlag = nNum;
		
		rSectD.SectAfter.SectJ.Size[0] = Slab.Bc;
		rSectD.SectAfter.SectJ.Size[1] = Slab.tc;
		rSectD.SectAfter.SectJ.Size[2] = Slab.Hh;
		
		rSectD.SectBefore.SectI.Size[0] = Girder.Hw;
		rSectD.SectBefore.SectI.Size[1] = Girder.tw;
		rSectD.SectBefore.SectI.Size[2] = Girder.B1;
		rSectD.SectBefore.SectI.Size[3] = Girder.Bf1;
		rSectD.SectBefore.SectI.Size[4] = Girder.tf1;
		rSectD.SectBefore.SectI.Size[5] = Girder.B2;
		rSectD.SectBefore.SectI.Size[6] = Girder.Bf2;
		rSectD.SectBefore.SectI.Size[7] = Girder.tf2;
		rSectD.SectBefore.SectI.Size[8] = Girder.Bf3;

		rSectD.SectBefore.Matl_Elast    = Girder.EsEc;
		rSectD.SectBefore.Matl_Density  = Girder.DsDc;
	}
};

struct T_SECT_COMPO_STLG_B
{
	CString csShape;
	double dWidth;
	int nNum;
	double dCTC;
	struct {
		double Bc, tc, Hh, Sg;
	} Slab;
	struct {
		double Top, Bot, B1, B2, B3, B4, B5, B6, H, t1, t2, tw1, tw2;
		double EsEc, DsDc;
	} Girder;

	void Set(const T_SECT_D& rSectD)
	{
		csShape = rSectD.SectBefore.Shape;
		dWidth = rSectD.SectAfter.SectI.Size[0];
		dCTC = rSectD.SectAfter.SectI.Size[1];
		nNum = rSectD.SectAfter.SectI.BuiltUpFlag;

		Slab.Bc = rSectD.SectAfter.SectJ.Size[0];
		Slab.tc = rSectD.SectAfter.SectJ.Size[1];
		Slab.Hh = rSectD.SectAfter.SectJ.Size[2];
		Slab.Sg = rSectD.SectAfter.SectJ.Size[3];

		Girder.Top = rSectD.SectAfter.SectJ.Size[4];
		Girder.Bot = rSectD.SectAfter.SectJ.Size[5];
		Girder.B1  = rSectD.SectBefore.SectI.Size[0];
		Girder.B2  = rSectD.SectBefore.SectI.Size[1];
		Girder.B3  = rSectD.SectBefore.SectI.Size[2];
		Girder.B4  = rSectD.SectBefore.SectI.Size[3];
		Girder.B5  = rSectD.SectBefore.SectI.Size[4];
		Girder.B6  = rSectD.SectBefore.SectI.Size[5];
		Girder.H   = rSectD.SectBefore.SectI.Size[6];
		Girder.t1  = rSectD.SectBefore.SectI.Size[7];
		Girder.t2  = rSectD.SectBefore.SectI.Size[8];
		Girder.tw1 = rSectD.SectBefore.SectI.Size[9];
		Girder.tw2 = rSectD.SectBefore.SectI.Size[10];

		Girder.EsEc = rSectD.SectBefore.Matl_Elast   ;
		Girder.DsDc = rSectD.SectBefore.Matl_Density ;
	}
	void Get(T_SECT_D& rSectD)
	{
		rSectD.Initialize();
		rSectD.nStype = D_SECT_TYPE_COMPO_STLG_B;

		rSectD.SectBefore.Shape = csShape;
		rSectD.SectAfter.SectI.Size[0] = dWidth;
		rSectD.SectAfter.SectI.Size[1] = dCTC;
		rSectD.SectAfter.SectI.BuiltUpFlag = nNum;

		rSectD.SectAfter.SectJ.Size[0] = Slab.Bc;
		rSectD.SectAfter.SectJ.Size[1] = Slab.tc;
		rSectD.SectAfter.SectJ.Size[2] = Slab.Hh;
		rSectD.SectAfter.SectJ.Size[3] = Slab.Sg;

		rSectD.SectAfter.SectJ.Size[4]   = Girder.Top; 
		rSectD.SectAfter.SectJ.Size[5]   = Girder.Bot; 
		rSectD.SectBefore.SectI.Size[0]  = Girder.B1;  
		rSectD.SectBefore.SectI.Size[1]  = Girder.B2;  
		rSectD.SectBefore.SectI.Size[2]  = Girder.B3;  
		rSectD.SectBefore.SectI.Size[3]  = Girder.B4;  
		rSectD.SectBefore.SectI.Size[4]  = Girder.B5;  
		rSectD.SectBefore.SectI.Size[5]  = Girder.B6;  
		rSectD.SectBefore.SectI.Size[6]  = Girder.H;   
		rSectD.SectBefore.SectI.Size[7]  = Girder.t1;  
		rSectD.SectBefore.SectI.Size[8]  = Girder.t2;  
		rSectD.SectBefore.SectI.Size[9]  = Girder.tw1; 
		rSectD.SectBefore.SectI.Size[10] = Girder.tw2; 
		
		rSectD.SectBefore.Matl_Elast    = Girder.EsEc;
		rSectD.SectBefore.Matl_Density  = Girder.DsDc;
	}
};

struct T_SECT_COMPO_STLG_I
{
	CString csShape;
	double dWidth;
	int nNum;
	double dCTC;
	struct {
		double Bc, tc, Hh, Sg;
	} Slab;
	struct {
		double Top, Bot, B1, B2, B3, B4, H, t1, t2, tw;
		double EsEc, DsDc;
	} Girder;

	void Set(const T_SECT_D& rSectD)
	{
		csShape = rSectD.SectBefore.Shape;
		dWidth = rSectD.SectAfter.SectI.Size[0];
		dCTC = rSectD.SectAfter.SectI.Size[1];
		nNum = rSectD.SectAfter.SectI.BuiltUpFlag;

		Slab.Bc = rSectD.SectAfter.SectJ.Size[0];
		Slab.tc = rSectD.SectAfter.SectJ.Size[1];
		Slab.Hh = rSectD.SectAfter.SectJ.Size[2];
		Slab.Sg = rSectD.SectAfter.SectJ.Size[3];

		Girder.Top = rSectD.SectAfter.SectJ.Size[4];
		Girder.Bot = rSectD.SectAfter.SectJ.Size[5];
		Girder.B1  = rSectD.SectBefore.SectI.Size[0];
		Girder.B2  = rSectD.SectBefore.SectI.Size[1];
		Girder.B3  = rSectD.SectBefore.SectI.Size[2];
		Girder.B4  = rSectD.SectBefore.SectI.Size[3];
		Girder.H   = rSectD.SectBefore.SectI.Size[4];
		Girder.t1  = rSectD.SectBefore.SectI.Size[5];
		Girder.t2  = rSectD.SectBefore.SectI.Size[6];
		Girder.tw  = rSectD.SectBefore.SectI.Size[7];

		Girder.EsEc = rSectD.SectBefore.Matl_Elast   ;
		Girder.DsDc = rSectD.SectBefore.Matl_Density ;
	}
	void Get(T_SECT_D& rSectD)
	{
		rSectD.Initialize();
		rSectD.nStype = D_SECT_TYPE_COMPO_STLG_I;

		rSectD.SectBefore.Shape = csShape;
		rSectD.SectAfter.SectI.Size[0] = dWidth;
		rSectD.SectAfter.SectI.Size[1] = dCTC;
		rSectD.SectAfter.SectI.BuiltUpFlag = nNum;

		rSectD.SectAfter.SectJ.Size[0] = Slab.Bc;
		rSectD.SectAfter.SectJ.Size[1] = Slab.tc;
		rSectD.SectAfter.SectJ.Size[2] = Slab.Hh;
		rSectD.SectAfter.SectJ.Size[3] = Slab.Sg;

		rSectD.SectAfter.SectJ.Size[4]   = Girder.Top; 
		rSectD.SectAfter.SectJ.Size[5]   = Girder.Bot; 
		rSectD.SectBefore.SectI.Size[0]  = Girder.B1;  
		rSectD.SectBefore.SectI.Size[1]  = Girder.B2;  
		rSectD.SectBefore.SectI.Size[2]  = Girder.B3;  
		rSectD.SectBefore.SectI.Size[3]  = Girder.B4;  
		rSectD.SectBefore.SectI.Size[4]  = Girder.H;   
		rSectD.SectBefore.SectI.Size[5]  = Girder.t1;  
		rSectD.SectBefore.SectI.Size[6]  = Girder.t2;  
		rSectD.SectBefore.SectI.Size[7]  = Girder.tw; 

		rSectD.SectBefore.Matl_Elast    = Girder.EsEc;
		rSectD.SectBefore.Matl_Density  = Girder.DsDc;
	}
};

struct T_SECT_COMPO_STLG_TUB
{
	CString csShape;
	double dWidth;
	int nNum;
	double dCTC;
	struct {
		double Bc, tc, Hh, Sg;
	} Slab;
	struct {
		double Top, Bot, B1, B2, B3, B4, B5, B6, H, t1, t2, tw1, tw2, Bf1, Bf2;
		double EsEc, DsDc;
	} Girder;

	void Set(const T_SECT_D& rSectD)
	{
		csShape = rSectD.SectBefore.Shape;
		dWidth = rSectD.SectAfter.SectI.Size[0];
		dCTC = rSectD.SectAfter.SectI.Size[1];
		nNum = rSectD.SectAfter.SectI.BuiltUpFlag;

		Slab.Bc = rSectD.SectAfter.SectJ.Size[0];
		Slab.tc = rSectD.SectAfter.SectJ.Size[1];
		Slab.Hh = rSectD.SectAfter.SectJ.Size[2];
		Slab.Sg = rSectD.SectAfter.SectJ.Size[3];

		Girder.Top = rSectD.SectAfter.SectJ.Size[4];
		Girder.Bot = rSectD.SectAfter.SectJ.Size[5];
		Girder.B1  = rSectD.SectBefore.SectI.Size[0];
		Girder.B2  = rSectD.SectBefore.SectI.Size[1];
		Girder.B3  = rSectD.SectBefore.SectI.Size[2];
		Girder.B4  = rSectD.SectBefore.SectI.Size[3];
		Girder.B5  = rSectD.SectBefore.SectI.Size[4];
		Girder.B6  = rSectD.SectBefore.SectI.Size[5];
		Girder.H   = rSectD.SectBefore.SectI.Size[6];
		Girder.t1  = rSectD.SectBefore.SectI.Size[7];
		Girder.t2  = rSectD.SectBefore.SectI.Size[8];
		Girder.tw1 = rSectD.SectBefore.SectI.Size[9];
		Girder.tw2 = rSectD.SectBefore.SectI.Size[10];
		Girder.Bf1 = rSectD.SectBefore.SectI.Size[11];
		Girder.Bf2 = rSectD.SectBefore.SectI.Size[12];

		Girder.EsEc = rSectD.SectBefore.Matl_Elast   ;
		Girder.DsDc = rSectD.SectBefore.Matl_Density ;
	}
	void Get(T_SECT_D& rSectD)
	{
		rSectD.Initialize();
		rSectD.nStype = D_SECT_TYPE_COMPO_STLG_B;

		rSectD.SectBefore.Shape = csShape;
		rSectD.SectAfter.SectI.Size[0] = dWidth;
		rSectD.SectAfter.SectI.Size[1] = dCTC;
		rSectD.SectAfter.SectI.BuiltUpFlag = nNum;

		rSectD.SectAfter.SectJ.Size[0] = Slab.Bc;
		rSectD.SectAfter.SectJ.Size[1] = Slab.tc;
		rSectD.SectAfter.SectJ.Size[2] = Slab.Hh;
		rSectD.SectAfter.SectJ.Size[3] = Slab.Sg;

		rSectD.SectAfter.SectJ.Size[4]   = Girder.Top; 
		rSectD.SectAfter.SectJ.Size[5]   = Girder.Bot; 
		rSectD.SectBefore.SectI.Size[0]  = Girder.B1;  
		rSectD.SectBefore.SectI.Size[1]  = Girder.B2;  
		rSectD.SectBefore.SectI.Size[2]  = Girder.B3;  
		rSectD.SectBefore.SectI.Size[3]  = Girder.B4;  
		rSectD.SectBefore.SectI.Size[4]  = Girder.B5;  
		rSectD.SectBefore.SectI.Size[5]  = Girder.B6;  
		rSectD.SectBefore.SectI.Size[6]  = Girder.H;   
		rSectD.SectBefore.SectI.Size[7]  = Girder.t1;  
		rSectD.SectBefore.SectI.Size[8]  = Girder.t2;  
		rSectD.SectBefore.SectI.Size[9]  = Girder.tw1; 
		rSectD.SectBefore.SectI.Size[10] = Girder.tw2; 
		rSectD.SectBefore.SectI.Size[11] = Girder.Bf1; 
		rSectD.SectBefore.SectI.Size[12] = Girder.Bf2; 

		rSectD.SectBefore.Matl_Elast    = Girder.EsEc;
		rSectD.SectBefore.Matl_Density  = Girder.DsDc;
	}
};

struct T_SECT_COMPO_CI
{
	CString csShape;
	double dWidth;
	int nNum;
	double dCTC;
	struct {
		double Bc, tc, Hh;
	} Slab;
	struct {
		BOOL J1, JL1, JL2, JL3, JL4, JR1, JR2, JR3, JR4;
		double H1;
		double HL1, HL2, HL21, HL22, HL3, HL4, HL41, HL42, HL5;
		double BL1, BL2, BL21, BL22, BL4, BL41, BL42;
		double HR1, HR2, HR21, HR22, HR3, HR4, HR41, HR42, HR5;
		double BR1, BR2, BR21, BR22, BR4, BR41, BR42;
	} Girder;

	void Set(const T_SECT_D& rSectD)
	{
		csShape = rSectD.SectBefore.Shape;
		dWidth = rSectD.SectAfter.SectI.Size[0];
		dCTC = rSectD.SectAfter.SectI.Size[1];
		nNum = rSectD.SectAfter.SectI.BuiltUpFlag;

		Slab.Bc = rSectD.SectAfter.SectJ.Size[0];
		Slab.tc = rSectD.SectAfter.SectJ.Size[1];
		Slab.Hh = rSectD.SectAfter.SectJ.Size[2];

		#define Map(x, y) Girder.x = rSectD.SectBefore.SectI.y
		Map(H1,Size[0]);
		Map(HL1,Size[1]); Map(HL2,Size[2]); Map(HL21,Size[3]); Map(HL22,Size[4]);
		Map(HL3,Size[5]); Map(HL4,Size[6]); Map(HL41,Size[7]); Map(HL42,Size[8]);
		Map(HL5,Size[9]);
		Map(BL1,Size[10]); Map(BL2,Size[11]); Map(BL21,Size[12]); Map(BL22,Size[13]);
		Map(BL4,Size[14]); Map(BL41,Size[15]); Map(BL42,Size[16]);
		Map(HR1,Size[17]); Map(HR2,Size[18]); Map(HR21,Size[19]); Map(HR22,Size[20]);
		Map(HR3,Size[21]); Map(HR4,Size[22]); Map(HR41,Size[23]); Map(HR42,Size[24]);
		Map(HR5,Size[25]);
		Map(BR1,Size[26]); Map(BR2,Size[27]); Map(BR21,Size[28]); Map(BR22,Size[29]);
		Map(BR4,Size[30]); Map(BR41,Size[31]); Map(BR42,Size[32]);
		#undef Map

		int nJoint = rSectD.SectBefore.nJoint;
		Girder.J1  = (nJoint & 0x0001) != 0;
		Girder.JL1 = (nJoint & 0x0002) != 0;
		Girder.JL2 = (nJoint & 0x0004) != 0;
		Girder.JL3 = (nJoint & 0x0008) != 0;
		Girder.JL4 = (nJoint & 0x0010) != 0;
		Girder.JR1 = (nJoint & 0x0020) != 0;
		Girder.JR2 = (nJoint & 0x0040) != 0;
		Girder.JR3 = (nJoint & 0x0080) != 0;
		Girder.JR4 = (nJoint & 0x0100) != 0;
	}
	void Get(T_SECT_D& rSectD)
	{
		rSectD.Initialize();
		rSectD.nStype = D_SECT_TYPE_COMPO_CI;

		rSectD.SectBefore.Shape = csShape;
		rSectD.SectAfter.SectI.Size[0] = dWidth;
		rSectD.SectAfter.SectI.Size[1] = dCTC;
		rSectD.SectAfter.SectI.BuiltUpFlag = nNum;

		rSectD.SectAfter.SectJ.Size[0] = Slab.Bc;
		rSectD.SectAfter.SectJ.Size[1] = Slab.tc;
		rSectD.SectAfter.SectJ.Size[2] = Slab.Hh;

		#define Map(x, y) rSectD.SectBefore.SectI.y = Girder.x
		Map(H1,Size[0]);
		Map(HL1,Size[1]); Map(HL2,Size[2]); Map(HL21,Size[3]); Map(HL22,Size[4]);
		Map(HL3,Size[5]); Map(HL4,Size[6]); Map(HL41,Size[7]); Map(HL42,Size[8]);
		Map(HL5,Size[9]);
		Map(BL1,Size[10]); Map(BL2,Size[11]); Map(BL21,Size[12]); Map(BL22,Size[13]);
		Map(BL4,Size[14]); Map(BL41,Size[15]); Map(BL42,Size[16]);
		Map(HR1,Size[17]); Map(HR2,Size[18]); Map(HR21,Size[19]); Map(HR22,Size[20]);
		Map(HR3,Size[21]); Map(HR4,Size[22]); Map(HR41,Size[23]); Map(HR42,Size[24]);
		Map(HR5,Size[25]);
		Map(BR1,Size[26]); Map(BR2,Size[27]); Map(BR21,Size[28]); Map(BR22,Size[29]);
		Map(BR4,Size[30]); Map(BR41,Size[31]); Map(BR42,Size[32]);
		#undef Map

		int Set=0x01;
		int nJoint = 0;
		if (Girder.J1 ) nJoint |= Set; Set <<= 1;
		if (Girder.JL1) nJoint |= Set; Set <<= 1;
		if (Girder.JL2) nJoint |= Set; Set <<= 1;
		if (Girder.JL3) nJoint |= Set; Set <<= 1;
		if (Girder.JL4) nJoint |= Set; Set <<= 1;
		if (Girder.JR1) nJoint |= Set; Set <<= 1;
		if (Girder.JR2) nJoint |= Set; Set <<= 1;
		if (Girder.JR3) nJoint |= Set; Set <<= 1;
		if (Girder.JR4) nJoint |= Set; Set <<= 1;
		rSectD.SectBefore.nJoint = nJoint;

		rSectD.SectBefore.nCellType = 2;  // left:polygon
		rSectD.SectBefore.nCellShape = 2; // right:polygon

		rSectD.SectBefore.Matl_Elast    = 1.0;
		rSectD.SectBefore.Matl_Density  = 1.0;
	}
};

struct T_SECT_COMPO_CT
{
	CString csShape;
	double dWidth;
	int nNum;
	double dCTC;
	struct {
		double Bc, tc, Hh;
	} Slab;
	struct {
		BOOL J1, JL1, JL2, JL3, JL4, JR1, JR2, JR3, JR4;
		double H1;
		double HL1, HL2, HL3, BL1, BL2, BL3, BL4;
		double HL21, HL22, HL31, HL32, BL21, BL22, BL31, BL32;
		double HR1, HR2, HR3, BR1, BR2, BR3, BR4;
		double HR21, HR22, HR31, HR32, BR21, BR22, BR31, BR32;
	} Girder;

	void Set(const T_SECT_D& rSectD)
	{
		csShape = rSectD.SectBefore.Shape;
		dWidth = rSectD.SectAfter.SectI.Size[0];
		dCTC = rSectD.SectAfter.SectI.Size[1];
		nNum = rSectD.SectAfter.SectI.BuiltUpFlag;

		Slab.Bc = rSectD.SectAfter.SectJ.Size[0];
		Slab.tc = rSectD.SectAfter.SectJ.Size[1];
		Slab.Hh = rSectD.SectAfter.SectJ.Size[2];

		#define Map(x, y) Girder.x = rSectD.SectBefore.SectI.y
		Map(H1,Size[0]); 
		Map(HL1,Size[1]); Map(HL2,Size[2]); Map(HL3,Size[3]);
		Map(BL1,Size[4]); Map(BL2,Size[5]); Map(BL3,Size[6]); Map(BL4,Size[7]);
		Map(HL21,Size[8]); Map(HL22,Size[9]); Map(HL31,Size[10]); Map(HL32,Size[11]);
		Map(BL21,Size[12]); Map(BL22,Size[13]); Map(BL31,Size[14]); Map(BL32,Size[15]);
		Map(HR1,Size[16]); Map(HR2,Size[17]); Map(HR3,Size[18]);
		Map(BR1,Size[19]); Map(BR2,Size[20]); Map(BR3,Size[21]); Map(BR4,Size[22]);
		Map(HR21,Size[23]); Map(HR22,Size[24]); Map(HR31,Size[25]); Map(HR32,Size[26]);
		Map(BR21,Size[27]); Map(BR22,Size[28]); Map(BR31,Size[29]); Map(BR32,Size[30]);
		#undef Map

		int nJoint = rSectD.SectBefore.nJoint;
		Girder.J1  = (nJoint & 0x0001) != 0;
		Girder.JL1 = (nJoint & 0x0002) != 0;
		Girder.JL2 = (nJoint & 0x0004) != 0;
		Girder.JL3 = (nJoint & 0x0008) != 0;
		Girder.JL4 = (nJoint & 0x0010) != 0;
		Girder.JR1 = (nJoint & 0x0020) != 0;
		Girder.JR2 = (nJoint & 0x0040) != 0;
		Girder.JR3 = (nJoint & 0x0080) != 0;
		Girder.JR4 = (nJoint & 0x0100) != 0;
	}
	void Get(T_SECT_D& rSectD)
	{
		rSectD.Initialize();
		rSectD.nStype = D_SECT_TYPE_COMPO_CT;

		rSectD.SectBefore.Shape = csShape;
		rSectD.SectAfter.SectI.Size[0] = dWidth;
		rSectD.SectAfter.SectI.Size[1] = dCTC;
		rSectD.SectAfter.SectI.BuiltUpFlag = nNum;

		rSectD.SectAfter.SectJ.Size[0] = Slab.Bc;
		rSectD.SectAfter.SectJ.Size[1] = Slab.tc;
		rSectD.SectAfter.SectJ.Size[2] = Slab.Hh;

		#define Map(x, y) rSectD.SectBefore.SectI.y = Girder.x
		Map(H1,Size[0]); 
		Map(HL1,Size[1]); Map(HL2,Size[2]); Map(HL3,Size[3]);
		Map(BL1,Size[4]); Map(BL2,Size[5]); Map(BL3,Size[6]); Map(BL4,Size[7]);
		Map(HL21,Size[8]); Map(HL22,Size[9]); Map(HL31,Size[10]); Map(HL32,Size[11]);
		Map(BL21,Size[12]); Map(BL22,Size[13]); Map(BL31,Size[14]); Map(BL32,Size[15]);
		Map(HR1,Size[16]); Map(HR2,Size[17]); Map(HR3,Size[18]);
		Map(BR1,Size[19]); Map(BR2,Size[20]); Map(BR3,Size[21]); Map(BR4,Size[22]);
		Map(HR21,Size[23]); Map(HR22,Size[24]); Map(HR31,Size[25]); Map(HR32,Size[26]);
		Map(BR21,Size[27]); Map(BR22,Size[28]); Map(BR31,Size[29]); Map(BR32,Size[30]);
		#undef Map

		int Set=0x01;
		int nJoint = 0;
		if (Girder.J1 ) nJoint |= Set; Set <<= 1;
		if (Girder.JL1) nJoint |= Set; Set <<= 1;
		if (Girder.JL2) nJoint |= Set; Set <<= 1;
		if (Girder.JL3) nJoint |= Set; Set <<= 1;
		if (Girder.JL4) nJoint |= Set; Set <<= 1;
		if (Girder.JR1) nJoint |= Set; Set <<= 1;
		if (Girder.JR2) nJoint |= Set; Set <<= 1;
		if (Girder.JR3) nJoint |= Set; Set <<= 1;
		if (Girder.JR4) nJoint |= Set; Set <<= 1;
		rSectD.SectBefore.nJoint = nJoint;

		rSectD.SectBefore.Matl_Elast    = 1.0;
		rSectD.SectBefore.Matl_Density  = 1.0;
	}
};

struct T_SECT_SOD_STIFFNER_B
{
	double Top, Bot;
	double B1, B2, B3, B4, B5, B6;
	double H, t1, t2;
	double twp_L, twp_R;

	void Initialize()
	{
		Top=Bot=0.0;
		B1=B2=B3=B4=B5=B6=0.0;
		H=t1=t2=0.0;
		twp_L=twp_R=0.0;
	}
};

struct T_SECT_SOD_STIFFNER_B_STIFF
{
	double As_TS, As_LWS, As_RWS, As_BS;
	double Y_TS, Y_LWS, Y_RWS, Y_BS;
	double AsY_TS, AsY_LWS, AsY_RWS, AsY_BS;
	double AsY2_TS, AsY2_LWS, AsY2_RWS, AsY2_BS;
	double Z_TS, Z_LWS, Z_RWS, Z_BS;
	double AsZ_TS, AsZ_LWS, AsZ_RWS, AsZ_BS;
	double AsZ2_TS, AsZ2_LWS, AsZ2_RWS, AsZ2_BS;
	double Igy_TS, Igy_LWS, Igy_RWS, Igy_BS;
	double Igz_TS, Igz_LWS, Igz_RWS, Igz_BS;
	double PeriOut_TS, PeriOut_LWS, PeriOut_RWS, PeriOut_BS;
	double PeriIn_TS, PeriIn_LWS, PeriIn_RWS, PeriIn_BS;
	double PeriOut_TS_M, PeriOut_LWS_M, PeriOut_RWS_M, PeriOut_BS_M; // 스티프너가 Web 에 접합하니까 빼줘야 할 길이
	double PeriIn_TS_M, PeriIn_LWS_M, PeriIn_RWS_M, PeriIn_BS_M; // 스티프너가 Web 에 접합하니까 빼줘야 할 길이

	void Initialize()
	{
		As_TS=As_LWS=As_RWS=As_BS=0.0;
		Y_TS=Y_LWS=Y_RWS=Y_BS=0.0;
		AsY_TS=AsY_LWS=AsY_RWS=AsY_BS=0.0;
		AsY2_TS=AsY2_LWS=AsY2_RWS=AsY2_BS=0.0;
		Z_TS=Z_LWS=Z_RWS=Z_BS=0.0;
		AsZ_TS=AsZ_LWS=AsZ_RWS=AsZ_BS=0.0;
		AsZ2_TS=AsZ2_LWS=AsZ2_RWS=AsZ2_BS=0.0;
		Igy_TS=Igy_LWS=Igy_RWS=Igy_BS=0.0;
		Igz_TS=Igz_LWS=Igz_RWS=Igz_BS=0.0;
		PeriOut_TS=PeriOut_LWS=PeriOut_RWS=PeriOut_BS=0.0;
		PeriIn_TS=PeriIn_LWS=PeriIn_RWS=PeriIn_BS=0.0;
		PeriOut_TS_M=PeriOut_LWS_M=PeriOut_RWS_M=PeriOut_BS_M=0.0;
		PeriIn_TS_M=PeriIn_LWS_M=PeriIn_RWS_M=PeriIn_BS_M=0.0;
	}
};

struct T_SECT_SOD_STIFFNER_I
{
	double Top, Bot;
	double B1, B2, B3, B4;
	double H, t1, t2;
	double twp;

	void Initialize()
	{
		Top=Bot=0.0;
		B1=B2=B3=B4=0.0;
		H=t1=t2=0.0;
		twp=0.0;
	}
};

struct T_SECT_SOD_STIFFNER_I_STIFF
{
	double As_TS, As_WS, As_BS;
	double Y_TS, Y_WS, Y_BS;
	double AsY_TS, AsY_WS, AsY_BS;
	double AsY2_TS, AsY2_WS, AsY2_BS;
	double Z_TS, Z_WS, Z_BS;
	double AsZ_TS, AsZ_WS, AsZ_BS;
	double AsZ2_TS, AsZ2_WS, AsZ2_BS;
	double Igy_TS, Igy_WS, Igy_BS;
	double Igz_TS, Igz_WS, Igz_BS;
	double PeriOut_TS, PeriOut_WS, PeriOut_BS;
	double PeriOut_TS_M, PeriOut_WS_M, PeriOut_BS_M; // 스티프너가 Web 에 접합하니까 빼줘야 할 길이

	void Initialize()
	{
		As_TS=As_WS=As_BS=0.0;
		Y_TS=Y_WS=Y_BS=0.0;
		AsY_TS=AsY_WS=AsY_BS=0.0;
		AsY2_TS=AsY2_WS=AsY2_BS=0.0;
		Z_TS=Z_WS=Z_BS=0.0;
		AsZ_TS=AsZ_WS=AsZ_BS=0.0;
		AsZ2_TS=AsZ2_WS=AsZ2_BS=0.0;
		Igy_TS=Igy_WS=Igy_BS=0.0;
		Igz_TS=Igz_WS=Igz_BS=0.0;
		PeriOut_TS=PeriOut_WS=PeriOut_BS=0.0;
		PeriOut_TS_M=PeriOut_WS_M=PeriOut_BS_M=0.0;
	}
};

struct T_SECT_SOD_STIFFNER_TUB
{
	double Top, Bot;
	double B1, B2, B3, B4, B5, B6;
	double H, t1, t2;
	double twp_L, twp_R;
	double bf1, bf2;

	void Initialize()
	{
		Top=Bot=0.0;
		B1=B2=B3=B4=B5=B6=0.0;
		H=t1=t2=0.0;
		twp_L=twp_R=0.0;
		bf1=bf2=0.0;
	}
};

struct T_SECT_SOD_STIFFNER_TUB_STIFF
{
	double As_LWS, As_RWS, As_BS;
	double Y_LWS, Y_RWS, Y_BS;
	double AsY_LWS, AsY_RWS, AsY_BS;
	double AsY2_LWS, AsY2_RWS, AsY2_BS;
	double Z_LWS, Z_RWS, Z_BS;
	double AsZ_LWS, AsZ_RWS, AsZ_BS;
	double AsZ2_LWS, AsZ2_RWS, AsZ2_BS;
	double Igy_LWS, Igy_RWS, Igy_BS;
	double Igz_LWS, Igz_RWS, Igz_BS;
	double PeriOut_LWS, PeriOut_RWS, PeriOut_BS;
	double PeriOut_LWS_M, PeriOut_RWS_M, PeriOut_BS_M; // 스티프너가 Web 에 접합하니까 빼줘야 할 길이

	void Initialize()
	{
		As_LWS=As_RWS=As_BS=0.0;
		Y_LWS=Y_RWS=Y_BS=0.0;
		AsY_LWS=AsY_RWS=AsY_BS=0.0;
		AsY2_LWS=AsY2_RWS=AsY2_BS=0.0;
		Z_LWS=Z_RWS=Z_BS=0.0;
		AsZ_LWS=AsZ_RWS=AsZ_BS=0.0;
		AsZ2_LWS=AsZ2_RWS=AsZ2_BS=0.0;
		Igy_LWS=Igy_RWS=Igy_BS=0.0;
		Igz_LWS=Igz_RWS=Igz_BS=0.0;
		PeriOut_LWS=PeriOut_RWS=PeriOut_BS=0.0;
		PeriOut_LWS_M=PeriOut_RWS_M=PeriOut_BS_M=0.0;
	}
};

struct T_SECT_SOD_STIFFNER_MCELL
{
	double B1, B2, B3, B4, B5, B6;
	double Br1, Br2, Br3, Br4, Br5;
	double H, tf1, tf2, tw1, twp_L, twp_R, tw2;
	double Top, Bot, Joint1, Joint2;
	double rh, rb, rt, rhr, rbr, rtr;
	double rh1, rh2, rh3, rt1, rt2, rt2p, rhr1, rhr2, rhr3, rtr1, rtr2, rtr2p;
	double sideB, sideH, sideBr, sideHr;
	double jh1, jh2, jb1, jb2, jhr1, jhr2, jbr1, jbr2;
	BOOL   J1, J2, Jr1, Jr2;
	
	void Initialize()
	{
		B1=B2=B3=B4=B5=B6=0.0;
		Br1=Br2=Br3=Br4=Br5=0.0;
		H=tf1=tf2=tw1=twp_L=twp_R=tw2=0.0;
		Top=Bot=Joint1=Joint2=0.0;
		rh=rb=rt=rhr=rbr=rtr=0.0;
		rh1=rh2=rh3=rt1=rt2=rt2p=rhr1=rhr2=rhr3=rtr1=rtr2=rtr2p=0.0;
		sideB=sideH=sideBr=sideHr=0.0;
		jh1=jh2=jb1=jb2=jhr1=jhr2=jbr1=jbr2=0.0;
		J1=J2=Jr1=Jr2=FALSE;
	}
};

struct T_SECT_SOD_STIFFNER_MCELL_STIFF
{
	double As_TS, As_LWS, As_RWS, As_BS, As_IWS, As_SF;
	double Y_TS,  Y_LWS,  Y_RWS,  Y_BS, Y_IWS, Y_SF;
	double AsY_TS, AsY_LWS, AsY_RWS, AsY_BS, AsY_IWS, AsY_SF;
	double AsY2_TS, AsY2_LWS, AsY2_RWS, AsY2_BS, AsY2_IWS, AsY2_SF;
	double Z_TS, Z_LWS, Z_RWS, Z_BS, Z_IWS, Z_SF;
	double AsZ_TS, AsZ_LWS, AsZ_RWS, AsZ_BS, AsZ_IWS, AsZ_SF;
	double AsZ2_TS, AsZ2_LWS, AsZ2_RWS, AsZ2_BS, AsZ2_IWS, AsZ2_SF;
	double Igy_TS, Igy_LWS, Igy_RWS, Igy_BS, Igy_IWS, Igy_SF;
	double Igz_TS, Igz_LWS, Igz_RWS, Igz_BS, Igz_IWS, Igz_SF;
	double PeriOut_TS, PeriOut_LWS, PeriOut_RWS, PeriOut_BS, PeriOut_IWS, PeriOut_SF;
	double PeriIn_TS, PeriIn_LWS, PeriIn_RWS, PeriIn_BS, PeriIn_IWS, PeriIn_SF;
	double PeriOut_TS_M, PeriOut_LWS_M, PeriOut_RWS_M, PeriOut_BS_M,PeriOut_IWS_M,PeriOut_SF_M; // 스티프너가 Web 에 접합하니까 빼줘야 할 길이
	double PeriIn_TS_M, PeriIn_LWS_M, PeriIn_RWS_M, PeriIn_BS_M, PeriIn_IWS_M, PeriIn_SF_M; // 스티프너가 Web 에 접합하니까 빼줘야 할 길이

	void Initialize()
	{
		As_TS=As_LWS=As_RWS=As_BS=As_IWS=As_SF=0.0;
		Y_TS=Y_LWS=Y_RWS=Y_BS=Y_IWS=Y_SF=0.0;
		AsY_TS=AsY_LWS=AsY_RWS=AsY_BS=AsY_IWS=AsY_SF=0.0;
		AsY2_TS=AsY2_LWS=AsY2_RWS=AsY2_BS=AsY2_IWS=AsY2_SF=0.0;
		Z_TS=Z_LWS=Z_RWS=Z_BS=Z_IWS=Z_SF=0.0;
		AsZ_TS=AsZ_LWS=AsZ_RWS=AsZ_BS=AsZ_IWS=AsZ_SF=0.0;
		AsZ2_TS=AsZ2_LWS=AsZ2_RWS=AsZ2_BS=AsZ2_IWS=AsZ2_SF=0.0;
		Igy_TS=Igy_LWS=Igy_RWS=Igy_BS=Igy_IWS=Igy_SF=0.0;
		Igz_TS=Igz_LWS=Igz_RWS=Igz_BS=Igz_IWS=Igz_SF=0.0;
		PeriOut_TS=PeriOut_LWS=PeriOut_RWS=PeriOut_BS=PeriOut_IWS=PeriOut_SF=0.0;
		PeriIn_TS=PeriIn_LWS=PeriIn_RWS=PeriIn_BS=PeriIn_IWS=PeriIn_SF=0.0;
		PeriOut_TS_M=PeriOut_LWS_M=PeriOut_RWS_M=PeriOut_BS_M=PeriOut_IWS_M=PeriOut_SF_M=0.0;
		PeriIn_TS_M=PeriIn_LWS_M=PeriIn_RWS_M=PeriIn_BS_M=PeriIn_IWS_M=PeriIn_SF_M=0.0;

	}
};

struct T_SECT_EFFWIDTH_SPACING
{
	CArray<double, double> aAsl_L; // left
	CArray<double, double> aAsl_R; // right
	CArray<double, double> aAsl_C; // left+right

	void Initialize()
	{
		aAsl_L.RemoveAll();     
		aAsl_R.RemoveAll();     
		aAsl_C.RemoveAll();     
	}
	T_SECT_EFFWIDTH_SPACING () { Initialize(); }
	T_SECT_EFFWIDTH_SPACING(const T_SECT_EFFWIDTH_SPACING& rData) {*this = rData;}
	T_SECT_EFFWIDTH_SPACING& operator=(const T_SECT_EFFWIDTH_SPACING& rData)
	{
		aAsl_L.Copy(rData.aAsl_L);
		aAsl_R.Copy(rData.aAsl_R);
		aAsl_C.Copy(rData.aAsl_C);
		return *this;
	}
};

/*
struct T_SECT_DB_DATA
{
	T_SECT_SECTBASE_D SectDB;
//  T_SECT_STIFFNESS Stiffness;
	T_SECT_DESIGN Design;   
	double Peri;
	int BuiltUpFlag;  // 1=Builtup Section(D_SECT_BUILT_BUILTUP)  2=Rolled Section(D_SECT_BUILT_ROLLED)
	void Initialize()
	{
		SectDB.Initialize();
//      Stiffness.Initialize();
		Design.Initialize();
		Peri=0.;
		BuiltUpFlag=0;
	}
};
*/
struct T_SECT_DB_UNIT
{
	T_SECT_DB_UNIT()
	{
		PeriIn=D_UNITSYS_BASE_LENGTH;
		PeriOut=D_UNITSYS_BASE_LENGTH;
		BuiltUpFlag=D_UNITSYS_NONE;
	}
	int PeriIn;
	int PeriOut;
	int BuiltUpFlag;
	T_SECT_SECTBASE_UNIT SectDB;
	T_SECT_STIFFNESS_UNIT Stiffness;
	T_SECT_DESIGN_UNIT Design;
};
#define HASHSIZESECT 101
#define D_SECT_MINNUM       1   // 입력가능한 최소 단면번호
//#define D_SECT_MAXNUM    9999   // 입력가능한 최대 단면번호
#define D_SECT_MAXNUM    999999   // 입력가능한 최대 단면번호(680부터)

//#define T_SECF_K T_SECT_K
#define T_SECF_KEY unsigned int
union T_SECF_K
{
	T_SECF_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int serial : 12;   // 0-4096
	}key;
};

typedef CArray<T_SECF_K, T_SECF_K> T_SECF_K_LIST;

struct T_SECF_D_530
{
	double dScaleFactor[6];
};
struct T_SECF_D_670
{
	double dScaleFactor[7];
};
struct T_SECF_D_712
{
	double dScaleFactor[7]; // Area, ..., WArea
	UINT   GroupKey;
};
struct T_SECF_D_720
{
	double dScaleFactor[7]; // Area, ..., WArea
	UINT   GroupKey;
	BOOL   bBefore;         // v720 추가, Composite단면의 경우 사용
	BOOL   bAfter;          // v720 추가, Composite단면의 경우 사용

};
struct T_SECF_D_815
{
	double dScaleFactor[7]; // Area, ..., WArea
	double dScaleFactorJ[7]; // v721 추가, Tapered단면의 경우 사용
	UINT   GroupKey;
	BOOL   bBefore;
	BOOL   bAfter;
	BOOL   bSameIJ; // v721 추가, Tapered단면의 경우 사용
};
struct T_SECF_D
{
	double dScaleFactor[7]; // Area, ..., WArea
	double dScaleFactorJ[7]; // v721 추가, Tapered단면의 경우 사용
	UINT   GroupKey;
	BOOL   bBefore;
	BOOL   bAfter;
	BOOL   bSameIJ; // v721 추가, Tapered단면의 경우 사용
	double dWarpingScaleFactor;		// v820 추가
	double dWarpingScaleFactorJ;	// v820 추가
	void Initialize()
	{
		for (int i = 0; i < 7; i++) dScaleFactor[i] = 1.0;
		for (int i = 0; i < 7; i++) dScaleFactorJ[i] = 1.0;
		GroupKey = 0;
		bBefore = TRUE;
		bAfter  = FALSE;
		bSameIJ = TRUE;
		dWarpingScaleFactor = 1.0;
		dWarpingScaleFactorJ = 1.0;
	}
	void Convert530(T_SECF_D_530& rData, int nStype)
	{
		for (int i = 0; i < 6; i++) dScaleFactor[i] = rData.dScaleFactor[i];
		dScaleFactor[6] = 1.0;
		for (int i = 0; i < 6; i++) dScaleFactorJ[i] = rData.dScaleFactor[i];
		dScaleFactorJ[6] = 1.0;
		GroupKey = 0;
		bBefore = TRUE;
		bAfter = (nStype==D_SECT_TYPE_COMPO_B || nStype==D_SECT_TYPE_COMPO_I);
		bSameIJ = TRUE;
		dWarpingScaleFactor = 1.0;
		dWarpingScaleFactorJ = 1.0;
	}
	void Convert670(T_SECF_D_670& rData, int nStype)
	{
		for (int i = 0; i < 7; i++) dScaleFactor[i] = rData.dScaleFactor[i];    
		for (int i = 0; i < 7; i++) dScaleFactorJ[i] = rData.dScaleFactor[i];
		GroupKey = 0;   
		bBefore = TRUE;
		bAfter = (nStype==D_SECT_TYPE_COMPO_B || nStype==D_SECT_TYPE_COMPO_I);
		bSameIJ = TRUE;
		dWarpingScaleFactor = 1.0;
		dWarpingScaleFactorJ = 1.0;
	}
	void Convert712(T_SECF_D_712& rData, int nStype)
	{
		for (int i = 0; i < 7; i++) dScaleFactor[i] = rData.dScaleFactor[i];    
		for (int i = 0; i < 7; i++) dScaleFactorJ[i] = rData.dScaleFactor[i];
		GroupKey = rData.GroupKey;   
		bBefore = TRUE;
		bAfter = (nStype==D_SECT_TYPE_COMPO_B || nStype==D_SECT_TYPE_COMPO_I);
		bSameIJ = TRUE;
		dWarpingScaleFactor = 1.0;
		dWarpingScaleFactorJ = 1.0;
	}
	void Convert720(T_SECF_D_720& rData, int nStype)
	{
		for (int i = 0; i < 7; i++) dScaleFactor[i] = rData.dScaleFactor[i];    
		for (int i = 0; i < 7; i++) dScaleFactorJ[i] = rData.dScaleFactor[i];
		GroupKey = rData.GroupKey;   
		bBefore = rData.bBefore;
		bAfter = rData.bAfter;
		bSameIJ = TRUE;
		dWarpingScaleFactor = 1.0;
		dWarpingScaleFactorJ = 1.0;
	}
	void Convert815(T_SECF_D_815& rData, int nStype)
	{
		for (int i = 0; i < 7; i++) dScaleFactor[i] = rData.dScaleFactor[i];    
		for (int i = 0; i < 7; i++) dScaleFactorJ[i] = rData.dScaleFactor[i];
		GroupKey = rData.GroupKey;   
		bBefore = rData.bBefore;
		bAfter = rData.bAfter;
		bSameIJ = TRUE;
		dWarpingScaleFactor = 1.0;
		dWarpingScaleFactorJ = 1.0;
	}
};

typedef CArray<T_SECF_D, T_SECF_D&> T_SECF_D_LIST;

struct T_SECF_UDRD_D_530
{
	T_SECF_K key;
	T_SECF_D_530 data;
};
struct T_SECF_UDRD_D_670
{
	T_SECF_K key;
	T_SECF_D_670 data;
};
struct T_SECF_UDRD_D_712
{
	T_SECF_K key;
	T_SECF_D_712 data;
};
struct T_SECF_UDRD_D_720
{
	T_SECF_K key;
	T_SECF_D_720 data;
};
struct T_SECF_UDRD_D_815
{
	T_SECF_K key;
	T_SECF_D_815 data;
};
struct T_SECF_UDRD_D
{
	T_SECF_K key;
	T_SECF_D data;
};
struct T_SECF_UNIT
{
	T_SECF_UNIT()
	{
		dScaleFactor=D_UNITSYS_NONE;
	}
	int dScaleFactor;
};
#define HASHSIZESECF HASHSIZESECT

// Composite Section for Construction Stage
#define T_CSCS_K unsigned int
struct T_CSCS_BASE_600
{
	int  nPart;
	UINT MatlKey;       // 0:Element, else Material No
	UINT CompStagKey;   // 0:Active Stage, else Stage Key
	double dAge;
	//double dStiffScale; // 아래것으로 변경
	T_SECT_STIFFNESS_600 StiffScale;  // 강성 Scale Factor
	T_SECT_STIFFNESS_600 Stiffness;   // Part별 강성
	T_SECT_STIFFNESS_600 StiffnessJ;  // Part별 강성 (Tapered일 때 J단면 강성)
	double YBar, ZBar;            // Part별 도심
	double YBarJ, ZBarJ;          // Part별 도심 (Tapered일 때 J단면 도심)
	void Initialize()
	{
		nPart = 0;
		MatlKey = 0;
		CompStagKey = 0;
		dAge = 0.0;
		StiffScale.Initialize(1.0);
		Stiffness.Initialize();
		StiffnessJ.Initialize();
		YBar = ZBar = 0.0;
		YBarJ, ZBarJ = 0.0;
	}
};
struct T_CSCS_BASE_721
{
	int  nPart;
	UINT MatlKey;       // 0:Element, else Material No
	UINT CompStagKey;   // 0:Active Stage, else Stage Key
	double dAge;
	//double dStiffScale; // 아래것으로 변경
	T_SECT_STIFFNESS_815 StiffScale;  // 강성 Scale Factor
	T_SECT_STIFFNESS_815 Stiffness;   // Part별 강성
	T_SECT_STIFFNESS_815 StiffnessJ;  // Part별 강성 (Tapered일 때 J단면 강성)
	double YBar, ZBar;            // Part별 도심
	double YBarJ, ZBarJ;          // Part별 도심 (Tapered일 때 J단면 도심)
	void Initialize()
	{
		nPart = 0;
		MatlKey = 0;
		CompStagKey = 0;
		dAge = 0.0;
		StiffScale.Initialize(1.0);
		Stiffness.Initialize();
		StiffnessJ.Initialize();
		YBar = ZBar = 0.0;
		YBarJ, ZBarJ = 0.0;
	}
};
struct T_CSCS_BASE_815
{
	int  nPart;
	UINT MatlKey;       // 0:Element, else Material No
	UINT CompStagKey;   // 0:Active Stage, else Stage Key
	double dAge;
	//double dStiffScale; // 아래것으로 변경
	T_SECT_STIFFNESS_815 StiffScale;  // 강성 Scale Factor
	T_SECT_STIFFNESS_815 Stiffness;   // Part별 강성
	T_SECT_STIFFNESS_815 StiffnessJ;  // Part별 강성 (Tapered일 때 J단면 강성)
	double YBar, ZBar;            // Part별 도심
	double YBarJ, ZBarJ;          // Part별 도심 (Tapered일 때 J단면 도심)
	double dH;                    // 730 추가
	void Initialize()
	{
		nPart = 0;
		MatlKey = 0;
		CompStagKey = 0;
		dAge = 0.0;
		StiffScale.Initialize(1.0);
		Stiffness.Initialize();
		StiffnessJ.Initialize();
		YBar = ZBar = 0.0;
		YBarJ, ZBarJ = 0.0;
		dH = 0.0;
	}
};

struct T_CSCS_BASE_842
{
	int               nPart;
	UINT              MatlKey;        // 0:Element, else Material No
	UINT              CompStagKey;    // 0:Active Stage, else Stage Key
	double            dAge;
	T_SECT_STIFFNESS  StiffScale;     // 강성 Scale Factor
	T_SECT_STIFFNESS  Stiffness;      // Part별 강성
	T_SECT_STIFFNESS  StiffnessJ;     // Part별 강성 (Tapered일 때 J단면 강성)
	double            YBar, ZBar;     // Part별 도심
	double            YBarJ, ZBarJ;   // Part별 도심 (Tapered일 때 J단면 도심)
	double            dH;             // 730 추가

	void Initialize()
	{
		nPart         = 0;
		MatlKey       = 0;
		CompStagKey   = 0;
		dAge          = 0.0;
		StiffScale    .Initialize(1.0);
		Stiffness     .Initialize();
		StiffnessJ    .Initialize();
		YBar = ZBar   = 0.0;
		YBarJ = ZBarJ = 0.0;
		dH            = 0.0;
	}
};

struct T_CSCS_BASE_905
{
	int               nPart;
	UINT              MatlKey;        // 0:Element, else Material No
	UINT              CompStagKey;    // 0:Active Stage, else Stage Key
	double            dAge;
	T_SECT_STIFFNESS  StiffScale;     // 강성 Scale Factor
	T_SECT_STIFFNESS  Stiffness;      // Part별 강성
	T_SECT_STIFFNESS  StiffnessJ;     // Part별 강성 (Tapered일 때 J단면 강성)
	double            YBar, ZBar;     // Part별 도심
	double            YBarJ, ZBarJ;   // Part별 도심 (Tapered일 때 J단면 도심)
	double            dH;             // 730 추가
	double            dVS;            // 845 추가

	void Initialize()
	{
		nPart = 0;
		MatlKey = 0;
		CompStagKey = 0;
		dAge = 0.0;
		StiffScale.Initialize(1.0);
		Stiffness.Initialize();
		StiffnessJ.Initialize();
		YBar = ZBar = 0.0;
		YBarJ = ZBarJ = 0.0;
		dH = 0.0;
		dVS = 0.0;
	}
};

struct T_CSCS_BASE
{
	int               nPart;
	UINT              MatlKey;        // 0:Element, else Material No
	UINT              CompStagKey;    // 0:Active Stage, else Stage Key
	double            dAge;
	T_SECT_STIFFNESS  StiffScale;     // 강성 Scale Factor
	T_SECT_STIFFNESS  Stiffness;      // Part별 강성
	T_SECT_STIFFNESS  StiffnessJ;     // Part별 강성 (Tapered일 때 J단면 강성)
	double            YBar, ZBar;     // Part별 도심
	double            YBarJ, ZBarJ;   // Part별 도심 (Tapered일 때 J단면 도심)
	double            dH;             // 730 추가
	double            dVS;            // 845 추가
	double            dM;             // 910 추가

	void Initialize()
	{
		nPart         = 0;
		MatlKey       = 0;
		CompStagKey   = 0;
		dAge          = 0.0;
		StiffScale    .Initialize(1.0);
		Stiffness     .Initialize();
		StiffnessJ    .Initialize();
		YBar = ZBar   = 0.0;
		YBarJ = ZBarJ = 0.0;
		dH            = 0.0;
		dVS           = 0.0;
		dM            = 0.0;
	}
	void Convert600(T_CSCS_BASE_600& rData)
	{
		nPart         = rData.nPart;
		MatlKey       = rData.MatlKey;
		CompStagKey   = rData.CompStagKey;
		dAge          = rData.dAge;
		StiffScale    .Convert600(rData.StiffScale);
		Stiffness     .Convert600(rData.Stiffness);
		StiffnessJ    .Convert600(rData.StiffnessJ);
		YBar          = rData.YBar;
		ZBar          = rData.ZBar;
		YBarJ         = rData.YBarJ;
		ZBarJ         = rData.ZBarJ;
		dH            = 0.0;
		dVS           = 0.0;
		dM = 0.0;
	}
	void Convert721(T_CSCS_BASE_721& rData)
	{
		nPart         = rData.nPart;
		MatlKey       = rData.MatlKey;
		CompStagKey   = rData.CompStagKey;
		dAge          = rData.dAge;
		StiffScale    .Convert815(rData.StiffScale);
		Stiffness     .Convert815(rData.Stiffness);
		StiffnessJ    .Convert815(rData.StiffnessJ);
		YBar          = rData.YBar;
		ZBar          = rData.ZBar;
		YBarJ         = rData.YBarJ;
		ZBarJ         = rData.ZBarJ;
		dH            = 0.0;
		dVS           = 0.0;
		dM = 0.0;
	}
	void Convert815(T_CSCS_BASE_815& rData)
	{
		nPart         = rData.nPart;
		MatlKey       = rData.MatlKey;
		CompStagKey   = rData.CompStagKey;
		dAge          = rData.dAge;
		StiffScale    .Convert815(rData.StiffScale);
		Stiffness     .Convert815(rData.Stiffness);
		StiffnessJ    .Convert815(rData.StiffnessJ);
		YBar          = rData.YBar;
		ZBar          = rData.ZBar;
		YBarJ         = rData.YBarJ;
		ZBarJ         = rData.ZBarJ;
		dH            = rData.dH;
		dVS           = 0.0;
		dM = 0.0;
	}
	void Convert842(T_CSCS_BASE_842& rData)
	{
		nPart         = rData.nPart;
		MatlKey       = rData.MatlKey;
		CompStagKey   = rData.CompStagKey;
		dAge          = rData.dAge;
		StiffScale    = rData.StiffScale;
		Stiffness     = rData.Stiffness;
		StiffnessJ    = rData.StiffnessJ;
		YBar          = rData.YBar;
		ZBar          = rData.ZBar;
		YBarJ         = rData.YBarJ;
		ZBarJ         = rData.ZBarJ;
		dH            = rData.dH;
		dVS           = 0.0;
		dM = 0.0;
	}
	void Convert905(T_CSCS_BASE_905& rData)
	{
		nPart = rData.nPart;
		MatlKey = rData.MatlKey;
		CompStagKey = rData.CompStagKey;
		dAge = rData.dAge;
		StiffScale = rData.StiffScale;
		Stiffness = rData.Stiffness;
		StiffnessJ = rData.StiffnessJ;
		YBar = rData.YBar;
		ZBar = rData.ZBar;
		YBarJ = rData.YBarJ;
		ZBarJ = rData.ZBarJ;
		dH = rData.dH;
		dVS = rData.dVS;
		dM = 0.0;
	}

	T_CSCS_BASE(){}
	T_CSCS_BASE(const T_CSCS_BASE& src) { *this = src; }
	T_CSCS_BASE& operator=(const T_CSCS_BASE& rData)
	{
		nPart         = rData.nPart;
		MatlKey       = rData.MatlKey;
		CompStagKey   = rData.CompStagKey;
		dAge          = rData.dAge;
		StiffScale    = rData.StiffScale;
		Stiffness     = rData.Stiffness;
		StiffnessJ    = rData.StiffnessJ;
		YBar          = rData.YBar;
		ZBar          = rData.ZBar;
		YBarJ         = rData.YBarJ;
		ZBarJ         = rData.ZBarJ;
		dH            = rData.dH;
		dVS           = rData.dVS;
		dM            = rData.dM;
		return *this;
	}  
};

struct T_CSCS_D_600
{
	UINT  SectKey;
	UINT  ActStagKey;
	int   nCompType;  // -2:Normal(0, 1제외) -1:User Type, 0: 1&2 + 3, 1: 1 + 2&3,
	CArray<T_CSCS_BASE_600, T_CSCS_BASE_600&> aPart;
	void Initialize()
	{
		SectKey = 0;
		ActStagKey = 0;
		nCompType = 0;
		aPart.RemoveAll();
	}
};
struct T_CSCS_D_721
{
	UINT  SectKey;
	UINT  ActStagKey;
	int   nCompType;  // -2:Normal(0, 1제외) -1:User Type, 0: 1&2 + 3, 1: 1 + 2&3,
	CArray<T_CSCS_BASE_721, T_CSCS_BASE_721&> aPart;
	void Initialize()
	{
		SectKey = 0;
		ActStagKey = 0;
		nCompType = 0;
		aPart.RemoveAll();
	}
};
struct T_CSCS_D_815
{
	UINT  SectKey;
	UINT  ActStagKey;
	int   nCompType;  // -2:Normal(0, 1제외) -1:User Type, 0: 1&2 + 3, 1: 1 + 2&3,
	CArray<T_CSCS_BASE_815, T_CSCS_BASE_815&> aPart;
	void Initialize()
	{
		SectKey = 0;
		ActStagKey = 0;
		nCompType = 0;
		aPart.RemoveAll();
	}
};
struct T_CSCS_D_842
{
	UINT  SectKey;
	UINT  ActStagKey;
	int   nCompType;  // -2:Normal(0, 1제외) -1:User Type, 0: 1&2 + 3, 1: 1 + 2&3,
	CArray<T_CSCS_BASE_842, T_CSCS_BASE_842&> aPart;
	void Initialize()
	{
		SectKey     = 0;
		ActStagKey  = 0;
		nCompType   = 0;
		aPart.RemoveAll();
	}
};
struct T_CSCS_D_905
{
	UINT  SectKey;
	UINT  ActStagKey;
	int   nCompType;  // -2:Normal(0, 1제외) -1:User Type, 0: 1&2 + 3, 1: 1 + 2&3,
	CArray<T_CSCS_BASE_905, T_CSCS_BASE_905&> aPart;
	void Initialize()
	{
		SectKey = 0;
		ActStagKey = 0;
		nCompType = 0;
		aPart.RemoveAll();
	}
};

struct T_CSCS_D
{
	UINT  SectKey;
	UINT  ActStagKey;
	int   nCompType;  // -2:Normal(0, 1제외) -1:User Type, 0: 1&2 + 3, 1: 1 + 2&3,
	CArray<T_CSCS_BASE, T_CSCS_BASE&> aPart;
	T_CSCS_D(){}
	T_CSCS_D(const T_CSCS_D& src) { *this = src; }
	T_CSCS_D& operator=(const T_CSCS_D& rData)
	{
		SectKey = rData.SectKey;
		ActStagKey = rData.ActStagKey;
		nCompType = rData.nCompType;
		aPart.Copy(rData.aPart);
		return *this;
	}
	void Initialize()
	{
		SectKey = 0;
		ActStagKey = 0;
		nCompType = 0;
		aPart.RemoveAll();
	}
	void Convert600(T_CSCS_D_600& rData)
	{
		SectKey = rData.SectKey;
		ActStagKey = rData.ActStagKey;
		nCompType = rData.nCompType;
		aPart.RemoveAll();
		for(int i = 0; i < rData.aPart.GetSize(); i++)
		{
			T_CSCS_BASE CscsBase;
			CscsBase.Convert600(rData.aPart[i]);
			aPart.Add(CscsBase);
		}    
	}
	void Convert721(T_CSCS_D_721& rData)
	{
		SectKey = rData.SectKey;
		ActStagKey = rData.ActStagKey;
		nCompType = rData.nCompType;
		aPart.RemoveAll();
		for(int i = 0; i < rData.aPart.GetSize(); i++)
		{
			T_CSCS_BASE CscsBase;
			CscsBase.Convert721(rData.aPart[i]);
			aPart.Add(CscsBase);
		}    
	}
	void Convert815(T_CSCS_D_815& rData)
	{
		SectKey = rData.SectKey;
		ActStagKey = rData.ActStagKey;
		nCompType = rData.nCompType;
		aPart.RemoveAll();
		for(int i = 0; i < rData.aPart.GetSize(); i++)
		{
			T_CSCS_BASE CscsBase;
			CscsBase.Convert815(rData.aPart[i]);
			aPart.Add(CscsBase);
		}    
	}
	void Convert842(T_CSCS_D_842& rData)
	{
		SectKey = rData.SectKey;
		ActStagKey = rData.ActStagKey;
		nCompType = rData.nCompType;
		aPart.RemoveAll();
		for(int i = 0; i < rData.aPart.GetSize(); i++)
		{
			T_CSCS_BASE CscsBase;
			CscsBase.Convert842(rData.aPart[i]);
			aPart.Add(CscsBase);
		}    
	}
	void Convert905(T_CSCS_D_905& rData)
	{
		SectKey = rData.SectKey;
		ActStagKey = rData.ActStagKey;
		nCompType = rData.nCompType;
		aPart.RemoveAll();
		for (int i = 0; i < rData.aPart.GetSize(); i++)
		{
			T_CSCS_BASE CscsBase;
			CscsBase.Convert905(rData.aPart[i]);
			aPart.Add(CscsBase);
		}
	}
};

struct T_CSCS_UDRD_D_600
{
	T_CSCS_K key;
	T_CSCS_D_600 data;
};
struct T_CSCS_UDRD_D
{
	T_CSCS_K key;
	T_CSCS_D data;
};
struct T_CSCS_UDRD_D_RW_600
{
	T_CSCS_K key;
	UINT  SectKey;
	UINT  ActStagKey;
	int   nCompType;
	int   nCountPart;
};
struct T_CSCS_UDRD_D_RW_721
{
	T_CSCS_K key;
	UINT  SectKey;
	UINT  ActStagKey;
	int   nCompType;
	int   nCountPart;
	int   nTemp;
};
struct T_CSCS_UDRD_D_RW
{
	T_CSCS_K key;
	UINT  SectKey;
	UINT  ActStagKey;
	int   nCompType;
	int   nCountPart;
	int   nTemp;
	int   nTemp2; // 721 과 구분
	void GetCscs(T_CSCS_K& rKey, T_CSCS_D& rData)
	{
		rKey = key;
		rData.SectKey = SectKey;
		rData.ActStagKey = ActStagKey;
		rData.nCompType = nCompType;
	}
	void SetCscs(const T_CSCS_K Key, const T_CSCS_D& rData)
	{
		key = Key;
		SectKey = rData.SectKey;
		ActStagKey = rData.ActStagKey;
		nCompType = rData.nCompType;
		nCountPart = (int)rData.aPart.GetSize();
	}
	void Convert600(T_CSCS_UDRD_D_RW_600& rData)
	{
		key = rData.key;
		SectKey = rData.SectKey;
		ActStagKey = rData.ActStagKey;
		nCompType = rData.nCompType;
		nCountPart = rData.nCountPart; 
		nTemp = 0;
		nTemp2 = 0;
	}
	void Convert721(T_CSCS_UDRD_D_RW_721& rData)
	{
		key = rData.key;
		SectKey = rData.SectKey;
		ActStagKey = rData.ActStagKey;
		nCompType = rData.nCompType;
		nCountPart = rData.nCountPart; 
		nTemp = 0;
		nTemp2 = 0;
	}
};
struct T_CSCS_UNIT
{
	T_CSCS_UNIT()
	{
		dAge  = D_UNITSYS_BASE_DAY;
		YBar  = D_UNITSYS_BASE_LENGTH;
		ZBar  = D_UNITSYS_BASE_LENGTH;
		dH    = D_UNITSYS_BASE_LENGTH;
		dVS   = D_UNITSYS_BASE_LENGTH;
		dM    = D_UNITSYS_BASE_PERLENGTH;
	}

	int dAge;
	int YBar;
	int ZBar;
	int dH;
	int dVS;
	int dM;

	T_SECT_STIFFNESS_UNIT Stiffness;
};
#define HASHSIZECSCS HASHSIZESECT


///
#define T_CSCV_K unsigned int
struct T_CSCV_D
{
	UINT  SectKey;
	UINT  ActStagKey;
	int   nCompType;  // -2:Normal(0, 1제외) -1:User Type, 0: 1&2 + 3, 1: 1 + 2&3,
	CArray<T_CSCS_BASE, T_CSCS_BASE&> aPart;

	T_CSCV_D() { Initialize(); }
	T_CSCV_D(const T_CSCV_D& src) { *this = src; }
	T_CSCV_D& operator=(const T_CSCV_D& rData)
	{
		SectKey = rData.SectKey;
		ActStagKey = rData.ActStagKey;
		nCompType = rData.nCompType;
		aPart.Copy(rData.aPart);
		return *this;
	}
	void Initialize()
	{
		SectKey = 0;
		ActStagKey = 0;
		nCompType = 0;
		aPart.RemoveAll();
	}

	void ConvertToCscs(T_CSCS_D& rData)
	{
		rData.Initialize();
		rData.SectKey    = SectKey;
		rData.ActStagKey = ActStagKey;
		rData.nCompType  = nCompType;
		rData.aPart.Copy(aPart);
	}
	void ConvertToCscv(T_CSCS_D& rData)
	{
		Initialize();
		SectKey = rData.SectKey;
		ActStagKey = rData.ActStagKey;
		nCompType = rData.nCompType;
		aPart.Copy(rData.aPart);
	}
};

struct T_CSCV_UDRD_D
{
	T_CSCV_K key;
	T_CSCV_D data;
};

struct T_CSCV_UNIT
{
	T_CSCV_UNIT()
	{
	}
};
#define HASHSIZECSCV HASHSIZESECT

// Reinforcement of Section
#define T_RPSC_K T_SECT_K

struct T_RPSC_MBAR_712
{
	CString strBarNa;
	int     iBarNum;
	int     iStaYRef;
	double  dStaY;
	int     iStaZRef;
	double  dStaZ;
	double  dPitch;
};
struct T_RPSC_MBAR  // 교축 방향 철근
{
	CString strBarNa;   // 철근 이름
	int     iBarNum;    // 배치 갯수
	int     iStaYRef;   // Y의 배치 기준(0=도심, 1=단면의 맨 왼쪽)
	double  dStaY;      // 배치 위치(Y) : +는 배치 기준점으로 부터 오른쪽, -는 왼쪽 
	int     iStaZRef;   // Z의 배치 기준(0=Top, 1=Bottom : 단면 MBR의 Top, Bottom)
	double  dStaZ;      // 배치 위치(Z) // 양수, 상대 거리(iStaZRef)
	double  dPitch;     // 간격

	int     nPart;      // v720추가, 0:PSC단면 1:Composite의 Part1, 2:Composite의 Part2
	void Initialize()
	{
		strBarNa    = "";
		iBarNum     = 0;
		iStaYRef    = 0;
		dStaY       = 0.0;
		iStaZRef    = 0;
		dStaZ       = 0.0;
		dPitch      = 0.0;
		nPart       = 0;
	}
	void Convert712(T_RPSC_MBAR_712& rData)
	{
		strBarNa = rData.strBarNa;
		iBarNum  = rData.iBarNum;
		iStaYRef = rData.iStaYRef;
		dStaY    = rData.dStaY;
		iStaZRef = rData.iStaZRef;
		dStaZ    = rData.dStaZ;
		dPitch   = rData.dPitch;
		nPart    = 0;
	}
};

struct T_RPSC_MBAR_CH_712
{
	char    strBarNa[8];
	int     iBarNum;
	int     iStaYRef;
	double  dStaY;
	int     iStaZRef;
	double  dStaZ;
	double  dPitch;
};
struct T_RPSC_MBAR_CH
{
	char    strBarNa[8];
	int     iBarNum;
	int     iStaYRef;
	double  dStaY;
	int     iStaZRef;
	double  dStaZ;
	double  dPitch;
	int     nPart;
	void ConvertToString(T_RPSC_MBAR& rData)
	{
		ConvertCharStr(strBarNa, rData.strBarNa, sizeof(strBarNa));
		rData.iBarNum     = iBarNum;
		rData.iStaYRef    = iStaYRef;
		rData.dStaY       = dStaY;
		rData.iStaZRef    = iStaZRef;
		rData.dStaZ       = dStaZ;
		rData.dPitch      = dPitch;
		rData.nPart       = nPart;
	}
	void ConvertToChar(T_RPSC_MBAR& rData)
	{
		ConvertStrChar(rData.strBarNa, strBarNa, sizeof(strBarNa));
		iBarNum     = rData.iBarNum;
		iStaYRef    = rData.iStaYRef;
		dStaY       = rData.dStaY;
		iStaZRef    = rData.iStaZRef;
		dStaZ       = rData.dStaZ;
		dPitch      = rData.dPitch;
		nPart       = rData.nPart;
	}
	void Convert712(T_RPSC_MBAR_CH_712& rData)
	{
		memcpy(strBarNa, rData.strBarNa, sizeof(strBarNa));
		iBarNum     = rData.iBarNum;
		iStaYRef    = rData.iStaYRef;
		dStaY       = rData.dStaY;
		iStaZRef    = rData.iStaZRef;
		dStaZ       = rData.dStaZ;
		dPitch      = rData.dPitch;
		nPart       = 0;
	}
};

struct T_RPSC_SBAR_660      // 전단보강 철근
{
	BOOL   bSbarExist;    // 경사인장철근 정보 있으면 TRUE
	double dSbarPitch;    // 경사인장철근의 간격
	double dSbarTheta;    // 경사인장철근의 각도
	double dSbarAw;       // 경사인장철근의 배치량
	BOOL   bTndnExist;    // 전단강봉 정보 있으면 TRUE
	double dTndnDist;     // 전단강봉의 간격
	double dTndnTheta;    // 전단강봉의 각도
	double dTndnAp;       // 전단강봉의 배치량
	double dTndnPs;       // 전단강봉의 유효인장력
	BOOL   bLbarExist;    // 횡방향철근 정보 있으면 TRUE
	double dLbarPitch;    // 횡방향철근의 간격
	double dLbarAwt;      // 횡방향철근의 Pitch당 철근 1개의 단면적
	double dLbarAlt;      // 횡뱡향철근의 전단면적
};
struct T_RPSC_SBAR_691      // 전단보강 철근
{
	BOOL   bSbarExist;    // 경사인장철근 정보 있으면 TRUE
	double dSbarPitch;    // 경사인장철근의 간격
	double dSbarTheta;    // 경사인장철근의 각도
	double dSbarAw;       // 경사인장철근의 배치량
	BOOL   bTndnExist;    // 전단강봉 정보 있으면 TRUE
	double dTndnDist;     // 전단강봉의 간격
	double dTndnTheta;    // 전단강봉의 각도
	double dTndnAp;       // 전단강봉의 배치량
	double dTndnPs;       // 전단강봉의 유효인장력
	BOOL   bLbarExist;    // 횡방향철근 정보 있으면 TRUE
	double dLbarPitch;    // 횡방향철근의 간격
	double dLbarAwt;      // 횡방향철근의 Pitch당 철근 1개의 단면적
	double dLbarAlt;      // 횡뱡향철근의 전단면적
	BOOL   bStirExist;    // 6.6.0 추가: 전단철근 정보 있으면 TRUE
	double dStirPitch;    // 6.6.0 추가: 전단철근의 간격
	double dStirAw;       // 6.6.0 추가: 전단철근의 배치량
	double dTndnFactor;   // 6.6.0 추가: 전단강봉에 의한 발생수직응력저감계수 (JTG-D62-04 6.3.3-3)
												// Default (CH=0.6, Etc.=1.0)
};


struct T_RPSC_SBAR_925      // 전단보강 철근
{
	BOOL   bSbarExist;    // 경사인장철근 정보 있으면 TRUE
	double dSbarPitch;    // 경사인장철근의 간격
	double dSbarTheta;    // 경사인장철근의 각도
	double dSbarAw;       // 경사인장철근의 배치량
	BOOL   bTndnExist;    // 전단강봉 정보 있으면 TRUE
	double dTndnDist;     // 전단강봉의 간격
	double dTndnTheta;    // 전단강봉의 각도
	double dTndnAp;       // 전단강봉의 배치량
	double dTndnPs;       // 전단강봉의 유효인장력
	BOOL   bLbarExist;    // 횡방향철근 정보 있으면 TRUE
	double dLbarPitch;    // 횡방향철근의 간격
	double dLbarAwt;      // 횡방향철근의 Pitch당 철근 1개의 단면적
	double dLbarAlt;      // 횡뱡향철근의 전단면적
	BOOL   bStirExist;    // 6.6.0 추가: 전단철근 정보 있으면 TRUE
	double dStirPitch;    // 6.6.0 추가: 전단철근의 간격
	double dStirAw;       // 6.6.0 추가: 전단철근의 배치량
	double dTndnFactor;   // 6.6.0 추가: 전단강봉에 의한 발생수직응력저감계수 (JTG-D62-04 6.3.3-3)
												// Default (CH=0.6, Etc.=1.0)
	BOOL bLbarFlg;      // 6.9.2 추가: Acor계산시 플랜지 부분을 스트럽으로 감는것을 고려하는 변수
	double dLbarThick;  // 6.9.2 추가: 비틀림 스트럽에서 연단까지의 거리(피복두께)
	BOOL bIncFC;        // 6.9.2 추가: Include Flange/Cantilever
};

struct T_RPSC_SBAR      // 전단보강 철근
{
	BOOL   bSbarExist;    // 경사인장철근 정보 있으면 TRUE
	double dSbarPitch;    // 경사인장철근의 간격
	double dSbarTheta;    // 경사인장철근의 각도
	double dSbarAw;       // 경사인장철근의 배치량
	BOOL   bTndnExist;    // 전단강봉 정보 있으면 TRUE
	double dTndnDist;     // 전단강봉의 간격
	double dTndnTheta;    // 전단강봉의 각도
	double dTndnAp;       // 전단강봉의 배치량
	double dTndnPs;       // 전단강봉의 유효인장력
	BOOL   bLbarExist;    // 횡방향철근 정보 있으면 TRUE
	double dLbarPitch;    // 횡방향철근의 간격
	double dLbarAwt;      // 횡방향철근의 Pitch당 철근 1개의 단면적
	double dLbarAlt;      // 횡뱡향철근의 전단면적
	BOOL   bStirExist;    // 6.6.0 추가: 전단철근 정보 있으면 TRUE
	double dStirPitch;    // 6.6.0 추가: 전단철근의 간격
	double dStirAw;       // 6.6.0 추가: 전단철근의 배치량
	double dTndnFactor;   // 6.6.0 추가: 전단강봉에 의한 발생수직응력저감계수 (JTG-D62-04 6.3.3-3)
												// Default (CH=0.6, Etc.=1.0)
	BOOL bLbarFlg;      // 6.9.2 추가: Acor계산시 플랜지 부분을 스트럽으로 감는것을 고려하는 변수
	double dLbarThick;  // 6.9.2 추가: 비틀림 스트럽에서 연단까지의 거리(피복두께)
	BOOL bIncFC;        // 6.9.2 추가: Include Flange/Cantilever
		
	 // T_RPSC_SBAR_SSM의 변환용으로 추가됨. 
	int nAutoUser;       // (v930) 0:Auto 1:User 	
	double dAk;          // (v930) User - 폐합단면
	double dUk;          // (v930) User - 폐합단면둘레
	double dAkCover;     // (v930) User - 폐합단면 + cover 고려
	double dUkCover;     // (v930) User - 폐합단면둘레 + cover 고려

	void Initialize()
	{
		bSbarExist = FALSE;
		dSbarPitch = 0.0;
		dSbarTheta = 0.0;
		dSbarAw    = 0.0;
		bTndnExist = FALSE;
		dTndnDist  = 0.0;
		dTndnTheta = 90.0;
		dTndnAp    = 0.0;
		dTndnPs    = 0.0;
		bLbarExist = FALSE;
		dLbarPitch = 0.0;
		dLbarAwt   = 0.0;
		dLbarAlt   = 0.0;
		bStirExist = FALSE;
		dStirPitch = 0.0;
		dStirAw    = 0.0;
		#if defined(_CH)
		dTndnFactor= 0.6;
		#else
		dTndnFactor= 1.0;
		#endif
		bLbarFlg = FALSE;
		dLbarThick = 0.0;
		bIncFC = FALSE;

		nAutoUser = 0;
		dAk = 0.0;
		dUk = 0.0;
		dAkCover = 0.0;
		dUkCover = 0.0;
	}
	void Convert660(T_RPSC_SBAR_660& rData)
	{
		bSbarExist = rData.bSbarExist;
		dSbarPitch = rData.dSbarPitch;
		dSbarTheta = rData.dSbarTheta;
		dSbarAw    = rData.dSbarAw;
		bTndnExist = rData.bTndnExist;
		dTndnDist  = rData.dTndnDist;
		dTndnTheta = rData.dTndnTheta;
		dTndnAp    = rData.dTndnAp;
		dTndnPs    = rData.dTndnPs;
		bLbarExist = rData.bLbarExist;
		dLbarPitch = rData.dLbarPitch;
		dLbarAwt   = rData.dLbarAwt;
		dLbarAlt   = rData.dLbarAlt;
		bStirExist = FALSE;
		dStirPitch = 0.0;
		dStirAw    = 0.0;
		dTndnFactor= 1.0;
		bLbarFlg = FALSE;
		dLbarThick = 0.0;
		bIncFC = FALSE;

		nAutoUser = 0;
		dAk = 0.0;
		dUk = 0.0;
		dAkCover = 0.0;
		dUkCover = 0.0;
	}
	void Convert691(T_RPSC_SBAR_691& rData)
	{
		bSbarExist = rData.bSbarExist;
		dSbarPitch = rData.dSbarPitch;
		dSbarTheta = rData.dSbarTheta;
		dSbarAw    = rData.dSbarAw;
		bTndnExist = rData.bTndnExist;
		dTndnDist  = rData.dTndnDist;
		dTndnTheta = rData.dTndnTheta;
		dTndnAp    = rData.dTndnAp;
		dTndnPs    = rData.dTndnPs;
		bLbarExist = rData.bLbarExist;
		dLbarPitch = rData.dLbarPitch;
		dLbarAwt   = rData.dLbarAwt;
		dLbarAlt   = rData.dLbarAlt;
		bStirExist = rData.bStirExist;
		dStirPitch = rData.dStirPitch;
		dStirAw    = rData.dStirAw;
		dTndnFactor= rData.dTndnFactor;
		bLbarFlg = FALSE;
		dLbarThick = 0.0;
		bIncFC = FALSE;

		nAutoUser = 0;
		dAk = 0.0;
		dUk = 0.0;
		dAkCover = 0.0;
		dUkCover = 0.0;
	}
	void Convert925(T_RPSC_SBAR_925& rData)
	{
		bSbarExist = rData.bSbarExist;
		dSbarPitch = rData.dSbarPitch;
		dSbarTheta = rData.dSbarTheta;
		dSbarAw    = rData.dSbarAw;
		bTndnExist = rData.bTndnExist;
		dTndnDist  = rData.dTndnDist;
		dTndnTheta = rData.dTndnTheta;
		dTndnAp    = rData.dTndnAp;
		dTndnPs    = rData.dTndnPs;
		bLbarExist = rData.bLbarExist;
		dLbarPitch = rData.dLbarPitch;
		dLbarAwt   = rData.dLbarAwt;
		dLbarAlt   = rData.dLbarAlt;
		bStirExist = rData.bStirExist;
		dStirPitch = rData.dStirPitch;
		dStirAw    = rData.dStirAw;
		dTndnFactor= rData.dTndnFactor;
		bLbarFlg   = rData.bLbarFlg   ;
		dLbarThick = rData.dLbarThick ;
		bIncFC     = rData.bIncFC     ;

		nAutoUser = 0;
		dAk = 0.0;
		dUk = 0.0;
		dAkCover = 0.0;
		dUkCover = 0.0;
	}
};

struct T_RPSC_D
{
	BOOL bMbarJ; // TRUE=교축방향 철근을 I, J 각각 입력할 경우
	BOOL bSbarJ; // TRUE=전당보강 철근을 I, J 각각 입력할 경우
	CArray<T_RPSC_MBAR, T_RPSC_MBAR&> Mbar[2]; // 교축방향 철근(I, J)
	T_RPSC_SBAR Sbar[2];  // 전단보강 철근(I, J)
	BOOL bCracked; // v720 추가, composite section에서 사용

	T_RPSC_D() {}
	T_RPSC_D(const T_RPSC_D& rData) { *this = rData; }
	T_RPSC_D& operator=(const T_RPSC_D& rData)
	{
		bMbarJ = rData.bMbarJ;
		bSbarJ = rData.bSbarJ;
		Mbar[0].Copy(rData.Mbar[0]);
		Mbar[1].Copy(rData.Mbar[1]);
		Sbar[0] = rData.Sbar[0];
		Sbar[1] = rData.Sbar[1];
		bCracked = rData.bCracked;
		return *this;
	}
	void Initialize()
	{
		bMbarJ = bSbarJ = FALSE;
		Mbar[0].RemoveAll();
		Mbar[1].RemoveAll();
		Sbar[0].Initialize();
		Sbar[1].Initialize();
		bCracked = FALSE;
	}
};
struct T_RPSC_D_CH_712
{
	BOOL bMbarJ; // TRUE=교축방향 철근을 I, J 각각 입력할 경우
	BOOL bSbarJ; // TRUE=전당보강 철근을 I, J 각각 입력할 경우
	CArray<T_RPSC_MBAR_CH_712, T_RPSC_MBAR_CH_712&> Mbar[2]; // 교축방향 철근(I, J)
	T_RPSC_SBAR Sbar[2];  // 전단보강 철근(I, J)
};
struct T_RPSC_D_CH
{
	BOOL bMbarJ; // TRUE=교축방향 철근을 I, J 각각 입력할 경우
	BOOL bSbarJ; // TRUE=전당보강 철근을 I, J 각각 입력할 경우
	CArray<T_RPSC_MBAR_CH, T_RPSC_MBAR_CH&> Mbar[2]; // 교축방향 철근(I, J)
	T_RPSC_SBAR Sbar[2];  // 전단보강 철근(I, J)
	BOOL bCracked;
	void ConvertToString(T_RPSC_D& rData)
	{
		rData.bMbarJ = bMbarJ;
		rData.bSbarJ = bSbarJ;
		int nCount = (int)Mbar[0].GetSize();
		rData.Mbar[0].SetSize(nCount);
		for (int i = 0; i < nCount; i++) Mbar[0][i].ConvertToString(rData.Mbar[0][i]);
		nCount     = (int)Mbar[1].GetSize();    
		rData.Mbar[1].SetSize(nCount);
		for (int i = 0; i < nCount; i++) Mbar[1][i].ConvertToString(rData.Mbar[1][i]);
		rData.Sbar[0] = Sbar[0];
		rData.Sbar[1] = Sbar[1];
		rData.bCracked = bCracked;
	}
	void ConvertToChar(T_RPSC_D& rData)
	{
		bMbarJ = rData.bMbarJ;
		bSbarJ = rData.bSbarJ;
		int nCount = (int)rData.Mbar[0].GetSize();
		Mbar[0].SetSize(nCount);
		for (int i = 0; i < nCount; i++) Mbar[0][i].ConvertToChar(rData.Mbar[0][i]);
		nCount     = (int)rData.Mbar[1].GetSize();    
		Mbar[1].SetSize(nCount);
		for (int i = 0; i < nCount; i++) Mbar[1][i].ConvertToChar(rData.Mbar[1][i]);
		Sbar[0] = rData.Sbar[0];
		Sbar[1] = rData.Sbar[1];
		bCracked = rData.bCracked;
	}
};
struct T_RPSC_UDRD_D
{
	T_RPSC_K key;
	T_RPSC_D data;
};
struct T_RPSC_UDRD_D_CH
{
	T_RPSC_K key;
	T_RPSC_D_CH data;
};
struct T_RPSC_UDRD_D_CH_RW_660
{
	T_RPSC_K key;
	BOOL bMbarJ;
	BOOL bSbarJ;
	unsigned int nCountMbar[2];
	T_RPSC_SBAR_660 Sbar[2];
};
struct T_RPSC_UDRD_D_CH_RW_691
{
	T_RPSC_K key;
	BOOL bMbarJ;
	BOOL bSbarJ;
	unsigned int nCountMbar[2];
	T_RPSC_SBAR_691 Sbar[2];
};
struct T_RPSC_UDRD_D_CH_RW_712
{
	T_RPSC_K key;
	BOOL bMbarJ;
	BOOL bSbarJ;
	unsigned int nCountMbar[2];
	T_RPSC_SBAR_925 Sbar[2];
};
struct T_RPSC_UDRD_D_CH_RW_925
{
	T_RPSC_K key;
	BOOL bMbarJ;
	BOOL bSbarJ;
	unsigned int nCountMbar[2];
	T_RPSC_SBAR_925 Sbar[2];
	BOOL bCracked;
};
struct T_RPSC_UDRD_D_CH_RW
{
	T_RPSC_K key;
	BOOL bMbarJ;
	BOOL bSbarJ;
	unsigned int nCountMbar[2];
	T_RPSC_SBAR Sbar[2];
	BOOL bCracked;
	void GetRpsc(T_RPSC_K& rKey, T_RPSC_D_CH& rData)
	{
		rKey = key;
		rData.bMbarJ  = bMbarJ;
		rData.bSbarJ  = bSbarJ;
		rData.Sbar[0] = Sbar[0];
		rData.Sbar[1] = Sbar[1];
		rData.bCracked = bCracked;
	}
	void SetRpsc(const T_RPSC_K Key, const T_RPSC_D_CH& rData)
	{
		key = Key;
		bMbarJ  = rData.bMbarJ;
		bSbarJ  = rData.bSbarJ;
		nCountMbar[0] = (int)rData.Mbar[0].GetSize();
		nCountMbar[1] = (int)rData.Mbar[1].GetSize();
		Sbar[0] = rData.Sbar[0];
		Sbar[1] = rData.Sbar[1];
		bCracked = rData.bCracked;
	}
	void Convert660(T_RPSC_UDRD_D_CH_RW_660& rData)
	{
		key = rData.key;
		bMbarJ  = rData.bMbarJ;
		bSbarJ  = rData.bSbarJ;
		nCountMbar[0] = rData.nCountMbar[0];
		nCountMbar[1] = rData.nCountMbar[1];
		Sbar[0].Convert660(rData.Sbar[0]);
		Sbar[1].Convert660(rData.Sbar[1]);
		bCracked = FALSE;
	}
	void Convert691(T_RPSC_UDRD_D_CH_RW_691& rData)
	{
		key = rData.key;
		bMbarJ  = rData.bMbarJ;
		bSbarJ  = rData.bSbarJ;
		nCountMbar[0] = rData.nCountMbar[0];
		nCountMbar[1] = rData.nCountMbar[1];
		Sbar[0].Convert691(rData.Sbar[0]);
		Sbar[1].Convert691(rData.Sbar[1]);
		bCracked = FALSE;
	}
	void Convert712(T_RPSC_UDRD_D_CH_RW_712& rData)
	{
		key = rData.key;
		bMbarJ  = rData.bMbarJ;
		bSbarJ  = rData.bSbarJ;
		nCountMbar[0] = rData.nCountMbar[0];
		nCountMbar[1] = rData.nCountMbar[1];
		Sbar[0].Convert925(rData.Sbar[0]);
		Sbar[1].Convert925(rData.Sbar[1]);
		bCracked = FALSE;
	}
	void Convert925(T_RPSC_UDRD_D_CH_RW_925& rData)
	{
		key = rData.key;
		bMbarJ = rData.bMbarJ;
		bSbarJ = rData.bSbarJ;
		nCountMbar[0] = rData.nCountMbar[0];
		nCountMbar[1] = rData.nCountMbar[1];
		Sbar[0].Convert925(rData.Sbar[0]);
		Sbar[1].Convert925(rData.Sbar[1]);
		bCracked = rData.bCracked;
	}
};

struct T_RPSC_UNIT
{
	T_RPSC_UNIT()
	{
		dStaY       = D_UNITSYS_BASE_LENGTH;
		dStaZ       = D_UNITSYS_BASE_LENGTH;
		dPitch      = D_UNITSYS_BASE_LENGTH;
		dSbarPitch  = D_UNITSYS_BASE_LENGTH;
		dSbarTheta  = D_UNITSYS_BASE_DEGREE;
		dSbarAw     = D_UNITSYS_BASE_AREA;
		dTndnDist   = D_UNITSYS_BASE_LENGTH;
		dTndnTheta  = D_UNITSYS_BASE_DEGREE;
		dTndnAp     = D_UNITSYS_BASE_AREA;
		dTndnPs     = D_UNITSYS_BASE_FORCE;
		dLbarPitch  = D_UNITSYS_BASE_LENGTH;
		dLbarAwt    = D_UNITSYS_BASE_AREA;
		dLbarAlt    = D_UNITSYS_BASE_AREA;
		dStirPitch  = D_UNITSYS_BASE_LENGTH;
		dStirAw     = D_UNITSYS_BASE_AREA;
		dLbarThick  = D_UNITSYS_BASE_LENGTH;
	}
	int dStaY;
	int dStaZ;
	int dPitch;
	int dSbarPitch;
	int dSbarTheta;
	int dSbarAw;
	int dTndnDist;
	int dTndnTheta;
	int dTndnAp;
	int dTndnPs;
	int dLbarPitch;
	int dLbarAwt;
	int dLbarAlt;
	int dStirPitch;
	int dStirAw;
	int dLbarThick;
};
#define HASHSIZERPSC HASHSIZESECT

// Add by ZINU.('04.03.10). For PSC Design at CRC.
struct T_SPSC_COOR
{
	CArray<double,double> arSpscPont[2];	// 0=y, 1=z.
	void Initialize()
	{
		arSpscPont[0].RemoveAll();
		arSpscPont[1].RemoveAll();
	}
	T_SPSC_COOR() {}
	T_SPSC_COOR(T_SPSC_COOR& rData)	{*this = rData;}
	T_SPSC_COOR& operator = (const T_SPSC_COOR& rData)
	{
		arSpscPont[0].Copy(rData.arSpscPont[0]);
		arSpscPont[1].Copy(rData.arSpscPont[1]);
		return *this;
	}
};

struct T_SPSC_DSHR
{
	// 0=Top(Web), 1=Center, 2=Bot(Web).
	double dQ[3];				// 단면1차 모멘트
	double db[3];				// 전체웨브두께(거더)
	double db_sl[3];		// 웨브두께(슬래브, psc 합성일 때)
	double dyz[2][6];		// 전단검토위치 1st(0=y,1=z), 2nd(TL,TR,CL,CR,BL,BR)
	double dJt;					// 비틀림 강성 (폐단면인 경우만 계산)
	double dWt;					// 비틀림재하능력 계산시 사용 하는 계수 (JTG D62-04)
	double dKt;			    // 비틀림에 의한 전단응력도에 관한 계수, 
	double dKtweb[6];   // 795에서 추가, Auto일 경우 비틀림에 대한 전단응력을 전단위치의 두께로..
	double dKtThik;			// 외측웨브의 최소두께
	double dyzKt[2][6];	// Kt계산을 위한 폐단면의 좌표 (CCW from TL).
	double dQyb, dQzb;	// 콘크리트만에 의한 단면1차모멘트를 두께로 나눈 값

	// 7Dof 해석 데이터 생성(6 Point) v850
	double dQ_7Dof[6];				// 단면1차 모멘트
	double db_7Dof[6];				// 전체웨브두께
	double dyz_7Dof[2][6];		// 전단검토위치 1st(0=y,1=z), 2nd(6 Point)

	void Initialize()
	{
		for(int i=0; i<3; i++)
		{
			dQ[i]=0.0;
			db[i]=0.0;
			db_sl[i]=0.0;
			dyz[0][i+0]=0.0; dyz[1][i+0]=0.0;
			dyz[0][i+3]=0.0; dyz[1][i+3]=0.0;
			dyzKt[0][i+0]=0.0; dyzKt[1][i+0]=0.0;
			dyzKt[0][i+3]=0.0; dyzKt[1][i+3]=0.0;
		}

		for(int i=0; i<6; i++)
		{
			dQ_7Dof[i]=0.0;
			db_7Dof[i]=0.0;
			dyz_7Dof[0][i]=0.0;  // y
			dyz_7Dof[1][i]=0.0;  // z
		}

		dJt=0.0;
		dWt=0.0;
		dKt=0.0;
		memset(dKtweb,0,sizeof(dKtweb));
		dKtThik=0.0;
		dQyb=0.0; dQzb=0.0;
	}
};

struct T_SPSC_RBAR
{
	double dAs;			// 전체철근면적
	double dyz[2];	// 콘크리트 단면에 대한 도심(0=y,1=z)
	double dEs;			// 일반적인 탄성계수
	double dDs;			// 일반적인 단위중량
	void Initialize()
	{
		dAs=0.0;
		dyz[0]=0.0; dyz[1]=0.0;
		dEs=0.0;
		dDs=0.0;
	}
};

struct T_SPSC_SECT
{
	// Change by ZINU.('05.03.03). Add Asy,Asz (Agree with 코우노).
	double dyDim, dzDim;	// 단면고
	double dyCen, dzCen;	// 중립축
	double dArea;					// 단면적
	double dAsy, dAsz;		// 전단면적
	double dIxx;					// 비틀림강성
	double dIyy, dIzz;		// 단면2차모멘트
	double dSlt, dSlb;		// 강축좌측단면계수(Top,Bot) //거더 
	double dSrt, dSrb;		// 강축우측단면계수(Top,Bot)
	double dSlt_s, dSlb_s;		// 강축좌측단면계수(Top,Bot) //슬래브
	double dSrt_s, dSrb_s;		// 강축우측단면계수(Top,Bot)
	double dYlt, dYlb;	    	// 강축좌측위치~도심(Top,Bot)    // 거더
	double dYrt, dYrb;		    // 강축우측위치~도심(Top,Bot)
	double dYlt_s, dYlb_s;		// 강축좌측위치~도심(Top,Bot) //슬래브
	double dYrt_s, dYrb_s;		// 강축우측위치~도심(Top,Bot)
	double dGirderWidth;      // 거더 상부 폭(General PSC Composite에만 고려)


	void Initialize()
	{
		dyDim=0.0; dzDim=0.0;
		dyCen=0.0; dzCen=0.0;
		dArea=0.0;
		dAsy=0.0; dAsz=0.0;
		dIxx=0.0;
		dIyy=0.0; dIzz=0.0;
		dSlt=0.0; dSlb=0.0;
		dSrt=0.0; dSrb=0.0;
		dSlt_s=0.0; dSlb_s=0.0;
		dSrt_s=0.0; dSrb_s=0.0;
		dYlt=0.0;   dYlb=0.0;
		dYrt=0.0;   dYrb=0.0;
		dYlt_s=0.0; dYlb_s=0.0;
		dYrt_s=0.0; dYrb_s=0.0;
		dGirderWidth = 0.0;

	}
};

struct T_SPSC_TNDN	// 평균텐던
{
	CString strTndnGrup;	// 텐던그룹명
	double dTotNum;				// 텐던개수
	double dEffNum;				// 유효개수
	double dRatioForStr;	// 무효장을 고려한 응력계산을 위한 계수
	double dEffDia;				// JTG-D62-04 : 유효직경 de=sqrt(n)*d.
	double dyz[2];				// 콘크리트 단면에 대한 도심(0=y,1=z)
	double dEffStress;		// 유효프리스트레스응력
	double dEffStressLL;	// 활하중시 유효프리스트레스응력
	double dStress[3];		// 프리스트레스 유효응력(0=x,1=y,2=z)
	double dStrLoss[3][3];// JTG-D62-04 : 프리스트레스 손실응력(0=x,1=y,2=z)(0=Elastic, 1=Relaxation, 2=Creep/Shrinkage)
	double dEffStressBrld;// For Bridge Load Rating 작용한 Moment에의한 Tendon의 증,감 응력 
	void Initialize()
	{
		strTndnGrup=_T("");
		dTotNum=0.0;
		dEffNum=0.0;
		dRatioForStr=0.0;
		dEffDia=0.0;
		dyz[0]=0.0; dyz[1]=0.0;
		dEffStress=0.0;
		dEffStressLL=0.0;
		dEffStressBrld=0.0;
		for(int i=0; i<3; i++)
		{
			dStress[i]=0.0;
			for(int j=0; j<3; j++)	dStrLoss[i][j]=0.0;
		}
	}
};

struct T_SECT_STIFF_BASE
{
	T_SECT_STIFFNESS Stiffness;
	double dYbar;
	double dZbar;

	void Initialize()
	{
		Stiffness.Initialize();
		dYbar=dZbar=0.0;    
	}
	T_SECT_STIFF_BASE () { Initialize(); }
	T_SECT_STIFF_BASE(const T_SECT_STIFF_BASE& rData) {*this = rData;}
	T_SECT_STIFF_BASE& operator=(const T_SECT_STIFF_BASE& rData)
	{
		Stiffness = rData.Stiffness;
		dYbar = rData.dYbar;
		dZbar = rData.dZbar;

		return *this;
	}
};

struct T_GSEC_RBAR
{
	double dDia;    // 철근지름 
	double dAs;			// 철근면적
	double dyz[2];	// 철근좌표(0=y,1=z), 도심 또는 원점 기준.
	double dEs;			// 일반적인 탄성계수
	double dDs;			// 일반적인 단위중량
	double dPs;     // Poisson
	void Initialize()
	{
		dDia=0.0;
		dAs=0.0;
		dyz[0]=0.0; dyz[1]=0.0;
		dEs=0.0;
		dDs=0.0;
		dPs=0.0;
	}
	T_GSEC_RBAR () { Initialize(); }
};

struct T_GSEC_STRN
{
	double dWidth;    
	double dThikness;
	double dyz[2];	 // 중심좌표(0=y,1=z), 도심 또는 원점 기준.
	double dEs;		 // 일반적인 탄성계수
	double dDs;		 // 일반적인 단위중량
	double dPs;      // Poisson
	void Initialize()
	{
		dWidth = 0.0;
		dThikness = 0.0;
		dyz[0] = 0.0; dyz[1] = 0.0;
		dEs = 0.0;
		dDs = 0.0;
		dPs = 0.0;
	}
	T_GSEC_STRN() { Initialize(); }
};


struct T_GSEC_TNDN
{
	BOOL bPrestressed;
	int nTendonType; // 1:Internal, 2:External
	int nLoadType;   // 0=Pre-tension, 1=Post-tension  
	int nBondType;   // 0: Bonded, 1: Unbonded
	double dDia;     // Tendon지름 
	double dDuctDia; // 덕트지름 
	double dAp;			 // Tendon면적
	double dyz[2];	 // Tendon좌표(0=y,1=z), 도심 또는 원점 기준.
	double dEp;			 // 일반적인 탄성계수
	double dDp;			 // 일반적인 단위중량
	double dPp;      // Poisson
	BOOL bDebonded;
	void Initialize()
	{
		bPrestressed=FALSE;
		nTendonType = 0;
		nLoadType = 0;
		nBondType = 0;
		dDia=0.0;
		dDuctDia=0.0;
		dAp=0.0;
		dyz[0]=0.0; dyz[1]=0.0;
		dEp=0.0;
		dDp=0.0;
		dPp=0.0;
		bDebonded=FALSE;
	}
	T_GSEC_TNDN () { Initialize(); }
};

struct T_SECT_PART_D
{
	T_SECT_STIFF_BASE Stiff;
	double dElast;
	double dPoisson;
	double dDensity;
	CArray<T_GSEC_RBAR,T_GSEC_RBAR&> aRbarData;
	CArray<T_GSEC_TNDN,T_GSEC_TNDN&> aTndnData;
	CArray<T_GSEC_STRN,T_GSEC_STRN&> aStrnData;

	void Initialize()
	{
		Stiff.Initialize();
		dElast=0.0;
		dPoisson=0.0;
		dDensity=0.0;
		//dYbar=dZbar=0.0;
		aRbarData.RemoveAll();
		aTndnData.RemoveAll();
		aStrnData.RemoveAll();
	}
	T_SECT_PART_D () { Initialize(); }
	T_SECT_PART_D(const T_SECT_PART_D& rData) {*this = rData;}
	T_SECT_PART_D& operator=(const T_SECT_PART_D& rData)
	{
		Initialize();
		Stiff    = rData.Stiff;
		dElast   = rData.dElast;
		dPoisson = rData.dPoisson;
		dDensity = rData.dDensity;    
		aRbarData.Copy(rData.aRbarData);
		aTndnData.Copy(rData.aTndnData);
		aStrnData.Copy(rData.aStrnData);

		return *this;
	}
};

struct T_SECT_INFO_D
{
	int nStype;    // D_SECT_TYPE_??
	CString Shape; // D_SECT_SHAPE_??
	int iGirder;   // Composite일 경우 Girder의 수

	T_SECT_STIFF_BASE StiffMainD; // 메인단면의 Stiffness, 합성일 경우는 합성후 메인단면기준 Stiffness
	CArray<int,int&> aStageId;
	CArray<T_SECT_PART_D,T_SECT_PART_D&> aSectPart;
	BOOL bCracked;
	double dConcFact;

	void Initialize()
	{
		nStype = 0;
		Shape.Empty(); 
		iGirder=1;
		StiffMainD.Initialize();
		aStageId.RemoveAll();
		aSectPart.RemoveAll();
		bCracked = FALSE;
		dConcFact = 0.0;
	}
	T_SECT_INFO_D () { Initialize(); }
	T_SECT_INFO_D(const T_SECT_INFO_D& rData) {*this = rData;}
	T_SECT_INFO_D& operator=(const T_SECT_INFO_D& rData)
	{
		Initialize();
		nStype = rData.nStype;
		Shape = rData.Shape;
		iGirder=rData.iGirder;
		StiffMainD = rData.StiffMainD;
		aStageId.Copy(rData.aStageId);
		aSectPart.Copy(rData.aSectPart);
		bCracked = rData.bCracked;
		dConcFact = rData.dConcFact;

		return *this;
	}
};

struct T_SECT_PROP_BASE
{
	T_SECT_STIFF_BASE StiffPure; 
	T_SECT_STIFF_BASE StiffRbar; // Strengthening 포함.
	T_SECT_STIFF_BASE StiffTndn;
	T_SECT_STIFF_BASE StiffTnRb; // Strengthening 포함.

	void Initialize()
	{
		StiffPure.Initialize();
		StiffRbar.Initialize();  
		StiffTndn.Initialize();  
		StiffTnRb.Initialize();  
	}
	T_SECT_PROP_BASE () { Initialize(); }
	T_SECT_PROP_BASE(const T_SECT_PROP_BASE& rData) {*this = rData;}
	T_SECT_PROP_BASE& operator=(const T_SECT_PROP_BASE& rData)
	{
		StiffPure = rData.StiffPure;
		StiffRbar = rData.StiffRbar;  
		StiffTndn = rData.StiffTndn;  
		StiffTnRb = rData.StiffTnRb;  

		return *this;
	}
};
struct T_SECT_COMP_PROP
{
	CArray<T_SECT_STIFF_BASE,T_SECT_STIFF_BASE&> aSectStiff;
	void Initialize()
	{    
		aSectStiff.RemoveAll();
	}
	T_SECT_COMP_PROP () { Initialize(); }
	T_SECT_COMP_PROP(const T_SECT_COMP_PROP& rData) {*this = rData;}
	T_SECT_COMP_PROP& operator=(const T_SECT_COMP_PROP& rData)
	{
		Initialize();    
		aSectStiff.Copy(rData.aSectStiff);

		return *this;
	}
};
struct T_SECT_PROP_D
{
	CArray<T_SECT_PROP_BASE,T_SECT_PROP_BASE&> aSectProp;
	void Initialize()
	{    
		aSectProp.RemoveAll();
	}
	T_SECT_PROP_D () { Initialize(); }
	T_SECT_PROP_D(const T_SECT_PROP_D& rData) {*this = rData;}
	T_SECT_PROP_D& operator=(const T_SECT_PROP_D& rData)
	{
		Initialize();    
		aSectProp.Copy(rData.aSectProp);

		return *this;
	}
};


// section stiffness using effective width
#define T_STFE_K T_SECT_K
struct T_STFE_D
{
	double dIy[2];
	double dCzp[2];
	double dCzm[2];
	
	void Initialize()
	{
		dIy[0]  = dIy[1]  = 0.;
		dCzp[0] = dCzp[1] = 0.;
		dCzm[0] = dCzm[1] = 0.;
	}
};
struct T_STFE_UDRD_D
{
	T_STFE_K key;
	T_STFE_D data;
};
struct T_STFE_UNIT
{
	T_STFE_UNIT()
	{
		dIy  = D_UNITSYS_BASE_STIF;
		dCzp = D_UNITSYS_BASE_LENGTH;
		dCzm = D_UNITSYS_BASE_LENGTH;
	}
	int dIy;
	int dCzp;
	int dCzm;
};
#define HASHSIZESTFE HASHSIZESECT

// Steel Design Calculation Condition (JP)
#define T_STDC_K T_SECT_K
struct T_STDC_D
{
	BOOL bUpperLimitLB; //Upper limit of allowable stress for Local buckling
	BOOL bUpperLimitBC; //Upper limit of allowable bending comp. stress
	int  nUpperLimitBC; // 0:Box 1:Pi
	BOOL bGussetPlane;  //Axis parallel to the Gusset plane 
	int  nGussetPlane;  // 0: Local-x   1: Local-y 

	void Initialize()
	{
		bUpperLimitLB = FALSE;
		bUpperLimitBC = FALSE;
		nUpperLimitBC = 0;
		bGussetPlane = FALSE;
		nGussetPlane = 0;
	}
};
struct T_STDC_UDRD_D
{
	T_STDC_K key;
	T_STDC_D data;
};
struct T_STDC_UNIT
{
	T_STDC_UNIT()
	{
	}
};
#define HASHSIZESTDC HASHSIZESECT

// Composite Section for PSC Design - Section Dialog 에서 입력
#define T_SECP_K T_SECT_K
struct T_SECP_D
{
	BOOL bCompPSC;
	double dZ1[2];
	double dZ2[2];
	double dZ3[2];
	double dt1[2];
	double dt2[2];
	double dt3[2];
	double dTort[2];

	T_SECP_D() {}
	T_SECP_D(const T_SECP_D& src) {*this = src;}
	T_SECP_D& operator=(const T_SECP_D& src)
	{
		bCompPSC = src.bCompPSC;

		for(int i=0; i<2; ++i)
		{
			dZ1[i] = src.dZ1[i];
			dZ2[i] = src.dZ2[i];
			dZ3[i] = src.dZ3[i];
			dt1[i] = src.dt1[i];
			dt2[i] = src.dt2[i];
			dt3[i] = src.dt3[i];
			dTort[i] = src.dTort[i];
		}
		return *this;
	}
	
	void Initialize()
	{
		bCompPSC = FALSE;
		
		for(int i=0; i<2; ++i)
		{
			dZ1[i] = 0.0;
			dZ2[i] = 0.0;
			dZ3[i] = 0.0;
			dt1[i] = 0.0;
			dt2[i] = 0.0;
			dt3[i] = 0.0;
			dTort[i] = 0.0;
		}
	}
};
struct T_SECP_UDRD_D
{
	T_SECP_K key;
	T_SECP_D data;
};
struct T_SECP_UNIT
{
	T_SECP_UNIT()
	{
		dZ1 = D_UNITSYS_BASE_LENGTH;
		dZ2 = D_UNITSYS_BASE_LENGTH;
		dZ3 = D_UNITSYS_BASE_LENGTH;
		dt1 = D_UNITSYS_BASE_LENGTH;
		dt2 = D_UNITSYS_BASE_LENGTH;
		dt3 = D_UNITSYS_BASE_LENGTH;
		dTort = D_UNITSYS_BASE_LENGTH;
	}
	int dZ1;
	int dZ2;
	int dZ3;
	int dt1;
	int dt2;
	int dt3;
	int dTort;
};
#define HASHSIZESECP HASHSIZESECT

// section stiffness for report
struct T_RSCT_STFF_BASE
{
	double Area;
	double Asy;
	double Asz;
	double Ixx;
	double Iyy;
	double Izz;
	double zp;
	double zm;
	double yp;
	double ym;
	void Initialize()
	{
		Area = 0.0;
		Asy  = 0.0;
		Asz  = 0.0;
		Ixx  = 0.0;
		Iyy  = 0.0;
		Izz  = 0.0;
		zp   = 0.0;
		zm   = 0.0;
		yp   = 0.0;
		ym   = 0.0;
	}  
};
struct T_RSCT_STIFF_D
{
	int    nType;   // 1:DB/User/Value(형상정보있음), 2:Value(형상정보없음), 3:PSC/SPC
									// 4:SRC, 5:composite, 6:Tapered, 7:Tapered Composite
	T_RSCT_STFF_BASE Si;
	T_RSCT_STFF_BASE Sj;
	double EsEc; // 탄성계수비
	double GsGc; // 전단탄성계수비
	double DsDc; // 질량비
	double Ps;   // 포아송비 (steel)
	double Pc;   // 포아송비 (공구리)
	void Initialize()
	{
		nType = -1;
		Si.Initialize();
		Sj.Initialize();
		EsEc  = 0.0;
		GsGc  = 0.0;
		DsDc  = 0.0;
		Ps    = 0.0;
		Pc    = 0.0;
	}
};
// reinforcement of section for report
struct T_RSCT_RBAR_BASE
{
	CString strSize;
	int     nPart;
	int     nNum;
	double  dSpacing;
	void Initialize()
	{
		strSize.Empty();
		nPart = 0;
		nNum = 0;
		dSpacing = 0.0;
	}
};
struct T_RSCT_RBAR_D
{
	CArray<T_RSCT_RBAR_BASE, T_RSCT_RBAR_BASE&> aLRbar;
	BOOL bShearDiagonal;
	BOOL bShearWeb;
	BOOL bShearTorsion;
	BOOL bShearEnclosing;
	void Initialize()
	{
		aLRbar.RemoveAll();
		bShearDiagonal = FALSE;
		bShearWeb = FALSE;
		bShearTorsion = FALSE;
		bShearEnclosing = FALSE;
	}
};
// construction stage composite section for report
struct T_RSCT_CSCT_D
{
	CString strPart;
	CString strMatl;
	CString strStage;
	double  dAge;
	T_RSCT_STFF_BASE Si;
	T_RSCT_STFF_BASE Sj;
	double cyi, czi;
	double cyj, czj;
	void Initialize()
	{
		strPart.Empty();
		strMatl.Empty();
		strStage.Empty();
		dAge = 0.0;
		Si.Initialize();
		Sj.Initialize();
		cyi = czi = 0.0;
		cyj = czj = 0.0;
	}
};

// Virtual Sect corresponding to T_SECT_D
#define T_SECV_K T_VBEM_K
struct T_SECV_D
{
	int nStype; /**< 단면 종류(타입) */    // D_SECT_TYPE_??
	CString SName;/**< 단면 이름 */  // section name
	T_SECT_SECTION_D SectBefore;
	T_SECT_SECTION_D SectAfter;

	T_SECT_SECTBASE_D CmpTapJ;

	void ConvertToSect(T_SECT_D& rData)
	{
		rData.Initialize();

		rData.nStype     = nStype;
		rData.SName      = SName;
		rData.SectBefore = SectBefore;
		rData.SectAfter  = SectAfter;
		rData.CmpTapJ    = CmpTapJ;
	}
	void ConvertToSecv(T_SECT_D& rData)
	{
		nStype     = rData.nStype;
		SName      = rData.SName;
		SectBefore = rData.SectBefore;
		SectAfter  = rData.SectAfter;
		CmpTapJ    = rData.CmpTapJ;
	}
	void ConvertToSevo(T_SECT_D& rData)
	{
		nStype     = rData.nStype;
		SName      = rData.SName;
		SectBefore = rData.SectBefore;
		SectAfter  = rData.SectAfter;
		CmpTapJ    = rData.CmpTapJ;
	}
	void Initialize()
	{
		nStype = 0;
		SName.Empty();
		SectBefore.Initialize();
		SectAfter.Initialize();
		CmpTapJ.Initialize();
	}
	T_SECV_D() { Initialize(); }
	T_SECV_D(const T_SECV_D& src) { *this = src; }
	T_SECV_D& operator=(const T_SECV_D& src)
	{
		nStype = src.nStype;
		SName = src.SName;
		SectBefore = src.SectBefore;
		SectAfter = src.SectAfter;
		CmpTapJ = src.CmpTapJ;

		return (*this);
	}
};
struct T_SECV_UNIT
{
	T_SECV_UNIT()
	{
	}
	T_SECT_SECTION_UNIT SectBefore;
	T_SECT_SECTION_UNIT SectAfter;
};
struct T_SECV_UDRD_D
{
	T_SECV_K key;
	T_SECV_D data;
};
#define HASHSIZESECV 997
#define HASHSIZESECVITEM HASHSIZEELEM

//////////////////////////////////////////////////////////////////////////
typedef  T_SECV_K  T_SEVO_K;
typedef  T_SECV_D  T_SEVO_D;

struct T_SEVO_UNIT
{
	T_SEVO_UNIT()
	{
	}
	T_SECT_SECTION_UNIT SectBefore;
	T_SECT_SECTION_UNIT SectAfter;
};

struct T_SEVO_UDRD_D
{
	T_SEVO_K key;
	T_SEVO_D data;
};

#define HASHSIZESEVO 997
#define HASHSIZESEVOITEM HASHSIZEELEM
//////////////////////////////////////////////////////////////////////////

#pragma pack(pop)

#endif