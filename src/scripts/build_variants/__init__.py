"""Build-variant runner support."""

from .application import (
    ApplicationError,
    prepare_application_output,
    run_application,
)
from .catalog import (
    CatalogError,
    ManifestCatalog,
    load_catalog,
)
from .cli import (
    CliError,
    RunnerArguments,
    create_parser,
    detect_operating_system,
    parse_arguments,
)
from .commands import (
    CommandError,
    ConfigureOptions,
    build_build_command,
    build_configure_command,
)
from .execution import (
    CommandExecutionResult,
    ExecutionError,
    VariantExecutionResult,
    execute_command,
    execute_variant,
    execute_variants,
)
from .manifest import (
    BuildVariantManifest,
    ManifestError,
    load_manifest,
)
from .output import (
    OutputError,
    format_command,
    render_catalog,
    render_variant_plan,
    render_variant_plans,
)
from .planning import (
    PlanningError,
    VariantPlan,
    variant_build_directory_name,
    compiler_tag,
    create_variant_plan,
    create_variant_plans,
    default_compiler,
    resolve_compiler,
)

__all__ = [
    "ApplicationError",
    "BuildVariantManifest",
    "CatalogError",
    "CliError",
    "CommandError",
    "CommandExecutionResult",
    "ConfigureOptions",
    "ExecutionError",
    "ManifestCatalog",
    "ManifestError",
    "OutputError",
    "PlanningError",
    "RunnerArguments",
    "VariantExecutionResult",
    "VariantPlan",
    "build_build_command",
    "build_configure_command",
    "variant_build_directory_name",
    "compiler_tag",
    "create_parser",
    "create_variant_plan",
    "create_variant_plans",
    "default_compiler",
    "execute_command",
    "execute_variant",
    "execute_variants",
    "detect_operating_system",
    "load_catalog",
    "load_manifest",
    "parse_arguments",
    "prepare_application_output",
    "run_application",
    "format_command",
    "render_catalog",
    "render_variant_plan",
    "render_variant_plans",
    "resolve_compiler",
]
