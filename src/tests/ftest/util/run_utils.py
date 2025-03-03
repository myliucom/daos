"""
  (C) Copyright 2022 Intel Corporation.

  SPDX-License-Identifier: BSD-2-Clause-Patent
"""
from socket import gethostname
import subprocess   # nosec

from ClusterShell.NodeSet import NodeSet
from ClusterShell.Task import task_self


class RunException(Exception):
    """Base exception for this module."""


class RemoteCommandResult():
    """Stores the command result from a Task object."""

    class ResultData():
        # pylint: disable=too-few-public-methods
        """Command result data for the set of hosts."""

        def __init__(self, command, returncode, hosts, stdout, timeout):
            """Initialize a ResultData object.

            Args:
                command (str): the executed command
                returncode (int): the return code of the executed command
                hosts (NodeSet): the host(s) on which the executed command yielded this result
                stdout (list): the result of the executed command split by newlines
                timeout (bool): indicator for a command timeout
            """
            self.command = command
            self.returncode = returncode
            self.hosts = hosts
            self.stdout = stdout
            self.timeout = timeout

    def __init__(self, command, task):
        """Create a RemoteCommandResult object.

        Args:
            command (str): command executed
            task (Task): object containing the results from an executed clush command
        """
        self.output = []
        self._process_task(task, command)

    @property
    def homogeneous(self):
        """Did all the hosts produce the same output.

        Returns:
            bool: if all the hosts produced the same output

        """
        return len(self.output) == 1

    @property
    def passed(self):
        """Did the command pass on all the hosts.

        Returns:
            bool: if the command was successful on each host

        """
        all_zero = all(data.returncode == 0 for data in self.output)
        return all_zero and not self.timeout

    @property
    def timeout(self):
        """Did the command timeout on any hosts.

        Returns:
            bool: True if the command timed out on at least one set of hosts; False otherwise

        """
        return any(data.timeout for data in self.output)

    def _process_task(self, task, command):
        """Populate the output list and determine the passed result for the specified task.

        Args:
            task (Task): a ClusterShell.Task.Task object for the executed command
            command (str): the executed command
        """
        # Get a dictionary of host list values for each unique return code key
        results = dict(task.iter_retcodes())

        # Get a list of any hosts that timed out
        timed_out = [str(hosts) for hosts in task.iter_keys_timeout()]

        # Populate the a list of unique output for each NodeSet
        for code in sorted(results):
            output_data = list(task.iter_buffers(results[code]))
            if not output_data:
                output_data = [["<NONE>", results[code]]]
            for output, output_hosts in output_data:
                # In run_remote(), task.run() is executed with the stderr=False default.
                # As a result task.iter_buffers() will return combined stdout and stderr.
                stdout = []
                for line in output.splitlines():
                    if isinstance(line, bytes):
                        stdout.append(line.decode("utf-8"))
                    else:
                        stdout.append(line)
                self.output.append(
                    self.ResultData(command, code, NodeSet.fromlist(output_hosts), stdout, False))
        if timed_out:
            self.output.append(
                self.ResultData(command, 124, NodeSet.fromlist(timed_out), None, True))

    def log_output(self, log):
        """Log the command result.

        Args:
            log (logger): logger for the messages produced by this method

        """
        for data in self.output:
            if data.timeout:
                log.debug("  %s (rc=%s): timed out", str(data.hosts), data.returncode)
            elif len(data.stdout) == 1:
                log.debug("  %s (rc=%s): %s", str(data.hosts), data.returncode, data.stdout[0])
            else:
                log.debug("  %s (rc=%s):", str(data.hosts), data.returncode)
                for line in data.stdout:
                    log.debug("    %s", line)


def get_clush_command_list(hosts, args=None, sudo=False):
    """Get the clush command with optional sudo arguments.

    Args:
        hosts (NodeSet): hosts with which to use the clush command
        args (str, optional): additional clush command line arguments. Defaults
            to None.
        sudo (bool, optional): if set the clush command will be configured to
            run a command with sudo privileges. Defaults to False.

    Returns:
        list: list of the clush command

    """
    command = ["clush", "-w", str(hosts)]
    if args:
        command.insert(1, args)
    if sudo:
        # If ever needed, this is how to disable host key checking:
        # command.extend(["-o", "-oStrictHostKeyChecking=no", "sudo"])
        command.append("sudo")
    return command


