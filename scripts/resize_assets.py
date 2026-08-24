"""
Batch resize flap assets to print dimensions (508x827px @ 300 DPI for 43x70mm).
Creates both PNG and JPEG versions for comparison.

Usage:
    pip install Pillow
    python resize_assets.py

Input:  static assets/Compressed Assets/Column X/
Output: static assets/Compressed Assets/jpg/Column X/
        static assets/Compressed Assets/png/Column X/
"""

import os
from pathlib import Path
from PIL import Image

# --- CONFIG ---
INPUT_BASE = Path(__file__).parent.parent / "static assets" / "Compressed Assets"
OUTPUT_BASE = Path(__file__).parent.parent / "static assets" / "Compressed Assets"

# Print dimensions: 43mm x 70mm @ 300 DPI
TARGET_WIDTH = 508
TARGET_HEIGHT = 827

JPEG_QUALITY = 90  # 85-95 is indistinguishable from lossless at this print size

COLUMNS = ["Column 0", "Column 1", "Column 2", "Column 3", "Column 4"]


def resize_image(input_path, output_path, format="PNG", quality=90):
    """Resize a single image to target dimensions and save."""
    try:
        img = Image.open(input_path)

        # Convert to RGB if saving as JPEG (JPEG doesn't support alpha)
        if format == "JPEG" and img.mode in ("RGBA", "P"):
            # Composite onto black background (our default BG)
            background = Image.new("RGB", img.size, (0, 0, 0))
            if img.mode == "P":
                img = img.convert("RGBA")
            background.paste(img, mask=img.split()[3])  # Use alpha as mask
            img = background
        elif format == "JPEG" and img.mode != "RGB":
            img = img.convert("RGB")

        # Resize to exact print dimensions (stretch to fit — artwork should already be cropped)
        img_resized = img.resize((TARGET_WIDTH, TARGET_HEIGHT), Image.LANCZOS)

        # Save
        if format == "JPEG":
            img_resized.save(output_path, "JPEG", quality=quality, optimize=True)
        else:
            img_resized.save(output_path, "PNG", optimize=True)

        return os.path.getsize(output_path)
    except Exception as e:
        print(f"  ERROR: {input_path.name} — {e}")
        return 0


def process_column(column_name):
    """Process all images in a column folder."""
    input_dir = INPUT_BASE / column_name
    if not input_dir.exists():
        print(f"  SKIP: {input_dir} not found")
        return

    # Create output dirs
    png_dir = OUTPUT_BASE / "png" / column_name
    jpg_dir = OUTPUT_BASE / "jpg" / column_name
    png_dir.mkdir(parents=True, exist_ok=True)
    jpg_dir.mkdir(parents=True, exist_ok=True)

    # Get all image files
    image_files = [f for f in input_dir.iterdir()
                   if f.suffix.lower() in (".png", ".jpg", ".jpeg", ".webp")]

    if not image_files:
        print(f"  No images found in {input_dir}")
        return

    total_png = 0
    total_jpg = 0

    for img_path in sorted(image_files):
        stem = img_path.stem  # filename without extension

        # PNG version
        png_out = png_dir / f"{stem}.png"
        size_png = resize_image(img_path, png_out, format="PNG")
        total_png += size_png

        # JPEG version
        jpg_out = jpg_dir / f"{stem}.jpg"
        size_jpg = resize_image(img_path, jpg_out, format="JPEG", quality=JPEG_QUALITY)
        total_jpg += size_jpg

        print(f"  {stem}: PNG={size_png // 1024}KB, JPG={size_jpg // 1024}KB")

    print(f"\n  {column_name} totals:")
    print(f"    PNG: {total_png // 1024}KB ({len(image_files)} files)")
    print(f"    JPG: {total_jpg // 1024}KB ({len(image_files)} files)")
    return total_png, total_jpg


def main():
    print("=" * 60)
    print("FLAP ASSET RESIZER")
    print(f"Target: {TARGET_WIDTH}x{TARGET_HEIGHT}px (300 DPI @ 43x70mm)")
    print(f"JPEG quality: {JPEG_QUALITY}%")
    print("=" * 60)
    print()

    grand_png = 0
    grand_jpg = 0

    for col in COLUMNS:
        print(f"\n--- {col} ---")
        result = process_column(col)
        if result:
            grand_png += result[0]
            grand_jpg += result[1]

    print("\n" + "=" * 60)
    print("GRAND TOTALS (all columns):")
    print(f"  PNG: {grand_png // 1024}KB ({grand_png / 1024 / 1024:.1f}MB)")
    print(f"  JPG: {grand_jpg // 1024}KB ({grand_jpg / 1024 / 1024:.1f}MB)")
    print(f"  Original: ~147MB")
    print(f"  Savings PNG: ~{100 - (grand_png / 1024 / 1024 / 147 * 100):.0f}%")
    print(f"  Savings JPG: ~{100 - (grand_jpg / 1024 / 1024 / 147 * 100):.0f}%")
    print("=" * 60)


if __name__ == "__main__":
    main()
