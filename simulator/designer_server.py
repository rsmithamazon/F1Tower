#!/usr/bin/env python3
# ===========================================================
# SPLIT FLAP ARTWORK DESIGNER — LOCAL SERVER
# ===========================================================
# Serves flap-designer.html over http://localhost and provides a
# /save endpoint that writes exported PNGs straight into the
# project's "static assets" folder (lossless, full resolution).
#
# Why: browsers can't write to a fixed filesystem path from a
# file:// page. Serving over localhost lets the designer POST the
# rendered PNG here, and this script saves it for you.
#
# Usage:
#   python3 simulator/designer_server.py
# Then open (in any browser):
#   http://localhost:8000/simulator/flap-designer.html
#
# Ctrl+C to stop.
# ===========================================================

import base64
import json
import os
import re
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer

PROJECT_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ASSETS_DIR = os.path.join(PROJECT_ROOT, "static assets")
PORT = 8000

_DATA_URL_RE = re.compile(r"^data:image/png;base64,(.*)$", re.DOTALL)


class DesignerHandler(SimpleHTTPRequestHandler):
    """Static file server (rooted at project) + POST /save endpoint."""

    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=PROJECT_ROOT, **kwargs)

    # --- helpers ---
    def _send_json(self, code, obj):
        body = json.dumps(obj).encode("utf-8")
        self.send_response(code)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Access-Control-Allow-Origin", "*")
        self.end_headers()
        self.wfile.write(body)

    # --- CORS preflight / capability probe ---
    def do_OPTIONS(self):
        self.send_response(204)
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "POST, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "Content-Type")
        self.end_headers()

    # --- save endpoint ---
    def do_POST(self):
        if self.path.rstrip("/") != "/save":
            self._send_json(404, {"ok": False, "error": "unknown endpoint"})
            return

        try:
            length = int(self.headers.get("Content-Length", 0))
            payload = json.loads(self.rfile.read(length).decode("utf-8"))
        except Exception as e:
            self._send_json(400, {"ok": False, "error": f"bad request: {e}"})
            return

        rel = str(payload.get("path", "")).strip().replace("\\", "/").lstrip("/")

        # Reject path traversal; keep everything inside static assets.
        if not rel or ".." in rel.split("/"):
            self._send_json(400, {"ok": False, "error": "invalid path"})
            return

        text = payload.get("text")
        if text is not None:
            # Plain-text payload (e.g. SVG); keep the given extension.
            raw = str(text).encode("utf-8")
            if not os.path.splitext(rel)[1]:
                rel += ".txt"
        else:
            data_url = payload.get("dataUrl", "")
            if not rel.lower().endswith(".png"):
                rel += ".png"
            m = _DATA_URL_RE.match(data_url)
            if not m:
                self._send_json(400, {"ok": False, "error": "expected a PNG data URL or text"})
                return
            try:
                raw = base64.b64decode(m.group(1))
            except Exception as e:
                self._send_json(400, {"ok": False, "error": f"decode failed: {e}"})
                return

        target = os.path.normpath(os.path.join(ASSETS_DIR, rel))
        if not target.startswith(os.path.normpath(ASSETS_DIR) + os.sep):
            self._send_json(400, {"ok": False, "error": "path escapes static assets"})
            return

        os.makedirs(os.path.dirname(target), exist_ok=True)
        with open(target, "wb") as f:
            f.write(raw)

        saved_rel = os.path.relpath(target, PROJECT_ROOT)
        print(f"[DESIGNER] saved {saved_rel} ({len(raw):,} bytes)")
        self._send_json(200, {"ok": True, "path": saved_rel, "bytes": len(raw)})

    def log_message(self, fmt, *args):
        pass  # keep the console quiet except for saves


def main():
    os.chdir(PROJECT_ROOT)
    httpd = ThreadingHTTPServer(("127.0.0.1", PORT), DesignerHandler)
    print("Split Flap Artwork Designer server")
    print(f"  Open:  http://localhost:{PORT}/simulator/flap-designer.html")
    print(f"  Saves: {ASSETS_DIR}")
    print("  Ctrl+C to stop.")
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("\nStopped.")


if __name__ == "__main__":
    main()