def get_clush_command(hosts, args=None, sudo=False):
    """Get the clush command with optional sudo arguments.

    Args:
        hosts (NodeSet): hosts with which to use the clush command
        args (str, optional): additional clush command line arguments. Defaults
            to None.
        sudo (bool, optional): if set the clush command will be configured to
            run a command with sudo privileges. Defaults to False.

    Returns:
        str: the clush command

    """
    return " ".join(get_clush_command_list(hosts, args, sudo))


def get_local_host():
    """Get the local host name.

    Returns:
        str: name of the local host
    """
    return gethostname().split(".")[0]


def run_local(log, command, capture_output=True, timeout=None, check=False, verbose=True):
    """Run the command locally.

    Args:
        log (logger): logger for the messages produced by this method
        command (list): command from which to obtain the output
        capture_output(bool, optional): whether or not to include the command output in the
            subprocess.CompletedProcess.stdout returned by this method. Defaults to True.
        timeout (int, optional): number of seconds to wait for the command to complete.
            Defaults to None.
        check (bool, optional): if set the method will raise an exception if the command does not
            yield a return code equal to zero. Defaults to False.
        verbose (bool, optional): if set log the output of the command (capture_output must also be
            set). Defaults to True.

    Raises:
        RunException: if the command fails: times out (timeout must be specified),
            yields a non-zero exit status (check must be True), is interrupted by the user, or
            encounters some other exception.

    Returns:
        subprocess.CompletedProcess: an object representing the result of the command execution with
            the following properties:
                - args (the command argument)
                - returncode
                - stdout (only set if capture_output=True)
                - stderr (not used; included in stdout)

    """
    local_host = get_local_host()
    command_str = " ".join(command)
    kwargs = {"encoding": "utf-8", "shell": False, "check": check, "timeout": timeout}
    if capture_output:
        kwargs["stdout"] = subprocess.PIPE
        kwargs["stderr"] = subprocess.STDOUT
    if timeout:
        log.debug("Running on %s with a %s timeout: %s", local_host, timeout, command_str)
    else:
        log.debug("Running on %s: %s", local_host, command_str)

    try:
        # pylint: disable=subprocess-run-check
        result = subprocess.run(command, **kwargs)

    except subprocess.TimeoutExpired as error:
        # Raised if command times out
        log.debug(str(error))
        log.debug("  output: %s", error.output)
        log.debug("  stderr: %s", error.stderr)
        raise RunException(f"Command '{command_str}' exceed {timeout}s timeout") from error

    except subprocess.CalledProcessError as error:
        # Raised if command yields a non-zero return status with check=True
        log.debug(str(error))
        log.debug("  output: %s", error.output)
        log.debug("  stderr: %s", error.stderr)
        raise RunException(f"Command '{command_str}' returned non-zero status") from error

    except KeyboardInterrupt as error:
        # User Ctrl-C
        message = f"Command '{command_str}' interrupted by user"
        log.debug(message)
        raise RunException(message) from error

    except Exception as error:
        # Catch all
        message = f"Command '{command_str}' encountered unknown error"
        log.debug(message)
        log.debug(str(error))
        raise RunException(message) from error

    if capture_output and verbose:
        # Log the output of the command
        log.debug("  %s (rc=%s):", local_host, result.returncode)
        if result.stdout:
            for line in result.stdout.splitlines():
                log.debug("    %s", line)

    return result


def run_remote(log, hosts, command, verbose=True, timeout=120, task_debug=False):
    """Run the command on the remote hosts.

    Args:
        log (logger): logger for the messages produced by this method
        hosts (NodeSet): hosts on which to run the command
        command (str): command from which to obtain the output
        verbose (bool, optional): log the command output. Defaults to True.
        timeout (int, optional): number of seconds to wait for the command to complete.
            Defaults to 120 seconds.
        task_debug (bool, optional): whether to enable debug for the task object. Defaults to False.

    Returns:
        RemoteCommandResult: a grouping of the command results from the same hosts with the same
            return status

    """
    task = task_self()
    if task_debug:
        task.set_info('debug', True)
    # Enable forwarding of the ssh authentication agent connection
    task.set_info("ssh_options", "-oForwardAgent=yes")
    log.debug("Running on %s with a %s second timeout: %s", hosts, timeout, command)
    task.run(command=command, nodes=hosts, timeout=timeout)
    results = RemoteCommandResult(command, task)
    if verbose:
        results.log_output(log)
    return results
