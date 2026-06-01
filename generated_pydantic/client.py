from __future__ import annotations

import json
from types import UnionType
import urllib.error
import urllib.request
from typing import Any, Literal, Union, get_args, get_origin

from pydantic import BaseModel, ConfigDict, Field

from .registry import get_models

_MISSING = object()


class EndpointResult(BaseModel):
    model_config = ConfigDict(populate_by_name=True)
    status: int
    status_text: str = Field(serialization_alias="statusText")
    request: Any
    response: Any
    raw_response: str = Field(exclude=True)

    @staticmethod
    def _find_table_node(obj: Any) -> dict[str, Any] | None:
        if isinstance(obj, dict):
            if "HEAD" in obj and ("DATA" in obj or "vDATA" in obj):
                return obj
            for v in obj.values():
                found = EndpointResult._find_table_node(v)
                if found is not None:
                    return found
        elif isinstance(obj, list):
            for item in obj:
                found = EndpointResult._find_table_node(item)
                if found is not None:
                    return found
        return None

    def to_polars(self) -> Any:
        try:
            import polars as pl
        except ImportError as exc:
            raise ImportError("polars is not installed. Run: pip install polars") from exc
        node = self._find_table_node(self.response)
        if node is None:
            raise ValueError("No HEAD/DATA table found in response")
        key = "DATA" if "DATA" in node else "vDATA"
        return pl.DataFrame(dict(zip(node["HEAD"], zip(*node[key]))))

    def to_polars_string(self) -> str:
        import polars as pl
        df = self.to_polars()
        with pl.Config(tbl_rows=len(df), tbl_cols=len(df.columns)):
            return str(df)


def _find_model(endpoint_id: str, suffix: str) -> type[Any] | None:
    models = get_models(endpoint_id)
    return next((model for name, model in models.items() if name.endswith(suffix)), None)


def _is_pydantic_model(value: Any) -> bool:
    return isinstance(value, type) and hasattr(value, "model_fields") and hasattr(value, "model_validate")


def _single_literal_value(annotation: Any) -> Any:
    origin = get_origin(annotation)
    if origin is Literal:
        values = get_args(annotation)
        if len(values) == 1:
            return values[0]
        return _MISSING

    if origin in (Union, UnionType):
        values = [
            _single_literal_value(item)
            for item in get_args(annotation)
            if item is not type(None)
        ]
        values = [item for item in values if item is not _MISSING]
        if len(values) == 1:
            return values[0]

    return _MISSING


def _literal_defaults(model_cls: type[Any], values: dict[str, Any]) -> dict[str, Any]:
    defaults: dict[str, Any] = {}
    for field_name, field in model_cls.model_fields.items():
        alias = field.alias or field_name
        if field_name in values or alias in values:
            continue
        value = _single_literal_value(field.annotation)
        if value is not _MISSING:
            defaults[field_name] = value
    return defaults


def _wrapper_info(request_model: type[Any]) -> tuple[str, type[Any]] | None:
    if len(request_model.model_fields) != 1:
        return None

    field_name, field = next(iter(request_model.model_fields.items()))
    field_model = field.annotation
    if not _is_pydantic_model(field_model):
        return None

    return field.alias or field_name, field_model


def _parse_headers(headers: dict[str, str] | str | None) -> dict[str, str]:
    if headers is None:
        return {}
    if isinstance(headers, dict):
        return headers
    stripped = headers.strip()
    if not stripped:
        return {}
    return json.loads(stripped)


def build_request(endpoint_id: str, payload: Any) -> Any:
    request_model = _find_model(endpoint_id, "Request")
    if request_model is None:
        raise KeyError(f"No Request model is registered for endpoint: {endpoint_id}")
    model = request_model.model_validate(payload)
    return model.model_dump(by_alias=True, mode="json", exclude_none=True)


def validate_response(endpoint_id: str, payload: Any) -> Any:
    response_model = _find_model(endpoint_id, "Response")
    if response_model is None or not isinstance(payload, dict):
        return payload
    model = response_model.model_validate(payload)
    return model.model_dump(by_alias=True, mode="json", exclude_none=True)


class EndpointCommand:
    def __init__(self, endpoint_id: str, method: str = "POST") -> None:
        self.endpoint_id = endpoint_id
        self.method = (method or "POST").upper()

    @property
    def request_model(self) -> type[Any]:
        request_model = _find_model(self.endpoint_id, "Request")
        if request_model is None:
            raise KeyError(f"No Request model is registered for endpoint: {self.endpoint_id}")
        return request_model

    @property
    def response_model(self) -> type[Any] | None:
        return _find_model(self.endpoint_id, "Response")

    def build_payload(self, **values: Any) -> dict[str, Any]:
        request_model = self.request_model
        clean_values = {key: value for key, value in values.items() if value is not None}
        wrapper = _wrapper_info(request_model)
        if wrapper is not None:
            wrapper_key, wrapper_model = wrapper
            data = {
                **_literal_defaults(wrapper_model, clean_values),
                **clean_values,
            }
            wrapper_payload = wrapper_model.model_validate(data).model_dump(
                by_alias=True,
                mode="json",
                exclude_none=True,
            )
            return {wrapper_key: wrapper_payload}

        data = {
            **_literal_defaults(request_model, clean_values),
            **clean_values,
        }
        model = request_model.model_validate(data)
        return model.model_dump(by_alias=True, mode="json", exclude_none=True)

    def validate_response(self, payload: Any) -> Any:
        return validate_response(self.endpoint_id, payload)

    def run(
        self,
        url: str,
        headers: dict[str, str] | str | None = None,
        method: str | None = None,
        timeout: int = 30,
        **values: Any,
    ) -> EndpointResult:
        return run_endpoint(
            endpoint_id=self.endpoint_id,
            payload=self.build_payload(**values),
            url=url,
            method=method or self.method,
            headers=headers,
            timeout=timeout,
        )


def endpoint(endpoint_id: str, method: str = "POST") -> EndpointCommand:
    return EndpointCommand(endpoint_id=endpoint_id, method=method)


def run_endpoint(
    endpoint_id: str,
    payload: Any,
    url: str,
    method: str = "POST",
    headers: dict[str, str] | str | None = None,
    timeout: int = 30,
) -> EndpointResult:
    method = (method or "POST").upper()
    request_headers = _parse_headers(headers)
    request_headers.setdefault("Content-Type", "application/json")
    request_headers.setdefault("Accept", "application/json")

    request_payload = build_request(endpoint_id, payload)
    request_body = None
    if method != "GET":
        request_body = json.dumps(request_payload).encode("utf-8")

    request = urllib.request.Request(
        url,
        data=request_body,
        headers=request_headers,
        method=method,
    )

    try:
        with urllib.request.urlopen(request, timeout=timeout) as response:
            response_text = response.read().decode("utf-8")
            status = response.status
            status_text = response.reason
    except urllib.error.HTTPError as error:
        response_text = error.read().decode("utf-8")
        status = error.code
        status_text = error.reason

    try:
        response_payload = json.loads(response_text)
    except json.JSONDecodeError:
        response_payload = response_text

    return EndpointResult(
        status=status,
        status_text=status_text,
        request=request_payload,
        response=validate_response(endpoint_id, response_payload),
        raw_response=response_text,
    )
