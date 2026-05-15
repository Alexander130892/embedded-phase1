"""
update_headers.py

Reads a list of .c/.h files, calls the Claude API to generate a one-line
description, then inserts or replaces the standard file-header block.

Usage:
    python update_headers.py <file_listing.txt>

Environment variables:
    ANTHROPIC_API_KEY  – required
    AUTHOR_NAME        – optional, falls back to "Alexander Vindelinckx"
"""

import os
import re
import sys
import json
import time
from datetime import date
import requests

# ---------------------------------------------------------------------------
# Configuration
# ---------------------------------------------------------------------------

AUTHOR      = os.environ.get("AUTHOR_NAME", "Alexander Vindelinckx")
API_KEY     = os.environ.get("ANTHROPIC_API_KEY", "")
API_URL     = "https://api.anthropic.com/v1/messages"
MODEL       = "claude-sonnet-4-6"

# Matches the full header block so we can replace it on subsequent runs
HEADER_RE = re.compile(
    r"^/\*\s*\n"           # opening /*
    r"(?:\s*\*[^\n]*\n)*"  # any number of  " * ..." lines
    r"\s*\*/\s*\n",        # closing */
    re.MULTILINE,
)

# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def generate_description(filename: str, source: str) -> str:
    """Ask Claude for a concise one-to-two sentence description of the file.

    Retries on transient errors (429 rate-limit, 529 overloaded) with
    exponential backoff: waits 5s, 10s, 20s, 40s before giving up.
    """
    if not API_KEY:
        return "No description available (ANTHROPIC_API_KEY not set)."

    # Strip any existing header so the model sees only the actual code
    clean_source = HEADER_RE.sub("", source, count=1).strip()

    prompt = (
        f"You are reading a C source file named '{filename}'.\n"
        f"Write a concise 1–2 sentence description of what this file does. "
        f"Focus on its purpose and any notable behaviour. "
        f"Reply with the description only — no preamble, no markdown.\n\n"
        f"```c\n{clean_source[:4000]}\n```"   # cap at 4 000 chars to stay lean
    )

    RETRYABLE = {429, 529}
    MAX_RETRIES = 8
    delay = 5  # seconds; doubles each attempt

    for attempt in range(MAX_RETRIES + 1):
        response = requests.post(
            API_URL,
            headers={
                "x-api-key": API_KEY,
                "anthropic-version": "2023-06-01",
                "content-type": "application/json",
            },
            json={
                "model": MODEL,
                "max_tokens": 200,
                "messages": [{"role": "user", "content": prompt}],
            },
            timeout=30,
        )

        if response.status_code in RETRYABLE and attempt < MAX_RETRIES:
            print(f"     ⚠ API returned {response.status_code} — "
                  f"retrying in {delay}s (attempt {attempt + 1}/{MAX_RETRIES}) …")
            time.sleep(delay)
            delay *= 2
            continue

        response.raise_for_status()
        data = response.json()
        return data["content"][0]["text"].strip()

    # Should never reach here, but keeps the type-checker happy
    raise RuntimeError("Exhausted all retries calling the Anthropic API.")


def wrap_description(description: str, prefix: str = " *   ") -> str:
    """
    Word-wrap a description to ~72 chars per line, each prefixed with ' *   '.
    """
    words  = description.split()
    lines  = []
    line   = prefix

    for word in words:
        if len(line) + len(word) + 1 > 72:
            lines.append(line.rstrip())
            line = prefix + word + " "
        else:
            line += word + " "

    if line.strip():
        lines.append(line.rstrip())

    return "\n".join(lines)


def build_header(filename: str, description: str) -> str:
    today       = date.today().strftime("%-d-%-m-%Y")   # e.g. 18-3-2026
    desc_block  = wrap_description(description)

    return (
        f"/*\n"
        f" * File:    {filename}\n"
        f" * Author:  {AUTHOR}\n"
        f" * Date:    {today}\n"
        f" *\n"
        f" * Description:\n"
        f"{desc_block}\n"
        f" */\n"
    )


def update_file(path: str) -> bool:
    """Return True if the file was actually modified."""
    filename = os.path.basename(path)

    with open(path, "r", encoding="utf-8", errors="replace") as fh:
        original = fh.read()

    print(f"  → Generating description for {filename} …")
    description = generate_description(filename, original)
    print(f"     {description}")

    new_header = build_header(filename, description)

    # Replace existing header, or prepend a fresh one
    if HEADER_RE.match(original):
        updated = HEADER_RE.sub(new_header, original, count=1)
    else:
        updated = new_header + "\n" + original

    if updated == original:
        return False

    with open(path, "w", encoding="utf-8") as fh:
        fh.write(updated)

    return True


# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------

def main(listing: str) -> None:
    with open(listing) as fh:
        files = [line.strip() for line in fh if line.strip()]

    if not files:
        print("No .c/.h files to update.")
        return

    changed = 0
    for path in files:
        if not os.path.isfile(path):
            print(f"Skipping (not found): {path}")
            continue
        print(f"Processing: {path}")
        if update_file(path):
            print(f"  ✓ Header updated.\n")
            changed += 1
        else:
            print(f"  – No change needed.\n")

    print(f"Done. {changed}/{len(files)} file(s) updated.")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(f"Usage: python {sys.argv[0]} <file_listing.txt>")
        sys.exit(1)
    main(sys.argv[1])