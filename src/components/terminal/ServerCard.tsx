/**
 * ServerCard - 개별 서버 상태 및 제어 카드
 */

import React from 'react';
import { Play, Square } from 'lucide-react';
import { Button } from '@/components/ui/button';
import { ServerConfig } from './serverConfig';

interface ServerCardProps {
    server: ServerConfig;
    isRunning: boolean;
    onStart: () => void;
    onStop: () => void;
    onSelect: () => void;
    isSelected: boolean;
}

export const ServerCard: React.FC<ServerCardProps> = ({
    server,
    isRunning,
    onStart,
    onStop,
    onSelect,
    isSelected,
}) => {
    const baseClasses = 'flex items-center gap-3 p-3 rounded-lg border transition-all cursor-pointer';
    const selectedClasses = isSelected
        ? 'bg-cyan-900/30 border-cyan-500/50'
        : 'bg-zinc-800/50 border-zinc-700/50 hover:bg-zinc-800';
    const runningClasses = isRunning ? 'border-l-2 border-l-green-500' : '';

    return (
        <div
            className={`${baseClasses} ${selectedClasses} ${runningClasses}`}
            onClick={onSelect}
        >
            {/* Icon */}
            <div className="text-2xl">{server.icon}</div>

            {/* Info */}
            <div className="flex-1 min-w-0">
                <div className="flex items-center gap-2">
                    <span className="font-medium text-sm text-zinc-100 truncate">
                        {server.name}
                    </span>
                    {isRunning && (
                        <span className="flex items-center gap-1 text-xs text-green-400">
                            <span className="w-1.5 h-1.5 bg-green-400 rounded-full animate-pulse" />
                            Running
                        </span>
                    )}
                </div>
                <div className="text-xs text-zinc-500 truncate">
                    {server.description || server.command}
                </div>
            </div>

            {/* Actions */}
            <div className="flex items-center gap-1">
                {!isRunning ? (
                    <Button
                        variant="ghost"
                        size="icon"
                        className="h-8 w-8 text-green-400 hover:text-green-300 hover:bg-green-900/30"
                        onClick={(e) => {
                            e.stopPropagation();
                            onStart();
                        }}
                    >
                        <Play className="h-4 w-4" />
                    </Button>
                ) : (
                    <Button
                        variant="ghost"
                        size="icon"
                        className="h-8 w-8 text-red-400 hover:text-red-300 hover:bg-red-900/30"
                        onClick={(e) => {
                            e.stopPropagation();
                            onStop();
                        }}
                    >
                        <Square className="h-4 w-4" />
                    </Button>
                )}
            </div>
        </div>
    );
};

export default ServerCard;
