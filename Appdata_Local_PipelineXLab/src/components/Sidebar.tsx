import { Home, Puzzle, Settings, ShoppingBag, MessageCircle, Sparkles, Rocket, Menu, ArrowLeft, FileText, FlaskConical } from 'lucide-react';

interface SidebarProps {
    activeTab: string;
    onTabChange: (tab: string) => void;
    collapsed: boolean;
    onToggleCollapse: () => void;
}

const tabs = [
    { id: 'home', label: 'Home', icon: Home },
    { id: 'dashboard', label: 'Launch', icon: Rocket },
    { id: 'components', label: 'Components', icon: Puzzle },
    { id: 'playground', label: 'Playground', icon: FlaskConical },
    { id: 'marketplace', label: 'Marketplace', icon: ShoppingBag },
    { id: 'ai', label: 'AI Assistant', icon: Sparkles },
];


const bottomTabs = [
    { id: 'document', label: 'Document', icon: FileText },
    { id: 'discord', label: 'Join Discord Server', icon: MessageCircle },
    { id: 'settings', label: 'Settings', icon: Settings },
];


export default function Sidebar({ activeTab, onTabChange, collapsed, onToggleCollapse }: SidebarProps) {
    return (
        <div className={`h-screen bg-surface-1 border-r border-surface-2 flex flex-col transition-all duration-200 ${collapsed ? 'w-14' : 'w-36'
            }`}>
            {/* Header with Toggle Buttons */}
            <div className={`border-b border-surface-2 ${collapsed ? 'px-2 py-3' : 'px-3 py-4'}`}>
                {!collapsed ? (
                    <>
                        <div className="flex items-center justify-between mb-2">
                            <button
                                onClick={onToggleCollapse}
                                className="p-1 hover:bg-accent text-muted-foreground hover:text-foreground rounded transition-colors"
                                title="뒤로"
                            >
                                <ArrowLeft className="w-3.5 h-3.5" />
                            </button>
                            <button
                                onClick={onToggleCollapse}
                                className="p-1 hover:bg-accent text-muted-foreground hover:text-foreground rounded transition-colors"
                                title="메뉴 접기"
                            >
                                <Menu className="w-3.5 h-3.5" />
                            </button>
                        </div>
                        <h1 className="text-[11px] text-foreground font-semibold tracking-tight">PipelineXLab</h1>
                        <p className="text-[9px] text-muted-foreground mt-0.5">Launcher v1.0</p>
                    </>
                ) : (
                    <button
                        onClick={onToggleCollapse}
                        className="w-full p-1.5 hover:bg-accent text-muted-foreground hover:text-foreground rounded transition-colors flex items-center justify-center"
                        title="메뉴 펼치기"
                    >
                        <Menu className="w-4 h-4" />
                    </button>
                )}
            </div>

            <nav className="flex-1 px-2 py-3">
                {tabs.map((tab) => {
                    const Icon = tab.icon;
                    const isActive = activeTab === tab.id;

                    return (
                        <button
                            key={tab.id}
                            onClick={() => onTabChange(tab.id)}
                            className={`w-full flex items-center ${collapsed ? 'justify-center' : 'gap-2'} px-2 py-2 mb-1 rounded text-[11px] transition-colors ${isActive
                                ? 'bg-blue-600 text-white shadow-sm'
                                : 'text-muted-foreground hover:text-foreground hover:bg-accent'
                                }`}
                            title={collapsed ? tab.label : undefined}
                        >
                            <Icon className="w-3.5 h-3.5" />
                            {!collapsed && <span>{tab.label}</span>}
                        </button>
                    );
                })}
            </nav>

            <div className="px-2 py-3 border-t border-surface-2">
                {bottomTabs.map((tab) => {
                    const Icon = tab.icon;
                    const isActive = activeTab === tab.id;

                    return (
                        <button
                            key={tab.id}
                            onClick={() => onTabChange(tab.id)}
                            className={`w-full flex items-center ${collapsed ? 'justify-center' : 'gap-2'} px-2 py-2 mb-1 rounded text-[11px] transition-colors ${isActive
                                ? 'bg-blue-600 text-white shadow-sm'
                                : 'text-muted-foreground hover:text-foreground hover:bg-accent'
                                }`}
                            title={collapsed ? tab.label : undefined}
                        >
                            <Icon className="w-3.5 h-3.5" />
                            {!collapsed && <span className="truncate">{tab.label}</span>}
                        </button>
                    );
                })}
            </div>
        </div>
    );
}
