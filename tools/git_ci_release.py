#!/usr/bin/env python3

import argparse
import os
import subprocess
import shutil
import sys
import glob
import git_utils



def _gen_tag():
    with open("version.txt", "r") as file:
        return f"{file.read().strip()}"


def _copy_files_to_docs(dst_dir):
    src_dir = 'o'
    
    file_extensions = ['html', 'md']
    for ext in file_extensions:
        files_to_copy = glob.glob(os.path.join(src_dir, f'*.{ext}'))
        if len(files_to_copy) == 0 :
            raise Exception(f"There is no *.{ext} !!:")

        dst_dir_ext =  os.path.join(dst_dir, ext)
        for file in files_to_copy:
            shutil.copy2(file, dst_dir_ext)
    


def _subprocess_run(cmd_line):
    print("executing ", " ".join(cmd_line))
    subprocess.run(cmd_line, check=True)

def _assert_on_root():
    top_level_dir = git_utils.get_git_root()
    current_dir = os.getcwd()

    if  current_dir != top_level_dir:
        print("must be on top level dir !!!")
        sys.exit(1)


def _process_on_main():
    docs = git_utils.get_git_docs_dir()

    git_utils.assert_no_durty()
    #_subprocess_run(["git", "submodule", "update", "--init", "--recursive"])
    #_copy_files_to_docs(docs)

    tag = _gen_tag()
    ci_msg = f"release {tag}"

    #_subprocess_run(["git", "-C", docs, "add", "."])
    #_subprocess_run(["git", "-C", docs, "commit", "-m", ci_msg])

    _subprocess_run(["git", "add", "."])
    _subprocess_run(["git", "commit", "-m", ci_msg])
    #_subprocess_run(["git", "-C", docs, "tag", "-a", tag, "-m", ci_msg])
    #_subprocess_run(["git", "tag", "-a", tag, "-m", ci_msg])


def _release_process():
    _assert_on_root()

    branch_name = git_utils.get_current_branch()

    if branch_name != "main":
        raise Exception("must be on main !!!")

    #docs = git_utils.get_git_docs_dir()

    #branch_name = git_utils.get_current_branch(docs)

    _process_on_main()


def _main():
    parser = argparse.ArgumentParser(description="Script for managing release process.")
    args = parser.parse_args()

    _release_process()


if __name__ == "__main__":
    _main()
