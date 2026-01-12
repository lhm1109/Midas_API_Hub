
import React, { useState, useRef } from 'react';
import { banners, freeItems, paidItems, MarketplaceItem } from '../data/mockMarketplace';
import { Search, Filter, Star, CheckCircle, Download, ShoppingCart, ChevronLeft, ChevronRight, Play } from 'lucide-react';

export default function MarketplaceTab() {
    return (
        <div className="flex flex-col h-full bg-background text-foreground overflow-y-auto">
            {/* Header / Search Area */}
            <div className="p-6 pb-2">
                <div className="flex items-center justify-between mb-6">
                    <div>
                        <h1 className="text-2xl font-bold tracking-tight">Marketplace</h1>
                        <p className="text-muted-foreground text-sm">Discover components, workflows, and automation services.</p>
                    </div>
                    <div className="flex items-center gap-3">
                        <div className="bg-secondary/50 px-3 py-1 rounded-full text-xs font-medium text-secondary-foreground border border-border">
                            My Points: 1,250 P
                        </div>
                    </div>
                </div>

                <div className="flex gap-2 mb-4">
                    <div className="relative flex-1">
                        <Search className="absolute left-3 top-1/2 -translate-y-1/2 w-4 h-4 text-muted-foreground" />
                        <input
                            type="text"
                            placeholder="Search marketplace..."
                            className="w-full bg-secondary/50 border border-input rounded-md pl-9 pr-4 py-2 text-sm focus:outline-none focus:ring-1 focus:ring-primary"
                        />
                    </div>
                    <button className="flex items-center gap-2 px-4 py-2 bg-secondary/50 border border-input rounded-md text-sm hover:bg-secondary/80 transition-colors">
                        <Filter className="w-4 h-4" />
                        <span>Filter</span>
                    </button>
                </div>
            </div>

            {/* Banner Carousel */}
            <BannerCarousel />

            {/* Main Content */}
            <div className="flex-1 p-6 space-y-10">

                {/* Paid Section */}
                <Section title="Premium Workflows & Services" subtitle="Professional solutions for complex needs">
                    <div className="flex gap-4 overflow-x-auto pb-4 -mx-6 px-6 scrollbar-hide snap-x">
                        {paidItems.map(item => (
                            <MarketplaceCard key={item.id} item={item} />
                        ))}
                    </div>
                </Section>

                {/* Free Section */}
                <Section title="Featured Free Components" subtitle="Essential tools for everyone">
                    <div className="flex gap-4 overflow-x-auto pb-4 -mx-6 px-6 scrollbar-hide snap-x">
                        {freeItems.map(item => (
                            <MarketplaceCard key={item.id} item={item} />
                        ))}
                    </div>
                </Section>

                {/* Categories / Tags (Optional visual filler) */}
                <div className="py-4">
                    <h3 className="text-sm font-semibold mb-3">Popular Categories</h3>
                    <div className="flex gap-2 flex-wrap">
                        {['Structural Analysis', 'AI & ML', 'Report Generation', 'Data Visualization', 'Cost Estimation', 'BIM Integration'].map(tag => (
                            <span key={tag} className="px-3 py-1 bg-secondary rounded-full text-xs hover:bg-primary/10 hover:text-primary cursor-pointer transition-colors">
                                {tag}
                            </span>
                        ))}
                    </div>
                </div>
            </div>
        </div>
    );
}

// --- Sub-components ---

function Section({ title, subtitle, children }: { title: string, subtitle: string, children: React.ReactNode }) {
    return (
        <div className="space-y-4">
            <div className="flex items-end justify-between">
                <div>
                    <h2 className="text-xl font-bold">{title}</h2>
                    <p className="text-sm text-muted-foreground">{subtitle}</p>
                </div>
                <button className="text-xs text-primary font-medium hover:underline">View All</button>
            </div>
            {children}
        </div>
    );
}

