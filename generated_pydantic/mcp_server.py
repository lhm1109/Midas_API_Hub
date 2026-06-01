from __future__ import annotations

import json
import os
import sys
from pathlib import Path
from typing import Any
from urllib.error import HTTPError, URLError
from urllib.parse import urlencode
from urllib.request import Request, urlopen

PACKAGE_ROOT = Path(__file__).resolve().parent
PACKAGE_PARENT = PACKAGE_ROOT.parent

if str(PACKAGE_PARENT) not in sys.path:
    sys.path.insert(0, str(PACKAGE_PARENT))

try:
    from mcp.server.fastmcp import FastMCP
except ImportError as exc:  # pragma: no cover - startup guidance
    raise SystemExit(
        "The MCP package is required. Install it with: pip install -r requirements-mcp.txt"
    ) from exc

from generated_pydantic.registry import PYDANTIC_MODEL_CLASSES, PYDANTIC_MODULES

MANIFEST_PATH = PACKAGE_ROOT / "manifest.json"
README_PATH = PACKAGE_ROOT / "README.md"

mcp = FastMCP("generated-pydantic-api")


def _load_dotenv(path: Path) -> None:
    if not path.exists():
        return
    for raw_line in path.read_text(encoding="utf-8").splitlines():
        line = raw_line.strip()
        if not line or line.startswith("#") or "=" not in line:
            continue
        key, value = line.split("=", 1)
        key = key.strip()
        value = value.strip().strip('"').strip("'")
        if key and key not in os.environ:
            os.environ[key] = value


_load_dotenv(PACKAGE_ROOT / ".env")
_load_dotenv(Path.cwd() / ".env")


def _read_json(path: Path) -> Any:
    return json.loads(path.read_text(encoding="utf-8"))


def _manifest_modules() -> list[dict[str, Any]]:
    manifest = _read_json(MANIFEST_PATH)
    modules = manifest.get("modules", [])
    return modules if isinstance(modules, list) else []


def _find_manifest_item(endpoint_id: str) -> dict[str, Any]:
    for item in _manifest_modules():
        if item.get("endpointId") == endpoint_id:
            return item
    raise ValueError(f"Unknown endpoint_id: {endpoint_id}")


def _safe_json_loads(value: Any) -> Any:
    if value is None:
        return None
    if isinstance(value, (dict, list)):
        return value
    if not isinstance(value, str):
        return None
    stripped = value.strip()
    if not stripped:
        return None
    try:
        return json.loads(stripped)
    except json.JSONDecodeError:
        return None


def _pick_schema(spec_row: dict[str, Any] | None, source: str = "auto") -> dict[str, Any]:
    if not spec_row:
        return {"source": None, "schema": None}

    candidates = {
        "enhanced": ("json_schema_enhanced", spec_row.get("json_schema_enhanced")),
        "current": ("json_schema", spec_row.get("json_schema")),
        "original": ("json_schema_original", spec_row.get("json_schema_original")),
    }

    if source != "auto":
        if source not in candidates:
            raise ValueError("source must be one of: auto, enhanced, current, original")
        column, value = candidates[source]
        return {"source": column, "schema": _safe_json_loads(value)}

    for _, (column, value) in candidates.items():
        parsed = _safe_json_loads(value)
        if parsed:
            return {"source": column, "schema": parsed}
    return {"source": None, "schema": None}


def _supabase_config() -> tuple[str, str]:
    url = os.environ.get("SUPABASE_URL", "").rstrip("/")
    key = os.environ.get("SUPABASE_SERVICE_KEY") or os.environ.get("SUPABASE_SERVICE_ROLE_KEY") or ""
    if not url or not key:
        raise RuntimeError(
            "SUPABASE_URL and SUPABASE_SERVICE_KEY are required. "
            "Put them in environment variables or generated_pydantic/.env."
        )
    return url, key


def _supabase_get(table: str, params: dict[str, str]) -> list[dict[str, Any]]:
    base_url, service_key = _supabase_config()
    query = urlencode(params)
    request = Request(
        f"{base_url}/rest/v1/{table}?{query}",
        headers={
            "apikey": service_key,
            "Authorization": f"Bearer {service_key}",
            "Accept": "application/json",
        },
    )
    try:
        with urlopen(request, timeout=30) as response:
            payload = response.read().decode("utf-8")
    except HTTPError as exc:
        detail = exc.read().decode("utf-8", errors="replace")
        raise RuntimeError(f"Supabase {table} query failed: HTTP {exc.code} {detail}") from exc
    except URLError as exc:
        raise RuntimeError(f"Supabase {table} query failed: {exc.reason}") from exc

    data = json.loads(payload)
    if not isinstance(data, list):
        raise RuntimeError(f"Supabase {table} returned non-list payload")
    return data


