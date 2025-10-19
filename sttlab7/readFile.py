import re
from typing import Tuple

# -------------------------------
# Source Code Processing Functions
# -------------------------------

def load_source_file(file_path: str) -> str:
    """
    Read the full content of a C source file.

    Args:
        file_path (str): Path to the C source file.

    Returns:
        str: Full source code as a string.

    Raises:
        ValueError: If the file does not exist.
    """
    try:
        with open(file_path, "r", encoding="utf-8") as f:
            return f.read()
    except FileNotFoundError:
        raise ValueError(f"Source file not found: {file_path}")


def clean_source_code(raw_source: str) -> str:
    """
    Remove comments and preprocessor directives from C source code.

    Args:
        raw_source (str): Raw C source code.

    Returns:
        str: Cleaned source code without comments or preprocessor lines.
    """
    # Remove block comments
    source_no_block_comments = re.sub(r"/\*.*?\*/", "", raw_source, flags=re.DOTALL)
    # Remove line comments
    source_no_line_comments = re.sub(r"//.*", "", source_no_block_comments)

    cleaned_lines = []
    for line in source_no_line_comments.splitlines():
        if line.lstrip().startswith("#"):
            cleaned_lines.append("")  # Replace preprocessor directives with empty line
        else:
            cleaned_lines.append(line)

    return "\n".join(cleaned_lines)


def locate_main_function(source_code: str) -> Tuple[int, int]:
    """
    Identify the start and end indices of the 'main' function in the source code.

    Args:
        source_code (str): C source code.

    Returns:
        Tuple[int, int]: (start_index, end_index) of the main function body.

    Raises:
        ValueError: If 'main' function signature or braces are invalid/missing.
    """
    # Match standard main function signatures
    match = re.search(r"\bint\s+main\s*\([^)]*\)\s*\{", source_code)
    if not match:
        match = re.search(r"\bmain\s*\([^)]*\)\s*\{", source_code)
    if not match:
        raise ValueError("Main function signature not detected in source code.")

    # Locate opening brace
    open_brace_index = source_code.find("{", match.end() - 1)
    if open_brace_index == -1:
        raise ValueError("Main function structure invalid: missing opening brace.")

    # Track nested braces to find corresponding closing brace
    nesting = 0
    pos = open_brace_index
    while pos < len(source_code):
        char = source_code[pos]
        if char == "{":
            nesting += 1
        elif char == "}":
            nesting -= 1
            if nesting == 0:
                return open_brace_index + 1, pos
        pos += 1

    raise ValueError("Main function structure invalid: missing closing brace.")


def extract_main_body(source_code: str) -> Tuple[str, int]:
    """
    Extract the body of the 'main' function along with its starting line number.

    Args:
        source_code (str): C source code.

    Returns:
        Tuple[str, int]: (main function body, starting line number)
    """
    start_idx, end_idx = locate_main_function(source_code)
    main_body = source_code[start_idx:end_idx]
    starting_line = source_code[:start_idx].count("\n") + 1
    return main_body, starting_line
