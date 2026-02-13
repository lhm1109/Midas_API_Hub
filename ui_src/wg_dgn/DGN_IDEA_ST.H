#ifndef __DB_ST_DT_EXPORT_D_H__
#define __DB_ST_DT_EXPORT_D_H__

template<class Type>
struct T_IDEA_K  
{
    operator unsigned long() { return (i1<<16) + (i2); }
    BOOL operator == (const T_IDEA_K& u2) const { return i1 == u2.i1 && i2 == u2.i2; }
    void Set(Type i1, Type i2) { this->i1 = i1; this->i2 = i2; }
    Type i1;
    Type i2;
};

template<class Type>
struct T_IDEA_K3  
{
    operator unsigned long() { return (i1<<16) + (i2<<8) + (i3); }
    BOOL operator == (const T_IDEA_K3& u2) const { return i1 == u2.i1 && i2 == u2.i2 &&i3 == u2.i3; }
    void Set(Type i1, Type i2, Type i3) { this->i1 = i1; this->i2 = i2; this->i3 = i3; }
    Type i1;
    Type i2;
    Type i3;
};

template< class KEY, class ARG_KEY, class VALUE, class ARG_VALUE > 
class CMapIDEA : public CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>
{
public:
    CMapIDEA(int nBlockSize = 10);

public:
    BOOL Exist(ARG_KEY key) const;
    void GetNextKey(POSITION& rNextPosition, KEY& rKey) const;
    int  GetKeyList(CArray<KEY, KEY>& raKey) const;
    int  GetSortedKeyListforUintK(CArray<KEY, KEY>& raKey) const;
	CAssoc* GetAssocAt(ARG_KEY, UINT&) const;

protected:
    static int  UINTAsc(const void *v1, const void *v2);
};

//----------------------------------------------------------------------
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
CMapIDEA<KEY, ARG_KEY, VALUE, ARG_VALUE>::CMapIDEA(int nBlockSize) 
: CMap<KEY, ARG_KEY, VALUE, ARG_VALUE>(nBlockSize)
{
}

//----------------------------------------------------------------------
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
BOOL CMapIDEA<KEY, ARG_KEY, VALUE, ARG_VALUE>::Exist(ARG_KEY key) const
{
	ASSERT_VALID(this);

	UINT nHash;
	CAssoc* pAssoc = GetAssocAt(key, nHash);
	if (pAssoc == NULL)
		return FALSE;  // not in map

	return TRUE;
}

