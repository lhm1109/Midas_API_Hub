import { useState, useEffect } from 'react';
import {
  Dialog,
  DialogContent,
  DialogDescription,
  DialogHeader,
  DialogTitle,
} from '@/components/ui/dialog';
import { Label } from '@/components/ui/label';
import { Input } from '@/components/ui/input';
import { Button } from '@/components/ui/button';
import { Textarea } from '@/components/ui/textarea';
import { Checkbox } from '@/components/ui/checkbox';
import { toast } from 'sonner';
import {
  DEFAULT_MANUAL_HUB_INDEX_LOCAL_PATH_EN,
  DEFAULT_MANUAL_HUB_INDEX_LOCAL_PATH_KO,
} from '@/config/constants';

interface SettingsModalProps {
  open: boolean;
  onOpenChange: (open: boolean) => void;
  settings: {
    baseUrl: string;
    mapiKey: string;
    commonHeaders: string;
    useAssignWrapper?: boolean;
    schemaMode?: 'enhanced' | 'normal';
    userName?: string;
    supabaseUrl?: string;
    supabaseServiceKey?: string;
    supabaseDbPassword?: string;
    manualHubIndexLocalPathKo?: string;
    manualHubIndexLocalPathEn?: string;
  };
  onSettingsChange: (settings: SettingsModalProps['settings']) => void;
}

