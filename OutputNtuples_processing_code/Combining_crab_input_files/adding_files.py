#!/usr/bin/env python3
import os, sys, glob, argparse, ROOT
ROOT.gROOT.SetBatch(True)

def find_root_files(paths):
    files = []
    for p in paths:
        if os.path.isfile(p) and p.endswith(".root"):
            files.append(os.path.abspath(p))
        else:
            files.extend(glob.glob(os.path.join(p, "**", "*.root"), recursive=True))
    # unique, sorted
    return sorted(set(map(os.path.abspath, files)))

def main():
    ap = argparse.ArgumentParser(description="Merge selected branches from cscRootMaker/Events.")
    ap.add_argument("inputs", nargs="+", help="Top folders or individual .root files")
    ap.add_argument("-o", "--output", required=True, help="Output ROOT file (merged)")
    ap.add_argument("--tree-path", default="cscRootMaker/Events",
                    help="Path to TTree inside file (default: cscRootMaker/Events)")
    ap.add_argument("-b", "--branches", nargs="+",
                    default=["Event", "LumiSect", "Run"],
                    help="Branches to keep (default: Event LumiSect Run)")
    args = ap.parse_args()

    files = find_root_files(args.inputs)
    if not files:
        print("No ROOT files found.", file=sys.stderr)
        sys.exit(2)

    # Build chain by checking tree existence first
    chain = ROOT.TChain(args.tree_path)
    bad = []
    added = 0
    for f in files:
        tf = ROOT.TFile.Open(f, "READ")
        if not tf or tf.IsZombie():
            bad.append((f, "cannot open"))
            continue
        t = tf.Get(args.tree_path)  # e.g. "cscRootMaker/Events"
        if not isinstance(t, ROOT.TTree):
            bad.append((f, f"missing tree '{args.tree_path}'"))
            tf.Close()
            continue
        tf.Close()
        if chain.Add(f, 0) == 0:
            bad.append((f, "Add failed"))
        else:
            added += 1

    if added == 0:
        for f, why in bad:
            print(f"SKIP {f} -> {why}", file=sys.stderr)
        sys.exit(3)

    # Enable only requested branches
    chain.SetBranchStatus("*", 0)
    for br in args.branches:
        chain.SetBranchStatus(br, 1)

    # Write output with only enabled branches
    out_path = os.path.abspath(args.output)
    os.makedirs(os.path.dirname(out_path) or ".", exist_ok=True)
    fout = ROOT.TFile(out_path, "RECREATE")
    if not fout or fout.IsZombie():
        print(f"Cannot create output file: {out_path}", file=sys.stderr)
        sys.exit(4)

    outtree = chain.CloneTree(-1, "fast")
    outtree.Write()
    fout.Close()

    print(f"\n✅ wrote: {out_path}")
    #print(f"entries: {outtree.GetEntries()}")
    print(f"kept   : {', '.join(args.branches)}")
    if bad:
        print("\n⚠️  skipped files:")
        for f, why in bad:
            print(f"  - {f} ({why})")

if __name__ == "__main__":
    main()
