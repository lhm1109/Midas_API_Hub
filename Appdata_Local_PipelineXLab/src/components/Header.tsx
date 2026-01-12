import { Bell, User } from 'lucide-react';

export default function Header() {
    return (
        <header className="h-12 bg-surface-1 border-b border-surface-2 flex items-center justify-end px-4">
            {/* Right Actions */}
            <div className="flex items-center gap-2">
                <button className="p-1.5 text-muted-foreground hover:text-foreground hover:bg-accent rounded transition-colors">
                    <Bell className="w-3.5 h-3.5" />
                </button>
                <button className="p-1.5 text-muted-foreground hover:text-foreground hover:bg-accent rounded transition-colors">
                    <User className="w-3.5 h-3.5" />
                </button>
            </div>
        </header>
    );
}
