import { useEffect, useRef, useState } from 'react';
import { Button } from '@/components/ui/button';
import { Input } from '@/components/ui/input';
import { Label } from '@/components/ui/label';
import { ScrollArea } from '@/components/ui/scroll-area';
import { FileDown, FileUp, Send, Eye, Code, ZoomIn, ZoomOut, RotateCcw, Save, Trash2 } from 'lucide-react';
import { useAppStore } from '@/store/useAppStore';
import type { ApiEndpoint } from '@/types';
import Editor from '@monaco-editor/react';
import { toast } from 'sonner';

interface ManualTabProps {
  endpoint: ApiEndpoint;
}

const DEFAULT_ZENDESK_LOCALE = 'en-us';
const ZENDESK_SERVER_BASE_URL = 'http://localhost:9527';

interface ZendeskEnvStatus {
  baseUrl?: string;
  subdomain: string;
  defaultLocale: string;
  defaultArticleUrl: string;
  defaultArticleId?: string;
  defaultSectionId?: number | null;
  defaultPermissionGroupId?: number | null;
  defaultUserSegmentId?: number | null;
  defaultUserSegmentIds?: number[];
  defaultLabels?: string[];
  defaultContentTagIds?: string[];
  defaultPromoted?: boolean | null;
  defaultCommentsDisabled?: boolean | null;
  defaultNotifySubscribers?: boolean | null;
  defaultDraft?: boolean | null;
  defaultAttachmentIds?: number[];
  hasCredentials: boolean;
  authType: 'token' | 'password' | null;
  missingFields: string[];
}

function normalizeZendeskLocale(locale?: string): string {
  const normalized = String(locale || '')
    .trim()
    .replace(/_/g, '-')
    .toLowerCase();
  return normalized || DEFAULT_ZENDESK_LOCALE;
}

