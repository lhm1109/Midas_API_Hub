import { useEffect, useRef, useState } from 'react';
import { Button } from '@/components/ui/button';
import { Input } from '@/components/ui/input';
import { Label } from '@/components/ui/label';
import { ScrollArea } from '@/components/ui/scroll-area';
import { Switch } from '@/components/ui/switch';
import { Tabs, TabsList, TabsTrigger } from '@/components/ui/tabs';
import { FileDown, FileUp, Send, Eye, Code, ZoomIn, ZoomOut, RotateCcw, Save, Trash2, GitCompare, RefreshCw, ArrowUpToLine } from 'lucide-react';
import { useAppStore } from '@/store/useAppStore';
import type { ApiEndpoint } from '@/types';
import Editor from '@monaco-editor/react';
import { toast } from 'sonner';

interface ManualTabProps {
  endpoint: ApiEndpoint;
}

type ManualPublisher = 'zendesk' | 'confluence';
type ZendeskLocaleOption = 'ko' | 'en-us' | 'jp';

const DEFAULT_ZENDESK_LOCALE = 'en-us';
const MANUAL_SERVER_BASE_URL = 'http://localhost:9527';
const ZENDESK_TRANSLATION_BODY_LIMIT_BYTES = 1_000_000;
const ZENDESK_LOCALE_OPTIONS: ZendeskLocaleOption[] = ['ko', 'en-us', 'jp'];
const REQUIRED_ZENDESK_LOCALES: ZendeskLocaleOption[] = ['ko', 'en-us'];
const DEFAULT_ZENDESK_LOCALES: Record<ZendeskLocaleOption, boolean> = {
  ko: true,
  'en-us': true,
  jp: false,
};

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

interface ConfluenceEnvStatus {
  baseUrl?: string;
  defaultPageUrl?: string;
  defaultPageId?: string;
  defaultSpaceId?: string;
  defaultParentPageId?: string;
  hasCredentials: boolean;
  authType: 'bearer' | 'basic' | null;
  missingFields: string[];
}

interface ZendeskHtmlOptions {
  compactCode?: boolean;
}

function getUtf8ByteSize(value: string): number {
  return new TextEncoder().encode(value).length;
}

function formatByteSize(bytes: number): string {
  if (bytes >= 1_000_000) {
    return `${(bytes / 1_000_000).toFixed(2)} MB`;
  }
  if (bytes >= 1_000) {
    return `${(bytes / 1_000).toFixed(1)} KB`;
  }
  return `${bytes} bytes`;
}

function normalizeZendeskLocale(locale?: string): string {
  const normalized = String(locale || '')
    .trim()
    .replace(/_/g, '-')
    .toLowerCase();
  return normalized || DEFAULT_ZENDESK_LOCALE;
}

function isRequiredZendeskLocale(locale: ZendeskLocaleOption): boolean {
  return REQUIRED_ZENDESK_LOCALES.includes(locale);
}

function parseZendeskLocaleFromTarget(targetInput?: string): ZendeskLocaleOption | null {
  const raw = String(targetInput || '').trim();
  if (!raw || /^\d+$/.test(raw)) {
    return null;
  }

  try {
    const parsedUrl = new URL(raw);
    const pathSegments = parsedUrl.pathname.split('/').filter(Boolean);
    let locale =
      parsedUrl.searchParams.get('locale') ||
      parsedUrl.searchParams.get('lang') ||
      '';

    const articleIndex = pathSegments.indexOf('articles');
    if (!locale && articleIndex >= 2 && pathSegments[0] === 'hc') {
      locale = pathSegments[1];
    }

    const normalized = normalizeZendeskLocale(locale);
    return ZENDESK_LOCALE_OPTIONS.includes(normalized as ZendeskLocaleOption)
      ? (normalized as ZendeskLocaleOption)
      : null;
  } catch {
    return null;
  }
}

function parseZendeskLabelInput(value: string): string[] {
  const normalized = String(value || '')
    .split(',')
    .map((token) => token.trim())
    .filter(Boolean);

  return Array.from(new Set(normalized));
}

function formatZendeskLabelInput(labels?: string[] | null): string {
  return Array.isArray(labels) ? labels.join(', ') : '';
}

function buildZendeskArticleTitle(endpoint: ApiEndpoint, rawTitle?: string | null): string {
  const endpointPath = String(endpoint.path || endpoint.id || '').replace(/^\/+/, '').trim();
  // endpoint.name이 항상 최신 이름이므로 우선 사용. rawTitle은 endpoint.name이 없을 때만 fallback
  const baseTitle = String(endpoint.name || rawTitle || endpoint.path || endpoint.id || 'Untitled').trim();

  // 이미 "endpoint : title" 형태로 저장된 경우 중복 prefix를 방지
  if (!endpointPath || baseTitle.startsWith(`${endpointPath} : `)) {
    return baseTitle;
  }

  return `${endpointPath} : ${baseTitle}`;
}

