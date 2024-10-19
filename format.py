from typing import List
import re


def format_bindings(input_string, custom_fn):
    def replacement(match):
        content = match.group(1)
        return f"bindings = <\n{custom_fn(content)}\n>;"

    return re.sub(
        r"\s\bbindings\s*=\s*<\s*\n([\n+\s\S]*?)>;",
        replacement,
        input_string,
        flags=re.DOTALL,
    )
    keymap_match = re.search(r"keymap\s*{([\s\S]*?)};\s*};", input_string)
    if not keymap_match:
        return input_string  # Return original string if no keymap block found

    keymap_content = keymap_match.group(1)
    processed_content = re.sub(
        r"bindings\s*=\s*<([\s\S]*?)>;", replacement, keymap_content
    )
    return (
        input_string[: keymap_match.start()]
        + f"keymap {{\n{processed_content}\n}}"
        + input_string[keymap_match.end() :]
    )


def align_ampersands(bindings_str: str) -> str:
    matrix = keymap_str_to_matrix(bindings_str)
    n_rows, n_cols = len(matrix), len(matrix[0])

    # Determining the min width between each column.
    max_length = max([max(map(len, line)) for line in matrix])
    max_length_per_column = []
    for j in range(n_cols):
        max_length_per_column.append(max([len(matrix[i][j]) for i in range(n_rows)]))

    for i, line in enumerate(matrix):
        for j, block in enumerate(line):
            matrix[i][j] = block.ljust(max_length_per_column[j])

    formatted_lines = []
    for line in matrix:
        formatted_lines.append(" ".join(line))
    return "\n".join(formatted_lines)


def keymap_str_to_matrix(bindings_str: str) -> List[List[str]]:
    # Remove leading/trailing whitespace and split into lines
    lines = bindings_str.strip().split("\n")

    matrix = []
    for i, line in enumerate(lines):
        blocks = []
        for j, block in enumerate(
            line.split("&")[1:]
        ):  # because the line always start with a "&" sign.
            if i == 4 and j == 0:
                blocks.extend(["", ""])
            blocks.append(block.strip())
            if i in [0, 1, 2] and j == 5:
                blocks.extend(["", ""])
            if i == 4 and j == 9:
                blocks.extend(["", ""])
        blocks = [f"&{block}" if len(block) > 0 else block for block in blocks]
        matrix.append(blocks)

    assert len(matrix) == 5
    for i, row in enumerate(matrix):
        assert len(row) == 14, f"{row=}, {len(row)=}"
    return matrix


def format_keymap(filepath):
    with open(filepath, "r") as file:
        lines = file.readlines()
    code_str = "".join(lines)
    code_str = format_bindings(code_str, align_ampersands)

    with open(filepath, "w") as file:
        file.writelines(code_str)


if __name__ == "__main__":
    format_keymap("config/sofle.keymap")
