import { useState, useEffect } from 'react';
import { Play, ChevronDown, Trash2, FileText, Clock } from 'lucide-react';
import { Button } from './ui/button';
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from './ui/select';
import { Input } from './ui/input';
import { ScrollArea } from './ui/scroll-area';
import { useAppStore } from '../store/useAppStore';

interface RunnerTabProps {
  endpoint: {
    id: string;
    method: 'GET' | 'POST' | 'PUT' | 'DELETE' | 'PATCH';
    path: string;
    name: string;
  };
  baseUrl: string;
}

export function RunnerTab({
  endpoint,
  baseUrl,
}: RunnerTabProps) {
  const { getCurrentVersion, updateRunnerData, deleteTestCase } = useAppStore();
  
  const currentVersion = getCurrentVersion();
  const requestBody = currentVersion?.runnerData.requestBody || '{}';
  const testCases = currentVersion?.runnerData.testCases || [];
  
  const [method, setMethod] = useState<string>(endpoint.method);
  const [isLoading, setIsLoading] = useState(false);
  const [response, setResponse] = useState<{
    status: number;
    statusText: string;
    time: number;
    body: string;
  } | null>(null);

  const handleSend = async () => {
    setIsLoading(true);
    const startTime = Date.now();

    // Simulate API call
    setTimeout(() => {
      const endTime = Date.now();
      setResponse({
        status: 200,
        statusText: 'OK',
        time: endTime - startTime,
        body: JSON.stringify(
          {
            [endpoint.name.toUpperCase()]: {
              status: 'success',
              result: 'Created',
              data: JSON.parse(requestBody || '{}'),
            },
          },
          null,
          2
        ),
      });
      setIsLoading(false);
    }, 800);
  };

  const handleDeleteTestCase = (caseId: string, e: React.MouseEvent) => {
    e.stopPropagation();
    if (confirm('이 Test Case를 삭제하시겠습니까?')) {
      deleteTestCase(caseId);
    }
  };

  const fullUrl = `${baseUrl}${endpoint.path}`;

  return (
    <div className="flex h-full w-full">
      {/* Left Sidebar - Test Case List */}
      <div className="w-80 flex flex-col border-r border-zinc-800 bg-zinc-950 flex-shrink-0">
        <div className="p-4 border-b border-zinc-800 bg-zinc-900 flex-shrink-0">
          <h3 className="text-sm font-semibold mb-1">🧪 Test Cases</h3>
          <p className="text-xs text-zinc-500">Select a case to run</p>
        </div>

        <ScrollArea className="flex-1 h-0">
          <div className="p-4 space-y-2">
            {testCases.length === 0 ? (
              <div className="text-center py-8">
                <FileText className="w-12 h-12 mx-auto mb-3 text-zinc-700" />
                <p className="text-sm text-zinc-500 mb-1">No test cases yet</p>
                <p className="text-xs text-zinc-600">
                  Create one in the Builder tab
                </p>
              </div>
            ) : (
              testCases.map((testCase) => (
                <div
                  key={testCase.id}
                  className="group relative p-3 rounded-lg border bg-zinc-900 border-zinc-800 hover:border-zinc-700 hover:bg-zinc-800 transition-all"
                >
                  {/* Delete Button */}
                  <Button
                    size="sm"
                    variant="ghost"
                    onClick={(e) => handleDeleteTestCase(testCase.id, e)}
                    className="absolute top-2 right-2 opacity-0 group-hover:opacity-100 transition-opacity h-6 w-6 p-0 text-red-400 hover:text-red-300 hover:bg-red-900/20"
                  >
                    <Trash2 className="w-3 h-3" />
                  </Button>

                  <div className="flex items-start gap-2 mb-2">
                    <div className="w-2 h-2 rounded-full mt-1.5 flex-shrink-0 bg-zinc-600" />
                    <div className="flex-1 min-w-0">
                      <h4 className="text-sm font-semibold text-zinc-100 truncate">
                        {testCase.name}
                      </h4>
                      {testCase.description && (
                        <p className="text-xs text-zinc-400 mt-1 line-clamp-2">
                          {testCase.description}
                        </p>
                      )}
                    </div>
                  </div>

                  <div className="flex items-center gap-2 text-xs text-zinc-500 mt-2">
                    <Clock className="w-3 h-3" />
                    <span>{new Date(testCase.createdAt).toLocaleDateString()}</span>
                  </div>
                </div>
              ))
            )}
          </div>
        </ScrollArea>
      </div>

      {/* Right Side - Runner */}
      <div className="flex-1 flex flex-col overflow-hidden">
        {/* Top Bar - Execution Controls */}
        <div className="border-b border-zinc-800 p-4 bg-zinc-900 flex-shrink-0">
          <div className="flex items-center gap-3">
            {/* Method Selector */}
            <Select value={method} onValueChange={setMethod}>
              <SelectTrigger className="w-32 bg-zinc-800 border-zinc-700">
                <SelectValue />
              </SelectTrigger>
              <SelectContent>
                <SelectItem value="GET">GET</SelectItem>
                <SelectItem value="POST">POST</SelectItem>
                <SelectItem value="PUT">PUT</SelectItem>
                <SelectItem value="DELETE">DELETE</SelectItem>
                <SelectItem value="PATCH">PATCH</SelectItem>
              </SelectContent>
            </Select>

            {/* URL Input */}
            <Input
              value={fullUrl}
              readOnly
              className="flex-1 bg-zinc-800 border-zinc-700 text-sm font-mono"
            />

            {/* Send Button */}
            <Button
              onClick={handleSend}
              disabled={isLoading}
              className="bg-green-600 hover:bg-green-700 min-w-24"
            >
              <Play className="w-4 h-4 mr-2" />
              {isLoading ? 'Sending...' : 'SEND'}
            </Button>
          </div>
        </div>

        {/* Split View - Request & Response */}
        <div className="flex-1 flex min-h-0">
          {/* Left Side - Request Body */}
          <div className="flex-1 flex flex-col border-r border-zinc-800">
            <div className="px-4 py-2 bg-zinc-900 border-b border-zinc-800 text-sm text-zinc-400">
              📤 Request Body
            </div>
            <ScrollArea className="flex-1">
              <textarea
                value={requestBody}
                onChange={(e) => updateRunnerData({ requestBody: e.target.value })}
                className="w-full h-full min-h-[400px] p-4 bg-zinc-950 text-zinc-100 font-mono text-sm resize-none focus:outline-none"
                placeholder="Enter request body JSON..."
              />
            </ScrollArea>
          </div>

          {/* Right Side - Response */}
          <div className="flex-1 flex flex-col">
            <div className="px-4 py-2 bg-zinc-900 border-b border-zinc-800 flex items-center justify-between">
              <span className="text-sm text-zinc-400">📥 Response (Result)</span>
              {response && (
                <span className="text-xs text-zinc-500">
                  HTTP/{response.status} {response.statusText} · Time: {response.time}ms
                </span>
              )}
            </div>
            <ScrollArea className="flex-1">
              <div className="p-4 bg-zinc-950">
                {response ? (
                  <pre className="text-zinc-100 font-mono text-sm whitespace-pre-wrap">
                    {response.body}
                  </pre>
                ) : (
                  <div className="text-zinc-600 text-sm">
                    Click SEND to execute the request and see the response here.
                  </div>
                )}
              </div>
            </ScrollArea>
          </div>
        </div>
      </div>
    </div>
  );
}
