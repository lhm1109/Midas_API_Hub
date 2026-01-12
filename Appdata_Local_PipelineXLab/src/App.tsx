import { useState, useEffect } from 'react';
import Sidebar from './components/Sidebar';
import Header from './components/Header';
import HomeTab from './components/HomeTab';
import { LaunchTab } from './components/LaunchTab';
import MarketplaceTab from './components/MarketplaceTab';
import ComponentsTab from './components/ComponentsTab';
import { PlaygroundTab } from './components/PlaygroundTab';
import AITab from './components/AITab';
import SettingsTab from './components/SettingsTab';
import DocumentTab from './components/DocumentTab';


export default function App() {
    const [activeTab, setActiveTab] = useState('home');
    const [sidebarCollapsed, setSidebarCollapsed] = useState(false);

    // Initialize theme on app load
    useEffect(() => {
        const savedTheme = localStorage.getItem('pxlab_theme') || 'dark';
        const root = document.documentElement;

        if (savedTheme === 'dark') {
            root.classList.add('dark');
            root.classList.remove('light');
        } else if (savedTheme === 'light') {
            root.classList.remove('dark');
            root.classList.add('light');
        } else {
            // system
            const prefersDark = window.matchMedia('(prefers-color-scheme: dark)').matches;
            root.classList.toggle('dark', prefersDark);
            root.classList.toggle('light', !prefersDark);
        }
    }, []);

    const renderContent = () => {
        switch (activeTab) {
            case 'home':
                return <HomeTab />;
            case 'dashboard':
                return <LaunchTab />;
            case 'marketplace':
                return <MarketplaceTab />;
            case 'components':
                return <ComponentsTab />;
            case 'playground':
                return <PlaygroundTab />;
            case 'ai':
                return <AITab />;
            case 'settings':
                return <SettingsTab />;
            case 'document':
                return <DocumentTab />;
            default:
                return <HomeTab />;
        }
    };


    return (
        <div className="flex h-screen bg-background overflow-hidden text-foreground">
            <Sidebar
                activeTab={activeTab}
                onTabChange={setActiveTab}
                collapsed={sidebarCollapsed}
                onToggleCollapse={() => setSidebarCollapsed(!sidebarCollapsed)}
            />
            <div className="flex-1 flex flex-col overflow-hidden">
                <Header />
                <main className="flex-1 overflow-y-auto">
                    {renderContent()}
                </main>
            </div>
        </div>
    );
}
