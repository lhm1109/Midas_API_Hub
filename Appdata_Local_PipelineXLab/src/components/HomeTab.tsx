import { ExternalLink, Newspaper, Calendar, User, FileText, Video, Search, ChevronDown, Check, Package, Star, Clock, BookOpen } from 'lucide-react';
import { useState } from 'react';

const newsItems = [
    {
        type: 'news',
        title: 'PipelineXLab v2.0 Major Update',
        description: '새로운 기능과 성능 개선이 포함된 메이저 업데이트가 출시되었습니다. Docker 최적화와 새로운 UI가 추가되었습니다.',
        date: '2025-12-30',
        author: 'PipelineXLab Team',
        image: '/assets/images/tech_abstract.png',
        link: 'https://pipelinexlab.com/news/v2-update',
        category: 'Release',
        relatedComponents: ['Core System', 'OpenSeesPy'],
        views: 1250,
        isNew: true,
    },
    {
        type: 'video',
        title: 'Introduction to Stress and Strain',
        description: 'Excellent animated guide by The Efficient Engineer explaining the core concepts of mechanics of materials.',
        date: '2020-02-11',
        author: 'The Efficient Engineer',
        image: 'https://img.youtube.com/vi/aQf6Q8t1FQE/maxresdefault.jpg',
        link: 'https://www.youtube.com/watch?v=aQf6Q8t1FQE',
        category: 'Video',
        relatedComponents: ['Mechanics', 'Engineering'],
        views: 2540000,
        isNew: false,
    },
    {
        type: 'blog',
        title: '구조 해석 워크플로우 최적화',
        description: 'OpenSeesPy를 활용한 효율적인 구조 해석 파이프라인 구축 방법. 실무에서 바로 적용 가능한 팁과 예제 코드를 제공합니다.',
        date: '2025-12-28',
        author: 'Engineering Team',
        image: '/assets/images/data_abstract.png',
        link: 'https://pipelinexlab.com/blog/optimization',
        category: 'Tutorial',
        relatedComponents: ['OpenSeesPy'],
        views: 892,
        isNew: true,
    },
    {
        type: 'video',
        title: 'What is a Retaining Wall?',
        description: 'Practical Engineering explains how retaining walls work and why they fail, with great physical demonstrations.',
        date: '2019-10-01',
        author: 'Practical Engineering',
        image: 'https://img.youtube.com/vi/SkMonNqLaYY/maxresdefault.jpg',
        link: 'https://www.youtube.com/watch?v=SkMonNqLaYY',
        category: 'Video',
        relatedComponents: ['Civil', 'Geotechnical'],
        views: 3100000,
        isNew: false,
    },
    {
        type: 'video',
        title: 'The Design of Skyscrapers',
        description: 'Understanding the lateral systems that keep skyscrapers standing against wind and earthquakes.',
        date: '2021-05-15',
        author: 'Practical Engineering',
        image: 'https://img.youtube.com/vi/FNj8h9a7tSc/maxresdefault.jpg',
        link: 'https://www.youtube.com/watch?v=FNj8h9a7tSc',
        category: 'Video',
        relatedComponents: ['Structural', 'Design'],
        views: 1800000,
        isNew: false,
    },
    {
        type: 'tutorial',
        title: 'Docker 컨테이너 관리 완벽 가이드',
        description: 'PipelineXLab에서 Docker 환경을 효율적으로 관리하는 방법. 리소스 최적화와 트러블슈팅 가이드 포함',
        date: '2025-12-23',
        author: 'DevOps Team',
        image: '/assets/images/tech_abstract.png',
        link: '#',
        category: 'Guide',
        relatedComponents: ['Core System'],
        views: 1580,
        isNew: false,
    },
    {
        type: 'news',
        title: '커뮤니티 워크샵 - 온라인 라이브',
        description: '실시간 Q&A와 함께하는 엔지니어링 워크플로우 최적화 워크샵. 전문가와 함께하는 실습 세션',
        date: '2025-12-20',
        author: 'Community',
        image: '/assets/images/community_abstract.png',
        link: '#',
        category: 'Event',
        relatedComponents: [],
        views: 756,
        isNew: false,
    },
    {
        type: 'blog',
        title: 'Streamlit 대시보드 커스터마이징',
        description: '데이터 시각화와 인터랙티브 대시보드 제작을 위한 실전 팁. 실무 예제와 코드 스니펫 제공',
        date: '2025-12-18',
        author: 'Data Team',
        image: '/assets/images/data_abstract.png',
        link: '#',
        category: 'Tutorial',
        relatedComponents: ['Streamlit Dashboard'],
        views: 1320,
        isNew: false,
    },
    {
        type: 'tutorial',
        title: 'Python 가상환경 설정 가이드',
        description: 'PipelineXLab과 함께 사용하는 Python 가상환경 설정 방법. pip, conda 환경 모두 지원',
        date: '2025-12-12',
        author: 'Python Team',
        image: '/assets/images/community_abstract.png',
        link: '#',
        category: 'Guide',
        relatedComponents: [],
        views: 1450,
        isNew: false,
    },
];

