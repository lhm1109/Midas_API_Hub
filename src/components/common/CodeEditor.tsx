import Editor from '@monaco-editor/react';

interface CodeEditorProps {
  value: string;
  onChange: (value: string | undefined) => void;
  language?: 'json' | 'typescript' | 'javascript' | 'html' | 'css' | 'markdown';
  readOnly?: boolean;
  height?: string;
  minimap?: boolean;
  lineNumbers?: boolean;
}

export function CodeEditor({
  value,
  onChange,
  language = 'json',
  readOnly = false,
  height = '100%',
  minimap = false,
  lineNumbers = true,
}: CodeEditorProps) {
  return (
    <Editor
      height={height}
      language={language}
      value={value}
      onChange={onChange}
      theme="vs-dark"
      options={{
        readOnly,
        minimap: { enabled: minimap },
        lineNumbers: lineNumbers ? 'on' : 'off',
        fontSize: 13,
        tabSize: 2,
        scrollBeyondLastLine: false,
        automaticLayout: true,
        wordWrap: 'on',
        folding: true,
        formatOnPaste: true,
        formatOnType: true,
        renderLineHighlight: 'all',
        bracketPairColorization: {
          enabled: true,
        },
        suggest: {
          showWords: true,
          showSnippets: true,
        },
        quickSuggestions: {
          other: true,
          strings: true,
          comments: false,
        },
      }}
    />
  );
}




