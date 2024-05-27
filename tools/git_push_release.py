#!/usr/bin/env python3

import argparse
import os
import subprocess
import shutil
import sys
import glob
import git_utils

def _subprocess_run(cmd_line):
    print("executing ", " ".join(cmd_line))
    subprocess.run(cmd_line, check=True)


def _push_process():
    branch_name = git_utils.get_current_branch()

    if branch_name != "master":
        print("must be on master !!!")
        sys.exit(1)

    docs = git_utils.get_git_docs_dir()
    branch_name = git_utils.get_current_branch(docs)

    if branch_name != "master":
        print("must be on release in docs !!!")
        sys.exit(1)

    #_subprocess_run(["git",  "-C", docs, "push", "HEAD:master", "--follow-tags"])
    _subprocess_run(["git", "push", "--follow-tags"])


def _main():
    parser = argparse.ArgumentParser(description="Script for push with tag.")
    args = parser.parse_args()

    _push_process()


if __name__ == "__main__":
    _main()

