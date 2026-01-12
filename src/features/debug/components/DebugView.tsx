import { Tabs, TabsContent, TabsList, TabsTrigger } from '@/components/ui/tabs';
import { Database, Bug, Network, FileCode, Upload } from 'lucide-react';
import { DatabaseTab, ImportTab } from './tabs';

export function DebugView() {
  return (
    <div className="flex-1 flex flex-col bg-zinc-950 overflow-hidden">
      {/* Header */}
      <div className="border-b border-zinc-800 bg-zinc-900 px-6 py-4 flex-shrink-0">
        <div>
          <h2 className="text-xl font-bold flex items-center gap-2">
            <Bug className="w-5 h-5 text-orange-400" />
            Debug Tools
          </h2>
          <p className="text-sm text-zinc-500 mt-1">
            개발 및 디버깅 도구 모음
          </p>
        </div>
      </div>

      {/* Tabs */}
      <Tabs defaultValue="database" className="flex flex-col h-full overflow-hidden">
        {/* Tab Headers */}
        <div className="border-b border-zinc-800 bg-zinc-900 px-6 pt-3 flex-shrink-0">
          <TabsList className="bg-zinc-800 h-10 p-1 rounded-lg">
            <TabsTrigger
              value="database"
              className="px-4 py-2 rounded-md text-sm font-medium transition-colors data-[state=active]:bg-zinc-700 data-[state=active]:text-white text-zinc-400 hover:text-zinc-200"
            >
              <Database className="w-4 h-4 mr-2" />
              🗄️ Database
            </TabsTrigger>
            <TabsTrigger
              value="import"
              className="px-4 py-2 rounded-md text-sm font-medium transition-colors data-[state=active]:bg-zinc-700 data-[state=active]:text-white text-zinc-400 hover:text-zinc-200"
            >
              <Upload className="w-4 h-4 mr-2" />
              📥 Import
            </TabsTrigger>
            <TabsTrigger
              value="network"
              disabled
              className="px-4 py-2 rounded-md text-sm font-medium transition-colors data-[state=active]:bg-zinc-700 data-[state=active]:text-white text-zinc-400 hover:text-zinc-200 disabled:opacity-30 disabled:cursor-not-allowed"
            >
              <Network className="w-4 h-4 mr-2" />
              🌐 Network
            </TabsTrigger>
            <TabsTrigger
              value="logs"
              disabled
              className="px-4 py-2 rounded-md text-sm font-medium transition-colors data-[state=active]:bg-zinc-700 data-[state=active]:text-white text-zinc-400 hover:text-zinc-200 disabled:opacity-30 disabled:cursor-not-allowed"
            >
              <FileCode className="w-4 h-4 mr-2" />
              📋 Logs
            </TabsTrigger>
          </TabsList>
        </div>

        {/* Tab Contents */}
        <div className="flex-1 overflow-hidden">
          <TabsContent value="database" className="h-full m-0">
            <DatabaseTab />
          </TabsContent>
          
          <TabsContent value="import" className="h-full m-0">
            <ImportTab />
          </TabsContent>
          
          <TabsContent value="network" className="h-full m-0">
            <div className="flex items-center justify-center h-full">
              <div className="text-center text-zinc-500">
                <Network className="w-12 h-12 mx-auto mb-4 opacity-50" />
                <p className="text-lg font-medium">Network Monitor</p>
                <p className="text-sm mt-2">Coming Soon...</p>
              </div>
            </div>
          </TabsContent>
          
          <TabsContent value="logs" className="h-full m-0">
            <div className="flex items-center justify-center h-full">
              <div className="text-center text-zinc-500">
                <FileCode className="w-12 h-12 mx-auto mb-4 opacity-50" />
                <p className="text-lg font-medium">Application Logs</p>
                <p className="text-sm mt-2">Coming Soon...</p>
              </div>
            </div>
          </TabsContent>
        </div>
      </Tabs>
    </div>
  );
}

