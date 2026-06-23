# cgalpy_backend.py
"""
PEP 517 build backend wrapper around scikit_build_core.

The problem this solves:
  cgalpy can be built in many configurations, each producing a differently-named
  binding library (e.g. cgalpy_kerepec_..., cgalpy_kerdepic_...). The PyPI package
  name must match the library name so that different configurations can coexist in
  the same Python environment without overwriting each other.

  PEP 517 requires 'name' to be static in pyproject.toml, so it cannot be set
  via the normal dynamic metadata mechanism. This backend works around that by:

  1. Intercepting the PEP 517 hooks prepare_metadata_for_build_wheel and build_wheel.
  2. Running a cmake configure pass with the user's --config-settings flags into a
     persistent build/metadata directory. This triggers Conan and configure_file(),
     writing GetMetadata.cmake into the source root with the correct METADATA_NAME
     and METADATA_VERSION baked in.
  3. Reading the name and version from GetMetadata.cmake.
  4. Monkeypatching scikit_build_core.build.metadata.get_standard_metadata to inject
     the correct name and version before scikit_build_core reads pyproject.toml.

  All other PEP 517 hooks are delegated to scikit_build_core unchanged.

Typical usage (single command, no prior cmake run needed):
  pip install <root-dir> --config-settings=cmake.define.CGALPY_KERNEL_NAME=epec ...
"""

import copy
import os
import subprocess

import scikit_build_core.build as _inner
import scikit_build_core.build.metadata as _meta

# ---------------------------------------------------------------------------
# Re-export all PEP 517 hooks that do not need interception.
# pip and build frontends expect all hooks to be present in the backend module.
# ---------------------------------------------------------------------------
get_requires_for_build_wheel = _inner.get_requires_for_build_wheel
get_requires_for_build_sdist = _inner.get_requires_for_build_sdist
build_sdist                  = _inner.build_sdist

try:
    # These hooks were added in newer versions of scikit_build_core
    get_requires_for_build_editable = _inner.get_requires_for_build_editable
    build_editable                  = _inner.build_editable
except AttributeError:
    pass


# ---------------------------------------------------------------------------
# CMake configure pass
# ---------------------------------------------------------------------------

def _cmake_args_from_config_settings(config_settings):
    """
    Extract cmake -D flags from pip's config_settings dict.

    pip passes --config-settings=cmake.define.FOO=BAR as:
        {"cmake.define.FOO": "BAR", ...}
    We convert these to:
        ["-DFOO=BAR", ...]
    which are then forwarded to the cmake configure invocation.
    """
    cmake_args = []
    if config_settings:
        for key, value in config_settings.items():
            if key.startswith("cmake.define."):
                define = key[len("cmake.define."):]
                cmake_args.append(f"-D{define}={value}")
    return cmake_args


def _run_cmake_configure(source_dir, config_settings):
    """
    Run a cmake configure pass into a persistent build/metadata directory.

    We use a persistent directory (not tempfile.TemporaryDirectory) because:
    - Conan writes its output to ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/generators/
      which is a source-tree path that must persist across configure passes.
    - scikit-build-core's own configure pass (which follows) reuses these Conan artifacts.

    We pass -DSKBUILD=1 because the Conan block in CMakeLists.txt is guarded
    by if(SKBUILD), and without it Conan would not run and find_package(CGAL)
    would fail.

    The side effect we care about is configure_file() writing GetMetadata.cmake
    into the source root with METADATA_NAME and METADATA_VERSION baked in.
    """
    build_dir = os.path.join(source_dir, "build", "metadata")
    os.makedirs(build_dir, exist_ok=True)

    cmake_args = _cmake_args_from_config_settings(config_settings)
    cmd = ["cmake", source_dir] + cmake_args + ["-DSKBUILD=1"]

    print(f"[cgalpy_backend] running cmake configure:", flush=True)
    print(f"[cgalpy_backend]   cmd: {' '.join(cmd)}", flush=True)
    print(f"[cgalpy_backend]   cwd: {build_dir}", flush=True)

    result = subprocess.run(
        cmd,
        cwd=build_dir,
        capture_output=True,
        text=True
    )

    # Always print output so failures are diagnosable
    print(f"[cgalpy_backend] cmake returncode: {result.returncode}", flush=True)
    if result.stdout:
        print(f"[cgalpy_backend] cmake stdout:\n{result.stdout[-2000:]}", flush=True)
    if result.stderr:
        print(f"[cgalpy_backend] cmake stderr:\n{result.stderr[-2000:]}", flush=True)

    if result.returncode != 0:
        raise RuntimeError(
            f"cmake configure failed with return code {result.returncode}"
        )


