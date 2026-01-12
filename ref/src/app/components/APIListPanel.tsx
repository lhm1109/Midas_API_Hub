import { useState } from 'react';
import { Search, ChevronRight, ChevronDown, FileText, FolderClosed, FolderOpen } from 'lucide-react';
import { Input } from './ui/input';
import { ScrollArea } from './ui/scroll-area';

export interface ApiEndpoint {
  id: string;
  name: string;
  method: 'GET' | 'POST' | 'PUT' | 'DELETE' | 'PATCH';
  path: string;
  status?: 'success' | 'error' | null;
}

export interface ApiGroup {
  id: string;
  name: string;
  endpoints: ApiEndpoint[];
}

export interface ApiProduct {
  id: string;
  name: string;
  groups: ApiGroup[];
}

interface APIListPanelProps {
  products: ApiProduct[];
  selectedEndpoint: string | null;
  onEndpointSelect: (endpoint: ApiEndpoint) => void;
}

export function APIListPanel({ products, selectedEndpoint, onEndpointSelect }: APIListPanelProps) {
  const [searchTerm, setSearchTerm] = useState('');
  const [expandedProducts, setExpandedProducts] = useState<Set<string>>(new Set(['civil-nx']));
  const [expandedGroups, setExpandedGroups] = useState<Set<string>>(new Set(['db']));

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
          {products.map((product) => (
            <div key={product.id} className="mb-2">
              {/* Product */}
              <button
                onClick={() => toggleProduct(product.id)}
                className="w-full flex items-center gap-2 px-2 py-1.5 hover:bg-zinc-800 rounded text-sm text-zinc-300"
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
                <span>{product.name}</span>
              </button>

              {/* Groups */}
              {expandedProducts.has(product.id) && (
                <div className="ml-4">
                  {product.groups.map((group) => (
                    <div key={group.id}>
                      <button
                        onClick={() => toggleGroup(group.id)}
                        className="w-full flex items-center gap-2 px-2 py-1.5 hover:bg-zinc-800 rounded text-sm text-zinc-300"
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
                        <span>{group.name}</span>
                      </button>

                      {/* Endpoints */}
                      {expandedGroups.has(group.id) && (
                        <div className="ml-4">
                          {group.endpoints
                            .filter((endpoint) =>
                              endpoint.name.toLowerCase().includes(searchTerm.toLowerCase())
                            )
                            .map((endpoint) => (
                              <button
                                key={endpoint.id}
                                onClick={() => onEndpointSelect(endpoint)}
                                className={`w-full flex items-center gap-2 px-2 py-1.5 rounded text-sm ${
                                  selectedEndpoint === endpoint.id
                                    ? 'bg-blue-600 text-white'
                                    : 'text-zinc-300 hover:bg-zinc-800'
                                }`}
                              >
                                <FileText className="w-4 h-4" />
                                <span className="flex-1 text-left">{endpoint.name}</span>
                                {getStatusIndicator(endpoint.status)}
                              </button>
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
    </div>
  );
}
