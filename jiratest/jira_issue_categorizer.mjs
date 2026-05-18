#!/usr/bin/env node

import fs from 'node:fs';
import path from 'node:path';
import process from 'node:process';
import { fileURLToPath } from 'node:url';
import dotenv from 'dotenv';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

dotenv.config({ path: path.join(process.cwd(), '.env'), quiet: true });
dotenv.config({ path: path.join(__dirname, '.env'), override: true, quiet: true });

const FIELD_ALIASES = {
  assignee: 'assignee',
  component: 'components',
  components: 'components',
  epic: 'epic',
  fixversion: 'fixVersions',
  fixversions: 'fixVersions',
  issuetype: 'issuetype',
  issueType: 'issuetype',
  label: 'labels',
  labels: 'labels',
  parent: 'parent',
  priority: 'priority',
  reporter: 'reporter',
  status: 'status',
};

function parseArgs(argv) {
  const args = {};

  for (let index = 0; index < argv.length; index += 1) {
    const arg = argv[index];
    if (arg === '--help' || arg === '-h') {
      args.help = true;
      continue;
    }

    if (!arg.startsWith('--')) {
      throw new Error(`Unknown argument: ${arg}`);
    }

    const raw = arg.slice(2);
    const equalsIndex = raw.indexOf('=');
    const rawKey = equalsIndex === -1 ? raw : raw.slice(0, equalsIndex);
    const inlineValue = equalsIndex === -1 ? undefined : raw.slice(equalsIndex + 1);
    const key = rawKey.replace(/-([a-z])/g, (_, char) => char.toUpperCase());
    let value = inlineValue;
    if (value === undefined) {
      value = argv[index + 1];
      index += 1;
    }

    if (!value || value.startsWith('--')) {
      throw new Error(`Missing value for --${rawKey}`);
    }

    args[key] = value;
  }

  return args;
}

function usage() {
  return `
Usage:
  node jira_issue_categorizer.mjs --project KEY --by components
  node jira_issue_categorizer.mjs --jql "project = KEY AND statusCategory != Done" --by labels

Required environment:
  JIRA_BASE_URL      https://your-site.atlassian.net
  JIRA_API_TOKEN     Jira API token or PAT

Cloud basic auth also requires:
  JIRA_EMAIL         Atlassian account email

Optional:
  JIRA_AUTH_MODE     basic | bearer (default: basic)
  JIRA_PROJECT_KEY   Used when --project and --jql are omitted
  JIRA_CATEGORY_FIELD Category field, default: components
  JIRA_MAX_RESULTS   Total issue cap, default: 1000
  JIRA_OUTPUT_DIR    Output folder, default: output
`.trim();
}

function requireConfig(config) {
  const missing = [];
  if (!config.baseUrl) missing.push('JIRA_BASE_URL');
  if (!config.token) missing.push('JIRA_API_TOKEN');
  if (config.authMode === 'basic' && !config.email) missing.push('JIRA_EMAIL');
  if (!config.jql) missing.push('JIRA_PROJECT_KEY or --project or --jql');

  if (missing.length > 0) {
    throw new Error(`Missing configuration: ${missing.join(', ')}`);
  }
}

function normalizeBaseUrl(baseUrl) {
  return baseUrl.replace(/\/+$/, '');
}

function buildAuthHeader(config) {
  if (config.authMode === 'bearer') {
    return `Bearer ${config.token}`;
  }

  const raw = `${config.email}:${config.token}`;
  return `Basic ${Buffer.from(raw, 'utf8').toString('base64')}`;
}

function resolveField(fieldName) {
  return FIELD_ALIASES[fieldName] ?? fieldName;
}

function fieldList(categoryField) {
  const selected = resolveField(categoryField);
  const fields = new Set([
    'summary',
    'status',
    'issuetype',
    'priority',
    'assignee',
    'reporter',
    'labels',
    'components',
    'fixVersions',
    'parent',
    selected,
  ]);

  return [...fields];
}

async function jiraRequest(config, urlPath, body) {
  const response = await fetch(`${normalizeBaseUrl(config.baseUrl)}${urlPath}`, {
    method: 'POST',
    headers: {
      Accept: 'application/json',
      Authorization: buildAuthHeader(config),
      'Content-Type': 'application/json',
    },
    body: JSON.stringify(body),
  });

  const text = await response.text();
  const payload = text ? JSON.parse(text) : {};

  if (!response.ok) {
    const message = payload.errorMessages?.join('; ') || payload.message || response.statusText;
    throw new Error(`Jira API failed (${response.status}): ${message}`);
  }

  return payload;
}

async function fetchIssues(config) {
  const issues = [];
  let nextPageToken;

  do {
    const remaining = config.maxResults - issues.length;
    const pageSize = Math.min(100, remaining);
    const payload = await jiraRequest(config, '/rest/api/3/search/jql', {
      fields: fieldList(config.categoryField),
      jql: config.jql,
      maxResults: pageSize,
      nextPageToken,
    });

    issues.push(...(payload.issues ?? []));
    nextPageToken = payload.nextPageToken;
  } while (nextPageToken && issues.length < config.maxResults);

  return issues;
}

