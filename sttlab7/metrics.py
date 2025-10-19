import shutil
import subprocess
from pathlib import Path
from typing import Dict, Iterable, List, Optional, Sequence, Set

from cfgBuilder import ControlFlowGraph, VariableDefinition, CodeBlock, ControlFlowEdge, CodeStatement

# -------------------------------
# CFG Visualization Functions
# -------------------------------

def export_dot(flow_graph: ControlFlowGraph, output_path: Path) -> None:
    """Generate DOT format file from a control flow graph."""
    lines: List[str] = ["digraph CFG {", "    node [shape=box];"]

    # Add nodes for each block with their statements
    for block in flow_graph.codeBlocks:
        content_lines = [f"{block.blockId}:"]
        for stmt in block.codeStatements:
            content_lines.append(stmt.content)
        escaped_content = "\n".join(content_lines).replace('"', '\\"')
        lines.append(f"    {block.blockId} [label=\"{escaped_content}\"];")

    # Add edges between blocks
    for block in flow_graph.codeBlocks:
        for edge in block.outgoingEdges:
            label_str = f' [label="{edge.edgeLabel}"]' if edge.edgeLabel else ''
            lines.append(f"    {block.blockId} -> {edge.destinationBlock}{label_str};")

    lines.append("}")
    output_path.write_text("\n".join(lines), encoding="utf-8")


def dot_to_png(dot_path: Path, output_path: Optional[Path] = None) -> Optional[Path]:
    """Convert a DOT file to a PNG image using Graphviz."""
    if output_path is None:
        output_path = dot_path.with_suffix(".png")

    dot_exec = shutil.which("dot")
    if not dot_exec:
        print("Graphviz 'dot' executable not found. Skipping image generation.")
        return None

    try:
        subprocess.run([dot_exec, "-Tpng", str(dot_path), "-o", str(output_path)], check=True)
        return output_path
    except Exception:
        print(f"Failed to convert {dot_path} to PNG.")
        return None


# -------------------------------
# CFG Documentation Functions
# -------------------------------

def export_variable_definitions(var_defs: Dict[str, VariableDefinition], output_path: Path) -> None:
    """Create a markdown table listing all variable definitions."""
    rows: List[str] = [
        "| Definition ID | Variable | Block | Line | Statement |",
        "|---------------|----------|-------|------|-----------|"
    ]

    sorted_defs = sorted(var_defs.values(), key=lambda d: int(d.defId[1:]))
    for definition in sorted_defs:
        stmt_escaped = definition.sourceStatement.replace("|", "\\|")
        rows.append(
            f"| {definition.defId} | {definition.variableName} | "
            f"{definition.containingBlock} | {definition.lineNumber} | `{stmt_escaped}` |"
        )

    output_path.write_text("\n".join(rows), encoding="utf-8")


def format_defs(def_items: Iterable[str]) -> str:
    """Format a set of definitions as a string for display."""
    if not def_items:
        return "{}"
    sorted_items = sorted(def_items, key=lambda x: (x[0], int(x[1:])))
    return "{" + ", ".join(sorted_items) + "}"


def export_iteration_analysis(
    snapshots: Sequence[Dict[str, Dict[str, Set[str]]]],
    flow_graph: ControlFlowGraph,
    output_path: Path
) -> None:
    """Generate a markdown report for each iteration of reaching definitions analysis."""
    report: List[str] = []

    for i, snapshot in enumerate(snapshots):
        report.append(f"## Iteration {i}")
        report.append("| Basic Block | gen[B] | kill[B] | in[B] | out[B] |")
        report.append("|-------------|--------|---------|-------|--------|")

        for block in flow_graph.codeBlocks:
            state = snapshot.get(block.blockId, {})
            report.append(
                "| {block} | {gen} | {kill} | {in_set} | {out_set} |".format(
                    block=block.blockId,
                    gen=format_defs(state.get("gen", set())),
                    kill=format_defs(state.get("kill", set())),
                    in_set=format_defs(state.get("in", set())),
                    out_set=format_defs(state.get("out", set())),
                )
            )

        report.append("")

    output_path.write_text("\n".join(report), encoding="utf-8")
