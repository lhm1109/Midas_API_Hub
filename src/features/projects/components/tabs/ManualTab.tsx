import { useState, useRef } from 'react';
import { Button } from '@/components/ui/button';
import { Input } from '@/components/ui/input';
import { Label } from '@/components/ui/label';
import { ScrollArea } from '@/components/ui/scroll-area';
import { FileDown, FileUp, Send, Eye, Code, ZoomIn, ZoomOut, RotateCcw, Save } from 'lucide-react';
import { useAppStore } from '@/store/useAppStore';
import type { ApiEndpoint } from '@/types';
import Editor from '@monaco-editor/react';

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
      e.stopPropagation();
      const delta = e.deltaY > 0 ? -0.05 : 0.05;
      setZoom(prev => Math.max(0.3, Math.min(3, prev + delta)));
    }
  };

  // 🎨 HTML 생성 함수
  const generateHTML = (): string => {
    if (!manualData) {
      return '<p>No manual data available. Please send data from Spec, Builder, or Runner tabs.</p>';
    }

    const { title, category, inputUri, activeMethods, jsonSchema, requestExamples, responseExamples, specifications } = manualData;
    
    // 🎯 Spec에서 보낸 스키마 사용 (JSON 문자열을 HTML로 포맷)
    const formatJsonToHTML = (jsonStr: string): string => {
      try {
        // JSON 문자열을 객체로 파싱
        const jsonObj = typeof jsonStr === 'string' ? JSON.parse(jsonStr) : jsonStr;
        const prettyJson = JSON.stringify(jsonObj, null, 2);
        
        // HTML 포맷팅
        return prettyJson
          .split('\n')
          .map(line => {
            const leadingSpaces = line.match(/^(\s*)/)?.[1] || '';
            const indent = leadingSpaces.replace(/ /g, '&nbsp;&nbsp;');
            const trimmedLine = line.trim();
            
            const keyMatch = trimmedLine.match(/^"([^"]+)":\s*(.+)$/);
            if (keyMatch) {
              const key = keyMatch[1];
              let value = keyMatch[2];
              const hasComma = value.endsWith(',');
              if (hasComma) {
                value = value.slice(0, -1);
              }
              
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
            
            if (trimmedLine.match(/^[{\[\}\]],?$/)) {
              return indent + trimmedLine;
            }
            
            return indent + trimmedLine;
          })
          .join('<br>');
      } catch (error) {
        // JSON 파싱 실패 시 원본 반환 (이미 HTML일 수도 있음)
        console.warn('Failed to parse JSON schema, using raw value:', error);
        return jsonStr;
      }
    };
    
    const currentSchema = formatJsonToHTML(jsonSchema || manualData.jsonSchemaOriginal || '{}');

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
            font-weight: 600;
            color: #2f3941;
            cursor: pointer;
            border-radius: 4px;
            margin: 0;
            position: relative;
            user-select: none;
        }
        
        .btn_dropdown:hover {
            background-color: #e8e9ea;
        }
        
        .btn_dropdown::after {
            content: '▼';
            position: absolute;
            right: 16px;
            font-size: 10px;
            transition: transform 0.3s;
        }
        
        .btn_dropdown.active::after {
            transform: rotate(180deg);
        }
        
        .btn_dropdown.active {
            border-radius: 4px 4px 0 0;
            border-bottom: none;
        }
        
        .code-block-wrapper {
            border: 1px solid #d8dcde;
            border-top: none;
            border-radius: 0 0 4px 4px;
            overflow: hidden;
            max-height: 0;
            opacity: 0;
            transition: max-height 0.3s ease-out, opacity 0.3s ease-out;
        }
        
        .code-block-wrapper.active {
            max-height: 10000px;
            opacity: 1;
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
<p class=\"btn_dropdown mgt4 active\" onclick=\"toggleAccordion(this)\">Details</p>
<div class=\"code-block-wrapper active\">
<div class=\"code-block-header\">
<button class=\"copy-btn\" onclick=\"copyText('copyTarget1')\">Copy</button>
</div>
<div class=\"code-content\" id=\"copyTarget1\">${currentSchema}</div>
</div>
</div>

${(requestExamples && requestExamples.length > 0) ? `
<h3 id=\"h_01J4K93QE3BGDFSY9AZXQ9ZJ8K\"><strong>Request Examples</strong></h3>
${requestExamples.map((ex, idx) => `
<div class=\"mgt32\">
<p class=\"btn_dropdown mgt4 active\" onclick=\"toggleAccordion(this)\">${ex.title}</p>
<div class=\"code-block-wrapper active\">
<div class=\"code-block-header\">
<button class=\"copy-btn\" onclick=\"copyText('copyTargetReq${idx}')\">Copy</button>
</div>
<div class=\"code-content\" id=\"copyTargetReq${idx}\">${ex.code}</div>
</div>
</div>
`).join('')}
` : ''}

${(responseExamples && responseExamples.length > 0) ? `
<h3 id=\"h_01J4K93QE3DTFGJEZ2NXN6ZH33\"><strong>Response Examples</strong></h3>
${responseExamples.map((ex, idx) => `
<div class=\"mgt32\">
<p class=\"btn_dropdown mgt4 active\" onclick=\"toggleAccordion(this)\">${ex.title}</p>
<div class=\"code-block-wrapper active\">
<div class=\"code-block-header\">
<button class=\"copy-btn\" onclick=\"copyText('copyTargetRes${idx}')\">Copy</button>
</div>
<div class=\"code-content\" id=\"copyTargetRes${idx}\">${ex.code}</div>
</div>
</div>
`).join('')}
` : ''}

${specifications}

</div>
</div>

<script>
// 🎯 Copy Text Function
function copyText(elementId) {
    var element = document.getElementById(elementId);
    var text = element.innerText || element.textContent;
    text = text.replace(/&nbsp;/g, " ");
    text = text.replace(/ /g, " ");
    
    var textarea = document.createElement('textarea');
    textarea.value = text;
    textarea.style.position = 'fixed';
    textarea.style.opacity = '0';
    document.body.appendChild(textarea);
    textarea.select();
    
    try {
        document.execCommand('copy');
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

// 🎯 Accordion Toggle Function
function toggleAccordion(button) {
    var wrapper = button.nextElementSibling;
    button.classList.toggle('active');
    wrapper.classList.toggle('active');
}
</script>

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
    reader.onload = (_event) => {
      // const htmlContent = event.target?.result as string;
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
    <div className="flex h-full w-full flex-col bg-zinc-950 relative">
      {/* Compact Header - All in One Line */}
      <div className="absolute top-0 left-0 right-0 z-20 px-3 py-2 bg-zinc-900/95 backdrop-blur-md border-b border-zinc-700/50 shadow-lg">
        <div className="flex items-center gap-3">
          {/* Title */}
          <div className="flex items-center gap-2">
            <h3 className="text-xs font-medium text-white whitespace-nowrap">📖 Manual</h3>
            <div className="h-4 w-px bg-zinc-700" />
          </div>
          
          {/* View Mode Toggle */}
          <div className="flex items-center gap-1">
            <Button
              size="sm"
              variant={viewMode === 'preview' ? 'default' : 'outline'}
              onClick={() => setViewMode('preview')}
              className="h-7 px-2 text-xs"
            >
              <Eye className="w-3 h-3 mr-1" />
              Preview
            </Button>
            <Button
              size="sm"
              variant={viewMode === 'code' ? 'default' : 'outline'}
              onClick={handleSwitchToCode}
              className="h-7 px-2 text-xs"
            >
              <Code className="w-3 h-3 mr-1" />
              Code
            </Button>
          </div>

          <div className="h-4 w-px bg-zinc-700" />

          {/* Import/Export */}
          <Button size="sm" variant="outline" onClick={handleImport} className="h-7 px-2 text-xs">
            <FileUp className="w-3 h-3 mr-1" />
            Import
          </Button>
          <input
            ref={fileInputRef}
            type="file"
            accept=".html"
            onChange={handleFileChange}
            className="hidden"
          />
          
          <Button size="sm" variant="outline" onClick={handleExport} className="h-7 px-2 text-xs">
            <FileDown className="w-3 h-3 mr-1" />
            Export
          </Button>

          <div className="h-4 w-px bg-zinc-700" />

          {/* Zendesk - Compact */}
          <div className="flex items-center gap-2 flex-1 min-w-0">
            <Label className="text-xs text-zinc-400 whitespace-nowrap">Zendesk:</Label>
            <Input
              value={zendeskUrl}
              onChange={(e) => setZendeskUrl(e.target.value)}
              placeholder="https://your-zendesk.com/api/v2/..."
              className="bg-zinc-800 border-zinc-700 h-7 text-xs flex-1 min-w-0"
            />
            <Button size="sm" onClick={handleSendToZendesk} className="h-7 px-2 text-xs bg-blue-600 hover:bg-blue-500 whitespace-nowrap">
              <Send className="w-3 h-3 mr-1" />
              Send
            </Button>
          </div>
        </div>
      </div>

      {/* Content Area - Full Height */}
      <div className="flex-1 h-full w-full overflow-hidden">
        {viewMode === 'preview' ? (
          <div className="relative h-full">
            {/* Zoom 컨트롤 바 - Floating below header */}
            {manualData && (
              <div className="absolute top-12 left-0 right-0 z-30 flex items-center justify-center gap-3 px-6 pointer-events-none">
                {/* Zoom Controls */}
                <div className="bg-zinc-900/95 backdrop-blur-sm border border-zinc-700 rounded-lg px-2 py-1.5 flex items-center gap-2 shadow-lg pointer-events-auto">
                  <Button
                    size="sm"
                    variant="ghost"
                    onClick={handleZoomOut}
                    className="h-6 w-6 p-0 hover:bg-zinc-700"
                    title="Zoom Out (Ctrl + Wheel Down)"
                  >
                    <ZoomOut className="w-3 h-3" />
                  </Button>
                  
                  <span className="text-xs font-mono text-zinc-300 min-w-[50px] text-center">
                    {Math.round(zoom * 100)}%
                  </span>
                  
                  <Button
                    size="sm"
                    variant="ghost"
                    onClick={handleZoomIn}
                    className="h-6 w-6 p-0 hover:bg-zinc-700"
                    title="Zoom In (Ctrl + Wheel Up)"
                  >
                    <ZoomIn className="w-3 h-3" />
                  </Button>
                  
                  <div className="h-3 w-px bg-zinc-700 mx-1" />
                  
                  <Button
                    size="sm"
                    variant="ghost"
                    onClick={handleResetZoom}
                    className="h-6 w-6 p-0 hover:bg-zinc-700"
                    title="Reset Zoom (100%)"
                  >
                    <RotateCcw className="w-3 h-3" />
                  </Button>
                  
                  <div className="text-[10px] text-zinc-500 ml-1 hidden md:block">
                    💡 Ctrl+Wheel
                  </div>
                </div>
              </div>
            )}
            
            <div 
              className="h-full w-full overflow-hidden"
              onWheel={handleWheel}
            >
              <ScrollArea className="h-full w-full">
                <div 
                  className="p-6 pt-24"
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
                    width: `${100 / zoom}%`,
                    minHeight: `${100 / zoom}vh`
                  }}
                >
                  <iframe
                    srcDoc={htmlContent}
                    className="w-full bg-white rounded-lg border border-zinc-700 pointer-events-auto"
                    title="Manual Preview"
                    sandbox="allow-scripts allow-same-origin"
                    style={{ 
                      height: '100vh',
                      minHeight: '100vh'
                    }}
                  />
                </div>
              )}
              </div>
              </ScrollArea>
            </div>
          </div>
        ) : (
          <div className="h-full flex flex-col bg-zinc-950 overflow-hidden">
            <div className="p-3 border-b border-zinc-800 bg-zinc-900 flex-shrink-0 mt-12">
              <h3 className="text-xs font-medium">Editable HTML Code</h3>
              <p className="text-xs text-zinc-500 mt-0.5">
                {isHTMLModified ? '✏️ Modified - Click Save to apply changes' : '✅ Ready to edit'}
              </p>
            </div>
            
            {/* 🎯 Monaco Editor - VSCode Style */}
            <div className="flex-1 relative">
              <Editor
                height="100%"
                defaultLanguage="html"
                theme="vs-dark"
                value={editableHTML || htmlContent}
                onChange={(value) => handleHTMLChange(value || '')}
                options={{
                  minimap: { enabled: true },
                  fontSize: 13,
                  lineNumbers: 'on',
                  scrollBeyondLastLine: false,
                  wordWrap: 'on',
                  automaticLayout: true,
                  tabSize: 2,
                  formatOnPaste: true,
                  formatOnType: true,
                  scrollbar: {
                    vertical: 'auto',
                    horizontal: 'auto',
                    verticalScrollbarSize: 10,
                    horizontalScrollbarSize: 10,
                  },
                }}
              />
              
              {/* 🎯 Modified Indicator */}
              {isHTMLModified && (
                <div className="absolute top-2 right-2 px-2 py-1 bg-orange-600/20 border border-orange-600/50 rounded text-xs text-orange-400 z-10">
                  Modified
                </div>
              )}
            </div>
            
            {/* 🎯 Footer with Save Button - Compact */}
            <div className="border-t border-zinc-800 bg-zinc-900 px-3 py-2 flex items-center justify-between flex-shrink-0">
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
                  className="h-7 px-2 text-xs"
                >
                  Reset
                </Button>
                
                <Button
                  onClick={handleSaveHTML}
                  size="sm"
                  disabled={!isHTMLModified}
                  className="h-7 px-2 text-xs bg-blue-600 hover:bg-blue-500 disabled:opacity-50 disabled:cursor-not-allowed"
                >
                  <Save className="w-3 h-3 mr-1" />
                  Save
                </Button>
              </div>
            </div>
          </div>
        )}
      </div>
    </div>
  );
}