const getIcon = (type: string) => {
    switch (type) {
        case 'news': return Newspaper;
        case 'blog': return FileText;
        case 'video': return Video;
        case 'tutorial': return BookOpen;
        default: return Newspaper;
    }
};

type CategoryFilter = 'All' | 'Release' | 'Tutorial' | 'Guide' | 'Video' | 'Event' | 'News';
type SortType = 'Latest' | 'Popular' | 'Relevant';

export default function HomeTab() {
    const [searchQuery, setSearchQuery] = useState('');
    const [categoryFilter, setCategoryFilter] = useState<CategoryFilter>('All');
    const [sortType, setSortType] = useState<SortType>('Latest');
    const [onlyInstalled, setOnlyInstalled] = useState(false);
    const [onlyUpdates, setOnlyUpdates] = useState(false);
    const [onlyUnread, setOnlyUnread] = useState(false);

    const installedComponents = ['Core System', 'OpenSeesPy', 'Streamlit Dashboard'];

    let filteredItems = newsItems.filter(item => {
        const matchesSearch = item.title.toLowerCase().includes(searchQuery.toLowerCase()) ||
            item.description.toLowerCase().includes(searchQuery.toLowerCase());
        const matchesCategory = categoryFilter === 'All' || item.category === categoryFilter;
        const matchesInstalled = !onlyInstalled ||
            item.relatedComponents.some(comp => installedComponents.includes(comp));
        const matchesUpdates = !onlyUpdates || item.category === 'Release';
        const matchesUnread = !onlyUnread || item.isNew;

        return matchesSearch && matchesCategory && matchesInstalled && matchesUpdates && matchesUnread;
    });

    return (
        <div className="flex flex-col h-full bg-background">
            {/* Header */}
            <div className="px-4 py-3 bg-surface-1 border-b border-surface-2">
                <h2 className="text-xs text-foreground font-semibold uppercase tracking-wide mb-3">Latest Updates</h2>

                {/* Search and Filters */}
                <div className="flex items-center gap-2 mb-2">
                    <div className="flex-1 relative">
                        <Search className="absolute left-2 top-1/2 transform -translate-y-1/2 w-3 h-3 text-muted-foreground" />
                        <input
                            type="text"
                            placeholder="Search updates..."
                            value={searchQuery}
                            onChange={(e) => setSearchQuery(e.target.value)}
                            className="w-full pl-7 pr-3 py-1.5 bg-surface-2 border border-surface-3 rounded text-[11px] text-foreground placeholder-muted-foreground focus:outline-none focus:border-blue-500"
                        />
                    </div>

                    <div className="flex gap-1">
                        {(['All', 'Release', 'Tutorial', 'Guide', 'Video', 'Event', 'News'] as CategoryFilter[]).map((cat) => (
                            <button
                                key={cat}
                                onClick={() => setCategoryFilter(cat)}
                                className={`px-2 py-1.5 text-[10px] rounded transition-colors whitespace-nowrap ${categoryFilter === cat
                                    ? 'bg-blue-600 text-white shadow-sm'
                                    : 'bg-surface-2 text-muted-foreground hover:text-foreground hover:bg-accent border border-surface-3'
                                    }`}
                            >
                                {cat}
                            </button>
                        ))}
                    </div>

                    <div className="relative">
                        <select
                            value={sortType}
                            onChange={(e) => setSortType(e.target.value as SortType)}
                            className="pl-2 pr-7 py-1.5 bg-surface-2 border border-surface-3 rounded text-[10px] text-foreground focus:outline-none focus:border-blue-500 appearance-none cursor-pointer"
                        >
                            <option value="Latest">Latest</option>
                            <option value="Popular">Popular</option>
                            <option value="Relevant">Relevant</option>
                        </select>
                        <ChevronDown className="absolute right-2 top-1/2 transform -translate-y-1/2 w-3 h-3 text-muted-foreground pointer-events-none" />
                    </div>
                </div>

                {/* Toggle Filters */}
                <div className="flex items-center gap-2">
                    <button
                        onClick={() => setOnlyInstalled(!onlyInstalled)}
                        className={`px-2 py-1 rounded text-[10px] flex items-center gap-1 transition-colors ${onlyInstalled
                            ? 'bg-green-600 text-white'
                            : 'bg-surface-2 text-muted-foreground hover:text-foreground hover:bg-accent border border-surface-3'
                            }`}
                    >
                        {onlyInstalled && <Check className="w-3 h-3" />}
                        <Package className="w-3 h-3" />
                        <span>My Components</span>
                    </button>
                    <button
                        onClick={() => setOnlyUpdates(!onlyUpdates)}
                        className={`px-2 py-1 rounded text-[10px] flex items-center gap-1 transition-colors ${onlyUpdates
                            ? 'bg-purple-600 text-white'
                            : 'bg-surface-2 text-muted-foreground hover:text-foreground hover:bg-accent border border-surface-3'
                            }`}
                    >
                        {onlyUpdates && <Check className="w-3 h-3" />}
                        <Star className="w-3 h-3" />
                        <span>Updates Only</span>
                    </button>
                    <button
                        onClick={() => setOnlyUnread(!onlyUnread)}
                        className={`px-2 py-1 rounded text-[10px] flex items-center gap-1 transition-colors ${onlyUnread
                            ? 'bg-blue-600 text-white'
                            : 'bg-surface-2 text-muted-foreground hover:text-foreground hover:bg-accent border border-surface-3'
                            }`}
                    >
                        {onlyUnread && <Check className="w-3 h-3" />}
                        <Clock className="w-3 h-3" />
                        <span>Unread</span>
                    </button>
                    <span className="text-[10px] text-muted-foreground ml-auto">
                        {filteredItems.length} results
                    </span>
                </div>
            </div>

            {/* Content */}
            <div className="flex-1 overflow-y-auto p-4">
                <div className="grid grid-cols-4 gap-3">
                    {filteredItems.map((item, index) => {
                        const Icon = getIcon(item.type);
                        const isRelated = item.relatedComponents.some(comp => installedComponents.includes(comp));

                        return (
                            <div
                                key={index}
                                onClick={() => item.link && item.link !== '#' && window.open(item.link, '_blank')}
                                className={`group bg-surface-1 border rounded-lg overflow-hidden hover:border-blue-500 hover:shadow-md transition-all cursor-pointer ${isRelated ? 'border-green-500/40' : 'border-surface-2'
                                    }`}
                            >
                                <div className="relative h-32 overflow-hidden bg-muted">
                                    <img src={item.image} alt={item.title} className="w-full h-full object-cover group-hover:scale-105 transition-all duration-300" />
                                    <div className={`absolute top-2 left-2 px-2 py-0.5 rounded text-[9px] font-medium backdrop-blur-sm ${item.category === 'Release' ? 'bg-purple-500/80 text-white border border-purple-600/50' :
                                        item.category === 'Tutorial' ? 'bg-blue-500/80 text-white border border-blue-600/50' :
                                            item.category === 'Guide' ? 'bg-green-500/80 text-white border border-green-600/50' :
                                                item.category === 'Video' ? 'bg-red-500/80 text-white border border-red-600/50' :
                                                    item.category === 'Event' ? 'bg-yellow-500/80 text-white border border-yellow-600/50' :
                                                        'bg-slate-500/80 text-white border border-slate-600/50'
                                        }`}>
                                        {item.category}
                                    </div>
                                    <div className="absolute top-2 right-2 p-1 bg-white/90 backdrop-blur-sm rounded">
                                        <Icon className="w-3 h-3 text-slate-700" />
                                    </div>
                                    {item.isNew && (
                                        <div className="absolute top-2 right-10 w-2 h-2 bg-blue-400 rounded-full animate-pulse"></div>
                                    )}
                                </div>

                                <div className="p-3">
                                    <h3 className="text-sm text-foreground font-semibold leading-snug group-hover:text-blue-600 transition-colors line-clamp-2 min-h-[2.5rem] mb-2">
                                        {item.title}
                                    </h3>
                                    <p className="text-[11px] text-muted-foreground leading-relaxed mb-3 line-clamp-2 min-h-[2.75rem]">
                                        {item.description}
                                    </p>

                                    {item.relatedComponents.length > 0 && (
                                        <div className="mb-3 flex flex-wrap gap-1">
                                            {item.relatedComponents.map((comp, idx) => (
                                                <span key={idx} className={`px-1.5 py-0.5 rounded text-[9px] ${installedComponents.includes(comp)
                                                    ? 'bg-green-500/20 text-green-700 border border-green-500/40'
                                                    : 'bg-slate-200 text-slate-700 border border-slate-300'
                                                    }`}>
                                                    {comp}
                                                </span>
                                            ))}
                                        </div>
                                    )}

                                    <div className="flex items-center justify-between text-[9px]">
                                        <div className="flex items-center gap-2 text-muted-foreground">
                                            <div className="flex items-center gap-1">
                                                <Calendar className="w-2.5 h-2.5" />
                                                <span>{item.date}</span>
                                            </div>
                                            <div className="flex items-center gap-1">
                                                <User className="w-2.5 h-2.5" />
                                                <span className="line-clamp-1">{item.author}</span>
                                            </div>
                                        </div>
                                        <ExternalLink className="w-3 h-3 text-muted-foreground group-hover:text-blue-600 transition-colors" />
                                    </div>
                                </div>
                            </div>
                        );
                    })}
                </div>
            </div>
        </div>
    );
}
