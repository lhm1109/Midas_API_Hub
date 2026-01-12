import { Gem, FolderOpen, History, Settings } from 'lucide-react';
import { Button } from './ui/button';
import { Tooltip, TooltipContent, TooltipProvider, TooltipTrigger } from './ui/tooltip';

interface GlobalSidebarProps {
  activeView: 'projects' | 'history';
  onViewChange: (view: 'projects' | 'history') => void;
  onSettingsClick: () => void;
}

export function GlobalSidebar({ activeView, onViewChange, onSettingsClick }: GlobalSidebarProps) {
  return (
    <div className="w-14 bg-zinc-900 flex flex-col items-center py-4 border-r border-zinc-800">
      <TooltipProvider>
        {/* Logo */}
        <div className="mb-6">
          <Gem className="w-6 h-6 text-blue-400" />
        </div>

        {/* Navigation Items */}
        <div className="flex-1 flex flex-col gap-2">
          <Tooltip>
            <TooltipTrigger asChild>
              <div>
                <Button
                  variant="ghost"
                  size="icon"
                  className={`w-10 h-10 ${
                    activeView === 'projects'
                      ? 'bg-zinc-800 text-white'
                      : 'text-zinc-400 hover:text-white hover:bg-zinc-800'
                  }`}
                  onClick={() => onViewChange('projects')}
                >
                  <FolderOpen className="w-5 h-5" />
                </Button>
              </div>
            </TooltipTrigger>
            <TooltipContent side="right">
              <p>Projects</p>
            </TooltipContent>
          </Tooltip>

          <Tooltip>
            <TooltipTrigger asChild>
              <div>
                <Button
                  variant="ghost"
                  size="icon"
                  className={`w-10 h-10 ${
                    activeView === 'history'
                      ? 'bg-zinc-800 text-white'
                      : 'text-zinc-400 hover:text-white hover:bg-zinc-800'
                  }`}
                  onClick={() => onViewChange('history')}
                >
                  <History className="w-5 h-5" />
                </Button>
              </div>
            </TooltipTrigger>
            <TooltipContent side="right">
              <p>History</p>
            </TooltipContent>
          </Tooltip>
        </div>

        {/* Settings at bottom */}
        <Tooltip>
          <TooltipTrigger asChild>
            <div>
              <Button
                variant="ghost"
                size="icon"
                className="w-10 h-10 text-zinc-400 hover:text-white hover:bg-zinc-800"
                onClick={onSettingsClick}
              >
                <Settings className="w-5 h-5" />
              </Button>
            </div>
          </TooltipTrigger>
          <TooltipContent side="right">
            <p>Settings</p>
          </TooltipContent>
        </Tooltip>
      </TooltipProvider>
    </div>
  );
}