import { useState } from 'react';
import { Search, ChevronRight, ChevronDown, FileText, FolderClosed, FolderOpen, Plus, Pencil, Trash2, MoreVertical } from 'lucide-react';
import { Input } from '@/components/ui/input';
import { ScrollArea } from '@/components/ui/scroll-area';
import {
  DropdownMenu,
  DropdownMenuContent,
  DropdownMenuItem,
  DropdownMenuTrigger,
} from '@/components/ui/dropdown-menu';
import { EndpointDialog } from './EndpointDialog';
import { ProductGroupDialog } from './ProductGroupDialog';
import { apiClient } from '@/lib/api-client';
import type { ApiEndpoint, ApiProduct } from '@/types';

interface APIListPanelProps {
  products: ApiProduct[];
  selectedEndpoint: string | null;
  onEndpointSelect: (endpoint: ApiEndpoint) => void;
  onEndpointsChange?: () => void;
}

export function APIListPanel({ products, selectedEndpoint, onEndpointSelect, onEndpointsChange }: APIListPanelProps) {
  const [searchTerm, setSearchTerm] = useState('');
  const [expandedProducts, setExpandedProducts] = useState<Set<string>>(new Set(['civil-nx']));
  const [expandedGroups, setExpandedGroups] = useState<Set<string>>(new Set(['db']));
  const [dialogOpen, setDialogOpen] = useState(false);
  const [editingEndpoint, setEditingEndpoint] = useState<ApiEndpoint | null>(null);
  const [dialogProductId, setDialogProductId] = useState<string>('');
  const [dialogGroupId, setDialogGroupId] = useState<string>('');
  const [productGroupDialogOpen, setProductGroupDialogOpen] = useState(false);
  const [productGroupDialogType, setProductGroupDialogType] = useState<'product' | 'group'>('product');
  const [productGroupDialogProductId, setProductGroupDialogProductId] = useState<string>('');

  const toggleProduct = (productId: string) => {
    const newExpanded = new Set(expandedProducts);
    if (newExpanded.has(productId)) {
      newExpanded.delete(productId);
    } else {
      newExpanded.add(productId);
    }
    setExpandedProducts(newExpanded);
  };

  const toggleGroup = (groupId: string) => {
    const newExpanded = new Set(expandedGroups);
    if (newExpanded.has(groupId)) {
      newExpanded.delete(groupId);
    } else {
      newExpanded.add(groupId);
    }
    setExpandedGroups(newExpanded);
  };

  const getStatusIndicator = (status?: 'success' | 'error' | null) => {
    if (!status) return null;
    return (
      <span
        className={`w-2 h-2 rounded-full ${
          status === 'success' ? 'bg-green-500' : 'bg-red-500'
        }`}
      />
    );
  };

  const handleAddEndpoint = (productId: string, groupId: string) => {
    setEditingEndpoint(null);
    setDialogProductId(productId);
    setDialogGroupId(groupId);
    setDialogOpen(true);
  };

  const handleEditEndpoint = (endpoint: ApiEndpoint, productId: string, groupId: string) => {
    setEditingEndpoint(endpoint);
    setDialogProductId(productId);
    setDialogGroupId(groupId);
    setDialogOpen(true);
  };

  const handleDeleteEndpoint = async (endpoint: ApiEndpoint) => {
    if (!confirm(`"${endpoint.name}" 엔드포인트를 삭제하시겠습니까?\n\n⚠️ 관련된 모든 버전과 데이터도 함께 삭제됩니다.`)) {
      return;
    }

    try {
      const result = await apiClient.deleteEndpoint(endpoint.id);
      if (result.error) {
        throw new Error(result.error);
      }
      alert('✅ 엔드포인트가 삭제되었습니다.');
      if (onEndpointsChange) {
        onEndpointsChange();
      }
    } catch (error) {
      console.error('Failed to delete endpoint:', error);
      alert(`❌ 삭제 실패: ${error instanceof Error ? error.message : 'Unknown error'}`);
    }
  };

  const handleDialogSuccess = () => {
    if (onEndpointsChange) {
      onEndpointsChange();
    }
  };

  const handleAddProduct = () => {
    setProductGroupDialogType('product');
    setProductGroupDialogProductId('');
    setProductGroupDialogOpen(true);
  };

  const handleAddGroup = (productId: string) => {
    setProductGroupDialogType('group');
    setProductGroupDialogProductId(productId);
    setProductGroupDialogOpen(true);
  };

  const handleDeleteProduct = async (product: ApiProduct) => {
    // 해당 product의 모든 엔드포인트 찾기
    const allEndpoints: ApiEndpoint[] = [];
    product.groups.forEach(group => {
      group.endpoints.forEach(endpoint => {
        allEndpoints.push(endpoint);
      });
    });

    if (allEndpoints.length === 0) {
      alert('삭제할 엔드포인트가 없습니다.');
      return;
    }

    if (!confirm(`"${product.name}" 제품과 관련된 모든 엔드포인트(${allEndpoints.length}개)를 삭제하시겠습니까?\n\n⚠️ 관련된 모든 버전과 데이터도 함께 삭제됩니다.`)) {
      return;
    }

    try {
      // 모든 엔드포인트 삭제
      for (const endpoint of allEndpoints) {
        const result = await apiClient.deleteEndpoint(endpoint.id);
        if (result.error) {
          throw new Error(result.error);
        }
      }
      alert('✅ 제품과 관련된 모든 엔드포인트가 삭제되었습니다.');
      if (onEndpointsChange) {
        onEndpointsChange();
      }
    } catch (error) {
      console.error('Failed to delete product:', error);
      alert(`❌ 삭제 실패: ${error instanceof Error ? error.message : 'Unknown error'}`);
    }
  };

  const handleDeleteGroup = async (_productId: string, groupName: string, endpoints: ApiEndpoint[]) => {
    if (endpoints.length === 0) {
      alert('삭제할 엔드포인트가 없습니다.');
      return;
    }

    if (!confirm(`"${groupName}" 그룹과 관련된 모든 엔드포인트(${endpoints.length}개)를 삭제하시겠습니까?\n\n⚠️ 관련된 모든 버전과 데이터도 함께 삭제됩니다.`)) {
      return;
    }

    try {
      // 모든 엔드포인트 삭제
      for (const endpoint of endpoints) {
        const result = await apiClient.deleteEndpoint(endpoint.id);
        if (result.error) {
          throw new Error(result.error);
        }
      }
      alert('✅ 그룹과 관련된 모든 엔드포인트가 삭제되었습니다.');
      if (onEndpointsChange) {
        onEndpointsChange();
      }
    } catch (error) {
      console.error('Failed to delete group:', error);
      alert(`❌ 삭제 실패: ${error instanceof Error ? error.message : 'Unknown error'}`);
    }
  };

  return (
    <div className="w-64 bg-zinc-900 border-r border-zinc-800 flex flex-col">
      {/* Search */}
      <div className="p-3 border-b border-zinc-800">
        <div className="relative">
          <Search className="absolute left-2 top-1/2 -translate-y-1/2 w-4 h-4 text-zinc-500" />
          <Input
            type="text"
            placeholder="Search API"
            value={searchTerm}
            onChange={(e) => setSearchTerm(e.target.value)}
            className="pl-8 bg-zinc-800 border-zinc-700 text-sm h-8"
          />
        </div>
      </div>

      {/* API Tree */}
      <ScrollArea className="flex-1">
        <div className="p-2">
          {/* Add Product Button */}
          <div className="mb-2">
            <div className="flex items-center gap-1 group">
              <button
                onClick={handleAddProduct}
                className="flex-1 flex items-center gap-2 px-2 py-1.5 hover:bg-zinc-800 rounded text-sm text-zinc-400 hover:text-zinc-200 transition-colors"
              >
                <Plus className="w-4 h-4" />
                <span>제품 추가</span>
              </button>
            </div>
          </div>

          {products.map((product) => (
            <div key={product.id} className="mb-2">
              {/* Product */}
              <div className="flex items-center gap-1 group">
              <button
                onClick={() => toggleProduct(product.id)}
                  className="flex-1 flex items-center gap-2 px-2 py-1.5 hover:bg-zinc-800 rounded text-sm text-zinc-300"
              >
                {expandedProducts.has(product.id) ? (
                  <ChevronDown className="w-4 h-4" />
                ) : (
                  <ChevronRight className="w-4 h-4" />
                )}
                {expandedProducts.has(product.id) ? (
                  <FolderOpen className="w-4 h-4 text-blue-400" />
                ) : (
                  <FolderClosed className="w-4 h-4 text-blue-400" />
                )}
                  <span className="flex-1 text-left">{product.name}</span>
              </button>
                
                {/* Product Actions Menu */}
                <DropdownMenu>
                  <DropdownMenuTrigger asChild>
                    <button
                      className="p-1 rounded hover:bg-zinc-700/50 opacity-0 group-hover:opacity-100 transition-opacity"
                      onClick={(e) => e.stopPropagation()}
                    >
                      <MoreVertical className="w-4 h-4" />
                    </button>
                  </DropdownMenuTrigger>
                  <DropdownMenuContent align="end" className="w-40">
                    <DropdownMenuItem
                      onClick={(e) => {
                        e.stopPropagation();
                        handleDeleteProduct(product);
                      }}
                      className="text-red-400 focus:text-red-300"
                    >
                      <Trash2 className="w-4 h-4 mr-2" />
                      제품 삭제
                    </DropdownMenuItem>
                  </DropdownMenuContent>
                </DropdownMenu>
              </div>

              {/* Groups */}
              {expandedProducts.has(product.id) && (
                <div className="ml-4 space-y-1">
                  {/* Add Group Button */}
                  <button
                    onClick={() => handleAddGroup(product.id)}
                    className="w-full flex items-center gap-2 px-2 py-1.5 rounded text-sm text-zinc-400 hover:bg-zinc-800 hover:text-zinc-200 transition-colors"
                  >
                    <Plus className="w-4 h-4" />
                    <span>그룹 추가</span>
                  </button>

                  {product.groups.map((group) => (
                    <div key={group.id}>
                      <div className="flex items-center gap-1 group">
                      <button
                        onClick={() => toggleGroup(group.id)}
                          className="flex-1 flex items-center gap-2 px-2 py-1.5 hover:bg-zinc-800 rounded text-sm text-zinc-300"
                      >
                        {expandedGroups.has(group.id) ? (
                          <ChevronDown className="w-4 h-4" />
                        ) : (
                          <ChevronRight className="w-4 h-4" />
                        )}
                        {expandedGroups.has(group.id) ? (
                          <FolderOpen className="w-4 h-4 text-amber-400" />
                        ) : (
                          <FolderClosed className="w-4 h-4 text-amber-400" />
                        )}
                          <span className="flex-1 text-left">{group.name}</span>
                      </button>
                        
                        {/* Group Actions Menu */}
                        <DropdownMenu>
                          <DropdownMenuTrigger asChild>
                            <button
                              className="p-1 rounded hover:bg-zinc-700/50 opacity-0 group-hover:opacity-100 transition-opacity"
                              onClick={(e) => e.stopPropagation()}
                            >
                              <MoreVertical className="w-4 h-4" />
                            </button>
                          </DropdownMenuTrigger>
                          <DropdownMenuContent align="end" className="w-40">
                            <DropdownMenuItem
                              onClick={(e) => {
                                e.stopPropagation();
                                handleDeleteGroup(product.id, group.name, group.endpoints);
                              }}
                              className="text-red-400 focus:text-red-300"
                            >
                              <Trash2 className="w-4 h-4 mr-2" />
                              그룹 삭제
                            </DropdownMenuItem>
                          </DropdownMenuContent>
                        </DropdownMenu>
                      </div>

                      {/* Endpoints */}
                      {expandedGroups.has(group.id) && (
                        <div className="ml-4 space-y-1">
                          {/* Add Endpoint Button */}
                          <button
                            onClick={() => handleAddEndpoint(product.id, group.name)}
                            className="w-full flex items-center gap-2 px-2 py-1.5 rounded text-sm text-zinc-400 hover:bg-zinc-800 hover:text-zinc-200 transition-colors"
                          >
                            <Plus className="w-4 h-4" />
                            <span className="flex-1 text-left">엔드포인트 추가</span>
                          </button>

                          {/* Endpoints List */}
                          {group.endpoints
                            .filter((endpoint) =>
                              endpoint.name.toLowerCase().includes(searchTerm.toLowerCase())
                            )
                            .map((endpoint) => (
                              <div
                                key={endpoint.id}
                                className={`group flex items-center gap-1 rounded text-sm ${
                                  selectedEndpoint === endpoint.id
                                    ? 'bg-blue-600 text-white'
                                    : 'text-zinc-300 hover:bg-zinc-800'
                                }`}
                              >
                                <button
                                  onClick={() => onEndpointSelect(endpoint)}
                                  className="flex-1 flex items-center gap-2 px-2 py-1.5 rounded"
                              >
                                <FileText className="w-4 h-4" />
                                <span className="flex-1 text-left">{endpoint.name}</span>
                                {getStatusIndicator(endpoint.status)}
                              </button>
                                
                                {/* Actions Menu */}
                                <DropdownMenu>
                                  <DropdownMenuTrigger asChild>
                                    <button
                                      className={`p-1 rounded hover:bg-zinc-700/50 opacity-0 group-hover:opacity-100 transition-opacity ${
                                        selectedEndpoint === endpoint.id ? 'opacity-100' : ''
                                      }`}
                                      onClick={(e) => e.stopPropagation()}
                                    >
                                      <MoreVertical className="w-4 h-4" />
                                    </button>
                                  </DropdownMenuTrigger>
                                  <DropdownMenuContent align="end" className="w-40">
                                    <DropdownMenuItem
                                      onClick={(e) => {
                                        e.stopPropagation();
                                        handleEditEndpoint(endpoint, product.id, group.name);
                                      }}
                                    >
                                      <Pencil className="w-4 h-4 mr-2" />
                                      수정
                                    </DropdownMenuItem>
                                    <DropdownMenuItem
                                      onClick={(e) => {
                                        e.stopPropagation();
                                        handleDeleteEndpoint(endpoint);
                                      }}
                                      className="text-red-400 focus:text-red-300"
                                    >
                                      <Trash2 className="w-4 h-4 mr-2" />
                                      삭제
                                    </DropdownMenuItem>
                                  </DropdownMenuContent>
                                </DropdownMenu>
                              </div>
                            ))}
                        </div>
                      )}
                    </div>
                  ))}
                </div>
              )}
            </div>
          ))}
        </div>
      </ScrollArea>

      {/* Endpoint Dialog */}
      <EndpointDialog
        open={dialogOpen}
        onOpenChange={setDialogOpen}
        endpoint={editingEndpoint}
        productId={dialogProductId}
        groupId={dialogGroupId}
        onSuccess={handleDialogSuccess}
      />

      {/* Product/Group Dialog */}
      <ProductGroupDialog
        open={productGroupDialogOpen}
        onOpenChange={setProductGroupDialogOpen}
        type={productGroupDialogType}
        productId={productGroupDialogProductId}
        onSuccess={handleDialogSuccess}
      />
    </div>
  );
}
