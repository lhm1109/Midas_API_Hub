import {
  Dialog,
  DialogContent,
  DialogDescription,
  DialogHeader,
  DialogTitle,
} from './ui/dialog';
import { Label } from './ui/label';
import { Input } from './ui/input';
import { Button } from './ui/button';
import { Textarea } from './ui/textarea';

interface SettingsModalProps {
  open: boolean;
  onOpenChange: (open: boolean) => void;
  settings: {
    baseUrl: string;
    mapiKey: string;
    commonHeaders: string;
  };
  onSettingsChange: (settings: {
    baseUrl: string;
    mapiKey: string;
    commonHeaders: string;
  }) => void;
}

export function SettingsModal({
  open,
  onOpenChange,
  settings,
  onSettingsChange,
}: SettingsModalProps) {
  const handleSave = () => {
    onOpenChange(false);
  };

  return (
    <Dialog open={open} onOpenChange={onOpenChange}>
      <DialogContent className="max-w-2xl bg-zinc-900 border-zinc-800">
        <DialogHeader>
          <DialogTitle>Global Environment Settings</DialogTitle>
          <DialogDescription>
            Configure global settings that will be applied to all API requests.
          </DialogDescription>
        </DialogHeader>

        <div className="space-y-6 py-4">
          {/* Base URL */}
          <div className="space-y-2">
            <Label htmlFor="baseUrl">Base URL</Label>
            <Input
              id="baseUrl"
              placeholder="https://api-beta.midasit.com/civil"
              value={settings.baseUrl}
              onChange={(e) =>
                onSettingsChange({ ...settings, baseUrl: e.target.value })
              }
              className="bg-zinc-800 border-zinc-700"
            />
            <p className="text-xs text-zinc-500">
              The base URL for all API requests. This will be prepended to endpoint paths.
            </p>
          </div>

          {/* MAPI-Key */}
          <div className="space-y-2">
            <Label htmlFor="mapiKey">MAPI-Key</Label>
            <Input
              id="mapiKey"
              type="password"
              placeholder="Enter your MAPI-Key"
              value={settings.mapiKey}
              onChange={(e) =>
                onSettingsChange({ ...settings, mapiKey: e.target.value })
              }
              className="bg-zinc-800 border-zinc-700"
            />
            <p className="text-xs text-zinc-500">
              Your authentication key. This will be automatically included in all requests.
            </p>
          </div>

          {/* Common Headers */}
          <div className="space-y-2">
            <Label htmlFor="commonHeaders">Common Headers (JSON)</Label>
            <Textarea
              id="commonHeaders"
              placeholder={`{\n  "Content-Type": "application/json",\n  "Accept": "application/json"\n}`}
              value={settings.commonHeaders}
              onChange={(e) =>
                onSettingsChange({ ...settings, commonHeaders: e.target.value })
              }
              className="bg-zinc-800 border-zinc-700 font-mono text-sm min-h-[120px]"
            />
            <p className="text-xs text-zinc-500">
              Additional headers in JSON format. These will be included in all API requests.
            </p>
          </div>
        </div>

        <div className="flex justify-end gap-2">
          <Button
            variant="outline"
            onClick={() => onOpenChange(false)}
            className="border-zinc-700"
          >
            Cancel
          </Button>
          <Button onClick={handleSave} className="bg-blue-600 hover:bg-blue-700">
            Save Settings
          </Button>
        </div>
      </DialogContent>
    </Dialog>
  );
}