def _read_metadata_cmake(metadata_cmake, source_dir):
    """
    Run GetMetadata.cmake in script mode (-P) and parse its output.

    GetMetadata.cmake is generated by configure_file and contains:
        message(STATUS "METADATA_VERSION:1.0.10")
        message(STATUS "METADATA_NAME:cgalpy_kerepec_...")

    cmake STATUS messages may go to stdout or stderr depending on the cmake
    version, so we check both.

    Returns (name, version) where either may be None if not found.
    """
    result = subprocess.run(
        ["cmake", "-P", metadata_cmake],
        capture_output=True,
        text=True,
        cwd=source_dir
    )
    name = None
    version = None
    for line in result.stdout.splitlines() + result.stderr.splitlines():
        if "METADATA_NAME:" in line:
            name = line.split("METADATA_NAME:")[1].strip().lower()
        elif "METADATA_VERSION:" in line:
            version = line.split("METADATA_VERSION:")[1].strip()
    return name, version


def _get_cmake_metadata(config_settings=None):
    """
    Orchestrate the cmake configure pass and metadata extraction.

    Fast path: if GetMetadata.cmake already exists (from a prior cmake run or
    a prior pip install invocation), read it directly without re-running cmake.
    This avoids re-running Conan unnecessarily.

    Slow path: if GetMetadata.cmake does not exist, run a cmake configure pass
    to generate it, then read it.

    Returns (name, version), either of which may be None on failure.
    In that case the placeholders in pyproject.toml are used as fallback.
    """
    source_dir = os.path.dirname(os.path.abspath(__file__))
    metadata_cmake = os.path.join(source_dir, "GetMetadata.cmake")

    if not os.path.exists(metadata_cmake):
        print(f"[cgalpy_backend] GetMetadata.cmake not found, running cmake configure...", flush=True)
        try:
            _run_cmake_configure(source_dir, config_settings)
        except Exception as e:
            print(f"[cgalpy_backend] Warning: cmake configure failed: {e}", flush=True)
            return None, None

    if not os.path.exists(metadata_cmake):
        print(
            f"[cgalpy_backend] Warning: cmake configure succeeded but "
            f"GetMetadata.cmake was not generated.",
            flush=True
        )
        return None, None

    try:
        name, version = _read_metadata_cmake(metadata_cmake, source_dir)
        print(f"[cgalpy_backend] name: {name}, version: {version}", flush=True)
        return name, version
    except Exception as e:
        print(f"[cgalpy_backend] Warning: failed to read GetMetadata.cmake: {e}", flush=True)
        return None, None


# ---------------------------------------------------------------------------
# Monkeypatching scikit_build_core
# ---------------------------------------------------------------------------

def _patch_get_standard_metadata(name, version):
    """
    Temporarily replace scikit_build_core.build.metadata.get_standard_metadata
    with a version that injects our cmake-derived name and version into the
    pyproject_dict before the original function processes it.

    Why monkeypatch:
      - PEP 517 forbids 'name' in dynamic[], so we cannot use the hook mechanism.
      - scikit_build_core reads pyproject.toml internally and calls
        get_standard_metadata(pyproject_dict, settings) at line 234 of wheel.py,
        before cmake runs. This is the only clean interception point.
      - get_standard_metadata already does copy.deepcopy internally, so injecting
        into the dict we pass is safe.

    Returns the original function so the caller can restore it in a finally block.
    """
    _original = _meta.get_standard_metadata

    def _patched(pyproject_dict, settings):
        d = copy.deepcopy(dict(pyproject_dict))
        if name:
            d["project"]["name"] = name
        if version:
            # Provide version statically; remove from dynamic[] if present
            d["project"]["version"] = version
            d["project"].pop("dynamic", None)
        return _original(d, settings)

    _meta.get_standard_metadata = _patched
    return _original


# ---------------------------------------------------------------------------
# PEP 517 hooks
# ---------------------------------------------------------------------------

def _with_metadata_override(func, *args, config_settings=None, **kwargs):
    """
    Wrapper that injects cmake-derived metadata around a PEP 517 hook call.

    Both prepare_metadata_for_build_wheel and build_wheel must see identical
    name and version. If only one were patched, scikit_build_core would raise
    'AssertionError: Metadata mismatch'.

    The monkeypatch is always restored in the finally block, even if the
    wrapped function raises an exception.
    """
    name, version = _get_cmake_metadata(config_settings)
    original = None
    if name or version:
        original = _patch_get_standard_metadata(name, version)
    try:
        return func(*args, config_settings=config_settings, **kwargs)
    finally:
        if original is not None:
            _meta.get_standard_metadata = original


def prepare_metadata_for_build_wheel(metadata_directory, config_settings=None):
    """
    PEP 517 hook: produces the .dist-info metadata directory before the wheel
    is built. pip calls this first to get the package name and version.
    """
    return _with_metadata_override(
        _inner.prepare_metadata_for_build_wheel,
        metadata_directory,
        config_settings=config_settings
    )


def build_wheel(wheel_directory, config_settings=None, metadata_directory=None):
    """
    PEP 517 hook: builds the wheel. Must produce metadata consistent with
    prepare_metadata_for_build_wheel, hence the same override is applied.
    """
    return _with_metadata_override(
        _inner.build_wheel,
        wheel_directory,
        config_settings=config_settings,
        metadata_directory=metadata_directory
    )
