"""
  (C) Copyright 2022 Intel Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent
"""
from itertools import chain, combinations

from dfuse_test_base import DfuseTestBase
from daos_utils import DaosCommand


def rwx_all_combos():
    '''Get all combinations of rwx permissions

    Returns:
        list: all rwx string combinations

    '''
    return [''.join(perms) for perms in chain(*map(lambda x: combinations('rwx', x), range(0, 4)))]


def rwx_inverse(rwx):
    '''Get the inverse rwx permissions for a string

    Args:
        rwx (str): string of rwx permissions

    Returns:
        str: the rwx permissions not in rwx

    '''
    return ''.join(inverse for inverse in 'rwx' if inverse not in rwx)


def rwx_lex_to_int(rwx):
    '''Convert an rwx string to the integer-equivalent

    Args:
        rwx (str): the rwx permissions

    Returns:
        int: the integer equivalent

    '''
    mapping = {
        'r': 4,
        'w': 2,
        'x': 1,
        '-': 0
    }
    try:
        return sum(map(mapping.get, rwx))
    except (TypeError, KeyError) as error:
        raise ValueError(f'Invalid rwx string: {rwx}') from error


class DfuseMUPermsBasic(DfuseTestBase):
    """Verify dfuse multi-user basic permissions."""

    def test_dfuse_mu_perms_basic(self):
        """Jira ID: DAOS-10854.

        Test Description:
            Verify dfuse multi-user basic permissions.
        Use cases:
            Create a pool.
            Create a container.
            Mount dfuse in multi-user mode.
            Verify basic permissions for other users.
        :avocado: tags=all,daily_regression
        :avocado: tags=vm
        :avocado: tags=dfuse,dfuse_mu,security
        :avocado: tags=test_dfuse_mu_perms_basic
        """
        client_users = self.params.get('client_users', '/run/*')
        client_users = [group_user.split(':') for group_user in client_users]
        dfuse_user, group_user, other_user = client_users
        root_dir = self.tmp

        for positive, negative in ([positive, rwx_inverse(positive)] for positive in rwx_all_combos()):
            self.log.info('positive=%s, negative=%s', positive, negative)
            self.log.info('chmod positive = %s', rwx_lex_to_int(positive))

        self.log.info('root_dir = %s', root_dir)

        # Create a pool and give dfuse_user access
        pool = self.get_pool(connect=False)
        pool.update_acl(False, f'A::{dfuse_user[0]}@:rw')

        # Create a container as dfuse_user
        daos_command = DaosCommand(self.bin)
        daos_command.run_as_user = dfuse_user[0]
        cont = self.get_container(pool, daos_command=daos_command)

        # Run dfuse as dfuse_user
        self.load_dfuse(self.hostlist_clients)
        self.dfuse.run_as_user = dfuse_user[0]
        self.start_dfuse(self.hostlist_clients, pool, cont)
