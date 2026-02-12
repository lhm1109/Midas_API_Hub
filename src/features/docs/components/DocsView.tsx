import { useState } from 'react';
import { BookOpen, Code, PlayCircle, Copy, Check } from 'lucide-react';
import { Tabs, TabsContent, TabsList, TabsTrigger } from '@/components/ui/tabs';
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from '@/components/ui/card';
import { Badge } from '@/components/ui/badge';
import { Button } from '@/components/ui/button';

/**
 * 애플리케이션 백엔드 API 문서
 */
export function DocsView() {
  const [copiedId, setCopiedId] = useState<string | null>(null);

  const copyToClipboard = (text: string, id: string) => {
    navigator.clipboard.writeText(text);
    setCopiedId(id);
    setTimeout(() => setCopiedId(null), 2000);
  };

  return (
    <div className="flex-1 flex flex-col bg-zinc-950 overflow-hidden">
      {/* Header */}
      <div className="border-b border-zinc-800 bg-zinc-900 px-6 py-4 flex-shrink-0">
        <div className="flex items-center gap-3">
          <BookOpen className="w-6 h-6 text-blue-400" />
          <div>
            <h2 className="text-xl font-bold">Backend API Reference</h2>
            <p className="text-sm text-zinc-500 mt-1">
              이 애플리케이션의 백엔드 API 사용 방법
            </p>
          </div>
        </div>
      </div>

      {/* Content */}
      <div className="flex-1 overflow-auto">
        <div className="p-6 max-w-6xl mx-auto">
          
          {/* Quick Start */}
          <Card className="bg-zinc-900 border-zinc-800 mb-6">
            <CardHeader>
              <CardTitle className="text-white">🚀 Quick Start</CardTitle>
              <CardDescription className="text-zinc-400">
                API 기본 정보
              </CardDescription>
            </CardHeader>
            <CardContent className="space-y-3">
              <div className="flex items-center gap-3">
                <span className="text-sm text-zinc-500 w-24">Base URL:</span>
                <code className="px-3 py-1.5 bg-zinc-950 rounded text-blue-400 font-mono text-sm">
                  http://localhost:9527/api
                </code>
              </div>
              <div className="flex items-center gap-3">
                <span className="text-sm text-zinc-500 w-24">Content-Type:</span>
                <code className="px-3 py-1.5 bg-zinc-950 rounded text-green-400 font-mono text-sm">
                  application/json
                </code>
              </div>
              <div className="flex items-center gap-3">
                <span className="text-sm text-zinc-500 w-24">Health Check:</span>
                <code className="px-3 py-1.5 bg-zinc-950 rounded text-zinc-300 font-mono text-sm">
                  GET /health
                </code>
              </div>
            </CardContent>
          </Card>

          <Tabs defaultValue="endpoints" className="w-full">
            <TabsList className="bg-zinc-800 mb-6">
              <TabsTrigger value="endpoints">📍 Endpoints API</TabsTrigger>
              <TabsTrigger value="versions">📦 Versions API</TabsTrigger>
              <TabsTrigger value="data">💾 Data API</TabsTrigger>
              <TabsTrigger value="debug">🐛 Debug API</TabsTrigger>
            </TabsList>

            {/* Endpoints API */}
            <TabsContent value="endpoints" className="space-y-6">
              <ApiEndpoint
                method="GET"
                path="/endpoints"
                title="모든 엔드포인트 조회"
                description="등록된 모든 API 엔드포인트 목록을 조회합니다"
                request={null}
                response={{
                  data: [
                    {
                      id: "db/nlct",
                      name: "NLCT",
                      method: "POST",
                      path: "/db/nlct",
                      product: "civil-nx",
                      group_name: "DB",
                      status: "active",
                      created_at: "2026-01-08T10:00:00Z",
                      updated_at: "2026-01-08T10:00:00Z"
                    }
                  ]
                }}
                copyToClipboard={copyToClipboard}
                copiedId={copiedId}
              />

              <ApiEndpoint
                method="GET"
                path="/endpoints/tree"
                title="계층 구조로 조회"
                description="엔드포인트를 제품 > 그룹 > 엔드포인트 계층으로 조회합니다"
                request={null}
                response={{
                  data: [
                    {
                      id: "civil-nx",
                      name: "civil-nx",
                      groups: [
                        {
                          id: "db",
                          name: "DB",
                          endpoints: [
                            {
                              id: "db/nlct",
                              name: "NLCT",
                              method: "POST",
                              path: "/db/nlct"
                            }
                          ]
                        }
                      ]
                    }
                  ]
                }}
                copyToClipboard={copyToClipboard}
                copiedId={copiedId}
              />

              <ApiEndpoint
                method="POST"
                path="/endpoints"
                title="새 엔드포인트 등록"
                description="새로운 API 엔드포인트를 등록합니다"
                request={{
                  id: "db/nlct",
                  name: "NLCT",
                  method: "POST",
                  path: "/db/nlct",
                  product: "civil-nx",
                  group_name: "DB",
                  description: "Non-Linear Constraint Table",
                  status: "active"
                }}
                response={{
                  id: "db/nlct",
                  message: "Endpoint created"
                }}
                copyToClipboard={copyToClipboard}
                copiedId={copiedId}
              />

              <ApiEndpoint
                method="PUT"
                path="/endpoints/:id"
                title="엔드포인트 수정"
                description="기존 엔드포인트 정보를 수정합니다"
                request={{
                  name: "NLCT (Updated)",
                  description: "Updated description",
                  status: "active"
                }}
                response={{
                  message: "Endpoint updated",
                  changes: 1
                }}
                copyToClipboard={copyToClipboard}
                copiedId={copiedId}
              />

              <ApiEndpoint
                method="DELETE"
                path="/endpoints/:id"
                title="엔드포인트 삭제"
                description="엔드포인트를 삭제합니다"
                request={null}
                response={{
                  message: "Endpoint deleted",
                  changes: 1
                }}
                copyToClipboard={copyToClipboard}
                copiedId={copiedId}
              />

              <ApiEndpoint
                method="POST"
                path="/endpoints/seed"
                title="샘플 데이터 생성"
                description="기본 엔드포인트 데이터를 생성합니다"
                request={null}
                response={{
                  message: "Endpoints seeded successfully",
                  count: 8
                }}
                copyToClipboard={copyToClipboard}
                copiedId={copiedId}
              />
            </TabsContent>

            {/* Versions API */}
            <TabsContent value="versions" className="space-y-6">
              <ApiEndpoint
                method="GET"
                path="/versions?endpoint_id=db/nlct"
                title="버전 목록 조회"
                description="특정 엔드포인트의 모든 버전을 조회합니다"
                request={null}
                response={{
                  data: [
                    {
                      id: "v_123",
                      version: "1",
                      endpointId: "db/nlct",
                      author: "John Doe",
                      createdAt: "2026-01-08T10:00:00Z",
                      manualData: {
                        title: "NLCT API",
                        jsonSchema: "{...}",
                        examples: []
                      }
                    }
                  ]
                }}
                copyToClipboard={copyToClipboard}
                copiedId={copiedId}
              />

              <ApiEndpoint
                method="GET"
                path="/versions/:id"
                title="특정 버전 조회"
                description="버전 ID로 상세 정보를 조회합니다"
                request={null}
                response={{
                  id: "v_123",
                  version: "1",
                  endpointId: "db/nlct",
                  manualData: { },
                  specData: { },
                  builderData: { },
                  runnerData: { }
                }}
                copyToClipboard={copyToClipboard}
                copiedId={copiedId}
              />

              <ApiEndpoint
                method="POST"
                path="/versions"
                title="새 버전 생성"
                description="엔드포인트의 새로운 버전을 생성합니다"
                request={{
                  id: "v_unique_id",
                  version: "1",
                  endpointId: "db/nlct",
                  author: "John Doe",
                  changeLog: "Initial version",
                  manualData: {
                    title: "NLCT API",
                    jsonSchema: "{...}",
                    htmlContent: "<div>...</div>",
                    examples: [
                      {
                        name: "Example 1",
                        request: {},
                        response: {}
                      }
                    ],
                    specifications: "API specifications..."
                  }
                }}
                response={{
                  id: "v_unique_id",
                  message: "Version created successfully"
                }}
                copyToClipboard={copyToClipboard}
                copiedId={copiedId}
              />

              <ApiEndpoint
                method="PUT"
                path="/versions/:id"
                title="버전 수정"
                description="기존 버전의 데이터를 수정합니다"
                request={{
                  version: "1.1",
                  changeLog: "Updated documentation",
                  manualData: {
                    title: "NLCT API (Updated)"
                  }
                }}
                response={{
                  message: "Version updated successfully"
                }}
                copyToClipboard={copyToClipboard}
                copiedId={copiedId}
              />

              <ApiEndpoint
                method="DELETE"
                path="/versions/:id"
                title="버전 삭제"
                description="버전과 관련 데이터를 삭제합니다"
                request={null}
                response={{
                  message: "Version deleted successfully"
                }}
                copyToClipboard={copyToClipboard}
                copiedId={copiedId}
              />
            </TabsContent>

            {/* Data API */}
            <TabsContent value="data" className="space-y-6">
              <div className="mb-6 p-4 bg-blue-950/30 border border-blue-800/50 rounded-lg">
                <p className="text-sm text-zinc-300">
                  <strong className="text-blue-400">실시간 작업 데이터</strong>: 버전을 저장하기 전의 임시 작업 데이터를 관리합니다.
                </p>
              </div>

              <ApiEndpoint
                method="GET"
                path="/data/manual"
                title="Manual 데이터 조회"
                description="현재 작업 중인 Manual 탭 데이터를 조회합니다"
                request={null}
                response={{
                  title: "NLCT API",
                  jsonSchema: "{...}"
                }}
                copyToClipboard={copyToClipboard}
                copiedId={copiedId}
              />

              <ApiEndpoint
                method="POST"
                path="/data/manual"
                title="Manual 데이터 저장"
                description="Manual 탭 데이터를 임시 저장합니다"
                request={{
                  title: "NLCT API",
                  jsonSchema: "{...}",
                  htmlContent: "<div>...</div>"
                }}
                response={{
                  message: "Manual data saved"
                }}
                copyToClipboard={copyToClipboard}
                copiedId={copiedId}
              />

              <ApiEndpoint
                method="GET"
                path="/data/all"
                title="모든 작업 데이터 조회"
                description="모든 탭의 임시 작업 데이터를 조회합니다"
                request={null}
                response={{
                  manualData: {},
                  specData: {},
                  builderData: {},
                  runnerData: {}
                }}
                copyToClipboard={copyToClipboard}
                copiedId={copiedId}
              />

              <ApiEndpoint
                method="DELETE"
                path="/data/all"
                title="모든 작업 데이터 초기화"
                description="모든 임시 작업 데이터를 삭제합니다"
                request={null}
                response={{
                  message: "All working data cleared"
                }}
                copyToClipboard={copyToClipboard}
                copiedId={copiedId}
              />
            </TabsContent>

            {/* Debug API */}
            <TabsContent value="debug" className="space-y-6">
              <ApiEndpoint
                method="GET"
                path="/debug/database"
                title="데이터베이스 전체 조회"
                description="모든 테이블의 데이터를 조회합니다"
                request={null}
                response={{
                  endpoints: [],
                  versions: [],
                  manualData: [],
                  specData: [],
                  builderData: [],
                  runnerData: [],
                  testCases: [],
                  summary: {
                    totalEndpoints: 8,
                    totalVersions: 2
                  }
                }}
                copyToClipboard={copyToClipboard}
                copiedId={copiedId}
              />

              <ApiEndpoint
                method="GET"
                path="/debug/database/stats"
                title="데이터베이스 통계"
                description="각 테이블의 레코드 수를 조회합니다"
                request={null}
                response={{
                  endpoints: 8,
                  versions: 2,
                  manualData: 2,
                  testCases: 0
                }}
                copyToClipboard={copyToClipboard}
                copiedId={copiedId}
              />

              <ApiEndpoint
                method="GET"
                path="/debug/database/table/:name"
                title="특정 테이블 조회"
                description="특정 테이블의 모든 데이터를 조회합니다"
                request={null}
                response={{
                  data: []
                }}
                copyToClipboard={copyToClipboard}
                copiedId={copiedId}
              />

              <ApiEndpoint
                method="DELETE"
                path="/debug/database/clear"
                title="데이터베이스 초기화"
                description="모든 데이터를 삭제합니다 (테이블 구조는 유지)"
                request={null}
                response={{
                  message: "Database cleared successfully"
                }}
                copyToClipboard={copyToClipboard}
                copiedId={copiedId}
              />
            </TabsContent>
          </Tabs>
        </div>
      </div>
    </div>
  );
}

