#!/usr/bin/env python

# Author: jonlabelle
# Source: https://gist.github.com/jonlabelle/dd8c3caa7808cbe4cfe0a47ee4881059

"""Replace line breaks, from one format to another."""

from __future__ import print_function

import argparse
import os
import sys
import glob
import tempfile

from stat import ST_ATIME, ST_MTIME

EOL_UNIX = b'\n'
EOL_WINDOWS = b'\r\n'
EOL_MAC = b'\r'


def _normalize_line_endings(lines, line_ending='unix'):
    r"""Normalize line endings to unix (\n), windows (\r\n) or mac (\r).

    :param lines: The lines to normalize.
    :param line_ending: The line ending format.
    Acceptable values are 'unix' (default), 'windows' and 'mac'.
    :return: Line endings normalized.
    """
    lines = lines.replace(EOL_WINDOWS, EOL_UNIX).replace(EOL_MAC, EOL_UNIX)
    if line_ending == 'windows':
        lines = lines.replace(EOL_UNIX, EOL_WINDOWS)
    elif line_ending == 'mac':
        lines = lines.replace(EOL_UNIX, EOL_MAC)
    return lines


def _copy_file_time(source_file, destination_file):
    """Copy one file's atime and mtime to another.

    :param source_file: Source file.
    :param destination_file: Destination file.
    """
    file1, file2 = source_file, destination_file

    try:
        stat1 = os.stat(file1)
    except os.error:
        sys.stderr.write(file1 + ' : cannot stat\n')
        sys.exit(1)

    try:
        os.utime(file2, (stat1[ST_ATIME], stat1[ST_MTIME]))
    except os.error:
        sys.stderr.write(file2 + ' : cannot change time\n')
        sys.exit(2)


def _create_temp_file(contents):
    """Create a temp file.

    :param contents: The temp file contents.
    :return: The absolute path of the created temp file.
    """
    tf = tempfile.NamedTemporaryFile(
        mode='wb', suffix='txt', delete=False)
    tf.write(contents)
    tf.close()
    return tf.name


def _delete_file_if_exists(filepath):
    """Delete the file if it exists.

    :param filepath: The file path.
    """
    if os.path.exists(filepath):
        os.remove(filepath)


def _read_file_data(filepath):
    """Read file data.

    :param filepath: The file path.
    :return: The file contents.
    """
    data = open(filepath, 'rb').read()
    return data


def _write_file_data(filepath, data):
    """Write file data.

    :param filepath: The file path.
    :param data: The data to write.
    """
    f = open(filepath, 'wb')
    f.write(data)
    f.close()


def main():
    """Main."""
    parser = argparse.ArgumentParser(
        prog='crlf',
        description='Replace CRLF (windows) line endings with LF (unix) line '
                    'endings in files (and vice-versa.')
    parser.add_argument('-q', '--quiet',
                        help='surpress descriptive messages from output',
                        action='store_true',
                        default=False)
    parser.add_argument('-n', '--dryrun',
                        help='show changes, but do not modify files',
                        action='store_true',
                        default=False)
    parser.add_argument('-w', '--windows',
                        help='replace LF (unix) line endings with '
                             'CRLF (windows) line endings',
                        action='store_true',
                        default=False)
    parser.add_argument('-u', '--unix',
                        help='replace CRLF (windows) line endings with '
                             'LF (unix) line endings (default)',
                        action='store_true',
                        default=False)
    parser.add_argument('-t', '--timestamps',
                        help="maintains the modfified file's time stamps "
                             "(atime and mtime)",
                        action='store_true',
                        default=False)
    parser.add_argument('files',
                        nargs='+',
                        help="a list of files or file glob patterns "
                             "to process",
                        default='.')

    if len(sys.argv) < 2:
        parser.print_help()
        sys.exit(2)

    args = parser.parse_args()

    if args.windows is True and args.unix is True:
        sys.stderr.write("Ambiguous options specified, 'unix' and 'windows'. "
                         "Please choose one option or the other.\n")
        sys.exit(2)

    files_to_process = []
    for argfile in args.files:
        files_to_process.extend(glob.glob(argfile))

    if len(files_to_process) <= 0:
        if args.quiet is False:
            sys.stderr.write('No files matched the specified pattern.\n')
        sys.exit(2)

    if args.dryrun is True and args.quiet is False:
        print('Dry run only... files will not be modifed.')

    for file_to_process in files_to_process:
        if os.path.isdir(file_to_process):
            if args.quiet is False:
                print("- '{0}' : is a directory (skip)"
                      .format(file_to_process))
            continue

        if os.path.isfile(file_to_process):
            data = _read_file_data(file_to_process)
            if b'\0' in data:
                if args.quiet is False:
                    print("- '{0}' : is a binary file (skip)".format(
                        file_to_process))
                continue

            if args.windows is True:
                newdata = _normalize_line_endings(data, line_ending='windows')
            else:
                newdata = _normalize_line_endings(data, line_ending='unix')

            if newdata != data:
                if args.quiet is False:
                    if args.windows is True:
                        if args.dryrun is True:
                            print("+ '{0}' : LF would be replaced with CRLF"
                                  .format(file_to_process))
                        else:
                            print("+ '{0}' : replacing LF with CRLF"
                                  .format(file_to_process))
                    else:
                        if args.dryrun is True:
                            print("+ '{0}' : CRLF would be replaced with LF"
                                  .format(file_to_process))
                        else:
                            print("+ '{0}' : replacing CRLF with LF"
                                  .format(file_to_process))

                if args.dryrun is False:
                    try:
                        if args.timestamps is True:
                            # create a temp file with the orignal file
                            # contents and copy the old file's atime a mtime
                            tmp_file_path = _create_temp_file(data)
                            _copy_file_time(file_to_process, tmp_file_path)

                        # overwrite the current file with the modfified
                        # contents
                        _write_file_data(file_to_process, newdata)

                        if args.timestamps is True:
                            # copy the original file's atime and mtime back to
                            # the orignial file w/ the modified contents,
                            # and delete the temp file.
                            _copy_file_time(tmp_file_path, file_to_process)
                            _delete_file_if_exists(tmp_file_path)
                    except Exception as ex:
                        sys.stderr.write('error : {0}\n'.format(str(ex)))
                        sys.exit(1)
            else:
                if args.quiet is False:
                    if args.windows is True:
                        print("- '{0}' : line endings are already CRLF "
                              "(windows)"
                              .format(file_to_process))
                    else:
                        print("- '{0}' : line endings are already LF (unix)"
                              .format(file_to_process))
        else:
            sys.stderr.write("- '{0}' : file not found\n"
                             .format(file_to_process))
            sys.exit(1)


if __name__ == '__main__':
    main()
