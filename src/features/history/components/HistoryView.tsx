import { ScrollArea } from '@/components/ui/scroll-area';
import { Clock } from 'lucide-react';

interface HistoryEntry {
  id: string;
  timestamp: string;
  method: string;
  endpoint: string;
  status: number;
  duration: number;
}

export function HistoryView() {
  const historyEntries: HistoryEntry[] = [
    {
      id: '1',
      timestamp: '2026-01-07 14:32:15',
      method: 'POST',
      endpoint: '/civil/db/nlct',
      status: 200,
      duration: 120,
    },
    {
      id: '2',
      timestamp: '2026-01-07 14:28:42',
      method: 'GET',
      endpoint: '/civil/db/node',
      status: 200,
      duration: 85,
    },
    {
      id: '3',
      timestamp: '2026-01-07 14:15:33',
      method: 'POST',
      endpoint: '/civil/db/beam',
      status: 201,
      duration: 156,
    },
    {
      id: '4',
      timestamp: '2026-01-07 13:58:11',
      method: 'DELETE',
      endpoint: '/civil/db/nlct',
      status: 204,
      duration: 92,
    },
    {
      id: '5',
      timestamp: '2026-01-07 13:45:27',
      method: 'PUT',
      endpoint: '/civil/db/node',
      status: 200,
      duration: 134,
    },
  ];

  const getStatusColor = (status: number) => {
    if (status >= 200 && status < 300) return 'text-green-500';
    if (status >= 300 && status < 400) return 'text-blue-500';
    if (status >= 400 && status < 500) return 'text-yellow-500';
    return 'text-red-500';
  };

  const getMethodColor = (method: string) => {
    switch (method) {
      case 'GET':
        return 'bg-green-600';
      case 'POST':
        return 'bg-blue-600';
      case 'PUT':
        return 'bg-orange-600';
      case 'DELETE':
        return 'bg-red-600';
      case 'PATCH':
        return 'bg-purple-600';
      default:
        return 'bg-zinc-600';
    }
  };

  return (
    <div className="flex-1 flex flex-col bg-zinc-950">
      <div className="p-4 border-b border-zinc-800 bg-zinc-900">
        <div className="flex items-center gap-2">
          <Clock className="w-5 h-5" />
          <h2 className="text-lg">Request History</h2>
        </div>
        <p className="text-sm text-zinc-500 mt-1">View all previous API requests and responses</p>
      </div>

      <ScrollArea className="flex-1">
        <div className="p-4">
          <div className="space-y-2">
            {historyEntries.map((entry) => (
              <button
                key={entry.id}
                className="w-full p-4 bg-zinc-900 border border-zinc-800 rounded-lg hover:border-zinc-700 transition-colors text-left"
              >
                <div className="flex items-center justify-between mb-2">
                  <div className="flex items-center gap-3">
                    <span
                      className={`px-2 py-1 text-xs rounded ${getMethodColor(
                        entry.method
                      )} text-white`}
                    >
                      {entry.method}
                    </span>
                    <code className="text-sm text-zinc-300 font-mono">{entry.endpoint}</code>
                  </div>
                  <span className={`text-sm ${getStatusColor(entry.status)}`}>
                    {entry.status}
                  </span>
                </div>
                <div className="flex items-center justify-between text-xs text-zinc-500">
                  <span>{entry.timestamp}</span>
                  <span>{entry.duration}ms</span>
                </div>
              </button>
            ))}
          </div>
        </div>
      </ScrollArea>
    </div>
  );
}
