
import bannerLaunch from '../assets/images/marketplace_banner_launch_1767449477337.png';
import bannerAI from '../assets/images/marketplace_banner_ai_1767449492493.png';
import bannerWorkflow from '../assets/images/marketplace_banner_workflow_1767449753122.png';
import bannerCommunity from '../assets/images/marketplace_banner_community_1767449771620.png';
import bannerPremium from '../assets/images/marketplace_banner_premium_1767449787718.png';
// Nanobanana avatar is still imported but assigned to a real name
import avatarNanobanana from '../assets/images/avatar_nanobanana_1767449510030.png';

// Import Thumbnail Images
import thumbStructural from '../assets/images/thumb_structural_analysis_1767450177660.png';
// thumbDataViz is removed
import thumbBridge from '../assets/images/thumb_bridge_design_1767452337646.png'; // New import
import thumbPdf from '../assets/images/thumb_pdf_exporter_1767450207076.png';
import thumbSeismic from '../assets/images/thumb_seismic_workflow_1767450248661.png';
import thumbWind from '../assets/images/thumb_wind_load_1767450306515.png';
import thumbBIM from '../assets/images/thumb_bim_connector_1767450334194.png';
import thumbRCBeam from '../assets/images/thumb_rc_beam_workflow.png';
import thumbSteel from '../assets/images/thumb_steel_connection_1767450906253.png';
import thumbRetainingWall from '../assets/images/thumb_retaining_wall_1767450921992.png';
import thumbUndergroundBox from '../assets/images/thumb_underground_box_1767451524019.png';


export interface MarketplaceItem {
    id: string;
    title: string;
    description: string;
    thumbnailUrl?: string; // Optional, use placeholders if missing
    type: 'component' | 'workflow' | 'automation_request';
    price: number; // 0 for free
    currency: string;
    rating: number;
    reviewCount: number;
    publisher: {
        name: string;
        avatarUrl?: string;
        isVerified: boolean;
    };
    category: string;
}

export interface BannerItem {
    id: string;
    imageUrl: string;
    title: string;
    subtitle: string;
    linkUrl: string;
}

export const banners: BannerItem[] = [
    {
        id: '1',
        imageUrl: bannerLaunch,
        title: "Grand Opening",
        subtitle: "Explore the new Marketplace for components & workflows.",
        linkUrl: "#"
    },
    {
        id: '2',
        imageUrl: bannerAI,
        title: "AI Power Week",
        subtitle: "50% off on all AI-related automation tools.",
        linkUrl: "#"
    },
    {
        id: '3',
        imageUrl: bannerWorkflow,
        title: "Automate Everything",
        subtitle: "Discover new workflows to streamline your engineering tasks.",
        linkUrl: "#"
    },
    {
        id: '4',
        imageUrl: bannerCommunity,
        title: "Join the Global Community",
        subtitle: "Connect with developers and engineers worldwide.",
        linkUrl: "#"
    },
    {
        id: '5',
        imageUrl: bannerPremium,
        title: "Premium Enterprise Services",
        subtitle: "Exclusive tools and support for enterprise clients.",
        linkUrl: "#"
    }
];