//----------------------------------------------------------------------
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void CMapIDEA<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetNextKey(POSITION& rNextPosition, KEY& rKey) const
{
	VALUE rValue;
	GetNextAssoc(rNextPosition, rKey, rValue);
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
int CMapIDEA<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetKeyList(CArray<KEY, KEY>& raKey) const
{
    int nCount = GetCount();
    raKey.SetSize(nCount);

    int i = 0;
    POSITION pos = GetStartPosition();
    while (pos) GetNextKey(pos, raKey[i++]);

    return nCount;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
int CMapIDEA<KEY, ARG_KEY, VALUE, ARG_VALUE>::UINTAsc(const void *v1, const void *v2)
{
    UINT u1 = *((UINT*)v1);
    UINT u2 = *((UINT*)v2);
    if (u1 > u2) return 1;
    else if (u1 < u2) return -1;
    return 0;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
int CMapIDEA<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetSortedKeyListforUintK(CArray<KEY, KEY>& raKey) const
{
    int nCount = GetKeyList(raKey);

    ASSERT(sizeof(KEY) == sizeof(UINT));
    qsort(raKey.GetData(), nCount, sizeof(KEY), UINTAsc);

    return nCount;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
typename CMapIDEA<KEY, ARG_KEY, VALUE, ARG_VALUE>::CAssoc*
    CMapIDEA<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetAssocAt(ARG_KEY key, UINT& nHashValue) const
    // find association (or return NULL)
{
    UINT nHashBucket;
    return CMap::GetAssocAt(key, nHashBucket, nHashValue);
}
// Map Size Index
enum
{
    S_IDEA_NODE=0,
    S_IDEA_ELEM,
    S_IDEA_MEMB,
    S_IDEA_MATL,
    S_IDEA_MTST,
    S_IDEA_MTCN,
    S_IDEA_MTRB,
    S_IDEA_SECT,  
    S_IDEA_ELST,  
    S_IDEA_CONS,
    S_IDEA_STLD,
    S_IDEA_LCOM,
    S_IDEA_LENG,
    S_IDEA_KFAC,
    S_IDEA_LTSR,
    S_IDEA_CMFT,
    S_IDEA_FMAG,
    S_IDEA_RDBM,
    S_IDEA_RDCN,
    S_IDEA_RCBM,
    S_IDEA_RCCN,
    S_IDEA_REBT,
    S_IDEA_RPSC,
    S_IDEA_TDIF,  
    S_IDEA_REAC,
    S_IDEA_DISP,
    S_IDEA_TFOR,
    S_IDEA_BFOR,
    S_IDEA_RECC,
    S_IDEA_DSPC,
    S_IDEA_BCFO,
    // for 2D Element
    S_IDEA_THIK,
    S_IDEA_PFOR,
    // for Construction Stage
    S_IDEA_CSLC,
    S_IDEA_CLAT,
    S_IDEA_STAG,
    S_IDEA_CSCS,
    S_IDEA_EGRP,
    S_IDEA_BGRP,
    S_IDEA_LGRP,  
    S_IDEA_CSDP,
    S_IDEA_CSTF,
    S_IDEA_CSBF,
    S_IDEA_CSPF,
    // Added For V200
    S_IDEA_CSET,
    S_IDEA_TDLS,
    // Added For V210  
    S_IDEA_EPTB,  // Beam Force Result For Part ( Static, Rs, EccenRS, TH )
    S_IDEA_EPTC,  // Concurrent Force Result For Part ( Moving, Settlement )
    S_IDEA_EPCS,  // Beam Force Result For Part (Construction Stage)
    S_IDEA_NUM,
};

enum
{
    S_IDEA_STLC=1,
    S_IDEA_RSLC,
    S_IDEA_ERLC,
    S_IDEA_THLC,
    S_IDEA_MLLC,
    S_IDEA_SMLC,
};

#define D_IDEARSFILE_100              100
#define D_IDEARSFILE_101              101 // Add Tendon Name
#define D_IDEARSFILE_110              110 // Add 2D Element Information
#define D_IDEARSFILE_200              200 // Add construction Stage Information
#define D_IDEARSFILE_201              201 // Add Tendon Key, Construnction Stage Result 
#define D_IDEARSFILE_210              210 // Add Part Result For CSCS, Add Section Size 
#define D_IDEARSFILE_220              220 // Add Line Type Section Data
#define D_IDEARSFILE_VERSION          D_IDEARSFILE_220

#define DT_UNITSYS_LENGTH_INDEX_MM    0
#define DT_UNITSYS_LENGTH_INDEX_CM    1
#define DT_UNITSYS_LENGTH_INDEX_M     2
#define DT_UNITSYS_LENGTH_INDEX_IN    3
#define DT_UNITSYS_LENGTH_INDEX_FT    4

#define DT_UNITSYS_FORCE_INDEX_KG     0
#define DT_UNITSYS_FORCE_INDEX_TON    1
#define DT_UNITSYS_FORCE_INDEX_N      2
#define DT_UNITSYS_FORCE_INDEX_KN     3
#define DT_UNITSYS_FORCE_INDEX_LBF    4
#define DT_UNITSYS_FORCE_INDEX_KIP    5

#define DT_SECT_SIZE_NUM_MAX          60

#define S_SECT_NUM                    2
#define S_ELEMSECT_NUM                5

#define S_ELEMPOS_I                   1
#define S_ELEMPOS_Q                   2
#define S_ELEMPOS_H                   3 
#define S_ELEMPOS_T                   4
#define S_ELEMPOS_J                   5

#define S_ELEMPOS_RI                  0.0
#define S_ELEMPOS_RQ                  0.25
#define S_ELEMPOS_RH                  0.5
#define S_ELEMPOS_RT                  0.75
#define S_ELEMPOS_RJ                  1.0

#pragma pack(push, 1)

//////////////////////////////////////////////////////////////////////////
// Unit
struct S_UNIT_D
{
    int nBase_Length;    // Length Unit
    int nBase_Force;     // Force Unit
    void Initialize()
    {
        nBase_Length = DT_UNITSYS_LENGTH_INDEX_M;
        nBase_Force  = DT_UNITSYS_FORCE_INDEX_N;
    }
    S_UNIT_D()  { Initialize(); }
    S_UNIT_D(S_UNIT_D &rData) {*this = rData;}
}; 

//////////////////////////////////////////////////////////////////////////
/// node ///
#define S_NODE_K unsigned int
#define S_NODE_K_LIST CArray<S_NODE_K, S_NODE_K>
struct S_NODE_D
{
    double dx;       // x
    double dy;       // y
    double dz;       // z
    void Initialize()
    {
        dx=0.0;
        dy=0.0;
        dz=0.0;
    }

    S_NODE_D()  { Initialize(); }
    S_NODE_D(S_NODE_D &rData) {*this = rData;}
};
#define S_NODE_D_LIST CArray<S_NODE_D, S_NODE_D&>

//////////////////////////////////////////////////////////////////////////
// element
#define TRUSS_EL        1
#define BEAM_EL         2
#define PLSTRS_EL       3
#define PLATE_EL        4
#define PLSTRN_EL       6

#define S_ELEM_K unsigned int
#define S_SECT_K unsigned int
#define S_MATL_K unsigned int

#define S_ELEM_K_LIST CArray<S_ELEM_K, S_ELEM_K>
#define S_SECT_K_LIST CArray<S_SECT_K, S_SECT_K>
#define S_MATL_K_LIST CArray<S_MATL_K, S_MATL_K>

#define DT_ELEM_MAXNOD 8
struct S_ELEM_D
{
    int      nEltype;               // Element type, 1=truss, 2=Beam
    S_MATL_K MatlK;                 // Material ID
    S_SECT_K SectK;                 // Section ID
    double   dAngle;                // Beta angle
    int      NodeK[DT_ELEM_MAXNOD]; // Connected Node ID

    void Initialize()
    {
        nEltype    = 0;
        MatlK      = 0;
        SectK      = 0;
        dAngle     = 0.0;
        memset(NodeK, 0, sizeof(NodeK));
    }
    S_ELEM_D()  { Initialize(); }
    S_ELEM_D(S_ELEM_D &rData) {*this = rData;}
};
#define S_ELEM_D_LIST CArray<S_ELEM_D, S_ELEM_D&>

//////////////////////////////////////////////////////////////////////////
// Member 
// Member type
#define DT_MBTP_COLUMN  1
#define DT_MBTP_BEAM    2
#define DT_MBTP_BRACE   3
#define DT_MBTP_WALL    4

#define S_MEMB_K S_ELEM_K
#define S_MEMB_K_LIST CArray<S_MEMB_K, S_MEMB_K>
struct S_MEMB_D
{
    int           nMbType;      // 1=Column 2=Beam 3=Brace
    S_ELEM_K_LIST aElemK;       // Element list
    BOOL          bReverseDir;  // Reverse Direction
    void Initialize()
    {
        nMbType = 0;
        aElemK.RemoveAll();
        bReverseDir = FALSE;
    }
    S_MEMB_D()  { Initialize(); }
    S_MEMB_D(S_MEMB_D &rData) {*this = rData;}
    S_MEMB_D& operator=(S_MEMB_D &rData)
    {
        nMbType = rData.nMbType;
        aElemK.Copy(rData.aElemK);
        bReverseDir = rData.bReverseDir;
        return *this;
    }  
};
#define S_MEMB_D_LIST CArray<S_MEMB_D, S_MEMB_D&>

//////////////////////////////////////////////////////////////////////////
// Material
struct S_MATL_PROP
{
    double dElast;          // Elastic Modulus
    double dPoisson;        // Poisson's Ratio
    double dThermal;        // Thermal Coefficient
    double dDensity;        // Weight Density
    double dMassDensity;    // Mass Density 
    double dShearModulus;   // Shear Modulus
    //
    void Initialize()
    {
        dElast=0.0;
        dPoisson=0.0;
        dThermal=0.0;
        dDensity=0.0;
        dMassDensity=0.0;
        dShearModulus=0.0;
    }

    S_MATL_PROP()  { Initialize(); }
    S_MATL_PROP(S_MATL_PROP &rData) {*this = rData;}
};

#define S_MATL_STEEL_K S_MATL_K
#define S_MATL_STEEL_K_LIST CArray<S_MATL_STEEL_K, S_MATL_STEEL_K>
struct S_MATL_STEEL_D
{
    CString     szCode;     // Code Name, ex) EN05(S)
    CString     szMatName;  // Material Name, ex) S275, S355
    S_MATL_PROP PropD;
    double      dS_Fu;        // Ultimate strength
    double      dS_Fy;        // Yield strength
    double      dS_Fy1;       //
    double      dS_Fy2;       //
    double      dS_Fy3;       //
    double      dS_Fy4;       //
    double      dS_Fy5;       //
    double      dS_Fy6;       //
    void Initialize()
    {
        szCode = _T("");
        szMatName = _T("");
        PropD.Initialize();
        dS_Fu=0.0; 
        dS_Fy=0.0; 
        dS_Fy1=0.0;
        dS_Fy2=0.0;
        dS_Fy3=0.0;
        dS_Fy4=0.0;
        dS_Fy5=0.0; 
        dS_Fy6=0.0; 
    }
    S_MATL_STEEL_D()  { Initialize(); }
    S_MATL_STEEL_D(S_MATL_STEEL_D &rData) {*this = rData;}
};
#define S_MATL_STEEL_D_LIST CArray<S_MATL_STEEL_D, S_MATL_STEEL_D&>

#define S_MATL_CONCRETE_K S_MATL_K
#define S_MATL_CONCRETE_K_LIST CArray<S_MATL_CONCRETE_K, S_MATL_CONCRETE_K>
struct S_MATL_CONCRETE_D
{
    CString     szCode;     // Code Name, ex) EN04(RC)
    CString     szMatName;  // Material Name, ex) C35/45...
    S_MATL_PROP PropD;        // Material property Data
    double      dC_fc;        // Compression strength
    void Initialize()
    {
        szCode = _T("");
        szMatName = _T("");
        PropD.Initialize();
        dC_fc=0.0; 
    }
    S_MATL_CONCRETE_D()  { Initialize(); }
    S_MATL_CONCRETE_D(S_MATL_CONCRETE_D &rData) {*this = rData;}
};
#define S_MATL_CONCRETE_D_LIST CArray<S_MATL_CONCRETE_D, S_MATL_CONCRETE_D&>

struct S_MATL_REBAR
{
    double dB_fy;      // Yield strength
    double dB_Elast;   // Elastic modulus
    void Initialize()
    {
        dB_fy=0.0;
        dB_Elast=0.0;
    }
    S_MATL_REBAR()  { Initialize(); }
    S_MATL_REBAR(S_MATL_REBAR &rData) {*this = rData;}
};

#define S_MATL_REIF_K S_MATL_K
#define S_MATL_REIF_K_LIST CArray<S_MATL_REIF_K, S_MATL_REIF_K>
struct S_MATL_REBAR_INFO
{
    CString      szRebar_CodeName;       // Code name
    CString      szMainRebar_RebarName;  // Longitudinal Rebar Name
    CString      szSubRebar_RebarName;   // Shear Rebar Name
    S_MATL_REBAR MainRebarData;            // Longitudinal Rebar property
    S_MATL_REBAR SubRebarData;             // Shear Rebar property
    void Initialize()
    {
        szRebar_CodeName=_T("");
        szMainRebar_RebarName=_T("");
        szSubRebar_RebarName=_T("");
        MainRebarData.Initialize();
        SubRebarData.Initialize();
    }
    S_MATL_REBAR_INFO()  { Initialize(); }
    S_MATL_REBAR_INFO(S_MATL_REBAR_INFO &rData) {*this = rData;}
};
#define S_MATL_REIF_D_LIST CArray<S_MATL_REBAR_INFO, S_MATL_REBAR_INFO&>

// S_MATL_K
#define S_MATL_ADIF_K S_MATL_K
#define S_MATL_ADIF_K_LIST CArray<S_MATL_ADIF_K, S_MATL_ADIF_K>
struct S_MATL_ADIF_D
{
    CString      szType;         // Material type
    CString      szName;         // Name
    double       dSpecificHeat;    // Specific Heat  
    double       dHeatConduction;  // Heat Conduction
    int          nThermalUnit;     // 0:Celsius, 1:Fahrenheit 

    void Initialize()
    {
        szType=_T("");
        szName=_T("");
        dSpecificHeat = 0.0;      
        dHeatConduction = 0.0;
        nThermalUnit = 0;
    }

    S_MATL_ADIF_D()  { Initialize(); }
    S_MATL_ADIF_D(S_MATL_ADIF_D &rData) {*this = rData;}
};
#define S_MATL_ADIF_D_LIST CArray<S_MATL_ADIF_D, S_MATL_ADIF_D&>

//////////////////////////////////////////////////////////////////////////
// Section

#define DT_SECT_TYPE_REGULAR       1
#define DT_SECT_TYPE_SRC           2
#define DT_SECT_TYPE_USER          3
#define DT_SECT_TYPE_COMBINED      4
#define DT_SECT_TYPE_TAPERED       5
#define DT_SECT_TYPE_CONSTRUCTION  6
#define DT_SECT_TYPE_HIBEAM        7
#define DT_SECT_TYPE_COLDFORMED    8
#define DT_SECT_TYPE_COMPO_B       9       // Composite Box
#define DT_SECT_TYPE_COMPO_I       10      // Composite I
#define DT_SECT_TYPE_PSC           11
#define DT_SECT_TYPE_COMPO_CI      12      // Composite Concrete-I
#define DT_SECT_TYPE_COMPO_CT      13      // Composite Concrete-T
#define DT_SECT_TYPE_COMPO_PC      14      // Composite Section (거더가 PSC단면을 사용하는 것들)
#define DT_SECT_TYPE_COMPO_G       15      // Composite General

//Regular
#define DT_SECT_SHAPE_REG_L          "L"
#define DT_SECT_SHAPE_REG_C          "C"
#define DT_SECT_SHAPE_REG_H          "H"
#define DT_SECT_SHAPE_REG_T          "T"
#define DT_SECT_SHAPE_REG_B          "B"
#define DT_SECT_SHAPE_REG_P          "P"
#define DT_SECT_SHAPE_REG_SB         "SB"
#define DT_SECT_SHAPE_REG_SR         "SR"
#define DT_SECT_SHAPE_REG_2L         "2L"
#define DT_SECT_SHAPE_REG_2C         "2C"
#define DT_SECT_SHAPE_REG_CC         "CC"
#define DT_SECT_SHAPE_REG_URIB       "URIB"
#define DT_SECT_SHAPE_REG_OCT        "OCT"
#define DT_SECT_SHAPE_REG_SOCT       "SOCT"
#define DT_SECT_SHAPE_REG_TRK        "TRK"
#define DT_SECT_SHAPE_REG_STRK       "STRK"
#define DT_SECT_SHAPE_REG_HTRK       "HTRK"
#define DT_SECT_SHAPE_REG_CL         "CL"    // Cross Angle
#define DT_SECT_SHAPE_REG_ROCT       "ROCT"
#define DT_SECT_SHAPE_REG_BSTF       "BSTF"  // Box with stiffener
#define DT_SECT_SHAPE_REG_PSTF       "PSTF"  // Pipe with stiffener
#define DT_SECT_SHAPE_REG_GEN        "GEN"   // General Section (Value에서만 사용)
#define DT_SECT_SHAPE_REG_UDT        "UDT"   // Upside-Down T
#define DT_SECT_SHAPE_REG_CB         "CB"    

// PSC
#define DT_SECT_SHAPE_PSC_1CELL      "1CEL"
#define DT_SECT_SHAPE_PSC_2CELL      "2CEL"
#define DT_SECT_SHAPE_PSC_3CELL      "3CEL"
#define DT_SECT_SHAPE_PSC_4CELL      "NCEL"
#define DT_SECT_SHAPE_PSC_MID        "PSCM"
#define DT_SECT_SHAPE_PSC_HALF       "PSCH"
#define DT_SECT_SHAPE_PSC_T          "PSCT"
#define DT_SECT_SHAPE_PSC_BOX        "PSCB"
#define DT_SECT_SHAPE_PSC_I          "PSCI"
#define DT_SECT_SHAPE_PSC_NCEL2      "NCE2"
#define DT_SECT_SHAPE_PSC_VALUE      "VALU"

// Composite Section 
#define DT_SECT_SHAPE_COMPO_CI       "CPCI"
#define DT_SECT_SHAPE_COMPO_CT       "CPCT"
#define DT_SECT_SHAPE_COMPO_PC       "CPPC"


#define S_SECT_IJ_K   unsigned int // Section part
#define S_SECT_IJ_K_LIST CArray<S_SECT_IJ_K, S_SECT_IJ_K>
#define S_SECT_PART_K unsigned int // Section part
#define S_SECT_PART_K_LIST CArray<S_SECT_PART_K, S_SECT_PART_K>
struct S_PROP_BASE
{
    S_SECT_K      SectK;
    S_SECT_PART_K SectPartK;  // Section Part key
    S_SECT_IJ_K   SectIJK;    // Section IJ Key
    S_MATL_K      MatlK;      // Material key
    void Initialize()
    {
        SectK     = 0;
        SectPartK = 0;
        SectIJK   = 0;
        MatlK     = 0;
    }  
	S_PROP_BASE()	{Initialize();}
	S_PROP_BASE(S_PROP_BASE &rData)	{*this = rData;}
};

#define S_SPRO_K S_SECT_K
#define S_SPRO_K_LIST CArray<S_SPRO_K, S_SPRO_K>
struct S_PROP_D
{
    CArray<S_PROP_BASE,S_PROP_BASE&> aPropD;
    void Initialize()
    {
        aPropD.RemoveAll();
    }  
	S_PROP_D()	{Initialize();}
	S_PROP_D(S_PROP_D &rData)	{*this = rData;}
	S_PROP_D& operator = (const S_PROP_D &rData)
	{
        aPropD.Copy(rData.aPropD);
		return *this;
	}
};
#define S_SPRO_D_LIST CArray<S_PROP_D, S_PROP_D&>

#define S_POLY_K unsigned int
struct S_GSEC_VERTEX  // Point of Polygon
{
    double dX;
    double dY;
    S_GSEC_VERTEX() {Initialize();}
    S_GSEC_VERTEX(double x, double y) { dX = x; dY = y; }
    S_GSEC_VERTEX& Set(double x, double y) { dX = x; dY = y; return *this; }
    void Initialize() { dX = dY = 0.0; }
};
#define S_GSEC_VERTEXT_LIST CArray<S_GSEC_VERTEX, S_GSEC_VERTEX&>

struct S_GSEC_POLYGON  // Polygon of General Section 
{
    S_GSEC_VERTEXT_LIST aVertex;
    void Initialize()
    {
        aVertex.RemoveAll();
    }

    S_GSEC_POLYGON() {Initialize();}
    S_GSEC_POLYGON(const S_GSEC_POLYGON& crData) { *this = crData; }
    S_GSEC_POLYGON& operator=(const S_GSEC_POLYGON& crData) 
    {
        aVertex.Copy(crData.aVertex);
        return (*this);
    }
};
#define S_GSEC_POLYGON_LIST CArray<S_GSEC_POLYGON, S_GSEC_POLYGON&>

struct S_SECT_STIFFNESS
{
    double dArea;    // Area
    double dAsy;     // Effective Shear Area-y
    double dAsz;     // Effective Shear Area-z
    double dRxx;     // Torsion constant
    double dRyy;     // Area Moment of Inertia - y
    double dRzz;     // Area Moment of Inertia - z
    double dCyp;     // Centroid to Right 
    double dCym;     // Centroid - y 
    double dCzp;     // Centroid to Top
    double dCzm;     // Centroid - z
    double dQyb;     // Q/b, Q : First Moment of Area, b: breadth - y
    double dQzb;     // Q/b, Q : First Moment of Area, b: breadth - z
    double dWArea;   // Weight Area 
    double dx1, dy1; // Stress point, Top Left
    double dx2, dy2; // Top Right
    double dx3, dy3; // Bottom Right
    double dx4, dy4; // Bottom Left
    //
    double dYBar;    // Centroid - y, origin of coordinate
    double dZBar;    // Centroid - z

    void Initialize()
    {
        dArea=0.0;
        dAsy=0.0;
        dAsz=0.0;
        dRxx=0.0;
        dRyy=0.0;
        dRzz=0.0;
        dCyp=0.0;
        dCym=0.0;
        dCzp=0.0;
        dCzm=0.0;
        dQyb=0.0;
        dQzb=0.0;
        dWArea=0.0;
        dx1 = 0.0;
        dx2 = 0.0;
        dx3 = 0.0;
        dx4 = 0.0;
        dy1 = 0.0;
        dy2 = 0.0;
        dy3 = 0.0;
        dy4 = 0.0;
        dYBar = 0.0;
        dZBar = 0.0;
    }
    S_SECT_STIFFNESS() {Initialize();}
    S_SECT_STIFFNESS(const S_SECT_STIFFNESS &rData) {*this = rData;}
    S_SECT_STIFFNESS& operator=(const S_SECT_STIFFNESS &rData)
    {
        dArea =rData.dArea;  
        dAsy  =rData.dAsy;
        dAsz  =rData.dAsz; 
        dRxx  =rData.dRxx;
        dRyy  =rData.dRyy; 
        dRzz  =rData.dRzz; 
        dCyp  =rData.dCyp; 
        dCym  =rData.dCym; 
        dCzp  =rData.dCzp; 
        dCzm  =rData.dCzm; 
        dQyb  =rData.dQyb; 
        dQzb  =rData.dQzb; 
        dWArea=rData.dWArea;
        dx1   =rData.dx1;  
        dx2   =rData.dx2;  
        dx3   =rData.dx3;  
        dx4   =rData.dx4;  
        dy1   =rData.dy1;  
        dy2   =rData.dy2;  
        dy3   =rData.dy3;  
        dy4   =rData.dy4;  
        dYBar =rData.dYBar;
        dZBar =rData.dZBar;
        return *this;
    }
};

struct S_GSPO_D
{
    CArray<S_GSEC_VERTEX,S_GSEC_VERTEX&> aPoint;  // Polygon
    void Initialize()
    {
        aPoint.RemoveAll();    
    }

    S_GSPO_D() {Initialize();}
    S_GSPO_D(const S_GSPO_D &rData) { *this = rData; }
    S_GSPO_D& operator=(const S_GSPO_D &rData) 
    {
        //nPolyType   = rData.nPolyType;
        //naPolyIndex = rData.naPolyIndex;
        aPoint.Copy(rData.aPoint);

        return *this;
    }
};

struct S_GSPI_D
{
    S_GSEC_VERTEXT_LIST aPoint; // Polygon
    
    void Initialize()
    {
        aPoint.RemoveAll();
    }  
    S_GSPI_D() {Initialize();}
    S_GSPI_D(const S_GSPI_D &rData) {*this = rData;}
    S_GSPI_D& operator=(const S_GSPI_D &rData) 
    {
        //naPolyIndex = rData.naPolyIndex;
        aPoint.Copy(rData.aPoint);
        return *this;
    }
};

struct S_GSEC_PART 
{
    CString szShape;                         // Shape
    CString szDBName;                        // DB Name
    double  dSize[DT_SECT_SIZE_NUM_MAX];       // Size Data
    int     nBuiltUpFlag;                      // 1=Builtup Section(D_SECT_BUILT_BUILTUP) 
                                                                                         // 2=Rolled Section(D_SECT_BUILT_ROLLED)

    S_SECT_STIFFNESS Stiffness;                // Stiffness
    S_SECT_STIFFNESS StiffnessCom;             // Composite stiffness
    double dPeriIn;                            // Perimeter - In
    double dPeriOut;                           // Perimeter - Out
    double dPeriInCom;                         // Combination Perimeter In
    double dPeriOutCom;                        // Combination Perimeter Out

    CArray<S_GSPO_D, S_GSPO_D&> aOuterPolygon; // Outer Polygon
    CArray<S_GSPI_D, S_GSPI_D&> aInnerPolygon; // Inner Polygon
    
    // S_SECT_SECTBASE_D -> [OutPolygon, InPolygon, Line, Shape, aLineLoop]
    // Ex1) IdxStart=0, IdxEnd=1 : 0
    // Ex2) IdxStart=1, IdxEnd=1 : No items
    // Ex3) IdxStart=3, IdxEnd=7 : 3,4,5,6
    int IdxStart[5]; // Start Index
    int IdxEnd  [5]; // Last Index. IdxEnd is not included.

    void Initialize()
    {
        szShape  = _T("");
        szDBName = _T("");
        memset(dSize, 0,sizeof(dSize));
        nBuiltUpFlag = 0;
        Stiffness.Initialize();
        StiffnessCom.Initialize();
        dPeriIn     =0.0;
        dPeriOut    =0.0;
        dPeriInCom  =0.0;
        dPeriOutCom =0.0;
        
        aOuterPolygon.RemoveAll();
        aInnerPolygon.RemoveAll();
        memset(IdxStart, 0, sizeof(IdxStart));
        memset(IdxEnd, 0, sizeof(IdxEnd));
    }
    S_GSEC_PART() {Initialize();}
    S_GSEC_PART(const S_GSEC_PART &rData) { *this = rData; }
    S_GSEC_PART& operator=(const S_GSEC_PART &rData) 
    {
        szShape  = rData.szShape;
        szDBName = rData.szDBName;
        memcpy(dSize, rData.dSize, sizeof(dSize));
        nBuiltUpFlag = rData.nBuiltUpFlag;
        Stiffness    = rData.Stiffness;
        StiffnessCom = rData.StiffnessCom;
        dPeriIn      = rData.dPeriIn;
        dPeriOut     = rData.dPeriOut;
        dPeriInCom   = rData.dPeriInCom;
        dPeriOutCom  = rData.dPeriOutCom;

        aOuterPolygon.Copy(rData.aOuterPolygon);
        aInnerPolygon.Copy(rData.aInnerPolygon);
        memcpy(IdxStart, rData.IdxStart, sizeof(IdxStart));
        memcpy(IdxEnd, rData.IdxEnd, sizeof(IdxEnd));

        return *this;
    }
};
#define S_GSEC_PART_D_LIST CArray<S_GSEC_PART, S_GSEC_PART&>

// T_SECT_LINE이랑 똑같음. dA와 dVF는 출력 안 함.
struct S_GSEC_LINE  // Value-General Section에서 라인 정보로 사용
{
    int v1, v2;   // Vertex ID
    double dThik; // 라인 두께
    int nAlign;   // 선의 Align 정보 (0:위쪽,1:가운데,2:아래쪽)
    void Initialize()
    {
        v1 = v2 = 0;
        dThik = 0.0;
        nAlign = 0;
    }
};

#define S_SECT_BASE_K S_SECT_K
#define S_SECT_BASE_K_LIST CArray<S_SECT_BASE_K, S_SECT_BASE_K>
struct S_SECT_SECTBASE_D
{
    int      nStype;      // D_SECT_TYPE_??
    CString  szShape;     // D_SECT_SHAPE_???
    CString  szDBName;    // DB Name
    CString  szName;      // Section name
    UINT     nPartBaseK;  // Part Base Key 
    double   dSize[DT_SECT_SIZE_NUM_MAX]; // Size Data

    S_PROP_D PartProp;
    S_SECT_STIFFNESS Stiffness;   // Stiffness

    S_GSEC_POLYGON_LIST aOuterPolygon; // Outer polygon
    S_GSEC_POLYGON_LIST aInnerPolygon; // Inner polygon
    
    CArray<S_GSEC_PART,S_GSEC_PART&> aSectPart; // Part Data
    
    // The polygon which has vertexes is determined by aSectPart->IdxStart/IdxEnd 
    CArray<S_GSEC_LINE, S_GSEC_LINE&> aLine; // Value-General Line Type
    
    // aLineLoop : Count(n1), LineIx1, LineIx2, ...LineIx(n1), Count(n2), LineIx1, LineIx2, ..., LineIx(n2), ...
    // LineIx value (-) means reversed direction.
    CArray<int, int> aLineLoop;  // Line Type Loop
    
    void Initialize()
    {
        nStype     = 0;
        szShape   = _T("");
        szDBName  = _T("");
        szName    = _T("");
        nPartBaseK = 0;
        memset(dSize, 0, sizeof(dSize));

        PartProp.Initialize();
        
        Stiffness.Initialize();
        aOuterPolygon.RemoveAll();
        aInnerPolygon.RemoveAll();
        aSectPart.RemoveAll();
        aLine.RemoveAll();
        aLineLoop.RemoveAll();    
    }

    S_SECT_SECTBASE_D() {Initialize();}
    S_SECT_SECTBASE_D(const S_SECT_SECTBASE_D &rData) { *this = rData; }
    S_SECT_SECTBASE_D& operator=(const S_SECT_SECTBASE_D &rData)
    {
        nStype     = rData.nStype;
        szShape   = rData.szShape;
        szDBName  = rData.szDBName;
        szName    = rData.szName;
        PartProp   = rData.PartProp;
        nPartBaseK = rData.nPartBaseK;
        Stiffness  = rData.Stiffness;
        memcpy(dSize, rData.dSize, sizeof(dSize));

        aOuterPolygon.Copy(rData.aOuterPolygon);
        aInnerPolygon.Copy(rData.aInnerPolygon);
        aSectPart.Copy(rData.aSectPart);
        aLine.Copy(rData.aLine);
        aLineLoop.Copy(rData.aLineLoop);

        return *this;
    }
};
#define S_SECT_BASE_D_LIST CArray<S_SECT_SECTBASE_D, S_SECT_SECTBASE_D&>

struct S_SECT_D
{
    S_SECT_SECTBASE_D SectI;  // I단의 CrossSection 정보
    S_SECT_SECTBASE_D SectJ;  // J단의 CrossSection 정보

    void Initialize()
    {
        SectI.Initialize();
        SectJ.Initialize();
    }
    
    S_SECT_D() {Initialize();}
    S_SECT_D(const S_SECT_D &rData) { *this = rData; }
    S_SECT_D& operator=(const S_SECT_D &rData)
    {
        SectI = rData.SectI;
        SectJ = rData.SectJ;
        return *this;
    }
};
#define S_SECT_D_LIST CArray<S_SECT_D, S_SECT_D&>

//////////////////////////////////////////////////////////////////////////
// Element for Tapered Section
struct S_ELST_D
{
    S_SECT_SECTBASE_D SectI;  // I단의 CrossSection 정보
    S_SECT_SECTBASE_D SectQ;  // 1/4 지점의 CrossSection 정보
    S_SECT_SECTBASE_D SectH;  // 1/2 지점의 CrossSection 정보
    S_SECT_SECTBASE_D SectT;  // 3/4 지점의 CrossSection 정보
    S_SECT_SECTBASE_D SectJ;  // J단의 CrossSection 정보

    void Initialize()
    {
        SectI.Initialize();
        SectQ.Initialize();
        SectH.Initialize();
        SectT.Initialize();
        SectJ.Initialize();
    }  
    S_ELST_D() {Initialize();}
    S_ELST_D(const S_ELST_D &rData) { *this = rData; }
    S_ELST_D& operator=(const S_ELST_D &rData)
    {
        SectI = rData.SectI;
        SectQ = rData.SectQ;
        SectH = rData.SectH;
        SectT = rData.SectT;
        SectJ = rData.SectJ;
        return *this;
    }
};
#define S_ELST_D_LIST CArray<S_ELST_D, S_ELST_D&>

//////////////////////////////////////////////////////////////////////////
// Composite Section for construction Stage for Element Use Tapered Section 
#define S_PART_K UINT
#define S_PART_K_LIST CArray<S_PART_K, S_PART_K>
struct S_CSET_D
{
    // 해당파트가 Active 되는 순운간
    int nActiveStage;  

    S_GSEC_POLYGON_LIST aOPolyI; // Outer polygon I
    S_GSEC_POLYGON_LIST aIPolyI; // Inner polygon I
    S_GSEC_POLYGON_LIST aOPolyQ; // Outer polygon 1/4
    S_GSEC_POLYGON_LIST aIPolyQ; // Inner polygon 1/4
    S_GSEC_POLYGON_LIST aOPolyH; // Outer polygon 2/4
    S_GSEC_POLYGON_LIST aIPolyH; // Inner polygon 2/4
    S_GSEC_POLYGON_LIST aOPolyT; // Outer polygon 3/4
    S_GSEC_POLYGON_LIST aIPolyT; // Inner polygon 3/4
    S_GSEC_POLYGON_LIST aOPolyJ; // Outer polygon J
    S_GSEC_POLYGON_LIST aIPolyJ; // Inner polygon J

    void Initialize()
    {
        nActiveStage = 0;

        aOPolyI.RemoveAll();
        aIPolyI.RemoveAll();
        aOPolyQ.RemoveAll();
        aIPolyQ.RemoveAll();
        aOPolyH.RemoveAll();
        aIPolyH.RemoveAll();
        aOPolyT.RemoveAll();
        aIPolyT.RemoveAll();
        aOPolyJ.RemoveAll();
        aIPolyJ.RemoveAll();
    }  
    S_CSET_D() {Initialize();}
    S_CSET_D(const S_CSET_D &rData) { *this = rData; }
    S_CSET_D& operator=(const S_CSET_D &rData)
    {
        nActiveStage = rData.nActiveStage;

        aOPolyI.Copy(rData.aOPolyI);
        aIPolyI.Copy(rData.aIPolyI);
        aOPolyQ.Copy(rData.aOPolyQ);
        aIPolyQ.Copy(rData.aIPolyQ);
        aOPolyH.Copy(rData.aOPolyH);
        aIPolyH.Copy(rData.aIPolyH);
        aOPolyT.Copy(rData.aOPolyT);
        aIPolyT.Copy(rData.aIPolyT);
        aOPolyJ.Copy(rData.aOPolyJ);
        aIPolyJ.Copy(rData.aIPolyJ);
        return *this;
    }
};
#define S_CSET_D_LIST CArray<S_CSET_D, S_CSET_D&>

//////////////////////////////////////////////////////////////////////////
// constraint
#define S_CONS_K unsigned int
#define S_CONS_K_LIST CArray<S_CONS_K, S_CONS_K>
struct S_CONS_D
{
    unsigned char sConstraint[6];    // '0'=free '1'=fix
    void Initialize()
    {
        for(int i=0; i<6; i++) sConstraint[i]='0';
    }
    S_CONS_D() {Initialize();}
    S_CONS_D(S_CONS_D &rData) { *this = rData; }
};
#define S_CONS_D_LIST CArray<S_CONS_D, S_CONS_D&>

//////////////////////////////////////////////////////////////////////////
// Load case
#define S_LDCS_K unsigned int // Load Case
#define S_STLD_K unsigned int // Static load case
#define S_SPLC_K unsigned int // Response spectrum load case
#define S_ESPC_K unsigned int // Eccentricity response spectrum load case
#define S_THIS_K unsigned int // Time history
#define S_MVLD_K unsigned int // Moving load case
#define S_SMLC_K unsigned int // Settlement load case
#define S_SGLD_K unsigned int // Construction stage load case
#define S_LDTP_K unsigned int // LoadCase Type

#define S_LDCS_K_LIST CArray<S_LDCS_K, S_LDCS_K>
#define S_STLD_K_LIST CArray<S_STLD_K, S_STLD_K>
#define S_SPLC_K_LIST CArray<S_SPLC_K, S_SPLC_K>
#define S_ESPC_K_LIST CArray<S_ESPC_K, S_ESPC_K>
#define S_THIS_K_LIST CArray<S_THIS_K, S_THIS_K>
#define S_MVLD_K_LIST CArray<S_MVLD_K, S_MVLD_K>
#define S_SMLC_K_LIST CArray<S_SMLC_K, S_SMLC_K>
#define S_SGLD_K_LIST CArray<S_SGLD_K, S_SGLD_K>
#define S_LDTP_K_LIST CArray<S_LDTP_K, S_LDTP_K>

struct S_STLD_D
{
    CString      szLoadCaseName;    // Load case name
    CString      szLoadCaseType;    // dead, live, wind ...
    CString      szDescription;     //
    unsigned int nLoadCaseId;         // ID
    void Initialize()
    {
        szLoadCaseName=_T("");
        szLoadCaseType=_T("");
        szDescription=_T("");
        nLoadCaseId=0;
    }
    S_STLD_D() {Initialize();}
    S_STLD_D(S_STLD_D &rData) { *this = rData; }
};
#define S_STLD_D_LIST CArray<S_STLD_D, S_STLD_D&>

#define DT_LCOM_STATIC       1
#define DT_LCOM_SPECTRUM     2
#define DT_LCOM_HISTORY      3
#define DT_LCOM_MOVING       4
#define DT_LCOM_SETTLEMENT   5
#define DT_LCOM_ENVELOPE     6
#define DT_LCOM_CB_GENERAL   7
#define DT_LCOM_CB_STEEL     8
#define DT_LCOM_CB_CONCRETE  9
#define DT_LCOM_CB_FOOTING   10
#define DT_LCOM_CB_SRC       11
#define DT_LCOM_PUSHOVER     12
#define DT_LCOM_STAGE        13
#define DT_LCOM_ECCENSPEC    14
#define DT_LCOM_GRIDMODEL    15

#define DT_LCOMTYPE_GENERAL     1
#define DT_LCOMTYPE_STEEL       2
#define DT_LCOMTYPE_CONCRETE    3
#define DT_LCOMTYPE_FDN         4
#define DT_LCOMTYPE_SRC         5

#define DT_LCOM_NUMCOMB  150
#define S_LCOM_K unsigned int
#define S_LCOM_T unsigned int
#define S_LCOM_K_LIST CArray<S_LCOM_K, S_LCOM_K>
#define S_LCOM_T_LIST CArray<S_LCOM_T, S_LCOM_T>

struct S_LCOM_BASE
{
    unsigned int nAnalType;     // DT_LCOM_???
    unsigned int nLoadCaseKey;  // Load Case Key
    double       dFactor;       // Factor

    void Initialize()
    {
        nAnalType    = 0;
        nLoadCaseKey = 0;
        dFactor      = 0.0;
    }

    S_LCOM_BASE() {Initialize();}
    S_LCOM_BASE(S_LCOM_BASE &rData) { *this = rData; }
};

struct S_LCOM_D
{
    CString      szLoadCombName;             // Name
    int          nLoadCombType;                // 0=ADD 1=ENVELOPE 2=ABS 3=SRSS
    CString      szDescription;
    int          nActive;                      // 0=inactive, 1=active or strength/stress(CIVIL RC), 2=serviceability(CIVIL RC)
    unsigned int nLoadCombId;
    S_LCOM_BASE  Combination[DT_LCOM_NUMCOMB]; //
    int          nServLcomType;                // Serviceability, 0=Quasi-permanent, 1=Frequent, 2=Characteristic

    void Initialize()
    {
        szLoadCombName = _T("");
        nLoadCombType   = 0;
        szDescription  = _T("");
        nActive         = 1;
        nLoadCombId     = 0;
        for(int i=0; i<DT_LCOM_NUMCOMB; i++)
        {
            Combination[i].Initialize();
        }
        nServLcomType = 0;
    }
    S_LCOM_D() {Initialize();}
    S_LCOM_D(S_LCOM_D &rData) { *this = rData; }

};
#define S_LCOM_D_LIST CArray<S_LCOM_D, S_LCOM_D&>

//////////////////////////////////////////////////////////////////////////
// Design Information
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Common Parameters

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Unbraced Length(L,Lb)
#define S_LENG_K unsigned int
#define S_LENG_K_LIST CArray<S_LENG_K, S_LENG_K>
struct S_LENG_D
{
    double dBLy;        // Ly : y-Unbraced Length
    double dBLz;        // Lz : z-Unbraced Length
    BOOL   bNotUseBLu;  // Do not consider - Laterally Unbraced Length
    double dBLu;        // Laterally Unbraced Length
    BOOL   bAutoCalcLe;  
    void Initialize()
    {
        dBLy        = 0.0;
        dBLz        = 0.0;
        bNotUseBLu  = FALSE;
        dBLu        = 0.0;
        bAutoCalcLe = FALSE;    
    }
    S_LENG_D()  { Initialize(); }
    S_LENG_D(S_LENG_D &rData) {*this = rData;}
};
#define S_LENG_D_LIST CArray<S_LENG_D, S_LENG_D&>

//////////////////////////////////////////////////////////////////////////
// Effective Length Factor(K)
#define S_KFAC_K unsigned int
#define S_KFAC_K_LIST CArray<S_KFAC_K, S_KFAC_K>
struct S_KFAC_D
{
    double dBKy;  // Factor - y
    double dBKz;  // Factor - z
    void Initialize()
    {
        dBKy=1.0;
        dBKz=1.0;
    }
    S_KFAC_D()  { Initialize(); }
    S_KFAC_D(S_KFAC_D &rData) {*this = rData;}
};
#define S_KFAC_D_LIST CArray<S_KFAC_D, S_KFAC_D&>

//////////////////////////////////////////////////////////////////////////
// Limiting Slenderness Ratio
#define S_LTSR_K unsigned int
#define S_LTSR_K_LIST CArray<S_LTSR_K, S_LTSR_K>
struct S_LTSR_D
{
    BOOL   bNotCheck;  // Do not Check
    double dCLMT;      // Compression Limiting Slenderness Ratio
    double dTLMT;      // Tension Limiting Slenderness Ratio
    void Initialize()
    {
        bNotCheck = FALSE;
        dCLMT     = 200.0;
        dTLMT     = 300.0;
    }
    S_LTSR_D()  { Initialize(); }
    S_LTSR_D(S_LTSR_D &rData) {*this = rData;}
};
#define S_LTSR_D_LIST CArray<S_LTSR_D, S_LTSR_D&>

//////////////////////////////////////////////////////////////////////////
// Equivalent Moment Correction Factor(Cm)
#define S_CMFT_K unsigned int
#define S_CMFT_K_LIST CArray<S_CMFT_K, S_CMFT_K>
struct S_CMFT_D
{
    BOOL   bAuto;  // Auto
    double dCmy;   // Cm - y
    double dCmz;   // Cm - z
    void Initialize()
    {
        bAuto = FALSE;
        dCmy  = 0.0;
        dCmz  = 0.0;
    }
    S_CMFT_D()  { Initialize(); }
    S_CMFT_D(S_CMFT_D &rData) {*this = rData;}
};
#define S_CMFT_D_LIST CArray<S_CMFT_D, S_CMFT_D&>

//////////////////////////////////////////////////////////////////////////
// Moment Magnifier
#define S_FMAG_K unsigned int
#define S_FMAG_K_LIST CArray<S_FMAG_K, S_FMAG_K>
struct S_FMAG_D
{
    double dB1y;  // B1y/Delta_by
    double dB1z;  // B1z/Delta_bz
    double dB2y;  // B2y/Delta_sy
    double dB2z;  // B2z/Delta_sz
    void Initialize()
    {
        dB1y=0.0;
        dB1z=0.0;
        dB2y=1.0;
        dB2z=1.0;
    }
    S_FMAG_D()  { Initialize(); }
    S_FMAG_D(S_FMAG_D &rData) {*this = rData;}
};
#define S_FMAG_D_LIST CArray<S_FMAG_D, S_FMAG_D&>

//////////////////////////////////////////////////////////////////////////
// RC Design Parameters

// Design Code
// Strength Reduction Factor
struct S_DCON_D
{
    CString szDesignCode;
    
    double  dPhi[6];         // 0=bending; 1=tension; 2=compression1; 3=compression2; 4=shear;
                                                     // EC2-2:0
                                                     // dPhi[0] : Concrete's Partial Safety Factor in Fundamental state.
                                                     // dPhi[1] : Concrete's Partial Safety Factor in Accidental  state.
                                                     // dPhi[2] : ReinRbar's Partial Safety Factor in Fundamental state.
                                                     // dPhi[3] : ReinRbar's Partial Safety Factor in Accidental  state.
                                                     // dPhi[4] : Concrete's Partial Safety Factor in SLS.              
                                                     // dPhi[5] : ReinRbar's Partial Safety Factor in SLS.              
    double  dRhoc;           // Rebar Ratio, Column Design
    double  dRhor;           // Rebar Ratio, Brace Design
    BOOL    bSpecialSeismic; // Apply Special Provisions for Seismic Design
    double  dMrft;           // Moment Redistribution Factor
    int     nSubCode;        // National Annex, 0:Standard, 1:British(UK), 2:Italian(UNI,IT)
    double  dAlphacc;        // Coefficient for Long Term Effects, 
    double  dMinRhoc;        // Minimum Rebar Ratio
    double  dStrutAngle;     // Strut Angle for Shear Resistance

    void Initialize()
    {
        szDesignCode = _T("");  
        memset(dPhi, 0, sizeof(dPhi)); 
        dRhoc = 0.0;
        dRhor = 0.0;
        bSpecialSeismic = FALSE;
        dMrft = 0.0;    
        nSubCode   = 0;
        dAlphacc   = 0.0;
        dMinRhoc   = 0.0;
        dStrutAngle     = 0.0;
    }
    S_DCON_D()  { Initialize(); }
    S_DCON_D(S_DCON_D &rData) {*this = rData;}
    
};

//////////////////////////////////////////////////////////////////////////
// Modify Concrete Materials

//////////////////////////////////////////////////////////////////////////
//Beam Section Data for Design

#define S_RDBM_K S_SECT_K
#define S_RDBM_K_LIST CArray<S_RDBM_K, S_RDBM_K>
struct S_RDGN_BEAM
{
    double  dDt;          
    double  dDb;
    double  dSubBarNum;    // leg number of Shear rebar
    CString szSubBarNa;  // Rebar Name
    void Initialize()
    {
        dDt        = 0.0;
        dDb        = 0.0;
        dSubBarNum = 2.0;
        szSubBarNa= _T("");
    }
    S_RDGN_BEAM()  { Initialize(); }
    S_RDGN_BEAM(S_RDGN_BEAM &rData) {*this = rData;}
};
#define S_RDBM_D_LIST CArray<S_RDGN_BEAM, S_RDGN_BEAM&>

//////////////////////////////////////////////////////////////////////////
// Column Section Data for Design
#define S_RDCN_K S_SECT_K
#define S_RDCN_K_LIST CArray<S_RDCN_K, S_RDCN_K>
struct S_RDGN_COLM
{
    double  dDc;
    double  dSubBarNum;   // leg number of Shear rebar
    CString szSubBarNa; // Rebar Name
    void Initialize()
    {
        dDc         = 0.0;
        dSubBarNum  = 2.0;    
        szSubBarNa = _T("");
    }
    S_RDGN_COLM()  { Initialize(); }
    S_RDGN_COLM(S_RDGN_COLM &rData) {*this = rData;}
};
#define S_RDCN_D_LIST CArray<S_RDGN_COLM, S_RDGN_COLM&>

//////////////////////////////////////////////////////////////////////////
// Beam Section Data for Checking
#define S_RCBM_K S_SECT_K
#define S_RCBM_K_LIST CArray<S_RCBM_K, S_RCBM_K>
struct S_RCHK_BEAM
{
    double  dDt[3][5];           // Distance from Top. [3]:I,M,J, [5]:Layer
    double  dDb[3][5];           // Distance from Bottom. 
    int     nTopBarNum[3][5];    // Top Rebar Number
    int     nBotBarNum[3][5];    // Bottom Rebar Number
    CString szTopBarNa1[3][5]; // Top Rebar Size Name1
    CString szTopBarNa2[3][5]; // Top Rebar Size Name2
    CString szBotBarNa1[3][5]; // Bottom Rebar Size Name1
    CString szBotBarNa2[3][5]; // Bottom Rebar Size Name2
    double  dSubBarNum[3];       // Shear Rebar Number
    double  dSubBarDist[3];      // Shear Rebar Space
    CString szSubBarNa[3];     // Shear Rebar Size Name
    void Initialize()
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                dDt[i][j] = 0.0;
                dDb[i][j] = 0.0;
                nTopBarNum[i][j] = 0;
                nBotBarNum[i][j] = 0;
                szTopBarNa1[i][j]=_T("");
                szTopBarNa2[i][j]=_T("");
                szBotBarNa1[i][j]=_T("");
                szBotBarNa2[i][j]=_T("");
            }
            dSubBarNum[i] = 2.0;
            dSubBarDist[i] = 0.0;
            szSubBarNa[i]=_T("");
        }
    }
    S_RCHK_BEAM()  { Initialize(); }
    S_RCHK_BEAM(S_RCHK_BEAM &rData) {*this = rData;}
};
#define S_RCBM_D_LIST CArray<S_RCHK_BEAM, S_RCHK_BEAM&>

//////////////////////////////////////////////////////////////////////////
// Column Section Data for Checking
#define S_RCCN_K S_SECT_K
#define S_RCCN_K_LIST CArray<S_RCCN_K, S_RCCN_K>
struct S_RCHK_COLM
{
    double  dDc[5];            // Distance from End, [5]:Layer
    int     nBarNum[3][5];     // Rebar Number. [3]:Rebar position, [5]:Layer
    CString szBarNa1[3][5];  // Rebar Size Name1
    CString szBarNa2[3][5];  // Rebar Size Name2
    double  dSubBarNum;        // Shear Rebar Size
    double  dSubBarDist;       // Shear Rebar Space
    CString szSubBarNa;	     // Shear Rebar Size Name

    void Initialize()
    {
        for (int i = 0; i < 5; i++)
        {
            dDc[i] = 0.0;
            for (int j = 0; j < 3; j++)
            {
                nBarNum[j][i] = 0;
                szBarNa1[j][i]=_T("");
                szBarNa2[j][i]=_T("");
            }
        }
        dSubBarNum  = 2.0;
        dSubBarDist = 0.0;
        szSubBarNa = _T("");
    }
    S_RCHK_COLM()  { Initialize(); }
    S_RCHK_COLM(S_RCHK_COLM &rData) {*this = rData;}
};
#define S_RCCN_D_LIST CArray<S_RCHK_COLM, S_RCHK_COLM&>

//////////////////////////////////////////////////////////////////////////
// Rebar Input for General Section

struct S_REBT_MABR
{
    int     nLabel;         // Id
    double  dCenterY;       // y-position
    double  dCenterZ;       // z-position
    CString szRebarName;  // Rebar Name
    void Initialize()
    {
        nLabel   = 0;
        dCenterY = 0.0;
        dCenterZ = 0.0;
        szRebarName=_T("");
    }
    S_REBT_MABR()  { Initialize(); }
    S_REBT_MABR(S_REBT_MABR &rData) {*this = rData;}
};

struct S_REBT_SHBR
{
 double  dSubRebarSpace;     // Shear Rebar space
 double  dSubRebarNum[2];    // Leg number of Shear rebar
 CString szSubRebarName;   // Rebar Name
 void Initialize()
 {
     dSubRebarSpace=0.0;
     memset(dSubRebarNum, 0, sizeof(dSubRebarNum));
     szSubRebarName=_T("");
 }
 S_REBT_SHBR()  { Initialize(); }
 S_REBT_SHBR(S_REBT_SHBR &rData) {*this = rData;}
};

#define S_REBT_K S_SECT_K
#define S_REBT_K_LIST CArray<S_REBT_K, S_REBT_K>
struct S_REBT_D
{
    int nCenterType;                               // Center of Point (0:centroid, 1:Bottom Left) (default = 0)
    CArray<S_REBT_MABR, S_REBT_MABR&> aMainRebar;  // Longitudinal Rebar
    S_REBT_SHBR ShearRebar;                        // Hoop Rebar
    void Initialize()
    {
        nCenterType=0;
        aMainRebar.RemoveAll(); 
        ShearRebar.Initialize();
    }
    S_REBT_D() {Initialize();}
    S_REBT_D(const S_REBT_D &rData) { *this = rData; }
    S_REBT_D& operator=(const S_REBT_D &rData)
    {
        nCenterType=rData.nCenterType;
        aMainRebar.Copy(rData.aMainRebar);
        ShearRebar = rData.ShearRebar;
        return *this;
    }
};
#define S_REBT_D_LIST CArray<S_REBT_D, S_REBT_D&>

// Rebar for PSC
struct S_POINT_SSM
{
    double dPointY;    // y-position
    double dPointZ;    // z-position

    void Initialize()
    {
        dPointY = 0.0;
        dPointZ = 0.0;
    }
 S_POINT_SSM()  { Initialize(); }
 S_POINT_SSM(S_POINT_SSM &rData) {*this = rData;}
};

struct S_RPSC_MBAR_SSM   // Longitudinal Reinforcement
{
    int         nType;          // 0:Point 1:Line 2:Arc 3:Circle 4:Poly Line
    int         nRefY;          // 0:Left 1:Center 2:Right (for Tapered) 
    int         nRefZ;          // 0:Top 1:Center 2:Bottom (for Tapered)
    S_POINT_SSM Start;          // Start Point
    S_POINT_SSM Center;         // Center Point
    CArray<S_POINT_SSM, S_POINT_SSM&> aPassage; // Passage Point(y,z)
    S_POINT_SSM End;            // End Point
    double      dRadius;        // Radius
    int         nNumorCTC;      // 0:Num. 1:CTC
    int         nNum;           // Number
    double      dCTC;           // CTC 
    BOOL        bEdgeBar;       // Edge Rebar
    CString     szBar;        // Rebar Name
    int         nStandRef;      // 0:Start 1:Center 2:End(for CTC input)
    int         nPart;          // 0:No Part
                                                            // 1:Part1 of Composite section
                                                            // 2:Part2 of Composite section
    int         nRefY_A;        // 0:Centroid 1:Left(for nType : 1Line)
	int         nRefZ_A;        // 0:Top 1:Bottom(for nType : 1Line)
	double      dRefY_A;
	double      dRefZ_A;
	double      dSpacing;

    void Initialize()
    {
        nType = 1;
        nRefY = 0;
        nRefZ = 0;
        Start.Initialize();
        Center.Initialize();
        aPassage.RemoveAll();
        End.Initialize();
        dRadius = 0.;
        nNumorCTC = 0;
        nNum = 0;
        dCTC = 0.;
        bEdgeBar = FALSE;
        szBar = _T("");
        nStandRef = 0;
        nPart = 0;

		nRefY_A = 0;
		nRefZ_A = 0;
		dRefY_A = 0.0;
		dRefZ_A = 0.0;
		dSpacing = 0.0;
    }

    S_RPSC_MBAR_SSM() {Initialize();}
    S_RPSC_MBAR_SSM(const S_RPSC_MBAR_SSM& rData) { *this = rData; }
    S_RPSC_MBAR_SSM& operator=(const S_RPSC_MBAR_SSM& rData) 
    {
        nType = rData.nType;
        nRefY = rData.nRefY;
        nRefZ = rData.nRefZ;
        Start = rData.Start;
        Center = rData.Center;
        aPassage.Copy(rData.aPassage);
        End = rData.End;
        dRadius = rData.dRadius;
        nNumorCTC = rData.nNumorCTC;
        nNum = rData.nNum;
        dCTC = rData.dCTC;
        bEdgeBar = rData.bEdgeBar;
        szBar = rData.szBar;
        nStandRef = rData.nStandRef;
        nPart = rData.nPart;

		nRefY_A = rData.nRefY_A;
		nRefZ_A = rData.nRefZ_A;
		dRefY_A = rData.dRefY_A;
		dRefZ_A = rData.dRefZ_A;
		dSpacing = rData.dSpacing;

        return (*this);
    }
};

struct S_RPSC_SBAR_SSM     
{
    BOOL   bDiagonal;        // Diagonal Rebar
    double dDiagonalPitch;   // Diagonal Rebar Pitch
    double dDiagonalAngle;   // Diagonal Rebar Angle
    double dDiagonalAw;      // Diagonal Rebar Area
    BOOL   bSteelBar;        // Steel bar for Web
    double dSteelBarPitch;   // Steel bar Pitch
    double dSteelBarAngle;   // Steel bar Angle
    double dSteelBarAp;      // Steel bar Area
    double dSteelBarPe;      // Steel bar Effective tensile force
    BOOL   bTorsional;       // Torsional Rebar
    double dTorsionalPitch;  // Torsional Rebar Pitch
    double dTorsionalAwt;    // Transverse Rebar Area(for Torsion) 
    double dTorsionalAlt;    // Transverse Rebar Total Area
    BOOL   bLbarFlg;         // Enclosing Stirrup
    double dLbarThick;       // Cover Thickness
    BOOL   bIncFC;           // Include Flange/Cantilever

    void Initialize()
    {
        bDiagonal = FALSE;
        dDiagonalPitch  = 0.0;
        dDiagonalAngle  = 0.0;
        dDiagonalAw     = 0.0;
        bSteelBar = FALSE;
        dSteelBarPitch  = 0.0;
        dSteelBarAngle  = 90.0;
        dSteelBarAp     = 0.0;
        dSteelBarPe     = 0.0;
        bTorsional = FALSE;
        dTorsionalPitch = 0.0;
        dTorsionalAwt   = 0.0;
        dTorsionalAlt   = 0.0;
        bLbarFlg = FALSE;
        dLbarThick = 0.0;
        bIncFC = FALSE;
    }
    S_RPSC_SBAR_SSM()  { Initialize(); }
    S_RPSC_SBAR_SSM(S_RPSC_SBAR_SSM &rData) {*this = rData;}
};

#define S_RPSC_K S_SECT_K
#define S_RPSC_K_LIST CArray<S_RPSC_K, S_RPSC_K>
struct S_RPSC_SSM_D
{
    CArray<S_RPSC_MBAR_SSM, S_RPSC_MBAR_SSM&> aMbar; // Longitudinal Rebar
    S_RPSC_SBAR_SSM Sbar;                            // Shear Rebar
    BOOL bCracked;                                   // Cracked Section(for Composite Section)

    void Initialize()
    {
        aMbar.RemoveAll();
        Sbar;
        bCracked = FALSE;
    }

    S_RPSC_SSM_D() {Initialize();}
    S_RPSC_SSM_D(const S_RPSC_SSM_D& rData) { *this = rData; }
    S_RPSC_SSM_D& operator=(const S_RPSC_SSM_D& rData)
    {
        aMbar.Copy(rData.aMbar);
        Sbar = rData.Sbar;
        bCracked = rData.bCracked;
        return *this;
    }
};
#define S_RPSC_D_LIST CArray<S_RPSC_SSM_D, S_RPSC_SSM_D&>

// Tendon parameters
struct S_SECG_TNDN
{
    BOOL    bPrestressed;
    int     nTendonType;  // 1:Internal, 2:External
    int     nLoadType;    // 0=Pre-tension, 1=Post-tension  
    int     nBondType;    // 0=Bonded, 1=Unbonded
    double  dDia;         // Tendon Diameter 
    double  dDuctDia;     // Duct Diameter
	double  dAp;			    // Tendon Area
	double  dyz[2];	      // Tendon Position(0=y,1=z), Centroid or Origin
	double  dEp;			    // Elastic modulus
	double  dDp;			    // Weight density
    double  dPp;          // Poisson's ratio
    CString szTendonName; // Tendon Profile Name V.101
    UINT    nTendonKey;   // Tendon Profile Key V.201
	void Initialize()
	{
        bPrestressed=FALSE;
        nTendonType = 0;
        nLoadType = 0;
        nBondType = 0;
        dDia=0.0;
        dDuctDia=0.0;
		dAp=0.0;
		memset(dyz, 0, sizeof(dyz));
		dEp=0.0;
		dDp=0.0;
        dPp=0.0;
        szTendonName = _T("");
        nTendonKey = 0;
	}
    S_SECG_TNDN () { Initialize(); }
    S_SECG_TNDN(S_SECG_TNDN &rData) {*this = rData;}
};

// Tendon of Cross section
#define S_TDIJ_K S_SECT_K
#define S_TDIF_K S_SECT_K
#define S_TDIJ_K_LIST CArray<S_TDIJ_K, S_TDIJ_K>
#define S_TDIF_K_LIST CArray<S_TDIF_K, S_TDIF_K>
struct S_TDIF_D
{
    CArray<S_SECG_TNDN, S_SECG_TNDN&> aTndn;  // Tendon
    void Initialize()
    {
        aTndn.RemoveAll();
    }
    S_TDIF_D() {Initialize();}
    S_TDIF_D(const S_TDIF_D &rData) { *this = rData; }
    S_TDIF_D& operator=(const S_TDIF_D &rData)
    {
        aTndn.Copy(rData.aTndn);
        return *this;
    }
};
#define S_TDIF_D_LIST CArray<S_TDIF_D, S_TDIF_D&>

// Tendon Time-Dependent Loss Graph
#define S_TDPF_K UINT
#define S_STEP_K UINT
#define S_TDPF_K_LIST CArray<S_TDPF_K, S_TDPF_K>
#define S_STEP_K_LIST CArray<S_STEP_K, S_STEP_K>
struct S_TDLS_D
{
    CArray <double,double> aTendonPos;
    CArray <double,double> aTendonFrc;
    void Initialize()
    {
        aTendonPos.RemoveAll();
        aTendonFrc.RemoveAll();
    }
    S_TDLS_D() {Initialize();}
    S_TDLS_D(const S_TDLS_D &rData) { *this = rData; }
    S_TDLS_D& operator=(const S_TDLS_D &rData)
    {
        aTendonPos.Copy(rData.aTendonPos);
        aTendonFrc.Copy(rData.aTendonFrc);    
        return *this;
    }
};
#define S_TDLS_D_LIST CArray<S_TDLS_D, S_TDLS_D&>

//// v201. Add Construnction Stage Information
// for 2D Element
#define S_THIK_K unsigned int
#define S_THIK_K_LIST CArray<S_THIK_K, S_THIK_K>
struct S_THIK_D
{ 
    int     nType;          // 0=Value, 1=Stiffened (T_THKS_K == T_THIK_K)
    // T_THIK_VALUE
    int     nThickType;  // 0=In&Out , 1=In,Out 따로
    double  ThickIn;     // L, nThickType = 0 In/Out공용으로 사용
    double  ThickOut;    // L, nThickType = 0 사용안됨  
    BOOL    bOffset;
    int     nOffsetType; // 0:ratio, 1:value
    double  dValue;
    double  dRatio;

    // T_THIK_STIFFENED
    int     nStifType;
    CString szDBName;
    double  dPlateThik;
    double  dWeightThik;
    int     nRibPos;

    // Stiff-Value
    CString szStVShape[2];
    double  dThickIn[2];
    double  dThickOut[2];
    double  dHu[2];
    double  dHl[2];
    // Stiff-Sect
    BOOL    bRibAttach[2];
    CString szStSShape[2];
    CString szName[2];
    double  dDist[2];
    double  dSize[2][10];
    double  dPeriIn[2];
    double  dPeriOut[2];

    void Initialize()
    { 
        nType = 0;

        // Value
        nThickType  = 0;
        ThickIn     = 0.;
        ThickOut    = 0.;
        bOffset     = FALSE;
        nOffsetType = 0;
        dValue      = 0.;
        dRatio      = .5;    

        // Stiffened
        nStifType   = 0;
        szDBName.Empty();
        dPlateThik  = 0.;
        dWeightThik = 0.;
        nRibPos = 0;

        for (int i=0; i<2; i++)
        {
            szStVShape[i].Empty();
            dThickIn[i] = 0.0;
            dThickOut[i] = 0.0;
            dHu[i] = 0.0;
            dHl[i] = 0.0;

            bRibAttach[i] = FALSE;
            szStSShape[i].Empty();
            szName[i].Empty();
            dDist[i] = 0.0;
            memset(dSize[i], 0, sizeof(dSize[i]));
            dPeriIn[i] = 0.0;
            dPeriOut[i] = 0.0;
        }
    }
    S_THIK_D() {Initialize();}
    S_THIK_D(const S_THIK_D &rData) { *this = rData; }
    S_THIK_D& operator=(const S_THIK_D &rData)
    {
        nType       = rData.nType;
        nThickType  = rData.nThickType;
        ThickIn     = rData.ThickIn;
        ThickOut    = rData.ThickOut;
        bOffset     = rData.bOffset;
        nOffsetType = rData.nOffsetType;
        dValue      = rData.dValue;
        dRatio      = rData.dRatio;

        nStifType   = rData.nStifType;
        szDBName    = rData.szDBName;
        dPlateThik  = rData.dPlateThik;
        dWeightThik = rData.dWeightThik;
        nRibPos     = rData.nRibPos;

        for (int i=0; i<2; i++)
        {
            szStVShape[i]   = rData.szStVShape[i];
            dThickIn[i]  = rData.dThickIn[i];
            dThickOut[i] = rData.dThickOut[i];
            dHu[i]       = rData.dHu[i];
            dHl[i]       = rData.dHl[i];
            
            bRibAttach[i] = rData.bRibAttach[i];
            szStSShape[i]    = rData.szStSShape[i];
            szName[i]     = rData.szName[i];
            dDist[i]      = rData.dDist[i];
            memcpy(dSize[i], rData.dSize[i], sizeof(dSize[i]));
            dPeriIn[i]    = rData.dPeriIn[i];
            dPeriOut[i]   = rData.dPeriOut[i];
        }
        return *this;
    }
};
#define S_THIK_D_LIST CArray<S_THIK_D, S_THIK_D&>

struct S_STRP_DL
{
    double dblStress[5][8];
    void Initialize()
    {
        memset(this, 0, sizeof(S_STRP_DL));
    }
    S_STRP_DL() {Initialize();}
    S_STRP_DL(const S_STRP_DL &rData) { *this = rData; }
    S_STRP_DL& operator=(const S_STRP_DL &rData)
    {
        memcpy(this->dblStress, rData.dblStress, sizeof(this->dblStress));
        return *this;
    }
};
#define S_STRP_DL_LIST CArray<S_STRP_DL, S_STRP_DL&>

struct S_STRP_CSDL
{
    CArray<S_STRP_DL, S_STRP_DL&> aSTRP;
    void Initialize()
    {
        aSTRP.RemoveAll();
    }
    S_STRP_CSDL() {Initialize();}
    S_STRP_CSDL(S_STRP_CSDL &rData) { *this = rData; }
    S_STRP_CSDL& operator=(S_STRP_CSDL& src)
    {
        aSTRP.Copy(src.aSTRP);
        return *this;
    }
};
#define S_STRP_CSDL_LIST CArray<S_STRP_CSDL, S_STRP_CSDL&>


// for Construction Stage
#define S_GRUP_K unsigned int
#define S_BNGR_K unsigned int
#define S_LDGR_K unsigned int
#define S_STAG_K unsigned int
#define S_STAG_K_LIST CArray<S_STAG_K, S_STAG_K>

struct S_STAG_AELEM_BASE
{
    S_GRUP_K GrupKey;
    double   dAge;      // Stage 시작 당시의 나이(단위:day)
    void Initialize()
    {
        GrupKey = 0;
        dAge = 0.0;
    }
};
struct S_STAG_DELEM_BASE
{
    S_GRUP_K GrupKey;
    double   dRedistribution;
    void Initialize()
    {
        GrupKey = 0;
        dRedistribution = 0.0;
    }
};
struct S_STAG_BNDR_BASE
{
    S_BNGR_K BngrKey;
    int      nPosition; // 1:Original Position, 2:Deformed Position
    void Initialize()
    {
        BngrKey = 0;
        nPosition = 2;
    }
};
struct S_STAG_LOAD_BASE
{
    S_LDGR_K LdgrKey;
    double   dDay;    // 하중이 재하되는 시점 (0.0 First, -1.0:Last, etc:User-Step)
    void Initialize()
    {
        LdgrKey = 0;
        dDay = 0.0;
    }
};

struct S_STAG_D
{
    CString szStageName;
    double  dDuration;
    CArray<double, double> aUserDefStep;
    CArray<S_STAG_AELEM_BASE, S_STAG_AELEM_BASE&> aActiveElem;
    CArray<S_STAG_DELEM_BASE, S_STAG_DELEM_BASE&> aDeactiveElem;
    CArray<S_STAG_BNDR_BASE,  S_STAG_BNDR_BASE&> aActiveBndr;
    CArray<S_BNGR_K, S_BNGR_K> aDeactiveBndr;
    CArray<S_STAG_LOAD_BASE,  S_STAG_LOAD_BASE&> aActiveLoad;
    CArray<S_STAG_LOAD_BASE,  S_STAG_LOAD_BASE&> aDeactiveLoad;
    unsigned int nStageId;
    void Initialize()
    {
        szStageName.Empty();
        dDuration = 0.0;
        aUserDefStep.RemoveAll();
        aActiveElem.RemoveAll();
        aDeactiveElem.RemoveAll();
        aActiveBndr.RemoveAll();
        aDeactiveBndr.RemoveAll();
        aActiveLoad.RemoveAll();
        aDeactiveLoad.RemoveAll();
        nStageId=0;
    }
    S_STAG_D()
    {
    }
    S_STAG_D(S_STAG_D& src)
    {
        *this = src;
    }
    S_STAG_D& operator=(S_STAG_D& src)
    {
        szStageName = src.szStageName;
        dDuration = src.dDuration;
        aUserDefStep.Copy(src.aUserDefStep);
        aActiveElem.Copy(src.aActiveElem);
        aDeactiveElem.Copy(src.aDeactiveElem);
        aActiveBndr.Copy(src.aActiveBndr);
        aDeactiveBndr.Copy(src.aDeactiveBndr);
        aActiveLoad.Copy(src.aActiveLoad);
        aDeactiveLoad.Copy(src.aDeactiveLoad);
        nStageId = src.nStageId;
        return *this;
    }
};
#define S_STAG_D_LIST CArray<S_STAG_D, S_STAG_D&>

// Composite Section for Construction Stage
#define S_CSCS_K unsigned int
#define S_CSCS_K_LIST CArray<S_CSCS_K, S_CSCS_K>
struct S_CSCS_BASE
{
    int    nPart;
    UINT   nMatlKey;       // 0:Element, else Material No
    UINT   nCompStagKey;   // 0:Active Stage, else Stage Key
    double dAge;  
    S_SECT_STIFFNESS StiffScale;  // 강성 Scale Factor
    S_SECT_STIFFNESS Stiffness;   // Part별 강성
    S_SECT_STIFFNESS StiffnessJ;  // Part별 강성 (Tapered일 때 J단면 강성)
    double YBar, ZBar;            // Part별 도심
    double YBarJ, ZBarJ;          // Part별 도심 (Tapered일 때 J단면 도심)
    double dH;                    // 730 추가
    void Initialize()
    {
        nPart = 0;
        nMatlKey = 0;
        nCompStagKey = 0;
        dAge = 0.0;
        StiffScale.Initialize();
        Stiffness.Initialize();
        StiffnessJ.Initialize();
        YBar = ZBar = 0.0;
        YBarJ = ZBarJ = 0.0;
        dH = 0.0;
    }
    S_CSCS_BASE(){}
    S_CSCS_BASE(const S_CSCS_BASE& src) { *this = src; }
    S_CSCS_BASE& operator=(const S_CSCS_BASE& rData)
    {
        nPart = rData.nPart;
        nMatlKey = rData.nMatlKey;
        nCompStagKey = rData.nCompStagKey;
        dAge = rData.dAge;
        StiffScale = rData.StiffScale;
        Stiffness = rData.Stiffness;
        StiffnessJ = rData.StiffnessJ;
        YBar = rData.YBar;
        ZBar = rData.ZBar;
        YBarJ = rData.YBarJ;
        ZBarJ = rData.ZBarJ;
        dH = rData.dH;
        return *this;
    }  
};

struct S_CSCS_D
{
    UINT  nSectKey;
    UINT  nActStagKey;
    int   nCompType;  // -2:Normal(0, 1제외) -1:User Type, 0: 1&2 + 3, 1: 1 + 2&3,
    CArray<S_CSCS_BASE, S_CSCS_BASE&> aPart;
    S_CSCS_D(){}
    S_CSCS_D(const S_CSCS_D& src) { *this = src; }
    S_CSCS_D& operator=(const S_CSCS_D& rData)
    {
        nSectKey = rData.nSectKey;
        nActStagKey = rData.nActStagKey;
        nCompType = rData.nCompType;
        aPart.Copy(rData.aPart);
        return *this;
    }
    void Initialize()
    {
        nSectKey = 0;
        nActStagKey = 0;
        nCompType = 0;
        aPart.RemoveAll();
    }
};
#define S_CSCS_D_LIST CArray<S_CSCS_D, S_CSCS_D&>

// Element Group
#define S_GRUP_K_LIST CArray<S_GRUP_K, S_GRUP_K>
struct S_GRUP_D
{
    CString szGroupName;
    CArray<S_NODE_K, S_NODE_K>arKeyNode;
    CArray<S_ELEM_K, S_ELEM_K>arKeyElem;
    unsigned int nGroupId;
    void Initialize()
    {
        szGroupName.Empty();
        arKeyNode.RemoveAll();
        arKeyElem.RemoveAll();
        nGroupId=0;
    }
    S_GRUP_D()
    {
        Initialize();
    }
    S_GRUP_D(S_GRUP_D& src)
    {
        *this = src;
    }
    S_GRUP_D& operator=(S_GRUP_D& src)
    {
        szGroupName=src.szGroupName;
        arKeyNode.Copy(src.arKeyNode);
        arKeyElem.Copy(src.arKeyElem);
        nGroupId=src.nGroupId;
        return *this;
    }
};
#define S_GRUP_D_LIST CArray<S_GRUP_D, S_GRUP_D&>

// Boundary Group
#define S_BNGR_K_LIST CArray<S_BNGR_K, S_BNGR_K>
struct S_BNGR_D
{
    CString szGroupName;
    int     nGroupId;

    S_BNGR_D() { Initialize(); }
    void Initialize()
    {
        szGroupName.Empty();
        nGroupId = 0;
    }
};
#define S_BNGR_D_LIST CArray<S_BNGR_D, S_BNGR_D&>

// Load Group
#define S_LDGR_K_LIST CArray<S_LDGR_K, S_LDGR_K>
struct S_LDGR_D
{
    CString szGroupName;
    int     nGroupId;
    void Initialize()
    {
        szGroupName.Empty();
        nGroupId = 0;
    }
};
#define S_LDGR_D_LIST CArray<S_LDGR_D, S_LDGR_D&>

//////////////////////////////////////////////////////////////////////////
// Analysis Result
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Reaction
#define S_REAC_K S_NODE_K
struct S_REAC_D
{
    double dblReac[6];  // 0=Fx, 1=Fy, 2=Fz, 3=Mx, 4=My, 5=Mz
    void Initialize()
    {
        memset(dblReac, 0, sizeof(dblReac));
    }
    S_REAC_D() {Initialize();}
    S_REAC_D(S_REAC_D &rData) { *this = rData; }
};
#define S_REAC_D_LIST CArray<S_REAC_D, S_REAC_D&>
struct S_RECC_D
{
    double dblReac[12];  // 0=Fx-max, 1=Fy-max, 2=Fz-max, 3=Mx-max, 4=My-max, 5=Mz-max, 6=Fx-min, 7=Fy-min, 8=Fz-min, 9=Mx-min, 10=My-min, 11=Mz-min
    void Initialize()
    {
        memset(dblReac, 0, sizeof(dblReac));
    }
    S_RECC_D() {Initialize();}
    S_RECC_D(S_RECC_D &rData) { *this = rData; }
};
#define S_RECC_D_LIST CArray<S_RECC_D, S_RECC_D&>

//////////////////////////////////////////////////////////////////////////
// Displacement
#define S_DISP_K S_NODE_K
struct S_DISP_D
{
    double dblDisp[6];  // 0=Dx, 1=Dy, 2=Dz, 3=Rx, 4=Ry, 5=Rz
    void Initialize()
    {
        memset(dblDisp, 0, sizeof(dblDisp));
    }
    S_DISP_D() {Initialize();}
    S_DISP_D(S_DISP_D &rData) { *this = rData; }
};
#define S_DISP_D_LIST CArray<S_DISP_D, S_DISP_D&>

struct S_DISP_CSD
{
    CArray<S_DISP_D, S_DISP_D&> aDisp;
    void Initialize()
    {
        aDisp.RemoveAll();
    }
    S_DISP_CSD() {Initialize();}
    S_DISP_CSD(S_DISP_CSD &rData) { *this = rData; }
    S_DISP_CSD& operator=(S_DISP_CSD& src)
    {
        aDisp.Copy(src.aDisp);
        return *this;
    }
};
#define S_DISP_CSD_LIST CArray<S_DISP_CSD, S_DISP_CSD&>

struct S_DSPC_D
{
    double dblDisp[12];  // 0=Dx-min, 1=Dy-min, 2=Dz-min, 3=Rx-min, 4=Ry-min, 5=Rz-min, 6=Dx-min, 7=Dy-min, 8=Dz-min, 9=Rx-min, 10=Ry-min, 11=Rz-min
    void Initialize()
    {
        memset(dblDisp, 0, sizeof(dblDisp));
    }
    S_DSPC_D() {Initialize();}
    S_DSPC_D(S_DSPC_D &rData) { *this = rData; }
};
#define S_DSPC_D_LIST CArray<S_DSPC_D, S_DSPC_D&>

//////////////////////////////////////////////////////////////////////////
// Truss Force

#define S_TFOR_K S_ELEM_K
struct S_TFOR_D
{
    double dblForce[2]; // component : Fxi, Fxj
    void Initialize()
    {
        memset(dblForce, 0, sizeof(dblForce));
    }
    S_TFOR_D() {Initialize();}
    S_TFOR_D(S_TFOR_D &rData) { *this = rData; }
};
#define S_TFOR_D_LIST CArray<S_TFOR_D, S_TFOR_D&>

struct S_TFOR_CSD
{
    CArray<S_TFOR_D, S_TFOR_D&> aTFOR;
    void Initialize()
    {
        aTFOR.RemoveAll();
    }
    S_TFOR_CSD() {Initialize();}
    S_TFOR_CSD(S_TFOR_CSD &rData) { *this = rData; }
    S_TFOR_CSD& operator=(S_TFOR_CSD& src)
    {
        aTFOR.Copy(src.aTFOR);
        return *this;
    }
};
#define S_TFOR_CSD_LIST CArray<S_TFOR_CSD, S_TFOR_CSD&>


//////////////////////////////////////////////////////////////////////////
// Beam Force

#define S_BFOR_K S_ELEM_K
struct S_BFOR_D
{
    double  dblForce[4][2][6]; // [4] = 4 part / [2] = i,j / [6] = Fxi, Fyi, Fzi, Mxi, Myi, Mzi
    void Initialize()
    {
        memset(dblForce, 0, sizeof(dblForce));
    }
    S_BFOR_D() {Initialize();}
};
#define S_BFOR_D_LIST CArray<S_BFOR_D, S_BFOR_D&>

struct S_BFOR_CSD
{
    CArray<S_BFOR_D, S_BFOR_D&> aBFOR;
    void Initialize()
    {
        aBFOR.RemoveAll();
    }
    S_BFOR_CSD() {Initialize();}
    S_BFOR_CSD& operator=(S_BFOR_CSD& src)
    {
        aBFOR.Copy(src.aBFOR);
        return *this;
    }
};
#define S_BFOR_CSD_LIST CArray<S_BFOR_CSD, S_BFOR_CSD&>

struct S_BFOR_PT
{
    CArray<S_BFOR_D, S_BFOR_D&> aBFOR;
    void Initialize()
    {
        aBFOR.RemoveAll();
    }
    S_BFOR_PT() {Initialize();}  
    S_BFOR_PT& operator=(S_BFOR_PT& src)
    {
        aBFOR.Copy(src.aBFOR);
        return *this;
    }
};
#define S_BFOR_PT_LIST CArray<S_BFOR_PT, S_BFOR_PT&>

struct S_BFOR_PTCS
{
    CArray<S_BFOR_PT, S_BFOR_PT&> aBFORPT;
    void Initialize()
    {
        aBFORPT.RemoveAll();
    }
    explicit S_BFOR_PTCS() {Initialize();}  
    S_BFOR_PTCS& operator=(S_BFOR_PTCS& src)
    {
        int nSize = src.aBFORPT.GetSize();
        aBFORPT.SetSize(nSize);
        for (int i=0; i<nSize; ++i)
        {
            aBFORPT[i] = src.aBFORPT[i];
        }    
        return *this;
    }
};
#define S_BFOR_PTCS_LIST CArray<S_BFOR_PTCS, S_BFOR_PTCS&>

// Concurrent Force
#define S_BCFO_K S_ELEM_K
struct S_BCFO_B
{  
    double dblForce[4][2][12][6]; // [4] = 4 part / [2] = i,j
                                                                // [12] = Concurrent Force Type : Fx-min, Fy-min, Fz-min, Mx-min, My-min, Mz-min,
                                                                //                                Fx-min, Fy-min, Fz-min, Mx-min, My-min, Mz-min
                                                                // [6] = Force : Fx, Fy, Fz, Mx, My, Mz
    void Initialize()
    {
        memset(this, 0, sizeof(dblForce));
    }
    S_BCFO_B() {Initialize();}
    S_BCFO_B(S_BCFO_B &rData) { *this = rData; }
};
#define S_BCFO_B_LIST CArray<S_BCFO_B, S_BCFO_B&>

struct S_BFOR_PTCC
{
    CArray<S_BCFO_B, S_BCFO_B&> aBFOR;
    void Initialize()
    {
        aBFOR.RemoveAll();
    }
    S_BFOR_PTCC() {Initialize();}  
    S_BFOR_PTCC& operator=(S_BFOR_PTCC& src)
    {
        aBFOR.Copy(src.aBFOR);
        return *this;
    }
};
#define S_BFOR_PTCC_LIST CArray<S_BFOR_PTCC, S_BFOR_PTCC&>

#pragma pack(pop)

#endif  // __DB_ST_DT_EXPORT_D_H__
