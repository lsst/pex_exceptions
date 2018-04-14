"""Sphinx configuration file for an LSST stack package.

This configuration only affects single-package Sphinx documenation builds.
"""

from documenteer.sphinxconfig.stackconf import build_package_configs
import lsst.pex.exceptions


_g = globals()
_g.update(build_package_configs(
    project_name='pex_exceptions',
    version=lsst.pex.exceptions.version.__version__))
