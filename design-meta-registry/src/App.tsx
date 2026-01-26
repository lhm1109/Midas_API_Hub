import { useState, useEffect } from 'react';

interface EntityType {
    id: string;
    name: string;
    description: string;
    icon: string;
    color: string;
    fieldCount: number;
    entityCount: number;
}

interface EntityField {
    id: number;
    entity_type_id: string;
    field_name: string;
    field_type: string;
    is_required: number;
    is_primary: number;
}

interface Entity {
    id: string;
    entity_type_id: string;
    data: Record<string, string>;
}

interface RelationType {
    id: string;
    name: string;
    source_type_id: string;
    target_type_id: string;
    source_type_name: string;
    target_type_name: string;
    source_icon: string;
    target_icon: string;
}

interface Relation {
    relation_type_id: string;
    source_entity_id: string;
    target_entity_id: string;
}

export default function App() {
    const [tab, setTab] = useState<'types' | 'data' | 'relations'>('types');
    const [entityTypes, setEntityTypes] = useState<EntityType[]>([]);
    const [selectedType, setSelectedType] = useState<string | null>(null);
    const [fields, setFields] = useState<EntityField[]>([]);
    const [entities, setEntities] = useState<Entity[]>([]);
    const [relationTypes, setRelationTypes] = useState<RelationType[]>([]);
    const [relations, setRelations] = useState<Relation[]>([]);
    const [selectedRelationType, setSelectedRelationType] = useState<string | null>(null);
    const [selectedSourceEntity, setSelectedSourceEntity] = useState<string | null>(null);
    const [message, setMessage] = useState('');
    const [loading, setLoading] = useState(true);

    const [showNewTypeModal, setShowNewTypeModal] = useState(false);
    const [showNewFieldModal, setShowNewFieldModal] = useState(false);
    const [showNewRelationTypeModal, setShowNewRelationTypeModal] = useState(false);
    const [newType, setNewType] = useState({ id: '', name: '', icon: '📁', color: '#6b7280' });
    const [newField, setNewField] = useState({ field_name: '', field_type: 'text', is_primary: false });
    const [newRelationType, setNewRelationType] = useState({ id: '', name: '', source_type_id: '', target_type_id: '' });
    const [targetEntities, setTargetEntities] = useState<Entity[]>([]);

    // Load data
    useEffect(() => {
        Promise.all([
            fetch('/api/entity-types').then(r => r.json()),
            fetch('/api/relation-types').then(r => r.json())
        ]).then(([types, relTypes]) => {
            setEntityTypes(types);
            setRelationTypes(relTypes);
            if (types.length > 0) setSelectedType(types[0].id);
            if (relTypes.length > 0) setSelectedRelationType(relTypes[0].id);
            setLoading(false);
        });
    }, []);

    // Load fields and entities when type changes
    useEffect(() => {
        if (selectedType) {
            Promise.all([
                fetch(`/api/entity-types/${selectedType}/fields`).then(r => r.json()),
                fetch(`/api/entities?type=${selectedType}`).then(r => r.json())
            ]).then(([f, e]) => {
                setFields(f);
                setEntities(e);
            });
        }
    }, [selectedType]);

    // Load relations when relation type changes
    useEffect(() => {
        if (selectedRelationType) {
            fetch(`/api/relations?type=${selectedRelationType}`).then(r => r.json()).then(setRelations);
        }
    }, [selectedRelationType]);

    // Load target entities for relation mapping
    useEffect(() => {
        if (selectedRelationType) {
            const relType = relationTypes.find(t => t.id === selectedRelationType);
            if (relType) {
                fetch(`/api/entities?type=${relType.target_type_id}`)
                    .then(r => r.json())
                    .then(setTargetEntities);
            }
        }
    }, [selectedRelationType, relationTypes]);

    // Show message
    const showMessage = (msg: string) => {
        setMessage(msg);
        setTimeout(() => setMessage(''), 3000);
    };

    // Create entity type
    const createEntityType = async () => {
        const res = await fetch('/api/entity-types', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(newType)
        });
        if (res.ok) {
            const types = await fetch('/api/entity-types').then(r => r.json());
            setEntityTypes(types);
            setShowNewTypeModal(false);
            setNewType({ id: '', name: '', icon: '📁', color: '#6b7280' });
            showMessage('✅ 오브젝트 타입 생성됨');
        }
    };

    // Create field
    const createField = async () => {
        if (!selectedType) return;
        const res = await fetch(`/api/entity-types/${selectedType}/fields`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(newField)
        });
        if (res.ok) {
            const f = await fetch(`/api/entity-types/${selectedType}/fields`).then(r => r.json());
            setFields(f);
            setShowNewFieldModal(false);
            setNewField({ field_name: '', field_type: 'text', is_primary: false });
            showMessage('✅ 필드 추가됨');
        }
    };

    // Create relation type
    const createRelationType = async () => {
        const res = await fetch('/api/relation-types', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(newRelationType)
        });
        if (res.ok) {
            const types = await fetch('/api/relation-types').then(r => r.json());
            setRelationTypes(types);
            setShowNewRelationTypeModal(false);
            setNewRelationType({ id: '', name: '', source_type_id: '', target_type_id: '' });
            showMessage('✅ 관계 타입 생성됨');
        }
    };

    // Handle paste for entities
    const handlePaste = async (e: React.ClipboardEvent) => {
        if (!selectedType || fields.length === 0) return;

        const text = e.clipboardData.getData('text');
        const rows = text.split('\n').filter(r => r.trim());
        if (rows.length === 0) return;

        e.preventDefault();

        const fieldNames = fields.map(f => f.field_name);
        const items = rows.map(row => {
            const values = row.split('\t');
            const item: Record<string, string> = {};
            fieldNames.forEach((name, i) => {
                if (values[i]) item[name] = values[i].trim();
            });
            // Use first field as ID if it looks like an ID
            if (values[0] && !values[0].includes(' ')) {
                item.id = values[0].trim();
            }
            return item;
        });

        const res = await fetch('/api/entities/bulk', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ entity_type_id: selectedType, items })
        });

        if (res.ok) {
            const e = await fetch(`/api/entities?type=${selectedType}`).then(r => r.json());
            setEntities(e);
            showMessage(`✅ ${items.length}개 항목 추가됨`);
        }
    };

    // Toggle relation
    const toggleRelation = async (sourceId: string, targetId: string) => {
        if (!selectedRelationType) return;

        const existing = relations.find(r =>
            r.source_entity_id === sourceId && r.target_entity_id === targetId
        );

        if (existing) {
            await fetch(`/api/relations/${selectedRelationType}/${sourceId}/${targetId}`, { method: 'DELETE' });
            setRelations(relations.filter(r =>
                !(r.source_entity_id === sourceId && r.target_entity_id === targetId)
            ));
        } else {
            await fetch('/api/relations', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ relation_type_id: selectedRelationType, source_entity_id: sourceId, target_entity_id: targetId })
            });
            setRelations([...relations, { relation_type_id: selectedRelationType, source_entity_id: sourceId, target_entity_id: targetId }]);
        }
    };

    if (loading) return <div className="loading">Loading...</div>;

    const currentType = entityTypes.find(t => t.id === selectedType);
    const currentRelationType = relationTypes.find(t => t.id === selectedRelationType);

    // Get entities for relation mapping
    const sourceEntities = currentRelationType
        ? entities.filter(e => e.entity_type_id === currentRelationType.source_type_id)
        : [];

    // CSV 임포트 함수
    const importCSV = async () => {
        showMessage('📂 CSV 임포트 중...');
        const res = await fetch('/api/import/csv', { method: 'POST' });
        if (res.ok) {
            const data = await res.json();
            showMessage(`✅ 임포트 완료! ${data.stats.entities}개 엔티티, ${data.stats.relations}개 관계`);
            // Reload data
            const [types, relTypes] = await Promise.all([
                fetch('/api/entity-types').then(r => r.json()),
                fetch('/api/relation-types').then(r => r.json())
            ]);
            setEntityTypes(types);
            setRelationTypes(relTypes);
            if (types.length > 0) setSelectedType(types[0].id);
        } else {
            showMessage('❌ 임포트 실패');
        }
    };

    return (
        <div className="app-simple">
            <header className="header-simple">
                <div className="header-top">
                    <h1>🗂️ Entity-Relation Registry</h1>
                    <button className="btn-import" onClick={importCSV}>
                        📥 civil_data CSV 임포트
                    </button>
                </div>
                <div className="tab-bar">
                    <button className={tab === 'types' ? 'active' : ''} onClick={() => setTab('types')}>
                        📦 Object Types ({entityTypes.length})
                    </button>
                    <button className={tab === 'data' ? 'active' : ''} onClick={() => setTab('data')}>
                        📋 Data
                    </button>
                    <button className={tab === 'relations' ? 'active' : ''} onClick={() => setTab('relations')}>
                        🔗 Relations ({relationTypes.length})
                    </button>
                </div>
            </header>

            {message && <div className="toast">{message}</div>}

            <main className="main-simple">

                {/* Object Types Tab */}
                {tab === 'types' && (
                    <div className="two-panel">
                        <div className="panel">
                            <div className="panel-header">
                                <h3>Object Types</h3>
                                <button className="btn-accent" onClick={() => setShowNewTypeModal(true)}>+ 추가</button>
                            </div>
                            <div className="type-list">
                                {entityTypes.map(t => (
                                    <div
                                        key={t.id}
                                        className={`type-card ${selectedType === t.id ? 'selected' : ''}`}
                                        onClick={() => setSelectedType(t.id)}
                                        style={{ borderLeftColor: t.color }}
                                    >
                                        <span className="type-icon">{t.icon}</span>
                                        <div className="type-info">
                                            <div className="type-name">{t.name}</div>
                                            <div className="type-meta">{t.fieldCount} fields · {t.entityCount} items</div>
                                        </div>
                                    </div>
                                ))}
                            </div>
                        </div>

                        <div className="panel">
                            <div className="panel-header">
                                <h3>Fields for: {currentType?.name}</h3>
                                <button className="btn-accent" onClick={() => setShowNewFieldModal(true)}>+ 필드 추가</button>
                            </div>
                            <table>
                                <thead>
                                    <tr>
                                        <th>Field Name</th>
                                        <th>Type</th>
                                        <th>Primary</th>
                                        <th>Required</th>
                                    </tr>
                                </thead>
                                <tbody>
                                    {fields.map(f => (
                                        <tr key={f.id}>
                                            <td>{f.field_name}</td>
                                            <td><span className="badge">{f.field_type}</span></td>
                                            <td>{f.is_primary ? '⭐' : ''}</td>
                                            <td>{f.is_required ? '✓' : ''}</td>
                                        </tr>
                                    ))}
                                </tbody>
                            </table>
                        </div>
                    </div>
                )}

                {/* Data Tab */}
                {tab === 'data' && (
                    <div className="data-section">
                        <div className="type-selector">
                            {entityTypes.map(t => (
                                <button
                                    key={t.id}
                                    className={selectedType === t.id ? 'active' : ''}
                                    onClick={() => setSelectedType(t.id)}
                                >
                                    {t.icon} {t.name} ({t.entityCount})
                                </button>
                            ))}
                        </div>

                        <div className="section-header">
                            <h3>{currentType?.icon} {currentType?.name} 데이터</h3>
                            <p className="hint">💡 Excel에서 복사 → 테이블 클릭 → Ctrl+V (컬럼순: {fields.map(f => f.field_name).join(', ')})</p>
                        </div>

                        <div
                            className="table-wrapper"
                            onPaste={handlePaste}
                            tabIndex={0}
                        >
                            <table>
                                <thead>
                                    <tr>
                                        <th>ID</th>
                                        {fields.map(f => <th key={f.id}>{f.field_name}</th>)}
                                    </tr>
                                </thead>
                                <tbody>
                                    {entities.map(e => (
                                        <tr key={e.id}>
                                            <td className="id-cell">{e.id}</td>
                                            {fields.map(f => (
                                                <td key={f.id}>{e.data[f.field_name] || ''}</td>
                                            ))}
                                        </tr>
                                    ))}
                                    {entities.length === 0 && (
                                        <tr>
                                            <td colSpan={fields.length + 1} className="empty-cell">
                                                데이터 없음. Excel에서 복사 후 붙여넣기 하세요.
                                            </td>
                                        </tr>
                                    )}
                                </tbody>
                            </table>
                        </div>
                    </div>
                )}

                {/* Relations Tab */}
                {tab === 'relations' && (
                    <div className="two-panel">
                        <div className="panel narrow">
                            <div className="panel-header">
                                <h3>Relation Types</h3>
                                <button className="btn-accent" onClick={() => setShowNewRelationTypeModal(true)}>+ 추가</button>
                            </div>
                            <div className="type-list">
                                {relationTypes.map(t => (
                                    <div
                                        key={t.id}
                                        className={`relation-type-card ${selectedRelationType === t.id ? 'selected' : ''}`}
                                        onClick={() => setSelectedRelationType(t.id)}
                                    >
                                        <div className="relation-viz">
                                            <span>{t.source_icon}</span>
                                            <span className="arrow">→</span>
                                            <span>{t.target_icon}</span>
                                        </div>
                                        <div className="relation-name">{t.name}</div>
                                    </div>
                                ))}
                            </div>
                        </div>

                        <div className="panel wide">
                            {currentRelationType && (
                                <>
                                    <div className="panel-header">
                                        <h3>{currentRelationType.source_icon} {currentRelationType.source_type_name} → {currentRelationType.target_icon} {currentRelationType.target_type_name}</h3>
                                    </div>
                                    <div className="relation-matrix">
                                        <div className="source-list">
                                            <h4>Source ({sourceEntities.length})</h4>
                                            {sourceEntities.map(e => (
                                                <div
                                                    key={e.id}
                                                    className={`source-item ${selectedSourceEntity === e.id ? 'selected' : ''}`}
                                                    onClick={() => setSelectedSourceEntity(e.id)}
                                                >
                                                    {e.data.name || e.id}
                                                </div>
                                            ))}
                                        </div>
                                        <div className="target-list">
                                            <h4>Target ({targetEntities.length}) - 체크박스로 연결</h4>
                                            {selectedSourceEntity ? (
                                                <div className="target-grid">
                                                    {targetEntities.map(t => {
                                                        const isConnected = relations.some(r =>
                                                            r.source_entity_id === selectedSourceEntity && r.target_entity_id === t.id
                                                        );
                                                        return (
                                                            <label key={t.id} className={`target-item ${isConnected ? 'connected' : ''}`}>
                                                                <input
                                                                    type="checkbox"
                                                                    checked={isConnected}
                                                                    onChange={() => toggleRelation(selectedSourceEntity, t.id)}
                                                                />
                                                                <span>{t.data.name || t.id}</span>
                                                            </label>
                                                        );
                                                    })}
                                                </div>
                                            ) : (
                                                <p className="empty-state">← Source 항목을 선택하세요</p>
                                            )}
                                        </div>
                                    </div>
                                </>
                            )}
                        </div>
                    </div>
                )}
            </main>

            {/* New Type Modal */}
            {showNewTypeModal && (
                <div className="modal-overlay" onClick={() => setShowNewTypeModal(false)}>
                    <div className="modal" onClick={e => e.stopPropagation()}>
                        <h3>새 Object Type</h3>
                        <div className="form-group">
                            <label>ID</label>
                            <input value={newType.id} onChange={e => setNewType({ ...newType, id: e.target.value })} placeholder="design-code" />
                        </div>
                        <div className="form-group">
                            <label>Name</label>
                            <input value={newType.name} onChange={e => setNewType({ ...newType, name: e.target.value })} placeholder="Design Code" />
                        </div>
                        <div className="form-group">
                            <label>Icon</label>
                            <input value={newType.icon} onChange={e => setNewType({ ...newType, icon: e.target.value })} placeholder="📁" />
                        </div>
                        <div className="form-group">
                            <label>Color</label>
                            <input type="color" value={newType.color} onChange={e => setNewType({ ...newType, color: e.target.value })} />
                        </div>
                        <div className="modal-actions">
                            <button onClick={() => setShowNewTypeModal(false)}>취소</button>
                            <button className="btn-accent" onClick={createEntityType}>생성</button>
                        </div>
                    </div>
                </div>
            )}

            {/* New Field Modal */}
            {showNewFieldModal && (
                <div className="modal-overlay" onClick={() => setShowNewFieldModal(false)}>
                    <div className="modal" onClick={e => e.stopPropagation()}>
                        <h3>새 Field</h3>
                        <div className="form-group">
                            <label>Field Name</label>
                            <input value={newField.field_name} onChange={e => setNewField({ ...newField, field_name: e.target.value })} placeholder="name" />
                        </div>
                        <div className="form-group">
                            <label>Type</label>
                            <select value={newField.field_type} onChange={e => setNewField({ ...newField, field_type: e.target.value })}>
                                <option value="text">Text</option>
                                <option value="number">Number</option>
                                <option value="boolean">Boolean</option>
                                <option value="enum">Enum</option>
                                <option value="date">Date</option>
                            </select>
                        </div>
                        <div className="form-group">
                            <label>
                                <input type="checkbox" checked={newField.is_primary} onChange={e => setNewField({ ...newField, is_primary: e.target.checked })} />
                                Primary (표시용 주요 필드)
                            </label>
                        </div>
                        <div className="modal-actions">
                            <button onClick={() => setShowNewFieldModal(false)}>취소</button>
                            <button className="btn-accent" onClick={createField}>추가</button>
                        </div>
                    </div>
                </div>
            )}

            {/* New Relation Type Modal */}
            {showNewRelationTypeModal && (
                <div className="modal-overlay" onClick={() => setShowNewRelationTypeModal(false)}>
                    <div className="modal" onClick={e => e.stopPropagation()}>
                        <h3>새 Relation Type</h3>
                        <div className="form-group">
                            <label>ID</label>
                            <input value={newRelationType.id} onChange={e => setNewRelationType({ ...newRelationType, id: e.target.value })} placeholder="code-parameters" />
                        </div>
                        <div className="form-group">
                            <label>Name</label>
                            <input value={newRelationType.name} onChange={e => setNewRelationType({ ...newRelationType, name: e.target.value })} placeholder="Code → Parameters" />
                        </div>
                        <div className="form-group">
                            <label>Source Type</label>
                            <select value={newRelationType.source_type_id} onChange={e => setNewRelationType({ ...newRelationType, source_type_id: e.target.value })}>
                                <option value="">선택...</option>
                                {entityTypes.map(t => <option key={t.id} value={t.id}>{t.icon} {t.name}</option>)}
                            </select>
                        </div>
                        <div className="form-group">
                            <label>Target Type</label>
                            <select value={newRelationType.target_type_id} onChange={e => setNewRelationType({ ...newRelationType, target_type_id: e.target.value })}>
                                <option value="">선택...</option>
                                {entityTypes.map(t => <option key={t.id} value={t.id}>{t.icon} {t.name}</option>)}
                            </select>
                        </div>
                        <div className="modal-actions">
                            <button onClick={() => setShowNewRelationTypeModal(false)}>취소</button>
                            <button className="btn-accent" onClick={createRelationType}>생성</button>
                        </div>
                    </div>
                </div>
            )}
        </div>
    );
}
