import re

# -------------------------------
# Variable Definition
# -------------------------------
def create_variable_definition(def_id, var_name, stmt, line_num, block_id):
    return {
        "defId": def_id,
        "variableName": var_name,
        "sourceStatement": stmt,
        "lineNumber": line_num,
        "containingBlock": block_id
    }

# -------------------------------
# Code Statement
# -------------------------------
def create_code_statement(content, line_num, stmt_type="Stmt", associated_defs=None):
    if associated_defs is None:
        associated_defs = []
    return {
        "content": content,
        "lineNumber": line_num,
        "statementType": stmt_type,
        "associatedDefs": associated_defs
    }

# -------------------------------
# Control Flow Edge
# -------------------------------
def create_control_flow_edge(destination_block, label=None):
    return {"destinationBlock": destination_block, "edgeLabel": label}

# -------------------------------
# Code Block
# -------------------------------
def create_code_block(block_id):
    return {
        "blockId": block_id,
        "codeStatements": [],
        "outgoingEdges": [],
        "incomingBlocks": set(),
        "generatedDefs": [],
        "killedDefs": set(),
        "reachingIn": set(),
        "reachingOut": set(),
        "allowAppending": True,
        "isTerminated": False
    }

def append_statement(block, stmt):
    block["codeStatements"].append(stmt)

def connect_blocks(source_block, target_block, label=None):
    source_block["outgoingEdges"].append(create_control_flow_edge(target_block["blockId"], label))
    target_block["incomingBlocks"].add(source_block["blockId"])
    source_block["allowAppending"] = False

# -------------------------------
# CFG
# -------------------------------
def create_cfg(blocks, var_defs, start_block, terminate_blocks):
    return {
        "codeBlocks": blocks,
        "variableDefinitions": var_defs,
        "startingBlockId": start_block,
        "terminatingBlockIds": terminate_blocks
    }

def block_lookup(cfg):
    return {blk["blockId"]: blk for blk in cfg["codeBlocks"]}

# -------------------------------
# Control Flow Detection
# -------------------------------
COND_PATTERN = re.compile(r'^\s*(if|else\s*if|else|while|for)\b')
JUMP_PATTERN = re.compile(r'^\s*(return|break|continue|goto)\b')

def is_conditional(stmt):
    return bool(COND_PATTERN.match(stmt))

def is_jump(stmt):
    return bool(JUMP_PATTERN.match(stmt))

# -------------------------------
# Source Processing
# -------------------------------
def process_source_lines(source, start_line=1):
    lines = []
    line_num = start_line
    for raw in source.splitlines():
        line = raw.strip()
        if line and line not in {"{", "}", ";"}:
            lines.append((line, line_num))
        line_num += 1
    return lines

def identify_leaders(statements):
    leaders = {0} if statements else set()
    n = len(statements)
    for idx, (stmt, _) in enumerate(statements):
        if is_conditional(stmt):
            leaders.update({idx, idx + 1})
        if is_jump(stmt) and idx + 1 < n:
            leaders.add(idx + 1)
        if stmt.startswith("else"):
            leaders.add(idx)
    return sorted(leaders)

def partition_blocks(statements, leader_indices):
    partitions = []
    for i, start in enumerate(leader_indices):
        end = leader_indices[i + 1] if i + 1 < len(leader_indices) else len(statements)
        partitions.append((f"B{i}", start, statements[start:end]))
    return partitions

# -------------------------------
# Variable Parsing
# -------------------------------
def parse_defined_vars(stmt):
    stmt = stmt.strip().rstrip(";")
    vars_set = set()

    # Declaration
    decl = re.match(r"(int|float|double|char|long|short|unsigned|signed)\b(.*)", stmt)
    if decl:
        for part in decl.group(2).split(","):
            var = re.split(r"\s|=|\[", part.strip())[0]
            if var:
                vars_set.add(var.lstrip("*"))

    # Assignment
    assign = re.match(r"([A-Za-z_][A-Za-z0-9_\.]*)\s*([+\-*/%&|^]?=)", stmt)
    if assign:
        vars_set.add(assign.group(1))

    # Increment/decrement
    post_inc = re.match(r"([A-Za-z_][A-Za-z0-9_\.]*)\s*(\+\+|--)$", stmt)
    pre_inc = re.match(r"(\+\+|--)\s*([A-Za-z_][A-Za-z0-9_\.]*)$", stmt)
    if post_inc:
        vars_set.add(post_inc.group(1))
    if pre_inc:
        vars_set.add(pre_inc.group(2))

    return vars_set

# -------------------------------
# Build CFG from Source
# -------------------------------
def construct_control_flow_graph(source):
    block_counter = 0
    def_counter = 0
    blocks = []
    var_defs = {}
    var_to_defs = {}

    lines = process_source_lines(source)
    if not lines:
        blk = create_code_block(f"B{block_counter}")
        blocks.append(blk)
        return create_cfg(blocks, var_defs, blk["blockId"], {blk["blockId"]})

    leaders = identify_leaders(lines)
    metadata = partition_blocks(lines, leaders)
    block_map = {}

    for idx, (blk_name, _, stmts) in enumerate(metadata):
        blk = create_code_block(blk_name)
        block_map[blk_name] = blk
        for stmt_text, line_num in stmts:
            defs = []
            for var in parse_defined_vars(stmt_text):
                def_id = f"D{def_counter}"
                def_counter += 1
                var_defs[def_id] = create_variable_definition(def_id, var, stmt_text, line_num, blk_name)
                var_to_defs.setdefault(var, set()).add(def_id)
                defs.append(def_id)
            append_statement(blk, create_code_statement(stmt_text, line_num, "Stmt", defs))
        blocks.append(blk)

    # Sequential connections
    for i in range(len(blocks) - 1):
        connect_blocks(blocks[i], blocks[i + 1])

    start_id = metadata[0][0]
    terminate_ids = {blk["blockId"] for blk in blocks if not blk["isTerminated"]}
    return create_cfg(blocks, var_defs, start_id, terminate_ids)
