/**
 * Product Schema Definition (PSD) View
 * 제품별 스키마 정의 세트 관리
 */

import { useState, useEffect } from 'react';
import { Button } from '@/components/ui/button';
import { Input } from '@/components/ui/input';
import { Search, Plus, Copy, Trash2, RefreshCw, Package, FileText, Edit } from 'lucide-react';
import {
  Card,
  CardContent,
  CardDescription,
  CardHeader,
  CardTitle,
} from '@/components/ui/card';
import {
  Dialog,
  DialogContent,
  DialogDescription,
  DialogFooter,
  DialogHeader,
  DialogTitle,
} from '@/components/ui/dialog';
import { Label } from '@/components/ui/label';
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from '@/components/ui/select';
import { Textarea } from '@/components/ui/textarea';
import { ScrollArea } from '@/components/ui/scroll-area';
import { Tabs, TabsContent, TabsList, TabsTrigger } from '@/components/ui/tabs';
import { apiClient } from '@/lib/api-client';
import type { ApiProduct } from '@/types';
import { getPSDForProduct, setCustomPSDMapping } from '@/config/psdMapping';
import yaml from 'js-yaml';

interface SchemaDefinitionSet {
  id: string;
  name: string;
  description: string;
  fileCount: number;
  files: string[];
  created: string;
  modified: string;
}

