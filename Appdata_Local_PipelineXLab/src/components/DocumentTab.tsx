import { useState, useEffect } from 'react';
import { ExternalLink, RefreshCw, Home, ChevronLeft, ChevronRight } from 'lucide-react';

export default function DocumentTab() {
    const [isLoading, setIsLoading] = useState(true);
    const [key, setKey] = useState(0);

    // Determine the correct path based on environment
    const getDocPath = () => {
        // Check if running in Electron
        const isElectron = !!(window as any).electron;

        if (isElectron) {
            // In Electron, use file:// protocol with app path
            // The doc folder is relative to the app directory
            return 'doc/index.html';
        }

        // In browser dev mode, serve from public folder or use relative path
        return '/doc/index.html';
    };

    const docPath = getDocPath();

    const handleRefresh = () => {
        setIsLoading(true);
        setKey(prev => prev + 1);
    };

    const handleOpenExternal = () => {
        const isElectron = !!(window as any).electron;
        if (isElectron) {
            (window as any).electron.openExternal(docPath);
        } else {
            window.open(docPath, '_blank');
        }
    };

    return (
        <div className="h-full flex flex-col bg-background">
            {/* Toolbar */}
            <div className="flex items-center gap-2 px-4 py-2 border-b border-surface-2 bg-surface-1">
                <button
                    onClick={handleRefresh}
                    className="p-1.5 hover:bg-accent rounded transition-colors text-muted-foreground hover:text-foreground"
                    title="Refresh"
                >
                    <RefreshCw className="w-4 h-4" />
                </button>
                <button
                    onClick={handleOpenExternal}
                    className="p-1.5 hover:bg-accent rounded transition-colors text-muted-foreground hover:text-foreground"
                    title="Open in Browser"
                >
                    <ExternalLink className="w-4 h-4" />
                </button>
                <div className="flex-1" />
                <span className="text-xs text-muted-foreground">PipelineXLab Documentation</span>
            </div>

            {/* Loading indicator */}
            {isLoading && (
                <div className="absolute inset-0 flex items-center justify-center bg-background/80 z-10 pointer-events-none">
                    <div className="flex flex-col items-center gap-2">
                        <div className="w-6 h-6 border-2 border-blue-500 border-t-transparent rounded-full animate-spin" />
                        <span className="text-sm text-muted-foreground">Loading documentation...</span>
                    </div>
                </div>
            )}

            {/* Documentation iframe */}
            <div className="flex-1 relative">
                <iframe
                    key={key}
                    src={docPath}
                    className="w-full h-full border-0"
                    title="Documentation"
                    onLoad={() => setIsLoading(false)}
                    style={{ backgroundColor: '#0d1117' }}
                />
            </div>
        </div>
    );
}
