import argparse
from pathlib import Path
from typing import Dict, List

from readFile import load_source_file, clean_source_code, extract_main_body
from cfgBuilder import (
    construct_control_flow_graph,
    perform_reaching_definitions_analysis,
    detect_ambiguous_definitions
)
from metrics import export_dot, dot_to_png, export_variable_definitions, export_iteration_analysis

# -------------------------------
# Program Analysis Functions
# -------------------------------

def analyze_program(program_path: Path, output_dir: Path) -> Dict[str, int]:
    """
    Perform full CFG and reaching definitions analysis for a C source file.
    """
    print(f"[info] Analyzing {program_path.name} ...")

    # Load and clean source code
    raw_source = load_source_file(str(program_path))
    cleaned_source = clean_source_code(raw_source)
    main_body, start_line = extract_main_body(cleaned_source)

    # Construct CFG and perform reaching definitions analysis
    cfg = construct_control_flow_graph(main_body)
    reaching_def_snapshots = perform_reaching_definitions_analysis(cfg)
    ambiguous_vars = detect_ambiguous_definitions(cfg)

    # Compute complexity metrics
    num_nodes = len(cfg["codeBlocks"])
    num_edges = sum(len(block["outgoingEdges"]) for block in cfg["codeBlocks"])
    cyclomatic_complexity = num_edges - num_nodes + 2

    # Prepare output directory for this program
    program_name = program_path.stem
    program_output_dir = output_dir / program_name
    program_output_dir.mkdir(parents=True, exist_ok=True)

    # Generate CFG visualizations
    dot_file = program_output_dir / "cfg.dot"
    export_dot(cfg, dot_file)
    dot_to_png(dot_file)

    # Generate documentation reports
    export_variable_definitions(cfg["variableDefinitions"], program_output_dir / "definitions.md")
    export_iteration_analysis(reaching_def_snapshots, cfg, program_output_dir / "reaching_definitions_iterations.md")

    print(f"[completed] {program_name}: Nodes={num_nodes}, Edges={num_edges}, CC={cyclomatic_complexity}")
    return {
        "program": program_name,
        "nodes": num_nodes,
        "edges": num_edges,
        "cc": cyclomatic_complexity
    }

# -------------------------------
# Summary Report
# -------------------------------

def compile_summary_report(metrics_list: List[Dict[str, int]], report_dir: Path) -> None:
    """
    Generate a markdown summary table for multiple programs' analysis results.
    """
    report_lines = [
        "| Program | Nodes (N) | Edges (E) | Cyclomatic Complexity (CC) |",
        "|---------|-----------|-----------|----------------------------|"
    ]

    for metrics in metrics_list:
        report_lines.append(
            f"| {metrics['program']} | {metrics['nodes']} | "
            f"{metrics['edges']} | {metrics['cc']} |"
        )

    (report_dir / "metrics_summary.md").write_text("\n".join(report_lines), encoding="utf-8")

# -------------------------------
# Command Line Interface
# -------------------------------

def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Perform reaching definitions analysis on one or more C source files."
    )
    parser.add_argument(
        "source_files",
        type=Path,
        nargs="+",
        help="One or more C source files to analyze."
    )
    parser.add_argument(
        "--output-dir",
        type=Path,
        default=Path("Output"),
        help="Directory to save analysis outputs and reports."
    )
    return parser.parse_args()

def main() -> None:
    args = parse_arguments()
    args.output_dir.mkdir(parents=True, exist_ok=True)

    all_metrics = []
    for source_file in args.source_files:
        metrics = analyze_program(source_file, args.output_dir)
        all_metrics.append(metrics)

    compile_summary_report(all_metrics, args.output_dir)

if __name__ == "__main__":
    main()
