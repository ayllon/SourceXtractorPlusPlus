# -*- coding: utf-8 -*-

# Copyright © 2019 Université de Genève, LMU Munich - Faculty of Physics, IAP-CNRS/Sorbonne Université
#
# This library is free software; you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by the Free
# Software Foundation; either version 3.0 of the License, or (at your option)
# any later version.
#
# This library is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
from __future__ import division, print_function

import sys

from .model_fitting import ParameterBase
from .aperture import Aperture

_used_names = set()
model_fitting_parameter_columns = []
aperture_columns = []

_type_column_map = {
    ParameterBase : model_fitting_parameter_columns,
    Aperture: aperture_columns
}


def print_output_columns(file=sys.stderr):
    """
    Print a human-readable representation of the configured output columns.

    Parameters
    ----------
    file : file object
        Where to print the representation. Defaults to sys.stderr
    """
    if model_fitting_parameter_columns:
        print('Model fitting parameter outputs:', file=file)
        for n, ids in model_fitting_parameter_columns:
            print('    {} : {}'.format(n, ids), file=file)
    if aperture_columns:
        print('Aperture outputs:', file=file)
        for n, ids in aperture_columns:
            print('    {} : {}'.format(n, ids), file=file)


def add_output_column(name, params):
    """
    Add a new set of columns to the output catalog.

    Parameters
    ----------
    name : str
        Name/prefix of the new set of columns
    params : list of columns
        List of properties to add to the output with the given name/prefix. They must be subtype
        of one of the known ones: ParameterBase for model fitting, or Aperture for aperture photometry.

    Raises
    ------
    ValueError
        If the name has already been used
    TypeError
        If any of the parameters are not of a known type (see params)

    See Also
    --------
    aperture.add_aperture_photometry
    model_fitting.ParameterBase
    """
    if name in _used_names:
        raise ValueError('Column {} is already set'.format(name))
    _used_names.add(name)

    if not isinstance(params, list):
        params = [params]
    param_type = type(params[0])

    known_subclass = False
    for base in _type_column_map:
        if issubclass(param_type, base):
            _type_column_map[base].append((name, params))
            known_subclass = True

    if not known_subclass:
        raise TypeError('{} is not a known column type'.format(str(param_type)))