export function ManualTab({ endpoint }: ManualTabProps) {
  const { manualData, setManualData, updateManualData } = useAppStore();
  const [manualPublisher, setManualPublisher] = useState<ManualPublisher>('zendesk');
  const [zendeskUrl, setZendeskUrl] = useState('');
  const [zendeskEnvStatus, setZendeskEnvStatus] = useState<ZendeskEnvStatus | null>(null);
  const [isZendeskSending, setIsZendeskSending] = useState(false);
  const [confluenceTarget, setConfluenceTarget] = useState('');
  const [confluenceEnvStatus, setConfluenceEnvStatus] = useState<ConfluenceEnvStatus | null>(null);
  const [isConfluenceSending, setIsConfluenceSending] = useState(false);
  const [viewMode, setViewMode] = useState<'preview' | 'code' | 'diff'>('preview');
  const [zendeskLabelInput, setZendeskLabelInput] = useState('');
  const [zendeskLocales, setZendeskLocales] = useState<Record<ZendeskLocaleOption, boolean>>(DEFAULT_ZENDESK_LOCALES);
  const fileInputRef = useRef<HTMLInputElement>(null);

  // 🔍 Zoom 상태 관리
  const [zoom, setZoom] = useState(1);

  // 🔄 Zendesk Diff 상태
  const [isFetchingZendesk, setIsFetchingZendesk] = useState(false);
  const [zendeskFetchedBody, setZendeskFetchedBody] = useState<string | null>(null);
  const [zendeskFetchedTitle, setZendeskFetchedTitle] = useState<string | null>(null);
  const [zendeskFetchedUpdatedAt, setZendeskFetchedUpdatedAt] = useState<string | null>(null);

  // 🎯 Editable HTML State
  const [editableHTMLByPublisher, setEditableHTMLByPublisher] = useState<Record<ManualPublisher, string>>({
    zendesk: '',
    confluence: '',
  });
  const [isHTMLModifiedByPublisher, setIsHTMLModifiedByPublisher] = useState<Record<ManualPublisher, boolean>>({
    zendesk: false,
    confluence: false,
  });

  const loadZendeskEnvStatus = async (): Promise<ZendeskEnvStatus | null> => {
    try {
      const zendeskAPI = window.electronAPI?.zendesk;
      let result: { success: boolean; data?: ZendeskEnvStatus; error?: string } | null = null;

      if (zendeskAPI?.getEnvConfig) {
        result = await zendeskAPI.getEnvConfig();
      } else {
        const response = await fetch(`${MANUAL_SERVER_BASE_URL}/api/zendesk/env`);
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

  const loadConfluenceEnvStatus = async (): Promise<ConfluenceEnvStatus | null> => {
    try {
      const response = await fetch(`${MANUAL_SERVER_BASE_URL}/api/confluence/env`);
      const payload = await response.json().catch(() => null);
      if (!response.ok) {
        const errorMessage = payload?.error || `HTTP ${response.status}`;
        throw new Error(errorMessage);
      }

      if (!payload?.success || !payload.data) {
        setConfluenceEnvStatus(null);
        return null;
      }

      setConfluenceEnvStatus(payload.data);
      setConfluenceTarget((current) => (current.trim() ? current : payload.data?.defaultPageUrl || ''));
      return payload.data;
    } catch (error) {
      console.warn('Failed to load Confluence env config:', error);
      setConfluenceEnvStatus(null);
      return null;
    }
  };

  useEffect(() => {
    void loadZendeskEnvStatus();
    void loadConfluenceEnvStatus();
  }, []);

  // manualData.url이 있으면 zendeskUrl 초기 설정
  useEffect(() => {
    if (manualData?.url && !zendeskUrl.trim()) {
      setZendeskUrl(manualData.url);
    }
  }, [manualData?.url]);

  useEffect(() => {
    const nextLabelInput = manualData?.zendeskLabelNames !== undefined
      ? formatZendeskLabelInput(manualData.zendeskLabelNames)
      : formatZendeskLabelInput(zendeskEnvStatus?.defaultLabels);

    setZendeskLabelInput(nextLabelInput);
  }, [manualData?.zendeskLabelNames, zendeskEnvStatus?.defaultLabels]);

  const handleFetchFromZendesk = async () => {
    const targetUrl = zendeskUrl.trim();
    if (!targetUrl) {
      toast.error('Zendesk URL 또는 Article ID를 입력하세요.');
      return;
    }

    const envStatus = zendeskEnvStatus ?? await loadZendeskEnvStatus();
    if (!envStatus?.hasCredentials) {
      toast.error('.env에 Zendesk 설정이 필요합니다.');
      return;
    }

    try {
      setIsFetchingZendesk(true);
      const locale = parseZendeskLocaleFromTarget(targetUrl)
        || ZENDESK_LOCALE_OPTIONS.find((option) => zendeskLocales[option])
        || normalizeZendeskLocale(envStatus.defaultLocale);
      const params = new URLSearchParams({ targetInput: targetUrl, locale });
      const response = await fetch(`${MANUAL_SERVER_BASE_URL}/api/zendesk/article?${params}`);
      const payload = await response.json().catch(() => null);
      if (!response.ok || !payload?.success) {
        toast.error(`Zendesk 불러오기 실패: ${payload?.error || `HTTP ${response.status}`}`);
        return;
      }

      setZendeskFetchedBody(payload.data.body || '');
      setZendeskFetchedTitle(payload.data.title || '');
      setZendeskFetchedUpdatedAt(payload.data.updatedAt || null);
      setViewMode('diff');
      toast.success(`Zendesk 아티클 불러오기 완료 (${payload.data.articleId})`);
    } catch (error) {
      toast.error(`Zendesk 불러오기 실패: ${error instanceof Error ? error.message : 'Unknown error'}`);
    } finally {
      setIsFetchingZendesk(false);
    }
  };

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

  const escapeHtml = (unsafe: string | number | boolean | null | undefined): string => {
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

  const escapeCodeHtml = (unsafe: string): string => {
    return String(unsafe)
      .replace(/&/g, '&amp;')
      .replace(/</g, '&lt;')
      .replace(/>/g, '&gt;');
  };

  const formatManualInputUri = (inputUri?: string): string => {
    const rawInputUri = String(inputUri || endpoint.path || '').trim();
    if (!rawInputUri) {
      return '{base url} + endpoint';
    }

    if (rawInputUri.includes('{base url}')) {
      return rawInputUri;
    }

    return `{base url} + ${rawInputUri.replace(/^\/+/, '')}`;
  };

  const normalizeCopyText = (text: string): string => {
    return String(text || '')
      .replace(/\u00A0/g, ' ')
      .replace(/\u200B/g, '')
      .replace(/\r\n/g, '\n');
  };

  const serializeJsonForZendesk = (jsonValue: string | object | null | undefined): string => {
    if (jsonValue === undefined) {
      return '{}';
    }

    if (jsonValue === null) {
      return 'null';
    }

    if (typeof jsonValue === 'string') {
      try {
        return JSON.stringify(JSON.parse(jsonValue), null, 2);
      } catch {
        return jsonValue;
      }
    }

    try {
      return JSON.stringify(jsonValue, null, 2);
    } catch {
      return String(jsonValue);
    }
  };

  const formatPlainTextToZendeskHTML = (rawText: string, compactCode = false): string => {
    const escaped = escapeCodeHtml(rawText);
    if (compactCode) {
      return escaped;
    }
    return escaped
      .replace(/ /g, '&nbsp;')
      .replace(/\n/g, '<br>');
  };

  const highlightJsonTextForZendesk = (rawText: string, compactCode = false): string => {
    if (compactCode) {
      return formatPlainTextToZendeskHTML(rawText, true);
    }

    const tokenRegex = /"(?:\\u[\da-fA-F]{4}|\\[^u]|[^\\"])*"(?=\s*:)|"(?:\\u[\da-fA-F]{4}|\\[^u]|[^\\"])*"|\b(?:true|false|null)\b|-?\d+(?:\.\d+)?(?:[eE][+\-]?\d+)?/g;

    let result = '';
    let lastIndex = 0;

    rawText.replace(tokenRegex, (match, offset) => {
      result += formatPlainTextToZendeskHTML(rawText.slice(lastIndex, offset), false);

      let style = 'color: #111827;';
      if (/^"/.test(match)) {
        style = 'color: #055bcc;';
      } else if (/^(true|false|null)$/.test(match)) {
        style = 'color: #055bcc; font-weight: bold;';
      } else {
        style = 'color: #0ab66c;';
      }

      if (/^\s*:/.test(rawText.slice(offset + match.length))) {
        style = 'color: #c31b1b;';
      }

      result += `<span style="${style}">${formatPlainTextToZendeskHTML(match, false)}</span>`;
      lastIndex = offset + match.length;
      return match;
    });

    result += formatPlainTextToZendeskHTML(rawText.slice(lastIndex), false);
    return result;
  };

  const formatCodeTextToZendeskHTML = (rawText: string, compactCode = false): string => {
    if (compactCode) {
      return formatPlainTextToZendeskHTML(rawText, true);
    }

    try {
      JSON.parse(rawText);
      return highlightJsonTextForZendesk(rawText, false);
    } catch {
      return formatPlainTextToZendeskHTML(rawText, false);
    }
  };

  const isPreformattedZendeskCode = (value: unknown): value is string => {
    return typeof value === 'string' && (
      value.includes('<span') ||
      value.includes('<br>') ||
      value.includes('&nbsp;')
    );
  };

  const extractCopyTextFromPreformattedHtml = (html: string): string => {
    const encodedCopyText = html.match(/data-copy-text="([^"]+)"/i)?.[1];
    if (encodedCopyText) {
      try {
        return normalizeCopyText(decodeURIComponent(encodedCopyText));
      } catch {
        // Fall through to text extraction when encoded payload is malformed.
      }
    }

    if (typeof document !== 'undefined') {
      const container = document.createElement('div');
      container.innerHTML = html.replace(/<br\s*\/?>/gi, '\n');
      return normalizeCopyText(container.textContent || container.innerText || '');
    }

    return normalizeCopyText(
      html
        .replace(/<br\s*\/?>/gi, '\n')
        .replace(/<[^>]+>/g, '')
        .replace(/&nbsp;/gi, ' ')
        .replace(/&quot;/gi, '"')
        .replace(/&#039;/gi, "'")
        .replace(/&lt;/gi, '<')
        .replace(/&gt;/gi, '>')
        .replace(/&amp;/gi, '&')
    );
  };

  const createZendeskCodePayload = (
    value: string | object | null | undefined,
    options: ZendeskHtmlOptions = {}
  ): { displayHtml: string; copyText: string } => {
    if (isPreformattedZendeskCode(value)) {
      const copyText = extractCopyTextFromPreformattedHtml(value);
      return {
        displayHtml: options.compactCode ? formatCodeTextToZendeskHTML(copyText, true) : value,
        copyText,
      };
    }

    const rawText = serializeJsonForZendesk(value);
    return {
      displayHtml: formatCodeTextToZendeskHTML(rawText, !!options.compactCode),
      copyText: rawText,
    };
  };

  // 🎨 HTML 생성 함수 (Zendesk 호환)
  const getZendeskCodeStyle = (compactCode = false): string => {
    const baseStyle = "font-size: 15px; letter-spacing: 0.01em; font-family: Consolas, 'Courier New', monospace; line-height: 1.6; word-break: break-word;";
    return compactCode
      ? `${baseStyle} white-space: pre-wrap;`
      : baseStyle;
  };

  const renderZendeskCopyTextAttribute = (copyText: string, compactCode = false): string => {
    if (compactCode) {
      return '';
    }
    return ` data-copy-text="${encodeURIComponent(normalizeCopyText(copyText))}"`;
  };

  const compactZendeskCodeBlocks = (html: string): string => {
    return String(html || '').replace(
      /<div\s+id="(copy(?:Req|Res)\d+|copyTarget1)"([^>]*)>([\s\S]*?)<\/div>/g,
      (match, id: string, attrs: string, innerHtml: string) => {
        const copyText = extractCopyTextFromPreformattedHtml(`<div id="${id}"${attrs}>${innerHtml}</div>`);
        if (!copyText) {
          return match;
        }
        return `<div id="${id}" style="${getZendeskCodeStyle(true)}">${formatCodeTextToZendeskHTML(copyText, true)}</div>`;
      }
    );
  };

  const generateZendeskHTML = (options: ZendeskHtmlOptions = {}): string => {
    if (!manualData) {
      return '<p>No manual data available. Please send data from Spec, Builder, or Runner tabs.</p>';
    }

    const compactCode = !!options.compactCode;
    const { inputUri, activeMethods, jsonSchema, requestExamples, specifications } = manualData;
    const displayedActiveMethods = endpoint.method?.trim() || activeMethods?.trim() || '-';
    const displayedInputUri = formatManualInputUri(inputUri);

    const schemaPayload = createZendeskCodePayload(jsonSchema || manualData.jsonSchemaOriginal || '{}', options);
    const currentSchema = schemaPayload.displayHtml;

    // 🎯 Request Examples 생성 (Zendesk 형식)
    let requestExamplesHTML = '';
    if (requestExamples && requestExamples.length > 0) {
      requestExamplesHTML = `
    <h3 id="h_01J4JJ26CHV3XS8BRMW0ND0E1W">
      <strong>Request Examples</strong>
    </h3>
${requestExamples.map((ex, idx) => {
        const examplePayload = createZendeskCodePayload(ex.code, options);
        return `    <div class="mgt32" style="margin: 10px;">
      <p class="btn_dropdown mgt4" style="font-size: 15px;">${escapeHtml(ex.title)}</p>
      <div style="background-color: #f5f7fa; color: black; padding: 10px 10px 10px 20px;">
        <div style="background-color: #f5f7fa;" align="right">
          <button style="background-color: #1c7ed6; border: none; color: white; padding: 7px 10px 7px 10px; text-align: center; display: inline-block; font-size: 13px; margin: 1px 1px; cursor: pointer; border-radius: 5px;" onclick="copyText('copyReq${idx + 1}')" onmousedown="this.style.backgroundColor='#1D70B5'" onmouseup="this.style.backgroundColor='#1C7ED6'">Copy</button>
        </div>
        <div id="copyReq${idx + 1}"${renderZendeskCopyTextAttribute(examplePayload.copyText, compactCode)} style="${getZendeskCodeStyle(compactCode)}">${examplePayload.displayHtml}</div>
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
        const examplePayload = createZendeskCodePayload(ex.code, options);
        return `    <div class="mgt32" style="margin: 10px;">
      <p class="btn_dropdown mgt4" style="font-size: 15px;">${escapeHtml(ex.title)}</p>
      <div style="background-color: #f5f7fa; color: black; padding: 10px 10px 10px 20px;">
        <div style="background-color: #f5f7fa;" align="right">
          <button style="background-color: #1c7ed6; border: none; color: white; padding: 7px 10px 7px 10px; text-align: center; display: inline-block; font-size: 13px; margin: 1px 1px; cursor: pointer; border-radius: 5px;" onclick="copyText('copyRes${idx + 1}')" onmousedown="this.style.backgroundColor='#1D70B5'" onmouseup="this.style.backgroundColor='#1C7ED6'">Copy</button>
        </div>
        <div id="copyRes${idx + 1}"${renderZendeskCopyTextAttribute(examplePayload.copyText, compactCode)} style="${getZendeskCodeStyle(compactCode)}">${examplePayload.displayHtml}</div>
      </div>
    </div>`;
      }).join('\n')}`;
    }

    // 🎯 Examples HTML 결합
    const examplesHTML = requestExamplesHTML + responseExamplesHTML;

    const isAlreadyFormattedSpecificationHTML = !!specifications && (
      specifications.includes('<table') || specifications.includes('class="table-wrap"')
    );
    const hasTopLevelSpecificationsHeading = !!specifications && (
      specifications.includes('>Specifications<') ||
      specifications.includes('<strong>Specifications')
    );
    const specificationSectionHTML = (() => {
      if (!specifications) {
        return `    <h3 id="h_01J4JJ26CHA44WRV3GCHGT1D41">
      <strong>Specifications</strong>
    </h3>
    <p>No specifications available.</p>`;
      }

      if (hasTopLevelSpecificationsHeading) {
        return specifications;
      }

      if (isAlreadyFormattedSpecificationHTML) {
        return `    <h3 id="h_01J4JJ26CHA44WRV3GCHGT1D41">
      <strong>Specifications</strong>
    </h3>
${specifications}`;
      }

      return `    <h3 id="h_01J4JJ26CHA44WRV3GCHGT1D41">
      <strong>Specifications</strong>
    </h3>
    <p>${escapeHtml(specifications)}</p>`;
    })();

    // 🎯 Zendesk 호환 HTML 반환 (zd-html-block 래퍼)
    return `<zd-html-block>
  <script>
    async function copyText(elementId) {
      const element = document.getElementById(elementId);
      if (!element) return;

      var text = element.getAttribute("data-copy-text");
      if (text) {
        try {
          text = decodeURIComponent(text);
        } catch (e) {
          text = element.innerText;
        }
      } else {
        text = element.innerText;
      }

      text = text.replace(/&nbsp;/g, " ");
      text = text.replace(/ /g, " ");

      text = text.replace(/\\u00A0/g, " ");
      text = text.replace(/\\u200B/g, "");
      text = text.replace(/\\r\\n/g, "\\n");

      try {
        await navigator.clipboard.writeText(text);
      } catch (e) {
        const textarea = document.createElement("textarea");
        textarea.value = text;
        document.body.appendChild(textarea);
        textarea.select();
        document.execCommand("copy");
        document.body.removeChild(textarea);
      }
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
                <strong>${escapeHtml(displayedInputUri)}</strong>
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
                <strong>${escapeHtml(displayedActiveMethods)}</strong>
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
        <div id="copyTarget1"${renderZendeskCopyTextAttribute(schemaPayload.copyText, compactCode)} style="${getZendeskCodeStyle(compactCode)}">${currentSchema}</div>
      </div>
    </div>
    <br>
    <br>
${examplesHTML}
    <br><br>
${specificationSectionHTML}
    <br>
    <br>
  </div>
</zd-html-block>`;
  };

  const escapeManualHtml = escapeHtml;

  const formatJsonForPre = (jsonStr: string | object | null | undefined): string => {
    try {
      const parsed = typeof jsonStr === 'string' ? JSON.parse(jsonStr) : jsonStr;
      return escapeManualHtml(JSON.stringify(parsed, null, 2));
    } catch {
      return escapeManualHtml(typeof jsonStr === 'string' ? jsonStr : JSON.stringify(jsonStr ?? {}, null, 2));
    }
  };

  const sanitizeZendeskSpecificMarkup = (html: string): string => {
    return String(html || '')
      .replace(/<zd-html-block>/gi, '')
      .replace(/<\/zd-html-block>/gi, '')
      .replace(/<script[\s\S]*?<\/script>/gi, '')
      .trim();
  };

  const generateConfluenceHTML = (): string => {
    if (!manualData) {
      return '<p>No manual data available. Please send data from Spec, Builder, or Runner tabs.</p>';
    }

    const { inputUri, activeMethods, jsonSchema, requestExamples, responseExamples, specifications } = manualData;
    const displayedActiveMethods = endpoint.method?.trim() || activeMethods?.trim() || '-';
    const displayedInputUri = formatManualInputUri(inputUri);
    const schemaCode = formatJsonForPre(jsonSchema || manualData.jsonSchemaOriginal || '{}');
    const cleanedSpecifications = sanitizeZendeskSpecificMarkup(specifications || '');
    const specificationSection = (() => {
      if (!cleanedSpecifications) {
        return '<h2>Specifications</h2><p>No specifications available.</p>';
      }

      if (cleanedSpecifications.includes('<table')) {
        return `<h2>Specifications</h2>${cleanedSpecifications}`;
      }

      return `<h2>Specifications</h2><p>${escapeManualHtml(cleanedSpecifications)}</p>`;
    })();

    return `
<div class="confluence-manual" style="font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', sans-serif; color: #172b4d; line-height: 1.6;">
  <h1>${escapeManualHtml(manualData.title || endpoint.name || 'API Manual')}</h1>
  <h2>Input URI</h2>
  <p><code>${escapeManualHtml(displayedInputUri)}</code></p>
  <h2>Active Methods</h2>
  <p><strong>${escapeManualHtml(displayedActiveMethods)}</strong></p>
  <h2>JSON Schema</h2>
  <pre style="background: #f4f5f7; padding: 16px; border-radius: 8px; overflow-x: auto;"><code>${schemaCode}</code></pre>
  <h2>Request Examples</h2>
  ${requestExamples && requestExamples.length > 0
        ? requestExamples.map((ex) => `
    <h3>${escapeManualHtml(ex.title)}</h3>
    <pre style="background: #f4f5f7; padding: 16px; border-radius: 8px; overflow-x: auto;"><code>${formatJsonForPre(ex.code)}</code></pre>
  `).join('\n')
        : '<p>No request examples available.</p>'}
  <h2>Response Examples</h2>
  ${responseExamples && responseExamples.length > 0
        ? responseExamples.map((ex) => `
    <h3>${escapeManualHtml(ex.title)}</h3>
    <pre style="background: #f4f5f7; padding: 16px; border-radius: 8px; overflow-x: auto;"><code>${formatJsonForPre(ex.code)}</code></pre>
  `).join('\n')
        : '<p>No response examples available.</p>'}
  ${specificationSection}
</div>`.trim();
  };

  const generateHTML = (): string => {
    return manualPublisher === 'confluence' ? generateConfluenceHTML() : generateZendeskHTML();
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

  const editableHTML = editableHTMLByPublisher[manualPublisher];
  const isHTMLModified = isHTMLModifiedByPublisher[manualPublisher];

  // Prepare Zendesk payload for the 1.0 MB per-locale translation limit.
  const prepareZendeskHTMLForPublish = (): {
    html: string;
    byteSize: number;
    compacted: boolean;
    originalByteSize: number;
  } => {
    const originalHtml = isHTMLModified && editableHTML
      ? editableHTML
      : (manualData?.htmlContent || generateZendeskHTML());
    const originalByteSize = getUtf8ByteSize(originalHtml);
    if (originalByteSize <= ZENDESK_TRANSLATION_BODY_LIMIT_BYTES) {
      return {
        html: originalHtml,
        byteSize: originalByteSize,
        compacted: false,
        originalByteSize,
      };
    }

    const candidates = [
      originalHtml,
      compactZendeskCodeBlocks(originalHtml),
      ...(isHTMLModified && editableHTML ? [] : [generateZendeskHTML({ compactCode: true })]),
    ];
    const best = candidates.reduce(
      (current, html) => {
        const byteSize = getUtf8ByteSize(html);
        return byteSize < current.byteSize ? { html, byteSize } : current;
      },
      { html: originalHtml, byteSize: originalByteSize }
    );

    if (best.byteSize < originalByteSize) {
      return {
        html: best.html,
        byteSize: best.byteSize,
        compacted: true,
        originalByteSize,
      };
    }

    return {
      html: originalHtml,
      byteSize: originalByteSize,
      compacted: false,
      originalByteSize,
    };
  };

  // Export HTML
  const handleExport = () => {
    const html = isHTMLModified && editableHTML ? editableHTML : (manualData?.htmlContent || generateHTML());
    const blob = new Blob([html], { type: 'text/html' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = `${endpoint.name}_${manualPublisher}_manual_${Date.now()}.html`;
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);
    URL.revokeObjectURL(url);
  };

  // 🚀 Send to Zendesk
  const handleSendToZendesk = async () => {
    if (!manualData) {
      toast.error('Manual data is missing. Send content from Spec, Builder, or Runner first.');
      return;
    }

    const zendeskAPI = window.electronAPI?.zendesk;
    const useElectronPublisher = !!zendeskAPI?.publishManualWithEnv;

    const envStatus = zendeskEnvStatus ?? await loadZendeskEnvStatus();
    if (!envStatus?.hasCredentials) {
      const missingMessage = envStatus?.missingFields?.length
        ? envStatus.missingFields.join(', ')
        : 'ZENDESK_SUBDOMAIN/ZENDESK_BASE_URL, ZENDESK_EMAIL, ZENDESK_API_TOKEN';
      toast.error(`.env Zendesk settings are required. ${missingMessage}`);
      return;
    }

    const title = buildZendeskArticleTitle(endpoint, manualData.title);
    const defaultPublishLocale = normalizeZendeskLocale(envStatus.defaultLocale);
    const selectedLocaleSet = new Set<ZendeskLocaleOption>(REQUIRED_ZENDESK_LOCALES);
    for (const option of ZENDESK_LOCALE_OPTIONS) {
      if (zendeskLocales[option]) {
        selectedLocaleSet.add(option);
      }
    }
    const selectedLocales = ZENDESK_LOCALE_OPTIONS
      .filter((option) => selectedLocaleSet.has(option))
      .sort((a, b) => {
        if (a === defaultPublishLocale) return -1;
        if (b === defaultPublishLocale) return 1;
        return 0;
      });
    if (selectedLocales.length === 0) {
      toast.error('Select at least one Zendesk locale.');
      return;
    }

    const zendeskPayload = prepareZendeskHTMLForPublish();
    if (zendeskPayload.byteSize > ZENDESK_TRANSLATION_BODY_LIMIT_BYTES) {
      const compactNote = zendeskPayload.compacted ? ' after HTML compaction' : '';
      toast.error(
        `Zendesk content is ${formatByteSize(zendeskPayload.byteSize)}${compactNote}. ` +
        `The per-locale translation limit is ${formatByteSize(ZENDESK_TRANSLATION_BODY_LIMIT_BYTES)}. ` +
        'Zendesk cannot accept this as a single article body unless the schema/example HTML is reduced.'
      );
      return;
    }

    if (zendeskPayload.compacted) {
      toast.info(
        `Zendesk HTML compacted from ${formatByteSize(zendeskPayload.originalByteSize)} ` +
        `to ${formatByteSize(zendeskPayload.byteSize)} without removing manual content.`
      );
    }
    const html = zendeskPayload.html;

    const labelNames = parseZendeskLabelInput(zendeskLabelInput);
    const commentsDisabled = manualData.zendeskCommentsDisabled ?? true;
    const targetBeforeSend = zendeskUrl.trim();
    let publishTarget = targetBeforeSend;

    updateManualData({ zendeskLabelNames: labelNames });

    try {
      setIsZendeskSending(true);
      const successfulLocales: string[] = [];
      const failedLocales: string[] = [];
      let firstResult: {
        success: boolean;
        data?: {
          mode: 'create' | 'update';
          articleId: string;
          locale: string;
          articleUrl: string;
          associatedAttachmentCount: number;
        };
        error?: string;
      } | null = null;

      for (const locale of selectedLocales) {
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
            publishTarget,
            locale,
            html,
            title || undefined,
            undefined,
            {
              labelNames,
              commentsDisabled,
            }
          );
        } else {
          const response = await fetch(`${MANUAL_SERVER_BASE_URL}/api/zendesk/publish`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({
              targetInput: publishTarget,
              locale,
              body: html,
              title: title || undefined,
              draft: undefined,
              labelNames,
              commentsDisabled,
            }),
          });

          const payload = await response.json().catch(() => null);
          result = payload || { success: false, error: `HTTP ${response.status}` };
        }

        if (!result.success) {
          failedLocales.push(`${locale}: ${result.error || 'Unknown error'}`);
          continue;
        }

        if (!firstResult) {
          firstResult = result;
        }
        if (!publishTarget && result.data?.articleId) {
          publishTarget = result.data.articleUrl || result.data.articleId;
        }
        successfulLocales.push(result.data?.locale || locale);
      }

      if (!firstResult?.success) {
        toast.error(`Zendesk send failed: ${failedLocales.join(' / ') || 'Unknown error'}`);
        return;
      }

      const modeLabel = firstResult.data?.mode === 'create' ? 'create' : 'update';
      const articleId = firstResult.data?.articleId || '(unknown)';
      const attachmentCount = firstResult.data?.associatedAttachmentCount || 0;
      const articleUrl = targetBeforeSend || firstResult.data?.articleUrl;
      if (articleUrl) {
        if (!targetBeforeSend && firstResult.data?.articleUrl) {
          setZendeskUrl(firstResult.data.articleUrl);
        }
        if (firstResult.data?.mode === 'create' || !manualData?.url) {
          updateManualData({ url: articleUrl, articleId: firstResult.data?.articleId });
        }
      }

      if (failedLocales.length > 0) {
        toast.error(`Zendesk partial locale failure: ${failedLocales.join(' / ')}`);
      }
      toast.success(`Zendesk ${modeLabel} complete (Article ${articleId}, ${successfulLocales.join(', ')}, Attachments ${attachmentCount})`);
    } catch (error) {
      toast.error(`Zendesk send failed: ${error instanceof Error ? error.message : 'Unknown error'}`);
    } finally {
      setIsZendeskSending(false);
    }
  };

  const handleSendToConfluence = async () => {
    if (!manualData) {
      toast.error('Manual 데이터가 없습니다. Spec/Builder/Runner에서 먼저 전송하세요.');
      return;
    }

    const envStatus = confluenceEnvStatus ?? await loadConfluenceEnvStatus();
    if (!envStatus?.hasCredentials) {
      const missingMessage = envStatus?.missingFields?.length
        ? envStatus.missingFields.join(', ')
        : 'CONFLUENCE_API_TOKEN';
      toast.error(`.env에 Confluence 설정이 필요합니다: ${missingMessage}`);
      return;
    }

    const html = isHTMLModified && editableHTML ? editableHTML : generateHTML();
    const title = (manualData.title || endpoint.name || '').trim();

    try {
      setIsConfluenceSending(true);
      const response = await fetch(`${MANUAL_SERVER_BASE_URL}/api/confluence/publish`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
          targetInput: confluenceTarget.trim(),
          body: html,
          title: title || undefined,
        }),
      });

      const result = await response.json().catch(() => null);
      const payload = result || { success: false, error: `HTTP ${response.status}` };
      if (!payload.success) {
        toast.error(`Confluence 전송 실패: ${payload.error || 'Unknown error'}`);
        return;
      }

      const modeLabel = payload.data?.mode === 'create' ? '생성' : '업데이트';
      const pageId = payload.data?.pageId || '(unknown)';
      const pageUrl = payload.data?.pageUrl || '';
      if (pageUrl) {
        setConfluenceTarget(pageUrl);
      }

      toast.success(`Confluence ${modeLabel} 완료 (Page ${pageId}, Version ${payload.data?.versionNumber || 1})`);
    } catch (error) {
      toast.error(`Confluence 전송 실패: ${error instanceof Error ? error.message : 'Unknown error'}`);
    } finally {
      setIsConfluenceSending(false);
    }
  };

  // 🎯 Switch to HTML Code mode
  const handleSwitchToCode = () => {
    if (!editableHTML) {
      setEditableHTMLByPublisher((prev) => ({
        ...prev,
        [manualPublisher]: htmlContent,
      }));
    }
    setViewMode('code');
  };

  // 🎯 Handle HTML Change
  const handleHTMLChange = (newHTML: string) => {
    setEditableHTMLByPublisher((prev) => ({
      ...prev,
      [manualPublisher]: newHTML,
    }));
    setIsHTMLModifiedByPublisher((prev) => ({
      ...prev,
      [manualPublisher]: true,
    }));
  };

  // 🎯 Save HTML Changes
  const handleSaveHTML = () => {
    updateManualData({ htmlContent: editableHTML || generateHTML() });
    setIsHTMLModifiedByPublisher((prev) => ({
      ...prev,
      [manualPublisher]: false,
    }));
  };

  // 🎯 Reset HTML
  const handleResetHTML = () => {
    setEditableHTMLByPublisher((prev) => ({
      ...prev,
      [manualPublisher]: '',
    }));
    setIsHTMLModifiedByPublisher((prev) => ({
      ...prev,
      [manualPublisher]: false,
    }));
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
      url: manualData?.url || zendeskUrl.trim(),
      articleId: manualData?.articleId,
      zendeskLabelNames: [],
      zendeskCommentsDisabled: true,
    });
    setZendeskLabelInput('');

    // HTML 에디터도 리셋
    setEditableHTMLByPublisher({
      zendesk: '',
      confluence: '',
    });
    setIsHTMLModifiedByPublisher({
      zendesk: false,
      confluence: false,
    });
  };

  // 🎯 htmlContent: editableHTML이 있으면 그것을 사용, 없으면 generateHTML()
  const htmlContent = editableHTML || manualData?.htmlContent || generateHTML();
  const hasElectronZendeskSender = typeof window !== 'undefined' && !!window.electronAPI?.zendesk?.publishManualWithEnv;
  const hasZendeskSender = typeof window !== 'undefined';
  const zendeskStatusText = zendeskEnvStatus
    ? zendeskEnvStatus.hasCredentials
      ? `${zendeskEnvStatus.subdomain}.zendesk.com (${zendeskEnvStatus.authType === 'token' ? 'Token' : 'Password'}) · Section ${zendeskEnvStatus.defaultSectionId ?? 'N/A'}`
      : `Missing: ${zendeskEnvStatus.missingFields.join(', ')}`
    : hasElectronZendeskSender
      ? 'Loading .env...'
      : 'Server mode (dev:all)';
  const hasConfluenceSender = typeof window !== 'undefined';
  const confluenceStatusText = confluenceEnvStatus
    ? confluenceEnvStatus.hasCredentials
      ? `${confluenceEnvStatus.baseUrl || 'Base URL 없음'} (${confluenceEnvStatus.authType || 'auth'})`
      : `Missing: ${confluenceEnvStatus.missingFields.join(', ')}`
    : 'Loading .env...';
  const currentTargetValue = manualPublisher === 'zendesk' ? zendeskUrl : confluenceTarget;
  const currentTargetSetter = manualPublisher === 'zendesk' ? setZendeskUrl : setConfluenceTarget;
  const currentStatusText = manualPublisher === 'zendesk' ? zendeskStatusText : confluenceStatusText;
  const isSendingCurrent = manualPublisher === 'zendesk' ? isZendeskSending : isConfluenceSending;
  const currentHasSender = manualPublisher === 'zendesk' ? hasZendeskSender : hasConfluenceSender;
  const currentEnvReady = manualPublisher === 'zendesk'
    ? !!zendeskEnvStatus?.hasCredentials
    : !!confluenceEnvStatus?.hasCredentials;
  const handleSendCurrent = () => {
    if (manualPublisher === 'zendesk') {
      void handleSendToZendesk();
      return;
    }

    void handleSendToConfluence();
  };
  const zendeskCommentsEnabled = manualData?.zendeskCommentsDisabled !== undefined
    ? !manualData.zendeskCommentsDisabled
    : false;

  return (
    <div className="flex h-full w-full flex-col bg-zinc-950 relative">
      <div className="border-b border-zinc-700/50 bg-zinc-900/95 px-3 py-3 backdrop-blur-md shadow-lg">
        <div className="flex flex-col gap-3">
          <div className="flex flex-col gap-3 xl:flex-row xl:items-end">
            <div className="min-w-[220px]">
              <div className="text-[10px] uppercase tracking-[0.18em] text-zinc-500 mb-1">Publisher</div>
              <Tabs value={manualPublisher} onValueChange={(value) => setManualPublisher(value as ManualPublisher)}>
                <TabsList className="bg-zinc-800 h-9 p-1 rounded-lg">
                  <TabsTrigger value="zendesk" className="text-xs">Zendesk</TabsTrigger>
                  <TabsTrigger value="confluence" className="text-xs">Confluence</TabsTrigger>
                </TabsList>
              </Tabs>
            </div>

            <div className="flex-1 min-w-0">
              <Label className="text-[10px] uppercase tracking-[0.18em] text-zinc-500 mb-1 block">
                {manualPublisher === 'zendesk' ? 'Zendesk Target' : 'Confluence Target'}
              </Label>
              <div className="flex flex-col gap-2 md:flex-row md:items-center">
                <Input
                  value={currentTargetValue}
                  onChange={(e) => currentTargetSetter(e.target.value)}
                  onBlur={(e) => {
                    if (manualPublisher === 'zendesk' && manualData) {
                      updateManualData({ url: e.target.value.trim() || undefined });
                    }
                  }}
                  placeholder={
                    manualPublisher === 'zendesk'
                      ? 'Article URL/ID (비우면 기본 Section에 신규 생성)'
                      : 'Page URL/ID (비우면 기본 Space에 신규 생성)'
                  }
                  className="bg-zinc-800 border-zinc-700 h-8 text-xs flex-1 min-w-0"
                />
                <span
                  className={`text-[10px] whitespace-nowrap ${
                    currentEnvReady ? 'text-emerald-400' : 'text-amber-400'
                  }`}
                  title={currentStatusText}
                >
                  {currentEnvReady ? 'Env Ready' : 'Env Check'}
                </span>
                {manualPublisher === 'zendesk' && (
                  <Button
                    size="sm"
                    variant="outline"
                    onClick={() => void handleFetchFromZendesk()}
                    disabled={isFetchingZendesk || !zendeskUrl.trim()}
                    title="Zendesk에서 현재 아티클을 불러와 로컬 문서와 비교합니다"
                    className="h-8 px-3 text-xs whitespace-nowrap border-violet-600/60 text-violet-400 hover:bg-violet-900/20 disabled:opacity-50"
                  >
                    {isFetchingZendesk ? (
                      <RefreshCw className="w-3 h-3 mr-1 animate-spin" />
                    ) : (
                      <GitCompare className="w-3 h-3 mr-1" />
                    )}
                    {isFetchingZendesk ? 'Fetching...' : 'Diff'}
                  </Button>
                )}
                <Button
                  size="sm"
                  onClick={handleSendCurrent}
                  disabled={isSendingCurrent || !currentHasSender}
                  title={currentStatusText}
                  className="h-8 px-3 text-xs bg-blue-600 hover:bg-blue-500 whitespace-nowrap disabled:opacity-50 disabled:cursor-not-allowed"
                >
                  <Send className="w-3 h-3 mr-1" />
                  {isSendingCurrent ? 'Sending...' : (zendeskUrl.trim() ? 'Update' : 'Create')}
                </Button>
              </div>

              {manualPublisher === 'zendesk' && manualData && (
                <div className="mt-2 grid gap-2 xl:grid-cols-[220px_minmax(0,1fr)_220px]">
                  <div className="min-w-0">
                    <Label className="text-[10px] uppercase tracking-[0.18em] text-zinc-500 mb-1 block">
                      Locales
                    </Label>
                    <div className="flex h-8 items-center gap-1 rounded-md border border-zinc-700 bg-zinc-800 px-1.5">
                      {ZENDESK_LOCALE_OPTIONS.map((locale) => {
                        const isRequired = isRequiredZendeskLocale(locale);
                        const isActive = isRequired || zendeskLocales[locale];
                        return (
                          <button
                            key={locale}
                            type="button"
                            onClick={() => {
                              if (isRequired) return;
                              setZendeskLocales((prev) => ({
                                ...prev,
                                [locale]: !prev[locale],
                              }));
                            }}
                            title={isRequired ? 'Required locale' : undefined}
                            className={`h-5 rounded px-2 text-[10px] font-medium ${
                              isActive
                                ? 'bg-blue-600 text-white'
                                : 'bg-zinc-900 text-zinc-400 hover:text-zinc-200'
                            }`}
                          >
                            {locale}
                          </button>
                        );
                      })}
                    </div>
                  </div>

                  <div className="min-w-0">
                    <Label className="text-[10px] uppercase tracking-[0.18em] text-zinc-500 mb-1 block">
                      Zendesk Labels
                    </Label>
                    <Input
                      value={zendeskLabelInput}
                      onChange={(e) => setZendeskLabelInput(e.target.value)}
                      onBlur={() => {
                        const parsedLabels = parseZendeskLabelInput(zendeskLabelInput);
                        updateManualData({ zendeskLabelNames: parsedLabels });
                        setZendeskLabelInput(formatZendeskLabelInput(parsedLabels));
                      }}
                      placeholder="e.g. Civil NX, Steel Design"
                      className="bg-zinc-800 border-zinc-700 h-8 text-xs"
                    />
                  </div>

                  <div className="min-w-0">
                    <Label className="text-[10px] uppercase tracking-[0.18em] text-zinc-500 mb-1 block">
                      Comments
                    </Label>
                    <div className="flex h-8 items-center justify-between rounded-md border border-zinc-700 bg-zinc-800 px-3">
                      <span className="text-xs text-zinc-300">
                        {zendeskCommentsEnabled ? 'Enabled' : 'Disabled'}
                      </span>
                      <Switch
                        checked={zendeskCommentsEnabled}
                        onCheckedChange={(checked) => updateManualData({ zendeskCommentsDisabled: !checked })}
                      />
                    </div>
                  </div>
                </div>
              )}
            </div>
          </div>
        </div>
      </div>

      <div className="border-b border-zinc-800/80 bg-zinc-900/90 px-3 py-2">
        <div className="flex flex-wrap items-center gap-3">
          <div className="flex items-center gap-2">
            <h3 className="text-xs font-medium text-white whitespace-nowrap">📖 Manual</h3>
            <div className="h-4 w-px bg-zinc-700" />
          </div>

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
            {manualPublisher === 'zendesk' && zendeskFetchedBody !== null && (
              <Button
                size="sm"
                variant={viewMode === 'diff' ? 'default' : 'outline'}
                onClick={() => setViewMode('diff')}
                className="h-7 px-2 text-xs text-violet-400 border-violet-600/50 data-[state=active]:bg-violet-600"
              >
                <GitCompare className="w-3 h-3 mr-1" />
                Diff
              </Button>
            )}
          </div>

          <div className="h-4 w-px bg-zinc-700" />

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
        </div>
      </div>

      <div className="flex-1 h-full w-full overflow-hidden">
        <div className="flex h-full flex-col overflow-hidden">
          <div className="flex-1 overflow-hidden">
        {viewMode === 'preview' ? (
          <div className="relative h-full">
            {/* Zoom 컨트롤 바 - Floating below header */}
            {manualData && (
              <div className="absolute top-3 left-0 right-0 z-30 flex items-center justify-center gap-3 px-6 pointer-events-none">
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
              className="h-full w-full overflow-hidden p-6 pt-6"
              onWheel={handleWheel}
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
                  className="h-full"
                  style={{
                    cursor: 'default',
                    transform: `scale(${zoom})`,
                    transformOrigin: 'top left',
                    transition: 'transform 0.1s ease-out',
                    width: `${100 / zoom}%`,
                    height: `${100 / zoom}%`
                  }}
                >
                  <iframe
                    srcDoc={htmlContent}
                    className="h-full w-full bg-white rounded-lg border border-zinc-700 pointer-events-auto"
                    title="Manual Preview"
                    sandbox="allow-scripts allow-same-origin"
                  />
                </div>
              )}
            </div>
          </div>
        ) : viewMode === 'diff' ? (
          <ZendeskDiffView
            localHtml={editableHTML || htmlContent}
            remoteHtml={zendeskFetchedBody || ''}
            remoteTitle={zendeskFetchedTitle}
            remoteUpdatedAt={zendeskFetchedUpdatedAt}
            onUseLocal={() => {
              void handleSendToZendesk();
            }}
            onUseRemote={() => {
              setEditableHTMLByPublisher((prev) => ({
                ...prev,
                [manualPublisher]: zendeskFetchedBody || '',
              }));
              setIsHTMLModifiedByPublisher((prev) => ({ ...prev, [manualPublisher]: true }));
              setViewMode('code');
            }}
          />
        ) : (
          <div className="h-full flex flex-col bg-zinc-950 overflow-hidden">
            <div className="p-3 border-b border-zinc-800 bg-zinc-900 flex-shrink-0">
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
      </div>
    </div>
  );
}

// ─── ZendeskDiffView ──────────────────────────────────────────────────────────
interface ZendeskDiffViewProps {
  localHtml: string;
  remoteHtml: string;
  remoteTitle: string | null;
  remoteUpdatedAt: string | null;
  onUseLocal: () => void;
  onUseRemote: () => void;
}

function stripHtmlTags(html: string): string {
  return html
    .replace(/<script[\s\S]*?<\/script>/gi, '')
    .replace(/<style[\s\S]*?<\/style>/gi, '')
    .replace(/<[^>]+>/g, '')
    .replace(/&nbsp;/gi, ' ')
    .replace(/&lt;/gi, '<')
    .replace(/&gt;/gi, '>')
    .replace(/&amp;/gi, '&')
    .replace(/&quot;/gi, '"')
    .replace(/&#039;/gi, "'")
    .replace(/\r\n/g, '\n')
    .replace(/\r/g, '\n')
    .replace(/\n{3,}/g, '\n\n')
    .trim();
}

function computeDiffLines(
  oldText: string,
  newText: string,
): Array<{ type: 'same' | 'removed' | 'added'; text: string }> {
  const oldLines = oldText.split('\n');
  const newLines = newText.split('\n');

  const m = oldLines.length;
  const n = newLines.length;

  const LIMIT = 1000;
  const oldTrunc = oldLines.slice(0, LIMIT);
  const newTrunc = newLines.slice(0, LIMIT);

  const dp: number[][] = Array.from({ length: oldTrunc.length + 1 }, () =>
    new Array(newTrunc.length + 1).fill(0)
  );

  for (let i = oldTrunc.length - 1; i >= 0; i--) {
    for (let j = newTrunc.length - 1; j >= 0; j--) {
      if (oldTrunc[i] === newTrunc[j]) {
        dp[i][j] = dp[i + 1][j + 1] + 1;
      } else {
        dp[i][j] = Math.max(dp[i + 1][j], dp[i][j + 1]);
      }
    }
  }

  const result: Array<{ type: 'same' | 'removed' | 'added'; text: string }> = [];
  let i = 0;
  let j = 0;

  while (i < oldTrunc.length || j < newTrunc.length) {
    if (i < oldTrunc.length && j < newTrunc.length && oldTrunc[i] === newTrunc[j]) {
      result.push({ type: 'same', text: oldTrunc[i] });
      i++;
      j++;
    } else if (
      j < newTrunc.length &&
      (i >= oldTrunc.length || dp[i + 1]?.[j] <= dp[i]?.[j + 1])
    ) {
      result.push({ type: 'added', text: newTrunc[j] });
      j++;
    } else {
      result.push({ type: 'removed', text: oldTrunc[i] });
      i++;
    }
  }

  if (m > LIMIT || n > LIMIT) {
    result.push({ type: 'added', text: `... (truncated, ${m} vs ${n} total lines)` });
  }

  return result;
}

function ZendeskDiffView({ localHtml, remoteHtml, remoteTitle, remoteUpdatedAt, onUseLocal, onUseRemote }: ZendeskDiffViewProps) {
  const localText = stripHtmlTags(localHtml);
  const remoteText = stripHtmlTags(remoteHtml);
  const diffLines = computeDiffLines(remoteText, localText);

  const addedCount = diffLines.filter(l => l.type === 'added').length;
  const removedCount = diffLines.filter(l => l.type === 'removed').length;
  const isIdentical = addedCount === 0 && removedCount === 0;

  return (
    <div className="h-full flex flex-col bg-zinc-950 overflow-hidden">
      {/* Diff header */}
      <div className="flex-shrink-0 px-4 py-3 border-b border-zinc-800 bg-zinc-900 flex flex-wrap items-center justify-between gap-3">
        <div className="flex items-center gap-3">
          <GitCompare className="w-4 h-4 text-violet-400" />
          <span className="text-sm font-medium text-white">Zendesk Diff</span>
          {remoteTitle && (
            <span className="text-xs text-zinc-400 truncate max-w-[200px]" title={remoteTitle}>{remoteTitle}</span>
          )}
          {remoteUpdatedAt && (
            <span className="text-xs text-zinc-500">
              Zendesk 최종 수정: {new Date(remoteUpdatedAt).toLocaleString()}
            </span>
          )}
        </div>
        <div className="flex items-center gap-2">
          {isIdentical ? (
            <span className="text-xs text-emerald-400 bg-emerald-900/30 border border-emerald-600/40 px-2 py-1 rounded">✓ 변경 없음 (동일)</span>
          ) : (
            <span className="text-xs text-zinc-300 bg-zinc-800 border border-zinc-700 px-2 py-1 rounded">
              <span className="text-emerald-400">+{addedCount}</span>
              {' '}
              <span className="text-red-400">-{removedCount}</span>
              {' '}줄
            </span>
          )}
          <Button
            size="sm"
            variant="outline"
            onClick={onUseRemote}
            className="h-7 px-2 text-xs border-amber-600/50 text-amber-400 hover:bg-amber-900/20"
            title="Zendesk 버전을 로컬로 가져오기"
          >
            <ArrowUpToLine className="w-3 h-3 mr-1 rotate-180" />
            Zendesk → Local
          </Button>
          <Button
            size="sm"
            onClick={onUseLocal}
            className="h-7 px-2 text-xs bg-blue-600 hover:bg-blue-500"
            title="로컬 버전을 Zendesk로 업데이트"
          >
            <ArrowUpToLine className="w-3 h-3 mr-1" />
            Local → Zendesk
          </Button>
        </div>
      </div>

      {/* Side-by-side label bar */}
      <div className="flex-shrink-0 grid grid-cols-2 border-b border-zinc-800 bg-zinc-900/60 text-xs">
        <div className="px-4 py-1.5 text-zinc-400 border-r border-zinc-800">
          <span className="text-red-400 font-medium">Zendesk (현재)</span>
        </div>
        <div className="px-4 py-1.5 text-zinc-400">
          <span className="text-emerald-400 font-medium">Local (새로운)</span>
        </div>
      </div>

      {/* Side-by-side diff */}
      <div className="flex-1 overflow-hidden grid grid-cols-2 divide-x divide-zinc-800">
        <ScrollArea className="h-full">
          <div className="font-mono text-xs leading-relaxed p-2 min-w-0">
            {diffLines.map((line, idx) => (
              <div
                key={idx}
                className={
                  line.type === 'removed'
                    ? 'bg-red-900/30 text-red-300 px-2 py-px whitespace-pre-wrap break-all'
                    : line.type === 'added'
                    ? 'text-zinc-700 px-2 py-px select-none'
                    : 'text-zinc-400 px-2 py-px whitespace-pre-wrap break-all'
                }
              >
                {line.type === 'removed' ? `- ${line.text}` : line.type === 'added' ? '\u00a0' : `  ${line.text}`}
              </div>
            ))}
          </div>
        </ScrollArea>
        <ScrollArea className="h-full">
          <div className="font-mono text-xs leading-relaxed p-2 min-w-0">
            {diffLines.map((line, idx) => (
              <div
                key={idx}
                className={
                  line.type === 'added'
                    ? 'bg-emerald-900/30 text-emerald-300 px-2 py-px whitespace-pre-wrap break-all'
                    : line.type === 'removed'
                    ? 'text-zinc-700 px-2 py-px select-none'
                    : 'text-zinc-400 px-2 py-px whitespace-pre-wrap break-all'
                }
              >
                {line.type === 'added' ? `+ ${line.text}` : line.type === 'removed' ? '\u00a0' : `  ${line.text}`}
              </div>
            ))}
          </div>
        </ScrollArea>
      </div>
    </div>
  );
}
