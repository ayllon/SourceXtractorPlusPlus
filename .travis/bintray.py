#!/usr/bin/env python3
# Helper tool for dealing with Bintray API
import logging
import os
import re
import sys
from argparse import ArgumentParser
from html.parser import HTMLParser

import requests
from requests.auth import HTTPBasicAuth

logger = logging.getLogger('bintray')


class Bintray(object):
    """
    Bintray API wrapper
    """
    CONTENT_URL = "https://api.bintray.com/content/"
    LIST_URL = "https://dl.bintray.com/"
    PROTECT = ['master', 'develop']

    def __init__(self, user, token):
        self.__user = user
        self.__token = token

    def listdir(self, path):
        """
        List all files in path
        """

        class ListParser(HTMLParser):
            def __init__(self):
                super(ListParser, self).__init__()
                self.__tags = []
                self.entries = []

            def handle_starttag(self, tag, attrs):
                self.__tags.append(tag)

            def handle_endtag(self, tag):
                self.__tags.pop(-1)

            def handle_data(self, data):
                if self.__tags and self.__tags[-1] == 'a':
                    self.entries.append(data)

        parser = ListParser()
        parser.feed(requests.get(f'{self.LIST_URL}{path}').text)
        return parser.entries

    def recursive(self, path):
        """
        List recursively all files within path. Does *not* return directories.
        """
        result = []
        dirs = [path]
        while len(dirs):
            p = dirs.pop(0)
            content = bintray.listdir(p)
            for e in content:
                full_path = os.path.join(p, e)
                if args.recursive and e[-1] == '/':
                    dirs.append(full_path)
                else:
                    result.append(full_path)
        return result

    def rm(self, path):
        """
        Remove a file
        """
        r = requests.request(
            'DELETE', f'{self.CONTENT_URL}{path}',
            json=True,
            auth=HTTPBasicAuth(self.__user, self.__token)
        )
        r.raise_for_status()
        response = r.json()
        if response['message'] != 'success':
            raise Exception(response['message'])


def listdir(bintray, args):
    if args.recursive:
        entries = bintray.recursive(args.path)
    else:
        entries = bintray.listdir(args.path)
    for e in entries:
        print(e)


def rm(bintray, args):
    path_components = os.path.split(args.path)
    protected = [p for p in path_components if p in bintray.PROTECT]
    if len(protected):
        if not args.force:
            raise ValueError('Trying to remove protected files! Aborting')
        else:
            answer = input('You are trying to remove protected files. Do you really want to do that? (yes or no)')
            if answer.lower() != 'yes':
                raise ValueError('Trying to remove protected files! Aborting')

    if args.recursive:
        entries = bintray.recursive(args.path)
    else:
        entries = bintray.listdir(args.path)
        entries = [os.path.join(args.path, e) for e in entries]

    regex = re.compile(args.pattern) if args.pattern else None

    for e in entries:
        if e[-1] != '/':
            if not regex or regex.match(e):
                bintray.rm(e)
                print(e)


if __name__ == '__main__':
    try:
        import coloredlogs

        coloredlogs.install(logging.DEBUG)
    except ImportError:
        handler = logging.StreamHandler(sys.stderr)
        handler.setLevel(logging.DEBUG)
        logging.getLogger().addHandler(handler)
        logging.getLogger().setLevel(logging.DEBUG)

    parser = ArgumentParser()
    parser.add_argument('-u', '--user', type=str, default=os.getenv('BINTRAY_USER'), help='Bintray user')
    parser.add_argument('-t', '--token', type=str, default=os.getenv('BINTRAY_TOKEN'), help='Bintray token')
    subparsers = parser.add_subparsers()

    ls_subparser = subparsers.add_parser('ls', description='List directory')
    ls_subparser.add_argument('-r', '--recursive', action='store_true', help='Recursive')
    ls_subparser.add_argument('path')
    ls_subparser.set_defaults(method=listdir)

    rm_subparser = subparsers.add_parser('rm', description='Remove')
    rm_subparser.add_argument('-r', '--recursive', action='store_true', help='Recursive')
    rm_subparser.add_argument('-f', '--force', action='store_true', help='Remove protected files')
    rm_subparser.add_argument('--pattern', type=str, help='Restrict erasing to files matching a regex')
    rm_subparser.add_argument('path')
    rm_subparser.set_defaults(method=rm)

    args = parser.parse_args()
    bintray = Bintray(args.user, args.token)
    try:
        args.method(bintray, args)
    except Exception as e:
        logger.error(str(e))