// API 엔드포인트 컴포넌트
interface ApiEndpointProps {
  method: string;
  path: string;
  title: string;
  description: string;
  request: any;
  response: any;
  copyToClipboard: (text: string, id: string) => void;
  copiedId: string | null;
}

function ApiEndpoint({
  method,
  path,
  title,
  description,
  request,
  response,
  copyToClipboard,
  copiedId
}: ApiEndpointProps) {
  const methodColors = {
    GET: 'text-green-400 border-green-400 bg-green-500/10',
    POST: 'text-blue-400 border-blue-400 bg-blue-500/10',
    PUT: 'text-yellow-400 border-yellow-400 bg-yellow-500/10',
    DELETE: 'text-red-400 border-red-400 bg-red-500/10',
  };

  const curlCommand = `curl -X ${method} http://localhost:9527/api${path}${
    request ? `\n  -H "Content-Type: application/json"\n  -d '${JSON.stringify(request, null, 2)}'` : ''
  }`;

  const endpointId = `${method}-${path}`;

  return (
    <Card className="bg-zinc-900 border-zinc-800">
      <CardHeader>
        <div className="flex items-start justify-between">
          <div className="flex-1">
            <div className="flex items-center gap-3 mb-2">
              <Badge className={`${methodColors[method as keyof typeof methodColors]} font-mono font-bold`}>
                {method}
              </Badge>
              <code className="text-sm text-zinc-300 font-mono">{path}</code>
            </div>
            <CardTitle className="text-white text-lg">{title}</CardTitle>
            <CardDescription className="text-zinc-400 mt-1">{description}</CardDescription>
          </div>
          <Button
            variant="ghost"
            size="sm"
            onClick={() => copyToClipboard(curlCommand, endpointId)}
            className="text-zinc-500 hover:text-zinc-300"
          >
            {copiedId === endpointId ? (
              <Check className="w-4 h-4 text-green-400" />
            ) : (
              <Copy className="w-4 h-4" />
            )}
          </Button>
        </div>
      </CardHeader>
      <CardContent className="space-y-4">
        {/* Request */}
        {request && (
          <div>
            <p className="text-sm font-medium text-zinc-400 mb-2 flex items-center gap-2">
              <PlayCircle className="w-4 h-4" />
              Request Body
            </p>
            <pre className="p-4 bg-zinc-950 rounded-lg overflow-auto text-xs text-zinc-300 border border-zinc-800">
              <code>{JSON.stringify(request, null, 2)}</code>
            </pre>
          </div>
        )}

        {/* Response */}
        <div>
          <p className="text-sm font-medium text-zinc-400 mb-2 flex items-center gap-2">
            <Code className="w-4 h-4" />
            Response
          </p>
          <pre className="p-4 bg-zinc-950 rounded-lg overflow-auto text-xs text-zinc-300 border border-zinc-800">
            <code>{JSON.stringify(response, null, 2)}</code>
          </pre>
        </div>

        {/* cURL Command */}
        <div>
          <p className="text-sm font-medium text-zinc-400 mb-2">cURL Command</p>
          <pre className="p-4 bg-zinc-950 rounded-lg overflow-auto text-xs text-green-400 border border-zinc-800 font-mono">
            <code>{curlCommand}</code>
          </pre>
        </div>
      </CardContent>
    </Card>
  );
}
