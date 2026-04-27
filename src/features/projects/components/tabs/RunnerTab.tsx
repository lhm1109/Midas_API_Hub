import { useEffect, useMemo, useRef, useState } from 'react';
import { Play, Trash2, FileText, Clock, Send, Save, LayoutGrid } from 'lucide-react';
import { Button } from '@/components/ui/button';
import { formatJsonToHTML } from '@/lib/utils/htmlFormatter';
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
import { parseRunnerJsonTable, parseRunnerResultTable } from './runnerResultTable.logic';
import { RunnerResultTableDialog } from './RunnerResultTableDialog';

interface RunnerTabProps {
  endpoint: {
    id: string;
    method: string; // 콤마 구분 복수 가능 (예: "GET,POST,PUT,DELETE")
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
  const { runnerData, updateRunnerData, updateTestCase, deleteTestCase, manualData, setManualData, saveCurrentVersion } = useAppStore();

  const requestBody = runnerData?.requestBody || '{}';
  const testCases = runnerData?.testCases || [];
  const selectedTestCaseId = runnerData?.selectedTestCaseId || null;
  const selectedTestCaseDraftBody = runnerData?.selectedTestCaseDraftBody || null;

  // 🎯 메뉴얼 데이터가 있으면 그것의 inputUri를 사용, 없으면 현재 endpoint.path 사용
  const endpointPath = manualData?.inputUri || endpoint.path;
  const defaultUrl = `${settings.baseUrl}${endpointPath}`;

  // 엔드포인트에 지정된 사용 가능한 메서드 목록 파싱
  const availableMethods = endpoint.method
    ? endpoint.method.split(',').map((m) => m.trim()).filter(Boolean)
    : ['GET', 'POST', 'PUT', 'DELETE', 'PATCH'];

  const [method, setMethod] = useState<string>(availableMethods[0] || 'POST');
  const [isLoading, setIsLoading] = useState(false);
  const [response, setResponse] = useState<{
    status: number;
    statusText: string;
    time: number;
    body: string;
  } | null>(null);

  // 🎯 Send to Manual 다이얼로그 상태
  const [showSendToManualDialog, setShowSendToManualDialog] = useState(false);
  const [exampleTitle, setExampleTitle] = useState('Example');
  const [sendToManualRequestBody, setSendToManualRequestBody] = useState('');
  const [sendToManualResponseBody, setSendToManualResponseBody] = useState('');
  const [showRequestTableDialog, setShowRequestTableDialog] = useState(false);
  const [showResultTableDialog, setShowResultTableDialog] = useState(false);
  const endpointIdRef = useRef(endpoint.id);

  const parsedRequestTable = useMemo(() => {
    return parseRunnerJsonTable(requestBody, {
      title: 'Request Body',
      description: 'Current request body table preview',
    });
  }, [requestBody]);

  const parsedResultTable = useMemo(() => {
    if (!response?.body) return null;
    return parseRunnerResultTable(response.body) || parseRunnerJsonTable(response.body, {
      title: 'Response Body',
      description: 'Current response body table preview',
    });
  }, [response?.body]);

  // URL 입력값 초기화/유지
  // - 최초 진입: 기본 URL 세팅
  // - endpoint 변경: 해당 endpoint의 기본 URL로 재설정
  useEffect(() => {
    const endpointChanged = endpointIdRef.current !== endpoint.id;
    if (endpointChanged) {
      endpointIdRef.current = endpoint.id;
      if (runnerData?.url !== defaultUrl) {
        updateRunnerData({ url: defaultUrl });
      }
      return;
    }

    if (runnerData?.url === undefined) {
      updateRunnerData({ url: defaultUrl });
    }
  }, [endpoint.id, defaultUrl, runnerData?.url, updateRunnerData]);

  // 탭 재진입/외부 탭 이동 후에도 선택된 Test Case body를 유지
  useEffect(() => {
    if (!selectedTestCaseId) return;
    if (testCases.length === 0) return;

    const testCase = testCases.find(tc => tc.id === selectedTestCaseId);
    if (!testCase) {
      return;
    }

    const bodyToRestore = selectedTestCaseDraftBody ?? testCase.requestBody;
    if (requestBody !== bodyToRestore) {
      updateRunnerData({ requestBody: bodyToRestore });
    }
  }, [requestBody, selectedTestCaseDraftBody, selectedTestCaseId, testCases, updateRunnerData]);

  useEffect(() => {
    if (parsedRequestTable) return;
    if (showRequestTableDialog) {
      setShowRequestTableDialog(false);
    }
  }, [parsedRequestTable, showRequestTableDialog]);

  useEffect(() => {
    if (parsedResultTable) return;
    if (showResultTableDialog) {
      setShowResultTableDialog(false);
    }
  }, [parsedResultTable, showResultTableDialog]);

  // 🔥 Request Body를 Assign 래퍼로 변환하는 함수
  const wrapWithAssign = (body: string, endpointName: string): string => {
    if (!settings.useAssignWrapper) {
      return body;
    }

    try {
      const parsed = JSON.parse(body);

      // 이미 canonical wrapper가 있으면 그대로 반환
      if (
        parsed &&
        typeof parsed === 'object' &&
        ['Assign', 'Argument', 'MCD'].some((key) => key in parsed)
      ) {
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

      const targetUrl = (runnerData?.url ?? defaultUrl).trim();
      if (!targetUrl) {
        toast.error('Please enter a valid request URL.');
        setIsLoading(false);
        return;
      }

      // 🔥 실제 API 호출
      const response = await fetch(targetUrl, {
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
        updateRunnerData({ selectedTestCaseId: null, selectedTestCaseDraftBody: null });
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

  // 🎯 Test Case 선택 시 Request Body + Response 로드
  const handleLoadTestCase = (testCaseId: string) => {
    const testCase = testCases.find(tc => tc.id === testCaseId);
    if (!testCase) return;

    // Request Body 로드
    updateRunnerData({
      requestBody: testCase.requestBody,
      selectedTestCaseId: testCaseId,
      selectedTestCaseDraftBody: testCase.requestBody,
    });

    // 🔥 Response도 함께 로드
    if (testCase.responseBody) {
      setResponse({
        status: testCase.responseStatus || 200,
        statusText: 'Loaded',
        time: testCase.responseTime || 0,
        body: testCase.responseBody,
      });
      toast.success(`✅ Test Case "${testCase.name}" loaded (with response)`);
    } else {
      setResponse(null);  // 저장된 응답이 없으면 초기화
      toast.success(`✅ Test Case "${testCase.name}" loaded`);
    }
  };

  // 🎯 Save Test Case 함수: 현재 Response를 선택된 Test Case에 저장
  const handleSaveTestCase = async () => {
    if (!selectedTestCaseId) {
      toast.error('Please select a test case first');
      return;
    }

    if (!response) {
      toast.error('No response to save. Run the request first.');
      return;
    }

    try {
      // Test Case 업데이트
      updateTestCase(selectedTestCaseId, {
        requestBody: requestBody,
        responseBody: response.body,
        responseStatus: response.status,
        responseTime: response.time,
        updatedAt: new Date().toISOString(),
      });
      updateRunnerData({ selectedTestCaseDraftBody: requestBody });

      // DB에 저장
      await saveCurrentVersion();
      toast.success('✅ Test case saved with response!');
    } catch (error) {
      console.error('Failed to save test case:', error);
      toast.error('❌ Failed to save test case');
    }
  };

  const resetSendToManualDialog = () => {
    setShowSendToManualDialog(false);
    setExampleTitle('Example');
    setSendToManualRequestBody('');
    setSendToManualResponseBody('');
  };

  const openSendToManualDialog = () => {
    setExampleTitle('Example');
    setSendToManualRequestBody(requestBody || '{}');
    setSendToManualResponseBody(response?.body || runnerData?.responseBody || '');
    setShowSendToManualDialog(true);
  };

  // 🎯 Send to Manual 함수
  const handleSendToManual = () => {
    if (!exampleTitle.trim()) {
      toast.error('Example title is required');
      return;
    }

    const requestInput = sendToManualRequestBody.trim();
    const responseInput = sendToManualResponseBody.trim();

    if (!requestInput && !responseInput) {
      toast.error('Please enter a request body or a response body to send.');
      return;
    }

    const requestExamples = [...(manualData?.requestExamples || [])];
    const responseExamples = [...(manualData?.responseExamples || [])];

    if (requestInput) {
      requestExamples.push({
        title: exampleTitle.trim(),
        code: formatJsonToHTML(requestInput),
      });
    }

    if (responseInput) {
      responseExamples.push({
        title: exampleTitle.trim(),
        code: formatJsonToHTML(responseInput),
      });
    }

    // Manual Data 업데이트 - Request/Response 분리
    const updatedManualData = {
      ...(manualData || {}),
      title: manualData?.title || `${endpoint.name} Manual`,
      category: manualData?.category || endpoint.method,
      inputUri: manualData?.inputUri || endpoint.path,
      activeMethods: manualData?.activeMethods || endpoint.method,
      jsonSchema: manualData?.jsonSchema || '',
      examples: manualData?.examples || [],  // deprecated
      requestExamples,
      responseExamples,
      specifications: manualData?.specifications || '',
    };

    setManualData(updatedManualData);
    resetSendToManualDialog();
    toast.success(`✅ Example "${exampleTitle}" added to Manual tab!`);
  };

  const urlInputValue = runnerData?.url ?? defaultUrl;

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
                  className={`group relative p-3 rounded-lg border cursor-pointer transition-all ${selectedTestCaseId === testCase.id
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
                    <div className={`w-2 h-2 rounded-full mt-1.5 flex-shrink-0 ${selectedTestCaseId === testCase.id ? 'bg-blue-500' : 'bg-zinc-600'
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

                  <div className="flex items-center justify-between text-xs text-zinc-500 mt-2">
                    <div className="flex items-center gap-2">
                      <Clock className="w-3 h-3" />
                      <span>{new Date(testCase.createdAt).toLocaleDateString()}</span>
                    </div>
                    {/* 🔥 Response 저장 상태 표시 */}
                    {testCase.responseBody && (
                      <span className={`px-1.5 py-0.5 rounded text-[10px] font-medium ${testCase.responseStatus && testCase.responseStatus >= 200 && testCase.responseStatus < 300
                          ? 'bg-green-900/50 text-green-400'
                          : 'bg-red-900/50 text-red-400'
                        }`}>
                        {testCase.responseStatus || 'OK'} {testCase.responseTime ? `· ${testCase.responseTime}ms` : ''}
                      </span>
                    )}
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
                {availableMethods.map((m) => (
                  <SelectItem key={m} value={m}>{m}</SelectItem>
                ))}
              </SelectContent>
            </Select>

            {/* URL Input */}
            <Input
              value={urlInputValue}
              onChange={(e) => updateRunnerData({ url: e.target.value })}
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
            <div className="px-4 py-2 bg-zinc-900 border-b border-zinc-800 flex items-center justify-between">
              <div className="flex items-center gap-3">
                <span className="text-sm text-zinc-400">📤 Request Body</span>
                {parsedRequestTable && (
                  <span className="text-[11px] text-zinc-500">
                    {parsedRequestTable.rows.length} rows
                  </span>
                )}
              </div>
              {parsedRequestTable && (
                <Button
                  variant="outline"
                  size="sm"
                  onClick={() => setShowRequestTableDialog(true)}
                  className="h-7 border-zinc-700 bg-zinc-800/70 text-zinc-200 hover:bg-zinc-700"
                >
                  <LayoutGrid className="w-3.5 h-3.5 mr-2" />
                  Table View
                </Button>
              )}
            </div>
            <div className="flex-1 h-full">
              <CodeEditor
                value={requestBody}
                onChange={(value) => {
                  const nextBody = value || '{}';
                  if (selectedTestCaseId) {
                    updateRunnerData({
                      requestBody: nextBody,
                      selectedTestCaseDraftBody: nextBody,
                    });
                    return;
                  }
                  updateRunnerData({ requestBody: nextBody });
                }}
                language="json"
                minimap={false}
              />
            </div>
          </div>

          {/* Right Side - Response */}
          <div className="flex-1 flex flex-col">
            <div className="px-4 py-2 bg-zinc-900 border-b border-zinc-800 flex items-center justify-between">
              <div className="flex items-center gap-3">
                <span className="text-sm text-zinc-400">📥 Response (Result)</span>
                {parsedResultTable && (
                  <span className="text-[11px] text-zinc-500">
                    {parsedResultTable.rows.length} rows · {parsedResultTable.columns.length} columns
                  </span>
                )}
              </div>
              <div className="flex items-center gap-2">
                {parsedResultTable && (
                  <Button
                    variant="outline"
                    size="sm"
                    onClick={() => setShowResultTableDialog(true)}
                    className="h-7 border-zinc-700 bg-zinc-800/70 text-zinc-200 hover:bg-zinc-700"
                  >
                    <LayoutGrid className="w-3.5 h-3.5 mr-2" />
                    Table View
                  </Button>
                )}
                {response && (
                  <span className="text-xs text-zinc-500">
                    HTTP/{response.status} {response.statusText} · Time: {response.time}ms
                  </span>
                )}
              </div>
            </div>
            <div className="flex-1 h-full">
              {response ? (
                <CodeEditor
                  value={response.body}
                  onChange={() => { }}
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

        {/* Footer with Save Test Case + Send to Manual Buttons */}
        <div className="border-t border-zinc-800 bg-zinc-900 p-4 flex items-center justify-between flex-shrink-0">
          {/* 왼쪽: 선택된 테스트 케이스 정보 */}
          <div className="text-xs text-zinc-500">
            {selectedTestCaseId && (
              <span>
                📋 Editing: <span className="text-zinc-300 font-medium">
                  {testCases.find(tc => tc.id === selectedTestCaseId)?.name || 'Unknown'}
                </span>
              </span>
            )}
          </div>

          {/* 오른쪽: 버튼들 */}
          <div className="flex items-center gap-2">
            <Button
              onClick={handleSaveTestCase}
              disabled={!selectedTestCaseId || !response}
              size="sm"
              className="h-8 text-xs bg-green-600 hover:bg-green-500"
            >
              <Save className="w-3 h-3 mr-2" />
              Save Test Case
            </Button>
            <Button
              onClick={openSendToManualDialog}
              size="sm"
              className="h-8 text-xs bg-blue-600 hover:bg-blue-500"
            >
              <Send className="w-3 h-3 mr-2" />
              Send to Manual
            </Button>
          </div>
        </div>
      </div>

      {/* 🎯 Send to Manual Dialog */}
      <Dialog
        open={showSendToManualDialog}
        onOpenChange={(open) => {
          if (open) {
            openSendToManualDialog();
            return;
          }
          resetSendToManualDialog();
        }}
      >
        <DialogContent className="bg-zinc-900 border-zinc-700 max-w-6xl">
          <DialogHeader>
            <DialogTitle className="text-white">Send to Manual Tab</DialogTitle>
            <DialogDescription className="text-zinc-400">
              Add request and response examples to the Manual tab. You can edit both before sending.
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
                This title will be used for the request/response examples in the Manual tab
              </p>
            </div>

            <div className="grid grid-cols-1 gap-4 xl:grid-cols-2">
              <div className="space-y-2">
                <div className="flex items-center justify-between">
                  <Label className="text-zinc-200">Request Example</Label>
                  <span className="text-xs text-zinc-500">{sendToManualRequestBody.length} chars</span>
                </div>
                <div className="h-72 overflow-hidden rounded-md border border-zinc-700">
                  <CodeEditor
                    value={sendToManualRequestBody}
                    onChange={(value) => setSendToManualRequestBody(value || '')}
                    language="json"
                    minimap={false}
                  />
                </div>
                <p className="text-xs text-zinc-400">
                  Leave empty if you do not want to add a request example.
                </p>
              </div>

              <div className="space-y-2">
                <div className="flex items-center justify-between">
                  <Label className="text-zinc-200">Response Example</Label>
                  <span className="text-xs text-zinc-500">{sendToManualResponseBody.length} chars</span>
                </div>
                <div className="h-72 overflow-hidden rounded-md border border-zinc-700">
                  <CodeEditor
                    value={sendToManualResponseBody}
                    onChange={(value) => setSendToManualResponseBody(value || '')}
                    language="json"
                    minimap={false}
                  />
                </div>
                <p className="text-xs text-zinc-400">
                  You can paste or edit a response manually even if the runner has not executed yet.
                </p>
              </div>
            </div>

            <div className="p-3 bg-zinc-800/50 rounded-lg border border-zinc-700">
              <p className="text-xs text-zinc-400 mb-2">Preview:</p>
              <div className="space-y-1 text-xs">
                {response && (
                  <>
                    <p className="text-zinc-300">
                      <span className="text-zinc-500">Last Status:</span> {response.status} {response.statusText}
                    </p>
                    <p className="text-zinc-300">
                      <span className="text-zinc-500">Last Time:</span> {response.time}ms
                    </p>
                  </>
                )}
                <p className="text-zinc-300">
                  <span className="text-zinc-500">Request Example:</span> {sendToManualRequestBody.trim() ? 'Will be added' : 'Skipped'}
                </p>
                <p className="text-zinc-300">
                  <span className="text-zinc-500">Response Example:</span> {sendToManualResponseBody.trim() ? 'Will be added' : 'Skipped'}
                </p>
              </div>
            </div>
          </div>

          <DialogFooter>
            <Button
              variant="outline"
              onClick={resetSendToManualDialog}
              className="border-zinc-700 text-zinc-200"
            >
              Cancel
            </Button>
            <Button
              onClick={handleSendToManual}
              disabled={!exampleTitle.trim() || (!sendToManualRequestBody.trim() && !sendToManualResponseBody.trim())}
              className="bg-blue-600 hover:bg-blue-500"
            >
              <Send className="w-3 h-3 mr-2" />
              Add to Manual
            </Button>
          </DialogFooter>
        </DialogContent>
      </Dialog>

      <RunnerResultTableDialog
        model={parsedRequestTable}
        open={showRequestTableDialog}
        onOpenChange={setShowRequestTableDialog}
      />

      <RunnerResultTableDialog
        model={parsedResultTable}
        open={showResultTableDialog}
        onOpenChange={setShowResultTableDialog}
      />
    </div>
  );
}
