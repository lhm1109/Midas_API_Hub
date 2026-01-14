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

interface SettingsModalProps {
  open: boolean;
  onOpenChange: (open: boolean) => void;
  settings: {
    baseUrl: string;
    mapiKey: string;
    commonHeaders: string;
    useAssignWrapper?: boolean;
    schemaDefinition?: 'original' | 'enhanced';
  };
  onSettingsChange: (settings: {
    baseUrl: string;
    mapiKey: string;
    commonHeaders: string;
    useAssignWrapper?: boolean;
    schemaDefinition?: 'original' | 'enhanced';
  }) => void;
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
    onSettingsChange(localSettings);
    onOpenChange(false);
    toast.success('✅ Settings saved successfully!');
  };

  return (
    <Dialog open={open} onOpenChange={onOpenChange}>
      <DialogContent className="max-w-2xl bg-zinc-900 border-zinc-800 text-zinc-100">
        <DialogHeader>
          <DialogTitle className="text-white">Global Environment Settings</DialogTitle>
          <DialogDescription className="text-zinc-400">
            Configure global settings that will be applied to all API requests.
          </DialogDescription>
        </DialogHeader>

        <div className="space-y-6 py-4">
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

          {/* 🔥 NEW: Schema Definition 선택 */}
          <div className="space-y-2">
            <Label htmlFor="schema-definition" className="text-zinc-200">
              Schema Definition (UI Rendering)
            </Label>
            <select
              id="schema-definition"
              value={localSettings.schemaDefinition || 'auto'}
              onChange={(e) => setLocalSettings({ ...localSettings, schemaDefinition: e.target.value === 'auto' ? undefined : e.target.value as 'original' | 'enhanced' })}
              className="w-full px-3 py-2 bg-zinc-800 border border-zinc-700 rounded-md text-white text-sm"
            >
              <option value="auto">Auto (자동 감지)</option>
              <option value="original">Original (기존 스키마)</option>
              <option value="enhanced">Enhanced (인핸스드 스키마)</option>
            </select>
            <p className="text-xs text-zinc-400 mt-1">
              UI 렌더링 규칙을 선택합니다. Auto는 스키마 타입을 자동으로 감지합니다.
            </p>
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
