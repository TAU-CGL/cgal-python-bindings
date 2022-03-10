# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
import os
import sys
sys.path.insert(0, os.path.abspath('@SPHINX_MODULE_DIR@'))

# -- Project information -----------------------------------------------------

project = '@SPHINX_TARGET_NAME@'
copyright = '@SPHINX_TARGET_YEAR@, taucgl'
author = '@SPHINX_TARGET_AUTHORS@'
version = '@SPHINX_TARGET_VERSION_MAJOR@.@SPHINX_TARGET_VERSION_MINOR@'
release = '@SPHINX_TARGET_VERSION@'

# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx.ext.autodoc'
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []

# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'sphinx_rtd_theme'
# html_theme = 'alabaster'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

latex_elements = {
    'preamble': r'''
% A command for temporary escaping underscore; for some reason it doesn't work
% \makeatletter
% \DeclareRobustCommand*{\escapeus}[1]{%
%   \begingroup\@activeus\scantokens{#1\endinput}\endgroup}
% \begingroup\lccode`\~=`\_\relax
%    \lowercase{\endgroup\def\@activeus{\catcode`\_=\active \let~\_}}
% \makeatother
% Instead escape underscore permanently.
% Requires a T1-encoded:
% \usepackage[T1]{fontenc}
% which is the deafult.
\catcode`\_=12
''',
}

# The title may have underscore. One simple way to handle it is to provide
# a string with the underscore characters escaped:
# title = '@SPHINX_TARGET_TITLE@'
title = '@SPHINX_TARGET_NAME@'
latex_documents = [('index', '@SPHINX_TARGET_NAME@.tex', title, author, 'manual')]
