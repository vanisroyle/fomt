import argparse
import pathlib
import re
import subprocess


def extract_function(source, symbol):
    match = re.search(
        r"\tthumb_func_start " + re.escape(symbol)
        + r"\n[\s\S]*?(?=\tthumb_func_start |\t.section |\Z)",
        source,
    )
    if match is None:
        raise ValueError("Not a standalone Thumb entry; inspect its parent: " + symbol)
    return match[0]


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("assembly", type=pathlib.Path)
    parser.add_argument("symbol")
    parser.add_argument("directory", type=pathlib.Path)
    args = parser.parse_args()
    root = pathlib.Path(__file__).resolve().parents[2]
    body = extract_function(args.assembly.read_text(), args.symbol)
    args.directory.mkdir(parents=True, exist_ok=True)
    target = args.directory / "target.s"
    target.write_text(
        '.include "asm/macro.inc"\n.syntax unified\n.text\n' + body
    )
    subprocess.run(
        ["arm-none-eabi-as", "-mcpu=arm7tdmi", "-I", str(root), str(target),
         "-o", str(args.directory / "target.o")],
        check=True,
    )
