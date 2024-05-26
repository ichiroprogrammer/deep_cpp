import re
import os
import configparser
import shutil
import subprocess
import sys


def get_repo_name():
    output = subprocess.check_output(
        ["git", "config", "--get", "remote.origin.url"]
    ).decode("utf-8")
    repo_name = os.path.basename(output).split(".")[0]
    return repo_name


def get_git_root():
    output = subprocess.check_output(["git", "rev-parse", "--show-toplevel"]).decode(
        "utf-8"
    )
    return output.strip()


def get_git_docs_dir():
    root = get_git_root()
    return os.path.join(root, 'docs')

def get_current_branch(dir = None) :
    if dir :
        output = subprocess.check_output(["git", "-C", dir, "branch", "--show-current"]).decode(
            "utf-8"
        )

    else :
        output = subprocess.check_output(["git", "branch", "--show-current"]).decode(
            "utf-8"
        )
    return output.strip()


def get_git_files(include_submodules):
    cmd = ["git", "ls-files"]
    if include_submodules:
        cmd.append("--recurse-submodules")
    output = subprocess.check_output(cmd).decode("utf-8")
    files = output.splitlines()
    return files


def copy_git_files(src, dst, git_files):
    for file in git_files:
        src_path = os.path.join(src, file)
        dst_path = os.path.join(dst, file)
        os.makedirs(os.path.dirname(dst_path), exist_ok=True)
        if not os.path.isdir(src_path):
            shutil.copy2(src_path, dst_path)


def _is_exclude(exclude, dir) :
    for ex in exclude:
        if dir.startswith(ex) :
            return True;

    return False


def delete_files_except_git(dst, exclude):
    exclude = [ os.path.normpath(os.path.join(dst, ex)) for ex in exclude ]

    for root, dirs, files in os.walk(dst, topdown=False):
        root = os.path.normpath(root)

        if _is_exclude(exclude, root) :
            continue

        for file in files:
            file_path = os.path.normpath(os.path.join(root, file))
            if _is_exclude(exclude, file_path) :
                continue

            os.remove(file_path)

        for dir in dirs:
            dir_path = os.path.normpath(os.path.join(root, dir))
            if _is_exclude(exclude, dir_path) :
                continue

            os.rmdir(dir_path)


def get_submodule_directories(repo_path):
    gitmodules_path = os.path.join(repo_path, ".gitmodules")
    
    if not os.path.exists(gitmodules_path):
        return []
    
    config = configparser.ConfigParser()
    config.read(gitmodules_path)

    submodule_directories = [config.get(section, "path") for section in config.sections()]
    
    return submodule_directories


def assert_on_release(ignore, suffix = ".*"):
    if ignore:
        return

    head_hash = subprocess.check_output(['git', 'rev-parse', 'HEAD'], text=True).strip()
    tags_output = subprocess.check_output(['git', 'tag'], text=True)

    for tag_name in tags_output.splitlines():
        if re.match(rf'^V\d\d\.\d\d{suffix}$', tag_name):

            tag_hash = subprocess.check_output(
                    ['git', 'rev-list', '-n', '1', tag_name], text=True).strip()

            if head_hash == tag_hash:
                return

    print(f"Error: Current HEAD is not for export.")
    print("Use '--ignore-on-release' or '-R' option to ignore this check.")
    sys.exit(1)


def assert_no_durty(dir = None):

    try :
        if dir != None:
            cmd = ["git", "-C", dir, "diff", "--quiet", "--exit-code"]
        else:
            cmd = ["git", "diff", "--quiet", "--exit-code"]

        subprocess.run(cmd, check=True)     # 非0終了なら例外

        cmd.append("--cached")
        subprocess.run(cmd, check=True)     # 非0終了なら例外

        if dir != None:
            cmd = ["git", "-C", dir, "ls-files", "--exclude-standard", "--others"]
        else:
            cmd = ["git", "ls-files", "--exclude-standard", "--others"]
    
    except Exception as e:
        raise f"Error: repository is not clean."

    others = subprocess.check_output(cmd, text=True).strip()

    if len(others) != 0 :
        raise f"Error: repository is not clean."

