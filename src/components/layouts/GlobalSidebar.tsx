import { Gem, FolderOpen, History, Bug, Settings, BookOpen, PackageOpen, Paintbrush, BarChart3, Terminal, Database, Library, Bot } from 'lucide-react';
import { Button } from '@/components/ui/button';
import { Tooltip, TooltipContent, TooltipProvider, TooltipTrigger } from '@/components/ui/tooltip';

interface GlobalSidebarProps {
  activeView:
    | 'ai'
    | 'terminal'
    | 'manager'
    | 'projects'
    | 'manualHub'
    | 'history'
    | 'docs'
    | 'debug'
    | 'schema'
    | 'builder'
    | 'database';
  onViewChange: (view: GlobalSidebarProps['activeView']) => void;
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
          {/* Nimbalyst - First tab */}
          <Tooltip>
            <TooltipTrigger asChild>
              <div>
                <Button
                  variant="ghost"
                  size="icon"
                  className={`w-10 h-10 ${activeView === 'ai'
                    ? 'bg-blue-900/50 text-blue-400 ring-1 ring-blue-500/50'
                    : 'text-zinc-400 hover:text-blue-400 hover:bg-zinc-800'
                    }`}
                  onClick={() => onViewChange('ai')}
                >
                  <Bot className="w-5 h-5" />
                </Button>
              </div>
            </TooltipTrigger>
            <TooltipContent side="right">
              <p>Nimbalyst</p>
            </TooltipContent>
          </Tooltip>

          <div className="w-6 border-b border-zinc-700 mx-auto my-1" />

          {/* Terminal - Top priority */}
          <Tooltip>
            <TooltipTrigger asChild>
              <div>
                <Button
                  variant="ghost"
                  size="icon"
                  className={`w-10 h-10 ${activeView === 'terminal'
                    ? 'bg-cyan-900/50 text-cyan-400 ring-1 ring-cyan-500/50'
                    : 'text-zinc-400 hover:text-cyan-400 hover:bg-zinc-800'
                    }`}
                  onClick={() => onViewChange('terminal')}
                >
                  <Terminal className="w-5 h-5" />
                </Button>
              </div>
            </TooltipTrigger>
            <TooltipContent side="right">
              <p>Servers</p>
            </TooltipContent>
          </Tooltip>

          <div className="w-6 border-b border-zinc-700 mx-auto my-1" />

          <Tooltip>
            <TooltipTrigger asChild>
              <div>
                <Button
                  variant="ghost"
                  size="icon"
                  className={`w-10 h-10 ${activeView === 'manager'
                    ? 'bg-zinc-800 text-white'
                    : 'text-zinc-400 hover:text-white hover:bg-zinc-800'
                    }`}
                  onClick={() => onViewChange('manager')}
                >
                  <BarChart3 className="w-5 h-5" />
                </Button>
              </div>
            </TooltipTrigger>
            <TooltipContent side="right">
              <p>Manager</p>
            </TooltipContent>
          </Tooltip>

          <Tooltip>
            <TooltipTrigger asChild>
              <div>
                <Button
                  variant="ghost"
                  size="icon"
                  className={`w-10 h-10 ${activeView === 'projects'
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
                  className={`w-10 h-10 ${activeView === 'manualHub'
                    ? 'bg-zinc-800 text-white'
                    : 'text-zinc-400 hover:text-white hover:bg-zinc-800'
                    }`}
                  onClick={() => onViewChange('manualHub')}
                >
                  <Library className="w-5 h-5" />
                </Button>
              </div>
            </TooltipTrigger>
            <TooltipContent side="right">
              <p>Manual Hub</p>
            </TooltipContent>
          </Tooltip>

          <Tooltip>
            <TooltipTrigger asChild>
              <div>
                <Button
                  variant="ghost"
                  size="icon"
                  className={`w-10 h-10 ${activeView === 'history'
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

          <Tooltip>
            <TooltipTrigger asChild>
              <div>
                <Button
                  variant="ghost"
                  size="icon"
                  className={`w-10 h-10 ${activeView === 'docs'
                    ? 'bg-zinc-800 text-white'
                    : 'text-zinc-400 hover:text-white hover:bg-zinc-800'
                    }`}
                  onClick={() => onViewChange('docs')}
                >
                  <BookOpen className="w-5 h-5" />
                </Button>
              </div>
            </TooltipTrigger>
            <TooltipContent side="right">
              <p>API Docs</p>
            </TooltipContent>
          </Tooltip>

          <Tooltip>
            <TooltipTrigger asChild>
              <div>
                <Button
                  variant="ghost"
                  size="icon"
                  className={`w-10 h-10 ${activeView === 'schema'
                    ? 'bg-zinc-800 text-white'
                    : 'text-zinc-400 hover:text-white hover:bg-zinc-800'
                    }`}
                  onClick={() => onViewChange('schema')}
                >
                  <PackageOpen className="w-5 h-5" />
                </Button>
              </div>
            </TooltipTrigger>
            <TooltipContent side="right">
              <p>Schema Definition (PSD)</p>
            </TooltipContent>
          </Tooltip>

          <Tooltip>
            <TooltipTrigger asChild>
              <div>
                <Button
                  variant="ghost"
                  size="icon"
                  className={`w-10 h-10 ${activeView === 'builder'
                    ? 'bg-zinc-800 text-white'
                    : 'text-zinc-400 hover:text-white hover:bg-zinc-800'
                    }`}
                  onClick={() => onViewChange('builder')}
                >
                  <Paintbrush className="w-5 h-5" />
                </Button>
              </div>
            </TooltipTrigger>
            <TooltipContent side="right">
              <p>Schema Builder</p>
            </TooltipContent>
          </Tooltip>

          <Tooltip>
            <TooltipTrigger asChild>
              <div>
                <Button
                  variant="ghost"
                  size="icon"
                  className={`w-10 h-10 ${activeView === 'debug'
                    ? 'bg-zinc-800 text-white'
                    : 'text-zinc-400 hover:text-white hover:bg-zinc-800'
                    }`}
                  onClick={() => onViewChange('debug')}
                >
                  <Bug className="w-5 h-5" />
                </Button>
              </div>
            </TooltipTrigger>
            <TooltipContent side="right">
              <p>Debug</p>
            </TooltipContent>
          </Tooltip>

          <Tooltip>
            <TooltipTrigger asChild>
              <div>
                <Button
                  variant="ghost"
                  size="icon"
                  className={`w-10 h-10 ${activeView === 'database'
                    ? 'bg-green-900/50 text-green-400 ring-1 ring-green-500/50'
                    : 'text-zinc-400 hover:text-green-400 hover:bg-zinc-800'
                    }`}
                  onClick={() => onViewChange('database')}
                >
                  <Database className="w-5 h-5" />
                </Button>
              </div>
            </TooltipTrigger>
            <TooltipContent side="right">
              <p>Database</p>
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
