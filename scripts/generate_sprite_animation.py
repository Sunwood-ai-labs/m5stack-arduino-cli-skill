#!/usr/bin/env -S uv run
# /// script
# dependencies = [
#   "pillow",
# ]
# ///

from __future__ import annotations

import argparse
import math
from pathlib import Path

from PIL import Image


TRANSPARENT_COLOR = 0x0001


def rgb888_to_rgb565(red: int, green: int, blue: int) -> int:
    return (((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3)) & 0xFFFF


def frame_indices(frame_count: int, step: int) -> list[int]:
    return list(range(0, frame_count, step))


def fit_frame_to_canvas(frame: Image.Image, size: int) -> Image.Image:
    rgba = frame.convert("RGBA")
    if rgba.width == 0 or rgba.height == 0:
        return Image.new("RGBA", (size, size), (0, 0, 0, 0))

    width, height = rgba.size
    scale = min(size / width, size / height)
    fitted_width = max(1, round(width * scale))
    fitted_height = max(1, round(height * scale))
    resized = rgba.resize((fitted_width, fitted_height), Image.Resampling.NEAREST)

    canvas = Image.new("RGBA", (size, size), (0, 0, 0, 0))
    offset_x = (size - fitted_width) // 2
    offset_y = (size - fitted_height) // 2
    canvas.alpha_composite(resized, (offset_x, offset_y))
    return canvas


def convert_frame(frame: Image.Image, size: int, alpha_threshold: int) -> list[int]:
    rgba = fit_frame_to_canvas(frame, size)

    pixels: list[int] = []
    for y in range(rgba.height):
        for x in range(rgba.width):
            red, green, blue, alpha = rgba.getpixel((x, y))
            if alpha < alpha_threshold:
                pixels.append(TRANSPARENT_COLOR)
            else:
                pixels.append(rgb888_to_rgb565(red, green, blue))

    return pixels


def preview_frame(frame: Image.Image, size: int) -> Image.Image:
    rgba = fit_frame_to_canvas(frame, size)
    background = Image.new("RGBA", (size, size), (52, 78, 90, 255))
    background.alpha_composite(rgba)
    return background


def write_header(path: Path, width: int, height: int, frames: list[list[int]]) -> None:
    lines = [
        "#pragma once",
        "",
        "#include <stdint.h>",
        "",
        f"constexpr int kCatFrameWidth = {width};",
        f"constexpr int kCatFrameHeight = {height};",
        f"constexpr int kCatFrameCount = {len(frames)};",
        f"constexpr uint16_t kCatTransparentColor = 0x{TRANSPARENT_COLOR:04X};",
        "",
        "const uint16_t kCatAnimationPixels[kCatFrameCount][kCatFrameWidth * kCatFrameHeight] = {",
    ]

    for frame in frames:
        lines.append("    {")
        for start in range(0, len(frame), 12):
            chunk = ", ".join(f"0x{value:04X}" for value in frame[start:start + 12])
            lines.append(f"        {chunk},")
        lines.append("    },")

    lines.extend(
        [
            "};",
            "",
        ]
    )

    path.write_text("\n".join(lines), encoding="utf-8")


def save_preview_strip(path: Path, frames: list[Image.Image], size: int) -> None:
    columns = len(frames)
    sheet = Image.new("RGBA", (columns * size, size), (52, 78, 90, 255))
    for index, frame in enumerate(frames):
        sheet.alpha_composite(frame, (index * size, 0))
    path.parent.mkdir(parents=True, exist_ok=True)
    sheet.save(path)


def save_sprite_sheet(path: Path, frames: list[Image.Image], size: int, columns: int) -> None:
    rows = math.ceil(len(frames) / columns)
    sheet = Image.new("RGBA", (columns * size, rows * size), (0, 0, 0, 0))
    for index, frame in enumerate(frames):
        x = (index % columns) * size
        y = (index // columns) * size
        sheet.alpha_composite(frame, (x, y))
    path.parent.mkdir(parents=True, exist_ok=True)
    sheet.save(path)


def main() -> None:
    parser = argparse.ArgumentParser(description="Convert an animated WebP into RGB565 animation frames.")
    parser.add_argument("--input", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--preview", type=Path)
    parser.add_argument("--sheet", type=Path)
    parser.add_argument("--size", type=int, default=112)
    parser.add_argument("--frame-step", type=int, default=4)
    parser.add_argument("--preview-frames", type=int, default=8)
    parser.add_argument("--sheet-columns", type=int, default=8)
    parser.add_argument("--alpha-threshold", type=int, default=16)
    args = parser.parse_args()

    image = Image.open(args.input)
    total_frames = getattr(image, "n_frames", 1)
    selected_indices = frame_indices(total_frames, args.frame_step)

    converted_frames: list[list[int]] = []
    preview_frames: list[Image.Image] = []
    sheet_frames: list[Image.Image] = []

    for index in selected_indices:
        image.seek(index)
        frame = image.copy()
        converted_frames.append(convert_frame(frame, args.size, args.alpha_threshold))
        fitted_frame = fit_frame_to_canvas(frame, args.size)
        fitted_preview = preview_frame(frame, args.size)
        sheet_frames.append(fitted_frame)
        if len(preview_frames) < args.preview_frames:
            preview_frames.append(fitted_preview)

    args.output.parent.mkdir(parents=True, exist_ok=True)
    write_header(args.output, args.size, args.size, converted_frames)

    if args.preview:
        save_preview_strip(args.preview, preview_frames, args.size)
    if args.sheet:
        save_sprite_sheet(args.sheet, sheet_frames, args.size, args.sheet_columns)


if __name__ == "__main__":
    main()