function BannerCarousel() {
    const scrollRef = useRef<HTMLDivElement>(null);
    const [activeIndex, setActiveIndex] = useState(0);

    const scroll = (direction: 'left' | 'right') => {
        if (scrollRef.current) {
            const { current } = scrollRef;
            const scrollAmount = current.clientWidth * 0.8;
            current.scrollBy({ left: direction === 'right' ? scrollAmount : -scrollAmount, behavior: 'smooth' });
        }
    };

    const handleScroll = () => {
        if (scrollRef.current) {
            const { scrollLeft, clientWidth } = scrollRef.current;
            // Calculate index based on scroll center
            const index = Math.round(scrollLeft / clientWidth);
            setActiveIndex(index);
        }
    };

    return (
        <div className="relative group mx-6">
            <div
                ref={scrollRef}
                onScroll={handleScroll}
                className="flex gap-4 overflow-x-auto snap-x snap-mandatory scrollbar-hide rounded-xl"
                style={{ scrollbarWidth: 'none', msOverflowStyle: 'none' }}
            >
                {banners.map((banner) => (
                    <div key={banner.id} className="min-w-full md:min-w-[85%] lg:min-w-[70%] snap-center relative aspect-[21/9] md:aspect-[24/9] lg:aspect-[32/9] rounded-xl overflow-hidden shadow-lg border border-white/10">
                        <img src={banner.imageUrl} alt={banner.title} className="w-full h-full object-cover" />
                        <div className="absolute inset-0 bg-gradient-to-t from-black/90 via-black/40 to-transparent p-8 flex flex-col justify-end">
                            <h3 className="text-3xl font-bold text-white mb-2">{banner.title}</h3>
                            <p className="text-gray-200 mb-4 max-w-lg">{banner.subtitle}</p>
                            <button className="w-fit px-6 py-2 bg-primary text-primary-foreground rounded-md font-medium text-sm hover:bg-primary/90 transition-colors flex items-center gap-2">
                                Explore Now <ChevronRight className="w-4 h-4" />
                            </button>
                        </div>
                    </div>
                ))}
            </div>

            {/* Pagination Dots */}
            <div className="flex justify-center gap-2 mt-4">
                {banners.map((_, index) => (
                    <button
                        key={index}
                        onClick={() => {
                            if (scrollRef.current) {
                                const { clientWidth } = scrollRef.current;
                                scrollRef.current.scrollTo({ left: index * clientWidth, behavior: 'smooth' });
                            }
                        }}
                        className={`transition-all duration-300 rounded-full ${index === activeIndex
                            ? 'bg-primary w-6 h-2'
                            : 'bg-muted-foreground/30 w-2 h-2 hover:bg-muted-foreground/50'
                            }`}
                        aria-label={`Go to slide ${index + 1}`}
                    />
                ))}
            </div>

            {/* Nav Buttons (Visible on hover) */}
            <button
                onClick={() => scroll('left')}
                className="absolute left-4 top-1/2 -translate-y-1/2 w-10 h-10 bg-black/50 backdrop-blur-sm text-white rounded-full flex items-center justify-center opacity-0 group-hover:opacity-100 transition-opacity hover:bg-black/70 z-10"
            >
                <ChevronLeft className="w-6 h-6" />
            </button>
            <button
                onClick={() => scroll('right')}
                className="absolute right-4 top-1/2 -translate-y-1/2 w-10 h-10 bg-black/50 backdrop-blur-sm text-white rounded-full flex items-center justify-center opacity-0 group-hover:opacity-100 transition-opacity hover:bg-black/70 z-10"
            >
                <ChevronRight className="w-6 h-6" />
            </button>
        </div>
    );
}

function MarketplaceCard({ item }: { item: MarketplaceItem }) {
    return (
        <div className="min-w-[280px] w-[280px] snap-center bg-card border border-border rounded-xl overflow-hidden hover:shadow-md hover:border-primary/50 transition-all group">
            <div className="aspect-video bg-secondary relative overflow-hidden">
                {item.thumbnailUrl ? (
                    <img src={item.thumbnailUrl} alt={item.title} className="w-full h-full object-cover group-hover:scale-105 transition-transform duration-500" />
                ) : (
                    <div className="w-full h-full flex items-center justify-center text-muted-foreground bg-secondary/50">
                        <span className="text-xs">No Preview</span>
                    </div>
                )}
                <div className="absolute top-2 left-2">
                    <span className={`px-2 py-1 rounded text-[10px] font-bold uppercase tracking-wider ${item.type === 'component' ? 'bg-blue-500/20 text-blue-400 border border-blue-500/30' :
                        item.type === 'workflow' ? 'bg-purple-500/20 text-purple-400 border border-purple-500/30' :
                            'bg-orange-500/20 text-orange-400 border border-orange-500/30'
                        }`}>
                        {item.category}
                    </span>
                </div>
            </div>

            <div className="p-4">
                <div className="flex items-start justify-between mb-2">
                    <h3 className="font-semibold text-base line-clamp-1 group-hover:text-primary transition-colors">{item.title}</h3>
                </div>
                <p className="text-xs text-muted-foreground line-clamp-2 mb-4 h-8">{item.description}</p>

                <div className="flex items-center gap-2 mb-4">
                    <div className="w-6 h-6 rounded-full bg-secondary overflow-hidden flex-shrink-0">
                        {item.publisher.avatarUrl ? (
                            <img src={item.publisher.avatarUrl} alt={item.publisher.name} className="w-full h-full object-cover" />
                        ) : (
                            <div className="w-full h-full flex items-center justify-center bg-muted text-[10px] font-bold text-muted-foreground">
                                {item.publisher.name.substring(0, 2).toUpperCase()}
                            </div>
                        )}
                    </div>
                    <span className="text-xs text-muted-foreground truncate hover:text-foreground cursor-pointer flex items-center gap-1">
                        {item.publisher.name}
                        {item.publisher.isVerified && <CheckCircle className="w-3 h-3 text-blue-400" />}
                    </span>
                </div>

                <div className="flex items-center justify-between pt-3 border-t border-border/50">
                    <div className="flex items-center gap-1 text-yellow-500">
                        <Star className="w-3 h-3 fill-current" />
                        <span className="text-xs font-medium text-foreground">{item.rating}</span>
                        <span className="text-[10px] text-muted-foreground">({item.reviewCount})</span>
                    </div>

                    <div className="font-bold text-sm">
                        {item.price === 0 ? (
                            <span className="text-green-500">Free</span>
                        ) : (
                            <span>${item.price.toFixed(2)}</span>
                        )}
                    </div>
                </div>

                <button className="w-full mt-3 py-1.5 rounded bg-foreground/5 hover:bg-foreground/10 text-xs font-medium transition-colors flex items-center justify-center gap-2">
                    {item.price === 0 ? (
                        <>
                            <Download className="w-3 h-3" /> Install
                        </>
                    ) : (
                        <>
                            <ShoppingCart className="w-3 h-3" /> Purchase
                        </>
                    )}
                </button>
            </div>
        </div>
    );
}
