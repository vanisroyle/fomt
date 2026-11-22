import pathlib
import re
import sys


def select_section(source, section):
    match = re.search(
        r"^\s*\.section\s+" + re.escape(section)
        + r"[,\s][\s\S]*?(?=^\s*\.(?:section|text)\b|\Z)",
        source,
        re.MULTILINE,
    )
    if match is None:
        raise ValueError("Missing function section: " + section)
    return "\t.code 16\n" + match[0]


if __name__ == "__main__":
    path = pathlib.Path(sys.argv[1])
    path.write_text(select_section(path.read_text(), sys.argv[2]))
