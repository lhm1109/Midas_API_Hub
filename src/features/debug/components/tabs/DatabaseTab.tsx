import { useState, useEffect, useRef } from 'react';
import { Button } from '@/components/ui/button';
import { Tabs, TabsContent, TabsList, TabsTrigger } from '@/components/ui/tabs';
import { Database, RefreshCw, Trash2, AlertCircle, Cloud, Download, Upload } from 'lucide-react';
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from '@/components/ui/card';
import {
  Table,
  TableBody,
  TableCell,
  TableHead,
  TableHeader,
  TableRow,
} from '@/components/ui/table';
import { Badge } from '@/components/ui/badge';

interface DbData {
  endpoints: any[];
  versions: any[];
  manualData: any[];
  specData: any[];
  builderData: any[];
  runnerData: any[];
  testCases: any[];
}

interface SupabaseStats {
  products: any[];
  groups: any[];
  endpoints: any[];
  versions: any[];
  locks: any[];
  storageSize?: number;
}

export function DatabaseTab() {
  const [data, setData] = useState<DbData | null>(null);
  const [supabaseStats, setSupabaseStats] = useState<SupabaseStats | null>(null);
  const [loading, setLoading] = useState(false);
  const [supabaseLoading, setSupabaseLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const [supabaseError, setSupabaseError] = useState<string | null>(null);
  const fileInputRef = useRef<HTMLInputElement>(null);

  const fetchData = async () => {
    setLoading(true);
    setError(null);
    try {
      // TODO: API 엔드포인트 구현 필요
      const response = await fetch('http://localhost:9527/api/debug/database');
      if (!response.ok) throw new Error('Failed to fetch database data');
      const result = await response.json();
      setData(result);
    } catch (err) {
      setError(err instanceof Error ? err.message : 'Unknown error');
      console.error('Database fetch error:', err);
    } finally {
      setLoading(false);
    }
  };

  const fetchSupabaseStats = async () => {
    setSupabaseLoading(true);
    setSupabaseError(null);
    try {
      const response = await fetch('http://localhost:9527/api/debug/supabase');
      if (!response.ok) throw new Error('Failed to fetch Supabase data');
      const result = await response.json();
      setSupabaseStats(result);
    } catch (err) {
      setSupabaseError(err instanceof Error ? err.message : 'Unknown error');
      console.error('Supabase fetch error:', err);
    } finally {
      setSupabaseLoading(false);
    }
  };

  const clearDatabase = async () => {
    if (!confirm('⚠️ 모든 데이터베이스 내용을 삭제하시겠습니까? 이 작업은 되돌릴 수 없습니다.')) {
      return;
    }
    
    try {
      const response = await fetch('http://localhost:9527/api/debug/database/clear', {
        method: 'DELETE',
      });
      if (!response.ok) throw new Error('Failed to clear database');
      alert('✅ 데이터베이스가 초기화되었습니다.');
      fetchData(); // 다시 로드
    } catch (err) {
      alert(`❌ 오류: ${err instanceof Error ? err.message : 'Unknown error'}`);
    }
  };

  // Export 전체 DB
  const handleExportDatabase = async () => {
    try {
      const response = await fetch('http://localhost:9527/api/debug/database/export');
      if (!response.ok) throw new Error('Failed to export database');
      
      const blob = await response.blob();
      const url = window.URL.createObjectURL(blob);
      const a = document.createElement('a');
      a.href = url;
      a.download = `supabase-backup-${new Date().toISOString().split('T')[0]}.json`;
      document.body.appendChild(a);
      a.click();
      window.URL.revokeObjectURL(url);
      document.body.removeChild(a);
      
      alert('✅ 데이터베이스 백업이 다운로드되었습니다.');
    } catch (err) {
      alert(`❌ Export 오류: ${err instanceof Error ? err.message : 'Unknown error'}`);
    }
  };

  // Import 전체 DB (덮어쓰기)
  const handleImportDatabase = async (file: File) => {
    if (!confirm(
      '⚠️ 경고: 현재 Supabase 데이터베이스의 모든 내용이 삭제되고 백업 파일로 덮어씌워집니다.\n\n' +
      '이 작업은 되돌릴 수 없습니다.\n\n' +
      '계속하시겠습니까?'
    )) {
      return;
    }

    if (!confirm('⚠️ 최종 확인: 정말로 데이터베이스를 덮어쓰시겠습니까?')) {
      return;
    }

    try {
      const text = await file.text();
      const data = JSON.parse(text);

      const response = await fetch('http://localhost:9527/api/debug/database/import', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(data),
      });

      if (!response.ok) {
        const errorData = await response.json();
        throw new Error(errorData.error || 'Failed to import database');
      }

      alert('✅ 데이터베이스가 성공적으로 복원되었습니다.');
      
      // 데이터 새로고침
      await fetchData();
      await fetchSupabaseStats();
    } catch (err) {
      alert(`❌ Import 오류: ${err instanceof Error ? err.message : 'Unknown error'}`);
    }
  };

  const handleFileSelect = (event: React.ChangeEvent<HTMLInputElement>) => {
    const file = event.target.files?.[0];
    if (file) {
      handleImportDatabase(file);
    }
    // Reset input
    if (fileInputRef.current) {
      fileInputRef.current.value = '';
    }
  };

  const deleteEndpoint = async (endpointId: string, endpointName: string) => {
    if (!confirm(`"${endpointName}" 엔드포인트를 삭제하시겠습니까?\n\n⚠️ 이 작업은 되돌릴 수 없습니다.`)) {
      return;
    }

    try {
      const response = await fetch(`http://localhost:9527/api/endpoints/${encodeURIComponent(endpointId)}`, {
        method: 'DELETE',
      });

      if (!response.ok) {
        throw new Error('Failed to delete endpoint');
      }

      alert('✅ 엔드포인트가 삭제되었습니다.');
      
      // 데이터 새로고침
      await fetchSupabaseStats();
    } catch (err) {
      alert(`❌ 삭제 오류: ${err instanceof Error ? err.message : 'Unknown error'}`);
    }
  };

  useEffect(() => {
    fetchData();
    fetchSupabaseStats();
  }, []);

  if (loading && !data) {
    return (
      <div className="flex items-center justify-center h-full">
        <div className="text-center">
          <RefreshCw className="w-8 h-8 animate-spin mx-auto mb-2 text-blue-400" />
          <p className="text-zinc-400">Loading database...</p>
        </div>
      </div>
    );
  }

  if (error && !data) {
    return (
      <div className="flex items-center justify-center h-full">
        <div className="text-center">
          <AlertCircle className="w-8 h-8 mx-auto mb-2 text-red-400" />
          <p className="text-red-400 mb-2">Error: {error}</p>
          <Button onClick={fetchData} variant="outline">
            Retry
          </Button>
        </div>
      </div>
    );
  }

  return (
    <div className="h-full flex flex-col bg-zinc-950">
      {/* Hidden file input */}
      <input
        ref={fileInputRef}
        type="file"
        accept=".json"
        onChange={handleFileSelect}
        style={{ display: 'none' }}
      />
      
      {/* Header */}
      <div className="border-b border-zinc-800 bg-zinc-900 px-6 py-4 flex items-center justify-between flex-shrink-0">
        <div>
          <h2 className="text-xl font-bold flex items-center gap-2">
            <Database className="w-5 h-5 text-blue-400" />
            Database Inspector
          </h2>
          <p className="text-sm text-zinc-500 mt-1">
            백엔드 SQLite 데이터베이스 내용 확인
          </p>
        </div>
        <div className="flex gap-2">
          <Button
            onClick={handleExportDatabase}
            variant="outline"
            size="sm"
          >
            <Download className="w-4 h-4 mr-2" />
            Export DB
          </Button>
          <Button
            onClick={() => fileInputRef.current?.click()}
            variant="outline"
            size="sm"
            className="border-amber-600 text-amber-400 hover:bg-amber-950/50"
          >
            <Upload className="w-4 h-4 mr-2" />
            Import DB
          </Button>
          <Button
            onClick={fetchData}
            disabled={loading}
            variant="outline"
            size="sm"
          >
            <RefreshCw className={`w-4 h-4 mr-2 ${loading ? 'animate-spin' : ''}`} />
            새로고침
          </Button>
          <Button
            onClick={clearDatabase}
            variant="destructive"
            size="sm"
          >
            <Trash2 className="w-4 h-4 mr-2" />
            DB 초기화
          </Button>
        </div>
      </div>

      {/* Content - Scrollable Area */}
      <div className="flex-1 overflow-auto">
        <div className="p-6">
          {/* Info Box */}
          <div className="mb-6 p-4 bg-amber-950/30 border border-amber-800/50 rounded-lg">
            <div className="flex items-start gap-3">
              <AlertCircle className="w-5 h-5 text-amber-400 flex-shrink-0 mt-0.5" />
              <div>
                <h3 className="text-sm font-medium text-amber-400 mb-1">💡 두 테이블의 차이점</h3>
                <ul className="text-xs text-zinc-300 space-y-1">
                  <li>• <span className="font-semibold text-amber-400">Endpoints 테이블</span>: 프로젝트 탭에 표시되는 <span className="text-white">사용 가능한 모든 API 목록</span></li>
                  <li>• <span className="font-semibold text-blue-400">Versions 테이블</span>: 실제로 <span className="text-white">"Create New Version"을 클릭하여 작업한 엔드포인트</span>의 버전들</li>
                  <li className="mt-2 pt-2 border-t border-amber-800/30">
                    <span className="text-zinc-400">→ 현재는 <span className="text-blue-400 font-mono">nlct</span> 엔드포인트에 대해서만 2개 버전이 생성되었습니다</span>
                  </li>
                </ul>
              </div>
            </div>
          </div>

          {/* Version Summary by Endpoint */}
          {data?.versions && data.versions.length > 0 && (
            <div className="mb-6 p-4 bg-blue-950/30 border border-blue-800/50 rounded-lg">
              <h3 className="text-sm font-medium text-blue-400 mb-2">📦 작업한 엔드포인트</h3>
              <p className="text-xs text-zinc-500 mb-3">이 엔드포인트들은 실제로 버전을 생성한 것들입니다</p>
              <div className="flex flex-wrap gap-2">
                {Array.from(new Set(data.versions.map((v: any) => v.endpoint_id || v.endpointId))).map((endpointId: any) => {
                  const count = data.versions.filter((v: any) => (v.endpoint_id || v.endpointId) === endpointId).length;
                  return (
                    <div key={endpointId} className="flex items-center gap-2 px-3 py-2 bg-zinc-800 rounded-md border border-zinc-700">
                      <Badge className="bg-blue-500/20 text-blue-400 border-blue-500 font-mono">
                        {endpointId}
                      </Badge>
                      <span className="text-xs text-zinc-400">
                        {count}개 버전
                      </span>
                    </div>
                  );
                })}
              </div>
            </div>
          )}

          {/* Summary Cards */}
          <div className="grid grid-cols-4 gap-4 mb-6">
            <Card className="bg-zinc-900 border-zinc-800">
              <CardHeader className="pb-3">
                <CardTitle className="text-sm font-medium text-zinc-400">🔌 Endpoints</CardTitle>
              </CardHeader>
              <CardContent>
                <div className="text-2xl font-bold text-amber-400">
                  {data?.endpoints?.length || 0}
                </div>
                <p className="text-xs text-zinc-500 mt-1">사용 가능한 API</p>
              </CardContent>
            </Card>
            
            <Card className="bg-zinc-900 border-zinc-800">
              <CardHeader className="pb-3">
                <CardTitle className="text-sm font-medium text-zinc-400">📦 Versions</CardTitle>
              </CardHeader>
              <CardContent>
                <div className="text-2xl font-bold text-blue-400">
                  {data?.versions?.length || 0}
                </div>
                <p className="text-xs text-zinc-500 mt-1">작업한 버전</p>
              </CardContent>
            </Card>
            
            <Card className="bg-zinc-900 border-zinc-800">
              <CardHeader className="pb-3">
                <CardTitle className="text-sm font-medium text-zinc-400">📝 Working Data</CardTitle>
              </CardHeader>
              <CardContent>
                <div className="text-2xl font-bold text-green-400">
                  {(data?.manualData?.length || 0) + 
                   (data?.specData?.length || 0) + 
                   (data?.builderData?.length || 0) + 
                   (data?.runnerData?.length || 0)}
                </div>
                <p className="text-xs text-zinc-500 mt-1">작업 데이터</p>
              </CardContent>
            </Card>
            
            <Card className="bg-zinc-900 border-zinc-800">
              <CardHeader className="pb-3">
                <CardTitle className="text-sm font-medium text-zinc-400">🧪 Test Cases</CardTitle>
              </CardHeader>
              <CardContent>
                <div className="text-2xl font-bold text-purple-400">
                  {data?.testCases?.length || 0}
                </div>
                <p className="text-xs text-zinc-500 mt-1">테스트 케이스</p>
              </CardContent>
            </Card>
          </div>

          {/* Detailed Tables */}
          <Tabs defaultValue="supabase" className="w-full">
            <TabsList className="bg-zinc-800">
              <TabsTrigger value="supabase">☁️ Supabase</TabsTrigger>
              <TabsTrigger value="endpoints">📍 Endpoints</TabsTrigger>
              <TabsTrigger value="versions">📦 Versions</TabsTrigger>
              <TabsTrigger value="manual">📖 Manual</TabsTrigger>
              <TabsTrigger value="spec">📄 Spec</TabsTrigger>
              <TabsTrigger value="builder">🏗️ Builder</TabsTrigger>
              <TabsTrigger value="runner">🚀 Runner</TabsTrigger>
              <TabsTrigger value="testcases">🧪 Tests</TabsTrigger>
            </TabsList>

            {/* Supabase Tab */}
            <TabsContent value="supabase" className="mt-4">
              <div className="space-y-4">
                {/* Header */}
                <div className="flex items-center justify-between">
                  <div>
                    <h3 className="text-lg font-semibold text-white flex items-center gap-2">
                      <Cloud className="w-5 h-5 text-green-400" />
                      Supabase PostgreSQL Status
                    </h3>
                    <p className="text-sm text-zinc-400 mt-1">
                      실시간 데이터베이스 상태 및 통계
                    </p>
                  </div>
                  <Button
                    onClick={fetchSupabaseStats}
                    disabled={supabaseLoading}
                    variant="outline"
                    size="sm"
                  >
                    <RefreshCw className={`w-4 h-4 mr-2 ${supabaseLoading ? 'animate-spin' : ''}`} />
                    새로고침
                  </Button>
                </div>

                {supabaseLoading && !supabaseStats ? (
                  <div className="flex items-center justify-center py-12">
                    <div className="text-center">
                      <RefreshCw className="w-8 h-8 animate-spin mx-auto mb-2 text-green-400" />
                      <p className="text-zinc-400">Loading Supabase data...</p>
                    </div>
                  </div>
                ) : supabaseError && !supabaseStats ? (
                  <Card className="bg-zinc-900 border-red-800">
                    <CardContent className="pt-6">
                      <div className="text-center">
                        <AlertCircle className="w-8 h-8 mx-auto mb-2 text-red-400" />
                        <p className="text-red-400 mb-2">Error: {supabaseError}</p>
                        <Button onClick={fetchSupabaseStats} variant="outline" size="sm">
                          Retry
                        </Button>
                      </div>
                    </CardContent>
                  </Card>
                ) : supabaseStats ? (
                  <>
                    {/* Summary Cards */}
                    <div className="grid grid-cols-5 gap-4">
                      <Card className="bg-zinc-900 border-zinc-800">
                        <CardHeader className="pb-3">
                          <CardTitle className="text-sm font-medium text-zinc-400">📦 Products</CardTitle>
                        </CardHeader>
                        <CardContent>
                          <div className="text-2xl font-bold text-blue-400">
                            {supabaseStats.products?.length || 0}
                          </div>
                        </CardContent>
                      </Card>

                      <Card className="bg-zinc-900 border-zinc-800">
                        <CardHeader className="pb-3">
                          <CardTitle className="text-sm font-medium text-zinc-400">📁 Groups</CardTitle>
                        </CardHeader>
                        <CardContent>
                          <div className="text-2xl font-bold text-purple-400">
                            {supabaseStats.groups?.length || 0}
                          </div>
                        </CardContent>
                      </Card>

                      <Card className="bg-zinc-900 border-zinc-800">
                        <CardHeader className="pb-3">
                          <CardTitle className="text-sm font-medium text-zinc-400">🔌 Endpoints</CardTitle>
                        </CardHeader>
                        <CardContent>
                          <div className="text-2xl font-bold text-green-400">
                            {supabaseStats.endpoints?.length || 0}
                          </div>
                        </CardContent>
                      </Card>

                      <Card className="bg-zinc-900 border-zinc-800">
                        <CardHeader className="pb-3">
                          <CardTitle className="text-sm font-medium text-zinc-400">📦 Versions</CardTitle>
                        </CardHeader>
                        <CardContent>
                          <div className="text-2xl font-bold text-amber-400">
                            {supabaseStats.versions?.length || 0}
                          </div>
                        </CardContent>
                      </Card>

                      <Card className="bg-zinc-900 border-zinc-800">
                        <CardHeader className="pb-3">
                          <CardTitle className="text-sm font-medium text-zinc-400">🔒 Locks</CardTitle>
                        </CardHeader>
                        <CardContent>
                          <div className="text-2xl font-bold text-red-400">
                            {supabaseStats.locks?.length || 0}
                          </div>
                        </CardContent>
                      </Card>
                    </div>

                    {/* Products Table */}
                    <Card className="bg-zinc-900 border-zinc-800">
                      <CardHeader>
                        <CardTitle className="text-white">📦 Products</CardTitle>
                        <CardDescription className="text-zinc-400">
                          제품 목록 및 순서
                        </CardDescription>
                      </CardHeader>
                      <CardContent>
                        {supabaseStats.products && supabaseStats.products.length > 0 ? (
                          <Table>
                            <TableHeader>
                              <TableRow className="border-zinc-800">
                                <TableHead className="text-zinc-300">Order</TableHead>
                                <TableHead className="text-zinc-300">ID</TableHead>
                                <TableHead className="text-zinc-300">Name</TableHead>
                                <TableHead className="text-zinc-300">Description</TableHead>
                                <TableHead className="text-zinc-300">Created At</TableHead>
                              </TableRow>
                            </TableHeader>
                            <TableBody>
                              {supabaseStats.products.map((product: any) => (
                                <TableRow key={product.id} className="border-zinc-800 hover:bg-zinc-800/50">
                                  <TableCell>
                                    <Badge className="bg-blue-500/20 text-blue-400 border-blue-500">
                                      #{product.order_index}
                                    </Badge>
                                  </TableCell>
                                  <TableCell className="font-mono text-xs text-zinc-300">{product.id}</TableCell>
                                  <TableCell className="text-white font-medium">{product.name}</TableCell>
                                  <TableCell className="text-zinc-400 text-sm">{product.description || '-'}</TableCell>
                                  <TableCell className="text-xs text-zinc-500">
                                    {new Date(product.created_at).toLocaleString('ko-KR')}
                                  </TableCell>
                                </TableRow>
                              ))}
                            </TableBody>
                          </Table>
                        ) : (
                          <div className="text-center py-8 text-zinc-500">
                            제품이 없습니다.
                          </div>
                        )}
                      </CardContent>
                    </Card>

                    {/* Groups Table */}
                    <Card className="bg-zinc-900 border-zinc-800">
                      <CardHeader>
                        <CardTitle className="text-white">📁 Groups</CardTitle>
                        <CardDescription className="text-zinc-400">
                          그룹 목록 및 순서
                        </CardDescription>
                      </CardHeader>
                      <CardContent>
                        {supabaseStats.groups && supabaseStats.groups.length > 0 ? (
                          <Table>
                            <TableHeader>
                              <TableRow className="border-zinc-800">
                                <TableHead className="text-zinc-300">Order</TableHead>
                                <TableHead className="text-zinc-300">Product</TableHead>
                                <TableHead className="text-zinc-300">Name</TableHead>
                                <TableHead className="text-zinc-300">Description</TableHead>
                                <TableHead className="text-zinc-300">Created At</TableHead>
                              </TableRow>
                            </TableHeader>
                            <TableBody>
                              {supabaseStats.groups.map((group: any) => (
                                <TableRow key={group.id} className="border-zinc-800 hover:bg-zinc-800/50">
                                  <TableCell>
                                    <Badge className="bg-purple-500/20 text-purple-400 border-purple-500">
                                      #{group.order_index}
                                    </Badge>
                                  </TableCell>
                                  <TableCell>
                                    <Badge className="bg-blue-500/20 text-blue-400 border-blue-500">
                                      {group.product_id}
                                    </Badge>
                                  </TableCell>
                                  <TableCell className="text-white font-medium">{group.name}</TableCell>
                                  <TableCell className="text-zinc-400 text-sm">{group.description || '-'}</TableCell>
                                  <TableCell className="text-xs text-zinc-500">
                                    {new Date(group.created_at).toLocaleString('ko-KR')}
                                  </TableCell>
                                </TableRow>
                              ))}
                            </TableBody>
                          </Table>
                        ) : (
                          <div className="text-center py-8 text-zinc-500">
                            그룹이 없습니다.
                          </div>
                        )}
                      </CardContent>
                    </Card>

                    {/* Endpoints Table */}
                    <Card className="bg-zinc-900 border-zinc-800">
                      <CardHeader>
                        <CardTitle className="text-white">🔌 Endpoints (Top 20)</CardTitle>
                        <CardDescription className="text-zinc-400">
                          엔드포인트 목록 및 순서 (최근 20개)
                        </CardDescription>
                      </CardHeader>
                      <CardContent>
                        {supabaseStats.endpoints && supabaseStats.endpoints.length > 0 ? (
                          <Table>
                            <TableHeader>
                              <TableRow className="border-zinc-800">
                                <TableHead className="text-zinc-300">Order</TableHead>
                                <TableHead className="text-zinc-300">Name</TableHead>
                                <TableHead className="text-zinc-300">Method</TableHead>
                                <TableHead className="text-zinc-300">Product</TableHead>
                                <TableHead className="text-zinc-300">Group</TableHead>
                                <TableHead className="text-zinc-300">Path</TableHead>
                                <TableHead className="text-zinc-300 text-right">Actions</TableHead>
                              </TableRow>
                            </TableHeader>
                            <TableBody>
                              {supabaseStats.endpoints.slice(0, 20).map((endpoint: any) => (
                                <TableRow key={endpoint.id} className="border-zinc-800 hover:bg-zinc-800/50">
                                  <TableCell>
                                    <Badge className="bg-green-500/20 text-green-400 border-green-500">
                                      #{endpoint.order_index}
                                    </Badge>
                                  </TableCell>
                                  <TableCell className="text-white font-medium">{endpoint.name}</TableCell>
                                  <TableCell>
                                    <Badge 
                                      variant="outline" 
                                      className={`
                                        ${endpoint.method === 'GET' ? 'text-green-400 border-green-400' : ''}
                                        ${endpoint.method === 'POST' ? 'text-blue-400 border-blue-400' : ''}
                                        ${endpoint.method === 'PUT' ? 'text-yellow-400 border-yellow-400' : ''}
                                        ${endpoint.method === 'DELETE' ? 'text-red-400 border-red-400' : ''}
                                      `}
                                    >
                                      {endpoint.method}
                                    </Badge>
                                  </TableCell>
                                  <TableCell>
                                    <Badge className="bg-blue-500/20 text-blue-400 border-blue-500 font-mono text-xs">
                                      {endpoint.product_id || endpoint.product}
                                    </Badge>
                                  </TableCell>
                                  <TableCell>
                                    <Badge className="bg-purple-500/20 text-purple-400 border-purple-500 font-mono text-xs">
                                      {endpoint.group_name}
                                    </Badge>
                                  </TableCell>
                                  <TableCell className="font-mono text-xs text-zinc-400">{endpoint.path}</TableCell>
                                  <TableCell className="text-right">
                                    <Button
                                      onClick={() => deleteEndpoint(endpoint.id, endpoint.name)}
                                      variant="ghost"
                                      size="sm"
                                      className="h-8 w-8 p-0 text-red-400 hover:text-red-300 hover:bg-red-900/20"
                                    >
                                      <Trash2 className="w-4 h-4" />
                                    </Button>
                                  </TableCell>
                                </TableRow>
                              ))}
                            </TableBody>
                          </Table>
                        ) : (
                          <div className="text-center py-8 text-zinc-500">
                            엔드포인트가 없습니다.
                          </div>
                        )}
                      </CardContent>
                    </Card>

                    {/* Active Locks */}
                    {supabaseStats.locks && supabaseStats.locks.length > 0 && (
                      <Card className="bg-zinc-900 border-red-800">
                        <CardHeader>
                          <CardTitle className="text-white">🔒 Active Locks</CardTitle>
                          <CardDescription className="text-zinc-400">
                            현재 활성화된 편집 잠금
                          </CardDescription>
                        </CardHeader>
                        <CardContent>
                          <Table>
                            <TableHeader>
                              <TableRow className="border-zinc-800">
                                <TableHead className="text-zinc-300">Endpoint/Version</TableHead>
                                <TableHead className="text-zinc-300">Locked By</TableHead>
                                <TableHead className="text-zinc-300">Locked At</TableHead>
                                <TableHead className="text-zinc-300">Expires At</TableHead>
                              </TableRow>
                            </TableHeader>
                            <TableBody>
                              {supabaseStats.locks.map((lock: any, idx: number) => (
                                <TableRow key={idx} className="border-zinc-800 hover:bg-zinc-800/50">
                                  <TableCell className="font-mono text-xs text-amber-400">
                                    {lock.endpoint_id || lock.version_id}
                                  </TableCell>
                                  <TableCell className="text-white">{lock.locked_by}</TableCell>
                                  <TableCell className="text-xs text-zinc-500">
                                    {new Date(lock.locked_at).toLocaleString('ko-KR')}
                                  </TableCell>
                                  <TableCell className="text-xs text-zinc-500">
                                    {new Date(lock.expires_at).toLocaleString('ko-KR')}
                                  </TableCell>
                                </TableRow>
                              ))}
                            </TableBody>
                          </Table>
                        </CardContent>
                      </Card>
                    )}
                  </>
                ) : (
                  <Card className="bg-zinc-900 border-zinc-800">
                    <CardContent className="pt-6">
                      <div className="text-center py-8 text-zinc-500">
                        데이터가 없습니다. 새로고침 버튼을 눌러주세요.
                      </div>
                    </CardContent>
                  </Card>
                )}
              </div>
            </TabsContent>

            {/* Endpoints Table */}
            <TabsContent value="endpoints" className="mt-4">
              <Card className="bg-zinc-900 border-zinc-800">
                <CardHeader>
                  <CardTitle className="text-white">Endpoints Table</CardTitle>
                  <CardDescription className="text-zinc-400">
                    📌 프로젝트 탭에 표시되는 사용 가능한 모든 엔드포인트
                  </CardDescription>
                </CardHeader>
                <CardContent>
                  {data?.endpoints && data.endpoints.length > 0 ? (
                    <Table>
                      <TableHeader>
                        <TableRow className="border-zinc-800">
                          <TableHead className="text-zinc-300">ID</TableHead>
                          <TableHead className="text-zinc-300">Name</TableHead>
                          <TableHead className="text-zinc-300">Method</TableHead>
                          <TableHead className="text-zinc-300">Path</TableHead>
                          <TableHead className="text-zinc-300">Product</TableHead>
                          <TableHead className="text-zinc-300">Group</TableHead>
                          <TableHead className="text-zinc-300">Status</TableHead>
                        </TableRow>
                      </TableHeader>
                      <TableBody>
                        {data.endpoints.map((endpoint: any) => (
                          <TableRow key={endpoint.id} className="border-zinc-800 hover:bg-zinc-800/50">
                            <TableCell className="font-mono text-xs text-amber-400">{endpoint.id}</TableCell>
                            <TableCell className="text-zinc-300">{endpoint.name}</TableCell>
                            <TableCell>
                              <Badge 
                                variant="outline" 
                                className={`
                                  ${endpoint.method === 'GET' ? 'text-green-400 border-green-400' : ''}
                                  ${endpoint.method === 'POST' ? 'text-blue-400 border-blue-400' : ''}
                                  ${endpoint.method === 'PUT' ? 'text-yellow-400 border-yellow-400' : ''}
                                  ${endpoint.method === 'DELETE' ? 'text-red-400 border-red-400' : ''}
                                `}
                              >
                                {endpoint.method}
                              </Badge>
                            </TableCell>
                            <TableCell className="font-mono text-xs text-zinc-400">{endpoint.path}</TableCell>
                            <TableCell>
                              <Badge className="bg-blue-500/20 text-blue-400 border-blue-500">
                                {endpoint.product}
                              </Badge>
                            </TableCell>
                            <TableCell>
                              <Badge className="bg-purple-500/20 text-purple-400 border-purple-500">
                                {endpoint.group_name}
                              </Badge>
                            </TableCell>
                            <TableCell>
                              <Badge 
                                className={`
                                  ${endpoint.status === 'active' ? 'bg-green-500/20 text-green-400 border-green-500' : 'bg-gray-500/20 text-gray-400 border-gray-500'}
                                `}
                              >
                                {endpoint.status || 'active'}
                              </Badge>
                            </TableCell>
                          </TableRow>
                        ))}
                      </TableBody>
                    </Table>
                  ) : (
                    <div className="text-center py-8 text-zinc-500">
                      등록된 엔드포인트가 없습니다.
                    </div>
                  )}
                </CardContent>
              </Card>
            </TabsContent>

            {/* Versions Table */}
            <TabsContent value="versions" className="mt-4">
              <Card className="bg-zinc-900 border-zinc-800">
                <CardHeader>
                  <CardTitle className="text-white">Versions Table</CardTitle>
                  <CardDescription className="text-zinc-400">
                    📦 실제로 작업한 버전들만 저장됨 (Create New Version 클릭 시 생성)
                  </CardDescription>
                </CardHeader>
                <CardContent>
                  {data?.versions && data.versions.length > 0 ? (
                    <Table>
                      <TableHeader>
                        <TableRow className="border-zinc-800">
                          <TableHead className="text-zinc-300">ID</TableHead>
                          <TableHead className="text-zinc-300">Version</TableHead>
                          <TableHead className="text-zinc-300">Endpoint ID</TableHead>
                          <TableHead className="text-zinc-300">Author</TableHead>
                          <TableHead className="text-zinc-300">Created At</TableHead>
                          <TableHead className="text-zinc-300">Updated At</TableHead>
                        </TableRow>
                      </TableHeader>
                      <TableBody>
                        {data.versions.map((version: any) => (
                          <TableRow key={version.id} className="border-zinc-800 hover:bg-zinc-800/50">
                            <TableCell className="font-mono text-xs text-zinc-300">{version.id}</TableCell>
                            <TableCell>
                              <Badge variant="outline" className="text-blue-400 border-blue-400">{version.version}</Badge>
                            </TableCell>
                            <TableCell>
                              <Badge className="bg-green-500/20 text-green-400 border-green-500 font-mono">
                                {version.endpoint_id || version.endpointId || 'N/A'}
                              </Badge>
                            </TableCell>
                            <TableCell className="text-zinc-400">{version.author || '-'}</TableCell>
                            <TableCell className="text-xs text-zinc-500">
                              {new Date(version.created_at || version.createdAt).toLocaleString('ko-KR')}
                            </TableCell>
                            <TableCell className="text-xs text-zinc-500">
                              {new Date(version.updated_at || version.updatedAt).toLocaleString('ko-KR')}
                            </TableCell>
                          </TableRow>
                        ))}
                      </TableBody>
                    </Table>
                  ) : (
                    <div className="text-center py-8 text-zinc-500">
                      저장된 버전이 없습니다.
                    </div>
                  )}
                </CardContent>
              </Card>
            </TabsContent>

            {/* Manual Data Table */}
            <TabsContent value="manual" className="mt-4">
              <Card className="bg-zinc-900 border-zinc-800">
                <CardHeader>
                  <CardTitle className="text-white">Manual Data Table</CardTitle>
                  <CardDescription className="text-zinc-400">저장된 매뉴얼 데이터</CardDescription>
                </CardHeader>
                <CardContent>
                  {data?.manualData && data.manualData.length > 0 ? (
                    <Table>
                      <TableHeader>
                        <TableRow className="border-zinc-800">
                          <TableHead className="text-zinc-300">Version ID</TableHead>
                          <TableHead className="text-zinc-300">Title</TableHead>
                          <TableHead className="text-zinc-300">Category</TableHead>
                          <TableHead className="text-zinc-300">URI</TableHead>
                        </TableRow>
                      </TableHeader>
                      <TableBody>
                        {data.manualData.map((item: any, idx: number) => (
                          <TableRow key={item.version_id || idx} className="border-zinc-800 hover:bg-zinc-800/50">
                            <TableCell className="font-mono text-xs text-zinc-300">{item.version_id}</TableCell>
                            <TableCell className="text-zinc-200">{item.title || '(비어있음)'}</TableCell>
                            <TableCell>
                              <Badge className="bg-blue-500/20 text-blue-400 border-blue-500">
                                {item.category || 'N/A'}
                              </Badge>
                            </TableCell>
                            <TableCell className="font-mono text-xs text-zinc-400">{item.input_uri || item.inputUri || '-'}</TableCell>
                          </TableRow>
                        ))}
                      </TableBody>
                    </Table>
                  ) : (
                    <div className="text-center py-8 text-zinc-500">
                      저장된 매뉴얼 데이터가 없습니다.
                    </div>
                  )}
                </CardContent>
              </Card>
            </TabsContent>

            {/* Spec Data Table */}
            <TabsContent value="spec" className="mt-4">
              <Card className="bg-zinc-900 border-zinc-800">
                <CardHeader>
                  <CardTitle className="text-white">Spec Data Table</CardTitle>
                  <CardDescription className="text-zinc-400">저장된 스펙 데이터</CardDescription>
                </CardHeader>
                <CardContent>
                  {data?.specData && data.specData.length > 0 ? (
                    <pre className="text-xs text-green-300 bg-zinc-950 p-4 rounded overflow-auto border border-zinc-800">
                      {JSON.stringify(data.specData, null, 2)}
                    </pre>
                  ) : (
                    <div className="text-center py-8 text-zinc-500">
                      저장된 스펙 데이터가 없습니다.
                    </div>
                  )}
                </CardContent>
              </Card>
            </TabsContent>

            {/* Builder Data Table */}
            <TabsContent value="builder" className="mt-4">
              <Card className="bg-zinc-900 border-zinc-800">
                <CardHeader>
                  <CardTitle className="text-white">Builder Data Table</CardTitle>
                  <CardDescription className="text-zinc-400">저장된 빌더 데이터</CardDescription>
                </CardHeader>
                <CardContent>
                  {data?.builderData && data.builderData.length > 0 ? (
                    <pre className="text-xs text-blue-300 bg-zinc-950 p-4 rounded overflow-auto border border-zinc-800">
                      {JSON.stringify(data.builderData, null, 2)}
                    </pre>
                  ) : (
                    <div className="text-center py-8 text-zinc-500">
                      저장된 빌더 데이터가 없습니다.
                    </div>
                  )}
                </CardContent>
              </Card>
            </TabsContent>

            {/* Runner Data Table */}
            <TabsContent value="runner" className="mt-4">
              <Card className="bg-zinc-900 border-zinc-800">
                <CardHeader>
                  <CardTitle className="text-white">Runner Data Table</CardTitle>
                  <CardDescription className="text-zinc-400">저장된 러너 데이터</CardDescription>
                </CardHeader>
                <CardContent>
                  {data?.runnerData && data.runnerData.length > 0 ? (
                    <pre className="text-xs text-purple-300 bg-zinc-950 p-4 rounded overflow-auto border border-zinc-800">
                      {JSON.stringify(data.runnerData, null, 2)}
                    </pre>
                  ) : (
                    <div className="text-center py-8 text-zinc-500">
                      저장된 러너 데이터가 없습니다.
                    </div>
                  )}
                </CardContent>
              </Card>
            </TabsContent>

            {/* Test Cases Table */}
            <TabsContent value="testcases" className="mt-4">
              <Card className="bg-zinc-900 border-zinc-800">
                <CardHeader>
                  <CardTitle className="text-white">Test Cases Table</CardTitle>
                  <CardDescription className="text-zinc-400">저장된 테스트 케이스</CardDescription>
                </CardHeader>
                <CardContent>
                  {data?.testCases && data.testCases.length > 0 ? (
                    <Table>
                      <TableHeader>
                        <TableRow className="border-zinc-800">
                          <TableHead className="text-zinc-300">ID</TableHead>
                          <TableHead className="text-zinc-300">Name</TableHead>
                          <TableHead className="text-zinc-300">Description</TableHead>
                          <TableHead className="text-zinc-300">Created At</TableHead>
                        </TableRow>
                      </TableHeader>
                      <TableBody>
                        {data.testCases.map((testCase: any) => (
                          <TableRow key={testCase.id} className="border-zinc-800 hover:bg-zinc-800/50">
                            <TableCell className="font-mono text-xs text-zinc-300">{testCase.id}</TableCell>
                            <TableCell className="text-zinc-200">{testCase.name}</TableCell>
                            <TableCell className="text-zinc-400 text-sm">
                              {testCase.description || '-'}
                            </TableCell>
                            <TableCell className="text-xs text-zinc-500">
                              {new Date(testCase.created_at || testCase.createdAt).toLocaleString('ko-KR')}
                            </TableCell>
                          </TableRow>
                        ))}
                      </TableBody>
                    </Table>
                  ) : (
                    <div className="text-center py-8 text-zinc-500">
                      저장된 테스트 케이스가 없습니다.
                    </div>
                  )}
                </CardContent>
              </Card>
            </TabsContent>
          </Tabs>
        </div>
      </div>
    </div>
  );
}