def _latest_version(endpoint_id: str) -> dict[str, Any] | None:
    rows = _supabase_get(
        "versions",
        {
            "select": "*",
            "endpoint_id": f"eq.{endpoint_id}",
            "order": "updated_at.desc",
            "limit": "1",
        },
    )
    return rows[0] if rows else None


def _endpoint_row(endpoint_id: str) -> dict[str, Any] | None:
    rows = _supabase_get(
        "endpoints",
        {
            "select": "*",
            "id": f"eq.{endpoint_id}",
            "limit": "1",
        },
    )
    return rows[0] if rows else None


def _spec_row(version_id: str) -> dict[str, Any] | None:
    rows = _supabase_get(
        "spec_data",
        {
            "select": "*",
            "version_id": f"eq.{version_id}",
            "limit": "1",
        },
    )
    return rows[0] if rows else None


@mcp.tool()
def list_endpoints(search: str = "", product_id: str = "", limit: int = 50) -> dict[str, Any]:
    """List generated endpoint ids and their Pydantic modules."""
    normalized_search = search.strip().lower()
    normalized_product = product_id.strip().lower()
    bounded_limit = max(1, min(int(limit), 500))

    matches: list[dict[str, Any]] = []
    for item in _manifest_modules():
        haystack = " ".join(
            str(item.get(key, ""))
            for key in ("endpointId", "endpointName", "productId", "moduleName", "importPath")
        ).lower()
        if normalized_search and normalized_search not in haystack:
            continue
        if normalized_product and str(item.get("productId", "")).lower() != normalized_product:
            continue
        matches.append(
            {
                "endpointId": item.get("endpointId"),
                "endpointName": item.get("endpointName"),
                "productId": item.get("productId"),
                "importPath": item.get("importPath"),
                "rootClasses": item.get("rootClasses", []),
            }
        )

    return {
        "count": len(matches),
        "returned": min(len(matches), bounded_limit),
        "endpoints": matches[:bounded_limit],
    }


@mcp.tool()
def get_pydantic_model(endpoint_id: str, include_code: bool = True) -> dict[str, Any]:
    """Return local generated Pydantic module metadata and optional Python source code."""
    item = _find_manifest_item(endpoint_id)
    file_path = PACKAGE_ROOT / str(item["filePath"])
    result = {
        "endpointId": endpoint_id,
        "endpointName": item.get("endpointName"),
        "productId": item.get("productId"),
        "importPath": item.get("importPath"),
        "rootClasses": item.get("rootClasses", []),
        "modulePath": str(file_path),
        "registryClasses": PYDANTIC_MODEL_CLASSES.get(endpoint_id, []),
        "registryModule": PYDANTIC_MODULES.get(endpoint_id),
    }
    if include_code:
        result["code"] = file_path.read_text(encoding="utf-8")
    return result


@mcp.tool()
def get_api_spec(endpoint_id: str, source: str = "auto") -> dict[str, Any]:
    """Fetch the latest API spec schema for an endpoint from Supabase."""
    endpoint = _endpoint_row(endpoint_id)
    version = _latest_version(endpoint_id)
    if not version:
        return {
            "endpointId": endpoint_id,
            "endpoint": endpoint,
            "version": None,
            "schemaSource": None,
            "schema": None,
            "error": "No version found for endpoint.",
        }

    spec = _spec_row(str(version["id"]))
    selected = _pick_schema(spec, source)
    return {
        "endpointId": endpoint_id,
        "endpoint": endpoint,
        "version": version,
        "schemaSource": selected["source"],
        "schema": selected["schema"],
    }


@mcp.tool()
def get_endpoint_context(
    endpoint_id: str,
    include_model_code: bool = True,
    include_api_spec: bool = True,
    spec_source: str = "auto",
) -> dict[str, Any]:
    """Return the combined context an AI coder usually needs for one endpoint."""
    context = {
        "pydantic": get_pydantic_model(endpoint_id, include_code=include_model_code),
        "apiSpec": None,
    }
    if include_api_spec:
        context["apiSpec"] = get_api_spec(endpoint_id, source=spec_source)
    return context


@mcp.tool()
def get_usage_guide() -> dict[str, Any]:
    """Return the generated_pydantic README content."""
    return {
        "path": str(README_PATH),
        "content": README_PATH.read_text(encoding="utf-8"),
    }


if __name__ == "__main__":
    mcp.run()
