// Generic Entity-Relation Registry - API Server
import express from 'express';
import cors from 'cors';
import Database from 'better-sqlite3';
import { fileURLToPath } from 'url';
import { dirname, join } from 'path';
import fs from 'fs';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

const app = express();
const PORT = 3002;

app.use(cors());
app.use(express.json());

const dbPath = join(__dirname, '../db/registry.db');
const db = new Database(dbPath);

// ============================================================================
// Entity Types (오브젝트 타입 정의)
// ============================================================================

app.get('/api/entity-types', (req, res) => {
    try {
        const types = db.prepare('SELECT * FROM entity_types ORDER BY name').all();
        // Get field count for each type
        for (const t of types) {
            t.fieldCount = db.prepare('SELECT COUNT(*) as cnt FROM entity_fields WHERE entity_type_id = ?').get(t.id).cnt;
            t.entityCount = db.prepare('SELECT COUNT(*) as cnt FROM entities WHERE entity_type_id = ?').get(t.id).cnt;
        }
        res.json(types);
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

app.post('/api/entity-types', (req, res) => {
    try {
        const { id, name, description, icon, color } = req.body;
        db.prepare(`
      INSERT INTO entity_types (id, name, description, icon, color)
      VALUES (?, ?, ?, ?, ?)
    `).run(id, name, description || '', icon || '📁', color || '#6b7280');
        res.json({ success: true, id });
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

app.delete('/api/entity-types/:id', (req, res) => {
    try {
        db.prepare('DELETE FROM entity_types WHERE id = ?').run(req.params.id);
        res.json({ success: true });
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

// ============================================================================
// Entity Fields (필드 정의)
// ============================================================================

app.get('/api/entity-types/:typeId/fields', (req, res) => {
    try {
        const { typeId } = req.params;
        const fields = db.prepare(`
      SELECT * FROM entity_fields 
      WHERE entity_type_id = ? 
      ORDER BY display_order, field_name
    `).all(typeId);
        res.json(fields);
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

app.post('/api/entity-types/:typeId/fields', (req, res) => {
    try {
        const { typeId } = req.params;
        const { field_name, field_type, is_required, is_primary, default_value, display_order } = req.body;
        db.prepare(`
      INSERT INTO entity_fields (entity_type_id, field_name, field_type, is_required, is_primary, default_value, display_order)
      VALUES (?, ?, ?, ?, ?, ?, ?)
    `).run(typeId, field_name, field_type || 'text', is_required ? 1 : 0, is_primary ? 1 : 0, default_value || '', display_order || 0);
        res.json({ success: true });
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

app.delete('/api/entity-fields/:id', (req, res) => {
    try {
        db.prepare('DELETE FROM entity_fields WHERE id = ?').run(req.params.id);
        res.json({ success: true });
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

// ============================================================================
// Entities (실제 데이터)
// ============================================================================

app.get('/api/entities', (req, res) => {
    try {
        const { type } = req.query;
        let entities;

        if (type) {
            entities = db.prepare('SELECT * FROM entities WHERE entity_type_id = ?').all(type);
        } else {
            entities = db.prepare('SELECT * FROM entities').all();
        }

        // Get data for each entity
        for (const e of entities) {
            const data = db.prepare('SELECT field_name, field_value FROM entity_data WHERE entity_id = ?').all(e.id);
            e.data = {};
            for (const d of data) {
                e.data[d.field_name] = d.field_value;
            }
        }

        res.json(entities);
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

app.post('/api/entities', (req, res) => {
    try {
        const { id, entity_type_id, data } = req.body;
        const entityId = id || `${entity_type_id}-${Date.now()}`;

        db.prepare('INSERT INTO entities (id, entity_type_id) VALUES (?, ?)').run(entityId, entity_type_id);

        if (data) {
            const insertData = db.prepare('INSERT INTO entity_data (entity_id, field_name, field_value) VALUES (?, ?, ?)');
            for (const [key, value] of Object.entries(data)) {
                insertData.run(entityId, key, String(value));
            }
        }

        res.json({ success: true, id: entityId });
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

// Bulk create entities (for paste)
app.post('/api/entities/bulk', (req, res) => {
    try {
        const { entity_type_id, items } = req.body;
        const insertEntity = db.prepare('INSERT OR REPLACE INTO entities (id, entity_type_id) VALUES (?, ?)');
        const insertData = db.prepare('INSERT OR REPLACE INTO entity_data (entity_id, field_name, field_value) VALUES (?, ?, ?)');

        const insertAll = db.transaction((items) => {
            for (const item of items) {
                const entityId = item.id || `${entity_type_id}-${Date.now()}-${Math.random().toString(36).substr(2, 5)}`;
                insertEntity.run(entityId, entity_type_id);
                for (const [key, value] of Object.entries(item)) {
                    if (key !== 'id') {
                        insertData.run(entityId, key, String(value));
                    }
                }
            }
        });

        insertAll(items);
        res.json({ success: true, count: items.length });
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

app.delete('/api/entities/:id', (req, res) => {
    try {
        db.prepare('DELETE FROM entities WHERE id = ?').run(req.params.id);
        res.json({ success: true });
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

// ============================================================================
// Relation Types (관계 정의)
// ============================================================================

app.get('/api/relation-types', (req, res) => {
    try {
        const types = db.prepare(`
      SELECT rt.*, 
             st.name as source_type_name, st.icon as source_icon,
             tt.name as target_type_name, tt.icon as target_icon
      FROM relation_types rt
      JOIN entity_types st ON rt.source_type_id = st.id
      JOIN entity_types tt ON rt.target_type_id = tt.id
    `).all();
        res.json(types);
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

app.post('/api/relation-types', (req, res) => {
    try {
        const { id, name, source_type_id, target_type_id, relation_kind, description } = req.body;
        db.prepare(`
      INSERT INTO relation_types (id, name, source_type_id, target_type_id, relation_kind, description)
      VALUES (?, ?, ?, ?, ?, ?)
    `).run(id, name, source_type_id, target_type_id, relation_kind || 'many-to-many', description || '');
        res.json({ success: true, id });
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

app.delete('/api/relation-types/:id', (req, res) => {
    try {
        db.prepare('DELETE FROM relation_types WHERE id = ?').run(req.params.id);
        res.json({ success: true });
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

// ============================================================================
// Relations (실제 관계 데이터)
// ============================================================================

app.get('/api/relations', (req, res) => {
    try {
        const { type, source, target } = req.query;
        let sql = 'SELECT * FROM relations WHERE 1=1';
        const params = [];

        if (type) { sql += ' AND relation_type_id = ?'; params.push(type); }
        if (source) { sql += ' AND source_entity_id = ?'; params.push(source); }
        if (target) { sql += ' AND target_entity_id = ?'; params.push(target); }

        const relations = db.prepare(sql).all(...params);
        res.json(relations);
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

app.post('/api/relations', (req, res) => {
    try {
        const { relation_type_id, source_entity_id, target_entity_id, data } = req.body;
        db.prepare(`
      INSERT OR REPLACE INTO relations (relation_type_id, source_entity_id, target_entity_id, data)
      VALUES (?, ?, ?, ?)
    `).run(relation_type_id, source_entity_id, target_entity_id, JSON.stringify(data || {}));
        res.json({ success: true });
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

app.delete('/api/relations/:typeId/:sourceId/:targetId', (req, res) => {
    try {
        const { typeId, sourceId, targetId } = req.params;
        db.prepare(`
      DELETE FROM relations 
      WHERE relation_type_id = ? AND source_entity_id = ? AND target_entity_id = ?
    `).run(typeId, sourceId, targetId);
        res.json({ success: true });
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

// ============================================================================
// CSV Import (civil_data 폴더용)
// ============================================================================

app.post('/api/import/csv', (req, res) => {
    try {
        const csvDir = join(__dirname, '../DOC/civil_data');

        // Helper: Parse CSV
        const parseCSV = (content) => {
            const lines = content.split('\n').filter(l => l.trim());
            const headers = lines[0].split(',').map(h => h.trim());
            return lines.slice(1).map(line => {
                const values = line.split(',').map(v => v.trim());
                const obj = {};
                headers.forEach((h, i) => obj[h] = values[i] || '');
                return obj;
            });
        };

        // Clear existing data
        db.prepare('DELETE FROM relations').run();
        db.prepare('DELETE FROM entity_data').run();
        db.prepare('DELETE FROM entities').run();
        db.prepare('DELETE FROM relation_types').run();
        db.prepare('DELETE FROM entity_fields').run();
        db.prepare('DELETE FROM entity_types').run();

        // Create entity types
        const insertType = db.prepare('INSERT INTO entity_types (id, name, icon, color) VALUES (?, ?, ?, ?)');
        insertType.run('panel', '패널 (코드그룹)', '📂', '#3b82f6');
        insertType.run('design-code', '설계 코드', '📋', '#22c55e');
        insertType.run('parameter', '파라미터', '📝', '#f59e0b');
        insertType.run('section', '패널 섹션', '🔷', '#8b5cf6');

        // Create fields
        const insertField = db.prepare('INSERT INTO entity_fields (entity_type_id, field_name, field_type, is_primary) VALUES (?, ?, ?, ?)');
        insertField.run('panel', 'name', 'text', 1);
        insertField.run('design-code', 'name', 'text', 1);
        insertField.run('design-code', 'description', 'text', 0);
        insertField.run('parameter', 'name', 'text', 1);
        insertField.run('parameter', 'section', 'text', 0);
        insertField.run('parameter', 'note', 'text', 0);
        insertField.run('section', 'name', 'text', 1);

        // Create relation types
        const insertRelType = db.prepare('INSERT INTO relation_types (id, name, source_type_id, target_type_id) VALUES (?, ?, ?, ?)');
        insertRelType.run('panel-codes', '패널 → 코드', 'panel', 'design-code');
        insertRelType.run('panel-parameters', '패널 → 파라미터', 'panel', 'parameter');

        const insertEntity = db.prepare('INSERT OR IGNORE INTO entities (id, entity_type_id) VALUES (?, ?)');
        const insertData = db.prepare('INSERT OR REPLACE INTO entity_data (entity_id, field_name, field_value) VALUES (?, ?, ?)');
        const insertRel = db.prepare('INSERT OR IGNORE INTO relations (relation_type_id, source_entity_id, target_entity_id) VALUES (?, ?, ?)');

        let entityCount = 0;
        let relationCount = 0;

        // Get all CSV files in the directory
        const csvFiles = fs.readdirSync(csvDir).filter(f => f.endsWith('.csv'));

        // Process Support Codes files (02, 04, 06)
        const codeFiles = csvFiles.filter(f => f.includes('Support_Codes'));
        for (const filename of codeFiles) {
            const codes = parseCSV(fs.readFileSync(join(csvDir, filename), 'utf-8'));

            for (const row of codes) {
                const panelId = row['패널']?.toLowerCase().replace(/_/g, '-');
                const codeName = row['지원 코드'];
                const codeDesc = row['코드명'];

                if (!panelId || !codeName) continue;

                // Create panel
                insertEntity.run(panelId, 'panel');
                insertData.run(panelId, 'name', panelId.toUpperCase());

                // Create code
                const codeId = codeName.toLowerCase().replace(/[^a-z0-9]/g, '-').replace(/-+/g, '-');
                insertEntity.run(codeId, 'design-code');
                insertData.run(codeId, 'name', codeName);
                insertData.run(codeId, 'description', codeDesc);
                entityCount++;

                // Create relation
                insertRel.run('panel-codes', panelId, codeId);
                relationCount++;
            }
        }

        // Process Parameters files (01, 03, 05, 07)
        const paramFiles = csvFiles.filter(f => f.includes('Parameters'));
        for (const filename of paramFiles) {
            const params = parseCSV(fs.readFileSync(join(csvDir, filename), 'utf-8'));

            for (const row of params) {
                const panelId = row['패널']?.toLowerCase().replace(/_/g, '-');
                const section = row['패널 섹션'];
                const paramName = row['파라미터명'];
                const note = row['비고'];

                if (!panelId || !paramName) continue;

                // Create parameter
                const paramId = (panelId + '-' + paramName).toLowerCase().replace(/[^a-z0-9]/g, '-').replace(/-+/g, '-').substring(0, 60);
                insertEntity.run(paramId, 'parameter');
                insertData.run(paramId, 'name', paramName);
                insertData.run(paramId, 'section', section);
                if (note) insertData.run(paramId, 'note', note);
                entityCount++;

                // Create relation
                insertRel.run('panel-parameters', panelId, paramId);
                relationCount++;
            }
        }

        const stats = {
            entityTypes: db.prepare('SELECT COUNT(*) as cnt FROM entity_types').get().cnt,
            entities: db.prepare('SELECT COUNT(*) as cnt FROM entities').get().cnt,
            relations: db.prepare('SELECT COUNT(*) as cnt FROM relations').get().cnt
        };

        res.json({ success: true, stats, entityCount, relationCount });
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

// ============================================================================
// Graph data for visualization
// ============================================================================

app.get('/api/graph', (req, res) => {
    try {
        const { relationTypeId } = req.query;

        const entities = db.prepare(`
            SELECT e.*, et.icon, et.color, et.name as type_name
            FROM entities e
            JOIN entity_types et ON e.entity_type_id = et.id
        `).all();

        for (const e of entities) {
            const data = db.prepare('SELECT field_name, field_value FROM entity_data WHERE entity_id = ?').all(e.id);
            e.data = {};
            for (const d of data) e.data[d.field_name] = d.field_value;
            e.label = e.data.name || e.id;
        }

        let relSql = 'SELECT r.*, rt.name as relation_name FROM relations r JOIN relation_types rt ON r.relation_type_id = rt.id';
        if (relationTypeId) relSql += ' WHERE r.relation_type_id = ?';
        const relations = relationTypeId ? db.prepare(relSql).all(relationTypeId) : db.prepare(relSql).all();

        const nodes = entities.map(e => ({
            id: e.id,
            label: e.label,
            type: e.entity_type_id,
            typeName: e.type_name,
            icon: e.icon,
            color: e.color,
            data: e.data
        }));

        const edges = relations.map(r => ({
            source: r.source_entity_id,
            target: r.target_entity_id,
            relationId: r.relation_type_id,
            relationName: r.relation_name
        }));

        res.json({ nodes, edges });
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

// ============================================================================
// Stats
// ============================================================================

app.get('/api/stats', (req, res) => {
    try {
        const stats = {
            entityTypes: db.prepare('SELECT COUNT(*) as cnt FROM entity_types').get().cnt,
            entities: db.prepare('SELECT COUNT(*) as cnt FROM entities').get().cnt,
            relationTypes: db.prepare('SELECT COUNT(*) as cnt FROM relation_types').get().cnt,
            relations: db.prepare('SELECT COUNT(*) as cnt FROM relations').get().cnt
        };
        res.json(stats);
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

// ============================================================================
// Server Start
// ============================================================================

app.listen(PORT, () => {
    console.log(`🚀 Generic Entity-Relation Registry API on http://localhost:${PORT}`);
    console.log(`   Database: ${dbPath}`);
});
