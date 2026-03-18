/**
 * TerminalEmulator - xterm.js 기반 터미널 렌더링
 */

import React, { useEffect, useRef } from 'react';
import { Terminal } from '@xterm/xterm';
import { FitAddon } from '@xterm/addon-fit';
import '@xterm/xterm/css/xterm.css';
import { terminalAPI } from './types';

interface TerminalEmulatorProps {
    terminalId: number;
    className?: string;
}

export const TerminalEmulator: React.FC<TerminalEmulatorProps> = ({
    terminalId,
    className = ''
}) => {
    const containerRef = useRef<HTMLDivElement>(null);
    const terminalRef = useRef<Terminal | null>(null);
    const fitAddonRef = useRef<FitAddon | null>(null);

    useEffect(() => {
        if (!containerRef.current) return;

        // Create terminal instance
        const terminal = new Terminal({
            theme: {
                background: '#1a1a2e',
                foreground: '#eaeaea',
                cursor: '#00d9ff',
                cursorAccent: '#1a1a2e',
                selectionBackground: '#44475a',
                black: '#21222c',
                red: '#ff5555',
                green: '#50fa7b',
                yellow: '#f1fa8c',
                blue: '#6272a4',
                magenta: '#ff79c6',
                cyan: '#8be9fd',
                white: '#f8f8f2',
            },
            fontSize: 13,
            fontFamily: 'Consolas, "Courier New", monospace',
            cursorBlink: true,
            scrollback: 5000,
            // 🔥 복사/붙여넣기 관련 설정
            rightClickSelectsWord: true,       // 우클릭으로 단어 선택
            allowProposedApi: true,            // 추가 API 허용
        });

        const fitAddon = new FitAddon();
        terminal.loadAddon(fitAddon);
        terminal.open(containerRef.current);

        // Fit terminal to container
        setTimeout(() => fitAddon.fit(), 0);

        terminalRef.current = terminal;
        fitAddonRef.current = fitAddon;

        // Keep custom copy shortcuts, but let xterm handle paste natively.
        // This preserves bracketed paste support and avoids double-pasting.
        terminal.attachCustomKeyEventHandler((event) => {
            // Ctrl+Shift+C: 복사
            if (event.ctrlKey && event.shiftKey && event.key === 'C') {
                const selection = terminal.getSelection();
                if (selection) {
                    navigator.clipboard.writeText(selection);
                }
                return false;  // 이벤트를 터미널로 전달하지 않음
            }

            // Ctrl+C: 선택된 텍스트가 있으면 복사, 없으면 SIGINT로 전달
            if (event.ctrlKey && !event.shiftKey && event.key === 'c') {
                const selection = terminal.getSelection();
                if (selection) {
                    navigator.clipboard.writeText(selection);
                    return false;
                }
                // 선택이 없으면 터미널로 전달 (SIGINT)
                return true;
            }

            return true;  // 다른 키는 터미널로 전달
        });

        const api = terminalAPI();
        if (api) {
            // Handle terminal input
            terminal.onData((data) => {
                api.write(terminalId, data);
            });

            // Load existing logs
            api.getLogs(terminalId).then((result) => {
                if (result.success && result.data) {
                    terminal.write(result.data);
                }
            });
        }

        // Handle resize
        const handleResize = () => {
            if (fitAddonRef.current && terminalRef.current) {
                fitAddonRef.current.fit();
                const api = terminalAPI();
                if (api) {
                    const dims = fitAddonRef.current.proposeDimensions();
                    if (dims) {
                        api.resize(terminalId, dims.cols, dims.rows);
                    }
                }
            }
        };

        window.addEventListener('resize', handleResize);

        return () => {
            window.removeEventListener('resize', handleResize);
            terminal.dispose();
        };
    }, [terminalId]);

    // Handle incoming data
    useEffect(() => {
        const api = terminalAPI();
        if (!api) return;

        const handleData = (payload: { id: number; data: string }) => {
            if (payload.id === terminalId && terminalRef.current) {
                terminalRef.current.write(payload.data);
            }
        };

        api.onData(handleData);

        return () => {
            api.removeListeners();
        };
    }, [terminalId]);

    return (
        <div
            ref={containerRef}
            className={`h-full w-full min-h-[200px] ${className}`}
        />
    );
};

export default TerminalEmulator;
