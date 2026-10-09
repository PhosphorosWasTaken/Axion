"""Copyright (C) 2026  PhosphorosWasTaken
Copyright (C) 2026  p123o215

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://gnu.org>.

See the LICENSE file in the project root for additional terms 
appended under GPL v3 Section 7 regarding attribution screens."""


import os

# Define your exact copyright header block
HEADER_TEXT = """Copyright (C) 2026  PhosphorosWasTaken
Copyright (C) 2026  p123o215

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://gnu.org>.

See the LICENSE file in the project root for additional terms 
appended under GPL v3 Section 7 regarding attribution screens."""

# Define file extensions and their comment styles
COMMENT_STYLES = {
    '.c': ('// ', ''),
    '.h': ('// ', ''),
    '.cpp': ('// ', ''),
    '.hpp': ('// ', ''),
    '.S': ('# ', ''),
    '.asm': ('; ', ''),
    '.py': ('# ', ''),
    'Makefile': ('# ', ''),
    ".sh": ("#",""),
    ".bat":("REM ",""),
    ".cmake":("#",""),
    ".ld":("/* "," */")
}

def format_header(text, comment_start, comment_end=""):
    lines = text.strip().split('\n')
    formatted = []
    for line in lines:
        if line.strip() == "":
            formatted.append(comment_start.strip())
        else:
            formatted.append(f"{comment_start}{line}{comment_end}")
    return "\n".join(formatted) + "\n\n"

def inject_header_to_file(file_path, comment_style):
    with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()
    
    # Check if the file already contains the copyright notice to prevent duplication
    if "GNU General Public License" in content or "Copyright (C) 2026" in content:
        print(f"[-] Skipped (Already contains header): {file_path}")
        return

    c_start, c_end = comment_style
    full_header = format_header(HEADER_TEXT, c_start, c_end)
    
    # Inject at the very top
    new_content = full_header + content
    
    with open(file_path, 'w', encoding='utf-8') as f:
        f.write(new_content)
    print(f"[+] Successfully added header to: {file_path}")

def scan_and_inject(root_dir):
    # Exclude directories you don't want to modify (like git or build outputs)
    exclude_dirs = {'.git', 'build', 'bin', 'obj'}
    
    for root, dirs, files in os.walk(root_dir):
        dirs[:] = [d for d in dirs if d not in exclude_dirs]
        
        for file in files:
            file_path = os.path.join(root, file)
            ext = os.path.splitext(file)[1]
            
            # Match by exact filename (like Makefile) or by extension
            if file in COMMENT_STYLES:
                inject_header_to_file(file_path, COMMENT_STYLES[file])
            elif ext in COMMENT_STYLES:
                inject_header_to_file(file_path, COMMENT_STYLES[ext])

if __name__ == "__main__":
    project_root = os.path.dirname(os.path.abspath(__file__))
    print(f"Scanning directory: {project_root}")
    scan_and_inject(project_root)
