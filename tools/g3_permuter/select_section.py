import pathlib
import re
import sys


def select_section(source, section):
    matches = re.finditer(
        r'^[ \t]*\.section[ \t]+(?:"([^"]+)"|([^,\s]+))[^\n]*\n'
        r'[\s\S]*?(?=^[ \t]*\.(?:section|text)\b|\Z)',
        source,
        re.MULTILINE,
    )
    chunks = [match[0] for match in matches if (match[1] or match[2]) == section]
    if not chunks:
        raise ValueError("Missing function section: " + section)
    return "\t.code 16\n" + "".join(chunks)


if __name__ == "__main__":
    path = pathlib.Path(sys.argv[1])
    path.write_text(select_section(path.read_text(), sys.argv[2]))