export function SettingsModal({
  open,
  onOpenChange,
  settings,
  onSettingsChange,
}: SettingsModalProps) {
  const [localSettings, setLocalSettings] = useState(settings);

  useEffect(() => {
    setLocalSettings(settings);
  }, [settings]);

  const handleSave = () => {
    console.log('🔥 SettingsModal saving:', localSettings);
    console.log('🔥 supabaseDbPassword:', localSettings.supabaseDbPassword);
    onSettingsChange(localSettings);
    onOpenChange(false);
    toast.success('✅ Settings saved successfully!');
  };

  return (
    <Dialog open={open} onOpenChange={onOpenChange}>
      <DialogContent className="max-w-2xl max-h-[85vh] overflow-y-auto bg-zinc-900 border-zinc-800 text-zinc-100">
        <DialogHeader>
          <DialogTitle className="text-white">Global Environment Settings</DialogTitle>
          <DialogDescription className="text-zinc-400">
            Configure global settings that will be applied to all API requests.
          </DialogDescription>
        </DialogHeader>

        <div className="space-y-6 py-4">
          {/* 🔥 사용자 이름 */}
          <div className="space-y-2">
            <Label htmlFor="userName" className="text-zinc-200">사용자 이름</Label>
            <Input
              id="userName"
              placeholder="Your Name"
              value={localSettings.userName || ''}
              onChange={(e) =>
                setLocalSettings({ ...localSettings, userName: e.target.value })
              }
              className="bg-zinc-800 border-zinc-700 text-white"
            />
            <p className="text-xs text-zinc-400">
              편집 잠금 시스템에서 표시될 사용자 이름입니다. 다른 사용자에게 표시됩니다.
            </p>
          </div>

          {/* Base URL */}
          <div className="space-y-2">
            <Label htmlFor="baseUrl" className="text-zinc-200">Base URL</Label>
            <Input
              id="baseUrl"
              placeholder="https://api-beta.midasit.com/civil"
              value={settings.baseUrl}
              onChange={(e) =>
                onSettingsChange({ ...settings, baseUrl: e.target.value })
              }
              className="bg-zinc-800 border-zinc-700 text-white"
            />
            <p className="text-xs text-zinc-400">
              The base URL for all API requests. This will be prepended to endpoint paths.
            </p>
          </div>

          {/* MAPI-Key */}
          <div className="space-y-2">
            <Label htmlFor="mapiKey" className="text-zinc-200">MAPI-Key</Label>
            <Input
              id="mapiKey"
              type="password"
              placeholder="Enter your MAPI-Key"
              value={settings.mapiKey}
              onChange={(e) =>
                onSettingsChange({ ...settings, mapiKey: e.target.value })
              }
              className="bg-zinc-800 border-zinc-700 text-white"
            />
            <p className="text-xs text-zinc-400">
              Your authentication key. This will be automatically included in all requests.
            </p>
          </div>

          {/* Common Headers */}
          <div className="space-y-2">
            <Label htmlFor="commonHeaders" className="text-zinc-200">Common Headers (JSON)</Label>
            <Textarea
              id="commonHeaders"
              placeholder={`{\n  "Content-Type": "application/json",\n  "Accept": "application/json"\n}`}
              value={settings.commonHeaders}
              onChange={(e) =>
                onSettingsChange({ ...settings, commonHeaders: e.target.value })
              }
              className="bg-zinc-800 border-zinc-700 font-mono text-sm min-h-[120px] text-white"
            />
            <p className="text-xs text-zinc-400">
              Additional headers in JSON format. These will be included in all API requests.
            </p>
          </div>

          {/* Use Assign Wrapper */}
          <div className="space-y-2">
            <div className="flex items-center space-x-2">
              <Checkbox
                id="useAssignWrapper"
                checked={localSettings.useAssignWrapper !== false}
                onCheckedChange={(checked) =>
                  setLocalSettings({ ...localSettings, useAssignWrapper: checked === true })
                }
                className="border-zinc-700"
              />
              <Label htmlFor="useAssignWrapper" className="text-zinc-200 cursor-pointer">
                Use Assign Wrapper
              </Label>
            </div>
            <p className="text-xs text-zinc-400 ml-7">
              Wrap request body with {"{"}"Assign": {"{"}"1": ...{"}"}{"}"} structure.
              This is the default format for MIDAS API endpoints.
            </p>
          </div>

          {/* 🔥 Schema Mode 선택 */}
          <div className="space-y-2">
            <Label htmlFor="schema-mode" className="text-zinc-200">
              스키마 모드
            </Label>
            <select
              id="schema-mode"
              value={localSettings.schemaMode || 'enhanced'}
              onChange={(e) => setLocalSettings({ ...localSettings, schemaMode: e.target.value as 'enhanced' | 'normal' })}
              className="w-full px-3 py-2 bg-zinc-800 border border-zinc-700 rounded-md text-white text-sm"
            >
              <option value="enhanced">개선 모드 (Original/Enhanced 2탭)</option>
              <option value="normal">일반 모드 (단일 스키마)</option>
            </select>
            <div className="mt-2 p-3 bg-zinc-800/50 rounded-md border border-zinc-700">
              <p className="text-xs text-zinc-300 mb-2">
                <strong className="text-blue-400">개선 모드:</strong>
              </p>
              <ul className="text-xs text-zinc-400 space-y-1 ml-4 list-disc">
                <li>Original/Enhanced 2개 탭 표시</li>
                <li>YAML 파일 2개 로드 (original.yaml, enhanced.yaml)</li>
                <li>스키마 간 전환 및 비교 가능</li>
              </ul>

              <p className="text-xs text-zinc-300 mb-2 mt-3">
                <strong className="text-green-400">일반 모드:</strong>
              </p>
              <ul className="text-xs text-zinc-400 space-y-1 ml-4 list-disc">
                <li>단일 스키마만 표시 (탭 구분 없음)</li>
                <li>YAML 파일 1개만 로드 (definition.yaml)</li>
                <li>심플한 UI로 빠른 작업</li>
              </ul>
            </div>
          </div>

          {/* Manual Hub — Site 인덱스 기본 로컬 경로 */}
          <div className="space-y-4 pt-4 border-t border-zinc-700">
            <h3 className="text-sm font-semibold text-amber-400">Manual Hub (Index)</h3>
            <p className="text-xs text-zinc-400">
              Manual Hub의 Site 인덱스(KO/EN)에서 로컬 파일 경로가 비어 있을 때 사용할 기본 절대 경로입니다.
            </p>
            <div className="space-y-2">
              <Label htmlFor="manualHubIndexLocalPathKo" className="text-zinc-200">
                Index KO — 기본 로컬 경로
              </Label>
              <Input
                id="manualHubIndexLocalPathKo"
                placeholder={DEFAULT_MANUAL_HUB_INDEX_LOCAL_PATH_KO}
                value={localSettings.manualHubIndexLocalPathKo ?? ''}
                onChange={(e) =>
                  setLocalSettings({ ...localSettings, manualHubIndexLocalPathKo: e.target.value })
                }
                className="bg-zinc-800 border-zinc-700 text-white font-mono text-xs"
              />
            </div>
            <div className="space-y-2">
              <Label htmlFor="manualHubIndexLocalPathEn" className="text-zinc-200">
                Index EN — 기본 로컬 경로
              </Label>
              <Input
                id="manualHubIndexLocalPathEn"
                placeholder={DEFAULT_MANUAL_HUB_INDEX_LOCAL_PATH_EN}
                value={localSettings.manualHubIndexLocalPathEn ?? ''}
                onChange={(e) =>
                  setLocalSettings({ ...localSettings, manualHubIndexLocalPathEn: e.target.value })
                }
                className="bg-zinc-800 border-zinc-700 text-white font-mono text-xs"
              />
            </div>
          </div>

          {/* 🔥 Supabase 설정 */}
          <div className="space-y-4 pt-4 border-t border-zinc-700">
            <h3 className="text-sm font-semibold text-green-400">Supabase Database</h3>

            <div className="space-y-2">
              <Label htmlFor="supabaseUrl" className="text-zinc-200">Supabase URL</Label>
              <Input
                id="supabaseUrl"
                placeholder="https://xxx.supabase.co"
                value={localSettings.supabaseUrl || ''}
                onChange={(e) =>
                  setLocalSettings({ ...localSettings, supabaseUrl: e.target.value })
                }
                className="bg-zinc-800 border-zinc-700 text-white"
              />
              <p className="text-xs text-zinc-400">
                Supabase 프로젝트 URL (프로젝트 설정에서 확인)
              </p>
            </div>

            <div className="space-y-2">
              <Label htmlFor="supabaseServiceKey" className="text-zinc-200">Service Role Key</Label>
              <Input
                id="supabaseServiceKey"
                type="password"
                placeholder="eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9..."
                value={localSettings.supabaseServiceKey || ''}
                onChange={(e) =>
                  setLocalSettings({ ...localSettings, supabaseServiceKey: e.target.value })
                }
                className="bg-zinc-800 border-zinc-700 text-white"
              />
              <p className="text-xs text-zinc-400">
                Service Role Key (API 인증용) - API Settings에서 확인
              </p>
            </div>

            <div className="space-y-2">
              <Label htmlFor="supabaseDbPassword" className="text-zinc-200">Database Password ⚠️</Label>
              <Input
                id="supabaseDbPassword"
                type="password"
                placeholder="프로젝트 생성 시 설정한 DB 비밀번호"
                value={localSettings.supabaseDbPassword || ''}
                onChange={(e) => {
                  console.log('🔥 Input onChange:', e.target.value);
                  setLocalSettings({ ...localSettings, supabaseDbPassword: e.target.value });
                }}
                className="bg-zinc-800 border-zinc-700 text-white"
              />
              <p className="text-xs text-zinc-400">
                ⚠️ 마이그레이션 실행에 필요! Project Settings → Database → Connection string에서 확인
              </p>
            </div>
          </div>
        </div>

        <div className="flex justify-end gap-2">
          <Button
            variant="outline"
            onClick={() => onOpenChange(false)}
            className="border-zinc-700 text-zinc-200 hover:bg-zinc-800"
          >
            Cancel
          </Button>
          <Button onClick={handleSave} className="bg-blue-600 hover:bg-blue-700 text-white">
            Save Settings
          </Button>
        </div>
      </DialogContent>
    </Dialog>
  );
}