export const freeItems: MarketplaceItem[] = [
    {
        id: 'free-new-1',
        title: "Underground Box Structure",
        description: "Automated analysis for culverts and underground boxes.",
        thumbnailUrl: thumbUndergroundBox,
        type: 'component',
        price: 0,
        currency: "USD",
        rating: 4.9,
        reviewCount: 72,
        publisher: {
            name: "Dr. Park",
            avatarUrl: avatarNanobanana,
            isVerified: true
        },
        category: "Civil"
    },
    {
        id: 'free-1',
        title: "Structural Analysis Pro",
        description: "Advanced beam and column analysis component.",
        thumbnailUrl: thumbStructural,
        type: 'component',
        price: 0,
        currency: "USD",
        rating: 4.8,
        reviewCount: 124,
        publisher: {
            name: "Eng. Kim",
            avatarUrl: avatarNanobanana,
            isVerified: true
        },
        category: "Engineering"
    },
    {
        id: 'free-2',
        title: "Bridge Design Suite",
        description: "Cable-stayed and suspension bridge analysis tools.",
        thumbnailUrl: thumbBridge,
        type: 'component',
        price: 0,
        currency: "USD",
        rating: 4.8,
        reviewCount: 95,
        publisher: {
            name: "BridgeMaster",
            isVerified: true
        },
        category: "Structural"
    },
    {
        id: 'free-3',
        title: "PDF Exporter",
        description: "One-click PDF generation for workflows.",
        thumbnailUrl: thumbPdf,
        type: 'component',
        price: 0,
        currency: "USD",
        rating: 4.2,
        reviewCount: 45,
        publisher: {
            name: "DevCommunity",
            isVerified: false
        },
        category: "Utility"
    },
    {
        id: 'free-4',
        title: "Unit Converter Plus",
        description: "Comprehensive unit conversion for all engineering fields.",
        type: 'component',
        price: 0,
        currency: "USD",
        rating: 4.0,
        reviewCount: 22,
        publisher: {
            name: "EngineerX",
            isVerified: false
        },
        category: "Utility"
    },
    {
        id: 'free-5',
        title: "Material Library Standard",
        description: "Database of common building materials.",
        type: 'component',
        price: 0,
        currency: "USD",
        rating: 4.6,
        reviewCount: 56,
        publisher: {
            name: "MaterialLabs",
            isVerified: true
        },
        category: "Database"
    }
];

export const paidItems: MarketplaceItem[] = [
    {
        id: 'paid-1',
        title: "Seismic Design Workflow",
        description: "Complete automated workflow for seismic compliance checks.",
        thumbnailUrl: thumbSeismic,
        type: 'workflow',
        price: 49.99,
        currency: "USD",
        rating: 5.0,
        reviewCount: 12,
        publisher: {
            name: "Prof. Lee",
            avatarUrl: avatarNanobanana,
            isVerified: true
        },
        category: "Workflow"
    },
    {
        id: 'paid-2',
        title: "RC Beam Design Automation",
        description: "Automated reinforced concrete beam design according to KDS code.",
        thumbnailUrl: thumbRCBeam,
        type: 'workflow',
        price: 89.00,
        currency: "USD",
        rating: 4.9,
        reviewCount: 8,
        publisher: {
            name: "Dr. Park",
            avatarUrl: avatarNanobanana,
            isVerified: true
        },
        category: "Concrete"
    },
    {
        id: 'paid-3',
        title: "Retaining Wall Stability",
        description: "Check sliding, overturning, and bearing capacity for cantilever walls.",
        thumbnailUrl: thumbRetainingWall,
        type: 'workflow',
        price: 59.99,
        currency: "USD",
        rating: 4.7,
        reviewCount: 15,
        publisher: {
            name: "GeoWorks",
            isVerified: true
        },
        category: "Geotech"
    },
    {
        id: 'paid-4',
        title: "Wind Load Generator",
        description: "Advanced CFD simulation connector for wind load.",
        thumbnailUrl: thumbWind,
        type: 'component',
        price: 99.00,
        currency: "USD",
        rating: 4.9,
        reviewCount: 18,
        publisher: {
            name: "WindTech",
            isVerified: true
        },
        category: "Simulation"
    },
    {
        id: 'paid-5',
        title: "Steel Connection Detailer",
        description: "Automated calculation for bolted/welded steel connections.",
        thumbnailUrl: thumbSteel,
        type: 'component',
        price: 120.00,
        currency: "USD",
        rating: 4.8,
        reviewCount: 25,
        publisher: {
            name: "SteelPro",
            isVerified: true
        },
        category: "Steel"
    }
];
