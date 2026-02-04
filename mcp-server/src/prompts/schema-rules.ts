/**
 * MCP Prompts - Load rules from YAML files
 * Rules are defined in mcp.yaml aiPrompts section (SSOT)
 */

import * as fs from 'fs';
import * as path from 'path';
import * as yaml from 'yaml';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

export interface MCPPrompt {
    name: string;
    description: string;
    arguments?: Array<{
        name: string;
        description: string;
        required?: boolean;
    }>;
}

export interface PromptMessage {
    role: 'user' | 'assistant';
    content: {
        type: 'text';
        text: string;
    };
}

// YAML rules file path
const MCP_YAML_PATH = path.resolve(
    __dirname,
    '../../../schema_definitions/civil_gen_definition/enhanced/mcp.yaml'
);

/**
 * Load aiPrompts section from mcp.yaml
 */
function loadPromptRules(): Record<string, unknown> | null {
    try {
        const content = fs.readFileSync(MCP_YAML_PATH, 'utf-8');
        const parsed = yaml.parse(content) as { aiPrompts?: Record<string, unknown> };
        return parsed.aiPrompts || null;
    } catch (error) {
        console.error('Failed to load aiPrompts from mcp.yaml:', error);
        return null;
    }
}

/**
 * Schema generation rules prompt definition
 */
export const schemaRulesPrompt: MCPPrompt = {
    name: 'schema_rules',
    description: 'Query schema generation rules. Loaded from mcp.yaml aiPrompts section.',
    arguments: [
        {
            name: 'section',
            description: 'Section to query (all, conditional, ui, validation, questions)',
            required: false,
        },
    ],
};

/**
 * Generate messages from YAML rules
 */
export function getSchemaRulesMessages(section?: string): PromptMessage[] {
    const aiPrompts = loadPromptRules();
    if (!aiPrompts) {
        return [{
            role: 'assistant',
            content: {
                type: 'text',
                text: '❌ Failed to load aiPrompts section from mcp.yaml.',
            },
        }];
    }

    const schemaRules = aiPrompts['schema_rules'] as { sections?: Record<string, string> } | undefined;
    if (!schemaRules?.sections) {
        return [{
            role: 'assistant',
            content: {
                type: 'text',
                text: '❌ aiPrompts.schema_rules.sections not found in mcp.yaml.',
            },
        }];
    }

    const sections = schemaRules.sections;
    let messages: PromptMessage[] = [];

    if (section && section !== 'all') {
        // Return specific section
        const text = sections[section];
        if (text) {
            messages = [{
                role: 'assistant',
                content: { type: 'text', text },
            }];
        } else {
            messages = [{
                role: 'assistant',
                content: {
                    type: 'text',
                    text: `❌ Section '${section}' not found. Available: ${Object.keys(sections).join(', ')}`,
                },
            }];
        }
    } else {
        // Return all sections
        for (const [, text] of Object.entries(sections)) {
            messages.push({
                role: 'assistant',
                content: { type: 'text', text },
            });
        }
    }

    return messages;
}

/**
 * Prompt list
 */
export const allPrompts: MCPPrompt[] = [schemaRulesPrompt];
