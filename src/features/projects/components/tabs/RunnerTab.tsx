import { useState, useEffect } from 'react';
import { Play, ChevronDown, Trash2, FileText, Clock, Send } from 'lucide-react';
import { Button } from '@/components/ui/button';
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from '@/components/ui/select';
import { Input } from '@/components/ui/input';
import { ScrollArea } from '@/components/ui/scroll-area';
import { CodeEditor } from '@/components/common';
import { useAppStore } from '@/store/useAppStore';
import { apiClient } from '@/lib/api-client';
import {
  Dialog,
  DialogContent,
  DialogDescription,
  DialogFooter,
  DialogHeader,
  DialogTitle,
} from '@/components/ui/dialog';
import { Label } from '@/components/ui/label';
import { toast } from 'sonner';

interface RunnerTabProps {
  endpoint: {
    id: string;
    method: 'GET' | 'POST' | 'PUT' | 'DELETE' | 'PATCH';
    path: string;
    name: string;
  };
  settings: {
    baseUrl: string;
    mapiKey: string;
    commonHeaders: string;
    useAssignWrapper?: boolean;
  };
}

export function RunnerTab({
  endpoint,
  settings,
}: RunnerTabProps) {
  const { runnerData, updateRunnerData, deleteTestCase, manualData, setManualData, saveCurrentVersion } = useAppStore();
  
  const requestBody = runnerData?.requestBody || '{}';
  const testCases = runnerData?.testCases || [];
  
  // 🎯 메뉴얼 데이터가 있으면 그것의 inputUri를 사용, 없으면 현재 endpoint.path 사용
  const endpointPath = manualData?.inputUri || endpoint.path;
  
  const [method, setMethod] = useState<string>(endpoint.method);
  const [isLoading, setIsLoading] = useState(false);
  const [response, setResponse] = useState<{
    status: number;
    statusText: string;
    time: number;
    body: string;
  } | null>(null);
  
  // 🎯 Send to Manual 다이얼로그 상태
  const [showSendToManualDialog, setShowSendToManualDialog] = useState(false);
  const [exampleTitle, setExampleTitle] = useState('');
  
  // 🎯 선택된 Test Case 상태
  const [selectedTestCaseId, setSelectedTestCaseId] = useState<string | null>(null);

  // 🔥 Request Body를 Assign 래퍼로 변환하는 함수
  const wrapWithAssign = (body: string, endpointName: string): string => {
    if (!settings.useAssignWrapper) {
      return body;
    }

    try {
      const parsed = JSON.parse(body);
      
      // 이미 Assign 래퍼가 있으면 그대로 반환
      if (parsed && typeof parsed === 'object' && 'Assign' in parsed) {
        return body;
      }

      // rootKey 추출 (endpoint name을 대문자로)
      const rootKey = endpointName.toUpperCase();
      let dataToWrap = parsed;

      // rootKey가 있으면 그것을 사용 (예: { "NODE": { ... } } -> { ... })
      if (parsed && typeof parsed === 'object' && rootKey in parsed) {
        dataToWrap = parsed[rootKey];
      } else if (parsed && typeof parsed === 'object') {
        // rootKey가 없으면 전체 객체를 사용
        dataToWrap = parsed;
      }

      // Assign 래퍼로 감싸기
      // 단일 객체인 경우 "1" 키로 감싸기
      const wrapped = {
        Assign: {
          "1": dataToWrap
        }
      };

      return JSON.stringify(wrapped, null, 2);
    } catch (error) {
      // JSON 파싱 실패 시 원본 반환
      console.warn('Failed to parse request body for Assign wrapper:', error);
      return body;
    }
  };

  const handleSend = async () => {
    setIsLoading(true);
    const startTime = Date.now();

    try {
      // 🔥 헤더 구성
      const headers: Record<string, string> = {
        'Content-Type': 'application/json',
        'Accept': 'application/json',
      };

      // Common Headers 파싱 및 추가
      try {
        const commonHeaders = JSON.parse(settings.commonHeaders || '{}');
        Object.assign(headers, commonHeaders);
      } catch (error) {
        console.warn('Failed to parse common headers:', error);
      }

      // MAPI-Key 추가
      if (settings.mapiKey) {
        headers['MAPI-Key'] = settings.mapiKey;
      }

      // 🔥 Assign 래퍼 적용 (설정에 따라)
      const finalRequestBody = method !== 'GET' 
        ? wrapWithAssign(requestBody, endpoint.name)
        : undefined;

      // 🔥 실제 API 호출
      const response = await fetch(fullUrl, {
        method: method,
        headers: headers,
        body: finalRequestBody,
      });

      const endTime = Date.now();
      const responseText = await response.text();
      let responseBody = responseText;

      // JSON 파싱 시도
      try {
        const jsonData = JSON.parse(responseText);
        responseBody = JSON.stringify(jsonData, null, 2);
      } catch {
        // JSON이 아니면 그대로 표시
        responseBody = responseText;
      }

      setResponse({
        status: response.status,
        statusText: response.statusText,
        time: endTime - startTime,
        body: responseBody,
      });

      // 응답을 runnerData에 저장
      updateRunnerData({ responseBody });

      // 🎯 엔드포인트 상태 자동 업데이트
      const updateEndpointStatus = async (success: boolean, message: string) => {
        try {
          await apiClient.updateEndpoint(endpoint.id, {
            status: success ? 'success' : 'error',
            statusMessage: message,
          });
          console.log('✅ Endpoint status updated:', success ? 'success' : 'error', message);
        } catch (error) {
          console.error('Failed to update endpoint status:', error);
        }
      };

      // 🎯 성공 토스트 및 상태 업데이트
      if (response.ok) {
        toast.success(`✅ Request successful (${response.status}) - ${endTime - startTime}ms`);
        await updateEndpointStatus(true, `Last tested: ${new Date().toLocaleString()} - ${response.status} ${response.statusText} (${endTime - startTime}ms)`);
      } else {
        toast.error(`⚠️ Request failed (${response.status}) - ${response.statusText}`);
        await updateEndpointStatus(false, `HTTP ${response.status}: ${response.statusText}\nLast tested: ${new Date().toLocaleString()}`);
      }
    } catch (error) {
      const endTime = Date.now();
      setResponse({
        status: 0,
        statusText: 'Network Error',
        time: endTime - startTime,
        body: JSON.stringify(
          {
            error: error instanceof Error ? error.message : 'Unknown error',
            message: 'Failed to fetch. Please check your network connection and CORS settings.',
          },
          null,
          2
        ),
      });

      // 🎯 엔드포인트 상태 자동 업데이트 (네트워크 오류)
      try {
        await apiClient.updateEndpoint(endpoint.id, {
          status: 'error',
          statusMessage: `Network Error: ${error instanceof Error ? error.message : 'Unknown error'}\nLast tested: ${new Date().toLocaleString()}`,
        });
        console.log('✅ Endpoint status updated: error');
      } catch (updateError) {
        console.error('Failed to update endpoint status:', updateError);
      }

      // 🎯 실패 토스트
      toast.error(`❌ Network error: ${error instanceof Error ? error.message : 'Unknown error'}`);
    } finally {
      setIsLoading(false);
    }
  };

  const handleDeleteTestCase = async (caseId: string, e: React.MouseEvent) => {
    e.stopPropagation();
    if (confirm('Are you sure you want to delete this test case?')) {
      deleteTestCase(caseId);
      if (selectedTestCaseId === caseId) {
        setSelectedTestCaseId(null);
      }
      
      // 🔥 글로벌 저장 (DB에 영구 저장)
      try {
        await saveCurrentVersion();
        toast.success('✅ Test case deleted successfully');
      } catch (error) {
        console.error('Failed to save after delete:', error);
        toast.error('❌ Failed to save after deletion');
      }
    }
  };
  
  // 🎯 Test Case 선택 시 Request Body 로드
  const handleLoadTestCase = (testCaseId: string) => {
    const testCase = testCases.find(tc => tc.id === testCaseId);
    if (!testCase) return;
    
    // Request Body 로드
    updateRunnerData({ requestBody: testCase.requestBody });
    setSelectedTestCaseId(testCaseId);
    toast.success(`✅ Test Case "${testCase.name}" loaded successfully`);
  };
  
  // 🎯 Send to Manual 함수
  const handleSendToManual = () => {
    if (!exampleTitle.trim()) {
      toast.error('Example title is required');
      return;
    }
    
    if (!response) {
      toast.error('No response available. Please run the request first.');
      return;
    }
    
    // JSON을 색상 있는 HTML로 변환
    const formatJsonToHTML = (jsonStr: string, isRequest: boolean = false): string => {
      try {
        const jsonObj = JSON.parse(jsonStr);
        const formatted = JSON.stringify(jsonObj, null, 2);
        
        return formatted
          .split('\n')
          .map(line => {
            // 공백을 &nbsp;로 변환하여 들여쓰기 유지
            const leadingSpaces = line.match(/^(\s*)/)?.[1] || '';
            const indent = leadingSpaces.replace(/ /g, '&nbsp;&nbsp;');
            const trimmedLine = line.trim();
            
            // 키-값 패턴 매칭
            const keyMatch = trimmedLine.match(/^"([^"]+)":\s*(.+)$/);
            if (keyMatch) {
              const key = keyMatch[1];
              let value = keyMatch[2];
              const hasComma = value.endsWith(',');
              if (hasComma) {
                value = value.slice(0, -1);
              }
              
              // 값의 타입에 따라 색상 적용
              let styledValue = value;
              if (value === 'true' || value === 'false') {
                styledValue = `<span style="color: #055bcc; font-weight: bold;">${value}</span>`;
              } else if (value.match(/^"[^"]*"$/)) {
                styledValue = `<span style="color: #055bcc;">${value}</span>`;
              } else if (value.match(/^-?\d+(\.\d+)?$/)) {
                styledValue = `<span style="color: #0ab66c;">${value}</span>`;
              } else if (value === '{' || value === '[') {
                styledValue = value;
              }
              
              const styledLine = `${indent}<span style="color: #c31b1b;">"${key}"</span>: ${styledValue}${hasComma ? ',' : ''}`;
              return styledLine;
            }
            
            // 중괄호, 대괄호만 있는 라인
            if (trimmedLine.match(/^[{\[\}\]],?$/)) {
              return indent + trimmedLine;
            }
            
            return indent + trimmedLine;
          })
          .join('<br>');
      } catch (e) {
        // JSON 파싱 실패 시 원본 반환
        return jsonStr.replace(/\n/g, '<br>').replace(/ /g, '&nbsp;');
      }
    };
    
    // Request와 Response를 HTML로 변환
    const requestHTML = formatJsonToHTML(requestBody);
    const responseHTML = formatJsonToHTML(response.body);
    
    // 🎯 Request Example 생성
    const newRequestExample = {
      title: exampleTitle.trim(),
      code: requestHTML
    };
    
    // 🎯 Response Example 생성
    const newResponseExample = {
      title: exampleTitle.trim(),
      code: responseHTML
    };
    
    // Manual Data 업데이트 - Request/Response 분리
    const updatedManualData = {
      ...manualData,
      title: manualData?.title || `${endpoint.name} Manual`,
      category: manualData?.category || endpoint.method,
      inputUri: manualData?.inputUri || endpoint.path,
      activeMethods: manualData?.activeMethods || endpoint.method,
      jsonSchema: manualData?.jsonSchema || '',
      examples: manualData?.examples || [],  // deprecated
      requestExamples: [...(manualData?.requestExamples || []), newRequestExample],
      responseExamples: [...(manualData?.responseExamples || []), newResponseExample],
      specifications: manualData?.specifications || '',
    };
    
    setManualData(updatedManualData);
    setShowSendToManualDialog(false);
    setExampleTitle('');
    toast.success(`✅ Example "${exampleTitle}" added to Manual tab!`);
  };

  const fullUrl = `${settings.baseUrl}${endpointPath}`;

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
                  onClick={() => handleLoadTestCase(testCase.id)}
                  className={`group relative p-3 rounded-lg border cursor-pointer transition-all ${
                    selectedTestCaseId === testCase.id
                      ? 'bg-blue-900/20 border-blue-700'
                      : 'bg-zinc-900 border-zinc-800 hover:border-zinc-700 hover:bg-zinc-800'
                  }`}
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
                    <div className={`w-2 h-2 rounded-full mt-1.5 flex-shrink-0 ${
                      selectedTestCaseId === testCase.id ? 'bg-blue-500' : 'bg-zinc-600'
                    }`} />
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
            <div className="flex-1 h-full">
              <CodeEditor
                value={requestBody}
                onChange={(value) => updateRunnerData({ requestBody: value || '{}' })}
                language="json"
                minimap={false}
              />
            </div>
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
            <div className="flex-1 h-full">
              {response ? (
                <CodeEditor
                  value={response.body}
                  onChange={() => {}}
                  language="json"
                  readOnly={true}
                  minimap={false}
                />
              ) : (
                <div className="p-4 text-zinc-600 text-sm">
                  Click SEND to execute the request and see the response here.
                </div>
              )}
            </div>
          </div>
        </div>
        
        {/* Footer with Send to Manual Button */}
        <div className="border-t border-zinc-800 bg-zinc-900 p-4 flex items-center justify-end flex-shrink-0">
          <Button
            onClick={() => setShowSendToManualDialog(true)}
            disabled={!response}
            size="sm"
            className="h-8 text-xs bg-blue-600 hover:bg-blue-500"
          >
            <Send className="w-3 h-3 mr-2" />
            Send to Manual
          </Button>
        </div>
      </div>
      
      {/* 🎯 Send to Manual Dialog */}
      <Dialog open={showSendToManualDialog} onOpenChange={setShowSendToManualDialog}>
        <DialogContent className="bg-zinc-900 border-zinc-700 max-w-md">
          <DialogHeader>
            <DialogTitle className="text-white">Send to Manual Tab</DialogTitle>
            <DialogDescription className="text-zinc-400">
              Add this test case as an example in the Manual tab
            </DialogDescription>
          </DialogHeader>
          
          <div className="space-y-4 py-4">
            <div className="space-y-2">
              <Label htmlFor="example-title" className="text-zinc-200">
                Example Title
              </Label>
              <Input
                id="example-title"
                value={exampleTitle}
                onChange={(e) => setExampleTitle(e.target.value)}
                placeholder="e.g., Basic POST Request Example"
                className="bg-zinc-800 border-zinc-700 text-white"
                autoFocus
              />
              <p className="text-xs text-zinc-400">
                This title will be used in the Manual tab's Examples section
              </p>
            </div>
            
            {response && (
              <div className="p-3 bg-zinc-800/50 rounded-lg border border-zinc-700">
                <p className="text-xs text-zinc-400 mb-2">Preview:</p>
                <div className="space-y-1 text-xs">
                  <p className="text-zinc-300">
                    <span className="text-zinc-500">Status:</span> {response.status} {response.statusText}
                  </p>
                  <p className="text-zinc-300">
                    <span className="text-zinc-500">Time:</span> {response.time}ms
                  </p>
                  <p className="text-zinc-300">
                    <span className="text-zinc-500">Request Body:</span> {requestBody.length} characters
                  </p>
                  <p className="text-zinc-300">
                    <span className="text-zinc-500">Response Body:</span> {response.body.length} characters
                  </p>
                </div>
              </div>
            )}
          </div>
          
          <DialogFooter>
            <Button
              variant="outline"
              onClick={() => {
                setShowSendToManualDialog(false);
                setExampleTitle('');
              }}
              className="border-zinc-700 text-zinc-200"
            >
              Cancel
            </Button>
            <Button
              onClick={handleSendToManual}
              disabled={!exampleTitle.trim()}
              className="bg-blue-600 hover:bg-blue-500"
            >
              <Send className="w-3 h-3 mr-2" />
              Add to Manual
            </Button>
          </DialogFooter>
        </DialogContent>
      </Dialog>
    </div>
  );
}