function displayName(value) {
  if (value === null || value === undefined || value === '') return '(uncategorized)';
  if (typeof value === 'string' || typeof value === 'number' || typeof value === 'boolean') {
    return String(value);
  }
  if (Array.isArray(value)) {
    return value.length === 0 ? '(uncategorized)' : value.map(displayName);
  }
  return value.name || value.displayName || value.key || value.value || value.id || JSON.stringify(value);
}

function categoryNames(issue, categoryField) {
  const field = resolveField(categoryField);
  const value = issue.fields?.[field];
  const names = displayName(value);
  return Array.isArray(names) ? names : [names];
}

function summarizeIssue(issue, categoryField) {
  const fields = issue.fields ?? {};
  return {
    key: issue.key,
    summary: fields.summary ?? '',
    status: displayName(fields.status),
    issueType: displayName(fields.issuetype),
    priority: displayName(fields.priority),
    assignee: displayName(fields.assignee),
    category: categoryNames(issue, categoryField).join(', '),
  };
}

function groupIssues(issues, categoryField) {
  const groups = new Map();

  for (const issue of issues) {
    for (const name of categoryNames(issue, categoryField)) {
      if (!groups.has(name)) groups.set(name, []);
      groups.get(name).push(summarizeIssue(issue, categoryField));
    }
  }

  return [...groups.entries()].sort(([left], [right]) => left.localeCompare(right));
}

function csvEscape(value) {
  const text = String(value ?? '');
  return /[",\r\n]/.test(text) ? `"${text.replaceAll('"', '""')}"` : text;
}

function writeReports(config, issues, groups) {
  fs.mkdirSync(config.outputDir, { recursive: true });

  const rawPath = path.join(config.outputDir, 'issues_raw.json');
  fs.writeFileSync(rawPath, `${JSON.stringify(issues, null, 2)}\n`, 'utf8');

  const groupedJsonPath = path.join(config.outputDir, 'issues_by_category.json');
  fs.writeFileSync(
    groupedJsonPath,
    `${JSON.stringify(Object.fromEntries(groups), null, 2)}\n`,
    'utf8',
  );

  const markdown = [
    `# Jira Issues by ${config.categoryField}`,
    '',
    `JQL: \`${config.jql}\``,
    `Fetched issues: ${issues.length}`,
    '',
    ...groups.flatMap(([category, items]) => [
      `## ${category} (${items.length})`,
      '',
      '| Key | Type | Status | Priority | Assignee | Summary |',
      '| --- | --- | --- | --- | --- | --- |',
      ...items.map((item) => (
        `| ${item.key} | ${item.issueType} | ${item.status} | ${item.priority} | ${item.assignee} | ${item.summary.replaceAll('|', '\\|')} |`
      )),
      '',
    ]),
  ].join('\n');

  const markdownPath = path.join(config.outputDir, 'issues_by_category.md');
  fs.writeFileSync(markdownPath, `${markdown}\n`, 'utf8');

  const csvRows = [
    ['category', 'key', 'issue_type', 'status', 'priority', 'assignee', 'summary'],
    ...groups.flatMap(([category, items]) => (
      items.map((item) => [
        category,
        item.key,
        item.issueType,
        item.status,
        item.priority,
        item.assignee,
        item.summary,
      ])
    )),
  ];
  const csvPath = path.join(config.outputDir, 'issues_by_category.csv');
  fs.writeFileSync(csvPath, `${csvRows.map((row) => row.map(csvEscape).join(',')).join('\n')}\n`, 'utf8');

  return { rawPath, groupedJsonPath, markdownPath, csvPath };
}

async function main() {
  const args = parseArgs(process.argv.slice(2));
  if (args.help) {
    console.log(usage());
    return;
  }

  const projectKey = args.project ?? process.env.JIRA_PROJECT_KEY;
  const jql = args.jql ?? process.env.JIRA_JQL ?? (projectKey ? `project = ${projectKey} ORDER BY created DESC` : '');
  const config = {
    authMode: (args.authMode ?? process.env.JIRA_AUTH_MODE ?? 'basic').toLowerCase(),
    baseUrl: args.baseUrl ?? process.env.JIRA_BASE_URL,
    categoryField: args.by ?? process.env.JIRA_CATEGORY_FIELD ?? 'components',
    email: args.email ?? process.env.JIRA_EMAIL,
    jql,
    maxResults: Number(args.maxResults ?? process.env.JIRA_MAX_RESULTS ?? 1000),
    outputDir: path.resolve(__dirname, args.outputDir ?? process.env.JIRA_OUTPUT_DIR ?? 'output'),
    token: args.token ?? process.env.JIRA_API_TOKEN,
  };

  requireConfig(config);

  console.log(`Fetching Jira issues: ${config.jql}`);
  const issues = await fetchIssues(config);
  const groups = groupIssues(issues, config.categoryField);
  const paths = writeReports(config, issues, groups);

  console.log(`Fetched ${issues.length} issues into ${groups.length} categories.`);
  console.log(`Markdown: ${paths.markdownPath}`);
  console.log(`CSV: ${paths.csvPath}`);
  console.log(`JSON: ${paths.groupedJsonPath}`);
}

main().catch((error) => {
  console.error(error.message);
  process.exitCode = 1;
});
