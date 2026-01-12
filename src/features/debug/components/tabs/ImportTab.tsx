import { useState } from 'react';
import { Button } from '@/components/ui/button';
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from '@/components/ui/card';
import { Textarea } from '@/components/ui/textarea';
import { Upload, FileJson, PlayCircle, Check, X, AlertCircle } from 'lucide-react';
import { importFromJSON, importFromFile, createSampleData, type ImportData } from '@/utils/dataImporter';

export function ImportTab() {
  const [jsonInput, setJsonInput] = useState('');
  const [loading, setLoading] = useState(false);
  const [result, setResult] = useState<any>(null);

  const handleJSONImport = async () => {
    if (!jsonInput.trim()) {
      alert('⚠️ JSON 데이터를 입력해주세요.');
      return;
    }

    setLoading(true);
    setResult(null);

    try {
      const result = await importFromJSON(jsonInput);
      setResult(result);
      
      if (result.success) {
        alert('✅ 데이터가 성공적으로 저장되었습니다!');
      } else {
        alert(`❌ 오류: ${result.error}`);
      }
    } catch (error) {
      setResult({
        success: false,
        error: error instanceof Error ? error.message : 'Unknown error',
      });
    } finally {
      setLoading(false);
    }
  };

  const handleFileImport = async (event: React.ChangeEvent<HTMLInputElement>) => {
    const file = event.target.files?.[0];
    if (!file) return;

    setLoading(true);
    setResult(null);

    try {
      const result = await importFromFile(file);
      setResult(result);
      
      if (result.success) {
        alert('✅ 파일이 성공적으로 임포트되었습니다!');
      } else {
        alert(`❌ 오류: ${result.error}`);
      }
    } catch (error) {
      setResult({
        success: false,
        error: error instanceof Error ? error.message : 'Unknown error',
      });
    } finally {
      setLoading(false);
      event.target.value = ''; // 리셋
    }
  };

  const loadSampleData = () => {
    const sample = createSampleData();
    setJsonInput(JSON.stringify(sample, null, 2));
  };

  return (
    <div className="h-full overflow-auto bg-zinc-950">
      <div className="p-6 max-w-5xl mx-auto">
        {/* Info Box */}
        <div className="mb-6 p-4 bg-blue-950/30 border border-blue-800/50 rounded-lg">
          <div className="flex items-start gap-3">
            <AlertCircle className="w-5 h-5 text-blue-400 flex-shrink-0 mt-0.5" />
            <div>
              <h3 className="text-sm font-medium text-blue-400 mb-1">📥 데이터 임포트 기능</h3>
              <p className="text-xs text-zinc-300">
                JSON 형식으로 엔드포인트, 스키마, HTML 메뉴얼, 예제 응답 등을 한번에 DB에 저장할 수 있습니다.
              </p>
            </div>
          </div>
        </div>

        {/* JSON Import */}
        <Card className="bg-zinc-900 border-zinc-800 mb-6">
          <CardHeader>
            <CardTitle className="text-white flex items-center gap-2">
              <FileJson className="w-5 h-5 text-green-400" />
              JSON 데이터 임포트
            </CardTitle>
            <CardDescription className="text-zinc-400">
              JSON 형식의 데이터를 붙여넣어 저장하세요
            </CardDescription>
          </CardHeader>
          <CardContent className="space-y-4">
            <Textarea
              value={jsonInput}
              onChange={(e) => setJsonInput(e.target.value)}
              placeholder={`{
  "endpoint": {
    "id": "db/nlct",
    "name": "NLCT",
    "method": "POST",
    "path": "/db/nlct",
    "product": "civil-nx",
    "group_name": "DB"
  },
  "version": {
    "version": "1",
    "author": "Your Name"
  },
  "schema": { ... },
  "manual": { ... },
  "examples": [ ... ]
}`}
              className="min-h-[300px] font-mono text-sm bg-zinc-950 border-zinc-700 text-zinc-300"
            />
            
            <div className="flex gap-2">
              <Button
                onClick={handleJSONImport}
                disabled={loading || !jsonInput.trim()}
                className="bg-green-600 hover:bg-green-700"
              >
                <PlayCircle className="w-4 h-4 mr-2" />
                {loading ? '저장 중...' : '임포트 실행'}
              </Button>
              
              <Button
                onClick={loadSampleData}
                variant="outline"
              >
                <FileJson className="w-4 h-4 mr-2" />
                샘플 데이터 로드
              </Button>
            </div>
          </CardContent>
        </Card>

        {/* File Upload */}
        <Card className="bg-zinc-900 border-zinc-800 mb-6">
          <CardHeader>
            <CardTitle className="text-white flex items-center gap-2">
              <Upload className="w-5 h-5 text-blue-400" />
              파일 업로드
            </CardTitle>
            <CardDescription className="text-zinc-400">
              JSON 파일을 선택하여 임포트하세요
            </CardDescription>
          </CardHeader>
          <CardContent>
            <label className="flex items-center justify-center w-full h-32 border-2 border-dashed border-zinc-700 rounded-lg cursor-pointer hover:border-blue-500 hover:bg-zinc-800/50 transition-colors">
              <div className="text-center">
                <Upload className="w-8 h-8 mx-auto mb-2 text-zinc-500" />
                <p className="text-sm text-zinc-400">클릭하거나 파일을 드롭하세요</p>
                <p className="text-xs text-zinc-600 mt-1">.json 파일만 지원</p>
              </div>
              <input
                type="file"
                accept=".json"
                onChange={handleFileImport}
                className="hidden"
                disabled={loading}
              />
            </label>
          </CardContent>
        </Card>

        {/* Result */}
        {result && (
          <Card className={`${
            result.success 
              ? 'bg-green-950/30 border-green-800/50' 
              : 'bg-red-950/30 border-red-800/50'
          }`}>
            <CardHeader>
              <CardTitle className={`flex items-center gap-2 ${
                result.success ? 'text-green-400' : 'text-red-400'
              }`}>
                {result.success ? (
                  <>
                    <Check className="w-5 h-5" />
                    임포트 성공
                  </>
                ) : (
                  <>
                    <X className="w-5 h-5" />
                    임포트 실패
                  </>
                )}
              </CardTitle>
            </CardHeader>
            <CardContent>
              {result.success ? (
                <div className="space-y-2">
                  {result.endpointId && (
                    <p className="text-sm text-zinc-300">
                      <span className="font-semibold">Endpoint ID:</span>{' '}
                      <code className="px-2 py-1 bg-zinc-800 rounded text-green-400">
                        {result.endpointId}
                      </code>
                    </p>
                  )}
                  {result.versionId && (
                    <p className="text-sm text-zinc-300">
                      <span className="font-semibold">Version ID:</span>{' '}
                      <code className="px-2 py-1 bg-zinc-800 rounded text-blue-400">
                        {result.versionId}
                      </code>
                    </p>
                  )}
                  {result.total && (
                    <div className="mt-4 pt-4 border-t border-green-800/30">
                      <p className="text-sm text-zinc-300">
                        총 <strong>{result.total}</strong>개 중{' '}
                        <strong className="text-green-400">{result.succeeded}</strong>개 성공,{' '}
                        <strong className="text-red-400">{result.failed}</strong>개 실패
                      </p>
                    </div>
                  )}
                </div>
              ) : (
                <p className="text-sm text-red-300">
                  오류: {result.error}
                </p>
              )}
            </CardContent>
          </Card>
        )}

        {/* Documentation */}
        <Card className="bg-zinc-900 border-zinc-800 mt-6">
          <CardHeader>
            <CardTitle className="text-white">📖 사용 가이드</CardTitle>
          </CardHeader>
          <CardContent className="text-sm text-zinc-300 space-y-3">
            <div>
              <h4 className="font-semibold text-zinc-200 mb-2">필수 필드:</h4>
              <ul className="list-disc list-inside space-y-1 ml-2">
                <li><code className="text-blue-400">endpoint.id</code> - 엔드포인트 고유 ID (예: "db/nlct")</li>
                <li><code className="text-blue-400">endpoint.name</code> - 표시 이름</li>
                <li><code className="text-blue-400">endpoint.method</code> - HTTP 메서드 (GET, POST 등)</li>
                <li><code className="text-blue-400">endpoint.path</code> - API 경로</li>
                <li><code className="text-blue-400">version.version</code> - 버전 번호</li>
              </ul>
            </div>
            
            <div>
              <h4 className="font-semibold text-zinc-200 mb-2">선택 필드:</h4>
              <ul className="list-disc list-inside space-y-1 ml-2">
                <li><code className="text-green-400">schema</code> - JSON 스키마 정의</li>
                <li><code className="text-green-400">manual.htmlContent</code> - HTML 메뉴얼 컨텐츠</li>
                <li><code className="text-green-400">examples</code> - 예제 요청/응답 배열</li>
                <li><code className="text-green-400">builderData</code> - 빌더 설정</li>
                <li><code className="text-green-400">runnerData</code> - 러너 설정</li>
              </ul>
            </div>

            <div className="pt-3 border-t border-zinc-800">
              <p className="text-xs text-zinc-500">
                💡 자세한 예제는{' '}
                <code className="px-1 bg-zinc-800 rounded">SAVE_ENDPOINT_DATA_EXAMPLE.md</code>{' '}
                파일을 참조하세요.
              </p>
            </div>
          </CardContent>
        </Card>
      </div>
    </div>
  );
}