export function SchemaView() {
  const [schemaSets, setSchemaSets] = useState<SchemaDefinitionSet[]>([]);
  const [products, setProducts] = useState<ApiProduct[]>([]);
  const [loading, setLoading] = useState(true);
  const [searchTerm, setSearchTerm] = useState('');
  const [createDialogOpen, setCreateDialogOpen] = useState(false);
  const [newSetName, setNewSetName] = useState('');
  const [baseSetName, setBaseSetName] = useState('enhanced');
  
  // YAML 편집 관련
  const [editDialogOpen, setEditDialogOpen] = useState(false);
  const [currentSet, setCurrentSet] = useState<SchemaDefinitionSet | null>(null);
  const [currentFileName, setCurrentFileName] = useState<string>('');
  const [fileContent, setFileContent] = useState<string>('');
  const [originalContent, setOriginalContent] = useState<string>('');
  const [loadingFile, setLoadingFile] = useState(false);
  const [savingFile, setSavingFile] = useState(false);

  // 스키마 세트 목록 로드
  const fetchSchemaSets = async () => {
    try {
      setLoading(true);
      const response = await fetch('http://localhost:9527/api/schema-definitions');
      const data = await response.json();
      setSchemaSets(data);
    } catch (error) {
      console.error('Failed to fetch schema sets:', error);
    } finally {
      setLoading(false);
    }
  };

  // 제품 목록 로드
  const fetchProducts = async () => {
    try {
      const response = await fetch('http://localhost:9527/api/endpoints/tree');
      const data = await response.json();
      setProducts(data);
    } catch (error) {
      console.error('Failed to fetch products:', error);
    }
  };

  useEffect(() => {
    fetchSchemaSets();
    fetchProducts();
  }, []);

  // 스키마 세트 생성
  const handleCreateSet = async () => {
    if (!newSetName.trim()) {
      alert('이름을 입력해주세요.');
      return;
    }

    try {
      const response = await fetch('http://localhost:9527/api/schema-definitions', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
          name: newSetName.trim(),
          basedOn: baseSetName,
        }),
      });

      if (!response.ok) {
        const error = await response.json();
        throw new Error(error.error || 'Failed to create schema set');
      }

      alert('✅ 스키마 세트가 생성되었습니다!');
      setCreateDialogOpen(false);
      setNewSetName('');
      fetchSchemaSets();
    } catch (error) {
      alert(`❌ 생성 실패: ${error instanceof Error ? error.message : 'Unknown error'}`);
    }
  };

  // 스키마 세트 삭제
  const handleDeleteSet = async (setName: string) => {
    if (!confirm(`"${setName}" 스키마 세트를 삭제하시겠습니까?`)) {
      return;
    }

    try {
      const response = await fetch(`http://localhost:9527/api/schema-definitions/${setName}`, {
        method: 'DELETE',
      });

      if (!response.ok) {
        const error = await response.json();
        throw new Error(error.error || 'Failed to delete schema set');
      }

      alert('✅ 삭제되었습니다!');
      fetchSchemaSets();
    } catch (error) {
      alert(`❌ 삭제 실패: ${error instanceof Error ? error.message : 'Unknown error'}`);
    }
  };

  // 제품의 PSD 설정 변경 (로컬 저장만)
  const handleUpdateProductPSD = (productId: string, psdSet: string, schemaType: 'original' | 'enhanced') => {
    try {
      // localStorage에만 저장 (애플리케이션별 설정)
      setCustomPSDMapping(productId, psdSet, schemaType);

      // UI 업데이트를 위해 제품 목록 다시 가져오기
      fetchProducts();
      
      alert(`✅ ${psdSet}/${schemaType} 설정이 로컬에 저장되었습니다!`);
    } catch (error) {
      alert(`❌ 설정 저장 실패: ${error instanceof Error ? error.message : 'Unknown error'}`);
    }
  };

  // YAML 파일 열기
  const handleOpenFile = async (set: SchemaDefinitionSet, fileName: string) => {
    setCurrentSet(set);
    setCurrentFileName(fileName);
    setEditDialogOpen(true);
    setLoadingFile(true);

    try {
      // fileName이 "enhanced/builder.yaml" 형태이므로 split
      const [schemaType, ...fileNameParts] = fileName.split('/');
      const actualFileName = fileNameParts.join('/');
      
      const response = await fetch(
        `http://localhost:9527/api/schema-definitions/${set.id}/${schemaType}/${actualFileName}`
      );
      const data = await response.json();

      if (!response.ok) {
        throw new Error(data.error || 'Failed to load file');
      }

      setFileContent(data.content);
      setOriginalContent(data.content);
    } catch (error) {
      alert(`❌ 파일 로드 실패: ${error instanceof Error ? error.message : 'Unknown error'}`);
      setEditDialogOpen(false);
    } finally {
      setLoadingFile(false);
    }
  };

  // YAML 파일 저장
  const handleSaveFile = async () => {
    if (!currentSet || !currentFileName) return;

    setSavingFile(true);
    try {
      // currentFileName이 "enhanced/builder.yaml" 형태이므로 split
      const [schemaType, ...fileNameParts] = currentFileName.split('/');
      const actualFileName = fileNameParts.join('/');
      
      const response = await fetch(
        `http://localhost:9527/api/schema-definitions/${currentSet.id}/${schemaType}/${actualFileName}`,
        {
          method: 'PUT',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({ content: fileContent }),
        }
      );

      const data = await response.json();

      if (!response.ok) {
        throw new Error(data.error || data.details || 'Failed to save file');
      }

      alert('✅ 파일이 저장되었습니다!\n브라우저를 새로고침하면 변경사항이 적용됩니다.');
      setOriginalContent(fileContent);
      setEditDialogOpen(false);
      fetchSchemaSets();
    } catch (error) {
      alert(`❌ 저장 실패: ${error instanceof Error ? error.message : 'Unknown error'}`);
    } finally {
      setSavingFile(false);
    }
  };

  // 변경사항 확인
  const hasChanges = fileContent !== originalContent;

  // YAML 파싱 (프리뷰용)
  const parseYAML = () => {
    try {
      const parsed = yaml.load(fileContent);
      return { success: true, data: parsed };
    } catch (error) {
      return { 
        success: false, 
        error: error instanceof Error ? error.message : 'Parsing error' 
      };
    }
  };

  const filteredSets = schemaSets.filter(set =>
    set.name.toLowerCase().includes(searchTerm.toLowerCase()) ||
    set.description.toLowerCase().includes(searchTerm.toLowerCase())
  );

  return (
    <div className="flex-1 flex flex-col bg-zinc-950 text-zinc-100">
      {/* Header */}
      <div className="border-b border-zinc-800 bg-zinc-900 p-4">
        <div className="flex items-center justify-between mb-4">
          <div>
            <div className="flex items-center gap-2">
              <Package className="w-5 h-5 text-cyan-400" />
              <h1 className="text-xl font-semibold">Product Schema Definition (PSD)</h1>
            </div>
            <p className="text-xs text-zinc-400 mt-1 ml-7">
              📦 각 폴더 = 완전한 YAML 스키마 세트 (ui-rules, builder, table 등 포함)
            </p>
          </div>
          <div className="flex items-center gap-2">
            <Button
              variant="outline"
              size="sm"
              onClick={() => {
                fetchSchemaSets();
                fetchProducts();
              }}
            >
              <RefreshCw className="w-4 h-4 mr-2" />
              새로고침
            </Button>
            <Button
              size="sm"
              onClick={() => setCreateDialogOpen(true)}
            >
              <Plus className="w-4 h-4 mr-2" />
              스키마 세트 생성
            </Button>
          </div>
        </div>

        {/* Search */}
        <div className="relative">
          <Search className="absolute left-3 top-2.5 h-4 w-4 text-zinc-400" />
          <Input
            placeholder="스키마 세트 검색..."
            value={searchTerm}
            onChange={(e) => setSearchTerm(e.target.value)}
            className="pl-9 bg-zinc-800 border-zinc-700"
          />
        </div>
      </div>

      {/* Content */}
      <div className="flex-1 overflow-auto p-6">
        <div className="grid grid-cols-1 lg:grid-cols-2 gap-6">
          {/* 왼쪽: 스키마 세트 목록 */}
          <div>
            <h2 className="text-lg font-semibold mb-2">📦 사용 가능한 스키마 세트</h2>
            <p className="text-xs text-zinc-400 mb-4">
              각 세트는 완전한 YAML 파일 묶음입니다. 파일을 클릭하여 내용을 확인하세요.
            </p>
            
            {loading ? (
              <div className="text-zinc-400 text-center py-8">로딩 중...</div>
            ) : (
              <div className="space-y-3">
                {filteredSets.map((set) => (
                  <Card key={set.id} className="bg-zinc-900 border-zinc-800">
                    <CardHeader className="pb-3">
                      <div className="flex items-start justify-between">
                        <div className="flex-1">
                          <CardTitle className="text-base flex items-center gap-2">
                            <Package className="w-4 h-4 text-cyan-400" />
                            {set.name}
                            <span className="text-[10px] px-2 py-0.5 rounded bg-cyan-600/20 text-cyan-400 font-normal">
                              완전한 세트
                            </span>
                          </CardTitle>
                          <CardDescription className="text-xs mt-1">
                            {set.description}
                          </CardDescription>
                        </div>
                        {!['enhanced', 'original', 'manual'].includes(set.id) && (
                          <Button
                            variant="ghost"
                            size="sm"
                            className="h-8 w-8 p-0"
                            onClick={() => handleDeleteSet(set.id)}
                          >
                            <Trash2 className="w-4 h-4 text-red-400" />
                          </Button>
                        )}
                      </div>
                    </CardHeader>
                    <CardContent className="pb-3">
                      <div className="text-xs text-zinc-400 space-y-2">
                        <div>📄 파일: {set.fileCount}개</div>
                        <div className="space-y-1">
                          {(set.files || []).map((file, idx) => (
                            <button
                              key={idx}
                              onClick={() => handleOpenFile(set, file)}
                              className="flex items-center gap-2 text-[11px] text-cyan-400 hover:text-cyan-300 hover:underline transition-colors w-full text-left"
                            >
                              <FileText className="w-3 h-3" />
                              {file}
                            </button>
                          ))}
                        </div>
                      </div>
                    </CardContent>
                  </Card>
                ))}
              </div>
            )}
          </div>

          {/* 오른쪽: 제품별 스키마 세트 할당 */}
          <div>
            <h2 className="text-lg font-semibold mb-2">🔧 제품별 스키마 세트 할당</h2>
            <p className="text-xs text-zinc-400 mb-4">
              각 제품에 완전한 스키마 세트를 할당합니다. 선택한 세트의 모든 YAML 파일이 적용됩니다.
            </p>
            
            <div className="space-y-3">
              {products.map((product) => (
                <Card key={product.id} className="bg-zinc-900 border-zinc-800">
                  <CardHeader className="pb-3">
                    <CardTitle className="text-base">{product.name}</CardTitle>
                  </CardHeader>
                  <CardContent>
                    <div className="space-y-3">
                    <div className="space-y-1">
                      <Label className="text-xs text-zinc-400">
                          PSD 세트 (Level 1)
                      </Label>
                      <Select
                        value={getPSDForProduct(product.id).psdSet}
                          onValueChange={(value) => handleUpdateProductPSD(
                            product.id, 
                            value, 
                            getPSDForProduct(product.id).schemaType as 'original' | 'enhanced'
                          )}
                      >
                        <SelectTrigger className="h-8 text-xs bg-zinc-800 border-zinc-700">
                          <SelectValue />
                        </SelectTrigger>
                        <SelectContent>
                          {schemaSets.map((set) => (
                            <SelectItem key={set.id} value={set.id} className="text-xs">
                              <div className="flex items-center gap-2">
                                <Package className="w-3 h-3 text-cyan-400" />
                                <span>{set.name}</span>
                                <span className="text-zinc-500">({set.fileCount || 0} 파일)</span>
                              </div>
                            </SelectItem>
                          ))}
                        </SelectContent>
                      </Select>
                      </div>
                      
                      <div className="space-y-1">
                        <Label className="text-xs text-zinc-400">
                          스키마 타입 (Level 2)
                        </Label>
                        <Select
                          value={getPSDForProduct(product.id).schemaType}
                          onValueChange={(value) => handleUpdateProductPSD(
                            product.id, 
                            getPSDForProduct(product.id).psdSet, 
                            value as 'original' | 'enhanced'
                          )}
                        >
                          <SelectTrigger className="h-8 text-xs bg-zinc-800 border-zinc-700">
                            <SelectValue />
                          </SelectTrigger>
                          <SelectContent>
                            <SelectItem value="enhanced" className="text-xs">
                              <div className="flex items-center gap-2">
                                <FileText className="w-3 h-3 text-green-400" />
                                <span>enhanced</span>
                                <span className="text-zinc-500">(고급 기능)</span>
                              </div>
                            </SelectItem>
                            <SelectItem value="original" className="text-xs">
                              <div className="flex items-center gap-2">
                                <FileText className="w-3 h-3 text-blue-400" />
                                <span>original</span>
                                <span className="text-zinc-500">(원본)</span>
                              </div>
                            </SelectItem>
                          </SelectContent>
                        </Select>
                      </div>
                      
                      <div className="text-[10px] text-zinc-500 pt-1">
                        💾 로컬에만 저장 (애플리케이션별 설정)
                      </div>
                    </div>
                  </CardContent>
                </Card>
              ))}
            </div>
          </div>
        </div>
      </div>

      {/* Create Dialog */}
      <Dialog open={createDialogOpen} onOpenChange={setCreateDialogOpen}>
        <DialogContent>
          <DialogHeader>
            <DialogTitle className="flex items-center gap-2">
              <Package className="w-5 h-5 text-cyan-400" />
              새 스키마 세트 생성
            </DialogTitle>
            <DialogDescription>
              완전한 YAML 스키마 세트를 생성합니다. 각 세트는 독립된 폴더로 관리되며,
              ui-rules.yaml, builder.yaml, table.yaml 등의 파일을 포함합니다.
            </DialogDescription>
          </DialogHeader>

          <div className="space-y-4">
            <div>
              <Label htmlFor="setName">세트 이름 *</Label>
              <Input
                id="setName"
                value={newSetName}
                onChange={(e) => setNewSetName(e.target.value)}
                placeholder="예: civil-nx-custom"
                className="mt-1"
              />
              <p className="text-xs text-zinc-400 mt-1">
                영문자, 숫자, 하이픈(-), 언더스코어(_)만 사용 가능
              </p>
            </div>

            <div>
              <Label htmlFor="baseSet">기반 세트</Label>
              <Select value={baseSetName} onValueChange={setBaseSetName}>
                <SelectTrigger id="baseSet" className="mt-1">
                  <SelectValue />
                </SelectTrigger>
                <SelectContent>
                  <SelectItem value="">빈 세트</SelectItem>
                  {schemaSets.map((set) => (
                    <SelectItem key={set.id} value={set.id}>
                      {set.name}
                    </SelectItem>
                  ))}
                </SelectContent>
              </Select>
              <p className="text-xs text-zinc-400 mt-1">
                선택한 세트를 복제하여 새 세트를 만듭니다
              </p>
            </div>
          </div>

          <DialogFooter>
            <Button variant="outline" onClick={() => setCreateDialogOpen(false)}>
              취소
            </Button>
            <Button onClick={handleCreateSet}>
              <Copy className="w-4 h-4 mr-2" />
              생성
            </Button>
          </DialogFooter>
        </DialogContent>
      </Dialog>

      {/* YAML 편집 Dialog */}
      <Dialog open={editDialogOpen} onOpenChange={setEditDialogOpen}>
        <DialogContent className="max-w-4xl h-[80vh] flex flex-col">
          <DialogHeader>
            <DialogTitle className="flex items-center gap-2">
              <Edit className="w-5 h-5" />
              {currentSet?.name} / {currentFileName}
            </DialogTitle>
            <DialogDescription>
              YAML 파일을 편집합니다. 저장 후 브라우저를 새로고침하면 변경사항이 적용됩니다.
              {['enhanced', 'original', 'manual'].includes(currentSet?.id || '') && (
                <span className="block mt-1 text-red-400">
                  ⚠️ 기본 세트는 수정할 수 없습니다. 복제하여 새 세트를 만들어주세요.
                </span>
              )}
            </DialogDescription>
          </DialogHeader>

          <div className="flex-1 flex flex-col gap-2 min-h-0">
            {loadingFile ? (
              <div className="flex-1 flex items-center justify-center text-zinc-400">
                로딩 중...
              </div>
            ) : (
              <>
                <Tabs defaultValue="edit" className="flex-1 flex flex-col min-h-0">
                  <TabsList className="grid w-full grid-cols-2 max-w-md">
                    <TabsTrigger value="edit">편집</TabsTrigger>
                    <TabsTrigger value="preview">미리보기 (JSON)</TabsTrigger>
                  </TabsList>
                  
                  <TabsContent value="edit" className="flex-1 mt-2 min-h-0">
                    <ScrollArea className="h-full border rounded-md">
                      <Textarea
                        value={fileContent}
                        onChange={(e) => setFileContent(e.target.value)}
                        className="min-h-[500px] font-mono text-xs bg-zinc-950 border-0 resize-none"
                        placeholder="YAML 내용을 입력하세요..."
                        disabled={['enhanced', 'original', 'manual'].includes(currentSet?.id || '')}
                      />
                    </ScrollArea>
                  </TabsContent>
                  
                  <TabsContent value="preview" className="flex-1 mt-2 min-h-0">
                    <ScrollArea className="h-full border rounded-md bg-zinc-950 p-4">
                      <pre className="text-xs text-zinc-300 font-mono">
                        {(() => {
                          const result = parseYAML();
                          if (result.success) {
                            return JSON.stringify(result.data, null, 2);
                          } else {
                            return `❌ YAML 파싱 오류:\n\n${result.error}`;
                          }
                        })()}
                      </pre>
                    </ScrollArea>
                  </TabsContent>
                </Tabs>
                
                {hasChanges && (
                  <div className="text-xs text-amber-400 flex items-center gap-1">
                    ⚠️ 저장되지 않은 변경사항이 있습니다
                  </div>
                )}
              </>
            )}
          </div>

          <DialogFooter>
            <Button 
              variant="outline" 
              onClick={() => {
                if (hasChanges && !confirm('저장하지 않은 변경사항이 있습니다. 정말 닫으시겠습니까?')) {
                  return;
                }
                setEditDialogOpen(false);
              }}
            >
              닫기
            </Button>
            {!['enhanced', 'original', 'manual'].includes(currentSet?.id || '') && (
              <>
                <Button 
                  variant="outline" 
                  onClick={() => setFileContent(originalContent)}
                  disabled={!hasChanges}
                >
                  되돌리기
                </Button>
                <Button 
                  onClick={handleSaveFile}
                  disabled={!hasChanges || savingFile}
                >
                  {savingFile ? '저장 중...' : '저장'}
                </Button>
              </>
            )}
          </DialogFooter>
        </DialogContent>
      </Dialog>
    </div>
  );
}

