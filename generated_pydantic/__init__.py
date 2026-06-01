from typing import Any

from .client import EndpointCommand, EndpointResult, build_request, endpoint, run_endpoint, validate_response
from .registry import PYDANTIC_MODEL_CLASSES, PYDANTIC_MODULES, get_model, get_models, get_module


def _make_endpoint_function(endpoint_id: str, function_name: str):
    def _run(
        *,
        url: str,
        headers: dict[str, str] | str | None = None,
        method: str | None = None,
        timeout: int = 30,
        **values: Any,
    ) -> EndpointResult:
        return endpoint(endpoint_id, method=method or "POST").run(
            url=url,
            headers=headers,
            timeout=timeout,
            **values,
        )

    _run.__name__ = function_name
    _run.__doc__ = f"Run generated Pydantic endpoint helper for {endpoint_id}."
    return _run


for _endpoint_id, _module_path in PYDANTIC_MODULES.items():
    _function_name = _module_path.rsplit(".", 1)[-1]
    if _function_name.isidentifier() and _function_name not in globals():
        globals()[_function_name] = _make_endpoint_function(_endpoint_id, _function_name)


__all__ = [
    "EndpointCommand",
    "EndpointResult",
    "PYDANTIC_MODEL_CLASSES",
    "PYDANTIC_MODULES",
    "build_request",
    "endpoint",
    "get_model",
    "get_models",
    "get_module",
    "run_endpoint",
    "validate_response",
    *[
        _module_path.rsplit(".", 1)[-1]
        for _module_path in PYDANTIC_MODULES.values()
        if _module_path.rsplit(".", 1)[-1].isidentifier()
    ],
]

del _endpoint_id, _module_path, _function_name
