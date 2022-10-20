#!/usr/bin/env python3
"""
  (C) Copyright 2022 Intel Corporation.

  SPDX-License-Identifier: BSD-2-Clause-Patent
"""

import sys
import os
from argparse import ArgumentParser


class PermissionFailure(Exception):
    '''Base Exception class'''


def verify(paths, positive=None, negative=None):
    '''Verify positive and/or negative rwx permissions.

    Args:
        paths (list): list of file and/or directory paths to verify
        positive (str, optional): rwx permissions expected to pass. Default is None
        negative (str, optional): rwx permissions expected to fail. Default is None

    Raises:
        ValueError: on invalid input
        FileNotFoundError: if a path does not exist
        PermissionFailure: if permissions are not as expected

    '''
    perms = _parse_perm_str(positive, negative)
    verify_matrix = {
        'r': lambda _path: os.access(_path, os.R_OK),
        'w': lambda _path: os.access(_path, os.W_OK),
        'x': lambda _path: os.access(_path, os.X_OK)
    }
    for path in paths:
        if not os.path.exists(path):
            raise FileNotFoundError(f'Not found: {path}')

        for perm, expect_pass in perms.items():
            print(f'Verifying "{perm}" {"succeeds" if expect_pass else "fails"} on "{path}"')
            success = verify_matrix[perm](path)
            if success and not expect_pass:
                raise PermissionFailure(f'Expected "{perm}" to fail on "{path}"')
            if not success and expect_pass:
                raise PermissionFailure(f'Expected "{perm}" to pass on "{path}"')


def _parse_perm_str(positive=None, negative=None):
    '''Parse rwx permissions from a string.

    Args:
        positive (str, optional): rwx permissions expected to pass. Default is None
        negative (str, optional): rwx permissions expected to fail. Default is None

    Raises:
        ValueError: on invalid input

    Returns:
        dict: 'r', 'w', 'x' mapped to True/False for positive/negative

    '''
    if not positive and not negative:
        raise ValueError('Must specify positive or negative permissions')

    perms = {}
    for string, expect_pass in ((positive, True), (negative, False)):
        for perm in string or '':
            perm = perm.lower()
            if perm not in 'rwx':
                raise ValueError(f'Permission "{perm}" unknown')
            perms[perm] = expect_pass
    return perms


def main():
    '''main execution of this program'''
    parser = ArgumentParser()
    parser.add_argument(
        "paths",
        action="append",
        type=str,
        help="path(s) to verify permissions of")
    parser.add_argument(
        '-p', '--positive',
        type=str,
        help='permissions in RWX that should succeed')
    parser.add_argument(
        '-n', '--negative',
        type=str,
        help='permissions in RWX that should fail')
    args = parser.parse_args()
    verify(args.paths, args.positive, args.negative)

    return 0


if __name__ == '__main__':
    sys.exit(main())
