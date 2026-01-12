import { useState, useEffect } from 'react';
import { Search, RefreshCw, Package, FolderOpen, Filter, ChevronDown, X, ArrowUpDown } from 'lucide-react';

interface ComponentInfo {
    id: string;
    name: string;
    category: string;
    version: string;
    description: string;
    icon: string;
    kind: string;
    runtime: string;
    path: string;
    hasUI: boolean;
    inputs: number;
    outputs: number;
}

// Check if running in Electron
const isElectron = typeof window !== 'undefined' && (window as any).electronAPI;

// Category colors
const categoryColors: Record<string, string> = {
    Input: 'text-green-400',
    Output: 'text-blue-400',
    Process: 'text-purple-400',
    API: 'text-orange-400',
    MIDAS: 'text-red-400',
    OpenSeesPy: 'text-cyan-400',
    Streamlit: 'text-pink-400',
    SymPy: 'text-yellow-400',
    Document: 'text-indigo-400',
    FlowControl: 'text-teal-400',
    Utility: 'text-slate-400',
    Test: 'text-gray-400',
};

type SortKey = 'name' | 'version' | 'category' | 'kind';

export default function ComponentsTab() {
    const [searchQuery, setSearchQuery] = useState('');
    const [components, setComponents] = useState<ComponentInfo[]>([]);
    const [loading, setLoading] = useState(true);
    const [selectedCategory, setSelectedCategory] = useState<string | null>(null);
    const [showCategoryFilter, setShowCategoryFilter] = useState(false);
    const [sortKey, setSortKey] = useState<SortKey>('category');
    const [sortAsc, setSortAsc] = useState(true);

    // Load components from Electron
    useEffect(() => {
        loadComponents();
    }, []);

    const loadComponents = async () => {
        setLoading(true);
        try {
            if (isElectron) {
                const result = await (window as any).electronAPI.getComponents();
                if (result.success) {
                    setComponents(result.components);
                }
            }
        } catch (error) {
            console.error('Failed to load components:', error);
        } finally {
            setLoading(false);
        }
    };

    const handleOpenFolder = async (path: string) => {
        if (isElectron) {
            await (window as any).electronAPI.openComponentFolder(path);
        }
    };

    const handleSort = (key: SortKey) => {
        if (sortKey === key) {
            setSortAsc(!sortAsc);
        } else {
            setSortKey(key);
            setSortAsc(true);
        }
    };

    // Get unique categories
    const categories = [...new Set(components.map(c => c.category))].sort();

    // Filter and sort components
    const filteredComponents = components
        .filter(c => {
            const matchesSearch = c.name.toLowerCase().includes(searchQuery.toLowerCase()) ||
                c.description.toLowerCase().includes(searchQuery.toLowerCase());
            const matchesCategory = !selectedCategory || c.category === selectedCategory;
            return matchesSearch && matchesCategory;
        })
        .sort((a, b) => {
            const aVal = a[sortKey] || '';
            const bVal = b[sortKey] || '';
            const cmp = aVal.localeCompare(bVal);
            return sortAsc ? cmp : -cmp;
        });

    const getCategoryColor = (category: string) => categoryColors[category] || 'text-slate-400';

    const SortHeader = ({ label, sortField }: { label: string; sortField: SortKey }) => (
        <th
            className="py-2 px-3 font-medium cursor-pointer hover:text-foreground transition-colors"
            onClick={() => handleSort(sortField)}
        >
            <div className="flex items-center gap-1">
                {label}
                <ArrowUpDown className={`w-3 h-3 ${sortKey === sortField ? 'text-blue-400' : 'opacity-30'}`} />
            </div>
        </th>
    );

    return (
        <div className="flex flex-col h-full bg-background">
            {/* Header Stats */}
            <div className="px-4 py-2 bg-card border-b border-border">
                <div className="flex items-center justify-between">
                    <div className="flex items-center gap-4 text-[10px]">
                        <div className="flex items-center gap-1.5">
                            <Package className="w-3 h-3 text-blue-400" />
                            <span className="text-muted-foreground">Total:</span>
                            <span className="text-blue-400 font-semibold">{components.length} components</span>
                        </div>
                        <div className="flex items-center gap-1.5">
                            <span className="text-muted-foreground">Categories:</span>
                            <span className="text-foreground font-semibold">{categories.length}</span>
                        </div>
                        {selectedCategory && (
                            <div className="flex items-center gap-1.5">
                                <span className="text-muted-foreground">Showing:</span>
                                <span className={`font-semibold ${getCategoryColor(selectedCategory)}`}>{selectedCategory}</span>
                            </div>
                        )}
                    </div>
                    <button
                        onClick={loadComponents}
                        disabled={loading}
                        className="px-2 py-1 bg-input hover:bg-accent text-foreground text-[10px] rounded flex items-center gap-1 transition-colors disabled:opacity-50"
                    >
                        <RefreshCw className={`w-3 h-3 ${loading ? 'animate-spin' : ''}`} />
                        Refresh
                    </button>
                </div>
            </div>

            {/* Search & Filter */}
            <div className="px-4 py-3 bg-card border-b border-border">
                <div className="flex items-center gap-2">
                    <div className="flex-1 relative max-w-md">
                        <Search className="absolute left-2.5 top-1/2 transform -translate-y-1/2 w-3.5 h-3.5 text-muted-foreground" />
                        <input
                            type="text"
                            placeholder="Search components..."
                            value={searchQuery}
                            onChange={(e) => setSearchQuery(e.target.value)}
                            className="w-full pl-8 pr-3 py-1.5 bg-input border border-border rounded text-xs text-foreground placeholder-muted-foreground focus:outline-none focus:border-blue-500/50"
                        />
                    </div>

                    {/* Category Filter */}
                    <div className="relative">
                        <button
                            onClick={() => setShowCategoryFilter(!showCategoryFilter)}
                            className={`px-3 py-1.5 rounded text-xs flex items-center gap-1.5 transition-colors border ${selectedCategory
                                ? 'bg-blue-500/20 border-blue-500/50 text-blue-400'
                                : 'bg-input border-border text-muted-foreground hover:text-foreground'
                                }`}
                        >
                            <Filter className="w-3.5 h-3.5" />
                            {selectedCategory || 'All Categories'}
                            <ChevronDown className="w-3 h-3" />
                        </button>

                        {showCategoryFilter && (
                            <>
                                <div className="fixed inset-0 z-10" onClick={() => setShowCategoryFilter(false)} />
                                <div className="absolute left-0 mt-1 w-48 bg-card border border-border rounded-lg shadow-xl z-20 py-1 max-h-64 overflow-y-auto">
                                    <button
                                        onClick={() => { setSelectedCategory(null); setShowCategoryFilter(false); }}
                                        className={`w-full px-3 py-1.5 text-left text-xs hover:bg-accent ${!selectedCategory ? 'text-blue-400' : 'text-foreground'}`}
                                    >
                                        All Categories ({components.length})
                                    </button>
                                    <div className="border-t border-border my-1" />
                                    {categories.map(cat => (
                                        <button
                                            key={cat}
                                            onClick={() => { setSelectedCategory(cat); setShowCategoryFilter(false); }}
                                            className={`w-full px-3 py-1.5 text-left text-xs hover:bg-accent flex items-center justify-between ${selectedCategory === cat ? 'bg-accent' : ''}`}
                                        >
                                            <span className={getCategoryColor(cat)}>{cat}</span>
                                            <span className="text-muted-foreground">{components.filter(c => c.category === cat).length}</span>
                                        </button>
                                    ))}
                                </div>
                            </>
                        )}
                    </div>

                    {selectedCategory && (
                        <button
                            onClick={() => setSelectedCategory(null)}
                            className="p-1.5 text-muted-foreground hover:text-foreground rounded hover:bg-accent"
                            title="Clear filter"
                        >
                            <X className="w-3.5 h-3.5" />
                        </button>
                    )}
                </div>
            </div>

            {/* Component Table */}
            <div className="flex-1 overflow-hidden">
                {loading ? (
                    <div className="flex items-center justify-center h-full text-muted-foreground">
                        <RefreshCw className="w-5 h-5 animate-spin mr-2" />
                        Loading components...
                    </div>
                ) : filteredComponents.length === 0 ? (
                    <div className="flex flex-col items-center justify-center h-full text-muted-foreground">
                        <Package className="w-12 h-12 mb-3 opacity-50" />
                        <p className="text-sm">No components found</p>
                    </div>
                ) : (
                    <div className="h-full overflow-y-auto">
                        <table className="w-full text-xs">
                            <thead className="sticky top-0 bg-card border-b border-border text-muted-foreground text-left">
                                <tr>
                                    <SortHeader label="Name" sortField="name" />
                                    <SortHeader label="Version" sortField="version" />
                                    <SortHeader label="Category" sortField="category" />
                                    <SortHeader label="Type" sortField="kind" />
                                    <th className="py-2 px-3 font-medium w-20">Actions</th>
                                </tr>
                            </thead>
                            <tbody>
                                {filteredComponents.map((component) => (
                                    <tr
                                        key={component.id}
                                        className="border-b border-border/50 hover:bg-accent/50 transition-colors group"
                                    >
                                        {/* Name */}
                                        <td className="py-2.5 px-3">
                                            <div className="flex items-center gap-2">
                                                <span className="text-lg">{component.icon}</span>
                                                <div>
                                                    <div className="flex items-center gap-2">
                                                        <span className="text-foreground font-medium">{component.name}</span>
                                                        {component.hasUI && (
                                                            <span className="px-1 py-0.5 rounded text-[9px] bg-blue-500/20 text-blue-400">UI</span>
                                                        )}
                                                    </div>
                                                    {component.description && (
                                                        <p className="text-[10px] text-muted-foreground line-clamp-1 max-w-md">
                                                            {component.description}
                                                        </p>
                                                    )}
                                                </div>
                                            </div>
                                        </td>

                                        {/* Version */}
                                        <td className="py-2.5 px-3">
                                            <span className="text-foreground">v{component.version}</span>
                                        </td>

                                        {/* Category */}
                                        <td className="py-2.5 px-3">
                                            <span className={getCategoryColor(component.category)}>{component.category}</span>
                                        </td>

                                        {/* Type */}
                                        <td className="py-2.5 px-3">
                                            <span className="text-muted-foreground">{component.kind}</span>
                                        </td>

                                        {/* Actions */}
                                        <td className="py-2.5 px-3">
                                            <button
                                                onClick={() => handleOpenFolder(component.path)}
                                                className="p-1.5 text-muted-foreground hover:text-foreground opacity-0 group-hover:opacity-100 transition-all rounded hover:bg-accent"
                                                title="Open folder"
                                            >
                                                <FolderOpen className="w-3.5 h-3.5" />
                                            </button>
                                        </td>
                                    </tr>
                                ))}
                            </tbody>
                        </table>
                    </div>
                )}
            </div>
        </div>
    );
}
