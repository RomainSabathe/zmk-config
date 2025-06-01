import re
from typing import List


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
    # Removing the lines that contain comments. This corresponds to the "index"
    # line and should be added manually by the formatting code.
    lines = [line for line in lines if "/*" not in line]

    matrix = []
    # Making the first row and last row of the matrix a comment with the key
    # index, for easier mapping and understanding.
    index_row = [f"/* {i + 1} */" for i in range(14)]
    matrix.append(index_row)

    for i, line in enumerate(lines):
        blocks = line.split("&")[1:]  # removing leading &
        blocks = ["&" + block.strip() for block in blocks]

        # Creating empty slots for the rows of the keyboard that don't
        # have 14 keys (e.g. the thumb row).
        if i in [0, 1, 2]:
            blocks = blocks[:6] + ["", ""] + blocks[6:]
        elif i in [4]:
            blocks = ["", ""] + blocks + ["", ""]

        matrix.append(blocks)

    # Adding the index row at the end as well.
    matrix.append(index_row)

    assert len(matrix) - 2 == 5, (
        f"Expected 5 keyboard rows, got {len(matrix) - 2}"
    )  # minus 2 because we don't want to count the two extra index rows.
    for i, row in enumerate(matrix):
        assert len(row) == 14, (
            f"Expected each keyboard row to have 14 keys, got {len(row)} for row {i}\n{row}"
        )
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
