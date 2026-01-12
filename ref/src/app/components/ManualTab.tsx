import { useState, useRef } from 'react';
import { Button } from './ui/button';
import { Input } from './ui/input';
import { Label } from './ui/label';
import { ScrollArea } from './ui/scroll-area';
import { FileDown, FileUp, Send, Eye, Code, ZoomIn, ZoomOut, RotateCcw, Save } from 'lucide-react';
import { useAppStore } from '../store/useAppStore';
import type { ApiEndpoint } from './APIListPanel';

interface ManualTabProps {
  endpoint: ApiEndpoint;
}

export function ManualTab({ endpoint }: ManualTabProps) {
  const { manualData } = useAppStore();
  const [zendeskUrl, setZendeskUrl] = useState('');
  const [viewMode, setViewMode] = useState<'preview' | 'code'>('preview');
  const fileInputRef = useRef<HTMLInputElement>(null);
  
  // 🔍 Zoom 상태 관리
  const [zoom, setZoom] = useState(1);
  const previewContainerRef = useRef<HTMLDivElement>(null);
  
  // 🎯 Schema Toggle: Original vs Enhanced
  const [schemaToggle, setSchemaToggle] = useState<'original' | 'enhanced'>('enhanced');
  
  // 🎯 Editable HTML State
  const [editableHTML, setEditableHTML] = useState('');
  const [isHTMLModified, setIsHTMLModified] = useState(false);
  
  // 🎯 Zoom 리셋
  const handleResetZoom = () => {
    setZoom(1);
  };
  
  // 🎯 Zoom In/Out 버튼
  const handleZoomIn = () => {
    setZoom(prev => Math.min(prev + 0.1, 3));
  };
  
  const handleZoomOut = () => {
    setZoom(prev => Math.max(prev - 0.1, 0.3));
  };
  
  // 🎯 Ctrl + Wheel로 Zoom 제어
  const handleWheel = (e: React.WheelEvent) => {
    if (e.ctrlKey) {
      e.preventDefault();
      const delta = e.deltaY > 0 ? -0.05 : 0.05;
      setZoom(prev => Math.max(0.3, Math.min(3, prev + delta)));
    }
  };

  // 🎨 HTML 생성 함수
  const generateHTML = (): string => {
    if (!manualData) {
      return '<p>No manual data available. Please send data from Spec, Builder, or Runner tabs.</p>';
    }

    const now = new Date().toISOString();
    const { title, category, inputUri, activeMethods, jsonSchemaOriginal, jsonSchemaEnhanced, examples, specifications } = manualData;
    
    // 🎯 토글 상태에 따라 사용할 스키마 선택
    const currentSchema = schemaToggle === 'enhanced' && jsonSchemaEnhanced 
      ? jsonSchemaEnhanced 
      : (jsonSchemaOriginal || manualData.jsonSchema);

    return `<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">
    <title>${title} - ${category}</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body { 
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Helvetica, Arial, sans-serif;
            font-size: 14px;
            line-height: 1.6;
            color: #2f3941;
            background-color: #fff;
            padding: 40px 20px;
        }
        .container { max-width: 800px; margin: 0 auto; }
        
        h1, h2, h3, h4, h5, h6 { 
            font-weight: 600;
            margin-top: 24px;
            margin-bottom: 16px;
            line-height: 1.25;
        }
        h1 { font-size: 32px; margin-top: 0; }
        h2 { font-size: 24px; }
        h3 { font-size: 20px; }
        
        p { margin-bottom: 16px; }
        
        /* Table Styles - Zendesk Style */
        .table-wrap {
            margin: 20px 0;
            overflow-x: auto;
        }
        
        table {
            border-collapse: collapse;
            width: 100%;
            margin: 16px 0;
            border: 1px solid #d8dcde;
            font-size: 14px;
        }
        
        th {
            background-color: #f8f9f9;
            color: #2f3941;
            font-weight: 600;
            text-align: left;
            padding: 12px 15px;
            border: 1px solid #d8dcde;
        }
        
        td {
            padding: 12px 15px;
            border: 1px solid #d8dcde;
            vertical-align: top;
        }
        
        tr:hover {
            background-color: #f8f9f9;
        }
        
        /* Code Block Styles */
        .mgt32 {
            margin: 20px 0;
        }
        
        .btn_dropdown {
            background-color: #f8f9f9;
            padding: 12px 16px;
            border: 1px solid #d8dcde;
            border-bottom: none;
            font-weight: 600;
            color: #2f3941;
            cursor: pointer;
            border-radius: 4px 4px 0 0;
            margin: 0;
        }
        
        .code-block-wrapper {
            border: 1px solid #d8dcde;
            border-radius: 0 0 4px 4px;
            overflow: hidden;
        }
        
        .code-block-header {
            background-color: #f8f9f9;
            padding: 8px 16px;
            text-align: right;
            border-bottom: 1px solid #d8dcde;
        }
        
        .copy-btn {
            background-color: #1f73b7;
            border: none;
            color: white;
            padding: 6px 16px;
            text-align: center;
            display: inline-block;
            font-size: 13px;
            font-weight: 500;
            cursor: pointer;
            border-radius: 4px;
            transition: background-color 0.2s;
        }
        
        .copy-btn:hover {
            background-color: #165a8e;
        }
        
        .copy-btn:active {
            background-color: #0d4a73;
        }
        
        .code-content {
            background-color: #f8f9f9;
            padding: 16px;
            overflow-x: auto;
            font-family: 'Monaco', 'Menlo', 'Ubuntu Mono', 'Consolas', monospace;
            font-size: 13px;
            line-height: 1.6;
            color: #2f3941;
        }
        
        /* Colored table cells */
        td[style*="background-color: #e6fcff"] {
            background-color: #e3f2fd !important;
        }
        
        /* Center aligned text */
        p[style*="text-align: center"] {
            text-align: center;
        }
        
        /* Link styles */
        a {
            color: #1f73b7;
            text-decoration: none;
        }
        
        a:hover {
            text-decoration: underline;
        }
        
        /* Strong/Bold */
        strong {
            font-weight: 600;
        }
        
        /* Lists */
        ul, ol {
            margin: 16px 0;
            padding-left: 32px;
        }
        
        li {
            margin-bottom: 8px;
        }
    </style>
</head>
<body>
<div class="container">
    <h1>${title}</h1>
    
    <div class="content">
        <script>
    function copyText(elementId) {
        var element = document.getElementById(elementId);
        var text = element.innerText || element.textContent;
        text = text.replace(/&nbsp;/g, " ");
        text = text.replace(/ /g, " ");
        
        // Create temporary textarea
        var textarea = document.createElement('textarea');
        textarea.value = text;
        textarea.style.position = 'fixed';
        textarea.style.opacity = '0';
        document.body.appendChild(textarea);
        textarea.select();
        
        try {
            document.execCommand('copy');
            // Optional: Show feedback
            var btn = event.target;
            var originalText = btn.textContent;
            btn.textContent = 'Copied!';
            setTimeout(function() {
                btn.textContent = originalText;
            }, 1500);
        } catch (err) {
            console.error('Copy failed:', err);
        }
        
        document.body.removeChild(textarea);
    }
</script>

<h3 id=\"h_01J4JK6Y55WBG41A5PTKHSBPQX\"><strong>Input URI</strong></h3>
<div class=\"table-wrap\">
<table>
<colgroup> <col style=\"width: 100.00%;\"> </colgroup>
<tbody>
<tr>
<th style="text-align: center;">
<strong>${inputUri}</strong>
</th>
</tr>
</tbody>
</table>
</div>

<h3 id=\"h_01J4JK6Y55M865QPHE3BYY76TV\"><strong>Active Methods</strong></h3>
<div class=\"table-wrap\">
<table>
<colgroup> <col style=\"width: 100.00%;\"> </colgroup>
<tbody>
<tr>
<th style="text-align: center;">
<strong>${activeMethods}</strong>
</th>
</tr>
</tbody>
</table>
</div>

<h3 id=\"h_01J4JK6Y55M7H2APG5Z99R0EHF\"><strong>JSON Schema</strong></h3>
<div class=\"mgt32\">
<p class=\"btn_dropdown mgt4\">Details</p>
<div class=\"code-block-wrapper\">
<div class=\"code-block-header\">
<button class=\"copy-btn\" onclick=\"copyText('copyTarget1')\">Copy</button>
</div>
<div class=\"code-content\" id=\"copyTarget1\">${currentSchema}</div>
</div>
</div>

<h3 id=\"h_01J4JK6Y55V8YWGG0D6W4T2E8P\"><strong>Examples</strong></h3>
${examples.map((ex, idx) => `
<div class=\"mgt32\">
<p class=\"btn_dropdown mgt4\">${ex.title}</p>
<div class=\"code-block-wrapper\">
<div class=\"code-block-header\">
<button class=\"copy-btn\" onclick=\"copyText('copyTarget${idx + 2}')\">Copy</button>
</div>
<div class=\"code-content\" id=\"copyTarget${idx + 2}\">${ex.code}</div>
</div>
</div>
`).join('')}

<h3 id=\"h_01J4JK6Y56RSE6RM7MN02S8ANN\"><strong>Specifications</strong></h3>
${specifications}

</div>
</div>
</body>
</html>`;
  };

  // 📥 Import HTML
  const handleImport = () => {
    fileInputRef.current?.click();
  };

  const handleFileChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    const file = e.target.files?.[0];
    if (!file) return;

    const reader = new FileReader();
    reader.onload = (event) => {
      const htmlContent = event.target?.result as string;
      // TODO: Parse HTML and extract manualData
      alert('Import functionality: Parse HTML to extract manual data');
    };
    reader.readAsText(file);
  };

  // 📤 Export HTML
  const handleExport = () => {
    const html = isHTMLModified && editableHTML ? editableHTML : generateHTML();
    const blob = new Blob([html], { type: 'text/html' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = `${endpoint.name}_manual_${Date.now()}.html`;
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);
    URL.revokeObjectURL(url);
  };

  // 🚀 Send to Zendesk
  const handleSendToZendesk = () => {
    if (!zendeskUrl.trim()) {
      alert('Please enter a Zendesk URL');
      return;
    }

    const html = isHTMLModified && editableHTML ? editableHTML : generateHTML();
    
    // TODO: Implement Zendesk API integration
    console.log('Sending to Zendesk:', zendeskUrl);
    console.log('HTML Content:', html);
    
    alert(`✅ Manual would be sent to: ${zendeskUrl}\n(Zendesk API integration required)`);
  };
  
  // 🎯 Switch to HTML Code mode
  const handleSwitchToCode = () => {
    if (viewMode === 'preview') {
      setEditableHTML(generateHTML());
      setIsHTMLModified(false);
    }
    setViewMode('code');
  };
  
  // 🎯 Handle HTML Change
  const handleHTMLChange = (newHTML: string) => {
    setEditableHTML(newHTML);
    setIsHTMLModified(true);
  };
  
  // 🎯 Save HTML Changes
  const handleSaveHTML = () => {
    setIsHTMLModified(false);
    alert('✅ HTML changes saved!\n\nYou can now export or send to Zendesk with the updated HTML.');
  };
  
  // 🎯 Reset HTML
  const handleResetHTML = () => {
    setEditableHTML(generateHTML());
    setIsHTMLModified(false);
  };

  const htmlContent = isHTMLModified && editableHTML ? editableHTML : generateHTML();

  return (
    <div className="flex h-full w-full flex-col bg-zinc-950">
      {/* Header */}
      <div className="p-4 border-b border-zinc-800 bg-zinc-900 flex-shrink-0">
        <div className="flex items-center justify-between">
          <div>
            <h3 className="text-sm mb-1">📖 Manual Generator</h3>
            <p className="text-xs text-zinc-500">Auto-generated HTML documentation with Zendesk export</p>
          </div>
          
          {/* View Mode Toggle */}
          <div className="flex items-center gap-2">
            <Button
              size="sm"
              variant={viewMode === 'preview' ? 'default' : 'outline'}
              onClick={() => setViewMode('preview')}
              className="h-8"
            >
              <Eye className="w-3 h-3 mr-1" />
              Preview
            </Button>
            <Button
              size="sm"
              variant={viewMode === 'code' ? 'default' : 'outline'}
              onClick={handleSwitchToCode}
              className="h-8"
            >
              <Code className="w-3 h-3 mr-1" />
              HTML Code
            </Button>
          </div>
        </div>
      </div>

      {/* Action Bar */}
      <div className="p-4 border-b border-zinc-800 bg-zinc-900/50 flex-shrink-0">
        <div className="flex items-center gap-3">
          {/* Import/Export */}
          <Button size="sm" variant="outline" onClick={handleImport} className="h-9">
            <FileUp className="w-4 h-4 mr-2" />
            Import HTML
          </Button>
          <input
            ref={fileInputRef}
            type="file"
            accept=".html"
            onChange={handleFileChange}
            className="hidden"
          />
          
          <Button size="sm" variant="outline" onClick={handleExport} className="h-9">
            <FileDown className="w-4 h-4 mr-2" />
            Export HTML
          </Button>

          <div className="h-6 w-px bg-zinc-700" />

          {/* Zendesk */}
          <div className="flex items-center gap-2 flex-1">
            <Label className="text-xs text-zinc-400 whitespace-nowrap">Zendesk URL:</Label>
            <Input
              value={zendeskUrl}
              onChange={(e) => setZendeskUrl(e.target.value)}
              placeholder="https://your-zendesk.com/api/v2/..."
              className="bg-zinc-800 border-zinc-700 h-9 text-sm flex-1"
            />
            <Button size="sm" onClick={handleSendToZendesk} className="h-9 bg-blue-600 hover:bg-blue-500">
              <Send className="w-4 h-4 mr-2" />
              Send to Zendesk
            </Button>
          </div>
        </div>
      </div>

      {/* Content Area */}
      <ScrollArea className="flex-1 h-0">
        {viewMode === 'preview' ? (
          <div className="relative">
            {/* Zoom 컨트롤 바 */}
            {manualData && (
              <div className="sticky top-4 left-0 right-0 z-10 mx-6 mt-4 mb-2 flex items-center justify-center gap-3">
                {/* Zoom Controls */}
                <div className="bg-zinc-900/95 backdrop-blur-sm border border-zinc-700 rounded-lg px-3 py-2 flex items-center gap-2 shadow-lg">
                  <Button
                    size="sm"
                    variant="ghost"
                    onClick={handleZoomOut}
                    className="h-7 w-7 p-0 hover:bg-zinc-700"
                    title="Zoom Out (Ctrl + Wheel Down)"
                  >
                    <ZoomOut className="w-4 h-4" />
                  </Button>
                  
                  <span className="text-xs font-mono text-zinc-300 min-w-[60px] text-center">
                    {Math.round(zoom * 100)}%
                  </span>
                  
                  <Button
                    size="sm"
                    variant="ghost"
                    onClick={handleZoomIn}
                    className="h-7 w-7 p-0 hover:bg-zinc-700"
                    title="Zoom In (Ctrl + Wheel Up)"
                  >
                    <ZoomIn className="w-4 h-4" />
                  </Button>
                  
                  <div className="h-4 w-px bg-zinc-700 mx-1" />
                  
                  <Button
                    size="sm"
                    variant="ghost"
                    onClick={handleResetZoom}
                    className="h-7 w-7 p-0 hover:bg-zinc-700"
                    title="Reset Zoom (100%)"
                  >
                    <RotateCcw className="w-4 h-4" />
                  </Button>
                  
                  <div className="text-[10px] text-zinc-500 ml-2 hidden md:block">
                    💡 Ctrl + Wheel to zoom
                  </div>
                </div>
                
                {/* Schema Toggle - Only show if both schemas exist */}
                {manualData.jsonSchemaOriginal && manualData.jsonSchemaEnhanced && (
                  <div className="bg-zinc-900/95 backdrop-blur-sm border border-zinc-700 rounded-lg p-1 flex items-center gap-1 shadow-lg">
                    <button
                      onClick={() => setSchemaToggle('original')}
                      className={`px-3 py-1 text-xs rounded transition-colors ${
                        schemaToggle === 'original'
                          ? 'bg-blue-600 text-white'
                          : 'text-zinc-400 hover:text-zinc-200'
                      }`}
                    >
                      Original
                    </button>
                    <button
                      onClick={() => setSchemaToggle('enhanced')}
                      className={`px-3 py-1 text-xs rounded transition-colors ${
                        schemaToggle === 'enhanced'
                          ? 'bg-green-600 text-white'
                          : 'text-zinc-400 hover:text-zinc-200'
                      }`}
                    >
                      Enhanced
                    </button>
                  </div>
                )}
              </div>
            )}
            
            <div 
              className="p-6 overflow-auto"
              onWheel={handleWheel}
              style={{ cursor: 'default' }}
            >
              {!manualData ? (
                <div className="flex flex-col items-center justify-center h-full min-h-[400px] text-center">
                  <div className="w-16 h-16 bg-zinc-800 rounded-full flex items-center justify-center mb-4">
                    <FileDown className="w-8 h-8 text-zinc-600" />
                  </div>
                  <h3 className="text-lg font-semibold text-zinc-300 mb-2">No Manual Data</h3>
                  <p className="text-sm text-zinc-500 max-w-md">
                    Click "Send to Manual" from Spec, Builder, or Runner tabs to automatically generate documentation.
                  </p>
                </div>
              ) : (
                <div 
                  style={{ 
                    transform: `scale(${zoom})`, 
                    transformOrigin: 'top left',
                    transition: 'transform 0.1s ease-out',
                    width: `${100 / zoom}%`
                  }}
                >
                  <iframe
                    srcDoc={htmlContent}
                    className="w-full min-h-[800px] bg-white rounded-lg border border-zinc-700 pointer-events-auto"
                    title="Manual Preview"
                    sandbox="allow-scripts"
                    style={{ 
                      height: `${800 / zoom}px`,
                    }}
                  />
                </div>
              )}
            </div>
          </div>
        ) : (
          <div className="h-full flex flex-col bg-zinc-950 overflow-hidden">
            <div className="p-4 border-b border-zinc-800 bg-zinc-900 flex-shrink-0">
              <h3 className="text-sm">Editable HTML Code</h3>
              <p className="text-xs text-zinc-500 mt-1">
                {isHTMLModified ? '✏️ Modified - Click Save to apply changes' : '✅ Ready to edit'}
              </p>
            </div>
            
            <ScrollArea className="flex-1 h-0">
              <div className="relative h-full">
                {/* 🎯 Line numbers */}
                <div className="absolute left-0 top-0 bottom-0 w-12 bg-zinc-900 border-r border-zinc-800 text-right text-xs text-zinc-600 select-none overflow-hidden">
                  {(editableHTML || htmlContent).split('\n').map((_, idx) => (
                    <div key={idx} className="px-2 leading-6">
                      {idx + 1}
                    </div>
                  ))}
                </div>
                
                {/* 🎯 Editable Code Area */}
                <textarea
                  className="w-full h-full bg-zinc-950 text-zinc-100 font-mono text-sm leading-6 pl-14 pr-4 py-4 resize-none border-none outline-none focus:ring-2 focus:ring-blue-500/30 rounded-none"
                  style={{
                    minHeight: '800px',
                    fontFamily: 'ui-monospace, SFMono-Regular, "SF Mono", Menlo, Consolas, "Liberation Mono", monospace',
                    tabSize: 2,
                  }}
                  value={editableHTML || htmlContent}
                  onChange={(e) => handleHTMLChange(e.target.value)}
                  spellCheck={false}
                  placeholder="HTML code will appear here..."
                />
                
                {/* 🎯 Modified Indicator */}
                {isHTMLModified && (
                  <div className="absolute top-2 right-2 px-2 py-1 bg-orange-600/20 border border-orange-600/50 rounded text-xs text-orange-400">
                    Modified
                  </div>
                )}
              </div>
            </ScrollArea>
            
            {/* 🎯 Footer with Save Button - Spec Tab Style */}
            <div className="border-t border-zinc-800 bg-zinc-900 p-4 flex items-center justify-between flex-shrink-0">
              <div className="flex items-center gap-2 text-xs text-zinc-500">
                {isHTMLModified ? (
                  <>
                    <div className="w-2 h-2 bg-orange-500 rounded-full animate-pulse"></div>
                    <span>Unsaved changes</span>
                  </>
                ) : (
                  <>
                    <div className="w-2 h-2 bg-green-500 rounded-full"></div>
                    <span>All changes saved</span>
                  </>
                )}
              </div>
              
              <div className="flex items-center gap-2">
                <Button
                  onClick={handleResetHTML}
                  variant="outline"
                  size="sm"
                  disabled={!isHTMLModified}
                  className="h-8 text-xs"
                >
                  Reset
                </Button>
                
                <Button
                  onClick={handleSaveHTML}
                  size="sm"
                  disabled={!isHTMLModified}
                  className="h-8 text-xs bg-blue-600 hover:bg-blue-500 disabled:opacity-50 disabled:cursor-not-allowed"
                >
                  <Save className="w-3 h-3 mr-2" />
                  Save HTML
                </Button>
              </div>
            </div>
          </div>
        )}
      </ScrollArea>
    </div>
  );
}