export function ManualTab({ endpoint }: ManualTabProps) {
  const { manualData, setManualData } = useAppStore();
  const [zendeskUrl, setZendeskUrl] = useState('');
  const [zendeskEnvStatus, setZendeskEnvStatus] = useState<ZendeskEnvStatus | null>(null);
  const [isZendeskSending, setIsZendeskSending] = useState(false);
  const [viewMode, setViewMode] = useState<'preview' | 'code'>('preview');
  const fileInputRef = useRef<HTMLInputElement>(null);

  // 🔍 Zoom 상태 관리
  const [zoom, setZoom] = useState(1);

  // 🎯 Editable HTML State
  const [editableHTML, setEditableHTML] = useState('');
  const [isHTMLModified, setIsHTMLModified] = useState(false);

  const loadZendeskEnvStatus = async (): Promise<ZendeskEnvStatus | null> => {
    try {
      const zendeskAPI = window.electronAPI?.zendesk;
      let result: { success: boolean; data?: ZendeskEnvStatus; error?: string } | null = null;

      if (zendeskAPI?.getEnvConfig) {
        result = await zendeskAPI.getEnvConfig();
      } else {
        const response = await fetch(`${ZENDESK_SERVER_BASE_URL}/api/zendesk/env`);
        const payload = await response.json().catch(() => null);
        if (!response.ok) {
          const errorMessage = payload?.error || `HTTP ${response.status}`;
          throw new Error(errorMessage);
        }
        result = payload;
      }

      if (!result?.success || !result.data) {
        setZendeskEnvStatus(null);
        return null;
      }

      setZendeskEnvStatus(result.data);
      setZendeskUrl((current) => (current.trim() ? current : result.data?.defaultArticleUrl || ''));
      return result.data;
    } catch (error) {
      console.warn('Failed to load Zendesk env config:', error);
      setZendeskEnvStatus(null);
      return null;
    }
  };

  useEffect(() => {
    void loadZendeskEnvStatus();
  }, []);

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

  // 🎨 HTML 생성 함수 (Zendesk 호환)
  const generateHTML = (): string => {
    if (!manualData) {
      return '<p>No manual data available. Please send data from Spec, Builder, or Runner tabs.</p>';
    }

    const { inputUri, activeMethods, jsonSchema, requestExamples, specifications } = manualData;

    // HTML 이스케이프 함수
    const escapeHtml = (unsafe: string | number | boolean | null): string => {
      if (unsafe === null || unsafe === undefined) return 'null';
      if (typeof unsafe === 'boolean') return unsafe.toString();
      if (typeof unsafe === 'number') return unsafe.toString();
      return String(unsafe)
        .replace(/&/g, '&amp;')
        .replace(/</g, '&lt;')
        .replace(/>/g, '&gt;')
        .replace(/"/g, '&quot;')
        .replace(/'/g, '&#039;');
    };

    // 🎯 Zendesk 호환 JSON 포맷터 (&nbsp;로 들여쓰기, <br>로 줄바꿈)
    const formatJsonToZendeskHTML = (jsonStr: string | object | null | undefined, indentLevel = 0): string => {
      let jsonObj: any;
      try {
        jsonObj = typeof jsonStr === 'string' ? JSON.parse(jsonStr) : jsonStr;
      } catch (error) {
        console.warn('Failed to parse JSON schema, using raw value:', error);
        return escapeHtml(jsonStr as string);
      }

      // &nbsp; x 4 per indent level
      const getIndent = (level: number): string => '&nbsp;'.repeat(level * 4);

      const formatValue = (value: any, level: number): string => {
        if (value === null) {
          return `<span style="color: #055bcc; font-weight: bold;">null</span>`;
        }
        if (typeof value === 'string') {
          return `<span style="color: #055bcc;">"${escapeHtml(value)}"</span>`;
        }
        if (typeof value === 'number') {
          return `<span style="color: #0ab66c;">${escapeHtml(value)}</span>`;
        }
        if (typeof value === 'boolean') {
          return `<span style="color: #055bcc; font-weight: bold;">${escapeHtml(value)}</span>`;
        }
        if (Array.isArray(value)) {
          if (value.length === 0) return '[]';
          const items = value.map(item =>
            `${getIndent(level + 1)}${formatValue(item, level + 1)}`
          ).join(',<br>\n');
          return `[<br>\n${items}<br>\n${getIndent(level)}]`;
        }
        if (typeof value === 'object') {
          const keys = Object.keys(value);
          if (keys.length === 0) return '{}';
          const props = keys.map(key =>
            `${getIndent(level + 1)}<span style="color: #c31b1b;">"${escapeHtml(key)}"</span>: ${formatValue(value[key], level + 1)}`
          ).join(',<br>\n');
          return `{<br>\n${props}<br>\n${getIndent(level)}}`;
        }
        return escapeHtml(String(value));
      };

      return formatValue(jsonObj, indentLevel);
    };

    const currentSchema = formatJsonToZendeskHTML(jsonSchema || manualData.jsonSchemaOriginal || '{}');

    // 🎯 Request Examples 생성 (Zendesk 형식)
    let requestExamplesHTML = '';
    if (requestExamples && requestExamples.length > 0) {
      requestExamplesHTML = `
    <h3 id="h_01J4JJ26CHV3XS8BRMW0ND0E1W">
      <strong>Request Examples</strong>
    </h3>
${requestExamples.map((ex, idx) => {
        let exampleJson = ex.code;
        const isAlreadyHTML = typeof ex.code === 'string' && (
          ex.code.includes('<span') ||
          ex.code.includes('<br>') ||
          ex.code.includes('&nbsp;')
        );

        if (isAlreadyHTML) {
          exampleJson = ex.code;
        } else {
          try {
            const parsed = typeof ex.code === 'string' ? JSON.parse(ex.code) : ex.code;
            exampleJson = formatJsonToZendeskHTML(parsed);
          } catch {
            exampleJson = escapeHtml(ex.code);
          }
        }
        return `    <div class="mgt32" style="margin: 10px;">
      <p class="btn_dropdown mgt4" style="font-size: 15px;">${escapeHtml(ex.title)}</p>
      <div style="background-color: #f5f7fa; color: black; padding: 10px 10px 10px 20px;">
        <div style="background-color: #f5f7fa;" align="right">
          <button style="background-color: #1c7ed6; border: none; color: white; padding: 7px 10px 7px 10px; text-align: center; display: inline-block; font-size: 13px; margin: 1px 1px; cursor: pointer; border-radius: 5px;" onclick="copyText('copyReq${idx + 1}')" onmousedown="this.style.backgroundColor='#1D70B5'" onmouseup="this.style.backgroundColor='#1C7ED6'">Copy</button>
        </div>
        <div id="copyReq${idx + 1}" style="font-size: 15px; letter-spacing: 0.01em;">
          ${exampleJson}
        </div>
      </div>
    </div>`;
      }).join('\n')}`;
    }

    // 🎯 Response Examples 생성 (Zendesk 형식)
    let responseExamplesHTML = '';
    const { responseExamples } = manualData;
    if (responseExamples && responseExamples.length > 0) {
      responseExamplesHTML = `
    <br><br>
    <h3 id="h_01J4JJ26GDKM4PZBGT5CCVJEPF">
      <strong>Response Examples</strong>
    </h3>
${responseExamples.map((ex, idx) => {
        let exampleJson = ex.code;
        const isAlreadyHTML = typeof ex.code === 'string' && (
          ex.code.includes('<span') ||
          ex.code.includes('<br>') ||
          ex.code.includes('&nbsp;')
        );

        if (isAlreadyHTML) {
          exampleJson = ex.code;
        } else {
          try {
            const parsed = typeof ex.code === 'string' ? JSON.parse(ex.code) : ex.code;
            exampleJson = formatJsonToZendeskHTML(parsed);
          } catch {
            exampleJson = escapeHtml(ex.code);
          }
        }
        return `    <div class="mgt32" style="margin: 10px;">
      <p class="btn_dropdown mgt4" style="font-size: 15px;">${escapeHtml(ex.title)}</p>
      <div style="background-color: #f5f7fa; color: black; padding: 10px 10px 10px 20px;">
        <div style="background-color: #f5f7fa;" align="right">
          <button style="background-color: #1c7ed6; border: none; color: white; padding: 7px 10px 7px 10px; text-align: center; display: inline-block; font-size: 13px; margin: 1px 1px; cursor: pointer; border-radius: 5px;" onclick="copyText('copyRes${idx + 1}')" onmousedown="this.style.backgroundColor='#1D70B5'" onmouseup="this.style.backgroundColor='#1C7ED6'">Copy</button>
        </div>
        <div id="copyRes${idx + 1}" style="font-size: 15px; letter-spacing: 0.01em;">
          ${exampleJson}
        </div>
      </div>
    </div>`;
      }).join('\n')}`;
    }

    // 🎯 Examples HTML 결합
    const examplesHTML = requestExamplesHTML + responseExamplesHTML;

    // 🎯 Zendesk 호환 HTML 반환 (zd-html-block 래퍼)
    return `<zd-html-block>
  <script>
    function copyText(elementId) {
        var text = document.getElementById(elementId).innerText;
        text = text.replace(/&nbsp;/g, " ");
        text = text.replace(/ /g, " ");
        navigator.clipboard.writeText(text);
    }
  </script>
  <div>
    <h3 id="h_01J4JJ26CH2ZW675YHESZJ9ZBV">
      <strong>Input URI</strong>
    </h3>
    <div class="table-wrap">
      <table style="width: 100%; margin-left: 0px; margin-right: auto;">
        <colgroup>
          <col style="width: 100.00%;">
        </colgroup>
        <tbody>
          <tr>
            <th style="padding: 10px 5px 10px 5px;">
              <p style="text-align: center;">
                <strong>${escapeHtml(inputUri || '{base url} + endpoint')}</strong>
              </p>
            </th>
          </tr>
        </tbody>
      </table>
    </div>
    <br>
    <br>
    <h3 id="h_01J4JJ26CHE5BP89TJEWH778AT">
      <strong>Active Methods</strong>
    </h3>
    <div class="table-wrap">
      <table style="width: 100%; margin-left: 0px; margin-right: auto;">
        <colgroup>
          <col style="width: 100.00%;">
        </colgroup>
        <tbody>
          <tr>
            <th style="padding: 10px 5px 10px 5px;">
              <p style="text-align: center;">
                <strong>${escapeHtml(activeMethods || 'POST, GET, PUT, DELETE')}</strong>
              </p>
            </th>
          </tr>
        </tbody>
      </table>
    </div>
    <br>
    <br>
    <h3 id="h_01J4JJ26CH0H9ZM7RSET9FZMGN">
      <strong>JSON Schema</strong>
    </h3>
    <div class="mgt32" style="margin: 10px;">
      <p class="btn_dropdown mgt4" style="font-size: 15px;">Details</p>
      <div style="background-color: #f5f7fa; color: black; padding: 10px 10px 10px 20px;">
        <div style="background-color: #f5f7fa;" align="right">
          <button style="background-color: #1c7ed6; border: none; color: white; padding: 7px 10px 7px 10px; text-align: center; display: inline-block; font-size: 13px; margin: 1px 1px; cursor: pointer; border-radius: 5px;" onclick="copyText('copyTarget1')" onmousedown="this.style.backgroundColor='#1D70B5'" onmouseup="this.style.backgroundColor='#1C7ED6'">Copy</button>
        </div>
        <div id="copyTarget1" style="font-size: 15px; letter-spacing: 0.01em;">
          ${currentSchema}
        </div>
      </div>
    </div>
    <br>
    <br>
${examplesHTML}
    <br><br>
${(() => {
        // specifications가 이미 완전한 Zendesk HTML인지 확인 (table-wrap, <strong>Specifications 포함)
        const isAlreadyFormattedHTML = specifications &&
          (specifications.includes('<table') || specifications.includes('class="table-wrap"'));
        const hasSpecTitle = specifications &&
          (specifications.includes('>Specifications<') || specifications.includes('<strong>Specifications'));

        if (!specifications) {
          return `    <h3 id="h_01J4JJ26CHA44WRV3GCHGT1D41">
      <strong>Specifications</strong>
    </h3>
    <p>No specifications available.</p>`;
        }

        if (isAlreadyFormattedHTML && hasSpecTitle) {
          // 이미 제목과 테이블이 포함된 완전한 HTML
          return specifications;
        }

        if (isAlreadyFormattedHTML) {
          // 테이블은 있지만 제목이 없는 경우
          return `    <h3 id="h_01J4JJ26CHA44WRV3GCHGT1D41">
      <strong>Specifications</strong>
    </h3>
${specifications}`;
        }

        // 일반 텍스트인 경우
        return `    <h3 id="h_01J4JJ26CHA44WRV3GCHGT1D41">
      <strong>Specifications</strong>
    </h3>
    <p>${escapeHtml(specifications)}</p>`;
      })()}
    <br>
    <br>
  </div>
</zd-html-block>`;
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
      toast.info('Import functionality: Parse HTML to extract manual data');
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
  const handleSendToZendesk = async () => {
    if (!manualData) {
      toast.error('Manual 데이터가 없습니다. Spec/Builder/Runner에서 먼저 전송하세요.');
      return;
    }

    const zendeskAPI = window.electronAPI?.zendesk;
    const useElectronPublisher = !!zendeskAPI?.publishManualWithEnv;

    const envStatus = zendeskEnvStatus ?? await loadZendeskEnvStatus();
    if (!envStatus?.hasCredentials) {
      const missingMessage = envStatus?.missingFields?.length
        ? envStatus.missingFields.join(', ')
        : 'ZENDESK_SUBDOMAIN/ZENDESK_BASE_URL, ZENDESK_EMAIL, ZENDESK_API_TOKEN';
      toast.error(`.env에 Zendesk 설정이 필요합니다: ${missingMessage}`);
      return;
    }

    const html = isHTMLModified && editableHTML ? editableHTML : generateHTML();
    const title = (manualData.title || endpoint.name || '').trim();
    const locale = normalizeZendeskLocale(envStatus.defaultLocale || DEFAULT_ZENDESK_LOCALE);

    try {
      setIsZendeskSending(true);
      let result: {
        success: boolean;
        data?: {
          mode: 'create' | 'update';
          articleId: string;
          locale: string;
          articleUrl: string;
          associatedAttachmentCount: number;
        };
        error?: string;
      };

      if (useElectronPublisher) {
        result = await zendeskAPI!.publishManualWithEnv(
          zendeskUrl.trim(),
          locale,
          html,
          title || undefined,
          undefined
        );
      } else {
        const response = await fetch(`${ZENDESK_SERVER_BASE_URL}/api/zendesk/publish`, {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({
            targetInput: zendeskUrl.trim(),
            locale,
            body: html,
            title: title || undefined,
            draft: undefined,
          }),
        });

        const payload = await response.json().catch(() => null);
        result = payload || { success: false, error: `HTTP ${response.status}` };
      }

      if (!result.success) {
        toast.error(`Zendesk 전송 실패: ${result.error || 'Unknown error'}`);
        return;
      }

      const modeLabel = result.data?.mode === 'create' ? '생성' : '업데이트';
      const articleId = result.data?.articleId || '(unknown)';
      const articleLocale = result.data?.locale || locale;
      const attachmentCount = result.data?.associatedAttachmentCount || 0;
      const articleUrl = result.data?.articleUrl;
      if (articleUrl) {
        setZendeskUrl(articleUrl);
      }

      toast.success(`Zendesk ${modeLabel} 완료 (Article ${articleId}, ${articleLocale}, Attachments ${attachmentCount})`);
    } catch (error) {
      toast.error(`Zendesk 전송 실패: ${error instanceof Error ? error.message : 'Unknown error'}`);
    } finally {
      setIsZendeskSending(false);
    }
  };

  // 🎯 Switch to HTML Code mode
  const handleSwitchToCode = () => {
    // 기존 editableHTML이 없을 때만 generateHTML()로 초기화
    // 이미 코드가 있으면 유지 (코드가 기준)
    if (!editableHTML) {
      setEditableHTML(generateHTML());
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
    // editableHTML을 저장된 HTML로 마크 (isHTMLModified를 false로 하지만 editableHTML은 유지)
    setIsHTMLModified(false);
    // 알림 없이 조용히 저장 (사용자 경험 개선)
  };

  // 🎯 Reset HTML
  const handleResetHTML = () => {
    setEditableHTML(''); // 빈 문자열로 리셋하여 generateHTML() 사용하도록
    setIsHTMLModified(false);
  };

  // 🎯 Clear Content (기본 템플릿 유지)
  const handleClear = () => {
    if (!window.confirm('내용을 클리어하시겠습니까?\n기본 템플릿 구조는 유지됩니다.')) return;

    // 기본 템플릿 값으로 초기화
    setManualData({
      title: endpoint.name || 'Untitled',
      category: '',
      inputUri: '',
      activeMethods: '',
      jsonSchema: '{}',
      jsonSchemaOriginal: undefined,
      jsonSchemaEnhanced: undefined,
      examples: [],
      requestExamples: [],
      responseExamples: [],
      specifications: '',
    });

    // HTML 에디터도 리셋
    setEditableHTML('');
    setIsHTMLModified(false);
  };

  // 🎯 htmlContent: editableHTML이 있으면 그것을 사용, 없으면 generateHTML()
  const htmlContent = editableHTML || generateHTML();
  const hasElectronZendeskSender = typeof window !== 'undefined' && !!window.electronAPI?.zendesk?.publishManualWithEnv;
  const hasZendeskSender = typeof window !== 'undefined';
  const zendeskStatusText = zendeskEnvStatus
    ? zendeskEnvStatus.hasCredentials
      ? `${zendeskEnvStatus.subdomain}.zendesk.com (${zendeskEnvStatus.authType === 'token' ? 'Token' : 'Password'}) · Section ${zendeskEnvStatus.defaultSectionId ?? 'N/A'}`
      : `Missing: ${zendeskEnvStatus.missingFields.join(', ')}`
    : hasElectronZendeskSender
      ? 'Loading .env...'
      : 'Server mode (dev:all)';

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

          <Button
            size="sm"
            variant="outline"
            onClick={handleClear}
            className="h-7 px-2 text-xs text-red-400 border-red-600/50 hover:bg-red-900/20 hover:text-red-300"
          >
            <Trash2 className="w-3 h-3 mr-1" />
            Clear
          </Button>

          <div className="h-4 w-px bg-zinc-700" />

          {/* Zendesk - Compact */}
          <div className="flex items-center gap-2 flex-1 min-w-0">
            <Label className="text-xs text-zinc-400 whitespace-nowrap">Zendesk:</Label>
            <Input
              value={zendeskUrl}
              onChange={(e) => setZendeskUrl(e.target.value)}
              placeholder="Article URL/ID (비우면 기본 Section에 신규 생성)"
              className="bg-zinc-800 border-zinc-700 h-7 text-xs flex-1 min-w-0"
            />
            <span
              className={`hidden xl:inline text-[10px] whitespace-nowrap ${
                zendeskEnvStatus?.hasCredentials ? 'text-emerald-400' : 'text-amber-400'
              }`}
              title={zendeskStatusText}
            >
              {zendeskEnvStatus?.hasCredentials ? 'Env Ready' : 'Env Check'}
            </span>
            <Button
              size="sm"
              onClick={handleSendToZendesk}
              disabled={isZendeskSending || !hasZendeskSender}
              title={zendeskStatusText}
              className="h-7 px-2 text-xs bg-blue-600 hover:bg-blue-500 whitespace-nowrap disabled:opacity-50 disabled:cursor-not-allowed"
            >
              <Send className="w-3 h-3 mr-1" />
              {isZendeskSending ? 'Sending...' : 'Send'}
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
